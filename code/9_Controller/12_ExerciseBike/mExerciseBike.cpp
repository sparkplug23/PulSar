#include "mExerciseBike.h"

#ifdef USE_MODULE_CONTROLLER_EXERCISE_BIKE

mExerciseBike::mExerciseBike(void){}

void mExerciseBike::init(void){
  
  // AddLog(LOG_LEVEL_DEBUG_LOWLEVEL, PSTR(D_LOG_GARAGE D_DEBUG_FUNCTION "\"%s\""),"mExerciseBike::init");

//   light_control_driveway.enabled_starttime.Mday = 6; // set to today with __DATE__ later
//   light_control_driveway.enabled_starttime.month = mTime::MONTH_AUGUST;
//   light_control_driveway.enabled_starttime.year = 2019;
//   light_control_driveway.enabled_starttime.hour = 16;
//   light_control_driveway.enabled_starttime.minute = 0;
//   light_control_driveway.enabled_starttime.second = 1;

//   light_control_driveway.enabled_endtime.Mday = 6; // set to today with __DATE__ later
//   light_control_driveway.enabled_endtime.month = mTime::MONTH_AUGUST;
//   light_control_driveway.enabled_endtime.year = 2019;
//   light_control_driveway.enabled_endtime.hour = 23;
//   light_control_driveway.enabled_endtime.minute = 59;
//   light_control_driveway.enabled_endtime.second = 1;

//   light_control_garden.enabled_starttime.Mday = 6; // set to today with __DATE__ later
//   light_control_garden.enabled_starttime.month = mTime::MONTH_AUGUST;
//   light_control_garden.enabled_starttime.year = 2019;
//   light_control_garden.enabled_starttime.hour = 20;
//   light_control_garden.enabled_starttime.minute = 0;
//   light_control_garden.enabled_starttime.second = 1;

//   light_control_garden.enabled_endtime.Mday = 6; // set to today with __DATE__ later
//   light_control_garden.enabled_endtime.month = mTime::MONTH_AUGUST;
//   light_control_garden.enabled_endtime.year = 2019;
//   light_control_garden.enabled_endtime.hour = 20;
//   light_control_garden.enabled_endtime.minute = 1;
//   light_control_garden.enabled_endtime.second = 1;

  // light_control_driveway.seconds_on = 10;
  // light_control_garden.seconds_on = 10;

}


// const char HTTP_DEVICE_CONTROL[] PROGMEM = "<td style='width:%d%%'><button onclick='la(\"&o=%d\");'>%s%s</button></td>";  // ?o is related to WebGetArg(request,"o", tmp, sizeof(tmp));
// const char HTTP_DEVICE_CONTROL_BUTTON_VARIABLE_HANDLE[] PROGMEM = "<td style='width:%d%%'><button onclick='la(\"&%s=%d\");'>%s%s</button></td>";  // ?o is related to WebGetArg(request,"o", tmp, sizeof(tmp));
// const char HTTP_DEVICE_STATE[] PROGMEM = "<td style='width:%d{c}%s;font-size:%dpx'>%s</div></td>";  // {c} = %'><div style='text-align:center;font-weight:





// Used for timed on or off events
void mExerciseBike::Tasker(uint8_t function, JsonParserObject obj){

 
  switch(function){
    case FUNC_INIT:
      init();
    break;
    case FUNC_LOOP: 

      // Import motion sense as light motion
      //[0] for driveway
    //   memcpy(&pir_detect_copy,&pCONT->mms->pir_detect[0],sizeof(pCONT->mms->pir_detect[0]));

      //[1] for back garden

      // if(pir_detect_copy.ischanged){
      //   pCONT->mms->pir_detect.ischanged = false; // we are handling it here
      // }

      //Todo, create "Command" instead of loop, that runs motion events etc, then clears eg
      //if motion happened
      //Tasker(FUNC_COMMAND)
      //if serviced, clear motion flag


    //if motion AND activetimes, turn light on
      // if(PIR_DETECTED()!=pCONT->mms->pir_detect.state){
      //   pCONT->mms->pir_detect.state = PIR_DETECTED();
      //   pCONT->mms->pir_detect.tDetectTime = millis();
      //   if(pCONT->mms->pir_detect.state){ 
      //     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIR "MOTION pir_detect \"%s\""),D_ON);
      //     pCONT->mms->pir_detect.isactive = true;
      //   }else{ 
      //     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIR "MOTION pir_detect \"%s\""),D_OFF);
      //     pCONT->mms->pir_detect.isactive = false;
      //   }
      //   pCONT->mms->pir_detect.ischanged = true;
      // }

    //   SubTask_Light();

    break;
    
    
    //#ifdef USE_MODULE_NETWORK_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
      MQTTHandler_Init(); //make a FUNC_MQTT_INIT and group mqtt togather
    break;
    case FUNC_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Set_DefaultPeriodRate(); // Load teleperiod setting into local handlers
    break;
    case FUNC_JSON_COMMAND:
      //parse_JSONCommand();
    break;  
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender(); //optional pass parameter
    break;
    case FUNC_MQTT_CONNECTED:
      //mqttConnected();
    break;
    case FUNC_MQTT_DISCONNECTED:
      //mqttDisconnected();
    break;
    //#endif

    
    case FUNC_WEB_ROOT_SEND_STATUS:{

      // /****
      //  *  Show LED power 
      //  * ****/
      // uint8_t onoff = pCONT->mry->CommandGet_Relay_Power(0);
      // uint8_t fsize = 32;
      // char onoff_ctr[50];
      // if(onoff){
      //   sprintf(onoff_ctr,"%s Light ON",pCONT->mry->GetRelayNamebyIDCtr(0));
      // }else{
      //   sprintf(onoff_ctr,"%s Light OFF",pCONT->mry->GetRelayNamebyIDCtr(0));
      // }

      // pCONT_web->WSBufferAppend_P(PSTR("{t}<tr>"));
      // pCONT_web->WSBufferAppend_P(HTTP_DEVICE_STATE, 
      //   100 / 1, //FULL WIDTH 1 item for length
      //   onoff ? "bold" : "normal", 
      //   fsize, 
      //   onoff_ctr
      // );
      // pCONT_web->WSBufferAppend_P(PSTR("</tr></table>"));


    }
    break;
  }

} // END function


void mExerciseBike::SubTask_Light(){

//   // Automatic
//   if(pir_detect_copy.ischanged){ pir_detect_copy.ischanged=false;

//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_GARAGE D_DEBUG_FUNCTION "\"%s\""),"mExerciseBike::SubTask_Light");

//     //driveway
//     if(pir_detect_copy.isactive && light_control_driveway.fEnableAutomaticLight){
//       AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_GARAGE D_DEBUG_FUNCTION "\"%s\""),"if(pir_detect.isactive");
//       if(pCONT_time->CheckBetween_Day_DateTimes(&light_control_driveway.enabled_starttime,&light_control_driveway.enabled_endtime)){
//         SetLight(LIGHT_DRIVEWAY_ID,TIMED_ON);
//         AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_GARAGE D_DEBUG_FUNCTION "\"%s\""),"SetLight(LIGHT_DRIVEWAY_ID,TIMED_ON);");
//       }else{
//         //SetLight(LIGHT_DRIVEWAY_ID,OFF);
//         AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_GARAGE D_DEBUG_FUNCTION "\"%s\""),"NOT SetLight(LIGHT_DRIVEWAY_ID,TIMED_ON);");
//       }
//     }

//     // Clear copy struct
//     memset(&pir_detect_copy,0,sizeof(pir_detect_copy));

//   }

  

    //garden
    // if(pir_detect.isactive && light_control_garden.fEnableAutomaticLight){
    //   if(pCONT_time->CheckBetween_Day_DateTimes(&light_control_garden.enabled_starttime,&light_control_garden.enabled_endtime)){
    //     SetLight(LIGHT_GARDEN_ID,TIMED_ON);
    //   }else{
    //     SetLight(LIGHT_GARDEN_ID,OFF);
    //   }
    // }

//  schedule list of ontimes?

  // Seconds_on
  
//   if(mTime::TimeReached(&tSavedSeconds,1000)){
//     if(light_control_driveway.seconds_on>0){ 
//       light_control_driveway.seconds_on--;
//       AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_GARAGE D_DEBUG_FUNCTION D_JSON_COMMAND_NVALUE),"light_control_driveway.seconds_on",light_control_driveway.seconds_on);
//       SetLight(LIGHT_DRIVEWAY_ID,ON);
//       //fForceMQTTUpdate = true;
//     }else if(light_control_driveway.seconds_on==0){
//       light_control_driveway.seconds_on = -1; //stop
//       //fForceMQTTUpdate = true;
//       AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_GARAGE D_DEBUG_FUNCTION D_JSON_COMMAND_NVALUE),"light_control_driveway.seconds_on",light_control_driveway.seconds_on);
//       SetLight(LIGHT_DRIVEWAY_ID,OFF);
//     }

//     // if(light_control_garden.seconds_on>0){
//     //   light_control_garden.seconds_on--;
//     //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_GARAGE D_DEBUG_FUNCTION D_JSON_COMMAND_NVALUE),"light_control_garden.seconds_on",light_control_garden.seconds_on);
//     //   SetLight(LIGHT_GARDEN_ID,ON);
//     //   fForceMQTTUpdate = true;
//     // }else if(light_control_garden.seconds_on==0){
//     //   light_control_garden.seconds_on = -1; //stop
//     //   fForceMQTTUpdate = true;
//     //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_GARAGE D_DEBUG_FUNCTION D_JSON_COMMAND_NVALUE),"light_control_garden.seconds_on",light_control_garden.seconds_on);
//     //   SetLight(LIGHT_GARDEN_ID,OFF);
//     // }
//   }

}


//ID, LIGHT_DRIVEWAY_ID,LIGHT_GARDEN_ID,LIGHT_ALL_ID
// remove all, call function twive
// pass struct pointer in?
// move timers on to be part of relay struct/class?
void mExerciseBike::SetLight(uint8_t light_id, uint8_t state){

//   int8_t relay_id;

//   switch(light_id){
//     case LIGHT_DRIVEWAY_ID: relay_id = pCONT->mry->GetRelayIDbyName("driveway"); light_control_driveway.ischanged = true; break;
//     case LIGHT_GARDEN_ID:   relay_id = pCONT->mry->GetRelayIDbyName("garden"); light_control_garden.ischanged = true; break;
//   }

//   if(state==TIMED_ON){
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_GARAGE D_DEBUG_FUNCTION D_JSON_COMMAND_SVALUE),"state","TIMED_ON");     
//     state = ON; // For relay call
//     switch(light_id){
//       case LIGHT_DRIVEWAY_ID: light_control_driveway.seconds_on = 60; break;
//       case LIGHT_GARDEN_ID:   light_control_driveway.seconds_on = 60; break;
//     }
//   }

//   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_GARAGE D_DEBUG_FUNCTION D_JSON_COMMAND_NVALUE),"relay_id",relay_id);      

//   if(relay_id>=0){ //if found
//     pCONT->mry->CommandSet_Relay_Power(relay_id,state);
//   }

}

/*********************************************************************************************
* MQTT ******************************************************************
*************************************************************************************************/


// manual light controls
// set seconds/minutes on
// set time to stay on from
int8_t mExerciseBike::parse_JSONCommand(){ //parse_Command() and pass packet (topic/len/payload/len structure)
  
  // Check if instruction is for me
  if(mSupport::mSearchCtrIndexOf(data_buffer.topic.ctr,"set/garagelights")>=0){
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND "/garagelights"));
      pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
  }else{
    return 0; // not meant for here
  }


  u
  int8_t device_id;

  // AddLog(LOG_LEVEL_DEBUG_LOWLEVEL, PSTR(D_LOG_GARAGE D_DEBUG_FUNCTION "\"%s\""),"mExerciseBike::parse_JSONCommand()");

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_GARAGE "Command: " "\"%s\""),data_buffer.payload.ctr);

  StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
  DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
  if(error){
    AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_GARAGE D_JSON_DESERIALIZATION_ERROR));
    Response_mP(S_JSON_COMMAND_SVALUE, D_ERROR,D_JSON_DESERIALIZATION_ERROR);
    return 0;
  }
  JsonObject obj = doc.as<JsonObject>();
  
//   if((device_id = pCONT->mry->GetRelayIDbyName(obj[D_JSON_DEVICE]))>=0){
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_GARAGE D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_DEVICE,device_id);
//     Response_mP(S_JSON_COMMAND_NVALUE, D_DEVICE,device_id);
//   }else{
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_GARAGE D_PARSING_NOMATCH));
//     Response_mP(S_JSON_COMMAND_SVALUE, D_DEVICE,D_PARSING_NOMATCH);
//     return 0; // Unknown device, can't execute
//   }

//   switch(device_id){
//     case LIGHT_DRIVEWAY_ID: light_control_ptr = &light_control_driveway; break;
//     case LIGHT_GARDEN_ID: light_control_ptr = &light_control_garden; break;
//   }

//   if(obj.containsKey(D_JSON_TIME_ON)){
//     light_control_ptr->seconds_on = obj[D_JSON_TIME_ON];
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_GARAGE D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_TIME_ON,light_control_ptr->seconds_on);
//     Response_mP(S_JSON_COMMAND_NVALUE, D_JSON_TIME_ON,light_control_ptr->seconds_on);
//     isserviced++;
//   }

//   if(obj.containsKey(D_JSON_ONOFF)){
//     const char* onoff = obj[D_JSON_ONOFF];
//     if(strstr(onoff,"ON")){ 
//       AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_GARAGE D_PARSING_MATCHED "\"onoff\"=\"ON\""));
//       SetLight(device_id,ON);
//       isserviced++;
//     }else if(strstr(onoff,"OFF")){
//       AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_GARAGE D_PARSING_MATCHED "\"onoff\"=\"OFF\""));
//       SetLight(device_id,OFF);
//       isserviced++;
//     }else{
//       AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_GARAGE D_PARSING_NOMATCH));
//     }
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_GARAGE D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_ONOFF,onoff);
//     Response_mP(S_JSON_COMMAND_SVALUE,D_JSON_ONOFF,onoff);
//     isserviced++;
//   }

}



// NEW METHOD -- first senders then on internals
// void mExerciseBike::SubTasker_MQTTSender(){

//   if(abs(millis()-tSavedForceUpdate)>(1000*10)){tSavedForceUpdate = millis();Serial.println("tSavedForceUpdate");
//     fForceMQTTUpdate = true;
//   }

//   if(abs(millis()-tSavedTick)>1000){tSavedTick = millis();//Serial.println("MQTTSendLightStatesIfChanged");

//   }

//   MQTTSendLightStatesIfChanged();

//   fForceMQTTUpdate = false;

// }



uint8_t mExerciseBike::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  D_DATA_BUFFER_CLEAR();
  DynamicJsonDocument doc(250);
  JsonObject root = doc.to<JsonObject>();

  root["tbd"] = 0;

  data_buffer.payload.len = measureJson(root)+1;
  serializeJson(doc,data_buffer.payload.ctr);
return 1;
}

uint8_t mExerciseBike::ConstructJSON_Sensor(uint8_t json_level){

  D_DATA_BUFFER_CLEAR();

  uint8_t ischanged=false;

  DynamicJsonDocument doc(250);
  JsonObject root = doc.to<JsonObject>();

  root["tbd"] = 0;

  data_buffer.payload.len = measureJson(root)+1;
  serializeJson(doc,data_buffer.payload.ctr);

  return 1;

}



// void mExerciseBike::MQTTSendLightStatesIfChanged(void){
//   ConstructJSON_LightStates(); //create data
//   if(data_buffer.payload.len>3){
//     pCONT->mqt->ppublish("status/lights/ifchanged/state",data_buffer.payload.ctr,false);
//   }
// }
uint8_t mExerciseBike::ConstructJSON_LightStates(uint8_t json_level){

  D_DATA_BUFFER_CLEAR();

  StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
  JsonObject root = doc.to<JsonObject>();

return 0;

}





/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

////////////////////// START OF MQTT /////////////////////////

void mExerciseBike::MQTTHandler_Init(){

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mExerciseBike::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mExerciseBike::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mExerciseBike::ConstructJSON_Sensor;

  ptr = &mqtthandler_lightstate_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = postfix_topic_lightstate;
  ptr->ConstructJSON_function = &mExerciseBike::ConstructJSON_LightStates;

  ptr = &mqtthandler_lightstate_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = postfix_topic_lightstate;
  ptr->ConstructJSON_function = &mExerciseBike::ConstructJSON_LightStates;

} //end "MQTTHandler_Init"


void mExerciseBike::MQTTHandler_Set_RefreshAll(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_teleperiod.flags.SendNow = true;
  mqtthandler_lightstate_ifchanged.flags.SendNow = true;
  mqtthandler_lightstate_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mExerciseBike::MQTTHandler_Set_DefaultPeriodRate(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_lightstate_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_DefaultPeriodRate"



void mExerciseBike::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  
  uint8_t mqtthandler_list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID, 
    MQTT_HANDLER_SENSOR_IFCHANGED_ID, 
    MQTT_HANDLER_SENSOR_TELEPERIOD_ID
  };
  
  struct handler<mSensorsDHT>* mqtthandler_list_ptr[] = {
    &mqtthandler_settings_teleperiod,
    &mqtthandler_sensor_ifchanged,
    &mqtthandler_sensor_teleperiod
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, D_MODULE_SENSORS_DHT_ID,
    mqtthandler_list_ptr, mqtthandler_list_ids,
    sizeof(mqtthandler_list_ptr)/sizeof(mqtthandler_list_ptr[0]),
    mqtt_handler_id
  );
  if(mqtt_handler_id == MQTT_HANDLER_ALL_ID){ flag_handle_all = true; } //else run only the one asked for

  do{

    switch(mqtt_handler_id){
      case MQTT_HANDLER_SETTINGS_ID:                       handler_found=true; ptr=&mqtthandler_settings_teleperiod; break;
      case MQTT_HANDLER_SENSOR_IFCHANGED_ID:               handler_found=true; ptr=&mqtthandler_sensor_ifchanged; break;
      case MQTT_HANDLER_SENSOR_TELEPERIOD_ID:              handler_found=true; ptr=&mqtthandler_sensor_teleperiod; break;
      case MQTT_HANDLER_MODULE_LIGHTSTATE_IFCHANGED_ID:               handler_found=true; ptr=&mqtthandler_lightstate_ifchanged; break;
      case MQTT_HANDLER_MODULE_LIGHTSTATE_TELEPERIOD_ID:              handler_found=true; ptr=&mqtthandler_lightstate_teleperiod; break;
      default: handler_found=false; break; // nothing 
    } // switch

    // Pass handlers into command to test and (ifneeded) execute
    if(handler_found){ pCONT->mqt->MQTTHandler_Command(*this,D_MODULE_CONTROLLER_EXERCISEBIKE_ID,ptr); }

    // stop searching
    if(mqtt_handler_id++>MQTT_HANDLER_MODULE_LENGTH_ID){flag_handle_all = false; return;}

  }while(flag_handle_all);

}


////////////////////// END OF MQTT /////////////////////////


//#endif

#endif
