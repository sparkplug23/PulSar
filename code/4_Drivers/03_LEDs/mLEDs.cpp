#include "mLEDs.h"

#ifdef USE_MODULE_DRIVERS_LEDS

const char* mLEDs::PM_MODULE_DRIVERS_LEDS_CTR = D_MODULE_DRIVERS_LEDS_CTR;
const char* mLEDs::PM_MODULE_DRIVERS_LEDS_FRIENDLY_CTR = D_MODULE_DRIVERS_LEDS_FRIENDLY_CTR;


int8_t mLEDs::Tasker(uint8_t function, JsonParserObject obj){

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

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP: 
      EveryLoop();
    break;
    case FUNC_EVERY_SECOND: 

      

      // digitalWrite(pCONT_pins->GetPin(GPIO_LED1_INV_ID), !digitalRead(pCONT_pins->GetPin(GPIO_LED1_INV_ID))); 
      // digitalWrite(pCONT_pins->GetPin(GPIO_LED2_INV_ID), !digitalRead(pCONT_pins->GetPin(GPIO_LED2_INV_ID))); 
      // digitalWrite(pCONT_pins->GetPin(GPIO_LED3_INV_ID), !digitalRead(pCONT_pins->GetPin(GPIO_LED3_INV_ID))); 


      // EveryLoop();
      // digitalWrite(pCONT_pins->GetPin(GPIO_LED1_ID),!digitalRead(pCONT_pins->GetPin(GPIO_LED1_ID)));
      // digitalWrite(pCONT_pins->GetPin(GPIO_LED2_ID),!digitalRead(pCONT_pins->GetPin(GPIO_LED2_ID)));

      // SetState
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_DefaultPeriodRate();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_RefreshAll();
    break;
    #endif //USE_MODULE_NETWORK_MQTT    
  }

} // END function


void mLEDs::Pre_Init(void){
  
  settings.fEnableSensor = false;

  // Check all possible pin options
  settings.leds_found = 0;    

  // Lets check each type on their own, normal, inverted etc
  for(uint8_t ii=0; 
              ii<MAX_LEDS; 
              ii++
  ){
    if(pCONT_pins->PinUsed(GPIO_LED1_ID, ii))
    {
      leds[settings.leds_found].pin = pCONT_pins->GetPin(GPIO_LED1_ID, ii);
      bitSet(led_inverted, 0);
      pinMode(leds[settings.leds_found].pin, OUTPUT); // Note: GPIO16/D0 inversion is pulldown, not up
      digitalWrite(leds[settings.leds_found].pin, bitRead(led_inverted, ii));
      if(settings.leds_found++ >= MAX_LEDS){ break; }
    }else
    if(pCONT_pins->PinUsed(GPIO_LED1_INV_ID, ii))
    {
      leds[settings.leds_found].pin = pCONT_pins->GetPin(GPIO_LED1_INV_ID, ii);
      bitSet(led_inverted, 1);
      pinMode(leds[settings.leds_found].pin, OUTPUT); // Note: GPIO16/D0 inversion is pulldown, not up
      digitalWrite(leds[settings.leds_found].pin, bitRead(led_inverted, ii));
      if(settings.leds_found++ >= MAX_LEDS){ break; }
    }
  }

  pCONT_set->devices_present += settings.leds_found;
  if(settings.leds_found){ settings.fEnableSensor = true; }

}


void mLEDs::Init(void)
{
  // Configured already
}


void mLEDs::EveryLoop()
{

  SubTask_Status_LEDs();


// digitalWrite(pCONT_pins->GetPin(GPIO_LED1_ID), !digitalRead(pCONT_pins->GetPin(GPIO_LED1_ID))); delay(200);

  // if((IsDoorOpen()!=door_detect.state)&&mTime::TimeReachedNonReset(&door_detect.tDetectTimeforDebounce,100)){
  //   door_detect.state = IsDoorOpen();
  //   door_detect.tDetectTimeforDebounce = millis();
  //   if(door_detect.state){ 
  //     door_detect.isactive = true;
  //     door_detect.detected_time = pCONT_time->GetTimeShortNow();
  //     mqtthandler_sensor_ifchanged.flags.SendNow = true;
  //   }else{ 
  //     door_detect.isactive = false;
  //   }
  //   door_detect.ischanged = true;
  //   mqtthandler_sensor_ifchanged.flags.SendNow = true;
  // }

}

/**
 * @brief Any LEDs configured as status leds will be handled here
 * 
 */
void mLEDs::SubTask_Status_LEDs()
{

  


}


 bool mLEDs::GetState(uint8_t index)
  {
    return digitalRead(bitRead(led_inverted, index)==1?0:1); // inverted, ON when low
  };

  void mLEDs::CommandSet_LED_Power(uint8_t state, uint8_t index)
  {

    
    // needs to be made special, is PWM type or not
    /**
     * @brief digital 0 = off, 1> then ON
     * 
     */
    int8_t pin_v = leds[index].pin;
    uint8_t state2 = state;

    ALOG_INF(PSTR("pin_v=%d,state=%d"), pin_v, state);

    /**
     * @brief 
     * PWM, 0-1023 range since esp32 is 10bit, esp8266 is 8bit
     * 
     */
    
        digitalWrite(pin_v, state);//bitRead(led_inverted, state)?0:1); // inverted, ON when low
  };




void mLEDs::UpdateLedPowerAll()
{
	// for (uint32_t i = 0; i < pCONT_set->leds_present; i++) {
	// 	SetLedPowerIdx(i, bitRead(pCONT_set->led_power, i));
	// }
}

void mLEDs::SetLedPowerIdx(uint32_t led, uint32_t state)
{
//   // if (!pCONT_pins->PinUsed(GPIO_LEDLNK_ID) && (0 == led)) {  // Legacy - LED1 is link led only if LED2 is present
//   //   if (pCONT_pins->PinUsed(GPIO_LED1_ID, 1)) {
//   //     led = 1;
//   //   }
//   // }
//   if (pCONT_pins->PinUsed(GPIO_LED1_ID, led)) {
//     uint32_t mask = 1 << led;
//     if (state) {
//       state = 1;
//       pCONT_set->led_power |= mask;
//     } else {
//       pCONT_set->led_power &= (0xFF ^ mask);
//     }
//     uint16_t pwm = 0;
//     if (bitRead(pCONT_set->Settings.ledpwm_mask, led)) {
// // #ifdef USE_LIGHT
// //       pwm = mapvalue(ledGamma10(state ? Settings.ledpwm_on : Settings.ledpwm_off), 0, 1023, 0, Settings.pwm_range); // gamma corrected
// // #else //USE_LIGHT
//       pwm = mapvalue((uint16_t)(state ? pCONT_set->Settings.ledpwm_on : pCONT_set->Settings.ledpwm_off), 0, 255, 0, pCONT_set->Settings.pwm_range); // linear
// // #endif //USE_LIGHT

// #ifdef ESP8266
//       analogWrite(pCONT_pins->Pin(GPIO_LED1_ID, led), bitRead(pCONT_set->led_inverted, led) ? pCONT_set->Settings.pwm_range - pwm : pwm);
      
// #endif // ESP8266
//     } else {
//       pCONT_pins->DigitalWrite(GPIO_LED1_ID+led, bitRead(pCONT_set->led_inverted, led) ? !state : state);
//     }
//   }
// // #ifdef USE_MODULE_DRIVERS_BUZZER
// //   if (led == 0) {
// //     BuzzerSetStateToLed(state);
// //   }
// // #endif // USE_MODULE_DRIVERS_BUZZER
}

void mLEDs::SetLedPower(uint32_t state)
{
  //   #ifdef ENABLE_LOG_LEVEL_INFO
  // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("SetLedPower(%d)"),state);
  //   #endif// ENABLE_LOG_LEVEL_INFO
  // // if (!pCONT_pins->PinUsed(GPIO_LEDLNK_ID)) {           // Legacy - Only use LED1 and/or LED2
  // //   SetLedPowerIdx(0, state);
  // // } else {
  //   power_t mask = 1;
  //   for (uint32_t i = 0; i < pCONT_set->leds_present; i++) {  // Map leds to power
  //     bool tstate = (pCONT_set->power & mask);
  //     SetLedPowerIdx(i, tstate);
  //     mask <<= 1;
  //   }
  // // }
}

void mLEDs::SetLedPowerAll(uint32_t state)
{
  // for (uint32_t i = 0; i < pCONT_set->leds_present; i++) {
  //   SetLedPowerIdx(i, state);
  // }
}

void mLEDs::SetLedLink(uint32_t state)
{
//     #ifdef ENABLE_LOG_LEVEL_INFO
//   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("SetLedLink(%d)"),state);
//     #endif// ENABLE_LOG_LEVEL_INFO

//   uint32_t led_pin = pCONT_pins->GetPin(GPIO_LED1_ID);
//   uint32_t led_inv = pCONT_set->ledlnk_inverted;
//   if (99 == led_pin) {                    // Legacy - LED1 is status
//     SetLedPowerIdx(0, state);
//   }
//   else if (led_pin < 99) {
//     if (state) { state = 1; }
//     digitalWrite(led_pin, (led_inv) ? !state : state);
//   }
// // #ifdef USE_MODULE_DRIVERS_BUZZER
// //   BuzzerSetStateToLed(state);
// // #endif // USE_MODULE_DRIVERS_BUZZER
}

/**
 * @brief 
 * 
 * Version 1: Just on/off functions
 * Version 2: Remove this and do my own method ie how neopixel notifications was to blink, pulse etc
 *
 * In this class, I will include a way to set a LED index as a specific type.
 * PowerLED (tied to relay), StatusLED (network/mqtt connection)
 * If none are set and only one exists, then assume as StatusLED and dont show PowerLED
 *  
 */
void mLEDs::UpdateStatusBlink()
{
  
  DEBUG_LINE;
  uint8_t blinkinterval = 1;
  // pCONT_set->global_state.network_down = (pCONT_set->global_state.wifi_down && pCONT_set->global_state.eth_down);

  if (!pCONT_set->Settings.flag_system.global_state) {                      // Problem blinkyblinky enabled
    if (pCONT_set->global_state.data) {                              // Any problem
      if (pCONT_set->global_state.mqtt_down) { blinkinterval = 7; }  // MQTT problem so blink every 2 seconds (slowest)
      if (pCONT_set->global_state.wifi_down) { blinkinterval = 3; }  // Wifi problem so blink every second (slow)
      pCONT_set->blinks = 201;                                       // Allow only a single blink in case the problem is solved
    }
  }

//DEBUG_LINE_HERE;
  DEBUG_LINE;
  if (pCONT_set->blinks || pCONT_set->restart_flag || pCONT_set->ota_state_flag) {

    // Work out the led state based on time
    if (pCONT_set->restart_flag || pCONT_set->ota_state_flag) {                 // Overrule blinks and keep led lit
    
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_WARN, PSTR("blinkstate phasing out for new method"));
      

    #endif //  ENABLE_LOG_LEVEL_INFO
      
      
      pCONT_set->blinkstate = true;                                  // Stay lit
    } else {
      pCONT_set->blinkspeed--; // based of multiples of 200ms
      if (!pCONT_set->blinkspeed) {
        pCONT_set->blinkspeed = blinkinterval;                       // Set interval to 0.2 (default), 1 or 2 seconds
        pCONT_set->blinkstate ^= 1;                                  // Blink
      }
    }

//DEBUG_LINE_HERE;
  DEBUG_LINE;
    // Update Link LED
    if ((!(pCONT_set->Settings.ledstate &0x08)) && ((pCONT_set->Settings.ledstate &0x06) || (pCONT_set->blinks > 200) || (pCONT_set->blinkstate))) {
      pCONT_led->SetLedLink(pCONT_set->blinkstate);                            // Set led on or off
    }

    // If blink has completed
    if (!pCONT_set->blinkstate) {
      pCONT_set->blinks--;
      if (200 == pCONT_set->blinks) pCONT_set->blinks = 0;                      // Disable blink
    }

  }


//DEBUG_LINE_HERE;
  // if (pCONT_set->Settings.ledstate &1 && (pCONT_pins->PinUsed(GPIO_LEDLNK_ID) || !(pCONT_set->blinks || pCONT_set->restart_flag || pCONT_set->ota_state_flag)) ) {
  //   bool tstate = pCONT_set->power & pCONT_set->Settings.ledmask;
  //   // if ((MODULE_SONOFF_TOUCH == pCONT_set->my_module_type) || 
  //   //(MODULE_SONOFF_T11 == pCONT_set->my_module_type) || 
  //   //(MODULE_SONOFF_T12 == pCONT_set->my_module_type) || 
  //   //(MODULE_SONOFF_T13 == pCONT_set->my_module_type)) {
  //   //   tstate = (!pCONT_set->power) ? 1 : 0;                          // As requested invert signal for Touch devices to find them in the dark
  //   // }
  //  SetLedPower(tstate);
  // }

  DEBUG_LINE;
  //   #ifdef ENABLE_LOG_LEVEL_INFO
  // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("{blinkstate:%d,blinks:%d}"),pCONT_set->blinkstate,pCONT_set->blinks);
  //   #endif// ENABLE_LOG_LEVEL_INFO

}

/******************************************************************************************************************
 * 
*******************************************************************************************************************/

  
/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

void mLEDs::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

int8_t led_id = -1;
uint16_t state_value = 0;

  int8_t relay_id= -1,state=-1;    //assume index 0 if none given

  if(jtok = obj["LED"].getObject()["Name"]){
    if(jtok.isStr()){
      // relay_id = GetRelayIDbyName(jtok.getStr());
      ALOG_INF( PSTR("relay_id = %s"), jtok.getStr() );
    }else 
    if(jtok.isNum()){
      relay_id  = jtok.getInt();
    }

    ALOG_INF( PSTR("relay_id = %d"), relay_id );
  }

  // Primary method since v0.86.14.21
  if(jtok = obj["LED"].getObject()["State"]){
    if(jtok.isStr()){
      state = pCONT_sup->GetStateNumber(jtok.getStr());
    }else 
    if(jtok.isNum()){
      state  = jtok.getInt();//pCONT_sup->GetStateNumber(jtok.getInt());
    }

    /**
     * @brief If off, clear any timer decounters for relays
     * 
     */
    if(state == 0)
    {
      // CommandSet_Timer_Decounter(0, relay_id);
    }

		//state needs checked for flipped
		// if(state == 2){

		// }

  }



  


  // if(jtok = obj[PM_JSON_RELAY].getObject()[PM_JSON_TIME_ON]){
  //   CommandSet_Timer_Decounter(jtok.getInt(), relay_id);
  // }else
  // if(jtok = obj[PM_JSON_RELAY].getObject()[PM_JSON_TIME_ON_SECS]){
  //   CommandSet_Timer_Decounter(jtok.getInt(), relay_id);
  // }else
  // if(jtok = obj[PM_JSON_RELAY].getObject()[PM_JSON_TIME_ON_MINUTES]){
  //   CommandSet_Timer_Decounter(jtok.getInt()*60, relay_id);
  // }

ALOG_INF(PSTR("HER1"));
  if(IsWithinRange(state, 0,10) && IsWithinRange(relay_id, 0,settings.leds_found)){
ALOG_INF(PSTR("HER2"));
    CommandSet_LED_Power(state,relay_id);
  }
ALOG_INF(PSTR("HER3"));


// 	if(jtok = obj["LED"].getObject()["Name"])
// 	{

// 		led_id = jtok.getInt();

// 	}

// 	if(jtok = obj["LED"].getObject()["SetState"])
// 	{

// 		if(jtok.isNum())
// 		{
// 			// mySwitch->setReceiveProtocolMask(jtok.getUInt());
// 			mqtthandler_settings_teleperiod.flags.SendNow = true;
// 		}

// 	}



//  if(jtok = obj["LEDName"]){
//     // if(jtok.isStr()){
//     //   relay_id = GetRelayIDbyName(jtok.getStr());
//     // }else 
//     // if(jtok.isNum()){
//     //   relay_id  = jtok.getInt();
//     // }
// 	led_id = jtok.getInt();
//   }

//   // Primary method since v0.86.14.21
//   if(jtok = obj["LEDState"]){
//     // if(jtok.isStr()){
//     //   state = pCONT_sup->GetStateNumber(jtok.getStr());
//     // }else 
//     // if(jtok.isNum()){
//       state_value  = jtok.getInt();//pCONT_sup->GetStateNumber(jtok.getInt());
//     // }

//     /**
//      * @brief If off, clear any timer decounters for relays
//      * 
//      */
//     // if(state == 0)
//     // {
//     //   CommandSet_Timer_Decounter(0, relay_id);
//     // }

// 		//state needs checked for flipped
// 		// if(state == 2){

// 		// }
		
// // SetState()

//   }
    
}
  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mLEDs::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_JSON_COUNT, settings.leds_found);
    // JBI->Add("RfMask", mySwitch->GetReceiveProtolMask());
  return JBI->End();

}

uint8_t mLEDs::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[40];

  JBI->Start();

    // JBI->Level_Start(D_JSON_RFRECEIVED);
  
      // JBI->Add("Pin1", pCONT_pins->GetPin(GPIO_LED1_ID));

      JBI->Add("LED1_INV", pCONT_pins->GetPin(GPIO_LED1_INV_ID));
      JBI->Add("LED2_INV", pCONT_pins->GetPin(GPIO_LED2_INV_ID));

      // JBI->Add(D_JSON_RF_BITS, rx_pkt.bit_length);
      // JBI->Add(D_JSON_RF_PROTOCOL, rx_pkt.protocol);
      // JBI->Add(D_JSON_RF_PULSE, rx_pkt.delay);   
      // JBI->Add(D_JSON_MILLIS, rx_pkt.received_time_millis);   
      // JBI->Add(D_JSON_TIME, mTime::ConvertU32TimetoCtr(&rx_pkt.received_utc_time, buffer, sizeof(buffer)));
      
    
    // JBI->Level_End();
  
  

  return JBI->End();

}


  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mLEDs::MQTTHandler_Init()
{

  struct handler<mLEDs>* ptr;

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 120; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mLEDs::ConstructJSON_Settings;

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mLEDs::ConstructJSON_State;

} //end "MQTTHandler_Init"


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mLEDs::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mLEDs::MQTTHandler_Set_DefaultPeriodRate()
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
void mLEDs::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_DRIVERS_LEDS_ID, handle, id);
  }
}

#endif // USE_MODULE_NETWORK_MQTT




#endif
