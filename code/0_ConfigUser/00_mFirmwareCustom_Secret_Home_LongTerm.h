#ifndef _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_HOME_LONGTERM_TEMPLATES_H
#define _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_HOME_LONGTERM_TEMPLATES_H

/***
 * - CrashDump needs to be saved and transmitted over mqtt on request in a json message, that can copy and pasted into decoder
 * - Figure out how to have the binary saved into another directory for each device, so it may be used later as the "latest version" 
 *   to reflash another device with the same code if needed (ie broken device that worked on older code)
 * 
 * 
 * Feb2023 List
 * 
 * KitchenSensor
 *  ** Print new housing with BH1750 inside with cover, BME680 outside hole only
 *  ** upgrade wiring to 6C
 * UtilitySensor
 *  ** Print new housing with BH1750 inside with cover, BME680 outside hole only
 *  ** upgrade wiring to 6C
 *  ** Might still need connector since it needs to be removable with wire left behind cabinets!! same for KS, still worth changing wire for untwisted 6C
 * 
 * BedroomSensor
 *  ** Print new housing with BH1750 inside with cover, BME680 outside hole only
 *  ** upgrade wiring to 6C
 *  ** Leave lighting with current tape method, but move wiring into wall
 *  ** Add 4C wire for the door sensor (or 3C if available)
 * 
 * AtticSensor
 *  ** Change to esp32
 *  ** Add water tank ultrasonic sensor, mostly as a long term ULT tester
 * HallwaySensor/Radiatorfan
 *  ** print casing with new BME680 and mount on wall/rail
 * 
 * 
 * **/

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
 * 
 * Design Generic Device that I can use for most devices in the future.
 * Possibly also get them manufactured if I reproduce them all with same design and test it.
 * 
 * Small
 *  ** ESP32
 *  ** Single Level Shifted out for RGB on ALL basic boards (I will use it in most cases)
 *  ** 2p header for power
 *  ** 3p header for LEDs out
 *  ** instead of tieing unused levelshifter to gnd to disable, connect to header that can be swapped to group enable/disable 3 outputs together
 *
 *
 * Medium
 *  ** All of small (optional up to 4 RGB outputs)
 *  ** One RJ45 wired for standard use
 *     *** gnd
 *     *** 5v
 *     *** gpio_1
 *     *** i2c_data (data is blue), also works for 1-wire
 *     *** i2c_clock (clock is white/blue)
 *     *** 3v3 on green
 *     *** serial2_rx
 *     *** serial2_tx transmit out on brown wire (serial as gpio on esp32 can be anything too)
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
// #define DEVICE_RGBCOOKER
// #define DEVICE_KITCHENSENSOR
// #define DEVICE_NEXTION_HEATING_KITCHEN_DISPLAY
// #define DEVICE_RGBFRIDGE
// #define DEVICE_DEFAULT_SONOFF_BASIC__KITCHEN_EXTRACTOR_FAN

/**
Utility
  - crystal1
  - pir, bme, door sensor
**/
// #define DEVICE_RGBUTILITY
// #define DEVICE_UTILITYSENSOR

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
// #define DEVICE_DEFAULT_SONOFF_BASIC_LAMP1
// #define DEVICE_DEFAULT_SONOFF_BASIC_LAMP2
// #define DEVICE_DEFAULT_SONOFF_BASIC_LAMP3
// #define DEVICE_DEFAULT_SHELLY_DIMMER_LAMP1
// #define DEVICE_DEFAULT_SONOFF_BASIC__LIVING_ROOM_LAMP1
// #define DEVICE_DEFAULT_SONOFF_BASIC__LIVING_ROOM_LAMP2

/**
 * @bri ef Attic
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
// #define DEVICE_HVAC_HEATING_MAIN
// #define DEVICE_HVAC_HEATING_MAIN_2023V2

/**
 *  Landing + Hotpress
  - CeilingLight = dimmer, 2 inputs
  - LandingPanel = PIR, Nextion, BME, light strip
  - dht
 **/  
// #define DEVICE_SHELLYDIMMER_LANDING_CEILING
#define DEVICE_IMMERSION_CONTROL_PANEL

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
// #define DEVICE_ENSUITE_CEILING_EXTRACTOR_FAN

/**
Bathroom
  - home mini
  - shellydimmer on pull cord?
**/
// #define DEVICE_BATHROOM_SENSOR

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
#ifdef DEVICE_DEFAULT_SONOFF_BASIC_15
  #define DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_SOCKET_NUMBER_CTR 15
  #define DEVICENAME_ROOMHINT_CTR "Hallway"  
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
#endif
#ifdef DEVICE_DEFAULT_SONOFF_BASIC__CANDLE_WARMER
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


#ifdef DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_CTR          "socket_number_" STR2(DEVICENAME_SOCKET_NUMBER_CTR)
  #define DEVICENAME_FRIENDLY_CTR "Socket Number " STR2(DEVICENAME_SOCKET_NUMBER_CTR)
  
  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

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
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
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
  
  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

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
 * @brief 
 *
 * REMOVED -- Now "KitchenShellyI4" 
 * 
 *
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
  #define DEVICENAME_ROOMHINT_CTR   "Kitchen"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  // #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_NEXTION
  #define NEXTION_DEFAULT_PAGE_NUMBER 2  
    #define ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT

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

  // #define DISABLE_NETWORK

  #define DISABLE_SERIAL
  #define DISABLE_SERIAL0_CORE
  #define DISABLE_SERIAL_LOGGING

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    /********* Group: Needed to build ************************/
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
    /********* Group: Ready for full integration ************************/
    // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    /********* Group: Testing ************************/
    #define ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD
    #define ENABLE_DEVFEATURE_REMOVE_INIT_OUTSIDE_OF_PALETTE_CLASS
    #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
    #define ENABLE_DEVFEATURE_UNNEEDED_WLED_ONLY_PARAMETERS
    #define ENABLE_DEVFEATURE_ALWAYS_LOAD_PALETTE_WHEN_NOT_TRANSITIONING
    // #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
    /********* Group: Debug options only ************************/
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
    #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
    #define ENABLE_DEBUG_SERIAL
    // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    // #define ENABLE_LOG_LEVEL_DEBUG
    // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

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

  #define STRIP_SIZE_MAX 33
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "HardwareType":"WS28XX",
    "ColourOrder":"RGB",
    "AnimationMode":"Effects",
    "ColourPalette":"Pastel 02",
    "Effects": {
      "Function":1,
      "Intensity":50
    },
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  }
  )=====";

#endif // DEVICE_RGBSHELF


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
    
  #define ESP32
  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_MODULE_CORE_RULES
       
  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
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

#ifdef DEVICE_UTILITY_WALLSWITCH_OUTSIDE_INPUTS_ONLY_SHELLYI4
  #define DEVICENAME_CTR          "utility_switch_i4"
  #define DEVICENAME_FRIENDLY_CTR "Utility WallSwitch I4"
  #define DEVICENAME_ROOMHINT_CTR "Utility"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_LSM303D)
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      #endif  
      "\"2\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
#endif


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

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS_BME
    
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

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** ROOM: Downstairs Toilet ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

/**
 * @brief Lets go ahead and upgrade the lighting to the newer version
 * Upgrade room sensing to new 3D printed type and glue facing down
 * Use new 6C wire, and edit 3D box to have single exit hole with right diameter so no glue is needed
 * Wait until BME680 arrives, sensor will poke out to sidedoor facing side, with BME module facing down so dust cant collect, use smallest slot possible! (BME will be internal!)
 * 
 */
#ifdef DEVICE_CONSUMERUNIT
  #define DEVICENAME_CTR          "consumerunit"
  #define DEVICENAME_FRIENDLY_CTR "Consumer Unit"
  #define DEVICENAME_ROOMHINT_CTR "Downstairs Toilet"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
    
  #define USE_TEMPLATED_DEFAULT_OTA_RECOVERY_METHODS

  #define DEVICENAMEBUFFER_NAME_BUFFER_LENGTH 800

  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION
    #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT

  #define USE_MODULE_ENERGY_INTERFACE
  #define USE_MODULE_ENERGY_PZEM004T_V3
    #define ENABLE_DEVFEATURE_REDUCE_SUBORDINATE_MQTT_REPORTING_ENERGY // If energy_interface is primary reporting, reduce pzem to slower (debug only)

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES_SK6812_FOR_ROOM_SENSORS
  #define USE_TEMPLATED_DEFAULT_LIGHTING_TEMPLATE_SK6812_FOR_ROOM_SENSORS__BOOT_STATE_OFF
    #define STRIP_SIZE_MAX 60

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
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":60,\"ConfigPeriod\":60}," 
    "\"MQTT_Interface_Priority\":{\"" D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR "\":1}" // Each interface will have ability to reduce its subclass mqtt "ifchanged" rate
  "}";


#endif


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** ROOM: Attic ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**
 * 
 * Change to ESP32
 * Add ultrasonic sensor to the cold water tank (glue, cut in 2 inches on lid so wire can be brought over and ziptie so it cant fall into the water ever)
 *  DONT FLASH UNTIL ITS ESP32
 * 
 * */
#ifdef DEVICE_ATTIC_SENSOR
  #define DEVICENAME_CTR          "atticsensor"
  #define DEVICENAME_FRIENDLY_CTR "Attic Sensor"
  #define DEVICENAME_ROOMHINT_CTR "Attic"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID
  
  #define USE_MODULE_SUBSYSTEM_SOLAR_LUNAR

    #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS__DS18X20_ESP32_2023
  #define USE_MODULE_SENSORS_SR04
    #define ENABLE_DEVFEATURE_SR04_FILTERING_EMA
    #define ENABLE_DEVFEATURE_SR04_FILTERING_DEMA
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750)
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_SR04
      "\"19\":\"" D_GPIO_FUNCTION_SR04_ECHO_CTR   "\","
      "\"18\":\"" D_GPIO_FUNCTION_SR04_TRIG_CTR  "\","  
      #endif 
      #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
      "\"5\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR  "\"," 
      #endif
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Attic"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Attic"  
  #define D_DEVICE_SENSOR_SR04_FRIENDLY_NAME_LONG "ColdWaterHeaderTank"  

  #define D_DEVICE_SENSOR_DB18S20_01_NAME        "Cold Water Tank"
  #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,50,199,3,0,0,0,102]"
  #define D_DEVICE_SENSOR_DB18S20_02_NAME        "Attic Top"
  #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,255,100,30,7,142,92,154]"
  #define D_DEVICE_SENSOR_DB18S20_03_NAME        "Attic Bottom"
  #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,255,100,30,7,143,73,252]"

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
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SUBSYSTEM_SOLAR_LUNAR_FRIENDLY_CTR "\":["
        "\"" "Home" "\""
      "],"  
      "\"" D_MODULE_SENSORS_SR04_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_SR04_FRIENDLY_NAME_LONG "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":{" 
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\":" D_DEVICE_SENSOR_DB18S20_03_ADDRESS ""
      "}"   
    "}"
  "}";

#endif


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** ROOM: Livingroom ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

#ifdef DEVICE_LIVINGROOMSENSOR
  #define DEVICENAME_CTR          "livingroomsensor"
  #define DEVICENAME_FRIENDLY_CTR "Living Room Sensor"
  #define DEVICENAME_ROOMHINT_CTR "Livingroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  #define USE_MODULE_CORE_RULES
     
  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
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



/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** ROOM: Hallway ****************************************************************************************************************************************************
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
 * splitting motion out from this, so the CAMERA can be added into the one unit
 * 
 */
#ifdef DEVICE_RADIATORFAN                                                        
  #define DEVICENAME_CTR          "radiatorfan"
  #define DEVICENAME_FRIENDLY_CTR "Radiator Fan"
  #define DEVICENAME_ROOMHINT_CTR "Hallway"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS_BME
    
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION
  #define USE_MODULE_SENSORS__DS18X20_ESP32_2023

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
      #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
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

  #define D_DEVICE_SENSOR_DB18S20_01_NAME     D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG
  #define D_DEVICE_SENSOR_DB18S20_02_NAME     D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG
  #define D_DEVICE_SENSOR_DB18S20_03_NAME     D_DEVICE_TEMP_RADIATOR_FRIENDLY_NAME_LONG
  #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS "[40,170,231,153,29,19,02,43]"
  #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS "[40,170,221,101,29,19,02,113]"
  #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS "[40,159,147,  2, 0, 0, 0,117]" //hex  28:9F:93:2:0:0:0:75    // Longer wire on radiator pipe (this will be used to trigger fan)
  
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
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":{" 
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\":" D_DEVICE_SENSOR_DB18S20_03_ADDRESS ""
      "}"  
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
****** ROOM: Understairs ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**
 * @brief 
 * 
 * Heating controller for first permenant install including manual sensing of voltage
 * 
 * 3 20A Relays
 * 3 way Voltage active inputs (on the switched live relay outputs)
 * 1 DHT22 from diningroom
 * 9 DB18S20 on water pipes
 * 1 Nextion panel currently in vertical mode, likely to have different interface HVAC remote (e.g. basic feedback for parents, working, connected etc)
 * 4 SK1612 on panel to show if any heating is active (perhaps reduce to 3 leds and treat immersion split?)
 * 
 * Ethernet      Function       ESP32         Note        
 * 
 * ** Bottom Entry (Relay Control Unit) **************************************************************************
 * w/o           GND            GND           
 * o/w           5V             5V            
 * w/g           P_DS           D27                
 * bl/w          R_US           D5                  
 * w/bl          P_US           D33                
 * g/w           R_DS           D18          
 * w/br          P_WB           D26                
 * br/w          R_WB           D19  
 
 * ** Left Entry (Nextion Display) **************************************************************************
 * w/o           GND            GND           
 * o/w           5V             5V            
 * w/g           NEO            4             SK6812 4 pixels of rgbW
 * bl/w                                      
 * w/bl                                      
 * g/w           3V3            3V3           
 * w/br          RX2            17*              Nextion TX
 * br/w          TX2            16*              Nextion RX   -- "SERIAL TX" of Serial will always be dominant colour (brown) as its important to know the output pin
 * Twin          Switch
 * Twin          Switch
 * 
 **/
#ifdef DEVICE_HVAC_HEATING_MAIN_2023V2
  #define DEVICENAME_CTR          "heating"
  #define DEVICENAME_FRIENDLY_CTR "HVAC House Heating 2023#2"
  #define DEVICENAME_ROOMHINT_CTR "Hallway"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
  
  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define DISABLE_SLEEP // loops per second less than 1hz // I need to make an "mqtt/alert" channel that lets me know this
  
  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES_SK6812_FOR_ROOM_SENSORS
  // #define USE_TEMPLATED_DEFAULT_LIGHTING_TEMPLATE_SK6812_FOR_ROOM_SENSORS__BOOT_STATE_OFF
    #define STRIP_SIZE_MAX 10

  #define USE_MODULE_CONTROLLER_HVAC
    #define HEATING_DEVICE_MAX 3
    #define ENABLE_DEVFEATURE_CONTROLLER_HVAC_NEW_HVAC_TIMEON

  #define USE_MODULE_SENSORS_INTERFACE
  #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS__DS18X20_ESP32_2023
    #define DS18X20_MAX_SENSORS 20
      // #define ENABLE_DEBUG_MQTT_CHANNEL_DB18X20      
  #define USE_MODULE_SENSORS_DHT
              
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_SENSORS_SWITCHES
  
    // #define SHOW_SPLASH
    // #define USE_MODULE_DISPLAYS_NEXTION
    // #define NEXTION_DEFAULT_PAGE_NUMBER 3   // I should add "p[c]" where c means current page, so I need to search and replace "p[c]" as "p[0]"

  // Actual
  #define GPIO_NAME_ZONE0_DOWNSTAIRS_RELAY  D_GPIO_FUNCTION_REL1_INV_CTR
  #define GPIO_NAME_ZONE1_UPSTAIRS_RELAY    D_GPIO_FUNCTION_REL2_INV_CTR
  #define GPIO_NAME_ZONE2_BOILER_RELAY      D_GPIO_FUNCTION_REL3_INV_CTR

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"5\":\""  GPIO_NAME_ZONE0_DOWNSTAIRS_RELAY  "\","
      "\"18\":\"" GPIO_NAME_ZONE1_UPSTAIRS_RELAY    "\","
      "\"19\":\"" GPIO_NAME_ZONE2_BOILER_RELAY      "\","
      #endif
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"33\":\""  D_GPIO_FUNCTION_SWT1_INV_CTR  "\","
      "\"27\":\""  D_GPIO_FUNCTION_SWT2_INV_CTR  "\","
      "\"26\":\""  D_GPIO_FUNCTION_SWT3_INV_CTR  "\","
      #endif  
      #ifdef USE_MODULE_SENSORS_DHT
      "\"25\":\"" D_GPIO_FUNCTION_DHT22_1_CTR   "\","
      #endif
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      #endif 
      #ifdef USE_MODULE_DISPLAYS_NEXTION
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
      #endif
      #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
      "\"23\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      #endif    
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""   // builtin led
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  /**
   * @brief Drivers and Sensors for HVAC zones
   **/
  #define D_DEVICE_DRIVER_RELAY_0_NAME "Downstairs"
  #define D_DEVICE_DRIVER_RELAY_1_NAME "Upstairs"
  #define D_DEVICE_DRIVER_RELAY_2_NAME "Boiler"

  #define D_DEVICE_SENSOR_DHT_0_NAME "Downstairs_DHT"

  /**
   * @brief HVAC zones
   **/
  #define D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "Downstairs"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "Upstairs"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "Boiler"

  /** 
   * Pin_DS
   * */
  #define D_DEVICE_SENSOR_DB18S20_01_NAME        "Water21-Upstairs"
  #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,208,174,149,240,1,60,127]"

  #define D_DEVICE_SENSOR_DB18S20_02_NAME        "Water22-HotCross"
  #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,168,253,149,240,1,60,157]"

  #define D_DEVICE_SENSOR_DB18S20_03_NAME        "Water23-R/C"
  #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,12,164,2,0,0,0,72]"

  #define D_DEVICE_SENSOR_DB18S20_04_NAME        "Water24-Mains"
  #define D_DEVICE_SENSOR_DB18S20_04_ADDRESS     "[40,9,77,4,0,0,0,131]"

  #define D_DEVICE_SENSOR_DB18S20_05_NAME        "Water25-HotFromBoiler"
  #define D_DEVICE_SENSOR_DB18S20_05_ADDRESS     "[40,121,172,3,0,0,0,138]"

  #define D_DEVICE_SENSOR_DB18S20_06_NAME        "Water26-Downstairs"
  #define D_DEVICE_SENSOR_DB18S20_06_ADDRESS     "[40,205,241,149,240,1,60,148]"

  #define D_DEVICE_SENSOR_DB18S20_07_NAME        "Water27-R/H"
  #define D_DEVICE_SENSOR_DB18S20_07_ADDRESS     "[40,195,112,2,0,0,0,178]"

  #define D_DEVICE_SENSOR_DB18S20_08_NAME        "Water28-HotFromFurnace"
  #define D_DEVICE_SENSOR_DB18S20_08_ADDRESS     "[40,103,49,3,0,0,0,153]"

  #define D_DEVICE_SENSOR_DB18S20_09_NAME        "Water29-WaterBoiler"
  #define D_DEVICE_SENSOR_DB18S20_09_ADDRESS     "[40,183,162,149,240,1,60,24]"


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_DRIVER_RELAY_0_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_2_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_DRIVER_RELAY_0_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_2_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_05_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_06_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_07_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_08_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_09_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DHT_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":{" 
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\":" D_DEVICE_SENSOR_DB18S20_03_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\":" D_DEVICE_SENSOR_DB18S20_04_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_05_NAME "\":" D_DEVICE_SENSOR_DB18S20_05_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_06_NAME "\":" D_DEVICE_SENSOR_DB18S20_06_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_07_NAME "\":" D_DEVICE_SENSOR_DB18S20_07_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_08_NAME "\":" D_DEVICE_SENSOR_DB18S20_08_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_09_NAME "\":" D_DEVICE_SENSOR_DB18S20_09_ADDRESS ""
      "}"  
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_06_NAME "\""
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
        "}"
      "]"
    "},"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60}"  
  "}";
  
#endif

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** ROOM: Outside ****************************************************************************************************************************************************
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

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  /*
    Method should only activate if boot loop happens 10 times
    Method A: Switch to Wifi.begin hardcoded ssid/pw, start OTA and wait, rebooting every 10 minutes if wifi does not connect
    Method B: Begin wifi.ap as host, so I can directly connect to it via x.x.x.x
  */
  //#define DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS
  // #define DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS

  #define USE_MODULE_NETWORKS_MQTT

  #define USE_MODULE_CORE_RULES // I still need the rule to report motion!!

  // #define ENABLE_DEVFEATURE_RELAY_ENABLE_TIME_WINDOW_LOCKS

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

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Sidedoor"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "DrivewayEnd"
  #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Sidedoor"
  #define D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "Disconnected"  
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
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
    "}"
  "}";

#endif


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** ROOM: Gazebo ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

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
  
  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_BH1750
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION

  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define USE_MODULE_LIGHTS_ADDRESSABLE
  //   #define STRIP_SIZE_MAX 100
  //   #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  //   /********* Group: Needed to build ************************/
  //   #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
  //   /********* Group: Ready for full integration ************************/
  //   // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   /********* Group: Testing ************************/
  //   #define ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD
  //   #define ENABLE_DEVFEATURE_REMOVE_INIT_OUTSIDE_OF_PALETTE_CLASS
  //   #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
  //   #define ENABLE_DEVFEATURE_UNNEEDED_WLED_ONLY_PARAMETERS
  //   #define ENABLE_DEVFEATURE_ALWAYS_LOAD_PALETTE_WHEN_NOT_TRANSITIONING
  //   // #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
  //   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
  //   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
  //   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
  //   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
  //   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
  //   // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
  //   /********* Group: Debug options only ************************/
  //   #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
  //   #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
  //   #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
  //   #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
  //   #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
  //   #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
  //   #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
  //   #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
  //   #define ENABLE_DEBUG_SERIAL
  //   // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
  //   // #define ENABLE_LOG_LEVEL_DEBUG
  //   // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
  //   // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items
    

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
      "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
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
    "}"
  "}";

  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "HardwareType":"WS28XX",
    "AnimationMode":"Effects",
    "ColourOrder":"grb",
    "ColourPalette":"Christmas 01",
    "Effects": {
      "Function":1,
      "Intensity":50
    },
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "SegColour": {
      "Hue": 120,
      "Sat": 100,
      "SubType":3
    },
    "BrightnessRGB_255": 255,
    "BrightnessCCT_255": 1
  }
  )=====";
  
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
****** ROOM: Garage ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

#ifdef DEVICE_OILTANK
  #define DEVICENAME_CTR          "oiltank"
  #define DEVICENAME_FRIENDLY_CTR "Oil Tank"
  #define DEVICENAME_ROOMHINT_CTR "Outside"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  // add db18 dropping from the waterproof box to know extra temp, perhaps add two for backup? (independant pin from tank sensors)

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS__DS18X20_ESP32_2023  
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
  

  #define D_DEVICE_SENSOR_DB18S20_01_NAME "AmbientTank"
  #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS "[40,226,218,149,240,1,60,142]"
  #define D_DEVICE_SENSOR_DB18S20_02_NAME "OutsideGarage"
  #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS "[40,255,169,120,53,22,4,240]"

  #define D_DEVICE_SR04_FRIENDLY_NAME_LONG "LevelSensor"
  
  #define D_DEVICE_BUTTON_1_CTR "FurnaceActive"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SR04_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SR04_FRIENDLY_NAME_LONG "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":{" 
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ""
      "}"  
    "}"
  "}";

#endif


#ifdef DEVICE_OILFURNACE
  #define DEVICENAME_CTR          "oilfurnace"
  #define DEVICENAME_FRIENDLY_CTR "Oil Furnace"
  #define DEVICENAME_ROOMHINT_CTR "Garage"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_MODULE_CORE_RULES

  /**
   * 4 pipe sensors (boiler_in, boiler_out, exhaust, garage radiator) = use 3PIN screw terminal
   * light sensor for lock_out (analog and digital)
   * furnace power  
   * bme ambient
   * motion sensor
   */

  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS_ADC_INTERNAL_ESP32
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS__DS18X20_ESP32_2023
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
      #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
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

  #define D_DEVICE_SENSOR_DB18S20_01_NAME     D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG
  #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS "[40,0,37,149,240,1,60,0]"
  #define D_DEVICE_SENSOR_DB18S20_02_NAME     D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG
  #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS "[40,124,194,149,240,1,60,110]"
  #define D_DEVICE_SENSOR_DB18S20_03_NAME     D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG
  #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS "[40,199,255,149,240,1,60,249]"
  #define D_DEVICE_SENSOR_DB18S20_04_NAME     D_DEVICE_TEMP_4_FRIENDLY_NAME_LONG
  #define D_DEVICE_SENSOR_DB18S20_04_ADDRESS "[40,246,153,149,240,1,60,174]"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\""
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
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":{" 
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\":" D_DEVICE_SENSOR_DB18S20_03_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\":" D_DEVICE_SENSOR_DB18S20_04_ADDRESS ""
      "}"  
    "}"  
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60}"  
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
  #define DEVICENAME_FRIENDLY_CTR "Garage Garden Lights"
  #define DEVICENAME_ROOMHINT_CTR "Outside"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70


  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID
  
  
  //#define DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS
  // #define DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS
  #define DEVICE_DEFAULT_CONFIGURATION_MODE_C_SWITCHES_ARE_MOTION_DETECTION_REPORTING_ONLY_OUTPUTS_ARE_REMOTE_CONTROLLED //sidedoor and garage new method

  // #define ENABLE_DEVFEATURE_RELAY_ENABLE_TIME_WINDOW_LOCKS
    // #define ENABLE_DRIVERS_RELAYS_TIME_LOCKS

  #define USE_MODULE_NETWORKS_MQTT

  #define USE_MODULE_CORE_RULES


  #define USE_MODULE_ENERGY_INTERFACE
  #define USE_MODULE_ENERGY_ADE7953

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE_SENSORS_MOTION

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
    #define MAX_RELAYS 2
    
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
    "}"   
    #ifdef ENABLE_DRIVERS_RELAYS_TIME_LOCKS
    ","
    "\"RelayEnabled0\":{\"Enabled\":1,\"OnTime\":\"00D20:00:00\",\"OffTime\":\"00D05:00:00\"},"
    "\"RelayEnabled1\":{\"Enabled\":1,\"OnTime\":\"00D20:01:00\",\"OffTime\":\"00D05:00:00\"}"
    #endif // ENABLE_DRIVERS_RELAYS_TIME_LOCKS
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

  /**
   * New option: No local control, but rule for converting switch into motion event on mqtt
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
  #ifdef DEVICE_DEFAULT_CONFIGURATION_MODE_C_SWITCHES_ARE_MOTION_DETECTION_REPORTING_ONLY_OUTPUTS_ARE_REMOTE_CONTROLLED
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Motion Event = Switch0
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
    "},"
    // Motion Event = Switch1
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":1,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":1," 
        "\"State\":\"Follow\""
      "}"
    "}"
  "}";
  #endif // DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS


#endif // DEVICE_NAME


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** ROOM: TV Room ****************************************************************************************************************************************************
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





/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** ROOM: Masterbedroom ****************************************************************************************************************************************************
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


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** ROOM: Ensuite ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

/**
 * @brief 
 * Cat5e Cable to Generic Room Sensor with BH1750, BME280 and HC-SR501 PIR
 * 
 * Ethernet      Function       ESP32         6P/2P Connector        
 * 
 * w/o           GND            GND           Black
 * o/w           5V             5V            Red
 * w/g           PIR            5             Yellow
 * bl/w          I2D            18            Green      // check i2c 
 * w/bl          I2C            19            Orange
 * g/w           3V3            3V3           White
 * w/br           -                           Black     // Button? Led?
 * br/w           -                           Red
 * 
 * 
 * 4CORE to RGBW Strip
 * Red          5v
 * Black        Gnd
 * Green        RGB Strip 1
 * White        ??
 * 
 * 
 **/
#ifdef DEVICE_ENSUITESENSOR
  #define DEVICENAME_CTR          "ensuitesensor"
  #define DEVICENAME_FRIENDLY_CTR "Ensuite Sensor"
  #define DEVICENAME_ROOMHINT_CTR "Ensuite"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
    
  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_MODULE_CORE_RULES
       
  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION
  #define USE_MODULE_SENSORS_BH1750
  
  #define USE_MODULE_CONTROLLER__LOUVOLITE_HUB_V2 // both rooms

  // #define USE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES_SK6812_FOR_ROOM_SENSORS
  #define USE_TEMPLATED_DEFAULT_LIGHTING_TEMPLATE_SK6812_FOR_ROOM_SENSORS__BOOT_STATE_OFF
   #define STRIP_SIZE_MAX 58

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"      
      #ifdef USE_MODULE_SENSORS_BME
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\"," //should be 27, missoldered, repair later
      #endif
      #if defined(USE_MODULE_CONTROLLER__LOUVOLITE_HUB) || defined(USE_MODULE_CONTROLLER__LOUVOLITE_HUB_V2)
      "\"27\":\"" D_GPIO_FUNCTION__RF_433MHZ_TX__CTR   "\","
      #endif  
      #ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED
      "\"14\":\"" D_GPIO_FUNCTION__RF_433MHZ_RX__CTR   "\","
      #endif  
      "\"18\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"33\":\"" D_GPIO_FUNCTION_SWT1_CTR   "\""
      #endif
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  // #define SETTINGS_SENSORS_MQTT_IFCHANGED_PERIOD_SECONDS 1
  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Ensuite"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Ensuite"
  
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
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)

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
 * Mode C (If "SwitchChanges" then "SetRelayToSwitchState")
 * 
 * */
#ifdef DEVICE_ENSUITE_CEILING_EXTRACTOR_FAN
  #define DEVICENAME_CTR          "ensuite_extractor_fan"
  #define DEVICENAME_FRIENDLY_CTR "Shelly1 Ensuite Extractor Fan"
  #define DEVICENAME_ROOMHINT_CTR "Ensuite"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID
   
  #define USE_MODULE_NETWORKS_MQTT

  #define USE_MODULE_CORE_RULES
    #define D_RULES_DELAY_LOAD_FROM_BOOT_TIME_SECOND 60
    #define ENABLE_DEVFEATURE_RULES_COMMAND_CAN_USE_TRIGGER_VALUE

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY1_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
    
  #define D_DEVICE_OUTPUT1_FRIENDLY_NAME_LONG "Fan"
  #define D_DEVICE_SWITCH1_FRIENDLY_NAME_LONG "Fan" 
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_OUTPUT1_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SWITCH1_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";
  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)  
  "{"
    // Switch0 ON = Relay0 Power ON for Timed seconds
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
        "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOn\\\":120}}\""
      "}"
    "},"
    // Switch0 OFF = Relay0 Reset to OFF
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":0"      // 2 meaning either low or high, 1 would be high only
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":0" // Started
      "}"
    "}"
  "}";


  // R"=====(
  // {
  //   "AddRule": {
  //     "Default":["Switch1ON->Relay1On2Hrs?Switch1OFF->Relay1Off"]
  //   }
  // }
  // )=====";

#endif


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** ROOM: Main bathroom ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

// Create the "above door light and mini set up" 


/**
 * @brief 
 * Cat5e Cable to Generic Room Sensor with BH1750, BME280 and HC-SR501 PIR
 * 
 * Ethernet (Using 6C wire)     Function       ESP32         Note        
 * 
 * w/o      (Black)             GND            GND           
 * o/w      (Red)               5V             5V            
 * w/g      ()                  PIR            5             
 * bl/w     ()              I2D            21            Preferred I2C Data pin
 * w/bl     ()        I2C            22            Preferred I2C Clock pin
 * g/w      ()            3V3            3V3           
 * w/br     (NC)                NC                           Perferred Serial2 ESP32 Rx In
 * br/w     (NC)                NC                           Perferred Serial2 ESP32 Tx Out
 * 
 * 
 * 4CORE to RGBW Strip
 * Red          5v
 * Black        Gnd
 * Green        RGBW SK6812 (56 leds)
 * White        NC
 *  
 **/
#ifdef DEVICE_BATHROOM_SENSOR
  #define DEVICENAME_CTR          "bathroomsensor"
  #define DEVICENAME_FRIENDLY_CTR "Bathroom Sensor"
  #define DEVICENAME_ROOMHINT_CTR "Bathroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
    
  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_MODULE_CORE_RULES
       
  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION
  #define USE_MODULE_SENSORS_BH1750

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define STRIP_SIZE_MAX 60
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    /********* Group: Needed to build ************************/
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
    /********* Group: Ready for full integration ************************/
    // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    /********* Group: Testing ************************/
    #define ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD
    #define ENABLE_DEVFEATURE_REMOVE_INIT_OUTSIDE_OF_PALETTE_CLASS
    #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
    #define ENABLE_DEVFEATURE_UNNEEDED_WLED_ONLY_PARAMETERS
    #define ENABLE_DEVFEATURE_ALWAYS_LOAD_PALETTE_WHEN_NOT_TRANSITIONING
    // #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
    /********* Group: Debug options only ************************/
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
    #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
    #define ENABLE_DEBUG_SERIAL
    // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    // #define ENABLE_LOG_LEVEL_DEBUG
    // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"      
      #ifdef USE_MODULE_SENSORS_BME
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      "\"19\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"5\":\"" D_GPIO_FUNCTION_SWT1_CTR   "\","
      #endif
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""  
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Bathroom"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Bathroom"
  
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
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)

  "}";


  #ifdef USE_MODULE_LIGHTS_INTERFACE
  #define USE_SK6812_METHOD_DEFAULT
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "HardwareType":"SK6812",
    "AnimationMode":"Effects",
    "ColourOrder":"grbw",
    "ColourPalette":"Rgbcct 01",
    "Effects": {
      "Function":0,
      "Intensity":50
    },
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "SegColour": {
      "Hue": 120,
      "Sat": 100,
      "SubType":3
    },
    "BrightnessRGB": 0,
    "BrightnessCCT": 0
  }
  )=====";
  #endif // USE_MODULE_LIGHTS_INTERFACE
  
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
****** ROOM: Landing ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

#ifdef DEVICE_SHELLYDIMMER_LANDING_CEILING
  #define DEVICENAME_CTR          "ceiling_landing_light"
  #define DEVICENAME_FRIENDLY_CTR "Shelly Dimmer Landing Room"
  #define DEVICENAME_ROOMHINT_CTR "Landing"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
  
  #define DISABLE_SERIAL_LOGGING
  
  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

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
/** NEW SYSTEM WITH FINAL WIRING
 * @description: 
 * 
 * Landing Panel
 *  ** PIR Detector for Landing
 *  ** Nextion Panel
 *  ** BME
 *  ** 4C RGB strip with upper/lower section
 * MasterBedroom
 *  ** BME 
 * Hotpress
 *  ** Button to be mounted on side of box
 *  ** Three LEDS to be mounted on box lid
 *  ** Mount box with rear hole and wood stand off
 *  ** Keep external 5V source (so it is away from clothes)
 *  ** OLED on side to show basic heating info as backup (not really needed)
 *  ** Water Tank RGB Strip to only show when button is pressed and only for 1 minute
 *  ** All water sensors
 *  ** Change relay box to the black one with glands. Use DB18 black wire for relay 5v trigger
 * 
 * Aim to close box and only open if wiring needs changing. 
 * Leave USB hanging out and below for easy reflash if needed via laptop
 * Clean and hover everything else out
 * 
 * 
 * 
 * 
 * "*" pin = confirmed soldering
 * Ethernet      Function       ESP32         Note        
 *                           
 * ** Blue (Landing - Nextion, BME, PIR) **************************************************************************
 * w/o           GND            GND           Black
 * o/w           5V             5V            Red
 * w/g           PIR            26            
 * bl/w          I2C            22           BME     Note: Bl/w on generic sensors is Data :: Inverted here
 * w/bl          I2D            21           BME     Note: w/Bl on generic sensors is Clock :: Inverted here
 * g/w           3V3            3V3           
 * w/br          NEXTION_RX     RX2           
 * br/w          NEXTION_TX     RX2           
 * ** Red (Masterbedroom - BME280) ************************************************************************** (BME280, LEDStrip 2 that is not yet coded. Will show temp when immersion is on for mums shower)
 * w/o           GND            GND           
 * o/w           5V             5V            
 * w/g                                     
 * bl/w          I2C            22         BME     Note: Bl/w on generic sensors is Data :: Inverted here
 * w/bl          I2D            21         BME     Note: w/Bl on generic sensors is Clock :: Inverted here       
 * g/w           3V3                        
 * w/br                         NC            
 * br/w                         NC
 * 
 * 
 * *********************************************************************************************************************************************************************************
 *                RGB Landing       RGB Tank         RGB Master       Relay Immersion        
 *                [+ 13 G]          [+ 14 G]         [+ 27 G]            [+ 26 G]
 *            __________________________________________________________________________
 *            |    3 Pin             3 Pin            3 Pin               3 Pin         |
 *            |                                                                         |
 *            |                                                                         |  
 *            |                                                                  2 Pin  |  2    Optional Indicator LED
 *        5V  | 2 Pin                                                                   |  G
 *        G   |                                                                         |  
 *            |                                                                  2 Pin  |  33   Button for Show Tank LEDs
 *            |                                                                         |  G
 *            |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                             |
 *            |  ~ Vin                                    ~                             |
 *            |  ~                                        ~                             |
 *            |  ~                                        ~                             |
 *            |  ~  Power                        ESP32    ~                             |
 *            |  ~                                        ~                             |
 *            |  ~                                        ~                             |
 *            |  ~ 3V3                                    ~                             |
 *            |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                             |
 *            |                                                                         |   +                
 *            |                                                                  3 Pin  |   23  Motion Stairs
 *            |                                                                         |   G 
 *            |                                                                         | 
 *            |                                                                         |   +
 *            |                                                                  3 Pin  |   19  DS18 Group 1
 *            |                                                                         |   G
 *            |                                                                         | 
 *            |                                                                         |   +
 *            |                                                                  3 Pin  |   18   DS18 Group 2
 *            |                                                                         |   G
 *            |                                                                         | 
 *            |    ~~~~~~~       ~~~~~~~                                                |
 *            |    ~  L  ~       ~  M  ~                                                |
 *            |    ~~~~~~~       ~~~~~~~                                                |
 *            |_________________________________________________________________________|
 *            
 * 
 * *********************************************************************************************************************************************************************************
*/
#ifdef DEVICE_IMMERSION_CONTROL_PANEL // New 2023 system unified
  #define DEVICENAME_CTR          "immersion_controller"
  #define DEVICENAME_FRIENDLY_CTR "Immersion Heater Panel"
  #define DEVICENAME_ROOMHINT_CTR "Landing"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  // ***************************************************************************************************************************************

  #define ENABLE_GROUPFEATURE__DX18S20_SENSORS
  #define ENABLE_GROUPFEATURE__BME_SENSORS
  #define ENABLE_GROUPFEATURE__MOTION_SENSORS
  #define ENABLE_GROUPFEATURE__CASE_BUTTONS
  // #define ENABLE_GROUPFEATURE__LIGHTS_PANEL_UPDOWNLIGHTS
  // #define ENABLE_GROUPFEATURE__LIGHTS_TANK_GRADIENT  
  // #define ENABLE_GROUPFEATURE__NEXTION_TOUCH_DISPLAY
  #define ENABLE_GROUPFEATURE__OLED_STATUS_DISPLAY  
  #define ENABLE_GROUPFEATURE__HVAC_IMMERSION

  // ***************************************************************************************************************************************

  #ifdef ENABLE_GROUPFEATURE__DX18S20_SENSORS
    #define USE_MODULE_SENSORS_INTERFACE
    #define USE_MODULE_SENSORS__DS18X20_ESP32_2023
      #define DS18X20_MAX_SENSORS 20
      #define ENABLE_DEVFEATURE_DS18B20_SEARCHING_SENSOR_LOCATION_WITH_ADDRESS_TEMP_SPLASH
      #define DEVICENAMEBUFFER_NAME_BUFFER_LENGTH 1000
      #define DEVICENAMEBUFFER_NAME_INDEX_LENGTH  100
      #define DB18_SENSOR_MAX                     15
      #define DATA_BUFFER_PAYLOAD_MAX_LENGTH      3000 //needed for db sensosrs, but currently causes crash in lighting
      #define MQTT_MAX_PACKET_SIZE                3000
      #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #endif // ENABLE_GROUPFEATURE__DX18S20_SENSORS

  #ifdef ENABLE_GROUPFEATURE__BME_SENSORS
    #define USE_MODULE_SENSORS_INTERFACE
    #define USE_MODULE_SENSORS_BME
    #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #endif // ENABLE_GROUPFEATURE__BME_SENSORS      

  #ifdef ENABLE_GROUPFEATURE__MOTION_SENSORS
    #define USE_MODULE_CORE_RULES
    #define USE_MODULE_SENSORS_SWITCHES
    #define USE_MODULE_SENSORS_MOTION
  #endif // ENABLE_GROUPFEATURE__MOTION_SENSORS

  #ifdef ENABLE_GROUPFEATURE__CASE_BUTTONS
    #define USE_MODULE_SENSORS_SWITCHES
    #define USE_MODULE_SENSORS_BUTTONS
    //   #define ENABLE_DEVFEATURE_PHASEOUT_CLEARING_EVENT
    //   #define ENABLE_DEVFEATURE_BUTTON_SET_FLAG_BUTTON_SINGLE 0 // allow multipress = false
  #endif // ENABLE_GROUPFEATURE__CASE_BUTTONS

  #ifdef ENABLE_GROUPFEATURE__LIGHTS_PANEL_UPDOWNLIGHTS
    #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES_SK6812_FOR_ROOM_SENSORS
    #define USE_TEMPLATED_DEFAULT_LIGHTING_TEMPLATE_SK6812_FOR_ROOM_SENSORS__BOOT_STATE_OFF
      #define STRIP_SIZE_MAX 40
  #endif // ENABLE_GROUPFEATURE__LIGHTS_PANEL_UPDOWNLIGHTS

  #ifdef ENABLE_GROUPFEATURE__LIGHTS_TANK_GRADIENT
    // #define USE_RGB_OUT_TANK
    #define USE_RGB_OUT_LANDING_PANEL
  #endif // ENABLE_GROUPFEATURE__LIGHTS_TANK_GRADIENT

  #ifdef ENABLE_GROUPFEATURE__NEXTION_TOUCH_DISPLAY
    #define USE_MODULE_DISPLAYS_INTERFACE
    #define USE_MODULE_DISPLAYS_NEXTION
      #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
    #define NEXTION_DEFAULT_PAGE_NUMBER 2  
      #define ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT
  #endif // ENABLE_GROUPFEATURE__NEXTION_TOUCH_DISPLAY

  #ifdef ENABLE_GROUPFEATURE__OLED_STATUS_DISPLAY  
    #define USE_MODULE_DISPLAYS_INTERFACE
    #define USE_MODULE_DISPLAYS_OLED_SH1106
      #define SHOW_SPLASH
    // 4x10
    // Uptime so I know its working by glance
    // Relay Minutes On
    // Shower Temp /     Bath Temp
  #endif // ENABLE_GROUPFEATURE__OLED_STATUS_DISPLAY

  #ifdef ENABLE_GROUPFEATURE__HVAC_IMMERSION
    #define USE_MODULE_DRIVERS_INTERFACE
    #define USE_MODULE_DRIVERS_RELAY
    #define USE_MODULE_DRIVERS_LEDS
      #define MAX_RELAYS 1
    // #define USE_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR
      // #define ENABLE_DEVFEATURE_SENSORS_INTERFACE_SHOW_TEMPERATURE_AS_COLOUR  //should this be a "controller", or via interface "getTemp convert to heatmap"
      // #define SETTINGS_SENSORS_MQTT_IFCHANGED_PERIOD_SECONDS 60  
      // #define USE_DEVFEATURE_SENSOR_COLOURS_TOP_TO_BOTTOM
    #define USE_MODULE_CONTROLLER_HVAC
      #define ENABLE_DEVFEATURE_CONTROLLER_HVAC_NEW_HVAC_TIMEON
      #define HEATING_DEVICE_MAX 1
    #define USE_MODULE_CONTROLLER_CUSTOM__IMMERSION_PANEL
  #endif // ENABLE_GROUPFEATURE__HVAC_IMMERSION  
     
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"26\":\""  D_GPIO_FUNCTION_REL1_CTR    "\","   // Immersion = Also add additonal LED with relay pin
      #endif
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_DISPLAYS_OLED_SH1106)
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
      "\"18\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR  "\"," // Group 1 = Basic Set, use just these until device is stable
      "\"19\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR  "\"," // Group 2 = Detailed, use these only after stress testing with 3 pins for sensors with rewrite. Read datasheet.
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"32\":\"" D_GPIO_FUNCTION_SWT1_CTR "\","       // Stairs
      "\"4\":\""  D_GPIO_FUNCTION_SWT2_CTR "\","       // Landing
      "\"25\":\"" D_GPIO_FUNCTION_SWT3_CTR "\","       // Hotpress (negating need of button? or use non-momentary switch to enable/disable it)
      #endif 
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
        // "\"26\":\"" D_GPIO_FUNCTION_RGB_DATA1_CTR  "\"," // Orange - Immersional Relay
        // "\"27\":\"" D_GPIO_FUNCTION_RGB_DATA1_CTR  "\"," // Green - Immersion Tank
        // "\"14\":\"" D_GPIO_FUNCTION_RGB_DATA1_CTR  "\"," // Blue - Landing Panel
        // "\"13\":\"" D_GPIO_FUNCTION_RGB_DATA1_CTR  "\"," // Brown - Immersion Relay (Label wrong, )
        #ifdef USE_RGB_OUT_LANDING_PANEL
        "\"14\":\"" D_GPIO_FUNCTION_RGB_DATA1_CTR  "\"," // Blue - Landing Panel
        #endif
        #ifdef USE_RGB_OUT_TANK
        "\"27\":\"" D_GPIO_FUNCTION_RGB_DATA1_CTR  "\"," // Green - Immersion Tank
        #endif      
        #ifdef ENABLE_DEVFEATURE_MULTIPLE_PIXEL_PINS
        "\"14\":\"" D_GPIO_FUNCTION_RGB_DATA2_CTR  "\","
        "\"27\":\"" D_GPIO_FUNCTION_RGB_DATA3_CTR  "\","
        #endif
      #endif
      #ifdef USE_MODULE_DISPLAYS_NEXTION
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
      #endif
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"33\":\"" D_GPIO_FUNCTION_KEY1_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_CTR  "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_LEDS
      "\"12\":\""  D_GPIO_FUNCTION_LED2_INV_CTR "\"," 
      "\"23\":\""  D_GPIO_FUNCTION_LED3_INV_CTR "\"," 
      #endif 
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""  // Also optional physical LED to be made external to box (buy one of those drill through ones!)
      // 5  // i2c oled needs to be another bus because of BME? or, what if it shared!!
      // 15
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  
  /**
   * @brief Group 1: Primary HVAC Feedback Sensors 
   */
  #define D_DEVICE_SENSOR_DB18S20_01_NAME        "Immersion_Heater"
  #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,255,136,105,53,22,4,114]"
  #define D_DEVICE_SENSOR_DB18S20_02_NAME        "Tank_Top_90" 
  #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,255,162,167,53,22,4,27]"
  #define D_DEVICE_SENSOR_DB18S20_03_NAME        "Tank_Middle_50"
  #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,255,219,93,53,22,4,239]"
  #define D_DEVICE_SENSOR_DB18S20_04_NAME        "Tank_Bottom_25"
  #define D_DEVICE_SENSOR_DB18S20_04_ADDRESS     "[40,255,50,176,193,23,4,197]"
  #define D_DEVICE_SENSOR_DB18S20_05_NAME        "Tank_Out" // Rear Pipe
  #define D_DEVICE_SENSOR_DB18S20_05_ADDRESS     "[40,255,216,108,53,22,4,102]"
  
  /**
   * @brief Group 2: Additonal Sensors
   * */
  #define D_DEVICE_SENSOR_DB18S20_10_NAME        "TankPosition00"
  #define D_DEVICE_SENSOR_DB18S20_10_ADDRESS     "[40,0,118,128,59,71,5,227]"
  #define D_DEVICE_SENSOR_DB18S20_11_NAME        "TankPosition20"
  #define D_DEVICE_SENSOR_DB18S20_11_ADDRESS     "[40,0,114,20,59,71,5,19]"
  #define D_DEVICE_SENSOR_DB18S20_12_NAME        "TankPosition40"
  #define D_DEVICE_SENSOR_DB18S20_12_ADDRESS     "[40,0,66,109,59,71,5,172]"
  #define D_DEVICE_SENSOR_DB18S20_13_NAME        "TankPosition60"
  #define D_DEVICE_SENSOR_DB18S20_13_ADDRESS     "[40,0,108,65,59,71,4,202]"
  #define D_DEVICE_SENSOR_DB18S20_14_NAME        "TankPosition80"
  #define D_DEVICE_SENSOR_DB18S20_14_ADDRESS     "[40,0,83,19,59,71,6,66]"
  #define D_DEVICE_SENSOR_DB18S20_15_NAME        "TankPosition100"
  #define D_DEVICE_SENSOR_DB18S20_15_ADDRESS     "[40,0,32,23,59,71,5,141]"
  #define D_DEVICE_SENSOR_DB18S20_16_NAME        "BoilerLoopTop"
  #define D_DEVICE_SENSOR_DB18S20_16_ADDRESS     "[40,0,40,61,59,71,4,134]"
  #define D_DEVICE_SENSOR_DB18S20_17_NAME        "BoilerLoopBottom"
  #define D_DEVICE_SENSOR_DB18S20_17_ADDRESS     "[40,0,66,140,59,71,6,136]"
  #define D_DEVICE_SENSOR_DB18S20_18_NAME        "ImmersionFeedIn"
  #define D_DEVICE_SENSOR_DB18S20_18_ADDRESS     "[40,0,95,50,59,71,5,126]"
  #define D_DEVICE_SENSOR_DB18S20_19_NAME        "FeedRed"
  #define D_DEVICE_SENSOR_DB18S20_19_ADDRESS     "[40,0,149,87,59,71,5,240]"

  #define D_DEVICE_DRIVER_RELAY_01_NAME "Immersion"
  
  #define D_DEVICE_SENSOR_MOTION_STAIRS_FRIENDLY_NAME_LONG "Stairs"
  #define D_DEVICE_SENSOR_MOTION_LANDING_FRIENDLY_NAME_LONG "Landing"
  #define D_DEVICE_SENSOR_BME_LANDING_NAME       "Landing"
  #define D_DEVICE_SENSOR_BME_MASTERBEDROOM_NAME "Master Bedroom"

  #define D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "Immersion"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_DRIVER_RELAY_01_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_STAIRS_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SENSOR_MOTION_LANDING_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_STAIRS_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SENSOR_MOTION_LANDING_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_BME_MASTERBEDROOM_NAME "\"," // 0x76
        "\"" D_DEVICE_SENSOR_BME_LANDING_NAME  "\""       // 0x77
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        // Group 1
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_05_NAME "\","
        // Group 2
        "\"" D_DEVICE_SENSOR_DB18S20_10_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_11_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_12_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_13_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_14_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_15_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_16_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_17_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_18_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_19_NAME "\""      
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":{" 
        // Group 1
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\":" D_DEVICE_SENSOR_DB18S20_03_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\":" D_DEVICE_SENSOR_DB18S20_04_ADDRESS ","      
        "\"" D_DEVICE_SENSOR_DB18S20_05_NAME "\":" D_DEVICE_SENSOR_DB18S20_05_ADDRESS ","  
        // Group 2
        "\"" D_DEVICE_SENSOR_DB18S20_10_NAME "\":" D_DEVICE_SENSOR_DB18S20_10_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_11_NAME "\":" D_DEVICE_SENSOR_DB18S20_11_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_12_NAME "\":" D_DEVICE_SENSOR_DB18S20_12_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_13_NAME "\":" D_DEVICE_SENSOR_DB18S20_13_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_14_NAME "\":" D_DEVICE_SENSOR_DB18S20_14_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_15_NAME "\":" D_DEVICE_SENSOR_DB18S20_15_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_16_NAME "\":" D_DEVICE_SENSOR_DB18S20_16_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_17_NAME "\":" D_DEVICE_SENSOR_DB18S20_17_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_18_NAME "\":" D_DEVICE_SENSOR_DB18S20_18_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_19_NAME "\":" D_DEVICE_SENSOR_DB18S20_19_ADDRESS ""
      "}"  
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_01_NAME "\"," // Also an array to match heating/cooling
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "}"
      "]"
    "}"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":600}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";

  // #ifdef USE_RGB_OUT_LANDING_PANEL
  // #ifdef USE_MODULE_LIGHTS_INTERFACE
  // #define USE_SK6812_METHOD_DEFAULT
  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // R"=====(
  // {
  //   "HardwareType":"SK6812",
  //   "AnimationMode":"Effects",
  //   "ColourOrder":"grbw",
  //   "ColourPalette":"Christmas 01",
  //   "Effects": {
  //     "Function":1,
  //     "Intensity":50
  //   },
  //   "Transition": {
  //     "TimeMs": 0,
  //     "RateMs": 1000
  //   },
  //   "SegColour": {
  //     "Hue": 120,
  //     "Sat": 100,
  //     "SubType":3
  //   },
  //   "BrightnessRGB_255": 255,
  //   "BrightnessCCT_255": 255
  // }
  // )=====";
  // #endif // USE_MODULE_LIGHTS_INTERFACE
  // #endif // USE_RGB_OUT_LANDING_PANEL

  // #ifdef USE_RGB_OUT_TANK
  // #ifdef USE_MODULE_LIGHTS_INTERFACE
  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // R"=====(
  // {
  //   "HardwareType":"WS28XX",
  //   "AnimationMode":"Effects",
  //   "ColourOrder":"grb",
  //   "ColourPalette":"Christmas 01",
  //   "Effects": {
  //     "Function":1,
  //     "Intensity":50
  //   },
  //   "Transition": {
  //     "TimeMs": 0,
  //     "RateMs": 1000
  //   },
  //   "SegColour": {
  //     "Hue": 120,
  //     "Sat": 100,
  //     "SubType":3
  //   },
  //   "BrightnessRGB_255": 255,
  //   "BrightnessCCT_255": 1
  // }
  // )=====";
  // #endif // USE_MODULE_LIGHTS_INTERFACE
  // #endif // USE_RGB_OUT_TANK
    
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
    "},"
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":1,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":1," 
        "\"State\":\"Follow\""
      "}"
    "}"
  "}";

#endif


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** ROOM: Bedroom (Long term only ie can remain when I leave home) ****************************************************************************************************************************************************
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
  #define USE_MODULE_DRIVERS_BUZZER_BASIC_BASIC
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


#ifdef DEVICE_SHELLYDIMMER_BEDROOM_CEILING
  #define DEVICENAME_CTR          "dimmer_bedroom_ceiling"
  #define DEVICENAME_FRIENDLY_CTR "Shelly Dimmer Spare Room"
  #define DEVICENAME_ROOMHINT_CTR "Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED       192,168,1,70
  
  #define DISABLE_SERIAL_LOGGING
  
  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_MODULE_CORE_RULES
    
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_SHELLY_DIMMER    
    #define ENABLE_DEVFEATURE_SHELLY_DIMMER_FORCED_BRIGHTNESS_TO_REQUESTED

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
        "\"Value\":35"
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

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_BH1750
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION
  // #define USE_MODULE_SENSORS_DOOR

  #define USE_MODULE_CONTROLLER__LOUVOLITE_HUB_V2 // both rooms
    
  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES_SK6812_FOR_ROOM_SENSORS
  #define USE_TEMPLATED_DEFAULT_LIGHTING_TEMPLATE_SK6812_FOR_ROOM_SENSORS__BOOT_STATE_OFF
    #define STRIP_SIZE_MAX 60

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
      // #ifdef USE_MODULE_SENSORS_DOOR
      // "\"18\":\"" D_GPIO_FUNCTION_DOOR_OPEN_CTR     "\","
      // "\"19\":\"" D_GPIO_FUNCTION_DOOR_LOCK_CTR     "\","
      "\"18\":\""  D_GPIO_FUNCTION_SWT2_INV_CTR "\","
      "\"19\":\""  D_GPIO_FUNCTION_SWT3_INV_CTR "\","
      // #endif
      #if defined(USE_MODULE_CONTROLLER__LOUVOLITE_HUB) || defined(USE_MODULE_CONTROLLER__LOUVOLITE_HUB_V2)
      "\"22\":\"" D_GPIO_FUNCTION__RF_433MHZ_TX__CTR   "\","
      #endif  
      "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      "\"2\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "Bedroom"
  #define D_DEVICE_SENSOR_MOTION1_FRIENDLY_NAME_LONG "Door Closed"
  #define D_DEVICE_SENSOR_MOTION2_FRIENDLY_NAME_LONG "Door Lock"
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
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SENSOR_MOTION1_FRIENDLY_NAME_LONG "1m\""
        "\"" D_DEVICE_SENSOR_MOTION2_FRIENDLY_NAME_LONG "2m\","
      "],"  
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SENSOR_MOTION1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SENSOR_MOTION2_FRIENDLY_NAME_LONG "\""
      "],"  
      "\"" D_MODULE_SENSORS_DOOR_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "1d\","
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "2d\""
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
    "},"
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":1,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":1," 
        "\"State\":\"Follow\""
      "}"
    "},"
    "\"Rule2\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":2,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":2," 
        "\"State\":\"Follow\""
      "}"
    "}"
  "}";


#endif

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** ROOM: Roaming devices ****************************************************************************************************************************************************
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
  #define USE_MODULE_DRIVERS_BUZZER_BASIC_BASIC
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
  #define USE_MODULE_DRIVERS_BUZZER_BASIC_BASIC
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
  #define USE_MODULE_DRIVERS_BUZZER_BASIC_BASIC
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



#endif // _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_HOME_LONGTERM_TEMPLATES_H
