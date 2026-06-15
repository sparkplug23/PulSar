#include "mPins.h"


/********************************************************************************************\
 * ESP8266 Module Templates
\********************************************************************************************/

#ifdef ESP8266 // ==========================================================================================================

/**
 * @brief This contains indexing for esp8266 THEN esp8285, the index (value) is important for retrieving template from either esp8266 or esp8285 arrays
 * 
 */
const uint16_t mPins::module_template__ids[] PROGMEM = {
  /**
   * ESP8266 modules
   **/
  #ifdef USE_MODULE_TEMPLATE_SONOFF_BASIC
    MODULE_SONOFF_BASIC,
  #endif
  #ifdef USE_MODULE_TEMPLATE_SONOFF_BASIC_EXTERNAL
    MODULE_SONOFF_BASIC_EXTERNAL,
  #endif
  #ifdef USE_MODULE_TEMPLATE_H801  
    MODULE_H801,
  #endif
  #ifdef USE_MODULE_TEMPLATE_MAGICHOME
    MODULE_MAGICHOME,
  #endif
  #ifdef USE_MODULE_TEMPLATE_SHELLY1
    MODULE_SHELLY1,
  #endif
  #ifdef USE_MODULE_TEMPLATE_SHELLY_2P5
    MODULE_SHELLY2P5,
  #endif
  #ifdef USE_MODULE_TEMPLATE_SHELLY_DIMMER2
    MODULE_SHELLY_DIMMER2,
  #endif  
  /**
  * ESP8285 modules
  **/
  MODULE_GENERIC,  //esp8285
  #ifdef USE_MODULE_TEMPLATE_SONOFF_IFAN03
    MODULE_SONOFF_IFAN03,  //esp8285
  #endif
  #ifdef USE_MODULE_TEMPLATE_SONOFF_4CHPRO
    MODULE_SONOFF_4CHPRO   //esp8285
  #endif
};


const char mPins::module_template__names[] PROGMEM
{
  // ESP8266 modules
  #ifdef USE_MODULE_TEMPLATE_SONOFF_BASIC
    D_MODULE_NAME_SONOFF_BASIC_CTR  "|"
  #endif
  #ifdef USE_MODULE_TEMPLATE_SONOFF_BASIC_EXTERNAL
    D_MODULE_NAME_SONOFF_BASIC_EXTERNAL_CTR  "|"
  #endif
  #ifdef USE_MODULE_TEMPLATE_H801  
    D_MODULE_NAME_H801_CTR          "|"
  #endif
  #ifdef USE_MODULE_TEMPLATE_MAGICHOME
    D_MODULE_NAME_MAGICHOME_CTR     "|"
  #endif
  #ifdef USE_MODULE_TEMPLATE_SHELLY1
    D_MODULE_NAME_SHELLY1_CTR       "|"
  #endif
  #ifdef USE_MODULE_TEMPLATE_SHELLY_2P5
    D_MODULE_NAME_SHELLY2P5_CTR     "|"
  #endif
  #ifdef USE_MODULE_TEMPLATE_SHELLY_DIMMER2
    D_MODULE_NAME_SHELLY_DIMMER2_CTR "|"
  #endif
  // ESP8285 modules
  D_MODULE_NAME_USERMODULE_CTR "|"
  #ifdef USE_MODULE_TEMPLATE_SONOFF_IFAN03
   D_MODULE_NAME_SONOFF_IFAN03_CTR  "|"
  #endif
  #ifdef USE_MODULE_TEMPLATE_SONOFF_4CHPRO
  D_MODULE_NAME_SONOFF_4CHPRO_CTR
  #endif   
};


/*********************************************************************************************\
 * Templates with 12 usable pins (ESP8266)
\*********************************************************************************************/
const mytmplt8266 mPins::module_template__gpio_map_ESP8266[MODULE_MAXMODULE_8266] PROGMEM = {
  
  #ifdef USE_MODULE_TEMPLATE_SONOFF_BASIC
  {                           // MODULE_SONOFF_BASIC_ID    // Sonoff Basic (ESP8266)
    GPIO_KEY,                // GPIO00 Button
    GPIO_USER,                // GPIO01 Serial RXD and Optional sensor
    PIGPIO_N(GPIO_LED_INV,2),            // GPIO02 Only available on newer Sonoff Basic R2 V1
    GPIO_USER,                // GPIO03 Serial TXD and Optional sensor
    GPIO_USER,                // GPIO04 Optional sensor
    0,                        // GPIO05
                              // GPIO06 (SD_CLK   Flash)
                              // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                              // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
                              // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
                              // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                              // GPIO11 (SD_CMD   Flash)
    PIGPIO_N(GPIO_REL,1),                // GPIO12 Red Led and Relay (0 = Off,    1 = On)
    PIGPIO_N(GPIO_LED_INV,1),            // GPIO13 Green Led (0 = On, 1 = Off) - Link and Power status
    GPIO_USER,                // GPIO14 Optional sensor
    0,                        // GPIO15
    0,                        // GPIO16
    0                         // ADC0 Analog input
  },
  #endif
  #ifdef USE_MODULE_TEMPLATE_SONOFF_BASIC_EXTERNAL
  {                   // MODULE_SONOFF_BASIC_EXTERNAL_ID    // Sonoff Basic (ESP8266)
    PIGPIO_N(GPIO_KEY,1),        // GPIO00 Button
    GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
    PIGPIO_N(GPIO_LED_INV,2),    // GPIO02 Only available on newer Sonoff Basic R2 V1
    PIGPIO_N(GPIO_KEY,2),        // GPIO03 Serial TXD and Optional sensor
    GPIO_USER,        // GPIO04 Optional sensor
    0,                // GPIO05
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
                      // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
                      // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    PIGPIO_N(GPIO_REL,1),        // GPIO12 Red Led and Relay (0 = Off,    1 = On)
    PIGPIO_N(GPIO_LED_INV,1),    // GPIO13 Green Led (0 = On, 1 = Off) - Link and Power status
    GPIO_USER,        // GPIO14 Optional sensor
    0,                // GPIO15
    0,                // GPIO16
    0                 // ADC0 Analog input
  },
  #endif
  #ifdef USE_MODULE_TEMPLATE_H801
  {                      // MODULE_H801,               // Lixada H801 Wifi (ESP8266)
    PIGPIO_N(GPIO_KEY,1),           // GPIO00 E-FW Button   // TEMPORARILY FORCING AS A KEY1 HERE, REMOVE WHEN GPIOC IS PROPERLY ADDED INTO CODE
    PIGPIO_N(GPIO_LED,1),           // GPIO01 Green LED - Link and Power status
    GPIO_USER,           // GPIO02 TX and Optional sensor - Pin next to TX on the PCB
    GPIO_USER,           // GPIO03 RX and Optional sensor - Pin next to GND on the PCB
    PIGPIO_N(GPIO_PWM,5),           // GPIO04 W2 - PWM5
    PIGPIO_N(GPIO_LED_INV,2),       // GPIO05 Red LED
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
                      // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
                      // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    PIGPIO_N(GPIO_PWM,3),           // GPIO12 Blue
    PIGPIO_N(GPIO_PWM,2),           // GPIO13 Green
    PIGPIO_N(GPIO_PWM,4),           // GPIO14 W1 - PWM4
    PIGPIO_N(GPIO_PWM,1),           // GPIO15 Red
    0, // GPIO16
    0 // A0
  },
  #endif
  #ifdef USE_MODULE_TEMPLATE_MAGICHOME
  { //MODULE_MAGICHOME,          // Magic Home (aka Flux-light) (ESP8266) and Arilux LC10 (ESP8285)
                      // https://www.aliexpress.com/item/Magic-Home-Mini-RGB-RGBW-Wifi-Controller-For-Led-Strip-Panel-light-Timing-Function-16million-colors/32686853650.html
    0,
    GPIO_USER,           // GPIO01 Serial RXD and Optional sensor
    PIGPIO_N(GPIO_LED_INV,1),       // GPIO02 Blue onboard LED - Link and Power status
    GPIO_USER,           // GPIO03 Serial TXD and Optional sensor
    GPIO_USER, //GPIO_ARIRFRCV,       // GPIO04 IR or RF receiver (optional) (Arilux LC10)
    PIGPIO_N(GPIO_PWM,2),           // GPIO05 RGB LED Green
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
                      // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
                      // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    PIGPIO_N(GPIO_PWM,3),           // GPIO12 RGB LED Blue
    GPIO_USER,           // GPIO13 RGBW LED White (optional - set to PWM4 for Cold White or Warm White as used on Arilux LC10)
    PIGPIO_N(GPIO_PWM,1),           // GPIO14 RGB LED Red
    PIGPIO_N(GPIO_LED_INV,4),       // GPIO15 RF receiver control (Arilux LC10)
    0, 0
  },
  #endif
  #ifdef USE_MODULE_TEMPLATE_SHELLY1
  {//MODULE_SHELLY1    // Shelly1 Open Source (ESP8266 - 2MB) - https://shelly.cloud/shelly1-open-source/
     0,                // GPIO00 - Can be changed to GPIO_USER, only if Shelly is powered with 12V DC
     0,                // GPIO01 Serial RXD - Can be changed to GPIO_USER, only if Shelly is powered with 12V DC
     0,
     0,                // GPIO03 Serial TXD - Can be changed to GPIO_USER, only if Shelly is powered with 12V DC
     PIGPIO_N(GPIO_REL,1),           // GPIO04 Relay (0 = Off, 1 = On)
     PIGPIO_N(GPIO_SWT_NP,1),        // GPIO05 SW pin
                       // GPIO06 (SD_CLK   Flash)
                       // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                       // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
                       // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
                       // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                       // GPIO11 (SD_CMD   Flash)
     0, 0, 0, 0, 0, 0
  },
  #endif
  #ifdef USE_MODULE_TEMPLATE_SHELLY_2P5  
  {//MODULE_SHELLY2P5     // Shelly2 (ESP8266 - 2MB) - https://shelly.cloud/shelly2/
    PIGPIO_N(GPIO_LED_INV,1),        // GPIO00 LED1i
    0,                    // GPIO01 None
    PIGPIO_N(GPIO_KEY,1),            // GPIO02 Button1
    0,                    // GPIO03 None
    PIGPIO_N(GPIO_REL,1),            // GPIO04 Relay1
    PIGPIO_N(GPIO_SWT_NP,2),         // GPIO05 Switch2n
                          // GPIO06 (SD_CLK   Flash)
                          // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                          // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
                          // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
                          // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                          // GPIO11 (SD_CMD   Flash)
    GPIO_I2C_SDA,         // GPIO12 I2C_SDA
    PIGPIO_N(GPIO_SWT_NP,1),         // GPIO13 Switch1n
    GPIO_I2C_SCL,         // GPIO14 I2C_SCL
    PIGPIO_N(GPIO_REL,2),            // GPIO15 Relay2
    GPIO_ADE7953_IRQ,     // GPIO16 ADE7953_IRQ
    GPIO_FLAG_ADC0_TEMP   // GPIOA0 Tempurature
  },
  #endif
  #ifdef USE_MODULE_TEMPLATE_SHELLY_DIMMER2
  {//MODULE_SHELLY_DIMMER2     // SHELLY_DIMMER2 - Shelly Dimmer 2 (ESP8285)
    0,                         // GPIO00 None
    GPIO_HWSERIAL_TX,         // GPIO01 ESP_TXD Serial RXD connection to Dimmer MCU
    0,                         // GPIO02 None 
    GPIO_HWSERIAL_RX,         // GPIO03 ESP_RXD Serial TXD connection to Dimmer MCU
    GPIO_SHELLY2_SHD_BOOT0,    // GPIO04 SHD Boot 0 
    GPIO_SHELLY2_SHD_RESET_INV,    // GPIO05 SHD Reset
                               // GPIO06 (SD_CLK   Flash)
                               // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                               // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
                               // GPIO09 None 
                               // GPIO10 None 
                               // GPIO11 (SD_CMD   Flash)
    PIGPIO_N(GPIO_SWT_NP,2),              // GPIO12 Switch2n 
    0,                         // GPIO13 None 
    PIGPIO_N(GPIO_SWT_NP,1),              // GPIO14 Switch1n 
    0,                         // GPIO15 None
    PIGPIO_N(GPIO_LED_INV,1),             // GPIO16 Led1i 
    GPIO_FLAG_ADC0_TEMP        // ADC Temperature
  },
  #endif
};


/*********************************************************************************************\
 * Templates with 14 usable pins (ESP8285) : gpio9/10 are available on ESP8285, but not on ESP8266
\*********************************************************************************************/
const mytmplt8285 mPins::module_template__gpio_map_ESP8285[3] PROGMEM = {
  {                           // MODULE_GENERIC_ID         // Generic option for ESP8285 (And esp8266 as fallback)
    GPIO_USER,                // GPIO00 D3 Wemos Button Shield
    GPIO_USER,                // GPIO01 TX Serial RXD
    GPIO_USER,                // GPIO02 D4 Wemos DHT Shield
    GPIO_USER,                // GPIO03 RX Serial TXD and Optional sensor
    GPIO_USER,                // GPIO04 D2 Wemos I2C SDA
    GPIO_USER,                // GPIO05 D1 Wemos I2C SCL / Wemos Relay Shield (0 = Off, 1 = On) / Wemos WS2812B RGB led Shield
                              // GPIO06 (SD_CLK   Flash)
                              // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                              // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    GPIO_USER,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    GPIO_USER,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                              // GPIO11 (SD_CMD   Flash)
    GPIO_USER,                // GPIO12 D6
    GPIO_USER,                // GPIO13 D7
    GPIO_USER,                // GPIO14 D5
    GPIO_USER,                // GPIO15 D8
    GPIO_USER,                // GPIO16 D0 Wemos Wake
    GPIO_FLAG_ADC0_ID         // ADC0 A0 Analog input
  },
  #ifdef USE_MODULE_TEMPLATE_SONOFF_IFAN03
  {          //MODULE_SONOFF_IFAN03_ID                  // SONOFF_IFAN03 - Sonoff iFan03 (ESP8285)
    PIGPIO_N(GPIO_KEY,1),           // GPIO00 WIFI_KEY0 Button 1
    PIGPIO_N(GPIO_HWSERIAL_TX,1),   // GPIO01 ESP_TXD Serial RXD connection to P0.5 of RF microcontroller
    0,                              // GPIO02 ESP_LOG
    PIGPIO_N(GPIO_HWSERIAL_RX,1),   // GPIO03 ESP_RXD Serial TXD connection to P0.4 of RF microcontroller
    0,                              // GPIO04 DEBUG_RX
    0,                              // GPIO05 DEBUG_TX
                                    // GPIO06 (SD_CLK   Flash)
                                    // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                                    // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    PIGPIO_N(GPIO_REL_INV,1),       // GPIO09 WIFI_O0 Relay 1 (0 = Off, 1 = On) controlling the light
    PIGPIO_N(GPIO_BUZZER_INV,1),    // GPIO10 WIFI_O4 Buzzer (0 = Off, 1 = On)
                                    // GPIO11 (SD_CMD   Flash)
    PIGPIO_N(GPIO_REL,3),           // GPIO12 WIFI_O2 Relay 3 (0 = Off, 1 = On) controlling the fan
    PIGPIO_N(GPIO_LED_INV,1),       // GPIO13 WIFI_CHK Blue Led on PCA (0 = On, 1 = Off) - Link and Power status
    PIGPIO_N(GPIO_REL,2),           // GPIO14 WIFI_O1 Relay 2 (0 = Off, 1 = On) controlling the fan
    PIGPIO_N(GPIO_REL,4),           // GPIO15 WIFI_O3 Relay 4 (0 = Off, 1 = On) controlling the fan
    0,                              // GPIO16 None 
    0                               // A0
  },
  #endif
  #ifdef USE_MODULE_TEMPLATE_SONOFF_4CHPRO  
  {                           // SONOFF_4CH - Sonoff 4CH (ESP8285)
    PIGPIO_N(GPIO_KEY,1),             // GPIO00 Button 1
    GPIO_USER,             // GPIO01 Serial RXD and Optional sensor
    GPIO_RF_433MHZ_RX,             // GPIO02 Optional sensor
    GPIO_USER,             // GPIO03 Serial TXD and Optional sensor
    PIGPIO_N(GPIO_REL,3),             // GPIO04 Sonoff 4CH Red Led and Relay 3 (0 = Off, 1 = On)
    PIGPIO_N(GPIO_REL,2),             // GPIO05 Sonoff 4CH Red Led and Relay 2 (0 = Off, 1 = On)
                        // GPIO06 (SD_CLK   Flash)
                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    PIGPIO_N(GPIO_KEY,2),             // GPIO09 Button 2
    PIGPIO_N(GPIO_KEY,3),             // GPIO10 Button 3
                        // GPIO11 (SD_CMD   Flash)
    PIGPIO_N(GPIO_REL,1),             // GPIO12 Red Led and Relay 1 (0 = Off, 1 = On) - Link and Power status
    PIGPIO_N(GPIO_LED_INV,1),         // GPIO13 Blue Led (0 = On, 1 = Off)
    PIGPIO_N(GPIO_KEY,4),             // GPIO14 Button 4
    PIGPIO_N(GPIO_REL,4),             // GPIO15 Red Led and Relay 4 (0 = Off, 1 = On)
    0, 0
  },
  #endif //  USE_MODULE_TEMPLATE_SONOFF_4CHPRO

};


#endif // ESP8266 ==========================================================================================================


#ifdef ESP32 // ESP32 ==========================================================================================================

#if CONFIG_IDF_TARGET_ESP32C2

  /********************************************************************************************\
   * ESP32-C2 Module templates
  \********************************************************************************************/

  #define USER_MODULE        255

  // Supported hardware modules
  enum SupportedModulesESP32C2 {
    WEMOS, // not really correct, a placeholder for now
    MAXMODULE };

  const uint8_t kModuleNiceList[] PROGMEM = {
    WEMOS,
  };

  const char kModuleNames[] PROGMEM =
    "ESP32C2|"
    ;

  const mytmplt kModules[] PROGMEM = {
    {                              // Generic ESP32C2 device
      GPIO_USER,               // 0       IO                  GPIO0, ADC1_CH0,  RTC
      GPIO_USER,               // 1       IO                  GPIO1, ADC1_CH1,  RTC
      GPIO_USER,               // 2       IO                  GPIO2, ADC1_CH2,  RTC
      GPIO_USER,               // 3       IO                  GPIO3, ADC1_CH3,  RTC 
      GPIO_USER,               // 4       IO                  GPIO4, ADC1_CH4,  RTC 
      GPIO_USER,               // 5       IO                  GPIO5, RTC 
      GPIO_USER,               // 6       IO                  GPIO6,
      GPIO_USER,               // 7       IO                  GPIO7, 
      GPIO_USER,               // 8       IO                  GPIO8, Strapping
      GPIO_USER,               // 9       IO                  GPIO9, Strapping
      GPIO_USER,               // 10      IO                  GPIO10
      0,                       // 11      IO                  GPIO11, output power supply for flash
      0,                       // 12      IO                  GPIO12, SPIHD - Free if flash DIO/DOUT
      0,                       // 13      IO                  GPIO13, SPIWP - Free if flash DIO/DOUT
      0,                       // 14      IO                  GPIO14, SPICS0
      0,                       // 15      IO                  GPIO15, SPICLK
      0,                       // 16      IO                  GPIO16, SPID
      0,                       // 17      IO                  GPIO17, SPIQ
      GPIO_USER,               // 18      IO                  GPIO18,
      GPIO_USER,               // 19      IO     RXD0         GPIO19, U0RXD
      GPIO_USER,               // 20      IO     TXD0         GPIO20, U0TXD
      0                        // Flag
    },
  };

#elif CONFIG_IDF_TARGET_ESP32C3

  /********************************************************************************************\
   * ESP32-C3 Module templates
  \********************************************************************************************/
  
  DEFINE_PGM_CTR(PM__MODULE_TEMPLATE__GENERIC__CTR)        D__MODULE_TEMPLATE__GENERIC__CTR;

  const char mPins::module_template__names[] PROGMEM =
    "ESP32C3|"
    ;

  const uint8_t mPins::module_template__ids[] PROGMEM = {
    MODULE_GENERIC
  };

    // !!! Update this list in the same order as SupportedModulesESP32C3 !!!
    const mytmplt mPins::module_template__gpio_map[] PROGMEM = {
      {                              // Generic ESP32C3 device
        GPIO_USER,              // 0       IO                  GPIO0, ADC1_CH0, XTAL_32K_P
        GPIO_USER,              // 1       IO                  GPIO1, ADC1_CH1, XTAL_32K_N
        GPIO_USER,              // 2       IO                  GPIO2, ADC1_CH2, FSPIQ
        GPIO_USER,              // 3       IO                  GPIO3, ADC1_CH3
        GPIO_USER,              // 4       IO                  GPIO4, ADC1_CH4, FSPIHD, MTMS
        GPIO_USER,              // 5       IO                  GPIO5, ADC2_CH0, FSPIWP, MTDI
        GPIO_USER,              // 6       IO                  GPIO6, FSPICLK, MTCK
        GPIO_USER,              // 7       IO                  GPIO7, FSPID, MTDO
        GPIO_USER,              // 8       IO                  GPIO8
        GPIO_USER,              // 9       IO                  GPIO9
        GPIO_USER,              // 10      IO                  GPIO10
        0,                           // 11      IO                  GPIO11, output power supply for flash
        0,                           // 12      IO                  GPIO12, SPIHD - Free if flash DIO/DOUT
        0,                           // 13      IO                  GPIO13, SPIWP - Free if flash DIO/DOUT
        0,                           // 14      IO                  GPIO14, SPICS0
        0,                           // 15      IO                  GPIO15, SPICLK
        0,                           // 16      IO                  GPIO16, SPID
        0,                           // 17      IO                  GPIO17, SPIQ
        GPIO_USER,              // 18      IO                  GPIO18, USB_D
        GPIO_USER,              // 19      IO                  GPIO19, USB_D+
        GPIO_USER,              // 20      IO     RXD0         GPIO20, U0RXD
        GPIO_USER,              // 21      IO     TXD0         GPIO21, U0TXD
        0                            // Flag
      },
    };


#elif CONFIG_IDF_TARGET_ESP32C6

  /********************************************************************************************\
   * ESP32-C6 Module templates
  \********************************************************************************************/

  #define USER_MODULE        255

  // Supported hardware modules
  enum SupportedModulesESP32C6 {
    WEMOS, // not really correct, a placeholder for now
    MAXMODULE };

  // Default module settings
  const uint8_t kModuleNiceList[] PROGMEM = {
    WEMOS,
  };

  // !!! Update this list in the same order as kModuleNiceList !!!
  const char kModuleNames[] PROGMEM =
    "ESP32C6|"
    ;

  // !!! Update this list in the same order as SupportedModulesESP32C6 !!!
  const mytmplt kModules[] PROGMEM = {
    {                              // Generic ESP32C6 device
      GPIO_USER,               // 0       IO                  GPIO0, ADC1_CH0, LP_GPIO0
      GPIO_USER,               // 1       IO                  GPIO1, ADC1_CH1, LP_GPIO1
      GPIO_USER,               // 2       IO                  GPIO2, ADC1_CH2, LP_GPIO2
      GPIO_USER,               // 3       IO                  GPIO3, ADC1_CH3, LP_GPIO3 
      GPIO_USER,               // 4       IO                  GPIO4, ADC1_CH4, LP_GPIO4, Strapping
      GPIO_USER,               // 5       IO                  GPIO5, ADC1_CH5, LP_GPIO5, Strapping
      GPIO_USER,               // 6       IO                  GPIO6, ADC1_CH6, LP_GPIO6
      GPIO_USER,               // 7       IO                  GPIO7,           LP_GPIO7
      GPIO_USER,               // 8       IO                  GPIO8, Strapping
      GPIO_USER,               // 9       IO                  GPIO9, Strapping
      GPIO_USER,               // 10      IO                  GPIO10 (QFN40 only)
      GPIO_USER,               // 11      IO                  GPIO11 (QFN40 only)
      GPIO_USER,               // 12      IO                  GPIO12, USB-JTAG
      GPIO_USER,               // 13      IO                  GPIO13, USB-JTAG
      GPIO_USER,               // 14      IO                  GPIO14 (QFN32 only)
      GPIO_USER,               // 15      IO                  GPIO15, Strapping
      GPIO_USER,               // 16      IO     TXD0         GPIO16, U0TXD
      GPIO_USER,               // 17      IO     RXD0         GPIO17, U0RXD
      GPIO_USER,               // 18      IO                  GPIO18, SDIO_CMD
      GPIO_USER,               // 19      IO                  GPIO19, SDIO_CLK
      GPIO_USER,               // 20      IO                  GPIO20, SDIO_DATA0
      GPIO_USER,               // 21      IO                  GPIO21, SDIO_DATA1
      GPIO_USER,               // 22      IO                  GPIO22, SDIO_DATA2
      GPIO_USER,               // 23      IO                  GPIO23, SDIO_DATA3
      0,                       // 24      IO                  GPIO24, SPICS1, PSRAM
      0,                       // 25      IO                  GPIO25, SPIQ
      0,                       // 26      IO                  GPIO26, SPIWP - Free if flash DIO/DOUT
      0,                       // 27      IO                  GPIO27, SPIVDD
      0,                       // 28      IO                  GPIO28, SPIHD - Free if flash DIO/DOUT
      0,                       // 29      IO                  GPIO29, SPICLK
      0,                       // 30      IO                  GPIO30, SPID
      0                        // Flag
    },
  };

  /*********************************************************************************************\
   Known templates
  \*********************************************************************************************/

#elif CONFIG_IDF_TARGET_ESP32S2

  /********************************************************************************************\
   * ESP32-S2 Module templates
  \********************************************************************************************/

  #define USER_MODULE        255

  enum SupportedModulesESP32S2 {
    WEMOS,
    MAXMODULE };

  const uint8_t kModuleNiceList[] PROGMEM = {
    WEMOS,
  };

  const char kModuleNames[] PROGMEM =
    "ESP32S2|"
    ;

  const mytmplt kModules[] PROGMEM = {
    {                              // Generic ESP32C3 device
      GPIO_USER,               // 0       IO                  GPIO0, RTC_GPIO0, Strapping
      GPIO_USER,               // 1       AO                  GPIO1, ADC1_CH0, RTC_GPIO1
      GPIO_USER,               // 2       AO                  GPIO2, ADC1_CH1, RTC_GPIO2
      GPIO_USER,               // 3       AO                  GPIO3, ADC1_CH2, RTC_GPIO3
      GPIO_USER,               // 4       AO                  GPIO4, ADC1_CH3, RTC_GPIO4
      GPIO_USER,               // 5       AO                  GPIO5, ADC1_CH4, RTC_GPIO5
      GPIO_USER,               // 6       AO                  GPIO6, ADC1_CH5, RTC_GPIO6
      GPIO_USER,               // 7       AO                  GPIO7, ADC1_CH6, RTC_GPIO7
      GPIO_USER,               // 8       AO                  GPIO8, ADC1_CH7, RTC_GPIO8
      GPIO_USER,               // 9       AO                  GPIO9, ADC1_CH8, RTC_GPIO9
      GPIO_USER,               // 10      AO                  GPIO10, ADC1_CH9, RTC_GPIO10
      GPIO_USER,               // 11      AO                  GPIO11, ADC2_CH0, RTC_GPIO11
      GPIO_USER,               // 12      AO                  GPIO12, ADC2_CH1, RTC_GPIO12
      GPIO_USER,               // 13      AO                  GPIO13, ADC2_CH2, RTC_GPIO13
      GPIO_USER,               // 14      AO                  GPIO14, ADC2_CH3, RTC_GPIO14
      GPIO_USER,               // 15      AO                  GPIO15, ADC2_CH4, RTC_GPIO15, XTAL_32K_P
      GPIO_USER,               // 16      AO                  GPIO16, ADC2_CH5, RTC_GPIO16, XTAL_32K_N
      GPIO_USER,               // 17      AO                  GPIO17, ADC2_CH6, RTC_GPIO17, DAC_1
      GPIO_USER,               // 18      AO                  GPIO18, ADC2_CH7, RTC_GPIO18, DAC_2
      GPIO_USER,               // 19      AO                  GPIO19, ADC2_CH8, RTC_GPIO19
      GPIO_USER,               // 20      AO                  GPIO20, ADC2_CH9, RTC_GPIO20
      GPIO_USER,               // 21      IO                  GPIO21, RTC_GPIO21
                               // 22      --                  Unused
                               // 23      --                  Unused
                               // 24      --                  Unused
                               // 25      --                  Unused
                               // 26      FL                  SPICS1, PSRAM
                               // 27      FL                  SPIHD
                               // 28      FL                  SPIWP
                               // 29      FL                  SPICS0
                               // 30      FL                  SPICLK
                               // 31      FL                  SPIQ
                               // 32      FL                  SPID
      GPIO_USER,               // 33      IO                  GPIO33
      GPIO_USER,               // 34      IO                  GPIO34
      GPIO_USER,               // 35      IO                  GPIO35
      GPIO_USER,               // 36      IO                  GPIO36
      GPIO_USER,               // 37      IO                  GPIO37
      GPIO_USER,               // 38      IO                  GPIO38
      GPIO_USER,               // 39      IO                  GPIO39, JTAG MTCK
      GPIO_USER,               // 40      IO                  GPIO40, JTAG MTDO
      GPIO_USER,               // 41      IO                  GPIO41, JTAG MTDI
      GPIO_USER,               // 42      IO                  GPIO42, JTAG MTMS
      GPIO_USER,               // 43      IO                  GPIO43, U0TXD
      GPIO_USER,               // 44      IO                  GPIO44, U0RXD
      GPIO_USER,               // 45      IO                  GPIO45, Strapping
      GPIO_USER,               // 46      I                   GPIO46, Input only, Strapping
      0                            // Flag
    },
  };

#elif CONFIG_IDF_TARGET_ESP32S3

  /********************************************************************************************\
   * ESP32-S3 Module templates
  \********************************************************************************************/

  #define USER_MODULE        255

  #ifdef USE_MODULE_TEMPLATE__CAMERA_XIAO_ESP32S3_SENSE
    DEFINE_PGM_CTR(PM__MODULE_TEMPLATE__CAMERA_XIAO_ESP32S3_SENSE__CTR)        D__MODULE_TEMPLATE__CAMERA_XIAO_ESP32S3_SENSE__CTR;
  #endif

  const char mPins::module_template__names[] PROGMEM =
    "ESP32S3|"
      #ifdef USE_MODULE_TEMPLATE__CAMERA_XIAO_ESP32S3_SENSE
      D__MODULE_TEMPLATE__CAMERA_XIAO_ESP32S3_SENSE__CTR "|"
      #endif
    ;

  const uint8_t mPins::module_template__ids[] PROGMEM = {
    MODULE_GENERIC,
    #ifdef USE_MODULE_TEMPLATE__CAMERA_XIAO_ESP32S3_SENSE
    MODULE__CAMERA_XIAO_ESP32S3_SENSE__ID,
    #endif
  };

  const mytmplt mPins::module_template__gpio_map[] PROGMEM = {
    {                              // Generic ESP32C3 device
      GPIO_USER,               // 0       IO                  RTC_GPIO0, GPIO0, Strapping
      GPIO_USER,               // 1       AO                  RTC_GPIO1, GPIO1, TOUCH1, ADC1_CH0
      GPIO_USER,               // 2       AO                  RTC_GPIO2, GPIO2, TOUCH2, ADC1_CH1
      GPIO_USER,               // 3       AO                  RTC_GPIO3, GPIO3, TOUCH3, ADC1_CH2, Strapping
      GPIO_USER,               // 4       AO                  RTC_GPIO4, GPIO4, TOUCH4, ADC1_CH3
      GPIO_USER,               // 5       AO                  RTC_GPIO5, GPIO5, TOUCH5, ADC1_CH4
      GPIO_USER,               // 6       AO                  RTC_GPIO6, GPIO6, TOUCH6, ADC1_CH5
      GPIO_USER,               // 7       AO                  RTC_GPIO7, GPIO7, TOUCH7, ADC1_CH6
      GPIO_USER,               // 8       AO                  RTC_GPIO8, GPIO8, TOUCH8, ADC1_CH7, SUBSPICS1
      GPIO_USER,               // 9       AO                  RTC_GPIO9, GPIO9, TOUCH9, ADC1_CH8, SUBSPIHD, FSPIHD
      GPIO_USER,               // 10      AO                  RTC_GPIO10, GPIO10, TOUCH10, ADC1_CH9, FSPIIO4, SUBSPICS0, FSPICS0
      GPIO_USER,               // 11      AO                  RTC_GPIO11, GPIO11, TOUCH11, ADC2_CH0, FSPIIO5, SUBSPID, FSPID
      GPIO_USER,               // 12      AO                  RTC_GPIO12, GPIO12, TOUCH12, ADC2_CH1, FSPIIO6, SUBSPICLK, FSPICLK
      GPIO_USER,               // 13      AO                  RTC_GPIO13, GPIO13, TOUCH13, ADC2_CH2, FSPIIO7, SUBSPIQ, FSPIQ
      GPIO_USER,               // 14      AO                  RTC_GPIO14, GPIO14, TOUCH14, ADC2_CH3, FSPIDQS, SUBSPIWP, FSPIWP
      GPIO_USER,               // 15      AO                  RTC_GPIO15, GPIO15, U0RTS, ADC2_CH4, XTAL_32K_P
      GPIO_USER,               // 16      AO                  RTC_GPIO16, GPIO16, U0CTS, ADC2_CH5, XTAL_32K_N
      GPIO_USER,               // 17      AO                  RTC_GPIO17, GPIO17, U1TXD, ADC2_CH6
      GPIO_USER,               // 18      AO                  RTC_GPIO18, GPIO18, U1RXD, ADC2_CH7, CLK_OUT3
      GPIO_USER,               // 19      AO                  RTC_GPIO19, GPIO19, U1RTS, ADC2_CH8, CLK_OUT2, USB_D­
      GPIO_USER,               // 20      AO                  RTC_GPIO20, GPIO20, U1CTS, ADC2_CH9, CLK_OUT1, USB_D+
      GPIO_USER,               // 21      IO                  RTC_GPIO21, GPIO21
                               // 22      --                  Unused
                               // 23      --                  Unused
                               // 24      --                  Unused
                               // 25      --                  Unused
                               // 26      FL                  SPICS1, GPIO26
                               // 27      FL                  SPIHD, GPIO27
                               // 28      FL                  SPIWP, GPIO28
                               // 29      FL                  SPICS0, GPIO29
                               // 30      FL                  SPICLK, GPIO30
                               // 31      FL                  SPIQ, GPIO31
                               // 32      FL                  SPID, GPIO32
      GPIO_NONE,               // 33      IO                  SPIIO4, GPIO33, FSPIHD, SUBSPIHD
      GPIO_NONE,               // 34      IO                  SPIIO5, GPIO34, FSPICS0, SUBSPICS0
      GPIO_NONE,               // 35      IO                  SPIIO6, GPIO35, FSPID, SUBSPID
      GPIO_NONE,               // 36      IO                  SPIIO7, GPIO36, FSPICLK, SUBSPICLK
      GPIO_NONE,               // 37      IO                  SPIDQS, GPIO37, FSPIQ, SUBSPIQ
      GPIO_USER,               // 38      IO                  GPIO38, FSPIWP, SUBSPIWP
      GPIO_USER,               // 39      IO                  MTCK, GPIO39, CLK_OUT3, SUBSPICS1
      GPIO_USER,               // 40      IO                  MTDO, GPIO40, CLK_OUT2
      GPIO_USER,               // 41      IO                  MTDI, GPIO41, CLK_OUT1
      GPIO_USER,               // 42      IO                  MTMS, GPIO42
      GPIO_USER,               // 43      IO                  U0TXD, GPIO43, CLK_OUT1
      GPIO_USER,               // 44      IO                  U0RXD, GPIO44, CLK_OUT2
      GPIO_USER,               // 45      IO                  GPIO45, Strapping
      GPIO_USER,               // 46      IO                  GPIO46, Strapping
      GPIO_USER,               // 47      IO                  SPICLK_P_DIFF, GPIO47, SUBSPICLK_P_DIFF
      GPIO_USER,               // 48      IO                  SPICLK_N_DIFF, GPIO48, SUBSPICLK_N_DIFF
      0                            // Flag
    },
    #ifdef USE_MODULE_TEMPLATE__CAMERA_XIAO_ESP32S3_SENSE
    {
      0,                            // GPIO0   — Not exposed  
      0,                            // GPIO1   — Not exposed  
      GPIO_USER,                    // GPIO2   — Exposed (D0)  
      GPIO_USER,                    // GPIO3   — Exposed (D1)  
      GPIO_USER,                    // GPIO4   — Exposed (D2)  
      GPIO_USER,                    // GPIO5   — Exposed (D3)  
      GPIO_USER,                    // GPIO6   — Exposed (D4 / SDA)  
      GPIO_USER,                    // GPIO7   — Exposed (D5 / SCL)  
      0,                            // GPIO8   — Not exposed  
      0,                            // GPIO9   — Not exposed  
      GPIO_WEBCAM_XCLK,             // GPIO10  — CAM_XCLK  
      GPIO_WEBCAM_DATA7,            // GPIO11  — CAM_Y8  
      GPIO_WEBCAM_DATA6,            // GPIO12  — CAM_Y7  
      GPIO_WEBCAM_PCLK,             // GPIO13  — CAM_PCLK  
      GPIO_WEBCAM_DATA5,            // GPIO14  — CAM_Y6  
      GPIO_WEBCAM_DATA1,            // GPIO15  — CAM_Y2  
      GPIO_WEBCAM_DATA4,            // GPIO16  — CAM_Y5  
      GPIO_WEBCAM_DATA2,            // GPIO17  — CAM_Y3  
      GPIO_WEBCAM_DATA3,            // GPIO18  — CAM_Y4  
      0,                            // GPIO19  — Not exposed  
      GPIO_USER,                    // GPIO20  — Exposed (RX)  
      GPIO_USER,                    // GPIO21  — Exposed (TX)  
                                    // GPIO22  — Not exposed  
                                    // GPIO23  — Not exposed  
                                    // GPIO24  — Not exposed  
                                    // GPIO25  — Not exposed  
                                    // GPIO26  — Not exposed  
                                    // GPIO27  — Not exposed  
                                    // GPIO28  — Not exposed  
                                    // GPIO29  — Not exposed  
                                    // GPIO30  — Not exposed  
                                    // GPIO31  — Not exposed  
                                    // GPIO32  — Not exposed  
      0,                            // GPIO33  — Not exposed  
      0,                            // GPIO34  — Not exposed  
      0,                            // GPIO35  — Not exposed  
      0,                            // GPIO36  — Not exposed  
      0,                            // GPIO37  — Not exposed  
      GPIO_WEBCAM_VSYNC,            // GPIO38  — CAM_VSYNC  
      GPIO_WEBCAM_SIOC,             // GPIO39  — CAM_SIOC  
      GPIO_WEBCAM_SIOD,             // GPIO40  — CAM_SIOD  
      0,                            // GPIO41  — Not exposed  
      0,                            // GPIO42  — Not exposed  
      0,                            // GPIO43  — Not exposed  
      0,                            // GPIO44  — Not exposed  
      0,                            // GPIO45  — Not exposed  
      0,                            // GPIO46  — Not exposed  
      GPIO_WEBCAM_HREF,             // GPIO47  — CAM_HREF  
      GPIO_WEBCAM_DATA8,            // GPIO48  — CAM_Y9  
      0                             // FLAG  — Not exposed
    },
    #endif  // USE_MODULE_TEMPLATE__CAMERA_XIAO_ESP32S3_SENSE
  };

#else  // not CONFIG_IDF_TARGET_ESP32C2/C3/C6 nor CONFIG_IDF_TARGET_ESP32S2 - ESP32

  /********************************************************************************************\
   * ESP32 Module templates
  \********************************************************************************************/

  #define USER_MODULE        255

  DEFINE_PGM_CTR(PM_MODULE_NAME__ESP32_DEVKIT__CTR)        D_MODULE_NAME__ESP32_DEVKIT__CTR;
  #ifdef USE_MODULE_TEMPLATE_CAM_AITHINKER
    DEFINE_PGM_CTR(PM_MODULE_NAME_CAM_AITHINKER_CTR)        D_MODULE_NAME_CAM_AITHINKER_CTR;
  #endif
  #ifdef USE_MODULE_TEMPLATE__CAMERA_FREENOVE_WROOVER
    DEFINE_PGM_CTR(PM__MODULE_TEMPLATE__CAMERA_FREENOVE_WROOVER__CTR)        D__MODULE_TEMPLATE__CAMERA_FREENOVE_WROOVER__CTR;
  #endif

  // Default module settings
  const uint16_t mPins::module_template__ids[] PROGMEM = {
    MODULE_DOIT_DEVKIT,
    #ifdef USE_MODULE_TEMPLATE_CAM_AITHINKER
    MODULE_CAM_AITHINKER_ID,
    #endif
    #ifdef USE_MODULE_TEMPLATE__CAMERA_FREENOVE_WROOVER
    MODULE__CAMERA_FREENOVE_WROOVER__ID,
    #endif
  };

  const char mPins::module_template__names[] PROGMEM =
    D_MODULE_NAME__ESP32_DEVKIT__CTR "|"
    #ifdef USE_MODULE_TEMPLATE_CAM_AITHINKER
    D_MODULE_NAME_CAM_AITHINKER_CTR "|"
    #endif 
    #ifdef USE_MODULE_TEMPLATE__CAMERA_FREENOVE_WROOVER
    D__MODULE_TEMPLATE__CAMERA_FREENOVE_WROOVER__CTR "|"
    #endif
  ;

  const mytmplt mPins::module_template__gpio_map[MAXMODULE - MODULE_DOIT_DEVKIT] PROGMEM = {
    {                              // WEMOS - Espressif ESP32-DevKitC - Any ESP32 device like WeMos and NodeMCU hardware (ESP32)
      GPIO_USER,               // 0       (I)O                GPIO0, ADC2_CH1, TOUCH1, RTC_GPIO11, CLK_OUT1, EMAC_TX_CLK
      GPIO_USER,               // 1       IO     TXD0         GPIO1, U0TXD, CLK_OUT3, EMAC_RXD2
      GPIO_USER,               // 2       IO                  GPIO2, ADC2_CH2, TOUCH2, RTC_GPIO12, HSPIWP, HS2_DATA0, SD_DATA0
      GPIO_USER,               // 3       IO     RXD0         GPIO3, U0RXD, CLK_OUT2
      GPIO_USER,               // 4       IO                  GPIO4, ADC2_CH0, TOUCH0, RTC_GPIO10, HSPIHD, HS2_DATA1, SD_DATA1, EMAC_TX_ER
      GPIO_USER,               // 5       IO                  GPIO5, VSPICS0, HS1_DATA6, EMAC_RX_CLK
                              // 6       IO                  GPIO6, Flash CLK
                              // 7       IO                  GPIO7, Flash D0
                              // 8       IO                  GPIO8, Flash D1
      GPIO_USER,               // 9       IO                  GPIO9, Flash D2, U1RXD
      GPIO_USER,               // 10      IO                  GPIO10, Flash D3, U1TXD
                              // 11      IO                  GPIO11, Flash CMD
      GPIO_USER,               // 12      (I)O                GPIO12, ADC2_CH5, TOUCH5, RTC_GPIO15, MTDI, HSPIQ, HS2_DATA2, SD_DATA2, EMAC_TXD3       (If driven High, flash voltage (VDD_SDIO) is 1.8V not default 3.3V. Has internal pull-down, so unconnected = Low = 3.3V. May prevent flashing and/or booting if 3.3V flash is connected and pulled high. See ESP32 datasheet for more details.)
      GPIO_USER,               // 13      IO                  GPIO13, ADC2_CH4, TOUCH4, RTC_GPIO14, MTCK, HSPID, HS2_DATA3, SD_DATA3, EMAC_RX_ER
      GPIO_USER,               // 14      IO                  GPIO14, ADC2_CH6, TOUCH6, RTC_GPIO16, MTMS, HSPICLK, HS2_CLK, SD_CLK, EMAC_TXD2
      GPIO_USER,               // 15      (I)O                GPIO15, ADC2_CH3, TOUCH3, MTDO, HSPICS0, RTC_GPIO13, HS2_CMD, SD_CMD, EMAC_RXD3         (If driven Low, silences boot messages from normal boot. Has internal pull-up, so unconnected = High = normal output.)
      GPIO_USER,               // 16      IO                  GPIO16, HS1_DATA4, U2RXD, EMAC_CLK_OUT
      GPIO_USER,               // 17      IO                  GPIO17, HS1_DATA5, U2TXD, EMAC_CLK_OUT_180
      GPIO_USER,               // 18      IO                  GPIO18, VSPICLK, HS1_DATA7
      GPIO_USER,               // 19      IO                  GPIO19, VSPIQ, U0CTS, EMAC_TXD0
      0,                       // 20
      GPIO_USER,               // 21      IO                  GPIO21, VSPIHD, EMAC_TX_EN
      GPIO_USER,               // 22      IO      LED         GPIO22, VSPIWP, U0RTS, EMAC_TXD1
      GPIO_USER,               // 23      IO                  GPIO23, VSPID, HS1_STROBE
      0,                       // 24
      GPIO_USER,               // 25      IO                  GPIO25, DAC_1, ADC2_CH8, RTC_GPIO6, EMAC_RXD0
      GPIO_USER,               // 26      IO                  GPIO26, DAC_2, ADC2_CH9, RTC_GPIO7, EMAC_RXD1
      GPIO_USER,               // 27      IO                  GPIO27, ADC2_CH7, TOUCH7, RTC_GPIO17, EMAC_RX_DV
      0,                       // 28
      0,                       // 29
      0,                       // 30
      0,                       // 31
      GPIO_USER,               // 32      IO                  GPIO32, XTAL_32K_P (32.768 kHz crystal oscillator input), ADC1_CH4, TOUCH9, RTC_GPIO9
      GPIO_USER,               // 33      IO                  GPIO33, XTAL_32K_N (32.768 kHz crystal oscillator output), ADC1_CH5, TOUCH8, RTC_GPIO8
      GPIO_USER,               // 34      I   NO PULLUP       GPIO34, ADC1_CH6, RTC_GPIO4
      GPIO_USER,               // 35      I   NO PULLUP       GPIO35, ADC1_CH7, RTC_GPIO5
      GPIO_USER,               // 36      I   NO PULLUP       GPIO36, SENSOR_VP, ADC_H, ADC1_CH0, RTC_GPIO0
      0,                       // 37          NO PULLUP
      0,                       // 38          NO PULLUP
      GPIO_USER,               // 39      I   NO PULLUP       GPIO39, SENSOR_VN, ADC1_CH3, ADC_H, RTC_GPIO3
      0                        // Flag
    },
    #ifdef USE_MODULE_TEMPLATE_CAM_AITHINKER
    {                              // ESP32_CAM_AITHINKER - Any ESP32 device with webcam (ESP32)
      GPIO_WEBCAM_XCLK,        // 0       (I)O                GPIO0, CAM_XCLK
      GPIO_USER,               // 1       IO     TXD0         GPIO1, U0TXD, CLK_OUT3, EMAC_RXD2
      GPIO_USER,               // 2       IO                  GPIO2, ADC2_CH2, TOUCH2, RTC_GPIO12, HSPIWP, HS2_DATA0, SD_DATA0
      GPIO_USER,               // 3       IO     RXD0         GPIO3, U0RXD, CLK_OUT2
      GPIO_USER,               // 4       IO                  GPIO4, ADC2_CH0, TOUCH0, RTC_GPIO10, HSPIHD, HS2_DATA1, SD_DATA1, EMAC_TX_ER
      GPIO_WEBCAM_DATA1,        // 5       IO                  GPIO5, CAM_DATA1
                                  // 6       IO                  GPIO6, Flash CLK
                                  // 7       IO                  GPIO7, Flash D0
                                  // 8       IO                  GPIO8, Flash D1
      GPIO_USER,               // 9       IO                  GPIO9, Flash D2, U1RXD
      GPIO_USER,               // 10      IO                  GPIO10, Flash D3, U1TXD
                                  // 11      IO                  GPIO11, Flash CMD
      GPIO_USER,               // 12      (I)O                GPIO12, ADC2_CH5, TOUCH5, RTC_GPIO15, MTDI, HSPIQ, HS2_DATA2, SD_DATA2, EMAC_TXD3       (If driven High, flash voltage (VDD_SDIO) is 1.8V not default 3.3V. Has internal pull-down, so unconnected = Low = 3.3V. May prevent flashing and/or booting if 3.3V flash is connected and pulled high. See ESP32 datasheet for more details.)
      GPIO_USER,               // 13      IO                  GPIO13, ADC2_CH4, TOUCH4, RTC_GPIO14, MTCK, HSPID, HS2_DATA3, SD_DATA3, EMAC_RX_ER
      GPIO_USER,               // 14      IO                  GPIO14, ADC2_CH6, TOUCH6, RTC_GPIO16, MTMS, HSPICLK, HS2_CLK, SD_CLK, EMAC_TXD2
      GPIO_USER,               // 15      (I)O                GPIO15, ADC2_CH3, TOUCH3, MTDO, HSPICS0, RTC_GPIO13, HS2_CMD, SD_CMD, EMAC_RXD3         (If driven Low, silences boot messages from normal boot. Has internal pull-up, so unconnected = High = normal output.)
      GPIO_USER,               // 16      IO                  GPIO16, HS1_DATA4, U2RXD, EMAC_CLK_OUT
      GPIO_USER,               // 17      IO                  GPIO17, HS1_DATA5, U2TXD, EMAC_CLK_OUT_180
      GPIO_WEBCAM_DATA2,     // 18      IO                  GPIO18, CAM_DATA2
      GPIO_WEBCAM_DATA3,     // 19      IO                  GPIO19, CAM_DATA3
      0,                           // 20
      GPIO_WEBCAM_DATA4,     // 21      IO                  GPIO21, CAM_DATA4
      GPIO_WEBCAM_PCLK,        // 22      IO      LED         GPIO22, CAM_PCLK
      GPIO_WEBCAM_HREF,        // 23      IO                  GPIO23, CAM_HREF
      0,                           // 24
      GPIO_WEBCAM_VSYNC,       // 25      IO                  GPIO25, CAM_VSYNC
      GPIO_WEBCAM_SIOD,        // 26      IO                  GPIO26, CAM_SIOD
      GPIO_WEBCAM_SIOC,        // 27      IO                  GPIO27, CAM_SIOC
      0,                           // 28
      0,                           // 29
      0,                           // 30
      0,                           // 31
      GPIO_WEBCAM_PWDN,        // 32      IO                  GPIO32, CAM_PWDN
      GPIO_USER,               // 33      IO                  GPIO33, XTAL_32K_N (32.768 kHz crystal oscillator output), ADC1_CH5, TOUCH8, RTC_GPIO8
      GPIO_WEBCAM_DATA7,     // 34      I   NO PULLUP       GPIO34, CAM_DATA7
      GPIO_WEBCAM_DATA8,     // 35      I   NO PULLUP       GPIO35, CAM_DATA8
      GPIO_WEBCAM_DATA5,     // 36      I   NO PULLUP       GPIO36, CAM_DATA5
      0,                           // 37          NO PULLUP
      0,                           // 38          NO PULLUP
      GPIO_WEBCAM_DATA6,     // 39      I   NO PULLUP       GPIO39, CAM_DATA6
      0                            // Flag
    },
    #endif  // USE_MODULE_TEMPLATE_CAM_AITHINKER
    #ifdef USE_MODULE_TEMPLATE__CAMERA_FREENOVE_WROOVER
    {//CAMERA_FREENOVE_WROOVER
      GPIO_USER,                // 0   IO      CAM_XCLK (can be used, but not hardwired on this board)
      GPIO_USER,                // 1   IO      U0TXD, Serial TX
      GPIO_USER,                // 2   IO      ADC2_CH2, Boot strapping pin (be careful)
      GPIO_USER,                // 3   IO      U0RXD, Serial RX
      GPIO_WEBCAM_DATA1,        // 4   IO      CAM_Y2
      GPIO_WEBCAM_DATA2,        // 5   IO      CAM_Y3
                                // 6   IO      Flash CLK (not usable)
                                // 7   IO      Flash D0 (not usable)
                                // 8   IO      Flash D1 (not usable)
      GPIO_USER,                // 9   IO      Flash D2, U1RXD (not usable)
      GPIO_USER,                // 10  IO      Flash D3, U1TXD (not usable)
                                // 11  IO      Flash CMD (not usable)
      GPIO_USER,                // 12  IO      MTDI, strapping pin — avoid using at boot
      GPIO_USER,                // 13  IO      CAM_Y8 / ADC2_CH4 (also SPI)
      GPIO_USER,                // 14  IO      CAM_Y7 / ADC2_CH6
      GPIO_USER,                // 15  IO      CAM_Y6 / ADC2_CH3 / MTDO (strapping, avoid LOW at boot)
      GPIO_USER,                // 16  IO      U2RXD
      GPIO_USER,                // 17  IO      U2TXD
      GPIO_WEBCAM_DATA3,        // 18  IO      CAM_Y4
      GPIO_WEBCAM_DATA4,        // 19  IO      CAM_Y5
      0,                        // 20  N/A     Not exposed
      GPIO_WEBCAM_XCLK,         // 21  IO      CAM_XCLK (must be connected manually)
      GPIO_WEBCAM_PCLK,         // 22  IO      CAM_PCLK
      GPIO_WEBCAM_HREF,         // 23  IO      CAM_HREF
      0,                        // 24  N/A     Not exposed
      GPIO_WEBCAM_VSYNC,        // 25  IO      CAM_VSYNC
      GPIO_WEBCAM_SIOD,         // 26  IO      CAM_SIOD (I2C SDA)
      GPIO_WEBCAM_SIOC,         // 27  IO      CAM_SIOC (I2C SCL)
      0,                        // 28  N/A     Not exposed
      0,                        // 29  N/A     Not exposed
      0,                        // 30  N/A     Not exposed
      0,                        // 31  N/A     Not exposed
      GPIO_USER,                // 32  IO      GPIO32 (can be CAM_PWDN if used)
      GPIO_USER,                // 33  IO      XTAL_32K_N, ADC1_CH5
      GPIO_WEBCAM_DATA7,        // 34  I       CAM_Y9
      GPIO_WEBCAM_DATA8,        // 35  I       CAM_Y8
      GPIO_WEBCAM_DATA5,        // 36  I       CAM_Y6
      0,                        // 37  N/A     Not routed
      0,                        // 38  N/A     Not routed
      GPIO_WEBCAM_DATA6,        // 39  I       CAM_Y7
      0                         // 40  Flag
    },
    #endif  // USE_MODULE_TEMPLATE__CAMERA_FREENOVE_WROOVER

  };

#endif  // ESP32/S2/C3 selection
#endif  // ESP32 ==========================================================================================================


int8_t mPins::Tasker(uint8_t function, JsonParserObject obj)
{

  // DEBUG_PRINT_FUNCTION_NAME_TEST;
  // DEBUG_PRINTF("works=%d\n\r",works);
  
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
      // PinAlloc_Init();
    break;
    case TASK_EVERY_MINUTE:
      //ModuleSettings_ShowTemplateLog();
      //ModuleSettings_ShowActiveTemplate();
    break;
    case TASK_LOOP:{



    }break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
  }

  return TASKER_RESULT__UNKNOWN_ID;

}

void mPins::parse_JSONCommand(JsonParserObject obj)
{

  #define ENABLE_DEBUGFEATURE_JSON_COMMANDS__INIT_COMMANDS

  #ifdef ENABLE_DEBUGFEATURE_JSON_COMMANDS__INIT_COMMANDS // all triggering of normally unsafe commands for telnet debug viewing
  JsonParserToken tok;
  if(tok = obj["Debug"].getObject()["GpioInit"])
  {
    ALOG_WRN(PSTR("GpioInit"));
    GpioInit();
  }

  if (tok = obj["Debug"].getObject()["ShowGPIOTemplates"]) {
    ALOG_WRN(PSTR("ShowGPIOTemplates"));
    char buffer[100];
    uint16_t gpio_buffer[MAX_USER_PINS];

    #ifdef ESP8266
    for (int module_i = 0; module_i < MODULE_MAXMODULE_8266; module_i++) {
      memcpy_P(gpio_buffer, &mPins::module_template__gpio_map_ESP8266[module_i], sizeof(gpio_buffer));
  
      ALOG_INF(PSTR("Module %d:"), module_i);
  
      for (int gpio_i = 0; gpio_i < MAX_USER_PINS; gpio_i++) {
        uint16_t gpio_fn = gpio_buffer[gpio_i];
  
        if (gpio_fn != 0) {
          const char* fn_name = GetGPIOFunctionNamebyID(gpio_fn, buffer, sizeof(buffer));
          ALOG_INF(PSTR("  GPIO%02d = %d (%s)"), gpio_i, gpio_fn, buffer);
        } else {
          ALOG_INF(PSTR("  GPIO%02d = None"), gpio_i);
        }
      }
    }
    #endif
    #ifdef ESP32 
    for (int module_i = 0; module_i < MAXMODULE; module_i++) {
      memcpy_P(gpio_buffer, &mPins::module_template__gpio_map[module_i], sizeof(gpio_buffer));
  
      ALOG_INF(PSTR("Module %d:"), module_i);
  
      for (int gpio_i = 0; gpio_i < MAX_USER_PINS; gpio_i++) {
        uint16_t gpio_fn = gpio_buffer[gpio_i];
  
        if (gpio_fn != 0) {
          const char* fn_name = GetGPIOFunctionNamebyID(gpio_fn, buffer, sizeof(buffer));
          ALOG_INF(PSTR("  GPIO%02d = %d (%s)"), gpio_i, gpio_fn, buffer);
        } else {
          ALOG_INF(PSTR("  GPIO%02d = None"), gpio_i);
        }
      }
    }
    #endif

  }

  /*
  {"Debug":{"GpioSet":{"pin":3,"digital":1}}}
  {"Debug":{"GpioSet":{"pin":3,"digital":0}}}
  {"Debug":{"GpioSet":{"pin":3,"analog":123}}}

  {"Debug":{"GpioRead":{"pin":3,"digital":1}}}
  {"Debug":{"GpioRead":{"pin":3,"analog":1}}}

  Notes:

  GpioSet makes pin OUTPUT.
  GpioRead makes pin INPUT.
  For read, digital and analog are just flags to choose which read to perform.
  On ESP32, analogWrite() only works if your build already provides that wrapper. If not, this needs to be swapped for your existing LEDC helper.
  */
  if(tok = obj["Debug"].getObject()["GpioSet"])
  {
    ALOG_WRN(PSTR("GpioSet"));

    JsonParserObject gpio_obj = tok.getObject();

    int pin = -1;
    if(auto tpin = gpio_obj["pin"]) {
      pin = tpin.getInt();
    }

    if(pin >= 0)
    {
      pinMode(pin, OUTPUT);

      if(auto tdigital = gpio_obj["digital"])
      {
        int value = tdigital.getInt() ? HIGH : LOW;
        digitalWrite(pin, value);
        ALOG_INF(PSTR("GPIO Set, pin=%d, digital=%d"), pin, value ? 1 : 0);
      }

      if(auto tanalog = gpio_obj["analog"])
      {
        int value = tanalog.getInt();

        #ifdef ESP32
        value = constrain(value, 0, 255);  // LEDC/PWM style range expected here
        analogWrite(pin, value);
        #else
        value = constrain(value, 0, 255);
        analogWrite(pin, value);
        #endif

        ALOG_INF(PSTR("GPIO Set, pin=%d, analog=%d"), pin, value);
      }
    }
    else
    {
      ALOG_INF(PSTR("GPIO Set failed, invalid pin"));
    }
  }

  if(tok = obj["Debug"].getObject()["GpioRead"])
  {
    ALOG_WRN(PSTR("GpioRead"));

    JsonParserObject gpio_obj = tok.getObject();

    int pin = -1;
    if(auto tpin = gpio_obj["pin"]) {
      pin = tpin.getInt();
    }

    if(pin >= 0)
    {
      pinMode(pin, INPUT);

      if(gpio_obj["digital"])
      {
        int value = digitalRead(pin);
        ALOG_INF(PSTR("GPIO Read, pin=%d, digital=%d"), pin, value);
      }

      if(gpio_obj["analog"])
      {
        int value = analogRead(pin);
        ALOG_INF(PSTR("GPIO Read, pin=%d, analog=%d"), pin, value);
      }
    }
    else
    {
      ALOG_INF(PSTR("GPIO Read failed, invalid pin"));
    }
  }
    if(tok = obj["Debug"].getObject()["GpioSeqTest"])
  {
    ALOG_WRN(PSTR("GpioSeqTest"));

    int pins[16];
    uint8_t pin_count = 0;
    uint32_t cycles = 1; // effectively long soak test

    // Support:
    // {"Debug":{"GpioSeqTest":[32,33,...]}}
    // {"Debug":{"GpioSeqTest":{"pins":[32,33,...],"cycles":100}}}

    JsonParserArray pin_array;

    if(tok.isArray())
    {
      pin_array = tok.getArray();
    }
    else if(tok.isObject())
    {
      JsonParserObject seq_obj = tok.getObject();
      if(auto tcycles = seq_obj["cycles"]) {
        cycles = tcycles.getInt();
      }
      pin_array = seq_obj["pins"].getArray();
    }

    for(auto item : pin_array)
    {
      if(pin_count < 16)
      {
        pins[pin_count++] = item.getInt();
      }
    }

    if(pin_count == 0)
    {
      ALOG_INF(PSTR("GpioSeqTest failed, no pins"));
      return;
    }

    // Configure all as outputs and force off first
    for(uint8_t i = 0; i < pin_count; i++)
    {
      pinMode(pins[i], OUTPUT);
      digitalWrite(pins[i], LOW);
    }

    auto all_off = [&]() {
      for(uint8_t i = 0; i < pin_count; i++)
      {
        digitalWrite(pins[i], LOW);
      }
    };

    auto delay_with_wdt = [&](uint32_t ms) {
      uint32_t t_start = millis();
      while((millis() - t_start) < ms)
      {
        // Replace with your actual watchdog feed if different
        #ifdef ESP32
        WDT_Reset();
        #endif
        delay(10);
      }
    };

    ALOG_DBG(PSTR("GpioSeqTest starting, pins=%d, cycles=%lu"), pin_count, cycles);

    for(uint32_t cycle_i = 0; cycle_i < cycles; cycle_i++)
    {
      /**********************************************************************
       * 1) ONE IN SEQUENCE ON
       * One relay on at a time, 1 second each
       *********************************************************************/
      for(uint8_t i = 0; i < pin_count; i++)
      {
        all_off();
        digitalWrite(pins[i], HIGH);
        ALOG_DBG(PSTR("GpioSeqTest seq1, step=%d, pin=%d ON"), i, pins[i]);
        delay_with_wdt(1000);
      }

      /**********************************************************************
       * 2) 4 ON, 4 OFF
       * First half ON, second half OFF, then invert
       *********************************************************************/
      all_off();
      for(uint8_t i = 0; i < pin_count; i++)
      {
        digitalWrite(pins[i], (i < 4) ? HIGH : LOW);
      }
      ALOG_INF(PSTR("GpioSeqTest seq2, first4 ON"));
      delay_with_wdt(1000);

      all_off();
      for(uint8_t i = 0; i < pin_count; i++)
      {
        digitalWrite(pins[i], (i < 4) ? LOW : HIGH);
      }
      ALOG_INF(PSTR("GpioSeqTest seq2, last4 ON"));
      delay_with_wdt(1000);

      /**********************************************************************
       * 3) 4 EVENS, 4 ODDS
       * Based on array index: 0,2,4,6 then 1,3,5,7
       *********************************************************************/
      all_off();
      for(uint8_t i = 0; i < pin_count; i++)
      {
        digitalWrite(pins[i], ((i % 2) == 0) ? HIGH : LOW);
      }
      ALOG_INF(PSTR("GpioSeqTest seq3, even index ON"));
      delay_with_wdt(1000);

      all_off();
      for(uint8_t i = 0; i < pin_count; i++)
      {
        digitalWrite(pins[i], ((i % 2) == 1) ? HIGH : LOW);
      }
      ALOG_INF(PSTR("GpioSeqTest seq3, odd index ON"));
      delay_with_wdt(1000);

      all_off();
      ALOG_INF(PSTR("GpioSeqTest cycle=%lu complete"), cycle_i + 1);
      delay_with_wdt(1000);
    }

    all_off();
    ALOG_INF(PSTR("GpioSeqTest finished"));
  }
  

  #endif
  

}


void mPins::Pre_Init()
{

}


int8_t mPins::GetRealPinNumberFromName(const char* c){

  int8_t pin = -1;
  #ifdef ESP8266
  //nodemcu/wemos named
  if     (strcmp(c,"D3")==0){ pin = 0; }
  else if(strcmp(c,"TX")==0){ pin = 1; }
  else if(strcmp(c,"D4")==0){ pin = 2; }
  else if(strcmp(c,"RX")==0){ pin = 3; }
  else if(strcmp(c,"D2")==0){ pin = 4; }
  else if(strcmp(c,"D1")==0){ pin = 5; }
  else if(strcmp(c,"SD2")==0){ pin = 9; }
  else if(strcmp(c,"SD3")==0){ pin = 10; }
  else if(strcmp(c,"D6")==0){ pin = 12; }
  else if(strcmp(c,"D7")==0){ pin = 13; }
  else if(strcmp(c,"D5")==0){ pin = 14; }
  else if(strcmp(c,"D8")==0){ pin = 15; }
  else if(strcmp(c,"D0")==0){ pin = 16; }
  // else if(strcmp(c,"A0")==0){ pin = 17; }
  else if(strcmp(c,"LBI")==0){ 
    #ifdef LED_BUILTIN
    pin = LED_BUILTIN; 
    #else
    pin = -1;
    #endif   
  }
  // numbered
  else if(strcmp(c,"0")==0){ pin = 0; }
  else if(strcmp(c,"1")==0){ pin = 1; }
  else if(strcmp(c,"2")==0){ pin = 2; }
  else if(strcmp(c,"3")==0){ pin = 3; }
  else if(strcmp(c,"4")==0){ pin = 4; }
  else if(strcmp(c,"5")==0){ pin = 5; }
  else if(strcmp(c,"6")==0){ pin = 6; }
  else if(strcmp(c,"7")==0){ pin = 7; }
  else if(strcmp(c,"8")==0){ pin = 8; }
  else if(strcmp(c,"9")==0){ pin = 9; }
  else if(strcmp(c,"10")==0){ pin = 10; }
  else if(strcmp(c,"12")==0){ pin = 12; }
  else if(strcmp(c,"13")==0){ pin = 13; }
  else if(strcmp(c,"14")==0){ pin = 14; }
  else if(strcmp(c,"15")==0){ pin = 15; }
  else if(strcmp(c,"16")==0){ pin = 16; }
  else{
    pin = -1;
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_ERROR, PSTR("\t\tGetRealPinNumberFromName = %d PIN UNKNOWN for \"%s\""), pin, c);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
  }

    #ifdef ENABLE_LOG_LEVEL_INFO
    ALOG_DBM( PSTR("GetRealPinNumberFromName = %d"), pin);
    #endif // ENABLE_LOG_LEVEL_INFO
  #endif // ESP8266

  // #ifdef ESP32
  
  // Check for pin_array matching
  char buffer[10];
  for(uint8_t i=0; i<ARRAY_SIZE(gpio_pin_by_index); i++)
  // for(uint8_t i=0; i<35; i++)
  {
    sprintf(buffer,"%d\0",gpio_pin_by_index[i]);
    // sprintf(buffer,"%d\0",i);
    
      // ALOG_INF(PSTR("NOOOOOOOOOmatched pin %d %d %s %s"),pin,i,c,buffer);

    if(strcmp(c,buffer)==0)
    {
      pin = gpio_pin_by_index[i];
      ALOG_DBM( PSTR("matched pin %d %d"),pin,i);
      break;
    }else{

    }
    
  }

  // Names for pins
  if(strcmp(c,"LBI")==0){ 
    pin = 2;
  }

  // if(pin<0)
  // {
  //   pin = -1;
  //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   AddLog(LOG_LEVEL_ERROR, PSTR("\t\tGetRealPinNumberFromName = %d PIN UNKNOWN for \"%s\""), pin, c);
  //   #endif // ENABLE_LOG_LEVEL_COMMANDS
  // }

    #ifdef ENABLE_LOG_LEVEL_INFO
    ALOG_INF( PSTR("GetRealPinNumberFromName = \t\t%d"), pin);
    #endif // ENABLE_LOG_LEVEL_INFO
  // #endif // ESP32

  return pin;

}

/**
 * @brief Convert the real_pin number to its indexed position within pin_array
 * @note 
 * @param real_pin physical external pin number
 * @return index position of pin in array
 */
int8_t mPins::ConvertRealPinToIndexPin(uint8_t real_pin){
  for(int index_pin=0;index_pin<MAX_GPIO_PIN;index_pin++)
  {
    if(real_pin == gpio_pin_by_index[index_pin])
    {
      return index_pin;
    }
  }
  return -1;
}


/*********************************************************************************************\
 * Pin lookup / assignment, packed GPIO function model
 *
 * Previous reference implementation kept here while the packed-ID rewrite settles:
 *
 *   #ifdef ENABLE_FEATURE_HARDWAREPINS__FUNCTION_AND_INDEX_PACKED_GPIO
 *     uint16_t real_gpio = gpio << 5;
 *     uint16_t mask = 0xFFE0;
 *     if (index < GPIO_ANY) {
 *       real_gpio += index;
 *       mask = 0xFFFF;
 *     }
 *     for (...) {
 *       if ((pin_attached_gpio_functions[i] & mask) == real_gpio) { ... }
 *     }
 *   #else
 *     uint16_t real_gpio = gpio + index;
 *     for (...) {
 *       if (pin_attached_gpio_functions[i] == real_gpio) { ... }
 *     }
 *   #endif
 *
 * New model:
 *   - pin_attached_gpio_functions[] always stores packed selected IDs.
 *   - stored_id = PGPIO(base_id) + instance_index.
 *   - public callers pass base + index: Pin(GPIO_MODEM_RX, 1).
 *   - no GPIO__X + index arithmetic is used outside this layer.
\*********************************************************************************************/

int16_t IRAM_ATTR mPins::Pin(uint32_t gpio, uint32_t index)
{
  uint16_t packed_gpio = PGPIO(gpio);
  uint16_t mask        = GPIO_BASE_MASK;

  if (index < GPIO_ANY)
  {
    packed_gpio += (index & GPIO_INDEX_MASK);
    mask = 0xFFFF;
  }

  for (uint32_t index_pin = 0; index_pin < nitems(pin_attached_gpio_functions); index_pin++)
  {
    if ((pin_attached_gpio_functions[index_pin] & mask) == packed_gpio)
    {
      return gpio_pin_by_index[index_pin];   // physical GPIO number configured for this function
    }
  }

  return -1;
}


boolean mPins::PinUsed(uint32_t gpio, uint32_t index)
{
  return (Pin(gpio, index) >= 0);
}


/**
 * @brief Assign a packed selected GPIO function to a physical GPIO pin.
 *
 * @param real_pin Physical GPIO number.
 * @param packed_gpio Packed selected GPIO ID, i.e. PGPIO(base_id) + index.
 */
void mPins::SetPin_GPIOFunction(uint32_t real_pin, uint16_t gpio_function)
{
  const int8_t internal_pin_index = ConvertRealPinToIndexPin(real_pin);

  if (internal_pin_index < 0)
  {
    ALOG_ERR(
      PSTR("SetPin_GPIOFunction failed: real_pin=%u gpio_function=0x%04X internal_index=%d"),
      real_pin,
      gpio_function,
      internal_pin_index
    );
    return;
  }

  pin_attached_gpio_functions[internal_pin_index] = gpio_function;

  if ((gpio_function == GPIO_NONE) || (gpio_function == GPIO_USER))
  {
    ALOG_DBG(
      PSTR("SetPin_GPIOFunction real_pin=%u internal_index=%d special_gpio=%u"),
      real_pin,
      internal_pin_index,
      gpio_function
    );
  }
  else
  {
    ALOG_DBG(
      PSTR("SetPin_GPIOFunction real_pin=%u internal_index=%d gpio_function=0x%04X base=%u index=%u"),
      real_pin,
      internal_pin_index,
      gpio_function,
      UGPIO(gpio_function),
      gpio_function & GPIO_INDEX_MASK
    );
  }
}


int8_t mPins::ConvertIndexPinToRealPin(uint8_t index_pin)
{
  
  if (index_pin >= MAX_USER_PINS)
  {
    ALOG_INF(PSTR("PIN: ConvertIndexPinToRealPin OOR, index_pin=%u >= MAX_USER_PINS=%u"),index_pin,MAX_USER_PINS);
    return -1;
  }

  return gpio_pin_by_index[index_pin];
}


void mPins::DigitalWrite(uint32_t gpio_pin, uint32_t state)
{

  ALOG_ERR(PSTR("May be invalid if gpio is not unpacked. Use 3 arg version."));

  ALOG_DBG(PSTR("DigitalWrite(uint32_t %d, uint32_t %d"), gpio_pin, state);

  DigitalWrite(gpio_pin, 0, state);
  // if (tkr_set->pin[gpio_pin] < 99) {
        // AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_RELAYS "DigitalWrite(%d[%d],%d)"),tkr_set->pin[gpio_pin],gpio_pin,state);
    // digitalWrite(tkr_set->pin[gpio_pin], state &1);
  // }
}


void mPins::DigitalWrite(uint32_t gpio_pin, uint32_t index, uint32_t state)
{
  uint8_t real_pin = Pin(gpio_pin, index);
  ALOG_DBG(PSTR("DigitalWrite gpio_pin=%d,real_pin=%d,index=%d,state=%d"),gpio_pin,real_pin, index, state &1);
  digitalWrite(real_pin, state &1);
}


bool mPins::DigitalRead(uint32_t gpio_pin, uint32_t index)
{
  uint8_t real_pin = Pin(gpio_pin, index);
  return digitalRead(real_pin);
}


uint8_t mPins::ModuleNr()
{
  // 0    = User module (255)
  // 1 up = Template module 0 up
  return (USER_MODULE == tkr_set->Settings.module) ? 0 : tkr_set->Settings.module + 1;
}


uint32_t mPins::ModuleTemplate(uint32_t module) {
  uint32_t i = 0;
  for (i = 0; i < sizeof(module_template__ids); i++) {
    if (module == pgm_read_byte(module_template__ids + i)) {
      break;
    }
  }
  if (i == sizeof(module_template__ids)) { i = 0; }
  return i;
}


bool mPins::ValidTemplateModule(uint8_t index)
{
  for (uint8_t i = 0; i < sizeof(module_template__ids); i++) {
    if (index == pgm_read_byte(module_template__ids + i)) {
      return true;
    }
  }
  return false;
}

bool mPins::ValidModule(uint8_t index)
{
  if (index == USER_MODULE) { return true; }
  return ValidTemplateModule(index);
}

/*********************************************************************************************\
 * GPIO function validation, packed GPIO function model
 *
 * Previous behaviour only range-checked legacy raw enum IDs. The packed model validates by
 * checking that the packed base ID exists in SelectablePins_BitPacked[] and that the stored
 * index is within the group's MGPIO(count) limit.
\*********************************************************************************************/


// bool mPins::ValidUserGPIOFunction(uint16_t* pin_array, uint8_t index)
// {
//   if (!pin_array) { return false; }

//   const uint16_t packed_gpio = pin_array[index];

//   for (uint32_t list_i = 0; list_i < SelectablePins_BitPacked_Count; list_i++)
//   {
//     const uint16_t entry = pgm_read_word(&SelectablePins_BitPacked[list_i]);
//     const uint16_t base  = UGPIO(entry);
//     const uint8_t count  = (entry & GPIO_INDEX_MASK) + 1;

//     if ((UGPIO(packed_gpio) == base) && ((packed_gpio & GPIO_INDEX_MASK) < count))
//     {
//       return true;
//     }
//   }

//   return false;
// }
bool mPins::ValidUserGPIOFunction(uint16_t* pin_array, uint8_t index)
{
  if (!pin_array)
  {
    ALOG_ERR(PSTR("ValidUserGPIOFunction: pin_array=null index=%u"), index);
    return false;
  }

  const uint16_t packed_gpio = pin_array[index];

  if (packed_gpio == GPIO_NONE)
  {
    ALOG_INF(PSTR("ValidUserGPIOFunction: VALID_NONE index=%u packed=%u"), index, packed_gpio);
    return true;
  }

  if (packed_gpio == GPIO_USER)
  {
    ALOG_INF(PSTR("ValidUserGPIOFunction: VALID_USER index=%u packed=%u"), index, packed_gpio);
    return true;
  }

  const uint16_t packed_base = UGPIO(packed_gpio);
  const uint8_t  packed_idx  = packed_gpio & GPIO_INDEX_MASK;

  char gpio_name[64];
  GetGPIOFunctionNamebyID(packed_gpio, gpio_name, sizeof(gpio_name));

  ALOG_INF(
    PSTR("ValidUserGPIOFunction: index=%u packed=0x%04X dec=%u base=%u idx=%u name=\"%s\""),
    index,
    packed_gpio,
    packed_gpio,
    packed_base,
    packed_idx,
    gpio_name
  );

  for (uint32_t list_i = 0; list_i < SelectablePins_BitPacked_Count; list_i++)
  {
    const uint16_t entry      = pgm_read_word(&SelectablePins_BitPacked[list_i]);
    const uint16_t entry_base = UGPIO(entry);
    const uint8_t  entry_max  = (entry & GPIO_INDEX_MASK) + 1;

    if (packed_base == entry_base)
    {
      if (packed_idx < entry_max)
      {
        ALOG_INF(
          PSTR("ValidUserGPIOFunction: VALID index=%u packed=0x%04X base=%u idx=%u max=%u"),
          index,
          packed_gpio,
          packed_base,
          packed_idx,
          entry_max
        );
        return true;
      }

      ALOG_ERR(
        PSTR("ValidUserGPIOFunction: INVALID_INDEX index=%u packed=0x%04X base=%u idx=%u max=%u"),
        index,
        packed_gpio,
        packed_base,
        packed_idx,
        entry_max
      );
      return false;
    }
  }

  ALOG_ERR(
    PSTR("ValidUserGPIOFunction: INVALID_BASE index=%u packed=0x%04X base=%u idx=%u"),
    index,
    packed_gpio,
    packed_base,
    packed_idx
  );

  return false;
}


const char* mPins::ModuleName()
{
  return AnyModuleName(tkr_set->Settings.module);
}
const char* mPins::AnyModuleName(uint8_t index)
{

// ERROR // if(buffer == nullptr){ buffer = tkr_sup->global_buffer; }
// dont save to nullptr, either take nullptr or gloabl
 
 // char* pointer;
 // if(buffer == nullptr){ pointer = tkr_sup->global_buffer; }
 // if(buffer != nullptr){ pointer = buffer;// = tkr_sup->global_buffer; }

 
  // if (USER_MODULE == index) {
  // // ALOG_TST(PSTR(D_LOG_HTTP "USER_MODULE == index"));
  //   return PM_SEARCH_NOMATCH;//tkr_set->Settings.user_template.hardware.name; //returns pointer
  // } else {
    // ALOG_TST(PSTR(D_LOG_HTTP "USER_MODULE != %d index %s"),index,module_template__gpio_map[index].name);
    // return module_template__gpio_map[index].name;

    //Change to use function

    return GetModuleNameByID(index);
    // return tkr_sup->GetTextIndexed_P(buffer, buflen, index, module_template__names);

    


  // }
}


gpio_flag mPins::ModuleFlag()
{
  gpio_flag flag;

  if (USER_MODULE == tkr_set->Settings.module) {
    flag = tkr_set->Settings.user_template.hardware.flag;
  }  else {
    #ifdef ESP8266
      GetInternalTemplate(&flag, tkr_set->Settings.module, 2);
    #endif  // ESP8266
    #ifdef ESP32
      memcpy_P(&flag, &module_template__gpio_map[tkr_set->Settings.module].flag, sizeof(gpio_flag));
    #endif  // ESP32
  }

  return flag;
}


void mPins::ModuleDefault(uint8_t module)
{
  if (USER_MODULE == module) { module = MODULE_DEFAULT; }  // Generic
  tkr_set->Settings.user_template.base = module;

  ALOG_ERR(PSTR("ModuleDefault REMOVED CODE, NEEDS FIXING module=%d"),module);

  DEBUG_LINE_HERE3
  DEBUG_LINE_HERE3
  DEBUG_LINE_HERE3

  #ifdef ESP32
    module = ModuleTemplate(module);
  #endif

    char name[TOPSZ];
    tkr_set->SettingsUpdateText(SET_TEMPLATE_NAME, tkr_sup->GetTextIndexed_P(name, sizeof(name), module, module_template__names));
  #ifdef ESP8266
    // Read the entire template with option 3 (GPIO + flags)
    GetInternalTemplate(&tkr_set->Settings.user_template, module, 3);
  #endif  // ESP8266
  #ifdef ESP32
  
    ALOG_INF( PSTR("ModuleDefault =================================================================================================================================================================================================module=%d %d %d"), module, sizeof(tkr_set->Settings.user_template), sizeof(mytmplt));

    memcpy_P(&tkr_set->Settings.user_template, &module_template__gpio_map[module], sizeof(mytmplt));
  #endif  // ESP32
}

void mPins::SetModuleType()
{
  tkr_set->runtime.my_module_type = (USER_MODULE == tkr_set->Settings.module) ? tkr_set->Settings.user_template.base : tkr_set->Settings.module;
}



bool mPins::CheckPhysicalPinIsFlashPin(uint32_t pin) 
{
  #ifdef ESP8266
    return (((pin > 5) && (pin < 9)) || (11 == pin));
  #endif  // ESP8266
  #ifdef ESP32
    #if CONFIG_IDF_TARGET_ESP32C2
      return (((pin > 10) && (pin < 12)) || ((pin > 13) && (pin < 18)));  // ESP32C3 has GPIOs 11-17 reserved for Flash, with some boards GPIOs 12 13 are useable
    #elif CONFIG_IDF_TARGET_ESP32C3
      return ((pin > 13) && (pin < 18));   // ESP32C3 has GPIOs 11-17 reserved for Flash, with some boards GPIOs 11 12 13 are useable
    #elif CONFIG_IDF_TARGET_ESP32C6
      return ((pin == 24) || (pin == 25) || (pin == 27) || (pin == 29) || (pin == 30));  // ESP32C6 has GPIOs 24-30 reserved for Flash, with some boards GPIOs 26 28 are useable
    #elif CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
      return (pin > 21) && (pin < 33);     // ESP32S2 skip 22-32
    #else
      return (pin >= 28) && (pin <= 31);   // ESP32 skip 28-31
    #endif  // ESP32C2/C3/C6 and S2/S3
  #endif  // ESP32
}


/**
 * @brief Fixed GPIO to U16 range
 * 
 * @param pin 
 * @param gpio 
 * @return uint16_t 
 */
uint16_t mPins::ValidPin_AdjustGPIO(uint8_t physical_pin, uint16_t gpio)
{
  uint16_t result = gpio;

  if (CheckPhysicalPinIsFlashPin(physical_pin)) { 
    return GPIO_NONE;    // Disable flash pins GPIO6, GPIO7, GPIO8 and GPIO11
  }

  #ifdef ESP8266
  if ((MODULE_GENERIC == tkr_set->Settings.module) && (!tkr_set->Settings.sysopt_system.bit.user_esp8285_enable)) {
    if ((physical_pin == 9) || (physical_pin == 10)) { return GPIO_NONE; }  // Disable possible flash GPIO9 and GPIO10
  }
  #endif

  return result;

}


bool mPins::ValidGPIO(uint8_t pin, uint16_t gpio)
{
  return (GPIO_USER == ValidPin_AdjustGPIO(pin, gpio));  // Only allow GPIO_USER pins
}


int16_t mPins::GetModuleIDbyName(const char* c){
  if(*c=='\0'){ return -1; }
  #ifdef ESP8266
    if(strcmp_P(c,PM_MODULE_NAME_SONOFF_BASIC_CTR)==0){    return MODULE_SONOFF_BASIC; }
    if(strcmp_P(c,PM_MODULE_NAME_SONOFF_BASIC_EXTERNAL_CTR)==0){    return MODULE_SONOFF_BASIC_EXTERNAL; }
    else if(strcmp_P(c,PM_MODULE_NAME_H801_CTR)==0){            return MODULE_H801; }
    else if(strcmp_P(c,PM_MODULE_NAME_MAGICHOME_CTR)==0){       return MODULE_MAGICHOME; }
    else if(strcmp_P(c,PM_MODULE_NAME_SHELLY1_CTR)==0){         return MODULE_SHELLY1; }
    else if(strcmp_P(c,PM_MODULE_NAME_SHELLY2P5_CTR)==0){         return MODULE_SHELLY2P5; }
    else if(strcmp_P(c,PM_MODULE_NAME_SHELLY_DIMMER2_CTR)==0){         return MODULE_SHELLY_DIMMER2; }
    #ifdef USE_MODULE_TEMPLATE_SONOFF_IFAN03
    else if(strcmp_P(c,PM_MODULE_NAME_SONOFF_IFAN03_CTR)==0){         return MODULE_SONOFF_IFAN03; }
    #endif
    else if(strcmp_P(c,PM_MODULE_NAME_SONOFF_4CHPRO_CTR)==0){         return MODULE_SONOFF_4CHPRO; }
  #endif
  #ifdef ESP32
    #if CONFIG_IDF_TARGET_ESP32C2

    #elif CONFIG_IDF_TARGET_ESP32C3

    #elif CONFIG_IDF_TARGET_ESP32C6

    #elif CONFIG_IDF_TARGET_ESP32S2

    #elif CONFIG_IDF_TARGET_ESP32S3
      #ifdef USE_MODULE_TEMPLATE__CAMERA_XIAO_ESP32S3_SENSE
      else if(strcmp_P(c,PM__MODULE_TEMPLATE__CAMERA_XIAO_ESP32S3_SENSE__CTR)==0){         return MODULE__CAMERA_XIAO_ESP32S3_SENSE__ID; }
      #endif    
    #else  // ESP32 STANDARD not CONFIG_IDF_TARGET_ESP32C2/C3/C6 nor CONFIG_IDF_TARGET_ESP32S2 - ESP32
      else if(strcmp_P(c,PM_MODULE_NAME__ESP32_DEVKIT__CTR)==0){         return MODULE_DOIT_DEVKIT; }
      #ifdef USE_MODULE_TEMPLATE_CAM_AITHINKER
      else if(strcmp_P(c,PM_MODULE_NAME_CAM_AITHINKER_CTR)==0){         return MODULE_CAM_AITHINKER; }
      #endif    
      #ifdef USE_MODULE_TEMPLATE__CAMERA_FREENOVE_WROOVER
      else if(strcmp_P(c,PM__MODULE_TEMPLATE__CAMERA_FREENOVE_WROOVER__CTR)==0){         return MODULE__CAMERA_FREENOVE_WROOVER__ID; }
      #endif    
    #endif
  #endif

  else if(strcmp_P(c,PM_MODULE_NAME_USERMODULE_CTR)==0){      return USER_MODULE; }

  return -1;

}

/*
Change to return pointer, not a buffer 
*/
const char* mPins::GetModuleNameByID(uint8_t id){
  switch(id){
    default:
    #ifdef ESP8266
      case MODULE_SONOFF_BASIC:               return PM_MODULE_NAME_SONOFF_BASIC_CTR;
      case MODULE_SONOFF_BASIC_EXTERNAL:      return PM_MODULE_NAME_SONOFF_BASIC_EXTERNAL_CTR;
      #ifdef USE_MODULE_TEMPLATE_SONOFF_IFAN03
      case MODULE_SONOFF_IFAN03:              return PM_MODULE_NAME_SONOFF_IFAN03_CTR;
      #endif
      case MODULE_SONOFF_4CHPRO:              return PM_MODULE_NAME_SONOFF_4CHPRO_CTR;
      case MODULE_H801:                       return PM_MODULE_NAME_H801_CTR;
      case MODULE_MAGICHOME:                  return PM_MODULE_NAME_MAGICHOME_CTR;
      case MODULE_SHELLY1:                    return PM_MODULE_NAME_SHELLY1_CTR;
      case MODULE_SHELLY2P5:                  return PM_MODULE_NAME_SHELLY2P5_CTR;
      case MODULE_SHELLY_DIMMER2:             return PM_MODULE_NAME_SHELLY_DIMMER2_CTR;
    #endif  // ESP8266
    #ifdef ESP32

      #if CONFIG_IDF_TARGET_ESP32C2
      #elif CONFIG_IDF_TARGET_ESP32C3
      #elif CONFIG_IDF_TARGET_ESP32C6
      #elif CONFIG_IDF_TARGET_ESP32S2
      #elif CONFIG_IDF_TARGET_ESP32S3
        #ifdef USE_MODULE_TEMPLATE__CAMERA_XIAO_ESP32S3_SENSE
        case MODULE__CAMERA_XIAO_ESP32S3_SENSE__ID:              return PM__MODULE_TEMPLATE__CAMERA_XIAO_ESP32S3_SENSE__CTR;
        #endif    
      #else  // ESP32 STANDARD not CONFIG_IDF_TARGET_ESP32C2/C3/C6 nor CONFIG_IDF_TARGET_ESP32S2 - ESP32
      
        case MODULE_DOIT_DEVKIT:                   return PM_MODULE_NAME__ESP32_DEVKIT__CTR;
        #ifdef USE_MODULE_TEMPLATE_CAM_AITHINKER
        case MODULE_CAM_AITHINKER:              return PM_MODULE_NAME_CAM_AITHINKER_CTR;
        #endif    
        #ifdef USE_MODULE_TEMPLATE__CAMERA_FREENOVE_WROOVER
        case MODULE__CAMERA_FREENOVE_WROOVER__ID:              return PM__MODULE_TEMPLATE__CAMERA_FREENOVE_WROOVER__CTR;
        #endif    
      #endif
    #endif
    case USER_MODULE:                            return PM_MODULE_NAME_USERMODULE_CTR;

  }
}















































/******************************************************************************************************************
 * PinViewer Compatibility / Diagnostic Wrapper API
*******************************************************************************************************************/

bool mPins::PinViewer__PhysicalPin__IsValid(uint8_t physical_pin)
{
  #ifdef ESP8266
    if(physical_pin > 16) { return false; }
    if((physical_pin >= 6) && (physical_pin <= 11)) { return false; } // flash pins
    return true;
  #else
    if(physical_pin >= 48) { return false; }
    return true;
  #endif
}


bool mPins::PinViewer__PhysicalPin__IsUsable(uint8_t physical_pin)
{
  if(!PinViewer__PhysicalPin__IsValid(physical_pin)) { return false; }
  if(PinViewer__PhysicalPin__IsReserved(physical_pin)) { return false; }

  return true;
}


bool mPins::PinViewer__PhysicalPin__IsReserved(uint8_t physical_pin)
{
  #ifdef ESP8266
    if((physical_pin >= 6) && (physical_pin <= 11)) { return true; }
  #endif

  #ifdef ESP32
    // Demo reservation examples, replace with real board/flash/PSRAM/strapping rules later.
    if(physical_pin == 6)  { return true; }
    if(physical_pin == 7)  { return true; }
    if(physical_pin == 8)  { return true; }
    if(physical_pin == 9)  { return true; }
    if(physical_pin == 10) { return true; }
    if(physical_pin == 11) { return true; }
  #endif

  return false;
}


bool mPins::PinViewer__PhysicalPin__IsUsed(uint8_t physical_pin)
{
  if(!PinViewer__PhysicalPin__IsUsable(physical_pin)) { return false; }

  const uint8_t group_id = PinViewer__PhysicalPin__GetGroupID(physical_pin);
  if(group_id != 0) { return true; }

  // Existing current map, where possible.
  const int8_t index = ConvertRealPinToIndexPin(physical_pin);
  if(index >= 0 && index < MAX_USER_PINS)
  {
    if(pin_attached_gpio_functions[index] != 0) { return true; }
  }

  return false;
}


bool mPins::PinViewer__PhysicalPin__IsLocked(uint8_t physical_pin)
{
  if(!PinViewer__PhysicalPin__IsUsed(physical_pin)) { return false; }

  // Temporary animated demo.
  // Gives visible changing lock states on the page.
  return (((millis() / 1000) + physical_pin) % 13) == 0;
}


bool mPins::PinViewer__PhysicalPin__HasConflict(uint8_t physical_pin)
{
  if(!PinViewer__PhysicalPin__IsUsable(physical_pin)) { return false; }

  // Temporary animated demo.
  // Gives visible changing conflict states on the page.
  return (((millis() / 1500) + (physical_pin * 3)) % 19) == 0;
}


uint16_t mPins::PinViewer__PhysicalPin__GetFunctionID(uint8_t physical_pin)
{
  if(!PinViewer__PhysicalPin__IsUsable(physical_pin)) { return 0; }

  switch(physical_pin)
  {
    case 21: return 3001; // I2C SDA demo
    case 22: return 3002; // I2C SCL demo

    case 18: return 3101; // SPI SCK demo
    case 19: return 3102; // SPI MISO demo
    case 23: return 3103; // SPI MOSI demo

    case 4:  return 4001; // Relay demo
    case 5:  return 4002; // Button demo
    case 2:  return 4003; // Status LED demo
  }

  const int8_t index = ConvertRealPinToIndexPin(physical_pin);
  if(index >= 0 && index < MAX_USER_PINS)
  {
    return pin_attached_gpio_functions[index];
  }

  return 0;
}


uint16_t mPins::PinViewer__PhysicalPin__GetOwnerID(uint8_t physical_pin)
{
  switch(PinViewer__PhysicalPin__GetGroupID(physical_pin))
  {
    case 1: return 3001000; // I2C bus owner demo
    case 2: return 3101000; // SPI bus owner demo
    case 3: return 4001000; // relay module demo
    case 4: return 4002000; // button module demo
    case 5: return 4003000; // led module demo
  }

  return 0;
}


uint8_t mPins::PinViewer__PhysicalPin__GetDirectionID(uint8_t physical_pin)
{
  switch(physical_pin)
  {
    case 5:
    case 19:
      return 1; // input

    case 2:
    case 4:
    case 18:
    case 21:
    case 22:
    case 23:
      return 2; // output / peripheral output-ish demo

    default:
      return 0; // unknown
  }
}


uint8_t mPins::PinViewer__PhysicalPin__GetShareModeID(uint8_t physical_pin)
{
  switch(PinViewer__PhysicalPin__GetGroupID(physical_pin))
  {
    case 1: return 2; // shared bus
    case 2: return 2; // shared bus
    case 3: return 1; // exclusive
    case 4: return 1; // exclusive
    case 5: return 1; // exclusive
  }

  return 0;
}


uint8_t mPins::PinViewer__PhysicalPin__GetUserCount(uint8_t physical_pin)
{
  switch(PinViewer__PhysicalPin__GetGroupID(physical_pin))
  {
    case 1: return 2; // BME280 + OLED demo
    case 2: return 1; // SPI display demo
    case 3: return 1;
    case 4: return 1;
    case 5: return 1;
  }

  return 0;
}


uint8_t mPins::PinViewer__PhysicalPin__GetGroupID(uint8_t physical_pin)
{
  switch(physical_pin)
  {
    case 21:
    case 22:
      return 1; // I2C group

    case 18:
    case 19:
    case 23:
      return 2; // SPI group

    case 4:
      return 3; // Relay group

    case 5:
      return 4; // Button group

    case 2:
      return 5; // Status LED group
  }

  return 0;
}


const char* mPins::PinViewer__PhysicalPin__GetFunctionName(uint8_t physical_pin, char* buffer, uint8_t buflen)
{
  if(!buffer || buflen == 0) { return ""; }

  switch(physical_pin)
  {
    case 21: snprintf(buffer, buflen, "I2C_SDA"); return buffer;
    case 22: snprintf(buffer, buflen, "I2C_SCL"); return buffer;

    case 18: snprintf(buffer, buflen, "SPI_SCK");  return buffer;
    case 19: snprintf(buffer, buflen, "SPI_MISO"); return buffer;
    case 23: snprintf(buffer, buflen, "SPI_MOSI"); return buffer;

    case 4:  snprintf(buffer, buflen, "Relay_1");     return buffer;
    case 5:  snprintf(buffer, buflen, "Button_1");    return buffer;
    case 2:  snprintf(buffer, buflen, "Status_LED");  return buffer;
  }

  const uint16_t function_id = PinViewer__PhysicalPin__GetFunctionID(physical_pin);
  if(function_id == 0)
  {
    snprintf(buffer, buflen, "None");
    return buffer;
  }

  snprintf(buffer, buflen, "GPIO_%u", function_id);
  return buffer;
}


const char* mPins::PinViewer__PhysicalPin__GetOwnerName(uint8_t physical_pin, char* buffer, uint8_t buflen)
{
  if(!buffer || buflen == 0) { return ""; }

  switch(PinViewer__PhysicalPin__GetGroupID(physical_pin))
  {
    case 1: snprintf(buffer, buflen, "mI2C"); return buffer;
    case 2: snprintf(buffer, buflen, "mSPI"); return buffer;
    case 3: snprintf(buffer, buflen, "mRelays[0]"); return buffer;
    case 4: snprintf(buffer, buflen, "mButtons[0]"); return buffer;
    case 5: snprintf(buffer, buflen, "mLEDs[0]"); return buffer;
  }

  snprintf(buffer, buflen, "None");
  return buffer;
}


const char* mPins::PinViewer__PhysicalPin__GetGroupName(uint8_t physical_pin, char* buffer, uint8_t buflen)
{
  return PinViewer__Group__GetName_ByID(PinViewer__PhysicalPin__GetGroupID(physical_pin), buffer, buflen);
}


const char* mPins::PinViewer__PhysicalPin__GetGroupColour(uint8_t physical_pin, char* buffer, uint8_t buflen)
{
  return PinViewer__Group__GetColour_ByID(PinViewer__PhysicalPin__GetGroupID(physical_pin), buffer, buflen);
}


uint8_t mPins::PinViewer__Group__GetCount(void)
{
  return 5;
}


uint8_t mPins::PinViewer__Group__GetID_ByIndex(uint8_t group_index)
{
  switch(group_index)
  {
    case 0: return 1;
    case 1: return 2;
    case 2: return 3;
    case 3: return 4;
    case 4: return 5;
  }

  return 0;
}


const char* mPins::PinViewer__Group__GetName_ByID(uint8_t group_id, char* buffer, uint8_t buflen)
{
  if(!buffer || buflen == 0) { return ""; }

  switch(group_id)
  {
    case 1: snprintf(buffer, buflen, "I2C Bus 0"); return buffer;
    case 2: snprintf(buffer, buflen, "SPI Bus 0"); return buffer;
    case 3: snprintf(buffer, buflen, "Relay Module"); return buffer;
    case 4: snprintf(buffer, buflen, "Button Module"); return buffer;
    case 5: snprintf(buffer, buflen, "System LED"); return buffer;
  }

  snprintf(buffer, buflen, "Unassigned");
  return buffer;
}


const char* mPins::PinViewer__Group__GetColour_ByID(uint8_t group_id, char* buffer, uint8_t buflen)
{
  if(!buffer || buflen == 0) { return ""; }

  switch(group_id)
  {
    case 1: snprintf(buffer, buflen, "#ff9800"); return buffer; // orange
    case 2: snprintf(buffer, buflen, "#42a5f5"); return buffer; // blue
    case 3: snprintf(buffer, buflen, "#66bb6a"); return buffer; // green
    case 4: snprintf(buffer, buflen, "#ab47bc"); return buffer; // purple
    case 5: snprintf(buffer, buflen, "#ef5350"); return buffer; // red
  }

  snprintf(buffer, buflen, "#666666");
  return buffer;
}


uint8_t mPins::PinViewer__Group__GetAddressCount(uint8_t group_id)
{
  switch(group_id)
  {
    case 1: return 2; // I2C demo: BME280 + OLED
    case 2: return 1; // SPI demo: display pseudo CS address
  }

  return 0;
}


bool mPins::PinViewer__Group__GetAddress_ByIndex(
  uint8_t group_id,
  uint8_t address_index,
  uint8_t* address,
  char* module_name,
  uint8_t module_name_len
){
  if(!address || !module_name || module_name_len == 0) { return false; }

  switch(group_id)
  {
    case 1:
      switch(address_index)
      {
        case 0:
          *address = 0x76;
          snprintf(module_name, module_name_len, "BME280");
          return true;

        case 1:
          *address = 0x3C;
          snprintf(module_name, module_name_len, "OLED");
          return true;
      }
    break;

    case 2:
      switch(address_index)
      {
        case 0:
          *address = 0x00;
          snprintf(module_name, module_name_len, "SPI_Display_CS0");
          return true;
      }
    break;
  }

  return false;
}
































// /******************************************************************************************************************
//  * Pin Allocation / Physical Pin Model
//  * Date Modified: 04Jun26
// *******************************************************************************************************************/

// #ifndef PINALLOC_DUMMY_FUNCTION_RELAY_ID
// #define PINALLOC_DUMMY_FUNCTION_RELAY_ID        4001
// #define PINALLOC_DUMMY_FUNCTION_BUTTON_ID       4002
// #define PINALLOC_DUMMY_FUNCTION_SWITCH_ID       4003
// #define PINALLOC_DUMMY_FUNCTION_STATUS_LED_ID   4004
// #define PINALLOC_DUMMY_FUNCTION_I2C_SDA_ID      4101
// #define PINALLOC_DUMMY_FUNCTION_I2C_SCL_ID      4102
// #define PINALLOC_DUMMY_FUNCTION_SERIAL_TX_ID    4201
// #define PINALLOC_DUMMY_FUNCTION_SERIAL_RX_ID    4202
// #define PINALLOC_DUMMY_FUNCTION_SPI_SCK_ID      4301
// #define PINALLOC_DUMMY_FUNCTION_SPI_MISO_ID     4302
// #define PINALLOC_DUMMY_FUNCTION_SPI_MOSI_ID     4303
// #endif

// #ifndef PINALLOC_DUMMY_OWNER_RELAY_ID
// #define PINALLOC_DUMMY_OWNER_RELAY_ID           4001000
// #define PINALLOC_DUMMY_OWNER_BUTTON_ID          4002000
// #define PINALLOC_DUMMY_OWNER_SWITCH_ID          4003000
// #define PINALLOC_DUMMY_OWNER_LED_ID             4004000
// #define PINALLOC_DUMMY_OWNER_I2C_ID             4100000
// #define PINALLOC_DUMMY_OWNER_SERIAL_ID          4200000
// #define PINALLOC_DUMMY_OWNER_SPI_ID             4300000
// #endif


// void mPins::PinAlloc_Init(void)
// {
//   PinAlloc_InitPhysicalInfo();
//   PinAlloc_InitAllocationTable();

//   /*
//    * Temporary layer while the real RequestPin()/ReleasePin() flow is developed.
//    * This makes the PinViewer useful immediately.
//    */
//   Init_InsertDummyPinAllocations();

//   #ifdef ENABLE_DEBUGFEATURE_SPLASH__PIN_ALLOCATIONS
//   Splash__PinAllocations();
//   #endif
// }


// void mPins::PinAlloc_InitPhysicalInfo(void)
// {
//   for(uint8_t physical_pin = 0; physical_pin < MAX_GPIO_PIN; physical_pin++)
//   {
//     pininfo[physical_pin].physical_pin = physical_pin;
//     pininfo[physical_pin].flags.data = 0;

//     /*
//      * First-pass assumption:
//      *   GPIO numbers inside MAX_GPIO_PIN exist unless marked otherwise.
//      *   Target-specific refinements are added below.
//      */
//     pininfo[physical_pin].flags.valid = 1;
//     pininfo[physical_pin].flags.input_capable = 1;
//     pininfo[physical_pin].flags.output_capable = 1;
//     pininfo[physical_pin].flags.pullup_capable = 1;
//     pininfo[physical_pin].flags.pulldown_capable = 1;
//     pininfo[physical_pin].flags.pwm_capable = 1;
//   }

//   #ifdef ESP8266
//   /*
//    * ESP8266 flash pins.
//    */
//   for(uint8_t physical_pin = 6; physical_pin <= 11 && physical_pin < MAX_GPIO_PIN; physical_pin++)
//   {
//     pininfo[physical_pin].flags.flash_reserved = 1;
//     pininfo[physical_pin].flags.board_reserved = 1;
//     pininfo[physical_pin].flags.hidden = 1;
//     pininfo[physical_pin].flags.input_capable = 0;
//     pininfo[physical_pin].flags.output_capable = 0;
//     pininfo[physical_pin].flags.pullup_capable = 0;
//     pininfo[physical_pin].flags.pulldown_capable = 0;
//     pininfo[physical_pin].flags.pwm_capable = 0;
//   }

//   /*
//    * GPIO16 is special on ESP8266.
//    */
//   if(16 < MAX_GPIO_PIN)
//   {
//     pininfo[16].flags.pulldown_capable = 0;
//   }
//   #endif


//   #ifdef ESP32
//   /*
//    * Common ESP32 flash pins. This is intentionally conservative for now.
//    */
//   for(uint8_t physical_pin = 6; physical_pin <= 11 && physical_pin < MAX_GPIO_PIN; physical_pin++)
//   {
//     pininfo[physical_pin].flags.flash_reserved = 1;
//     pininfo[physical_pin].flags.board_reserved = 1;
//     pininfo[physical_pin].flags.hidden = 1;
//     pininfo[physical_pin].flags.input_capable = 0;
//     pininfo[physical_pin].flags.output_capable = 0;
//     pininfo[physical_pin].flags.pullup_capable = 0;
//     pininfo[physical_pin].flags.pulldown_capable = 0;
//     pininfo[physical_pin].flags.pwm_capable = 0;
//   }

//   /*
//    * Classic ESP32 input-only pins.
//    * Guarded so this also compiles for variants with lower MAX_GPIO_PIN.
//    */
//   for(uint8_t physical_pin = 34; physical_pin <= 39 && physical_pin < MAX_GPIO_PIN; physical_pin++)
//   {
//     pininfo[physical_pin].flags.output_capable = 0;
//     pininfo[physical_pin].flags.pwm_capable = 0;
//     pininfo[physical_pin].flags.input_only = 1;
//   }

//   /*
//    * Boot-sensitive examples. Refine per target/board later.
//    */
//   if(0 < MAX_GPIO_PIN)  { pininfo[0].flags.boot_sensitive = 1; }
//   if(2 < MAX_GPIO_PIN)  { pininfo[2].flags.boot_sensitive = 1; }
//   if(12 < MAX_GPIO_PIN) { pininfo[12].flags.boot_sensitive = 1; }
//   if(15 < MAX_GPIO_PIN) { pininfo[15].flags.boot_sensitive = 1; }
//   #endif


//   /*
//    * Mark user-configurable pins from the current compiled template mapping.
//    */
//   for(uint8_t index = 0; index < MAX_USER_PINS; index++)
//   {
//     const uint8_t physical_pin = gpio_pin_by_index[index];

//     if(physical_pin >= MAX_GPIO_PIN)
//     {
//       continue;
//     }

//     pininfo[physical_pin].flags.user_configurable = 1;
//   }
// }


// void mPins::PinAlloc_InitAllocationTable(void)
// {
//   for(uint8_t physical_pin = 0; physical_pin < MAX_GPIO_PIN; physical_pin++)
//   {
//     pin_index_by_physical[physical_pin] = -1;
//   }

//   for(uint8_t index = 0; index < MAX_USER_PINS; index++)
//   {
//     PinAlloc_ClearAllocation(&pinalloc[index]);

//     const uint8_t physical_pin = gpio_pin_by_index[index];

//     pinalloc[index].physical_pin = physical_pin;

//     if(physical_pin < MAX_GPIO_PIN)
//     {
//       pin_index_by_physical[physical_pin] = index;

//       /*
//        * Mirror physical restrictions into allocation flags.
//        */
//       pinalloc[index].flags.reserved_by_board = pininfo[physical_pin].flags.board_reserved;
//       pinalloc[index].flags.disabled = !pininfo[physical_pin].flags.user_configurable;
//     }
//   }
// }


// void mPins::Init_InsertDummyPinAllocations(void)
// {
//   /*
//    * These are deliberately demo allocations.
//    * They should be replaced by real RequestPin() calls later.
//    */

//   #ifdef ESP8266
//   PinAlloc_AssignDummy(0, PINALLOC_DUMMY_FUNCTION_BUTTON_ID,     PINALLOC_DUMMY_OWNER_BUTTON_ID, PIN_DIRECTION_INPUT_ID,        PIN_SHARE_EXCLUSIVE_ID,  false, false, false, false);
//   PinAlloc_AssignDummy(2, PINALLOC_DUMMY_FUNCTION_STATUS_LED_ID, PINALLOC_DUMMY_OWNER_LED_ID,    PIN_DIRECTION_OUTPUT_ID,       PIN_SHARE_EXCLUSIVE_ID,  false, false, true,  true);
//   PinAlloc_AssignDummy(4, PINALLOC_DUMMY_FUNCTION_I2C_SDA_ID,    PINALLOC_DUMMY_OWNER_I2C_ID,    PIN_DIRECTION_INPUT_OUTPUT_ID, PIN_SHARE_SHARED_BUS_ID, true,  true,  true,  true);
//   PinAlloc_AssignDummy(5, PINALLOC_DUMMY_FUNCTION_I2C_SCL_ID,    PINALLOC_DUMMY_OWNER_I2C_ID,    PIN_DIRECTION_INPUT_OUTPUT_ID, PIN_SHARE_SHARED_BUS_ID, true,  true,  true,  true);
//   PinAlloc_AssignDummy(12, PINALLOC_DUMMY_FUNCTION_RELAY_ID,     PINALLOC_DUMMY_OWNER_RELAY_ID,  PIN_DIRECTION_OUTPUT_ID,       PIN_SHARE_EXCLUSIVE_ID,  false, false, false, true);
//   PinAlloc_AssignDummy(13, PINALLOC_DUMMY_FUNCTION_SWITCH_ID,    PINALLOC_DUMMY_OWNER_SWITCH_ID, PIN_DIRECTION_INPUT_ID,        PIN_SHARE_EXCLUSIVE_ID,  false, false, false, false);
//   #endif


//   #ifdef ESP32
//   /*
//    * Generic ESP32/C3/S3 style demo.
//    * These calls are safe: if a pin is not user-configurable for the current board,
//    * PinAlloc_AssignDummy() will ignore it.
//    */

//   PinAlloc_AssignDummy(2,  PINALLOC_DUMMY_FUNCTION_STATUS_LED_ID, PINALLOC_DUMMY_OWNER_LED_ID,    PIN_DIRECTION_OUTPUT_ID,       PIN_SHARE_EXCLUSIVE_ID,  false, false, true,  true);

//   PinAlloc_AssignDummy(4,  PINALLOC_DUMMY_FUNCTION_RELAY_ID,      PINALLOC_DUMMY_OWNER_RELAY_ID,  PIN_DIRECTION_OUTPUT_ID,       PIN_SHARE_EXCLUSIVE_ID,  false, false, false, true);
//   PinAlloc_AssignDummy(5,  PINALLOC_DUMMY_FUNCTION_BUTTON_ID,     PINALLOC_DUMMY_OWNER_BUTTON_ID, PIN_DIRECTION_INPUT_ID,        PIN_SHARE_EXCLUSIVE_ID,  false, false, false, false);

//   PinAlloc_AssignDummy(21, PINALLOC_DUMMY_FUNCTION_I2C_SDA_ID,    PINALLOC_DUMMY_OWNER_I2C_ID,    PIN_DIRECTION_INPUT_OUTPUT_ID, PIN_SHARE_SHARED_BUS_ID, true,  true,  true,  true);
//   PinAlloc_AssignDummy(22, PINALLOC_DUMMY_FUNCTION_I2C_SCL_ID,    PINALLOC_DUMMY_OWNER_I2C_ID,    PIN_DIRECTION_INPUT_OUTPUT_ID, PIN_SHARE_SHARED_BUS_ID, true,  true,  true,  true);

//   PinAlloc_AssignDummy(1,  PINALLOC_DUMMY_FUNCTION_SERIAL_TX_ID,  PINALLOC_DUMMY_OWNER_SERIAL_ID, PIN_DIRECTION_ALT_FUNCTION_ID,  PIN_SHARE_EXCLUSIVE_ID,  true,  false, false, false);
//   PinAlloc_AssignDummy(3,  PINALLOC_DUMMY_FUNCTION_SERIAL_RX_ID,  PINALLOC_DUMMY_OWNER_SERIAL_ID, PIN_DIRECTION_ALT_FUNCTION_ID,  PIN_SHARE_EXCLUSIVE_ID,  true,  false, false, false);

//   PinAlloc_AssignDummy(18, PINALLOC_DUMMY_FUNCTION_SPI_SCK_ID,    PINALLOC_DUMMY_OWNER_SPI_ID,    PIN_DIRECTION_ALT_FUNCTION_ID,  PIN_SHARE_SHARED_BUS_ID, true,  true,  false, false);
//   PinAlloc_AssignDummy(19, PINALLOC_DUMMY_FUNCTION_SPI_MISO_ID,   PINALLOC_DUMMY_OWNER_SPI_ID,    PIN_DIRECTION_ALT_FUNCTION_ID,  PIN_SHARE_SHARED_BUS_ID, true,  true,  false, false);
//   PinAlloc_AssignDummy(23, PINALLOC_DUMMY_FUNCTION_SPI_MOSI_ID,   PINALLOC_DUMMY_OWNER_SPI_ID,    PIN_DIRECTION_ALT_FUNCTION_ID,  PIN_SHARE_SHARED_BUS_ID, true,  true,  false, false);
//   #endif
// }


// bool mPins::PinAlloc_AssignDummy(
//   uint8_t physical_pin,
//   uint16_t function_id,
//   uint16_t owner_id,
//   uint8_t direction,
//   uint8_t share_mode,
//   bool grouped,
//   bool shared,
//   bool locked,
//   bool required
// ){
//   PinAllocation* allocation = PinAlloc_GetByPhysicalPin(physical_pin);

//   if(!allocation)
//   {
//     return false;
//   }

//   PinPhysicalInfo* info = PinInfo_GetByPhysicalPin(physical_pin);

//   if(!info)
//   {
//     return false;
//   }

//   if(!info->flags.user_configurable)
//   {
//     return false;
//   }

//   allocation->function_id = function_id;
//   allocation->owner_id = owner_id;
//   allocation->direction = direction;
//   allocation->share_mode = share_mode;
//   allocation->user_count = shared ? 2 : 1;

//   allocation->flags.allocated = 1;
//   allocation->flags.grouped = grouped ? 1 : 0;
//   allocation->flags.shared = shared ? 1 : 0;
//   allocation->flags.locked = locked ? 1 : 0;
//   allocation->flags.required = required ? 1 : 0;
//   allocation->flags.from_dummy = 1;
//   allocation->flags.reserved_by_board = info->flags.board_reserved;

//   /*
//    * Soft conflict checks for early visualisation.
//    */
//   if((direction == PIN_DIRECTION_OUTPUT_ID) && !info->flags.output_capable)
//   {
//     allocation->flags.conflict = 1;
//   }

//   if((direction == PIN_DIRECTION_INPUT_ID) && !info->flags.input_capable)
//   {
//     allocation->flags.conflict = 1;
//   }

//   if(info->flags.board_reserved || info->flags.flash_reserved)
//   {
//     allocation->flags.conflict = 1;
//   }

//   return true;
// }


// void mPins::PinAlloc_ClearAllocation(PinAllocation* allocation)
// {
//   if(!allocation)
//   {
//     return;
//   }

//   allocation->physical_pin = -1;
//   allocation->function_id = PIN_FUNCTION_NONE_ID;
//   allocation->owner_id = PIN_OWNER_NONE_ID;
//   allocation->direction = PIN_DIRECTION_UNKNOWN_ID;
//   allocation->share_mode = PIN_SHARE_UNKNOWN_ID;
//   allocation->user_count = 0;
//   allocation->flags.data = 0;
// }


// void mPins::PinAlloc_ClearAllAllocations(void)
// {
//   for(uint8_t index = 0; index < MAX_USER_PINS; index++)
//   {
//     const int8_t physical_pin = pinalloc[index].physical_pin;

//     PinAlloc_ClearAllocation(&pinalloc[index]);

//     pinalloc[index].physical_pin = physical_pin;
//   }
// }


// /******************************************************************************************************************
//  * Pin Allocation Accessors
// *******************************************************************************************************************/

// mPins::PinPhysicalInfo* mPins::PinInfo_GetByPhysicalPin(uint8_t physical_pin)
// {
//   if(physical_pin >= MAX_GPIO_PIN)
//   {
//     return nullptr;
//   }

//   return &pininfo[physical_pin];
// }


// mPins::PinAllocation* mPins::PinAlloc_GetByIndex(uint8_t index)
// {
//   if(index >= MAX_USER_PINS)
//   {
//     return nullptr;
//   }

//   return &pinalloc[index];
// }


// mPins::PinAllocation* mPins::PinAlloc_GetByPhysicalPin(uint8_t physical_pin)
// {
//   if(physical_pin >= MAX_GPIO_PIN)
//   {
//     return nullptr;
//   }

//   const int8_t index = pin_index_by_physical[physical_pin];

//   if(index < 0)
//   {
//     return nullptr;
//   }

//   if(index >= MAX_USER_PINS)
//   {
//     return nullptr;
//   }

//   return &pinalloc[index];
// }


// const mPins::PinPhysicalInfo* mPins::PinInfo_GetByPhysicalPin_Const(uint8_t physical_pin) const
// {
//   if(physical_pin >= MAX_GPIO_PIN)
//   {
//     return nullptr;
//   }

//   return &pininfo[physical_pin];
// }


// const mPins::PinAllocation* mPins::PinAlloc_GetByIndex_Const(uint8_t index) const
// {
//   if(index >= MAX_USER_PINS)
//   {
//     return nullptr;
//   }

//   return &pinalloc[index];
// }


// const mPins::PinAllocation* mPins::PinAlloc_GetByPhysicalPin_Const(uint8_t physical_pin) const
// {
//   if(physical_pin >= MAX_GPIO_PIN)
//   {
//     return nullptr;
//   }

//   const int8_t index = pin_index_by_physical[physical_pin];

//   if(index < 0)
//   {
//     return nullptr;
//   }

//   if(index >= MAX_USER_PINS)
//   {
//     return nullptr;
//   }

//   return &pinalloc[index];
// }


// bool mPins::PinAlloc_IsPhysicalPinKnown(uint8_t physical_pin) const
// {
//   if(physical_pin >= MAX_GPIO_PIN)
//   {
//     return false;
//   }

//   return pininfo[physical_pin].flags.valid;
// }


// bool mPins::PinAlloc_IsPhysicalPinUserConfigurable(uint8_t physical_pin) const
// {
//   if(physical_pin >= MAX_GPIO_PIN)
//   {
//     return false;
//   }

//   return pininfo[physical_pin].flags.user_configurable;
// }


// /******************************************************************************************************************
//  * Pin Allocation Names
// *******************************************************************************************************************/

// const char* mPins::PinAlloc_GetDirectionName(uint8_t direction, char* buffer, uint8_t buflen) const
// {
//   if(!buffer || buflen == 0)
//   {
//     return "";
//   }

//   switch(direction)
//   {
//     case PIN_DIRECTION_INPUT_ID:
//       snprintf(buffer, buflen, "IN");
//       break;

//     case PIN_DIRECTION_OUTPUT_ID:
//       snprintf(buffer, buflen, "OUT");
//       break;

//     case PIN_DIRECTION_INPUT_OUTPUT_ID:
//       snprintf(buffer, buflen, "I/O");
//       break;

//     case PIN_DIRECTION_ALT_FUNCTION_ID:
//       snprintf(buffer, buflen, "ALT");
//       break;

//     case PIN_DIRECTION_ANALOG_ID:
//       snprintf(buffer, buflen, "ANA");
//       break;

//     default:
//       snprintf(buffer, buflen, "-");
//       break;
//   }

//   return buffer;
// }


// const char* mPins::PinAlloc_GetShareModeName(uint8_t share_mode, char* buffer, uint8_t buflen) const
// {
//   if(!buffer || buflen == 0)
//   {
//     return "";
//   }

//   switch(share_mode)
//   {
//     case PIN_SHARE_EXCLUSIVE_ID:
//       snprintf(buffer, buflen, "EXCL");
//       break;

//     case PIN_SHARE_SHARED_BUS_ID:
//       snprintf(buffer, buflen, "BUS");
//       break;

//     case PIN_SHARE_SHARED_READ_ID:
//       snprintf(buffer, buflen, "READ");
//       break;

//     case PIN_SHARE_INTERNAL_ID:
//       snprintf(buffer, buflen, "INT");
//       break;

//     default:
//       snprintf(buffer, buflen, "-");
//       break;
//   }

//   return buffer;
// }


// const char* mPins::PinAlloc_GetFunctionName(uint16_t function_id, char* buffer, uint8_t buflen) const
// {
//   if(!buffer || buflen == 0)
//   {
//     return "";
//   }

//   switch(function_id)
//   {
//     case PIN_FUNCTION_NONE_ID:                    snprintf(buffer, buflen, "None");       break;
//     case PINALLOC_DUMMY_FUNCTION_RELAY_ID:        snprintf(buffer, buflen, "Relay");      break;
//     case PINALLOC_DUMMY_FUNCTION_BUTTON_ID:       snprintf(buffer, buflen, "Button");     break;
//     case PINALLOC_DUMMY_FUNCTION_SWITCH_ID:       snprintf(buffer, buflen, "Switch");     break;
//     case PINALLOC_DUMMY_FUNCTION_STATUS_LED_ID:   snprintf(buffer, buflen, "StatusLED");  break;
//     case PINALLOC_DUMMY_FUNCTION_I2C_SDA_ID:      snprintf(buffer, buflen, "I2C_SDA");    break;
//     case PINALLOC_DUMMY_FUNCTION_I2C_SCL_ID:      snprintf(buffer, buflen, "I2C_SCL");    break;
//     case PINALLOC_DUMMY_FUNCTION_SERIAL_TX_ID:    snprintf(buffer, buflen, "UART_TX");    break;
//     case PINALLOC_DUMMY_FUNCTION_SERIAL_RX_ID:    snprintf(buffer, buflen, "UART_RX");    break;
//     case PINALLOC_DUMMY_FUNCTION_SPI_SCK_ID:      snprintf(buffer, buflen, "SPI_SCK");    break;
//     case PINALLOC_DUMMY_FUNCTION_SPI_MISO_ID:     snprintf(buffer, buflen, "SPI_MISO");   break;
//     case PINALLOC_DUMMY_FUNCTION_SPI_MOSI_ID:     snprintf(buffer, buflen, "SPI_MOSI");   break;

//     default:
//       snprintf(buffer, buflen, "F%u", function_id);
//       break;
//   }

//   return buffer;
// }


// const char* mPins::PinAlloc_GetOwnerName(uint16_t owner_id, char* buffer, uint8_t buflen) const
// {
//   if(!buffer || buflen == 0)
//   {
//     return "";
//   }

//   switch(owner_id)
//   {
//     case PIN_OWNER_NONE_ID:              snprintf(buffer, buflen, "None");     break;
//     case PINALLOC_DUMMY_OWNER_RELAY_ID:  snprintf(buffer, buflen, "mRelays");  break;
//     case PINALLOC_DUMMY_OWNER_BUTTON_ID: snprintf(buffer, buflen, "mButtons"); break;
//     case PINALLOC_DUMMY_OWNER_SWITCH_ID: snprintf(buffer, buflen, "mSwitch");  break;
//     case PINALLOC_DUMMY_OWNER_LED_ID:    snprintf(buffer, buflen, "mLEDs");    break;
//     case PINALLOC_DUMMY_OWNER_I2C_ID:    snprintf(buffer, buflen, "mI2C");     break;
//     case PINALLOC_DUMMY_OWNER_SERIAL_ID: snprintf(buffer, buflen, "mSerial");  break;
//     case PINALLOC_DUMMY_OWNER_SPI_ID:    snprintf(buffer, buflen, "mSPI");     break;

//     default:
//       snprintf(buffer, buflen, "O%u", owner_id);
//       break;
//   }

//   return buffer;
// }

/******************************************************************************************************************
 * Pin Allocation Debug Splash
*******************************************************************************************************************/

#ifdef ENABLE_DEBUGFEATURE_SPLASH__PIN_ALLOCATIONS

void mPins::Splash__PinAllocations(void)
{
  Serial.println();
  Serial.println(F("==============================================================="));
  Serial.println(F("PIN ALLOCATIONS"));
  Serial.println(F("==============================================================="));
  Serial.println(F("Pin Usr Phy  Fn        Own       Dir  Shr  U  Flags"));
  Serial.println(F("--- --- ---- --------- --------- ---- ---- -- ---------------"));

  for(uint8_t physical_pin = 0; physical_pin < MAX_GPIO_PIN; physical_pin++)
  {
    const PinPhysicalInfo* info = PinInfo_GetByPhysicalPin_Const(physical_pin);
    const PinAllocation* allocation = PinAlloc_GetByPhysicalPin_Const(physical_pin);

    if(!info)
    {
      continue;
    }

    char function_name[10];
    char owner_name[10];
    char direction_name[5];
    char share_name[5];
    char flags[16];

    function_name[0] = 0;
    owner_name[0] = 0;
    direction_name[0] = 0;
    share_name[0] = 0;
    flags[0] = 0;

    if(allocation)
    {
      PinAlloc_GetFunctionName(allocation->function_id, function_name, sizeof(function_name));
      PinAlloc_GetOwnerName(allocation->owner_id, owner_name, sizeof(owner_name));
      PinAlloc_GetDirectionName(allocation->direction, direction_name, sizeof(direction_name));
      PinAlloc_GetShareModeName(allocation->share_mode, share_name, sizeof(share_name));

      uint8_t pos = 0;

      if(allocation->flags.allocated && pos < sizeof(flags)-1) { flags[pos++] = 'A'; }
      if(allocation->flags.locked && pos < sizeof(flags)-1) { flags[pos++] = 'L'; }
      if(allocation->flags.reserved_by_board && pos < sizeof(flags)-1) { flags[pos++] = 'R'; }
      if(allocation->flags.conflict && pos < sizeof(flags)-1) { flags[pos++] = 'C'; }
      if(allocation->flags.grouped && pos < sizeof(flags)-1) { flags[pos++] = 'G'; }
      if(allocation->flags.shared && pos < sizeof(flags)-1) { flags[pos++] = 'S'; }
      if(allocation->flags.required && pos < sizeof(flags)-1) { flags[pos++] = 'Q'; }
      if(allocation->flags.from_dummy && pos < sizeof(flags)-1) { flags[pos++] = 'D'; }
      if(allocation->flags.disabled && pos < sizeof(flags)-1) { flags[pos++] = 'X'; }
      if(allocation->flags.sensitive_to_probe && pos < sizeof(flags)-1) { flags[pos++] = 'P'; }

      flags[pos] = 0;
    }
    else
    {
      snprintf(function_name, sizeof(function_name), "-");
      snprintf(owner_name, sizeof(owner_name), "-");
      snprintf(direction_name, sizeof(direction_name), "-");
      snprintf(share_name, sizeof(share_name), "-");

      uint8_t pos = 0;

      if(info->flags.valid && pos < sizeof(flags)-1) { flags[pos++] = 'V'; }
      if(info->flags.user_configurable && pos < sizeof(flags)-1) { flags[pos++] = 'U'; }
      if(info->flags.input_capable && pos < sizeof(flags)-1) { flags[pos++] = 'I'; }
      if(info->flags.output_capable && pos < sizeof(flags)-1) { flags[pos++] = 'O'; }
      if(info->flags.flash_reserved && pos < sizeof(flags)-1) { flags[pos++] = 'F'; }
      if(info->flags.board_reserved && pos < sizeof(flags)-1) { flags[pos++] = 'B'; }
      if(info->flags.boot_sensitive && pos < sizeof(flags)-1) { flags[pos++] = 'T'; }
      if(info->flags.hidden && pos < sizeof(flags)-1) { flags[pos++] = 'H'; }

      flags[pos] = 0;
    }

    const int8_t index = (physical_pin < MAX_GPIO_PIN) ? pin_index_by_physical[physical_pin] : -1;

    Serial.printf(
      PSTR("%3u %3d %04X %-9s %-9s %-4s %-4s %2u %-15s\n\r"),
      physical_pin,
      index,
      info->flags.data,
      function_name,
      owner_name,
      direction_name,
      share_name,
      allocation ? allocation->user_count : 0,
      flags
    );
  }

  Serial.println(F("--- --- ---- --------- --------- ---- ---- -- ---------------"));
  Serial.println(F("Flags: A=Alloc L=Lock R=Rsv C=Confl G=Group S=Shared"));
  Serial.println(F("       Q=Req D=Dummy X=Dis V=Valid U=User I=In O=Out"));
  Serial.println(F("       F=Flash B=Board T=Boot H=Hidden"));
  Serial.println(F("==============================================================="));
  Serial.println();
}

#endif // ENABLE_DEBUGFEATURE_SPLASH__PIN_ALLOCATIONS