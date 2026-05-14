/*
  mPZEM004T.h - PZEM004T v3 MODBUS

  Copyright (C) 2021  Michael

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef HEADER_MODULE_DRIVERS_LEDS_H
#define HEADER_MODULE_DRIVERS_LEDS_H
  
#define D_UNIQUE_MODULE_DRIVERS_LEDS_ID 4003 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DRIVERS_LEDS

#include <Arduino.h>
#include <vector>

#include "1_TaskerManager/mTaskerInterface.h"

// enum LedMode{
//   LED_MANUAL=0,
//   LED_SHOW_POWER,
//   LED_SHOW_STATUS, // Network, plus others
// };

/*********************************************************************************************\
 * System LED Event Macros
 *
 * DESCRIPTION:
 *   These macros provide simple cross-module hooks for setting or clearing system LED events.
 *
 *   Modules such as WiFi, Network, MQTT, OTA, and WebServer should not directly call
 *   StartEffect_Blink(), StartEffect_Pulse(), etc. Instead, they should only set/clear logical
 *   system events.
 *
 *   The LED module then resolves the highest-priority active event inside Handle_SystemLEDs()
 *   and applies the correct pattern to the configured system status LED.
 *
 *   If USE_MODULE_DRIVERS_LEDS is not enabled, these macros compile to nothing.
 *
 * EXAMPLE:
 *   SET_SYSTEM_LED__NO_NETWORK(true);   // WiFi/network not connected yet
 *   SET_SYSTEM_LED__NO_NETWORK(false);  // WiFi/network connected
 *
\*********************************************************************************************/
#ifdef USE_MODULE_DRIVERS_LEDS

  #define SET_SYSTEM_LED__NO_NETWORK(state) \
    tkr_led->SystemLED_SetEvent(SYSTEM_LED_EVENT_NO_NETWORK, state)

  #define SET_SYSTEM_LED__NO_MQTT(state) \
    tkr_led->SystemLED_SetEvent(SYSTEM_LED_EVENT_NO_MQTT, state)

  #define SET_SYSTEM_LED__AP_MODE(state) \
    tkr_led->SystemLED_SetEvent(SYSTEM_LED_EVENT_AP_MODE, state)

  #define SET_SYSTEM_LED__OTA_ACTIVE(state) \
    tkr_led->SystemLED_SetEvent(SYSTEM_LED_EVENT_OTA_ACTIVE, state)

  #define SET_SYSTEM_LED__ERROR(state) \
    tkr_led->SystemLED_SetEvent(SYSTEM_LED_EVENT_ERROR, state)

#else

  #define SET_SYSTEM_LED__NO_NETWORK(state)
  #define SET_SYSTEM_LED__NO_MQTT(state)
  #define SET_SYSTEM_LED__AP_MODE(state)
  #define SET_SYSTEM_LED__OTA_ACTIVE(state)
  #define SET_SYSTEM_LED__ERROR(state)

#endif







enum LedEffect {
  LED_OFF,
  LED_ON,
  LED_BLINK,
  LED_PULSE
};

typedef union {
  uint8_t data;
  struct {
    uint8_t network_down : 1;  // No wifi/ethernet connect
    uint8_t mqtt_down : 1;     // No mqtt connect
    uint8_t wifi_ap_active : 1;       // In AP mode, waiting config
    uint8_t show_power : 1;
    uint8_t spare04 : 1;
    uint8_t spare05 : 1;
    uint8_t slave : 1;     // Controlled by another task
    uint8_t manual : 1;    // Direct control
  };
} LedMode_BitField;

struct LedState {
  bool configured = false;
  bool inverted = false;
  int16_t pin = -1;

  LedEffect effect = LED_OFF;
  uint8_t state = LOW;
  uint8_t count = 0;
  uint16_t period = 0;
  uint16_t groupPause = 0;
  uint8_t duration_secs = 0;
  uint32_t lastUpdateTime = 0;
  uint32_t startTime = 0;
  LedMode_BitField mode = {0};
};

#define LED_INDEX_NONE 255

enum SystemLedEventId : uint8_t
{
  SYSTEM_LED_EVENT_NONE = 0,

  SYSTEM_LED_EVENT_OTA_ACTIVE,
  SYSTEM_LED_EVENT_AP_MODE,
  SYSTEM_LED_EVENT_NO_NETWORK,
  SYSTEM_LED_EVENT_NO_MQTT,
  SYSTEM_LED_EVENT_ERROR,

  SYSTEM_LED_EVENT_MAX
};
struct LedRoleConfig {
  uint8_t power_index  = LED_INDEX_NONE;
  uint8_t status_index = LED_INDEX_NONE;
};


class mLEDs :
  public mTaskerInterface
{

  private:
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mLEDs(){};
    void Pre_Init(void);
    void Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void BootMessage();
    
    static constexpr const char* PM_MODULE_DRIVERS_LEDS_CTR = D_MODULE_DRIVERS_LEDS_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DRIVERS_LEDS_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS_LEDS_ID; }

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/
    
    void SetInvertFlag(uint8_t b);
    bool IsUsed(uint8_t index);
    void SetUsed(uint8_t index);


    std::vector<LedState> leds;  // Vector of LedState structs to track each LED
    void Refresh_LED(uint8_t index); // Updates a specific LED by index
    void Refresh_AllLEDs();          // Loops through all LEDs and updates them

    void StartEffect_On(uint8_t index);  // Set specific LED to ON mode
    void StartEffect_Off(uint8_t index); // Set specific LED to OFF mode
    void StartEffect_Blink(uint8_t index, uint8_t blinkCount, uint16_t blinkInterval, uint16_t blinkGroupPause, uint8_t duration_secs = 0);
    void StartEffect_Pulse(uint8_t index, uint8_t pulseCount, uint16_t period, uint16_t groupPause, uint8_t duration_secs = 0) ;

    void UpdateLEDEffect(uint8_t led_index);  // Helper function to update an individual LED

    uint8_t UsedCount();

    bool LED_PinIsValid(int16_t pin);
    void LED_Write(uint8_t index, bool on);
    void LED_PWMWrite(uint8_t index, uint8_t duty);

    void Set_PowerLED(bool state);
    void Set_StatusLED(uint8_t count, uint16_t interval, uint16_t event_pause, uint8_t duration_secs, LedEffect effect);

    void Config_StatusEffect();

    uint16_t GetLED_Output(uint8_t index);

    #define MODULE_LEDS_MAX 4

    /**
     * @brief Bitpacked LEDs
     * 
     */
    uint8_t leds_present = 0;                   // Max number of LED supported
    uint8_t inverted_bitmask = 0;                   // LED inverted flag (1 = (0 = On, 1 = Off))
    uint8_t led_power = 0;                      // LED power state
    uint8_t ledlnk_inverted = 0;                // Link LED inverted flag (1 = (0 = On, 1 = Off))
    uint8_t used_bitmask = 0; 

    struct SYSTEM_LED_t
    {
      // Optional aggregate power LED.
      // Used for devices with one general system LED, e.g. Sonoff Basic.
      // Leave as LED_INDEX_NONE when relay-specific LEDs are used instead.
      uint8_t power_index = LED_INDEX_NONE;

      // Optional system/status LED.
      // Used for WiFi/MQTT/AP/OTA/error indication.
      // When active, this LED suppresses normal relay/power writes to the same LED.
      uint8_t status_index = LED_INDEX_NONE;

      // Bitmask of active system LED events.
      // Each bit corresponds to one SystemLedEventId.
      uint32_t event_mask = 0;

      // Currently displayed highest-priority system event.
      // Used to avoid restarting the same effect every 50 ms.
      SystemLedEventId current_event = SYSTEM_LED_EVENT_NONE;

      // True when the status LED is actively showing a system event.
      // Normal LED writes to the same LED should return while this is true.
      bool status_active = false;

      // Time when the current system LED event was last changed.
      // Useful for future debounce, minimum display time, or timeout logic.
      uint32_t last_change_millis = 0;
    };

    SYSTEM_LED_t system_led;
    void SystemLED_SetEvent(SystemLedEventId event_id, bool enabled);
    bool SystemLED_IsStatusBusy(uint8_t index);
    bool SystemLED_IsEventActive(SystemLedEventId event_id);
    SystemLedEventId SystemLED_GetHighestPriorityEvent(void);
    void SystemLED_ApplyEventPattern(SystemLedEventId event_id);
    void SystemLED_ReleaseStatusLED(void);
    void SystemLED_HandlePowerLED(void);
    void Handle_SystemLEDs(void);


    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/


    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/
    
    void parse_JSONCommand(JsonParserObject obj);

    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/
    
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_method = 0, bool json_appending = true); //default is true
    
    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    std::vector<struct handler<mLEDs>*> mqtthandler_list;    
    struct handler<mLEDs> mqtthandler_settings;
    struct handler<mLEDs> mqtthandler_state_ifchanged;
    #endif // USE_MODULE_NETWORK_MQTT 


    
};

#endif // USE_MODULE_DRIVERS_LEDS

#endif