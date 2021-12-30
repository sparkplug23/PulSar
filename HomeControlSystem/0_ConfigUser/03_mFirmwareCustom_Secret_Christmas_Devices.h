#ifndef MSYSTEMCONFIG_HARDWAREDEFAULTS_FIRMWARE_CHRISTMAS_DEVICES_H
#define MSYSTEMCONFIG_HARDWAREDEFAULTS_FIRMWARE_CHRISTMAS_DEVICES_H

/*********************************************************************************************\
 * Christmas devices: Temporary devices only for the christmas season of 2021, to be removed after
\*********************************************************************************************/

// #define DEVICE_FORCED_TO_BE_TESTER
#define DISABLE_WEBSERVER
// //#define FORCE_TEMPLATE_LOADING

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/Languages/mLanguageDefault.h"
#include "6_Lights/_Interface/mInterfaceLight_Defines.h"
#include "2_CoreSystem/HardwareTemplates/mHardwareTemplates.h"


/**
 * For therese, buttons allow controls
 * nodemcu v3
 * */
//#define DEVICE_RGBSTRING_CONTROLLER_01
// #define DEVICE_RGBSTRING_UTILITY_SHELF
/**
 * Jacqueline and Paula, programmed to be exact, never change
 * nodemuc v3
 * */
// #define DEVICE_RGBSTRING_CONTROLLER_STATIC_PAULA_01
//#define DEVICE_RGBSTRING_CONTROLLER_STATIC_JACQUELINE_01

/**
 * Outside tree
 * DIOT esp32
 * */
// #define DEVICE_OUTSIDETREE_CONTROLLER_BASIC_01
// #define DEVICE_OUTSIDETREE_CONTROLLER_BASIC_02
// #define DEVICE_OUTSIDETREE_CONTROLLER_ESP32_DEBUG
// #define DEVICE_RGBSTRING_OUTSIDE_TREE_SEGMENTS


// #define DEVICE_LIVINGROOM_TREE_WATER_SENSOR

/**
 * Dining Room tree 
 * */
// #define DEVICE_RGBSTRING_DININGROOM_CONTROLLER_01  // RGB Varient
// #define DEVICE_RGBSTRING_DININGROOM_CONTROLLER_02  // GRB Varient

/**
 * Hallway tree 
 * */
// #define DEVICE_RGBSTRING_HALLWAY_CONTROLLER_01


// Include the home devices, which should ONLY contain final hardware
#include "0_ConfigUser/00_mFirmwareCustom_Secret_Home.h"


//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/**
 * For christmas lights to be given to other people as a closed unit
 * Test hardware is nodemcu VERSION 3 (4MB)
 * Push buttons will be placed onto a small bread board on the back, then using heatshrink attached and closed to the nodemcu
 * Buttons: Brightness down, Brightness up (0,10,20,...80,90,92,94,96,98,100) .. or long press just increments!
 *          Basic list of a few animations people might like, but default to static on
 *          Pallette up, palette down
 *          Animation speed up, speed down
 * 
 *          Maybe use one button to mean "option reverse, or, option 2", hence up/down, left/right can be selected by holding "shift" button
 *          Instead of (button+1)*n for each new function, its (button*n)+1
 * */
#ifdef DEVICE_RGBSTRING_CONTROLLER_01
  #define DEVICENAME_CTR          "testbed_rgb_controller_01"
  #define DEVICENAME_FRIENDLY_CTR "RGB testbed_rgb_controller_01 01"

  #define USE_MODULE_SENSORS_BUTTONS
  #define ENABLE_DEVFEATURE_ANIMATOR_BASIC_BUTTON_CONTROLLER // ie Basic button controls for others, christmas controller
  #define USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS
  
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE

  IDEA: for dinning room tree, use second controller (or alternate GRB lights) "inside" the tree on a lower brightness, to add "depth"

  - can a esp32 be used with hardware output for leds? This could solve my issue of having single controller for different led types that can be addressed

  alternatively, I could feed the GRB first, then RGB from it, and thus invert the signal colour order

  #define SETTINGS_HOLDER 59

  //define ENABLE_FORCED_TEMPLATE_LOADING_TO_OVERRIDE_SAVED_SETTINGS  // Previously "FORCE_TEMPLATE_LOADING"

  // #define DISABLE_SETTINGS_STORAGE_OVERRIDE
  // #define ENABLE_XMAS_CONTROLLER_SAVING_IN_EEPROM

  #define DISABLE_NETWORK
  
  #define ENABLE_DEVFEATURE_ADVANCED_SETTINGS_SAVE_DEBUG
  #define ENABLE_DEVFEATURE_SAVE_REBOOT_COMMAND_FOR_SETTINGS_TESTING
  #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
  #define ENABLE_DEVFEATURE_SETTINGS_SAVED_USED_CORRECTLY_AND_PROGMEM_TEMPLATES_ARE_ONLY_READ_WHEN_SETTINGS_HOLDER_CHANGES // ie works like tas
  // Based on tasmota saving original vs new esp32 enabled
  #define ENABLE_SETTINGS_VERSION_1_ESP8266_ONLY
  // #define ENABLE_SETTINGS_VERSION_2_ESP32_JOINT_HARDWARE

  /**
   * Three types of animations, exclusive only
   * */
  
  // #define USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
  // #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

  /**
   * I might hard code the button config into eeprom, instead of fixing the complex saving method until next year.
   * Load in animator, save every 5 seconds
   * */
  #define ENABLE_TEMPFEATURE_BASIC_SAVING_XMAS_CONTROLLER_SETTINGS

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"D0\":\"" D_GPIO_FUNCTION_KEY8_INV_CTR  "\"," // 3V for high
      "\"D1\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"D2\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"D3\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\","
      "\"D4\":\"" D_GPIO_FUNCTION_KEY4_INV_CTR  "\","
      "\"D5\":\"" D_GPIO_FUNCTION_KEY5_INV_CTR  "\","
      "\"D6\":\"" D_GPIO_FUNCTION_KEY6_INV_CTR  "\","
      "\"D7\":\"" D_GPIO_FUNCTION_KEY7_INV_CTR  "\","
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

 #define STRIP_SIZE_MAX 100
 #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"rgb\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\""
    // "\"" D_JSON_EFFECTS "\":{" 
    //   "\"Function\":\"Static Glow\""
    // "},"    
    // "\"" D_JSON_TRANSITION       "\":{"
    //   // "\"" D_JSON_TIME_MS "\":500,"
    //   // "\"" D_JSON_RATE_MS "\":2000,"
    //   // "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":10,"
    //   "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
    // "}"
    // "\"ColourPalette\":\"Christmas MultiColoured Warmer\","
    // "\"BrightnessRGB\":0"
  "}";

#endif

#ifdef DEVICE_RGBSTRING_UTILITY_SHELF
  #define DEVICENAME_CTR          "rgbstring_xmas_utility"
  #define DEVICENAME_FRIENDLY_CTR "RGB Notifications 01"
  

  #define USE_DEVFEATURE_METHOD_SEGMENTS_BUILD
  //#define USE_DEVFEATURE_METHOD_HACS_LEGACY_BUILD
  // #define USE_DEVFEATURE_METHOD_WLED_BUILD

  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
 
 
  #ifdef USE_DEVFEATURE_METHOD_SEGMENTS_BUILD
    #define USE_BUILD_TYPE_LIGHTING
    #define USE_MODULE_LIGHTS_INTERFACE
    #define USE_MODULE_LIGHTS_ANIMATOR
    #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT
    //#define LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
    // #define LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS_FOR_UTILITY
    #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_UTILITY
    #define D_EFFECT_INSIDE_TEMPLATE "Effects"
    // enable some wled conerted aniamtions
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    #define DEBUG_WLED_EFFECT_FUNCTIONS
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
    // #define USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION  // to test existing effects in wled
    
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
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
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
 #define STRIP_SIZE_MAX 50
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


  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_UTILITY
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":10," 
    "\"Effects\":{"
      "\"Function\":109"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":20"
    "},"    
    "\"Hue\":12,"
    "\"Sat\":100,"
    "\"CCT_TempPercentage\":100,"
    "\"BrightnessCCT\":100,"
    "\"BrightnessRGB\":100"
  "}";

//   {
//     "PixelRange": [
//       0,
//       49
//     ],
//     "Effects": {
//       "Function": 109,
//       "Speed": 127,
//       "Current": 127,
//       "Palette": "Christmas 21",
//       "Intensity": 127,
//       "Mode": 6
//     },
//     "ColourPalette": 10,
//     "Hue":20,
//     "Sat":100,
//     "BrightnessCCT":100,
//     "CCT_TempPercentage":100,
//     "ColourOrder": "grbwc",
//     "Transition": {
//       "TimeMs": 900,
//       "RateMs": 20
//     },
//     "BrightnessRGB": 100
// }




  #endif // LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS


  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
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




#ifdef DEVICE_RGBSTRING_TESTBED_UTILITY_TREE
  #define DEVICENAME_CTR          "utility_xmas_tree"
  #define DEVICENAME_FRIENDLY_CTR "RGB Notifications 01"
  

  #define USE_DEVFEATURE_METHOD_SEGMENTS_BUILD
  //#define USE_DEVFEATURE_METHOD_HACS_LEGACY_BUILD
  // #define USE_DEVFEATURE_METHOD_WLED_BUILD
 
 
  #ifdef USE_DEVFEATURE_METHOD_SEGMENTS_BUILD
    #define USE_BUILD_TYPE_LIGHTING
    #define USE_MODULE_LIGHTS_INTERFACE
    #define USE_MODULE_LIGHTS_ANIMATOR
    #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT
    // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
    // #define LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS_FOR_UTILITY
    #define D_EFFECT_INSIDE_TEMPLATE "Effects"
    // enable some wled conerted aniamtions
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    #define DEBUG_WLED_EFFECT_FUNCTIONS
    #define ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL

    #define ENABLE_DEVFEATURE_PHASE_OUT_LEDORDERARRAY
    #define ENABLE_DEVFEATURE_PHASE_OUT_ANIMATIONCOLOUR_STRUCT
    #define ENABLE_FREERAM_APPENDING_SERIAL

    #define STRIP_SIZE_MAX 50

#define LIGHTING_TEMPLATE_SINGLE_SEGMENT_UTILITY_TREE
    /**
     * @brief 
     * structs in headers
     * 13k to 18k
     * 
     */
  
    // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS

  #endif 

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
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
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
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

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_UTILITY_TREE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 06\"," 
    "\"Effects\":{"
      "\"Function\":85" //shimmering pal
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":30"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS

#endif






#ifdef DEVICE_RGBSTRING_OUTSIDE_TREE_SEGMENTS
  #define DEVICENAME_CTR          "outside_tree_segments"
  #define DEVICENAME_FRIENDLY_CTR "ESP32 Outside Tree Segments"

  #define DISABLE_NETWORK  

  #define USE_DEVFEATURE_METHOD_SEGMENTS_BUILD
  //#define USE_DEVFEATURE_METHOD_HACS_LEGACY_BUILD
  // #define USE_DEVFEATURE_METHOD_WLED_BUILD
 
 
  #ifdef USE_DEVFEATURE_METHOD_SEGMENTS_BUILD
    #define USE_BUILD_TYPE_LIGHTING
    #define USE_MODULE_LIGHTS_INTERFACE
    #define USE_MODULE_LIGHTS_ANIMATOR
    #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT
    // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
    // #define LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS_FOR_UTILITY
    #define D_EFFECT_INSIDE_TEMPLATE "Effects"
    // enable some wled conerted aniamtions
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    #define DEBUG_WLED_EFFECT_FUNCTIONS
    #define ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL

    #define ENABLE_DEVFEATURE_PHASE_OUT_LEDORDERARRAY
    #define ENABLE_DEVFEATURE_PHASE_OUT_ANIMATIONCOLOUR_STRUCT
    #define ENABLE_FREERAM_APPENDING_SERIAL

    #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_OUTSIDE_TREE_BASIC_STATIC

    #define  ENABLE_DEVFEATURE_INCLUDE_WLED_PALETTES
    #define  ENABLE_DEVFEATURE_INCLUDE_WLED_PRIMARY_COLOUR_OPTIONS

    #define ENABLE_CRGBPALETTES_IN_PROGMEM

    #define STRIP_SIZE_MAX 1300

    /**
     * @brief 
     * structs in headers
     * 13k to 18k
     * 
     */
  
    // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS

  #endif 
  #ifdef USE_DEVFEATURE_METHOD_HACS_LEGACY_BUILD
 #define STRIP_SIZE_MAX 50
    #define USE_BUILD_TYPE_LIGHTING
    #define USE_MODULE_LIGHTS_INTERFACE
    #define USE_MODULE_LIGHTS_ANIMATOR
    #define USE_MODULE_LIGHTS_ADDRESSABLE
    
    #define LIGHTING_TEMPLATE_SINGLE_SEGMENT
    #define D_EFFECT_INSIDE_TEMPLATE "Effects"
  #endif 
//   #ifdef USE_DEVFEATURE_METHOD_WLED_BUILD
//  #define STRIP_SIZE_MAX 50
//     #define USE_BUILD_TYPE_LIGHTING
//     #define USE_MODULE_LIGHTS_INTERFACE
//     #define USE_MODULE_LIGHTS_ANIMATOR
//     #define USE_MODULE_LIGHTS_ADDRESSABLE
//     #define USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION  // to test existing effects in wled
    
//     // #define LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS
//     #define LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS
//     // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT
//     #define D_EFFECT_INSIDE_TEMPLATE "WLED"
//     #define DEBUG_WLED_EFFECT_FUNCTIONS
//   #endif


  #define USE_MODULE_TEMPLATE


  



  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      // "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
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

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_OUTSIDE_TREE_BASIC_STATIC
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 06\"," 
    "\"Effects\":{"
      "\"Function\":83,"
      "\"Instensity\":127"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":25"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS




  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
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
      "\"TimeMs\":0,"
      "\"RateMs\":1000"
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




/**
 * For christmas lights to be given to other people as a closed unit
 * Test hardware is nodemcu VERSION 3 (4MB)
 * For jacq/paula
 * Manually pick the colours, then hardcode, it should only show that on static (in order)
 * they might want the slow glow, maybe use a wire/jumper for it
 * */
#ifdef DEVICE_RGBSTRING_CONTROLLER_STATIC_01
  #define DEVICENAME_CTR          "rgb_static_controller_01"
  #define DEVICENAME_FRIENDLY_CTR "RGB Static Controller 01"

  // #define USE_MODULE_SENSORS_BUTTONS
  // #define ENABLE_DEVFEATURE_ANIMATOR_BASIC_BUTTON_CONTROLLER // ie Basic button controls for others, christmas controller
  // #define USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS
  
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE

  #define SETTINGS_HOLDER 1

  //define ENABLE_FORCED_TEMPLATE_LOADING_TO_OVERRIDE_SAVED_SETTINGS  // Previously "FORCE_TEMPLATE_LOADING"

  // #define DISABLE_SETTINGS_STORAGE_OVERRIDE
  // #define ENABLE_XMAS_CONTROLLER_SAVING_IN_EEPROM

  // #define DISABLE_NETWORK
  
  // #define ENABLE_DEVFEATURE_ADVANCED_SETTINGS_SAVE_DEBUG
  // #define ENABLE_DEVFEATURE_SAVE_REBOOT_COMMAND_FOR_SETTINGS_TESTING
  // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
  // #define ENABLE_DEVFEATURE_SETTINGS_SAVED_USED_CORRECTLY_AND_PROGMEM_TEMPLATES_ARE_ONLY_READ_WHEN_SETTINGS_HOLDER_CHANGES // ie works like tas
  // Based on tasmota saving original vs new esp32 enabled
  // #define ENABLE_SETTINGS_VERSION_1_ESP8266_ONLY
  // #define ENABLE_SETTINGS_VERSION_2_ESP32_JOINT_HARDWARE

  /**
   * Three types of animations, exclusive only
   * */
  
  // #define USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
  // #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

  /**
   * I might hard code the button config into eeprom, instead of fixing the complex saving method until next year.
   * Load in animator, save every 5 seconds
   * */
  #define ENABLE_TEMPFEATURE_BASIC_SAVING_XMAS_CONTROLLER_SETTINGS

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


  // #define USE_CUSTOM_USER_PAULA
  #define USE_CUSTOM_USER_JACQUELINE


  #ifdef USE_CUSTOM_USER_PAULA
  #define STRIP_SIZE_MAX 100
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
      "\"" D_JSON_RATE_MS "\":2000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":10,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
    "},"
    "\"ColourPalette\":\"Custom User 01\","
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef USE_CUSTOM_USER_JACQUELINE
  #define STRIP_SIZE_MAX 100
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
      "\"" D_JSON_TIME_MS "\":10,"
      "\"" D_JSON_RATE_MS "\":1000,"
    "},"
    "\"ColourPalette\":\"Custom User 01\","
    "\"BrightnessRGB\":50"
  "}";
  #endif

#endif


/**
 * For christmas lights to be given to other people as a closed unit
 * Test hardware is nodemcu VERSION 3 (4MB)
 * For jacq/paula
 * Manually pick the colours, then hardcode, it should only show that on static (in order)
 * they might want the slow glow, maybe use a wire/jumper for it
 * */
#ifdef DEVICE_RGBSTRING_CONTROLLER_STATIC_JACQUELINE_01
  #define DEVICENAME_CTR          "rgb_static_jacq_01"
  #define DEVICENAME_FRIENDLY_CTR "RGB Static Controller 01"

  #define USE_CUSTOM_USER_JACQUELINE

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  

  #define DISABLE_NETWORK

  #define SETTINGS_HOLDER 1

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define STRIP_SIZE_MAX 100
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":\"Static Glow\""
    "},"    
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":1000,"
      "\"" D_JSON_RATE_MS "\":5000,"
    "},"
    "\"ColourPalette\":\"Custom User 01\","
    "\"BrightnessRGB\":100"
  "}";

#endif


/**
 * For christmas lights to be given to other people as a closed unit
 * Test hardware is nodemcu VERSION 3 (4MB)
 * For jacq/paula
 * Manually pick the colours, then hardcode, it should only show that on static (in order)
 * they might want the slow glow, maybe use a wire/jumper for it
 * */
#ifdef DEVICE_RGBSTRING_CONTROLLER_STATIC_PAULA_01
  #define DEVICENAME_CTR          "rgb_static_paula_01"
  #define DEVICENAME_FRIENDLY_CTR "RGB Static Controller 01"

  #define USE_CUSTOM_USER_PAULA

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  
  
  #define DISABLE_NETWORK

  #define SETTINGS_HOLDER 1

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define STRIP_SIZE_MAX 100
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":\"Static Glow\""
    "},"    
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":1000,"
      "\"" D_JSON_RATE_MS "\":5000,"
    "},"
    "\"ColourPalette\":\"Custom User 01\","
    "\"BrightnessRGB\":40"
  "}";

#endif


/**
 * For christmas lights to be given to other people as a closed unit
 * Test hardware is nodemcu VERSION 3 (4MB)
 * For jacq/paula
 * Manually pick the colours, then hardcode, it should only show that on static (in order)
 * they might want the slow glow, maybe use a wire/jumper for it
 * */
#ifdef DEVICE_RGBSTRING_CONTROLLER_STATIC_THERESE_RGB_01
  #define DEVICENAME_CTR          "rgb_static_therese_01"
  #define DEVICENAME_FRIENDLY_CTR "RGB Static Controller 01"

  #define USE_CUSTOM_USER_PAULA

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  

  #define DISABLE_NETWORK

  #define SETTINGS_HOLDER 1

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define STRIP_SIZE_MAX 100
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":\"Static Glow\""
    "},"    
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":1000,"
      "\"" D_JSON_RATE_MS "\":5000,"
    "},"
    "\"ColourPalette\":\"Custom User 01\","
    "\"BrightnessRGB\":100"
  "}";

#endif

/**
 * For christmas lights to be given to other people as a closed unit
 * Test hardware is nodemcu VERSION 3 (4MB)
 * For jacq/paula
 * Manually pick the colours, then hardcode, it should only show that on static (in order)
 * they might want the slow glow, maybe use a wire/jumper for it
 * */
#ifdef DEVICE_RGBSTRING_CONTROLLER_STATIC_THERESE_GRB_01
  #define DEVICENAME_CTR          "grb_static_therese_01"
  #define DEVICENAME_FRIENDLY_CTR "GRB Static Controller 01"

  #define USE_CUSTOM_USER_PAULA

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  

  #define DISABLE_NETWORK

  #define SETTINGS_HOLDER 1

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define STRIP_SIZE_MAX 100
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":\"Static Glow\""
    "},"    
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":1000,"
      "\"" D_JSON_RATE_MS "\":5000,"
    "},"
    "\"ColourPalette\":\"Custom User 01\","
    "\"BrightnessRGB\":100"
  "}";

#endif



/**
 * Outside tree, ie testbed for esp32 controller with wifi if possible
 * */
#ifdef DEVICE_OUTSIDETREE_CONTROLLER_BASIC_01
  // #define DEVICENAME_CTR          "testbed_rgboutide_tree_controller_01"
  #define DEVICENAME_CTR          "outsidetree_01"
  #define DEVICENAME_FRIENDLY_CTR "Outside Tree ESP32 Basic" // Basic version used until I redo the animations later

  /***
   * Use bh1520 lux sensor to control brightness
   * 
   * 
   * ie, goes from upper boundary (before dark), then slowly drops brightness.
   * probably better to use sun elevation for this instead? ie <10 degrees below horizon, then turn brightness down
   * */


  /***                               Total
   * 4 sets of 5v black              (200)   4
   * 7 sets of 5v green              (350)   7
   * 15 sets of 12v green            (750) ONE FAULT    14
   * 
   *                                 (1300)  used to be 1000?   25
   * */

  // #define USE_MODULE_SENSORS_BUTTONS
  // #define ENABLE_DEVFEATURE_ANIMATOR_BASIC_BUTTON_CONTROLLER // ie Basic button controls for others, christmas controller
  // #define USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS

  #define ENABLE_DEVFEATURE_RELAY_CONTROLLED_VIA_ANIMATIONS_OUTPUT

  #define USE_MODULE_DRIVERS_RELAY
  #define MAX_RELAYS 1
  #define USE_MODULE_DRIVERS_INTERFACE


  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  

  #define SETTINGS_HOLDER 1

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"23\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      "\"22\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\"" // not using "un"inverted, as I want the power to stay on by default
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

                                                                  
  #define STRIP_SIZE_MAX 1400 // (26 sets)
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":\"Static Glow\""
    "},"    
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":0,"
      "\"" D_JSON_RATE_MS "\":1000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":10,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\""
    "},"
    "\"ColourPalette\":\"Christmas MultiColoured Warmer\","
    "\"BrightnessRGB\":0"
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

#endif


/**
 * Outside tree, ie testbed for esp32 controller with wifi if possible
 * */
#ifdef DEVICE_OUTSIDETREE_CONTROLLER_ESP32_DEBUG
  // #define DEVICENAME_CTR          "testbed_rgboutide_tree_controller_01"
  #define DEVICENAME_CTR          "outsidetree_esp32_debug"
  #define DEVICENAME_FRIENDLY_CTR "Outside Tree ESP32 Basic" // Basic version used until I redo the animations later

  #define USE_DEVFEATURE_METHOD_SEGMENTS_BUILD
  //#define USE_DEVFEATURE_METHOD_HACS_LEGACY_BUILD
  // #define USE_DEVFEATURE_METHOD_WLED_BUILD
 
 
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
    #define ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL

    // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS

  #endif 



  // /***
  //  * Use bh1520 lux sensor to control brightness
  //  * 
  //  * 
  //  * ie, goes from upper boundary (before dark), then slowly drops brightness.
  //  * probably better to use sun elevation for this instead? ie <10 degrees below horizon, then turn brightness down
  //  * */


  // /***                               Total
  //  * 4 sets of 5v black              (200)   4
  //  * 7 sets of 5v green              (350)   7
  //  * 15 sets of 12v green            (750) ONE FAULT    14
  //  * 
  //  *                                 (1300)  used to be 1000?   25
  //  * */

  // // #define USE_MODULE_SENSORS_BUTTONS
  // // #define ENABLE_DEVFEATURE_ANIMATOR_BASIC_BUTTON_CONTROLLER // ie Basic button controls for others, christmas controller
  // // #define USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS

  // #define ENABLE_DEVFEATURE_RELAY_CONTROLLED_VIA_ANIMATIONS_OUTPUT

  // #define USE_MODULE_DRIVERS_RELAY
  // #define MAX_RELAYS 1
  // #define USE_MODULE_DRIVERS_INTERFACE


  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define USE_MODULE_LIGHTS_ADDRESSABLE
  

  // #define SETTINGS_HOLDER 1

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"23\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      "\"22\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\"" // not using "un"inverted, as I want the power to stay on by default
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

                                                                  
  #define STRIP_SIZE_MAX 1300 // (26 sets)
  #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
  //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  //   "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","
  //   "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
  //   "\"" D_JSON_EFFECTS "\":{" 
  //     "\"Function\":\"Static Glow\""
  //   "},"    
  //   "\"" D_JSON_TRANSITION       "\":{"
  //     "\"" D_JSON_TIME_MS "\":0,"
  //     "\"" D_JSON_RATE_MS "\":1000,"
  //     "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":10,"
  //     "\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\""
  //   "},"
  //   "\"ColourPalette\":\"Christmas MultiColoured Warmer\","
  //   "\"BrightnessRGB\":0"
  // "}";

#define LIGHTING_TEMPLATE_DEBUG_SINGLE_PIXEL


  #ifdef LIGHTING_TEMPLATE_DEBUG_SINGLE_PIXEL
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":," 
    "\"ColourPalette\":\"Christmas 06\"," 
    "\"Effects\":{"
      "\"Function\":1"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":3000,"
      "\"RateMs\":10000"
    "},"    
    "\"BrightnessRGB\":100"
  "}"
  
  
  // R"=====({"PixelRange":[0,1000],
  // "AnimationMode": 3,
  // "Strip": {
  //   "ClearTo": [0,0,0,0,0],
  //   "ManualSetPixelToScene":[0,1,2,3,4,5,40,41,42,43,44,45,46,47,48,49]
  // },
  // "Effects": {
  //   "Function": 109
  // },
  // "ColourPalette": 10,
  // "Hue": 20,
  // "Sat": 100,
  // "BrightnessCCT": 100,
  // "CCT_TempPercentage": 100,
  // "ColourOrder": "grbwc",
  // "Transition": {
  //   "TimeMs": 900,
  //   "RateMs": 20
  // },
  // "BrightnessRGB": 100"})====="


  ;


  #endif


  
  // #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Socket"
  
  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  // "{"
  //   "\"" D_JSON_DEVICENAME "\":{"
  //     "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
  //       "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
  //     "]"
  //   "}"
  // "}";

#endif



/**
 * Outside tree, ie testbed for esp32 controller with wifi if possible
 * */
#ifdef DEVICE_OUTSIDETREE_CONTROLLER_BASIC_02
  // #define DEVICENAME_CTR          "testbed_rgboutide_tree_controller_01"
  #define DEVICENAME_CTR          "outsidetree_02"
  #define DEVICENAME_FRIENDLY_CTR "Outside Tree ESP32 Basic" // Basic version used until I redo the animations later

  /***
   * Use bh1520 lux sensor to control brightness
   * 
   * 
   * ie, goes from upper boundary (before dark), then slowly drops brightness.
   * probably better to use sun elevation for this instead? ie <10 degrees below horizon, then turn brightness down
   * */


  /***                               Total
   * 4 sets of 5v black              (200)   4
   * 7 sets of 5v green              (350)   7
   * 15 sets of 12v green            (750) ONE FAULT    14
   * 
   *                                 (1300)  used to be 1000?   25
   * */

  // #define USE_MODULE_SENSORS_BUTTONS
  // #define ENABLE_DEVFEATURE_ANIMATOR_BASIC_BUTTON_CONTROLLER // ie Basic button controls for others, christmas controller
  // #define USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS

  // #define ENABLE_DEVFEATURE_RELAY_CONTROLLED_VIA_ANIMATIONS_OUTPUT

  #define USE_MODULE_DRIVERS_RELAY
  #define MAX_RELAYS 1
  #define USE_MODULE_DRIVERS_INTERFACE


  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  

  // #define SETTINGS_HOLDER 1

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"23\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      "\"22\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\"" // not using "un"inverted, as I want the power to stay on by default
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

                                                                  
  #define STRIP_SIZE_MAX 1400 // (26 sets)
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":\"Static Glow\""
    "},"    
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":0,"
      "\"" D_JSON_RATE_MS "\":1000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":10,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\""
    "},"
    "\"ColourPalette\":\"Christmas MultiColoured Warmer\","
    "\"BrightnessRGB\":0"
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

#endif



/**
 * Outside tree, ie testbed for esp32 controller with wifi if possible
 * */
#ifdef DEVICE_OUTSIDETREE_CONTROLLER_ADVANCED_01
  // #define DEVICENAME_CTR          "testbed_rgboutide_tree_controller_01"
  #define DEVICENAME_CTR          "outsidetree_01"
  #define DEVICENAME_FRIENDLY_CTR "Outside Tree ESP32 Basic" // Basic version used until I redo the animations later

  /***
   * Use bh1520 lux sensor to control brightness
   * 
   * 
   * ie, goes from upper boundary (before dark), then slowly drops brightness.
   * probably better to use sun elevation for this instead? ie <10 degrees below horizon, then turn brightness down
   * */


  /***                               Total
   * 4 sets of 5v black              (200)
   * 7 sets of 5v green              (350)
   * 15 sets of 12v green            (750)
   * 
   *                                 (1300)  used to be 1000?
   * */

  // #define USE_MODULE_SENSORS_BUTTONS
  // #define ENABLE_DEVFEATURE_ANIMATOR_BASIC_BUTTON_CONTROLLER // ie Basic button controls for others, christmas controller
  // #define USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS

  #define ENABLE_DEVFEATURE_RELAY_CONTROLLED_VIA_ANIMATIONS_OUTPUT

  #define USE_MODULE_DRIVERS_RELAY
  #define MAX_RELAYS 1
  #define USE_MODULE_DRIVERS_INTERFACE


  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  

  #define SETTINGS_HOLDER 1

  // #define ENABLE_PIXEL_FUNCTION_MIXER

  // #define   DEBUG_MODULE_TIME_STD

  //define ENABLE_FORCED_TEMPLATE_LOADING_TO_OVERRIDE_SAVED_SETTINGS  // Previously "FORCE_TEMPLATE_LOADING"

  // #define DISABLE_SETTINGS_STORAGE_OVERRIDE
  // #define ENABLE_XMAS_CONTROLLER_SAVING_IN_EEPROM

  // #define DISABLE_NETWORK
  
  // #define ENABLE_DEVFEATURE_ADVANCED_SETTINGS_SAVE_DEBUG
  // #define ENABLE_DEVFEATURE_SAVE_REBOOT_COMMAND_FOR_SETTINGS_TESTING
  // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
  // #define ENABLE_DEVFEATURE_SETTINGS_SAVED_USED_CORRECTLY_AND_PROGMEM_TEMPLATES_ARE_ONLY_READ_WHEN_SETTINGS_HOLDER_CHANGES // ie works like tas
  // // Based on tasmota saving original vs new esp32 enabled
  // #define ENABLE_SETTINGS_VERSION_1_ESP8266_ONLY
  // #define ENABLE_SETTINGS_VERSION_2_ESP32_JOINT_HARDWARE

  /**
   * Three types of animations, exclusive only
   * */
  // #define USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
  // #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

  // #define ENABLE_PIXEL_FUNCTION_MIXER // wait until december to fix this, as basic slow glow changes are enough for outside.

  //#define ENABLE_FREERAM_APPENDING_SERIAL

  /**
   * I might hard code the button config into eeprom, instead of fixing the complex saving method until next year.
   * Load in animator, save every 5 seconds
   * */
  // #define ENABLE_TEMPFEATURE_BASIC_SAVING_XMAS_CONTROLLER_SETTINGS

  // 256 leds = 75w so 15A

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"23\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

                                                                  
  #define STRIP_SIZE_MAX 1300 // (26 sets)
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"grb\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":\"Slow Glow\""
    "},"    
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":5000,"
      "\"" D_JSON_RATE_MS "\":10000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":10,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\""
    "},"
    "\"ColourPalette\":\"Christmas 01\","
    "\"BrightnessRGB\":70"

    // // Dev testing
    // "\"Mixer\":{"
    //   "\"Enabled\":0,"
    //   // "\"" D_JSON_RUNNING_ID "\":7,"
    //   "\"" D_JSON_TIME_SCALER_AS_PERCENTAGE "\":10"
    // "},"
    // "\"PixelGrouping\":{"
    //   // "\"Enabled\":\"Off\","
    //   "\"Mode\":1"
    // "}"
  "}";

  
  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "PowerSupplies"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

// #define ENABLE_FEATURE_PIXEL_GROUP_MULTIPLIERS
  













// //keep this, as esp32 test device 
//   #define DEVICENAME_CTR          "rgboutsidetree"
//   #define DEVICENAME_FRIENDLY_CTR "Outside Tree"

//   //#define ESP32

//   /*
  
//   use pin held low to disable wifi if possible, so I can test wifi things
//   make it if low, enable wifi

// at minimum, I can swap two esp32w around, one with and without wifi, as they wont cause neetwork name issues


// add new function, that sets output via progmem stored directly


// add function that enables ,qtt control per pixel, which uses setpixel (no naimator) to control ouptu
// use to work out rows, but also write jay and evans names, then sore htem in progmem


// using adc and ldr, enable "sweep" that lights each pixel until it stops on the one the ldr is against, and have that pixel count print over serial every second
// have methods : write one, and stop on ldr
// with mqqt flag, write then light all subsequent pixels found
// with another, deductive method, turn off pixels found

// in all cases, in array form, print the pixels lit over serial every seround and over mqtt!!

// use car rad test push buttons, try adding ldr to it
// use black etherhetn czble in dads rooms for it
  
//   */

//   #define ENABLE_GAMMA_BRIGHTNESS_ON_DESIRED_COLOUR_GENERATION

//   #define USE_PM_OUTSIDE_TREE_MIXER_DESCRIPTION
//   DEFINE_PGM_CTR(PM_OUTSIDE_TREE_MIXER_DESCRIPTION)  
//   {  // movement, order, flasher, colours,   rate/time, mutlipler, (One look explanation for design)
//     // Single basic
//     "%d, Stat, InOrdr, SlowGlow, C 09 (basic), Single, %d/%d, %d \n\r(Simple Static)" "|" // 1
//     "%d, Medm, Random, SlowGlow, C 09 (basic), Single, %d/%d, %d, \n\r(Random Blending with same colour)" "|" // 2
//     "%d, Fast, Random, SlowGlow, C 10 (many),  Single, %d/%d, %d, \n\r(Fast random instant switching, more colours)" "|" // 3
//     // Rows
//     "%d, Slow, Random, SlowGlow, C 11 (many),  Rows, %d/%d, %d, \n\r(Rows, slow random blending)" "|" // 4
//     "%d, Fast, Random, SlowGlow, C 11 (many),  Rows, %d/%d, %d, \n\r(Rows, fast random blending)" "|" // 5
//     "%d, Stat, InOrdr, SlowGlow, C 09 (basic), Rows, %d/%d, %d, \n\r(Rows, Static InOrder)" "|" // 6  
//     "%d, Slow, InOrdr, Sequentl, C 09 (basic), Rows, %d/%d, %d, \n\r(Rows, Now rotating)" "|" // 7
//     // Berry green
//     // "%d, Stat, InOrdr, SlowGlow, Berry Green,  Rows, %d/%d, %d, \n\r(Rows, Berry green static)" "|" // 8
//     // "%d, Stat, InOrdr, SlowGlow, Berry Green,  Single, %d/%d, %d, \n\r(Red every 4th pixel)" "|" // 9
//     // "%d, Fast, InOrdr, TwinkleC, C 10 (many),  Single, %d/%d, %d, \n\r(Simple Static)" "|" // 10
//     // Twinkle
//     "%d, Slow, Random, SlowGlow, C 10 (many),  Single, %d/%d, %d, \n\r(Simple Static)" "|" // 11
//     // Step through palette
//     "%d, Slow, Random, SlowGlow, C 13 (g adj), Single, %d/%d, %d, \n\r(2 colour stepping)" "|" // 12
//     "%d, Slow, Random, SlowGlow, C 12 (hall),  Single, %d/%d, %d, \n\r(2 colour stepping)" "|" // 13
//     // Random single, with dark colours as most
//     "%d, Slow, Random, SlowGlow, C 14 (most dark), Single, %d/%d, %d, \n\r(Single, fast random)" "|" // 14
//     // Returning to normal, but new hallway tree colours
//     "%d, Slow, Random, SlowGlow, C 12 (Hallway), Single, %d/%d, %d, \n\r(Simple Static)" "|" // 15
//   };


  // #define ENABLE_FEATURE_PIXEL_GROUP_MULTIPLIERS
  // #define USE_DEVFEATURE_PIXEL_OUTPUT_MULTIPLIER 2

//   //#define FORCE_TEMPLATE_LOADING
//   #define SETTINGS_HOLDER 1

//   // #define ENABLE_BUG_TRACING
//   // #define DEBUG_PRINT_FUNCTION_NAME

// //  #define ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL


//   //#define ENABLE_LOG_FILTERING_TEST_ONLY
//   #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
//   // #define USE_DEBUG_PRINT_FUNCTION_NAME_TEST
//   #define DISABLE_WEBSERVER 
  
//   #define ENABLE_PIXEL_FUNCTION_MIXER
//   
//   //#define ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL

//   #define STRIP_SIZE_MAX 1050// 750   *15 //changing gazebo to be 12v

//   #define DISABLE_NETWORK
//   // #define DISABLE_FOR_FAULTY_ESP32_FLICKERING

//   #define DISABLE_SLEEP

//   // #define USE_WS28XX_METHOD_RMT0_800KBPS_ESP32
    
//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR

//   // #define ENABLE_DEVFEATURE_LIGHTING_//Scene_OBJECT_TO_STRUCT "v78.24.11+" //only remove when all device exceed this
//   // #define ENABLE_DEVFEATURE_RGBCOLOR_DESIRED
//   //#define ENABLE_DEVFEATURE_SINGLE_ANIMATOR_INTERFACE   "v79.31.22+"   

//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE)   
//   "{"
//     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
//     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_JSON_GPIOC "\":{"
//       "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR   "\""
//     "},"
//     "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
//   "}";

//   #define USE_LIGHTING_TEMPLATE
//   DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   "{"
//     "\"" D_JSON_HARDWARE_TYPE    "\":\"" D_JSON_WS2812 "\","
//     #ifdef STRIP_SIZE_MAX
//     "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//     #else
//     "\"" D_JSON_STRIP_SIZE       "\":50,"
//     #endif //STRIP_SIZE_MAX
//     "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","
//     "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
//     "\"" D_JSON_COLOUR_PALETTE   "\":43,"
//     "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
//     // "\"" D_JSON_//Scene_COLOUR     "\":{\"" D_JSON_HSB    "\":[15,95,0]},"
//     // "\"" D_JSON_BRIGHTNESS       "\":0,"
//     // "\"" D_JSON_BRIGHTNESS_RGB          "\":0,"

//     // new commands I want to run during boot

//     "\"AnimationMode\":\"Flasher\","
//     // "\"Flasher\":{"
//     //   "\"Function\":11,"
//     //   "\"AgedColouring\":0"
//     // "},"
//     "\"Mixer\":{"
//       "\"Enabled\":1,"
//       // "\"" D_JSON_RUNNING_ID "\":7,"
//       "\"" D_JSON_TIME_SCALER_AS_PERCENTAGE "\":100"
//     "},"
//     "\"PixelGrouping\":{"
//       // "\"Enabled\":\"Off\","
//       "\"Mode\":3"
//     "},"
//     "\"Transition\":{\"Order\":\"Random\",\"PixelUpdatePerc\":100},"
//     "\"" D_JSON_BRIGHTNESS_RGB "\":20"

//   "}";



#endif



/**
 * User:      Me
 * Location:  Diningroom tree, new LEDs
 * Networked: Yes
 * */
#ifdef DEVICE_RGBSTRING_DININGROOM_CONTROLLER_01
  #define DEVICENAME_CTR          "xmas_dinigroom_01"
  #define DEVICENAME_FRIENDLY_CTR "RGB Static Controller 01"

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
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define STRIP_SIZE_MAX 100
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"rgb\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":\"Static Glow\""
    "},"    
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":10,"
      "\"" D_JSON_RATE_MS "\":10000,"
    "},"
    "\"ColourPalette\":\"Custom User 01\","
    "\"BrightnessRGB\":40"
  "}";

#endif

/**
 * User:      Me
 * Location:  Diningroom tree, for older micro leds (GRB) 
 * Networked: Yes
 * */
#ifdef DEVICE_RGBSTRING_DININGROOM_CONTROLLER_02
  #define DEVICENAME_CTR          "xmas_dinigroom_02"
  #define DEVICENAME_FRIENDLY_CTR "RGB Static Controller 02"

  // #define USE_MODULE_SENSORS_BUTTONS
  // #define ENABLE_DEVFEATURE_ANIMATOR_BASIC_BUTTON_CONTROLLER // ie Basic button controls for others, christmas controller
  // #define USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS
  
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  

  #define SETTINGS_HOLDER 1

  //define ENABLE_FORCED_TEMPLATE_LOADING_TO_OVERRIDE_SAVED_SETTINGS  // Previously "FORCE_TEMPLATE_LOADING"

  // #define DISABLE_SETTINGS_STORAGE_OVERRIDE
  // #define ENABLE_XMAS_CONTROLLER_SAVING_IN_EEPROM

  // #define DISABLE_NETWORK
  
  // #define ENABLE_DEVFEATURE_ADVANCED_SETTINGS_SAVE_DEBUG
  // #define ENABLE_DEVFEATURE_SAVE_REBOOT_COMMAND_FOR_SETTINGS_TESTING
  // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
  // #define ENABLE_DEVFEATURE_SETTINGS_SAVED_USED_CORRECTLY_AND_PROGMEM_TEMPLATES_ARE_ONLY_READ_WHEN_SETTINGS_HOLDER_CHANGES // ie works like tas
  // Based on tasmota saving original vs new esp32 enabled
  // #define ENABLE_SETTINGS_VERSION_1_ESP8266_ONLY
  // #define ENABLE_SETTINGS_VERSION_2_ESP32_JOINT_HARDWARE

  /**
   * Three types of animations, exclusive only
   * */
  // #define USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
  // #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

  /**
   * I might hard code the button config into eeprom, instead of fixing the complex saving method until next year.
   * Load in animator, save every 5 seconds
   * */
  #define ENABLE_TEMPFEATURE_BASIC_SAVING_XMAS_CONTROLLER_SETTINGS

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


  // // #define USE_CUSTOM_USER_PAULA
  // #define USE_CUSTOM_USER_JACQUELINE


  // #ifdef USE_CUSTOM_USER_PAULA
  // #define STRIP_SIZE_MAX 100
  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
  //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  //   "\"" D_JSON_RGB_COLOUR_ORDER "\":\"grb\","
  //   "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
  //   "\"" D_JSON_EFFECTS "\":{" 
  //     "\"Function\":\"Static Glow\""
  //   "},"    
  //   "\"" D_JSON_TRANSITION       "\":{"
  //     "\"" D_JSON_TIME_MS "\":1000,"
  //     "\"" D_JSON_RATE_MS "\":2000,"
  //     "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":10,"
  //     "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
  //   "},"
  //   "\"ColourPalette\":\"Custom User 01\","
  //   "\"BrightnessRGB\":100"
  // "}";
  // #endif

  // #ifdef USE_CUSTOM_USER_JACQUELINE
  #define STRIP_SIZE_MAX 100
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"rgb\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":\"Static Glow\""
    "},"    
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":10,"
      "\"" D_JSON_RATE_MS "\":1000,"
    "},"
    "\"ColourPalette\":\"Custom User 01\","
    "\"BrightnessRGB\":40"
  "}";
  // #endif

#endif


/**
 * User:      Me
 * Location:  Hallway tree, 12mm style
 * Networked: Yes
 * */
#ifdef DEVICE_RGBSTRING_HALLWAY_CONTROLLER_01
  #define DEVICENAME_CTR          "xmas_hallway_tree_01"
  #define DEVICENAME_FRIENDLY_CTR "XMAS Hallway Tree 1/1"


//hallway tree could have animation, that, when motion, it goes from warm white to coloured after a few seconds... then after 5 minutes
// will saturate back to warmwhite again


  // Currently being testing on bedroom tree with white 12mm, but will be moved directly to hallway
  // Note, this controller also needs a 5v relay to control the secondary leds(400) that are also on tree

  // #define USE_DEVFEATURE_FUNCTION_UPGRADE_CORE_TRANSITIONMETHODS
  // #define DEBUG_ANIMATIONS_REFRESHPIXELINDEXING
  // #define ENABLE_LOG_LEVEL_DEBUG 

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
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define STRIP_SIZE_MAX 200 // might be pushing memory??
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":\"Static\""
    "},"    
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":2000,"                // 2000
      "\"" D_JSON_RATE_MS "\":5000,"             // 5000
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2" // Not function right now
    "},"
    "\"ColourPalette\":\"Christmas MultiColoured Warmer\","
    "\"BrightnessRGB\":100"
  "}";

#endif


/**
 * User:      Me
 * Location:  Hallway tree, 12mm style
 * Networked: Yes
 * */
#ifdef DEVICE_RGBSTRING_HALLWAY2_CONTROLLER_01
  #define DEVICENAME_CTR          "xmas_hallway2_tree_01"
  #define DEVICENAME_FRIENDLY_CTR "XMAS Hallway2 Tree 1/1"


//hallway tree could have animation, that, when motion, it goes from warm white to coloured after a few seconds... then after 5 minutes
// will saturate back to warmwhite again


  // Currently being testing on bedroom tree with white 12mm, but will be moved directly to hallway
  // Note, this controller also needs a 5v relay to control the secondary leds(400) that are also on tree

  // #define USE_DEVFEATURE_FUNCTION_UPGRADE_CORE_TRANSITIONMETHODS
  // #define DEBUG_ANIMATIONS_REFRESHPIXELINDEXING
  // #define ENABLE_LOG_LEVEL_DEBUG 

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
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define STRIP_SIZE_MAX 150
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":\"Slow Glow\""
    "},"    
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":3900,"                // 2000
      "\"" D_JSON_RATE_MS "\":4000,"             // 5000
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2" // Not function right now
    "},"
    "\"ColourPalette\":\"Christmas MultiColoured Warmer\","
    "\"BrightnessRGB\":50"
  "}";

#endif




/**
 * User:      Me
 * Location:  Hallway tree, 12mm style
 * Networked: Yes
 * */
#ifdef DEVICE_LIVINGROOM_TREE_WATER_SENSOR
  #define DEVICENAME_CTR          "xmas_livingroom_water"
  #define DEVICENAME_FRIENDLY_CTR "XMAS LivingRoom Water Sensor"

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_ADC_INTERNAL

  // new devfeature controller
  #define USE_CONTROLLER_ADC_WATER // needs better name

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

#endif



#endif // MSYSTEMCONFIG_HARDWAREDEFAULTS_H




