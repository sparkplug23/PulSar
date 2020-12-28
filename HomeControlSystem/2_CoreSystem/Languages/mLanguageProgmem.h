#ifndef mLanguageProgmem_H2
#define mLanguageProgmem_H2

// Take list of defines and create matched progmem list for use with arduinojson
// REMEMBER: you need to ifdef the ones not needed, as everything here uses flash memory whether used or not

#include "2_CoreSystem/mGlobalMacros.h"
#include "mLanguageDefault.h"

//A
//B
DEFINE_PGM_CTR(PM_JSON_BASE)              D_JSON_BASE;
DEFINE_PGM_CTR(PM_JSON_BAUDRATE)          D_JSON_BAUDRATE;
DEFINE_PGM_CTR(PM_JSON_BOOTCOUNT)         D_JSON_BOOTCOUNT;
DEFINE_PGM_CTR(PM_JSON_BOOTCOUNTERRORS)   D_JSON_BOOTCOUNTERRORS;
DEFINE_PGM_CTR(PM_JSON_BOOTVERSION)       D_JSON_BOOTVERSION;
DEFINE_PGM_CTR(PM_JSON_BUILDDATE)         D_JSON_BUILDDATE;
DEFINE_PGM_CTR(PM_JSON_BUILDTIME)         D_JSON_BUILDTIME;
DEFINE_PGM_CTR(PM_JSON_BUILDDATETIME)     D_JSON_BUILDDATETIME;
DEFINE_PGM_CTR(PM_JSON_BROKERHOSTNAME)    D_JSON_BROKERHOSTNAME;
DEFINE_PGM_CTR(PM_JSON_BSSID)             D_JSON_BSSID;
DEFINE_PGM_CTR(PM_JSON_BUTTONRETAIN)      D_JSON_BUTTONRETAIN;
//C
DEFINE_PGM_CTR(PM_JSON_CLASSID)           D_JSON_CLASSID;
DEFINE_PGM_CTR(PM_JSON_CONNECTS)          D_JSON_CONNECTS;
DEFINE_PGM_CTR(PM_JSON_CONNECTCOUNT)      D_JSON_CONNECTCOUNT;
DEFINE_PGM_CTR(PM_JSON_COUNT)             D_JSON_COUNT;
DEFINE_PGM_CTR(PM_JSON_COREVERSION)       D_JSON_COREVERSION;




//D
DEFINE_PGM_CTR(PM_JSON_DEVICE)            D_JSON_DEVICE;
DEFINE_PGM_CTR(PM_JSON_DEVICEID)          D_JSON_DEVICEID;
DEFINE_PGM_CTR(PM_JSON_DEVICENAME)        D_JSON_DEVICENAME;
DEFINE_PGM_CTR(PM_JSON_DEVICES_CONNECTED) D_JSON_DEVICES_CONNECTED;
DEFINE_PGM_CTR(PM_JSON_DATETIME)          D_JSON_DATETIME;
DEFINE_PGM_CTR(PM_JSON_DOWNSECS)          D_JSON_DOWNSECS;
DEFINE_PGM_CTR(PM_JSON_DOWNTIME)          D_JSON_DOWNTIME;
DEFINE_PGM_CTR(PM_JSON_DNSSERVER)         D_JSON_DNSSERVER;
DEFINE_PGM_CTR(PM_JSON_DRIVERS)           D_JSON_DRIVERS;
DEFINE_PGM_CTR(PM_JSON_DEVICEFRIENDLYNAME) D_JSON_DEVICEFRIENDLYNAME;
DEFINE_PGM_CTR(PM_JSON_DESERIALIZATION_ERROR) D_JSON_DESERIALIZATION_ERROR;

//E
DEFINE_PGM_CTR(PM_JSON_ENDDST)            D_JSON_ENDDST;
DEFINE_PGM_CTR(PM_JSON_ENABLED)           D_JSON_ENABLED;
//F
DEFINE_PGM_CTR(PM_JSON_FREEHEAP)          D_JSON_FREEHEAP;
DEFINE_PGM_CTR(PM_JSON_FREEMEMORY)        D_JSON_FREEMEMORY;   
DEFINE_PGM_CTR(PM_JSON_FREESKETCHSPACE)   D_JSON_FREESKETCHSPACE;         
DEFINE_PGM_CTR(PM_JSON_FEATURES)          D_JSON_FEATURES;
DEFINE_PGM_CTR(PM_JSON_FLASHSIZE)         D_JSON_FLASHSIZE;
DEFINE_PGM_CTR(PM_JSON_FLASHCHIPID)       D_JSON_FLASHCHIPID;
DEFINE_PGM_CTR(PM_JSON_FLASHMODE)         D_JSON_FLASHMODE;
DEFINE_PGM_CTR(PM_JSON_FRIENDLYNAME)      D_JSON_FRIENDLYNAME;
DEFINE_PGM_CTR(PM_JSON_FUNCTION)          D_JSON_FUNCTION;
//G
DEFINE_PGM_CTR(PM_JSON_GATEWAY)           D_JSON_GATEWAY;
DEFINE_PGM_CTR(PM_JSON_GPIO)              D_JSON_GPIO;
DEFINE_PGM_CTR(PM_JSON_GPIOC)              D_JSON_GPIOC;
//H
DEFINE_PGM_CTR(PM_JSON_HOSTNAME)          D_JSON_HOSTNAME;
DEFINE_PGM_CTR(PM_JSON_HEAPSIZE)          D_JSON_HEAPSIZE;

//I
DEFINE_PGM_CTR(PM_JSON_IPADDRESS)         D_JSON_IPADDRESS;
DEFINE_PGM_CTR(PM_JSON_INDEX)             D_JSON_INDEX;
//L
DEFINE_PGM_CTR(PM_JSON_LEDMASK)           D_JSON_LEDMASK;
DEFINE_PGM_CTR(PM_JSON_LEDSTATE)          D_JSON_LEDSTATE;
DEFINE_PGM_CTR(PM_JSON_LEVEL)             D_JSON_LEVEL;
DEFINE_PGM_CTR(PM_JSON_LOCAL_TIME)        D_JSON_LOCAL_TIME;
DEFINE_PGM_CTR(PM_JSON_LOOPSSEC)          D_JSON_LOOPSSEC;
DEFINE_PGM_CTR(PM_JSON_LOOPDELAY)         D_JSON_LOOPDELAY;
DEFINE_PGM_CTR(PM_JSON_LOADAVERAGE)       D_JSON_LOADAVERAGE;
DEFINE_PGM_CTR(PM_JSON_LOOPRATIO)         D_JSON_LOOPRATIO;
DEFINE_PGM_CTR(PM_JSON_LOGLEVELS)         D_JSON_LOGLEVELS;


//M
DEFINE_PGM_CTR(PM_JSON_MAC)               D_JSON_MAC;
DEFINE_PGM_CTR(PM_JSON_MDNS)              D_JSON_MDNS;
DEFINE_PGM_CTR(PM_JSON_MODULENAME)        D_JSON_MODULENAME;
DEFINE_PGM_CTR(PM_JSON_MODULEID)          D_JSON_MODULEID;
DEFINE_PGM_CTR(PM_JSON_MOTION)            D_JSON_MOTION;
DEFINE_PGM_CTR(PM_JSON_MODE)              D_JSON_MODE;


DEFINE_PGM_CTR(PM_JSON_MOTION_STARTED)    D_JSON_MOTION_STARTED;
DEFINE_PGM_CTR(PM_JSON_MOTION_ENDED)      D_JSON_MOTION_ENDED;

DEFINE_PGM_CTR(PM_JSON_MQTT)              D_JSON_MQTT;
DEFINE_PGM_CTR(PM_JSON_MQTT_ENABLE_RESTART)  D_JSON_MQTT_ENABLE_RESTART;
DEFINE_PGM_CTR(PM_JSON_MESSAGE)           D_JSON_MESSAGE;


//N
DEFINE_PGM_CTR(PM_JSON_NETWORK)           D_JSON_NETWORK;
DEFINE_PGM_CTR(PM_JSON_NAME)              D_JSON_NAME;
//O
DEFINE_PGM_CTR(PM_JSON_OTAURL)            D_JSON_OTAURL;
DEFINE_PGM_CTR(PM_JSON_ORDER)             D_JSON_ORDER;
DEFINE_PGM_CTR(PM_JSON_ONOFF)             D_JSON_ONOFF;
DEFINE_PGM_CTR(PM_JSON_ONOFF_NAME)        D_JSON_ONOFF_NAME;
DEFINE_PGM_CTR(PM_JSON_LAST)              D_JSON_LAST;
DEFINE_PGM_CTR(PM_JSON_ONTIME)            D_JSON_ONTIME;
DEFINE_PGM_CTR(PM_JSON_OFFTIME)           D_JSON_OFFTIME;



//P
DEFINE_PGM_CTR(PM_JSON_PACKETS)           D_JSON_PACKETS;
DEFINE_PGM_CTR(PM_JSON_POWER)             D_JSON_POWER;
DEFINE_PGM_CTR(PM_JSON_POWERONSTATE)      D_JSON_POWERONSTATE;
DEFINE_PGM_CTR(PM_JSON_POWERRETAIN)       D_JSON_POWERRETAIN;
DEFINE_PGM_CTR(PM_JSON_PROGRAMSIZE)       D_JSON_PROGRAMSIZE;
DEFINE_PGM_CTR(PM_JSON_PROGRAMFLASHSIZE)  D_JSON_PROGRAMFLASHSIZE;



//Q
//R
DEFINE_PGM_CTR(PM_JSON_RELAY_NAME)        D_JSON_RELAY_NAME;
DEFINE_PGM_CTR(PM_JSON_RSSI)              D_JSON_RSSI;
DEFINE_PGM_CTR(PM_JSON_RECEIVEDCOUNT)     D_JSON_RECEIVEDCOUNT;


DEFINE_PGM_CTR(PM_JSON_RATE)              D_JSON_RATE;
DEFINE_PGM_CTR(PM_JSON_RATE_SECS)         D_JSON_RATE_SECS;
DEFINE_PGM_CTR(PM_JSON_RATE_MS)           D_JSON_RATE_MS;

//S
DEFINE_PGM_CTR(PM_JSON_SAVEADDRESS)       D_JSON_SAVEADDRESS;
DEFINE_PGM_CTR(PM_JSON_SAVEDATA)          D_JSON_SAVEDATA;
DEFINE_PGM_CTR(PM_JSON_SAVESTATE)         D_JSON_SAVESTATE;
DEFINE_PGM_CTR(PM_JSON_SAVECOUNT)         D_JSON_SAVECOUNT;
DEFINE_PGM_CTR(PM_JSON_SENTCOUNT)         D_JSON_SENTCOUNT;
DEFINE_PGM_CTR(PM_JSON_SENTPERMINUTE)     D_JSON_SENTPERMINUTE;
DEFINE_PGM_CTR(PM_JSON_SENSORRETAIN)      D_JSON_SENSORRETAIN;
DEFINE_PGM_CTR(PM_JSON_SENSORADDRESS)     D_JSON_SENSORADDRESS;
DEFINE_PGM_CTR(PM_JSON_SDKVERSION)        D_JSON_SDKVERSION;
DEFINE_PGM_CTR(PM_JSON_SENSORS)           D_JSON_SENSORS;
DEFINE_PGM_CTR(PM_JSON_SENSORCOUNT)       D_JSON_SENSORCOUNT;
DEFINE_PGM_CTR(PM_JSON_SETTINGS_HOLDER)   D_JSON_SETTINGS_HOLDER;
DEFINE_PGM_CTR(PM_JSON_SIZE)              D_JSON_SIZE;
DEFINE_PGM_CTR(PM_JSON_SLEEP)             D_JSON_SLEEP;
DEFINE_PGM_CTR(PM_JSON_SLEEPMODE)         D_JSON_SLEEPMODE;
DEFINE_PGM_CTR(PM_JSON_SUNRISE)           D_JSON_SUNRISE;
DEFINE_PGM_CTR(PM_JSON_SUNSET)            D_JSON_SUNSET;
DEFINE_PGM_CTR(PM_JSON_SSID)              D_JSON_SSID;
DEFINE_PGM_CTR(PM_JSON_SSID_NUMBERED)     D_JSON_SSID_NUMBERED;
DEFINE_PGM_CTR(PM_JSON_STARTDST)          D_JSON_STARTDST;
DEFINE_PGM_CTR(PM_JSON_STARTUPUTC)        D_JSON_STARTUPUTC;
DEFINE_PGM_CTR(PM_JSON_STATUS)            D_JSON_STATUS;
DEFINE_PGM_CTR(PM_JSON_STATIC_IPADDRESS)  D_JSON_STATIC_IPADDRESS;
DEFINE_PGM_CTR(PM_JSON_SUBNETMASK)        D_JSON_SUBNETMASK;
DEFINE_PGM_CTR(PM_JSON_SERIAL)            D_JSON_SERIAL;
DEFINE_PGM_CTR(PM_JSON_SWITCHMODE)        D_JSON_SWITCHMODE;
DEFINE_PGM_CTR(PM_JSON_SWITCHRETAIN)      D_JSON_SWITCHRETAIN;
DEFINE_PGM_CTR(PM_JSON_SYSTEM)            D_JSON_SYSTEM;






//T
DEFINE_PGM_CTR(PM_JSON_TEMPLATE_USED)     D_JSON_TEMPLATE_USED;
DEFINE_PGM_CTR(PM_JSON_TEMPLATE_PARSE_SUCCESS) D_JSON_TEMPLATE_PARSE_SUCCESS;
DEFINE_PGM_CTR(PM_JSON_TIMEZONE)          D_JSON_TIMEZONE;
DEFINE_PGM_CTR(PM_JSON_TELNET)            D_JSON_TELNET;
DEFINE_PGM_CTR(PM_JSON_TEMPLATE)          D_JSON_TEMPLATE;
DEFINE_PGM_CTR(PM_JSON_TIME)              D_JSON_TIME;
DEFINE_PGM_CTR(PM_JSON_TIME_SECS)         D_JSON_TIME_SECS;
DEFINE_PGM_CTR(PM_JSON_TIME_MS)           D_JSON_TIME_MS;
DEFINE_PGM_CTR(PM_JSON_TIME_ON)              D_JSON_TIME_ON;
DEFINE_PGM_CTR(PM_JSON_TIME_ON_SECS)         D_JSON_TIME_ON_SECS;
DEFINE_PGM_CTR(PM_JSON_TIME_ON_MS)           D_JSON_TIME_ON_MS;
DEFINE_PGM_CTR(PM_JSON_TYPE)              D_JSON_TYPE;






//U
DEFINE_PGM_CTR(PM_JSON_UTC_TIME)          D_JSON_UTC_TIME;
DEFINE_PGM_CTR(PM_JSON_UPSECONDS)         D_JSON_UPSECONDS;
DEFINE_PGM_CTR(PM_JSON_UPTIME)            D_JSON_UPTIME;
//V
DEFINE_PGM_CTR(PM_JSON_VERSION)           D_JSON_VERSION;
DEFINE_PGM_CTR(PM_JSON_VERSIONNAME)       D_JSON_VERSIONNAME;
DEFINE_PGM_CTR(PM_JSON_VERSIONNUMBER)     D_JSON_VERSIONNUMBER;
//W
DEFINE_PGM_CTR(PM_JSON_WEBSERVER_ENABLED)   D_JSON_WEBSERVER_ENABLED;
DEFINE_PGM_CTR(PM_JSON_WIFICONFIG_STATE)    D_JSON_WIFICONFIG_STATE;
DEFINE_PGM_CTR(PM_JSON_WEB)                 D_JSON_WEB;



DEFINE_PGM_CTR(WARNING_NOTHANDLED_CTR) "NOT_HANDLED";

DEFINE_PGM_CTR(PM_SEARCH_NOMATCH)   "No Match";



/*************************************************************************************************************************************************************
***********************************************************************************************************************************************************
***********************************************************************************************************************************************************
***********************************************************************************************************************************************************
************* ABOVE will always be included **********************************************************************************************************************************************
***********************************************************************************************************************************************************
***********************************************************************************************************************************************************
***********************************************************************************************************************************************************
***********************************************************************************************************************************************************
************** BELOW will only be included if ifdef exists *********************************************************************************************************************************************
***********************************************************************************************************************************************************
***********************************************************************************************************************************************************
********************************************************************************************************************************************************/


#ifdef USE_MODULE_LIGHTS_INTERFACE

DEFINE_PGM_CTR(PM_JSON_ANIMATIONENABLE)    D_JSON_ANIMATIONENABLE;
DEFINE_PGM_CTR(PM_JSON_ANIMATIONMODE)      D_JSON_ANIMATIONMODE;

//B
DEFINE_PGM_CTR(PM_JSON_BRIGHTNESS_MAX)           D_JSON_BRIGHTNESS_MAX;
DEFINE_PGM_CTR(PM_JSON_BRIGHTNESS_MIN)           D_JSON_BRIGHTNESS_MIN;
DEFINE_PGM_CTR(PM_JSON_BRIGHTNESS)               D_JSON_BRIGHTNESS;
DEFINE_PGM_CTR(PM_JSON_BRIGHTNESS_255)           D_JSON_BRIGHTNESS_255;
DEFINE_PGM_CTR(PM_JSON_BRIGHTNESS_RGB)           D_JSON_BRIGHTNESS_RGB;
DEFINE_PGM_CTR(PM_JSON_BRIGHTNESS_RGB_255)       D_JSON_BRIGHTNESS_RGB "_255";
DEFINE_PGM_CTR(PM_JSON_BRIGHTNESS_CCT)           D_JSON_BRIGHTNESS_CCT;
DEFINE_PGM_CTR(PM_JSON_BRIGHTNESS_CCT_255)       D_JSON_BRIGHTNESS_CCT "_255";
//C
DEFINE_PGM_CTR(PM_JSON_COLOUR_PALETTE)      D_JSON_COLOUR_PALETTE;
DEFINE_PGM_CTR(PM_JSON_COLOUR)      D_JSON_COLOUR;


//H
DEFINE_PGM_CTR(PM_JSON_HARDWARE_TYPE)           D_JSON_HARDWARE_TYPE;
DEFINE_PGM_CTR(PM_JSON_HSB)                 D_JSON_HSB;
DEFINE_PGM_CTR(PM_JSON_HUE)                 D_JSON_HUE;
//L
DEFINE_PGM_CTR (PM_JSON_LIGHTPOWER) D_JSON_LIGHTPOWER;
//P
DEFINE_PGM_CTR(PM_JSON_PIXELS_UPDATE_NUMBER)        D_JSON_PIXELS_UPDATE_NUMBER;
DEFINE_PGM_CTR(PM_JSON_PIXELS_UPDATE_PERCENTAGE)        D_JSON_PIXELS_UPDATE_PERCENTAGE;
//R
DEFINE_PGM_CTR(PM_JSON_RGB_COLOUR_ORDER)                    D_JSON_RGB_COLOUR_ORDER;

DEFINE_PGM_CTR(PM_JSON_CCT_PERCENTAGE)      D_JSON_CCT_PERCENTAGE;
DEFINE_PGM_CTR(PM_JSON_CCT_TEMP)            D_JSON_CCT_TEMP;
DEFINE_PGM_CTR(PM_JSON_RGBCCT_LINKED)       D_JSON_RGBCCT_LINKED;
DEFINE_PGM_CTR(PM_JSON_SCENE_COLOUR)        D_JSON_SCENE_COLOUR;
DEFINE_PGM_CTR(PM_JSON_SCENE_NAME)        D_JSON_SCENE_NAME;
DEFINE_PGM_CTR(PM_JSON_SAT)        D_JSON_SAT;
DEFINE_PGM_CTR(PM_JSON_SAT_255)        D_JSON_SAT "_255";
DEFINE_PGM_CTR(PM_JSON_STRIP_SIZE)  D_JSON_STRIP_SIZE;
DEFINE_PGM_CTR(PM_JSON_SCENE) D_JSON_SCENE;

DEFINE_PGM_CTR(PM_JSON_TRANSITION)        D_JSON_TRANSITION;


#endif //USE_MODULE_LIGHTS_INTERFACE



#ifdef USE_MODULE_LIGHTS_ADDRESSABLE
//A
DEFINE_PGM_CTR(PM_JSON_AGED_COLOURING)    D_JSON_AGED_COLOURING;
DEFINE_PGM_CTR(PM_JSON_ALTERNATE)          D_JSON_ALTERNATE;
DEFINE_PGM_CTR(PM_JSON_ALTERNATE_AMOUNT)   D_JSON_ALTERNATE_AMOUNT;
DEFINE_PGM_CTR(PM_JSON_ALTERNATE_BRIGHTNESS_MAX)   D_JSON_ALTERNATE_BRIGHTNESS_MAX;
DEFINE_PGM_CTR(PM_JSON_ALTERNATE_BRIGHTNESS_MIN)   D_JSON_ALTERNATE_BRIGHTNESS_MIN;
//B
//D
DEFINE_PGM_CTR(PM_RGB_DATA_STREAM)          D_RGB_DATA_STREAM;
DEFINE_PGM_CTR(PM_JSON_DIRECTION)           D_JSON_DIRECTION;
//F
DEFINE_PGM_CTR(PM_JSON_FLASHER)            D_JSON_FLASHER;
//M
DEFINE_PGM_CTR(PM_JSON_MIXER)           D_JSON_MIXER;
DEFINE_PGM_CTR(PM_JSON_MULTIPLIER)     D_JSON_MULTIPLIER;
DEFINE_PGM_CTR(PM_JSON_MAPPED_MULTIPLIER_DATA)     D_JSON_MAPPED_MULTIPLIER_DATA;
//P
DEFINE_PGM_CTR(PM_JSON_PIXELGROUPING)     D_JSON_PIXELGROUPING;
//R
DEFINE_PGM_CTR (PM_JSON_COLOUR_REFRESH_RATE)                D_JSON_COLOUR_REFRESH_RATE;
DEFINE_PGM_CTR(PM_JSON_RUNTIME_DURATION_SCALER_PERCENTAGE)  D_JSON_RUNTIME_DURATION_SCALER_PERCENTAGE;
DEFINE_PGM_CTR(PM_JSON_RUNNING_ID)                          D_JSON_RUNNING_ID;
//S
DEFINE_PGM_CTR(PM_JSON_MANUAL_SETPIXEL_TO_SCENE) D_JSON_MANUAL_SETPIXEL_TO_SCENE;
//T
DEFINE_PGM_CTR (PM_JSON_TIME_SCALER_AS_PERCENTAGE) D_JSON_TIME_SCALER_AS_PERCENTAGE;

#endif //USE_MODULE_LIGHTS_ADDRESSABLE



#ifdef USE_MODULE_DRIVERS_RELAY
DEFINE_PGM_CTR(PM_JSON_POWERNAME)         D_JSON_POWERNAME;
DEFINE_PGM_CTR(PM_JSON_RELAY)             D_JSON_RELAY;
#endif //USE_MODULE_DRIVERS_RELAY

#ifdef USE_MODULE_SENSORS_BUTTONS
DEFINE_PGM_CTR(PM_PRESSED)                D_PRESSED;
DEFINE_PGM_CTR(PM_NOT_PRESSED)                D_NOT_PRESSED;
#endif // USE_MODULE_SENSORS_BUTTONS



// Common
enum UnitNames {
  UNIT_AMPERE,
  UNIT_HOUR,
  UNIT_KILOOHM,
  UNIT_KILOWATTHOUR,
  UNIT_LUX,
  UNIT_MICROSECOND,
  UNIT_MILLIAMPERE,
  UNIT_MILLIMETER_MERCURY,
  UNIT_MILLISECOND,
  UNIT_MINUTE,
  UNIT_PPB,
  UNIT_PPD,
  UNIT_PPM,
  UNIT_PERCENTAGE,
  UNIT_PRESSURE,
  UNIT_SECOND,
  UNIT_SECTORS,
  UNIT_VOLT,
  UNIT_WATT,
  UNIT_WATTHOUR,
  UNIT_HERTZ };
const char kUnitNames[] PROGMEM =
  D_UNIT_AMPERE "|"
  D_UNIT_HOUR "|"
  D_UNIT_KILOOHM "|"
  D_UNIT_KILOWATTHOUR "|"
  D_UNIT_LUX "|"
  D_UNIT_MICROSECOND "|"
  D_UNIT_MILLIAMPERE "|"
  D_UNIT_MILLIMETER_MERCURY "|"
  D_UNIT_MILLISECOND "|"
  D_UNIT_MINUTE "|"
  D_UNIT_PARTS_PER_BILLION "|"
  D_UNIT_PARTS_PER_DECILITER "|"
  D_UNIT_PARTS_PER_MILLION "|"
  "%|"
  D_UNIT_PRESSURE "|"
  D_UNIT_SECOND "|"
  D_UNIT_SECTORS "|"
  D_UNIT_VOLT "|"
  D_UNIT_WATT "|"
  D_UNIT_WATTHOUR "|"
  D_UNIT_HERTZ ;

const char S_JSON_COMMAND_NVALUE_SPACE_UNIT[] PROGMEM =       "{\"%s\":\"%d %s\"}";
const char S_JSON_COMMAND_LVALUE_SPACE_UNIT[] PROGMEM =       "{\"%s\":\"%lu %s\"}";
const char S_JSON_COMMAND_SVALUE_SPACE_UNIT[] PROGMEM =       "{\"%s\":\"%s %s\"}";
const char S_JSON_COMMAND_NVALUE_UNIT[] PROGMEM =             "{\"%s\":\"%d%s\"}";
const char S_JSON_COMMAND_NVALUE_UNIT_NVALUE_UNIT[] PROGMEM = "{\"%s\":\"%d%s (%d%s)\"}";

const char S_JSON_COMMAND_NVALUE_SVALUE[] PROGMEM =           "{\"%s\":\"%d (%s)\"}";
const char S_JSON_COMMAND_NVALUE_ACTIVE_NVALUE[] PROGMEM =    "{\"%s\":\"%d (" D_JSON_ACTIVE " %d)\"}";

const char S_JSON_COMMAND_NVALUE[] PROGMEM =                  "{\"%s\":%d}";
#define    D_JSON_COMMAND_NVALUE                              "{\"%s\":%d}"
#define    D_JSON_COMMAND_NVALUE_P                            "{\"%S\":%d}"
#define    D_JSON_COMMAND_NVALUE_K(KEY)                       "{\"" KEY "\":%d}"

#define    D_JSON_COMMAND_FVALUE                              "{\"%s\":%.1f}"
const char S_JSON_COMMAND_LVALUE[] PROGMEM =                  "{\"%s\":%lu}";
const char S_JSON_COMMAND_FVALUE[] PROGMEM =                  "{\"%s\":%.1f}";
const char S_JSON_COMMAND_SVALUE[] PROGMEM =                  "{\"%s\":\"%s\"}";
#define    D_JSON_COMMAND_SVALUE                              "{\"%s\":\"%s\"}"
#define    D_JSON_COMMAND_SVALUE_P                            "{\"%S\":\"%s\"}"


#define    D_JSON_COMMAND_SVALUE_K(KEY)                       "{\"" KEY ":\"%s\"}"

// #define    D_JSON_COMMAND_SVALUE_K1                       "{\""
// #define    D_JSON_COMMAND_SVALUE_K2                       ":\"%s\"}"


const char S_JSON_COMMAND_SVALUE_SVALUE[] PROGMEM =           "{\"%s\":{\"%s\":\"%s\"}}";
#define    D_JSON_COMMAND_SVALUE_SVALUE                       "{\"%s\":{\"%s\":\"%s\"}}"
#define    D_JSON_COMMAND_SVALUE_SVALUE_K(KEY1, KEY2)         "{\"" KEY1 "\":{\"" KEY2 "\":\"%s\"}}"

const char S_JSON_COMMAND_SVALUE_NVALUE[] PROGMEM =           "{\"%s\":{\"%s\":%d}}";
#define    D_JSON_COMMAND_SVALUE_NVALUE                       "{\"%s\":{\"%s\":%d}}"
#define    D_JSON_COMMAND_SVALUE_NVALUE_K(KEY1, KEY2)         "{\"" KEY1 "\":{\"" KEY2 "\":%d}}"



const char S_JSON_COMMAND_ASTERIX[] PROGMEM =                 "{\"%s\":\"" D_ASTERIX "\"}";
const char S_JSON_COMMAND_XVALUE[] PROGMEM =                  "{\"%s\":%s}";  // %s must provide quotes on non-number


#define    D_JSON_KEYVALUE_NVALUE                             "\"%s\":%d"
DEFINE_PGM_CTR(PM_JSON_KEYVALUE_NVALUE)                   D_JSON_KEYVALUE_NVALUE;
#define    D_JSON_KEYVALUE_SVALUE                             "\"%s\":\"%s\""
DEFINE_PGM_CTR(PM_JSON_KEYVALUE_SVALUE)                   D_JSON_KEYVALUE_SVALUE;
#define    D_JSON_KEYVALUE_FVALUE                             "\"%s\":%s"
DEFINE_PGM_CTR(PM_JSON_KEYVALUE_FVALUE)                   D_JSON_KEYVALUE_FVALUE;



#define    D_FUNCTION_NAME_SVALUE                              "f::\"%s\""

const char S_JSON_COMMAND_INDEX_NVALUE[] PROGMEM =            "{\"%s%d\":%d}";
const char S_JSON_COMMAND_INDEX_LVALUE[] PROGMEM =            "{\"%s%d\":%lu}";
const char S_JSON_COMMAND_INDEX_SVALUE[] PROGMEM =            "{\"%s%d\":\"%s\"}";
const char S_JSON_COMMAND_INDEX_ASTERIX[] PROGMEM =           "{\"%s%d\":\"" D_ASTERIX "\"}";
const char S_JSON_COMMAND_INDEX_SVALUE_SVALUE[] PROGMEM =     "{\"%s%d\":\"%s%s\"}";
const char S_JSON_COMMAND_INDEX_NVALUE_ACTIVE_NVALUE[] PROGMEM = "{\"%s%d\":\"%d (" D_JSON_ACTIVE " %d)\"}";

const char S_JSON_SENSOR_INDEX_NVALUE[] PROGMEM =             "{\"" D_JSON_SENSOR "%d\":%d}";
const char S_JSON_SENSOR_INDEX_SVALUE[] PROGMEM =             "{\"" D_JSON_SENSOR "%d\":\"%s\"}";

const char S_JSON_DRIVER_INDEX_NVALUE[] PROGMEM =             "{\"" D_JSON_DRIVER "%d\":%d}";
const char S_JSON_DRIVER_INDEX_SVALUE[] PROGMEM =             "{\"" D_JSON_DRIVER "%d\":\"%s\"}";

const char JSON_SNS_TEMP[] PROGMEM = ",\"%s\":{\"" D_JSON_TEMPERATURE "\":%s}";
const char JSON_SNS_TEMPHUM[] PROGMEM = ",\"%s\":{\"" D_JSON_TEMPERATURE "\":%s,\"" D_JSON_HUMIDITY "\":%s}";

const char JSON_SNS_GNGPM[] PROGMEM = ",\"%s\":{\"" D_JSON_TOTAL_USAGE "\":%s,\"" D_JSON_FLOWRATE "\":%s}";

const char S_LOG_I2C_FOUND_AT[] PROGMEM = D_LOG_I2C "%s " D_FOUND_AT " 0x%x";

const char S_LOG_HTTP[] PROGMEM = D_LOG_HTTP;
const char S_LOG_WIFI[] PROGMEM = D_LOG_WIFI;
const char S_LOG_MQTT[] PROGMEM = D_LOG_MQTT;
const char S_RSLT_POWER[] PROGMEM = D_RSLT_POWER;
const char S_RSLT_RESULT[] PROGMEM = D_RSLT_RESULT;
const char S_RSLT_WARNING[] PROGMEM = D_RSLT_WARNING;
const char S_LWT[] PROGMEM = D_LWT;
const char S_OFFLINE[] PROGMEM = D_OFFLINE;

// sonoff.ino
// #define MAX_BUTTON_COMMANDS  5  // Max number of button commands supported
// const char kCommands[MAX_BUTTON_COMMANDS][14] PROGMEM = {
//   D_JSON_WIFICONFIG " 1",   // Press button three times
//   D_JSON_WIFICONFIG " 2",   // Press button four times
//   D_JSON_WIFICONFIG " 3",   // Press button five times
//   D_JSON_RESTART " 1",      // Press button six times
//   D_JSON_UPGRADE " 1" };    // Press button seven times
// #ifdef ESP8266
// const char kWifiConfig[MAX_WIFI_OPTION][WCFG_MAX_STRING_LENGTH] PROGMEM = {
//   D_WCFG_0_RESTART,
//   D_WCFG_1_SMARTCONFIG,
//   D_WCFG_2_WIFIMANAGER,
//   D_WCFG_3_WPSCONFIG,
//   D_WCFG_4_RETRY,
//   D_WCFG_5_WAIT,
//   D_WCFG_6_SERIAL,
//   D_WCFG_7_WIFIMANAGER_RESET_ONLY };
//   #endif
// const char kPrefixes[3][PRFX_MAX_STRING_LENGTH] PROGMEM = {
//   D_CMND,
//   D_STAT,
//   D_TELE };



// support.ino
static const char kMonthNames[] PROGMEM = D_MONTH3LIST;

DEFINE_PGM_CTR(kOptionOff)        "OFF|" "0|" D_OFF "|" D_FALSE "|" D_STOP "|" D_JSON_OFF ;
DEFINE_PGM_CTR(kOptionOn)         "ON|"  "1|" D_ON "|" D_TRUE "|" D_START "|" D_JSON_ON "|" D_USER ;
DEFINE_PGM_CTR(kOptionToggle)     "TOGGLE|" "2|" D_TOGGLE "|" D_ADMIN ;
DEFINE_PGM_CTR(kOptionBlink)      "BLINK|" D_BLINK ;
DEFINE_PGM_CTR(kOptionBlinkOff)   "BLINKOFF|" D_BLINKOFF ;

// xdrv_02_webserver.ino
// #ifdef USE_MODULE_CORE_WEBSERVER
// const char HTTP_SNS_TEMP[] PROGMEM = "{s}%s " D_TEMPERATURE "{m}%s&deg;%c{e}";                    // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
// const char HTTP_SNS_HUM[] PROGMEM = "{s}%s " D_HUMIDITY "{m}%s%%{e}";                             // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
// const char HTTP_SNS_PRESSURE[] PROGMEM = "{s}%s " D_PRESSURE "{m}%s %s{e}";                       // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
// const char HTTP_SNS_SEAPRESSURE[] PROGMEM = "{s}%s " D_PRESSUREATSEALEVEL "{m}%s %s{e}";          // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
// const char HTTP_SNS_ANALOG[] PROGMEM = "{s}%s " D_ANALOG_INPUT "%d{m}%d{e}";                      // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
// const char HTTP_SNS_ILLUMINANCE[] PROGMEM = "{s}%s " D_ILLUMINANCE "{m}%d " D_UNIT_LUX "{e}";     // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
// const char HTTP_SNS_CO2[] PROGMEM = "{s}%s " D_CO2 "{m}%d " D_UNIT_PARTS_PER_MILLION "{e}";       // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
// const char HTTP_SNS_CO2EAVG[] PROGMEM = "{s}%s " D_ECO2 "{m}%d " D_UNIT_PARTS_PER_MILLION "{e}";  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
// const char HTTP_SNS_GALLONS[] PROGMEM = "{s}%s " D_TOTAL_USAGE "{m}%s " D_UNIT_GALLONS " {e}";    // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
// const char HTTP_SNS_GPM[] PROGMEM = "{s}%s " D_FLOW_RATE "{m}%s " D_UNIT_GALLONS_PER_MIN" {e}";   // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
// const char HTTP_SNS_DISTANCE[] PROGMEM = "{s}%s " "Distance" "{m}%s%s{e}";                             // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>

// const char HTTP_SNS_GENERIC[] PROGMEM = "{s}%s{m}%s{e}";
// const char HTTP_SNS_GENERIC_NVALUE[] PROGMEM = "{s}%s{m}%d{e}";
// const char HTTP_SNS_GENERIC_SVALUE[] PROGMEM = "{s}%s{m}%s{e}";
// const char HTTP_SNS_GENERIC_UNIT[] PROGMEM = "{s}%s{m}%s %s{e}";
// const char HTTP_SNS_GENERIC_NVALUE_UNIT[] PROGMEM = "{s}%s{m}%d %s{e}";
// const char HTTP_SNS_GENERIC_SVALUE_UNIT[] PROGMEM = "{s}%s{m}%s %s{e}";
 
// const char HTTP_SNS_VOLTAGE[] PROGMEM = "{s}" D_VOLTAGE "{m}%s " D_UNIT_VOLT "{e}";
// const char HTTP_SNS_CURRENT[] PROGMEM = "{s}" D_CURRENT "{m}%s " D_UNIT_AMPERE "{e}";
// const char HTTP_SNS_POWER[] PROGMEM = "{s}" D_POWERUSAGE "{m}%s " D_UNIT_WATT "{e}";
// const char HTTP_SNS_ENERGY_TOTAL[] PROGMEM = "{s}" D_ENERGY_TOTAL "{m}%s " D_UNIT_KILOWATTHOUR "{e}";


const char S_MAIN_MENU[] PROGMEM = D_MAIN_MENU;
const char S_CONFIGURATION[] PROGMEM = D_CONFIGURATION;
const char S_SYSTEM_SETTINGS[] PROGMEM = D_SYSTEM_SETTINGS;
const char S_CONFIGURE_TEMPLATE[] PROGMEM = D_CONFIGURE_TEMPLATE;
const char S_CONFIGURE_MODULE[] PROGMEM = D_CONFIGURE_MODULE;
const char S_COLOUR_PALETTE_EDITOR[] PROGMEM = D_COLOUR_PALETTE_EDITOR;
const char S_COLOUR_MIXER_EDITOR[] PROGMEM = D_COLOUR_MIXER_EDITOR;
const char S_CONFIGURE_WIFI[] PROGMEM = D_CONFIGURE_WIFI;
const char S_NO_NETWORKS_FOUND[] PROGMEM = D_NO_NETWORKS_FOUND;
const char S_CONFIGURE_LOGGING[] PROGMEM = D_CONFIGURE_LOGGING;
const char S_CONFIGURE_OTHER[] PROGMEM = D_CONFIGURE_OTHER;
const char S_SAVE_CONFIGURATION[] PROGMEM = D_SAVE_CONFIGURATION;
const char S_RESET_CONFIGURATION[] PROGMEM = D_RESET_CONFIGURATION;
const char S_RESTORE_CONFIGURATION[] PROGMEM = D_RESTORE_CONFIGURATION;
const char S_FIRMWARE_UPGRADE[] PROGMEM = D_FIRMWARE_UPGRADE;
const char S_CONSOLE[] PROGMEM = D_CONSOLE;
const char PM_INFORMATION[] PROGMEM = D_INFORMATION;
const char S_RESTART[] PROGMEM = D_RESTART;
// #endif  // USE_MODULE_CORE_WEBSERVER



#endif