#ifndef _mHardwareTemplates2_H_
#define _mHardwareTemplates2_H_


#define D_UNIQUE_MODULE_CORE_HARDWARETEMPLATES_ID ((2*1000)+4)


#include "stdint.h"
#include "2_CoreSystem/mBaseConfig.h"
#include "2_CoreSystem/mGlobalMacros.h"

/********************************************************************************************/

#define USER_MODULE        255

#ifdef ESP8266

    #define MAX_GPIO_PIN       17   // Number of supported GPIO
    #define MIN_FLASH_PINS     4    // Number of flash chip pins unusable for configuration (GPIO6, 7, 8 and 11)
    #define MAX_USER_PINS      13   // MAX_GPIO_PIN - MIN_FLASH_PINS  // GPIO12 + A0
    #define WEMOS_MODULE       0    // Wemos module ie GERNIC IS FIRST ALWAYS
    #define MAX_GPI8_PIN       17   // Number of supported GPIO (0..16)
    #define FLASH_PINS         6    // Number of flash chip pins

    typedef struct MYIO8 {
      uint16_t      io[MAX_GPI8_PIN];
    } myio8;                        // 17 bytes

    typedef struct MYCFGIO8285 {
      uint16_t      io[MAX_GPI8_PIN - MIN_FLASH_PINS];
    } mycfgio8285;                  // 13 bytes

    typedef struct MYTMPLT8285 {
      mycfgio8285  gp;
      uint8_t      flag;
    } mytmplt8285;                  // 14 bytes

    typedef struct MYCFGIO8266 {
      uint16_t      io[MAX_GPI8_PIN - 4];//temp fix FLASH_PINS];
    } mycfgio8266;                  // 11 bytes

    typedef struct MYTMPLT8266 {
      mycfgio8266  gp;
      uint16_t      flag;
    } mytmplt8266;                  // 12 bytes

#endif // ESP8266


#ifdef ESP32


    #define MAX_GPIO_PIN       40   // Number of supported GPIO
    #define MIN_FLASH_PINS     4    // Number of flash chip pins unusable for configuration (GPIO6, 7, 8 and 11)
    #define MAX_USER_PINS      36   // MAX_GPIO_PIN - MIN_FLASH_PINS
    #define WEMOS_MODULE       0    // Wemos module

#endif


// ESP32?
typedef struct MYIO {
  uint16_t      io[MAX_GPIO_PIN];
} myio;                         // 18 * 2 = 36 bytes / 40 * 2 = 80 bytes

typedef struct MYCFGIO {
  uint16_t      io[MAX_USER_PINS];// = {GPIO_NONE_ID};
} mycfgio;                      // 14 * 2 = 28 bytes / 36 * 2 = 72 bytes


typedef union {
  uint8_t data;
  struct {
    uint8_t adc0 : 1;             // Allow ADC0 when define USE_ADC_VCC is disabled
    uint8_t adc0_temp : 1;        // Allow ADC0 as Temperature sensor when define USE_ADC_VCC is disabled
    uint8_t spare02 : 1;
    uint8_t spare03 : 1;
    uint8_t spare04 : 1;
    uint8_t spare05 : 1;
    uint8_t spare06 : 1;
    uint8_t spare07 : 1;
  };
} gpio_flag;


typedef struct MYTMPLT {
  mycfgio      gp;
  gpio_flag    flag;
} mytmplt;


#endif  // _mHardwareTemplates_H_