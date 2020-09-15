#include "mDoorSensor.h"

#ifdef USE_MODULE_SENSORS_DOOR

void mDoorSensor::pre_init(void){

  if(pCONT_pins->PinUsed(GPIO_DOOR_OPEN_ID)) {  // not set when 255
    pin_open = pCONT_pins->GetPin(GPIO_DOOR_OPEN_ID);
    pinMode(pin_open,INPUT_PULLUP);
  }else{
    AddLog_P(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin Invalid %d"),pin_open);
    //disable pir code
  }

  // if(pCONT_set->pin[GPIO_DOOR_LOCK_ID] < 99) {  // not set when 255
  //   pin_lock = pCONT_set->pin[GPIO_DOOR_LOCK_ID];
  //   pinMode(pin_lock,INPUT_PULLUP);
  // }else{
  //   AddLog_P(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin Invalid %d"),pin_lock);
  //   //disable pir code
  // }

}


uint8_t mDoorSensor::IsDoorOpen(){
  return digitalRead(pin_open);
}

const char* mDoorSensor::IsDoorOpen_Ctr(){
  return IsDoorOpen() ? "Open" : "Closed";
}



void mDoorSensor::init(void){
  // REED_SWITCH_INIT();
  // #ifdef DOORLOCK_SWITCH_PIN
  //   DOORLOCK_SWITCH_INIT();
  // #endif
  
  sprintf(door_detect.detected_rtc_ctr,"--:--:--\0");

  #ifdef DOORALERT_PAYLOAD_CTR
    sprintf(door_detect.friendly_name_ctr,DOORALERT_PAYLOAD_CTR);
  #else
    sprintf(door_detect.friendly_name_ctr,"undefined");
  #endif



}

// const char HTTP_DEVICE_STATE[] PROGMEM = "<td style='width:%d{c}%s;font-size:%dpx'>%s</div></td>";  // {c} = %'><div style='text-align:center;font-weight:

int8_t mDoorSensor::Tasker(uint8_t function){
  
  switch(function){
    case FUNC_PRE_INIT:
      pre_init();
    break;
    case FUNC_INIT:
      init();
    break;
    case FUNC_LOOP: 

      if((IsDoorOpen()!=door_detect.state)&&(abs(millis()-door_detect.tDetectTimeforDebounce)>100)){
        door_detect.state = IsDoorOpen(); //tDetectTime = millis();
        if(door_detect.state){ Serial.print("Active high door_detect");
          door_detect.isactive = true;
          door_detect.tDetectTimeforDebounce = millis();
          memcpy(door_detect.detected_rtc_ctr,pCONT->mt->mtime.hhmmss_ctr,sizeof(pCONT->mt->mtime.hhmmss_ctr));
        }else{ Serial.print("Active low door_detect");
          door_detect.isactive = false;
        }
        door_detect.ischanged = true;
        door_detect.changedtime = pCONT->mt->mtime;
        fUpdateSendDoorSensor = true;
      }

      #ifdef DOORLOCK_SWITCH_PIN //set flasg if pin changes
        if(LOCKOPEN()!=lock_detect.state){
          lock_detect.state = LOCKOPEN();
          //tDetectTime = millis();
          if(lock_detect.state){ Serial.print("Active high door_detect");
            lock_detect.isactive = true;
          }else{ Serial.print("Active low door_detect");
            lock_detect.isactive = false;
          }
          lock_detect.ischanged = true;
          lock_detect.changedtime = pCONT->mt->mtime;
          fUpdateSendDoorSensor = true;
        }
      #endif

    break;
    case FUNC_MQTT_SENDER:
      SubTasker_MQTTSender();
    break;
    case FUNC_WEB_SHOW_PARAMETERS:{
      
      // uint8_t fsize = 16;
      // char sensor_ctr[50];

      // memset(sensor_ctr,0,sizeof(sensor_ctr));
      // sprintf(sensor_ctr,"Reed %s Door Position",door_detect.friendly_name_ctr);
      // pCONT_web->WSBufferAppend_P(HTTP_SNS_GENERIC, 
      //   sensor_ctr,
      //   door_detect.state?"Opened":"Closed"
      // );

      // memset(sensor_ctr,0,sizeof(sensor_ctr));
      // sprintf(sensor_ctr,"Reed %s Door Last Opened",door_detect.friendly_name_ctr);
      // pCONT_web->WSBufferAppend_P(HTTP_SNS_GENERIC, 
      //   sensor_ctr,
      //   door_detect.detected_rtc_ctr
      // );
      
    }break;
  }

} // END function


//#ifdef USE_MQTT

// NEW METHOD -- first senders then on internals
void mDoorSensor::SubTasker_MQTTSender(){

  if((abs(millis()-tSavedSendDoorSensor)>=(60000*10))||(fUpdateSendDoorSensor)){ tSavedSendDoorSensor=millis(); fUpdateSendDoorSensor = false;
    MQQTSendDoorUpdate();
  }

  MQTTSendDoorSensorIfChanged(); // No timed delay

  #ifdef DOORLOCK_SWITCH_PIN
    MQTTSendDoorLockIfChanged(); // No timed delay
  #endif

}

void mDoorSensor::MQTTSendDoorSensorIfChanged(){

  if(door_detect.ischanged){door_detect.ischanged=false;

    memset(&data_buffer2,0,sizeof(data_buffer2));

    StaticJsonDocument<300> doc;
    JsonObject root = doc.to<JsonObject>();

    root["location"] = DOORALERT_PAYLOAD_CTR;
    root["time"] = pCONT->mt->mtime.hhmmss_ctr;

    data_buffer2.payload.len = measureJson(root)+1;
    serializeJson(doc,data_buffer2.payload.ctr);

    if(door_detect.isactive){
      pCONT->mqt->ppublish("status/motion/detected",data_buffer2.payload.ctr,false);
    }else{
      pCONT->mqt->ppublish("status/motion/over",data_buffer2.payload.ctr,false);
    }

  }

} // END function


#ifdef DOORLOCK_SWITCH_PIN
void mDoorSensor::MQTTSendDoorLockIfChanged(){

  if(lock_detect.ischanged){ lock_detect.ischanged=false;

    memset(&data_buffer2,0,sizeof(data_buffer2));

    StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
    JsonObject root = doc.to<JsonObject>();
    root["location"] = LOCKOPENCTR;
    root["time"] = pCONT->mt->mtime.hhmmss_ctr;

    data_buffer2.payload.len = measureJson(root)+1;
    serializeJson(doc,data_buffer2.payload.ctr);

    pCONT->mqt->ppublish("status/door/lock",data_buffer2.payload.ctr,false);

  }

} // END function
#endif


void mDoorSensor::MQQTSendDoorUpdate(void){

  memset(&data_buffer2,0,sizeof(data_buffer2));

  StaticJsonDocument<400> doc;
  JsonObject root = doc.to<JsonObject>();

  char buffer[10];

  JsonObject doorobj = root.createNestedObject("door");
    doorobj["position"] = IsDoorOpen() ? "Open" : "Closed";//IsDoorOpen_Ctr();
    char timectr[12]; memset(timectr,0,sizeof(timectr));
    sprintf(timectr,"%sT%02d:%02d:%02d",
      pCONT->mt->GetDOWShortctr(door_detect.changedtime.Wday, buffer),
      door_detect.changedtime.hour,
      door_detect.changedtime.minute,
      door_detect.changedtime.second
    );
    doorobj["changedtime"] = timectr;

  #ifdef DOORLOCK_SWITCH_PIN
  JsonObject lockobj = root.createNestedObject("lock");
    lockobj["position"] = LOCKOPENCTR;
    char timectr2[15]; memset(timectr2,0,sizeof(timectr2));
    sprintf(timectr2,"%sT%02d:%02d:%02d",
      pCONT->mt->GetDOWShortctr(lock_detect.changedtime.Wday),
      lock_detect.changedtime.hour,
      lock_detect.changedtime.minute,
      lock_detect.changedtime.second
    );
    lockobj["changedtime"] = timectr2;
  #endif

  data_buffer2.payload.len = measureJson(root)+1;
  serializeJson(doc,data_buffer2.payload.ctr);

  pCONT->mqt->ppublish("status/door/status",data_buffer2.payload.ctr,false);

}



/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/
////////////////////// START OF MQTT /////////////////////////

void mDoorSensor::MQTTHandler_Init(){

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_settings;
  mqtthandler_ptr->ConstructJSON_function = &mDoorSensor:
  :ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_sensors;
  mqtthandler_ptr->ConstructJSON_function = &mDoorSensor::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_sensors;
  mqtthandler_ptr->ConstructJSON_function = &mDoorSensor::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"


void mDoorSensor::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.fSendNow = true;
  mqtthandler_sensor_ifchanged.fSendNow = true;
  mqtthandler_sensor_teleperiod.fSendNow = true;

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
