
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
    // if(runtime.settings_save_decounter_seconds_delayed_save)
    // {
    //   runtime.settings_save_decounter_seconds_delayed_save--;
    // #ifdef ENABLE_LOG_LEVEL_INFO
    //   AddLog(LOG_LEVEL_TEST, PSTR("settings_save_decounter_seconds_delayed_save = %d"), runtime.settings_save_decounter_seconds_delayed_save);
    // #endif // ENABLE_LOG_LEVEL_INFO
    //   if(runtime.settings_save_decounter_seconds_delayed_save==0)
    //   {
    //     pCONT_set->SettingsSaveAll();
    //   }
    // }

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

       // AddLog(LOG_LEVEL_TEST,PSTR("sizeof(SETTINGS)=%d %%"),map(sizeof(SETTINGS),0,4095,0,100));


       
     } break;
    case FUNC_EVERY_FIVE_SECOND:{



    }break;

    case FUNC_EVERY_MINUTE:

      
      #ifdef USE_MODULE_DRIVERS_FILESYSTEM
      #ifdef ENABLE_SYSTEM_SETTINGS_IN_FILESYSTEM
        // Copy Settings as Last Known Good if no changes have been saved since 30 minutes
        if (!runtime.settings_lkg && (pCONT_time->UtcTime() > START_VALID_UTC_TIME) && (Settings.cfg_timestamp < pCONT_time->UtcTime() - (3 * 60))) 
        {
          pCONT_mfile->TfsSaveFile(TASM_FILE_SETTINGS_LKG_LAST_KNOWN_GOOD, (const uint8_t*)&Settings, sizeof(SETTINGS));
          runtime.settings_lkg = true;
        }
        else
        {
          ALOG_INF(PSTR("UtcTime()%d > START_VALID_UTC_TIME%d) && (Settings.cfg_timestamp%d < UtcTime() - (30 * 60)) %d"), 
            pCONT_time->UtcTime(),
            START_VALID_UTC_TIME,
            Settings.cfg_timestamp,
            pCONT_time->UtcTime() - (3 * 60)
         );
        }
      #endif // ENABLE_SYSTEM_SETTINGS_IN_FILESYSTEM
      #endif // USE_MODULE_DRIVERS_FILESYSTEM

      
    break;
    case FUNC_EVERY_HOUR:

      #ifdef ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
        #ifdef ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
        pCONT_set->SettingsSaveAll();
        #endif
      #else 
      #ifdef ENABLE_LOG_LEVEL_INFO
      DEBUG_PRINTLN("SettingsSave dis");
      #endif // ifdef ENABLE_LOG_LEVEL_INFO
      #endif // ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING




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
    // case FUNC_EVERY_FIVE_MINUTE:

    //   ALOG_INF( PSTR(D_LOG_APPLICATION D_BOOT_COUNT " = %d"), Settings.bootcount);

    // break;
    
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    case FUNC_FILESYSTEM_APPEND_JSON__CONFIG_SETTINGS__ID:
      JsonAppend_Settings();
    break;

    /************
     * xx SECTION * 
    *******************/

    case FUNC_TEMPLATE_DEVICE_LOAD_FROM_PROGMEM:
      Function_Template_Load();
    break;
  }

} 




// load in driver and sensor template settings
void mSettings::Function_Template_Load(){

  #ifndef DISABLE_SERIAL_LOGGING
  //DEBUG_PRINTF("mSettings::Function_Template_Load"); Serial.flush();
  #endif

  runtime.boot_status.function_template_parse_success = 0;
  runtime.boot_status.rules_template_parse_success = 0;

  #ifdef USE_FUNCTION_TEMPLATE  
  // Read into local
  D_DATA_BUFFER_CLEAR();
  memcpy_P(data_buffer.payload.ctr,FUNCTION_TEMPLATE,sizeof(FUNCTION_TEMPLATE));
  data_buffer.payload.len = strlen(data_buffer.payload.ctr);

  #ifdef ENABLE_LOG_LEVEL_INFO
  // AddLog(LOG_LEVEL_DEBUG, PSTR("FUNCTION_TEMPLATE Load"));// = \"%d|%s\""),data_buffer.payload.len, data_buffer.payload.ctr);
  ALOG_INF( PSTR(DEBUG_INSERT_PAGE_BREAK  "FUNCTION_TEMPLATE READ = \"%d|%s\""),data_buffer.payload.len, data_buffer.payload.ctr);
  #endif // ENABLE_LOG_LEVEL_INFO

  pCONT->Tasker_Interface(FUNC_JSON_COMMAND_ID);

  delay(1000);

  runtime.boot_status.function_template_parse_success = 1;
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

void mSettings::JsonAppend_Settings()
{

  // JBI->Object_Start_F(GetModuleFriendlyName());  //json file parser will pass them to the modules, but should strip out the level_object from commands
    JBI->Add("BootCount", Settings.bootcount);
    #ifdef ENABLE_DEVFEATURE_FASTBOOT_DETECTION
    JBI->Add("FastBootCount", RtcFastboot.fast_reboot_count);
    #endif
  // JBI->Object_End();

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

//into logging
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



/**
 * If true, then load important info from settings struct into runtime values, else, remain on old settings
 * */
void mSettings::SettingsLoad_CheckSuccessful()
{
  
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
  memset(runtime.my_hostname,0,sizeof(runtime.my_hostname));
  sprintf(runtime.my_hostname,PSTR("%s"),pCONT_set->Settings.system_name.device);

  //Only load wifi here or else set fallback

  // AddLog(LOG_LEVEL_INFO, PSTR(D_PROJECT " %s %s " D_VERSION " %s%s-" ARDUINO_ESP8266_RELEASE), pCONT_set->Settings.system_name.device, Settings.system_name.friendly, my_version, my_image);
  

}

