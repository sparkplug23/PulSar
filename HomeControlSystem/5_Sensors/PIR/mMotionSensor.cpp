#include "mMotionSensor.h"

#ifdef USE_MODULE_SENSORS_MOTION

void mMotionSensor::Pre_Init(void){

  sensors_active = 0;

  fEnableSensor = false;

  uint16_t pin_id = 0;
  for(uint8_t ii=0;ii<(GPIO_PIR_3_INV_ID-GPIO_PIR_1_ID+1);ii++){
    
    pin_id = GPIO_PIR_1_ID+ii;
    // Make sure to not exceed limits
    if(sensors_active>MAXIMUM_SENSORS){ break; }

    if(pCONT_pins->PinUsed(pin_id)){
      // Save GPIO
      pin[sensors_active] = pCONT_pins->GetPin(pin_id);
      // config
      switch (pin_id){
        case GPIO_PIR_1_INV_ID:
        case GPIO_PIR_2_INV_ID:
        case GPIO_PIR_3_INV_ID:
          pin_isinverted[sensors_active] = true;
          pinMode(pin[sensors_active], INPUT_PULLUP);
        break;
        case GPIO_PIR_1_NP_ID:
        case GPIO_PIR_2_NP_ID:
        case GPIO_PIR_3_NP_ID:
          pin_isinverted[sensors_active] = false;
          pinMode(pin[sensors_active], INPUT);   
        break;   
        default:
          pin_isinverted[sensors_active] = false;
          pinMode(pin[sensors_active], INPUT);
        break;
      }
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_PIR "pin[%d] %d"), sensors_active, pin[sensors_active]);
      sensors_active++;
      fEnableSensor = true;
    }
    
  }

}


void mMotionSensor::Init(){

  memset(&pir_detect[0].detected_rtc_ctr,0,sizeof(pir_detect[0].detected_rtc_ctr));
  memset(&pir_detect[1].detected_rtc_ctr,0,sizeof(pir_detect[1].detected_rtc_ctr));
  memset(&pir_detect[2].detected_rtc_ctr,0,sizeof(pir_detect[1].detected_rtc_ctr));
  
  for(uint8_t sensor_id=0;sensor_id<sensors_active;sensor_id++){
    sprintf(pir_detect[sensor_id].detected_rtc_ctr,"--:--:--\0");
    pir_detect[sensor_id].state = PIR_Detected(sensor_id);
  }

}

uint8_t mMotionSensor::PIR_Detected(uint8_t sensor_id){
  return pin_isinverted[sensor_id] ? digitalRead(pin[sensor_id]) : !digitalRead(pin[sensor_id]);
}

const char* mMotionSensor::PIR_Detected_Ctr(uint8_t sensor_id){
  return PIR_Detected(sensor_id) ? "MOTION" : "NO_MOTION"; //F PSTR failed
}

void mMotionSensor::WebPage_Root_AddHandlers(){
  // Not needed
}

//append to internal buffer if any root messages table
void mMotionSensor::WebAppend_Root_Status_Table(){

  // pCONT_web->AppendBuffer_PI2(PSTR("\"%s\":["),PSTR("tab_pir")); 

  JsonBuilderI->Array_Start("tab_pir");// Class name
  

  for(int sensor_id=0;sensor_id<sensors_active;sensor_id++){
    
    JsonBuilderI->Level_Start();
      JsonBuilderI->Add("id",sensor_id);

    char colour_ctr[8];
    uint32_t millis_elapsed = mTime::MillisElapsed(&pir_detect[sensor_id].tEndedTime);
    // Motion in progress
    if(pir_detect[sensor_id].isactive){
      sprintf(colour_ctr,PSTR("#00ff00"));
    }else
    // If movement event has just finished
    if(millis_elapsed<(1000*60)){
      // Show colour as fading back to white over X seconds SINCE EVENT OVER
      uint8_t colour_G = constrain(
                            map(millis_elapsed,0,(1000*60),0,255)
                            ,0,255 //increases with time
                          );
                          //Serial.printf("colour_G=%d\n\r",colour_G);
      // sprintf(colour_ctr,"%s",
      pCONT_web->WebColorCtr(255,colour_G,colour_G, colour_ctr);
      //);
    }
    // no event show, just white
    else{
      sprintf(colour_ctr,"#ffffff");
    }

    
      JsonBuilderI->Add("ih",pir_detect[sensor_id].detected_rtc_ctr);
      JsonBuilderI->Add("fc",colour_ctr);

    // pCONT_web->AppendBuffer_PI2(PSTR("{\"id\":%d,\"ih\":\"%s\",\"fc\":\"%s\"},"),
    //   sensor_id,
    //   pir_detect[sensor_id].detected_rtc_ctr,
    //   colour_ctr
    // );
    
    JsonBuilderI->Level_End();
  }

  // *pCONT_web->buffer_writer_internal = (*pCONT_web->buffer_writer_internal) - 1;// remove extra comma
  // pCONT_web->AppendBuffer_PI2(PSTR("],")); 

  
  JsonBuilderI->Array_End();

}

void mMotionSensor::EveryLoop(){
  

  for(uint8_t sensor_id=0;sensor_id<sensors_active;sensor_id++){
  //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_PIR "PIR %s %d"),PIR_Detected_Ctr(sensor_id),sensor_id);
    if(PIR_Detected(sensor_id)!=pir_detect[sensor_id].state){
      //if(pCONT->mt->mtime.seconds_nonreset<20){ break; }
      pCONT->mqt->ppublish("status/motion/event",PIR_Detected_Ctr(sensor_id),false);
      pir_detect[sensor_id].state = PIR_Detected(sensor_id);
      if(pir_detect[sensor_id].state){
        pir_detect[sensor_id].tDetectTime = millis(); 
        memcpy(pir_detect[sensor_id].detected_rtc_ctr,pCONT->mt->mtime.hhmmss_ctr,sizeof(pCONT->mt->mtime.hhmmss_ctr));
        pir_detect[sensor_id].isactive = true;
      }else{
        pir_detect[sensor_id].tEndedTime = millis();
        pir_detect[sensor_id].isactive = false;
      }
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIR "pir_detect \"%s\""),PIR_Detected_Ctr(sensor_id));
      pir_detect[sensor_id].ischanged = true;
      mqtthandler_sensor_ifchanged.flags.SendNow = true;
    }
  }
  // Use short timer to automatically clear event
  //if > 1 sec
  //clear struct
}

int8_t mMotionSensor::Tasker(uint8_t function){

  // Run even when sensor is disabled (Will set fEnableSensor)
  switch(function){
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
  }
  
  if(!fEnableSensor){ 
    //Serial.println("!fEnableSensor"); 
  return 0; }

      //AddLog_P(LOG_LEVEL_DEBUG,PSTR("\n\r\r\nFUNC_WEB_ADD_ROOT_TABLE_ROWS"));
  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case FUNC_INIT:
      Init();
    break;
    /************
     * SETTINGS SECTION * 
    *******************/
    case FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE: 
      // Settings_Load();
    break;
    case FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE: 
      // Settings_Save();
    break;
    case FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES:
      // Settings_Default();
    break;
    case FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT:
      // Settings_Default();
      // pCONT_set->SettingsSave(2);
    break;
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP:
    // Serial.println("FUNC_LOOP");
      EveryLoop();
    break;     
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_COMMAND:

    break;
    case FUNC_JSON_COMMAND:
      //function_result = parse_JSONCommand();
      // parse_JSONCommand();
    break;
    case FUNC_TEMPLATE_DEVICE_EXECUTE_LOAD:
      // parsesub_TopicCheck_JSONCommand();
    break;
    /************
     * WEBPAGE SECTION * 
    *******************/
    // #ifdef USE_MODULE_CORE_WEBSERVER
    case FUNC_WEB_ADD_ROOT_TABLE_ROWS:{
      char sensor_ctr[50];
      char buffer[50];
      AddLog_P(LOG_LEVEL_DEBUG,PSTR("\n\r\r\nFUNC_WEB_ADD_ROOT_TABLE_ROWS"));

      for(uint8_t sensor_id=0;sensor_id<sensors_active;sensor_id++){
        memset(sensor_ctr,0,sizeof(sensor_ctr));


        sprintf(sensor_ctr,"PIR Motion %s", GetDeviceNamebyIDCtr(sensor_id, buffer, sizeof(buffer)));// pir_detect[sensor_id].friendly_name_ctr);

        BufferWriterI->Append_P(PSTR("<tr>"));
          BufferWriterI->Append_P(PSTR("<td>%s</td>"), sensor_ctr);//pCONT_sup->GetTextIndexed_P(listheading, sizeof(listheading), ii, kTitle_TableTitles_Root));//"Animation List Tester");      //titles are fixed, so send them here using getindex
          BufferWriterI->Append_P(PSTR("<td><div class='%s'>%s</div></td>"),"tab_pir","?");   
        BufferWriterI->Append_P(PSTR("</tr>"));
      }
    }break;
    
    case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
      WebAppend_Root_Status_Table();
    break;
    // #endif //USE_MODULE_CORE_WEBSERVER    
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
      MQTTHandler_Init(); 
    break;
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
      MQTTHandler_Connected();
    break;
    case FUNC_MQTT_DISCONNECTED:
      MQTTHandler_Disconnected();
    break;
    #endif //USE_MQTT
  }

} // END function

int8_t mMotionSensor::Tasker(uint8_t function, JsonObjectConst obj){
  switch(function){
    case FUNC_JSON_COMMAND_OBJECT:
      parse_JSONCommand(obj);
    break;
    case FUNC_JSON_COMMAND_OBJECT_WITH_TOPIC:
      return CheckAndExecute_JSONCommands(obj);
    break;
  }
}


int8_t mMotionSensor::CheckAndExecute_JSONCommands(JsonObjectConst obj){

  // Check if instruction is for me
  if(mSupport::mSearchCtrIndexOf(data_buffer2.topic.ctr,"set/motion")>=0){
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND ));
      pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
      parse_JSONCommand(obj);
      return FUNCTION_RESULT_HANDLED_ID;
  }else{
    return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
  }

}

//relay:1,[1,2],"all",["fountain,bulbs,led"]
void mMotionSensor::parse_JSONCommand(JsonObjectConst obj){ //parse_Command() and pass packet (topic/len/payload/len structure)
  
  // // Check if instruction is for me
  // if(mSupport::mSearchCtrIndexOf(data_buffer2.topic.ctr,"set/motion")>=0){
  //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_RELAYS));
  //     pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
  // }else{
  //   return; // not meant for here
  // }

  // AddLog_P(LOG_LEVEL_DEBUG_LOWLEVEL, PSTR(D_LOG_RELAYS D_DEBUG_FUNCTION "\"%s\""),"mRelays::parse_JSONCommand()");

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_RELAYS "Command: " "\"%s\""),data_buffer2.payload.ctr);

  // StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
  // DeserializationError error = deserializeJson(doc, data_buffer2.payload.ctr);
  // JsonObject obj = doc.as<JsonObject>();

  //parsesub_TopicCheck_JSONCommand(&obj);

}



/*********************************************************************************************
* HELPER & CONVERSION FUNCTIONS ******************************************************************
*************************************************************************************************/

const char* mMotionSensor::GetDeviceNamebyIDCtr(uint8_t device_id, char* buffer, uint8_t buffer_length){
  DEBUG_LINE;
  if(device_id >= sensors_active){ 
    AddLog_P(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "device_id >= sensors_active %d %d"),device_id,sensors_active);
    return PM_SEARCH_NOMATCH; 
  }
  DEBUG_LINE;
  return pCONT_set->GetDeviceName(D_MODULE_SENSORS_MOTION_ID, device_id, buffer, buffer_length);
}

const char* mMotionSensor::GetDeviceNameWithStateLongbyIDCtr(uint8_t device_id, char* buffer, uint8_t buffer_length){
  
  // char onoffctr[5];
  // switch(GetRelay(device_id)){
  //   case 0: 
  //   sprintf(onoffctr,"%s","OFF"); break;
  //   case 1: sprintf(onoffctr,"%s","ON");  break;
  // }

  // char buffer_internal[50];
  // sprintf(buffer, "%s %s", GetRelayNamebyIDCtr(device_id, buffer_internal), onoffctr);

  return buffer;
}

// void mMotionSensor::parsesub_TopicCheck_JSONCommand(JsonObject& _obj){

//   //If its null (nothing passed), try global parameter
//   if(_obj == nullptr){
//     AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_MQTT "obj == nullptr"));
//     if(pCONT_set->pObj != nullptr){
//       AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_MQTT "pCONT_set->pObj != nullptr"));
//       _obj = pCONT_set->pObj;
//     }else{
//       AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_MQTT "No Object"));
//       return;
//     }
//   }

//   // Create local dereferenced variable
//   JsonObject obj = (*_obj); 



// }




#ifdef USE_MQTT

/*********************************************************************************************************************************************
******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/


uint8_t mMotionSensor::ConstructJSON_Settings(uint8_t json_method){

  memset(&data_buffer2,0,sizeof(data_buffer2));

  sprintf(data_buffer2.payload.ctr,
          PSTR("{\"sensors_active\":%d}"),
          sensors_active    
  );

  return strlen(data_buffer2.payload.ctr);

}



// /************ CONSTRUCT JSON BUILDERS *****************************************************************************************************************************/

uint8_t mMotionSensor::ConstructJSON_Sensor(uint8_t json_level){
  
  memset(&data_buffer2,0,sizeof(data_buffer2));
  char buffer[50];

  for(uint8_t sensor_id=0;sensor_id<sensors_active;sensor_id++){
    if(pir_detect[sensor_id].ischanged){ pir_detect[sensor_id].ischanged = false;
      sprintf(data_buffer2.payload.ctr,
              PSTR("{\"location\":\"%s\",\"time\":\"%s\",\"event\":\"%s\"}"),
              GetDeviceNamebyIDCtr(sensor_id, buffer, sizeof(buffer)),
              // pir_detect[sensor_id].friendly_name_ctr,
              pir_detect[sensor_id].detected_rtc_ctr,
              pir_detect[sensor_id].isactive ? F("detected"): F("over")     
      );
    }
  }

  return strlen(data_buffer2.payload.ctr);

}


/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

void mMotionSensor::MQTTHandler_Init(){

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mMotionSensor::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = false;
  mqtthandler_ptr->flags.SendNow = false;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_sensors;
  mqtthandler_ptr->ConstructJSON_function = &mMotionSensor::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"


void mMotionSensor::MQTTHandler_Connected(){
  MQTTHandler_Set_fSendNow();
}
void mMotionSensor::MQTTHandler_Disconnected(){
  // Nothing
}

void mMotionSensor::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mMotionSensor::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mMotionSensor::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t flag_handle_all = false, handler_found = false;
  if(mqtt_handler_id == MQTT_HANDLER_ALL_ID){ flag_handle_all = true; } //else run only the one asked for

  do{

    switch(mqtt_handler_id){
      case MQTT_HANDLER_SETTINGS_ID:                       handler_found=true; mqtthandler_ptr=&mqtthandler_settings_teleperiod; break;
      case MQTT_HANDLER_SENSOR_IFCHANGED_ID:               handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_ifchanged; break;
      default: handler_found=false; break; // nothing 
    } // switch

    // Pass handlers into command to test and (ifneeded) execute
    if(handler_found){ pCONT->mqt->MQTTHandler_Command(*this,D_MODULE_SENSORS_MOTION_ID,mqtthandler_ptr); }

    // stop searching
    if(mqtt_handler_id++>MQTT_HANDLER_MODULE_LENGTH_ID){flag_handle_all = false; return;}

  }while(flag_handle_all);

}

//////////////////// END OF MQTT /////////////////////////

#endif

#endif
