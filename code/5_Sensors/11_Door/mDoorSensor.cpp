#include "mDoorSensor.h"

#ifdef USE_MODULE_SENSORS_DOOR

// Should this be a controller? Since the underlying input is from switches sensor. This would also mean motion should be controller.
// But then how does motion + others become an "event"
// Event should be anything really, not just switch ie power level threshold crossed


int8_t mDoorSensor::Tasker(uint8_t function, JsonParserObject obj)
{
  
  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      init();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return TASKER_RESULT__MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP: 
      EveryLoop();
    break;
    case TASK_EVERY_SECOND:

      // if(tkr_pins->PinUsed(GPIO_DOOR_LOCK_ID)) // phase out in favour of basic switch? if so, doorsensor can become similar to motion that is non-resetting
      // {
      //   ALOG_TST(PSTR("DoorLockPin=%d"), digitalRead(tkr_pins->GetPin(GPIO_DOOR_LOCK_ID)));
      // }

    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_STATUS_REFRESH_SEND_ALL:
      tkr_mqtt->MQTTHandler_RefreshAll(mqtthandler_list);
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      tkr_mqtt->MQTTHandler_Rate(mqtthandler_list);
    break;
    case TASK_MQTT_SENDER:
      tkr_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }

} // END function


void mDoorSensor::Pre_Init(void)
{
  module_state.mode = ModuleStatus::Initialising;
  module_state.devices = 0;
  
}

void mDoorSensor::Init(void)
{
  
  if (tkr_pins->PinUsed(GPIO_DOOR_OPEN))
  {
    pinMode(tkr_pins->GetPin(GPIO_DOOR_OPEN), INPUT_PULLUP);
    module_state.devices++;
  }
  else
  {
    AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PIR "Pin Invalid %d"), tkr_pins->GetPin(GPIO_DOOR_OPEN));
  }

  if (tkr_pins->PinUsed(GPIO_DOOR_LOCK))
  {
    pinMode(tkr_pins->GetPin(GPIO_DOOR_LOCK), INPUT_PULLUP);
    module_state.devices++;
  }
  else
  {
    AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PIR "Pin Invalid %d"), tkr_pins->GetPin(GPIO_DOOR_LOCK));
  }

  if(module_state.devices)
  {
    module_state.mode = ModuleStatus::Running;
  }

  door_detect.state = IsDoorOpen();
  lock_detect.state = IsLock_Locked();

}




/**
 * @brief "LOW" is closed, HIGH is open
 * 
 * @return uint8_t 
 */
uint8_t mDoorSensor::IsDoorOpen(){
  return (digitalRead(tkr_pins->GetPin(GPIO_DOOR_OPEN))==HIGH);
}
uint8_t mDoorSensor::IsLock_Locked(){
  return (digitalRead(tkr_pins->GetPin(GPIO_DOOR_LOCK))==LOW);
}



void mDoorSensor::EveryLoop()
{
  /**
   * @brief Reed Switch Door Position
   **/
  if ((IsDoorOpen() != door_detect.state) && mTime::TimeReachedNonReset(&door_detect.tDetectTimeforDebounce, 100))
  {
    ALOG_TST(PSTR("IsDoorOpen()"));

    door_detect.state = IsDoorOpen();
    door_detect.tDetectTimeforDebounce = millis();
    door_detect.ischanged = true;

    if (door_detect.state)
    {
      door_detect.isactive = true;
      door_detect.tDetectTime = millis();
      door_detect.detected_time = tkr_time->LocalTime();

      tkr_rules->NewEventRun(GetModuleUniqueID(), TASK_EVENT_MOTION_STARTED_ID, 0, door_detect.isactive);
    }
    else
    {
      door_detect.isactive = false;
      door_detect.tEndedTime = millis();
      door_detect.detected_time = tkr_time->LocalTime();

      tkr_rules->NewEventRun(GetModuleUniqueID(), TASK_EVENT_MOTION_ENDED_ID, 0, door_detect.isactive);
    }

    mqtthandler_sensor_ifchanged.flags.SendNow = true;
  }
  else
  {
    door_detect.ischanged = false;
  }

  /**
   * @brief Door lock
   **/
  if (tkr_pins->PinUsed(GPIO_DOOR_LOCK) &&
      (IsLock_Locked() != lock_detect.state) &&
      mTime::TimeReachedNonReset(&lock_detect.tDetectTimeforDebounce, 100))
  {
    ALOG_TST(PSTR("IsLock_Locked()"));

    lock_detect.state = IsLock_Locked();
    lock_detect.tDetectTimeforDebounce = millis();
    lock_detect.ischanged = true;

    if (lock_detect.state)
    {
      lock_detect.isactive = true;
      lock_detect.detected_time = tkr_time->LocalTime();

      tkr_rules->NewEventRun(GetModuleUniqueID(), TASK_EVENT_MOTION_STARTED_ID, 1, lock_detect.isactive);
    }
    else
    {
      lock_detect.isactive = false;
      lock_detect.detected_time = tkr_time->LocalTime();

      tkr_rules->NewEventRun(GetModuleUniqueID(), TASK_EVENT_MOTION_ENDED_ID, 1, lock_detect.isactive);
    }

    mqtthandler_sensor_ifchanged.flags.SendNow = true;
  }
  else
  {
    lock_detect.ischanged = false;
  }
}


const char* mDoorSensor::IsDoorOpen_Ctr(char* buffer, uint8_t buflen)
{
  if(door_detect.isactive){
    snprintf_P(buffer, buflen, PM_EVENT_DOOR_OPENED_CTR, sizeof(PM_EVENT_DOOR_OPENED_CTR));
  }else{
    snprintf_P(buffer, buflen, PM_EVENT_DOOR_CLOSED_CTR, sizeof(PM_EVENT_DOOR_CLOSED_CTR));
  }
  return buffer;
}

  
/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/


uint8_t mDoorSensor::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_DEVICES, module_state.devices);
  return JBI->End();

}


uint8_t mDoorSensor::ConstructJSON_Sensor(uint8_t json_level, bool json_appending)
{
  char buffer[80];

  JBI->Start();

  JBI->Add(D_LOCATION, DLI->GetDeviceName_WithModuleUniqueID(GetModuleUniqueID(), 0, buffer, sizeof(buffer)));
  JBI->Add("Position", IsDoorOpen_Ctr(buffer, sizeof(buffer)));

  if (json_level >= JSON_LEVEL_IFCHANGED)
  {
    JBI->Add(D_TIME, tkr_time->GetTimeStr(door_detect.detected_time).c_str());
    JBI->Add(D_UTC_TIME, door_detect.detected_time);
    JBI->Add(D_EVENT, IsDoorOpen_Ctr(buffer, sizeof(buffer)));
  }

  JBI->Add("DoorOpenPin", digitalRead(tkr_pins->GetPin(GPIO_DOOR_OPEN)));
  JBI->Add("IsDoorOpen", IsDoorOpen());

  if (tkr_pins->PinUsed(GPIO_DOOR_LOCK))
  {
    JBI->Add("DoorLockPin", digitalRead(tkr_pins->GetPin(GPIO_DOOR_LOCK)));
    JBI->Add("IsLock_Locked", IsLock_Locked());

    if (json_level >= JSON_LEVEL_IFCHANGED)
    {
      JBI->Add("LockTime", tkr_time->GetTimeStr(lock_detect.detected_time).c_str());
      JBI->Add("LockTimeUtc", lock_detect.detected_time);
    }
  }

  return JBI->End();
}

  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mDoorSensor::MQTTHandler_Init(){

  struct handler<mDoorSensor>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mDoorSensor::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mDoorSensor::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);
  
} 

#endif // USE_MODULE_NETWORK_MQTT


#endif


