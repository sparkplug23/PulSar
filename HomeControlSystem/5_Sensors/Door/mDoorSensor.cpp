#include "mDoorSensor.h"

#ifdef USE_MODULE_SENSORS_DOOR

void mDoorSensor::pre_init(void){
  
  settings.fEnableSensor = false;

  if(pCONT_pins->PinUsed(GPIO_DOOR_OPEN_ID)) {  // not set when 255
    // pin_open = pCONT_pins->GetPin(GPIO_DOOR_OPEN_ID);
    pinMode(pCONT_pins->GetPin(GPIO_DOOR_OPEN_ID),INPUT_PULLUP);
    settings.fEnableSensor = true;
  }else{
    AddLog_P(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin Invalid %d"),pCONT_pins->GetPin(GPIO_DOOR_OPEN_ID));
    //disable pir code
  }

  // if(pCONT_set->pin[GPIO_DOOR_LOCK_ID] < 99) {  // not set when 255
  //   pin_lock = pCONT_set->pin[GPIO_DOOR_LOCK_ID];
  //   pinMode(pin_lock,INPUT_PULLUP);
  // }else{
  //   AddLog_P(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin Invalid %d"),pin_lock);
  //   //disable pir code
  // }

  // pin_open = D6;
  // pinMode(pin_open,INPUT_PULLUP);

}


uint8_t mDoorSensor::IsDoorOpen(){
  return digitalRead(pCONT_pins->GetPin(GPIO_DOOR_OPEN_ID));
}


void mDoorSensor::init(void){

  sprintf(door_detect.detected_rtc_ctr,"--:--:--\0");

  //record init state
  door_detect.state = IsDoorOpen();

  #ifdef DOORALERT_PAYLOAD_CTR
    sprintf(door_detect.friendly_name_ctr,DOORALERT_PAYLOAD_CTR);
  #else
    sprintf(door_detect.friendly_name_ctr,"undefined");
  #endif

}


int8_t mDoorSensor::Tasker(uint8_t function){

  switch(function){
    case FUNC_PRE_INIT:
      pre_init();
    break;
    case FUNC_INIT:
      init();
    break;
  }

  // AddLog_P(LOG_LEVEL_DEBUG, PSTR("mDoorSensor::Tasker"));
  if(!settings.fEnableSensor){ return 0; }
  // AddLog_P(LOG_LEVEL_DEBUG, PSTR("mDoorSensor::Tasker return"));

  
  // pin_open = D4;
  // pinMode(pin_open,INPUT_PULLUP);
  // AddLog_P(LOG_LEVEL_DEBUG, PSTR("mDoorSensor::Pin %s"),digitalRead(D4)?"ON":"OFF");
  
  switch(function){
    case FUNC_LOOP: 

      if((IsDoorOpen()!=door_detect.state)&&(abs(millis()-door_detect.tDetectTimeforDebounce)>100)){
        door_detect.state = IsDoorOpen(); //tDetectTime = millis();
        if(door_detect.state){ 
          //Serial.print("Active high door_detect");
          door_detect.isactive = true;
          door_detect.tDetectTimeforDebounce = millis();
          memcpy(door_detect.detected_rtc_ctr,pCONT->mt->mtime.hhmmss_ctr,sizeof(pCONT->mt->mtime.hhmmss_ctr));
          // pCONT->mqt->ppublish("status/motion/event",door_detect.isactive? "MOTION" : "NO_MOTION",false); //phase this out for the new flag method below
          mqtthandler_sensor_ifchanged.flags.SendNow = true;
        }else{ 
          //Serial.print("Active low door_detect");
          door_detect.isactive = false;
        }
        door_detect.ischanged = true;
        mqtthandler_sensor_ifchanged.flags.SendNow = true;
        door_detect.changedtime = pCONT->mt->mtime;
        fUpdateSendDoorSensor = true;
      }

      // #ifdef DOORLOCK_SWITCH_PIN //set flasg if pin changes
      //   if(LOCKOPEN()!=lock_detect.state){
      //     lock_detect.state = LOCKOPEN();
      //     //tDetectTime = millis();
      //     if(lock_detect.state){ Serial.print("Active high door_detect");
      //       lock_detect.isactive = true;
      //     }else{ Serial.print("Active low door_detect");
      //       lock_detect.isactive = false;
      //     }
      //     lock_detect.ischanged = true;
      //     lock_detect.changedtime = pCONT->mt->mtime;
      //     fUpdateSendDoorSensor = true;
      //   }
      // #endif

    break;
    case FUNC_EVERY_SECOND:{
      char buffer[30];
      AddLog_P(LOG_LEVEL_INFO,PSTR("Door open sensor %s"),IsDoorOpen_Ctr(buffer, sizeof(buffer)));
    }break;
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
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_fSendNow();
    break;
    #endif //USE_MQTT    
    /************
     * WEBPAGE SECTION * 
    *******************/
    #ifdef USE_MODULE_CORE_WEBSERVER
    case FUNC_WEB_ADD_ROOT_TABLE_ROWS:
      WebAppend_Root_Status_Table_Draw();
      break;
    case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
      WebAppend_Root_Status_Table_Data();
      break;
    #endif //USE_MODULE_CORE_WEBSERVER
  }

} // END function


void mDoorSensor::WebAppend_Root_Status_Table_Draw(){

  char value_ctr[8];
  uint8_t sensor_counter = 0;
  char buffer[10];
    
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
    BufferWriterI->Append_P(PSTR("<td>Door Position</td>"));
    BufferWriterI->Append_P(PSTR("<td>{dc}%s'>%s</div></td>"),"tab_door", IsDoorOpen_Ctr(buffer, sizeof(buffer)));   
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
  
}


//append to internal buffer if any root messages table
void mDoorSensor::WebAppend_Root_Status_Table_Data(){
  
  uint8_t sensor_counter = 0;
  char value_ctr[8];
  char colour_ctr[10];
  char inner_html[100];
  char door_pos_ctr[20];

  JsonBuilderI->Array_Start("tab_door");// Class name
  
  for(int sensor_id=0;sensor_id<1;sensor_id++){
    
    JsonBuilderI->Level_Start();
      JsonBuilderI->Add("id",sensor_id);

      char colour_ctr[8];
      uint32_t millis_elapsed = mTime::MillisElapsed(&door_detect.tEndedTime);
      // Motion in progress
      if(door_detect.isactive){
        sprintf(colour_ctr,PSTR("#00ff00"));
      }else
      // If movement event has just finished
      if(millis_elapsed<(1000*60)){
        // Show colour as fading back to white over X seconds SINCE EVENT OVER
        uint8_t colour_G = constrain(
                              map(millis_elapsed,0,(1000*60),0,255)
                              ,0,255 //increases with time
                            );
        pCONT_web->WebColorCtr(255,colour_G,colour_G, colour_ctr);
      }
      // no event show, just white
      else{
        sprintf(colour_ctr,"#ffffff");
      }

      sprintf(inner_html,"%s %s",IsDoorOpen_Ctr(door_pos_ctr,sizeof(door_pos_ctr)),door_detect.detected_rtc_ctr);
    
      JsonBuilderI->Add("ih",inner_html);
      JsonBuilderI->Add("fc",colour_ctr);
    
    JsonBuilderI->Level_End();
  }

  JsonBuilderI->Array_End();

}


  const char* mDoorSensor::IsDoorOpen_Ctr(char* buffer, uint8_t buflen){

    // if(door_detect.wasactive && !door_detect.isactive){
    //   snprintf_P(buffer, buflen, PM_EVENT_DOOR_OPENING_CTR, sizeof(PM_EVENT_DOOR_OPENING_CTR));
    // }else
    // if(door_detect.wasactive && door_detect.isactive){
    //   snprintf_P(buffer, buflen, PM_EVENT_DOOR_OPENING_CTR, sizeof(PM_EVENT_DOOR_OPENING_CTR));
    // }
    // else
    // if(door_detect.wasactive && door_detect.isactive){
    //   snprintf_P(buffer, buflen, PM_EVENT_DOOR_OPENING_CTR, sizeof(PM_EVENT_DOOR_OPENING_CTR));
    // }else
    // if(door_detect.wasactive && door_detect.isactive){
    //   snprintf_P(buffer, buflen, PM_EVENT_DOOR_OPENING_CTR, sizeof(PM_EVENT_DOOR_OPENING_CTR));
    // }

    
    if(door_detect.isactive){
      snprintf_P(buffer, buflen, PM_EVENT_DOOR_OPENED_CTR, sizeof(PM_EVENT_DOOR_OPENED_CTR));
    }else{
      snprintf_P(buffer, buflen, PM_EVENT_DOOR_CLOSED_CTR, sizeof(PM_EVENT_DOOR_CLOSED_CTR));
    }
    return buffer;

  }



// #ifdef DOORLOCK_SWITCH_PIN
// void mDoorSensor::MQTTSendDoorLockIfChanged(){

//   if(lock_detect.ischanged){ lock_detect.ischanged=false;

//     memset(&data_buffer2,0,sizeof(data_buffer2));

//     StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
//     JsonObject root = doc.to<JsonObject>();
//     root["location"] = LOCKOPENCTR;
//     root["time"] = pCONT->mt->mtime.hhmmss_ctr;

//     data_buffer2.payload.len = measureJson(root)+1;
//     serializeJson(doc,data_buffer2.payload.ctr);

//     pCONT->mqt->ppublish("status/door/lock",data_buffer2.payload.ctr,false);

//   }

// } // END function
// #endif



uint8_t mDoorSensor::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
    // JsonBuilderI->Add(D_JSON_SENSOR_COUNT, 0);
  return JsonBuilderI->End();

}


uint8_t mDoorSensor::ConstructJSON_Sensor(uint8_t json_level){
  
  
  if(json_level >= JSON_LEVEL_IFCHANGED){

    char buffer[50];

    JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_LOCATION, pCONT_set->GetDeviceName(D_MODULE_SENSORS_DOOR_ID,0,buffer,sizeof(buffer)));
    JsonBuilderI->Add(D_JSON_TIME, door_detect.detected_rtc_ctr);
    
    if(door_detect.ischanged){ door_detect.ischanged = false;
      JsonBuilderI->Add(D_JSON_EVENT, door_detect.isactive ? "detected": "over");
    }




    // JsonObject doorobj = root.createNestedObject("door");
    //   doorobj["position"] = IsDoorOpen() ? "Open" : "Closed";//IsDoorOpen_Ctr();
    //   char timectr[12]; memset(timectr,0,sizeof(timectr));
    //   sprintf(timectr,"%sT%02d:%02d:%02d",
    //     pCONT->mt->GetDOWShortctr(door_detect.changedtime.Wday, buffer),
    //     door_detect.changedtime.hour,
    //     door_detect.changedtime.minute,
    //     door_detect.changedtime.second
    //   );
    //   doorobj["changedtime"] = timectr;

    // #ifdef DOORLOCK_SWITCH_PIN
    // JsonObject lockobj = root.createNestedObject("lock");
    //   lockobj["position"] = LOCKOPENCTR;
    //   char timectr2[15]; memset(timectr2,0,sizeof(timectr2));
    //   sprintf(timectr2,"%sT%02d:%02d:%02d",
    //     pCONT->mt->GetDOWShortctr(lock_detect.changedtime.Wday),
    //     lock_detect.changedtime.hour,
    //     lock_detect.changedtime.minute,
    //     lock_detect.changedtime.second
    //   );
    //   lockobj["changedtime"] = timectr2;
    // #endif
    return JsonBuilderI->End();

  }
  
}


/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/
////////////////////// START OF MQTT /////////////////////////

void mDoorSensor::MQTTHandler_Init(){

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mDoorSensor::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mDoorSensor::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = false;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mDoorSensor::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"


void mDoorSensor::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mDoorSensor::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mDoorSensor::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t flag_handle_all = false, handler_found = false;
  if(mqtt_handler_id == MQTT_HANDLER_ALL_ID){ flag_handle_all = true; } //else run only the one asked for

  do{

    switch(mqtt_handler_id){
      case MQTT_HANDLER_SETTINGS_ID:                       handler_found=true; mqtthandler_ptr=&mqtthandler_settings_teleperiod; break;
      case MQTT_HANDLER_SENSOR_IFCHANGED_ID:               handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_ifchanged; break;
      case MQTT_HANDLER_SENSOR_TELEPERIOD_ID:              handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_teleperiod; break;
      // No specialised needed
      default: handler_found=false; break; // nothing 
    } // switch

    // Pass handlers into command to test and (ifneeded) execute
    if(handler_found){ pCONT->mqt->MQTTHandler_Command(*this,D_MODULE_SENSORS_DOOR_ID,mqtthandler_ptr); }

    // stop searching
    if(mqtt_handler_id++>MQTT_HANDLER_MODULE_LENGTH_ID){flag_handle_all = false; return;}

  }while(flag_handle_all);

}


////////////////////// END OF MQTT /////////////////////////



#endif


