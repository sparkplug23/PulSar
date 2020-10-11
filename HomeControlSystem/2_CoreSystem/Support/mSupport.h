//#ifdef ESP8266
#ifndef _MSUPPORT_H_
#define _MSUPPORT_H_


#include "0_ConfigUser/mUserConfig.h"

//move to time lib
// typedef struct TIMEREACHED_SAVED{
//   uint32_t millis_saved;
//   uint8_t RunNow; // send early
// }timereached_t;

enum FUNCHANDLER_TIME_IDS{
  FUNCHANDLER_TIME_MS_ID=0,
  FUNCHANDLER_TIME_SECS_ID=1,
  FUNCHANDLER_TIME_MINS_ID=2,
};

enum STATE_NUMBER_IDS{
  STATE_NUMBER_INVALID_ID=-1,
  STATE_NUMBER_OFF_ID=0,
  STATE_NUMBER_ON_ID,
  STATE_NUMBER_TOGGLE_ID,
  STATE_NUMBER_BLINK_ID,
  STATE_NUMBER_BLINK_OFF_ID,
  STATE_NUMBER_LENGTH_ID
};


#define P_PHASE_OUT() Serial.println(F("PHASE OUT"));


#define CALL_VOID_FUNCTION(object,ptrToMember)  ((object).*(ptrToMember))

// 8 bit packed flag 
typedef union {
  uint8_t flags;
  struct {
    uint8_t run_now   : 1;
    // 0 - secs, 1 - ms, 2 - hours
    uint8_t time_unit : 2;   
    uint8_t periodic_enabled   : 1;
    uint8_t run_always   : 1; //ie loops
    uint8_t reserved  : 4;     
  };
}FUNCTION_HANDLER_FLAGS;


template <typename Class>
struct functionhandler {
  uint32_t      saved_millis = 0;
  uint16_t      time_val = 1;
  uint8_t       module_id = 0;
  void          (Class::*function)(); // member-function to sender with one args
  FUNCTION_HANDLER_FLAGS flags;
};


#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

#ifdef ESP8266
#define AGPIO(x) (x)
#else  // ESP32
#define AGPIO(x) (x<<5)
#endif  // ESP8266 - ESP32

#ifdef ESP32
  #include <WiFi.h>
#endif
#ifdef ESP8266
  #include <ESP8266WiFi.h>            // Wifi, MQTT, Ota, WifiManager
  #include <ESP8266httpUpdate.h>
#endif

#include <ArduinoJson.h>
#include "1_TaskerManager/mInterfaceController.h"
#include "2_CoreSystem/Languages/mLanguage.h"
#include "2_CoreSystem/mHardwareTemplates.h"
#include "2_CoreSystem/Logging/mLogging.h"
#include "2_CoreSystem/Time/mTime.h"
#include "2_CoreSystem/Settings/mSettings.h"

extern "C" {
extern struct rst_info resetInfo;
}




// Returns value if its between values, if not, returns midpoint
template<typename T>
T WithinLimits(T lower, T value, T upper){
  // returns value if its between limits
  if((value>lower)&&(value<upper)){
    return value;
  }
  // return midpoint if above was not true
  else{
    return ((upper-lower)/2)+value;
  }
}

template<typename T>
T min3(T a, T b, T c) {
  return (a < b && a < c) ? a : (b < c) ? b : c;
}

template<typename T>
bool SetAndKeepHighestNumber(T* current, T new_value){
  if(new_value > *current){
    *current = new_value;
    return true; // true is changed
  }
  return false;
} 


extern "C" void custom_crash_callback(struct rst_info * rst_info, uint32_t stack, uint32_t stack_end);

/*********************************************************************************************\
 * Watchdog extension (https://github.com/esp8266/Arduino/issues/1532)
\*********************************************************************************************/

// // Global functions
// void AppendJSON_Start();

// template<typename KEY, typename VALUE>
// void AppendJSON_Value(KEY key, VALUE value);

// class mInterfaceController;
    
// template<typename KEY, typename VALUE>
// void AppendJSON_Value(KEY key, VALUE value)
// // template<typename VALUE>
// // void mSupport::AppendJSON_Value(const char* key, VALUE value)
// {

  
//   char* buff = data_buffer2.payload.ctr;
//   uint16_t* len = &data_buffer2.payload.len;

//   *len = sprintf(&buff[*len],"%s","AppendJSON_Value");

//   // va_list arg;
//   // va_start(arg, formatP);
//   // *length += vsnprintf_P(&buffer[*length], DATA_BUFFER_PAYLOAD_MAX_LENGTH, formatP, arg);
//   // va_end(arg);
//   // return;
// }

    const uint32_t crash_magic = 0x53415400;   // Stack trace magic number (TASx)
    const uint32_t crash_rtc_offset = 32;      // Offset in RTC memory skipping OTA used block
    const uint32_t crash_dump_max_len = 31;    // Dump only 31 call addresses to satisfy max JSON length of about 600 characters


#include <Ticker.h>




class mSupport{
  public:
    mSupport(){};
    
    int8_t Tasker(uint8_t function);
    void init(void);

    /****
     * "class-less" Pointer Member function that takes the struct handler which contains if/when a mqtt payload should
     * be sent. If a payload should sent, the formatted publish function is called.
     * */
    template<typename T>
    void FunctionHandler_Call(T& class_ptr, uint8_t class_id, functionhandler<T>* handler_ptr)
    {
      if(handler_ptr == nullptr){
        return;
      }

      uint16_t time_multiplier = 0;
      switch(handler_ptr->flags.time_unit){
        default:
        case FUNCHANDLER_TIME_MS_ID:   time_multiplier = 1;     break;
        case FUNCHANDLER_TIME_SECS_ID: time_multiplier = 1000;  break;
        case FUNCHANDLER_TIME_MINS_ID: time_multiplier = 60000; break;
      }

      if(handler_ptr->flags.periodic_enabled){
        if(abs(millis()-handler_ptr->saved_millis)>=handler_ptr->time_val*time_multiplier){ handler_ptr->saved_millis=millis();
          handler_ptr->flags.run_now = true;
        }
      }
      if(handler_ptr->flags.run_now || handler_ptr->flags.run_always){ handler_ptr->flags.run_now = false;    
        handler_ptr->saved_millis = millis();    
        // (class_ptr.*handler_ptr->function)(); //manual
        CALL_VOID_FUNCTION(class_ptr,handler_ptr->function)();
      }
    };

    // JsonObject& jsonobject_parsing = nullptr;

    float FastPrecisePowf(const float x, const float y);

    void parse_JSONCommand();
    IPAddress syslog_host_addr;      // Syslog host IP address
    uint32_t syslog_host_hash = 0;   // Syslog host name hash

    uint32_t state_100msecond,state_250msecond;

    Ticker tickerOSWatch;

char* GetTextIndexedTemp(char* destination, size_t destination_size, uint16_t index, const char* haystack);


bool JsonLevelFlagCheck(uint8_t json_level_testing, uint8_t json_level_set, uint8_t ischanged = false);

void AppendDList(char* buffer, const char* to_add);
void AppendDList(char* buffer, uint16_t buflen, const char* formatP, ...);

    uint32_t tSaved_SlowAllTemplatesOnSerial;
    void SlowAllTemplatesOnSerial();


    struct LOOP_PERFORMANCE{
      uint32_t loop_counter=0;
      uint32_t cycles_per_sec=0;
      uint32_t tSaved;
    }activity;

    // A shared temporary/one-use buffer used as conversion helpers
    char global_buffer[100];


    const uint32_t OSWATCH_RESET_TIME = 30;

    // static unsigned long oswatch_last_loop_time;
    uint32_t tSaved_OSWatchLoop = millis();
    uint8_t oswatch_blocked_loop = 0;

    #ifndef USE_WS2812_DMA  // Collides with Neopixelbus but solves exception
    //void OsWatchTicker() ICACHE_RAM_ATTR;
    #endif  // USE_WS2812_DMA

    #ifdef USE_KNX
    bool knx_started = false;
    #endif  // USE_KNX

    char* p_snprintf(char* buffer, uint16_t buflen, const char* formatP, ...);
    

    uint8_t fSendTemplatesOnce = true;

    // void OsWatchInit(void);
    // void OsWatchLoop(void);
    String GetResetReason(void);
    const char* GetResetReason(char* buffer, uint8_t buflen);

    bool OsWatchBlockedLoop(void);
    size_t strchrspn(const char *str1, int character);
    char* subStr(char* dest, char* str, const char *delim, int index);
    double CharToDouble(const char *str);
    int TextToInt(char *str);
    // char* ulltoa(unsigned long long value, char *str, int radix);
    char* dtostrfd(double number, unsigned char prec, char *s);
    char* Unescape(char* buffer, uint16_t* size);
    char* RemoveSpace(char* p);
    char* LowerCase(char* dest, const char* source);
    char* UpperCase(char* dest, const char* source);
    char* UpperCase_P(char* dest, const char* source);
    char* Trim(char* p);
    char* NoAlNumToUnderscore(char* dest, const char* source);
    void SetShortcut(char* str, uint8_t action);
    uint8_t Shortcut(const char* str);
    bool ValidIpAddress(const char* str);
    bool ParseIp(uint32_t* addr, const char* str);
    void MakeValidMqtt(uint8_t option, char* str);
    bool NewerVersion(char* version_str);
    char* GetPowerDevice(char* dest, uint8_t idx, size_t size, uint8_t option);
    char* GetPowerDevice(char* dest, uint8_t idx, size_t size);
    float ConvertTemp(float c);
    char TempUnit(void);
    float ConvertPressure(float p);
    String PressureUnit(void);
    void SetGlobalValues(float temperature, float humidity);
    void ResetGlobalValues(void);
    double FastPrecisePow(double a, double b);
    uint32_t SqrtInt(uint32_t num);
    uint32_t RoundSqrtInt(uint32_t num);
    char* GetTextIndexed(char* destination, size_t destination_size, uint16_t index, const char* haystack);
    char* GetTextIndexed_P(char* destination, size_t destination_size, uint16_t index, const char* haystack);
    
int16_t SearchForTextIndexedID(const char* name_tofind, const char* haystack, int8_t* class_id, int8_t* device_id);
    
    int GetCommandCode(char* destination, size_t destination_size, const char* needle, const char* haystack);
    int8_t GetStateNumber(const char *state_text);
    void SetSerialBaudrate(int baudrate);
    void ClaimSerial(void);
    void SerialSendRaw(char *codes);
    uint32_t GetHash(const char *buffer, size_t size);
    void ShowSource(int source);
    
    uint16_t WriteBuffer_P(char* buffer, const char* formatP, ...);
    void WriteBuffer_P(char* buffer, uint16_t* length, const char* formatP, ...);

    
static uint16_t changeUIntScale(uint16_t inum, uint16_t ifrom_min, uint16_t ifrom_max,
                                       uint16_t ito_min, uint16_t ito_max) ;





char* GetVersionColour(char* buffer);

    void CrashDump_AddJson();
    void CmndCrash(void);
    void CmndWDT(void);
    void CmndBlockedLoop(void);
    void CrashDumpClear(void);
    bool CrashFlag(void);
    void CrashDump(void);

void UpdateLedPowerAll();
void SetLedPowerIdx(uint32_t led, uint32_t state);
void SetLedPower(uint32_t state);
void SetLedPowerAll(uint32_t state);
void SetLedLink(uint32_t state);




    void ExecuteCommand(char *cmnd, int source);
    int Response_P(const char* format, ...);
    int ResponseAppend_P(const char* format, ...);

    
uint16_t WriteBuffer_P(const char* formatP, ...);

    // long TimeDifference(unsigned long prev, unsigned long next);
    // long TimePassedSince(unsigned long timestamp);
    // bool TimeReachedTimer(unsigned long timer);
    // void SetNextTimeInterval(unsigned long& timer, const unsigned long step);

    void GetLog(uint8_t idx, char** entry_pp, size_t* len_p);

    // void OverrideModule();

    void DebugFreeMem(void);

    // void ParseAndExecuteWebCommands(char* topic, uint8_t* data, unsigned int data_len);


    char* Format(char* output, const char* input, int size);
    char* GetOtaUrl(char *otaurl, size_t otaurl_size);
    void PerformEverySecond(void);
    void Every100mSeconds(void);
    void Every250mSeconds(void);


    void MqttDataHandler(char* topic, uint8_t* data, unsigned int data_len);



void UpdateStatusBlink();

    timereached_t testtime2;


    char* GetStateText(uint8_t state);



    // Add command to 
    void MQTTCommand_Add(const char* topic, const char* payload); // Write command into mpkt struct and set as waiting
    void MQTTCommand_Execute(const char* topic, const char* payload); // Write command into mpkt struct and set as waiting


int8_t GetRSSdBm();
uint8_t GetRSSPercentage();

    uint32_t ResetReason(void);

    void SetPulseTimer(uint32_t index, uint32_t time);

    uint32_t GetPulseTimer(uint32_t index);

    

int ResponseJsonEnd(void);
int ResponseJsonEndEnd(void);

    static int mSearchCtrIndexOf(const char* toSearch, const char* toFind);
    static int mSearchNCtrIndexOf(const char* toSearch, int length,const char* toFind);
    static int NumDigits(int x);
    static void PrintDebugger(char *in, unsigned char length);
    static int memsearch(const char* dataset, int datasetLength, const char* target, int targetLen);
    static uint16_t NumCtr2Num(char* numctr, uint8_t numberlength);
    static float roundfloat(float in, uint8_t dec);
    static float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);
    static int16_t findClosetArrayIndex_float(float* array, uint8_t array_len, float desired);
    static char* strtolower(char *str);
//    uint8_t WITHINLIMITS(float minv, float var, float maxv);


uint32_t tSavedLoop1Sec = millis();
uint32_t tSavedLoop50mSec = millis();
uint32_t tSavedLoop100mSec = millis();
uint32_t tSavedLoop200mSec = millis();
uint32_t tSavedLoop250mSec = millis();
uint32_t loop_runtime_millis;
uint32_t loop_start_millis = millis();
uint32_t loop_delay = millis();
uint32_t loops_per_second = millis();
uint32_t this_cycle_ratio = millis();

//move into timelib
    static bool TimeReached(uint32_t* tSaved, uint32_t ElapsedTime);
    static bool TimeElapsed(uint32_t* tSaved, uint32_t ElapsedTime);
    static bool TimeReachedNonReset(uint32_t* tSaved, uint32_t ElapsedTime);
    // static bool TimeReached(TIMEREACHED_SAVED* tSaved, uint32_t ElapsedTime);
    static bool MillisReached(uint32_t* tTarget);
    static int32_t safeDivideInt(int32_t num, int32_t den);

    //random()
    //random_skewed(low,high,mean_value)
    //random_skewed(low,high,mean_value,steps) (0,10,4,2) = numbers 0-10, but only in steps of 2, centered around 4

    //static const char* FloatToCStr(float f);
    // ConvertFloat2Ctr(char* result, float f, )
    // dtostrfd
    
    static int16_t FindNearestValueIndexUInt8(uint8_t* tosearch, uint8_t tosearch_len, uint8_t tofind);
    static int32_t FindNearestValueIndexUInt16(uint16_t* tosearch, uint16_t tosearch_len, uint16_t tofind);

const char* GetVersionBranchTypeNameByID(uint8_t id);
char GetVersionBranchTypeCharNameByID(uint8_t id);

void init_FirmwareVersion();


// /**
//  * @defgroup lwip_version Version
//  * @ingroup lwip
//  * @{
//  */

// /** X.x.x: Major version of the stack */
// #define LWIP_VERSION_MAJOR      2
// /** x.X.x: Minor version of the stack */
// #define LWIP_VERSION_MINOR      1
// /** x.x.X: Revision of the stack */
// #define LWIP_VERSION_REVISION   2
// /** For release candidates, this is set to 1..254
//   * For official releases, this is set to 255 (LWIP_RC_RELEASE)
//   * For development versions (Git), this is set to 0 (LWIP_RC_DEVELOPMENT) */
// #define LWIP_VERSION_RC         LWIP_RC_RELEASE

// /** LWIP_VERSION_RC is set to LWIP_RC_RELEASE for official releases */
// #define LWIP_RC_RELEASE         255
// /** LWIP_VERSION_RC is set to LWIP_RC_DEVELOPMENT for Git versions */
// #define LWIP_RC_DEVELOPMENT     0

// #define LWIP_VERSION_IS_RELEASE     (LWIP_VERSION_RC == LWIP_RC_RELEASE)
// #define LWIP_VERSION_IS_DEVELOPMENT (LWIP_VERSION_RC == LWIP_RC_DEVELOPMENT)
// #define LWIP_VERSION_IS_RC          ((LWIP_VERSION_RC != LWIP_RC_RELEASE) && (LWIP_VERSION_RC != LWIP_RC_DEVELOPMENT))

// /* Some helper defines to get a version string */
// #define LWIP_VERSTR2(x) #x
// #define LWIP_VERSTR(x) LWIP_VERSTR2(x)
// #if LWIP_VERSION_IS_RELEASE
//   #define LWIP_VERSION_STRING_SUFFIX ""
// #elif LWIP_VERSION_IS_DEVELOPMENT
//   #define LWIP_VERSION_STRING_SUFFIX "d"
// #else
//   #define LWIP_VERSION_STRING_SUFFIX "rc" LWIP_VERSTR(LWIP_VERSION_RC)
// #endif

// /** Provides the version of the stack */
// #define LWIP_VERSION   ((LWIP_VERSION_MAJOR) << 24   | (LWIP_VERSION_MINOR) << 16 | \
//                         (LWIP_VERSION_REVISION) << 8 | (LWIP_VERSION_RC))
// /** Provides the version of the stack as string */
// #define LWIP_VERSION_STRING     LWIP_VERSTR(LWIP_VERSION_MAJOR) "." LWIP_VERSTR(LWIP_VERSION_MINOR) "." LWIP_VERSTR(LWIP_VERSION_REVISION) LWIP_VERSION_STRING_SUFFIX

// /**
//  * @}
//  */

// /* Modules initialization */
// void lwip_init(void);

};





#endif  // _SONOFF_H_
//#endif