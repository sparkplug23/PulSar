#ifndef _MSYSTEMCONSTANTS_H_
#define _MSYSTEMCONSTANTS_H_

#include <stdint.h>

/*********************************************************************************************\
 * Power Type
\*********************************************************************************************/

typedef uint32_t power_t;                   // Power (Relay) type
const uint32_t POWER_MASK = 0xFFFFFFFFUL;   // Power (Relay) full mask
const uint32_t POWER_SIZE = 32;             // Power (relay) bit count

/*********************************************************************************************\
 * Constants
\*********************************************************************************************/

/**
 * @brief 
 * I need to recheck these sizes match my GPIO enums, as its causing issues
 * 
 */
#ifdef ESP8266
const uint8_t MAX_INTERLOCKS = 16;          // Max number of interlock groups (up to MAX_INTERLOCKS_SET)
#endif  // ESP8266
#ifdef ESP32
const uint8_t MAX_INTERLOCKS = 16;          // Max number of interlock groups (up to MAX_INTERLOCKS_SET)
#endif  // ESP32

const uint16_t VL53LXX_MAX_SENSORS = 8;     // Max number of VL53L0X sensors


const uint8_t MAX_RELAYS_SET = 32;          // Max number of relays MUST be 32bit wide to clear correctly.
const uint8_t MAX_KEYS_SET = 32;            // Max number of keys
// Changes to the following MAX_ defines will impact settings layout
const uint8_t MAX_INTERLOCKS_SET = 14;      // Max number of interlock groups (MAX_RELAYS_SET / 2)
const uint8_t MAX_SWITCHES_SET = 28;        // Max number of switches
const uint8_t MAX_DIGITAL_LEDS = 8;                 // MAX_LEDS Max number of leds
const uint8_t MAX_PWMS_LEGACY = 5;          // Max number of PWM channels in first settings block - Legacy limit for ESP8266, but extended for ESP32 (see below)
#ifdef ESP32                                // Max number of PWM channels (total including extended) - ESP32 only
  #if CONFIG_IDF_TARGET_ESP32
  const uint8_t MAX_PWMS = 5;              // ESP32: 16 ledc PWM channels in total - TODO for now
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
// const uint16_t VL53LXX_MAX_SENSORS = 8;     // Max number of VL53L0X sensors
const char WIFI_HOSTNAME[]  = "%s-%04d"; //!PROGMEM    // Expands to <MQTT_TOPIC>-<last 4 decimal chars of MAC address>
const uint32_t HLW_PREF_PULSE = 12530;      // was 4975us = 201Hz = 1000W
const uint32_t HLW_UREF_PULSE = 1950;       // was 1666us = 600Hz = 220V
const uint32_t HLW_IREF_PULSE = 3500;       // was 1666us = 600Hz = 4.545A
const uint8_t MQTT_RETRY_SECS = 10;         // Minimum seconds to retry MQTT connection
const uint32_t PWM_RANGE = 1023;            // 255..1023 needs to be devisible by 256
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
const uint8_t IMMINENT_RESET_FACTOR = 10;   // Factor to extent button hold time for imminent Reset to default 40 seconds using KEY_HOLD_TIME of 40
const uint8_t BOOT_LOOP_TIME = 10;         // Number of seconds to stop detecting boot loops
const uint16_t FLOATSZ = 16;                // Max number of characters in float result from dtostrfd (max 32)
const uint16_t TBUFFER_SIZE_FLOAT = 16; // TBUFFER are (T)emporary Buffers that are used for conversions
#define STANDARD_SMS_CHAR_LENGTH 160
const uint8_t SENSOR_MAX_MISS = 5;          // Max number of missed sensor reads before deciding it's offline
const uint32_t SOFT_BAUDRATE = 9600;        // Default software serial baudrate
const uint32_t APP_BAUDRATE = 115200;       // Default serial baudrate
const uint8_t MAX_MQTT_CONNECTIONS = 2;

// Changes to the following MAX_ defines need to be in line with enum SettingsTextIndex
const uint8_t MAX_MQTT_PREFIXES = 3;        // Max number of MQTT prefixes (cmnd, stat, tele)
const uint8_t MAX_SSIDS = 2;                // Max number of SSIDs
const uint8_t MAX_STATE_TEXT = 4;           // Max number of State names (OFF, ON, TOGGLE, HOLD)
const uint8_t MAX_NTP_SERVERS = 3;          // Max number of NTP servers
const uint8_t MAX_RULE_MEMS = 16;           // Max number of saved vars
const uint8_t MAX_FRIENDLYNAMES = 8;        // Max number of Friendly names
const uint8_t MAX_BUTTON_TEXT = 32;         // Max number of GUI button labels
const uint8_t MAX_GROUP_TOPICS = 4;         // Max number of Group Topics
const uint8_t MAX_DEV_GROUP_NAMES = 4;      // Max number of Device Group names

#ifdef ESP8266
const uint8_t MAX_ADCS = 1;                 // Max number of ESP8266 ADC pins
const uint8_t MAX_SWITCHES_TXT = 8;         // Max number of switches user text
#endif  // ESP8266
#ifdef ESP32
  #if CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32C3
  const uint8_t MAX_ADCS = 5;               // Max number of ESP32-C3 ADC pins (ADC2 pins are unusable with Wifi enabled)
  #elif CONFIG_IDF_TARGET_ESP32C6
  const uint8_t MAX_ADCS = 7;               // Max number of ESP32 ADC pins (ADC2 pins are unusable with Wifi enabled)
  #else   // ESP32
  const uint8_t MAX_ADCS = 8;               // Max number of ESP32 ADC pins (ADC2 pins are unusable with Wifi enabled)
  #endif  // ESP32C3
const uint8_t MAX_SWITCHES_TXT = 28;        // Max number of switches user text
#endif  // ESP32

const uint16_t TOPSZ = 151;                 // Max number of characters in topic string

const uint32_t START_VALID_UTC_TIME = 1697014158;  // Time is synced and after 2023 October 11, this will need adjusting when NTP code is updated


/*********************************************************************************************\
 * Defines
\*********************************************************************************************/

#define MAX_RULE_TIMERS        8            // Max number of rule timers (4 bytes / timer)
#define MAX_RULE_VARS          5            // Max number of rule variables (10 bytes / variable)

#define tmin(a,b) ((a)<(b)?(a):(b))
#define tmax(a,b) ((a)>(b)?(a):(b))

#define STR_HELPER(x) #x
#ifndef STR
#define STR(x) STR_HELPER(x)
#endif

enum LedStateOptions {LED_OFF1, LED_POWER, LED_MQTTSUB, LED_POWER_MQTTSUB, LED_MQTTPUB, LED_POWER_MQTTPUB, LED_MQTT, LED_POWER_MQTT, MAX_LED_OPTION};

// Change these to have matching words
enum ExecuteCommandPowerOptions { POWER_OFF, POWER_ON, POWER_TOGGLE, POWER_BLINK, POWER_BLINK_STOP, POWER_OFF_FORCE,
                                  POWER_OFF_NO_STATE = 8, POWER_ON_NO_STATE, POWER_TOGGLE_NO_STATE,
                                  POWER_SHOW_STATE = 16 };
                                  
enum PowerOnStateOptions { 
  POWER_ALL_OFF, 
  POWER_ALL_ON, 
  POWER_ALL_SAVED_TOGGLE, 
  POWER_ALL_SAVED, 
  POWER_ALL_ALWAYS_ON, 
  POWER_ALL_OFF_PULSETIME_ON 
};

enum TopicOptions { CMND, STAT, TELE, nu1, RESULT_OR_CMND, RESULT_OR_STAT, RESULT_OR_TELE };


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

/**
 * @brief Initial used with buttons, but should be expanded into rule type events
 **/
enum INPUT_TYPE_IDS{ //style fron nextion
  INPUT_TYPE_SINGLE_PRESS_ID,
  INPUT_TYPE_MULTIPLE_PRESS_ID,
  INPUT_TYPE_SINGLE_HOLD_ID,
  INPUT_TYPE_SINGLE_HOLD_RELEASED_ID,
  INPUT_TYPE_SINGLE_HOLD_RESET_TIME_ID,
  INPUT_TYPE_LENGTH_ID
};

/**
 * @brief 
 * 
 * Just delete these, dont like the params8
 * 
 */
enum SO32_49Index { P_HOLD_TIME,              // SetOption32 - (Button/Switch) Key hold time detection in decaseconds (default 40)
                    P_MAX_POWER_RETRY,        // SetOption33 - (Energy) Maximum number of retries before deciding power limit overflow (default 5)
                    P_BACKLOG_DELAY,          // SetOption34 - (Backlog) Minimal delay in milliseconds between executing backlog commands (default 200)
                    P_MDNS_DELAYED_START,     // SetOption35 - (mDNS) Number of seconds before mDNS is started (default 0) - Obsolete
                    P_BOOT_LOOP_OFFSET,       // SetOption36 - (Restart) Number of restarts to start detecting boot loop (default 1)
                    P_SO37_FREE,              // SetOption37 - (Light) RGB and White channel separation (default 0)
                    P_IR_UNKNOW_THRESHOLD,    // SetOption38 - (IR) Set the smallest sized "UNKNOWN" message packets we actually care about (default 6, max 255)
                    P_CSE7766_INVALID_POWER,  // SetOption39 - (CSE7766) Number of invalid power measurements before declaring it invalid allowing low load measurments (default 128)
                    P_HOLD_IGNORE,            // SetOption40 - (Button/Shutter) Ignore button change in seconds (default 0)
                    P_ARP_GRATUITOUS,         // SetOption41 - (Wifi) Interval in seconds between gratuitous ARP requests (default 60)
                    P_OVER_TEMP,              // SetOption42 - (Energy) Turn all power off at or above this temperature (default 90C)
                    P_ROTARY_MAX_STEP,        // SetOption43 - (Rotary) Rotary step boundary (default 10)
                    P_IR_TOLERANCE,           // SetOption44 - (IR) Base tolerance percentage for matching incoming IR messages (default 25, max 100)
                    P_BISTABLE_PULSE,         // SetOption45 - (Bistable) Pulse time for two coil bistable latching relays (default 40)
                    P_POWER_ON_DELAY,         // SetOption46 - (PowerOn) Add delay of 10 x value milliseconds at power on
                    P_POWER_ON_DELAY2,        // SetOption47 - (PowerOn) Add delay of value seconds at power on before activating relays
                    P_DUMMY_RELAYS,           // SetOption48 - (Energy) Support energy dummy relays
                    P_SO49_FREE               // SetOption49
                  };  // Max is PARAM8_SIZE (18) - SetOption32 until SetOption49


#endif