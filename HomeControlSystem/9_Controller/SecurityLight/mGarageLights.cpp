#include "mGarageLights.h"

#ifdef USE_MODULE_CUSTOM_SECURITY_LIGHT

void mGarageLights::init(void){
  
  // AddLog_P(LOG_LEVEL_DEBUG_LOWLEVEL, PSTR(D_LOG_GARAGE D_DEBUG_FUNCTION "\"%s\""),"mGarageLights::init");

  light_control_driveway.enabled_starttime.Mday = 6; // set to today with __DATE__ later
  light_control_driveway.enabled_starttime.month = mTime::MONTH_AUGUST;
  light_control_driveway.enabled_starttime.year = 2019;
  light_control_driveway.enabled_starttime.hour = 16;
  light_control_driveway.enabled_starttime.minute = 0;
  light_control_driveway.enabled_starttime.second = 1;

  light_control_driveway.enabled_endtime.Mday = 6; // set to today with __DATE__ later
  light_control_driveway.enabled_endtime.month = mTime::MONTH_AUGUST;
  light_control_driveway.enabled_endtime.year = 2019;
  light_control_driveway.enabled_endtime.hour = 23;
  light_control_driveway.enabled_endtime.minute = 59;
  light_control_driveway.enabled_endtime.second = 1;

  light_control_garden.enabled_starttime.Mday = 6; // set to today with __DATE__ later
  light_control_garden.enabled_starttime.month = mTime::MONTH_AUGUST;
  light_control_garden.enabled_starttime.year = 2019;
  light_control_garden.enabled_starttime.hour = 20;
  light_control_garden.enabled_starttime.minute = 0;
  light_control_garden.enabled_starttime.second = 1;

  light_control_garden.enabled_endtime.Mday = 6; // set to today with __DATE__ later
  light_control_garden.enabled_endtime.month = mTime::MONTH_AUGUST;
  light_control_garden.enabled_endtime.year = 2019;
  light_control_garden.enabled_endtime.hour = 20;
  light_control_garden.enabled_endtime.minute = 1;
  light_control_garden.enabled_endtime.second = 1;

  // light_control_driveway.seconds_on = 10;
  // light_control_garden.seconds_on = 10;

}


// Used for timed on or off events
int8_t mGarageLights::Tasker(uint8_t function){

 
  switch(function){
    case FUNC_INIT:
      init();
    break;
    case FUNC_LOOP: 

      // Import motion sense as light motion
      //[0] for driveway
      memcpy(&pir_detect_copy,&pCONT->mms->pir_detect[0],sizeof(pCONT->mms->pir_detect[0]));

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
      //     AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIR "MOTION pir_detect \"%s\""),D_ON);
      //     pCONT->mms->pir_detect.isactive = true;
      //   }else{ 
      //     AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIR "MOTION pir_detect \"%s\""),D_OFF);
      //     pCONT->mms->pir_detect.isactive = false;
      //   }
      //   pCONT->mms->pir_detect.ischanged = true;
      // }

      SubTask_Light();

    break;
    //#ifdef USE_MODULE_NETWORKS_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
    case FUNC_MQTT_HANDLERS_RESET:
      MQTTHandler_Init(); //make a FUNC_MQTT_INIT and group mqtt togather
    break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_TelePeriod(); // Load teleperiod setting into local handlers
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender(); //optional pass parameter
    break;
    //#endif
  }

} // END function

int8_t mGarageLights::Tasker(uint8_t function, JsonObjectConst obj){
  switch(function){
    case FUNC_JSON_COMMAND_OBJECT:
      parse_JSONCommand(obj);
    break;
    case FUNC_JSON_COMMAND_OBJECT_WITH_TOPIC:
      return CheckAndExecute_JSONCommands(obj);
    break;
  }
}

int8_t mGarageLights::CheckAndExecute_JSONCommands(JsonObjectConst obj){

  // Check if instruction is for me
  if(mSupport::mSearchCtrIndexOf(data_buffer2.topic.ctr,"set/garagelights")>=0){
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_MODULE_CUSTOM_SECURITYLIGHT_FRIENDLY_CTR));
      pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
      parsesub_TopicCheck_JSONCommand(obj);
      return FUNCTION_RESULT_HANDLED_ID;
  }else{
    return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
  }

}

void mGarageLights::parsesub_TopicCheck_JSONCommand(JsonObjectConst obj){

  int8_t device_id = -1;
  int8_t isserviced=-1;

  if((device_id = pCONT->mry->GetRelayIDbyName(obj[D_JSON_DEVICE]))>=0){
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_GARAGE D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_DEVICE,device_id);
    Response_mP(S_JSON_COMMAND_NVALUE, D_DEVICE,device_id);
  }else{
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_GARAGE D_PARSING_NOMATCH));
    // Response_mP(S_JSON_COMMAND_SVALUE, D_DEVICE,D_PARSING_NOMATCH);
    return ; // Unknown device, can't execute
  }

  switch(device_id){
    case LIGHT_DRIVEWAY_ID: light_control_ptr = &light_control_driveway; break;
    case LIGHT_GARDEN_ID: light_control_ptr = &light_control_garden; break;
  }

  if(obj.containsKey(D_JSON_TIME_ON)){
    light_control_ptr->seconds_on = obj[D_JSON_TIME_ON];
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_GARAGE D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_TIME_ON,light_control_ptr->seconds_on);
    Response_mP(S_JSON_COMMAND_NVALUE, D_JSON_TIME_ON,light_control_ptr->seconds_on);
    isserviced++;
  }

  if(obj.containsKey(D_JSON_ONOFF)){
    const char* onoff = obj[D_JSON_ONOFF];
    if(strstr(onoff,"ON")){ 
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_GARAGE D_PARSING_MATCHED "\"onoff\"=\"ON\""));
      SetLight(device_id,ON);
      isserviced++;
    }else if(strstr(onoff,"OFF")){
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_GARAGE D_PARSING_MATCHED "\"onoff\"=\"OFF\""));
      SetLight(device_id,OFF);
      isserviced++;
    }else{
      AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_GARAGE D_PARSING_NOMATCH));
    }
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_GARAGE D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_ONOFF,onoff);
    Response_mP(S_JSON_COMMAND_SVALUE,D_JSON_ONOFF,onoff);
    isserviced++;
  }

}

void mGarageLights::SubTask_Light(){

/*
new switch case method/mode

enum AUTOMATIC_LIGHT_MODE{ //should this be in lights instead? probably most if not all of this class can be merged with light, simply having the output as relays
and the lights "automatic" inside the interface

manual, automatic_local_motion, automatic_time_of_day, automatic_with_motion_and_day_dust_by_time


}




*/


  // Automatic
  if(pir_detect_copy.ischanged){ pir_detect_copy.ischanged=false;

    // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_GARAGE D_DEBUG_FUNCTION "\"%s\""),"mGarageLights::SubTask_Light");

    //driveway
    if(pir_detect_copy.isactive && light_control_driveway.fEnableAutomaticLight){
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_GARAGE D_DEBUG_FUNCTION "\"%s\""),"if(pir_detect.isactive");

      #ifndef DISABLE_MOTIONLIGHT_TIMED_LINK
      if(pCONT->mt->CheckBetween_Day_DateTimes(&light_control_driveway.enabled_starttime,&light_control_driveway.enabled_endtime)){
      #endif
        SetLight(LIGHT_DRIVEWAY_ID,TIMED_ON);
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_GARAGE D_DEBUG_FUNCTION "\"%s\""),"SetLight(LIGHT_DRIVEWAY_ID,TIMED_ON);");
      #ifndef DISABLE_MOTIONLIGHT_TIMED_LINK
      }else{
        //SetLight(LIGHT_DRIVEWAY_ID,OFF);
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_GARAGE D_DEBUG_FUNCTION "\"%s\""),"NOT SetLight(LIGHT_DRIVEWAY_ID,TIMED_ON);");
      }
      #endif
    }

    // Clear copy struct
    memset(&pir_detect_copy,0,sizeof(pir_detect_copy));

  }

  

    //garden
    // if(pir_detect.isactive && light_control_garden.fEnableAutomaticLight){
    //   if(pCONT->mt->CheckBetween_Day_DateTimes(&light_control_garden.enabled_starttime,&light_control_garden.enabled_endtime)){
    //     SetLight(LIGHT_GARDEN_ID,TIMED_ON);
    //   }else{
    //     SetLight(LIGHT_GARDEN_ID,OFF);
    //   }
    // }

//  schedule list of ontimes?

  // Seconds_on

  // for now, use openhab remote control for schedules
  

  // MOVE THIS IN TO RELAYS, NOT SECURITY LIGHTS, USE GETTERS AND SETTERS FOR RELAYS
  // no longer named structs, needs to be veriable, timed goes by relay
  if(mSupport::TimeReached(&tSavedSeconds,1000)){
    if(light_control_driveway.seconds_on>0){ 
      light_control_driveway.seconds_on--;
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_GARAGE D_DEBUG_FUNCTION D_JSON_COMMAND_NVALUE),"light_control_driveway.seconds_on",light_control_driveway.seconds_on);
      SetLight(LIGHT_DRIVEWAY_ID,ON);
      //fForceMQTTUpdate = true;
    }else if(light_control_driveway.seconds_on==0){
      light_control_driveway.seconds_on = -1; //stop
      //fForceMQTTUpdate = true;
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_GARAGE D_DEBUG_FUNCTION D_JSON_COMMAND_NVALUE),"light_control_driveway.seconds_on",light_control_driveway.seconds_on);
      SetLight(LIGHT_DRIVEWAY_ID,OFF);
    }

    if(light_control_garden.seconds_on>0){
      light_control_garden.seconds_on--;
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_GARAGE D_DEBUG_FUNCTION D_JSON_COMMAND_NVALUE),"light_control_garden.seconds_on",light_control_garden.seconds_on);
      SetLight(LIGHT_GARDEN_ID,ON);
      // fForceMQTTUpdate = true;
    }else if(light_control_garden.seconds_on==0){
      light_control_garden.seconds_on = -1; //stop
      // fForceMQTTUpdate = true;
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_GARAGE D_DEBUG_FUNCTION D_JSON_COMMAND_NVALUE),"light_control_garden.seconds_on",light_control_garden.seconds_on);
      SetLight(LIGHT_GARDEN_ID,OFF);
    }
  }

}


//ID, LIGHT_DRIVEWAY_ID,LIGHT_GARDEN_ID,LIGHT_ALL_ID
// remove all, call function twive
// pass struct pointer in?
// move timers on to be part of relay struct/class?
void mGarageLights::SetLight(uint8_t light_id, uint8_t state){

  int8_t relay_id;
// THIS IS BAD< PHASE OU TTO WORK WITH OTEHER LIGHTS
//this should all be in interface light
  switch(light_id){
    case LIGHT_DRIVEWAY_ID: relay_id = pCONT->mry->GetRelayIDbyName("Driveway Light"); light_control_driveway.ischanged = true; break;
    case LIGHT_GARDEN_ID:   relay_id = pCONT->mry->GetRelayIDbyName("Garden Light"); light_control_garden.ischanged = true; break;
  }

  if(state==TIMED_ON){
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_GARAGE D_DEBUG_FUNCTION D_JSON_COMMAND_SVALUE),"state","TIMED_ON");     
    state = ON; // For relay call
    switch(light_id){
      case LIGHT_DRIVEWAY_ID: light_control_driveway.seconds_on = 60; break;
      case LIGHT_GARDEN_ID:   light_control_driveway.seconds_on = 60; break;
    }
  }

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_GARAGE D_DEBUG_FUNCTION D_JSON_COMMAND_NVALUE),"relay_id",relay_id);      

  if(relay_id>=0){ //if found
    pCONT->mry->SetRelay(relay_id,state);
  }

}

/*********************************************************************************************
* MQTT ******************************************************************
*************************************************************************************************/


// NEW METHOD -- first senders then on internals
// void mGarageLights::SubTasker_MQTTSender(){

//   if(abs(millis()-tSavedForceUpdate)>(1000*10)){tSavedForceUpdate = millis();Serial.println("tSavedForceUpdate");
//     fForceMQTTUpdate = true;
//   }

//   if(abs(millis()-tSavedTick)>1000){tSavedTick = millis();//Serial.println("MQTTSendLightStatesIfChanged");

//   }

//   MQTTSendLightStatesIfChanged();

//   fForceMQTTUpdate = false;

// }



uint8_t mGarageLights::ConstructJSON_Settings(uint8_t json_method){

  memset(&data_buffer2,0,sizeof(data_buffer2));
  // DynamicJsonDocument doc(250);
  // JsonObject root = doc.to<JsonObject>();

  // root["tbd"] = 0;

  // data_buffer2.payload.len = measureJson(root)+1;
  // serializeJson(doc,data_buffer2.payload.ctr);
  return 0;

}

uint8_t mGarageLights::ConstructJSON_Sensor(uint8_t json_level){

  memset(&data_buffer2,0,sizeof(data_buffer2));

  // uint8_t ischanged=false;

  // DynamicJsonDocument doc(250);
  // JsonObject root = doc.to<JsonObject>();

  // root["tbd"] = 0;

  // data_buffer2.payload.len = measureJson(root)+1;
  // serializeJson(doc,data_buffer2.payload.ctr);

  return 0;

}



// void mGarageLights::MQTTSendLightStatesIfChanged(void){
//   ConstructJSON_LightStates(); //create data
//   if(data_buffer2.payload.len>3){
//     pCONT->mqt->ppublish("status/lights/ifchanged/state",data_buffer2.payload.ctr,false);
//   }
// }
uint8_t mGarageLights::ConstructJSON_LightStates(uint8_t json_level){

  memset(&data_buffer2,0,sizeof(data_buffer2));

  // StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
  // JsonObject root = doc.to<JsonObject>();

  // for(int device_id=0;device_id<RELAYS_CONNECTED;device_id++){

  //   switch(device_id){
  //     case LIGHT_DRIVEWAY_ID: light_control_ptr = &light_control_driveway; break;
  //     case LIGHT_GARDEN_ID: light_control_ptr = &light_control_garden; break;
  //   }

  //   if(light_control_ptr->ischanged){ light_control_ptr->ischanged=false;

  //     char buffer[50];

  //     JsonObject relayobj = root.createNestedObject(pCONT->mry->GetRelayNamebyIDCtr(device_id, buffer, sizeof(buffer)));

  //     relayobj["onoff"] = pCONT->mry->GetRelay(device_id);
  //     relayobj["seconds_on"] = light_control_ptr->seconds_on;

  //     char time_ctr[40]; memset(time_ctr,'\0',sizeof(time_ctr));
  //     sprintf(time_ctr, "%02d:%02d:%02d",pCONT->mry->relay_status[device_id].ontime.hour,pCONT->mry->relay_status[device_id].ontime.minute,pCONT->mry->relay_status[device_id].ontime.second);
  //     relayobj["ontime"] = time_ctr;
  //     char time_ctr2[40]; memset(time_ctr2,'\0',sizeof(time_ctr2));
  //     sprintf(time_ctr2, "%02d:%02d:%02d",pCONT->mry->relay_status[device_id].offtime.hour,pCONT->mry->relay_status[device_id].offtime.minute,pCONT->mry->relay_status[device_id].offtime.second);
  //     relayobj["offtime"] = time_ctr2;

  //     char time_ctr3[40]; memset(time_ctr3,'\0',sizeof(time_ctr3));
  //     sprintf(time_ctr3, "%02d:%02d:%02d",light_control_ptr->enabled_starttime.hour,light_control_ptr->enabled_starttime.minute,light_control_ptr->enabled_starttime.second);
  //     relayobj["enabled_starttime"] = time_ctr3;
  //     char time_ctr4[40]; memset(time_ctr4,'\0',sizeof(time_ctr4));
  //     sprintf(time_ctr4, "%02d:%02d:%02d",light_control_ptr->enabled_endtime.hour,light_control_ptr->enabled_endtime.minute,light_control_ptr->enabled_endtime.second);
  //     relayobj["enabled_endtime"] = time_ctr4;

  //   }
  // }

  // data_buffer2.payload.len = measureJson(root)+1;
  // if(data_buffer2.payload.len>3){
  //   serializeJson(doc,data_buffer2.payload.ctr);
  //   return 1;
  // }

  return 0;

}





/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

////////////////////// START OF MQTT /////////////////////////

void mGarageLights::MQTTHandler_Init(){

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mGarageLights::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mGarageLights::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mGarageLights::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_lightstate_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_LIGHTSTATE_CTR;//postfix_topic_lightstate;
  mqtthandler_ptr->ConstructJSON_function = &mGarageLights::ConstructJSON_LightStates;

  mqtthandler_ptr = &mqtthandler_lightstate_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_LIGHTSTATE_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mGarageLights::ConstructJSON_LightStates;

} //end "MQTTHandler_Init"


void mGarageLights::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_teleperiod.flags.SendNow = true;
  mqtthandler_lightstate_ifchanged.flags.SendNow = true;
  mqtthandler_lightstate_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mGarageLights::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_lightstate_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"



void mGarageLights::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t flag_handle_all = false, handler_found = false;
  if(mqtt_handler_id == MQTT_HANDLER_ALL_ID){ flag_handle_all = true; } //else run only the one asked for

  do{

    switch(mqtt_handler_id){
      case MQTT_HANDLER_SETTINGS_ID:                       handler_found=true; mqtthandler_ptr=&mqtthandler_settings_teleperiod; break;
      case MQTT_HANDLER_SENSOR_IFCHANGED_ID:               handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_ifchanged; break;
      case MQTT_HANDLER_SENSOR_TELEPERIOD_ID:              handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_teleperiod; break;
      case MQTT_HANDLER_MODULE_LIGHTSTATE_IFCHANGED_ID:               handler_found=true; mqtthandler_ptr=&mqtthandler_lightstate_ifchanged; break;
      case MQTT_HANDLER_MODULE_LIGHTSTATE_TELEPERIOD_ID:              handler_found=true; mqtthandler_ptr=&mqtthandler_lightstate_teleperiod; break;
      default: handler_found=false; break; // nothing 
    } // switch

    // Pass handlers into command to test and (ifneeded) execute
    if(handler_found){ pCONT->mqt->MQTTHandler_Command(*this,D_MODULE_CUSTOM_SECURITYLIGHT_ID,mqtthandler_ptr); }

    // stop searching
    if(mqtt_handler_id++>MQTT_HANDLER_MODULE_LENGTH_ID){flag_handle_all = false; return;}

  }while(flag_handle_all);

}


////////////////////// END OF MQTT /////////////////////////


//#endif

#endif
