#include "mRadiatorFan.h"
#ifdef USE_MODULE_CONTROLLER_RADIATORFAN

const char* mRadiatorFan::PM_MODULE_CONTROLLER_RADIATORFAN_CTR = D_MODULE_CONTROLLER_RADIATORFAN_CTR;
const char* mRadiatorFan::PM_MODULE_CONTROLLER_RADIATORFAN_FRIENDLY_CTR = D_MODULE_CONTROLLER_RADIATORFAN_FRIENDLY_CTR;


// Rename to something generic

// Description: Temperature controlled relays
// Use example: Temp probe that measures radiator temperature and controls fan to assist in heat disspersion

// What this could be merged into: sensor controlled, threshold high and low (hysteresis), output (any driver)
// Actions/Rule

//Relay mode to use temp sensor to control state


void mRadiatorFan::init(void){


}

int8_t mRadiatorFan::Tasker(uint8_t function, JsonParserObject obj){ //Serial.println("mRadiatorFan::Tasker");

  // // Check for changes and report via mqtt
  // if(fan.savedstate != FAN_ACTIVE()){
  //   fan.ischanged = true;
  //   fan.savedstate = FAN_ACTIVE();
  // }
  
  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case FUNC_PRE_INIT:
      // Pre_Init();  
      break;
    case FUNC_INIT:
      init();
      break;
  }

  switch(function){
    // case FUNC_INIT:
    //   init();
    // break;
    case FUNC_LOOP: 

  // Count activity
  // handled via ontime
  // if(FAN_ACTIVE()){
  //   if(abs(millis()-tTick)>=1000){tTick=millis();
  //     fan.secs++;
  //   }
  // }else{
  //   fan.secs = 0;
  // }


  #ifdef USE_MODULE_SENSORS_DS18B20
    if(abs(millis()-tCheckForMaxTemp)>=60000*10){tCheckForMaxTemp=millis();

     pCONT_mry->CommandSet_Relay_Power(RAD_FAN_RELAY_ON);

      // int tempsensorid;
      // if((tempsensorid=pCONT_msdb18->getIDbyName("inside"))>=0){

      //   //check slower, short term fix... need hysterisies
      //   if((pCONT_msdb18->sensor[tempsensorid].reading.val>25)&&(pCONT_msdb18->sensor[tempsensorid].reading.isvalid)){
      //      //FAN_ON(); 
      //      #ifdef USE_MODULE_DRIVERS_RELAY
      //       pCONT->mry->CommandSet_Relay_Power(RAD_FAN_RELAY_ON);
      //      #endif
      //      fan.ischanged = true;
      //   }else{
      //      //FAN_OFF(); 
      //      #ifdef USE_MODULE_DRIVERS_RELAY
      //      pCONT->mry->CommandSet_Relay_Power(RAD_FAN_RELAY_OFF);
      //      #endif
      //      fan.ischanged = true;
      //      fan.secs = 0;
      //   }

      // }
      // Serial.print("tempsensorid"); Serial.println(tempsensorid);

    } // tCheckForMaxTemp
  #endif

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
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_fSendNow();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
    // Other stuff
    case FUNC_STATUS_MESSAGE_APPEND:
      Append_Hardware_Status_Message();
    break;
    // case FUNC_MQTT_SENDER:
    //  // SubTasker_MQTTSender();
    // break;
    // case FUNC_JSON_COMMAND:
    //  // parse_JSONCommand();
    // break;
  }


}




void mRadiatorFan::SubTasker_MQTTSender(){

  // if(abs(millis()-tSaved)>20000){tSaved=millis();
  //   MQQTSendFanStatus();
  // }

}


//#ifdef DEVICE_RADIATORFAN
void mRadiatorFan::MQQTSendFanStatus(void){
    // ConstructJSON_FanStatus();
    // if(data_buffer.payload.len){ // if something to send
    //   pCONT->mqt->ppublish("status/fan", data_buffer.payload.ctr,false);
    // }
}
void mRadiatorFan::ConstructJSON_FanStatus(){

  // StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
  // JsonObject root = doc.to<JsonObject>();

  // D_DATA_BUFFER_CLEAR();

  // int tempsensorid;
  // //root["onoff"] = (int)FAN_ACTIVE(); // instead of boolean
  // root["ontime"] = fan.secs;

  // JsonObject programobj = root.createNestedObject("program");
  // //programobj["mode"] = FanModeCtr();

  // // #ifdef USE_MODULE_SENSORS_DS18B20
  // //   if(fan_mode == AUTO_TEMP){
  // //     JsonObject tempobj = programobj.createNestedObject("temp");
  // //     if((tempsensorid=pCONT_msdb18->getIDbyName("inside"))>=0){
  // //       tempobj["current"] = pCONT_msdb18->sensor[tempsensorid].reading.val;
  // //     }
  // //     tempobj["threshold"] = 25;
  // //   }

  // //   if((tempsensorid=pCONT_msdb18->getIDbyName("inside"))>=0){
  // //     root["inside"] = pCONT_msdb18->sensor[tempsensorid].reading.val;
  // //   }
  // //   if((tempsensorid=pCONT_msdb18->getIDbyName("outside"))>=0){
  // //     root["outside"] = pCONT_msdb18->sensor[tempsensorid].reading.val;
  // //   }
  // // #endif

  // data_buffer.payload.len = measureJson(root)+1;
  // serializeJson(doc,data_buffer.payload.ctr);


}

// void mRadiatorFan::parse_JSONCommand(){

//   // Check if instruction is for me
//   if(mSupport::mSearchCtrIndexOf(data_buffer.topic.ctr,"set/radiatorfan")>=0){
//       AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_RADIATORFAN));
//       pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
//   }else{
//     return; // not meant for here
//   }

//   // StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
//   // DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
//   // JsonObject root = doc.as<JsonObject>();

//   // uint8_t onoff = root["onoff"];

//   // switch(onoff){
//   //   case 1: FAN_ON(); break;
//   //   case 0: default: FAN_OFF(); break;
//   // }

//   //if onoff set, remember change mode, or set via mode

//   fan.ischanged=true;
//   //pCONT->mso->MessagePrint("[MATCHED] fan.ischanged to onoff = ");pCONT->mso->MessagePrintln(onoff);

// }


// void mRadiatorFan::AddToJsonObject_AddHardware(JsonObject root){

//   // JsonObject obj = root.createNestedObject("fan");
//   //   obj["type"] = "FAN-SPST-relay";
//   //   obj["function"] = "on|off";
//   //   obj["unpoweredstate"] = "POWER_OFF-NO";
//   //   obj["pin"] = 0;

// }


void mRadiatorFan::Append_Hardware_Status_Message(){

  // struct STATUSHARDWARE{
  //   char ctr[200];
  //   uint8_t len = 0;
  //   uint8_t importance = 0; //0 low, 1 med, 2 high
  // };

  // if(numpixels>55){numpixels=55;}

//   int8_t fan_state = -1;
//   #ifdef USE_MODULE_DRIVERS_RELAY
//     fan_state = pCONT->mry->CommandGet_Relay_Power(0);
//   #endif

// //  if(fan_state){
//     sprintf(&pCONT_tel->hardwarestatus.ctr[pCONT_tel->hardwarestatus.len],"Fan %s",fan_state?"ON":"OFF");
//     pCONT_tel->hardwarestatus.len = strlen(pCONT_tel->hardwarestatus.ctr);
//     pCONT_tel->hardwarestatus.importance++;
//   //}

}



//enum FAN_MODE{MANUAL=1,AUTO_LOCAL,AUTO_REMOTE};
const char* mRadiatorFan::FanModeCtr(void){
  // return (fan_mode == MANUAL ? "MANUAL\0" :
  //     (fan_mode == AUTO_TEMP ?  "AUTO_TEMP\0" :
  //     (fan_mode == AUTO_REMOTE ?  "AUTO_REMOTE\0" :
  //     "unk\0")));
}


// //enum FAN_MODE{MANUAL=1,AUTO_LOCAL,AUTO_REMOTE};
// const char* mHBridge::FanModeCtr(char* buffer){

//   switch(fan_mode){
//     default:
//     FAN_MODE_MANUAL_ID:
//     FAN_MODE_AUTO_TEMP_ID:
//     FAN_MODE_AUTO_REMOTE_ID:
//   }

//     return (fan_mode == MANUAL ? "MANUAL\0" :
//       (fan_mode == AUTO_TEMP ?  "AUTO_TEMP\0" :
//       (fan_mode == AUTO_REMOTE ?  "AUTO_REMOTE\0" :
//       "unk\0")));

//   return buffer;
// }



uint8_t mRadiatorFan::ConstructJSON_Settings(uint8_t json_method){

  // D_DATA_BUFFER_CLEAR();
  // DynamicJsonDocument doc(250);
  // JsonObject root = doc.to<JsonObject>();

  // root["tbd"] = 0;

  // data_buffer.payload.len = measureJson(root)+1;
  // serializeJson(doc,data_buffer.payload.ctr);
  return 0;
}

uint8_t mRadiatorFan::ConstructJSON_Sensor(uint8_t json_level){

  // D_DATA_BUFFER_CLEAR();

  // uint8_t ischanged=false;

  // DynamicJsonDocument doc(250);
  // JsonObject root = doc.to<JsonObject>();

  // root["tbd"] = 0;

  // data_buffer.payload.len = measureJson(root)+1;
  // serializeJson(doc,data_buffer.payload.ctr);

  return 0;

}





/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

////////////////////// START OF MQTT /////////////////////////

void mRadiatorFan::MQTTHandler_Init(){

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mRadiatorFan::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mRadiatorFan::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mRadiatorFan::ConstructJSON_Sensor;

} //end "MQTTHandler_Init"


void mRadiatorFan::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mRadiatorFan::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"



void mRadiatorFan::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t mqtthandler_list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID, 
    MQTT_HANDLER_SENSOR_IFCHANGED_ID, 
    MQTT_HANDLER_SENSOR_TELEPERIOD_ID
  };
  
  struct handler<mRadiatorFan>* mqtthandler_list_ptr[] = {
    &mqtthandler_settings_teleperiod,
    &mqtthandler_sensor_ifchanged,
    &mqtthandler_sensor_teleperiod
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, EM_MODULE_CONTROLLER_RADIATORFAN_ID,
    mqtthandler_list_ptr, mqtthandler_list_ids,
    sizeof(mqtthandler_list_ptr)/sizeof(mqtthandler_list_ptr[0]),
    mqtt_handler_id
  );

}


////////////////////// END OF MQTT /////////////////////////



#endif
