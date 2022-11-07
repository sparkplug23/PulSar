#ifndef MSYSTEMCONFIG_HARDWAREDEFAULTS_FIRMWAR_MIC4HAEL_CHRISTMAS_H
#define MSYSTEMCONFIG_HARDWAREDEFAULTS_FIRMWAR_MIC4HAEL_CHRISTMAS_H

/*********************************************************************************************\
 * Developing for home only: 
 *  - Devices to be put into home, but not yet ready for install. These are NOT "Devboards" (examples) but meant for actual developing of devices
\*********************************************************************************************/

/*********************************************************************************************\
 * This file is one of two user configurable files
 * 
 * "mFirmwareCustom.h" allows advanced users to create a custom build of firmware tailored to a specefic tasks 
 * For example, using a device to control heating, which includes numorous sensors, relays and controls that are unique.
 * This can also include lights or displays as notification/information systems. 
 * Although all sensors/drivers are accesible through default builds, this allows users to config and add their own controllers.
 * Ideally, when possible, templates can be used to replicate the custom firmware options performed here.
 * 
 * Ideas:
 * Create two basic LCD numbers, one for outside and one for bedroom temperature
 * Create world map (like I said to jess) that lights to show where the sun is, and day/night land-mass light up with cities being bright spots.
\*********************************************************************************************/

// #define DEVICE_FORCED_TO_BE_TESTER
// #define DISABLE_WEBSERVER
// //#define FORCE_TEMPLATE_LOADING

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"
#include "6_Lights/00_Interface/mInterfaceLight_Defines.h"
#include "2_CoreSystem/03_HardwareTemplates/mHardwareTemplates.h"

//--------------------------------[Enable Device]-------------------------------------

/**
 * Christmas 2022 Development Devices
 * */
// #define DEVICE_RGBSTRING_DEVBOARD_SEGMENT
// #define DEVICE_RGBSTRING_CHRISTMAS_ESP32_DEVKIT1

/**
 *  Christmas 2022 Installed Devices
 * */  
// #define DEVICE_RGBBEDLIGHT 


/**
 * @brief 
 * 
 * Ideas for this christmas 2022
 * 
 * Create new "Preset" (WLED) and "Playlist". This will replace my "Mixer".
 * 
 * Each function (inc. brightness, effect, palette etc) will be considered one preset
 * Going through a group of presets, will be known as a playlist
 * 
 * Preset ideas for Outside tree
 *  ** (look for video in 2020)
 *  ** Static - Christmas 01 (RGPBO) - No Grouping
 * 
 * 
 * New effect could be growing and shrinking grouping count e.g. "Grouping Breath" (ie from 0% [group=1] to 100% [group=100% of string length])
 * 
 */
#ifdef DEVICE_RGBSTRING_CHRISTMAS_ESP32_DEVKIT1
  #define DEVICENAME_CTR          "devboard_string_segment_01"
  #define DEVICENAME_FRIENDLY_CTR "RGB Notifications 01"
  #define DEVICENAME_ROOMHINT_CTR "Testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  #define DISABLE_NETWORK

  // #define USE_DEVFEATURE_METHOD_SEGMENTS_BUILD
  //#define USE_DEVFEATURE_METHOD_HACS_LEGACY_BUILD
  // #define USE_DEVFEATURE_METHOD_WLED_BUILD
 
 
  // #ifdef USE_DEVFEATURE_METHOD_SEGMENTS_BUILD
    // #define USE_BUILD_TYPE_LIGHTING
    // #define USE_MODULE_LIGHTS_INTERFACE
    // #define USE_MODULE_LIGHTS_ANIMATOR
    // #define USE_MODULE_LIGHTS_ADDRESSABLE
    // #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    // // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT
    // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
    // // #define LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS_FOR_UTILITY
    // #define D_EFFECT_INSIDE_TEMPLATE "Effects"
    // // enable some wled conerted aniamtions
    // #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    // #define DEBUG_WLED_EFFECT_FUNCTIONS
    // #define ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL

    // #define ENABLE_DEVFEATURE_PHASE_OUT_LEDORDERARRAY
    // #define ENABLE_DEVFEATURE_PHASE_OUT_ANIMATIONCOLOUR_STRUCT
    // #define ENABLE_FREERAM_APPENDING_SERIAL

    // #define  ENABLE_FEATURE_INCLUDE_WLED_PALETTES

    // #define ENABLE_CRGBPALETTES_IN_PROGMEM

    // #define STRIP_SIZE_MAX 100
    
  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

    
//     #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
//     #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
//     #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
//     #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS

//       #define ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32

//     // #define USE_BUILD_TYPE_LIGHTING
//     // #define USE_MODULE_LIGHTS_INTERFACE
//     // #define USE_MODULE_LIGHTS_ANIMATOR
//     // #define USE_MODULE_LIGHTS_ADDRESSABLE
//     // #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//     // // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT
//     // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
//     // // #define LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS_FOR_UTILITY
//     #define D_EFFECT_INSIDE_TEMPLATE "Effects"
//     // // enable some wled conerted aniamtions
//     // #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//     // #define DEBUG_WLED_EFFECT_FUNCTIONS
//     // // #define ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL

//     // #define ENABLE_DEVFEATURE_PHASE_OUT_LEDORDERARRAY
//     // #define ENABLE_DEVFEATURE_PHASE_OUT_ANIMATIONCOLOUR_STRUCT
//     // #define ENABLE_FREERAM_APPENDING_SERIAL

//     // // #define  ENABLE_FEATURE_INCLUDE_WLED_PALETTES

//     // // to merge h801 rgbcct animation into the new method
//     // #define DISABLE_ANIMATION_COLOURS_FOR_RGBCCT_OLD_METHOD
//     // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS
 

  
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS // Not ready to remove
    #define STRIP_SIZE_MAX 16+1 //still needs extra +1
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
    "\"ColourOrder\": \"GRB\","
    "\"ColourPalette\":\"Christmas 01\","
    "\"Effects\":{"
      "\"Function\":4,"
      "\"Intensity\":255,"
      "\"Grouping\":4,"
      "\"Speed\":10"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":1500,"
      "\"RateMs\":2000"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // USE_MODULE_LIGHTS_INTERFACE


//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
//     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_JSON_GPIOC "\":{"
//       #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
//       "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
//       #endif 
//       "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
//     "},"
//     "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
//     "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
//   "}";

// /**
//  * @brief 
//  Segment command for single segment to replace old effects
//  Without using a "Segment#" the commands will be assumed as for the entire strip and internally be segment0
//  * 
//  */
//  #define USE_LIGHTING_TEMPLATE


// #define LIGHTING_TEMPLATE_SINGLE_SEGMENT
// //  #define LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS


//   #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT
//   DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   "{"
//     "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
//     "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//     "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
//     "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
//     "\"ColourPalette\":\"Christmas 06\"," 
//     "\"Effects\":{"
//       "\"Function\":0"
//     "},"
//     "\"Transition\":{"
//       "\"TimeMs\":500,"
//       "\"RateMs\":1000"
//     "},"    
//     "\"BrightnessRGB\":100"
//   "}";
//   #endif

//   #ifdef LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS
//   DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   "{"
//     "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
//     "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//     "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
//     "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
//     "\"ColourPalette\":\"Christmas 21\"," 
//     "\"Effects\":{"
//       "\"Function\":30"
//     "},"
//     "\"Transition\":{"
//       "\"TimeMs\":0,"
//       "\"RateMs\":30"
//     "},"    
//     "\"BrightnessRGB\":100"
//   "}";
//   #endif // LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS

//   #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
//   DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   "{"
//     "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
//     "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//     "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
//     "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
//     "\"ColourPalette\":\"Christmas 06\"," 
//     "\"Effects\":{"
//       "\"Function\":1"
//     "},"
//     "\"Transition\":{"
//       "\"TimeMs\":0,"
//       "\"RateMs\":30"
//     "},"    
//     "\"BrightnessRGB\":100"
//   "}";
//   #endif // LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS

//   #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS
//   DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   "{"
//     "\"Segment0\":{"
//       "\"PixelRange\":[0,19],"
//       "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
//       "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//       "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
//       "\"Effects\":{"
//         "\"Function\":\"Static\""
//       "},"
//       "\"ColourPalette\":\"Christmas 09\","
//       "\"Transition\":{"
//         "\"TimeMs\":3000,"
//         "\"RateMs\":10000"
//       "},"    
//       "\"BrightnessRGB\":100"
//     "},"
//     "\"Segment1\":{"
//       "\"PixelRange\":[20,29],"
//       "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
//       "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//       "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","   
//       "\"Effects\":{"
//         "\"Function\":\"Solid RGBCCT\""
//       "},"
//       "\"ColourPalette\":\"Solid Rgbcct 01\","
//       "\"Transition\":{"
//         "\"TimeMs\":500,"
//         "\"RateMs\":1000"
//       "},"    
//       "\"BrightnessRGB\":100"
//     "},"
//     "\"Segment2\":{"
//       "\"PixelRange\":[30,49],"
//       "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
//       "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//       "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
//       "\"Effects\":{"
//         "\"Function\":\"Slow Glow\""
//       "},"
//       "\"ColourPalette\":\"Christmas 01\","
//       "\"Transition\":{"
//         "\"TimeMs\":500,"
//         "\"RateMs\":1000"
//       "},"    
//       "\"BrightnessRGB\":100"
//     "},"
//     "\"" D_JSON_ANIMATIONMODE    "\":\"Effects\","
//     "\"BrightnessRGB\":100"
//   "}";
//   #endif
  
//   #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS_FOR_UTILITY
//   DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   "{"
//     "\"Segment0\":{"
//       "\"PixelRange\":[0,19],"
//       "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
//       "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//       "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
//       "\"Effects\":{"
//         "\"Function\":29"
//       "},"
//       "\"ColourPalette\":\"Christmas 09\","
//       "\"Transition\":{"
//         "\"TimeMs\":0,"
//         "\"RateMs\":23"
//       "},"    
//       "\"BrightnessRGB\":100"
//     "},"
//     "\"Segment1\":{"
//       "\"PixelRange\":[20,29],"
//       "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
//       "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//       "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","   
//       "\"Effects\":{"
//         "\"Function\":28"
//       "},"
//       "\"ColourPalette\":11,"
//       "\"Hue\":20,"
//       "\"Sat\":90,"
//       "\"Transition\":{"
//         "\"TimeMs\":0,"
//         "\"RateMs\":25"
//       "},"    
//       "\"BrightnessRGB\":100"
//     "},"
//     "\"Segment2\":{"
//       "\"PixelRange\":[30,49],"
//       "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
//       "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//       "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
//       "\"Effects\":{"
//         "\"Function\":\"Slow Glow\""
//       "},"
//       "\"ColourPalette\":\"Christmas 01\","
//       "\"Transition\":{"
//         "\"TimeMs\":500,"
//         "\"RateMs\":1000"
//       "},"    
//       "\"BrightnessRGB\":100"
//     "},"
//     "\"" D_JSON_ANIMATIONMODE    "\":\"Effects\","
//     "\"BrightnessRGB\":100"
//   "}";
//   #endif


  /** Copy from mqtt broker
   * 
   * {
  "Segment0": {
    "PixelRange": [
      31,
      40
    ],
    "Effects": {
      "Function":1
    },
    "ColourPalette": "Christmas 06",
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 1500,
      "RateMs": 2000
    },
    "BrightnessRGB": 100
  },
  "Segment1": {
    "PixelRange": [
      21,
      30
    ],
    "Effects": {
      "Function": 28
    },
    "ColourPalette": 11,
    "Hue": 20,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    },
    "BrightnessRGB": 100
  },
  "Segment2": {
    "PixelRange": [
      0,
      20
    ],
    "Effects": {
      "Function": 29
    },
    "ColourPalette": "Christmas 06",
    "Hue": 120,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    },
    "BrightnessRGB": 100
  }
}


  {
  "Segment0": {
    "PixelRange": [
      40,
      49
    ],
    "Effects": {
      "Function": 28,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 20,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    }
  },
  "Segment1": {
    "PixelRange": [
      0,
      39
    ],
    "ColourOrder":"grb",
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    }
  },
  "BrightnessRGB": 100
}


{
  "Segment0": {
    "PixelRange": [
      0,
      10
    ],
    "Effects": {
      "Function": 27,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 10,
    "Sat": 100,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB":100
  },
    "Hue": 120,
    "Sat": 90,
  "Segment1": {
    "PixelRange": [
      11,
      49
    ],
    "ColourOrder":"grb",
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    }
  },
  "BrightnessRGB": 100
}

{
  "Segment0": {
    "PixelRange": [
      0,
      10
    ],
    "Effects": {
      "Function": 28,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 25,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 20
    },
    "BrightnessRGB": 10
  },
  "Segment1": {
    "PixelRange": [
      11,
      20
    ],
    "Effects": {
      "Function": 27,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 11,
    "Hue": 240,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  },
  "Segment2": {
    "PixelRange": [
      25,
      40
    ],
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 14,
    "Hue": 120,
    "Sat": 100,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  }
}

{
  "PixelRange": [
    0,
    49
  ],
  "AnimationMode": 3,
  "Strip": {
    "ClearTo": [
      0,
      0,
      0,
      0,0
    ],
    "ManualSetPixelToScene":[0,1,2,3,4,5,40,41,42,43,44,45,46,47,48,49]
  },
  "Effects": {
    "Function": 109
  },
  "ColourPalette": 10,
  "Hue": 20,
  "Sat": 100,
  "BrightnessCCT": 100,
  "CCT_TempPercentage": 100,
  "ColourOrder": "grbwc",
  "Transition": {
    "TimeMs": 900,
    "RateMs": 20
  },
  "BrightnessRGB": 100
}

*/

#endif




#ifdef DEVICE_RGBSTRING_DEVBOARD_SEGMENT_ESP32
  #define DEVICENAME_CTR          "devboard_string_segment_01"
  #define DEVICENAME_FRIENDLY_CTR "RGB Notifications 01"
  

  #define USE_DEVFEATURE_METHOD_SEGMENTS_BUILD
  //#define USE_DEVFEATURE_METHOD_HACS_LEGACY_BUILD
  // #define USE_DEVFEATURE_METHOD_WLED_BUILD
 
/**
 * @brief 
 * 
 * Outside show
 * 
 * 6 - red/green
 * 12
 * 31
 */

 
  #ifdef USE_DEVFEATURE_METHOD_SEGMENTS_BUILD

    #define USE_BUILD_TYPE_LIGHTING
    #define USE_MODULE_LIGHTS_INTERFACE
    #define USE_MODULE_LIGHTS_ANIMATOR
    #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT
    #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
    // #define LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS_FOR_UTILITY
    #define D_EFFECT_INSIDE_TEMPLATE "Effects"
    // enable some wled conerted aniamtions
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    #define DEBUG_WLED_EFFECT_FUNCTIONS
    #define ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL

    #define ENABLE_DEVFEATURE_PHASE_OUT_LEDORDERARRAY
    #define ENABLE_DEVFEATURE_PHASE_OUT_ANIMATIONCOLOUR_STRUCT
    #define ENABLE_FREERAM_APPENDING_SERIAL

    #define  ENABLE_FEATURE_INCLUDE_WLED_PALETTES

    // to merge h801 rgbcct animation into the new method
    #define DISABLE_ANIMATION_COLOURS_FOR_RGBCCT_OLD_METHOD

    #define ENABLE_CRGBPALETTES_IN_PROGMEM

    #define STRIP_SIZE_MAX 1300

    #define ENABLE_DEVFEATURE_SHIMMERING_PALETTE_BRIGHTNESS_LIMIT
  
    // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS

  #endif 
  #ifdef USE_DEVFEATURE_METHOD_HACS_LEGACY_BUILD
    #define USE_BUILD_TYPE_LIGHTING
    #define USE_MODULE_LIGHTS_INTERFACE
    #define USE_MODULE_LIGHTS_ANIMATOR
    #define USE_MODULE_LIGHTS_ADDRESSABLE
    
    #define LIGHTING_TEMPLATE_SINGLE_SEGMENT
    #define D_EFFECT_INSIDE_TEMPLATE "Effects"
  #endif 
  #ifdef USE_DEVFEATURE_METHOD_WLED_BUILD
    #define USE_BUILD_TYPE_LIGHTING
    #define USE_MODULE_LIGHTS_INTERFACE
    #define USE_MODULE_LIGHTS_ANIMATOR
    #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION  // to test existing effects in wled
    
    // #define LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS
    #define LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS
    // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT
    #define D_EFFECT_INSIDE_TEMPLATE "WLED"
    #define DEBUG_WLED_EFFECT_FUNCTIONS
  #endif


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"23\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
      //"\"LBI\":\"" D_GPIO_FUNCTION_LED1_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

/**
 * @brief 
 Segment command for single segment to replace old effects
 Without using a "Segment#" the commands will be assumed as for the entire strip and internally be segment0
 * 
 */
//  #define STRIP_SIZE_MAX 1300
 #define USE_LIGHTING_TEMPLATE

//  #define LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS


  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 06\"," 
    "\"Effects\":{"
      "\"Function\":1"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":3000,"
      "\"RateMs\":10000"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 21\"," 
    "\"Effects\":{"
      "\"Function\":30"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":30"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 06\"," 
    "\"Effects\":{"
      "\"Function\":1"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":30"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS

  #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"Segment0\":{"
      "\"PixelRange\":[0,19],"
      "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Static\""
      "},"
      "\"ColourPalette\":\"Christmas 09\","
      "\"Transition\":{"
        "\"TimeMs\":3000,"
        "\"RateMs\":10000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment1\":{"
      "\"PixelRange\":[20,29],"
      "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","   
      "\"Effects\":{"
        "\"Function\":\"Solid RGBCCT\""
      "},"
      "\"ColourPalette\":\"Solid Rgbcct 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment2\":{"
      "\"PixelRange\":[30,49],"
      "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Slow Glow\""
      "},"
      "\"ColourPalette\":\"Christmas 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\"Effects\","
    "\"BrightnessRGB\":100"
  "}";
  #endif
  
  #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS_FOR_UTILITY
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"Segment0\":{"
      "\"PixelRange\":[0,19],"
      "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":29"
      "},"
      "\"ColourPalette\":\"Christmas 09\","
      "\"Transition\":{"
        "\"TimeMs\":0,"
        "\"RateMs\":23"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment1\":{"
      "\"PixelRange\":[20,29],"
      "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","   
      "\"Effects\":{"
        "\"Function\":28"
      "},"
      "\"ColourPalette\":11,"
      "\"Hue\":20,"
      "\"Sat\":90,"
      "\"Transition\":{"
        "\"TimeMs\":0,"
        "\"RateMs\":25"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment2\":{"
      "\"PixelRange\":[30,49],"
      "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Slow Glow\""
      "},"
      "\"ColourPalette\":\"Christmas 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\"Effects\","
    "\"BrightnessRGB\":100"
  "}";
  #endif
  
  /** Copy from mqtt broker
   * 
   * {
  "Segment0": {
    "PixelRange": [
      31,
      40
    ],
    "Effects": {
      "Function":1
    },
    "ColourPalette": "Christmas 06",
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 1500,
      "RateMs": 2000
    },
    "BrightnessRGB": 100
  },
  "Segment1": {
    "PixelRange": [
      21,
      30
    ],
    "Effects": {
      "Function": 28
    },
    "ColourPalette": 11,
    "Hue": 20,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    },
    "BrightnessRGB": 100
  },
  "Segment2": {
    "PixelRange": [
      0,
      20
    ],
    "Effects": {
      "Function": 29
    },
    "ColourPalette": "Christmas 06",
    "Hue": 120,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    },
    "BrightnessRGB": 100
  }
}


  {
  "Segment0": {
    "PixelRange": [
      40,
      49
    ],
    "Effects": {
      "Function": 28,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 20,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    }
  },
  "Segment1": {
    "PixelRange": [
      0,
      39
    ],
    "ColourOrder":"grb",
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    }
  },
  "BrightnessRGB": 100
}


{
  "Segment0": {
    "PixelRange": [
      0,
      10
    ],
    "Effects": {
      "Function": 27,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 10,
    "Sat": 100,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB":100
  },
    "Hue": 120,
    "Sat": 90,
  "Segment1": {
    "PixelRange": [
      11,
      49
    ],
    "ColourOrder":"grb",
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    }
  },
  "BrightnessRGB": 100
}

{
  "Segment0": {
    "PixelRange": [
      0,
      10
    ],
    "Effects": {
      "Function": 28,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 25,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 20
    },
    "BrightnessRGB": 10
  },
  "Segment1": {
    "PixelRange": [
      11,
      20
    ],
    "Effects": {
      "Function": 27,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 11,
    "Hue": 240,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  },
  "Segment2": {
    "PixelRange": [
      25,
      40
    ],
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 14,
    "Hue": 120,
    "Sat": 100,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  }
}

*/

#endif

#ifdef DEVICE_BUCKET_WATER_LEVEL
  #define DEVICENAME_CTR          "bucketwatersensor"
  #define DEVICENAME_FRIENDLY_CTR "RGB Notifications 01"
  
  #define USE_MODULE_CONTROLLER_BUCKET_WATER_LEVEL
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"23\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","

      "\"32\":\"" D_GPIO_FUNCTION_ADC1_CH4_CTR   "\","
      "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\""

    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";



  #define D_DEVICE_ADC_INPUT_NAME_LONG "WaterSensor"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_CONTROLLER_BUCKET_WATER_LEVEL_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_ADC_INPUT_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif





#endif // MSYSTEMCONFIG_HARDWAREDEFAULTS_H

