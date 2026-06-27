#include "mLEDs.h"

#ifdef USE_MODULE_DRIVERS_LEDS

/*********************************************************************************************\
 * Set_PowerLED
 *
 * DESCRIPTION:
 *   Sets the current logical "Power LED" state.
 *
 *   The Power LED is not treated as a separate GPIO function. Instead, it is a logical role
 *   assigned to one of the configured LEDs in the `leds[]` array.
 *
 *   Example:
 *     system_led.power_index = 0  -> use leds[0] as the power LED
 *     system_led.power_index = 1  -> use leds[1] as the power LED
 *     system_led.power_index = 255 / LED_INDEX_NONE -> no power LED assigned
 *
 *   This keeps the physical LED configuration separate from the behavioural role:
 *
 *     GPIO template / pin mapping  -> defines which pins are LEDs
 *     leds[] array                 -> stores the configured LED resources
 *     system_led.power_index       -> selects which LED resource acts as the Power LED
 *
 *   This function is intended as a single-fire role command. It starts the requested effect
 *   on the assigned Power LED, but it does not itself perform recurring timing updates.
 *   Ongoing blink/pulse/on/off processing should be handled elsewhere by the normal LED
 *   update/task loop.
 *
 *   This function should only be used for an optional aggregate power indication. For example,
 *   on a Sonoff Basic with one built-in LED, the same LED may be used to show:
 *
 *     - System status patterns when WiFi/MQTT/AP/OTA/error conditions are active.
 *     - Aggregate power/relay state when the device is otherwise stable.
 *
 *   This function should not be used for per-relay LED mapping. For devices such as a Sonoff
 *   Pro, where each relay has its own LED, leave `system_led.power_index` as LED_INDEX_NONE
 *   and bind each relay state to a normal LED using relay-to-LED bindings or rules.
 *
 * BEHAVIOUR:
 *   state == true:
 *     Starts the ON effect on the configured Power LED.
 *
 *   state == false:
 *     Starts the OFF effect on the configured Power LED.
 *
 * SUPPRESSION BEHAVIOUR:
 *   If the configured Power LED is also the configured Status LED, and the Status LED is
 *   currently showing a system-status event, this function returns without changing the LED.
 *
 *   This prevents normal stable-state indication, such as relay/power state, from overwriting
 *   temporary system diagnostic patterns.
 *
 * SAFETY CHECKS:
 *   - If no Power LED is assigned, the function returns immediately.
 *   - If the configured index is outside the valid LED array range, it returns.
 *   - If the selected LED resource is not configured, it returns.
 *   - If the selected LED is currently reserved by the system-status handler, it returns.
 *
 * NOTES:
 *   - LED_INDEX_NONE should be a sentinel value, normally 255.
 *   - Internal LED indexing should remain zero-based.
 *   - User-facing configuration may call these LED1, LED2, etc., but internally they should
 *     map to leds[0], leds[1], etc.
 *   - Do not use physical GPIO numbers here. This function should only deal with LED indices.
 *   - Power and Status may point to the same physical LED index if required.
 *   - Status indication has priority over Power indication.
 *
 * INTENDED USE:
 *   Set_PowerLED(true);   // Power LED on
 *   Set_PowerLED(false);  // Power LED off
 *
\*********************************************************************************************/
void mLEDs::Set_PowerLED(bool state)
{
  const uint8_t index = system_led.power_index;

  if(index == LED_INDEX_NONE) { return; }
  if(index >= leds.size()) { return; }
  if(!leds[index].configured) { return; }

  if(SystemLED_IsStatusBusy(index)) { return; }

  if(state) {
    StartEffect_On(index);
  } else {
    StartEffect_Off(index);
  }
}


/*********************************************************************************************\
 * Set_StatusLED
 *
 * DESCRIPTION:
 *   Sets the current logical "Status LED" effect.
 *
 *   The Status LED is not treated as a separate GPIO function. Instead, it is a logical role
 *   assigned to one of the configured LEDs in the `leds[]` array.
 *
 *   Example:
 *     system_led.status_index = 0  -> use leds[0] as the status LED
 *     system_led.status_index = 1  -> use leds[1] as the status LED
 *     system_led.status_index = 255 / LED_INDEX_NONE -> no status LED assigned
 *
 *   This keeps system status signalling independent from physical pin mapping. The GPIO
 *   layer only needs to know which pins are LEDs. The LED module then decides which configured
 *   LED resource is used for power indication, status indication, network warning, MQTT
 *   warning, AP mode indication, OTA indication, etc.
 *
 *   This function is intended as a single-fire role command that starts a selected effect on
 *   the configured Status LED. It does not itself decide which system condition should be
 *   shown. That policy should be handled by `Handle_SystemLEDs()`, which resolves the current
 *   highest-priority system event and applies the corresponding status pattern.
 *
 * PARAMETERS:
 *   count:
 *     Number of blink or pulse events, depending on the selected effect.
 *
 *   interval:
 *     Timing interval for the effect, usually in milliseconds.
 *     For blink, this may represent the on/off interval.
 *     For pulse, this may represent the pulse step or cycle timing, depending on implementation.
 *
 *   event_pause:
 *     Pause between repeated blink/pulse event groups.
 *
 *   duration_secs:
 *     Optional effect duration in seconds. Exact meaning depends on StartEffect_* implementation.
 *     A value of 0 may be treated as indefinite if supported by the lower-level effect engine.
 *
 *   effect:
 *     LED_ON:
 *       Force the status LED on.
 *
 *     LED_OFF:
 *       Force the status LED off.
 *
 *     LED_BLINK:
 *       Start a blink effect using count, interval, event_pause, and duration_secs.
 *
 *     LED_PULSE:
 *       Start a pulse effect using count, interval, event_pause, and duration_secs.
 *
 * BEHAVIOUR:
 *   - The configured Status LED index is read from `system_led.status_index`.
 *   - If no status LED is configured, the function returns.
 *   - If the index is invalid, the function returns.
 *   - If the LED resource is not configured, the function returns.
 *   - The system status active flag is set before the effect is started.
 *   - The requested effect is then started on the selected LED.
 *
 * SUPPRESSION BEHAVIOUR:
 *   While the Status LED is active, normal power/relay/stable-state writes to the same LED
 *   should be suppressed by checking `SystemLED_IsStatusBusy(index)`.
 *
 *   This allows devices with one built-in LED, such as a Sonoff Basic, to use the same LED
 *   for both status patterns and stable relay/power indication:
 *
 *     - System status event active  -> show status pattern.
 *     - No system status event      -> allow normal power/relay indication.
 *
 * SAFETY CHECKS:
 *   - LED_INDEX_NONE disables the logical Status LED.
 *   - Invalid LED indices are ignored.
 *   - Unconfigured LED resources are ignored.
 *   - Unknown effects fall back to LED_OFF.
 *
 * NOTES:
 *   - Power and Status may point to the same physical LED index if required.
 *   - If Power and Status share an LED, status effects have priority over steady power
 *     indication.
 *   - This function should not directly query WiFi/MQTT/network state.
 *   - Use `SystemLED_SetEvent()` and `Handle_SystemLEDs()` for system-state policy.
 *   - Per-relay LEDs should not be handled here. Devices with one LED per relay should use
 *     relay-to-LED bindings/rules instead of the aggregate Power LED role.
 *
 * INTENDED USE:
 *   Set_StatusLED(0, 0, 0, 0, LED_ON);
 *   Set_StatusLED(0, 0, 0, 0, LED_OFF);
 *   Set_StatusLED(3, 100, 2000, 10, LED_BLINK);
 *   Set_StatusLED(1, 50, 5000, 0, LED_PULSE);
 *
\*********************************************************************************************/
void mLEDs::Set_StatusLED(
  uint8_t count,
  uint16_t interval,
  uint16_t event_pause,
  uint8_t duration_secs,
  LedEffect effect
)
{
  const uint8_t index = system_led.status_index;

  if(index == LED_INDEX_NONE) { return; }
  if(index >= leds.size()) { return; }
  if(!leds[index].configured) { return; }

  system_led.status_active = true;
  system_led.last_change_millis = millis();

  switch(effect)
  {
    case LED_ON:
      StartEffect_On(index);
      break;

    case LED_OFF:
      StartEffect_Off(index);
      break;

    case LED_BLINK:
      StartEffect_Blink(index, count, interval, event_pause, duration_secs);
      break;

    case LED_PULSE:
      StartEffect_Pulse(index, count, interval, event_pause, duration_secs);
      break;

    default:
      StartEffect_Off(index);
      break;
  }
}

/*********************************************************************************************\
 * SystemLED_SetEvent
 *
 * DESCRIPTION:
 *   Sets or clears a logical system LED event.
 *
 *   This function does not directly blink, pulse, or write to the LED. It only updates the
 *   active system event bitmask stored in `system_led.event_mask`.
 *
 *   `Handle_SystemLEDs()` should be called periodically, e.g. every 50 ms, and will resolve
 *   the highest-priority active event before applying the correct LED pattern.
 *
 * PARAMETERS:
 *   event_id:
 *     The system LED event to set or clear.
 *
 *   enabled:
 *     true  -> mark the event as active.
 *     false -> clear the event.
 *
 * BEHAVIOUR:
 *   - Invalid event IDs are ignored.
 *   - SYSTEM_LED_EVENT_NONE is ignored.
 *   - Active events are stored as bits in `system_led.event_mask`.
 *
 * EXAMPLE:
 *   SystemLED_SetEvent(SYSTEM_LED_EVENT_NO_NETWORK, true);
 *   SystemLED_SetEvent(SYSTEM_LED_EVENT_NO_NETWORK, false);
 *
\*********************************************************************************************/
void mLEDs::SystemLED_SetEvent(SystemLedEventId event_id, bool enabled)
{
  if(event_id == SYSTEM_LED_EVENT_NONE) { return; }
  if(event_id >= SYSTEM_LED_EVENT_MAX) { return; }

  const uint32_t mask = (1UL << event_id);

  if(enabled) {
    system_led.event_mask |= mask;
  } else {
    system_led.event_mask &= ~mask;
  }

  ALOG_INF("SystemLED_SetEvent: event_id=%u, enabled=%u, new_mask=0x%08X", event_id, enabled, system_led.event_mask);
}

/*********************************************************************************************\
 * SystemLED_IsStatusBusy
 *
 * DESCRIPTION:
 *   Checks whether a given LED index is currently reserved by the system-status LED handler.
 *
 *   This is the suppression check used by normal LED writers, such as Power LED or relay LED
 *   updates, to avoid overwriting an active system-status pattern.
 *
 *   Example:
 *     Sonoff Basic has one LED:
 *
 *       system_led.status_index = 0;
 *       system_led.power_index  = 0;
 *
 *     When a system event is active, e.g. NO_NETWORK, the status LED owns leds[0].
 *     Any stable-state write to leds[0], such as relay/power ON/OFF indication, should return
 *     until the system-status event clears.
 *
 * BEHAVIOUR:
 *   Returns true only when:
 *     - A valid Status LED is configured.
 *     - The requested index matches the configured Status LED index.
 *     - The Status LED handler is currently active.
 *
 *   Returns false when:
 *     - The requested index is LED_INDEX_NONE.
 *     - No Status LED is configured.
 *     - The requested index is not the Status LED.
 *     - No system-status event is currently active.
 *
 * NOTES:
 *   - This function does not check whether the LED resource itself is configured.
 *   - It only answers whether normal writes to this LED should be suppressed.
 *   - This should be called before normal/stable LED writes.
 *
 * INTENDED USE:
 *   if(SystemLED_IsStatusBusy(index)) { return; }
 *
\*********************************************************************************************/
bool mLEDs::SystemLED_IsStatusBusy(uint8_t index)
{
  if(index == LED_INDEX_NONE) { return false; }
  if(system_led.status_index == LED_INDEX_NONE) { return false; }

  if(index != system_led.status_index) { return false; }

  return system_led.status_active;
}

/*********************************************************************************************\
 * SystemLED_IsEventActive
 *
 * DESCRIPTION:
 *   Checks whether a specific system LED event is currently active in the system LED event
 *   bitmask.
 *
 *   Events are set and cleared using SystemLED_SetEvent(). This function is used internally
 *   by Handle_SystemLEDs() and SystemLED_GetHighestPriorityEvent() to decide which system
 *   condition should currently be displayed.
 *
 * PARAMETERS:
 *   event_id:
 *     System LED event ID to check.
 *
 * RETURNS:
 *   true:
 *     The event is valid and currently active.
 *
 *   false:
 *     The event is invalid, SYSTEM_LED_EVENT_NONE, or not active.
 *
\*********************************************************************************************/
bool mLEDs::SystemLED_IsEventActive(SystemLedEventId event_id)
{
  if(event_id == SYSTEM_LED_EVENT_NONE) { return false; }
  if(event_id >= SYSTEM_LED_EVENT_MAX) { return false; }

  return (system_led.event_mask & (1UL << event_id));
}

/*********************************************************************************************\
 * SystemLED_GetHighestPriorityEvent
 *
 * DESCRIPTION:
 *   Resolves the highest-priority currently active system LED event.
 *
 *   Only one system status pattern is shown at a time. If multiple events are active, the
 *   event listed highest in this function wins.
 *
 * PRIORITY ORDER:
 *   1. OTA active
 *   2. AP mode active
 *   3. No network
 *   4. No MQTT
 *   5. Error
 *
 * NOTES:
 *   - This function only resolves priority.
 *   - It does not start or stop LED effects.
 *   - Pattern application is handled by SystemLED_ApplyEventPattern().
 *
 * RETURNS:
 *   Highest-priority active SystemLedEventId, or SYSTEM_LED_EVENT_NONE if no event is active.
 *
\*********************************************************************************************/
SystemLedEventId mLEDs::SystemLED_GetHighestPriorityEvent(void)
{
  if(SystemLED_IsEventActive(SYSTEM_LED_EVENT_OTA_ACTIVE)) {
    return SYSTEM_LED_EVENT_OTA_ACTIVE;
  }

  if(SystemLED_IsEventActive(SYSTEM_LED_EVENT_AP_MODE)) {
    return SYSTEM_LED_EVENT_AP_MODE;
  }

  if(SystemLED_IsEventActive(SYSTEM_LED_EVENT_NO_NETWORK)) {
    return SYSTEM_LED_EVENT_NO_NETWORK;
  }

  if(SystemLED_IsEventActive(SYSTEM_LED_EVENT_NO_MQTT)) {
    return SYSTEM_LED_EVENT_NO_MQTT;
  }

  if(SystemLED_IsEventActive(SYSTEM_LED_EVENT_ERROR)) {
    return SYSTEM_LED_EVENT_ERROR;
  }

  return SYSTEM_LED_EVENT_NONE;
}

/*********************************************************************************************\
 * SystemLED_ApplyEventPattern
 *
 * DESCRIPTION:
 *   Applies the LED effect associated with a specific system LED event.
 *
 *   This function is called only when the highest-priority active event changes. This prevents
 *   the same effect being restarted every time Handle_SystemLEDs() runs.
 *
 * HARD-CODED PATTERNS:
 *   SYSTEM_LED_EVENT_AP_MODE:
 *     Double fast blink every 10 seconds.
 *
 *     Implemented as:
 *       count       = 2
 *       interval    = 100 ms
 *       event_pause = 10000 ms
 *       duration    = 0, indefinite
 *
 *   SYSTEM_LED_EVENT_NO_NETWORK:
 *     Fast repeating blink pattern while network is unavailable.
 *
 *   SYSTEM_LED_EVENT_NO_MQTT:
 *     Triple blink pattern while network is available but MQTT is unavailable.
 *
 *   SYSTEM_LED_EVENT_OTA_ACTIVE:
 *     Pulse while OTA is active.
 *
 *   SYSTEM_LED_EVENT_ERROR:
 *     Fast repeated warning blink.
 *
 * PARAMETERS:
 *   event_id:
 *     Event whose pattern should be started.
 *
 * NOTES:
 *   - This function does not decide priority.
 *   - This function does not check the event mask.
 *   - This function assumes Handle_SystemLEDs() has already selected the event to display.
 *
\*********************************************************************************************/
void mLEDs::SystemLED_ApplyEventPattern(SystemLedEventId event_id)
{
  const uint8_t index = system_led.status_index;

  if(index == LED_INDEX_NONE) { return; }
  if(index >= leds.size()) { return; }
  if(!leds[index].configured) { return; }

  switch(event_id)
  {
    case SYSTEM_LED_EVENT_OTA_ACTIVE:
      StartEffect_Pulse(index, 1, 50, 0, 0);
      break;

    case SYSTEM_LED_EVENT_AP_MODE:
      // AP mode active:
      // double fast blink every 10 seconds.
      StartEffect_Blink(index, 2, 100, 10000, 0);
      break;

    case SYSTEM_LED_EVENT_NO_NETWORK:
      StartEffect_Blink(index, 1, 250, 1000, 0);
      break;

    case SYSTEM_LED_EVENT_NO_MQTT:
      StartEffect_Blink(index, 3, 150, 2000, 0);
      break;

    case SYSTEM_LED_EVENT_ERROR:
      StartEffect_Blink(index, 5, 100, 1500, 0);
      break;

    case SYSTEM_LED_EVENT_NONE:
    default:
      StartEffect_Off(index);
      break;
  }
}

/*********************************************************************************************\
 * SystemLED_ReleaseStatusLED
 *
 * DESCRIPTION:
 *   Releases the system status LED after all system status events have cleared.
 *
 *   While `system_led.status_active` is true, normal LED writes to the configured status LED
 *   should be suppressed. This prevents relay/power/stable-state indication from overwriting
 *   active diagnostic patterns.
 *
 *   Once no system LED events remain active, this function clears the active status state and
 *   turns the status LED off. After this, stable-state handlers such as Set_PowerLED() can
 *   reassert the normal LED state.
 *
 * NOTES:
 *   - This function does not clear event bits.
 *   - Event bits are set/cleared using SystemLED_SetEvent().
 *   - This function only releases the status LED runtime state.
 *
\*********************************************************************************************/
void mLEDs::SystemLED_ReleaseStatusLED(void)
{
  system_led.status_active = false;
  system_led.current_event = SYSTEM_LED_EVENT_NONE;
  system_led.last_change_millis = millis();

  const uint8_t index = system_led.status_index;

  if(index == LED_INDEX_NONE) { return; }
  if(index >= leds.size()) { return; }
  if(!leds[index].configured) { return; }

  StartEffect_Off(index);
}

/*********************************************************************************************\
 * SystemLED_HandlePowerLED
 *
 * DESCRIPTION:
 *   Handles the optional aggregate Power LED when no system-status event is currently active.
 *
 *   This is intended for devices with a single general-purpose LED, such as a Sonoff Basic.
 *   In that case, the same LED may be used for:
 *
 *     - Status LED patterns during WiFi/MQTT/AP/OTA/error conditions.
 *     - Aggregate relay/power indication when the device is stable.
 *
 * IMPORTANT DEVICE EDGE CASE:
 *   This function is only for an optional aggregate power LED. It should not handle per-relay
 *   LED mapping.
 *
 *   For devices such as Sonoff Pro, where each relay has its own LED, leave
 *   `system_led.power_index` as LED_INDEX_NONE and bind each relay state to a normal LED
 *   through relay-to-LED bindings or rules.
 *
 * BEHAVIOUR:
 *   - If no aggregate Power LED is configured, returns.
 *   - If the Power LED is also the active Status LED, returns while status is busy.
 *   - Otherwise writes the aggregate power state.
 *
 * TODO:
 *   Replace the hard-coded `any_power_on` placeholder with the real relay/power state source.
 *
\*********************************************************************************************/
void mLEDs::SystemLED_HandlePowerLED(void)
{
  const uint8_t index = system_led.power_index;

  if(index == LED_INDEX_NONE) { return; }
  if(index >= leds.size()) { return; }
  if(!leds[index].configured) { return; }

  if(SystemLED_IsStatusBusy(index)) {
    return;
  }

  // TODO: Replace with real relay/power-state query.
  // Example:
  // const bool any_power_on = tkr_relay->AnyRelayOn();
  const bool any_power_on = false;

  if(any_power_on) {
    StartEffect_On(index);
  } else {
    StartEffect_Off(index);
  }
}

/*********************************************************************************************\
 * Handle_SystemLEDs
 *
 * DESCRIPTION:
 *   Main periodic system LED handler.
 *
 *   This should be called regularly, for example every 50 ms.
 *
 *   The function resolves which system LED event should be displayed, based on the active
 *   event bitmask and fixed priority order.
 *
 *   Only the highest-priority active event is shown. Lower-priority events remain stored in
 *   the event mask but are not displayed until higher-priority events clear.
 *
 * BEHAVIOUR:
 *   1. Resolve highest-priority active event.
 *   2. If no event is active:
 *        - Release the status LED if it was previously active.
 *        - Allow optional aggregate Power LED handling.
 *        - Return.
 *
 *   3. If an event is active:
 *        - Mark the status LED as active.
 *        - If the event changed since last time, apply the new event pattern.
 *        - Suppress normal stable-state writes to the same LED while active.
 *
 * STATUS SUPPRESSION:
 *   While `system_led.status_active` is true, normal writes to the configured Status LED
 *   should be blocked by SystemLED_IsStatusBusy(index).
 *
 *   This is required for devices such as Sonoff Basic where the same LED may be used for
 *   both system status patterns and normal relay/power indication.
 *
 * HARD-CODED STATUS EXAMPLE:
 *   If AP mode is active, the selected status LED will show:
 *
 *     double fast blink every 10 seconds
 *
 *   This pattern is started inside SystemLED_ApplyEventPattern().
 *
\*********************************************************************************************/
void mLEDs::Handle_SystemLEDs(void)
{
  const SystemLedEventId highest_event = SystemLED_GetHighestPriorityEvent();

  if(highest_event == SYSTEM_LED_EVENT_NONE)
  {
    if(system_led.status_active) {
      SystemLED_ReleaseStatusLED();
    }

    SystemLED_HandlePowerLED();
    return;
  }

  system_led.status_active = true;

  if(highest_event != system_led.current_event)
  {
    system_led.current_event = highest_event;
    system_led.last_change_millis = millis();

    SystemLED_ApplyEventPattern(highest_event);
  }
}


/**
 * @brief Status LED 
 * 
 * For each LED configured as status type, this function will read all status flags across the systems and make sure the LEDs are configured to show.
 * This is a interface of sorts.
 * 
 */
void mLEDs::Config_StatusEffect()
{
  
  DEBUG_LINE;
  uint8_t blinkinterval = 1;
  
  if (tkr_set->runtime.global_state.data) {                              // Any problem
    if (tkr_set->runtime.global_state.mqtt_down) { blinkinterval = 7; }  // MQTT problem so blink every 2 seconds (slowest)
    if (tkr_set->runtime.global_state.wifi_down) { blinkinterval = 3; }  // Wifi problem so blink every second (slow)
    tkr_set->runtime.blinks = 201;                                       // Allow only a single blink in case the problem is solved
  }
  


}










#endif