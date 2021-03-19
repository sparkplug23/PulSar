#include "mSensorsAnalog.h"

#ifdef USE_MODULE_SENSORS_ANALOG

// Make basic class, report adc via mqtt (for tree level)

void mSensorsAnalog::Pre_Init(void){

  // sensors_active = 0;

  // // Using ++ means always the lowest index will be the active sensor, ie, PIR2 can be 0 when PIR1 is not defined
  // if (pCONT_pins->GetPin(GPIO_PIR_1OF2_ID] < 99) {  // not set when 255
  //   pin[sensors_active++] = pCONT_pins->GetPin(GPIO_PIR_1OF2_ID];
  //     AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_PIR "pin[GPIO_PIR1] %d"),pCONT_pins->GetPin(GPIO_PIR_1OF2_ID]);
  // }
  // if (pCONT_pins->GetPin(GPIO_PIR_2OF2_ID] < 99) {  // not set when 255
  //   pin[sensors_active++] = pCONT_pins->GetPin(GPIO_PIR_2OF2_ID];
  //     AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_PIR "pin[GPIO_PIR2] %d"),pCONT_pins->GetPin(GPIO_PIR_2OF2_ID]);
  // }

  // fEnableSensor = false;

  // for(uint8_t sensor_id=0;sensor_id<sensors_active;sensor_id++){
  //   if(pin[sensor_id]>=0){
  //     AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_PIR "Pin Set %d"),pin[sensor_id]);
  //     pinMode(pin[sensor_id],INPUT_PULLUP);
  //     fEnableSensor = true; // true if any are active
  //   }else{
  //     AddLog_P(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin Invalid %d"),pin[sensor_id]);
  //   }
  // }

  pinMode(A0, INPUT);

}


void mSensorsAnalog::Init(){

  
}


void mSensorsAnalog::EveryLoop(){
  

  // Use short timer to automatically clear event
  //if > 1 sec
  //clear struct
}

int8_t mSensorsAnalog::Tasker(uint8_t function){

  // Run even when sensor is disabled (Will set fEnableSensor)
  switch(function){
    case FUNC_PRE_INIT:
      // Pre_Init();
    break;
  }
  
  // if(!fEnableSensor){ 
  //   //Serial.println("!fEnableSensor"); 
  // return 0; }

  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case FUNC_INIT:
      // Init();
    break;
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP:
    // Serial.println("FUNC_LOOP");
      // EveryLoop();
    break;   
    case FUNC_EVERY_SECOND:
      AddLog_P(LOG_LEVEL_TEST, PSTR("mSensorsAnalog"));
    break;  
    /************
     * COMMANDS SECTION * 
    *******************/
    // case FUNC_COMMAND:

    // break;
    // case FUNC_JSON_COMMAND:
    //   //function_result = parse_JSONCommand();
    //   parse_JSONCommand();
    // break;
    case FUNC_TEMPLATE_DEVICE_EXECUTE_LOAD:
      // parsesub_JSONCommands();
    break;  
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MQTT
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
    #endif //USE_MQTT
  }

} // END function




// //relay:1,[1,2],"all",["fountain,bulbs,led"]
// void mSensorsAnalog::parse_JSONCommand(){ //parse_Command() and pass packet (topic/len/payload/len structure)
  
//   // Check if instruction is for me
//   if(mSupport::mSearchCtrIndexOf(data_buffer.topic.ctr,"set/motion")>=0){
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_RELAYS));
//       pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
//   }else{
//     return; // not meant for here
//   }

//   // AddLog_P(LOG_LEVEL_DEBUG_LOWLEVEL, PSTR(D_LOG_RELAYS D_DEBUG_FUNCTION "\"%s\""),"mRelays::parse_JSONCommand()");

//   AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_RELAYS "Command: " "\"%s\""),data_buffer.payload.ctr);

//   StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
//   DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
//   JsonObject obj = doc.as<JsonObject>();

//   //parsesub_JSONCommands(&obj);

// }


// // void mSensorsAnalog::parsesub_JSONCommands(JsonObject& _obj){

// //   //If its null (nothing passed), try global parameter
// //   if(_obj == nullptr){
// //     AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_MQTT "obj == nullptr"));
// //     if(pCONT_set->pObj != nullptr){
// //       AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_MQTT "pCONT_set->pObj != nullptr"));
// //       _obj = pCONT_set->pObj;
// //     }else{
// //       AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_MQTT "No Object"));
// //       return;
// //     }
// //   }

// //   // Create local dereferenced variable
// //   JsonObject obj = (*_obj); 



// // }




// #ifdef USE_MQTT

/*********************************************************************************************************************************************
******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/


uint8_t mSensorsAnalog::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add("analog", analogRead(A0));
  return JsonBuilderI->End();
  
}



// /************ CONSTRUCT JSON BUILDERS *****************************************************************************************************************************/

uint8_t mSensorsAnalog::ConstructJSON_Sensor(uint8_t json_level){
  
  JsonBuilderI->Start();
    JsonBuilderI->Add("analog", analogRead(A0));

    uint8_t percentage = mapvalue(analogRead(A0), 1024,320, 0,100);

    JsonBuilderI->Add("percentage", percentage);
    JsonBuilderI->Level_Start("limits");
      JsonBuilderI->Add("min", 1024);    
      JsonBuilderI->Add("max", 320);
    JsonBuilderI->Level_End();

  return JsonBuilderI->End();

}


/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

void mSensorsAnalog::MQTTHandler_Init(){

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = SEC_IN_HOUR; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSensorsAnalog::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSensorsAnalog::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"


void mSensorsAnalog::MQTTHandler_Connected(){
  MQTTHandler_Set_fSendNow();
}
void mSensorsAnalog::MQTTHandler_Disconnected(){
  // Nothing
}

void mSensorsAnalog::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mSensorsAnalog::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mSensorsAnalog::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t mqtthandler_list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID, 
    MQTT_HANDLER_SENSOR_IFCHANGED_ID
  };
  
  struct handler<mSensorsAnalog>* mqtthandler_list_ptr[] = {
    &mqtthandler_settings_teleperiod,
    &mqtthandler_sensor_ifchanged
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, D_MODULE_SENSORS_ANALOG_ID,
    mqtthandler_list_ptr, mqtthandler_list_ids,
    sizeof(mqtthandler_list_ptr)/sizeof(mqtthandler_list_ptr[0]),
    mqtt_handler_id
  );

}

//////////////////// END OF MQTT /////////////////////////

#endif

