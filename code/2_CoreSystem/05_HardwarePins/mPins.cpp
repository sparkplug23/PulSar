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
    GPIO_KEY1,                // GPIO00 Button
    GPIO_USER,                // GPIO01 Serial RXD and Optional sensor
    GPIO_LED2_INV,            // GPIO02 Only available on newer Sonoff Basic R2 V1
    GPIO_USER,                // GPIO03 Serial TXD and Optional sensor
    GPIO_USER,                // GPIO04 Optional sensor
    0,                        // GPIO05
                              // GPIO06 (SD_CLK   Flash)
                              // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                              // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
                              // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
                              // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                              // GPIO11 (SD_CMD   Flash)
    GPIO_REL1,                // GPIO12 Red Led and Relay (0 = Off,    1 = On)
    GPIO_LED1_INV,            // GPIO13 Green Led (0 = On, 1 = Off) - Link and Power status
    GPIO_USER,                // GPIO14 Optional sensor
    0,                        // GPIO15
    0,                        // GPIO16
    0                         // ADC0 Analog input
  },
  #endif
  #ifdef USE_MODULE_TEMPLATE_SONOFF_BASIC_EXTERNAL
  {                   // MODULE_SONOFF_BASIC_EXTERNAL_ID    // Sonoff Basic (ESP8266)
    GPIO_KEY1,        // GPIO00 Button
    GPIO_USER,        // GPIO01 Serial RXD and Optional sensor
    GPIO_LED2_INV,    // GPIO02 Only available on newer Sonoff Basic R2 V1
    GPIO_KEY2,        // GPIO03 Serial TXD and Optional sensor
    GPIO_USER,        // GPIO04 Optional sensor
    0,                // GPIO05
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
                      // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
                      // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL1,        // GPIO12 Red Led and Relay (0 = Off,    1 = On)
    GPIO_LED1_INV,    // GPIO13 Green Led (0 = On, 1 = Off) - Link and Power status
    GPIO_USER,        // GPIO14 Optional sensor
    0,                // GPIO15
    0,                // GPIO16
    0                 // ADC0 Analog input
  },
  #endif
  #ifdef USE_MODULE_TEMPLATE_H801
  {                      // MODULE_H801,               // Lixada H801 Wifi (ESP8266)
    GPIO_KEY1,           // GPIO00 E-FW Button   // TEMPORARILY FORCING AS A KEY1 HERE, REMOVE WHEN GPIOC IS PROPERLY ADDED INTO CODE
    GPIO_LED1,           // GPIO01 Green LED - Link and Power status
    GPIO_USER,           // GPIO02 TX and Optional sensor - Pin next to TX on the PCB
    GPIO_USER,           // GPIO03 RX and Optional sensor - Pin next to GND on the PCB
    GPIO_PWM5,           // GPIO04 W2 - PWM5
    GPIO_LED2_INV,       // GPIO05 Red LED
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
                      // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
                      // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_PWM3,           // GPIO12 Blue
    GPIO_PWM2,           // GPIO13 Green
    GPIO_PWM4,           // GPIO14 W1 - PWM4
    GPIO_PWM1,           // GPIO15 Red
    0, // GPIO16
    0 // A0
  },
  #endif
  #ifdef USE_MODULE_TEMPLATE_MAGICHOME
  { //MODULE_MAGICHOME,          // Magic Home (aka Flux-light) (ESP8266) and Arilux LC10 (ESP8285)
                      // https://www.aliexpress.com/item/Magic-Home-Mini-RGB-RGBW-Wifi-Controller-For-Led-Strip-Panel-light-Timing-Function-16million-colors/32686853650.html
    0,
    GPIO_USER,           // GPIO01 Serial RXD and Optional sensor
    GPIO_LED1_INV,       // GPIO02 Blue onboard LED - Link and Power status
    GPIO_USER,           // GPIO03 Serial TXD and Optional sensor
    GPIO_ARIRFRCV,       // GPIO04 IR or RF receiver (optional) (Arilux LC10)
    GPIO_PWM2,           // GPIO05 RGB LED Green
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
                      // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
                      // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_PWM3,           // GPIO12 RGB LED Blue
    GPIO_USER,           // GPIO13 RGBW LED White (optional - set to PWM4 for Cold White or Warm White as used on Arilux LC10)
    GPIO_PWM1,           // GPIO14 RGB LED Red
    GPIO_LED4_INV,       // GPIO15 RF receiver control (Arilux LC10)
    0, 0
  },
  #endif
  #ifdef USE_MODULE_TEMPLATE_SHELLY1
  {//MODULE_SHELLY1    // Shelly1 Open Source (ESP8266 - 2MB) - https://shelly.cloud/shelly1-open-source/
     0,                // GPIO00 - Can be changed to GPIO_USER, only if Shelly is powered with 12V DC
     0,                // GPIO01 Serial RXD - Can be changed to GPIO_USER, only if Shelly is powered with 12V DC
     0,
     0,                // GPIO03 Serial TXD - Can be changed to GPIO_USER, only if Shelly is powered with 12V DC
     GPIO_REL1,           // GPIO04 Relay (0 = Off, 1 = On)
     GPIO_SWT1_NP,        // GPIO05 SW pin
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
    GPIO_LED1_INV,        // GPIO00 LED1i
    0,                    // GPIO01 None
    GPIO_KEY1,            // GPIO02 Button1
    0,                    // GPIO03 None
    GPIO_REL1,            // GPIO04 Relay1
    GPIO_SWT2_NP,         // GPIO05 Switch2n
                          // GPIO06 (SD_CLK   Flash)
                          // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                          // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
                          // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
                          // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                          // GPIO11 (SD_CMD   Flash)
    GPIO_I2C_SDA,         // GPIO12 I2C_SDA
    GPIO_SWT1_NP,         // GPIO13 Switch1n
    GPIO_I2C_SCL,         // GPIO14 I2C_SCL
    GPIO_REL2,            // GPIO15 Relay2
    GPIO_ADE7953_IRQ,     // GPIO16 ADE7953_IRQ
    GPIO_FLAG_ADC0_TEMP   // GPIOA0 Tempurature
  },
  #endif
  #ifdef USE_MODULE_TEMPLATE_SHELLY_DIMMER2
  {//MODULE_SHELLY_DIMMER2     // SHELLY_DIMMER2 - Shelly Dimmer 2 (ESP8285)
    0,                         // GPIO00 None
    GPIO_HWSERIAL0_TX,         // GPIO01 ESP_TXD Serial RXD connection to Dimmer MCU
    0,                         // GPIO02 None 
    GPIO_HWSERIAL0_RX,         // GPIO03 ESP_RXD Serial TXD connection to Dimmer MCU
    GPIO_SHELLY2_SHD_BOOT0,    // GPIO04 SHD Boot 0 
    GPIO_SHELLY2_SHD_RESET_INV,    // GPIO05 SHD Reset
                               // GPIO06 (SD_CLK   Flash)
                               // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                               // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
                               // GPIO09 None 
                               // GPIO10 None 
                               // GPIO11 (SD_CMD   Flash)
    GPIO_SWT2_NP,              // GPIO12 Switch2n 
    0,                         // GPIO13 None 
    GPIO_SWT1_NP,              // GPIO14 Switch1n 
    0,                         // GPIO15 None
    GPIO_LED1_INV,             // GPIO16 Led1i 
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
    GPIO_KEY1,                 // GPIO00 WIFI_KEY0 Button 1
    GPIO_HWSERIAL0_TX,         // GPIO01 ESP_TXD Serial RXD connection to P0.5 of RF microcontroller
    0,                         // GPIO02 ESP_LOG
    GPIO_HWSERIAL0_RX,         // GPIO03 ESP_RXD Serial TXD connection to P0.4 of RF microcontroller
    0,                         // GPIO04 DEBUG_RX
    0,                         // GPIO05 DEBUG_TX
                               // GPIO06 (SD_CLK   Flash)
                               // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                               // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    GPIO_REL1_INV,             // GPIO09 WIFI_O0 Relay 1 (0 = Off, 1 = On) controlling the light
    GPIO_BUZZER_INV,           // GPIO10 WIFI_O4 Buzzer (0 = Off, 1 = On)
                               // GPIO11 (SD_CMD   Flash)
    GPIO_REL3,                 // GPIO12 WIFI_O2 Relay 3 (0 = Off, 1 = On) controlling the fan
    GPIO_LED1_INV,             // GPIO13 WIFI_CHK Blue Led on PCA (0 = On, 1 = Off) - Link and Power status
    GPIO_REL2,                 // GPIO14 WIFI_O1 Relay 2 (0 = Off, 1 = On) controlling the fan
    GPIO_REL4,                 // GPIO15 WIFI_O3 Relay 4 (0 = Off, 1 = On) controlling the fan
    0,                         // GPIO16 None 
    0                          // A0
  },
  #endif
  #ifdef USE_MODULE_TEMPLATE_SONOFF_4CHPRO  
  {                           // SONOFF_4CH - Sonoff 4CH (ESP8285)
    GPIO_KEY1,             // GPIO00 Button 1
    GPIO_USER,             // GPIO01 Serial RXD and Optional sensor
    GPIO_RF_433MHZ_RX,             // GPIO02 Optional sensor
    GPIO_USER,             // GPIO03 Serial TXD and Optional sensor
    GPIO_REL3,             // GPIO04 Sonoff 4CH Red Led and Relay 3 (0 = Off, 1 = On)
    GPIO_REL2,             // GPIO05 Sonoff 4CH Red Led and Relay 2 (0 = Off, 1 = On)
                        // GPIO06 (SD_CLK   Flash)
                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    GPIO_KEY2,             // GPIO09 Button 2
    GPIO_KEY3,             // GPIO10 Button 3
                        // GPIO11 (SD_CMD   Flash)
    GPIO_REL1,             // GPIO12 Red Led and Relay 1 (0 = Off, 1 = On) - Link and Power status
    GPIO_LED1_INV,         // GPIO13 Blue Led (0 = On, 1 = Off)
    GPIO_KEY4,             // GPIO14 Button 4
    GPIO_REL4,             // GPIO15 Red Led and Relay 4 (0 = Off, 1 = On)
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

  return FUNCTION_RESULT_UNKNOWN_ID;

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
    ALOG_INF( PSTR("GetRealPinNumberFromName = %d"), pin);
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


#ifdef ENABLE_DEBUFEATURE_HARDWAREPINS__ENABLE_DEBUG_ON_PINUSED
int IRAM_ATTR mPins::Pin(uint32_t gpio, uint32_t index, bool enable_debug)
#else
int16_t IRAM_ATTR mPins::Pin(uint32_t gpio, uint32_t index)
#endif
{
  #ifdef ENABLE_FEATURE_HARDWAREPINS__FUNCTION_AND_INDEX_PACKED_GPIO
  uint16_t real_gpio = gpio << 5;
  uint16_t mask = 0xFFE0;
  if (index < GPIO_ANY) {
    real_gpio += index;
    mask = 0xFFFF;
  }
  for (uint32_t i = 0; i < nitems(pin_attached_gpio_functions); i++) {
    if ((pin_attached_gpio_functions[i] & mask) == real_gpio) {
      return i;              // Pin number configured for gpio
    }
  }
  #else
  uint16_t real_gpio = gpio + index;
  for (uint32_t index_pin = 0; index_pin < nitems(pin_attached_gpio_functions); index_pin++) {
    if (pin_attached_gpio_functions[index_pin] == real_gpio) {
      // ALOG_INF(PSTR("Pin(%d,%d) pin_attached[%d] == real_gpio|%d==%d"), gpio,index, index_pin, pin_attached_gpio_functions[index_pin],real_gpio);
      return gpio_pin_by_index[index_pin];              // Pin number configured for gpio
    }
  }
  #endif
  return -1;                 // No pin used for gpio
}


#ifdef ENABLE_DEBUFEATURE_HARDWAREPINS__ENABLE_DEBUG_ON_PINUSED
boolean mPins::PinUsed(uint32_t gpio, uint32_t index, bool enable_debug)
{
  return (Pin(gpio, index, enable_debug) >= 0);
}
#else
boolean mPins::PinUsed(uint32_t gpio, uint32_t index)
{
  return (Pin(gpio, index) >= 0);
}
#endif


/**
 * @brief Assigns a pin with a GPIO
 * @note 
 * @param lpin The real_pin number GPIO#
 * @param gpio The actual hardware attached to the pin (eg led, relay, sensor data)
 * @return none
 */
void mPins::SetPin(uint32_t real_pin, uint32_t gpio) 
{
  // if(lpin < ARRAY_SIZE(pin_attached_gpio_functions)){


  //   pin_attached_gpio_functions[internal_pin_index] = gpio;

  // }else{

  // }

  // ALOG_INF( PSTR("SetPin real_pin=%d  internal_index=%d gpio=%d"), real_pin, gpio); Serial.flush();

  // Adjust real_pin to internal indexpin
  int8_t internal_pin_index = ConvertRealPinToIndexPin(real_pin);

  ALOG_INF( PSTR("SetPinAreal_pin=%d  internal_index=%d gpio=%d"),real_pin,internal_pin_index,gpio);

  // delay(1000);
  // Serial.printf("this = %p\n", this);


  // Serial.println(internal_pin_index);
  // delay(1000);

  

  // Serial.println(pin_attached_gpio_functions[0]);
  // Serial.println(pin_attached_gpio_functions[1]);
  // delay(1000);

  // Serial.println(pin_attached_gpio_functions[(uint8_t)internal_pin_index]);
  // delay(1000);
  // uint8_t pin_count = ARRAY_SIZE(gpio_pin_by_index);

  // AddLog_Array(LOG_LEVEL_INFO, "gpio_pin_by_index", gpio_pin_by_index, ARRAY_SIZE(gpio_pin_by_index));
  // AddLog_Array(LOG_LEVEL_INFO, "pin_attached_gpio_functions", pin_attached_gpio_functions, ARRAY_SIZE(pin_attached_gpio_functions));

  if(internal_pin_index != -1) // -1 means unset pin
  {
    pin_attached_gpio_functions[internal_pin_index] = gpio;
    // ALOG_INF( PSTR(DEBUG_INSERT_PAGE_BREAK "SetPin real_pin=%d  internal_index=%d gpio=%d\t\n\r=====%d%d"),real_pin,internal_pin_index,gpio,pin_attached_gpio_functions[internal_pin_index],internal_pin_index);
  }
  else
  {
    ALOG_ERR( PSTR("Error SetPin %d %d<%d"), gpio, internal_pin_index, MAX_USER_PINS);
  }
  // ALOG_INF( PSTR("SetPin DONE real_pin=%d  internal_index=%d gpio=%d"), real_pin, gpio); Serial.flush();

}


int8_t mPins::ConvertIndexPinToRealPin(uint8_t index_pin){
  return gpio_pin_by_index[index_pin];
}


void mPins::DigitalWrite(uint32_t gpio_pin, uint32_t state)
{
  ALOG_ERR(PSTR("SHould never be here!!"));
  DigitalWrite(gpio_pin, 0, state);
  // if (tkr_set->pin[gpio_pin] < 99) {
        // AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_RELAYS "DigitalWrite(%d[%d],%d)"),tkr_set->pin[gpio_pin],gpio_pin,state);
    // digitalWrite(tkr_set->pin[gpio_pin], state &1);
  // }
}


void mPins::DigitalWrite(uint32_t gpio_pin, uint32_t index, uint32_t state)
{
  uint8_t real_pin = Pin(gpio_pin, index);
  ALOG_INF(PSTR("DigitalWrite gpio_pin=%d,real_pin=%d,index=%d,state=%d"),gpio_pin,real_pin, index, state &1);
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

bool mPins::ValidUserGPIOFunction(uint8_t* pin_array, uint8_t index)
{
  // Outside valid gpio function range
  if((pin_array[index] >= GPIO_SENSOR_END) && (pin_array[index] < GPIO_USER)){
      return false;
    }else{
      return true;
    }
}

bool mPins::ValidUserGPIOFunction(uint16_t* pin_array, uint8_t index)
{
  ALOG_ERR(PSTR("not in tas25 %d[%d]"), pin_array[index], index);
  // Outside valid gpio function range
  // if((pin_array[index] >= GPIO_SENSOR_END) && (pin_array[index] < GPIO_USER)){
  //     return false;
  //   }else{
      return true;
    // }
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
uint16_t mPins::ValidPin_AdjustGPIO(uint8_t pin, uint16_t gpio)
{
  uint16_t result = gpio;

  // // #ifdef ESP8266

  // // DEBUG_LINE;
  // // if (((pin > 5) && (pin < 9)) || (11 == pin)) {
  // //   result = GPIO_NONE_ID;  // Disable flash pins GPIO6, GPIO7, GPIO8 and GPIO11
  // // }
  // // DEBUG_LINE;
  // // // need to add my other boards here
  // // if ((MODULE_WEMOS_ID == tkr_set->Settings.module) && (!tkr_set->Settings.flag_network.user_esp8285_enable)) {
  // //   if ((pin == 9) || (pin == 10)) { result = GPIO_NONE_ID; }  // Disable possible flash GPIO9 and GPIO10
  // // }
  // // DEBUG_LINE;

  // // #endif

  // return result;

//   if (CheckPhysicalPinIsFlashPin(pin)) { // WRONG, BECAUSE IT IS NOT A GPIO PIN. This expects the physical pin number, not the GPIO function
//     return GPIO_NONE_ID;    // Disable flash pins GPIO6, GPIO7, GPIO8 and GPIO11
//   }

// #ifdef ESP8266
//   // if (((WEMOS == Settings->module) || isTuya) && !Settings->flag3.user_esp8285_enable) {  // SetOption51 - Enable ESP8285 user GPIO's
//   //   if ((9 == pin) || (10 == pin)) {
//   //     return GPIO_NONE_ID;  // Disable possible flash GPIO9 and GPIO10
//   //   }
//   // }
//    if ((MODULE_WEMOS_ID == tkr_set->Settings.module) && (!tkr_set->Settings.flag_network.user_esp8285_enable)) {
//     if ((pin == 9) || (pin == 10)) { gpio = GPIO_NONE_ID; }  // Disable possible flash GPIO9 and GPIO10
//   }
// #endif

  return gpio;
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
