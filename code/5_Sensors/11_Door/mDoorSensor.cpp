#include "mDoorSensor.h"

#ifdef USE_MODULE_SENSORS_DOOR

const char* mDoorSensor::PM_MODULE_SENSORS_DOOR_CTR = D_MODULE_SENSORS_DOOR_CTR;
const char* mDoorSensor::PM_MODULE_SENSORS_DOOR_FRIENDLY_CTR = D_MODULE_SENSORS_DOOR_FRIENDLY_CTR;

// Should this be a controller? Since the underlying input is from switches sensor. This would also mean motion should be controller.
// But then how does motion + others become an "event"
// Event should be anything really, not just switch ie power level threshold crossed


int8_t mDoorSensor::Tasker(uint8_t function, JsonParserObject obj)
{
  
  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
    case FUNC_INIT:
      init();
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
    case FUNC_EVERY_SECOND:

      // if(pCONT_pins->PinUsed(GPIO_DOOR_LOCK_ID)) // phase out in favour of basic switch? if so, doorsensor can become similar to motion that is non-resetting
      // {
      //   AddLog(LOG_LEVEL_TEST, PSTR("DoorLockPin=%d"), digitalRead(pCONT_pins->GetPin(GPIO_DOOR_LOCK_ID)));
      // }

    break;
    case FUNC_SENSOR_SHOW_LATEST_LOGGED_ID:
      ShowSensor_AddLog();
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
      MQTTHandler_Set_RefreshAll();
    break;
    #endif //USE_MODULE_NETWORK_MQTT    
    /************
     * WEBPAGE SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case FUNC_WEB_ADD_ROOT_TABLE_ROWS:
      WebAppend_Root_Status_Table_Draw();
      break;
    case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
      WebAppend_Root_Status_Table_Data();
      break;
    #endif //USE_MODULE_NETWORK_WEBSERVER
  }

} // END function



void mDoorSensor::Pre_Init(void){
  
  settings.fEnableSensor = false;

  if(pCONT_pins->PinUsed(GPIO_DOOR_OPEN_ID)) {  // not set when 255
    // pin_open = pCONT_pins->GetPin(GPIO_DOOR_OPEN_ID);
    pinMode(pCONT_pins->GetPin(GPIO_DOOR_OPEN_ID), INPUT_PULLUP);
    settings.fEnableSensor = true;
    settings.fSensorCount = 1;
  }else{
    AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin Invalid %d"),pCONT_pins->GetPin(GPIO_DOOR_OPEN_ID));
    //disable pir code
  }


  if(pCONT_pins->PinUsed(GPIO_DOOR_LOCK_ID)) // phase out in favour of basic switch? if so, doorsensor can become similar to motion that is non-resetting
  {
    pinMode(pCONT_pins->GetPin(GPIO_DOOR_LOCK_ID), INPUT_PULLUP);
    settings.fEnableSensor = true;
    settings.fSensorCount = 1;
  }else{
    AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin Invalid %d"),pCONT_pins->GetPin(GPIO_DOOR_LOCK_ID));
    //disable pir code
  }

}

/**
 * @brief "LOW" is closed, HIGH is open
 * 
 * @return uint8_t 
 */
uint8_t mDoorSensor::IsDoorOpen(){
  return (digitalRead(pCONT_pins->GetPin(GPIO_DOOR_OPEN_ID))==HIGH);
}
uint8_t mDoorSensor::IsLock_Locked(){
  return (digitalRead(pCONT_pins->GetPin(GPIO_DOOR_LOCK_ID))==LOW);
}


void mDoorSensor::init(void){

  door_detect.state = IsDoorOpen();
  lock_detect.state = IsLock_Locked();

}



void mDoorSensor::EveryLoop(){

  /**
   * @brief Reed Switch Door Position
   **/
  if((IsDoorOpen()!=door_detect.state)&&mTime::TimeReachedNonReset(&door_detect.tDetectTimeforDebounce,100)){

    AddLog(LOG_LEVEL_TEST, PSTR("IsDoorOpen()"));

    door_detect.state = IsDoorOpen();
    door_detect.tDetectTimeforDebounce = millis();
    if(door_detect.state){ 
      door_detect.isactive = true;
      door_detect.detected_time = pCONT_time->GetTimeShortNow();
      pCONT_rules->NewEventRun( GetModuleUniqueID(), FUNC_EVENT_MOTION_STARTED_ID, 0, door_detect.isactive);
    }else{ 
      door_detect.isactive = false;
      pCONT_rules->NewEventRun( GetModuleUniqueID(), FUNC_EVENT_MOTION_ENDED_ID, 0, door_detect.isactive);
    }
    door_detect.ischanged = true;
    mqtthandler_sensor_ifchanged.flags.SendNow = true;
    mqtthandler_sensor_teleperiod.flags.SendNow = true;

  }

  /**
   * @brief Door lock  
   **/
  if((IsLock_Locked()!=lock_detect.state)&&mTime::TimeReachedNonReset(&lock_detect.tDetectTimeforDebounce,100)){

    AddLog(LOG_LEVEL_TEST, PSTR("IsLock_Locked()"));

    lock_detect.state = IsLock_Locked();
    lock_detect.tDetectTimeforDebounce = millis();
    if(lock_detect.state){ 
      lock_detect.isactive = true;
      lock_detect.detected_time = pCONT_time->GetTimeShortNow();
      pCONT_rules->NewEventRun( GetModuleUniqueID(), FUNC_EVENT_MOTION_STARTED_ID, 1, lock_detect.isactive);
    }else{ 
      lock_detect.isactive = false;
      pCONT_rules->NewEventRun( GetModuleUniqueID(), FUNC_EVENT_MOTION_ENDED_ID, 1, lock_detect.isactive);
    }
    lock_detect.ischanged = true;
    mqtthandler_sensor_ifchanged.flags.SendNow = true;
    mqtthandler_sensor_teleperiod.flags.SendNow = true;

  }



}


const char* mDoorSensor::IsDoorOpen_Ctr(char* buffer, uint8_t buflen){
  if(door_detect.isactive){
    snprintf_P(buffer, buflen, PM_EVENT_DOOR_OPENED_CTR, sizeof(PM_EVENT_DOOR_OPENED_CTR));
  }else{
    snprintf_P(buffer, buflen, PM_EVENT_DOOR_CLOSED_CTR, sizeof(PM_EVENT_DOOR_CLOSED_CTR));
  }
  return buffer;
}


void mDoorSensor::ShowSensor_AddLog()
{
  
  ConstructJSON_Sensor(JSON_LEVEL_SHORT);
  ALOG_INF(PSTR(D_LOG_BME "\"%s\""),JBI->GetBufferPtr());

}




uint8_t mDoorSensor::ConstructJSON_Settings(uint8_t json_level, bool json_object_start_end_required){

  JsonBuilderI->Start();
    //JsonBuilderI->Add_P(PM_JSON_SENSORCOUNT, settings.);
  return JsonBuilderI->End();

}


uint8_t mDoorSensor::ConstructJSON_Sensor(uint8_t json_level){
  
  char buffer[50];

  JsonBuilderI->Start();
  JsonBuilderI->Add(D_JSON_LOCATION, DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(),0,buffer,sizeof(buffer)));
  JsonBuilderI->Add("Position", IsDoorOpen_Ctr(buffer, sizeof(buffer))); // give telemetry update of position
  
  if(json_level >= JSON_LEVEL_IFCHANGED){
    JsonBuilderI->Add(D_JSON_TIME, mTime::ConvertShortTimetoCtr(&door_detect.detected_time, buffer, sizeof(buffer)));
    JsonBuilderI->Add(D_JSON_EVENT, IsDoorOpen_Ctr(buffer, sizeof(buffer)));
  }

  JBI->Add("DoorOpenPin", digitalRead(pCONT_pins->GetPin(GPIO_DOOR_OPEN_ID)));

    
  JBI->Add("IsDoorOpen", IsDoorOpen());


  if(pCONT_pins->PinUsed(GPIO_DOOR_LOCK_ID)) // phase out in favour of basic switch? if so, doorsensor can become similar to motion that is non-resetting
  {
    JBI->Add("DoorLockPin", digitalRead(pCONT_pins->GetPin(GPIO_DOOR_LOCK_ID)));
    JBI->Add("IsLock_Locked", IsLock_Locked());
  }

  return JsonBuilderI->End();

}

#endif


