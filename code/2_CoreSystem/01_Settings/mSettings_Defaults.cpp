#include "2_CoreSystem/01_Settings/mSettings.h"



// /********************************************************************************************/

void mSettings::SettingsInit(void)
{
  #ifdef ENABLE_DEVFEATURE_SETTINGS_JULY2022
  if (SETTINGS_LOCATION > 0xFA) {
    SETTINGS_LOCATION = 0xFD;       // Skip empty partition part and keep in first 1M
  }
  #endif
}


void mSettings::SettingsDefault(void)
{

  // DEBUG_LINE_HERE;
  
  Settings.flag_system.stop_flash_rotate = true;
  stop_flash_rotate = true;

  // DEBUG_LINE_HERE;
  // Init new devicename buffer
  DeviceNameListI->Init(
    Settings.device_name_buffer.name_buffer,
    sizeof(Settings.device_name_buffer.name_buffer),
    Settings.device_name_buffer.class_id,
    Settings.device_name_buffer.device_id,
    DEVICENAMEBUFFER_NAME_INDEX_LENGTH
  );

  // DEBUG_LINE_HERE;
   //Serial.println("SettingsDefault");

    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG D_USE_DEFAULTS));
    #endif // ENABLE_LOG_LEVEL_COMMANDS

  // DEBUG_LINE_HERE;
   SystemSettings_DefaultHeader();
  // DEBUG_LINE_HERE;
  //  Serial.println(DEBUG_INSERT_PAGE_BREAK "SystemSettings_DefaultBody");
   SystemSettings_DefaultBody();

  // DEBUG_LINE_HERE;
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_MEMORY D_LOAD " Loading any progmem templates"));
    #endif // ENABLE_LOG_LEVEL_INFO

    // pCONT->Tasker_Interface(FUNC_TEMPLATE_MODULE_LOAD_FROM_PROGMEM); // loading module, only interface modules will have these
    // boot_status.module_template_used = true;
    // AddLog(LOG_LEVEL_HIGHLIGHT,PSTR(D_LOG_MEMORY D_LOAD " DISABLED Loading any progmem templates"));



    DEBUG_LINE;
    // Clear module defaults
    pCONT->Tasker_Interface(FUNC_SETTINGS_DEFAULT); // replace with below?
    DEBUG_LINE;
    pCONT->Tasker_Interface(FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT);
    
  // DEBUG_LINE_HERE;
  //   pCONT_set->SettingsSave(2);
    
  // DEBUG_LINE_HERE;
  //   DEBUG_LINE;
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_MEMORY D_LOAD " %s %d %d"), "SettingsDefault",Settings.cfg_holder,SETTINGS_HOLDER);
    #endif// ENABLE_LOG_LEVEL_INFO
    
}

void mSettings::SystemSettings_DefaultHeader(void)
{
  // Erase ALL settings to 0
  memset(&Settings, 0x00, sizeof(SYSCFG));

  Settings.cfg_holder = (uint16_t)SETTINGS_HOLDER;
  Settings.cfg_size = sizeof(SYSCFG);
  Settings.save_flag = 0;
  Settings.version = PROJECT_VERSION;
  Settings.bootcount = 0;
  // AddLog(LOG_LEVEL_HIGHLIGHT, PSTR("Resetting \t\t\tSettings.bootcount"));
  // delay(5000);
  Settings.cfg_crc = 0;
}



void mSettings::SystemSettings_DefaultBody_System(void)
{

  Settings.bootcount_errors_only = 0;     // E01
  Settings.module = MODULE;
  Settings.last_module = MODULE;
  // uint8_t       user_template_base;        // 71F
  // mytmplt       user_template;             // 720  29 bytes
  Settings.save_data = SAVE_DATA;
  for (uint8_t i = 0; i < sizeof(Settings.module_pins); i++) { Settings.module_pins.io[i] = GPIO_NONE_ID; }
  // strlcpy(Settings.ota_url, OTA_URL, sizeof(Settings.ota_url));
  Settings.baudrate = APP_BAUDRATE / 1200;
  // uint8_t       rule_stop;                 // 1A7

  seriallog_level_during_boot = SERIAL_LOG_LEVEL_DURING_BOOT;
  Settings.seriallog_level = seriallog_level_during_boot;
  

  Settings.seriallog_level = SERIAL_LOG_LEVEL;


  Settings.syslog_port = SYS_LOG_PORT;
  Settings.syslog_level = SYS_LOG_LEVEL;
  Settings.weblog_level = WEB_LOG_LEVEL;
  Settings.telnetlog_level = TELNET_LOG_LEVEL;
  Settings.log_time_isshort = LOGTIME_DEFAULT_FORMAT;
  // strlcpy(Settings.system_name.friendly[0], FRIENDLY_NAME "1", sizeof(Settings.system_name.friendly[0]));
  // strlcpy(Settings.system_name.friendly[1], FRIENDLY_NAME "2", sizeof(Settings.system_name.friendly[1]));
  // strlcpy(Settings.system_name.friendly[2], FRIENDLY_NAME "3", sizeof(Settings.system_name.friendly[2]));
  // strlcpy(Settings.system_name.friendly[3], FRIENDLY_NAME "4", sizeof(Settings.system_name.friendly[3]));
  Settings.enable_sleep = true; //on
  Settings.serial_delimiter = 0xff;
  Settings.sbaudrate = SOFT_BAUDRATE / 1200;
  Settings.sleep = DEFAULT_LOOP_SLEEP;

  /**
   * @brief Set everything from normal template to something generic to stop crashes/lost devices when template/gpio error parsing occurs
   * 
   */


  snprintf(pCONT_set->Settings.system_name.device,sizeof(pCONT_set->Settings.system_name.device),"%s","fallback");
  snprintf(pCONT_set->Settings.system_name.friendly,sizeof(pCONT_set->Settings.system_name.friendly),"%s","fallback");
  snprintf(pCONT_set->Settings.room_hint,sizeof(pCONT_set->Settings.room_hint),"%s","none_set");



  Settings.flag_network.sleep_normal = true; // USE DYNAMIC sleep
  
  Settings.mqtt_retry = MQTT_RETRY_SECS;
  
  // strlcpy(Settings.state_text[0], MQTT_STATUS_OFF, sizeof(Settings.state_text[0]));
  // strlcpy(Settings.state_text[1], MQTT_STATUS_ON, sizeof(Settings.state_text[1]));
  // strlcpy(Settings.state_text[2], MQTT_CMND_TOGGLE, sizeof(Settings.state_text[2]));
  // strlcpy(Settings.state_text[3], MQTT_CMND_HOLD, sizeof(Settings.state_text[3]));
  
DEBUG_LINE;

/**
 * @brief Need to change the array access to be "CommandSet_SetOption(id, value)" and "CommandGet_SetOption(id)"
 * 
 */

  Settings.setoption_255[P_BOOT_LOOP_OFFSET] = BOOT_LOOP_OFFSET;
  Settings.setoption_255[P_HOLD_TIME] = KEY_HOLD_TIME;  // Default 4 seconds hold time
  Settings.setoption_255[P_MAX_POWER_RETRY] = MAX_POWER_RETRY;
  #ifdef USE_NETWORK_MDNS
  Settings.setoption_255[P_MDNS_DELAYED_START] = 0;
  #endif // #ifdef USE_NETWORK_MDNS
  Settings.setoption_255[P_RGB_REMAP] = 0;//RGB_REMAP_RGBW;
  Settings.setoption_255[P_BOOT_LOOP_OFFSET] = BOOT_LOOP_OFFSET;

DEBUG_LINE;
    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("DefaultBody_System"));
    #endif// ENABLE_LOG_LEVEL_INFO

DEBUG_LINE;
}


void mSettings::SystemSettings_DefaultBody_Network(){

  // Serial.println("START"); Serial.flush();
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

  // Serial.println("DONE"); Serial.flush();

}


void mSettings::SystemSettings_DefaultBody_WebServer(){

  Settings.webserver = WEB_SERVER;
  strlcpy(Settings.web_password, WEB_PASSWORD, sizeof(Settings.web_password));
  // uint16_t      web_refresh;               // 7CC
  // SettingsDefaultWebColor(); // web_color[18][3];          // 73E

}


void mSettings::SystemSettings_DefaultBody_MQTT(){

  strlcpy(Settings.mqtt.host_address, MQTT_HOST, sizeof(Settings.mqtt.host_address));
  Settings.mqtt.port = MQTT_PORT;
  // strlcpy(Settings.mqtt.client, MQTT_CLIENT_ID, sizeof(Settings.mqtt.client));
  strlcpy(Settings.mqtt.user, MQTT_USER, sizeof(Settings.mqtt.user));
  strlcpy(Settings.mqtt.pwd, MQTT_PASS, sizeof(Settings.mqtt.pwd));
  // strlcpy(Settings.mqtt_topic, MQTT_TOPIC, sizeof(Settings.mqtt_topic));

  
  Settings.flag_system.mqtt_enabled = true;

  // strlcpy(Settings.button_topic, MQTT_BUTTON_TOPIC, sizeof(Settings.button_topic));
  // strlcpy(Settings.switch_topic, MQTT_SWITCH_TOPIC, sizeof(Settings.switch_topic));
  // strlcpy(Settings.mqtt_grptopic, MQTT_GRPTOPIC, sizeof(Settings.mqtt_grptopic));
  // strlcpy(Settings.mqtt_fulltopic, MQTT_FULLTOPIC, sizeof(Settings.mqtt_fulltopic));
  Settings.mqtt.retry = MQTT_RETRY_SECS;
  // strlcpy(Settings.mqtt_prefix[0], SUB_PREFIX, sizeof(Settings.mqtt_prefix[0]));
  // strlcpy(Settings.mqtt_prefix[1], PUB_PREFIX, sizeof(Settings.mqtt_prefix[1]));
  // strlcpy(Settings.mqtt_prefix[2], PUB_PREFIX2, sizeof(Settings.mqtt_prefix[2]));

  // char fingerprint[60];
  // strlcpy(fingerprint, MQTT_FINGERPRINT1, sizeof(fingerprint));
  // char *p = fingerprint;
  // for (uint8_t i = 0; i < 20; i++) {
  //   Settings.mqtt_fingerprint[0][i] = strtol(p, &p, 16);
  // }
  // strlcpy(fingerprint, MQTT_FINGERPRINT2, sizeof(fingerprint));
  // p = fingerprint;
  // for (uint8_t i = 0; i < 20; i++) {
  //   Settings.mqtt_fingerprint[1][i] = strtol(p, &p, 16);
  // }

}


void mSettings::SystemSettings_DefaultBody_TelePeriods(){

//make mqtt commands to allow me to tweak and debug 
  Settings.sensors.ifchanged_secs = SETTINGS_SENSORS_MQTT_IFCHANGED_PERIOD_SECONDS;
  Settings.sensors.ifchanged_json_level = JSON_LEVEL_IFCHANGED; //default
  Settings.sensors.teleperiod_secs = 120;
  Settings.sensors.teleperiod_json_level = JSON_LEVEL_DETAILED; //default
  Settings.sensors.flags.mqtt_retain = 1;// = JSON_METHOD_SHORT; //default
  Settings.sensors.configperiod_secs = SEC_IN_HOUR;

}


void mSettings::SystemSettings_DefaultBody_Time(){

  if (((APP_TIMEZONE > -14) && (APP_TIMEZONE < 15)) || (99 == APP_TIMEZONE)) {
    Settings.timezone = APP_TIMEZONE;
    Settings.timezone_minutes = 0;
  } else {
    Settings.timezone = APP_TIMEZONE / 60;
    Settings.timezone_minutes = abs(APP_TIMEZONE % 60);
  }

  // memset(&Settings.timer, 0x00, sizeof(Timer) * MAX_TIMERS);  // Reset timers as layout has changed from v5.12.0i
  // Settings.pulse_timer[0] = APP_PULSETIME;

  SettingsResetStd();// TimeRule      tflag[2];
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

}


void mSettings::SystemSettings_DefaultBody_Weight(){

// uint16_t      weight_max;                // 7BE Total max weight in kilogram
// unsigned long weight_reference;          // 7C0 Reference weight in gram
// unsigned long weight_calibration;        // 7C4
// unsigned long weight_item;               // 7B8 Weight of one item in gram * 10 

}


void mSettings::SystemSettings_DefaultBody_Lighting(){

  Settings.flag_animations.clear_on_reboot = false;
  Settings.light_settings.light_brightness_as_percentage = 100;
  // memset(Settings.animation_settings.palette_user_colour_map_ids,0,sizeof(Settings.animation_settings.palette_user_colour_map_ids)); // [][]
  // memset(Settings.animation_settings.palette_user_variable_name_ctr,0,sizeof(Settings.animation_settings.palette_user_variable_name_ctr)); // [][]
  // memset(Settings.animation_settings.palette_user_amounts,0,sizeof(Settings.animation_settings.palette_user_amounts)); // [][]
  Settings.animation_settings.animation_mode = 2;
  Settings.animation_settings.animation_palette = 0;
  Settings.animation_settings.animation_transition_order = 2;
  Settings.animation_settings.animation_transition_time_ms = 10000; //TBD save as seconds
  Settings.animation_settings.animation_transition_rate_ms = 10000; //TBD save as seconds
  // Settings.animation_settings.animation_transition_pixels_to_update_as_number = 1;
  // Settings.animation_settings.animation_transition_pixels_to_update_as_percentage = 100;

  Settings.light_settings.light_fade = 0;
  Settings.light_settings.light_speed = 20;
  Settings.light_settings.light_scheme = 0;
  Settings.light_settings.light_width = 1;
  Settings.light_settings.light_rotation = 0;
  Settings.light_settings.light_pixels = WS2812_LEDS;
  
  Settings.light_settings.type = 0;//LT_ADDRESSABLE_WS281X; //default for now
  for (uint8_t i = 0; i < MAX_PWMS; i++) {
    // Settings.light_settings.light_color[i] = 0;
    Settings.pwm_value[i] = 0;
  }
  Settings.light_settings.light_correction = 0;
  Settings.light_settings.light_dimmer = 10;
  Settings.light_settings.light_wakeup = 0;
  // Settings.light_settings.ws_width[WS_SECOND] = 1;
  // Settings.light_settings.ws_color[WS_SECOND][WS_RED] = 255;
  // Settings.light_settings.ws_color[WS_SECOND][WS_GREEN] = 0;
  // Settings.light_settings.ws_color[WS_SECOND][WS_BLUE] = 255;
  // Settings.light_settings.ws_width[WS_MINUTE] = 3;
  // Settings.light_settings.ws_color[WS_MINUTE][WS_RED] = 0;
  // Settings.light_settings.ws_color[WS_MINUTE][WS_GREEN] = 255;
  // Settings.light_settings.ws_color[WS_MINUTE][WS_BLUE] = 0;
  // Settings.light_settings.ws_width[WS_HOUR] = 5;
  // Settings.light_settings.ws_color[WS_HOUR][WS_RED] = 255;
  // Settings.light_settings.ws_color[WS_HOUR][WS_GREEN] = 0;
  // Settings.light_settings.ws_color[WS_HOUR][WS_BLUE] = 0;
  
  // for (uint8_t j = 0; j < 5; j++) {
  //   Settings.light_settings.rgbwwTable[j] = 255;
  // }

  // uint8_t init_colours[25] = {
  //   255, 0, 0, 0, 0,
  //   0, 255, 0, 0, 0,
  //   0, 0, 255, 0, 0,
  //   255, 0, 255, 0, 0,
  //   0, 255, 255, 0, 0
  // };
  uint8_t init_colours[25] = {
    255,0,0,255,0,
    0,255,0,127,255,
    0,0,255,0,127,
    255,0,255,0,0,
    255,255,0,1,2
  };

  memcpy(Settings.animation_settings.palette_rgbcct_users_colour_map, init_colours, sizeof(init_colours));
  
  uint8_t init_colours2[30] = {
    // map_size, map_id_type, 0, 0, 0,
    5*5, 9, 0, 0, 0,
     255,0,0,255,0,
    0,255,0,127,255,
    0,0,255,0,127,
    255,0,255,0,0,
    255,255,0,1,2
    // 1,0,0,255,0,
    // 0,2,0,127,255,
    // 0,0,3,255,127,
    // 4,0,4,0,0,
    // 5,5,0,1,2
  };

  memcpy(Settings.animation_settings.palette_encoded_users_colour_map, init_colours2, sizeof(init_colours2));
  

}


void mSettings::SystemSettings_DefaultBody_PulseCounters(){

  // unsigned long pulse_counter[MAX_COUNTERS];  // 5C0
  // uint16_t      pulse_counter_type;        // 5D0
  // uint16_t      pulse_counter_debounce;    // 5D2

}


void mSettings::SystemSettings_DefaultBody_Sensors(){

  // SensorCfg1    SensorBits1;               // 717  On/Off settings used by Sensor Commands
  // uint32_t      sensors[3];                // 7A4
  // Mcp230xxCfg   mcp230xx_config[16];       // 6F6
  // uint8_t       mcp230xx_int_prio;         // 716
  // uint16_t      mcp230xx_int_timer;        // 718
  Settings.button_debounce = KEY_DEBOUNCE_TIME;
  // char          switch_topic[33];          // 430
  Settings.switch_debounce = SWITCH_DEBOUNCE_TIME;
  #ifdef USE_MODULE_SENSORS_SWITCHES
    for (uint8_t i = 0; i < MAX_SWITCHES; i++) { Settings.switchmode[i] = SWITCH_MODE; }
  #endif
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


}


void mSettings::SystemSettings_DefaultBody_Power()
{
  // Settings.power = APP_POWER;
  #ifdef SET_POWERONSTATE_AS_ON
    Settings.poweronstate = POWER_ALL_ON; //temporary fix for poweronstate
  #else
    Settings.poweronstate = APP_POWERON_STATE;
  #endif
  // Settings.interlock[0] = 0xFF;         // Legacy support using all relays in one interlock group
}


void mSettings::SystemSettings_DefaultBody_Energy(){

  #ifdef USE_MODULE_ENERGY_INTERFACE
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
//   EnergyUsage   energy_usage;              // 77C
//   uint32_t      energy_kWhtotal_time;      // 7B4
//   unsigned long energy_frequency_calibration;  // 7C8
  Settings.energy_usage.energy_kWhtotal = 0;
//   uint16_t      tariff[4][2];              // E30

  RtcSettings.energy_kWhtotal = 0;

  Settings.flag_power.current_resolution = 3;
  Settings.flag_power.voltage_resolution = 0;
  Settings.flag_power.wattage_resolution = 0;
  Settings.flag_power.energy_resolution = ENERGY_RESOLUTION;

  
//   // Energy
//   flag.no_power_on_check |= ENERGY_VOLTAGE_ALWAYS;
//   flag2.current_resolution |= 3;
// //  flag2.voltage_resolution |= 0;
// //  flag2.wattage_resolution |= 0;
//   flag2.energy_resolution |= ENERGY_RESOLUTION;
//   flag3.dds2382_model |= ENERGY_DDS2382_MODE;
//   flag3.hardware_energy_total |= ENERGY_HARDWARE_TOTALS;
//   Settings.setoption_255[P_MAX_POWER_RETRY] = MAX_POWER_RETRY;
// //  Settings.energy_power_delta[0] = 0;
// //  Settings.energy_power_delta[1] = 0;
// //  Settings.energy_power_delta[2] = 0;
  Settings.energy_usage.energy_power_calibration = HLW_PREF_PULSE;
  Settings.energy_usage.energy_voltage_calibration = HLW_UREF_PULSE;
  Settings.energy_usage.energy_current_calibration = HLW_IREF_PULSE;
// //  Settings.energy_kWhtoday = 0;
// //  Settings.energy_kWhyesterday = 0;
// //  Settings.energy_kWhdoy = 0;
// //  Settings.energy_min_power = 0;
// //  Settings.energy_max_power = 0;
// //  Settings.energy_min_voltage = 0;
// //  Settings.energy_max_voltage = 0;
// //  Settings.energy_min_current = 0;
// //  Settings.energy_max_current = 0;
// //  Settings.energy_max_power_limit = 0;                            // MaxPowerLimit
//   Settings.energy_max_power_limit_hold = MAX_POWER_HOLD;
//   Settings.energy_max_power_limit_window = MAX_POWER_WINDOW;
// //  Settings.energy_max_power_safe_limit = 0;                       // MaxSafePowerLimit
//   Settings.energy_max_power_safe_limit_hold = SAFE_POWER_HOLD;
//   Settings.energy_max_power_safe_limit_window = SAFE_POWER_WINDOW;
// //  Settings.energy_max_energy = 0;                                 // MaxEnergy
// //  Settings.energy_max_energy_start = 0;                           // MaxEnergyStart
// //  Settings.energy_kWhtotal = 0;
//   RtcSettings.energy_kWhtotal = 0;
// //  memset((char*)&Settings.energy_usage, 0x00, sizeof(Settings.energy_usage));
//   memset((char*)&RtcSettings.energy_usage, 0x00, sizeof(RtcSettings.energy_usage));
//   Settings.setoption_255[P_OVER_TEMP] = ENERGY_OVERTEMP;
  #endif

}


void mSettings::SystemSettings_DefaultBody_Drivers(){

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
  
  // for (uint8_t i = 0; i < 17; i++) { Settings.rf_code[i][0] = 0; }
  // memcpy_P(Settings.rf_code[0], kDefaultRfCode, 9);

  // uint32_t      drivers[3];                // 794
  //memset(&Settings.drivers, 0xFF, 32);  // Enable all possible monitors, displays, drivers and sensors

}


void mSettings::SystemSettings_DefaultBody_Rules(){

//  Settings.rule_enabled = 0;
//  Settings.rule_once = 0;
  // char          mems[MAX_RULE_MEMS][10];   // 7CE
  //for (uint8_t i = 1; i < MAX_RULE_SETS; i++) { Settings.rules[i][0] = '\0'; }

}


void mSettings::SystemSettings_DefaultBody_Displays(){

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


void mSettings::SystemSettings_DefaultBody(void)
{
  // Erase only body region 16 wrong
  //memset((char*)&Settings +16, 0x00, sizeof(SYSCFG) -16);
DEBUG_LINE;
  SystemSettings_DefaultBody_System();
DEBUG_LINE;
  //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("SystemSettings_DefaultBody_Time1"));
  SystemSettings_DefaultBody_Network();
DEBUG_LINE;
  //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("SystemSettings_DefaultBody_Network"));
  SystemSettings_DefaultBody_WebServer();
DEBUG_LINE;
  //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("SystemSettings_DefaultBody_Time2"));
  SystemSettings_DefaultBody_MQTT();
DEBUG_LINE;
  SystemSettings_DefaultBody_TelePeriods();
DEBUG_LINE;
  SystemSettings_DefaultBody_Time();
DEBUG_LINE;
  //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("SystemSettings_DefaultBody_Time"));
  SystemSettings_DefaultBody_Weight();
DEBUG_LINE;
  SystemSettings_DefaultBody_Lighting();
DEBUG_LINE;
  SystemSettings_DefaultBody_PulseCounters();
DEBUG_LINE;
  //  Serial.println("SystemSettings_DefaultBody_PulseCounters"); Serial.flush();
  SystemSettings_DefaultBody_Sensors();
DEBUG_LINE;

  SystemSettings_DefaultBody_Power();
DEBUG_LINE;
  SystemSettings_DefaultBody_Energy();
DEBUG_LINE;
  SystemSettings_DefaultBody_Drivers();
DEBUG_LINE;
  SystemSettings_DefaultBody_Rules();
DEBUG_LINE;
  SystemSettings_DefaultBody_Displays();
DEBUG_LINE;
  //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("SystemSettings_DefaultBody_Displays"));
  
  //  Serial.println("SystemSettings_DefaultBody_DONE"); Serial.flush();

  Settings.flag_system.mqtt_enabled = 1;
  Settings.flag_network.timers_enable = 0;
  Settings.flag_network.use_wifi_rescan = 1;
  Settings.flag_system.stop_flash_rotate = true;
  stop_flash_rotate = true;//Settings.flag_system.stop_flash_rotate;
  devices_present = 6; 
  //global_state.data = 3;  // Init global state (wifi_down, mqtt_down) to solve possible network issues
  // baudrate = Settings.baudrate * 1200;


  #ifdef USE_NETWORK_MDNS
  Settings.flag_network.mdns_enabled = 1;//USE_NETWORK_MDNS;
  mdns_delayed_start = 60;//Settings.setoption_255[P_MDNS_DELAYED_START];
  #endif // #ifdef USE_NETWORK_MDNS


  // seriallog_level = Settings.seriallog_level;
  // seriallog_timer = SERIALLOG_TIMER;
  // syslog_level = Settings.syslog_level;
  // Settings.flag_system.stop_flash_rotate = true;
  // stop_flash_rotate = true;//Settings.flag_system.stop_flash_rotate;
  // save_data_counter = Settings.save_data; 
  runtime_var.sleep = Settings.sleep;
//  Settings.flag_system.value_units = 0;
//  Settings.flag_system.stop_flash_rotate = 0;
//  Settings.flag_system.interlock = 0;
  Settings.flag_system.save_state = SAVE_STATE;
//  for (uint8_t i = 1; i < MAX_PULSETIMERS; i++) { Settings.pulse_timer[i] = 0; }
  // Settings.flag_power.emulation = EMULATION;
 Settings.flag_system.button_restrict = 0;
 Settings.flag_system.button_swap = 0;

 Settings.flag_system.button_single = 0; // support only single press to support faster button recognition (disable to allow multipress)


#ifdef ENABLE_DEVFEATURE_BUTTON_SET_FLAG_BUTTON_SINGLE
Settings.flag_system.button_single = ENABLE_DEVFEATURE_BUTTON_SET_FLAG_BUTTON_SINGLE;
#endif // ENABLE_DEVFEATURE_BUTTON_SET_FLAG_BUTTON_SINGLE


  Settings.flag_system.mqtt_enabled = true;
//  Settings.flag_system.mqtt_response = 0;
  Settings.flag_system.mqtt_power_retain = 0;//MQTT_POWER_RETAIN;
  Settings.flag_system.mqtt_button_retain = 0;//MQTT_BUTTON_RETAIN;
  Settings.flag_system.mqtt_switch_retain = 0;//MQTT_SWITCH_RETAIN;
  Settings.flag_network.button_switch_force_local =0;// MQTT_BUTTON_SWITCH_FORCE_LOCAL;
 // Settings.flag_network.hass_tele_on_power = TELE_ON_POWER;
//  Settings.flag_system.mqtt_sensor_retain = 0;
//  Settings.flag_system.mqtt_offline = 0;
//  Settings.flag_system.mqtt_serial = 0;
//  Settings.flag_system.device_index_enable = 0;
  // Settings.sensors.flags.mqtt_retain = true;
  Settings.flag_power.calc_resolution = CALC_RESOLUTION;
//  Settings.flag_system.knx_enabled = 0;
//  Settings.flag_system.knx_enable_enhancement = 0;
  Settings.flag_system.pwm_control = 1;
  //Settings.flag_system.ws_clock_reverse = 0;
  //Settings.flag_system.light_signal = 0;
  //Settings.flag_system.not_power_linked = 0;
  //Settings.flag_system.decimal_text = 0;
  
// void SettingsEnableAllI2cDrivers(void) {
  Settings.i2c_drivers[0] = I2CDRIVERS_0_31;
  Settings.i2c_drivers[1] = I2CDRIVERS_32_63;
  Settings.i2c_drivers[2] = I2CDRIVERS_64_95;
// }
  

  sprintf(my_hostname,"%s",pCONT_set->Settings.system_name.device);
  
}



void mSettings::SettingsResetStd(void)
{
  Settings.tflag[0].hemis = TIME_STD_HEMISPHERE;
  Settings.tflag[0].week  = TIME_STD_WEEK;
  Settings.tflag[0].dow   = TIME_STD_DAY;
  Settings.tflag[0].month = TIME_STD_MONTH;
  Settings.tflag[0].hour  = TIME_STD_HOUR;
  Settings.toffset[0]     = TIME_STD_OFFSET;
}




void mSettings::SettingsResetDst(void)
{
  Settings.tflag[1].hemis = TIME_DST_HEMISPHERE;
  Settings.tflag[1].week = TIME_DST_WEEK;
  Settings.tflag[1].dow = TIME_DST_DAY;
  Settings.tflag[1].month = TIME_DST_MONTH;
  Settings.tflag[1].hour = TIME_DST_HOUR;
  Settings.toffset[1] = TIME_DST_OFFSET;
}


// void mSettings::SettingsDefaultWebColor(void)
// {
  
//   #ifdef USE_MODULE_NETWORK_WEBSERVER
//   char scolor[10];
//   for (uint8_t i = 0; i < COL_LAST; i++) {
//     pCONT_web->WebHexCode(i, pCONT_sup->GetTextIndexed_P(scolor, sizeof(scolor), i, kWebColors));
//   }
//   #endif //  #ifdef USE_MODULE_NETWORK_WEBSERVER
// }
