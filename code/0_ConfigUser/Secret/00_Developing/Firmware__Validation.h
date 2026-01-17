#ifndef _CONFIG_USER__SECRET__FIRMWARE__VALIDATION_H
#define _CONFIG_USER__SECRET__FIRMWARE__VALIDATION_H

/*********************************************************************************************\
 * VALIDATION
 * These are complex builds, which MUST ALL BE USED (leave none old) that will be compiled to for validation of succesful build/ifdef patterns
 * Although the intent is never to be used for hardware, using the ini esp32/esp8266/nodemcu etc can be used to test.
 * This will be automated later, and perhaps include an actual physical hardware "uptime" stable callback test.
\*********************************************************************************************/

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"

//--------------------------------[Enable Device]-------------------------------------

/**
 * DEVICE_TYPE CORE: Any testbeds related to core system
 * */
// #define DEVICE_TESTBED__SETTINGS_STORING
// #define DEVICE_TESTBED__FILESYSTEM

// #define DEVICE_VALIDATION__ESP8266__MINIMAL__NODEMCU

// #define DEVICE_VALIDATION__ESP32S3__MINIMAL_SEEDSTUDIO_XIAO_CAMERA

// #define DEVICE_VALIDATION__ESP32__CAMERA_WROOVER_E

// #define DEVICE_VALIDATION__ESP32__ETHERNET


// #define DEVICE_VALIDATION__ESP32__LIGHTING__1D_SINGLE_BUS



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
// #define DEVICE_VALIDATION__ESP32__LIGHTING__1D_SINGLE_BUS

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
// #define DEVICE_TESTBED_OLED_SH1106


//-----------------[User Defined Devices == USE_BUILD_TYPE_ENERGY == Any Energy Monitoring Firmware]-------------------------------------


#ifdef DEVICE_VALIDATION__ESP32__MINIMAL_DEVKIT
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "validation"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "validation"

  /***********************************
  * SECTION: Enable Grouped
  ************************************/  

  /***********************************
   * SECTION: Lighting Configs
  ************************************/   
  
      
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

#endif 



#ifdef DEVICE_VALIDATION__ESP32__ETHERNET
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "validation"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "validation"

  /***********************************
  * SECTION: Enable Grouped
  ************************************/  

  #define USE_MODULE_NETWORK_ETHERNET

  /***********************************
   * SECTION: Lighting Configs
  ************************************/   
  
      
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

#endif 


/**************************************************************************************************************************************************************************************************
***************************************************************************************************************************************************************************************************
***************************************************************************************************************************************************************************************************
*  DEVICE_TYPE LIGHTING: Any testbeds related to lighting 
***************************************************************************************************************************************************************************************************
***************************************************************************************************************************************************************************************************
***************************************************************************************************************************************************************************************************/

#ifdef DEVICE_VALIDATION__ESP32__LIGHTING__1D_SINGLE_BUS
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "validation"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "validation"


                  //  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE
                  //  #define  ENABLE_FEATURE_BUILD__11JAN25_WIFI_AND_WEBUI
                  // #define  STRIP_DEFAULT_LENGTH  100
                  // #define ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__SUN_POSITIONS


  /***********************************
  * SECTION: Enable Grouped
  ************************************/  

  /***********************************
   * SECTION: Lighting Configs
  ************************************/   

  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE)
  "{"
    "\"BusConfig\":[{"
      "\"Pin\":2,"
      "\"ColourOrder\":\"RGB\","
      "\"BusType\":\"WS2812_RGB\","
      "\"Start\":0,"
      "\"Length\":" STR(STRIP_DEFAULT_LENGTH)
    "}],"
    "\"Segment0\":{"
      "\"PixelRange\":[0," STR(STRIP_DEFAULT_LENGTH) "],"
      "\"ColourPalette\":\"Snowy 02\","
      "\"Effects\":{"
        "\"Function\":\"Static\","
        "\"Speed\":127,"
        "\"Intensity\":127,"
        "\"Grouping\":1,"
        "\"RateMs\":25"
      "},"
      "\"BrightnessRGB\":100,"
      "\"Preset\":{\"Load\":1}"
    "},"
    "\"BrightnessRGB\":100"
  "}";
  
      
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"   
      "\"12\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"26\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"32\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"14\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"27\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"25\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      "\"33\":\"" D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR   "\","
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
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
 */
#ifdef DEVICE_VALIDATION__ESP32C3__MINIMAL_SUPERMINI
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "validation"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "validation"

#define USE_MODULE_TEMPLATE
DEFINE_PGM_CTR(MODULE_TEMPLATE) 
"{"
  "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
  "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  "\"" D_GPIO_NUMBER "\":{"          
    #if defined(USE_MODULE_SENSORS__TOF_VL53L0X) || defined(USE_MODULE_SENSORS__TOF_VL53L1X) || defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750) || defined(USE_MODULE_ENERGY_INA219) || defined(USE_MODULE_DISPLAYS_OLED_SH1106)
    "\"10\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\"," //instead of 9
    "\"9\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","    
    #endif
    #ifdef USE_MODULE_SENSORS_PIR
    "\"4\":\""  D_GPIO_FUNCTION_PIR_1_CTR "\","
    "\"7\":\""  D_GPIO_FUNCTION_PIR_2_CTR "\","
    #endif
    #ifdef USE_MODULE_SENSORS_SR04
    "\"4\":\"" D_GPIO_FUNCTION_SR04_ECHO_CTR   "\","
    "\"2\":\"" D_GPIO_FUNCTION_SR04_TRIG_CTR  "\","  
    #endif 
    #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ
    "\"35\":\""  D_GPIO_FUNCTION_PIR_2_INV_CTR "\","
    #endif
    #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__PIR_SMALL
    "\"5\":\""  D_GPIO_FUNCTION_PIR_3_INV_CTR "\","
    #endif
    #ifdef USE_MODULE_SENSORS__RADAR_HLK_LD2410
    "\"6\":\""  D_GPIO_FUNCTION__HLK_LD2410_TX__CTR "\","
    "\"5\":\""  D_GPIO_FUNCTION__HLK_LD2410_RX__CTR "\""
    #endif
    #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__PIR_LARGE
    "\"34\":\""  D_GPIO_FUNCTION_PIR_1_INV_CTR "\""
    #endif
  "},"
  "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
  "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
"}";


#endif



  
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
#ifdef DEVICE_VALIDATION__ESP8266__MINIMAL__NODEMCU
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "validation"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "validation"

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
 * Although SeedStudio XIAO includes camera, this is limit to building on esp32S3 chip
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
#ifdef DEVICE_VALIDATION__ESP32S3__MINIMAL_SEEDSTUDIO_XIAO_CAMERA
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "validation"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "validation"

  #define FIRMWARE_DEFAULT__INCLUDE_WEBSERVER_FULL
  #define ENABLE_DEBUG_LINE_HERE3
  #define ENABLE_DEBUG_MANUAL_DELAYS
  #define ENABLE_DEBUGFEATURE_LOGS__FORCE_FLUSH_ON_TRANSMIT
  #define DISABLE_LEDC_DEFINE_TO_PATCH_BUILD_ERROR_JUNE25
  #define ENABLE_IMMEDIATE_BOOT_SERIAL_PRINT_WHILE
  #define ENABLE_DEBUGFEATURE_TIME__SHOW_UPTIME_EVERY_SECOND
  #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  #define ENABLE_DEVFEATURE_ANALOG_WRITE_EMULATION_VERSION2
  
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
  #define CAMERA_MODEL_XIAO_ESP32S3_SENSE




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



#endif



//--------------------------------[Enable Device]-------------------------------------

#ifdef DEVICE_VALIDATION__ESP32__CAMERA_WROOVER_E
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
  //  
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
  #define CAMERA_MODEL_WROVER_KIT
  #define WEBCAM_DEV_DEBUG

  

#define USE_MODULE_CORE_FILESYSTEM


#define DEBUG_DRIVERS__CAMERA_2025
#define ENABLE_RTSPSERVER

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


#define USE_FUNCTION_TEMPLATE
DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
"{"
  "\"" D_MODULE_DRIVERS__CAMERA_CTR "\":{"
    "\"Mirror\":1,"
      "\"Flip\":1,"
      "\"AWB\":0,"
      "\"Resolution\":9" //12 max
    "}"


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
"}";

#endif







#endif // _CONFIG_USER__SECRET__FIRMWARE__VALIDATION_H

