#include "mExampleClass.h"

#ifdef USE_MODULE_SENSORS_EXAMPLE

int8_t mExampleClass::Tasker(uint8_t function, JsonParserObject obj){
  
  int8_t function_result = 0;
  
  // some functions must run regardless
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
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_TELEMETRY_HANDLERS_INIT:
      Telemetry_Init();
    break;
    case TASK_TELEMETRY_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Rate();
    break;
    case TASK_TELEMETRY__SENDER_MQTT:
      MQTTHandler_Sender();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }
  
  return function_result;

} // END function


void mExampleClass::parse_JSONCommand(JsonParserObject obj)
{

}



void mExampleClass::Pre_Init(void)
{
  if (tkr_pins->PinUsed(GPIO_PZEM0XX_RX_MODBUS_ID) && tkr_pins->PinUsed(GPIO_PZEM0XX_TX_ID))
  {
    settings.fEnableSensor = true;
  }

}


void mExampleClass::Init(void)
{


}


void mExampleClass::EveryLoop()
{


}





uint8_t mExampleClass::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_CHANNELCOUNT, 0);
  return JBI->End();

}


uint8_t mExampleClass::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_VOLTAGE, 0);
  return JBI->End();
    
}




/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

void mExampleClass::Telemetry_Init(){

  struct telemetry_handler<mExampleClass>* ptr;

  ptr = &telemetry_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mExampleClass::ConstructJSON_Settings;

  ptr = &telemetry_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mExampleClass::ConstructJSON_Sensor;

  ptr = &telemetry_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mExampleClass::ConstructJSON_Sensor;
  
} 


void mExampleClass::MQTTHandler_RefreshAll(){

  telemetry_settings.flags.SendNow = true;
  telemetry_sensor_ifchanged.flags.SendNow = true;
  telemetry_sensor_teleperiod.flags.SendNow = true;

} 


void mExampleClass::MQTTHandler_Rate(){

  telemetry_settings.tRateSecs = tkr_mqtt->dt.teleperiod_secs;
  telemetry_sensor_teleperiod.tRateSecs = tkr_mqtt->dt.teleperiod_secs;

} //end "MQTTHandler_Rate"


void mExampleClass::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  tkr_mqtt->MQTTHandler_Command_Array_Group(*this, 
    EM_MODULE_SENSORS_EXAMPLE_ID, list_ptr, list_ids, sizeof(list_ptr)/sizeof(list_ptr[0]), mqtt_handler_id
  );

}

////////////////////// END OF MQTT /////////////////////////

#endif
