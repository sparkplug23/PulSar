#include "mRelays.h"

#ifdef USE_MODULE_DRIVERS_RELAY

/**
 * @brief: Module Interface
 * */
int8_t mRelays::Tasker(uint8_t function, JsonParserObject obj)
{

  int8_t function_result = 0;

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
  }

  if(module_state.mode != ModuleStatus::Running){ return TASKER_RESULT__MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:
      EverySecond();
    break;
    case TASK_EVERY_MINUTE:
      SubTask_Every_Minute();
    break;
    /************
     * STORAGE SECTION * 
    *******************/  
    #ifdef USE_MODULE_CORE_FILESYSTEM
    #ifdef ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__DRIVERS___RELAYS
    case TASK_FILESYSTEM__SAVE__MODULE_DATA__ID:
      Save_Module();
    break;
    case TASK_FILESYSTEM__LOAD__MODULE_DATA__ID:
      Load_Module();
    break;
    #endif // ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__DRIVERS___RELAYS
    #endif // USE_MODULE_CORE_FILESYSTEM
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    case TASK_SET_POWER_ON_ID:
      CommandSet_Relay_Power(STATE_NUMBER_ON_ID);
    break;    
    case TASK_APPEND_RESPONSE_JSON_DRIVERS_STATUS_ID:
      AppendJSONResponse_Drivers_Unified();
    break;
    /************
     * RULES SECTION * 
    *******************/
    #ifdef USE_MODULE_CORE_RULES
    case TASK_EVENT_SET_POWER_ID: 
      RulesEvent_Set_Power();
    break;
    #endif// USE_MODULE_CORE_RULES
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
    #endif // USE_MODULE_NETWORK_MQTT
  } // end switch

  return TASKER_RESULT__UNKNOWN_ID;

} // END function




// Updated 2025
void mRelays::SetLatchingRelay(power_t lpower, uint32_t state)
{
  // power xx00 - toggle REL1 (Off) and REL3 (Off) - device 1 Off, device 2 Off
  // power xx01 - toggle REL2 (On)  and REL3 (Off) - device 1 On,  device 2 Off
  // power xx10 - toggle REL1 (Off) and REL4 (On)  - device 1 Off, device 2 On
  // power xx11 - toggle REL2 (On)  and REL4 (On)  - device 1 On,  device 2 On
  static power_t latching_power = 0;     // Power state at latching start

  if (state && !tkr_set->runtime.latching_relay_pulse) {  // Set latching relay to power if previous pulse has finished
    latching_power = lpower;
    tkr_set->runtime.latching_relay_pulse = 2;            // max 200mS (initiated by stateloop())
  }

  for (uint32_t i = 0; i < rt.devices_present; i++) {
    uint32_t port = (i << 1) + ((latching_power >> i) &1);
          ALOG_INF(PSTR("DigitalWrite Pre %d"), 3);
    tkr_pins->DigitalWrite(GPIO_REL1 + port, bitRead(rt.bitpacked.rel_inverted, port) ? !state : state);
  }
}


// Updated 2025
void mRelays::SetDevicePower(power_t rpower, uint32_t source)
{
  if (tkr_set->runtime.power_on_delay) {
    rt.bitpacked.power_on_delay_state = rpower;
    return;
  }

  tkr_sup->ShowSource(source);
  tkr_set->runtime.last_source = source;

  if (POWER_ALL_ALWAYS_ON == tkr_set->Settings.poweronstate) {  // All on and stay on
    tkr_set->runtime.power = POWER_MASK >> (POWER_SIZE - rt.devices_present);
    rpower = tkr_set->runtime.power;
  }

  if (tkr_set->Settings.sysopt_system.bit.interlock) {          // Allow only one or no relay set - CMND_INTERLOCK - Enable/disable interlock
    for (uint32_t i = 0; i < MAX_INTERLOCKS; i++) {
      power_t mask = 1;
      uint32_t count = 0;
      for (uint32_t j = 0; j < rt.devices_present; j++) {
        if ((tkr_set->Settings.interlock[i] & mask) && (rpower & mask)) {
          count++;
        }
        mask <<= 1;
      }
      if (count > 1) {
        mask = ~tkr_set->Settings.interlock[i];    // Turn interlocked group off as there would be multiple relays on
        tkr_set->runtime.power &= mask;
        rpower &= mask;
      }
    }
  }

  if (rpower) {                           // Any power set
    rt.bitpacked.last_power = rpower;
  }

  tkr_events->XdrvMailbox.index = rpower;
  // XdrvXsnsCall(FUNC_SET_POWER);           // Signal power state

  tkr_events->XdrvMailbox.index = rpower;
  tkr_events->XdrvMailbox.payload = source;
  if (0){//XdrvCall(FUNC_SET_DEVICE_POWER)) {  // Set power state and stop if serviced
    // Serviced
  }
// #ifdef ESP8266
//   else if ((SONOFF_DUAL == tkr_set->runtime.module_type) || (CH4 == tkr_set->runtime.module_type)) {
//     Serial.write(0xA0);
//     Serial.write(0x04);
//     Serial.write(rpower &0xFF);
//     Serial.write(0xA1);
//     Serial.write('\n');
//     Serial.flush();
//   }
//   else if (EXS_RELAY == tkr_set->runtime.module_type) {
//     SetLatchingRelay(rpower, 1);
//   }
// #endif  // ESP8266
  else {
    uint32_t port = 0;
    uint32_t port_next;
    power_t bistable = 0;

    #ifdef ENABLE_FEATURE_POWER__ZERO_CROSS_DETECTION
    ZeroCrossMomentStart();
    #endif

    for (uint32_t i = 0; i < rt.devices_present; i++) {
      power_t state = rpower &1;

      port_next = 1;                              // Select next relay
      bool update = true;
      if (bitRead(rt.bitpacked.rel_bistable, port)) {
        if (tkr_set->Settings.flag6.bistable_single_pin) {  // SetOption152 - (Power) Use single pin bistable
          if (0x80000000 == tkr_set->runtime.power_latching) {
            tkr_set->runtime.power_latching = tkr_set->runtime.power;  // Init last known state
          }
          update = (bitRead(tkr_set->runtime.power_latching, port) != state);
          if (update) {
            bitWrite(tkr_set->runtime.power_latching, port, state);
            bitSet(bistable, port);
          }

        } else {
          if (!state) { port_next = 2; }          // Skip highest relay
          port += state;                          // Relay<lowest> = Off, Relay<highest> = On
        }
        state = 1;                                // Set pulse
      }
      if (update && (i < MAX_RELAYS_SET)) {        
        uint16_t gpio_pin = 0;
        if(bitRead(rt.bitpacked.rel_inverted, i))
        { //add the gpio mpin shift back in
          gpio_pin = GPIO_REL1_INV;          
        }else{
          gpio_pin = GPIO_REL1;
        }
        
        ALOG_INF(PSTR("DigitalWrite Pre %d"), 2);
        tkr_pins->DigitalWrite(gpio_pin +i, bitRead(rt.bitpacked.rel_inverted, i) ? !state : state);
        // tkr_pins->DigitalWrite(GPIO_REL1, port, bitRead(rt.bitpacked.rel_inverted, port) ? !state : state);
      }
      port += port_next;                          // Select next relay
      rpower >>= 1;                               // Select next power
    }
    
    #ifdef ENABLE_FEATURE_POWER__ZERO_CROSS_DETECTION
    ZeroCrossMomentEnd();
    #endif 

    // Reset bistable relay here to fix non-interlock situations due to fast switching
    if (rt.bitpacked.rel_bistable) {             // If bistable relays in the mix reset them after 40ms
      delay(tkr_set->Settings.setoption_255[P_BISTABLE_PULSE]);   // SetOption45 - Keep energized for about 5 x operation time
      for (uint32_t i = 0; i < port; i++) {       // Reset up to detected amount of ports
        if (bitRead(rt.bitpacked.rel_bistable, i)) {
          if (tkr_set->Settings.flag6.bistable_single_pin) {  // SetOption152 - (Power) Use single pin bistable
            if (!bitRead(bistable, i)) {
              continue;
            }
          }
          uint16_t gpio_pin = 0;
          if(bitRead(rt.bitpacked.rel_inverted, i))
          { //add the gpio mpin shift back in
            gpio_pin = GPIO_REL1_INV;          
          }else{
            gpio_pin = GPIO_REL1;
          }
          power_t state = rpower &1;
          ALOG_INF(PSTR("DigitalWrite Pre %d"), 1);
          tkr_pins->DigitalWrite(gpio_pin +i, bitRead(rt.bitpacked.rel_inverted, i) ? !state : state);
          // tkr_pins->DigitalWrite(GPIO_REL1, i, bitRead(rt.bitpacked.rel_inverted, i) ? 1 : 0);
          ALOG_INF(PSTR("Relay DigitalWrite %d, %d"), gpio_pin +i, bitRead(rt.bitpacked.rel_inverted, i) ? !state : state);
        }
      }
    }
  }

}

// Updated 2025
void mRelays::RestorePower(bool publish_power, uint32_t source)
{
  if (tkr_set->runtime.power != rt.bitpacked.last_power) {
    SetDevicePower(rt.bitpacked.last_power, source);
    if (publish_power) {
      mqtthandler_state_teleperiod.flags.SendNow = true;
      mqtthandler_state_ifchanged.flags.SendNow = true;
    }
  }
}

// Updated 2025
void mRelays::SetAllPower(uint32_t state, uint32_t source)
{
  // state 0 = POWER_OFF = Relay Off
  // state 1 = POWER_ON = Relay On (turn off after tkr_set->Settings.pulse_timer * 100 mSec if enabled)
  // state 2 = POWER_TOGGLE = Toggle relay
  // state 5 = POWER_OFF_FORCE = Relay Off even if locked
  // state 8 = POWER_OFF_NO_STATE = Relay Off and no publishPowerState
  // state 9 = POWER_ON_NO_STATE = Relay On and no publishPowerState
  // state 10 = POWER_TOGGLE_NO_STATE = Toggle relay and no publishPowerState
  // state 16 = POWER_SHOW_STATE = Show power state
  
  bool publish_power = true;
  if ((state >= POWER_OFF_NO_STATE) && (state <= POWER_TOGGLE_NO_STATE)) {
    state &= 3;                           // POWER_OFF, POWER_ON or POWER_TOGGLE
    publish_power = false;
  }
  if (((state >= POWER_OFF) && (state <= POWER_TOGGLE)) || (POWER_OFF_FORCE == state))  {
    power_t all_on = POWER_MASK >> (POWER_SIZE - rt.devices_present);
    switch (state) {
    case POWER_OFF:
      // Keep locked bits and set all other to 0
      tkr_set->runtime.power &= tkr_set->Settings.power_lock; 
      break;
    case POWER_ON:
      // Keep locked bits and set all other to 1
      tkr_set->runtime.power = (tkr_set->runtime.power & tkr_set->Settings.power_lock) | (all_on & ~tkr_set->Settings.power_lock);
      break;
    case POWER_TOGGLE:
      // Keep locked bits and toggle all other
      tkr_set->runtime.power ^= ~tkr_set->Settings.power_lock & all_on;
      break;
    case POWER_OFF_FORCE:
      // Set all off even if locked on (Used by overtemp and overcurrent)
      tkr_set->runtime.power = 0; 
      break;
    }
    SetDevicePower(tkr_set->runtime.power, source);
  }

  if (publish_power) {
    mqtthandler_state_teleperiod.flags.SendNow = true;
    mqtthandler_state_ifchanged.flags.SendNow = true;
  }
}

// Updated 2025
void mRelays::SetPowerOnState(void)
{
  
  if (POWER_ALL_ALWAYS_ON == tkr_set->Settings.poweronstate) {
    SetDevicePower(1, SRC_RESTART);
  } else {
    
    power_t devices_mask = POWER_MASK >> (POWER_SIZE - rt.devices_present);
    if (tkr_sup->ResetReasonPowerOn()){
      tkr_set->runtime.power_latching = 0;   // Single pin latching relay is powered off after re-applying power

      switch (tkr_set->Settings.poweronstate)
      {
      case POWER_ALL_OFF:
      case POWER_ALL_OFF_PULSETIME_ON:
        tkr_set->runtime.power = 0;
        SetDevicePower(tkr_set->runtime.power, SRC_RESTART);
        break;
      case POWER_ALL_ON:  // All on
        tkr_set->runtime.power = devices_mask;
        SetDevicePower(tkr_set->runtime.power, SRC_RESTART);
        break;
      case POWER_ALL_SAVED_TOGGLE:
        tkr_set->runtime.power = (tkr_set->Settings.power & devices_mask) ^ POWER_MASK;
        if (tkr_set->Settings.sysopt_system.bit.save_state) {  // SetOption0 - Save power state and use after restart
          SetDevicePower(tkr_set->runtime.power, SRC_RESTART);
        }
        break;
      case POWER_ALL_SAVED:
        tkr_set->runtime.power = tkr_set->Settings.power & devices_mask;
        if (tkr_set->Settings.sysopt_system.bit.save_state) {  // SetOption0 - Save power state and use after restart
          SetDevicePower(tkr_set->runtime.power, SRC_RESTART);
        }
        break;
      }

    } else {
      tkr_set->runtime.power = tkr_set->Settings.power & devices_mask;
      if (tkr_set->Settings.sysopt_system.bit.save_state) {    // SetOption0 - Save power state and use after restart
        SetDevicePower(tkr_set->runtime.power, SRC_RESTART);
      }
    }

    //  AddLog(LOG_LEVEL_DEBUG, PSTR("PWR: PowerOnState %d restored"), tkr_set->Settings.poweronstate);

    // TasIssue #526 and #909
    uint32_t port = 0;
    for (uint32_t i = 0; i < rt.devices_present; i++) {
      #ifdef ESP8266
      if (!tkr_set->Settings.flag3.no_power_feedback &&  // SetOption63 - Don't scan relay power state at restart - #5594 and #5663
          !tkr_set->runtime.power_on_delay          // SetOption47 - Delay switching relays to reduce power surge at power on
          #ifdef USE_SHUTTER
          && !tkr_set->Settings.flag3.shutter_mode       // SetOption80 - Enable shutter support
          #endif // USE_SHUTTER
        ) {
        if ((port < MAX_RELAYS) && tkr_pins->PinUsed(GPIO_REL1, port)) {
          if (bitRead(rt.bitpacked.rel_bistable, port)) {
            port++;                              // Skip both bistable relays as always 0
          } else {
            bitWrite(tkr_set->runtime.power, i, digitalRead(tkr_pins->Pin(GPIO_REL1, port)) ^ bitRead(rt.bitpacked.rel_inverted, port));
          }
        }
        port++;
      }
      #endif  // ESP8266
      if (bitRead(tkr_set->runtime.power, i) || (POWER_ALL_OFF_PULSETIME_ON == tkr_set->Settings.poweronstate)) {
        tkr_sup->SetPulseTimer(i % MAX_PULSETIMERS, tkr_set->Settings.pulse_timer[i % MAX_PULSETIMERS]);
      }
    }
      
  }

  rt.bitpacked.blink_powersave = tkr_set->runtime.power;
}



void mRelays::ExecuteCommandPower(uint32_t device, uint32_t state, uint32_t source)
{
// device  = Relay number 1 and up
// state 0 = POWER_OFF = Relay Off
// state 1 = POWER_ON = Relay On (turn off after Settings->pulse_timer * 100 mSec if enabled)
// state 2 = POWER_TOGGLE = Toggle relay
// state 3 = POWER_BLINK = Blink relay
// state 4 = POWER_BLINK_STOP = Stop blinking relay
// state 5 = POWER_OFF_FORCE = Relay off even if locked
// state 8 = POWER_OFF_NO_STATE = Relay Off and no publishPowerState
// state 9 = POWER_ON_NO_STATE = Relay On and no publishPowerState
// state 10 = POWER_TOGGLE_NO_STATE = Toggle relay and no publishPowerState
// state 16 = POWER_SHOW_STATE = Show power state

  tkr_sup->ShowSource(source);

//  if (1049 == LANGUAGE_LCID) { return; }

#ifdef ENABLE_DEVFEATURE_RESET_RELAY_DECOUNTER_WHEN_TURNED_OFF
    bool wasOn = (CommandGet_Relay_Power(device) != 0);
#endif

#ifdef USE_SONOFF_IFAN
  if (IsModuleIfan()) {
    tkr_set->runtime.blink_mask &= 1;       // No blinking on the fan relays
    Settings->flag.interlock = 0;        // No interlock mode as it is already done by the microcontroller - CMND_INTERLOCK - Enable/disable interlock
    Settings->pulse_timer[1] = 0;        // No pulsetimers on the fan relays
    Settings->pulse_timer[2] = 0;
    Settings->pulse_timer[3] = 0;
  }
#endif  // USE_SONOFF_IFAN

  bool force_power_off = false;
  if (POWER_OFF_FORCE == state) {
    force_power_off = true;
    state = POWER_OFF;
  }

  bool publish_power = true;
  if ((state >= POWER_OFF_NO_STATE) && (state <= POWER_TOGGLE_NO_STATE)) {
    state &= 3;                          // POWER_OFF, POWER_ON or POWER_TOGGLE
    publish_power = false;
  }

  // if ((device < 1) || (device > rt.devices_present)) { // Caused 0 and 1 commands wrong
  //   device = 1;
  // }
  tkr_set->runtime.active_device = device;

  if (!force_power_off && bitRead(tkr_set->Settings.power_lock, device -1)) {
    AddLog(LOG_LEVEL_INFO, PSTR("CMD: Power%d is LOCKED"), device);
    state = POWER_SHOW_STATE;            // Only show state. Make no change
  }

  if (state != POWER_SHOW_STATE) {
    tkr_sup->SetPulseTimer((device -1) % MAX_PULSETIMERS, 0);
  }

  static bool interlock_mutex = false;   // Interlock power command pending
  power_t mask = 1 << device; //(device -1);       // Device to control
  if (state <= POWER_TOGGLE) {
    if ((rt.bitpacked.blink_mask & mask)) {
      rt.bitpacked.blink_mask &= (POWER_MASK ^ mask);  // Clear device mask
      #ifdef ENABLE_DEVFEATURE_MQTT__PUBLUSH_TASMOTA_METHODS
      tkr_mqtt->MqttPublishPowerBlinkState(device);
      #endif
    }

    if (tkr_set->Settings.sysopt_system.bit.interlock &&      // CMND_INTERLOCK - Enable/disable interlock
        !interlock_mutex &&
        ((POWER_ON == state) || ((POWER_TOGGLE == state) && !(tkr_set->runtime.power & mask)))
       ) {
      interlock_mutex = true;            // Clear all but masked relay in interlock group if new set requested
      bool perform_interlock_delay = false;
      for (uint32_t i = 0; i < MAX_INTERLOCKS; i++) {
        if (tkr_set->Settings.interlock[i] & mask) {  // Find interlock group
          for (uint32_t j = 0; j < rt.devices_present; j++) {
            power_t imask = 1 << j;
            if ((tkr_set->Settings.interlock[i] & imask) && (tkr_set->runtime.power & imask) && (mask != imask)) {
              ExecuteCommandPower(j +1, POWER_OFF, SRC_IGNORE);
              perform_interlock_delay = true;
            }
          }
          break;                         // An interlocked relay is only present in one group so quit
        }
      }
      if (perform_interlock_delay) {
        delay(50);                       // Add some delay to make sure never have more than one relay on
      }
      interlock_mutex = false;
    }

#ifdef USE_DEVICE_GROUPS
    power_t old_power = tkr_set->runtime.power;
#endif  // USE_DEVICE_GROUPS
    switch (state) {
    case POWER_OFF: {
      tkr_set->runtime.power &= (POWER_MASK ^ mask);
      break; }
    case POWER_ON:
      tkr_set->runtime.power |= mask;
      break;
    case POWER_TOGGLE:
      tkr_set->runtime.power ^= mask;
      Serial.println("Toggle");
      Serial.println(tkr_set->runtime.power,BIN);
      ALOG_INF(PSTR("RLY Toggle"));
    }
#ifdef USE_DEVICE_GROUPS
    if (tkr_set->runtime.power != old_power && SRC_REMOTE != source && SRC_RETRY != source) {
      power_t dgr_power = tkr_set->runtime.power;
      if (Settings->flag4.multiple_device_groups) {  // SetOption88 - Enable relays in separate device groups
        dgr_power = (dgr_power >> (device - 1)) & 1;
      }
      SendDeviceGroupMessage(device, DGR_MSGTYP_UPDATE, DGR_ITEM_POWER, dgr_power);
    }
#endif  // USE_DEVICE_GROUPS
ALOG_INF(PSTR("before SetDeivePower %d,%d,%d %d"),device,state,source,tkr_set->runtime.power);
    SetDevicePower(tkr_set->runtime.power, source);
#ifdef USE_DOMOTICZ
    DomoticzUpdatePowerState(device);
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
    KnxUpdatePowerState(device, tkr_set->runtime.power);
#endif  // USE_KNX
    // if (publish_power && Settings->flag3.hass_tele_on_power) {  // SetOption59 - Send tele/%topic%/STATE in addition to stat/%topic%/RESULT
    //   MqttPublishTeleState();
    // }

    // Restart PulseTime if powered On
    tkr_sup->SetPulseTimer((device -1) % MAX_PULSETIMERS, (((POWER_ALL_OFF_PULSETIME_ON == tkr_set->Settings.poweronstate) ? ~tkr_set->runtime.power : tkr_set->runtime.power) & mask) ? tkr_set->Settings.pulse_timer[(device -1) % MAX_PULSETIMERS] : 0);
  }
  else if (POWER_BLINK == state) {
    if (!(rt.bitpacked.blink_mask & mask)) {
      rt.bitpacked.blink_powersave = (rt.bitpacked.blink_powersave & (POWER_MASK ^ mask)) | (tkr_set->runtime.power & mask);  // Save state
      rt.bitpacked.blink_power = (tkr_set->runtime.power >> (device -1))&1;  // Prep to Toggle
    }
    tkr_set->runtime.blink_timer = millis() + 100;
    tkr_set->runtime.blink_counter = ((!tkr_set->Settings.blinkcount) ? 64000 : (tkr_set->Settings.blinkcount *2)) +1;
    rt.bitpacked.blink_mask |= mask;    // Set device mask
    #ifdef ENABLE_DEVFEATURE_MQTT__PUBLUSH_TASMOTA_METHODS
    tkr_mqtt->MqttPublishPowerBlinkState(device);
    #endif
    return;
  }
  else if (POWER_BLINK_STOP == state) {
    bool flag = (rt.bitpacked.blink_mask & mask);
    rt.bitpacked.blink_mask &= (POWER_MASK ^ mask);  // Clear device mask
    #ifdef ENABLE_DEVFEATURE_MQTT__PUBLUSH_TASMOTA_METHODS
    tkr_mqtt->MqttPublishPowerBlinkState(device);
    #endif
    if (flag) {
      ExecuteCommandPower(device, (rt.bitpacked.blink_powersave >> (device -1))&1, SRC_IGNORE);  // Restore state
    }
    return;
  }
  if (publish_power) {
    mqtthandler_state_teleperiod.flags.SendNow = true;
    mqtthandler_state_ifchanged.flags.SendNow = true;
  }

  #ifdef ENABLE_DEVFEATURE_RESET_RELAY_DECOUNTER_WHEN_TURNED_OFF
    // Now check if the relay transitioned from on to off.
    bool isOff = (CommandGet_Relay_Power(device) == 0);
    if (wasOn && isOff) {
        ALOG_INF(PSTR("Relay %d turned from ON to OFF, resetting decounter."), device);
        CommandSet_Timer_Decounter(0, device);
    }
  #endif
  
}


void mRelays::StopAllPowerBlink(void)
{
  power_t mask;

  for (uint32_t i = 1; i <= rt.devices_present; i++) {
    mask = 1 << (i -1);
    if (rt.bitpacked.blink_mask & mask) {
      rt.bitpacked.blink_mask &= (POWER_MASK ^ mask);  // Clear device mask
      #ifdef ENABLE_DEVFEATURE_MQTT__PUBLUSH_TASMOTA_METHODS
      tkr_mqtt->MqttPublishPowerBlinkState(i);
      #endif
      ExecuteCommandPower(i, (rt.bitpacked.blink_powersave >> (i -1))&1, SRC_IGNORE);  // Restore state
    }
  }
}



//////////////// EVERYTHING BELOW HERE NEEDS UPDATING IN 2025 /////////////////////////////////////////////////////////////////////////////
//////////////// EVERYTHING BELOW HERE NEEDS UPDATING IN 2025 /////////////////////////////////////////////////////////////////////////////
//////////////// EVERYTHING BELOW HERE NEEDS UPDATING IN 2025 /////////////////////////////////////////////////////////////////////////////
//////////////// EVERYTHING BELOW HERE NEEDS UPDATING IN 2025 /////////////////////////////////////////////////////////////////////////////
//////////////// EVERYTHING BELOW HERE NEEDS UPDATING IN 2025 /////////////////////////////////////////////////////////////////////////////
//////////////// EVERYTHING BELOW HERE NEEDS UPDATING IN 2025 /////////////////////////////////////////////////////////////////////////////
//////////////// EVERYTHING BELOW HERE NEEDS UPDATING IN 2025 /////////////////////////////////////////////////////////////////////////////
//////////////// EVERYTHING BELOW HERE NEEDS UPDATING IN 2025 /////////////////////////////////////////////////////////////////////////////
//////////////// EVERYTHING BELOW HERE NEEDS UPDATING IN 2025 /////////////////////////////////////////////////////////////////////////////
//////////////// EVERYTHING BELOW HERE NEEDS UPDATING IN 2025 /////////////////////////////////////////////////////////////////////////////
//////////////// EVERYTHING BELOW HERE NEEDS UPDATING IN 2025 /////////////////////////////////////////////////////////////////////////////
//////////////// EVERYTHING BELOW HERE NEEDS UPDATING IN 2025 /////////////////////////////////////////////////////////////////////////////
//////////////// EVERYTHING BELOW HERE NEEDS UPDATING IN 2025 /////////////////////////////////////////////////////////////////////////////







#ifdef USE_MODULE_CORE_FILESYSTEM
#ifdef ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__DRIVERS___RELAYS

void mRelays::Save_Module()
{
  ALOG_INF(PSTR(D_LOG_RELAYS "Save_Module"));
  tkr_mfile->ByteFile_Save("/relays" FILE_EXTENSION_BIN, (uint8_t*)&rt, sizeof(rt));
}

void mRelays::Load_Module(bool erase)
{
  ALOG_INF(PSTR(D_LOG_RELAYS "Load_Module"));
  tkr_mfile->ByteFile_Load("/relays" FILE_EXTENSION_BIN, (uint8_t*)&rt, sizeof(rt));
}

#endif // ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__DRIVERS___RELAYS
#endif // USE_MODULE_CORE_FILESYSTEM


void mRelays::Pre_Init(void){
  
  module_state.mode = ModuleStatus::Initialising;
  module_state.devices = 0;

  // Lets check each type on their own, normal, inverted etc
  for(uint8_t driver_index=0; driver_index<MAX_RELAYS_SET; driver_index++)
  {
    if(tkr_pins->PinUsed(GPIO_REL1, driver_index))
    {
      uint8_t pin_number = tkr_pins->Pin(GPIO_REL1, driver_index);
      pinMode(pin_number, OUTPUT);
      rt.devices_present++;
      if(module_state.devices++ >= MAX_RELAYS_SET){ break; }
    }else
    if(tkr_pins->PinUsed(GPIO_REL1_INV, driver_index))
    {
      uint8_t pin_number = tkr_pins->Pin(GPIO_REL1_INV, driver_index);
      pinMode(pin_number, OUTPUT);
      bitSet(rt.bitpacked.rel_inverted, driver_index); //temp fix
      rt.devices_present++;
      if(module_state.devices++ >= MAX_RELAYS_SET){ break; }
    }
  }

  if(module_state.devices)
  {
    module_state.mode = ModuleStatus::Running;
  }

}


void mRelays::Init(void)
{

  // clear all settings to 0
  memset(&rt.relay_status, 0, sizeof(rt.relay_status));

  // Set defaults
  for(int relay_id=0;relay_id<MAX_RELAYS_SET;relay_id++){
    rt.relay_status[relay_id].timer_decounter.seconds = 0;
    rt.relay_status[relay_id].timer_decounter.active = false;
  }

  #ifdef ENABLE_DEVFEATURE_RELAY_ENABLE_TIME_WINDOW_LOCKS
  flags.enabled_relays_allowed_time_window_checks = true;
  #else
  rt.flags.enabled_relays_allowed_time_window_checks = false;
  #endif // ENABLE_DEVFEATURE_RELAY_ENABLE_TIME_WINDOW_LOCKS
  
  SetPowerOnState();
}


// void mRelays::BootMessage()
// {
//   #ifdef ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
//   char buffer[100];
//   if(module_state.devices)
//   {
//     mSupport::appendToBuffer(buffer, sizeof(buffer), "#%d ", bmp_count);  
//     char buffer2[50];
//     for(uint8_t sensor_id = 0; sensor_id<bmp_count; sensor_id++)
//     {      
//       mSupport::appendToBuffer(buffer, sizeof(buffer), "%s:\"%s\", ", bmp_sensors[sensor_id].bmp_name, DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), sensor_id, buffer2, sizeof(buffer2)));    
//     }
//   }
//   else{
//     mSupport::appendToBuffer(buffer, sizeof(buffer), "None");  
//   }
//   mSupport::removeTrailingComma(buffer);
//   ALOG_IMP(PSTR(D_LOG_BME "%s"), buffer);
//   #endif // ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
// }


void mRelays::EverySecond()
{

  // e.g. Resetting a device 
  SubTask_Relay_PulseOff();   // RELAY_MODE_PULSE_OFF ;;;;;;; PulseOff is part of CycleTimer
  // TimeOn, PowerCycleInterval, PowerCycleLength 
  SubTask_Relay_CycleTimer(); // to enable relay toggling every X minutes (eg floor mat)
  // Function is used to actually configure the output based on above functions
  SubTask_Relay_TimeOn(); // this function will simply check timeroff, timeron and time restriction. Other functions will set these values


}


void mRelays::SubTask_Relay_CycleTimer()
{

}



/*
1) MQTT control, including minutes on
2) Report relay status correctly
3) Button controls work, toggling
4) Save power state in settings
5) Restore power state from settings on boot
6) Enable mode to start with, on, off, as saved
7) Schedule automatic tod on/off, include duty cycle (with variable for scele, seconds, minutes, hours)

Time_On : Auto turn off after x seconds (or minutes)
Scheduled time, time on/off by a scheduled time, or,
Disable via internal setters (use flag to check or not) when outside permitted time windows

time_short = 4 bytes, on/off pair = 8 bytes, 4 periods = 32 bytes per relay, 4 relays = 128 bytes ram (passible)

add table (optional flag to turn this on/off)
Relay Name:    ontime, offtime, timeon, last controlled by//
  // I should add a relay "type" ie, external power, internal power, light

*/

void mRelays::SubTask_Every_Minute(){

  #ifdef ENABLE_DEVFEATURE_RELAY_ENABLE_TIME_WINDOW_LOCKS
    // Check if time is outside limits and set flag

  #endif

}


#ifdef USE_MODULE_CORE_RULES

void mRelays::RulesEvent_Set_Power(){

  // struct RELAY_EVENT_PARAMETERS{
  //   uint8_t index;
  //   uint8_t state;
  //   uint8_t state;
  // }rule_event_layout;


  ALOG_TST(PSTR("MATCHED RulesEvent_Set_Power"));

  uint8_t relay_index = tkr_rules->rules[tkr_rules->rules_active_index].command.device_id;
  uint8_t relay_state = tkr_rules->rules[tkr_rules->rules_active_index].command.value.data[0];

  ALOG_INF(PSTR("index %d"), relay_index);
  ALOG_INF(PSTR("state %d"), relay_state);

  #ifdef ENABLE_DEVFEATURE_RULES_COMMAND_CAN_USE_TRIGGER_VALUE // This probably needs moved into RulesEngine to work everywhere
  if(tkr_rules->rules[tkr_rules->rules_active_index].command.value.data[0] == STATE_NUMBER_FOLLOW_ID)
  {
    // Replace relay_state with event_triggered value
    ALOG_INF(PSTR(" tkr_rules->event_triggered.value.data[0] = %d"), tkr_rules->event_triggered.value.data[0]);
    relay_state = tkr_rules->event_triggered.value.data[0]; // ie relay will follow the button state
  }
  #endif //  ENABLE_DEVFEATURE_RULES_COMMAND_CAN_USE_TRIGGER_VALUE


  #ifdef ENABLE_DEVFEATURE_RELAY_RULEEVENT_USES_COMMANDSET
  CommandSet_Relay_Power(relay_state, relay_index);
  #else
  ExecuteCommandPower(relay_index, relay_state, SRC_IGNORE);
  #endif // ENABLE_DEVFEATURE_RELAY_RULEEVENT_USES_COMMANDSET

    
}
#endif // USE_MODULE_CORE_RULES


/**
 * @note: Time a relay will remain ON
 * */
void mRelays::SubTask_Relay_TimeOn(){
  
  // Stop if no relays connected
  if(!module_state.devices){
    return;
  }

  
  // Loop across each connected relay
  for(int relay_id=0;relay_id<module_state.devices;relay_id++){
    
    /**
     * Temporary timeoff will require blocking this command
     * */
    if(rt.relay_status[relay_id].timer_off_then_on_decounter.seconds>0) //if active, then stop this function
    {
      AddLog(LOG_LEVEL_WARNING, PSTR("SubTask_Relay_TimeOn BLOCKED by timeoffthenon"));
      return;     
    }

    //change seconds
    if(rt.relay_status[relay_id].time_seconds_on){
      rt.relay_status[relay_id].time_seconds_on++; // increment if positive, if 0, it doesnt increase
    }

    // Auto time off decounters
    if(rt.relay_status[relay_id].timer_decounter.seconds == 1){ //if =1 then turn off and clear to 0
      #ifdef ENABLE_LOG_LEVEL_COMMANDS
      ALOG_INF(PSTR(D_LOG_NEO "relay_status[%d].timer_decounter.seconds==1 and disable"), relay_id);
      #endif       

      CommandSet_Relay_Power(0, relay_id);

      rt.relay_status[relay_id].timer_decounter.seconds=0;

    }else
    if(rt.relay_status[relay_id].timer_decounter.seconds>1){ //if =1 then turn off and clear to 0
      rt.relay_status[relay_id].timer_decounter.seconds--; //decrease

      CommandSet_Relay_Power(1, relay_id);
      
      #ifdef ENABLE_LOG_LEVEL_COMMANDS
      ALOG_INF(PSTR(D_LOG_NEO "relay_status[%d].timer_decounter.seconds=%d dec"),relay_id, rt.relay_status[relay_id].timer_decounter.seconds);
      #endif

      mqtthandler_state_ifchanged.flags.SendNow = true; // If active, send every second

    }else{
      //assumed off ie == 0
    }
  }//end for

}

/**
 * @brief: Time a relay will turn off in seconds, then return to on again
 * */
void mRelays::SubTask_Relay_PulseOff(){
  
  // Stop if no relays connected
  if(!module_state.devices){
    return;
  }
  
  // Loop across each connected relay
  for(int relay_id=0;relay_id<module_state.devices;relay_id++){

    // Auto time off decounters
    if(rt.relay_status[relay_id].timer_off_then_on_decounter.seconds == 1){ //if =1 then turn off and clear to 0
      #ifdef ENABLE_LOG_LEVEL_COMMANDS
      ALOG_INF(PSTR(D_LOG_NEO "relay_status[%d].timer_off_then_on_decounter.seconds==1 and disable"), relay_id);
      #endif       

      CommandSet_Relay_Power(1, relay_id); // TURN ON

      rt.relay_status[relay_id].timer_off_then_on_decounter.seconds=0;

    }else
    if(rt.relay_status[relay_id].timer_off_then_on_decounter.seconds>1){ //if =1 then turn off and clear to 0
      rt.relay_status[relay_id].timer_off_then_on_decounter.seconds--; //decrease

      CommandSet_Relay_Power(0, relay_id); // TURN OFF
      
      #ifdef ENABLE_LOG_LEVEL_COMMANDS
      ALOG_INF(PSTR(D_LOG_NEO "relay_status[%d].timer_off_then_on_decounter.seconds=%d dec"),relay_id, rt.relay_status[relay_id].timer_off_then_on_decounter.seconds);
      #endif

      mqtthandler_state_ifchanged.flags.SendNow = true;

    }else{
      //assumed off ie == 0
    }
  }//end for

}





/*********************************************************************************************
* HELPER & CONVERSION FUNCTIONS ******************************************************************
*************************************************************************************************/

const char* mRelays::GetRelayNamebyIDCtr(uint8_t device_id, char* buffer, uint8_t buffer_length){
  DEBUG_LINE;
  if(device_id >= module_state.devices){ 
    AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_RELAYS "device_id >= module_state.devices %d %d"),device_id,module_state.devices);
    return PM_SEARCH_NOMATCH; 
  }
  DEBUG_LINE;
  // AddLog(LOG_LEVEL_INFO,PSTR("etRelayNamebyIDCtr"));
  // return DLI->GetDeviceNameWithEnumNumber(E M_MODULE_DRIVERS_RELAY_ID, device_id, buffer, buffer_length);
  return DLI->GetDeviceName_WithModuleUniqueID(GetModuleUniqueID(), device_id, buffer, buffer_length);
}


int8_t mRelays::GetRelayIDbyName(const char* c){
  if(*c=='\0'){ return -1; }  

  int16_t device_id_found = DLI->GetDeviceIDbyName(c, GetModuleUniqueID());

  AddLog(LOG_LEVEL_HIGHLIGHT,PSTR("\n\r\n\rdevice_id_found = %d"),device_id_found);

  // show options
  if(device_id_found == -1){
    // for(int ii=0;ii<tkr_set->GetDeviceNameCount(D_MODULE_DRIVERS_RELAY_ID);ii++){
    //   ALOG_INF(PSTR("GetDeviceIDbyName option #%d"),ii,tkr_set->GetDeviceIDbyName(c,tkr_set->Settings.device_name_buffer.name_buffer,&ii,&class_id));
    // }
    AddLog(LOG_LEVEL_INFO,PSTR("\n\r\n\nsearching=%s"),c);
    AddLog(LOG_LEVEL_INFO,PSTR("\n\r\n\name_buffer = %s"),tkr_set->Settings.device_name_buffer.name_buffer);


  }



  return device_id_found;

}


bool mRelays::IsRelayTimeWindowAllowed(uint8_t relay_id, uint8_t range_id){

  bool isenabled = false;

  //if range_id == 255, then check all, else check only it

  //check all, one for now
  // for(int range_id=0;range_id<D_SCHEDULED_ENABLED_TIME_PERIODS_AMOUNT;range_id++){
  //   if(
  //     tkr_time->CheckBetween_Day_DateTimesShort(
  //       &rt.relay_status[relay_id].enabled_ranges[range_id].ontime,
  //       &rt.relay_status[relay_id].enabled_ranges[range_id].offtime
  //     )
  //   ){
      isenabled = true;
  //   }
  // }


  return isenabled;

}





void mRelays::parse_JSONCommand(JsonParserObject obj)
{

  ALOG_DBM(PSTR("mRelays::parse_JSONCommand"));

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  JsonParserObject jobj = 0;
 
  int8_t relay_id= -1,state=-1;    //assume index 0 if none given

  if(jtok = obj[PM_POWERNAME]){
    if(jtok.isStr()){
      relay_id = GetRelayIDbyName(jtok.getStr());
    ALOG_INF( PSTR("relay_id = %s"), jtok.getStr() );
    }else 
    if(jtok.isNum()){
      relay_id  = jtok.getInt();
    }
    ALOG_INF( PSTR("relay_id = %d"), relay_id );
  }


  if(jtok = obj[PM_POWER_STATE]){
    if(jtok.isStr()){
      state = tkr_sup->GetStateNumber(jtok.getStr());
    }else 
    if(jtok.isNum()){
      state  = jtok.getInt();//tkr_sup->GetStateNumber(jtok.getInt());
    }

    /**
     * @brief If off, clear any timer decounters for relays
     **/
    if(state == 0)
    {
      CommandSet_Timer_Decounter(0, relay_id);
    }

  } 


  if(jtok = obj[PM_RELAY].getObject()[PM_TIME_ON]){
    CommandSet_Timer_Decounter(jtok.getInt(), relay_id);
  }else
  if(jtok = obj[PM_RELAY].getObject()[PM_TIME_ON_SECS]){
    CommandSet_Timer_Decounter(jtok.getInt(), relay_id);
  }else
  if(jtok = obj[PM_RELAY].getObject()[PM_TIME_ON_MINUTES]){
    CommandSet_Timer_Decounter(jtok.getInt()*60, relay_id);
  }


  if(jtok = obj[PM_RELAY].getObject()[PM_TIME_OFF_THEN_ON_SECS]){
    CommandSet_RelayAsRessetingDevice_TurnOffThenOnAgain(jtok.getInt(), relay_id);
  }


  if(IsWithinRange(state, 0,10) && IsWithinRange(relay_id, 0,module_state.devices)){
    CommandSet_Relay_Power(state,relay_id);
  }

	// Search for match which uses relayindex
	if(jtok = obj["RelayEnabled"]){
		SubCommandSet_EnabledTime(jtok.getObject());
	}

  char rule_name[20] = {0};
  for(uint8_t ii=0;ii<3;ii++){
    sprintf(rule_name, "RelayEnabled%d", ii);
      
		if(jtok = obj[rule_name]){
			ALOG_INF(PSTR("MATCHED %s"),rule_name);
			SubCommandSet_EnabledTime(jtok.getObject(), ii);
		}

  }  

}


void mRelays::SubCommandSet_EnabledTime(JsonParserObject jobj, uint8_t relay_id){

  JsonParserToken jtok;
  int16_t matched_id = 0;

		
  // if(jtok = obj["EnabledTime"]){
    time_short_t ontime;
    time_short_t offtime;
    uint8_t index = 0;
    
    if(jtok = jobj[PM_INDEX]){
      index = jtok.getInt();    
    }
    
    // if(jtok = jobj[PM_ONTIME]){
    //   ontime = mTime::Parse_Time_TimeShortCtr_To_TimeShort(jtok.getStr());
    //   rt.relay_status[relay_id].enabled_ranges[index].ontime = ontime;
    // }
    // if(jtok = jobj[PM_OFFTIME]){
    //   offtime = mTime::Parse_Time_TimeShortCtr_To_TimeShort(jtok.getStr());
    //   rt.relay_status[relay_id].enabled_ranges[index].offtime = offtime;
    // }
    // if(jtok = jobj[PM_ENABLED]){
    //   rt.relay_status[relay_id].enabled_ranges[index].enabled = jtok.getInt();
    // }


    // tkr_time->PrintDateTime(ontime);
    // tkr_time->PrintDateTime(offtime);


  // }


}




/*********************************************************************************************
* Input & Output Interface ******************************************************************
*************************************************************************************************/


  //flip args around so the single arg means state of relay 0
  // Only apply changes when state is changed
void mRelays::CommandSet_Relay_Power(uint8_t state, uint8_t num){

  ALOG_INF(PSTR(D_LOG_RELAYS D_FUNCTION_NAME_SVALUE " " D_COMMAND_NVALUE " " D_COMMAND_NVALUE), "CommandSet_Relay_Power","num",num,"state",state);  

	// Check state if it needs to toggle result

  /**
   * @brief Construct a new Modify State Number If Toggled object
   * 
   * Is this wrong, as ExecutePower toggles, so I might be double toggling
   * 
   */
  ModifyStateNumberIfToggled(&state, CommandGet_Relay_Power(num));
  
  //error patch  within vaible range
  if(!((num>=0)&&(num<=module_state.devices))){
    AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_RELAYS D_FUNCTION_NAME_SVALUE " Command Invalid"),"CommandSet_Relay_Power");
    return;
  }

  #ifdef ENABLE_DEVFEATURE_RELAY_ENABLE_TIME_WINDOW_LOCKS

    // AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_RELAYS "module_state.flags.enabled_relays_allowed_time_window_checks=%d"), module_state.flags.enabled_relays_allowed_time_window_checks);
    // AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_RELAYS "IsRelayTimeWindowAllowed(num)=%d"), IsRelayTimeWindowAllowed(num));
    
    /**
     * @note Only block relay from turning on, as off period may occur outside window
     * */
    if(state==1)
    {
      if(module_state.flags.enabled_relays_allowed_time_window_checks && tkr_time->RtcTime.valid)
      {  
        if(!IsRelayTimeWindowAllowed(num))
        {
          state = 0; // forcing off
          ALOG_INF(PSTR(D_LOG_RELAYS "Relay %d is locked: Outside enabled time window"), num);
          //   return;
        }
        else
        {
          ALOG_INF(PSTR(D_LOG_RELAYS "Relay %d is permitted: Inside enabled time window"), num);
        }
      }
    }
  #endif // ENABLE_DEVFEATURE_RELAY_DISABLING_SCHEDULE_CHECKS

  if(CommandGet_Relay_Power(num)==state){
    rt.relay_status[num].ischanged = false;
    return;
  }else{
    rt.relay_status[num].ischanged = true;
  }

  // relay_status[num].onoff = state;
  bitWrite(tkr_set->runtime.power, num, state);

  if(state){ 
    rt.relay_status[num].last.ontime = tkr_time->RtcTime; //create future "operators" to handle these conversions
    rt.relay_status[num].time_seconds_on = 1;
  }else{ 
    rt.relay_status[num].last.offtime = tkr_time->RtcTime; 
    rt.relay_status[num].time_seconds_on = 0; // Off 
  }

  ALOG_INF(PSTR("ExecuteCommandPower to be called"));

  ExecuteCommandPower(num,state,SRC_MQTT);

}

uint8_t mRelays::CommandGet_Relay_Power(uint8_t num){
  return bitRead(tkr_set->runtime.power, num);
}

uint32_t mRelays::CommandGet_SecondsRelayHasBeenOn(uint8_t relay_id) // why function, just use direct access (or place function into header)
{
  return rt.relay_status[relay_id].time_seconds_on;
}


/**********************************************************************************************
 *********************************************************************************************
  Parameter: TimerDecounter
 *********************************************************************************************
 ********************************************************************************************/

void mRelays::CommandSet_Timer_Decounter(uint16_t time_secs, uint8_t relay_id){
  rt.relay_status[relay_id].timer_decounter.seconds = time_secs;
  rt.relay_status[relay_id].timer_decounter.active = time_secs > 0 ? true : false;
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_RELAYS "Set" D_TIME "Relay%d " "%d" D_UNIT_SECOND), relay_id, rt.relay_status[relay_id].timer_decounter.seconds);  
  #endif
}

uint16_t mRelays::CommandGet_SecondsToRemainOn(uint8_t relay_id)
{
  // relay_status[relay_id].timer_decounter.seconds = time_secs;
  // relay_status[relay_id].timer_decounter.active = time_secs > 0 ? true : false;
  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_RELAYS "Set" D_TIME "Relay%d " "%d" D_UNIT_SECOND), relay_id, relay_status[relay_id].timer_decounter.seconds);  
  // #endif

  return rt.relay_status[relay_id].timer_decounter.seconds;
}


/**********************************************************************************************
 *********************************************************************************************
  Parameter: New function, to briefly command a relay turns off, then to turn it on again (ie when used to reset devices)
 *********************************************************************************************
 ********************************************************************************************/

void mRelays::CommandSet_RelayAsRessetingDevice_TurnOffThenOnAgain(uint16_t time_secs, uint8_t relay_id)
{
  // relay_status[relay_id].timer_decounter.seconds = time_secs;
  // relay_status[relay_id].timer_decounter.active = time_secs > 0 ? true : false;
  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_RELAYS "Set" D_TIME "Relay%d " "%d" D_UNIT_SECOND), relay_id, relay_status[relay_id].timer_decounter.seconds);  
  // #endif

  /**
   * Variable that is "off" peroid
   * Variable that will then turn it on again
   * */

  rt.relay_status[relay_id].timer_off_then_on_decounter.seconds = time_secs;
  rt.relay_status[relay_id].timer_off_then_on_decounter.active = time_secs > 0 ? true : false;
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_RELAYS "Set" D_TIME "Relay%d " "%d" D_UNIT_SECOND), relay_id, rt.relay_status[relay_id].timer_off_then_on_decounter.seconds);  
  #endif



  // return relay_status[relay_id].time_seconds_on;
}


uint16_t mRelays::CommandGet_SecondsToRemainOff(uint8_t relay_id)
{
  // relay_status[relay_id].timer_decounter.seconds = time_secs;
  // relay_status[relay_id].timer_decounter.active = time_secs > 0 ? true : false;
  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_RELAYS "Set" D_TIME "Relay%d " "%d" D_UNIT_SECOND), relay_id, relay_status[relay_id].timer_decounter.seconds);  
  // #endif

  return rt.relay_status[relay_id].timer_off_then_on_decounter.seconds;
}


// "PowerCycle" as a mode, should mean something like a PWM timer, so 50% would mean if the relay should be on for a total length of 1 hour, this value will mean how often it turns on and off ie PC=5, would mean 60/5=12 minutes on, 12 minutes off and repeat
void mRelays::CommandSet_PowerCycle_Interval()
{

// https://tasmota.github.io/docs/Commands/#control

// PulseTime<x>


}


// Needs a better name, but essentially for the floor mat. If I turn this on for 3 hours, the minutes set here will enable the relay to turn off and back on again after 55 minutes
void mRelays::CommandSet_PowerTimeOnLimit_Until_Reset()
{


  

}







uint8_t mRelays::ConstructJSON_Settings(uint8_t json_method, bool json_appending){

  JBI->Start();
    // JBI->Add(PM_DEVICES_CONNECTED, module_state.devices);

    // JBI->Array_Start_P(PSTR("rel_inverted"));
    //   for(int8_t bits=0; bits<sizeof(rt.rel_inverted)*8; bits++)
    //   {
    //     JBI->Add(bitRead(rt.rel_inverted,bits));
    //   }
    // JBI->Array_End();



  return JBI->End();

}


uint8_t mRelays::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[100];

  if(json_appending)
  {
    JBI->Start();
  }


  for(int device_id=0;device_id<module_state.devices;device_id++){
    if(rt.relay_status[device_id].ischanged||(json_level>JSON_LEVEL_IFCHANGED)){ rt.relay_status[device_id].ischanged=false;
      
      JBI->Object_Start(GetRelayNamebyIDCtr(device_id,buffer,sizeof(buffer)));
      //phase onoff out


        JBI->Add_P(PM_ONOFF,        CommandGet_Relay_Power(device_id));
        JBI->Add_P(PM_ONOFF_NAME,   CommandGet_Relay_Power(device_id)?"ON":"OFF");

        JBI->Add_P(PM_POWER_STATE,        CommandGet_Relay_Power(device_id));
        JBI->Add_P(PM_POWER_STATE_NAME,   CommandGet_Relay_Power(device_id)?"ON":"OFF");
        JBI->Add_P(PM_FRIENDLYNAME, GetRelayNamebyIDCtr(device_id,buffer,sizeof(buffer)));
        JBI->Add("TimerDeCounter", CommandGet_SecondsToRemainOn(device_id)); //Phase out
        JBI->Add("SecondsToRemainOn", CommandGet_SecondsToRemainOn(device_id));          
        JBI->Add("SecondsToRemainOff", CommandGet_SecondsRelayHasBeenOn(device_id));


        JBI->Level_Start_P(PM_LAST);
          snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", rt.relay_status[device_id].last.ontime.hour, rt.relay_status[device_id].last.ontime.minute, rt.relay_status[device_id].last.ontime.second);
          JBI->Add_P(PM_ONTIME, buffer);
          snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", rt.relay_status[device_id].last.offtime.hour, rt.relay_status[device_id].last.offtime.minute, rt.relay_status[device_id].last.offtime.second);
          JBI->Add_P(PM_OFFTIME, buffer);
        JBI->Object_End();
      
      // Schedules time periods the relays are allowed to run (or should this be locking off?)
      JBI->Level_Start_P("RelayEnabled");
      for(int ii=0;ii<D_SCHEDULED_ENABLED_TIME_PERIODS_AMOUNT;ii++){
        JBI->Level_Start_P("Range%d",ii);

          // time_short_t t = rt.relay_status[device_id].enabled_ranges[ii].ontime;
          // snprintf(buffer, sizeof(buffer), "%02dD%02d:%02d:%02d", t.Wday, t.hour, t.minute, t.second);
          // JBI->Add_P(PM_ONTIME, buffer);

          // t = rt.relay_status[device_id].enabled_ranges[ii].offtime;
          // snprintf(buffer, sizeof(buffer), "%02dD%02d:%02d:%02d", t.Wday, t.hour, t.minute, t.second);
          // JBI->Add_P(PM_OFFTIME, buffer);

          // Add if relay is within window etc here
          JBI->Add("IsRelayTimeWindowAllowed", IsRelayTimeWindowAllowed(device_id));

        JBI->Object_End();
      }
      JBI->Object_End();

      JBI->Object_End();
      
    }
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
uint8_t mRelays::AppendJSONResponse_Drivers_Unified()
{
  JBI->Level_Start_P(PM_MODULE_DRIVERS_RELAY_CTR);
    ConstructJSON_State(JSON_LEVEL_SHORT, false);
  JBI->Object_End();

  return 1; // fix crash
}


uint8_t mRelays::ConstructJSON_Scheduled(uint8_t json_level, bool json_appending)
{

  char buffer[50];

  if(json_appending)
  {
    JBI->Start();
  }
  
  for(int device_id=0;device_id<module_state.devices;device_id++){
    if(rt.relay_status[device_id].ischanged||(json_level>JSON_LEVEL_IFCHANGED)){ rt.relay_status[device_id].ischanged=false;
      
      JBI->Object_Start(GetRelayNamebyIDCtr(device_id,buffer,sizeof(buffer)));
        
        // JBI->Object_Start("scheduled");
        //   JBI->Add("enabled", rt.relay_status[device_id].scheduled[0].enabled);
        //   JBI->Array_Start("ontime");
        //     JBI->Add(mTime::ConvertShortTimetoCtr(&rt.relay_status[device_id].scheduled[0].ontime, buffer, sizeof(buffer)));
        //   JBI->Array_End();
        //   JBI->Array_Start("offtime");
        //     JBI->Add(mTime::ConvertShortTimetoCtr(&rt.relay_status[device_id].scheduled[0].offtime, buffer, sizeof(buffer)));
        //   JBI->Array_End();
        //   JBI->Add("days", rt.relay_status[device_id].scheduled[0].days_of_week_enabled_bitpacked);
        // JBI->Object_End();

        // JBI->Object_Start("enabled_ranges");
        //   JBI->Add("enabled", rt.relay_status[device_id].enabled_ranges[0].enabled);
        //   JBI->Array_Start("ontime");
        //     JBI->Add(mTime::ConvertShortTimetoCtr(&rt.relay_status[device_id].enabled_ranges[0].ontime, buffer, sizeof(buffer)));
        //   JBI->Array_End();
        //   JBI->Array_Start("offtime");
        //     JBI->Add(mTime::ConvertShortTimetoCtr(&rt.relay_status[device_id].enabled_ranges[0].offtime, buffer, sizeof(buffer)));
        //   JBI->Array_End();
        //   JBI->Add("IsRelayTimeWindowAllowed", IsRelayTimeWindowAllowed(device_id));
        // JBI->Object_End();

      JBI->Object_End();
      
    }
  }

  if(json_appending)
  {
    return JBI->End();
  }
  return JBI->Length();

}


#ifdef USE_MODULE_NETWORK_MQTT

void mRelays::MQTTHandler_Init()
{

  struct handler<mRelays>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetConfigPeriod(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mRelays::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetTelePeriod(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mRelays::ConstructJSON_State;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetIfChangedPeriod(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mRelays::ConstructJSON_State;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_scheduled_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetTelePeriod(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SCHEDULED_CTR;
  ptr->ConstructJSON_function = &mRelays::ConstructJSON_Scheduled;
  mqtthandler_list.push_back(ptr);

} 

#endif // USE_MODULE_NETWORK_MQTT


#endif

