#ifndef _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_CHRISTMAS26_H
#define _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_CHRISTMAS26_H

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"


/*****
 * Things to do:
 * 1. [Meadows] Front tree needs new controller
 * 2. [Whitehall] Snow tree needs new controller
 * 3. [Meadows] Need to test pebble lights, specifically issues with power, flicker, and gamma
 * 4. [Meadows] Possible icicle lights, would need another set of lights
 * 5. [Whitehall] Put snow tree up in garage attic, so I can control it. This needs a camera (2nd roaming one)
 * 6. [Meadows] Install front controller lights onto wall for testing while in colorado?
 *               - Hang wood from ceiling using rope
 *               - Then light I did before, wire them all up
 *               - Use my energy_socket for control and power
 *               - If hung high enough, I could have 2m drops, and make a test curtain of sorts
 * - Without garage tree/fence lights, I should use the 5cm lights or similar to add the others needed. 
 * - Plan on 3000 lights again, perhaps test up to 4000 for FPS but expect not going to work. This is where more 20cm spaced lights would be ideal.
 * 
 * 
 * hub75 over
 * esp32s3
 * make 3 esp32s3 little boards, with the 5v green pebbles leds
 * ideally figure out the proper "decimate hardware" where I can view 4000 pixels, but only seeing the 100.
 * I will want to compare normal esp32 running and esp32s3 running same playlists, ideally get python to log FPS (poll state mid preset run) and save to a json file on computer, just do with mqtt commands (ie make a get, save response). This will let me test lots of types of hardware for xmas26
 * 
 * 
 * before going
 * need to make X16 esp32s3 boards, probably aim for 3 (all outdoor trees)
 * 
 * https://github.com/sparkplug23/WLED-3D-Locator
 * bring pixel 4xl for this task
 * bring glasses sock + smarttag!
 * 
 */



/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Enable Defines: Whitehall *******************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


// #define DEVICE_XMAS26__MEADOWS__FRONT_TREE
// #define DEVICE_XMAS26__MEADOWS__FRONT_TREE_ESP32S3_N16R8_02




/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Enable Defines: Whitehall *******************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/





#ifdef DEVICE_XMAS26__MEADOWS__FRONT_TREE
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

  
  #define ENABLE_DEBUGFEATURE_LIGHTING__VIRTUALVIEW

  #define ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_METHODS__PARALLEL_FORCED_X16

  
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
      // "\"28\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\"," // Bus8
      // "\"13\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      "\"12\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      "\"26\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      "\"32\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      "\"14\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      "\"27\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      "\"25\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      "\"33\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_KEY3_INV_CTR  "\""
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
      // "\"28\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\"," // Bus8
      // "\"13\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      "\"12\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      "\"26\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      "\"32\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      "\"14\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      "\"27\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      "\"25\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      "\"33\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_KEY3_INV_CTR  "\""
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
      // "\"28\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\"," // Bus8
      // "\"13\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      "\"12\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      "\"26\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      "\"32\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      "\"14\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      "\"27\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      "\"25\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      "\"33\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  #endif

  

  #ifdef ENABLE_BUSCONFG__OUTPUTS_INSTALLED_ON_TREE

//   #define ENABLE_DEVFEATURE_LIGHTS__SEGMENT_MATCHBUS
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
        "n":"L2",
        "Pin":18,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":250,
        "Length":250
      },
      {
        "n":"L3",
        "Pin":16,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":500,
        "Length":250
      },
      {
        "n":"L4",
        "Pin":17,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":750,
        "Length":250
      },
      {
        "n":"L5",
        "Pin":19,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1000,
        "Length":200
      },
      {
        "n":"L6",
        "Pin":21,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1200,
        "Length":200
      },
      {
        "n":"L7",
        "Pin":22,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1400,
        "Length":200
      },
      {
        "n":"L8",
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
    "Segments":[
      {
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
            "Decimate":1,
            "Custom1":255,
            "Custom2":220,
            "RateMs": 1000
        },
        "BrightnessRGB": 100
      }
    ],
    "BrightnessRGB": 100
  }
  )=====";


  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      // "\"28\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\"," // Bus8
      // "\"13\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
    //   "\"12\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
    //   "\"26\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
    //   "\"32\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
    //   "\"14\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
    //   "\"27\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
    //   "\"25\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
    //   "\"33\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_KEY3_INV_CTR  "\""
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
      // "\"28\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\"," // Bus8
      // "\"13\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
          // "\"12\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
          // "\"26\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
          // "\"32\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
          // "\"14\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
          // "\"27\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
          "\"25\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
          "\"33\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_KEY3_INV_CTR  "\""
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
      // "\"28\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\"," // Bus8
      // "\"13\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
          // "\"12\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
          // "\"26\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
          // "\"32\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
          // "\"14\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
          // "\"27\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
          "\"25\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
          "\"33\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  #endif

#endif // DEVICE_END



/**
 * @brief
 *
 * Xmas 2026 Meadows Front Tree
 * ESP32-S3 N16R8
 *
 * 16 lighting outputs via 2x SN74HCT245
 *
 *          STRAP  -> Boot/configuration strapping pin - avoid for LED outputs
 *          USB    -> Native USB D-/D+
 *          UART0  -> Default UART0 TX/RX
 *          JTAG   -> Default JTAG-capable pins
 *          MEM    -> Flash / Octal PSRAM - unavailable on N16R8
 *
 *          OUT01 -> OUT16 = dedicated lighting data outputs
 *
 *
 *                              ESP32-S3 N16R8
 *
 *                          _______________________
 *                    3V3  |3V3               GND |
 *                    RST  |RST               TX43| UART0
 *             OUT03 GPIO4 |4                 RX44| UART0
 *             OUT04 GPIO5 |5                    1| GPIO1  OUT01
 *             OUT05 GPIO6 |6                    2| GPIO2  OUT02
 *             OUT06 GPIO7 |7                   42| JTAG / spare
 *             OUT14 GPIO15|15                  41| JTAG / spare
 *             OUT15 GPIO16|16                  40| JTAG / spare
 *             OUT16 GPIO17|17                  39| JTAG / spare
 *              SPARE GPIO18|18                 38| spare
 *             OUT07 GPIO8 |8                   37| MEM - Octal PSRAM
 *              STRAP GPIO3|3                   36| MEM - Octal PSRAM
 *              STRAP GPIO46|46                 35| MEM - Octal PSRAM
 *             OUT08 GPIO9 |9                    0| STRAP / BOOT
 *             OUT09 GPIO10|10                  45| STRAP
 *             OUT10 GPIO11|11                  48| spare / board dependent
 *             OUT11 GPIO12|12                  47| spare
 *             OUT12 GPIO13|13                  21| spare
 *             OUT13 GPIO14|14                  20| USB D+
 *                    5V   |5V                   19| USB D-
 *                    GND  |GND                 GND|
 *                          _______________________
 *
 *
 * Lighting output allocation
 *
 * SN74HCT245 #1
 *
 *   OUT01 -> GPIO1
 *   OUT02 -> GPIO2
 *   OUT03 -> GPIO4
 *   OUT04 -> GPIO5
 *   OUT05 -> GPIO6
 *   OUT06 -> GPIO7
 *   OUT07 -> GPIO8
 *   OUT08 -> GPIO9
 *
 * SN74HCT245 #2
 *
 *   OUT09 -> GPIO10
 *   OUT10 -> GPIO11
 *   OUT11 -> GPIO12
 *   OUT12 -> GPIO13
 *   OUT13 -> GPIO14
 *   OUT14 -> GPIO15
 *   OUT15 -> GPIO16
 *   OUT16 -> GPIO17
 *
 * Reserved / deliberately avoided
 *
 *   GPIO0, GPIO3, GPIO45, GPIO46 -> strapping
 *   GPIO19, GPIO20               -> native USB
 *   GPIO26..GPIO37               -> Flash / Octal PSRAM on N16R8
 *   GPIO39..GPIO42               -> leave available for JTAG/debug
 *   GPIO43, GPIO44               -> UART0
 *
 * Useful spare GPIO
 *
 *   GPIO18
 *   GPIO21
 *   GPIO47
 *   GPIO48
 *
 */


#ifdef DEVICE_XMAS26__MEADOWS__FRONT_TREE_ESP32S3_N16R8
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
//   #define USE_MODULE_SENSORS_INTERFACE
//   #define USE_MODULE_SENSORS_PIR
//   // #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT
//   #endif


  /***********************************
   * SECTION: Lighting Configs
  ************************************/


  // #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__BETA
  // #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE

  // #define ENABLE_DEBUGFEATURE_LIGHTING__SPLASH_FPS
  // #define ENABLE_DEBUGFEATURE_LIGHTING__EFFECT_LOOP_TIME_SERIAL

  // #define ENABLE_BUSCONFIG_8X_TEST_OUTPUTS
  // #define ENABLE_BUSCONFIG_16X_1800_200
  // #define ENABLE_BUSCONFIG_16X_TESTING
  // #define ENABLE_BUSCONFIG_16X_BUS_EACH_A_SEGMENT
  // #define ENABLE_BUSCONFIG_16X_TEST_MANUAL_BUS_PINS
  // #define ENABLE_BUSCONFG__OUTPUTS_INSTALLED_ON_TREE
  #define ENABLE_BUSCONFG__OUTPUTS_TESTING_FOR_S3_BUILD

  // #define ENABLE_DEBUGFEATURE_LIGHTING__VIRTUALVIEW

  // #define ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_METHODS__PARALLEL_FORCED_X16


  #ifdef ENABLE_BUSCONFG__OUTPUTS_TESTING_FOR_S3_BUILD

  // // #define ENABLE_DEVFEATURE_LIGHTS__SEGMENT_MATCHBUS
  // #define BUSCONFIG_MAX_PINS_FOR_PARALLEL_I2S 1000
  // #define MAX_LED_MEMORY 64000*5
  // #define MAX_NUM_SEGMENTS 16

  /**
   * @brief tree physical wiring connections
   *
   * 16-port PCB:
   *
   *   OUT01 GPIO1
   *   OUT02 GPIO2
   *   OUT03 GPIO4
   *   OUT04 GPIO5
   *   OUT05 GPIO6
   *   OUT06 GPIO7
   *   OUT07 GPIO8
   *   OUT08 GPIO9
   *
   *   OUT09 GPIO10
   *   OUT10 GPIO11
   *   OUT11 GPIO12
   *   OUT12 GPIO13
   *   OUT13 GPIO14
   *   OUT14 GPIO15
   *   OUT15 GPIO16
   *   OUT16 GPIO17
   *
   * Current tree uses outputs 1-14.
   * Outputs 15-16 remain available.
   */

   #define ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_METHODS__PARALLEL_AUTO

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE)
  R"=====(
  {
    "BusConfig":[
      {
        "n":"L1",
        "Pin":1,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":250
      },
      {
        "n":"L2",
        "Pin":2,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":250,
        "Length":250
      },
      {
        "n":"L3",
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":500,
        "Length":250
      },
      {
        "n":"L4",
        "Pin":5,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":750,
        "Length":250
      },
      {
        "n":"L5",
        "Pin":6,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1000,
        "Length":200
      },
      {
        "n":"L6",
        "Pin":7,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1200,
        "Length":200
      },
      {
        "n":"L7",
        "Pin":8,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1400,
        "Length":200
      },
      {
        "n":"L8",
        "Pin":9,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1600,
        "Length":200
      },
      {
        "n":"L9",
        "Pin":10,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1800,
        "Length":200
      },
      {
        "n":"L10",
        "Pin":11,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2000,
        "Length":200
      },
      {
        "n":"L11",
        "Pin":12,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2200,
        "Length":200
      },
      {
        "n":"L12",
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2400,
        "Length":200
      },
      {
        "n":"L13",
        "Pin":14,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2600,
        "Length":200
      },
      {
        "n":"L14",
        "Pin":15,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2800,
        "Length":200
      }
    ],
    "Segments":[
      {
        "PixelRange":[
          0,
          3000
        ],
        "ColourPalette":"Snowy 02",
        "Palette2":"Cold White",
        "Effects":{
          "Function":"Static",
          "Intensity":1,
          "Speed":255,
          "Grouping":1,
          "Decimate":1,
          "Custom1":255,
          "Custom2":220,
          "RateMs":1000
        },
        "BrightnessRGB":100
      }
    ],
    "BrightnessRGB":100
  }
  )=====";


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE)
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"18\":\"" D_GPIO_KEY1_INV_CTR  "\","
      "\"21\":\"" D_GPIO_KEY2_INV_CTR  "\","
      "\"47\":\"" D_GPIO_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #endif // ENABLE_BUSCONFG__OUTPUTS_INSTALLED_ON_TREE

#endif // DEVICE_XMAS26__MEADOWS__FRONT_TREE_ESP32S3_N16R8


#ifdef DEVICE_XMAS26__MEADOWS__FRONT_TREE_ESP32S3_N16R8_02
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
//   #define USE_MODULE_SENSORS_INTERFACE
//   #define USE_MODULE_SENSORS_PIR
//   // #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT
//   #endif


  /***********************************
   * SECTION: Lighting Configs
  ************************************/


  // #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__BETA
  // #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE

  // #define ENABLE_DEBUGFEATURE_LIGHTING__SPLASH_FPS
  // #define ENABLE_DEBUGFEATURE_LIGHTING__EFFECT_LOOP_TIME_SERIAL

  // #define ENABLE_BUSCONFIG_8X_TEST_OUTPUTS
  // #define ENABLE_BUSCONFIG_16X_1800_200
  // #define ENABLE_BUSCONFIG_16X_TESTING
  // #define ENABLE_BUSCONFIG_16X_BUS_EACH_A_SEGMENT
  // #define ENABLE_BUSCONFIG_16X_TEST_MANUAL_BUS_PINS
  // #define ENABLE_BUSCONFG__OUTPUTS_INSTALLED_ON_TREE
  #define ENABLE_BUSCONFG__OUTPUTS_TESTING_FOR_S3_BUILD

  // #define ENABLE_DEBUGFEATURE_LIGHTING__VIRTUALVIEW

  // #define ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_METHODS__PARALLEL_FORCED_X16


  #ifdef ENABLE_BUSCONFG__OUTPUTS_TESTING_FOR_S3_BUILD

  // // #define ENABLE_DEVFEATURE_LIGHTS__SEGMENT_MATCHBUS
  // #define BUSCONFIG_MAX_PINS_FOR_PARALLEL_I2S 1000
  // #define MAX_LED_MEMORY 64000*5
  // #define MAX_NUM_SEGMENTS 16

  /**
   * @brief tree physical wiring connections
   *
   * 16-port PCB:
   *
   *   OUT01 GPIO1
   *   OUT02 GPIO2
   *   OUT03 GPIO4
   *   OUT04 GPIO5
   *   OUT05 GPIO6
   *   OUT06 GPIO7
   *   OUT07 GPIO8
   *   OUT08 GPIO9
   *
   *   OUT09 GPIO10
   *   OUT10 GPIO11
   *   OUT11 GPIO12
   *   OUT12 GPIO13
   *   OUT13 GPIO14
   *   OUT14 GPIO15
   *   OUT15 GPIO16
   *   OUT16 GPIO17
   *
   * Current tree uses outputs 1-14.
   * Outputs 15-16 remain available.
   */

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE)
  R"=====(
  {
    "BusConfig":[
      {
        "n":"L1",
        "Pin":1,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":250
      },
      {
        "n":"L2",
        "Pin":2,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":250,
        "Length":250
      },
      {
        "n":"L3",
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":500,
        "Length":250
      },
      {
        "n":"L4",
        "Pin":5,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":750,
        "Length":250
      },
      {
        "n":"L5",
        "Pin":6,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1000,
        "Length":200
      },
      {
        "n":"L6",
        "Pin":7,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1200,
        "Length":200
      },
      {
        "n":"L7",
        "Pin":8,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1400,
        "Length":200
      },
      {
        "n":"L8",
        "Pin":9,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1600,
        "Length":200
      },
      {
        "n":"L9",
        "Pin":10,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1800,
        "Length":200
      },
      {
        "n":"L10",
        "Pin":11,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2000,
        "Length":200
      },
      {
        "n":"L11",
        "Pin":12,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2200,
        "Length":200
      },
      {
        "n":"L12",
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2400,
        "Length":200
      },
      {
        "n":"L13",
        "Pin":14,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2600,
        "Length":200
      },
      {
        "n":"L14",
        "Pin":15,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2800,
        "Length":200
      }
    ],
    "Segments":[
      {
        "PixelRange":[
          0,
          3000
        ],
        "ColourPalette":"Snowy 02",
        "Palette2":"Cold White",
        "Effects":{
          "Function":"Static",
          "Intensity":1,
          "Speed":255,
          "Grouping":1,
          "Decimate":1,
          "Custom1":255,
          "Custom2":220,
          "RateMs":1000
        },
        "BrightnessRGB":100
      }
    ],
    "BrightnessRGB":100
  }
  )=====";


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE)
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"18\":\"" D_GPIO_KEY1_INV_CTR  "\","
      "\"21\":\"" D_GPIO_KEY2_INV_CTR  "\","
      "\"47\":\"" D_GPIO_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #endif // ENABLE_BUSCONFG__OUTPUTS_INSTALLED_ON_TREE

#endif // DEVICE_XMAS26__MEADOWS__FRONT_TREE_ESP32S3_N16R8



#endif