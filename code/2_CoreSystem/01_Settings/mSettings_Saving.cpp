
#include "2_CoreSystem/01_Settings/mSettings.h"

#include "2_CoreSystem/06_Support/mSupport.h"


#define EEPROM_LOCATION (SPI_FLASH_SEC_SIZE * 200)
uint32_t settings_location = EEPROM_LOCATION;
uint32_t settings_crc32 = 0;
const uint8_t CFG_ROTATES = 7;      // Number of flash sectors used (handles uploads)


void mSettings::SettingsSaveAll(void)
{
  #ifndef ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  ALOG_ERR(PSTR(D_LOG_SETTINGS "SettingsSaveAll: Not enabled"));
  #endif

  snprintf(Settings.settings_time_ctr, sizeof(Settings.settings_time_ctr), "%02d:%02d:%02d", pCONT_time->RtcTime.hour, pCONT_time->RtcTime.minute, pCONT_time->RtcTime.second);

  Serial.println("SettingsSaveAll -- should only be called prior to planned reboot/restart; OTA start/commanded");
  // if (Settings.flag_system.save_state) {
  //   Settings.power = power;
  // } else {
  //   Settings.power = 0;
  // }
  pCONT->Tasker_Interface(FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE);
  
  SettingsSave(1);
}


uint32_t mSettings::SettingsRead(void *data, size_t size) 
{
  DEBUG_LINE_HERE;
  #ifdef USE_MODULE_DRIVERS_FILESYSTEM
  if (pCONT_mfile->TfsLoadFile(TASM_FILE_SETTINGS, (uint8_t*)data, size)) 
  {
  DEBUG_LINE_HERE;
    return 2;
  }
  #endif
//   if (NvmLoad("main", "Settings", data, size)) {
//     return 1;
//   };
  DEBUG_LINE_HERE;
  return 0;
}


/***
 * Modified version: 
 * - Removing rotating writing until location0 works well
 * - Adding that settings first gets read into a local temporary copy, then a version merges the loaded values into values I want to overwrite.
 *    This is so I can slowly add what I like to have saved in memory until I can trust it full
 * */
void mSettings::SettingsLoad(void) 
{
  
  #ifndef ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  ALOG_ERR(PSTR(D_LOG_SETTINGS "SettingsLoad: Not enabled"));
  #endif

  DEBUG_LINE_HERE;
  
  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_MEMORY D_LOAD ));
  #endif// ENABLE_LOG_LEVEL_INFO

  /****************************************************************************************************************************************************
  ** Check if SETTINGS_HOLDER value has changed from code build, and thus stored settings should be erased ********************************************
  ****************************************************************************************************************************************************/
  
  #ifdef ESP8266  
    // Load configuration from optional file and flash (eeprom and 7 additonal slots) if first valid load does not stop_flash_rotate
    // Activated with version 8.4.0.2 - Fails to read any config before version 6.6.0.11
    settings_location = 0;
    uint32_t save_flag = 0;
    uint32_t max_slots = CFG_ROTATES +1;
    uint32_t flash_location;
    uint32_t slot = 1;
  #ifdef USE_UFILESYS
    if (TfsLoadFile(TASM_FILE_SETTINGS, (uint8_t*)Settings, sizeof(TSettings))) {
      flash_location = 1;
      slot = 0;
    }
  #endif  // USE_UFILESYS
    while (slot <= max_slots) {                                  // Read all config pages in search of valid and latest
      if (slot > 0) {
        flash_location = (1 == slot) ? EEPROM_LOCATION : (2 == slot) ? SETTINGS_LOCATION : flash_location -1;
        // ESP.flashRead(flash_location * SPI_FLASH_SEC_SIZE, &Settings, sizeof(SETTINGS_HOLDER));
      }
      if ((Settings.cfg_crc32 != 0xFFFFFFFF) && (Settings.cfg_crc32 != 0x00000000) && (Settings.cfg_crc32 == GetSettingsCrc32())) {
        if (Settings.save_flag > save_flag) {                    // Find latest page based on incrementing save_flag
          save_flag = Settings.save_flag;
          settings_location = flash_location;
          if (Settings.flag_system.stop_flash_rotate && (1 == slot)) {  // Stop if only eeprom area should be used and it is valid
            break;
          }
        }
      }
      slot++;
      delay(1);
    }
    if (settings_location > 0) {
  #ifdef USE_UFILESYS
      if (1 == settings_location) {
        TfsLoadFile(TASM_FILE_SETTINGS, (uint8_t*)Settings, sizeof(TSettings));
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "Loaded from File, " D_COUNT " %lu"), Settings->save_flag);
      } else
  #endif  // USE_UFILESYS
      {
        // ESP.flashRead(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)Settings, sizeof(TSettings));
        // ESP.flashRead(settings_location * SPI_FLASH_SEC_SIZE, &Settings, sizeof(SETTINGS));
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG D_LOADED_FROM_FLASH_AT " %X, " D_COUNT " %lu"), settings_location, Settings.save_flag);
      }
    }
  #endif // ESP8266
  #ifdef ESP32
    // No temp settings, just fully load as if invalid reset will occur and defaults will be loaded
    uint32_t source = SettingsRead(&Settings, sizeof(SETTINGS));
    if (source) {
      settings_location = 1;
      if (Settings.cfg_holder == (uint16_t)SETTINGS_HOLDER) {
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "Loaded from %s, " D_COUNT " %lu"), (2 == source)?"File":"NVS", Settings.save_flag);
      }
      if(Settings.cfg_size != sizeof(SETTINGS)) {
        AddLog(LOG_LEVEL_HIGHLIGHT, PSTR(D_LOG_CONFIG "Settings size does not match (%d|%d), load invalid"), Settings.cfg_size, sizeof(SETTINGS));
        settings_location = 0; //force default settings load
      }
    }
  #endif  // ESP32

  /****************************************************************************************************************************************************
  ** If stored settings should be erased to defaults (using file or code inits) ********************************************
  ****************************************************************************************************************************************************/

  if ((0 == settings_location) || (Settings.cfg_holder != (uint16_t)SETTINGS_HOLDER)) {  // Init defaults if cfg_holder differs from user settings in my_user_config.h
    /**
     * @brief Two cases may be true here:
     *  (1) IF the original settings file is correct, but no code change occured. THEN use backup settings file to recover, but cfg_size must still match
     *  (2) IF the settings holder has been changed to indicate code change, THEN use code defaults and ignored saved settings. This is essentially a factory reset.
     */
    bool settings_reset_by_defaults = false;
    
    #ifdef USE_MODULE_DRIVERS_FILESYSTEM
    /**
     * Load as temporary settings, to enable testing of cfg_size 
     **/
    SETTINGS settings_temp_load;
    if (pCONT_mfile->TfsLoadFile(TASM_FILE_SETTINGS_LKG_LAST_KNOWN_GOOD, (uint8_t*)&settings_temp_load, sizeof(SETTINGS))) 
    {
      if(settings_temp_load.cfg_size == sizeof(SETTINGS))
      {
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "Loaded from LKG File, " D_COUNT " %lu"), Settings.save_flag);
        memcpy(&Settings, &settings_temp_load, sizeof(SETTINGS));
      }
      else
      {
        AddLog(LOG_LEVEL_HIGHLIGHT, PSTR(D_LOG_CONFIG "LKG Settings size does not match (%d|%d), load invalid"), settings_temp_load.cfg_size, sizeof(SETTINGS));
        settings_reset_by_defaults = true;
      }
    } 
    #endif  // USE_MODULE_DRIVERS_FILESYSTEM

    if(settings_reset_by_defaults)
    {
      AddLog(LOG_LEVEL_HIGHLIGHT, PSTR(D_LOG_CONFIG "Settings reseting to DEFAULT"));
      SettingsDefault();
      runtime.settings_holder_hardcorded_stored_changed = true;
    }

  }
  settings_crc32 = GetSettingsCrc32();

  #ifdef ENABLE_DEVFEATURE_RTC_SETTINGS
  RtcSettingsLoad(1);
  #endif // ENABLE_DEVFEATURE_RTC_SETTINGS

}




void mSettings::SettingsWrite(const void *pSettings, unsigned nSettingsLen) 
{

DEBUG_LINE_HERE;
  #ifdef USE_MODULE_DRIVERS_FILESYSTEM
  pCONT_mfile->TfsSaveFile(TASM_FILE_SETTINGS, (const uint8_t*)pSettings, nSettingsLen);
  #endif // USE_MODULE_DRIVERS_FILESYSTEM
  

DEBUG_LINE_HERE;

  // NvmSave("main", "Settings", pSettings, nSettingsLen);

}

/* Save configuration in eeprom or one of 7 slots below
 *
 * rotate 0 = Save in next flash slot
 * rotate 1 = Save only in eeprom flash slot until SetOption12 0 or restart
 * rotate 2 = Save in eeprom flash slot, erase next flash slots and continue depending on stop_flash_rotate
 * stop_flash_rotate 0 = Allow flash slot rotation (SetOption12 0)
 * stop_flash_rotate 1 = Allow only eeprom flash slot use (SetOption12 1)
 */
void mSettings::SettingsSave(uint8_t rotate)
{
  #ifndef ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  ALOG_ERR(PSTR(D_LOG_SETTINGS "SettingsSave: Not enabled"));
  #endif

  if ((GetSettingsCrc32() != settings_crc32) || rotate) {
    if (1 == rotate) {                                 // Use eeprom flash slot only and disable flash rotate from now on (upgrade)
      runtime.stop_flash_rotate = 1;
    }

    if (runtime.stop_flash_rotate || (2 == rotate)) {  // Use eeprom flash slot and erase next flash slots if stop_flash_rotate is off (default)
      settings_location = EEPROM_LOCATION;
    } else {                                           // Rotate flash slots
      if (settings_location == EEPROM_LOCATION) {
        settings_location = SETTINGS_LOCATION;
      } else {
        settings_location--;
      }
      if (settings_location <= (SETTINGS_LOCATION - CFG_ROTATES)) {
        settings_location = EEPROM_LOCATION;
      }
    }

    Settings.save_flag++;
    if (pCONT_time->Rtc.utc_time > START_VALID_UTC_TIME) {
      Settings.cfg_timestamp = pCONT_time->Rtc.utc_time;
    } else {
      Settings.cfg_timestamp++;
    }
    Settings.cfg_size = sizeof(SETTINGS);
    Settings.cfg_crc32 = GetSettingsCrc32();
#ifdef USE_COUNTER
    CounterInterruptDisable(true);  // may be to disable all interrupts?
#endif

#ifdef ESP8266
#ifdef USE_MODULE_DRIVERS_FILESYSTEM
    // pCONT_mfile->TfsSaveFile(TASM_FILE_SETTINGS, &Settings, sizeof(SETTINGS));
#endif  // USE_MODULE_DRIVERS_FILESYSTEM
    // if (ESP.flashEraseSector(settings_location)) {
    //   ESP.flashWrite(settings_location * SPI_FLASH_SEC_SIZE, &Settings, sizeof(SETTINGS));
    // }

    if (!Settings.flag_system.stop_flash_rotate && rotate) {  // SetOption12 - (Settings) Switch between dynamic (0) or fixed (1) slot flash save location
      for (uint32_t i = 0; i < CFG_ROTATES; i++) {
        ESP.flashEraseSector(SETTINGS_LOCATION -i);    // Delete previous configurations by resetting to 0xFF
        delay(1);
      }
    }
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_CONFIG D_SAVED_TO_FLASH_AT " %X, " D_COUNT " %d, " D_BYTES " %d"), settings_location, Settings.save_flag, sizeof(SETTINGS));
#endif  // ESP8266
#ifdef ESP32
    SettingsWrite(&Settings, sizeof(SETTINGS));
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_CONFIG "Saved, " D_COUNT " %d, " D_BYTES " %d"), Settings.save_flag, sizeof(SETTINGS));
#endif  // ESP32

    settings_crc32 = Settings.cfg_crc32;
  }

  #ifdef ENABLE_DEVFEATURE_RTC_SETTINGS
  RtcSettingsSave();
  #endif
  
}

uint32_t mSettings::GetCfgCrc32(uint8_t *bytes, uint32_t size)
{
  // https://create.stephan-brumme.com/crc32/#bitwise
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
  return GetCfgCrc32((uint8_t*)&Settings, sizeof(Settings) -4);  // Skip crc32
}


/**
 * Test load and print critical test points from primary storage location
 * */
void mSettings::TestSettingsLoad()
{
    #ifdef ESP8266
  SETTINGS settings_tmp;

  settings_location = SETTINGS_LOCATION;
  
  ESP.flashRead(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)&settings_tmp, sizeof(settings_tmp));

    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_TEST, PSTR("TestSettingsLoad \t\t\t\t TestSettingsLoad\n\r"
      "cfg_holder\t\t\t%d\n\r"
      "cfg_size\t\t\t%d\n\r"
      "save_flag\t\t\t%d\n\r"
      "version\t\t\t%d\n\r"
      "bootcount\t\t\t%d\n\r"
      "cfg_crc\t\t\t%d\n\r"
    ),
      settings_tmp.cfg_holder,
      settings_tmp.cfg_size,
      settings_tmp.save_flag,
      settings_tmp.version,
      settings_tmp.bootcount,
      0
  );
    #endif // ESP8266
  
    #endif // ENABLE_LOG_LEVEL_INFO
}


void mSettings::TestSettings_ShowLocal_Header()
{

    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_TEST, PSTR("TestSettings_ShowLocal_Header\n\r"
      "cfg_holder\t\t\t%d\n\r"
      "cfg_size\t\t\t%d\n\r"
      "save_flag\t\t\t%d\n\r"
      "version\t\t\t%d\n\r"
      "bootcount\t\t\t%d\n\r"
      "cfg_crc\t\t\t%d\n\r"
    ),
      Settings.cfg_holder,
      Settings.cfg_size,
      Settings.save_flag,
      Settings.version,
      Settings.bootcount,
      0
  );
    #endif// ENABLE_LOG_LEVEL_INFO
  
}


uint32_t mSettings::GetSettingsAddress(void)
{
  #ifdef ESP8266
  return settings_location * SPI_FLASH_SEC_SIZE;
  #else
  return 0;
  #endif
}

void mSettings::SettingsErase(uint8_t type)
{
  /*
    0 = Erase from program end until end of physical flash
    1 = Erase SDK parameter area at end of linker memory model (0x0FDxxx - 0x0FFFFF) solving possible wifi errors
  */

#ifndef FIRMWARE_MINIMAL
  bool result;

  uint32_t _sectorStart = (ESP.getSketchSize() / SPI_FLASH_SEC_SIZE) + 1;
  uint32_t _sectorEnd = ESP.getFlashChipRealSize() / SPI_FLASH_SEC_SIZE;
  if (1 == type) {
    _sectorStart = SETTINGS_LOCATION +2;  // SDK parameter area above EEPROM area (0x0FDxxx - 0x0FFFFF)
    _sectorEnd = SETTINGS_LOCATION +5;
  }

  bool _serialoutput = (LOG_LEVEL_DEBUG_MORE <= runtime.seriallog_level);

    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_ERASE " %d " D_UNIT_SECTORS), _sectorEnd - _sectorStart);
    #endif// ENABLE_LOG_LEVEL_INFO

  for (uint32_t _sector = _sectorStart; _sector < _sectorEnd; _sector++) {
    result = ESP.flashEraseSector(_sector);
    if (_serialoutput) {
      Serial.print(F(D_LOG_APPLICATION D_ERASED_SECTOR " "));
      Serial.print(_sector);
      if (result) {
        Serial.println(F(" " D_OK));
      } else {
        Serial.println(F(" " D_ERROR));
      }
      delay(10);
    }
    //OsWatchLoop();
  }
#endif  // FIRMWARE_MINIMAL
}

// Copied from 2.4.0 as 2.3.0 is incomplete
bool mSettings::SettingsEraseConfig(void) 
{
  const size_t cfgSize = 0x4000;
  size_t cfgAddr = ESP.getFlashChipSize() - cfgSize;

  for (size_t offset = 0; offset < cfgSize; offset += SPI_FLASH_SEC_SIZE) {
    if (!ESP.flashEraseSector((cfgAddr + offset) / SPI_FLASH_SEC_SIZE)) {
      return false;
    }
  }
  return true;
}

void mSettings::SettingsSdkErase(void)
{
//   WiFi.disconnect(true);    // Delete SDK wifi config
//   SettingsErase(1);
//   SettingsEraseConfig();
//   delay(1000);
}

