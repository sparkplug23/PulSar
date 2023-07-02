#ifndef _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_DEV_TESTBEDS_BETA_H
#define _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_DEV_TESTBEDS_BETA_H

/*********************************************************************************************\
 * Developing/Testbeds: 
 *  ** New code under test, not yet ready to be used as a template (ie may not work yet)
\*********************************************************************************************/

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"
#include "6_Lights/00_Interface/mInterfaceLight_Defines.h"
#include "2_CoreSystem/03_HardwareTemplates/mHardwareTemplates.h"

//--------------------------------[Enable Device]-------------------------------------

/**
 * DEVICE_TYPE NETWORK: Any testbeds related to lighting
 * */
// #define DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_01

/**
 * DEVICE_TYPE LIGHTING: Any testgroups related to lighting
 * */
// #define DEVICE_TESTGROUP__LIGHTING_MULTIPIN
  // #define SUBDEVICE_TESTGROUP__LIGHTING_MULTIPIN__H801_AS_FIVE_SINGLE_WHITE_CHANNELS
  // #define SUBDEVICE_TESTGROUP__LIGHTING_MULTIPIN__H801_AS_RGB_AND_CCT_SPLIT_CHANNELS
  // #define SUBDEVICE_TESTGROUP__LIGHTING_MULTIPIN__ESP32_AS_NEOPIXEL_DUAL_I2S_BUS

/**
 * DEVICE_TYPE DRIVERS: Any testbeds related to lighting
 * */
// #define DEVICE_ESP32_WEBCAM1

/**
 * DEVICE_TYPE CONTROLLERS: Any testbeds related to lighting
 * */


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/************************************************************************************************************************************************************************
 *************************************************************************************************************************************************************************
  @brief TESTGROUP__LIGHTING_MULTIPIN
 ************************************************************************************************************************************************************************
 *************************************************************************************************************************************************************************/

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

  #define  ENABLE_DEBUG_MULTIPIN

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

    #define ENABLE_DEVFEATURE__PIXEL_USE_I2S_FOR_BUS
    

    // #define ENABLE_DEVFEATURE_LIGHT__PRELOAD_BUSCONFIG_FROM_TEMPLATE_AS_TEMPORARY_MEASURE
    #define ENABLE_DEVFEATURE_LIGHT__MOVE_ALL_BUS_STARTING_CODE_UNTIL_LOOP



#endif // DEVICE_TESTGROUP__LIGHTING_MULTIPIN


#ifdef SUBDEVICE_TESTGROUP__LIGHTING_MULTIPIN__H801_AS_RGB_AND_CCT_SPLIT_CHANNELS
  #define DEVICENAME_CTR          "testgroup_multipin_h801_rgbsplitcct"
  #define DEVICENAME_FRIENDLY_CTR "TestGroup H801 RGB Split CCT Channels"
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883


  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  // #define USE_MODULE_DRIVERS_PWM


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








/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef SUBDEVICE_TESTGROUP__LIGHTING_MULTIPIN__ESP32_AS_NEOPIXEL_DUAL_I2S_BUS
  #define DEVICENAME_CTR          "testgroup_multipin_esp32_neodual"
  #define DEVICENAME_FRIENDLY_CTR "TestGroup H801 RGB Dual Channels"
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883

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
  #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS

  #ifdef USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS
  #define STRIP_SIZE_MAX 120
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":13,
        "ColourOrder":"GRBW",
        "BusType":"SK6812_RGBW",
        "Start":0,
        "Length":3
      },
      {
        "Pin":4,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":3,
        "Length":117
      }
    ],
    "Segment0":{
      "PixelRange": [
        0,
        3
      ],
      "ColourPalette":"Christmas 01",
      "Effects": {
        "Function":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "Segment1": {
      "PixelRange": [
        3,
        120
      ],
      "ColourPalette":"Christmas 01",
      "SegColour0": {
        "Hue": 330,
        "Sat":0,
        "BrightnessRGB":5
      },
      "Effects": {
        "Function": 2,
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
  #endif // USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS
/*




*/
#endif // DEVICE_TESTBED_LIGHT_SEGMENT_ESP32__MULTIPIN


















#endif // _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_DEV_TESTBEDS_BETA_H

