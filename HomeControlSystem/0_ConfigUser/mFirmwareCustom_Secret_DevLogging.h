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
#define DEVICE_UART_GPS_TESTER

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
 * 3) Serial2 is GPS input (9600)
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
   *  - Putty Serial inputs into Serial2 buffer
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




#endif // MSYSTEMCONFIG_HARDWAREDEFAULTS_H

