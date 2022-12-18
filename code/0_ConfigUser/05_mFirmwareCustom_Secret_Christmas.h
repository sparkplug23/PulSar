#ifndef MSYSTEMCONFIG_HARDWAREDEFAULTS_FIRMWAR_MIC4HAEL_CHRISTMAS_H
#define MSYSTEMCONFIG_HARDWAREDEFAULTS_FIRMWAR_MIC4HAEL_CHRISTMAS_H

/*********************************************************************************************\
 * Christmas 2022
\*********************************************************************************************/

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
// #define DEVICE_RGBSTRING_CHRISTMAS_ESP32_TEST_TREE_XMAS2022
// #define DEVICE_RGBBEDLIGHT 
// #define DEVICE_RGBSTRING_CHRISTMAS_ESP32_DININGROOM_XMAS2022
// #define DEVICE_RGBSTRING_CHRISTMAS_ESP32_OUTSIDETREE_PRIMATY_WIFI_ENABLED_XMAS2022
// #define DEVICE_RGBSTRING_CHRISTMAS_ESP32_HALLWAYTREE_XMAS2022
#define DEVICE_RGBSTRING_CHRISTMAS_ESP32_BEDROOM_CEILING_STRING
// #define DEVICE_RGBSTRING_CHRISTMAS_ESP32_UTILITY_TREE
// #define DEVICE_RGBSTRING_CHRISTMAS_ESP32_UTILITY_CANDLE

//--------------------------------[Device Templates]-------------------------------------

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

  #define ENABLE_DEVFEATURE_SHOW_UPTIME_SECONDS

  //#define ENABLE_DEVFEATURE___CAUTION_CAUTION__FORCE_CRASH_FASTBOOT_TESTING

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


  #define ENABLE_DEVFEATURE_CANSHOW_VERSION3
  #define ENABLE_DEVFEATURE_COLORADO_FORCED_TYPE
  #define ENABLE_DEVFEATURE_FORCED_REMOVE_091122
  #define ENABLE_DEVFEATURE_ANIMATORLIGHT_EVERYLOOP_2022_METHOD

  // #define ENABLE_DEVFEATURE_DEBUG_TRACE_LIGHTING_CHRISTMAS
  // #define ENABLE_DEVFEATURE_DEBUG_SERIAL__ANIMATION_OUTPUT

    
//     #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
//     #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
//     #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
//     #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS

  // #define ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32

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
    // #define STRIP_SIZE_MAX 16+1 //still needs extra +1
    #define STRIP_SIZE_MAX 2000 //still needs extra +1
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
    
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
    
    // #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS    
    #define ENABLE_DEVFEATURE_MOVING_GETCOLOUR_AND_PALETTE_TO_RAM

    // #define ENABLE_WLED_EFFECTS

    //#define ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM

// get code stable before worrying about "flickering"
    // #define ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32

// #define ENABLE_PIXEL_AUTOMATION_PLAYLIST
#define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS


    /**
     * @brief Preset
     * 
     * Each preset will allow to configure an animation with various changes (best to use json string? that way its future proof??)
     * Also, setting the value max (255/65535 etc) will mean this mode will not overwrite an aspect of the previous, that way they are additive
     * For 2022, just hard code them in a function
     * 
     *
     */

    /**
     * @brief Playlist
     * 
     * must include actives times, blocked times
     * 
     */


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
    "\"ColourOrder\": \"RGB\","
    "\"ColourPalette\":\"Christmas 24\","
    "\"Effects\":{"
      "\"Function\":2,"
      "\"Intensity\":255"
      // "\"Grouping\":4,"
      // "\"Speed\":10"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":2000,"
      "\"RateMs\":2200"
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

#endif // DEVICE_RGBSTRING_CHRISTMAS_ESP32_DEVKIT1


#ifdef DEVICE_RGBSTRING_CHRISTMAS_ESP32_DININGROOM_XMAS2022
  #define DEVICENAME_CTR          "xmas_diningroom_tree_01"
  #define DEVICENAME_FRIENDLY_CTR "XMAS Dining Room 2022"
  #define DEVICENAME_ROOMHINT_CTR "Dining Room"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70


  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID


  #define ENABLE_DEVFEATURE_CANSHOW_VERSION3
  #define ENABLE_DEVFEATURE_COLORADO_FORCED_TYPE
  #define ENABLE_DEVFEATURE_FORCED_REMOVE_091122
  #define ENABLE_DEVFEATURE_ANIMATORLIGHT_EVERYLOOP_2022_METHOD


  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS // Not ready to remove
    #define STRIP_SIZE_MAX 101 //still needs extra +1

    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID    
    #define ENABLE_DEVFEATURE_MOVING_GETCOLOUR_AND_PALETTE_TO_RAM


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
    "\"AnimationMode\":\"Effects\","
    "\"ColourOrder\":\"GRB\","
    "\"ColourPalette\":\"Christmas 24\","
    "\"Effects\":{"
      "\"Function\":1,"
      "\"Intensity\":255"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":1000"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // USE_MODULE_LIGHTS_INTERFACE

#endif // DEVICE_RGBSTRING_CHRISTMAS_ESP32_DININGROOM_XMAS2022


/**
 * @brief 
 * No WIFI
 * Basic Fallback that creates the desired static colours
 * For installing, it will show blending
 */
#ifdef DEVICE_RGBSTRING_CHRISTMAS_ESP32_OUTSIDETREE_TESTER_01_BLENDING_XMAS2022
  #define DEVICENAME_CTR          "xmas_outsidetree_tester_blending"
  #define DEVICENAME_FRIENDLY_CTR "XMAS Outside Tree Tester Blending"
  #define DEVICENAME_ROOMHINT_CTR "Outside"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  //#define DISABLE_NETWORK
  #define DISABLE_SERIAL

  #define DISABLE_SERIAL0_CORE
  #define DISABLE_SERIAL_LOGGING


  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID


  #define ENABLE_DEVFEATURE_CANSHOW_VERSION3
  #define ENABLE_DEVFEATURE_COLORADO_FORCED_TYPE
  #define ENABLE_DEVFEATURE_FORCED_REMOVE_091122
  #define ENABLE_DEVFEATURE_ANIMATORLIGHT_EVERYLOOP_2022_METHOD


  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS // Not ready to remove
  
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID    
    #define ENABLE_DEVFEATURE_MOVING_GETCOLOUR_AND_PALETTE_TO_RAM
    #define ENABLE_DEVFEATURE_REMOVE_BRIGHTNESS_RANDOMNESS_INSIDE_APPLY_BRIGHTNESS

  //#define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS

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

  #define STRIP_SIZE_MAX 1202 //still needs extra +1
  #ifdef USE_MODULE_LIGHTS_INTERFACE
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"AnimationMode\":\"Effects\","
    "\"ColourOrder\":\"RGB\","
    "\"ColourPalette\":\"Christmas 24\","
    "\"Effects\":{"
      "\"Function\":12," //shimmering palette
      "\"Speed\":127,"
      "\"Intensity\":127"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":23"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // USE_MODULE_LIGHTS_INTERFACE

#endif // DEVICE_RGBSTRING_CHRISTMAS_ESP32_OUTSIDETREE_TESTER_01_BLENDING_XMAS2022


/**
 * @brief 
 * No WIFI
 * Basic Fallback that creates the desired static colours
 * For installing, it will show blending
 */
#ifdef DEVICE_RGBSTRING_CHRISTMAS_ESP32_OUTSIDETREE_TESTER_02_STATIC_XMAS2022
  #define DEVICENAME_CTR          "xmas_diningroom_tree_01"
  #define DEVICENAME_FRIENDLY_CTR "XMAS Dining Room 2022"
  #define DEVICENAME_ROOMHINT_CTR "Dining Room"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70


  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID


  #define ENABLE_DEVFEATURE_CANSHOW_VERSION3
  #define ENABLE_DEVFEATURE_COLORADO_FORCED_TYPE
  #define ENABLE_DEVFEATURE_FORCED_REMOVE_091122
  #define ENABLE_DEVFEATURE_ANIMATORLIGHT_EVERYLOOP_2022_METHOD


  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS // Not ready to remove
    #define STRIP_SIZE_MAX 101 //still needs extra +1

    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID    
    #define ENABLE_DEVFEATURE_MOVING_GETCOLOUR_AND_PALETTE_TO_RAM


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
    "\"AnimationMode\":\"Effects\","
    "\"ColourOrder\":\"GRB\","
    "\"ColourPalette\":\"Christmas 24\","
    "\"Effects\":{"
      "\"Function\":1,"
      "\"Intensity\":255"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":1000"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // USE_MODULE_LIGHTS_INTERFACE

#endif // DEVICE_RGBSTRING_CHRISTMAS_ESP32_OUTSIDETREE_TESTER_02_STATIC_XMAS2022


/**
 * @brief 
 * No WIFI
 * Basic Fallback that creates the desired static colours
 * For installing, it will show blending
 */
#ifdef DEVICE_RGBSTRING_CHRISTMAS_ESP32_OUTSIDETREE_PRIMATY_WIFI_ENABLED_XMAS2022
  #define DEVICENAME_CTR          "xmas_outsidetree"
  #define DEVICENAME_FRIENDLY_CTR "XMAS Outside Tree Wifi Enabled"
  #define DEVICENAME_ROOMHINT_CTR "Outside"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  //#define DISABLE_NETWORK

  #define USE_DEVFEATURE_PRESETS_MANUALUSERCUSTOM_OUTSIDETREE

  #define DISABLE_SERIAL
  #define DISABLE_SERIAL0_CORE
  #define DISABLE_SERIAL_LOGGING


  #define ENABLE_DEVFEATURE_GROUPING_FIX_METHOD_B_XMAS2022

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID


  #define ENABLE_DEVFEATURE_CANSHOW_VERSION3
  #define ENABLE_DEVFEATURE_COLORADO_FORCED_TYPE
  #define ENABLE_DEVFEATURE_FORCED_REMOVE_091122
  #define ENABLE_DEVFEATURE_ANIMATORLIGHT_EVERYLOOP_2022_METHOD


  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS // Not ready to remove
  
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID    
    #define ENABLE_DEVFEATURE_MOVING_GETCOLOUR_AND_PALETTE_TO_RAM
    #define ENABLE_DEVFEATURE_REMOVE_BRIGHTNESS_RANDOMNESS_INSIDE_APPLY_BRIGHTNESS

    #define ENABLE_DEVFEATURE_FIX_STRIPSIZE_LENGTH_ISSUE_XMAS2022

    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEVFEATURE_REMOVE_SEG_STOP_INDEX

    #define ENABLE_DEVFEATURE_FORCED_FRAMERATE_FOR_TRANSITION_SPEED_WITH_WLED_EFFECTS

    // make my own for this

    #define ENABLE_WLED_EFFECTS // probably switch to always enabled going forward

  #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS


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

  #define STRIP_SIZE_MAX 1201 //still needs extra +1
  #ifdef USE_MODULE_LIGHTS_INTERFACE
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"AnimationMode\":\"Effects\","
    "\"ColourOrder\":\"RGB\","
    "\"ColourPalette\":\"Christmas 24\","
    "\"Effects\":{"
      "\"Function\":2," //shimmering palette
      "\"Speed\":127,"
      "\"Intensity\":20,"
      "\"Grouping\":1"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":50"
    "},"    
    "\"BrightnessRGB\":1" // incase uncontrolled reboots
  "}";
  #endif // USE_MODULE_LIGHTS_INTERFACE

#endif // DEVICE_RGBSTRING_CHRISTMAS_ESP32_OUTSIDETREE_TESTER_01_BLENDING_XMAS2022

/**
 * @brief 
 * No WIFI
 * Basic Fallback that creates the desired static colours
 * For installing, it will show blending
 */
#ifdef DEVICE_RGBSTRING_CHRISTMAS_ESP32_TEST_TREE_XMAS2022
  #define DEVICENAME_CTR          "xmas_test_tree_01"
  #define DEVICENAME_FRIENDLY_CTR "XMAS Test Tree"
  #define DEVICENAME_ROOMHINT_CTR "xmas"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  //#define DISABLE_NETWORK

  #define DISABLE_SERIAL
  #define DISABLE_SERIAL0_CORE
  #define DISABLE_SERIAL_LOGGING

  // #define ENABLE_DEVFEATURE_GROUPING_FIX_DEC2022
  // #define ENABLE_DEVFEATURE_GETPIXELCOLOUR_DIRECTLY_DEC2022

  // GROUPING NOT OPERATIONAL

  #define USE_DEVFEATURE_PRESETS_MANUALUSERCUSTOM_TESTTREE

  #define ENABLE_DEVFEATURE_GROUPING_FIX_METHOD_B_XMAS2022


  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID


  #define ENABLE_DEVFEATURE_CANSHOW_VERSION3
  #define ENABLE_DEVFEATURE_COLORADO_FORCED_TYPE
  #define ENABLE_DEVFEATURE_FORCED_REMOVE_091122
  #define ENABLE_DEVFEATURE_ANIMATORLIGHT_EVERYLOOP_2022_METHOD


  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS // Not ready to remove
  
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID    
    #define ENABLE_DEVFEATURE_MOVING_GETCOLOUR_AND_PALETTE_TO_RAM
    #define ENABLE_DEVFEATURE_REMOVE_BRIGHTNESS_RANDOMNESS_INSIDE_APPLY_BRIGHTNESS

    #define ENABLE_DEVFEATURE_FIX_STRIPSIZE_LENGTH_ISSUE_XMAS2022
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEVFEATURE_REMOVE_SEG_STOP_INDEX

    #define ENABLE_DEVFEATURE_FORCED_FRAMERATE_FOR_TRANSITION_SPEED_WITH_WLED_EFFECTS
    #define ENABLE_WLED_EFFECTS // probably switch to always enabled going forward

    #define ENABLE_EXTRA_WLED_EFFECTS

    #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS

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

  #define STRIP_SIZE_MAX 100 //still needs extra +1
  // the stop count is the length of segment and is NOT included in segment
  #ifdef USE_MODULE_LIGHTS_INTERFACE
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"AnimationMode\":\"Effects\","
    "\"ColourOrder\":\"RGB\","
    "\"ColourPalette\":\"Christmas 01\","
    "\"Effects\":{"
      "\"Function\":1," //shimmering palette
      "\"Speed\":255,"
      "\"Intensity\":255,"
      "\"Grouping\":1"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":1000"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // USE_MODULE_LIGHTS_INTERFACE

#endif // DEVICE_RGBSTRING_CHRISTMAS_ESP32_TEST_TREE_XMAS2022


/**
 * @brief 
 * No WIFI
 * Basic Fallback that creates the desired static colours
 * For installing, it will show blending
 */
#ifdef DEVICE_RGBSTRING_CHRISTMAS_ESP32_BEDROOM_CEILING_STRING
  #define DEVICENAME_CTR          "xmas_bedroom_string"
  #define DEVICENAME_FRIENDLY_CTR "XMAS Bedroom String"
  #define DEVICENAME_ROOMHINT_CTR "xmas"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  //#define DISABLE_NETWORK

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  #define USE_DEVFEATURE_PRESETS_MANUALUSERCUSTOM_OUTSIDETREE  // replicating outside for testing

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID


  #define ENABLE_DEVFEATURE_CANSHOW_VERSION3
  #define ENABLE_DEVFEATURE_COLORADO_FORCED_TYPE
  #define ENABLE_DEVFEATURE_FORCED_REMOVE_091122
  #define ENABLE_DEVFEATURE_ANIMATORLIGHT_EVERYLOOP_2022_METHOD


  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS // Not ready to remove
  
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID    
    #define ENABLE_DEVFEATURE_MOVING_GETCOLOUR_AND_PALETTE_TO_RAM
    #define ENABLE_DEVFEATURE_REMOVE_BRIGHTNESS_RANDOMNESS_INSIDE_APPLY_BRIGHTNESS

    #define ENABLE_DEVFEATURE_FIX_STRIPSIZE_LENGTH_ISSUE_XMAS2022
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
    #define ENABLE_DEVFEATURE_REMOVE_SEG_STOP_INDEX
    #define ENABLE_DEVFEATURE_FORCED_FRAMERATE_FOR_TRANSITION_SPEED_WITH_WLED_EFFECTS
    #define ENABLE_WLED_EFFECTS // probably switch to always enabled going forward
    #define ENABLE_EXTRA_WLED_EFFECTS
    #define ENABLE_DEVFEATURE_PALETTE_ENCODING_REWRITE
    #define ENABLE_DEVFEATURE_PALETTE_ENCODING_REWRITE_MQTT_INFO
    // #define ENABLE_DEVFEATURE_PALETTE_LOADED_AS_NEW_CLASS
    // #define ENABLE_DEVFEATURE_PALETTECONTAINER
    #define ENABLE_DEVFEATURE_DEBUG_PALETTE_DATA_LENGTH_MQTT
    #define ENABLE_DEVFEATURE_COLOUR_PALETTE_MERGED
    #define ENABLE_DEVFEATURE_COLOUR_PALETTE_REMOVE_OLD

    #define WLED_DEBUG

    #define ENABLE_DEVFEATURE_NEW_UNIFIED_SEGMENT_STRUCT_DEC2022


    // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items
    // #define ENABLE_LOG_LEVEL_DEBUG

    #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS

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

  #define STRIP_SIZE_MAX 100
  #ifdef USE_MODULE_LIGHTS_INTERFACE
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"AnimationMode\":\"Effects\","
    "\"ColourOrder\":\"RGB\","
    "\"ColourPalette\":\"Christmas 01\","
    "\"Effects\":{"
      "\"Function\":1," //shimmering palette
      "\"Speed\":255,"
      "\"Intensity\":255,"
      "\"Grouping\":1"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":1000"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // USE_MODULE_LIGHTS_INTERFACE

#endif // DEVICE_RGBSTRING_CHRISTMAS_ESP32_TEST_TREE_XMAS2022




/**
 * @brief 
 * No WIFI
 * Basic Fallback that creates the desired static colours
 * For installing, it will show blending
 */
#ifdef DEVICE_RGBSTRING_CHRISTMAS_ESP32_UTILITY_TREE
  #define DEVICENAME_CTR          "xmas_utility_tree"
  #define DEVICENAME_FRIENDLY_CTR "XMAS Utility Tree"
  #define DEVICENAME_ROOMHINT_CTR "xmas"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  //#define DISABLE_NETWORK

  #define DISABLE_SERIAL
  #define DISABLE_SERIAL0_CORE
  #define DISABLE_SERIAL_LOGGING

  #define USE_DEVFEATURE_PRESETS_MANUALUSERCUSTOM_OUTSIDETREE  // replicating outside for testing

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID


  #define ENABLE_DEVFEATURE_CANSHOW_VERSION3
  #define ENABLE_DEVFEATURE_COLORADO_FORCED_TYPE
  #define ENABLE_DEVFEATURE_FORCED_REMOVE_091122
  #define ENABLE_DEVFEATURE_ANIMATORLIGHT_EVERYLOOP_2022_METHOD


  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS // Not ready to remove
  
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID    
    #define ENABLE_DEVFEATURE_MOVING_GETCOLOUR_AND_PALETTE_TO_RAM
    #define ENABLE_DEVFEATURE_REMOVE_BRIGHTNESS_RANDOMNESS_INSIDE_APPLY_BRIGHTNESS

    #define ENABLE_DEVFEATURE_FIX_STRIPSIZE_LENGTH_ISSUE_XMAS2022
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEVFEATURE_REMOVE_SEG_STOP_INDEX
    #define ENABLE_DEVFEATURE_FORCED_FRAMERATE_FOR_TRANSITION_SPEED_WITH_WLED_EFFECTS
    #define ENABLE_WLED_EFFECTS // probably switch to always enabled going forward
    #define ENABLE_EXTRA_WLED_EFFECTS
    #define ENABLE_DEVFEATURE_PALETTE_ENCODING_REWRITE
    #define ENABLE_DEVFEATURE_PALETTE_ENCODING_REWRITE_MQTT_INFO
    // #define ENABLE_DEVFEATURE_PALETTE_LOADED_AS_NEW_CLASS
    // #define ENABLE_DEVFEATURE_PALETTECONTAINER
    #define ENABLE_DEVFEATURE_DEBUG_PALETTE_DATA_LENGTH_MQTT

    #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"16\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      #endif 
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define STRIP_SIZE_MAX 51
  #ifdef USE_MODULE_LIGHTS_INTERFACE
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"AnimationMode\":\"Effects\","
    "\"ColourOrder\":\"GRB\","
    "\"ColourPalette\":\"Christmas 27\","
    "\"Effects\":{"
      "\"Function\":\"Slow Glow\"," //shimmering palette
      "\"Speed\":100,"
      "\"Intensity\":255,"
      "\"Grouping\":1"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":500,"
      "\"RateMs\":1000"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // USE_MODULE_LIGHTS_INTERFACE

#endif // DEVICE_RGBSTRING_CHRISTMAS_ESP32_TEST_TREE_XMAS2022


/**
 * @brief 
 * No WIFI
 * Basic Fallback that creates the desired static colours
 * For installing, it will show blending
 */
#ifdef DEVICE_RGBSTRING_CHRISTMAS_ESP32_UTILITY_CANDLE
  #define DEVICENAME_CTR          "xmas_utility_candle"
  #define DEVICENAME_FRIENDLY_CTR "XMAS Utility Candle"
  #define DEVICENAME_ROOMHINT_CTR "xmas"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  //#define DISABLE_NETWORK

  #define DISABLE_SERIAL
  #define DISABLE_SERIAL0_CORE
  #define DISABLE_SERIAL_LOGGING

  #define USE_DEVFEATURE_PRESETS_MANUALUSERCUSTOM_OUTSIDETREE  // replicating outside for testing

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID


  #define ENABLE_DEVFEATURE_CANSHOW_VERSION3
  #define ENABLE_DEVFEATURE_COLORADO_FORCED_TYPE
  #define ENABLE_DEVFEATURE_FORCED_REMOVE_091122
  #define ENABLE_DEVFEATURE_ANIMATORLIGHT_EVERYLOOP_2022_METHOD


  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS // Not ready to remove

    #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
    #define USE_SK6812_METHOD_DEFAULT
  
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID    
    #define ENABLE_DEVFEATURE_MOVING_GETCOLOUR_AND_PALETTE_TO_RAM
    #define ENABLE_DEVFEATURE_REMOVE_BRIGHTNESS_RANDOMNESS_INSIDE_APPLY_BRIGHTNESS

    #define ENABLE_DEVFEATURE_FIX_STRIPSIZE_LENGTH_ISSUE_XMAS2022
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEVFEATURE_REMOVE_SEG_STOP_INDEX
    #define ENABLE_DEVFEATURE_FORCED_FRAMERATE_FOR_TRANSITION_SPEED_WITH_WLED_EFFECTS
    #define ENABLE_WLED_EFFECTS // probably switch to always enabled going forward
    #define ENABLE_EXTRA_WLED_EFFECTS
    #define ENABLE_DEVFEATURE_PALETTE_ENCODING_REWRITE
    #define ENABLE_DEVFEATURE_PALETTE_ENCODING_REWRITE_MQTT_INFO
    // #define ENABLE_DEVFEATURE_PALETTE_LOADED_AS_NEW_CLASS
    // #define ENABLE_DEVFEATURE_PALETTECONTAINER
    #define ENABLE_DEVFEATURE_DEBUG_PALETTE_DATA_LENGTH_MQTT

    #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"16\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      #endif 
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define STRIP_SIZE_MAX 4
  #ifdef USE_MODULE_LIGHTS_INTERFACE
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"AnimationMode\":\"Effects\","
    "\"ColourOrder\":\"GRB\","
    "\"ColourPalette\":\"Candle Flame 01\","
    "\"Effects\":{"
      "\"Function\":\"Shimmering Palette\"," //shimmering palette
      "\"Speed\":150,"
      "\"Intensity\":150,"
      "\"Grouping\":1"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":23"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // USE_MODULE_LIGHTS_INTERFACE

#endif // DEVICE_RGBSTRING_CHRISTMAS_ESP32_TEST_TREE_XMAS2022

/**
 * @brief 
 * No WIFI
 * Basic Fallback that creates the desired static colours
 * For installing, it will show blending
 */
#ifdef DEVICE_RGBSTRING_CHRISTMAS_ESP32_HALLWAYTREE_XMAS2022
  #define DEVICENAME_CTR          "xmas_hallwaytree"
  #define DEVICENAME_FRIENDLY_CTR "XMAS Hallway Tree"
  #define DEVICENAME_ROOMHINT_CTR "Outside"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  //#define DISABLE_NETWORK

  #define USE_DEVFEATURE_PRESETS_MANUALUSERCUSTOM_OUTSIDETREE

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING


  #define ENABLE_DEVFEATURE_GROUPING_FIX_METHOD_B_XMAS2022

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID


  #define ENABLE_DEVFEATURE_CANSHOW_VERSION3
  #define ENABLE_DEVFEATURE_COLORADO_FORCED_TYPE
  #define ENABLE_DEVFEATURE_FORCED_REMOVE_091122
  #define ENABLE_DEVFEATURE_ANIMATORLIGHT_EVERYLOOP_2022_METHOD


  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS // Not ready to remove
  
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID    
    #define ENABLE_DEVFEATURE_MOVING_GETCOLOUR_AND_PALETTE_TO_RAM
    #define ENABLE_DEVFEATURE_REMOVE_BRIGHTNESS_RANDOMNESS_INSIDE_APPLY_BRIGHTNESS

    #define ENABLE_DEVFEATURE_FIX_STRIPSIZE_LENGTH_ISSUE_XMAS2022

    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEVFEATURE_REMOVE_SEG_STOP_INDEX

    #define ENABLE_DEVFEATURE_FORCED_FRAMERATE_FOR_TRANSITION_SPEED_WITH_WLED_EFFECTS

    #define ENABLE_WLED_EFFECTS // probably switch to always enabled going forward

  #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS


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

  #define STRIP_SIZE_MAX 1201 //still needs extra +1
  #ifdef USE_MODULE_LIGHTS_INTERFACE
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"AnimationMode\":\"Effects\","
    "\"ColourOrder\":\"RGB\","
    "\"ColourPalette\":\"Christmas 24\","
    "\"Effects\":{"
      "\"Function\":1," //shimmering palette
      "\"Speed\":127,"
      "\"Intensity\":20,"
      "\"Grouping\":1"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":900,"
      "\"RateMs\":1000"
    "},"    
    "\"BrightnessRGB\":100" // incase uncontrolled reboots
  "}";
  #endif // USE_MODULE_LIGHTS_INTERFACE

#endif // DEVICE_RGBSTRING_CHRISTMAS_ESP32_HALLWAYTREE_XMAS2022




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

