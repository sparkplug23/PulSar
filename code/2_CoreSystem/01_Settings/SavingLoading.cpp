/*****
 *
 * Settings saving/loading.
 *
 * PulSar policy:
 * - Filesystem/TFS is the only persistent backend for the full SETTINGS blob.
 * - ESP32 and ESP8266 both use /settings.txt when the filesystem is available.
 * - NVS/NVM is not used for SETTINGS.
 * - Raw ESP8266 flash-sector rotation is not used.
 * - RTC memory remains separate and is used for fastboot/quick boot state only.
 *
 * Files:
 * - /settings.txt      active binary SETTINGS struct
 * - /settings_prev.txt previous binary SETTINGS struct saved before replacement
 * - /settings_lkg.txt  last-known-good binary SETTINGS struct
 *
 *****/

#include "2_CoreSystem/01_Settings/mSettings.h"
#include "2_CoreSystem/06_Support/mSupport.h"

#ifndef TASM_FILE_SETTINGS_PREV
#define TASM_FILE_SETTINGS_PREV "/settings_prev.txt"
#endif

uint32_t settings_location = 0;     // Filesystem backend: logical source marker only
uint32_t settings_crc32    = 0;     // CRC of current in-RAM Settings after last successful save/load


#ifdef USE_MODULE_CORE_FILESYSTEM
static void SettingsSavePrevFile_FromBuffer(const mSettings::SETTINGS *settings_ptr)
{
  if (settings_ptr == nullptr) {
    return;
  }

  #ifdef ENABLE_DEVFEATURE_SETTINGS__BLOCK_USER_CONFIG_SAVE
  ALOG_WRN(PSTR(D_LOG_SETTINGS "Settings prev save blocked"));
  return;
  #endif

  tkr_mfile->SaveFile(
    TASM_FILE_SETTINGS_PREV,
    (const uint8_t*)settings_ptr,
    sizeof(mSettings::SETTINGS)
  );

  ALOG_WRN(PSTR(D_LOG_SETTINGS "Saved previous settings to " TASM_FILE_SETTINGS_PREV));
}
#endif


static void SettingsUpdateRuntimeHostname(void)
{
  memset(tkr_set->runtime.my_hostname, 0, sizeof(tkr_set->runtime.my_hostname));
  snprintf(
    tkr_set->runtime.my_hostname,
    sizeof(tkr_set->runtime.my_hostname),
    PSTR("%s"),
    tkr_set->Settings.system_name.device
  );
}


static void SettingsEpochToUtcDateTime(
  uint32_t utc,
  uint8_t *hour,
  uint8_t *minute,
  uint8_t *second,
  uint8_t *day,
  uint8_t *month,
  uint8_t *year_2digit
)
{
  const uint32_t seconds_per_day = 86400UL;

  uint32_t days = utc / seconds_per_day;
  uint32_t sod  = utc % seconds_per_day;

  *hour   = sod / 3600UL;
  sod    %= 3600UL;
  *minute = sod / 60UL;
  *second = sod % 60UL;

  // Civil date conversion for days since 1970-01-01.
  // Based on Howard Hinnant's civil_from_days algorithm, reduced for UTC date extraction.
  int32_t z = (int32_t)days + 719468;
  int32_t era = (z >= 0 ? z : z - 146096) / 146097;
  uint32_t doe = (uint32_t)(z - era * 146097);                                      // [0, 146096]
  uint32_t yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;              // [0, 399]
  int32_t y = (int32_t)yoe + era * 400;
  uint32_t doy = doe - (365 * yoe + yoe / 4 - yoe / 100);                            // [0, 365]
  uint32_t mp = (5 * doy + 2) / 153;                                                  // [0, 11]
  uint32_t d = doy - (153 * mp + 2) / 5 + 1;                                          // [1, 31]
  uint32_t m = mp + (mp < 10 ? 3 : -9);                                               // [1, 12]

  y += (m <= 2);

  *day         = (uint8_t)d;
  *month       = (uint8_t)m;
  *year_2digit = (uint8_t)(y % 100);
}


void mSettings::SettingsUpdateFileWriteTimeAscii(void)
{
  // Default visible value when UTC is not valid yet.
  snprintf(Settings.settings_file_update_utc_ascii,sizeof(Settings.settings_file_update_utc_ascii),PSTR("utc------:------"));

  const uint32_t utc = tkr_time->UtcTime();

  if (utc <= START_VALID_UTC_TIME) {
    return;
  }

  uint8_t hour = 0;
  uint8_t minute = 0;
  uint8_t second = 0;
  uint8_t day = 0;
  uint8_t month = 0;
  uint8_t year_2digit = 0;

  SettingsEpochToUtcDateTime(
    utc,
    &hour,
    &minute,
    &second,
    &day,
    &month,
    &year_2digit
  );

  // Format: utcHHMMSS:DDMMYY
  // Example: utc142305:130526
  snprintf(
    Settings.settings_file_update_utc_ascii,
    sizeof(Settings.settings_file_update_utc_ascii),
    PSTR("utc%02u%02u%02u:%02u%02u%02u"),
    hour,
    minute,
    second,
    day,
    month,
    year_2digit
  );
}


void mSettings::SettingsSaveAll(void)
{

  #ifdef ENABLE_DEVFEATURE_SETTINGS__BLOCK_USER_CONFIG_SAVE
  ALOG_WRN(PSTR(D_LOG_SETTINGS "SettingsSaveAll blocked"));
  return;
  #endif

  if (Settings.sysopt_system.bit.save_state) {
    Settings.power = runtime.power;
  } else {
    Settings.power = 0;
  }

  // Modules must only copy persistent values here, not high-churn telemetry.
  tkr->Tasker_Interface(TASK_SETTINGS_SAVE_VALUES_FROM_MODULE);

  SettingsSave(0);
}

/************************************************************************************************
 * FUNCTION: SettingsLoad
 *
 * SUMMARY:
 * - Loads persisted settings if valid.
 * - If persisted settings are missing/invalid, rebuilds defaults, applies compile-time
 *   MODULE_TEMPLATE as default provisioning, then saves the result.
 * - If USE_MODULE_TEMPLATE__OVERRIDE is enabled, applies MODULE_TEMPLATE again after the
 *   loaded/default state has been established, then saves the enforced result.
 *
 * BOOT POLICY:
 * - Valid saved settings:
 *     Use saved settings.
 *     Ignore compile-time MODULE_TEMPLATE unless USE_MODULE_TEMPLATE__OVERRIDE is defined.
 *
 * - Missing/invalid saved settings:
 *     SettingsDefault().
 *     Apply MODULE_TEMPLATE default pass.
 *     Save generated settings.
 *
 * - Override build:
 *     Apply MODULE_TEMPLATE override pass after saved/default settings are loaded.
 *     Save enforced settings so the file reflects the active state.
 *
 * IMPORTANT:
 * - This replaces the old setup-level TASK_TEMPLATES__LOAD_MODULE sequencing.
 * - setup() should not separately call TASK_TEMPLATES__LOAD_MODULE for normal boot anymore.
 *
 * CHANGED:
 * - 17May26: Moved early MODULE_TEMPLATE sequencing into SettingsLoad().
 ************************************************************************************************/
void mSettings::SettingsLoad(void)
{
  ALOG_INF(PSTR(D_LOG_MEMORY D_LOAD));

  settings_location = 0;

  bool settings_loaded_from_valid_file = false;
  bool settings_generated_from_defaults = false;
  bool settings_recovered_from_lkg = false;
  bool settings_should_save = false;

  /************************************************************************************************
   * SECTION: Forced compiled defaults path
   *
   * Used for development or deliberate bypass of user-defined saved settings.
   * Treat this like a generated-default path, so MODULE_TEMPLATE default provisioning applies.
   ************************************************************************************************/

  #ifdef ENABLE_FEATURE_SETTINGS__IGNORE_USER_DEFINED_FILES
  {
    ALOG_WRN(PSTR(D_LOG_SETTINGS "SettingsLoad blocked, using compiled RAM defaults"));

    SettingsDefault();
    runtime.settings_holder_hardcorded_stored_changed = true;
    settings_generated_from_defaults = true;

    #ifdef USE_MODULE_CORE_JSON_TEMPLATE
    if (tkr_json_template) {
      if (tkr_json_template->ModuleDeviceTemplate_CompileTime_Load_ModuleOnly(false)) {
        settings_should_save = true;
      }
    }
    #endif

    settings_should_save = true;
  }
  #else


  /************************************************************************************************
   * SECTION: Forced precoded/default path
   *
   * Used when the build explicitly ignores saved settings and reloads compiled defaults.
   ************************************************************************************************/

  #ifdef ENABLE_FEATURE_SETTINGS__LOAD_PRECODED_SETTINGS_ON_BOOT_NO_SAVED_STATES
  {
    ALOG_WRN(PSTR(D_LOG_SETTINGS "Loading compiled defaults, saved settings ignored"));

    SettingsDefault();
    runtime.settings_holder_hardcorded_stored_changed = true;
    settings_generated_from_defaults = true;

    #ifdef USE_MODULE_CORE_JSON_TEMPLATE
    if (tkr_json_template) {
      if (tkr_json_template->ModuleDeviceTemplate_CompileTime_Load_ModuleOnly(false)) {
        settings_should_save = true;
      }
    }
    #endif

    settings_should_save = true;
  }
  #else


  /************************************************************************************************
   * SECTION: Read saved settings
   ************************************************************************************************/

  SETTINGS loaded_settings;
  memset(&loaded_settings, 0x00, sizeof(SETTINGS));

  const uint32_t source = SettingsRead(&loaded_settings, sizeof(SETTINGS));

  if (!source)
  {
    /************************************************************************************************
     * SECTION: No saved settings found
     *
     * Build defaults, apply compile-time module template as provisioning, then save.
     ************************************************************************************************/

    ALOG_WRN(PSTR(D_LOG_SETTINGS "No saved settings found, using compiled defaults"));

    SettingsDefault();
    runtime.settings_holder_hardcorded_stored_changed = true;
    settings_generated_from_defaults = true;

    #ifdef USE_MODULE_CORE_JSON_TEMPLATE
    if (tkr_json_template) {
      if (tkr_json_template->ModuleDeviceTemplate_CompileTime_Load_ModuleOnly(false)) {
        settings_should_save = true;
      }
    }
    #endif

    settings_should_save = true;
  }
  else
  {
    settings_location = source;

    const uint32_t loaded_crc = GetCfgCrc32((uint8_t*)&loaded_settings, sizeof(SETTINGS) - 4);

    const bool size_valid =
      (loaded_settings.cfg_size == sizeof(SETTINGS));

    const bool holder_valid =
      (loaded_settings.cfg_holder == (uint16_t)SETTINGS_HOLDER);

    const bool crc_nonblank =
      (loaded_settings.cfg_crc32 != 0xFFFFFFFF) &&
      (loaded_settings.cfg_crc32 != 0x00000000);

    const bool crc_valid =
      crc_nonblank &&
      (loaded_settings.cfg_crc32 == loaded_crc);

    ALOG_INF(
      PSTR(D_LOG_SETTINGS "LoadCheck src=%d holder=%d expected=%d size=%d expected_size=%d crc_valid=%d"),
      source,
      loaded_settings.cfg_holder,
      (uint16_t)SETTINGS_HOLDER,
      loaded_settings.cfg_size,
      sizeof(SETTINGS),
      crc_valid
    );

    /************************************************************************************************
     * SECTION: Valid saved settings
     ************************************************************************************************/

    if (size_valid && holder_valid && crc_valid)
    {
      memcpy(&Settings, &loaded_settings, sizeof(SETTINGS));

      ALOG_INF(
        PSTR(D_LOG_CONFIG "Loaded from file, " D_COUNT " %lu"),
        Settings.save_flag
      );

      settings_loaded_from_valid_file = true;
    }
    else
    {
      /************************************************************************************************
       * SECTION: Invalid saved settings
       *
       * Save the invalid binary before replacing it.
       ************************************************************************************************/

      #ifdef USE_MODULE_CORE_FILESYSTEM
      SettingsSavePrevFile_FromBuffer(&loaded_settings);
      #endif


      /************************************************************************************************
       * SECTION: Holder mismatch
       ************************************************************************************************/

      if (!holder_valid)
      {
        ALOG_WRN(
          PSTR(D_LOG_SETTINGS "Settings holder mismatch, using compiled defaults (%d != %d)"),
          loaded_settings.cfg_holder,
          (uint16_t)SETTINGS_HOLDER
        );

        SettingsDefault();
        runtime.settings_holder_hardcorded_stored_changed = true;
        settings_generated_from_defaults = true;

        #ifdef USE_MODULE_CORE_JSON_TEMPLATE
        if (tkr_json_template) {
          if (tkr_json_template->ModuleDeviceTemplate_CompileTime_Load_ModuleOnly(false)) {
            settings_should_save = true;
          }
        }
        #endif

        settings_should_save = true;
      }
      else
      if (!size_valid)
      {
        /************************************************************************************************
         * SECTION: Size mismatch
         ************************************************************************************************/

        ALOG_WRN(
          PSTR(D_LOG_SETTINGS "Settings size mismatch, using compiled defaults (%d != %d)"),
          loaded_settings.cfg_size,
          sizeof(SETTINGS)
        );

        SettingsDefault();
        runtime.settings_holder_hardcorded_stored_changed = true;
        settings_generated_from_defaults = true;

        #ifdef USE_MODULE_CORE_JSON_TEMPLATE
        if (tkr_json_template) {
          if (tkr_json_template->ModuleDeviceTemplate_CompileTime_Load_ModuleOnly(false)) {
            settings_should_save = true;
          }
        }
        #endif

        settings_should_save = true;
      }
      else
      if (!crc_valid)
      {
        /************************************************************************************************
         * SECTION: CRC invalid, try last-known-good before rebuilding defaults
         ************************************************************************************************/

        ALOG_WRN(PSTR(D_LOG_SETTINGS "Settings CRC invalid"));

        #ifdef USE_MODULE_CORE_FILESYSTEM
        SETTINGS lkg_settings;
        memset(&lkg_settings, 0x00, sizeof(SETTINGS));

        if (tkr_mfile->LoadFile(TASM_FILE_SETTINGS_LKG_LAST_KNOWN_GOOD, (uint8_t*)&lkg_settings, sizeof(SETTINGS)))
        {
          const uint32_t lkg_crc = GetCfgCrc32((uint8_t*)&lkg_settings, sizeof(SETTINGS) - 4);

          const bool lkg_valid =
            (lkg_settings.cfg_size == sizeof(SETTINGS)) &&
            (lkg_settings.cfg_holder == (uint16_t)SETTINGS_HOLDER) &&
            (lkg_settings.cfg_crc32 != 0xFFFFFFFF) &&
            (lkg_settings.cfg_crc32 != 0x00000000) &&
            (lkg_settings.cfg_crc32 == lkg_crc);

          if (lkg_valid)
          {
            memcpy(&Settings, &lkg_settings, sizeof(SETTINGS));

            ALOG_WRN(PSTR(D_LOG_SETTINGS "Recovered settings from last-known-good file"));

            settings_recovered_from_lkg = true;
            settings_should_save = true;
          }
          else
          {
            ALOG_WRN(PSTR(D_LOG_SETTINGS "Last-known-good settings invalid"));
          }
        }
        #endif

        if (!settings_recovered_from_lkg)
        {
          SettingsDefault();
          runtime.settings_holder_hardcorded_stored_changed = true;
          settings_generated_from_defaults = true;

          #ifdef USE_MODULE_CORE_JSON_TEMPLATE
          if (tkr_json_template) {
            if (tkr_json_template->ModuleDeviceTemplate_CompileTime_Load_ModuleOnly(false)) {
              settings_should_save = true;
            }
          }
          #endif

          settings_should_save = true;
        }
      }
      else
      {
        /************************************************************************************************
         * SECTION: Defensive fallback
         ************************************************************************************************/

        ALOG_ERR(PSTR(D_LOG_SETTINGS "Unhandled invalid SettingsLoad state, using compiled defaults"));

        SettingsDefault();
        runtime.settings_holder_hardcorded_stored_changed = true;
        settings_generated_from_defaults = true;

        #ifdef USE_MODULE_CORE_JSON_TEMPLATE
        if (tkr_json_template) {
          if (tkr_json_template->ModuleDeviceTemplate_CompileTime_Load_ModuleOnly(false)) {
            settings_should_save = true;
          }
        }
        #endif

        settings_should_save = true;
      }
    }
  }

  #endif // ENABLE_FEATURE_SETTINGS__LOAD_PRECODED_SETTINGS_ON_BOOT_NO_SAVED_STATES
  #endif // ENABLE_FEATURE_SETTINGS__IGNORE_USER_DEFINED_FILES


  /************************************************************************************************
   * SECTION: Compile-time MODULE_TEMPLATE override pass
   *
   * This is intentionally after the saved/default/LKG state is established.
   *
   * If USE_MODULE_TEMPLATE__OVERRIDE is not defined, the called function will skip.
   * If it is defined, MODULE_TEMPLATE is enforced over the loaded/generated settings.
   ************************************************************************************************/

  #ifdef USE_MODULE_CORE_JSON_TEMPLATE
  if (tkr_json_template) {
    if (tkr_json_template->ModuleDeviceTemplate_CompileTime_Load_ModuleOnly(true)) {
      settings_should_save = true;

      ALOG_WRN(PSTR(D_LOG_SETTINGS "MODULE_TEMPLATE override pass applied"));
    }
  }
  #endif


  /************************************************************************************************
   * SECTION: Save generated/modified settings
   *
   * Save when:
   * - settings were generated from defaults
   * - LKG recovery happened
   * - module template default/override changed the RAM settings
   ************************************************************************************************/

  if (settings_should_save)
  {
    SettingsSaveAll();
  }


  /************************************************************************************************
   * SECTION: Finalise runtime metadata
   ************************************************************************************************/

  settings_crc32 = GetSettingsCrc32();

  #ifdef ENABLE_DEVFEATURE_RTC_SETTINGS
  RtcSettingsLoad(1);
  #endif

  SettingsUpdateRuntimeHostname();

  ALOG_INF(
    PSTR(D_LOG_SETTINGS "SettingsLoad complete valid_file=%u defaults=%u lkg=%u saved=%u crc=0x%08X"),
    (unsigned)settings_loaded_from_valid_file,
    (unsigned)settings_generated_from_defaults,
    (unsigned)settings_recovered_from_lkg,
    (unsigned)settings_should_save,
    settings_crc32
  );
}


uint32_t mSettings::SettingsRead(void *data, size_t size)
{
  #ifdef ENABLE_FEATURE_SETTINGS__IGNORE_USER_DEFINED_FILES
  ALOG_WRN(PSTR(D_LOG_SETTINGS "SettingsRead blocked"));
  return 0;
  #endif

  #ifdef USE_MODULE_CORE_FILESYSTEM
  if (tkr_mfile->LoadFile(TASM_FILE_SETTINGS, (uint8_t*)data, size)) {
    return 1;
  }
  #endif

  return 0;
}


void mSettings::SettingsWrite(const void *pSettings, unsigned nSettingsLen)
{
  #ifdef ENABLE_DEVFEATURE_SETTINGS__BLOCK_USER_CONFIG_SAVE
  ALOG_WRN(PSTR(D_LOG_SETTINGS "SettingsWrite blocked"));
  return;
  #endif

  #ifdef USE_MODULE_CORE_FILESYSTEM
  tkr_mfile->SaveFile(TASM_FILE_SETTINGS, (const uint8_t*)pSettings, nSettingsLen);
  #else
  ALOG_WRN(PSTR(D_LOG_SETTINGS "SettingsWrite skipped, TFS disabled"));
  #endif
}


/*
 * Save configuration.
 *
 * Filesystem-only policy:
 *   - ESP32 and ESP8266 both write /settings.txt through TFS.
 *   - No raw flash-sector rotation is used.
 *   - No NVS/NVM SETTINGS blob is used.
 *
 * rotate is retained as legacy API naming:
 *   rotate == 0: save only if Settings CRC changed.
 *   rotate != 0: force save.
 */
void mSettings::SettingsSave(uint8_t rotate)
{

  #ifdef ENABLE_DEVFEATURE_SETTINGS__BLOCK_USER_CONFIG_SAVE
  ALOG_WRN(PSTR(D_LOG_SETTINGS "SettingsSave blocked"));
  return;
  #endif

  const bool force_save = (rotate != 0);

  if ((GetSettingsCrc32() != settings_crc32) || force_save) {
    Settings.save_flag++;

    if (tkr_time->Rtc.utc_time > START_VALID_UTC_TIME) {
      Settings.cfg_timestamp = tkr_time->Rtc.utc_time;
    } else {
      Settings.cfg_timestamp++;
    }

    Settings.cfg_holder = (uint16_t)SETTINGS_HOLDER;
    Settings.cfg_size   = sizeof(SETTINGS);
    Settings.version    = PROJECT_VERSION;

    SettingsUpdateFileWriteTimeAscii();

    Settings.cfg_crc32 = GetSettingsCrc32();

    #ifdef USE_COUNTER
    CounterInterruptDisable(true);
    #endif

    SettingsWrite(&Settings, sizeof(SETTINGS));

    ALOG_DBG(
      PSTR(D_LOG_CONFIG "Saved to file, " D_COUNT " %d, " D_BYTES " %d"),
      Settings.save_flag,
      sizeof(SETTINGS)
    );

    settings_crc32 = Settings.cfg_crc32;
  }

  #ifdef ENABLE_DEVFEATURE_RTC_SETTINGS
  RtcSettingsSave();
  #endif
}


uint32_t mSettings::GetCfgCrc32(uint8_t *bytes, uint32_t size)
{
  uint32_t crc = 0;

  while (size--) {
    crc ^= *bytes++;

    for (uint32_t j = 0; j < 8; j++) {
      crc = (crc >> 1) ^ (-int(crc & 1) & 0xEDB88320);
    }
  }

  return ~crc;
}


uint32_t mSettings::GetSettingsCrc32(void)
{
  return GetCfgCrc32((uint8_t*)&Settings, sizeof(Settings) - 4);
}


void mSettings::TestSettingsLoad()
{
  SETTINGS settings_tmp;
  memset(&settings_tmp, 0x00, sizeof(settings_tmp));

  if (SettingsRead(&settings_tmp, sizeof(settings_tmp))) {
    ALOG_TST(
      PSTR("TestSettingsLoad \t\t\t\t TestSettingsLoad\n\r"
        "cfg_holder\t\t\t%d\n\r"
        "cfg_size\t\t\t%d\n\r"
        "save_flag\t\t\t%d\n\r"
        "version\t\t\t%d\n\r"
        "bootcount\t\t\t%d\n\r"
        "file_time\t\t\t%s\n\r"
        "cfg_crc\t\t\t%d\n\r"
      ),
      settings_tmp.cfg_holder,
      settings_tmp.cfg_size,
      settings_tmp.save_flag,
      settings_tmp.version,
      settings_tmp.bootcount,
      settings_tmp.settings_file_update_utc_ascii,
      settings_tmp.cfg_crc32
    );
  } else {
    ALOG_TST(PSTR("TestSettingsLoad: no settings file found"));
  }
}


void mSettings::TestSettings_ShowLocal_Header()
{
  ALOG_TST(
    PSTR("TestSettings_ShowLocal_Header\n\r"
      "cfg_holder\t\t\t%d\n\r"
      "cfg_size\t\t\t%d\n\r"
      "save_flag\t\t\t%d\n\r"
      "version\t\t\t%d\n\r"
      "bootcount\t\t\t%d\n\r"
      "file_time\t\t\t%s\n\r"
      "cfg_crc\t\t\t%d\n\r"
    ),
    Settings.cfg_holder,
    Settings.cfg_size,
    Settings.save_flag,
    Settings.version,
    Settings.bootcount,
    Settings.settings_file_update_utc_ascii,
    Settings.cfg_crc32
  );
}


uint32_t mSettings::GetSettingsAddress(void)
{
  // Filesystem-only settings do not have a raw flash address.
  return 0;
}


void mSettings::SettingsErase(uint8_t type)
{
  switch (type) {
    case 0:
    case 2:
    case 3:
    default:
    {
      int32_t result = 0;

      #ifdef USE_MODULE_CORE_FILESYSTEM
      result = tkr_mfile->DeleteFile(TASM_FILE_SETTINGS);
      #endif

      ALOG_DBG(PSTR(D_LOG_APPLICATION D_ERASE " settings file (%d)"), result);
      break;
    }

    case 1:
    case 4:
    {
      // Keep SDK/PHY erase separate from application settings storage.
      #ifdef ESP32
      int32_t result = esp_phy_erase_cal_data_in_nvs();
      ALOG_DBG(PSTR(D_LOG_APPLICATION D_ERASE " PHY data (%d)"), result);
      #else
      ALOG_DBG(PSTR(D_LOG_APPLICATION D_ERASE " PHY erase skipped on this platform"));
      #endif
      break;
    }
  }
}


bool mSettings::SettingsEraseConfig(void)
{
  // Filesystem-only settings have no raw application config sector to erase.
  SettingsErase(3);
  return true;
}


void mSettings::SettingsSdkErase(void)
{
  WiFi.disconnect(true);
  SettingsErase(1);
  delay(1000);
}


bool mSettings::SaveSettings__LastKnownGood(void)
{
#ifndef USE_MODULE_CORE_FILESYSTEM
  return false;
#endif

#ifndef USE_MODULE_CORE_FILESYSTEM
  return false;
#endif

#ifdef ENABLE_DEVFEATURE_SETTINGS__BLOCK_USER_CONFIG_SAVE
  ALOG_WRN(PSTR(D_LOG_SETTINGS "Last-known-good save blocked"));
  return false;
#endif

  // Save last-known-good once after a stable period.
  // Do not save this early, otherwise a boot-looping configuration could be
  // incorrectly blessed as good.
  if (runtime.settings_lkg) {
    return false;
  }

  if (millis() <= 3600000UL) {
    return false;
  }

  if (tkr_time->UtcTime() <= START_VALID_UTC_TIME) {
    return false;
  }

  // Make the binary visibly inspectable before CRC is updated.
  // This keeps /settings_lkg.txt internally valid too.
  Settings.cfg_holder = (uint16_t)SETTINGS_HOLDER;
  Settings.cfg_size   = sizeof(SETTINGS);
  Settings.version    = PROJECT_VERSION;

  SettingsUpdateFileWriteTimeAscii();

  Settings.cfg_crc32 = GetSettingsCrc32();

  tkr_mfile->SaveFile(
    TASM_FILE_SETTINGS_LKG_LAST_KNOWN_GOOD,
    (const uint8_t*)&Settings,
    sizeof(SETTINGS)
  );

  runtime.settings_lkg = true;

  ALOG_INF(PSTR(D_LOG_SETTINGS "Saved last-known-good settings"));

  return true;
}

bool mSettings::LoadSettings__RestoreFrom_LastKnownGood(void)
{
#ifndef USE_MODULE_CORE_FILESYSTEM
  return false;
#endif

#ifndef USE_MODULE_CORE_FILESYSTEM
  return false;
#endif

#ifdef ENABLE_FEATURE_SETTINGS__IGNORE_USER_DEFINED_FILES
  ALOG_WRN(PSTR(D_LOG_SETTINGS "Last-known-good load blocked"));
  return false;
#endif

  SETTINGS settings_lkg;
  memset(&settings_lkg, 0x00, sizeof(SETTINGS));

  if (!tkr_mfile->LoadFile(
        TASM_FILE_SETTINGS_LKG_LAST_KNOWN_GOOD,
        (uint8_t*)&settings_lkg,
        sizeof(SETTINGS)
      ))
  {
    ALOG_WRN(PSTR(D_LOG_SETTINGS "No last-known-good settings file"));
    return false;
  }

  const uint32_t lkg_crc = GetCfgCrc32(
    (uint8_t*)&settings_lkg,
    sizeof(SETTINGS) - 4
  );

  const bool size_valid =
    (settings_lkg.cfg_size == sizeof(SETTINGS));

  const bool holder_valid =
    (settings_lkg.cfg_holder == (uint16_t)SETTINGS_HOLDER);

  const bool crc_nonblank =
    (settings_lkg.cfg_crc32 != 0xFFFFFFFF) &&
    (settings_lkg.cfg_crc32 != 0x00000000);

  const bool crc_valid =
    crc_nonblank &&
    (settings_lkg.cfg_crc32 == lkg_crc);

  if (!size_valid) {
    ALOG_WRN(
      PSTR(D_LOG_SETTINGS "LKG settings size mismatch (%d != %d)"),
      settings_lkg.cfg_size,
      sizeof(SETTINGS)
    );
    return false;
  }

  if (!holder_valid) {
    ALOG_WRN(
      PSTR(D_LOG_SETTINGS "LKG settings holder mismatch (%d != %d)"),
      settings_lkg.cfg_holder,
      (uint16_t)SETTINGS_HOLDER
    );
    return false;
  }

  if (!crc_valid) {
    ALOG_WRN(PSTR(D_LOG_SETTINGS "LKG settings CRC invalid"));
    return false;
  }

  memcpy(&Settings, &settings_lkg, sizeof(SETTINGS));

  settings_crc32 = GetSettingsCrc32();

  ALOG_WRN(
    PSTR(D_LOG_SETTINGS "Restored settings from last-known-good, save_flag=%lu"),
    Settings.save_flag
  );

  return true;
}