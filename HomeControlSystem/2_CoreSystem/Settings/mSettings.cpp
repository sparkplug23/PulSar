
#include "2_CoreSystem/Settings/mSettings.h"

struct DATA_BUFFER data_buffer;

// struct TIME_T RtcTime;
// struct TIMERULES TimeRules;
// union TimeRule TimeRuleTest;


const char* mSettings::PM_MODULE_CORE_SETTINGS_CTR = D_MODULE_CORE_SETTINGS_CTR;
const char* mSettings::PM_MODULE_CORE_SETTINGS_FRIENDLY_CTR = D_MODULE_CORE_SETTINGS_FRIENDLY_CTR;



    #ifdef ENABLE_DEBUG_FUNCTION_NAMES
// Switch case should be faster than getext progmem
// Use progmem WITHOUT buffer for speed improvements, should be read as expected progmem and handled that way
const char* mSettings::GetTaskName(uint8_t task, char* buffer){

  switch(task){
    default:
    case FUNC_POINTER_INIT:                           return PM_FUNC_POINTER_INIT_CTR;
    case FUNC_TEMPLATE_MODULE_LOAD:                          return PM_FUNC_TEMPLATE_LOAD_CTR;
    // case FUNC_MODULE_INIT:                            return PM_FUNC_MODULE_INIT_CTR;
    case FUNC_PRE_INIT:                               return PM_FUNC_PRE_INIT_CTR;
    case FUNC_INIT:                                   return PM_FUNC_INIT_CTR;
    case FUNC_CONFIGURE_MODULES_FOR_DEVICE:           return PM_FUNC_CONFIGURE_MODULES_FOR_DEVICE_CTR;
    case FUNC_LOOP:                                   return PM_FUNC_LOOP_CTR;
    case FUNC_EVERY_50_MSECOND:                       return PM_FUNC_EVERY_50_MSECOND_CTR;
    case FUNC_EVERY_100_MSECOND:                      return PM_FUNC_EVERY_100_MSECOND_CTR;
    case FUNC_EVERY_250_MSECOND:                      return PM_FUNC_EVERY_250_MSECOND_CTR;
    case FUNC_EVERY_SECOND:                           return PM_FUNC_EVERY_SECOND_CTR;
    case FUNC_EVERY_MINUTE:                           return PM_FUNC_EVERY_MINUTE_CTR; 
    case FUNC_EVERY_HOUR:                             return PM_FUNC_EVERY_HOUR_CTR; 
    case FUNC_EVERY_MIDNIGHT:                         return PM_FUNC_EVERY_MIDNIGHT_CTR;
    case FUNC_EVERY_MIDDAY:                           return PM_FUNC_EVERY_MIDDAY_CTR;
    case FUNC_ON_BOOT_SUCCESSFUL:                     return PM_FUNC_ON_SUCCESSFUL_BOOT_CTR;
    case FUNC_UPTIME_10_SECONDS:                      return PM_FUNC_UPTIME_10_SECONDS_CTR;
    case FUNC_UPTIME_1_MINUTES:                       return PM_FUNC_UPTIME_1_MINUTES_CTR;
    case FUNC_UPTIME_10_MINUTES:                      return PM_FUNC_UPTIME_10_MINUTES_CTR;
    case FUNC_UPTIME_60_MINUTES:                      return PM_FUNC_UPTIME_60_MINUTES_CTR;
    // case FUNC_RESTART_SPLASH_INFORMATION:             return PM_FUNC_RESTART_SPLASH_INFORMATION_CTR;
    // case FUNC_PREP_BEFORE_TELEPERIOD:                 return PM_FUNC_PREP_BEFORE_TELEPERIOD_CTR;
    case FUNC_JSON_APPEND:                            return PM_FUNC_JSON_APPEND_CTR;
    case FUNC_SAVE_BEFORE_RESTART:                    return PM_FUNC_SAVE_BEFORE_RESTART_CTR;
    case FUNC_SETTINGS_DEFAULT:                       return PM_FUNC_SETTINGS_DEFAULT_CTR;
    case FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES:    return PM_FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES_CTR;
    case FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT:    return PM_FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT_CTR;
    case FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE:       return PM_FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE_CTR;
    case FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE:       return PM_FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE_CTR;
    case FUNC_FUNCTION_LAMBDA_INIT:                   return PM_FUNC_FUNCTION_LAMBDA_INIT_CTR;
    case FUNC_FUNCTION_LAMBDA_LOOP:                   return PM_FUNC_FUNCTION_LAMBDA_LOOP_CTR;
    // case FUNC_COMMAND:                                return PM_FUNC_COMMAND_CTR;
    // case FUNC_COMMAND_SENSOR:                         return PM_FUNC_COMMAND_SENSOR_CTR;
    // case FUNC_COMMAND_DRIVER:                         return PM_FUNC_COMMAND_DRIVER_CTR;
    // case FUNC_JSON_COMMAND:                           return PM_FUNC_JSON_COMMAND_CTR;
    // case FUNC_JSON_COMMAND_OBJECT:                    return PM_FUNC_JSON_COMMAND_OBJECT_CTR;
    case FUNC_WIFI_CONNECTED:                         return PM_FUNC_WIFI_CONNECTED_CTR;
    case FUNC_WIFI_DISCONNECTED:                      return PM_FUNC_WIFI_DISCONNECTED_CTR;
    case FUNC_MQTT_SUBSCRIBE:                         return PM_FUNC_MQTT_SUBSCRIBE_CTR;
    // case FUNC_MQTT_INIT:                              return PM_FUNC_MQTT_INIT_CTR;
    case FUNC_MQTT_CONNECTED:                         return PM_FUNC_MQTT_CONNECTED_CTR;
    case FUNC_MQTT_DISCONNECTED:                      return PM_FUNC_MQTT_DISCONNECTED_CTR;
    case FUNC_MQTT_COMMAND:                           return PM_FUNC_MQTT_COMMAND_CTR;
    case FUNC_MQTT_SENDER:                            return PM_FUNC_MQTT_SENDER_CTR;
    case FUNC_MQTT_HANDLERS_RESET:                    return PM_FUNC_MQTT_HANDLERS_RESET_CTR;
    case FUNC_MQTT_HANDLERS_INIT:                     return PM_FUNC_MQTT_HANDLERS_INIT_CTR;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:       return PM_FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD_CTR;
    case FUNC_SET_POWER:                              return PM_FUNC_SET_POWER_CTR;
    case FUNC_SET_DEVICE_POWER:                       return PM_FUNC_SET_DEVICE_POWER_CTR;
    case FUNC_SHOW_SENSOR:                            return PM_FUNC_SHOW_SENSOR_CTR;
    case FUNC_RULES_PROCESS:                          return PM_FUNC_RULES_PROCESS_CTR;
    case FUNC_SERIAL:                                 return PM_FUNC_SERIAL_CTR;
    case FUNC_FREE_MEM:                               return PM_FUNC_FREE_MEM_CTR;
    case FUNC_BUTTON_PRESSED:                         return PM_FUNC_BUTTON_PRESSED_CTR;
    case FUNC_ENERGY_RESET:                           return PM_FUNC_ENERGY_RESET_CTR;
    case FUNC_SENSOR_UPDATED:                         return PM_FUNC_SENSOR_UPDATED_CTR;
    case FUNC_STATUS_MESSAGE_APPEND:                  return PM_FUNC_STATUS_MESSAGE_APPEND_CTR;
    case FUNC_JSON_HARDWARE_APPEND:                   return PM_FUNC_JSON_HARDWARE_APPEND_CTR;
    case FUNC_SET_CHANNELS:                           return PM_FUNC_SET_CHANNELS_CTR;
    case FUNC_WEB_ADD_MAIN_BUTTON:                    return PM_FUNC_WEB_ADD_MAIN_BUTTON_CTR;
    case FUNC_WEB_ADD_BUTTON:                         return PM_FUNC_WEB_ADD_BUTTON_CTR;
    case FUNC_WEB_ADD_BUTTON_SYSTEM_SETTINGS:         return PM_FUNC_WEB_ADD_BUTTON_SYSTEM_SETTINGS_CTR;
    case FUNC_WEB_ADD_HANDLER:                        return PM_FUNC_WEB_ADD_HANDLER_CTR;
    case FUNC_WEB_ROOT_SEND_STYLE:                    return PM_FUNC_WEB_ROOT_SEND_STYLE_CTR;
    case FUNC_WEB_ROOT_SEND_SCRIPT:                   return PM_FUNC_WEB_ROOT_SEND_SCRIPT_CTR;
    case FUNC_WEB_ROOT_SCRIPT_JSON_FETCH_MODULEPARSING:return PM_FUNC_WEB_ROOT_SCRIPT_JSON_FETCH_MODULEPARSING_CTR;
    case FUNC_WEB_ROOT_SEND_BODY:                     return PM_FUNC_WEB_ROOT_SEND_BODY_CTR;
    case FUNC_WEB_ROOT_SEND_STATUS:                   return PM_FUNC_WEB_ROOT_SEND_STATUS_CTR;
    case FUNC_WEB_ADD_ROOT_SHOWS:                     return PM_FUNC_WEB_ADD_ROOT_SHOWS_CTR;
    case FUNC_WEB_ADD_JSON_FETCH_RESULT:              return PM_FUNC_WEB_ADD_JSON_FETCH_RESULT_CTR;
    case FUNC_WEB_ADD_JSON_DATA_FETCH_URL:            return PM_FUNC_WEB_ADD_JSON_DATA_FETCH_URL_CTR;
    case FUNC_WEB_ADD_ROOT_TABLE_ROWS:                return PM_FUNC_WEB_ADD_ROOT_TABLE_ROWS_CTR;
    case FUNC_WEB_ADD_ROOT_SCRIPT:                    return PM_FUNC_WEB_ADD_ROOT_SCRIPT_CTR;
    case FUNC_WEB_ADD_ROOT_STYLE:                     return PM_FUNC_WEB_ADD_ROOT_STYLE_CTR;
    // case FUNC_WEB_APPEND_LOADTIME_ROOT_URLS:          return PM_FUNC_WEB_APPEND_LOADTIME_ROOT_URLS_CTR;
    // case FUNC_WEB_APPEND_LOADTIME_ROOT_RATES:         return PM_FUNC_WEB_APPEND_LOADTIME_ROOT_RATES_CTR;
    case FUNC_WEB_APPEND_RUNTIME_ROOT_URLS:           return PM_FUNC_WEB_APPEND_RUNTIME_ROOT_URLS_CTR;
    // case FUNC_WEB_APPEND_RUNTIME_ROOT_RATES:          return PM_FUNC_WEB_APPEND_RUNTIME_ROOT_RATES_CTR;
    case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED: return PM_FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED_CTR;
    case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_FORCED:    return PM_FUNC_WEB_APPEND_ROOT_STATUS_TABLE_FORCED_CTR;
    case FUNC_WEB_APPEND_ROOT_ADD_MAIN_BUTTONS:       return PM_FUNC_WEB_APPEND_ROOT_ADD_MAIN_BUTTONS_CTR;
    case FUNC_WEB_APPEND_ROOT_BUTTONS:                return PM_FUNC_WEB_APPEND_ROOT_BUTTONS_CTR;
    case FUNC_WEB_PAGEINFORMATION_SEND_MODULE:        return PM_FUNC_WEB_PAGEINFORMATION_SEND_MODULE_CTR;
    case FUNC_WEB_COMMAND:                            return PM_FUNC_WEB_COMMAND_CTR;
    // case FUNC_CHECK_POINTERS:                         return PM_FUNC_CHECK_POINTERS_CTR;
    case FUNC_WEB_SYSTEM_INFO:                        return PM_FUNC_WEB_SYSTEM_INFO_CTR;
    case FUNC_DEBUG_CONFIGURE:                        return PM_FUNC_DEBUG_CONFIGURE_CTR;
  }

}

    #endif // ENABLE_DEBUG_FUNCTION_NAMES



// load in driver and sensor template settings
void mSettings::Function_Template_Load(){

  #ifndef DISABLE_SERIAL_LOGGING
  //DEBUG_PRINTF("mSettings::Function_Template_Load"); Serial.flush();
  #endif

  boot_status.function_template_parse_success = 0;
  boot_status.rules_template_parse_success = 0;

  #ifdef USE_FUNCTION_TEMPLATE  
  // Read into local
  D_DATA_BUFFER_CLEAR();
  memcpy_P(data_buffer.payload.ctr,FUNCTION_TEMPLATE,sizeof(FUNCTION_TEMPLATE));
  data_buffer.payload.len = strlen(data_buffer.payload.ctr);

  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_DEBUG, PSTR("FUNCTION_TEMPLATE Load"));// = \"%d|%s\""),data_buffer.payload.len, data_buffer.payload.ctr);
  AddLog(LOG_LEVEL_DEBUG, PSTR("FUNCTION_TEMPLATE READ = \"%d|%s\""),data_buffer.payload.len, data_buffer.payload.ctr);
  #endif // ENABLE_LOG_LEVEL_INFO

  pCONT->Tasker_Interface(FUNC_JSON_COMMAND_ID);

  boot_status.function_template_parse_success = 1;
  #endif //USE_FUNCTION_TEMPLATE


  #ifdef USE_RULES_TEMPLATE
  // Read into local
  D_DATA_BUFFER_CLEAR();
  memcpy_P(data_buffer.payload.ctr,RULES_TEMPLATE,sizeof(RULES_TEMPLATE));
  data_buffer.payload.len = strlen(data_buffer.payload.ctr);

  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_DEBUG, PSTR("RULES_TEMPLATE Load"));// = \"%d|%s\""),data_buffer.payload.len, data_buffer.payload.ctr);
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("RULES_TEMPLATE READ = \"%d|%s\""),data_buffer.payload.len, data_buffer.payload.ctr);
  #endif // ENABLE_LOG_LEVEL_INFO

  pCONT->Tasker_Interface(FUNC_JSON_COMMAND_ID);
//IF TASKER RESULT WAS TRUE, THEN SUCCESS
  boot_status.rules_template_parse_success = 1;
  #endif //USE_RULES_TEMPLATE

  
}

int16_t mSettings::GetFunctionIDbyFriendlyName(const char* c){

  if(c=='\0'){
    return -1;
  }
  if(strcasecmp_P(c,PM_FUNC_EVENT_INPUT_STATE_CHANGED_CTR)==0){ return FUNC_EVENT_INPUT_STATE_CHANGED_ID; }

  if(strcasecmp_P(c,PM_FUNC_EVENT_MOTION_STARTED_CTR)==0){ return FUNC_EVENT_MOTION_STARTED_ID; }

  if(strcasecmp_P(c,PM_FUNC_EVENT_MOTION_STARTED_CTR)==0){ return FUNC_EVENT_MOTION_STARTED_ID; }
  if(strcasecmp_P(c,PM_FUNC_EVENT_MOTION_ENDED_CTR)==0){ return FUNC_EVENT_MOTION_ENDED_ID; }


  if(strcasecmp_P(c,PM_FUNC_EVENT_SET_POWER_CTR)==0){ return FUNC_EVENT_SET_POWER_ID; }

  if(strcasecmp_P(c,PM_FUNC_EVENT_SET_SPEED_CTR)==0){ return FUNC_EVENT_SET_SPEED_ID; }


  return -1;
}



// Settings will contain all jsoncommands for "CoreSystem"

//overload fix when only one parameter is called
int8_t mSettings::Tasker(uint8_t function, JsonParserObject obj){//}, uint8_t param1){  
//   JsonObjectConst dummy; return Tasker(function, dummy);
// }
// template<typename T>
// int8_t mSettings::Tasker(uint8_t function, JsonParserObject obj), T param1){ 

  // DEBUG_PRINT_FUNCTION_NAME_TEST;
  switch(function){
    case FUNC_INIT:
    

    break;
    case FUNC_LOOP:

      // if(mTime::TimeReached(&tSavedSavingTest,10000)){
      //   // pCONT->Tasker_Interface(FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE);
      //   // SettingsSave(1);
      //   // pCONT->Tasker_Interface(FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE);
      //   // SettingsLoad();
      //   //     AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_MEMORY D_LOAD " 1before"));
      //   // pCONT->Tasker_Interface(FUNC_SETTINGS_DEFAULT);
      //   // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_MEMORY D_LOAD " 1after"));

      // }

    break;
    case FUNC_EVERY_SECOND:{

    // Function_Template_Load();

    
  // AddLog(LOG_LEVEL_DEBUG,PSTR( "TaskerTest SUCCESS!!"));


  // AddLog(LOG_LEVEL_DEBUG,PSTR( "GetNameBuffer_Length=%d"),DeviceNameListI->GetNameBuffer_Length());



// SystemSettings_DefaultBody_Network();

       // AddLog(LOG_LEVEL_TEST,PSTR("sizeof(SYSCFG)=%d %%"),map(sizeof(SYSCFG),0,4095,0,100));


       
     } break;
    case FUNC_EVERY_FIVE_SECOND:{


    // Function_Template_Load();

#ifdef ENABLE_DEVFEATURE_PERIODIC_RULE_FILLING

  D_DATA_BUFFER_CLEAR();
  memcpy_P(data_buffer.payload.ctr,RULES_TEMPLATE,sizeof(RULES_TEMPLATE));
  data_buffer.payload.len = strlen(data_buffer.payload.ctr);

  #ifdef ENABLE_LOG_LEVEL_INFO
  // AddLog(LOG_LEVEL_TEST, PSTR("RULES_TEMPLATE READ = \"%d|%s\""),data_buffer.payload.len, data_buffer.payload.ctr);
  #endif // ENABLE_LOG_LEVEL_INFO

  pCONT->Tasker_Interface(FUNC_JSON_COMMAND_ID);

  

  D_DATA_BUFFER_CLEAR();

  // char buffer_unescaped[100];
  
  // pCONT_sup->GetTextIndexed(
  //   buffer_unescaped, 
  //   sizeof(buffer_unescaped), 
  //   pCONT_rules->rules[0].command.json_commands_dlist_id, 
  //   pCONT_rules->jsonbuffer.data
  // ); 

  // for(int i=0;i<strlen(buffer_unescaped);i++){
  //   // if(buffer_unescaped[i] == '\"'){
  //   //   data_buffer.payload.len+=sprintf(data_buffer.payload.ctr+data_buffer.payload.len,"\\\"");
  //   // }else{    
  //    data_buffer.payload.ctr[data_buffer.payload.len++] = buffer_unescaped[i];
  //   // }
  // }
  
  // char buffer_unescaped[100];
  
  pCONT_sup->GetTextIndexed(
    data_buffer.payload.ctr, 
    sizeof(data_buffer.payload.ctr), 
    pCONT_rules->rules[0].command.json_commands_dlist_id, 
    pCONT_rules->jsonbuffer.data
  ); 
  data_buffer.payload.len += strlen(data_buffer.payload.ctr);

  // for(int i=0;i<strlen(buffer_unescaped);i++){
  //   // if(buffer_unescaped[i] == '\"'){
  //   //   data_buffer.payload.len+=sprintf(data_buffer.payload.ctr+data_buffer.payload.len,"\\\"");
  //   // }else{    
  //    data_buffer.payload.ctr[data_buffer.payload.len++] = buffer_unescaped[i];
  //   // }
  // }
  

  // sprintf(data_buffer.payload.ctr,"\"");
  
  // pCONT_sup->GetTextIndexed(
  //   data_buffer.payload.ctr+strlen(data_buffer.payload.ctr), 
  //   sizeof(data_buffer.payload.ctr), 
  //   pCONT_rules->rules[0].command.json_commands_dlist_id, 
  //   pCONT_rules->jsonbuffer.data
  // ); 
  // sprintf(data_buffer.payload.ctr+strlen(data_buffer.payload.ctr),"\"");

  AddLog(LOG_LEVEL_TEST,PSTR("FUNC_JSON_COMMAND_ID1=%s"),data_buffer.payload.ctr);

  pCONT->Tasker_Interface(FUNC_JSON_COMMAND_ID);
  
  // snprintf(data_buffer.payload.ctr,pCONT_rules->rules[0].command.);
  // // memcpy_P(data_buffer.payload.ctr,RULES_TEMPLATE,sizeof(RULES_TEMPLATE));
  // data_buffer.payload.len = strlen(data_buffer.payload.ctr);

#endif // ENABLE_DEVFEATURE_PERIODIC_RULE_FILLING



      // int8_t device_id;
      // int8_t class_id = EM_MODULE_DRIVERS_RELAY_ID;
      // AddLog(LOG_LEVEL_INFO,PSTR("FUNC_EVERY_FIVE_SECOND\n\r\n\r"));

      // int16_t device_id_found = GetDeviceIDbyName("Socket",pCONT_set->Settings.device_name_buffer.name_buffer,&device_id,&class_id);
      // AddLog(LOG_LEVEL_INFO,PSTR("\n\r\n\rdevice_id_found = %d"),device_id_found);

      // device_id_found = GetDeviceIDbyName("Plug",pCONT_set->Settings.device_name_buffer.name_buffer,&device_id,&class_id);
      // AddLog(LOG_LEVEL_INFO,PSTR("\n\r\n\rdevice_id_found = %d"),device_id_found);

      // device_id_found = GetDeviceIDbyName("Plug2",pCONT_set->Settings.device_name_buffer.name_buffer,&device_id,&class_id);
      // AddLog(LOG_LEVEL_INFO,PSTR("\n\r\n\rdevice_id_found = %d"),device_id_found);

      // device_id_found = GetDeviceIDbyName("Plug3",pCONT_set->Settings.device_name_buffer.name_buffer,&device_id,&class_id);
      // AddLog(LOG_LEVEL_INFO,PSTR("\n\r\n\rdevice_id_found = %d"),device_id_found);

    }break;

    case FUNC_EVERY_MINUTE:
    // Change to saving using counter later, Settings.save_data

      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_BOOT_COUNT " = %d"), Settings.bootcount);
      #ifdef ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
      // Update Settings with local module values that need saving
      pCONT->Tasker_Interface(FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE);

      pCONT_set->SettingsSave(1);
      #else 
      DEBUG_PRINTLN("SettingsSave dis");
      #endif // ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING

      // Serial.println("FUNC_EVERY_MINUTE");
      
// #ifdef DISABLE_SETTINGS_SAVING_BUG
      // pCONT_set->SettingsSave(1);
    // #endif
    break;
    case FUNC_ON_BOOT_SUCCESSFUL:
      Settings.bootcount++;              // Moved to here to stop flash writes during start-up
      #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_BOOT_COUNT "SUCCESSFUL BOOT %d"), Settings.bootcount);
      #endif// ENABLE_LOG_LEVEL_INFO

          
      // #ifdef USE_MICHAEL_DEBUG_OVERRIDE 
      //   #ifdef DEBUG_SERIAL_TESTING
      //     Settings.seriallog_level = LOG_LEVEL_COMMANDS;
      //   #else
      //     Settings.seriallog_level = SERIAL_LOG_LEVEL_DURING_BOOT;//LOG_LEVEL_INFO;
      //   #endif
      //   Settings.weblog_level = LOG_LEVEL_INFO;
      //   //Settings.telnetlog_level = LOG_LEVEL_INFO;
      //   Settings.seriallog_level = LOG_LEVEL_DEBUG;
      //   #ifdef ENABLE_LOG_FILTERING_TEST_ONLY
      //     enable_serial_logging_filtering = true;
      //     Settings.seriallog_level = LOG_LEVEL_TEST;
      //   #endif
      //   //enable_web_logging_filtering = true;
      //   //Settings.flog_time_short = true;
      //   #ifdef DEBUG_FOR_FAULT
      //     Settings.seriallog_level = LOG_LEVEL_ALL;
      //   #endif
      // #endif



    break;
    
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;

    /************
     * xx SECTION * 
    *******************/

    case FUNC_TEMPLATE_DEVICE_LOAD:
      Function_Template_Load();
    break;
  }

} 


uint16_t mSettings::CountCharInCtr(const char* tosearch, char tofind){
  uint16_t count = 0;
  for(uint16_t i=0;i<strlen(tosearch);i++){
    if(tosearch[i]==tofind){ count++; }
  }
  return count;
}


uint32_t mSettings::GetRtcRebootCrc(void)
{
  uint32_t crc = 0;
  uint8_t *bytes = (uint8_t*)&RtcReboot;

  for (uint16_t i = 0; i < sizeof(RTCRBT); i++) {
    crc += bytes[i]*(i+1);
  }
  return crc;
}

void mSettings::RtcRebootSave(void)
{
  #ifdef ESP8266
  if (GetRtcRebootCrc() != rtc_reboot_crc) {
    RtcReboot.valid = RTC_MEM_VALID;
    ESP.rtcUserMemoryWrite(100 - sizeof(RTCRBT), (uint32_t*)&RtcReboot, sizeof(RTCRBT));
    rtc_reboot_crc = GetRtcRebootCrc();
  }
  #endif
}

void mSettings::RtcRebootLoad(void)
{
  #ifdef ESP8266

  ESP.rtcUserMemoryRead(100 - sizeof(RTCRBT), (uint32_t*)&RtcReboot, sizeof(RTCRBT));  // 0x280
  if (RtcReboot.valid != RTC_MEM_VALID) {
    memset(&RtcReboot, 0, sizeof(RTCRBT));
    RtcReboot.valid = RTC_MEM_VALID;
//    RtcReboot.fast_reboot_count = 0;  // Explicit by memset
    RtcRebootSave();
  }
  rtc_reboot_crc = GetRtcRebootCrc();
  DEBUG_PRINTF("fast_reboot=%d\n\r",RtcReboot.fast_reboot_count);
  #endif
}




const char* mSettings::GetTelePeriodJsonLevelCtr(char* buffer){
  return GetTelePeriodJsonLevelCtr(pCONT_set->Settings.sensors.teleperiod_json_level, buffer);
}
const char* mSettings::GetTelePeriodJsonLevelCtr(uint8_t id, char* buffer){
  switch(id){
    default:
    case JSON_LEVEL_NONE:      memcpy_P(buffer, PM_JSON_LEVEL_NONE_CTR, sizeof(PM_JSON_LEVEL_NONE_CTR)); break;
    case JSON_LEVEL_IFCHANGED: memcpy_P(buffer, PM_JSON_LEVEL_IFCHANGED_CTR, sizeof(PM_JSON_LEVEL_IFCHANGED_CTR)); break;
    // case JSON_LEVEL_SHORT:     return "SHORT";
    // case JSON_LEVEL_DETAILED:  return "DETAILED";
    // case JSON_LEVEL_ALL:       return "ALL";
  }
  return buffer;
}


/********************************************************************************************/
/*
 * Based on cores/esp8266/Updater.cpp
 */
void mSettings::SetFlashModeDout(void)
{
  uint8_t *_buffer;
  uint32_t address;

    #ifdef ESP8266
  eboot_command ebcmd;
  eboot_command_read(&ebcmd);
  address = ebcmd.args[0];
  _buffer = new uint8_t[FLASH_SECTOR_SIZE];

  if (ESP.flashRead(address, (uint32_t*)_buffer, FLASH_SECTOR_SIZE)) {
    if (_buffer[2] != 3) {  // DOUT
      _buffer[2] = 3;
      if (ESP.flashEraseSector(address / FLASH_SECTOR_SIZE)) ESP.flashWrite(address, (uint32_t*)_buffer, FLASH_SECTOR_SIZE);
    }
  }
  delete[] _buffer;
  
    #endif // ESP8266
}

void mSettings::SettingsBufferFree(void)
{
    #ifdef ESP8266
  if (settings_buffer != nullptr) {
    free(settings_buffer);
    settings_buffer = nullptr;
  }
    #endif // ESP8266
}

bool mSettings::SettingsBufferAlloc(void)
{
    #ifdef ESP8266
  SettingsBufferFree();
  if (!(settings_buffer = (uint8_t *)malloc(sizeof(Settings)))) {
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_UPLOAD_ERR_2));  // Not enough (memory) space
    #endif // ENABLE_LOG_LEVEL_INFO
    return false;
  }
  return true;
    #endif // ESP8266
}


/**
 * If true, then load important info from settings struct into runtime values, else, remain on old settings
 * */
void mSettings::SettingsLoad_CheckSuccessful(){
  
  if (Settings.param[P_BOOT_LOOP_OFFSET]) {
    // Disable functionality as possible cause of fast restart within BOOT_LOOP_TIME seconds (Exception, WDT or restarts)
    if (RtcReboot.fast_reboot_count > Settings.param[P_BOOT_LOOP_OFFSET]) {       // Restart twice
      Settings.flag_network.user_esp8285_enable = 0;       // Disable ESP8285 Generic GPIOs interfering with flash SPI
      if (RtcReboot.fast_reboot_count > Settings.param[P_BOOT_LOOP_OFFSET] +1) {  // Restart 3 times
        // for (uint8_t i = 0; i < MAX_RULE_SETS; i++) {
        //   // if (bitRead(Settings.rule_stop, i)) {
        //   //   bitWrite(Settings.rule_enabled, i, 0);  // Disable rules causing boot loop
        //   // }
        // }
      }
      // if (RtcReboot.fast_reboot_count > Settings.param[P_BOOT_LOOP_OFFSET] +2) {  // Restarted 4 times
      //   Settings.rule_enabled = 0;                  // Disable all rules
      // }
      if (RtcReboot.fast_reboot_count > Settings.param[P_BOOT_LOOP_OFFSET] +3) {  // Restarted 5 times
        for (uint8_t i = 0; i < sizeof(Settings.module_pins); i++) {
          Settings.module_pins.io[i] = GPIO_NONE_ID;         // Reset user defined GPIO disabling sensors
        }
      }
      if (RtcReboot.fast_reboot_count > Settings.param[P_BOOT_LOOP_OFFSET] +4) {  // Restarted 6 times
        Settings.module = MODULE_WEMOS_ID;             // Reset module to Sonoff Basic
        Settings.last_module = MODULE_WEMOS_ID;
      }
      //reset 7 times, then fail into safe boot awaiting OTA
      // HandleFailedBootFailBack();
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_LOG_SOME_SETTINGS_RESET " (%d)"), RtcReboot.fast_reboot_count);
      #endif///   #ifdef ENABLE_LOG_LEVEL_INFO
    }
  }

  memset(Settings.mqtt.topic,0,sizeof(Settings.mqtt.topic));
  memcpy(Settings.mqtt.topic,pCONT_set->Settings.system_name.device,strlen(pCONT_set->Settings.system_name.device));
  
  // Configure hostname 
  memset(my_hostname,0,sizeof(my_hostname));
  sprintf(my_hostname,PSTR("%s"),pCONT_set->Settings.system_name.device);

  //Only load wifi here or else set fallback

  // AddLog(LOG_LEVEL_INFO, PSTR(D_PROJECT " %s %s " D_VERSION " %s%s-" ARDUINO_ESP8266_RELEASE), pCONT_set->Settings.system_name.device, Settings.system_name.friendly, my_version, my_image);
  

}





/*********************************************************************************************\
 * RTC memory
\*********************************************************************************************/

uint32_t mSettings::GetRtcSettingsCrc(void)
{
  uint32_t crc = 0;
  uint8_t *bytes = (uint8_t*)&RtcSettings;

  for (uint32_t i = 0; i < sizeof(RTCMEM); i++) {
    crc += bytes[i]*(i+1);
  }
  return crc;
}

void mSettings::RtcSettingsSave(void)
{
  
  #ifdef ESP8266
  if (GetRtcSettingsCrc() != rtc_settings_crc) {
    RtcSettings.valid = RTC_MEM_VALID;
    ESP.rtcUserMemoryWrite(100, (uint32_t*)&RtcSettings, sizeof(RTCMEM));
    rtc_settings_crc = GetRtcSettingsCrc();
  }
  #endif// ESP8266
}

void mSettings::RtcSettingsLoad(void)
{
    #ifdef ESP8266
  ESP.rtcUserMemoryRead(100, (uint32_t*)&RtcSettings, sizeof(RTCMEM));  // 0x290
  if (RtcSettings.valid != RTC_MEM_VALID) {
    memset(&RtcSettings, 0, sizeof(RTCMEM));
    RtcSettings.valid = RTC_MEM_VALID;
    
  #ifdef USE_ENERGY
    RtcSettings.energy_kWhtoday = Settings.energy_usage.energy_kWhtoday;
    RtcSettings.energy_kWhtotal = Settings.energy_usage.energy_kWhtotal;
    RtcSettings.energy_usage = Settings.energy_usage;
    // for (uint32_t i = 0; i < MAX_COUNTERS; i++) {
    //   RtcSettings.pulse_counter[i] = Settings.pulse_counter[i];
    //}
    #endif
    RtcSettings.power = Settings.power;
    RtcSettingsSave();
  }
  rtc_settings_crc = GetRtcSettingsCrc();
    #endif // ESP8266
}

bool mSettings::RtcSettingsValid(void)
{
  return (RTC_MEM_VALID == RtcSettings.valid);
}

/********************************************************************************************/


void mSettings::RtcRebootReset(void)
{
  RtcReboot.fast_reboot_count = 0;
  RtcRebootSave();
}


bool mSettings::RtcRebootValid(void)
{
  return (RTC_MEM_VALID == RtcReboot.valid);
}


// /********************************************************************************************/


    // template<typename T>
    // int8_t mSettings::Tasker2(uint8_t function, T param1){

    // }
    

// template int8_t mSettings::Tasker<uint8_t, char>(uint8_t function, char param1);


