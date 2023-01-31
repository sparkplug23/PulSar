
#include "2_CoreSystem/01_Settings/mSettings.h"

struct DATA_BUFFER data_buffer;

const char* mSettings::PM_MODULE_CORE_SETTINGS_CTR = D_MODULE_CORE_SETTINGS_CTR;
const char* mSettings::PM_MODULE_CORE_SETTINGS_FRIENDLY_CTR = D_MODULE_CORE_SETTINGS_FRIENDLY_CTR;



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


    // 
    // TestSettingsLoad();

    /**
     * Decounter until saving.
     * This is used when I want to force save data after an update has happened, but want the storage event to be delayed so it doesn't slow the event (eg relay set, mqtt response)
     * */
    if(settings_save_decounter_seconds_delayed_save)
    {
      settings_save_decounter_seconds_delayed_save--;
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_TEST, PSTR("settings_save_decounter_seconds_delayed_save = %d"), settings_save_decounter_seconds_delayed_save);
    #endif // ENABLE_LOG_LEVEL_INFO
      if(settings_save_decounter_seconds_delayed_save==0)
      {
        pCONT_set->SettingsSaveAll();
      }
    }

/**
 * @brief I will implement this, but its another receovery option not related the RTC Fastboot
 * 
 */
  // if (POWER_CYCLE_TIME == TasmotaGlobal.uptime) {
  //   UpdateQuickPowerCycle(false);
  // }

    
  // AddLog(LOG_LEVEL_DEBUG,PSTR( "TaskerTest SUCCESS!!"));


  // AddLog(LOG_LEVEL_DEBUG,PSTR( "GetNameBuffer_Length=%d"),DeviceNameListI->GetNameBuffer_Length());



// SystemSettings_DefaultBody_Network();

       // AddLog(LOG_LEVEL_TEST,PSTR("sizeof(SYSCFG)=%d %%"),map(sizeof(SYSCFG),0,4095,0,100));


       
     } break;
    case FUNC_EVERY_FIVE_SECOND:{



    }break;

    case FUNC_EVERY_MINUTE:
    // Change to saving using counter later, Settings.save_data

      #ifdef ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
      // Update Settings with local module values that need saving
      pCONT->Tasker_Interface(FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE);

      // pCONT_set->SettingsSave(1);
      pCONT_set->SettingsSaveAll();

      #else 
      
    #ifdef ENABLE_LOG_LEVEL_INFO
      DEBUG_PRINTLN("SettingsSave dis");
      
    #endif // ifdef ENABLE_LOG_LEVEL_INFO
      #endif // ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING

      // Serial.println("FUNC_EVERY_MINUTE");
      
// #ifdef DISABLE_SETTINGS_SAVING_BUG
      // pCONT_set->SettingsSave(1);
    // #endif
    break;
    case FUNC_ON_BOOT_SUCCESSFUL:
      Settings.bootcount++;              // Moved to here to stop flash writes during start-up

      #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_BOOT_COUNT "SUCCESSFUL BOOT %d after %d seconds"), Settings.bootcount, 120);
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

      
      // I think this is in time module, check it (the bootcount is I think, not sure about boot_loop_time, this should be moved into settings)


      // Save stable start
      // Dont use a bootloop time, instead have other "stable qualifiers", probably from a function I can call that can be diffferent by device ie (if mqtt, then require it..... if network, then require it...... )
        // if (BOOT_LOOP_TIME == pCONT_time-> uptime.seconds_nonreset) {

          #ifdef ENABLE_DEVFEATURE_FASTBOOT_DETECTION
          RtcFastboot_Reset(); // ie reset the value so bootloops wont be detected after this point (eg 10 seconds)
          #endif

      #ifdef ENABLE_DEVFEATURE_RTC_FASTBOOT_GLOBALTEST_V3
      flag_rtc_reboot_reset_on_success = true;
      ALOG_HGL(PSTR("flag_rtc_reboot_reset_on_success"));
      #endif // ENABLE_DEVFEATURE_RTC_FASTBOOT_GLOBALTEST_V3


          //Settings->last_module = Settings->module;

      #ifdef USE_DEEPSLEEP
          if (!(DeepSleepEnabled() && !Settings->flag3.bootcount_update)) {  
            // SetOption76  - (Deepsleep) Enable incrementing bootcount (1) when deepsleep is enabled
      #endif
            // Settings->bootcount++;              // Moved to here to stop flash writes during start-up
            // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_BOOT_COUNT " %d"), Settings->bootcount);
      #ifdef USE_DEEPSLEEP
          }
      #endif

        // }






    break;
    case FUNC_EVERY_FIVE_MINUTE:

      ALOG_INF( PSTR(D_LOG_APPLICATION D_BOOT_COUNT " = %d"), Settings.bootcount);

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

    case FUNC_TEMPLATE_DEVICE_LOAD_FROM_PROGMEM:
      Function_Template_Load();
    break;
  }

} 


#ifdef ENABLE_DEBUG_FUNCTION_NAMES
// Switch case should be faster than getext progmem
// Use progmem WITHOUT buffer for speed improvements, should be read as expected progmem and handled that way
const char* mSettings::GetTaskName(uint8_t task, char* buffer){

  switch(task){
    default:
    case FUNC_POINTER_INIT:                           return PM_FUNC_POINTER_INIT_CTR;
    case FUNC_TEMPLATE_MODULE_LOAD_FROM_PROGMEM:                          return PM_FUNC_TEMPLATE_LOAD_CTR;
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
    case FUNC_MQTT_HANDLERS_INIT:                     return PM_FUNC_MQTT_HANDLERS_INIT_CTR;
    case FUNC_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:       return PM_FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD_CTR;
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
  // AddLog(LOG_LEVEL_DEBUG, PSTR("FUNCTION_TEMPLATE Load"));// = \"%d|%s\""),data_buffer.payload.len, data_buffer.payload.ctr);
  ALOG_DBM( PSTR(DEBUG_INSERT_PAGE_BREAK  "FUNCTION_TEMPLATE READ = \"%d|%s\""),data_buffer.payload.len, data_buffer.payload.ctr);
  #endif // ENABLE_LOG_LEVEL_INFO

  pCONT->Tasker_Interface(FUNC_JSON_COMMAND_ID);

  boot_status.function_template_parse_success = 1;
  #endif //USE_FUNCTION_TEMPLATE
  
}

int16_t mSettings::GetFunctionIDbyFriendlyName(const char* c){

  if(*c=='\0'){
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


/**
 * @brief 
 * 
 
 TOGGLE
 FOLLOW
 FOLLOW_INV

  
 * 
 * 
 */



/**
 * @brief 
 * Actually not right for rules, I should probably make "SwitchMode_GetID_by_Name" and "GetStateNumber" together
 * 
 * @param c 
 * @return int16_t 
 */
int16_t mSettings::SwitchMode_GetID_by_Name(const char* c)
// D_DATE_TIME_SEPARATOR
// RuleCommand? I need to be able to react to trigger, or simply directly set, so needs both switchmode and getstate range, create new LIST
{
  if(*c=='\0'){    return -1; }
  if(strcasecmp_P(c,PM_SWITCHMODE_TOGGLE_CTR)==0){ return SWITCHMODE_TOGGLE_ID; }
  if(strcasecmp_P(c,PM_SWITCHMODE_FOLLOW_CTR)==0){ return SWITCHMODE_FOLLOW_ID; }
  if(strcasecmp_P(c,PM_SWITCHMODE_FOLLOW_INV_CTR)==0){ return SWITCHMODE_FOLLOW_INV_ID; }
  if(strcasecmp_P(c,PM_SWITCHMODE_PUSHBUTTON_CTR)==0){ return SWITCHMODE_PUSHBUTTON_ID; }
  if(strcasecmp_P(c,PM_SWITCHMODE_PUSHBUTTON_INV_CTR)==0){ return SWITCHMODE_PUSHBUTTON_INV_ID; }
  if(strcasecmp_P(c,PM_SWITCHMODE_PUSHBUTTONHOLD_CTR)==0){ return SWITCHMODE_PUSHBUTTONHOLD_ID; }
  if(strcasecmp_P(c,PM_SWITCHMODE_PUSHBUTTONHOLD_INV_CTR)==0){ return SWITCHMODE_PUSHBUTTONHOLD_INV_ID; }
  if(strcasecmp_P(c,PM_SWITCHMODE_PUSHBUTTON_TOGGLE_CTR)==0){ return SWITCHMODE_PUSHBUTTON_TOGGLE_ID; }
  return -1;
}


const char* mSettings::SwitchMode_GetName_by_ID(uint8_t id, char* buffer, uint8_t buflen){
  switch(id){
    default:
    case SWITCHMODE_TOGGLE_ID:                memcpy_P(buffer, PM_SWITCHMODE_TOGGLE_CTR, sizeof(PM_SWITCHMODE_TOGGLE_CTR)); break;
    case SWITCHMODE_FOLLOW_ID:                memcpy_P(buffer, PM_SWITCHMODE_FOLLOW_CTR, sizeof(PM_SWITCHMODE_FOLLOW_CTR)); break; 
    case SWITCHMODE_FOLLOW_INV_ID:            memcpy_P(buffer, PM_SWITCHMODE_FOLLOW_INV_CTR, sizeof(PM_SWITCHMODE_FOLLOW_INV_CTR)); break; 
    case SWITCHMODE_PUSHBUTTON_ID:            memcpy_P(buffer, PM_SWITCHMODE_PUSHBUTTON_CTR, sizeof(PM_SWITCHMODE_PUSHBUTTON_CTR)); break; 
    case SWITCHMODE_PUSHBUTTON_INV_ID:        memcpy_P(buffer, PM_SWITCHMODE_PUSHBUTTON_INV_CTR, sizeof(PM_SWITCHMODE_PUSHBUTTON_INV_CTR)); break; 
    case SWITCHMODE_PUSHBUTTONHOLD_ID:        memcpy_P(buffer, PM_SWITCHMODE_PUSHBUTTONHOLD_CTR, sizeof(PM_SWITCHMODE_PUSHBUTTONHOLD_CTR)); break; 
    case SWITCHMODE_PUSHBUTTONHOLD_INV_ID:    memcpy_P(buffer, PM_SWITCHMODE_PUSHBUTTONHOLD_INV_CTR, sizeof(PM_SWITCHMODE_PUSHBUTTONHOLD_INV_CTR)); break; 
    case SWITCHMODE_PUSHBUTTON_TOGGLE_ID:     memcpy_P(buffer, PM_SWITCHMODE_PUSHBUTTON_TOGGLE_CTR, sizeof(PM_SWITCHMODE_PUSHBUTTON_TOGGLE_CTR)); break; 
  }
  return buffer;
}





uint16_t mSettings::CountCharInCtr(const char* tosearch, char tofind){
  uint16_t count = 0;
  for(uint16_t i=0;i<strlen(tosearch);i++){
    if(tosearch[i]==tofind){ count++; }
  }
  return count;
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



// /*********************************************************************************************\
//  * Quick power cycle monitoring ie The user can power the device On/Off 7 times (With at most 10 second "On periods", and force a reset)
// \*********************************************************************************************/

// void UpdateQuickPowerCycle(bool update) {
// // #ifndef FIRMWARE_MINIMAL
// //   if (Settings->flag3.fast_power_cycle_disable) { return; }  // SetOption65 - Disable fast power cycle detection for device reset

// //   const uint32_t QPC_COUNT = 7;  // Number of Power Cycles before Settings erase
// //   const uint32_t QPC_SIGNATURE = 0xFFA55AFF;
// // #ifdef USE_COUNTER
// //   CounterInterruptDisable(true);
// // #endif
// // #ifdef ESP8266
// //   const uint32_t qpc_sector = SETTINGS_LOCATION - CFG_ROTATES;
// //   const uint32_t qpc_location = qpc_sector * SPI_FLASH_SEC_SIZE;

// //   uint32_t qpc_buffer[QPC_COUNT +1];
// //   ESP.flashRead(qpc_location, (uint32*)&qpc_buffer, sizeof(qpc_buffer));
// //   if (update && (QPC_SIGNATURE == qpc_buffer[0])) {
// //     uint32_t counter = 1;
// //     while ((0 == qpc_buffer[counter]) && (counter <= QPC_COUNT)) { counter++; }
// //     if (QPC_COUNT == counter) {  // 7 power cycles in a row
// //       SettingsErase(3);          // Quickly reset all settings including QuickPowerCycle flag
// //       EspRestart();              // And restart
// //     } else {
// //       qpc_buffer[0] = 0;
// //       ESP.flashWrite(qpc_location + (counter * 4), (uint32*)&qpc_buffer, 4);
// //       AddLog(LOG_LEVEL_INFO, PSTR("QPC: Count %d"), counter);
// //     }
// //   }
// //   else if ((qpc_buffer[0] != QPC_SIGNATURE) || (0 == qpc_buffer[1])) {
// //     qpc_buffer[0] = QPC_SIGNATURE;
// //     // Assume flash is default all ones and setting a bit to zero does not need an erase
// //     if (ESP.flashEraseSector(qpc_sector)) {
// //       ESP.flashWrite(qpc_location, (uint32*)&qpc_buffer, 4);
// //       AddLog(LOG_LEVEL_INFO, PSTR("QPC: Reset"));
// //     }
// //   }
// // #endif  // ESP8266
// // #ifdef ESP32
// //   uint32_t pc_register;
// //   QPCRead(&pc_register, sizeof(pc_register));
// //   if (update && ((pc_register & 0xFFFFFFF0) == 0xFFA55AF0)) {
// //     uint32_t counter = pc_register & 0xF;  // Allow up to 15 cycles
// //     if (0xF == counter) { counter = 0; }
// //     counter++;
// //     if (QPC_COUNT == counter) {  // 7 power cycles in a row
// //       SettingsErase(3);          // Quickly reset all settings including QuickPowerCycle flag
// //       EspRestart();              // And restart
// //     } else {
// //       pc_register = 0xFFA55AF0 | counter;
// //       QPCWrite(&pc_register, sizeof(pc_register));
// //       AddLog(LOG_LEVEL_INFO, PSTR("QPC: Count %d"), counter);
// //     }
// //   }
// //   else if (pc_register != QPC_SIGNATURE) {
// //     pc_register = QPC_SIGNATURE;
// //     QPCWrite(&pc_register, sizeof(pc_register));
// //     AddLog(LOG_LEVEL_INFO, PSTR("QPC: Reset"));
// //   }
// // #endif  // ESP32
// // #ifdef USE_COUNTER
// //   CounterInterruptDisable(false);
// // #endif
// // #endif  // FIRMWARE_MINIMAL
// }

//   #include "rom/rtc.h"

// // typedef enum {
// //     NO_MEAN                =  0,
// //     POWERON_RESET          =  1,    /**<1, Vbat power on reset*/
// //     SW_RESET               =  3,    /**<3, Software reset digital core*/
// //     OWDT_RESET             =  4,    /**<4, Legacy watch dog reset digital core*/
// //     DEEPSLEEP_RESET        =  5,    /**<3, Deep Sleep reset digital core*/
// //     SDIO_RESET             =  6,    /**<6, Reset by SLC module, reset digital core*/
// //     TG0WDT_SYS_RESET       =  7,    /**<7, Timer Group0 Watch dog reset digital core*/
// //     TG1WDT_SYS_RESET       =  8,    /**<8, Timer Group1 Watch dog reset digital core*/
// //     RTCWDT_SYS_RESET       =  9,    /**<9, RTC Watch dog Reset digital core*/
// //     INTRUSION_RESET        = 10,    /**<10, Instrusion tested to reset CPU*/
// //     TGWDT_CPU_RESET        = 11,    /**<11, Time Group reset CPU*/
// //     SW_CPU_RESET           = 12,    /**<12, Software reset CPU*/
// //     RTCWDT_CPU_RESET       = 13,    /**<13, RTC Watch dog Reset CPU*/
// //     EXT_CPU_RESET          = 14,    /**<14, for APP CPU, reseted by PRO CPU*/
// //     RTCWDT_BROWN_OUT_RESET = 15,    /**<15, Reset when the vdd voltage is not stable*/
// //     RTCWDT_RTC_RESET       = 16     /**<16, RTC Watch dog reset digital core and rtc module*/
// // } RESET_REASON;

// uint32_t ResetReason(void) {
//   /*
//     user_interface.h
//     REASON_DEFAULT_RST      = 0,  // "Power on"                normal startup by power on
//     REASON_WDT_RST          = 1,  // "Hardware Watchdog"       hardware watch dog reset
//     REASON_EXCEPTION_RST    = 2,  // "Exception"               exception reset, GPIO status won’t change
//     REASON_SOFT_WDT_RST     = 3,  // "Software Watchdog"       software watch dog reset, GPIO status won’t change
//     REASON_SOFT_RESTART     = 4,  // "Software/System restart" software restart ,system_restart , GPIO status won’t change
//     REASON_DEEP_SLEEP_AWAKE = 5,  // "Deep-Sleep Wake"         wake up from deep-sleep
//     REASON_EXT_SYS_RST      = 6   // "External System"         external system reset
//   */

// // #ifdef ESP32

//  RESET_REASON reason = rtc_get_reset_reason(0);
//   if (1  == reason) { return REASON_DEFAULT_RST; }       // POWERON_RESET
//   if (12 == reason) { return REASON_SOFT_RESTART; }      // SW_CPU_RESET / RTC_SW_CPU_RESET
//   if (5  == reason) { return REASON_DEEP_SLEEP_AWAKE; }  // DEEPSLEEP_RESET
//   if (3  == reason) { return REASON_EXT_SYS_RST; }       // SW_RESET / RTC_SW_SYS_RESET
//   return -1; //no "official error code", but should work with the current code base

// // #endif 

// #ifdef ESP8266
//  return resetInfo.reason;

// #endif


//   // return ESP_ResetInfoReason();
// }

// #endif // ENABLE_DEVFEATURE_RTC_FASTBOOT_GLOBALTEST_V3

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
  
  // if (Settings.setoption_255[P_BOOT_LOOP_OFFSET]) {
  //   // Disable functionality as possible cause of fast restart within BOOT_LOOP_TIME seconds (Exception, WDT or restarts)
  //   if (RtcReboot.fast_reboot_count > Settings.setoption_255[P_BOOT_LOOP_OFFSET]) {       // Restart twice
  //     Settings.flag_network.user_esp8285_enable = 0;       // Disable ESP8285 Generic GPIOs interfering with flash SPI
  //     if (RtcReboot.fast_reboot_count > Settings.setoption_255[P_BOOT_LOOP_OFFSET] +1) {  // Restart 3 times
  //       // for (uint8_t i = 0; i < MAX_RULE_SETS; i++) {
  //       //   // if (bitRead(Settings.rule_stop, i)) {
  //       //   //   bitWrite(Settings.rule_enabled, i, 0);  // Disable rules causing boot loop
  //       //   // }
  //       // }
  //     }
  //     // if (RtcReboot.fast_reboot_count > Settings.setoption_255[P_BOOT_LOOP_OFFSET] +2) {  // Restarted 4 times
  //     //   Settings.rule_enabled = 0;                  // Disable all rules
  //     // }
  //     if (RtcReboot.fast_reboot_count > Settings.setoption_255[P_BOOT_LOOP_OFFSET] +3) {  // Restarted 5 times
  //       for (uint8_t i = 0; i < sizeof(Settings.module_pins); i++) {
  //         Settings.module_pins.io[i] = GPIO_NONE_ID;         // Reset user defined GPIO disabling sensors
  //       }
  //     }
  //     if (RtcReboot.fast_reboot_count > Settings.setoption_255[P_BOOT_LOOP_OFFSET] +4) {  // Restarted 6 times
  //       Settings.module = MODULE_WEMOS_ID;             // Reset module to Sonoff Basic
  //       Settings.last_module = MODULE_WEMOS_ID;
  //     }
  //     //reset 7 times, then fail into safe boot awaiting OTA
  //     // HandleFailedBootFailBack();
  //   #ifdef ENABLE_LOG_LEVEL_INFO
  //     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_LOG_SOME_SETTINGS_RESET " (%d)"), RtcReboot.fast_reboot_count);
  //     #endif///   #ifdef ENABLE_LOG_LEVEL_INFO
  //   }
  // }

  memset(Settings.mqtt.topic,0,sizeof(Settings.mqtt.topic));
  memcpy(Settings.mqtt.topic,pCONT_set->Settings.system_name.device,strlen(pCONT_set->Settings.system_name.device));
  
  // Configure hostname 
  memset(my_hostname,0,sizeof(my_hostname));
  sprintf(my_hostname,PSTR("%s"),pCONT_set->Settings.system_name.device);

  //Only load wifi here or else set fallback

  // AddLog(LOG_LEVEL_INFO, PSTR(D_PROJECT " %s %s " D_VERSION " %s%s-" ARDUINO_ESP8266_RELEASE), pCONT_set->Settings.system_name.device, Settings.system_name.friendly, my_version, my_image);
  

}





// /********************************************************************************************/


    // template<typename T>
    // int8_t mSettings::Tasker2(uint8_t function, T param1){

    // }
    

// template int8_t mSettings::Tasker<uint8_t, char>(uint8_t function, char param1);


