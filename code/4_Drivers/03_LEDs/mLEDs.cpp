#include "mLEDs.h"

#ifdef USE_MODULE_DRIVERS_LEDS

/****
 * 
 * 
 * Status LED needs added inside this
 * Status_Mode
 *  - network_down (3 quick blinlks, 1 second pause)
 *  - mqtt_down (2 quick blinks, 1 second pause)
 *  
 * Notice that state needs placed on top of an LED which is being used for relay on/off either way
 * status_led optionally will show power state of primary.
 * 
 * 
 * Show MOTION on STATUS_LED
 * 
 * 
 */

int8_t mLEDs::Tasker(uint8_t function, JsonParserObject obj){

  // Serial.printf("mLEDs::Tasker function=%u\n\r", function); Serial.flush();
  // return 0;

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
    case TASK_BOOT_MESSAGE:
      BootMessage();
    break;
    #ifdef ENABLE_DEVFEATURE_DRIVER_LED__FORCED_LED_TOGGLE_ON_PIN
    case TASK_EVERY_SECOND:
      pinMode(ENABLE_DEVFEATURE_DRIVER_LED__FORCED_LED_TOGGLE_ON_PIN, OUTPUT); // Set pin output
      digitalWrite(ENABLE_DEVFEATURE_DRIVER_LED__FORCED_LED_TOGGLE_ON_PIN, !digitalRead(ENABLE_DEVFEATURE_DRIVER_LED__FORCED_LED_TOGGLE_ON_PIN)); // Blink the LED on pin 8 every second
      Serial.printf("LED on pin %d toggled\n\r", ENABLE_DEVFEATURE_DRIVER_LED__FORCED_LED_TOGGLE_ON_PIN); // Debug message
    break;
    #endif // ENABLE_DEVFEATURE_DRIVER_LED__FORCED_LED_TOGGLE_ON_PIN
    #ifdef ENABLE_DEVFEATURE_DRIVER_LED__FORCED_LED_TOGGLE_LED1
    case TASK_EVERY_SECOND:
      digitalWrite(tkr_pins->Pin(GPIO_LED1_INV), !digitalRead(tkr_pins->Pin(GPIO_LED1_INV))); // Blink the LED on pin 8 every second
      Serial.printf("LED on pin %d toggled\n\r", tkr_pins->Pin(GPIO_LED1_INV)); // Debug message
    break;
    #endif // ENABLE_DEVFEATURE_DRIVER_LED__FORCED_LED_TOGGLE_ON_PIN
  }

  if(module_state.mode != ModuleStatus::Running){ return TASKER_RESULT__MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP: 
      Config_StatusEffect();
      Refresh_AllLEDs();  // This will update all the LEDs according to their modes
    break;

    case TASK_UPTIME_1_MINUTES:
    
    // Test inject patterns
    SET_SYSTEM_LED__AP_MODE(true);

    break;

    case TASK_EVERY_50_MSECOND:
      Handle_SystemLEDs();
    break;

    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
     /************
     * TELEMETRY SECTION * 
    *******************/
    case TASK_TELEMETRY_HANDLERS_INIT:
      Telemetry_Init();
    break;
    case TASK_TELEMETRY_REFRESH_SEND_ALL:
      tkr_tele->Telemetry_RefreshAll(telemetry_list);
    break;
    case TASK_TELEMETRY_SET_DEFAULT_TRANSMIT_PERIOD:
      tkr_tele->Telemetry_Rate(telemetry_list);
    break;
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_TELEMETRY__SENDER_MQTT:
      tkr_mqtt->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
    #ifdef USE_MODULE_SERIAL
    case TASK_SERIAL_TELEMETRY:
      tkr_serial->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case TASK_WEB_TELEMETRY:
      tkr_web->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif 
  }

  return TASKER_RESULT__UNKNOWN_ID;

} // END function


void mLEDs::Pre_Init(void)
{
  ALOG_INF(PSTR("D_LOG_STARTUP: " "LED Init"));

  used_bitmask = 0;
  inverted_bitmask = 0;

  leds.clear();
  leds.resize(MODULE_LEDS_MAX);

  for(uint8_t led_index = 0; led_index < MODULE_LEDS_MAX; led_index++)
  {
    int16_t pin = -1;
    bool inverted = false;

    if(tkr_pins->PinUsed(GPIO_LED1, led_index))
    {
      pin = tkr_pins->GetPin(GPIO_LED1, led_index);
      inverted = false;
    }
    else if(tkr_pins->PinUsed(GPIO_LED1_INV, led_index))
    {
      pin = tkr_pins->GetPin(GPIO_LED1_INV, led_index);
      inverted = true;
    }
    else
    {
      ALOG_DBG(PSTR(D_LOG_LED "%u None"), led_index);
      continue;
    }

    if(!LED_PinIsValid(pin))
    {
      ALOG_ERR(PSTR(D_LOG_LED "LED%u invalid GPIO pin=%d"), led_index, pin);
      continue;
    }

    SetUsed(led_index);

    if(inverted)
    {
      SetInvertFlag(led_index);
    }

    LedState& led = leds[led_index];

    led.configured = true;
    led.inverted = inverted;
    led.pin = pin;
    led.effect = LED_OFF;
    led.state = inverted ? HIGH : LOW;
    led.lastUpdateTime = millis();
    led.startTime = millis();

    pinMode(pin, OUTPUT);
    LED_Write(led_index, false);

    ALOG_INF(
      PSTR(D_LOG_LED "LED%u pin=%d inverted=%u used=%s"),
      led_index,
      pin,
      inverted,
      toBinaryString(used_bitmask, MODULE_LEDS_MAX).c_str()
    );
  }

  if(used_bitmask)
  {
    module_state.mode = ModuleStatus::Initialising;
  }
}

void mLEDs::Init(void)
{
  module_state.mode = ModuleStatus::Running;

  for(uint8_t led_index = 0; led_index < MODULE_LEDS_MAX; led_index++)
  {
    if(!IsUsed(led_index)) { continue; }
    if(led_index >= leds.size()) { continue; }
    if(!leds[led_index].configured) { continue; }

    pinMode(leds[led_index].pin, OUTPUT);
    LED_Write(led_index, false);

    StartEffect_Blink(led_index, 40, 100, 1000, 60);
  }

  // Default system LED role assignment.
  // LED1 is user-facing naming, but internally this maps to leds[0].
  // Later this can be overridden by JSON/template configuration.
  system_led.status_index = 0;
  // system_led.power_index = 0;   // LED1 / leds[0] used as aggregate power LED when no status event is active

}


void mLEDs::BootMessage()
{
  #ifdef ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
  char buffer[100];
  if(used_bitmask)
  {
    mSupport::appendToBuffer(buffer, sizeof(buffer), "#%d ", UsedCount());  
    char buffer2[50];
    for(uint8_t sensor_id = 0; sensor_id<MODULE_LEDS_MAX; sensor_id++)
    {      
      //if bit0 is used as LEDSTATUS, then show this here too
      mSupport::appendToBuffer(buffer, sizeof(buffer), "%d, ", sensor_id);    
    }
  }
  else{
    mSupport::appendToBuffer(buffer, sizeof(buffer), "None");  
  }
  mSupport::removeTrailingComma(buffer);
  ALOG_IMP(PSTR(D_LOG_LED "%s"), buffer);
  #endif // ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
}

bool mLEDs::LED_PinIsValid(int16_t pin)
{
  if(pin < 0) { return false; }

  // Catches bad encoded/invalid values such as 226.
  // ESP32 variants normally stay well below this.
  if(pin > 48) { return false; }

  return true;
}


void mLEDs::LED_Write(uint8_t index, bool on)
{
  if(index >= leds.size()) { return; }

  LedState& led = leds[index];

  if(!led.configured) { return; }
  if(!LED_PinIsValid(led.pin)) { return; }

  const uint8_t level = led.inverted ? (on ? LOW : HIGH) : (on ? HIGH : LOW);

  digitalWrite(led.pin, level);
  led.state = level;
}


void mLEDs::LED_PWMWrite(uint8_t index, uint8_t duty)
{
  if(index >= leds.size()) { return; }

  LedState& led = leds[index];

  if(!led.configured) { return; }
  if(!LED_PinIsValid(led.pin)) { return; }

  const uint8_t pwm = led.inverted ? (255 - duty) : duty;

  analogWrite(led.pin, pwm);
  led.state = pwm ? HIGH : LOW;
}


void mLEDs::Refresh_AllLEDs()
{
  for(uint8_t led_index = 0; led_index < MODULE_LEDS_MAX; led_index++)
  {
    if(!IsUsed(led_index)) { continue; }

    Refresh_LED(led_index);
  }
}


void mLEDs::Refresh_LED(uint8_t led_index)
{
  if(led_index >= leds.size()) { return; }
  if(!IsUsed(led_index)) { return; }

  LedState& led = leds[led_index];

  if(!led.configured) { return; }

  if(!LED_PinIsValid(led.pin))
  {
    ALOG_ERR(PSTR(D_LOG_LED "Refresh_LED invalid pin led=%u pin=%d"), led_index, led.pin);
    return;
  }

  const uint32_t currentTime = millis();
  uint32_t tElapsed = currentTime - led.lastUpdateTime;

  if(led.duration_secs > 0 && (currentTime - led.startTime) >= ((uint32_t)led.duration_secs * 1000UL))
  {
    if(led.effect != LED_OFF)
    {
      LED_Write(led_index, false);
      led.effect = LED_OFF;
      ALOG_DBM(PSTR("LED%u pin=%d Reset OFF after duration"), led_index, led.pin);
    }

    return;
  }

  switch(led.effect)
  {
    case LED_ON:
    {
      LED_Write(led_index, true);
    }
    break;

    case LED_OFF:
    {
      LED_Write(led_index, false);
    }
    break;

    case LED_BLINK:
    {
      if(led.period == 0 || led.count == 0)
      {
        LED_Write(led_index, false);
        return;
      }

      const uint32_t totalCycle = ((uint32_t)led.period * led.count) + led.groupPause;

      if(tElapsed >= totalCycle)
      {
        led.lastUpdateTime = currentTime;
        tElapsed = 0;
      }

      if(tElapsed < ((uint32_t)led.period * led.count))
      {
        if((tElapsed % led.period) < (led.period / 2))
        {
          LED_Write(led_index, true);
        }
        else
        {
          LED_Write(led_index, false);
        }
      }
      else
      {
        LED_Write(led_index, false);
      }
    }
    break;

    case LED_PULSE:
    {
      if(led.period == 0 || led.count == 0)
      {
        LED_PWMWrite(led_index, 0);
        return;
      }

      const uint32_t totalCycle = ((uint32_t)led.period * led.count) + led.groupPause;

      if(tElapsed >= totalCycle)
      {
        led.lastUpdateTime = currentTime;
        tElapsed = 0;
      }

      if(tElapsed < ((uint32_t)led.period * led.count))
      {
        const uint32_t currentPhaseTime = tElapsed % led.period;
        const float phase = (float)currentPhaseTime / (float)led.period;
        const float dutyCycle = (sin(2 * PI * phase) + 1.0f) / 2.0f;

        LED_PWMWrite(led_index, (uint8_t)(dutyCycle * 255.0f));
      }
      else
      {
        LED_PWMWrite(led_index, 0);
      }
    }
    break;
  }
}


void mLEDs::StartEffect_On(uint8_t index)
{
  if(index >= leds.size()) { return; }
  if(!IsUsed(index)) { return; }

  leds[index].effect = LED_ON;
  leds[index].lastUpdateTime = millis();
}


void mLEDs::StartEffect_Off(uint8_t index)
{
  if(index >= leds.size()) { return; }
  if(!IsUsed(index)) { return; }

  leds[index].effect = LED_OFF;
  leds[index].lastUpdateTime = millis();
}


void mLEDs::StartEffect_Blink(uint8_t index, uint8_t blinkCount, uint16_t blinkInterval, uint16_t blinkGroupPause, uint8_t duration_secs)
{
  if(index >= leds.size()) { return; }
  if(!IsUsed(index)) { return; }

  leds[index].effect = LED_BLINK;
  leds[index].count = blinkCount;
  leds[index].period = blinkInterval;
  leds[index].groupPause = blinkGroupPause;
  leds[index].lastUpdateTime = millis();
  leds[index].state = LOW;
  leds[index].duration_secs = duration_secs;
  leds[index].startTime = millis();
}


void mLEDs::StartEffect_Pulse(uint8_t index, uint8_t pulseCount, uint16_t period, uint16_t groupPause, uint8_t duration_secs)
{
  if(index >= leds.size()) { return; }
  if(!IsUsed(index)) { return; }

  LedState& led = leds[index];

  if(!led.configured) { return; }
  if(!LED_PinIsValid(led.pin)) { return; }

  led.effect = LED_PULSE;
  led.period = period;
  led.groupPause = groupPause;
  led.count = pulseCount;
  led.lastUpdateTime = millis();
  led.duration_secs = duration_secs;
  led.startTime = millis();

  #ifdef ESP32
  analogAttach(led.pin, index);
  #endif
}

void mLEDs::SetInvertFlag(uint8_t b) {
  bitSet(inverted_bitmask, b);
}

bool mLEDs::IsUsed(uint8_t index) {
  return (tkr_pins->PinUsed(GPIO_LED1, index) || tkr_pins->PinUsed(GPIO_LED1_INV, index) || bitRead(used_bitmask, index));
}

void mLEDs::SetUsed(uint8_t index) {
  bitSet(used_bitmask, index);
}

uint8_t mLEDs::UsedCount() {
  return __builtin_popcount(used_bitmask);
}



/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

void mLEDs::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  JsonParserToken jtok_sub = 0; 
  int8_t tmp_id = 0;

int8_t led_id = -1;
uint16_t state_value = 0;

  // int8_t relay_id= -1,
  int8_t state = -1;    //assume index 0 if none given

  int8_t led_index = -1; // -1 means unset

  if(!(jtok = obj["LED"]))
  {
    return;
  }

  ALOG_INF(PSTR("LED command found"));

  // All commands are inside {"LED":{X}}
  JsonParserObject jobj = jtok.getObject();
  // JsonParserToken jtok_sub = 0;

  if(jtok_sub = jobj["Name"])
  {
    if(jtok_sub.isStr()){
      // relay_id = GetRelayIDbyName(jtok.getStr());
      int8_t id_tmp = DLI->GetDeviceIDbyName(jtok_sub.getStr(), GetModuleUniqueID());
      if(IsWithinRange(id_tmp, 0, UsedCount()))
      {
        led_index = id_tmp;
        ALOG_INF(PSTR("Valid index %d for %s"), led_index, jtok_sub.getStr());
      }
      ALOG_INF( PSTR("led_index = %s"), jtok_sub.getStr() ); // setting via name is not actually supported at this time
    }else 
    if(jtok_sub.isNum()){
      led_index  = jtok_sub.getInt();
    }

    ALOG_INF( PSTR(D_LOG_LED "Name = %d"), led_index );
  }


  // Primary method since v0.86.14.21
  if(jtok_sub = jobj["State"]){
    if(jtok_sub.isStr()){
      state = tkr_sup->GetStateNumber(jtok_sub.getStr());
    }else 
    if(jtok_sub.isNum()){
      state  = jtok_sub.getInt();//tkr_sup->GetStateNumber(jtok.getInt());
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


  // -------------------------------------------------------------------------
  // LED Blink command
  // -------------------------------------------------------------------------
  // JSON:
  //   {"LED":{"Blink":[led_index, blink_count, blink_period_ms, group_pause_ms, duration_secs]}}
  //
  // Example:
  //   {"LED":{"Blink":[0,10,200,200,10]}}
  //
  // Meaning:
  //   led_index        = 0     LED index to control
  //   blink_count      = 10    Number of blink periods per group
  //   blink_period_ms  = 200   Full ON+OFF blink period in milliseconds
  //   group_pause_ms   = 200   Pause after each blink group
  //   duration_secs    = 10    Total effect duration in seconds
  //
  // Notes:
  //   - If LED name lookup was used, led_index is overridden by led_index.
  //   - blink_period_ms is split internally into ON for half period,
  //     then OFF for half period.
  // -------------------------------------------------------------------------
  if(jtok_sub = jobj["Blink"])
  {
    std::vector<uint32_t> data;
    for(auto v : jtok_sub.getArray()) 
    {
      data.push_back(v.getInt());
    }
    ALOG_INF(PSTR("TEST BLINK"));

    if(led_index != -1)
    {
      // Assume set via name, so will override the array
      data[0] = led_index;
    }

    StartEffect_Blink(data[0], data[1], data[2], data[3], data[4]);
  }


  // -------------------------------------------------------------------------
  // LED Pulse command
  // -------------------------------------------------------------------------
  // JSON:
  //   {"LED":{"Pulse":[led_index, pulse_count, pulse_period_ms, group_pause_ms, duration_secs]}}
  //
  // Example:
  //   {"LED":{"Pulse":[0,5,1000,500,20]}}
  //
  // Meaning:
  //   led_index        = 0      LED index to control
  //   pulse_count      = 5      Number of pulse periods per group
  //   pulse_period_ms  = 1000   Full brightness wave period in milliseconds
  //   group_pause_ms   = 500    Pause after each pulse group
  //   duration_secs    = 20     Total effect duration in seconds
  //
  // Notes:
  //   - If LED name lookup was used, led_index is overridden by led_index.
  //   - Pulse uses PWM/sine brightness modulation.
  //   - Inverted LEDs are handled in LED_PWMWrite().
  // -------------------------------------------------------------------------
  if(jtok_sub = jobj["Pulse"])
  {
    std::vector<uint32_t> data;
    for(auto v : jtok_sub.getArray()) 
    {
      data.push_back(v.getInt());
    }
    ALOG_INF(PSTR("TEST PULSE"));
    
    if(led_index != -1)
    {
      // Assume set via name, so will override the array
      data[0] = led_index;
    }
    
    StartEffect_Pulse(data[0], data[1], data[2], data[3], data[4]);
  }

  // if(IsWithinRange(state, 0,10)){//} && IsWithinRange(relay_id, 0,settings.leds_found)){
  //   // If set manually, then override EffectMode to be manual
  //   // CommandSet_LED_Power(state,relay_id);
  // }


}
  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mLEDs::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_COUNT, UsedCount() );
  return JBI->End();

}

uint8_t mLEDs::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[40];

  JBI->Start();

    JBI->Add("LED1_INV", tkr_pins->GetPin(GPIO_LED1_INV,0));
    JBI->Add("LED2_INV", tkr_pins->GetPin(GPIO_LED1_INV,1));

  return JBI->End();

}
  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mLEDs::Telemetry_Init()
{

  struct telemetry_handler<mLEDs>* ptr;

  ptr = &telemetry_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetConfigPeriod(); 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mLEDs::ConstructJSON_Settings;
  telemetry_list.push_back(ptr);

  ptr = &telemetry_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetIfChangedPeriod(); 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->flags.json_level = JSON_LEVEL_IFCHANGED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mLEDs::ConstructJSON_State;
  telemetry_list.push_back(ptr);

} 

#endif // USE_MODULE_NETWORK_MQTT


#endif
