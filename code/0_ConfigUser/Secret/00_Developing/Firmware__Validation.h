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


                   #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE
                   #define  ENABLE_FEATURE_BUILD__10JAN25_SOFTAP_WEBUI
                  #define  STRIP_DEFAULT_LENGTH  100
                  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS

#define TEST_ERROR_MAKE

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

  #define ENABLE_FEATURE_BUILD__10JAN25_SOFTAP_WEBUI


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



#endif // _CONFIG_USER__SECRET__FIRMWARE__VALIDATION_H

