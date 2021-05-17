#include "mSDCardLogger.h"

#ifdef USE_MODULE_CONTROLLER_SDCARDLOGGER

const char* mSDCardLogger::PM_MODULE_CONTROLLER_SDCARDLOGGER_CTR = D_MODULE_CONTROLLER_SDCARDLOGGER_CTR;
const char* mSDCardLogger::PM_MODULE_CONTROLLER_SDCARDLOGGER_FRIENDLY_CTR = D_MODULE_CONTROLLER_SDCARDLOGGER_FRIENDLY_CTR;


int8_t mSDCardLogger::Tasker(uint8_t function, JsonParserObject obj){
  
  int8_t function_result = 0;
  
  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
    case FUNC_INIT:
      Init();
    break;
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP: 
      EveryLoop();
    break;  
    case FUNC_EVERY_SECOND: 
      EverySecond();
    break;  
    //Rule based on button toggle for sd open and close 
    //have buitin led on when sd card has shown write activity in last 100ms
    /************
     * RULES SECTION * 
    *******************/
    #ifdef USE_MODULE_CORE_RULES
    // case FUNC_EVENT_BUTTON_PRESSED:
    //   RulesEvent_Button_Pressed();
    // break;
    #endif// USE_MODULE_CORE_RULES

    /************
     * TRIGGERS SECTION * 
    *******************/
    case FUNC_EVENT_INPUT_STATE_CHANGED_ID:
      CommandSet_SDCard_OpenClose_Toggle();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
    case FUNC_MQTT_HANDLERS_RESET:
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_TelePeriod();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }
  
  return function_result;

} // END function


void mSDCardLogger::parse_JSONCommand(JsonParserObject obj)
{

}


void mSDCardLogger::Pre_Init(void)
{
  // if (pCONT_pins->PinUsed(GPIO_PZEM0XX_RX_MODBUS_ID) && pCONT_pins->PinUsed(GPIO_PZEM0XX_TX_ID))
  // {
    settings.fEnableSensor = true;
  // }

}


void mSDCardLogger::Init(void)
{


  //For ease, lets hard code the button rule

  
    #if defined(USE_MODULE_SENSORS_BUTTONS)
    
    // // Trigger0
    // p_event = &pCONT_rules->rules[pCONT_rules->rules_active_index].trigger;   
    // p_event->module_id = EM_MODULE_SENSORS_BUTTONS_ID;
    // p_event->function_id = FUNC_EVENT_INPUT_STATE_CHANGED_ID;
    // p_event->device_id = 0; // Button0
    // p_event->value.length = 0;
    // p_event->value.data[p_event->value.length++] = 1;  // Pressed 
    // // Command0
    // p_event = &pCONT_rules->rules[pCONT_rules->rules_active_index].command;   
    // p_event->module_id = EM_MODULE_CONTROLLER_SONOFF_IFAN_ID;
    // p_event->function_id = FUNC_EVENT_SET_SPEED_ID;
    // p_event->device_id = 0; // Button0
    // p_event->value.length = 0;
    // p_event->value.data[p_event->value.length++] = STATE_NUMBER_INCREMENT_ID;  // Increment 
    // pCONT_rules->rules_active_index++;
    // settings.loaded_default_for_moduled = true;

    #endif


}


void mSDCardLogger::EveryLoop()
{


}

//Use gps time to set rtc time?

void mSDCardLogger::EverySecond()
{

  SubTask_UpdateOLED();

}

void mSDCardLogger::SubTask_UpdateOLED()
{

  //Test display messages
  //[1234567890123456]
  //[Op f123456] // Op/Cd for open and closed file, f# where # is GPS time for file name (although also using random in case gps is not working ie millis+gpsUTC)
  //[Val 123456] // Val Err for GPS fix, showing UTC time
  //[ ] //Also show lat/long so I know its working
  //[] packets received on serialRSS in thousands
#ifdef USE_MODULE_DISPLAYS_OLED_SSD1306
  pCONT_set->Settings.display.mode = EM_DISPLAY_MODE_LOG_STATIC_ID;
  char buffer[25];
  snprintf(buffer, sizeof(buffer), "%s %s","Op","fMillis123456");
  pCONT_iDisp->LogBuffer_AddRow(buffer, 0);

  snprintf(buffer, sizeof(buffer), "%s",sdcard_status.isopened?"Open":"CLOSED");
  pCONT_iDisp->LogBuffer_AddRow(buffer, 1);


  
  snprintf(buffer, sizeof(buffer), "%s %s","Op",pCONT_time->RtcTime.hhmmss_ctr);//pCONT_time->GEt DT_UTC;
  pCONT_iDisp->LogBuffer_AddRow(buffer, 3);
  #endif // USE_MODULE_DISPLAYS_OLED_SSD1306

}





uint8_t mSDCardLogger::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_CHANNELCOUNT, 0);
  return JsonBuilderI->End();

}


uint8_t mSDCardLogger::ConstructJSON_Sensor(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_VOLTAGE, 0);
  return JsonBuilderI->End();
    
}




/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

void mSDCardLogger::MQTTHandler_Init(){

  struct handler<mSDCardLogger>* mqtthandler_ptr;

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSDCardLogger::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSDCardLogger::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSDCardLogger::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"


void mSDCardLogger::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mSDCardLogger::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mSDCardLogger::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, EM_MODULE_CONTROLLER_SDCARDLOGGER_ID, list_ptr, list_ids, sizeof(list_ptr)/sizeof(list_ptr[0]), mqtt_handler_id);

}

////////////////////// END OF MQTT /////////////////////////

#endif
