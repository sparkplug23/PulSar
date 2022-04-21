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

      if(pCONT_pins->PinUsed(GPIO_DOOR_LOCK_ID)) // phase out in favour of basic switch? if so, doorsensor can become similar to motion that is non-resetting
      {
        AddLog(LOG_LEVEL_TEST, PSTR("DoorLockPin=%d"), digitalRead(pCONT_pins->GetPin(GPIO_DOOR_LOCK_ID)));
      }

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
  }else{
    AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin Invalid %d"),pCONT_pins->GetPin(GPIO_DOOR_OPEN_ID));
    //disable pir code
  }


  if(pCONT_pins->PinUsed(GPIO_DOOR_LOCK_ID)) // phase out in favour of basic switch? if so, doorsensor can become similar to motion that is non-resetting
  {
    pinMode(pCONT_pins->GetPin(GPIO_DOOR_LOCK_ID), INPUT_PULLUP);
  }else{
    AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin Invalid %d"),pCONT_pins->GetPin(GPIO_DOOR_LOCK_ID));
    //disable pir code
  }

}


uint8_t mDoorSensor::IsDoorOpen(){
  return digitalRead(pCONT_pins->GetPin(GPIO_DOOR_OPEN_ID));
}


void mDoorSensor::init(void){

  door_detect.state = IsDoorOpen();

}



void mDoorSensor::EveryLoop(){

  if((IsDoorOpen()!=door_detect.state)&&mTime::TimeReachedNonReset(&door_detect.tDetectTimeforDebounce,100)){

    AddLog(LOG_LEVEL_TEST, PSTR("IsDoorOpen()"));

    door_detect.state = IsDoorOpen();
    door_detect.tDetectTimeforDebounce = millis();
    if(door_detect.state){ 
      door_detect.isactive = true;
      door_detect.detected_time = pCONT_time->GetTimeShortNow();
    }else{ 
      door_detect.isactive = false;
    }
    door_detect.ischanged = true;
    mqtthandler_sensor_ifchanged.flags.SendNow = true;
    mqtthandler_sensor_teleperiod.flags.SendNow = true;

    pCONT_rules->NewEventRun(EM_MODULE_SENSORS_DOOR_ID, FUNC_EVENT_INPUT_STATE_CHANGED_ID, 0, door_detect.isactive);
   
  }

}

    #ifdef USE_MODULE_NETWORK_WEBSERVER
void mDoorSensor::WebAppend_Root_Status_Table_Draw(){

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
  char time_ctr[20];

  JsonBuilderI->Array_Start("tab_door");// Class name
  
  for(int sensor_id=0;sensor_id<1;sensor_id++){
    
    JsonBuilderI->Level_Start();
      JsonBuilderI->Add("id",sensor_id);

      char colour_ctr[8];
      uint32_t millis_elapsed = mTime::MillisElapsed(&door_detect.tEndedTime);
      // Motion in progress
      if(door_detect.isactive){
        sprintf_P(colour_ctr,PSTR("#00ff00"));
      }else
      // If movement event has just finished
      if(millis_elapsed<(1000*60)){
        // Show colour as fading back to white over X seconds SINCE EVENT OVER
        uint8_t colour_G = constrain(
                              map(millis_elapsed,0,(1000*60),0,255)
                              ,0,255 //increases with time
                            );
        pCONT_web->WebColorCtr(255,colour_G,colour_G, colour_ctr, sizeof(colour_ctr));
      }
      // no event show, just white
      else{
        sprintf(colour_ctr,"#ffffff");
      }

      // sprintf(inner_html,"%s %s",IsDoorOpen_Ctr(door_pos_ctr,sizeof(door_pos_ctr)),
      //   mTime::ConvertShortTime_HHMMSS(&door_detect.detected_time, time_ctr, sizeof(time_ctr)));
    
      JsonBuilderI->Add("ih",inner_html);
      JsonBuilderI->Add("fc",colour_ctr);
    
    JsonBuilderI->Level_End();
  }

  JsonBuilderI->Array_End();

}

#endif // USE_MODULE_NETWORK_WEBSERVER


const char* mDoorSensor::IsDoorOpen_Ctr(char* buffer, uint8_t buflen){
  if(door_detect.isactive){
    snprintf_P(buffer, buflen, PM_EVENT_DOOR_OPENED_CTR, sizeof(PM_EVENT_DOOR_OPENED_CTR));
  }else{
    snprintf_P(buffer, buflen, PM_EVENT_DOOR_CLOSED_CTR, sizeof(PM_EVENT_DOOR_CLOSED_CTR));
  }
  return buffer;
}


uint8_t mDoorSensor::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
    //JsonBuilderI->Add_P(PM_JSON_SENSORCOUNT, settings.);
  return JsonBuilderI->End();

}


uint8_t mDoorSensor::ConstructJSON_Sensor(uint8_t json_level){
  
  char buffer[50];

  JsonBuilderI->Start();
  JsonBuilderI->Add(D_JSON_LOCATION, DLI->GetDeviceNameWithEnumNumber(EM_MODULE_SENSORS_DOOR_ID,0,buffer,sizeof(buffer)));
  JsonBuilderI->Add("Position", IsDoorOpen_Ctr(buffer, sizeof(buffer))); // give telemetry update of position
  
  if(json_level >= JSON_LEVEL_IFCHANGED){
    JsonBuilderI->Add(D_JSON_TIME, mTime::ConvertShortTimetoCtr(&door_detect.detected_time, buffer, sizeof(buffer)));
    JsonBuilderI->Add(D_JSON_EVENT, IsDoorOpen_Ctr(buffer, sizeof(buffer)));
  }

  JBI->Add("DoorOpenPin", digitalRead(pCONT_pins->GetPin(GPIO_DOOR_OPEN_ID)));


  if(pCONT_pins->PinUsed(GPIO_DOOR_LOCK_ID)) // phase out in favour of basic switch? if so, doorsensor can become similar to motion that is non-resetting
  {
  JBI->Add("DoorLockPin", digitalRead(pCONT_pins->GetPin(GPIO_DOOR_LOCK_ID)));
  }
  return JsonBuilderI->End();

}

#endif


