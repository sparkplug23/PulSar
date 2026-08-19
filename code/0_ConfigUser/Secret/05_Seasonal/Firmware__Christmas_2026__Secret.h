#ifndef _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_CHRISTMAS26_H
#define _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_CHRISTMAS26_H

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"






/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Enable Defines: Whitehall *******************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


// #define DEVICE_XMAS26__MEADOWS__FRONT_TREE




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

  #define ENABLE_PIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE

  
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







#endif