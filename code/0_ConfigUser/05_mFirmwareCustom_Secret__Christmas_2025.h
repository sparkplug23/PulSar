#ifndef _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_CHRISTMAS25_H
#define _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_CHRISTMAS25_H

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"

/*** Christmas 2025
 * 
 * Stairs: PIR standby testing
 * Redboard1: Whitehall playlists
 * Redboard2: Meadows redtree playlists
 * Playlist: Serial debugger of playlist
 * Santa Hat: 2D Python/Effect Tester (Put on minifridge)
 * 
 * 
 * Large Front Tree (16X) ==> do with pebbles, both black and green (start with black, then fill in with green later if needed)
 * Side Tree (?, use 8X dual connected until I make X16) ==> Left over whitehall bullet, plus 12v green
 * Icicle lights ==> 4 port redboard
 * 
 * 
 * 
 * 
 * 
 */

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Enable Defines: Whitehall *******************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Enable Defines: Meadows *******************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/
/**
 * Tree at side of house at the fence
 * 8X output with redboard
**/
// #define DEVICE_XMAS25__MEADOWS__SIDE_TREE
// #define DEVICE_XMAS25__MEADOWS__FRONT_TREE

// #define DEVICE_XMAS25__MEADOWS__DESK_PLAYLIST

// #define DEVICE_MEADOWS__BED_ALARM_LIGHT

// #define DEVICE_CHRISTMAS25__FINAL__16X_SNOW_TREE

// #define DEVICE_XMAS25__MEADOWS__HALLWAY_TREE

// #define DEVICE_CHRISTMAS25__FINAL__16X_OUTSIDE_TREE

// #define DEVICE_XMAS25__MEADOWS__PEBBLE_CONE_1D



/** ONLY POWERED/USED DAILY BELOW *************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 **************************************************************************************************************************************************************************************************************************************************************************************************************************** 
 */




#ifdef DEVICE_XMAS25__MEADOWS__SIDE_TREE
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "coxmas24__redboard_01"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"


  // #define ENABLE_DEVFEATURE_LIGHTING__CANSHOW_BACKOFF

 /***********************************
  * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
 ************************************/  
  // #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION

//  /***********************************
//   * SECTION: Sensor Configs
//  ************************************/  

//   #if defined(ENABLE_TEMPLATE_SECTION__SENSORS__MOTION) || defined(ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ)
//    #define USE_MODULE_SENSORS_INTERFACE
//    #define USE_MODULE_SENSORS_PIR
//     //  #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT
//  #endif


  /***********************************
   * SECTION: Lighting Configs
  ************************************/   


  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__BETA
  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE

  // #define ENABLE_DEBUGFEATURE_LIGHTING__SPLASH_FPS
  // #define ENABLE_DEBUGFEATURE_LIGHTING__EFFECT_LOOP_TIME_SERIAL

  // #define ENABLE_BUSCONFG__OUTPUTS_ALL_300
  // #define ENABLE_BUSCONFG__OUTPUTS_MAKE_EACH_BUS_A_SEGMENT // useful for testing
  // #define ENABLE_BUSCONFG__OUTPUTS_INSTALLED_ON_TREE
  #define ENABLE_BUSCONFG__OUTPUTS_INSTALLED_ON_TREE_TAKING_DOWN_EACH_BUS_SEGMENT

  #define ENABLE_DEVFEATURE_LIGHT__GRADIENT_PATCH_4DEC25

  #define ENABLE_DEVFEATURE_LIGHTS__FIX_MULTISEGMENT_DMA_FLICKER
  #define MIN_SHOW_DELAY 25
  


  #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE

  
  #ifdef ENABLE_BUSCONFG__OUTPUTS_ALL_300 
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[      
      {
        "Pin":16,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":300
      },
      {
        "Pin":17,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":300,
        "Length":300
      },
      {
        "Pin":2,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":600,
        "Length":300
      },
      {
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":900,
        "Length":300
      },
      {
        "Pin":5,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1200,
        "Length":300
      },
      {
        "Pin":12,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1500,
        "Length":300
      },
      {
        "Pin":14,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1800,
        "Length":300
      },
      {
        "Pin":15,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2100,
        "Length":300
      }
    ],
    "Segments":[
      {
        "Name":"Bus 1",
        "PixelRange": [
          0,
          2400
        ],
        "ColourPalette":"Snowy 02",
        "ColourType":3,
        "Effects": {
          "Function":"Static",
          "Speed":255,
          "Intensity":127,
          "Grouping":1,
          "RateMs": 1000
        },
        "BrightnessRGB": 100
      }
    ],
    "BrightnessRGB": 100
  }
  )=====";
  #endif
  #ifdef ENABLE_BUSCONFG__OUTPUTS_MAKE_EACH_BUS_A_SEGMENT 
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[      
      {
        "Pin":16,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":300
      },
      {
        "Pin":17,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":300,
        "Length":300
      },
      {
        "Pin":2,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":600,
        "Length":300
      },
      {
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":900,
        "Length":300
      },
      {
        "Pin":5,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1200,
        "Length":300
      },
      {
        "Pin":12,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1500,
        "Length":300
      },
      {
        "Pin":14,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1800,
        "Length":200
      }
    ],
    "Segments":[
      {
        "Name":"Bus 1",
        "PixelRange":"MatchBus",
        "ColourPalette":"Snowy 02",
        "ColourType":3,
        "Effects": {
          "Function":"Static",
          "Speed":255,
          "Intensity":127,
          "Grouping":1,
          "RateMs": 20
        },
        "BrightnessRGB": 100
      }
    ],
    "BrightnessRGB": 100
  }
  )=====";
  #endif
  #ifdef ENABLE_BUSCONFG__OUTPUTS_INSTALLED_ON_TREE 
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[      
      {
        "Pin":16,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":300
      },
      {
        "Pin":17,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":300,
        "Length":300
      },
      {
        "Pin":2,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":600,
        "Length":300
      },
      {
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":900,
        "Length":300
      },
      {
        "Pin":5,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1200,
        "Length":300
      },
      {
        "Pin":12,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1500,
        "Length":300
      },
      {
        "Pin":14,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1800,
        "Length":200
      }
    ],
    "Segments":[
      {
        "Name":"Bus 1",
        "PixelRange": [
          0,
          2000
        ],
        "ColourPalette":"Snowy 02",
        "ColourType":3,
        "Effects": {
          "Function":"Static",
          "Speed":255,
          "Intensity":127,
          "Grouping":1,
          "RateMs": 20
        },
        "BrightnessRGB": 100
      }
    ],
    "BrightnessRGB": 100
  }
  )=====";
  #endif
  

  
  #ifdef ENABLE_BUSCONFG__OUTPUTS_INSTALLED_ON_TREE_TAKING_DOWN_EACH_BUS_SEGMENT
  #define ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[      
      {
        "Pin":16,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":300
      },
      {
        "Pin":17,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":300,
        "Length":300
      },
      {
        "Pin":2,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":600,
        "Length":300
      },
      {
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":900,
        "Length":300
      },
      {
        "Pin":5,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1200,
        "Length":300
      },
      {
        "Pin":12,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1500,
        "Length":300
      },
      {
        "Pin":14,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1800,
        "Length":200
      }
    ],
    "Segments":[
      {
        "Name":"Bus 1",
        "PixelRange":"MatchBus",
        "ColourPalette":"Snowy 02",
        "ColourType":3,
        "Effects": {
          "Function":"Static",
          "Speed":255,
          "Intensity":127,
          "Grouping":1,
          "RateMs": 20
        },
        "BrightnessRGB": 100
      }
    ],
    "BrightnessRGB": 100
  }
  )=====";
  #endif

  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
     "\"15\":\""  D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR "\","
     "\"2\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


#endif // DEVICE_END






#ifdef DEVICE_XMAS25__MEADOWS__FRONT_TREE
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "coxmas24__redboard_01"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"


 /***********************************
  * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
 ************************************/  
  // #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION

//  /***********************************
//   * SECTION: Sensor Configs
//  ************************************/  

//   #if defined(ENABLE_TEMPLATE_SECTION__SENSORS__MOTION) || defined(ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ)
//    #define USE_MODULE_SENSORS_INTERFACE
//    #define USE_MODULE_SENSORS_PIR
//     //  #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT
//  #endif


  /***********************************
   * SECTION: Lighting Configs
  ************************************/   


  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__BETA
  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE

  // #define ENABLE_DEBUGFEATURE_LIGHTING__SPLASH_FPS
  // #define ENABLE_DEBUGFEATURE_LIGHTING__EFFECT_LOOP_TIME_SERIAL

  // #define ENABLE_BUSCONFIG_8X_TEST_OUTPUTS
  // #define ENABLE_BUSCONFIG_16X_1800_200
  // #define ENABLE_BUSCONFIG_16X_TESTING
  // #define ENABLE_BUSCONFIG_16X_BUS_EACH_A_SEGMENT
  // #define ENABLE_BUSCONFIG_16X_TEST_MANUAL_BUS_PINS
  #define ENABLE_BUSCONFG__OUTPUTS_INSTALLED_ON_TREE

  
  #define ENABLE_DEVFEATURE_LIGHT__GRADIENT_PATCH_4DEC25

  #define ENABLE_DEVFEATURE_LIGHTS__FIX_MULTISEGMENT_DMA_FLICKER
  // #define MIN_SHOW_DELAY 25
  

  #define ENABLE_DEVFEATURE_LIGHTS__FIX_MULTISEGMENT_DMA_FLICKER


  #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE

  
  #ifdef ENABLE_BUSCONFIG_8X_TEST_OUTPUTS

  /**
   * @brief tree physical wiring connections
   * 16 outputs
   */
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
        "Length":250
      },
      {
        "Pin":18,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":250,
        "Length":250
      },
      {
        "Pin":19,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":500,
        "Length":250
      },
      {
        "Pin":21,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":750,
        "Length":250
      },
      {
        "Pin":16,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1000,
        "Length":250
      },
      {
        "Pin":17,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1250,
        "Length":250
      },
      {
        "Pin":22,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1500,
        "Length":250
      },
      {
        "Pin":23,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1750,
        "Length":250
      },
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2000,
        "Length":250
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        2250
      ],
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Static",
        "Speed":127,
        "Intensity":127,
        "Grouping":1,
        "RateMs": 25
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 0,
    "BrightnessCCT": 0
  }
  )=====";

  
      
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      // "\"28\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\"," // Bus8
      // "\"13\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"12\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"26\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"32\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"14\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"27\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"25\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"33\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  #endif
  #ifdef ENABLE_BUSCONFIG_16X_1800_200

  /**
   * @brief tree physical wiring connections
   * 16 outputs
   */
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
        "Length":250
      },
      {
        "Pin":18,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":250,
        "Length":250
      },
      {
        "Pin":19,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":500,
        "Length":250
      },
      {
        "Pin":21,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":750,
        "Length":250
      },
      {
        "Pin":16,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1000,
        "Length":250
      },
      {
        "Pin":17,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1250,
        "Length":250
      },
      {
        "Pin":22,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1500,
        "Length":250
      },
      {
        "Pin":23,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1750,
        "Length":250
      },
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2000,
        "Length":250
      },
      {
        "Pin":12,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2250,
        "Length":250
      },
      {
        "Pin":26,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2500,
        "Length":250
      },
      {
        "Pin":32,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2750,
        "Length":250
      },
      {
        "Pin":14,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":3000,
        "Length":250
      },
      {
        "Pin":27,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":3250,
        "Length":250
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        3500
      ],
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Static",
        "Speed":255,
        "Intensity":127,
        "Grouping":1,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";

 
  //     {
  //       "Pin":25,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":3500,
  //       "Length":250
  //     },
  //     {
  //       "Pin":33,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":3750,
  //       "Length":250
  //     }
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      // "\"28\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\"," // Bus8
      // "\"13\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"12\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"26\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"32\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"14\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"27\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"25\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"33\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  #endif


  #ifdef ENABLE_BUSCONFIG_16X_TESTING

  /**
   * @brief tree physical wiring connections
   * 16 outputs
   */
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
        "Length":250
      },
      {
        "Pin":18,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":250,
        "Length":250
      },
      {
        "Pin":19,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":500,
        "Length":250
      },
      {
        "Pin":21,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":750,
        "Length":250
      },
      {
        "Pin":16,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1000,
        "Length":250
      },
      {
        "Pin":17,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1250,
        "Length":250
      },
      {
        "Pin":22,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1500,
        "Length":250
      },
      {
        "Pin":23,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1750,
        "Length":250
      },
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2000,
        "Length":250
      },
      {
        "Pin":12,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2250,
        "Length":250
      },
      {
        "Pin":26,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2500,
        "Length":250
      },
      {
        "Pin":32,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2750,
        "Length":250
      },
      {
        "Pin":14,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":3000,
        "Length":250
      },
      {
        "Pin":27,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":3250,
        "Length":250
      },
      {
        "n":"16",
        "Pin":25,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":3500,
        "Length":250
      },
      {
        "n":"16",
        "Pin":33,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":3750,
        "Length":250
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        4000
      ],
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Static",
        "Speed":255,
        "Intensity":127,
        "Grouping":1,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";

  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      // "\"28\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\"," // Bus8
      // "\"13\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"12\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"26\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"32\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"14\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"27\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"25\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"33\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  #endif

  

  #ifdef ENABLE_BUSCONFG__OUTPUTS_INSTALLED_ON_TREE

  #define ENABLE_DEVFEATURE_LIGHTS__SEGMENT_MATCHBUS
  #define BUSCONFIG_MAX_PINS_FOR_PARALLEL_I2S 1000
  #define MAX_LED_MEMORY 64000*5
  
  #define MAX_NUM_SEGMENTS 16

  /**
   * @brief tree physical wiring connections
   * 16 outputs
   * 
   * C1L1 
   * 
   * 13g,12g,14g,27g,26g,25g,33g,32g
   * 4g,16g,17g,-,1,18g,19g,21g,-,-,22g,23g
   * 
   * 17T
   * 16R
   * 
   * 
   * Grouped by shifter clusters
   * 1  3         9   11
   * 2  4         10  12
   * -  -  ESP32  -    -
   * 5  7         13  15
   * 6  8         14  16
   * 
   * Grouped by shifter clusters (PINS)
   * 4   16R      13  14
   * 18  17T      12  27
   * -  -  ESP32  -    -
   * 19  22       26  25*
   * 21  23       32  33*
   * * are not being used on this tree
   */

  /**
   * @brief tree physical wiring connections
   * 16 outputs
   */
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "n":"L1",
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":250
      },
      {
        "n":"L1",
        "Pin":18,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":250,
        "Length":250
      },
      {
        "n":"L1",
        "Pin":16,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":500,
        "Length":250
      },
      {
        "n":"L1",
        "Pin":17,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":750,
        "Length":250
      },
      {
        "n":"L1",
        "Pin":19,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1000,
        "Length":200
      },
      {
        "n":"L1",
        "Pin":21,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1200,
        "Length":200
      },
      {
        "n":"L1",
        "Pin":22,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1400,
        "Length":200
      },
      {
        "n":"L1",
        "Pin":23,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1600,
        "Length":200
      },
      {
        "n":"L1",
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1800,
        "Length":200
      },
      {
        "n":"L1",
        "Pin":12,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2000,
        "Length":200
      },
      {
        "n":"L1",
        "Pin":14,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2200,
        "Length":200
      },
      {
        "n":"L1",
        "Pin":27,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2400,
        "Length":200
      },
      {
        "n":"L1",
        "Pin":26,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2600,
        "Length":200
      },
      {
        "n":"L1",
        "Pin":32,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2800,
        "Length":200
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        3000
      ],
      "ColourPalette":"Snowy 02",
      "Palette2":"Cold White",
      "Effects": {
        "Function":"Static",
        "Intensity":1,
        "Speed":255,
        "Grouping":1,
        "Decimate":4,
        "Custom1":255,
        "Custom2":220,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";

  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      // "\"28\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\"," // Bus8
      // "\"13\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"12\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"26\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"32\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"14\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"27\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"25\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"33\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  #endif

  #ifdef ENABLE_BUSCONFIG_16X_BUS_EACH_A_SEGMENT

  #define ENABLE_DEVFEATURE_LIGHTS__SEGMENT_MATCHBUS
  #define BUSCONFIG_MAX_PINS_FOR_PARALLEL_I2S 1000
  #define MAX_LED_MEMORY 64000*5
  /**
   * @brief tree physical wiring connections
   * 16 outputs
   * 
   * C1L1 
   * 
   * 13g,12g,14g,27g,26g,25g,33g,32g
   * 4g,16g,17g,-,1,18g,19g,21g,-,-,22g,23g
   * 
   * 17T
   * 16R
   * 
   * 
   * Grouped by shifter clusters
   * 1  3         9   11
   * 2  4         10  12
   * -  -  ESP32  -    -
   * 5  7         13  15
   * 6  8         14  16
   * 
   * Grouped by shifter clusters (PINS)
   * 4   16R      13  14
   * 18  17T      12  27
   * -  -  ESP32  -    -
   * 19  22       26  25
   * 21  23       32  33
   * 
   */
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "n":"L1",
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":250
      },
      {
        "n":"L1",
        "Pin":18,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":250,
        "Length":250
      },
      {
        "n":"L1",
        "Pin":16,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":500,
        "Length":250
      },
      {
        "n":"L1",
        "Pin":17,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":750,
        "Length":250
      },
      {
        "n":"L1",
        "Pin":19,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1000,
        "Length":200
      },
      {
        "n":"L1",
        "Pin":21,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1200,
        "Length":200
      },
      {
        "n":"L1",
        "Pin":22,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1400,
        "Length":200
      },
      {
        "n":"L1",
        "Pin":23,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1600,
        "Length":200
      },
      {
        "n":"L1",
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1800,
        "Length":200
      },
      {
        "n":"L1",
        "Pin":12,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2000,
        "Length":200
      },
      {
        "n":"L1",
        "Pin":14,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2200,
        "Length":200
      },
      {
        "n":"L1",
        "Pin":27,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2400,
        "Length":200
      },
      {
        "n":"L1",
        "Pin":26,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2600,
        "Length":200
      },
      {
        "n":"L1",
        "Pin":32,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2800,
        "Length":200
      },
      {
        "n":"L1",
        "Pin":25,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":3500,
        "Length":250
      },
      {
        "n":"L1",
        "Pin":33,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":3750,
        "Length":250
      }
    ],
    "Segments":[
      {
        "PixelRange":"MatchBus",
        "ColourPalette":"RGPBY",
        "Effects": {
          "Function":"Static",
          "Speed":255,
          "Intensity":127,
          "Grouping":50,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      }      
    ],
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";

  //27 not working, or bottom right R3R4

  #define MAX_NUM_SEGMENTS 16

  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      // "\"28\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\"," // Bus8
      // "\"13\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
          // "\"12\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
          // "\"26\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
          // "\"32\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
          // "\"14\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
          // "\"27\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
          "\"25\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
          "\"33\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  #endif


  
  

  #ifdef ENABLE_BUSCONFIG_16X_TEST_MANUAL_BUS_PINS

  #define ENABLE_DEVFEATURE_LIGHTS__SEGMENT_MATCHBUS
  #define BUSCONFIG_MAX_PINS_FOR_PARALLEL_I2S 1000
  #define MAX_LED_MEMORY 64000*5
  /**
   * @brief tree physical wiring connections
   * 16 outputs
   * 
   * C1L1 
   * 
   * UNK: 
   * Good: 27,4, 18, 19, 21, 16, 
   * 
   * 
   * 
   */
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":32,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":250
      }
    ],
    "Segments":[
      {
        "PixelRange":"MatchBus",
        "ColourPalette":"RGPBO",
        "Effects": {
          "Function":"Static",
          "Speed":255,
          "Intensity":127,
          "Grouping":50,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      }      
    ],
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";

  //27 not working, or bottom right R3R4

  #define MAX_NUM_SEGMENTS 16

  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      // "\"28\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\"," // Bus8
      // "\"13\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
          // "\"12\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
          // "\"26\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
          // "\"32\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
          // "\"14\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
          // "\"27\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
          "\"25\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
          "\"33\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  #endif

#endif // DEVICE_END



#ifdef DEVICE_XMAS25__MEADOWS__ICICLES
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "coxmas24__redboard_01"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"


 /***********************************
  * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
 ************************************/  
  // #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION

//  /***********************************
//   * SECTION: Sensor Configs
//  ************************************/  

//   #if defined(ENABLE_TEMPLATE_SECTION__SENSORS__MOTION) || defined(ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ)
//    #define USE_MODULE_SENSORS_INTERFACE
//    #define USE_MODULE_SENSORS_PIR
//     //  #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT
//  #endif


  /***********************************
   * SECTION: Lighting Configs
  ************************************/   


  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__BETA
  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE

  // #define ENABLE_DEBUGFEATURE_LIGHTING__SPLASH_FPS
  // #define ENABLE_DEBUGFEATURE_LIGHTING__EFFECT_LOOP_TIME_SERIAL

  #define ENABLE_BUSCONFIG_4X_TEST_OUTPUTS
  // #define ENABLE_BUSCONFG__OUTPUTS_INSTALLED_ON_TREE

  
  #define ENABLE_DEVFEATURE_LIGHT__GRADIENT_PATCH_4DEC25

  #define ENABLE_DEVFEATURE_LIGHTS__FIX_MULTISEGMENT_DMA_FLICKER
  #define MIN_SHOW_DELAY 25
  

  #define ENABLE_DEVFEATURE_LIGHTS__FIX_MULTISEGMENT_DMA_FLICKER


  #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE

  
  #ifdef ENABLE_BUSCONFIG_4X_TEST_OUTPUTS

  /**
   * @brief tree physical wiring connections
   * 16 outputs
   */
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":2,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":200
      },
      {
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":200,
        "Length":200
      },
      {
        "Pin":18,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":400,
        "Length":200
      },
      {
        "Pin":19,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":600,
        "Length":200
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        800
      ],
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Static",
        "Speed":127,
        "Intensity":127,
        "Grouping":1,
        "RateMs": 25
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";

  
      
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      // "\"28\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\"," // Bus8
      // "\"13\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"12\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"26\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"32\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"14\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"27\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"25\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"33\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  #endif
  #ifdef ENABLE_BUSCONFIG_16X_1800_200

  /**
   * @brief tree physical wiring connections
   * 16 outputs
   */
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
        "Length":200
      },
      {
        "Pin":18,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":200,
        "Length":200
      },
      {
        "Pin":19,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":400,
        "Length":200
      },
      {
        "Pin":21,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":600,
        "Length":200
      },
      {
        "Pin":16,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":800,
        "Length":200
      },
      {
        "Pin":17,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1000,
        "Length":200
      },
      {
        "Pin":22,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1200,
        "Length":200
      },
      {
        "Pin":23,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1400,
        "Length":200
      },
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1600,
        "Length":200
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        1800
      ],
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Static",
        "Speed":127,
        "Intensity":127,
        "Grouping":1,
        "RateMs": 25
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 0,
    "BrightnessCCT": 0
  }
  )=====";

  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      // "\"28\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\"," // Bus8
      // "\"13\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"12\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"26\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"32\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"14\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"27\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"25\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"33\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  #endif


#endif // DEVICE_END


#ifdef DEVICE_XMAS25__MEADOWS__HALLWAY_TREE
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "coxmas24__redboard_01"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"


 /***********************************
  * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
 ************************************/  
  #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION

//  /***********************************
//   * SECTION: Sensor Configs
//  ************************************/  

//   #if defined(ENABLE_TEMPLATE_SECTION__SENSORS__MOTION) || defined(ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ)
//    #define USE_MODULE_SENSORS_INTERFACE
//    #define USE_MODULE_SENSORS_PIR
//     //  #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT
//  #endif


  /***********************************
   * SECTION: Lighting Configs
  ************************************/   


  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__BETA
  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE

  // #define ENABLE_DEBUGFEATURE_LIGHTING__SPLASH_FPS
  // #define ENABLE_DEBUGFEATURE_LIGHTING__EFFECT_LOOP_TIME_SERIAL

  // #define ENABLE_BUSCONFG__NEW_BUSCONFIG_RANGING
  // #define ENABLE_BUSCONFG__OUTPUTS_INSTALLED_ON_TREE
  #define ENABLE_BUSCONFG__OUTPUTS_INSTALLED_ON_TREE_ONE_SEGMENT


  
  #ifdef ENABLE_BUSCONFG__NEW_BUSCONFIG_RANGING 
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[     
      {
        "Name":"Star",
        "Pin":2,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":201
      },
      {
        "Name":"Top",
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":201,
        "Length":250
      },
      {
        "Pin":18,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":451,
        "Length":125
      },
      {
        "Pin":19,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":576,
        "Length":3
      }
    ],
    "Segments":[
      {
        "PixelRange":"MatchBus",
        "ColourPalette":"RGPBY",
        "ColourType":3,
        "Effects": {
          "Function":"Static",
          "Speed":255,
          "Intensity":127,
          "Grouping":1,
          "RateMs": 20
        },
        "BrightnessRGB": 100
      }
    ],
    "BrightnessRGB": 100
  }
  )=====";
  #define BUSCONFIG_MAX_PINS_FOR_PARALLEL_I2S 1000
  #define MAX_LED_MEMORY 64000*5
  #define ENABLE_DEVFEATURE_LIGHTS__SEGMENT_MATCHBUS
  #endif
  #ifdef ENABLE_BUSCONFG__OUTPUTS_INSTALLED_ON_TREE 
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[     
      {
        "Name":"Star",
        "Pin":2,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":201
      },
      {
        "Name":"Top",
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":201,
        "Length":250
      },
      {
        "Pin":18,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":451,
        "Length":125
      }
    ],
    "Segments":[
      {
        "Name":"Star",
        "PixelRange": [
          0,
          201
        ],
        "ColourPalette":"Colour 01",
        "SegColour0": {
          "Hue": 25,
          "Sat":94,
          "BrightnessRGB":100
        },
        "ColourType":3,
        "Effects": {
          "Function":"Candles",
          "Speed":255,
          "Intensity":127,
          "Grouping":1,
          "RateMs": 20
        },
        "BrightnessRGB": 50
      },
      {
        "Name":"Tree",
        "PixelRange": [
          202,
          576
        ],
        "ColourPalette":"Snowy 02",
        "ColourType":3,
        "Effects": {
          "Function":"Static",
          "Speed":255,
          "Intensity":127,
          "Grouping":1,
          "RateMs": 20
        },
        "BrightnessRGB": 100
      }
    ],
    "BrightnessRGB": 100
  }
  )=====";
  // #define BUSCONFIG_MAX_PINS_FOR_PARALLEL_I2S 1000
  // #define MAX_LED_MEMORY 64000*5
  // #define ENABLE_DEVFEATURE_LIGHTS__SEGMENT_MATCHBUS
  #define ENABLE_DEVFEATURE_LIGHTS__FIX_MULTISEGMENT_DMA_FLICKER
  // #define ENABLE_DEVFETURE_LIGHTING__SEGMENT_INDEX_AS_ITER
  #endif
  #ifdef ENABLE_BUSCONFG__OUTPUTS_INSTALLED_ON_TREE_ONE_SEGMENT
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[     
      {
        "Name":"Star",
        "Pin":2,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":201
      },
      {
        "Name":"Top",
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":201,
        "Length":250
      },
      {
        "Pin":18,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":451,
        "Length":125
      }
    ],
    "Segments":[
      {
        "Name":"Bus 1",
        "PixelRange": [
          0,
          576
        ],
        "ColourPalette":"Snowy 02",
        "ColourType":3,
        "Effects": {
          "Function":"Static",
          "Speed":255,
          "Intensity":127,
          "Grouping":1,
          "RateMs": 20
        },
        "BrightnessRGB": 75
      }
    ],
    "BrightnessRGB": 100
  }
  )=====";
  #define BUSCONFIG_MAX_PINS_FOR_PARALLEL_I2S 1000
  #define MAX_LED_MEMORY 64000*5
  #define ENABLE_DEVFEATURE_LIGHTS__SEGMENT_MATCHBUS
  #endif
  
  

  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      // "\"13\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
     "\"15\":\""  D_GPIO_FUNCTION_PIR_1_CTR "\","
     "\"2\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


#endif // DEVICE_END



#ifdef DEVICE_XMAS25__MEADOWS__PEBBLE_CONE_1D
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "coxmas24__redboard_01"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"

  #define ENABLE_FEATURE_LIGHTING__REALTIME_MQTT_SETPIXEL

 /***********************************
  * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
 ************************************/  
  // #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION

//  /***********************************
//   * SECTION: Sensor Configs
//  ************************************/  

//   #if defined(ENABLE_TEMPLATE_SECTION__SENSORS__MOTION) || defined(ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ)
//    #define USE_MODULE_SENSORS_INTERFACE
//    #define USE_MODULE_SENSORS_PIR
//     //  #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT
//  #endif


  /***********************************
   * SECTION: Lighting Configs
  ************************************/   


  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__BETA
  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE

  // #define ENABLE_DEBUGFEATURE_LIGHTING__SPLASH_FPS
  // #define ENABLE_DEBUGFEATURE_LIGHTING__EFFECT_LOOP_TIME_SERIAL

  // #define ENABLE_BUSCONFG__NEW_BUSCONFIG_RANGING
  #define ENABLE_BUSCONFG__OUTPUTS_INSTALLED_ON_TREE
  // #define ENABLE_BUSCONFG__OUTPUTS_INSTALLED_ON_TREE_ONE_SEGMENT


  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[     
      {
        "Name":"Star",
        "Pin":2,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":1010
      }
    ],
    "Segments":[
      {
        "PixelRange":[0,1010],
        "ColourPalette":"RGPBY",
        "ColourType":3,
        "Effects": {
          "Function":"Static",
          "Speed":255,
          "Intensity":127,
          "Grouping":1,
          "RateMs": 20
        },
        "BrightnessRGB": 100
      }
    ],
    "BrightnessRGB": 100
  }
  )=====";
  #define BUSCONFIG_MAX_PINS_FOR_PARALLEL_I2S 1000
  #define MAX_LED_MEMORY 64000*5
  #define ENABLE_DEVFEATURE_LIGHTS__SEGMENT_MATCHBUS

  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      // "\"13\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
     "\"15\":\""  D_GPIO_FUNCTION_PIR_1_CTR "\","
     "\"2\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


#endif // DEVICE_END

/**
 * @brief 
 * {
  "BrightnessRGB": 100,
  "ColourPalette":"RGPBY",
  "Effects":{"Grouping":1},
   "MQTTPixel": {
    "OnPixelsWithIndex": [
      [0, 0, 65],
      [1, 66, 131],
      [4, 132, 134],
      [0, 135, 199],
      [1, 200, 264],
      [4, 265, 269],
      [0, 270, 335],
      [1, 336, 401],
      [4, 402, 406],
      [0, 407, 471],
      [1, 472, 537],
      [4, 538, 541],
      [0, 542, 607],
      [1, 608, 673],
      [4, 674, 677],
      [0, 678, 743],
      [1, 744, 809],
      [4, 810, 814],
      [0, 815, 880],
      [1, 881, 946],
      [4, 947, 949],
      [0, 950, 1000]
    ],
    "OnPixelsFill": false
  }
 }
 * 
 */
#ifdef DEVICE_XMAS25__MEADOWS__PEBBLE_CONE_2D
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "coxmas24__redboard_01"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"

  #define ENABLE_FEATURE_LIGHTING__REALTIME_MQTT_SETPIXEL


 /***********************************
  * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
 ************************************/  
  #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION

//  /***********************************
//   * SECTION: Sensor Configs
//  ************************************/  

//   #if defined(ENABLE_TEMPLATE_SECTION__SENSORS__MOTION) || defined(ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ)
//    #define USE_MODULE_SENSORS_INTERFACE
//    #define USE_MODULE_SENSORS_PIR
//     //  #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT
//  #endif


  /***********************************
   * SECTION: Lighting Configs
  ************************************/   


  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__BETA
  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE

  // #define ENABLE_DEBUGFEATURE_LIGHTING__SPLASH_FPS
  // #define ENABLE_DEBUGFEATURE_LIGHTING__EFFECT_LOOP_TIME_SERIAL

  // #define ENABLE_BUSCONFG__NEW_BUSCONFIG_RANGING
  #define ENABLE_BUSCONFG__OUTPUTS_INSTALLED_ON_TREE
  // #define ENABLE_BUSCONFG__OUTPUTS_INSTALLED_ON_TREE_ONE_SEGMENT


  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[     
      {
        "Name":"Star",
        "Pin":2,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":1000
      }
    ],
    "MatrixConfig":[
      {
        "Width":15,
        "Height":66,
        "BottomStart":1,
        "RightStart":0,
        "Vertical":1,
        "Serpentine":1,
        "xOffset":0,
        "yOffset":0
      }
    ],
    "Segments":[
      {
        "PixelRange":[0,15,0,66],
        "ColourPalette":"RGPBY",
        "ColourType":3,
        "Effects": {
          "Function":"Static",
          "Speed":255,
          "Intensity":127,
          "Grouping":10,
          "RateMs": 20
        },
        "BrightnessRGB": 100
      }
    ],
    "BrightnessRGB": 5
  }
  )=====";
  #define BUSCONFIG_MAX_PINS_FOR_PARALLEL_I2S 1000
  #define MAX_LED_MEMORY 64000*5
  #define ENABLE_DEVFEATURE_LIGHTS__SEGMENT_MATCHBUS

  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      // "\"13\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
     "\"15\":\""  D_GPIO_FUNCTION_PIR_1_CTR "\","
     "\"2\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


#endif // DEVICE_END


#ifdef DEVICE_XMAS25__MEADOWS__FRONTDOOR_WREATH
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "xmas25__final__frontdoor_wreath"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"


 /***********************************
  * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
 ************************************/  
  #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION

//  /***********************************
//   * SECTION: Sensor Configs
//  ************************************/  

//   #if defined(ENABLE_TEMPLATE_SECTION__SENSORS__MOTION) || defined(ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ)
//    #define USE_MODULE_SENSORS_INTERFACE
//    #define USE_MODULE_SENSORS_PIR
//     //  #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT
//  #endif


  /***********************************
   * SECTION: Lighting Configs
  ************************************/   


  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__BETA
  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE

  // #define ENABLE_DEBUGFEATURE_LIGHTING__SPLASH_FPS
  // #define ENABLE_DEBUGFEATURE_LIGHTING__EFFECT_LOOP_TIME_SERIAL


  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // R"=====(
  // {
  //   "BusConfig":[
  //     {
  //       "Pin":2,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":0,
  //       "Length":100
  //     },
  //     {
  //       "Pin":4,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":100,
  //       "Length":100
  //     },
  //     {
  //       "Pin":18,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":200,
  //       "Length":100
  //     },
  //     {
  //       "Pin":19,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":300,
  //       "Length":1
  //     }
  //   ],
  //   "Segment0": {
  //     "PixelRange": [
  //       0,
  //       266
  //     ],
  //     "ColourPalette":"Snowy 02",
  //     "Effects": {
  //       "Function":"Candles",
  //       "Speed":26,
  //       "Intensity":255,
  //       "Decimate":0,
  //       "Grouping":1,
  //       "RateMs": 25
  //     },
  //     "BrightnessRGB": 100
  //   },
  //   "Segment1": {
  //     "PixelRange": [
  //       267,
  //       300
  //     ],
  //     "ColourPalette":"Snowy 02",
  //     "SegColour0": {
  //       "Hue": 0,
  //       "Sat":100,
  //       "BrightnessRGB":5
  //     },
  //     "Effects": {
  //       "Function":"Solid",
  //       "Speed":255,
  //       "Decimate":0,
  //       "Grouping":1,
  //       "RateMs": 1000
  //     },
  //     "BrightnessRGB": 0,
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
        "Pin":2,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      },
      {
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":100,
        "Length":100
      },
      {
        "Pin":18,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":200,
        "Length":100
      },
      {
        "Pin":19,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":300,
        "Length":1
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        266
      ],
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Candles",
        "Speed":26,
        "Intensity":255,
        "Decimate":0,
        "Grouping":1,
        "RateMs": 25
      },
      "BrightnessRGB": 100
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
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      // "\"13\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
     "\"15\":\""  D_GPIO_FUNCTION_PIR_1_CTR "\","
     "\"2\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


#endif // DEVICE_END


#ifdef DEVICE_CHRISTMAS25__FINAL__16X_SNOW_TREE
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "xmas24__final__16x_snow_tree"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define MQTT_HOST   "192.168.1.70"
    #define MQTT_PORT     1883

  /***********************************
   * SECTION: System Debug Options
  ************************************/    

  // #define ENABLE_DEBUGFEATURE_WEBUI__SHOW_BUILD_DATETIME_IN_FOOTER

  // #define ENABLE_DEBUGFEATURE__16PIN_PARALLEL_OUTPUT

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS
  // #define ENABLE_DEBUG_LINE_HERE3
  

  /***********************************
   * SECTION: System Configs
  ************************************/    
   
    
  /***********************************
   * SECTION: Network Configs
  ************************************/    

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  
 
  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__BETA
  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE

  


  #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE


  // 4, 16, 17, 18, 19, 21, 22, 23, 2, 13, 14, 27, 26, 25, 33, 32
  /**
   * @brief 2024 Snow Tree physical wiring connections
   * Lights start from the base, and are put on the tree "clockwise" when looking down from the top, so "to the left" when looking headon
   * 
   * Tree has 13 levels, plus solid section on top
   * 
   * [CON]
   * [R6] Top section: 200 leds + power injection
   * ---------------------------- Tree Break
   * [R5] Level 13: 100 LEDS (RGB*)
   * [R4] Level 12: 100 LEDS
   * ---------------------------- Tree Break
   * [R3] Level 11: 200 LEDS  ** This is fed by power injection between two 100 sets
   * [R2] Level 10: 100 LEDS
   * [R1] Level  9: 100 LEDS
   * [L8] Level  8: 100 LEDS
   * ---------------------------- Tree Break
   * [L7] Level  7: 100 LEDS
   * [L6] Level  6: 100 LEDS
   * [L5] Level  5: 100 LEDS
   * [L4] Level  4: 100 LEDS
   * ---------------------------- Tree Break
   * [L4 + L5 ] Level  3: 100 LEDS + 100 LEDS
   * [L2 + L3 ] Level  2: 100 LEDS + 100 LEDS
   * [L1      ] Level  1: 200 LEDS + power injection
    
   
   {
      "Effects": {
        "Function":"Debug Pixel Range",
        "Param0": 1400,
        "Param1": 1700
      }
    }

   */
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":2,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":200
      },
      {
        "Pin":16,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":200,
        "Length":100
      },
      {
        "Pin":17,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":300,
        "Length":100
      },
      {
        "Pin":18,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":400,
        "Length":100
      },
      {
        "Pin":19,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":500,
        "Length":100
      },
      {
        "Pin":21,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":600,
        "Length":100
      },
      {
        "Pin":22,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":700,
        "Length":100
      },
      {
        "Pin":23,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":800,
        "Length":100
      },
      {
        "Pin":13,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":900,
        "Length":100
      },
      {
        "Pin":14,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":1000,
        "Length":100
      },
      {
        "Pin":27,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":1100,
        "Length":100
      },
      {
        "Pin":26,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":1200,
        "Length":100
      },
      {
        "Pin":25,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":1300,
        "Length":200
      },
      {
        "Pin":33,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":1500,
        "Length":100
      },
      {
        "Pin":32,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1600,
        "Length":100
      },
      {
        "Pin":4,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB", 
        "Start":1700,
        "Length":200
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        1900
      ],
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Static",
        "Speed":127,
        "Intensity":127,
        "Grouping":1,
        "RateMs": 25
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 60,
    "BrightnessCCT": 0
  }
  )=====";

  
  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

#endif // DEVICE END



#ifdef DEVICE_CHRISTMAS25__FINAL__16X_OUTSIDE_TREE
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "xmas24__final__16x_outside_tree"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define MQTT_HOST   "192.168.1.70"
    #define MQTT_PORT     1883

  /***********************************
   * SECTION: System Debug Options
  ************************************/    

  // #define ENABLE_DEBUGFEATURE_WEBUI__SHOW_BUILD_DATETIME_IN_FOOTER

  // #define ENABLE_DEBUGFEATURE__16PIN_PARALLEL_OUTPUT

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS
  // #define ENABLE_DEBUG_LINE_HERE3
  

  /***********************************
   * SECTION: System Configs
  ************************************/    
   
    
  /***********************************
   * SECTION: Network Configs
  ************************************/    

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  
 
  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__BETA
  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE


  #define ENABLE_DEVFEATURE_LIGHT__GRADIENT_PATCH_4DEC25

  #define ENABLE_DEVFEATURE_LIGHTS__FIX_MULTISEGMENT_DMA_FLICKER
  #define MIN_SHOW_DELAY 25
  


  #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE


  #define ENABLE_BUSCONFIG_7X_2100_ON_AT_BOOT                     // NORMAL 2025 Operation
  // #define ENABLE_BUSCONFIG_7X_2100_FORCED_OFF_ON_BOOT  // Failsafe when powerloss of sonoff
  


  #ifdef ENABLE_BUSCONFIG_7X_2100_ON_AT_BOOT

  
  

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
        "Length":300
      },
      {
        "Pin":18,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":300,
        "Length":300
      },
      {
        "Pin":19,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":600,
        "Length":300
      },
      {
        "Pin":21,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":900,
        "Length":300
      },
      {
        "Pin":16,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1200,
        "Length":300
      },
      {
        "Pin":17,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1500,
        "Length":300
      },
      {
        "Pin":22,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1800,
        "Length":300
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        2100
      ],
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Static",
        "Speed":127,
        "Intensity":127,
        "Grouping":1,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "Preset":{"Load":208}
    },
    "BrightnessRGB": 100
  }
  )=====";

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      "\"13\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"12\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"26\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"32\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"14\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"27\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"25\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"33\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"28\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\"," // Bus8
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #endif // ENABLE_BUSCONFIG_7X_2100

  
  #ifdef ENABLE_BUSCONFIG_7X_2100_FORCED_OFF_ON_BOOT

  

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
        "Length":300
      },
      {
        "Pin":18,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":300,
        "Length":300
      },
      {
        "Pin":19,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":600,
        "Length":300
      },
      {
        "Pin":21,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":900,
        "Length":300
      },
      {
        "Pin":16,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1200,
        "Length":300
      },
      {
        "Pin":17,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1500,
        "Length":300
      },
      {
        "Pin":22,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1800,
        "Length":300
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        2100
      ],
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Static",
        "Speed":127,
        "Intensity":127,
        "Grouping":1,
        "RateMs": 1000
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 0
  }
  )=====";

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      "\"13\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"12\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"26\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"32\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"14\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"27\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"25\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"33\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"28\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\"," // Bus8
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #endif // ENABLE_BUSCONFIG_7X_2100


  #ifdef ENABLE_BUSCONFIG_8X_2400

  

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
        "Length":300
      },
      {
        "Pin":18,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":300,
        "Length":300
      },
      {
        "Pin":19,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":600,
        "Length":300
      },
      {
        "Pin":21,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":900,
        "Length":300
      },
      {
        "Pin":16,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1200,
        "Length":300
      },
      {
        "Pin":17,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1500,
        "Length":300
      },
      {
        "Pin":22,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1800,
        "Length":300
      },
      {
        "Pin":28,
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
      "ColourPalette":"RGBY",
      "Effects": {
        "Function":"Static",
        "Speed":127,
        "Intensity":127,
        "Grouping":100,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 0,
    "BrightnessCCT": 0
  }
  )=====";

  #define ENABLE_DEVFEATURE_LIGHTS__FIX_MULTISEGMENT_DMA_FLICKER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      "\"13\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"12\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"26\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"32\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"14\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"27\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"25\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"33\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"28\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\"," // Bus8
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #endif // ENABLE_BUSCONFIG_8X_2400


  // // 4, 16, 17, 18, 19, 21, 22, 23, 2, 13, 14, 27, 26, 25, 33, 32
  // /**
  //  * @brief 2024 Snow Tree physical wiring connections
  //  * Lights start from the base, and are put on the tree "clockwise" when looking down from the top, so "to the left" when looking headon
  //  * 
  //  * Tree has 13 levels, plus solid section on top
  //  * 
  //  * [CON]
  //  * [R6] Top section: 200 leds + power injection
  //  * ---------------------------- Tree Break
  //  * [R5] Level 13: 100 LEDS (RGB*)
  //  * [R4] Level 12: 100 LEDS
  //  * ---------------------------- Tree Break
  //  * [R3] Level 11: 200 LEDS  ** This is fed by power injection between two 100 sets
  //  * [R2] Level 10: 100 LEDS
  //  * [R1] Level  9: 100 LEDS
  //  * [L8] Level  8: 100 LEDS
  //  * ---------------------------- Tree Break
  //  * [L7] Level  7: 100 LEDS
  //  * [L6] Level  6: 100 LEDS
  //  * [L5] Level  5: 100 LEDS
  //  * [L4] Level  4: 100 LEDS
  //  * ---------------------------- Tree Break
  //  * [L4 + L5 ] Level  3: 100 LEDS + 100 LEDS
  //  * [L2 + L3 ] Level  2: 100 LEDS + 100 LEDS
  //  * [L1      ] Level  1: 200 LEDS + power injection
    
   
  //  {
  //     "Effects": {
  //       "Function":"Debug Pixel Range",
  //       "Param0": 1400,
  //       "Param1": 1700
  //     }
  //   }

  //  */
  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // R"=====(
  // {
  //   "BusConfig":[
  //     {
  //       "Pin":2,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":0,
  //       "Length":200
  //     },
  //     {
  //       "Pin":16,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":200,
  //       "Length":100
  //     },
  //     {
  //       "Pin":17,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":300,
  //       "Length":100
  //     },
  //     {
  //       "Pin":18,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":400,
  //       "Length":100
  //     },
  //     {
  //       "Pin":19,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":500,
  //       "Length":100
  //     },
  //     {
  //       "Pin":21,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":600,
  //       "Length":100
  //     },
  //     {
  //       "Pin":22,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":700,
  //       "Length":100
  //     },
  //     {
  //       "Pin":23,
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
  //       "Start":1200,
  //       "Length":100
  //     },
  //     {
  //       "Pin":25,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":1300,
  //       "Length":200
  //     },
  //     {
  //       "Pin":33,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":1500,
  //       "Length":100
  //     },
  //     {
  //       "Pin":32,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":1600,
  //       "Length":100
  //     },
  //     {
  //       "Pin":4,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB", 
  //       "Start":1700,
  //       "Length":200
  //     }
  //   ],
  //   "Segment0": {
  //     "PixelRange": [
  //       0,
  //       1900
  //     ],
  //     "ColourPalette":"Snowy 02",
  //     "Effects": {
  //       "Function":"Static",
  //       "Speed":127,
  //       "Intensity":127,
  //       "Grouping":1,
  //       "RateMs": 25
  //     },
  //     "BrightnessRGB": 100,
  //     "BrightnessCCT": 0
  //   },
  //   "BrightnessRGB": 60,
  //   "BrightnessCCT": 0
  // }
  // )=====";

  
  // /***********************************
  //  * SECTION: Template Configs
  // ************************************/    

  // #define USE_MODULE_TEMPLATE
  // DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  // "{"
  //   "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
  //   "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  //   "\"" D_GPIO_NUMBER "\":{},"
  //   "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
  //   "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  // "}";

#endif // DEVICE END




#ifdef DEVICE_XMAS25__MEADOWS__DESK_PLAYLIST
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "coxmas24__redboard_01"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"


  /***********************************
  * SECTION: Enable Grouped
  ************************************/  

  /***********************************
   * SECTION: Lighting Configs
  ************************************/   

  #define ENABLE_DEVFEATURE_LIGHTS__GETTOKENALIAS


  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__BETA
  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE

  // #define ENABLE_DEBUGFEATURE_LIGHTING__SPLASH_FPS
  // #define ENABLE_DEBUGFEATURE_LIGHTING__EFFECT_LOOP_TIME_SERIAL

  #define ENABLE_BUSCONFG__OUTPUTS_1X_100
  // #define ENABLE_BUSCONFG__OUTPUTS_1X_2000 // Just for testing larger compute times

  #define ENABLE_DEVFEATURE_LIGHT__PLAYLIST_NAME_BASED_LOADING_4DEC25
  #define ENABLEDEVFEATURE_LIGHT__PLAYLIST_NAME_BASED_LOADING_4DEC25
  #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS
  #define ENABLE_FEATURE_LIGHTING__PRESET_FILE_METADATA
  #define ENABLE_DEVFEATURE_LIGHTING__PLAYLIST_BY_NAME_AUTOGENERATE_ID_LIST
  // #define ENABLE_DEBUGFEATURE_LIGHTING__PLAYLIST_PSN_TO_PS_CREATE_BACKUP_FILE
  #define ENABLE_DEVFEATURE_LIGHT__GRADIENT_PATCH_4DEC25
  #define ENABLE_DEVFEATURE_LIGHTS__FIX_MULTISEGMENT_DMA_FLICKER
  #define MIN_SHOW_DELAY 25



  #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE

  #define ENABLE_DEVFEATURE_LIGHTS__PRESET_TESTING_GROUPING_OVERRIDE_RESCALE_TO_STRIP // Testing playlists on smaller strips
  // #define ENABLE_DEVFEATURE_LIGHTS__PRESET_TESTING_BRIGHTNESS_BLOCKED // Testing playlists on smaller strips


  #ifdef ENABLE_BUSCONFG__OUTPUTS_1X_100

  /**
   * @brief tree physical wiring connections
   * 16 outputs
   */
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":13,
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
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Static",
        "Speed":127,
        "Intensity":127,
        "Grouping":1,
        "RateMs": 25
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 6,
    "Preset":{"Load":200}
  }
  )=====";
 
      
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      // "\"28\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\"," // Bus8
      // "\"13\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"12\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"26\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"32\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"14\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"27\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"25\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"33\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  #endif
  #ifdef ENABLE_BUSCONFG__OUTPUTS_1X_2000

  /**
   * @brief tree physical wiring connections
   * 16 outputs
   */
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
        "Length":250
      },
      {
        "Pin":18,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":250,
        "Length":250
      },
      {
        "Pin":19,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":500,
        "Length":250
      },
      {
        "Pin":21,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":750,
        "Length":250
      },
      {
        "Pin":16,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1000,
        "Length":250
      },
      {
        "Pin":17,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1250,
        "Length":250
      },
      {
        "Pin":22,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1500,
        "Length":250
      },
      {
        "Pin":23,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1750,
        "Length":250
      },
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2000,
        "Length":250
      },
      {
        "Pin":12,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2250,
        "Length":250
      },
      {
        "Pin":26,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2500,
        "Length":250
      },
      {
        "Pin":32,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2750,
        "Length":250
      },
      {
        "Pin":14,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":3000,
        "Length":250
      },
      {
        "Pin":27,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":3250,
        "Length":250
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        3500
      ],
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Static",
        "Speed":255,
        "Intensity":127,
        "Grouping":1,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";

 
  //     {
  //       "Pin":25,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":3500,
  //       "Length":250
  //     },
  //     {
  //       "Pin":33,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":3750,
  //       "Length":250
  //     }
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      // "\"28\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\"," // Bus8
      // "\"13\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"12\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"26\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"32\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"14\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"27\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"25\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"33\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #endif // END CONFIG OPTION
  

#endif // END DEVICE





#ifdef DEVICE_XMAS25__MEADOWS__WS2815_PANEL_12V
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "coxmas24__redboard_01"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"


  /***********************************
  * SECTION: Enable Grouped
  ************************************/  

  /***********************************
   * SECTION: Lighting Configs
  ************************************/   

  #define ENABLE_DEVFEATURE_LIGHTS__GETTOKENALIAS


  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__BETA
  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE

  // #define ENABLE_DEBUGFEATURE_LIGHTING__SPLASH_FPS
  // #define ENABLE_DEBUGFEATURE_LIGHTING__EFFECT_LOOP_TIME_SERIAL

  #define ENABLE_BUSCONFG__OUTPUTS_1X_100
  // #define ENABLE_BUSCONFG__OUTPUTS_1X_2000 // Just for testing larger compute times

  #define ENABLE_DEVFEATURE_LIGHT__PLAYLIST_NAME_BASED_LOADING_4DEC25
  #define ENABLEDEVFEATURE_LIGHT__PLAYLIST_NAME_BASED_LOADING_4DEC25
  #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS
  #define ENABLE_FEATURE_LIGHTING__PRESET_FILE_METADATA
  #define ENABLE_DEVFEATURE_LIGHTING__PLAYLIST_BY_NAME_AUTOGENERATE_ID_LIST
  // #define ENABLE_DEBUGFEATURE_LIGHTING__PLAYLIST_PSN_TO_PS_CREATE_BACKUP_FILE
  #define ENABLE_DEVFEATURE_LIGHT__GRADIENT_PATCH_4DEC25
  #define ENABLE_DEVFEATURE_LIGHTS__FIX_MULTISEGMENT_DMA_FLICKER
  #define MIN_SHOW_DELAY 25



  #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE

  #define ENABLE_DEVFEATURE_LIGHTS__PRESET_TESTING_GROUPING_OVERRIDE_RESCALE_TO_STRIP // Testing playlists on smaller strips
  // #define ENABLE_DEVFEATURE_LIGHTS__PRESET_TESTING_BRIGHTNESS_BLOCKED // Testing playlists on smaller strips


  #ifdef ENABLE_BUSCONFG__OUTPUTS_1X_100

  /**
   * @brief tree physical wiring connections
   * 16 outputs
   */
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":2,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":256
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        256
      ],
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Static",
        "Speed":127,
        "Intensity":127,
        "Grouping":1,
        "RateMs": 25
      },
      "BrightnessRGB": 100,
      "Preset":{"Load":200}
    },
    "BrightnessRGB": 100
  }
  )=====";
 
      
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      // "\"28\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\"," // Bus8
      // "\"13\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"12\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"26\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"32\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"14\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"27\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"25\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"33\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  #endif  

#endif // END DEVICE





#ifdef DEVICE_MEADOWS__BED_ALARM_LIGHT
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "coxmas24__redboard_01"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"


  /***********************************
  * SECTION: Enable Grouped
  ************************************/  

  /***********************************
   * SECTION: Lighting Configs
  ************************************/   


  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__BETA
  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE

  // #define ENABLE_DEBUGFEATURE_LIGHTING__SPLASH_FPS
  // #define ENABLE_DEBUGFEATURE_LIGHTING__EFFECT_LOOP_TIME_SERIAL

  #define ENABLE_BUSCONFG__OUTPUTS_1X_100
  // #define ENABLE_BUSCONFG__OUTPUTS_1X_2000 // Just for testing larger compute times

  #define ENABLE_DEVFEATURE_LIGHT__PLAYLIST_NAME_BASED_LOADING_4DEC25
  #define ENABLEDEVFEATURE_LIGHT__PLAYLIST_NAME_BASED_LOADING_4DEC25
  #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS
  #define ENABLE_FEATURE_LIGHTING__PRESET_FILE_METADATA


  #define ENABLE_DEVFEATURE_LIGHTING__PLAYLIST_BY_NAME_AUTOGENERATE_ID_LIST
  // #define ENABLE_DEBUGFEATURE_LIGHTING__PLAYLIST_PSN_TO_PS_CREATE_BACKUP_FILE

  #define ENABLE_DEVFEATURE_LIGHT__GRADIENT_PATCH_4DEC25
  #define ENABLE_DEVFEATURE_LIGHTS__FIX_MULTISEGMENT_DMA_FLICKER
  #define MIN_SHOW_DELAY 25
  #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE

  #define ENABLE_DEVFEATURE_LIGHTS__PRESET_TESTING_GROUPING_OVERRIDE_RESCALE_TO_STRIP // Testing playlists on smaller strips
  // #define ENABLE_DEVFEATURE_LIGHTS__PRESET_TESTING_BRIGHTNESS_BLOCKED // Testing playlists on smaller strips


  #ifdef ENABLE_BUSCONFG__OUTPUTS_1X_100

  /**
   * @brief tree physical wiring connections
   * 16 outputs
   */
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":2,
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
      "ColourPalette":"Cold White",
      "Effects": {
        "Function":"Solid",
        "Speed":127,
        "Intensity":127,
        "Grouping":1,
        "RateMs": 25
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 100
  }
  )=====";
 
      
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      // "\"28\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\"," // Bus8
      // "\"13\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"12\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"26\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"32\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"14\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"27\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"25\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"33\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  #endif
  

#endif // END DEVICE

#endif // _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_CHRISTMAS23_H
