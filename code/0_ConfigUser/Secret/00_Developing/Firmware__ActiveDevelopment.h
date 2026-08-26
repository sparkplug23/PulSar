#ifndef _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_ACTIVEDEVELOPMENT_H
#define _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_ACTIVEDEVELOPMENT_H


/*********************************************************************************************\
 * Devices I am currently working on, kept as its own file to make it easier
 * 
 * 
 * Restructing logging levels
 * INF - a module should only give one per minute (ie general health of module)
 * DBG - detailed logging, sensor readings each time they are read
 * 
\*********************************************************************************************/

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Enable Defines*******************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


// #define DEVICE_MEADOWS__TREADMILL_POWER_MONITOR
// #define DEVICE_LIGHTING__LED_MATRIX_BOX_01
// #define DEVICE_TESTBED__NEXTION_DISPLAY__GENERIC_WITH_WEBUI__10INCH
// #define DEVICE_TESTBED__NEXTION_DISPLAY__GENERIC_WITH_WEBUI__7INCH
// #define DEVICE_TESTBED__NEXTION_DISPLAY__TREADMILL_01
// #define DEVICE_TESTBED__NEXTION_DISPLAY__TREADMILL_02
// #define DEVICE_TESTBED__NEXTION_DISPLAY__KITCHEN_7INCH
// #define DEVICE_TESTBED__LED_MATRIX
// #define DEVICE_CAMERA_XIAO_TESTBED
// #define DEVICE_DESKSENSOR // tester with ring led/
// #define DEVICE_TESTBED__FLIGHT__LED_CONTROL_MAVLINK
// #define DEVICE_DOLPHIN__FLIGHT__LED_CONTROL_MAVLINK
// #define DEVICE_TESTBED__ULTRASONIC
// #define DEVICE_LIGHTING__LIGHTING_EFFECTS__MATRIX_SEGMENT_TESTER
// #define DEVICE_TESTBED__GPS_SERIAL_WITH_COMPASS
// #define DEVICE_TESTBED__ROOM_SENSOR_ESP32C3_SUPERMINI
// #define DEVICE_TESTBED__BOARDTEST_ESP32S3__XIAO_ESP32_S3_SEEDSTUDIO
// #define DEVICE_TESTBED__BOARDTEST_ESP32_WROVER_E
// #define DEVICE_DEVBOARD__DESK__ESP32DOIT_GPIO_HARDWARE_PINS_TESTING



/**
 * DEVICE_TYPE CORE: Any testbeds related to core system
 * */
// #define DEVICE_TESTBED__SETTINGS_STORING


/**
 * DEVICE_TYPE NETWORK: Any testbeds related to lighting
 * */
// #define DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_01
// #define DEVICE_MAVLINK_DECODER_LOCATOR_LTE_DOWNLINK_01
// #define DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH_01
// #define DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH_02
// #define DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH_03
// #define DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH_04_MQTT_ON_WIFI

// #define DEVICE_CELLULAR_LTE__GPS_POSITION_LOCATOR_01__ON_SIM9000G_2024
// #define DEVICE_CELLULAR_LTE__GPS_POSITION_LOCATOR_02__ON_SIM9000G_2024
// #define DEVICE_CELLULAR_LTE__GPS_POSITION_LOCATOR_03__ON_SIM800L_2024
// #define DEVICE_HARDWARE_TESTBED__TINY_GPS_SERIAL

/**
 * DEVICE_TYPE LIGHTING: Any testbeds related to lighting
 * */
// #define DEVICE_TESTBED_ESP32_LILYGO_SIM7000G
// #define DEVICE_TESTBED_ESP32_LILYGO_SIM7000G_V2
// #define DEVICE_TESTBED_ESP32_LILYGO_SIM7000G_CELLULAR_LOCATOR_02
// #define DEVICE_TESTBED_ESP32_LILYGO_SIM800L
// #define DEVICE_TESTBED_ESP32_BUZZER_TONES
// #define DEVICE_TESTBED_MAVLINK_DECODER
// #define DEVICE_TESTBED_MAVLINK_TELEMETRY_WIFI_01
// #define DEVICE_TESTBED_ESP32_LILYGO_SIM7000G_CELLULAR_LOCATOR_01

/**
 * DEVICE_TYPE DRIVERS: Any testbeds related to lighting
 * */


/**
 * DEVICE_TYPE CONTROLLERS: Any testbeds related to lighting
 * */


/**
 * DEVICE_TYPE DISPLAYS: Any testbeds related to lighting
 * */

/**
 *  SHORT TERM DEVICES IN BEDROOM -- SHORT TERM DEVICES -- SHORT TERM DEVICES -- SHORT TERM DEVICES -- SHORT TERM DEVICES -- SHORT TERM DEVICES -- SHORT TERM DEVICES -- SHORT TERM DEVICES -- SHORT TERM DEVICES -- 
**/
// #define DEVICE_TESTBED_CAMERA_SENSOR_MODULE

// #define DEVICE_TESTBED_RTC_FASTBOOT
// #define DEVICE_TESTBED_BUTTON_MULTIPRESS
// #define DEVICE_TESTBED_FONA_800L_BASIC
// #define DEVICE_TESTBED_RCSWITCH_NORMAL
// #define DEVICE_testbed_rcs_ext
// #define DEVICE_TESTBED_FASTBOOT_ESP8266
// #define DEVICE_TESTBED_CRASHREPORT

// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__ESP32C3_TESTER

// #define DEVICE_DEVELOPMENT__MOISTURE_SENSOR

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** LOCATION: Livingroom ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**
 * 
 * Change to ESP32
 * Add ultrasonic sensor to the cold water tank (glue, cut in 2 inches on lid so wire can be brought over and ziptie so it cant fall into the water ever)
 *  DONT FLASH UNTIL ITS ESP32
 * 
 * */
#ifdef DEVICE_TESTBED__ULTRASONIC
  #define DEVICENAME_CTR          "testbed_ultrasonic"
  #define DEVICENAME_FRIENDLY_CTR "Attic Sensor"
  #define DEVICENAME_ROOMHINT_CTR "Attic"
  #define MQTT_HOST   "192.168.1.70"

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  
  
  
  //   #define USE_MODULE_SENSORS_INTERFACE
  //   #
  // #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_DS18X20
  #define USE_MODULE_SENSORS_SR04
    #define ENABLE_DEVFEATURE_SR04_FILTERING_EMA
    #define ENABLE_DEVFEATURE_SR04_FILTERING_DEMA
    // #define SONAR_SERIAL_BAUD 115200
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750)
      "\"21\":\"" D_GPIO_I2C_SDA_CTR   "\","
      "\"22\":\"" D_GPIO_I2C_SCL_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_SR04
      "\"19\":\"" D_GPIO_SR04_ECHO_CTR   "\","
      "\"18\":\"" D_GPIO_SR04_TRIG_CTR  "\","  
      #endif 
      #ifdef USE_MODULE_SENSORS_DS18X20
      "\"5\":\"" D_GPIO_DS18X20_1_CTR  "\"," 
      #endif
      "\"2\":\""  D_GPIO_LED1_INV_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Attic"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Attic"  
  #define D_DEVICE_SENSOR_SR04_FRIENDLY_NAME_LONG "ColdWaterHeaderTank"  

  #define D_DEVICE_SENSOR_DB18S20_01_NAME        "Cold Water Tank"
  #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,50,199,3,0,0,0,102]"
  #define D_DEVICE_SENSOR_DB18S20_02_NAME        "Attic Top"
  #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,255,100,30,7,142,92,154]"
  #define D_DEVICE_SENSOR_DB18S20_03_NAME        "Attic Bottom"
  #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,255,100,30,7,143,73,252]"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
      "],"  
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SR04_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_SR04_FRIENDLY_NAME_LONG "\""
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":{" 
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\":" D_DEVICE_SENSOR_DB18S20_03_ADDRESS ""
      "}"   
    "}"
  "}";

#endif





/**
 * @brief 
 * Testbed for LED control via Mavlink
 * 
 * Test 1:
 *  - Control LED strip and set to simply be colours, use my segments to make different patterns!
 * 
 * Test 2:
 *  - Decode mavlink, change wing colours based an altitude. Or at least, when below 30, then 20m, switch to "landing" colours
 * 
 * Test 3:
 *  - Based on TX16S left slider, have maybe 4 modes of lighting and switch between them. Have "altitude" mode be a mode
 *    -- Mode 1: 
 *        -- Altitude changes speed of flashing, and or colour?
 * 
 * 
 */
#ifdef DEVICE_TESTBED__FLIGHT__LED_CONTROL_MAVLINK
  #define DEVICENAME_CTR          "testbed_mavlink_led"
  #define DEVICENAME_FRIENDLY_CTR "HVAC Desk DevPlatform"
  #define DEVICENAME_ROOMHINT_CTR "Bedroom"
  // #define MQTT_HOST   "192.168.1.70" // primary
  #define MQTT_HOST   "192.168.1.70" // Auto as temporary IP
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
  // #define ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE
  // #define ENABLE_DEBUG_LINE_HERE_MILLIS

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

  #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

  #define ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS 600
    #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_DEVFEATURE_PINS__GPIO_VIEWER_LIBRARY
  // #define ENABLE_DEVFEATURE_PINS__GPIO_UI_VIEWER

  /***********************************
   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
  ************************************/  

  #define ENABLE_TEMPLATE_SECTION__LIGHTING__TESTRING  
  #define ENABLE_TEMPLATE_SECTION__DRIVERS_MAVLINK
  #define ENABLE_TEMPLATE_SECTION__CUSTOM__MAVLINK_FLYING_LEDS

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

  

  #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging

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


  /***********************************
   * SECTION: Network Configs
  ************************************/    

 #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 20

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  // #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
  //   #define USE_MODULE_SENSORS_INTERFACE
  //     #define USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
  // #endif
  // #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
  //   #define USE_MODULE_SENSORS_DS18X20
  //     #define DS18X20_MAX_SENSORS 20
  //       #define ENABLE_DEBUG_MQTT_CHANNEL_DB18X20    
  // #endif 
  // #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
  //   #define USE_MODULE_SENSORS_BME
  //     
  // #endif
  // #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__SOLAR
  //   #define USE_MODULE_SENSORS_SUN_TRACKING     
  // #endif
  // #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
  //   #define USE_MODULE_SENSORS_BH1750
  // #endif
    
  /***********************************
   * SECTION: Display Configs
  ************************************/  

  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SH1106
  //   #define SHOW_SPLASH
  // #define USE_MODULE_DISPLAYS_NEXTION
  //   #ifdef USE_MODULE_DISPLAYS_NEXTION
  //     #define NEXTION_DEFAULT_PAGE_NUMBER 6  
  //       
  //       //  // change to be code option later
  //       #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
  //       #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE
  //       //                     
  //       #define ENABLE_DEVFEATURE_NEXTION_DISPLAY        
  //       #define ENABLE_DEVFEATURE_NEXTION_WEBUI
  //       #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER 1
  //       #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  //        
  //       
  //       #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER_SERIAL1_HVAC_DESK
  //         #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 115200
      
  //       DEFINE_PGM_CTR(DISPLAY_TEMPLATE)
  //       R"=====(
  //       {
  //         "PageNames": [
  //           "boot_flash",
  //           "boot",
  //           "message",
  //           "multiline",
  //           "logger",
  //           "heating",
  //           "hvacdesk",
  //           "main"
  //         ],
  //         "DefaultPageName":"hvacdesk",
  //         "DefaultBaud":115200,
  //         "TargetBaud":115200,
  //         "ObjectNameID": {
  //           "hdIconHAI": 2,
  //           "hdTimeHAI": 6,
  //           "hdBoostHAI": 11,
  //           "hdAutoHAI": 16,
  //           "powHAI": 21,
  //           "hdIconFLR": 3,
  //           "hdTimeFLR": 7,
  //           "hdBoostFLR": 12,
  //           "hdAutoFLR": 17,
  //           "powFLR": 22,
  //           "hdIconFAN": 4,
  //           "hdTimeFAN": 8,
  //           "hdBoostFAN": 13,
  //           "hdAutoFAN": 18,
  //           "powFAN": 23,
  //           "hdIconOIL": 5,
  //           "hdTimeOIL": 9,
  //           "hdBoostOIL": 14,
  //           "hdAutoOIL": 19,
  //           "powOIL": 24
  //         }
  //       }
  //       )=====";

  //   #endif // USE_MODULE_DISPLAYS_NEXTION


  /***********************************
   * SECTION: Driver Configs
  ************************************/  
        
  //  #define USE_MODULE_DRIVERS_INTERFACE
  //  #define USE_MODULE_DRIVERS_RELAY

  #ifdef ENABLE_TEMPLATE_SECTION__DRIVERS_MAVLINK
    #define USE_MODULE__DRIVERS_MAVLINK_DECODER
      #define USE_FEATURE_SEARCH_FOR_UNHANDLED_MAVLINK_MESSAGES_ON_ALLOWEDLIST
      #define ENABLE_FEATURE_MAVLINK_CONVERT_MQTT_DATA_VALUES
      #define ENABLE_FEATURE_MAVLINK_MQTT_SEND_ALL_PACKETS_AS_TELEMETRY_TOPICS
    #define USE_MODULE_CORE__SERIAL
      #define ENABLE_HARDWARE_UART_2
      #define HARDWARE_UART_2_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST

      #define ENABLE_DEBUG_FEATURE__MAVLINK_FLYING_LEDS__FORCED_TESTBED_MODE 3

      // //#define DATA_BUFFER_PAYLOAD_MAX_LENGTH 3500


  #endif

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  #ifdef ENABLE_TEMPLATE_SECTION__LIGHTING__TESTRING

    // #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options

    // #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_FEBRUARY_2023
    // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
    // 
    //   
    

    #define ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__CONTROLLED_FROM_ANOTHER_MODULE

    // 

    
    


    #define FIRMWARE_DEFAULT__LIGHTING__ESP32_OPTIONS_MINIMAL__MAY24

    
    
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_NO_GRADIENT
    #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_WITH_GRADIENT
    




  #define FIRMWARE_DEFAULT__LIGHTING__ESP32_OPTIONS_MINIMAL__MAY24

  
  #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_WITH_GRADIENT
  

  #define USE_LIGHTING_TEMPLATE

  
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
          "Pin":13,
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
        "ColourPalette":"IceCream Floats",
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
          "Grouping":1
        },
        "Transition": {
          "TimeMs": 200,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "BrightnessRGB": 5,
      "BrightnessCCT": 0
    }
    )=====";

  #endif // ENABLE_TEMPLATE_SECTION__LIGHTING

  /***********************************
   * SECTION: Energy Configs
  ************************************/  

  // #ifdef ENABLE_TEMPLATE_SECTION__ENERGY
  //   #define USE_MODULE_ENERGY_INTERFACE
  // #endif
  
  // #ifdef ENABLE_TEMPLATE_SECTION__ENERGY__PZEM
  //   #define USE_MODULE_ENERGY_PZEM004T_V3
  //     #define ENABLE_DEVFEATURE_REDUCE_SUBORDINATE_MQTT_REPORTING_ENERGY // If energy_interface is primary reporting, reduce pzem to slower (debug only)
  //   #define MAX_ENERGY_SENSORS 4
  //   #define MAX_PZEM004T_DEVICES 4
  //   #define ENABLE_DEVFEATURE_PZEM004T__AUTOSEARCH
  // #endif
  // #ifdef ENABLE_TEMPLATE_SECTION__ENERGY__INA219
  //   #define USE_MODULE_ENERGY_INA219
  //   // #define ENABLE_DEVFEATURE_ENERGY__DISABLE_ENERGY_INTERFACE_FOR_DEBUGGING
  // #endif

  /***********************************
   * SECTION: Controller Configs
  ************************************/  

  // #define USE_MODULE_CONTROLLER_HVAC
  //    #define HEATING_DEVICE_MAX 4
  //    
  //    #define ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES

  //   #ifdef USE_LIGHTING_TEMPLATE
  //     #define USE_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR
  //     #define USE_MODULE_CONTROLLERS__RELAY_STATE_LEDSTRIP
  //       #define ENABLE_CONTROLLERS__RELAY_STATE_LEDSTRIP__SEGMENT_INDEX   1
  //   #endif // USE_LIGHTING_TEMPLATE
  //  #define USE_MODULE_CONTROLLER_CUSTOM__ENERGY_OLED

  
  #ifdef ENABLE_TEMPLATE_SECTION__CUSTOM__MAVLINK_FLYING_LEDS

      #define USE_MODULE_CONTROLLER_CUSTOM__MAVLINK_FLYING_LEDS


  #endif
   //   // Make all the water tank sensors be "remote sensors", then internally they will let me do the colour bar in the same way with IDs

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
      // "\"16\":\""  D_GPIO_PZEM0XX_RX_MODBUS_CTR "\"," 
      // "\"17\":\""  D_GPIO_PZEM0XX_TX_CTR "\","
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
      #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
      "\"17\":\"" D_GPIO_HWSERIAL2_TX_CTR   "\","
      "\"16\":\"" D_GPIO_HWSERIAL2_RX_CTR   "\","   
      #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER   
      "\"2\":\""  D_GPIO_LED1_INV_CTR "\""   // builtin led
      // 32 - LED Strip External
      // 21 - LED Strip Onboard
      // 25?
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
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
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
      "},"  
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":[" 
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
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_0_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_1_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_2_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_3_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "}"
      "]"
    "},"    
    "\"" D_MODULE_NETWORK_MQTT_CTR "\":{"
      "\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120,"
      "\"" D_REALTIME_SLOWDOWN "\":0"
    "},"
    "\"MQTTSubscribe\":["
      "\"openhab_broadcast/nextion/group/hvac_home\","
      "\"openhab_broadcast/nextion/group/hvac_desk_power\""
    "],"
  "}";



  
#endif


/**
 * @brief 
 * Testbed for LED control via Mavlink
 * 
 * Test 1:
 *  - Control LED strip and set to simply be colours, use my segments to make different patterns!
 * 
 * Test 2:
 *  - Decode mavlink, change wing colours based an altitude. Or at least, when below 30, then 20m, switch to "landing" colours
 * 
 * Test 3:
 *  - Based on TX16S left slider, have maybe 4 modes of lighting and switch between them. Have "altitude" mode be a mode
 *    -- Mode 1: 
 *        -- Altitude changes speed of flashing, and or colour?
 * 
 * 
 */
#ifdef DEVICE_DOLPHIN__FLIGHT__LED_CONTROL_MAVLINK
  #define DEVICENAME_CTR          "flight_mavlink_led"
  #define DEVICENAME_FRIENDLY_CTR "MAVLink Dolphin Lighting"
  #define DEVICENAME_ROOMHINT_CTR "Dolphin"
  // #define MQTT_HOST   "192.168.1.70" // primary
  #define MQTT_HOST   "192.168.1.70" // Auto as temporary IP
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
  // #define ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE
  // #define ENABLE_DEBUG_LINE_HERE_MILLIS

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

  #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

  #define ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS 600
    #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_DEVFEATURE_PINS__GPIO_VIEWER_LIBRARY
  // #define ENABLE_DEVFEATURE_PINS__GPIO_UI_VIEWER

  /***********************************
   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
  ************************************/  

  #define ENABLE_TEMPLATE_SECTION__LIGHTING__TESTRING  
  #define ENABLE_TEMPLATE_SECTION__DRIVERS_MAVLINK
  #define ENABLE_TEMPLATE_SECTION__CUSTOM__MAVLINK_FLYING_LEDS

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

  #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging


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


  /***********************************
   * SECTION: Network Configs
  ************************************/    

 #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 20
// #define ENABLE_FEATURE_BUILD__RELEASE_TO_OTHERS_WITHOUT_NETWORKING 
// #define DISABLE_NETWORK

#define ENABLE_DEVFEATURE_NETWORK__BLOCK_CONNECT_PUSH_BACKOFF_LONG_AS_TEMP_SOLUTION_TO_NO_WIFI

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  // #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
  //   #define USE_MODULE_SENSORS_INTERFACE
  //     #define USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
  // #endif
  // #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
  //   #define USE_MODULE_SENSORS_DS18X20
  //     #define DS18X20_MAX_SENSORS 20
  //       #define ENABLE_DEBUG_MQTT_CHANNEL_DB18X20    
  // #endif 
  // #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
  //   #define USE_MODULE_SENSORS_BME
  //     
  // #endif
  // #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__SOLAR
  //   #define USE_MODULE_SENSORS_SUN_TRACKING     
  // #endif
  // #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
  //   #define USE_MODULE_SENSORS_BH1750
  // #endif
    
  /***********************************
   * SECTION: Display Configs
  ************************************/  

  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SH1106
  //   #define SHOW_SPLASH
  // #define USE_MODULE_DISPLAYS_NEXTION
  //   #ifdef USE_MODULE_DISPLAYS_NEXTION
  //     #define NEXTION_DEFAULT_PAGE_NUMBER 6  
  //       
  //       //  // change to be code option later
  //       #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
  //       #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE
  //       //                     
  //       #define ENABLE_DEVFEATURE_NEXTION_DISPLAY        
  //       #define ENABLE_DEVFEATURE_NEXTION_WEBUI
  //       #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER 1
  //       #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  //        
  //       
  //       #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER_SERIAL1_HVAC_DESK
  //         #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 115200
  //       DEFINE_PGM_CTR(DISPLAY_TEMPLATE)
  //       R"=====(
  //       {
  //         "PageNames": [
  //           "boot_flash",
  //           "boot",
  //           "message",
  //           "multiline",
  //           "logger",
  //           "heating",
  //           "hvacdesk",
  //           "main"
  //         ],
  //         "DefaultPageName":"hvacdesk",
  //         "DefaultBaud":115200,
  //         "TargetBaud":115200,
  //         "ObjectNameID": {
  //           "hdIconHAI": 2,
  //           "hdTimeHAI": 6,
  //           "hdBoostHAI": 11,
  //           "hdAutoHAI": 16,
  //           "powHAI": 21,
  //           "hdIconFLR": 3,
  //           "hdTimeFLR": 7,
  //           "hdBoostFLR": 12,
  //           "hdAutoFLR": 17,
  //           "powFLR": 22,
  //           "hdIconFAN": 4,
  //           "hdTimeFAN": 8,
  //           "hdBoostFAN": 13,
  //           "hdAutoFAN": 18,
  //           "powFAN": 23,
  //           "hdIconOIL": 5,
  //           "hdTimeOIL": 9,
  //           "hdBoostOIL": 14,
  //           "hdAutoOIL": 19,
  //           "powOIL": 24
  //         }
  //       }
  //       )=====";

  //   #endif // USE_MODULE_DISPLAYS_NEXTION


  /***********************************
   * SECTION: Driver Configs
  ************************************/  
        
  //  #define USE_MODULE_DRIVERS_INTERFACE
  //  #define USE_MODULE_DRIVERS_RELAY

  #ifdef ENABLE_TEMPLATE_SECTION__DRIVERS_MAVLINK
    #define USE_MODULE__DRIVERS_MAVLINK_DECODER
      #define USE_FEATURE_SEARCH_FOR_UNHANDLED_MAVLINK_MESSAGES_ON_ALLOWEDLIST
      #define ENABLE_FEATURE_MAVLINK_CONVERT_MQTT_DATA_VALUES
      #define ENABLE_FEATURE_MAVLINK_MQTT_SEND_ALL_PACKETS_AS_TELEMETRY_TOPICS
    #define USE_MODULE_CORE__SERIAL
      #define ENABLE_HARDWARE_UART_2
      #define HARDWARE_UART_2_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
      // #define HARDWARE_UART_2_BAUD_RATE_SPEED  115200  //D_GPS_BAUD_RATE_FAST


      // #define ENABLE_DEBUG_FEATURE__MAVLINK_FLYING_LEDS__FORCED_TESTBED_MODE 2

      // //#define DATA_BUFFER_PAYLOAD_MAX_LENGTH 3500


  #endif

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  #ifdef ENABLE_TEMPLATE_SECTION__LIGHTING__TESTRING

    #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options

    // #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_FEBRUARY_2023
    // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
    // 
    //   

    #define ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__CONTROLLED_FROM_ANOTHER_MODULE

    // 


    
    


    // #define FIRMWARE_DEFAULT__LIGHTING__ESP32_OPTIONS_MINIMAL__MAY24

    
    
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_NO_GRADIENT
    #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_WITH_GRADIENT
    


  /***********************************
   * SECTION: System Configs
  ************************************/       

  #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging


  /***********************************
   * SECTION: Network Configs
  ************************************/    
 #ifndef ESP8266
  
  
#endif
  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  /***********************************
   * SECTION: Display Configs
  ************************************/  

  /***********************************
   * SECTION: Driver Configs
  ************************************/  

  //  #define ENABLE_DEVFEATURE_MAVLINK__USE_TIME_FOR_RANGE_TESTING
  // #define ENABLE_DEVFEATURE_MAVLINK__USE_MANUAL_FOR_RANGE_TESTING
  // #define ENABLE_DEBUG_FEATURE__MAVLINK_FLYING_LEDS__FORCED_TESTBED_MODE

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  // #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
  // #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
  // #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
  
  

  

  

  // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
  
    
  
  #define USE_MODULE_LIGHTS_ANIMATOR
    /********* Group: Testing ************************/
      // Phase out
    #define ENABLE_DEVFEATURE_LIGHT__HYPERION
    // #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    // #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    /********* Group: Debug options only ************************/
    // #define ENABLE_DEBUG_LINE_HERE
    #define ENABLE_DEBUG_SERIAL    

    
  // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S_SINGLE_CHANNELS_THEN_8_RMT_CHANNELS
  // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
  // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE
  // #define ENABLE_NEOPIXELBUS_BUSMETHODS__RMT_8_CHANNELS_THEN_I2S_DUAL_CHANNELS

    
  

  #define ENABLE_DEVFEATURE_LIGHTING__PRESET_LOAD_FROM_FILE
  #define ENABLE_FEATURE_LIGHTS__PRESETS
  
  // _LINES
  
  #define ENABLE_FEATURE_LIGHTS__PLAYLISTS
  
  // #define ENABLE_DEVFEATURE_LIGHTING__SETTINGS



  // #define FIRMWARE_DEFAULT__LIGHTING__ESP32_OPTIONS_MINIMAL__MAY24

  
  #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_WITH_GRADIENT
  

  #define USE_LIGHTING_TEMPLATE

  

  // #undef  ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
  #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE

  // #define ENABLE_TEMPLATE_LIGHTING__SHOW_BUS_CONNECTED_RGB
  // #define ENABLE_TEMPLATE_LIGHTING__SHOW_BUS_CONNECTED_RGBW
  #define ENABLE_TEMPLATE_LIGHTING__DESIGN_1
  // #define ENABLE_TEMPLATE_LIGHTING__TEST_WING_ONLY

  #ifdef ENABLE_TEMPLATE_LIGHTING__DESIGN_1
    #define USE_LIGHTING_TEMPLATE
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":18,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":0,
          "Length":41,
          "Reversed":1
        },
        {
          "Pin":13,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":41,
          "Length":41
        },
        {
          "Pin":14,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":82,
          "Length":18,
          "Reversed":1
        },
        {
          "Pin":4,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":100,
          "Length":18
        },
        {
          "Pin":12,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":118,
          "Length":21
        },
        {
          "Pin":2,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":139,
          "Length":25
        },
        {
          "Pin":21,
          "ColourOrder":"GRB",
          "BusType":"WS2812_RGB",
          "Start":164,
          "Length":16,
          "Reversed":1
        },
        {
          "Pin":19,
          "ColourOrder":"GRB",
          "BusType":"WS2812_RGB",
          "Start":180,
          "Length":16
        },
        {
          "Pin":5,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":196,
          "Length":16
        }
      ]
    }
    )=====";
    // ,
    //     {
    //       "Pin":15,
    //       "ColourOrder":"GRBC",
    //       "BusType":"SK6812_RGBW",
    //       "Start":190,
    //       "Length":10
    //     }

    #define USE_LIGHTING_TEMPLATE_ANOTHER
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE_ANOTHER)
    R"=====(
    {
      "Segment0": {
        "Name":"Wing",
        "PixelRange": [
          0,
          82
        ],
        "ColourPalette":0,
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
          "Grouping":1
        },
        "Transition": {
          "TimeMs": 200,
          "RateMs": 1000
        },
        "ColourType":3,
        "BrightnessRGB": 2,
        "BrightnessCCT": 2
      },
      "Segment1": {
        "Name":"Fuselage",
        "ColourPalette":1,
        "PixelRange": [
          82,
          118
        ],
        "Effects": {
          "Function":"Gradient"
        },
        "ColourType":3,
        "BrightnessRGB": 2,
        "BrightnessCCT": 2
      },
      "Segment2": {
        "Name":"Hatches",
        "PixelRange": [
          118,
          164
        ],
        "ColourPalette":2,
        "Effects": {
          "Function":"Gradient"
        },
        "ColourType":3,
        "Transition": {
          "TimeMs": 200,
          "RateMs": 1000
        },
        "BrightnessRGB": 2,
        "BrightnessCCT": 2
      },
      "Segment3": {
        "Name":"Rudder",
        "PixelRange": [
          164,
          196
        ],
        "ColourPalette":3,
        "Effects": {
          "Function":"Gradient"
        },
        "ColourType":3,
        "Transition": {
          "TimeMs": 200,
          "RateMs": 1000
        },
        "BrightnessRGB": 2,
        "BrightnessCCT": 2
      },
      "Segment4": {
        "Name":"Nose",
        "PixelRange": [
          196,
          212
        ],
        "ColourPalette":4,
        "SegColour0": {
          "Hue": 0,
          "Sat":100,
          "CCT_TempPercentage":90,
        },
        "Effects": {
          "Function":"Solid Colour"
        },
        "ColourType":3,
        "BrightnessRGB": 2,
        "BrightnessCCT": 2
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 100
    }
    )=====";

    // #define USE_LIGHTING_TEMPLATE_ANOTHER
    // DEFINE_PGM_CTR(LIGHTING_TEMPLATE_ANOTHER)
    // R"=====(
    // {
    //   "Segment0": {
    //     "Name":"View Bus",
    //     "PixelRange": [
    //       0,
    //       214
    //     ],
    //     "ColourPalette":"Rainbow 16",
    //     "Effects": {
    //       "Function":"Gradient"
    //     },
    //     "ColourType":3,
    //     "Transition": {
    //       "TimeMs": 200,
    //       "RateMs": 1000
    //     },
    //     "BrightnessRGB": 100,
    //     "BrightnessCCT": 0
    //   },
    //   "BrightnessRGB": 5,
    //   "BrightnessCCT": 0
    // }
    // )=====";


  #endif // ENABLE_TEMPLATE_LIGHTING__DESIGN_1
  
  #ifdef ENABLE_TEMPLATE_LIGHTING__TEST_WING_ONLY
    #define USE_LIGHTING_TEMPLATE
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":18,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":0,
          "Length":41
        }
      ]
    }
    )=====";
    
    #define USE_LIGHTING_TEMPLATE_ANOTHER
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE_ANOTHER)
    R"=====(
    {
      "Segment0": {
        "Name":"Wing",
        "PixelRange": [
          0,
          41
        ],
        "ColourPalette":0,
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
          "Grouping":1
        },
        "Transition": {
          "TimeMs": 200,
          "RateMs": 1000
        },
        "ColourType":3,
        "BrightnessRGB": 2,
        "BrightnessCCT": 2
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 100
    }
    )=====";

  #endif // ENABLE_TEMPLATE_LIGHTING__TEST_WING_ONLY


  #ifdef ENABLE_TEMPLATE_LIGHTING__SHOW_BUS_CONNECTED_RGB

    #define USE_LIGHTING_TEMPLATE
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
          "Start":0,
          "Length":41,
          "Reversed":1
        },
        {
          "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
          "Start":41,
          "Length":41
        },
        {
          "Pin":18,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
          "Start":82,
          "Length":18
        },
        {
          "Pin":5,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
          "Start":100,
          "Length":18
        },
        {
          "Pin":15,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
          "Start":118,
          "Length":21
        },
        {
          "Pin":12,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
          "Start":139,
          "Length":21
        },
        {
          "Pin":2,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
          "Start":160,
          "Length":10
        },
        {
          "Pin":14,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
          "Start":170,
          "Length":10
        },
        {
          "Pin":19,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
          "Start":180,
          "Length":10
        },
        {
          "Pin":21,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
          "Start":190,
          "Length":10
        }
      ],
      "Segment0": {
        "PixelRange": [
          0,
          200
        ],
        "ColourPalette":"IceCream Floats",
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
          "Grouping":1
        },
        "Transition": {
          "TimeMs": 200,
          "RateMs": 1000
        },
        "ColourType":3,
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "BrightnessRGB": 5,
      "BrightnessCCT": 0
    }
    )=====";

  #endif
  #ifdef ENABLE_TEMPLATE_LIGHTING__FLIGHT_TYPE_1

  
    // 13, 18, 19, 22, 23, 25, 26, 27       USED
    // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
    // 21 = on PCB (4 pixels)
    // 32 = external
    /**
     * @brief 
     * 
     * Wing Length: 41
     * Fuselage Length: 18
     * Front Cover: 21
     * Rear Cover: 25
     * Rudder: --
     * 
     * Bus Order
     * ** Wing Right
     * ** Wing Left
     * ** Fuselage Right
     * ** Fuselage Left
     * ** Front Cover
     * ** Rear Cover
     * ** Rudder Right
     * ** Rudder Left
     * 
     * 
     */
    #define USE_LIGHTING_TEMPLATE
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":4,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":0,
          "Length":41,
          "Reversed":1
        },
        {
          "Pin":13,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":41,
          "Length":41
        },
        {
          "Pin":18,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":82,
          "Length":18
        },
        {
          "Pin":5,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":100,
          "Length":18
        },
        {
          "Pin":15,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":118,
          "Length":21
        },
        {
          "Pin":12,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":139,
          "Length":25
        },
        {
          "Pin":2,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":160,
          "Length":10
        },
        {
          "Pin":14,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":170,
          "Length":10
        },
        {
          "Pin":19,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":180,
          "Length":10
        },
        {
          "Pin":21,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":190,
          "Length":10
        }
      ],
      "Segment0": {
        "PixelRange": [
          0,
          82
        ],
        "ColourPalette":"IceCream Floats",
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
          "Grouping":1
        },
        "Transition": {
          "TimeMs": 200,
          "RateMs": 1000
        },
        "ColourType":3,
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "Segment1": {
        "PixelRange": [
          82,
          118
        ],
        "Effects": {
          "Function":"Gradient"
        },
        "ColourType":3
      },
      "Segment2": {
        "PixelRange": [
          118,
          200
        ],
        "ColourPalette":"IceCream Floats",
        "Effects": {
          "Function":"Gradient"
        },
        "ColourType":3,
        "Transition": {
          "TimeMs": 200,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "BrightnessRGB": 5,
      "BrightnessCCT": 0
    }
    )=====";

  #endif // ENABLE_TEMPLATE_LIGHTING__FLIGHT_TYPE_1

  #endif // ENABLE_TEMPLATE_SECTION__LIGHTING

  /***********************************
   * SECTION: Energy Configs
  ************************************/  

  // #ifdef ENABLE_TEMPLATE_SECTION__ENERGY
  //   #define USE_MODULE_ENERGY_INTERFACE
  // #endif
  
  // #ifdef ENABLE_TEMPLATE_SECTION__ENERGY__PZEM
  //   #define USE_MODULE_ENERGY_PZEM004T_V3
  //     #define ENABLE_DEVFEATURE_REDUCE_SUBORDINATE_MQTT_REPORTING_ENERGY // If energy_interface is primary reporting, reduce pzem to slower (debug only)
  //   #define MAX_ENERGY_SENSORS 4
  //   #define MAX_PZEM004T_DEVICES 4
  //   #define ENABLE_DEVFEATURE_PZEM004T__AUTOSEARCH
  // #endif
  // #ifdef ENABLE_TEMPLATE_SECTION__ENERGY__INA219
  //   #define USE_MODULE_ENERGY_INA219
  //   // #define ENABLE_DEVFEATURE_ENERGY__DISABLE_ENERGY_INTERFACE_FOR_DEBUGGING
  // #endif

  /***********************************
   * SECTION: Controller Configs
  ************************************/  

  // #define USE_MODULE_CONTROLLER_HVAC
  //    #define HEATING_DEVICE_MAX 4
  //    
  //    #define ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES

  //   #ifdef USE_LIGHTING_TEMPLATE
  //     #define USE_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR
  //     #define USE_MODULE_CONTROLLERS__RELAY_STATE_LEDSTRIP
  //       #define ENABLE_CONTROLLERS__RELAY_STATE_LEDSTRIP__SEGMENT_INDEX   1
  //   #endif // USE_LIGHTING_TEMPLATE
  //  #define USE_MODULE_CONTROLLER_CUSTOM__ENERGY_OLED

  
  #ifdef ENABLE_TEMPLATE_SECTION__CUSTOM__MAVLINK_FLYING_LEDS

      #define USE_MODULE_CONTROLLER_CUSTOM__MAVLINK_FLYING_LEDS


  #endif
   //   // Make all the water tank sensors be "remote sensors", then internally they will let me do the colour bar in the same way with IDs

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
      // "\"16\":\""  D_GPIO_PZEM0XX_RX_MODBUS_CTR "\"," 
      // "\"17\":\""  D_GPIO_PZEM0XX_TX_CTR "\","
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
      #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
      "\"17\":\"" D_GPIO_HWSERIAL2_TX_CTR   "\","
      "\"16\":\"" D_GPIO_HWSERIAL2_RX_CTR   "\","   
      #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER   
      "\"2\":\""  D_GPIO_LED1_INV_CTR "\""   // builtin led
      // 32 - LED Strip External
      // 21 - LED Strip Onboard
      // 25?
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
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
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
      "},"  
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":[" 
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
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_0_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_1_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_2_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_3_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "}"
      "]"
    "},"    
    "\"" D_MODULE_NETWORK_MQTT_CTR "\":{"
      "\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120,"
      "\"" D_REALTIME_SLOWDOWN "\":0"
    "}," 
    "\"MQTTSubscribe\":["
      "\"openhab_broadcast/nextion/group/hvac_home\","
      "\"openhab_broadcast/nextion/group/hvac_desk_power\""
    "],"
  "}";



  
#endif




/**
 * @brief 2024 Device
 * For testing lnog range devices, without a battery that uses the aircrafts power source. Last known will be used as tracking instead of constant updates.
 * 800L as smaller and lighter than 7000G
 */
#ifdef DEVICE_TESTBED__GPS_SERIAL_WITH_COMPASS
  #define DEVICENAME_CTR          "testbed_gps_serial"
  #define DEVICENAME_FRIENDLY_CTR "Plane2024 - 1Hz position updater"
  #define DEVICENAME_ROOMHINT_CTR "roaming"
  #define MQTT_HOST   "192.168.1.70"

  
  /***********************************
   * SECTION: System Debug Options
  ************************************/  

 
//   // #define DISABLE_SERIAL
//   // #define DISABLE_SERIAL0_CORE
  
//   // #define ENABLE_ADVANCED_DEBUGGING
//   // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
//   // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
//   // #define ENABLE_DEBUG_FUNCTION_NAMES

//   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES

//   #define ENABLE_FREERAM_APPENDING_SERIAL



  /***********************************
   * SECTION: System Configs
  ************************************/    
  
  /***********************************
   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
  ************************************/  

  #define ENABLE_GROUPFEATURE__GPS__WITH_SERIAL_POLLING
  #define ENABLE_GROUPFEATURE__GPS__WITH_SERIAL_INTERRUPTS_BUFFER
  #define ENABLE_GROUPFEATURE__COMPASS__QMC5883


  /***********************************
   * SECTION: Network Configs
  ************************************/    

//  #define ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS



//   /***********************************
//    * SECTION: Lighting Configs
//   ************************************/    


//   // #define ENABLE_GROUPFEATURE__DEBUG_CONNECTION_WITH_LOGGING
//   #define ENABLE_GROUPFEATURE__TESTING_NEW_OPTIONS


//   // #define ENABLE_DEVFEATURE_FASTBOOT_CELLULAR_SMS_BEACON_FALLBACK_DEFAULT_SSID
//   //                                                               #define ENABLE_DEVFEATURE___CAUTION_CAUTION__FORCE_CRASH_FASTBOOT_TESTING

//   #ifdef ENABLE_GROUPFEATURE__TESTING_NEW_OPTIONS
//     // #define ENABLE_DEBUGFEATURE__CELLULAR_CONNECTION_ISSUES
//     #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 10    
//     // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES    
//     // #define ENABLE_DEBUGFEATURE__LOGGING_MQTT__CHECK_CONNECTION
//     #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//     #define ENABLE_DEBUG_FUNCTION_NAMES
//     #define ENABLE_DEBUG_SHOW_ADVANCED_LOGS_FOR_STARTUP_UPSECONDS 20
//     // #define ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
//     #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_TASKS
//     #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 300
//     // #define ENABLE_DEVFEATURE__MQTT_SPLASH_CONNECTION_STATUS_BEFORE_SENDING
//     #define ENABLE_DEBUGFEATURE__MQTT_COUNT_PUBLISH_SUCCESS_RATE
//     #define ENABLE_DEVFEATURE__MQTT_CLEANING_UP_MANY_NETWORK_CHECKS
//     #define ENABLE_DEVFEATURE__MQTT_STOP_SENDING_EVERYTHING_ON_RECONNECT
//     // #define ENABLE_DEBUGFEATURE__MQTT_STOP_STATUS_BASE_TELEMETRY
//     // #define ENABLE_DEVFEATURE__NTP_OVER_CELLULAR_TEST_WITHOUT_INTERFACE_INTEGRATION
//     #define ENABLE_DEVFEATURE__TIME_UPDATE_WITH_GPS_TIME
//     // #define ENABLE_DEVFEATURE__TIME_NTP_UPDATE_WITH_VERSION2
//     #define ENABLE_DEVFEATURE__MODEM_FORCE_RECONNECT_WHEN_MQTT_IS_DISCONNECTED_SECONDS 600
//   #endif // ENABLE_GROUPFEATURE__TESTING_NEW_OPTIONS


//   #ifdef ENABLE_GROUPFEATURE__DEBUG_CONNECTION_WITH_LOGGING
//     #define ENABLE_ADVANCED_DEBUGGING
//     #define ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
//   #endif // ENABLE_GROUPFEATURE__DEBUG_CONNECTION_WITH_LOGGING


//   #define ENABLE_DEVFEATURE_TASKER__TASK_FUNCTION_QUEUE


//   // *************************************************************************************

//   // #ifdef USE_GROUPFEATURE__FASTER_SERIAL_LOGGING
//   //   #define SERIAL_DEBUG_BAUD_DEFAULT 921600
//   // #endif 
//   // #ifdef USE_GROUPFEATURE__MAVLINK_DECODER
//   //   #define USE_MODULE__DRIVERS_MAVLINK_DECODER
//   //     #define USE_FEATURE_SEARCH_FOR_UNHANDLED_MAVLINK_MESSAGES_ON_ALLOWEDLIST
//   //     #define ENABLE_FEATURE_MAVLINK_CONVERT_MQTT_DATA_VALUES
//   //     #define ENABLE_FEATURE_MAVLINK_MQTT_SEND_ALL_PACKETS_AS_TELEMETRY_TOPICS
//   //   #define USE_MODULE_CORE_SERIAL_UART
//   //     #define ENABLE_HARDWARE_UART_2
//   //     #define HARDWARE_UART_2_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
//   // #endif
//   // #ifdef USE_GROUPFEATURE__MODEM_GPS
//   //   #define USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
//   //   #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
//   //   #define USE_MODULE_SENSORS_GPS_MODEM
//   // #endif 
//   // #ifdef USE_GROUPFEATURE__MQTT_AS_CELLULAR 
//   //   #define DISABLE_NETWORK_WIFI
//   //   #define USE_MODULE_NETWORK_CELLULAR
//   //   #define USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED
//   //   #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
//   //   #define USE_MODULE_SENSORS_INTERFACE
//   //   #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
//   //   #define USE_MODULE_NETWORK_MQTT
//   //   // #define MQTT_SOCKET_TIMEOUT 1
//   //   // #define MQTT_PORT 51884 // Temporary exposed primry broker : STABLE
//   //    #define MQTT_PORT_CELLULAR 51883 //external mqtt broker on TOWER  : Unstable 192.168.1.66
//   //   // #define ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
//   //   // #define ENABLE_DEVFEATURE_SIM7000G_INIT_SKIP_MODEM_RESTART
//   //   #define ENABLE_DEVFEATURE_MQTT_BLOCK_TRANSMIT_IF_NOT_CONNECTED
//   //   // #define ENABLE_DEVFEATURE_CELLULAR_SMS__PDU_MODE  //no
//   // #endif
//   // #ifdef USE_GROUPFEATURE__MQTT_AS_WIFI
//   //   #define USE_MODULE_NETWORK_WIFI
//   //   #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
//   //   
//   //   #define MQTT_HOST       MQTT_HOST
//   //   #define MQTT_PORT     1883
//   // #endif
//   // #ifdef USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
//   //   #define DISABLE_NETWORK_WIFI
//   //   #define USE_MODULE_NETWORK_CELLULAR
//   //   #define USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED
//   //   #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
//   //   // #define USE_MODULE_SENSORS_INTERFACE
//   //   // #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
//   //   // #define MQTT_PORT 51883 // Temporary exposed primry broker : STABLE
//   //   // #define ENABLE_DEVFEATURE_STOP_MQTT_FROM_CONNECTING
//   // #endif

//   /**
//    *  GPS
//    * */
//   #ifdef ENABLE_GROUPFEATURE__GPS_SERIAL
//     #define USE_MODULE_SENSORS_GPS_SERIAL //remove?
//     #define USE_MODULE_SENSORS_GPS_SERIAL
//     #define ENABLE_GPS_PARSER_NMEA
//     #define ENABLE_GPS_PARSER_UBX
//     #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
//     #define NMEAGPS_DERIVED_TYPES
//     // #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
//     #define NMEAGPS_PARSE_SAVE_MILLIS
//     // #define gpsPort Serial1
//     // #define D_GPS_BAUD_RATE_FAST    921600
//     // #define D_GPS_BAUD_RATE_DEFAULT 9600

//     // #define USE_DEVFEATURE_GPS_POLLING_INPUT

//     // #define ENABLE_DEVFEATURE_GPS_SERIAL__NEW_CODE

    
// #define ENABLE_DEVFEATURE__START_STATIC_WHILE
// #define ENABLE_DEVFEATURE__START_STATIC_INIT_PORT
// #define ENABLE_DEVFEATURE__START_STATIC_LOOP

// #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME

//     #define NMEAGPS_PARSE_SAVE_MILLIS
//     #define gpsPort Serial2
//     // #define D_GPS_BAUD_RATE_FAST    921600
//     // #define D_GPS_BAUD_RATE_DEFAULT 9600
//     // #define D_GPS_BAUD_RATE_DEFAULT 115200
//     #define D_GPS_BAUD_RATE_DEFAULT 230400
//     // #define D_GPS_BAUD_RATE_DEFAULT 460800

//     // #define D_GPS_BAUD_RATE_DEFAULT 115200
//     // #define D_GPS_TX_PIN_DEFAULT 19
//     // #define D_GPS_RX_PIN_DEFAULT 18

//     #define USE_DEVFEATURE_GPS_POLLING_INPUT

//     // #define USE_MODULE_CORE__SERIAL

//     // #define ENABLE_DEVFEATURE_GPS_SERIAL__NEW_CODE

//     #define ENABLE_DEVFEATURE_USE_HARDWARE_SERIAL2_FOR_GPS

//     #define USE_DEVFEATURE_UBLOX_GLOBAL
    
//     // #define ENABLE_DEVFEATURE__ENABLE_UBX_PARSER_IN_CLASS

//     #define USE_DEVFEATURE__UBLOX_TEST_CLASS

//     #define ENABLE_DEBUGFEATURE__GPS_COMMANDS_FOR_TESTING





//     // #define USE_MODULE_DRIVERS_INTERFACE
//     // #define USE_MODULE_DRIVERS_SERIAL_UART
//     #define ENABLE_HARDWARE_UART_1
//     #define HARDWARE_UART_1_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
//   #endif // USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS

//   /***********************************
//    * SECTION: Template Configs
//   ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_CORE__SERIAL
      "\"17\":\"" D_GPIO_HWSERIAL2_TX_CTR   "\","
      "\"16\":\"" D_GPIO_HWSERIAL2_RX_CTR   "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_RF433_CODES
      "\"22\":\"" D_GPIO__RF_433MHZ_TX__CTR   "\","
      #endif  
      #ifdef USE_MODULE_DRIVERS_RF433_CODES
      "\"23\":\"" D_GPIO__RF_433MHZ_RX__CTR   "\","
      #endif  
      "\"2\":\"" D_GPIO_LED1_CTR  "\""
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  
//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
//     "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_GPIO_NUMBER "\":{"
//       #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
//       "\"22\":\"" D_GPIO_I2C_SCL_CTR   "\","
//       "\"21\":\"" D_GPIO_I2C_SDA_CTR   "\","   
//       #endif // USE_MODULE_DISPLAYS_OLED_SH1106   
//       #ifdef USE_MODULE_NETWORK_CELLULAR
//       "\"25\":\"" D_GPIO__MODEM_DATA_TERMINAL_READY_DTR__CTR   "\","
//       "\"27\":\"" D_GPIO__MODEM_TX__CTR   "\","   
//       "\"26\":\"" D_GPIO__MODEM_RX__CTR   "\","   
//       "\"4\":\""  D_GPIO__MODEM_POWER__CTR   "\","   
//       #endif // USE_MODULE_NETWORK_CELLULAR   


//       /** 5P small - UART1 GPS Stream
//        * Orange      19, UART1_TX
//        * Yellow      18, UART1_RX
//        * White        
//        * Red         VCC, 3V3
//        * Black       GND
//        * */
//       // "\"32\":\"" D_GPIO_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
//       // "\"33\":\"" D_GPIO_HWSERIAL1_RING_BUFFER_TX_CTR   "\","



//       #ifdef USE_MODULE_DRIVERS_SDCARD
//       "\"2\":\"" D_GPIO_SDCARD_HSPI_MISO_CTR   "\","
//       "\"15\":\"" D_GPIO_SDCARD_HSPI_MOSI_CTR   "\","   
//       "\"14\":\"" D_GPIO_SDCARD_HSPI_CLK_CTR   "\","
//       "\"13\":\"" D_GPIO_SDCARD_HSPI_CSO_CTR   "\","  
//       #endif // USE_MODULE_DRIVERS_SDCARD   
//       #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
//       "\"19\":\"" D_GPIO_HWSERIAL2_TX_CTR   "\","
//       "\"18\":\"" D_GPIO_HWSERIAL2_RX_CTR   "\","   
//       #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER   
//       "\"12\":\"" D_GPIO_LED1_INV_CTR "\","
//       "\"35\":\"" D_GPIO_ADC1_CH7_CTR "\""
//     "},"
//     "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
//     "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
//   "}";


#define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "gps"
  #define TEMP_MQTT_RECONNECT_SECOND_BACKOFF_CTR  "1" 

  /***********************************
   * SECTION: Device Configs
  ************************************/    

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE__SENSORS_GPS_MODEM__CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE__SENSORS_BATTERY_MODEM__CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "]"
    "},"    
    "\"" D_MODULE_NETWORK_MQTT_CTR "\":{"
      "\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120,"
      "\"" D_REALTIME_SLOWDOWN "\":0"
    "}," // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
    // "\"SMSAuto_GPS\":"  SMS_AUTO_GPS_TIME_SECONDS_RATE_CTR ","
    "\"MQTT\":{\"RetrySecs\":"  TEMP_MQTT_RECONNECT_SECOND_BACKOFF_CTR "}"
  "}";


  /***********************************
   * SECTION: Core Configs
  ************************************/     
  // #define USE_MODULE_CORE_FILESYSTEM
  //   
  //   
  //   

  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // /***********************************
  //  * SECTION: Network Configs
  // ************************************/    

  // // 

  // // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE


  // // #define ENABLE_DEBUG_LINE_HERE

  // #define ENABLE_DEVFEATURE__START_STATIC_WHILE
  // #define ENABLE_DEVFEATURE__START_STATIC_INIT_PORT
  // #define ENABLE_DEVFEATURE__START_STATIC_LOOP

  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME

  // /**
  //  *  GPS
  //  * */
  // // #ifdef ENABLE_GROUPFEATURE__GPS_SERIAL
  //   #define USE_MODULE_SENSORS_GPS_SERIAL //remove?
  //   #define USE_MODULE_SENSORS_GPS_SERIAL
  //   #define ENABLE_GPS_PARSER_NMEA
  //   #define ENABLE_GPS_PARSER_UBX
  //   #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
  //   #define NMEAGPS_DERIVED_TYPES
  //   // #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
  //   #define NMEAGPS_PARSE_SAVE_MILLIS
  //   // #define gpsPort Serial1
  //   // #define D_GPS_BAUD_RATE_FAST    921600
  //   // #define D_GPS_BAUD_RATE_DEFAULT 9600

  //   // #define USE_DEVFEATURE_GPS_POLLING_INPUT

  //   // #define ENABLE_DEVFEATURE_GPS_SERIAL__NEW_CODE

    
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

  //   #define ENABLE_DEBUGFEATURE__GPS_COMMANDS_FOR_TESTING





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

  //   // #define ENABLE_DEVFEATURE_NEOGPS__CLASS_AS_INSTANCE


  //   // #define USE_MODULE_DRIVERS_INTERFACE
  //   // #define USE_MODULE_DRIVERS_SERIAL_UART
  //   #define ENABLE_HARDWARE_UART_2
  //   #define HARDWARE_UART_2_BAUD_RATE_SPEED  9600  //D_GPS_BAUD_RATE_FAST
  // #endif // USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS




#endif // DEVICE_TESTBED__GPS_SERIAL_WITH_COMPASS

              



// /**
//  * 
//  * 
//  * Ethernet R
//  * 
//  * w/o  gnd
//  * o/w     5v
//  * w/g    left LED (upstairs)              D27
//  * bl/w   centre Relay (downstairs)        D5
//  * w/bl   centre LED (downstairs)          D33
//  * g/w    left Relay (upstairs)            D18
//  * w/br   right LED (boiler)               D26
//  * br/w   right Relay (boiler)             D19
//  * 
//  * Ethernet N
//  * 
//  * w/o  gnd
//  * o/w     5v
//  * w/g    SK6812 D4
//  * bl/w   NC
//  * w/bl   NC
//  * g/w    NC
//  * w/br   RX2 of esp32 from nextion
//  * br/w   TX2 of esp32 from nextion    
//  * 

// */
// #ifdef DEVICE_TESTBED__LED_MATRIX
//   #define DEVICENAME_CTR          "testbed_ledmatrix"
//   #define DEVICENAME_FRIENDLY_CTR "HVAC Desk DevPlatform"
//   #define DEVICENAME_ROOMHINT_CTR "Bedroom"
//   // #define MQTT_HOST   "192.168.1.70" // primary
//   #define MQTT_HOST   "192.168.1.70" // Auto as temporary IP
//     #define MQTT_PORT     1883
    
//   #define SETTINGS_HOLDER 1239


//   /***********************************
//    * SECTION: System Debug Options
//   ************************************/    
//   // #define DISABLE_SERIAL
//   // #define DISABLE_SERIAL0_CORE
  
//   // #define ENABLE_ADVANCED_DEBUGGING
//   // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
//   // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
//   // #define ENABLE_DEBUG_FUNCTION_NAMES

//   // #define ENABLE_DEBUG_LINE_HERE_TRACE
//   // #define ENABLE_DEBUG_LINE_HERE

//   // #define ENABLE_FREERAM_APPENDING_SERIAL

//   // #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

//   #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

//   /***********************************
//    * SECTION: System Configs
//   ************************************/     

  

  

//   #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging
//   // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
//   //  // until devices can reliably be used without compiling per device

//   // 

//   #define USE_MODULE_SENSORS_SUN_TRACKING


//   /***********************************
//    * SECTION: Network Configs
//   ************************************/    

//   /***********************************
//    * SECTION: Sensor Configs
//   ************************************/  

//   /***********************************
//    * SECTION: Display Configs
//   ************************************/  

//   /***********************************
//    * SECTION: Driver Configs
//   ************************************/  

//   /***********************************
//    * SECTION: Lighting Configs
//   ************************************/  

//   // #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//   // #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//   // #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
  
//   #define ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS

//   #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS  
//     #define ENABLE_FEATURE_LIGHTING__2D_MATRIX
//     #define ENABLE_FEATURE_LIGHTS__EFFECT__AUDIO_REACTIVE__2D
//     #define ENABLE_FEATURE_LIGHTS__EFFECT__AUDIO_REACTIVE__1D
//     
//     #define ENABLE_DEVFEATURE_LIGHT__HARDCODE_MATRIX_SETUP  
//     #define ENABLE_DEVFEATURE_LIGHT__INCLUDE_AUDIOREACTIVE_USERMOD
//     // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_LOAD_PALETTE_PATCH_IN_WEBUI_PALETTE_CHANGE
//   #endif 

//   


//   #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_FEBRUARY_2023
//   #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
//   
//     

//   
//   


  

//   // 13, 18, 19, 22, 23, 25, 26, 27       USED
//   // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
//   // 21 = on PCB (4 pixels)
//   // 32 = external
//   #define USE_LIGHTING_TEMPLATE
//   DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   R"=====(
//   {
//     "BusConfig":[
//       {
//         "Pin":32,
//         "ColourOrder":"GRB",
//         "BusType":"WS2812_RGB",
//         "Start":0,
//         "Length":256
//       }
//     ],
//     "Segment0": {
//       "PixelRange": [
//         0,
//         256
//       ],
//       "ColourPalette":"Party 16",
//       "PaletteMappingValues":[10,15,20],
//       "SegColour0": {
//         "Hue": 0,
//         "Sat":100,
//         "BrightnessRGB":1
//       },
//       "SegColour1": {
//         "Hue": 120,
//         "Sat":20,
//         "BrightnessRGB":1
//       },
//       "SegColour2": {
//         "Hue": 240,
//         "Sat":100,
//         "BrightnessRGB":1
//       },
//       "Effects": {
//         "Function":"Gradient",
//         "Speed":127,
//         "Intensity":127,
//         "Decimate":0,
//         "Grouping":1
//       },
//       "Transition": {
//         "TimeMs": 0,
//         "RateMs": 25
//       },
//       "BrightnessRGB": 100,
//       "BrightnessCCT": 0
//     },
//     "BrightnessRGB": 20,
//     "BrightnessCCT": 0
//   }
//   )=====";

//   /***********************************
//    * SECTION: Energy Configs
//   ************************************/  

//   /***********************************
//    * SECTION: Controller Configs
//   ************************************/  

//   /***********************************
//    * SECTION: GPIO Template
//   ************************************/  

//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
//     "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_GPIOC "\":{"
//       #ifdef USE_MODULE_DRIVERS_RELAY
//       "\"13\":\"" D_GPIO_REL1_INV_CTR  "\","
//       "\"27\":\"" D_GPIO_REL2_INV_CTR    "\","
//       "\"26\":\"" D_GPIO_REL3_INV_CTR      "\","
//       "\"14\":\"" D_GPIO_REL4_INV_CTR      "\"," //pins need sety on L
//       #endif
//       "\"16\":\""  D_GPIO_PZEM0XX_RX_MODBUS_CTR "\"," 
//       "\"17\":\""  D_GPIO_PZEM0XX_TX_CTR "\","
//       #ifdef USE_MODULE_SENSORS_DS18X20
//       "\"33\":\"" D_GPIO_DS18X20_1_CTR "\"," // DS_DB - 3 pin
//       #endif    
//       #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750)
//       "\"23\":\"" D_GPIO_I2C_SCL_CTR   "\","
//       "\"22\":\"" D_GPIO_I2C_SDA_CTR   "\","   
//       #endif
//       #ifdef USE_MODULE_DISPLAYS_NEXTION
//       "\"18\":\"" D_GPIO_NEXTION_TX_CTR "\","
//       "\"19\":\"" D_GPIO_NEXTION_RX_CTR "\","
//       #endif
//       "\"2\":\""  D_GPIO_LED1_INV_CTR "\""   // builtin led
//       // 32 - LED Strip External
//       // 21 - LED Strip Onboard
//       // 25?
//       // 
//     "},"
//     "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
//     "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
//   "}";


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
//   #define D_DEVICE_SENSOR_PZEM004T_1_ADDRESS "2"
//   #define D_DEVICE_SENSOR_PZEM004T_2_ADDRESS "3"
//   #define D_DEVICE_SENSOR_PZEM004T_3_ADDRESS "4"
  
//   #define D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR D_DEVICE_HEATER_0_NAME
//   #define D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR D_DEVICE_HEATER_1_NAME
//   #define D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR D_DEVICE_HEATER_2_NAME
//   #define D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR D_DEVICE_HEATER_3_NAME 
  
//   #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_0_NAME
//   #define D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_1_NAME
//   #define D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_2_NAME
//   #define D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_3_NAME

//   #define USE_FUNCTION_TEMPLATE
//   DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
//   "{"
//     "\"" D_ENERGY "\":{"
//         "\"DeviceCount\":4"    
//     "},"
//     "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":{"
//         "\"DeviceCount\":4"    
//     "},"
//     "\"" D_DEVICENAME "\":{"
//       "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
//         "\"" D_DEVICE_HEATER_0_NAME "\","
//         "\"" D_DEVICE_HEATER_1_NAME "\","
//         "\"" D_DEVICE_HEATER_2_NAME "\","
//         "\"" D_DEVICE_HEATER_3_NAME "\""
//       "],"
//       "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
//         "\"" D_DEVICE_HEATER_0_NAME "\","
//         "\"" D_DEVICE_HEATER_1_NAME "\","
//         "\"" D_DEVICE_HEATER_2_NAME "\","
//         "\"" D_DEVICE_HEATER_3_NAME "\""
//       "],"
//       "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
//         // Downstairs
//         "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
//         "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
//         "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
//         "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\""
//       "],"
//       "\"" D_MODULE_SENSORS_DHT_CTR "\":["
//         "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
//       "],"
//       "\"" D_MODULE_SENSORS_SUN_TRACKING_CTR "\":["
//         "\"" "Desk" "\""
//       "],"  
//       "\"" D_MODULE_SENSORS_BME_CTR "\":["
//         "\"" D_DEVICE_SENSOR_BME_280_NAME "\","
//         "\"" D_DEVICE_SENSOR_BME_680_NAME "\""
//       "],"
//       "\"" D_MODULE_SENSORS_INA219_CTR "\":["
//         "\"" D_DEVICE_SENSOR_CURRENT "\""
//       "],"
//       "\"" D_MODULE_SENSORS_BH1750_CTR "\":["
//         "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
//       "],"
//       "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":["
//         "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
//         "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\","
//         "\"" D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "\","
//         "\"" D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR "\""
//       "],"
//       "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":["
//         "\"" D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "\","
//         "\"" D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR "\","
//         "\"" D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR "\","
//         "\"" D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR "\""
//       "],"
//       "\"" D_MODULE_CONTROLLER_HVAC_CTR "\":["
//         "\"" D_DEVICE_HEATER_0_NAME "\","
//         "\"" D_DEVICE_HEATER_1_NAME "\","
//         "\"" D_DEVICE_HEATER_2_NAME "\","
//         "\"" D_DEVICE_HEATER_3_NAME "\""
//       "]"
//     "},"
//     "\"" D_SENSORADDRESS "\":{"
//       "\"" D_MODULE_SENSORS_DB18S20_CTR "\":{" 
//         // Downstairs
//         "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
//         "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
//         "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\":" D_DEVICE_SENSOR_DB18S20_03_ADDRESS ","
//         "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\":" D_DEVICE_SENSOR_DB18S20_04_ADDRESS ""
//       "},"  
//       "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":[" 
//         D_DEVICE_SENSOR_PZEM004T_0_ADDRESS ","
//         D_DEVICE_SENSOR_PZEM004T_1_ADDRESS ","
//         D_DEVICE_SENSOR_PZEM004T_2_ADDRESS ","
//         D_DEVICE_SENSOR_PZEM004T_3_ADDRESS
//       "]"  
//     "},"
//     "\"" "HVACZone" "\":{"
//       "\"" "SetSensor" "\":["
//         "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
//         "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
//         "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
//         "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\""
//       "],"
//       "\"" "SetOutput" "\":["
//         "{"
//           "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
//           "\"" "DriverName" "\":\"" D_DEVICE_HEATER_0_NAME "\","
//           "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
//         "},"
//         "{"
//           "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
//           "\"" "DriverName" "\":\"" D_DEVICE_HEATER_1_NAME "\","
//           "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
//         "},"
//         "{"
//           "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
//           "\"" "DriverName" "\":\"" D_DEVICE_HEATER_2_NAME "\","
//           "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
//         "},"
//         "{"
//           "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
//           "\"" "DriverName" "\":\"" D_DEVICE_HEATER_3_NAME "\","
//           "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
//         "}"
//       "]"
//     "},"
//         
    // "\"" D_MODULE_NETWORK_MQTT_CTR "\":{"
    //   "\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120,"
    //   "\"" D_REALTIME_SLOWDOWN "\":0"
    // "}"
//   "}";



  
// #endif










// /**
//  * @brief 
//  * Make several of these, one in the garage to show progress
//  * One in my bedroom to motivate me
//  * 
//  */
// #ifdef DEVICE_LIGHTING__LED_MATRIX_BOX_01
//   #ifndef DEVICENAME_CTR
//     #define DEVICENAME_CTR          "led_matrix_box_01"
//   #endif
//   #ifndef DEVICENAME_FRIENDLY_CTR
//     #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
//   #endif
//   #ifndef DEVICENAME_DESCRIPTION_CTR
//     #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
//   #endif
//   #define DEVICENAME_ROOMHINT_CTR "Roaming"
//   #define MQTT_HOST   "192.168.1.70"
//     #define MQTT_PORT     1883
    
//   #define SETTINGS_HOLDER 1239

//   /***********************************
//    * SECTION: System Debug Options
//   ************************************/    
//   // #define DISABLE_SERIAL
//   // #define DISABLE_SERIAL0_CORE
  
//   // #define ENABLE_ADVANCED_DEBUGGING
//   // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
//   // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
//   // #define ENABLE_DEBUG_FUNCTION_NAMES

//   #define ENABLE_FREERAM_APPENDING_SERIAL

//   /***********************************
//    * SECTION: System Configs
//   ************************************/     


//   /***********************************
//    * SECTION: Network Configs
//   ************************************/    

//   
//   
  
//   /***********************************
//    * SECTION: Lighting Configs
//   ************************************/    
//   #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//   #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//   #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options

//   #define ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__LED_MATRIX

//   #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_OCTOBER_2023

//   #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE

//   

//   
  
//   // 13, 18, 19, 22, 23, 25, 26, 27       USED
//   // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
//   #define USE_LIGHTING_TEMPLATE
//   DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   R"=====(
//   {
//     "BusConfig":[
//       {
//         "Pin":19,
//         "ColourOrder":"RGB",
//         "BusType":"WS2812_RGB",
//         "Start":0,
//         "Length":256
//       }
//     ],
//     "Segment0": {
//       "PixelRange": [
//         0,
//         256
//       ],
//       "ColourPalette":"Christmas Snowy 02",
//       "SegColour0": {
//         "Hue": 0,
//         "Sat":100,
//         "BrightnessRGB":5
//       },
//       "Effects": {
//         "Function":"Shimmer",
//         "Speed":127,
//         "Intensity":101,
//         "Decimate":0,
//         "Grouping":1
//       },
//       "Transition": {
//         "TimeMs": 0,
//         "RateMs": 1000
//       },
//       "BrightnessRGB": 100,
//       "BrightnessCCT": 0
//     },
//     "BrightnessRGB": 100,
//     "BrightnessCCT": 0
//   }
//   )=====";
  
//   /***********************************
//    * SECTION: Template Configs
//   ************************************/    

//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_NAME          "\":\"" DEVICENAME_CTR "\","
//     "\"" D_FRIENDLYNAME  "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_GPIO_FUNCTION "\":{},"
//     "\"" D_BASE          "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
//     "\"" D_ROOMHINT      "\":\"" DEVICENAME_ROOMHINT_CTR "\""
//   "}";

// #endif // DEVICE_CHRISTMAS__OUTSIDE_WREATH




#ifdef DEVICE_TESTBED__NEXTION_DISPLAY__GENERIC_WITH_WEBUI__10INCH //10inch display
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "nextion_testbed"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define MQTT_HOST   "192.168.1.70"
    #define MQTT_PORT     1883

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  ///////////////////////////////////////////// Enable Logs
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  #define ENABLE_DEBUG_MANUAL_DELAYS // permits blocking delays
  
  ///////////////////////////////////////////// System Logs
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES
  
  #define SERIAL_LOG_LEVEL_DURING_BOOT 8
  // #define ENABLE_DEBUG_LINE_HERE3
  // #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS_OLD
  // #define USE_DEBUG_PRINT

  ///////////////////////////////////////////// Module Logs
  // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS  
  #define ENABLE_FREERAM_APPENDING_SERIAL
  
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

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  /***********************************
   * SECTION: Display Configs
  ************************************/  

  #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
   


  #ifdef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
    
    
  #endif // ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  // #define ENABLE_FREERAM_APPENDING_SERIAL

    // #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 115200
    #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 921600

  // #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_NEXTION

  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY        
        #define ENABLE_DEVFEATURE_NEXTION_WEBUI
        #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER 1
        #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
         
        
        #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER_SERIAL1_HVAC_DESK



// #define USE_FEATURE_NEXTION__SERIAL_DEFAULT_BUAD_NEW_PANEL_FIRST_OTA

// #define USE_FEATURE_NEXTION__FORCE_SERIAL_BAUDRATE_FROM_DEFAULT 115200

  
  #define USE_MODULE_DISPLAYS_NEXTION
    #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  #define NEXTION_DEFAULT_PAGE_NUMBER 6  
    
    //  // change to be code option later
    #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
    #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE

  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  
  #define ENABLE_DEVFEATURE_NEXTION_WEBUI

  // 
  
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
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"17\":\"" D_GPIO_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_NEXTION_RX_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";



  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
        
    "\"" D_MODULE_NETWORK_MQTT_CTR "\":{"
      "\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120,"
      "\"" D_REALTIME_SLOWDOWN "\":0"
    "}," 
    "\"MQTTSubscribe\":["
      "\"openhab_broadcast/nextion/group/#\""
    "],"
  "}";


#endif



#ifdef DEVICE_TESTBED__NEXTION_DISPLAY__GENERIC_WITH_WEBUI__7INCH
  #define DEVICENAME_CTR            "nextion_desk"
  #define DEVICENAME_FRIENDLY_CTR   "Testbed Nextion Display"
  #define DEVICENAME_ROOMHINT_CTR                   "Testbed"
  #define MQTT_HOST   "192.168.1.70"
    #define MQTT_PORT 1883

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


  #define ENABLE_FEATURE_WATCHDOG_TIMER
  
  


  #define   ENABLE_DEBUG_LINE_HERE


  #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
   


  #ifdef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
    
    
  #endif // ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_PIR

  // #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_NEXTION

  
  #define USE_MODULE_DISPLAYS_NEXTION
    #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  #define NEXTION_DEFAULT_PAGE_NUMBER 2  
    
    //  // change to be code option later
    #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
    #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE

  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  
  #define ENABLE_DEVFEATURE_NEXTION_WEBUI

  // #define ENABLE_DEVFEATURE_NEXTION__PHASE_OUT_COMMAND_FORMATTED

  // 

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"17\":\"" D_GPIO_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_NEXTION_RX_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


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


#endif


#ifdef DEVICE_INSTALLED__NEXTION_DISPLAY__KITCHEN_7INCH
  #define DEVICENAME_CTR            "nextion_kitchen"
  #define DEVICENAME_FRIENDLY_CTR   "Installed Nextion Display"
  #define DEVICENAME_ROOMHINT_CTR                   "Installed"
  #define MQTT_HOST   "192.168.1.70"
    #define MQTT_PORT 1883
  #define SUBDEVICE_TESTBED__NEXTION_DISPLAY__KITCHEN_7INCH
#endif
#ifdef DEVICE_TESTBED__NEXTION_DISPLAY__KITCHEN_7INCH
  #define DEVICENAME_CTR            "nextion_kitchen_testbed"
  #define DEVICENAME_FRIENDLY_CTR   "Testbed Nextion Display"
  #define DEVICENAME_ROOMHINT_CTR                   "Testbed"
  #define MQTT_HOST   "192.168.1.70"
    #define MQTT_PORT 1883
  #define SUBDEVICE_TESTBED__NEXTION_DISPLAY__KITCHEN_7INCH
#endif
#ifdef SUBDEVICE_TESTBED__NEXTION_DISPLAY__KITCHEN_7INCH

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

  #define ENABLE_DEBUG_LINE_HERE

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  /***********************************
   * SECTION: System Configs
  ************************************/     

  
  



  #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
   


  #ifdef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
    
    
  #endif // ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_PIR

  // #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_NEXTION

  
  #define USE_MODULE_DISPLAYS_NEXTION
    #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  #define NEXTION_DEFAULT_PAGE_NUMBER 5  
    
    //  // change to be code option later
    #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
    #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE
    #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 921600

    

  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY        
        #define ENABLE_DEVFEATURE_NEXTION_WEBUI
        #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER 1
        #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
         
        
        #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER_SERIAL1_HVAC_DESK


  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  
  #define ENABLE_DEVFEATURE_NEXTION_WEBUI

  // #define ENABLE_DEVFEATURE_NEXTION__PHASE_OUT_COMMAND_FORMATTED

  // 

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"17\":\"" D_GPIO_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_NEXTION_RX_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  DEFINE_PGM_CTR(DISPLAY_TEMPLATE)
  R"=====(
  {
    "ObjectNameID": {
      "hIconUS": 3,
      "hTimeUS": 7,
      "hBoostUS": 12,
      "hAutoUS": 16,
      "hIconDS": 4,
      "hTimeDS": 8,
      "hBoostDS": 13,
      "hAutoDS": 17,
      "hIconIH": 5,
      "hTimeIH": 9,
      "hBoostIH": 14,
      "hAutoIH": 18,
      "hIconWB": 6,
      "hTimeWB": 10,
      "hBoostWB": 15,
      "hAutoWB": 19,
      "hIconCook": 55,
      "hTimeCook": 56,
      "hBoostCook": 57,
      "hAutoCook": 58,
      "powGardenLgt0": 151,
      "powGardenLgt10": 152,
      "powGardenLgt1": 153,
      "powOutsLght0": 154,
      "powOutsLght10": 155,
      "powOutsLght1": 156,
      "powGazLED": 157,
      "powGazWater": 158,
      "posGazSpeaker": 159
    }
  }
  )=====";

  //start here, probably roll into general command structure. I maybe need to create "COMMAND_SET_01" and 02,03 up to 5 as needed




  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
        
    "\"" D_MODULE_NETWORK_MQTT_CTR "\":{"
      "\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120,"
      "\"" D_REALTIME_SLOWDOWN "\":0"
    "},"
    "\"MQTTSubscribe\":["
      "\"openhab_broadcast/nextion/group/#\""
    "],"
  "}";




#endif


#ifdef DEVICE_TESTBED__NEXTION_DISPLAY__TREADMILL_01
  #define DEVICENAME_CTR            "nextion_treadmill_01"
  #define DEVICENAME_FRIENDLY_CTR   "Testbed Nextion Display"
  #define DEVICENAME_ROOMHINT_CTR                   "Testbed"
  #define MQTT_HOST   "192.168.1.70"
    #define MQTT_PORT 1883

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

  #define ENABLE_FREERAM_APPENDING_SERIAL

  /***********************************
   * SECTION: System Configs
  ************************************/     

  
  


  #define   ENABLE_DEBUG_LINE_HERE


  #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
   


  #ifdef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
    
    
  #endif // ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  // #define ENABLE_FREERAM_APPENDING_SERIAL
    // #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 230400

  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_PIR

  // #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_NEXTION


  #define USE_MODULE_DISPLAYS_NEXTION
    #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  #define NEXTION_DEFAULT_PAGE_NUMBER 6
    
    //  // change to be code option later
    #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
    #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE
    #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 921600

    

  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  
  #define ENABLE_DEVFEATURE_NEXTION_WEBUI

  // #define ENABLE_DEVFEATURE_NEXTION__PHASE_OUT_COMMAND_FORMATTED

  // 

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"17\":\"" D_GPIO_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_NEXTION_RX_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


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


#endif


#ifdef DEVICE_TESTBED__NEXTION_DISPLAY__TREADMILL_02
  #define DEVICENAME_CTR            "nextion_treadmill_02"
  #define DEVICENAME_FRIENDLY_CTR   "Testbed Nextion Display"
  #define DEVICENAME_ROOMHINT_CTR                   "Testbed"
  #define MQTT_HOST   "192.168.1.70"
    #define MQTT_PORT 1883

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

  #define ENABLE_FREERAM_APPENDING_SERIAL

  /***********************************
   * SECTION: System Configs
  ************************************/     


  
  


  #define   ENABLE_DEBUG_LINE_HERE


  #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
   


  #ifdef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
    
    
  #endif // ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_PIR

  // #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_NEXTION

  
  #define USE_MODULE_DISPLAYS_NEXTION
    #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  #define NEXTION_DEFAULT_PAGE_NUMBER 6 
    #define ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT
    //  // change to be code option later
    #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
    #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE
    #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 230400

  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  
  #define ENABLE_DEVFEATURE_NEXTION_WEBUI

  // #define ENABLE_DEVFEATURE_NEXTION__PHASE_OUT_COMMAND_FORMATTED

  // 

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"17\":\"" D_GPIO_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_NEXTION_RX_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


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


#endif


/**
 * @brief 
 * To test the ensuite code
 */
#ifdef DEVICE_ACTIVE_DEVELOPMENT__HALLWAY_FLOWERS
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define MQTT_HOST   "192.168.1.70"
    #define MQTT_PORT     1883

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

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

  // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE // comment out to enable fastboot recovery

  #define LOG_BUFFER_SIZE 2000

  /***********************************
   * SECTION: System Configs
  ************************************/     

    

  #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging


  /***********************************
   * SECTION: Network Configs
  ************************************/    

  
  

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  /***********************************
   * SECTION: Display Configs
  ************************************/  

  /***********************************
   * SECTION: Driver Configs
  ************************************/  

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

 #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
 
    
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_NO_GRADIENT
    #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_WITH_GRADIENT


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_FUNCTION "\":{},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE

  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":27,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        100
      ],
      "ColourPalette":"Colourful Greenless",
      "Effects": {
        "Function":"Static Palette Aged",
        "Speed":127,
        "Intensity":47,
        "Grouping":1,
        "RateMs": 1000
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";
  

#endif // DEVICE_TESTGROUP__LIGHTING_EFFECTS__01__ESP32_1CH




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
 *                         |5V     |USB|     5|
 *                         |GND              6| 
 *                         |3V3              7|
 *                         |4  (fL, BIL)     8| I2C SDA   - Blue LED
 *                         |3                9| I2C SCL
 *                         |2 fL            10| 
 *                    U1RX |1               20| U0TX
 *                    U1TX |0               21| U0RX
 *                         ____________________
 * 
        Pin	Function	ESP Pin	Input/Output	Description
        1	5V	5V	POWER INPUT	5V power input for the board
        2	GND	GND	POWER GROUNT	Ground connection
        3	3V3	3.3V	POWER OUTPUT	3.3V power output
        4	IO0	A0	BIDIRECTIONAL	GPIO, ADC pin, PWM
        5	IO1	A1	BIDIRECTIONAL	GPIO, ADC pin, PWM
        6	IO2	A2	BIDIRECTIONAL	GPIO, ADC pin, PWM
        7	IO3	A3	BIDIRECTIONAL	GPIO, ADC pin, PWM
        8	IO4	A4	BIDIRECTIONAL	GPIO, ADC pin, SCK, PWM
        9	IO5	A5	BIDIRECTIONAL	GPIO, ADC pin, SPI Master In Slave Out, PWM
        10	IO6	MISO	BIDIRECTIONAL	GPIO, SPI Master Out Slave In, PWM
        11	IO7	SS	BIDIRECTIONAL	GPIO, SPI Slave Select, PWM
        12	IO8	SDA	BIDIRECTIONAL	GPIO, I2C Data line, PWM
        13	IO9	SCL	BIDIRECTIONAL	GPIO, I2C Clock line, PWM
        14	IO10	RX	BIDIRECTIONAL	GPIO, PWM
        15	IO21	TX	BIDIRECTIONAL	GPIO, UART Transmit
        16	IO20	RX	BIDIRECTIONAL	GPIO, UART Receive (secondary)

        Hardware serial port, there are two hardware serial ports on the board:
          USB serial port
          ART serial port

        GPIO Matrix pins with one of the following important functions, as described in Section 2.3.3
          Restrictions for GPIOs:–GPIO2,
          GPIO8, GPIO9 : Strapping pins.–GPIO18,
          GPIO19 : USB_C Serial/JTAG interface.–GPIO4,
          GPIO5, GPIO6, GPIO7 : JTAG interface.–GPIO2
          0, GPIO21 : UART0 interface.–GPIO11: The VDD_SPI pin. The power supply pin for flash by default, and can only be reconfigured
          as a GPIO pin if the flash is powered by an external power supply.

  * BH1750 (Good)
  * PIR small
  * Radar
  * BME680
  * maybe some thin leds?


 */
#ifdef DEVICE_TESTBED__ROOM_SENSOR_ESP32C3_SUPERMINI
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
// // #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS_OLD
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
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__ULTRASONIC
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__PIR_SMALL
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__PIR_LARGE
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ
#define ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_24GHZ
#define ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
#define ENABLE_TEMPLATE_SECTION__SENSORS__BME
// #define ENABLE_TEMPLATE_SECTION__LIGHTS__NEOPIXELBUS


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
   #define USE_MODULE_SENSORS_PIR
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

#ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
#define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BH1750
#endif
#ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
  #define USE_MODULE_SENSORS_BME
    
#endif
#ifdef ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_24GHZ
  #define USE_MODULE_SENSORS__RADAR_HLK_LD2410
  #define ENABLE_FEATURE_HLK_LD2410__USE_SERIAL_CHUNK_MODE
#endif

// //   #define ENABLE_ADVANCED_DEBUGGING
// //   #define ENABLE_DEBUG_FUNCTION_NAMES
// //   #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
// //   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
// //   // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
// //   #define ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
// //   #define ENABLE_SERIAL_FLUSH
// //   #define DEBUG_FASTBOOT
// //   #define ENABLE_DEBUG_LINE_HERE


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

/***********************************
 * SECTION: Lighting Configs
************************************/  

#ifdef ENABLE_TEMPLATE_SECTION__LIGHTS__NEOPIXELBUS
  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_JANUARY_2025_NO_GPIO
  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":8,
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
    #if defined(USE_MODULE_SENSORS__TOF_VL53L0X) || defined(USE_MODULE_SENSORS__TOF_VL53L1X) || defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750) || defined(USE_MODULE_ENERGY_INA219) || defined(USE_MODULE_DISPLAYS_OLED_SH1106)
    "\"10\":\"" D_GPIO_I2C_SDA_CTR   "\"," //instead of 9
    "\"9\":\"" D_GPIO_I2C_SCL_CTR   "\","    
    #endif
    #ifdef USE_MODULE_SENSORS_PIR
    "\"4\":\""  D_GPIO_PIR_1_CTR "\","
    "\"7\":\""  D_GPIO_PIR_2_CTR "\","
    #endif
    #ifdef USE_MODULE_SENSORS_SR04
    "\"4\":\"" D_GPIO_SR04_ECHO_CTR   "\","
    "\"2\":\"" D_GPIO_SR04_TRIG_CTR  "\","  
    #endif 
    #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ
    "\"35\":\""  D_GPIO_PIR_2_INV_CTR "\","
    #endif
    #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__PIR_SMALL
    "\"5\":\""  D_GPIO_PIR_3_INV_CTR "\","
    #endif
    #ifdef USE_MODULE_SENSORS__RADAR_HLK_LD2410
    "\"6\":\""  D_GPIO__HLK_LD2410_TX__CTR "\","
    "\"5\":\""  D_GPIO__HLK_LD2410_RX__CTR "\""
    #endif
    #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__PIR_LARGE
    "\"34\":\""  D_GPIO_PIR_1_INV_CTR "\""
    #endif
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

#define D_DEVICE_SENSOR_BH1750_NAME "Hallway"
#define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Hallway"


#define USE_FUNCTION_TEMPLATE
DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
"{"
  "\"" D_DEVICENAME "\":{"
    "\"" D_MODULE_SENSORS_PIR_CTR "\":["
      "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "1\","
      "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "2\","
      "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "3\""
    "],"
    "\"" D_MODULE_SENSORS__TOF_VL53L0X__CTR "\":["
      "\"" "TOF_VL53L0X" "\""
    "],"
    "\"" D_MODULE_SENSORS__TOF_VL53L1X__CTR "\":["
      "\"" "TOF_VL53L1X" "\""
    "],"
    "\"" D_MODULE_SENSORS_SR04_CTR "\":["
      "\"" "SRO4" "\""
    "],"
    "\"" D_MODULE_SENSORS_BH1750_CTR "\":["
      "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
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
      "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
    "]"
  "},"    
  "\"" D_MODULE_NETWORK_MQTT_CTR "\":{"
    "\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120,"
    "\"" D_REALTIME_SLOWDOWN "\":0"
  "},"
  "\"MQTT_Interface_Priority\":{\"" D_MODULE_ENERGY_INTERFACE_CTR "\":1}" // Each interface will have ability to reduce its subclass mqtt "ifchanged" rate
"}";


#endif




#ifdef DEVICE_TESTBED__BOARDTEST_ESP32_WROVER_E
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
// // #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS_OLD
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
    // #if defined(USE_MODULE_SENSORS__TOF_VL53L0X) || defined(USE_MODULE_SENSORS__TOF_VL53L1X) || defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750) || defined(USE_MODULE_ENERGY_INA219) || defined(USE_MODULE_DISPLAYS_OLED_SH1106)
    // "\"8\":\"" D_GPIO_I2C_SDA_CTR   "\","
    // "\"9\":\"" D_GPIO_I2C_SCL_CTR   "\""    
    // #endif
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
    
    // "\"" D_MODULE_NETWORK_MQTT_CTR "\":{"
    //   "\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120,"
    //   "\"" D_REALTIME_SLOWDOWN "\":0"
    // "}"
//   "\"MQTT_Interface_Priority\":{\"" D_MODULE_ENERGY_INTERFACE_CTR "\":1}" // Each interface will have ability to reduce its subclass mqtt "ifchanged" rate
// "}";


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
 *                  DB_3V3 |3V3     |USB|     VIN| TOF 5V
 *                  DB_GND |GND               GND| TOF GND
 *                    DB18 |15 (fL)            13|
 *                         |2  (fL, BIL)  (fH) 12| 
 *                         |4             (fH) 14|
 *                         |RX2/17             27| 
 *                         |TX2/16             26|  
 *                         |5  (fL)            25|  
 *                         |18                 33|  
 *                         |19                 32|  
 *             TOF I2C_SDA |21  SDA     (fL) * 35|   
 *                         |RX0         (fL) * 34|  
 *                         |TX0              ' VN| 
 *             TOF I2C_SCL |22  SCL          ' VP| 
 *                         |23               ' EN| 
 *                          _____________________
 * 
 */
#ifdef DEVICE_DEVBOARD__DESK__ESP32DOIT_GPIO_HARDWARE_PINS_TESTING
#ifndef DEVICENAME_CTR
#define DEVICENAME_CTR          "testbed_default"
#endif
#ifndef DEVICENAME_FRIENDLY_CTR
#define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
#endif
#ifndef DEVICENAME_DESCRIPTION_CTR
#define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
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

#define ENABLE_DEBUG_MANUAL_DELAYS

// #define ENABLE_DEVFEATURE_GPIO_PIN_METHOD_MAY_2025


/////////////////////////////////////////// Module Logs
//  #define ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS  
//  #define ENABLE_FREERAM_APPENDING_SERIAL

// /***********************************
// * SECTION: System Configs
// ************************************/    

// #define SETTINGS_HOLDER 1239

// 
// 
// #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
//  // until devices can reliably be used without compiling per device
// 
   
// /***********************************
// * SECTION: Network Configs
// ************************************/    

// 
//   

// /***********************************
// * SECTION: Sensor Configs
// ************************************/  

// /*------------------------------------
// * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
// * -----------------------------------*/  

// #define ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L1X
// #define ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20

// /***********************************
// * SECTION: Sensor Configs
// ************************************/  

// #define USE_MODULE_SENSORS_INTERFACE

// #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L1X
// #define USE_MODULE_SENSORS__TOF_VL53L1X
// #define ENABLE_DEVFEATURE_I2C__SET_WIRE_INSTANCE_WITH_TWOWIRE_ZERO
// #define VL53L1X_DISTANCE_MODE Short
// #endif
// #define ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING
// //  #define ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING__HVACDESK_HARDCODED_ADD
// #define ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING__HVACDESK_OILTANK_ADD
// #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
//  #define USE_MODULE_SENSORS_DS18X20
//    #define DS18X20_MAX_SENSORS 5
//      #define ENABLE_DEBUG_MQTT_CHANNEL_DB18X20    
//       #define ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
// #endif 

  #define USE_MODULE_DRIVERS_LEDS

  // #define ENABLE_DEVFEATURE_DRIVER_LED__FORCED_LED_TOGGLE_ON_PIN 2
  #define ENABLE_DEVFEATURE_DRIVER_LED__FORCED_LED_TOGGLE_LED1

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
   #ifdef USE_MODULE_SENSORS_DS18X20
   "\"15\":\"" D_GPIO_DS18X20_1_CTR "\","
   #endif            
   #if defined(USE_MODULE_SENSORS__TOF_VL53L0X) || defined(USE_MODULE_SENSORS__TOF_VL53L1X) || defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750) || defined(USE_MODULE_ENERGY_INA219) || defined(USE_MODULE_DISPLAYS_OLED_SH1106)
   "\"21\":\"" D_GPIO_I2C_SDA_CTR   "\","
   "\"22\":\"" D_GPIO_I2C_SCL_CTR   "\""   
   #endif
   "\"2\":\""  D_GPIO_LED1_INV_CTR "\""  
 "},"
 "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
 "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
"}";

/***********************************
* SECTION: TEMPLATE: Names
************************************/    

#define D_DEVICE_SENSOR_DB18S20_01_NAME        "OilTank"
#define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,131,147,47,0,0,0,190]"

#define USE_FUNCTION_TEMPLATE
DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
"{"
 "\"" D_DEVICENAME "\":{"
   "\"" D_MODULE_SENSORS__TOF_VL53L1X__CTR "\":["
     "\"" "OilTankDistance" "\""
   "],"
    "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
      "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\""
    "],"
 "},"
 "\"" D_SENSORADDRESS "\":{"
    "\"" D_MODULE_SENSORS_DB18S20_CTR "\":{" 
      "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ""
    "}"   
  "},"    
  "\"" D_MODULE_NETWORK_MQTT_CTR "\":{"
    "\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120,"
    "\"" D_REALTIME_SLOWDOWN "\":0"
  "}"
"}";


#endif





#ifdef DEVICE_TESTBED__BOARDTEST_ESP32S3__XIAO_ESP32_S3_SEEDSTUDIO
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
// // #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS_OLD
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
    
    // "\"" D_MODULE_NETWORK_MQTT_CTR "\":{"
    //   "\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120,"
    //   "\"" D_REALTIME_SLOWDOWN "\":0"
    // "}"
//   "\"MQTT_Interface_Priority\":{\"" D_MODULE_ENERGY_INTERFACE_CTR "\":1}" // Each interface will have ability to reduce its subclass mqtt "ifchanged" rate
// "}";


#endif



/* ** 

3D printed base, to hold an esp32 that replicates the whole hvac heating for the house from a hardware perspective.

Four single socket outputs, with pzem 100A CT monitoring them.

Hardware to implement:
- 4 PZEM 400A
- 4 16A relays (remove V2 relays, and make sure its just one going forward)
- 2 BME680
- 8 DB18S20 across 2 pins for pin developing
- 1 DHT22
- 4 sk6812 strip to show relays active
- another strip (and pin) with maybe 10 144/m pixels that should show the 8 DB18S20 sensors in a colour gradient
- maybe peizo buzzer for alarm
- Add OLED for debug
- Nextion 2.5inch could be used for manual control and feedback
- Add BH1750 for light level (since i2c is already in use)

Software to implement:
- Filesystem
  -- Heating schedules should be done, so the electric floor mat automatically turns on, this needs to include power cycle tool to stop the timeout
- Should enable integration with remote sensor to use the rooms temperature as the target temperature for oil heater




Blue (Upstairs Link) **************************************************************************
 * w/o           GND            GND           
 * o/w           5V             5V            
 * w/g           R_IH           21*           Immersion relay (5v IO)
 * bl/w          I2D            12*           DHT22?     
 * w/bl          I2C            13*           UNUSED   
 * g/w           3V3            3V3           
 * w/br                      
 * br/w          DSX            14*           ds18b20 water, 4k7 pulled high        (comes from blue by connector)   NOT WORKING, wiring too long, Needs rerouted
 * ** Green (Downstairs Sensors) **************************************************************************
 * w/o           GND            GND           Black
 * o/w           5V             5V            Red
 * w/g           1Wire          4*            DS18B20 water pipe sensors
 * bl/w          I2D            22*           BME in Dinning Room (Alternatively, DHT22 data pin)
 * w/bl          I2C            23*           BME in Dinning Room
 * g/w           3V3            3V3           White
 * w/br                                       NC
 * br/w          DHT22          25*           DHT22 (until BME is working)            
 * ** Orange (Nextion Display) **************************************************************************
 * w/o           GND            GND           
 * o/w           5V             5V            
 * w/g           NEO            27*             SK6812 4 pixels of rgbW
 * bl/w                                      
 * w/bl                                      
 * g/w           3V3            3V3           
 * w/br          RX2            17*              Nextion TX
 * br/w          TX2            16*              Nextion RX   -- "SERIAL TX" of Serial will always be dominant colour (brown) as its important to know the output pin
 * Twin          Switch
 * Twin          Switch
 * ** Power Screw Jacks **************************************************************************
 * 4 (Top)       12V
 * 3             5V
 * 2
 * 1 (Bottom)    GND
 * ** ADC Input **************************************************************************
 * 4 (Top)       LDR_US         33
 *               LDR_DS         32 
 *               LDR_WB         35
 * Extra Ethernet for LDRs hot glued onto the red led of the servos?
 * 
 * Upstairs Connectors
 * 3pin (DHT22)    - gnd,5v,dht_data 
 * 3pin (relay US) - gnd,5v,relay_ih
 * 3pin (water sensors) - gnd,3v3,ds18b20
 * 
 * Create seond device dad can swap in, only have basic relay control, no sensors, so it should be the most stable.
 * 
 **/
#ifdef DEVICE_TESTBED__HEATING_ENERGY_SYSTEM
  #define DEVICENAME_CTR          "heating"
  #define DEVICENAME_FRIENDLY_CTR "HVAC House Heating 2023#2"
  #define DEVICENAME_ROOMHINT_CTR "Hallway"
  #define MQTT_HOST   "192.168.1.70"
  
  #define ENABLE_FEATURE_WATCHDOG_TIMER
  
  

  #define DISABLE_SLEEP // loops per second less than 1hz // I need to make an "mqtt/alert" channel that lets me know this
  
  // #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES_SK6812_FOR_ROOM_SENSORS
  // #define USE_TEMPLATED_DEFAULT_LIGHTING_TEMPLATE_SK6812_FOR_ROOM_SENSORS__BOOT_STATE_OFF
  //   #define STRIP_SIZE_MAX 10

  //   #define USE_RGB_OUT_BASIC_SHOW_PALETTE


  #define USE_MODULE_CONTROLLER_HVAC
    #define HEATING_DEVICE_MAX 3
    

  #define USE_MODULE_SENSORS_INTERFACE
    #define USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
    #
  #define USE_MODULE_SENSORS_DS18X20
    #define DS18X20_MAX_SENSORS 20
      #define ENABLE_DEBUG_MQTT_CHANNEL_DB18X20      
  #define USE_MODULE_SENSORS_DHT
              
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_SENSORS_SWITCHES
  
    // #define SHOW_SPLASH
    // #define USE_MODULE_DISPLAYS_NEXTION
    // #define NEXTION_DEFAULT_PAGE_NUMBER 3   // I should add "p[c]" where c means current page, so I need to search and replace "p[c]" as "p[0]"


  // Actual
  #define GPIO_NAME_ZONE0_DOWNSTAIRS_RELAY  D_GPIO_REL1_INV_CTR
  #define GPIO_NAME_ZONE1_UPSTAIRS_RELAY    D_GPIO_REL2_INV_CTR
  #define GPIO_NAME_ZONE2_BOILER_RELAY      D_GPIO_REL3_INV_CTR
/**
 * 
 * 
 * Ethernet R
 * 
 * w/o  gnd
 * o/w     5v
 * w/g    left LED (upstairs)              D27
 * bl/w   centre Relay (downstairs)        D5
 * w/bl   centre LED (downstairs)          D33
 * g/w    left Relay (upstairs)            D18
 * w/br   right LED (boiler)               D26
 * br/w   right Relay (boiler)             D19
 * 
 * Ethernet N
 * 
 * w/o  gnd
 * o/w     5v
 * w/g    SK6812 D4
 * bl/w   NC
 * w/bl   NC
 * g/w    NC
 * w/br   RX2 of esp32 from nextion
 * br/w   TX2 of esp32 from nextion    
 * 

*/

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"5\":\""  GPIO_NAME_ZONE0_DOWNSTAIRS_RELAY  "\","
      "\"18\":\"" GPIO_NAME_ZONE1_UPSTAIRS_RELAY    "\","
      "\"19\":\"" GPIO_NAME_ZONE2_BOILER_RELAY      "\","
      #endif
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"33\":\""  D_GPIO_SWT1_INV_CTR  "\","
      "\"27\":\""  D_GPIO_SWT2_INV_CTR  "\","
      "\"26\":\""  D_GPIO_SWT3_INV_CTR  "\","
      #endif  
      #ifdef USE_MODULE_SENSORS_DHT
      "\"25\":\"" D_GPIO_DHT22_1_CTR   "\"," // DiningRoom 
      #endif
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"4\":\"" D_GPIO_RGB_DATA_CTR  "\","
      #endif 
      #ifdef USE_MODULE_DISPLAYS_NEXTION
      "\"17\":\"" D_GPIO_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_NEXTION_RX_CTR "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DS18X20
      "\"23\":\"" D_GPIO_DS18X20_1_CTR "\"," // DS_DB - 3 pin
      #endif    
      "\"2\":\""  D_GPIO_LED1_INV_CTR "\""   // builtin led
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  /**
   * @brief Drivers and Sensors for HVAC zones
   **/
  #define D_DEVICE_DRIVER_RELAY_0_NAME "Downstairs"
  #define D_DEVICE_DRIVER_RELAY_1_NAME "Upstairs"
  #define D_DEVICE_DRIVER_RELAY_2_NAME "Boiler"

  #define D_DEVICE_SENSOR_DHT_0_NAME "Downstairs_DHT"

  /**
   * @brief HVAC zones
   **/
  #define D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "Downstairs"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "Upstairs"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "Boiler"

  /** 
   * Pin_DS
   * */
  #define D_DEVICE_SENSOR_DB18S20_06_NAME        "Water21-Upstairs"
  #define D_DEVICE_SENSOR_DB18S20_06_ADDRESS     "[40,208,174,149,240,1,60,127]"

  #define D_DEVICE_SENSOR_DB18S20_07_NAME        "Water22-HotCross"
  #define D_DEVICE_SENSOR_DB18S20_07_ADDRESS     "[40,168,253,149,240,1,60,157]"

  #define D_DEVICE_SENSOR_DB18S20_08_NAME        "Water23-R/C"
  #define D_DEVICE_SENSOR_DB18S20_08_ADDRESS     "[40,12,164,2,0,0,0,72]"

  #define D_DEVICE_SENSOR_DB18S20_09_NAME        "Water24-Mains"
  #define D_DEVICE_SENSOR_DB18S20_09_ADDRESS     "[40,9,77,4,0,0,0,131]"

  #define D_DEVICE_SENSOR_DB18S20_10_NAME        "Water25-HotFromBoiler"
  #define D_DEVICE_SENSOR_DB18S20_10_ADDRESS     "[40,121,172,3,0,0,0,138]"

  #define D_DEVICE_SENSOR_DB18S20_11_NAME        "Water26-Downstairs"
  #define D_DEVICE_SENSOR_DB18S20_11_ADDRESS     "[40,205,241,149,240,1,60,148]"

  #define D_DEVICE_SENSOR_DB18S20_12_NAME        "Water27-R/H"
  #define D_DEVICE_SENSOR_DB18S20_12_ADDRESS     "[40,195,112,2,0,0,0,178]"

  #define D_DEVICE_SENSOR_DB18S20_13_NAME        "Water28-HotFromFurnace"
  #define D_DEVICE_SENSOR_DB18S20_13_ADDRESS     "[40,103,49,3,0,0,0,153]"

  #define D_DEVICE_SENSOR_DB18S20_14_NAME        "Water29-WaterBoiler"
  #define D_DEVICE_SENSOR_DB18S20_14_ADDRESS     "[40,183,162,149,240,1,60,24]"


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" D_DEVICE_DRIVER_RELAY_0_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_2_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
        "\"" D_DEVICE_DRIVER_RELAY_0_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_2_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_06_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_07_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_08_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_09_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_10_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_11_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_12_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_13_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_14_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DHT_CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_CTR "\":["
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "\""
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":{" 
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_06_NAME "\":" D_DEVICE_SENSOR_DB18S20_06_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_07_NAME "\":" D_DEVICE_SENSOR_DB18S20_07_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_08_NAME "\":" D_DEVICE_SENSOR_DB18S20_08_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_09_NAME "\":" D_DEVICE_SENSOR_DB18S20_09_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_10_NAME "\":" D_DEVICE_SENSOR_DB18S20_10_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_11_NAME "\":" D_DEVICE_SENSOR_DB18S20_11_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_12_NAME "\":" D_DEVICE_SENSOR_DB18S20_12_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_13_NAME "\":" D_DEVICE_SENSOR_DB18S20_13_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_14_NAME "\":" D_DEVICE_SENSOR_DB18S20_14_ADDRESS ""
      "}"  
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_06_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_06_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_0_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_2_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "}"
      "]"
    "},"        
    "\"" D_MODULE_NETWORK_MQTT_CTR "\":{"
      "\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120,"
      "\"" D_REALTIME_SLOWDOWN "\":0"
    "}"
  "}";
  
#endif



/**
 * @brief Integrating best camera code
 * 
 */
#ifdef DEVICE_TESTBED_CAMERA_OV26_MODULE
  #define DEVICENAME_CTR          "testbed_camera"
  #define DEVICENAME_FRIENDLY_CTR "Ensuite Sensor"
  #define DEVICENAME_ROOMHINT_CTR "Ensuite"
  #define MQTT_HOST   "192.168.1.70"
    
  #define ENABLE_FEATURE_WATCHDOG_TIMER
  
  

  #define DISABLE_SERIAL
  #define DISABLE_SERIAL0_CORE

  #define USE_MODULE_CORE_RULES
       
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_PIR
  #define USE_MODULE_SENSORS_BH1750

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"      
      #ifdef USE_MODULE_SENSORS_BME
      "\"3\":\"" D_GPIO_I2C_SCL_CTR   "\","
      "\"1\":\"" D_GPIO_I2C_SDA_CTR   "\"," //should be 27, missoldered, repair later
      #endif
      #ifdef USE_MODULE_SENSORS_PIR
      "\"16\":\"" D_GPIO_SWT1_CTR   "\""
      #endif
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  // #define SETTINGS_SENSORS_MQTT_IFCHANGED_PERIOD_SECONDS 1
  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Ensuite"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Ensuite"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_BH1750_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "]"
    "},"        
    "\"" D_MODULE_NETWORK_MQTT_CTR "\":{"
      "\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120,"
      "\"" D_REALTIME_SLOWDOWN "\":0"
    "}"  // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)

  "}";


  #define STRIP_SIZE_MAX 58
  #ifdef USE_MODULE_LIGHTS_INTERFACE
  #define USE_SK6812_METHOD_DEFAULT
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "HardwareType":"SK6812",
    "AnimationMode":"Effects",
    "ColourOrder":"grbw",
    "ColourPalette":"Rgbcct 01",
    "Effects": {
      "Function":0,
      "Intensity":50
    },
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "SegColour": {
      "Hue": 18,
      "Sat": 100,
      "SubType":3
    },
    "BrightnessRGB":0,
    "BrightnessCCT":0
  }
  )=====";
  #endif // USE_MODULE_LIGHTS_INTERFACE
  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"// for PIR to follow
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






/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTBED_MAVLINK_DECODE_R
  #define DEVICENAME_CTR          "testbed_mavlink_decoder"
  #define DEVICENAME_FRIENDLY_CTR "Testbed MAVLink Decoder"
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define MQTT_HOST   "192.168.1.70"

  // 
  // 

  // #define ENABLE_DEBUG_FUNCTION_NAMES
  // #define ENABLE_DEBUG_SHOW_ADVANCED_LOGS_FOR_STARTUP_UPSECONDS 20

  












  #define USE_MODULE__DRIVERS_MAVLINK_DECODER
    #define USE_FEATURE_SEARCH_FOR_UNHANDLED_MAVLINK_MESSAGES_ON_ALLOWEDLIST
    #define USE_DEVFEATURE_MAVLINK_DECOVER_SERIAL_NUMBER 2

  #define USE_MODULE_CORE_SERIAL_UART
    #define ENABLE_HARDWARE_UART_2
    #define HARDWARE_UART_2_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST


  /**
   * @brief WiFi MQTT
   * 
   */
  #define USE_MODULE_NETWORK_WIFI
  #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  

  /**
   * @brief Cellular MQTT
   * 
   */  
  // #define DISABLE_NETWORK_WIFI
  // #define USE_MODULE_NETWORK_CELLULAR
  // #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  // #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
  // #define USE_MODULE_SENSORS_GPS_MODEM
  // #define USE_MODULE_SENSORS_INTERFACE
  // #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
  // #define MQTT_PORT 51883 //external mqtt broker on TOWER 


  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SH1106
  //   #define SHOW_SPLASH

  // #define USE_MODULE_CORE_SERIAL_UART


  //   #define USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"
      #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
      "\"17\":\"" D_GPIO_HWSERIAL2_TX_CTR   "\","
      "\"16\":\"" D_GPIO_HWSERIAL2_RX_CTR   "\""   
      #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER   
      #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
      "\"22\":\"" D_GPIO_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_I2C_SDA_CTR   "\""   
      #endif // USE_MODULE_DISPLAYS_OLED_SH1106   
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularTracker01"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE__SENSORS_GPS_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE__SENSORS_BATTERY_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "]"
    "},"   
    "\"" D_DISPLAY "\":{"
      "\"" "DisplayRows" "\":8,"
      "\"" "DisplayCols" "\":[21,2],"
      "\"" "DisplaySize" "\":1"
    "},"      
    "\"" D_MODULE_NETWORK_MQTT_CTR "\":{"
      "\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120,"
      "\"" D_REALTIME_SLOWDOWN "\":0"
    "},"
    "\"MQTT\":{\"RetrySecs\":10}"
  "}";

#endif // DEVICE_TESTBED_MAVLINK_DECODER




/**
 * @brief 
 * 
 * 
// Add function to "settime_update" ie with GPS ticks. When it happens, reset the 
// update millis so it does not double update, same for uptime
 * 
 * 
 * If LTE connects, then set the mqtt retry counter to immediate
 * 
 */







/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTBED_MAVLINK_TELEMETRY_WIFI_01
  #define DEVICENAME_CTR          "testbed_mavlink_telemetry_01"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Version 2"
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define MQTT_HOST   "192.168.1.70"


  #define ENABLE_DEBUG_FUNCTION_NAMES
  #define ENABLE_DEBUG_SHOW_ADVANCED_LOGS_FOR_STARTUP_UPSECONDS 20

  
  // #define ENABLE_DEVFEATURE_DISABLE_MQTT_FREQUENCY_REDUNCTION_RATE

  // #define USE_MODULE_NETWORK_MQTT
  // #define USE_MODULE_NETWORK_MQTT_MULTIPLE

  // #define USE_SSIDS_NONE_DEBUGGING
  // #define DISABLE_DEVFEATURE_NETWORK_WIFI

  // #define DISABLE_NETWORK

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE

  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define USE_MODULE__DRIVERS_MAVLINK_DECODER
  #define USE_MODULE__DRIVERS_MAVLINK_TELEMETRY_WIFI

  #define USE_DEVFEATURE_DEFINED_SERIAL2

  #define ENABLE_FEATURE_MAVLINK_CONVERT_MQTT_DATA_VALUES


  /**
   * @brief WiFi MQTT
   * 
   */
  #define USE_MODULE_NETWORK_WIFI
  #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  


  // // 
  // // 


  // #define ENABLE_DEBUG_FUNCTION_NAMES
  // #define ENABLE_DEBUG_SHOW_ADVANCED_LOGS_FOR_STARTUP_UPSECONDS 20

  
  // // #define ENABLE_DEVFEATURE_DISABLE_MQTT_FREQUENCY_REDUNCTION_RATE

  // // #define USE_MODULE_NETWORK_MQTT
  // // #define USE_MODULE_NETWORK_MQTT_MULTIPLE

  // // #define USE_SSIDS_NONE_DEBUGGING
  // // #define DISABLE_DEVFEATURE_NETWORK_WIFI

  // // #define DISABLE_NETWORK

  // // #define DISABLE_SERIAL
  // // #define DISABLE_SERIAL0_CORE

  // // #define ENABLE_ADVANCED_DEBUGGING
  // // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define USE_MODULE__DRIVERS_MAVLINK_DECODER

  // #define MQTT_PORT 51883 //external mqtt broker on TOWER 

  #define ENABLE_DEVFEATURE_SIM7000G_INIT_SKIP_MODEM_RESTART


  // #define USE_DEVFEATURE_DEFINED_SERIAL2

 #define MQTT_PORT 51884 // Temporary exposed primry broker : STABLE
//  #define MQTT_PORT 51883 //external mqtt broker on TOWER  : Unstable

  /**
   * @brief WiFi MQTT
   * 
   */
  #define USE_MODULE_NETWORK_WIFI
  #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  

  /**
   * @brief Cellular MQTT
   * 
   */  
  // #define DISABLE_NETWORK_WIFI
  // #define USE_MODULE_NETWORK_CELLULAR
  // #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  // #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
  // #define USE_MODULE_SENSORS_GPS_MODEM
  // #define USE_MODULE_SENSORS_INTERFACE
  // #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR

  // #define USE_MODULE_SENSORS_BATTERY_MODEM

  // #define ENABLE_DEVFEATURE_DISABLE_MQTT_FREQUENCY_REDUNCTION_RATE

  // // #define USE_MODULE_NETWORK_MQTT
  // // #define USE_MODULE_NETWORK_MQTT_MULTIPLE

  // // #define USE_SSIDS_NONE_DEBUGGING
  // // #define DISABLE_DEVFEATURE_NETWORK_WIFI

  // // #define DISABLE_NETWORK

  // // #define DISABLE_SERIAL
  // // #define DISABLE_SERIAL0_CORE

  // // #define ENABLE_ADVANCED_DEBUGGING
  // // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define MQTT_PORT 51884 //external mqtt broker on TOWER 

  // /**
  //  * @brief WiFi MQTT
  //  * 
  //  */
  // // #define USE_MODULE_NETWORK_WIFI
  // #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  // // 

  // /**
  //  * @brief Cellular MQTT
  //  * 
  //  */  
  // #define DISABLE_NETWORK_WIFI
  // #define USE_MODULE_NETWORK_CELLULAR
  // #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  // #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
  // #define USE_MODULE_SENSORS_GPS_MODEM
  // #define USE_MODULE_SENSORS_INTERFACE
  // #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR

  // #define USE_MODULE_SENSORS_BATTERY_MODEM

  // #define ENABLE_DEVFEATURE_DISABLE_MQTT_FREQUENCY_REDUNCTION_RATE

  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SH1106
  //   #define SHOW_SPLASH

  //   // #define USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX

  // #define USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_MAVLINK_BLACK_BOX_OLED

//  6P connector

//  Green VIN (5v)
// Orange 18
// Yellow 19
// White 25
// Red 26
// Black gnd


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"
      #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
      "\"22\":\"" D_GPIO_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_I2C_SDA_CTR   "\""   
      #endif // USE_MODULE_DISPLAYS_OLED_SH1106   
      "\"12\":\"" D_GPIO_LED1_INV_CTR "\","
      "\"35\":\"" D_GPIO_ADC1_CH7_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularLocator01"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE__SENSORS_GPS_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE__SENSORS_BATTERY_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "]"
    "},"   


    "\"" D_DISPLAY "\":{"
      "\"" "DisplayRows" "\":8,"
      "\"" "DisplayCols" "\":[21,2],"
      "\"" "DisplaySize" "\":1"
    "},"        
    "\"" D_MODULE_NETWORK_MQTT_CTR "\":{"
      "\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120,"
      "\"" D_REALTIME_SLOWDOWN "\":0"
    "}," // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
    "\"MQTT\":{\"RetrySecs\":10}"
  "}";


  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  // "{"
  //   "\"MQTT\":{\"RetrySecs\":10}"
  // "}";
  /*
  https://infocenter.nordicsemi.com/index.jsp?topic=%2Fref_at_commands%2FREF%2Fat_commands%2Ftext_mode%2Fcnmi_read.html
  
  {
  "GPS_Enable": 0,
  "SMS_Enable": 1,
  "ATCommands": [
    "AT",
    "AT+CMGF=1",
    "AT+CMTI?",
    "AT+CNMI= 2,2,0,0,0"
  ]
}
  
  
  
  
  
  
  
  
  
  
  
  
  */




#endif // DEVICE_TESTBED_ESP32_LILYGO_SIM7000G




/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTBED_ESP32_BUZZER_TONES
  #define DEVICENAME_CTR          "testbed_buzzer_tones"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Version 2"
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define MQTT_HOST   "192.168.1.70"

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  
  

  #define USE_MODULE__DRIVERS_BUZZER_TONES

  #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"23\":\"" D_GPIO_BUZZER_CTR   "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularTracker02"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE__SENSORS_GPS_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE__SENSORS_BATTERY_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "]"
    "},"   


    "\"" D_DISPLAY "\":{"
      "\"" "DisplayRows" "\":8,"
      "\"" "DisplayCols" "\":[21,2],"
      "\"" "DisplaySize" "\":1"
    "},"    

    "\"BuzzerMode\":{\"FreqMode\":1},"
    
    "\"" D_MODULE_NETWORK_MQTT_CTR "\":{"
      "\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120,"
      "\"" D_REALTIME_SLOWDOWN "\":0"
    "},"  // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
    "\"MQTT\":{\"RetrySecs\":10}"
  "}";


  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  // "{"
  //   "\"MQTT\":{\"RetrySecs\":10}"
  // "}";
  /*
  https://infocenter.nordicsemi.com/index.jsp?topic=%2Fref_at_commands%2FREF%2Fat_commands%2Ftext_mode%2Fcnmi_read.html
  
  {
  "GPS_Enable": 0,
  "SMS_Enable": 1,
  "ATCommands": [
    "AT",
    "AT+CMGF=1",
    "AT+CMTI?",
    "AT+CNMI= 2,2,0,0,0"
  ]
}
  
  
  
  
  
  
  
  
  
  
  
  
  */




#endif // DEVICE_TESTBED_ESP32_LILYGO_SIM7000G










#ifdef DEVICE_TESTBED_RTC_FASTBOOT
  #define DEVICENAME_CTR          "testbed_rtc_fastboot"   APPEND_ESP_TYPE_MQTT_STRING                                    // Change: The unique mqtt topic, however, mqtt client names are appended with mac address, so for basic testing (ie of templates) it is not essential this be changed
  #define DEVICENAME_FRIENDLY_CTR "Testbed RTC Fastboot"   APPEND_ESP_TYPE_NAME_STRING                                 // Change: You may change this, but it is not important to do so (more important when webui is functioning)
  
  #define STRIP_SIZE_MAX 55                                                                           // Change: Set *total* length of string, segment0 will default to this length
  #define PIN_NAME_STRING_ESP8266_DEFAULT   "RX"                                                      // Change: Set to the pin you want, esp8266 this will default to this anyway
  #define PIN_NAME_STRING_ESP32_DEFAULT     "23"                                                      //         Set to the pin you want, any output pin should work

  // #define ENABLE_DEVFEATURE_RTC_FASTBOOT_V2
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    // "\"" D_GPIOC "\":{"
    // #ifdef ESP8266 
    //   "\"" PIN_NAME_STRING_ESP8266_DEFAULT "\":\"" D_GPIO_RGB_DATA_CTR  "\""
    // #else
    //   "\"" PIN_NAME_STRING_ESP32_DEFAULT "\":\"" D_GPIO_RGB_DATA_CTR  "\""
    // #endif
    // "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
    
#endif


#ifdef DEVICE_TESTBED_BUTTON_MULTIPRESS
  #define DEVICENAME_CTR          "testbed_button_longpress"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Button Longpress"

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BUTTONS
  #define ENABLE_DEVFEATURE_BUTTONS_SEND_EVENT_MESSAGES

  #define MAX_KEYS 1                 // Max number of keys or buttons

  #define USE_MODULE_CORE_RULES


  #define ENABLE_DEVFEATURE_PHASEOUT_CLEARING_EVENT

  #define ENABLE_DEVFEATURE_BUTTON_SET_FLAG_BUTTON_SINGLE 0 // allow multipress = false

  #define ENABLE_DEBUG_DEV_BUTTONS
  
  #define USE_MODULE_DRIVERS_RELAY
  #define MAX_RELAYS 1
  #define USE_MODULE_DRIVERS_INTERFACE

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"D7\":\""  D_GPIO_KEY1_CTR  "\","
      "\"4\":\""   D_GPIO_REL1_CTR  "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Builtin Button as toggle
    "\"Rule0\":{" //switch example
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_BUTTONS_FRIENDLY_CTR "\","    //sensor
        "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0," // eg Switch0, Switch1, Button#, Motion, # (number for index)  
        "\"State\":0" //eg. On, Off, Toggle, Any, LongPress, ShortPress, RisingEdge, FallingEdge, Started, Ended, TimerOnStarted
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
        "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0," //number, name, or all
        "\"State\":2" // toggle
      "}"
    "},"
    // Optional external button on RX pin
    "\"Rule1\":{" //switch example
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_BUTTONS_FRIENDLY_CTR "\","    //sensor
        "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":1," // eg Switch0, Switch1, Button#, Motion, # (number for index)  
        "\"State\":0" //eg. On, Off, Toggle, Any, LongPress, ShortPress, RisingEdge, FallingEdge, Started, Ended, TimerOnStarted
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
        "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0," //number, name, or all
        "\"State\":2" // toggle
      "}"
    "},"
    // Optional external button on RX pin
    "\"Rule2\":{" //switch example
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_BUTTONS_FRIENDLY_CTR "\","    //sensor
        "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":1," // eg Switch0, Switch1, Button#, Motion, # (number for index)  
        "\"State\":0" //eg. On, Off, Toggle, Any, LongPress, ShortPress, RisingEdge, FallingEdge, Started, Ended, TimerOnStarted
        "\"Data\":[0,2]" // [state,button_hold]
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
        "\"Function\":\"" D_TASK_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOn\\\":3}}\""
      "}"
    "}"
  "}";



#endif


/**
 * esp32 version -- SIM800L
 * */
#ifdef DEVICE_TESTBED_RCSWITCH_NORMAL
  #define DEVICENAME_CTR          "testbed_rcswitch_normal"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Fona 800L"

  #define USE_MODULE_DRIVERS_INTERFACE

  #define USE_MODULE_DRIVERS_RF433_CODES

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RF433_CODES
      "\"23\":\"" D_GPIO__RF_433MHZ_RX__CTR   "\","
      #endif  
      "\"2\":\"" D_GPIO_LED1_CTR  "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
#endif

#ifdef DEVICE_testbed_rcs_ext
  #define DEVICENAME_CTR          "testbed_rcs_ext"
  #define DEVICENAME_FRIENDLY_CTR "Testbed 433MHz RCSwitch Extended"
  #define MQTT_HOST   "192.168.1.70"

  // #define USE_MODULE_DRIVERS_INTERFACE

  #define USE_MODULE_DRIVERS_RF433_CODES
    #define ENABLE_DEVFETURE_DISABLE_EXTENDED_FEATURES_START

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RF433_CODES
      "\"23\":\"" D_GPIO__RF_433MHZ_RX__CTR   "\","
      #endif  
      "\"2\":\"" D_GPIO_LED1_CTR  "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
#endif


#ifdef DEVICE_TESTBED_FASTBOOT_ESP8266
  #define DEVICENAME_CTR          "testbed_fastboot_esp8266"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Fona 800L"

  
  
  // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_CREATE_SSID_AP
  // #define ENABLE_DEVFEATURE_FASTBOOT_HTTP_FALLBACK_DEFAULT_SSID
  // #define DEBUG_FASTBOOT
  // #define ENABLE_DEVFEATURE___CAUTION_CAUTION__FORCE_CRASH_FASTBOOT_TESTING

  /**
   * @brief wEBSERVER ISSUES
   * 
   */


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_CONTROLLER__LOUVOLITE_HUB
      // "\"23\":\"" D_GPIO__RF_433MHZ_RX__CTR   "\","
      "\"22\":\"" D_GPIO__RF_433MHZ_TX__CTR   "\","
      #endif  
      "\"2\":\"" D_GPIO_LED1_CTR  "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
#endif



/**
 * @brief Getting crashreports to work so I can now start tracking crashes.
 * Start saving the mqtt json response with openhab string item, therefore I can easily go back and track issues.
 * A rule to request item string info from persistance will allow me to recall the data (print into logs?)
 * 
 */
#ifdef DEVICE_TESTBED_CRASHREPORT
  #define DEVICENAME_CTR          "testbed_crashreport" APPEND_ESP_TYPE_MQTT_STRING
  #define DEVICENAME_FRIENDLY_CTR "Testbed CrashReport" APPEND_ESP_TYPE_NAME_STRING

  // 
  // 
  // // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_CREATE_SSID_AP
  // // #define ENABLE_DEVFEATURE_FASTBOOT_HTTP_FALLBACK_DEFAULT_SSID
  // // #define DEBUG_FASTBOOT
  // // #define ENABLE_DEVFEATURE___CAUTION_CAUTION__FORCE_CRASH_FASTBOOT_TESTING



  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"LBI\":\"" D_GPIO_LED1_CTR  "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
#endif


#ifdef DEVICE_TESTBED_SR04
  #define DEVICENAME_CTR          "testbed_sr04"
  #define DEVICENAME_FRIENDLY_CTR "Oil Tank"
  #define DEVICENAME_ROOMHINT_CTR "Outside"
  #define MQTT_HOST   "192.168.1.70"

  // add db18 dropping from the waterproof box to know extra temp, perhaps add two for backup? (independant pin from tank sensors)

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  
  

  #define USE_MODULE_SENSORS_INTERFACE
    #
  #define USE_MODULE_SENSORS_SR04
    // #define USE_AMBIENT_TEMP_SENSOR_FOR_SPEEDOFSOUND
    // #define ENABLE_DEVFEATURE_ULTRASONIC_DURATION_RAW_THRESHOLD_CHECK
    #define ENABLE_DEVFEATURE_SR04_FILTERING_EMA
    #define ENABLE_DEVFEATURE_SR04_FILTERING_DEMA
    // #define ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"      
      #ifdef USE_MODULE_SENSORS_SR04
      "\"21\":\"" D_GPIO_SR04_ECHO_CTR   "\","
      "\"22\":\"" D_GPIO_SR04_TRIG_CTR  "\","  
      #endif
      "\"2\":\"" D_GPIO_LED1_INV_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  

  #define D_DEVICE_SENSOR_DB18S20_01_NAME "AmbientTank"
  #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS "[40,226,218,149,240,1,60,142]"
  #define D_DEVICE_SENSOR_DB18S20_02_NAME "OutsideGarage"
  #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS "[40,255,169,120,53,22,4,240]"

  #define D_DEVICE_SR04_FRIENDLY_NAME_LONG "LevelSensor"
  
  #define D_DEVICE_BUTTON_1_CTR "FurnaceActive"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SR04_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SR04_FRIENDLY_NAME_LONG "\""
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":{" 
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ""
      "}"  
    "}"
  "}";

#endif


#ifdef DEVICE_BUCKET_WATER_LEVEL //get capacitive one for each chritmas!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  #define DEVICENAME_CTR          "bucketwatersensor"
  #define DEVICENAME_FRIENDLY_CTR "RGB Notifications 01"
  
  #define USE_MODULE_CONTROLLER_BUCKET_WATER_LEVEL
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"23\":\"" D_GPIO_REL1_INV_CTR  "\","

      "\"32\":\"" D_GPIO_ADC1_CH4_CTR   "\","
      "\"35\":\"" D_GPIO_ADC1_CH7_CTR   "\""

    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";



  #define D_DEVICE_ADC_INPUT_NAME_LONG "WaterSensor"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_CONTROLLER_BUCKET_WATER_LEVEL_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_ADC_INPUT_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif





#ifdef DEVICE_TESTBED__SETTINGS_STORING
  #define DEVICENAME_CTR          "testbed_settings_storage"
  #define DEVICENAME_FRIENDLY_CTR "Testbed 433MHz RCSwitch Extended"
  #define MQTT_HOST   "192.168.1.70"
  #define DEVICENAME_ROOMHINT_CTR "Example"

  /***********************************
   * SECTION: Core Configs
  ************************************/    

  // #define ENABLE_DEVFEATURE__SETTINGS_STORAGE
  //   #define ENABLE_DEVFEATURE__SETTINGS_STORAGE__SAVE_LOAD_STRUCT
  //   #define ENABLE_DEVFEATURE__SETTINGS_STORAGE__SEND_DEBUG_MQTT_MESSAGES
  //   #define ENABLE_DEVFEATURE__SETTINGS_STORAGE_ESP_SUPPORT

  
  // #define USE_MODULE_CORE_FILESYSTEM
  // 
  // 


  /***********************************
   * SECTION: Network Configs
  ************************************/    

  



  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RF433_CODES
      "\"22\":\"" D_GPIO__RF_433MHZ_TX__CTR   "\","
      #endif  
      #ifdef USE_MODULE_DRIVERS_RF433_CODES
      "\"23\":\"" D_GPIO__RF_433MHZ_RX__CTR   "\","
      #endif  
      "\"2\":\"" D_GPIO_LED1_CTR  "\""
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  
#endif







#ifdef DEVICE_DEVELOPMENT__MOISTURE_SENSOR
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

  // #define DEVICENAME_CTR          "treadmill_power_monitor"
  // #define DEVICENAME_FRIENDLY_CTR "HVAC Desk DevPlatform"
  // #define DEVICENAME_ROOMHINT_CTR "Bedroom"
  // #define MQTT_HOST   "192.168.1.70" // primary
  //   #define MQTT_PORT     1883
    
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



  /***********************************
   * SECTION: Network Configs
  ************************************/    

  

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  #define MOISTURE_DIGITAL_PIN 23

  #define USE_MODULE_SENSORS_MOISTURE

  // #define USE_MODULE_SENSORS_INTERFACE  
  // #define USE_MODULE_SENSORS_SWITCHES

  /***********************************
   * SECTION: Display Configs
  ************************************/  

  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SH1106
  //   #define SHOW_SPLASH

  // Add this, 4 rows so show the power, current, voltage and energy

  /***********************************
   * SECTION: Driver Configs
  ************************************/  

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  /***********************************
   * SECTION: Energy Configs
  ************************************/  

  // #define USE_MODULE_ENERGY_INTERFACE
  // #define USE_MODULE_ENERGY_PZEM004T_V3
  //   #define ENABLE_DEVFEATURE_REDUCE_SUBORDINATE_MQTT_REPORTING_ENERGY // If energy_interface is primary reporting, reduce pzem to slower (debug only)
  // #define MAX_ENERGY_SENSORS 1
  // #define MAX_PZEM004T_DEVICES 1

  /***********************************
   * SECTION: Controller Configs
  ************************************/  

//  #define USE_MODULE_CONTROLLER__ENERGY_OLED
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
      "\"32\":\""  D_GPIO_MOISTURE_ANALOG_CTR "\"" 
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  
  #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "1"

  #define D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "MainFeed"
  
  
  #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR   D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_ENERGY "\":{"
        "\"DeviceCount\":1"    
    "},"
    "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":{"
        "\"DeviceCount\":1"    
    "},"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":["
        "\"" D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "\""
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":[" 
        D_DEVICE_SENSOR_PZEM004T_0_ADDRESS ""
      "]"  
    "},"    
    "\"" D_MODULE_NETWORK_MQTT_CTR "\":{"
      "\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120,"
      "\"" D_REALTIME_SLOWDOWN "\":0"
    "}"
  "}";



  
#endif


#ifdef DEVICE_INTERMEDIATE_FLASH__ESP8266_MINIMAL_OTA

  #ifndef ESP8266
  #define ESP8266
  #endif

  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR "intermediate_flash__esp8266_minimal_OTA"
  #endif

  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif

  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif

  #ifndef DEVICENAME_ROOMHINT_CTR
  #define DEVICENAME_ROOMHINT_CTR "recovery"
  #endif

  /***********************************
   * SECTION: Purpose
   ************************************
   *
   * Minimal ESP8266 OTA bridge firmware.
   *
   * Intended use:
   *   1. OTA this small image onto an ESP8266 device that no longer has enough
   *      free sketch space to accept the full firmware.
   *   2. Device reboots into minimal AP+STA recovery updater.
   *   3. OTA or HTTP-upload the full firmware.
   *
   * This build should remain intentionally small.
   ************************************/

  /***********************************
   * SECTION: Settings
   ************************************/

  #define SETTINGS_HOLDER 1239

  /***********************************
   * SECTION: Recovery / OTA
   ************************************/

  #define ENABLE_FEATURE_SYSTEM__SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_HTTP_FALLBACK_DEFAULT_SSID
  #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

  /*
   * Keep only the basic HTTP update path.
   * Do not enable full WebUI.
   */
  // #define FIRMWARE_DEFAULT__INCLUDE_WEBSERVER_BASIC

  /***********************************
   * SECTION: Logging
   ************************************/

  /*
   * Keep serial logging available for recovery diagnostics.
   * Avoid heavy debug features.
   */
  #define ENABLE_DEBUGFEATURE_TIME__SHOW_UPTIME_EVERY_SECOND

  /***********************************
   * SECTION: Drivers
   ************************************/

  /*
   * Optional status LED only.
   * Remove this too if you need the absolute smallest binary.
   */
  #define USE_MODULE_DRIVERS_LEDS

  /***********************************
   * SECTION: Explicitly avoid heavy subsystems
   ************************************/

  /*
   * Do not enable:
   * - filesystem,
   * - MQTT,
   * - rules,
   * - sensors,
   * - lighting,
   * - displays,
   * - crash recorder,
   * - full WebUI,
   * - template export.
   */

  /***********************************
   * SECTION: Module/GPIO template
   ************************************/

  #define USE_MODULE_TEMPLATE

  DEFINE_PGM_CTR(MODULE_TEMPLATE)
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"D3\":\"" D_GPIO_KEY1_INV_CTR "\","
      "\"D4\":\"" D_GPIO_LED1_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  /***********************************
   * SECTION: Function template
   ************************************/

  /*
   * Keep this empty/minimal.
   * MQTT is not needed in the bridge image.
   */
  #define USE_FUNCTION_TEMPLATE

  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
  "}";

#endif // DEVICE_INTERMEDIATE_FLASH__ESP8266_MINIMAL_OTA





#endif // _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_ACTIVEDEVELOPMENT_H
