#pragma once

#include <stdint.h>

#ifdef ESP32
#if CONFIG_IDF_TARGET_ESP32C2

/* ****************************************
 * ESP32C2
 * ****************************************/
#define MAX_GPIO_PIN       21   // Number of supported GPIO
#define MIN_FLASH_PINS     0    // Number of flash chip pins unusable for configuration (GPIO11 to 17)
#define MAX_USER_PINS      21   // MAX_GPIO_PIN - MIN_FLASH_PINS
#define WEMOS_MODULE       0    // Wemos module

//                                  0 1 2 3 4 5 6 7 8 91011121314151617181920
const char PINS_WEMOS[] PROGMEM = "AOAOAOAOAOIOIOIOIOIOIOFLFLFLFLFLFLFLIORXTX";

#elif defined(CONFIG_IDF_TARGET_ESP32C3)


// ✅ Key Bootstrapping Pins on ESP32-C3
// Pin	Function	Required Boot State
// GPIO9	MTDI / Strapping pin	Low (0) for boot from flash
// GPIO8	MTDO / Strapping pin	High (1) for 3.3V SPI flash
// GPIO2	MTMS / Strapping pin	Typically unconnected or high
// GPIO0	BOOT / Flash download mode	High (1) to boot normally

// ⚠️ If GPIO0 is LOW at reset, the chip enters UART download mode, which could explain why it boots only when the serial port is triggered.


/* ****************************************
 * ESP32C3
 * ****************************************/
#define MAX_GPIO_PIN       22   // Number of supported GPIO
#define MIN_FLASH_PINS     0    // Number of flash chip pins unusable for configuration (GPIO11 to 17)
#define MAX_USER_PINS      22   // MAX_GPIO_PIN - MIN_FLASH_PINS
#define WEMOS_MODULE       0    // Wemos module


#define ESP32_TEMPLATE_TO_PHY         \
   0,   1,  2,  3,  4,                \
   5,   6,  7,  8,  9,                \
   10, 11, 12, 13, 14,                \
   15, 16, 17, 18, 19,                \
   20, 21                             


//                                  0 1 2 3 4 5 6 7 8 9101112131415161718192021
const char PINS_WEMOS[] PROGMEM = "AOAOAOAOAOAOIOIOIOIOIOFLFLFLFLFLFLFLIOIORXTX";

#elif CONFIG_IDF_TARGET_ESP32C6

/* ****************************************
 * ESP32C6
 * ****************************************/
#define MAX_GPIO_PIN       31   // Number of supported GPIO
#define MIN_FLASH_PINS     0    // Number of flash chip pins unusable for configuration (GPIO24 to 30)
#define MAX_USER_PINS      31   // MAX_GPIO_PIN - MIN_FLASH_PINS
#define WEMOS_MODULE       0    // Wemos module

//                                  0 1 2 3 4 5 6 7 8 9101112131415161718192021222324252627282930
const char PINS_WEMOS[] PROGMEM = "AOAOAOAOAOAOAOIOIOIOIOIOIOIOIOIOTXRXIOIOIOIOIOIOFLFLFLFLFLFLFL";

#elif CONFIG_IDF_TARGET_ESP32S2

/* ****************************************
 * ESP32S2
 * ****************************************/
#define MAX_GPIO_PIN       47   // Number of supported GPIO
#define MIN_FLASH_PINS     11   // Number of flash chip pins unusable for configuration (22-25 don't exist, 26-32 for SPI)
#define MAX_USER_PINS      36   // MAX_GPIO_PIN - MIN_FLASH_PINS
#define WEMOS_MODULE       0    // Wemos module

//                                  0 1 2 3 4 5 6 7 8 910111213141516171819202122232425262728293031323334353637383940414243444546
const char PINS_WEMOS[] PROGMEM = "IOAOAOAOAOAOAOAOAOAOAOAOAOAOAOAOAOAOAOAOAOIO--------FLFLFLFLFLFLFLIOIOIOIOIOIOIOIOIOIOIOIOIOI ";

#elif CONFIG_IDF_TARGET_ESP32S3
/* ****************************************
 * ESP32S3
 * GPIOs 0..21 + 33..48
 * - 22..25 are not used
 * - 26..32 are used for SPI Flash
 * - 33..37 are used by PSRAM
 * ****************************************/
#define MAX_GPIO_PIN       49   // Number of supported GPIO, 0..48
#define MIN_FLASH_PINS     11   // Number of flash chip pins unusable for configuration (22-25 don't exist, 26-32 for SPI)
#define MAX_USER_PINS      38   // MAX_GPIO_PIN - MIN_FLASH_PINS
#define WEMOS_MODULE       0    // Wemos module


#define ESP32_TEMPLATE_TO_PHY         \
   0,  1,  2,  3,  4,                 \
   5,  6,  7,  8,  9,                 \
  10, 11, 12, 13, 14,                 \
  15, 16, 17, 18, 19,                 \
  20, 21,                             \
  33, 34, 35, 36, 37, 38, 39, 40,     \
  41, 42, 43, 44, 45, 46, 47, 48



//                                  0 1 2 3 4 5 6 7 8 9101112131415161718192021222324252627282930313233343536373839404142434445464748
const char PINS_WEMOS[] PROGMEM = "IOAOAOAOAOAOAOAOAOAOAOAOAOAOAOAOAOAOAOAOAOIO--------FLFLFLFLFLFLFLIOIOIOIOIOIOIOIOIOIOIOIOIOIOIOIO";

#else  // not CONFIG_IDF_TARGET_ESP32C2/C3/C6 nor CONFIG_IDF_TARGET_ESP32S2 - ESP32


/****** GPIO ESP32  **** https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
 * ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * Index   |  GPIO   | DoitDevKit|      Input      |     Output     |    Hardware                                                                   |   Notes                                                                     |   Ranking  
 * ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * [00]    |    0   |            |    pulled up    |       OK       | ADC2_CH1, TOUCH1, RTC_GPIO11, CLK_OUT1, EMAC_TX_CLK                           |  ADC2 not functional with WiFi, outputs PWM signal at boot                  |   2  
 * [01]    |    1   |            |     TX pin	   |       OK       | U0TXD, CLK_OUT3, EMAC_RXD2                                                    |  Debug output at boot                                                       |   2  
 * [02]    |    2   |            |       OK        |       OK       | ADC2_CH2, TOUCH2, RTC_GPIO12, HSPIWP, HS2_DATA0, SD_DATA0                     |  ADC2 not functional with WiFi, connected to on-board LED                   |   2  
 * [03]    |    3   |            |       OK	       |     RX pin     | U0RXD, CLK_OUT2                                                               |  HIGH at boot                                                               |   2  
 * [04]    |    4   |            |       OK        |       OK       | ADC2_CH0, TOUCH0, RTC_GPIO10, HSPIHD, HS2_DATA1, SD_DATA1, EMAC_TX_ER         |  ADC2 not functional with WiFi                                              |   1  
 * [05]    |    5   |            |       OK        |       OK       | VSPICS0, HS1_DATA6, EMAC_RX_CLK                                               |  Outputs PWM signal at boot                                                 |   2  
 * [--]    |    6   |      x     |      Flash      |      Flash     | Flash CLK                                                                     |  Connected to the integrated SPI flash                                      |   N/A  
 * [--]    |    7   |      x     |      Flash      |      Flash     | Flash D0                                                                      |  Connected to the integrated SPI flash                                      |   N/A  
 * [--]    |    8   |      x     |      Flash      |      Flash     | Flash D1                                                                      |  Connected to the integrated SPI flash                                      |   N/A  
 * [06]    |    9   |      *     |      Flash      |      Flash     | Flash D2, U1RXD                                                               |  Connected to the integrated SPI flash                                      |   N/A  
 * [07]    |    10  |      *     |      Flash      |      Flash     | Flash D3, U1TXD                                                               |  Connected to the integrated SPI flash                                      |   N/A  
 * [--]    |    11  |      x     |      Flash      |      Flash     | Flash CMD                                                                     |  Connected to the integrated SPI flash                                      |   N/A  
 * [08]    |    12  |            |       OK        |       OK       | ADC2_CH5, TOUCH5, RTC_GPIO15, MTDI, HSPIQ, HS2_DATA2, SD_DATA2, EMAC_TXD3     |  Bootstrapping: If HIGH, changes flash voltage. Use caution at boot         |   2  
 * [09]    |    13  |            |       OK        |       OK       | ADC2_CH4, TOUCH4, RTC_GPIO14, MTCK, HSPID, HS2_DATA3, SD_DATA3, EMAC_RX_ER    |  ADC2 not functional with WiFi                                              |   1  
 * [10]    |    14  |            |       OK        |       OK       | ADC2_CH6, TOUCH6, RTC_GPIO16, MTMS, HSPICLK, HS2_CLK, SD_CLK, EMAC_TXD2       |  ADC2 not functional with WiFi, outputs PWM signal at boot                  |   2  
 * [11]    |    15  |            |       OK        |       OK       | ADC2_CH3, TOUCH3, MTDO, HSPICS0, RTC_GPIO13, HS2_CMD, SD_CMD, EMAC_RXD3       |  ADC2 not functional with WiFi, outputs PWM signal at boot                  |   2  
 * [12]    |    16  |            |       OK        |       OK       | HS1_DATA4, U2RXD, EMAC_CLK_OUT                                                |                                                                             |   1  
 * [13]    |    17  |            |       OK        |       OK       | HS1_DATA5, U2TXD, EMAC_CLK_OUT_180                                            |                                                                             |   1  
 * [14]    |    18  |            |       OK        |       OK       | VSPICLK, HS1_DATA7                                                            |                                                                             |   1  
 * [15]    |    19  |            |       OK        |       OK       | VSPIQ, U0CTS, EMAC_TXD0                                                       |                                                                             |   1  
 * [16]    |    21  |            |       OK        |       OK       | VSPIHD, EMAC_TX_EN, default_I2C_DATA                                          |                                                                             |   1  
 * [17]    |    22  |            |       OK        |       OK       | VSPIWP, U0RTS, EMAC_TXD1, default_I2C_CLOCK                                   |                                                                             |   1  
 * [18]    |    23  |            |       OK        |       OK       | VSPID, HS1_STROBE                                                             |                                                                             |   1  
 * [--]    |    24  |            |       --        |       --       |                                                                               |                                                                             |   N/A  
 * [19]    |    25  |            |       OK        |       OK       | DAC_1, ADC2_CH8, RTC_GPIO6, EMAC_RXD0                                         |  ADC2 not functional with WiFi                                              |   2  
 * [20]    |    26  |            |       OK        |       OK       | DAC_2, ADC2_CH9, RTC_GPIO7, EMAC_RXD1                                         |  ADC2 not functional with WiFi                                              |   2  
 * [21]    |    27  |            |       OK        |       OK       | ADC2_CH7, TOUCH7, RTC_GPIO17, EMAC_RX_DV                                      |                                                                             |   1  
 * [22]    |    28  |            |       --        |       --       |                                                                               |                                                                             |   N/A  
 * [22]    |    29  |            |       --        |       --       |                                                                               |                                                                             |   N/A  
 * [22]    |    30  |            |       --        |       --       |                                                                               |                                                                             |   N/A  
 * [22]    |    31  |            |       --        |       --       |                                                                               |                                                                             |   N/A  
 * [22]    |    32  |            |       OK        |       OK       | XTAL_32K_P (32.768 kHz crystal oscillator input), ADC1_CH4, TOUCH9, RTC_GPIO9 |                                                                             |   1  
 * [23]    |    33  |            |       OK        |       OK       | XTAL_32K_N (32.768 kHz crystal oscillator output), ADC1_CH5, TOUCH8, RTC_GPIO8|                                                                             |   1  
 * [24]    |    34  |            |       OK        |    input only  | ADC1_CH6, RTC_GPIO4                                                           |  No pull-up                                                                 |   3  
 * [25]    |    35  |            |       OK        |    input only  | ADC1_CH7, RTC_GPIO5                                                           |  No pull-up                                                                 |   3  
 * [26]    |    36  |            |       OK        |    input only  | SENSOR_VP, ADC_H, ADC1_CH0, RTC_GPIO0                                         |  No pull-up                                                                 |   3  
 * [--]    |    37  |            |       --        |       --       |                                                                               |  No pull-up                                                                 |   N/A  
 * [--]    |    38  |            |       --        |       --       |                                                                               |  No pull-up                                                                 |   N/A  
 * [27]    |    39  |            |       OK        |    input only  | SENSOR_VN, ADC1_CH3, ADC_H, RTC_GPIO3                                         |  No pull-up                                                                 |   3  
 * Total: 28 GPIO 
 * 
 * Explanation of Rankings:

    1: Pins with no bootstrapping or output restrictions and can be used freely. (12)
    2: Pins with bootstrapping restrictions (e.g., outputting signals or must not be driven high at boot). (10)
    3: Pins that are input-only and have no internal pull-up resistors, making them less flexible. (4)
    N/A: Reserved for pins that are connected to the integrated SPI flash or unavailable for GPIO use.

    The numbering helps identify the pins most suitable for general usage (rank 1) and the ones with limitations that should be considered before use (rank 2 and 3).                     
 * ******/

/* ****************************************
 * ESP32 - including Pico
 *
 * The initial template was 0-5 9-10 12-39
 * New template covers 0-27 32-39
 * However to maintain backwards compatibility, the following mapping occures
 * Template GPIO | Phyiscal GPIO
 *   28          |  6
 *   29          |  7
 *   30          |  8
 *   31          |  11
 * ****************************************/
// Conversion table
#define ESP32_TEMPLATE_TO_PHY         \
   0,  1,  2,  3,  4,  5,             \
   /* 6-8 Flash */                    \
   9, 10,                             \
   /* 11 Flash */                     \
  12, 13, 14, 15, 16, 17, 18, 19,     \
  20, 21, 22, 23, 24, 25, 26, 27,     \
  28, 29, 30, 31, /* 28-31 ?? */      \
  32, 33, 34, 35, 36, 37, 38, 39

#define MAX_GPIO_PIN       40   // Number of supported GPIO
#define MIN_FLASH_PINS     4    // Number of flash chip pins unusable for configuration (GPIO6, 7, 8 and 11)
#define MAX_USER_PINS      36   // MAX_GPIO_PIN - MIN_FLASH_PINS
#define WEMOS_MODULE       0    // Wemos module

//                                  0 1 2 3 4 5 6 7 8 9101112131415161718192021222324252627282930313233343536373839
const char PINS_WEMOS[] PROGMEM = "IOTXIORXIOIOFLFLFLFLFLFLIOIOIOIOIOIOIOIOIOIOIOIOIOIOIOIO--------AOAOIAIAIAIAIAIA";

#endif  // ESP32/S2/C2/C3/C6 selection

const uint8_t gpio_pin_by_index[MAX_USER_PINS] = {ESP32_TEMPLATE_TO_PHY};



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

 
#endif  // ESP32

// const uint8_t gpio_pin_by_index[MAX_USER_PINS] = {
//     0, 1, 2, 3, 4, 5,
//     // 6, 7, 8,
//     9, 10, 
//     // 11, 
//     12, 13, 14, 15, 16, 17, 18, 19, 
//     20, 21, 22, 23, 24, 25, 26, 27, 
//     // 28, 29, 30, 31, /* 28-31 */
//     32, 33, 34, 35, 36, 37, 38, 39 
// };
