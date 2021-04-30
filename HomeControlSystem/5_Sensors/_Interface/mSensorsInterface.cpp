#include "mSensorsInterface.h" 

/***
 * New joint motion triggered class, all future motion events will also trigger a response from this class (no rules required)
 * */

// All sensors should have generic functions for getting their status
// We should get it from their name, as this would be truly unique and doesnt need any module name or indexing (unless I use that as identifier)
//

/*

struct to return "sensors"


float GetSensorTemperature(module_id, sensor_id)











*/



#ifdef USE_MODULE_SENSORS_INTERFACE 

const char* mSensorsInterface::PM_MODULE_SENSORS_INTERFACE_CTR = D_MODULE_SENSORS_INTERFACE_CTR;
const char* mSensorsInterface::PM_MODULE_SENSORS_INTERFACE_FRIENDLY_CTR = D_MODULE_SENSORS_INTERFACE_FRIENDLY_CTR;

int8_t mSensorsInterface::Tasker(uint8_t function, JsonParserObject obj){
  
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

  // if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP: 
      EveryLoop();
    break;  
    case FUNC_EVERY_SECOND:{
      Serial.println(sizeof(sensors_reading_t));
      // Serial.println(pCONT_msdb18->GetSensorReading());
      // Serial.println(pCONT_dht->GetSensorReading());
      // Serial.println(pCONT_msdb18->test123());
      // Serial.println(pCONT_dht->test123());
      

      for(auto& pmod:pCONT->pModule)
      {
        sensors_reading_t val;
        pmod->GetSensorReading(&val);
        if(val.type_list[0])
        {
          Serial.println(val.type_list[0]);
          AddLog(LOG_LEVEL_TEST, PSTR("%S val.data[0]=%d"),pmod->GetModuleFriendlyName(),(int)val.GetValue(SENSOR2_TYPE_AMBIENT_TEMPERATURE));
        }
      }

      
      //   pModule[switch_index]->Tasker(function, obj);


      // pCONT_msdb18->test1234(&val);
      // Serial.println(val.data[0]);
      // pCONT_dht->test1234(&val);
      // Serial.println(val.data[0]);
  }break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    case FUNC_EVENT_MOTION_STARTED_ID:
      CommandEvent_Motion(1);
    break;
    case FUNC_EVENT_MOTION_ENDED_ID:
      CommandEvent_Motion(0);
    break; 
    /************
     * RULES SECTION * 
    *******************/
    #ifdef USE_MODULE_CORE_RULES
    // case FUNC_EVENT_SET_POWER_ID:
    //   RulesEvent_Set_Power();
    // break;
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


   

void mSensorsInterface::Pre_Init(void)
{

}


void mSensorsInterface::Init(void)
{
  settings.fEnableSensor = true;
}


void mSensorsInterface::EveryLoop()
{

}


void mSensorsInterface::CommandEvent_Motion(uint8_t event_type)
{

  mqtthandler_motion_event_ifchanged.flags.SendNow = true;

}


uint8_t mSensorsInterface::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
  // Ask all modules for their sensor count to get total (or check devicelist via type sensor)
    JsonBuilderI->Add(D_JSON_CHANNELCOUNT, 0);
  return JsonBuilderI->End();

}


uint8_t mSensorsInterface::ConstructJSON_Sensor(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add("motion", 0);



  return JsonBuilderI->End();
    
}

uint8_t mSensorsInterface::ConstructJSON_Motion_Event(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add("motion", 0);

    
  //   for(uint8_t sensor_id=0;sensor_id<settings.sensors_active;sensor_id++){
  //   if(pir_detect[sensor_id].ischanged){ 
      
  //     pir_detect[sensor_id].ischanged = false;
      
  //     JsonBuilderI->Add(D_JSON_LOCATION, pCONT_set->GetDeviceName(EM_MODULE_SENSORS_MOTION_ID, sensor_id, buffer, sizeof(buffer)));
  //     JsonBuilderI->Add(D_JSON_TIME, mTime::ConvertShortTime_HHMMSS(&pir_detect[sensor_id].detected_time, buffer, sizeof(buffer)));
  //     JsonBuilderI->Add(D_JSON_EVENT, pir_detect[sensor_id].isactive ? "detected": "over");
  //triggering sensor (via module id)
  // also module name
  /// also sensor index

/**
 * 
 * Module needs a way to provide known format of type motion, then GetMotion will return a struct with all the important info
 * Should maybe be called GetSensor and GetDriver and internally there are different
 * 
 * 
 * */



  //     //if another is yet to send, then reset the mqtt_handler to fire immeditely again!
  //     //if any mtion flag remains, then set mqtt again
      
  //   }
  // }




  return JsonBuilderI->End();
    
}




/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

  // uint8_t list_ids[] = {
  //   MQTT_HANDLER_SETTINGS_ID, 
  //   MQTT_HANDLER_SENSOR_IFCHANGED_ID, 
  //   MQTT_HANDLER_SENSOR_TELEPERIOD_ID
  // };
  
  // struct handler<mSensorsInterface>* list_ptr[] = {
  //   &mSensorsInterface::mqtthandler_settings_teleperiod,
  //   &mSensorsInterface::mqtthandler_sensor_ifchanged,
  //   &mSensorsInterface::mqtthandler_sensor_teleperiod
  // };

void mSensorsInterface::MQTTHandler_Init(){

  struct handler<mSensorsInterface>* mqtthandler_ptr;

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = false;
  mqtthandler_ptr->flags.SendNow = false;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = false;
  mqtthandler_ptr->flags.SendNow = false;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_Sensor;

  //motion events
  mqtthandler_ptr = &mqtthandler_motion_event_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = false;
  mqtthandler_ptr->flags.SendNow = false;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_MOTION_EVENT_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_Motion_Event;
  
} //end "MQTTHandler_Init"


void mSensorsInterface::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mSensorsInterface::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mSensorsInterface::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, EM_MODULE_SENSORS_INTERFACE_ID, list_ptr, list_ids, sizeof(list_ptr)/sizeof(list_ptr[0]), mqtt_handler_id);

}

////////////////////// END OF MQTT /////////////////////////

#endif
