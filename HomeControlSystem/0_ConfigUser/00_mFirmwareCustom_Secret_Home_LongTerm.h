#ifndef MFIRMWARECUSTOM_SECRET_HOME_LONGTERM_H
#define MFIRMWARECUSTOM_SECRET_HOME_LONGTERM_H

/***
 * - CrashDump needs to be saved and transmitted over mqtt on request in a json message, that can copy and pasted into decoder
 * - Figure out how to have the binary saved into another directory for each device, so it may be used later as the "latest version" to reflash another device with the same code if needed (ie broken device that worked on older code)
 * 
 * 
 * 
 * 
 * **/


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
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"
#include "2_CoreSystem/03_HardwareTemplates/mHardwareTemplates.h"

/**
 * @brief 
 * 
 * New devices
 * 
 * masterbedroom sensor 
 *      - up high with full sensors (rj45)
 * 
 * masterbedroom/under dresser floor light for mum so it auto comes on for mum to toilet
 * 
 *  *  (Install 2 PIR outside? one beside camera)
 * 
 * Home ToDo:
 * - TAPE!! fence camera wiring (consider inline box?)
 * - TAPE!! and add plastic for backdoor camera
 * - Go around and take pictures of all boards, add a folder to keep track of the current hardware config (folder per device), so I can always compare hardware to software pins easily
 * -
 * 
 */


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
// #define DEVICE_RGBLIGHTS_GLASSBOX
// #define DEVICE_RGBCOOKER
// #define DEVICE_KITCHENSENSOR
// #define DEVICE_RGBFRIDGE
// #define DEVICE_DEFAULT_SONOFF_BASIC__KITCHEN_EXTRACTOR_FAN

/**
Utility
  - crystal1
  - pir, bme, door sensor
**/
// #define DEVICE_RGBUTILITY
// #define DEVICE_UTILITYSENSOR
// #define DEVICE_RGBLAMP_CYLINDER

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
// #define DEVICE_SHELLYDIMMER_LIVING_LAMP               // needs to work with no neutral, which I need to test and develop on desk first (as a lamp?)
// #define DEVICE_DEFAULT_SONOFF_BASIC_LAMP1
// #define DEVICE_DEFAULT_SONOFF_BASIC_LAMP2
// #define DEVICE_DEFAULT_SONOFF_BASIC_LAMP3
// #define DEVICE_DEFAULT_SHELLY_DIMMER_LAMP1
// #define DEVICE_DEFAULT_SONOFF_BASIC__LIVING_ROOM_LAMP1
// #define DEVICE_DEFAULT_SONOFF_BASIC__LIVING_ROOM_LAMP2


/**
 * @brief Attic
 **/
// #define DEVICE_ATTIC_SENSOR

/**
 * 
Hallway + Understairs
  - radiatorfan = fan controls, 2 db, bme, dht, pir
  - front door/table socket
  - hallway table
 */
// #define DEVICE_RADIATORFAN
// #define DEVICE_CANDLE_ELECTRIC_HALLWAY  // Socket_SocketNumber16_Power
// #define DEVICE_DEFAULT_SHELLY_DIMMER__HALLWAY_TABLE_LAMP
// #define DEVICE_HEATING

/**
 *  Landing + Hotpress
  - CeilingLight = dimmer, 2 inputs
  - LandingPanel = PIR, Nextion, BME, light strip
  - dht
 **/  
// #define DEVICE_SHELLYDIMMER_LANDING_CEILING
// #define DEVICE_LANDINGPANEL
// #define DEVICE_IMMERSIONSENSOR

/**
Outside + Garage
  - CeilingLight
**/
// #define DEVICE_SIDEDOORLIGHT
// #define DEVICE_GAZEBO_SONOFF_4CHPRO
// #define DEVICE_GAZEBO_SENSOR
// #define DEVICE_H801_RGBGAZEBO
// #define DEVICE_OILTANK
// #define DEVICE_OILFURNACE
// #define DEVICE_GARAGELIGHT

/**
TV Room (prev. spareroom)
  - CeilingLight
**/
// #define DEVICE_SHELLYDIMMER_TVROOM_CEILING
// #define DEVICE_TVROOM_H801_UPLIGHTS
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
// #define DEVICE_MASTERBEDROOMSENSOR
// #define DEVICE_FLOORFAN3                                         // new silver fan to buy
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
// #define DEVICE_DEFAULT_SONOFF_BASIC_ENSUITE_CEILING_FAN

/**
Bathroom
  - home mini
  - shellydimmer on pull cord?
**/
// #define DEVICE_BATHROOM_SENSOR                                     // same as other rooms

/**
 * Bedroom
 - ceilingfan
 - bedroomsensor (climate, motion, door) = remove it as a tester
 - rgbbedroomclock
 - candlewarmer
 **/
// #define DEVICE_BEDROOMSENSOR
// #define DEVICE_BEDROOM_CEILINGFAN
// #define DEVICE_H801_INSIDE_BEDROOM_WARDROBE

// #define DEVICE_DEFAULT_SHELLY_DIMMER__BEDROOM_GLOBE
// #define DEVICE_BEDROOM_LOUVOLITE_HUB

/**
 * Roaming
 * 
 * - Airpurifier
 * - Candle Warmer
 * - Floor fan 1
 * 
 * */
// #define DEVICE_AIRPURIFIER
// #define DEVICE_FLOORFAN1
// #define DEVICE_FLOORFAN2


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Generic devices -- Sonoff Basic ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

/**
 * @brief instead of ini, configure them here
 **/
#ifdef DEVICE_DEFAULT_SONOFF_BASIC_x
  #define DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_SOCKET_NUMBER_CTR 01
#endif
#ifdef DEVICE_DEFAULT_SONOFF_BASIC__BLACK_SHORT
  #define DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_SOCKET_NUMBER_CTR 02
  #define DEVICENAME_ROOMHINT_CTR "Roaming"  
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
#endif
#ifdef DEVICE_DEFAULT_SONOFF_BASIC__LIVING_ROOM_LAMP1
  #define DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_SOCKET_NUMBER_CTR 03
  #define DEVICENAME_ROOMHINT_CTR "Livingroom"  
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
#endif
#ifdef DEVICE_DEFAULT_SONOFF_BASIC__LIVING_ROOM_LAMP2
  #define DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_SOCKET_NUMBER_CTR 04
  #define DEVICENAME_ROOMHINT_CTR "Livingroom"  
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
#endif
#ifdef DEVICE_DEFAULT_SONOFF_BASIC_x
  #define DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_SOCKET_NUMBER_CTR 05
#endif
#ifdef DEVICE_DEFAULT_SONOFF_BASIC__06
  #define DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_SOCKET_NUMBER_CTR 06
  #define DEVICENAME_ROOMHINT_CTR "Roaming"  
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
#endif
#ifdef DEVICE_DEFAULT_SONOFF_BASIC_x
  #define DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_SOCKET_NUMBER_CTR 07
#endif
#ifdef DEVICE_DEFAULT_SONOFF_BASIC_x
  #define DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_SOCKET_NUMBER_CTR 08
#endif
#ifdef DEVICE_DEFAULT_SONOFF_BASIC_x
  #define DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_SOCKET_NUMBER_CTR 09
#endif
#ifdef DEVICE_DEFAULT_SONOFF_BASIC_x
  #define DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_SOCKET_NUMBER_CTR 10
#endif
#ifdef DEVICE_DEFAULT_SONOFF_BASIC__BEDRROM_DESK_5V_PSU
  #define DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_SOCKET_NUMBER_CTR 12
  #define DEVICENAME_ROOMHINT_CTR "Temporary_Bedroom"  // "|" is for multinames for rooms while "Temporary_" is devices that will leave with me 
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
#endif
#ifdef DEVICE_DEFAULT_SONOFF_BASIC__E2NSUITE_CEILING_FAN
  #define DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_SOCKET_NUMBER_CTR 13
#endif
#ifdef DEVICE_DEFAULT_SONOFF_BASIC_x
  #define DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_SOCKET_NUMBER_CTR 14
#endif
#ifdef DEVICE_DEFAULT_SONOFF_BASIC_x
  #define DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_SOCKET_NUMBER_CTR 15
#endif
#ifdef DEVICE_DEFAULT_SONOFF_BASIC__CANDLE_WARMER   // not sure what candle warmer is, I need to check with button testing
  #define DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_SOCKET_NUMBER_CTR 16
  #define DEVICENAME_ROOMHINT_CTR "Hallway"  
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
#endif
#ifdef DEVICE_DEFAULT_SONOFF_BASIC_x
  #define DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_SOCKET_NUMBER_CTR 17
#endif
#ifdef DEVICE_DEFAULT_SONOFF_BASIC_x
  #define DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_SOCKET_NUMBER_CTR 18
#endif
#ifdef DEVICE_DEFAULT_SONOFF_BASIC_x
  #define DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_SOCKET_NUMBER_CTR 19
#endif
#ifdef DEVICE_DEFAULT_SONOFF_BASIC__20
  #define DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_SOCKET_NUMBER_CTR 20
  #define DEVICENAME_ROOMHINT_CTR "Roaming"  
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
#endif
#ifdef DEVICE_DEFAULT_SONOFF_BASIC__21
  #define DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_SOCKET_NUMBER_CTR 21
  #define DEVICENAME_ROOMHINT_CTR "Roaming"  
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
#endif
#ifdef DEVICE_DEFAULT_SONOFF_BASIC__22
  #define DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_SOCKET_NUMBER_CTR 22
  #define DEVICENAME_ROOMHINT_CTR "Roaming"  
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
#endif
#ifdef DEVICE_DEFAULT_SONOFF_BASIC__23
  #define DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_SOCKET_NUMBER_CTR 23
  #define DEVICENAME_ROOMHINT_CTR "Roaming"  
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
#endif
#ifdef DEVICE_DEFAULT_SONOFF_BASIC__24
  #define DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_SOCKET_NUMBER_CTR 24
  #define DEVICENAME_ROOMHINT_CTR "Roaming"  
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
#endif

/**
 * @brief Permenant installed devices can use a static name (ie not numbered)
 **/
#ifdef DEVICE_DEFAULT_SONOFF_BASIC__KITCHEN_EXTRACTOR_FAN  
  #define SUB_DEVICE_SONOFF_BASIC__DEFAULT_CONFIG_NAMED
  #define DEVICENAME_CTR          "cooker_extractor_fan"
  #define DEVICENAME_FRIENDLY_CTR "Cooker Extractor Fan"
  #define DEVICENAME_ROOMHINT_CTR "Kitchen"  
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
#endif
#ifdef DEVICE_DEFAULT_SONOFF_BASIC__ENSUITE_EXTRACTOR_FAN  
  #define SUB_DEVICE_SONOFF_BASIC__DEFAULT_CONFIG_NAMED
  #define DEVICENAME_CTR          "ensuite_extractor_fan"
  #define DEVICENAME_FRIENDLY_CTR "Ensuite Extractor Fan"
  #define DEVICENAME_ROOMHINT_CTR "Ensuite"  
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
#endif


#ifdef DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_CTR          "socket_number_" STR2(DEVICENAME_SOCKET_NUMBER_CTR)
  #define DEVICENAME_FRIENDLY_CTR "Socket Number " STR2(DEVICENAME_SOCKET_NUMBER_CTR)
  
  #define USE_MODULE_CORE_RULES
  
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BUTTONS
  
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
    #define MAX_RELAYS 1
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SONOFF_BASIC_CTR  "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
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
  #define DEVICENAME_CTR          "socket_number_" STR2(DEVICENAME_SOCKET_NUMBER_CTR)
  #define DEVICENAME_FRIENDLY_CTR "Socket Number " STR2(DEVICENAME_SOCKET_NUMBER_CTR)
  
  /**
   * Disable serial logging
   * Use RX pin, gpio 1, as switch input
   * TX pin should still allow debugging
   * */
  // #define DISABLE_SERIAL_LOGGING
  
  #define USE_MODULE_CORE_RULES
  
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BUTTONS
    #define ENABLE_DEVFEATURE_BUTTON_HANDLER_V2
  
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
    #define MAX_RELAYS 1
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    /**
     * Temporary fix creating a new template, as adding gpio on top of existing default templates is not working
     * DONT MAKE THIS DEFAULT until I can set this gpio for button here, and hence combine this "DEVICE_...GPIO_BUTTON" with above using ifdef around GPIOC
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


/**
 * @brief "SUB_DEVICE" is a template that is the same as others, only differing by its name that is set above
 * @note Generic Sonoff Basic configuration. Serial RX pin used to also toggle relay
 */
#ifdef SUB_DEVICE_SONOFF_BASIC__DEFAULT_CONFIG_NAMED
  #ifndef DEVICENAME_CTR
  #error "DEVICENAME_CTR missing"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #error "DEVICENAME_FRIENDLY_CTR missing"
  #endif
  #ifndef DEVICENAME_ROOMHINT_CTR
  #error "DEVICENAME_ROOMHINT_CTR missing"
  #endif

  /**
   * Disable serial logging
   * Use RX pin, gpio 1, as switch input
   * TX pin should still allow debugging
   * */
  // #define DISABLE_SERIAL_LOGGING
  
  #define USE_MODULE_CORE_RULES
  
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BUTTONS
  
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
    #define MAX_RELAYS 1
    #define ENABLE_DEVFEATURE_RESET_RELAY_DECOUNTER_WHEN_TURNED_OFF
    #define ENABLE_DEVFEATURE_RELAY_RULEEVENT_USES_COMMANDSET
    
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
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SONOFF_BASIC_EXTERNAL_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
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
****** Generic devices --SHelly Dimmer as plugged in device ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

#ifdef DEVICE_DEFAULT_SHELLY_DIMMER__HALLWAY_TABLE_LAMP
  #define SUB_DEVICE_SHELLY_DIMMER__DEFAULT_CONFIG_NAMED
  #define DEVICENAME_CTR          "dimmer_hallway_lamp"
  #define DEVICENAME_FRIENDLY_CTR "Dimmer Hallway Lamp"
  #define DEVICENAME_ROOMHINT_CTR "Hallway"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
#endif
#ifdef DEVICE_DEFAULT_SHELLY_DIMMER__LIVINGROOM_LAMP1
  #define DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_SOCKET_NUMBER_CTR 04
#endif
#ifdef DEVICE_DEFAULT_SHELLY_DIMMER__BEDROOM_GLOBE // Insitu tester
  #define SUB_DEVICE_SHELLY_DIMMER__DEFAULT_CONFIG_NAMED
  #define DEVICENAME_CTR          "dimmer_bedroom_globe"
  #define DEVICENAME_FRIENDLY_CTR "Shelly Dimmer Bedroom Globe"
  #define DEVICENAME_ROOMHINT_CTR "Bedroom"
  // #define USE_HARDWARE_DEFAULT_RULES_SHELLY_DIMMER_BOTH_INPUTS_TOGGLE_OUTPUT // Going to install 2gang so I can test this going forward
#endif
#ifdef DEVICE_DEFAULT_SHELLY_DIMMER__BEDROOM_CEILING
  #define DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_SOCKET_NUMBER_CTR 04
#endif


#ifdef DEVICE_SHELLY_DIMMER_NUMBERED
  #define DEVICENAME_CTR          "shelly_dimmer_" STR2(DEVICENAME_SOCKET_NUMBER_CTR)
  #define DEVICENAME_FRIENDLY_CTR "Shelly Dimmer " STR2(DEVICENAME_SOCKET_NUMBER_CTR)
  
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

/**
 * @brief "SUB_DEVICE" is a template that is the same as others, only differing by its name that is set above
 * @note Generic Shelly Dimmer Version2 configuration. Either switch input with toggle
 */
#ifdef SUB_DEVICE_SHELLY_DIMMER__DEFAULT_CONFIG_NAMED
  #ifndef DEVICENAME_CTR
  #error "DEVICENAME_CTR missing"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #error "DEVICENAME_FRIENDLY_CTR missing"
  #endif
  #ifndef DEVICENAME_ROOMHINT_CTR
  #error "DEVICENAME_ROOMHINT_CTR missing"
  #endif
    
  #define DISABLE_SERIAL_LOGGING //temp measure

  #define USE_MODULE_CORE_RULES
  
  #define USE_MODULE_DRIVERS_SHELLY_DIMMER    
    #define ENABLE_DEVFEATURE_SHELLY_DIMMER_FORCED_BRIGHTNESS_TO_REQUESTED

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY_DIMMER2_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
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
  #define DEVICENAME_ROOMHINT_CTR "Kitchen"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
  
  #define USE_MODULE_CORE_RULES

  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_ENERGY_INTERFACE
  #define USE_MODULE_ENERGY_ADE7953

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
    #define MAX_RELAYS 2
    
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
      #ifdef USE_MODULE_ENERGY_ADE7953
      "\"16\":\""  D_GPIO_FUNCTION_ADE7953_IRQ_CTR  "\","
      "\"14\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"12\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif 
      "\"0\":\"" D_GPIO_FUNCTION_LED1_CTR "\""
      // Button1 GPIO2
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
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

/**
 * 
 * Button is on shelly PCB, mains, not to be used in normal operation (perhaps reset later?)
 * */
#ifdef DEVICE_KITCHENLIGHT2
  #define DEVICENAME_CTR          "kitchenlight2"
  #define DEVICENAME_FRIENDLY_CTR "Kitchen Light 2 [Table/Window]"
  #define DEVICENAME_ROOMHINT_CTR "Kitchen"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
    
  /*
    Method should only activate if boot loop happens 10 times
    Method A: Switch to Wifi.begin hardcoded ssid/pw, start OTA and wait, rebooting every 10 minutes if wifi does not connect
    Method B: Begin wifi.ap as host, so I can directly connect to it via x.x.x.x
  */
  #define DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS
  //#define DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS

  #define USE_MODULE_NETWORKS_MQTT
  #define USE_MODULE_CORE_RULES

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_BUTTONS

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
    #define ENABLE_DEVFEATURE_DISABLE_PHASEDOUT_RELAY_ONOFF
    #define MAX_RELAYS 2
  
  #define USE_MODULE_ENERGY_INTERFACE
  #define USE_MODULE_ENERGY_ADE7953
      
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY2P5_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG  "Window"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG  "Dining Room"
  #define D_DEVICE_SWITCH_0_FRIENDLY_NAME_LONG "Dining Room"
  #define D_DEVICE_SWITCH_1_FRIENDLY_NAME_LONG "Window"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
      "],"
        "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_SWITCH_0_FRIENDLY_NAME_LONG "\","
          "\"" D_DEVICE_SWITCH_1_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";

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


/**
 * @brief Virtual Switch, installed in kitchen 4gang (left most switch)
 * Shelly 1, switch input only, relay not enabled
 * Sends command to cupboard lights directly with mqtt (downlights)
 * cupboard_spot vs cupboard_under
 */
#ifdef DEVICE_KITCHENLIGHT3
  #define DEVICENAME_CTR          "kitchenlight3"
  #define DEVICENAME_FRIENDLY_CTR "Kitchen 3 VirtualSwt"
  #define DEVICENAME_ROOMHINT_CTR "Kitchen"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
  
  //#define FORCE_TEMPLATE_LOADING
  #define USE_MODULE_CORE_RULES

  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
    #define MAX_RELAYS 1
    
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
      "\"4\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","  // Just so it gets turned off as a known state
      #endif 
      "\"0\":\"" D_GPIO_FUNCTION_LED1_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Virtual"

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
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\"," //mqtt function
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\"," //mqtt_send
        "\"DeviceName\":0,"
        "\"State\":0,"
        // Even though I will use sonoff basics for cupboard lights (or else buy shelly 2.5), the command will be set as kitchenlight#
        // "\"JsonCommands\":\"{\\\"MQTTSend\\\":{\\\"Topic\\\":\\\"kitchen_cupboard/set\\\",\\\"Payload\\\":\\\"{~PowerName~:~Downlight~,~PowerState~:2}\\\"}}\""
        "\"JsonCommands\":\"{\\\"MQTTSend\\\":{\\\"Topic\\\":\\\"kitchenlight4/set/fromvirtual\\\",\\\"Payload\\\":\\\"{~PowerName~:0,~PowerState~:2}\\\"}}\""
      "}"
    "}"
  "}";

#endif


#ifdef DEVICE_KITCHENLIGHT4
  #define DEVICENAME_CTR          "kitchenlight4"
  #define DEVICENAME_FRIENDLY_CTR "Kitchen Light Cupboard [Under|Downlight]"
  #define DEVICENAME_ROOMHINT_CTR "Kitchen"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
    
  /*
    Method should only activate if boot loop happens 10 times
    Method A: Switch to Wifi.begin hardcoded ssid/pw, start OTA and wait, rebooting every 10 minutes if wifi does not connect
    Method B: Begin wifi.ap as host, so I can directly connect to it via x.x.x.x
  */
  #define DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS
  //#define DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS

  #define USE_MODULE_NETWORKS_MQTT
  #define USE_MODULE_CORE_RULES

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_BUTTONS

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
    #define ENABLE_DEVFEATURE_DISABLE_PHASEDOUT_RELAY_ONOFF
    #define MAX_RELAYS 2
  
  #define USE_MODULE_ENERGY_INTERFACE
  #define USE_MODULE_ENERGY_ADE7953
      
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY2P5_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG  "Sink"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG  "Cupboard"
  #define D_DEVICE_SWITCH_0_FRIENDLY_NAME_LONG "Switch1"
  #define D_DEVICE_SWITCH_1_FRIENDLY_NAME_LONG "Switch2"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
      "],"
        "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_SWITCH_0_FRIENDLY_NAME_LONG "\","
          "\"" D_DEVICE_SWITCH_1_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";

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

//cct_temp, 100% should be highest colour temp (more white?)


/**
 * This will become the final version of a panel, but requires animation fixing first
 * */
#ifdef DEVICE_NEXTION_HEATING_KITCHEN_DISPLAY
  #define DEVICENAME_CTR            "kitchenpanel"
  #define DEVICENAME_FRIENDLY_CTR   "Heating Kitchen Display"
  #define DEVICENAME_ROOMHINT_CTR "Kitchen"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  // #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_NEXTION
  #define NEXTION_DEFAULT_PAGE_NUMBER 1

  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

#endif





#ifdef DEVICE_RGBSHELF
  #define DEVICENAME_CTR          "rgbshelf"
  #define DEVICENAME_FRIENDLY_CTR "Shelf Lights"
  #define DEVICENAME_ROOMHINT_CTR "Kitchen"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
  
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    #define STRIP_SIZE_MAX 50
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT
    
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_JSON_EFFECTS "\"," 
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

#endif

#ifdef DEVICE_RGBLIGHTS_GLASSBOX
  #define DEVICENAME_CTR          "rgbglassbox"//"rgbcrystal3"
  #define DEVICENAME_FRIENDLY_CTR "Glass Box Lights"
  #define DEVICENAME_ROOMHINT_CTR "Kitchen"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
  
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    #define STRIP_SIZE_MAX 50
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT

    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define STRIP_SIZE_MAX 50
  #define USE_LIGHTING_TEMPLATE
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
      "\"" D_JSON_RATE_MS "\":2000"
    "},"
    "\"ColourPalette\":\"Christmas MultiColoured Warmer\","
    "\"BrightnessRGB\":100"
  "}";

#endif


#ifdef DEVICE_RGBCOOKER
  #define DEVICENAME_CTR          "rgbcooker"
  #define DEVICENAME_FRIENDLY_CTR "RGB Cooker H801"
  #define DEVICENAME_ROOMHINT_CTR "Kitchen"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
      
  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  #define DISABLE_WEBSERVER
    
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_PWM  
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    #define ENABLE_DEVFEATURE_SOLAR_PALETTES
    #define ENABLE_DEVFEATURE_CHECK_SEGMENT_INIT_ERROR
    #define DEBUG_TARGET_ANIMATOR_SEGMENTS
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT  

    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS
  
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
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_HARDWARE_TYPE  "\":\"" "RGBCCT_PWM" "\","
  //   "\"" D_JSON_STRIP_SIZE       "\":1,"
  //   "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"RGBCW\","
  //   "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":2,\"" D_JSON_RATE "\":20},"
  //   "\"" D_JSON_COLOUR_PALETTE "\":\"Solid Rgbcct 00\","
  //   "\"" D_JSON_ANIMATIONMODE  "\":\"Effects\","
  //   "\"" D_JSON_EFFECTS        "\"{\"Function\":\"Solid RGBCCT\"},"
  //   "\"" D_JSON_BRIGHTNESS     "\":100"
  // "}";

  
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "RGBCCT_PWM" "\","
    "\"" D_JSON_STRIP_SIZE       "\":1,"
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGBCW\","
    "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":2,\"" D_JSON_RATE "\":20},"
    "\"" D_JSON_COLOUR_PALETTE   "\":\"Solid Rgbcct 00\","
    "\"Hue\":345,\"Sat\":100,"
    "\"" D_JSON_ANIMATIONMODE    "\":\"Effects\","
    "\"" D_JSON_EFFECTS          "\":{\"Function\":\"Solid RGBCCT\"},"
    "\"" D_JSON_BRIGHTNESS_RGB   "\":100,"
    "\"" D_JSON_BRIGHTNESS_CCT   "\":100"
  "}";



#endif


/**
 * @brief 
 * Cat5e Cable to Generic Room Sensor with BH1750, BME280 and HC-SR501 PIR
 * 
 * Ethernet      Function       ESP32         6P/2P Connector        
 * 
 * w/o           GND            GND           Black
 * o/w           5V             5V            Red
 * w/g           PIR            5             Yellow
 * bl/w          I2D            18            Green      //check i2c 
 * w/bl          I2C            19            Orange
 * g/w           3V3            3V3           White
 * w/br           -                           Black     // Button? Led?
 * br/w           -                           Red
 **/
#ifdef DEVICE_KITCHENSENSOR
  #define DEVICENAME_CTR          "kitchensensor"
  #define DEVICENAME_FRIENDLY_CTR "Kitchen Sensor"
  #define DEVICENAME_ROOMHINT_CTR "Kitchen"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
  
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FASTBOOT_HTTP_FALLBACK_DEFAULT_SSID
  // #define ENABLE_DEVFEATURE_RTC_SETTINGS


  #define USE_MODULE_CORE_RULES
       
  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION
  #define USE_MODULE_SENSORS_BH1750

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"      
      #ifdef USE_MODULE_SENSORS_BME
      "\"26\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"25\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\"," //should be 27, missoldered, repair later
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"5\":\"" D_GPIO_FUNCTION_SWT1_CTR   "\""
      #endif
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
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
      "],"
      "\"" D_MODULE_SENSORS_BH1750_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "]"
    "},"    
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1}"
  "}";
  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"// for PIR to follow
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
  #define DEVICENAME_ROOMHINT_CTR "Kitchen"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
      
  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  #define DISABLE_WEBSERVER
    
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_PWM  
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    #define ENABLE_DEVFEATURE_SOLAR_PALETTES
    #define ENABLE_DEVFEATURE_CHECK_SEGMENT_INIT_ERROR
    #define DEBUG_TARGET_ANIMATOR_SEGMENTS
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT  

    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS
  
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
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "RGBCCT_PWM" "\","
    "\"" D_JSON_STRIP_SIZE       "\":1,"
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGBCW\","
    "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":2,\"" D_JSON_RATE "\":20},"
    "\"" D_JSON_COLOUR_PALETTE   "\":\"Solid Rgbcct 00\","
    "\"Hue\":345,\"Sat\":100,"
    "\"" D_JSON_ANIMATIONMODE    "\":\"Effects\","
    "\"" D_JSON_EFFECTS          "\":{\"Function\":\"Solid RGBCCT\"},"
    "\"" D_JSON_BRIGHTNESS_RGB   "\":100,"
    "\"" D_JSON_BRIGHTNESS_CCT   "\":100"
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
  #define DEVICENAME_ROOMHINT_CTR "Utility"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
      
  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  #define DISABLE_WEBSERVER
    
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_PWM  
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    #define ENABLE_DEVFEATURE_SOLAR_PALETTES
    #define ENABLE_DEVFEATURE_CHECK_SEGMENT_INIT_ERROR
    #define DEBUG_TARGET_ANIMATOR_SEGMENTS
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT  

    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS
  
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
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "RGBCCT_PWM" "\","
    "\"" D_JSON_STRIP_SIZE       "\":1,"
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGBCW\","
    "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":2,\"" D_JSON_RATE "\":20},"
    "\"" D_JSON_COLOUR_PALETTE   "\":\"Solid Rgbcct 00\","
    "\"Hue\":345,\"Sat\":100,"
    "\"" D_JSON_ANIMATIONMODE    "\":\"Effects\","
    "\"" D_JSON_EFFECTS          "\":{\"Function\":\"Solid RGBCCT\"},"
    "\"" D_JSON_BRIGHTNESS_RGB   "\":100,"
    "\"" D_JSON_BRIGHTNESS_CCT   "\":100"
  "}";

#endif

/**
 * @brief 
 * Cat5e Cable to Generic Room Sensor with BH1750, BME280 and HC-SR501 PIR
 * 
 * Ethernet      Function       ESP32         6P/2P Connector        
 * 
 * w/o           GND            GND           Black
 * o/w           5V             5V            Red
 * w/g           PIR            5             Yellow
 * bl/w          I2D            18            Green      //check i2c 
 * w/bl          I2C            19            Orange
 * g/w           3V3            3V3           White
 * w/br           -                           Black     // Button? Led?
 * br/w           -                           Red
 * 
 * gnd
 * 3v3 removed
 * door_input
 * button_input
 * led_for_door_disabled to high, removes need to 3v3
 * 
 * GND
 * DOOR_INPUT
 * BUTTON_DISABLE
 * STATUS_LED (this could just be from the main board and lighting the ceiling?)
 * 
 **/
#ifdef DEVICE_UTILITYSENSOR
  #define DEVICENAME_CTR          "utilitysensor"
  #define DEVICENAME_FRIENDLY_CTR "Utility Sensor"
  #define DEVICENAME_ROOMHINT_CTR "Utility"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  // #define USE_MODULE_DRIVERS_LEDS

  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS_BME
    #define ENABLE_DEVFEATURE_BME_DUAL_DEVICES
  #define USE_MODULE_SENSORS_BH1750
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION
  #define USE_MODULE_SENSORS_DOOR
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"      
      #ifdef USE_MODULE_SENSORS_BME
      "\"26\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"27\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\""
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"5\":\""  D_GPIO_FUNCTION_SWT1_CTR "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DOOR
      "\"18\":\"" D_GPIO_FUNCTION_DOOR_OPEN_CTR     "\","
      #endif
      "\"19\":\"" D_GPIO_FUNCTION_LED2_INV_CTR "\","
      "\"2\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "Utility"
  #define D_DEVICE_SENSOR_CLIMATE "Utility"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE "\""
      "],"  
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["         // so this is probably to be phased out?
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
      "],"  
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" "Utility" "\""
      "],"  
      "\"" D_MODULE_SENSORS_DOOR_FRIENDLY_CTR "\":["
        "\"" "Sidedoor" "\""
      "],"
      "\"" D_MODULE_SENSORS_BH1750_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE "\""
      "]"
    "}"
  "}";

  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{" // MOTION
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

/**
 * @brief Using ESP32 due to length of pixels in strip. Also useful as long term esp32 neopixel testbed
 **/
#ifdef DEVICE_RGBLAMP_CYLINDER
  #define DEVICENAME_CTR            "rgblamp_cylinder"
  #define DEVICENAME_FRIENDLY_CTR   "Crystal Light Cylinder Utility"
  #define DEVICENAME_ROOMHINT_CTR   "Utility"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
  
  #define DISABLE_WEBSERVER

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS // Not ready to remove
    #define STRIP_SIZE_MAX 210
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins

    
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      #endif 
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #ifdef USE_MODULE_LIGHTS_INTERFACE
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"AnimationMode\": \"Effects\","
    "\"ColourOrder\": \"grb\","
    "\"ColourPalette\":\"Pastel\","
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
  #endif // USE_MODULE_LIGHTS_INTERFACE
  
#endif


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Downstairs Toilet ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


#ifdef DEVICE_CONSUMERUNIT
  #define DEVICENAME_CTR          "consumerunit"
  #define DEVICENAME_FRIENDLY_CTR "Consumer Unit"
  #define DEVICENAME_ROOMHINT_CTR "Downstairs Toilet"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
    
  #define DEVICENAMEBUFFER_NAME_BUFFER_LENGTH 800
  #define SETTINGS_SENSORS_MQTT_IFCHANGED_PERIOD_SECONDS 1 // temporary force the value, should be handled by command

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION

  #define USE_MODULE_ENERGY_INTERFACE
  #define USE_MODULE_ENERGY_PZEM004T_V3
    #define ENABLE_DEVFEATURE_REDUCE_SUBORDINATE_MQTT_REPORTING_ENERGY // If energy_interface is primary reporting, reduce pzem to slower (debug only)

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    #define STRIP_SIZE_MAX 55
    #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
    #define USE_SK6812_METHOD_DEFAULT
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT
    
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS
  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "SK6812" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"Effects\"," 
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
      "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\"" 
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
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
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
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
    "},"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1},"
    "\"MQTT_Interface_Priority\":{\"" D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR "\":1}" // Each interface will have ability to reduce its subclass mqtt "ifchanged" rate
  "}";

  /**
   * In the future, make a way to push this exact rule via single command (append new rule, start using vectors for indexing?)
   * */
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{" // for PIR to follow
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
****** Attic ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**
 * 
 * 
 * 
 * 
 * */
#ifdef DEVICE_ATTIC_SENSOR
  #define DEVICENAME_CTR          "atticsensor"
  #define DEVICENAME_FRIENDLY_CTR "Attic Sensor"
  #define DEVICENAME_ROOMHINT_CTR "Attic"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  #define USE_MODULE_CORE_RULES
     
  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_DS18X
  
  #define USE_MODULE_CONTROLLER__LOUVOLITE_HUB_V2 //both rooms

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750)
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR    "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DS18X
      "\"D6\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      #endif
      #if defined(USE_MODULE_CONTROLLER__LOUVOLITE_HUB) || defined(USE_MODULE_CONTROLLER__LOUVOLITE_HUB_V2)
      "\"D7\":\"" D_GPIO_FUNCTION__RF_433MHZ_TX__CTR   "\","
      #endif  
      "\"D4\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Attic"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Attic"
  #define D_DEVICE_TEMP_0_FRIENDLY_NAME_LONG "Cold Water Tank"
  #define D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "Attic Top"
  #define D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "Attic Bottom"
  
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
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_TEMP_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["    
        "[40,50,199,3,0,0,0,102],"
        "[40,255,100,30,7,142,92,154],"
        "[40,255,100,30,7,143,73,252]"
      "]"  
    "}"
  "}";

#endif


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Livingroom ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

#ifdef DEVICE_LIVINGROOMSENSOR
  #define DEVICENAME_CTR          "livingroomsensor"
  #define DEVICENAME_FRIENDLY_CTR "Living Room Sensor"
  #define DEVICENAME_ROOMHINT_CTR "Livingroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  #define USE_MODULE_CORE_RULES
     
  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_BH1750
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750)
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR    "\","
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D6\":\"" D_GPIO_FUNCTION_SWT1_CTR   "\","
      #endif
      "\"D4\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "LivingRoom"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "LivingRoom"
  
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
      "],"
      "\"" D_MODULE_SENSORS_BH1750_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{" // for PIR to follow
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


#ifdef DEVICE_RGBFIREPLACE
  #define DEVICENAME_CTR            "rgbfireplace"
  #define DEVICENAME_FRIENDLY_CTR   "RGB Fire Place"
  #define DEVICENAME_ROOMHINT_CTR   "Livingroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
  
  #define DISABLE_WEBSERVER

  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS // Not ready to remove
    #define STRIP_SIZE_MAX 300
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins

    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      #endif 
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #ifdef USE_MODULE_LIGHTS_INTERFACE
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"AnimationMode\": \"Effects\","
    "\"ColourOrder\": \"grb\","
    "\"ColourPalette\":\"Single Flame 01\","
    "\"Effects\":{"
      "\"Function\":\"FirePlace01\","
      "\"Intensity\":255,"
      "\"Speed\":10"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":23"
    "},"    
    "\"BrightnessRGB\":50,"
    "\"TimeOn\":3600"
  "}";
  #endif // USE_MODULE_LIGHTS_INTERFACE
  
#endif





/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Hallway ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

/* 
  * Single Relay controls TWO 12V DC fans to improve heating (1 D_Out)
  * Measure PWM speed of motors (2 D_in) (Needs level shifter from 12v to 3.3v)
  * Motion sensor (1 D_in)
  * THREE DS18b20 to measure inside/outside of vent area (1 D_io)
  *  - (3) on radiator pipe, to be used as fan trigger when set to auto mode
  * One DHT22 for climate sensing -- towards door (1 D_io)
  * One BME280 for improved climate sensing -- towards hallway (1 I2C) 
 * 
 * >>>>>>>>>>>>>>>>>>> 3P (1/3) - PIR
 * 5V
 * PIR_Input                  23
 * GND
 * 
 * >>>>>>>>>>>>>>>>>>> 3P (2/3) - DB18XX already on RJ45, alternate connection
 * 3V3
 * Signal_db18xx              5
 * GND
 * 
 * >>>>>>>>>>>>>>>>>>> RJ45
 * w/o   GND
 * o/w   5V
 * w/g   DB18X                5  
 * bl/w  I2D                  27
 * w/bl  I2C                  26
 * g/w   3V3
 * w/br  DHT22                18
 * br/w  N/C                  N/C
 * 
 * >>>>>>>>>>>>>>>>>>> 3P (3/3)
 * Relay_Fan_12V              4
 * PWM_Speed_Input            N/C
 * GND
 * 
 * 
 * >>>>>>>>>>>>>>>>>>> 3P (3/3)
 * 12V    (left side)
 * GND    (right side)
 * 
 * 
 * Build wooden frame to hold insualtion and also add fans onto the wall! (so radiator is fully ducted)
 * The db sensors from the fans would now be on the wall!
 * Get wood from garage attic, use it for frame.
 * 
 */
#ifdef DEVICE_RADIATORFAN                                                        
  #define DEVICENAME_CTR          "radiatorfan"
  #define DEVICENAME_FRIENDLY_CTR "Radiator Fan"
  #define DEVICENAME_ROOMHINT_CTR "Hallway"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS_BME
    #define ENABLE_DEVFEATURE_BME_DUAL_DEVICES
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION
  #define USE_MODULE_SENSORS_DS18X
  #define USE_MODULE_SENSORS_DHT

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
    #define MAX_RELAYS 1

  #define USE_MODULE_CONTROLLER_RADIATORFAN

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"   
      // 3P - Top Right
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"23\":\""  D_GPIO_FUNCTION_SWT1_CTR     "\","
      #endif
      // RJ45 Connector   
      #ifdef USE_MODULE_SENSORS_BME
      "\"26\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"27\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DHT
      "\"18\":\"" D_GPIO_FUNCTION_DHT22_1_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DS18X
      "\"5\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR  "\","
      #endif   
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"4\":\"" D_GPIO_FUNCTION_REL1_INV_CTR   "\","
      #endif
      "\"2\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_DRIVER_RELAY_NAME_LONG               "Fan"
  #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG    "Hallway"
  #define D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG            "OutsideVent"
  #define D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG            "InsideVent"
  #define D_DEVICE_TEMP_RADIATOR_FRIENDLY_NAME_LONG     "RadiatorPipe"
  #define D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG            "Hallway"     // BME Primary
  #define D_DEVICE_TEMP_4_FRIENDLY_NAME_LONG            "Hallway-DHT" // Secondary
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
      "],"  
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_DRIVER_RELAY_NAME_LONG "\""
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
  "{"     // MOTION
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

/**
 * @brief 
 * Cat5e Cable to Generic Room Sensor with BH1750, BME280 and HC-SR501 PIR
 * "*" pin = confirmed soldering
 * Ethernet      Function       ESP32         Note        
 * 
 * ** Blue (Upstairs Link) **************************************************************************
 * w/o           GND            GND           
 * o/w           5V             5V            
 * w/g           R_IH           21*           Immersion relay (5v IO)
 * bl/w          I2D            12*           DHT22?     
 * w/bl          I2C            13*           UNUSED   
 * g/w           3V3            3V3           
 * w/br                      
 * br/w          DSX            14*           ds18b20 water, 4k7 pulled high        (comes from blue by connector)   
 * ** Green (Downstairs Sensors) **************************************************************************
 * w/o           GND            GND           Black
 * o/w           5V             5V            Red
 * w/g           1Wire          4*            DS18B20 water pipe sensors
 * bl/w          I2D            22*           BME in Dinning Room (Alternatively, DHT22 data pin)
 * w/bl          I2C            23*           BME in Dinning Room
 * g/w           3V3            3V3           White
 * w/br                                       NC
 * br/w          DHT22          25*           DHT22 (until BME is working)
 * ** Red (Relay Board) **************************************************************************
 * w/o           GND            GND           
 * o/w           5V             5V            
 * w/g                          5*             
 * bl/w          R_DS           18*                   
 * w/bl                         19*            
 * g/w           R_WB           3V3           
 * w/br           -                           
 * br/w          R_US                         
 * ** Orange (Nextion Display) **************************************************************************
 * w/o           GND            GND           
 * o/w           5V             5V            
 * w/g           NEO            27*             SK6812 4 pixels of rgbW
 * bl/w                                      
 * w/bl                                      
 * g/w           3V3            3V3           
 * w/br          RX2            17*              Nextion TX
 * br/w          TX2            16*              Nextion RX   -- "SERIAL TX" of Serial will always be dominant colour (brown) as its important to know the output pin
 * Twin          Switch
 * Twin          Switch
 * ** Power Screw Jacks **************************************************************************
 * 4 (Top)       12V
 * 3             5V
 * 2
 * 1 (Bottom)    GND
 * ** ADC Input **************************************************************************
 * 4 (Top)       LDR_US         33
 *               LDR_DS         32 
 *               LDR_WB         35
 * Extra Ethernet for LDRs hot glued onto the red led of the servos?
 * 
 * Upstairs Connectors
 * 3pin (DHT22)    - gnd,5v,dht_data 
 * 3pin (relay US) - gnd,5v,relay_ih
 * 3pin (water sensors) - gnd,3v3,ds18b20
 * 
 **/
#ifdef DEVICE_HEATING
  #define DEVICENAME_CTR          "heating"
  #define DEVICENAME_FRIENDLY_CTR "HVAC House Heating"
  #define DEVICENAME_ROOMHINT_CTR "Hallway"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
  
  #define ENABLE_FEATURE_WATCHDOG_TIMER

  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define DISABLE_SLEEP // loops per second less than 1hz // I need to make an "mqtt/alert" channel that lets me know this
  
  /**
   * @brief Increasing buffers for the larger controller than default minimums
   **/
  #define DEVICENAMEBUFFER_NAME_BUFFER_LENGTH 1000
  #define DEVICENAMEBUFFER_NAME_INDEX_LENGTH  100
  #define DB18_SENSOR_MAX                     15
  #define DATA_BUFFER_PAYLOAD_MAX_LENGTH      3000 //needed for db sensosrs, but currently causes crash in lighting
  #define MQTT_MAX_PACKET_SIZE                3000

  #define USE_MODULE_CONTROLLER_HVAC
    // #define USE_MODULE_CONTROLLER_HEATING_STRIP_COLOUR_UNDERSTAIRS
    #define ENABLE_DEVFEATURE_GET_SENSOR_READINGS_FOR_HVAC_ZONES

  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS_DS18X
  #define USE_MODULE_SENSORS_BME //removing for now, will place short wire one understairs and use for long term debugging
  #define USE_MODULE_SENSORS_DHT

  //add 3 LDRs onto the motor neons, so I can check if they are turned on ((hot glue them on)) 30+ pins == use this to know what has been turned on manually.
  //Also need to add a mains detector for the furnace trigger (orange wire from servos)
  
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define USE_MODULE_LIGHTS_ADDRESSABLE
  // #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS // Not ready to remove
  // #define STRIP_SIZE_MAX 4
  // #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  // #define USE_SK6812_METHOD_DEFAULT
  // #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
  // #define USE_DEVFEATURE_FIX_TO_PIXEL_LENGTH
  
  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define USE_MODULE_LIGHTS_ADDRESSABLE
  //   #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  //   #define STRIP_SIZE_MAX 5
  //   #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  //   #define USE_SK6812_METHOD_DEFAULT
  //   #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT
  
  //   #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
  //   #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
  //   #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
  //   #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
  //   #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS


  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
  
  // #define USE_MODULE_DISPLAYS_NEXTION
  // #define ENABLE_DEVFEATURE_NEXTION_DISPLAY

  // Actual
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
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"5\":\""  GPIO_NAME_ZONE0_UPSTAIRS_RELAY    "\","
      "\"19\":\"" GPIO_NAME_ZONE1_DOWNSTAIRS_RELAY  "\","
      "\"21\":\"" GPIO_NAME_ZONE3_IMMERISON_RELAY   "\","
      "\"18\":\"" GPIO_NAME_ZONE4_BOILER_RELAY      "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DHT
      "\"25\":\"" D_GPIO_FUNCTION_DHT22_2_CTR   "\"," // DS_DHT 
      // "\"12\":\"" D_GPIO_FUNCTION_DHT22_1_CTR   "\"," // US_DHT 
      #endif
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750)
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"23\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"27\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      #endif 
      #ifdef USE_MODULE_DISPLAYS_NEXTION
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DS18X
      "\"14\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\"," // US_DB - 3 pin
      "\"4\":\""  D_GPIO_FUNCTION_DS18X20_2_CTR "\"," // DS_DB - 3 pin
      #endif    
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""   // builtin led
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  /**
   * @brief Drivers and Sensors for HVAC zones
   **/
  #define D_DEVICE_DRIVER_RELAY_0_NAME "Upstairs"
  #define D_DEVICE_DRIVER_RELAY_1_NAME "Downstairs"
  #define D_DEVICE_DRIVER_RELAY_2_NAME "Immersion"
  #define D_DEVICE_DRIVER_RELAY_3_NAME "Boiler"

  #define D_DEVICE_SENSOR_DHT_0_NAME "Upstairs_DHT"
  #define D_DEVICE_SENSOR_DHT_1_NAME "Downstairs_DHT"

  /**
   * @brief HVAC zones
   **/
  #define D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "Upstairs"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "Downstairs"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "Immersion"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE3_NAME "Boiler"

  /**
   * @brief Pin_US 
   */
  #define D_DEVICE_SENSOR_DB18S20_00_NAME        "Immersion_Heater"
  #define D_DEVICE_SENSOR_DB18S20_00_ADDRESS     "[40,255,136,105,53,22,4,114]"

  #define D_DEVICE_SENSOR_DB18S20_01_NAME        "Tank_Top"
  #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,255,50,176,193,23,4,197]"

  #define D_DEVICE_SENSOR_DB18S20_02_NAME        "Tank_Middle"
  #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,255,216,108,53,22,4,102]"

  #define D_DEVICE_SENSOR_DB18S20_03_NAME        "Tank_Bottom"
  #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,255,162,167,53,22,4,27]"

  #define D_DEVICE_SENSOR_DB18S20_04_NAME        "Tank_Out"
  #define D_DEVICE_SENSOR_DB18S20_04_ADDRESS     "[40,255,219,93,53,22,4,239]"

  /** 
   * Pin_DS
   * */
  #define D_DEVICE_SENSOR_DB18S20_21_NAME        "Water21-Upstairs"
  #define D_DEVICE_SENSOR_DB18S20_21_ADDRESS     "[40,208,174,149,240,1,60,127]"

  #define D_DEVICE_SENSOR_DB18S20_22_NAME        "Water22-HotCross"
  #define D_DEVICE_SENSOR_DB18S20_22_ADDRESS     "[40,168,253,149,240,1,60,157]"

  #define D_DEVICE_SENSOR_DB18S20_23_NAME        "Water23-R/C"
  #define D_DEVICE_SENSOR_DB18S20_23_ADDRESS     "[40,12,164,2,0,0,0,72]"

  #define D_DEVICE_SENSOR_DB18S20_24_NAME        "Water24-Mains"
  #define D_DEVICE_SENSOR_DB18S20_24_ADDRESS     "[40,9,77,4,0,0,0,131]"

  #define D_DEVICE_SENSOR_DB18S20_25_NAME        "Water25-HotFromBoiler"
  #define D_DEVICE_SENSOR_DB18S20_25_ADDRESS     "[40,121,172,3,0,0,0,138]"

  #define D_DEVICE_SENSOR_DB18S20_26_NAME        "Water26-Downstairs"
  #define D_DEVICE_SENSOR_DB18S20_26_ADDRESS     "[40,205,241,149,240,1,60,148]"

  #define D_DEVICE_SENSOR_DB18S20_27_NAME        "Water27-R/H"
  #define D_DEVICE_SENSOR_DB18S20_27_ADDRESS     "[40,195,112,2,0,0,0,178]"

  #define D_DEVICE_SENSOR_DB18S20_28_NAME        "Water28-HotFromFurnace"
  #define D_DEVICE_SENSOR_DB18S20_28_ADDRESS     "[40,103,49,3,0,0,0,153]"

  #define D_DEVICE_SENSOR_DB18S20_29_NAME        "Water29-WaterBoiler"
  #define D_DEVICE_SENSOR_DB18S20_29_ADDRESS     "[40,183,162,149,240,1,60,24]"


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
        // Upstairs
        "\"" D_DEVICE_SENSOR_DB18S20_00_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\","
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_21_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_22_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_23_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_24_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_25_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_26_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_27_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_28_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_29_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DHT_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_1_NAME "\","
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
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
        // Upstairs
        D_DEVICE_SENSOR_DB18S20_00_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_03_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_04_ADDRESS ","
        // Downstairs
        D_DEVICE_SENSOR_DB18S20_21_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_22_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_23_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_24_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_25_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_26_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_27_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_28_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_29_ADDRESS ""
      "]"  
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        // "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
        // "\"" D_DEVICE_SENSOR_DHT_1_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\"," //debug fix
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\"," //debig fix
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_0_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_2_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_3_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "}"
      "]"
    "},"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1}"
  "}";

  #ifdef USE_MODULE_LIGHTS_INTERFACE
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "SK6812" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"AnimationMode\": \"Effects\","
    "\"ColourOrder\": \"grbw\","
    "\"ColourPalette\":\"Christmas 01\","
    "\"Effects\": {"
      "\"Function\": \"Static\""
    "},"
    "\"CCT_TempPercentage\":100,"
    "\"BrightnessRGB\":10,"
    "\"BrightnessCCT\":10,"
    "\"Transition\": {"
      "\"Time\":0,"
      "\"RateMs\":1000"
    "}"
  "}";
  #endif // USE_MODULE_LIGHTS_INTERFACE

  
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
 * Just to be quick, I will make a controller just for this use and will later replace with rules
 * 
 * If Input1 (ie Utility Wall Switch) is toggled, then turn the light on (toggle ON = 2 hours, toggle OFF resets timer)
 * If Input2 (ie motion) then turn light on for 5 minutes, then off
 * 
 * Have options for paired lights
 *  - if no motion, have the "pretty light" on 
 *  - if motion, turn both on 
 * 
 * */
#ifdef DEVICE_SIDEDOORLIGHT
  #define DEVICENAME_CTR          "sidedoorlight"
  #define DEVICENAME_FRIENDLY_CTR "Side Door Motion Light"
  #define DEVICENAME_ROOMHINT_CTR   "Outdoor|Driveway"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70



  /*
    Method should only activate if boot loop happens 10 times
    Method A: Switch to Wifi.begin hardcoded ssid/pw, start OTA and wait, rebooting every 10 minutes if wifi does not connect
    Method B: Begin wifi.ap as host, so I can directly connect to it via x.x.x.x
  */
  //#define DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS
  // #define DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS

  #define USE_MODULE_NETWORKS_MQTT

  #define USE_MODULE_CORE_RULES

  #define ENABLE_DEVFEATURE_RELAY_ENABLE_TIME_WINDOW_LOCKS

  // #define USE_MODULE_ENERGY_INTERFACE
  // #define USE_MODULE_ENERGY_ADE7953

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE_SENSORS_MOTION

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
    #define MAX_RELAYS 2

  #define ENABLE_DEVFEATURE_OTA_FALLBACK_ON_BOOT_LOOPING

  #define USE_MODULE_CONTROLLER_CUSTOM__SIDEDOOR_LIGHTS  

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY2P5_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
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
  // #define USE_RULES_TEMPLATE
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
  // #define USE_RULES_TEMPLATE
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



  // //#define FORCE_TEMPLATE_LOADING
  // #define SETTINGS_HOLDER 2
   
  // /*
  //   Method should only activate if boot loop happens 10 times
  //   Method A: Switch to Wifi.begin hardcoded ssid/pw, start OTA and wait, rebooting every 10 minutes if wifi does not connect
  //   Method B: Begin wifi.ap as host, so I can directly connect to it via x.x.x.x
  // */
  // //#define DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS
  // #define DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS

  // #define ENABLE_DEVFEATURE_RELAY_ENABLE_TIME_WINDOW_LOCKS

  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_SWITCHES
  // #define USE_MODULE_SENSORS_MOTION

  // #define USE_MODULE_NETWORKS_MQTT

  // #define USE_MODULE_CORE_RULES

  // #define USE_MODULE_DRIVERS_INTERFACE
  // #define USE_MODULE_DRIVERS_RELAY
  // #define MAX_RELAYS 1
  
  // #define USE_MODULE_DRIVERS_RELAY
    
  // #define USE_MODULE_TEMPLATE
  // DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
  //   "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY1_CTR "\","
  //   "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  // "}";
    
  // #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Side Door"
  // #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Driveway Middle" 
  
  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  // "{"
  //   "\"" D_JSON_DEVICENAME "\":{"
  //     "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
  //       "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
  //     "],"
  //     "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
  //       "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\""
  //     "],"
  //     "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
  //       "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\""
  //     "]"    
  //   "},"
  //   "\"RelayEnabled0\":{\"Enabled\":1,\"OnTime\":\"00D21:00:00\",\"OffTime\":\"00D05:00:00\"}"
  // "}";


  
  // #define USE_RULES_TEMPLATE
  // DEFINE_PGM_CTR(RULES_TEMPLATE)
  // "{"// for PIR to follow
  //   "\"Rule0\":{"
  //     "\"Trigger\":{"
  //       "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
  //       "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
  //       "\"DeviceName\":0,"
  //       "\"State\":\"On\""
  //     "},"
  //     "\"Command\":{"
  //       "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
  //       "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
  //       "\"DeviceName\":0," 
  //       "\"State\":\"Follow\""
  //     "}"
  //   "}"
  // "}";
  

#endif


#ifdef DEVICE_GAZEBO_SONOFF_4CHPRO
  #define DEVICENAME_CTR          "gazebo_sonoff_4chpro"
  #define DEVICENAME_FRIENDLY_CTR "Sonoff 4CH Pro"
  #define DEVICENAME_ROOMHINT_CTR  "Gazebo"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
  
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
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SONOFF_4CHPRO_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  // {"RfReceived":{"Data":11071155,"Bits":24,"Protocol":1,"Pulse":191,"millis":83687314,"Time":"18:48:02"}} // black doorbell

#endif


/**
 * @brief Presence detector
 * - PIR 
 * - BME280
 * - BH1750
 * 
 **/
#ifdef DEVICE_GAZEBO_SENSOR
  #define DEVICENAME_CTR          "gazebosensor"
  #define DEVICENAME_FRIENDLY_CTR "Gazebo Sensor"
  #define DEVICENAME_ROOMHINT_CTR "Gazebo"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
  
  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_BH1750
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT
    #define STRIP_SIZE_MAX 100
    
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS  
    
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS2812" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"Effects\"," 
    "\"ColourPalette\":15," 
    "\"PaletteEdit\": {"
      "\"ColourPalette\": 15,"
      "\"Data\": ["
        "4,5,0,0,0,"
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
    "\"BrightnessRGB\":100,"
    "\"TimeOn\":3600" // 1 hour
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
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
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
      "],"  
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
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


/**
 * 24W/meter
 * Approx 5.5m used, 24*5.5 = 132W so 132/12 = 11Amps at 12V
 * 20A 12V should be enough
 * */
#ifdef DEVICE_H801_RGBGAZEBO
  #define DEVICENAME_CTR          "h801_rgbgazebo"
  #define DEVICENAME_FRIENDLY_CTR "H801 RGB Gazebo RGBCCT Strip"
  #define DEVICENAME_ROOMHINT_CTR "Gazebo"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
  
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  // #define ENABLE_DEVFEATURE_DEBUG_PWM_CHANNELS_MQTT

  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  #define DISABLE_WEBSERVER
    
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_PWM  
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    #define ENABLE_DEVFEATURE_SOLAR_PALETTES
    #define ENABLE_DEVFEATURE_CHECK_SEGMENT_INIT_ERROR
    #define DEBUG_TARGET_ANIMATOR_SEGMENTS
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT  

    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
    // #define ENABLE_DEVFEATURE_TRANSITION_MODE_LEVEL_OPTIONS
    // #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_OLD_FUNCTIONS
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS

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
  
  // #define STRIP_SIZE_MAX 1
  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_HARDWARE_TYPE  "\":\"" "RGBCCT_PWM" "\","
  //   "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"GRBCW\","
  //   "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20\"},"
  //   "\"" D_JSON_COLOUR_PALETTE "\":10,"
  //   "\"" D_JSON_ANIMATIONMODE  "\":\"" D_JSON_EFFECTS "\","
  //   "\"" D_JSON_EFFECTS        "\"{\"Function\":\"Solid\"},"
  //   "\"" D_JSON_BRIGHTNESS     "\":100"
  // "}";

  #define STRIP_SIZE_MAX 2 // tmp fix
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "RGBCCT_PWM" "\","
    "\"" D_JSON_STRIP_SIZE       "\":1,"
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGBCW\","
    "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":2,\"" D_JSON_RATE "\":20},"
    "\"" D_JSON_COLOUR_PALETTE   "\":\"Solid Rgbcct 00\","
    "\"Hue\":345,\"Sat\":100,"
    "\"" D_JSON_ANIMATIONMODE    "\":\"Effects\","
    "\"" D_JSON_EFFECTS          "\":{\"Function\":\"Solid RGBCCT\"},"
    "\"" D_JSON_BRIGHTNESS_RGB   "\":100,"
    "\"" D_JSON_BRIGHTNESS_CCT   "\":100"
  "}";

#endif


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Garage ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

#ifdef DEVICE_OILTANK
  #define DEVICENAME_CTR          "oiltank"
  #define DEVICENAME_FRIENDLY_CTR "Oil Tank"
  #define DEVICENAME_ROOMHINT_CTR "Outside"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  // add db18 dropping from the waterproof box to know extra temp, perhaps add two for backup? (independant pin from tank sensors)

  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS_DS18X  
  #define USE_MODULE_SENSORS_SR04
    // #define USE_AMBIENT_TEMP_SENSOR_FOR_SPEEDOFSOUND
    // #define ENABLE_DEVFEATURE_ULTRASONIC_DURATION_RAW_THRESHOLD_CHECK
    #define ENABLE_DEVFEATURE_SR04_FILTERING_EMA
    #define ENABLE_DEVFEATURE_SR04_FILTERING_DEMA
    #define ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION
  
  #define USE_MODULE_CONTROLLER_TANKVOLUME

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"      
      #ifdef USE_MODULE_SENSORS_SR04
      "\"19\":\"" D_GPIO_FUNCTION_SR04_ECHO_CTR   "\","
      "\"18\":\"" D_GPIO_FUNCTION_SR04_TRIG_CTR  "\","  
      #endif 
      "\"27\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      "\"2\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  
  #define D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "AmbientTank"
  #define D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "OutsideGarage"
  
  #define D_DEVICE_BUTTON_1_CTR "FurnaceActive"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "[40,226,218,149,240,1,60,142],"
        "[40,255,169,120,53,22,4,240]"
      "]"  
    "}"
  "}";

#endif


#ifdef DEVICE_OILFURNACE
  #define DEVICENAME_CTR          "oilfurnace"
  #define DEVICENAME_FRIENDLY_CTR "Oil Furnace"
  #define DEVICENAME_ROOMHINT_CTR "Outside"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  #define USE_MODULE_CORE_RULES

  /**
   * 4 pipe sensors (boiler_in, boiler_out, exhaust, garage radiator) = use 3PIN screw terminal
   * light sensor for lock_out (analog and digital)
   * furnace power  
   * bme ambient
   * motion sensor
   */

  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS_ADC_INTERNAL_ESP32
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_DS18X
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_BH1750
  // #define USE_MODULE_SENSORS_MOTION // add this       == motion added with esp32 upgrade
  
  #define USE_MODULE_CONTROLLER_FURNACE_SENSOR
 
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"      
      #ifdef USE_MODULE_SENSORS_BME
      "\"26\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"27\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\""
      #endif
      // #ifdef USE_MODULE_SENSORS_MOTION
      // "\"5\":\""  D_GPIO_FUNCTION_SWT3_CTR "\","
      // #endif
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"18\":\"" D_GPIO_FUNCTION_SWT1_CTR  "\","
      "\"4\":\""  D_GPIO_FUNCTION_SWT2_CTR  "\","
      #endif  
      #ifdef USE_MODULE_SENSORS_ADC_INTERNAL_ESP32
      "\"35\":\"" D_GPIO_FUNCTION_ANALOG_INPUT0_CTR "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DS18X
      "\"19\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      #endif
      "\"2\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
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
      "],"
      "\"" D_MODULE_SENSORS_BH1750_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["    
        "[40,0,37,149,240,1,60,0],"
        "[40,124,194,149,240,1,60,110],"
        "[40,199,255,149,240,1,60,249],"
        "[40,246,153,149,240,1,60,174]"
      "]"  
    "},"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1}"
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
  #define DEVICENAME_ROOMHINT_CTR "Outside"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  #define USE_SSIDS_OUTSIDE_HOUSE
  
  /*
    Method should only activate if boot loop happens 10 times
    Method A: Switch to Wifi.begin hardcoded ssid/pw, start OTA and wait, rebooting every 10 minutes if wifi does not connect
    Method B: Begin wifi.ap as host, so I can directly connect to it via x.x.x.x
  */
  //#define DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS
  #define DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS

  #define USE_MODULE_NETWORKS_MQTT

  #define USE_MODULE_CORE_RULES

  #define ENABLE_DEVFEATURE_RELAY_ENABLE_TIME_WINDOW_LOCKS

  #define USE_MODULE_ENERGY_INTERFACE
  #define USE_MODULE_ENERGY_ADE7953

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE_SENSORS_MOTION

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
    #define MAX_RELAYS 2

  // #define ENABLE_DEVFEATURE_OTA_FALLBACK_ON_BOOT_LOOPING
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY2P5_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
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


#ifdef DEVICE_SHELLYDIMMER_TVROOM_CEILING
  #define DEVICENAME_CTR          "shellydimmer_tvroom"
  #define DEVICENAME_FRIENDLY_CTR "Shelly Dimmer TV Room"
  #define DEVICENAME_ROOMHINT_CTR "TV Room"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED       192,168,1,70
  
  #define DISABLE_SERIAL_LOGGING
  #define USE_MODULE_CORE_RULES
    
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_SHELLY_DIMMER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY_DIMMER2_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_DIMMER_FRIENDLY_NAME_LONG "Light"
  #define D_DEVICE_SWITCH_STAIRS_FRIENDLY_NAME_LONG "Switch_Bright"
  #define D_DEVICE_SWITCH_BATHROOM_FRIENDLY_NAME_LONG "Switch_Dim"

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



#ifdef DEVICE_TVROOM_H801_UPLIGHTS
  #define DEVICENAME_CTR          "h801_tvroom_uplights"//"h801_spareroom_wardrobe"
  #define DEVICENAME_FRIENDLY_CTR "H801 Spareroom Wardrobe"
  #define DEVICENAME_ROOMHINT_CTR "TV Room"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
      
  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  #define DISABLE_WEBSERVER
    
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_PWM  
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    #define ENABLE_DEVFEATURE_SOLAR_PALETTES
    #define ENABLE_DEVFEATURE_CHECK_SEGMENT_INIT_ERROR
    #define DEBUG_TARGET_ANIMATOR_SEGMENTS
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT  
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
    
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS
  
  
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
    "\"" D_JSON_STRIP_SIZE       "\":1,"
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"RGBCW\","
    "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":2,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\"},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"Solid Rgbcct 00\","
    "\"" D_JSON_ANIMATIONMODE  "\":\"Effects\","
    "\"" D_JSON_EFFECTS        "\"{\"Function\":\"Solid RGBCCT\"},"
    "\"" D_JSON_BRIGHTNESS     "\":100"
  "}";
  
#endif



#ifdef DEVICE_AMBILIGHT_SAMSUNG_SPAREROOM
  #define DEVICENAME_CTR          "ambilight_spareroom"
  #define DEVICENAME_FRIENDLY_CTR "Ambilight Spareroom"
  #define DEVICENAME_ROOMHINT_CTR "TV Room"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
  
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    #define STRIP_SIZE_MAX 55
    #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
    #define USE_SK6812_METHOD_DEFAULT
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT
  
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "SK6812" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_JSON_EFFECTS "\"," 
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
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

#endif

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Masterbedroom ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

#ifdef DEVICE_SHELLYDIMMER_MASTERBEDROOM_CEILING
  #define DEVICENAME_CTR          "ceiling_masterbedroom_light"//"shellydimmer_masterbedroom"
  #define DEVICENAME_FRIENDLY_CTR "Shelly Dimmer MBed Room"
  #define DEVICENAME_ROOMHINT_CTR "Masterbedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  #define DISABLE_SERIAL_LOGGING
  #define USE_MODULE_CORE_RULES
    
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_SHELLY_DIMMER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY_DIMMER2_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
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

#ifdef DEVICE_DIMMER_MASTERBEDROOM_LAMP1
  #define DEVICENAME_CTR          "dimmer_masterbedroom_lamp1"
  #define DEVICENAME_FRIENDLY_CTR "Dimmer Masterbedroom Lamp1"
  #define DEVICENAME_ROOMHINT_CTR "Masterbedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  #define DISABLE_SERIAL_LOGGING
  #define USE_MODULE_CORE_RULES
    
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_SHELLY_DIMMER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY_DIMMER2_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_DIMMER_FRIENDLY_NAME_LONG "Light"
  #define D_DEVICE_SWITCH_1_FRIENDLY_NAME_LONG "Input1"
  #define D_DEVICE_SWITCH_2_FRIENDLY_NAME_LONG "Input2"

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
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

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
    "\"" D_JSON_COLOUR_PALETTE "\":\"Solid Rgbcct 00\","
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
  #define DEVICENAME_ROOMHINT_CTR "Masterbedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
  
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
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT

    
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS


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
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
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
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_JSON_EFFECTS "\"," 
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

/**
 * @brief Above the door light downstairs toilet with google mini
 * 
 * wires needed
 * 
 * 8 core to 3d printed box
 * 4 core to led strip
 * mini wires up 
 * 
 */

#ifdef DEVICE_ENSUITESENSOR
  #define DEVICENAME_CTR          "ensuitesensor"
  #define DEVICENAME_FRIENDLY_CTR "Ensuite Sensor"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
     
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
    "\"" D_JSON_COLOUR_PALETTE "\":\"Solid Rgbcct 00\"," //ie 10
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
  #define DEVICENAME_CTR          "ceiling_ensuite_light"
  #define DEVICENAME_FRIENDLY_CTR "Shelly Dimmer Ensuite"
  #define DEVICENAME_ROOMHINT_CTR "Ensuite"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
      
  #define DISABLE_SERIAL_LOGGING
  #define USE_MODULE_CORE_RULES
    
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_SHELLY_DIMMER
      
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY_DIMMER2_CTR  "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
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

// Create the "above door light and mini set up"

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Landing ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

#ifdef DEVICE_SHELLYDIMMER_LANDING_CEILING
  #define DEVICENAME_CTR          "ceiling_landing_light"
  #define DEVICENAME_FRIENDLY_CTR "Shelly Dimmer Landing Room"
  #define DEVICENAME_ROOMHINT_CTR "Landing"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
  
  #define DISABLE_SERIAL_LOGGING
  #define USE_MODULE_CORE_RULES
    
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_SHELLY_DIMMER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY_DIMMER2_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
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
 * This is considered backup to heating
 * heating will use about 3 sensors on main heating esp32
 * Light strip to show colours of heat, I must add button that will toggle these lights for 1 minute to show, to save power
 * 
 * Put sensor on floor to connect to RGB strip and stairs motion sensor
 * or...
 * 
 * Put esp32 inside box
 * - flip strip, so its coming from the top of the tank down
 * - Add doorbell as way to show colours for 60 seconds (fit on to wood shelf)
 * - 3wire under carpet to stairs for motion
 * - 3wire to water sensors (ie previously backup, but now all the extra detailed ones)
 * 
 * screw terminals
 * 
 * DS18B20 INPUT
 * PIR     INPUT
 * RGB     OUTPUT
 * 3V3
 * 5V
 * GND
 * 
 * 
 * 
 * */
#ifdef DEVICE_IMMERSIONSENSOR
  #define DEVICENAME_CTR          "immersionsensor"
  #define DEVICENAME_FRIENDLY_CTR "Immersion Sensor ESP32"
  #define DEVICENAME_ROOMHINT_CTR "Landing"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  #define USE_MODULE_CORE_RULES
  
  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define USE_MODULE_LIGHTS_ADDRESSABLE
  //   #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  //   #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT

    
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    #define STRIP_SIZE_MAX 88                                                                        // Change: Set *total* length of string, segment0 will default to this length
    // #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
    // #define USE_SK6812_METHOD_DEFAULT
      #define ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT
    
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS



  
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_DS18X
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION
  #define USE_MODULE_SENSORS_BUTTONS
    #define ENABLE_DEVFEATURE_PHASEOUT_CLEARING_EVENT
    #define ENABLE_DEVFEATURE_BUTTON_SET_FLAG_BUTTON_SINGLE 0 // allow multipress = false

/**
 * @brief 
 * Add extra settings parameter that allows for "expected total device count" (relays+sensors) and allow a NEW alert topic (status/alert status/event) to tell me something is wrong
 * 
 */

  // #define ENABLE_DEVFEATURE_SENSORS_INTERFACE_SHOW_TEMPERATURE_AS_COLOUR  //should this be a "controller", or via interface "getTemp convert to heatmap"
  // #define SETTINGS_SENSORS_MQTT_IFCHANGED_PERIOD_SECONDS 60
  
  #define USE_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR

  #define USE_DEVFEATURE_SENSOR_COLOURS_TOP_TO_BOTTOM
     
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_DS18X
      "\"5\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR  "\","
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"18\":\""  D_GPIO_FUNCTION_SWT1_CTR "\","
      #endif
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"19\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      #endif 
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"23\":\"" D_GPIO_FUNCTION_KEY1_CTR  "\","
      #endif
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  #define D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "TankPosition00"  // Bottom
  #define D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "TankPosition20"
  #define D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG "TankPosition40"
  #define D_DEVICE_TEMP_4_FRIENDLY_NAME_LONG "TankPosition60"
  #define D_DEVICE_TEMP_5_FRIENDLY_NAME_LONG "TankPosition80"
  #define D_DEVICE_TEMP_6_FRIENDLY_NAME_LONG "TankPosition100" // Top

  #define D_DEVICE_TEMP_7_FRIENDLY_NAME_LONG "BoilerLoopTop"
  #define D_DEVICE_TEMP_8_FRIENDLY_NAME_LONG "BoilerLoopBottom"
  #define D_DEVICE_TEMP_9_FRIENDLY_NAME_LONG "ImmersionFeedIn"
  #define D_DEVICE_TEMP_10_FRIENDLY_NAME_LONG "FeedRed" // not sure what this does, cold in it seems
  
  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Stairs"

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
        "[40,0,118,128,59,71,5,227]," // 1   
        "[40,0,114,20,59,71,5,19],"   // 2  
        "[40,0,66,109,59,71,5,172],"  // 3  
        "[40,0,108,65,59,71,4,202],"  // 4  
        "[40,0,83,19,59,71,6,66],"    // 5         
        "[40,0,32,23,59,71,5,141],"   // 6
        "[40,0,40,61,59,71,4,134],"   // 
        "[40,0,66,140,59,71,6,136],"  // not calib
        "[40,0,95,50,59,71,5,126],"   // not calib
        "[40,0,149,87,59,71,5,240]"   // not calib
      "]"  
    "}"
  "}";

  /**
   * @brief Currently requires this exact template to make the light work, not sure what command is missing from controller class
   * 
   */
  #define STRIP_SIZE_MAX 33
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":0,"
      "\"" D_JSON_RATE_MS "\":1000"
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":\"" "Static" "\""
    "},"
    "\"ColourPalette\":10,"
    "\"Hue\":30,\"Sat\":100,"
    "\"BrightnessRGB\":0"
  "}";
  
  // 6,5,0,0,0,
  // 0,0,0,26,
  // 50,0,26,26,
  // 100,0,26,26,
  // 150,0,64,32,
  // 200,109,156,0,
  // 255,89,148,0
  
  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"// for PIR to follow
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



/**
 * This will become the final version of this panel, but requires animation fixing first
 * 
 * 
 * Nextion Panel on Landing (Look into larger screens)
 * PIR large for landing
 * BME in landing
 * Masterbedroom BME only (No lights)
 * Landing SK6812 for night lights
 * 
 * */
/**
 * @brief 
 * Cat5e Cable to Generic Room Sensor with BH1750, BME280 and HC-SR501 PIR
 * "*" pin = confirmed soldering
 * Ethernet      Function       ESP32         Note        
 *                           
 * ** Green (Landing2 - Nextion, BME, PIR) **************************************************************************
 * w/o           GND            GND           Black
 * o/w           5V             5V            Red
 * w/g           PIR          4*            DS18B20 water pipe sensors
 * bl/w          I2D            22*           BME in Dinning Room (Alternatively, DHT22 data pin)
 * w/bl          I2C            23*           BME in Dinning Room
 * g/w           3V3            3V3           White
 * w/br          NEXTION_RX                             NC
 * br/w          NEXTION_TX     25*           DHT22 (until BME is working)
 * ** Red (Masterbedroom1 - Panel) ************************************************************************** (BME280, LEDStrip 2 that is not yet coded. Will show temp when immersion is on for mums shower)
 * w/o           GND            GND           
 * o/w           5V             5V            
 * w/g           NEO               5*             
 * bl/w          I2D           18*                   
 * w/bl          I2C               19*            
 * g/w           3V           3V3           
 * w/br                                     
 * br/w              
 * 
 * 
 * ** Blue (Landing1 - Nextion Panel) **************************************************************************  (replace with 4 core for lighting?)
 * w/o           GND            GND           
 * o/w           5V             5V            
 * w/g           NEO           21*           Immersion relay (5v IO)
 * bl/w                      12*           Reserving, likely not possible      
 * w/bl                      13*              
 * g/w           3V3            3V3           
 * w/br                    14*            ds18b20 water, 4k7 pulled high        (comes from blue by connector)
 * br/w           -  
 * 
 * 
 * 
 * ** Red (Masterbedroom1 - Relay Board) **************************************************************************
 * Maybe add 4 wire for stairway PIR?
 * ** Red (Two Speaker wire) **************************************************************************
 * White        5V                           LED power injection
 * White/Black  GND 
 * **/
#ifdef DEVICE_LANDINGPANEL
  #define DEVICENAME_CTR            "landingpanel"
  #define DEVICENAME_FRIENDLY_CTR   "Landing Panel"
  #define DEVICENAME_ROOMHINT_CTR   "Landing"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
  
  #define USE_MODULE_CORE_RULES

  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ESP32

  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS_BME
    #define ENABLE_DEVFEATURE_BME_DUAL_DEVICES  
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION
    
  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define USE_MODULE_LIGHTS_ADDRESSABLE
  //   #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  //   #define STRIP_SIZE_MAX 15 //14 installed                                                                        // Change: Set *total* length of string, segment0 will default to this length
  //   #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  //   #define USE_SK6812_METHOD_DEFAULT
  //     #define ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32
  //   #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT
    
  //   #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
  //   #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
  //   #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
  //   #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
  //   #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS

  // #define USE_MODULE_DISPLAYS_NEXTION
  //   #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  //   #define NEXTION_DEFAULT_PAGE_NUMBER 1

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750)
      "\"27\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"14\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"26\":\""  D_GPIO_FUNCTION_SWT1_CTR "\","
      #endif
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      // "\"5\":\"" D_GPIO_FUNCTION_RGB_DATA2_CTR  "\","  // Masterbedroom panel
      #endif 
      #ifdef USE_MODULE_DISPLAYS_NEXTION
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
      #endif
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Landing"
  #define D_DEVICE_SENSOR_BME_LANDING_NAME       "Landing"
  #define D_DEVICE_SENSOR_BME_MASTERBEDROOM_NAME "Master Bedroom"


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
        "\"" D_DEVICE_SENSOR_BME_MASTERBEDROOM_NAME "\"," //76
        "\"" D_DEVICE_SENSOR_BME_LANDING_NAME  "\"" //77
      "]"
    "}"
    // "},"
    // "\"MQTTUpdateSeconds\":{\"IfChanged\":1}"
  "}";


  // #ifdef USE_MODULE_LIGHTS_INTERFACE
  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_HARDWARE_TYPE    "\":\"" "SK6812" "\","                //should be default
  //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  //   "\"AnimationMode\": \"Effects\","
  //   "\"ColourOrder\": \"GRBW\","
  //   "\"ColourPalette\":10,"
  //   "\"Effects\": {"
  //     "\"Function\":\"Solid\""
  //   "},"
  //   "\"Hue\":120," 
  //   "\"Sat\":100," 
  //   "\"BrightnessRGB\":0,"
  //   "\"BrightnessCCT\":100,"
  //   "\"CCT_TempPercentage\": 100,"
  //   "\"Transition\": {"
  //     "\"TimeMs\":400,"
  //     "\"RateMs\":1000"
  //   "},"
  //   "\"Light\":{\"TimeOn\":60}"
  // "}";
  // #endif // USE_MODULE_LIGHTS_INTERFACE

  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"// for PIR to follow
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
****** Bedroom (Long term only ie can remain when I leave home) ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


#ifdef DEVICE_BEDROOM_CEILINGFAN
  #define DEVICENAME_CTR          "bedroom_ceilingfan"
  #define DEVICENAME_FRIENDLY_CTR "Bedroom Ceiling Fan"
  #define DEVICENAME_ROOMHINT_CTR "Roaming"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
  
  #define USE_MODULE_CORE_RULES
  
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE_DRIVERS_BUZZER
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_CONTROLLER_SONOFF_IFAN
    #define USE_MODULE_TEMPLATE_SONOFF_IFAN03

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SONOFF_IFAN03_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

#endif


// Not yet installed but will be with dual buttons like ensuite
#ifdef DEVICE_SHELLYDIMMER_BEDROOM_CEILING
  #define DEVICENAME_CTR          "dimmer_bedroom_ceiling"
  #define DEVICENAME_FRIENDLY_CTR "Shelly Dimmer Spare Room"
  #define DEVICENAME_ROOMHINT_CTR "Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED       192,168,1,70
  
  #define DISABLE_SERIAL_LOGGING
  #define USE_MODULE_CORE_RULES
    
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_SHELLY_DIMMER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY_DIMMER2_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_DIMMER_FRIENDLY_NAME_LONG "Light"
  #define D_DEVICE_SWITCH_STAIRS_FRIENDLY_NAME_LONG "Switch_Bright"
  #define D_DEVICE_SWITCH_BATHROOM_FRIENDLY_NAME_LONG "Switch_Dim"

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
 * @brief  Above door like bathrooms
 * 
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
 * Use esp32 at bedroom to test the sidedoor of the house for utility
 * 
 * 
 **/
#ifdef DEVICE_BEDROOMSENSOR
  #define DEVICENAME_CTR          "bedroomsensor"
  #define DEVICENAME_FRIENDLY_CTR "Bedroom Sensor"
  #define DEVICENAME_ROOMHINT_CTR "Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define ENABLE_FEATURE_WATCHDOG_TIMER

  // #define USE_MODULE_SUBSYSTEM_SOLAR_LUNAR

  // #define USE_MODULE_SENSORS_INTERFACE
  //   #define ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  // #define USE_MODULE_SENSORS_BME
  //   #define ENABLE_DEVFEATURE_BME_DUAL_DEVICES
  // #define USE_MODULE_SENSORS_BH1750
  // #define USE_MODULE_SENSORS_SWITCHES
  // #define USE_MODULE_SENSORS_MOTION
  // #define USE_MODULE_SENSORS_DOOR

  //  { 20,  16, { 14, 30 }, 1, { 510,  30 }, { 14,  30 }, { 30, 14 }, false,  230 }  // 36 (Louvolite) with premable

  // #define USE_MODULE_CONTROLLER__LOUVOLITE_HUB // my room only
  #define USE_MODULE_CONTROLLER__LOUVOLITE_HUB_V2 //both rooms
    
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    #define STRIP_SIZE_MAX 88                                                                        // Change: Set *total* length of string, segment0 will default to this length
    #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
    #define USE_SK6812_METHOD_DEFAULT
      #define ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT
    
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "SK6812" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRBW\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" "Effects" "\"," 
    "\"ColourPalette\":10," 
    "\"Hue\":120," 
    "\"Sat\":100," 
    "\"Effects\":{"
      "\"Function\":\"Solid\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":900,"
      "\"RateMs\":1000"
    "},"    
    "\"CCT_TempPercentage\":50,"
    "\"BrightnessCCT\":100,"
    "\"BrightnessRGB\":0,"
    "\"TimeOn\":10"
  "}";

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"      
      #ifdef USE_MODULE_SENSORS_BME
      "\"26\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"27\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\""
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"5\":\""  D_GPIO_FUNCTION_SWT1_CTR "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DOOR
      "\"18\":\"" D_GPIO_FUNCTION_DOOR_OPEN_CTR     "\","
      "\"19\":\"" D_GPIO_FUNCTION_DOOR_LOCK_CTR     "\","
      #endif
      #if defined(USE_MODULE_CONTROLLER__LOUVOLITE_HUB) || defined(USE_MODULE_CONTROLLER__LOUVOLITE_HUB_V2)
      // "\"23\":\"" D_GPIO_FUNCTION__RF_433MHZ_RX__CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION__RF_433MHZ_TX__CTR   "\","
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
      "\"" D_MODULE_SENSORS_BH1750_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE "\""
      "],"  
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["         // so this is probably to be phased out?
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
      "],"  
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
      "],"  
      "\"" D_MODULE_SENSORS_DOOR_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "_Door" "\""
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
    // "},"
    // // Door Opening
    // "\"Rule1\":{"
    //   "\"Trigger\":{"
    //     "\"Module\":\"" D_MODULE_SENSORS_DOOR_FRIENDLY_CTR "\","
    //     "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
    //     "\"DeviceName\":0,"
    //     "\"State\":\"On\""
    //   "},"
    //   "\"Command\":{"
    //     "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
    //     "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
    //     "\"DeviceName\":1," 
    //     "\"State\":\"Follow\""
    //   "}"
    "}"
  "}";


#endif



//rgbmicro2/set/light///Scene
//{"//SceneName":"COLOUR//Scene","hue":25,"sat":100,"brt_rgb":100,"cct_temp":500,"brt_cct":100,"Time":0,"time_on":3600}
#ifdef DEVICE_RGBCLOCK_BEDROOM_WALL
  #define DEVICENAME_CTR          "rgbclock_bedroom_wall"
  #define DEVICENAME_FRIENDLY_CTR "RGB Clock 01"
  #define DEVICENAME_ROOMHINT_CTR "Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS // Not ready to remove
  #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
  #define STRIP_SIZE_MAX 94//93  
  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  #define USE_SK6812_METHOD_DEFAULT

  // #define USE_DEVFEATURE_SK6812_METHOD_DEFAULT_ALTERNATE
  
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
  // #define USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK


    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS
    


  #define DISABLE_WEBSERVER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      #endif 
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "SK6812" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"grb\","
    "\"" D_JSON_COLOUR_TYPE "\":3,"//\"RGBW\","   //3=rgb, 4=rgbw
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":950,"
      "\"" D_JSON_RATE_MS "\":1000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\""
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":\"Clock Basic 01\""
    "},"
    "\"" D_JSON_COLOUR_PALETTE "\":34,"
    "\"BrightnessCCT\":0,"
    "\"" D_JSON_BRIGHTNESS_RGB "\":100"

  "}";


#endif


/**
 * @brief Currently its own device, but try integrate into bedroomsensor in attic 
 * 
 */
#ifdef DEVICE_BEDROOM_LOUVOLITE_HUB
  #define DEVICENAME_CTR          "bedroom_louvolite_hub"
  #define DEVICENAME_FRIENDLY_CTR "Bedroom Louvolite Blind Hub"
  #define DEVICENAME_ROOMHINT_CTR "Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  // #define USE_MODULE_DRIVERS_INTERFACE

  // #define USE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED
  //   #define ENABLE_DEVFETURE_DISABLE_EXTENDED_FEATURES_START

  //  { 20,  16, { 14, 30 }, 1, { 510,  30 }, { 14,  30 }, { 30, 14 }, false,  230 }  // 36 (Louvolite) with premable

  #define USE_MODULE_CONTROLLER__LOUVOLITE_HUB

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_CONTROLLER__LOUVOLITE_HUB
      // "\"23\":\"" D_GPIO_FUNCTION__RF_433MHZ_RX__CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION__RF_433MHZ_TX__CTR   "\","
      #endif  
      "\"2\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  
#endif


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Roaming devices ****************************************************************************************************************************************************
********

HVAC controllers here

- Airpurifier
- Oil Heater
- Kitchen Electric Heater (Make a short wire version)

********************************************************************************************************************************************
*******************************************************************************************************************************************/



#ifdef DEVICE_AIRPURIFIER
  #define DEVICENAME_CTR          "airpurifier"
  #define DEVICENAME_FRIENDLY_CTR "Air Purifier 1"
  #define DEVICENAME_ROOMHINT_CTR "Roaming"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
  
  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define USE_MODULE_CORE_RULES
  
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE_DRIVERS_BUZZER
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_CONTROLLER_SONOFF_IFAN
    #define USE_MODULE_TEMPLATE_SONOFF_IFAN03

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SONOFF_IFAN03_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

#endif


#ifdef DEVICE_FLOORFAN1
  #define DEVICENAME_CTR          "floorfan1"
  #define DEVICENAME_FRIENDLY_CTR "IFan Floor Fan 1"
  #define DEVICENAME_ROOMHINT_CTR "Roaming"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED       192,168,1,70
  
  #define USE_MODULE_CORE_RULES
  
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE_DRIVERS_BUZZER
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_CONTROLLER_SONOFF_IFAN
    #define USE_MODULE_TEMPLATE_SONOFF_IFAN03

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SONOFF_IFAN03_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


#endif




#ifdef DEVICE_FLOORFAN2
  #define DEVICENAME_CTR          "floorfan2"
  #define DEVICENAME_FRIENDLY_CTR "IFan Floor Fan 2"
  #define DEVICENAME_ROOMHINT_CTR "Roaming"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED       192,168,1,70
  
  #define USE_MODULE_CORE_RULES
  
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE_DRIVERS_BUZZER
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_CONTROLLER_SONOFF_IFAN
    #define USE_MODULE_TEMPLATE_SONOFF_IFAN03

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SONOFF_IFAN03_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

#endif



#endif // MFIRMWARECUSTOM_SECRET_HOME_LONGTERM_H
