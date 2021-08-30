#ifndef MFIRMWARECUSTOM_SECRET_HOME_H
#define MFIRMWARECUSTOM_SECRET_HOME_H

// #ifdef USER_MICHAEL

/*********************************************************************************************\
 * Personal configs for installed home devices as of 2021
\*********************************************************************************************/

  #define USE_MODULE_NETWORKS_MQTT
// #define DEVICE_FORCED_TO_BE_TESTER
// #define ENABLE_TESTUSER
// #define DISABLE_WEBSERVER
#define FORCE_TEMPLATE_LOADING
#define USE_MODULE_CORE_RULES

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/Languages/mLanguageDefault.h"
#include "2_CoreSystem/HardwareTemplates/mHardwareTemplates.h"

// //--------------------------------[Enable Device]-------------------------------------
/**
 * Generic devices ie those configured via `ini` configs
 */
// #define DEVICE_SOCKET_NUMBERED

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
  - consumerunit = 8 pzem, bme
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
// #define DEVICE_HEATING
// #define DEVICE_HEATING_ESP32
// #define DEVICE_DOORBELLWALLCHIME

/**
Outside
  - CeilingLight
**/
// #define DEVICE_SIDEDOORLIGHT
// #define DEVICE_GAZEBO_CONTROLLER
// #define DEVICE_GAZEBO_SENSOR
// #define DEVICE_H801_RGBGAZEBO

/**
Garage
  - CeilingLight
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


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Generic devices ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


#ifdef DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_CTR          "socket_number_" STR2(DEVICENAME_SOCKET_NUMBER_CTR)
  #define DEVICENAME_FRIENDLY_CTR "Socket Number " STR2(DEVICENAME_SOCKET_NUMBER_CTR)
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

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


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Kitchen ****************************************************************************************************************************************************
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
  
  #define FORCE_TEMPLATE_LOADING
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
  
  // #define FORCE_TEMPLATE_LOADING
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
  
  #define FORCE_TEMPLATE_LOADING
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
        "\"JsonCommands\":\"{\\\"MQTTSend\\\":{\\\"Topic\\\":\\\"socket_number_11/set/relays\\\",\\\"Payload\\\":\\\"{~PowerName~:0,~PowerState~:2}\\\"}}\""
      "}"
    "}"
  "}";

#endif


#ifdef DEVICE_KITCHENLIGHT4
  #define DEVICENAME_CTR          "kitchenlight4"
  #define DEVICENAME_FRIENDLY_CTR "Kitchen Cupboard Lights"
  
  #define FORCE_TEMPLATE_LOADING
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


#ifdef DEVICE_RGBSHELF
  #define DEVICENAME_CTR          "rgbshelf"
  #define DEVICENAME_FRIENDLY_CTR "Shelf Lights"
    
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  
  #define DISABLE_WEBSERVER

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

  #define STRIP_SIZE_MAX 33

 #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":\"" "Slow Glow" "\""
    "},"
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":10000,"
      "\"" D_JSON_RATE_MS "\":1000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\""
    "},"
    "\"TimeMs\":30000,"
    "\"ColourPalette\":\"Shelf Hearts\","
    "\"BrightnessRGB\":70"
  "}";

#endif

#ifdef DEVICE_RGBCRYSTAL3
  #define DEVICENAME_CTR          "rgbcrystal3"
  #define DEVICENAME_FRIENDLY_CTR "Glass Box Lights"
  
  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 11
   
  // #define ENABLE_PIXEL_FUNCTION_EFFECTS
  // #define DISABLE_WEBSERVER

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
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
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
    // "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":1" //slow glow
    "},"
    "\"Transition\":{\"Order\":\"InOrder\",\"PixelUpdatePerc\":2,\"RateMs\":10000},"
    "\"TimeMs\":5000,"
    "\"ColourPalette\":\"Shelf Hearts\"," //c12    43 is the colours for this christmas
    "\"BrightnessRGB\":100"
  "}";

#endif


#ifdef DEVICE_RGBCOOKER
  #define DEVICENAME_CTR          "rgbcooker"
  #define DEVICENAME_FRIENDLY_CTR "RGB Cooker H801"
      
  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  #define FORCE_TEMPLATE_LOADING
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

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2
     
  #define USE_MODULE_SENSORS_MOTION

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
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D6\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR     "\","
      #endif      
      "\"0\":\"" D_GPIO_FUNCTION_LED1_CTR "\""
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
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif


#ifdef DEVICE_RGBFRIDGE
  #define DEVICENAME_CTR          "rgbfridge"
  #define DEVICENAME_FRIENDLY_CTR "RGB Fridge H801"

  #define FORCE_TEMPLATE_LOADING
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

  // #define FORCE_TEMPLATE_LOADING
  // //#define SETTINGS_HOLDER 2 //maintain other settings (bootcount)
  
  // #define USE_SERIAL_ALTERNATE_TX
   
  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  // #define USE_MODULE_LIGHTS_PWM
  
  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  #define FORCE_TEMPLATE_LOADING
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

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2
     
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
      DEFINE_APP_SVALUE("D6",D_GPIO_FUNCTION_SWT1_INV_CTR)
      #endif
      #ifdef USE_MODULE_SENSORS_DOOR
      DEFINE_APP_SVALUE("D7",D_GPIO_FUNCTION_DOOR_OPEN_CTR)
      #endif
      DEFINE_END_SVALUE("D4",D_GPIO_FUNCTION_LED1_CTR)
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
        "\"State\":2" // Started
      "}"
    "},"
    // Switch0 HIGH = Motion0 Event Started, ie report as motion with motion name
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_DOOR_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":1" // FOLLOW, ie command follows trigger, or follow_inv, ie command is inverted to source
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":1,"     // Index of motion to be used for name eg garage, motion, then time from when mqtt is sent
        "\"State\":2" // Started
      "}"
    "}"
  "}";



#endif


#ifdef DEVICE_RGBCRYSTAL1
  #define DEVICENAME_CTR            "rgbcrystal1"
  #define DEVICENAME_FRIENDLY_CTR   "Crystal Light Cylinder"
  
  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 11
   
  // #define ENABLE_PIXEL_FUNCTION_EFFECTS
  // #define DISABLE_WEBSERVER

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
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
  #define STRIP_REPEAT_OUTPUT_MAX 150

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
    // "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":1" //slow glow
    "},"
    "\"Transition\":{\"Order\":\"InOrder\",\"PixelUpdatePerc\":2,\"RateMs\":10000},"
    "\"TimeMs\":5000,"
    "\"ColourPalette\":\"Shelf Hearts\"," //c12    43 is the colours for this christmas
    "\"BrightnessRGB\":100"
  "}";



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
    
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BME

  #define USE_MODULE_ENERGY_INTERFACE
  #define USE_MODULE_ENERGY_PZEM004T_V3

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
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_CLIMATE "Downstairs Toilet"
  #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "16"
  #define D_DEVICE_SENSOR_PZEM004T_1_ADDRESS "17"
  #define D_DEVICE_SENSOR_PZEM004T_2_ADDRESS "18"
  #define D_DEVICE_SENSOR_PZEM004T_3_ADDRESS "19"
  #define D_DEVICE_SENSOR_PZEM004T_4_ADDRESS "20"
  #define D_DEVICE_SENSOR_PZEM004T_5_ADDRESS "21"
  #define D_DEVICE_SENSOR_PZEM004T_6_ADDRESS "22"
  #define D_DEVICE_SENSOR_PZEM004T_7_ADDRESS "23"
  
  #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "Mains"
  #define D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "Cooker"
  #define D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "Tumble Dryer"
  #define D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR "Washing Machine"
  #define D_DRIVER_ENERGY_4_FRIENDLY_NAME_CTR "Dish Washer"
  #define D_DRIVER_ENERGY_5_FRIENDLY_NAME_CTR "Shower"
  #define D_DRIVER_ENERGY_6_FRIENDLY_NAME_CTR "Sockets"
  #define D_DRIVER_ENERGY_7_FRIENDLY_NAME_CTR "Kitchen Sockets"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_4_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_5_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_6_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_7_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE "\""
      "],"
      "\"" D_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_4_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_5_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_6_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_7_FRIENDLY_NAME_CTR "\""
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
        D_DEVICE_SENSOR_PZEM004T_7_ADDRESS ""
      "]"  
    "},"
    "\"" D_JSON_ENERGY "\":{"
        "\"DeviceCount\":8"    
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
#ifdef DEVICE_LIVINGROOMSENSOR
  #define DEVICENAME_CTR          "livingroomsensor"
  #define DEVICENAME_FRIENDLY_CTR "Living Room Sensor"
    
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_MOTION
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_BME

  #define USE_MODULE_CORE_RULES

  #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "DriveFront"
  #define D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "LivingRoom"
  // #define D_DEVICE_SENSOR_MOTION_2_FRIENDLY_NAME_LONG "LivingRadar" // Test device

  #define D_DEVICE_SENSOR_DRIVEWAY_ULTRSONIC_FRIENDLY_NAME_LONG "DriveFront"

  // #define USE_MODULE_SENSORS_ANALOG

  // #define USE_MODULE_SENSORS_ULTRASONICS
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
        // "\"" D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "\","
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
  #define ENABLE_PIXEL_FUNCTION_EFFECTS
  
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


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Hallway ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

#ifdef DEVICE_RADIATORFAN
  /* Single Relay controls TWO 12V DC fans to improve heating (1 D_Out)
   * Measure PWM speed of motors (2 D_in) (Needs level shifter from 12v to 3.3v)
   * Motion sensor (1 D_in)
   * Two DS18b20 to measure inside/outside of vent area (1 D_io)
   * One DHT22 for climate sensing -- towards door (1 D_io)
   * One BME280 for improved climate sensing -- towards hallway (1 I2C) */
  #define DEVICENAME_CTR          "radiatorfan"
  #define DEVICENAME_FRIENDLY_CTR "Radiator Fan"
  
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION
  #define USE_MODULE_SENSORS_DS18B20
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
      #ifdef USE_MODULE_SENSORS_DS18B20
      "\"D5\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR     "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"D6\":\"" D_GPIO_FUNCTION_REL1_INV_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D7\":\"" D_GPIO_FUNCTION_SWT1_INV_CTR     "\","
      #endif 
      "\"D4\":\""  D_GPIO_FUNCTION_LED1_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Fan"
  #define MAX_RELAYS 1

  #define D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "Top Vent"
  #define D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "Inside Vent"
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
        "\"" D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "\""
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
        "[40,170,221,101,29,19,02,113]"
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


#endif

#ifdef DEVICE_HEATING
  #define DEVICENAME_CTR          "heating"
  #define DEVICENAME_FRIENDLY_CTR "HVAC Heating Gen 2"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1 //maintain other settings (bootcount)
   
  // #define ENABLE_BUG_TRACING
  //#define ENABLE_MQTT_DEBUG_MESSAGES

  #define USE_INTERNAL_HOME_APS_ONLY

  //#define FORCE_DEVICENAME_CLEAR_ON_BOOT
  // #define ENABLE_HVAC_DEBUG_TIMES
  #define DISABLE_WEBSERVER

  // #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  #define EMABLE_DEVFEATURE_HARDWAREPINS_CLEANED_UP

  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_DHT
  #define USE_MODULE_SENSORS_DS18B20
  #define USE_MODULE_SENSORS_REMOTE_DEVICE
  
  #define REMOTE_SENSOR_1_MQTT_TOPIC "masterbedroomsensor/status/bme/+/sensors"
  #define REMOTE_SENSOR_JSON_NAME "MasterBedroom"

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_CONTROLLER_HVAC
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      // "\"D0\":\"" D_GPIO_FUNCTION_DHT22_1_CTR   "\","
      "\"D1\":\"" D_GPIO_FUNCTION_REL2_INV_CTR  "\","
      "\"D2\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      "\"D3\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      "\"D5\":\"" D_GPIO_FUNCTION_REL4_INV_CTR  "\","
      "\"D6\":\"" D_GPIO_FUNCTION_REL3_CTR      "\","
      "\"D7\":\"" D_GPIO_FUNCTION_DHT22_1_CTR   "\","
      "\"D4\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
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

  #define D_DEVICE_SENSOR_DHT_0_NAME "Downstairs_DHT"
  #define D_DEVICE_SENSOR_DHT_1_NAME "Downstairs_DHT"

  #define D_DEVICE_SENSOR_REMOTE_BME_BEDROOM_NAME "RemoteBedroomBME"

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
      "\"" D_MODULE_SENSORS_REMOTE_DEVICE_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_REMOTE_BME_BEDROOM_NAME "\""
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
        "\"" D_DEVICE_SENSOR_REMOTE_BME_BEDROOM_NAME "\","
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


#ifdef DEVICE_HEATING_ESP32
  #define DEVICENAME_CTR          "heating"
  #define DEVICENAME_FRIENDLY_CTR "HVAC Heating Gen 2"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1 //maintain other settings (bootcount)
   
  // #define ENABLE_BUG_TRACING
  //#define ENABLE_MQTT_DEBUG_MESSAGES
  #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  #define EMABLE_DEVFEATURE_HARDWAREPINS_CLEANED_UP

  #define USE_INTERNAL_HOME_APS_ONLY

  #define DISABLE_WEBSERVER
  #define ESP32

  // #define USE_MODULE_CONTROLLER_HVAC
  
  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_DHT
  #define USE_MODULE_SENSORS_DS18B20
  
  #define ENABLE_DEVFEATURE_ESP32_FORCED_DB18S20_GPIO1_SENSOR_COUNT 7
  // #define ENABLE_DEVFEATURE_ESP32_FORCED_DB18S20_GPIO2_SENSOR_COUNT 5

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"13\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      "\"12\":\"" D_GPIO_FUNCTION_REL2_INV_CTR  "\","
      "\"14\":\"" D_GPIO_FUNCTION_REL3_CTR      "\","
      "\"27\":\"" D_GPIO_FUNCTION_REL4_INV_CTR  "\","
      "\"25\":\"" D_GPIO_FUNCTION_DHT22_1_CTR   "\","
      "\"26\":\"" D_GPIO_FUNCTION_DHT22_2_CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      // "\"23\":\""  D_GPIO_FUNCTION_DS18X20_2_CTR "\","
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
      // "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\"," //future bme
      // "\"23\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\"" //builtin led
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



#ifdef DEVICE_DOORBELLWALLCHIME
  #define DEVICENAME_CTR          "doorbellwallchime"
  #define DEVICENAME_FRIENDLY_CTR "Doorbell Wall Chime"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define DISABLE_WEBSERVER
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  #define USE_SK6812_METHOD_DEFAULT
  #define STRIP_SIZE_MAX 12 // 8 front, 4 top
  
  #define USE_MODULE_CONTROLLER_DOORCHIME
  #define DOORALERT_PAYLOAD_CTR   "frontdoorbell"

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_CONTROLLER_DOORCHIME
      "\"D6\":\"" D_GPIO_FUNCTION_CHIME_INPUT_CTR     "\","
      "\"D7\":\"" D_GPIO_FUNCTION_CHIME_RINGER_CTR     "\","
      #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

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
      "\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\""
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":8"
    "},"
    "\"CCT_Temp\": 152,"
    "\"Hue\":25,"
    "\"Sat\":100,"
    "\"ColourPalette\":10,"
    "\"BrightnessCCT\":0,"
    "\"BrightnessRGB\":100"
  "}";
  

  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "FrontDoorbell"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_CONTROLLER_DOORBELL_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
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

  #define FORCE_TEMPLATE_LOADING
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
    "\"RelayEnabled0\":{\"Enabled\":1,\"OnTime\":\"00D20:00:00\",\"OffTime\":\"00D05:00:00\"}"
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
        "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOn\\\":10}}\""
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










#ifdef DEVICE_GAZEBO_CONTROLLER
  #define DEVICENAME_CTR          "gazebo_controller"
  #define DEVICENAME_FRIENDLY_CTR "Gazebo Controller"

  //to become a sonoff 4ch (non pro)
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
  #define MAX_RELAYS 4
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"D5\":\"" D_GPIO_FUNCTION_REL1_INV_CTR "\","
      "\"D6\":\"" D_GPIO_FUNCTION_REL2_INV_CTR "\","
      "\"D7\":\"" D_GPIO_FUNCTION_REL3_INV_CTR "\","
      "\"D0\":\"" D_GPIO_FUNCTION_REL4_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Red"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Yellow"
  #define D_DEVICE_RELAY_2_FRIENDLY_NAME_LONG "Blue"
  #define D_DEVICE_RELAY_3_FRIENDLY_NAME_LONG "White"

  #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Gazebo"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_RELAY_2_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_RELAY_3_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif


// gazebosensor for motion, light, temperature... these may all become esp32 with wired POE
#ifdef DEVICE_GAZEBO_SENSOR
  #define DEVICENAME_CTR          "gazebosensor"
  #define DEVICENAME_FRIENDLY_CTR "Gazebo Sensor"
  
  #define USE_MODULE_SENSORS_INTERFACE
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
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750)
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D5\":\"" D_GPIO_FUNCTION_SWT1_INV_CTR "\""
      #endif
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" "Outside" "\""
      "],"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" "Gazebo" "\""
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

#endif



#ifdef DEVICE_H801_RGBGAZEBO
  #define DEVICENAME_CTR          "h801_rgbgazebo"
  #define DEVICENAME_FRIENDLY_CTR "H801 RGB Gazebo RGBCCT Strip"

  /**
   * 24W/meter
   * Approx 5.5m used, 24*5.5 = 132W so 132/12 = 11Amps at 12V
   * 20A 12V should be enough
   * */

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
  

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\"" "RGBCCT_PWM" "\","
    "\"" D_JSON_STRIP_SIZE       "\":1,"
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"RGBCW\","
    "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":0,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\"},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"Solid Rgbcct 01\","
    "\"" D_JSON_ANIMATIONMODE  "\":\"" D_JSON_EFFECTS "\","
    "\"" D_JSON_EFFECTS        "\"{\"Function\":8},"//\"Solid RGBCCT\"},"
    "\"" D_JSON_BRIGHTNESS     "\":100"
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

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2

  #define USE_MODULE_CONTROLLER_OILFURNACE
    
  #define USE_MODULE_SENSORS_DS18B20
  
  #define USE_MODULE_SENSORS_ULTRASONICS
  #define USE_AMBIENT_TEMP_SENSOR_FOR_SPEEDOFSOUND
  // #define ENABLE_DEVFEATURE_ULTRASONIC_DURATION_RAW_THRESHOLD_CHECK

  #define ENABLE_DEVFEATURE_DELAYED_RESTART_WITHOTA_FOR_DEBUGGING
  // #define ENABLE_DEVFEATURE_OTAFALLBACK_WITH_FASTBOOT_DETECTED //to be done long term

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_ULTRASONICS
      "\"D1\":\"" D_GPIO_FUNCTION_SR04_ECHO_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_SR04_TRIG_CTR  "\","  
      #endif        
      "\"D6\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  
  #define D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG "GarageOutside"
  #define D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "SpeedOfSound_Ambient"
  #define D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "BackUpTank"
  // #define D_DEVICE_FURNACE_ACTIVE_FRIENDLY_NAME_LONG "Furnace Power"
  
  #define D_DEVICE_BUTTON_1_CTR "FurnaceActive"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_BUTTON_1_CTR "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "[40,143,81,7,51,20,1,189],"   //D6 group of 3                                           
        "[40,255,100,29,205,201,168,203],"                                             
        "[40,255,100,29,205,248,248,249]"  
      "]"  
    "}"
  "}";

#endif


#ifdef DEVICE_OILFURNACE
  #define DEVICENAME_CTR          "oilfurnace"
  #define DEVICENAME_FRIENDLY_CTR "Oil Furnace"

  /**
   * 2 pipe sensors (boiler_in, boiler_out, maybe on the heat pipe?)
   * light sensor for lock_out
   * lockout powered
   * furnace power  
   * bme ambient
   * motion sensor
   */

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2

  #define USE_MODULE_SENSORS_ANALOG
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_DS18B20
  #define USE_MODULE_SENSORS_BME
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
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"D3\":\"" D_GPIO_FUNCTION_SWT1_CTR  "\","        // Furnace main power
      "\"D7\":\"" D_GPIO_FUNCTION_SWT2_CTR  "\","        // Furnace "lockout"
      #endif      
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D5\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR "\","
      #endif
      "\"D6\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  
  #define D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG  "FurnaceInflow"
  #define D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG  "FurnaceOutflow"
  #define D_DEVICE_SENSORS_ANALOG_1_CTR       "LockOut Lamp Luminance"
  #define D_DEVICE_SENSORS_SWITCH_1_CTR       "FurnaceActive"
  #define D_DEVICE_SENSORS_SWITCH_2_CTR       "FurnaceLockOut"
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
        "\"" D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_ANALOG_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSORS_ANALOG_1_CTR "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "]"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSORS_SWITCH_1_CTR "\","
        "\"" D_DEVICE_SENSORS_SWITCH_2_CTR "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["                                
        "[40,170,67,3,30,19,2,25],"                              
        "[40,255,169,120,53,22,4,240]"
      "]"  
    "}"
  "}";

#endif

#ifdef DEVICE_GARAGELIGHT
  #define DEVICENAME_CTR          "garagelight"
  #define DEVICENAME_FRIENDLY_CTR "Garage Security Lights 2"
  
//   #define FORCE_TEMPLATE_LOADING
//   #define SETTINGS_HOLDER 1

//   // Core
//   #define USE_MODULE_CORE_RULES
//   // Sensors 
//   #define USE_MODULE_SENSORS_INTERFACE 
//   #define USE_MODULE_SENSORS_MOTION
//   #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Driveway Top"
//   #define D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "Back Garden"
//   // Drivers
//   #define USE_MODULE_DRIVERS_INTERFACE
//   #define USE_MODULE_DRIVERS_RELAY
//   #define MAX_RELAYS 2
//   // Energy
//   #define USE_MODULE_ENERGY_INTERFACE
//   #define USE_MODULE_ENERGY_ADE7953

//   #define ENABLE_DEVFEATURE_RELAY_ENABLE_TIME_WINDOW_LOCKS

//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
//     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_JSON_GPIOC "\":{"
//       #ifdef USE_MODULE_SENSORS_MOTION
//       "\"13\":\"" D_GPIO_FUNCTION_PIR_1_NP_CTR     "\","
//       "\"5\":\"" D_GPIO_FUNCTION_PIR_2_NP_CTR     "\","
//       #endif
//       #ifdef USE_MODULE_SENSORS_SWITCHES
//       "\"13\":\"" D_GPIO_FUNCTION_SWT1_NP_CTR  "\","
//       "\"5\":\""  D_GPIO_FUNCTION_SWT2_NP_CTR  "\","
//       #endif
//       #ifdef USE_MODULE_DRIVERS_RELAY
//       "\"4\":\"" D_GPIO_FUNCTION_REL1_CTR  "\","
//       "\"15\":\"" D_GPIO_FUNCTION_REL2_CTR  "\","
//       #endif 
//       #ifdef USE_MODULE_ENERGY_ADE7953
//       "\"14\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR  "\","
//       "\"12\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR  "\","
//       "\"16\":\"" D_GPIO_FUNCTION_ADE7953_IRQ_CTR  "\","
//       #endif 
//       #ifdef USE_MODULE_SENSORS_ANALOG_TEMPERATURE
//       "\"A0\":\"" D_GPIO_FUNCTION_ANALOG_TEMPERATURE_CTR  "\","
//       #endif 
//       "\"0\":\"" D_GPIO_FUNCTION_LED1_CTR "\""
//     "},"
//     "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
//   "}";

//   #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Driveway"
//   #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Garden"

//   #define USE_FUNCTION_TEMPLATE
//   DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
//   "{"
//     //device_names:{"module_name":["relay1","relay2"]}
//     "\"" D_JSON_DEVICENAME "\":{"
//       "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
//         "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
//         "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
//       "],"
//       "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
//         "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
//         "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
//       "],"
//       "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
//         "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\","
//         "\"" D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "\""
//       "]"    
//     "},"
//     "\"RelayEnabled0\":{\"Enabled\":1,\"OnTime\":\"00D17:00:00\",\"OffTime\":\"00D00:00:00\"},"
//     "\"RelayEnabled1\":{\"Enabled\":1,\"OnTime\":\"00D17:00:00\",\"OffTime\":\"00D00:00:00\"}"
//   "}";


//   #define USE_RULES_TEMPLATE // Rules, like the other templates, will be feed into the same command structure, so can actually be combined with `FUNCTION_TEMPLATE`
//   DEFINE_PGM_CTR(RULES_TEMPLATE)
//   "{"
//     "\"Rule0\":{"
//       "\"Trigger\":{"
//         "\"Module\":\"Motion\","
//         "\"Function\":\"MotionStarted\","
//         "\"DeviceName\":0,"
//         "\"State\":2"
//       "},"
//       "\"Command\":{"
//         "\"Module\":\"Relays\","
//         "\"Function\":\"SetPower\","
//         "\"DeviceName\":0,"
//         "\"State\":1,"
//         "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOn\\\":300}}\""
//       "}"
//     "},"
//     "\"Rule1\":{"
//       "\"Trigger\":{"
//         "\"Module\":\"Motion\","
//         "\"Function\":\"MotionStarted\","
//         "\"DeviceName\":1,"
//         "\"State\":1"
//       "},"
//       "\"Command\":{"
//         "\"Module\":\"Relays\","
//         "\"Function\":\"SetPower\","
//         "\"DeviceName\":1,"
//         "\"State\":1,"
//         "\"JsonCommands\":\"{\\\"PowerName\\\":1,\\\"Relay\\\":{\\\"TimeOn\\\":300}}\""
//       "}"
//     "}"
//   "}";

// #endif
// #ifdef DEVICE_TESTBED_SHELLY2P5_01
  // #define DEVICENAME_CTR          "testbed_shelly25_01"
  // #define DEVICENAME_FRIENDLY_CTR "Testbed Shelly 2.5 #01"

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
    "\"RelayEnabled0\":{\"Enabled\":1,\"OnTime\":\"00D20:00:00\",\"OffTime\":\"00D05:00:00\"},"
    "\"RelayEnabled1\":{\"Enabled\":1,\"OnTime\":\"00D19:01:00\",\"OffTime\":\"00D05:00:00\"}"
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


#ifdef DEVICE_TREADMILL
  #define DEVICENAME_CTR          "treadmill"
  #define DEVICENAME_FRIENDLY_CTR "Camera OV2640 Tester"
  #define FORCE_TEMPLATE_LOADING
  
  #define USE_MODULE_ENERGY_INA219
  #define USE_MODULE_CONTROLLER_TREADMILL
  #define D_DEVICE_SENSOR_CURRENT "Trigger"

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE)   
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"    
      #if defined(USE_MODULE_ENERGY_INA219) || defined(USE_MODULE_SENSORS_BME)
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR   "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_INA219_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CURRENT "\""
      "]"
    "}"
  "}";

#endif





/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Spare room ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


#ifdef DEVICE_SHELLYDIMMER_SPAREROOM_CEILING
  #define DEVICENAME_CTR          "shellydimmer_spareroom"
  #define DEVICENAME_FRIENDLY_CTR "Shelly Dimmer Spare Room"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2
  
  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_CORE_RULES
  #define USE_HARDWARE_DEFAULT_RULES_1

  #define DISABLE_SERIAL_LOGGING //temp measure

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
 * */
#ifdef DEVICE_MASTERBEDROOMSENSOR
  #define DEVICENAME_CTR          "masterbedroomsensor"
  #define DEVICENAME_FRIENDLY_CTR "Master Bedroom Sensor"

  #define FORCE_TEMPLATE_LOADING
     
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

#ifdef DEVICE_FLOORFAN2
  #define DEVICENAME_CTR          "floorfan2"
  #define DEVICENAME_FRIENDLY_CTR "IFan Floor Fan 2"
  
  #define FORCE_TEMPLATE_LOADING
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



/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Bedroom ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/
 
#ifdef DEVICE_BEDROOMSENSOR
  #define DEVICENAME_CTR          "bedroomsensor"
  #define DEVICENAME_FRIENDLY_CTR "Bedroom Sensor"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2

  /**
   * BME              D1/D2
   * PIR Motion       D6
   * Door Position    D5
   * Door Lock        D7
   * BH1 Light        D1/D2
   */
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION
  #define USE_MODULE_SENSORS_DOOR
  
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE // future devices will move to creating 3/4 types via "new" and are dynamic (aka wled)

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
      "\"D6\":\"" D_GPIO_FUNCTION_SWT1_CTR     "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DOOR
      "\"D5\":\"" D_GPIO_FUNCTION_DOOR_OPEN_CTR     "\","
      "\"D7\":\"" D_GPIO_FUNCTION_DOOR_LOCK_CTR     "\","
      #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      "\"LBI\":\"" D_GPIO_FUNCTION_LED1_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  /**
   * Allow using notifications with pixel groupings, ie applying pixel notificaiton across entire strip or section with pixel_index ranges
   * 
   * */


 #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"grb\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":1" //slow glow
    "},"
    "\"Transition\":{\"Order\":\"InOrder\",\"PixelUpdatePerc\":2,\"RateMs\":1000},"
    "\"TimeMs\":500,"
    "\"ColourPalette\":43," //c12    43 is the colours for this christmas
    "\"BrightnessRGB\":100"
  "}";
  #define USE_TASK_RGBLIGHTING_NOTIFICATIONS   
  #define STRIP_SIZE_MAX                      50   

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" "bedroom" "\","
        "\"" "bedroomdoor" "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" "Bedroom" "\""
      "],"
      "\"" D_MODULE_SENSORS_DOOR_FRIENDLY_CTR "\":["
        "\"" "bedroomDOOR" "\","
        "\"" "bedroomlock" "\""
      "]"
    "}"
  "}";


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
    // Switch0 HIGH = Motion0 Event Started, ie report as motion with motion name
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_DOOR_FRIENDLY_CTR "\","
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


#ifdef DEVICE_RGBNOTIFICATION_01
  #define DEVICENAME_CTR          "rgbnotification_01"
  #define DEVICENAME_FRIENDLY_CTR "RGB Notifications 01"
  
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE // future devices will move to creating 3/4 types via "new" and are dynamic (aka wled)

  #define USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK

  #define ENABLE_BOOT_OVERRIDE_INIT


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      "\"LBI\":\"" D_GPIO_FUNCTION_LED1_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

 #define STRIP_SIZE_MAX 12
 #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"grbw\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_NOTIFICATIONS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":\"Slow Glow\"" //slow glow
    "},"
    "\"Transition\":{\"Order\":\"InOrder\",\"PixelUpdatePerc\":2,\"RateMs\":1000},"
    "\"TimeMs\":500,"
    "\"ColourPalette\":43," //c12    43 is the colours for this christmas
    "\"BrightnessRGB\":0"
  "}";
  // #define USE_TASK_RGBLIGHTING_NOTIFICATIONS   
  #define STRIP_SIZE_MAX                      50   

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" "bedroom" "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" "Bedroom" "\""
      "],"
      "\"" D_MODULE_SENSORS_DOOR_FRIENDLY_CTR "\":["
        "\"" "bedroomDOOR" "\","
        "\"" "bedroomlock" "\""
      "]"
    "}"
  "}";

#endif



#ifdef DEVICE_AIRPURIFIER
  #define DEVICENAME_CTR          "airpurifier"
  #define DEVICENAME_FRIENDLY_CTR "Air Purifier 1"
  
  #define FORCE_TEMPLATE_LOADING
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

#ifdef DEVICE_BEDROOM_CEILINGFAN
  #define DEVICENAME_CTR          "bedroom_ceilingfan"
  #define DEVICENAME_FRIENDLY_CTR "Bedroom Ceiling Fan"
  
  #define FORCE_TEMPLATE_LOADING
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

#ifdef DEVICE_FLOORFAN1
  #define DEVICENAME_CTR          "floorfan1"
  #define DEVICENAME_FRIENDLY_CTR "IFan Floor Fan 1"
  
  #define FORCE_TEMPLATE_LOADING
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


#ifdef DEVICE_BEDROOMBLINDS
  #define DEVICENAME_CTR          "bedroomblinds"
  #define DEVICENAME_FRIENDLY_CTR "Bedroom Blinds"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  
  // #define ENABLE_BUG_TRACING

  #define USE_MODULE_CONTROLLER_BLINDS
  #define USE_MODULE_DRIVERS_HBRIDGE
  //#define USE_MODULE_SENSORS_ANALOG
      
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_HBRIDGE
      "\"D1\":\"" D_GPIO_FUNCTION_HBRIDGE_L9110_IA_CTR   "\","
      "\"D6\":\"" D_GPIO_FUNCTION_HBRIDGE_L9110_OA_CTR   "\","
      // "\"D5\":\"" D_GPIO_FUNCTION_HBRIDGE_L9110_IB_CTR   "\","
      // "\"D7\":\"" D_GPIO_FUNCTION_HBRIDGE_L9110_OB_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_ANALOG
      "\"A0\":\"" D_GPIO_FUNCTION_ANALOG_POSITION_CTR "\","
      #endif
      "\"D0\":\"" D_GPIO_FUNCTION_LED1_INV_CTR   "\","  
      "\"D4\":\""  D_GPIO_FUNCTION_LED1_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_FRIENDLY_NAME_LONG "Blinds"

  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    //device_names:{"module_name":["relay1","relay2"]}
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_HBRIDGE_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_FRIENDLY_NAME_LONG "\""
      "]"
  "}";



  // #define DEVICE_CRITICALITY_CTR  CRITICALITY_LEVEL_LOW
  // #define DEVICE_STABILITY_CTR    STABILITY_LEVEL_PRE_RELEASE
  // #define USE_MODULE_CONTROLLER_BLINDS
  // #define USE_MODULE_DRIVERS_HBRIDGE
  
  //   pCONT_set->Settings.module = MODULE_WEMOS_ID;
  //   pCONT_set->Settings.module_pins.io[3] = GPIO_RGB_DATA_ID;
#endif




/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Landing ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

/**
 * To be esp32 poe later
 * */
#ifdef DEVICE_LANDINGPANEL
  #define DEVICENAME_CTR            "landingpanel"
  #define DEVICENAME_FRIENDLY_CTR   "Landing Panel ESP8266"

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  #define USE_SK6812_METHOD_DEFAULT
  #define STRIP_SIZE_MAX 36

  #define USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
  
  #define DISABLE_WEBSERVER

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_MOTION

  #define USE_MODULE_CORE_RULES

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
      "\"D7\":\"" D_GPIO_FUNCTION_SWT1_CTR "\","
      "\"D6\":\"" D_GPIO_FUNCTION_SWT2_CTR "\","
      #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":40,"
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"rgbwc\","
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
  
  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Landing"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Landing"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "_Doppler" "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";
    
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

/**
 * This will become the final version of this panel, but requires animation fixing first
 * */
#ifdef DEVICE_LANDINGPANEL2
  #define DEVICENAME_CTR            "landingpanel2"
  #define DEVICENAME_FRIENDLY_CTR   "Landing Panel ESP32 Tester"

  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define USE_MODULE_LIGHTS_ADDRESSABLE
  // #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  // #define USE_SK6812_METHOD_DEFAULT
  // #define STRIP_SIZE_MAX 40
  
  #define DISABLE_WEBSERVER

  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_MOTION

  // #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_NEXTION

  // #define USE_MODULE_TEMPLATE
  // DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
  //   "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  //   "\"" D_JSON_GPIOC "\":{"
  //     #ifdef USE_MODULE_SENSORS_BME
  //     "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
  //     "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
  //     #endif
  //     #ifdef USE_MODULE_SENSORS_MOTION
  //     "\"D7\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR "\","
  //     #endif
  //     "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
  //   "},"
  //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  // "}";
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"      
      // #ifdef USE_MODULE_SENSORS_BME
      // "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      // "\"23\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      // #endif
      // #ifdef USE_MODULE_SENSORS_MOTION
      // "\"21\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR "\","
      // #endif
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
      // "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
      "\"2\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":40,"
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"rgbwc\","
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
    "\"" D_JSON_BRIGHTNESS_RGB "\":100"
  "}";
  
  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Landing"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Landing"
  
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

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  
  #define USE_MODULE_SENSORS_INTERFACE
  #define ENABLE_DEVFEATURE_SENSORS_INTERFACE_SHOW_TEMPERATURE_AS_COLOUR
  #define USE_MODULE_SENSORS_DS18B20
  
  #define USE_MODULE_CONTROLLER_SENSORCOLOURS
     
  
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

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  
  // #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_DS18B20
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
      #ifdef USE_MODULE_SENSORS_DS18B20
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
****** vv ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** vv ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** vv ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** vv ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** vv ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** vv ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** vv ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/
















//  */


// #endif // user_mciaehl

#endif //header
