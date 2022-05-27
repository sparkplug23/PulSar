#include "mMotion.h" 

/***
 * New joint motion triggered class, all future motion events will also trigger a response from this class (no rules required)
 * */

#ifdef USE_MODULE_SENSORS_MOTION 

const char* mMotion::PM_MODULE_SENSORS_MOTION_CTR = D_MODULE_SENSORS_MOTION_CTR;
const char* mMotion::PM_MODULE_SENSORS_MOTION_FRIENDLY_CTR = D_MODULE_SENSORS_MOTION_FRIENDLY_CTR;

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
    case FUNC_MQTT_HANDLERS_RESET:
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_TelePeriod();
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
  AddLog(LOG_LEVEL_WARN, PSTR("USE_DEVFEATURE_FORCE_SWITCH_TO_FOLLOW_FOR_SWITCH"));
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
void mMotion::RulesEvent_Motion_Change(){


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

pCONT_rules->ShowRuleAddLogByIndex();
pCONT_rules->ShowRuleEvent_AddLog();

//     if(command_state_in == SWITCHMODE_FOLLOW_ID)
//     {
//       AddLog(LOG_LEVEL_TEST, PSTR("SWITCHMODE_FOLLOW_ID"));
//     }
// else{
//       AddLog(LOG_LEVEL_TEST, PSTR("ELSE SWITCHMODE_FOLLOW_ID"));

// }

AddLog(LOG_LEVEL_TEST, PSTR("trigger_state=%d"),trigger_state);
AddLog(LOG_LEVEL_TEST, PSTR("command_state_in=%d"),command_state_in);
AddLog(LOG_LEVEL_TEST, PSTR("newevent_command_state_in=%d"),newevent_command_state_in);
AddLog(LOG_LEVEL_TEST, PSTR("command_state_out=%d"), command_state_out);

ALOG_INF( PSTR("\t\t\t\t\t current_module_id=%d"), current_module_id );


switch(command_state_in)
{
  default: //force off
    command_state_out = 0;
    break;
  case STATE_NUMBER_FOLLOW_ID: 
    command_state_out = newevent_command_state_in;
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


AddLog(LOG_LEVEL_TEST, PSTR("Bommand_state_out=%d"), command_state_out);

char buffer[100];

AddLog(LOG_LEVEL_TEST, PSTR("state=[%d->%d]\"%s\""), newevent_command_state_in, command_state_out, pCONT_sup->GetState_Name_by_ID(command_state_out, buffer, sizeof(buffer)));


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
        pir_detect[sensor_id].detected_time = pCONT_time->GetTimeShortNowU32();
        pir_detect[sensor_id].isactive = true;

        #ifdef ENABLE_LOG_LEVEL_DEBUG
        AddLog(LOG_LEVEL_DEBUG,PSTR("pir_detect[sensor_id].state=%d"),pir_detect[sensor_id].state);
        #endif
        AddLog(LOG_LEVEL_DEBUG,PSTR("pir_detect[sensor_id].detected_time=%d"),pir_detect[sensor_id].detected_time);
        
        // #ifdef USE_MODULE_CORE_RULES
        // pCONT_rules->New_Event(EM_MODULE_SENSORS_MOTION_ID, sensor_id);
        // #endif
        // pCONT->Tasker_Interface(FUNC_EVENT_MOTION_STARTED_ID);

      }
      else
      {
        pir_detect[sensor_id].tEndedTime = millis();
        pir_detect[sensor_id].isactive = false;

        // #ifdef USE_MODULE_CORE_RULES
        // pCONT_rules->New_Event(EM_MODULE_SENSORS_MOTION_ID, sensor_id);
        // #endif
        // pCONT->Tasker_Interface(FUNC_EVENT_MOTION_ENDED_ID);

      }

/**
 * @brief Remember what triggered this so the device name can be retrieved in sender
 **/
      pir_detect[sensor_id].device_name.module_id = pCONT_rules->event_triggered.module_id;
      pir_detect[sensor_id].device_name.device_id = pCONT_rules->event_triggered.device_id;


      pir_detect[sensor_id].ischanged = true;
      mqtthandler_sensor_ifchanged.flags.SendNow = true;
  //   }
  // }

  // AddLog(LOG_LEVEL_TEST, PSTR(DEBUG_INSERT_PAGE_BREAK "MOTION Event %d"),sensor_id);

}



uint8_t mMotion::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_CHANNELCOUNT, 0);
  return JsonBuilderI->End();

}


uint8_t mMotion::ConstructJSON_Sensor(uint8_t json_method){

  char buffer[80];

  JsonBuilderI->Start();
    // JsonBuilderI->Add("motion", 0);

    for(
      uint8_t sensor_id = 0;//pCONT_rules->rules[pCONT_rules->rules_active_index].command.device_id;
      // uint8_t pir_detect_isactive  = pCONT_rules->rules[pCONT_rules->rules_active_index].command.value.data[0];
      sensor_id<settings.sensors_active;sensor_id++)
    {

    if(pir_detect[sensor_id].ischanged){ 
      
      pir_detect[sensor_id].ischanged = false;
      
      #ifdef USE_DEVFEATURE_MOTION_EVENT_WITH_RULE_IDS_FOR_DOORSENSOR

      uint16_t device_name_module_id = pir_detect[sensor_id].device_name.module_id;
      uint8_t device_name_device_id = pir_detect[sensor_id].device_name.device_id; // this needs to be constrained/checked for when rules are used for SWITCH==>MOTION linkage
      
      JsonBuilderI->Add(D_JSON_LOCATION, DLI->GetDeviceNameWithEnumNumber(device_name_module_id, device_name_device_id, buffer, sizeof(buffer)));


      JsonBuilderI->Add(D_JSON_TIME, mTime::ConvertU32TimetoCtr(&pir_detect[sensor_id].detected_time, buffer, sizeof(buffer)));
      JsonBuilderI->Add(D_JSON_EVENT, pir_detect[sensor_id].isactive ? "detected": "over");

      #else
      //Phase out
      JsonBuilderI->Add(D_JSON_LOCATION, DLI->GetDeviceNameWithEnumNumber(EM_MODULE_SENSORS_MOTION_ID, sensor_id, buffer, sizeof(buffer)));
      JsonBuilderI->Add(D_JSON_TIME, mTime::ConvertU32TimetoCtr(&pir_detect[sensor_id].detected_time, buffer, sizeof(buffer)));
      JsonBuilderI->Add(D_JSON_EVENT, pir_detect[sensor_id].isactive ? "detected": "over");
      #endif


  //     JBI->Level_Start(DLI->GetDeviceNameWithEnumNumber(EM_MODULE_SENSORS_MOTION_ID, sensor_id, buffer, sizeof(buffer)));
  //       JBI->Add(D_JSON_TIME, mTime::ConvertShortTime_HHMMSS(&pir_detect[sensor_id].detected_time, buffer, sizeof(buffer)));
  //       JBI->Add(D_JSON_EVENT, pir_detect[sensor_id].isactive ? "detected": "over");
  //     JBI->Level_End();

  //     //if another is yet to send, then reset the mqtt_handler to fire immeditely again!
  //     //if any mtion flag remains, then set mqtt again
      
    }
  }


  return JsonBuilderI->End();
    
}



#endif
