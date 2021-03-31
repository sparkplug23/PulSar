/*
  settings.ino - user settings for Sonoff-Tasmota

  Copyright (C) 2019  Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "2_CoreSystem/Settings/mSettings.h"

struct DATA_BUFFER data_buffer;

// struct TIME_T RtcTime;
// struct TIMERULES TimeRules;
// union TimeRule TimeRuleTest;


const char* mSettings::PM_MODULE_CORE_SETTINGS_CTR = D_MODULE_CORE_SETTINGS_CTR;
const char* mSettings::PM_MODULE_CORE_SETTINGS_FRIENDLY_CTR = D_MODULE_CORE_SETTINGS_FRIENDLY_CTR;




// /*********************struct DATA_BUFFER2 data_buffer;***********************************************************************/
//
// CONSIDER combining driver/sensor into one module "AddSensor"

void mSettings::ClearAllDeviceName(void){

  data_buffer.payload.ctr[0] = 0;

  memset(&Settings.device_name_buffer.name_buffer,0,sizeof(Settings.device_name_buffer.name_buffer));
  memset(&Settings.device_name_buffer.class_id,DEVICENAME_EMPTY_ID,sizeof(Settings.device_name_buffer.class_id));
  memset(&Settings.device_name_buffer.device_id,DEVICENAME_EMPTY_ID,sizeof(Settings.device_name_buffer.device_id));

}

//returns length
// Other variations needed:
//  - Overwrite existing devicename
//  - Remove by name OR by ID
int8_t mSettings::AddDeviceName(const char* name_ctr, int16_t class_id, int8_t device_id){

  char* buffer = Settings.device_name_buffer.name_buffer;
  uint16_t buffer_length = strlen(buffer);
    #ifdef ENABLE_LOG_LEVEL_INFO
  //AddLog_P(LOG_LEVEL_INFO,PSTR("mSettings::AddDeviceName len=%d"),buffer_length);
    #endif// ENABLE_LOG_LEVEL_INFO
  buffer_length = buffer_length > DEVICENAMEBUFFER_NAME_BUFFER_LENGTH ? DEVICENAMEBUFFER_NAME_BUFFER_LENGTH : buffer_length;
  
  #ifdef USE_LOG
  //AddLog_P(LOG_LEVEL_INFO,PSTR("name_bufferB=%s"), buffer);
  #endif
  // Check indexing
  uint8_t index = 0;
  if(buffer_length){ //if anything in buffer
    index = CountCharInCtr(buffer,'|'); // first | indicates index from 0
  }
  
  // Check not above "id" limits
  if(index >= (DEVICENAMEBUFFER_NAME_INDEX_LENGTH-1)){
    return 0; // too many devices in list
  }

  uint16_t new_buffer_length = buffer_length+strlen(name_ctr);

    #ifdef ENABLE_LOG_LEVEL_INFO
  // AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR("AddDeviceName(%s,%d,%d) %d index=%d"), 
  //   name_ctr, 
  //   class_id, 
  //   device_id, 
  //   buffer_length, 
  //   index
  // );
    #endif// ENABLE_LOG_LEVEL_INFO
  //AddLog_P(LOG_LEVEL_INFO,PSTR("name_bufferB=%s"), buffer);

  // Write name to next slot
  if(new_buffer_length<DEVICENAMEBUFFER_NAME_BUFFER_LENGTH){
    buffer_length+=sprintf(buffer+buffer_length, "%s|", name_ctr); 
    #ifdef ENABLE_LOG_LEVEL_INFO
    // AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR("AddDeviceName ADDED + \"%s\""),name_ctr); 
    #endif // ENABLE_LOG_LEVEL_INFO
  }

  // Add index to next slot
  Settings.device_name_buffer.class_id[index] = class_id;
  Settings.device_name_buffer.device_id[index] = device_id;

    #ifdef ENABLE_LOG_LEVEL_INFO
  // AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR("name_bufferE=%s"), buffer);
  // AddLog_Array(LOG_LEVEL_DEBUG_MORE,PSTR("class_id"),Settings.device_name_buffer.class_id,(int16_t)DEVICENAMEBUFFER_NAME_INDEX_LENGTH);
  // AddLog_Array(LOG_LEVEL_DEBUG_MORE,PSTR("device_id"),Settings.device_name_buffer.device_id,(int8_t)DEVICENAMEBUFFER_NAME_INDEX_LENGTH);
    #endif// ENABLE_LOG_LEVEL_INFO

}

int8_t mSettings::RemoveDeviceName(const char* name_ctr, int16_t class_id, int8_t device_id){
  // Find name, shift results to remove found name
  // Not needed until I want to remove values
}

const char* mSettings::GetDeviceName(int16_t module_id, int8_t device_id, char* buffer, uint16_t buffer_size){

  int8_t found_index = -1;
  // Check if class & id match
  for(int i=0;i<DEVICENAMEBUFFER_NAME_INDEX_LENGTH;i++){
    if((Settings.device_name_buffer.class_id[i]==module_id)&&(Settings.device_name_buffer.device_id[i]==device_id)){
      found_index = i;
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR("mSettings::GetDeviceName found_index %d"),i);
    #endif // ENABLE_LOG_LEVEL_INFO
      break;
    }
  }
  //future, if none found, have a list of the prefered defaults, relay%d, sensor%d etc

  if(found_index == -1){
    memcpy(buffer,PM_SEARCH_NOMATCH,sizeof(PM_SEARCH_NOMATCH));
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("F::%s >> %s"),__FUNCTION__,PM_SEARCH_NOMATCH);
    #endif // ENABLE_LOG_LEVEL_INFO
    return buffer;
  }

  char* name_buffer = Settings.device_name_buffer.name_buffer;
    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR("GetDeviceName len=%d"),strlen(buffer));
    #endif // ENABLE_LOG_LEVEL_INFO
  // gets first index from the array, where we start at the position the desired name is the next name
  pCONT_sup->GetTextIndexed(buffer, buffer_size, found_index, name_buffer);
    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR("GetDeviceName=%s"),buffer);
    #endif // ENABLE_LOG_LEVEL_INFO
  // AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR("GetDeviceName &name_buffer[index]=%s"),&name_buffer[index]);

  if(buffer == nullptr){
    memcpy(buffer,PM_SEARCH_NOMATCH,sizeof(PM_SEARCH_NOMATCH));
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_ERROR, PSTR("F::%s ERROR >> %s"),__FUNCTION__,PM_SEARCH_NOMATCH);
    #endif // ENABLE_LOG_LEVEL_INFO
  }

  return buffer;
  
}

// search for name, return its index position using pointers
// search for name within devicebuffer, return its class/device id's
// int8_t mSettings::GetDeviceIDbyName(int8_t* class_id, int8_t* device_id, char* name_tofind){

//   // pCONT_sup->GetText





//   return *device_id;
// }


// if class_id is nullptr (default value), then ignore matching class first and return first instance of device name
// if class_id is passed, then limit matching results to include that class
int16_t mSettings::GetDeviceIDbyName(const char* name_tofind, int8_t device_id, int8_t class_id){
//   return GetDeviceIDbyName(name_tofind, Settings.device_name_buffer.name_buffer, &device_id, &class_id);
// }


// // if class_id is nullptr (default value), then ignore matching class first and return first instance of device name
// // if class_id is passed, then limit matching results to include that class
// int16_t mSettings::GetDeviceIDbyName(const char* name_tofind, const char* haystack, int8_t* device_id, int8_t* class_id)
// {
  const char* haystack = Settings.device_name_buffer.name_buffer;
  const char* read = haystack;
  int16_t position = -1;

  char name_tofind_with_delimeter[50];
  snprintf(name_tofind_with_delimeter,sizeof(name_tofind_with_delimeter),"%s|",name_tofind);


    // AddLog_P(LOG_LEVEL_INFO,PSTR("Name_tofind_with_delimeter = %s"),name_tofind_with_delimeter);

  // Search for substring
  char *p_start_of_found = strstr(haystack,name_tofind_with_delimeter);

  if(p_start_of_found == NULL){
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR("p_start_of_found == NOT FOUND"));
    #endif// ENABLE_LOG_LEVEL_INFO
    return -1;
  }

  bool limit_result_to_class_ids = true;
  // if((class_id == nullptr)||
  if(class_id == -1){
    limit_result_to_class_ids = false;
  }
    #ifdef ENABLE_LOG_LEVEL_INFO
  
  AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR("limit_result_to_class_ids = %s"),limit_result_to_class_ids?"YES":"NO");

    #endif// ENABLE_LOG_LEVEL_INFO

  uint8_t delimeter_count = 0;
  uint8_t delimeter_within_class_count = 0;
  uint16_t haystack_index = 0;
  uint16_t haystack_length = strlen(haystack);
  //search for delimeters between found index and start ie indexed list places
  while((read != p_start_of_found) && (haystack_index++ < haystack_length)){
    // Count delimeters
    if(*read == '|'){
      if(limit_result_to_class_ids){
        if(pCONT_set->Settings.device_name_buffer.class_id[delimeter_count] == class_id){
          delimeter_within_class_count++;
          // #ifdef ENABLE_LOG_LEVEL_INFO
          // // AddLog_P(LOG_LEVEL_TEST,PSTR("\n\r%s\n\r found wclass_count\n\r %s\n\r %d %d\n\r\n\r"),haystack,read,delimeter_within_class_count,Settings.device_name_buffer.class_id[delimeter_count]);
          // AddLog_P(LOG_LEVEL_TEST,
          //   PSTR(
          //     "Searching \"%s\"\n\r"
          //     "Found \"%s\" @ index %d\n\r"
          //     "del count/index pos %d\n\r"
          //   ),         
          //   name_tofind,
          //   p_start_of_found,
          //   p_start_of_found - read, // pointer distance
          //   delimeter_within_class_count
          // );
          // #endif // ENABLE_LOG_LEVEL_INFO
        }
      }else{
        delimeter_within_class_count++;        
      }
      delimeter_count++;
    #ifdef ENABLE_LOG_LEVEL_INFO
      // AddLog_P(LOG_LEVEL_INFO,PSTR("%s found %s %d"),haystack,read,delimeter_count);
    #endif // ENABLE_LOG_LEVEL_INFO
    }
    read++; //move pointer along
  }

  #ifdef ENABLE_LOG_LEVEL_INFO
          // AddLog_P(LOG_LEVEL_TEST,PSTR("\n\r%s\n\r found wclass_count\n\r %s\n\r %d %d\n\r\n\r"),haystack,read,delimeter_within_class_count,Settings.device_name_buffer.class_id[delimeter_count]);
          AddLog_P(LOG_LEVEL_TEST,
            PSTR(
              "\n\rSearching \"%s\"\n\r"
              "Found \"%s\" @ index %d\n\r"
              "del count/index pos %d\n\r"
            ),         
            name_tofind,
            p_start_of_found,
            p_start_of_found - haystack, // pointer distance
            delimeter_within_class_count
          );
          #endif // ENABLE_LOG_LEVEL_INFO

  return delimeter_within_class_count;
}




int16_t mSettings::GetIndexOfNthCharPosition(const char* tosearch, char tofind, uint8_t occurance_count){
  uint16_t count = 0;
  for(int16_t i=0;i<strlen(tosearch);i++){
    if(tosearch[i]==tofind) count++;
    if(count == occurance_count) return i;
  }
  return -1;
}

// If no class_id provided, return total driver count, not just within that class
int8_t mSettings::GetDeviceNameCount(int16_t class_id){
  int8_t count = 0;
  for(int i=0;i<DEVICENAMEBUFFER_NAME_INDEX_LENGTH;i++){
    if(Settings.device_name_buffer.class_id[i]==class_id){
      count++;
      break;
    }
  }
  return count;
}

uint16_t mSettings::CountCharInCtr(const char* tosearch, char tofind){
  uint16_t count = 0;
  for(uint16_t i=0;i<strlen(tosearch);i++){
    if(tosearch[i]==tofind){ count++; }
  }
  return count;
}



// load in driver and sensor template settings
void mSettings::Function_Template_Load(){

  #ifndef DISABLE_SERIAL_LOGGING
  Serial.printf("mSettings::Function_Template_Load"); Serial.flush();
  #endif

  boot_status.function_template_parse_success = 0;
  boot_status.rules_template_parse_success = 0;

  #ifdef USE_FUNCTION_TEMPLATE  
  // Read into local
  D_DATA_BUFFER_CLEAR();
  memcpy_P(data_buffer.payload.ctr,FUNCTION_TEMPLATE,sizeof(FUNCTION_TEMPLATE));
  data_buffer.payload.len = strlen(data_buffer.payload.ctr);

  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog_P(LOG_LEVEL_TEST, PSTR("FUNCTION_TEMPLATE READ = \"%d|%s\""),data_buffer.payload.len, data_buffer.payload.ctr);
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
  AddLog_P(LOG_LEVEL_TEST, PSTR("RULES_TEMPLATE READ = \"%d|%s\""),data_buffer.payload.len, data_buffer.payload.ctr);
  #endif // ENABLE_LOG_LEVEL_INFO

  pCONT->Tasker_Interface(FUNC_JSON_COMMAND_ID);

  boot_status.rules_template_parse_success = 1;
  #endif //USE_RULES_TEMPLATE

  
}

int16_t mSettings::GetFunctionIDbyFriendlyName(const char* c){

  if(c=='\0'){
    return -1;
  }
  if(strcasecmp_P(c,PM_FUNC_EVENT_INPUT_STATE_CHANGED_CTR)==0){ return FUNC_EVENT_INPUT_STATE_CHANGED_ID; }

  if(strcasecmp_P(c,PM_FUNC_EVENT_MOTION_STARTED_CTR)==0){ return FUNC_EVENT_MOTION_STARTED_ID; }


  if(strcasecmp_P(c,PM_FUNC_EVENT_SET_POWER_CHANGED_CTR)==0){ return FUNC_EVENT_SET_POWER_ID; }


  return -1;
}



// Settings will contain all jsoncommands for "CoreSystem"

//overload fix when only one parameter is called
int8_t mSettings::Tasker(uint8_t function){//}, uint8_t param1){  
//   JsonObjectConst dummy; return Tasker(function, dummy);
// }
// template<typename T>
// int8_t mSettings::Tasker(uint8_t function, T param1){ 

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
      //   //     AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_MEMORY D_LOAD " 1before"));
      //   // pCONT->Tasker_Interface(FUNC_SETTINGS_DEFAULT);
      //   // AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_MEMORY D_LOAD " 1after"));

      // }

    break;
    case FUNC_EVERY_SECOND:{

    // Function_Template_Load();

    
  // AddLog_P(LOG_LEVEL_DEBUG,PSTR( "TaskerTest SUCCESS!!"));
// SystemSettings_DefaultBody_Network();
      //AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_BOOT_COUNT " = %d"), Settings.bootcount);

       // AddLog_P(LOG_LEVEL_TEST,PSTR("sizeof(SYSCFG)=%d %%"),map(sizeof(SYSCFG),0,4095,0,100));
     } break;
    case FUNC_EVERY_FIVE_SECOND:{


#ifdef ENABLE_DEVFEATURE_PERIODIC_RULE_FILLING

  D_DATA_BUFFER_CLEAR();
  memcpy_P(data_buffer.payload.ctr,RULES_TEMPLATE,sizeof(RULES_TEMPLATE));
  data_buffer.payload.len = strlen(data_buffer.payload.ctr);

  #ifdef ENABLE_LOG_LEVEL_INFO
  // AddLog_P(LOG_LEVEL_TEST, PSTR("RULES_TEMPLATE READ = \"%d|%s\""),data_buffer.payload.len, data_buffer.payload.ctr);
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

  AddLog_P(LOG_LEVEL_TEST,PSTR("FUNC_JSON_COMMAND_ID1=%s"),data_buffer.payload.ctr);

  pCONT->Tasker_Interface(FUNC_JSON_COMMAND_ID);
  
  // snprintf(data_buffer.payload.ctr,pCONT_rules->rules[0].command.);
  // // memcpy_P(data_buffer.payload.ctr,RULES_TEMPLATE,sizeof(RULES_TEMPLATE));
  // data_buffer.payload.len = strlen(data_buffer.payload.ctr);

#endif // ENABLE_DEVFEATURE_PERIODIC_RULE_FILLING



      // int8_t device_id;
      // int8_t class_id = D_MODULE_DRIVERS_RELAY_ID;
      // AddLog_P(LOG_LEVEL_INFO,PSTR("FUNC_EVERY_FIVE_SECOND\n\r\n\r"));

      // int16_t device_id_found = GetDeviceIDbyName("Socket",pCONT_set->Settings.device_name_buffer.name_buffer,&device_id,&class_id);
      // AddLog_P(LOG_LEVEL_INFO,PSTR("\n\r\n\rdevice_id_found = %d"),device_id_found);

      // device_id_found = GetDeviceIDbyName("Plug",pCONT_set->Settings.device_name_buffer.name_buffer,&device_id,&class_id);
      // AddLog_P(LOG_LEVEL_INFO,PSTR("\n\r\n\rdevice_id_found = %d"),device_id_found);

      // device_id_found = GetDeviceIDbyName("Plug2",pCONT_set->Settings.device_name_buffer.name_buffer,&device_id,&class_id);
      // AddLog_P(LOG_LEVEL_INFO,PSTR("\n\r\n\rdevice_id_found = %d"),device_id_found);

      // device_id_found = GetDeviceIDbyName("Plug3",pCONT_set->Settings.device_name_buffer.name_buffer,&device_id,&class_id);
      // AddLog_P(LOG_LEVEL_INFO,PSTR("\n\r\n\rdevice_id_found = %d"),device_id_found);

    }break;

    case FUNC_EVERY_MINUTE:
    // Change to saving using counter later, Settings.save_data
      // pCONT->Tasker_Interface(FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE);

      // Serial.println("FUNC_EVERY_MINUTE");
      
// #ifdef DISABLE_SETTINGS_SAVING_BUG
      // pCONT_set->SettingsSave(1);
      Serial.println("SettingsSave dis");
    // #endif
    break;
    case FUNC_ON_SUCCESSFUL_BOOT:
      Settings.bootcount++;              // Moved to here to stop flash writes during start-up
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_BOOT_COUNT "SUCCESSFUL BOOT %d"), Settings.bootcount);
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
    case FUNC_JSON_COMMAND_CHECK_TOPIC_ID:
      CheckAndExecute_JSONCommands();
    break;
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand();
    break;


    case FUNC_TEMPLATE_DEVICE_LOAD:
      Function_Template_Load();
    break;
  }

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
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_UPLOAD_ERR_2));  // Not enough (memory) space
    #endif // ENABLE_LOG_LEVEL_INFO
    return false;
  }
  return true;
    #endif // ESP8266
}

void mSettings::SettingsSaveAll(void)
{
  // Serial.println("SettingsSaveAll");
  // if (Settings.flag_system.save_state) {
  //   Settings.power = power;
  // } else {
  //   Settings.power = 0;
  // }
  pCONT->Tasker_Interface(FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE);
#ifdef USE_EEPROM
  EepromCommit();
#endif
  SettingsSave(0);
}

// /*********************************************************************************************\
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
//  * Config Save - Save parameters to Flash ONLY if any parameter has changed
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/

// #ifdef ENABLE_DEVFEATURE_SETTINGS_V2

void mSettings::SettingsLoad(void) {
  
    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_MEMORY D_LOAD));
    #endif// ENABLE_LOG_LEVEL_INFO

#ifdef ESP8266
  // Load configuration from eeprom or one of 7 slots below if first valid load does not stop_flash_rotate
  // Activated with version 8.4.0.2 - Fails to read any config before version 6.6.0.11
  settings_location = 0;
  uint32_t save_flag = 0;
  uint32_t flash_location = SETTINGS_LOCATION;
  for (uint32_t i = 0; i < CFG_ROTATES; i++) {              // Read all config pages in search of valid and latest
    ESP.flashRead(flash_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(Settings));
    
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_MEMORY D_LOAD " i=%d bootcount=%d version=%X"),i,Settings.bootcount,Settings.version);
    #endif // ENABLE_LOG_LEVEL_INFO

    if ((Settings.cfg_crc32 != 0xFFFFFFFF) && (Settings.cfg_crc32 != 0x00000000) && (Settings.cfg_crc32 == GetSettingsCrc32())) {
      
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_MEMORY D_LOAD " cfg_crc==GetSettingsCrc32()| %d==%d"),Settings.cfg_crc32,GetSettingsCrc32());
    #endif // ENABLE_LOG_LEVEL_INFO
      
      if (Settings.save_flag > save_flag) {                 // Find latest page based on incrementing save_flag
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_MEMORY D_LOAD " Settings.save_flag > save_flag %d>%d"),Settings.save_flag,save_flag);
    #endif // ENABLE_LOG_LEVEL_INFO
     
        save_flag = Settings.save_flag;
        settings_location = flash_location;
        if (Settings.flag_system.stop_flash_rotate && (0 == i)) {  // Stop if only eeprom area should be used and it is valid
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_MEMORY D_LOAD " Settings.flag_system.stop_flash_rotate && (0 == i)"));
    #endif // ENABLE_LOG_LEVEL_INFO
          break;
        }
      }
    }
    flash_location--;
    delay(1);
  }
  if (settings_location > 0) {
    ESP.flashRead(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(Settings));
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_CONFIG D_LOADED_FROM_FLASH_AT " %X, " D_COUNT " %lu " D_JSON_BOOTCOUNT " %d"), settings_location, Settings.save_flag, Settings.bootcount);
    #endif // ENABLE_LOG_LEVEL_INFO
  }
// #else  // ESP32
//   SettingsRead(&Settings, sizeof(Settings));
//   AddLog_P2(LOG_LEVEL_NONE, PSTR(D_LOG_CONFIG "Loaded, " D_COUNT " %lu"), Settings.save_flag);
// #endif  // ESP8266 - ESP32

#ifndef FIRMWARE_MINIMAL
  if ((0 == settings_location) || (Settings.cfg_holder != (uint16_t)SETTINGS_HOLDER)) {  // Init defaults if cfg_holder differs from user settings in my_user_config.h
    //Settings.seriallog_level = LOG_LEVEL_ALL;
    pCONT_set->Settings.seriallog_level = pCONT_set->seriallog_level_during_boot;
    
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_MEMORY "cfg_holder(%d) != SETTINGS_HOLDER(%d), Erasing"),Settings.cfg_holder,SETTINGS_HOLDER);
    #endif // ENABLE_LOG_LEVEL_INFO

    SettingsDefault();
  }else{
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_MEMORY D_LOAD " " D_JSON_SUCCESSFUL ));
    #endif // ENABLE_LOG_LEVEL_INFO
  }
  settings_crc32 = GetSettingsCrc32();
#endif  // FIRMWARE_MINIMAL

  RtcSettingsLoad();

  #endif //  ESP8266
}


uint16_t mSettings::GetCfgCrc16(uint8_t *bytes, uint32_t size)
{
  uint16_t crc = 0;

  for (uint32_t i = 0; i < size; i++) {
    if ((i < 14) || (i > 15)) { crc += bytes[i]*(i+1); }  // Skip crc
  }
  return crc;
}

uint16_t mSettings::GetSettingsCrc(void)
{
  // Fix miscalculation if previous Settings was 3584 and current Settings is 4096 between 0x06060007 and 0x0606000A
  uint32_t size = /*((Settings.version < 0x06060007) || (Settings.version > 0x0606000A)) ? 3584 :*/ sizeof(Settings);
  return GetCfgCrc16((uint8_t*)&Settings, size);
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



void mSettings::SettingsSave(uint8_t rotate)
{

// Serial.println("SettingsSave NOT return");
//   return;
  
    #ifdef ESP8266
/* Save configuration in eeprom or one of 7 slots below
 *
 * rotate 0 = Save in next flash slot
 * rotate 1 = Save only in eeprom flash slot until SetOption12 0 or restart
 * rotate 2 = Save in eeprom flash slot, erase next flash slots and continue depending on stop_flash_rotate
 * stop_flash_rotate 0 = Allow flash slot rotation (SetOption12 0)
 * stop_flash_rotate 1 = Allow only eeprom flash slot use (SetOption12 1)
 */
// #ifndef FIRMWARE_MINIMAL
  // UpdateBackwardCompatibility();
  if ((GetSettingsCrc32() != settings_crc32) || rotate) {
     if (1 == rotate) {   // Use eeprom flash slot only and disable flash rotate from now on (upgrade)
      stop_flash_rotate = 1;
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_MEMORY D_SAVE " stop_flash_rotate"));//(upgrade) Use eeprom flash slot only and disable flash rotate from now on"));
    #endif// ENABLE_LOG_LEVEL_INFO
    }
    if (2 == rotate) {   // Use eeprom flash slot and erase next flash slots if stop_flash_rotate is off (default)
      settings_location = SETTINGS_LOCATION +1;
    // #ifdef ENABLE_LOG_LEVEL_INFO
    //   AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_MEMORY D_SAVE " (default) Use eeprom flash slot and erase next flash slots if stop_flash_rotate is off(%d) (default)"),stop_flash_rotate);
    // #endif// ENABLE_LOG_LEVEL_INFO
    }
    if (stop_flash_rotate) {
      settings_location = SETTINGS_LOCATION;
    } else {
      settings_location--;
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_MEMORY D_SAVE " settings_location=%d"),settings_location);
    #endif// ENABLE_LOG_LEVEL_INFO
      if (settings_location <= (SETTINGS_LOCATION - CFG_ROTATES)) {
        settings_location = SETTINGS_LOCATION;
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_TEST,PSTR("settings_location <= (SETTINGS_LOCATION - CFG_ROTATES)"));
    #endif // ENABLE_LOG_LEVEL_INFO
      }
    }

    Settings.save_flag++;
    // if (UtcTime() > START_VALID_TIME) {
    //   Settings.cfg_timestamp = UtcTime();
    // } else {
    //   Settings.cfg_timestamp++;
    // }
    Settings.cfg_size = sizeof(Settings);
    Settings.cfg_crc = GetSettingsCrc();  // Keep for backward compatibility in case of fall-back just after upgrade
    Settings.cfg_crc32 = GetSettingsCrc32();

// #ifdef ESP8266
    if (ESP.flashEraseSector(settings_location)) {
      ESP.flashWrite(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(Settings));
    }

    if (!stop_flash_rotate && rotate) {
      for (uint32_t i = 1; i < CFG_ROTATES; i++) {
        ESP.flashEraseSector(settings_location -i);  // Delete previous configurations by resetting to 0xFF
        delay(1);
      }
    }
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_CONFIG D_SAVED_TO_FLASH_AT " %X, " D_COUNT " %d, " D_BYTES " %d"), settings_location, Settings.save_flag, sizeof(Settings));
    #endif// ENABLE_LOG_LEVEL_INFO
// #else  // ESP32
//     SettingsWrite(&Settings, sizeof(Settings));
//     AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_CONFIG "Saved, " D_COUNT " %d, " D_BYTES " %d"), Settings.save_flag, sizeof(Settings));
// #endif  // ESP8266

    settings_crc32 = Settings.cfg_crc32;
  }
// #endif  // FIRMWARE_MINIMAL
  RtcSettingsSave();
  #endif //  ESP8266
}



// #else // ENABLE_DEVFEATURE_SETTINGS_V2


// void mSettings::SettingsLoad(void)
// {
//   AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_MEMORY D_LOAD));

//   // Load configuration from eeprom or one of 7 slots below if first valid load does not stop_flash_rotate
//   struct SYSCFGH {
//     uint16_t cfg_holder;                     // 000
//     uint16_t cfg_size;                       // 002
//     unsigned long save_flag;                 // 004
//   } _SettingsH;
//   unsigned long save_flag = 0;

//   Settings.flag_system.stop_flash_rotate = 1;// temp measure

//   settings_location = 0;
//   uint32_t flash_location = SETTINGS_LOCATION +1; //next memory location

//   uint16_t cfg_holder = 0;

  
//   for (uint8_t i = 0; i < CFG_ROTATES; i++) {
//     flash_location--;
//     ESP.flashRead(flash_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(SYSCFG));

//     AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_MEMORY D_LOAD " i=%d bootcount=%d version=%X"),i,Settings.bootcount,Settings.version);

//     // if(Settings.version != 0xFFFFFFFF){
//     //   AddLog_P(LOG_LEVEL_TEST,PSTR("i=%d version=%X DOES NOT EQUAL tversion=%X"),i,Settings.version,0xFFFFFFFF);
//     // }else{
//     //   AddLog_P(LOG_LEVEL_TEST,PSTR("ELSE ELSE i=%d version=%X tversion=%X"),i,Settings.version,0xFFFFFFFF);
//     //   //break;// test
//     // }

//     bool valid = false;
//     if((Settings.version > 0x06000000)&&(Settings.version != 0xFFFFFFFF)) {
//       //AddLog_P(LOG_LEVEL_TEST,PSTR("ESP.flashRead %i IF"),i);
//       bool almost_valid = (Settings.cfg_crc == GetSettingsCrc());
//       AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_MEMORY D_LOAD " cfg_crc==GetSettingsCrc()| %d==%d"),Settings.cfg_crc,GetSettingsCrc());
//       // Sometimes CRC on pages below FB, overwritten by OTA, is fine but Settings are still invalid. So check cfg_holder too
//       if (almost_valid && (0 == cfg_holder)) { cfg_holder = Settings.cfg_holder; }  // At FB always active cfg_holder
//       valid = (cfg_holder == Settings.cfg_holder);
//     } else {


//       ESP.flashRead((flash_location -1) * SPI_FLASH_SEC_SIZE, (uint32*)&_SettingsH, sizeof(SYSCFGH));
//       valid = (Settings.cfg_holder == _SettingsH.cfg_holder);
//       if(Settings.cfg_holder == 65535){
//         // catch when memory is all 1's and not 0
//         valid = false;
//       }
//       AddLog_P(LOG_LEVEL_TEST,PSTR("flashRead ELSE %d %d %d"),valid,Settings.cfg_holder,_SettingsH.cfg_holder);
//     }
//     AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_MEMORY D_LOAD " ESP.flashRead valid=%d"),valid);
//     if (valid) {
//       AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_MEMORY D_LOAD " Settings.save_flag > save_flag %d>%d"),Settings.save_flag,save_flag);
//       if (Settings.save_flag > save_flag) {
//         save_flag = Settings.save_flag;
//         settings_location = flash_location;
//         if (Settings.flag_system.stop_flash_rotate && (0 == i)) {  // Stop only if eeprom area should be used and it is valid
//           break;
//         }
//       }
//     }

//     delay(1);
//   }

// /*
// #else  // CFG_RESILIENT
//   // Activated with version 8.4.0.2 - Fails to read any config before version 6.6.0.11
//   settings_location = 0;
//   uint32_t save_flag = 0;
//   uint32_t flash_location = SETTINGS_LOCATION;
//   for (uint32_t i = 0; i < CFG_ROTATES; i++) {              // Read all config pages in search of valid and latest
//     ESP.flashRead(flash_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(Settings));
//     if ((Settings.cfg_crc32 != 0xFFFFFFFF) && (Settings.cfg_crc32 != 0x00000000) && (Settings.cfg_crc32 == GetSettingsCrc32())) {
//       if (Settings.save_flag > save_flag) {                 // Find latest page based on incrementing save_flag
//         save_flag = Settings.save_flag;
//         settings_location = flash_location;
//         if (Settings.flag.stop_flash_rotate && (0 == i)) {  // Stop if only eeprom area should be used and it is valid
//           break;
//         }
//       }
//     }
//     flash_location--;
//     delay(1);
//   }
//   */
  
//   if (settings_location > 0) {
//     ESP.flashRead(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(SYSCFG));
//     AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_MEMORY D_LOAD D_LOADED_FROM_FLASH_AT " %X, " D_COUNT " %lu" D_BOOT_COUNT " %d"), settings_location, Settings.save_flag, Settings.bootcount);
//   }
//   // else{    
//   //   AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_MEMORY D_LOAD " %s>%d"), "settings_location",settings_location);
//   // }

//   #ifndef FIRMWARE_MINIMAL
//   if (!settings_location || (Settings.cfg_holder != (uint16_t)SETTINGS_HOLDER)) {  // Init defaults if cfg_holder differs from user settings in .h
//     //Settings.seriallog_level = LOG_LEVEL_ALL;
//     pCONT_set->Settings.seriallog_level = pCONT_set->seriallog_level_during_boot;
    
//     AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_MEMORY D_LOAD "cfg_holder(%d) != SETTINGS_HOLDER(%d), Erasing settings"),Settings.cfg_holder,SETTINGS_HOLDER);

//     // Clear system defaults
//     SettingsDefault();

//   }
//   settings_crc = GetSettingsCrc();
//   #endif  // FIRMWARE_MINIMAL

//   RtcSettingsLoad();
  
// }


// // uint16_t mSettings::GetSettingsCrc(void)
// // {
// //   uint16_t crc = 0;
// //   uint8_t *bytes = (uint8_t*)&Settings;

// //   for (uint16_t i = 0; i < sizeof(SYSCFG); i++) {
// //     if ((i < 14) || (i > 15)) { crc += bytes[i]*(i+1); }  // Skip crc
// //   }
// //   return crc;
// // }


// void mSettings::SettingsSave(uint8_t rotate)
// { 

  

// // return ;
// // #endif

//   // Serial.print("SettingsSave> ");Serial.println(rotate);
//   /* Save configuration in eeprom or one of 7 slots below
//  *
//  * rotate 0 = Save in next flash slot
//  * rotate 1 = Save only in eeprom flash slot until SetOption12 0 or restart
//  * rotate 2 = Save in eeprom flash slot, erase next flash slots and continue depending on stop_flash_rotate
//  * stop_flash_rotate 0 = Allow flash slot rotation (SetOption12 0)
//  * stop_flash_rotate 1 = Allow only eeprom flash slot use (SetOption12 1)
//  */
// //DEBUG_LINE_HERE;
// // #ifndef FIRMWARE_MINIMAL
//   if ((GetSettingsCrc() != settings_crc) || rotate) {
// //DEBUG_LINE_HERE;
//     if (1 == rotate) {   // Use eeprom flash slot only and disable flash rotate from now on (upgrade)
//       stop_flash_rotate = 1;
//       //AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_MEMORY D_SAVE " stop_flash_rotate"));//(upgrade) Use eeprom flash slot only and disable flash rotate from now on"));
//     }
//     if (2 == rotate) {   // Use eeprom flash slot and erase next flash slots if stop_flash_rotate is off (default)
//       settings_location = SETTINGS_LOCATION +1;
//       //AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_MEMORY D_SAVE " (default) Use eeprom flash slot and erase next flash slots if stop_flash_rotate is off(%d) (default)"),stop_flash_rotate);
//     }
//     if (stop_flash_rotate) {
//       settings_location = SETTINGS_LOCATION;
//     } else {
//       settings_location--;
//       //AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_MEMORY D_SAVE " settings_location=%d"),settings_location);
//       if (settings_location <= (SETTINGS_LOCATION - CFG_ROTATES)) {
//         settings_location = SETTINGS_LOCATION;
//       //AddLog_P(LOG_LEVEL_TEST,PSTR("settings_location <= (SETTINGS_LOCATION - CFG_ROTATES)"));
//       }
//     }
//     Settings.save_flag++;
//     Settings.cfg_size = sizeof(SYSCFG);
//     Settings.cfg_crc = GetSettingsCrc();

// //DEBUG_LINE_HERE;
// // #ifdef USE_EEPROM
// //     if (SPIFFS_END == settings_location) {
// //       uint8_t* flash_buffer;
// //       flash_buffer = new uint8_t[SPI_FLASH_SEC_SIZE];
// //       if (eeprom_data && eeprom_size) {
// //         size_t flash_offset = SPI_FLASH_SEC_SIZE - eeprom_size;
// //         memcpy(flash_buffer + flash_offset, eeprom_data, eeprom_size);  // Write dirty EEPROM data
// //       } else {
// //         ESP.flashRead(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)flash_buffer, SPI_FLASH_SEC_SIZE);   // Read EEPROM area
// //       }
// //       memcpy(flash_buffer, &Settings, sizeof(Settings));
// //       ESP.flashEraseSector(settings_location);
// //       ESP.flashWrite(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)flash_buffer, SPI_FLASH_SEC_SIZE);
// //       delete[] flash_buffer;
// //     } else {
// //       ESP.flashEraseSector(settings_location);
// //       ESP.flashWrite(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(SYSCFG));
// //     }
// // #else

// // ESP.wdtFeed();
// // ESP.wdtDisable();
// //DEBUG_LINE_HERE;
//     // settings_location = SETTINGS_LOCATION;
// //     AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_MEMORY D_SAVE " settings_location <= (SETTINGS_LOCATION - CFG_ROTATES) %lu"),settings_location);

// // AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_MEMORY D_SAVE " ENTERING UNSAFE AREA %d"),settings_location);

// // Serial.flush();
// // delay(1000);

// //DEBUG_LINE_HERE;

// #ifdef ENABLE_FLASH_ERASE_SECTOR_CURRENTLY_BUG
//   if (ESP.flashEraseSector(settings_location)) {
//     ESP.flashWrite(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(Settings));
//   }
// #endif

// // AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_MEMORY D_SAVE " LEAVING UNSAFE AREA %d"),settings_location);
// // ESP.wdtFeed();
// // delay(1000);

//     // ESP.flashEraseSector(settings_location);
// // //DEBUG_LINE_HERE;
// //     ESP.flashWrite(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(SYSCFG));
// //DEBUG_LINE_HERE;
// // #endif  // USE_EEPROM

//     if (!stop_flash_rotate && rotate) {
//       for (uint8_t i = 1; i < CFG_ROTATES; i++) {
// //DEBUG_LINE_HERE;
//         ESP.flashEraseSector(settings_location -i);  // Delete previous configurations by resetting to 0xFF
//         delay(1);
//       }
//     }

//     AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_MEMORY D_SAVE D_LOG_CONFIG D_SAVED_TO_FLASH_AT " %X, " D_COUNT " %d, " D_BYTES " %d"), settings_location, Settings.save_flag, sizeof(SYSCFG));

//     settings_crc = Settings.cfg_crc;
//   }
// // #endif  // FIRMWARE_MINIMAL
// //DEBUG_LINE_HERE;
//   RtcSettingsSave();

// }



// #endif // ENABLE_DEVFEATURE_SETTINGS_V2



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

  bool _serialoutput = (LOG_LEVEL_DEBUG_MORE <= seriallog_level);

    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_ERASE " %d " D_UNIT_SECTORS), _sectorEnd - _sectorStart);
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
        Settings.module = MODULE_NODEMCU_ID;             // Reset module to Sonoff Basic
        Settings.last_module = MODULE_NODEMCU_ID;
      }
      //reset 7 times, then fail into safe boot awaiting OTA
      // HandleFailedBootFailBack();
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_LOG_SOME_SETTINGS_RESET " (%d)"), RtcReboot.fast_reboot_count);
      #endif///   #ifdef ENABLE_LOG_LEVEL_INFO
    }
  }

  memset(Settings.mqtt.topic,0,sizeof(Settings.mqtt.topic));
  memcpy(Settings.mqtt.topic,pCONT_set->Settings.system_name.device,strlen(pCONT_set->Settings.system_name.device));
  
  // Configure hostname 
  memset(my_hostname,0,sizeof(my_hostname));
  sprintf(my_hostname,PSTR("%s"),pCONT_set->Settings.system_name.device);

  // AddLog_P(LOG_LEVEL_INFO, PSTR(D_PROJECT " %s %s " D_VERSION " %s%s-" ARDUINO_ESP8266_RELEASE), pCONT_set->Settings.system_name.device, Settings.system_name.friendly, my_version, my_image);
  

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

void mSettings::SettingsDefault(void)
{
  
  Settings.flag_system.stop_flash_rotate = true;
  stop_flash_rotate = true;

   //Serial.println("SettingsDefault");
  //  AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_CONFIG D_USE_DEFAULTS));
   SystemSettings_DefaultHeader();
  //  Serial.println(DEBUG_INSERT_PAGE_BREAK "SystemSettings_DefaultBody");
   SystemSettings_DefaultBody();

    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_MEMORY D_LOAD " Loading any progmem templates"));
    #endif // ENABLE_LOG_LEVEL_INFO
    pCONT->Tasker_Interface(FUNC_TEMPLATE_MODULE_LOAD); // loading module, only interface modules will have these
    boot_status.module_template_used = true;
    DEBUG_LINE;
    // Clear module defaults
    pCONT->Tasker_Interface(FUNC_SETTINGS_DEFAULT); // replace with below?
    DEBUG_LINE;
    pCONT->Tasker_Interface(FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT);
    
    pCONT_set->SettingsSave(2);
    
    DEBUG_LINE;
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_MEMORY D_LOAD " %s %d %d"), "SettingsDefault",Settings.cfg_holder,SETTINGS_HOLDER);
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

  Settings.flag_network.sleep_normal = true; // USE DYNAMIC sleep
  
  Settings.mqtt_retry = MQTT_RETRY_SECS;
  
  // strlcpy(Settings.state_text[0], MQTT_STATUS_OFF, sizeof(Settings.state_text[0]));
  // strlcpy(Settings.state_text[1], MQTT_STATUS_ON, sizeof(Settings.state_text[1]));
  // strlcpy(Settings.state_text[2], MQTT_CMND_TOGGLE, sizeof(Settings.state_text[2]));
  // strlcpy(Settings.state_text[3], MQTT_CMND_HOLD, sizeof(Settings.state_text[3]));
  
DEBUG_LINE;
  Settings.param[P_BOOT_LOOP_OFFSET] = BOOT_LOOP_OFFSET;
  Settings.param[P_HOLD_TIME] = KEY_HOLD_TIME;  // Default 4 seconds hold time
  Settings.param[P_MAX_POWER_RETRY] = MAX_POWER_RETRY;
  #ifdef USE_NETWORK_MDNS
  Settings.param[P_MDNS_DELAYED_START] = 0;
  #endif // #ifdef USE_NETWORK_MDNS
  Settings.param[P_RGB_REMAP] = 0;//RGB_REMAP_RGBW;
  Settings.param[P_BOOT_LOOP_OFFSET] = BOOT_LOOP_OFFSET;

DEBUG_LINE;
    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR("DefaultBody_System"));
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

  strlcpy(Settings.mqtt.host, MQTT_HOST, sizeof(Settings.mqtt.host));
  Settings.mqtt.port = MQTT_PORT;
  strlcpy(Settings.mqtt.client, MQTT_CLIENT_ID, sizeof(Settings.mqtt.client));
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

  Settings.sensors.ifchanged_secs = 10;
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
  Settings.animation_settings.animation_transition_method = 0;
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
  
  Settings.light_settings.type = 0;//LT_WS2812; //default for now
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
  Settings.sensors.altitude = 0; 
  Settings.sensors.latitude = (int)((double)LATITUDE * 1000000);
  Settings.sensors.longitude = (int)((double)LONGITUDE * 1000000);

  Settings.sensors.flags.decimal_precision = 3;

  // Settings.flag_sensor.temperature_conversion = TEMP_CONVERSION;
  // Settings.flag_sensor.pressure_conversion = PRESSURE_CONVERSION;
  // Settings.flag_sensor.pressure_resolution = PRESSURE_RESOLUTION;
  // Settings.flag_sensor.humidity_resolution = HUMIDITY_RESOLUTION;
  // Settings.flag_sensor.temperature_resolution = TEMP_RESOLUTION;


}


void mSettings::SystemSettings_DefaultBody_Power(){
  // Settings.power = APP_POWER;
  #ifdef SET_POWERONSTATE_AS_ON
    Settings.poweronstate = POWER_ALL_ON; //temporary fix for poweronstate
  #else
    Settings.poweronstate = APP_POWERON_STATE;
  #endif
  // Settings.interlock[0] = 0xFF;         // Legacy support using all relays in one interlock group
}


void mSettings::SystemSettings_DefaultBody_Energy(){

  #ifdef USE_ENERGY
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

  ClearAllDeviceName();

}


void mSettings::SystemSettings_DefaultBody_Rules(){

//  Settings.rule_enabled = 0;
//  Settings.rule_once = 0;
  // char          mems[MAX_RULE_MEMS][10];   // 7CE
  //for (uint8_t i = 1; i < MAX_RULE_SETS; i++) { Settings.rules[i][0] = '\0'; }

}


void mSettings::SystemSettings_DefaultBody_Displays(){

  Settings.display.model = 0;
  Settings.display.mode = 1;
  Settings.display.refresh = 2;
  Settings.display.rows = 2;
  // Settings.display.cols[0] = 16;
  // Settings.display.cols[1] = 8;
  Settings.display.dimmer = 1;
  Settings.display.size = 1;
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
  //AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR("SystemSettings_DefaultBody_Time1"));
  SystemSettings_DefaultBody_Network();
DEBUG_LINE;
  //AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR("SystemSettings_DefaultBody_Network"));
  SystemSettings_DefaultBody_WebServer();
DEBUG_LINE;
  //AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR("SystemSettings_DefaultBody_Time2"));
  SystemSettings_DefaultBody_MQTT();
DEBUG_LINE;
  SystemSettings_DefaultBody_TelePeriods();
DEBUG_LINE;
  SystemSettings_DefaultBody_Time();
DEBUG_LINE;
  //AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR("SystemSettings_DefaultBody_Time"));
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
  //AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR("SystemSettings_DefaultBody_Displays"));
  
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
  Settings.flag_network.mdns_enabled = USE_NETWORK_MDNS;
  mdns_delayed_start = 60;//Settings.param[P_MDNS_DELAYED_START];

  #endif // #ifdef USE_NETWORK_MDNS
  // seriallog_level = Settings.seriallog_level;
  // seriallog_timer = SERIALLOG_TIMER;
  // syslog_level = Settings.syslog_level;
  // Settings.flag_system.stop_flash_rotate = true;
  // stop_flash_rotate = true;//Settings.flag_system.stop_flash_rotate;
  // save_data_counter = Settings.save_data; 
  runtime_value.sleep = Settings.sleep;
//  Settings.flag_system.value_units = 0;
//  Settings.flag_system.stop_flash_rotate = 0;
//  Settings.flag_system.interlock = 0;
  Settings.flag_system.save_state = SAVE_STATE;
//  for (uint8_t i = 1; i < MAX_PULSETIMERS; i++) { Settings.pulse_timer[i] = 0; }
  // Settings.flag_power.emulation = EMULATION;
//  Settings.flag_system.button_restrict = 0;
//  Settings.flag_system.button_swap = 0;
//  Settings.flag_system.button_single = 0;
  Settings.flag_system.mqtt_enabled = true;
//  Settings.flag_system.mqtt_response = 0;
  Settings.flag_system.mqtt_power_retain = MQTT_POWER_RETAIN;
  Settings.flag_system.mqtt_button_retain = MQTT_BUTTON_RETAIN;
  Settings.flag_system.mqtt_switch_retain = MQTT_SWITCH_RETAIN;
  Settings.flag_network.button_switch_force_local = MQTT_BUTTON_SWITCH_FORCE_LOCAL;
  Settings.flag_network.hass_tele_on_power = TELE_ON_POWER;
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
  

  sprintf(my_hostname,"%s",pCONT_set->Settings.system_name.device);
  
}



void mSettings::SettingsResetStd(void)
{
  Settings.tflag[0].hemis = TIME_STD_HEMISPHERE;
  Settings.tflag[0].week = TIME_STD_WEEK;
  Settings.tflag[0].dow = TIME_STD_DAY;
  Settings.tflag[0].month = TIME_STD_MONTH;
  Settings.tflag[0].hour = TIME_STD_HOUR;
  Settings.toffset[0] = TIME_STD_OFFSET;
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
  
//   #ifdef USE_MODULE_CORE_WEBSERVER
//   char scolor[10];
//   for (uint8_t i = 0; i < COL_LAST; i++) {
//     pCONT_web->WebHexCode(i, pCONT_sup->GetTextIndexed_P(scolor, sizeof(scolor), i, kWebColors));
//   }
//   #endif //  #ifdef USE_MODULE_CORE_WEBSERVER
// }

// /********************************************************************************************/


    // template<typename T>
    // int8_t mSettings::Tasker2(uint8_t function, T param1){

    // }
    

// template int8_t mSettings::Tasker<uint8_t, char>(uint8_t function, char param1);


