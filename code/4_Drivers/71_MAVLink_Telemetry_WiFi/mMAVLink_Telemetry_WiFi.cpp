#include "mMAVLink_Telemetry_WiFi.h"

#ifdef USE_MODULE__DRIVERS_MAVLINK_TELEMETRY_WIFI

// onds (effective 1124.0 kbit/s)...
// Hash of data verified.

// Leaving...                                               esptool -p COM5 -b 460800 --before default_reset --after hard_reset --chip esp32  write_flash --flash_mode dio --flash_size detect --flash_freq 40m 0x1000 bootloader.bin 0x8000 partition-table.bin 0x10000 db_esp32.bin 0x110000 www.bin>
// esptool.py v4.5.1
// Serial port COM5
// Connecting......

int8_t mMAVLink_Telemetry_WiFi::Tasker(uint8_t function, JsonParserObject obj){

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return TASKER_RESULT__MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP: 
      EveryLoop();
    break;
    case TASK_EVERY_SECOND: 




    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
     /************
     * TELEMETRY SECTION * 
    *******************/
    case TASK_TELEMETRY_HANDLERS_INIT:
      Telemetry_Init();
    break;
    case TASK_TELEMETRY_REFRESH_SEND_ALL:
      tkr_tele->Telemetry_RefreshAll(telemetry_list);
    break;
    case TASK_TELEMETRY_SET_DEFAULT_TRANSMIT_PERIOD:
      tkr_tele->Telemetry_Rate(telemetry_list);
    break;
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_TELEMETRY__SENDER_MQTT:
      //tkr_mqtt->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
    #ifdef USE_MODULE_SERIAL
    case TASK_SERIAL_TELEMETRY:
      tkr_serial->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case TASK_WEB_TELEMETRY:
      tkr_web->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
  }

} // END function


void mMAVLink_Telemetry_WiFi::Pre_Init(void){
  
  module_state.mode = ModuleStatus::Initialising;

}


void mMAVLink_Telemetry_WiFi::Init(void)
{
  // Configured already
  module_state.mode = ModuleStatus::Running;
}


void mMAVLink_Telemetry_WiFi::EveryLoop()
{


}

  
/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

void mMAVLink_Telemetry_WiFi::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
    
}

  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

  
uint8_t mMAVLink_Telemetry_WiFi::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
  
  return JBI->End();

}


uint8_t mMAVLink_Telemetry_WiFi::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[40];

  JBI->Start();

    JBI->Object_Start(D_RFRECEIVED);
    
    JBI->Object_End();
  
  return JBI->End();

}


/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mMAVLink_Telemetry_WiFi::Telemetry_Init()
{

  struct telemetry_handler<mMAVLink_Telemetry_WiFi>* ptr;

  ptr = &telemetry_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 120; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mMAVLink_Telemetry_WiFi::ConstructJSON_Settings;
  telemetry_list.push_back(ptr);

  ptr = &telemetry_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 1; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->flags.json_level = JSON_LEVEL_IFCHANGED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mMAVLink_Telemetry_WiFi::ConstructJSON_State;
  telemetry_list.push_back(ptr);

} 

#endif // USE_MODULE_NETWORK_MQTT


#endif
