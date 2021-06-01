#include "mDoorSensor.h"

#ifdef USE_MODULE_SENSORS_DOOR



const char* mDoorSensor::PM_MODULE_SENSORS_DOOR_CTR = D_MODULE_SENSORS_DOOR_CTR;
const char* mDoorSensor::PM_MODULE_SENSORS_DOOR_FRIENDLY_CTR = D_MODULE_SENSORS_DOOR_FRIENDLY_CTR;



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

  // if(pCONT_pins->GetPin(GPIO_DOOR_LOCK_ID] < 99) {  // not set when 255
  //   pin_lock = pCONT_pins->GetPin(GPIO_DOOR_LOCK_ID];
  //   pinMode(pin_lock,INPUT_PULLUP);
  // }else{
  //   AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin Invalid %d"),pin_lock);
  //   //disable pir code
  // }

  // pin_open = D6;
  // pinMode(pin_open,INPUT_PULLUP);

}


uint8_t mDoorSensor::IsDoorOpen(){
  return digitalRead(pCONT_pins->GetPin(GPIO_DOOR_OPEN_ID));
}


void mDoorSensor::init(void){

  door_detect.state = IsDoorOpen();

}


int8_t mDoorSensor::Tasker(uint8_t function, JsonParserObject obj){

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

void mDoorSensor::EveryLoop(){

  if((IsDoorOpen()!=door_detect.state)&&mTime::TimeReachedNonReset(&door_detect.tDetectTimeforDebounce,100)){
    door_detect.state = IsDoorOpen();
    door_detect.tDetectTimeforDebounce = millis();
    if(door_detect.state){ 
      door_detect.isactive = true;
      door_detect.detected_time = pCONT_time->GetTimeShortNow();
      mqtthandler_sensor_ifchanged.flags.SendNow = true;
    }else{ 
      door_detect.isactive = false;
    }
    door_detect.ischanged = true;
    mqtthandler_sensor_ifchanged.flags.SendNow = true;
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

      sprintf(inner_html,"%s %s",IsDoorOpen_Ctr(door_pos_ctr,sizeof(door_pos_ctr)),
        mTime::ConvertShortTime_HHMMSS(&door_detect.detected_time, time_ctr, sizeof(time_ctr)));
    
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
    JsonBuilderI->Add(D_JSON_TIME, mTime::ConvertShortTime_HHMMSS(&door_detect.detected_time, buffer, sizeof(buffer)));
    JsonBuilderI->Add(D_JSON_EVENT, IsDoorOpen_Ctr(buffer, sizeof(buffer)));
  }

  return JsonBuilderI->End();

}


/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

void mDoorSensor::MQTTHandler_Init(){

  struct handler<mDoorSensor>* mqtthandler_ptr;

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = SEC_IN_HOUR; 
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
  mqtthandler_ptr->flags.SendNow = false;
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

  uint8_t mqtthandler_list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID, MQTT_HANDLER_SENSOR_IFCHANGED_ID, MQTT_HANDLER_SENSOR_TELEPERIOD_ID
  };
  
  struct handler<mDoorSensor>* mqtthandler_list_ptr[] = {
    &mqtthandler_settings_teleperiod, &mqtthandler_sensor_ifchanged, &mqtthandler_sensor_teleperiod
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, EM_MODULE_SENSORS_DOOR_ID,
    mqtthandler_list_ptr, mqtthandler_list_ids,
    sizeof(mqtthandler_list_ptr)/sizeof(mqtthandler_list_ptr[0]),
    mqtt_handler_id
  );

}

#endif


