#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#define D_UNIQUE_MODULE_CORE_SETTINGS_ID ((2*1000)+01)

#include "2_CoreSystem/mBaseConfig.h"

#define DATA_BUFFER_TOPIC_MAX_LENGTH    100

#ifdef USE_MODULE_NETWORK_WEBSERVER
  #ifndef DATA_BUFFER_PAYLOAD_MAX_LENGTH
    #define DATA_BUFFER_PAYLOAD_MAX_LENGTH 3000
  #endif
#else
  #ifndef DATA_BUFFER_PAYLOAD_MAX_LENGTH
    #ifdef ESP32
      #define DATA_BUFFER_PAYLOAD_MAX_LENGTH 3000
    #else
      #define DATA_BUFFER_PAYLOAD_MAX_LENGTH 3000
    #endif
  #endif
#endif //USE_MODULE_NETWORK_WEBSERVER




enum DATA_BUFFER_FLAG_SOURCE_IDS
{
  DATA_BUFFER_FLAG_SOURCE_MQTT=0,
  DATA_BUFFER_FLAG_SOURCE_WEBUI
};


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
    uint16_t encoded_type_id : 1; //json,raw
    uint16_t reserved : 10;
  };
} DATA_BUFFER_FLAGS;


struct DATA_BUFFER{
  struct TOPIC{
    char ctr[DATA_BUFFER_TOPIC_MAX_LENGTH];
    uint16_t length_used = 0;
    // uint16_t length_max = DATA_BUFFER_TOPIC_MAX_LENGTH;
  }topic;
  struct PAYLOAD{
    char ctr[DATA_BUFFER_PAYLOAD_MAX_LENGTH];
    uint16_t length_used = 0;
    // uint16_t length_max = DATA_BUFFER_PAYLOAD_MAX_LENGTH;
  }payload;
  uint16_t isserviced = 0; // Set to 0 on new mqtt
  DATA_BUFFER_FLAGS flags;
};
extern struct DATA_BUFFER data_buffer;



/**
 * @brief Complete, slow
 **/
// #define D_DATA_BUFFER_CLEAR()  memset(&data_buffer,0,sizeof(data_buffer))
/**
 * @brief Minimal, fast
 **/
#define D_DATA_BUFFER_CLEAR()             \
    data_buffer.topic.ctr[0]=0;           \
    data_buffer.topic.length_used = 0;    \
    data_buffer.payload.ctr[0]=0;         \
    data_buffer.payload.length_used = 0; 


#include "2_CoreSystem/06_Support/mSupport.h"
#include "2_CoreSystem/05_Logging/mLogging.h"
#include "1_TaskerManager/mTaskerManager.h"
#include "2_CoreSystem/03_HardwareTemplates/mHardwareTemplates.h"
#include "2_CoreSystem/mFirmwareDefaults.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"
#include "2_CoreSystem/11_Languages/mLanguageProgmem.h"
#include "1_TaskerManager/mTaskerManager.h"
#include "2_CoreSystem/02_Time/mTime.h"


#ifdef ESP32
  #include <WiFi.h>
#endif
#ifdef ESP8266
  #include <ESP8266WiFi.h>
  // #include "avr/pgmspace.h"
  #if (defined(__AVR__))
  #include <avr\pgmspace.h>
  #else
  #include <pgmspace.h>
  #endif
#endif


#ifdef ENABLE_DEVFEATURE_SETTINGS__TEXT_BUFFER
// Later I may want to just use this for the settings stuff (not buttons, just the core stuff)
enum SettingsTextIndex { 
                        //  SET_OTAURL,
                        //  SET_MQTTPREFIX1, SET_MQTTPREFIX2, SET_MQTTPREFIX3,  // MAX_MQTT_PREFIXES
                        //  SET_STASSID1, SET_STASSID2,  // MAX_SSIDS
                        //  SET_STAPWD1, SET_STAPWD2,  // MAX_SSIDS
                        //  SET_HOSTNAME, SET_SYSLOG_HOST,
                        //  SET_WEBPWD, SET_CORS,
                        //  SET_MQTT_HOST, SET_MQTT_CLIENT,
                        //  SET_MQTT_USER, SET_MQTT_PWD,
                        //  SET_MQTT_FULLTOPIC, SET_MQTT_TOPIC,
                        //  SET_MQTT_BUTTON_TOPIC, SET_MQTT_SWITCH_TOPIC, SET_MQTT_GRP_TOPIC,
                        //  SET_STATE_TXT1, SET_STATE_TXT2, SET_STATE_TXT3, SET_STATE_TXT4,  // MAX_STATE_TEXT
                         SET_NTPSERVER1, SET_NTPSERVER2, SET_NTPSERVER3,  // MAX_NTP_SERVERS
//                          SET_MEM1, SET_MEM2, SET_MEM3, SET_MEM4, SET_MEM5, SET_MEM6, SET_MEM7, SET_MEM8,
//                          SET_MEM9, SET_MEM10, SET_MEM11, SET_MEM12, SET_MEM13, SET_MEM14, SET_MEM15, SET_MEM16,  // MAX_RULE_MEMS
//                          SET_FRIENDLYNAME1, SET_FRIENDLYNAME2, SET_FRIENDLYNAME3, SET_FRIENDLYNAME4,
//                          SET_FRIENDLYNAME5, SET_FRIENDLYNAME6, SET_FRIENDLYNAME7, SET_FRIENDLYNAME8,  // MAX_FRIENDLYNAMES
//                          SET_BUTTON1, SET_BUTTON2, SET_BUTTON3, SET_BUTTON4, SET_BUTTON5, SET_BUTTON6, SET_BUTTON7, SET_BUTTON8,
//                          SET_BUTTON9, SET_BUTTON10, SET_BUTTON11, SET_BUTTON12, SET_BUTTON13, SET_BUTTON14, SET_BUTTON15, SET_BUTTON16,  // MAX_BUTTON_TEXT
//                          SET_MQTT_GRP_TOPIC2, SET_MQTT_GRP_TOPIC3, SET_MQTT_GRP_TOPIC4,  // MAX_GROUP_TOPICS
//                          SET_TEMPLATE_NAME,
//                          SET_DEV_GROUP_NAME1, SET_DEV_GROUP_NAME2, SET_DEV_GROUP_NAME3, SET_DEV_GROUP_NAME4,  // MAX_DEV_GROUP_NAMES
//                          SET_DEVICENAME,
//                          SET_TELEGRAM_TOKEN, SET_TELEGRAM_CHATID,
// #ifdef ESP8266
//                          SET_ADC_PARAM1,
//                          SET_SWITCH_TXT1, SET_SWITCH_TXT2, SET_SWITCH_TXT3, SET_SWITCH_TXT4, SET_SWITCH_TXT5, SET_SWITCH_TXT6, SET_SWITCH_TXT7, SET_SWITCH_TXT8,  // MAX_SWITCHES_TXT
// #endif  // ESP8266
// #ifdef ESP32
//                          SET_ADC_PARAM1, SET_ADC_PARAM2, SET_ADC_PARAM3, SET_ADC_PARAM4, SET_ADC_PARAM5, SET_ADC_PARAM6, SET_ADC_PARAM7, SET_ADC_PARAM8,  // MAX_ADCS
//                          SET_SWITCH_TXT1, SET_SWITCH_TXT2, SET_SWITCH_TXT3, SET_SWITCH_TXT4, SET_SWITCH_TXT5, SET_SWITCH_TXT6, SET_SWITCH_TXT7, SET_SWITCH_TXT8,  // MAX_SWITCHES_TXT
//                          SET_SWITCH_TXT9, SET_SWITCH_TXT10, SET_SWITCH_TXT11, SET_SWITCH_TXT12, SET_SWITCH_TXT13, SET_SWITCH_TXT14, SET_SWITCH_TXT15, SET_SWITCH_TXT16,  // MAX_SWITCHES_TXT
//                          SET_SWITCH_TXT17, SET_SWITCH_TXT18, SET_SWITCH_TXT19, SET_SWITCH_TXT20, SET_SWITCH_TXT21, SET_SWITCH_TXT22, SET_SWITCH_TXT23, SET_SWITCH_TXT24,  // MAX_SWITCHES_TXT
//                          SET_SWITCH_TXT25, SET_SWITCH_TXT26, SET_SWITCH_TXT27, SET_SWITCH_TXT28,  // MAX_SWITCHES_TXT
// #endif  // ESP32
//                          SET_SHD_PARAM,
//                          SET_RGX_SSID, SET_RGX_PASSWORD,
//                          SET_INFLUXDB_HOST, SET_INFLUXDB_PORT, SET_INFLUXDB_ORG, SET_INFLUXDB_TOKEN, SET_INFLUXDB_BUCKET, SET_INFLUXDB_RP,
//                          SET_CANVAS,
                         SET_MAX, // limit of texts stored in Settings
                         // Index above are not stored in Settings and should be handled specifically in SettingText()
                         SET_BUTTON17, SET_BUTTON18, SET_BUTTON19, SET_BUTTON20, SET_BUTTON21, SET_BUTTON22, SET_BUTTON23, SET_BUTTON24,
                         SET_BUTTON25, SET_BUTTON26, SET_BUTTON27, SET_BUTTON28, SET_BUTTON29, SET_BUTTON30, SET_BUTTON31, SET_BUTTON32,
                         SET_FINAL_MAX
                         };
#endif // ENABLE_DEVFEATURE_SETTINGS__TEXT_BUFFER



// #ifdef ENABLE_DEVFEATURE_BUILD_REPAIR__FIXING_COMPILE_FOR_SONOFF_BASIC_DEC2023
//command source will be useful for rules, 
enum CommandSource { SRC_IGNORE, SRC_MQTT, SRC_RESTART, SRC_BUTTON, SRC_SWITCH, SRC_BACKLOG, SRC_SERIAL, SRC_WEBGUI, SRC_WEBCOMMAND, SRC_WEBCONSOLE, SRC_PULSETIMER,
                     SRC_TIMER, SRC_RULE, SRC_MAXPOWER, SRC_MAXENERGY, SRC_LIGHT, SRC_KNX, SRC_DISPLAY, SRC_WEMO, SRC_HUE, SRC_RETRY, SRC_MAX };
const char kCommandSource[] PROGMEM = "I|MQTT|Restart|Button|Switch|Backlog|Serial|WebGui|WebCommand|WebConsole|PulseTimer|Timer|Rule|MaxPower|MaxEnergy|Light|Knx|Display|Wemo|Hue|Retry";
// #endif 


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

#define CODE_IMAGE 0

//#define USE_DHT                             // Default DHT11 sensor needs no external library
#define USE_ENERGY_SENSOR                   // Use energy sensors (+14k code)
#define USE_HLW8012                         // Use energy sensor for Sonoff Pow and WolfBlitz
#define USE_CSE7766                         // Use energy sensor for Sonoff S31 and Pow R2

/*********************************************************************************************\
 * Power Type
\*********************************************************************************************/

typedef unsigned long power_t;              // Power (Relay) type

/*********************************************************************************************\
 * Constants
\*********************************************************************************************/

// Why 28? Because in addition to relays there may be lights and uint32_t bitmap can hold up to 32 devices
#ifdef ESP8266

#ifndef ENABLE_DEVFEATURE_BUILD_REPAIR__FIXING_RELAY_KEYS_DEFINES_TO_SETTINGS_HEADER

// const uint8_t MAX_RELAYS = 8;               // Max number of relays selectable on GPIO
// const uint8_t MAX_INTERLOCKS = 4;           // Max number of interlock groups (up to MAX_INTERLOCKS_SET)
// const uint8_t MAX_SWITCHES = 8;             // Max number of switches selectable on GPIO
// const uint8_t MAX_KEYS = 8;                 // Max number of keys or buttons selectable on GPIO
#endif
#endif  // ESP8266
#ifdef ESP32
#ifndef ENABLE_DEVFEATURE_BUILD_REPAIR__FIXING_RELAY_KEYS_DEFINES_TO_SETTINGS_HEADER
// const uint8_t MAX_RELAYS = 28;              // Max number of relays selectable on GPIO
// const uint8_t MAX_INTERLOCKS = 14;          // Max number of interlock groups (up to MAX_INTERLOCKS_SET)
// // const uint8_t MAX_SWITCHES = 28;            // Max number of switches selectable on GPIO
// const uint8_t MAX_KEYS = 28;                // Max number of keys or buttons selectable on GPIO
#endif
#endif  // ESP32
const uint8_t MAX_RELAYS_SET = 32;          // Max number of relays
const uint8_t MAX_KEYS_SET = 32;            // Max number of keys
// Changes to the following MAX_ defines will impact settings layout
const uint8_t MAX_INTERLOCKS_SET = 14;      // Max number of interlock groups (MAX_RELAYS_SET / 2)
const uint8_t MAX_SWITCHES_SET = 28;        // Max number of switches
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
const uint32_t POWER_MASK = 0xffffffffUL;   // Power (Relay) full mask
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
const uint8_t MAX_FRIENDLYNAMES = 4;        // Max number of Friendly names
const char WIFI_HOSTNAME[]  = "%s"; //!PROGMEM    // Expands to <MQTT_TOPIC>-<last 4 decimal chars of MAC address>
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
const uint8_t MAX_NTP_SERVERS = 3;          // Max number of NTP servers
const uint32_t SOFT_BAUDRATE = 9600;        // Default software serial baudrate
const uint32_t APP_BAUDRATE = 115200;       // Default serial baudrate

// const uint32_t START_VALID_TIME = 1451602800;  // Time is synced and after 2016-01-01   // Will need adjusting when NTP code is updated
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

enum LedStateOptions {LED_OFF, LED_POWER, LED_MQTTSUB, LED_POWER_MQTTSUB, LED_MQTTPUB, LED_POWER_MQTTPUB, LED_MQTT, LED_POWER_MQTT, MAX_LED_OPTION};

// Change these to have matching words
enum ExecuteCommandPowerOptions { POWER_OFF, POWER_ON, POWER_TOGGLE, POWER_BLINK, POWER_BLINK_STOP,
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
    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void parse_JSONCommand(JsonParserObject obj);

    static const char* PM_MODULE_CORE_SETTINGS_CTR;
    static const char* PM_MODULE_CORE_SETTINGS_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CORE_SETTINGS_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_CORE_SETTINGS_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_SETTINGS_ID; }
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){ return sizeof(mSettings); };
    #endif

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

    // Version 5.2 allow for more flash space
    const uint8_t CFG_ROTATES = 1;//8;          // Number of flash sectors used (handles uploads)

    uint32_t settings_location = SETTINGS_LOCATION;
    // uint32_t settings_crc = 0;
    uint32_t settings_crc32 = 0;
    uint8_t *settings_buffer = nullptr;
  #endif // ESP8266

    void JsonAppend_Settings();    
    void init(void);
    uint16_t CountCharInCtr(const char* tosearch, char tofind);
    int16_t GetIndexOfNthCharPosition(const char* tosearch, char tofind, uint8_t occurance_count);
    void Function_Template_Load();
    void SettingsWrite(const void *pSettings, unsigned nSettingsLen);
  int16_t GetFunctionIDbyFriendlyName(const char* c);

  void SettingsLoad_CheckSuccessful();

  void SettingsInit();

  void SetFlashModeDout(void);
  void SettingsBufferFree(void);
  bool SettingsBufferAlloc(void);

  uint32_t SettingsRead(void *data, size_t size);
  
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

  uint32_t GetCfgCrc32(uint8_t *bytes, uint32_t size);
  uint32_t GetSettingsCrc32(void);

  void SettingsResetStd(void);
  void SettingsResetDst(void);

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

  /**
   * SECTION
   * Templates
   * - MODULE_TEMPLATE
   * - FUNCTION_TEMPLATE
   * - LIGHTING_TEMPLATE
   * - NEXTION_HMI_CONTROL_MAP
   * 
   */

  // a flag status group showing how succesful the boot was
  // 3 state template, used, succuss, fail, none

  #define MODULE_TEMPLATE_MAX_SIZE 500
  enum TemplateSource
  {
    NONE,
    HEADER_CUSTOM,   // custom
    HEADER_TEMPLATE, // from a predefined template
    FILE,            // from a file (loading into internal storage with webui or mqtt command)
    FAILED_LOAD      // error occurs when loaded
  };
  struct TemplateLoading
  {
    struct STATUS{
      uint8_t module                = TemplateSource::NONE;
      uint8_t function              = TemplateSource::NONE;
      uint8_t lighting              = TemplateSource::NONE;
      uint8_t nextion_hmi_input_map = TemplateSource::NONE;
      uint8_t rules                 = TemplateSource::NONE;
    }status;

    // Other
  };






  #include "Sub_mSettings_Stored.h"

  void TestSettingsLoad();
  void TestSettings_ShowLocal_Header();
    
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
  };

  #ifdef ESP8266
  SerialConfig serial_config = SERIAL_8N1;    // Serial interface configuration 8 data bits, No parity, 1 stop bit
  #endif

  // These are used only at runtime, and not saved eg TasmotaGlobals
  struct RUNTIME_GLOBALS{
    uint8_t sleep;                              // Current copy of Settings.sleep
    uint32_t tSavedUpdateLoopStatistics;
    uint8_t energy_driver;                    // Energy monitor configured    // phase this out, more than one can exist
    uint8_t light_driver;                     // Light module configured
    uint8_t light_type;                       // Light types
    TemplateLoading template_loading;
    FIRMWARE_VERSION firmware_version;    
    power_t power = 0;                          // Current copy of Settings.power
    int ota_state_flag = 0;                     // OTA state flag
    int ota_result = 0;                         // OTA result
    int restart_flag = 0;                       // Sonoff restart flag
    int blinks = 201;                           // Number of LED blinks
    uint32_t loop_load_avg = 0;                 // Indicative loop load average
    char *ota_url;                              // OTA url string pointer
    int16_t save_data_counter;                  // Counter and flag for config save to Flash
    uint8_t latching_relay_pulse = 0;           // Latching relay pulse timer
    uint8_t backlog_index = 0;                  // Command backlog index
    uint8_t backlog_pointer = 0;                // Command backlog pointer
    uint8_t blinkspeed = 1;                     // LED blink rate
    uint8_t settings_save_decounter_seconds_delayed_save = 0;    
    uint8_t active_device = 1;                  // Active device in ExecuteCommandPower
    uint8_t pwm_inverted = 0;                   // PWM inverted flag (1 = inverted)
    uint8_t counter_no_pullup = 0;              // Counter input pullup flag (1 = No pullup)
    uint8_t serial_in_byte;                     // Received byte
    uint8_t devices_present = 0;                // Max number of devices supported
    uint8_t seriallog_level;                    // Current copy of Settings.seriallog_level
    uint8_t syslog_level;                       // Current copy of Settings.syslog_level
    uint8_t seriallog_level_during_boot;
    uint8_t my_module_type;                     // Current copy of Settings.module or user template type
    uint8_t last_source = 0;                    // Last command source
    uint8_t mdns_delayed_start = 0;             // mDNS delayed start
    bool stop_flash_rotate = false;             // Allow flash configuration rotation
    bool blinkstate = false;                    // LED state
    bool pwm_present = false;                   // Any PWM channel configured with SetOption15 0
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
    char my_hostname[33];                       // Composed Wifi hostname
    uint8_t flag_boot_complete = false;
    int wifi_state_flag = 0;         // WIFI_RESTART Wifi state flag    
    bool settings_lkg = false;  // Settings saved as Last Known Good
    uint8_t enable_web_logging_filtering = false;
    uint8_t enable_serial_logging_filtering = false;    
    bool settings_holder_hardcorded_stored_changed = false; // if true, other files may want to reset too
  }runtime;

  #define RESET_BOOT_STATUS() memset(&pCONT_set->runtime.boot_status,0,sizeof(pCONT_set->runtime.boot_status))

  void CommandSet_SystemRestartID(uint8_t value);

  int16_t     SwitchMode_GetID_by_Name(const char* c);
  const char* SwitchMode_GetName_by_ID(uint8_t id, char* buffer, uint8_t buflen);

  #define RESPONSE_MESSAGE_BUFFER_SIZE 100
  char response_msg[RESPONSE_MESSAGE_BUFFER_SIZE] = {0};

  const char* GetTelePeriodJsonLevelCtr(char* buffer);
  const char* GetTelePeriodJsonLevelCtr(uint8_t id, char* buffer);

  


  #ifdef ENABLE_DEVFEATURE_SETTINGS__TEXT_BUFFER
  uint32_t GetSettingsTextLen(void);
  bool SettingsUpdateFinished(void);
  bool SettingsUpdateText(uint32_t index, const char* replace_me);
  char* SettingsText(uint32_t index);
  #endif // ENABLE_DEVFEATURE_SETTINGS__TEXT_BUFFER

};

#endif  // _SETTINGS_H_

