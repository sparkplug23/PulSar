#ifndef _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_QTQ_H
#define _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_QTQ_H

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"

/*** QTQ Specific
 * 
 * In relation to work
 * 
 */

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Enable Defines *******************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

// #define DEVICE_QTQ__SERVER_RESET_CONTROLLER__TESTBOARD




/***************************************************************************************************************************************************************************************************************************************************************************************************************
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



/**
 * @brief 
 * 
 * QTQ Server Reset Controller Testboard
 * Device will use softAP mode so any device can remotely connect into it.
 * 
 * 8 relays will be controllable via basic webui.
 * 
 * 3 buttons on web, off, on, 5 second ON pulse (for reset)
 * 
 * First lets develop this with normal wifi connection, then move to softAP only mode.
 * 
 * 
 */
#ifdef DEVICE_QTQ__SERVER_RESET_CONTROLLER__TESTBOARD
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "template"
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
//  #define ENABLE_ADVANCED_DEBUGGING
// #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
// #define ENABLE_DEBUG_FUNCTION_NAMES
// #define ENABLE_DEBUGFEATURE_WEBUI__SHOW_BUILD_DATETIME_IN_FOOTER
// #define SERIAL_LOG_LEVEL_DURING_BOOT 8
// #define ENABLE_DEBUG_LINE_HERE
// #define ENABLE_DEBUG_LINE_HERE2
// #define ENABLE_DEBUG_LINE_HERE3
// #define ENABLE_DEBUG_LINE_HERE4
// #define ENABLE_DEBUG_LINE_HERE_TRACE
// #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
// #define USE_DEBUG_PRINT
// #define ENABLE_DEBUGFEATURE_LOGS__FORCE_FLUSH_ON_TRANSMIT

// #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE
 
  #define ENABLE_FEATURE_SYSTEM__BOOT_SPLASH__DISPLAY_BLOCK_TO_SHOW_END_OF_INIT
  #define ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE

  /***********************************
   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
  ************************************/  

  // #define ENABLE_TEMPLATE_SECTION__SENSORS__BME

  // #define ENABLE_TEMPLATE_SECTION__ENERGY
  // #define ENABLE_TEMPLATE_SECTION__ENERGY__PZEM

 
  /***********************************
   * SECTION: Storage Configs
  ************************************/  


  /***********************************
   * SECTION: System Configs
  ************************************/     

  // #define USE_TEMPLATED_DEFAULT_OTA_RECOVERY_METHODS

  // #define DEVICENAMEBUFFER_NAME_BUFFER_LENGTH 800

  /***********************************
   * SECTION: Network Configs
  ************************************/    

  // #define USE_MODULE_NETWORK_WEBSERVER
  // #define ENABLE_WEBSERVER_LIGHTING_WEBUI

  #define ENABLE_DEVFEATURE_WEBSERVER__ROOT_DEBUG_LINKS
  #define ENABLE_DEVFEATURE_WEBSERVER__JAN26_REDESIGNED_WEBUI
  #define ENABLE_DEVFEATURE_WEBUI__INCLUDE_URI_PRE2023

  #define ENABLE_DEBUGFEATURE_TIME__SHOW_UPTIME_EVERY_SECOND

  #define ENABLE_DEVFEATURE_DATABUFFER_LOCK

  // #define ENABLE_FEATURE_WEBSERVER__DELAYED_JSONLOCKED_COMMAND_PROCESSING

  #define ENABLE_DEVFEATURE_LIGHTING__SETTINGS_HTTP_API



  #define ENABLE_DEVFEATURE_WEBSERVER__SETTINGS_WEBPAGES
  #define ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS__SUBPAGE_WIFI

  #define ENABLE_DEBUGFEATURE_TASKER__SPLASH_JSON_BUFFER

  #define ENABLE_DEVFEATURE_NETWORK__CONSOLE_WEBSOCKET

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  // #define USE_MODULE_SENSORS_INTERFACE  
  // #define USE_MODULE_SENSORS_BUTTONS    
  //   #define SOC_TOUCH_VERSION_1

  // #define USE_MODULE_SENSORS_SWITCHES
    

  /***********************************
   * SECTION: Display Configs
  ************************************/  

 
  /***********************************
   * SECTION: Driver Configs
  ************************************/  
 
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
//   // #define USE_MODULE_DRIVERS_LEDS
 
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

#define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE
   #define FIRMWARE_DEFAULT__ENABLE_SOLAR_PALETTES

  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[     
      {
        "Name":"Star",
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      }
    ],
    "Segments":[
      {
        "PixelRange":[0,100],
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
   * SECTION: Energy Configs
  ************************************/  

  /***********************************
   * SECTION: Controller Configs
  ************************************/  

  #define USE_MODULE_CONTROLLER_CUSTOM__SERVER_RESET_RELAYS

  /***********************************
   * SECTION: MQTT Template Test Loading
  ************************************/  

//  #define ENABLE_DEVFEATURE_MQTT__PUBLUSH_TASMOTA_METHODS
// //  #define ENABLE_DEBUG_TRACE__MQTT_TOPIC_AS_TRASNMITTED
//  #define ENABLE_RULES_ENGINE__TEST_BUTTON0_RELAY0
//  #define ENABLE_DEVFEATURE_RESET_RELAY_DECOUNTER_WHEN_TURNED_OFF

  /***********************************
   * SECTION: GPIO Template
  ************************************/  

//  #define ENABLE_DEBUG_POINT_MODULE_TEMPLATE_BOOT_SPLASH

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_LEDS
      "\"2\":\"" D_GPIO_FUNCTION_LED1_CTR  "\","  // BUILTIN LED as new Status LED, to reflect Network and Relay0 status
      #else
      // "\"2\":\""  D_GPIO_FUNCTION_LED3_CTR  "\"," //builtin BLUE
      #endif  
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"5\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"4\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"26\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\","
      "\"15\":\"" D_GPIO_FUNCTION_KEY4_INV_CTR  "\","
      #ifdef SOC_TOUCH_VERSION_1
      "\"32\":\"" D_GPIO_FUNCTION_KEY5_TOUCH_CTR  "\","
      "\"33\":\"" D_GPIO_FUNCTION_KEY6_TOUCH_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY7_INV_CTR  "\","
      #endif
      #endif
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"18\":\"" D_GPIO_FUNCTION_SWT1_INV_CTR  "\","
      "\"19\":\"" D_GPIO_FUNCTION_SWT2_INV_CTR  "\","
      #endif  
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"2\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      "\"4\":\"" D_GPIO_FUNCTION_REL2_CTR  "\","
      "\"12\":\"" D_GPIO_FUNCTION_REL3_CTR  "\","
      "\"13\":\"" D_GPIO_FUNCTION_REL4_CTR  "\""
      #endif
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  /***********************************
   * SECTION: Lighting Configs
  ************************************/    


  /***********************************
   * SECTION: TEMPLATE: Names
  ************************************/    

  #define D_DEVICE_UNIQUE_NAME "OilRadiator01"
  #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "16"
  #define D_DEVICE_SENSOR_ZONE_0_NAME "OilRadiator01-BME0"
  #define D_DEVICE_DRIVER_RELAY_0_NAME "OilRadiator01-DriverZone0"
  
  #define D_DEVICE_SENSOR_DB18S20_0_NAME        "Radiator"
  #define D_DEVICE_SENSOR_DB18S20_0_ADDRESS     "[40,143,81,7,51,20,1,189]"

  #define D_DEVICE_HEATER_0_NAME "Dryer"
  #define D_DEVICE_HEATER_1_NAME "FloorMat"
  #define D_DEVICE_HEATER_2_NAME "FanHeater"
  #define D_DEVICE_HEATER_3_NAME "OilRadiator"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_PIR_CTR "\":["
        "\"" D_DEVICE_UNIQUE_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
        "\"" "SwitchEdge" "\","
        "\"" "SwitchClose" "\""
      "],"
      "\"" D_MODULE_SENSORS_BUTTONS_CTR "\":["
        "\"" "WallRed" "\","
        "\"" "WallBlue" "\","
        "\"" "DoorAlert" "\""
        // "\"" "BUT1" "\","
        // "\"" "BUT2" "\","
        // "\"" "BUT3" "\","
        "\"" "BUT4" "\","
        "\"" "BUT5" "\","
        "\"" "BUT6" "\","
        "\"" "ResetGPIO0" "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
        "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\""
      "],"      
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":["
        "\"" D_DEVICE_UNIQUE_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_CTR "\":["
        "\"" D_DEVICE_UNIQUE_NAME "\""
      "],"
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "],"
      "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":["
        "\"" D_DEVICE_UNIQUE_NAME "\""
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":{" 
        "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\":" D_DEVICE_SENSOR_DB18S20_0_ADDRESS ","
      "},"  
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":[" 
        D_DEVICE_SENSOR_PZEM004T_0_ADDRESS ""
      "]"  
    "},"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60}," 
    "\"MQTT_Interface_Priority\":{\"" D_MODULE_ENERGY_INTERFACE_CTR "\":1}" // Each interface will have ability to reduce its subclass mqtt "ifchanged" rate
  "}";

#endif // END DEVICE





#endif // END GUARD
