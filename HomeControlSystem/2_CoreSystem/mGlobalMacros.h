
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


#define SET_BUFFER_AS_GLOBAL_OR_LOCAL(X,Y) \
          if(Y == nullptr){ \
            X = pCONT_sup->global_buffer; \
          }else{ \
            X = Y; \
          }

#define BUFFER_ISREADY() strlen(data_buffer.payload.ctr)?1:0


#endif