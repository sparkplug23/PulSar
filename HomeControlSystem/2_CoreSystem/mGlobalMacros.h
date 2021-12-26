
#ifndef _GLOBAL_MACROS_H_
#define _GLOBAL_MACROS_H_ 1

#ifdef ESP8266
#include <avr/pgmspace.h>
#endif // ESP8266
#ifdef ESP32
#define PROGMEM // is nothing, as "const" is enough in esp32 to push to progmem space
#endif

#define DEFINE_PGM_CTR(X) \
  const char X[] PROGMEM =
#define DEFINE_SPGM_CTR(X) \
  static const char X[] PROGMEM =
#define DEFINE_PROGMEM_CTR2(X,Y) \
  const char X[] PROGMEM = Y;

// enable additional debug output
#define DEBUG_PRINT Serial.print
#define DEBUG_PRINTLN Serial.println
#define DEBUG_PRINTF Serial.printf

  #define SPF(param_name,string_type,parameter)  Serial.printf("\"" param_name "\"=" string_type "\n\r", parameter)
  

#define DEBUGGING_PROCEED_ONLY_AFTER_MILLIS_SHOWS_STABLE(X) \
              if(millis()<60000){ return X; } 

// #define ENABLE_DEBUG123 //USE_SOFTWARE_SERIAL_DEBUG

#if defined(ENABLE_DEBUG123)
  // #define DEBUG_PRINT_FUNCTION_NAME   SERIAL_DEBUG.print(__FILE__);\
  //                                     SERIAL_DEBUG.print("\t");\
  //                                     SERIAL_DEBUG.println(__FUNCTION__);\
  //                                     SERIAL_DEBUG.print("\t");\
  //                                     SERIAL_DEBUG.println(__LINE__);\
  //                                     SERIAL_DEBUG.flush();
                                      
#define DEBUG_PRINT Serial.print
#define DEBUG_PRINTLN Serial.println
#define DEBUG_PRINTF Serial.printf
#else
  // #define DEBUG_PRINT_FUNCTION_NAME   //nothing, no code
  
#define DEBUG_PRINT //Serial.print
#define DEBUG_PRINTLN //Serial.println
#define DEBUG_PRINTF //Serial.printf
#endif

/**
 * Use this macro to turn on delays that I can easily turn off again (in case I forget about one). Stoped using "delay" directly, do with macros
 * */
#define ENABLE_DEBUG_DELAYS
#if defined(ENABLE_DEBUG_DELAYS)
#define DEBUG_DELAY(x) delay(x)
#else
#define DEBUG_DELAY(x)
#endif

#ifdef ESP8266
  #define APPEND_ESP_TYPE_MQTT_STRING "_esp8266"
  #define APPEND_ESP_TYPE_NAME_STRING " ESP8266"
#endif
#ifdef ESP32
  #define APPEND_ESP_TYPE_MQTT_STRING "_esp32"
  #define APPEND_ESP_TYPE_NAME_STRING " ESP32"
#endif





/*
// ArduinoJson relies on the type const __FlashStringHelper* to detect if a string is in Flash.

// If you use the F() macro as above, you don’t have to worry about it. However, if you use a char[] with the PROGMEM attribute, you must cast the pointer before passing it to ArduinoJson. Here is an example:

// const char myValue[] PROGMEM = "hello world";
// root["message"] = (const __FlashStringHelper*)myValue;
// This gotcha is not limited to ArduinoJson; many Arduino functions, like Serial.println(), need this cast too.
// //https://arduinojson.org/v6/api/config/enable_progmem/


// RAM:   [======    ]  56.2% (used 46040 bytes from 81920 bytes)
// Flash: [======    ]  56.4% (used 577724 bytes from 1023984 bytes)


// */

// #define USE_DEVFEATURE_JSON_COMMANDS_IN_PROGMEM




// #ifdef USE_DEVFEATURE_JSON_COMMANDS_IN_PROGMEM
//   #define CFLASH  (const __FlashStringHelper*)    //cast flash needed for AJ type check
//   //to allow json testing along with cflash, turning off progme
//   #define DEFINE_PGM_CTR(X) \
//     const char X[] PROGMEM =
// #else
//   #define CFLASH  //(const __FlashStringHelper*)    //cast flash needed for AJ type check
//   //to allow json testing along with cflash, turning off progme
//  #define DEFINE_PGM_CTR(X) \
//   const char X[] =
// #endif // USE_DEVFEATURE_JSON_COMMANDS_IN_PROGMEM
  


#define PRINT_FLUSHED(X) Serial.printf(X); \
                        Serial.println(); \
                        Serial.flush();

#define DEBUG_INSERT_PAGE_BREAK "\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r"
#define D_DEBUG_FUNCTION_NAME_CTR __func__

                      
#define SUBDEFINE_GPIO_TEMPLATE_PIN(X,Y) "\"" X "\":\"" Y "\","            
#define SUBDEFINE_GPIO_TEMPLATE_PIN_END(X,Y) "\"" X "\":\"" Y "\""


#define DEFINE_APP_SVALUE(X,Y) "\"" X "\":\"" Y "\","    //append
#define DEFINE_END_SVALUE(X,Y) "\"" X "\":\"" Y "\""   


#define STR_HELPER2(x) #x
#ifndef STR2
#define STR2(x) STR_HELPER2(x)
#endif




#define DEF_PGM_CTR(X) \
  const char X[] PROGMEM =
#define DEF_PGM_UINT8(X) \
  const uint8_t X[] PROGMEM =


// #define SET_BUFFER_AS_GLOBAL_OR_LOCAL(X,Y) \
//           if(Y == nullptr){ \
//             X = pCONT_sup->global_buffer; \
//           }else{ \
//             X = Y; \
//           }

// #define BUFFER_ISREADY() strlen(data_buffer.payload.ctr)?1:0


#endif