//#ifdef ESP8266
#ifndef _MSUPPORT_H_
#define _MSUPPORT_H_

#define D_UNIQUE_MODULE_CORE_SUPPORT_ID 2




/*********************************************************************************************
 * Watchdog related
\*********************************************************************************************/
#ifdef ENABLE_FEATURE_WATCHDOG_TIMER
#ifdef ESP8266
  extern void WDT_Init();
  extern void WDT_Reset();
#endif // ESP8266
#ifdef ESP32
  #include "esp_system.h"
  #ifndef ARDUINO_ISR_ATTR
  #define ARDUINO_ISR_ATTR IRAM_ATTR 
  #endif
  extern void ARDUINO_ISR_ATTR resetModule();
  extern void WDT_Init();
  extern void WDT_Reset();
#endif // ESP32
#endif // WATCHDOG_TIMER_SECTION_GUARD_H


//using class, to save known sun position, maybe in support? azimuth, elevation, isvalid, age


#define ARRAY_SIZE(_a) (sizeof((_a)) / sizeof((_a)[0]))


#include "2_CoreSystem/mBaseConfig.h"


#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <cmath>


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
  STATE_NUMBER_INCREMENT_ID, //eg fans, increase /// Also add a "STATE_NUMBER_INCREMENT_NO_WRAP_ID"
  STATE_NUMBER_DECREMENT_ID,
  STATE_NUMBER_FOLLOW_ID,
  STATE_NUMBER_FOLLOW_INV_ID,
  STATE_NUMBER_LENGTH_ID
};


#define P_PHASE_OUT() Serial.println(F("PHASE OUT"));

// Methods for disable (returning from loop early) until an uptime, network established, or network uptime > x amount
#define DEBUG_OTA_FLASH_BLOCKER_UNTIL_STABLE_RETURN_ZERO()   if(pCONT_time->RtcTime.seconds_nonreset < 120){ return 0; }
#define DEBUG_OTA_FLASH_BLOCKER_UNTIL_UPTIME_X_RETURN_ZERO(X)   if(pCONT_time->RtcTime.seconds_nonreset < X){ return 0; }
// #define DEBUG_OTA_FLASH_BLOCKER_UNTIL_NETWORK_UPTIME_X_RETURN_ZERO(X)   if(pCONT_time->RtcTime.seconds_nonreset < X){ return 0; }


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

#include "1_TaskerManager/mTaskerManager.h"
//#include "//2_CoreSystem/11_Languages/mLanguage.h"
#include "2_CoreSystem/03_HardwareTemplates/mHardwareTemplates.h"
#include "2_CoreSystem/05_Logging/mLogging.h"
#include "2_CoreSystem/02_Time/mTime.h"
#include "2_CoreSystem/01_Settings/mSettings.h"




//#ifdef USE_I2C
  #include <Wire.h>                         // I2C support library
//#endif  // USE_I2C

extern "C" {
extern struct rst_info resetInfo;
}


const char D_CSTRING_ERROR_MESSAGE_CTR[] = "Error";
const char PM_CSTRING_ERROR_MESSAGE_CTR[] PROGMEM = "Error";


// Returns value if its between values, if not, returns midpoint
template<typename T>
T WithinLimits(T lower, T value, T upper){ //RENAME
  // returns value if its between limits
  if((value>lower)&&(value<upper)){
    return value;
  }
  // return midpoint if above was not true
  else{
    return ((upper-lower)/2)+value; //huh?
  }
}

// Returns value if its between values, if not, returns midpoint
template<typename T>
bool IsWithinLimits(T lower, T value, T upper){
  // returns value if its between limits
  if((value>lower)&&(value<upper)){
    return true;
  }
  return false;
}

template <typename T>
bool CheckAndClearFlag(T* flag){
  if(*flag){        // If set, clear and return true, else false
    *flag = false;
    return true;
  }
  return false;
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

/**
 * @brief Faster to check for false than all true
 * 
 * @tparam A 
 * @tparam B 
 * @tparam C 
 * @tparam D 
 * @param x 
 * @param xl 
 * @param y 
 * @param yl 
 * @return true 
 * @return false 
 */
template<typename A, typename B, typename C, typename D>
bool ArrayCompare(A x, B x_len, C y, D y_len)
{
  for(B i=0; i<x_len; i++)
  {
    if(i < y_len)
    {
      if(x[i] != y[i])
      {
        return false;
      }
    }
  }
  return true;


      // if(memcmp(
      //   sensor[sensor_count].address,
      //   address_to_find,
      //   sizeof(sensor[sensor_count].address
      //   ))==0){ // 0 means equal


}

template<typename A,typename B,typename C,typename D,typename E>
E mapvalue(A a, B b, C c, D d, E e){ //should be returning at largest, the size of E range
  return map(a,b,c,d,e);
}

template <typename T, typename U>
T ArrayCountUntilNull(T* buffer, U buflen){
  T count = 0;
  while(buffer != 0){
    count++;
    if(count > buflen){
      break;
    }
  }
  return count;
}

template <typename T>
T GetRandomSaturationVariation(T mean, T standard_deviation, T constrained_min = 0, T constrained_max = 0)
{

  // Get a value between (and inclusing zero) and the maximum range with SD as the centre point
  T random_value = random(0,standard_deviation);

  // Either add or subtract the value from the centre
  T result = mean + (random(0,1)?1:-1)*(int)random_value; 

  // If these are not equal, then apply contraint
  if(constrained_min != constrained_max){
    result = constrain(result, constrained_min, constrained_max);
  }

  // AddLog(LOG_LEVEL_INFO, PSTR("result=%d"),result);

  return result; 

}


template <typename T, typename U, typename V>
bool IsWithinRange(T val_to_check, U min_val, V max_val){

  if(
    (val_to_check >= min_val)&&
    (val_to_check <= max_val)
  ){
    return true;
  }

  return false;

}



// Return new state
template <typename T, typename U>
T ModifyStateNumberIfToggled(T* command_state, U check_state){

  if(*command_state == STATE_NUMBER_TOGGLE_ID){
    if(check_state){
      *command_state = STATE_NUMBER_OFF_ID;
    }else{
      *command_state = STATE_NUMBER_ON_ID;
    }
  }
  // Serial.printf("NOW is %d\n\r",command_state);

  return *command_state; // return new state
}



// Return new state
template <typename T, typename U>
T ConvertStateNumberIfToggled(T command_state, U check_state){

  //Serial.printf("FLIP TO BE %d\n\r",command_state);
  if(command_state == STATE_NUMBER_TOGGLE_ID){
    if(check_state){
      //Serial.println("FLIP TO BE OFF");
      command_state = STATE_NUMBER_OFF_ID;
      // check_state = STATE_NUMBER_OFF_ID;
    }else{
      //Serial.println("FLIP TO BE ON");
      command_state = STATE_NUMBER_ON_ID;
      // check_state = STATE_NUMBER_ON_ID;
    }
  }
  // Serial.printf("NOW is %d\n\r",command_state);

  return command_state; // return new state
}




extern "C" void custom_crash_callback(struct rst_info * rst_info, uint32_t stack, uint32_t stack_end);

/*********************************************************************************************\
 * Watchdog extension (https://github.com/esp8266/Arduino/issues/1532)
\*********************************************************************************************/

// // Global functions
// void AppendJSON_Start();

// template<typename KEY, typename VALUE>
// void AppendJSON_Value(KEY key, VALUE value);

// class mTaskerManager;
    
// template<typename KEY, typename VALUE>
// void AppendJSON_Value(KEY key, VALUE value)
// // template<typename VALUE>
// // void mSupport::AppendJSON_Value(const char* key, VALUE value)
// {

  
//   char* buff = data_buffer.payload.ctr;
//   uint16_t* len = &data_buffer.payload.len;

//   *len = sprintf(&buff[*len],"%s","AppendJSON_Value");

//   // va_list arg;
//   // va_start(arg, formatP);
//   // *length += vsnprintf_P(&buffer[*length], DATA_BUFFER_PAYLOAD_MAX_LENGTH, formatP, arg);
//   // va_end(arg);
//   // return;
// }

extern uint32_t ResetReason_g(void);

extern void SafeMode_StartAndAwaitOTA();

#ifdef ENABLE_DEVFEATURE_FASTBOOT_CELLULAR_SMS_BEACON_FALLBACK_DEFAULT_SSID
#define TINY_GSM_MODEM_SIM7000
#define TINY_GSM_DEBUG Serial
#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb
#define SerialAT Serial1
#include <TinyGsmClient.h>
extern void SafeMode_CellularConnectionAndSendLocation();
#endif // ENABLE_DEVFEATURE_FASTBOOT_CELLULAR_SMS_BEACON_FALLBACK_DEFAULT_SSID


    const uint32_t crash_magic = 0x53415400;   // Stack trace magic number (TASx)
    const uint32_t crash_rtc_offset = 32;      // Offset in RTC memory skipping OTA used block
    const uint32_t crash_dump_max_len = 31;    // Dump only 31 call addresses to satisfy max JSON length of about 600 characters


#include <Ticker.h>


#ifdef ENABLE_DEVFEATURE_HARDWARE_STATUS
enum HARDWARE_STATUS_IMPORTANCE_IDS{
  // No risk to remain on
  HARDWARE_STATUS_IMPORTANCE_LOW_ID=0,
  // RGB leds, not high voltage
  HARDWARE_STATUS_IMPORTANCE_MEDIUM_ID,
  // Plugs/sockets, mains running
  HARDWARE_STATUS_IMPORTANCE_HIGHEST_ID,
};
#endif // ENABLE_DEVFEATURE_HARDWARE_STATUS


#include "1_TaskerManager/mTaskerInterface.h"


class mSupport :
  public mTaskerInterface
{
  public:
    mSupport(){};
    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void init(void);

    static const char* PM_MODULE_CORE_SUPPORT_CTR;
    static const char* PM_MODULE_CORE_SUPPORT_FRIENDLY_CTR;
    PGM_P GetModuleName(){ return PM_MODULE_CORE_SUPPORT_CTR; }
    PGM_P GetModuleFriendlyName(){ return PM_MODULE_CORE_SUPPORT_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_SUPPORT_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mSupport);
    };
    #endif

    void CheckResetConditions();
    void Handle_OTA_URLS();
    void Handle_Check_Power_Saving();


    // #ifdef ENABLE_DEVFEATURE_OTA_METHOD
    #ifdef USE_ARDUINO_OTA
      /*********************************************************************************************\
       * Allow updating via the Arduino OTA-protocol.
       *
       * - Once started disables current wifi clients and udp
       * - Perform restart when done to re-init wifi clients
      \*********************************************************************************************/

      bool arduino_ota_triggered = false;
      uint16_t arduino_ota_progress_dot_count = 0;
      bool ota_init_success = false;

      void ArduinoOTAInit(void);
      void ArduinoOtaLoop(void);
    #endif // USE_ARDUINO_OTA
    // #endif // ENABLE_DEVFEATURE_OTA_METHOD



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


    #ifdef USE_I2C
    const uint8_t I2C_RETRY_COUNTER = 3;
    uint32_t i2c_active[4] = { 0 };
    uint32_t i2c_buffer = 0;
    TwoWire* wire = nullptr;
    bool I2cValidRead(uint8_t addr, uint8_t reg, uint8_t size);
    bool I2cValidRead8(uint8_t *data, uint8_t addr, uint8_t reg);
    bool I2cValidRead16(uint16_t *data, uint8_t addr, uint8_t reg);
    bool I2cValidReadS16(int16_t *data, uint8_t addr, uint8_t reg);
    bool I2cValidRead16LE(uint16_t *data, uint8_t addr, uint8_t reg);
    bool I2cValidReadS16_LE(int16_t *data, uint8_t addr, uint8_t reg);
    bool I2cValidRead24(int32_t *data, uint8_t addr, uint8_t reg);
    uint8_t I2cRead8(uint8_t addr, uint8_t reg);
    uint16_t I2cRead16(uint8_t addr, uint8_t reg);
    int16_t I2cReadS16(uint8_t addr, uint8_t reg);
    uint16_t I2cRead16LE(uint8_t addr, uint8_t reg);
    int16_t I2cReadS16_LE(uint8_t addr, uint8_t reg);
    int32_t I2cRead24(uint8_t addr, uint8_t reg);
    bool I2cWrite(uint8_t addr, uint8_t reg, uint32_t val, uint8_t size);
    bool I2cWrite8(uint8_t addr, uint8_t reg, uint16_t val);
    bool I2cWrite16(uint8_t addr, uint8_t reg, uint16_t val);
    int8_t I2cReadBuffer(uint8_t addr, uint8_t reg, uint8_t *reg_data, uint16_t len);
    int8_t I2cWriteBuffer(uint8_t addr, uint8_t reg, uint8_t *reg_data, uint16_t len);
    void I2cScan(char *devs, unsigned int devs_len);
    bool I2cDevice(uint8_t addr);
    bool I2cDevice_IsConnected(uint8_t addr) ;
    void I2cResetActive(uint32_t addr, uint32_t count = 1);
    void I2cSetActive(uint32_t addr, uint32_t count = 1);
    void I2cSetActiveFound(uint32_t addr, const char *types);
    bool I2cActive(uint32_t addr);
    bool I2cSetDevice(uint32_t addr);
    bool I2cEnabled(uint32_t i2c_index);
    void Debug_I2CScan_To_Serial();
    #endif // USE_I2C


    char* GetState_Name_by_ID(uint8_t id, char* buffer, uint8_t buflen);





    IPAddress syslog_host_addr;      // Syslog host IP address
    uint32_t syslog_host_hash = 0;   // Syslog host name hash

    uint8_t GetNormalDistributionRandom(uint8_t mean, uint8_t standard_deviation, uint8_t constrained_min = 0, uint8_t constrained_max = 0);
    // uint8_t GetRandomSaturationVariation(uint8_t mean, uint8_t standard_deviation, uint8_t constrained_min = 0, uint8_t constrained_max = 0);


    // uint32_t state_100msecond,state_250msecond;

    Ticker tickerOSWatch;

    void parse_JSONCommand(JsonParserObject obj);

    // Randomise array (aka std lib)
    template<typename DATA, typename LENGTH>
    void ArrayRandomise(DATA* data, LENGTH length){

      //duplicate original array
      // DATA data_original[length];
      // memcpy(data_original,data,sizeof(DATA)*length);

      //
  // std::array<int,5> foo {1,2,3,4,5};

   
  //     std::shuffle(foo.begin(),array.end(),std::default_random_engine(analogRead(0)));




    }

    
    #ifdef ENABLE_DEVFEATURE_HARDWARE_STATUS
    #define HARDWARE_STATUS_MAX_LENGTH 200
    struct STATUSHARDWARE{
      //make function that appends pretty simple message
      char ctr[HARDWARE_STATUS_MAX_LENGTH];
      uint8_t len = 0;
      uint8_t importance = 0; //0 low, 1 med, 2 high
    }hardwarestatus;
    void ConstructCtr_HardwareStatus();
    #endif // ENABLE_DEVFEATURE_HARDWARE_STATUS


char* GetTextIndexedTemp(char* destination, size_t destination_size, uint16_t index, const char* haystack);


bool JsonLevelFlagCheck(uint8_t json_level_testing, uint8_t json_level_set, uint8_t ischanged = false);

void AppendDList(char* buffer, const char* to_add);
void AppendDList(char* buffer, uint16_t buflen, const char* formatP, ...);
int GetDListIDbyNameCtr_P(char* destination, size_t destination_size, const char* needle, const char* haystack);
int GetDListIDbyNameCtr(char* destination, size_t destination_size, const char* needle, const char* haystack);



    uint32_t tSaved_SlowAllTemplatesOnSerial;
    void SlowAllTemplatesOnSerial();


    struct LOOP_PERFORMANCE{
      uint32_t loop_counter=0;
      uint32_t cycles_per_sec=0;
      uint32_t tSaved;
    }activity;

    // A shared temporary/one-use buffer used as conversion helpers
    #define GLOBAL_BUFFER_LENGTH 100
    char global_buffer[GLOBAL_BUFFER_LENGTH];


    // const uint32_t OSWATCH_RESET_TIME = 30;

    // // static unsigned long oswatch_last_loop_time;
    // uint32_t tSaved_OSWatchLoop = millis();
    // uint8_t oswatch_blocked_loop = 0;

    // #ifndef USE_WS2812_DMA  // Collides with Neopixelbus but solves exception
    // //void OsWatchTicker() ICACHE_RAM_ATTR;
    // #endif  // USE_WS2812_DMA

    // #ifdef USE_KNX
    // bool knx_started = false;
    // #endif  // USE_KNX

    char* p_snprintf(char* buffer, uint16_t buflen, const char* formatP, ...);
    

    // uint8_t fSendTemplatesOnce = true;

    // void OsWatchInit(void);
    // void OsWatchLoop(void);
    String GetResetReason(void);
    const char* GetResetReason(char* buffer, uint8_t buflen);

    bool OsWatchBlockedLoop(void);
    size_t strchrspn(const char *str1, int character);
    char* subStr(char* dest, char* str, const char *delim, int index);
    float CharToFloat(const char *str);
    double CharToDouble(const char *str);
    int TextToInt(char *str);
    // char* ulltoa(unsigned long long value, char *str, int radix);
    // char* dtostrfd(double number, unsigned char prec, char *s);
    static char* float2CString(float number, unsigned char prec, char *s);
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
    bool NewerVersion(char* version_str);
    // char* GetPowerDevice(char* dest, uint8_t idx, size_t size, uint8_t option);
    // char* GetPowerDevice(char* dest, uint8_t idx, size_t size);
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


// Force a float value between two ranges, and adds or substract the range until we fit
float ModulusRangef(float f, float a, float b);

static bool CheckCommand_P(const char* needle, const char* haystack);
static int8_t GetCommandID(const char* needle, const char* haystack, char* destination = nullptr, size_t destination_size = 0);
static int8_t GetCommandID_P(const char* needle, const char* haystack, char* destination = nullptr, size_t destination_size = 0);
static int16_t GetCommandID16_P(const char* needle, const char* haystack, char* destination = nullptr, size_t destination_size = 0);


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




    void CommandSet_Restart(int8_t command);



    char* GetVersionColour(char* buffer);

    void CrashDump_AddJson();
    void CmndCrash(void);
    void CmndWDT(void);
    void CmndBlockedLoop(void);
    void CrashDumpClear(void);
    bool CrashFlag(void);
    void CrashDump(void);




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

    char* GetOtaUrl(char *otaurl, size_t otaurl_size);
    void PerformEverySecond(void);


    void MqttDataHandler(char* topic, uint8_t* data, unsigned int data_len);



void UpdateStatusBlink();

    timereached_t testtime2;


    char* GetStateText(uint8_t state);


void SleepDelay(uint32_t mseconds);

    // Add command to 
    void MQTTCommand_Add(const char* topic, const char* payload); // Write command into mpkt struct and set as waiting
    void MQTTCommand_Execute(const char* topic, const char* payload); // Write command into mpkt struct and set as waiting



    uint32_t ResetReason(void);

    void SetPulseTimer(uint32_t index, uint32_t time);

    uint32_t GetPulseTimer(uint32_t index);

    int ResponseJsonEnd(void);
    int ResponseJsonEndEnd(void);

    static bool SetTopicMatch(const char* toSearch, const char* set_topic_path);
    static bool SetTopicMatch_P(const char* toSearch, const char* set_topic_path);
    static int mSearchCtrIndexOf(const char* toSearch, const char* toFind);
    static int mSearchNCtrIndexOf(const char* toSearch, int length,const char* toFind);
    static int NumDigits(int x);

    
template <typename T>
static uint8_t NumDigitsT(T x)
{
    x = abs(x); //remove any negative numbers
    return (x < 10 ? 1 :
        (x < 100 ? 2 :
        (x < 1000 ? 3 :
        (x < 10000 ? 4 :
        (x < 100000 ? 5 :
        (x < 1000000 ? 6 :
        (x < 10000000 ? 7 :
        (x < 100000000 ? 8 :
        (x < 1000000000 ? 9 :
        10)))))))));
}




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
uint32_t tSavedLoop250mSec = millis();
uint32_t loop_runtime_millis;
uint32_t loop_start_millis = millis();
uint32_t loop_delay_temp = millis();
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