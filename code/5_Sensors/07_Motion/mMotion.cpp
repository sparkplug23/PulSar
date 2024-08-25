#include "mMotion.h" 

/***
 * New joint motion triggered class, all future motion events will also trigger a response from this class (no rules required)
 * */

#ifdef USE_MODULE_SENSORS_MOTION 

int8_t mMotion::Tasker(uint8_t function, JsonParserObject obj){
  
  int8_t function_result = 0;
  
  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
    case FUNC_INIT:
      Init();
    break;
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    // case FUNC_EVERY_SECOND:{

    //   // char buffer[100] = {0};

    //   // uint32_t detected_time = pCONT_time->GetTimeShortNowU32();

    //   //   mTime::ConvertU32TimetoCtr(&detected_time, buffer, sizeof(buffer));

    //   //   ALOG_INF( PSTR("buffer=\"%s\""), buffer );

    // }break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * RULES SECTION * 
    *******************/
    #ifdef USE_MODULE_CORE_RULES
    case FUNC_RULES_ADD_DEFAULT_RULES_USING_GPIO_FUNCTIONS_ID:
      Rules_Add_Rule();
    break;
    case FUNC_EVENT_MOTION_STARTED_ID:
    case FUNC_EVENT_MOTION_ENDED_ID:
      RulesEvent_Motion_Change(); //not called here?
    break;
    #endif// USE_MODULE_CORE_RULES
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Set_DefaultPeriodRate();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }
  
  return function_result;

} // END function


void mMotion::Pre_Init(void)
{

}


void mMotion::Init(void)
{
  settings.fEnableSensor = true;

  settings.sensors_active = MAXIMUM_SENSORS;

  // #ifdef USE_DEVFEATURE_FORCE_SWITCH_TO_FOLLOW_FOR_SWITCH
  AddLog(LOG_LEVEL_WARNING, PSTR("USE_DEVFEATURE_FORCE_SWITCH_TO_FOLLOW_FOR_SWITCH"));
  // default is toggle, follow must be used here for motion 
  for (uint8_t i = 0; i < MAX_SWITCHES; i++) { pCONT_set->Settings.switchmode[i] = SWITCHMODE_FOLLOW_ID; }
  // #endif // USE_DEVFEATURE_FORCE_SWITCH_TO_FOLLOW_FOR_SWITCH


}


void mMotion::Rules_Add_Rule()
{

// Get current rule, increment




}



/**
 * @brief MOTION Events, will replace all motion type events.
 * The motion (PIR), ultrasonic change to trigger motion, will be controlled via the rule engine
 * Thus, any state change (or later mqtt topic input or url etc) can be treated internally as a motion change, no longer a pin state
 * 
 * @note Information about trigger, will be contained within the stored event
 * @param DeviceName gives the index to the stored location index from sensor list
 * @param State gives the ON/Started (1) or OFF/Ended (0)
 * */
void mMotion::RulesEvent_Motion_Change()
{

  ALOG_INF(PSTR("RulesEvent_Motion_Change"));


  // for(
/**
 * Rhis "rules" should be changed, unless rules are to be on all the time
 * */


    uint8_t sensor_id = pCONT_rules->rules[pCONT_rules->rules_active_index].command.device_id;

    /**
     * If command state is follow (now value 2), then use trigger.data[0] as destination.data[0]
     * */
    uint8_t trigger_state = pCONT_rules->rules[pCONT_rules->rules_active_index].trigger.value.data[0];
    uint8_t command_state_in = pCONT_rules->rules[pCONT_rules->rules_active_index].command.value.data[0];
    uint8_t newevent_command_state_in = pCONT_rules->event_triggered.value.data[0];
    uint8_t command_state_out = 0;

    uint8_t current_module_id = pCONT_rules->event_triggered.module_id;

// enum SwitchModeOptions_IDS {
//   TOGGLE, 
//   FOLLOW, 
//   FOLLOW_INV, 
//   PUSHBUTTON, 
//   PUSHBUTTON_INV, 
//   PUSHBUTTONHOLD, 
//   PUSHBUTTONHOLD_INV, 
//   PUSHBUTTON_TOGGLE, 
//   MAX_SWITCH_OPTION
// };

// pCONT_rules->ShowRuleAddLogByIndex();
// pCONT_rules->ShowRuleEvent_AddLog();

//     if(command_state_in == SWITCHMODE_FOLLOW_ID)
//     {
//       AddLog(LOG_LEVEL_TEST, PSTR("SWITCHMODE_FOLLOW_ID"));
//     }
// else{
//       AddLog(LOG_LEVEL_TEST, PSTR("ELSE SWITCHMODE_FOLLOW_ID"));

// }

// AddLog(LOG_LEVEL_TEST, PSTR("trigger_state=%d"),trigger_state);
// AddLog(LOG_LEVEL_TEST, PSTR("command_state_in=%d"),command_state_in);
// AddLog(LOG_LEVEL_TEST, PSTR("newevent_command_state_in=%d"),newevent_command_state_in);
// AddLog(LOG_LEVEL_TEST, PSTR("command_state_out=%d"), command_state_out);

// ALOG_INF( PSTR("\t\t\t\t\t current_module_id=%d"), current_module_id );


switch(command_state_in)
{
  default: //force off
    command_state_out = 0;
    break;
  case STATE_NUMBER_FOLLOW_ID: 
    command_state_out = newevent_command_state_in;
    ALOG_INF( PSTR("STATE_NUMBER_FOLLOW_ID command_state_out = %d"), command_state_out );
    break;
  case STATE_NUMBER_FOLLOW_INV_ID: 
    command_state_out = newevent_command_state_in?0:1;
    break;
  case STATE_NUMBER_OFF_ID: 
    command_state_out = 0;
    break;
  case STATE_NUMBER_ON_ID: 
    command_state_out = 1;
    break;
}


// AddLog(LOG_LEVEL_TEST, PSTR("Bommand_state_out=%d"), command_state_out);

char buffer[100];

// AddLog(LOG_LEVEL_TEST, PSTR("state=[%d->%d]\"%s\""), newevent_command_state_in, command_state_out, pCONT_sup->GetState_Name_by_ID(command_state_out, buffer, sizeof(buffer)));


    // sensor_id<settings.sensors_active;sensor_id++)
  // {

  //   if(PIR_Detected(sensor_id)!=pir_detect[sensor_id].state)
  //   {

// 1 ie HIGH will ALWAYS mean active, the inversion should be handled on the trigger/switch side

      pir_detect[sensor_id].state = command_state_out;//pCONT_rules->rules[pCONT_rules->rules_active_index].command.value.data[0];//PIR_Detected(sensor_id);
      
      
      AddLog(LOG_LEVEL_DEBUG,PSTR("pir_detect[sensor_id].state=%d %d %d %d %d"),
      pir_detect[sensor_id].state, trigger_state, command_state_in, command_state_out, newevent_command_state_in);


      if(pir_detect[sensor_id].state)
      {
        pir_detect[sensor_id].tDetectTime = millis(); 
        pir_detect[sensor_id].detected_time = pCONT_time->LocalTime();
        pir_detect[sensor_id].isactive = true;

        // #ifdef ENABLE_LOG_LEVEL_DEBUG
        // AddLog(LOG_LEVEL_DEBUG,PSTR("pir_detect[sensor_id].state=%d"),pir_detect[sensor_id].state);
        // #endif
        // AddLog(LOG_LEVEL_DEBUG,PSTR("pir_detect[sensor_id].detected_time=%d"),pir_detect[sensor_id].detected_time);
        
        // #ifdef USE_MODULE_CORE_RULES
        // pCONT_rules->New_Event(E M_MODULE_SENSORS_MOTION_ID, sensor_id);
        // #endif
        // pCONT->Tasker_Interface(FUNC_EVENT_MOTION_STARTED_ID);

      }
      else
      {
        pir_detect[sensor_id].tEndedTime = millis();
        pir_detect[sensor_id].detected_time = pCONT_time->LocalTime();
        pir_detect[sensor_id].isactive = false;

        // #ifdef USE_MODULE_CORE_RULES
        // pCONT_rules->New_Event(E M_MODULE_SENSORS_MOTION_ID, sensor_id);
        // #endif
        // pCONT->Tasker_Interface(FUNC_EVENT_MOTION_ENDED_ID);

      }

/**
 * @brief Remember what triggered this so the device name can be retrieved in sender
 **/
      pir_detect[sensor_id].device_name.unique_module_id = pCONT_rules->event_triggered.module_id;
      pir_detect[sensor_id].device_name.device_id = pCONT_rules->event_triggered.device_id;
      pir_detect[sensor_id].ischanged = true;
      mqtthandler_sensor_ifchanged.flags.SendNow = true;
  //   }
  // }

  AddLog(LOG_LEVEL_TEST, PSTR(DEBUG_INSERT_PAGE_BREAK "MOTION Event %d"),sensor_id);

}






/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

  

void mMotion::parse_JSONCommand(JsonParserObject obj)
{



}

/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mMotion::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_JSON_CHANNELCOUNT, 0);
  return JBI->End();

}


uint8_t mMotion::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  ALOG_INF( PSTR("ConstructJSON_Sensor %d"), millis() );

  char buffer[80];

  JBI->Start();
    // JBI->Add("motion", 0);

    for(
      uint8_t sensor_id = 0;//pCONT_rules->rules[pCONT_rules->rules_active_index].command.device_id;
      //
      sensor_id<settings.sensors_active;sensor_id++)
    {

    if(pir_detect[sensor_id].ischanged){ 
      
      pir_detect[sensor_id].ischanged = false;
      
      // #if defined(USE_DEVFEATURE_MOTION_EVENT_WITH_RULE_IDS_FOR_DOORSENSOR) || defined(USE_DEVFEATURE_MOTION_EVENT_USES_MODULE_ID_FOR_DEVICENAME)

      uint16_t device_name_module_id = pir_detect[sensor_id].device_name.unique_module_id;
      uint8_t device_name_device_id = pir_detect[sensor_id].device_name.device_id; // this needs to be constrained/checked for when rules are used for SWITCH==>MOTION linkage
      // uint8_t pir_detect_isactive  = pCONT_rules->rules[pCONT_rules->rules_active_index].command.value.data[0];
      /**
       * @brief Need to convert from unique_module_id to e m_module_id
       * 
       */
      // uint16_t e m_module_id = pCONT->GetEnumNumber_UsingModuleUniqueID(device_name_module_id);
      
      // JBI->Add(D_JSON_LOCATION, DLI->GetDeviceNameWithEnumNumber(e m_module_id, device_name_device_id, buffer, sizeof(buffer)));

      JBI->Add(D_JSON_LOCATION, DLI->GetDeviceName_WithModuleUniqueID( device_name_module_id, device_name_device_id, buffer, sizeof(buffer))); 
      // JBI->Add(D_JSON_TIME, mTime::ConvertU32TimetoCtr(&pir_detect[sensor_id].detected_time, buffer, sizeof(buffer)));
      // JBI->Add("Week" D_JSON_TIME, mTime::ConvertU32TimetoCtr(&pir_detect[sensor_id].detected_time, buffer, sizeof(buffer), true));


      JBI->Add("Time", pCONT_time->GetTimeStr(pCONT_time->Rtc.local_time).c_str());
      JBI->Add("UTCTime", pCONT_time->Rtc.local_time);


      JBI->Add(D_JSON_EVENT, pir_detect[sensor_id].isactive ? "detected": "over");

      #ifdef ENABLE_DEVFEATURE_REPORT_MOTION_TIMES_WITH_EPOCH_AND_ISO8601_STANDARD_TIME
      JBI->Object_Start("TriggerTime");
        JBI->Add("Epoch",0);
        JBI->Add("ISO8601","2022-07-24T15:18:25+00:00"); //16 with +01:00 for DST?
      JBI->End();
      #endif // ENABLE_DEVFEATURE_REPORT_MOTION_TIMES_WITH_EPOCH_AND_ISO8601_STANDARD_TIME



// https://www.jamesridgway.co.uk/why-storing-datetimes-as-utc-isnt-enough/
//       As well as representing a datetime:

// 2021-01-24T07:28:37-05:00
// 2021-01-24T12:28:37Z



// Where TimeNow may be different types selected by user, but standard is ISO8601



      JBI->Add("Index", sensor_id);

  //     //if another is yet to send, then reset the mqtt_handler to fire immeditely again!
  //     //if any mtion flag remains, then set mqtt again
      
    }
  }

  // ALOG_INF(PSTR("JBI MOTION %s"), JBI->GetBufferPtr());


  return JBI->End();
    
}
  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mMotion::MQTTHandler_Init(){

  struct handler<mMotion>* ptr;

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mMotion::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mMotion::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mMotion::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mMotion::MQTTHandler_Set_DefaultPeriodRate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
  }
}

/**
 * @brief MQTTHandler_Sender
 * */
void mMotion::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_SENSORS_MOTION_ID, handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT
/******************************************************************************************************************
 * WebServer
*******************************************************************************************************************/

// #ifndef DISABLE_WEBSERVER
// void mMotionSensor::WebAppend_Root_Draw_PageTable(){
// char buffer[50];
// for(uint8_t sensor_id=0;sensor_id<settings.sensors_active;sensor_id++){       
//   BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
//     BufferWriterI->Append_P(PSTR("<td>PIR Motion %s</td>"), DLI->GetDeviceNameWithEnumNumber(D_MODULE_SENSORS_MOTION_ID, sensor_id, buffer, sizeof(buffer)));
//     BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_CLASS_TYPE_2V,"tab_pir","?");   
//   BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
// }
// }


// void mMotionSensor::WebPage_Root_AddHandlers(){
//   // Not needed
// }

// //append to internal buffer if any root messages table
// void mMotionSensor::WebAppend_Root_Status_Table(){

//   char buffer[20];

//   JBI->Array_Start("tab_pir");// Class name
//   for(int sensor_id=0;sensor_id<settings.sensors_active;sensor_id++){
//     JBI->Object_Start();
//       JBI->Add("id",sensor_id);

//         char colour_ctr[8];
//         uint32_t millis_elapsed = mTime::MillisElapsed(&pir_detect[sensor_id].tEndedTime);
//         // Motion in progress
//         if(pir_detect[sensor_id].isactive){
//           sprintf_P(colour_ctr,PSTR("#00ff00"));
//         }else
//         // If movement event has just finished
//         if(millis_elapsed<(1000*60)){
//           // Show colour as fading back to white over X seconds SINCE EVENT OVER
//           uint8_t colour_G = constrain(
//                                 map(millis_elapsed,0,(1000*60),0,255)
//                                 ,0,255 //increases with time
//                               );
//                               //Serial.printf("colour_G=%d\n\r",colour_G);
//           // sprintf(colour_ctr,"%s",
//           pCONT_web->WebColorCtr(255,colour_G,colour_G, colour_ctr, sizeof(colour_ctr));
//           //);
//         }
//         // no event show, just white
//         else{
//           sprintf_P(colour_ctr,PSTR("#ffffff"));
//         }


//       JBI->Add("ih",
//         pCONT_time->ConvertShortTime_HHMMSS(&pir_detect[sensor_id].detected_time, buffer, sizeof(buffer)));
      
//       //detected_rtc_ctr);
//       JBI->Add("fc",colour_ctr);    
//     JBI->Object_End();
//   }

//   JBI->Array_End();

// }
// #endif // DISABLE_WEBSERVER



#endif
