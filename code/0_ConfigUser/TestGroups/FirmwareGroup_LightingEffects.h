#ifndef _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_ACTIVE2_TESTGROUPS_BETA_H
#define _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_ACTIVE2_TESTGROUPS_BETA_H

/*********************************************************************************************\
 * Developing/Testbeds: 
 *  ** Only devices that are actively being ran as test devices.
 * Aiming to move these into the garage on a shelf, with a camera looking at them. Everything should be OTA flashed and debugged.
\*********************************************************************************************/

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"
#include "6_Lights/00_Interface/mInterfaceLight_Defines.h"
#include "2_CoreSystem/03_HardwareTemplates/mHardwareTemplates.h"

/**
 * @brief List of hardware, group L for lighting
 * 
 * Device L1: 
 * - ESP32
 * - Single Bus, Single Segment
 * - Single 100 micro leds (RGB)
 * 
 * Device L2:
 * - ESP32
 * - Dual Bus, Dual Segment as addressable
 * - 100 Micro + 16 RGB Ring
 * 
 * Device L3:
 * - ESP8266
 * - Single Bus, Single Segment
 * - Single 100 micro leds
 * 
 * Device L4: 
 * - H801 (ESP8266)
 * - Single RGBCCT Bus, 5 PWM
 * - Run as normal scene light
 * 
 * Device L5: 
 * - H801 (ESP8266)
 * - RGB Channels on Segment 0
 * - WW/CW Channels on Segment 1
 * 
 * Device L6: 
 * - H801 (ESP8266)
 * - 5 Split White channels
 * 
 * Device L7: 
 * - ESP32
 * - Single Bus, Single Segment
 * - Single 100 micro leds (RGB) on roof
 * 
 * Device L8: 
 * - ESP32
 * - Single Bus, Single Segment
 * - Single RGBW 60 Strip on metal holder   SK6812_RGBW
 * 
 * Device L9: 
 * - ESP32
 * - Single Bus, Single Segment
 * - Single RGBW 60 Strip on metal holder   SK6812_RGBW
 * 
 * Device L10: 
 * - ESP32
 * - Single Bus, Single Segment
 * - Single RGBW 60 Strip on metal holder   SK6812_RGBW
 * 
 * Device L11: 
 * - ESP32
 * - Single Bus, Single Segment
 * - Single RGBW 60 Strip on metal holder   SK6812_RGBW
 * 
 * Device L12: 
 * - ESP32
 * - Single Bus, Single Segment
 * - Single RGBW 60 Strip on metal holder   SK6812_RGBW
 * 
 */

//--------------------------------[Enable Device]-------------------------------------

// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L1__ESP32_1CH_ADDESSABLE_100LEDS
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L2__ESP32_2CH_DUAL_BUS_ADDRESSABLE
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L3__ESP8266_1CH_RGB_100LEDS
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L4__H801_5CH_PWM_RGBCCT
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L5__H801_3CHRGB_AND_2CHCW
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L6__H801_FIVE_1CH_WHITE_CHANNELS
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L7__ESP32_ROOM_CEILING_100LEDS   // change this to something useful
#define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L8__ESP32_ROOM_METAL_144LEDS
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L9__ESP8266_WEBUI_OPTIM_3LEDS
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L10__ESP32__7SEGMENTCLOCK
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L11__ESP32_LARGE_SINGLE_PIN_TESTER    // Outside Tree Tester
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L12__ESP32_LARGE_SINGLE_PIN_TESTER_WITH_DEBUG_PINS // Timing Tester

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


/**
 * Going forward, here will support define that simply insert in firmwarecustom
 * Ie, instead of bathroom/ensuite/bedroom sensor lighting type having the same ifdefs configed, here will
 * be the define for it
 * 
 * Update Date: 2 Feb 23
*/

#ifdef USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__BASE_CONFIG_TO_BECOME_PHASED_INTO_DEFAULT_OPTIONS

  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define USE_MODULE_LIGHTS_ADDRESSABLE
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
    
  //   #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  //   #define USE_SK6812_METHOD_DEFAULT
    

    


// Step A: Replicate WLED to function as it was designed on the webui part. Only when this works for effect/colour controls etc, slowly change it to add more of my original stuff. 
// Gain complete understanding/implemenation as is, only then start changes. 
// WebUI of WLED style should become its own webserver option (not just the debug version).

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_MODULE_NETWORK_WEBSERVER23
  #define USE_MODULE_NETWORK_WEBSERVER23

  #define ESP32
  #undef ESP8266

  // #define ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE
  #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS

  // #define ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
  // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL

  // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_TESTER
  // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_DEVELOPER

  


  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES


  //mqtt debug
  #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
  // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // #define DISABLE_NETWORK
  // #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_WIFI
  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  #define ANIMATION_UPDATOR_TIME_MINIMUM 20

  #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  // #undef ESP32
  // #define ESP8266

  

  #define ENABLE_DEBUG_MANUAL_DELAYS

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

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
    #define DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
    #define ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT
    #define ENABLE_DEVFEATURE_SWITCH_TO_U16_GPIO_FUNCTIONS
    #define ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
    #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    #define ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
    /********* Group: Debug options only ************************/
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
    #define ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
    #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
    #define ENABLE_DEBUG_SERIAL

    #define ENABLE_DEVFEATURE_LIGHT__WEBUI_APPEND_EFFECT_CONFIG_TO_JSON_RESPONSE

    // Needs significant change to merge WLED and HACS
    #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
    // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

    #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG

    #define ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL

    #define ENABLE_DEVFEATURE_LIGHT__WLED_WEBUI_SEND_MY_PALETTE_COLOUR_BARS

    // #define ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

    // #define ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
    // #define ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
    
    #define ENABLE_DEBUG_LINE_HERE
    #define ENABLE_DEVFEATURE_PALETTE__FIX_WEBUI_GRADIENT_PREVIEW

    #define ENABLE_DEVFEATURE_LIGHT__HYPERION
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE
    // #define ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32

    #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD


    #define ENABLE_WEBSERVER_LIGHTING_WEBUI

    // #define ENABLE_DEVFEATURE_WEBUI__INCLUDE_URI_PRE2023



    // #define ENABLE_DEVFEATURE_LIGHT__PERMIT_PIXEL_INDEXING_GREATER_THAN_FIRST_ON_PWM_CHANNELS_FOR_MULTIPLE_SEGMENTS

    // #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28



    // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    // #define ENABLE_LOG_LEVEL_DEBUG
    // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

    
    

    // #define ENABLE_DEVFEATURE_LIGHT__PRELOAD_BUSCONFIG_FROM_TEMPLATE_AS_TEMPORARY_MEASURE
    #define ENABLE_DEVFEATURE_LIGHT__MOVE_ALL_BUS_STARTING_CODE_UNTIL_LOOP



#endif // USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__BASE_CONFIG


/************************************************************************************************************************************************************************
 *************************************************************************************************************************************************************************
  @brief TESTGROUP__LIGHTING
 ************************************************************************************************************************************************************************
 *************************************************************************************************************************************************************************/

/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__L1__ESP32_1CH_ADDESSABLE_100LEDS
  #ifndef DEVICENAME_CTR
    #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883

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

  /***********************************
   * SECTION: System Configs
  ************************************/    
 
  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define USE_MODULE_NETWORK_WEBSERVER23
  
  /***********************************
   * SECTION: Lighting Configs
  ************************************/    
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_SEPTEMBER_2023

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        100
      ],
      "ColourPalette":0,
      "SegColour0": {
        "Hue": 330,
        "Sat":100,
        "BrightnessRGB":5
      },
      "Effects": {
        "Function": 0,
        "Speed":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";
    
  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{" 
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":4"                // Digital WS2812
      #endif
    "},"
    "\"" D_JSON_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  /***********************************
   * SECTION: Device Configs
  ************************************/    

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":1},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";

#endif // DEVICE_TESTGROUP__LIGHTING_EFFECTS__L1__ESP32_1CH_ADDESSABLE_100LEDS


/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__L2__ESP32_2CH_DUAL_BUS_ADDRESSABLE
  #ifndef DEVICENAME_CTR
    #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883


    


// Step A: Replicate WLED to function as it was designed on the webui part. Only when this works for effect/colour controls etc, slowly change it to add more of my original stuff. 
// Gain complete understanding/implemenation as is, only then start changes. 
// WebUI of WLED style should become its own webserver option (not just the debug version).

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_MODULE_NETWORK_WEBSERVER23
  #define USE_MODULE_NETWORK_WEBSERVER23

  #define ESP32
  #undef ESP8266

  // #define ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE
  #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS

  // #define ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
  // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL

  // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_TESTER
  // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_DEVELOPER

  


  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES


  //mqtt debug
  #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
  // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // #define DISABLE_NETWORK
  // #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_WIFI
  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  #define ANIMATION_UPDATOR_TIME_MINIMUM 20

  #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  // #undef ESP32
  // #define ESP8266

  

  #define ENABLE_DEBUG_MANUAL_DELAYS

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

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
    #define DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
    #define ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT
    #define ENABLE_DEVFEATURE_SWITCH_TO_U16_GPIO_FUNCTIONS
    #define ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
    #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    #define ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
    /********* Group: Debug options only ************************/
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
    #define ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
    #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
    #define ENABLE_DEBUG_SERIAL

    #define ENABLE_DEVFEATURE_LIGHT__WEBUI_APPEND_EFFECT_CONFIG_TO_JSON_RESPONSE

    // Needs significant change to merge WLED and HACS
    #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
    // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

    #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG

    #define ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL

    #define ENABLE_DEVFEATURE_LIGHT__WLED_WEBUI_SEND_MY_PALETTE_COLOUR_BARS

    // #define ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

    // #define ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
    // #define ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
    
    #define ENABLE_DEBUG_LINE_HERE
    #define ENABLE_DEVFEATURE_PALETTE__FIX_WEBUI_GRADIENT_PREVIEW

    #define ENABLE_DEVFEATURE_LIGHT__HYPERION
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE
    // #define ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32

    #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD


    #define ENABLE_WEBSERVER_LIGHTING_WEBUI

    // #define ENABLE_DEVFEATURE_WEBUI__INCLUDE_URI_PRE2023



    // #define ENABLE_DEVFEATURE_LIGHT__PERMIT_PIXEL_INDEXING_GREATER_THAN_FIRST_ON_PWM_CHANNELS_FOR_MULTIPLE_SEGMENTS

    // #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28



    // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    // #define ENABLE_LOG_LEVEL_DEBUG
    // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

    
    

    // #define ENABLE_DEVFEATURE_LIGHT__PRELOAD_BUSCONFIG_FROM_TEMPLATE_AS_TEMPORARY_MEASURE
    #define ENABLE_DEVFEATURE_LIGHT__MOVE_ALL_BUS_STARTING_CODE_UNTIL_LOOP



  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{" 
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":4,"                // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_02_A_CTR "\":13,"               // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_03_A_CTR "\":14,"               // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_04_A_CTR "\":27"               // Digital SK6812
      #endif
    "},"
    "\"" D_JSON_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":1},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";

  #define USE_LIGHTING_TEMPLATE

  #define STRIP_SIZE_MAX 116
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      },
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":100,
        "Length":16
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        116
      ],
      "ColourPalette":0,
      "SegColour0": {
        "Hue": 330,
        "Sat":100,
        "BrightnessRGB":5
      },
      "Effects": {
        "Function": 1,
        "Speed":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";
  

#endif // DEVICE_TESTGROUP__LIGHTING_EFFECTS__L2__ESP32_2CH_DUAL_BUS_ADDRESSABLE





/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__L3__ESP8266_1CH_RGB_100LEDS
  #ifndef DEVICENAME_CTR
    #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883


  #define USE_MODULE_SENSORS_SOLAR_LUNAR




    


// Step A: Replicate WLED to function as it was designed on the webui part. Only when this works for effect/colour controls etc, slowly change it to add more of my original stuff. 
// Gain complete understanding/implemenation as is, only then start changes. 
// WebUI of WLED style should become its own webserver option (not just the debug version).

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  // #define USE_MODULE_NETWORK_WEBSERVER23
  // #define USE_MODULE_NETWORK_WEBSERVER23

  #undef ESP32
  #define ESP8266


  // #define ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE
  #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS

  // #define ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
  // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL

  // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_TESTER
  // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_DEVELOPER

  


  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES


  //mqtt debug
  #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
  // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // #define DISABLE_NETWORK
  // #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_WIFI
  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  #define ANIMATION_UPDATOR_TIME_MINIMUM 20

  #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  // #undef ESP32
  // #define ESP8266

  

  #define ENABLE_DEBUG_MANUAL_DELAYS

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_AUGUST_2023

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define USE_MODULE_LIGHTS_ADDRESSABLE
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
  //   #define DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
  //   #define ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT
  //   #define ENABLE_DEVFEATURE_SWITCH_TO_U16_GPIO_FUNCTIONS
  //   #define ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
  //   #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
  //   #define ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
  //   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
  //   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
  //   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
  //   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
  //   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
  //   // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
  //   /********* Group: Debug options only ************************/
  //   #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
  //   // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
  //   // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
  //   // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
  //   #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
  //   // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
  //   #define ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
  //   #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
  //   #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
  //   #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
  //   #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
  //   #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
  //   #define ENABLE_DEBUG_SERIAL

  //   #define ENABLE_DEVFEATURE_LIGHT__WEBUI_APPEND_EFFECT_CONFIG_TO_JSON_RESPONSE

  //   // Needs significant change to merge WLED and HACS
  //   #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
  //   // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

  //   #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG

  //   #define ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL

  //   #define ENABLE_DEVFEATURE_LIGHT__WLED_WEBUI_SEND_MY_PALETTE_COLOUR_BARS

  //   // #define ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

  //   // #define ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
  //   // #define ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
    
  //   #define ENABLE_DEBUG_LINE_HERE
  //   #define ENABLE_DEVFEATURE_PALETTE__FIX_WEBUI_GRADIENT_PREVIEW

  //   #define ENABLE_DEVFEATURE_LIGHT__HYPERION
  //   #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE
  //   // #define ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32

  //   #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD


  //   // #define ENABLE_WEBSERVER_LIGHTING_WEBUI

  //   // #define ENABLE_DEVFEATURE_WEBUI__INCLUDE_URI_PRE2023



  //   // #define ENABLE_DEVFEATURE_LIGHT__PERMIT_PIXEL_INDEXING_GREATER_THAN_FIRST_ON_PWM_CHANNELS_FOR_MULTIPLE_SEGMENTS

  //   // #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28



  //   // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
  //   // #define ENABLE_LOG_LEVEL_DEBUG
  //   // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
  //   // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

    
    

  //   // #define ENABLE_DEVFEATURE_LIGHT__PRELOAD_BUSCONFIG_FROM_TEMPLATE_AS_TEMPORARY_MEASURE
  //   #define ENABLE_DEVFEATURE_LIGHT__MOVE_ALL_BUS_STARTING_CODE_UNTIL_LOOP

  #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS 
  #define ANIMATION_UPDATOR_TIME_MINIMUM 20
  #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR
  #define ENABLE_DEBUG_MANUAL_DELAYS

  #define ENABLE_DEVFEATURE_PALETTE__CHANGE_MY_PALETTE_INDEXING_TO_255_RANGE

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_AUGUST_2023

  #define USE_LIGHTING_TEMPLATE
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS


  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{" 
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":3"   
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":1},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";


  #define USE_LIGHTING_TEMPLATE
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS

        // "ColourOrder":"RGBW",
        // "BusType":"SK6812_RGBW",
  #define STRIP_SIZE_MAX 100
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":3,
        "ColourOrder":"GRBW",
        "BusType":"SK6812_RGBW",
        "Start":0,
        "Length":100
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        100
      ],
      "ColourPalette":0,
      "SegColour0": {
        "Hue": 330,
        "Sat":100,
        "BrightnessRGB":100
      },
      "Effects": {
        "Function": 0,
        "Speed":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 100
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 100
  }
  )=====";

#endif // DEVICE_TESTGROUP__LIGHTING_EFFECTS__L3__ESP8266_1CH_RGB_100LEDS














































































// define all shared params here, and only device related in "subdevice"
#ifdef DEVICE_TESTGROUP__LIGHTING_MULTIPIN

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID


  // #define ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE
  #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS



  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES


  //mqtt debug
  #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
  // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // #define DISABLE_NETWORK
  // #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_WIFI
  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  #define ANIMATION_UPDATOR_TIME_MINIMUM 20

  #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  // #undef ESP32
  // #define ESP8266

  

  #define ENABLE_DEBUG_MANUAL_DELAYS

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_ANIMATOR
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
    #define DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
    #define ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT
    #define ENABLE_DEVFEATURE_SWITCH_TO_U16_GPIO_FUNCTIONS
    #define ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
    #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    #define ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
    /********* Group: Debug options only ************************/
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
    #define ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
    #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
    #define ENABLE_DEBUG_SERIAL

    // Needs significant change to merge WLED and HACS
    #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
    // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

    #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG

    #define ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL

    // #define ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

    // #define ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
    // #define ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
    

    
    // #define ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32

    #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD



// #define ENABLE_DEVFEATURE_LIGHT__PERMIT_PIXEL_INDEXING_GREATER_THAN_FIRST_ON_PWM_CHANNELS_FOR_MULTIPLE_SEGMENTS

// #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28



    // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    // #define ENABLE_LOG_LEVEL_DEBUG
    // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

    
    

    // #define ENABLE_DEVFEATURE_LIGHT__PRELOAD_BUSCONFIG_FROM_TEMPLATE_AS_TEMPORARY_MEASURE
    #define ENABLE_DEVFEATURE_LIGHT__MOVE_ALL_BUS_STARTING_CODE_UNTIL_LOOP



#endif // DEVICE_TESTGROUP__LIGHTING_MULTIPIN


#ifdef SUBDEVICE_TESTGROUP__LIGHTING_MULTIPIN__H801_AS_RGB_AND_CCT_SPLIT_CHANNELS2
  #define DEVICENAME_CTR          "testgroup_multipin_h801_rgbsplitcct"
  #define DEVICENAME_FRIENDLY_CTR "TestGroup H801 RGB Split CCT Channels"
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883


//   #define USE_SERIAL_ALTERNATE_TX
//   #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
//   // #define USE_MODULE_DRIVERS_PWM


  

//     


// // Step A: Replicate WLED to function as it was designed on the webui part. Only when this works for effect/colour controls etc, slowly change it to add more of my original stuff. 
// // Gain complete understanding/implemenation as is, only then start changes. 
// // WebUI of WLED style should become its own webserver option (not just the debug version).

//   #define ENABLE_FEATURE_WATCHDOG_TIMER
//   #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
//   #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
//   #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

//   #define USE_MODULE_NETWORK_WEBSERVER23
//   #define USE_MODULE_NETWORK_WEBSERVER23

//   // #define ESP32
//   // #undef ESP8266

//   // #define ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE
//   #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS

//   // #define ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
//   // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL

//   // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_TESTER
//   // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_DEVELOPER

//   


//   // #define ENABLE_ADVANCED_DEBUGGING
//   // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
//   // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
//   // #define ENABLE_DEBUG_FUNCTION_NAMES


//   //mqtt debug
//   #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
//   // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

//   // #define DISABLE_NETWORK
//   // #define DISABLE_NETWORK_WIFI
//   #define USE_MODULE_NETWORK_WIFI
//   #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

//   #define ANIMATION_UPDATOR_TIME_MINIMUM 20

//   #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR

//   // #define DISABLE_SERIAL
//   // #define DISABLE_SERIAL0_CORE
//   // #define DISABLE_SERIAL_LOGGING

//   // #undef ESP32
//   // #define ESP8266

//   

//   #define ENABLE_DEBUG_MANUAL_DELAYS

//   #define ENABLE_FEATURE_WATCHDOG_TIMER
//   #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
//   // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
//   // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR
//   #define USE_MODULE_LIGHTS_ADDRESSABLE
//     #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//     /********* Group: Needed to build ************************/
//     #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
//     /********* Group: Ready for full integration ************************/
//     // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
//     /********* Group: Testing ************************/
//     #define ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD
//     #define ENABLE_DEVFEATURE_REMOVE_INIT_OUTSIDE_OF_PALETTE_CLASS
//     #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
//     #define ENABLE_DEVFEATURE_UNNEEDED_WLED_ONLY_PARAMETERS
//     #define ENABLE_DEVFEATURE_ALWAYS_LOAD_PALETTE_WHEN_NOT_TRANSITIONING
//     #define DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
//     #define ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT
//     #define ENABLE_DEVFEATURE_SWITCH_TO_U16_GPIO_FUNCTIONS
//     #define ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
//     #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
//     #define ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
//     // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
//     /********* Group: Debug options only ************************/
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
//     #define ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
//     #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
//     #define ENABLE_DEBUG_SERIAL

//     #define ENABLE_DEVFEATURE_LIGHT__WEBUI_APPEND_EFFECT_CONFIG_TO_JSON_RESPONSE

//     // Needs significant change to merge WLED and HACS
//     #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
//     // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

//     #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG

//     #define ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL

//     #define ENABLE_DEVFEATURE_LIGHT__WLED_WEBUI_SEND_MY_PALETTE_COLOUR_BARS

//     // #define ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

//     // #define ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
//     // #define ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
    
//     #define ENABLE_DEBUG_LINE_HERE
//     #define ENABLE_DEVFEATURE_PALETTE__FIX_WEBUI_GRADIENT_PREVIEW

//     #define ENABLE_DEVFEATURE_LIGHT__HYPERION
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE
//     // #define ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32

//     #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD


//     #define ENABLE_WEBSERVER_LIGHTING_WEBUI

//     // #define ENABLE_DEVFEATURE_WEBUI__INCLUDE_URI_PRE2023



//     // #define ENABLE_DEVFEATURE_LIGHT__PERMIT_PIXEL_INDEXING_GREATER_THAN_FIRST_ON_PWM_CHANNELS_FOR_MULTIPLE_SEGMENTS

//     // #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28



//     // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
//     // #define ENABLE_LOG_LEVEL_DEBUG
//     // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
//     // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

//     
    

//     // #define ENABLE_DEVFEATURE_LIGHT__PRELOAD_BUSCONFIG_FROM_TEMPLATE_AS_TEMPORARY_MEASURE
//     #define ENABLE_DEVFEATURE_LIGHT__MOVE_ALL_BUS_STARTING_CODE_UNTIL_LOOP



  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{" 
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":15,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_B_CTR "\":13,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_C_CTR "\":12,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_D_CTR "\":14,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_E_CTR "\":4"  // PWM RGBCCT
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_H801_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":1},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";


  #define USE_LIGHTING_TEMPLATE
  #define USE_LIGHTING_TEMPLATE__H801_SPLIT_RGB_WHITE_CHANNELS_TWO_SEGMENTS

  #ifdef USE_LIGHTING_TEMPLATE__H801_SPLIT_RGB_WHITE_CHANNELS_TWO_SEGMENTS
    #define STRIP_SIZE_MAX 2
    #define ENABLE_DEVFEATURE_LIGHT__BUS_MANAGER_DEFAULT_FORCED_AS_PWM
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":[15,13,12],
          "ColourOrder":"RGB",
          "BusType":"ANALOG_3CH",
          "Start":0,
          "Length":1
        },
        {
          "Pin":[14,4],
          "ColourOrder":"CW",
          "BusType":"ANALOG_2CH",
          "Start":1,
          "Length":1
        }
      ],    
      "Segment0":{
        "PixelRange": [
          0,
          1
        ],
        "ColourPalette":10,
        "SegColour0": {
          "Hue": 25,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":90,
          "ColourType":4
        },
        "Effects": {
          "Function":0
        },
        "Transition": {
          "TimeMs": 0,
          "RateMs": 1000
        },
        "BrightnessRGB":100,
        "BrightnessCCT":100
      },      
      "Segment1":{
        "PixelRange": [
          1,
          2
        ],
        "ColourPalette":10,
        "SegColour0": {
          "Hue": 0,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":50,
          "ColourType":4
        },
        "Effects": {
          "Function":0
        },
        "Transition": {
          "TimeMs": 0,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 100
      },
      "BrightnessRGB":10,
      "BrightnessCCT":10
    }
    )=====";

    /*

    {
      "Segment0": {
        "PixelRange": [
          0,
          5
        ],
        "ColourPalette": 10,
        "SegColour0": {
          "Hue": 240,
          "Sat": 0,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage": 100,
          "ColourType": 4
        },
        "Effects": {
          "Function": 0
        },
        "Transition": {
          "TimeMs":0,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT_255": 255,
        "TimeOnSecs":10,
        "Override":{"Animation_Off":{"TimeMs":10000}}
      },
      "BrightnessRGB":100,
      "BrightnessCCT": 100
    }

    */

  #endif // USE_LIGHTING_TEMPLATE__H801_FIVE_WHITE_CHANNELS_FIVE_SEGMENTS


#endif // SUBDEVICE_TESTGROUP__LIGHTING_MULTIPIN__H801_AS_RGB_AND_CCT_SPLIT_CHANNELS


#ifdef SUBDEVICE_TESTGROUP__LIGHTING_MULTIPIN__H801_AS_FIVE_SINGLE_WHITE_CHANNELS
  #define DEVICENAME_CTR          "testgroup_multipin_h801_5white"
  #define DEVICENAME_FRIENDLY_CTR "TestGroup H801 Five White Channels"
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883


  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  // #define USE_MODULE_DRIVERS_PWM


  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_AUGUST_2023
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{" 
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":15,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_B_CTR "\":13,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_C_CTR "\":12,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_D_CTR "\":14,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_E_CTR "\":4"  // PWM RGBCCT
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_H801_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":1},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";


  #define USE_LIGHTING_TEMPLATE
  #define USE_LIGHTING_TEMPLATE__H801_FIVE_WHITE_CHANNELS_FIVE_SEGMENTS

  #ifdef USE_LIGHTING_TEMPLATE__H801_FIVE_WHITE_CHANNELS_FIVE_SEGMENTS
    #define STRIP_SIZE_MAX 5
    #define ENABLE_DEVFEATURE_LIGHT__BUS_MANAGER_DEFAULT_FORCED_AS_PWM
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":[15],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":0,
          "Length":1
        },
        {
          "Pin":[13],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":1,
          "Length":1
        },
        {
          "Pin":[14],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":2,
          "Length":1
        },
        {
          "Pin":[4],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":3,
          "Length":1
        },
        {
          "Pin":[12],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":4,
          "Length":1
        }
      ],      
      "Segment0":{
        "PixelRange": [
          0,
          2
        ],
        "ColourPalette":10,
        "SegColour0": {
          "Hue": 25,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":50,
          "ColourType":4
        },
        "Effects": {
          "Function":0
        },
        "Transition": {
          "TimeMs": 0,
          "RateMs": 1000
        },
        "BrightnessRGB":100,
        "BrightnessCCT":100
      },      
      "Segment1":{
        "PixelRange": [
          2,
          5
        ],
        "ColourPalette":10,
        "SegColour0": {
          "Hue": 0,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":50,
          "ColourType":4
        },
        "Effects": {
          "Function":0
        },
        "Transition": {
          "TimeMs": 0,
          "RateMs": 1000
        },
        "BrightnessRGB":100,
        "BrightnessCCT":100
      },
      "BrightnessRGB":100,
      "BrightnessCCT":1
    }
    )=====";

    /*

    {
      "Segment0": {
        "PixelRange": [
          0,
          5
        ],
        "ColourPalette": 10,
        "SegColour0": {
          "Hue": 240,
          "Sat": 0,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage": 100,
          "ColourType": 4
        },
        "Effects": {
          "Function": 0
        },
        "Transition": {
          "TimeMs":0,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT_255": 255,
        "TimeOnSecs":10,
        "Override":{"Animation_Off":{"TimeMs":10000}}
      },
      "BrightnessRGB":100,
      "BrightnessCCT": 100
    }

    */

  #endif // USE_LIGHTING_TEMPLATE__H801_FIVE_WHITE_CHANNELS_FIVE_SEGMENTS


#endif // DEVICE_TESTGROUP__LIGHTING_MULTIPIN__H801_AS_FIVE_SINGLE_WHITE_CHANNELS






#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__L4__H801_5CH_PWM_RGBCCT
  #ifndef DEVICENAME_CTR
    #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883


  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  // #define USE_MODULE_DRIVERS_PWM

#define USE_SERIAL_ALTERNATE_TX
  

  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES
  
  // #define USE_MODULE_NETWORK_WEBSERVER23
  // #define USE_MODULE_NETWORK_WEBSERVER23
    // #define ENABLE_WEBSERVER_LIGHTING_WEBUI

  // #define ESP8266
  // #undef ESP32
  

  


    



  // #define USE_MODULE_NETWORK_WEBSERVER23
  // #define USE_MODULE_NETWORK_WEBSERVER23

  // #define ESP32
  // #undef ESP8266

  // #define ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE
  #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS

  // #define ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
  // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL

  // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_TESTER
  // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_DEVELOPER

  


  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES


  //mqtt debug
  #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
  // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // #define DISABLE_NETWORK
  // #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_WIFI
  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  #define ANIMATION_UPDATOR_TIME_MINIMUM 20

  #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  // #undef ESP32
  // #define ESP8266

  

  #define ENABLE_DEBUG_MANUAL_DELAYS

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
    #define DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
    #define ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT
    #define ENABLE_DEVFEATURE_SWITCH_TO_U16_GPIO_FUNCTIONS
    #define ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
    #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    #define ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
    /********* Group: Debug options only ************************/
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
    #define ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
    #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
    #define ENABLE_DEBUG_SERIAL

    #define ENABLE_DEVFEATURE_LIGHT__WEBUI_APPEND_EFFECT_CONFIG_TO_JSON_RESPONSE

    // Needs significant change to merge WLED and HACS
    #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
    // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

    #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG

    #define ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL

    #define ENABLE_DEVFEATURE_LIGHT__WLED_WEBUI_SEND_MY_PALETTE_COLOUR_BARS

    // #define ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

    // #define ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
    // #define ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
    
    #define ENABLE_DEBUG_LINE_HERE
    #define ENABLE_DEVFEATURE_PALETTE__FIX_WEBUI_GRADIENT_PREVIEW

    #define ENABLE_DEVFEATURE_LIGHT__HYPERION
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE
    // #define ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32

    #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD



    // #define ENABLE_DEVFEATURE_WEBUI__INCLUDE_URI_PRE2023



    // #define ENABLE_DEVFEATURE_LIGHT__PERMIT_PIXEL_INDEXING_GREATER_THAN_FIRST_ON_PWM_CHANNELS_FOR_MULTIPLE_SEGMENTS

    // #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28



    // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    // #define ENABLE_LOG_LEVEL_DEBUG
    // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

    
    

    // #define ENABLE_DEVFEATURE_LIGHT__PRELOAD_BUSCONFIG_FROM_TEMPLATE_AS_TEMPORARY_MEASURE
    #define ENABLE_DEVFEATURE_LIGHT__MOVE_ALL_BUS_STARTING_CODE_UNTIL_LOOP



//     


// // Step A: Replicate WLED to function as it was designed on the webui part. Only when this works for effect/colour controls etc, slowly change it to add more of my original stuff. 
// // Gain complete understanding/implemenation as is, only then start changes. 
// // WebUI of WLED style should become its own webserver option (not just the debug version).

//   #define ENABLE_FEATURE_WATCHDOG_TIMER
//   #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
//   #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
//   #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

//   #define USE_MODULE_NETWORK_WEBSERVER23
//   #define USE_MODULE_NETWORK_WEBSERVER23

//   // #define ESP32
//   // #undef ESP8266

//   // #define ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE
//   #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS

//   // #define ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
//   // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL

//   // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_TESTER
//   // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_DEVELOPER

//   


//   // #define ENABLE_ADVANCED_DEBUGGING
//   // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
//   // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
//   // #define ENABLE_DEBUG_FUNCTION_NAMES


//   //mqtt debug
//   #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
//   // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

//   // #define DISABLE_NETWORK
//   // #define DISABLE_NETWORK_WIFI
//   #define USE_MODULE_NETWORK_WIFI
//   #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

//   #define ANIMATION_UPDATOR_TIME_MINIMUM 20

//   #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR

//   // #define DISABLE_SERIAL
//   // #define DISABLE_SERIAL0_CORE
//   // #define DISABLE_SERIAL_LOGGING

//   // #undef ESP32
//   // #define ESP8266

//   

//   #define ENABLE_DEBUG_MANUAL_DELAYS

//   #define ENABLE_FEATURE_WATCHDOG_TIMER
//   #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
//   // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
//   // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR
//   #define USE_MODULE_LIGHTS_ADDRESSABLE
//     #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//     /********* Group: Needed to build ************************/
//     #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
//     /********* Group: Ready for full integration ************************/
//     // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
//     /********* Group: Testing ************************/
//     #define ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD
//     #define ENABLE_DEVFEATURE_REMOVE_INIT_OUTSIDE_OF_PALETTE_CLASS
//     #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
//     #define ENABLE_DEVFEATURE_UNNEEDED_WLED_ONLY_PARAMETERS
//     #define ENABLE_DEVFEATURE_ALWAYS_LOAD_PALETTE_WHEN_NOT_TRANSITIONING
//     #define DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
//     #define ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT
//     #define ENABLE_DEVFEATURE_SWITCH_TO_U16_GPIO_FUNCTIONS
//     #define ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
//     #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
//     #define ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
//     // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
//     /********* Group: Debug options only ************************/
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
//     #define ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
//     #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
//     #define ENABLE_DEBUG_SERIAL

//     #define ENABLE_DEVFEATURE_LIGHT__WEBUI_APPEND_EFFECT_CONFIG_TO_JSON_RESPONSE

//     // Needs significant change to merge WLED and HACS
//     #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
//     // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

//     #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG

//     #define ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL

//     #define ENABLE_DEVFEATURE_LIGHT__WLED_WEBUI_SEND_MY_PALETTE_COLOUR_BARS

//     // #define ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

//     // #define ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
//     // #define ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
    
//     #define ENABLE_DEBUG_LINE_HERE
//     #define ENABLE_DEVFEATURE_PALETTE__FIX_WEBUI_GRADIENT_PREVIEW

//     #define ENABLE_DEVFEATURE_LIGHT__HYPERION
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE
//     // #define ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32

//     #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD


//     #define ENABLE_WEBSERVER_LIGHTING_WEBUI

//     // #define ENABLE_DEVFEATURE_WEBUI__INCLUDE_URI_PRE2023



//     // #define ENABLE_DEVFEATURE_LIGHT__PERMIT_PIXEL_INDEXING_GREATER_THAN_FIRST_ON_PWM_CHANNELS_FOR_MULTIPLE_SEGMENTS

//     // #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28



//     // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
//     // #define ENABLE_LOG_LEVEL_DEBUG
//     // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
//     // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

//     
    

//     // #define ENABLE_DEVFEATURE_LIGHT__PRELOAD_BUSCONFIG_FROM_TEMPLATE_AS_TEMPORARY_MEASURE
//     #define ENABLE_DEVFEATURE_LIGHT__MOVE_ALL_BUS_STARTING_CODE_UNTIL_LOOP


  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_AUGUST_2023

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{" 
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":15,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_B_CTR "\":13,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_C_CTR "\":12,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_D_CTR "\":14,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_E_CTR "\":4"    // PWM RGBCCT
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_H801_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":1},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";


  #define USE_LIGHTING_TEMPLATE
  #define STRIP_SIZE_MAX 2
  #define ENABLE_DEVFEATURE_LIGHT__BUS_MANAGER_DEFAULT_FORCED_AS_PWM
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":[15,13,12,14,4],
        "ColourOrder":"RGBCW",
        "BusType":"ANALOG_5CH",
        "Start":0,
        "Length":1
      }
    ],    
    "Segment0":{
      "PixelRange": [
        0,
        1
      ],
      "ColourPalette":10,
      "SegColour0": {
        "Hue": 0,
        "Sat": 100,
        "BrightnessRGB": 100,
        "BrightnessCCT": 100,
        "CCT_TempPercentage":90,
        "ColourType":4
      },
      "Effects": {
        "Function":0
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 1000
      },
      "BrightnessRGB":100,
      "BrightnessCCT":100
    },
    "BrightnessRGB":10,
    "BrightnessCCT":10
  }
  )=====";


#endif // DEVICE_TESTGROUP__LIGHTING_EFFECTS__L4__H801_5CH_PWM_RGBCCT








#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__L5__H801_3CHRGB_AND_2CHCW
  #ifndef DEVICENAME_CTR
    #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883


  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  // #define USE_MODULE_DRIVERS_PWM

#define USE_SERIAL_ALTERNATE_TX
  

  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES
  
  // #define USE_MODULE_NETWORK_WEBSERVER23
  // #define USE_MODULE_NETWORK_WEBSERVER23
    // #define ENABLE_WEBSERVER_LIGHTING_WEBUI

  // #define ESP8266
  // #undef ESP32
  

  


    



  // #define USE_MODULE_NETWORK_WEBSERVER23
  // #define USE_MODULE_NETWORK_WEBSERVER23

  // #define ESP32
  // #undef ESP8266

  // #define ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE
  #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS

  // #define ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
  // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL

  // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_TESTER
  // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_DEVELOPER

  


  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES


  //mqtt debug
  #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
  // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // #define DISABLE_NETWORK
  // #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_WIFI
  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  #define ANIMATION_UPDATOR_TIME_MINIMUM 20

  #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  // #undef ESP32
  // #define ESP8266

  

  #define ENABLE_DEBUG_MANUAL_DELAYS

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
    #define DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
    #define ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT
    #define ENABLE_DEVFEATURE_SWITCH_TO_U16_GPIO_FUNCTIONS
    #define ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
    #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    #define ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
    /********* Group: Debug options only ************************/
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
    #define ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
    #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
    #define ENABLE_DEBUG_SERIAL

    #define ENABLE_DEVFEATURE_LIGHT__WEBUI_APPEND_EFFECT_CONFIG_TO_JSON_RESPONSE

    // Needs significant change to merge WLED and HACS
    #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
    // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

    #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG

    #define ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL

    #define ENABLE_DEVFEATURE_LIGHT__WLED_WEBUI_SEND_MY_PALETTE_COLOUR_BARS

    // #define ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

    // #define ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
    // #define ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
    
    #define ENABLE_DEBUG_LINE_HERE
    #define ENABLE_DEVFEATURE_PALETTE__FIX_WEBUI_GRADIENT_PREVIEW

    #define ENABLE_DEVFEATURE_LIGHT__HYPERION
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE
    // #define ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32

    #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD



    // #define ENABLE_DEVFEATURE_WEBUI__INCLUDE_URI_PRE2023



    // #define ENABLE_DEVFEATURE_LIGHT__PERMIT_PIXEL_INDEXING_GREATER_THAN_FIRST_ON_PWM_CHANNELS_FOR_MULTIPLE_SEGMENTS

    // #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28



    // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    // #define ENABLE_LOG_LEVEL_DEBUG
    // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

    
    

    // #define ENABLE_DEVFEATURE_LIGHT__PRELOAD_BUSCONFIG_FROM_TEMPLATE_AS_TEMPORARY_MEASURE
    #define ENABLE_DEVFEATURE_LIGHT__MOVE_ALL_BUS_STARTING_CODE_UNTIL_LOOP



//   #define USE_SERIAL_ALTERNATE_TX
//   #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
//   // #define USE_MODULE_DRIVERS_PWM


  

//     


// // Step A: Replicate WLED to function as it was designed on the webui part. Only when this works for effect/colour controls etc, slowly change it to add more of my original stuff. 
// // Gain complete understanding/implemenation as is, only then start changes. 
// // WebUI of WLED style should become its own webserver option (not just the debug version).

//   #define ENABLE_FEATURE_WATCHDOG_TIMER
//   #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
//   #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
//   #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

//   #define USE_MODULE_NETWORK_WEBSERVER23
//   #define USE_MODULE_NETWORK_WEBSERVER23

//   // #define ESP32
//   // #undef ESP8266

//   // #define ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE
//   #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS

//   // #define ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
//   // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL

//   // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_TESTER
//   // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_DEVELOPER

//   


//   // #define ENABLE_ADVANCED_DEBUGGING
//   // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
//   // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
//   // #define ENABLE_DEBUG_FUNCTION_NAMES


//   //mqtt debug
//   #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
//   // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

//   // #define DISABLE_NETWORK
//   // #define DISABLE_NETWORK_WIFI
//   #define USE_MODULE_NETWORK_WIFI
//   #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

//   #define ANIMATION_UPDATOR_TIME_MINIMUM 20

//   #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR

//   // #define DISABLE_SERIAL
//   // #define DISABLE_SERIAL0_CORE
//   // #define DISABLE_SERIAL_LOGGING

//   // #undef ESP32
//   // #define ESP8266

//   

//   #define ENABLE_DEBUG_MANUAL_DELAYS

//   #define ENABLE_FEATURE_WATCHDOG_TIMER
//   #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
//   // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
//   // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR
//   #define USE_MODULE_LIGHTS_ADDRESSABLE
//     #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//     /********* Group: Needed to build ************************/
//     #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
//     /********* Group: Ready for full integration ************************/
//     // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
//     /********* Group: Testing ************************/
//     #define ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD
//     #define ENABLE_DEVFEATURE_REMOVE_INIT_OUTSIDE_OF_PALETTE_CLASS
//     #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
//     #define ENABLE_DEVFEATURE_UNNEEDED_WLED_ONLY_PARAMETERS
//     #define ENABLE_DEVFEATURE_ALWAYS_LOAD_PALETTE_WHEN_NOT_TRANSITIONING
//     #define DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
//     #define ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT
//     #define ENABLE_DEVFEATURE_SWITCH_TO_U16_GPIO_FUNCTIONS
//     #define ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
//     #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
//     #define ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
//     // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
//     /********* Group: Debug options only ************************/
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
//     #define ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
//     #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
//     #define ENABLE_DEBUG_SERIAL

//     #define ENABLE_DEVFEATURE_LIGHT__WEBUI_APPEND_EFFECT_CONFIG_TO_JSON_RESPONSE

//     // Needs significant change to merge WLED and HACS
//     #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
//     // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

//     #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG

//     #define ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL

//     #define ENABLE_DEVFEATURE_LIGHT__WLED_WEBUI_SEND_MY_PALETTE_COLOUR_BARS

//     // #define ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

//     // #define ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
//     // #define ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
    
//     #define ENABLE_DEBUG_LINE_HERE
//     #define ENABLE_DEVFEATURE_PALETTE__FIX_WEBUI_GRADIENT_PREVIEW

//     #define ENABLE_DEVFEATURE_LIGHT__HYPERION
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE
//     // #define ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32

//     #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD


//     #define ENABLE_WEBSERVER_LIGHTING_WEBUI

//     // #define ENABLE_DEVFEATURE_WEBUI__INCLUDE_URI_PRE2023



//     // #define ENABLE_DEVFEATURE_LIGHT__PERMIT_PIXEL_INDEXING_GREATER_THAN_FIRST_ON_PWM_CHANNELS_FOR_MULTIPLE_SEGMENTS

//     // #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28



//     // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
//     // #define ENABLE_LOG_LEVEL_DEBUG
//     // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
//     // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

//     
    

//     // #define ENABLE_DEVFEATURE_LIGHT__PRELOAD_BUSCONFIG_FROM_TEMPLATE_AS_TEMPORARY_MEASURE
//     #define ENABLE_DEVFEATURE_LIGHT__MOVE_ALL_BUS_STARTING_CODE_UNTIL_LOOP

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_AUGUST_2023


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{" 
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":15,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_B_CTR "\":13,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_C_CTR "\":12,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_D_CTR "\":14,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_E_CTR "\":4"  // PWM RGBCCT
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_H801_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":1},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";


  #define USE_LIGHTING_TEMPLATE
  #define USE_LIGHTING_TEMPLATE__H801_SPLIT_RGB_WHITE_CHANNELS_TWO_SEGMENTS

  #ifdef USE_LIGHTING_TEMPLATE__H801_SPLIT_RGB_WHITE_CHANNELS_TWO_SEGMENTS
    #define STRIP_SIZE_MAX 2
    #define ENABLE_DEVFEATURE_LIGHT__BUS_MANAGER_DEFAULT_FORCED_AS_PWM
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":[15,13,12],
          "ColourOrder":"RGB",
          "BusType":"ANALOG_3CH",
          "Start":0,
          "Length":1
        },
        {
          "Pin":[14,4],
          "ColourOrder":"CW",
          "BusType":"ANALOG_2CH",
          "Start":1,
          "Length":1
        }
      ],    
      "Segment0":{
        "PixelRange": [
          0,
          1
        ],
        "ColourPalette":10,
        "SegColour0": {
          "Hue": 25,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":90,
          "ColourType":4
        },
        "Effects": {
          "Function":0
        },
        "Transition": {
          "TimeMs": 0,
          "RateMs": 1000
        },
        "BrightnessRGB":100,
        "BrightnessCCT":100
      },      
      "Segment1":{
        "PixelRange": [
          1,
          2
        ],
        "ColourPalette":10,
        "SegColour0": {
          "Hue": 0,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":50,
          "ColourType":4
        },
        "Effects": {
          "Function":0
        },
        "Transition": {
          "TimeMs": 0,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 100
      },
      "BrightnessRGB":10,
      "BrightnessCCT":10
    }
    )=====";

    /*

    {
      "Segment0": {
        "PixelRange": [
          0,
          5
        ],
        "ColourPalette": 10,
        "SegColour0": {
          "Hue": 240,
          "Sat": 0,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage": 100,
          "ColourType": 4
        },
        "Effects": {
          "Function": 0
        },
        "Transition": {
          "TimeMs":0,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT_255": 255,
        "TimeOnSecs":10,
        "Override":{"Animation_Off":{"TimeMs":10000}}
      },
      "BrightnessRGB":100,
      "BrightnessCCT": 100
    }

    */

  #endif // USE_LIGHTING_TEMPLATE__H801_FIVE_WHITE_CHANNELS_FIVE_SEGMENTS


#endif // DEVICE_TESTGROUP__LIGHTING_EFFECTS__L5__H801_3CHRGB_AND_2CHCW
















#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__L6__H801_FIVE_1CH_WHITE_CHANNELS
  #ifndef DEVICENAME_CTR
    #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883


  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  // #define USE_MODULE_DRIVERS_PWM

#define USE_SERIAL_ALTERNATE_TX
  

  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES
  
  // #define USE_MODULE_NETWORK_WEBSERVER23
  // #define USE_MODULE_NETWORK_WEBSERVER23
    // #define ENABLE_WEBSERVER_LIGHTING_WEBUI

  // #define ESP8266
  // #undef ESP32
  

  


    



  // #define USE_MODULE_NETWORK_WEBSERVER23
  // #define USE_MODULE_NETWORK_WEBSERVER23

  // #define ESP32
  // #undef ESP8266

  // #define ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE
  #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS

  // #define ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
  // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL

  // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_TESTER
  // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_DEVELOPER

  


  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES


  //mqtt debug
  #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
  // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // #define DISABLE_NETWORK
  // #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_WIFI
  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  #define ANIMATION_UPDATOR_TIME_MINIMUM 20

  #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  // #undef ESP32
  // #define ESP8266

  

  #define ENABLE_DEBUG_MANUAL_DELAYS

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
    #define DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
    #define ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT
    #define ENABLE_DEVFEATURE_SWITCH_TO_U16_GPIO_FUNCTIONS
    #define ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
    #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    #define ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
    /********* Group: Debug options only ************************/
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
    #define ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
    #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
    #define ENABLE_DEBUG_SERIAL

    #define ENABLE_DEVFEATURE_LIGHT__WEBUI_APPEND_EFFECT_CONFIG_TO_JSON_RESPONSE

    // Needs significant change to merge WLED and HACS
    #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
    // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

    #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG

    #define ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL

    #define ENABLE_DEVFEATURE_LIGHT__WLED_WEBUI_SEND_MY_PALETTE_COLOUR_BARS

    // #define ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

    // #define ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
    // #define ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
    
    #define ENABLE_DEBUG_LINE_HERE
    #define ENABLE_DEVFEATURE_PALETTE__FIX_WEBUI_GRADIENT_PREVIEW

    #define ENABLE_DEVFEATURE_LIGHT__HYPERION
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE
    // #define ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32

    #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD



    // #define ENABLE_DEVFEATURE_WEBUI__INCLUDE_URI_PRE2023



    // #define ENABLE_DEVFEATURE_LIGHT__PERMIT_PIXEL_INDEXING_GREATER_THAN_FIRST_ON_PWM_CHANNELS_FOR_MULTIPLE_SEGMENTS

    // #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28



    // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    // #define ENABLE_LOG_LEVEL_DEBUG
    // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

    
    

    // #define ENABLE_DEVFEATURE_LIGHT__PRELOAD_BUSCONFIG_FROM_TEMPLATE_AS_TEMPORARY_MEASURE
    #define ENABLE_DEVFEATURE_LIGHT__MOVE_ALL_BUS_STARTING_CODE_UNTIL_LOOP



//   #define USE_SERIAL_ALTERNATE_TX
//   #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
//   // #define USE_MODULE_DRIVERS_PWM


  

//     


// // Step A: Replicate WLED to function as it was designed on the webui part. Only when this works for effect/colour controls etc, slowly change it to add more of my original stuff. 
// // Gain complete understanding/implemenation as is, only then start changes. 
// // WebUI of WLED style should become its own webserver option (not just the debug version).

//   #define ENABLE_FEATURE_WATCHDOG_TIMER
//   #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
//   #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
//   #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

//   #define USE_MODULE_NETWORK_WEBSERVER23
//   #define USE_MODULE_NETWORK_WEBSERVER23

//   // #define ESP32
//   // #undef ESP8266

//   // #define ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE
//   #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS

//   // #define ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
//   // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL

//   // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_TESTER
//   // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_DEVELOPER

//   


//   // #define ENABLE_ADVANCED_DEBUGGING
//   // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
//   // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
//   // #define ENABLE_DEBUG_FUNCTION_NAMES


//   //mqtt debug
//   #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
//   // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

//   // #define DISABLE_NETWORK
//   // #define DISABLE_NETWORK_WIFI
//   #define USE_MODULE_NETWORK_WIFI
//   #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

//   #define ANIMATION_UPDATOR_TIME_MINIMUM 20

//   #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR

//   // #define DISABLE_SERIAL
//   // #define DISABLE_SERIAL0_CORE
//   // #define DISABLE_SERIAL_LOGGING

//   // #undef ESP32
//   // #define ESP8266

//   

//   #define ENABLE_DEBUG_MANUAL_DELAYS

//   #define ENABLE_FEATURE_WATCHDOG_TIMER
//   #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
//   // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
//   // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR
//   #define USE_MODULE_LIGHTS_ADDRESSABLE
//     #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//     /********* Group: Needed to build ************************/
//     #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
//     /********* Group: Ready for full integration ************************/
//     // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
//     /********* Group: Testing ************************/
//     #define ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD
//     #define ENABLE_DEVFEATURE_REMOVE_INIT_OUTSIDE_OF_PALETTE_CLASS
//     #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
//     #define ENABLE_DEVFEATURE_UNNEEDED_WLED_ONLY_PARAMETERS
//     #define ENABLE_DEVFEATURE_ALWAYS_LOAD_PALETTE_WHEN_NOT_TRANSITIONING
//     #define DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
//     #define ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT
//     #define ENABLE_DEVFEATURE_SWITCH_TO_U16_GPIO_FUNCTIONS
//     #define ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
//     #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
//     #define ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
//     // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
//     /********* Group: Debug options only ************************/
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
//     #define ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
//     #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
//     #define ENABLE_DEBUG_SERIAL

//     #define ENABLE_DEVFEATURE_LIGHT__WEBUI_APPEND_EFFECT_CONFIG_TO_JSON_RESPONSE

//     // Needs significant change to merge WLED and HACS
//     #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
//     // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

//     #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG

//     #define ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL

//     #define ENABLE_DEVFEATURE_LIGHT__WLED_WEBUI_SEND_MY_PALETTE_COLOUR_BARS

//     // #define ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

//     // #define ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
//     // #define ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
    
//     #define ENABLE_DEBUG_LINE_HERE
//     #define ENABLE_DEVFEATURE_PALETTE__FIX_WEBUI_GRADIENT_PREVIEW

//     #define ENABLE_DEVFEATURE_LIGHT__HYPERION
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE
//     // #define ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32

//     #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD


//     #define ENABLE_WEBSERVER_LIGHTING_WEBUI

//     // #define ENABLE_DEVFEATURE_WEBUI__INCLUDE_URI_PRE2023



//     // #define ENABLE_DEVFEATURE_LIGHT__PERMIT_PIXEL_INDEXING_GREATER_THAN_FIRST_ON_PWM_CHANNELS_FOR_MULTIPLE_SEGMENTS

//     // #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28



//     // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
//     // #define ENABLE_LOG_LEVEL_DEBUG
//     // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
//     // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

//     
    

//     // #define ENABLE_DEVFEATURE_LIGHT__PRELOAD_BUSCONFIG_FROM_TEMPLATE_AS_TEMPORARY_MEASURE
//     #define ENABLE_DEVFEATURE_LIGHT__MOVE_ALL_BUS_STARTING_CODE_UNTIL_LOOP

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_AUGUST_2023


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{" 
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":15,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_B_CTR "\":13,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_C_CTR "\":12,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_D_CTR "\":14,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_E_CTR "\":4"  // PWM RGBCCT
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_H801_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":1},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";



  #define USE_LIGHTING_TEMPLATE
  #define USE_LIGHTING_TEMPLATE__H801_FIVE_WHITE_CHANNELS_FIVE_SEGMENTS

  #ifdef USE_LIGHTING_TEMPLATE__H801_FIVE_WHITE_CHANNELS_FIVE_SEGMENTS
    #define STRIP_SIZE_MAX 5
    #define ENABLE_DEVFEATURE_LIGHT__BUS_MANAGER_DEFAULT_FORCED_AS_PWM
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":[15],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":0,
          "Length":1
        },
        {
          "Pin":[13],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":1,
          "Length":1
        },
        {
          "Pin":[14],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":2,
          "Length":1
        },
        {
          "Pin":[4],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":3,
          "Length":1
        },
        {
          "Pin":[12],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":4,
          "Length":1
        }
      ],      
      "Segment0":{
        "PixelRange": [
          0,
          2
        ],
        "ColourPalette":10,
        "SegColour0": {
          "Hue": 25,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":50,
          "ColourType":4
        },
        "Effects": {
          "Function":0
        },
        "Transition": {
          "TimeMs": 0,
          "RateMs": 1000
        },
        "BrightnessRGB":100,
        "BrightnessCCT":100
      },      
      "Segment1":{
        "PixelRange": [
          2,
          5
        ],
        "ColourPalette":10,
        "SegColour0": {
          "Hue": 0,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":50,
          "ColourType":4
        },
        "Effects": {
          "Function":0
        },
        "Transition": {
          "TimeMs": 0,
          "RateMs": 1000
        },
        "BrightnessRGB":100,
        "BrightnessCCT":100
      },
      "BrightnessRGB":100,
      "BrightnessCCT":1
    }
    )=====";


#endif // DEVICE_TESTGROUP__LIGHTING_EFFECTS__L6__H801_FIVE_1CH_WHITE_CHANNELS

#endif


/**
 * @brief 
 * Primary testgroup for most addresable light cases
 */
#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__L7__ESP32_ROOM_CEILING_100LEDS
  #define DEVICENAME_CTR          "testgroup_lighting_L7"
  #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L7n8__ESP32_PALETTE_WEBUI_DESIGN
  #define USE_LIGHTING_TEMPLATE__PALETTE_TESTING_CEILING
#endif
#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__L8__ESP32_ROOM_METAL_144LEDS
  #define DEVICENAME_CTR          "testgroup_lighting_L8"
  #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L7n8__ESP32_PALETTE_WEBUI_DESIGN
  // #define USE_LIGHTING_TEMPLATE__PALETTE_TESTING_METAL
  // #define USE_LIGHTING_TEMPLATE__PALETTE_TESTING_METAL
#endif


#if defined(DEVICE_TESTGROUP__LIGHTING_EFFECTS__L7n8__ESP32_PALETTE_WEBUI_DESIGN)
  #ifndef DEVICENAME_CTR
    #define DEVICENAME_CTR          "testbed_default"
  #endif
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883

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

  // #define ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING

  /***********************************
   * SECTION: System Configs
  ************************************/    

  #define SETTINGS_HOLDER 1238

  #define USE_MODULE_DRIVERS_FILESYSTEM
    #define WLED_ENABLE_FS_EDITOR
    #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
    #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM
    // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
    #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
    #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
    #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
    // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA

    
  #define USE_LIGHTING_TEMPLATE__PALETTE_TESTING_200LED_DEFAULT
 
  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define USE_MODULE_NETWORK_WEBSERVER23
  
  /***********************************
   * SECTION: Lighting Configs
  ************************************/    

  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_OCTOBER_2023

  // #define ENABLE_DEVFEATURE_LIGHT_ESP32_RMT_METHOD_AS_PRIMARY
  
  #ifdef USE_LIGHTING_TEMPLATE__PALETTE_TESTING_CEILING
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        100
      ],
      "ColourPalette":0,
      "SegColour0": {
        "Hue": 330,
        "Sat":100,
        "BrightnessRGB":5
      },
      "Effects": {
        "Function": 0,
        "Speed":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__PALETTE_TESTING_CEILING
  
  #ifdef USE_LIGHTING_TEMPLATE__PALETTE_TESTING_METAL_DEFAULT
  #define USE_LIGHTING_TEMPLATE_DEFAULT
  // template should be loaded only if the file did not exist already
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE_DEFAULT) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":144
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        144
      ],
      "ColourPalette":"Colourful Default",
      "SegColour0": {
        "Hue": 0,
        "Sat":100,
        "BrightnessRGB":100
      },
      "Effects": {
        "Function": 1,
        "Speed":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 2000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__PALETTE_TESTING_CEILING

  #ifdef USE_LIGHTING_TEMPLATE__PALETTE_TESTING_200LED_DEFAULT
  #define USE_LIGHTING_TEMPLATE_DEFAULT
  // template should be loaded only if the file did not exist already
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE_DEFAULT) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":200
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        200
      ],
      "ColourPalette":"Colourful Default",
      "SegColour0": {
        "Hue": 0,
        "Sat":100,
        "BrightnessRGB":100
      },
      "Effects": {
        "Function": 1,
        "Speed":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 2000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__PALETTE_TESTING_CEILING

  
  #ifdef USE_LIGHTING_TEMPLATE__PALETTE_TESTING_METAL
  #define USE_LIGHTING_TEMPLATE
  // template should be loaded only if the file did not exist already
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"GRBW",
        "BusType":"SK6812_RGBW",
        "Start":0,
        "Length":144
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        144
      ],
      "ColourPalette":"Colourful Default",
      "SegColour0": {
        "Hue": 0,
        "Sat":100,
        "BrightnessRGB":100
      },
      "Effects": {
        "Function": 1,
        "Speed":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 2000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__PALETTE_TESTING_CEILING


  #ifdef USE_LIGHTING_TEMPLATE__PALETTE_TESTING_LARGE_OUTPUT_ERROR
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"GRBW",
        "BusType":"SK6812_RGBW",
        "Start":0,
        "Length":600
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        600
      ],
      "ColourPalette":"Colourful Default",
      "SegColour0": {
        "Hue": 0,
        "Sat":100,
        "BrightnessRGB":100
      },
      "Effects": {
        "Function": 1,
        "Speed":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 2000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__PALETTE_TESTING_LARGE_OUTPUT_ERROR

  
  #ifdef USE_LIGHTING_TEMPLATE__PALETTE_TESTING_LARGE_OUTPUT_TWO_PINS
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"GRBW",
        "BusType":"SK6812_RGBW",
        "Start":0,
        "Length":1000
      },
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1000,
        "Length":2000
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        2000
      ],
      "ColourPalette":"Colourful Default",
      "SegColour0": {
        "Hue": 0,
        "Sat":100,
        "BrightnessRGB":100
      },
      "Effects": {
        "Function": 1,
        "Speed":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 2000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__PALETTE_TESTING_LARGE_OUTPUT_TWO_PINS



  #ifdef USE_LIGHTING_TEMPLATE__PALETTE_TESTING_METAL_TWO_SEGMENTS
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"GRBW",
        "BusType":"SK6812_RGBW",
        "Start":0,
        "Length":144
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        100
      ],
      "ColourPalette":0,
      "SegColour0": {
        "Hue": 0,
        "Sat":100,
        "BrightnessRGB":100
      },
      "Effects": {
        "Function": 1,
        "Speed":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 2000
      },
      "BrightnessRGB": 10,
      "BrightnessCCT": 0
    },
    "Segment1": {
      "PixelRange": [
        100,
        144
      ],
      "ColourPalette":1,
      "SegColour0": {
        "Hue": 0,
        "Sat":100,
        "BrightnessRGB":100
      },
      "Effects": {
        "Function": 2,
        "Speed":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 2000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__PALETTE_TESTING_CEILING_TWO_SEGMENTS

  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{" 
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":4,"                // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_02_A_CTR "\":13,"               // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_03_A_CTR "\":14,"               // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_04_A_CTR "\":27"                // Digital SK6812
      #endif
    "},"
    "\"" D_JSON_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  /***********************************
   * SECTION: Device Configs
  ************************************/    

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":1},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";


#endif // DEVICE_TESTBED_LIGHT_SEGMENT_ESP32__MULTIPIN











/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__L9__ESP8266_WEBUI_OPTIM_3LEDS
  #ifndef DEVICENAME_CTR
    #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883


  #define ENABLE_FREERAM_APPENDING_SERIAL

  #define USE_MODULE_SENSORS_SOLAR_LUNAR

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  // #define USE_MODULE_NETWORK_WEBSERVER23
  // #define USE_MODULE_NETWORK_WEBSERVER23
  // #define  ENABLE_WEBSERVER_LIGHTING_WEBUI

  #undef ESP32
  #define ESP8266

  // #define ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE
  #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS

  // #define ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
  // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL

  // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_TESTER
  // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_DEVELOPER


  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES


  //mqtt debug
  #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
  // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // #define DISABLE_NETWORK
  // #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_WIFI
  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  #define ANIMATION_UPDATOR_TIME_MINIMUM 20

  #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  // #undef ESP32
  // #define ESP8266

  #define ENABLE_DEBUG_MANUAL_DELAYS

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_AUGUST_2023

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{" 
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":3"   
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":1},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";


  #define USE_LIGHTING_TEMPLATE
  #define STRIP_SIZE_MAX 100
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":3,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        100
      ],
      "ColourPalette":0,
      "SegColour0": {
        "Hue": 330,
        "Sat":100,
        "BrightnessRGB":5
      },
      "Effects": {
        "Function": 3,
        "Speed":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";

#endif // DEVICE_TESTGROUP__LIGHTING_EFFECTS__L9__ESP8266_WEBUI_OPTIM_3LEDS















#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__L10__ESP32__7SEGMENTCLOCK
  #define DEVICENAME_CTR            "rgbclock_testclock_01"
  #define DEVICENAME_FRIENDLY_CTR   "RGB Dell 32"
  #define DEVICENAME_ROOMHINT_CTR   "Temporary_Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883
  
  /***********************************
   * SECTION: System Configs
  ************************************/    
  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES


  #define USE_MODULE_SENSORS_SOLAR_LUNAR

  /***********************************
   * SECTION: Network Configs
  ************************************/    

  // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL
  #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
  // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // #define DISABLE_NETWORK
  // #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_WIFI
  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  #define USE_MODULE_NETWORK_WEBSERVER23
  #define USE_MODULE_NETWORK_WEBSERVER23

  #define ENABLE_WEBSERVER_LIGHTING_WEBUI
  
  /***********************************
   * SECTION: Lighting Configs
  ************************************/    

  #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS 
  #define ANIMATION_UPDATOR_TIME_MINIMUM 20
  #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR
  #define ENABLE_DEBUG_MANUAL_DELAYS

  #define ENABLE_DEVFEATURE_PALETTE__CHANGE_MY_PALETTE_INDEXING_TO_255_RANGE

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_AUGUST_2023

  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS

  #define USE_LIGHTING_TEMPLATE
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS

  #define STRIP_SIZE_MAX 93 
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"GRBW",
        "BusType":"SK6812_RGBW",
        "Start":0,
        "Length":93
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        93
      ],
      "ColourPalette":97,
      "SegColour0": {
        "Hue": 330,
        "Sat":100,
        "BrightnessRGB":5
      },
      "Effects": {
        "Function":"Clock Basic 01",
        "Speed":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 2000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";

  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{" 
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":4,"                // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_02_A_CTR "\":13,"               // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_03_A_CTR "\":14,"               // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_04_A_CTR "\":27"               // Digital SK6812
      #endif
    "},"
    "\"" D_JSON_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  /***********************************
   * SECTION: Device Configs
  ************************************/    

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":1},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";

#endif






#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__L11__ESP32_LARGE_SINGLE_PIN_TESTER
  #define DEVICENAME_CTR          "testgroup_lighting_L11"
  #define USE_LIGHTING_TEMPLATE__PALETTE_TESTING_METAL
  #ifndef DEVICENAME_CTR
    #define DEVICENAME_CTR          "testbed_default"
  #endif
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883

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

  /***********************************
   * SECTION: System Configs
  ************************************/    
 
  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define USE_MODULE_NETWORK_WEBSERVER23
  
  /***********************************
   * SECTION: Lighting Configs
  ************************************/    
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_SEPTEMBER_2023

  // #define ENABLE_DEVFEATURE_LIGHT_ESP32_RMT_METHOD_AS_PRIMARY

    // #define USE_DEVFEATURE_PRESETS_MANUALUSERCUSTOM_OUTSIDETREE // view so I can replicate in json
    // #define ENABLE_DEVFEATURE_DEBUG_GARGAE_PIXEL_OUTPUT


  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // R"=====(
  // {
  //   "BusConfig":[
  //     {
  //       "Pin":13,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":0,
  //       "Length":2
  //     }
  //   ],
  //   "Segment0": {
  //     "PixelRange": [
  //       0,
  //       2
  //     ],
  //     "ColourPalette":"*Two Colours",
  //     "SegColour0": {
  //       "Hue": 0,
  //       "Sat":100,
  //       "BrightnessRGB":5
  //     },
  //     "Effects": {
  //       "Function": 1,
  //       "Speed":1,
  //       "Intensity":255
  //     },
  //     "Transition": {
  //       "TimeMs": 0,
  //       "RateMs": 1000
  //     },
  //     "BrightnessRGB": 100,
  //     "BrightnessCCT": 0
  //   },
  //   "BrightnessRGB": 100,
  //   "BrightnessCCT": 0
  // }
  // )=====";



  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // R"=====(
  // {
  //   "BusConfig":[
  //     {
  //       "Pin":4,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":0,
  //       "Length":3
  //     }
  //   ],
  //   "Segment0": {
  //     "PixelRange": [
  //       0,
  //       3
  //     ],
  //     "ColourPalette":"*Colour 01",
  //     "SegColour0": {
  //       "Hue": 0,
  //       "Sat":100,
  //       "BrightnessRGB":100
  //     },
  //     "Effects": {
  //       "Function": 1,
  //       "Speed":1,
  //       "Intensity":255
  //     },
  //     "Transition": {
  //       "TimeMs": 0,
  //       "RateMs": 1000
  //     },
  //     "BrightnessRGB": 100,
  //     "BrightnessCCT": 0
  //   },
  //   "BrightnessRGB": 100,
  //   "BrightnessCCT": 0
  // }
  // )=====";





  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // R"=====(
  // {
  //   "BusConfig":[
  //     {
  //       "Pin":4,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":0,
  //       "Length":500
  //     }
  //   ],
  //   "Segment0": {
  //     "PixelRange": [
  //       0,
  //       500
  //     ],
  //     "ColourPalette":"*Two Colours",
  //     "SegColour0": {
  //       "Hue": 0,
  //       "Sat":100,
  //       "BrightnessRGB":5
  //     },
  //     "Effects": {
  //       "Function": 1,
  //       "Speed":1,
  //       "Intensity":255
  //     },
  //     "Transition": {
  //       "TimeMs": 0,
  //       "RateMs": 1000
  //     },
  //     "BrightnessRGB": 100,
  //     "BrightnessCCT": 0
  //   },
  //   "BrightnessRGB": 100,
  //   "BrightnessCCT": 0
  // }
  // )=====";


  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":1000
      },
      {
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1001,
        "Length":1000
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        2000
      ],
      "ColourPalette":"*Two Colours",
      "SegColour0": {
        "Hue": 0,
        "Sat":100,
        "BrightnessRGB":5
      },
      "Effects": {
        "Function": 1,
        "Speed":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";

  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{" 
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      // "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":4,"                // Digital WS2812
      #endif
    "},"
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
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";

#endif // DEVICE_TESTBED_LIGHT_SEGMENT_ESP32__MULTIPIN




#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__L12__ESP32_LARGE_SINGLE_PIN_TESTER_WITH_DEBUG_PINS
  #define DEVICENAME_CTR          "testgroup_lighting_L12"
  #define USE_LIGHTING_TEMPLATE__PALETTE_TESTING_METAL
  #ifndef DEVICENAME_CTR
    #define DEVICENAME_CTR          "testbed_default"
  #endif
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883

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

  /***********************************
   * SECTION: System Configs
  ************************************/    
 
  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define USE_MODULE_NETWORK_WEBSERVER23
  
  /***********************************
   * SECTION: Lighting Configs
  ************************************/    
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_SEPTEMBER_2023


      // {
      //   "Pin":4,
      //   "ColourOrder":"GRBW",
      //   "BusType":"SK6812_RGBW",
      //   "Start":0,
      //   "Length":3
      // },

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":5,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":3
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        3
      ],
      "ColourPalette":"*Two Colours",
      "SegColour0": {
        "Hue": 0,
        "Sat":100,
        "BrightnessRGB":5
      },
      "Effects": {
        "Function": 1,
        "Speed":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";



  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // R"=====(
  // {
  //   "BusConfig":[
  //     {
  //       "Pin":4,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":0,
  //       "Length":1000
  //     },
  //     {
  //       "Pin":5,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":1000,
  //       "Length":2000
  //     }
  //   ],
  //   "Segment0": {
  //     "PixelRange": [
  //       0,
  //       2000
  //     ],
  //     "ColourPalette":"Colourful Default",
  //     "SegColour0": {
  //       "Hue": 0,
  //       "Sat":100,
  //       "BrightnessRGB":5
  //     },
  //     "Effects": {
  //       "Function": 1,
  //       "Speed":1,
  //       "Intensity":255
  //     },
  //     "Transition": {
  //       "TimeMs": 0,
  //       "RateMs": 1000
  //     },
  //     "BrightnessRGB": 100,
  //     "BrightnessCCT": 0
  //   },
  //   "BrightnessRGB": 100,
  //   "BrightnessCCT": 0
  // }
  // )=====";

  /***********************************
   * SECTION: Debug Pins
  ************************************/    

  #define DEBUG_PIN1_GPIO       32
  #define DEBUG_PIN2_GPIO       33
  #define DEBUG_PIN3_GPIO       25
  #define DEBUG_PIN4_GPIO       14
  #define DEBUG_PIN5_GPIO       12
  #define DEBUG_PIN6_GPIO       13
  #define DEBUG_PIN7_GPIO       5 //motion pin

  #define USE_FEATURE_DEBUG_PIN_INIT_LOGIC_LEVEL_SEQUENCE_CHECK

  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{" 
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":4,"                // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_02_A_CTR "\":13,"               // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_03_A_CTR "\":14,"               // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_04_A_CTR "\":27"               // Digital SK6812
      #endif
    "},"
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
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";

#endif // DEVICE_TESTBED_LIGHT_SEGMENT_ESP32__MULTIPIN














#endif // _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_ACTIVE_TESTGROUPS_BETA_H

