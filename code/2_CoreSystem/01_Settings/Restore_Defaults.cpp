#include "2_CoreSystem/01_Settings/mSettings.h"


void mSettings::SettingsInit(void)
{
  DeviceNameListI->Init(
    Settings.device_name_buffer.name_buffer,
    sizeof(Settings.device_name_buffer.name_buffer),
    Settings.device_name_buffer.class_id,
    Settings.device_name_buffer.device_id,
    DEVICENAMEBUFFER_NAME_INDEX_LENGTH
  );
}


/**
 * @brief Build compiled default settings into RAM only.
 *
 * IMPORTANT:
 * - This function must NOT save settings.
 * - This function must NOT erase stored settings.
 * - This function is called before SettingsLoad() on every boot.
 *
 * Intended boot order:
 *   SettingsInit()
 *   SettingsDefault()       // RAM defaults only
 *   SettingsLoad()          // /settings.txt replaces RAM defaults if valid
 *   SettingsDelta()         // future migration hook
 *   Fastboot_RecoveryCheck()
 */
void mSettings::SettingsDefault(void)
{
  #ifdef ENABLE_LOG_LEVEL_INFO
  ALOG_INF(PSTR(D_LOG_CONFIG D_USE_DEFAULTS));
  #endif

  // --------------------------------------------------------------------------
  // 0. Clear all settings
  // --------------------------------------------------------------------------
  memset(&Settings, 0x00, sizeof(SETTINGS));

  // --------------------------------------------------------------------------
  // 1. Required settings header
  // --------------------------------------------------------------------------
  Settings.cfg_holder = (uint16_t)SETTINGS_HOLDER;
  Settings.cfg_size   = sizeof(SETTINGS);
  Settings.save_flag  = 0;
  Settings.version    = PROJECT_VERSION;
  Settings.bootcount  = 0;
  Settings.cfg_crc32  = 0;

  snprintf(
    Settings.settings_file_update_utc_ascii,
    sizeof(Settings.settings_file_update_utc_ascii),
    PSTR("utc------:------")
  );

  #ifdef ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  snprintf(Settings.settings_holder_ctr, sizeof(Settings.settings_holder_ctr), "%d", Settings.cfg_holder);
  #endif

  // --------------------------------------------------------------------------
  // 2. Core system defaults
  // --------------------------------------------------------------------------
  Settings.bootcount_errors_only = 0;

  Settings.save_data    = SAVE_DATA;
  Settings.enable_sleep = true;
  Settings.sleep        = DEFAULT_LOOP_SLEEP;

  Settings.flag_system.save_state = SAVE_STATE;

  // Legacy flag retained in the struct, but filesystem settings do not use raw flash rotation.
  Settings.flag_system.stop_flash_rotate = true;
  runtime.stop_flash_rotate = true;

  runtime.sleep = Settings.sleep;

  Settings.unified_interface_reporting_invalid_reading_timeout_seconds =
    UNIFIED_INTERFACE_REPORTING_INVALID_SECONDS;

  // --------------------------------------------------------------------------
  // 3. Module / template / GPIO defaults
  // --------------------------------------------------------------------------
  snprintf(Settings.system_name.device,   sizeof(Settings.system_name.device),   "%s", "fallback");
  snprintf(Settings.system_name.friendly, sizeof(Settings.system_name.friendly), "%s", "fallback");
  snprintf(Settings.room_hint,            sizeof(Settings.room_hint),            "%s", "none_set");

  Settings.module      = MODULE;
  Settings.last_module = MODULE;

  for (uint8_t i = 0; i < sizeof(Settings.module_pins.io); i++) {
    Settings.module_pins.io[i] = GPIO_NONE;
  }

  SettingsUpdateText(SET_FRIENDLYNAME1, PSTR(DEVICENAME_FRIENDLY_CTR));

  #ifdef USE_DEBUGFEATURE_DEVICE_CLONE_TESTBED
    #ifdef DEVICENAME_CTR
      SettingsUpdateText(SET_DEVICENAME, PSTR("tb_" DEVICENAME_CTR));
    #else
      SettingsUpdateText(SET_DEVICENAME, PSTR("tb_fallback"));
    #endif
  #else
    #ifdef DEVICENAME_CTR
      SettingsUpdateText(SET_DEVICENAME, PSTR(DEVICENAME_CTR));
    #else
      SettingsUpdateText(SET_DEVICENAME, SettingsText(SET_FRIENDLYNAME1));
    #endif
  #endif

  // --------------------------------------------------------------------------
  // 4. Serial defaults
  // --------------------------------------------------------------------------
  Settings.baudrate         = APP_BAUDRATE / 1200;
  Settings.sbaudrate        = SOFT_BAUDRATE / 1200;
  Settings.serial_delimiter = 0xff;

  // --------------------------------------------------------------------------
  // 5. Logging defaults
  // --------------------------------------------------------------------------
  runtime.seriallog_level_during_boot = SERIAL_LOG_LEVEL_DURING_BOOT;

  Settings.logging.serial_level = runtime.seriallog_level_during_boot;
  Settings.logging.sys_port     = SYS_LOG_PORT;
  Settings.logging.sys_level    = SYS_LOG_LEVEL;
  Settings.logging.web_level    = WEB_LOG_LEVEL;
  Settings.logging.telnet_level = TELNET_LOG_LEVEL;
  Settings.logging.mqtt_level   = LOG_LEVEL_INFO;
  Settings.logging.time_isshort = LOGTIME_DEFAULT_FORMAT;

  SettingsUpdateText(SET_SYSLOG_HOST, PSTR(SYS_LOG_HOST));

  // --------------------------------------------------------------------------
  // 6. Network / WiFi / cellular defaults
  // --------------------------------------------------------------------------
  SettingsUpdateText(SET_OTAURL, PSTR(D_OTA_URL));

  runtime.global_state.network_down = 1;
  runtime.global_state.wifi_down    = true;

  tkr_sup->ParseIPv4(&Settings.ipv4_address[0], PSTR(WIFI_IP_ADDRESS));
  tkr_sup->ParseIPv4(&Settings.ipv4_address[1], PSTR(WIFI_GATEWAY));
  tkr_sup->ParseIPv4(&Settings.ipv4_address[2], PSTR(WIFI_SUBNETMASK));
  tkr_sup->ParseIPv4(&Settings.ipv4_address[3], PSTR(WIFI_DNS));
  tkr_sup->ParseIPv4(&Settings.ipv4_address[4], PSTR(WIFI_DNS2));
  tkr_sup->ParseIPv4(&Settings.ipv4_rgx_address, PSTR(WIFI_RGX_IP_ADDRESS));
  tkr_sup->ParseIPv4(&Settings.ipv4_rgx_subnetmask, PSTR(WIFI_RGX_SUBNETMASK));

  Settings.network.flag.network_wifi = 1;
  Settings.network.flag.sleep_normal = true;
  Settings.network.flag.timers_enable = 0;
  Settings.network.flag.use_wifi_rescan = 1;
  Settings.network.flag.mdns_enabled = 1;
  Settings.network.flag.button_switch_force_local = 0;

  runtime.mdns_delayed_start = 60;

  SettingsUpdateText(SET_HOSTNAME,     WIFI_HOSTNAME);
  SettingsUpdateText(SET_RGX_SSID,     PSTR(WIFI_RGX_SSID));
  SettingsUpdateText(SET_RGX_PASSWORD, PSTR(WIFI_RGX_PASSWORD));

  memset(runtime.my_hostname, 0, sizeof(runtime.my_hostname));
  snprintf(runtime.my_hostname, sizeof(runtime.my_hostname), "%s", Settings.system_name.device);

  // --------------------------------------------------------------------------
  // 7. Web server defaults
  // --------------------------------------------------------------------------
  Settings.webserver = WEB_SERVER;

  SettingsUpdateText(SET_WEBPWD, PSTR(WEB_PASSWORD));
  SettingsUpdateText(SET_CORS,   PSTR(CORS_DOMAIN));

  // --------------------------------------------------------------------------
  // 8. MQTT defaults
  // --------------------------------------------------------------------------
  Settings.flag_system.mqtt_enabled       = true;
  Settings.flag_system.mqtt_power_retain  = 0;
  Settings.flag_system.mqtt_button_retain = 0;
  Settings.flag_system.mqtt_switch_retain = 0;

  // --------------------------------------------------------------------------
  // 9. Param / SetOption defaults
  // --------------------------------------------------------------------------
  Settings.setoption_255[P_BOOT_LOOP_OFFSET] = BOOT_LOOP_OFFSET;
  Settings.setoption_255[P_HOLD_TIME]        = KEY_HOLD_TIME;
  Settings.setoption_255[P_MAX_POWER_RETRY]  = MAX_POWER_RETRY;

  #ifdef USE_NETWORK_MDNS
  Settings.setoption_255[P_MDNS_DELAYED_START] = 0;
  #endif

  // --------------------------------------------------------------------------
  // 10. Time defaults
  // --------------------------------------------------------------------------
  if (((APP_TIMEZONE > -14) && (APP_TIMEZONE < 15)) || (99 == APP_TIMEZONE)) {
    Settings.timezone          = APP_TIMEZONE;
    Settings.timezone_minutes  = 0;
    Settings.timezone2         = APP_TIMEZONE;
    Settings.timezone_minutes2 = 0;
  } else {
    Settings.timezone          = APP_TIMEZONE / 60;
    Settings.timezone_minutes  = abs(APP_TIMEZONE % 60);
    Settings.timezone2         = APP_TIMEZONE / 60;
    Settings.timezone_minutes2 = abs(APP_TIMEZONE % 60);
  }

  Settings.tflag[0].hemis = TIME_STD_HEMISPHERE;
  Settings.tflag[0].week  = TIME_STD_WEEK;
  Settings.tflag[0].dow   = TIME_STD_DAY;
  Settings.tflag[0].month = TIME_STD_MONTH;
  Settings.tflag[0].hour  = TIME_STD_HOUR;
  Settings.toffset[0]     = TIME_STD_OFFSET;

  Settings.tflag[1].hemis = TIME_DST_HEMISPHERE;
  Settings.tflag[1].week  = TIME_DST_WEEK;
  Settings.tflag[1].dow   = TIME_DST_DAY;
  Settings.tflag[1].month = TIME_DST_MONTH;
  Settings.tflag[1].hour  = TIME_DST_HOUR;
  Settings.toffset[1]     = TIME_DST_OFFSET;

  #ifdef ENABLE_DEVFEATURE_SETTINGS__TEXT_BUFFER
  SettingsUpdateText(SET_NTPSERVER1, PSTR(NTP_SERVER1));
  SettingsUpdateText(SET_NTPSERVER2, PSTR(NTP_SERVER2));
  SettingsUpdateText(SET_NTPSERVER3, PSTR(NTP_SERVER3));

  for (uint32_t i = 0; i < MAX_NTP_SERVERS; i++) {
    SettingsUpdateText(
      SET_NTPSERVER1 + i,
      tkr_sup->ReplaceCommaWithDot(SettingsText(SET_NTPSERVER1 + i))
    );
  }
  #endif

  // --------------------------------------------------------------------------
  // 11. Human interface defaults
  // --------------------------------------------------------------------------
  Settings.button_debounce = KEY_DEBOUNCE_TIME;
  Settings.switch_debounce = SWITCH_DEBOUNCE_TIME;

  #ifdef USE_MODULE_SENSORS_SWITCHES
  for (uint8_t i = 0; i < MAX_SWITCHES_SET; i++) {
    Settings.switchmode[i] = SWITCH_MODE;
  }
  #endif

  Settings.flag_system.button_restrict          = 0;
  Settings.flag_system.button_swap              = 0;
  Settings.flag_system.button_single_press_only = 0;
  Settings.flag_system.mqtt_switches            = 0;

  SettingsUpdateText(SET_STATE_TXT1, PSTR(D_OFF));
  SettingsUpdateText(SET_STATE_TXT2, PSTR(D_ON));
  SettingsUpdateText(SET_STATE_TXT3, PSTR(D_TOGGLE));
  SettingsUpdateText(SET_STATE_TXT4, PSTR(D_HOLD));

  // --------------------------------------------------------------------------
  // 12. Sensor defaults
  // --------------------------------------------------------------------------
  Settings.sensors.altitude  = ALTITUDE_ABOVE_SEALEVEL;
  Settings.sensors.latitude  = LATITUDE;
  Settings.sensors.longitude = LONGITUDE;
  Settings.sensors.flags.decimal_precision = 3;

  // --------------------------------------------------------------------------
  // 13. Driver / actuator defaults
  // --------------------------------------------------------------------------
  #ifdef SET_POWERONSTATE_AS_ON
    Settings.poweronstate = POWER_ALL_ON;
  #else
    Settings.poweronstate = APP_POWERON_STATE;
  #endif

  Settings.power_lock = 0;

  Settings.ledmask    = APP_LEDMASK;
  Settings.ledstate   = APP_LEDSTATE;
  Settings.blinktime  = APP_BLINKTIME;
  Settings.blinkcount = APP_BLINKCOUNT;

  Settings.pwm_frequency = PWM_FREQ;
  Settings.pwm_range     = PWM_RANGE;
  Settings.ledpwm_off    = 0;
  Settings.ledpwm_on     = 255;

  Settings.i2c_drivers[0] = I2CDRIVERS_0_31;
  Settings.i2c_drivers[1] = I2CDRIVERS_32_63;
  Settings.i2c_drivers[2] = I2CDRIVERS_64_95;

  Settings.flag_system.pwm_control = 1;

  // --------------------------------------------------------------------------
  // 14. Energy defaults
  // --------------------------------------------------------------------------
  Settings.energy_usage.energy_power_delta = DEFAULT_POWER_DELTA;

  Settings.energy_usage.energy_power_calibration   = HLW_PREF_PULSE;
  Settings.energy_usage.energy_voltage_calibration = HLW_UREF_PULSE;
  Settings.energy_usage.energy_current_calibration = HLW_IREF_PULSE;

  Settings.energy_usage.energy_kWhtoday     = 0;
  Settings.energy_usage.energy_kWhyesterday = 0;
  Settings.energy_usage.energy_kWhdoy       = 0;
  Settings.energy_usage.energy_kWhtotal     = 0;

  Settings.energy_usage.energy_min_power   = 0;
  Settings.energy_usage.energy_max_power   = 0;
  Settings.energy_usage.energy_min_voltage = 0;
  Settings.energy_usage.energy_max_voltage = 0;
  Settings.energy_usage.energy_min_current = 0;
  Settings.energy_usage.energy_max_current = 0;

  Settings.energy_usage.energy_max_power_limit        = 0;
  Settings.energy_usage.energy_max_power_limit_hold   = MAX_POWER_HOLD;
  Settings.energy_usage.energy_max_power_limit_window = MAX_POWER_WINDOW;

  Settings.energy_usage.energy_max_power_safe_limit        = 0;
  Settings.energy_usage.energy_max_power_safe_limit_hold   = SAFE_POWER_HOLD;
  Settings.energy_usage.energy_max_power_safe_limit_window = SAFE_POWER_WINDOW;

  Settings.energy_usage.energy_max_energy       = 0;
  Settings.energy_usage.energy_max_energy_start = 0;

  Settings.flag_power.current_resolution = 3;
  Settings.flag_power.voltage_resolution = 0;
  Settings.flag_power.wattage_resolution = 0;
  Settings.flag_power.energy_resolution  = ENERGY_RESOLUTION;
  Settings.flag_power.calc_resolution    = CALC_RESOLUTION;

  // --------------------------------------------------------------------------
  // 15. Display defaults
  // --------------------------------------------------------------------------
  Settings.display.model   = 0;
  Settings.display.mode    = 0;
  Settings.display.refresh = 2;
  Settings.display.rows    = 4;
  Settings.display.cols[0] = 16;
  Settings.display.cols[1] = 8;
  Settings.display.dimmer  = 7;
  Settings.display.size    = 2;
  Settings.display.font    = 1;
  Settings.display.rotate  = 0;

  // --------------------------------------------------------------------------
  // 16. Module defaults hook
  // --------------------------------------------------------------------------
  // This must only populate RAM defaults. It must not write saved settings.
  tkr->Tasker_Interface(TASK_SETTINGS_DEFAULT);

  ALOG_INF(
    PSTR(D_LOG_MEMORY D_LOAD " %s holder=%d expected=%d size=%d"),
    "SettingsDefault",
    Settings.cfg_holder,
    SETTINGS_HOLDER,
    Settings.cfg_size
  );
}


/**
 * @brief Apply migration deltas to valid loaded settings.
 *
 * This is intentionally not part of SettingsDefault(). Use this later when
 * firmware changes add fields or alter interpretation while SETTINGS_HOLDER
 * remains compatible.
 */
void mSettings::SettingsDelta(void)
{
  bool changed = false;

  // Future pattern:
  // if (Settings.version < SOME_PROJECT_VERSION) {
  //   Settings.some_new_field = SOME_DEFAULT_VALUE;
  //   changed = true;
  // }

  if (changed) {
    Settings.version = PROJECT_VERSION;
    SettingsSaveAll();
  }
}
