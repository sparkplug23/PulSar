#ifndef _MSTREAMOUT_H
#define _MSTREAMOUT_H 0.5

#define D_UNIQUE_MODULE_CORE_LOGGING_ID 3

#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h> //esp32
#endif
#ifdef ESP8266
  #include <ESP8266WiFi.h>
#endif

#include "1_TaskerManager/mTaskerManager.h"

#include <Arduino.h>
#include <stdint.h>

#ifdef USE_SOFTWARE_SERIAL_DEBUG
  #include <SoftwareSerial.h>
  #define SERIAL_DEBUG Serial
#else
  #define SERIAL_DEBUG Serial
#endif

enum LoggingLevels {LOG_LEVEL_NONE, 
                    LOG_LEVEL_ERROR, 
                    LOG_LEVEL_WARN, 
                    /**
                     * Used when developing a new thing
                     * */
                    LOG_LEVEL_TEST, // New level with elevated previledge - during code development use only
                    /**
                     * Should be used only when I want to highlight it in the serial monitor, it will add new lines to show it better
                     * */
                    LOG_LEVEL_HIGHLIGHT, // New level with elevated previledge - during code development use only
                    LOG_LEVEL_INFO,
                    LOG_LEVEL_COMMANDS, // extra case, this will show when cases are matched 
                    LOG_LEVEL_DEBUG, 
                    LOG_LEVEL_DEBUG_MORE,
                    //#ifdef ENABLE_ADVANCED_DEBUGGING 
                    LOG_LEVEL_DEBUG_LOWLEVEL, 
                    //#endif
                    LOG_LEVEL_ALL
                  };
// Put around ALL addlog_p

  
//  #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES


// Can only be used when hardware serial is enabled
#if defined(USE_DEBUG_LINE) && !defined(USE_SOFTWARE_SERIAL_DEBUG)
  #define DEBUG_LINE    SERIAL_DEBUG.printf("DEBUG: ");\
                        SERIAL_DEBUG.print(__FILE__);\
                        SERIAL_DEBUG.println(__LINE__);\
                        SERIAL_DEBUG.flush();
#else
  #define DEBUG_LINE   //nothing, no code
#endif


// Can only be used when hardware serial is enabled
// #if defined(USE_DEBUG_LINE) && !defined(USE_SOFTWARE_SERIAL_DEBUG)
  #define DEBUG_CHECK_AND_PRINT_NULLPTR(X)    if(X==nullptr){ \
                        SERIAL_DEBUG.printf("nullptr true"); }else{  SERIAL_DEBUG.printf("nullptr false"); }\
                        SERIAL_DEBUG.print(__FILE__);\
                        SERIAL_DEBUG.println(__LINE__);\
                        SERIAL_DEBUG.flush();
// #else
//   #define DEBUG_LINE   //nothing, no code
// #endif


// DEEP DEBUG, added throughout code
#if defined(USE_DEBUG_PRINT_FUNCTION_NAME) && !defined(USE_SOFTWARE_SERIAL_DEBUG)
  #define DEBUG_PRINT_FUNCTION_NAME   SERIAL_DEBUG.print(__FILE__);\
                                      SERIAL_DEBUG.print("\t");\
                                      SERIAL_DEBUG.println(__FUNCTION__);\
                                      SERIAL_DEBUG.print("\t");\
                                      SERIAL_DEBUG.println(__LINE__);\
                                      SERIAL_DEBUG.flush();
#else
  #define DEBUG_PRINT_FUNCTION_NAME   //nothing, no code
#endif
// TEST VERSION, to be used once (not left in code) for development
#if defined(USE_DEBUG_PRINT_FUNCTION_NAME_TEST) && !defined(USE_SOFTWARE_SERIAL_DEBUG)
  #define DEBUG_PRINT_FUNCTION_NAME_TEST   SERIAL_DEBUG.print(__FILE__);\
                                      SERIAL_DEBUG.print("\t");\
                                      SERIAL_DEBUG.println(__FUNCTION__);\
                                      SERIAL_DEBUG.print("\t");\
                                      SERIAL_DEBUG.println(__LINE__);\
                                      SERIAL_DEBUG.flush();
#else
  #define DEBUG_PRINT_FUNCTION_NAME_TEST   //nothing, no code
#endif


/** Easy copy and paste for insitu header debug

  Serial.printf("DEBUG HERE: ");\
                        SERIAL_DEBUG.print(__FILE__);\
                        SERIAL_DEBUG.println(__LINE__);\
                        SERIAL_DEBUG.flush();
**/


//For single test use, no ifdefs
// #ifdef USE_DEBUG_LINE
#if !defined(USE_SOFTWARE_SERIAL_DEBUG)
  #define DEBUG_LINE_HERE    SERIAL_DEBUG.printf("DEBUG HERE: ");\
                        SERIAL_DEBUG.print(__FILE__);\
                        SERIAL_DEBUG.println(__LINE__);\
                        SERIAL_DEBUG.flush();

#else
  #define DEBUG_LINE_HERE   //nothing, no code
#endif

#if !defined(USE_SOFTWARE_SERIAL_DEBUG)
  #define DEBUG_LINE_HERE_PAUSE    SERIAL_DEBUG.printf("DEBUG HERE: ");\
                        SERIAL_DEBUG.print(__FILE__);\
                        SERIAL_DEBUG.println(__LINE__);\
                        SERIAL_DEBUG.flush();\
                        delay(5000);

#else
  #define DEBUG_LINE_HERE_PAUSE   //nothing, no code
#endif



// Can only be used when hardware serial is enabled
// #if defined(USE_DEBUG_LINE) && !defined(USE_SOFTWARE_SERIAL_DEBUG)
  #define DEBUG_HOLD_POINT   while(1) { \
                              SERIAL_DEBUG.printf("Debug Hold Point: ");\
                              SERIAL_DEBUG.print(__FILE__);\
                              SERIAL_DEBUG.println(__LINE__);\
                              SERIAL_DEBUG.flush(); \
                              delay(1000); \
                            }
// #else
//   #define DEBUG_HOLD_POINT   //nothing, no code
// #endif


/**
 * Compact forms of basic AddLog, to make prototyping faster. 
 * Release versions should avoid these, as they don't allow for changing the log_level at runtime
 * */
// #define AddLogI(...)  AddLog(LOG_LEVEL_INFO, 
// ...)




#ifdef USE_DEBUG_LINE_LED
  #define DEBUG_LINE_LED    pinMode(D4,OUTPUT);\
                            digitalWrite(D4,LOW);\
                            delay(500);\
                            digitalWrite(D4,HIGH);\
                            delay(500);\
                            digitalWrite(D4,LOW);\
                            delay(500);\
                            digitalWrite(D4,HIGH);\
                            delay(500);\
                            digitalWrite(D4,LOW);\
                            delay(500);\
                            digitalWrite(D4,HIGH);\
                            delay(500);\
                            digitalWrite(D4,LOW);\
                            delay(500);\
                            digitalWrite(D4,HIGH);
#else
  #define DEBUG_LINE_LED   //nothing, no code
#endif

#include "2_CoreSystem/Time/mTime.h"
class mTime;

#ifdef ESP8266
extern "C" {
#include <cont.h>
  extern cont_t* g_pcont;
}
#endif // ESP8266


#include "1_TaskerManager/mTaskerManager.h"


#include "2_CoreSystem/Languages/mLanguageDefault.h"
#include "2_CoreSystem/Settings/mSettings.h"


  #define D_LOG_LEVEL_NONE_SHORT_CTR            "NON"
  #define D_LOG_LEVEL_ERROR_SHORT_CTR           "ERR"
  #define D_LOG_LEVEL_WARN_SHORT_CTR            "WRN"
  #define D_LOG_LEVEL_TEST_SHORT_CTR            "TST"
  #define D_LOG_LEVEL_INFO_SHORT_CTR            "INF"
  #define D_LOG_LEVEL_COMMANDS_SHORT_CTR        "INP"
  #define D_LOG_LEVEL_DEBUG_SHORT_CTR           "DBG"
  #define D_LOG_LEVEL_DEBUG_MORE_SHORT_CTR      "DBM"
  #define D_LOG_LEVEL_DEBUG_LOWLEVEL_SHORT_CTR  "DBL"
  #define D_LOG_LEVEL_ALL_SHORT_CTR             "ALL"

  DEFINE_PGM_CTR(PM_LOG_LEVEL_NONE_SHORT_CTR)            D_LOG_LEVEL_NONE_SHORT_CTR;
  DEFINE_PGM_CTR(PM_LOG_LEVEL_ERROR_SHORT_CTR)           D_LOG_LEVEL_ERROR_SHORT_CTR;
  DEFINE_PGM_CTR(PM_LOG_LEVEL_WARN_SHORT_CTR)            D_LOG_LEVEL_WARN_SHORT_CTR;
  DEFINE_PGM_CTR(PM_LOG_LEVEL_TEST_SHORT_CTR)            D_LOG_LEVEL_TEST_SHORT_CTR;
  DEFINE_PGM_CTR(PM_LOG_LEVEL_HIGHLIGHT_SHORT_CTR)       "HLT";
  DEFINE_PGM_CTR(PM_LOG_LEVEL_INFO_SHORT_CTR)            D_LOG_LEVEL_INFO_SHORT_CTR;
  DEFINE_PGM_CTR(PM_LOG_LEVEL_COMMANDS_SHORT_CTR)    D_LOG_LEVEL_COMMANDS_SHORT_CTR;
  DEFINE_PGM_CTR(PM_LOG_LEVEL_DEBUG_SHORT_CTR)           D_LOG_LEVEL_DEBUG_SHORT_CTR;
  DEFINE_PGM_CTR(PM_LOG_LEVEL_DEBUG_MORE_SHORT_CTR)      D_LOG_LEVEL_DEBUG_MORE_SHORT_CTR;
  DEFINE_PGM_CTR(PM_LOG_LEVEL_DEBUG_LOWLEVEL_SHORT_CTR)  D_LOG_LEVEL_DEBUG_LOWLEVEL_SHORT_CTR;
  DEFINE_PGM_CTR(PM_LOG_LEVEL_ALL_SHORT_CTR)             D_LOG_LEVEL_ALL_SHORT_CTR;

  #define D_LOG_LEVEL_NONE_LONG_CTR            "NON"
  #define D_LOG_LEVEL_ERROR_LONG_CTR           "ERR"
  #define D_LOG_LEVEL_WARN_LONG_CTR            "WRN"
  #define D_LOG_LEVEL_TEST_LONG_CTR            "TST"
  #define D_LOG_LEVEL_INFO_LONG_CTR            "INF"
  #define D_LOG_LEVEL_COMMANDS_LONG_CTR        "INP"
  #define D_LOG_LEVEL_DEBUG_LONG_CTR           "DBG"
  #define D_LOG_LEVEL_DEBUG_MORE_LONG_CTR      "DBM"
  #define D_LOG_LEVEL_DEBUG_LOWLEVEL_LONG_CTR  "DBL"
  #define D_LOG_LEVEL_ALL_LONG_CTR             "ALL"

  DEFINE_PGM_CTR(PM_LOG_LEVEL_NONE_LONG_CTR)            D_LOG_LEVEL_NONE_LONG_CTR;
  DEFINE_PGM_CTR(PM_LOG_LEVEL_ERROR_LONG_CTR)           D_LOG_LEVEL_ERROR_LONG_CTR;
  DEFINE_PGM_CTR(PM_LOG_LEVEL_WARN_LONG_CTR)            D_LOG_LEVEL_WARN_LONG_CTR;
  DEFINE_PGM_CTR(PM_LOG_LEVEL_TEST_LONG_CTR)            D_LOG_LEVEL_TEST_LONG_CTR;
  DEFINE_PGM_CTR(PM_LOG_LEVEL_INFO_LONG_CTR)            D_LOG_LEVEL_INFO_LONG_CTR;
  DEFINE_PGM_CTR(PM_LOG_LEVEL_COMMANDS_LONG_CTR)    D_LOG_LEVEL_COMMANDS_LONG_CTR;
  DEFINE_PGM_CTR(PM_LOG_LEVEL_DEBUG_LONG_CTR)           D_LOG_LEVEL_DEBUG_LONG_CTR;
  DEFINE_PGM_CTR(PM_LOG_LEVEL_DEBUG_MORE_LONG_CTR)      D_LOG_LEVEL_DEBUG_MORE_LONG_CTR;
  DEFINE_PGM_CTR(PM_LOG_LEVEL_DEBUG_LOWLEVEL_LONG_CTR)  D_LOG_LEVEL_DEBUG_LOWLEVEL_LONG_CTR;
  DEFINE_PGM_CTR(PM_LOG_LEVEL_ALL_LONG_CTR)             D_LOG_LEVEL_ALL_LONG_CTR;

//https://eli.thegreenplace.net/2014/variadic-templates-in-c/

void AddLog(uint8_t loglevel, PGM_P formatP, ...);


void AddLog(uint8_t loglevel, uint32_t* tSaved, uint16_t limit_ms, PGM_P formatP, ...);


// void AddLog(PGM_P formatP, ...);
// void AddSerialLog_mP2(uint8_t loglevel, PGM_P formatP, ...);
int Response_mP(const char* format, ...);
int ResponseAppend_mP(const char* format, ...);
void AddLog_NoTime(uint8_t loglevel, PGM_P formatP, ...);



template<typename T, typename U>
void AddLog_Array(uint8_t loglevel, const char* name_ctr, T* arr, U arr_len)
{
#ifndef DISABLE_SERIAL_LOGGING
  SERIAL_DEBUG.printf("%s = ",name_ctr);

  for(T index=0;index<arr_len;index++){
    SERIAL_DEBUG.printf("%d,", arr[index]);
  }
  SERIAL_DEBUG.printf("\n\r");
  #endif

}

template<typename T, typename U>
void AddLog_Array_P(uint8_t loglevel, const char* name_ctr, T* arr, U arr_len)
{
  T ch;
  #ifndef DISABLE_SERIAL_LOGGING
  SERIAL_DEBUG.printf("%S = ",name_ctr);
  for(T index=0;index<arr_len;index++){
    ch = pgm_read_byte(arr + index);
    SERIAL_DEBUG.printf("%d,", ch);
  }
  SERIAL_DEBUG.printf("\n\r");
  #endif
}


template<typename T>
void AddLog_Array(uint8_t loglevel, uint32_t* tSaved, uint16_t limit_ms, const char* name_ctr, T* arr, T arr_len)//}, uint8_t fWithIndex = 0, uint8_t fVertical = 0)
{
  if(abs(millis()-*tSaved)>=limit_ms){ *tSaved=millis();
    AddLog_Array(loglevel,name_ctr,arr,arr_len);
  }
}



// #define TEST_SINGLETON

#ifdef TEST_SINGLETON
// mLogging* mLogging::mso3 = nullptr;
#endif

#include "1_TaskerManager/mTaskerInterface.h"

class mLogging :
  public mTaskerInterface
{
public:
    mLogging(){}; // Class constructor
    
    void init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mLogging);
    };
    #endif

    static const char* PM_MODULE_CORE_LOGGING_CTR;
    static const char* PM_MODULE_CORE_LOGGING_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CORE_LOGGING_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_CORE_LOGGING_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_LOGGING_ID; }

    void parse_JSONCommand(JsonParserObject obj);

    void handleTelnet();

    enum DEBUG_OUTPUT_IDS{
      DEBUG_OUTPUT_HARDWARE_ID=0,
      DEBUG_OUTPUT_SOFTWARE_ID
    };

     
    #ifdef USE_SOFTWARE_SERIAL_DEBUG
      uint8_t fDebugOutputMode = DEBUG_OUTPUT_SOFTWARE_ID;
    #else
      uint8_t fDebugOutputMode = DEBUG_OUTPUT_HARDWARE_ID;
    #endif

     
    #ifdef DISABLE_SERIAL_LOGGING
      uint8_t fDebugSerialMode = 1;
    #else
      uint8_t fDebugSerialMode = 0;
    #endif
    //mode to include
    // OFF, ON, TIMED_10_MINUTES_FROM_BOOT, TIMED_MINUTES_FROM_USER_REQUEST

 
    void GetLog(uint8_t idx, char** entry_pp, size_t* len_p);
    void SetSeriallog(uint8_t loglevel);
    void Syslog(void);
    void AddLogAddLog(uint8_t loglevel);
    void AddLog(uint8_t loglevel, const char *formatP);
    void AddLog(uint8_t loglevel, const char *formatP, const char *formatP2);
    void AddLog_P2(uint8_t loglevel, PGM_P formatP, ...);
    void AddLogBuffer(uint8_t loglevel, uint8_t *buffer, int count);
    void AddLogSerial(uint8_t loglevel);
    void AddLogMissed(char *sensor, uint8_t misses);

    int8_t SetLogLevelIDbyName(const char* name);
    const char* GetLogLevelNameShort(char* buffer);   
    const char* GetLogLevelNamebyID(uint8_t id, char* buffer); 
    const char* GetLogLevelNameShortbyID(uint8_t id, char* buffer);

    void StartTelnetServer();

    #define TELNET_PORT 24

    WiFiServer* TelnetServer;
    WiFiClient Telnet;

    bool telnet_started = false;
    
};

#endif // header guard
