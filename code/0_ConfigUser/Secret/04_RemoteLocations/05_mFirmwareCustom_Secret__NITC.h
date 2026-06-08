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
#define DEVICE_NITC__SERVER_ROOM__AMBIENT_SENSOR




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
 * **********************************************************************************************************************************************************************************/
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
 
  /***********************************
   * SECTION: System Configs
  ************************************/     

  /***********************************
   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
  ************************************/  
  
  #define ENABLE_TEMPLATE_SECTION__SENSORS__BME
  #define ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
  #define ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
  #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION
  #define ENABLE_TEMPLATE_SECTION__DISPLAY_OLED

  /***********************************
   * SECTION: Network Configs
  ************************************/    

  /***********************************
   * SECTION: Lighting Configs
  ************************************/

  #define USE_MODULE_SENSORS_INTERFACE
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
    #define USE_MODULE_SENSORS_DS18X20
      #define DS18X20_MAX_SENSORS 20
        #define ENABLE_DEBUG_MQTT_CHANNEL_DB18X20    
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
    #define USE_MODULE_SENSORS_BME
  #endif  
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
    #define USE_MODULE_SENSORS_BH1750
  #endif
  #if defined(ENABLE_TEMPLATE_SECTION__SENSORS__MOTION)
   #define USE_MODULE_SENSORS_INTERFACE
   #define USE_MODULE_SENSORS_PIR
  #endif
  
  /***********************************
   * SECTION: Drivers Configs
  ************************************/
 
  // #define USE_MODULE_DRIVERS_INTERFACE
  // #define USE_MODULE_DRIVERS_LEDS
  
  /***********************************
   * SECTION: Displays Configs
  ************************************/

  #ifdef ENABLE_TEMPLATE_SECTION__DISPLAY_OLED
  #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_OLED_SH1106
    #define SHOW_SPLASH
    // 4x10
    // Uptime so I know its working by glance
    // Relay Minutes On
    // Shower Temp /     Bath Temp
  #endif
  
  /***********************************
   * SECTION: Controllers Configs
  ************************************/
 
  #define USE_MODULE_CONTROLLER_CUSTOM__OLED_NITC_AMBIENT

 
  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_DISPLAYS_OLED_SH1106)
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DS18X20
      "\"23\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR  "\","
      #endif
      #ifdef USE_MODULE_SENSORS_PIR
      "\"4\":\"" D_GPIO_FUNCTION_PIR_1_CTR "\","       // Room
      #endif 
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  
  /***********************************
   * SECTION: Device Configs
  ************************************/    

  #define D_DEVICE_SENSOR_DB18S20_01_NAME        "DB2m"
  #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,169,117,83,0,0,0,59]"
  #define D_DEVICE_SENSOR_DB18S20_02_NAME        "DB3m"
  #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,227,225,191,0,0,0,114]"

  #define D_DEVICE_SENSOR_DB18S20_03_NAME        "FrontRight50"
  #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,38,95,51,0,0,0,32]"
  #define D_DEVICE_SENSOR_DB18S20_04_NAME        "FrontRight75"
  #define D_DEVICE_SENSOR_DB18S20_04_ADDRESS     "[40,15,153,47,0,0,0,148]"
  #define D_DEVICE_SENSOR_DB18S20_05_NAME        "FrontRight100"
  #define D_DEVICE_SENSOR_DB18S20_05_ADDRESS     "[40,217,113,51,0,0,0,44]"
  
  #define D_DEVICE_SENSOR_BME_LONG_WIRE_NAME  "Room"
  #define D_DEVICE_SENSOR_BME_SHORT_WIRE_NAME "None"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_SUN_TRACKING_CTR "\":["
        "\"" "PRINTER" "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_CTR "\":["
        "\"" D_DEVICE_SENSOR_BME_LONG_WIRE_NAME "\","   // 0x76
        "\"" D_DEVICE_SENSOR_BME_SHORT_WIRE_NAME  "\""  // 0x77
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
        // Group 1
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_05_NAME "\""    
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":{" 
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\":" D_DEVICE_SENSOR_DB18S20_03_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\":" D_DEVICE_SENSOR_DB18S20_04_ADDRESS ","      
        "\"" D_DEVICE_SENSOR_DB18S20_05_NAME "\":" D_DEVICE_SENSOR_DB18S20_05_ADDRESS ""
      "}"  
    "}"
  "}";


  /**
   * @brief Should make the display layout via json, which negates the need of a custom controller each time
   * DH %d.%d, 
   * 
   */

#endif






#endif // END GUARD
