#ifndef BusWrapper_h
#define BusWrapper_h

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

#include "NeoPixelBus.h"

#include "mHardwareColourType.h"

/**
 * @brief 
 * C3: I2S0 and I2S1 methods not supported (has one I2S bus)
 * S2: I2S1 methods not supported (has one I2S bus)
 * S3: I2S0 and I2S1 methods not supported yet (has two I2S buses)
 * https://github.com/Makuna/NeoPixelBus/blob/b32f719e95ef3c35c46da5c99538017ef925c026/src/internal/Esp32_i2s.h#L4
 * https://github.com/Makuna/NeoPixelBus/blob/b32f719e95ef3c35c46da5c99538017ef925c026/src/internal/NeoEsp32RmtMethod.h#L857
**/

#if !defined(NEOPIXEL_DISABLE_I2S0_PIXELBUS) && (defined(CONFIG_IDF_TARGET_ESP32S3) || defined(CONFIG_IDF_TARGET_ESP32C3))
#define NEOPIXEL_DISABLE_I2S0_PIXELBUS
#endif
#if !defined(NEOPIXEL_DISABLE_I2S1_PIXELBUS) && (defined(CONFIG_IDF_TARGET_ESP32S3) || defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32S2))
#define NEOPIXEL_DISABLE_I2S1_PIXELBUS
#endif

//Hardware SPI Pins
#define P_8266_HS_MOSI 13
#define P_8266_HS_CLK  14
#define P_32_HS_MOSI   13
#define P_32_HS_CLK    14
#define P_32_VS_MOSI   23
#define P_32_VS_CLK    18

enum EM_BUS_TYPE
{
  BUSTYPE__NONE__ID = 0,
  // ESP8266 RGB
  BUSTYPE__8266_U0_NEO_3__ID = 1,
  BUSTYPE__8266_U1_NEO_3__ID = 2,  // i want this
  BUSTYPE__8266_DM_NEO_3__ID = 3,
  BUSTYPE__8266_BB_NEO_3__ID = 4,
  // RGBW
  BUSTYPE__8266_U0_NEO_4__ID = 5,
  BUSTYPE__8266_U1_NEO_4__ID = 6,
  BUSTYPE__8266_DM_NEO_4__ID = 7,
  BUSTYPE__8266_BB_NEO_4__ID = 8,
  
  /*** ESP32 Neopixel methods ***/
  // RGB
  BUSTYPE__32_RN_NEO_3__ID = 21,
  BUSTYPE__32_I0_NEO_3__ID = 22,
  BUSTYPE__32_I1_NEO_3__ID = 23,
  BUSTYPE__32_BB_NEO_3__ID = 24,  // bitbangging on ESP32 not recommended
  BUSTYPE__32_8PARALELL_3__ID,
  BUSTYPE__32_16PARALLEL_3__ID,
  // RGBW
  BUSTYPE__32_RN_NEO_4__ID,
  BUSTYPE__32_I0_NEO_4__ID,
  BUSTYPE__32_I1_NEO_4__ID,
  BUSTYPE__32_BB_NEO_4__ID,  // not fully added, temp added to keep the ID working so X16 for RGBW should work
  BUSTYPE__32_8PARALELL_4__ID,
  BUSTYPE__32_16PARALLEL_4__ID,
  //
  BUSTYPE__32_RN_NEO_5__ID, 
  BUSTYPE__32_I0_NEO_5__ID,
  BUSTYPE__32_I1_NEO_5__ID,
  BUSTYPE__32_BB_NEO_5__ID,  // ERROR: NOT ADDED BUT KEEPING IN SO INDEXING RELATIVE TO RN IS THE SAME AS 3/4 PIXEL WIDTH
  BUSTYPE__32_8PARALELL_5__ID,
  BUSTYPE__32_16PARALLEL_5__ID,
  // 400Kbps
  BUSTYPE__32_RN_400_3__ID,
  BUSTYPE__32_I0_400_3__ID,
  BUSTYPE__32_I1_400_3__ID,
  BUSTYPE__32_BB_400_3__ID,  // bitbangging on ESP32 not recommended



  // TM1814 (RGBW)
  BUSTYPE__32_RN_TM1_4__ID,
  BUSTYPE__32_I0_TM1_4__ID,
  BUSTYPE__32_I1_TM1_4__ID,
  // TM1829 (RGB)
  BUSTYPE__32_RN_TM2_3__ID,
  BUSTYPE__32_I0_TM2_3__ID,
  BUSTYPE__32_I1_TM2_3__ID,
  // APA102
  BUSTYPE__HS_DOT_3__ID, // hardware SPI
  BUSTYPE__SS_DOT_3__ID, // soft SPI
  // LPD8806
  BUSTYPE__HS_LPD_3__ID,
  BUSTYPE__SS_LPD_3__ID,
  // WS2801
  BUSTYPE__HS_WS1_3__ID,
  BUSTYPE__SS_WS1_3__ID,
  // P9813
  BUSTYPE__HS_P98_3__ID,
  BUSTYPE__SS_P98_3__ID,
  // LPD6803
  BUSTYPE__HS_LPO_3__ID,
  BUSTYPE__SS_LPO_3__ID
};


#define ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
#define ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
#define ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES


// 
// #ifdef ENABLE_DEVFEATURE__NEOPIXELBUS_JULY_ONWARDS

/*** ESP8266 Neopixel methods ***/
#ifdef ESP8266
//RGB
#ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
#define NEOPIXELBUS_8266_U0_NEO_3 NeoPixelBus<NeoRgbFeature, NeoEsp8266Uart0Ws2813Method> //3 chan, esp8266, gpio1
#endif
#ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
#define NEOPIXELBUS_8266_U1_NEO_3 NeoPixelBus<NeoRgbFeature, NeoEsp8266Uart1Ws2813Method> //3 chan, esp8266, gpio2
#endif 
#ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
#define NEOPIXELBUS_8266_DM_NEO_3 NeoPixelBus<NeoRgbFeature, NeoEsp8266Dma800KbpsMethod>  //3 chan, esp8266, gpio3
#endif
// #define NEOPIXELBUS_8266_BB_NEO_3 NeoPixelBus<NeoRgbFeature, NeoEsp8266BitBang800KbpsMethod> //3 chan, esp8266, bb (any pin but 16)
//RGBW
#ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
#define NEOPIXELBUS_8266_U0_NEO_4 NeoPixelBus<NeoRgbwFeature, NeoEsp8266Uart0Ws2813Method>   //4 chan, esp8266, gpio1
#endif 
#ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
#define NEOPIXELBUS_8266_U1_NEO_4 NeoPixelBus<NeoRgbwFeature, NeoEsp8266Uart1Ws2813Method>   //4 chan, esp8266, gpio2
#endif
#ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
#define NEOPIXELBUS_8266_DM_NEO_4 NeoPixelBus<NeoRgbwFeature, NeoEsp8266Dma800KbpsMethod>    //4 chan, esp8266, gpio3
#endif
#endif

/*** ESP32 Neopixel methods ***/
#ifdef ARDUINO_ARCH_ESP32
//RGB
#define NEOPIXELBUS_32_RN_NEO_3 NeoPixelBus<NeoRgbFeature, NeoEsp32RmtNWs2812xMethod>
#ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
#define NEOPIXELBUS_32_I0_NEO_3 NeoPixelBus<NeoRgbFeature, NeoEsp32I2s0800KbpsMethod>
#endif
#ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
#define NEOPIXELBUS_32_I1_NEO_3 NeoPixelBus<NeoRgbFeature, NeoEsp32I2s1800KbpsMethod>
#endif
#ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS   // New parallel output
#define NEOPIXELBUS_32_8PARALLEL_NEO_3 NeoPixelBus<NeoRgbFeature, NeoEsp32I2s1X8Ws2812xMethod>
#endif
#ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS   // New parallel output
#define NEOPIXELBUS_32_16PARALLEL_NEO_3 NeoPixelBus<NeoRgbFeature, NeoEsp32I2s0X16Ws2812xMethod>
#endif

//RGBW
#define NEOPIXELBUS_32_RN_NEO_4 NeoPixelBus<NeoRgbwFeature, NeoEsp32RmtNWs2812xMethod>
#ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
#define NEOPIXELBUS_32_I0_NEO_4 NeoPixelBus<NeoRgbwFeature, NeoEsp32I2s0800KbpsMethod>
#endif
#ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
#define NEOPIXELBUS_32_I1_NEO_4 NeoPixelBus<NeoRgbwFeature, NeoEsp32I2s1800KbpsMethod>
#endif
#ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS   // New parallel output
#define NEOPIXELBUS_32_8PARALLEL_NEO_4 NeoPixelBus<NeoRgbwFeature, NeoEsp32I2s1X8Sk6812Method>
#endif
#ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS   // New parallel output
#define NEOPIXELBUS_32_16PARALLEL_NEO_4 NeoPixelBus<NeoRgbwFeature, NeoEsp32I2s0X16Sk6812Method>
#endif

#define ENABLE_DEVFEATURE__WS2805


#ifdef ENABLE_DEVFEATURE__WS2805
  //RGBWW (WS2805)
  // #define NEOPIXELBUS_32_RN_NEO_5 NeoPixelBus<NeoRgbFeature, NeoEsp32I2s0800KbpsMethod>//NeoEsp32RmtNWs2805Method>
  #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
  #define NEOPIXELBUS_32_I0_NEO_5 NeoPixelBus<NeoRgbwwFeature, NeoEsp32I2s0Ws2805Method>
  #endif
  #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
  #define NEOPIXELBUS_32_I1_NEO_5 NeoPixelBus<NeoRgbwwFeature, NeoEsp32I2s1Ws2805Method>
  #endif
  #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS   // New parallel output
  #define NEOPIXELBUS_32_8PARALLEL_NEO_5 NeoPixelBus<NeoRgbwwFeature, NeoEsp32I2s1X8Ws2805Method>
  #endif
  #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS   // New parallel output
  #define NEOPIXELBUS_32_16PARALLEL_NEO_5 NeoPixelBus<NeoRgbwwFeature, NeoEsp32I2s0X16Ws2805Method>
  #endif
#endif // ENABLE_DEVFEATURE__WS2805

//400Kbps
#define NEOPIXELBUS_32_RN_400_3 NeoPixelBus<NeoRgbFeature, NeoEsp32RmtN400KbpsMethod>
#ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
#define NEOPIXELBUS_32_I0_400_3 NeoPixelBus<NeoRgbFeature, NeoEsp32I2s0400KbpsMethod>
#endif
#ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
#define NEOPIXELBUS_32_I1_400_3 NeoPixelBus<NeoRgbFeature, NeoEsp32I2s1400KbpsMethod>
#endif
//TM1814 (RGBW)
#define NEOPIXELBUS_32_RN_TM1_4 NeoPixelBus<NeoWrgbTm1814Feature, NeoEsp32RmtNTm1814Method>
#ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
#define NEOPIXELBUS_32_I0_TM1_4 NeoPixelBus<NeoWrgbTm1814Feature, NeoEsp32I2s0Tm1814Method>
#endif
#ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
#define NEOPIXELBUS_32_I1_TM1_4 NeoPixelBus<NeoWrgbTm1814Feature, NeoEsp32I2s1Tm1814Method>
#endif
//Bit Bang theoratically possible, but very undesirable and not needed (no pin restrictions on RMT and I2S)
//TM1829 (RGB)
#define NEOPIXELBUS_32_RN_TM2_3 NeoPixelBus<NeoBrgFeature, NeoEsp32RmtNTm1829Method>
#ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
#define NEOPIXELBUS_32_I0_TM2_3 NeoPixelBus<NeoBrgFeature, NeoEsp32I2s0Tm1829Method>
#endif
#ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
#define NEOPIXELBUS_32_I1_TM2_3 NeoPixelBus<NeoBrgFeature, NeoEsp32I2s1Tm1829Method>
#endif
//Bit Bang theoratically possible, but very undesirable and not needed (no pin restrictions on RMT and I2S)
#endif

//APA102
// #define NEOPIXELBUS_HS_DOT_3 NeoPixelBus<DotStarBgrFeature, DotStarSpi5MhzMethod> //hardware SPI
// #define NEOPIXELBUS_SS_DOT_3 NeoPixelBus<DotStarBgrFeature, DotStarMethod>    //soft SPI

// //LPD6803
// #define NEOPIXELBUS_HS_LPO_3 NeoPixelBus<Lpd6803RgbFeature, Lpd6803SpiMethod>
// #define NEOPIXELBUS_SS_LPO_3 NeoPixelBus<Lpd6803RgbFeature, Lpd6803Method>

// handles pointer type conversion for all possible bus types
class PolyBus 
{
  public:
  // Begin & initialize the PixelSettings for TM1814 strips.
  template <class T>
  static void beginTM1814(void* busPtr) {
    T tm1814_strip = static_cast<T>(busPtr);
    tm1814_strip->Begin();
    // Max current for each LED (22.5 mA).
    // tm1814_strip->SetPixelSettings(NeoTm1814Settings(/*R*/225, /*G*/225, /*B*/225, /*W*/225));
  }
  static void begin(void* busPtr, uint8_t busType, uint8_t* pins) 
  {
    
    #ifdef ENABLE_DEBUGFEATURE__16PIN_PARALLEL_OUTPUT
    DEBUG_PRINTF("PolyBus::begin busPtr UNSET, busType %d, pin[0] %d\n\r", busType, pins[0]);
    #endif

    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
      #ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
      case BUSTYPE__8266_U0_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_U0_NEO_3*>(busPtr))->Begin(); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
      case BUSTYPE__8266_U1_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_U1_NEO_3*>(busPtr))->Begin(); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
      case BUSTYPE__8266_DM_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_DM_NEO_3*>(busPtr))->Begin(); break;
      #endif
      // case BUSTYPE__8266_BB_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_BB_NEO_3*>(busPtr))->Begin(); break;
      
      #ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
      case BUSTYPE__8266_U0_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_U0_NEO_4*>(busPtr))->Begin(); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
      case BUSTYPE__8266_U1_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_U1_NEO_4*>(busPtr))->Begin(); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
      case BUSTYPE__8266_DM_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_DM_NEO_4*>(busPtr))->Begin(); break;
      #endif
      // case BUSTYPE__8266_BB_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_BB_NEO_4*>(busPtr))->Begin(); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32

      case BUSTYPE__32_RN_NEO_3__ID: (static_cast<NEOPIXELBUS_32_RN_NEO_3*>(busPtr))->Begin(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_3__ID: (static_cast<NEOPIXELBUS_32_I0_NEO_3*>(busPtr))->Begin(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_3__ID: (static_cast<NEOPIXELBUS_32_I1_NEO_3*>(busPtr))->Begin(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_3__ID: (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_3*>(busPtr))->Begin(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_16PARALLEL_3__ID: (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_3*>(busPtr))->Begin(); break;
      #endif

      case BUSTYPE__32_RN_NEO_4__ID: (static_cast<NEOPIXELBUS_32_RN_NEO_4*>(busPtr))->Begin(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_4__ID: (static_cast<NEOPIXELBUS_32_I0_NEO_4*>(busPtr))->Begin(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_4__ID: (static_cast<NEOPIXELBUS_32_I1_NEO_4*>(busPtr))->Begin(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_4__ID: (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_4*>(busPtr))->Begin(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_16PARALLEL_4__ID: (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_4*>(busPtr))->Begin(); break;
      #endif

#ifdef ENABLE_DEVFEATURE__WS2805
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_5__ID: (static_cast<NEOPIXELBUS_32_I0_NEO_5*>(busPtr))->Begin(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_5__ID: (static_cast<NEOPIXELBUS_32_I1_NEO_5*>(busPtr))->Begin(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_5__ID: (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_5*>(busPtr))->Begin(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_16PARALLEL_5__ID: (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_5*>(busPtr))->Begin(); break;
      #endif
#endif // ENABLE_DEVFEATURE__WS2805

      case BUSTYPE__32_RN_400_3__ID: (static_cast<NEOPIXELBUS_32_RN_400_3*>(busPtr))->Begin(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_400_3__ID: (static_cast<NEOPIXELBUS_32_I0_400_3*>(busPtr))->Begin(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_400_3__ID: (static_cast<NEOPIXELBUS_32_I1_400_3*>(busPtr))->Begin(); break;
      #endif

      case BUSTYPE__32_RN_TM1_4__ID: beginTM1814<NEOPIXELBUS_32_RN_TM1_4*>(busPtr); break;
      case BUSTYPE__32_RN_TM2_3__ID: (static_cast<NEOPIXELBUS_32_RN_TM2_3*>(busPtr))->Begin(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_TM1_4__ID: beginTM1814<NEOPIXELBUS_32_I0_TM1_4*>(busPtr); break;
      case BUSTYPE__32_I0_TM2_3__ID: (static_cast<NEOPIXELBUS_32_I0_TM2_3*>(busPtr))->Begin(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_TM1_4__ID: beginTM1814<NEOPIXELBUS_32_I1_TM1_4*>(busPtr); break;
      case BUSTYPE__32_I1_TM2_3__ID: (static_cast<NEOPIXELBUS_32_I1_TM2_3*>(busPtr))->Begin(); break;
      #endif
    #endif
    }
  };
  static void* create(uint8_t busType, uint8_t* pins, uint16_t len, uint8_t channel) 
  {
    #ifdef ENABLE_DEBUGFEATURE__16PIN_PARALLEL_OUTPUT
    DEBUG_PRINTF("PolyBus::create busType %d, pin[0] %d, len %d, channel %d\n\r", busType, pins[0], len, channel);
    #endif
    
    void* busPtr = nullptr;
    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
      #ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
      case BUSTYPE__8266_U0_NEO_3__ID: busPtr = new NEOPIXELBUS_8266_U0_NEO_3(len, pins[0]); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
      case BUSTYPE__8266_U1_NEO_3__ID: busPtr = new NEOPIXELBUS_8266_U1_NEO_3(len, pins[0]); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
      case BUSTYPE__8266_DM_NEO_3__ID: busPtr = new NEOPIXELBUS_8266_DM_NEO_3(len, pins[0]); break;
      #endif
      // case BUSTYPE__8266_BB_NEO_3__ID: busPtr = new NEOPIXELBUS_8266_BB_NEO_3(len, pins[0]); break;
      #ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
      case BUSTYPE__8266_U0_NEO_4__ID: busPtr = new NEOPIXELBUS_8266_U0_NEO_4(len, pins[0]); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
      case BUSTYPE__8266_U1_NEO_4__ID: busPtr = new NEOPIXELBUS_8266_U1_NEO_4(len, pins[0]); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
      case BUSTYPE__8266_DM_NEO_4__ID: busPtr = new NEOPIXELBUS_8266_DM_NEO_4(len, pins[0]); break;
      #endif
      // case BUSTYPE__8266_BB_NEO_4__ID: busPtr = new NEOPIXELBUS_8266_BB_NEO_4(len, pins[0]); break;
      // case BUSTYPE__8266_U0_400_3__ID: busPtr = new NEOPIXELBUS_8266_U0_400_3(len, pins[0]); break;
      // case BUSTYPE__8266_U1_400_3__ID: busPtr = new NEOPIXELBUS_8266_U1_400_3(len, pins[0]); break;
      // case BUSTYPE__8266_DM_400_3__ID: busPtr = new NEOPIXELBUS_8266_DM_400_3(len, pins[0]); break;
      // case BUSTYPE__8266_BB_400_3__ID: busPtr = new NEOPIXELBUS_8266_BB_400_3(len, pins[0]); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32

      case BUSTYPE__32_RN_NEO_3__ID: busPtr = new NEOPIXELBUS_32_RN_NEO_3(len, pins[0], (NeoBusChannel)channel); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_3__ID: busPtr = new NEOPIXELBUS_32_I0_NEO_3(len, pins[0]); break;
      #endif      
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_3__ID: busPtr = new NEOPIXELBUS_32_I1_NEO_3(len, pins[0]); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_3__ID: busPtr = new NEOPIXELBUS_32_8PARALLEL_NEO_3(len, pins[0]); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_3__ID: busPtr = new NEOPIXELBUS_32_16PARALLEL_NEO_3(len, pins[0]); break;
      #endif

      case BUSTYPE__32_RN_NEO_4__ID: busPtr = new NEOPIXELBUS_32_RN_NEO_4(len, pins[0], (NeoBusChannel)channel); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_4__ID: busPtr = new NEOPIXELBUS_32_I0_NEO_4(len, pins[0]); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_4__ID: busPtr = new NEOPIXELBUS_32_I1_NEO_4(len, pins[0]); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_4__ID: busPtr = new NEOPIXELBUS_32_8PARALLEL_NEO_4(len, pins[0]); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_4__ID: busPtr = new NEOPIXELBUS_32_16PARALLEL_NEO_4(len, pins[0]); break;
      #endif

#ifdef ENABLE_DEVFEATURE__WS2805
      case BUSTYPE__32_RN_NEO_5__ID: 
      // busPtr = new NEOPIXELBUS_32_RN_NEO_5(len, pins[0], (NeoBusChannel)channel); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_5__ID: busPtr = new NEOPIXELBUS_32_I0_NEO_5(len, pins[0]); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_5__ID: busPtr = new NEOPIXELBUS_32_I1_NEO_5(len, pins[0]); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_5__ID: busPtr = new NEOPIXELBUS_32_8PARALLEL_NEO_5(len, pins[0]); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_5__ID: busPtr = new NEOPIXELBUS_32_16PARALLEL_NEO_5(len, pins[0]); break;
      #endif
#endif // ENABLE_DEVFEATURE__WS2805

      case BUSTYPE__32_RN_400_3__ID: busPtr = new NEOPIXELBUS_32_RN_400_3(len, pins[0], (NeoBusChannel)channel); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_400_3__ID: busPtr = new NEOPIXELBUS_32_I0_400_3(len, pins[0]); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_400_3__ID: busPtr = new NEOPIXELBUS_32_I1_400_3(len, pins[0]); break;
      #endif

      case BUSTYPE__32_RN_TM1_4__ID: busPtr = new NEOPIXELBUS_32_RN_TM1_4(len, pins[0], (NeoBusChannel)channel); break;
      case BUSTYPE__32_RN_TM2_3__ID: busPtr = new NEOPIXELBUS_32_RN_TM2_3(len, pins[0], (NeoBusChannel)channel); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_TM1_4__ID: busPtr = new NEOPIXELBUS_32_I0_TM1_4(len, pins[0]); break;
      case BUSTYPE__32_I0_TM2_3__ID: busPtr = new NEOPIXELBUS_32_I0_TM2_3(len, pins[0]); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_TM1_4__ID: busPtr = new NEOPIXELBUS_32_I1_TM1_4(len, pins[0]); break;
      case BUSTYPE__32_I1_TM2_3__ID: busPtr = new NEOPIXELBUS_32_I1_TM2_3(len, pins[0]); break;
      #endif

    #endif
      // // for 2-wire: pins[1] is clk, pins[0] is dat.  begin expects (len, clk, dat)
      // case BUSTYPE__HS_DOT_3__ID: busPtr = new NEOPIXELBUS_HS_DOT_3(len, pins[1], pins[0]); break;
      // case BUSTYPE__SS_DOT_3__ID: busPtr = new NEOPIXELBUS_SS_DOT_3(len, pins[1], pins[0]); break;
      // case BUSTYPE__HS_LPO_3__ID: busPtr = new NEOPIXELBUS_HS_LPO_3(len, pins[1], pins[0]); break;
      // case BUSTYPE__SS_LPO_3__ID: busPtr = new NEOPIXELBUS_SS_LPO_3(len, pins[1], pins[0]); break;
    }
      
    begin(busPtr, busType, pins);
    
    return busPtr;
  };
  static void show(void* busPtr, uint8_t busType, bool consistent = true) 
  {
    
    #ifdef ENABLE_DEBUGFEATURE__16PIN_PARALLEL_OUTPUT
    DEBUG_PRINTF("PolyBus::show busType %d\n\r", busType);
    #endif 
    
    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
    
      #ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
      case BUSTYPE__8266_U0_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_U0_NEO_3*>(busPtr))->Show(); break;
      #endif      
      #ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
      case BUSTYPE__8266_U1_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_U1_NEO_3*>(busPtr))->Show(); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
      case BUSTYPE__8266_DM_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_DM_NEO_3*>(busPtr))->Show(); break;
      #endif
      // case BUSTYPE__8266_BB_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_BB_NEO_3*>(busPtr))->Show(); break;
      
      #ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
      case BUSTYPE__8266_U0_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_U0_NEO_4*>(busPtr))->Show(); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
      case BUSTYPE__8266_U1_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_U1_NEO_4*>(busPtr))->Show(); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
      case BUSTYPE__8266_DM_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_DM_NEO_4*>(busPtr))->Show(); break;
      #endif
      // case BUSTYPE__8266_BB_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_BB_NEO_4*>(busPtr))->Show(); break;
      // case BUSTYPE__8266_U0_400_3__ID: (static_cast<NEOPIXELBUS_8266_U0_400_3*>(busPtr))->Show(); break;
      // case BUSTYPE__8266_U1_400_3__ID: (static_cast<NEOPIXELBUS_8266_U1_400_3*>(busPtr))->Show(); break;
      // case BUSTYPE__8266_DM_400_3__ID: (static_cast<NEOPIXELBUS_8266_DM_400_3*>(busPtr))->Show(); break;
      // case BUSTYPE__8266_BB_400_3__ID: (static_cast<NEOPIXELBUS_8266_BB_400_3*>(busPtr))->Show(); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32

      case BUSTYPE__32_RN_NEO_3__ID: (static_cast<NEOPIXELBUS_32_RN_NEO_3*>(busPtr))->Show();       
      break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_3__ID: (static_cast<NEOPIXELBUS_32_I0_NEO_3*>(busPtr))->Show(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_3__ID: (static_cast<NEOPIXELBUS_32_I1_NEO_3*>(busPtr))->Show(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_3__ID: (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_3*>(busPtr))->Show(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_3__ID: (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_3*>(busPtr))->Show(); break;
      #endif

      case BUSTYPE__32_RN_NEO_4__ID: (static_cast<NEOPIXELBUS_32_RN_NEO_4*>(busPtr))->Show(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_4__ID: (static_cast<NEOPIXELBUS_32_I0_NEO_4*>(busPtr))->Show(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_4__ID: (static_cast<NEOPIXELBUS_32_I1_NEO_4*>(busPtr))->Show(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_4__ID: (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_4*>(busPtr))->Show(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_4__ID: (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_4*>(busPtr))->Show(); break;
      #endif

#ifdef ENABLE_DEVFEATURE__WS2805
      case BUSTYPE__32_RN_NEO_5__ID: 
      // (static_cast<NEOPIXELBUS_32_RN_NEO_5*>(busPtr))->Show(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_5__ID: (static_cast<NEOPIXELBUS_32_I0_NEO_5*>(busPtr))->Show(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_5__ID: (static_cast<NEOPIXELBUS_32_I1_NEO_5*>(busPtr))->Show(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_5__ID: (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_5*>(busPtr))->Show(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_5__ID: (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_5*>(busPtr))->Show(); break;
      #endif
#endif // ENABLE_DEVFEATURE__WS2805

      case BUSTYPE__32_RN_400_3__ID: (static_cast<NEOPIXELBUS_32_RN_400_3*>(busPtr))->Show(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_400_3__ID: (static_cast<NEOPIXELBUS_32_I0_400_3*>(busPtr))->Show(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_400_3__ID: (static_cast<NEOPIXELBUS_32_I1_400_3*>(busPtr))->Show(); break;
      #endif

      case BUSTYPE__32_RN_TM1_4__ID: (static_cast<NEOPIXELBUS_32_RN_TM1_4*>(busPtr))->Show(); break;
      case BUSTYPE__32_RN_TM2_3__ID: (static_cast<NEOPIXELBUS_32_RN_TM2_3*>(busPtr))->Show(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_TM1_4__ID: (static_cast<NEOPIXELBUS_32_I0_TM1_4*>(busPtr))->Show(); break;
      case BUSTYPE__32_I0_TM2_3__ID: (static_cast<NEOPIXELBUS_32_I0_TM2_3*>(busPtr))->Show(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_TM1_4__ID: (static_cast<NEOPIXELBUS_32_I1_TM1_4*>(busPtr))->Show(); break;
      case BUSTYPE__32_I1_TM2_3__ID: (static_cast<NEOPIXELBUS_32_I1_TM2_3*>(busPtr))->Show(); break;
      #endif

    #endif
      // case BUSTYPE__HS_DOT_3__ID: (static_cast<NEOPIXELBUS_HS_DOT_3*>(busPtr))->Show(); break;
      // case BUSTYPE__SS_DOT_3__ID: (static_cast<NEOPIXELBUS_SS_DOT_3*>(busPtr))->Show(); break;
      // case BUSTYPE__HS_LPO_3__ID: (static_cast<NEOPIXELBUS_HS_LPO_3*>(busPtr))->Show(); break;
      // case BUSTYPE__SS_LPO_3__ID: (static_cast<NEOPIXELBUS_SS_LPO_3*>(busPtr))->Show(); break;
    }


  };
  static bool canShow(void* busPtr, uint8_t busType) 
  {
    
    //DEBUG_PRINTF("PolyBus::canShow busType %d\n\r", busType);

    switch (busType) {
      case BUSTYPE__NONE__ID: return true;
    #ifdef ESP8266
    
      #ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
      case BUSTYPE__8266_U0_NEO_3__ID: return (static_cast<NEOPIXELBUS_8266_U0_NEO_3*>(busPtr))->CanShow(); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
      case BUSTYPE__8266_U1_NEO_3__ID: return (static_cast<NEOPIXELBUS_8266_U1_NEO_3*>(busPtr))->CanShow(); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
      case BUSTYPE__8266_DM_NEO_3__ID: return (static_cast<NEOPIXELBUS_8266_DM_NEO_3*>(busPtr))->CanShow(); break;
      #endif
      // case BUSTYPE__8266_BB_NEO_3__ID: return (static_cast<NEOPIXELBUS_8266_BB_NEO_3*>(busPtr))->CanShow(); break;
      
      #ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
      case BUSTYPE__8266_U0_NEO_4__ID: return (static_cast<NEOPIXELBUS_8266_U0_NEO_4*>(busPtr))->CanShow(); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
      case BUSTYPE__8266_U1_NEO_4__ID: return (static_cast<NEOPIXELBUS_8266_U1_NEO_4*>(busPtr))->CanShow(); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
      case BUSTYPE__8266_DM_NEO_4__ID: return (static_cast<NEOPIXELBUS_8266_DM_NEO_4*>(busPtr))->CanShow(); break;
      #endif
      // case BUSTYPE__8266_BB_NEO_4__ID: return (static_cast<NEOPIXELBUS_8266_BB_NEO_4*>(busPtr))->CanShow(); break;
      // case BUSTYPE__8266_U0_400_3__ID: return (static_cast<NEOPIXELBUS_8266_U0_400_3*>(busPtr))->CanShow(); break;
      // case BUSTYPE__8266_U1_400_3__ID: return (static_cast<NEOPIXELBUS_8266_U1_400_3*>(busPtr))->CanShow(); break;
      // case BUSTYPE__8266_DM_400_3__ID: return (static_cast<NEOPIXELBUS_8266_DM_400_3*>(busPtr))->CanShow(); break;
      // case BUSTYPE__8266_BB_400_3__ID: return (static_cast<NEOPIXELBUS_8266_BB_400_3*>(busPtr))->CanShow(); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32

      case BUSTYPE__32_RN_NEO_3__ID: return (static_cast<NEOPIXELBUS_32_RN_NEO_3*>(busPtr))->CanShow(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_3__ID: return (static_cast<NEOPIXELBUS_32_I0_NEO_3*>(busPtr))->CanShow(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_3__ID: return (static_cast<NEOPIXELBUS_32_I1_NEO_3*>(busPtr))->CanShow(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_3__ID: return (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_3*>(busPtr))->CanShow(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_3__ID: return (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_3*>(busPtr))->CanShow(); break;
      #endif

      case BUSTYPE__32_RN_NEO_4__ID: return (static_cast<NEOPIXELBUS_32_RN_NEO_4*>(busPtr))->CanShow(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_4__ID: return (static_cast<NEOPIXELBUS_32_I0_NEO_4*>(busPtr))->CanShow(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_4__ID: return (static_cast<NEOPIXELBUS_32_I1_NEO_4*>(busPtr))->CanShow(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_4__ID: return (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_4*>(busPtr))->CanShow(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_4__ID: return (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_4*>(busPtr))->CanShow(); break;
      #endif

#ifdef ENABLE_DEVFEATURE__WS2805
      case BUSTYPE__32_RN_NEO_5__ID: 
      // return (static_cast<NEOPIXELBUS_32_RN_NEO_5*>(busPtr))->CanShow(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_5__ID: return (static_cast<NEOPIXELBUS_32_I0_NEO_5*>(busPtr))->CanShow(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_5__ID: return (static_cast<NEOPIXELBUS_32_I1_NEO_5*>(busPtr))->CanShow(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_5__ID: return (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_5*>(busPtr))->CanShow(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_5__ID: return (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_5*>(busPtr))->CanShow(); break;
      #endif
#endif // ENABLE_DEVFEATURE__WS2805

      case BUSTYPE__32_RN_400_3__ID: return (static_cast<NEOPIXELBUS_32_RN_400_3*>(busPtr))->CanShow(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_400_3__ID: return (static_cast<NEOPIXELBUS_32_I0_400_3*>(busPtr))->CanShow(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_400_3__ID: return (static_cast<NEOPIXELBUS_32_I1_400_3*>(busPtr))->CanShow(); break;
      #endif

      case BUSTYPE__32_RN_TM1_4__ID: return (static_cast<NEOPIXELBUS_32_RN_TM1_4*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_RN_TM2_3__ID: return (static_cast<NEOPIXELBUS_32_RN_TM2_3*>(busPtr))->CanShow(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_TM1_4__ID: return (static_cast<NEOPIXELBUS_32_I0_TM1_4*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_I0_TM2_3__ID: return (static_cast<NEOPIXELBUS_32_I0_TM2_3*>(busPtr))->CanShow(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_TM1_4__ID: return (static_cast<NEOPIXELBUS_32_I1_TM1_4*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_I1_TM2_3__ID: return (static_cast<NEOPIXELBUS_32_I1_TM2_3*>(busPtr))->CanShow(); break;
      #endif

    #endif

      // case BUSTYPE__HS_DOT_3__ID: return (static_cast<NEOPIXELBUS_HS_DOT_3*>(busPtr))->CanShow(); break;
      // case BUSTYPE__SS_DOT_3__ID: return (static_cast<NEOPIXELBUS_SS_DOT_3*>(busPtr))->CanShow(); break;
      // case BUSTYPE__HS_LPO_3__ID: return (static_cast<NEOPIXELBUS_HS_LPO_3*>(busPtr))->CanShow(); break;
      // case BUSTYPE__SS_LPO_3__ID: return (static_cast<NEOPIXELBUS_SS_LPO_3*>(busPtr))->CanShow(); break;

    }
    return true;
  };
  
  static void setPixelColor(void* busPtr, uint8_t busType, uint16_t pix, RgbcctColor colour_internal, COLOUR_ORDER_T colour_order = {0}) 
  {
    
    RgbcctColor colour_hardware = colour_internal; // Start with original

    if(colour_order.red        != COLOUR_ORDER_DISABLED){ colour_hardware.raw[colour_order.red]         = colour_internal.R;  }
    if(colour_order.green      != COLOUR_ORDER_DISABLED){ colour_hardware.raw[colour_order.green]       = colour_internal.G;  }
    if(colour_order.blue       != COLOUR_ORDER_DISABLED){ colour_hardware.raw[colour_order.blue]        = colour_internal.B;  }
    if(colour_order.white_warm != COLOUR_ORDER_DISABLED){ colour_hardware.raw[colour_order.white_warm]  = colour_internal.WW; }
    if(colour_order.white_cold != COLOUR_ORDER_DISABLED){ colour_hardware.raw[colour_order.white_cold]  = colour_internal.CW; }
    #ifdef ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS
    if(pix==0){ // Just first pixel
      Serial.printf("set colour R=%d, G=%d, B=%d, CW=%d, WW=%d %d/%d/%d/%d/%d\n\r",
        colour_order.red,
        colour_order.green,
        colour_order.blue,
        colour_order.white_cold,
        colour_order.white_warm, 
        colour_internal.R,
        colour_internal.G,
        colour_internal.B,
        colour_internal.CW,
        colour_internal.WW
      );
    }
    // colour_hardware = RgbcctColor(5,0,0,5,0);
    #endif // ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS
    #ifdef ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS
    if(pix<1){ // Just first pixel
      Serial.printf("setPixelColor[%d] R=%d, G=%d, B=%d, WW=%d, CW=%d\n\r", pix, colour_internal.R, colour_internal.G, colour_internal.B, colour_internal.WW, colour_internal.CW);
    }
    #endif // ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS
    #ifdef ENABLE_DEVFEATURE_DEBUG_GARGAE_PIXEL_OUTPUT
    if(pix>0){ // Just first pixel
      // Serial.printf("set %d colour_order R=%d, G=%d, B=%d, CW=%d, WW=%d\n\r",
      //   pix,
      //   colour_order.red,
      //   colour_order.green,
      //   colour_order.blue,
      //   colour_order.white_cold,
      //   colour_order.white_warm
      // );
      // Serial.printf("set %03d\tR=%d\tG=%d\tB=%d\tCW=%d\tWW=%d\n\r",
      //   pix,
      //   colour_internal.R,
      //   colour_internal.G,
      //   colour_internal.B,
      //   colour_internal.W1,
      //   colour_internal.W2
      // );
    }
    #endif // ENABLE_DEVFEATURE_DEBUG_GARGAE_PIXEL_OUTPUT
// colour_hardware = RgbColor(255-(pix*15),0,pix*15);

// if(pix==0)
// Serial.println(busType);

    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
      #ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
      case BUSTYPE__8266_U0_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_U0_NEO_3*>(busPtr))->SetPixelColor(pix, RgbColor(colour_hardware.R,colour_hardware.G,colour_hardware.B)); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
      case BUSTYPE__8266_U1_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_U1_NEO_3*>(busPtr))->SetPixelColor(pix, RgbColor(colour_hardware.R,colour_hardware.G,colour_hardware.B)); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
      case BUSTYPE__8266_DM_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_DM_NEO_3*>(busPtr))->SetPixelColor(pix, RgbColor(colour_hardware.R,colour_hardware.G,colour_hardware.B)); break;
      #endif
      // case BUSTYPE__8266_BB_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_BB_NEO_3*>(busPtr))->SetPixelColor(pix, RgbColor(colour_hardware.R,colour_hardware.G,colour_hardware.B)); break;
      #ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
      case BUSTYPE__8266_U0_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_U0_NEO_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
      case BUSTYPE__8266_U1_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_U1_NEO_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
      case BUSTYPE__8266_DM_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_DM_NEO_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      // case BUSTYPE__8266_BB_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_BB_NEO_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      // case BUSTYPE__8266_U0_400_3__ID: (static_cast<NEOPIXELBUS_8266_U0_400_3*>(busPtr))->SetPixelColor(pix, RgbColor(colour_hardware.R,colour_hardware.G,colour_hardware.B)); break;
      // case BUSTYPE__8266_U1_400_3__ID: (static_cast<NEOPIXELBUS_8266_U1_400_3*>(busPtr))->SetPixelColor(pix, RgbColor(colour_hardware.R,colour_hardware.G,colour_hardware.B)); break;
      // case BUSTYPE__8266_DM_400_3__ID: (static_cast<NEOPIXELBUS_8266_DM_400_3*>(busPtr))->SetPixelColor(pix, RgbColor(colour_hardware.R,colour_hardware.G,colour_hardware.B)); break;
      // case BUSTYPE__8266_BB_400_3__ID: (static_cast<NEOPIXELBUS_8266_BB_400_3*>(busPtr))->SetPixelColor(pix, RgbColor(colour_hardware.R,colour_hardware.G,colour_hardware.B)); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32

      case BUSTYPE__32_RN_NEO_3__ID: (static_cast<NEOPIXELBUS_32_RN_NEO_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_3__ID: (static_cast<NEOPIXELBUS_32_I0_NEO_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_3__ID: (static_cast<NEOPIXELBUS_32_I1_NEO_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_3__ID: (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_3__ID: (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif

      case BUSTYPE__32_RN_NEO_4__ID: (static_cast<NEOPIXELBUS_32_RN_NEO_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_4__ID: (static_cast<NEOPIXELBUS_32_I0_NEO_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_4__ID: (static_cast<NEOPIXELBUS_32_I1_NEO_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_4__ID: (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_4__ID: (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif

#ifdef ENABLE_DEVFEATURE__WS2805
      case BUSTYPE__32_RN_NEO_5__ID: 
      // (static_cast<NEOPIXELBUS_32_RN_NEO_5*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_5__ID: (static_cast<NEOPIXELBUS_32_I0_NEO_5*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_5__ID: (static_cast<NEOPIXELBUS_32_I1_NEO_5*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_5__ID: (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_5*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_5__ID: (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_5*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
#endif // ENABLE_DEVFEATURE__WS2805

      case BUSTYPE__32_RN_400_3__ID: (static_cast<NEOPIXELBUS_32_RN_400_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_400_3__ID: (static_cast<NEOPIXELBUS_32_I0_400_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_400_3__ID: (static_cast<NEOPIXELBUS_32_I1_400_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif

      case BUSTYPE__32_RN_TM1_4__ID: (static_cast<NEOPIXELBUS_32_RN_TM1_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      case BUSTYPE__32_RN_TM2_3__ID: (static_cast<NEOPIXELBUS_32_RN_TM2_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_TM1_4__ID: (static_cast<NEOPIXELBUS_32_I0_TM1_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      case BUSTYPE__32_I0_TM2_3__ID: (static_cast<NEOPIXELBUS_32_I0_TM2_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_TM1_4__ID: (static_cast<NEOPIXELBUS_32_I1_TM1_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      case BUSTYPE__32_I1_TM2_3__ID: (static_cast<NEOPIXELBUS_32_I1_TM2_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif

    #endif

      // case BUSTYPE__HS_DOT_3__ID: (static_cast<NEOPIXELBUS_HS_DOT_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      // case BUSTYPE__SS_DOT_3__ID: (static_cast<NEOPIXELBUS_SS_DOT_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      // case BUSTYPE__HS_LPO_3__ID: (static_cast<NEOPIXELBUS_HS_LPO_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      // case BUSTYPE__SS_LPO_3__ID: (static_cast<NEOPIXELBUS_SS_LPO_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
    }
  };

  
  static RgbcctColor getPixelColor(void* busPtr, uint8_t busType, uint16_t pix, COLOUR_ORDER_T colour_order) 
  {

    // if(pix < 5) DEBUG_PRINTF("PolyBus::getPixelColor busType %d, pix %d, co -\n\r", busType, pix);//, co);

    RgbcctColor col; 
    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
    
      #ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
      case BUSTYPE__8266_U0_NEO_3__ID: col = (static_cast<NEOPIXELBUS_8266_U0_NEO_3*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
      case BUSTYPE__8266_U1_NEO_3__ID: col = (static_cast<NEOPIXELBUS_8266_U1_NEO_3*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
      case BUSTYPE__8266_DM_NEO_3__ID: col = (static_cast<NEOPIXELBUS_8266_DM_NEO_3*>(busPtr))->GetPixelColor(pix); break;
      #endif
      // case BUSTYPE__8266_BB_NEO_3__ID: col = (static_cast<NEOPIXELBUS_8266_BB_NEO_3*>(busPtr))->GetPixelColor(pix); break;
      
      #ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
      case BUSTYPE__8266_U0_NEO_4__ID: col = (static_cast<NEOPIXELBUS_8266_U0_NEO_4*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
      case BUSTYPE__8266_U1_NEO_4__ID: col = (static_cast<NEOPIXELBUS_8266_U1_NEO_4*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
      case BUSTYPE__8266_DM_NEO_4__ID: col = (static_cast<NEOPIXELBUS_8266_DM_NEO_4*>(busPtr))->GetPixelColor(pix); break;
      #endif
      // case BUSTYPE__8266_BB_NEO_4__ID: col = (static_cast<NEOPIXELBUS_8266_BB_NEO_4*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__8266_U0_400_3__ID: col = (static_cast<NEOPIXELBUS_8266_U0_400_3*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__8266_U1_400_3__ID: col = (static_cast<NEOPIXELBUS_8266_U1_400_3*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__8266_DM_400_3__ID: col = (static_cast<NEOPIXELBUS_8266_DM_400_3*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__8266_BB_400_3__ID: col = (static_cast<NEOPIXELBUS_8266_BB_400_3*>(busPtr))->GetPixelColor(pix); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32

      case BUSTYPE__32_RN_NEO_3__ID: 
      
      // auto p = (static_cast<NEOPIXELBUS_32_RN_NEO_3*>(busPtr))->GetPixelColor(pix);
      
      col = (static_cast<NEOPIXELBUS_32_RN_NEO_3*>(busPtr))->GetPixelColor(pix); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_3__ID: col = (static_cast<NEOPIXELBUS_32_I0_NEO_3*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_3__ID: col = (static_cast<NEOPIXELBUS_32_I1_NEO_3*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_3__ID: col = (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_3*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_3__ID: col = (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_3*>(busPtr))->GetPixelColor(pix); break;
      #endif

      case BUSTYPE__32_RN_NEO_4__ID: col = (static_cast<NEOPIXELBUS_32_RN_NEO_4*>(busPtr))->GetPixelColor(pix); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_4__ID: col = (static_cast<NEOPIXELBUS_32_I0_NEO_4*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_4__ID: col = (static_cast<NEOPIXELBUS_32_I1_NEO_4*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_4__ID: col = (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_4*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_4__ID: col = (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_4*>(busPtr))->GetPixelColor(pix); break;
      #endif

#ifdef ENABLE_DEVFEATURE__WS2805
      case BUSTYPE__32_RN_NEO_5__ID: 
      // col = (static_cast<NEOPIXELBUS_32_RN_NEO_5*>(busPtr))->GetPixelColor(pix); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_5__ID: col = (static_cast<NEOPIXELBUS_32_I0_NEO_5*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_5__ID: col = (static_cast<NEOPIXELBUS_32_I1_NEO_5*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_5__ID: col = (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_5*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_5__ID: col = (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_5*>(busPtr))->GetPixelColor(pix); break;
      #endif
#endif // ENABLE_DEVFEATURE__WS2805

      case BUSTYPE__32_RN_400_3__ID: col = (static_cast<NEOPIXELBUS_32_RN_400_3*>(busPtr))->GetPixelColor(pix); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_400_3__ID: col = (static_cast<NEOPIXELBUS_32_I0_400_3*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_400_3__ID: col = (static_cast<NEOPIXELBUS_32_I1_400_3*>(busPtr))->GetPixelColor(pix); break;
      #endif

      case BUSTYPE__32_RN_TM1_4__ID: col = (static_cast<NEOPIXELBUS_32_RN_TM1_4*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_RN_TM2_3__ID: col = (static_cast<NEOPIXELBUS_32_RN_TM2_3*>(busPtr))->GetPixelColor(pix); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_TM1_4__ID: col = (static_cast<NEOPIXELBUS_32_I0_TM1_4*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I0_TM2_3__ID: col = (static_cast<NEOPIXELBUS_32_I0_TM2_3*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_TM1_4__ID: col = (static_cast<NEOPIXELBUS_32_I1_TM1_4*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I1_TM2_3__ID: col = (static_cast<NEOPIXELBUS_32_I1_TM2_3*>(busPtr))->GetPixelColor(pix); break;
      #endif

    #endif

      // case BUSTYPE__HS_DOT_3__ID: col = (static_cast<NEOPIXELBUS_HS_DOT_3*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__SS_DOT_3__ID: col = (static_cast<NEOPIXELBUS_SS_DOT_3*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__HS_LPO_3__ID: col = (static_cast<NEOPIXELBUS_HS_LPO_3*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__SS_LPO_3__ID: col = (static_cast<NEOPIXELBUS_SS_LPO_3*>(busPtr))->GetPixelColor(pix); break;
    }
    
    RgbcctColor color_internal = col; 
  
    if(colour_order.red        != COLOUR_ORDER_DISABLED){ color_internal.raw[colour_order.red] = col.R; }
    if(colour_order.green      != COLOUR_ORDER_DISABLED){ color_internal.raw[colour_order.green] = col.G; }
    if(colour_order.blue       != COLOUR_ORDER_DISABLED){ color_internal.raw[colour_order.blue] = col.B; }
    if(colour_order.white_warm != COLOUR_ORDER_DISABLED){ color_internal.raw[colour_order.white_warm] = col.WW; }
    if(colour_order.white_cold != COLOUR_ORDER_DISABLED){ color_internal.raw[colour_order.white_cold] = col.CW; }
    #ifdef ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS
    if(pix<5>){ // Just first pixel
      Serial.printf("get colour_order R=%d, G=%d, B=%d, CW=%d, WW=%d\n\r",
        colour_order.red,
        colour_order.green,
        colour_order.blue,
        colour_order.white_cold,
        colour_order.white_warm
      );
    }
    #endif // ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS
    #ifdef ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS
    if(pix<1){ // Just first pixel
      Serial.printf("getPixelColor[%d] R=%d, G=%d, B=%d, WW=%d, CW=%d\n\r", pix, color_internal.R, color_internal.G, color_internal.B, color_internal.WW, color_internal.CW);
    }
    #endif // ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS

    return color_internal;

  }

  static void cleanup(void* busPtr, uint8_t busType) 
  {

    // DEBUG_PRINTF("PolyBus::cleanup busType %d\n\r", busType);
    
    if (busPtr == nullptr) return;
    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
    
      #ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
      case BUSTYPE__8266_U0_NEO_3__ID: delete (static_cast<NEOPIXELBUS_8266_U0_NEO_3*>(busPtr)); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
      case BUSTYPE__8266_U1_NEO_3__ID: delete (static_cast<NEOPIXELBUS_8266_U1_NEO_3*>(busPtr)); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
      case BUSTYPE__8266_DM_NEO_3__ID: delete (static_cast<NEOPIXELBUS_8266_DM_NEO_3*>(busPtr)); break;
      #endif
      // case BUSTYPE__8266_BB_NEO_3__ID: delete (static_cast<NEOPIXELBUS_8266_BB_NEO_3*>(busPtr)); break;
      #ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
      case BUSTYPE__8266_U0_NEO_4__ID: delete (static_cast<NEOPIXELBUS_8266_U0_NEO_4*>(busPtr)); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
      case BUSTYPE__8266_U1_NEO_4__ID: delete (static_cast<NEOPIXELBUS_8266_U1_NEO_4*>(busPtr)); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
      case BUSTYPE__8266_DM_NEO_4__ID: delete (static_cast<NEOPIXELBUS_8266_DM_NEO_4*>(busPtr)); break;
      #endif
      // case BUSTYPE__8266_BB_NEO_4__ID: delete (static_cast<NEOPIXELBUS_8266_BB_NEO_4*>(busPtr)); break;
      // case BUSTYPE__8266_U0_400_3__ID: delete (static_cast<NEOPIXELBUS_8266_U0_400_3*>(busPtr)); break;
      // case BUSTYPE__8266_U1_400_3__ID: delete (static_cast<NEOPIXELBUS_8266_U1_400_3*>(busPtr)); break;
      // case BUSTYPE__8266_DM_400_3__ID: delete (static_cast<NEOPIXELBUS_8266_DM_400_3*>(busPtr)); break;
      // case BUSTYPE__8266_BB_400_3__ID: delete (static_cast<NEOPIXELBUS_8266_BB_400_3*>(busPtr)); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32

      case BUSTYPE__32_RN_NEO_3__ID: delete (static_cast<NEOPIXELBUS_32_RN_NEO_3*>(busPtr)); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_3__ID: delete (static_cast<NEOPIXELBUS_32_I0_NEO_3*>(busPtr)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_3__ID: delete (static_cast<NEOPIXELBUS_32_I1_NEO_3*>(busPtr)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_3__ID: delete (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_3*>(busPtr)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_3__ID: delete (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_3*>(busPtr)); break;
      #endif

      case BUSTYPE__32_RN_NEO_4__ID: delete (static_cast<NEOPIXELBUS_32_RN_NEO_4*>(busPtr)); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_4__ID: delete (static_cast<NEOPIXELBUS_32_I0_NEO_4*>(busPtr)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_4__ID: delete (static_cast<NEOPIXELBUS_32_I1_NEO_4*>(busPtr)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_4__ID: delete (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_4*>(busPtr)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_4__ID: delete (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_4*>(busPtr)); break;
      #endif

#ifdef ENABLE_DEVFEATURE__WS2805
      case BUSTYPE__32_RN_NEO_5__ID: 
      // delete (static_cast<NEOPIXELBUS_32_RN_NEO_5*>(busPtr)); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_5__ID: delete (static_cast<NEOPIXELBUS_32_I0_NEO_5*>(busPtr)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_5__ID: delete (static_cast<NEOPIXELBUS_32_I1_NEO_5*>(busPtr)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_5__ID: delete (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_5*>(busPtr)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_5__ID: delete (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_5*>(busPtr)); break;
      #endif
#endif // ENABLE_DEVFEATURE__WS2805

      case BUSTYPE__32_RN_400_3__ID: delete (static_cast<NEOPIXELBUS_32_RN_400_3*>(busPtr)); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_400_3__ID: delete (static_cast<NEOPIXELBUS_32_I0_400_3*>(busPtr)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_400_3__ID: delete (static_cast<NEOPIXELBUS_32_I1_400_3*>(busPtr)); break;
      #endif

      case BUSTYPE__32_RN_TM1_4__ID: delete (static_cast<NEOPIXELBUS_32_RN_TM1_4*>(busPtr)); break;
      case BUSTYPE__32_RN_TM2_3__ID: delete (static_cast<NEOPIXELBUS_32_RN_TM2_3*>(busPtr)); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_TM1_4__ID: delete (static_cast<NEOPIXELBUS_32_I0_TM1_4*>(busPtr)); break;
      case BUSTYPE__32_I0_TM2_3__ID: delete (static_cast<NEOPIXELBUS_32_I0_TM2_3*>(busPtr)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_TM1_4__ID: delete (static_cast<NEOPIXELBUS_32_I1_TM1_4*>(busPtr)); break;
      case BUSTYPE__32_I1_TM2_3__ID: delete (static_cast<NEOPIXELBUS_32_I1_TM2_3*>(busPtr)); break;
      #endif

    #endif

      // case BUSTYPE__HS_DOT_3__ID: delete (static_cast<NEOPIXELBUS_HS_DOT_3*>(busPtr)); break;
      // case BUSTYPE__SS_DOT_3__ID: delete (static_cast<NEOPIXELBUS_SS_DOT_3*>(busPtr)); break;
      // case BUSTYPE__HS_LPO_3__ID: delete (static_cast<NEOPIXELBUS_HS_LPO_3*>(busPtr)); break;
      // case BUSTYPE__SS_LPO_3__ID: delete (static_cast<NEOPIXELBUS_SS_LPO_3*>(busPtr)); break;
    }
  }

  //gives back the internal type index (I_XX_XXX_X above) for the input 
  static uint8_t getI(uint8_t busType, uint8_t* pins, uint8_t num = 0) 
  {
    
    // DEBUG_PRINTF("PolyBus::getI busType %d\n\r", busType);

    if (!IS_BUSTYPE_DIGITAL(busType)) return BUSTYPE__NONE__ID;
    if (IS_BUSTYPE_2PIN(busType)) { //SPI LED chips
      bool isHSPI = false;
      #ifdef ESP8266
      if (pins[0] == P_8266_HS_MOSI && pins[1] == P_8266_HS_CLK) isHSPI = true;
      #else
      // temporary hack to limit use of hardware SPI to a single SPI peripheral (HSPI): only allow ESP32 hardware serial on segment 0
      // SPI global variable is normally linked to VSPI on ESP32 (or FSPI C3, S3)
      if (!num) isHSPI = true;
      #endif
      uint8_t t = BUSTYPE__NONE__ID;
      switch (busType) {
        case BUSTYPE_APA102:  t = BUSTYPE__SS_DOT_3__ID; break;
        case BUSTYPE_LPD8806: t = BUSTYPE__SS_LPD_3__ID; break;
        case BUSTYPE_LPD6803: t = BUSTYPE__SS_LPO_3__ID; break;
        case BUSTYPE_WS2801:  t = BUSTYPE__SS_WS1_3__ID; break;
        case BUSTYPE_P9813:   t = BUSTYPE__SS_P98_3__ID; break;
        default: t = BUSTYPE__NONE__ID;
      }
      if (t > BUSTYPE__NONE__ID && isHSPI) t--; //hardware SPI has one smaller ID than software
      return t;
    } else {
      #ifdef ESP8266
      uint8_t offset_method_inside_group = pins[0] - 1; // for driver: 0 = uart0, 1 = uart1, 2 = dma, 3 = bitbang
      if (offset_method_inside_group > 3) offset_method_inside_group = 3;
      switch (busType) {
        case BUSTYPE_WS2812_RGB:
        case BUSTYPE_WS2812_WWA:
          return BUSTYPE__8266_U0_NEO_3__ID + offset_method_inside_group;
        case BUSTYPE_SK6812_RGBW:
          return BUSTYPE__8266_U0_NEO_4__ID + offset_method_inside_group;
        // case BUSTYPE_WS2811_400KHZ:
        //   return BUSTYPE__8266_U0_400_3__ID + offset_method_inside_group;
        // case BUSTYPE_TM1814:
        //   return BUSTYPE__8266_U0_TM1_4__ID + offset_method_inside_group;
        // case BUSTYPE_TM1829:
        //   return BUSTYPE__8266_U0_TM2_3__ID + offset_method_inside_group;
      }
      #else //ESP32
      uint8_t offset_method_inside_group = 0;
      #if defined(CONFIG_IDF_TARGET_ESP32S2)
      // ESP32-S2 only has 4 RMT channels
      if (num > 4) return I_NONE;
      if (num > 3) offset_method_inside_group = 1;  // only one I2S
      #elif defined(CONFIG_IDF_TARGET_ESP32C3)
      // On ESP32-C3 only the first 2 RMT channels are usable for transmitting
      if (num > 1) return I_NONE;
      //if (num > 1) offset_method_inside_group = 1; // I2S not supported yet (only 1 I2S)
      #elif defined(CONFIG_IDF_TARGET_ESP32S3)
      // On ESP32-S3 only the first 4 RMT channels are usable for transmitting
      if (num > 3) return I_NONE;
      //if (num > 3) offset_method_inside_group = num -4; // I2S not supported yet
      #else

        #ifdef ENABLE_NEOPIXELBUS_BUSMETHODS__I2S_AUTO_CHANNEL_SWITCHING


        #else
        
          #ifdef ENABLE_NEOPIXELBUS_BUSMETHODS__I2S_SINGLE_CHANNELS_THEN_8_RMT_CHANNELS // Primary method for single pin output
          // I2S0 and I2S1 are 0 and 1
          // RMT0 to 7 as 2 to 9
          if(num < 2)
          {
            offset_method_inside_group = num + 1; // to skip that RMT was entered first in enum
          }
          else
          if(num < 9)
          {
            offset_method_inside_group = num - 7;
          }
          else
          {
            return BUSTYPE__NONE__ID;
          }
          #endif // ENABLE_NEOPIXELBUS_BUSMETHODS__I2S_SINGLE_CHANNELS_THEN_8_RMT_CHANNELS
          #ifdef ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
          if(num < 8)
          {
            // Assume a max of 8 pins supported for now, using the I2S1 (which supports 8) and maybe later I2S0 (which supports 16)
            offset_method_inside_group = 4; // handled inside library automatically for I2S1 types
          }
          else
          {
            return BUSTYPE__NONE__ID;
          }
          #endif // ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
          #ifdef ENABLE_NEOPIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE
          if(num < 16)
          {
            // Assume a max of 8 pins supported for now, using the I2S1 (which supports 8) and maybe later I2S0 (which supports 16)
            offset_method_inside_group = 5; // handled inside library automatically for I2S1 types
          }
          else
          {
            return BUSTYPE__NONE__ID;
          }
          #endif // ENABLE_NEOPIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE
          #ifdef ENABLE_NEOPIXELBUS_BUSMETHODS__RMT_8_CHANNELS_THEN_I2S_DUAL_CHANNELS
          #error "I2S methods cause flickering on ESP32, use RMT methods instead -- needs debugging"
          // standard ESP32 has 8 RMT and 2 I2S channels
          if (num > 9) return BUSTYPE__NONE__ID;
          if (num > 7) offset_method_inside_group = num -7;
          #warning "RMT methods cause flickering on ESP32, use I2S methods instead -- needs debugging"
          #endif // ENABLE_NEOPIXELBUS_BUSMETHODS__RMT_8_CHANNELS_THEN_I2S_DUAL_CHANNELS

        #endif // ENABLE_NEOPIXELBUS_BUSMETHODS__I2S_AUTO_CHANNEL_SWITCHING

        #if !defined(ENABLE_NEOPIXELBUS_BUSMETHODS__I2S_SINGLE_CHANNELS_THEN_8_RMT_CHANNELS) &&  \
            !defined(ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE) &&                 \
            !defined(ENABLE_NEOPIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE) &&                \
            !defined(ENABLE_NEOPIXELBUS_BUSMETHODS__RMT_8_CHANNELS_THEN_I2S_DUAL_CHANNELS)
        #error "No I2S or RMT channels selected"
        #endif

      #endif
      switch (busType) {
        case BUSTYPE_WS2812_RGB:
        case BUSTYPE_WS2812_WWA:
          return BUSTYPE__32_RN_NEO_3__ID + offset_method_inside_group;
        case BUSTYPE_SK6812_RGBW:
          return BUSTYPE__32_RN_NEO_4__ID + offset_method_inside_group;
        case BUSTYPE_WS2805_RGBWW:
          return BUSTYPE__32_RN_NEO_5__ID + offset_method_inside_group;
        case BUSTYPE_WS2811_400KHZ:
          return BUSTYPE__32_RN_400_3__ID + offset_method_inside_group;
        case BUSTYPE_TM1814:
          return BUSTYPE__32_RN_TM1_4__ID + offset_method_inside_group;
        case BUSTYPE_TM1829:
          return BUSTYPE__32_RN_TM2_3__ID + offset_method_inside_group;
      }
      #endif
    }
    return BUSTYPE__NONE__ID;
  }
};

#endif // USE_MODULE_LIGHTS_ANIMATOR

#endif // guard
