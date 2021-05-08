#ifndef MSYSTEMCONFIG_HARDWAREDEFAULTS_FIRMWAR_MIC4HAEL_H
#define MSYSTEMCONFIG_HARDWAREDEFAULTS_FIRMWAR_MIC4HAEL_H

/*********************************************************************************************\
 * This file is one of two user configurable files
 * 
 * "mFirmwareCustom.h" allows advanced users to create a custom build of firmware tailored to a specefic tasks 
 * For example, using a device to control heating, which includes numorous sensors, relays and controls that are unique.
 * This can also include lights or displays as notification/information systems. 
 * Although all sensors/drivers are accesible through default builds, this allows users to config and add their own controllers.
 * Ideally, when possible, templates can be used to replicate the custom firmware options performed here.
\*********************************************************************************************/

// #define DEVICE_FORCED_TO_BE_TESTER
#define DISABLE_WEBSERVER
#define FORCE_TEMPLATE_LOADING
#define USE_MODULE_NETWORK_MQTT

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/Languages/mLanguageDefault.h"
#include "6_Lights/_Interface/mInterfaceLight_Defines.h"
#include "2_CoreSystem/HardwareTemplates/mHardwareTemplates.h"

//--------------------------------[Enable Device]-------------------------------------

/**
 *  SHORT TERM DEVICES IN BEDROOM -- SHORT TERM DEVICES -- SHORT TERM DEVICES -- SHORT TERM DEVICES -- SHORT TERM DEVICES -- SHORT TERM DEVICES -- SHORT TERM DEVICES -- SHORT TERM DEVICES -- SHORT TERM DEVICES -- 
**/  
// #define DEVICE_RGBBEDLIGHT 
// #define DEVICE_RGBBEDROOMFLOOR
// #define DEVICE_RGBCLOCK_01
// #define DEVICE_RGBCLOCK_02 
// #define DEVICE_RGBFIREPLACE_TESTER  
// #define DEVICE_RGBDESK        
// #define DEVICE_RGBDISPLAY_GARAGE   
// #define DEVICE_RGBMICRO3 //Alpha tester, before rgbroof "RGBROOF2"      
//#define DEVICE_BEDROOMBLINDS     
// #define DEVICE_DESKFAN
// #define DEVICE_DESKPANEL
// #define DEVICE_HVAC_BEDROOM
#define DEVICE_BEDROOM_PZEM_TESTER

/**
 *  DEV -- -- DEV -- -- DEV -- -- DEV -- -- DEV -- -- DEV -- -- DEV -- -- DEV -- -- DEV -- -- 
**/  
// #define DEVICE_GPSPARSER_TESTER
// #define DEVICE_CAM_TESTER
// #define DEVICE_CAMERA_DRIVEWAY
// #define DEVICE_H801_TESTER
// #define DEVICE_HEATING_ESP32_TESTER

/**
 *  LIGHTING -- LIGHTING -- LIGHTING -- LIGHTING -- LIGHTING -- LIGHTING -- LIGHTING -- LIGHTING -- LIGHTING -- 
**/  
// #define DEVICE_RGBROOF
// #define DEVICE_RGBDELL
// #define DEVICE_RGBNOTIFICATION_01           
//#define DEVICE_RGBBEDLIGHT_TEST 
//#define DEVICE_RGBCUSTOM_USER_01
//#define DEVICE_RGBCUSTOM_USER_02
// #define DEVICE_TESTER_RGBW
// #define DEVICE_RGBMICRO1 //
// #define DEVICE_RGBMICRO2 //              
// #define DEVICE_RGBMICRO4 //
// #define DEVICE_RGBESP32_1_TESTER

/**
 *  CONTROLLERS   -- CONTROLLERS   -- CONTROLLERS   -- CONTROLLERS   -- CONTROLLERS   -- CONTROLLERS   -- CONTROLLERS   -- CONTROLLERS   -- CONTROLLERS   -- 
**/
// #define DEVICE_SHELLY1_TESTER
//#define DEVICE_KITCHENPANEL
//#define DEVICE_BLACKDOORBELL
// #define DEVICE_CAM_TESTER
// #define DEVICE_GPSPARSER_TESTER
// #define DEVICE_SDCARDWRITER_TESTER
// #define DEVICE_TREADMILL
// #define DEVICE_SDLOGGER1_TESTER

/**
 *  ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- 
**/
// #define DEVICE_PZEM_TESTER

/**
 *  SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- 
**/
// #define DEVICE_TESTSENSOR
// #define DEVICE_NODEMCU_TESTER
// #define DEVICE_BEDROOMSENSOR_TESTER

/**
 *  DISPLAYS   -- DISPLAYS   -- DISPLAYS   -- DISPLAYS   -- DISPLAYS   -- DISPLAYS   -- DISPLAYS   -- DISPLAYS   -- DISPLAYS   -- 
**/
// #define DEVICE_BEDROOMPANEL

/**
 *  SONOFF and lighting  -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- 
**/
//#define DEVICE_SONOFFTESTER

/**
 *  SONOFF iFANS
 * and lighting  -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- 
**/

/**
 *  SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- 
**/
// #define DEVICE_SHELLYDIMMER_GLOBE
// #define DEVICE_SIDEDOORLIGHT_TEST

/**
 *  TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- 
**/
// #define DEVICE_TESTBED_SHELLY1_01
// #define DEVICE_TESTBED_SHELLY2_DIMMER
// #define DEVICE_TESTBED_SHELLY2P5_01
// #define DEVICE_ESP32_DEVKIT_BASIC
// #define DEVICE_ESP32_WEBCAM1

// Include the home devices, which should ONLY contain final hardware
#include "0_ConfigUser/mFirmwareCustom_Secret_Home.h"


//-----------------[User Defined Devices == USE_BUILD_TYPE_ENERGY == Any Energy Monitoring Firmware]-------------------------------------


#ifdef DEVICE_PZEM_TESTER
  #define DEVICENAME_CTR          "pzem_tester"
  #define DEVICENAME_FRIENDLY_CTR "PZEM Tester"
  
//   #define FORCE_TEMPLATE_LOADING
//   #define SETTINGS_HOLDER 2

//   #define USE_BUILD_TYPE_ENERGY
//   #define USE_MODULE_SENSORS_PZEM004T_V3
//   #define MAX_ENERGY_SENSORS 2

//   // #define ENABLE_BUG_TRACING
  
//   #define USE_SOFTWARE_SERIAL_DEBUG
//   #define DISABLE_SERIAL_LOGGING //temp measure

//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
//     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_JSON_GPIOC "\":{"
//       "\"1\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
//       "\"3\":\""  D_GPIO_FUNCTION_PZEM0XX_MODBUS__RX_CTR "\"," 
//       "\"D0\":\""  D_GPIO_FUNCTION_LED1_INV_CTR   "\","  
//       "\"D4\":\""  D_GPIO_FUNCTION_LED1_CTR "\""
//     "},"
//     "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
//   "}";

//   #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "Closed CT 100A"
//   #define D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "Open CT 100A"
//   #define D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "Inline 100ohm 10A"
  
//   // Drivers, Sensors and lights?
//   #define USE_FUNCTION_TEMPLATE
//   DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
//   "{"
//     "\"" D_JSON_DEVICENAME "\":{"
//       "\"" D_INTERFACE_ENERGY_MODULE_FRIENDLY_CTR "\":["
//         "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
//         "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\","
//         "\"" D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "\""
//       "]"
//     "}"
//   "}";
// #endif

// #ifdef DEVICE_CONSUMERUNIT
//   #define DEVICENAME_CTR          "consumerunit33"
//   #define DEVICENAME_FRIENDLY_CTR "Consumer Unit33"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2

  #define USE_BUILD_TYPE_ENERGY
  #define USE_MODULE_ENERGY_INTERFACE
  #define USE_MODULE_ENERGY_PZEM004T_V3
  #define USE_DEVFEATURE_ENABLE_PZEM004T_SERIAL2

  // #define USE_MODULE_SENSORS_BME
  #define D_DEVICE_SENSOR_CLIMATE "Downstairs Toilet"

  #define ESP32

  // #define ENABLE_BUG_TRACING
  
  // #define USE_SOFTWARE_SERIAL_DEBUG
  //#define DISABLE_SERIAL_LOGGING //temp measure

  // Switching to esp32 for dual serial ports, to enable isolated pzem serial traffic until I can be assured I can disable serial0 on the esp8266

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      // #ifdef ESP266
      //   #ifdef USE_MODULE_SENSORS_BME
      //   "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      //   "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      //   #endif
      //   "\"1\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
      //   "\"3\":\""  D_GPIO_FUNCTION_PZEM0XX_MODBUS__RX_CTR "\"," 
      //   "\"D0\":\""  D_GPIO_FUNCTION_LED1_INV_CTR   "\","  
      //   "\"D4\":\""  D_GPIO_FUNCTION_LED1_CTR "\""
      // #endif //ESP266
      #ifdef ESP32
        #ifdef USE_MODULE_SENSORS_BME
        // "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
        // "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
        #endif
        // "\"1\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
        // "\"3\":\""  D_GPIO_FUNCTION_PZEM0XX_MODBUS__RX_CTR "\"," 
        // "\"D0\":\""  D_GPIO_FUNCTION_LED1_INV_CTR   "\","  
        "\"LBI\":\""  D_GPIO_FUNCTION_LED1_CTR "\""
      #endif //ESP32
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "Mains"
  #define D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "Cooker"
  #define D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "Tumble Dryer"
  #define D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR "Washing Machine"
  #define D_DRIVER_ENERGY_4_FRIENDLY_NAME_CTR "Dish Washer"
  #define D_DRIVER_ENERGY_5_FRIENDLY_NAME_CTR "Shower"
  #define D_DRIVER_ENERGY_6_FRIENDLY_NAME_CTR "Sockets"
  #define D_DRIVER_ENERGY_7_FRIENDLY_NAME_CTR "Kitchen Sockets"
  
  // Drivers, Sensors and lights?
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
      "]"
    "}"
  "}";

#endif


#ifdef DEVICE_BEDROOM_PZEM_TESTER
  #define DEVICENAME_CTR          "bedroom_pzem"
  #define DEVICENAME_FRIENDLY_CTR "Bedroom PZEM Tester"
  #define ESP32
    
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2

  #define USE_MODULE_ENERGY_INTERFACE
  #define USE_MODULE_ENERGY_PZEM004T_V3
  #define USE_DEVFEATURE_ENABLE_PZEM004T_SERIAL2

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"      
      "\"16\":\""  D_GPIO_FUNCTION_PZEM0XX_MODBUS__RX_CTR "\"," 
      "\"17\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "Mains"
  #define D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "Test"
  #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "24"
  #define D_DEVICE_SENSOR_PZEM004T_1_ADDRESS "25"
  #define ENERGY_SENSORS_DYNAMIC_TEMPFIX 2
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR "\":[" 
        D_DEVICE_SENSOR_PZEM004T_0_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_1_ADDRESS ""
      "]"  
    "},"
    "\"" D_JSON_ENERGY "\":{"
        "\"DeviceCount\":2"    
    "}"
  "}";

#endif


//-----------------[User Defined Devices == USE_BUILD_TYPE_LIGHTING == RGB Lighting] ----------------------------


#ifdef DEVICE_CAM_TESTER 
  #define DEVICENAME_CTR          "cam_tester"
  #define DEVICENAME_FRIENDLY_CTR "Camera OV2640 Tester"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1//random(1,1000)

  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  // #define ENABLE_DEVFEATURE_ARDUINOJSON

  // #define ENABLE_DEVFEATURE_BREAK_ADDLOG

  // #define ENABLE_PIXEL_FUNCTION_EFFECTS

//  #define USE_PUBSUB_V1

  //#define ENABLE_DEVFEATURE_MQTT_CONNECTION_EDIT1

  // #define ENABLE_BUG_TRACING

  // #define DISABLE_WEBSERVER

  //#define ENABLE_LOG_FILTERING_TEST_ONLY

  #define STRIP_SIZE_MAX 50//*15
  //#define ENABLE_PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL_CCT_SPACE
  //#define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  // #define USE_INTERFACE_NEW

  // #define USE_MODULE_ENERGY_INA219
  // #define USE_MODULE_SENSORS_BME

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR

  //#define ENABLE_DEVFEATURE_LIGHTING_//Scene_OBJECT_TO_STRUCT "v78.24.11+" //only remove when all device exceed this
  //#define ENABLE_DEVFEATURE_RGBCOLOR_DESIRED
  //#define ENABLE_DEVFEATURE_SINGLE_ANIMATOR_INTERFACE   "v79.31.22+"   
  #define ENABLE_DEVFEATURE_PIXEL_LIVEVIEW_IN_PAGE_ROW    "v79.31.22+" 
  #define ENABLE_DEVFEATURE_PIXEL_NEW_JSON_PALETTE_SELECT_ROOT

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
//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE)   
//  "{\"Na\":\"rgb\",\"FNa\":\"fname\",\"GC\":{\"pin1\":\"func1\",\"pin2\":\"func2\"}}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" D_JSON_WS2812 "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"" D_PIXEL_HARDWARE_COLOR_ORDER_GRB_CTR "\","
    "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_COLOUR_PALETTE   "\":43,"
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_JSON_//Scene "\","
    "\"" D_JSON_//Scene_COLOUR     "\":{\"" D_JSON_HSB "\":[120,100,0]" "}," //this set the brightness
    "\"" D_JSON_BRIGHTNESS       "\":0,"
    "\"" D_JSON_BRIGHTNESS_RGB          "\":0"
  "}";

  #define D_DEVICE_SENSOR_CURRENT "Module 1"
  
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

#ifdef DEVICE_CAMERA_DRIVEWAY 
  #define DEVICENAME_CTR          "camera_driveway"
  #define DEVICENAME_FRIENDLY_CTR "Camera OV2640 Driveway"

  #define FORCE_TEMPLATE_LOADING

  #define USE_MODULE_DRIVERS_CAMERA_OV2640
  
  // #define USE_MODULE_NETWORK_WEBSERVER

  //#define ENABLE_LOG_FILTERING_TEST_ONLY

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE)   
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"    
      // #if defined(USE_MODULE_ENERGY_INA219) || defined(USE_MODULE_SENSORS_BME)
      // "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      // "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      // #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR   "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define D_DEVICE_SENSOR_CURRENT "Module 1"
  
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

#ifdef DEVICE_CAMERA_2 
  #define DEVICENAME_CTR          "camera_2"
  #define DEVICENAME_FRIENDLY_CTR "Camera OV2640 2"

  #define FORCE_TEMPLATE_LOADING

  #define USE_MODULE_DRIVERS_CAMERA_OV2640
  
  // #define USE_MODULE_NETWORK_WEBSERVER

  //#define ENABLE_LOG_FILTERING_TEST_ONLY

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE)   
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"    
      // #if defined(USE_MODULE_ENERGY_INA219) || defined(USE_MODULE_SENSORS_BME)
      // "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      // "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      // #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR   "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define D_DEVICE_SENSOR_CURRENT "Module 1"
  
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



#ifdef DEVICE_RGBROOF
  #define DEVICENAME_CTR            "rgbroof"
  #define DEVICENAME_FRIENDLY_CTR   "Bedroom String Lights"

  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 21

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  // #define USE_MODULE_LIGHTS_WLED_EFFECTS
  // #define WLED_DEFINE_GLOBAL_VARS //only in one source file, wled.cpp!
  // #define DISABLE_PIXEL_FUNCTION_EFFECTS
  // #define USE_MODULE_DRIVERS_LEDS
  #define DISABLE_WEBSERVER
  
//   //#define USE_WEBSERVER_ADVANCED_MULTIPAGES // new develop option to limit scope to only include root page while testing
  
//   #define USE_MODULE_SENSORS_SWITCHES
//   #define USE_MODULE_SENSORS_ANALOG

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

  
  #define STRIP_SIZE_MAX 100//256
  #define STRIP_REPEAT_OUTPUT_MAX 256

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
    "\"ColourPalette\":\"Single Fire 01\","//Christmas MultiColoured Warmer\","
    "\"BrightnessRGB\":2"
  "}";

#endif




#ifdef DEVICE_TESTER_RGBW
  #define DEVICENAME_CTR            "tester_rgbw"
  #define DEVICENAME_FRIENDLY_CTR   "Tester RGBW WS2812 Lights"

  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 21

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR

  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  #define USE_SK6812_METHOD_DEFAULT

  // Advanced options  
  //#define ENABLE_DEVFEATURE_SETPIXELOUTPUT_VARIABLE
  // #define USE_DEVFEATURE_PIXEL_OUTPUT_MULTIPLIER 2

  //#define USE_WEBSERVER_ADVANCED_MULTIPAGES // new develop option to limit scope to only include root page while testing
  
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
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_//Scene  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":1" //slow glow
    "},"
    "\"Transition\":{\"Order\":\"InOrder\",\"PixelUpdatePerc\":2,\"RateMs\":10000},"
    "\"TimeMs\":0,"
    "\"ColourPalette\":\"" "User 01" "\","
    "\"BrightnessRGB\":100"
  "}";

#endif

#ifdef DEVICE_RGBDELL
  #define DEVICENAME_CTR            "rgbdell"
  #define DEVICENAME_FRIENDLY_CTR   "RGB Dell"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
   
  #define DISABLE_WEBSERVER

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE

  //#define ENABLE_PIXEL_FUNCTION_AMBILIGHT
  
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

  //ADALIGHT_ViaSerialAndWifi
  //SCREENEDGES


  #define STRIP_SIZE_MAX 133

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":\"" D_EFFECTS_FUNCTION_SOLID_COLOUR_NAME_CTR "\""
    "},"
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":1000,"
      "\"" D_JSON_RATE_MS "\":1000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
    "},"
    "\"" D_JSON_CCT_TEMP "\":300,"
    "\"" D_JSON_HUE "\":15,"
    "\"" D_JSON_SAT "\":90,"
    "\"" D_JSON_COLOUR_PALETTE "\":\"RGBCCTColour 00\","
    "\"" D_JSON_BRIGHTNESS_CCT "\":100,"
    "\"" D_JSON_BRIGHTNESS_RGB "\":100"
  "}";

#endif

/**
 * New heating controller, designed to work from single device to multizone system
 * */
#ifdef DEVICE_HVAC_BEDROOM
  #define DEVICENAME_CTR          "hvacbedroom"
  #define DEVICENAME_FRIENDLY_CTR "HVAC Bedroom"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1 //maintain other settings (bootcount)
   
  // #define ENABLE_BUG_TRACING
  //#define ENABLE_MQTT_DEBUG_MESSAGES

  //#define FORCE_DEVICENAME_CLEAR_ON_BOOT
  // #define ENABLE_HVAC_DEBUG_TIMES
  #define DISABLE_WEBSERVER
  
  #define ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  #define EMABLE_DEVFEATURE_HARDWAREPINS_CLEANED_UP

  #define USE_MODULE_CONTROLLER_HVAC
  
  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_DHT
  // #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_DS18B20

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"D6\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      "\"D1\":\"" D_GPIO_FUNCTION_REL2_INV_CTR  "\","
      "\"D2\":\"" D_GPIO_FUNCTION_REL3_INV_CTR      "\","
      // "\"D6\":\"" D_GPIO_FUNCTION_REL3_CTR      "\","
      // "\"D5\":\"" D_GPIO_FUNCTION_REL4_INV_CTR  "\","
      "\"D0\":\"" D_GPIO_FUNCTION_DHT22_1_CTR   "\","
      "\"D7\":\"" D_GPIO_FUNCTION_DHT22_2_CTR   "\","

      // "\"D5\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      // "\"D3\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\""
      
      // "\"LBI\":\"" D_GPIO_FUNCTION_LED1_CTR   "\""
      
      "\"D5\":\""  D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      "\"D3\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define D_DEVICE_DRIVER_RELAY_0_NAME "Room"
  #define D_DEVICE_DRIVER_RELAY_1_NAME "Desk"
  #define D_DEVICE_DRIVER_RELAY_2_NAME "Test1"
  #define D_DEVICE_DRIVER_RELAY_3_NAME "Test2"

  #define D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "Room"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "Desk"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "Test1"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE3_NAME "Test2"

  #define D_DEVICE_SENSOR_DHT_0_NAME "Room_DHT"
  #define D_DEVICE_SENSOR_DHT_1_NAME "Desk_DHT"

  #define D_DEVICE_SENSOR_DB18S20_0_NAME        "Room_DB18S20"
  #define D_DEVICE_SENSOR_DB18S20_0_ADDRESS     "[40,255,100,29,194,124,254,111]"
  #define D_DEVICE_SENSOR_DB18S20_1_NAME        "Desk_DB18S20"
  #define D_DEVICE_SENSOR_DB18S20_1_ADDRESS     "[40,255,100,29,194,102,202,187]"
  #define D_DEVICE_SENSOR_DB18S20_2_NAME        "Boiler_Pipe"
  #define D_DEVICE_SENSOR_DB18S20_2_ADDRESS     "[40,255,100,29,195,135,126,242]"
  #define D_DEVICE_SENSOR_DB18S20_3_NAME        "Immersion_Heater"
  #define D_DEVICE_SENSOR_DB18S20_3_ADDRESS     "[40,255,100,29,195,135,215,193]"
  #define D_DEVICE_SENSOR_DB18S20_4_NAME        "Tank_Top"
  #define D_DEVICE_SENSOR_DB18S20_4_ADDRESS     "[40,255,100,29,205,202,237,231]"
  #define D_DEVICE_SENSOR_DB18S20_5_NAME        "Tank_Middle"
  #define D_DEVICE_SENSOR_DB18S20_5_ADDRESS     "[40,255,100,29,205,206,170,25]"

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
        "\"" D_DEVICE_SENSOR_DB18S20_5_NAME "\""
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
        D_DEVICE_SENSOR_DB18S20_5_ADDRESS ""
      "]"  
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DHT_1_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_1_NAME "\""
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


#ifdef DEVICE_HEATING_ESP32_TESTER
  #define DEVICENAME_CTR          "heating_esp32_tester"
  #define DEVICENAME_FRIENDLY_CTR "HVAC Heating Gen 2"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1 //maintain other settings (bootcount)
   
  // #define ENABLE_BUG_TRACING
  //#define ENABLE_MQTT_DEBUG_MESSAGES
  #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  #define EMABLE_DEVFEATURE_HARDWAREPINS_CLEANED_UP
  #define ENABLE_DEBUG_DELAYS

  #define USE_INTERNAL_HOME_APS_ONLY

  #define DISABLE_WEBSERVER
  #define ESP32

  // #define USE_MODULE_CONTROLLER_HVAC
  
  #define USE_MODULE_SENSORS_INTERFACE  
  // #define USE_MODULE_SENSORS_DHT
  #define USE_MODULE_SENSORS_DS18B20

  #define ENABLE_DEVFEATURE_ESP32_FORCED_DB18S20_GPIO1_SENSOR_COUNT 1
  #define ENABLE_DEVFEATURE_ESP32_FORCED_DB18S20_GPIO2_SENSOR_COUNT 1

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      // "\"13\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      // "\"12\":\"" D_GPIO_FUNCTION_REL2_INV_CTR  "\","
      // "\"14\":\"" D_GPIO_FUNCTION_REL3_CTR      "\","
      // "\"27\":\"" D_GPIO_FUNCTION_REL4_INV_CTR  "\","
      // "\"25\":\"" D_GPIO_FUNCTION_DHT22_1_CTR   "\","
      // "\"26\":\"" D_GPIO_FUNCTION_DHT22_2_CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      "\"23\":\""  D_GPIO_FUNCTION_DS18X20_2_CTR "\","
      // "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      // "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
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

#ifdef DEVICE_RGBDESK
  #define DEVICENAME_CTR            "rgbdesk"
  #define DEVICENAME_FRIENDLY_CTR   "RGB Under Desk"

  // #define FORCE_TEMPLATE_LOADING
   
  // #define USE_MODULE_SENSORS_MOTION


  // #define USE_MODULE_NETWORK_WEBSERVER
  #define DISABLE_WEBSERVER

  // #define USE_MODULE_DRIVERS_FILESYSTEM
  //#define USE_MODULE_DRIVERS_FILESYSTEM

  // #define USE_MODULE_CORE_UPDATES

  #define USE_MODULE_SENSORS_BUTTONS
  // #define ENABLE_DEVFEATURE_PIN_FUNCTION_METHOD

  // #define ENABLE_SETTINGS_STORAGE
  // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
  
  // #define USE_DEBUG_PRINT_FUNCTION_NAME_TEST

  // #define ENABLE_DEVFEATURE_RTC_TIME_V2_MQTT_DEBUG

  // #define USE_MODULE_DRIVERS_LEDS

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
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"D0\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\"," // Test, nodemcu might allow this button after boot, I will use this to toggle on/off leds
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D7\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR     "\","
      #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      "\"D4\":\"" D_GPIO_FUNCTION_LED1_INV_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define STRIP_SIZE_MAX 60
  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":\"" D_EFFECTS_FUNCTION_SOLID_COLOUR_NAME_CTR "\""
    "},"
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":1000,"
      "\"" D_JSON_RATE_MS "\":1000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
    "},"
    "\"" D_JSON_CCT_TEMP "\":300,"
    "\"" D_JSON_HUE "\":25,"
    "\"" D_JSON_SAT "\":100,"
    "\"" D_JSON_COLOUR_PALETTE "\":\"RGBCCTColour 00\","
    "\"" D_JSON_BRIGHTNESS_CCT "\":100,"
    "\"" D_JSON_BRIGHTNESS_RGB "\":100"
  "}";
  
  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Radar"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif



#ifdef DEVICE_RGBMICRO1
  #define DEVICENAME_CTR          "rgbmicro1"
  #define DEVICENAME_FRIENDLY_CTR "Glass Box Lights"
  
  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 11
   
  // #define ENABLE_PIXEL_FUNCTION_EFFECTS
  // #define DISABLE_WEBSERVER

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
    "\"ColourPalette\":19," //c12    43 is the colours for this christmas
    "\"BrightnessRGB\":100"
  "}";

#endif

#ifdef DEVICE_RGBMICRO2 //projector
  #define DEVICENAME_CTR          "rgbmicro2"
  #define DEVICENAME_FRIENDLY_CTR "Projector Micro Lights"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1//random(1,1000)

  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  // #define ENABLE_DEVFEATURE_ARDUINOJSON

  // #define ENABLE_DEVFEATURE_BREAK_ADDLOG

  // #define ENABLE_PIXEL_FUNCTION_EFFECTS

//  #define USE_PUBSUB_V1

  //#define ENABLE_DEVFEATURE_MQTT_CONNECTION_EDIT1

  // #define ENABLE_BUG_TRACING

  // #define DISABLE_WEBSERVER

  //#define ENABLE_LOG_FILTERING_TEST_ONLY

  #define STRIP_SIZE_MAX 50//*15
  //#define ENABLE_PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL_CCT_SPACE
  //#define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  // #define USE_INTERFACE_NEW

  // #define USE_MODULE_ENERGY_INA219
  // #define USE_MODULE_SENSORS_BME

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR

  #define ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL

  //#define ENABLE_DEVFEATURE_LIGHTING_//Scene_OBJECT_TO_STRUCT "v78.24.11+" //only remove when all device exceed this
  //#define ENABLE_DEVFEATURE_RGBCOLOR_DESIRED
  //#define ENABLE_DEVFEATURE_SINGLE_ANIMATOR_INTERFACE   "v79.31.22+"   
  #define ENABLE_DEVFEATURE_PIXEL_LIVEVIEW_IN_PAGE_ROW    "v79.31.22+" 
  #define ENABLE_DEVFEATURE_PIXEL_NEW_JSON_PALETTE_SELECT_ROOT

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
//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE)   
//  "{\"Na\":\"rgb\",\"FNa\":\"fname\",\"GC\":{\"pin1\":\"func1\",\"pin2\":\"func2\"}}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" D_JSON_WS2812 "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"" D_PIXEL_HARDWARE_COLOR_ORDER_RGB_CTR "\","
    "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_COLOUR_PALETTE   "\":43,"
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_JSON_//Scene "\","
    "\"" D_JSON_//Scene_COLOUR     "\":{\"" D_JSON_HSB "\":[120,100,0]" "}," //this set the brightness
    "\"" D_JSON_BRIGHTNESS       "\":0,"
    "\"" D_JSON_BRIGHTNESS_RGB          "\":0"
  "}";

  #define D_DEVICE_SENSOR_CURRENT "Module 1"
  
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


#ifdef DEVICE_RGBMICRO3 //room speaker test
  #define DEVICENAME_CTR          "rgbmicro3"
  #define DEVICENAME_FRIENDLY_CTR "Micro Lights 3"  
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

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
      "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  
  #define STRIP_SIZE_MAX 100

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":9000,"
      "\"" D_JSON_RATE_MS "\":30000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":\"" "Slow Glow" "\""
    "},"
    "\"ColourPalette\":\"Christmas MultiColoured Warmer\","
    "\"BrightnessRGB\":100"
  "}";

#endif


#ifdef DEVICE_RGBNOTIFICATION_01
  #define DEVICENAME_CTR          "rgbnotification_01"
  #define DEVICENAME_FRIENDLY_CTR "RGB Notification 01"  
  
  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 21

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  // #define USE_MODULE_LIGHTS_WLED_EFFECTS
  // #define WLED_DEFINE_GLOBAL_VARS //only in one source file, wled.cpp!
  // #define DISABLE_PIXEL_FUNCTION_EFFECTS
  #define USE_MODULE_DRIVERS_LEDS
  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  #define USE_TASK_RGBLIGHTING_NOTIFICATIONS
  #define STRIP_NOTIFICATION_SIZE 12

  #define DISABLE_WEBSERVER
  
  //#define USE_WEBSERVER_ADVANCED_MULTIPAGES // new develop option to limit scope to only include root page while testing
  
  // #define USE_MODULE_SENSORS_SWITCHES
  // #define USE_MODULE_SENSORS_ANALOG

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
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":10000,"
      "\"" D_JSON_RATE_MS "\":1000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\""
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_NOTIFICATIONS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":\"" "Slow Glow" "\""
    "},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"Christmas MultiColoured Warmer\","
    "\"BrightnessRGB\":100"
  "}";

#endif



#ifdef DEVICE_RGBFIREPLACE_TESTER
  #define DEVICENAME_CTR          "rgbfireplace_tester"
  #define DEVICENAME_FRIENDLY_CTR "RGB Fire Place 32"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE

  // #define USE_MODULE_LIGHTS_WLED_EFFECTS
  // #define WLED_DEFINE_GLOBAL_VARS //only in one source file, wled.cpp!
  // #define DISABLE_PIXEL_FUNCTION_EFFECTS
  // #define USE_MODULE_DRIVERS_LEDS
  // #define DISABLE_WEBSERVER 

  #define ENABLE_DEVFEATURE_DEBUG_GET_PIXEL_ZERO
  #define ENABLE_DEVFEATURE_DIRECT_TEMPFIX_RANDOMISE_BRIGHTNESS_ON_PALETTE_GET
  #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  #define EMABLE_DEVFEATURE_HARDWAREPINS_CLEANED_UP
  // #define ENABLE_DEVFEATURE_BASIC_NEOPIXEL_TEST

  //   //#define USE_WEBSERVER_ADVANCED_MULTIPAGES // new develop option to limit scope to only include root page while testing

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"23\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define STRIP_SIZE_MAX 48//100//256
  // #define STRIP_REPEAT_OUTPUT_MAX 256
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":2000,"
      "\"" D_JSON_RATE_MS "\":5000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":10,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      // "\"" D_JSON_FUNCTION "\":\"" "FirePlace01" "\""
      "\"" D_JSON_FUNCTION "\":\"" "Slow Glow" "\""
    "},"
    // "\"ColourPalette\":\"Single Fire 01\","
    "\"" D_JSON_COLOUR_PALETTE "\":\"Christmas MultiColoured Warmer\","
    "\"BrightnessRGB\":20"
  "}";

// "{HardwareType":"WS28XX","Transition":{"TimeMs":400,"RateMs":100,"PixelUpdatePerc":10,"Order":"Random"},"AnimationMode":"Effects","Effects":{"Function":"FirePlace01"},"ColourPalette":"Single Fire 01","BrightnessRGB":5}

#endif


#ifdef DEVICE_RGBESP32_1_TESTER
  #define DEVICENAME_CTR          "rgbesp32_1_tester"
  #define DEVICENAME_FRIENDLY_CTR "RGB Fire Place 32"

  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 21

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  // #define USE_MODULE_LIGHTS_WLED_EFFECTS
  // #define WLED_DEFINE_GLOBAL_VARS //only in one source file, wled.cpp!
  // #define DISABLE_PIXEL_FUNCTION_EFFECTS
  // #define USE_MODULE_DRIVERS_LEDS
  // #define DISABLE_WEBSERVER 

  // #define ENABLE_DEVFEATURE_FLICKER_TESTING
  
  // #define DISABLE_NETWORK

  // #define ENABLE_DEVFEATURE_FLICKER_TESTING
  // #define ENABLE_DEVFEATURE_FLICKER_TESTING2
  
//   //#define USE_WEBSERVER_ADVANCED_MULTIPAGES // new develop option to limit scope to only include root page while testing
  
//   #define USE_MODULE_SENSORS_SWITCHES
//   #define USE_MODULE_SENSORS_ANALOG

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

  
  #define STRIP_SIZE_MAX 50//100//256
  // #define STRIP_REPEAT_OUTPUT_MAX 256
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
      // "\"" D_JSON_TIME_MS "\":300,"
      // "\"" D_JSON_RATE_MS "\":100,"
      "\"" D_JSON_TIME_MS "\":4000,"
      "\"" D_JSON_RATE_MS "\":1000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":10,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      // "\"" D_JSON_FUNCTION "\":\"" "FirePlace01" "\""
      // "\"" D_JSON_FUNCTION "\":"  "1"
      "\"" D_JSON_FUNCTION "\":\"" "Slow Glow" "\""
    "},"
    "\"ColourPalette\":\"Christmas MultiColoured Warmer\","
    // "\"ColourPalette\":\"Single Fire 01\","
    "\"BrightnessRGB\":100"
    // "\"Brightness\":1"

    

    // command to enable random brightness from palette on generation
    // command to set the max and min of those
    // disable by defaultThanks
  "}";

#endif



#ifdef DEVICE_RGBMICRO4 //gazebo
  #define DEVICENAME_CTR          "rgbmicro4"
  #define DEVICENAME_FRIENDLY_CTR "RGB Micro LED Lights 4"

  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 21

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  // #define USE_MODULE_LIGHTS_WLED_EFFECTS
  // #define WLED_DEFINE_GLOBAL_VARS //only in one source file, wled.cpp!
  // #define DISABLE_PIXEL_FUNCTION_EFFECTS
  // #define USE_MODULE_DRIVERS_LEDS
  #define DISABLE_WEBSERVER
  
//   //#define USE_WEBSERVER_ADVANCED_MULTIPAGES // new develop option to limit scope to only include root page while testing
  
//   #define USE_MODULE_SENSORS_SWITCHES
//   #define USE_MODULE_SENSORS_ANALOG

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

  
  #define STRIP_SIZE_MAX 256

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
    "\"BrightnessRGB\":10"
  "}";

#endif


//rgbmicro2/set/light///Scene
//{"//SceneName":"COLOUR//Scene","hue":25,"sat":100,"brt_rgb":100,"cct_temp":500,"brt_cct":100,"Time":0,"time_on":3600}
#ifdef DEVICE_RGBOUTSIDETREE

//keep this, as esp32 test device 
  #define DEVICENAME_CTR          "rgboutsidetree"
  #define DEVICENAME_FRIENDLY_CTR "Outside Tree"

  //#define ESP32

  /*
  
  use pin held low to disable wifi if possible, so I can test wifi things
  make it if low, enable wifi

at minimum, I can swap two esp32w around, one with and without wifi, as they wont cause neetwork name issues


add new function, that sets output via progmem stored directly


add function that enables ,qtt control per pixel, which uses setpixel (no naimator) to control ouptu
use to work out rows, but also write jay and evans names, then sore htem in progmem


using adc and ldr, enable "sweep" that lights each pixel until it stops on the one the ldr is against, and have that pixel count print over serial every second
have methods : write one, and stop on ldr
with mqqt flag, write then light all subsequent pixels found
with another, deductive method, turn off pixels found

in all cases, in array form, print the pixels lit over serial every seround and over mqtt!!

use car rad test push buttons, try adding ldr to it
use black etherhetn czble in dads rooms for it
  
  */

  #define ENABLE_GAMMA_BRIGHTNESS_ON_DESIRED_COLOUR_GENERATION

  #define USE_PM_OUTSIDE_TREE_MIXER_DESCRIPTION
  DEFINE_PGM_CTR(PM_OUTSIDE_TREE_MIXER_DESCRIPTION)  
  {  // movement, order, flasher, colours,   rate/time, mutlipler, (One look explanation for design)
    // Single basic
    "%d, Stat, InOrdr, SlowGlow, C 09 (basic), Single, %d/%d, %d \n\r(Simple Static)" "|" // 1
    "%d, Medm, Random, SlowGlow, C 09 (basic), Single, %d/%d, %d, \n\r(Random Blending with same colour)" "|" // 2
    "%d, Fast, Random, SlowGlow, C 10 (many),  Single, %d/%d, %d, \n\r(Fast random instant switching, more colours)" "|" // 3
    // Rows
    "%d, Slow, Random, SlowGlow, C 11 (many),  Rows, %d/%d, %d, \n\r(Rows, slow random blending)" "|" // 4
    "%d, Fast, Random, SlowGlow, C 11 (many),  Rows, %d/%d, %d, \n\r(Rows, fast random blending)" "|" // 5
    "%d, Stat, InOrdr, SlowGlow, C 09 (basic), Rows, %d/%d, %d, \n\r(Rows, Static InOrder)" "|" // 6  
    "%d, Slow, InOrdr, Sequentl, C 09 (basic), Rows, %d/%d, %d, \n\r(Rows, Now rotating)" "|" // 7
    // Berry green
    // "%d, Stat, InOrdr, SlowGlow, Berry Green,  Rows, %d/%d, %d, \n\r(Rows, Berry green static)" "|" // 8
    // "%d, Stat, InOrdr, SlowGlow, Berry Green,  Single, %d/%d, %d, \n\r(Red every 4th pixel)" "|" // 9
    // "%d, Fast, InOrdr, TwinkleC, C 10 (many),  Single, %d/%d, %d, \n\r(Simple Static)" "|" // 10
    // Twinkle
    "%d, Slow, Random, SlowGlow, C 10 (many),  Single, %d/%d, %d, \n\r(Simple Static)" "|" // 11
    // Step through palette
    "%d, Slow, Random, SlowGlow, C 13 (g adj), Single, %d/%d, %d, \n\r(2 colour stepping)" "|" // 12
    "%d, Slow, Random, SlowGlow, C 12 (hall),  Single, %d/%d, %d, \n\r(2 colour stepping)" "|" // 13
    // Random single, with dark colours as most
    "%d, Slow, Random, SlowGlow, C 14 (most dark), Single, %d/%d, %d, \n\r(Single, fast random)" "|" // 14
    // Returning to normal, but new hallway tree colours
    "%d, Slow, Random, SlowGlow, C 12 (Hallway), Single, %d/%d, %d, \n\r(Simple Static)" "|" // 15
  };


  #define ENABLE_DEVFEATURE_SETPIXELOUTPUT_VARIABLE
  #define USE_DEVFEATURE_PIXEL_OUTPUT_MULTIPLIER 2

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  // #define ENABLE_BUG_TRACING
  // #define DEBUG_PRINT_FUNCTION_NAME

//  #define ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL


  //#define ENABLE_LOG_FILTERING_TEST_ONLY
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  // #define USE_DEBUG_PRINT_FUNCTION_NAME_TEST
  #define DISABLE_WEBSERVER 
  
  #define ENABLE_PIXEL_FUNCTION_MIXER
  #define ENABLE_PIXEL_FUNCTION_EFFECTS
  //#define ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL

  #define STRIP_SIZE_MAX 1050// 750   *15 //changing gazebo to be 12v

  #define DISABLE_NETWORK
  // #define DISABLE_FOR_FAULTY_ESP32_FLICKERING

  #define DISABLE_SLEEP

  // #define USE_WS28XX_METHOD_RMT0_800KBPS_ESP32
    
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR

  // #define ENABLE_DEVFEATURE_LIGHTING_//Scene_OBJECT_TO_STRUCT "v78.24.11+" //only remove when all device exceed this
  // #define ENABLE_DEVFEATURE_RGBCOLOR_DESIRED
  //#define ENABLE_DEVFEATURE_SINGLE_ANIMATOR_INTERFACE   "v79.31.22+"   

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE)   
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR   "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" D_JSON_WS2812 "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","
    "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_COLOUR_PALETTE   "\":43,"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    // "\"" D_JSON_//Scene_COLOUR     "\":{\"" D_JSON_HSB    "\":[15,95,0]},"
    // "\"" D_JSON_BRIGHTNESS       "\":0,"
    // "\"" D_JSON_BRIGHTNESS_RGB          "\":0,"

    // new commands I want to run during boot

    "\"AnimationMode\":\"Flasher\","
    // "\"Flasher\":{"
    //   "\"Function\":11,"
    //   "\"AgedColouring\":0"
    // "},"
    "\"Mixer\":{"
      "\"Enabled\":1,"
      // "\"" D_JSON_RUNNING_ID "\":7,"
      "\"" D_JSON_TIME_SCALER_AS_PERCENTAGE "\":100"
    "},"
    "\"PixelGrouping\":{"
      // "\"Enabled\":\"Off\","
      "\"Mode\":3"
    "},"
    "\"Transition\":{\"Order\":\"Random\",\"PixelUpdatePerc\":100},"
    "\"BrightnessRGB\":20"

  "}";


#endif


//rgbmicro2/set/light///Scene
//{"//SceneName":"COLOUR//Scene","hue":25,"sat":100,"brt_rgb":100,"cct_temp":500,"brt_cct":100,"Time":0,"time_on":3600}
#ifdef DEVICE_RGBCLOCK_02
  #define DEVICENAME_CTR          "rgbclock_02"
  #define DEVICENAME_FRIENDLY_CTR "RGB Clock 02"
 
  #define FORCE_TEMPLATE_LOADING
  // #define SETTINGS_HOLDER 1

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  //#define USE_TASK_RGBLIGHTING_NOTIFICATIONS  // BREAKS
  #define USE_SK6812_METHOD_DEFAULT

  #define ENABLE_DEVFEATURE_RGB_CLOCK

  // #define ENABLE_GAMMA_BRIGHTNESS_ON_DESIRED_COLOUR_GENERATION
  // // #define ENABLE_DEVFEATURE_SETPIXELOUTPUT_VARIABLE
  // // #define USE_DEVFEATURE_PIXEL_OUTPUT_MULTIPLIER 2

  // // #define ENABLE_BUG_TRACING
  // // #define DEBUG_PRINT_FUNCTION_NAME
  // //  #define ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
  // //#define ENABLE_LOG_FILTERING_TEST_ONLY
  // #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  // // #define USE_DEBUG_PRINT_FUNCTION_NAME_TEST
  // #define DISABLE_WEBSERVER 
  
  // #define ENABLE_PIXEL_FUNCTION_MIXER
  // #define ENABLE_PIXEL_FUNCTION_EFFECTS
  // //#define ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL

  #define STRIP_SIZE_MAX 93// 750   *15 //changing gazebo to be 12v

  // #define DISABLE_NETWORK
  // // #define DISABLE_FOR_FAULTY_ESP32_FLICKERING

  // #define DISABLE_SLEEP
    
  #define DISABLE_WEBSERVER

  // #define USE_MODULE_SENSORS_BME
  
  // #define USE_MODULE_SENSORS_MOTION


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE)   
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR   "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    // "\"" D_JSON_HARDWARE_TYPE    "\":\"" D_JSON_WS2812 "\","
    // #ifdef STRIP_SIZE_MAX
    // "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    // #else
    // "\"" D_JSON_STRIP_SIZE       "\":50,"
    // #endif //STRIP_SIZE_MAX
    // "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","
  

    // "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    // "\"" D_JSON_COLOUR_PALETTE   "\":43,"
    // "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    // // "\"" D_JSON_//Scene_COLOUR     "\":{\"" D_JSON_HSB    "\":[15,95,0]},"
    // // "\"" D_JSON_BRIGHTNESS       "\":0,"
    // // "\"" D_JSON_BRIGHTNESS_RGB          "\":0,"

    // // new commands I want to run during boot

    // "\"AnimationMode\":\"Flasher\","
    // // "\"Flasher\":{"
    // //   "\"Function\":11,"
    // //   "\"AgedColouring\":0"
    // // "},"
    // "\"Mixer\":{"
    //   "\"Enabled\":1,"
    //   // "\"" D_JSON_RUNNING_ID "\":7,"
    //   "\"" D_JSON_TIME_SCALER_AS_PERCENTAGE "\":100"
    // "},"
    // "\"PixelGrouping\":{"
    //   // "\"Enabled\":\"Off\","
    //   "\"Mode\":3"
    // "},"
    // "\"Transition\":{\"Order\":\"Random\",\"PixelUpdatePerc\":100},"
    // "\"BrightnessRGB\":20"

    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRBw\","
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":0,"
      "\"" D_JSON_RATE_MS "\":1000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\""
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":29"
    "},"
    // "\"CCT_Temp\": 152,"
    // "\"Hue\":25,"
    // "\"Sat\":100,"
    "\"ColourPalette\":41,"
    "\"BrightnessCCT\":10,"
    "\"BrightnessRGB\":100"

  "}";


#endif


#ifdef DEVICE_RGBDISPLAY_GARAGE
  #define DEVICENAME_CTR          "rgbdisplay_garage"
  #define DEVICENAME_FRIENDLY_CTR "RGB Display Garage"
 
  #define FORCE_TEMPLATE_LOADING
  // #define SETTINGS_HOLDER 1

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  // #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  // #define USE_SK6812_METHOD_DEFAULT

  #define ENABLE_DEVFEATURE_RGB_CLOCK

  #define STRIP_SIZE_MAX 93// 750   *15 //changing gazebo to be 12
    
  #define DISABLE_WEBSERVER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE)   
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR   "\""
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
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRBw\","
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":0,"
      "\"" D_JSON_RATE_MS "\":1000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\""
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":29"
    "},"
    // "\"CCT_Temp\": 152,"
    // "\"Hue\":25,"
    // "\"Sat\":100,"
    "\"ColourPalette\":41,"
    "\"BrightnessCCT\":10,"
    "\"BrightnessRGB\":100"

  "}";


#endif


//rgbmicro2/set/light///Scene
//{"//SceneName":"COLOUR//Scene","hue":25,"sat":100,"brt_rgb":100,"cct_temp":500,"brt_cct":100,"Time":0,"time_on":3600}
#ifdef DEVICE_RGBOUTSIDETREE_TESTER
  #define DEVICENAME_CTR          "rgboutsidetree_tester"
  #define DEVICENAME_FRIENDLY_CTR "Outside Tree Tester"

// #define ENABLE_BUG_TRACING
// #define ENABLE_DEVFEATURE_FLICKER_TESTING
// #define ENABLE_DEVFEATURE_FLICKERING_TEST2
// #define ENABLE_DEVFEATURE_FLICKERING_TEST3
#define ENABLE_DEVFEATURE_FLICKERING_TEST5


/*


  #ifdef ENABLE_DEVFEATURE_FLICKER_TESTING

  #endif // ENABLE_DEVFEATURE_FLICKER_TESTING

  */

#define USE_PM_OUTSIDE_TREE_MIXER_DESCRIPTION
DEFINE_PGM_CTR(PM_OUTSIDE_TREE_MIXER_DESCRIPTION)
{  // trans method, flasher, colours,   rate/time, mutlipler
  "Static/InOrder, SlowGlow, Few Colour, %d/%d, Single" "|" // 1
  "Medium/Random, SlowGlow, Few Colour, %d/%d, Single" "|" // 2
  "Fast/Random, SlowGlow, Many Colour 10, %d/%d, Single" "|" // 3
  "Slow/Random, SlowGlow, Many Colour 10, %d/%d, Rows" "|" // 4
  "Fast/Random, SlowGlow, Many Colour 10, %d/%d, Rows" "|" // 5
  "Static/InOrder, SlowGlow, Few Colour 9, %d/%d, Rows" "|" // 6
  "Rotate/InOrder, Sequential, Many Colour 10, %d/%d, Rows" "|" // 7
  "Static/InOrder, SlowGlow, Berry Green, %d/%d, Rows" "|" // 8
  "Static/InOrder, SlowGlow, Berry Green, %d/%d, Single" "|" // 9
  "Fast/InOrder, TwinkleColour, Many Colour 10, %d/%d, Single" "|" // 10
  "Slow/Random, SlowGlow, Partial Colours 5, %d/%d, Single" "|" // 11
};



#define ENABLE_DEVFEATURE_SETPIXELOUTPUT_VARIABLE
#define USE_DEVFEATURE_PIXEL_OUTPUT_MULTIPLIER 2


  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  // #define ENABLE_BUG_TRACING
  #define DEBUG_PRINT_FUNCTION_NAME


  //#define ENABLE_LOG_FILTERING_TEST_ONLY
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  #define USE_DEBUG_PRINT_FUNCTION_NAME_TEST
  
  #define ENABLE_PIXEL_FUNCTION_MIXER
  #define ENABLE_PIXEL_FUNCTION_EFFECTS

  #define STRIP_SIZE_MAX 1000// 750   *15 //changing gazebo to be 12v

  #define DISABLE_WEBSERVER
  #define DISABLE_NETWORK
  // #define DISABLE_FOR_FAULTY_ESP32_FLICKERING


  // Memory reduction techniques
  // #define DEVICENAMEBUFFER_NAME_BUFFER_LENGTH 10 
  // #define WEB_LOG_SIZE 1
  // #define LOG_BUFFER_SIZE 500

  // #define DISABLE_SLEEP

  // #define USE_WS28XX_METHOD_RMT0_800KBPS_ESP32

  // #define STRIP_REPEAT_OUTPUT_MAX STRIP_SIZE_MAX
  //#define ENABLE_PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL_CCT_SPACE
  //#define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION



  #define USE_INTERFACE_NEW
    
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR

  #define DISBALE_TEST_SECTION

  // #define ENABLE_DEVFEATURE_LIGHTING_//Scene_OBJECT_TO_STRUCT "v78.24.11+" //only remove when all device exceed this
  // #define ENABLE_DEVFEATURE_RGBCOLOR_DESIRED
  //#define ENABLE_DEVFEATURE_SINGLE_ANIMATOR_INTERFACE   "v79.31.22+"   

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE)   
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR   "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" D_JSON_WS2812 "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
    "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_COLOUR_PALETTE   "\":\"User 00\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_//Scene_COLOUR     "\":{\"" D_JSON_HSB    "\":[15,95,0]},"
    // "\"" D_JSON_BRIGHTNESS       "\":0,"
    // "\"" D_JSON_BRIGHTNESS_RGB          "\":0,"

    // new commands I want to run during boot

    "\"AnimationMode\":\"Flasher\","
    // "\"Flasher\":{"
    //   "\"Function\":11,"
    //   "\"AgedColouring\":0"
    // "},"
    "\"Mixer\":{"
      "\"Enabled\":1,"
      // "\"" D_JSON_RUNNING_ID "\":7,"
      "\"" D_JSON_TIME_SCALER_AS_PERCENTAGE "\":10"
    "},"
    "\"PixelGrouping\":{"
      // "\"Enabled\":\"Off\","
      "\"Mode\":3"
    "},"
    "\"Transition\":{\"Order\":\"Random\",\"PixelUpdatePerc\":100},"
    "\"BrightnessRGB\":100"

  "}";

#endif


#ifdef DEVICE_RGBDININGROOMTREE 
  #define DEVICENAME_CTR          "rgbdiningroomtree"
  #define DEVICENAME_FRIENDLY_CTR "RGB Dining Room Tree"

 #define ENABLE_DEVFEATURE_SETPIXELOUTPUT_VARIABLE
  #define USE_DEVFEATURE_PIXEL_OUTPUT_MULTIPLIER 2

  #define ENABLE_GAMMA_BRIGHTNESS_ON_DESIRED_COLOUR_GENERATION

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  // #define ENABLE_BUG_TRACING
  #define DEBUG_PRINT_FUNCTION_NAME

  //#define ENABLE_LOG_FILTERING_TEST_ONLY
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  #define USE_DEBUG_PRINT_FUNCTION_NAME_TEST
  
  // #define ENABLE_PIXEL_FUNCTION_MIXER
  #define ENABLE_PIXEL_FUNCTION_EFFECTS

  #define STRIP_SIZE_MAX 150

  #define DISABLE_WEBSERVER
  // #define DISABLE_NETWORK
  // #define DISABLE_FOR_FAULTY_ESP32_FLICKERING

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  

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

   #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","
    // "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":1" //slow glow
    "},"
    "\"Transition\":{\"Order\":\"InOrder\",\"PixelUpdatePerc\":30,\"RateMs\":2000},"
    "\"TimeMs\":1000,"
    "\"ColourPalette\":43," //c12    43 is the colours for this christmas
    "\"BrightnessRGB\":100"
  "}";

#endif



//rgbmicro2/set/light///Scene
//{"//SceneName":"COLOUR//Scene","hue":25,"sat":100,"brt_rgb":100,"cct_temp":500,"brt_cct":100,"Time":0,"time_on":3600}
#ifdef DEVICE_RGBBACKDOORWREATH
  #define DEVICENAME_CTR          "rgbbackdoorwreath"
  #define DEVICENAME_FRIENDLY_CTR "RGB Backdoor Wreath"
  
  // #define ENABLE_DEVFEATURE_FLICKERING_TEST5

  // #define USE_PM_OUTSIDE_TREE_MIXER_DESCRIPTION
  // DEFINE_PGM_CTR(PM_OUTSIDE_TREE_MIXER_DESCRIPTION)
  // {  // trans method, flasher, colours,   rate/time, mutlipler
  //   "Static/InOrder, SlowGlow, Few Colour, %d/%d, Single" "|" // 1
  //   "Medium/Random, SlowGlow, Few Colour, %d/%d, Single" "|" // 2
  //   "Fast/Random, SlowGlow, Many Colour 10, %d/%d, Single" "|" // 3
  //   "Slow/Random, SlowGlow, Many Colour 10, %d/%d, Rows" "|" // 4
  //   "Fast/Random, SlowGlow, Many Colour 10, %d/%d, Rows" "|" // 5
  //   "Static/InOrder, SlowGlow, Few Colour 9, %d/%d, Rows" "|" // 6
  //   "Rotate/InOrder, Sequential, Many Colour 10, %d/%d, Rows" "|" // 7
  //   "Static/InOrder, SlowGlow, Berry Green, %d/%d, Rows" "|" // 8
  //   "Static/InOrder, SlowGlow, Berry Green, %d/%d, Single" "|" // 9
  //   "Fast/InOrder, TwinkleColour, Many Colour 10, %d/%d, Single" "|" // 10
  //   "Slow/Random, SlowGlow, Partial Colours 5, %d/%d, Single" "|" // 11
  // };

/*
RAM:   [======    ]  56.2% (used 46024 bytes from 81920 bytes)
Flash: [======    ]  56.9% (used 582400 bytes from 1023984 bytes)*/
  #define ENABLE_DEVFEATURE_SETPIXELOUTPUT_VARIABLE
  #define USE_DEVFEATURE_PIXEL_OUTPUT_MULTIPLIER 2


  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  // #define ENABLE_BUG_TRACING
  #define DEBUG_PRINT_FUNCTION_NAME


  //#define ENABLE_LOG_FILTERING_TEST_ONLY
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  #define USE_DEBUG_PRINT_FUNCTION_NAME_TEST
  
  // #define ENABLE_PIXEL_FUNCTION_MIXER
  #define ENABLE_PIXEL_FUNCTION_EFFECTS

  #define STRIP_SIZE_MAX 100

  // #define DISABLE_WEBSERVER
  // #define DISABLE_NETWORK
  // #define DISABLE_FOR_FAULTY_ESP32_FLICKERING

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE)   
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR   "\""
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
    "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_COLOUR_PALETTE   "\":\"User 00\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":1" //slow glow
    "},"
    "\"Transition\":{\"Order\":\"InOrder\",\"PixelUpdatePerc\":30,\"RateMs\":2000},"
    "\"TimeMs\":1000,"
    "\"ColourPalette\": 43,"
    "\"BrightnessRGB\":100"
  "}";


#endif






#ifdef DEVICE_H801_TESTER // for PWM dev
  #define DEVICENAME_CTR          "h801_tester"
  #define DEVICENAME_FRIENDLY_CTR "H801 Tester Strip"
    
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
    "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":0,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\"},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"Solid Rgbcct 01\","
    "\"" D_JSON_ANIMATIONMODE  "\":\"" D_JSON_EFFECTS "\","
    "\"" D_JSON_EFFECTS        "\"{\"Function\":8},"//\"Solid RGBCCT\"},"
    "\"" D_JSON_BRIGHTNESS     "\":100"
  "}";

#endif



#ifdef DEVICE_RGBBEDLIGHT
  #define DEVICENAME_CTR          "rgbbedlight"
  #define DEVICENAME_FRIENDLY_CTR "H801 BedLight"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1   

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_PWM

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_H801_CTR "\""
  "}";
  
  #define STRIP_SIZE_MAX 1 // PWM type, set size to 1
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "RGBCCT_PWM" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGBCW\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":\"" D_EFFECTS_FUNCTION_SOLID_COLOUR_NAME_CTR "\""
    "},"
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME "\":1,"
      "\"" D_JSON_RATE "\":5,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
    "},"
    "\"" D_JSON_CCT_TEMP "\":300,"
    "\"" D_JSON_HUE "\":25,"
    "\"" D_JSON_SAT "\":100,"
    "\"" D_JSON_COLOUR_PALETTE "\":\"RGBCCTColour 00\","
    "\"" D_JSON_BRIGHTNESS_CCT "\":100,"
    "\"" D_JSON_BRIGHTNESS_RGB "\":100"
  "}";
    
#endif


#ifdef DEVICE_RGBBEDROOMFLOOR
  #define DEVICENAME_CTR          "rgbbedroomfloor"
  #define DEVICENAME_FRIENDLY_CTR "H801 Bedroom Floor Light"
  
  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2 

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE
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
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"GRBcw\","
    "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"Solid Rgbcct 01\","
    "\"" D_JSON_ANIMATIONMODE  "\":\"" D_JSON_EFFECTS "\","
    "\"" D_JSON_EFFECTS        "\"{\"Function\":\"Solid RGBCCT\"},"
    "\"" D_JSON_BRIGHTNESS     "\":100"
  "}";
  
#endif



#ifdef DEVICE_RGBBEDLIGHT_TEST // for PWM dev
  #define DEVICENAME_CTR          "rgbbedlight_test"
  #define DEVICENAME_FRIENDLY_CTR "H801 BedLight TEST"
  
  // #define ENABLE_FUNCTION_DEBUG  

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2 //maintain other settings (bootcount)
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_PWM
  
  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define DISABLE_TEMPORARY_RGBANIMATOR
  // #define USE_MODULE_LIGHTS_PWM

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
      "\"" D_JSON_HARDWARE_TYPE  "\":\"" D_PIXEL_HARDWARE_TYPE_RGBCCT_PWM_CTR "\","   //"\":\"WS2812\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
      "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"GRBcw\","
      "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
      "\"" D_JSON_COLOUR_PALETTE "\":\"USER_17\","
      "\"" D_JSON_ANIMATIONMODE           "\":\"Flasher\","
      "\"" D_JSON_BRIGHTNESS     "\":99"
    "}";
    
    // #define ESP8266

#endif

#ifdef DEVICE_RGBCRYSTAL2
  #define DEVICENAME_CTR            "rgbcrystal2"
  #define DEVICENAME_FRIENDLY_CTR   "Crystal Light Round 2"
  
  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 10
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR

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

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    "{"
      "\"" D_JSON_HARDWARE_TYPE  "\":\"WS2812\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
      "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"GRB\","
      "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
      "\"" D_JSON_COLOUR_PALETTE "\":\"User 19\"," //purple/pink/red (at sunset orange?)
      "\"" D_JSON_ANIMATIONMODE           "\":\"" D_JSON_EFFECTS "\","
      "\"" D_JSON_BRIGHTNESS     "\":14"
    "}";

#endif


#ifdef DEVICE_TESTBUILDER
  #define DEVICENAME_CTR          "development_device"
  #define DEVICENAME_FRIENDLY_CTR "Development Device"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER random(1,10000)
  
  // #define ENABLE_BUG_TRACING

  // #define USE_MODULE_SENSORS_DOOR
  // #define USE_SENSOR_DOOR_LOCK
  // #define DOORALERT_PAYLOAD_CTR "bedroom"

  // #define USE_MODULE_CONTROLLER_SONOFF_IFAN
  // #define USE_MODULE_DRIVERS_IRTRANSCEIVER

  // #define USE_MODULE_SENSORS_DS18B20

  // #define USE_MODULE_SENSORS_MOTION
  // #define MOTIONALERT_PAYLOAD1_CTR "bedroom"
  
  // #define USE_MODULE_SENSORS_BME

  // #define USE_MODULE_SENSORS_BUTTONS

  #define USE_MODULE_DRIVERS_RELAY
  #define MAX_RELAYS 1
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"D0\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      #endif
      #ifdef USE_MODULE_SENSORS_BME
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DS18B20
      "\"D3\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      //"\"D6\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"D4\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      // "\"D0\":\"" D_GPIO_FUNCTION_REL2_INV_CTR  "\","
      #endif
      "\"D5\":\"" D_GPIO_FUNCTION_FAN_IRSEND_CTR    "\","
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D7\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR     "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DOOR
      "\"D6\":\"" D_GPIO_FUNCTION_DOOR_OPEN_CTR     "\","
      #endif
      #ifdef USE_MODULE_LIGHTS_INTERFACE
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      #endif
      "\"D0\":\"" D_GPIO_FUNCTION_LED1_INV_CTR   "\","    
      "\"D4\":\"" D_GPIO_FUNCTION_LED1_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE)                                                  //parsed via lighting interface
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\"WS2812\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_REPEAT   "\":5,"
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"GRB\"," //GRBW
    "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"USER_18\","
    "\"" D_JSON_ANIMATIONMODE           "\":\"" D_JSON_EFFECTS "\","
    "\"" D_JSON_BRIGHTNESS     "\":100"
  "}";
  // #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  // #define USE_TASK_RGBLIGHTING_NOTIFICATIONS   
  // #define STRIP_SIZE_MAX                      12   


  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Attic Socket"
  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    //device_names:{"module_name":["relay1","relay2"]}
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
          "\"" "Attic" "\""
        "],"
        "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";

#endif








#ifdef DEVICE_BEDROOMPANEL
  #define DEVICENAME_CTR            "bedroompanel"
  #define DEVICENAME_FRIENDLY_CTR   "Bedroom Control Panel"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
   
  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define USE_MODULE_LIGHTS_ADDRESSABLE
  // #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  // //#define USE_TASK_RGBLIGHTING_NOTIFICATIONS  // BREAKS
  // #define USE_SK6812_METHOD_DEFAULT

  
  #define USE_MODULE_DISPLAYS_NEXTION
  #define NEXTION_DEFAULT_PAGE_NUMBER 4 //heating page

  #define USE_BUILD_TYPE_DISPLAY
  
  #define DISABLE_WEBSERVER

  // #define USE_MODULE_SENSORS_BME
  
  // #define USE_MODULE_SENSORS_MOTION

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
      "\"D7\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR "\","
      #endif
      
      "\"14\":\""  D_GPIO_FUNCTION_NEXTION_TX_CTR "\"," //1
      "\"12\":\""  D_GPIO_FUNCTION_NEXTION_RX_CTR "\"," //3

      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


  // #define STRIP_SIZE_MAX 40
  
  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
  //   #ifdef STRIP_SIZE_MAX
  //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  //   #else
  //   "\"" D_JSON_STRIP_SIZE       "\":50,"
  //   #endif //STRIP_SIZE_MAX
  //   "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
  //   "\"" D_JSON_TRANSITION       "\":{"
  //     "\"" D_JSON_TIME_MS "\":10000,"
  //     "\"" D_JSON_RATE_MS "\":1000,"
  //     "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
  //     "\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\""
  //   "},"
  //   "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
  //   "\"" D_JSON_EFFECTS "\":{" 
  //     "\"" D_JSON_FUNCTION "\":\"" "Slow Glow" "\""
  //   "},"
  //   "\"ColourPalette\":\"Christmas MultiColoured Warmer\","
  //   "\"BrightnessRGB\":100"
  // "}";
  
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

//REMOVE LANDING PANEL, PARENTS CANT SEE IT!!
//REMOVE KITCHEN PANEL


#ifdef DEVICE_DESKPANEL
  #define DEVICENAME_CTR            "deskpanel"
  #define DEVICENAME_FRIENDLY_CTR   "Desk Alert Panel"

  // #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
   
  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define USE_MODULE_LIGHTS_ADDRESSABLE
  // #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  // //#define USE_TASK_RGBLIGHTING_NOTIFICATIONS  // BREAKS
  // #define USE_SK6812_METHOD_DEFAULT

  
  // #define USE_MODULE_DISPLAYS_NEXTION
  #define NEXTION_DEFAULT_PAGE_NUMBER 4 //heating page

  #define USE_BUILD_TYPE_DISPLAY
  
  #define DISABLE_WEBSERVER

  // #define USE_MODULE_SENSORS_BME
  
  // #define USE_MODULE_SENSORS_MOTION

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
      "\"D7\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR "\","
      #endif
      
      "\"14\":\""  D_GPIO_FUNCTION_NEXTION_TX_CTR "\"," //1
      "\"12\":\""  D_GPIO_FUNCTION_NEXTION_RX_CTR "\"," //3

      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


  // #define STRIP_SIZE_MAX 40
  
  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
  //   #ifdef STRIP_SIZE_MAX
  //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  //   #else
  //   "\"" D_JSON_STRIP_SIZE       "\":50,"
  //   #endif //STRIP_SIZE_MAX
  //   "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
  //   "\"" D_JSON_TRANSITION       "\":{"
  //     "\"" D_JSON_TIME_MS "\":10000,"
  //     "\"" D_JSON_RATE_MS "\":1000,"
  //     "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
  //     "\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\""
  //   "},"
  //   "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
  //   "\"" D_JSON_EFFECTS "\":{" 
  //     "\"" D_JSON_FUNCTION "\":\"" "Slow Glow" "\""
  //   "},"
  //   "\"ColourPalette\":\"Christmas MultiColoured Warmer\","
  //   "\"BrightnessRGB\":100"
  // "}";
  
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


#ifdef DEVICE_RGBCLOCK_01
  #define DEVICENAME_CTR            "rgbclock_01"
  #define DEVICENAME_FRIENDLY_CTR   "RGB Clock 0l"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  #define USE_SK6812_METHOD_DEFAULT

  #define ENABLE_DEVFEATURE_RGB_CLOCK
  #define STRIP_SIZE_MAX 93
  
  #define DISABLE_WEBSERVER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"LBI\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
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
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRBw\","
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":0,"
      "\"" D_JSON_RATE_MS "\":1000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\""
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":29"
    "},"
    // "\"CCT_Temp\": 152,"
    // "\"Hue\":25,"
    // "\"Sat\":100,"
    "\"ColourPalette\":41,"
    "\"BrightnessCCT\":10,"
    "\"BrightnessRGB\":100"
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




#ifdef DEVICE_TESTSENSOR
  #define DEVICENAME_CTR            "landingpanel"
  #define DEVICENAME_FRIENDLY_CTR   "Landing Control Panel"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2 //maintain other settings (bootcount)
   
  #define USE_MODULE_SENSORS_MOTION

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
        #ifdef USE_MODULE_SENSORS_MOTION
        "\"D7\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR     "\","
        #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Radar"
  
  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";

  



#endif


#ifdef DEVICE_KITCHENPANEL
  #define DEVICENAME_CTR          "kitchenpanel"
  #define DEVICENAME_FRIENDLY_CTR "Kitchen Panel"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2

  #define USE_MODULE_DISPLAYS_NEXTION
  #define NEXTION_DEFAULT_PAGE_NUMBER 4 //heating page

  #define USE_BUILD_TYPE_DISPLAY

  // #define ENABLE_BUG_TRACING
  
  // #define USE_SOFTWARE_SERIAL_DEBUG
  // #define DISABLE_SERIAL_LOGGING //temp measure

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"1\":\""  D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"3\":\""  D_GPIO_FUNCTION_NEXTION_RX_CTR "\"," 
      "\"D0\":\""  D_GPIO_FUNCTION_LED1_INV_CTR   "\","  
      "\"D4\":\""  D_GPIO_FUNCTION_LED1_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

#endif



#ifdef DEVICE_BEDROOMSENSOR_TESTER // the old device with everything, now being cut back to long term stable and essentials
  #define DEVICENAME_CTR          "bedroomsensor_tester"
  #define DEVICENAME_FRIENDLY_CTR "Bedroom Sensor Tester"

  /**
   * BME              D1/D2
   * DB18, 1 channel  D3
   * PIR Motion       D7
   * Doppler Motion   D6
   * Door Position    D4
   * Ultrasonic 
   * Analog Light Sensor A0
   */

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_MOTION
  #define USE_MODULE_SENSORS_BH1750
  #define USE_MODULE_SENSORS_SR04

  // Future, split notification light out of bedroom sensor, to allow continued future operation of sensor
  
  // #define ENABLE_BUG_TRACING
  // #define USE_MODULE_SENSORS_ANALOG
  
  // // #define ENABLE_DEVFEATURE_SERIAL_PRINT_LONG_LOOP_TASKERS
  // // #define ENABLE_DEVFEATURE_TESTING_LONG_LOOPS
  // // #define ENABLE_DEVFEATURE_RTC_TIME_V2
  
  // // #define USE_MODULE_SENSORS_SWITCHES

  // #define USE_MODULE_SENSORS_DOOR
  // // // #define USE_SENSOR_DOOR_LOCK
  // // #define DOORALERT_PAYLOAD_CTR "bedroom"

  // #define USE_MODULE_SENSORS_DS18B20
  // #define ENABLE_DEVFEATURE_DB18_TEMPLATE_CORRECTED_INDEXES

  

  // #define ENABLE_PIXEL_FUNCTION_EFFECTS
  // Test ultrasonic oilfurnace code
  // #define USE_MODULE_CONTROLLER_OILFURNACE
  // #define USE_MODULE_SENSORS_ULTRASONICS  
  // #define USE_AMBIENT_TEMP_SENSOR_FOR_SPEEDOFSOUND
  
  // #define USE_MODULE_SENSORS_BME

  // ir send D5

  // // #define USE_MODULE_SENSORS_BUTTONS

  // #define USE_MODULE_DRIVERS_RELAY
  // #define MAX_RELAYS 1
  // #define USE_MODULE_DRIVERS_INTERFACE
  
  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  /**
   *  [=====     ]  51.3% (used 41988 bytes from 81920 bytes)
   *  [=====     ]  47.7% (used 488592 bytes from 1023984 bytes)
   * 
      RAM:   [=====     ]  51.3% (used 41988 bytes from 81920 bytes)
      Flash: [=====     ]  47.7% (used 488272 bytes from 1023984 bytes)

      RAM:   [=====     ]  51.3% (used 41988 bytes from 81920 bytes)
      Flash: [=====     ]  47.6% (used 487476 bytes from 1023984 bytes)
   * */
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"    
      // #ifdef USE_MODULE_SENSORS_BUTTONS
      // "\"D0\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      // #endif
      // #ifdef USE_MODULE_SENSORS_ULTRASONICS
      "\"D1\":\"" D_GPIO_FUNCTION_SR04_ECHO_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_SR04_TRIG_CTR  "\","  
      // #endif  
      // #ifdef USE_MODULE_SENSORS_BME
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      // #endif
      // #ifdef USE_MODULE_SENSORS_DS18B20
      // "\"D3\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      // //"\"D6\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\","
      // #endif
      // #ifdef USE_MODULE_DRIVERS_RELAY
      // "\"D4\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      // // "\"D0\":\"" D_GPIO_FUNCTION_REL2_INV_CTR  "\","
      // #endif
      // "\"D5\":\"" D_GPIO_FUNCTION_FAN_IRSEND_CTR    "\","
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D7\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR     "\","
      // "\"D6\":\"" D_GPIO_FUNCTION_PIR_2_INV_CTR     "\","
      #endif
      // #ifdef USE_MODULE_SENSORS_DOOR
      // "\"D4\":\"" D_GPIO_FUNCTION_DOOR_OPEN_CTR     "\","
      // #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
      // add status led to here
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


//  #define USE_LIGHTING_TEMPLATE
//   DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   "{"
//     "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
//     #ifdef STRIP_SIZE_MAX
//     "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//     #else
//     "\"" D_JSON_STRIP_SIZE       "\":50,"
//     #endif //STRIP_SIZE_MAX
//     "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","
//     // "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
//     "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
//     "\"" D_JSON_EFFECTS "\":{" 
//       "\"Function\":1" //slow glow
//     "},"
//     "\"Transition\":{\"Order\":\"InOrder\",\"PixelUpdatePerc\":2,\"RateMs\":60000},"
//     "\"TimeMs\":30000,"
//     "\"ColourPalette\":43," //c12    43 is the colours for this christmas
//     "\"BrightnessRGB\":100"
//   "}";
//   #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
//   // #define USE_TASK_RGBLIGHTING_NOTIFICATIONS   
//   #define STRIP_SIZE_MAX                      12   

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "LED1"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "LED2"

// maybe I need to delay template name loading until after init phase?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" "bedroom" "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" "Bedroom" "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" "speed_of_sound_ambient" "\","
        "\"" "Bedroom02" "\","
        "\"" "Bedroom03" "\","
        "\"" "Bedroom04" "\","
        "\"" "Bedroom05" "\","
        "\"" "Bedroom06" "\","
        "\"" "Bedroom07" "\""
      "],"
      "\"" D_MODULE_SENSORS_DOOR_FRIENDLY_CTR "\":["
        "\"" "bedroomDOOR" "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["                                
        "[40,255,100,29,194,102,202,187],"     // D3 group of 2                                 
        "[40,255,100,29,195,135,215,193],"
        "[40,255,100,29,194,124,254,111],"   //D6 group of 3                                           
        "[40,255,100,29,195,134,175,63],"                                             
        "[40,255,100,29,195,135,126,242]"  

                            
        // "[40,0,32,23,59,71,5,141],"  
        // "[40,0,66,109,59,71,5,172],"  
        // "[40,0,118,128,59,71,5,227],"  
        // "[40,0,83,19,59,71,6,66],"  
        // "[40,0,114,20,59,71,5,19],"  
        // "[40,0,108,65,59,71,4,202],"  
        // "[40,0,40,61,59,71,4,134]"       

      "]"  
    "}"
  "}";

#endif





#ifdef DEVICE_SIDEDOORLIGHT_TEST
  #define DEVICENAME_CTR          "sidedoorlight_test"
  #define DEVICENAME_FRIENDLY_CTR "Side Door Motion Test"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 3

  //#define DISABLE_WEBSERVER
   
  #define USE_MODULE_SENSORS_MOTION

  #define ENABLE_DEVFEATURE_BLOCK_RESTART
  #define ENABLE_DEVFEATURE_ADVANCED_RELAY_CONTROLS
  // #define ENABLE_DEVFEATURE_RELAY_TIME_SCHEDULED_DEFAULT_ON
  
  #define USE_MODULE_CONTROLLER_SECURITY_LIGHT //disable until I rewrite for the sidelight

  #define USE_MODULE_DRIVERS_RELAY
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D1\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR     "\","
      #endif 
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"D2\":\"" D_GPIO_FUNCTION_REL1_CTR   "\"," //d2 normally
      #endif
      "\"D4\":\""  D_GPIO_FUNCTION_LED1_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Side Door"
  #define MAX_RELAYS 1
  
  #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Driveway Middle"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif





#ifdef DEVICE_SHELLY1_TESTER
  #define DEVICENAME_CTR          "shelly1_tester"
  #define DEVICENAME_FRIENDLY_CTR "Shelly1 Tester"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2
   
  #define USE_MODULE_SENSORS_MOTION
  #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Driveway Middle"

  #define 
  
  // #define USE_MODULE_CONTROLLER_SECURITY_LIGHT //disable until I rewrite for the sidelight

  #define USE_MODULE_DRIVERS_RELAY
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D1\":\"" D_GPIO_FUNCTION_PIR_1_NP_INV_CTR     "\","
      #endif 
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"D2\":\"" D_GPIO_FUNCTION_REL1_CTR   "\","
      #endif
      "\"D4\":\""  D_GPIO_FUNCTION_LED1_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Side Door"
  #define MAX_RELAYS 1
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif




#ifdef DEVICE_NODEMCU_TESTER
  #define DEVICENAME_CTR          "nodemcu_tester"
  #define DEVICENAME_FRIENDLY_CTR "NodeMCU Tester"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2
     
  // #define USE_MODULE_SENSORS_MOTION
  // #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE_SENSORS_SWITCHES

  // #define USE_MODULE_SENSORS_BME

  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_ANIMATOR  

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
      // #ifdef USE_MODULE_SENSORS_BUTTONS
      // "\"D3\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      // "\"D4\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      // #endif    
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"D3\":\"" D_GPIO_FUNCTION_SWT1_CTR  "\","
      "\"D4\":\""  D_GPIO_FUNCTION_SWT2_CTR  "\","
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
    "\"" D_JSON_HARDWARE_TYPE  "\":\""  D_JSON_WS2812 "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\""  D_JSON_GRB    "\","
    "\"" D_JSON_ANIMATIONMODE           "\":\""  D_JSON_//Scene  "\","
    "\"" D_JSON_//Scene_COLOUR   "\":{"
          "\"" D_JSON_HSB    "\":[15,95,0]" 
        "},"
    "\"" D_JSON_BRIGHTNESS       "\":0,"
    "\"" D_JSON_BRIGHTNESS_RGB          "\":0"
  "}";
  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  #define PIXEL_LIGHTING_HARDWARE_SK6812_STRIP

  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Ensuite"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Ensuite"

  #define D_DEVICE_BUTTON_1_CTR "FurnaceActive"
  #define D_DEVICE_BUTTON_2_CTR "Button2"
  #define D_DEVICE_BUTTON_3_CTR "Button3"
  #define D_DEVICE_BUTTON_4_CTR "Button4"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_BUTTONS_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_BUTTON_1_CTR "\","
        "\"" D_DEVICE_BUTTON_2_CTR "\","
        "\"" D_DEVICE_BUTTON_3_CTR "\","
        "\"" D_DEVICE_BUTTON_4_CTR "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_BUTTON_1_CTR "\","
        "\"" D_DEVICE_BUTTON_2_CTR "\","
        "\"" D_DEVICE_BUTTON_3_CTR "\","
        "\"" D_DEVICE_BUTTON_4_CTR "\""
      "]"
    "}"
  "}";

#endif



#ifdef DEVICE_BLACKDOORBELL
  #define DEVICENAME_CTR          "blackdoorbell"
  #define DEVICENAME_FRIENDLY_CTR "Black Doorbell"

  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 2 //maintain other settings (bootcount)
     
  #define USE_MODULE_SENSORS_MOTION
  #define USE_MODULE_SENSORS_BUTTONS

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D1\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR     "\","
      #endif      
      "\"D2\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR     "\","
      
      "\"0\":\"" D_GPIO_FUNCTION_LED1_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "BlackDoorbell"
  
  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif

#ifdef DEVICE_WHITEFAN1
  #define DEVICENAME_CTR          "whitefan1"
  #define DEVICENAME_FRIENDLY_CTR "White Fan 1"
  #define USE_MODULE_DRIVERS_RELAY 1
  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "relay"
  #define USE_MODULE_SENSORS_BUTTONS

  
    pCONT_set->Settings.module = MODULE_SONOFF_BASIC_ID;
    //pCONT_set->Settings.module_pins.io[14]= GPIO_REL1_INV_ID; 
    pCONT_set->devices_present = 1_ID;
#endif







#ifdef DEVICE_KITCHENLIGHT5
  #define DEVICENAME_CTR          "kitchenlight5"
  #define DEVICENAME_FRIENDLY_CTR "Kitchen Light 5 Shelly 2 desktester"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2
  
  // #define ENABLE_BUG_TRACING

  // #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE_SENSORS_SWITCHES

  #define ENABLE_SONOFF_TEMPORARY_SHOW_LED_STATUS

  #define USE_MODULE_DRIVERS_RELAY
  #define MAX_RELAYS 1
  #define USE_MODULE_DRIVERS_INTERFACE
    
  // #define USE_MODULE_TEMPLATE
  // DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
  //   "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY2P5_CTR "\""
  // "}";

  
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
      "\"4\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      // "\"15\":\"" D_GPIO_FUNCTION_REL2_INV_CTR  "\","
      #endif 
      "\"0\":\"" D_GPIO_FUNCTION_LED1_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Light1"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Light2"
  // #define SET_POWERONSTATE_AS_ON

  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    //device_names:{"module_name":["relay1","relay2"]}
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
          "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";
#endif



// difference between dimmer1 and dimmer2, make myself a hardware template

#ifdef DEVICE_TESTBED_SHELLY2_DIMMER
  #define DEVICENAME_CTR          "shellydimmer_testbed1"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Shelly2"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2
  
  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_DRIVERS_SHELLY_DIMMER
  #define DISABLE_SERIAL_LOGGING //temp measure

  // #define USE_MODULE_SENSORS_MOTION
  // #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Driveway Top"
  // #define D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "Back Garden"
  
  // #define USE_MODULE_CONTROLLER_SECURITY_LIGHT //remote controls for now

  // #define USE_MODULE_DRIVERS_RELAY
  // #define MAX_RELAYS 2
  // #define USE_MODULE_DRIVERS_INTERFACE
    

  //     #define PIN_GPIO_SHELLY_DIMMER_RST_INV 5
  //     #define PIN_GPIO_SHELLY_DIMMER_BOOT0   4

  // #define USE_MODULE_TEMPLATE
  // DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
  //   "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  //   "\"" D_JSON_GPIOC "\":{"
  //     //0,<100>,0,<101>,<174>,<175>,0,0,<6>,0,<6>,0,<10>,<148>
  //     //0,    1,2,    3,    4,    5,9,10,11,12,13,14,15,16

  //     //1=100, TX
  //     //3=101, RX
  //     "\"1\":\"" D_GPIO_FUNCTION_HWSERIAL0_TX_CTR  "\","
  //     "\"3\":\"" D_GPIO_FUNCTION_HWSERIAL0_RX_CTR  "\","
  //     //4=174, GPIO_SHELLY_DIMMER_BOOT0
  //     //5=175, GPIO_SHELLY_DIMMER_RST_INV
  //     // "\"1\":\"" D_GPIO_FUNCTION_HWSERIAL0_TX_CTR  "\","
  //     // "\"3\":\"" D_GPIO_FUNCTION_HWSERIAL0_RX_CTR  "\","



  //     //11=6, GPIO_SWT1_NP
  //     //13=6, GPIO_SWT1_NP
  //     #ifdef USE_MODULE_SENSORS_SWITCHES
  //     "\"12\":\"" D_GPIO_FUNCTION_SWT1_NP_CTR  "\","
  //     "\"14\":\"" D_GPIO_FUNCTION_SWT2_NP_CTR  "\""
  //     #endif
  //     //16=148, GPIO_ADC_TEMP
      
  //     //15=10, GPIO_LED1_INV
  //     // "\"16\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
  //   "},"
  //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  // "}";
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY_DIMMER2_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Driveway Light"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Garden Light"
  // #define SET_POWERONSTATE_AS_ON

  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    //device_names:{"module_name":["relay1","relay2"]}
    "\"" D_JSON_DEVICENAME "\":{"
        // "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        //   "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
        //   "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
        // "],"
        "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
          "\"" "Switch1" "\","
          "\"" "Switch2" "\""
        // "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        //   "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\","
        //   "\"" D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";
#endif






#ifdef DEVICE_SHELLYDIMMER_GLOBE
  #define DEVICENAME_CTR          "shellydimmer_globe" //shelly1
  #define DEVICENAME_FRIENDLY_CTR "Shelly Dimmer 1 Globe"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2
  
  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_DRIVERS_SHELLY_DIMMER
  #define DISABLE_SERIAL_LOGGING //temp measure

  // #define USE_MODULE_SENSORS_MOTION
  // #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Driveway Top"
  // #define D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "Back Garden"
  
  // #define USE_MODULE_CONTROLLER_SECURITY_LIGHT //remote controls for now

  // #define USE_MODULE_DRIVERS_RELAY
  // #define MAX_RELAYS 2
  // #define USE_MODULE_DRIVERS_INTERFACE
    

      #define PIN_GPIO_SHELLY_DIMMER_RST_INV 5
      #define PIN_GPIO_SHELLY_DIMMER_BOOT0   4

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      //0,<100>,0,<101>,<174>,<175>,0,0,<6>,0,<6>,0,<10>,<148>
      //0,    1,2,    3,    4,    5,9,10,11,12,13,14,15,16

      //1=100, TX
      //3=101, RX
      "\"1\":\"" D_GPIO_FUNCTION_HWSERIAL0_TX_CTR  "\","
      "\"3\":\"" D_GPIO_FUNCTION_HWSERIAL0_RX_CTR  "\","
      //4=174, GPIO_SHELLY_DIMMER_BOOT0
      //5=175, GPIO_SHELLY_DIMMER_RST_INV

      //11=6, GPIO_SWT1_NP
      //13=6, GPIO_SWT1_NP
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"14\":\"" D_GPIO_FUNCTION_SWT1_NP_CTR  "\","
      "\"12\":\"" D_GPIO_FUNCTION_SWT2_NP_CTR  "\","
      #endif
      //16=148, GPIO_ADC_TEMP
      
      //15=10, GPIO_LED1_INV
      "\"16\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Driveway Light"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Garden Light"
  // #define SET_POWERONSTATE_AS_ON

  // Drivers, Sensors and lights?
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
          "\"" "Switch1" "\","
          "\"" "Switch2" "\""
        // "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        //   "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\","
        //   "\"" D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";
#endif




#ifdef DEVICE_DESKFAN
  #define DEVICENAME_CTR          "deskfan"
  #define DEVICENAME_FRIENDLY_CTR "Desk Fan"
    
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  
  #define USE_MODULE_DRIVERS_PWM
  #define USE_MODULE_CONTROLLER_FAN
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_PWM
      "\"D1\":\"" D_GPIO_FUNCTION_FAN_PWM1_CTR   "\","
      #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

#endif


#ifdef DEVICE_EXERCISE_BIKE
  #define DEVICENAME_CTR          "exercise_bike"
  #define DEVICENAME_FRIENDLY_CTR "Sonoff Basic Tester"
  #define USE_MODULE_SENSORS_PULSE_COUNTER
  #define USE_MODULE_CONTROLLER_EXERCISE_BIKE  
  
    pCONT_set->Settings.module_pins.io[D7] = GPIO_PULSE_COUNTER_ID;
#endif



#ifdef DEVICE_GPSPARSER_TESTER
  #define DEVICENAME_CTR            "gpsparser_tester"
  #define DEVICENAME_FRIENDLY_CTR   "GPS Parser Tester"

  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 21
  #define USE_DEBUG_PRINT_FUNCTION_NAME_TEST

  #define USE_MODULE_DRIVERS_GPS
  // #define NMEAGPS_INTERRUPT_PROCESSING

  #define USE_DEVFEATURE_RINGBUFFERS

  // #define USE_MODULE_DRIVERS_SERIAL_UART

  #define NMEAGPS_PARSE_SAVE_MILLIS
  
  #define NEOGPS_USE_SERIAL1
#include <GPSport.h>

// FEATURE
// Create an "alert" mqtt topic that all devices will broadcast important info on

#define ENABLE_DEVFEATURE_GPSTEST1

  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define USE_MODULE_LIGHTS_ADDRESSABLE
  // #define USE_MODULE_LIGHTS_WLED_EFFECTS
  // #define WLED_DEFINE_GLOBAL_VARS //only in one source file, wled.cpp!
  // #define DISABLE_PIXEL_FUNCTION_EFFECTS
  // #define USE_MODULE_DRIVERS_LEDS
  #define DISABLE_WEBSERVER
  
//   //#define USE_WEBSERVER_ADVANCED_MULTIPAGES // new develop option to limit scope to only include root page while testing
  
//   #define USE_MODULE_SENSORS_SWITCHES
//   #define USE_MODULE_SENSORS_ANALOG

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

  
  // #define STRIP_SIZE_MAX 100//256
  // #define STRIP_REPEAT_OUTPUT_MAX 256

  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
  //   #ifdef STRIP_SIZE_MAX
  //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  //   #else
  //   "\"" D_JSON_STRIP_SIZE       "\":50,"
  //   #endif //STRIP_SIZE_MAX
  //   "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
  //   "\"" D_JSON_TRANSITION       "\":{"
  //     "\"" D_JSON_TIME_MS "\":10000,"
  //     "\"" D_JSON_RATE_MS "\":1000,"
  //     "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
  //     "\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\""
  //   "},"
  //   "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
  //   "\"" D_JSON_EFFECTS "\":{" 
  //     "\"" D_JSON_FUNCTION "\":\"" "Slow Glow" "\""
  //   "},"
  //   "\"ColourPalette\":\"Single Fire 01\","//Christmas MultiColoured Warmer\","
  //   "\"BrightnessRGB\":2"
  // "}";



#endif // DEVICE_GPSPARSER_TESTER


#ifdef DEVICE_SDCARDWRITER_TESTER
  #define DEVICENAME_CTR            "sdcardwriter_tester"
  #define DEVICENAME_FRIENDLY_CTR   "SD Card Writer Tester"

  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 21

  #define USE_MODULE_DRIVERS_SDCARD

  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define USE_MODULE_LIGHTS_ADDRESSABLE
  // #define USE_MODULE_LIGHTS_WLED_EFFECTS
  // #define WLED_DEFINE_GLOBAL_VARS //only in one source file, wled.cpp!
  // #define DISABLE_PIXEL_FUNCTION_EFFECTS
  // #define USE_MODULE_DRIVERS_LEDS
  #define DISABLE_WEBSERVER
  
//   //#define USE_WEBSERVER_ADVANCED_MULTIPAGES // new develop option to limit scope to only include root page while testing
  
//   #define USE_MODULE_SENSORS_SWITCHES
//   #define USE_MODULE_SENSORS_ANALOG

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

  
  #define STRIP_SIZE_MAX 100//256
  #define STRIP_REPEAT_OUTPUT_MAX 256

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
    "\"ColourPalette\":\"Single Fire 01\","//Christmas MultiColoured Warmer\","
    "\"BrightnessRGB\":2"
  "}";



#endif // DEVICE_GPSPARSER_TESTER



/**
 *  TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- 
**/


#ifdef DEVICE_TESTBED_SHELLY1_01
  #define DEVICENAME_CTR          "testbed_shelly1_01"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Shelly1 #01"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2
   
  #define USE_MODULE_SENSORS_MOTION
  #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Test"

  // #define USE_MODULE_CONTROLLER_SECURITY_LIGHT //disable until I rewrite for the sidelight

  #define USE_MODULE_DRIVERS_RELAY
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D1\":\"" D_GPIO_FUNCTION_PIR_1_NP_INV_CTR     "\","
      #endif 
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"D2\":\"" D_GPIO_FUNCTION_REL1_CTR   "\","
      #endif
      "\"D4\":\""  D_GPIO_FUNCTION_LED1_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Power"
  #define MAX_RELAYS 1
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif


#ifdef DEVICE_TESTBED_SHELLY2P5_01
  #define DEVICENAME_CTR          "testbed_shelly25_01"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Shelly 2.5 #01"

  // #define USE_MODULE_SENSORS_MOTION
  // #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Test"

  // #define ENABLE_DEVFEATURE_RULES_MQTT_FASTER_SECS

  // #define ENABLE_DEVFEATURE_OTA_FALLBACK_ON_BOOT_LOOPING
  /*
    Method should only activate if boot loop happens 10 times
    Method A: Switch to Wifi.begin hardcoded ssid/pw, start OTA and wait, rebooting every 10 minutes if wifi does not connect
    Method B: Begin wifi.ap as host, so I can directly connect to it via x.x.x.x
  */

  //{"NAME":"Shelly 2.5","GPIO":[
    
  /*
  320,0,32,0,224,193,0,0,640,192,608,225,3456,4736
  */
  //],"FLAG":0,"BASE":18}
  #define USE_MODULE_SENSORS_PRESENCE

  #define USE_MODULE_ENERGY_INTERFACE
  #define USE_MODULE_ENERGY_ADE7953

  #define FORCE_TEMPLATE_LOADING
  // #define SETTINGS_HOLDER 2
  
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_BUTTONS
  // #define USE_MODULE_SENSORS_MOTION

  #define USE_MODULE_CORE_RULES

  #define USE_MODULE_DRIVERS_RELAY
  #define MAX_RELAYS 2
  #define USE_MODULE_DRIVERS_INTERFACE

  // #define ENABLE_DEVFEATURE_RELAY_TIME_SCHEDULED_DEFAULT_ON
  

  // #define USE_MODULE_CONTROLLER_SECURITY_LIGHT
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    // "\"" D_JSON_GPIOC "\":{"
    //   #ifdef USE_MODULE_SENSORS_SWITCHES
    //   "\"13\":\"" D_GPIO_FUNCTION_SWT1_NP_CTR  "\","
    //   "\"5\":\""  D_GPIO_FUNCTION_SWT2_NP_CTR  "\","
    //   #endif
    //   #ifdef USE_MODULE_SENSORS_MOTION
    //   "\"13\":\"" D_GPIO_FUNCTION_PIR_1_NP_CTR  "\","
    //   "\"5\":\""  D_GPIO_FUNCTION_PIR_2_NP_CTR  "\","
    //   #endif
    //   #ifdef USE_MODULE_DRIVERS_RELAY
    //   "\"4\":\"" D_GPIO_FUNCTION_REL1_CTR  "\","
    //   "\"15\":\"" D_GPIO_FUNCTION_REL2_CTR  "\","
    //   #endif 
    //   #ifdef USE_MODULE_SENSORS_BUTTONS
    //   "\"2\":\"" D_GPIO_FUNCTION_KEY1_CTR  "\","
    //   #endif 
    //   #ifdef USE_MODULE_ENERGY_ADE7953 // I2C address 0x38
    //   "\"14\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR  "\","
    //   "\"12\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR  "\","
    //   "\"16\":\"" D_GPIO_FUNCTION_ADE7953_IRQ_CTR  "\","
    //   #endif 
    //   #ifdef USE_MODULE_SENSORS_ANALOG_TEMPERATURE
    //   "\"A0\":\"" D_GPIO_FUNCTION_ANALOG_TEMPERATURE_CTR  "\","
    //   #endif 
    //   "\"0\":\"" D_GPIO_FUNCTION_LED1_CTR "\""
    // "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY2P5_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Power0"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Power1"
  #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Motion0"
  #define D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "Motion1"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "\""
      "]"
    "},"
    "\"RelayEnabled0\":{\"Enabled\":1,\"OnTime\":\"00D15:45:00\",\"OffTime\":\"00D00:00:00\"},"
    "\"RelayEnabled1\":{\"Enabled\":1,\"OnTime\":\"00D17:00:00\",\"OffTime\":\"00D00:00:00\"}"
  "}";

  // #define USE_RULES_TEMPLATE // Rules, like the other templates, will be feed into the same command structure, so can actually be combined with `FUNCTION_TEMPLATE`
  // DEFINE_PGM_CTR(RULES_TEMPLATE)
  // "{"
  //   "\"Rule0\":{"
  //     "\"Trigger\":{"
  //       "\"Module\":\"Switches\","
  //       "\"Function\":\"StateChanged\","
  //       "\"DeviceName\":0,"
  //       "\"State\":2"
  //     "},"
  //     "\"Command\":{"
  //       "\"Module\":\"Relays\","
  //       "\"Function\":\"SetPower\","
  //       "\"DeviceName\":0,"
  //       "\"State\":2"
  //     "}"
  //   "},"
  //   "\"Rule1\":{"
  //     "\"Trigger\":{"
  //       "\"Module\":\"Switches\","
  //       "\"Function\":\"StateChanged\","
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

//   // #define ENABLE_DEVFEATURE_PERIODIC_RULE_FILLING
//   #define LOG_BUFFER_SIZE  1000

//   #define USE_RULES_TEMPLATE // Rules, like the other templates, will be feed into the same command structure, so can actually be combined with `FUNCTION_TEMPLATE`
//   DEFINE_PGM_CTR(RULES_TEMPLATE)
//   "{"
//     "\"Rule0\":{" //switch example
//       "\"Trigger\":{"
//       //module_id
//         "\"Module\":\"Motion\","    //sensor
//         //?
//         "\"Function\":\"MotionStarted\"," //eg. InputChange (TemperatureThreshold)
//         //[1]
//         "\"DeviceName\":0," // eg Switch0, Switch1, Button#, Motion, # (number for index)  
//         //[0]
//         "\"State\":2" //eg. On, Off, Toggle, Any, LongPress, ShortPress, RisingEdge, FallingEdge, Started, Ended, TimerOnStarted
//       "},"
//       "\"Command\":{"
//         "\"Module\":\"Relays\","
//         // set power?
//         "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
//         //1
//         "\"DeviceName\":0," //number, name, or all
//         //2
//         "\"State\":1,"//setpower
//         // Append other "normal" commands here? this would need storing
//         // "\"JsonCommands\":\"{\\\"Relay\\\":{\\\"TimeOn\\\":60}}\""
//         "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOn\\\":60}}\""

//         //relay:timeon needs a way to specify which device
        
//         // {"PowerName":0,"PowerState":1,"Relay":{"TimeOn":5}}


//         //relay 
//       "}"
//     "},"
//     "\"Rule1\":{" //switch example
//       "\"Trigger\":{"
//       //module_id
//         "\"Module\":\"Motion\","    //sensor
//         //?
//         "\"Function\":\"MotionStarted\"," //eg. InputChange (TemperatureThreshold)
//         //[1]
//         "\"DeviceName\":1," // eg Switch0, Switch1, Button#, Motion, # (number for index)  
//         //[0]
//         "\"State\":1" //eg. On, Off, Toggle, Any, LongPress, ShortPress, RisingEdge, FallingEdge, Started, Ended, TimerOnStarted
//       "},"
//       "\"Command\":{"
//         "\"Module\":\"Relays\","
//         // set power?
//         "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
//         //1
//         "\"DeviceName\":1," //number, name, or all
//         //2
//         "\"State\":1,"//setpower
//         // Append other "normal" commands here? this would need storing
//         // "\"JsonCommands\":\"{\\\"Relay\\\":{\\\"TimeOn\\\":60}}\""
        
//         // "\"JsonCommands\":\"{\\\"PowerName\\\":5,\\\"Relay\\\":{\\\"TimeOn\\\":12}}\""
//         // "\"JsonCommands\":\"{\\\"MQTTSend\\\":{\\\"Topic\\\":\\\"kitchenlight5/set/relay\\\",\\\"Payload\\\":\\\"{\\\\\"PowerName\\\\\":0,\\\\\"PowerState\\\\\":2}\\\"}}\""
//         // "\"JsonCommands\":\"{\\\"MQTTSend\\\":{\\\"Topic\\\":\\\"kitchenlight5/set/relay\\\",\\\"Payload\\\":\\\"hello\\\"}\""
//         "\"JsonCommands\":\"{\\\"MQTTSend\\\":{\\\"Topic\\\":\\\"kitchenlight4/set/relays\\\",\\\"Payload\\\":\\\"{~PowerName~:0,~PowerState~:2,~TimeOn~:10}\\\"}}\""
 
// //  {"MQTTSend":
// //   {
// //     "Topic":"kitchenlight5/set/relay",
// //     "Payload":
// //       "{"PowerName":0,"PowerState":2}"
// //   }
// // }




//         // "\"JsonCommands\":\"{\\\"PowerName\\\":1,\\\"Relay\\\":{\\\"TimeOn\\\":60}}\""
//         //relay 
//       "}"
//     "}"
    

//   "}";




#endif



#ifdef DEVICE_ESP32_DEVKIT_BASIC
  #define DEVICENAME_CTR            "esp32_devkit_basic"
  #define DEVICENAME_FRIENDLY_CTR   "esp32_devkit_basic"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define ESP32

//   #define ENABLE_DEVFEATURE_PIN_FUNCTION_METHOD

//   #define USE_DEBUG_PRINT_FUNCTION_NAME_TEST

//   #define ENABLE_SERIAL_FLUSH

//   #define USE_MODULE_DRIVERS_INTERFACE
//   #define USE_MODULE_DRIVERS_GPS

//   #define USE_MODULE_DRIVERS_SERIAL_UART
//   #define USE_DEVFEATURE_GPS_FROM_SERIAL2_BUFFER

//   //Interrupt method
//   // #define NMEAGPS_INTERRUPT_PROCESSING

//   #define ENABLE_UART2_ISR_BUFFERS

//   #define NEOGPS_USE_SERIAL1

//   #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED 192,168,1,116

//   #define USE_DEVFEATURE_RINGBUFFERS

//   // #define USE_MODULE_DRIVERS_SERIAL_UART

//   #define NMEAGPS_PARSE_SAVE_MILLIS
  
// #include <GPSport.h>

// // FEATURE
// // Create an "alert" mqtt topic that all devices will broadcast important info on

  #define USE_MODULE_DRIVERS_PWM
// #define ENABLE_DEVFEATURE_GPSTEST1

//   // #define DISABLE_WEBSERVER
  
// //   //#define USE_WEBSERVER_ADVANCED_MULTIPAGES // new develop option to limit scope to only include root page while testing
  
// //   #define USE_MODULE_SENSORS_SWITCHES
// //   #define USE_MODULE_SENSORS_ANALOG

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"LBI\":\""  D_GPIO_FUNCTION_PWM1_CTR "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

#endif // DEVICE_ESP32_DEVKIT_BASIC


#ifdef DEVICE_ESP32_WEBCAM1
  #define DEVICENAME_CTR            "esp32_webcam1"
  #define DEVICENAME_FRIENDLY_CTR   "esp32_webcam1"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define ESP32

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"LBI\":\""  D_GPIO_FUNCTION_PWM1_CTR "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_CAM_AITHINKER_CTR "\""
  "}";

#endif // DEVICE_ESP32_DEVKIT_BASIC


/**
 * 1) Serial0 is debugging, and will be disabled
 * 2) Serial1 is RSS samples in as superframes
 * 3) Serial2 is GPS input (9600)
 * 4) GPSParser using my hardware, as fast as possible
 * 5) OLED screen to give sd card status
 * 6) SDCard records superframe+gpsfix
 * 7) Need button to open/close sd (or dupont short? or small surface mount)
 * 
 * Configure dummy data, and feed it into both UART_GPS and UART_RSS simultaneously,
 * Echo both directly out of UART_RSS_TX onto putty, leave it working all the time (laptop side)
 * 
 * 
 * 1 - Check what gps can do, can I enter it into ublox? Does this improve speed, let me do manual decoding?
 * Test 1 - Create long wire for gps to sit in window and give active gps data
 * 
 * STEPS # - uncomment just the section below it, so I can quickly test configs as I go, only uncomment one step at a time
 * 
 * **/
#ifdef DEVICE_SDLOGGER1_TESTER
  #define DEVICENAME_CTR            "sdlogger1_tester"
  #define DEVICENAME_FRIENDLY_CTR   "SD & GPS Logger Tester"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  /**
   *  - Test Buttons
   * */
  // #define USE_MODULE_SENSORS_BUTTONS
  /**
   *  - Enable rule triggers
   * */
  // #define USE_MODULE_CORE_RULES
  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BUTTONS
  /**
   *  - Add triggered function in sdcard that will toggle sd later via button
   * */
  // #define USE_MODULE_CORE_RULES
  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BUTTONS
  // #define USE_MODULE_CONTROLLER_SDCARDLOGGER
  /**
   *  - Enable displays, and show button pressed on it
   * */
  // #define USE_MODULE_CORE_RULES
  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BUTTONS
  // #define USE_MODULE_CONTROLLER_SDCARDLOGGER
  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SSD1306
  /**
   *  - Get stable uart2 ringbuffer
   * */
  // #define USE_MODULE_DRIVERS_SERIAL_UART
  // #define USE_DEVFEATURE_RINGBUFFERS // Rename, its simply a uart method, to use ringbuffers method vs basic read/poll
  // #define ENABLE_UART2_ISR_BUFFERS
  // #define ENABLE_FEATURE_BLINK_ON_ISR_ACTIVITY
  /**
   *  - Read GPS Data into UART_GPS and UART_RSS (as dummy data)
   * */
  // #define USE_MODULE_CORE_RULES
  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BUTTONS
  // #define USE_MODULE_CONTROLLER_SDCARDLOGGER
  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SSD1306
  // #define USE_MODULE_DRIVERS_INTERFACE
  // #define USE_MODULE_DRIVERS_SERIAL_UART
  // #define USE_DEVFEATURE_RINGBUFFERS // Rename, its simply a uart method, to use ringbuffers method vs basic read/poll
  // #define ENABLE_UART2_ISR_BUFFERS


  #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES

  #define ESP32

  // 

  // #define USE_MODULE_SENSORS_BUTTONS

  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SSD1306
  // #define USE_DISPLAY_MODES1TO5
  // #define SHOW_SPLASH


//   #define USE_DEBUG_PRINT_FUNCTION_NAME_TEST

//   #define ENABLE_SERIAL_FLUSH

//   #define USE_MODULE_DRIVERS_INTERFACE
// // #define USE_MODULE_DRIVERS_GPS

//   
//   #define USE_DEVFEATURE_GPS_FROM_SERIAL2_BUFFER

//   //Interrupt method
//   // #define NMEAGPS_INTERRUPT_PROCESSING

//   

//   #define NEOGPS_USE_SERIAL1

//   // #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED 192,168,1,116

//   #define USE_DEVFEATURE_RINGBUFFERS

//   // #define USE_MODULE_DRIVERS_SERIAL_UART

//   #define NMEAGPS_PARSE_SAVE_MILLIS
  
  // #include <GPSport.h>

  // FEATURE
  // Create an "alert" mqtt topic that all devices will broadcast important info on

  // #define ENABLE_DEVFEATURE_GPSTEST1

  // #define DISABLE_WEBSERVER

  //   //#define USE_WEBSERVER_ADVANCED_MULTIPAGES // new develop option to limit scope to only include root page while testing

  //   #define USE_MODULE_SENSORS_SWITCHES
  //   #define USE_MODULE_SENSORS_ANALOG

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"

      // OLED screen
      #ifdef USE_MODULE_DISPLAYS_OLED_SSD1306
      "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      // "\"2\":\"" D_GPIO_FUNCTION_OLED_RESET_CTR   "\","
      #endif // USE_MODULE_DISPLAYS_OLED_SSD1306

      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"22\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\","
      #endif 

      // 2 push buttons

      // SD card

      // GPS

      // Serial data logger (just receives into a buffer)
      
      "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
      "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","




      "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    // "\"" D_JSON_DEVICENAME "\":{"
    //   "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
    //     "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
    //   "],"
    //   "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
    //     "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\""
    //   "]"
    // "}"
    
    "\"" D_JSON_DISPLAY_MODE "\":6"
  "}";







#endif // DEVICE_GPSPARSER_TESTER



#ifdef DEVICE_FORCED_TO_BE_TESTER
  #undef DEVICENAME_CTR
  #define DEVICENAME_CTR "testerdevice"
  #warning "--------------------------------------------------------------- DEVICE_FORCED_TO_BE_TESTER" is enabled for this compile
#endif


#endif // MSYSTEMCONFIG_HARDWAREDEFAULTS_H

