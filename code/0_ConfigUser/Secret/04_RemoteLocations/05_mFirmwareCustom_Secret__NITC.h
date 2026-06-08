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
// #define DEVICE_QTQ__SERVER_RESET_CONTROLLER__INSTALLED_BOARD
// #define DEVICE_NITC__SERVER_ROOM__AMBIENT_SENSOR




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
// 
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

  #define ENABLE_DEBUGFEATURE_TIME__SHOW_UPTIME_EVERY_SECOND
  
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

  
  
  #define ENABLE_FEATURE__DATABUFFER_LOCK
  
  #define ENABLE_DEVFEATURE_NETWORK__CONSOLE_POLLING
  #define ENABLE_DEVFEATURE_NETWORK__CONSOLE_WEBSOCKET
  #define ENABLE_DEVFEATURE_NETWORK__CAPTIVE_PORTAL

  #define DEBUG_FASTBOOT

                                                  // #define ENABLE_DEVFEATURE_WIFI__FORCE_SOFTAP_MODE_BY_BLOCKING_SSIDS


  #define   D_CAPTIVE_PORTAL_URL_REDIRECT_PATH "/m/serverrelays"

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
#define MAX_RELAYS 8
 
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
      "\"5\":\"" D_GPIO_FUNCTION_REL4_CTR  "\","
      "\"26\":\"" D_GPIO_FUNCTION_REL5_CTR  "\","
      "\"15\":\"" D_GPIO_FUNCTION_REL6_CTR  "\","
      "\"22\":\"" D_GPIO_FUNCTION_REL7_CTR  "\","
      "\"23\":\"" D_GPIO_FUNCTION_REL8_CTR  "\""
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

  #define D_DEVICE_RELAY_0_NAME "ECIT1752"
  #define D_DEVICE_RELAY_1_NAME "ECIT1820"
  #define D_DEVICE_RELAY_2_NAME "ECIT1840"
  #define D_DEVICE_RELAY_3_NAME "R4"
  #define D_DEVICE_RELAY_4_NAME "R5"
  #define D_DEVICE_RELAY_5_NAME "R6"
  #define D_DEVICE_RELAY_6_NAME "R7"
  #define D_DEVICE_RELAY_7_NAME "R8"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_BUTTONS_CTR "\":["
        "\"" "WallRed" "\","
        "\"" "WallBlue" "\","
        "\"" "DoorAlert" "\""
        "\"" "BUT4" "\","
        "\"" "BUT5" "\","
        "\"" "BUT6" "\","
        "\"" "ResetGPIO0" "\""
      "],"
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_NAME "\","
        "\"" D_DEVICE_RELAY_1_NAME "\","
        "\"" D_DEVICE_RELAY_2_NAME "\","
        "\"" D_DEVICE_RELAY_3_NAME "\","
        "\"" D_DEVICE_RELAY_4_NAME "\","
        "\"" D_DEVICE_RELAY_5_NAME "\","
        "\"" D_DEVICE_RELAY_6_NAME "\","
        "\"" D_DEVICE_RELAY_7_NAME "\""
      "]"
    "},"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60}," 
    "\"MQTT_Interface_Priority\":{\"" D_MODULE_ENERGY_INTERFACE_CTR "\":1}" // Each interface will have ability to reduce its subclass mqtt "ifchanged" rate
  "}";

#endif // END DEVICE




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
#ifdef DEVICE_QTQ__SERVER_RESET_CONTROLLER__INSTALLED_BOARD
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
 
  #define ENABLE_FEATURE_SYSTEM__BOOT_SPLASH__DISPLAY_BLOCK_TO_SHOW_END_OF_INIT
  #define ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
  #define ENABLE_DEBUGFEATURE_TIME__SHOW_UPTIME_EVERY_SECOND
  
  /***********************************
   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
  ************************************/  

  // #define ENABLE_TEMPLATE_SECTION__SENSORS__BME

  #define ENABLE_TEMPLATE_SECTION__DISPLAYS__OLED

  #define ENABLE_TEMPLATE_SECTIONS__LIGHTS

 
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

  #define ENABLE_DEBUGFEATURE_WEBSERVER_URL_LIST  
  
  /************************************************************************
   * FILESYSTEM: 
   ************************************************************************/
  
  
  #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
   // until devices can reliably be used without compiling per device
  

  /************************************************************************
   * WEBPAGE:
   ************************************************************************/
  
  #define FIRMWARE_DEFAULT__INCLUDE_WEBSERVER_FULL
    
  

                                #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS
                                #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_LEDS
                                #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_SYNC

                                #define ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS


                              // lets wire with relay8 as "PRE_ARM", which means it must also be powered to give the other relays power, to stop restarts causing resets.



                                #define ENABLE_FEATURE__DATABUFFER_LOCK
                                #define ENABLE_DEVFEATURE_NETWORK__CONSOLE_POLLING
                                #define ENABLE_DEVFEATURE_NETWORK__CONSOLE_WEBSOCKET
                                #define ENABLE_DEVFEATURE_NETWORK__CAPTIVE_PORTAL

                                #define ENABLE_DEVFEATURE_WEBSERVER__STYLES_NOW_SHARED

                                #define DEBUG_FASTBOOT

  // #define ENABLE_DEVFEATURE_WIFI__FORCE_SOFTAP_MODE_BY_BLOCKING_SSIDS


  #define   D_CAPTIVE_PORTAL_URL_REDIRECT_PATH "/m/serverrelays"

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_BUTTONS    
    #define SOC_TOUCH_VERSION_1

  // #define USE_MODULE_SENSORS_SWITCHES
    

 /***********************************
  * SECTION: Display Configs
 ************************************/  
 #ifdef ENABLE_TEMPLATE_SECTION__DISPLAYS__OLED
   #define USE_MODULE_DISPLAYS_INTERFACE
   #define USE_MODULE_DISPLAYS_OLED_SH1106
     #define SHOW_SPLASH
 #endif

 
  // #define ENABLE_FREERAM_APPENDING_SERIAL
    // #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 115200
    #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 921600
  // #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_NEXTION
  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY        
        #define ENABLE_DEVFEATURE_NEXTION_WEBUI
        #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER    
// #define USE_FEATURE_NEXTION__SERIAL_DEFAULT_BUAD_NEW_PANEL_FIRST_OTA
// #define USE_FEATURE_NEXTION__FORCE_SERIAL_BAUDRATE_FROM_DEFAULT 115200  
  #define USE_MODULE_DISPLAYS_NEXTION
    #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  #define NEXTION_DEFAULT_PAGE_NUMBER 10//6      
    //  // change to be code option later
    #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
    #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE
  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY  
  #define ENABLE_DEVFEATURE_NEXTION_WEBUI

  #define ENABLE_FEATURE_NEXTION__ADDLOG_ON_LOG_PAGE




  DEFINE_PGM_CTR(DISPLAY_TEMPLATE)
  R"=====(
  {
    "ObjectNameID": {
      "hIconUS": 2,
      "hTimeUS": 6,
      "hBoostUS": 11,
      "hAutoUS": 16,
      "hIconDS": 3,
      "hTimeDS": 7,
      "hBoostDS": 12,
      "hAutoDS": 17,
      "hIconIH": 4,
      "hTimeIH": 8,
      "hBoostIH": 13,
      "hAutoIH": 18,
      "hIconWB": 5,
      "hTimeWB": 9,
      "hBoostWB": 14,
      "hAutoWB": 19,
      "hIconDryer": 122,
      "hTimeDryer": 123,
      "hBoostDryer": 124,
      "hAutoDryer": 125
    }
  }
  )=====";

    
  /***********************************
   * SECTION: Driver Configs
  ************************************/  
 
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
  // #define USE_MODULE_DRIVERS_LEDS
 
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
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"0\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      #endif
      #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      #endif   
      #ifdef USE_MODULE_DRIVERS_LEDS
      "\"23\":\"" D_GPIO_FUNCTION_LED1_CTR  "\","
      #endif  
      #ifdef USE_MODULE_DISPLAYS_NEXTION
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"32\":\"" D_GPIO_FUNCTION_REL1_CTR  "\","
      "\"33\":\"" D_GPIO_FUNCTION_REL2_CTR  "\","
      "\"25\":\"" D_GPIO_FUNCTION_REL3_CTR  "\","
      "\"26\":\"" D_GPIO_FUNCTION_REL4_CTR  "\","
      "\"27\":\"" D_GPIO_FUNCTION_REL5_CTR  "\","
      "\"14\":\"" D_GPIO_FUNCTION_REL6_CTR  "\","
      "\"12\":\"" D_GPIO_FUNCTION_REL7_CTR  "\","
      "\"13\":\"" D_GPIO_FUNCTION_REL8_CTR  "\""
      #endif
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


//   4 star as it was a battle getting it working in my use case programmed by ESPHOME for Home Assistant use.
// When you follow the ESPHOME documentation to re-flash the device with a serial adapter (you need to solder the provided pins on the serial interface ports) what is missing is the device type and pin definitions and the following ESPHOME YAML parts worked for me:
// esp32:
// board: esp32dev
// framework:
// type: esp-idf

// switch:
// - platform: gpio
// pin: GPIO32
// name: "Relay1"
// - platform: gpio
// pin: GPIO33
// name: "Relay2"
// - platform: gpio
// pin: GPIO25
// name: "Relay3"
// - platform: gpio
// pin: GPIO26
// name: "Relay4"
// - platform: gpio
// pin: GPIO27
// name: "Relay5"
// - platform: gpio
// pin: GPIO14
// name: "Relay6"
// - platform: gpio
// pin: GPIO12
// name: "Relay7"
// - platform: gpio
// pin: GPIO13
// name: "Relay8"


// switch:
// - platform: gpio
// pin: GPIO32
// name: "Relay1"

// - platform: gpio
// pin: GPIO33
// name: "Relay2"

// - platform: gpio
// pin: GPIO25
// name: "Relay3"

// - platform: gpio
// pin: GPIO26
// name: "Relay4"

// - platform: gpio
// pin: GPIO27
// name: "Relay5"

// - platform: gpio
// pin: GPIO14
// name: "Relay6"

// - platform: gpio
// pin: GPIO12
// name: "Relay7"

// - platform: gpio
// pin: GPIO13
// name: "Relay8"

  /***********************************
   * SECTION: Lighting Configs
  ************************************/    


  /***********************************
   * SECTION: TEMPLATE: Names
  ************************************/    

  #define D_DEVICE_RELAY_0_NAME "ECIT1752"
  #define D_DEVICE_RELAY_1_NAME "ECIT1820"
  #define D_DEVICE_RELAY_2_NAME "ECIT1840"
  #define D_DEVICE_RELAY_3_NAME "R4"
  #define D_DEVICE_RELAY_4_NAME "R5"
  #define D_DEVICE_RELAY_5_NAME "R6"
  #define D_DEVICE_RELAY_6_NAME "R7"
  #define D_DEVICE_RELAY_7_NAME "R8"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_BUTTONS_CTR "\":["
        "\"" "WallRed" "\","
        "\"" "WallBlue" "\","
        "\"" "DoorAlert" "\""
        "\"" "BUT4" "\","
        "\"" "BUT5" "\","
        "\"" "BUT6" "\","
        "\"" "ResetGPIO0" "\""
      "],"
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_NAME "\","
        "\"" D_DEVICE_RELAY_1_NAME "\","
        "\"" D_DEVICE_RELAY_2_NAME "\","
        "\"" D_DEVICE_RELAY_3_NAME "\","
        "\"" D_DEVICE_RELAY_4_NAME "\","
        "\"" D_DEVICE_RELAY_5_NAME "\","
        "\"" D_DEVICE_RELAY_6_NAME "\","
        "\"" D_DEVICE_RELAY_7_NAME "\""
      "]"
    "},"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60}," 
    "\"MQTT_Interface_Priority\":{\"" D_MODULE_ENERGY_INTERFACE_CTR "\":1}" // Each interface will have ability to reduce its subclass mqtt "ifchanged" rate
  "}";

#endif // END DEVICE





/**
 * @description: 
 * 
 * Landing Panel
 *  ** PIR Detector for Landing
 *  ** Nextion Panel
 *  ** BME
 *  ** 4C RGB strip with upper/lower section
 * MasterBedroom
 * 
 * 
 * *********************************************************************************************************************************************************************************
*/
#ifdef DEVICE_NITC__SERVER_ROOM__AMBIENT_SENSOR
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "template_name"
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

  /***********************************
   * SECTION: System Configs
  ************************************/     


  // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
   // until devices can reliably be used without compiling per device

  


  /***********************************
   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
  ************************************/  
  
  // #define ENABLE_TEMPLATE_SECTION__SENSORS__BME
  // #define ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
  // #define ENABLE_TEMPLATE_SECTION__SENSORS__SOLAR
  // #define ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
  // #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION
  // #define ENABLE_TEMPLATE_SECTION__LIGHTING
  // #define ENABLE_TEMPLATE_SECTION__ENERGY
  // #define ENABLE_TEMPLATE_SECTION__ENERGY__PZEM
  // #define ENABLE_TEMPLATE_SECTION__ENERGY__INA219
  // #define ENABLE_TEMPLATE_SECTION__DISPLAY_NEXTION
  // #define ENABLE_TEMPLATE_SECTION__CONTROLLER__HVAC
  // #define ENABLE_TEMPLATE_SECTION__DISPLAY_OLED
  // #define ENABLE_TEMPLATE_SECTION__DRIVER_RELAYS


  /***********************************
   * SECTION: Network Configs
  ************************************/    

  // 
  // 

  


  /***********************************
   * SECTION: Lighting Configs
  ************************************/    
  // #define ENABLE_FEATURE_WATCHDOG_TIMER
  // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  // // #define ENABLE_DEBUGFEATURE_TELEMETRY__MQTT_SEND_HEALTH_EVERY_SECOND
  // // #define ENABLE_DEVFEATURE_DEBUG_REMOVE_POSSIBLE_ERROR_CODE
  // // #define ENABLE_FEATURE_DEBUG_POINT_TASKER_INFO_AFTER_UPSECONDS 110
  // // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define DISABLE_SLEEP

  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME

  // #define USE_MODULE_CORE_RULES

  // // #define DEVICENAMEBUFFER_NAME_BUFFER_LENGTH 1000
  // // #define DEVICENAMEBUFFER_NAME_INDEX_LENGTH  100
  // // #define DB18_SENSOR_MAX                     15
  // //#define DATA_BUFFER_PAYLOAD_MAX_LENGTH      3000 //needed for db sensosrs, but currently causes crash in lighting
  // // #define MQTT_MAX_PACKET_SIZE                3000

  #define USE_MODULE_SENSORS_INTERFACE
  //   #
  // //   #define USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
  // // #define USE_MODULE_SENSORS_DS18X20
  // //   #define DS18X20_MAX_SENSORS 20
  // //   #define ENABLE_DEVFEATURE_DS18B20_SEARCHING_SENSOR_LOCATION_WITH_ADDRESS_TEMP_SPLASH
  
  // #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
    #define USE_MODULE_SENSORS_DS18X20
      #define DS18X20_MAX_SENSORS 20
        #define ENABLE_DEBUG_MQTT_CHANNEL_DB18X20    
  // #endif 
  #define USE_MODULE_SENSORS_BME
    

  // #ifdef ENABLE_TEMPLATE_SECTION__DISPLAY_OLED
  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SH1106
  //   #define SHOW_SPLASH
  //   // 4x10
  //   // Uptime so I know its working by glance
  //   // Relay Minutes On
  //   // Shower Temp /     Bath Temp
  // #endif
  

  // #define ENABLE_FEATURE_SENSORS_INTERFACE__SNAPSHOT_READINGS_TO_CALIBRATION_FILE

  // #define USE_MODULE_CORE_FILESYSTEM
  // 
  // 
  // #define CALIB_FS
  // #define ENABLE_DEBUGFEATURE__CALIB_CAPTURE_VERBOSE


  // #define USE_MODULE_DRIVERS_INTERFACE
  // #define USE_MODULE_DRIVERS_RELAY
  // #define USE_MODULE_DRIVERS_LEDS
  //   #define MAX_RELAYS 1
    
  /**
   * @brief 
   * Add extra settings parameter that allows for "expected total device count" (relays+sensors) and allow a NEW alert topic (status/alert status/event) to tell me something is wrong
   * 
   */
  // #define USE_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR
    // #define ENABLE_DEVFEATURE_SENSORS_INTERFACE_SHOW_TEMPERATURE_AS_COLOUR  //should this be a "controller", or via interface "getTemp convert to heatmap"
    // #define SETTINGS_SENSORS_MQTT_IFCHANGED_PERIOD_SECONDS 60  
    // #define USE_DEVFEATURE_SENSOR_COLOURS_TOP_TO_BOTTOM

  // #define USE_MODULE_CONTROLLER_HVAC
  //   
  //   #define HEATING_DEVICE_MAX 1

  // #define USE_MODULE_CONTROLLER_CUSTOM__IMMERSION_PANEL




  //turn on later
  // #define USE_MODULE_CONTROLLER_CUSTOM__3DPRINTER_ENCLOSURE


  /***********************************
   * SECTION: Template Configs
  ************************************/    

  /***
   * 23 = dallas
   * 21/22 i2c
   * 4 = PIR
   */

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_DISPLAYS_OLED_SH1106)
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DS18X20
      "\"23\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR  "\","
      #endif
      #ifdef USE_MODULE_SENSORS_PIR
      "\"4\":\"" D_GPIO_FUNCTION_SWT1_CTR "\","       // Room
      #endif 
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  
  /***********************************
   * SECTION: Device Configs
  ************************************/    
 

//  {"NumDevices":13,"DeviceNameIndex":[-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1],"AddressList":[
//   [40,48,142,47,0,0,0,112],
//   [40,4,161,51,0,0,0,252],
//   [40,194,109,51,0,0,0,210],
//   [40,38,95,51,0,0,0,32],
//   [40,182,157,48,0,0,0,184],
//   [40,153,85,50,0,0,0,49],
//   [40,217,113,51,0,0,0,44],
//   [40,121,104,51,0,0,0,255],
//   [40,53,140,51,0,0,0,50],
//   [40,181,105,48,0,0,0,88],
//   [40,15,153,47,0,0,0,148],
//   [40,111,124,47,0,0,0,95],
//   [40,127,141,47,0,0,0,111]]}

/*** View of the enclosure fron the front, with signal starting at the bottom left
 * 
 *     *******************************************
 *     *   FTL             FTC             FTR   *
 *     * FL100              ||             FR100 *
 *     *                    ||                   *
 *     * FL75               ||              FR75 *
 *     *                 Front View              *
 *     * FL50               ||              FR50 *
 *     *                    ||                   *
 *     * FL25               ||              FR25 *
 *     *                    ||                   *
 *     * FL00               ||              FR00 *
 *     *                    ||                   *
 *     *******************************************
 */

/**
 * @brief Group 1: Front enclosure sensors (bottom left, up, across, to bottom right)
 * */
// Front right - top to bottom
#define D_DEVICE_SENSOR_DB18S20_01_NAME        "FrontRight00"
#define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,153,85,50,0,0,0,49]"
#define D_DEVICE_SENSOR_DB18S20_02_NAME        "FrontRight25"
#define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,48,142,47,0,0,0,112]"
#define D_DEVICE_SENSOR_DB18S20_03_NAME        "FrontRight50"
#define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,38,95,51,0,0,0,32]"
#define D_DEVICE_SENSOR_DB18S20_04_NAME        "FrontRight75"
#define D_DEVICE_SENSOR_DB18S20_04_ADDRESS     "[40,15,153,47,0,0,0,148]"
#define D_DEVICE_SENSOR_DB18S20_05_NAME        "FrontRight100"
#define D_DEVICE_SENSOR_DB18S20_05_ADDRESS     "[40,217,113,51,0,0,0,44]"
// Front cetnre
#define D_DEVICE_SENSOR_DB18S20_06_NAME        "FrontTopLeft"
#define D_DEVICE_SENSOR_DB18S20_06_ADDRESS     "[40,182,157,48,0,0,0,184]"
#define D_DEVICE_SENSOR_DB18S20_07_NAME        "FrontTopCentre"
#define D_DEVICE_SENSOR_DB18S20_07_ADDRESS     "[40,121,104,51,0,0,0,255]"
#define D_DEVICE_SENSOR_DB18S20_08_NAME        "FrontTopRight"
#define D_DEVICE_SENSOR_DB18S20_08_ADDRESS     "[40,111,124,47,0,0,0,95]"
// Front left - bottom to top
#define D_DEVICE_SENSOR_DB18S20_09_NAME        "FrontLeft00"
#define D_DEVICE_SENSOR_DB18S20_09_ADDRESS     "[40,127,141,47,0,0,0,111]"
#define D_DEVICE_SENSOR_DB18S20_10_NAME        "FrontLeft25"
#define D_DEVICE_SENSOR_DB18S20_10_ADDRESS     "[40,53,140,51,0,0,0,50]"
#define D_DEVICE_SENSOR_DB18S20_11_NAME        "FrontLeft50"
#define D_DEVICE_SENSOR_DB18S20_11_ADDRESS     "[40,181,105,48,0,0,0,88]"
#define D_DEVICE_SENSOR_DB18S20_12_NAME        "FrontLeft75"
#define D_DEVICE_SENSOR_DB18S20_12_ADDRESS     "[40,4,161,51,0,0,0,252]"
#define D_DEVICE_SENSOR_DB18S20_13_NAME        "FrontLeft100"
#define D_DEVICE_SENSOR_DB18S20_13_ADDRESS     "[40,194,109,51,0,0,0,210]"

  
  #define D_DEVICE_SENSOR_DB18S20_14_NAME        "Sensor 01"
  #define D_DEVICE_SENSOR_DB18S20_14_ADDRESS     "[40,68,132,149,240,1,60,87]"
  #define D_DEVICE_SENSOR_DB18S20_15_NAME        "Sensor 02" 
  #define D_DEVICE_SENSOR_DB18S20_15_ADDRESS     "[40,174,159,3,0,0,0,1]"



  #define D_DEVICE_DRIVER_RELAY_01_NAME "Immersion"
  
  #define D_DEVICE_SENSOR_BME_LONG_WIRE_NAME  "BME Dryer"
  #define D_DEVICE_SENSOR_BME_SHORT_WIRE_NAME "BME Enclosure"

  #define D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "Immersion"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_SUN_TRACKING_CTR "\":["
        "\"" "PRINTER" "\""
      "],"  
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" D_DEVICE_DRIVER_RELAY_01_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_CTR "\":["
        "\"" D_DEVICE_SENSOR_BME_LONG_WIRE_NAME "\"," // 0x76
        "\"" D_DEVICE_SENSOR_BME_SHORT_WIRE_NAME  "\""       // 0x77
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
        // Group 1
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_05_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_06_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_07_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_08_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_09_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_10_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_11_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_12_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_13_NAME "\","
        // Group 2
        "\"" D_DEVICE_SENSOR_DB18S20_14_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_15_NAME "\""    
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_CTR "\":["
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "\""
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":{" 
        // Group 1
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\":" D_DEVICE_SENSOR_DB18S20_03_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\":" D_DEVICE_SENSOR_DB18S20_04_ADDRESS ","      
        "\"" D_DEVICE_SENSOR_DB18S20_05_NAME "\":" D_DEVICE_SENSOR_DB18S20_05_ADDRESS ","       
        "\"" D_DEVICE_SENSOR_DB18S20_06_NAME "\":" D_DEVICE_SENSOR_DB18S20_06_ADDRESS ","       
        "\"" D_DEVICE_SENSOR_DB18S20_07_NAME "\":" D_DEVICE_SENSOR_DB18S20_07_ADDRESS ","       
        "\"" D_DEVICE_SENSOR_DB18S20_08_NAME "\":" D_DEVICE_SENSOR_DB18S20_08_ADDRESS ","       
        "\"" D_DEVICE_SENSOR_DB18S20_09_NAME "\":" D_DEVICE_SENSOR_DB18S20_09_ADDRESS ","  
        // Group 2
        "\"" D_DEVICE_SENSOR_DB18S20_10_NAME "\":" D_DEVICE_SENSOR_DB18S20_10_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_11_NAME "\":" D_DEVICE_SENSOR_DB18S20_11_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_12_NAME "\":" D_DEVICE_SENSOR_DB18S20_12_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_13_NAME "\":" D_DEVICE_SENSOR_DB18S20_13_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_14_NAME "\":" D_DEVICE_SENSOR_DB18S20_14_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_15_NAME "\":" D_DEVICE_SENSOR_DB18S20_15_ADDRESS ""
      "}"  
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_01_NAME "\"," // Also an array to match heating/cooling
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "}"
      "]"
    "}"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":60},"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  
  //  "\"Calib\": {"
  //     "\"Enable\": 1,"            // start capturing at boot
  //     "\"Name\": \"\","             // empty → no filter (capture all)
  //     "\"SampleMs\": 1000,"       // collect once per second
  //     "\"DumpSecs\": 60"          // flush to /sensor_calib_captures.json every 60s
  //   "}"
    "\"Calib\": {"
        "\"Enable\": 1,"
        "\"Name\": \"\","                // ignored if NameList present
        "\"SampleMs\": 1000,"
        "\"DumpSecs\": 60,"
        "\"NameList\": ["
          "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
          "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
          "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
          "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\","
          "\"" D_DEVICE_SENSOR_DB18S20_05_NAME "\","
          "\"" D_DEVICE_SENSOR_DB18S20_06_NAME "\","
          "\"" D_DEVICE_SENSOR_DB18S20_07_NAME "\","
          "\"" D_DEVICE_SENSOR_DB18S20_08_NAME "\","
          "\"" D_DEVICE_SENSOR_DB18S20_09_NAME "\","
          "\"" D_DEVICE_SENSOR_DB18S20_10_NAME "\","
          "\"" D_DEVICE_SENSOR_DB18S20_11_NAME "\","
          "\"" D_DEVICE_SENSOR_DB18S20_12_NAME "\","
          "\"" D_DEVICE_SENSOR_DB18S20_13_NAME "\","
          "\"" D_DEVICE_SENSOR_DB18S20_14_NAME "\","
          "\"" D_DEVICE_SENSOR_DB18S20_15_NAME "\""
        "]"
      "}"

    "}";

  // #ifdef USE_RGB_OUT_LANDING_PANEL
  // #ifdef USE_MODULE_LIGHTS_INTERFACE
  // #define USE_SK6812_METHOD_DEFAULT
  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // R"=====(
  // {
  //   "HardwareType":"SK6812",
  //   "AnimationMode":"Effects",
  //   "ColourOrder":"grbw",
  //   "ColourPalette":"Christmas 01",
  //   "Effects": {
  //     "Function":1,
  //     "Intensity":50
  //   },
  //   "Transition": {
  //     "TimeMs": 0,
  //     "RateMs": 1000
  //   },
  //   "SegColour": {
  //     "Hue": 120,
  //     "Sat": 100,
  //     "SubType":3
  //   },
  //   "BrightnessRGB_255": 255,
  //   "BrightnessCCT_255": 255
  // }
  // )=====";
  // #endif // USE_MODULE_LIGHTS_INTERFACE
  // #endif // USE_RGB_OUT_LANDING_PANEL

  // #ifdef USE_RGB_OUT_TANK
  // #ifdef USE_MODULE_LIGHTS_INTERFACE
  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // R"=====(
  // {
  //   "HardwareType":"WS28XX",
  //   "AnimationMode":"Effects",
  //   "ColourOrder":"grb",
  //   "ColourPalette":"Christmas 01",
  //   "Effects": {
  //     "Function":1,
  //     "Intensity":50
  //   },
  //   "Transition": {
  //     "TimeMs": 0,
  //     "RateMs": 1000
  //   },
  //   "SegColour": {
  //     "Hue": 120,
  //     "Sat": 100,
  //     "SubType":3
  //   },
  //   "BrightnessRGB_255": 255,
  //   "BrightnessCCT_255": 1
  // }
  // )=====";
  // #endif // USE_MODULE_LIGHTS_INTERFACE
  // #endif // USE_RGB_OUT_TANK
    
  // #define USE_RULES_TEMPLATE
  // DEFINE_PGM_CTR(RULES_TEMPLATE)
  // "{"// for PIR to follow
  //   "\"Rule0\":{"
  //     "\"Trigger\":{"
  //       "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_CTR "\","
  //       "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\","
  //       "\"DeviceName\":0,"
  //       "\"State\":\"On\""
  //     "},"
  //     "\"Command\":{"
  //       "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
  //       "\"Function\":\"" D_TASK_EVENT_MOTION_STARTED_CTR "\","
  //       "\"DeviceName\":0," 
  //       "\"State\":\"Follow\""
  //     "}"
  //   "},"
  //   "\"Rule1\":{"
  //     "\"Trigger\":{"
  //       "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_CTR "\","
  //       "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\","
  //       "\"DeviceName\":1,"
  //       "\"State\":\"On\""
  //     "},"
  //     "\"Command\":{"
  //       "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
  //       "\"Function\":\"" D_TASK_EVENT_MOTION_STARTED_CTR "\","
  //       "\"DeviceName\":1," 
  //       "\"State\":\"Follow\""
  //     "}"
  //   "}"
  // "}";

#endif






#endif // END GUARD
