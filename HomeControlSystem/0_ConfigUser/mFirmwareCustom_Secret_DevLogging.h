#ifndef MSYSTEMCONFIG_HARDWAREDEFAULTS_FIRMWAR_MIC4HAEL2_H
#define MSYSTEMCONFIG_HARDWAREDEFAULTS_FIRMWAR_MIC4HAEL2_H

/*********************************************************************************************\
 * This file is one of two user configurable files
 * 
 * "mFirmwareCustom.h" allows advanced users to create a custom build of firmware tailored to a specefic tasks 
 * For example, using a device to control heating, which includes numorous sensors, relays and controls that are unique.
 * This can also include lights or displays as notification/information systems. 
 * Although all sensors/drivers are accesible through default builds, this allows users to config and add their own controllers.
 * Ideally, when possible, templates can be used to replicate the custom firmware options performed here.
\*********************************************************************************************/

// #define DEVICE_FORCED_TO_BE_TESTER
#define DISABLE_WEBSERVER
#define FORCE_TEMPLATE_LOADING

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/Languages/mLanguageDefault.h"
#include "6_Lights/_Interface/mInterfaceLight_Defines.h"
#include "2_CoreSystem/HardwareTemplates/mHardwareTemplates.h"

// //--------------------------------[Enable Device]-------------------------------------

/**
 *  DEV -- -- DEV -- -- DEV -- -- DEV -- -- DEV -- -- DEV -- -- DEV -- -- DEV -- -- DEV -- -- 
**/  
// #define DEVICE_GPSPARSER_TESTER_POLLING
// #define DEVICE_GPSPARSER_TESTER_INTERRUPT
// #define DEVICE_SDLOGGER1_TESTER
// #define DEVICE_UART_TESTER
// #define DEVICE_UART_GPSNMEA_TESTER
// #define DEVICE_UART_GPSUBX_TESTER
// #define DEVICE_SDCARD_TESTER
// #define DEVICE_GPS_TO_SDCARD_TESTER
// #define DEVICE_MEASUREMENT_SYSTEM_1
// #define DEVICE_OLED_WITH_SD_LIPO_TEST//this
// #define DEVICE_PIC32_RSS_STREAM_OUTPUT_TEST
// #define DEVICE_SDCARD_LIPO_TESTER
// #define DEVICE_GPS_SDCARD_LOGGER_WITH_OLED_TEST
// #define DEVICE_ADC_PIN_INTERRUPT_TRIGGER_TEST
// #define DEVICE_RECEIVE_RSS_STREAM_FROM_PIC32_INTO_ESP32_BUFFER_TEST
// #define DEVICE_I2S_ADC_TESTER
// #define DEVICE_TESTBED_I2S_MEASUREMENT
// #define DEVICE_TESTBED_I2S_MEASUREMENT_SINGLE_CHANNEL

/**
 * PhD systems
 * */
// #define DEVICE_MEASUREMENT_SYSTEM_ALPHA_1

// #include "0_ConfigUser/mFirmwareCustom_Secret_Home.h"


// //-----------------[User Defined Devices == USE_BUILD_TYPE_ENERGY == Any Energy Monitoring Firmware]-------------------------------------


// /**
//  * Basic polling first, to get the GPS Parse to mqtt testing functional
//  * Advanced interrupt via buffers to parsing
//  * */

#ifdef DEVICE_GPSPARSER_TESTER_POLLING
  #define DEVICENAME_CTR            "gpsparser_tester"
  #define DEVICENAME_FRIENDLY_CTR   "GPS Parser Tester"

  #define FORCE_TEMPLATE_LOADING

  #define USE_MODULE_DRIVERS_GPS
  
  // #define USE_MODULE_DRIVERS_SERIAL_UART
  #define USE_DEVFEATURE_GPS_POLLING_INPUT
  #define NMEAGPS_PARSE_SAVE_MILLIS
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

#endif // DEVICE_GPSPARSER_TESTER



// #ifdef DEVICE_GPSPARSER_TESTER_INTERRUPT
//   #define DEVICENAME_CTR            "gpsparser_tester"
//   #define DEVICENAME_FRIENDLY_CTR   "GPS Parser Tester"

//   #define FORCE_TEMPLATE_LOADING
//   //#define SETTINGS_HOLDER 21
//   #define USE_DEBUG_PRINT_FUNCTION_NAME_TEST

//   #define USE_MODULE_DRIVERS_GPS
//   // #define NMEAGPS_INTERRUPT_PROCESSING

//   #define USE_DEVFEATURE_RINGBUFFERS

//   // #define USE_MODULE_DRIVERS_SERIAL_UART

//   #define NMEAGPS_PARSE_SAVE_MILLIS
  
//   #define NEOGPS_USE_SERIAL1
// #include <GPSport.h>

// // FEATURE
// // Create an "alert" mqtt topic that all devices will broadcast important info on

// #define ENABLE_DEVFEATURE_GPSTEST1

//   // #define USE_BUILD_TYPE_LIGHTING
//   // #define USE_MODULE_LIGHTS_INTERFACE
//   // #define USE_MODULE_LIGHTS_ANIMATOR
//   // #define USE_MODULE_LIGHTS_ADDRESSABLE
//   // #define USE_MODULE_LIGHTS_WLED_EFFECTS
//   // #define WLED_DEFINE_GLOBAL_VARS //only in one source file, wled.cpp!
//   // #define DISABLE_PIXEL_FUNCTION_EFFECTS
//   // #define USE_MODULE_DRIVERS_LEDS
//   #define DISABLE_WEBSERVER
  
// //   //#define USE_WEBSERVER_ADVANCED_MULTIPAGES // new develop option to limit scope to only include root page while testing
  
// //   #define USE_MODULE_SENSORS_SWITCHES
// //   #define USE_MODULE_SENSORS_ANALOG

//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
//     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_JSON_GPIOC "\":{"
//       "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
//     "},"
//   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
//   "}";

  
//   // #define STRIP_SIZE_MAX 100//256
//   // #define STRIP_REPEAT_OUTPUT_MAX 256

//   // #define USE_LIGHTING_TEMPLATE
//   // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   // "{"
//   //   "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
//   //   #ifdef STRIP_SIZE_MAX
//   //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//   //   #else
//   //   "\"" D_JSON_STRIP_SIZE       "\":50,"
//   //   #endif //STRIP_SIZE_MAX
//   //   "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
//   //   "\"" D_JSON_TRANSITION       "\":{"
//   //     "\"" D_JSON_TIME_MS "\":10000,"
//   //     "\"" D_JSON_RATE_MS "\":1000,"
//   //     "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
//   //     "\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\""
//   //   "},"
//   //   "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
//   //   "\"" D_JSON_EFFECTS "\":{" 
//   //     "\"" D_JSON_FUNCTION "\":\"" "Slow Glow" "\""
//   //   "},"
//   //   "\"" D_JSON_COLOUR_PALETTE "\":\"Single Fire 01\","//Christmas MultiColoured Warmer\","
//   //   "\"" D_JSON_BRIGHTNESS_RGB "\":2"
//   // "}";



// #endif // DEVICE_GPSPARSER_TESTER


// #ifdef DEVICE_SDCARDWRITER_TESTER
//   #define DEVICENAME_CTR            "sdcardwriter_tester"
//   #define DEVICENAME_FRIENDLY_CTR   "SD Card Writer Tester"

//   #define FORCE_TEMPLATE_LOADING
//   //#define SETTINGS_HOLDER 21

//   #define USE_MODULE_DRIVERS_SDCARD

//   // #define USE_BUILD_TYPE_LIGHTING
//   // #define USE_MODULE_LIGHTS_INTERFACE
//   // #define USE_MODULE_LIGHTS_ANIMATOR
//   // #define USE_MODULE_LIGHTS_ADDRESSABLE
//   // #define USE_MODULE_LIGHTS_WLED_EFFECTS
//   // #define WLED_DEFINE_GLOBAL_VARS //only in one source file, wled.cpp!
//   // #define DISABLE_PIXEL_FUNCTION_EFFECTS
//   // #define USE_MODULE_DRIVERS_LEDS
//   #define DISABLE_WEBSERVER
  
// //   //#define USE_WEBSERVER_ADVANCED_MULTIPAGES // new develop option to limit scope to only include root page while testing
  
// //   #define USE_MODULE_SENSORS_SWITCHES
// //   #define USE_MODULE_SENSORS_ANALOG

//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
//     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_JSON_GPIOC "\":{"
//       "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
//     "},"
//   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
//   "}";

  
//   #define STRIP_SIZE_MAX 100//256
//   #define STRIP_REPEAT_OUTPUT_MAX 256

//   #define USE_LIGHTING_TEMPLATE
//   DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   "{"
//     "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
//     #ifdef STRIP_SIZE_MAX
//     "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//     #else
//     "\"" D_JSON_STRIP_SIZE       "\":50,"
//     #endif //STRIP_SIZE_MAX
//     "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
//     "\"" D_JSON_TRANSITION       "\":{"
//       "\"" D_JSON_TIME_MS "\":10000,"
//       "\"" D_JSON_RATE_MS "\":1000,"
//       "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
//       "\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\""
//     "},"
//     "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
//     "\"" D_JSON_EFFECTS "\":{" 
//       "\"" D_JSON_FUNCTION "\":\"" "Slow Glow" "\""
//     "},"
//     "\"" D_JSON_COLOUR_PALETTE "\":\"Single Fire 01\","//Christmas MultiColoured Warmer\","
//     "\"" D_JSON_BRIGHTNESS_RGB "\":2"
//   "}";



// #endif // DEVICE_GPSPARSER_TESTER



/**
 * 1) Serial0 is debugging, and will be disabled
 * 2) Serial1 is RSS samples in as superframes
 * 3) Serial1 is GPS input (9600)
 * 4) GPSParser using my hardware, as fast as possible
 * 5) OLED screen to give sd card status
 * 6) SDCard records superframe+gpsfix
 * 7) Need button to open/close sd (or dupont short? or small surface mount)
 * 
 * Configure dummy data, and feed it into both UART_GPS and UART_RSS simultaneously,
 * Echo both directly out of UART_RSS_TX onto putty, leave it working all the time (laptop side)
 * 
 * 
 * 1 - Check what gps can do, can I enter it into ublox? Does this improve speed, let me do manual decoding?
 * Test 1 - Create long wire for gps to sit in window and give active gps data
 * 
 * STEPS # - uncomment just the section below it, so I can quickly test configs as I go, only uncomment one step at a time
 * 
 * **/
#ifdef DEVICE_SDLOGGER1_TESTER
  #define DEVICENAME_CTR            "sdlogger1_tester"
  #define DEVICENAME_FRIENDLY_CTR   "SD & GPS Logger Tester"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define USE_MODULE_NETWORK_MQTT

  /**
   *  - Test Buttons
   * */
  // #define USE_MODULE_SENSORS_BUTTONS
  /**
   *  - Enable rule triggers
   * */
  // #define USE_MODULE_CORE_RULES
  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BUTTONS
  /**
   *  - Add triggered function in sdcard that will toggle sd later via button
   * */
  // #define USE_MODULE_CORE_RULES
  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BUTTONS
  // #define USE_MODULE_CONTROLLER_SDCARDLOGGER
  /**
   *  - Enable displays, and show button pressed on it
   * */
  // #define USE_MODULE_CORE_RULES
  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BUTTONS
  // #define USE_MODULE_CONTROLLER_SDCARDLOGGER
  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SSD1306
  /**
   *  - Try sd open, basic write, close
   * */
  // #define USE_MODULE_DRIVERS_SDCARD
  /**
   *  - Putty Serial inputs into Serial1 buffer
   * */
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_SERIAL_UART
  #define ENABLE_UART2_ISR_BUFFERS
  /**
   *  - Get stable uart2 ringbuffer
   * */
  // #define USE_MODULE_DRIVERS_SERIAL_UART
  // // #define ENABLE_UART2_ISR_BUFFERS
  // #define ENABLE_UART_RINGBUFFERS
  // #define ENABLE_FEATURE_BLINK_ON_ISR_ACTIVITY
  /**
   *  - Read GPS Data into UART_GPS and UART_RSS (as dummy data)
   * */
  // #define USE_MODULE_CORE_RULES
  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BUTTONS
  // #define USE_MODULE_CONTROLLER_SDCARDLOGGER
  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SSD1306
  // #define USE_MODULE_DRIVERS_INTERFACE
  // #define USE_MODULE_DRIVERS_SERIAL_UART
  // #define ENABLE_UART2_ISR_BUFFERS
  
//   
//   #define USE_DEVFEATURE_GPS_FROM_SERIAL2_BUFFER
//   #define USE_DEVFEATURE_RINGBUFFERS
//   // #define USE_MODULE_DRIVERS_SERIAL_UART

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"

    //https://randomnerdtutorials.com/esp32-adc-analog-read-arduino-ide/

    //   // OLED screen
    //   #ifdef USE_MODULE_DISPLAYS_OLED_SSD1306
    //   "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
    //   "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
    //   // "\"2\":\"" D_GPIO_FUNCTION_OLED_RESET_CTR   "\","
    //   #endif // USE_MODULE_DISPLAYS_OLED_SSD1306

    //   #ifdef USE_MODULE_SENSORS_BUTTONS
    //   "\"22\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\","
    //   #endif 

    //   // 2 push buttons

    //   // SD card

    //   // GPS

    //   // Serial data logger (just receives into a buffer)
      
    //   "\"22\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
    //   "\"23\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","


      "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
      "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","




    //   "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    // "\"" D_JSON_DEVICENAME "\":{"
    //   "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
    //     "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
    //   "],"
    //   "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
    //     "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\""
    //   "]"
    // "}"
    
    "\"" D_JSON_DISPLAY_MODE "\":6"
  "}";







#endif // 




// /**
//  * Basic polling first, to get the GPS Parse to mqtt testing functional
//  * Advanced interrupt via buffers to parsing
//  * */

#ifdef DEVICE_UART_TESTER
  #define DEVICENAME_CTR            "uart_tester"
  #define DEVICENAME_FRIENDLY_CTR   "UART Tester"

  #define ESP32

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_SERIAL_UART
  
  #define ENABLE_HARDWARE_UART_1
  #define ENABLE_HARDWARE_UART_2
  #define HARDWARE_UART_1_BAUD_RATE_SPEED  1536000
  #define HARDWARE_UART_2_BAUD_RATE_SPEED  9600

  #define DEBUG_PIN1_GPIO     12
  #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
  #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
  #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

  #define DEBUG_PIN2_GPIO     13
  #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
  #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
  #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"12\":\"" D_GPIO_FUNCTION_DEBUG_PIN1_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
      "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
      "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

#endif // DEVICE_GPSPARSER_TESTER


#ifdef DEVICE_UART_GPS_TESTER
  #define DEVICENAME_CTR            "uart_gps_tester"
  #define DEVICENAME_FRIENDLY_CTR   "UART GPS Tester"

  #define ESP32
  
  #define USE_MODULE_DRIVERS_GPS
  
  // #define USE_MODULE_DRIVERS_SERIAL_UART
  // #define USE_DEVFEATURE_GPS_POLLING_INPUT
  #define NMEAGPS_PARSE_SAVE_MILLIS
  #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_SERIAL_UART
  // #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
  
  #define ENABLE_HARDWARE_UART_1
  #define ENABLE_HARDWARE_UART_2
  #define HARDWARE_UART_1_BAUD_RATE_SPEED  9600
  #define HARDWARE_UART_2_BAUD_RATE_SPEED  9600

  #define DEBUG_PIN1_GPIO     12
  #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
  #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
  #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

  #define DEBUG_PIN2_GPIO     13
  #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
  #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
  #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"12\":\"" D_GPIO_FUNCTION_DEBUG_PIN1_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
      "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
      "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

#endif // DEVICE_GPSPARSER_TESTER


#ifdef DEVICE_UART_GPSUBX_TESTER
  #define DEVICENAME_CTR            "uart_gpsubx_tester"
  #define DEVICENAME_FRIENDLY_CTR   "UART GPS UBX Tester"

  #define ESP32
  
  #define USE_MODULE_DRIVERS_GPS

  #define DISABLE_NETWORK
  
  #define NMEAGPS_DERIVED_TYPES
  #define gpsPort Serial1

  // #define USE_DEVFEATURE_GPS_POLLING_INPUT
  // #define NMEAGPS_PARSE_SAVE_MILLIS
  #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
  #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_SERIAL_UART
  // #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL

  // #define USE_DEVFEATURE_GPS_POLLING_INPUT_UBX

  #define ENABLE_GPS_PARSER_UBX
  
  #define ENABLE_HARDWARE_UART_1
  #define ENABLE_HARDWARE_UART_2
  #define HARDWARE_UART_1_BAUD_RATE_SPEED  9600
  #define HARDWARE_UART_2_BAUD_RATE_SPEED  921600

  #define DEBUG_PIN1_GPIO     12
  #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
  #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
  #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

  #define DEBUG_PIN2_GPIO     13
  #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
  #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
  #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"12\":\"" D_GPIO_FUNCTION_DEBUG_PIN1_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
      "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
      "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

#endif // DEVICE_GPSPARSER_TESTER



/**
 * Description: init dev of sdcard code, to open, write then close an SD card
 * Also adding time tests, writing of bytes bottleneck
 * */
#ifdef DEVICE_SDCARD_TESTER
  #define DEVICENAME_CTR            "sdcard_tester"
  #define DEVICENAME_FRIENDLY_CTR   "SDCard Tester"

  #define ESP32
  
  #define USE_MODULE_DRIVERS_SDCARD
  // #define USE_MODULE_CONTROLLER_SDCARDLOGGER

  #define DEBUG_PIN1_GPIO     12
  #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
  #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
  #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

  #define DEBUG_PIN2_GPIO     13
  #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
  #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
  #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"12\":\"" D_GPIO_FUNCTION_DEBUG_PIN1_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
      "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
      "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

#endif // DEVICE_GPSPARSER_TESTER


/**
 * Description: gps via uart2 into ringbuffer, then ringbuffer into sdcard
 * */
#ifdef DEVICE_GPS_TO_SDCARD_TESTER
  #define DEVICENAME_CTR            "gps_to_sdcard_tester"
  #define DEVICENAME_FRIENDLY_CTR   "GPS to SDCard Tester"

  #define ESP32
  
  #define USE_MODULE_DRIVERS_GPS
  
  // #define USE_MODULE_DRIVERS_SERIAL_UART
  // #define USE_DEVFEATURE_GPS_POLLING_INPUT
  #define NMEAGPS_PARSE_SAVE_MILLIS
  #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_SERIAL_UART
  // #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
  
  #define ENABLE_HARDWARE_UART_1
  #define ENABLE_HARDWARE_UART_2
  #define HARDWARE_UART_1_BAUD_RATE_SPEED  9600
  #define HARDWARE_UART_2_BAUD_RATE_SPEED  9600

  #define USE_MODULE_DRIVERS_SDCARD
  #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER

  #define DEBUG_PIN1_GPIO     12
  #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
  #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
  #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

  #define DEBUG_PIN2_GPIO     13
  #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
  #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
  #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"12\":\"" D_GPIO_FUNCTION_DEBUG_PIN1_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
      "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
      "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

#endif // DEVICE_GPSPARSER_TESTER


/**
 * Description: init dev of sdcard code, to open, write then close an SD card
 * Also adding time tests, writing of bytes bottleneck
 * */
#ifdef DEVICE_SDCARD_LIPO_TESTER
  #define DEVICENAME_CTR            "sdcard_lipo_tester"
  #define DEVICENAME_FRIENDLY_CTR   "sdcard_lipo_tester Tester"

  #define ESP32
  
  #define USE_MODULE_DRIVERS_SDCARD
  // #define USE_MODULE_CONTROLLER_SDCARDLOGGER

  #define DEBUG_PIN1_GPIO     12
  #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
  #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
  #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

  #define DEBUG_PIN2_GPIO     13
  #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
  #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
  #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"12\":\"" D_GPIO_FUNCTION_DEBUG_PIN1_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
      "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
      "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

#endif // DEVICE_GPSPARSER_TESTER




/**
 * Description: Use of button, oled and sdcard together on LIPO_ESP32
 * Button will toggle logging start/end (open/close), which will write the RTC time line by line. This will let me test sd opening and closing when expected
 * */
#ifdef DEVICE_OLED_WITH_SD_LIPO_TEST
  #define DEVICENAME_CTR            "system_1_tester"
  #define DEVICENAME_FRIENDLY_CTR   "SYSTEM_1 Tester"

  // Also defining so "HardwareSerial.cpp" will use these
  #define RX1 18
  #define TX1 19
  #define RX2 16
  #define TX2 17

  // General defines for debugging only, not for finished
  #define DISABLE_NETWORK
  #define DISABLE_SLEEP

  /** Next things to do
   * Completely make it so it can compile without NTP, wifi, mqtt and telemetry
   * Add that the GPS can be init after a period of time with no updates, ie connected after
   * Add that a counter shows the last known time period of GPS signal, if none, alert me
   * Order cheap caps, to attach the hardware to
   * Use a GPIO to trigger interrupt, which will actually inform when to record adc readings.. these will be filtered into vector...
   * I will need a GPIO_SuperFrame reset and GPIO_RSS_ADC measure to properly know when a packet is ready. The start of a new SF will trigger sending/saving latest completed SF frame, OR, every 1 second to keep the buffers clear
   * */

  // General defines needed in release version
  #define ESP32
  #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

  // // Section A: GPS
  #define USE_MODULE_DRIVERS_GPS
  #define ENABLE_GPS_PARSER_NMEA
  #define ENABLE_GPS_PARSER_UBX
  #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
  // #define ENABLE_GPS_DEVICE_CONFIG_SPEED_SLOW5
  #define NMEAGPS_DERIVED_TYPES
  #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
  #define NMEAGPS_PARSE_SAVE_MILLIS
  #define gpsPort Serial1
  #define D_GPS_BAUD_RATE_FAST 921600//230400//115200
  #define D_GPS_BAUD_RATE_DEFAULT 9600

  // // Section UART
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_SERIAL_UART
  #define ENABLE_HARDWARE_UART_1
  #define ENABLE_HARDWARE_UART_2
  #define HARDWARE_UART_1_BAUD_RATE_SPEED  D_GPS_BAUD_RATE_FAST
  #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000

  // // Section RSS data
  // #define ENABLE_DEVFEATURE_DUMMY_RSS_DATA

  // // Section B: SDCard driver
  #define USE_MODULE_DRIVERS_SDCARD

  // Seciton C: Logger controller
  #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
  #define ENABLE_SDLOGGER_APPEND_TIME_TEST

  // // Section x: Button to toggle logging/sd state
  // #define USE_MODULE_CORE_RULES
  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE_SENSORS_ADC_INTERNAL
  #define ENABLE_SMOOTHING_ON_ADC_READING
  #define ADC_CAPTURE_EXTERNAL_PIN 32
  #define ADC_CAPTURE_INPUT0_PIN 34
  #define ADC_CAPTURE_INPUT1_PIN 35

  // // Section x: OLED display 
  #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_OLED_SSD1306


  // debug
  #define DEBUG_PIN1_GPIO     12
  #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
  #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
  #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

  #define DEBUG_PIN2_GPIO     13
  #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
  #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
  #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      /**
       * Debug pins
       * */

// Change this to be a 6pin between pic32 and esp32
/**
 * Shared power for stable IO
 * 3v3
 * ground
 * 
 * adc2
 * adc5
 * esp32 adc trigger
 * esp32 json completed
 * 
 * serial?
 * 
 * 
 * */

      /** 4P large JST - ADC
       * Yellow     34(I), ADC2G, ADC1_CH6
       * White      35(I), ADC5G, ADC1_CH7
       * Red        32(I), ADC Record Trigger
       * Black      GND
       * */
      "\"34\":\"" D_GPIO_FUNCTION_ADC1_CH6_CTR   "\","
      "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","
      "\"32\":\"" D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR   "\","
      // "\"32\":\"" D_GPIO_FUNCTION_GENERIC_GPIO1_CTR   "\","

      /** 5P small - UART2 RSS Stream
       * Orange      17, UART2_TX
       * Yellow      16, UART2_RX
       * White       25, 
       * Red         5V
       * Black       GND
       * */
      "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
      "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","

      /** 5P small - UART1 GPS Stream
       * Orange      19, UART1_TX
       * Yellow      18, UART1_RX
       * White        
       * Red         VCC, 3V3
       * Black       GND
       * */
      "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
      "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","

      /** 6P small - SD Card
       * Green       15, CS
       * Orange      14, SCK
       * Yellow      13, MOSI
       * White       12, MISO
       * Red         3V3
       * Black       GND
       * */
      "\"15\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CSO_CTR   "\","
      "\"14\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CLK_CTR   "\","
      "\"13\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MOSI_CTR  "\","
      "\"12\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MISO_CTR  "\","

      /** Built in - OLED
       * 
       * */
      "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
      
      /** 2P small
       * Red        Button Logging Toggle
       * Black      GND
       * */
      "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

#endif // DEVICE_GPSPARSER_TESTER


/**
 * Description: Use of button, oled and sdcard together on LIPO_ESP32
 * Button will toggle logging start/end (open/close), which will write the RTC time line by line. This will let me test sd opening and closing when expected
 * Test version before adding pic32 stream of data (although will use same format)
 * Aim: To log GPS data at 10hz, to sd card, and have me drive around and get that 3d fix saved and into matlab then google earth
 * 
 * GPS @ 912800 baud recorded to local fix
 * - each new fix, triggers a "save" flag
 * OLED screen will show bytes written every second
 * No networking
 * 
 * 
 * */
#ifdef DEVICE_GPS_SDCARD_LOGGER_WITH_OLED_TEST
  #define DEVICENAME_CTR            "system_1_tester"
  #define DEVICENAME_FRIENDLY_CTR   "SYSTEM_1 Tester"

  // Also defining so "HardwareSerial.cpp" will use these
  #define RX1 18
  #define TX1 19
  #define RX2 16
  #define TX2 17

  // General defines for debugging only, not for finished
  #define DISABLE_NETWORK
  #define DISABLE_SLEEP

  /** Next things to do
   * Completely make it so it can compile without NTP, wifi, mqtt and telemetry
   * Add that the GPS can be init after a period of time with no updates, ie connected after
   * Add that a counter shows the last known time period of GPS signal, if none, alert me
   * Order cheap caps, to attach the hardware to
   * Use a GPIO to trigger interrupt, which will actually inform when to record adc readings.. these will be filtered into vector...
   * I will need a GPIO_SuperFrame reset and GPIO_RSS_ADC measure to properly know when a packet is ready. The start of a new SF will trigger sending/saving latest completed SF frame, OR, every 1 second to keep the buffers clear
   * */

  // General defines needed in release version
  #define ESP32
  #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

  // // Section A: GPS
  #define USE_MODULE_DRIVERS_GPS
  #define ENABLE_GPS_PARSER_NMEA
  #define ENABLE_GPS_PARSER_UBX
  #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
  // #define ENABLE_GPS_DEVICE_CONFIG_SPEED_SLOW5
  #define NMEAGPS_DERIVED_TYPES
  #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
  #define NMEAGPS_PARSE_SAVE_MILLIS
  #define gpsPort Serial1
  #define D_GPS_BAUD_RATE_FAST    921600//230400//115200
  #define D_GPS_BAUD_RATE_DEFAULT 9600

  // // Section UART
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_SERIAL_UART
  #define ENABLE_HARDWARE_UART_1
  #define ENABLE_HARDWARE_UART_2
  #define HARDWARE_UART_1_BAUD_RATE_SPEED  D_GPS_BAUD_RATE_FAST
  #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000

  // // Section RSS data
  // #define ENABLE_DEVFEATURE_DUMMY_RSS_DATA

  // // Section B: SDCard driver
  #define USE_MODULE_DRIVERS_SDCARD
  #define USE_SDCARD_RINGBUFFER_STEAM_OUT

  // Seciton C: Logger controller
  #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
  // #define ENABLE_SDLOGGER_APPEND_TIME_TEST
  #define ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST

  // // Section x: Button to toggle logging/sd state
  #define USE_MODULE_CORE_RULES
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BUTTONS
  // Next phase, will include saving adc values
  // #define USE_MODULE_SENSORS_ADC_INTERNAL
  // #define ENABLE_SMOOTHING_ON_ADC_READING
  // #define ADC_CAPTURE_EXTERNAL_PIN 32
  // #define ADC_CAPTURE_INPUT0_PIN 34
  // #define ADC_CAPTURE_INPUT1_PIN 35

  // Section x: OLED display 
  #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_OLED_SSD1306
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      /**
       * Debug pins
       * */

      /** 4P large JST - ADC
       * Yellow     34(I), ADC2G, ADC1_CH6
       * White      35(I), ADC5G, ADC1_CH7
       * Red        32(I), ADC Record Trigger
       * Black      GND
       * */
      "\"34\":\"" D_GPIO_FUNCTION_ADC1_CH6_CTR   "\","
      "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","
      "\"32\":\"" D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR   "\","
      // "\"32\":\"" D_GPIO_FUNCTION_GENERIC_GPIO1_CTR   "\","

      /** 5P small - UART2 RSS Stream
       * Orange      17, UART2_TX
       * Yellow      16, UART2_RX
       * White       25, 
       * Red         5V
       * Black       GND
       * */
      "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
      "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","

      /** 5P small - UART1 GPS Stream
       * Orange      19, UART1_TX
       * Yellow      18, UART1_RX
       * White        
       * Red         VCC, 3V3
       * Black       GND
       * */
      "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
      "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","

      /** 6P small - SD Card
       * Green       15, CS
       * Orange      14, SCK
       * Yellow      13, MOSI
       * White       12, MISO
       * Red         3V3
       * Black       GND
       * */
      "\"15\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CSO_CTR   "\","
      "\"14\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CLK_CTR   "\","
      "\"13\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MOSI_CTR  "\","
      "\"12\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MISO_CTR  "\","

      /** Built in - OLED
       * 
       * */
      "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
      
      /** 2P small
       * Red        Button Logging Toggle
       * Black      GND
       * */
      "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";



  // debug
  #define DEBUG_PIN1_GPIO     26
  #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
  #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
  #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

  #define DEBUG_PIN2_GPIO     27
  #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
  #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
  #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));

  #define DEBUG_PIN3_GPIO     21
  #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
  #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
  #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

  #define DEBUG_PIN4_GPIO     22
  #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
  #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
  #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

#endif // DEVICE_GPSPARSER_TESTER



/**
 * Description: Use of button, oled and sdcard together on LIPO_ESP32
 * Button will toggle logging start/end (open/close), which will write the RTC time line by line. This will let me test sd opening and closing when expected
 * Test version before adding pic32 stream of data (although will use same format)
 * Aim: To log GPS data at 10hz, to sd card, and have me drive around and get that 3d fix saved and into matlab then google earth
 * 
 * GPS @ 912800 baud recorded to local fix
 * - each new fix, triggers a "save" flag
 * OLED screen will show bytes written every second
 * No networking
 * 
 * 
 * */
#ifdef DEVICE_ADC_PIN_INTERRUPT_TRIGGER_TEST
  #define DEVICENAME_CTR            "system_1_tester"
  #define DEVICENAME_FRIENDLY_CTR   "SYSTEM_1 Tester"

  #define SERIAL_DEBUG_BAUD_DEFAULT 921600

  // Also defining so "HardwareSerial.cpp" will use these
  #define RX1 18
  #define TX1 19
  #define RX2 16
  #define TX2 17

  // General defines for debugging only, not for finished
  #define DISABLE_NETWORK
  #define DISABLE_SLEEP

  /** Next things to do
   * Completely make it so it can compile without NTP, wifi, mqtt and telemetry
   * Add that the GPS can be init after a period of time with no updates, ie connected after
   * Add that a counter shows the last known time period of GPS signal, if none, alert me
   * Order cheap caps, to attach the hardware to
   * Use a GPIO to trigger interrupt, which will actually inform when to record adc readings.. these will be filtered into vector...
   * I will need a GPIO_SuperFrame reset and GPIO_RSS_ADC measure to properly know when a packet is ready. The start of a new SF will trigger sending/saving latest completed SF frame, OR, every 1 second to keep the buffers clear
   * */

  // General defines needed in release version
  #define ESP32
  #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

  // // // Section A: GPS
  // #define USE_MODULE_DRIVERS_GPS
  // #define ENABLE_GPS_PARSER_NMEA
  // #define ENABLE_GPS_PARSER_UBX
  // #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
  // // #define ENABLE_GPS_DEVICE_CONFIG_SPEED_SLOW5
  // #define NMEAGPS_DERIVED_TYPES
  // #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
  // #define NMEAGPS_PARSE_SAVE_MILLIS
  // #define gpsPort Serial1
  // #define D_GPS_BAUD_RATE_FAST    921600//230400//115200
  // #define D_GPS_BAUD_RATE_DEFAULT 9600

  // // // Section UART
  // #define USE_MODULE_DRIVERS_INTERFACE
  // #define USE_MODULE_DRIVERS_SERIAL_UART
  // #define ENABLE_HARDWARE_UART_1
  // #define ENABLE_HARDWARE_UART_2
  // #define HARDWARE_UART_1_BAUD_RATE_SPEED  D_GPS_BAUD_RATE_FAST
  // #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000

  // // // Section RSS data
  // // #define ENABLE_DEVFEATURE_DUMMY_RSS_DATA

  // // // Section B: SDCard driver
  // #define USE_MODULE_DRIVERS_SDCARD
  // #define USE_SDCARD_RINGBUFFER_STEAM_OUT

  // // Seciton C: Logger controller
  // #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
  // // #define ENABLE_SDLOGGER_APPEND_TIME_TEST
  // #define ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST

  // // Section x: Button to toggle logging/sd state
  #define USE_MODULE_CORE_RULES
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BUTTONS
  // Next phase, will include saving adc values
  #define USE_MODULE_SENSORS_ADC_INTERNAL
  #define ENABLE_SMOOTHING_ON_ADC_READING
  #define ENABLE_ADC_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
  #define ADC_CAPTURE_EXTERNAL_PIN 32
  #define ADC_CAPTURE_INPUT0_PIN 34
  #define ADC_CAPTURE_INPUT1_PIN 35

  // Section x: OLED display 
  #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_OLED_SSD1306
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      /**
       * Debug pins
       * */

      /** 4P large JST - ADC
       * Yellow     34(I), ADC2G, ADC1_CH6
       * White      35(I), ADC5G, ADC1_CH7
       * Red        32(I), ADC Record Trigger
       * Black      GND
       * */
      "\"34\":\"" D_GPIO_FUNCTION_ADC1_CH6_CTR   "\","
      "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","
      "\"32\":\"" D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR   "\","
      // "\"32\":\"" D_GPIO_FUNCTION_GENERIC_GPIO1_CTR   "\","

      /** 5P small - UART2 RSS Stream
       * Orange      17, UART2_TX
       * Yellow      16, UART2_RX
       * White       25, ie superframe event over, rising edge interrupt
       * Red         5V
       * Black       GND
       * */
      "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
      "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","

      /** 5P small - UART1 GPS Stream
       * Orange      19, UART1_TX
       * Yellow      18, UART1_RX
       * White        
       * Red         VCC, 3V3
       * Black       GND
       * */
      "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
      "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","

      /** 6P small - SD Card
       * Green       15, CS
       * Orange      14, SCK
       * Yellow      13, MOSI
       * White       12, MISO
       * Red         3V3
       * Black       GND
       * */
      "\"15\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CSO_CTR   "\","
      "\"14\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CLK_CTR   "\","
      "\"13\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MOSI_CTR  "\","
      "\"12\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MISO_CTR  "\","

      /** Built in - OLED
       * 
       * */
      "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
      
      /** 2P small
       * Red        Button Logging Toggle
       * Black      GND
       * */
      "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";



  // debug
  #define DEBUG_PIN1_GPIO     26
  #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
  #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
  #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

  #define DEBUG_PIN2_GPIO     27
  #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
  #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
  #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));

  #define DEBUG_PIN3_GPIO     21
  #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
  #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
  #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

  #define DEBUG_PIN4_GPIO     22
  #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
  #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
  #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

#endif // DEVICE_GPSPARSER_TESTER



/**
 * Description: Serial2 will output stream as if it is the pic32, with entire superframe including:
 * - ramping analogwriter, so I can test that
 * - superframe of 50, with digital trigger lines for adc and when super frame is over
 * 
 * 
 * Button will toggle logging start/end (open/close), which will write the RTC time line by line. This will let me test sd opening and closing when expected
 * */
#ifdef DEVICE_PIC32_RSS_STREAM_OUTPUT_TEST
  #define DEVICENAME_CTR            "pic32_emulator_tester"
  #define DEVICENAME_FRIENDLY_CTR   "SYSTEM_1 Tester"

  // Also defining so "HardwareSerial.cpp" will use these
  #define RX1 18
  #define TX1 19
  #define RX2 16
  #define TX2 17

  // General defines for debugging only, not for finished
  #define DISABLE_NETWORK

  /** Next things to do
   * Completely make it so it can compile without NTP, wifi, mqtt and telemetry
   * Add that the GPS can be init after a period of time with no updates, ie connected after
   * Add that a counter shows the last known time period of GPS signal, if none, alert me
   * Order cheap caps, to attach the hardware to
   * Use a GPIO to trigger interrupt, which will actually inform when to record adc readings.. these will be filtered into vector...
   * I will need a GPIO_SuperFrame reset and GPIO_RSS_ADC measure to properly know when a packet is ready. The start of a new SF will trigger sending/saving latest completed SF frame, OR, every 1 second to keep the buffers clear
   * */
  // #define DISABLE_SERIAL0_CORE
  #define DISABLE_SLEEP

  // General defines needed in release version
  #define ESP32
  // #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

  #define USE_MODULE_CONTROLLER_USERMOD_01

  // // Section A: GPS
  // #define USE_MODULE_DRIVERS_GPS
  // #define ENABLE_GPS_PARSER_NMEA
  // #define ENABLE_GPS_PARSER_UBX
  // #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
  // // #define ENABLE_GPS_DEVICE_CONFIG_SPEED_SLOW5
  // #define NMEAGPS_DERIVED_TYPES
  // #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
  // #define NMEAGPS_PARSE_SAVE_MILLIS
  // #define gpsPort Serial1
  // #define D_GPS_BAUD_RATE_FAST 921600//230400//115200
  // #define D_GPS_BAUD_RATE_DEFAULT 9600

  // Section UART
  // #define USE_MODULE_DRIVERS_INTERFACE
  // #define USE_MODULE_DRIVERS_SERIAL_UART
  // #define ENABLE_HARDWARE_UART_1
  // #define ENABLE_HARDWARE_UART_2
  // #define HARDWARE_UART_1_BAUD_RATE_SPEED  D_GPS_BAUD_RATE_FAST
  // #define HARDWARE_UART_2_BAUD_RATE_SPEED  115200

  // // Section RSS data
  // #define ENABLE_DEVFEATURE_DUMMY_RSS_DATA

  // // Section B: SDCard driver
  // #define USE_MODULE_DRIVERS_SDCARD

  // // Seciton C: Logger controller
  // #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
  // #define ENABLE_SDLOGGER_APPEND_TIME_TEST

  // // Section x: Button to toggle logging/sd state
  // #define USE_MODULE_CORE_RULES
  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BUTTONS

  // // Section x: OLED display 
  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SSD1306

  #define DEBUG_PIN1_GPIO     12
  #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
  #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
  #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

  #define DEBUG_PIN2_GPIO     13
  #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
  #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
  #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      /**
       * Debug pins
       * */
      "\"22\":\"" D_GPIO_FUNCTION_PWM1_CTR   "\","
      "\"2\":\"" D_GPIO_FUNCTION_PWM2_CTR   "\","

    //   /** 4P large JST - ADC
    //    * Yellow     34(I), ADC2G, ADC1_CH6
    //    * White      35(I), ADC5G, ADC1_CH7
    //    * Red        32(I), ADC Record Trigger
    //    * Black      GND
    //    * */
    //   "\"34\":\"" D_GPIO_FUNCTION_ADC1_CH6_CTR   "\","
    //   "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","
    //   "\"32\":\"" D_GPIO_FUNCTION_GENERIC_GPIO1_CTR   "\","

    //   /** 5P small - UART2 RSS Stream
    //    * Orange      17, UART2_TX
    //    * Yellow      16, UART2_RX
    //    * White       25, 
    //    * Red         5V
    //    * Black       GND
    //    * */
    //   "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
    //   "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","

    //   /** 5P small - UART1 GPS Stream
    //    * Orange      19, UART1_TX
    //    * Yellow      18, UART1_RX
    //    * White        
    //    * Red         VCC, 3V3
    //    * Black       GND
    //    * */
    //   "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
    //   "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","

    //   /** 6P small - SD Card
    //    * Green       15, CS
    //    * Orange      14, SCK
    //    * Yellow      13, MOSI
    //    * White       12, MISO
    //    * Red         3V3
    //    * Black       GND
    //    * */
    //   "\"15\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CSO_CTR   "\","
    //   "\"14\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CLK_CTR   "\","
    //   "\"13\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MOSI_CTR  "\","
    //   "\"12\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MISO_CTR  "\","

    //   /** Built in - OLED
    //    * 
    //    * */
    //   "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
    //   "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
      
    //   /** 2P small
    //    * Red        Button Logging Toggle
    //    * Black      GND
    //    * */
    //   "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

#endif // DEVICE_GPSPARSER_TESTER


/**
 * Description: gps via uart2 into ringbuffer, then ringbuffer into sdcard
 * */
#ifdef DEVICE_MEASUREMENT_SYSTEM_1
  #define DEVICENAME_CTR            "measurement_system_1_tester"
  #define DEVICENAME_FRIENDLY_CTR   "MEASUREMENT_SYSTEM_1 Tester"

  // General defines for debugging only, not for finished
  #define DISABLE_NETWORK

  // General defines needed in release version
  #define ESP32

  // Section A: GPS
  #define USE_MODULE_DRIVERS_GPS
  #define ENABLE_GPS_PARSER_UBX
  #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
  // #define ENABLE_GPS_DEVICE_CONFIG_SPEED_SLOW1
  // #define ENABLE_GPS_DEVICE_CONFIG_SPEED_SLOW2
  // #define ENABLE_GPS_DEVICE_CONFIG_SPEED_SLOW3
  // #define ENABLE_GPS_DEVICE_CONFIG_SPEED_SLOW4
  #define ENABLE_GPS_DEVICE_CONFIG_SPEED_SLOW5
  #define NMEAGPS_DERIVED_TYPES
  #define gpsPort Serial1
  #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
  #define NMEAGPS_PARSE_SAVE_MILLIS

  // Section UART
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_SERIAL_UART
  #define ENABLE_HARDWARE_UART_1
  #define ENABLE_HARDWARE_UART_2
  #define HARDWARE_UART_1_BAUD_RATE_SPEED  9600
  #define HARDWARE_UART_2_BAUD_RATE_SPEED  115200//921600

  // Section B: SDCard driver
  // #define USE_MODULE_DRIVERS_SDCARD

  // Seciton C: Logger controller
  
  
  // #define USE_MODULE_DRIVERS_SERIAL_UART
  // #define USE_DEVFEATURE_GPS_POLLING_INPUT

  // #define USE_MODULE_CORE_RULES
  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BUTTONS
  
  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SSD1306

  // #define ENABLE_BUG_TRACING
  // #define DEBUG_NUM1

  #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING



  // #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER

  #define DEBUG_PIN1_GPIO     12
  #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
  #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
  #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

  #define DEBUG_PIN2_GPIO     13
  #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
  #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
  #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"22\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\","
      #endif 
      
      // OLED screen
      #ifdef USE_MODULE_DISPLAYS_OLED_SSD1306
      "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      // "\"2\":\"" D_GPIO_FUNCTION_OLED_RESET_CTR   "\","
      #endif // USE_MODULE_DISPLAYS_OLED_SSD1306
      "\"12\":\"" D_GPIO_FUNCTION_DEBUG_PIN1_CTR   "\","
      "\"26\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
      "\"27\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
      "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
      "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

#endif // DEVICE_GPSPARSER_TESTER


/**
 * Description: Use of button, oled and sdcard together on LIPO_ESP32
 * Button will toggle logging start/end (open/close), which will write the RTC time line by line. This will let me test sd opening and closing when expected
 * Test version before adding pic32 stream of data (although will use same format)
 * Aim: To log GPS data at 10hz, to sd card, and have me drive around and get that 3d fix saved and into matlab then google earth
 * 
 * GPS @ 912800 baud recorded to local fix
 * - each new fix, triggers a "save" flag
 * OLED screen will show bytes written every second
 * No networking
 * 
 * 
 * */
#ifdef DEVICE_RECEIVE_RSS_STREAM_FROM_PIC32_INTO_ESP32_BUFFER_TEST
  #define DEVICENAME_CTR            "system_1_tester"
  #define DEVICENAME_FRIENDLY_CTR   "SYSTEM_1 Tester"

  #define SERIAL_DEBUG_BAUD_DEFAULT 921600

  // Also defining so "HardwareSerial.cpp" will use these
  #define RX1 18
  #define TX1 19
  #define RX2 16
  #define TX2 17

  // General defines for debugging only, not for finished
  #define DISABLE_NETWORK
  #define DISABLE_SLEEP

  /** Next things to do
   * Completely make it so it can compile without NTP, wifi, mqtt and telemetry
   * Add that the GPS can be init after a period of time with no updates, ie connected after
   * Add that a counter shows the last known time period of GPS signal, if none, alert me
   * Order cheap caps, to attach the hardware to
   * Use a GPIO to trigger interrupt, which will actually inform when to record adc readings.. these will be filtered into vector...
   * I will need a GPIO_SuperFrame reset and GPIO_RSS_ADC measure to properly know when a packet is ready. The start of a new SF will trigger sending/saving latest completed SF frame, OR, every 1 second to keep the buffers clear
   * */

  // General defines needed in release version
  #define ESP32
  #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

  // // // Section A: GPS
  // #define USE_MODULE_DRIVERS_GPS
  // #define ENABLE_GPS_PARSER_NMEA
  // #define ENABLE_GPS_PARSER_UBX
  // #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
  // #define NMEAGPS_DERIVED_TYPES
  // #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
  // #define NMEAGPS_PARSE_SAVE_MILLIS
  // #define gpsPort Serial1
  // #define D_GPS_BAUD_RATE_FAST    921600//230400//115200
  // #define D_GPS_BAUD_RATE_DEFAULT 9600

  // // // Section UART
  // #define USE_MODULE_DRIVERS_INTERFACE
  // #define USE_MODULE_DRIVERS_SERIAL_UART
  // #define ENABLE_HARDWARE_UART_1
  // #define ENABLE_HARDWARE_UART_2
  // #define HARDWARE_UART_1_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
  // #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
  // #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
  // #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
  // #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT

  // // Section B: SDCard driver
  // #define USE_MODULE_DRIVERS_SDCARD
  // #define USE_SDCARD_RINGBUFFER_STEAM_OUT

  // // // Seciton C: Logger controller
  // #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
  // // #define ENABLE_SDLOGGER_APPEND_TIME_TEST
  // #define ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
  // #define ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS
  // #define GPIO_SYNC_FRAME_ISR_PIN 25
  // #define ENABLE_INTERRUPT_ON_LOW_PINX_FOR_ADC_CAPTURE
  // #define GPIO_ADC_CAPTURE_ISR_PIN 32

  // // Section x: Button to toggle logging/sd state
  #define USE_MODULE_CORE_RULES
  // #define USE_MODULE_SENSORS_INTERFACE // NOT NEEDED
  #define USE_MODULE_SENSORS_BUTTONS
  // Next phase, will include saving adc values
  // #define USE_MODULE_SENSORS_ADC_INTERNAL
  #define USE_MODULE_SENSORS_ADC_I2S_INTERNAL
  #define ENABLE_SMOOTHING_ON_ADC_READING
  #define ENABLE_ADC_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
  #define ENABLE_ADC_I2S_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
  // #define ADC_CAPTURE_EXTERNAL_PIN 32
  #define ADC_CAPTURE_INPUT0_PIN 34
  #define ADC_CAPTURE_INPUT1_PIN 35

  // // Section x: OLED display 
  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SSD1306
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      /**
       * Debug pins
       * */

      /** 4P large JST - ADC
       * Yellow     34(I), ADC2G, ADC1_CH6
       * White      35(I), ADC5G, ADC1_CH7
       * Red        32(I), ADC Record Trigger
       * Black      GND
       * */
      "\"34\":\"" D_GPIO_FUNCTION_ADC1_CH6_CTR   "\","
      "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","
      "\"32\":\"" D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR   "\","
      // "\"32\":\"" D_GPIO_FUNCTION_GENERIC_GPIO1_CTR   "\","

      /** 5P small - UART2 RSS Stream
       * Orange      17, UART2_TX
       * Yellow      16, UART2_RX
       * White       25, ie superframe event over, rising edge interrupt
       * Red         5V
       * Black       GND
       * */
      "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
      "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","

      /** 5P small - UART1 GPS Stream
       * Orange      19, UART1_TX
       * Yellow      18, UART1_RX
       * White        
       * Red         VCC, 3V3
       * Black       GND
       * */
      "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
      "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","

      /** 6P small - SD Card
       * Green       15, CS
       * Orange      14, SCK
       * Yellow      13, MOSI
       * White       12, MISO
       * Red         3V3
       * Black       GND
       * */
      "\"15\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CSO_CTR   "\","
      "\"14\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CLK_CTR   "\","
      "\"13\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MOSI_CTR  "\","
      "\"12\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MISO_CTR  "\","

      /** Built in - OLED
       * 
       * */
      "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
      
      /** 2P small
       * Red        Button Logging Toggle
       * Black      GND
       * */
      "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";



  // debug
  #define DEBUG_PIN1_GPIO     21
  #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
  #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
  #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

  #define DEBUG_PIN2_GPIO     22
  #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
  #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
  #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));

  #define DEBUG_PIN3_GPIO     26
  #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
  #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
  #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

  #define DEBUG_PIN4_GPIO     27
  #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
  #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
  #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

  #define DEBUG_ADC_ISR_EVENT_SET(X)          DEBUG_PIN1_SET(X)
  //#define DEBUG_ADC_ISR_EVENT_SET(X)          // nothing


#endif // DEVICE_GPSPARSER_TESTER




/**
 * I2S ADC Tester
 * 
 * */
#ifdef DEVICE_I2S_ADC_TESTER
  #define DEVICENAME_CTR            "system_1_alpha"
  #define DEVICENAME_FRIENDLY_CTR   "Device 0"

  #define SERIAL_DEBUG_BAUD_DEFAULT 921600

  // General defines for debugging only
  #define DISABLE_NETWORK
  #define DISABLE_SLEEP

  // Section ADC: High speed internal ADC
  // #define ENABLE_ESP32_ADC_SAMPLING
  //   #define USE_MODULE_SENSORS_ADC_I2S_INTERNAL
  //   #define ENABLE_ADC_I2S_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
  //   #define ADC_CAPTURE_EXTERNAL_PIN 32 // Trigger sample period
  //   #define ADC_CAPTURE_INPUT0_PIN 34
  //   #define ADC_CAPTURE_INPUT1_PIN 35

  // General defines for debugging only, not for finished
  #define DISABLE_NETWORK
  #define DISABLE_SLEEP

  // // Section UART
  // #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_SERIAL_UART
  // #define ENABLE_HARDWARE_UART_1
  // #define ENABLE_HARDWARE_UART_2
  #define HARDWARE_UART_1_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
  #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000



  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      /** 2P small
       * Red        Button Logging Toggle
       * Black      GND
       * */
      "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  /**
   * Debug pins
   * */
  #define DEBUG_PIN1_GPIO     21
  #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
  #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
  #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

  #define DEBUG_PIN2_GPIO     22
  #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
  #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
  #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));

  #define DEBUG_PIN3_GPIO     26
  #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
  #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
  #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

  #define DEBUG_PIN4_GPIO     27
  #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
  #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
  #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

  #define DEBUG_ADC_ISR_EVENT_SET(X)          DEBUG_PIN1_SET(X)
  //#define DEBUG_ADC_ISR_EVENT_SET(X)          // nothing


#endif // DEVICE_GPSPARSER_TESTER



/**
 * Test of high speed I2S measurements
 * Record 1000 samples per syncframe (then sum the regions that would be measurements, or try offload for summing later in code)
 * On finish of sync, average the sample rate down to 10 times more than actual measurement rate, save these to sd if not too many
 * 
 * 
 * */
#ifdef DEVICE_TESTBED_I2S_MEASUREMENT
  #define DEVICENAME_CTR            "system_1_alpha"

  // #define DEVICENAME_FRIENDLY_CTR   "Device 2"
  // #define DEVICENAME_FRIENDLY_CTR   "Device 2"
  #define DEVICENAME_FRIENDLY_CTR   "DN0"  //white wire, blue tape, to be uav 

/**
 * SDCARD name
 * */

  // #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "DN0"  //white wire, blue tape, to be uav 
  #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "DN0"  //white wire, blue tape, to be uav 
  // #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "DN2"  //white wire, blue tape, to be uav 


  #define SERIAL_DEBUG_BAUD_DEFAULT 921600

  // Also defining so "HardwareSerial.cpp" will use these
  #define RX1 18
  #define TX1 19
  #define RX2 16
  #define TX2 17

  // General defines for debugging only, not for finished
  #define DISABLE_NETWORK
  #define DISABLE_SLEEP

  // General defines needed in release version
  #define ESP32
  #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

  // // // Section A: GPS
  // #define USE_MODULE_DRIVERS_GPS
  // #define ENABLE_GPS_PARSER_NMEA
  // #define ENABLE_GPS_PARSER_UBX
  // #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
  // #define NMEAGPS_DERIVED_TYPES
  // #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
  // #define NMEAGPS_PARSE_SAVE_MILLIS
  // #define gpsPort Serial1
  // #define D_GPS_BAUD_RATE_FAST    921600
  // #define D_GPS_BAUD_RATE_DEFAULT 9600

  // // // Section UART
  // #define USE_MODULE_DRIVERS_INTERFACE
  // #define USE_MODULE_DRIVERS_SERIAL_UART
  // #define ENABLE_HARDWARE_UART_1
  // #define ENABLE_HARDWARE_UART_2
  // #define HARDWARE_UART_1_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
  // #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
  // #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
  // #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
  // #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT

  // // Section B: SDCard driver
  // #define USE_MODULE_DRIVERS_SDCARD
  // #define USE_SDCARD_RINGBUFFER_STEAM_OUT

  // // // Seciton C: Logger controller
  // #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
  // // #define ENABLE_SDLOGGER_APPEND_TIME_TEST
  // #define ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
  // #define ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS
  // #define GPIO_SYNC_FRAME_ISR_PIN 25
  // #define ENABLE_INTERRUPT_ON_LOW_PINX_FOR_ADC_CAPTURE
  // #define GPIO_ADC_CAPTURE_ISR_PIN 32

  // // Section x: Button to toggle logging/sd state
  #define USE_MODULE_CORE_RULES
  // #define USE_MODULE_SENSORS_INTERFACE // NOT NEEDED
  #define USE_MODULE_SENSORS_BUTTONS

  #define ENABLE_ESP32_ADC_SAMPLING // not used in aplha
    // Next phase, will include saving adc values
    // #define USE_MODULE_SENSORS_ADC_INTERNAL
    #define USE_MODULE_SENSORS_ADC_I2S_INTERNAL
    #define ENABLE_SMOOTHING_ON_ADC_READING
    #define ENABLE_ADC_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
    #define ENABLE_ADC_I2S_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
    // #define ADC_CAPTURE_EXTERNAL_PIN 32
    #define ADC_CAPTURE_INPUT0_PIN 34
    #define ADC_CAPTURE_INPUT1_PIN 35

  // // Section x: OLED display 
  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SSD1306
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      /** 4P large JST - ADC
       * Yellow     34(I), ADC2G, ADC1_CH6
       * White      35(I), ADC5G, ADC1_CH7
       * Red        32(I), ADC Record Trigger
       * Black      GND
       * */
      "\"34\":\"" D_GPIO_FUNCTION_ADC1_CH6_CTR   "\","
      "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","
      "\"32\":\"" D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR   "\","
      /** 5P small - UART2 RSS Stream
       * Orange      17, UART2_TX
       * Yellow      16, UART2_RX
       * White       25, ie superframe event over, rising edge interrupt
       * Red         5V
       * Black       GND
       * */
      "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
      "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","
      /** 5P small - UART1 GPS Stream
       * Orange      19, UART1_TX
       * Yellow      18, UART1_RX
       * White        
       * Red         VCC, 3V3
       * Black       GND
       * */
      "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
      "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
      /** 6P small - SD Card
       * Green       15, CS
       * Orange      14, SCK
       * Yellow      13, MOSI
       * White       12, MISO
       * Red         3V3
       * Black       GND
       * */
      "\"15\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CSO_CTR   "\","
      "\"14\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CLK_CTR   "\","
      "\"13\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MOSI_CTR  "\","
      "\"12\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MISO_CTR  "\","
      /** Built in - OLED
       * 
       * */
      "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
      /** 2P small
       * Red        Button Logging Toggle
       * Black      GND
       * */
      "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  /**
   * Debug pins
   * */
  #define DEBUG_PIN1_GPIO     21
  #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
  #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
  #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

  #define DEBUG_PIN2_GPIO     22
  #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
  #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
  #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));

  #define DEBUG_PIN3_GPIO     26
  #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
  #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
  #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

  #define DEBUG_PIN4_GPIO     27
  #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
  #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
  #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

  #define DEBUG_ADC_ISR_EVENT_SET(X)          DEBUG_PIN1_SET(X)
  //#define DEBUG_ADC_ISR_EVENT_SET(X)          // nothing


#endif // DEVICE_GPSPARSER_TESTER



/**
 * 
 * Test of high speed I2S measurements
 * 
 * How it works:
 * - sync pin will reset the item counter, which is an array that stores an ID number for each item in the ringbuffer
 *    - reading the item from the buffer, will also mean popping the value out of it. I will use a SECOND ringbuffer for just this purpose
 * - sync pin keeps track of which ringbuffer items belongs to each group, so I need a counter for sync (in a ringbuffer) and a counter for RXON state changes
 * 
 * - When RXON pin is active low, then the I2S samples will be commited to the ringbuffer
 * - The values per item (DMA read), will still be averaged in 10s, ie 1000 samples becomes 100 samples per item
 *
 * 
 * 
 * 
 * 
 * 
 * Record 1000 samples per syncframe (then sum the regions that would be measurements, or try offload for summing later in code)
 * On finish of sync, average the sample rate down to 10 times more than actual measurement rate, save these to sd if not too many
 * 
 * 
 * 
 * 

sync period will trigger the i2s to run, and when X samples are recorded, will stop... 
This will be known as a frame, I can post downsample and remove other things after



sample 5.8ghz with esp32
have sync pulse restart counter, so I know which "item" is the start of that sync window
during capture constant capture, only commit to memory when a pin is low, which signifies "active rxon period"
There should be 40 of these windows per sync

sync_counter = [0,0,0,0,0 1,1,1,,,,,,,, 40,40,40 ] where X for each number will be many samples, which I can post downsample in matlab



 * 
 * */
#ifdef DEVICE_TESTBED_I2S_MEASUREMENT_SINGLE_CHANNEL
  #define DEVICENAME_CTR            "system_1_alpha"

  // #define DEVICENAME_FRIENDLY_CTR   "Device 2"
  // #define DEVICENAME_FRIENDLY_CTR   "Device 2"
  #define DEVICENAME_FRIENDLY_CTR   "DN0"  //white wire, blue tape, to be uav 

  /**
   * New defines to enable functions below in the way I need for them to work (ie cross enable tasks where needed)
   * */
  // #define USE_SYSTEM_BUTTON_TO_TOGGLE_SDCARD_LOGGING
  #define USE_SYSTEM_MAIN_LOGGER_CONTROLLER
  #define USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER

  /**
   * Debug methods
   * */
  #define USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_UART_ESP32_OUTPUT

  /**
   * Since I have no comms from esp32 to pic32, I should use this for debugging (disconnect from esp32)
   * Just on the debug lipo-esp32 (front soldered)
   * Use this to send "SDCARD" data directly to PC to logging to file and parsing.
   * */

  /**
   * Required defines based on above defines
   * */
  #ifdef USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_UART_ESP32_OUTPUT
    // #define USE_MODULE_DRIVERS_SERIAL_UART
  #endif



  /**
   * SDCARD name
   * */

  // #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "DN0"  //white wire, blue tape, to be uav 
  #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "DN0"  //white wire, blue tape, to be uav 
  // #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "DN2"  //white wire, blue tape, to be uav 


  #define SERIAL_DEBUG_BAUD_DEFAULT 921600

  // Also defining so "HardwareSerial.cpp" will use these
  #define RX1 18
  #define TX1 19
  #define RX2 16
  #define TX2 17

  // General defines for debugging only, not for finished
  #define DISABLE_NETWORK
  #define DISABLE_SLEEP

  // General defines needed in release version
  #define ESP32
  #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

  // // // Section A: GPS
  // #define USE_MODULE_DRIVERS_GPS
  // #define ENABLE_GPS_PARSER_NMEA
  // #define ENABLE_GPS_PARSER_UBX
  // #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
  // #define NMEAGPS_DERIVED_TYPES
  // #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
  // #define NMEAGPS_PARSE_SAVE_MILLIS
  // #define gpsPort Serial1
  // #define D_GPS_BAUD_RATE_FAST    921600
  // #define D_GPS_BAUD_RATE_DEFAULT 9600

  // // // Section UART
  // #define USE_MODULE_DRIVERS_INTERFACE
  // #define USE_MODULE_DRIVERS_SERIAL_UART
  // #define ENABLE_HARDWARE_UART_1
  // #define ENABLE_HARDWARE_UART_2
  // #define HARDWARE_UART_1_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
  // #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
  // #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
  // #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
  // #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT

  // // Section B: SDCard driver
  // #define USE_MODULE_DRIVERS_SDCARD
  // #define USE_SDCARD_RINGBUFFER_STEAM_OUT

  // // // Seciton C: Logger controller
  #ifdef USE_SYSTEM_MAIN_LOGGER_CONTROLLER
    #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
    #define ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
    #define ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS
    #define PIN_GPIO_FUNCTION_SYNC_FRAME_ISR_PIN_NUM 25
    // #define ENABLE_INTERRUPT_ON_LOW_PINX_FOR_ADC_CAPTURE
    // #define GPIO_ADC_CAPTURE_ISR_PIN 32
  #endif // USE_SYSTEM_MAIN_LOGGER_CONTROLLER

  // // Section x: Button to toggle logging/sd state
  // #define USE_MODULE_CORE_RULES
  // // #define USE_MODULE_SENSORS_INTERFACE // NOT NEEDED
  // #define USE_MODULE_SENSORS_BUTTONS


  /***
   * ESP32    wire colour       PIC32                     PURPOSE
   *  25             
   *  33
   *  32         red               D1                       PIN_OUTPUT_ESP32_TRIGGER_ADC_SAMPLE_TIMESLOT_PERIOD
   *  35        yellow             B1 - RSS 5800            AD1_CH7
   *  34        white              D7                       PIN_OUTPUT_ESP32_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD
   * 
   * */
    #define ENABLE_INTERRUPT_ON_CHANGE_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD


    #define PIN_GPIO_FUNCTION_RXON_SAMPLING_ENABLED_NUM 32    // This allows me to call these directly, instead of via pinmethod, so faster    
    #define PIN_GPIO_FUNCTION_CC1110_SYNC_PULSE_SIGNAL_NUM 33    // This allows me to call these directly, instead of via pinmethod, so faster
    #define PIN_GPIO_FUNCTION_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD_NUM 26    // This allows me to call these directly, instead of via pinmethod, so faster
    #define PIN_GPIO_FUNCTION_RXON_SAMPLE_5800_ADC_NUM         35

    #define ENABLE_INTERRUPT_ON_CHANGE_PIN35_FOR_RXON_SAMPLING_TIMESLOT


  #ifdef USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER
    #define USE_MODULE_SENSORS_ADC_I2S_INTERNAL

  #endif // USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER

    
  // // Section x: OLED display 
  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SSD1306
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      /** 4P large JST - ADC
       * Yellow     34(I), ADC5G, ADC1_CH6
       * White      35(I), RXON SAMPLING ENABLED (ie region of sampling on)
       * Red        32(I), Start of Sync period (resetting this windows values)
       * Black      GND
       * 
       * */
      //"\"" STR2(PIN_GPIO_FUNCTION_CC1110_SYNC_PULSE_SIGNAL_NUM) "\":\"" D_GPIO_FUNCTION_CC1110_SYNC_PULSE_SIGNAL_CTR   "\","  // 33 = Sync period from cc1110, this is used to reset the sampler (maybe change to another ringbuffer?, have two, one per active and writer)
      //"\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","               // 5.8 GHz RSS 


//35 is the new ADC

     // "\"32\":\"" D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR   "\","   // pic32 tells the device when a new sync window starts, I will probably use this for swapping buffers
      //"\"" STR2(PIN_GPIO_FUNCTION_RXON_SAMPLING_ENABLED_NUM) "\":\"" D_GPIO_FUNCTION_RXON_SAMPLING_ENABLED_CTR   "\","  // When low, samples are collected. An interrupt will also be applied to it, where state change will increment a counter (ie new time slot)

      /** 5P small - UART2 RSS Stream
       * Orange      17, UART2_TX
       * Yellow      16, UART2_RX
       * White       25, ie superframe event over, rising edge interrupt
       * Red         5V
       * Black       GND
       * */
      "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
      "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","
      /** 5P small - UART1 GPS Stream
       * Orange      19, UART1_TX
       * Yellow      18, UART1_RX
       * White        
       * Red         VCC, 3V3
       * Black       GND
       * */
      "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
      "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
      /** 6P small - SD Card
       * Green       15, CS
       * Orange      14, SCK
       * Yellow      13, MOSI
       * White       12, MISO
       * Red         3V3
       * Black       GND
       * */
      "\"15\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CSO_CTR   "\","
      "\"14\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CLK_CTR   "\","
      "\"13\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MOSI_CTR  "\","
      "\"12\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MISO_CTR  "\","
      /** Built in - OLED
       * 
       * */
      "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
      /** 2P small
       * Red        Button Logging Toggle
       * Black      GND
       * */
      "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  /**
   * Debug pins
   * */
  // #define DEBUG_PIN1_GPIO     21
  // #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
  // #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
  // #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

  // #define DEBUG_PIN2_GPIO     22
  // #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
  // #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
  // #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));

  // #define DEBUG_PIN3_GPIO     26
  // #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
  // #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
  // #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

  // #define DEBUG_PIN4_GPIO     27
  // #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
  // #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
  // #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

  #define DEBUG_ADC_ISR_EVENT_SET(X)          DEBUG_PIN1_SET(X)
  //#define DEBUG_ADC_ISR_EVENT_SET(X)          // nothing


#endif // DEVICE_GPSPARSER_TESTER







/**
 * First measurement system, without esp adc until I can resolve that
 * This will be used for first aplha tests to plan things out
 * Future system will attempt to include esp adc later
 * 
 * Description: Use of button, oled and sdcard together on LIPO_ESP32
 * Button will toggle logging start/end (open/close), which will write the RTC time line by line. This will let me test sd opening and closing when expected
 * Test version before adding pic32 stream of data (although will use same format)
 * Aim: To log GPS data at 10hz, to sd card, and have me drive around and get that 3d fix saved and into matlab then google earth
 * 
 * GPS @ 912800 baud recorded to local fix
 * - each new fix, triggers a "save" flag
 * OLED screen will show bytes written every second
 * No networking
 * 
 * 
 * 
 * */
#ifdef DEVICE_MEASUREMENT_SYSTEM_ALPHA_1
  #define DEVICENAME_CTR            "system_1_alpha"

  // #define DEVICENAME_FRIENDLY_CTR   "Device 2"
  // #define DEVICENAME_FRIENDLY_CTR   "Device 2"
  #define DEVICENAME_FRIENDLY_CTR   "DN0"  //white wire, blue tape, to be uav 

/**
 * SDCARD name
 * */

  // #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "DN0"  //white wire, blue tape, to be uav 
  #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "DN0"  //white wire, blue tape, to be uav 
  // #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "DN2"  //white wire, blue tape, to be uav 


  #define SERIAL_DEBUG_BAUD_DEFAULT 921600

  // Also defining so "HardwareSerial.cpp" will use these
  #define RX1 18
  #define TX1 19
  #define RX2 16
  #define TX2 17

  // General defines for debugging only, not for finished
  #define DISABLE_NETWORK
  #define DISABLE_SLEEP

  // General defines needed in release version
  #define ESP32
  #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

  // // Section A: GPS
  #define USE_MODULE_DRIVERS_GPS
  #define ENABLE_GPS_PARSER_NMEA
  #define ENABLE_GPS_PARSER_UBX
  #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
  #define NMEAGPS_DERIVED_TYPES
  #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
  #define NMEAGPS_PARSE_SAVE_MILLIS
  #define gpsPort Serial1
  #define D_GPS_BAUD_RATE_FAST    921600
  #define D_GPS_BAUD_RATE_DEFAULT 9600

  // // Section UART
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_SERIAL_UART
  #define ENABLE_HARDWARE_UART_1
  #define ENABLE_HARDWARE_UART_2
  #define HARDWARE_UART_1_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
  #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
  #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
  #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
  #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT

  // Section B: SDCard driver
  #define USE_MODULE_DRIVERS_SDCARD
  #define USE_SDCARD_RINGBUFFER_STEAM_OUT

  // // Seciton C: Logger controller
  #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
  // #define ENABLE_SDLOGGER_APPEND_TIME_TEST
  #define ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
  #define ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS
  #define GPIO_SYNC_FRAME_ISR_PIN 25
  #define ENABLE_INTERRUPT_ON_LOW_PINX_FOR_ADC_CAPTURE
  #define GPIO_ADC_CAPTURE_ISR_PIN 32

  // // Section x: Button to toggle logging/sd state
  #define USE_MODULE_CORE_RULES
  // #define USE_MODULE_SENSORS_INTERFACE // NOT NEEDED
  #define USE_MODULE_SENSORS_BUTTONS
  // #define ENABLE_ESP32_ADC_SAMPLING // not used in aplha
    // // Next phase, will include saving adc values
    // // #define USE_MODULE_SENSORS_ADC_INTERNAL
    // #define USE_MODULE_SENSORS_ADC_I2S_INTERNAL
    // #define ENABLE_SMOOTHING_ON_ADC_READING
    // #define ENABLE_ADC_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
    // #define ENABLE_ADC_I2S_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
    // // #define ADC_CAPTURE_EXTERNAL_PIN 32
    // #define ADC_CAPTURE_INPUT0_PIN 34
    // #define ADC_CAPTURE_INPUT1_PIN 35

  // Section x: OLED display 
  #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_OLED_SSD1306
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      /** 4P large JST - ADC
       * Yellow     34(I), ADC2G, ADC1_CH6
       * White      35(I), ADC5G, ADC1_CH7
       * Red        32(I), ADC Record Trigger
       * Black      GND
       * */
      "\"34\":\"" D_GPIO_FUNCTION_ADC1_CH6_CTR   "\","
      "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","
      "\"32\":\"" D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR   "\","
      /** 5P small - UART2 RSS Stream
       * Orange      17, UART2_TX
       * Yellow      16, UART2_RX
       * White       25, ie superframe event over, rising edge interrupt
       * Red         5V
       * Black       GND
       * */
      "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
      "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","
      /** 5P small - UART1 GPS Stream
       * Orange      19, UART1_TX
       * Yellow      18, UART1_RX
       * White        
       * Red         VCC, 3V3
       * Black       GND
       * */
      "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
      "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
      /** 6P small - SD Card
       * Green       15, CS
       * Orange      14, SCK
       * Yellow      13, MOSI
       * White       12, MISO
       * Red         3V3
       * Black       GND
       * */
      "\"15\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CSO_CTR   "\","
      "\"14\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CLK_CTR   "\","
      "\"13\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MOSI_CTR  "\","
      "\"12\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MISO_CTR  "\","
      /** Built in - OLED
       * 
       * */
      "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
      /** 2P small
       * Red        Button Logging Toggle
       * Black      GND
       * */
      "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  /**
   * Debug pins
   * */
  #define DEBUG_PIN1_GPIO     21
  #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
  #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
  #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

  #define DEBUG_PIN2_GPIO     22
  #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
  #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
  #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));

  #define DEBUG_PIN3_GPIO     26
  #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
  #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
  #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

  #define DEBUG_PIN4_GPIO     27
  #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
  #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
  #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

  #define DEBUG_ADC_ISR_EVENT_SET(X)          DEBUG_PIN1_SET(X)
  //#define DEBUG_ADC_ISR_EVENT_SET(X)          // nothing


#endif // DEVICE_GPSPARSER_TESTER





#endif // MSYSTEMCONFIG_HARDWAREDEFAULTS_H

