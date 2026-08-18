#include "mUARTLogger.h"

#ifdef USE_MODULE_CONTROLLER_UARTLOGGER

int8_t mUARTLogger::Tasker(uint8_t function, JsonParserObject obj){
  
  int8_t function_result = 0;
  
  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
  }

  if(!settings.fEnableSensor){ return TASKER_RESULT__MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP: 
      EveryLoop();
    break;  
    case TASK_EVERY_SECOND: 
      EverySecond();
    break;  
    //Rule based on button toggle for sd open and close 
    //have buitin led on when sd card has shown write activity in last 100ms
    /************
     * RULES SECTION * 
    *******************/
    #ifdef USE_MODULE_CORE_RULES
    // case TASK_EVENT_BUTTON_PRESSED:
    //   RulesEvent_Button_Pressed();
    // break;
    #endif// USE_MODULE_CORE_RULES

    /************
     * TRIGGERS SECTION * 
    *******************/
    case TASK_EVENT_INPUT_STATE_CHANGED_ID:
      CommandSet_SDCard_OpenClose_Toggle();
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
  
  return function_result;

} // END function


void mUARTLogger::parse_JSONCommand(JsonParserObject obj)
{

}


void mUARTLogger::Pre_Init(void)
{
  // if (tkr_pins->PinUsed(GPIO_PZEM0XX_RX_MODBUS_ID) && tkr_pins->PinUsed(GPIO_PZEM0XX_TX_ID))
  // {
    settings.fEnableSensor = true;
  // }

}


void mUARTLogger::Init(void)
{


  //For ease, lets hard code the button rule

  
    #if defined(USE_MODULE_SENSORS_BUTTONS)
    
    // // Trigger0
    // p_event = &tkr_rules->rules[tkr_rules->rules_active_index].trigger;   
    // p_event->module_id = EM_MODULE_SENSORS_BUTTONS_ID;
    // p_event->function_id = TASK_EVENT_INPUT_STATE_CHANGED_ID;
    // p_event->device_id = 0; // Button0
    // p_event->value.length = 0;
    // p_event->value.data[p_event->value.length++] = 1;  // Pressed 
    // // Command0
    // p_event = &tkr_rules->rules[tkr_rules->rules_active_index].command;   
    // p_event->module_id = EM_MODULE_CONTROLLER_SONOFF_IFAN_ID;
    // p_event->function_id = TASK_EVENT_SET_SPEED_ID;
    // p_event->device_id = 0; // Button0
    // p_event->value.length = 0;
    // p_event->value.data[p_event->value.length++] = STATE_NUMBER_INCREMENT_ID;  // Increment 
    // tkr_rules->rules_active_index++;
    // settings.loaded_default_for_moduled = true;

    #endif


}


void mUARTLogger::EveryLoop()
{


}

//Use gps time to set rtc time?

void mUARTLogger::EverySecond()
{

  SubTask_UpdateOLED();

}

void mUARTLogger::SubTask_UpdateOLED()
{

  //Test display messages
  //[1234567890123456]
  //[Op f123456] // Op/Cd for open and closed file, f# where # is GPS time for file name (although also using random in case gps is not working ie millis+gpsUTC)
  //[Val 123456] // Val Err for GPS fix, showing UTC time
  //[ ] //Also show lat/long so I know its working
  //[] packets received on serialRSS in thousands
#ifdef USE_MODULE_DISPLAYS_OLED_SSD1306
  tkr_set->Settings.display.mode = EM_DISPLAY_MODE_LOG_STATIC_ID;
  char buffer[25];
  snprintf(buffer, sizeof(buffer), "%s %s","Op","fMillis123456");
  tkr_iDisp->LogBuffer_AddRow(buffer, 0);

  snprintf(buffer, sizeof(buffer), "%s",sdcard_status.isopened?"Open":"CLOSED");
  tkr_iDisp->LogBuffer_AddRow(buffer, 1);


  
  snprintf(buffer, sizeof(buffer), "%s %s","Op",tkr_time->RtcTime.hhmmss_ctr);//tkr_time->GEt DT_UTC;
  tkr_iDisp->LogBuffer_AddRow(buffer, 3);
  #endif // USE_MODULE_DISPLAYS_OLED_SSD1306

}





uint8_t mUARTLogger::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_CHANNELCOUNT, 0);
  return JBI->End();

}


uint8_t mUARTLogger::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_VOLTAGE, 0);
  return JBI->End();
    
}




/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

void mUARTLogger::Telemetry_Init(){

  struct telemetry_handler<mUARTLogger>* ptr;

  ptr = &telemetry_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mUARTLogger::ConstructJSON_Settings;

  ptr = &telemetry_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mUARTLogger::ConstructJSON_Sensor;

  ptr = &telemetry_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mUARTLogger::ConstructJSON_Sensor;
  
} 


void mUARTLogger::MQTTHandler_RefreshAll(){

  telemetry_settings.flags.SendNow = true;
  telemetry_sensor_ifchanged.flags.SendNow = true;
  telemetry_sensor_teleperiod.flags.SendNow = true;

} 


void mUARTLogger::MQTTHandler_Rate(){

  telemetry_settings.tRateSecs = tkr_mqtt->dt.teleperiod_secs;
  telemetry_sensor_teleperiod.tRateSecs = tkr_mqtt->dt.teleperiod_secs;

} //end "MQTTHandler_Rate"


void mUARTLogger::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  tkr_mqtt->MQTTHandler_Command_Array_Group(*this, EM_MODULE_CONTROLLER_SDCARDLOGGER_ID, list_ptr, list_ids, sizeof(list_ptr)/sizeof(list_ptr[0]), mqtt_handler_id);

}

////////////////////// END OF MQTT /////////////////////////

#endif
