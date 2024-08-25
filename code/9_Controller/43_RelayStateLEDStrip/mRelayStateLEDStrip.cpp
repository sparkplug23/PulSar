#include "mRelayStateLEDStrip.h"

#ifdef USE_MODULE_CONTROLLERS__RELAY_STATE_LEDSTRIP

/**
 * @brief: Module Interface
 * */
int8_t mRelayStateLEDStrip::Tasker(uint8_t function, JsonParserObject obj)
{

  int8_t function_result = 0;

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

  if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_EVERY_SECOND:
      EverySecond();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    case FUNC_APPEND_RESPONSE_JSON_DRIVERS_STATUS_ID:
      AppendJSONResponse_Drivers_Unified(); // should this be an override? prob too complex to be useful
    break;
    /************
     * RULES SECTION * 
    *******************/
    #ifdef USE_MODULE_CORE_RULES
    // case FUNC_EVENT_SET_POWER_ID: 
    //   RulesEvent_Set_Power();
    // break;
    #endif// USE_MODULE_CORE_RULES
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
      MQTTHandler_Init(); //make a FUNC_MQTT_INIT and group mqtt togather
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender(); //optional pass parameter
    break;
    case FUNC_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Set_DefaultPeriodRate(); // Load teleperiod setting into local handlers
    break; 
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_RefreshAll();
    break;
    #endif // USE_MODULE_NETWORK_MQTT
  } // end switch
} // END function

void mRelayStateLEDStrip::Pre_Init(void){
  
  module_state.mode = ModuleStatus::Initialising;
  module_state.devices = 0;

  module_state.devices++;

  if(module_state.devices)
  {
    module_state.mode = ModuleStatus::Running;
  }

}


void mRelayStateLEDStrip::Init(void){

  // module_state.devices = pCONT_set->runtime.devices_present; //phase out

  // module_state.devices = MAX_RELAYS;


  // clear all settings to 0
  memset(&rt.relay_status, 0, sizeof(rt.relay_status));

  // Set defaults
  for(int relay_id=0;relay_id<MAX_RELAYS;relay_id++){
    rt.relay_status[relay_id].timer_decounter.seconds = 0;
    rt.relay_status[relay_id].timer_decounter.active = false;
  }
  
}


void mRelayStateLEDStrip::EverySecond()
{

  /*
    Change strip into manual mode
  */
  uint8_t segment_index = ENABLE_CONTROLLERS__RELAY_STATE_LEDSTRIP__SEGMENT_INDEX;

  pSEGMENT_I(segment_index).effect_id = mAnimatorLight::EFFECTS_FUNCTION__MANUAL__PIXEL_SET_ELSEWHERE__ID;
  
  // pSEGMENT_I(segment_index).SetPixelColor(0, RgbcctColor(255,0,0));
  // pSEGMENT_I(segment_index).SetPixelColor(1, RgbcctColor(0,255,0));
  // pSEGMENT_I(segment_index).SetPixelColor(2, RgbcctColor(0,0,255));

  uint8_t relays_available = pCONT_mry->module_state.devices;
  uint8_t pixels_in_strip = pSEGMENT_I(segment_index).length();

  for(uint8_t relay_id=0;relay_id<relays_available;relay_id++)
  {
    if(relay_id < pixels_in_strip)
    {
      if(pCONT_mry->CommandGet_Relay_Power(relay_id))
      {
        RgbcctColor colour_on = RgbcctColor(0,50,0);
        // Toggle LED colour if timer is active, ie if relay is on but will turn itself off
        if(rt.relay_status[relay_id].timer_decounter.active) // zone timer instead should be set into relay code too, but keep this code here so I know when I fix that
        {
          colour_on = rt.relay_status[relay_id].timer_decounter.active % 2 ?  RgbcctColor(0,50,0) :  RgbcctColor(0,0,50);
        }
        pSEGMENT_I(segment_index).SetPixelColor(relay_id, colour_on);
        pSEGMENT_I(segment_index).setBrightnessRGB(255);


      }
      else
      {
        pSEGMENT_I(segment_index).SetPixelColor(relay_id, RgbcctColor(5,0,0));
        pSEGMENT_I(segment_index).setBrightnessRGB(255);
      }
    }
  }

}


void mRelayStateLEDStrip::parse_JSONCommand(JsonParserObject obj)
{

  ALOG_DBM(PSTR("mRelayStateLEDStrip::parse_JSONCommand"));

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  JsonParserObject jobj = 0;
 
  int8_t relay_id= -1,state=-1;    //assume index 0 if none given

  // if(jtok = obj[PM_JSON_POWERNAME]){
  //   if(jtok.isStr()){
  //     relay_id = GetRelayIDbyName(jtok.getStr());
  //   ALOG_INF( PSTR("relay_id = %s"), jtok.getStr() );
  //   }else 
  //   if(jtok.isNum()){
  //     relay_id  = jtok.getInt();
  //   }

  //   ALOG_INF( PSTR("relay_id = %d"), relay_id );
  // }

  // // Primary method since v0.86.14.21
  // if(jtok = obj[PM_JSON_POWER_STATE]){
  //   if(jtok.isStr()){
  //     state = pCONT_sup->GetStateNumber(jtok.getStr());
  //   }else 
  //   if(jtok.isNum()){
  //     state  = jtok.getInt();//pCONT_sup->GetStateNumber(jtok.getInt());
  //   }

  //   /**
  //    * @brief If off, clear any timer decounters for relays
  //    * 
  //    */
  //   if(state == 0)
  //   {
  //     CommandSet_Timer_Decounter(0, relay_id);
  //   }

	// 	//state needs checked for flipped
	// 	// if(state == 2){

	// 	// }

  // }


}






uint8_t mRelayStateLEDStrip::ConstructJSON_Settings(uint8_t json_method, bool json_appending){

  JBI->Start();
    // JBI->Add(PM_JSON_DEVICES_CONNECTED, module_state.devices);

    // JBI->Array_Start_P(PSTR("rel_inverted"));
    //   for(int8_t bits=0; bits<sizeof(rt.rel_inverted)*8; bits++)
    //   {
    //     JBI->Add(bitRead(rt.rel_inverted,bits));
    //   }
    // JBI->Array_End();



  JBI->End();

}


uint8_t mRelayStateLEDStrip::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[100];

  if(json_appending)
  {
    JBI->Start();
  }


  
  if(json_appending)
  {
    return JBI->End();
  }
  return JBI->Length();

}

/**
 * @brief I should consider passing another argument into normal ConstructJson to disabled adding Start/Stop so this function could simply call another without them?
 * 
 * @return uint_t 
 */
uint8_t mRelayStateLEDStrip::AppendJSONResponse_Drivers_Unified()
{
  JBI->Level_Start_P(PM_MODULE_CONTROLLERS__RELAY_STATE_LEDSTRIP__CTR);
    ConstructJSON_State(JSON_LEVEL_SHORT, false);
  JBI->Object_End();
}



#ifdef USE_MODULE_NETWORK_MQTT

void mRelayStateLEDStrip::MQTTHandler_Init()
{

  struct handler<mRelayStateLEDStrip>* ptr;

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mRelayStateLEDStrip::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mRelayStateLEDStrip::ConstructJSON_State;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mRelayStateLEDStrip::ConstructJSON_State;
  mqtthandler_list.push_back(ptr);

} //end "MQTTHandler_Init"

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mRelayStateLEDStrip::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mRelayStateLEDStrip::MQTTHandler_Set_DefaultPeriodRate()
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
void mRelayStateLEDStrip::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_CONTROLLERS__RELAY_STATE_LEDSTRIP__ID, handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT


#endif

