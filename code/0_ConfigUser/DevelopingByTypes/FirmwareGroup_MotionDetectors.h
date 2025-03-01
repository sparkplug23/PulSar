#ifndef _CONFIG__FIRMWARE_GROUP__MOTION_DETECTORS_H
#define _CONFIG__FIRMWARE_GROUP__MOTION_DETECTORS_H

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
// #define DEVICE_TESTGROUP__MOTION_DETECTORS__SOCKET_ENERGY_01 // for use with outdoor christmas lights


/***
 * 
 * 
 * New device needs to be made that includes as many testers of motion on one board
 * 

 PIR Large
 PIR Small
 TOF
 Ultrasonic
 24GHz Radar

 Use a 144/m strip added to the PCB, where each LED can be used to show when motion is detected by each device
  
  
  
 */


//----------------------------------------



/**
 * @brief 
 * 
 * Testbed for motion/distance sensors
 * 
 *          fH (Boot Fail - Pulled High) → Pin must be LOW at boot, else boot may fail
 *          fL (Boot Fail - Pulled Low) → Pin must be HIGH at boot, else boot may fail
 *          key (Key Pin) → GPIO0 on DOIT DevKit v1 (not )
 *          BIL (Built-in LED) → On some boards, pin is used for onboard LED
 *                               *I ~PWM 'NC    
 *                          _____________________
 *                    3V3  |3V3     |USB|     VIN|
 *                    GND  |GND               GND| 
 *                         |15 (fL)            13| NEO0 - Motion Flash (two side by side, reverse I/O, seg0 notification per device, seg1 show distance as "lit/unlit")
 *               SONIC TX1 |2  (fL, BIL)  (fH) 12| 
 *               SONIC RX1 |4             (fH) 14| Piezo  (pitch for distance, only when close to it so its not all the time)
 *               RADAR TX2 |RX2/17             27| BUZZER (when distance is very small)
 *               RADAR RX2 |TX2/16             26|
 *                  TOF0EN |5  (fL)            25|
 *                 TOF0INT |18                 33| PIR_Small
 *                 TOF1INT |19                 32| PIR_Large
 *        OLED,TOF I2C_SDA |21          (fL) * 35|
 *                         |RX0         (fL) * 34|
 *                         |TX0              ' VN| 
 *        OLED,TOF I2C_SCL |22               ' VP| 
 *                  TOF1EN |23               ' EN| 
 *                          _____________________
 * 
 * 
 */

 #ifdef DEVICE_TESTGROUP__MOTION_DETECTORS__DESK_TESTBED
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
 #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
   #define MQTT_PORT     1883
   

   
 /***********************************
  * SECTION: System Debug Options
 ************************************/    
 ///////////////////////////////////////////// Enable Logs
 // #define DISABLE_SERIAL
 // #define DISABLE_SERIAL0_CORE
 // #define DISABLE_SERIAL_LOGGING
 // #define ENABLE_DEBUG_MANUAL_DELAYS // permits blocking delays
 
 ///////////////////////////////////////////// System Logs
 // #define ENABLE_ADVANCED_DEBUGGING
 // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
 // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
 // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
 // #define ENABLE_DEBUG_FUNCTION_NAMES
 // #define ENABLE_DEBUGFEATURE_WEBUI__SHOW_BUILD_DATETIME_IN_FOOTER
 // #define SERIAL_LOG_LEVEL_DURING_BOOT 8
 // #define ENABLE_DEBUG_LINE_HERE
 // #define ENABLE_DEBUG_LINE_HERE2
 // #define ENABLE_DEBUG_LINE_HERE3
 // #define ENABLE_DEBUG_LINE_HERE_TRACE
 // #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
 // #define USE_DEBUG_PRINT
 // #define ENABLE_DEBUGFEATURE_LOGS__FORCE_FLUSH_ON_TRANSMIT

 ///////////////////////////////////////////// Module Logs
 // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS  
 // #define ENABLE_FREERAM_APPENDING_SERIAL
 
 /***********************************
  * SECTION: System Configs
 ************************************/    

 #define SETTINGS_HOLDER 1239

 #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
 #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS
 #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
 #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device
 #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES
     
 /***********************************
  * SECTION: Network Configs
 ************************************/    

 #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
 #define USE_MODULE_NETWORK_WEBSERVER
 #define ENABLE_WEBSERVER_LIGHTING_WEBUI  

 /***********************************
  * SECTION: Sensor Configs
 ************************************/  

 // #define USE_MODULE_SENSORS_INTERFACE  
 // #define USE_MODULE_SENSORS_BUTTONS
 //   
   /**
    * @brief 
    * Button 1: Single button installs, means {"short":"iter over nice palettes", "long": "iter over 4 brightness levels"}
    * Button 2: (Short) Two button installs, this button is iter common effects || (Long) starts a demo and debug mode
    **/

 /***********************************
  * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
 ************************************/  
 
 #define ENABLE_TEMPLATE_SECTION__SENSORS__BME
 #define ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
 #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION
 // #define ENABLE_TEMPLATE_SECTION__LIGHTING
 #define ENABLE_TEMPLATE_SECTION__ENERGY
 #define ENABLE_TEMPLATE_SECTION__ENERGY__PZEM

 /***********************************
  * SECTION: Sensor Configs
 ************************************/  

 #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
   #define USE_MODULE_SENSORS_INTERFACE
     #define USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
 #endif
 #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
   #define USE_MODULE_SENSORS_BME
     #define ENABLE_DEVFEATURE_BME680
 #endif
 #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
   #define USE_MODULE_SENSORS_BH1750
 #endif
 #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__MOTION
   #define USE_MODULE_SENSORS_INTERFACE
   #define USE_MODULE_SENSORS_PIR
     #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT
 #endif
 #define USE_MODULE_SENSORS_BUTTONS
   #define ENABLE_DEVFATURE_BUTTON__REMOVE_MQTT_BUTTONS
   #define SOC_TOUCH_VERSION_1

 #define USE_MODULE_DRIVERS_LEDS  

 /***********************************
  * SECTION: Lighting Configs
 ************************************/  

 #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_JANUARY_2025

 #define DATA_BUFFER_PAYLOAD_MAX_LENGTH 4000

 /***********************************
  * SECTION: Energy Configs
 ************************************/  

 #ifdef ENABLE_TEMPLATE_SECTION__ENERGY
   #define USE_MODULE_ENERGY_INTERFACE
 #endif
 
 #ifdef ENABLE_TEMPLATE_SECTION__ENERGY__PZEM
   #define USE_MODULE_ENERGY_PZEM004T_V3
     #define ENABLE_DEVFEATURE_REDUCE_SUBORDINATE_MQTT_REPORTING_ENERGY // If energy_interface is primary reporting, reduce pzem to slower (debug only)
   #define MAX_ENERGY_SENSORS 12
   #define MAX_PZEM004T_DEVICES 12
   #define ENABLE_DEVFEATURE_PZEM004T__AUTOSEARCH
   #define DEVICENAMEBUFFER_NAME_BUFFER_LENGTH 800
 #endif

 /***********************************
  * SECTION: Controller Configs
 ************************************/  

 // #define USE_MODULE_CONTROLLER_CUSTOM__LIGHTNEO_MOTION_ALERTS

 /***********************************
  * SECTION: Lighting BusConfig Set
 ************************************/  

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
       "Length":350
     },
     {
       "Pin":12,
       "ColourOrder":"GRBW",
       "BusType":"SK6812_RGBW",
       "Start":350,
       "Length":127
     }
   ],
   "Segment0": {
     "Name":"Door Edge",
     "PixelRange": [
       0,
       302
     ],
     "ColourPalette":"Pink White Purple Grad",
     "Effects": {
       "Function":"Gradient",
       "Speed":127,
       "Intensity":0,
       "Grouping":1,
       "RateMs": 1000
     },
     "BrightnessRGB": 0,
     "BrightnessCCT": 100
   },
   "Segment1": {
     "Name":"Bottom",
     "PixelRange": [
       302,
       350
     ],
     "ColourPalette":"Rainbow 16",
     "Effects": {
       "Function":"Static",
       "Speed":200,
       "Intensity":127,
       "Grouping":1,
       "RateMs": 1000
     },
     "SegColour1": {
       "Hue": 0,
       "Sat":100,
       "BrightnessRGB":0
     },
     "BrightnessRGB": 0,
     "BrightnessCCT": 100
   },
   "Segment2": {
     "Name":"Top",
     "PixelRange": [
       350,
       477
     ],
     "ColourPalette":"Colour 01",
     "ColourType":4,
     "Effects": {
       "Function":"Solid",
       "Speed":255,
       "RateMs": 1000
     },
     "SegColour1": {
       "Hue": 25,
       "Sat":100,
       "BrightnessRGB":0
     },
     "BrightnessRGB": 0,
     "BrightnessCCT": 100
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
     "\"16\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
     "\"17\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
     #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750) || defined(USE_MODULE_ENERGY_INA219)
     "\"21\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
     "\"22\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
     #endif
     #ifdef USE_MODULE_SENSORS_PIR
     "\"23\":\""  D_GPIO_FUNCTION_PIR_1_CTR "\","
     #endif
     #ifdef USE_MODULE_SENSORS_BUTTONS
     "\"18\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
     "\"19\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
     "\"33\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\","
     #endif
     "\"4\":\"" D_GPIO_FUNCTION_LED1_CTR  "\","
     "\"5\":\"" D_GPIO_FUNCTION_LED2_CTR  "\","
     "\"2\":\"" D_GPIO_FUNCTION_LED3_CTR  "\""
   "},"
   "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
   "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
 "}";

 #define ENABLE_DEVFEATURE_MQTT__PUBLUSH_TASMOTA_METHODS
 #define ENABLE_FEATURE_BUTTON__FACTORY_RESET_WITH_LONG_HOLD

 
 /***********************************
  * SECTION: TEMPLATE: Names
 ************************************/    

 #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "Downstairs Toilet"
 #define D_DEVICE_SENSOR_CLIMATE "Downstairs Toilet"
 #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "1"
 #define D_DEVICE_SENSOR_PZEM004T_1_ADDRESS "2"
 #define D_DEVICE_SENSOR_PZEM004T_2_ADDRESS "3"
 #define D_DEVICE_SENSOR_PZEM004T_3_ADDRESS "4"
 #define D_DEVICE_SENSOR_PZEM004T_4_ADDRESS "5"
 #define D_DEVICE_SENSOR_PZEM004T_5_ADDRESS "6"
 #define D_DEVICE_SENSOR_PZEM004T_6_ADDRESS "7"
 #define D_DEVICE_SENSOR_PZEM004T_7_ADDRESS "8"
 #define D_DEVICE_SENSOR_PZEM004T_8_ADDRESS "9"
 #define D_DEVICE_SENSOR_PZEM004T_9_ADDRESS "10"
 #define D_DEVICE_SENSOR_PZEM004T_10_ADDRESS "11"
 #define D_DEVICE_SENSOR_PZEM004T_11_ADDRESS "12"


 #define D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "MainFeed"
 #define D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR "Cooker"
 #define D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR "Immersion"
 #define D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR "WashingMachine"
 #define D_SENSOR_PZEM004T_4_FRIENDLY_NAME_CTR "Dishwasher"
 #define D_SENSOR_PZEM004T_5_FRIENDLY_NAME_CTR "PumpShower"
 #define D_SENSOR_PZEM004T_6_FRIENDLY_NAME_CTR "Heating"
 #define D_SENSOR_PZEM004T_7_FRIENDLY_NAME_CTR "TumbleDryer"
 #define D_SENSOR_PZEM004T_8_FRIENDLY_NAME_CTR "Garage"
 #define D_SENSOR_PZEM004T_9_FRIENDLY_NAME_CTR "BathroomShower"
 #define D_SENSOR_PZEM004T_10_FRIENDLY_NAME_CTR "MainSockets"
 #define D_SENSOR_PZEM004T_11_FRIENDLY_NAME_CTR "KitchenSockets"
 
 #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR   D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR
 #define D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR   D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR
 #define D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR   D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR
 #define D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR   D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR
 #define D_DRIVER_ENERGY_4_FRIENDLY_NAME_CTR   D_SENSOR_PZEM004T_4_FRIENDLY_NAME_CTR
 #define D_DRIVER_ENERGY_5_FRIENDLY_NAME_CTR   D_SENSOR_PZEM004T_5_FRIENDLY_NAME_CTR
 #define D_DRIVER_ENERGY_6_FRIENDLY_NAME_CTR   D_SENSOR_PZEM004T_6_FRIENDLY_NAME_CTR
 #define D_DRIVER_ENERGY_7_FRIENDLY_NAME_CTR   D_SENSOR_PZEM004T_7_FRIENDLY_NAME_CTR
 #define D_DRIVER_ENERGY_8_FRIENDLY_NAME_CTR   D_SENSOR_PZEM004T_8_FRIENDLY_NAME_CTR
 #define D_DRIVER_ENERGY_9_FRIENDLY_NAME_CTR   D_SENSOR_PZEM004T_9_FRIENDLY_NAME_CTR
 #define D_DRIVER_ENERGY_10_FRIENDLY_NAME_CTR  D_SENSOR_PZEM004T_10_FRIENDLY_NAME_CTR
 #define D_DRIVER_ENERGY_11_FRIENDLY_NAME_CTR  D_SENSOR_PZEM004T_11_FRIENDLY_NAME_CTR


 #define D_DEVICE_SENSOR_BME_280_NAME "BME280"
 #define D_DEVICE_SENSOR_BME_680_NAME "BME680"

 #define D_DEVICE_SENSOR_BH1750_NAME "Ambient"


 #define USE_FUNCTION_TEMPLATE
 DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
 "{"
   "\"" D_DEVICENAME "\":{"
     "\"" D_MODULE_SENSORS_PIR_CTR "\":["
       "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
     "],"
     "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
       "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
     "],"
     "\"" D_MODULE_SENSORS_BUTTONS_CTR "\":["
       "\"" "WallRed" "\","
       "\"" "WallBlue" "\","
       "\"" "DoorAlert" "\""
     "],"
     "\"" D_MODULE_DRIVERS_LEDS_CTR "\":["
       "\"" "WallRed" "\","
       "\"" "WallBlue" "\","
       "\"" "StatusLED" "\""
     "],"
     "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":["
       "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
       "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\","
       "\"" D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "\","
       "\"" D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR "\","
       "\"" D_DRIVER_ENERGY_4_FRIENDLY_NAME_CTR "\","
       "\"" D_DRIVER_ENERGY_5_FRIENDLY_NAME_CTR "\","
       "\"" D_DRIVER_ENERGY_6_FRIENDLY_NAME_CTR "\","
       "\"" D_DRIVER_ENERGY_7_FRIENDLY_NAME_CTR "\","
       "\"" D_DRIVER_ENERGY_8_FRIENDLY_NAME_CTR "\","
       "\"" D_DRIVER_ENERGY_9_FRIENDLY_NAME_CTR "\","
       "\"" D_DRIVER_ENERGY_10_FRIENDLY_NAME_CTR "\","
       "\"" D_DRIVER_ENERGY_11_FRIENDLY_NAME_CTR "\""
     "],"
     "\"" D_MODULE_SENSORS_BME_CTR "\":["
       "\"" D_DEVICE_SENSOR_CLIMATE "\""
     "],"
     "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":["
       "\"" D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "\","
       "\"" D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR "\","
       "\"" D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR "\","
       "\"" D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR "\","
       "\"" D_SENSOR_PZEM004T_4_FRIENDLY_NAME_CTR "\","
       "\"" D_SENSOR_PZEM004T_5_FRIENDLY_NAME_CTR "\","
       "\"" D_SENSOR_PZEM004T_6_FRIENDLY_NAME_CTR "\","
       "\"" D_SENSOR_PZEM004T_7_FRIENDLY_NAME_CTR "\","
       "\"" D_SENSOR_PZEM004T_8_FRIENDLY_NAME_CTR "\","
       "\"" D_SENSOR_PZEM004T_9_FRIENDLY_NAME_CTR "\","
       "\"" D_SENSOR_PZEM004T_10_FRIENDLY_NAME_CTR "\","
       "\"" D_SENSOR_PZEM004T_11_FRIENDLY_NAME_CTR "\""
     "]"
   "},"
   "\"" D_SENSORADDRESS "\":{"
     "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":[" 
       D_DEVICE_SENSOR_PZEM004T_0_ADDRESS ","
       D_DEVICE_SENSOR_PZEM004T_1_ADDRESS ","
       D_DEVICE_SENSOR_PZEM004T_2_ADDRESS ","
       D_DEVICE_SENSOR_PZEM004T_3_ADDRESS ","
       D_DEVICE_SENSOR_PZEM004T_4_ADDRESS ","
       D_DEVICE_SENSOR_PZEM004T_5_ADDRESS ","
       D_DEVICE_SENSOR_PZEM004T_6_ADDRESS ","
       D_DEVICE_SENSOR_PZEM004T_7_ADDRESS ","
       D_DEVICE_SENSOR_PZEM004T_8_ADDRESS ","
       D_DEVICE_SENSOR_PZEM004T_9_ADDRESS ","
       D_DEVICE_SENSOR_PZEM004T_10_ADDRESS ","
       D_DEVICE_SENSOR_PZEM004T_11_ADDRESS ""
     "]"  
   "},"
   "\"" D_ENERGY "\":{"
       "\"DeviceCount\":12"    
   "},"
   "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60}," 
   "\"MQTT_Interface_Priority\":{\"" D_MODULE_ENERGY_INTERFACE_CTR "\":1}" // Each interface will have ability to reduce its subclass mqtt "ifchanged" rate
 "}";


#endif



#endif // _CONFIG__FIRMWARE_GROUP__MOTION_DETECTORS_H

