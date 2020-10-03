#ifndef MSYSTEMCONFIG_H
#define MSYSTEMCONFIG_H

// File contains settings for the project that should not be edited by user

#include "stdint.h"
// DEVICE named defines that set modules to use below
#include "1_ConfigUser/mFirmwareCustom.h"

enum FIRMWARE_VERSION_BRANCH_TYPE_IDS{
  FIRMWARE_VERSION_TYPE_RELEASE_ID = 0,  FIRMWARE_VERSION_TYPE_RELEASE_CANDIDATE_ID,
  FIRMWARE_VERSION_TYPE_BETA_ID,  FIRMWARE_VERSION_TYPE_DEVELOPING_ID
};

#define FIRMWARE_VERSION_TYPE     FIRMWARE_VERSION_TYPE_DEVELOPING_ID
#define FIRMWARE_VERSION_MAJOR    0
#define FIRMWARE_VERSION_MINOR    78
#define FIRMWARE_VERSION_CORE     23
#define FIRMWARE_VERSION_MODULE   10

#ifndef SETTINGS_HOLDER
#define SETTINGS_HOLDER           1//                (random(1,1000))
#endif

// #define ENABLE_BUG_TRACING

// BUG TRACING
#ifdef ENABLE_BUG_TRACING
  #define ENABLE_SERIAL_FLUSH
  #define ENABLE_ADVANCED_DEBUGGING // new feature that only includes lowest level debugging when needed
  #define DEBUG_SERIAL_TESTING // Sets debug level of serial, else info for performance improvements
  #define DEBUG_MQTT_HANDLER
  #define DEBUG_FOR_FAULT
  #define USE_DEBUG_LINE  
  #define USE_DEBUG_LINE_LED
  #define ENABLE_DELAYED_BOOT
  
  #define ENABLE_LOG
  #define ENABLE_LOG_LEVEL_DEBUG // Addlog of debug level or lower
  #define ENABLE_LOG_LEVEL_INFO  // General useful info, will be enabled at most times

#endif
#ifdef ENABLE_BUG_TRACING2
  // #define ENABLE_SERIAL_FLUSH
  //#define ENABLE_ADVANCED_DEBUGGING // new feature that only includes lowest level debugging when needed
  // #define DEBUG_SERIAL_TESTING // Sets debug level of serial, else info for performance improvements
  // #define DEBUG_MQTT_HANDLER
  // #define DEBUG_FOR_FAULT
  // #define USE_DEBUG_LINE  
  // #define USE_DEBUG_LINE_LED
  #define ENABLE_DELAYED_BOOT
  
  // #define ENABLE_LOG
  // #define ENABLE_LOG_LEVEL_DEBUG // Addlog of debug level or lower
  // #define ENABLE_LOG_LEVEL_INFO  // General useful info, will be enabled at most times

#endif

#define ENABLE_LOG
#define ENABLE_LOG_LEVEL_DEBUG // Addlog of debug level or lower
#define ENABLE_LOG_LEVEL_INFO  // General useful info, will be enabled at most times

#define ENABLE_SETTINGS_STORAGE

#define DEBUG_WEBSERVER_LIB  //used in improving core lib
#define DEBUG_WEBSERVER_MEMORY


// Muted blue/green
#define COLOR_BUTTON_HIGHLIGHT      "#1be7f1"
#define COLOR_BUTTON                "#4d8d90"    // [WebColor11] Button color - Blueish
#define COLOR_BUTTON_HOVER          "#31595b"
#define D_COLOUR_PAGE_TITLE         "#4bb7e6"
#define D_COL_TEXT_MODULE_TITLE_CTR "#4bb7e6"

#define ENABLE_USER_CONFIG_OVERRIDE

/**
 *  Extra Debugging methods
 **/
#define ENABLE_FREERAM_APPENDING_SERIAL
// #define ADD_DEBUG_PERIODS_FOR_SERIAL_MONITORING
// #ifdef ENABLE_ADVANCED_DEBUGGING
//   #define DEBUG_FOR_FAULT
// #endif


//--------------------------------[Defines needed here]-------------------------------------

// Project version is bit-packed as follows
// bits 31-30  (2)  (Range 0-4)     Type/Branch (e.g. Stable, Developing)
// bits 29-25  (6)  (Range 0-77)    Major version 
// bits 24-16  (8)  (Range 0-255)   Minor version 
// bits 15-8   (8)  (Range 0-255)   System version (e.g. Wifi, Support)
// bits 7-0    (8)  (Range 0-255)   Module version (e.g. Sensors, Drivers)

#define FIRMWARE_VERSION_TYPE_MINIMAL     FIRMWARE_VERSION_TYPE_DEVELOPING_ID
#define FIRMWARE_VERSION_MAJOR_MINIMAL    0
#define FIRMWARE_VERSION_MINOR_MINIMAL    70
#define FIRMWARE_VERSION_CORE_MINIMAL     0
#define FIRMWARE_VERSION_MODULE_MINIMAL   0

// Generate Version AFTER Hardware defaults have been called, this allows undef VERSION_TYPE for overriding
const uint32_t PROJECT_VERSION = ((FIRMWARE_VERSION_TYPE & 0x03) << 30) | ((FIRMWARE_VERSION_MAJOR & 0x3F) << 24) | (FIRMWARE_VERSION_MINOR << 16) | (FIRMWARE_VERSION_CORE << 8) | (FIRMWARE_VERSION_MODULE);
const uint32_t PROJECT_VERSION_MINIMAL = ((FIRMWARE_VERSION_TYPE_MINIMAL & 0x03) << 30) | ((FIRMWARE_VERSION_MAJOR_MINIMAL & 0x3F) << 24) | (FIRMWARE_VERSION_MINOR_MINIMAL << 16) | (FIRMWARE_VERSION_CORE_MINIMAL << 8) | (FIRMWARE_VERSION_MODULE_MINIMAL);

//--------------------------------[AUTOSET - DEFINE HARDWARE needed to run tasks requested]-------------------------------------

#ifdef DEVICE_FORCED_TO_BE_TESTER
  #undef DEVICENAME_CTR
  #define DEVICENAME_CTR "testerdevice"
#endif

// #ifdef ENABLE_TRACING
//   #define ARDUINOTRACE_ENABLE_FULLPATH
//   #define ARDUINOTRACE_ENABLE
//   #include <ArduinoTrace.h>
// #endif


#define PROJECT_NAME_CTR "Home Automation Control System (HACS)"
// #ifndef DEVICENAME_FRIENDLY_CTR
//   #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
// #endif
#ifdef DEVICE_FORCED_TO_BE_TESTER
  #warning "DEVICE_FORCED_TO_BE_TESTER is set!"
#endif



#endif


