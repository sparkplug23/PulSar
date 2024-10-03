#pragma once
// #ifndef _mHardwarePins_H_82
// #define _mHardwarePins_H_82

#include <stdint.h>
/****** GPIO ESP8266
 * ---------------------------------------------------------------------------------------------------------------------------------------
 * Index   |  GPIO   |  NodeMCU  |     Input      |     Output     |    Hardware    |   Notes                                                                                        |   Ranking
 * ---------------------------------------------------------------------------------------------------------------------------------------
 * [00]    |    0    |    D3     |    pulled up   |       OK       |    D3          |   Connected to FLASH button, boot fails if pulled LOW                                          |   2
 * [01]    |    1    |    TX     |     TX pin	  |       OK       |    TXD0        |   HIGH at boot, debug output at boot, boot fails if pulled LOW                                 |   2
 * [02]    |    2    |    D4     |    pulled up   |       OK       |    TXD1        |   HIGH at boot, connected to on-board LED1, boot fails if pulled LOW                           |   2
 * [03]    |    3    |    RX     |       OK	      |     RX pin     |    RXD0        |   HIGH at boot                                                                                 |   2
 * [04]    |    4    |    D2     |       OK       |       OK       |    SDA         |   Serial0                                                                                      |   1
 * [05]    |    5    |    D1     |       OK       |       OK       |    SCL         |                                                                                                |   1
 * [--]    |    6    |    x      |     Flash      |      Flash     |                |                                                                                                |   N/A
 * [--]    |    7    |    x      |     Flash      |      Flash     |                |                                                                                                |   N/A
 * [--]    |    8    |    x      |     Flash      |      Flash     |                |                                                                                                |   N/A
 * [06]    |    9    |    SD2    |       ?        |        ?       |                |   Only ESP8285, Not ESP8266EX                                                                  |   N/A
 * [07]    |    10   |    SD3    |       ?        |        ?       |                |   Only ESP8285, Not ESP8266EX                                                                  |   N/A
 * [--]    |    11   |    x      |       x        |        x       |                |                                                                                                |   N/A
 * [08]    |    12   |    D6     |       OK       |       OK       |    MISO        |                                                                                                |   1
 * [09]    |    13   |    D7     |       OK       |       OK       |    MOSI, RXD2  |                                                                                                |   1
 * [10]    |    14   |    D5     |       OK       |       OK       |    SCLK        |                                                                                                |   1
 * [11]    |    15   |    D8     |   pulled down  |       OK       |    CS, TXD2    |   Boot fails if pulled HIGH                                                                    |   2
 * [12]    |    16   |    D0     |  no Interrupt  |   no PWM/I2C   |    Wake        |   HIGH at boot, used to wake up from deep sleep                                                |   2
 * [13]    |    A0   |    A0     |                |                |                |   10-bit ADC                                                                                   |   1 (ADC)
 * +++++++++++++++++++++++++++++++++++++++++ Totals ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * 12 GPIO |  12D/1A |           |  6 any, 3 limited 
    Explanation of Rankings:

    1: Pins with no bootstrapping or output restrictions and can be used freely.
    2: Pins with bootstrapping restrictions (e.g., pins that cause boot failures when pulled high/low or are used for special purposes like wake-up or deep sleep).
    N/A: Pins that are connected to the integrated flash or reserved for other purposes and can't be used for GPIO.

    This ranking helps prioritize the best pins for general-purpose use on the ESP8266.
 * ******/


#define MAX_GPIO_PIN       17   // Number of supported GPIO
#define MIN_FLASH_PINS     4    // Number of flash chip pins unusable for configuration (GPIO6, 7, 8 and 11)
#define MAX_USER_PINS      13   // MAX_GPIO_PIN - MIN_FLASH_PINS  // GPIO12 + A0
#define WEMOS_MODULE       0    // Wemos module ie GERNIC IS FIRST ALWAYS


// New method ESP8266
// #ifdef ESP8266

//to be moved
#define MAX_GPI8_PIN       17   // Number of supported GPIO (0..16)
#define FLASH_PINS         6    // Number of flash chip pins

// #define MAX_GPIO_PIN       18   // Number of supported GPIO (0..16 + ADC0)
#define ADC0_PIN           17   // Pin number of ADC0
#define MIN_FLASH_PINS     4    // Number of flash chip pins unusable for configuration (GPIO6, 7, 8 and 11)
// #define MAX_USER_PINS      14   // MAX_GPIO_PIN - MIN_FLASH_PINS
// #define WEMOS_MODULE       17   // Wemos module



// #endif // ESP8266

// #define MAX_GPI8_PIN       17   // Number of supported GPIO (0..16)
// #define FLASH_PINS         6    // Number of flash chip pins

// #define MAX_GPIO_PIN       18   // Number of supported GPIO (0..16 + ADC0)
// #define ADC0_PIN           17   // Pin number of ADC0
// #define MIN_FLASH_PINS     4    // Number of flash chip pins unusable for configuration (GPIO6, 7, 8 and 11)
// // #define MAX_USER_PINS      14   // MAX_GPIO_PIN - MIN_FLASH_PINS
// // #define WEMOS_MODULE       17   // Wemos module


const uint8_t gpio_pin_by_index[MAX_USER_PINS] = {
    0, 1, 2, 3, 4, 5, 
    // 6, 7, 8, 
    9, 10, 
    // 11, 
    12, 13, 14, 15, 16
};



// Idea: Use bitpacked values to store what pins can have inputs, outputs, or other things on them


// #endif