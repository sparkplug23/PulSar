#ifndef _LOGGING_H
#define _LOGGING_H

#define D_UNIQUE_MODULE_CORE_LOGGING_ID   2008 // [(Folder_Number*100)+ID_File]

#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h> //esp32
#endif
#ifdef ESP8266
  #include <ESP8266WiFi.h>
#endif

#include "1_TaskerManager/mTaskerManager.h"

#include <stdint.h>

/*
| Type             | Purpose                                | Path / Scope                    | Transport / Storage             | Port / Notes                      |
|------------------|----------------------------------------|----------------------------------|----------------------------------|-----------------------------------|
| SerialLog        | Live developer log over USB/UART       | Local, transient                 | Serial                           | No network; best for boot/crash   |
| WebLog           | Recent retained log for Web UI/console | Local on device, short history   | Internal RAM ring buffer         | No port by itself; shown via Web  |
| TelnetLog        | Live remote terminal-style debug       | Remote, transient                | TCP                              | Usually port 23                   |
| Syslog           | Centralised multi-device logging       | External collector/server        | UDP by default, TCP/TLS possible | Common: 514/UDP, 601/TCP, 6514/TLS|
| Console          | Interactive text console               | Local/remote operator interface  | Web UI / serial terminal         | Command entry + textual feedback  |
| MQTT Result      | Command return / state response        | External automation systems      | MQTT broker                      | cmnd in, stat/tele out            |
| ResponseAppend   | Internal response builder              | Internal only                    | RAM buffer then flushed outward  | Use for command/API replies       |
| ResponseCmnd     | Final command reply helper             | Console/Web/MQTT-facing output   | Depends on caller/output path    | Not debug log; command result     |
| Event / Alert    | Async status, fault, or rule message   | External/local subscribers       | Usually MQTT, optionally log too | Separate from developer logging   |
| FileLog          | Persistent on-device diagnostics       | Local, retained                  | Filesystem                       | Useful for post-mortem/debug      |
| NextionMirror    | Optional human-facing mirrored log     | Local attached display           | UART/display text                | Presentation sink, not primary    |
*/

#ifdef USE_SOFTWARE_SERIAL_DEBUG
  #include <SoftwareSerial.h>
  #define SERIAL_DEBUG Serial
#else
  #define SERIAL_DEBUG Serial
#endif

enum LoggingLevels {
  LOG_LEVEL_NONE=0, 

  /**
   *  Highest level of trace debug that will always be shown when called, but should always be disabled via ifdef calls ie "ENABLE_DEBUG_TRACE__##"
   **/
  LOG_LEVEL_DEBUG_TRACE,

  /**
   *  
   **/
  LOG_LEVEL_ERROR, 

  /**
   *  
   **/
  LOG_LEVEL_WARNING, 

  /**
   *  New level with elevated previledge - during code development use only
   **/
  LOG_LEVEL_DEV_TEST,

  /**
   *  Used to pad a message in logging to emphasis in a busy log, used only when required
   **/
  LOG_LEVEL_HIGHLIGHT,
  LOG_LEVEL_HIGHLIGHT_TOP, // Coupled with BOT, they pad a section of debug
  LOG_LEVEL_HIGHLIGHT_BOT,

  /**
   *  Show critical information, such as during boot for easy at a glance that the system is working as expected
   **/
  LOG_LEVEL_IMPORTANT,

  /**
   *  General information and used most often
   **/
  LOG_LEVEL_INFO,

  /**
   *  Detailed logging on commands being parseed
   **/
  LOG_LEVEL_COMMANDS,

  /**
   *  Used often, debugging in the serial monitor
   **/
  LOG_LEVEL_DEBUG, 

  /**
   *  Only used when searching for a high level issue, but keeping up performance
   **/
  LOG_LEVEL_DEBUG_MORE,

  /**
   *  Only used when searching for a crash, will cause critical slow down
   **/
  LOG_LEVEL_DEBUG_LOWLEVEL, 

  /**
   *  Enable all messages
   **/
  LOG_LEVEL_ALL
};

#define BOOT_LOG_LEVEL tkr_set->runtime.seriallog_level_during_boot


// Can only be used when hardware serial is enabled
#if defined(USE_DEBUG_LINE) && !defined(USE_SOFTWARE_SERIAL_DEBUG)
  #define DEBUG_LINE    SERIAL_DEBUG.printf("DEBUG: ");\
                        SERIAL_DEBUG.print(__FILE__);\
                        SERIAL_DEBUG.println(__LINE__);\
                        SERIAL_DEBUG.flush();
#else
  #define DEBUG_LINE   //nothing, no code
#endif


// #define ENABLE_DEBUG_DELAYS
#if defined(ENABLE_DEBUG_MANUAL_DELAYS)
#define DELAY_DEBUG(x) delay(x)
#else
#define DELAY_DEBUG(x)
#endif


#define DEBUG_CRITICAL_STOP_CODE_PRINT while(1);{ delay(1000);  Serial.println("STOPPED: while(1)");}

// Can only be used when hardware serial is enabled
#if defined(USE_DEBUG_CHECK_AND_PRINT_NULLPTR) && !defined(USE_SOFTWARE_SERIAL_DEBUG)
  #define DEBUG_CHECK_AND_PRINT_NULLPTR(X)    if(X==nullptr){ \
                        SERIAL_DEBUG.printf("nullptr true"); }else{  SERIAL_DEBUG.printf("nullptr false"); }\
                        SERIAL_DEBUG.print(__FILE__);\
                        SERIAL_DEBUG.println(__LINE__);\
                        SERIAL_DEBUG.flush();
#else
  #define DEBUG_CHECK_AND_PRINT_NULLPTR   //nothing, no code
#endif


#define USE_DEBUG_PRINT

#ifdef USE_DEBUG_PRINT
  #define DEBUG_PRINT(x) SERIAL_DEBUG.print(x)
  #define DEBUG_PRINTLN(x) SERIAL_DEBUG.println(x); \
                            Serial.flush();
  #define DEBUG_PRINTF(x...) SERIAL_DEBUG.printf(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINTF(x...)
#endif


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


#if defined(ENABLE_DEBUG_LINE_HERE_TRACE)
  #define DEBUG_LINE_HERE_TRACE    SERIAL_DEBUG.printf("DEBUG HERE: ");\
                        SERIAL_DEBUG.print(__FILE__);\
                        SERIAL_DEBUG.println(__LINE__);\
                        SERIAL_DEBUG.flush();
#else
  #define DEBUG_LINE_HERE_TRACE   //nothing, no code
#endif


#if defined(ENABLE_DEBUG_LINE_HERE)
  #define DEBUG_LINE_HERE    SERIAL_DEBUG.printf("DEBUG HERE: ");\
                        SERIAL_DEBUG.print(__FILE__);\
                        SERIAL_DEBUG.println(__LINE__);\
                        SERIAL_DEBUG.flush();
#else
  #define DEBUG_LINE_HERE   //nothing, no code
#endif


#if defined(ENABLE_DEBUG_LINE_HERE2)
  #define DEBUG_LINE_HERE2    SERIAL_DEBUG.printf("DEBUG HERE2: ");\
                        SERIAL_DEBUG.print(__FILE__);\
                        SERIAL_DEBUG.println(__LINE__);\
                        SERIAL_DEBUG.flush();
#else
  #define DEBUG_LINE_HERE2   //nothing, no code
#endif


#if defined(ENABLE_DEBUG_LINE_HERE3)
  #define DEBUG_LINE_HERE3    SERIAL_DEBUG.printf("DEBUG HERE3: ");\
                        SERIAL_DEBUG.print(__FILE__);\
                        SERIAL_DEBUG.println(__LINE__);\
                        SERIAL_DEBUG.flush();
#else
  #define DEBUG_LINE_HERE3   //nothing, no code
#endif


#if defined(ENABLE_DEBUG_LINE_HERE4)
  #define DEBUG_LINE_HERE4    SERIAL_DEBUG.printf("DEBUG HERE4: ");\
                        SERIAL_DEBUG.print(__FILE__);\
                        SERIAL_DEBUG.println(__LINE__);\
                        SERIAL_DEBUG.flush();
#else
  #define DEBUG_LINE_HERE4   //nothing, no code
#endif



// Used when we need to wait at a point considered safe, but need to allow other threads to potentially cause a crash, proving the code after this test point is not the cause.
#define DEBUG_WAIT_POINT_MS(ms) SERIAL_DEBUG.printf("[WAIT DEBUG] %s:%d - wait %lu ms START\n", __FILE__, __LINE__, (unsigned long)(ms)); delay(ms); SERIAL_DEBUG.printf("[WAIT DEBUG] %s:%d - %lu ms Continue\n", __FILE__, __LINE__, (unsigned long)(ms));


#if defined(ENABLE_WAIT_WITH_PRINT_TICK)
  #define WAIT_WITH_PRINT_TICK(ms) do { \
      SERIAL_DEBUG.printf("[WAIT DEBUG] %s:%d - wait %lu ms START\n", __FILE__, __LINE__, (unsigned long)(ms)); \
      uint32_t __wait_tick_start_##__LINE__ = millis(); \
      while (millis() - __wait_tick_start_##__LINE__ < (uint32_t)(ms)) delay(1); \
      SERIAL_DEBUG.printf("[WAIT DEBUG] %s:%d - wait DONE\n", __FILE__, __LINE__); \
      SERIAL_DEBUG.flush(); \
    } while(0)
#else
  #define WAIT_WITH_PRINT_TICK(ms)  // No-op
#endif


#if defined(ENABLE_DEBUG_LINE_HERE_MILLIS)
  #define DEBUG_LINE_HERE_MILLIS    SERIAL_DEBUG.printf("DEBUG: ");\
                        SERIAL_DEBUG.print(__FILE__);\
                        SERIAL_DEBUG.print(__LINE__);\
                        SERIAL_DEBUG.print('-');\
                        SERIAL_DEBUG.println(millis());\
                        SERIAL_DEBUG.flush();
#else
  #define DEBUG_LINE_HERE_MILLIS   //nothing, no code
#endif


#if defined(ENABLE_DEBUG_PRINT_F)
  #define DEBUG_PRINT_F(format, ...)    SERIAL_DEBUG.printf("DEBUG: ");\
                                        SERIAL_DEBUG.printf(format, ##__VA_ARGS__);\
                                        SERIAL_DEBUG.println();\
                                        SERIAL_DEBUG.flush();
#else
  #define DEBUG_PRINT_F(format, ...)   //nothing, no code
#endif


#if defined(ENABLE_DEBUG_TIME__PRINT)
    // Macro to start time measurement, creates a local variable to store the start time
    #define DEBUG_TIME__START uint32_t __debug_time_start__ = micros();
    #define DEBUG_TIME__RESET __debug_time_start__ = micros();

    // Macro to show the elapsed time with a custom message
    #define DEBUG_TIME__SHOW_MESSAGE(message) \
        SERIAL_DEBUG.printf("%s - Time elapsed: %lu us\n\r", message, micros() - __debug_time_start__); \
        SERIAL_DEBUG.flush();

    // Macro to show the elapsed time with file, function, and line information
    // #define DEBUG_TIME__SHOW \
    //     SERIAL_DEBUG.printf("(%s:%d) %luus\n\r", \
    //                         __FILE__, __LINE__, micros() - __debug_time_start__); \
    //     SERIAL_DEBUG.flush();
    // Macro to show the elapsed time with file, function, and line information  with MS
    #define DEBUG_TIME__SHOW \
        SERIAL_DEBUG.printf("(%s:%d) %luus %dms\n\r", \
                            __FILE__, __LINE__, micros() - __debug_time_start__, (micros() - __debug_time_start__)/1000); \
        SERIAL_DEBUG.flush();
    // Macro to show the elapsed time with file, function, and line information
    #define DEBUG_TIME__SHOW_F \
        SERIAL_DEBUG.printf("Time elapsed: %lu us in function %s (%s:%d)\n", \
                            micros() - __debug_time_start__, __func__, __FILE__, __LINE__); \
        SERIAL_DEBUG.flush();

#else
    #define DEBUG_TIME__START  // No operation if debug is disabled
    #define DEBUG_TIME__SHOW_MESSAGE(message)  // No operation if debug is disabled
    #define DEBUG_TIME__SHOW  // No operation if debug is disabled
#endif


#define ENABLE_DEBUG_PRINT_U32
#if defined(ENABLE_DEBUG_PRINT_U32)
    // Macro to print a 32-bit number and its individual bytes
    #define DEBUG_PRINT_U32(message, number) \
        SERIAL_DEBUG.printf("%s %lu: \t%d,%d,%d,%d\n\r", \
                            message, \
                            static_cast<uint32_t>(number), \
                            static_cast<uint8_t>((number) >> 24), /* 4th byte (MSB) */ \
                            static_cast<uint8_t>((number) >> 16), /* 3rd byte */ \
                            static_cast<uint8_t>((number) >> 8),  /* 2nd byte */ \
                            static_cast<uint8_t>((number) & 0xFF)); /* 1st byte (LSB) */ \
        SERIAL_DEBUG.flush();
#else
    #define DEBUG_PRINT_U32(message, number)  // No operation if debug is disabled
#endif


#define DEBUG_CRITICAL_STOP_CODE_PRINT()                                     \
do {                                                                         \
  Serial.printf("line %d\n\r", __LINE__);                                    \
  uint32_t stoptick = 0;                                                     \
  while(1) {                                                                 \
    const uint32_t t_start = millis();                                       \
    while((uint32_t)(millis() - t_start) < 1000) {                           \
      ESP.wdtFeed();                                                         \
      delayMicroseconds(1000);                                               \
    }                                                                        \
    Serial.printf("STOPPED: tick=%lu millis=%lu\n\r",                        \
                  (unsigned long)stoptick,                                   \
                  (unsigned long)millis());                                  \
    stoptick++;                                                              \
  }                                                                          \
} while(0);


// Added indexing, as nested debug points need different saved start points. 
#ifdef ENABLE_DEBUGFEATURE_LIGHTING__TIME_CRITICAL_RECORDING

  #define DEBUG_LIGHTING__START_TIME_RECORDING(X) lighting_time_critical_logging.start_value[X] = micros();
  #define DEBUG_LIGHTING__SAVE_TIME_RECORDING(X, Y)  Y = micros() - lighting_time_critical_logging.start_value[X];

  #define DEBUG_LIGHTING__START_TIME_RECORDING_TASK(X) tkr_anim->lighting_time_critical_logging.start_value[X] = micros();
  #define DEBUG_LIGHTING__SAVE_TIME_RECORDING_TASK(X, Y)  tkr_anim->Y = micros() - tkr_anim->lighting_time_critical_logging.start_value[X];

#else

  #define DEBUG_LIGHTING__START_TIME_RECORDING(X) //none
  #define DEBUG_LIGHTING__SAVE_TIME_RECORDING(X,Y) //none
  #define DEBUG_LIGHTING__START_TIME_RECORDING_TASK(X) //none
  #define DEBUG_LIGHTING__SAVE_TIME_RECORDING_TASK(X,Y) //none

#endif // ENABLE_DEBUGFEATURE_LIGHTING__TIME_CRITICAL_RECORDING



#define ENABLE_SERIAL_TERMINAL_POSITION_RESET__PUTTY


#if defined(ENABLE_SERIAL_TERMINAL_POSITION_RESET__PUTTY)

  // ANSI / VT100 style reset
  // Clear screen + move cursor home
  #define SERIAL_TERMINAL_POSITION_RESET() \
    do { \
      SERIAL_DEBUG.print("\033[2J\033[H"); \
      SERIAL_DEBUG.flush(); \
    } while(0)

#elif defined(ENABLE_SERIAL_TERMINAL_POSITION_RESET__REALTERM)

  // RealTerm is not a proper ANSI terminal in the same way.
  // Best effort only: push a large visual break so the next table
  // appears in a predictable fresh area.
  //
  // You can tune the number of blank lines if needed.
  #define SERIAL_TERMINAL_POSITION_RESET() \
    do { \
      SERIAL_DEBUG.print("\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r"); \
      SERIAL_DEBUG.print("------------------------------------------------------------\n\r"); \
      SERIAL_DEBUG.flush(); \
    } while(0)

#else

  #define SERIAL_TERMINAL_POSITION_RESET() \
    do { \
    } while(0)

#endif


#define DEBUG_PRINT_LN(x) Serial.println(x); Serial.flush();






#if defined(ENABLE_DEBUG_LINE_HERE)
  #define DEBUG_LINE_HERE_MARKER    SERIAL_DEBUG.printf("DEBUG HERE: -------------------------------------------\n\r");\
                        SERIAL_DEBUG.print(__FILE__);\
                        SERIAL_DEBUG.println(__LINE__);\
                        SERIAL_DEBUG.flush();
#else
  #define DEBUG_LINE_HERE_MARKER   //nothing, no code
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

#if !defined(USE_SOFTWARE_SERIAL_DEBUG)
  #define DEBUG_LINE_HERE_SHORT_PAUSE    SERIAL_DEBUG.printf("DEBUG HERE: ");\
                        SERIAL_DEBUG.print(__FILE__);\
                        SERIAL_DEBUG.println(__LINE__);\
                        SERIAL_DEBUG.flush();\
                        delay(1000);

#else
  #define DEBUG_LINE_HERE_PAUSE   //nothing, no code
#endif



#define DEBUG_LINE_HERE_WITH_INT(x)    Serial.printf("DEBUG HERE: [%d] ", x);\
                      Serial.print(__FILE__);\
                      Serial.println(__LINE__);\
                      Serial.flush();
#define DEBUG_LINE_HERE_VALUE(x)    Serial.printf("DEBUG HERE: ");\
                      Serial.print(x);\
                      Serial.print(" ");\
                      Serial.print(__FILE__);\
                      Serial.println(__LINE__);\
                      Serial.flush();

// Can only be used when hardware serial is enabled
#if defined(USE_DEBUG_HOLD_POINT) && !defined(USE_SOFTWARE_SERIAL_DEBUG)
  #define DEBUG_HOLD_POINT   while(1) { \
                              SERIAL_DEBUG.printf("Debug Hold Point: ");\
                              SERIAL_DEBUG.print(__FILE__);\
                              SERIAL_DEBUG.println(__LINE__);\
                              SERIAL_DEBUG.flush(); \
                              delay(1000); \
                            }
#else
  #define DEBUG_HOLD_POINT   //nothing, no code
#endif



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



/**
 * @brief Compact ways of adding a log and compile disabling it
 **/
#ifdef ENABLE_LOG_LEVEL_ERROR
#define ALOG_ERR(...) AddLog(LOG_LEVEL_ERROR, __VA_ARGS__)
#else
#define ALOG_ERR(...)
#endif

#ifdef ENABLE_LOG_LEVEL_WARNING
#define ALOG_WRN(...) AddLog(LOG_LEVEL_WARNING, __VA_ARGS__)
#else
#define ALOG_WRN(...)
#endif

#ifdef ENABLE_LOG_LEVEL_IMPORTANT
#define ALOG_IMP(...) AddLog(LOG_LEVEL_IMPORTANT, __VA_ARGS__)
#else
#define ALOG_IMP(...)
#endif

#ifdef ENABLE_LOG_LEVEL_INFO
#define ALOG_INF(...) AddLog(LOG_LEVEL_INFO, __VA_ARGS__)
#else
#define ALOG_INF(...)
#endif


/**
 * @brief  New method that allows an IF check, this enables the "if" to be removed when not enabled as debug option and hence stops the "if" branch compiling
 
 * Enabled =>  
 * if(x) AddLog(LOG_LEVEL_DEBUG_LOWLEVEL,  __VA_ARGS__)
 * 
 * Disabled
 * BLANK
 */
#ifdef ENABLE_LOG_LEVEL_INFO
#define ALOG_INF_IF(x, ...) if(x) AddLog(LOG_LEVEL_INFO,  __VA_ARGS__) 
#else
#define ALOG_INF_IF(...)
#endif

#ifdef ENABLE_LOG_LEVEL_DEBUG
#define ALOG_DBG(...) AddLog(LOG_LEVEL_DEBUG, __VA_ARGS__)
#else
#define ALOG_DBG(...)
#endif

#ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
#define ALOG_DBM(...) AddLog(LOG_LEVEL_DEBUG_MORE,  __VA_ARGS__)
#else
#define ALOG_DBM(...)
#endif

/**
 * @brief  New method that allows an IF check, this enables the "if" to be removed when not enabled as debug option and hence stops the "if" branch compiling
 
 * Enabled =>  
 * if(x) AddLog(LOG_LEVEL_DEBUG_LOWLEVEL,  __VA_ARGS__)
 * 
 * Disabled
 * BLANK
 */
#ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
#define ALOG_DBM_IF(x, ...) if(x) AddLog(LOG_LEVEL_DEBUG_MORE,  __VA_ARGS__) 
#else
#define ALOG_DBM_IF(...)
#endif

#ifdef ENABLE_LOG_LEVEL_DEBUG_LOWLEVEL
#define ALOG_DBL(...) AddLog(LOG_LEVEL_DEBUG_LOWLEVEL,  __VA_ARGS__)
#else
#define ALOG_DBL(...)
#endif

#ifdef ENABLE_LOG_LEVEL__DEBUG_TRACE
#define ALOG_TRA(...) AddLog(LOG_LEVEL_DEBUG_TRACE, __VA_ARGS__)
#else
#define ALOG_TRA(...)
#endif

#ifdef ENABLE_LOG_LEVEL_COMMANDS
#define ALOG_COM(...) AddLog(LOG_LEVEL_COMMANDS, __VA_ARGS__)
#else
#define ALOG_COM(...)
#endif

// New feature to add, when a new json comes in, the flag will autoclear this. I can send "cmd_response" which will mean all commands should echo their parsing
// instead of fancy passing around of flags, it can be performed in the AddLog by adding a special flag to ignore levels and only show these when the flag is active
#ifdef ENABLE_LOG_LEVEL_COMMANDS_ECHO
#define ALOG_CME(...) AddLog(LOG_LEVEL_COMMANDS, __VA_ARGS__)
#else
#define ALOG_CME(...)
#endif

#ifdef ENABLE_LOG_LEVEL_HIGHLIGHT
#define ALOG_HGL(...) AddLog(LOG_LEVEL_HIGHLIGHT, __VA_ARGS__)
#else
#define ALOG_HGL(...)
#endif
#ifdef ENABLE_LOG_LEVEL_HIGHLIGHT
#define ALOG_HGLT(...) AddLog(LOG_LEVEL_HIGHLIGHT_TOP, __VA_ARGS__)
#else
#define ALOG_HGL(...)
#endif
#ifdef ENABLE_LOG_LEVEL_HIGHLIGHT
#define ALOG_HGLB(...) AddLog(LOG_LEVEL_HIGHLIGHT_BOT, __VA_ARGS__)
#else
#define ALOG_HGL(...)
#endif

#ifdef ENABLE_LOG_LEVEL_TEST
#define ALOG_TST(...) AddLog(LOG_LEVEL_DEV_TEST, __VA_ARGS__)
#else
#define ALOG_TST(...)
#endif


#ifdef ENABLE_LOG_LEVEL_DYNAMIC_LEVELS
#define ALOG(x, ...) AddLog(x, ##__VA_ARGS__)
#else
#define ALOG(x, ...)
#endif


#ifdef USE_DEBUG_DRIVER
#define SHOW_FREE_MEM(WHERE) ShowFreeMem(WHERE);
#else
#define SHOW_FREE_MEM(WHERE)
#endif


#define ALOG_DEBUG_LINE_HERE ALOG_DBG(PSTR("DP:%s|%d"),__FILE__,__LINE__);


#define DEBUG_CRITICAL_FORCE_CRASH  volatile uint32_t dummy; \
                              dummy = *((uint32_t*) 0x00000000);


#include "2_CoreSystem/07_Time/mTime.h"
class mTime;

#ifdef ESP8266
extern "C" {
#include <cont.h>
  extern cont_t* g_pcont;
}
#endif // ESP8266


#include "1_TaskerManager/mTaskerManager.h"

#include "2_CoreSystem/11_Languages/mLanguageDefault.h"
#include "2_CoreSystem/01_Settings/mSettings.h"


#define D_LOG_LEVEL_NONE_CTR            "NON"
#define D_LOG_LEVEL_DEBUG_TRACE_CTR     "DTR"
#define D_LOG_LEVEL_ERROR_CTR           "ERR"
#define D_LOG_LEVEL_WARN_CTR            "WRN"
#define D_LOG_LEVEL_TEST_CTR            "TST"
#define D_LOG_LEVEL_IMPORTANT_CTR       "IMP"
#define D_LOG_LEVEL_INFO_CTR            "INF"
#define D_LOG_LEVEL_HIGHLIGHT_CTR       "HGL"
#define D_LOG_LEVEL_COMMANDS_CTR        "INP"
#define D_LOG_LEVEL_DEBUG_CTR           "DBG"
#define D_LOG_LEVEL_DEBUG_MORE_CTR      "DBM"
#define D_LOG_LEVEL_DEBUG_LOWLEVEL_CTR  "DBL"
#define D_LOG_LEVEL_ALL_CTR             "ALL"

DEFINE_PGM_CTR(PM_LOG_LEVEL_NONE_CTR)            D_LOG_LEVEL_NONE_CTR;
DEFINE_PGM_CTR(PM_LOG_LEVEL_DEBUG_TRACE_CTR)     D_LOG_LEVEL_DEBUG_TRACE_CTR;
DEFINE_PGM_CTR(PM_LOG_LEVEL_ERROR_CTR)           D_LOG_LEVEL_ERROR_CTR;
DEFINE_PGM_CTR(PM_LOG_LEVEL_WARN_CTR)            D_LOG_LEVEL_WARN_CTR;
DEFINE_PGM_CTR(PM_LOG_LEVEL_TEST_CTR)            D_LOG_LEVEL_TEST_CTR;
DEFINE_PGM_CTR(PM_LOG_LEVEL_HIGHLIGHT_CTR)       D_LOG_LEVEL_HIGHLIGHT_CTR;
DEFINE_PGM_CTR(PM_LOG_LEVEL_IMPORTANT_CTR)       D_LOG_LEVEL_IMPORTANT_CTR;
DEFINE_PGM_CTR(PM_LOG_LEVEL_INFO_CTR)            D_LOG_LEVEL_INFO_CTR;
DEFINE_PGM_CTR(PM_LOG_LEVEL_COMMANDS_CTR)        D_LOG_LEVEL_COMMANDS_CTR;
DEFINE_PGM_CTR(PM_LOG_LEVEL_DEBUG_CTR)           D_LOG_LEVEL_DEBUG_CTR;
DEFINE_PGM_CTR(PM_LOG_LEVEL_DEBUG_MORE_CTR)      D_LOG_LEVEL_DEBUG_MORE_CTR;
DEFINE_PGM_CTR(PM_LOG_LEVEL_DEBUG_LOWLEVEL_CTR)  D_LOG_LEVEL_DEBUG_LOWLEVEL_CTR;
DEFINE_PGM_CTR(PM_LOG_LEVEL_ALL_CTR)             D_LOG_LEVEL_ALL_CTR;


/**
 *  
 * Defining for global access, in the cpp
 * Called mostly via define ALOG_X
 */
void AddLog(uint8_t loglevel, PGM_P formatP, ...);
void AddLog(uint8_t loglevel, uint32_t* tSaved, uint16_t limit_ms, PGM_P formatP, ...);


int Response_mP(const char* format, ...);
int ResponseAppend_mP(const char* format, ...);


template<typename T, typename U>
void AddLog_Array(uint8_t loglevel, const char* name_ctr, T* arr, U arr_len, bool use_hex = false)
{
    char logBuffer[512];
    char* logPointer = logBuffer;

    // Prefix
    size_t written = snprintf(logPointer, sizeof(logBuffer), "%s = ", name_ctr);
    logPointer += written;

    // Element format string
    const char* fmt = use_hex ? "%02X" : "%d";

    for (U index = 0; index < arr_len && (logPointer - logBuffer) < sizeof(logBuffer) - 10; ++index) {
        written = snprintf(logPointer, sizeof(logBuffer) - (logPointer - logBuffer), fmt, arr[index]);
        logPointer += written;

        if (index < arr_len - 1) {
            *logPointer++ = ',';
        }
    }

    // Null-terminate
    *logPointer = '\0';

    AddLog(loglevel, PSTR("%s"), logBuffer);
}


template<typename T, typename U, typename V>
void AddLog_Array_Block(uint8_t loglevel, const char* name_ctr, T* arr, U arr_len, V arr_width = 10, bool use_tabs = false)
{
    // Create a buffer to store the log message
    char logBuffer[512];  // Adjust the size if needed
    char* logPointer = logBuffer;

    // Add the array name to the log message and start a new line
    size_t written = snprintf(logPointer, sizeof(logBuffer), "\r\n%s =\r\n", name_ctr);
    logPointer += written;

    // Add array elements to the log message
    for (U index = 0; index < arr_len && (logPointer - logBuffer) < sizeof(logBuffer) - 10; ++index) {
        // Write the value to the buffer
        if (use_tabs) {
            written = snprintf(logPointer, sizeof(logBuffer) - (logPointer - logBuffer), "%d\t", arr[index]);
        } else {
            written = snprintf(logPointer, sizeof(logBuffer) - (logPointer - logBuffer), "%d,", arr[index]);
        }
        logPointer += written;

        // Insert a new line every 'arr_width' values
        if ((index + 1) % arr_width == 0) {
            written = snprintf(logPointer, sizeof(logBuffer) - (logPointer - logBuffer), "\r\n");
            logPointer += written;
        }
    }

    // Null-terminate the string
    if ((logPointer - logBuffer) < sizeof(logBuffer)) {
        *logPointer = '\0';
    }

    // Pass the formatted string to AddLog
    AddLog(loglevel, PSTR("%s"), logBuffer);
}


template<typename T, typename U>
void AddLog_Array_P(uint8_t loglevel, const char* name_ctr, T* arr, U arr_len)
{
    // Create a buffer to store the log message
    char logBuffer[512];  // Adjust the size if needed
    char* logPointer = logBuffer;

    // Add the array name to the log message
    size_t written = snprintf(logPointer, sizeof(logBuffer), "%s = ", name_ctr);
    logPointer += written;

    // Add array elements to the log message
    for (U index = 0; index < arr_len && (logPointer - logBuffer) < sizeof(logBuffer) - 10; ++index) {  // Limit size to avoid buffer overflow
        int value = pgm_read_byte(arr + index);
        written = snprintf(logPointer, sizeof(logBuffer) - (logPointer - logBuffer), "%d,", value);
        logPointer += written;
    }

    // Null-terminate the string
    if ((logPointer - logBuffer) < sizeof(logBuffer)) {
        *logPointer = '\0';
    }

    // Pass the formatted string to AddLog
    AddLog(loglevel, PSTR("%s"), logBuffer);
}



#include "1_TaskerManager/mTaskerInterface.h"

class mLogging :
  public mTaskerInterface
{
public:
    mLogging(){}; // Class constructor
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    static constexpr const char* PM_MODULE_CORE_LOGGING_CTR = D_MODULE_CORE_LOGGING_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CORE_LOGGING_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_LOGGING_ID; }


    void parse_JSONCommand(JsonParserObject obj);


    enum DEBUG_OUTPUT_IDS{
      DEBUG_OUTPUT_HARDWARE_ID=0,
      DEBUG_OUTPUT_SOFTWARE_ID
    };     
    #ifdef USE_SOFTWARE_SERIAL_DEBUG
      uint8_t fDebugOutputMode = DEBUG_OUTPUT_SOFTWARE_ID;
    #else
      uint8_t fDebugOutputMode = DEBUG_OUTPUT_HARDWARE_ID;
    #endif

    const char* GetLogLevelNamebyID(uint8_t id);
    uint8_t GetLogLevelIDbyName(const char* name);

     
    #ifdef DISABLE_SERIAL_LOGGING
      uint8_t fDebugSerialMode = 1;
    #else
      uint8_t fDebugSerialMode = 0;
    #endif
    //mode to include
    // OFF, ON, TIMED_10_MINUTES_FROM_BOOT, TIMED_MINUTES_FROM_USER_REQUEST

    
    void Syslog(void);
    

    #ifndef WEB_LOG_SIZE
    #define WEB_LOG_SIZE 200       // Max number of characters in weblog
    #endif
    struct WEBLOG{
      char buffer[WEB_LOG_SIZE] = {'\0'}; // Web log buffer - REMEMBERS EVERYTHING for new load
      uint8_t index = 1;                  // Index in Web log buffer (should never be 0)
    }weblog;
    void GetLog(uint8_t idx, char** entry_pp, size_t* len_p);


    struct TELNET{
      WiFiServer* server;
      WiFiClient client;
      bool running = false;
    }telnet;
    void StartTelnetServer();
    void handleTelnet();
    #define TELNET_PORT 23


};


#endif // header guard
