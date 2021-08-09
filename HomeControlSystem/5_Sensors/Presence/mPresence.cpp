#include "mPresence.h" 

/***
 * New joint motion triggered class, all future motion events will also trigger a response from this class (no rules required)
 * */

#ifdef USE_MODULE_SENSORS_PRESENCE 

const char* mPresence::PM_MODULE_SENSORS_PRESENCE_CTR = D_MODULE_SENSORS_PRESENCE_CTR;
const char* mPresence::PM_MODULE_SENSORS_PRESENCE_FRIENDLY_CTR = D_MODULE_SENSORS_PRESENCE_FRIENDLY_CTR;

int8_t mPresence::Tasker(uint8_t function, JsonParserObject obj){
  
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
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    // case FUNC_EVENT_MOTION_STARTED_ID:
    // case FUNC_EVENT_MOTION_ENDED_ID:
    //   // CommandSet_Relay_Power(STATE_NUMBER_ON_ID);

    //   mqtthandler_sensor_ifchanged.flags.SendNow = true;

    // break; 
    // case FUNC_
    /************
     * RULES SECTION * 
    *******************/
    #ifdef USE_MODULE_CORE_RULES
    case FUNC_EVENT_PRESENCE_STARTED_ID:
    case FUNC_EVENT_PRESENCE_ENDED_ID:
      RulesEvent_Presence_Change();
    break;
    #endif// USE_MODULE_CORE_RULES
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


void mPresence::Pre_Init(void)
{

}


void mPresence::Init(void)
{
  settings.fEnableSensor = true;

  settings.sensors_active = MAXIMUM_SENSORS;

}


void mPresence::EveryLoop()
{

}



/**
 * @brief Presence Events, will replace all motion type events.
 * The motion (PIR), ultrasonic change to trigger presence, will be controlled via the rule engine
 * Thus, any state change (or later mqtt topic input or url etc) can be treated internally as a presence change, no longer a pin state
 * 
 * @note Information about trigger, will be contained within the stored event
 * @param DeviceName gives the index to the stored location index from sensor list
 * @param State gives the ON/Started (1) or OFF/Ended (0)
 * */
void mPresence::RulesEvent_Presence_Change(){


  // for(
    uint8_t sensor_id = pCONT_rules->rules[pCONT_rules->rules_active_index].command.device_id;
    // sensor_id<settings.sensors_active;sensor_id++)
  // {

  //   if(PIR_Detected(sensor_id)!=pir_detect[sensor_id].state)
  //   {
      pir_detect[sensor_id].state = pCONT_rules->rules[pCONT_rules->rules_active_index].command.value.data[0];//PIR_Detected(sensor_id);

      if(pir_detect[sensor_id].state)
      {
        pir_detect[sensor_id].tDetectTime = millis(); 
        pir_detect[sensor_id].detected_time = pCONT_time->GetTimeShortNowU32();
        pir_detect[sensor_id].isactive = true;

        #ifdef ENABLE_LOG_LEVEL_DEBUG
        AddLog(LOG_LEVEL_DEBUG,PSTR("pir_detect[sensor_id].state=%d"),pir_detect[sensor_id].state);
        #endif
        
        // #ifdef USE_MODULE_CORE_RULES
        // pCONT_rules->New_Event(EM_MODULE_SENSORS_MOTION_ID, sensor_id);
        // #endif
        // pCONT->Tasker_Interface(FUNC_EVENT_MOTION_STARTED_ID);

      }
      else
      {
        pir_detect[sensor_id].tEndedTime = millis();
        pir_detect[sensor_id].isactive = false;

        // #ifdef USE_MODULE_CORE_RULES
        // pCONT_rules->New_Event(EM_MODULE_SENSORS_MOTION_ID, sensor_id);
        // #endif
        // pCONT->Tasker_Interface(FUNC_EVENT_MOTION_ENDED_ID);

      }

      pir_detect[sensor_id].ischanged = true;
      mqtthandler_sensor_ifchanged.flags.SendNow = true;
  //   }
  // }

  AddLog(LOG_LEVEL_TEST, PSTR(DEBUG_INSERT_PAGE_BREAK "Presence Event"));

}



uint8_t mPresence::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_CHANNELCOUNT, 0);
  return JsonBuilderI->End();

}


uint8_t mPresence::ConstructJSON_Sensor(uint8_t json_method){

  char buffer[80];

  JsonBuilderI->Start();
    // JsonBuilderI->Add("motion", 0);

    for(
      uint8_t sensor_id = 0;//pCONT_rules->rules[pCONT_rules->rules_active_index].command.device_id;
      // uint8_t pir_detect_isactive  = pCONT_rules->rules[pCONT_rules->rules_active_index].command.value.data[0];
      sensor_id<settings.sensors_active;sensor_id++)
    {

    if(pir_detect[sensor_id].ischanged){ 
      
      pir_detect[sensor_id].ischanged = false;
      
      //Phase out
      JsonBuilderI->Add(D_JSON_LOCATION, DLI->GetDeviceNameWithEnumNumber(EM_MODULE_SENSORS_PRESENCE_ID, sensor_id, buffer, sizeof(buffer)));
      JsonBuilderI->Add(D_JSON_TIME, mTime::ConvertShortTime_HHMMSS(&pir_detect[sensor_id].detected_time, buffer, sizeof(buffer)));
      JsonBuilderI->Add(D_JSON_EVENT, pir_detect[sensor_id].isactive ? "detected": "over");

  //     JBI->Level_Start(DLI->GetDeviceNameWithEnumNumber(EM_MODULE_SENSORS_MOTION_ID, sensor_id, buffer, sizeof(buffer)));
  //       JBI->Add(D_JSON_TIME, mTime::ConvertShortTime_HHMMSS(&pir_detect[sensor_id].detected_time, buffer, sizeof(buffer)));
  //       JBI->Add(D_JSON_EVENT, pir_detect[sensor_id].isactive ? "detected": "over");
  //     JBI->Level_End();

  //     //if another is yet to send, then reset the mqtt_handler to fire immeditely again!
  //     //if any mtion flag remains, then set mqtt again
      
    }
  }


  return JsonBuilderI->End();
    
}




/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

void mPresence::MQTTHandler_Init(){

  struct handler<mPresence>* mqtthandler_ptr;

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mPresence::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = false;
  mqtthandler_ptr->flags.SendNow = false;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mPresence::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = false;
  mqtthandler_ptr->flags.SendNow = false;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mPresence::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"


void mPresence::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mPresence::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mPresence::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID, 
    MQTT_HANDLER_SENSOR_IFCHANGED_ID, 
    MQTT_HANDLER_SENSOR_TELEPERIOD_ID
  };
  
  struct handler<mPresence>* list_ptr[] = {
    &mqtthandler_settings_teleperiod,
    &mqtthandler_sensor_ifchanged,
    &mqtthandler_sensor_teleperiod
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, EM_MODULE_SENSORS_PRESENCE_ID, list_ptr, list_ids, sizeof(list_ptr)/sizeof(list_ptr[0]), mqtt_handler_id);

}

////////////////////// END OF MQTT /////////////////////////

#endif
