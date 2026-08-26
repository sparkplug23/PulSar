/**
 * @file mBaseConfig.h
 * @brief PulSar default build configuration.
 *
 * This file contains the project-level defaults required for normal builds.
 * Keep this file focused on PulSar defaults. Do not bulk-merge the full Tasmota
 * my_user_config.h option set.
 *
 * User/device-specific overrides should live in:
 *   - 0_ConfigUser/G0_mFirmwareCustom_Secret.h
 *   - 0_ConfigUser/G1_mUserConfig_Secret.h
 *
 * Long-term shared debug features and safe default compile-time options may be
 * added here.
 */

#ifndef MUSERCONFIG_DEFAULTS_H
#define MUSERCONFIG_DEFAULTS_H

#include "stdint.h"
#include "2_CoreSystem/Version.h"
#include "0_ConfigUser/G0_mFirmwareCustom_Secret.h"


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * SECTION: 2_CORE_SYSTEM
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 00_ConfigEntryPoints
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

#define USE_CONFIG_OVERRIDE                      // Include 0_ConfigUser/G1_mUserConfig_Secret.h at the end of this file.
#define ENABLE_USER_CONFIG_OVERRIDE
#define USE_MODULE_TEMPLATE__OVERRIDE            // CRITICAL: SettignsLoad not yet stable, must override with compile-time template to ensure GPIO mapping and device identity are established before GpioInit() and TASK_PRE_INIT

/*********************************************************************************************\
 * This MUST BE CHANGED for ANY parameters in this file to overwrite those saved in settings
\*********************************************************************************************/
#ifndef SETTINGS_HOLDER
#define SETTINGS_HOLDER             1              // [Reset 1] Change this value (max 32000) to load SECTION1 configuration parameters to flash
#endif

// #define ENABLE_FEATURE_SETTINGS__IGNORE_USER_DEFINED_FILES // [optional] Disable user defined settings to always use hardcoded compile time values


#define SERIALD Serial //serial debug
#ifndef SERIAL_DEBUG_BAUD_DEFAULT
#define SERIAL_DEBUG_BAUD_DEFAULT 115200
#endif // SERIAL_DEBUG_BAUD_DEFAULT

#define ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE

/*********************************************************************************************\
 * CAUTION! Only select from values defined in the comments next to (right hand side) the parameter. 
 * Some protection is added on value ranges, but it is not guarenteed especially during the development phase.
 * 
 * The names of parameters are suffixed by a shorthand for the type of value it MUST be adhered to:
 * "_CTR"     String (word or phrase) eg. "ON" or "This Value"
 * "_NUM"     Number that is an integer only (no decimel points!) eg 1 or 1023
 * "_FLT"     Float number, which is any number that MAY include a decimel, but does not need to. eg. 1 or 1.0 or 1.234
 * "_ID"      Same as "_NUM"
\*********************************************************************************************/

  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR            "template"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR    DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #ifndef DEVICENAME_ROOMHINT_CTR
  #define DEVICENAME_ROOMHINT_CTR   "template"
  #endif

  // 
  // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #ifdef ESP32
  #define USE_MODULE_CORE__CRASH_RECORDER
  #endif

  #ifndef ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE
  #define ENABLE_FEATURE_FASTBOOT__DETECTION
  #endif
  
  #define USE_MODULE_CORE__JSON_TEMPLATE
  #define USE_MODULE_CORE_TIME
  #define USE_MODULE_CORE_SPI
  #define USE_MODULE_CORE_I2C
  #define USE_MODULE_CORE_PWM

  // #ifdef ESP32
  #ifndef DISABLE_FILESYSTEM
  #define USE_MODULE_CORE_FILESYSTEM
  // #pragma message "Filesystem enabled by default, make sure to define DISABLE_FILESYSTEM if you do not want this"
  #endif
  // #endif



  #define ENABLE_FEATURE_JSON__ASYNCJSON_V6


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 01_Settings
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

/*********************************************************************************************\
 *  Module Settings
\*********************************************************************************************/
#define MODULE                        MODULE_DEFAULT      // [Module] Select default model from mHardwareTemplates.h

#define SAVE_DATA                     1                 // [SaveData] Save changed parameters to Flash (0 = disable, 1 - 3600 seconds)
#define SAVE_STATE                    1                 // [SetOption0] Save changed power state to Flash (0 = disable, 1 = enable)
#define BOOT_LOOP_OFFSET              1                 // [SetOption36] Number of boot loops before starting restoring defaults (0 = disable, 1..200 = boot loops offset)

// Forcing this to disable until all devices are optimised so nothing is starved of CPU 
#define DISABLE_SLEEP

#if defined(ESP32)
#ifndef ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE
#define ENABLE_FEATURE_WATCHDOG_TIMER
#define D_WATCHDOG_TIMER_TIMEOUT_PERIOD_MS 60000
#define ENABLE_FEATURE_FASTBOOT__DETECTION

#define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID
#endif
#endif
#ifdef ESP8266
#ifndef ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE
#define ENABLE_FEATURE_WATCHDOG_TIMER
#define D_WATCHDOG_TIMER_TIMEOUT_PERIOD_MS 60000
#define ENABLE_FEATURE_FASTBOOT__DETECTION

#define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID
#endif
#endif


#define ENABLE_FEATURE_WEBSERVER__SYSTEM_CONTROLS

#define USE_MODULE_SENSORS_ESP32_TEMPERATURE
#define ENABLE_FEATURE_INTERNAL_CHIP_TEMPERATURE__ALLOW_CLASSIC_ESP32





#define ENABLE_FEATURE_DRIVERS_INTERFACE_UNIFIED_DRIVER_REPORTING


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 03_FileSystem
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

#ifdef ESP32
// #define USE_MODULE_DRIVERS_SDCARD

  #define ENABLE_FEATURE_FILESYSTEM__SDCARD_MMC // keep pn, planned also on
#endif


/*********************************************************************************************\
 * File system
\*********************************************************************************************/


#define UFS_FILE_WRITE              "w"
#define UFS_FILE_READ               "r"
#define FS_FILE_WRITE               "w"
#define FS_FILE_READ                "r"
#define FS_FILE_APPEND              "a"

// // #define FILENAME__SETTINGS__SYSTEM_BLOCK          "/.settings"       // Settings binary blob
#define FILENAME__SETTINGS__SYSTEM_BLOCK          "/settings.txt"       // Settings binary blob as text so I can view on webui
// // #define FILENAME__SETTINGS__LKG_LAST_KNOWN_GOOD      "/.settings.lkg"   // Last Known Good Settings binary blob
#define FILENAME__SETTINGS__LKG_LAST_KNOWN_GOOD      "/settings_lkg.txt"   // Last Known Good Settings binary blob

// #define TASM_FILE_DRIVER            "/.drvset%03d"
// #define TASM_FILE_SENSOR            "/.snsset%03d"
// #define TASM_FILE_TLSKEY            "/tlskey"          // TLS private key
// #define TASM_FILE_ZIGBEE_LEGACY_V2  "/zb"              // Zigbee devices information blob, legacy v2
// #define TASM_FILE_ZIGBEE            "/zbv4"            // Zigbee devices information blob, now v4
// #define TASM_FILE_ZIGBEE_DATA       "/zbdata"          // Zigbee last known values of devices
// #define TASM_FILE_AUTOEXEC          "/autoexec.bat"    // Commands executed after restart
// #define TASM_FILE_CONFIG            "/config.sys"      // Settings executed after restart


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 04_JsonTemplate
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

/*********************************************************************************************\
 *
 * Overrides for creating cloned test devices and renaming the templates
 * Prefix
 * 
 * 
 * 
 * 
 * 
 * *****************************************************************************************************/
// Store original defines into intermediate macros if USE_DEBUGFEATURE_DEVICE_CLONE_TESTBED is defined
// #ifdef USE_DEBUGFEATURE_DEVICE_CLONE_TESTBED
// // Directly redefine the macros with the prefix
// #undef DEVICENAME_CTR
// #undef DEVICENAME_FRIENDLY_CTR
// #undef DEVICENAME_DESCRIPTION_CTR
// #undef DEVICENAME_ROOMHINT_CTR

// // Use stringification to directly concatenate
// #define DEVICENAME_CTR          "tb_" DEVICENAME_CTR
// #define DEVICENAME_FRIENDLY_CTR "tb_" DEVICENAME_FRIENDLY_CTR
// #define DEVICENAME_DESCRIPTION_CTR "tb_" DEVICENAME_DESCRIPTION_CTR
// #define DEVICENAME_ROOMHINT_CTR "tb_" DEVICENAME_ROOMHINT_CTR
// #endif
// #pragma message ("DEVICENAME_CTR: " DEVICENAME_CTR)


#define USE_FUNCTION_TEMPLATE__OVERRIDE
#define USE_LIGHTING_TEMPLATE__OVERRIDE
#define USE_RULES_TEMPLATE__OVERRIDE
#define USE_NEXTION_HMI_INPUT_MAP_TEMPLATE__OVERRIDE
#define USE_DISPLAY_TEMPLATE__OVERRIDE
#define USE_DISPLAY_HMI_TEMPLATE__OVERRIDE
#define USE_DRIVERS_TEMPLATE__OVERRIDE
#define USE_SENSORS_TEMPLATE__OVERRIDE
#define USE_CONTROLLER_TEMPLATE__OVERRIDE
#define USE_NETWORK_TEMPLATE__OVERRIDE
#define USE_SYSTEM_TEMPLATE__OVERRIDE

/*********************************************************************************************\
 *  Templates
\*********************************************************************************************/
#define ENABLE_FEATURE_TEMPLATES__LOAD_FROM_PROGMEM_TO_OVERRIDE_STORED_SETTINGS_TO_MAINTAIN_KNOWN_WORKING_VALUES

#define ENABLE_DEBUGFEATURE__FILESYSTEM__LOAD_HARDCODED_TEMPLATES_INTO_FILESYSTEM


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 05_Pins
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 06_Support
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// -- Application ---------------------------------
#define APP_TIMEZONE           99                 // [Timezone] +1 hour (Amsterdam) (-13 .. 14 = hours from UTC, 99 = use TIME_DST/TIME_STD)
#define APP_LEDSTATE           LED_POWER         // [LedState] Function of led
                                                  // (LED_OFF, LED_POWER, LED_MQTTSUB, LED_POWER_MQTTSUB, LED_MQTTPUB, LED_POWER_MQTTPUB, LED_MQTT, LED_POWER_MQTT)
#define APP_LEDMASK            0xFFFF            // [LedMask] Assign Relay to Power led (0xFFFF is default)
#define APP_PULSETIME          0                 // [PulseTime] Time in 0.1 Sec to turn off power for relay 1 (0 = disabled)
#define APP_POWERON_STATE      POWER_ALL_SAVED   // [PowerOnState] Power On Relay state
                                                 //  (POWER_ALL_OFF, POWER_ALL_ON, POWER_ALL_SAVED_TOGGLE, POWER_ALL_SAVED, POWER_ALL_ALWAYS_ON, POWER_ALL_OFF_PULSETIME_ON)
#define APP_BLINKTIME          10                // [BlinkTime] Time in 0.1 Sec to blink/toggle power for relay 1
#define APP_BLINKCOUNT         10                // [BlinkCount] Number of blinks (0 = 32000)
#define DEFAULT_LOOP_SLEEP     20                // [Sleep] Sleep time to lower energy consumption (0 = Off, 1 - 250 mSec)

#define KEY_CHECK_TIME         1000
#define KEY_DEBOUNCE_TIME      50                // [ButtonDebounce] Number of mSeconds button press debounce time
#define KEY_HOLD_TIME          25                // [SetOption32] Number of 0.1 seconds to hold Button or external Pushbutton before sending HOLD message

#ifndef SWITCH_DEBOUNCE_TIME
#define SWITCH_DEBOUNCE_TIME   50                // [SwitchDebounce] Number of mSeconds switch press debounce time
#endif
#ifndef SWITCH_MODE
#define SWITCH_MODE            SWITCHMODE_TOGGLE_ID            // [SwitchMode] TOGGLE, FOLLOW, FOLLOW_INV, PUSHBUTTON, PUSHBUTTON_INV, PUSHBUTTONHOLD, PUSHBUTTONHOLD_INV, PUSHBUTTON_TOGGLE (the wall switch state)
#endif

/*********************************************************************************************\
 * ESP32 only features
\*********************************************************************************************/

#ifdef ESP32

#define SET_ESP32_STACK_SIZE  (8 * 1024)         // Set the stack size for Tasmota. The default value is 8192 for Arduino, some builds might need to increase it

#ifdef SOC_TOUCH_VERSION_1                       // ESP32
  #define ESP32_TOUCH_THRESHOLD   40             // [TouchThres] Below this level a touch is detected
#endif
#ifdef SOC_TOUCH_VERSION_2                       // ESP32-S2 and ESP32-S3
  #define ESP32_TOUCH_THRESHOLD   40000          // [TouchThres] Above this level a touch is detected
#endif

#endif // ESP32


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 07_Time
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

#define ENABLE_FEATURE_RTC__SETTINGS

// -- Time - Up to three NTP servers in your region
#define NTP_SERVER2            "pool.ntp.org"       // [NtpServer1] automatically redirects your request to a geographically close server, ensuring low latency and accurate time. 
#define NTP_SERVER1            "ie.pool.ntp.org"    // [NtpServer2] This directs your request specifically to servers within Ireland.
#define NTP_SERVER3            "europe.pool.ntp.org"  // This directs your request to servers located in Europe.




// -- Time - Start Daylight Saving Time and timezone offset from UTC in minutes
#define TIME_DST_HEMISPHERE    North             // [TimeDst] Hemisphere (0 or North, 1 or South)
#define TIME_DST_WEEK          Last              // Week of month (0 or Last, 1 or First, 2 or Second, 3 or Third, 4 or Fourth)
#define TIME_DST_DAY           Sun               // Day of week (1 or Sun, 2 or Mon, 3 or Tue, 4 or Wed, 5 or Thu, 6 or Fri, 7 or Sat)
#define TIME_DST_MONTH         Mar               // Month (1 or Jan, 2 or Feb, 3 or Mar, 4 or Apr, 5 or May, 6 or Jun, 7 or Jul, 8 or Aug, 9 or Sep, 10 or Oct, 11 or Nov, 12 or Dec)
#define TIME_DST_HOUR          2                 // Hour (0 to 23)
#ifndef TIME_DST_OFFSET
#define TIME_DST_OFFSET        60                // Offset from UTC in minutes (-780 to +780)
#endif 

// -- Time - Start Standard Time and timezone offset from UTC in minutes
#define TIME_STD_HEMISPHERE    North             // [TimeStd] Hemisphere (0 or North, 1 or South)
#define TIME_STD_WEEK          Last              // Week of month (0 or Last, 1 or First, 2 or Second, 3 or Third, 4 or Fourth)
#define TIME_STD_DAY           Sun               // Day of week (1 or Sun, 2 or Mon, 3 or Tue, 4 or Wed, 5 or Thu, 6 or Fri, 7 or Sat)
#define TIME_STD_MONTH         Oct               // Month (1 or Jan, 2 or Feb, 3 or Mar, 4 or Apr, 5 or May, 6 or Jun, 7 or Jul, 8 or Aug, 9 or Sep, 10 or Oct, 11 or Nov, 12 or Dec)
#define TIME_STD_HOUR          2                 // Hour (0 to 23)
#ifndef TIME_STD_OFFSET
#define TIME_STD_OFFSET        0                 // Offset from UTC in minutes (-780 to +780)
#endif

// -- Location ------------------------------------
#ifndef LATITUDE              // eg. Belfast, Northern Ireland
#define LATITUDE               54.6         // [Latitude] Your location to be used with sunrise and sunset
#define LONGITUDE              -5.92        // [Longitude] Your location to be used with sunrise and sunset
#endif



// Sunrise and Sunset DawnType
#define DAWN_NORMAL            -0.8333
#define DAWN_CIVIL             -6.0
#define DAWN_NAUTIC            -12.0
#define DAWN_ASTRONOMIC        -18.0

// -- Time ----------------------------------------
#define USE_SUNRISE                            // Add support for Sunrise and sunset tools (+16k)
#define SUNRISE_DAWN_ANGLE DAWN_NORMAL       // Select desired Dawn Angle from (DAWN_NORMAL, DAWN_CIVIL, DAWN_NAUTIC, DAWN_ASTRONOMIC)


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 08_Logging
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

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
  #define ENABLE_DEBUG_FUNCTION_NAMES

  #define ENABLE_DEBUG_TRACE__MQTT_TOPIC_AS_TRASNMITTED
  #define ENABLE_DEBUG_TRACE__MQTT_PAYLOAD_AS_TRANSMITTED
  #define ENABLE_DEBUG_TRACE__MQTT_TOPIC_AS_TRASNMITTED


  #define ENABLE_LOG_LEVEL_NONE 
  #define ENABLE_LOG_LEVEL_ERROR 
  #define ENABLE_LOG_LEVEL_WARNING 
  #define ENABLE_LOG_LEVEL_TEST
  #define ENABLE_LOG_LEVEL_INFO 
  #define ENABLE_LOG_LEVEL_COMMANDS
  #define ENABLE_LOG_LEVEL_DEBUG 
  #define ENABLE_LOG_LEVEL_DEBUG_MORE 
  #define ENABLE_LOG_LEVEL_DEBUG_LOWLEVEL 
  #define ENABLE_LOG_LEVEL_ALL

  #define ENABLE_LOG

  // Enable debugging on only this module parts
  #define ENABLE_DEBUG_MODULE_LIGHTS_ADDRESSABLE

#endif

// Enable special d ebugs that trigger only regions
// #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES

#define ENABLE_LOG


// To reduce ram usage
#define ENABLE_LOG_LEVEL_NONE 
#define ENABLE_LOG_LEVEL_ERROR 
#define ENABLE_LOG_LEVEL_WARNING
#define ENABLE_LOG_LEVEL_TEST
#define ENABLE_LOG_LEVEL_IMPORTANT 
#define ENABLE_LOG_LEVEL_INFO 
#define ENABLE_LOG_LEVEL_COMMANDS
#define ENABLE_LOG_LEVEL_HIGHLIGHT
//#define ENABLE_LOG_LEVEL_DEBUG 
//#define ENABLE_LOG_LEVEL_DEBUG_MORE 
//#define ENABLE_LOG_LEVEL_DEBUG_LOWLEVEL
//#define ENABLE_LOG_LEVEL__DEBUG_TRACE 
//#define ENABLE_LOG_LEVEL_ALL
#define ENABLE_LOG_LEVEL_DYNAMIC_LEVELS


//  #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES


/**
 *  Extra Debugging methods
 **/
//#define ENABLE_FREERAM_APPENDING_SERIAL
// #define ADD_DEBUG_PERIODS_FOR_SERIAL_MONITORING
// #ifdef ENABLE_ADVANCED_DEBUGGING
//   #define DEBUG_FOR_FAULT
// #endif


/*********************************************************************************************\
 *  Logging
\*********************************************************************************************/
#define SYS_LOG_HOST                  ""                      // [LogHost] (Linux) syslog host
#define SYS_LOG_PORT                  514                     // [LogPort] default syslog UDP port
#define SYS_LOG_LEVEL                 LOG_LEVEL_DEBUG_MORE    // [SysLog] (LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE)

#define SERIAL_LOG_LEVEL              LOG_LEVEL_DEBUG         // [SerialLog] (LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE)
#ifndef SERIAL_LOG_LEVEL_DURING_BOOT
  #define SERIAL_LOG_LEVEL_DURING_BOOT  LOG_LEVEL_INFO         // [SerialLogBoot] LOG_LEVEL_DEV_TEST//LOG_LEVEL_ALL
#endif
#define WEB_LOG_LEVEL                 LOG_LEVEL_INFO          // [WebLog] (LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE)
#define TELNET_LOG_LEVEL              LOG_LEVEL_DEBUG

// #ifdef ENABLE_FEATURE_LOGGING__INCLUDE_RTC_IN_LOGS
// #define LOGTIME_DEFAULT_FORMAT        0                       //  false == LONG
// #else
#define LOGTIME_DEFAULT_FORMAT        1                       //  true == short
// #endif

#define ENABLE_LOGGING__BOOT_LOGLEVEL_SET_BY_FASTBOOT_COUNTER

/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 09_Telemetry
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 10_CrashRecorder
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 12_Events
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 13_RuleEngine
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/


#ifndef D_RULES_DELAY_LOAD_FROM_BOOT_TIME_SECOND
#define D_RULES_DELAY_LOAD_FROM_BOOT_TIME_SECOND 20
#endif


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 14_Updates
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

#define USE_ARDUINO_OTA

#define D_OTA_URL                       "http://minimal.bin"  // [OtaUrl]

#define USE_MODULE_CORE_UPDATES


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 15_SerialUART
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 31_I2C
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// -- I2C sensors ---------------------------------
#define USE_I2C                                  // I2C using library wire (+10k code, 0k2 mem, 124 iram)
#define I2CDRIVERS_0_31        0xFFFFFFFF          // Enable I2CDriver0  to I2CDriver31
#define I2CDRIVERS_32_63       0xFFFFFFFF          // Enable I2CDriver32 to I2CDriver63
#define I2CDRIVERS_64_95       0xFFFFFFFF          // Enable I2CDriver64 to I2CDriver95
#ifndef I2C_BUS_SPEED
#define I2C_BUS_SPEED           100000             // I2C bus speed (100000 or 400000)
// #define I2C_BUS_SPEED           400000             // I2C bus speed (100000 or 400000)
#endif

#define ENABLE_DEVFEATURE_I2C__SET_WIRE_INSTANCE_WITH_TWOWIRE_ZERO

#define USE_PWM // NEEDED FOR H801


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 32_SPI
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 34_PWM
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.

/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 35_PinViewer
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

#define USE_MODULE_CORE_PINVIEWER

/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 99_DevelopmentDebugging
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * SECTION: 3_NETWORK
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 00_Interface
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 03_WiFi2
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

  #define ENABLE_DEBUGFEATURE_WEB__TELEMETRY
  #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS

  /***********************************
   * SECTION: Networking
  ************************************/    
  #define WIFI_IP_ADDRESS               "0.0.0.0"         // [IpAddress1] Set to 0.0.0.0 for using DHCP or enter a static IP address
  #define WIFI_GATEWAY                  "192.168.1.1"     // [IpAddress2] If not using DHCP set Gateway IP address
  #define WIFI_SUBNETMASK               "255.255.255.0"   // [IpAddress3] If not using DHCP set Network mask
  #define WIFI_DNS                      "192.168.1.1"     // [IpAddress4] If not using DHCP set DNS IP address (might be equal to WIFI_GATEWAY)
  #define WIFI_DNS2                     "0.0.0.0"         // [IpAddress5] If not using DHCP set DNS2 IP address (might be equal to WIFI_GATEWAY)

  // #define STA_SSID1                     ""                // [Ssid1] Wifi SSID
  // #define STA_PASS1                     ""                // [Password1] Wifi password
  // #define STA_SSID2                     ""                // [Ssid2] Optional alternate AP Wifi SSID
  // #define STA_PASS2                     ""                // [Password2] Optional alternate AP Wifi password

  #define WIFI_CONFIG_TOOL              4//WIFI_RETRY        // [WiFi_Config_Mode_Set] Default tool if wifi fails to connect
                                                          //   (WIFI_RESTART, WIFI_SMARTCONFIG, WIFI_MANAGER, WIFI_WPSCONFIG, WIFI_RETRY, WIFI_WAIT, WIFI_SERIAL)
  #define WIFI_CONFIG_NO_SSID           WIFI_WPSCONFIG    // Default tool if wifi fails to connect and no SSID is configured
                                                          //   (WIFI_SMARTCONFIG, WIFI_MANAGER, WIFI_WPSCONFIG, WIFI_SERIAL)
                                                          //   *** NOTE: When WPS is disabled by USE_WPS below, WIFI_WPSCONFIG will execute WIFI_MANAGER ***
                                                          //   *** NOTE: When WIFI_MANAGER is disabled by USE_MODULE_CORE_WEBSERVER below, WIFI_MANAGER will execute WIFI_SMARTCONFIG ***
                                                          //   *** NOTE: When WIFI_SMARTCONFIG is disabled by USE_SMARTCONFIG below, WIFI_SMARTCONFIG will execute WIFI_SERIAL ***

   

  #define WIFI_MAXIMUM_CONNECTIONS 3 // static constexpr uint8_t NETWORK_PROFILE_SLOTS = 3;
  
  
  // 

// // -- Wifi Config tools ---------------------------
// #define WIFI_SOFT_AP_CHANNEL   1                 // Soft Access Point Channel number between 1 and 13 as used by Wifi Manager web GUI
// //#define USE_WPS                                  // Add support for WPS as initial wifi configuration tool (+33k code, 1k mem (5k mem with core v2.4.2+))
// //#define USE_SMARTCONFIG                          // Add support for Wifi SmartConfig as initial wifi configuration tool (+23k code, +0.6k mem)


#ifndef WIFI_RGX_STATE
#define WIFI_RGX_STATE              0
#endif
#ifndef WIFI_RGX_NAPT
#define WIFI_RGX_NAPT               0
#endif
#ifndef WIFI_RGX_SSID
#define WIFI_RGX_SSID               ""
#endif
#ifndef WIFI_RGX_PASSWORD
#define WIFI_RGX_PASSWORD           ""
#endif
#ifndef WIFI_RGX_IP_ADDRESS
#define WIFI_RGX_IP_ADDRESS         "192.168.99.1"
#endif
#ifndef WIFI_RGX_SUBNETMASK
#define WIFI_RGX_SUBNETMASK         "255.255.255.0"
#endif


// -- mDNS ----------------------------------------
#define USE_DISCOVERY                            // Enable mDNS for the following services (+8k code, +0.3k mem)
  #define USE_NETWORK_MDNS                    // Provide access to webserver by name <Hostname>.local/
  #define MQTT_HOST_DISCOVERY                    // Find MQTT host server (overrides MQTT_HOST if found)


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 04_Ethernet
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 05_Cellular
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 10_MQTT
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

#ifndef DISABLE_NETWORK
#define USE_MODULE_NETWORK_MQTT 
// here
#define USE_MODULE_NETWORK_WIFI
#endif // DISABLE_NETWORK

// #ifndef MQTT_HOST
// #define MQTT_HOST "192.168.0.70" // default
// #endif

  #define ENABLE_DEVFEATURE_MQTT__ESTIMATED_INCOMING_COMMANDS_AND_REPORT_ISSERVICED

#define DEFAULT_MQTT_SYSTEM_BOOT_RATE_SECS 1
#define DEFAULT_MQTT_SYSTEM_MINIMAL_RATE_SECS 120
#define DEFAULT_MQTT_SENSOR_MINIMAL_RATE_SECS 10
#define DEFAULT_MQTT_DRIVER_MINIMAL_RATE_SECS 10


#ifndef MQTT_HOST
#define MQTT_HOST "192.168.3.70"
#endif
#ifndef MQTT_PORT
#define MQTT_PORT 1883 // default
#endif

// #define MQTT_HOST       192,168,1,65   //TEMPORARY FIX, WILL BE REPLACED BY ABOVE PARSED STRING
#define MQTT_HOST_DISCOVERY

#ifdef USE_NETWORK_MDNS
#define MDNS_MQTT_HOSTNAME1   "TOWER.local" 
#define MDNS_MQTT_HOSTNAME2   "rasbpi.local" 
#endif // #ifdef USE_NETWORK_MDNS

#define DEFAULT_MDNS_NAME "pulsar"

#define MQTT_TOPIC             "PulSar_%06X"   // [Topic] unique MQTT device topic including (part of) device MAC address
#define MQTT_BUTTON_TOPIC      "0"               // [ButtonTopic] MQTT button topic, "0" = same as MQTT_TOPIC, set to 'PROJECT "_BTN_%06X"' for unique topic including device MAC address
#define MQTT_SWITCH_TOPIC      "0"               // [SwitchTopic] MQTT button topic, "0" = same as MQTT_TOPIC, set to 'PROJECT "_SW_%06X"' for unique topic including device MAC address
#define MQTT_GRPTOPIC          "pulsars"        // [GroupTopic] MQTT Group topic

// #define MQTT_PORT                     1884              // [MqttPort] MQTT port (10123 on CloudMQTT)
#define MQTT_USER                     "MQTT_USER"       // [MqttUser] MQTT user
#define MQTT_PASS                     "MQTT_PASS"       // [MqttPassword] MQTT password

#define MQTT_CLIENT_ID         "DVES_%06X"       // [MqttClient] Also fall back topic using last 6 characters of MAC address or use "DVES_%12X" for complete MAC address

#ifndef   MQTT_MAX_PACKET_SIZE
#define   MQTT_MAX_PACKET_SIZE          2000
#endif // MQTT_MAX_PACKET_SIZE

#define ENABLE_MQTT_DEBUG_TELEMETRY
#define USE_MQTT_RETAINED_VERSION_HISTORY_CHECK
#define ENABLE_MQTT_SEND_DISCONNECT_ON_RECONNECT

#define D_MQTT_COMMAND                "set"
#define D_MQTT_SYNC                   "sync"


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 21_WebServer
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

#define WEB_SERVER             2                 // [WebServer] Web server (0 = Off, 1 = Start as User, 2 = Start as Admin)
#define WEB_PASSWORD           ""                // [WebPassword] Web server Admin mode Password for WEB_USERNAME (empty string = Disable)

// //#define USE_CORS                                 // [Cors] Enable CORS - Be aware that this feature is unsecure ATM (https://github.com/arendst/Tasmota/issues/6767)
  #define CORS_DOMAIN            ""                // [Cors] CORS Domain for preflight requests


// -- HTTP ----------------------------------------
// #define USE_WEBSERVER                            // Enable web server and Wi-Fi Manager (+66k code, +8k mem)
  #define WEB_PORT             80                // Web server Port for User and Admin mode


#define ENABLE_DEBUGFEATURE_WEBSERVER_URL_LIST



/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * SECTION: 4_DRIVERS
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 00_Interface
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 03_LEDs
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/


// #define USE_MODULE_DRIVERS_LEDS

#ifndef DISABLE_LEDC_DEFINE_TO_PATCH_BUILD_ERROR_JUNE25

  #ifndef USE_MODULE_DRIVERS__CAMERA

    #ifndef LEDC_CHANNEL_MAX
      #define LEDC_CHANNEL_MAX 8 // should come from esp32-hal-ledc.h
    #endif
    #ifndef LEDC_SPEED_MODE_MAX
      #define LEDC_SPEED_MODE_MAX 2 // should come from esp32-hal-ledc.h ledc_mode_t
    #endif

  #endif

#endif


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 04_Relays
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 07_IRDevices
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// -- IR Remote features - subset of IR protocols --------------------------
#define USE_IR_REMOTE                            // Send IR remote commands using library IRremoteESP8266 (+4k3 code, 0k3 mem, 48 iram)
  #define IR_SEND_INVERTED          false        // Invert the output. (default = false) e.g. LED is illuminated when GPIO is LOW rather than HIGH.
                                                 // Setting inverted to something other than the default could easily destroy your IR LED if you are overdriving it.
                                                 // Unless you REALLY know what you are doing, don't change this.
  #define IR_SEND_USE_MODULATION    true         // Do we do frequency modulation during transmission? i.e. If not, assume a 100% duty cycle.

  // When using 'USE_IR_REMOTE_FULL', parameters below
  // (USE_IR_REMOTE, USE_IR_RECEIVE, USE_IR_HVAC...) are IGNORED.
  #define USE_IR_SEND_NEC                        // Support IRsend NEC protocol
  #define USE_IR_SEND_RC5                        // Support IRsend Philips RC5 protocol
  #define USE_IR_SEND_RC6                        // Support IRsend Philips RC6 protocol

  // Enable IR devoder via GPIO `IR Recv` - always enabled if `USE_IR_REMOTE_FULL`
  #define USE_IR_RECEIVE                         // Support for IR receiver (+7k2 code, 264 iram)
    #define IR_RCV_BUFFER_SIZE      100          // Max number of packets allowed in capture buffer (default 100 (*2 bytes ram))
    #define IR_RCV_TIMEOUT          15           // Number of milli-Seconds of no-more-data before we consider a message ended (default 15)
    #define IR_RCV_MIN_UNKNOWN_SIZE 6            // Set the smallest sized "UNKNOWN" message packets we actually care about (default 6, max 255)
    #define IR_RCV_WHILE_SENDING    0            // Turns on receiver while sending messages, i.e. receive your own. This is unreliable and can cause IR timing issues
    #define IR_RCV_TOLERANCE        25           // Base tolerance percentage for matching incoming IR messages (default 25, max 100)


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 08_IRRemote
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 09_Servos
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 10_RF433Codes
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 11_HBridgeL9110
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 12_SD
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 15_ShellyDimmer
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 20_Buzzer_Basic
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 21_Buzzer_Tones
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 40_Modem_SIM7000G
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 41_Modem_SIM800L
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 50_Camera
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 70_MAVLink_Decoder
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 71_MAVLink_Telemetry_WiFi
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 72_MAVLink_Telemetry_Cellular
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * SECTION: 5_SENSORS
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 00_Interface
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

#define TEMP_CONVERSION        0                 // [SetOption8] Return temperature in (0 = Celsius or 1 = Fahrenheit)
#define PRESSURE_CONVERSION    0                 // [SetOption24] Return pressure in (0 = hPa or 1 = mmHg)
#define TEMP_RESOLUTION        1                 // [TempRes] Maximum number of decimals (0 - 3) showing sensor Temperature
#define HUMIDITY_RESOLUTION    1                 // [HumRes] Maximum number of decimals (0 - 3) showing sensor Humidity
#define PRESSURE_RESOLUTION    1                 // [PressRes] Maximum number of decimals (0 - 3) showing sensor Pressure
#define ENERGY_RESOLUTION      3                 // [EnergyRes] Maximum number of decimals (0 - 5) showing energy usage in kWh
#define CALC_RESOLUTION        3                 // [CalcRes] Maximum number of decimals (0 - 7) used in commands ADD, SUB, MULT and SCALE
#define UNIFIED_INTERFACE_REPORTING_INVALID_SECONDS 180 // If any sensor/driver/energy reports greater than this since the last reading in seconds, it is excluded from the unified reporting
#define UNIFIED_INTERFACE_REPORTING_REPLACES_SUBMODULE_REPORTING false


#define FLAG_ENABLE_DEFAULT_PERIODIC_SENSOR_MQTT_MESSAGES   false


#ifndef SETTINGS_SENSORS_MQTT_IFCHANGED_PERIOD_SECONDS
#define SETTINGS_SENSORS_MQTT_IFCHANGED_PERIOD_SECONDS 10
#endif

#define ENABLE_DEVFEATURE_DEVICENAMES__USE_DEVICE_ID_WHEN_NO_NAME_MATCHED


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 01_Switches
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 02_Buttons
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 03_BME
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 04_DB18x20
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 05_DHT
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 06_BH1750Light
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 07_PIR
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 11_Door
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 13_L3GD20_3Axis_Gryo
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 14_LDRBasic
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 15_LSM303D_3Axis_AccMag
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 16_Moisture
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 17_SR04
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 18_MPU9250
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: PulseCounter
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 20_RemoteDevice
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 21_RotaryEncoder
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 22_SunTracking
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

  /***********************************
   * SECTION: Sensors
  ************************************/    


  // #define USE_MODULE_SENSORS_SUN_TRACKING
  //   #define USE_MODULE_SENSORS_SUN_TRACKING__ANGLES__MANUAL_OVERRIDE_FOR_TESTING


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 26_TOF_VL53L0X
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 27_TOF_VL53L1X
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 28_Radar_HLK_LD2410
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 30_ADCInternal
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 50_GPS_Serial
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * SECTION: 6_LIGHTS
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 00_Interface
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 03_Animator
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

  /***********************************
   * SECTION: Lighting Configs: Features
  ************************************/    

  #ifndef ANIMATION_UPDATOR_TIME_MINIMUM
  #define ANIMATION_UPDATOR_TIME_MINIMUM 20
  #endif 

  #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR
  // #define ENABLE_DEBUG_MANUAL_DELAYS

  // #if !defined(ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE) && !defined(ENABLE_NEOPIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE)
  // // Enable the default method must devices need to use
  // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S_SINGLE_CHANNELS_THEN_8_RMT_CHANNELS // To switch between I2S and RMT as primary channels
  // #endif

  #define ENABLE_DEVFEATURE_PALETTE__CHANGE_MY_PALETTE_INDEXING_TO_255_RANGE

     // Testing only, should be optimised to load when needed
  
  #define ENABLE_DEVFEATURE_PALETTE__FIX_WEBUI_GRADIENT_PREVIEW // Phase in
  #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT // Phase in
  #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED // Phase in
   // Phase in
  #define ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL // Phase in
  #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Phase in
   // Phase in
  #define USE_MODULE_LIGHTS_ADDRESSABLE // Phase in, with busses its always enabled
  #define USE_MODULE_LIGHTS_PWM // Phase in, with busses its always enabled

  /***********************************
   * SECTION: Lighting Configs: Debugging (to remain for testing)
  ************************************/    
  // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

  
    // #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG

  /**
   * @brief Send extra mqtt messages for debugging the palette
   * */
  #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST

  /**
   * @brief Send extra mqtt messages for debugging the ?
   * */
  #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
  #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
  #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
  #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
  #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE
  #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG
  

// Size of buffer for API JSON object (increase for more segments)
#ifdef ESP8266
  #define JSON_BUFFER_SIZE 10240
#else
  #if defined(ARDUINO_ARCH_ESP32S2)
    #define JSON_BUFFER_SIZE 24576
  #else
    #define JSON_BUFFER_SIZE 32767
  #endif
#endif
// Reserved for energy module defaults, ordered to match the code tree.
// Reserved for display module defaults, ordered to match the code tree.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * SECTION: 7_ENERGY
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 00_Interface
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 01_PZEM004T
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 02_ADE7953
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 03_INA219
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * SECTION: 8_DISPLAYS
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 00_Interface
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 01_Nextion
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 02_OLED_SSD1606
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 03_OLED_SH1106
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * SECTION: 9_CONTROLLER
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 03_Sonoff_iFan
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: Fan
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 08_TankVolume
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: Blinds
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: BucketWaterLevel
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: DoorBell
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: GPS_SD_Logger
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: SDCardLogger
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 40_HVAC
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 41_HVAC_Remote
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 42_SensorColourBar
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 43_RelayStateLEDStrip
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 50_Animator_Input_Controller
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 51_Animator_Playlists
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 60_EnergyOnOLED
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: SDLoggerIMURadiationPattern
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: SerialPositionalLogger
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: SerialCalibrationMeasurmentLogger
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: UserMod_01
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * SECTION: 10_CONSPEC
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 00_RadiatorFan
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 01_ImmersionTankColour
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 03_FurnaceSensor
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 04_LouvoliteHub
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 04v2_LouvoliteHub
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 05_SideDoorLight
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 06_ImmersionPanel
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 07_TempSensorOnOLED
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 08_CellularBlackBox
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 09_mMAVLink_Decoder_OLED
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 10_EnergyOnOLED
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 11_3DPrinterEnclosure
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 12_TreadmillLogger
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 13_ServerResetRelays
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 19_MAVLinkFlyingLEDS
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 20_DeskSensorsOnOLED
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 21_LightNeo_MotionAlerts
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.


/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * MODULE: 22_LightNeo_RadarDistance
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Reserved for module-specific defaults.

/*********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 * SECTION: FINAL_OVERRIDES
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************
 *********************************************************************************************/

// Anything added by this file must "undef" then "ifdef" to replace it.
#ifdef USE_CONFIG_OVERRIDE
#include "0_ConfigUser/G1_mUserConfig_Secret.h"
#endif

#endif // MUSERCONFIG_DEFAULTS_H
