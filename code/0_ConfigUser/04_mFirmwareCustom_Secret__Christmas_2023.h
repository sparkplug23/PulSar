#ifndef _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_CHRISTMAS23_H
#define _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_CHRISTMAS23_H


/*********************************************************************************************\
 * Devices to be used around christmas time
 * - I should keep this file as backup each year and add when needed, perhaps install on USB stick in light boxes with compiled versions for flashing .bin (or just copy of project at that state for recompile) each year if needed
\*********************************************************************************************/

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"
#include "2_CoreSystem/03_HardwareTemplates/mHardwareTemplates.h"

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Enable Defines*******************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


// #define DEVICE_CHRISTMAS__OUTSIDE_TREE_I2S_PARALLEL_8_CHANNELS_MODE
// #define DEVICE_CHRISTMAS__LIGHTING_EFFECTS__DININGROOM_SNOWTREE_16_CHANNELS
// DEVICE_RGBSTRING_CHRISTMAS_ESP32_HALLWAYTREE_XMAS2022  // just do the 200 ws2811 inline with the 400 under, same as last year, single esp32-1pin
// DEVICE_RGBSTRING_CHRISTMAS_ESP32_TVROOM_XMAS2022 // Use other snow tree, use transparant wire from big snow last year. Do 4pin esp32. No docorations, just light it up
// DEVICE_RGBSTRING_CHRISTMAS_ESP32_UTILITY_SHELF // do 2pin, 2 segments, green tree (50) + candle (rgbw sk6812). Use new wire (clear 30awg) incoming for it, perhaps 3 pin it one for each candle to reduce visible wiries
#define DEVICE_CHRISTMAS__GIVEAWAY_ESP32_4PIN_CONTROLLER     // 4pin versions for green BGR


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** LOCATION: Outside ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

/**
 * @brief Outside Tree controller should not load its saved state, but instead use the default template
 * Presets/Playlists will still need to load from the filesystem
 * 
 */
#ifdef DEVICE_CHRISTMAS__OUTSIDE_TREE_I2S_PARALLEL_8_CHANNELS_MODE
  #define DEVICENAME_CTR          "xmas_outside_tree"
  #define DEVICENAME_FRIENDLY_CTR "XMAS Outside Tree"
  #define DEVICENAME_ROOMHINT_CTR "XMAS|Outside"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883
    #define SETTINGS_HOLDER 1239

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  #define ENABLE_FREERAM_APPENDING_SERIAL

  /***********************************
   * SECTION: System Configs
  ************************************/     

  // #define USE_MODULE_DRIVERS_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define USE_MODULE_NETWORK_WEBSERVER23
  #define ENABLE_WEBSERVER_LIGHTING_WEBUI
  
  /***********************************
   * SECTION: Lighting Configs
  ************************************/    
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_OCTOBER_2023

  #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE

  // 13, 18, 19, 22, 23, 25, 26, 27       USED
  // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":27,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":300
      },
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":300,
        "Length":300
      },
      {
        "Pin":25,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":600,
        "Length":300
      },
      {
        "Pin":26,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":900,
        "Length":300
      },
      {
        "Pin":22,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1200,
        "Length":300
      },
      {
        "Pin":23,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1500,
        "Length":300
      },
      {
        "Pin":18,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1800,
        "Length":300
      },
      {
        "Pin":19,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2100,
        "Length":300
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        2400
      ],
      "ColourPalette":"Christmas Snowy 02",
      "SegColour0": {
        "Hue": 0,
        "Sat":100,
        "BrightnessRGB":5
      },
      "Effects": {
        "Function":"Slow Glow",
        "Speed":127,
        "Intensity":255,
        "Grouping":1
      },
      "Transition": {
        "TimeMs": 980,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 7,
    "BrightnessCCT": 0
  }
  )=====";
  
  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME          "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME  "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{},"
    "\"" D_JSON_BASE          "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT      "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  /***********************************
   * SECTION: Device Configs
  ************************************/    

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}" 
  "}";

#endif // DEVICE_CHRISTMAS__OUTSIDE_TREE_I2S_PARALLEL_8_CHANNELS_MODE


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** LOCATION: DiningRoom ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**
 * @brief Outside Tree controller should not load its saved state, but instead use the default template
 * Presets/Playlists will still need to load from the filesystem
 * 
 */
#ifdef DEVICE_CHRISTMAS__LIGHTING_EFFECTS__DININGROOM_SNOWTREE_16_CHANNELS
  #define DEVICENAME_CTR          "xmas_diningroom_snowtree"
  #define DEVICENAME_FRIENDLY_CTR "xmas_diningroom_snowtree"
  #define DEVICENAME_ROOMHINT_CTR "XMAS|Outside"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883

  /***********************************
   * SECTION: System Debug Options
  ************************************/    

  #define ENABLE_DEBUGFEATURE__16PIN_PARALLEL_OUTPUT

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  /***********************************
   * SECTION: System Configs
  ************************************/    
 
  #define SETTINGS_HOLDER 1239

  // #define USE_MODULE_DRIVERS_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define USE_MODULE_NETWORK_WEBSERVER23
  #define ENABLE_WEBSERVER_LIGHTING_WEBUI
  
  /***********************************
   * SECTION: Lighting Configs
  ************************************/    
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_OCTOBER_2023

  //#define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
  #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE

  //#define DATA_BUFFER_PAYLOAD_MAX_LENGTH 3000

  // #define ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM

  // // 13, 18, 19, 22, 23, 25, 26, 27       USED
  // // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // R"=====(
  // {
  //   "BusConfig":[
  //     {
  //       "Pin":13,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":0,
  //       "Length":100
  //     },
  //     {
  //       "Pin":14,
  //       "ColourOrder":"BGR",
  //       "BusType":"WS2812_RGB",
  //       "Start":100,
  //       "Length":100
  //     },
  //     {
  //       "Pin":27,
  //       "ColourOrder":"BGR",
  //       "BusType":"WS2812_RGB",
  //       "Start":200,
  //       "Length":100
  //     },
  //     {
  //       "Pin":26,
  //       "ColourOrder":"BGR",
  //       "BusType":"WS2812_RGB",
  //       "Start":300,
  //       "Length":100
  //     },
  //     {
  //       "Pin":25,
  //       "ColourOrder":"BGR",
  //       "BusType":"WS2812_RGB",
  //       "Start":400,
  //       "Length":100
  //     },
  //     {
  //       "Pin":33,
  //       "ColourOrder":"BGR",
  //       "BusType":"WS2812_RGB",
  //       "Start":500,
  //       "Length":100
  //     },
  //     {
  //       "Pin":32,
  //       "ColourOrder":"BGR",
  //       "BusType":"WS2812_RGB",
  //       "Start":600,
  //       "Length":100
  //     },
  //     {
  //       "Pin":2,
  //       "ColourOrder":"BGR",
  //       "BusType":"WS2812_RGB",
  //       "Start":700,
  //       "Length":100
  //     }
  //   ],
  //   "Segment0": {
  //     "PixelRange": [
  //       0,
  //       800
  //     ],
  //     "ColourPalette":"Christmas Snowy 02",
  //     "SegColour0": {
  //       "Hue": 0,
  //       "Sat":100,
  //       "BrightnessRGB":5
  //     },
  //     "Effects": {
  //       "Function":"Slow Glow",
  //       "Speed":127,
  //       "Intensity":255,
  //       "Grouping":1
  //     },
  //     "Transition": {
  //       "TimeMs": 980,
  //       "RateMs": 1000
  //     },
  //     "BrightnessRGB": 100,
  //     "BrightnessCCT": 0
  //   },
  //   "BrightnessRGB": 100,
  //   "BrightnessCCT": 0
  // }
  // )=====";


/*

Pin    :      Connection (Left, Right, 1 to 8 with 1 at top)

4  : L1
16 : L2
17 : L3
18 : L4
19 : L5
21 : L6
22 : L7
23 : L8

2  : R1
13 : R2
14 : R3
27 : R4
26 : R5
25 : R6
33 : R7
32 : R8








*/

#ifdef ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
  // 13, 18, 19, 22, 23, 25, 26, 27       USED
  // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      },
      {
        "Pin":16,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":100,
        "Length":100
      },
      {
        "Pin":17,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":200,
        "Length":100
      },
      {
        "Pin":18,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":300,
        "Length":100
      },
      {
        "Pin":19,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":400,
        "Length":100
      },
      {
        "Pin":21,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":500,
        "Length":100
      },
      {
        "Pin":22,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":600,
        "Length":100
      },
      {
        "Pin":23,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":700,
        "Length":100
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        800
      ],
      "ColourPalette":"Christmas Snowy 02",
      "Effects": {
        "Function":"Sweep Random",
        "Speed":127,
        "Intensity":127,
        "Grouping":1
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 23
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 10,
    "BrightnessCCT": 0
  }
  )=====";

#endif

#ifdef ENABLE_NEOPIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE
  
  // #define DATA_BUFFER_PAYLOAD_MAX_LENGTH 4000

  // // 13, 18, 19, 22, 23, 25, 26, 27       USED
  // // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // R"=====(
  // {
  //   "BrightnessRGB": 10,
  //   "BusConfig":[
  //     {
  //       "Pin":4,
  //       "CO":"BGR",
  //       "BT":"WS2812_RGB",
  //       "S":0,
  //       "L":100
  //     }
  //   ],    
  //   "Segment0": {
  //     "PixelRange": [
  //       0,
  //       100
  //     ],
  //     "ColourPalette":"Christmas Snowy 02",
  //     "Effects": {
  //       "Function":"Sweep Random",
  //       "Speed":127,
  //       "Intensity":127,
  //       "Grouping":1
  //     },
  //     "Transition": {
  //       "TimeMs": 0,
  //       "RateMs": 23
  //     },
  //     "BrightnessRGB": 100,
  //     "BrightnessCCT": 0
  //   },
  //   "BrightnessRGB": 10,
  //   "BrightnessCCT": 0
  // }
  // )=====";



  // #define DATA_BUFFER_PAYLOAD_MAX_LENGTH 4000

  // 13, 18, 19, 22, 23, 25, 26, 27       USED
  // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "CO":"GRB",
        "BT":"WS2812_RGB",
        "S":0,
        "L":100
      },
      {
        "Pin":16,
        "CO":"GRB",
        "BT":"WS2812_RGB",
        "S":100,
        "L":100
      },
      {
        "Pin":17,
        "CO":"GRB",
        "BT":"WS2812_RGB",
        "S":200,
        "L":100
      },
      {
        "Pin":18,
        "CO":"GRB",
        "BT":"WS2812_RGB",
        "S":300,
        "L":100
      },
      {
        "Pin":19,
        "CO":"GRB",
        "BT":"WS2812_RGB",
        "S":400,
        "L":100
      },
      {
        "Pin":21,
        "CO":"GRB",
        "BT":"WS2812_RGB",
        "S":500,
        "L":100
      },
      {
        "Pin":22,
        "CO":"GRB",
        "BT":"WS2812_RGB",
        "S":600,
        "L":100
      },
      {
        "Pin":23,
        "CO":"GRB",
        "BT":"WS2812_RGB",
        "S":700,
        "L":100
      },
      {
        "Pin":2,
        "CO":"GRB",
        "BT":"WS2812_RGB",
        "S":800,
        "L":100
      },
      {
        "Pin":13,
        "CO":"GRB",
        "BT":"WS2812_RGB",
        "S":900,
        "L":100
      },
      {
        "Pin":14,
        "CO":"GRB",
        "BT":"WS2812_RGB",
        "S":1000,
        "L":100
      },
      {
        "Pin":27,
        "CO":"GRB",
        "BT":"WS2812_RGB",
        "S":1100,
        "L":100
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        1200
      ],
      "Effects": {
        "Function":"Sweep Random"
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 23
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 100
  }
  }
  )=====";



  // // 13, 18, 19, 22, 23, 25, 26, 27       USED
  // // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // R"=====(
  // {
  //   "BusConfig":[
  //     {
  //       "Pin":4,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":0,
  //       "Length":100
  //     },
  //     {
  //       "Pin":16,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":100,
  //       "Length":100
  //     },
  //     {
  //       "Pin":17,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":200,
  //       "Length":100
  //     },
  //     {
  //       "Pin":18,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":300,
  //       "Length":100
  //     },
  //     {
  //       "Pin":19,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":400,
  //       "Length":100
  //     },
  //     {
  //       "Pin":21,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":500,
  //       "Length":100
  //     },
  //     {
  //       "Pin":22,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":600,
  //       "Length":100
  //     },
  //     {
  //       "Pin":23,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":700,
  //       "Length":100
  //     },
  //     {
  //       "Pin":2,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":800,
  //       "Length":100
  //     },
  //     {
  //       "Pin":13,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":900,
  //       "Length":100
  //     },
  //     {
  //       "Pin":14,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":1000,
  //       "Length":100
  //     },
  //     {
  //       "Pin":27,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":1100,
  //       "Length":100
  //     },
  //     {
  //       "Pin":26,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":400,
  //       "Length":100
  //     },
  //     {
  //       "Pin":25,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":1200,
  //       "Length":100
  //     },
  //     {
  //       "Pin":33,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":1300,
  //       "Length":100
  //     },
  //     {
  //       "Pin":32,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":1400,
  //       "Length":100
  //     }
  //   ],
  //   "Segment0": {
  //     "PixelRange": [
  //       0,
  //       1400
  //     ],
  //     "ColourPalette":"Christmas Snowy 02",
  //     "Effects": {
  //       "Function":"Sweep Random",
  //       "Speed":127,
  //       "Intensity":127,
  //       "Grouping":1
  //     },
  //     "Transition": {
  //       "TimeMs": 0,
  //       "RateMs": 23
  //     },
  //     "BrightnessRGB": 100,
  //     "BrightnessCCT": 0
  //   },
  //   "BrightnessRGB": 10,
  //   "BrightnessCCT": 0
  // }
  // )=====";

  #endif 


  // #define ENABLE_DEBUGFEATURE_SET_PINS_MANUAL_STATES


  #define GPIO_SET_LEFT_TO_LOW

  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    // "\"" D_JSON_GPIO_NUMBER "\":{" 
    //   #ifdef GPIO_SET_LEFT_TO_LOW
    //   "\"4\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
    //   "\"16\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
    //   "\"17\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
    //   "\"18\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
    //   "\"19\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
    //   "\"21\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
    //   "\"22\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
    //   "\"23\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\""
    //   #endif
    // "},"
    "\"" D_JSON_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  /***********************************
   * SECTION: Device Configs
  ************************************/    

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}" 
  "}";

#endif // DEVICE_TESTGROUP__LIGHTING_EFFECTS__L15__ESP32_I2S_PARALLEL_RED_BOARDS





/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** LOCATION:  ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** LOCATION: Shared ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**
 * @brief Outside Tree controller should not load its saved state, but instead use the default template
 * Presets/Playlists will still need to load from the filesystem
 * 
 */
#ifdef DEVICE_CHRISTMAS__GIVEAWAY_ESP32_4PIN_CONTROLLER
  #define DEVICENAME_CTR          "xmas32_giveaway1"
  #define DEVICENAME_FRIENDLY_CTR "XMAS Outside Tree"
  #define DEVICENAME_ROOMHINT_CTR "XMAS|Outside"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883
    #define SETTINGS_HOLDER 1239

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  #define ENABLE_FREERAM_APPENDING_SERIAL
  
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  /***********************************
   * SECTION: System Configs
  ************************************/   

  #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define USE_MODULE_NETWORK_WEBSERVER23
  #define ENABLE_WEBSERVER_LIGHTING_WEBUI
  
  /***********************************
   * SECTION: Lighting Configs
  ************************************/    
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_OCTOBER_2023

  #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE

  #define ENABLE_DEVFEATURE_LIGHTING__ADD_DEVSTAGE_TO_EFFECT_NAME

  #define ENABLE_DEVFEATURE_LIGHTING__REMOVE_PALETTEO23

  // #define ENABLE_DEBUG_LINE_HERE_TRACE // should only be used when bug finding, then disabled

  // PixelDriver 2,4,18,19
  // 13, 18, 19, 22, 23, 25, 26, 27       USED
  // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":2,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      },
      {
        "Pin":4,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":100,
        "Length":100
      },
      {
        "Pin":18,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":200,
        "Length":100
      },
      {
        "Pin":19,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":300,
        "Length":100
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        400
      ],
      "ColourPalette":"Christmas Snowy 02",
      "Effects": {
        "Function":"Static Palette",
        "Speed":127,
        "Intensity":127,
        "Grouping":1
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 1000
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 7
  }
  )=====";
  
  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME          "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME  "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{},"
    "\"" D_JSON_BASE          "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT      "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  /***********************************
   * SECTION: Device Configs
  ************************************/    

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}" 
  "}";

#endif // DEVICE_CHRISTMAS__OUTSIDE_TREE_I2S_PARALLEL_8_CHANNELS_MODE




#endif // _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_CHRISTMAS23_H
