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


void mSettings::SettingsDefault(void)
{
  
  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG D_USE_DEFAULTS));
  #endif // ENABLE_LOG_LEVEL_COMMANDS

  Settings.flag_system.stop_flash_rotate = true;
  runtime.stop_flash_rotate = true;

  // Erase ALL settings to 0
  memset(&Settings, 0x00, sizeof(SETTINGS));

  SystemSettings_DefaultHeader();
  
  SystemSettings_DefaultBody();

  // Clear module defaults
  pCONT->Tasker_Interface(FUNC_SETTINGS_DEFAULT); // replace with below?
  DEBUG_LINE_HERE;
  pCONT->Tasker_Interface(FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT);
      
  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_MEMORY D_LOAD " %s %d %d"), "SettingsDefault",Settings.cfg_holder,SETTINGS_HOLDER);
  #endif// ENABLE_LOG_LEVEL_INFO

  // After defaults are loaded everything should immediately be saved
  SettingsSaveAll();
    
}

void mSettings::SystemSettings_DefaultHeader(void)
{

  Settings.cfg_holder = (uint16_t)SETTINGS_HOLDER;
  Settings.cfg_size = sizeof(SETTINGS);
  Settings.save_flag = 0;
  Settings.version = PROJECT_VERSION;
  Settings.bootcount = 0;

  #ifdef ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  snprintf(Settings.settings_holder_ctr, sizeof(Settings.settings_holder_ctr), "%d", Settings.cfg_holder);
  #endif

}


void mSettings::SystemSettings_DefaultBody(void)
{

  /*********************************************************************************************
   ******* CoreSystem **************************************************************************
   *********************************************************************************************/

  Settings.bootcount_errors_only = 0;     // E01
  Settings.module = MODULE;
  Settings.last_module = MODULE;
  
  Settings.save_data = SAVE_DATA;
  Settings.enable_sleep = true; //on
  Settings.sleep = DEFAULT_LOOP_SLEEP;
  
  runtime.stop_flash_rotate = true;//Settings.flag_system.stop_flash_rotate;
  runtime.devices_present = 6; 

  runtime.sleep = Settings.sleep;
  Settings.flag_system.save_state = SAVE_STATE;
  
  /*********************************************************************************************
   ******* Template/GPIO ***********************************************************************
   *********************************************************************************************/

  snprintf(pCONT_set->Settings.system_name.device,sizeof(pCONT_set->Settings.system_name.device),"%s","fallback");
  snprintf(pCONT_set->Settings.system_name.friendly,sizeof(pCONT_set->Settings.system_name.friendly),"%s","fallback");
  snprintf(pCONT_set->Settings.room_hint,sizeof(pCONT_set->Settings.room_hint),"%s","none_set");

  for (uint8_t i = 0; i < sizeof(Settings.module_pins); i++) { Settings.module_pins.io[i] = GPIO_NONE_ID; }
  
  /*********************************************************************************************
   ******* Serial *****************************************************************************
   *********************************************************************************************/

  Settings.baudrate = APP_BAUDRATE / 1200;
  Settings.serial_delimiter = 0xff;
  Settings.sbaudrate = SOFT_BAUDRATE / 1200;

  /*********************************************************************************************
   ******* Logging *****************************************************************************
   *********************************************************************************************/

  runtime.seriallog_level_during_boot = SERIAL_LOG_LEVEL_DURING_BOOT;
  Settings.logging.serial_level = runtime.seriallog_level_during_boot;
  Settings.logging.serial_level = SERIAL_LOG_LEVEL;
  Settings.logging.sys_port = SYS_LOG_PORT;
  Settings.logging.sys_level = SYS_LOG_LEVEL;
  Settings.logging.web_level = WEB_LOG_LEVEL;
  Settings.logging.telnet_level = TELNET_LOG_LEVEL;
  Settings.logging.time_isshort = LOGTIME_DEFAULT_FORMAT;

  /*********************************************************************************************
   ******* Networking: Wifi/Network/Cellular ***************************************************
   *********************************************************************************************/


  Settings.sta_config = WIFI_CONFIG_TOOL;
  Settings.sta_active = 0;
  strlcpy(Settings.sta_ssid[0], STA_SSID1, sizeof(Settings.sta_ssid[0]));
  strlcpy(Settings.sta_ssid[1], STA_SSID2, sizeof(Settings.sta_ssid[1]));
  strlcpy(Settings.sta_ssid[2], STA_SSID3, sizeof(Settings.sta_ssid[2]));
  strlcpy(Settings.sta_pwd[0], STA_PASS1, sizeof(Settings.sta_pwd[0]));
  strlcpy(Settings.sta_pwd[1], STA_PASS2, sizeof(Settings.sta_pwd[1]));
  strlcpy(Settings.sta_pwd[2], STA_PASS3, sizeof(Settings.sta_pwd[2]));
  strlcpy(Settings.hostname, WIFI_HOSTNAME, sizeof(Settings.hostname));
  strlcpy(Settings.syslog_host, SYS_LOG_HOST, sizeof(Settings.syslog_host));
  pCONT_sup->ParseIp(&Settings.ip_address[0], WIFI_IP_ADDRESS);
  pCONT_sup->ParseIp(&Settings.ip_address[1], WIFI_GATEWAY);
  pCONT_sup->ParseIp(&Settings.ip_address[2], WIFI_SUBNETMASK);
  pCONT_sup->ParseIp(&Settings.ip_address[3], WIFI_DNS); 

  Settings.flag_network.network_wifi = 1;
  Settings.flag_network.sleep_normal = true; // USE DYNAMIC sleep
  Settings.webserver = WEB_SERVER;
  strlcpy(Settings.web_password, WEB_PASSWORD, sizeof(Settings.web_password));

  Settings.flag_network.timers_enable = 0;
  Settings.flag_network.use_wifi_rescan = 1;
  Settings.flag_system.stop_flash_rotate = true;

  Settings.flag_network.mdns_enabled = 1;
  runtime.mdns_delayed_start = 60;
  sprintf(runtime.my_hostname,"%s",pCONT_set->Settings.system_name.device);

  /*********************************************************************************************
   ******* Networking: MQTT ********************************************************************
   *********************************************************************************************/

  Settings.flag_system.mqtt_enabled = 1;
  strlcpy(Settings.mqtt.host_address, MQTT_HOST, sizeof(Settings.mqtt.host_address));
  strlcpy(Settings.mqtt.user, MQTT_USER, sizeof(Settings.mqtt.user));
  strlcpy(Settings.mqtt.pwd, MQTT_PASS, sizeof(Settings.mqtt.pwd));
  
  Settings.mqtt_retry = MQTT_RETRY_SECS;
  Settings.flag_system.mqtt_enabled = true;
  Settings.mqtt.retry = MQTT_RETRY_SECS;

  Settings.sensors.ifchanged_secs = SETTINGS_SENSORS_MQTT_IFCHANGED_PERIOD_SECONDS; // ifchanged etc timing should be moved into mqtt substruct
  Settings.sensors.ifchanged_json_level = JSON_LEVEL_IFCHANGED; //default
  Settings.sensors.teleperiod_secs = 120;
  Settings.sensors.teleperiod_json_level = JSON_LEVEL_DETAILED; //default
  Settings.sensors.flags.mqtt_retain = 1;// = JSON_METHOD_SHORT; //default
  Settings.sensors.configperiod_secs = SEC_IN_HOUR;
  
  Settings.flag_system.mqtt_enabled = true;
//  Settings.flag_system.mqtt_response = 0;
  Settings.flag_system.mqtt_power_retain = 0;//MQTT_POWER_RETAIN;
  Settings.flag_system.mqtt_button_retain = 0;//MQTT_BUTTON_RETAIN;
  Settings.flag_system.mqtt_switch_retain = 0;//MQTT_SWITCH_RETAIN;

  /*********************************************************************************************
   ******* Param Options ***********************************************************************
   *********************************************************************************************/
  
  Settings.setoption_255[P_BOOT_LOOP_OFFSET] = BOOT_LOOP_OFFSET;
  Settings.setoption_255[P_HOLD_TIME] = KEY_HOLD_TIME;  // Default 4 seconds hold time
  Settings.setoption_255[P_MAX_POWER_RETRY] = MAX_POWER_RETRY;
  #ifdef USE_NETWORK_MDNS
  Settings.setoption_255[P_MDNS_DELAYED_START] = 0;
  #endif // #ifdef USE_NETWORK_MDNS
  Settings.setoption_255[P_RGB_REMAP] = 0;//RGB_REMAP_RGBW;
  Settings.setoption_255[P_BOOT_LOOP_OFFSET] = BOOT_LOOP_OFFSET;

  /*********************************************************************************************
   ******* Time ********************************************************************************
   *********************************************************************************************/

  if (((APP_TIMEZONE > -14) && (APP_TIMEZONE < 15)) || (99 == APP_TIMEZONE)) {
    Settings.timezone = APP_TIMEZONE;
    Settings.timezone_minutes = 0;
  } else {
    Settings.timezone = APP_TIMEZONE / 60;
    Settings.timezone_minutes = abs(APP_TIMEZONE % 60);
  }

  SettingsResetStd();
  SettingsResetDst();

  // strlcpy(Settings.ntp_server[0], NTP_SERVER1, sizeof(Settings.ntp_server[0]));
  // strlcpy(Settings.ntp_server[1], NTP_SERVER2, sizeof(Settings.ntp_server[1]));
  // strlcpy(Settings.ntp_server[2], NTP_SERVER3, sizeof(Settings.ntp_server[2]));
  // for (uint8_t j = 0; j < 3; j++) {
  //   for (uint8_t i = 0; i < strlen(Settings.ntp_server[j]); i++) {
  //     if (Settings.ntp_server[j][i] == ',') {
  //       Settings.ntp_server[j][i] = '.';
  //     }
  //   }
  // }

  /*********************************************************************************************
   ******* Lighting ****************************************************************************
   *********************************************************************************************/

  // Most will be moved into its own settings and own file
  
  Settings.flag_animations.clear_on_reboot = false;
  Settings.light_settings.light_brightness_as_percentage = 100;

  Settings.light_settings.light_fade = 0;
  Settings.light_settings.light_speed = 20;
  Settings.light_settings.light_scheme = 0;
  Settings.light_settings.light_width = 1;
  Settings.light_settings.light_rotation = 0;
  Settings.light_settings.light_pixels = WS2812_LEDS;
  
  Settings.light_settings.type = 0;
  
  for (uint8_t i = 0; i < MAX_PWMS; i++)
  {
    Settings.pwm_value[i] = 0;
  }
  Settings.light_settings.light_correction = 0;
  Settings.light_settings.light_dimmer = 10;
  Settings.light_settings.light_wakeup = 0;
  
  /*********************************************************************************************
   ******* Human Interface Devices (HID) e.g. Buttons/Switches *********************************
   *********************************************************************************************/
  
  Settings.button_debounce = KEY_DEBOUNCE_TIME;
  Settings.switch_debounce = SWITCH_DEBOUNCE_TIME;
  #ifdef USE_MODULE_SENSORS_SWITCHES
    for (uint8_t i = 0; i < MAX_SWITCHES; i++) { Settings.switchmode[i] = SWITCH_MODE; }
  #endif
  Settings.flag_system.button_restrict = 0;
  Settings.flag_system.button_swap = 0;
  Settings.flag_system.button_single = 0; // support only single press to support faster button recognition (disable to allow multipress)
  Settings.flag_network.button_switch_force_local =0;

  /*********************************************************************************************
   ******* Sensors *****************************************************************************
   *********************************************************************************************/

  Settings.sensors.altitude = ALTITUDE_ABOVE_SEALEVEL; 
  Settings.sensors.latitude = LATITUDE;//(int)((double)LATITUDE * 1000000);
  Settings.sensors.longitude = LONGITUDE;//(int)((double)LONGITUDE * 1000000);
  Settings.sensors.flags.decimal_precision = 3;
  Settings.SensorBits1.bh1750_1_resolution  = 1;
  Settings.SensorBits1.bh1750_2_resolution  = 1;
  // Settings.flag_sensor.temperature_conversion = TEMP_CONVERSION;
  // Settings.flag_sensor.pressure_conversion = PRESSURE_CONVERSION;
  // Settings.flag_sensor.pressure_resolution = PRESSURE_RESOLUTION;
  // Settings.flag_sensor.humidity_resolution = HUMIDITY_RESOLUTION;
  // Settings.flag_sensor.temperature_resolution = TEMP_RESOLUTION;
  
  /*********************************************************************************************
   ******* Drivers *****************************************************************************
   *********************************************************************************************/

  #ifdef SET_POWERONSTATE_AS_ON
    Settings.poweronstate = POWER_ALL_ON; //temporary fix for poweronstate
  #else
    Settings.poweronstate = APP_POWERON_STATE;
  #endif
  
  Settings.ledmask = APP_LEDMASK;
  Settings.ledstate = APP_LEDSTATE;
  Settings.blinktime = APP_BLINKTIME;
  Settings.blinkcount = APP_BLINKCOUNT;
  // uint32_t      monitors;                  // 7A0
  Settings.pwm_frequency = PWM_FREQ;
  Settings.pwm_range = PWM_RANGE;
  // uint16_t      pwm_value[MAX_PWMS];       // 2EC
  Settings.ledpwm_off = 0;
  Settings.ledpwm_on = 255;
  
  Settings.i2c_drivers[0] = I2CDRIVERS_0_31;
  Settings.i2c_drivers[1] = I2CDRIVERS_32_63;
  Settings.i2c_drivers[2] = I2CDRIVERS_64_95;  
  
  Settings.flag_system.pwm_control = 1;

  /*********************************************************************************************
   ******* Energy ******************************************************************************
   *********************************************************************************************/

  Settings.energy_usage.energy_power_delta = DEFAULT_POWER_DELTA;
  Settings.energy_usage.energy_power_calibration = HLW_PREF_PULSE;
  Settings.energy_usage.energy_voltage_calibration = HLW_UREF_PULSE;
  Settings.energy_usage.energy_current_calibration = HLW_IREF_PULSE; 
  Settings.energy_usage.energy_kWhtoday = 0;
  Settings.energy_usage.energy_kWhyesterday = 0;
  Settings.energy_usage.energy_kWhdoy = 0;
  Settings.energy_usage.energy_min_power = 0;
  Settings.energy_usage.energy_max_power = 0;
  Settings.energy_usage.energy_min_voltage = 0;
  Settings.energy_usage.energy_max_voltage = 0;
  Settings.energy_usage.energy_min_current = 0;
  Settings.energy_usage.energy_max_current = 0;
  Settings.energy_usage.energy_max_power_limit = 0;   
  Settings.energy_usage.energy_max_power_limit_hold = MAX_POWER_HOLD;
  Settings.energy_usage.energy_max_power_limit_window = MAX_POWER_WINDOW;
  Settings.energy_usage.energy_max_power_safe_limit = 0;                       // MaxSafePowerLimit
  Settings.energy_usage.energy_max_power_safe_limit_hold = SAFE_POWER_HOLD;
  Settings.energy_usage.energy_max_power_safe_limit_window = SAFE_POWER_WINDOW;
  Settings.energy_usage.energy_max_energy = 0;                                 // MaxEnergy
  Settings.energy_usage.energy_max_energy_start = 0;                           // MaxEnergyStart
  Settings.energy_usage.energy_kWhtotal = 0;

  Settings.flag_power.current_resolution = 3;
  Settings.flag_power.voltage_resolution = 0;
  Settings.flag_power.wattage_resolution = 0;
  Settings.flag_power.energy_resolution = ENERGY_RESOLUTION;
  Settings.energy_usage.energy_power_calibration = HLW_PREF_PULSE;
  Settings.energy_usage.energy_voltage_calibration = HLW_UREF_PULSE;
  Settings.energy_usage.energy_current_calibration = HLW_IREF_PULSE;

  Settings.flag_power.calc_resolution = CALC_RESOLUTION;

  /*********************************************************************************************
   ******* Display *****************************************************************************
   *********************************************************************************************/
  
  Settings.display.model = 0;
  Settings.display.mode = 0;//EM_DISPLAY_MODE_LOCAL1_ID;
  Settings.display.refresh = 2;
  Settings.display.rows = 4;
  Settings.display.cols[0] = 16;
  Settings.display.cols[1] = 8;
  Settings.display.dimmer = 7;
  Settings.display.size = 2;
  Settings.display.font = 1;
  Settings.display.rotate = 0;
  
}



void mSettings::SettingsResetStd(void)
{
  pCONT_time->tflag[0].hemis = TIME_STD_HEMISPHERE;
  pCONT_time->tflag[0].week  = TIME_STD_WEEK;
  pCONT_time->tflag[0].dow   = TIME_STD_DAY;
  pCONT_time->tflag[0].month = TIME_STD_MONTH;
  pCONT_time->tflag[0].hour  = TIME_STD_HOUR;
  pCONT_time->toffset[0]     = TIME_STD_OFFSET;
}




void mSettings::SettingsResetDst(void)
{
  pCONT_time->tflag[1].hemis = TIME_DST_HEMISPHERE;
  pCONT_time->tflag[1].week = TIME_DST_WEEK;
  pCONT_time->tflag[1].dow = TIME_DST_DAY;
  pCONT_time->tflag[1].month = TIME_DST_MONTH;
  pCONT_time->tflag[1].hour = TIME_DST_HOUR;
  pCONT_time->toffset[1] = TIME_DST_OFFSET;
}
