#ifndef _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_DEV_TESTBEDS_H
#define _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_DEV_TESTBEDS_H

/*********************************************************************************************\
 * Developing/Testbeds: 
 *  ** New code under test, not yet ready to be used as a template (ie may not work yet)
\*********************************************************************************************/

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"

//--------------------------------[Enable Device]-------------------------------------

// #define DEVICE_TESTBED_01__SENSORS_NEXTION


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
// #ifdef DEVICE_DESKSENSOR
#ifdef DEVICE_TESTBED_01__SENSORS_NEXTION
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "testbed_01__sensors_nextion"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testbeds"
  


  // #define DEVICENAME_CTR          "desksensor"
  // #define DEVICENAME_FRIENDLY_CTR "Desk Sensor"
  // #define DEVICENAME_ROOMHINT_CTR "Temporary_Bedroom"
  // #define MQTT_HOST   "192.168.1.70"
  // #define MQTT_PORT     1883

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE 
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE
  // #define ENABLE_DEBUG_LINE_HERE_MILLIS

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

  #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

  #define ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS 600
    #define ENABLE_DEBUG_FUNCTION_NAMES

  /***********************************
   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
  ************************************/  
  
  // #define ENABLE_TEMPLATE_SECTION__SENSORS__BME
  // #define ENABLE_TEMPLATE_SECTION__SENSORS__DHT
  // #define ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
  // // #define ENABLE_TEMPLATE_SECTION__SENSORS__SOLAR
  // #define ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
  // #define ENABLE_TEMPLATE_SECTION__DISPLAYS__NEXTION
  // #define ENABLE_TEMPLATE_SECTION__LIGHTS // maybe dont want them here, too much memory. or, need lightweight version (few effects, no webui? ie basic notification tester!>)
  // #define ENABLE_TEMPLATE_SECTION__SENSORS__PIR

  /***********************************
   * SECTION: Storage Configs
  ************************************/  
  /**
   * For debugging and short term I may want to store everything as JSON, so I can view the data?
   * Longer term, a mixture of JSON/Binary for space.
   * Options should just be ifdef to switch between methods. 
  */
  // #define ENABLE_DEVFEATURE_STORAGE__ALL_DATA_AS_JSON // this will require methods to serialise and deserialise all data

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

  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
    #define USE_MODULE_SENSORS_INTERFACE
    #define USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
    #define USE_MODULE_SENSORS__DS18X20_ESP32_2023
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
  #ifdef USE_MODULE_SENSORS_LDR_BASIC  // leave until analog sensor is redone, then use it. actually, analog although sensor, should become a core part (like i2c etc)
    #define USE_MODULE_SENSORS_LDR_BASIC_DIGITAL
    #define USE_MODULE_SENSORS_LDR_BASIC_ANALOG
  #endif
  
  /***********************************
   * SECTION: Display Configs
  ************************************/  

  #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_OLED_SH1106
    #define ENABLE_DEVFEATURE_DISPLAY__INVERT
    #define SHOW_SPLASH

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
        
  //  #define USE_MODULE_DRIVERS_INTERFACE
  //  #define USE_MODULE_DRIVERS_RELAY

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  #ifdef ENABLE_TEMPLATE_SECTION__LIGHTS

    #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE
    #define ENABLE_FEATURE_LIGHTING__DISABLE_WEBPAGE_TO_REDUCE_MEMORY_USAGE // save memory by removing web page

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
      #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023   
      "\"23\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      "\"22\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\","
      #endif // USE_MODULE_SENSORS__DS18X20_ESP32_2023
      // GPIO1 - TX0 - Debug Serial TX
      // GPIO3 - RX0 - Debug Serial RX
      #ifdef USE_MODULE_SENSORS_LDR_BASIC_DIGITAL
      "\"21\":\"" D_GPIO_FUNCTION_LDR_BASIC_DIGITAL1_CTR "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DHT
      "\"19\":\"" D_GPIO_FUNCTION_DHT22_1_CTR "\","
      "\"18\":\"" D_GPIO_FUNCTION_DHT22_2_CTR "\","      
      #endif
      #ifdef USE_MODULE_SENSORS_PIR
      "\"5\":\""  D_GPIO_FUNCTION_SWT1_CTR "\","
      #endif
      #ifdef USE_MODULE_DISPLAYS_NEXTION
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
      #endif
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      #endif 
      #ifdef USE_MODULE_DRIVERS_LEDS
      "\"2\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_IR_RECEIVER
      "\"15\":\"" D_GPIO_FUNCTION_IR_RECV_CTR "\","
      #endif
      // GPIO0 - ADC2 CH1
      /**
       * @brief Left side
       **/
      // EN
      // 36 - INPUT ONLY - VP
      // 39 - INPUT ONLY - VN
      #ifdef USE_MODULE_SENSORS_LDR_BASIC_ANALOG
      "\"34\":\"" D_GPIO_FUNCTION_LDR_BASIC_ANALOG1_CTR "\"," // adc1_6 // INPUT ONLY
      #endif
      // 35 - INPUT ONLY - adc1_7
      // 32 - Touch9 (Debug Header 1)
      // 33 - Touch8 (Debug Header 2)
      // 25 - DAC1 = LM386 Amplifier Module (Debug Header 3)
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750)
      "\"26\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      #endif
      "\"27\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
      // 14 (Debug Header 4)
      // 12 (Debug Header 5)
      // 13 (Debug Header 6) + RX
      //rx receivers?
      // Can I introduce a way that a comma at the end, does not make a broken json?
      "\"0\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
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
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DHT_CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DHT_1_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SUN_TRACKING_CTR "\":["
        "\"" "Desk" "\""
      "],"  
      "\"" D_MODULE_SENSORS_BME_CTR "\":["
        "\"" D_DEVICE_SENSOR_BME_280_NAME "\","
        "\"" D_DEVICE_SENSOR_BME_680_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_INA219_CTR "\":["
        "\"" D_DEVICE_SENSOR_CURRENT "\""
      "],"
      "\"" D_MODULE_SENSORS_BH1750_CTR "\":["
        "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
      "],"
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":["
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
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":{" 
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\":" D_DEVICE_SENSOR_DB18S20_03_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\":" D_DEVICE_SENSOR_DB18S20_04_ADDRESS ""
      "}"  
    "},"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120},"  
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
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_CTR "\","
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




#endif // _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_DEV_TESTBEDS_H

