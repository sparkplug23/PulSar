#ifndef _CONFIG__FIRMWARE_GROUP__CAMERAS_H
#define _CONFIG__FIRMWARE_GROUP__CAMERAS_H

/*********************************************************************************************\
 * Developing/Testbeds: 
 *  ** Only devices that are unique with pzem style
 *  ** So not the consumer unit, but standalone basic MOTION_DETECTORS devices that may have additional sensors but are many for 
 *     tracking or controller power (eg treadmill, christmas lights)
 *  ** Will include 100A and 10A variants.
\*********************************************************************************************/

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"

//--------------------------------[Enable Device]-------------------------------------

//    ;;;;;;;;;;;; ESP32 ;;;;;;;;;;;;;;;;
// #define DEVICE_TESTBED__BOARDTEST_ESP32S3__XIAO_ESP32_S3_SEEDSTUDIO
#define DEVICE_TESTGROUP__CAMERAS__WROOVER_E_LARGE_ESP32_BOARD_01
// #define DEVICE_TESTGROUP__CAMERAS__WROOVER_E_LARGE_ESP32_BOARD_02
// #define DEVICE_TESTGROUP__CAMERAS__WROOVER_E_LARGE_ESP32_BOARD_03
// #define DEVICE_GROUP__CAMERAS__ESP32_CAM_EYE_SENSOR_01

//--------------------------------[Enable Device]-------------------------------------

#ifdef DEVICE_TESTGROUP__CAMERAS__WROOVER_E_LARGE_ESP32_BOARD_01
#ifndef DEVICENAME_CTR
#define DEVICENAME_CTR          "template"
#endif
#ifndef DEVICENAME_FRIENDLY_CTR
#define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
#endif
#define DEVICENAME_ROOMHINT_CTR "testgroup"
   #define MQTT_PORT     1883

  //  #define ENABLE_DEBUG_LINE_HERE3

  //  #define ENABLE_DEBUGFEATURE_TIME__SHOW_UPTIME_EVERY_SECOND


/***********************************
* SECTION: Network Configs
************************************/  

#define ENABLE_DEBUGFEATURE_WEBSERVER_URL_LIST
#define FIRMWARE_DEFAULT__INCLUDE_WEBSERVER_BASIC
  
/***********************************
* SECTION: Driver Configs
************************************/  

#define ESP32

#define USE_MODULE_CORE_FILESYSTEM

#define USE_MODULE_DRIVERS_INTERFACE

// #define USE_MODULE_DRIVERS_SDCARD

// #define USE_MODULE_DRIVERS_SDCARD
// #define ENABLE_FEATURE_FILESYSTEM__SDCARD_MMC

#define USE_MODULE_DRIVERS__CAMERA
#define CAMERA_MODEL_WROVER_KIT
#define ENABLE_DEVFEATURE_ESP32__AUTO_MUTEX
#define ENABLE_RTSPSERVER


// #define ENABLE_DEBUGFEATURE_SPLASH__PIN_ALLOCATIONS

// #define USE_MODULE_CORE_PINVIEWER

/***********************************
* SECTION: Sensors Configs
************************************/  

// #define USE_MODULE_SENSORS_INTERFACE

// #define USE_MODULE_SENSORS_ESP32_TEMPERATURE
// #define ENABLE_FEATURE_INTERNAL_CHIP_TEMPERATURE__ALLOW_CLASSIC_ESP32


/***********************************
 * SECTION: Module/GPIO Configs
************************************/  

#define USE_MODULE_TEMPLATE
DEFINE_PGM_CTR(MODULE_TEMPLATE) 
"{"
  "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
  "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  "\"" D_GPIO_NUMBER "\":{"
    #ifdef USE_MODULE_DRIVERS_SDCARD
    "\"15\":\"" D_GPIO_SDCARD_MMC_CMD_CTR  "\","
    "\"14\":\"" D_GPIO_SDCARD_MMC_CLK_CTR   "\","   
    "\"2\":\"" D_GPIO_SDCARD_MMC_D0_CTR   "\""
    #endif
  "},"
  "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
  "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
"}";

/***********************************
 * SECTION: Templates
************************************/    

// #define USE_FUNCTION_TEMPLATE
// DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
// "{"
//   "\"" D_MODULE_DRIVERS__CAMERA_CTR "\":{"
//     "\"Mirror\":0,"
//     "\"Flip\":0,"
//     "\"AWB\":0,"
//     "\"Resolution\":12" //12 max
//   "}"
// "}";

#define USE_NETWORK_CONFIG_TEMPLATE
  DEFINE_PGM_CTR(NETWORK_CONFIG_TEMPLATE)
  "{"
    "\"Version\":1,"

    "\"Interface\":{"
      "\"Policy\":{"
        "\"PreferOrder\":[\"Ethernet\",\"WiFi\",\"Cellular\"],"
        "\"AllowMultipleActive\":true,"
        "\"BlockRemoteMqttWhenLocalAvailable\":true"
      "}"
    "},"

    "\"WiFi\":{"
      "\"EN\":true,"
      "\"Backoff\":[5,60,600],"

      "\"Mode\":{"
        "\"STA\":true,"
        "\"AP\":true,"
        "\"STA_AP\":true,"
        "\"APBootMins\":10,"
        "\"APOnSTAFail\":true,"
        "\"APFailDelayMins\":0,"
        "\"APAlwaysOn\":false"
      "},"

      "\"Station\":{"
        "\"Profiles\":["
          "{"
            "\"SSID\":\"" STA_SSID1 "\","
            "\"Password\":\"" STA_PASS1 "\""
          "},"
          "{"
            "\"SSID\":\"" STA_SSID3 "\","
            "\"Password\":\"" STA_PASS3 "\""
          "}"
        "],"

        "\"IPv4\":{"
          "\"Static\":false,"
          "\"IP\":\"0.0.0.0\","
          "\"Gateway\":\"0.0.0.0\","
          "\"Subnet\":\"0.0.0.0\","
          "\"DNS1\":\"0.0.0.0\","
          "\"DNS2\":\"0.0.0.0\""
        "}"
      "},"

      "\"SoftAP\":{"
        "\"SSID\":\"" SOFTAP_SSID "\","
        "\"Password\":\"" SOFTAP_PASSWORD "\","
        "\"Channel\":1"
      "}"
    "},"

    "\"Ethernet\":{"
      "\"EN\":true,"
      "\"Backoff\":[5,30,60],"

      "\"IPv4\":{"
        "\"Static\":false,"
        "\"IP\":\"0.0.0.0\","
        "\"Gateway\":\"0.0.0.0\","
        "\"Subnet\":\"0.0.0.0\","
        "\"DNS1\":\"0.0.0.0\","
        "\"DNS2\":\"0.0.0.0\""
      "}"
    "},"

    "\"Cellular\":{"
      "\"EN\":true,"
      "\"Backoff\":[10,60,600],"

      "\"Modem\":{"
        "\"EN\":true,"
        "\"APN\":\"" CELLULAR_APN "\","
        "\"User\":\"\","
        "\"Password\":\"\""
      "},"

      "\"GNSS\":{"
        "\"EN\":true"
      "},"

      "\"SMS\":{"
        "\"EN\":true,"
        "\"PrivilegedOnly\":true"
      "}"
    "},"

    "\"MQTT\":{"
      "\"EN\":true,"

      "\"UpdateSeconds\":{"
        "\"IfChanged\":1,"
        "\"TelePeriod\":60,"
        "\"ConfigPeriod\":60"
      "},"

      "\"Brokers\":["
        "{"
          "\"Id\":\"home\","
          "\"EN\":true,"
          "\"Host\":\"" MQTT_HOST "\","
          "\"Port\":" STR(MQTT_PORT) ","
          "\"User\":\"\","
          "\"Password\":\"\","
          "\"TopicPrefix\":\"" DEVICENAME_CTR "\","
          "\"ClientName\":\"" DEVICENAME_CTR "\","
          "\"Backoff\":[5,10,60],"
          "\"Transport\":[\"Ethernet\",\"WiFi\"],"
          "\"PrefTransport\":[\"Ethernet\",\"WiFi\"],"
          "\"OutgoingLevel\":3,"
          "\"OutgoingLimiterMs\":0"
        "}"
      "]"
    "}"
  "}";
#endif


#ifdef DEVICE_TESTGROUP__CAMERAS__WROOVER_E_LARGE_ESP32_BOARD_02
#ifndef DEVICENAME_CTR
#define DEVICENAME_CTR          "template"
#endif
#ifndef DEVICENAME_FRIENDLY_CTR
#define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
#endif
#define DEVICENAME_ROOMHINT_CTR "testgroup"
   #define MQTT_PORT     1883

/***********************************
* SECTION: Network Configs
************************************/  

#define ENABLE_DEBUGFEATURE_WEBSERVER_URL_LIST
#define FIRMWARE_DEFAULT__INCLUDE_WEBSERVER_BASIC
  
/***********************************
* SECTION: Driver Configs
************************************/  

#define ESP32

#define USE_MODULE_CORE_FILESYSTEM

#define USE_MODULE_DRIVERS_INTERFACE

#define USE_MODULE_DRIVERS_SDCARD

// #define USE_MODULE_DRIVERS_SDCARD
#define ENABLE_FEATURE_FILESYSTEM__SDCARD_MMC

#define USE_MODULE_DRIVERS__CAMERA
#define CAMERA_MODEL_WROVER_KIT
#define ENABLE_DEVFEATURE_ESP32__AUTO_MUTEX
#define ENABLE_RTSPSERVER


// #define ENABLE_DEBUGFEATURE_SPLASH__PIN_ALLOCATIONS

// #define USE_MODULE_CORE_PINVIEWER

/***********************************
* SECTION: Sensors Configs
************************************/  

#define USE_MODULE_SENSORS_INTERFACE

#define USE_MODULE_SENSORS_ESP32_TEMPERATURE
#define ENABLE_FEATURE_INTERNAL_CHIP_TEMPERATURE__ALLOW_CLASSIC_ESP32


/***********************************
 * SECTION: Module/GPIO Configs
************************************/  

#define USE_MODULE_TEMPLATE
DEFINE_PGM_CTR(MODULE_TEMPLATE) 
"{"
  "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
  "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  "\"" D_GPIO_NUMBER "\":{"
    #ifdef USE_MODULE_DRIVERS_SDCARD
    "\"15\":\"" D_GPIO_SDCARD_MMC_CMD_CTR  "\","
    "\"14\":\"" D_GPIO_SDCARD_MMC_CLK_CTR   "\","   
    "\"2\":\"" D_GPIO_SDCARD_MMC_D0_CTR   "\""
    #endif 
  "},"
  "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
  "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
"}";

/***********************************
 * SECTION: Templates
************************************/    

#define USE_FUNCTION_TEMPLATE
DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
"{"
  "\"" D_MODULE_DRIVERS__CAMERA_CTR "\":{"
    "\"Mirror\":0,"
      "\"Flip\":0,"
      "\"AWB\":0,"
      "\"Resolution\":12" //12 max
    "}"
"}";

#endif


#ifdef DEVICE_TESTGROUP__CAMERAS__WROOVER_E_LARGE_ESP32_BOARD_03
#ifndef DEVICENAME_CTR
#define DEVICENAME_CTR          "template"
#endif
#ifndef DEVICENAME_FRIENDLY_CTR
#define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
#endif
#define DEVICENAME_ROOMHINT_CTR "testgroup"
   #define MQTT_PORT     1883

/***********************************
* SECTION: Network Configs
************************************/  

#define ENABLE_DEBUGFEATURE_WEBSERVER_URL_LIST
#define FIRMWARE_DEFAULT__INCLUDE_WEBSERVER_BASIC
  
/***********************************
* SECTION: Driver Configs
************************************/  

#define USE_MODULE_CORE_FILESYSTEM

#define USE_MODULE_DRIVERS__CAMERA
#define CAMERA_MODEL_WROVER_KIT
#define ENABLE_DEVFEATURE_ESP32__AUTO_MUTEX
#define ENABLE_RTSPSERVER

/***********************************
 * SECTION: Module/GPIO Configs
************************************/  

#define USE_MODULE_TEMPLATE
DEFINE_PGM_CTR(MODULE_TEMPLATE) 
"{"
  "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
  "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  "\"" D_GPIO_NUMBER "\":{"
    "\"2\":\"" D_GPIO_STATUS_LED_CTR  "\""
  "},"
  "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
  "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
"}";

/***********************************
 * SECTION: Templates
************************************/    

#define USE_FUNCTION_TEMPLATE
DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
"{"
  "\"" D_MODULE_DRIVERS__CAMERA_CTR "\":{"
    "\"Mirror\":1,"
      "\"Flip\":1,"
      "\"AWB\":0,"
      "\"Resolution\":12" //12 max
    "}"
"}";

#endif



#ifdef DEVICE_CAMERA_XIAO_TESTBED2
  #define DEVICENAME_CTR          "treadmill_power_monitor"
  #define DEVICENAME_FRIENDLY_CTR "HVAC Desk DevPlatform"
  #define DEVICENAME_ROOMHINT_CTR "Bedroom"
  #define MQTT_HOST   "192.168.1.70" // primary
    #define MQTT_PORT     1883
    
  #define SETTINGS_HOLDER 1239


  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

  /***********************************
   * SECTION: System Configs
  ************************************/     


  #define USE_MODULE_SENSORS_SUN_TRACKING

  #define USE_MODULE_CORE_FILESYSTEM


//   /***********************************
//    * SECTION: Network Configs
//   ************************************/    

//   /***********************************
//    * SECTION: Sensor Configs
//   ************************************/  

//   #define USE_MODULE_SENSORS_INTERFACE  
//   #define USE_MODULE_SENSORS_SWITCHES
//     #

//   /***********************************
//    * SECTION: Display Configs
//   ************************************/  

//   #define USE_MODULE_DISPLAYS_INTERFACE
//   #define USE_MODULE_DISPLAYS_OLED_SH1106
//     #define SHOW_SPLASH

//   // Add this, 4 rows so show the power, current, voltage and energy

//   /***********************************
//    * SECTION: Driver Configs
//   ************************************/  

//   /***********************************
//    * SECTION: Lighting Configs
//   ************************************/  

//   /***********************************
//    * SECTION: Energy Configs
//   ************************************/  

//   #define USE_MODULE_ENERGY_INTERFACE
//   #define USE_MODULE_ENERGY_PZEM004T_V3
//     #define ENABLE_DEVFEATURE_REDUCE_SUBORDINATE_MQTT_REPORTING_ENERGY // If energy_interface is primary reporting, reduce pzem to slower (debug only)
//   #define MAX_ENERGY_SENSORS 1
//   #define MAX_PZEM004T_DEVICES 1

//   /***********************************
//    * SECTION: Controller Configs
//   ************************************/  

// //  #define USE_MODULE_CONTROLLER__ENERGY_OLED
//  #define USE_MODULE_CONTROLLER_CUSTOM__TREADMILL_LOGGER

  /***********************************
   * SECTION: GPIO Template
  ************************************/  

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"23\":\"" D_GPIO_SWT1_CTR  "\","
      #endif
      "\"16\":\""  D_GPIO_PZEM0XX_RX_MODBUS_CTR "\"," 
      "\"17\":\""  D_GPIO_PZEM0XX_TX_CTR "\","
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_DISPLAYS_OLED_SH1106)
      "\"22\":\"" D_GPIO_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_I2C_SDA_CTR   "\","   
      #endif
      "\"2\":\""  D_GPIO_LED1_INV_CTR "\""   // builtin led
      // 32 - LED Strip External
      // 21 - LED Strip Onboard
      // 25?
      // 
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


//   /**
//    * @brief Drivers and Sensors for HVAC zones
//    **/
//   #define D_DEVICE_HEATER_0_NAME "Dryer"
//   #define D_DEVICE_HEATER_1_NAME "FloorMat"
//   #define D_DEVICE_HEATER_2_NAME "FanHeater"
//   #define D_DEVICE_HEATER_3_NAME "OilRadiator"

//   #define D_DEVICE_SENSOR_DHT_0_NAME "Downstairs_DHT"

// // {"NumDevices":4,"DeviceNameIndex":[-1,-1,-1,-1],"AddressList":[[40,140,131,47,0,0,0,230],[40,18,77,49,0,0,0,233],[40,233,112,49,0,0,0,11],[40,165,161,47,0,0,0,189]]}

//   /** 
//    * MainBoard
//    * */
//   #define D_DEVICE_SENSOR_DB18S20_01_NAME        "MainBoard-1"
//   #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,165,161,47,0,0,0,189]"

//   #define D_DEVICE_SENSOR_DB18S20_02_NAME        "MainBoard-2"
//   #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,233,112,49,0,0,0,11]"

//   #define D_DEVICE_SENSOR_DB18S20_03_NAME        "MainBoard-3"
//   #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,140,131,47,0,0,0,230]"

//   #define D_DEVICE_SENSOR_DB18S20_04_NAME        "MainBoard-4"
//   #define D_DEVICE_SENSOR_DB18S20_04_ADDRESS     "[40,18,77,49,0,0,0,233]" //233 4

//   #define D_DEVICE_SENSOR_BME_280_NAME "BME280"
//   #define D_DEVICE_SENSOR_BME_680_NAME "BME680"

//   #define D_DEVICE_SENSOR_BH1750_NAME "Ambient"

//   #define D_DEVICE_SENSOR_CURRENT "LEDStrip"

  
//   #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "1"

//   #define D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "Treadmill"
  
  
//   #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR   D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
//     "\"" D_ENERGY "\":{"
//         "\"DeviceCount\":1"    
//     "},"
//     "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":{"
//         "\"DeviceCount\":1"    
//     "},"
//     "\"" D_DEVICENAME "\":{"
//       "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":["
//         "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\""
//       "],"
//       "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":["
//         "\"" D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "\""
//       "],"
//       "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
//         "\"" "TrackingActive" "\""
//       "]"
//     "},"
//     "\"" D_SENSORADDRESS "\":{"
//       "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":[" 
//         D_DEVICE_SENSOR_PZEM004T_0_ADDRESS ""
//       "]"  
//     "},"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":60,\"ConfigPeriod\":120}"  
  "}";



  
#endif




#ifdef DEVICE_TESTBED__BOARDTEST_ESP32S3__XIAO_ESP32_S3_SEEDSTUDIO2
#ifndef DEVICENAME_CTR
#define DEVICENAME_CTR          "consumerunit"
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

  // #define USE_MODULE_DRIVERS_CAMERA_OV2640
    // #define CAMERA_MODEL_XIAO_ESP32S3
  // #define USE_MODULE_DRIVERS__CAMERA_TASMOTA
    // #define ENABLE_DEVFEATURE_CAMERA_TASMOTA_INCLUDE_WEBSERVER

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
   //  "\"16\":\""  D_GPIO_PZEM0XX_RX_MODBUS_CTR "\"," 
   //  "\"17\":\""  D_GPIO_PZEM0XX_TX_CTR "\","
    #if defined(USE_MODULE_SENSORS__TOF_VL53L0X) || defined(USE_MODULE_SENSORS__TOF_VL53L1X) || defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750) || defined(USE_MODULE_ENERGY_INA219) || defined(USE_MODULE_DISPLAYS_OLED_SH1106)
    "\"8\":\"" D_GPIO_I2C_SDA_CTR   "\","
    "\"9\":\"" D_GPIO_I2C_SCL_CTR   "\""    
    #endif
    #ifdef USE_MODULE_SENSORS_PIR
   //  "\"23\":\""  D_GPIO_PIR_1_CTR "\","
    #endif
    #ifdef USE_MODULE_SENSORS__TOF_VL53L0X
    "\"33\":\""  D_GPIO__TOF_VL53L0X_XSHUT1__CTR "\","
   //  "\"33\":\""  D_GPIO_UNUSED_FORCED_HIGH_CTR "\"," // Connected to XSHUT but not wanted. HIGH for remain enabled
   //  "\"26\":\""  D_GPIO_UNUSED_FORCED_LOW_CTR "\"," // Connected to XSHUT but not wanted. HIGH for remain enabled
    #endif
    #ifdef USE_MODULE_SENSORS__TOF_VL53L1X
     // "\"26\":\""  D_GPIO__TOF_VL53L1X_XSHUT1__CTR "\"," // turned off only for testing new sensor interface, needed for dual TOF use
   //  "\"33\":\""  D_GPIO_UNUSED_FORCED_LOW_CTR "\"," // Connected to XSHUT but not wanted. HIGH for remain enabled
    #endif
    #ifdef USE_MODULE_SENSORS_SR04
    "\"4\":\"" D_GPIO_SR04_ECHO_CTR   "\","
    "\"2\":\"" D_GPIO_SR04_TRIG_CTR  "\","  
    #endif 
    #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ
    "\"35\":\""  D_GPIO_PIR_2_INV_CTR "\","
    #endif
    #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__PIR_LARGE
    "\"34\":\""  D_GPIO_PIR_1_INV_CTR "\","
    #endif
    #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__PIR_SMALL
    "\"5\":\""  D_GPIO_PIR_3_INV_CTR "\","
    #endif
    #ifdef USE_MODULE_SENSORS__RADAR_HLK_LD2410
    "\"17\":\""  D_GPIO__HLK_LD2410_TX__CTR "\","
    "\"16\":\""  D_GPIO__HLK_LD2410_RX__CTR "\","
    #endif
   //  #ifdef USE_MODULE_SENSORS__TOF_VL53L0X
   //  "\"27\":\""  D_GPIO__TOF_VL53L1X_XSHUT__CTR "\","
   //  #endif
    #ifdef USE_MODULE_SENSORS_BUTTONS
   //  "\"18\":\"" D_GPIO_KEY1_INV_CTR  "\","
   //  "\"19\":\"" D_GPIO_KEY2_INV_CTR  "\","
   //  "\"33\":\"" D_GPIO_KEY3_INV_CTR  "\","
    #endif
    
   //  "\"4\":\"" D_GPIO_LED1_CTR  "\","
   //  "\"5\":\"" D_GPIO_LED2_CTR  "\","
    // "\"8\":\"" D_GPIO_LED1_CTR  "\""
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
#ifdef DEVICE_GROUP__CAMERAS__ESP32_CAM_EYE_SENSOR_01
#ifndef DEVICENAME_CTR
#define DEVICENAME_CTR          "tg_cameras__cam_eye_sens01"
#endif
#ifndef DEVICENAME_FRIENDLY_CTR
#define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
#endif
#ifndef DEVICENAME_DESCRIPTION_CTR
#define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
#endif
#define DEVICENAME_ROOMHINT_CTR "testgroup"
   #define MQTT_HOST     "192.168.3.70"
   #define MQTT_PORT     1883
  
/***********************************
 * SECTION: System Debug Options
************************************/    

///////////////////////////////////////////// Enable Logs
// #define DISABLE_SERIAL
// #define DISABLE_SERIAL0_CORE
// #define ENABLE_DEBUG_MANUAL_DELAYS // permits blocking delays

///////////////////////////////////////////// System Logs
// #define ENABLE_ADVANCED_DEBUGGING
// #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
// #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
// #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
// #define ENABLE_DEBUG_FUNCTION_NAMES
// 
// #define SERIAL_LOG_LEVEL_DURING_BOOT 8
// #define ENABLE_DEBUG_LINE_HERE
// #define ENABLE_DEBUG_LINE_HERE2
// #define ENABLE_DEBUG_LINE_HERE3
// #define ENABLE_DEBUG_LINE_HERE_TRACE
// #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
// #define USE_DEBUG_PRINT
// #define ENABLE_DEBUGFEATURE_LOGS__FORCE_FLUSH_ON_TRANSMIT

//new feature to only show logs for a specific module when developing code
 // #define ENABLE_DEBUGFEATURE_LOGGING__RESTRICT_SERIAL_LOGS_TO_MODULE 5028
 // #define ENABLE_DEBUGFEATURE_LOGGING__RESTRICT_SERIAL_LOGS_TO_MODULE_ARRAY [1, 2]

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
* SECTION: Network Configs
************************************/    


#define USE_MODULE_CORE_FILESYSTEM


/***********************************
* SECTION: Enable with one line (to make it easier to switch on and off for debugging)
************************************/  

#define ENABLE_TEMPLATE_SECTION__SENSORS__PIR

/***********************************
* SECTION: Drivers Configs
************************************/  

#define USE_MODULE_DRIVERS__CAMERA

#define CAMERA_MODEL_AI_THINKER

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
    #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__PIR
    "\"16\":\""  D_GPIO_PIR_1_INV_CTR "\""
    #endif
    #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750) || defined(USE_MODULE_ENERGY_INA219) || defined(USE_MODULE_DISPLAYS_OLED_SH1106)
    "\"8\":\"" D_GPIO_I2C_SDA_CTR   "\","
    "\"9\":\"" D_GPIO_I2C_SCL_CTR   "\""    
    #endif
  "},"
  "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
  "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
"}";

/***********************************
 * SECTION: TEMPLATE: Names
************************************/    

#define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "PIRLarge"
#define D_DEVICE_SENSOR_BME_680_NAME "BME680"
#define D_DEVICE_SENSOR_BH1750_NAME "Ambient"

#define USE_FUNCTION_TEMPLATE
DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
"{"
  "\"" D_DEVICENAME "\":{"
    "\"" D_MODULE_SENSORS_PIR_CTR "\":["
      "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
    "],"
    "\"" D_MODULE_SENSORS_BME_CTR "\":["
      "\"" D_DEVICE_SENSOR_BME_680_NAME "\""
    "]"
  "},"
  "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60}"
"}";


#endif


#endif // _CONFIG__FIRMWARE_GROUP__CAMERAS_H

