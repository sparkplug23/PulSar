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
// #define DEVICE_TESTGROUP__MOTION_DETECTORS__DESK_TESTBED

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
 *                 =BUZZER |15 (fL)            13|
 *              =SONIC TX1 |2  (fL, BIL)  (fH) 12| 
 *              =SONIC RX1 |4             (fH) 14|
 *              =RADAR TX2 |RX2/17             27| 
 *              =RADAR RX2 |TX2/16             26| TOF1EN
 *                         |5  (fL)            25| TOF1INT
 *                         |18                 33| TOF0EN
 *              LM386 SPKR |19                 32| TOF0INT
 *        OLED,TOF I2C_SDA |21  SDA     (fL) * 35| RADAR_3p18GHZ 
 *                         |RX0         (fL) * 34| PIR_LARGE
 *                         |TX0              ' VN| 
 *        OLED,TOF I2C_SCL |22  SCL          ' VP| 
 *                     NEO |23               ' EN| 
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
 
//  #define ENABLE_TEMPLATE_SECTION__SENSORS__BME
//  #define ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
//  #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION
// #define ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L0X
#define ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L1X
// #define ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_24GHZ
// #define ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ
// #define ENABLE_TEMPLATE_SECTION__SENSORS__ULTRASONIC
// #define ENABLE_TEMPLATE_SECTION__SENSORS__PIR_SMALL
// #define ENABLE_TEMPLATE_SECTION__SENSORS__PIR_LARGE

// #define ENABLE_TEMPLATE_SECTION__DRIVERS__AUDIO_SPEAKER
// #define ENABLE_TEMPLATE_SECTION__DRIVERS__AUDIO_BUZZER

// #define ENABLE_TEMPLATE_SECTION__DISPLAYS__OLED

//  // #define ENABLE_TEMPLATE_SECTION__LIGHTING
//  #define ENABLE_TEMPLATE_SECTION__ENERGY
//  #define ENABLE_TEMPLATE_SECTION__ENERGY__PZEM

 /***********************************
  * SECTION: Sensor Configs
 ************************************/  

 #define USE_MODULE_SENSORS_INTERFACE
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
 #if defined(ENABLE_TEMPLATE_SECTION__SENSORS__MOTION) || defined(ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ)
   #define USE_MODULE_SENSORS_INTERFACE
   #define USE_MODULE_SENSORS_PIR
    //  #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT
 #endif
 #define USE_MODULE_SENSORS_BUTTONS
   #define ENABLE_DEVFATURE_BUTTON__REMOVE_MQTT_BUTTONS
   #define SOC_TOUCH_VERSION_1

 #define USE_MODULE_DRIVERS_LEDS  

 #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L0X
  #define USE_MODULE_SENSORS__TOF_VL53L0X
  #define ENABLE_DEVFEATURE_I2C__SET_WIRE_INSTANCE_WITH_TWOWIRE_ZERO
  #define VL53L0X_LONG_RANGE
 #endif
 #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L1X
  #define USE_MODULE_SENSORS__TOF_VL53L1X
  #define ENABLE_DEVFEATURE_I2C__SET_WIRE_INSTANCE_WITH_TWOWIRE_ZERO
 #endif

 #define ENABLE_DEVFEATURE_SENSORS__TOF_BOTH_VL53_ACTIVE_ON_SHARED_ADD29

 /***********************************
  * SECTION: Lighting Configs
 ************************************/  

 #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_JANUARY_2025_NO_GPIO

 #define DATA_BUFFER_PAYLOAD_MAX_LENGTH 4000

 
 #define USE_LIGHTING_TEMPLATE
 DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
 R"=====(
 {
   "BusConfig":[
     {
       "Pin":23,
       "ColourOrder":"GRBW",
       "BusType":"SK6812_RGBW",
       "Start":0,
       "Length":9
     }
   ],
   "Segment0": {
     "Name":"Door Edge",
     "PixelRange": [
       0,
       9
     ],
     "ColourPalette":"Rainbow 16",
     "Effects": {
       "Function":"Static",
       "Speed":255,
       "Intensity":0,
       "Grouping":1,
       "RateMs": 1000
     },
     "BrightnessRGB": 100,
     "BrightnessCCT": 100
   }
   "BrightnessRGB": 5
 }
 )=====";

  /***********************************
   * SECTION: Display Configs
  ************************************/  
  #ifdef ENABLE_TEMPLATE_SECTION__DISPLAYS__OLED
    #define USE_MODULE_DISPLAYS_INTERFACE
    #define USE_MODULE_DISPLAYS_OLED_SH1106
      #define SHOW_SPLASH
  #endif

 /***********************************
  * SECTION: Energy Configs
 ************************************/  

 /***********************************
  * SECTION: Controller Configs
 ************************************/  

 // #define USE_MODULE_CONTROLLER_CUSTOM__LIGHTNEO_MOTION_ALERTS

 /***********************************
  * SECTION: Module/GPIO Configs
 ************************************/  

 #define USE_MODULE_TEMPLATE
 DEFINE_PGM_CTR(MODULE_TEMPLATE) 
 "{"
   "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
   "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
   "\"" D_GPIO_NUMBER "\":{"          
    //  "\"16\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
    //  "\"17\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
     #if defined(USE_MODULE_SENSORS__TOF_VL53L0X) || defined(USE_MODULE_SENSORS__TOF_VL53L1X) || defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750) || defined(USE_MODULE_ENERGY_INA219) || defined(USE_MODULE_DISPLAYS_OLED_SH1106)
     "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
     "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","    
    // "\"22\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\"," // Flipped
    // "\"21\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\"," // Flipped      
     #endif
     #ifdef USE_MODULE_SENSORS_PIR
    //  "\"23\":\""  D_GPIO_FUNCTION_PIR_1_CTR "\","
     #endif
     #ifdef USE_MODULE_SENSORS__TOF_VL53L0X
     "\"33\":\""  D_GPIO_FUNCTION__TOF_VL53L0X_XSHUT1__CTR "\","
    //  "\"33\":\""  D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR "\"," // Connected to XSHUT but not wanted. HIGH for remain enabled
    //  "\"26\":\""  D_GPIO_FUNCTION_UNUSED_FORCED_LOW_CTR "\"," // Connected to XSHUT but not wanted. HIGH for remain enabled
     #endif
     #ifdef USE_MODULE_SENSORS__TOF_VL53L1X
      "\"26\":\""  D_GPIO_FUNCTION__TOF_VL53L1X_XSHUT1__CTR "\","
    //  "\"26\":\""  D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR "\"," // Connected to XSHUT but not wanted. HIGH for remain enabled
     #endif
     #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ
     "\"35\":\""  D_GPIO_FUNCTION_PIR_2_CTR "\","
     #endif
     #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__PIR_LARGE
     "\"34\":\""  D_GPIO_FUNCTION_PIR_1_CTR "\","
     #endif
     #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__PIR_SMALL
     "\"5\":\""  D_GPIO_FUNCTION_PIR_3_CTR "\","
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
     "\"2\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
   "},"
   "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
   "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
 "}";

 #define ENABLE_DEVFEATURE_MQTT__PUBLUSH_TASMOTA_METHODS
 #define ENABLE_FEATURE_BUTTON__FACTORY_RESET_WITH_LONG_HOLD

 
 /***********************************
  * SECTION: TEMPLATE: Names
 ************************************/    

 #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "PIRLarge"
 #define D_DEVICE_SENSOR_MOTION1_FRIENDLY_NAME_LONG "RADAR 3p18GHz"
 #define D_DEVICE_SENSOR_MOTION2_FRIENDLY_NAME_LONG "PIRSmall"


 #define D_DEVICE_SENSOR_CLIMATE "PIRLarge"

 


 #define D_DEVICE_SENSOR_BME_280_NAME "BME280"
 #define D_DEVICE_SENSOR_BME_680_NAME "BME680"

 #define D_DEVICE_SENSOR_BH1750_NAME "Ambient"


 #define USE_FUNCTION_TEMPLATE
 DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
 "{"
   "\"" D_DEVICENAME "\":{"
     "\"" D_MODULE_SENSORS_PIR_CTR "\":["
       "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\","
       "\"" D_DEVICE_SENSOR_MOTION1_FRIENDLY_NAME_LONG "\","
       "\"" D_DEVICE_SENSOR_MOTION2_FRIENDLY_NAME_LONG "\""
     "],"
     "\"" D_MODULE_SENSORS__TOF_VL53L0X__CTR "\":["
       "\"" "TOF0x" "\""
     "],"
     "\"" D_MODULE_SENSORS__TOF_VL53L1X__CTR "\":["
       "\"" "TOF1x" "\""
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
     "\"" D_MODULE_SENSORS_BME_CTR "\":["
       "\"" D_DEVICE_SENSOR_CLIMATE "\""
     "]"
   "},"
   "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60}," 
   "\"MQTT_Interface_Priority\":{\"" D_MODULE_ENERGY_INTERFACE_CTR "\":1}" // Each interface will have ability to reduce its subclass mqtt "ifchanged" rate
 "}";


#endif



#endif // _CONFIG__FIRMWARE_GROUP__MOTION_DETECTORS_H

