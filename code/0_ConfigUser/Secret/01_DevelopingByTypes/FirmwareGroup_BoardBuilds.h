#ifndef _CONFIG__FIRMWARE_GROUP__BOARDBUILDS_H
#define _CONFIG__FIRMWARE_GROUP__BOARDBUILDS_H

/*********************************************************************************************\
 * Descriptions: 
 *  ** Simple base builds to test the firmware can be built for different board variants
 *  == ESP8266
 *     - NodeMCU
 * 
 *  == ESP8285
 * 
 *  == ESP32 
 *     - Doit Devkit
 *     - Aithinker Camera
 *     - Wrover Kit
 *     - C3 SeedStudio Xiao
 *     - S3 Supermini Pro
\*********************************************************************************************/

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"

//--------------------------------[Enable Device]-------------------------------------


// ======================== ESP8266 ========================
// #define DEVICE_TESTGROUP__BOARDBUILDS__ESP8266__NODEMCU
// ======================== ESP8285 ========================
// #define DEVICE_TESTGROUP__BOARDBUILDS__ESP8285__IFAN03
// ======================== ESP32 ===========================
// #define DEVICE_TESTGROUP__BOARDBUILDS__ESP32WROOVER__CAMERA
// #define DEVICE_TESTGROUP__BOARDBUILDS__ESP32S3_SEEDSTUDIO_XIAO__CAMERA
// #define DEVICE_TESTGROUP__BOARDBUILDS__ESP32_CAM_EYE


//--------------------------------[Device Template]-------------------------------------


/**
 * @brief 
 *          fH (Boot Fail - Pulled High) → Pin must be LOW at boot, else boot may fail
 *          fL (Boot Fail - Pulled Low) → Pin must be HIGH at boot, else boot may fail
 *          key (Key Pin) → GPIO0 on DOIT DevKit v1 (not )
 *          BIL (Built-in LED) → On some boards, pin is used for onboard LED
 *          BIB (Built-in Button) → On some boards, pin is used for onboard button
 *                               *I ~PWM 'NC    
 *                       _____________________
 *                      |3V3     |USB|     VIN|
 *                      |GND               GND| 
 *                      |TX  G1, fL        RST| 
 *                      |RX  G3             EN| 
 *                      |D8  G15,fL        3V3|
 *                      |D7  G13           GND| 
 *                      |D6  G12           CLK| 
 *                      |D5  G14           SDO| 
 *                      |GND               CMD| 
 *                      |3V3               SD1| 
 *                      |D4  G2,fL,BIL     SD2| 
 *                      |D3  G0,fL,BIB     SD3| 
 *                      |D2  G4            RSV| 
 *                      |D1  G5            RSV| 
 *                      |D0  G16      ADC0  A0| 
 *                       _____________________
 **/
#ifdef DEVICE_TESTGROUP__BOARDBUILDS__ESP8266__NODEMCU
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "tg_boardbuild__esp8266_nodemcu"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #ifndef DEVICENAME_ROOMHINT_CTR
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #endif
   
  /***********************************
  * SECTION: System Debug Options
  ************************************/    
  ///////////////////////////////////////////// Enable Logs
  // #define DISABLE_SERIAL

  #define ESP8266

  ///////////////////////////////////////////// System Logs
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME

  /***********************************
  * SECTION: System Configs
  ************************************/    

  #define SETTINGS_HOLDER 1239

  #define ENABLE_DEBUGFEATURE_TIME__SHOW_UPTIME_EVERY_SECOND


  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES
  // #define ENABLE_DEBUG_LINE_HERE

  #define ENABLE_FEATURE_BUILD__11JAN25_WIFI_AND_WEBUI


  // #define DEBUG_FASTBOOT

  // #define ENABLE_DEVFEATURE_WIFI__FORCE_SOFTAP_MODE_BY_BLOCKING_SSIDS


  #define   D_CAPTIVE_PORTAL_URL_REDIRECT_PATH "/m/serverrelays"
      
  /***********************************
  * SECTION: Enable Sections
  ************************************/  

  // #define ENABLE_TEMPLATE_SECTION__SENSORS__BUTTONS
  // #define ENABLE_TEMPLATE_SECTION__DRIVERS__LEDS // Status LED

  /***********************************
  * SECTION: Network Configs
  ************************************/    


  /***********************************
  * SECTION: Drivers Configs
  ************************************/  
  #define USE_MODULE_DRIVERS_LEDS // 3 blink - no network, 2 blink = network, no mqtt
    // #define ENABLE_DEVFEATURE_DRIVER_LED__FORCED_LED_TOGGLE_ON_PIN 2

  /***********************************
  * SECTION: Sensor Configs
  ************************************/  

  /***********************************
  * SECTION: Lighting Configs
  ************************************/  

  /***********************************
  * SECTION: Energy Configs
  ************************************/  

  /***********************************
   * SECTION: Display Configs
  ************************************/  

  /***********************************
  * SECTION: Controller Configs
  ************************************/  

  /***********************************
  * SECTION: Module/GPIO Configs
  ************************************/  

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"D3\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"D4\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  /***********************************
  * SECTION: TEMPLATE: Names
  ************************************/    

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60}," 
  "}";

#endif



/**
 * @brief 
 * 
 * Using IFAN03 hardware as test build for esp8285 boards. Confirming if template loads.
 * 
 *          fH (Boot Fail - Pulled High) → Pin must be LOW at boot, else boot may fail
 *          fL (Boot Fail - Pulled Low) → Pin must be HIGH at boot, else boot may fail
 *          key (Key Pin) → GPIO0 on DOIT DevKit v1 (not )
 *          BIL (Built-in LED) → On some boards, pin is used for onboard LED
 *          BIB (Built-in Button) → On some boards, pin is used for onboard button
 *                               *I ~PWM 'NC    
 *                       _____________________
 *                      |3V3     |USB|     VIN|
 *                      |GND               GND| 
 *                      |TX  G1, fL        RST| 
 *                      |RX  G3             EN| 
 *                      |D8  G15,fL        3V3|
 *                      |D7  G13           GND| 
 *                      |D6  G12           CLK| 
 *                      |D5  G14           SDO| 
 *                      |GND               CMD| 
 *                      |3V3               SD1| 
 *                      |D4  G2,fL,BIL     SD2| 
 *                      |D3  G0,fL,BIB     SD3| 
 *                      |D2  G4            RSV| 
 *                      |D1  G5            RSV| 
 *                      |D0  G16      ADC0  A0| 
 *                       _____________________
 **/
#ifdef DEVICE_TESTGROUP__BOARDBUILDS__ESP8285__IFAN03
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "tg_boardbuild__esp8285_ifan03"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "Template Name"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "Template Description"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "template_roomhint"
  #define MQTT_HOST   "192.168.3.70"
    #define MQTT_PORT     1883
  
  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define ESP8266

  #define USE_MODULE_CORE_RULES
  
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE__DRIVERS_BUZZER_BASIC
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_CONTROLLER_SONOFF_IFAN
    #define USE_MODULE_TEMPLATE_SONOFF_IFAN03

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_BASE "\":\"" D_MODULE_NAME_SONOFF_IFAN03_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE) // temp solution, the unedefined relay should be handled by GetDeviceName to add the unique index and not random
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_BUTTONS_CTR "\":["
        "\"Button\""
      "],"
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"Relay1\","
        "\"Relay2\","
        "\"Relay3\","
        "\"Relay4\""
      "]"
    "}"
  "}";


#endif



/**
 * @brief 
 * 
 * Using IFAN03 hardware as test build for esp8285 boards. Confirming if template loads.
 * 
 *          fH (Boot Fail - Pulled High) → Pin must be LOW at boot, else boot may fail
 *          fL (Boot Fail - Pulled Low) → Pin must be HIGH at boot, else boot may fail
 *          key (Key Pin) → GPIO0 on DOIT DevKit v1 (not )
 *          BIL (Built-in LED) → On some boards, pin is used for onboard LED
 *          BIB (Built-in Button) → On some boards, pin is used for onboard button
 *                               *I ~PWM 'NC    
 *                       _____________________
 *                      |3V3     |USB|     VIN|
 *                      |GND               GND| 
 *                      |TX  G1, fL        RST| 
 *                      |RX  G3             EN| 
 *                      |D8  G15,fL        3V3|
 *                      |D7  G13           GND| 
 *                      |D6  G12           CLK| 
 *                      |D5  G14           SDO| 
 *                      |GND               CMD| 
 *                      |3V3               SD1| 
 *                      |D4  G2,fL,BIL     SD2| 
 *                      |D3  G0,fL,BIB     SD3| 
 *                      |D2  G4            RSV| 
 *                      |D1  G5            RSV| 
 *                      |D0  G16      ADC0  A0| 
 *                       _____________________
 **/
#ifdef DEVICE_TESTGROUP__BOARDBUILDS__ESP32WROOVER__CAMERA
#ifndef DEVICENAME_CTR
#define DEVICENAME_CTR          "tg_boardbuild__esp32_freenove_wroover"
#endif
#ifndef DEVICENAME_FRIENDLY_CTR
#define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
#endif
#ifndef DEVICENAME_DESCRIPTION_CTR
#define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
#endif
#define DEVICENAME_ROOMHINT_CTR "testgroup"
#define MQTT_HOST   "192.168.3.70"
   
   #define MQTT_PORT     1883
    
/***********************************
 * SECTION: System Debug Options
************************************/    
// ///////////////////////////////////////////// Enable Logs
// // #define DISABLE_SERIAL
// // #define DISABLE_SERIAL0_CORE
// // #define DISABLE_SERIAL_LOGGING
// // #define ENABLE_DEBUG_MANUAL_DELAYS // permits blocking delays

// ///////////////////////////////////////////// System Logs
// // #define ENABLE_ADVANCED_DEBUGGING
// // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
// // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
// // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
// // #define ENABLE_DEBUG_FUNCTION_NAMES
// // 
// // #define SERIAL_LOG_LEVEL_DURING_BOOT 8
// // #define ENABLE_DEBUG_LINE_HERE
// // #define ENABLE_DEBUG_LINE_HERE2
// // #define ENABLE_DEBUG_LINE_HERE3
// // #define ENABLE_DEBUG_LINE_HERE_TRACE
// // #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
// // #define USE_DEBUG_PRINT
// // #define ENABLE_DEBUGFEATURE_LOGS__FORCE_FLUSH_ON_TRANSMIT

// //new feature to only show logs for a specific module when developing code
//  // #define ENABLE_DEBUGFEATURE_LOGGING__RESTRICT_SERIAL_LOGS_TO_MODULE 5028
//  // #define ENABLE_DEBUGFEATURE_LOGGING__RESTRICT_SERIAL_LOGS_TO_MODULE_ARRAY [1, 2]

#define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES

// ///////////////////////////////////////////// Module Logs
// // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS  
// // #define ENABLE_FREERAM_APPENDING_SERIAL

/***********************************
 * SECTION: System Configs
************************************/    

// #define SETTINGS_HOLDER 1239

// 
// 
// #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
//  // until devices can reliably be used without compiling per device
// 
    
/***********************************
* SECTION: Enable with one line (to make it easier to switch on and off for debugging)
************************************/  

#define ENABLE_TEMPLATE_SECTION__DRIVERS__CAMERA_2025

/***********************************
* SECTION: Network Configs
************************************/    


#define USE_MODULE_CORE_FILESYSTEM

 
/***********************************
* SECTION: Driver Configs
************************************/  

  #define USE_MODULE_DRIVERS__CAMERA_2025
  #define WEBCAM_DEV_DEBUG
  #define USE_MODULE_TEMPLATE__CAMERA_FREENOVE_WROOVER
  // #define ENABLE_RTSPSERVER
  #define DEBUG_DRIVERS__CAMERA_2025
  #define ENABLE_DEVFEATURE_ESP32__AUTO_MUTEX

/***********************************
* SECTION: Sensor Configs
************************************/  

/***********************************
 * SECTION: Lighting Configs
************************************/  

/***********************************
* SECTION: Display Configs
************************************/  

/***********************************
 * SECTION: Energy Configs
************************************/  

/***********************************
 * SECTION: Controller Configs
************************************/  

/***********************************
 * SECTION: Module/GPIO Configs
************************************/  

#define USE_MODULE_TEMPLATE
DEFINE_PGM_CTR(MODULE_TEMPLATE) 
"{"
  "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
  "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  "\"" D_GPIO_NUMBER "\":{"
    #ifdef USE_MODULE_SENSORS_BUTTONS
    "\"18\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
    #endif
  "},"
  "\"" D_BASE     "\":\"" D__MODULE_TEMPLATE__CAMERA_FREENOVE_WROOVER__CTR "\","
  "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
"}";

// FutureIdea: Add button press rule is trigger to take photo to SD Card

#endif





/**
 * @brief 
 * 
 * Using IFAN03 hardware as test build for esp8285 boards. Confirming if template loads.
 * 
 *          fH (Boot Fail - Pulled High) → Pin must be LOW at boot, else boot may fail
 *          fL (Boot Fail - Pulled Low) → Pin must be HIGH at boot, else boot may fail
 *          key (Key Pin) → GPIO0 on DOIT DevKit v1 (not )
 *          BIL (Built-in LED) → On some boards, pin is used for onboard LED
 *          BIB (Built-in Button) → On some boards, pin is used for onboard button
 *                               *I ~PWM 'NC    
 *                       _____________________
 *                      |3V3     |USB|     VIN|
 *                      |GND               GND| 
 *                      |TX  G1, fL        RST| 
 *                      |RX  G3             EN| 
 *                      |D8  G15,fL        3V3|
 *                      |D7  G13           GND| 
 *                      |D6  G12           CLK| 
 *                      |D5  G14           SDO| 
 *                      |GND               CMD| 
 *                      |3V3               SD1| 
 *                      |D4  G2,fL,BIL     SD2| 
 *                      |D3  G0,fL,BIB     SD3| 
 *                      |D2  G4            RSV| 
 *                      |D1  G5            RSV| 
 *                      |D0  G16      ADC0  A0| 
 *                       _____________________
 **/
#ifdef DEVICE_TESTGROUP__BOARDBUILDS__ESP32S3_SEEDSTUDIO_XIAO__CAMERA
#ifndef DEVICENAME_CTR
#define DEVICENAME_CTR          "tg_boardbuild__esp32_xiao_camera"
#endif
#ifndef DEVICENAME_FRIENDLY_CTR
#define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
#endif
#ifndef DEVICENAME_DESCRIPTION_CTR
#define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
#endif
#define DEVICENAME_ROOMHINT_CTR "testgroup"
#define MQTT_HOST   "192.168.3.70"
   
   #define MQTT_PORT     1883
  

  
// /***********************************
//  * SECTION: System Debug Options
// ************************************/    
// ///////////////////////////////////////////// Enable Logs
// // #define DISABLE_SERIAL
// // #define DISABLE_SERIAL0_CORE
// // #define DISABLE_SERIAL_LOGGING
// // #define ENABLE_DEBUG_MANUAL_DELAYS // permits blocking delays

// ///////////////////////////////////////////// System Logs
// // #define ENABLE_ADVANCED_DEBUGGING
// // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
// // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
// // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
// // #define ENABLE_DEBUG_FUNCTION_NAMES
// // 
// // #define SERIAL_LOG_LEVEL_DURING_BOOT 8
// // #define ENABLE_DEBUG_LINE_HERE
// // #define ENABLE_DEBUG_LINE_HERE2
// // #define ENABLE_DEBUG_LINE_HERE3
// // #define ENABLE_DEBUG_LINE_HERE_TRACE
// // #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
// // #define USE_DEBUG_PRINT
// // #define ENABLE_DEBUGFEATURE_LOGS__FORCE_FLUSH_ON_TRANSMIT

// //new feature to only show logs for a specific module when developing code
//  // #define ENABLE_DEBUGFEATURE_LOGGING__RESTRICT_SERIAL_LOGS_TO_MODULE 5028
//  // #define ENABLE_DEBUGFEATURE_LOGGING__RESTRICT_SERIAL_LOGS_TO_MODULE_ARRAY [1, 2]

#define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES

// #define ESP32
// #define CONFIG_IDF_TARGET_ESP32C3
// #define ENABLE_DEVFEATURE_GPIO_PIN_METHOD_MAY_2025

#define ENABLE_DEVFEATURE_ESP32__AUTO_MUTEX


// ///////////////////////////////////////////// Module Logs
// // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS  
// // #define ENABLE_FREERAM_APPENDING_SERIAL

// /***********************************
//  * SECTION: System Configs
// ************************************/    

// #define SETTINGS_HOLDER 1239

// 
// 
// #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
//  // until devices can reliably be used without compiling per device
// 
    
// //  /***********************************
// //   * SECTION: Network Configs
// //  ************************************/    

// //  
// //    

// #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

// //  /***********************************
// //   * SECTION: Sensor Configs
// //  ************************************/  

// //  // #define USE_MODULE_SENSORS_INTERFACE  
// //  // #define USE_MODULE_SENSORS_BUTTONS
// //  //   
// //    /**
// //     * @brief 
// //     * Button 1: Single button installs, means {"short":"iter over nice palettes", "long": "iter over 4 brightness levels"}
// //     * Button 2: (Short) Two button installs, this button is iter common effects || (Long) starts a demo and debug mode
// //     **/

  // #define USE_MODULE_DRIVERS__CAMERA_ARDUINO
  // #define CAMERA_MODEL_WROVER_KIT

  // #define USE_MODULE_DRIVERS_CAMERA_OV2640
  // #define USE_MODULE_DRIVERS_CAMERA_OV2640
  #define USE_MODULE_DRIVERS__CAMERA_2025
  // #define CAMERA_MODEL_WROVER_KIT
  #define WEBCAM_DEV_DEBUG

  #define ESP32
  #define CONFIG_IDF_TARGET_ESP32S3 1

  // #define USE_MODULE_TEMPLATE__CAMERA_FREENOVE_WROOVER

  #define USE_MODULE_TEMPLATE__CAMERA_XIAO_ESP32S3_SENSE // not working yet. esp32s3 map is different
  // #define CAMERA_MODEL_XIAO_ESP32S3_SENSE




#define USE_MODULE_CORE_FILESYSTEM


// #define ENABLE_RTSPSERVER

#define DEBUG_DRIVERS__CAMERA_2025
// //  /***********************************
// //   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
// //  ************************************/  

// // //  #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L0X
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L1X
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__ULTRASONIC
// #define ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_24GHZ
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__ULTRASONIC
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__PIR_SMALL
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__PIR_LARGE
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ


// // // #define ENABLE_TEMPLATE_SECTION__DRIVERS__AUDIO_SPEAKER
// // // #define ENABLE_TEMPLATE_SECTION__DRIVERS__AUDIO_BUZZER

// // // #define ENABLE_TEMPLATE_SECTION__DISPLAYS__OLED

// // //  // #define ENABLE_TEMPLATE_SECTION__LIGHTING
// // //  #define ENABLE_TEMPLATE_SECTION__ENERGY
// // //  #define ENABLE_TEMPLATE_SECTION__ENERGY__PZEM

// #define USE_MODULE_DRIVERS_LEDS  

// //  /***********************************
// //   * SECTION: Sensor Configs
// //  ************************************/  

// // //  #define USE_MODULE_SENSORS_INTERFACE
// //  #if defined(ENABLE_TEMPLATE_SECTION__SENSORS__MOTION) || defined(ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ)
// //    #define USE_MODULE_SENSORS_INTERFACE
// //    #define USE_MODULE_SENSORS_PIR
// //     //  #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT
// //  #endif
// // //  #define USE_MODULE_SENSORS_BUTTONS
// // //    #define ENABLE_DEVFATURE_BUTTON__REMOVE_MQTT_BUTTONS
// // //    #define SOC_TOUCH_VERSION_1

// //   #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__ULTRASONIC
// //    #define USE_MODULE_SENSORS_SR04
// //    #define ENABLE_DEVFEATURE_SR04_FILTERING_EMA
// //    #define ENABLE_DEVFEATURE_SR04_FILTERING_DEMA
// //   #endif

//  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_24GHZ
//  #define USE_MODULE_SENSORS__RADAR_HLK_LD2410
//  #define ENABLE_FEATURE_HLK_LD2410__USE_SERIAL_CHUNK_MODE
//  #endif

// //   #define ENABLE_ADVANCED_DEBUGGING
// //   #define ENABLE_DEBUG_FUNCTION_NAMES
// //   #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
// //   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
// //   // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
// //   #define ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
// //   #define ENABLE_SERIAL_FLUSH
// //   #define DEBUG_FASTBOOT
// //   #define ENABLE_DEBUG_LINE_HERE


// #define USE_MODULE_SENSORS_INTERFACE
//   #define USE_MODULE_SENSORS_BH1750

// // //  #define USE_MODULE_DRIVERS_LEDS  

// //  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L0X
// //   #define USE_MODULE_SENSORS__TOF_VL53L0X
// //   #define ENABLE_DEVFEATURE_I2C__SET_WIRE_INSTANCE_WITH_TWOWIRE_ZERO
// //   #define VL53L0X_LONG_RANGE
// //  #endif
// //  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L1X
// //   #define USE_MODULE_SENSORS__TOF_VL53L1X
// //   #define ENABLE_DEVFEATURE_I2C__SET_WIRE_INSTANCE_WITH_TWOWIRE_ZERO
// //   // #define USE_SENSORS_TOFVL_AVERAGING_DATA
// //  #endif

// //  #define ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING

// //  #define ENABLE_DEVFEATURE_SENSORS__TOF_BOTH_VL53_ACTIVE_ON_SHARED_ADD29

// /***********************************
//  * SECTION: Lighting Configs
// ************************************/  

// //  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_JANUARY_2025_NO_GPIO

// 


// #define USE_LIGHTING_TEMPLATE
// DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
// R"=====(
// {
//   "BusConfig":[
//     {
//       "Pin":23,
//       "ColourOrder":"GRBW",
//       "BusType":"SK6812_RGBW",
//       "Start":0,
//       "Length":9
//     }
//   ],
//   "Segment0": {
//     "Name":"Door Edge",
//     "PixelRange": [
//       0,
//       9
//     ],
//     "ColourPalette":"Rainbow 16",
//     "Effects": {
//       "Function":"Static",
//       "Speed":255,
//       "Intensity":0,
//       "Grouping":1,
//       "RateMs": 1000
//     },
//     "BrightnessRGB": 100,
//     "BrightnessCCT": 100
//   }
//   "BrightnessRGB": 5
// }
// )=====";

//  /***********************************
//   * SECTION: Display Configs
//  ************************************/  
//  #ifdef ENABLE_TEMPLATE_SECTION__DISPLAYS__OLED
//    #define USE_MODULE_DISPLAYS_INTERFACE
//    #define USE_MODULE_DISPLAYS_OLED_SH1106
//      #define SHOW_SPLASH
//  #endif

// /***********************************
//  * SECTION: Energy Configs
// ************************************/  

// /***********************************
//  * SECTION: Controller Configs
// ************************************/  

// // #define USE_MODULE_CONTROLLER_CUSTOM__LIGHTNEO_MOTION_ALERTS

// /***********************************
//  * SECTION: Module/GPIO Configs
// ************************************/  

#define USE_MODULE_TEMPLATE
DEFINE_PGM_CTR(MODULE_TEMPLATE) 
"{"
  "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
  "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  "\"" D_GPIO_NUMBER "\":{"          
   //  "\"16\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
   //  "\"17\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
    // #if defined(USE_MODULE_SENSORS__TOF_VL53L0X) || defined(USE_MODULE_SENSORS__TOF_VL53L1X) || defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750) || defined(USE_MODULE_ENERGY_INA219) || defined(USE_MODULE_DISPLAYS_OLED_SH1106)
    // "\"8\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
    // "\"9\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\""    
    // #endif
    #ifdef USE_MODULE_SENSORS_PIR
   //  "\"23\":\""  D_GPIO_FUNCTION_PIR_1_CTR "\","
    #endif
    #ifdef USE_MODULE_SENSORS__TOF_VL53L0X
    "\"33\":\""  D_GPIO_FUNCTION__TOF_VL53L0X_XSHUT1__CTR "\","
   //  "\"33\":\""  D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR "\"," // Connected to XSHUT but not wanted. HIGH for remain enabled
   //  "\"26\":\""  D_GPIO_FUNCTION_UNUSED_FORCED_LOW_CTR "\"," // Connected to XSHUT but not wanted. HIGH for remain enabled
    #endif
    #ifdef USE_MODULE_SENSORS__TOF_VL53L1X
     // "\"26\":\""  D_GPIO_FUNCTION__TOF_VL53L1X_XSHUT1__CTR "\"," // turned off only for testing new sensor interface, needed for dual TOF use
   //  "\"33\":\""  D_GPIO_FUNCTION_UNUSED_FORCED_LOW_CTR "\"," // Connected to XSHUT but not wanted. HIGH for remain enabled
    #endif
    #ifdef USE_MODULE_SENSORS_SR04
    "\"4\":\"" D_GPIO_FUNCTION_SR04_ECHO_CTR   "\","
    "\"2\":\"" D_GPIO_FUNCTION_SR04_TRIG_CTR  "\","  
    #endif 
    #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ
    "\"35\":\""  D_GPIO_FUNCTION_PIR_2_INV_CTR "\","
    #endif
    #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__PIR_LARGE
    "\"34\":\""  D_GPIO_FUNCTION_PIR_1_INV_CTR "\","
    #endif
    #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__PIR_SMALL
    "\"5\":\""  D_GPIO_FUNCTION_PIR_3_INV_CTR "\","
    #endif
    #ifdef USE_MODULE_SENSORS__RADAR_HLK_LD2410
    "\"17\":\""  D_GPIO_FUNCTION__HLK_LD2410_TX__CTR "\","
    "\"16\":\""  D_GPIO_FUNCTION__HLK_LD2410_RX__CTR "\","
    #endif
   //  #ifdef USE_MODULE_SENSORS__TOF_VL53L0X
   //  "\"27\":\""  D_GPIO_FUNCTION__TOF_VL53L1X_XSHUT__CTR "\","
   //  #endif
    #ifdef USE_MODULE_SENSORS_BUTTONS
   //  "\"18\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
   //  "\"19\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
   //  "\"33\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\","
    #endif
    
   //  "\"4\":\"" D_GPIO_FUNCTION_LED1_CTR  "\","
   //  "\"5\":\"" D_GPIO_FUNCTION_LED2_CTR  "\","
    // "\"8\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
  "},"
  // "\"" D_BASE     "\":\"" D__MODULE_TEMPLATE__CAMERA_FREENOVE_WROOVER__CTR "\","
  "\"" D_BASE     "\":\"" D__MODULE_TEMPLATE__CAMERA_XIAO_ESP32S3_SENSE__CTR "\","
  "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
"}";

//  #define ENABLE_DEVFEATURE_MQTT__PUBLUSH_TASMOTA_METHODS
//  #define ENABLE_FEATURE_BUTTON__FACTORY_RESET_WITH_LONG_HOLD


// /***********************************
//  * SECTION: TEMPLATE: Names
// ************************************/    

// #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "PIRLarge"
// #define D_DEVICE_SENSOR_MOTION1_FRIENDLY_NAME_LONG "RADAR 3p18GHz"
// #define D_DEVICE_SENSOR_MOTION2_FRIENDLY_NAME_LONG "PIRSmall"


// #define D_DEVICE_SENSOR_CLIMATE "PIRLarge"




// #define D_DEVICE_SENSOR_BME_280_NAME "BME280"
// #define D_DEVICE_SENSOR_BME_680_NAME "BME680"

// #define D_DEVICE_SENSOR_BH1750_NAME "Ambient"


// #define USE_FUNCTION_TEMPLATE
// DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
// "{"
//   "\"" D_DEVICENAME "\":{"
//     "\"" D_MODULE_SENSORS_PIR_CTR "\":["
//       "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\","
//       "\"" D_DEVICE_SENSOR_MOTION1_FRIENDLY_NAME_LONG "\","
//       "\"" D_DEVICE_SENSOR_MOTION2_FRIENDLY_NAME_LONG "\""
//     "],"
//     "\"" D_MODULE_SENSORS__TOF_VL53L0X__CTR "\":["
//       "\"" "TOF_VL53L0X" "\""
//     "],"
//     "\"" D_MODULE_SENSORS__TOF_VL53L1X__CTR "\":["
//       "\"" "TOF_VL53L1X" "\""
//     "],"
//     "\"" D_MODULE_SENSORS_SR04_CTR "\":["
//       "\"" "SRO4" "\""
//     "],"
//     "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
//       "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
//     "],"
//     "\"" D_MODULE_SENSORS_BUTTONS_CTR "\":["
//       "\"" "WallRed" "\","
//       "\"" "WallBlue" "\","
//       "\"" "DoorAlert" "\""
//     "],"
//     "\"" D_MODULE_DRIVERS_LEDS_CTR "\":["
//       "\"" "WallRed" "\","
//       "\"" "WallBlue" "\","
//       "\"" "StatusLED" "\""
//     "],"
//     "\"" D_MODULE_SENSORS_BME_CTR "\":["
//       "\"" D_DEVICE_SENSOR_CLIMATE "\""
//     "]"
//   "},"
//   "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60}," 
//   "\"MQTT_Interface_Priority\":{\"" D_MODULE_ENERGY_INTERFACE_CTR "\":1}" // Each interface will have ability to reduce its subclass mqtt "ifchanged" rate
// "}";


#endif





/**
 * @brief 
 * 
 * Using IFAN03 hardware as test build for esp8285 boards. Confirming if template loads.
 * 
 *          fH (Boot Fail - Pulled High) → Pin must be LOW at boot, else boot may fail
 *          fL (Boot Fail - Pulled Low) → Pin must be HIGH at boot, else boot may fail
 *          key (Key Pin) → GPIO0 on DOIT DevKit v1 (not )
 *          BIL (Built-in LED) → On some boards, pin is used for onboard LED
 *          BIB (Built-in Button) → On some boards, pin is used for onboard button
 *                               *I ~PWM 'NC    
 *                       _____________________
 *                      |3V3     |USB|     VIN|
 *                      |GND               GND| 
 *                      |TX  G1, fL        RST| 
 *                      |RX  G3             EN| 
 *                      |D8  G15,fL        3V3|
 *                      |D7  G13           GND| 
 *                      |D6  G12           CLK| 
 *                      |D5  G14           SDO| 
 *                      |GND               CMD| 
 *                      |3V3               SD1| 
 *                      |D4  G2,fL,BIL     SD2| 
 *                      |D3  G0,fL,BIB     SD3| 
 *                      |D2  G4            RSV| 
 *                      |D1  G5            RSV| 
 *                      |D0  G16      ADC0  A0| 
 *                       _____________________
 **/
#ifdef DEVICE_TESTGROUP__BOARDBUILDS__ESP32_CAM_EYE
#ifndef DEVICENAME_CTR
#define DEVICENAME_CTR          "tg_boardbuild__esp32_cam_eye"
#endif
#ifndef DEVICENAME_FRIENDLY_CTR
#define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
#endif
#ifndef DEVICENAME_DESCRIPTION_CTR
#define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
#endif
#define DEVICENAME_ROOMHINT_CTR "testgroup"
#define MQTT_HOST   "192.168.3.70"
   
   #define MQTT_PORT     1883
  

  
// /***********************************
//  * SECTION: System Debug Options
// ************************************/    
// ///////////////////////////////////////////// Enable Logs
// // #define DISABLE_SERIAL
// // #define DISABLE_SERIAL0_CORE
// // #define DISABLE_SERIAL_LOGGING
// // #define ENABLE_DEBUG_MANUAL_DELAYS // permits blocking delays

// ///////////////////////////////////////////// System Logs
// // #define ENABLE_ADVANCED_DEBUGGING
// // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
// // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
// // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
// // #define ENABLE_DEBUG_FUNCTION_NAMES
// // 
// // #define SERIAL_LOG_LEVEL_DURING_BOOT 8
// // #define ENABLE_DEBUG_LINE_HERE
// // #define ENABLE_DEBUG_LINE_HERE2
// // #define ENABLE_DEBUG_LINE_HERE3
// // #define ENABLE_DEBUG_LINE_HERE_TRACE
// // #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
// // #define USE_DEBUG_PRINT
// // #define ENABLE_DEBUGFEATURE_LOGS__FORCE_FLUSH_ON_TRANSMIT

// //new feature to only show logs for a specific module when developing code
//  // #define ENABLE_DEBUGFEATURE_LOGGING__RESTRICT_SERIAL_LOGS_TO_MODULE 5028
//  // #define ENABLE_DEBUGFEATURE_LOGGING__RESTRICT_SERIAL_LOGS_TO_MODULE_ARRAY [1, 2]

#define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES

// #define ESP32
// #define CONFIG_IDF_TARGET_ESP32C3
// #define ENABLE_DEVFEATURE_GPIO_PIN_METHOD_MAY_2025

#define ENABLE_DEVFEATURE_ESP32__AUTO_MUTEX

#define ENABLE_DEBUGFEATURE_TELEMETRY__MQTT_SEND_HEALTH_EVERY_SECOND

#define ENABLE_LOGGING_ADDLOG__MESSAGES_OVER_MQTT

// ///////////////////////////////////////////// Module Logs
// // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS  
// // #define ENABLE_FREERAM_APPENDING_SERIAL

// /***********************************
//  * SECTION: System Configs
// ************************************/    

// #define SETTINGS_HOLDER 1239

// 
// 
// #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
//  // until devices can reliably be used without compiling per device
// 
    
// //  /***********************************
// //   * SECTION: Network Configs
// //  ************************************/    

// //  
// //    

// #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

// //  /***********************************
// //   * SECTION: Sensor Configs
// //  ************************************/  

// //  // #define USE_MODULE_SENSORS_INTERFACE  
// //  // #define USE_MODULE_SENSORS_BUTTONS
// //  //   
// //    /**
// //     * @brief 
// //     * Button 1: Single button installs, means {"short":"iter over nice palettes", "long": "iter over 4 brightness levels"}
// //     * Button 2: (Short) Two button installs, this button is iter common effects || (Long) starts a demo and debug mode
// //     **/

  // #define USE_MODULE_DRIVERS__CAMERA_ARDUINO
  // #define CAMERA_MODEL_WROVER_KIT

  // #define USE_MODULE_DRIVERS_CAMERA_OV2640
  // #define USE_MODULE_DRIVERS_CAMERA_OV2640
  #define USE_MODULE_DRIVERS__CAMERA_2025
  // #define CAMERA_MODEL_WROVER_KIT
  #define WEBCAM_DEV_DEBUG

  // #define USE_MODULE_TEMPLATE__CAMERA_FREENOVE_WROOVER
  #define CAMERA_MODEL_AI_THINKER

  // #define ENABLE_DEBUG_SPLASH__PSRAM_USAGE

  

#define USE_MODULE_CORE_FILESYSTEM



// // #define ENABLE_DEBUGFEATURE_LOGGING__ENABLE_TELNET_IMMEDIATE_WITH_WAIT

// // #define ENABLE_RTSPSERVER

// #define DEBUG_DRIVERS__CAMERA_2025
// //  /***********************************
// //   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
// //  ************************************/  

// // //  #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L0X
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L1X
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__ULTRASONIC
// #define ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_24GHZ
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__ULTRASONIC
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__PIR_SMALL
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__PIR_LARGE
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ


// // // #define ENABLE_TEMPLATE_SECTION__DRIVERS__AUDIO_SPEAKER
// // // #define ENABLE_TEMPLATE_SECTION__DRIVERS__AUDIO_BUZZER

// // // #define ENABLE_TEMPLATE_SECTION__DISPLAYS__OLED

// // //  // #define ENABLE_TEMPLATE_SECTION__LIGHTING
// // //  #define ENABLE_TEMPLATE_SECTION__ENERGY
// // //  #define ENABLE_TEMPLATE_SECTION__ENERGY__PZEM

// #define USE_MODULE_DRIVERS_LEDS  

// //  /***********************************
// //   * SECTION: Sensor Configs
// //  ************************************/  

// // //  #define USE_MODULE_SENSORS_INTERFACE
// //  #if defined(ENABLE_TEMPLATE_SECTION__SENSORS__MOTION) || defined(ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ)
// //    #define USE_MODULE_SENSORS_INTERFACE
// //    #define USE_MODULE_SENSORS_PIR
// //     //  #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT
// //  #endif
// // //  #define USE_MODULE_SENSORS_BUTTONS
// // //    #define ENABLE_DEVFATURE_BUTTON__REMOVE_MQTT_BUTTONS
// // //    #define SOC_TOUCH_VERSION_1

// //   #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__ULTRASONIC
// //    #define USE_MODULE_SENSORS_SR04
// //    #define ENABLE_DEVFEATURE_SR04_FILTERING_EMA
// //    #define ENABLE_DEVFEATURE_SR04_FILTERING_DEMA
// //   #endif

//  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_24GHZ
//  #define USE_MODULE_SENSORS__RADAR_HLK_LD2410
//  #define ENABLE_FEATURE_HLK_LD2410__USE_SERIAL_CHUNK_MODE
//  #endif

// //   #define ENABLE_ADVANCED_DEBUGGING
// //   #define ENABLE_DEBUG_FUNCTION_NAMES
// //   #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
// //   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
// //   // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
// //   #define ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
// //   #define ENABLE_SERIAL_FLUSH
// //   #define DEBUG_FASTBOOT
// //   #define ENABLE_DEBUG_LINE_HERE


// #define USE_MODULE_SENSORS_INTERFACE
//   #define USE_MODULE_SENSORS_BH1750

// // //  #define USE_MODULE_DRIVERS_LEDS  

// //  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L0X
// //   #define USE_MODULE_SENSORS__TOF_VL53L0X
// //   #define ENABLE_DEVFEATURE_I2C__SET_WIRE_INSTANCE_WITH_TWOWIRE_ZERO
// //   #define VL53L0X_LONG_RANGE
// //  #endif
// //  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L1X
// //   #define USE_MODULE_SENSORS__TOF_VL53L1X
// //   #define ENABLE_DEVFEATURE_I2C__SET_WIRE_INSTANCE_WITH_TWOWIRE_ZERO
// //   // #define USE_SENSORS_TOFVL_AVERAGING_DATA
// //  #endif

// //  #define ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING

// //  #define ENABLE_DEVFEATURE_SENSORS__TOF_BOTH_VL53_ACTIVE_ON_SHARED_ADD29

// /***********************************
//  * SECTION: Lighting Configs
// ************************************/  

// //  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_JANUARY_2025_NO_GPIO

// 


// #define USE_LIGHTING_TEMPLATE
// DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
// R"=====(
// {
//   "BusConfig":[
//     {
//       "Pin":23,
//       "ColourOrder":"GRBW",
//       "BusType":"SK6812_RGBW",
//       "Start":0,
//       "Length":9
//     }
//   ],
//   "Segment0": {
//     "Name":"Door Edge",
//     "PixelRange": [
//       0,
//       9
//     ],
//     "ColourPalette":"Rainbow 16",
//     "Effects": {
//       "Function":"Static",
//       "Speed":255,
//       "Intensity":0,
//       "Grouping":1,
//       "RateMs": 1000
//     },
//     "BrightnessRGB": 100,
//     "BrightnessCCT": 100
//   }
//   "BrightnessRGB": 5
// }
// )=====";

//  /***********************************
//   * SECTION: Display Configs
//  ************************************/  
//  #ifdef ENABLE_TEMPLATE_SECTION__DISPLAYS__OLED
//    #define USE_MODULE_DISPLAYS_INTERFACE
//    #define USE_MODULE_DISPLAYS_OLED_SH1106
//      #define SHOW_SPLASH
//  #endif

// /***********************************
//  * SECTION: Energy Configs
// ************************************/  

// /***********************************
//  * SECTION: Controller Configs
// ************************************/  

// // #define USE_MODULE_CONTROLLER_CUSTOM__LIGHTNEO_MOTION_ALERTS

// /***********************************
//  * SECTION: Module/GPIO Configs
// ************************************/  

#define USE_MODULE_TEMPLATE
DEFINE_PGM_CTR(MODULE_TEMPLATE) 
"{"
  "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
  "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  "\"" D_GPIO_NUMBER "\":{"          
   //  "\"16\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
   //  "\"17\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
    // #if defined(USE_MODULE_SENSORS__TOF_VL53L0X) || defined(USE_MODULE_SENSORS__TOF_VL53L1X) || defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750) || defined(USE_MODULE_ENERGY_INA219) || defined(USE_MODULE_DISPLAYS_OLED_SH1106)
    // "\"8\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
    // "\"9\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\""    
    // #endif
    #ifdef USE_MODULE_SENSORS_PIR
   //  "\"23\":\""  D_GPIO_FUNCTION_PIR_1_CTR "\","
    #endif
    #ifdef USE_MODULE_SENSORS__TOF_VL53L0X
    "\"33\":\""  D_GPIO_FUNCTION__TOF_VL53L0X_XSHUT1__CTR "\","
   //  "\"33\":\""  D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR "\"," // Connected to XSHUT but not wanted. HIGH for remain enabled
   //  "\"26\":\""  D_GPIO_FUNCTION_UNUSED_FORCED_LOW_CTR "\"," // Connected to XSHUT but not wanted. HIGH for remain enabled
    #endif
    #ifdef USE_MODULE_SENSORS__TOF_VL53L1X
     // "\"26\":\""  D_GPIO_FUNCTION__TOF_VL53L1X_XSHUT1__CTR "\"," // turned off only for testing new sensor interface, needed for dual TOF use
   //  "\"33\":\""  D_GPIO_FUNCTION_UNUSED_FORCED_LOW_CTR "\"," // Connected to XSHUT but not wanted. HIGH for remain enabled
    #endif
    #ifdef USE_MODULE_SENSORS_SR04
    "\"4\":\"" D_GPIO_FUNCTION_SR04_ECHO_CTR   "\","
    "\"2\":\"" D_GPIO_FUNCTION_SR04_TRIG_CTR  "\","  
    #endif 
    #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ
    "\"35\":\""  D_GPIO_FUNCTION_PIR_2_INV_CTR "\","
    #endif
    #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__PIR_LARGE
    "\"34\":\""  D_GPIO_FUNCTION_PIR_1_INV_CTR "\","
    #endif
    #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__PIR_SMALL
    "\"5\":\""  D_GPIO_FUNCTION_PIR_3_INV_CTR "\","
    #endif
    #ifdef USE_MODULE_SENSORS__RADAR_HLK_LD2410
    "\"17\":\""  D_GPIO_FUNCTION__HLK_LD2410_TX__CTR "\","
    "\"16\":\""  D_GPIO_FUNCTION__HLK_LD2410_RX__CTR "\","
    #endif
   //  #ifdef USE_MODULE_SENSORS__TOF_VL53L0X
   //  "\"27\":\""  D_GPIO_FUNCTION__TOF_VL53L1X_XSHUT__CTR "\","
   //  #endif
    #ifdef USE_MODULE_SENSORS_BUTTONS
   //  "\"18\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
   //  "\"19\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
   //  "\"33\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\","
    #endif
    
   //  "\"4\":\"" D_GPIO_FUNCTION_LED1_CTR  "\","
   //  "\"5\":\"" D_GPIO_FUNCTION_LED2_CTR  "\","
    // "\"8\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
  "},"
  "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
  "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
"}";

//  #define ENABLE_DEVFEATURE_MQTT__PUBLUSH_TASMOTA_METHODS
//  #define ENABLE_FEATURE_BUTTON__FACTORY_RESET_WITH_LONG_HOLD


// /***********************************
//  * SECTION: TEMPLATE: Names
// ************************************/    

// #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "PIRLarge"
// #define D_DEVICE_SENSOR_MOTION1_FRIENDLY_NAME_LONG "RADAR 3p18GHz"
// #define D_DEVICE_SENSOR_MOTION2_FRIENDLY_NAME_LONG "PIRSmall"


// #define D_DEVICE_SENSOR_CLIMATE "PIRLarge"




// #define D_DEVICE_SENSOR_BME_280_NAME "BME280"
// #define D_DEVICE_SENSOR_BME_680_NAME "BME680"

// #define D_DEVICE_SENSOR_BH1750_NAME "Ambient"


// #define USE_FUNCTION_TEMPLATE
// DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
// "{"
//   "\"" D_DEVICENAME "\":{"
//     "\"" D_MODULE_SENSORS_PIR_CTR "\":["
//       "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\","
//       "\"" D_DEVICE_SENSOR_MOTION1_FRIENDLY_NAME_LONG "\","
//       "\"" D_DEVICE_SENSOR_MOTION2_FRIENDLY_NAME_LONG "\""
//     "],"
//     "\"" D_MODULE_SENSORS__TOF_VL53L0X__CTR "\":["
//       "\"" "TOF_VL53L0X" "\""
//     "],"
//     "\"" D_MODULE_SENSORS__TOF_VL53L1X__CTR "\":["
//       "\"" "TOF_VL53L1X" "\""
//     "],"
//     "\"" D_MODULE_SENSORS_SR04_CTR "\":["
//       "\"" "SRO4" "\""
//     "],"
//     "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
//       "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
//     "],"
//     "\"" D_MODULE_SENSORS_BUTTONS_CTR "\":["
//       "\"" "WallRed" "\","
//       "\"" "WallBlue" "\","
//       "\"" "DoorAlert" "\""
//     "],"
//     "\"" D_MODULE_DRIVERS_LEDS_CTR "\":["
//       "\"" "WallRed" "\","
//       "\"" "WallBlue" "\","
//       "\"" "StatusLED" "\""
//     "],"
//     "\"" D_MODULE_SENSORS_BME_CTR "\":["
//       "\"" D_DEVICE_SENSOR_CLIMATE "\""
//     "]"
//   "},"
//   "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60}," 
//   "\"MQTT_Interface_Priority\":{\"" D_MODULE_ENERGY_INTERFACE_CTR "\":1}" // Each interface will have ability to reduce its subclass mqtt "ifchanged" rate
// "}";


#endif






#endif // _CONFIG__FIRMWARE_GROUP__BOARDBUILDS_H

