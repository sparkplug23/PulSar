#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#define D_UNIQUE_MODULE_CORE_SETTINGS_ID 1

#include "2_CoreSystem/mBaseConfig.h"

/*
go with a singleton if you really need one global instance, better and safer than extern
struct singleton
{
static singleton& get()
{
static singleton potato;
return potato;
}
};
*/

#define DATA_BUFFER_TOPIC_MAX_LENGTH    100

#ifdef USE_MODULE_NETWORK_WEBSERVER
#define DATA_BUFFER_PAYLOAD_MAX_LENGTH  4000
#else
#define DATA_BUFFER_PAYLOAD_MAX_LENGTH  1500
#endif //USE_MODULE_NETWORK_WEBSERVER

#define   pCONT_set                               static_cast<mSettings*>(pCONT->pModule[EM_MODULE_CORE_SETTINGS_ID])


enum DATA_BUFFER_FLAG_SOURCE_IDS{
  DATA_BUFFER_FLAG_SOURCE_MQTT=0,
  DATA_BUFFER_FLAG_SOURCE_WEBUI
};


typedef union {
  uint16_t data;
  struct { 
    // 3 bits (9 values)
    // (0) DATA_BUFFER_FLAG_SOURCE_MQTT  // Defaulted to 0
    // (1) DATA_BUFFER_FLAG_SOURCE_WEBUI
    uint16_t source_id : 4;
    // Waiting
    uint16_t waiting : 1;
    // Encoding format
    uint8_t encoded_type_id; //json,raw
  };
} DATA_BUFFER_FLAGS;


struct DATA_BUFFER{
  struct TOPIC{
    char ctr[DATA_BUFFER_TOPIC_MAX_LENGTH];
    uint8_t len = 0;
  }topic;
  struct PAYLOAD{
    char ctr[DATA_BUFFER_PAYLOAD_MAX_LENGTH];
    uint16_t len = 0;
  }payload;
  uint8_t isserviced = 0; // Set to 0 on new mqtt
  DATA_BUFFER_FLAGS flags;
};
extern struct DATA_BUFFER data_buffer;
#define D_DATA_BUFFER_CLEAR() memset(&data_buffer,0,sizeof(data_buffer))


// struct TIME_T {
//   uint8_t       second;
//   uint8_t       minute;
//   uint8_t       hour;
//   uint8_t       day_of_week;               // sunday is day 1
//   uint8_t       day_of_month;
//   uint8_t       month;
//   char          name_of_month[4];
//   uint16_t      day_of_year;
//   uint16_t      year;
//   unsigned long days;
//   unsigned long valid;
// } ;
// extern struct TIME_T RtcTime;
// RtcTime;

typedef union {
  uint16_t data;
  struct {
    uint16_t hemis : 1;                    // bit 0        = 0=Northern, 1=Southern Hemisphere (=Opposite DST/STD)
    uint16_t week : 3;                     // bits 1 - 3   = 0=Last week of the month, 1=First, 2=Second, 3=Third, 4=Fourth
    uint16_t month : 4;                    // bits 4 - 7   = 1=Jan, 2=Feb, ... 12=Dec
    uint16_t dow : 3;                      // bits 8 - 10  = day of week, 1=Sun, 2=Mon, ... 7=Sat
    uint16_t hour : 5;                     // bits 11 - 15 = 0-23
  };
} TimeRule;

// extern union TimeRule TimeRuleTest;
// TimeRule;

// struct TIMERULES{
//   TimeRule timerule;
// };
// extern struct TIMERULES TimeRule;



#include "2_CoreSystem/Support/mSupport.h"
#include "2_CoreSystem/Logging/mLogging.h"

#include "1_TaskerManager/mTaskerManager.h"

//#include "//2_CoreSystem/Languages/mLanguage.h"
#include "2_CoreSystem/mHardwareTemplates.h"
#include "2_CoreSystem/mFirmwareDefaults.h"
#include "2_CoreSystem/Languages/mLanguageDefault.h"
#include "2_CoreSystem/Languages/mLanguageProgmem.h"

#include "1_TaskerManager/mTaskerManager.h"


#ifdef ESP32
  #include <WiFi.h>
#endif
#ifdef ESP8266
  #include <ESP8266WiFi.h>            // Wifi, MQTT, Ota, WifiManager
  // #include "avr/pgmspace.h"
  #if (defined(__AVR__))
  #include <avr\pgmspace.h>
  #else
  #include <pgmspace.h>
  #endif
#endif

#include "2_CoreSystem/Time/mTime.h"

#include "5_Sensors/Switches/mSwitches.h"



// const char EMPTY_MESSAGE_CTR[] PROGMEM = "Unused";

// default buffer sizes assumed by klists, negates the need to pass it to function
#define D_DEFAULT_DEVICE_BUFFER_LENGTH 50 

#ifndef LATITUDE
#define LATITUDE                    48.858360  // [Latitude] Your location to be used with sunrise and sunset
#endif
#ifndef LONGITUDE
#define LONGITUDE                   2.294442   // [Longitude] Your location to be used with sunrise and sunset
#endif


// Sunrise and Sunset DawnType
#define DAWN_NORMAL            -0.8333
#define DAWN_CIVIL             -6.0
#define DAWN_NAUTIC            -12.0
#define DAWN_ASTRONOMIC        -18.0

#ifndef COLOR_TEXT
#define COLOR_TEXT                  "#000"     // Global text color - Black
#endif
#ifndef COLOR_BACKGROUND
#define COLOR_BACKGROUND            "#fff"     // Global background color - White
#endif
#ifndef COLOR_FORM
#define COLOR_FORM                  "#000000"  // Form background color - Greyish
#endif
#ifndef COLOR_INPUT_TEXT
#define COLOR_INPUT_TEXT            "#000"     // Input text color - Black
#endif
#ifndef COLOR_INPUT
#define COLOR_INPUT                 "#fff"     // Input background color - White
#endif
#ifndef COLOR_CONSOLE_TEXT
#define COLOR_CONSOLE_TEXT          "#000"     // Console text color - Black
#endif
#ifndef COLOR_CONSOLE
#define COLOR_CONSOLE               "#fff"     // Console background color - White
#endif
#ifndef COLOR_TEXT_WARNING
#define COLOR_TEXT_WARNING          "#f00"     // Warning text color - Red
#endif
#ifndef COLOR_TEXT_SUCCESS
#define COLOR_TEXT_SUCCESS          "#008000"  // Success text color - Green
#endif
#ifndef COLOR_BUTTON_TEXT
#define COLOR_BUTTON_TEXT           "#fff"     // Button text color - White
#endif
#ifndef COLOR_BUTTON
#define COLOR_BUTTON                "#1fa3ec"  // Button color - Blueish
#endif
#ifndef COLOR_BUTTON_HOVER
#define COLOR_BUTTON_HOVER          "#0e70a4"  // Button color when hovered over - Darker blueish
#endif
#ifndef COLOR_BUTTON_RESET
#define COLOR_BUTTON_RESET          "#d43535"  // Restart/Reset/Delete button color - Redish
#endif
#ifndef COLOR_BUTTON_RESET_HOVER
#define COLOR_BUTTON_RESET_HOVER    "#931f1f"  // Restart/Reset/Delete button color when hovered over - Darker redish
#endif
#ifndef COLOR_BUTTON_SAVE
#define COLOR_BUTTON_SAVE           "#47c266"  // Save button color - Greenish
#endif
#ifndef COLOR_BUTTON_SAVE_HOVER
#define COLOR_BUTTON_SAVE_HOVER     "#5aaf6f"  // Save button color when hovered over - Darker greenish
#endif
#ifndef COLOR_TIMER_TAB_TEXT
#define COLOR_TIMER_TAB_TEXT        "#fff"     // Config timer tab text color - White
#endif
#ifndef COLOR_TIMER_TAB_BACKGROUND
#define COLOR_TIMER_TAB_BACKGROUND  "#999"     // Config timer tab background color - Light grey
#endif


enum DATABUILDER_JSON_LEVEL{ //in order of importance
  JSON_LEVEL_NONE=0,
  JSON_LEVEL_IFCHANGED,
  JSON_LEVEL_SHORT,
  JSON_LEVEL_DETAILED,
  JSON_LEVEL_ALL,
  JSON_LEVEL_DEBUG // Share extra info relating to the generation of the json
};

DEFINE_PGM_CTR(PM_JSON_LEVEL_NONE_CTR)        "None";
DEFINE_PGM_CTR(PM_JSON_LEVEL_IFCHANGED_CTR)   "IfChanged";
  

enum SWITCH_SPLIT_TASK_IDS{
  SPLIT_TASK_NOT_RUNNING_ID=0,
  SPLIT_TASK_SUCCESS_ID=1,
  SPLIT_TASK_SEC1_ID,
  SPLIT_TASK_SEC2_ID,
  SPLIT_TASK_SEC3_ID,
  SPLIT_TASK_SEC4_ID,
  SPLIT_TASK_SEC5_ID,
  SPLIT_TASK_SEC6_ID,
  SPLIT_TASK_SEC7_ID,
  SPLIT_TASK_SEC8_ID,
  SPLIT_TASK_TIMEOUT_ID,
  SPLIT_TASK_DONE_ID
};


/*********************************************************************************************\
 * Performance ROM (PROGMEM) vs RAM (RODATA)
\*********************************************************************************************/

//#define XFUNC_PTR_IN_ROM                    // Enable for keeping tables in ROM (PROGMEM) which seem to have access issues on some flash types

/*********************************************************************************************\
 * Default sensor states
\*********************************************************************************************/

#define CODE_IMAGE 0

//#define USE_DHT                             // Default DHT11 sensor needs no external library
#define USE_ENERGY_SENSOR                   // Use energy sensors (+14k code)
#define USE_HLW8012                         // Use energy sensor for Sonoff Pow and WolfBlitz
#define USE_CSE7766                         // Use energy sensor for Sonoff S31 and Pow R2

/*********************************************************************************************\
 * Power Type
\*********************************************************************************************/

typedef unsigned long power_t;              // Power (Relay) type
const uint32_t POWER_MASK = 0xffffffffUL;   // Power (Relay) full mask

/*********************************************************************************************\
 * Constants
\*********************************************************************************************/

// Changes to the following MAX_ defines will impact settings layout
const uint8_t MAX_RELAYS = 8;               // Max number of relays
const uint8_t MAX_INTERLOCKS = 4;           // Max number of interlock groups (MAX_RELAYS / 2)
const uint8_t MAX_LEDS = 4;                 // Max number of leds

const uint8_t MAX_PWMS = 5;                 // Max number of PWM channels
const uint8_t MAX_COUNTERS = 4;             // Max number of counter sensors
const uint8_t MAX_TIMERS = 16;              // Max number of Timers
#define MAX_PULSETIMERS 8                   // Max number of supported pulse timers
const uint8_t MAX_FRIENDLYNAMES = 4;        // Max number of Friendly names
// const uint8_t MAX_DOMOTICZ_IDX = 4;         // Max number of Domoticz device, key and switch indices
// const uint8_t MAX_DOMOTICZ_SNS_IDX = 12;    // Max number of Domoticz sensors indices
// const uint8_t MAX_KNX_GA = 10;              // Max number of KNX Group Addresses to read that can be set
// const uint8_t MAX_KNX_CB = 10;              // Max number of KNX Group Addresses to write that can be set
// const uint8_t MAX_XNRG_DRIVERS = 32;        // Max number of allowed energy drivers
// const uint8_t MAX_XDSP_DRIVERS = 32;        // Max number of allowed display drivers
// const uint8_t MAX_XDRV_DRIVERS = 96;        // Max number of allowed driver drivers
// const uint8_t MAX_XSNS_DRIVERS = 96;        // Max number of allowed sensor drivers
// const uint8_t MAX_RULE_MEMS = 5;            // Max number of saved vars
// const uint8_t MAX_RULE_SETS = 3;            // Max number of rule sets of size 512 characters
// const uint16_t MAX_RULE_SIZE = 512;         // Max number of characters in rules
// const uint8_t MAX_FAN_SPEED = 4;            // Max number of iFan02 fan speeds (0 .. 3)

// const char MQTT_TOKEN_PREFIX[] PROGMEM = "%prefix%";  // To be substituted by mqtt_prefix[x]
// const char MQTT_TOKEN_TOPIC[] PROGMEM = "%topic%";    // To be substituted by mqtt_topic, mqtt_grptopic, mqtt_buttontopic, mqtt_switchtopic
// //const char WIFI_HOSTNAME[] = "%s-%04d";     // Expands to <MQTT_TOPIC>-<last 4 decimal chars of MAC address>
const char WIFI_HOSTNAME[]  = "%s"; //!PROGMEM    // Expands to <MQTT_TOPIC>-<last 4 decimal chars of MAC address>
//^ cant be porgmem
// const uint8_t CONFIG_FILE_SIGN = 0xA5;      // Configuration file signature
// const uint8_t CONFIG_FILE_XOR = 0x5A;       // Configuration file xor (0 = No Xor)

// const uint32_t HLW_PREF_PULSE = 12530;      // was 4975us = 201Hz = 1000W
// const uint32_t HLW_UREF_PULSE = 1950;       // was 1666us = 600Hz = 220V
// const uint32_t HLW_IREF_PULSE = 3500;       // was 1666us = 600Hz = 4.545A

const uint8_t MQTT_RETRY_SECS = 10;         // Minimum seconds to retry MQTT connection
// const uint32_t GLOBAL_VALUES_VALID = 300;   // Max number of seconds to keep last received values
// const power_t APP_POWER = 0;                // Default saved power state Off
//const uint16_t WS2812_MAX_LEDS = 512;       // Max number of ledout.index

const uint32_t PWM_RANGE = 1023;            // 255..1023 needs to be devisible by 256
//const uint16_t PWM_FREQ = 1000;             // 100..1000 Hz led refresh
//const uint16_t PWM_FREQ = 910;              // 100..1000 Hz led refresh (iTead value)
const uint16_t PWM_FREQ = 880;              // 100..1000 Hz led refresh (BN-SZ01 value)
const uint16_t PWM_MAX = 4000;              // [PWM_MAX] Maximum frequency - Default: 4000
const uint16_t PWM_MIN = 100;               // [PWM_MIN] Minimum frequency - Default: 100
                                            //    For Dimmers use double of your mains AC frequecy (100 for 50Hz and 120 for 60Hz)
                                            //    For Controlling Servos use 50 and also set PWM_FREQ as 50 (DO NOT USE THESE VALUES FOR DIMMERS)

// const uint8_t DEFAULT_POWER_DELTA = 80;     // Power change percentage
// const uint16_t MAX_POWER_HOLD = 10;         // Time in SECONDS to allow max agreed power
// const uint16_t MAX_POWER_WINDOW = 30;       // Time in SECONDS to disable allow max agreed power
// const uint16_t SAFE_POWER_HOLD = 10;        // Time in SECONDS to allow max unit safe power
// const uint16_t SAFE_POWER_WINDOW = 30;      // Time in MINUTES to disable allow max unit safe power
const uint8_t MAX_POWER_RETRY = 5;          // Retry count allowing agreed power limit overflow

// const uint8_t STATES = 20;                  // Number of states per second using 50 mSec interval
const uint8_t IMMINENT_RESET_FACTOR = 10;   // Factor to extent button hold time for imminent Reset to default 40 seconds using KEY_HOLD_TIME of 40
const uint8_t BOOT_LOOP_TIME = 10;         // Number of seconds to stop detecting boot loops
// const uint16_t SYSLOG_TIMER = 600;          // Seconds to restore syslog_level
// const uint16_t SERIALLOG_TIMER = 600;       // Seconds to disable SerialLog
// const uint8_t OTA_ATTEMPTS = 5;             // Number of times to try fetching the new firmware

const uint16_t FLOATSZ = 16;                // Max number of characters in float result from dtostrfd (max 32)
// // const uint16_t INPUT_BUFFER_SIZE = 520;     // Max number of characters in (serial and http) command buffer
// const uint16_t CMDSZ = 2;                  // Max number of characters in command
// const uint16_t TOPSZ = 2;                 // Max number of characters in topic string
// // const uint16_t MIN_MESSZ = 893;             // Min number of characters in MQTT message

// const uint8_t SENSOR_MAX_MISS = 5;          // Max number of missed sensor reads before deciding it's offline
const uint8_t MAX_NTP_SERVERS = 3;          // Max number of NTP servers

// #ifdef USE_MQTT_TLS
//   const uint16_t WEB_LOG_SIZE = 2000;       // Max number of characters in weblog
// #else
//  const uint16_t WEB_LOG_SIZE = 1500;       // Max number of characters in weblog
// #endif

// #ifdef ENABLE_LOW_MEMORY_MODE
//   const uint16_t WEB_LOG_SIZE = 1000;       // Max number of characters in weblog
// #else
// #endif

// const uint8_t MAX_BACKLOG = 30;             // Max number of commands in backlog
// const uint32_t MIN_BACKLOG_DELAY = 2;       // Minimal backlog delay in 0.1 seconds

const uint32_t SOFT_BAUDRATE = 9600;        // Default software serial baudrate
const uint32_t APP_BAUDRATE = 115200;       // Default serial baudrate
// const uint32_t SERIAL_POLLING = 100;        // Serial receive polling in ms
// // const uint8_t MAX_STATUS = 11;              // Max number of status lines


const uint32_t START_VALID_TIME = 1451602800;  // Time is synced and after 2016-01-01

const char D_NO_MATCH_CTR[] = "NoMatch";

//const uint32_t DRIVER_BOOT_DELAY = 1;       // Number of milliseconds to retard driver cycles during boot-up time to reduce overall CPU load whilst Wifi is connecting
//const uint32_t LOOP_SLEEP_DELAY = 50;       // Lowest number of milliseconds to go through the main loop using delay when needed

/*********************************************************************************************\
 * Defines
\*********************************************************************************************/

#define MAX_RULE_TIMERS        8            // Max number of rule timers (4 bytes / timer)
#define MAX_RULE_VARS          5            // Max number of rule variables (10 bytes / variable)

#define NO_EXTRA_4K_HEAP                    // Allocate 4k heap for WPS in ESP8166/Arduino core v2.4.2 (was always allocated in previous versions)

#define tmin(a,b) ((a)<(b)?(a):(b))
#define tmax(a,b) ((a)>(b)?(a):(b))

#define STR_HELPER(x) #x
#ifndef STR
#define STR(x) STR_HELPER(x)
#endif



/*********************************************************************************************\
 * Enumeration
\*********************************************************************************************/

enum WeekInMonthOptions {Last, First, Second, Third, Fourth};
enum DayOfTheWeekOptions {Sun=1, Mon, Tue, Wed, Thu, Fri, Sat};
enum MonthNamesOptions {Jan=1, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec};
enum HemisphereOptions {North, South};
// enum GetDateAndTimeOptions { DT_LOCAL, DT_UTC, DT_RESTART, DT_ENERGY };
enum GetDateAndTimeOptions { DT_LOCAL, DT_UTC, DT_LOCALNOTZ, DT_DST, DT_STD, DT_RESTART, DT_ENERGY, DT_BOOTCOUNT, DT_LOCAL_MILLIS, DT_TIMEZONE, DT_SUNRISE, DT_SUNSET };


enum WifiConfigOptions {WIFI_RESTART, WIFI_SMARTCONFIG, WIFI_MANAGER, WIFI_WPSCONFIG, WIFI_RETRY, WIFI_WAIT, WIFI_SERIAL, WIFI_MANAGER_RESET_ONLY, MAX_WIFI_OPTION};

enum SwitchModeOptions_IDS {
  TOGGLE, 
  FOLLOW, 
  FOLLOW_INV, 
  PUSHBUTTON, 
  PUSHBUTTON_INV, 
  PUSHBUTTONHOLD, 
  PUSHBUTTONHOLD_INV, 
  PUSHBUTTON_TOGGLE, 
  MAX_SWITCH_OPTION
};

enum LedStateOptions {LED_OFF, LED_POWER, LED_MQTTSUB, LED_POWER_MQTTSUB, LED_MQTTPUB, LED_POWER_MQTTPUB, LED_MQTT, LED_POWER_MQTT, MAX_LED_OPTION};

// enum EmulationOptions {EMUL_NONE, EMUL_WEMO, EMUL_HUE, EMUL_MAX};

enum TopicOptions { CMND, STAT, TELE, nu1, RESULT_OR_CMND, RESULT_OR_STAT, RESULT_OR_TELE };

// Change these to have matching words
enum ExecuteCommandPowerOptions { POWER_OFF, POWER_ON, POWER_TOGGLE, POWER_BLINK, POWER_BLINK_STOP,
                                  POWER_OFF_NO_STATE = 8, POWER_ON_NO_STATE, POWER_TOGGLE_NO_STATE,
                                  POWER_SHOW_STATE = 16 };
// enum POWER_COMMAND_IDS{
//   POWER_COMMAND_OFF_ID,
//   POWER_COMMAND_ON_ID,
//   POWER_COMMAND_TOGGLE_ID,
// };
                                  
enum PowerOnStateOptions { 
  POWER_ALL_OFF, 
  POWER_ALL_ON, 
  POWER_ALL_SAVED_TOGGLE, 
  POWER_ALL_SAVED, 
  POWER_ALL_ALWAYS_ON, 
  POWER_ALL_OFF_PULSETIME_ON 
};


// Ordered in levels, if(>0 then active)
enum MODULE_STATUS{
  MODULE_STATUS_DISABLED = 0,       // Disabled = User has chosen not to use module, block tasker from executing
  MODULE_STATUS_PRE_INIT_PASS,      // Pre-init completely successfully
  MODULE_STATUS_INIT_PASS,          // Init completed successfully
  MODULE_STATUS_ENABLED_NOT_ACTIVE, // Sensor not found, or driver didnt complete init, retry (pre-)init process
  MODULE_STATUS_ENABLED_ACTIVE,     // Working = Normal state
  MODULE_STATUS_NOTSET,             // Default start state
  MODULE_STATUS_LENGTH
};


 enum Shortcuts { SC_CLEAR, SC_DEFAULT, SC_USER };

//TBR
enum SettingsParmaIndex {P_HOLD_TIME, P_MAX_POWER_RETRY, 
P_TUYA_DIMMER_ID, P_MDNS_DELAYED_START, P_BOOT_LOOP_OFFSET, 
P_RGB_REMAP, P_MAX_PARAM8};  // Max is PARAM8_SIZE (18) - SetOption32 until SetOption49




//scenes animations?
enum LightSchemes {LS_POWER, LS_WAKEUP, LS_CYCLEUP, LS_CYCLEDN, LS_RANDOM, LS_MAX};

// enum COMMAND_FUNCTIONS_IDS{}


//move functions into taskermanager
enum XsnsFunctions {
  // Init stuff (in importance of boot)
  
  // FUNC_CHECK_POINTERS, //phased out
  FUNC_TEMPLATE_MODULE_LOAD,  // Read progmem configs if needed, read settings configuration
  FUNC_SETTINGS_DEFAULT,   // Use defaults in code
  FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT,   // Use defaults in code
  FUNC_POINTER_INIT, // Any pointers to buffers, anything not set in this (unset nullptr) must be protected 
  FUNC_MODULE_INIT,  // Read progmem configs if needed, read settings configuration
  FUNC_PRE_INIT,     // Configure sub modules and classes as needed
  FUNC_INIT,         // Actually complete init, read sensors, enable modules fully etc
  FUNC_POST_INIT,    // similar to FUNC_CONFIGURE_MODULES_FOR_DEVICE, will run immediately after init
  FUNC_FUNCTION_LAMBDA_INIT,
  FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES,   // Use defaults in code
  FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE, // Load values from settings struct and overwrite module values
  FUNC_CONFIGURE_MODULES_FOR_DEVICE, // Configure any sensors that are needed for controllers, to the required settings
  FUNC_MQTT_HANDLERS_INIT,
  // FUNC_MQTT_INIT,
  
  FUNC_TEMPLATE_DEVICE_LOAD,  // Read progmem configs if needed, read settings configuration
  FUNC_TEMPLATE_DEVICE_EXECUTE_LOAD, // This is called from the above function, used to parse the object json
  
  // New trigger events which only modules to react to changes "triggers" from other modules or external sources
  FUNC_TRIGGER_EVENT_BUTTON_PRESSED,
  // FUNC_TRIGGER_
  
  
  FUNC_ON_BOOT_COMPLETE, //also used as boot percentage progress divisor : Reaches end of setup
// END OF BOOT SECTION 
  FUNC_ON_BOOT_SUCCESSFUL, //should be triggered once after device is considered stable

  // Configure sensors and drivers for device
  // Looping trigger times
  FUNC_LOOP, FUNC_EVERY_50_MSECOND, FUNC_EVERY_100_MSECOND, FUNC_EVERY_250_MSECOND, 
  FUNC_EVERY_SECOND, FUNC_EVERY_SECOND_TP0MS_WINDOW, FUNC_EVERY_SECOND_TP250MS_WINDOW, FUNC_EVERY_SECOND_TP500MS_WINDOW, FUNC_EVERY_SECOND_TP750MS_WINDOW, // All run only once a second, but with delays to spread out tasks which occur at that time cycle
  FUNC_EVERY_FIVE_SECOND, //Used mainly as debugging
  FUNC_EVERY_FIVE_MINUTE,
  FUNC_EVERY_MINUTE, FUNC_EVERY_HOUR, FUNC_EVERY_MIDNIGHT, FUNC_EVERY_MIDDAY,
  // Run once from restart/uptime
  FUNC_UPTIME_10_SECONDS, //FUNC_ON_BOOT_SUCCESSFUL same time slot
  FUNC_UPTIME_1_MINUTES,
  FUNC_UPTIME_10_MINUTES,
  FUNC_UPTIME_60_MINUTES,
  // Welcome message to show on boot after X seconds with config
  // FUNC_RESTART_SPLASH_INFORMATION, //have flag that shows level of information AND make this an ifdef as a debug option
  
  // FUNC_PREP_BEFORE_TELEPERIOD, 
  FUNC_JSON_APPEND, 
  
  // Saving/EEPROM related
  FUNC_SAVE_BEFORE_RESTART, //phase out
  
  FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE, // Save internal module values to global settings struct, prior to saving to memory

  // New call functio similar to mqtt method, called from class loop (checked in support)
  FUNC_FUNCTION_LAMBDA_LOOP,

  // System/Status message
  FUNC_BOOT_MESSAGE, //at 10 seconds, show how the function is configured

  //Light commands
  FUNC_LIGHT_POWER_ON,
  FUNC_LIGHT_POWER_OFF,
  FUNC_LIGHT_UPDATE_OUTPUT,

  // Commands
  // FUNC_COMMAND, 
  // FUNC_COMMAND_SENSOR, 
  // FUNC_COMMAND_DRIVER, 
  // FUNC_JSON_COMMAND, 
  // FUNC_JSON_COMMAND_OBJECT, 
  // FUNC_JSON_COMMAND_OBJECT_WITH_TOPIC,  //ie check for the topic (in the future use module name are set/<moduclename>)
  FUNC_JSON_COMMAND_CHECK_TOPIC_ID,
  FUNC_JSON_COMMAND_ID,  //ie check for the topic (in the future use module name are set/<moduclename>)
  // Wifi 
  FUNC_WIFI_CONNECTED, FUNC_WIFI_DISCONNECTED,
  // Mqtt
  FUNC_MQTT_SUBSCRIBE, FUNC_MQTT_CONNECTED, FUNC_MQTT_DISCONNECTED, FUNC_MQTT_COMMAND, FUNC_MQTT_SENDER,
  FUNC_MQTT_HANDLERS_RESET, FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD,
  FUNC_MQTT_CHECK_REDUNCTION_LEVEL, //
  // 
  FUNC_SET_POWER, FUNC_SET_DEVICE_POWER, FUNC_SHOW_SENSOR,
  FUNC_RULES_PROCESS, FUNC_SERIAL, FUNC_FREE_MEM, FUNC_BUTTON_PRESSED,
  FUNC_SET_POWER_ON_ID,
  FUNC_SET_POWER_OFF_ID,

  // Events (new internal triggers, if something happens trigger another... this will become rules)
  // Ie if this event below happens, then have another ID which is called to run the triggered event
  FUNC_EVENT_MOTION_STARTED_ID,
  FUNC_EVENT_MOTION_ENDED_ID,


  // Energy
  FUNC_ENERGY_RESET,
  // Calls to run drivers when sensors change rather than waiting on loop
  FUNC_SENSOR_UPDATED,
  //
  FUNC_STATUS_MESSAGE_APPEND, FUNC_JSON_HARDWARE_APPEND, // Messages
  FUNC_SET_CHANNELS, 
  // Webpage
  FUNC_WEB_ADD_MAIN_BUTTON, 
  FUNC_WEB_ADD_BUTTON, 
  FUNC_WEB_ADD_BUTTON_SYSTEM_SETTINGS, //system settings 
  FUNC_WEB_ADD_HANDLER, //root page

  FUNC_WEB_ROOT_SEND_STYLE, // Send any style for root page
  FUNC_WEB_ROOT_SEND_SCRIPT,  
  FUNC_WEB_ROOT_SCRIPT_JSON_FETCH_MODULEPARSING, 
  FUNC_WEB_ROOT_SEND_BODY,   // send once on load 
  FUNC_WEB_ROOT_SEND_STATUS,     //the tas way
  FUNC_WEB_ADD_ROOT_SHOWS,

  FUNC_WEB_ADD_JSON_FETCH_RESULT, // As part of fetch
  FUNC_WEB_ADD_JSON_DATA_FETCH_URL, // Adding urls that will be requested for as part of jsonparse
  FUNC_WEB_ADD_ROOT_TABLE_ROWS,
  FUNC_WEB_ADD_ROOT_MODULE_TABLE_CONTAINER,

  FUNC_WEB_ADD_ROOT_SCRIPT,
  FUNC_WEB_ADD_ROOT_STYLE,  

  // FUNC_WEB_APPEND_LOADTIME_ROOT_URLS,
  // FUNC_WEB_APPEND_LOADTIME_ROOT_RATES,
  FUNC_WEB_APPEND_RUNTIME_ROOT_URLS,
  // FUNC_WEB_APPEND_RUNTIME_ROOT_RATES,
  
  FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED, //send table data
  FUNC_WEB_APPEND_ROOT_STATUS_TABLE_FORCED,
  FUNC_WEB_APPEND_ROOT_ADD_MAIN_BUTTONS,
  FUNC_WEB_APPEND_ROOT_BUTTONS,

  FUNC_WEB_PAGEINFORMATION_SEND_MODULE, //send parts of page info

  // parital sub functions that need to be address later by special "interface tasker"
  FUNC_WEB_RESPOND_LIGHT_LIVEVIEW_JSON, //reply with liveview json data "WEB_RESPOND"=func group, "LIGHT"=interface, LIVEVIEW_JSON=type


  FUNC_WEB_COMMAND,    
  // FUNC_WEB_SHOW_PARAMETERS,   PHASED OUT
  // FUNC_WEB_SHOW_PARAMETERS2, 
  FUNC_WEB_SYSTEM_INFO,  //WEBSERVER

  // Debuging functions
  FUNC_DEBUG_CONFIGURE,

  /**
   * Group of "Events" functions, which are called when sections of the code execute
   * */
  FUNC_EVENT_INPUT_STATE_CHANGED_ID,
  FUNC_EVENT_SET_POWER_ID,


  FUNC_FINALLY_END_OF_LOOP // Ran at the end of each loop, used to reset flags that should have been handled eg motion
};

#ifdef ENABLE_DEBUG_FUNCTION_NAMES

DEFINE_PGM_CTR(PM_FUNC_POINTER_INIT_CTR)                            D_FUNC_POINTER_INIT_CTR;
DEFINE_PGM_CTR(PM_FUNC_TEMPLATE_LOAD_CTR)                           D_FUNC_TEMPLATE_LOAD_CTR;
DEFINE_PGM_CTR(PM_FUNC_MODULE_INIT_CTR)                             D_FUNC_MODULE_INIT_CTR;
DEFINE_PGM_CTR(PM_FUNC_PRE_INIT_CTR)                                D_FUNC_PRE_INIT_CTR;
DEFINE_PGM_CTR(PM_FUNC_INIT_CTR)                                    D_FUNC_INIT_CTR;
DEFINE_PGM_CTR(PM_FUNC_CONFIGURE_MODULES_FOR_DEVICE_CTR)            D_FUNC_CONFIGURE_MODULES_FOR_DEVICE_CTR;
DEFINE_PGM_CTR(PM_FUNC_LOOP_CTR)                                    D_FUNC_LOOP_CTR;
DEFINE_PGM_CTR(PM_FUNC_EVERY_50_MSECOND_CTR)                        D_FUNC_EVERY_50_MSECOND_CTR;
DEFINE_PGM_CTR(PM_FUNC_EVERY_100_MSECOND_CTR)                       D_FUNC_EVERY_100_MSECOND_CTR;
DEFINE_PGM_CTR(PM_FUNC_EVERY_250_MSECOND_CTR)                       D_FUNC_EVERY_250_MSECOND_CTR;
DEFINE_PGM_CTR(PM_FUNC_EVERY_SECOND_CTR)                            D_FUNC_EVERY_SECOND_CTR;
DEFINE_PGM_CTR(PM_FUNC_EVERY_MINUTE_CTR)                            D_FUNC_EVERY_MINUTE_CTR; 
DEFINE_PGM_CTR(PM_FUNC_EVERY_HOUR_CTR)                              D_FUNC_EVERY_HOUR_CTR; 
DEFINE_PGM_CTR(PM_FUNC_EVERY_MIDNIGHT_CTR)                          D_FUNC_EVERY_MIDNIGHT_CTR;
DEFINE_PGM_CTR(PM_FUNC_EVERY_MIDDAY_CTR)                            D_FUNC_EVERY_MIDDAY_CTR;
DEFINE_PGM_CTR(PM_FUNC_ON_SUCCESSFUL_BOOT_CTR)                      D_FUNC_ON_SUCCESSFUL_BOOT_CTR;
DEFINE_PGM_CTR(PM_FUNC_UPTIME_10_SECONDS_CTR)                       D_FUNC_UPTIME_10_SECONDS_CTR;
DEFINE_PGM_CTR(PM_FUNC_UPTIME_1_MINUTES_CTR)                        D_FUNC_UPTIME_1_MINUTES_CTR;
DEFINE_PGM_CTR(PM_FUNC_UPTIME_10_MINUTES_CTR)                       D_FUNC_UPTIME_10_MINUTES_CTR;
DEFINE_PGM_CTR(PM_FUNC_UPTIME_60_MINUTES_CTR)                       D_FUNC_UPTIME_60_MINUTES_CTR;
DEFINE_PGM_CTR(PM_FUNC_RESTART_SPLASH_INFORMATION_CTR)              D_FUNC_RESTART_SPLASH_INFORMATION_CTR;
DEFINE_PGM_CTR(PM_FUNC_PREP_BEFORE_TELEPERIOD_CTR)                  D_FUNC_PREP_BEFORE_TELEPERIOD_CTR;
DEFINE_PGM_CTR(PM_FUNC_JSON_APPEND_CTR)                             D_FUNC_JSON_APPEND_CTR;
DEFINE_PGM_CTR(PM_FUNC_SAVE_BEFORE_RESTART_CTR)                     D_FUNC_SAVE_BEFORE_RESTART_CTR;
DEFINE_PGM_CTR(PM_FUNC_SETTINGS_DEFAULT_CTR)                        D_FUNC_SETTINGS_DEFAULT_CTR;
DEFINE_PGM_CTR(PM_FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES_CTR)     D_FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES_CTR;
DEFINE_PGM_CTR(PM_FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT_CTR)     D_FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT_CTR;
DEFINE_PGM_CTR(PM_FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE_CTR)        D_FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE_CTR;
DEFINE_PGM_CTR(PM_FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE_CTR)        D_FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE_CTR;
DEFINE_PGM_CTR(PM_FUNC_FUNCTION_LAMBDA_INIT_CTR)                    D_FUNC_FUNCTION_LAMBDA_INIT_CTR;
DEFINE_PGM_CTR(PM_FUNC_FUNCTION_LAMBDA_LOOP_CTR)                    D_FUNC_FUNCTION_LAMBDA_LOOP_CTR;
DEFINE_PGM_CTR(PM_FUNC_COMMAND_CTR)                                 D_FUNC_COMMAND_CTR;
DEFINE_PGM_CTR(PM_FUNC_COMMAND_SENSOR_CTR)                          D_FUNC_COMMAND_SENSOR_CTR;
DEFINE_PGM_CTR(PM_FUNC_COMMAND_DRIVER_CTR)                          D_FUNC_COMMAND_DRIVER_CTR;
DEFINE_PGM_CTR(PM_FUNC_JSON_COMMAND_CTR)                            D_FUNC_JSON_COMMAND_CTR;
DEFINE_PGM_CTR(PM_FUNC_JSON_COMMAND_OBJECT_CTR)                     D_FUNC_JSON_COMMAND_OBJECT_CTR;
DEFINE_PGM_CTR(PM_FUNC_WIFI_CONNECTED_CTR)                          D_FUNC_WIFI_CONNECTED_CTR;
DEFINE_PGM_CTR(PM_FUNC_WIFI_DISCONNECTED_CTR)                       D_FUNC_WIFI_DISCONNECTED_CTR;
DEFINE_PGM_CTR(PM_FUNC_MQTT_SUBSCRIBE_CTR)                          D_FUNC_MQTT_SUBSCRIBE_CTR;
DEFINE_PGM_CTR(PM_FUNC_MQTT_INIT_CTR)                               D_FUNC_MQTT_INIT_CTR;
DEFINE_PGM_CTR(PM_FUNC_MQTT_CONNECTED_CTR)                          D_FUNC_MQTT_CONNECTED_CTR;
DEFINE_PGM_CTR(PM_FUNC_MQTT_DISCONNECTED_CTR)                       D_FUNC_MQTT_DISCONNECTED_CTR;
DEFINE_PGM_CTR(PM_FUNC_MQTT_COMMAND_CTR)                            D_FUNC_MQTT_COMMAND_CTR;
DEFINE_PGM_CTR(PM_FUNC_MQTT_SENDER_CTR)                             D_FUNC_MQTT_SENDER_CTR;
DEFINE_PGM_CTR(PM_FUNC_MQTT_HANDLERS_RESET_CTR)                     D_FUNC_MQTT_HANDLERS_RESET_CTR;
DEFINE_PGM_CTR(PM_FUNC_MQTT_HANDLERS_INIT_CTR)                      D_FUNC_MQTT_HANDLERS_INIT_CTR;
DEFINE_PGM_CTR(PM_FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD_CTR)        D_FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD_CTR;
DEFINE_PGM_CTR(PM_FUNC_SET_POWER_CTR)                               D_FUNC_SET_POWER_CTR;
DEFINE_PGM_CTR(PM_FUNC_SET_DEVICE_POWER_CTR)                        D_FUNC_SET_DEVICE_POWER_CTR;
DEFINE_PGM_CTR(PM_FUNC_SHOW_SENSOR_CTR)                             D_FUNC_SHOW_SENSOR_CTR;
DEFINE_PGM_CTR(PM_FUNC_RULES_PROCESS_CTR)                           D_FUNC_RULES_PROCESS_CTR;
DEFINE_PGM_CTR(PM_FUNC_SERIAL_CTR)                                  D_FUNC_SERIAL_CTR;
DEFINE_PGM_CTR(PM_FUNC_FREE_MEM_CTR)                                D_FUNC_FREE_MEM_CTR;
DEFINE_PGM_CTR(PM_FUNC_BUTTON_PRESSED_CTR)                          D_FUNC_BUTTON_PRESSED_CTR;
DEFINE_PGM_CTR(PM_FUNC_ENERGY_RESET_CTR)                            D_FUNC_ENERGY_RESET_CTR;
DEFINE_PGM_CTR(PM_FUNC_SENSOR_UPDATED_CTR)                          D_FUNC_SENSOR_UPDATED_CTR;
DEFINE_PGM_CTR(PM_FUNC_STATUS_MESSAGE_APPEND_CTR)                   D_FUNC_STATUS_MESSAGE_APPEND_CTR;
DEFINE_PGM_CTR(PM_FUNC_JSON_HARDWARE_APPEND_CTR)                    D_FUNC_JSON_HARDWARE_APPEND_CTR;
DEFINE_PGM_CTR(PM_FUNC_SET_CHANNELS_CTR)                            D_FUNC_SET_CHANNELS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_MAIN_BUTTON_CTR)                     D_FUNC_WEB_ADD_MAIN_BUTTON_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_BUTTON_CTR)                          D_FUNC_WEB_ADD_BUTTON_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_BUTTON_SYSTEM_SETTINGS_CTR)          D_FUNC_WEB_ADD_BUTTON_SYSTEM_SETTINGS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_HANDLER_CTR)                         D_FUNC_WEB_ADD_HANDLER_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ROOT_SEND_STYLE_CTR)                     D_FUNC_WEB_ROOT_SEND_STYLE_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ROOT_SEND_SCRIPT_CTR)                    D_FUNC_WEB_ROOT_SEND_SCRIPT_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ROOT_SCRIPT_JSON_FETCH_MODULEPARSING_CTR) D_FUNC_WEB_ROOT_SCRIPT_JSON_FETCH_MODULEPARSING_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ROOT_SEND_BODY_CTR)                      D_FUNC_WEB_ROOT_SEND_BODY_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ROOT_SEND_STATUS_CTR)                    D_FUNC_WEB_ROOT_SEND_STATUS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_ROOT_SHOWS_CTR)                      D_FUNC_WEB_ADD_ROOT_SHOWS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_JSON_FETCH_RESULT_CTR)               D_FUNC_WEB_ADD_JSON_FETCH_RESULT_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_JSON_DATA_FETCH_URL_CTR)             D_FUNC_WEB_ADD_JSON_DATA_FETCH_URL_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_ROOT_TABLE_ROWS_CTR)                 D_FUNC_WEB_ADD_ROOT_TABLE_ROWS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_ROOT_SCRIPT_CTR)                     D_FUNC_WEB_ADD_ROOT_SCRIPT_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_ROOT_STYLE_CTR)                      D_FUNC_WEB_ADD_ROOT_STYLE_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_APPEND_LOADTIME_ROOT_URLS_CTR)           D_FUNC_WEB_APPEND_LOADTIME_ROOT_URLS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_APPEND_LOADTIME_ROOT_RATES_CTR)          D_FUNC_WEB_APPEND_LOADTIME_ROOT_RATES_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_APPEND_RUNTIME_ROOT_URLS_CTR)            D_FUNC_WEB_APPEND_RUNTIME_ROOT_URLS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_APPEND_RUNTIME_ROOT_RATES_CTR)           D_FUNC_WEB_APPEND_RUNTIME_ROOT_RATES_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED_CTR)  D_FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_APPEND_ROOT_STATUS_TABLE_FORCED_CTR)     D_FUNC_WEB_APPEND_ROOT_STATUS_TABLE_FORCED_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_APPEND_ROOT_ADD_MAIN_BUTTONS_CTR)        D_FUNC_WEB_APPEND_ROOT_ADD_MAIN_BUTTONS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_APPEND_ROOT_BUTTONS_CTR)                 D_FUNC_WEB_APPEND_ROOT_BUTTONS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_PAGEINFORMATION_SEND_MODULE_CTR)         D_FUNC_WEB_PAGEINFORMATION_SEND_MODULE_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_COMMAND_CTR)                             D_FUNC_WEB_COMMAND_CTR;
// DEFINE_PGM_CTR(PM_FUNC_CHECK_POINTERS_CTR)                          D_FUNC_CHECK_POINTERS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_SYSTEM_INFO_CTR)                         D_FUNC_WEB_SYSTEM_INFO_CTR;
DEFINE_PGM_CTR(PM_FUNC_DEBUG_CONFIGURE_CTR)                         D_FUNC_DEBUG_CONFIGURE_CTR;

#endif // ENABLE_DEBUG_FUNCTION_NAMES
// Function names exposed to rules, need places here
DEFINE_PGM_CTR(PM_FUNC_EVENT_MOTION_STARTED_CTR)  D_FUNC_EVENT_MOTION_STARTED_CTR;
DEFINE_PGM_CTR(PM_FUNC_EVENT_INPUT_STATE_CHANGED_CTR)   D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR;
DEFINE_PGM_CTR(PM_FUNC_EVENT_SET_POWER_CHANGED_CTR)   D_FUNC_EVENT_SET_POWER_CHANGED_CTR;


//command source will be useful for rules, 
enum CommandSource { SRC_IGNORE, SRC_MQTT, SRC_RESTART, SRC_BUTTON, SRC_SWITCH, SRC_BACKLOG, SRC_SERIAL, SRC_WEBGUI, SRC_WEBCOMMAND, SRC_WEBCONSOLE, SRC_PULSETIMER,
                     SRC_TIMER, SRC_RULE, SRC_MAXPOWER, SRC_MAXENERGY, SRC_LIGHT, SRC_KNX, SRC_DISPLAY, SRC_WEMO, SRC_HUE, SRC_RETRY, SRC_MAX };
const char kCommandSource[] PROGMEM = "I|MQTT|Restart|Button|Switch|Backlog|Serial|WebGui|WebCommand|WebConsole|PulseTimer|Timer|Rule|MaxPower|MaxEnergy|Light|Knx|Display|Wemo|Hue|Retry";

//const uint8_t kDefaultRfCode[9] PROGMEM = { 0x21, 0x16, 0x01, 0x0E, 0x03, 0x48, 0x2E, 0x1A, 0x00 };


// enum COMMAND_LIST_SHARED_IDS {
//   COMMAND_NONE_ID=0,

//   // LAST OF LIST FOR COUNT
//   COMMAND_GENERIC_LIST_LENGTH_ID
// };



/*********************************************************************************************\
 * Extern global variables
\*********************************************************************************************/

extern uint8_t light_device;  // Light device number
extern uint8_t light_power_state;  // Light power
extern uint8_t rotary_changed; // Rotary switch changed



const char kWebColors[] PROGMEM =
  COLOR_TEXT "|" COLOR_BACKGROUND "|" 
  COLOR_FORM "|" COLOR_INPUT_TEXT "|" 
  COLOR_INPUT "|" COLOR_CONSOLE_TEXT "|" COLOR_CONSOLE "|"
  COLOR_TEXT_WARNING "|" COLOR_TEXT_SUCCESS "|"
  COLOR_BUTTON_TEXT "|" COLOR_BUTTON "|" COLOR_BUTTON_HOVER "|" COLOR_BUTTON2 "|" COLOR_BUTTON2_HOVER "|" 
  COLOR_BUTTON_RESET "|" COLOR_BUTTON_RESET_HOVER "|" COLOR_BUTTON_SAVE "|" 
  COLOR_BUTTON_SAVE_HOVER "|"
  COLOR_TIMER_TAB_TEXT "|" COLOR_TIMER_TAB_BACKGROUND "|" D_COL_TEXT_MODULE_TITLE_CTR;


/*********************************************************************************************\
 * Config - Flash
\*********************************************************************************************/
#ifdef ESP8266
extern "C" {
#include "spi_flash.h"
}
#include "eboot_command.h"
// extern "C" uint32_t _SPIFFS_end;
#endif


#ifdef ESP8266

  #if AUTOFLASHSIZE

  #else
    extern "C" uint32_t _FS_end;
  #endif  // AUTOFLASHSIZE
#endif

#include "1_TaskerManager/mTaskerInterface.h"

class mSettings :
  public mTaskerInterface
{
  public:
    mSettings(){};
    
    //overload
    int8_t Tasker(uint8_t function);

    static const char* PM_MODULE_CORE_SETTINGS_CTR;
    static const char* PM_MODULE_CORE_SETTINGS_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CORE_SETTINGS_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_CORE_SETTINGS_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_SETTINGS_ID; }
    
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mSettings);
    };
    #endif

    #ifdef ENABLE_DEBUG_FUNCTION_NAMES
    const char* GetTaskName(uint8_t task, char* buffer);
    #endif
    
    void init(void);

    uint16_t CountCharInCtr(const char* tosearch, char tofind);
    void ClearAllDeviceName(void);
    int16_t GetIndexOfNthCharPosition(const char* tosearch, char tofind, uint8_t occurance_count);

    // int8_t GetDeviceIDbyName(int8_t* class_id, int8_t* device_id, char* name_tofind);
    // int16_t GetDeviceIDbyName(const char* name_tofind, const char* haystack, int8_t* device_id, int8_t* class_id = nullptr);
    int16_t GetDeviceIDbyName(const char* name_tofind, int8_t device_id, int8_t class_id = -1);

    void Function_Template_Load();

    // #ifdef ESP8266
    //   // From libraries/EEPROM/EEPROM.cpp EEPROMClass
    //   const uint32_t SPIFFS_END = ((uint32_t)&_SPIFFS_end - 0x40200000) / SPI_FLASH_SEC_SIZE;

    //   // Version 4.2 config = eeprom area
    //   const uint32_t SETTINGS_LOCATION = SPIFFS_END;  // No need for SPIFFS as it uses EEPROM area
      
      uint8_t enable_web_logging_filtering = false;
      uint8_t enable_serial_logging_filtering = false;

    //   // Version 5.2 allow for more flash space
    //   const uint8_t CFG_ROTATES = 8;          // Number of flash sectors used (handles uploads)

    //   uint16_t settings_crc = 0;
    //   uint32_t settings_location = SETTINGS_LOCATION;
    //   uint8_t *settings_buffer = nullptr;
    // #else
    //   uint16_t settings_crc = 0;
    //   uint32_t settings_location = 0;
    //   uint8_t *settings_buffer = nullptr;
    // #endif

  
/*********************************************************************************************\
 * Config - Flash
\*********************************************************************************************/

// extern "C" {
// #include "spi_flash.h"
// }
// #include "eboot_command.h"

// #if defined(ARDUINO_ESP8266_RELEASE_2_3_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_1) || defined(ARDUINO_ESP8266_RELEASE_2_4_2) || defined(ARDUINO_ESP8266_RELEASE_2_5_0) || defined(ARDUINO_ESP8266_RELEASE_2_5_1) || defined(ARDUINO_ESP8266_RELEASE_2_5_2)

//   extern "C" uint32_t _SPIFFS_end;
//   // From libraries/EEPROM/EEPROM.cpp EEPROMClass
//   const uint32_t SPIFFS_END = ((uint32_t)&_SPIFFS_end - 0x40200000) / SPI_FLASH_SEC_SIZE;

// #else  //Core > 2.5.2 and STAGE

//   #if AUTOFLASHSIZE

//     #include "flash_hal.h"

//     // From libraries/EEPROM/EEPROM.cpp EEPROMClass
//     const uint32_t SPIFFS_END = (_SPIFFS_end - 0x40200000) / SPI_FLASH_SEC_SIZE;

//   #else

//     // From libraries/EEPROM/EEPROM.cpp EEPROMClass
//     const uint32_t SPIFFS_END = ((uint32_t)&_SPIFFS_end - 0x40200000) / SPI_FLASH_SEC_SIZE;

//   #endif  // AUTOFLASHSIZE

// #endif  // All cores < pre-2.6.0

// Version 4.2 config = eeprom area
// const uint32_t SETTINGS_LOCATION = SPIFFS_END;  // No need for SPIFFS as it uses EEPROM area


#ifdef ESP8266

  #if AUTOFLASHSIZE

    #include "flash_hal.h"

    // From libraries/EEPROM/EEPROM.cpp EEPROMClass
    const uint32_t SPIFFS_END = (FS_end - 0x40200000) / SPI_FLASH_SEC_SIZE;

  #else

    // extern "C" uint32_t _FS_end;
    // From libraries/EEPROM/EEPROM.cpp EEPROMClass
    const uint32_t SPIFFS_END = ((uint32_t)&_FS_end - 0x40200000) / SPI_FLASH_SEC_SIZE;

  #endif  // AUTOFLASHSIZE

// Version 4.2 config = eeprom area
const uint32_t SETTINGS_LOCATION = SPIFFS_END;  // No need for SPIFFS as it uses EEPROM area

#endif  // ESP8266



#ifdef ESP8266
// Version 5.2 allow for more flash space
const uint8_t CFG_ROTATES = 8;          // Number of flash sectors used (handles uploads)

uint32_t settings_location = SETTINGS_LOCATION;
// uint32_t settings_crc = 0;
uint32_t settings_crc32 = 0;
uint8_t *settings_buffer = nullptr;
uint32_t rtc_reboot_crc = 0;
#endif // ESP8266

int16_t GetFunctionIDbyFriendlyName(const char* c);

void SettingsLoad_CheckSuccessful();

  void RtcRebootReset(void);
  uint32_t GetRtcSettingsCrc(void);
  void RtcSettingsSave(void);
  void RtcSettingsLoad(void);
  bool RtcSettingsValid(void);
  uint32_t GetRtcRebootCrc(void);
  void RtcRebootSave(void);
  void RtcRebootLoad(void);
  bool RtcRebootValid(void);

  void SetFlashModeDout(void);
  void SettingsBufferFree(void);
  bool SettingsBufferAlloc(void);
  
  void SettingsSaveAll(void);
  uint32_t GetSettingsAddress(void);
  void SettingsSave(uint8_t rotate);
  void SettingsLoad(void);
  void SettingsErase(uint8_t type);
  bool SettingsEraseConfig(void) ;
  void SettingsSdkErase(void);
  void SettingsDefault(void);
  void SystemSettings_DefaultHeader(void);
  void SystemSettings_DefaultBody(void);
  void SystemSettings_DefaultBody_System();
  void SystemSettings_DefaultBody_Network();
  void SystemSettings_DefaultBody_WebServer();
  void SystemSettings_DefaultBody_MQTT();
  void SystemSettings_DefaultBody_TelePeriods();
  void SystemSettings_DefaultBody_Time();
  void SystemSettings_DefaultBody_Weight();
  void SystemSettings_DefaultBody_Lighting();
  void SystemSettings_DefaultBody_PulseCounters();
  void SystemSettings_DefaultBody_Sensors();
  void SystemSettings_DefaultBody_Power();
  void SystemSettings_DefaultBody_Energy();
  void SystemSettings_DefaultBody_Drivers();
  void SystemSettings_DefaultBody_Displays();
  void SystemSettings_DefaultBody_Rules();



uint16_t GetCfgCrc16(uint8_t *bytes, uint32_t size);
uint16_t GetSettingsCrc(void);
uint32_t GetCfgCrc32(uint8_t *bytes, uint32_t size);
uint32_t GetSettingsCrc32(void);


  void SettingsResetStd(void);
  void SettingsResetDst(void);

  // void SettingsDefaultSet_5_13_1c(void);
  // void SettingsDefaultWebColor(void)  ; 
  // void SettingsDelta(void);
  // void SettingsMichael_Echo(void);

  // enum WebColors {
  //   COL_TEXT, COL_BACKGROUND, COL_FORM,
  //   COL_INPUT_TEXT, COL_INPUT, COL_CONSOLE_TEXT, COL_CONSOLE,
  //   COL_TEXT_WARNING, COL_TEXT_SUCCESS,
  //   COL_BUTTON_TEXT, COL_BUTTON, COL_BUTTON_HOVER, COL_BUTTON2, COL_BUTTON2_HOVER, COL_BUTTON_RESET, COL_BUTTON_RESET_HOVER, COL_BUTTON_SAVE, COL_BUTTON_SAVE_HOVER,
  //   COL_TIMER_TAB_TEXT, COL_TIMER_TAB_BACKGROUND, COL_TEXT_MODULE_TITLE,
  //   COL_LAST };



typedef union {
  uint8_t data;
  struct {
    uint8_t network_down : 1;
    uint8_t mqtt_down : 1;
    uint8_t wifi_down : 1;
    uint8_t eth_down : 1;
    uint8_t spare04 : 1;
    uint8_t spare05 : 1;
    uint8_t spare06 : 1;
    uint8_t spare07 : 1;
  };
} StateBitfield;



// /*********************************************************************************************\
//  * RTC memory
// \*********************************************************************************************/

const uint16_t RTC_MEM_VALID = 0xA55A;

uint32_t rtc_settings_crc = 0;

#define PARAM8_SIZE 18            // Number of param bytes (SetOption)


typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint32_t data;                           // Allow bit manipulation using SetOption
  struct {                                 // SetOption0 .. SetOption31
    uint32_t save_state : 1;               // bit 0              - SetOption0  - Save power state and use after restart
    uint32_t button_restrict : 1;          // bit 1              - SetOption1  - Control button multipress
    uint32_t value_units : 1;              // bit 2              - SetOption2  - Add units to JSON status messages
    uint32_t mqtt_enabled : 1;             // bit 3              - SetOption3  - Control MQTT
    uint32_t mqtt_response : 1;            // bit 4              - SetOption4  - Switch between MQTT RESULT or COMMAND
    uint32_t mqtt_power_retain : 1;        // bit 5              - CMND_POWERRETAIN
    uint32_t mqtt_button_retain : 1;       // bit 6              - CMND_BUTTONRETAIN
    uint32_t mqtt_switch_retain : 1;       // bit 7              - CMND_SWITCHRETAIN
    uint32_t temperature_conversion : 1;   // bit 8              - SetOption8  - Switch between Celsius or Fahrenheit
    uint32_t mqtt_sensor_retain : 1;       // bit 9              - CMND_SENSORRETAIN
    uint32_t mqtt_offline : 1;             // bit 10             - SetOption10 - Control MQTT LWT message format
    uint32_t button_swap : 1;              // bit 11 (v5.1.6)    - SetOption11 - Swap button single and double press functionality
    uint32_t stop_flash_rotate : 1;        // bit 12 (v5.2.0)    - SetOption12 - Switch between dynamic or fixed slot flash save location
    uint32_t button_single : 1;            // bit 13 (v5.4.0)    - SetOption13 - Support only single press to speed up button press recognition
    uint32_t interlock : 1;                // bit 14 (v5.6.0)    - CMND_INTERLOCK
    uint32_t pwm_control : 1;              // bit 15 (v5.8.1)    - SetOption15 - Switch between commands PWM or COLOR/DIMMER/CT/CHANNEL
    uint32_t ws_clock_reverse : 1;         // bit 16 (v5.8.1)    - SetOption16 - Switch between clockwise or counter-clockwise
    uint32_t decimal_text : 1;             // bit 17 (v5.8.1)    - SetOption17 - Switch between decimal or hexadecimal output
    uint32_t light_signal : 1;             // bit 18 (v5.10.0c)  - SetOption18 - Pair light signal with CO2 sensor
    uint32_t hass_discovery : 1;           // bit 19 (v5.11.1a)  - SetOption19 - Control Home Assistantautomatic discovery (See SetOption59)
    uint32_t not_power_linked : 1;         // bit 20 (v5.11.1f)  - SetOption20 - Control power in relation to Dimmer/Color/Ct changes
    uint32_t no_power_on_check : 1;        // bit 21 (v5.11.1i)  - SetOption21 - Show voltage even if powered off
    uint32_t mqtt_serial : 1;              // bit 22 (v5.12.0f)  - CMND_SERIALSEND and CMND_SERIALLOG
    uint32_t mqtt_serial_raw : 1;          // bit 23 (v6.1.1c)   - CMND_SERIALSEND3
    uint32_t pressure_conversion : 1;      // bit 24 (v6.3.0.2)  - SetOption24 - Switch between hPa or mmHg pressure unit
    uint32_t knx_enabled : 1;              // bit 25 (v5.12.0l)  - CMND_KNX_ENABLED
    uint32_t device_index_enable : 1;      // bit 26 (v5.13.1a)  - SetOption26 - Switch between POWER or POWER1
    uint32_t knx_enable_enhancement : 1;   // bit 27 (v5.14.0a)  - CMND_KNX_ENHANCED
    uint32_t rf_receive_decimal : 1;       // bit 28 (v6.0.0a)   - SetOption28 - RF receive data format
    uint32_t ir_receive_decimal : 1;       // bit 29 (v6.0.0a)   - SetOption29 - IR receive data format
    uint32_t hass_light : 1;               // bit 30 (v6.0.0b)   - SetOption30 - Enforce HAss autodiscovery as light
    uint32_t global_state : 1;             // bit 31 (v6.1.0)    - SetOption31 - Control link led blinking
  };
} SysBitfield_System;


// typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
//   uint32_t data;                           // Allow bit manipulation using SetOption
//   struct {                                 // SetOption82 .. SetOption113
//     uint32_t alexa_ct_range : 1;           // bit 0 (v8.1.0.2)   - SetOption82 - Reduced CT range for Alexa
//     uint32_t zigbee_use_names : 1;         // bit 1 (v8.1.0.4)   - SetOption83 - Use FriendlyNames instead of ShortAddresses when possible
//     uint32_t awsiot_shadow : 1;            // bit 2 (v8.1.0.5)   - SetOption84 - (AWS IoT) publish MQTT state to a device shadow
//     uint32_t device_groups_enabled : 1;    // bit 3 (v8.1.0.9)   - SetOption85 - Enable Device Groups
//     uint32_t led_timeout : 1;              // bit 4 (v8.1.0.9)   - SetOption86 - PWM Dimmer Turn brightness LED's off 5 seconds after last change
//     uint32_t powered_off_led : 1;          // bit 5 (v8.1.0.9)   - SetOption87 - PWM Dimmer Turn red LED on when powered off
//     uint32_t remote_device_mode : 1;       // bit 6 (v8.1.0.9)   - SetOption88 - Enable relays in separate device groups/PWM Dimmer Buttons control remote devices
//     uint32_t zigbee_distinct_topics : 1;   // bit 7 (v8.1.0.10)  - SetOption89 - Distinct MQTT topics per device for Zigbee (#7835)
//     uint32_t only_json_message : 1;        // bit 8 (v8.2.0.3)   - SetOption90 - Disable non-json MQTT response
//     uint32_t fade_at_startup : 1;          // bit 9 (v8.2.0.3)   - SetOption91 - Enable light fading at start/power on
//     uint32_t pwm_ct_mode : 1;              // bit 10 (v8.2.0.4)  - SetOption92 - Set PWM Mode from regular PWM to ColorTemp control (Xiaomi Philips ...)
//     uint32_t compress_rules_cpu : 1;       // bit 11 (v8.2.0.6)  - SetOption93 - Keep uncompressed rules in memory to avoid CPU load of uncompressing at each tick
//     uint32_t max6675 : 1;                  // bit 12 (v8.3.1.2)  - SetOption94 - Implement simpler MAX6675 protocol instead of MAX31855
//     uint32_t spare13 : 1;
//     uint32_t spare14 : 1;
//     uint32_t spare15 : 1;
//     uint32_t spare16 : 1;
//     uint32_t spare17 : 1;
//     uint32_t spare18 : 1;
//     uint32_t spare19 : 1;
//     uint32_t spare20 : 1;
//     uint32_t spare21 : 1;
//     uint32_t spare22 : 1;
//     uint32_t spare23 : 1;
//     uint32_t spare24 : 1;
//     uint32_t spare25 : 1;
//     uint32_t spare26 : 1;
//     uint32_t spare27 : 1;
//     uint32_t spare28 : 1;
//     uint32_t spare29 : 1;
//     uint32_t spare30 : 1;
//     uint32_t spare31 : 1;                  // bit 31
//   };
// } SysBitfield4;

// Sensors
typedef union {
  uint32_t data;                           // Allow bit manipulation
  struct {
    uint32_t spare00 : 1;
    uint32_t spare01 : 1;
    uint32_t spare02 : 1;
    uint32_t spare03 : 1;
    uint32_t spare04 : 1;
    uint32_t spare05 : 1;
    uint32_t calc_resolution : 3;
    uint32_t weight_resolution : 2;
    uint32_t frequency_resolution : 2;
    uint32_t axis_resolution : 2;
    uint32_t current_resolution : 2;
    uint32_t voltage_resolution : 2;
    uint32_t wattage_resolution : 2;
    uint32_t emulation : 2;
    uint32_t energy_resolution : 3;
    uint32_t pressure_resolution : 2;
    uint32_t humidity_resolution : 2;
    uint32_t temperature_resolution : 2;
  };
} SysBitfield_Power;


typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint32_t data;                           // Allow bit manipulation using SetOption
  struct {                                 // SetOption50 .. SetOption81
    uint32_t timers_enable : 1;            // bit 0 (v6.1.1b)    - CMND_TIMERS
    uint32_t user_esp8285_enable : 1;      // bit 1 (v6.1.1.14)  - SetOption51 - Enable ESP8285 user GPIO's
    uint32_t time_append_timezone : 1;     // bit 2 (v6.2.1.2)   - SetOption52 - Append timezone to JSON time
    uint32_t gui_hostname_ip : 1;          // bit 3 (v6.2.1.20)  - SetOption53 - Show hostanme and IP address in GUI main menu
    uint32_t tuya_apply_o20 : 1;           // bit 4 (v6.3.0.4)   - SetOption54 - Apply SetOption20 settings to Tuya device
    uint32_t mdns_enabled : 1;             // bit 5 (v6.4.1.4)   - SetOption55 - Control mDNS service
    uint32_t use_wifi_scan : 1;            // bit 6 (v6.3.0.10)  - SetOption56 - Scan wifi network at restart for configured AP's
    uint32_t use_wifi_rescan : 1;          // bit 7 (v6.3.0.10)  - SetOption57 - Scan wifi network every 44 minutes for configured AP's
    uint32_t receive_raw : 1;              // bit 8 (v6.3.0.11)  - SetOption58 - Add IR Raw data to JSON message
    uint32_t hass_tele_on_power : 1;       // bit 9 (v6.3.0.13)  - SetOption59 - Send tele/%topic%/STATE in addition to stat/%topic%/RESULT
    uint32_t sleep_normal : 1;             // bit 10 (v6.3.0.15) - SetOption60 - Enable normal sleep instead of dynamic sleep
    uint32_t button_switch_force_local : 1;// bit 11 (v6.3.0.16) - SetOption61 - Force local operation when button/switch topic is set
    uint32_t no_hold_retain : 1;           // bit 12 (v6.4.1.19) - SetOption62 - Don't use retain flag on HOLD messages
    uint32_t no_power_feedback : 1;        // bit 13 (v6.5.0.9)  - SetOption63 - Don't scan relay power state at restart
    uint32_t use_underscore : 1;           // bit 14 (v6.5.0.12) - SetOption64 - Enable "_" instead of "-" as sensor index separator
    uint32_t fast_power_cycle_disable : 1; // bit 15 (v6.6.0.20) - SetOption65 - Disable fast power cycle detection for device reset
    uint32_t tuya_serial_mqtt_publish : 1; // bit 16 (v6.6.0.21) - SetOption66 - Enable TuyaMcuReceived messages over Mqtt
    uint32_t buzzer_enable : 1;            // bit 17 (v6.6.0.1)  - SetOption67 - Enable buzzer when available
    uint32_t pwm_multi_channels : 1;       // bit 18 (v6.6.0.3)  - SetOption68 - Enable multi-channels PWM instead of Color PWM
    uint32_t ex_tuya_dimmer_min_limit : 1; // bit 19 (v6.6.0.5)  - SetOption69 - Limits Tuya dimmers to minimum of 10% (25) when enabled.
    uint32_t energy_weekend : 1;           // bit 20 (v6.6.0.8)  - CMND_TARIFF
    uint32_t dds2382_model : 1;            // bit 21 (v6.6.0.14) - SetOption71 - Select different Modbus registers for Active Energy (#6531)
    uint32_t hardware_energy_total : 1;    // bit 22 (v6.6.0.15) - SetOption72 - Enable hardware energy total counter as reference (#6561)
    uint32_t ex_cors_enabled : 1;          // bit 23 (v7.0.0.1)  - SetOption73 - Enable HTTP CORS
    uint32_t ds18x20_internal_pullup : 1;  // bit 24 (v7.0.0.1)  - SetOption74 - Enable internal pullup for single DS18x20 sensor
    uint32_t grouptopic_mode : 1;          // bit 25 (v7.0.0.1)  - SetOption75 - GroupTopic replaces %topic% (0) or fixed topic cmnd/grouptopic (1)
    uint32_t bootcount_update : 1;         // bit 26 (v7.0.0.4)  - SetOption76 - Enable incrementing bootcount when deepsleep is enabled
    uint32_t slider_dimmer_stay_on : 1;    // bit 27 (v7.0.0.6)  - SetOption77 - Do not power off if slider moved to far left
    uint32_t compatibility_check : 1;      // bit 28 (v7.1.2.6)  - SetOption78 - Disable OTA compatibility check
    uint32_t counter_reset_on_tele : 1;    // bit 29 (v8.1.0.1)  - SetOption79 - Enable resetting of counters after telemetry was sent
    // uint32_t shutter_mode : 1;             // bit 30 (v6.6.0.14) - SetOption80 - Enable shutter support
    // uint32_t pcf8574_ports_inverted : 1;   // bit 31 (v6.6.0.14) - SetOption81 - Invert all ports on PCF8574 devices
    uint32_t network_wifi : 1;             // bit 13 (v8.3.1.3)  - CMND_WIFI
    uint32_t network_ethernet : 1;         // bit 14 (v8.3.1.3)  - CMND_ETHERNET


  };
} SysBitfield_Network;



// typedef union {
//   uint16_t data;
//   struct {
//     uint16_t hemis : 1;                    // bit 0        = 0=Northern, 1=Southern Hemisphere (=Opposite DST/STD)
//     uint16_t week : 3;                     // bits 1 - 3   = 0=Last week of the month, 1=First, 2=Second, 3=Third, 4=Fourth
//     uint16_t month : 4;                    // bits 4 - 7   = 1=Jan, 2=Feb, ... 12=Dec
//     uint16_t dow : 3;                      // bits 8 - 10  = day of week, 1=Sun, 2=Mon, ... 7=Sat
//     uint16_t hour : 5;                     // bits 11 - 15 = 0-23
//   };
// } TimeRule;

typedef union {
  uint32_t data;
  struct {
    uint32_t time : 11;                    // bits 0 - 10 = minutes in a day
    uint32_t window : 4;                   // bits 11 - 14 = minutes random window
    uint32_t repeat : 1;                   // bit 15
    uint32_t days : 7;                     // bits 16 - 22 = week day mask
    uint32_t device : 4;                   // bits 23 - 26 = 16 devices
    uint32_t power : 2;                    // bits 27 - 28 = 4 power states - Off, On, Toggle, Blink or Rule
    uint32_t mode : 2;                     // bits 29 - 30 = timer modes - 0 = Scheduler, 1 = Sunrise, 2 = Sunset
    uint32_t arm : 1;                      // bit 31
  };
} Timer;

typedef union {
  uint16_t data;
  struct {
    uint16_t pinmode : 3;                   // Pin mode (1 through 6)
    uint16_t pullup : 1;                    // Enable internal weak pull-up resistor
    uint16_t saved_state : 1;               // Save output state, if used.
    uint16_t int_report_mode : 2;           // Interrupt reporting mode 0 = immediate telemetry & event, 1 = immediate event only, 2 = immediate telemetry only
    uint16_t int_report_defer : 4;          // Number of interrupts to ignore until reporting (default 0, max 15)
    uint16_t int_count_en : 1;              // Enable interrupt counter for this pin
    uint16_t int_retain_flag : 1;           // Report if interrupt occured for pin in next teleperiod
    uint16_t spare13 : 1;
    uint16_t spare14 : 1;
    uint16_t spare15 : 1;
  };
} Mcp230xxCfg;

// typedef union {
//   uint8_t data;
//   struct {
//     uint8_t spare0 : 1;
//     uint8_t spare1 : 1;
//     uint8_t spare2 : 1;
//     uint8_t spare3 : 1;
//     uint8_t spare4 : 1;
//     uint8_t spare5 : 1;
//     uint8_t spare6 : 1;
//     uint8_t mhz19b_abc_disable : 1;        // Disable ABC (Automatic Baseline Correction for MHZ19(B) (0 = Enabled (default), 1 = Disabled with Sensor15 command)
//   };
// } SensorCfg1;

// OLD LIST IN RTC, CREAT A NEW ONE IN SETTINGS
// struct EnergyUsage{
//   uint32_t usage1_kWhtotal;
//   uint32_t usage2_kWhtotal;
//   uint32_t return1_kWhtotal;
//   uint32_t return2_kWhtotal;
//   uint32_t last_return_kWhtotal;
//   uint32_t last_usage_kWhtotal;
  
  
//   uint8_t       energy_power_delta;        // 33F
//   unsigned long energy_power_calibration;  // 364
//   unsigned long energy_voltage_calibration;  // 368
//   unsigned long energy_current_calibration;  // 36C
//   unsigned long energy_kWhtoday;           // 370
//   unsigned long energy_kWhyesterday;       // 374
//   uint16_t      energy_kWhdoy;             // 378
//   uint16_t      energy_min_power;          // 37A
//   uint16_t      energy_max_power;          // 37C
//   uint16_t      energy_min_voltage;        // 37E
//   uint16_t      energy_max_voltage;        // 380
//   uint16_t      energy_min_current;        // 382
//   uint16_t      energy_max_current;        // 384
//   uint16_t      energy_max_power_limit;    // 386 MaxPowerLimit
//   uint16_t      energy_max_power_limit_hold;         // 388 MaxPowerLimitHold
//   uint16_t      energy_max_power_limit_window;       // 38A MaxPowerLimitWindow
//   uint16_t      energy_max_power_safe_limit;         // 38C MaxSafePowerLimit
//   uint16_t      energy_max_power_safe_limit_hold;    // 38E MaxSafePowerLimitHold
//   uint16_t      energy_max_power_safe_limit_window;  // 390 MaxSafePowerLimitWindow
//   uint16_t      energy_max_energy;         // 392 MaxEnergy
//   uint16_t      energy_max_energy_start;   // 394 MaxEnergyStart
//   uint32_t      energy_kWhtotal_time;      // 7B4
//   unsigned long energy_frequency_calibration;  // 7C8
//   unsigned long energy_kWhtotal;           // 554
//   uint16_t      tariff[4][2];              // E30

//   //char name_buffer[100]; // To hold the names of the sensor as list
// };// EnergyUsage;
// #endif

struct EnergyUsageNew{
  uint32_t usage1_kWhtotal;
  uint32_t usage2_kWhtotal;
  uint32_t return1_kWhtotal;
  uint32_t return2_kWhtotal;
  uint32_t last_return_kWhtotal;
  uint32_t last_usage_kWhtotal;
  
  
  uint8_t       energy_power_delta;        // 33F
  unsigned long energy_power_calibration;  // 364
  unsigned long energy_voltage_calibration;  // 368
  unsigned long energy_current_calibration;  // 36C
  unsigned long energy_kWhtoday;           // 370
  unsigned long energy_kWhyesterday;       // 374
  uint16_t      energy_kWhdoy;             // 378
  uint16_t      energy_min_power;          // 37A
  uint16_t      energy_max_power;          // 37C
  uint16_t      energy_min_voltage;        // 37E
  uint16_t      energy_max_voltage;        // 380
  uint16_t      energy_min_current;        // 382
  uint16_t      energy_max_current;        // 384
  uint16_t      energy_max_power_limit;    // 386 MaxPowerLimit
  uint16_t      energy_max_power_limit_hold;         // 388 MaxPowerLimitHold
  uint16_t      energy_max_power_limit_window;       // 38A MaxPowerLimitWindow
  uint16_t      energy_max_power_safe_limit;         // 38C MaxSafePowerLimit
  uint16_t      energy_max_power_safe_limit_hold;    // 38E MaxSafePowerLimitHold
  uint16_t      energy_max_power_safe_limit_window;  // 390 MaxSafePowerLimitWindow
  uint16_t      energy_max_energy;         // 392 MaxEnergy
  uint16_t      energy_max_energy_start;   // 394 MaxEnergyStart
  uint32_t      energy_kWhtotal_time;      // 7B4
  unsigned long energy_frequency_calibration;  // 7C8
  unsigned long energy_kWhtotal;           // 554
  uint16_t      tariff[4][2];              // E30

  //char name_buffer[100]; // To hold the names of the sensor as list
};// EnergyUsage;



typedef union {
  uint16_t data;
  struct {
    uint16_t clear_on_reboot : 1;
    uint16_t reserved : 15;     
  };
} SysBitfield_Animations;

typedef union {
  uint16_t data;
  struct {
    uint16_t spare0 : 1;
    uint16_t reserved : 15;     
  };
} SysBitfield_Lighting;

struct LightSettings{
  uint8_t light_brightness_as_percentage;
  uint8_t light_fade;          // 4A1
  uint8_t light_speed;         // 4A2
  uint8_t light_scheme;        // 4A3
  uint8_t light_width;         // 4A4
  uint16_t light_rotation;     // 39E
  uint16_t light_pixels;       // 496
  // uint8_t light_color[5];      // 498
  uint8_t light_correction;    // 49D
  uint8_t light_dimmer;        // 49E
  uint16_t light_wakeup;       // 4A6
  // uint8_t ws_color[4][3];      // 475
  // uint8_t ws_width[3];         // 481
  // uint8_t       rgbwwTable[5];             // 71A
  uint8_t type;
  uint16_t size;
};


typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint8_t data;                           // Allow bit manipulation using SetOption
  struct {                                 // SetOption0 .. SetOption31
    uint8_t mqtt_retain : 1;               // bit 0              - SetOption0  - Save power state and use after restart
    uint8_t button_restrict : 1;          // bit 1              - SetOption1  - Control button multipress
    uint8_t decimal_precision : 2;        // bit 2,3   4 levels [0,1,2,3]

  };
} SysBitfield_Sensors;



// a flag status group showing how succesful the boot was
// 3 state template, used, succuss, fail, none
typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint16_t data;                           // Allow bit manipulation using SetOption
  struct {                                 // SetOption0 .. SetOption31
    uint8_t module_template_used : 1;               // bit 0              - SetOption0  - Save power state and use after restart
    uint8_t module_template_parse_success : 1;          // bit 1              - SetOption1  - Control button multipress
    uint8_t function_template_parse_success : 2;          // bit 1              - SetOption1  - Control button multipress
    uint8_t decimal_precision : 2;        // bit 2,3   4 levels [0,1,2,3]
    uint8_t mdns_started_succesfully: 1;
    uint8_t rules_template_parse_success : 1;
  };
} SysBitfield_BootStatus;

SysBitfield_BootStatus boot_status;

#define RESET_BOOT_STATUS() memset(&pCONT_set->boot_status,0,sizeof(pCONT_set->boot_status))


struct SensorSettings{
  uint16_t      configperiod_secs;
  uint16_t      teleperiod_secs;
  uint16_t      ifchanged_secs;
  // Percentage of possible values that signify a large enough change has occured worth reporting.
  // A value of '0' percent means anything at all
  uint8_t       ifchanged_change_percentage_threshold;
  uint8_t       teleperiod_json_level;
  uint8_t       ifchanged_json_level;
  uint8_t       teleperiod_retain_flag;
  int16_t       altitude;                  
  int           latitude;                  
  int           longitude;               
  SysBitfield_Sensors flags;
};

struct DisplaySettings{
  uint8_t       model;             // 2D2
  uint8_t       mode;              // 2D3
  uint8_t       refresh;           // 2D4
  uint8_t       rows;              // 2D5
  // uint8_t       cols[2];           // 2D6
  // uint8_t       address[8];        // 2D8
  uint8_t       dimmer;            // 2E0
  uint8_t       size;              // 2E1
  uint8_t       font;              // 312
  uint8_t       rotate;            // 2FA
};



//hold the parsed template from use
// struct TemplateCurrent{
//   // For testing only
//   char value_ctr[150]; //as set by user, if not set, assume generic
  
// };


// #define PALETTE_USER_NAME_LIST_LENGTH 300
struct AnimationSettings{
  // HsbColour map ids
  uint8_t palette_hsbid_users_colour_map[20*20];//20][20]; 
  //rgbcct user values
  uint8_t palette_rgbcct_users_colour_map[5*5];//20][20]; 
  //generic variable buffer
  uint8_t palette_encoded_users_colour_map[200];//20][20]; 
  // Change names to be a klist =   // char    palette_user_variable_name_ctr[20][20];

  //move into devicelist?
  //hmm, maybe not, device list needs to be moved into settings storage first, currently runtime variable
  // char    palette_user_variable_name_list_ctr[PALETTE_USER_NAME_LIST_LENGTH]; //100 less


  // Active pixels in each user palette, can be replaced by setting colour_map is inactive, use function to count active/hold it in ram not memory
  // uint8_t palette_user_amounts[20]; // move to searching not none type id
  uint8_t animation_mode;
  uint8_t animation_palette;
  //struct transition
  uint8_t animation_transition_order; 
  uint8_t animation_transition_method;
  uint16_t animation_transition_time_ms; //TBD save as seconds
  uint32_t animation_transition_rate_ms; //TBD save as seconds
  uint8_t transition_pixels_to_update_as_number;
};


// Buffer that stores names of sensors as delimeter list
#define DEVICENAMEBUFFER_NAME_INDEX_LENGTH  50
#ifndef DEVICENAMEBUFFER_NAME_BUFFER_LENGTH // Memory reduction
#define DEVICENAMEBUFFER_NAME_BUFFER_LENGTH 400 
#endif // DEVICENAMEBUFFER_NAME_BUFFER_LENGTH
struct DeviceNameBuffer{ // size(230)
  // delimeter name list
  char name_buffer[DEVICENAMEBUFFER_NAME_BUFFER_LENGTH];
  // index array that holds name_list sensor (class name, sensor number)
  int16_t class_id[DEVICENAMEBUFFER_NAME_INDEX_LENGTH]; //hold class id
  int8_t  device_id[DEVICENAMEBUFFER_NAME_INDEX_LENGTH];  //max of X sensors per module
};



#define MODULE_TEMPLATE_MAX_SIZE 300
struct Template_Config{
  uint8_t flags;
  uint8_t       base;        // 71F
  mytmplt       hardware;             // 720  29 bytes    parsed user template
  // char          full_ctr[MODULE_TEMPLATE_SIZE]; //for testing

};

struct SystemName{ 
  char          friendly[33]; // Used in titles, set by templates "FriendlyName"
  char          device[30];   // USed as mqtt topic by default, set by templates "Name"
};

struct SettingsMQTT{
  // char topic_prefix[50]; //temp remove
  
   char          host[33];             // 1E9 - Keep together with below as being copied as one chunck with reset 6
   uint16_t      port;                 // 20A - Keep together
  char          client[33];           // 20C - Keep together
  char          user[33];             // 22D - Keep together
  char          pwd[33];              // 24E - Keep together
  char          topic[33];            // 26F - Keep together with above items as being copied as one chunck with reset 6
  // char          button_topic[33];          // 290
  // char          grptopic[33];         // 2B1
  // char          prefix[3][11];        // 07C
  // uint8_t       fingerprint[2][20];   // 1AD
  uint16_t      retry;                // 396
  // char          fulltopic[100];       // 558
  
  char lwt_topic[30];
  char client_name[40]; 
  char hostname_ctr[20];
  char prefixtopic[50]; // "<devicename>/User extras?"

};

#ifdef ESP32
typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint32_t data;
  struct {
    uint32_t stream : 1;
    uint32_t mirror : 1;
    uint32_t flip : 1;
    uint32_t rtsp : 1;
    uint32_t spare4 : 1;
    uint32_t spare5 : 1;
    uint32_t spare6 : 1;
    uint32_t spare7 : 1;
    uint32_t spare8 : 1;
    uint32_t spare9 : 1;
    uint32_t spare10 : 1;
    uint32_t spare11 : 1;
    uint32_t spare12 : 1;
    uint32_t spare13 : 1;
    uint32_t spare14 : 1;
    uint32_t spare15 : 1;
    uint32_t spare16 : 1;
    uint32_t spare17 : 1;
    uint32_t spare18 : 1;
    uint32_t contrast : 3;
    uint32_t brightness : 3;
    uint32_t saturation : 3;
    uint32_t resolution : 4;
  };
} WebCamCfg;
#endif // esp32


struct SYSCFG {
  // Header (partial loading during boot)
  uint16_t      cfg_holder;                // 000 v6 header
  uint16_t      cfg_size;                  // 002
  unsigned long save_flag;                 // 004
  unsigned long version;                   // 008
  uint16_t      bootcount;                 // 00C
  uint16_t      cfg_crc;                   // 00E
  // Body
  // System/Core
  uint16_t      bootcount_errors_only;     // E01
  uint8_t       module;                    // 474
  uint8_t       last_module;               // 399
  // Templates
  Template_Config user_template2;
  SystemName      system_name;

  SysBitfield_System   flag_system;                      // 010
  int16_t       save_data;                 // 014
  myio          module_pins;                     // 484     
  // char          ota_url[101];              // 017
  uint8_t       baudrate;                  // 09D
  uint8_t       rule_stop;                 // 1A7
  //LoggingSettings logging;
  uint8_t       seriallog_level;           // 09E
  uint16_t      syslog_port;               // 1A8
  uint8_t       syslog_level;              // 1AA
  uint8_t       weblog_level;              // 1AC
  uint8_t       telnetlog_level;              // 1AC
  uint8_t       log_time_isshort;

  
  uint8_t enable_sleep;        // E03
  char          serial_delimiter;          // 451
  uint8_t       sbaudrate;                 // 452
  uint8_t       sleep;                     // 453

// remove params, to be handled as bitmap flags
  uint8_t       param[PARAM8_SIZE];        // 2FC  SetOption32 .. SetOption49


  // Network
  uint8_t       sta_config;                // 09F
  uint8_t       sta_active;                // 0A0
  char          sta_ssid[3][33];           // 0A1 - Keep together with sta_pwd as being copied as one chunck with reset 5
  char          sta_pwd[3][65];            // 0E3 - Keep together with sta_ssid as being copied as one chunck with reset 5
  char          hostname[33];              // 165
  char          syslog_host[33];           // 186
  uint32_t      ip_address[4];             // 544
  uint8_t wifi_channel;
  uint8_t       wifi_bssid[6];             // F0A
  SysBitfield_Network  flag_network;                     // 3A0
  // Webserver
  uint8_t       webserver;                 // 1AB
  char          web_password[33];          // 4A9
  uint16_t      web_refresh;               // 7CC
  // uint8_t       web_color[25][3];          // 73E   //UNSURE OF LENGTH
  // MQTT
  SettingsMQTT  mqtt;
  // Time
  int8_t        timezone;                  // 016
  uint8_t       timezone_minutes;          // 66D

  
  uint8_t       ina219_mode;               // 531

  // StateBitfield global_state;                 // Global states (currently Wifi and Mqtt) (8 bits)

  uint16_t      mqtt_retry;                // 396
  

  // Timer         timer[MAX_TIMERS];         // 670
  // uint16_t      pulse_timer[8]; // 532 //#define MAX_PULSETIMERS 8  
  TimeRule tflag[2];                  // 2E2
  // TimeRule      tflag[2];                  // 2E2
  char          ntp_server[3][33];         // 4CE
  int16_t       toffset[2];                // 30E
  // Weight
  // uint16_t      weight_max;                // 7BE Total max weight in kilogram
  // unsigned long weight_reference;          // 7C0 Reference weight in gram
  // unsigned long weight_calibration;        // 7C4
  // unsigned long weight_item;               // 7B8 Weight of one item in gram * 10
  // Lighting
  SysBitfield_Lighting    flag_lighting;
  // ws82xx strips
  SysBitfield_Animations  flag_animations;
  LightSettings           light_settings;
  AnimationSettings       animation_settings;

  DeviceNameBuffer        device_name_buffer;
  
  // Pulse Counter
  // unsigned long pulse_counter[MAX_COUNTERS];  // 5C0
  uint16_t      pulse_counter_type;        // 5D0
  uint16_t      pulse_counter_debounce;    // 5D2
  // Sensors
  // SensorCfg1    SensorBits1;               // 717  On/Off settings used by Sensor Commands
  //uint32_t      sensors[3];                // 7A4
  // Mcp230xxCfg   mcp230xx_config[16];       // 6F6
  // uint8_t       mcp230xx_int_prio;         // 716
  // uint16_t      mcp230xx_int_timer;        // 718
  uint16_t      button_debounce;           // 542
  // char          switch_topic[33];          // 430
  uint16_t      switch_debounce;           // 66E
  uint8_t       switchmode[8];//MAX_SWITCHES];  // 3A4  (6.0.0b - moved from 0x4CA)
  SensorSettings sensors;
  // Drivers
  uint16_t      ledmask;                   // 7BC
  uint8_t       ledstate;                  // 2FB
  uint8_t       ledpwm_mask;               // E8F
  
  uint8_t       ledpwm_on;                 // F3F
  uint8_t       ledpwm_off;                // F40

  uint16_t      blinktime;                 // 39A
  uint16_t      blinkcount;                // 39C
  // uint32_t      drivers[3];                // 794
  uint32_t      monitors;                  // 7A0
  uint16_t      pwm_range;                 // 342
  uint16_t      pwm_frequency;             // 2E6
  uint16_t      pwm_value[MAX_PWMS];       // 2EC
  // uint8_t       rf_code[17][9];            // 5D4

  // Power
  unsigned long power; //power_t       power;                     // 2E8
  uint8_t       poweronstate;              // 398
  // Energy
  EnergyUsageNew   energy_usage;              // 77C 

  SysBitfield_Power  flag_power;                     // 5BC
  // Displays
  DisplaySettings   display;
  // Rules
  // uint8_t       rule_enabled;              // 49F
  // uint8_t       rule_once;                 // 4A0
  // char          mems[MAX_RULE_MEMS][10];   // 7CE
  // char          rules[MAX_RULE_SETS][MAX_RULE_SIZE]; // 800 uses 512 bytes in v5.12.0m, 3 x 512 bytes in v5.14.0b
  

  // SysBitfield4  flag4;                     // TEMP FIX
// TO SORT

#ifdef ESP32

  WebCamCfg     webcam_config;             // 44C

#endif


  // E00 - FFF (4095 ie eeprom size) free locations
  uint32_t      cfg_crc32;                 // FFC
} Settings;

// NEW flag that allows anything to run on reboot
uint8_t fSystemRestarted = true; // will be restart at the end of the first loop

struct RTCRBT {
  uint16_t      valid;                     // 280 (RTC memory offset 100 - sizeof(RTCRBT))
  uint8_t       fast_reboot_count;         // 282
  uint8_t       free_003[1];               // 283
} RtcReboot;

struct RTCMEM {
  uint16_t      valid;                     // 290 (RTC memory offset 100)
  uint8_t       oswatch_blocked_loop;      // 292
  uint8_t       ota_loader;                // 293
  unsigned long energy_kWhtoday;              // 294
  unsigned long energy_kWhtotal;              // 298
  unsigned long pulse_counter[MAX_COUNTERS];  // 29C
  power_t       power;                     // 2AC
  //#ifdef USE_ENERGY
  //  EnergyUsage   energy_usage;              // 2B0    PHASE OUT
  //#endif
  unsigned long nextwakeup;                // 2C8
  uint8_t       free_004[4];               // 2CC
  uint32_t      ultradeepsleep;            // 2D0
  uint16_t      deepsleep_slip;            // 2D4

  uint8_t       free_022[22];              // 2D6
                                           // 2EC - 2FF free locations
} RtcSettings;




uint8_t flag_boot_complete = false;


/**
 * Group of functions for storing/loading driver name list
 * */
enum SETTINGS_DEVICENAME_LIST_IDS{
  DEVICENAME_EMPTY_ID = -1,
  DEVICENAME_DISABLED_ID = -2,
};
int8_t      AddDeviceName(const char* name_ctr, int16_t class_id = 1000, int8_t driver_id = 255);
int8_t      RemoveDeviceName(const char* name_ctr, int16_t class_id = 1000, int8_t driver_id = 255);
const char* GetDeviceName(int16_t class_id, int8_t driver_id, char* buffer, uint16_t buffer_size);
int8_t      GetDeviceNameCount(int16_t class_id = 1000);



struct XDRVMAILBOX {
  uint16_t      valid;
  uint16_t      index;
  uint16_t      data_len;
  uint16_t      payload16;
  int16_t       payload;
  bool          grpflg;
  uint8_t       notused;
  char         *topic;
  char         *data;
} XdrvMailbox;



typedef union {
  uint8_t data;
  struct {
    uint8_t waiting : 1;
    uint8_t reserved : 7;
  };
} COMMAND_JSON_FLAG;

struct COMMAND_JSON{
  COMMAND_JSON_FLAG flag;
  

}command;




const uint8_t MAX_RULES_FLAG = 8;          // Number of bits used in RulesBitfield (tricky I know...)
typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint16_t data;                           // Allow bit manipulation
  struct {
    uint16_t system_boot : 1;
    uint16_t time_init : 1;
    uint16_t time_set : 1;
    uint16_t mqtt_connected : 1;
    uint16_t mqtt_disconnected : 1;
    uint16_t wifi_connected : 1;
    uint16_t wifi_disconnected : 1;
    uint16_t http_init : 1;
    uint16_t spare08 : 1;
    uint16_t spare09 : 1;
    uint16_t spare10 : 1;
    uint16_t spare11 : 1;
    uint16_t spare12 : 1;
    uint16_t spare13 : 1;
    uint16_t spare14 : 1;
    uint16_t spare15 : 1;
  };
} RulesBitfield;



int8_t CheckAndExecute_JSONCommands();
void parse_JSONCommand();


// See issue https://github.com/esp8266/Arduino/issues/2913
// #ifdef USE_ADC_VCC
//   ADC_MODE(ADC_VCC);                       // Set ADC input for Power Supply Voltage usage
// #endif

#define MAX_BUTTON_COMMANDS  5  // Max number of button commands supported
// const char kCommands[MAX_BUTTON_COMMANDS][14] PROGMEM = {
//   D_JSON_WIFICONFIG " 1",   // Press button three times
//   D_JSON_WIFICONFIG " 2",   // Press button four times
//   D_JSON_WIFICONFIG " 3",   // Press button five times
//   D_JSON_RESTART " 1",      // Press button six times
//   D_JSON_UPGRADE " 1" };    // Press button seven times

// const char kWifiConfig[MAX_WIFI_OPTION][WCFG_MAX_STRING_LENGTH] PROGMEM = {
//   D_WCFG_0_RESTART,
//   D_WCFG_1_SMARTCONFIG,
//   D_WCFG_2_WIFIMANAGER,
//   D_WCFG_3_WPSCONFIG,
//   D_WCFG_4_RETRY,
//   D_WCFG_5_WAIT,
//   D_WCFG_6_SERIAL,
//   D_WCFG_7_WIFIMANAGER_RESET_ONLY };

#ifdef ESP8266
SerialConfig serial_config = SERIAL_8N1;    // Serial interface configuration 8 data bits, No parity, 1 stop bit
#endif

//bit packed and used outside relays for lights too
power_t power = 0;                          // Current copy of Settings.power

// int baudrate = APP_BAUDRATE;                // Serial interface baud rate
// int serial_in_byte_counter = 0;             // Index in receive buffer
int ota_state_flag = 0;                     // OTA state flag
int ota_result = 0;                         // OTA result
int restart_flag = 0;                       // Sonoff restart flag
int wifi_state_flag = WIFI_RESTART;         // Wifi state flag
int tele_period = 1;                        // Tele period timer
int blinks = 201;                           // Number of LED blinks
uint32_t uptime = 0;                        // Counting every second until 4294967295 = 130 year
uint32_t loop_load_avg = 0;                 // Indicative loop load average
// uint32_t global_update = 0;                 // Timestamp of last global temperature and humidity update
// float global_temperature = 0;               // Provide a global temperature to be used by some sensors
// float global_humidity = 0;                  // Provide a global humidity to be used by some sensors
char *ota_url;                              // OTA url string pointer
// uint16_t mqtt_cmnd_publish = 0;             // ignore flag for publish command
// uint16_t blink_counter = 0;                 // Number of blink cycles
// uint16_t seriallog_timer = 0;               // Timer to disable Seriallog
// uint16_t syslog_timer = 0;                  // Timer to re-enable syslog_level
int16_t save_data_counter;                  // Counter and flag for config save to Flash
// RulesBitfield rules_flag;                   // Rule state flags (16 bits)
// uint8_t state_250mS = 0;                    // State 250msecond per second flag
uint8_t latching_relay_pulse = 0;           // Latching relay pulse timer
uint8_t backlog_index = 0;                  // Command backlog index
uint8_t backlog_pointer = 0;                // Command backlog pointer
uint8_t blinkspeed = 1;                     // LED blink rate


// These are used only at runtime, and not saved
struct RUNTIME_VALUES{
  uint8_t sleep;                              // Current copy of Settings.sleep
  uint32_t tSavedUpdateLoopStatistics;
}runtime_value;


// phased out, new tas method only records used pins, otherwise returns 99 for not set
// instead of remembering giant list
uint8_t pin[GPIO_MAX_ID];                      // Possible pin configurations

// new method which only holds the max pins to replace above array
// #ifdef ESP8266
uint8_t gpio_pin_new[MAX_GPIO_PIN] = { 0 };     // GPIO functions indexed by pin number
// #endif  // ESP8266 - ESP32

#ifdef ESP32
uint16_t gpio_pin[MAX_GPIO_PIN] = { 0 };    // GPIO functions indexed by pin number
#endif  // ESP32





uint8_t active_device = 1;                  // Active device in ExecuteCommandPower
uint8_t leds_present = 0;                   // Max number of LED supported
uint8_t led_inverted = 0;                   // LED inverted flag (1 = (0 = On, 1 = Off))
uint8_t led_power = 0;                      // LED power state
uint8_t ledlnk_inverted = 0;                // Link LED inverted flag (1 = (0 = On, 1 = Off))


uint8_t pwm_inverted = 0;                   // PWM inverted flag (1 = inverted)
uint8_t counter_no_pullup = 0;              // Counter input pullup flag (1 = No pullup)
uint8_t energy_flg = 0;                     // Energy monitor configured
// uint8_t Settings.light_settings.type = 0;                     // Light types
uint8_t serial_in_byte;                     // Received byte
// uint8_t ota_retry_counter = OTA_ATTEMPTS;   // OTA retry counter
uint8_t web_log_index = 1;                  // Index in Web log buffer (should never be 0)
uint8_t devices_present = 0;                // Max number of devices supported


uint8_t seriallog_level;                    // Current copy of Settings.seriallog_level
uint8_t syslog_level;                       // Current copy of Settings.syslog_level
uint8_t seriallog_level_during_boot;



uint8_t my_module_type;                     // Current copy of Settings.module or user template type
// uint8_t my_adc0;                            // Active copy of Module ADC0
uint8_t last_source = 0;                    // Last command source
// uint8_t shutters_present = 0;               // Number of actual define shutters
uint8_t mdns_delayed_start = 0;             // mDNS delayed start
// bool serial_local = false;                  // Handle serial locally;
// bool fallback_topic_flag = false;           // Use Topic or FallbackTopic
// bool backlog_mutex = false;                 // Command backlog pending
// bool interlock_mutex = false;               // Interlock power command pending
bool stop_flash_rotate = false;             // Allow flash configuration rotation

//TBR
bool blinkstate = false;                    // LED state

// bool latest_uptime_flag = true;             // Signal latest uptime
bool pwm_present = false;                   // Any PWM channel configured with SetOption15 0
// bool dht_flg = false;                       // DHT configured
bool i2c_flg = false;                       // I2C configured
bool spi_flg = false;                       // SPI configured
bool soft_spi_flg = false;                  // Software SPI configured
bool ntp_force_sync = false;                // Force NTP sync
myio my_module;                             // Active copy of Module GPIOs (17 x 8 bits)
gpio_flag my_module_flag;                   // Active copy of Module GPIO flags
StateBitfield global_state;                 // Global states (currently Wifi and Mqtt) (8 bits)
// char my_version[33];                        // Composed version string
// char my_image[33];                          // Code image and/or commit
char my_hostname[33];                       // Composed Wifi hostname
// char mqtt_client[33];                       // Composed MQTT Clientname
// char mqtt_topic[33];                        // Composed MQTT topic


void CommandSet_SystemRestartID(uint8_t value);


// #define DATA_BUFFER_TOPIC_MAX_LENGTH    100
// #define DATA_BUFFER_PAYLOAD_MAX_LENGTH  4000
// struct DATA_BUFFER{
//   struct TOPIC{
//     char ctr[DATA_BUFFER_TOPIC_MAX_LENGTH];
//     uint8_t len = 0;
//   }topic;
//   struct PAYLOAD{
//     char ctr[DATA_BUFFER_PAYLOAD_MAX_LENGTH];
//     uint16_t len = 0;
//     uint8_t encoded_type_id; //json,raw
//   }payload;
//   uint8_t fWaiting = false;
//   uint8_t method = false; // For detailed, ifchanged, all
//   uint8_t isserviced = 0; // Set to 0 on new mqtt, incremented with handled CORRECTLY payloads
// }data_buffer_old;

#ifndef WEB_LOG_SIZE
#define WEB_LOG_SIZE 400       // Max number of characters in weblog
#endif // WEB_LOG_SIZE
#ifndef LOG_BUFFER_SIZE
#define LOG_BUFFER_SIZE 500
#endif // LOG_BUFFER_SIZE
char log_data[LOG_BUFFER_SIZE];                       // Logging
char web_log[WEB_LOG_SIZE] = {'\0'};        // Web log buffer - REMEMBERS EVERYTHING for new load
#define RESPONSE_MESSAGE_BUFFER_SIZE 100
char response_msg[RESPONSE_MESSAGE_BUFFER_SIZE];


const char* GetTelePeriodJsonLevelCtr(char* buffer);
const char* GetTelePeriodJsonLevelCtr(uint8_t id, char* buffer);

struct FIRMWARE_VERSION{
  struct type{
    uint32_t number;
    uint8_t part_branch = 0;
    uint8_t part_major = 0;
    uint8_t part_minor = 0;
    uint8_t part_system = 0;
    uint8_t part_module = 0;
    char name_ctr[15];
  };
  struct type current;
  struct type latest;
  struct type lowest;
  uint8_t fNewVersionAvailable = false;
  uint8_t fCurrentVersionNotSupported = false;
}firmware_version;

};

#endif  // _SETTINGS_H_
//#endif


// #ifndef _MIOINTERFACE
// #define _MIOINTERFACE

// #include "2_CoreSystem/mBaseConfig.h"

// #warning "mIOInterface EOL"

// #include <stdint.h>
// #include <Arduino.h>

//   #define LED_RED_PIN BUILTIN_LED // turn the NodeMCU LED off (HIGH is the voltage level)
//   #define LED_RED BUILTIN_LED
//   #define LED_STATUS_PIN BUILTIN_LED
//   #define LED_WIFISTATUS_PIN BUILTIN_LED

//   #define LED_BLUE_PIN BUILTIN_LED

//   #define LED_RED_PIN LED_BLUE_PIN
//   #define LED_STATUS_PIN LED_BLUE_PIN
//   #define LED_MQTT_PIN LED_RED_PIN
//   #define LED_RED_PIN LED_BLUE_PIN
//   #define LED_WIFI_PIN LED_BLUE_PIN


// // #ifdef DEVICE_HEATING

// //   // Ethernet Signal Cable TO hotpress
// //   // br/w - 9V
// //   // w/br - GND

// //   // g/w  - RELAY Immersion (ends at boiler)
// //   // w/g  - DB18 Sensors (ends at boiler)

// //   // bl/w - BME_D
// //   // w/bl - BME_A

// //   // o/w  - 3V3
// //   // w/o  - GND

// //   //AVOID D3,D4,D8 (0,2,15)
// //   // Relays
// //   #define CLIMATE_US_PIN D0
// //   #define RELAY_DS_PIN D1
// //   #define RELAY_US_PIN D2
// //   // SKIPPED - D3
// //   // SKIPPED - D4
// //   #define RELAY_WB_PIN D5
// //   #define RELAY_IH_PIN D6
// //   #define CLIMATE_DS_PIN D7
// //   // SKIPPED - D8

// //   #define DB_DS_SENSOR_PIN 9//SD2
// //   #define DB_US_SENSOR_PIN D3//10//SD3

// // #endif



// #ifdef ESP32
//   #define LED_RED 22 // turn the NodeMCU LED off (HIGH is the voltage level)
//   #define CLIMATE_DHT1_PIN 13
//   #define PIR_DETECT_PIN 22
//     //#define RGB_DATA_PIN 19

//   #define LED_WIFI_PIN 22

//   // #if defined(USE_WS28XX_HARDWARE_WS2801)
//   //   #define RGB_DATA_PIN 19
//   //   #define RGBCLOCKPIN 18
//   // #else
//     #define RGB_DATA_PIN 19//3 // phase over to 19
//   // #endif

//   #if defined(DEVICE_RGBOUTSIDETREE)
//     #define RELAY_0_PIN LED_BUILTIN    // OUTPUT ONLY GPIO0
//   #endif
  

//   #ifdef DEVICE_BEDROOMBLINDS

//     #define BLINDS_ADC_PIN 33

//     #define BLINDS_MOTOR_IA_PIN 18 // Motor B PWM Speed
//     #define BLINDS_MOTOR_IB_PIN 19 // Motor B Direction

//     // // wired connections
//     // #define HG7881_B_IA D5 // D10 --> Motor B Input A --> MOTOR B +
//     // #define HG7881_B_IB D6 // D11 --> Motor B Input B --> MOTOR B -
    
//     // // functional connections
//     // #define MOTOR_B_PWM HG7881_B_IA // Motor B PWM Speed
//     // #define MOTOR_B_DIR HG7881_B_IB // Motor B Direction

//     // #define BLINDS_MOTOR_PWM MOTOR_B_PWM
//     // #define BLINDS_MOTOR_DIR MOTOR_B_DIR

//   #endif


//   #define LED_BLUE_PIN LED_BUILTIN
//   #define LED_BLUE LED_BUILTIN // turn the ESP-12 LED off (HIGH is the voltage level)
//   #define DB_SENSOR_PIN 15
//   #define REED_SWITCH_PIN 22
//   #define FAN_RELAY_PIN 22

//   #define SAW_RADIO_RX_PIN 19
//   #define SAW_RADIO_TX_PIN 4
//   #define IR_PIN 23 //needs changed

  
//   #ifdef DEVICE_RGBHEATINGPANEL
//     //#define CLIMATE_DHT1_PIN D0
//     //sdc d1 -bme
//     //sda d2 -bme
//     #define PIR_DETECT_PIN 23 // GPIO2 - BLUELED PIN
//     //#define LED_BLUE_PIN D8   // OUTPUT ONLY GPIO15
//     #define ALL_PINS_DEFINED
//   #endif



//   #define LED_RED_PIN LED_BLUE_PIN
//   #define LED_STATUS_PIN LED_BLUE_PIN
//   #define LED_MQTT_PIN LED_BLUE_PIN
//   #define LED_RED_PIN LED_BLUE_PIN

// #endif

// // SHARED PIN MACROS

// #define   LED_BLUE_INIT() pinMode(LED_BLUE_PIN,OUTPUT)
// #define   LED_BLUE_ON() digitalWrite(LED_BLUE_PIN,LOW);
// #define   LED_BLUE_OFF() digitalWrite(LED_BLUE_PIN,HIGH);
// #define   LED_BLUE_TOGGLE() digitalWrite(LED_BLUE_PIN,!digitalRead(LED_BLUE_PIN));
// #define   LED_BLUE_SET(x) digitalWrite(LED_BLUE_PIN,x);

// #define   LED_RED_INIT() pinMode(LED_RED_PIN,OUTPUT)
// #define   LED_RED_ON() digitalWrite(LED_RED,LOW);
// #define   LED_RED_OFF() digitalWrite(LED_RED,HIGH);
// #define   LED_RED_TOGGLE() digitalWrite(LED_RED,!digitalRead(LED_RED));
// #define   LED_RED_SET(x) digitalWrite(LED_RED_PIN,x);

// #define   LED_MQTT_INIT() pinMode(LED_MQTT_PIN,OUTPUT)
// #define   LED_MQTT_ON() digitalWrite(LED_MQTT_PIN,LOW);
// #define   LED_MQTT_OFF() digitalWrite(LED_MQTT_PIN,HIGH);
// #define   LED_MQTT_TOGGLE() digitalWrite(LED_MQTT_PIN,!digitalRead(LED_MQTT_PIN));
// #define   LED_MQTT_SET(x) digitalWrite(LED_MQTT_PIN,x);

// #define   LED_WIFI_INIT() pinMode(LED_WIFI_PIN,OUTPUT)
// #define   LED_WIFI_ON() digitalWrite(LED_WIFI_PIN,LOW);
// #define   LED_WIFI_OFF() digitalWrite(LED_WIFI_PIN,HIGH);
// #define   LED_WIFI_TOGGLE() digitalWrite(LED_WIFI_PIN,!digitalRead(LED_WIFI_PIN));
// #define   LED_WIFI_SET(x) digitalWrite(LED_WIFI_PIN,x);

// #define   LED_STATUS_INIT() pinMode(LED_STATUS_PIN,OUTPUT)
// #define   LED_STATUS_ON() digitalWrite(LED_STATUS_PIN,LOW);
// #define   LED_STATUS_OFF() digitalWrite(LED_STATUS_PIN,HIGH);
// #define   LED_STATUS_TOGGLE() digitalWrite(LED_STATUS_PIN,!digitalRead(LED_STATUS_PIN));
// #define   LED_STATUS_SET(x) digitalWrite(LED_STATUS_PIN,x);


// #endif
