#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#define D_UNIQUE_MODULE_CORE_SETTINGS_ID ((2*1000)+01)

#include "2_CoreSystem/mBaseConfig.h"

#define DATA_BUFFER_TOPIC_MAX_LENGTH    100

#ifdef USE_MODULE_NETWORK_WEBSERVER23
  #ifndef DATA_BUFFER_PAYLOAD_MAX_LENGTH
    #define DATA_BUFFER_PAYLOAD_MAX_LENGTH 3000
  #endif
#else
  #ifndef DATA_BUFFER_PAYLOAD_MAX_LENGTH
    #define DATA_BUFFER_PAYLOAD_MAX_LENGTH 3000
  #endif
#endif //USE_MODULE_NETWORK_WEBSERVER23


enum DATA_BUFFER_FLAG_SOURCE_IDS
{
  DATA_BUFFER_FLAG_SOURCE_MQTT=0,
  DATA_BUFFER_FLAG_SOURCE_WEBUI
};


typedef union 
{
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
  uint16_t isserviced = 0; // Set to 0 on new mqtt
  DATA_BUFFER_FLAGS flags;
};
extern struct DATA_BUFFER data_buffer;
#define D_DATA_BUFFER_CLEAR() memset(&data_buffer,0,sizeof(data_buffer))


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


#include "2_CoreSystem/06_Support/mSupport.h"
#include "2_CoreSystem/05_Logging/mLogging.h"
#include "1_TaskerManager/mTaskerManager.h"
#include "2_CoreSystem/03_HardwareTemplates/mHardwareTemplates.h"
#include "2_CoreSystem/mFirmwareDefaults.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"
#include "2_CoreSystem/11_Languages/mLanguageProgmem.h"
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

#include "2_CoreSystem/02_Time/mTime.h"

#define D_DEFAULT_DEVICE_BUFFER_LENGTH 50 



enum DATABUILDER_JSON_LEVEL{ //in order of importance
  JSON_LEVEL_NONE=0,
  // Only show what has changed over mqtt
  JSON_LEVEL_IFCHANGED,
  // Used to show in serial the most basic stuff anytime it is called (ie above IFCHANGED) but not as detailed as short would be 
  // Basic and short may be the same, so remove base
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
  SPLIT_TASK_ERROR_ID,
  SPLIT_TASK_DONE_ID
};



const uint32_t settings_text_size = 699;   // Settings->text_pool[size] = Settings->display_model (2D2) - Settings->text_pool (017)
const uint8_t MAX_TUYA_FUNCTIONS = 16;
const uint8_t PARAM8_SIZE = 18;            // Number of param bytes (SetOption)

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

#ifdef ENABLE_DEVFEATURE__SETTINGS_NEW_STRUCT_2023
// Why 28? Because in addition to relays there may be lights and uint32_t bitmap can hold up to 32 devices
#ifdef ESP8266
const uint8_t MAX_RELAYS = 8;               // Max number of relays selectable on GPIO
const uint8_t MAX_INTERLOCKS = 4;           // Max number of interlock groups (up to MAX_INTERLOCKS_SET)
const uint8_t MAX_SWITCHES = 8;             // Max number of switches selectable on GPIO
const uint8_t MAX_KEYS = 8;                 // Max number of keys or buttons selectable on GPIO
#endif  // ESP8266
#ifdef ESP32
const uint8_t MAX_RELAYS = 28;              // Max number of relays selectable on GPIO
const uint8_t MAX_INTERLOCKS = 14;          // Max number of interlock groups (up to MAX_INTERLOCKS_SET)
const uint8_t MAX_SWITCHES = 28;            // Max number of switches selectable on GPIO
const uint8_t MAX_KEYS = 28;                // Max number of keys or buttons selectable on GPIO
#endif  // ESP32
const uint8_t MAX_RELAYS_SET = 32;          // Max number of relays
const uint8_t MAX_KEYS_SET = 32;            // Max number of keys

// Changes to the following MAX_ defines will impact settings layout
const uint8_t MAX_INTERLOCKS_SET = 14;      // Max number of interlock groups (MAX_RELAYS_SET / 2)
const uint8_t MAX_SWITCHES_SET = 28;        // Max number of switches
const uint8_t MAX_LEDS = 4;                 // Max number of leds
const uint8_t MAX_PWMS_LEGACY = 5;          // Max number of PWM channels in first settings block - Legacy limit for ESP8266, but extended for ESP32 (see below)
#ifdef ESP32                                // Max number of PWM channels (total including extended) - ESP32 only
  #if CONFIG_IDF_TARGET_ESP32
  const uint8_t MAX_PWMS = 16;              // ESP32: 16 ledc PWM channels in total - TODO for now
  #elif CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
  const uint8_t MAX_PWMS = 8;               // ESP32S2/S3: 8 ledc PWM channels in total
  #elif CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C6
  const uint8_t MAX_PWMS = 6;               // ESP32C2/C3/C6: 6 ledc PWM channels in total
  #else
  const uint8_t MAX_PWMS = 5;               // Unknown - revert to 5 PWM max
  #endif
#else
const uint8_t MAX_PWMS = 5;                 // (not used on ESP8266)
#endif
const uint8_t MAX_COUNTERS = 4;             // Max number of counter sensors
const uint8_t MAX_TIMERS = 16;              // Max number of Timers
const uint8_t MAX_PULSETIMERS = 32;         // Max number of supported pulse timers
const uint8_t MAX_DOMOTICZ_IDX = 4;         // Max number of Domoticz device, key and switch indices
const uint8_t MAX_DOMOTICZ_SNS_IDX = 12;    // Max number of Domoticz sensors indices
const uint8_t MAX_KNX_GA = 10;              // Max number of KNX Group Addresses to read that can be set
const uint8_t MAX_KNX_CB = 10;              // Max number of KNX Group Addresses to write that can be set
const uint8_t MAX_XNRG_DRIVERS = 32;        // Max number of allowed energy drivers
const uint8_t MAX_XDRV_DRIVERS = 96;        // Max number of allowed driver drivers
const uint8_t MAX_XSNS_DRIVERS = 128;       // Max number of allowed sensor drivers
const uint8_t MAX_I2C_DRIVERS = 96;         // Max number of allowed i2c drivers
const uint8_t MAX_SHUTTERS = 4;             // Max number of shutters
const uint8_t MAX_SHUTTER_KEYS = 4;         // Max number of shutter keys or buttons
const uint8_t MAX_PCF8574 = 4;              // Max number of PCF8574 devices
const uint8_t MAX_DS3502 = 4;               // Max number of DS3502 digitsal potentiometer devices
const uint8_t MAX_IRSEND = 16;              // Max number of IRSEND GPIOs
const uint8_t MAX_RULE_SETS = 3;            // Max number of rule sets of size 512 characters
const uint16_t MAX_RULE_SIZE = 512;         // Max number of characters in rules
const uint16_t VL53LXX_MAX_SENSORS = 8;     // Max number of VL53L0X sensors
#endif // ENABLE_DEVFEATURE__SETTINGS_NEW_STRUCT_2023

#ifndef ENABLE_DEVFEATURE__SETTINGS_NEW_STRUCT_2023
// Changes to the following MAX_ defines will impact settings layout
// #define MAX_RELAYS 4
// const uint8_t MAX_RELAYS = 8;               // Max number of relays
const uint8_t MAX_INTERLOCKS = 4;           // Max number of interlock groups (MAX_RELAYS / 2)

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
#endif // ENABLE_DEVFEATURE__SETTINGS_NEW_STRUCT_2023

// const char MQTT_TOKEN_PREFIX[] PROGMEM = "%prefix%";  // To be substituted by mqtt_prefix[x]
// const char MQTT_TOKEN_TOPIC[] PROGMEM = "%topic%";    // To be substituted by mqtt_topic, mqtt_grptopic, mqtt_buttontopic, mqtt_switchtopic
// //const char WIFI_HOSTNAME[] = "%s-%04d";     // Expands to <MQTT_TOPIC>-<last 4 decimal chars of MAC address>
const char WIFI_HOSTNAME[]  = "%s"; //!PROGMEM    // Expands to <MQTT_TOPIC>-<last 4 decimal chars of MAC address>
//^ cant be porgmem
// const uint8_t CONFIG_FILE_SIGN = 0xA5;      // Configuration file signature
// const uint8_t CONFIG_FILE_XOR = 0x5A;       // Configuration file xor (0 = No Xor)

const uint32_t HLW_PREF_PULSE = 12530;      // was 4975us = 201Hz = 1000W
const uint32_t HLW_UREF_PULSE = 1950;       // was 1666us = 600Hz = 220V
const uint32_t HLW_IREF_PULSE = 3500;       // was 1666us = 600Hz = 4.545A


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

const uint8_t DEFAULT_POWER_DELTA = 80;     // Power change percentage
const uint16_t MAX_POWER_HOLD = 10;         // Time in SECONDS to allow max agreed power
const uint16_t MAX_POWER_WINDOW = 30;       // Time in SECONDS to disable allow max agreed power
const uint16_t SAFE_POWER_HOLD = 10;        // Time in SECONDS to allow max unit safe power
const uint16_t SAFE_POWER_WINDOW = 30;      // Time in MINUTES to disable allow max unit safe power
const uint8_t MAX_POWER_RETRY = 5;          // Retry count allowing agreed power limit overflow

// const uint8_t STATES = 20;                  // Number of states per second using 50 mSec interval
const uint8_t IMMINENT_RESET_FACTOR = 10;   // Factor to extent button hold time for imminent Reset to default 40 seconds using KEY_HOLD_TIME of 40
const uint8_t BOOT_LOOP_TIME = 10;         // Number of seconds to stop detecting boot loops
// const uint16_t SYSLOG_TIMER = 600;          // Seconds to restore syslog_level
// const uint16_t SERIALLOG_TIMER = 600;       // Seconds to disable SerialLog
// const uint8_t OTA_ATTEMPTS = 5;             // Number of times to try fetching the new firmware

const uint16_t FLOATSZ = 16;                // Max number of characters in float result from dtostrfd (max 32)
const uint16_t TBUFFER_SIZE_FLOAT = 16; // TBUFFER are (T)emporary Buffers that are used for conversions

#define STANDARD_SMS_CHAR_LENGTH 160

// // const uint16_t INPUT_BUFFER_SIZE = 520;     // Max number of characters in (serial and http) command buffer
// const uint16_t CMDSZ = 2;                  // Max number of characters in command
// const uint16_t TOPSZ = 2;                 // Max number of characters in topic string
// // const uint16_t MIN_MESSZ = 893;             // Min number of characters in MQTT message

const uint8_t SENSOR_MAX_MISS = 5;          // Max number of missed sensor reads before deciding it's offline
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
  SWITCHMODE_TOGGLE_ID, 
  SWITCHMODE_FOLLOW_ID, 
  SWITCHMODE_FOLLOW_INV_ID, 
  SWITCHMODE_PUSHBUTTON_ID, 
  SWITCHMODE_PUSHBUTTON_INV_ID, 
  SWITCHMODE_PUSHBUTTONHOLD_ID, 
  SWITCHMODE_PUSHBUTTONHOLD_INV_ID, 
  SWITCHMODE_PUSHBUTTON_TOGGLE_ID, 
  SWITCHMODE_MAX_SWITCH_OPTION_ID
};

#define D_SWITCHMODE_TOGGLE_CTR               "Toggle"
#define D_SWITCHMODE_FOLLOW_CTR               "Follow"
#define D_SWITCHMODE_FOLLOW_INV_CTR           "Follow Inv"
#define D_SWITCHMODE_PUSHBUTTON_CTR           "PushButton"
#define D_SWITCHMODE_PUSHBUTTON_INV_CTR       "PushButton Inv"
#define D_SWITCHMODE_PUSHBUTTONHOLD_CTR       "PushButton Hold"
#define D_SWITCHMODE_PUSHBUTTONHOLD_INV_CTR   "PushButton Hold Inv"
#define D_SWITCHMODE_PUSHBUTTON_TOGGLE_CTR    "PushButton Toggle"

DEFINE_PGM_CTR(PM_SWITCHMODE_TOGGLE_CTR) D_SWITCHMODE_TOGGLE_CTR;
DEFINE_PGM_CTR(PM_SWITCHMODE_FOLLOW_CTR) D_SWITCHMODE_FOLLOW_CTR;
DEFINE_PGM_CTR(PM_SWITCHMODE_FOLLOW_INV_CTR) D_SWITCHMODE_FOLLOW_INV_CTR;
DEFINE_PGM_CTR(PM_SWITCHMODE_PUSHBUTTON_CTR) D_SWITCHMODE_PUSHBUTTON_CTR;
DEFINE_PGM_CTR(PM_SWITCHMODE_PUSHBUTTON_INV_CTR) D_SWITCHMODE_PUSHBUTTON_INV_CTR;
DEFINE_PGM_CTR(PM_SWITCHMODE_PUSHBUTTONHOLD_CTR) D_SWITCHMODE_PUSHBUTTONHOLD_CTR;
DEFINE_PGM_CTR(PM_SWITCHMODE_PUSHBUTTONHOLD_INV_CTR) D_SWITCHMODE_PUSHBUTTONHOLD_INV_CTR;
DEFINE_PGM_CTR(PM_SWITCHMODE_PUSHBUTTON_TOGGLE_CTR) D_SWITCHMODE_PUSHBUTTON_TOGGLE_CTR;


enum LedStateOptions {LED_OFF, LED_POWER, LED_MQTTSUB, LED_POWER_MQTTSUB, LED_MQTTPUB, LED_POWER_MQTTPUB, LED_MQTT, LED_POWER_MQTT, MAX_LED_OPTION};

// enum EmulationOptions {EMUL_NONE, EMUL_WEMO, EMUL_HUE, EMUL_MAX};

enum TopicOptions { CMND, STAT, TELE, nu1, RESULT_OR_CMND, RESULT_OR_STAT, RESULT_OR_TELE };


/**
 * @brief To be phased into "STATE_NUMBER_IDS"
 * 
 */

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

// enum SettingsParmaIndex {P_HOLD_TIME, P_MAX_POWER_RETRY, 
// P_TUYA_DIMMER_ID, P_MDNS_DELAYED_START, P_BOOT_LOOP_OFFSET, 
// P_RGB_REMAP, 

// P_HOLD_IGNORE,


// P_MAX_PARAM8};  // Max is PARAM8_SIZE (18) - SetOption32 until SetOption49

/**
 * @brief Initial used with buttons, but should be expanded into rule type events
 * 
 */
enum INPUT_TYPE_IDS{ //style fron nextion
  INPUT_TYPE_SINGLE_PRESS_ID,
  INPUT_TYPE_MULTIPLE_PRESS_ID,
  INPUT_TYPE_SINGLE_HOLD_ID,
  INPUT_TYPE_LENGTH_ID
};


enum SO32_49Index { P_HOLD_TIME,              // SetOption32 - (Button/Switch) Key hold time detection in decaseconds (default 40)
                    P_MAX_POWER_RETRY,        // SetOption33 - (Energy) Maximum number of retries before deciding power limit overflow (default 5)
                    P_BACKLOG_DELAY,          // SetOption34 - (Backlog) Minimal delay in milliseconds between executing backlog commands (default 200)
                    P_MDNS_DELAYED_START,     // SetOption35 - (mDNS) Number of seconds before mDNS is started (default 0) - Obsolete
                    P_BOOT_LOOP_OFFSET,       // SetOption36 - (Restart) Number of restarts to start detecting boot loop (default 1)
                    P_RGB_REMAP,              // SetOption37 - (Light) RGB and White channel separation (default 0)
                    P_IR_UNKNOW_THRESHOLD,    // SetOption38 - (IR) Set the smallest sized "UNKNOWN" message packets we actually care about (default 6, max 255)
                    P_CSE7766_INVALID_POWER,  // SetOption39 - (CSE7766) Number of invalid power measurements before declaring it invalid allowing low load measurments (default 128)
                    P_HOLD_IGNORE,            // SetOption40 - (Button/Shutter) Ignore button change in seconds (default 0)
                    P_ARP_GRATUITOUS,         // SetOption41 - (Wifi) Interval in seconds between gratuitous ARP requests (default 60)
                    P_OVER_TEMP,              // SetOption42 - (Energy) Turn all power off at or above this temperature (default 90C)
                    P_ROTARY_MAX_STEP,        // SetOption43 - (Rotary) Rotary step boundary (default 10)
                    P_IR_TOLERANCE,           // SetOption44 - (IR) Base tolerance percentage for matching incoming IR messages (default 25, max 100)
                    P_SO45_FREE,              // SetOption45
                    P_SO46_FREE,              // SetOption46
                    P_SO47_FREE,              // SetOption47
                    P_SO48_FREE,              // SetOption48
                    P_SO49_FREE               // SetOption49
                  };  // Max is PARAM8_SIZE (18) - SetOption32 until SetOption49



//scenes animations?
enum LightSchemes {LS_POWER, LS_WAKEUP, LS_CYCLEUP, LS_CYCLEDN, LS_RANDOM, LS_MAX};

// enum COMMAND_FUNCTIONS_IDS{}


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



// const char kWebColors[] PROGMEM =
//   COLOR_TEXT "|" COLOR_BACKGROUND "|" 
//   COLOR_FORM "|" COLOR_INPUT_TEXT "|" 
//   COLOR_INPUT "|" COLOR_CONSOLE_TEXT "|" COLOR_CONSOLE "|"
//   COLOR_TEXT_WARNING "|" COLOR_TEXT_SUCCESS "|"
//   COLOR_BUTTON_TEXT "|" COLOR_BUTTON "|" COLOR_BUTTON_HOVER "|" COLOR_BUTTON2 "|" COLOR_BUTTON2_HOVER "|" 
//   COLOR_BUTTON_RESET "|" COLOR_BUTTON_RESET_HOVER "|" COLOR_BUTTON_SAVE "|" 
//   COLOR_BUTTON_SAVE_HOVER "|"
//   COLOR_TIMER_TAB_TEXT "|" COLOR_TIMER_TAB_BACKGROUND "|" D_COL_TEXT_MODULE_TITLE_CTR;


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

#include "2_CoreSystem/13_DeviceNameList/mDeviceNameList.h"


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
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static const char* PM_MODULE_CORE_SETTINGS_CTR;
    static const char* PM_MODULE_CORE_SETTINGS_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CORE_SETTINGS_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_CORE_SETTINGS_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_SETTINGS_ID; }
    
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
    int16_t GetIndexOfNthCharPosition(const char* tosearch, char tofind, uint8_t occurance_count);


    void Function_Template_Load();











#ifdef ENABLE_DEVFEATURE__SETTINGS_NEW_STRUCT_2023

// Bitfield to be used for any SetOption0 .. SetOption31 persistent single bit
typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint32_t data;                           // Allow bit manipulation using SetOption
  struct {                                 // SetOption0 .. SetOption31
    uint32_t save_state : 1;               // bit 0              - SetOption0   - (Settings) Save power state (1) and use after restart
    uint32_t button_restrict : 1;          // bit 1              - SetOption1   - (Button) Control button single press (1) or multipress (0)
    uint32_t mqtt_add_global_info : 1;     // bit 2              - SetOption2   - (MQTT) Add global temperature/humidity/pressure info to JSON sensor message
    uint32_t mqtt_enabled : 1;             // bit 3              - SetOption3   - (MQTT) Enable (1)
    uint32_t mqtt_response : 1;            // bit 4              - SetOption4   - (MQTT) Switch between RESULT (0) or COMMAND (1)
    uint32_t mqtt_power_retain : 1;        // bit 5              - CMND_POWERRETAIN
    uint32_t mqtt_button_retain : 1;       // bit 6              - CMND_BUTTONRETAIN
    uint32_t mqtt_switch_retain : 1;       // bit 7              - CMND_SWITCHRETAIN
    uint32_t temperature_conversion : 1;   // bit 8              - SetOption8   - (Temperature) Switch between Celsius (0) or Fahrenheit (1)
    uint32_t mqtt_sensor_retain : 1;       // bit 9              - CMND_SENSORRETAIN
    uint32_t mqtt_offline : 1;             // bit 10             - SetOption10  - (MQTT) Control LWT message format, Offline (1) or none (0)
    uint32_t button_swap : 1;              // bit 11 (v5.1.6)    - SetOption11  - (Button) Swap (1) button single and double press functionality
    uint32_t stop_flash_rotate : 1;        // bit 12 (v5.2.0)    - SetOption12  - (Settings) Switch between dynamic (0) or fixed (1) slot flash save location
    uint32_t button_single : 1;            // bit 13 (v5.4.0)    - SetOption13  - (Button) Support only single press (1) to speed up button press recognition
    uint32_t interlock : 1;                // bit 14 (v5.6.0)    - CMND_INTERLOCK - Enable (1) /disable (0) interlock
    uint32_t pwm_control : 1;              // bit 15 (v5.8.1)    - SetOption15  - (Light) Switch between commands PWM (0) or COLOR/DIMMER/CT/CHANNEL (1)
    uint32_t ws_clock_reverse : 1;         // bit 16 (v5.8.1)    - SetOption16  - (WS2812) Switch between clockwise (0) or counter-clockwise (1)
    uint32_t decimal_text : 1;             // bit 17 (v5.8.1)    - SetOption17  - (Light) Switch between decimal (1) or hexadecimal (0) output
    uint32_t light_signal : 1;             // bit 18 (v5.10.0c)  - SetOption18  - (Light) Pair light signal (1) with CO2 sensor
    uint32_t hass_discovery : 1;           // bit 19 (v5.11.1a)  - SetOption19  - (HAss) Control automatic discovery (1) (See SetOption59)
    uint32_t not_power_linked : 1;         // bit 20 (v5.11.1f)  - SetOption20  - (Light) Control power in relation to Dimmer/Color/Ct changes (1)
    uint32_t no_power_on_check : 1;        // bit 21 (v5.11.1i)  - SetOption21  - (Energy) Show voltage even if powered off (1)
    uint32_t mqtt_serial : 1;              // bit 22 (v5.12.0f)  - CMND_SERIALSEND and CMND_SERIALLOG
    uint32_t mqtt_serial_raw : 1;          // bit 23 (v6.1.1c)   - CMND_SERIALSEND3
    uint32_t pressure_conversion : 1;      // bit 24 (v6.3.0.2)  - SetOption24  - (Pressure) switch between hPa (0)or mmHg (1) unit
    uint32_t knx_enabled : 1;              // bit 25 (v5.12.0l)  - CMND_KNX_ENABLED
    uint32_t device_index_enable : 1;      // bit 26 (v5.13.1a)  - SetOption26  - (MQTT, HAss) Switch between POWER (0) or POWER1 (1)
    uint32_t knx_enable_enhancement : 1;   // bit 27 (v5.14.0a)  - CMND_KNX_ENHANCED
    uint32_t rf_receive_decimal : 1;       // bit 28 (v6.0.0a)   - SetOption28  - (RF) Receive data format hexadecimal (0) or decimal (1)
    uint32_t ir_receive_decimal : 1;       // bit 29 (v6.0.0a)   - SetOption29  - (IR) Receive data format hexadecimal (0) or decimal (1)
    uint32_t hass_light : 1;               // bit 30 (v6.0.0b)   - SetOption30  - (HAss) enforce autodiscovery as light (1)
    uint32_t global_state : 1;             // bit 31 (v6.1.0)    - SetOption31  - (Wifi, MQTT) Control link led blinking (1)
  };
} SOBitfield;

// Bitfield to be used for any SetOption50 .. SetOption81 persistent single bit
typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint32_t data;                           // Allow bit manipulation using SetOption
  struct {                                 // SetOption50 .. SetOption81
    uint32_t timers_enable : 1;            // bit 0 (v6.1.1b)    - CMND_TIMERS
    uint32_t user_esp8285_enable : 1;      // bit 1 (v6.1.1.14)  - SetOption51  - (GPIO) Enable ESP8285 user GPIO's (1)
    uint32_t time_append_timezone : 1;     // bit 2 (v6.2.1.2)   - SetOption52  - (Time) Append timezone to JSON time (1)
    uint32_t gui_hostname_ip : 1;          // bit 3 (v6.2.1.20)  - SetOption53  - (GUI) Show hostname and IP address in GUI main menu (1)
    uint32_t tuya_apply_o20 : 1;           // bit 4 (v6.3.0.4)   - SetOption54  - (Tuya) Apply SetOption20 settings to Tuya device (1)
    uint32_t mdns_enabled : 1;             // bit 5 (v6.4.1.4)   - SetOption55  - (mDNS) Service on (1) or off (0)
    uint32_t use_wifi_scan : 1;            // bit 6 (v6.3.0.10)  - SetOption56  - (Wifi) Scan network at restart for configured AP's (1) or used stored AP (0)
    uint32_t use_wifi_rescan : 1;          // bit 7 (v6.3.0.10)  - SetOption57  - (Wifi) Scan network every 44 minutes for configured AP's (1)
    uint32_t receive_raw : 1;              // bit 8 (v6.3.0.11)  - SetOption58  - (IR) Add IR Raw data to JSON message (1)
    uint32_t hass_tele_on_power : 1;       // bit 9 (v6.3.0.13)  - SetOption59  - (MQTT) Send tele/%topic%/STATE in addition to stat/%topic%/RESULT (1)
    uint32_t sleep_normal : 1;             // bit 10 (v6.3.0.15) - SetOption60  - (Sleep) Enable normal sleep (1) instead of dynamic sleep (0)
    uint32_t button_switch_force_local : 1;// bit 11 (v6.3.0.16) - SetOption61  - (Button, Switch) Force local operation (1) when button/switch topic is set
    uint32_t no_hold_retain : 1;           // bit 12 (v6.4.1.19) - SetOption62  - (MQTT) Don't use retain flag on HOLD messages (1)
    uint32_t no_power_feedback : 1;        // bit 13 (v6.5.0.9)  - SetOption63  - (Power) Don't scan relay power state at restart (1)
    uint32_t use_underscore : 1;           // bit 14 (v6.5.0.12) - SetOption64  - (JSON) Enable "_" (1) instead of "-" (0) as sensor index separator
    uint32_t fast_power_cycle_disable : 1; // bit 15 (v6.6.0.20) - SetOption65  - (QPC) Disable (1) fast power cycle detection for device reset
    uint32_t tuya_serial_mqtt_publish : 1; // bit 16 (v6.6.0.21) - SetOption66  - (Tuya) Enable (1) TuyaMcuReceived messages over Mqtt
    uint32_t buzzer_enable : 1;            // bit 17 (v6.6.0.1)  - SetOption67  - (Buzzer) Enable (1) buzzer when available
    uint32_t pwm_multi_channels : 1;       // bit 18 (v6.6.0.3)  - SetOption68  - (Light) Enable multi-channels PWM (1) instead of Color PWM (0)
    uint32_t ex_tuya_dimmer_min_limit : 1; // bit 19 (v6.6.0.5)  - SetOption69  - (not used) Limits Tuya dimmers to minimum of 10% (25) when enabled
    uint32_t energy_weekend : 1;           // bit 20 (v6.6.0.8)  - CMND_TARIFF
    uint32_t dds2382_model : 1;            // bit 21 (v6.6.0.14) - SetOption71  - (DDS2382) Select different Modbus registers (1) for Active Energy (#6531)
    uint32_t hardware_energy_total : 1;    // bit 22 (v6.6.0.15) - SetOption72  - (Energy) Enable (1) hardware energy total counter as reference (#6561)
    uint32_t mqtt_buttons : 1;             // bit 23 (v8.2.0.3)  - SetOption73  - (Button) Detach buttons from relays (1) and enable MQTT action state for multipress
    uint32_t ds18x20_internal_pullup : 1;  // bit 24 (v7.0.0.1)  - SetOption74  - (DS18x20) Enable internal pullup (1) for single DS18x20 sensor
    uint32_t grouptopic_mode : 1;          // bit 25 (v7.0.0.1)  - SetOption75  - (MQTT) GroupTopic replaces %topic% (0) or fixed topic cmnd/grouptopic (1)
    uint32_t bootcount_update : 1;         // bit 26 (v7.0.0.4)  - SetOption76  - (Deepsleep) Enable incrementing bootcount (1) when deepsleep is enabled
    uint32_t slider_dimmer_stay_on : 1;    // bit 27 (v7.0.0.6)  - SetOption77  - (Light) Do not power off (1) if slider moved to far left
    uint32_t ex_compatibility_check : 1;   // bit 28 (v7.1.2.6)  - SetOption78  - (not used) Disable OTA compatibility check
    uint32_t counter_reset_on_tele : 1;    // bit 29 (v8.1.0.1)  - SetOption79  - (Counter) Enable resetting of counters (1) after telemetry was sent
    uint32_t shutter_mode : 1;             // bit 30 (v6.6.0.14) - SetOption80  - (Shutter) Enable shutter support (1)
    uint32_t pcf8574_ports_inverted : 1;   // bit 31 (v6.6.0.14) - SetOption81  - (PCF8574) Invert all ports on PCF8574 devices (1)
  };
} SOBitfield3;

// Bitfield to be used for any SetOption82 .. SetOption113 persistent single bit
typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint32_t data;                           // Allow bit manipulation using SetOption
  struct {                                 // SetOption82 .. SetOption113
    uint32_t alexa_ct_range : 1;           // bit 0 (v8.1.0.2)   - SetOption82  - (Alexa) Reduced CT range for Alexa (1)
    uint32_t zigbee_use_names : 1;         // bit 1 (v8.1.0.4)   - SetOption83  - (Zigbee) Use FriendlyNames (1) instead of ShortAddresses (0) when possible
    uint32_t awsiot_shadow : 1;            // bit 2 (v8.1.0.5)   - SetOption84  - (AWS IoT) publish MQTT state to a device shadow (1)
    uint32_t device_groups_enabled : 1;    // bit 3 (v8.1.0.9)   - SetOption85  - (DevGroups) Enable Device Groups (1)
    uint32_t led_timeout : 1;              // bit 4 (v8.1.0.9)   - SetOption86  - (PWM Dimmer) Turn brightness LED's off (1) 5 seconds after last change
    uint32_t powered_off_led : 1;          // bit 5 (v8.1.0.9)   - SetOption87  - (PWM Dimmer) Turn red LED on (1) when powered off
    uint32_t multiple_device_groups : 1;   // bit 6 (v8.1.0.9)   - SetOption88  - (DevGroups) Enable relays in separate device groups/PWM Dimmer Buttons control remote devices (1)
    uint32_t zigbee_distinct_topics : 1;   // bit 7 (v8.1.0.10)  - SetOption89  - (MQTT, Zigbee) Distinct MQTT topics per device for Zigbee (1) (#7835)
    uint32_t only_json_message : 1;        // bit 8 (v8.2.0.3)   - SetOption90  - (MQTT) Disable non-json MQTT response (1)
    uint32_t fade_at_startup : 1;          // bit 9 (v8.2.0.3)   - SetOption91  - (Light) Enable light fading at start/power on (1)
    uint32_t pwm_ct_mode : 1;              // bit 10 (v8.2.0.4)  - SetOption92  - (Light) Set PWM Mode from regular PWM (0) to ColorTemp (1) control (Xiaomi Philips ...)
    uint32_t compress_rules_cpu : 1;       // bit 11 (v8.2.0.6)  - SetOption93  - (Compress) Keep uncompressed rules in memory to avoid CPU load of uncompressing at each tick (1)
    uint32_t max6675 : 1;                  // bit 12 (v8.3.1.2)  - SetOption94  - (MAXxxx) Implement simpler MAX6675 protocol (1) instead of MAX31855 (0)
    uint32_t network_wifi : 1;             // bit 13 (v8.3.1.3)  - CMND_WIFI
    uint32_t network_ethernet : 1;         // bit 14 (v8.3.1.3)  - CMND_ETHERNET
    uint32_t tuyamcu_baudrate : 1;         // bit 15 (v8.3.1.6)  - SetOption97  - (Tuya) Set Baud rate for TuyaMCU serial communication to 9600 (0) or 115200 (1)
    uint32_t rotary_uses_rules : 1;        // bit 16 (v8.3.1.6)  - SetOption98  - (Rotary) Use rules (1) instead of light control (0)
    uint32_t zerocross_dimmer : 1;         // bit 17 (v8.3.1.4)  - SetOption99  - (PWM Dimmer) Enable zerocross dimmer (1)
    uint32_t remove_zbreceived : 1;        // bit 18 (v8.3.1.7)  - SetOption100 - (Zigbee) Remove ZbReceived form JSON message (1)
    uint32_t zb_index_ep : 1;              // bit 19 (v8.3.1.7)  - SetOption101 - (Zigbee) Add the source endpoint as suffix to attributes, ex `Power3` (1) instead of `Power` (0) if sent from endpoint 3
    uint32_t obsolete1 : 1;                // bit 20 (v9.3.1.3)  - SetOption102 - teleinfo_baudrate Obsolete Teleinfo config has now a dedicated bit field
    uint32_t mqtt_tls : 1;                 // bit 21 (v8.4.0.1)  - SetOption103 - (MQTT TLS) Enable TLS mode (1) (requires TLS version)
    uint32_t mqtt_no_retain : 1;           // bit 22 (v8.4.0.1)  - SetOption104 - (MQTT) No Retain (1) - disable all MQTT retained messages, some brokers don't support it: AWS IoT, Losant
    uint32_t white_blend_mode : 1;         // bit 23 (v8.4.0.1)  - SetOption105 - (Light) White Blend Mode (1) - used to be `RGBWWTable` last value `0`, now deprecated in favor of this option
    uint32_t virtual_ct : 1;               // bit 24 (v8.4.0.1)  - SetOption106 - (Light) Virtual CT (1) - Creates a virtual White ColorTemp for RGBW lights
    uint32_t virtual_ct_cw : 1;            // bit 25 (v8.4.0.1)  - SetOption107 - (Light) Virtual CT Channel (1) - signals whether the hardware white is cold CW (true) or warm WW (false)
    uint32_t obsolete2 : 1;                // bit 26 (v9.3.1.3)  - SetOption108 - teleinfo_rawdata Obsolete Teleinfo config has now a dedicated bit field
    uint32_t alexa_gen_1 : 1;              // bit 27 (v8.4.0.3)  - SetOption109 - (Alexa) Gen1 mode (1) - if you only have Echo Dot 2nd gen devices
    uint32_t zb_disable_autobind : 1;      // bit 28 (v8.5.0.1)  - SetOption110 - (Zigbee) Disable auto-config (1) when pairing new devices
    uint32_t buzzer_freq_mode : 1;         // bit 29 (v8.5.0.1)  - SetOption111 - (Buzzer) Use frequency output (1) for buzzer pin instead of on/off signal (0)
    uint32_t zb_topic_fname : 1;           // bit 30 (v8.5.0.1)  - SetOption112 - (Zigbee) Use friendly name in zigbee topic (1) (use with SetOption89)
    uint32_t rotary_poweron_dimlow : 1;    // bit 31 (v9.0.0.2)  - SetOption113 - (Rotary) Set dimmer low on rotary dial after power off (1)
  };
} SOBitfield4;

// Bitfield to be used for any SetOption114 .. SetOption145 persistent single bit
typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint32_t data;                           // Allow bit manipulation using SetOption
  struct {                                 // SetOption114 .. SetOption145
    uint32_t mqtt_switches : 1;            // bit 0 (v9.0.0.3)   - SetOption114 - (Switch) Detach Switches from relays and enable MQTT action state for all the SwitchModes (1)
    uint32_t mi32_enable : 1;              // bit 1 (v9.1.0.1)   - SetOption115 - (ESP32 BLE) Enable ESP32 MI32 BLE (1)
    uint32_t zb_disable_autoquery : 1;     // bit 2 (v9.1.0.1)   - SetOption116 - (Zigbee) Disable auto-query of zigbee lights and devices (1)
    uint32_t fade_fixed_duration : 1;      // bit 3 (v9.1.0.2)   - SetOption117 - (Light) run fading at fixed duration instead of fixed slew rate
    uint32_t zb_received_as_subtopic : 1;  // bit 4 (v9.2.0.3)   - SetOption118 - (Zigbee) Move ZbReceived from JSON message and into the subtopic replacing "SENSOR" default
    uint32_t zb_omit_json_addr : 1;        // bit 5 (v9.2.0.3)   - SetOption119 - (Zigbee) Remove the device addr from json payload, can be used with zb_topic_fname where the addr is already known from the topic
    uint32_t zb_topic_endpoint : 1;        // bit 6 (v9.2.0.4)   - SetOption120 - (Zigbee) Append endpoint number to topic if device dependent (use with SetOption89)
    uint32_t mqtt_state_retain : 1;        // bit 7 (v9.3.0.1)   - CMND_STATERETAIN
    uint32_t mqtt_info_retain  : 1;        // bit 8 (v9.3.0.1)   - CMND_INFORETAIN
    uint32_t wiegand_hex_output : 1;       // bit 9 (v9.3.1.1)   - SetOption123 - (Wiegand) switch tag number output to hex format (1)
    uint32_t wiegand_keypad_to_tag : 1;    // bit 10 (v9.3.1.1)  - SetOption124 - (Wiegand) send key pad stroke as single char (0) or one tag (ending char #) (1)
    uint32_t zigbee_hide_bridge_topic : 1; // bit 11 (v9.3.1.1)  - SetOption125 - (Zigbee) Hide bridge topic from zigbee topic (use with SetOption89) (1)
    uint32_t ds18x20_mean : 1;             // bit 12 (v9.3.1.2)  - SetOption126 - (DS18x20) Enable arithmetic mean over teleperiod for JSON temperature (1)
    uint32_t wifi_no_sleep : 1;            // bit 13 (v9.5.0.2)  - SetOption127 - (Wifi) Keep wifi in no-sleep mode, prevents some occasional unresponsiveness
    uint32_t disable_referer_chk : 1;      // bit 14 (v9.5.0.5)  - SetOption128 - (Web) Allow access without referer check
    uint32_t energy_phase : 1;             // bit 15 (v9.5.0.9)  - SetOption129 - (Energy) Show phase information
    uint32_t show_heap_with_timestamp : 1; // bit 16 (v9.5.0.9)  - SetOption130 - (Debug) Show heap with logging timestamp
    uint32_t tuya_allow_dimmer_0 : 1;      // bit 17 (v10.0.0.3) - SetOption131 - (Tuya) Allow save dimmer = 0 receved by MCU
    uint32_t tls_use_fingerprint : 1;      // bit 18 (v10.0.0.4) - SetOption132 - (TLS) Use fingerprint validation instead of CA based
    uint32_t shift595_invert_outputs : 1;  // bit 19 (v10.0.0.4) - SetOption133 - (Shift595) Invert outputs of 74x595 shift registers
    uint32_t pwm_force_same_phase : 1;     // bit 20 (v10.1.0.6) - SetOption134 - (PWM) force PWM lights to start at same phase, default is to spread phases to minimze overlap (also needed for H-bridge)
    uint32_t display_no_splash : 1;        // bit 21 (v11.0.0.2) - SetOption135 - (Display & LVGL) force disabling default splash screen
    uint32_t tuyasns_no_immediate : 1;     // bit 22 (v11.0.0.4) - SetOption136 - (TuyaSNS) When ON disable publish single SNS value on Tuya Receive (keep Teleperiod)
    uint32_t tuya_exclude_from_mqtt : 1;   // bit 23 (v11.0.0.5) - SetOption137 - (Tuya) When Set, avoid the (MQTT-) publish of defined Tuya CMDs (see xdrv_16_tuyamcu.ino) if SetOption66 is active
    uint32_t gui_table_align : 1;          // bit 24 (v11.0.0.7) - SetOption138 - (GUI) Align (energy) table values left (0) or right (1)
    uint32_t mm_vs_inch : 1;               // bit 25 (v11.1.0.1) - SetOption139 - (Pressure) Switch between mmHg (0) or inHg (1) when SO24 1
    uint32_t mqtt_persistent : 1;          // bit 26 (v11.1.0.1) - SetOption140 - (MQTT) MQTT clean session (0 = default) or persistent session (1)
    uint32_t gui_module_name : 1;          // bit 27 (v11.1.0.3) - SetOption141 - (GUI) Disable display of GUI module name (1)
    uint32_t wait_for_wifi_result : 1;     // bit 28 (v11.1.0.4) - SetOption142 - (Wifi) Wait 1 second for wifi connection solving some FRITZ!Box modem issues (1)
    uint32_t zigbee_no_batt_autoprobe : 1; // bit 29 (v12.0.2.4) - SetOption143 - (Zigbee) Disable Battery auto-probe and using auto-binding
    uint32_t zigbee_include_time : 1;      // bit 30 (v12.0.2.4) - SetOption144 - (Zigbee) Include time in `ZbReceived` messages like other sensors
    uint32_t mqtt_status_retain : 1;       // bit 31 (v12.1.0.1) - CMND_STATUSRETAIN
  };
} SOBitfield5;

typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint32_t data;                           // Allow bit manipulation using SetOption
  struct {                                 // SetOption146 .. SetOption177
    uint32_t use_esp32_temperature : 1;    // bit 0  (v12.1.1.1) - SetOption146 - (ESP32) Show ESP32 internal temperature sensor
    uint32_t mqtt_disable_sserialrec : 1;  // bit 1  (v12.1.1.2) - SetOption147 - (MQTT) Disable publish SSerialReceived MQTT messages, you must use event trigger rules instead.
    uint32_t artnet_autorun : 1;           // bit 2  (v12.2.0.4) - SetOption148 - (Light) start DMX ArtNet at boot, listen to UDP port as soon as network is up
    uint32_t dns_ipv6_priority : 1;        // bit 3  (v12.2.0.6) - SetOption149 - (Wifi) prefer IPv6 DNS resolution to IPv4 address when available. Requires `#define USE_IPV6`
    uint32_t no_voltage_common : 1;        // bit 4  (v12.3.1.5) - SetOption150 - (Energy) Force no voltage/frequency common
    uint32_t matter_enabled : 1;           // bit 5  (v12.3.1.5) - SetOption151 - (Matter) Enable Matter protocol over Wifi
    uint32_t bistable_single_pin : 1;      // bit 6  (v12.5.0.1) - SetOption152 - (Power) Switch between two (0) or one (1) pin bistable relay control
    uint32_t berry_no_autoexec : 1;        // bit 7  (v12.5.0.3) - SetOption153 - (Berry) Disable autoexec.be on restart (1)
    uint32_t berry_light_scheme : 1;       // bit 8  (v12.5.0.3) - SetOption154 - (Berry) Handle berry led using RMT0 as additional WS2812 scheme
    uint32_t zcfallingedge : 1;            // bit 9  (v13.0.0.1) - SetOption155 - (ZCDimmer) Enable rare falling Edge dimmer instead of leading edge
    uint32_t sen5x_passive_mode : 1;       // bit 10 (v13.1.0.1) - SetOption156 - (Sen5x) Run in passive mode when there is another I2C master (e.g. Ikea Vindstyrka), i.e. do not set up Sen5x sensor, higher polling interval
    uint32_t spare11 : 1;                  // bit 11
    uint32_t spare12 : 1;                  // bit 12
    uint32_t spare13 : 1;                  // bit 13
    uint32_t spare14 : 1;                  // bit 14
    uint32_t spare15 : 1;                  // bit 15
    uint32_t spare16 : 1;                  // bit 16
    uint32_t spare17 : 1;                  // bit 17
    uint32_t spare18 : 1;                  // bit 18
    uint32_t spare19 : 1;                  // bit 19
    uint32_t spare20 : 1;                  // bit 20
    uint32_t spare21 : 1;                  // bit 21
    uint32_t spare22 : 1;                  // bit 22
    uint32_t spare23 : 1;                  // bit 23
    uint32_t spare24 : 1;                  // bit 24
    uint32_t spare25 : 1;                  // bit 25
    uint32_t spare26 : 1;                  // bit 26
    uint32_t spare27 : 1;                  // bit 27
    uint32_t spare28 : 1;                  // bit 28
    uint32_t spare29 : 1;                  // bit 29
    uint32_t spare30 : 1;                  // bit 30
    uint32_t spare31 : 1;                  // bit 31
  };
} SOBitfield6;

// Bitfield to be used for persistent multi bit
typedef union {
  uint32_t data;                           // Allow bit manipulation
  struct {
    uint32_t spare00 : 1;
    uint32_t speed_conversion : 3;         // (v8.1.0.10) - Tx2x sensor
    uint32_t time_format : 2;              // (v6.6.0.9)  - CMND_TIME
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
} SysMBitfield1;

// Bitfield to be used for persistent multi bit
typedef union {
  uint32_t data;                           // Allow bit manipulation
  struct {
    uint32_t spare00 : 1;                  // bit 0
    uint32_t spare01 : 1;                  // bit 1
    uint32_t spare02 : 1;                  // bit 2
    uint32_t spare03 : 1;                  // bit 3
    uint32_t spare04 : 1;                  // bit 4
    uint32_t spare05 : 1;                  // bit 5
    uint32_t spare06 : 1;                  // bit 6
    uint32_t spare07 : 1;                  // bit 7
    uint32_t spare08 : 1;                  // bit 8
    uint32_t spare09 : 1;                  // bit 9
    uint32_t spare10 : 1;                  // bit 10
    uint32_t spare11 : 1;                  // bit 11
    uint32_t spare12 : 1;                  // bit 12
    uint32_t spare13 : 1;                  // bit 13
    uint32_t spare14 : 1;                  // bit 14
    uint32_t spare15 : 1;                  // bit 15
    uint32_t spare16 : 1;                  // bit 16
    uint32_t spare17 : 1;                  // bit 17
    uint32_t spare18 : 1;                  // bit 18
    uint32_t spare19 : 1;                  // bit 19
    uint32_t spare20 : 1;                  // bit 20
    uint32_t spare21 : 1;                  // bit 21
    uint32_t spare22 : 1;                  // bit 22
    uint32_t spare23 : 1;                  // bit 23
    uint32_t spare24 : 1;                  // bit 24
    uint32_t spare25 : 1;                  // bit 25
    uint32_t tariff_forced : 2;            // bit 26..27 (v12.4.0.2) - Energy forced tariff : 0=tariff change on time, 1|2=tariff forced
    uint32_t sunrise_dawn_angle : 2;       // bits 28/29 (v12.1.1.4) -
    uint32_t temperature_set_res : 2;      // bits 30/31 (v9.3.1.4) - (Tuya)
  };
} SysMBitfield2;

// Bitfield to be used for non-SetOption persistent single bit
typedef union {
  uint32_t data;                           // Allow bit manipulation
  struct {
    uint32_t telegram_send_enable : 1;     // bit 0  (v9.4.0.3) - CMND_TMSTATE 0/1 - Enable Telegram send
    uint32_t telegram_recv_enable : 1;     // bit 1  (v9.4.0.3) - CMND_TMSTATE 2/3 - Enable Telegram receive
    uint32_t telegram_echo_enable : 1;     // bit 2  (v9.4.0.3) - CMND_TMSTATE 4/5 - Enable Telegram echo
    uint32_t range_extender : 1;           // bit 3  (v9.5.0.5) - CMND_RGXSTATE - Enable range extender
    uint32_t range_extender_napt : 1;      // bit 4  (v9.5.0.5) - CMND_RGXNAPT - Enable range extender NAPT
    uint32_t sonoff_l1_music_sync : 1;     // bit 5  (v9.5.0.5) - CMND_L1MUSICSYNC - Enable sync to music
    uint32_t influxdb_default : 1;         // bit 6  (v9.5.0.5) - Set influxdb initial defaults if 0
    uint32_t influxdb_state : 1;           // bit 7  (v9.5.0.5) - CMND_IFX - Enable influxdb support
    uint32_t sspm_display : 1;             // bit 8  (v10.0.0.4) - CMND_SSPMDISPLAY - Enable gui display of powered on relays only
    uint32_t local_ntp_server : 1;         // bit 9  (v11.0.0.4) - CMND_RTCNTPSERVER - Enable local NTP server
    uint32_t influxdb_sensor : 1;          // bit 10 (v11.0.0.5) - CMND_IFXSENSOR - Enable sensor support in addition to teleperiod support
    uint32_t serbridge_console : 1;        // bit 11 (v11.1.0.4) - CMND_SSERIALSEND9 - Enable logging tee to serialbridge
    uint32_t spare12 : 1;                  // bit 12
    uint32_t spare13 : 1;                  // bit 13
    uint32_t spare14 : 1;                  // bit 14
    uint32_t spare15 : 1;                  // bit 15
    uint32_t spare16 : 1;                  // bit 16
    uint32_t spare17 : 1;                  // bit 17
    uint32_t spare18 : 1;                  // bit 18
    uint32_t spare19 : 1;                  // bit 19
    uint32_t spare20 : 1;                  // bit 20
    uint32_t spare21 : 1;                  // bit 21
    uint32_t spare22 : 1;                  // bit 22
    uint32_t spare23 : 1;                  // bit 23
    uint32_t spare24 : 1;                  // bit 24
    uint32_t spare25 : 1;                  // bit 25
    uint32_t spare26 : 1;                  // bit 26
    uint32_t spare27 : 1;                  // bit 27
    uint32_t spare28 : 1;                  // bit 28
    uint32_t spare29 : 1;                  // bit 29
    uint32_t spare30 : 1;                  // bit 30
    uint32_t spare31 : 1;                  // bit 31
  };
} SBitfield1;

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

typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint32_t data;
  struct {
    uint32_t stream : 1;
    uint32_t mirror : 1;
    uint32_t flip : 1;
    uint32_t rtsp : 1;
    uint32_t awb : 1;
    uint32_t awb_gain : 1;
    uint32_t aec : 1;
    uint32_t aec2 : 1;
    uint32_t agc : 1;
    uint32_t raw_gma : 1;
    uint32_t lenc : 1;
    uint32_t colorbar : 1;
    uint32_t wpc : 1;
    uint32_t dcw : 1;
    uint32_t bpc : 1;
    uint32_t spare15 : 1;
    uint32_t spare16 : 1;
    uint32_t feature : 2;
    uint32_t contrast : 3;
    uint32_t brightness : 3;
    uint32_t saturation : 3;
    uint32_t resolution : 4;
  };
} WebCamCfg;

typedef union {
  uint32_t data;
  struct {
    uint32_t wb_mode : 3;
    uint32_t ae_level : 3;
    uint32_t aec_value : 11;
    uint32_t gainceiling : 3;
    uint32_t agc_gain: 5;
    uint32_t special_effect : 3;
    uint32_t auth : 1;
    uint32_t spare29 : 1;
    uint32_t spare30 : 1;
    uint32_t upgraded : 1;
  };
} WebCamCfg2;

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
    uint16_t keep_output : 1;               // For output modes, preserve the value currently in the MCP230xx
    uint16_t spare14 : 1;
    uint16_t spare15 : 1;
  };
} Mcp230xxCfg;

typedef union {
  uint8_t data;
  struct {
    uint8_t spare0 : 1;
    uint8_t flowratemeter_unit : 1;        // Sensor96 9,x - unit l/min (0) or m³/h (1)
    uint8_t bh1750_2_resolution : 2;
    uint8_t bh1750_1_resolution : 2;       // Sensor10 1,2,3
    uint8_t hx711_json_weight_change : 1;  // Sensor34 8,x - Enable JSON message on weight change
    uint8_t mhz19b_abc_disable : 1;        // Disable ABC (Automatic Baseline Correction for MHZ19(B) (0 = Enabled (default), 1 = Disabled with Sensor15 command)
  };
} SensorCfg1;

typedef union {
  uint8_t data;
  struct {
  uint8_t pwm_count : 3;              // Number of PWMs to use for light
  uint8_t spare3 : 1;
  uint8_t spare4 : 1;
  uint8_t spare5 : 1;
  uint8_t spare6 : 1;
  uint8_t spare7 : 1;
  };
} PWMDimmerCfg;

typedef union {
  uint8_t data;
  struct {
  uint8_t nf_autotune : 1;            // Autotune the NF Noise Level
  uint8_t dist_autotune : 1;          // Autotune Disturber on/off
  uint8_t nf_autotune_both : 1;       // Autotune over both Areas: INDOORS/OUDOORS
  uint8_t mqtt_only_Light_Event : 1;  // mqtt only if lightning Irq
  uint8_t suppress_irq_no_Event : 1;  // suppress mqtt "IRQ with no Event". (Chip Bug)
  uint8_t spare5 : 1;
  uint8_t spare6 : 1;
  uint8_t spare7 : 1;
  };
} As3935IntCfg;

typedef union {
  uint16_t data;
  struct {
  uint16_t nf_autotune_time : 4;            // NF Noise Autotune Time
  uint16_t dist_autotune_time : 4;          // Disturber Autotune Time
  uint16_t nf_autotune_min : 4;             // Min Stages
  uint16_t spare3 : 4;
  };
} As3935Param;

typedef union {
  uint32_t data;
  struct {
  uint32_t raw_skip : 8;               // raw frame to skip when sending raw data (set to 2 means send 1 frame, then skip 2, ...)
  uint32_t raw_report_changed : 1;     // Report only changed values in raw frames (only valid if raw_skip=0)
  uint32_t raw_send : 1;               // Enable sending also real time raw data over MQTT
  uint32_t raw_limit : 1;              // Limit raw data to minimal relevant fields (the ones moving quickly)
  uint32_t mode_standard : 1;          // Set Linky Standard Mode (9600 bps stream) else legacy (1200 bps)
  uint32_t show_stats : 1;             // Display frames stats informations on WEB interface
  uint32_t spare1_1 : 1;               // Keep some spares for future uses
  uint32_t spare1_2 : 1;               // Keep some spares for future uses
  uint32_t spare1_3 : 1;               // Keep some spares for future uses
  uint32_t spare8_1 : 8;               // Keep some spares for future uses
  uint32_t spare8_2 : 8;               // Keep some spares for future uses
  };
} TeleinfoCfg;

typedef struct {
  uint32_t usage1_kWhtotal;
  uint32_t usage2_kWhtotal;
  uint32_t return1_kWhtotal;
  uint32_t return2_kWhtotal;
  uint32_t last_return_kWhtotal;
  uint32_t last_usage_kWhtotal;
} EnergyUsage;

typedef struct {
  uint8_t fnid = 0;
  uint8_t dpid = 0;
} TuyaFnidDpidMap;

typedef union {
  uint8_t data;
  struct {
  uint8_t type : 3;
  uint8_t invert : 1;
  uint8_t spare4 : 1;
  uint8_t spare5 : 1;
  uint8_t spare6 : 1;
  uint8_t spare7 : 1;
  };
} DisplayOptions;


#define settings_text_size 699   // Settings->text_pool[size] = Settings->display_model (2D2) - Settings->text_pool (017)
#define MAX_TUYA_FUNCTIONS 16
// const uint8_t PARAM8_SIZE = 18;            // Number of param bytes (SetOption)



typedef struct {
  uint16_t      cfg_holder;                // 000  v6 header
  uint16_t      cfg_size;                  // 002
  uint32_t      save_flag;                 // 004
  uint32_t      version;                   // 008
  uint16_t      bootcount;                 // 00C
  uint16_t      cfg_crc;                   // 00E
  SOBitfield    flag;                      // 010
  int16_t       save_data;                 // 014
  int8_t        timezone;                  // 016

  // Start of char array storing all parameter strings ********

  char          text_pool[138];            // 017  Size is settings_text_size
  char          ex_sta_ssid[2][33];        // 0A1
  char          ex_sta_pwd[2][65];         // 0E3
  uint8_t       text_pool_165[132];        // 165
  char          ex_mqtt_host[33];          // 1E9
  uint8_t       text_pool_20A[35];         // 20A
  char          ex_mqtt_user[33];          // 22D
  char          ex_mqtt_pwd[33];           // 24E
  char          ex_mqtt_topic[33];         // 26F
  uint8_t       text_pool_290[66];         // 290

  // End of single char array of 698 chars max ****************
  uint8_t       display_model;             // 2D2
  uint8_t       display_mode;              // 2D3
  uint8_t       display_refresh;           // 2D4
  uint8_t       display_rows;              // 2D5
  uint8_t       display_cols[2];           // 2D6
  uint8_t       display_address[8];        // 2D8
  int8_t        display_dimmer_protected;  // 2E0 - if positive range 0..15, if negative range 0..100 (neg) - don't use directly
  uint8_t       display_size;              // 2E1
  TimeRule      tflag[2];                  // 2E2
  uint16_t      pwm_frequency;             // 2E6
  power_t       power;                     // 2E8
  uint16_t      pwm_value[MAX_PWMS_LEGACY];// 2EC
  int16_t       altitude;                  // 2F6
  uint16_t      tele_period;               // 2F8
  uint8_t       display_rotate;            // 2FA
  uint8_t       ledstate;                  // 2FB
  uint8_t       param[PARAM8_SIZE];        // 2FC  SetOption32 .. SetOption49
  int16_t       toffset[2];                // 30E
  uint8_t       display_font;              // 312
  DisplayOptions  display_options;         // 313
  int32_t       energy_kWhtoday_ph[3];     // 314
  int32_t       energy_kWhyesterday_ph[3]; // 320
  int32_t       energy_kWhtotal_ph[3];     // 32C
  int32_t       weight_user_tare;          // 338
  uint8_t       web_time_start;            // 33C
  uint8_t       web_time_end;              // 33D
  uint8_t       sserial_config;            // 33E
  uint8_t       tuyamcu_topic;             // 33F  Manage tuyaSend topic. ex_energy_power_delta on 6.6.0.20, replaced on 8.5.0.1
  uint16_t      domoticz_update_timer;     // 340
  uint16_t      pwm_range;                 // 342
  uint32_t      domoticz_relay_idx[MAX_DOMOTICZ_IDX];  // 344
  uint32_t      domoticz_key_idx[MAX_DOMOTICZ_IDX];    // 354
  uint32_t      energy_power_calibration;    // 364
  uint32_t      energy_voltage_calibration;  // 368
  uint32_t      energy_current_calibration;  // 36C
  uint32_t      energy_power_calibration2;   // 370 - ex_energy_kWhtoday
  uint32_t      energy_voltage_calibration2; // 374 - ex_energy_kWhyesterday
  uint32_t      energy_current_calibration2; // 378 - ex_energy_kWhdoy, ex_energy_min_power
  uint16_t      energy_max_power;          // 37C
  uint16_t      energy_min_voltage;        // 37E
  uint16_t      energy_max_voltage;        // 380
  uint16_t      energy_min_current;        // 382
  uint16_t      energy_max_current;        // 384
  uint16_t      energy_max_power_limit;              // 386  MaxPowerLimit
  uint16_t      energy_max_power_limit_hold;         // 388  MaxPowerLimitHold
  uint16_t      energy_max_power_limit_window;       // 38A  MaxPowerLimitWindow
  uint16_t      energy_max_power_safe_limit;         // 38C  MaxSafePowerLimit
  uint16_t      energy_max_power_safe_limit_hold;    // 38E  MaxSafePowerLimitHold
  uint16_t      energy_max_power_safe_limit_window;  // 390  MaxSafePowerLimitWindow
  uint16_t      energy_max_energy;         // 392  MaxEnergy
  uint16_t      energy_max_energy_start;   // 394  MaxEnergyStart
  uint16_t      mqtt_retry;                // 396
  uint8_t       poweronstate;              // 398
  uint8_t       last_module;               // 399
  uint16_t      blinktime;                 // 39A
  uint16_t      blinkcount;                // 39C
  uint16_t      light_rotation;            // 39E
  SOBitfield3   flag3;                     // 3A0
  uint16_t      energy_kWhdoy;             // 3A4
  uint16_t      energy_min_power;          // 3A6
  uint32_t      pn532_password;            // 3A8 - ex_switchmode4-7, Free since 9.2.0.6

#ifdef CONFIG_IDF_TARGET_ESP32S3
  // ------------------------------------
  // Remapping of the section for ESP32S3
  // ------------------------------------
  myio          my_gp;                     // 3AC  (+x62) 2x49 bytes (ESP32-S3)
  uint8_t       eth_type;                  // 40E
  uint8_t       eth_clk_mode;              // 40F
  mytmplt       user_template;             // 410  (9x4E) 2x39 bytes (ESP32-S3)
  uint8_t       eth_address;               // 45E
  uint8_t       module;                    // 45F
  WebCamCfg     webcam_config;             // 460
  uint8_t       ws_width[3];               // 464
  char          serial_delimiter;          // 467
  uint8_t       seriallog_level;           // 468
  uint8_t       sleep;                     // 469
  uint16_t      domoticz_switch_idx[MAX_DOMOTICZ_IDX];      // 46A (+8)
  uint16_t      domoticz_sensor_idx[MAX_DOMOTICZ_SNS_IDX];  // 472 (+x18)
  uint8_t       ws_color[4][3];            // 48A (+xC)
                                           // 496

  // ----------------------------------------
  // End of remapping, next is all other CPUs
  // ----------------------------------------
#else
  myio          my_gp;                     // 3AC  2x18 bytes (ESP8266) / 2x40 bytes (ESP32) / 2x21 bytes (ESP32-C2) / 2x22 bytes (ESP32-C3) / 2x31 bytes (ESP32-C6) / 2x47 bytes (ESP32-S2)
#ifdef ESP8266
  uint16_t      gpio16_converted;          // 3D0
  uint8_t       free_esp8266_3D2[42];      // 3D2
#endif  // ESP8266
#ifdef ESP32
  #if CONFIG_IDF_TARGET_ESP32C2
  uint8_t       free_esp32c2_3D6[38];      // 3D6  - Due to smaller myio
  #elif CONFIG_IDF_TARGET_ESP32C3
  uint8_t       free_esp32c3_3D8[36];      // 3D8  - Due to smaller myio
  #elif CONFIG_IDF_TARGET_ESP32C6
  uint8_t       free_esp32c6_3EA[18];      // 3EA  - Due to smaller myio
  #endif  // CONFIG_IDF_TARGET_ESP32C2/3/6
#endif  // ESP32
  mytmplt       user_template;             // 3FC  2x15 bytes (ESP8266) / 2x37 bytes (ESP32) / 2x22 bytes (ESP32-C2) / 2x23 bytes (ESP32-C3) / 2x32 bytes (ESP32-C6) / 2x37 bytes (ESP32-S2)
#ifdef ESP8266
  uint8_t       free_esp8266_41A[55];      // 41A
#endif  // ESP8266
#ifdef ESP32
  #if CONFIG_IDF_TARGET_ESP32C2
  uint8_t       free_esp32c2_428[30];      // 428  - Due to smaller mytmplt
  #elif CONFIG_IDF_TARGET_ESP32C3
  uint8_t       free_esp32c3_42A[28];      // 42A  - Due to smaller mytmplt
  #elif CONFIG_IDF_TARGET_ESP32C6
  uint8_t       free_esp32c3_43C[10];      // 43C  - Due to smaller mytmplt
  #endif  // CONFIG_IDF_TARGET_ESP32C2/3/6

  uint8_t       eth_type;                  // 446
  uint8_t       eth_clk_mode;              // 447

  uint8_t       free_esp32_448[4];         // 448
  #ifdef CONFIG_IDF_TARGET_ESP32S2
  uint8_t       free_esp32s2_456[2];       // 456 - fix 32-bit offset for WebCamCfg
  #endif

  WebCamCfg     webcam_config;             // 44C
  uint8_t       eth_address;               // 450
#endif  // ESP32

  char          serial_delimiter;          // 451
  uint8_t       seriallog_level;           // 452
  uint8_t       sleep;                     // 453
  uint16_t      domoticz_switch_idx[MAX_DOMOTICZ_IDX];      // 454
  uint16_t      domoticz_sensor_idx[MAX_DOMOTICZ_SNS_IDX];  // 45C
  uint8_t       module;                    // 474
  uint8_t       ws_color[4][3];            // 475
  uint8_t       ws_width[3];               // 481

#ifdef ESP8266
  myio8         ex_my_gp8;                 // 484 17 bytes (ESP8266) - Free since 9.0.0.1
#endif  // ESP8266
#ifdef ESP32
#ifdef CONFIG_IDF_TARGET_ESP32S2
  uint8_t       free_esp32s2_494[1];       // 494 - 2 bytes extra because of WebCamCfg 32-bit offset
#else
  uint8_t       free_esp32_484[17];        // 484
#endif
#endif  // ESP32

  uint8_t       ex_my_adc0;                // 495  Free since 9.0.0.1 - Do not use anymore because of ESP32S3

  // ----------------------------------------
  // End of remapping for non-ESP32S3
  // ----------------------------------------
#endif // ESP32S3

  uint16_t      light_pixels;              // 496
  uint8_t       light_color[5];      // 498  LST_MAX = 5
  uint8_t       light_correction;          // 49D
  uint8_t       light_dimmer;              // 49E
  uint8_t       rule_enabled;              // 49F
  uint8_t       rule_once;                 // 4A0  Bit 6+7 used by xdrv_10_scripter
  uint8_t       light_fade;                // 4A1
  uint8_t       light_speed;               // 4A2
  uint8_t       light_scheme;              // 4A3
  uint8_t       light_width;               // 4A4
  uint8_t       knx_GA_registered;         // 4A5  Number of Group Address to read
  uint16_t      light_wakeup;              // 4A6
  uint8_t       knx_CB_registered;         // 4A8  Number of Group Address to write
  uint8_t       switchmode[MAX_SWITCHES_SET];  // 4A9
  uint8_t       global_sensor_index[3];    // 4C5
  uint16_t      dns_timeout;               // 4C8
  uint8_t       ds3502_state[MAX_DS3502];  // 4CA
  uint16_t      influxdb_port;             // 4CE
  power_t       interlock[MAX_INTERLOCKS_SET];  // 4D0 MAX_INTERLOCKS = MAX_RELAYS / 2
  int8_t        shutter_tilt_config[5][MAX_SHUTTERS];  //508
  int8_t        shutter_tilt_pos[MAX_SHUTTERS];        //51C
  uint16_t      influxdb_period;           // 520
  uint16_t      rf_duplicate_time;         // 522
  int32_t       weight_absconv_a;          // 524
  int32_t       weight_absconv_b;          // 528
  uint16_t      mqtt_keepalive;            // 52C
  uint16_t      mqtt_socket_timeout;       // 52E
  uint8_t       mqtt_wifi_timeout;         // 530
  uint8_t       ina219_mode;               // 531

  uint16_t      ex_pulse_timer[8];         // 532  ex_pulse_timer free since 11.0.0.3

  uint16_t      button_debounce;           // 542
  uint32_t      ipv4_address[5];           // 544
  uint32_t      ipv4_rgx_address;          // 558
  uint32_t      ipv4_rgx_subnetmask;       // 55C
  uint16_t      pwm_value_ext[16-5];       // 560  Extension to pwm_value to store up to 16 PWM for ESP32. This array stores values 5..15
  uint16_t      pn532_pack;                // 576
  int32_t       weight_offset;             // 578
  uint16_t      pulse_timer[MAX_PULSETIMERS];  // 57C
  SysMBitfield1 flag2;                     // 5BC
  uint32_t      pulse_counter[MAX_COUNTERS];  // 5C0
  uint16_t      pulse_counter_type;        // 5D0
  uint16_t      pulse_counter_debounce;    // 5D2
  uint8_t       rf_code[17][9];            // 5D4
  uint8_t       timezone_minutes;          // 66D
  uint16_t      switch_debounce;           // 66E
  Timer         timer[MAX_TIMERS];         // 670
  int           latitude;                  // 6B0
  int           longitude;                 // 6B4
  uint16_t      knx_physsical_addr;        // 6B8  (address_t is a uint16_t)
  uint16_t      knx_GA_addr[MAX_KNX_GA];   // 6BA  (address_t is a uint16_t) x KNX_max_GA
  uint16_t      knx_CB_addr[MAX_KNX_CB];   // 6CE  (address_t is a uint16_t) x KNX_max_CB
  uint8_t       knx_GA_param[MAX_KNX_GA];  // 6E2  Type of Input (relay changed, button pressed, sensor read <-teleperiod)
  uint8_t       knx_CB_param[MAX_KNX_CB];  // 6EC  Type of Output (set relay, toggle relay, reply sensor value)
  Mcp230xxCfg   mcp230xx_config[16];       // 6F6
  uint8_t       mcp230xx_int_prio;         // 716
  SensorCfg1    SensorBits1;               // 717  On/Off settings used by Sensor Commands
  uint16_t      mcp230xx_int_timer;        // 718
  uint8_t       rgbwwTable[5];             // 71A
  uint8_t       user_template_base;        // 71F
  char          user_template_name[15];    // 720  15 bytes - Backward compatibility since v8.2.0.3

#ifdef ESP8266
  uint8_t       ex_user_template8[5];      // 72F  14 bytes (ESP8266) - Free since 9.0.0.1 - only 5 bytes referenced now
#endif  // ESP8266
#ifdef ESP32
  uint8_t       webcam_clk;                // 72F
  WebCamCfg2    webcam_config2;            // 730
#endif  // ESP32
  uint16_t      artnet_universe;           // 734
  uint16_t      modbus_sbaudrate;          // 736
  uint16_t      shutter_motorstop;         // 738
  uint8_t       battery_level_percent;     // 73A
  uint8_t       hdmi_addr[2];              // 73B  HDMI CEC physical address - warning this is a non-aligned uint16

  uint8_t       novasds_startingoffset;    // 73D
  uint8_t       web_color[18][3];          // 73E
  uint16_t      display_width;             // 774
  uint16_t      display_height;            // 776
  uint16_t      baudrate;                  // 778
  uint16_t      sbaudrate;                 // 77A
  EnergyUsage   energy_usage;              // 77C
  uint32_t      sensors[2][4];             // 794  Disable individual (0) sensor drivers / (1) GUI sensor output
  uint32_t      energy_kWhtotal_time;      // 7B4
  uint32_t      weight_item;               // 7B8  Weight of one item in gram * 10
  uint16_t      ledmask;                   // 7BC
  uint16_t      weight_max;                // 7BE  Total max weight in kilogram
  uint32_t      weight_reference;          // 7C0  Reference weight in gram
  uint32_t      weight_calibration;        // 7C4
  uint32_t      energy_frequency_calibration;  // 7C8  Also used by HX711 to save last weight
  uint16_t      web_refresh;               // 7CC
  char          script_pram[5][10];        // 7CE
  char          rules[MAX_RULE_SETS][MAX_RULE_SIZE];  // 800  Uses 512 bytes in v5.12.0m, 3 x 512 bytes in v5.14.0b
  TuyaFnidDpidMap tuya_fnid_map[MAX_TUYA_FUNCTIONS];  // E00  32 bytes
  uint16_t      ina226_r_shunt[4];         // E20
  uint16_t      ina226_i_fs[4];            // E28
  uint16_t      tariff[4][2];              // E30
  uint16_t      shutter_opentime[MAX_SHUTTERS];      // E40
  uint16_t      shutter_closetime[MAX_SHUTTERS];     // E48
  int16_t       shuttercoeff[5][MAX_SHUTTERS];       // E50
  uint8_t       shutter_options[MAX_SHUTTERS];       // E78
  uint8_t       shutter_set50percent[MAX_SHUTTERS];  // E7C
  uint8_t       shutter_position[MAX_SHUTTERS];      // E80
  uint8_t       shutter_startrelay[MAX_SHUTTERS];    // E84
  uint8_t       pcf8574_config[MAX_PCF8574];         // E88
  uint8_t       ot_hot_water_setpoint;     // E8C
  uint8_t       ot_boiler_setpoint;        // E8D
  uint8_t       ot_flags;                  // E8E
  uint8_t       ledpwm_mask;               // E8F
  uint16_t      dimmer_hw_min;             // E90
  uint16_t      dimmer_hw_max;             // E92
  uint32_t      deepsleep;                 // E94
  uint16_t      hass_new_discovery;        // E98  ex2_energy_power_delta on 8.4.0.3, replaced on 8.5.0.1
  uint8_t       shutter_motordelay[MAX_SHUTTERS];    // E9A
  int8_t        temp_comp;                 // E9E
  uint8_t       weight_change;             // E9F
  uint8_t       web_color2[2][3];          // EA0  Needs to be on integer / 3 distance from web_color
  uint16_t      zcdimmerset[5];            // EA6

  uint8_t       free_eb0[22];              // EB0  22 bytes

  uint8_t       shift595_device_count;     // EC6
  uint8_t       sta_config;                // EC7
  uint8_t       sta_active;                // EC8
  uint8_t       rule_stop;                 // EC9
  uint16_t      syslog_port;               // ECA
  uint8_t       syslog_level;              // ECC
  uint8_t       webserver;                 // ECD
  uint8_t       weblog_level;              // ECE
  uint8_t       mqtt_fingerprint[2][20];   // ECF
  uint8_t       influxdb_version;          // EF7
  SOBitfield4   flag4;                     // EF8
  uint16_t      mqtt_port;                 // EFC
  uint8_t       serial_config;             // EFE
  uint8_t       wifi_output_power;         // EFF
  uint8_t       shutter_accuracy;          // F00
  uint8_t       mqttlog_level;             // F01
  uint8_t       sps30_inuse_hours;         // F02
  uint8_t       hotplug_scan;              // F03
  uint8_t       bri_power_on;              // F04
  PWMDimmerCfg  pwm_dimmer_cfg;            // F05
  uint8_t       bri_preset_low;            // F06
  uint8_t       bri_preset_high;           // F07
  int8_t        hum_comp;                  // F08
  uint8_t       wifi_channel;              // F09
  uint8_t       wifi_bssid[6];             // F0A
  uint8_t       as3935_sensor_cfg[5];      // F10
  As3935IntCfg  as3935_functions;          // F15
  As3935Param   as3935_parameter;          // F16
  uint64_t      zb_ext_panid;              // F18
  uint64_t      zb_precfgkey_l;            // F20
  uint64_t      zb_precfgkey_h;            // F28
  uint16_t      zb_pan_id;                 // F30
  uint8_t       zb_channel;                // F32
  int8_t        zb_txradio_dbm;            // F33
  uint16_t      pms_wake_interval;         // F34
  uint8_t       config_version;            // F36  Setting mem layout identifier for different ESP types - don't move!
  uint8_t       windmeter_pulses_x_rot;    // F37
  uint16_t      windmeter_radius;          // F38
  uint16_t      windmeter_pulse_debounce;  // F3A
  int16_t       windmeter_speed_factor;    // F3C
  uint8_t       windmeter_tele_pchange;    // F3E
  uint8_t       ledpwm_on;                 // F3F
  uint8_t       ledpwm_off;                // F40
  uint8_t       tcp_baudrate;              // F41
  uint8_t       fallback_module;           // F42
  uint8_t       shutter_mode;              // F43
  uint16_t      energy_power_delta[3];     // F44
  uint16_t      shutter_pwmrange[2][MAX_SHUTTERS];  // F4A
  uint8_t       dimmer_step;               // F5A
  uint8_t       shd_leading_edge;          // F5B
  uint16_t      shd_warmup_brightness;     // F5C
  uint8_t       shd_warmup_time;           // F5E
  uint8_t       tcp_config;                // F5F
  uint8_t       light_step_pixels;				 // F60

  uint8_t       hdmi_cec_device_type;      // F61  - v13.1.0.1 (was ex_modbus_sbaudrate v12.2.0.5)

  uint8_t       modbus_sconfig;            // F62

  uint8_t       free_f63[13];              // F63 - Decrement if adding new Setting variables just above and below

  // Only 32 bit boundary variables below
  uint32_t      touch_threshold;           // F70
  SOBitfield6   flag6;                     // F74
  uint16_t      flowratemeter_calibration[2];// F78
  int32_t       energy_kWhexport_ph[3];    // F7C
  uint32_t      eth_ipv4_address[5];       // F88

  uint32_t      ex_energy_kWhtotal;        // F9C

  SBitfield1    sbflag1;                   // FA0
  TeleinfoCfg   teleinfo;                  // FA4
  uint64_t      rf_protocol_mask;          // FA8
  uint8_t       device_group_tie[4];       // FB0
  SOBitfield5   flag5;                     // FB4
  uint16_t      pulse_counter_debounce_low;   // FB8
  uint16_t      pulse_counter_debounce_high;  // FBA
  uint32_t      keeloq_master_msb;         // FBC
  uint32_t      keeloq_master_lsb;         // FC0
  uint32_t      keeloq_serial;             // FC4
  uint32_t      keeloq_count;              // FC8
  uint32_t      device_group_share_in;     // FCC  Bitmask of device group items imported
  uint32_t      device_group_share_out;    // FD0  Bitmask of device group items exported
  uint32_t      bootcount_reset_time;      // FD4
  SysMBitfield2 mbflag2;                   // FD8
  uint32_t      shutter_button[MAX_SHUTTER_KEYS];  // FDC
  uint32_t      i2c_drivers[3];            // FEC  I2cDriver
  uint32_t      cfg_timestamp;             // FF8
  uint32_t      cfg_crc32;                 // FFC
} TSettings2;


TSettings2* Settings2 = nullptr;

static_assert(sizeof(TSettings2) == 4096, "TSettings Size is not correct");

typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint16_t data;                           // Allow bit manipulation
  struct {
    uint16_t system_init : 1;              // Changing layout here needs adjustments in xdrv_10_rules.ino too
    uint16_t system_boot : 1;
    uint16_t time_init : 1;
    uint16_t time_set : 1;
    uint16_t mqtt_connected : 1;
    uint16_t mqtt_disconnected : 1;
    uint16_t wifi_connected : 1;
    uint16_t wifi_disconnected : 1;
    uint16_t eth_connected : 1;
    uint16_t eth_disconnected : 1;
    uint16_t http_init : 1;
    uint16_t shutter_moved : 1;
    uint16_t shutter_moving : 1;
    uint16_t spare13 : 1;
    uint16_t spare14 : 1;
    uint16_t spare15 : 1;
  };
} RulesBitfield;

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

// See issue https://github.com/esp8266/Arduino/issues/2913
#ifdef ESP8266
#ifdef USE_ADC_VCC
  ADC_MODE(ADC_VCC);                       // Set ADC input for Power Supply Voltage usage
#endif
#endif

#endif  // ENABLE_DEVFEATURE__SETTINGS_NEW_STRUCT_2023

































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
const uint8_t CFG_ROTATES = 1;//8;          // Number of flash sectors used (handles uploads)

uint32_t settings_location = SETTINGS_LOCATION;
// uint32_t settings_crc = 0;
uint32_t settings_crc32 = 0;
uint8_t *settings_buffer = nullptr;
#endif // ESP8266

int16_t GetFunctionIDbyFriendlyName(const char* c);

void SettingsLoad_CheckSuccessful();

void SettingsInit();

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



#ifndef ENABLE_DEVFEATURE__SETTINGS_NEW_STRUCT_2023
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
#endif // ENABLE_DEVFEATURE__SETTINGS_NEW_STRUCT_2023



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
    //uint32_t buzzer_enable : 1;            // bit 17 (v6.6.0.1)  - SetOption67 - Enable buzzer when available
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

#ifndef ENABLE_DEVFEATURE__SETTINGS_NEW_STRUCT_2023
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
#endif // ENABLE_DEVFEATURE__SETTINGS_NEW_STRUCT_2023

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
  uint8_t type; //phase out now with multipin?
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

typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint8_t data;                           // Allow bit manipulation using SetOption
  struct {                                 // SetOption0 .. SetOption31
    uint8_t buzzer_freq_mode : 1;               // bit 0              - SetOption0  - Save power state and use after restart
    uint8_t buzzer_enable : 1;
    uint8_t reserved : 6;          // bit 1              - SetOption1  - Control button multipress
   
  };
} SysBitfield_Drivers;



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
  /**
   * Stored as ints for data savings, flat change to int
   * */
  int16_t       altitude;            
  float           latitude;      //54.5 shall be 54500000
  float           longitude;               
  SysBitfield_Sensors flags;
};

struct DisplaySettings{
  uint8_t       model; 
  uint8_t       mode;
  uint8_t       refresh;
  uint8_t       rows;
  uint8_t       cols[2];
  uint8_t       address[8];
  uint8_t       dimmer;
  uint8_t       size;
  uint8_t       font;
  uint8_t       rotate;
  uint16_t      width;
  uint16_t      height;
};



#ifndef ENABLE_DEVFEATURE__SETTINGS_NEW_STRUCT_2023
typedef union {
  uint8_t data;
  struct {
    uint8_t spare0 : 1;
    uint8_t spare1 : 1;
    uint8_t bh1750_1_resolution : 2;       // Sensor10 1,2,3
    uint8_t bh1750_2_resolution : 2;
    uint8_t hx711_json_weight_change : 1;  // Sensor34 8,x - Enable JSON message on weight change
    uint8_t mhz19b_abc_disable : 1;        // Disable ABC (Automatic Baseline Correction for MHZ19(B) (0 = Enabled (default), 1 = Disabled with Sensor15 command)
  };
} SensorCfg1;
#endif // ENABLE_DEVFEATURE__SETTINGS_NEW_STRUCT_2023


#define palette_encoded_users_colour_map_LENGTH 200

// // #define PALETTE_USER_NAME_LIST_LENGTH 300
struct AnimationSettings{
//   // HsbColour map ids
//   uint8_t palette_hsbid_users_colour_map[20*20];//20][20]; 
//   //rgbcct user values
  uint8_t palette_rgbcct_users_colour_map[5*5];//20][20]; 
//   //generic variable buffer
  uint8_t palette_encoded_users_colour_map[palette_encoded_users_colour_map_LENGTH];//20][20]; 
//   // Change names to be a klist =   // char    palette_user_variable_name_ctr[20][20];

//   //move into devicelist?
//   //hmm, maybe not, device list needs to be moved into settings storage first, currently runtime variable
//   // char    palette_user_variable_name_list_ctr[PALETTE_USER_NAME_LIST_LENGTH]; //100 less


//   // Active pixels in each user palette, can be replaced by setting colour_map is inactive, use function to count active/hold it in ram not memory
//   // uint8_t palette_user_amounts[20]; // move to searching not none type id
//   uint8_t animation_mode;
//   uint8_t animation_palette;
//   //struct transition
//   uint8_t animation_transition_order; 
//   uint8_t animation_transition_method;
//   uint16_t animation_transition_time_ms; //TBD save as seconds
//   uint32_t animation_transition_rate_ms; //TBD save as seconds
//   uint8_t transition_pixels_to_update_as_number;

//   uint8_t xmas_controller_params[10];

};


// Buffer that stores names of sensors as delimeter list
#ifndef DEVICENAMEBUFFER_NAME_INDEX_LENGTH // Memory reduction
#define DEVICENAMEBUFFER_NAME_INDEX_LENGTH 70 
#endif // DEVICENAMEBUFFER_NAME_INDEX_LENGTH
#ifndef DEVICENAMEBUFFER_NAME_BUFFER_LENGTH // Memory reduction
#define DEVICENAMEBUFFER_NAME_BUFFER_LENGTH 400 
#endif // DEVICENAMEBUFFER_NAME_BUFFER_LENGTH
struct DeviceNameBuffer{ // size(230)
  // delimeter name list
  char name_buffer[DEVICENAMEBUFFER_NAME_BUFFER_LENGTH];
  // index array that holds name_list sensor (class name, sensor number)
  /**
   * This needs to be the large unique identifier
   * "Unused" will be the largest possible value, instead of using signed value and setting to "-1", which makes -2->minimum wasted 
   * */
  uint16_t class_id[DEVICENAMEBUFFER_NAME_INDEX_LENGTH]  = { D_MAX_UINT16 }; //hold class id
  uint8_t  device_id[DEVICENAMEBUFFER_NAME_INDEX_LENGTH] = { D_MAX_UINT8  };  //max of X sensors per module
};



#define MODULE_TEMPLATE_MAX_SIZE 500
struct Template_Config{
  uint8_t flags;
  uint8_t       base;        // 71F
  // needs removed like tas and read directly into settings
  mytmplt       hardware;             // 720  29 bytes    parsed user template
  // char          full_ctr[MODULE_TEMPLATE_SIZE]; //for testing

};

struct SystemName{ 
  char          friendly[50]; // Used in titles, set by templates "FriendlyName"
  char          device[50];   // USed as mqtt topic by default, set by templates "Name"
};

struct SettingsMQTT{
  // char topic_prefix[50]; //temp remove
  
   char          host_address[33];             // 1E9 - Keep together with below as being copied as one chunck with reset 6
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
  
  char lwt_topic[40];
  char client_name[50]; 
  char hostname_ctr[50];
  char prefixtopic[50]; // "<devicename>/User extras?"

  /**
   * @brief devfeature
   * @note  enable flags (to be condensed to bitmap) that allow interface to be priority, and reduce subclass reporting for ifchanged
   * 
   */
  struct interface_priority_flags_s{
    uint8_t energy = 0;
    uint8_t light = 0;
  }interface_reporting_priority;

};


#ifndef ENABLE_DEVFEATURE__SETTINGS_NEW_STRUCT_2023
typedef union {
  uint8_t data;
  struct {
  uint8_t type : 3;
  uint8_t invert : 1;
  uint8_t spare4 : 1;
  uint8_t spare5 : 1;
  uint8_t spare6 : 1;
  uint8_t spare7 : 1;
  };
} DisplayOptions;
#endif // ENABLE_DEVFEATURE__SETTINGS_NEW_STRUCT_2023


#ifdef ESP32
// typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
//   uint32_t data;
//   struct {
//     uint32_t stream : 1;
//     uint32_t mirror : 1;
//     uint32_t flip : 1;
//     uint32_t rtsp : 1;
//     uint32_t spare4 : 1;
//     uint32_t spare5 : 1;
//     uint32_t spare6 : 1;
//     uint32_t spare7 : 1;
//     uint32_t spare8 : 1;
//     uint32_t spare9 : 1;
//     uint32_t spare10 : 1;
//     uint32_t spare11 : 1;
//     uint32_t spare12 : 1;
//     uint32_t spare13 : 1;
//     uint32_t spare14 : 1;
//     uint32_t spare15 : 1;
//     uint32_t spare16 : 1;
//     uint32_t spare17 : 1;
//     uint32_t spare18 : 1;
//     uint32_t contrast : 3;
//     uint32_t brightness : 3;
//     uint32_t saturation : 3;
//     uint32_t resolution : 4;
//   };
// } WebCamCfg;
#ifndef ENABLE_DEVFEATURE__SETTINGS_NEW_STRUCT_2023
typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint32_t data;
  struct {
    uint32_t stream : 1;
    uint32_t mirror : 1;
    uint32_t flip : 1;
    uint32_t rtsp : 1;
    uint32_t awb : 1;
    uint32_t awb_gain : 1;
    uint32_t aec : 1;
    uint32_t aec2 : 1;
    uint32_t agc : 1;
    uint32_t raw_gma : 1;
    uint32_t lenc : 1;
    uint32_t colorbar : 1;
    uint32_t wpc : 1;
    uint32_t dcw : 1;
    uint32_t bpc : 1;
    uint32_t spare15 : 1;
    uint32_t spare16 : 1;
    uint32_t feature : 2;
    uint32_t contrast : 3;
    uint32_t brightness : 3;
    uint32_t saturation : 3;
    uint32_t resolution : 4;
  };
} WebCamCfg;

typedef union {
  uint32_t data;
  struct {
    uint32_t wb_mode : 3;
    uint32_t ae_level : 3;
    uint32_t aec_value : 11;
    uint32_t gainceiling : 3;
    uint32_t agc_gain: 5;
    uint32_t special_effect : 3;
    uint32_t auth : 1;
    uint32_t spare29 : 1;
    uint32_t spare30 : 1;
    uint32_t upgraded : 1;
  };
} WebCamCfg2;
#endif // ENABLE_DEVFEATURE__SETTINGS_NEW_STRUCT_2023

#endif // esp32


// struct SYSCFG_HEADER



struct SYSCFG {
  // Header (partial loading during boot)
  uint16_t      cfg_holder = 0;                // 000 v6 header
  uint16_t      cfg_size = 0;                  // 002
  unsigned long save_flag = 0;                 // 004
  unsigned long version = 0;                   // 008
  uint16_t      bootcount = 1234;                 // 00C
  uint16_t      cfg_crc = 0;                   // 00E
  // Body
  // System/Core
  uint16_t      bootcount_errors_only;     // E01
  uint8_t       module;                    // 474
  uint8_t       last_module;               // 399
  // Templates
  Template_Config user_template2;
  SystemName      system_name;

  char room_hint[50];

  SysBitfield_System   flag_system;                      // 010
  int16_t       save_data;                 // 014
  myio          module_pins;                     // 484     
  // char          ota_url[101];              // 017
  uint8_t       baudrate;                  // 09D  // saved as (/300) value. ie 9600/300 => 32, 115200=>384?? I want to change this to full uint32_t for higher speed bauds
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

  /**
   * @brief To be phased out, tasmota generic way to have SetOption list
   * These style flags are to be changed to their own, or at least, recreated as a second `param` list to differentiate between legacy tas and my new code.
   * 
   */

  // uint8_t       param[PARAM8_SIZE];        // 2FC  SetOption32 .. SetOption49
  uint8_t       setoption_255[PARAM8_SIZE];


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

  SysBitfield_Drivers    flag_drivers;
  

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
  SensorCfg1    SensorBits1;               // 717  On/Off settings used by Sensor Commands
  
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
  uint16_t      rf_duplicate_time;         // 522

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
  
  uint32_t      i2c_drivers[3];            // FEC  I2cDriver

  // SysBitfield4  flag4;                     // TEMP FIX
// TO SORT

  uint64_t      rf_protocol_mask;          // FA8
 


#ifdef ESP32

  WebCamCfg     webcam_config;             // 44C
  WebCamCfg2     webcam_config2;             // 460

#endif


  // E00 - FFF (4095 ie eeprom size) free locations
  uint32_t      cfg_crc32;                 // FFC
} Settings;

// phasing over tas to mine
// #define param setoption_255 

void TestSettingsLoad();

void TestSettings_ShowLocal_Header();

void SettingsMerge(SYSCFG* saved, SYSCFG* new_settings);



// NEW flag that allows anything to run on reboot
// uint8_t fSystemRestarted = true; // will be restart at the end of the first loop

#ifndef ENABLE_DEVFEATURE_RTC_FASTBOOT_V2
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
#endif // ENABLE_DEVFEATURE_RTC_FASTBOOT_V2



#ifdef ENABLE_DEVFEATURE_RTC_FASTBOOT_V2

typedef struct {
  uint32_t usage1_kWhtotal;
  uint32_t usage2_kWhtotal;
  uint32_t return1_kWhtotal;
  uint32_t return2_kWhtotal;
  uint32_t last_return_kWhtotal;
  uint32_t last_usage_kWhtotal;
} EnergyUsage;


typedef struct {
  uint16_t      valid;                     // 280  (RTC memory offset 100 - sizeof(RTCRBT))
  uint8_t       fast_reboot_count;         // 282
  uint8_t       free_003[1];               // 283
} TRtcReboot;
TRtcReboot RtcReboot;
#ifdef ESP32
RTC_NOINIT_ATTR TRtcReboot RtcDataReboot;
#endif  // ESP32

typedef struct {
  uint16_t      valid;                     // 290  (RTC memory offset 100)
  uint8_t       oswatch_blocked_loop;      // 292
  uint8_t       ota_loader;                // 293
  uint32_t      energy_kWhtoday;           // 294
  uint32_t      energy_kWhtotal;           // 298
  volatile uint32_t pulse_counter[MAX_COUNTERS];  // 29C - See #9521 why volatile
  power_t       power;                     // 2AC
  EnergyUsage   energy_usage;              // 2B0
  uint32_t      nextwakeup;                // 2C8
  uint32_t      baudrate;                  // 2CC
  uint32_t      ultradeepsleep;            // 2D0
  uint16_t      deepsleep_slip;            // 2D4
  uint8_t       improv_state;              // 2D6

  uint8_t       free_2d7[1];               // 2D7

  int32_t       energy_kWhtoday_ph[3];     // 2D8
  int32_t       energy_kWhtotal_ph[3];     // 2E4

                                           // 2F0 - 2FF free locations
} TRtcSettings;
TRtcSettings RtcSettings;
#ifdef ESP32
RTC_NOINIT_ATTR TRtcSettings RtcDataSettings;
#endif  // ESP32


#endif // ENABLE_DEVFEATURE_RTC_FASTBOOT_V2


uint8_t flag_boot_complete = false;


/**
 * Group of functions for storing/loading driver name list
 * */
// enum SETTINGS_DEVICENAME_LIST_IDS{
//   DEVICENAME_EMPTY_ID = -1,
//   DEVICENAME_DISABLED_ID = -2,
// };
// int8_t      AddDeviceName(const char* name_ctr, int16_t class_id = 1000, int8_t driver_id = 255);
// int8_t      RemoveDeviceName(const char* name_ctr, int16_t class_id = 1000, int8_t driver_id = 255);
// const char* GetDeviceNameWithEnumNumber(int16_t class_id, int8_t driver_id, char* buffer, uint16_t buffer_size);
// int8_t      GetDeviceNameCount(int16_t class_id = 1000);



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


#ifndef ENABLE_DEVFEATURE__SETTINGS_NEW_STRUCT_2023
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
#endif // ENABLE_DEVFEATURE__SETTINGS_NEW_STRUCT_2023


  
  void parse_JSONCommand(JsonParserObject obj);
  
  

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

uint8_t settings_save_decounter_seconds_delayed_save = 0;


// These are used only at runtime, and not saved eg TasmotaGlobals
struct RUNTIME_VALUES{
  uint8_t sleep;                              // Current copy of Settings.sleep
  uint32_t tSavedUpdateLoopStatistics;
  uint8_t energy_driver;                    // Energy monitor configured
  uint8_t light_driver;                     // Light module configured
  uint8_t light_type;                       // Light types
 

  
}runtime_var;


// phased out, new tas method only records used pins, otherwise returns 99 for not set
// instead of remembering giant list
//THIS WONT BE HERE
// uint8_t pin[GPIO_MAX_ID];                      // Possible pin configurations

// new method which only holds the max pins to replace above array
// #ifdef ESP8266
// uint8_t gpio_pin_new[MAX_GPIO_PIN] = { 0 };     // GPIO functions indexed by pin number
// // #endif  // ESP8266 - ESP32

// #ifdef ESP32
// uint16_t gpio_pin[MAX_GPIO_PIN] = { 0 };    // GPIO functions indexed by pin number
// #endif  // ESP32





uint8_t active_device = 1;                  // Active device in ExecuteCommandPower


uint8_t pwm_inverted = 0;                   // PWM inverted flag (1 = inverted)
uint8_t counter_no_pullup = 0;              // Counter input pullup flag (1 = No pullup)


//phased out for runtime.energy_Driver
        //uint8_t energy_flg = 0;                     // Energy monitor configured


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

bool i2c_enabled = false;                       // I2C configured
#ifdef ESP32
bool i2c_enabled_2 = false;                       // I2C configured, second controller on ESP32, Wire1
#endif

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

int16_t     SwitchMode_GetID_by_Name(const char* c);
const char* SwitchMode_GetName_by_ID(uint8_t id, char* buffer, uint8_t buflen);

#ifndef WEB_LOG_SIZE
#define WEB_LOG_SIZE 200       // Max number of characters in weblog
#endif // WEB_LOG_SIZE
#ifndef LOG_BUFFER_SIZE

#ifdef ESP8266
#define LOG_BUFFER_SIZE 400 //if debug is enabled, push this to 1000, if not, keep at much smaller 300
#else //esp32
#define LOG_BUFFER_SIZE 1000
#endif
#endif // LOG_BUFFER_SIZE
char log_data[LOG_BUFFER_SIZE];                       // Logging
char web_log[WEB_LOG_SIZE] = {'\0'};        // Web log buffer - REMEMBERS EVERYTHING for new load

#define RESPONSE_MESSAGE_BUFFER_SIZE 100
char response_msg[RESPONSE_MESSAGE_BUFFER_SIZE] = {0};


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

