#include "mMotionSensor.h"

#ifdef USE_MODULE_SENSORS_MOTION


int8_t mMotionSensor::Tasker(uint8_t function){

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
    case FUNC_INIT:
      Init();
    break;
  }
  
  if(!settings.fEnableSensor){ return 0; }

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
    case FUNC_JSON_COMMAND_CHECK_TOPIC_ID:
      CheckAndExecute_JSONCommands();
    break;
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand();
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
    /************
     * WEBPAGE SECTION * 
    *******************/
    #ifdef USE_MODULE_CORE_WEBSERVER
    case FUNC_WEB_ADD_ROOT_TABLE_ROWS:
      WebAppend_Root_Draw_PageTable();
    break;
    case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
      WebAppend_Root_Status_Table();
    break;
    #endif //USE_MODULE_CORE_WEBSERVER
  }

} // END function


#ifndef DISABLE_WEBSERVER
void mMotionSensor::WebAppend_Root_Draw_PageTable(){
char buffer[50];
for(uint8_t sensor_id=0;sensor_id<settings.sensors_active;sensor_id++){       
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
    BufferWriterI->Append_P(PSTR("<td>PIR Motion %s</td>"), pCONT_set->GetDeviceName(D_MODULE_SENSORS_MOTION_ID, sensor_id, buffer, sizeof(buffer)));
    BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_CLASS_TYPE_2V,"tab_pir","?");   
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
}
}


void mMotionSensor::WebPage_Root_AddHandlers(){
  // Not needed
}

//append to internal buffer if any root messages table
void mMotionSensor::WebAppend_Root_Status_Table(){

  char buffer[20];

  JsonBuilderI->Array_Start("tab_pir");// Class name
  for(int sensor_id=0;sensor_id<settings.sensors_active;sensor_id++){
    JsonBuilderI->Level_Start();
      JsonBuilderI->Add("id",sensor_id);

        char colour_ctr[8];
        uint32_t millis_elapsed = mTime::MillisElapsed(&pir_detect[sensor_id].tEndedTime);
        // Motion in progress
        if(pir_detect[sensor_id].isactive){
          sprintf_P(colour_ctr,PSTR("#00ff00"));
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
          pCONT_web->WebColorCtr(255,colour_G,colour_G, colour_ctr, sizeof(colour_ctr));
          //);
        }
        // no event show, just white
        else{
          sprintf_P(colour_ctr,PSTR("#ffffff"));
        }


      JsonBuilderI->Add("ih",
        pCONT_time->ConvertShortTime_HHMMSS(&pir_detect[sensor_id].detected_time, buffer, sizeof(buffer)));
      
      //detected_rtc_ctr);
      JsonBuilderI->Add("fc",colour_ctr);    
    JsonBuilderI->Level_End();
  }

  JsonBuilderI->Array_End();

}
#endif // DISABLE_WEBSERVER

void mMotionSensor::Pre_Init(void){

  settings.sensors_active = 0;

  settings.fEnableSensor = false;

  uint16_t pin_id = 0;
  for(uint8_t ii=0;ii<(GPIO_PIR_3_INV_ID-GPIO_PIR_1_ID+1);ii++){
    
    pin_id = GPIO_PIR_1_ID+ii;
    // Make sure to not exceed limits
    if(settings.sensors_active>MAXIMUM_SENSORS){ break; }

    if(pCONT_pins->PinUsed(pin_id)){
      // Save GPIO
      pin[settings.sensors_active] = pCONT_pins->GetPin(pin_id);
      // config
      switch (pin_id){
        case GPIO_PIR_1_INV_ID:
        case GPIO_PIR_2_INV_ID:
        case GPIO_PIR_3_INV_ID:
          pin_isinverted[settings.sensors_active] = true;
          pinMode(pin[settings.sensors_active], INPUT_PULLUP);
        break;
        case GPIO_PIR_1_NP_INV_ID:
        case GPIO_PIR_2_NP_INV_ID:
        case GPIO_PIR_3_NP_INV_ID:
          pin_isinverted[settings.sensors_active] = true;
          pinMode(pin[settings.sensors_active], INPUT);
        break;
        case GPIO_PIR_1_NP_ID:
        case GPIO_PIR_2_NP_ID:
        case GPIO_PIR_3_NP_ID:
          pin_isinverted[settings.sensors_active] = false;
          pinMode(pin[settings.sensors_active], INPUT);   
        break;   
        default:
          pin_isinverted[settings.sensors_active] = false;
          pinMode(pin[settings.sensors_active], INPUT);
        break;
      }
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_PIR "pin[%d] %d"), settings.sensors_active, pin[settings.sensors_active]);
      settings.sensors_active++;
      settings.fEnableSensor = true;
    }
    
  }

}


void mMotionSensor::Init(){

  for(uint8_t sensor_id=0;sensor_id<settings.sensors_active;sensor_id++){
    pir_detect[sensor_id].state = PIR_Detected(sensor_id);
  }

  #ifdef DEVICE_SIDEDOORLIGHT
  settings.motion_trigger_type = MOTION_TRIGGER_TYPE_COMMANDS_INTERNAL_POWER_COMMAND_ID;
  #endif

}

uint8_t mMotionSensor::PIR_Detected(uint8_t sensor_id){
  return pin_isinverted[sensor_id] ? digitalRead(pin[sensor_id]) : !digitalRead(pin[sensor_id]);
}

const char* mMotionSensor::PIR_Detected_Ctr(uint8_t sensor_id, char* buffer, uint8_t buflen){
  switch(PIR_Detected(sensor_id)){
    case 0: snprintf_P(buffer, buflen, PM_JSON_MOTION_ENDED);
    case 1: snprintf_P(buffer, buflen, PM_JSON_MOTION_STARTED);
  }
  return buffer;
}


void mMotionSensor::EveryLoop(){

  // pinMode(12, INPUT_PULLUP);

  // AddLog_P(LOG_LEVEL_TEST, PSTR("MOTION"));

  // Serial.println(digitalRead(12));
  

  for(uint8_t sensor_id=0;sensor_id<settings.sensors_active;sensor_id++){
  //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_PIR "PIR %s %d"),PIR_Detected_Ctr(sensor_id),sensor_id);
    if(PIR_Detected(sensor_id)!=pir_detect[sensor_id].state){
      //if(pCONT->mt->RtcTime.seconds_nonreset<20){ break; }
      // pCONT->mqt->ppublish("status/motion/event",PIR_Detected_Ctr(sensor_id),false);
      pir_detect[sensor_id].state = PIR_Detected(sensor_id);



      if(pir_detect[sensor_id].state){
        pir_detect[sensor_id].tDetectTime = millis(); 
//        pir_detect[sensor_id].detected_time_of_day_seconds = pCONT_time->GetTimeOfDay_Seconds();

AddLog_P(LOG_LEVEL_TEST,PSTR(DEBUG_INSERT_PAGE_BREAK "pir_detect[sensor_id].state=%d"),pir_detect[sensor_id].state);
        
        pir_detect[sensor_id].detected_time = pCONT_time->GetTimeShortNow();
        
        // memcpy(pir_detect[sensor_id].detected_rtc_ctr,pCONT->mt->RtcTime.hhmmss_ctr,sizeof(pCONT->mt->RtcTime.hhmmss_ctr));
        pir_detect[sensor_id].isactive = true;

        #ifdef USE_MODULE_CORE_RULES
        pCONT_rules->New_Event(D_MODULE_SENSORS_MOTION_ID, sensor_id);
        #endif
        pCONT->Tasker_Interface(FUNC_EVENT_MOTION_STARTED_ID);

        if(pCONT_time->UpTime()>60){
          if(settings.motion_trigger_type == MOTION_TRIGGER_TYPE_COMMANDS_INTERNAL_POWER_COMMAND_ID){
            pCONT->Tasker_Interface(FUNC_SET_POWER_ON_ID);
            #ifdef USE_MODULE_DRIVERS_RELAY
              pCONT_mry->CommandSet_Timer_Decounter(60); //retrigger to 60 seconds
            #endif // USE_MODULE_DRIVERS_RELAY
          }
        }

      }else{
        pir_detect[sensor_id].tEndedTime = millis();
        pir_detect[sensor_id].isactive = false;

        if(settings.motion_trigger_type == MOTION_TRIGGER_TYPE_COMMANDS_INTERNAL_POWER_COMMAND_ID){
          pCONT->Tasker_Interface(FUNC_SET_POWER_OFF_ID);
        }

        #ifdef USE_MODULE_CORE_RULES
        pCONT_rules->New_Event(D_MODULE_SENSORS_MOTION_ID, sensor_id);
        #endif
        pCONT->Tasker_Interface(FUNC_EVENT_MOTION_ENDED_ID);

      }
      // char buffer[20];
      // char buffer2[20];
      // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_PIR "pir_detect \"%s\" @ %s"),
      //                                   PIR_Detected_Ctr(sensor_id, buffer, sizeof(buffer)),
      //                                   mTime::ConvertTimeOfDay_Seconds_HHMMSS(pir_detect[sensor_id].detected_time_of_day_seconds, buffer2, sizeof(buffer2))                                                                          
      //         );

      
// char buffer[20];
//       char buffer2[20];
//       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_PIR "pir_detect %d @ %s"),
//                                         pCONT_time->GetTimeOfDay_Seconds(),
//                                         mTime::ConvertTimeOfDay_Seconds_HHMMSS(pCONT_time->GetTimeOfDay_Seconds(), buffer2, sizeof(buffer2))                                                                          
//               );


      pir_detect[sensor_id].ischanged = true;
      mqtthandler_sensor_ifchanged.flags.SendNow = true;
    }
  }
  // Use short timer to automatically clear event
  //if > 1 sec
  //clear struct




}



int8_t mMotionSensor::CheckAndExecute_JSONCommands(){

  if(mSupport::mSearchCtrIndexOf(data_buffer.topic.ctr,"set/motion")>=0){
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND));
    pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
    parse_JSONCommand();
    return FUNCTION_RESULT_HANDLED_ID;
  }else{
    return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
  }

}


void mMotionSensor::parse_JSONCommand(){ //parse_Command() and pass packet (topic/len/payload/len structure)
  
  // Need to parse on a copy
  char parsing_buffer[data_buffer.payload.len+1];
  memcpy(parsing_buffer,data_buffer.payload.ctr,sizeof(char)*data_buffer.payload.len+1);
  JsonParser parser(parsing_buffer);
  JsonParserObject obj = parser.getRootObject();   
  if (!obj) { 
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_ERROR, PSTR(D_JSON_DESERIALIZATION_ERROR));
    #endif //ENABLE_LOG_LEVEL_COMMANDS
    return;
  }  
  JsonParserToken jtok = 0; 

}


/*********************************************************************************************************************************************
******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/


uint8_t mMotionSensor::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add_P(PM_JSON_SENSORCOUNT, settings.sensors_active);
    JsonBuilderI->Array_AddArray("pin", pin, 3);


    //debug
    // JsonBuilderI->Add_P("pin[0]", settings.sensors_active);
    JsonBuilderI->Array_Start("pin_state");
      JsonBuilderI->Add_FV(PSTR("%d"),digitalRead(pin[0]));
      JsonBuilderI->Add_FV(PSTR("%d"),digitalRead(pin[1]));
      JsonBuilderI->Add_FV(PSTR("%d"),digitalRead(pin[2]));
    JsonBuilderI->Array_End();
    




  return JsonBuilderI->End();

}


uint8_t mMotionSensor::ConstructJSON_Sensor(uint8_t json_level){
  
  char buffer[80];
  // char buffer2[20];

  JsonBuilderI->Start();

  for(uint8_t sensor_id=0;sensor_id<settings.sensors_active;sensor_id++){
    if(pir_detect[sensor_id].ischanged){ 
      
      pir_detect[sensor_id].ischanged = false;
      
      JsonBuilderI->Add(D_JSON_LOCATION, pCONT_set->GetDeviceName(D_MODULE_SENSORS_MOTION_ID, sensor_id, buffer, sizeof(buffer)));
      JsonBuilderI->Add(D_JSON_TIME, mTime::ConvertShortTime_HHMMSS(&pir_detect[sensor_id].detected_time, buffer, sizeof(buffer)));
      JsonBuilderI->Add(D_JSON_EVENT, pir_detect[sensor_id].isactive ? "detected": "over");

      //if another is yet to send, then reset the mqtt_handler to fire immeditely again!
      //if any mtion flag remains, then set mqtt again
      
    }
  }

  return JsonBuilderI->End();

}


/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

#ifdef USE_MQTT

void mMotionSensor::MQTTHandler_Init(){

  struct handler<mMotionSensor>* mqtthandler_ptr;

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = SEC_IN_HOUR; 
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
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mMotionSensor::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"


void mMotionSensor::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mMotionSensor::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mMotionSensor::MQTTHandler_Sender(uint8_t mqtt_handler_id){
 
  uint8_t mqtthandler_list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID, MQTT_HANDLER_SENSOR_IFCHANGED_ID
  };
  
  struct handler<mMotionSensor>* mqtthandler_list_ptr[] = {
    &mqtthandler_settings_teleperiod, &mqtthandler_sensor_ifchanged
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, D_MODULE_SENSORS_MOTION_ID,
    mqtthandler_list_ptr, mqtthandler_list_ids,
    sizeof(mqtthandler_list_ptr)/sizeof(mqtthandler_list_ptr[0]),
    mqtt_handler_id
  );

}

//////////////////// END OF MQTT /////////////////////////

#endif

#endif
