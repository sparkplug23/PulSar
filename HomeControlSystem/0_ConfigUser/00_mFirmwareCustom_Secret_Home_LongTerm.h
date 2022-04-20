#ifndef MFIRMWARECUSTOM_SECRET_HOME_LONGTERM_H
#define MFIRMWARECUSTOM_SECRET_HOME_LONGTERM_H


/*********************************************************************************************\
 * Personal configs for installed home devices as of 2022 -- getting ready for leaving home
\*********************************************************************************************/

#define USE_MODULE_NETWORKS_MQTT
// #define DEVICE_FORCED_TO_BE_TESTER
// #define ENABLE_TESTUSER
// #define DISABLE_WEBSERVER
//#define FORCE_TEMPLATE_LOADING
#define USE_MODULE_CORE_RULES

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/Languages/mLanguageDefault.h"
#include "2_CoreSystem/HardwareTemplates/mHardwareTemplates.h"


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Enable Defines *******************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/
/**
 * Generic devices ie those configured via `ini` configs
 */
// #define DEVICE_SOCKET_NUMBERED
// #define DEVICE_SOCKET_NUMBERED_WITH_SERIAL_GPIO_BUTTON



/**
Kitchen/Dining
  - kitchensensor = pir, bme
  - rgbcooker = h801
  - rgbfridge - h801
  - shelf1
  - crystal3
**/
// #define DEVICE_KITCHENLIGHT1
// #define DEVICE_KITCHENLIGHT2
// #define DEVICE_KITCHENLIGHT3
// #define DEVICE_KITCHENLIGHT4
// #define DEVICE_RGBSHELF
// #define DEVICE_RGBCRYSTAL3
// #define DEVICE_RGBCOOKER
// #define DEVICE_KITCHENSENSOR
// #define DEVICE_RGBFRIDGE

/**
Utility
  - crystal1
  - pir, bme, door sensor
**/
// #define DEVICE_RGBUTILITY
// #define DEVICE_UTILITYSENSOR
// #define DEVICE_RGBCRYSTAL1

/**
DSToilet
  - consumerunit = 12 pzem, bme
*/
// #define DEVICE_CONSUMERUNIT

/**
LivingRoom
  - 3 optional sockets for the corners
  - sensor = pir inside, bme..... ultrasonic, outside pir
**/
// #define DEVICE_LIVINGROOMSENSOR
// #define DEVICE_RGBFIREPLACE

/**
 * 
Hallway
  - radiatorfan = fan controls, 2 db, bme, dht, pir
  - front door/table socket
  - hallway table
 */
// #define DEVICE_RADIATORFAN
// #define DEVICE_SHELLYDIMMER_HALLWAY_LAMP 
// #define DEVICE_CANDLE_ELECTRIC_HALLWAY  // Socket_SocketNumber16_Power

/**
 * 
Understairs
  - heating
 */
// #define DEVICE_HEATING_ESP32



/**
Outside
  - CeilingLight
**/
// #define DEVICE_SIDEDOORLIGHT
// #define DEVICE_GAZEBO_SONOFF_4CHPRO
// #define DEVICE_GAZEBO_SENSOR
// #define DEVICE_H801_RGBGAZEBO
// #define DEVICE_STRING_ROSEBUD

/**
Garage
 **/
// #define DEVICE_OILTANK
// #define DEVICE_OILFURNACE
// #define DEVICE_GARAGELIGHT
// #define DEVICE_TREADMILL

/**
Spareroom
  - CeilingLight
**/
// #define DEVICE_SHELLYDIMMER_SPAREROOM_CEILING
// #define DEVICE_SPAREROOM_H801_WARDROBE_UPLIGHTS
// #define DEVICE_BLACK_STAND_LIGHT
// #define DEVICE_AMBILIGHT_SAMSUNG_SPAREROOM

/**
 * 
Masterbedroom
  - CeilingLight = ShellyDimmer
  - FloorFan
  - sensor?
  - non camera home display
 */
// #define DEVICE_SHELLYDIMMER_MASTERBEDROOM_CEILING
//define DEVICE_MASTERBEDROOMSENSOR
// #define DEVICE_FLOORFAN2
// #define DEVICE_MASTERBEDROOM_DRESSER_LIGHTS

/**
 * 
 Ensuite
  - CeilingLight = ShellyDimmer
  - Fan ? 
  - Sensor = motion, led strip, bme
  - home mini
 */
// #define DEVICE_ENSUITESENSOR
// #define DEVICE_SHELLYDIMMER_ENSUITE_CEILING

/**
 *  Landing
  - CeilingLight = dimmer, 2 inputs
  - LandingPanel = PIR, Nextion, BME, light strip
  - dht
 **/  
// #define DEVICE_SHELLYDIMMER_LANDING_CEILING
// #define DEVICE_LANDINGPANEL
// #define DEVICE_LANDINGPANEL2
// #define DEVICE_IMMERSIONSENSOR

/**
 * Attic
 */
// #define DEVICE_ATTICSENSOR

/**
Bathroom
  - home mini
  - shellydimmer on pull cord?
**/

/**
 * Bedroom
 - floorfan1
 - ceilingfan
 - bedroomsensor (climate, motion, door) = remove it as a tester
 - airpurifier
 - rgbbedroomclock
 - candlewarmer
 **/
// #define DEVICE_BEDROOMSENSOR
// #define DEVICE_AIRPURIFIER
// #define DEVICE_BEDROOM_CEILINGFAN
// #define DEVICE_FLOORFAN1
// #define DEVICE_BEDROOMBLINDS
// #define DEVICE_RGBNOTIFICATION_01
// #define DEVICE_BEDROOM_WALLCLOCK_01
// #define DEVICE_HVAC_HAIRDRYER
// #define DEVICE_H801_INSIDE_BEDROOM_WARDROBE




/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Generic devices ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


#ifdef DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_CTR          "socket_number_" STR2(DEVICENAME_SOCKET_NUMBER_CTR)
  #define DEVICENAME_FRIENDLY_CTR "Socket Number " STR2(DEVICENAME_SOCKET_NUMBER_CTR)
  
  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define DISABLE_WEBSERVER

  #define USE_MODULE_CORE_RULES
  
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BUTTONS
  
  #define USE_MODULE_DRIVERS_RELAY
  #define MAX_RELAYS 1
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SONOFF_BASIC_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Socket"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";
  
 
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    "\"Rule0\":{" //switch example
      "\"Trigger\":{"
        "\"Module\":\"Buttons\","    //sensor
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0," // eg Switch0, Switch1, Button#, Motion, # (number for index)  
        "\"State\":2" //eg. On, Off, Toggle, Any, LongPress, ShortPress, RisingEdge, FallingEdge, Started, Ended, TimerOnStarted
      "},"
      "\"Command\":{"
        "\"Module\":\"Relays\","
        "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0," //number, name, or all
        "\"State\":2" // toggle
      "}"
    "}"
  "}";

#endif


#ifdef DEVICE_SOCKET_NUMBERED_WITH_SERIAL_GPIO_BUTTON
  // #define DEVICENAME_SOCKET_NUMBER_CTR 2
  #define DEVICENAME_CTR          "socket_number_" STR2(DEVICENAME_SOCKET_NUMBER_CTR)
  #define DEVICENAME_FRIENDLY_CTR "Socket Number " STR2(DEVICENAME_SOCKET_NUMBER_CTR)
  
  /**
   * Disable serial logging
   * Use RX pin, gpio 1, as switch input
   * TX pin should still allow debugging
   * */
  // #define DISABLE_SERIAL_LOGGING
  
  #define DISABLE_WEBSERVER
  #define USE_MODULE_CORE_RULES
  
  #define USE_MODULE_SENSORS_BUTTONS
  
  #define USE_MODULE_DRIVERS_RELAY
  #define MAX_RELAYS 1
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    /**
     * Temporary fix creating a new template, as adding gpio on top of existing default templates is not working
     * */
    // "\"" D_JSON_GPIOC "\":{"
    //   "\"14\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR   "\"" // RX pin, possibly to leave TX pin (GPIO1) for debugging later
    // "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SONOFF_BASIC_EXTERNAL_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Socket"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";
  

  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Builtin Button as toggle
    "\"Rule0\":{" //switch example
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_BUTTONS_FRIENDLY_CTR "\","    //sensor
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0," // eg Switch0, Switch1, Button#, Motion, # (number for index)  
        "\"State\":0" //eg. On, Off, Toggle, Any, LongPress, ShortPress, RisingEdge, FallingEdge, Started, Ended, TimerOnStarted
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
        "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0," //number, name, or all
        "\"State\":2" // toggle
      "}"
    "},"
    // Optional external button on RX pin
    "\"Rule1\":{" //switch example
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_BUTTONS_FRIENDLY_CTR "\","    //sensor
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":1," // eg Switch0, Switch1, Button#, Motion, # (number for index)  
        "\"State\":0" //eg. On, Off, Toggle, Any, LongPress, ShortPress, RisingEdge, FallingEdge, Started, Ended, TimerOnStarted
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
        "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0," //number, name, or all
        "\"State\":2" // toggle
      "}"
    "}"
  "}";

#endif


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Kitchen/Dining  ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


#ifdef DEVICE_KITCHENLIGHT1
/**
 * Location: Utility Room Wall Switch
 * Inputs: Switch1 = Kitchen Switch1 (right hand side), Switch2 = Utility Switch1 (RHS)
 * Outputs: 2 relays possible, Relay1 = Kitchen light, Relay2 = Not connected
 * https://tasmota.github.io/docs/devices/Shelly-2.5/
 * */
  #define DEVICENAME_CTR          "kitchenlight1"
  #define DEVICENAME_FRIENDLY_CTR "Kitchen Light 1 [Main Kitchen]"
  
  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  
  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_CORE_RULES

  #define USE_MODULE_DRIVERS_RELAY
  #define MAX_RELAYS 1
  #define USE_MODULE_DRIVERS_INTERFACE

  #define DISABLE_SLEEP
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"13\":\"" D_GPIO_FUNCTION_SWT1_NP_CTR  "\","
      "\"5\":\""  D_GPIO_FUNCTION_SWT2_NP_CTR  "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"4\":\""  D_GPIO_FUNCTION_REL1_CTR  "\","
      "\"15\":\"" D_GPIO_FUNCTION_REL2_CTR  "\","
      #endif 
      "\"0\":\"" D_GPIO_FUNCTION_LED1_CTR "\""
      // Button1 GPIO2
      // I2C SDA 12
      // I2C SCL 14
      // ADE7953 IRQ GPIO16
      // Energy metering is done by a ADE7953 chip connected via I2C and IRQ on GPIO16.
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Kitchen Main"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Disconnected"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";
  
  #define USE_RULES_TEMPLATE // Rules, like the other templates, will be feed into the same command structure, so can actually be combined with `FUNCTION_TEMPLATE`
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"Switches\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"Module\":\"Relays\","
        "\"Function\":\"SetPower\","
        "\"DeviceName\":0,"
        "\"State\":2"
      "}"
    "},"
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"Switches\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":1,"
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"Module\":\"Relays\","
        "\"Function\":\"SetPower\","
        "\"DeviceName\":0,"
        "\"State\":2"
      "}"
    "}"
  "}";

#endif

#ifdef DEVICE_KITCHENLIGHT2
  #define DEVICENAME_CTR          "kitchenlight2"
  #define DEVICENAME_FRIENDLY_CTR "Kitchen Light 2 [Table/Window]"
  
  // //#define FORCE_TEMPLATE_LOADING
  // #define USE_MODULE_SENSORS_SWITCHES
  // #define USE_MODULE_CORE_RULES
  
  /*
    Method should only activate if boot loop happens 10 times
    Method A: Switch to Wifi.begin hardcoded ssid/pw, start OTA and wait, rebooting every 10 minutes if wifi does not connect
    Method B: Begin wifi.ap as host, so I can directly connect to it via x.x.x.x
  */
  #define DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS
  //#define DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS

  #define USE_MODULE_ENERGY_INTERFACE
  #define USE_MODULE_ENERGY_ADE7953

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_BUTTONS

  #define USE_MODULE_NETWORKS_MQTT

  #define USE_MODULE_CORE_RULES

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
  #define MAX_RELAYS 2
    
  // #define USE_MODULE_TEMPLATE
  // DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
  //   "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  //   "\"" D_JSON_GPIOC "\":{"
  //     #ifdef USE_MODULE_SENSORS_SWITCHES
  //     "\"13\":\"" D_GPIO_FUNCTION_SWT1_NP_CTR  "\","
  //     "\"5\":\""  D_GPIO_FUNCTION_SWT2_NP_CTR  "\","
  //     #endif
  //     #ifdef USE_MODULE_DRIVERS_RELAY
  //     "\"4\":\"" D_GPIO_FUNCTION_REL1_CTR  "\","
  //     "\"15\":\"" D_GPIO_FUNCTION_REL2_CTR  "\","
  //     #endif 
  //     "\"0\":\"" D_GPIO_FUNCTION_LED1_CTR "\""
  //   "},"
  //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  // "}";
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY2P5_CTR "\""
  "}";


  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Dining Room"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Window"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
      "],"
        "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
          "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";

  
  // #define USE_RULES_TEMPLATE
  // DEFINE_PGM_CTR(RULES_TEMPLATE)
  // "{"
  //   "\"Rule0\":{"
  //     "\"Trigger\":{"
  //       "\"Module\":\"Switches\","
  //       "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
  //       "\"DeviceName\":0,"
  //       "\"State\":2"
  //     "},"
  //     "\"Command\":{"
  //       "\"Module\":\"Relays\","
  //       "\"Function\":\"SetPower\","
  //       "\"DeviceName\":1,"
  //       "\"State\":2" 
  //     "}"
  //   "},"
  //   "\"Rule1\":{"
  //     "\"Trigger\":{"
  //       "\"Module\":\"Switches\","
  //       "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
  //       "\"DeviceName\":1,"
  //       "\"State\":2"
  //     "},"
  //     "\"Command\":{"
  //       "\"Module\":\"Relays\","
  //       "\"Function\":\"SetPower\","
  //       "\"DeviceName\":0,"
  //       "\"State\":2"
  //     "}"
  //   "}"
  // "}";

  /**
   * NOTE: indexes swapped
   * */
  #ifdef DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Switch0 Toggle = Relay0 Power Toggle
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":1,"
        "\"State\":2" // 3 (or other) means follow, so copy input from trigger
      "}"
    "},"
    // Switch1 Toggle = Relay1 Power Toggle
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":1,"
        "\"State\":2"      // 2 meaning either low or high, 1 would be high only
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2" // 3 (or other) means follow, so copy input from trigger
      "}"
    "},"
    // Button0 Single Press = Relay0 Power On for 10 seconds tester
    "\"Rule2\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_BUTTONS_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2" // 
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOn\\\":10}}\""
      "}"
    "}"
  "}";
  #endif // DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS

#endif



#ifdef DEVICE_KITCHENLIGHT3
  #define DEVICENAME_CTR          "kitchenlight3"
  #define DEVICENAME_FRIENDLY_CTR "Kitchen 3 VirtualSwt"
  
  //#define FORCE_TEMPLATE_LOADING
  #define USE_MODULE_CORE_RULES

  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_DRIVERS_RELAY
  #define MAX_RELAYS 1
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"5\":\"" D_GPIO_FUNCTION_SWT1_NP_CTR  "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"4\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      #endif 
      "\"0\":\"" D_GPIO_FUNCTION_LED1_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Switch4_Virtual"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
      "],"
    "}"
  "}";


  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"Switches\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"Module\":\"Relays\","
        "\"Function\":\"SetPower\","
        "\"DeviceName\":0,"
        "\"State\":0,"
        "\"JsonCommands\":\"{\\\"MQTTSend\\\":{\\\"Topic\\\":\\\"socket_number_11/set\\\",\\\"Payload\\\":\\\"{~PowerName~:0,~PowerState~:2}\\\"}}\""
      "}"
    "}"
  "}";

#endif


#ifdef DEVICE_KITCHENLIGHT4
  #define DEVICENAME_CTR          "kitchenlight4"
  #define DEVICENAME_FRIENDLY_CTR "Kitchen Cupboard Lights"
  
  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define USE_MODULE_SENSORS_BUTTONS

  #define USE_MODULE_DRIVERS_RELAY
  #define MAX_RELAYS 1
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SONOFF_BASIC_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Cupboard"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif


/**
 * This will become the final version of a panel, but requires animation fixing first
 * */
#ifdef DEVICE_NEXTION_HEATING_KITCHEN_DISPLAY
  #define DEVICENAME_CTR            "heating_kitchen_display"
  #define DEVICENAME_FRIENDLY_CTR   "Heating Kitchen Display"

  // #define USE_MODULE_NETWORK_WEBSERVER
  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_MOTION

  // #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_NEXTION

  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      // #ifdef USE_MODULE_SENSORS_MOTION
      // "\"21\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR "\","
      // #endif
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
  //   "\"" D_JSON_STRIP_SIZE       "\":40,"
  //   "\"" D_JSON_RGB_COLOUR_ORDER "\":\"rgbwc\","
  //   "\"" D_JSON_TRANSITION       "\":{"
  //     "\"" D_JSON_TIME_MS "\":1000,"
  //     "\"" D_JSON_RATE_MS "\":1000,"
  //     "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
  //     "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
  //   "},"
  //   "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
  //   "\"" D_JSON_EFFECTS "\":{" 
  //     "\"" D_JSON_FUNCTION "\":8"
  //   "},"
  //   "\"" D_JSON_CCT_TEMP "\":300,"
  //   "\"" D_JSON_HUE "\":25,"
  //   "\"" D_JSON_SAT "\":100,"
  //   "\"" D_JSON_COLOUR_PALETTE "\":\"RGBCCTColour 01\"," //ie 10
  //   "\"" D_JSON_BRIGHTNESS_CCT "\":100,"
  //   "\"" D_JSON_BRIGHTNESS_RGB "\":100"
  // "}";
  
  // #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Landing"
  // #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Landing"
  
  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  // "{"
  //   "\"" D_JSON_DEVICENAME "\":{"
  //     "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
  //       "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
  //     "],"
  //     "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
  //       "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
  //     "]"
  //   "}"
  // "}";

  // #define USE_FUNCTION_NEXTION_INIT_PANEL_COMMAND_TEMPLATE
  // DEFINE_PGM_CTR(NEXTION_INIT_PANEL_COMMAND_TEMPLATE)
  // "{"
  //   // "\"" D_JSON_DEVICENAME "\":{"
  //   //   "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
  //   //     "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
  //   //   "],"
  //   //   "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
  //   //     "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
  //   //   "]"
  //   // "}"
  //   "p[0]."


  // "}";

#endif





#ifdef DEVICE_RGBSHELF
  #define DEVICENAME_CTR          "rgbshelf"
  #define DEVICENAME_FRIENDLY_CTR "Shelf Lights"
    
    
  
  #define STRIP_SIZE_MAX 33                                                                           // Change: Set *total* length of string, segment0 will default to this length
  #define PIN_NAME_STRING_ESP8266_DEFAULT   "RX"                                                      // Change: Set to the pin you want, esp8266 this will default to this anyway
  #define PIN_NAME_STRING_ESP32_DEFAULT     "23"                                                      //         Set to the pin you want, any output pin should work

  /**
   * @brief Uncomment one line to use testing template configs for lighting_template
   * 
   */
  // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE                                         // Change: You can pick one as examples
  #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
   
  /**
   * @brief Mostly for me testing, switching between my segments or testing orginal wled effects
   **/
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  #define D_EFFECT_INSIDE_TEMPLATE "Effects"
  /**
   * @brief defines to be tested and incorporated fully
   **/
  #define ENABLE_FEATURE_INCLUDE_WLED_PALETTES
  #define ENABLE_CRGBPALETTES_IN_PROGMEM
  #define ENABLE_DEVFEATURE_SHIMMERING_PALETTE_BRIGHTNESS_LIMIT
  #define ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS
  // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS
  // #define ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
  // #define ENABLE_DEVFEATURE_WS2812FX_DEFAULT_PALETTE_EFFECTS
  // #define ENABLE_DEVFEATURE_GET_COLOUR_PALETTE_JOINT_METHOD
  // #define ENABLE_DEVFEATURE_PALETTE_ADVANCED_METHODS_GEN2 // ie the new way of merging fastled to mine
  /**
   * @brief Debug flags, used mostly be me
   * 
   */  
  // #define ENABLE_FREERAM_APPENDING_SERIAL
  // #define DEBUG_WLED_EFFECT_FUNCTIONS
  // #define ENABLE_DEVFEATURE_LEARNING_FASTLED_PALETTES


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
    #ifdef ESP8266 // default pins for ws28xx
      "\"" PIN_NAME_STRING_ESP8266_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #else
      "\"" PIN_NAME_STRING_ESP32_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #endif
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

/**
 * @brief 
 * LIGHTING_TEMPLATE
 * 1) Without "Segment#:{}" used, it is assumed the command should be applied to the entire strip, defaulting entire strip as segment number 0
 * 2) Multiple segments can be set, be using the same commands but under multiple json keys called segment with its number (currently maximum of 5 segments)
 *    eg
 *        {
 *        "Segment0":{
 *                      "ColourPalette":"Christmas 01" 
 *                   },
 *        "Segment1":{
 *                      "ColourPalette":"Christmas 02" 
 *                   },
 *        "Segment2":{
 *                      "ColourPalette":"Christmas 03" 
 *                   }
 *         }
 * 
 */

/**
 * @brief The following templates are tested examples
 * 
 */

  #define USE_LIGHTING_TEMPLATE

  // #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Pink purple 01\"," 
    "\"Effects\":{"
      "\"Function\":\"Static Palette\","
      "\"Intensity\":127,"
      "\"Speed\":10"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":1000,"
      "\"RateMs\":1000"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  // #endif



//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR
//   #define USE_MODULE_LIGHTS_ADDRESSABLE
  

//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
//     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_JSON_GPIOC "\":{"
//       "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
//     "},"
//     "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
//   "}";

//   #define STRIP_SIZE_MAX 33

//  #define USE_LIGHTING_TEMPLATE
//   // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   // "{"
//   //   "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
//   //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//   //   "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","
//   //   "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
//   //   "\"" D_JSON_EFFECTS "\":{" 
//   //     "\"" D_JSON_FUNCTION "\":\"" "Slow Glow" "\""
//   //   "},"
//   //   "\"" D_JSON_TRANSITION       "\":{"
//   //     "\"" D_JSON_TIME_MS "\":10000,"
//   //     "\"" D_JSON_RATE_MS "\":1000,"
//   //     "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
//   //     "\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\""
//   //   "},"
//   //   "\"TimeMs\":30000,"
//   //   "\"ColourPalette\":\"Shelf Hearts\","
//   //   "\"BrightnessRGB\":70"
//   // "}";
  
//   // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   // "{"
//   //   "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
//   //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//   //   "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","
//   //   "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
//   //   "\"" D_JSON_EFFECTS "\":{" 
//   //     "\"Function\":\"Static Glow\""
//   //   "},"    
//   //   "\"" D_JSON_TRANSITION       "\":{"
//   //     "\"" D_JSON_TIME_MS "\":1000,"
//   //     "\"" D_JSON_RATE_MS "\":2000,"
//   //     "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":10,"
//   //     "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
//   //   "},"
//   //   "\"ColourPalette\":\"Christmas Warm White\","
//   //   "\"BrightnessRGB\":100"
//   // "}";

//   #define USE_CUSTOM_USER_PAULA
//   #define STRIP_SIZE_MAX 100
//   #define USE_LIGHTING_TEMPLATE
//   DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   "{"
//     "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
//     "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//     "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RgB\","
//     "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
//     "\"" D_JSON_EFFECTS "\":{" 
//       "\"Function\":\"Static Glow\""
//     "},"    
//     "\"" D_JSON_TRANSITION       "\":{"
//       "\"" D_JSON_TIME_MS "\":1000,"
//       "\"" D_JSON_RATE_MS "\":5000,"
//     "},"
//     "\"ColourPalette\":\"Custom User 01\","
//     "\"BrightnessRGB\":70"
//   "}";

#endif

#ifdef DEVICE_RGBCRYSTAL3
  #define DEVICENAME_CTR          "rgbcrystal3"
  #define DEVICENAME_FRIENDLY_CTR "Glass Box Lights"
  
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define STRIP_SIZE_MAX 50
  #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
  //   #ifdef STRIP_SIZE_MAX
  //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  //   #else
  //   "\"" D_JSON_STRIP_SIZE       "\":50,"
  //   #endif //STRIP_SIZE_MAX
  //   "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
  //   // "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
  //   "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
  //   "\"" D_JSON_EFFECTS "\":{" 
  //     "\"Function\":1" //slow glow
  //   "},"
  //   "\"Transition\":{\"Order\":\"InOrder\",\"PixelUpdatePerc\":2,\"RateMs\":10000},"
  //   "\"TimeMs\":5000,"
  //   "\"ColourPalette\":\"Shelf Hearts\"," //c12    43 is the colours for this christmas
  //   "\"BrightnessRGB\":100"
  // "}";
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"grb\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":\"Static Glow\""
    "},"    
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":1000,"
      "\"" D_JSON_RATE_MS "\":2000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":10,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
    "},"
    "\"ColourPalette\":\"Christmas MultiColoured Warmer\","
    "\"BrightnessRGB\":100"
  "}";

#endif


#ifdef DEVICE_RGBCOOKER
  #define DEVICENAME_CTR          "rgbcooker"
  #define DEVICENAME_FRIENDLY_CTR "RGB Cooker H801"
      
  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define DISABLE_WEBSERVER

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_PWM
  
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"1\":\""  D_GPIO_FUNCTION_LED1_CTR "\","
      "\"5\":\""  D_GPIO_FUNCTION_LED2_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_H801_CTR "\""
  "}";
  
  #define STRIP_SIZE_MAX 1
 #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\"" "RGBCCT_PWM" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":1,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"RGBCW\","
    "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":2,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\"},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"Solid Rgbcct 01\","
    "\"" D_JSON_ANIMATIONMODE  "\":\"" D_JSON_EFFECTS "\","
    "\"" D_JSON_EFFECTS        "\"{\"Function\":8},"
    "\"" D_JSON_BRIGHTNESS     "\":100"
  "}";

#endif


#ifdef DEVICE_KITCHENSENSOR
  #define DEVICENAME_CTR          "kitchensensor"
  #define DEVICENAME_FRIENDLY_CTR "Kitchen Sensor"

  #define USE_MODULE_CORE_RULES
       
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_BME
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D6\":\"" D_GPIO_FUNCTION_SWT1_CTR   "\""
      #endif
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Kitchen"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Kitchen"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"    
    // for PIR to follow
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":0," 
        "\"State\":\"Follow\""
      "}"
    "}"
  "}";

#endif


#ifdef DEVICE_RGBFRIDGE
  #define DEVICENAME_CTR          "rgbfridge"
  #define DEVICENAME_FRIENDLY_CTR "RGB Fridge H801"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_PWM
  
  
  #define USE_MODULE_SUBSYSTEM_SOLAR_LUNAR 
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_H801_CTR "\""
  "}";


  #define STRIP_SIZE_MAX 1
 #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\"" "RGBCCT_PWM" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":1,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"RGBCW\","
    "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":2,\"" D_JSON_RATE "\":20},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"Solid RGBCCT Sun Elevation 04: Kitchen Pink, CW, WW\","
    "\"" D_JSON_ANIMATIONMODE  "\":\"" D_JSON_EFFECTS "\","
    "\"" D_JSON_EFFECTS        "\"{\"Function\":\"Sun Elevation RGBCCT Solid Palette 01\"},"
    "\"" D_JSON_BRIGHTNESS_CCT "\":100,"
    "\"" D_JSON_BRIGHTNESS_RGB "\":100"
  "}";


#endif




/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Utility ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


#ifdef DEVICE_RGBUTILITY
  #define DEVICENAME_CTR          "rgbutility"
  #define DEVICENAME_FRIENDLY_CTR "RGB Utility H801"

  // //#define FORCE_TEMPLATE_LOADING
  // //#define SETTINGS_HOLDER 2 //maintain other settings (bootcount)
  
  // #define USE_SERIAL_ALTERNATE_TX
   
  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  // #define USE_MODULE_LIGHTS_PWM
  
  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define DISABLE_WEBSERVER

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_PWM
  
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"1\":\""  D_GPIO_FUNCTION_LED1_CTR "\","
      "\"5\":\""  D_GPIO_FUNCTION_LED2_INV_CTR "\""
    "},"
    "\"FLAG\":0,"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_H801_CTR "\""
  "}";

  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_HARDWARE_TYPE  "\":\"" D_PIXEL_HARDWARE_TYPE_RGBCCT_PWM_CTR "\","
  //   "\"" D_JSON_CCT_TEMP       "\":500,"
  //   "\"" D_JSON_BRIGHTNESS     "\":100"
  // "}";

  #define STRIP_SIZE_MAX 1
 #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\"" "RGBCCT_PWM" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":1,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"RGBCW\","
    "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":2,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\"},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"Solid Rgbcct 01\","
    "\"" D_JSON_ANIMATIONMODE  "\":\"" D_JSON_EFFECTS "\","
    "\"" D_JSON_EFFECTS        "\"{\"Function\":8},"
    "\"" D_JSON_BRIGHTNESS     "\":100"
  "}";


#endif

#ifdef DEVICE_UTILITYSENSOR
  #define DEVICENAME_CTR          "utilitysensor"
  #define DEVICENAME_FRIENDLY_CTR "Utility Sensor"

  #define USE_MODULE_CORE_RULES
     
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION
  #define USE_MODULE_SENSORS_DOOR

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    DEFINE_APP_SVALUE("NAME",DEVICENAME_CTR)
    DEFINE_APP_SVALUE("FRIENDLYNAME",DEVICENAME_FRIENDLY_CTR)
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_BME
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR    "\","
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      DEFINE_APP_SVALUE("D6",D_GPIO_FUNCTION_SWT1_CTR)
      #endif
      #ifdef USE_MODULE_SENSORS_DOOR
      DEFINE_END_SVALUE("D7",D_GPIO_FUNCTION_DOOR_OPEN_CTR)
      #endif
      // DEFINE_END_SVALUE("D4",D_GPIO_FUNCTION_LED1_CTR)
    "},"
    DEFINE_END_SVALUE("BASE",D_MODULE_NAME_USERMODULE_CTR)
  "}";

  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Utility"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Utility"
  #define D_DEVICE_SENSOR_DOOROPEN_FRIENDLY_NAME_LONG "SideDoor"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Door" "\""
      "],"
      "\"" D_MODULE_SENSORS_DOOR_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_DOOROPEN_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";


  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"

  // for PIR to follow
   "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":0," 
        "\"State\":\"Follow\""
      "}"


    // // Switch0 HIGH = Motion0 Event Started, ie report as motion with motion name
    // "\"Rule0\":{"
    //   // "\"Trigger\":{"
    //   //   "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
    //   //   "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
    //   //   "\"DeviceName\":0,"
    //   //   "\"State\":1" // FOLLOW, ie command follows trigger, or follow_inv, ie command is inverted to source
    //   // "},"
    //   // "\"Command\":{"
    //   //   "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
    //   //   "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
    //   //   "\"DeviceName\":0,"     // Index of motion to be used for name eg garage, motion, then time from when mqtt is sent
    //   //   "\"State\":2" // Started
    //   // "}"
      
    // "\"Trigger\":{"
    //     "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
    //     "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
    //     "\"DeviceName\":0,"
    //     "\"State\":\"On\""
    //   "},"
    //   "\"Command\":{"
    //     "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
    //     "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
    //     "\"DeviceName\":0," 
    //     "\"State\":\"FollowInv\""
    //   "}"
    // "}"



    // "},"
    // // Switch0 HIGH = Motion0 Event Started, ie report as motion with motion name
    // "\"Rule1\":{"
    //   "\"Trigger\":{"
    //     "\"Module\":\"" D_MODULE_SENSORS_DOOR_FRIENDLY_CTR "\","
    //     "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
    //     "\"DeviceName\":1,"
    //     "\"State\":\"On\"" // FOLLOW, ie command follows trigger, or follow_inv, ie command is inverted to source
    //   "},"
    //   "\"Command\":{"
    //     "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
    //     "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
    //     "\"DeviceName\":1,"     // Index of motion to be used for name eg garage, motion, then time from when mqtt is sent
    //     "\"State\":\"FollowInv\""
    //   "}"
    "}"



  "}";



#endif


/**
 * @brief Changed to segments now 
 **/
#ifdef DEVICE_RGBCRYSTAL1
  #define DEVICENAME_CTR            "rgbcrystal1"
  #define DEVICENAME_FRIENDLY_CTR   "Crystal Light Cylinder Utility"
  

This might actually be worth changing to esp32, to give year long debug data!!! yes 

  
  #define STRIP_SIZE_MAX 100                                                                           // Change: Set *total* length of string, segment0 will default to this length
  #define PIN_NAME_STRING_ESP8266_DEFAULT   "RX"                                                      // Change: Set to the pin you want, esp8266 this will default to this anyway
  #define PIN_NAME_STRING_ESP32_DEFAULT     "23"                                                      //         Set to the pin you want, any output pin should work

  /**
   * @brief Uncomment one line to use testing template configs for lighting_template
   * 
   */
  // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE                                         // Change: You can pick one as examples
  #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
   
  /**
   * @brief Mostly for me testing, switching between my segments or testing orginal wled effects
   **/
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  #define D_EFFECT_INSIDE_TEMPLATE "Effects"
  /**
   * @brief defines to be tested and incorporated fully
   **/
  #define ENABLE_FEATURE_INCLUDE_WLED_PALETTES
  #define ENABLE_CRGBPALETTES_IN_PROGMEM
  #define ENABLE_DEVFEATURE_SHIMMERING_PALETTE_BRIGHTNESS_LIMIT
  #define ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS
  // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS
  // #define ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
  // #define ENABLE_DEVFEATURE_WS2812FX_DEFAULT_PALETTE_EFFECTS
  // #define ENABLE_DEVFEATURE_GET_COLOUR_PALETTE_JOINT_METHOD
  // #define ENABLE_DEVFEATURE_PALETTE_ADVANCED_METHODS_GEN2 // ie the new way of merging fastled to mine
  /**
   * @brief Debug flags, used mostly be me
   * 
   */  
  // #define ENABLE_FREERAM_APPENDING_SERIAL
  // #define DEBUG_WLED_EFFECT_FUNCTIONS
  // #define ENABLE_DEVFEATURE_LEARNING_FASTLED_PALETTES
  //#define ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES 3


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
    #ifdef ESP8266 // default pins for ws28xx
      "\"" PIN_NAME_STRING_ESP8266_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #else
      "\"" PIN_NAME_STRING_ESP32_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #endif
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

/**
 * @brief 
 * LIGHTING_TEMPLATE
 * 1) Without "Segment#:{}" used, it is assumed the command should be applied to the entire strip, defaulting entire strip as segment number 0
 * 2) Multiple segments can be set, be using the same commands but under multiple json keys called segment with its number (currently maximum of 5 segments)
 *    eg
 *        {
 *        "Segment0":{
 *                      "ColourPalette":"Christmas 01" 
 *                   },
 *        "Segment1":{
 *                      "ColourPalette":"Christmas 02" 
 *                   },
 *        "Segment2":{
 *                      "ColourPalette":"Christmas 03" 
 *                   }
 *         }
 * 
 */

/**
 * @brief The following templates are tested examples
 * 
 */

  #define USE_LIGHTING_TEMPLATE

  // #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Pink purple 01\"," 
    "\"Effects\":{"
      "\"Function\":\"Shimmering Palette\","
      "\"Intensity\":255,"
      "\"Speed\":10"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":23"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  // #endif

  // #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
  //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  //   "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
  //   "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
  //   "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
  //   "\"Effects\":{"
  //     "\"Function\":\"" D_EFFECTS_FUNCTION__SLOW_GLOW__NAME_CTR "\""
  //   "},"
  //   "\"Transition\":{"
  //     "\"TimeMs\":5000,"
  //     "\"RateMs\":20000"
  //   "},"    
  //   "\"BrightnessRGB\":100"
  // "}";
  // #endif

  // #ifdef LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
  //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  //   "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
  //   "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
  //   "\"ColourPalette\":\"Christmas 21\"," 
  //   "\"Effects\":{"
  //     "\"Function\":30"
  //   "},"
  //   "\"Transition\":{"
  //     "\"TimeMs\":0,"
  //     "\"RateMs\":30"
  //   "},"    
  //   "\"BrightnessRGB\":100"
  // "}";
  // #endif // LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS

  // #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
  //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  //   "\"" D_JSON_RGB_COLOUR_ORDER "\":\"grb\","    
  //   "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
  //   "\"ColourPalette\":\"Christmas 06\"," 
  //   "\"Effects\":{"
  //     "\"Function\":1"
  //   "},"
  //   "\"Transition\":{"
  //     "\"TimeMs\":0,"
  //     "\"RateMs\":30"
  //   "},"    
  //   "\"BrightnessRGB\":100"
  // "}";
  // #endif // LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS

  // // #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS
  // // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // // "{"
  // //   "\"Segment0\":{"
  // //     "\"PixelRange\":[0,19],"
  // //     "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
  // //     "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  // //     "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
  // //     "\"Effects\":{"
  // //       "\"Function\":\"Static\""
  // //     "},"
  // //     "\"ColourPalette\":\"Christmas 09\","
  // //     "\"Transition\":{"
  // //       "\"TimeMs\":3000,"
  // //       "\"RateMs\":10000"
  // //     "},"    
  // //     "\"BrightnessRGB\":100"
  // //   "},"
  // //   "\"Segment1\":{"
  // //     "\"PixelRange\":[20,29],"
  // //     "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
  // //     "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  // //     "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","   
  // //     "\"Effects\":{"
  // //       "\"Function\":\"Solid RGBCCT\""
  // //     "},"
  // //     "\"ColourPalette\":\"Solid Rgbcct 01\","
  // //     "\"Transition\":{"
  // //       "\"TimeMs\":500,"
  // //       "\"RateMs\":1000"
  // //     "},"    
  // //     "\"BrightnessRGB\":100"
  // //   "},"
  // //   "\"Segment2\":{"
  // //     "\"PixelRange\":[30,49],"
  // //     "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
  // //     "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  // //     "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
  // //     "\"Effects\":{"
  // //       "\"Function\":\"Slow Glow\""
  // //     "},"
  // //     "\"ColourPalette\":\"Christmas 01\","
  // //     "\"Transition\":{"
  // //       "\"TimeMs\":500,"
  // //       "\"RateMs\":1000"
  // //     "},"    
  // //     "\"BrightnessRGB\":100"
  // //   "},"
  // //   "\"" D_JSON_ANIMATIONMODE    "\":\"Effects\","
  // //   "\"BrightnessRGB\":100"
  // // "}";
  // // #endif
  
  // // #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS_FOR_UTILITY
  // // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // // "{"
  // //   "\"Segment0\":{"
  // //     "\"PixelRange\":[0,19],"
  // //     "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
  // //     "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  // //     "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
  // //     "\"Effects\":{"
  // //       "\"Function\":29"
  // //     "},"
  // //     "\"ColourPalette\":\"Christmas 09\","
  // //     "\"Transition\":{"
  // //       "\"TimeMs\":0,"
  // //       "\"RateMs\":23"
  // //     "},"    
  // //     "\"BrightnessRGB\":100"
  // //   "},"
  // //   "\"Segment1\":{"
  // //     "\"PixelRange\":[20,29],"
  // //     "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
  // //     "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  // //     "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","   
  // //     "\"Effects\":{"
  // //       "\"Function\":28"
  // //     "},"
  // //     "\"ColourPalette\":11,"
  // //     "\"Hue\":20,"
  // //     "\"Sat\":90,"
  // //     "\"Transition\":{"
  // //       "\"TimeMs\":0,"
  // //       "\"RateMs\":25"
  // //     "},"    
  // //     "\"BrightnessRGB\":100"
  // //   "},"
  // //   "\"Segment2\":{"
  // //     "\"PixelRange\":[30,49],"
  // //     "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
  // //     "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  // //     "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
  // //     "\"Effects\":{"
  // //       "\"Function\":\"Slow Glow\""
  // //     "},"
  // //     "\"ColourPalette\":\"Christmas 01\","
  // //     "\"Transition\":{"
  // //       "\"TimeMs\":500,"
  // //       "\"RateMs\":1000"
  // //     "},"    
  // //     "\"BrightnessRGB\":100"
  // //   "},"
  // //   "\"" D_JSON_ANIMATIONMODE    "\":\"Effects\","
  // //   "\"BrightnessRGB\":100"
  // // "}";
  // // #endif


  
//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR
//   #define USE_MODULE_LIGHTS_ADDRESSABLE
  


//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
//     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_JSON_GPIOC "\":{"
//       "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
//     "},"
//     "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
//   "}";

//   #define STRIP_SIZE_MAX 155

// //   #define ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES 150

//  #define USE_LIGHTING_TEMPLATE
// //   DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
// //   "{"
// //     "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
// //     #ifdef STRIP_SIZE_MAX
// //     "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
// //     #else
// //     "\"" D_JSON_STRIP_SIZE       "\":50,"
// //     #endif //STRIP_SIZE_MAX
// //     "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
// //     // "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
// //     "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
// //     "\"" D_JSON_EFFECTS "\":{" 
// //       "\"Function\":1" //slow glow
// //     "},"
// //     "\"Transition\":{\"Order\":\"InOrder\",\"PixelUpdatePerc\":2,\"RateMs\":10000},"
// //     "\"TimeMs\":5000,"
// //     "\"ColourPalette\":\"Shelf Hearts\"," //c12    43 is the colours for this christmas
// //     "\"BrightnessRGB\":100"
// //   "}";

  
//   DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   "{"
//     "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
//     "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//     "\"" D_JSON_RGB_COLOUR_ORDER "\":\"grb\","
//     "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
//     "\"" D_JSON_EFFECTS "\":{" 
//       "\"Function\":\"Slow Glow\""
//     "},"    
//     "\"" D_JSON_TRANSITION       "\":{"
//       "\"" D_JSON_TIME_MS "\":1000,"
//       "\"" D_JSON_RATE_MS "\":10000"
//     "},"
//     "\"ColourPalette\":\"Christmas MultiColoured Warmer\","
//     "\"BrightnessRGB\":40"
//   "}";





#endif




/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Downstairs Toilet ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


#ifdef DEVICE_CONSUMERUNIT
  #define DEVICENAME_CTR          "consumerunit"
  #define DEVICENAME_FRIENDLY_CTR "Consumer Unit"
  #define ESP32
    
  #define DEVICENAMEBUFFER_NAME_BUFFER_LENGTH 800

  #define SETTINGS_SENSORS_MQTT_IFCHANGED_PERIOD_SECONDS 1 // temporary force the value, should be handled by command

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION

  #define USE_MODULE_ENERGY_INTERFACE
  #define USE_MODULE_ENERGY_PZEM004T_V3

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  #define D_EFFECT_INSIDE_TEMPLATE "Effects"
  #define STRIP_SIZE_MAX 55                                                                           // Change: Set *total* length of string, segment0 will default to this length
  #define ENABLE_FEATURE_INCLUDE_WLED_PALETTES
  #define ENABLE_CRGBPALETTES_IN_PROGMEM
  #define ENABLE_DEVFEATURE_SHIMMERING_PALETTE_BRIGHTNESS_LIMIT
  #define ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS
  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  #define USE_SK6812_METHOD_DEFAULT

  // Hard coded to alternate pin until I make this template based
  #define ENABLE_DEVFEATURE_SET_ESP32_RGB_DATAPIN_BY_TEMPLATE
  #define PINSET_TEMP_METHOD_RGB_PIN_RGB 4
  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "SK6812" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":10," 
    "\"Hue\":25," 
    "\"Sat\":70," 
    "\"Effects\":{"
      "\"Function\":0"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":900,"
      "\"RateMs\":1000"
    "},"    
    "\"BrightnessRGB\":10"
  "}";

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"      
      "\"16\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
      "\"17\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
      #ifdef USE_MODULE_SENSORS_BME
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"23\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"5\":\""  D_GPIO_FUNCTION_SWT1_CTR "\","
      #endif
      "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\"" // ENABLE_DEVFEATURE_SET_ESP32_RGB_DATAPIN_BY_TEMPLATE forcing this, not working by pinused
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "Downstairs Toilet"


  #define D_DEVICE_SENSOR_CLIMATE "Downstairs Toilet"
  #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "1"
  #define D_DEVICE_SENSOR_PZEM004T_1_ADDRESS "2"
  #define D_DEVICE_SENSOR_PZEM004T_2_ADDRESS "3"
  #define D_DEVICE_SENSOR_PZEM004T_3_ADDRESS "4"
  #define D_DEVICE_SENSOR_PZEM004T_4_ADDRESS "5"
  #define D_DEVICE_SENSOR_PZEM004T_5_ADDRESS "6"
  #define D_DEVICE_SENSOR_PZEM004T_6_ADDRESS "7"
  #define D_DEVICE_SENSOR_PZEM004T_7_ADDRESS "8"
  #define D_DEVICE_SENSOR_PZEM004T_8_ADDRESS "9"
  #define D_DEVICE_SENSOR_PZEM004T_9_ADDRESS "10"
  #define D_DEVICE_SENSOR_PZEM004T_10_ADDRESS "11"
  #define D_DEVICE_SENSOR_PZEM004T_11_ADDRESS "12"


  #define D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "Mains"
  #define D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR "Cooker"
  #define D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR "Immersion"
  #define D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR "Washing Machine"
  #define D_SENSOR_PZEM004T_4_FRIENDLY_NAME_CTR "Dishwasher"
  #define D_SENSOR_PZEM004T_5_FRIENDLY_NAME_CTR "Pump Shower"
  #define D_SENSOR_PZEM004T_6_FRIENDLY_NAME_CTR "Heating"
  #define D_SENSOR_PZEM004T_7_FRIENDLY_NAME_CTR "Tumble Dryer"
  #define D_SENSOR_PZEM004T_8_FRIENDLY_NAME_CTR "Garage"
  #define D_SENSOR_PZEM004T_9_FRIENDLY_NAME_CTR "Bathroom Shower"
  #define D_SENSOR_PZEM004T_10_FRIENDLY_NAME_CTR "Main Sockets"
  #define D_SENSOR_PZEM004T_11_FRIENDLY_NAME_CTR "Kitchen Sockets"
  
  
  #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR   "Mains"
  #define D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR   "Cooker"
  #define D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR   "Immersion"
  #define D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR   "Washing Machine"
  #define D_DRIVER_ENERGY_4_FRIENDLY_NAME_CTR   "Dishwasher"
  #define D_DRIVER_ENERGY_5_FRIENDLY_NAME_CTR   "Pump Shower"
  #define D_DRIVER_ENERGY_6_FRIENDLY_NAME_CTR   "Heating"
  #define D_DRIVER_ENERGY_7_FRIENDLY_NAME_CTR   "Tumble Dryer"
  #define D_DRIVER_ENERGY_8_FRIENDLY_NAME_CTR   "Garage"
  #define D_DRIVER_ENERGY_9_FRIENDLY_NAME_CTR   "Bathroom Shower"
  #define D_DRIVER_ENERGY_10_FRIENDLY_NAME_CTR  "Main Sockets"
  #define D_DRIVER_ENERGY_11_FRIENDLY_NAME_CTR  "Kitchen Sockets"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_4_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_5_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_6_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_7_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_8_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_9_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_10_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_11_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE "\""
      "],"
      "\"" D_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR "\":["
        "\"" D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_4_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_5_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_6_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_7_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_8_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_9_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_10_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_11_FRIENDLY_NAME_CTR "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR "\":[" 
        D_DEVICE_SENSOR_PZEM004T_0_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_1_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_2_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_3_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_4_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_5_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_6_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_7_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_8_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_9_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_10_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_11_ADDRESS ""
      "]"  
    "},"
    "\"" D_JSON_ENERGY "\":{"
        "\"DeviceCount\":12"    
    "}"
  "}";

  /**
   * In the future, make a way to push this exact rule via single command (append new rule, start using vectors for indexing?)
   * */
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
      // for PIR to follow
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":0," 
        "\"State\":\"Follow\""
      "}"
    // "},"
    // "\"Rule1\":{"
    //   "\"Trigger\":{"
    //     "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
    //     "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
    //     "\"DeviceName\":1,"
    //     "\"State\":1" // FOLLOW, ie command follows trigger, or follow_inv, ie command is inverted to source
    //   "},"
    //   "\"Command\":{"
    //     "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
    //     "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
    //     "\"DeviceName\":1,"     // Index of motion to be used for name eg garage, motion, then time from when mqtt is sent
    //     "\"State\":2" // Started
    //   "}"
    // "},"
    // "\"Rule2\":{"
    //   "\"Trigger\":{"
    //     "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
    //     "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
    //     "\"DeviceName\":2,"
    //     "\"State\":1" // FOLLOW, ie command follows trigger, or follow_inv, ie command is inverted to source
    //   "},"
    //   "\"Command\":{"
    //     "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
    //     "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
    //     "\"DeviceName\":2,"     // Index of motion to be used for name eg garage, motion, then time from when mqtt is sent
    //     "\"State\":2" // Started
    //   "}"
    "}"
  "}";

#endif


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Livingroom ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

/**
 * Testing without ultrasonics until I get stable inside/outside PIR working
 * */

/**
 * Via Connector: Ethernet 1
 * w/o    
 * o/w
 * w/g
 * bl/w
 * w/bl
 * g/w
 * w/br  Gnd for Camera
 * br/w  9v  for Camera
 * 
 * 
 * Direct: connection to GPIO (ie future prototype boards)
 * 
 * 
 * Future RJ11, 6 or 4 pin varient of telephone connector for simplier connections
 * eg power,gnd, serial rx, tx (nextion)
 * eg power, G, I2C, I2C
 * eg power, G, Lux I2C needs 2, Motion
 * 
 * 
 * livingroom sensor will be divided into "drivewaysensor", where driveway will be esp32_cam + PIR and maybe ultrasonic
 * 
 * */
#ifdef DEVICE_LIVINGROOMSENSOR
  #define DEVICENAME_CTR          "livingroomsensor"
  #define DEVICENAME_FRIENDLY_CTR "Living Room Sensor"
    
  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_MOTION
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_BME

  #define USE_MODULE_CORE_RULES

  #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "LivingRoom"
  #define D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "DriveFront"
  // #define D_DEVICE_SENSOR_MOTION_2_FRIENDLY_NAME_LONG "LivingRadar" // Test device

  #define D_DEVICE_SENSOR_DRIVEWAY_ULTRSONIC_FRIENDLY_NAME_LONG "DriveFrontUltra"

  // #define USE_MODULE_SENSORS_ANALOG

  #define USE_MODULE_SENSORS_ULTRASONICS
  // #define USE_AMBIENT_TEMP_SENSOR_FOR_SPEEDOFSOUND
  
  #define D_DEVICE_SENSOR_CLIMATE "LivingRoom"

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_BME
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_ULTRASONICS
      "\"D8\":\"" D_GPIO_FUNCTION_SR04_ECHO_CTR     "\"," //d0 to d3
      "\"D3\":\"" D_GPIO_FUNCTION_SR04_TRIG_CTR     "\","
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D7\":\"" D_GPIO_FUNCTION_SWT1_INV_CTR "\","
      "\"D6\":\"" D_GPIO_FUNCTION_SWT2_INV_CTR "\""
      #endif
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_ULTRASONIC_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_DRIVEWAY_ULTRSONIC_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

  /**
   * Add new function, if we try to access sensor name and none exists, then return the classname + number (which will be reading its internal sensor count)
   */

  
  /**
   * In the future, make a way to push this exact rule via single command (append new rule, start using vectors for indexing?)
   * 
   * */
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Switch0 HIGH = Motion0 Event Started, ie report as motion with motion name
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":1" // FOLLOW, ie command follows trigger, or follow_inv, ie command is inverted to source
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":0,"     // Index of motion to be used for name eg garage, motion, then time from when mqtt is sent
        "\"State\":1" // Started
      "}"
    "},"
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":1,"
        "\"State\":1" // FOLLOW, ie command follows trigger, or follow_inv, ie command is inverted to source
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":1,"     // Index of motion to be used for name eg garage, motion, then time from when mqtt is sent
        "\"State\":1" // Started
      "}"
    "}"
  "}";


#endif

#ifdef DEVICE_RGBFIREPLACE
  #define DEVICENAME_CTR          "rgbfireplace"
  #define DEVICENAME_FRIENDLY_CTR "RGB Fire Place 32"

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define STRIP_SIZE_MAX 300//256 + ~50
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":30,"
      "\"" D_JSON_RATE_MS "\":100,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":10,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":\"" "FirePlace01" "\""
    "},"
    "\"PaletteGeneration\":{\"RandomiseBrightnessMode\":1},"
    "\"ColourPalette\":\"Single Fire 01\","
    "\"BrightnessRGB\":100"
  "}";

#endif




#ifdef DEVICE_SHELLYDIMMER_LIVING_LAMP
  #define DEVICENAME_CTR          "dimmer_living_lamp"
  #define DEVICENAME_FRIENDLY_CTR "Dimmer LivingRoom Lamp tbc ceiling"
  
  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2

  // #define ENABLE_DEVFEATURE_SHELLYDIMMER2_INVERTED_EDGE_FOR_ERROR
  
  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_CORE_RULES
  #define USE_HARDWARE_DEFAULT_RULES_1

  #define DISABLE_SERIAL_LOGGING //temp measure
  // #define DISABLE_SERIAL0_CORE //dont think its needed

  #define USE_MODULE_DRIVERS_SHELLY_DIMMER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY_DIMMER2_CTR "\""
  "}";

  #define D_DEVICE_OUTPUT1_FRIENDLY_NAME_LONG "Light"
  #define D_DEVICE_SWITCH1_FRIENDLY_NAME_LONG "Switch1"
  #define D_DEVICE_SWITCH2_FRIENDLY_NAME_LONG "Switch2"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_SHELLY_DIMMER_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_OUTPUT1_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SWITCH1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SWITCH2_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif



/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Hallway ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


#ifdef DEVICE_RADIATORFAN
  /* Single Relay controls TWO 12V DC fans to improve heating (1 D_Out)
   * Measure PWM speed of motors (2 D_in) (Needs level shifter from 12v to 3.3v)
   * Motion sensor (1 D_in)
   * THREE DS18b20 to measure inside/outside of vent area (1 D_io)
   *  - (3) on radiator pipe, to be used as fan trigger when set to auto mode
   * One DHT22 for climate sensing -- towards door (1 D_io)
   * One BME280 for improved climate sensing -- towards hallway (1 I2C) */
  #define DEVICENAME_CTR          "radiatorfan"
  #define DEVICENAME_FRIENDLY_CTR "Radiator Fan"
  
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION
  #define USE_MODULE_SENSORS_DS18X
  #define USE_MODULE_SENSORS_DHT

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_CONTROLLER_RADIATORFAN

  #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Hallway"
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"

/**
 * o/w 5v        dht_5v, 
 * w/o ground    dht_g, bme_g, db18_g
 * g/w 3v        bme_3v, db18_3v
 * w/g           dht_sig
 * bl/w          db18_sig
 * w/bl
 * br/w I2c      bme_a
 * w/br I2c      bme_d
 * */

/**
 * 3 pin
 * PIR
 * 
 * 
 * */

/**
 * 3pin
 * fan connector
 * *
 * 12v
 * pwm recieved
 * ground
 * 
 * */


      #ifdef USE_MODULE_SENSORS_DHT
      "\"D0\":\"" D_GPIO_FUNCTION_DHT22_1_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_BME
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DS18X
      "\"D6\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR     "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"D5\":\"" D_GPIO_FUNCTION_REL1_INV_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D7\":\"" D_GPIO_FUNCTION_SWT1_CTR     "\","
      #endif 
      "\"D4\":\""  D_GPIO_FUNCTION_LED1_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Fan"
  #define MAX_RELAYS 1

  #define D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "OutsideVent"
  #define D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "InsideVent"
  #define D_DEVICE_TEMP_RADIATOR_FRIENDLY_NAME_LONG "RadiatorPipe"
  #define D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG "Hallway" //BME primary
  #define D_DEVICE_TEMP_4_FRIENDLY_NAME_LONG "Hallway-DHT" //secondary
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_RADIATOR_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_DHT_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_TEMP_4_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["                
        "[40,170,231,153,29,19,02,43],"
        "[40,170,221,101,29,19,02,113],"    
        "[40,159,147,  2, 0, 0, 0,117]" //hex  28:9F:93:2:0:0:0:75    // Longer wire on radiator pipe (this will be used to trigger fan)
      "]"  
    "}"
  "}";

  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Switch0 HIGH = Motion0 Event Started, ie report as motion with motion name
     "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":0," 
        "\"State\":\"Follow\""
      "}"
    "}"
  "}";


#endif


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Understairs ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

/***
 * Gen 2 will stickw ith current wiring
 *  - I will add an ethernet wire down the house, if just temporary for new internet
 * 
 * 
 * Gen 3
 * New Split Heating controller
 * "heating" as primary
 * "heating_remote_1" as device in hotpress
 *    - remote tank sensors
 *    - remote upstairs temperatures
 *    - relay control for immersion
 * (initally connected via mqtt in json format, eventually socket directly but also by json format)
 * (this allows both to be connected via ethernet indirectly with switches, future esp32-poe)
 * 
 * */


#ifdef DEVICE_HEATING_ESP32

/**
 * 1)
 * Transition hardware, will use esp32_A for control of relays 
 * Nodemcu will continue to measure db18 sensors
 * 
 * 2) Esp32_b will be introduced, to measure just the downstairs db sensors. At this point
 * there will be 3 active esp's (1 esp8266 and 2 esp32)
 * 
 * 3) Adding the new downstairs sensors, into the primary esp32 heating
 * 
 * 4) Adding all sensors into primary esp32
 * 
 * */

  #define DEVICENAME_CTR          "heating"  // to be named "hvac_house_master" with later "hvac_house_slave" for remote controls.
  #define DEVICENAME_FRIENDLY_CTR "HVAC Heating Gen 2" // Gen 3 = Ethernet version

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1 //maintain other settings (bootcount)
   
  // #define ENABLE_BUG_TRACING
  //#define ENABLE_MQTT_DEBUG_MESSAGES
  #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  #define EMABLE_DEVFEATURE_HARDWAREPINS_CLEANED_UP

  #define DISABLE_WEBSERVER
  #define ESP32

  #define USE_MODULE_CONTROLLER_HVAC

  //add 3 LDRs onto the motor neons, so I can check if they are turned on ((hot glue them on))
  
  // #define USE_MODULE_SENSORS_INTERFACE  
  // #define USE_MODULE_SENSORS_DHT
  // #define USE_MODULE_SENSORS_DS18X
  
  // #define ENABLE_DEVFEATURE_ESP32_FORCED_DB18S20_GPIO1_SENSOR_COUNT 8 // This should not be needed
  // #define ENABLE_DEVFEATURE_ESP32_FORCED_DB18S20_GPIO2_SENSOR_COUNT 5

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY

  #define GPIO_NAME_ZONE0_UPSTAIRS_RELAY    D_GPIO_FUNCTION_REL1_INV_CTR
  #define GPIO_NAME_ZONE1_DOWNSTAIRS_RELAY  D_GPIO_FUNCTION_REL2_INV_CTR
  #define GPIO_NAME_ZONE3_IMMERISON_RELAY   D_GPIO_FUNCTION_REL3_CTR
  #define GPIO_NAME_ZONE4_BOILER_RELAY      D_GPIO_FUNCTION_REL4_INV_CTR

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"

    // Phase 1 - Just relays, no sensors


      "\"5\":\"" GPIO_NAME_ZONE0_UPSTAIRS_RELAY    "\","
      "\"19\":\"" GPIO_NAME_ZONE1_DOWNSTAIRS_RELAY  "\","
      "\"21\":\"" GPIO_NAME_ZONE3_IMMERISON_RELAY   "\","
      "\"18\":\"" GPIO_NAME_ZONE4_BOILER_RELAY      "\","

      // // "\"12\":\"" D_GPIO_FUNCTION_REL5_INV_CTR  "\"," //spare relay inside DS device

      // "\"18\":\"" D_GPIO_FUNCTION_DHT22_1_CTR   "\"," // US_DHT 

      // /**
      //  * Upstairs
      //  * */
      // "\"5\":\"" D_GPIO_FUNCTION_DHT22_2_CTR   "\"," // DS_DHT- 3 pin connector


      // "\"22\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","  // US_DB - 3 pin
      // // "\"23\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\","  // DS_DB - 3 pin

      // // "\"23\":\""  D_GPIO_FUNCTION_DS18X20_2_CTR "\","
      // // Understairs control panel with basic controls, would be great if the GUI can output a json command when pressed? would simplify design. Though need output, so needs its own control stuff
      // "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\"," // TBA
      // "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\"," // TBA
      // // "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\"," //future bme


      // "\"25\":\"" GPIO_NAME_ZONE0_UPSTAIRS_RELAY    "\","
      // "\"26\":\"" GPIO_NAME_ZONE1_DOWNSTAIRS_RELAY  "\","
      // "\"27\":\"" GPIO_NAME_ZONE3_IMMERISON_RELAY   "\","
      // "\"14\":\"" GPIO_NAME_ZONE4_BOILER_RELAY      "\","

      // // "\"12\":\"" D_GPIO_FUNCTION_REL5_INV_CTR  "\"," //spare relay inside DS device

      // "\"18\":\"" D_GPIO_FUNCTION_DHT22_1_CTR   "\"," // US_DHT 

      // /**
      //  * Upstairs
      //  * */
      // "\"5\":\"" D_GPIO_FUNCTION_DHT22_2_CTR   "\"," // DS_DHT- 3 pin connector


      "\"23\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","  // US_DB - 3 pin  // try all on the same pin? in practice it probably would be better with different pullup values, but long term phasing out so for now, keep it simple
      // // "\"23\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\","  // DS_DB - 3 pin

      // // "\"23\":\""  D_GPIO_FUNCTION_DS18X20_2_CTR "\","
      // // Understairs control panel with basic controls, would be great if the GUI can output a json command when pressed? would simplify design. Though need output, so needs its own control stuff
      // "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\"," // TBA
      // "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\"," // TBA
      // // "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\"," //future bme
      // // "\"23\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","

/**
 * 
 * LDRs onto input only 30 gpios
 * */


      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\"" //builti n led
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  
  #define D_DEVICE_DRIVER_RELAY_0_NAME "Upstairs"
  #define D_DEVICE_DRIVER_RELAY_1_NAME "Downstairs"
  #define D_DEVICE_DRIVER_RELAY_2_NAME "Immersion"
  #define D_DEVICE_DRIVER_RELAY_3_NAME "Boiler"

  #define D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "Upstairs"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "Downstairs"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "Immersion"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE3_NAME "Boiler"

  #define D_DEVICE_SENSOR_DHT_0_NAME "Upstairs_DHT"
  #define D_DEVICE_SENSOR_DHT_1_NAME "Downstairs_DHT"
//flip these
  #define D_DEVICE_SENSOR_DB18S20_0_NAME        "Downstairs_Pipe"
  #define D_DEVICE_SENSOR_DB18S20_0_ADDRESS     "[40,255,152,171,193,23,4,231]"
  #define D_DEVICE_SENSOR_DB18S20_1_NAME        "Upstairs_Pipe"
  #define D_DEVICE_SENSOR_DB18S20_1_ADDRESS     "[40,255,131,6,194,23,4,59]"
  #define D_DEVICE_SENSOR_DB18S20_2_NAME        "Boiler_Pipe"
  #define D_DEVICE_SENSOR_DB18S20_2_ADDRESS     "[40,255,204,226,193,23,4,30]"
  #define D_DEVICE_SENSOR_DB18S20_3_NAME        "Immersion_Heater"
  #define D_DEVICE_SENSOR_DB18S20_3_ADDRESS     "[40,255,136,105,53,22,4,114]"
  #define D_DEVICE_SENSOR_DB18S20_4_NAME        "Tank_Top"
  #define D_DEVICE_SENSOR_DB18S20_4_ADDRESS     "[40,255,50,176,193,23,4,197]"
  #define D_DEVICE_SENSOR_DB18S20_5_NAME        "Tank_Middle"
  #define D_DEVICE_SENSOR_DB18S20_5_ADDRESS     "[40,255,216,108,53,22,4,102]"
  #define D_DEVICE_SENSOR_DB18S20_6_NAME        "Tank_Bottom"
  #define D_DEVICE_SENSOR_DB18S20_6_ADDRESS     "[40,255,162,167,53,22,4,27]"
  #define D_DEVICE_SENSOR_DB18S20_7_NAME        "Tank_Out"
  #define D_DEVICE_SENSOR_DB18S20_7_ADDRESS     "[40,255,219,93,53,22,4,239]"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_DRIVER_RELAY_0_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_2_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_3_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_1_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_2_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_3_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_4_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_5_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_6_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_7_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DHT_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DHT_1_NAME "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE3_NAME "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":[" 
        D_DEVICE_SENSOR_DB18S20_0_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_1_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_2_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_3_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_4_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_5_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_6_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_7_ADDRESS ""
      "]"  
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DHT_1_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_4_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_5_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_0_NAME "\"," // Also an array to match heating/cooling
          "\"" "HVAC_Type" "\":[" "\"Heating\",\"Cooling\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_2_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Cooling\"" "]"
        "}"
      "]"
    "}"
  "}";
  
#endif



/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Outside ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**
 * Device Type: Shelly 1
 * Modules:   
 *  - 2 Mains " D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "
 *  - 2 Mains Inputs
 *  - Energy monitoring (Voltage, Current and power)
 *  - Reset button on PCB
 *  - LED
 * Notes: Requires neutral
 * 
 * Default configuration
 * Mode A (Inputs are switches eg basic wall switch)
 * Mode B (Inputs are motion events eg security light)
 * 
 * */
#ifdef DEVICE_SIDEDOORLIGHT
  #define DEVICENAME_CTR          "sidedoorlight"
  #define DEVICENAME_FRIENDLY_CTR "Side Door Motion Light"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2
   
  /*
    Method should only activate if boot loop happens 10 times
    Method A: Switch to Wifi.begin hardcoded ssid/pw, start OTA and wait, rebooting every 10 minutes if wifi does not connect
    Method B: Begin wifi.ap as host, so I can directly connect to it via x.x.x.x
  */
  //#define DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS
  #define DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS

  #define ENABLE_DEVFEATURE_RELAY_ENABLE_TIME_WINDOW_LOCKS

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION

  #define USE_MODULE_NETWORKS_MQTT

  #define USE_MODULE_CORE_RULES

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
  #define MAX_RELAYS 1
  
  #define USE_MODULE_DRIVERS_RELAY
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY1_CTR "\""
  "}";
    
  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Side Door"
  #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Driveway Middle" 
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\""
      "]"    
    "},"
    "\"RelayEnabled0\":{\"Enabled\":1,\"OnTime\":\"00D15:00:00\",\"OffTime\":\"00D09:00:00\"}"
  "}";


  #ifdef DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Switch0 Toggle = Relay0 Power Toggle
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2" // 3 (or other) means follow, so copy input from trigger
      "}"
    "}"
  "}";
  #endif // DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS

  /**
   * Motion needs to change, to instead be a rule. ie.
   * 
   * Switch, button, distance etc changes will trigger a rule which fires the motion detection class. This will then respond via mqtt that event/sensor input "X" occured, and what time etc.
   * One rule will be required for direction, ie motion started (button low) and motion over (button high)
  
   * Similarly, switch change rule will also need to set the relays to be commanded based on how long I want
   * Example
   * Rule0
   * - Switch 0 = Low, Motion0 started
   * Rule1
   * - Switch 0 = low, Relay0 on for X minutes   (time of day on relay operation will be controlled via relay_commands, to set operation time ranges)
   * Rule2
   * - Switch 1 = Low, Motion1 started
   * Rule3
   * - Switch 1 = low, Relay1 on for X minutes
   * */
  #ifdef DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Switch0 HIGH = Relay0 Power ON for Timed seconds
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":1"
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOn\\\":60}}\""
      "}"
    "},"
    // Switch0 HIGH = Motion0 Event Started, ie report as motion with motion name
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":1" // FOLLOW, ie command follows trigger, or follow_inv, ie command is inverted to source
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":0,"     // Index of motion to be used for name eg garage, motion, then time from when mqtt is sent
        "\"State\":1" // Started
      "}"
    "}"
  "}";
  #endif // DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS




#endif


#ifdef DEVICE_GAZEBO_SONOFF_4CHPRO
  #define DEVICENAME_CTR          "gazebo_sonoff_4chpro"
  #define DEVICENAME_FRIENDLY_CTR "Sonoff 4CH Pro"
  
  #define USE_MODULE_CORE_RULES
  
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE_DRIVERS_RELAY
  #define USE_MODULE_DRIVERS_RF433_RCSWITCH

  // default key# = relay#
  // RF Key433 using rules to match patterns then need limit to how often RF matches 

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SONOFF_4CHPRO_CTR "\""
  "}";

  // {"RfReceived":{"Data":11071155,"Bits":24,"Protocol":1,"Pulse":191,"millis":83687314,"Time":"18:48:02"}} // black doorbell

#endif


// gazebosensor for motion, light, temperature... these may all become esp32 with wired POE

/**
 * @brief Presence detector
 * - PIR 
 * - BME280
 * - BH1750
 * 
 * Ethernet Interface Layout A [gnd, 5v, pir, -, -, 3v3, i2c_data, i2c_clock] where [w/o, o/w, w/g, bl/w, w/bl, g/w, w/br, br/w] == kitchen/utility are the same, probably make the same for livingroom, same room? 3d print a box
 */


#ifdef DEVICE_GAZEBO_SENSOR //new esp32
  #define DEVICENAME_CTR          "gazebosensor"
  #define DEVICENAME_FRIENDLY_CTR "Gazebo Sensor"

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_BH1750
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION

  #define   ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  #define D_EFFECT_INSIDE_TEMPLATE "Effects"
  #define STRIP_SIZE_MAX 100                                                                           // Change: Set *total* length of string, segment0 will default to this length
  #define ENABLE_FEATURE_INCLUDE_WLED_PALETTES
  #define ENABLE_CRGBPALETTES_IN_PROGMEM
  #define ENABLE_DEVFEATURE_SHIMMERING_PALETTE_BRIGHTNESS_LIMIT
  #define ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS
  // #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  // #define USE_SK6812_METHOD_DEFAULT

  // Hard coded to alternate pin until I make this template based
  #define ENABLE_DEVFEATURE_SET_ESP32_RGB_DATAPIN_BY_TEMPLATE
  #define PINSET_TEMP_METHOD_RGB_PIN_RGB 4
  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS2812" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":15," 

    "\"PaletteEdit\": {"
    "\"ColourPalette\": 15,"
    "\"Data\": ["
      "4,6,0,0,0,"
      "252, 3, 45,"
      "252, 40, 3,"
      "252, 3, 177,"
      "128, 1, 122"
    "]"
  "},"
    

    "\"Effects\":{"
      "\"Function\":\"Static\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":900,"
      "\"RateMs\":1000"
    "},"    
    "\"BrightnessRGB\":100"
  "}";



  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"      
      #ifdef USE_MODULE_SENSORS_BME
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"23\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"5\":\""  D_GPIO_FUNCTION_SWT1_CTR "\","
      #endif
      "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\"," // ENABLE_DEVFEATURE_SET_ESP32_RGB_DATAPIN_BY_TEMPLATE forcing this, not working by pinused
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "Gazebo"


  #define D_DEVICE_SENSOR_CLIMATE "Outside"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE "\""
      "],"  
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
      "]"  
    "},"
    "\"" D_JSON_ENERGY "\":{"
        "\"DeviceCount\":12"    
    "}"
  "}";

  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Switch0 HIGH = Motion0 Event Started, ie report as motion with motion name
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":0," 
        "\"State\":\"Follow\""
      "}"
    "}"
  "}";

#endif


#ifdef DEVICE_H801_RGBGAZEBO
  #define DEVICENAME_CTR          "h801_rgbgazebo"
  #define DEVICENAME_FRIENDLY_CTR "H801 RGB Gazebo RGBCCT Strip"
  #define DEVICENAME_ROOMHINT_CTR "Gazebo"

  /**
   * 24W/meter
   * Approx 5.5m used, 24*5.5 = 132W so 132/12 = 11Amps at 12V
   * 20A 12V should be enough
   * */

  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  #define ENABLE_DEVFEATURE_CHECK_SEGMENT_INIT_ERROR
  #define DEBUG_TARGET_ANIMATOR_SEGMENTS
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_PWM  
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  #define MAX_NUM_SEGMENTS 5  
  #define ENABLE_FEATURE_INCLUDE_WLED_PALETTES
  #define ENABLE_CRGBPALETTES_IN_PROGMEM
  #define ENABLE_DEVFEATURE_SHIMMERING_PALETTE_BRIGHTNESS_LIMIT
  #define ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS

  #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT
  
  #define USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"1\":\""  D_GPIO_FUNCTION_LED1_CTR "\","
      "\"5\":\""  D_GPIO_FUNCTION_LED2_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_H801_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  
  #define STRIP_SIZE_MAX 1
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\"" "RGBCCT_PWM" "\","
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"GRBCW\","
    "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20\"},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"RGBCCTColour 01\","
    "\"" D_JSON_ANIMATIONMODE  "\":\"" D_JSON_EFFECTS "\","
    "\"" D_JSON_EFFECTS        "\"{\"Function\":\"Solid RGBCCT\"},"
    "\"" D_JSON_BRIGHTNESS     "\":100"
  "}";

#endif

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Garage ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


#ifdef DEVICE_OILTANK // ESP32
  #define DEVICENAME_CTR          "oiltank"
  #define DEVICENAME_FRIENDLY_CTR "Oil Tank"

  #define USE_MODULE_CONTROLLER_TANKVOLUME
    
  #define USE_MODULE_SENSORS_DS18X
  
  // #define USE_MODULE_SENSORS_ULTRASONICS
  #define USE_MODULE_SENSORS_SR04
  // #define USE_AMBIENT_TEMP_SENSOR_FOR_SPEEDOFSOUND
  // #define ENABLE_DEVFEATURE_ULTRASONIC_DURATION_RAW_THRESHOLD_CHECK

  #define ENABLE_DEVFEATURE_SR04_FILTERING_EMA
  #define ENABLE_DEVFEATURE_SR04_FILTERING_DEMA

  #define ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION


  // #define ENABLE_DEVFEATURE_DELAYED_RESTART_WITHOTA_FOR_DEBUGGING
  // #define ENABLE_DEVFEATURE_OTAFALLBACK_WITH_FASTBOOT_DETECTED //to be done long term

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"      
      #ifdef USE_MODULE_SENSORS_ULTRASONICS
      "\"18\":\"" D_GPIO_FUNCTION_SR04_ECHO_CTR   "\","
      "\"19\":\"" D_GPIO_FUNCTION_SR04_TRIG_CTR  "\","  
      #endif 
      #ifdef USE_MODULE_SENSORS_SR04
      "\"19\":\"" D_GPIO_FUNCTION_SR04_ECHO_CTR   "\","
      "\"18\":\"" D_GPIO_FUNCTION_SR04_TRIG_CTR  "\","  
      #endif 
      "\"27\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      "\"2\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "AmbientTank"
  #define D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "SpeedOfSound_Ambient"
  #define D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG "BackUpTank"
  
  #define D_DEVICE_BUTTON_1_CTR "FurnaceActive"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "[40,226,218,149,240,1,60,142],"                                          
        "[40,255,100,29,205,201,168,203],"                                             
        "[40,255,100,29,205,248,248,249]"  
      "]"  
    "},"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1}"
  "}";

#endif


#ifdef DEVICE_OILFURNACE
  #define DEVICENAME_CTR          "oilfurnace"
  #define DEVICENAME_FRIENDLY_CTR "Oil Furnace"

  /**
   * 4 pipe sensors (boiler_in, boiler_out, exhaust, garage radiator)
   * light sensor for lock_out (analog and digital)
   * furnace power  
   * bme ambient
   * motion sensor
   */

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_ADC_INTERNAL_ESP8266
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_DS18X
  #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_MOTION
  
  #define USE_MODULE_CORE_RULES

  #define USE_MODULE_CONTROLLER_FURNACE_SENSOR

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{" 
      #ifdef USE_MODULE_SENSORS_BME
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"D5\":\"" D_GPIO_FUNCTION_SWT1_CTR  "\","
      "\"D6\":\"" D_GPIO_FUNCTION_SWT2_CTR  "\","
      #endif      
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D0\":\"" D_GPIO_FUNCTION_SWT3_INV_CTR  "\","
      #endif
      #ifdef USE_MODULE_SENSORS_ADC_INTERNAL_ESP8266
      "\"A0\":\"" D_GPIO_FUNCTION_ANALOG_INPUT0_CTR "\","
      #endif
      "\"D7\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  
  #define D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG  "FurnaceInflow"
  #define D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG  "FurnaceOutflow"
  #define D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG  "Exhaust"
  #define D_DEVICE_TEMP_4_FRIENDLY_NAME_LONG  "GarageRadiator"
  #define D_DEVICE_SENSORS_ANALOG_1_CTR       "LockOut Lamp Luminance"
  #define D_DEVICE_SENSORS_SWITCH_1_CTR       "FurnaceActive"
  #define D_DEVICE_SENSORS_SWITCH_2_CTR       "FurnaceLockOut"
  #define D_DEVICE_SENSORS_SWITCH_3_CTR       "PIR"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Garage"
  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Garage"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_4_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_ANALOG_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSORS_ANALOG_1_CTR "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "]"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSORS_SWITCH_1_CTR "\","
        "\"" D_DEVICE_SENSORS_SWITCH_2_CTR "\","
        "\"" D_DEVICE_SENSORS_SWITCH_3_CTR "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["    
        "[40,0,37,149,240,1,60,0],"
        "[40,124,194,149,240,1,60,110],"
        "[40,199,255,149,240,1,60,249],"
        "[40,246,153,149,240,1,60,174]"
      "]"  
    "}"
  "}";

  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Switch0 HIGH = Motion0 Event Started, ie report as motion with motion name
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":2,"
        "\"State\":1" // FOLLOW, ie command follows trigger, or follow_inv, ie command is inverted to source
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":0,"     // Index of motion to be used for name eg garage, motion, then time from when mqtt is sent
        "\"State\":2" // Started
      "}"
    "}"
  "}";


#endif


#ifdef DEVICE_GARAGELIGHT
  #define DEVICENAME_CTR          "garagelight"
  #define DEVICENAME_FRIENDLY_CTR "Garage Security Lights 2"

  #define USE_SSIDS_OUTSIDE_HOUSE
  
  /*
    Method should only activate if boot loop happens 10 times
    Method A: Switch to Wifi.begin hardcoded ssid/pw, start OTA and wait, rebooting every 10 minutes if wifi does not connect
    Method B: Begin wifi.ap as host, so I can directly connect to it via x.x.x.x
  */
  //#define DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS
  #define DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS

  #define ENABLE_DEVFEATURE_RELAY_ENABLE_TIME_WINDOW_LOCKS

  #define USE_MODULE_ENERGY_INTERFACE
  #define USE_MODULE_ENERGY_ADE7953

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE_SENSORS_MOTION

  #define USE_MODULE_NETWORKS_MQTT

  #define USE_MODULE_CORE_RULES

  #define USE_MODULE_DRIVERS_RELAY
  #define MAX_RELAYS 2
  #define USE_MODULE_DRIVERS_INTERFACE

  // #define ENABLE_DEVFEATURE_OTA_FALLBACK_ON_BOOT_LOOPING
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY2P5_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Driveway"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Garden"
  #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Driveway Top"
  #define D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "Back Garden"  
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    //device_names:{"module_name":["relay1","relay2"]}
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "\""
      "]"    
    "},"
    "\"RelayEnabled0\":{\"Enabled\":1,\"OnTime\":\"00D16:00:00\",\"OffTime\":\"00D09:00:00\"},"
    "\"RelayEnabled1\":{\"Enabled\":1,\"OnTime\":\"00D16:00:00\",\"OffTime\":\"00D09:00:00\"}"
  "}";


  #ifdef DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Switch0 Toggle = Relay0 Power Toggle
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"Switches\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"Module\":\"Relays\","
        "\"Function\":\"SetPower\","
        "\"DeviceName\":0,"
        "\"State\":2" // 3 (or other) means follow, so copy input from trigger
      "}"
    "},"
    // Switch1 Toggle = Relay1 Power Toggle
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"Switches\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":1,"
        "\"State\":2"      // 2 meaning either low or high, 1 would be high only
      "},"
      "\"Command\":{"
        "\"Module\":\"Relays\","
        "\"Function\":\"SetPower\","
        "\"DeviceName\":1,"
        "\"State\":2" // 3 (or other) means follow, so copy input from trigger
      "}"
    "},"
    // Button0 Single Press = Relay0 Power On for 10 seconds tester
    "\"Rule2\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_BUTTONS_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2" // 
      "},"
      "\"Command\":{"
        "\"Module\":\"Relays\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOn\\\":10}}\""
      "}"
    "}"
  "}";
  #endif // DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS

  /**
   * Motion needs to change, to instead be a rule. ie.
   * 
   * Switch, button, distance etc changes will trigger a rule which fires the motion detection class. This will then respond via mqtt that event/sensor input "X" occured, and what time etc.
   * One rule will be required for direction, ie motion started (button low) and motion over (button high)
   * 
   * Similarly, switch change rule will also need to set the relays to be commanded based on how long I want
   * 
   * Example
   * 
   * Rule0
   * - Switch 0 = Low, Motion0 started
   * 
   * Rule1
   * - Switch 0 = low, Relay0 on for X minutes   (time of day on relay operation will be controlled via relay_commands, to set operation time ranges)
   * 
   * Rule2
   * - Switch 1 = Low, Motion1 started
   * 
   * Rule3
   * - Switch 1 = low, Relay1 on for X minutes
   * 
   * */
  #ifdef DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Switch0 HIGH = Relay0 Power ON for Timed seconds
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"Switches\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":1"
      "},"
      "\"Command\":{"
        "\"Module\":\"Relays\","
        "\"Function\":\"SetPower\","
        "\"DeviceName\":0,"
        // "\"State\":2" // 3 (or other) means follow, so copy input from trigger
        "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOn\\\":300}}\""
      "}"
    "},"
    // Switch1 HIGH = Relay1 Power ON for Timed seconds
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"Switches\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":1,"
        "\"State\":1"      // 2 meaning either low or high, 1 would be high only
      "},"
      "\"Command\":{"
        "\"Module\":\"Relays\","
        "\"Function\":\"SetPower\","
        "\"DeviceName\":1,"
        // "\"State\":2" // 3 (or other) means follow, so copy input from trigger
        "\"JsonCommands\":\"{\\\"PowerName\\\":1,\\\"Relay\\\":{\\\"TimeOn\\\":120}}\""
      "}"
    "},"
    // Switch0 HIGH = Motion0 Event Started, ie report as motion with motion name
    "\"Rule2\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":1" // FOLLOW, ie command follows trigger, or follow_inv, ie command is inverted to source
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":0,"     // Index of motion to be used for name eg garage, motion, then time from when mqtt is sent
        "\"State\":1" // Started
      "}"
    "},"
    // Switch1 HIGH = Motion1 Event Started, ie report as motion with motion name
    "\"Rule3\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":1,"
        "\"State\":1" // 
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":1,"     // Index of motion to be used for name eg garage, motion, then time from when mqtt is sent
        "\"State\":1" // Started        
        // "\"JsonCommands\":\"{\\\"PowerName\\\":1,\\\"Relay\\\":{\\\"TimeOn\\\":30}}\""
      "}"
    "},"
    // Button0 Single Press = Relay0 Power On for 10 seconds tester
    "\"Rule4\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_BUTTONS_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2" // 
      "},"
      "\"Command\":{"
        "\"Module\":\"Relays\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOn\\\":10}}\""
      "}"
    "}"
  "}";
  #endif // DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS


#endif // DEVICE_NAME


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** TV Room / Spareroom ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


#ifdef DEVICE_SHELLYDIMMER_SPAREROOM_CEILING
  #define DEVICENAME_CTR          "shellydimmer_spareroom"
  #define DEVICENAME_FRIENDLY_CTR "Shelly Dimmer Spare Room"
  
  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2

  // #define ENABLE_DEVFEATURE_SHELLYDIMMER2_INVERTED_EDGE_FOR_ERROR
  
  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_CORE_RULES
  #define USE_HARDWARE_DEFAULT_RULES_1

  #define DISABLE_SERIAL_LOGGING //temp measure
  // #define DISABLE_SERIAL0_CORE //dont think its needed

  #define USE_MODULE_DRIVERS_SHELLY_DIMMER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY_DIMMER2_CTR "\""
  "}";

  #define D_DEVICE_OUTPUT1_FRIENDLY_NAME_LONG "Light"
  #define D_DEVICE_SWITCH1_FRIENDLY_NAME_LONG "Switch1"
  #define D_DEVICE_SWITCH2_FRIENDLY_NAME_LONG "Switch2"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_SHELLY_DIMMER_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_OUTPUT1_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SWITCH1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SWITCH2_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif



#ifdef DEVICE_SPAREROOM_H801_WARDROBE_UPLIGHTS
  #define DEVICENAME_CTR          "h801_spareroom_wardrobe"
  #define DEVICENAME_FRIENDLY_CTR "H801 Spareroom Wardrobe"
  
  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  #define ENABLE_DEVFEATURE_CHECK_SEGMENT_INIT_ERROR

  #define DEBUG_TARGET_ANIMATOR_SEGMENTS

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_PWM
  
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  #define D_EFFECT_INSIDE_TEMPLATE "Effects"

  #define MAX_NUM_SEGMENTS 5
  
  
  #define ENABLE_FEATURE_INCLUDE_WLED_PALETTES
  #define ENABLE_CRGBPALETTES_IN_PROGMEM
  #define ENABLE_DEVFEATURE_SHIMMERING_PALETTE_BRIGHTNESS_LIMIT
  #define ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS


  
  #define USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"1\":\""  D_GPIO_FUNCTION_LED1_CTR "\","
      "\"5\":\""  D_GPIO_FUNCTION_LED2_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_H801_CTR "\""
  "}";
 

  #define STRIP_SIZE_MAX 1
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\"" "RGBCCT_PWM" "\","
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"GRBcw\","
    "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20\"},"
    "\"" D_JSON_COLOUR_PALETTE "\":10,"
    "\"" D_JSON_ANIMATIONMODE  "\":\"" D_JSON_EFFECTS "\","
    "\"" D_JSON_EFFECTS        "\"{\"Function\":\"Solid RGBCCT\"},"//Sun Elevation RGBCCT Solid Palette 01\"},"
    "\"" D_JSON_BRIGHTNESS     "\":100"
  "}";
  
#endif



#ifdef DEVICE_AMBILIGHT_SAMSUNG_SPAREROOM
  #define DEVICENAME_CTR          "ambilight_spareroom"
  #define DEVICENAME_FRIENDLY_CTR "Ambilight Spareroom"
  #define ESP32

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  #define D_EFFECT_INSIDE_TEMPLATE "Effects"
  #define STRIP_SIZE_MAX 226                                                                           // Change: Set *total* length of string, segment0 will default to this length
  #define ENABLE_FEATURE_INCLUDE_WLED_PALETTES
  #define ENABLE_CRGBPALETTES_IN_PROGMEM
  #define ENABLE_DEVFEATURE_SHIMMERING_PALETTE_BRIGHTNESS_LIMIT
  #define ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS
  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  #define USE_SK6812_METHOD_DEFAULT

  // Hard coded to alternate pin until I make this template based
  #define ENABLE_DEVFEATURE_SET_ESP32_RGB_DATAPIN_BY_TEMPLATE
  #define PINSET_TEMP_METHOD_RGB_PIN_RGB 4
  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "SK6812" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":10," 
    "\"Hue\":25," 
    "\"Sat\":70," 
    "\"Effects\":{"
      "\"Function\":0"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":900,"
      "\"RateMs\":1000"
    "},"    
    "\"BrightnessRGB\":10"
  "}";

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\"" // ENABLE_DEVFEATURE_SET_ESP32_RGB_DATAPIN_BY_TEMPLATE forcing this, not working by pinused
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  /**
   * In the future, make a way to push this exact rule via single command (append new rule, start using vectors for indexing?)
   * */
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // PIR detected
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":1" // FOLLOW, ie command follows trigger, or follow_inv, ie command is inverted to source
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":0,"     // Index of motion to be used for name eg garage, motion, then time from when mqtt is sent
        "\"State\":2" // Started
      "}"
    "}"
  "}";

#endif

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Masterbedroom ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


#ifdef DEVICE_SHELLYDIMMER_MASTERBEDROOM_CEILING
  #define DEVICENAME_CTR          "shellydimmer_masterbedroom"
  #define DEVICENAME_FRIENDLY_CTR "Shelly Dimmer Spare Room"
    
  #define USE_MODULE_CORE_RULES
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_DRIVERS_SHELLY_DIMMER

  #define DISABLE_SERIAL_LOGGING
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY_DIMMER2_CTR "\""
  "}";

  #define D_DEVICE_DIMMER_FRIENDLY_NAME_LONG "Light"
  #define D_DEVICE_SWITCH_1_FRIENDLY_NAME_LONG "Main"
  #define D_DEVICE_SWITCH_2_FRIENDLY_NAME_LONG "Bed"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_SHELLY_DIMMER_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_DIMMER_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SWITCH_1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SWITCH_2_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"Switches\"," 
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"Module\":\"ShellyDimmer\","
        "\"Function\":\"SetPower\","
        "\"DeviceName\":0,"
        "\"State\":2,"
        "\"Value\":100"
      "}"
    "},"
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"Switches\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":1,"
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"Module\":\"ShellyDimmer\","
        "\"Function\":\"SetPower\","
        "\"DeviceName\":0,"
        "\"State\":2,"
        "\"Value\":50"
      "}"
    "}"    
  "}";


#endif

/**
 * Potentionally to be merged with landingpanel controller
 * BME, RGB
 * DHT in place, not part of sensor (wired to heating) 
 * 
 * 
 * */
#ifdef DEVICE_MASTERBEDROOMSENSOR
  #define DEVICENAME_CTR          "masterbedroomsensor"
  #define DEVICENAME_FRIENDLY_CTR "Master Bedroom Sensor"

  //#define FORCE_TEMPLATE_LOADING
     
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  #define USE_SK6812_METHOD_DEFAULT
  #define STRIP_SIZE_MAX 4

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BME

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_BME
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":4,"
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"grbwc\","
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":1000,"
      "\"" D_JSON_RATE_MS "\":1000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":8"
    "},"
    "\"" D_JSON_CCT_TEMP "\":300,"
    "\"" D_JSON_HUE "\":350,"
    "\"" D_JSON_SAT "\":100,"
    "\"" D_JSON_COLOUR_PALETTE "\":\"RGBCCTColour 01\","
    "\"" D_JSON_BRIGHTNESS_CCT "\":100,"
    "\"" D_JSON_BRIGHTNESS_RGB "\":100"
  "}";

  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "MasterBedroom"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif


#ifdef DEVICE_MASTERBEDROOM_DRESSER_LIGHTS
  #define DEVICENAME_CTR          "dresser_lights"                                      // Change: The unique mqtt topic, however, mqtt client names are appended with mac address, so for basic testing (ie of templates) it is not essential this be changed
  #define DEVICENAME_FRIENDLY_CTR "Dresser Lights"                                   // Change: You may change this, but it is not important to do so (more important when webui is functioning)
  
  #define STRIP_SIZE_MAX 50                                                                           // Change: Set *total* length of string, segment0 will default to this length
  #define PIN_NAME_STRING_ESP8266_DEFAULT   "RX"                                                      // Change: Set to the pin you want, esp8266 this will default to this anyway
  #define PIN_NAME_STRING_ESP32_DEFAULT     "23"                                                      //         Set to the pin you want, any output pin should work

  /**
   * @brief Mostly for me testing, switching between my segments or testing orginal wled effects
   **/
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  #define D_EFFECT_INSIDE_TEMPLATE "Effects"
  /**
   * @brief defines to be tested and incorporated fully
   **/
  #define ENABLE_FEATURE_INCLUDE_WLED_PALETTES
  #define ENABLE_CRGBPALETTES_IN_PROGMEM
  #define ENABLE_DEVFEATURE_SHIMMERING_PALETTE_BRIGHTNESS_LIMIT
  #define ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS
  // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS
  // #define ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
  // #define ENABLE_DEVFEATURE_WS2812FX_DEFAULT_PALETTE_EFFECTS
  // #define ENABLE_DEVFEATURE_GET_COLOUR_PALETTE_JOINT_METHOD
  // #define ENABLE_DEVFEATURE_PALETTE_ADVANCED_METHODS_GEN2 // ie the new way of merging fastled to mine
  /**
   * @brief Debug flags, used mostly be me
   **/  
  // #define ENABLE_FREERAM_APPENDING_SERIAL
  // #define DEBUG_WLED_EFFECT_FUNCTIONS
  // #define ENABLE_DEVFEATURE_LEARNING_FASTLED_PALETTES


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
    #ifdef ESP8266 // default pins for ws28xx
      "\"" PIN_NAME_STRING_ESP8266_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #else
      "\"" PIN_NAME_STRING_ESP32_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #endif
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

/**
 * @brief The following templates are tested examples
 * 
 */

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Pink purple 02\"," 
    "\"Effects\":{"
      "\"Function\":\"" D_EFFECTS_FUNCTION__STATIC__NAME_CTR "\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":2000,"
      "\"RateMs\":5000"
    "},"    
    "\"BrightnessRGB\":0"
  "}";
  

#endif




/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Ensuite ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


#ifdef DEVICE_ENSUITESENSOR
  #define DEVICENAME_CTR          "ensuitesensor"
  #define DEVICENAME_FRIENDLY_CTR "Ensuite Sensor"
     
  #define USE_MODULE_SENSORS_MOTION
  #define USE_MODULE_SENSORS_BME
  #define DISABLE_WEBSERVER

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  #define USE_SK6812_METHOD_DEFAULT

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_BME
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D6\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR     "\","
      #endif      
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":1000,"
      "\"" D_JSON_RATE_MS "\":1000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":8"
    "},"
    "\"" D_JSON_CCT_TEMP "\":300,"
    "\"" D_JSON_HUE "\":25,"
    "\"" D_JSON_SAT "\":100,"
    "\"" D_JSON_COLOUR_PALETTE "\":\"RGBCCTColour 01\"," //ie 10
    "\"" D_JSON_BRIGHTNESS_CCT "\":100,"
    "\"" D_JSON_BRIGHTNESS_RGB "\":1"
  "}";

  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Ensuite"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Ensuite"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif



#ifdef DEVICE_SHELLYDIMMER_ENSUITE_CEILING
  #define DEVICENAME_CTR          "shellydimmer_ensuite"
  #define DEVICENAME_FRIENDLY_CTR "Shelly Dimmer Ensuite"
    
  #define USE_MODULE_CORE_RULES
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_DRIVERS_SHELLY_DIMMER

  #define DISABLE_SERIAL_LOGGING
      
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY_DIMMER2_CTR "\""
  "}";

  #define D_DEVICE_DIMMER_FRIENDLY_NAME_LONG "Light"
  #define D_DEVICE_SWITCH_1_FRIENDLY_NAME_LONG "DoorSide"
  #define D_DEVICE_SWITCH_2_FRIENDLY_NAME_LONG "FarSide"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_SHELLY_DIMMER_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_DIMMER_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SWITCH_1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SWITCH_2_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"Switches\"," 
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0," //Switch 0 of 1
        "\"State\":2" // Toggle
      "},"
      "\"Command\":{"
        "\"Module\":\"ShellyDimmer\","
        "\"Function\":\"SetPower\","
        "\"DeviceName\":0,"
        "\"State\":2,"
        "\"Value\":35"
      "}"
    "},"
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"Switches\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":1," // Closest to bathroom door
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"Module\":\"ShellyDimmer\","
        "\"Function\":\"SetPower\","
        "\"DeviceName\":0," //Switch 1 of 1
        "\"State\":2,"
        "\"Value\":100"
      "}"
    "}"    
  "}";


#endif

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Main bathroom ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Landing ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

#ifdef DEVICE_SHELLYDIMMER_LANDING_CEILING
  #define DEVICENAME_CTR          "shellydimmer_landing"
  #define DEVICENAME_FRIENDLY_CTR "Shelly Dimmer Landing Room"
  
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_DRIVERS_SHELLY_DIMMER
  #define USE_MODULE_CORE_RULES

  // #define ENABLE_DEVFEATURE_DEBUG_MQTT_RECONNECT

  #define DISABLE_SERIAL_LOGGING
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY_DIMMER2_CTR "\""
  "}";

  #define D_DEVICE_DIMMER_FRIENDLY_NAME_LONG "Light"
  #define D_DEVICE_SWITCH_STAIRS_FRIENDLY_NAME_LONG "Stairs"
  #define D_DEVICE_SWITCH_BATHROOM_FRIENDLY_NAME_LONG "Landing"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_SHELLY_DIMMER_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_DIMMER_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SWITCH_STAIRS_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SWITCH_BATHROOM_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";
  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"Switches\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"Module\":\"ShellyDimmer\","
        "\"Function\":\"SetPower\"," 
        "\"DeviceName\":0,"
        "\"State\":2,"
        "\"Value\":50"
      "}"
    "},"
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"Switches\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":1,"
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"Module\":\"ShellyDimmer\","
        "\"Function\":\"SetPower\","
        "\"DeviceName\":0,"
        "\"State\":2,"
        "\"Value\":100"
      "}"
    "}"   
  "}";

#endif


/**
 * via sensor_interface, temps of all sensors will also generate a heatmap
 * */
#ifdef DEVICE_IMMERSIONSENSOR
  #define DEVICENAME_CTR          "immersionsensor"
  #define DEVICENAME_FRIENDLY_CTR "Immersion Sensor"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  #define ENABLE_FEATURE_INCLUDE_WLED_PALETTES
  #define ENABLE_CRGBPALETTES_IN_PROGMEM
  #define ENABLE_DEVFEATURE_SHIMMERING_PALETTE_BRIGHTNESS_LIMIT
  #define ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS
  
  
  #define USE_MODULE_SENSORS_INTERFACE
  #define ENABLE_DEVFEATURE_SENSORS_INTERFACE_SHOW_TEMPERATURE_AS_COLOUR
  #define USE_MODULE_SENSORS_DS18X
  
  #define USE_MODULE_CONTROLLER_SENSORCOLOURS

  #define SETTINGS_SENSORS_MQTT_IFCHANGED_PERIOD_SECONDS 60
     
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"  
      "\"D6\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","      
      "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "TankPosition00" //bottom
  #define D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "TankPosition20"
  #define D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG "TankPosition40"
  #define D_DEVICE_TEMP_4_FRIENDLY_NAME_LONG "TankPosition60"
  #define D_DEVICE_TEMP_5_FRIENDLY_NAME_LONG "TankPosition80"
  #define D_DEVICE_TEMP_6_FRIENDLY_NAME_LONG "TankPosition100" //top

  #define D_DEVICE_TEMP_7_FRIENDLY_NAME_LONG "BoilerLoopTop"
  #define D_DEVICE_TEMP_8_FRIENDLY_NAME_LONG "BoilerLoopBottom"
  #define D_DEVICE_TEMP_9_FRIENDLY_NAME_LONG "ImmersionFeedIn"
  #define D_DEVICE_TEMP_10_FRIENDLY_NAME_LONG "FeedRed" // not sure what this does, cold in it seems
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_4_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_5_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_6_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_7_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_8_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_9_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_10_FRIENDLY_NAME_LONG "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":[" 
        "[40,0,118,128,59,71,5,227],"//1   
        "[40,0,114,20,59,71,5,19],"  //2  
        "[40,0,66,109,59,71,5,172],"   //3  
        "[40,0,108,65,59,71,4,202],"  //4  
        "[40,0,83,19,59,71,6,66],"  //5         
        "[40,0,32,23,59,71,5,141],"  //6


        "[40,0,40,61,59,71,4,134],"
        "[40,0,66,140,59,71,6,136]," // not calib
        "[40,0,95,50,59,71,5,126]," // not calib
        "[40,0,149,87,59,71,5,240]" // not calib
      "]"  
    "},"
    "\"" D_JSON_SENSOR_MEASURERATE "\":{"
      // 1 second
    "}"
  "}";

  
  #define STRIP_SIZE_MAX 33

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":10000,"
      "\"" D_JSON_RATE_MS "\":1000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\""
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":\"" "Slow Glow" "\""
    "},"
    "\"ColourPalette\":\"Christmas MultiColoured Warmer\","
    "\"BrightnessRGB\":100"
  "}";
  
  // 6,5,0,0,0,
  // 0,0,0,26,
  // 50,0,26,26,
  // 100,0,26,26,
  // 150,0,64,32,
  // 200,109,156,0,
  // 255,89,148,0
  
  

#endif


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Attic ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

#ifdef DEVICE_ATTICSENSOR
  #define DEVICENAME_CTR          "atticsensor"
  #define DEVICENAME_FRIENDLY_CTR "Attic Sensor"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  
  // #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_DS18X
  #define USE_MODULE_SENSORS_DHT
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_DHT
      "\"D1\":\"" D_GPIO_FUNCTION_DHT22_1_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_BME
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DS18X
      "\"D3\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      #endif
      "\"D0\":\"" D_GPIO_FUNCTION_LED1_INV_CTR   "\","    
      "\"D4\":\"" D_GPIO_FUNCTION_LED1_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" "Attic" "\""
      "],"
      "\"" D_MODULE_SENSORS_DHT_FRIENDLY_CTR "\":["
        "\"" "Attic" "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" "Cold Water Tank" "\""
      "]"
    "}"
  "}";

#endif

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Bedroom (Long term only ie can remain when I leave home) ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


#ifdef DEVICE_BEDROOM_CEILINGFAN
  #define DEVICENAME_CTR          "bedroom_ceilingfan"
  #define DEVICENAME_FRIENDLY_CTR "Bedroom Ceiling Fan"
  
  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define USE_MODULE_CORE_RULES
  
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE_DRIVERS_BUZZER
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_CONTROLLER_SONOFF_IFAN

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SONOFF_IFAN03_CTR "\""
  "}";

#endif



#ifdef DEVICE_SHELLYDIMMER_BEDROOM_CEILING
  #define DEVICENAME_CTR          "dimmer_bedroom_ceiling"
  #define DEVICENAME_FRIENDLY_CTR "Shelly Dimmer Spare Room"
  
  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2

  // #define ENABLE_DEVFEATURE_SHELLYDIMMER2_INVERTED_EDGE_FOR_ERROR
  
  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_CORE_RULES
  #define USE_HARDWARE_DEFAULT_RULES_1

  #define DISABLE_SERIAL_LOGGING //temp measure
  // #define DISABLE_SERIAL0_CORE //dont think its needed

  #define USE_MODULE_DRIVERS_SHELLY_DIMMER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY_DIMMER2_CTR "\""
  "}";

  #define D_DEVICE_OUTPUT1_FRIENDLY_NAME_LONG "Light"
  #define D_DEVICE_SWITCH1_FRIENDLY_NAME_LONG "Switch1"
  #define D_DEVICE_SWITCH2_FRIENDLY_NAME_LONG "Switch2"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_SHELLY_DIMMER_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_OUTPUT1_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SWITCH1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SWITCH2_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif


/**
 * @brief 
 * * Ethernet Interface Layout A 
 * [w/o, o/w, w/g, bl/w, w/bl, g/w, w/br, br/w]
 * [gnd,  5v, pir,    RGB,    -, 3v3, i2c_data, i2c_clock] where  == kitchen/utility are the same, probably make the same for livingroom, same room? 3d print a box
 *
 * LAYOUT_B_2022Q1 - GPIO_ABC_NO_UART
 * 
 * This will be used to properly incorporate motion AND doorsensor opening, thus, making the utiltiysensor work properly
 * 
 * 
 * New PCB
 * 
 * - PIR GPIO_A
 * - BME
 * - BH1
 * 
 * - DOOR reed
 * - DOOR lock
 * - RGB_Data
 * 
 * [GND, 5V, PIR, ICD2, I2CC, 3V3, Reed, Lock]
 * 
 * 22 = RGB
 * 21 = PIR
 * 
 * 
 **/
#ifdef DEVICE_BEDROOMSENSOR
  #define DEVICENAME_CTR          "bedroomsensor"
  #define DEVICENAME_FRIENDLY_CTR "Bedroom Sensor"
  #define DEVICENAME_ROOMHINT_CTR "Bedroom"

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_BH1750
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION
  #define USE_MODULE_SENSORS_DOOR
  
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  #define D_EFFECT_INSIDE_TEMPLATE "Effects"
  #define STRIP_SIZE_MAX 55                                                                           // Change: Set *total* length of string, segment0 will default to this length
  #define ENABLE_FEATURE_INCLUDE_WLED_PALETTES
  #define ENABLE_CRGBPALETTES_IN_PROGMEM
  #define ENABLE_DEVFEATURE_SHIMMERING_PALETTE_BRIGHTNESS_LIMIT
  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  #define USE_SK6812_METHOD_DEFAULT

  #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "SK6812" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGBW\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" "Effects" "\"," 
    "\"ColourPalette\":\"Solid Rgbcct 01\"," 
    "\"Hue\":120," 
    "\"Sat\":100," 
    "\"Effects\":{"
      "\"Function\":\"Solid RGBCCT\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":900,"
      "\"RateMs\":1000"
    "},"    
    "\"CCT_TempPercentage\":100,"
    "\"BrightnessCCT\":100,"
    "\"BrightnessRGB\":100,"
    "\"TimeOn\":30"
  "}";

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"      
      #ifdef USE_MODULE_SENSORS_BME
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"23\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"5\":\""  D_GPIO_FUNCTION_SWT1_CTR "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DOOR
      "\"18\":\"" D_GPIO_FUNCTION_DOOR_OPEN_CTR     "\","
      "\"19\":\"" D_GPIO_FUNCTION_DOOR_LOCK_CTR     "\","
      #endif
      "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      "\"2\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "Bedroom"
  #define D_DEVICE_SENSOR_CLIMATE "Bedroom"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE "\""
      "],"  
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
      "]"  
    "}"
  "}";

  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // MOTION
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":0," 
        "\"State\":\"Follow\""
      "}"
    "}"
  "}";


#endif

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Roaming devices ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/



#ifdef DEVICE_FLOORFAN2
  #define DEVICENAME_CTR          "floorfan2"
  #define DEVICENAME_FRIENDLY_CTR "IFan Floor Fan 2"
  
  #define USE_MODULE_CORE_RULES
  
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE_DRIVERS_BUZZER
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_CONTROLLER_SONOFF_IFAN

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SONOFF_IFAN03_CTR "\""
  "}";

#endif

#endif // MFIRMWARECUSTOM_SECRET_HOME_LONGTERM_H