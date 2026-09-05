#ifndef _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_DEV_TESTBEDS_H
#define _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_DEV_TESTBEDS_H

/*********************************************************************************************\
 * Developing/Testbeds: 
 *  ** New code under test, not yet ready to be used as a template (ie may not work yet)
\*********************************************************************************************/

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"

//--------------------------------[Enable Device]-------------------------------------

// #define DEVICE_TESTBED__SENSORS_NEXTION
// #define DEVICE_TESTBED__MOTION_AUDIO
// #define DEVICE_TESTBED__HVAC_X1
// #define DEVICE_TESTBED__SWITCHES_BUTTONS
// #define DEVICE_TESTBED__GPS_DECODER_WITH_SERIAL_SNIFFERS
// #define DEVICE_TESTBED__SONOFF_BASIC_CLONE_NODEMCU
// #define DEVICE_TESTBED__FILESYSTEM

// room_sensor   : BME680, Light, PIR, RADAR
// camera_sensor : Cam gives too much heat to make BME reliable, instead, this will focus on LIGHT_SENS,LIGHT_RGB(for night vision),PIR,RADAR so ability to sense motion and capture via FS and transmit later to networked storage?


//-----------------[User Defined Devices == USE_BUILD_TYPE_ENERGY == Any Energy Monitoring Firmware]-------------------------------------


/**
 * @brief 
 * 
 * - This will be the test device, full of as many sensors as possible, and also use 4 LEDs to emulate relay outputs
 * - This will be used as a rapid/testbed device for improving as many sensors as possible (at minimum, the "...sensor" devices)
 * 
 * Modules to be tested
 * - DS18B20
 * - DHT22
 * - BME280
 * - BH1750
 * - Status LEDs (mqtt/wifi down)
 * - Nextion = OTA updating
 * - rgbW lights = Notification lights as a mode
 * - Motion sensor = including nicely having interface showing "event" of motion
 * - ESP32 Touch inputs
 * - DAC Output - Mono Speaker Audio
 * - ADC Input (probably with LDR)
 * - Audio Input? MCP3208
 * 
 * development branch only
 * 
 * Use one pixel to show segment1 effect paired with sun elevation, where sunrise to midday, then midday to sunset is different colours
 * another pixel to show segment2 effect paired with sun azimuth, where sunrise point to sunset point is full colour wheel
 * 
 * add solar value I can poll to get time of day (day/dusk/twilight/) and direction of sun movement (rising/falling)
 * 
 * Temporary sensor tester with dual DHT
 * 
 * 
 **/
#ifdef DEVICE_TESTBED__SENSORS_NEXTION
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "testbed__sensors_nextion"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testbeds"
  
  /***********************************
   * SECTION: System Debug Options
  ************************************/    

  /***********************************
   * SECTION: Enable System
  ************************************/  
  
  #define ENABLE_TEMPLATE_SECTION__SENSORS__SOLAR
  #define ENABLE_TEMPLATE_SECTION__SENSORS__BME
  #define ENABLE_TEMPLATE_SECTION__SENSORS__DHT
  #define ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
  #define ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
  #define ENABLE_TEMPLATE_SECTION__DISPLAYS__OLED_SH1106
  #define ENABLE_TEMPLATE_SECTION__DISPLAYS__NEXTION
  // #define ENABLE_TEMPLATE_SECTION__LIGHTS 
  #define ENABLE_TEMPLATE_SECTION__SENSORS__PIR
  #define ENABLE_TEMPLATE_SECTION__SENSORS__LDR_ANALOG

  /***********************************
   * SECTION: Storage Configs
  ************************************/  
 
  /***********************************
   * SECTION: System Configs
  ************************************/     

  #define USE_MODULE_CORE__CRASH_RECORDER

  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define FIRMWARE_DEFAULT__INCLUDE_WEBSERVER_FULL
  
  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
    #define USE_MODULE_SENSORS_INTERFACE
    #define USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
    #define USE_MODULE_SENSORS_DS18X20
    #define DS18X20_MAX_SENSORS 20
    #define ENABLE_DEBUG_MQTT_CHANNEL_DB18X20    
  #endif 
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
    #define USE_MODULE_SENSORS_BME      
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__DHT
    #define USE_MODULE_SENSORS_DHT      
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__SOLAR
    #define USE_MODULE_SENSORS_SUN_TRACKING     
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
    #define USE_MODULE_SENSORS_BH1750
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__PIR
    #define USE_MODULE_SENSORS_PIR
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__LDR_ANALOG  // leave until analog sensor is redone, then use it. actually, analog although sensor, should become a core part (like i2c etc)
    #define USE_MODULE_SENSORS_LDR_BASIC_DIGITAL
    #define USE_MODULE_SENSORS_LDR_BASIC_ANALOG
  #endif
  
  /***********************************
   * SECTION: Display Configs
  ************************************/  

  #if defined(ENABLE_TEMPLATE_SECTION__DISPLAYS__OLED_SH1106) || defined(ENABLE_TEMPLATE_SECTION__DISPLAYS__NEXTION)
    #define USE_MODULE_DISPLAYS_INTERFACE
  #endif

  #ifdef ENABLE_TEMPLATE_SECTION__DISPLAYS__OLED_SH1106
  #define USE_MODULE_DISPLAYS_OLED_SH1106
    #define ENABLE_DEVFEATURE_DISPLAY__INVERT
    #define SHOW_SPLASH
  #endif

  #ifdef ENABLE_TEMPLATE_SECTION__DISPLAYS__NEXTION
    #define USE_MODULE_DISPLAYS_NEXTION
    #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
    #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE
    #define ENABLE_DEVFEATURE_NEXTION_DISPLAY      
    #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 921600
    #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER 1
    #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER_SERIAL1_HVAC_DESK
    #define NEXTION_DEFAULT_PAGE_NUMBER 5

    DEFINE_PGM_CTR(DISPLAY_TEMPLATE)
    R"=====(
    {
      "DisplayMode":1,
      "PageNames": [
        "boot_flash",
        "boot",
        "message",
        "multiline",
        "logger",
        "heating",
        "hvacdesk",
        "main"
      ],
      "DefaultPageName":"hvacdesk",
      "DefaultBaud":115200,
      "TargetBaud":115200,
      "ObjectNameID": {
        "hdIconHAI": 2,
        "hdTimeHAI": 6,
        "hdBoostHAI": 11,
        "hdAutoHAI": 16,
        "powHAI": 21,
        "hdIconFLR": 3,
        "hdTimeFLR": 7,
        "hdBoostFLR": 12,
        "hdAutoFLR": 17,
        "powFLR": 22,
        "hdIconFAN": 4,
        "hdTimeFAN": 8,
        "hdBoostFAN": 13,
        "hdAutoFAN": 18,
        "powFAN": 23,
        "hdIconOIL": 5,
        "hdTimeOIL": 9,
        "hdBoostOIL": 14,
        "hdAutoOIL": 19,
        "powOIL": 24
      }
    }
    )=====";

  #endif // ENABLE_TEMPLATE_SECTION__DISPLAYS__NEXTION

  /***********************************
   * SECTION: Driver Configs
  ************************************/  
        
  #define USE_MODULE_DRIVERS_INTERFACE 
  #define USE_MODULE_DRIVERS_LEDS
    #define ENABLE_DRIVERS_LEDS__STATUS_LED

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  #ifdef ENABLE_TEMPLATE_SECTION__LIGHTS

    #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE
    // #define ENABLE_FEATURE_LIGHTING__DISABLE_WEBPAGE_TO_REDUCE_MEMORY_USAGE // save memory by removing web page

    // 13, 18, 19, 22, 23, 25, 26, 27       USED
    // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
    // 21 = on PCB (4 pixels)
    // 32 = external
    #define USE_LIGHTING_TEMPLATE
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":4,
          "ColourOrder":"GRB",
          "BusType":"WS2812_RGB",
          "Start":0,
          "Length":16
        }
      ],
      "Segment0": {
        "PixelRange": [
          0,
          16
        ],
        "ColourPalette":"Christmas RGPBO",
        "Effects": {
          "Function":"Static",
          "Speed":255,
          "Intensity":255,
          "Decimate":0,
          "Grouping":1,
          "TimeMs": 200,
          "RateMs": 10000
        },
        "BrightnessRGB": 100
      },
      "BrightnessRGB": 5
    }
    )=====";
  #endif // ENABLE_TEMPLATE_SECTION__LIGHTS

  /***********************************
   * SECTION: Energy Configs
  ************************************/  

  /***********************************
   * SECTION: Controller Configs
  ************************************/  

  /***********************************
   * SECTION: GPIO Template
  ************************************/  
  
  #define ENABLE_DEBUG_POINT_MODULE_TEMPLATE_BOOT_SPLASH
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"   
      /**
       * @brief Right side
       **/
      #ifdef USE_MODULE_SENSORS_DS18X20   
      "\"23\":\"" D_GPIO_DS18B20_CTR "1" "\","
      "\"22\":\"" D_GPIO_DS18B20_CTR "2" "\","
      #endif // USE_MODULE_SENSORS_DS18X20
      // GPIO1 - TX0 - Debug Serial TX
      // GPIO3 - RX0 - Debug Serial RX
      #ifdef USE_MODULE_SENSORS_LDR_BASIC_DIGITAL
      "\"21\":\"" D_GPIO_LDR_BASIC_DIGITAL1_CTR "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DHT
      "\"18\":\"" D_GPIO_DHT22_CTR "1" "\","
      "\"19\":\"" D_GPIO_DHT22_CTR "2" "\","      
      #endif
      #ifdef USE_MODULE_SENSORS_PIR
      "\"5\":\""  D_GPIO_SWT_CTR "1" "\","
      #endif
      #ifdef USE_MODULE_DISPLAYS_NEXTION
      "\"17\":\"" D_GPIO_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_NEXTION_RX_CTR "\","
      #endif
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      // "\"4\":\"" D_GPIO_RGB_DATA_CTR  "\","
      #endif 
      #ifdef USE_MODULE_DRIVERS_LEDS
      "\"2\":\"" D_GPIO_LED_CTR "1" "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_IR_RECEIVER
      "\"15\":\"" D_GPIO_IR_RECV_CTR "\","
      #endif
      // GPIO0 - ADC2 CH1
      /**
       * @brief Left side
       **/
      // EN
      // 36 - INPUT ONLY - VP
      // 39 - INPUT ONLY - VN
      #ifdef USE_MODULE_SENSORS_LDR_BASIC_ANALOG
      "\"34\":\"" D_GPIO_LDR_BASIC_ANALOG1_CTR "\"," // adc1_6 // INPUT ONLY
      #endif
      // 35 - INPUT ONLY - adc1_7
      // 32 - Touch9 (Debug Header 1)
      // 33 - Touch8 (Debug Header 2)
      // 25 - DAC1 = LM386 Amplifier Module (Debug Header 3)
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750) || defined(ENABLE_TEMPLATE_SECTION__DISPLAYS__OLED_SH1106)
      "\"26\":\"" D_GPIO_I2C_SCL_CTR   "\","
      "\"27\":\"" D_GPIO_I2C_SDA_CTR   "\","  
      #endif    
      // 14 (Debug Header 4)
      // 12 (Debug Header 5)
      // 13 (Debug Header 6) + RX
      //rx receivers?
      // Can I introduce a way that a comma at the end, does not make a broken json?
      "\"0\":\"" D_GPIO_KEY_INV_CTR "1"   "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";



  /**
   * @brief Drivers and Sensors for HVAC zones
   **/
  #define D_DEVICE_HEATER_0_NAME "Dryer"
  #define D_DEVICE_HEATER_1_NAME "FloorMat"
  #define D_DEVICE_HEATER_2_NAME "FanHeater"
  #define D_DEVICE_HEATER_3_NAME "OilRadiator"

  #define D_DEVICE_SENSOR_DHT_0_NAME "DHT1"
  #define D_DEVICE_SENSOR_DHT_1_NAME "DHT2"

// {"NumDevices":4,"DeviceNameIndex":[-1,-1,-1,-1],"AddressList":[[40,140,131,47,0,0,0,230],[40,18,77,49,0,0,0,233],[40,233,112,49,0,0,0,11],[40,165,161,47,0,0,0,189]]}

  /** 
   * MainBoard
   * 
  [40,255,100,29,194,124,254,111]
  [40,255,100,29,205,206,170,25]
  [40,255,100,29,195,134,175,63]
   * */
  #define D_DEVICE_SENSOR_DB18S20_01_NAME        "MainBoard-1"
  #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,255,100,29,194,124,254,111]"

  #define D_DEVICE_SENSOR_DB18S20_05_NAME        "MainBoard-2"
  #define D_DEVICE_SENSOR_DB18S20_05_ADDRESS     "[40,255,100,29,205,206,170,25]"

  #define D_DEVICE_SENSOR_DB18S20_03_NAME        "MainBoard-3"
  #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,255,100,29,195,134,175,63]"

  #define D_DEVICE_SENSOR_DB18S20_04_NAME        "MainBoard-4"
  #define D_DEVICE_SENSOR_DB18S20_04_ADDRESS     "[40,18,77,49,0,0,0,233]" //233 4

  #define D_DEVICE_SENSOR_DB18S20_02_NAME        "MainBoard-5"
  #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,255,100,29,205,202,237,231]"

  #define D_DEVICE_SENSOR_BME_280_NAME "BME280"
  #define D_DEVICE_SENSOR_BME_680_NAME "Office"

  #define D_DEVICE_SENSOR_BH1750_NAME "Ambient"

  #define D_DEVICE_SENSOR_CURRENT "LEDStrip"

  
  #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "1"
  #define D_DEVICE_SENSOR_PZEM004T_1_ADDRESS "2"
  #define D_DEVICE_SENSOR_PZEM004T_2_ADDRESS "3"
  #define D_DEVICE_SENSOR_PZEM004T_3_ADDRESS "4"
  
  #define D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR D_DEVICE_HEATER_0_NAME
  #define D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR D_DEVICE_HEATER_1_NAME
  #define D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR D_DEVICE_HEATER_2_NAME
  #define D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR D_DEVICE_HEATER_3_NAME 
  
  #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_0_NAME
  #define D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_1_NAME
  #define D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_2_NAME
  #define D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_3_NAME

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_ENERGY "\":{"
        "\"DeviceCount\":4"    
    "},"
    "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":{"
        "\"DeviceCount\":4"    
    "},"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE__SENSORS__INTERFACE__CTR "\":["
        "\"" "Internal" "\""
      "],"  
      "\"" D_MODULE__DRIVERS__RELAY__CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "],"
      "\"" D_MODULE__SENSORS__SWITCHES__CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "],"
      "\"" D_MODULE__SENSORS__DB18S20__CTR "\":["
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_05_NAME "\""
      "],"
      "\"" D_MODULE__SENSORS__DHT__CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DHT_1_NAME "\""
      "],"
      "\"" D_MODULE__SENSORS__SUN_TRACKING__CTR "\":["
        "\"" "Desk" "\""
      "],"  
      "\"" D_MODULE__SENSORS__BME__CTR "\":["
        "\"" D_DEVICE_SENSOR_BME_280_NAME "\","
        "\"" D_DEVICE_SENSOR_BME_680_NAME "\""
      "],"
      "\"" D_MODULE_ENERGY_INA219_CTR "\":["
        "\"" D_DEVICE_SENSOR_CURRENT "\""
      "],"
      "\"" D_MODULE__SENSORS__BH1750__CTR "\":["
        "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
      "],"
      "\"" D_MODULE__ENERGY__INTERFACE__CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":["
        "\"" D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE__SENSORS__DB18S20__CTR "\":{" 
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\":" D_DEVICE_SENSOR_DB18S20_03_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\":" D_DEVICE_SENSOR_DB18S20_04_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_05_NAME "\":" D_DEVICE_SENSOR_DB18S20_05_ADDRESS ""
      "}"  
    "},"
    "\"" D_MODULE__NETWORK__MQTT__CTR "\":{"
      "\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120,"
      "\"" D_REALTIME_SLOWDOWN "\":0"
    "},"
    "\"MQTTSubscribe\":["
      "\"openhab_broadcast/nextion/group/hvac_home\","
      "\"openhab_broadcast/nextion/group/hvac_desk_power\""
    "]"
  "}";




  #define D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "motion"

  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)  // needs appending method 
  "{"
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE__SENSORS__SWITCHES__CTR "\","
        "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_TASK_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":0," 
        "\"State\":\"Follow\""
      "}"
    "}"
  "}";

#endif




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
 *                 =BUZZER |15 (fL)            13| RADAR_OUT_MOT4
 *              =SONIC TX1 |2  (fL, BIL)  (fH) 12| 
 *              =SONIC RX1 |4             (fH) 14|
 *              =RADAR TX2 |RX2/17             27| 
 *              =RADAR RX2 |TX2/16             26| TOF1EN
 *         PIR_SMALL_MOT2  |5  (fL)            25| TOF1INT
 *                    aud  |18                 33| TOF0EN
 *              LM386 SPKR |19                 32| TOF0INT
 *        OLED,TOF I2C_SDA |21  SDA     (fL) * 35| RADAR_3p18GHZ_MOT3
 *                         |RX0         (fL) * 34| PIR_LARGE_MOT1
 *                         |TX0              ' VN| 
 *        OLED,TOF I2C_SCL |22  SCL          ' VP| 
 *                     NEO |23               ' EN| 
 *                          _____________________
 * 
 * 
 */

 #ifdef DEVICE_TESTBED__MOTION_AUDIO
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

    #define SECONDS_FROM_BUILDTIME_TO_ENABLE_SPLASHING_TELEMETRY 10
   

   
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
 // #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS_OLD
 // #define USE_DEBUG_PRINT
 // #define ENABLE_DEBUGFEATURE_LOGS__FORCE_FLUSH_ON_TRANSMIT

//new feature to only show logs for a specific module when developing code
  // #define ENABLE_DEBUGFEATURE_LOGGING__RESTRICT_SERIAL_LOGS_TO_MODULE 5028
  // #define ENABLE_DEBUGFEATURE_LOGGING__RESTRICT_SERIAL_LOGS_TO_MODULE_ARRAY [1, 2]


 ///////////////////////////////////////////// Module Logs
 // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS  
 // #define ENABLE_FREERAM_APPENDING_SERIAL
 
 /***********************************
  * SECTION: System Configs
 ************************************/    

 #define SETTINGS_HOLDER 1239

 
 
 #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  // until devices can reliably be used without compiling per device
 
     
 /***********************************
  * SECTION: Network Configs
 ************************************/    

 
   

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

  #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION
  #define ENABLE_TEMPLATE_SECTION__SENSORS__BUTTONS
  // #define ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L0X
  #define ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L1X
  #define ENABLE_TEMPLATE_SECTION__SENSORS__ULTRASONIC
  #define ENABLE_TEMPLATE_SECTION__SENSORS__PIR_LARGE        // PIR1
  #define ENABLE_TEMPLATE_SECTION__SENSORS__PIR_SMALL        // PIR2
  #define ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ    // PIR3
  #define ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_24GHZ      // PIR4


// #define ENABLE_TEMPLATE_SECTION__DRIVERS__AUDIO_SPEAKER
// #define ENABLE_TEMPLATE_SECTION__DRIVERS__AUDIO_BUZZER

// #define ENABLE_TEMPLATE_SECTION__LIGHTS__NEOPIXEL

#define ENABLE_TEMPLATE_SECTION__DISPLAYS__OLED

//  // #define ENABLE_TEMPLATE_SECTION__LIGHTING
//  #define ENABLE_TEMPLATE_SECTION__ENERGY
//  #define ENABLE_TEMPLATE_SECTION__ENERGY__PZEM

 /***********************************
  * SECTION: Sensor Configs
 ************************************/  

 #define USE_MODULE_SENSORS_INTERFACE
 #if defined(ENABLE_TEMPLATE_SECTION__SENSORS__MOTION) || defined(ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ)
   #define USE_MODULE_SENSORS_INTERFACE
   #define USE_MODULE_SENSORS_PIR
    //  #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT
 #endif
 #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BUTTONS
  #define USE_MODULE_SENSORS_BUTTONS
    #define ENABLE_DEVFATURE_BUTTON__REMOVE_MQTT_BUTTONS
    #define SOC_TOUCH_VERSION_1
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__ULTRASONIC
    #define USE_MODULE_SENSORS_SR04
    #define ENABLE_DEVFEATURE_SR04_FILTERING_EMA
    #define ENABLE_DEVFEATURE_SR04_FILTERING_DEMA
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_24GHZ
    #define USE_MODULE_SENSORS__RADAR_HLK_LD2410
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L0X
    #define USE_MODULE_SENSORS__TOF_VL53L0X
    #define ENABLE_DEVFEATURE_I2C__SET_WIRE_INSTANCE_WITH_TWOWIRE_ZERO
    #define VL53L0X_LONG_RANGE
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L1X
    #define USE_MODULE_SENSORS__TOF_VL53L1X
    #define ENABLE_DEVFEATURE_I2C__SET_WIRE_INSTANCE_WITH_TWOWIRE_ZERO
    // #define USE_SENSORS_TOFVL_AVERAGING_DATA
  #endif

//  #define ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING

//  #define ENABLE_DEVFEATURE_SENSORS__TOF_BOTH_VL53_ACTIVE_ON_SHARED_ADD29

 /***********************************
  * SECTION: Lighting Configs
 ************************************/  

  #ifdef ENABLE_TEMPLATE_SECTION__LIGHTS__NEOPIXEL

    #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_JANUARY_2025_NO_GPIO

    


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

 #endif

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
     #if defined(USE_MODULE_SENSORS__TOF_VL53L0X) || defined(USE_MODULE_SENSORS__TOF_VL53L1X) || defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750) || defined(USE_MODULE_ENERGY_INA219) || defined(USE_MODULE_DISPLAYS_OLED_SH1106)
     "\"21\":\"" D_GPIO_I2C_SDA_CTR   "\","
     "\"22\":\"" D_GPIO_I2C_SCL_CTR   "\","    
    // "\"22\":\"" D_GPIO_I2C_SDA_CTR   "\"," // Flipped
    // "\"21\":\"" D_GPIO_I2C_SCL_CTR   "\"," // Flipped      
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
     #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__PIR_LARGE //c
     "\"34\":\""  D_GPIO_PIR_CTR "1" "\","
     #endif
     #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__PIR_SMALL
     "\"5\":\""  D_GPIO_PIR_CTR "2" "\"," //COR
     #endif
     #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ
     "\"35\":\""  D_GPIO_PIR_CTR "3" "\","
     #endif
     #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_24GHZ
     "\"13\":\""  D_GPIO_PIR_CTR "4" "\","
     #endif
     #ifdef USE_MODULE_SENSORS__RADAR_HLK_LD2410
     "\"17\":\""  D_GPIO_LD2410_TX_CTR "\","
     "\"16\":\""  D_GPIO_LD2410_RX_CTR "\","
     #endif
     #ifdef USE_MODULE_SENSORS_BUTTONS
     "\"0\":\"" D_GPIO_KEY1_INV_CTR  "\""
     #endif
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
     "\"" D_MODULE__SENSORS__PIR__CTR "\":["
       "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\","
       "\"" D_DEVICE_SENSOR_MOTION1_FRIENDLY_NAME_LONG "\","
       "\"" D_DEVICE_SENSOR_MOTION2_FRIENDLY_NAME_LONG "\""
     "],"
     "\"" D_MODULE__SENSORS__TOF_VL53L0X__CTR "\":["
       "\"" "TOF_VL53L0X" "\""
     "],"
     "\"" D_MODULE__SENSORS__TOF_VL53L1X__CTR "\":["
       "\"" "TOF_VL53L1X" "\""
     "],"
     "\"" D_MODULE__SENSORS__SR04__CTR "\":["
       "\"" "SRO4" "\""
     "],"
     "\"" D_MODULE__SENSORS__RADAR_HLK_LD2410__CTR"\":["
       "\"" "Radar" "\""
     "],"
     "\"" D_MODULE__SENSORS__SWITCHES__CTR "\":["
       "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
     "],"
     "\"" D_MODULE__SENSORS__BUTTONS__CTR "\":["
       "\"" "WallRed" "\","
       "\"" "WallBlue" "\","
       "\"" "DoorAlert" "\""
     "],"
     "\"" D_MODULE__DRIVERS__LEDS__CTR "\":["
       "\"" "WallRed" "\","
       "\"" "WallBlue" "\","
       "\"" "StatusLED" "\""
     "],"
     "\"" D_MODULE__SENSORS__BME__CTR "\":["
       "\"" D_DEVICE_SENSOR_CLIMATE "\""
     "]"
   "},"    
    "\"" D_MODULE__NETWORK__MQTT__CTR "\":{"
      "\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120,"
      "\"" D_REALTIME_SLOWDOWN "\":0"
    "},"
   "\"MQTT_Interface_Priority\":{\"" D_MODULE__ENERGY__INTERFACE__CTR "\":1}" // Each interface will have ability to reduce its subclass mqtt "ifchanged" rate
 "}";


#endif





/**
 * @brief 
 * 
 * HVAC X1
 * 
 *          fH (Boot Fail - Pulled High) → Pin must be LOW at boot, else boot may fail
 *          fL (Boot Fail - Pulled Low) → Pin must be HIGH at boot, else boot may fail
 *          key (Key Pin) → GPIO0 on DOIT DevKit v1 (not )
 *          BIL (Built-in LED) → On some boards, pin is used for onboard LED
 *                               *I ~PWM 'NC    
 *                          _____________________
 *                    3V3  |3V3     |USB|     VIN|
 *                    GND  |GND               GND| 
 *                 =BUZZER |15 (fL)            13| RADAR_OUT_MOT4
 *              =SONIC TX1 |2  (fL, BIL)  (fH) 12| 
 *              =SONIC RX1 |4             (fH) 14|
 *              =RADAR TX2 |RX2/17             27| 
 *              =RADAR RX2 |TX2/16             26| TOF1EN
 *         PIR_SMALL_MOT2  |5  (fL)            25| TOF1INT
 *                    aud  |18                 33| TOF0EN
 *              LM386 SPKR |19                 32| TOF0INT
 *        OLED,TOF I2C_SDA |21  SDA     (fL) * 35| RADAR_3p18GHZ_MOT3
 *                         |RX0         (fL) * 34| PIR_LARGE_MOT1
 *                         |TX0              ' VN| 
 *        OLED,TOF I2C_SCL |22  SCL          ' VP| 
 *                     NEO |23               ' EN| 
 *                          _____________________
 * 
 * 
 */
#ifdef DEVICE_TESTBED__HVAC_X1
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
               
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY  
  #define USE_MODULE_DRIVERS_LEDS

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"2\":\""  D_GPIO_REL1_INV_CTR  "\","
      #endif
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"0\":\""  D_GPIO_SWT1_INV_CTR  "\","
      #endif
      "\"4\":\""  D_GPIO_LED1_INV_CTR "\""   // builtin led
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE__SENSORS__INTERFACE__CTR"\":["
        "\"" "System" "\""
      "],"
      "\"" D_MODULE__DRIVERS__RELAY__CTR "\":["
        "\"" "Relay" "\""
      "],"
      "\"" D_MODULE__SENSORS__SWITCHES__CTR "\":["
        "\"" "Button" "\""
      "]"
    "},"    
    "\"" D_MODULE__NETWORK__MQTT__CTR "\":{"
      "\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120,"
      "\"" D_REALTIME_SLOWDOWN "\":0"
    "}"
  "}";
  
#endif


/**
 * @brief Device will be made that contains different GPIO testing for the esp32
 * * leave 22,21 for possible OLED later
 * * 4 Buttons  (Pull down when active) [12, 14, 18, 19]
 * * 2 Touch Buttons [32,33]                                 (Solder wires to a pad, then tape or something over it so I can test touch through it (and other materials)) 
 * * 2 Switches (Pull down when active) [25, 26, 27, 13]
 * * 4 LEDs
 * * * 2 as Relays
 * * * 2 as LED module for status 
 * 
 * 9 inputs (2 touch, 4 push, 2 switch, esp32 button)
 * 5 leds (4 leds, esp32 led)
 * 
 * ONBOARD_LED should be used now for connection status by default from now on, unless another GPIO takes it over
 * 
 * Two types of boards I cna test here
 * LEDS_ARE_RELAYS: sonoff pro 4, single push toggle, hold button is relay on timer.
 * 
 * Use this to develop auto rule adds via name parsing, SWT1>REL3?FOLLOW etc
 * 
 * 
 */
#ifdef DEVICE_TESTBED__SWITCHES_BUTTONS
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

  #define USE_TEMPLATED_DEFAULT_OTA_RECOVERY_METHODS

  #define DEVICENAMEBUFFER_NAME_BUFFER_LENGTH 800

  /***********************************
   * SECTION: Network Configs
  ************************************/    

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_BUTTONS    
    #define SOC_TOUCH_VERSION_1

  #define USE_MODULE_SENSORS_SWITCHES
    

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
        
  /***********************************
   * SECTION: Energy Configs
  ************************************/  

  /***********************************
   * SECTION: Controller Configs
  ************************************/  

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
      "\"2\":\"" D_GPIO_LED1_CTR  "\","  // BUILTIN LED as new Status LED, to reflect Network and Relay0 status
      #else
      "\"2\":\""  D_GPIO_LED_CTR "3"  "\"," //builtin BLUE
      #endif  
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"5\":\"" D_GPIO_KEY_INV_CTR "1"  "\","
      "\"4\":\"" D_GPIO_KEY_INV_CTR "2"  "\","
      "\"26\":\"" D_GPIO_KEY_INV_CTR "3" "\","
      "\"15\":\"" D_GPIO_KEY_INV_CTR "4" "\","
      #ifdef SOC_TOUCH_VERSION_1
      "\"32\":\"" D_GPIO_KEY_TOUCH_CTR "5" "\","
      "\"33\":\"" D_GPIO_KEY_TOUCH_CTR "6" "\","
      "\"0\":\"" D_GPIO_KEY_INV_CTR "7"  "\","
      #endif
      #endif
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"18\":\"" D_GPIO_SWT_INV_CTR "1"  "\","
      "\"19\":\"" D_GPIO_SWT_INV_CTR "2" "\","
      #endif  
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"27\":\"" D_GPIO_REL_CTR "1" "\","
      "\"14\":\"" D_GPIO_REL_CTR "2" "\","
      "\"12\":\"" D_GPIO_REL_CTR "3" "\","
      "\"13\":\"" D_GPIO_REL_CTR "4" "\","
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
      "\"" D_MODULE__SENSORS__PIR__CTR "\":["
        "\"" D_DEVICE_UNIQUE_NAME "\""
      "],"
      "\"" D_MODULE__SENSORS__SWITCHES__CTR "\":["
        "\"" "SwitchEdge" "\","
        "\"" "SwitchClose" "\""
      "],"
      "\"" D_MODULE__SENSORS__BUTTONS__CTR "\":["
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
      "\"" D_MODULE__SENSORS__DB18S20__CTR "\":["
        "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\""
      "],"      
      "\"" D_MODULE__ENERGY__INTERFACE__CTR "\":["
        "\"" D_DEVICE_UNIQUE_NAME "\""
      "],"
      "\"" D_MODULE__SENSORS__BME__CTR "\":["
        "\"" D_DEVICE_UNIQUE_NAME "\""
      "],"
      "\"" D_MODULE__DRIVERS__RELAY__CTR "\":["
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
      "\"" D_MODULE__SENSORS__DB18S20__CTR "\":{" 
        "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\":" D_DEVICE_SENSOR_DB18S20_0_ADDRESS ","
      "},"  
      "\"" D_MODULE__ENERGY__INTERFACE__CTR "\":[" 
        D_DEVICE_SENSOR_PZEM004T_0_ADDRESS ""
      "]"  
    "},"    
    "\"" D_MODULE__NETWORK__MQTT__CTR "\":{"
      "\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120,"
      "\"" D_REALTIME_SLOWDOWN "\":0"
    "},"
    "\"MQTT_Interface_Priority\":{\"" D_MODULE__ENERGY__INTERFACE__CTR "\":1}" // Each interface will have ability to reduce its subclass mqtt "ifchanged" rate
  "}";


    
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  R"=====(
  {
    "AppendRules": [
      {
        "Name": "Switch0 toggles Relay0",
        "Trigger": {
          "Module": "switches",
          "Function": "StateChanged",
          "DeviceName": 0,
          "State": "On"
        },
        "Command": {
          "Module": "relays",
          "Function": "SetPower",
          "DeviceName": 0,
          "State": 2
        }
      },
      {
        "Name": "Switch1 toggles Relay1",
        "Trigger": {
          "Module": "switches",
          "Function": "StateChanged",
          "DeviceName": 1,
          "State": "On"
        },
        "Command": {
          "Module": "relays",
          "Function": "SetPower",
          "DeviceName": 1,
          "State": 2
        }
      },
      {
        "Name": "Button0 toggles Relay2",
        "Trigger": {
          "Module": "buttons",
          "Function": "StateChanged",
          "DeviceName": 0,
          "State": "On"
        },
        "Command": {
          "Module": "relays",
          "Function": "SetPower",
          "DeviceName": 2,
          "State": 2
        }
      },
      {
        "Name": "Button1 toggles Relay3",
        "Trigger": {
          "Module": "buttons",
          "Function": "StateChanged",
          "DeviceName": 1,
          "State": "On"
        },
        "Command": {
          "Module": "relays",
          "Function": "SetPower",
          "DeviceName": 3,
          "State": 2
        }
      }
    ]
  }
  )=====";

#endif



/**
 * @brief 
 * 
 * Testbed for GPS receivers, with TTYL on each serial pin (rx and tx), and across it (for ublox u-center)
 * 
 *          fH (Boot Fail - Pulled High) → Pin must be LOW at boot, else boot may fail
 *          fL (Boot Fail - Pulled Low) → Pin must be HIGH at boot, else boot may fail
 *          key (Key Pin) → GPIO0 on DOIT DevKit v1 (not )
 *          BIL (Built-in LED) → On some boards, pin is used for onboard LED
 *                               *I ~PWM 'NC    
 *                          _____________________
 *                    3V3  |3V3     |USB|     VIN|
 *                    GND  |GND               GND| 
 *                 =BUZZER |15 (fL)            13| RADAR_OUT_MOT4
 *              =SONIC TX1 |2  (fL, BIL)  (fH) 12| 
 *              =SONIC RX1 |4             (fH) 14|
 *              =RADAR TX2 |RX2/17             27| 
 *              =RADAR RX2 |TX2/16             26| TOF1EN
 *         PIR_SMALL_MOT2  |5  (fL)            25| TOF1INT
 *                    aud  |18                 33| TOF0EN
 *              LM386 SPKR |19                 32| TOF0INT
 *        OLED,TOF I2C_SDA |21  SDA     (fL) * 35| RADAR_3p18GHZ_MOT3
 *                         |RX0         (fL) * 34| PIR_LARGE_MOT1
 *                         |TX0              ' VN| 
 *        OLED,TOF I2C_SCL |22  SCL          ' VP| 
 *                     NEO |23               ' EN| 
 *                          _____________________
 * 
 * 
 */

 #ifdef DEVICE_TESTBED__GPS_DECODER_WITH_SERIAL_SNIFFERS
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
 // #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS_OLD
 // #define USE_DEBUG_PRINT
 // #define ENABLE_DEBUGFEATURE_LOGS__FORCE_FLUSH_ON_TRANSMIT

//new feature to only show logs for a specific module when developing code
  // #define ENABLE_DEBUGFEATURE_LOGGING__RESTRICT_SERIAL_LOGS_TO_MODULE 5028
  // #define ENABLE_DEBUGFEATURE_LOGGING__RESTRICT_SERIAL_LOGS_TO_MODULE_ARRAY [1, 2]


 ///////////////////////////////////////////// Module Logs
 // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS  
 // #define ENABLE_FREERAM_APPENDING_SERIAL
 
 /***********************************
  * SECTION: System Configs
 ************************************/    

 #define SETTINGS_HOLDER 1239

 
 
 #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  // until devices can reliably be used without compiling per device
 
     
 /***********************************
  * SECTION: Network Configs
 ************************************/    

 #define FIRMWARE_DEFAULT__INCLUDE_WEBSERVER_FULL
 #define ENABLE_FEATURE_WEBSERVER__ADVANCED_URL_LIST
 #define ENABLE_FEATURE_WEBSERVER__GPS_CONFIG_DEVICE
 #define ENABLE_FEATURE_WEBSERVER__GPS_OVERVIEW
   

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
  // #define ENABLE_GROUPFEATURE__GPS__WITH_SERIAL_POLLING
  // #define ENABLE_GROUPFEATURE__GPS__WITH_SERIAL_INTERRUPTS_BUFFER

 /***********************************
  * SECTION: Driver Configs
 ************************************/  


  // /**
  //  *  GPS
  //  * */
  // // #ifdef ENABLE_GROUPFEATURE__GPS_SERIAL
    #define USE_MODULE_SENSORS_GPS_SERIAL
  //   #define ENABLE_GPS_PARSER_NMEA
    #define ENABLE_GPS_PARSER_UBX
  //   #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
    #define NMEAGPS_DERIVED_TYPES // for ubx packets
  //   // #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
  //   #define NMEAGPS_PARSE_SAVE_MILLIS
    #define gpsPort Serial2
  //   // #define D_GPS_BAUD_RATE_FAST    921600
    #define D_GPS_BAUD_RATE_DEFAULT 115200
    // #define D_GPS_BAUD_RATE_DEFAULT 921600

    // #define ENABLE_DEVFEATURE_GPS__PARSER_TEST1

    #define ENABLE_DEVFEATURE__START_STATIC_LOOP

  //   // #define USE_DEVFEATURE_GPS_POLLING_INPUT

  //   // #define ENABLE_DEVFEATURE_GPS_SERIAL__NEW_CODE

  // #define ENABLE_FEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 40

    
  //   #define NMEAGPS_PARSE_SAVE_MILLIS
  //   #define gpsPort Serial2
  //   #define D_GPS_BAUD_RATE_DEFAULT 9600
  //   // #define D_GPS_BAUD_RATE_DEFAULT 115200
  //   // #define D_GPS_BAUD_RATE_DEFAULT 230400
  //   // #define D_GPS_BAUD_RATE_DEFAULT 460800

  //   // #define D_GPS_BAUD_RATE_DEFAULT 115200
  //   // #define D_GPS_TX_PIN_DEFAULT 19
  //   // #define D_GPS_RX_PIN_DEFAULT 18

  //   #define USE_DEVFEATURE_GPS_POLLING_INPUT

  //   #define USE_MODULE_CORE__SERIAL

  //   // #define ENABLE_DEVFEATURE_GPS_SERIAL__NEW_CODE

  //   #define ENABLE_DEVFEATURE_USE_HARDWARE_SERIAL2_FOR_GPS

  //   #define USE_DEVFEATURE_UBLOX_GLOBAL
    
  //   // #define ENABLE_DEVFEATURE__ENABLE_UBX_PARSER_IN_CLASS

  //   #define USE_DEVFEATURE__UBLOX_TEST_CLASS




  //   // #define USE_MODULE_DRIVERS_INTERFACE
  //   // #define USE_MODULE_DRIVERS_SERIAL_UART
  //   #define ENABLE_HARDWARE_UART_1
  //   #define HARDWARE_UART_1_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
  // // #endif // ENABLE_GROUPFEATURE__GPS_SERIAL

  // /**
  //  *  GPS
  //  * */
  // #define USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS
  // #ifdef USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS
  //   #define USE_MODULE_SENSORS_GPS_SERIAL //remove?
  //   #define USE_MODULE_SENSORS_GPS_SERIAL
  //   #define ENABLE_GPS_PARSER_NMEA
  //   #define ENABLE_GPS_PARSER_UBX
  //   #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
  //   #define NMEAGPS_DERIVED_TYPES
  //   // #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
  //   #define NMEAGPS_PARSE_SAVE_MILLIS
  //   #define gpsPort Serial2
  //   #define D_GPS_BAUD_RATE_FAST    921600
  //   #define D_GPS_BAUD_RATE_DEFAULT 9600

  //   #define USE_DEVFEATURE_GPS_POLLING_INPUT

  //   // #define USE_MODULE_CORE__SERIAL

  //   // #define ENABLE_DEVFEATURE_GPS_SERIAL__NEW_CODE

  //   #define ENABLE_DEVFEATURE_USE_HARDWARE_SERIAL2_FOR_GPS

  //   #define USE_DEVFEATURE_UBLOX_GLOBAL
    
  //   // #define ENABLE_DEVFEATURE__ENABLE_UBX_PARSER_IN_CLASS

  //   #define USE_DEVFEATURE__UBLOX_TEST_CLASS

    #define ENABLE_DEVFEATURE_NEOGPS__CLASS_AS_INSTANCE


    #define ENABLE_DEBUGFEATURE_TIME__SHOW_UPTIME_EVERY_SECOND

    #define ENABLE_FEATURE_GPSSERIAL__PACKET_METRICS

    
#define GPS_FIX_DATE
#define GPS_FIX_TIME
#define GPS_FIX_LOCATION
#define GPS_FIX_LOCATION_DMS
#define GPS_FIX_ALTITUDE
#define GPS_FIX_SPEED
#define GPS_FIX_VELNED
#define GPS_FIX_HEADING
#define GPS_FIX_SATELLITES
#define GPS_FIX_HDOP
#define GPS_FIX_VDOP
#define GPS_FIX_PDOP
#define GPS_FIX_LAT_ERR
#define GPS_FIX_LON_ERR
#define GPS_FIX_ALT_ERR
#define GPS_FIX_SPD_ERR
#define GPS_FIX_HDG_ERR
#define GPS_FIX_TIME_ERR
#define GPS_FIX_GEOID_HEIGHT

  //   // #define USE_MODULE_DRIVERS_INTERFACE
  //   // #define USE_MODULE_DRIVERS_SERIAL_UART
  //   #define ENABLE_HARDWARE_UART_2
  //   #define HARDWARE_UART_2_BAUD_RATE_SPEED  9600  //D_GPS_BAUD_RATE_FAST
  // #endif // USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS

//  #define USE_MODULE_CORE__SERIAL
      #define ENABLE_HARDWARE_UART_2
      #define HARDWARE_UART_2_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST

      #define ENABLE_DEBUG_FEATURE__MAVLINK_FLYING_LEDS__FORCED_TESTBED_MODE 3

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
      // #ifdef USE_MODULE_CORE__SERIAL
      "\"17\":\"" D_GPIO_HWSERIAL2_TX_CTR   "\","
      "\"16\":\"" D_GPIO_HWSERIAL2_RX_CTR   "\","
      // #endif
      "\"2\":\"" D_GPIO_LED1_CTR  "\""
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
     "\"" D_MODULE__SENSORS__PIR__CTR "\":["
       "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\","
       "\"" D_DEVICE_SENSOR_MOTION1_FRIENDLY_NAME_LONG "\","
       "\"" D_DEVICE_SENSOR_MOTION2_FRIENDLY_NAME_LONG "\""
     "],"
     "\"" D_MODULE__SENSORS__TOF_VL53L0X__CTR "\":["
       "\"" "TOF_VL53L0X" "\""
     "],"
     "\"" D_MODULE__SENSORS__TOF_VL53L1X__CTR "\":["
       "\"" "TOF_VL53L1X" "\""
     "],"
     "\"" D_MODULE__SENSORS__SR04__CTR "\":["
       "\"" "SRO4" "\""
     "],"
     "\"" D_MODULE__SENSORS__SWITCHES__CTR "\":["
       "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
     "],"
     "\"" D_MODULE__SENSORS__BUTTONS__CTR "\":["
       "\"" "WallRed" "\","
       "\"" "WallBlue" "\","
       "\"" "DoorAlert" "\""
     "],"
     "\"" D_MODULE__DRIVERS__LEDS__CTR "\":["
       "\"" "WallRed" "\","
       "\"" "WallBlue" "\","
       "\"" "StatusLED" "\""
     "],"
     "\"" D_MODULE__SENSORS__BME__CTR "\":["
       "\"" D_DEVICE_SENSOR_CLIMATE "\""
     "]"
   "},"    
    "\"" D_MODULE__NETWORK__MQTT__CTR "\":{"
      "\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120,"
      "\"" D_REALTIME_SLOWDOWN "\":0"
    "},"
   "\"MQTT_Interface_Priority\":{\"" D_MODULE__ENERGY__INTERFACE__CTR "\":1}" // Each interface will have ability to reduce its subclass mqtt "ifchanged" rate
 "}";


#endif










/*
 * ** Main board **************************************************************************

This is a new device
To be known as "testbed_presense_sensor". It will be installed in the office, perhaps under the desk

 
TOF VL53L1X
TOF VL53L0X (I got both types)
Ultrasonic sensor (the cheap blue kind)
The new small PIR sensor I am testing
new 26GHz radar sensor





*/
#ifdef DEVICE_DESK_SENSOR   // is this the one moved into the garage??
  #define DEVICENAME_CTR          "desk_sensor"
  #define DEVICENAME_FRIENDLY_CTR "HVAC Desk DevPlatform"
  #define DEVICENAME_ROOMHINT_CTR "Bedroom"
  // #define MQTT_HOST   "192.168.1.70" // primary
  #define MQTT_HOST   "192.168.1.70" // Auto as temporary IP
    #define MQTT_PORT     1883
    
  #define SETTINGS_HOLDER 1241
  #define ENABLE_FEATURE_SETTINGS__ADD_LOCAL_TIME_AS_ASCII_FOR_SAVE_TIME_DEBUGGING
  #define USE_MODULE_CORE_FILESYSTEM
  #define ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__CORE__MQTT

  #define ESP32

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
  // #define ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS 600
  // #define ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
  // #define ENABLE_DEBUG_TRACE__MQTT_TOPIC_AS_TRASNMITTED
  // #define ENABLE_DEBUG_TRACE__MQTT_PAYLOAD_AS_TRANSMITTED
  // #define ENABLE_DEBUGFEATURE__LOGGING_MQTT__CHECK_CONNECTION

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE
  // #define ENABLE_DEBUG_LINE_HERE2
  // #define ENABLE_DEBUG_LINE_HERE_MILLIS
  
  // #define ENABLE_DEBUG_TRACE__MQTT_PAYLOAD_AS_TRANSMITTED
  // #define ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING

  // #define ENABLE_DEBUGFEATURE_LIGHT__PALETTE_RELOAD_LOGGING

  // #define ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS 200

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

  // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

  #define ENABLE_FEATURE_SYSTEM__BOOT_SPLASH__DISPLAY_BLOCK_TO_SHOW_END_OF_INIT

  #define ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE


  // #define ENABLE_DEVFEATURE_PINS__GPIO_VIEWER_LIBRARY
  // #define ENABLE_DEVFEATURE_PINS__GPIO_UI_VIEWER

  /***********************************
   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
  ************************************/  
  
  #define ENABLE_TEMPLATE_SECTION__SENSORS__BME
  #define ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
  #define ENABLE_TEMPLATE_SECTION__SENSORS__SOLAR
  #define ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
  #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION
  #define ENABLE_TEMPLATE_SECTION__LIGHTING
  #define ENABLE_TEMPLATE_SECTION__ENERGY
  #define ENABLE_TEMPLATE_SECTION__ENERGY__PZEM
  #define ENABLE_TEMPLATE_SECTION__ENERGY__INA219
  #define ENABLE_TEMPLATE_SECTION__DISPLAY_NEXTION
  #define ENABLE_TEMPLATE_SECTION__CONTROLLER__HVAC

  /***********************************
   * SECTION: System Configs
  ************************************/     


  /***********************************
   * SECTION: Storage Configs
  ************************************/    

  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_MINUTE
  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_FIVE_SECONDS

  // #define ENABLE_DEVFEATURE_STORAGE__LOAD_TRIGGER_DURING_BOOT

  // 

  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__DRIVERS___RELAYS
  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__CONTROLLERS___HVAC

  // I should add new "purely for debugging" "serialise" data struct. So this will be a new way to take important data from the module data struct that will all be saved in binary, but instead 
  // include functions that "pretty print" them for easier comparing. Will use lots of memory, so debug only.

  #define ENABLE_DEBUGFEATURE__FILESYSTEM__LOAD_HARDCODED_TEMPLATES_INTO_FILESYSTEM

  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_FIVE_SECONDS
  // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_MINUTE

  /**
   * For debugging and short term I may want to store everything as JSON, so I can view the data?
   * Longer term, a mixture of JSON/Binary for space.
   * Options should just be ifdef to switch between methods. 
  */
  //  // this will require methods to serialise and deserialise all data

  
  


  /***********************************
   * SECTION: Network Configs
  ************************************/    

  
  


  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
    #define USE_MODULE_SENSORS_INTERFACE
      #define USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
    #define USE_MODULE_SENSORS_DS18X20
      #define DS18X20_MAX_SENSORS 20
        #define ENABLE_DEBUG_MQTT_CHANNEL_DB18X20    
  #endif 
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
    #define USE_MODULE_SENSORS_BME
      
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__SOLAR
    #define USE_MODULE_SENSORS_SUN_TRACKING     
      #define USE_MODULE_SENSORS_SUN_TRACKING__ANGLES
        #define USE_MODULE_SENSORS_SUN_TRACKING__ANGLES__MANUAL_OVERRIDE_FOR_TESTING
      #define USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_TODAY
      #define USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_FULL
      #define USE_MODULE_SENSORS_SUN_TRACKING__ADVANCED
        // #define ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS

      // #define ENABLE_DEVFEATURE_SUNTRACKING__SUN_TIME_CALCULATE_SUN_PATHS_ACROSS_DAY

      // #define USE_MODULE_SENSORS_SUN_TRACKING__DETAILED_MQTT_INFO_UNIX


  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
    #define USE_MODULE_SENSORS_BH1750
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__MOTION
  #define USE_MODULE_SENSORS_PIR
    #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT
  // #define USE_MODULE_SENSORS_SWITCHES
  #endif

  // #define ENABLE_DEVFEATURE_MQTT__SUPPRESS_SUBMODULE_IFCHANGED_WHEN_UNIFIED_IS_PREFFERRED
    
  /***********************************
   * SECTION: Display Configs
  ************************************/  

  #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_OLED_SH1106
    #define SHOW_SPLASH
  #ifdef ENABLE_TEMPLATE_SECTION__DISPLAY_NEXTION
    #define USE_MODULE_DISPLAYS_NEXTION
    #define NEXTION_DEFAULT_PAGE_NUMBER 6  
      
      //  // change to be code option later
      #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
      #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE
      //                     
      #define ENABLE_DEVFEATURE_NEXTION_DISPLAY        
      #define ENABLE_DEVFEATURE_NEXTION_WEBUI
      #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER 1
      #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
       
      
      #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER_SERIAL1_HVAC_DESK
        #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 115200
        
      DEFINE_PGM_CTR(DISPLAY_TEMPLATE)
      R"=====(
      {
        "PageNames": [
          "boot_flash",
          "boot",
          "message",
          "multiline",
          "logger",
          "heating",
          "hvacdesk",
          "main"
        ],
        "DefaultPageName":"hvacdesk",
        "DefaultBaud":115200,
        "TargetBaud":115200,
        "ObjectNameID": {
          "hdIconHAI": 2,
          "hdTimeHAI": 6,
          "hdBoostHAI": 11,
          "hdAutoHAI": 16,
          "powHAI": 21,
          "hdIconFLR": 3,
          "hdTimeFLR": 7,
          "hdBoostFLR": 12,
          "hdAutoFLR": 17,
          "powFLR": 22,
          "hdIconFAN": 4,
          "hdTimeFAN": 8,
          "hdBoostFAN": 13,
          "hdAutoFAN": 18,
          "powFAN": 23,
          "hdIconOIL": 5,
          "hdTimeOIL": 9,
          "hdBoostOIL": 14,
          "hdAutoOIL": 19,
          "powOIL": 24
        },
        "MQTTSubscribe":[
          "openhab_broadcast/nextion/group/hvac_home",
          "openhab_broadcast/nextion/group/hvac_desk_power"
        ]
      }
      )=====";

  #endif // USE_MODULE_DISPLAYS_NEXTION
  /***********************************
   * SECTION: Driver Configs
  ************************************/  
        
   #define USE_MODULE_DRIVERS_INTERFACE
   #define USE_MODULE_DRIVERS_RELAY

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  // #define ENABLE_DEVFEATURE_LIGHT__HEATMAP_PALETTES

  #ifdef ENABLE_TEMPLATE_SECTION__LIGHTING

    #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options

    #define ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__CONTROLLED_FROM_ANOTHER_MODULE

    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_NO_GRADIENT
    #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_WITH_GRADIENT
    
    
    // 13, 18, 19, 22, 23, 25, 26, 27       USED
    // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
    // 21 = on PCB (4 pixels)
    // 32 = external
    #define USE_LIGHTING_TEMPLATE
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":32,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":0,
          "Length":144
        },
        {
          "Pin":21,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":144,
          "Length":4
        },
        {
          "Pin":4,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":148,
          "Length":44
        }
      ],
      "Segment0": {
        "PixelRange": [
          0,
          144
        ],
        "ColourPalette":"Live Solar Sky",
        "PaletteMappingValues":[10,15,20],
        "SegColour0": {
          "Hue": 0,
          "Sat":100,
          "BrightnessRGB":5
        },
        "Effects": {
          "Function":"Gradient",
          "Speed":127,
          "Intensity":255,
          "Decimate":0,
          "Grouping":1,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "Segment1": {
        "PixelRange": [
          144,
          148
        ],
        "ColourPalette":"Christmas RGPBO",
        "PaletteMappingValues":[10,15,20],
        "SegColour0": {
          "Hue": 0,
          "Sat":100,
          "BrightnessRGB":5
        },
        "Effects": {
          "Function":"Static",
          "Speed":127,
          "Intensity":255,
          "Decimate":0,
          "Grouping":1,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "Segment2": {
        "Name":"Leg",
        "PixelRange": [
          148,
          184
        ],
        "ColourPalette":"Rainbow 16",
        "PaletteMappingValues":[10,15,20],
        "SegColour0": {
          "Hue": 0,
          "Sat":100,
          "BrightnessRGB":5
        },
        "Effects": {
          "Function":"Static",
          "Speed":127,
          "Intensity":255,
          "Decimate":0,
          "Grouping":1,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "Segment3": {
        "Name":"Floor",
        "PixelRange": [
          184,
          192
        ],
        "ColourPalette":"Live SunEL CCT",
        "PaletteMappingValues":[10,15,20],
        "SegColour0": {
          "Hue": 0,
          "Sat":100,
          "BrightnessRGB":5
        },
        "Effects": {
          "Function":"Static",
          "Speed":127,
          "Intensity":255,
          "Decimate":0,
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

    

  #endif // ENABLE_TEMPLATE_SECTION__LIGHTING

  /***********************************
   * SECTION: Energy Configs
  ************************************/  

  #ifdef ENABLE_TEMPLATE_SECTION__ENERGY
    #define USE_MODULE_ENERGY_INTERFACE
  #endif
  
  #ifdef ENABLE_TEMPLATE_SECTION__ENERGY__PZEM
    #define USE_MODULE_ENERGY_PZEM004T_V3
      #define ENABLE_DEVFEATURE_REDUCE_SUBORDINATE_MQTT_REPORTING_ENERGY // If energy_interface is primary reporting, reduce pzem to slower (debug only)
    #define MAX_ENERGY_SENSORS 4
    #define MAX_PZEM004T_DEVICES 4
    #define ENABLE_DEVFEATURE_PZEM004T__AUTOSEARCH
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__ENERGY__INA219
    #define USE_MODULE_ENERGY_INA219
    // #define ENABLE_DEVFEATURE_ENERGY__DISABLE_ENERGY_INTERFACE_FOR_DEBUGGING
  #endif

  /***********************************
   * SECTION: Controller Configs
  ************************************/  

  #ifdef ENABLE_TEMPLATE_SECTION__CONTROLLER__HVAC

  #define USE_MODULE_CONTROLLER_HVAC
     #define HEATING_DEVICE_MAX 4
     
     #define ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES

    #ifdef USE_LIGHTING_TEMPLATE
      #define USE_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR
      #define USE_MODULE_CONTROLLERS__RELAY_STATE_LEDSTRIP
        #define ENABLE_CONTROLLERS__RELAY_STATE_LEDSTRIP__SEGMENT_INDEX   1
    #endif // USE_LIGHTING_TEMPLATE
  //  #define USE_MODULE_CONTROLLER_CUSTOM__ENERGY_OLED
   #define USE_MODULE_CONTROLLER_CUSTOM__DESK_SENSORS_ON_OLED

  #endif // ENABLE_TEMPLATE_SECTION__CONTROLLER__HVAC

   // add new controller custom that instead shows the other sensors as PZEM is on nextion
   /**
    * @brief 
    * BME680 Temp
    * BME280 Temp
    * Lux
    * LED current
    * 
    * Display 2, show all 4 db18s20. So every 5 seconds, flip between 1 and 2
    * 
    */

   //   // Make all the water tank sensors be "remote sensors", then internally they will let me do the colour bar in the same way with IDs

  /***********************************
   * SECTION: MQTT Template Test Loading
  ************************************/  

  #define USE_MQTT_TEMPLATE
  DEFINE_PGM_CTR(MQTT_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  /***********************************
   * SECTION: GPIO Template
  ************************************/  

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"13\":\"" D_GPIO_REL1_INV_CTR  "\","
      "\"27\":\"" D_GPIO_REL2_INV_CTR    "\","
      "\"26\":\"" D_GPIO_REL3_INV_CTR      "\","
      "\"14\":\"" D_GPIO_REL4_INV_CTR      "\"," //pins need sety on L
      #endif
      "\"16\":\""  D_GPIO_PZEM0XX_RX_MODBUS_CTR "\"," 
      "\"17\":\""  D_GPIO_PZEM0XX_TX_CTR "\","
      #ifdef USE_MODULE_SENSORS_DS18X20
      "\"33\":\"" D_GPIO_DS18X20_1_CTR "\"," // DS_DB - 3 pin
      #endif    
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750) || defined(USE_MODULE_ENERGY_INA219)
      "\"23\":\"" D_GPIO_I2C_SCL_CTR   "\","
      "\"22\":\"" D_GPIO_I2C_SDA_CTR   "\","   
      #endif
      #ifdef USE_MODULE_DISPLAYS_NEXTION
      "\"18\":\"" D_GPIO_NEXTION_TX_CTR "\","
      "\"19\":\"" D_GPIO_NEXTION_RX_CTR "\","
      #endif
      #ifdef USE_MODULE_SENSORS_PIR
      "\"15\":\""  D_GPIO_PIR_1_CTR "\","
      #endif
      "\"2\":\""  D_GPIO_LED1_INV_CTR "\""   // builtin led
      // 32 - LED Strip External
      // 21 - LED Strip Onboard
      // 25?
      // 15 - PIR
      // 2
      // 
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  

  /**
   * @brief Drivers and Sensors for HVAC zones
   **/
  #define D_DEVICE_HEATER_0_NAME "Dryer"
  #define D_DEVICE_HEATER_1_NAME "FloorMat"
  #define D_DEVICE_HEATER_2_NAME "FanHeater"
  #define D_DEVICE_HEATER_3_NAME "OilRadiator"

  #define D_DEVICE_SENSOR_DHT_0_NAME "Downstairs_DHT"

// {"NumDevices":4,"DeviceNameIndex":[-1,-1,-1,-1],"AddressList":[[40,140,131,47,0,0,0,230],[40,18,77,49,0,0,0,233],[40,233,112,49,0,0,0,11],[40,165,161,47,0,0,0,189]]}

  /** 
   * MainBoard
   * */
  #define D_DEVICE_SENSOR_DB18S20_01_NAME        "MainBoard-1"
  #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,165,161,47,0,0,0,189]"

  #define D_DEVICE_SENSOR_DB18S20_02_NAME        "MainBoard-2"
  #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,233,112,49,0,0,0,11]"

  #define D_DEVICE_SENSOR_DB18S20_03_NAME        "MainBoard-3"
  #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,140,131,47,0,0,0,230]"

  #define D_DEVICE_SENSOR_DB18S20_04_NAME        "MainBoard-4"
  #define D_DEVICE_SENSOR_DB18S20_04_ADDRESS     "[40,18,77,49,0,0,0,233]" //233 4

  #define D_DEVICE_SENSOR_BME_280_NAME "BME280"
  #define D_DEVICE_SENSOR_BME_680_NAME "BME680"

  #define D_DEVICE_SENSOR_BH1750_NAME "Ambient"

  #define D_DEVICE_SENSOR_CURRENT "LEDStrip"

  
  #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "1"
  #define D_DEVICE_SENSOR_PZEM004T_1_ADDRESS "2"
  #define D_DEVICE_SENSOR_PZEM004T_2_ADDRESS "3"
  #define D_DEVICE_SENSOR_PZEM004T_3_ADDRESS "4"
  
  #define D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR D_DEVICE_HEATER_0_NAME
  #define D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR D_DEVICE_HEATER_1_NAME
  #define D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR D_DEVICE_HEATER_2_NAME
  #define D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR D_DEVICE_HEATER_3_NAME 
  
  #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_0_NAME
  #define D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_1_NAME
  #define D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_2_NAME
  #define D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_3_NAME
  
  #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "Desk"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_ENERGY "\":{"
        "\"DeviceCount\":4"    
    "},"
    "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":{"
        "\"DeviceCount\":4"    
    "},"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE__SENSORS__PIR__CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE__SENSORS__SWITCHES__CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE__DRIVERS__RELAY__CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "],"
      "\"" D_MODULE__SENSORS__SWITCHES__CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "],"
      "\"" D_MODULE__SENSORS__DB18S20__CTR "\":["
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\""
      "],"
      "\"" D_MODULE__SENSORS__DHT__CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
      "],"
      "\"" D_MODULE__SENSORS__SUN_TRACKING__CTR "\":["
        "\"" "Desk" "\""
      "],"  
      "\"" D_MODULE__SENSORS__BME__CTR "\":["
        "\"" D_DEVICE_SENSOR_BME_280_NAME "\","
        "\"" D_DEVICE_SENSOR_BME_680_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_INA219_CTR "\":["
        "\"" D_DEVICE_SENSOR_CURRENT "\""
      "],"
      "\"" D_MODULE__SENSORS__BH1750__CTR "\":["
        "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
      "],"
      "\"" D_MODULE__ENERGY__INTERFACE__CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":["
        "\"" D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE__SENSORS__DB18S20__CTR "\":{" 
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\":" D_DEVICE_SENSOR_DB18S20_03_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\":" D_DEVICE_SENSOR_DB18S20_04_ADDRESS ""
      "},"  
      "\"" D_MODULE__ENERGY__INTERFACE__CTR "\":[" 
        D_DEVICE_SENSOR_PZEM004T_0_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_1_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_2_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_3_ADDRESS
      "]"  
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE__DRIVERS__RELAY__CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_0_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE__DRIVERS__RELAY__CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_1_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE__DRIVERS__RELAY__CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_2_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE__DRIVERS__RELAY__CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_3_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "}"
      "]"
    "},"
    "\"" D_MODULE__NETWORK__MQTT__CTR "\":{"
      "\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120,"
      "\"" D_REALTIME_SLOWDOWN "\":0"
    "},"
    "\"MQTTSubscribe\":["
      "\"openhab_broadcast/nextion/group/hvac_home\","
      "\"openhab_broadcast/nextion/group/hvac_desk_power\""
    "]"
  "}";



  
#endif

#ifdef DEVICE_TESTBED__FILESYSTEM
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "testbed__sensors_nextion"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testbeds"
  
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

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE
  // #define ENABLE_DEBUG_LINE_HERE_MILLIS

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

  // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

  // #define ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS 600
  //   #define ENABLE_DEBUG_FUNCTION_NAMES

  /***********************************
   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
  ************************************/  
  
  #define ENABLE_TEMPLATE_SECTION__SENSORS__BME
  // #define ENABLE_TEMPLATE_SECTION__SENSORS__DHT
  #define ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
  #define ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
  #define ENABLE_TEMPLATE_SECTION__DISPLAYS__NEXTION
  #define ENABLE_TEMPLATE_SECTION__LIGHTS 
  #define ENABLE_TEMPLATE_SECTION__SENSORS__PIR

  /***********************************
   * SECTION: Storage Configs
  ************************************/  
  /**
   * For debugging and short term I may want to store everything as JSON, so I can view the data?
   * Longer term, a mixture of JSON/Binary for space.
   * Options should just be ifdef to switch between methods. 
  */
  //  // this will require methods to serialise and deserialise all data

  /***********************************
   * SECTION: System Configs
  ************************************/     

  #define USE_MODULE_CORE__CRASH_RECORDER

  /***********************************
   * SECTION: Storage Configs
  ************************************/    

  // I should add new "purely for debugging" "serialise" data struct. So this will be a new way to take important data from the module data struct that will all be saved in binary, but instead 
  // include functions that "pretty print" them for easier comparing. Will use lots of memory, so debug only.

  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define FIRMWARE_DEFAULT__INCLUDE_WEBSERVER_FULL
  
  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  
  /***********************************
   * SECTION: Display Configs
  ************************************/  

  /***********************************
   * SECTION: Driver Configs
  ************************************/  

  #define USE_MODULE_DRIVERS_LEDS
        
  //  #define USE_MODULE_DRIVERS_INTERFACE
  //  #define USE_MODULE_DRIVERS_RELAY

  #define USE_FILESYSTEM_SDCARD_BUFFERS
  // #define ENABLE_DEVFEATURE_FILESYSTEM__SDCARD_LOGGING_PERFORMANCE_TEST

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  /***********************************
   * SECTION: Energy Configs
  ************************************/  

  /***********************************
   * SECTION: Controller Configs
  ************************************/  

  /***********************************
   * SECTION: GPIO Template
  ************************************/  
  
  #define ENABLE_DEBUG_POINT_MODULE_TEMPLATE_BOOT_SPLASH
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"   
      /**
       * @brief Right side
       **/
      #ifdef USE_MODULE_SENSORS_DS18X20   
      "\"23\":\"" D_GPIO_DS18X20_1_CTR "\","
      "\"22\":\"" D_GPIO_DS18X20_2_CTR "\","
      #endif // USE_MODULE_SENSORS_DS18X20
      // GPIO1 - TX0 - Debug Serial TX
      // GPIO3 - RX0 - Debug Serial RX
      #ifdef USE_MODULE_SENSORS_LDR_BASIC_DIGITAL
      "\"21\":\"" D_GPIO_LDR_BASIC_DIGITAL1_CTR "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DHT
      "\"19\":\"" D_GPIO_DHT22_1_CTR "\","
      "\"18\":\"" D_GPIO_DHT22_2_CTR "\","      
      #endif
      #ifdef USE_MODULE_SENSORS_PIR
      "\"5\":\""  D_GPIO_SWT1_CTR "\","
      #endif
      #ifdef USE_MODULE_DISPLAYS_NEXTION
      "\"17\":\"" D_GPIO_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_NEXTION_RX_CTR "\","
      #endif
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"4\":\"" D_GPIO_RGB_DATA_CTR  "\","
      #endif 
      #ifdef USE_MODULE_DRIVERS_LEDS
      "\"12\":\"" D_GPIO_LED1_CTR "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_IR_RECEIVER
      "\"15\":\"" D_GPIO_IR_RECV_CTR "\","
      #endif
      // GPIO0 - ADC2 CH1
      
      #ifdef USE_MODULE_DRIVERS_SDCARD
      "\"2\":\"" D_GPIO_SDCARD_HSPI_MISO_CTR   "\","
      "\"15\":\"" D_GPIO_SDCARD_HSPI_MOSI_CTR   "\","   
      "\"14\":\"" D_GPIO_SDCARD_HSPI_CLK_CTR   "\","
      "\"13\":\"" D_GPIO_SDCARD_HSPI_CSO_CTR   "\","  
      #endif



      /**
       * @brief Left side
       **/
      // EN
      // 36 - INPUT ONLY - VP
      // 39 - INPUT ONLY - VN
      #ifdef USE_MODULE_SENSORS_LDR_BASIC_ANALOG
      "\"34\":\"" D_GPIO_LDR_BASIC_ANALOG1_CTR "\"," // adc1_6 // INPUT ONLY
      #endif
      // 35 - INPUT ONLY - adc1_7
      // 32 - Touch9 (Debug Header 1)
      // 33 - Touch8 (Debug Header 2)
      // 25 - DAC1 = LM386 Amplifier Module (Debug Header 3)
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750)
      "\"26\":\"" D_GPIO_I2C_SCL_CTR   "\","
      "\"27\":\"" D_GPIO_I2C_SDA_CTR   "\","  
      #endif    
      // 14 (Debug Header 4)
      // 12 (Debug Header 5)
      // 13 (Debug Header 6) + RX
      //rx receivers?
      // Can I introduce a way that a comma at the end, does not make a broken json?
      "\"0\":\"" D_GPIO_KEY1_INV_CTR   "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";



  /**
   * @brief Drivers and Sensors for HVAC zones
   **/
  #define D_DEVICE_HEATER_0_NAME "Dryer"
  #define D_DEVICE_HEATER_1_NAME "FloorMat"
  #define D_DEVICE_HEATER_2_NAME "FanHeater"
  #define D_DEVICE_HEATER_3_NAME "OilRadiator"

  #define D_DEVICE_SENSOR_DHT_0_NAME "DHT1"
  #define D_DEVICE_SENSOR_DHT_1_NAME "DHT2"

// {"NumDevices":4,"DeviceNameIndex":[-1,-1,-1,-1],"AddressList":[[40,140,131,47,0,0,0,230],[40,18,77,49,0,0,0,233],[40,233,112,49,0,0,0,11],[40,165,161,47,0,0,0,189]]}

  /** 
   * MainBoard
   * 
  [40,255,100,29,194,124,254,111]
  [40,255,100,29,205,206,170,25]
  [40,255,100,29,195,134,175,63]
   * */
  #define D_DEVICE_SENSOR_DB18S20_01_NAME        "MainBoard-1"
  #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,255,100,29,194,124,254,111]"

  #define D_DEVICE_SENSOR_DB18S20_02_NAME        "MainBoard-2"
  #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,255,100,29,205,206,170,25]"

  #define D_DEVICE_SENSOR_DB18S20_03_NAME        "MainBoard-3"
  #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,255,100,29,195,134,175,63]"

  #define D_DEVICE_SENSOR_DB18S20_04_NAME        "MainBoard-4"
  #define D_DEVICE_SENSOR_DB18S20_04_ADDRESS     "[40,18,77,49,0,0,0,233]" //233 4

  #define D_DEVICE_SENSOR_BME_280_NAME "BME280"
  #define D_DEVICE_SENSOR_BME_680_NAME "Office"

  #define D_DEVICE_SENSOR_BH1750_NAME "Ambient"

  #define D_DEVICE_SENSOR_CURRENT "LEDStrip"

  
  #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "1"
  #define D_DEVICE_SENSOR_PZEM004T_1_ADDRESS "2"
  #define D_DEVICE_SENSOR_PZEM004T_2_ADDRESS "3"
  #define D_DEVICE_SENSOR_PZEM004T_3_ADDRESS "4"
  
  #define D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR D_DEVICE_HEATER_0_NAME
  #define D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR D_DEVICE_HEATER_1_NAME
  #define D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR D_DEVICE_HEATER_2_NAME
  #define D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR D_DEVICE_HEATER_3_NAME 
  
  #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_0_NAME
  #define D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_1_NAME
  #define D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_2_NAME
  #define D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_3_NAME

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_ENERGY "\":{"
        "\"DeviceCount\":4"    
    "},"
    "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":{"
        "\"DeviceCount\":4"    
    "},"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE__DRIVERS__RELAY__CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "],"
      "\"" D_MODULE__SENSORS__SWITCHES__CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "],"
      "\"" D_MODULE__SENSORS__DB18S20__CTR "\":["
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\""
      "],"
      "\"" D_MODULE__SENSORS__DHT__CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DHT_1_NAME "\""
      "],"
      "\"" D_MODULE__SENSORS__SUN_TRACKING__CTR "\":["
        "\"" "Desk" "\""
      "],"  
      "\"" D_MODULE__SENSORS__BME__CTR "\":["
        "\"" D_DEVICE_SENSOR_BME_280_NAME "\","
        "\"" D_DEVICE_SENSOR_BME_680_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_INA219_CTR "\":["
        "\"" D_DEVICE_SENSOR_CURRENT "\""
      "],"
      "\"" D_MODULE__SENSORS__BH1750__CTR "\":["
        "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
      "],"
      "\"" D_MODULE__ENERGY__INTERFACE__CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":["
        "\"" D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE__SENSORS__DB18S20__CTR "\":{" 
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\":" D_DEVICE_SENSOR_DB18S20_03_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\":" D_DEVICE_SENSOR_DB18S20_04_ADDRESS ""
      "}"  
    "},"    
    "\"" D_MODULE__NETWORK__MQTT__CTR "\":{"
      "\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120,"
      "\"" D_REALTIME_SLOWDOWN "\":0"
    "},"
    "\"MQTTSubscribe\":["
      "\"openhab_broadcast/nextion/group/hvac_home\","
      "\"openhab_broadcast/nextion/group/hvac_desk_power\""
    "],"
  "}";

  #define D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "motion"

  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)  // needs appending method 
  "{"
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE__SENSORS__SWITCHES__CTR "\","
        "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_TASK_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":0," 
        "\"State\":\"Follow\""
      "}"
    "}"
  "}";

#endif



/****
 * Replicating sonoff basic with a nodemcu, to allow easy serial debugging and testing
 * Due to the pinout on the esp8266 (vs esp8285), a duplicated template is used with nodemcu friendly pins (using both LEDs)
 */
#ifdef DEVICE_TESTBED__SONOFF_BASIC_CLONE_NODEMCU
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


  #define ENABLE_FEATURE_WATCHDOG_TIMER
  
  #define ESP8266
  

  
  #define USE_MODULE_TEMPLATE_SONOFF_BASIC_R2

  #define USE_MODULE_CORE_RULES
  
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BUTTONS
  
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
    #define MAX_RELAYS 1
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_BASE "\":\"" D_MODULE_NAME_SONOFF_BASIC_NODEMCU_CTR  "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Socket"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE__DRIVERS__RELAY__CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE__SENSORS__BUTTONS__CTR "\":["
        "\"Button\""
      "]"
    "}"
  "}";
   
  // Default Rule Defined (DefaultRule_Sonoff_Basic_R2)

#endif







#endif // _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_DEV_TESTBEDS_H


