
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

      #ifdef ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
        #ifdef ENABLE_DEBUGFEATURE_SETTINGS_STORAGE__ENABLED_SETTINGS_SAVE_EVERY_MINUTE_FOR_DEBUG
        pCONT_set->SettingsSaveAll();
        #endif
      #endif


    }break;

    case FUNC_EVERY_MINUTE:

      
      #ifdef USE_MODULE_CORE_FILESYSTEM
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
      #endif // USE_MODULE_CORE_FILESYSTEM


      
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
        // if (BOOT_LOOP_TIME == pCONT_time-> uptime_seconds_nonreset) {

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

  #ifdef USE_FUNCTION_TEMPLATE  
  // Read into local
  D_DATA_BUFFER_CLEAR();
  memcpy_P(data_buffer.payload.ctr,FUNCTION_TEMPLATE,sizeof(FUNCTION_TEMPLATE));
  data_buffer.payload.length_used = strlen(data_buffer.payload.ctr);

  #ifdef ENABLE_LOG_LEVEL_INFO
  //ALOG_INF( PSTR(DEBUG_INSERT_PAGE_BREAK  "FUNCTION_TEMPLATE READ = \"%d|%s\""),data_buffer.payload.length_used, data_buffer.payload.ctr);
  #endif // ENABLE_LOG_LEVEL_INFO

  pCONT->Tasker_Interface(FUNC_JSON_COMMAND_ID);

  runtime.template_loading.status.function = TemplateSource::HEADER_TEMPLATE;

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






#ifdef ENABLE_DEVFEATURE_SETTINGS__TEXT_BUFFER


/*********************************************************************************************\
 * Config Settings->text char array support
\*********************************************************************************************/

uint32_t mSettings::GetSettingsTextLen(void) {
  char* position = Settings.text_pool;
  for (uint32_t size = 0; size < SET_MAX; size++) {
    while (*position++ != '\0') { }
  }
  return position - Settings.text_pool;
}

bool settings_text_mutex = false;
uint32_t settings_text_busy_count = 0;

bool mSettings::SettingsUpdateFinished(void) {
  uint32_t wait_loop = 10;
  while (settings_text_mutex && wait_loop) {  // Wait for any update to finish
    yield();
    delayMicroseconds(1);
    wait_loop--;
  }
  return (wait_loop > 0);  // true if finished
}

bool mSettings::SettingsUpdateText(uint32_t index, const char* replace_me) 
{

  ALOG_INF(PSTR("SettingsUpdateText %d %s"),index,replace_me);

  if (index >= SET_MAX) {
    return false;  // Setting not supported - internal error
  }

  // Make a copy first in case we use source from Settings->text
  uint32_t replace_len = strlen_P(replace_me);
  char replace[replace_len +1];
  memcpy_P(replace, replace_me, sizeof(replace));
  uint32_t index_save = index;

  uint32_t start_pos = 0;
  uint32_t end_pos = 0;
  char* position = Settings.text_pool;
  for (uint32_t size = 0; size < SET_MAX; size++) {
    while (*position++ != '\0') { }
    if (1 == index) {
      start_pos = position - Settings.text_pool;
    }
    else if (0 == index) {
      end_pos = position - Settings.text_pool -1;
    }
    index--;
  }
  uint32_t char_len = position - Settings.text_pool;

  uint32_t current_len = end_pos - start_pos;
  int diff = replace_len - current_len;

 AddLog(LOG_LEVEL_INFO, PSTR("TST: start %d, end %d, len %d, current %d, replace %d, diff %d"),
   start_pos, end_pos, char_len, current_len, replace_len, diff);

  int too_long = (char_len + diff) - settings_text_size;
  if (too_long > 0) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "Text overflow by %d char(s)"), too_long);
    return false;  // Replace text too long
  }

  if (settings_text_mutex && !SettingsUpdateFinished()) {
    settings_text_busy_count++;
  } else {
    settings_text_mutex = true;

    if (diff != 0) {
      // Shift Settings->text up or down
      memmove_P(Settings.text_pool + start_pos + replace_len, Settings.text_pool + end_pos, char_len - end_pos);
    }
    // Replace text
    memmove_P(Settings.text_pool + start_pos, replace, replace_len);
    // Fill for future use
    memset(Settings.text_pool + char_len + diff, 0x00, settings_text_size - char_len - diff);

    settings_text_mutex = false;
  }

#ifdef DEBUG_FUNC_SETTINGSUPDATETEXT
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_CONFIG "CR %d/%d, Busy %d, Id %02d = \"%s\""), GetSettingsTextLen(), settings_text_size, settings_text_busy_count, index_save, replace);
Serial.println("test_pool");
for(int i=0;i<100;i++)
{
  Serial.print(Settings.text_pool[i]);
}
Serial.println();
#else
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_CONFIG "CR %d/%d, Busy %d"), GetSettingsTextLen(), settings_text_size, settings_text_busy_count);
#endif


  return true;
}

char* mSettings::SettingsText(uint32_t index) {
  char* position = Settings.text_pool;

  if (index >= SET_MAX) { // Index above SET_MAX are not stored in Settings
#ifdef USE_WEBSERVER
    if (SET_BUTTON17 <= index && index <= SET_BUTTON32)
      return (char*)GetWebButton(index-SET_BUTTON17+16);
#endif
    position += settings_text_size -1;  // Setting not supported - internal error - return empty string
  } else {
    SettingsUpdateFinished();
    for (;index > 0; index--) {
      while (*position++ != '\0') { }
    }
  }
  return position;
}

#endif // ENABLE_DEVFEATURE_SETTINGS__TEXT_BUFFER