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
  BUSTYPE__8266_U1_NEO_3__ID = 2,
  BUSTYPE__8266_DM_NEO_3__ID = 3,
  BUSTYPE__8266_BB_NEO_3__ID = 4,
  // RGBW
  BUSTYPE__8266_U0_NEO_4__ID = 5,
  BUSTYPE__8266_U1_NEO_4__ID = 6,
  BUSTYPE__8266_DM_NEO_4__ID = 7,
  BUSTYPE__8266_BB_NEO_4__ID = 8,
  // 400Kbps
  BUSTYPE__8266_U0_400_3__ID = 9,
  BUSTYPE__8266_U1_400_3__ID = 10,
  BUSTYPE__8266_DM_400_3__ID = 11,
  BUSTYPE__8266_BB_400_3__ID = 12,
  // TM1814 (RGBW)
  BUSTYPE__8266_U0_TM1_4__ID = 13,
  BUSTYPE__8266_U1_TM1_4__ID = 14,
  BUSTYPE__8266_DM_TM1_4__ID = 15,
  BUSTYPE__8266_BB_TM1_4__ID = 16,
  // TM1829 (RGB)
  BUSTYPE__8266_U0_TM2_3__ID = 17,
  BUSTYPE__8266_U1_TM2_3__ID = 18,
  BUSTYPE__8266_DM_TM2_3__ID = 19,
  BUSTYPE__8266_BB_TM2_3__ID = 20,
  /*** ESP32 Neopixel methods ***/
  // RGB
  BUSTYPE__32_RN_NEO_3__ID = 21,
  BUSTYPE__32_I0_NEO_3__ID = 22,
  BUSTYPE__32_I1_NEO_3__ID = 23,
  BUSTYPE__32_BB_NEO_3__ID = 24,  // bitbangging on ESP32 not recommended
  // RGBW
  BUSTYPE__32_RN_NEO_4__ID = 25,
  BUSTYPE__32_I0_NEO_4__ID = 26,
  BUSTYPE__32_I1_NEO_4__ID = 27,
  BUSTYPE__32_BB_NEO_4__ID = 28,  // bitbangging on ESP32 not recommended
  // 400Kbps
  BUSTYPE__32_RN_400_3__ID = 29,
  BUSTYPE__32_I0_400_3__ID = 30,
  BUSTYPE__32_I1_400_3__ID = 31,
  BUSTYPE__32_BB_400_3__ID = 32,  // bitbangging on ESP32 not recommended
  // TM1814 (RGBW)
  BUSTYPE__32_RN_TM1_4__ID = 33,
  BUSTYPE__32_I0_TM1_4__ID = 34,
  BUSTYPE__32_I1_TM1_4__ID = 35,
  // TM1829 (RGB)
  BUSTYPE__32_RN_TM2_3__ID = 36,
  BUSTYPE__32_I0_TM2_3__ID = 37,
  BUSTYPE__32_I1_TM2_3__ID = 38,
  // APA102
  BUSTYPE__HS_DOT_3__ID = 39, // hardware SPI
  BUSTYPE__SS_DOT_3__ID = 40, // soft SPI
  // LPD8806
  BUSTYPE__HS_LPD_3__ID = 41,
  BUSTYPE__SS_LPD_3__ID = 42,
  // WS2801
  BUSTYPE__HS_WS1_3__ID = 43,
  BUSTYPE__SS_WS1_3__ID = 44,
  // P9813
  BUSTYPE__HS_P98_3__ID = 45,
  BUSTYPE__SS_P98_3__ID = 46,
  // LPD6803
  BUSTYPE__HS_LPO_3__ID = 47,
  BUSTYPE__SS_LPO_3__ID = 48
};


/*** ESP8266 Neopixel methods ***/
#ifdef ESP8266
//RGB
#define NEOPIXELBUS_8266_U0_NEO_3 NeoPixelBus<NeoRgbFeature, NeoEsp8266Uart0Ws2813Method> //3 chan, esp8266, gpio1
#define NEOPIXELBUS_8266_U1_NEO_3 NeoPixelBus<NeoRgbFeature, NeoEsp8266Uart1Ws2813Method> //3 chan, esp8266, gpio2
#define NEOPIXELBUS_8266_DM_NEO_3 NeoPixelBus<NeoRgbFeature, NeoEsp8266Dma800KbpsMethod>  //3 chan, esp8266, gpio3
#define NEOPIXELBUS_8266_BB_NEO_3 NeoPixelBus<NeoRgbFeature, NeoEsp8266BitBang800KbpsMethod> //3 chan, esp8266, bb (any pin but 16)
//RGBW
#define NEOPIXELBUS_8266_U0_NEO_4 NeoPixelBus<NeoRgbwFeature, NeoEsp8266Uart0Ws2813Method>   //4 chan, esp8266, gpio1
#define NEOPIXELBUS_8266_U1_NEO_4 NeoPixelBus<NeoRgbwFeature, NeoEsp8266Uart1Ws2813Method>   //4 chan, esp8266, gpio2
#define NEOPIXELBUS_8266_DM_NEO_4 NeoPixelBus<NeoRgbwFeature, NeoEsp8266Dma800KbpsMethod>    //4 chan, esp8266, gpio3
#define NEOPIXELBUS_8266_BB_NEO_4 NeoPixelBus<NeoRgbwFeature, NeoEsp8266BitBang800KbpsMethod> //4 chan, esp8266, bb (any pin)
//400Kbps
#define NEOPIXELBUS_8266_U0_400_3 NeoPixelBus<NeoRgbFeature, NeoEsp8266Uart0400KbpsMethod>   //3 chan, esp8266, gpio1
#define NEOPIXELBUS_8266_U1_400_3 NeoPixelBus<NeoRgbFeature, NeoEsp8266Uart1400KbpsMethod>   //3 chan, esp8266, gpio2
#define NEOPIXELBUS_8266_DM_400_3 NeoPixelBus<NeoRgbFeature, NeoEsp8266Dma400KbpsMethod>     //3 chan, esp8266, gpio3
#define NEOPIXELBUS_8266_BB_400_3 NeoPixelBus<NeoRgbFeature, NeoEsp8266BitBang400KbpsMethod> //3 chan, esp8266, bb (any pin)
//TM1814 (RGBW)
// #define NEOPIXELBUS_8266_U0_TM1_4 NeoPixelBus<NeoWrgbTm1814Feature, NeoEsp8266Uart0Tm1814Method>
// #define NEOPIXELBUS_8266_U1_TM1_4 NeoPixelBus<NeoWrgbTm1814Feature, NeoEsp8266Uart1Tm1814Method>
// #define NEOPIXELBUS_8266_DM_TM1_4 NeoPixelBus<NeoWrgbTm1814Feature, NeoEsp8266DmaTm1814Method>
// #define NEOPIXELBUS_8266_BB_TM1_4 NeoPixelBus<NeoWrgbTm1814Feature, NeoEsp8266BitBangTm1814Method>
//TM1829 (RGB)
// #define NEOPIXELBUS_8266_U0_TM2_4 NeoPixelBus<NeoBrgFeature, NeoEsp8266Uart0Tm1829Method>
// #define NEOPIXELBUS_8266_U1_TM2_4 NeoPixelBus<NeoBrgFeature, NeoEsp8266Uart1Tm1829Method>
// #define NEOPIXELBUS_8266_DM_TM2_4 NeoPixelBus<NeoBrgFeature, NeoEsp8266DmaTm1829Method>
// #define NEOPIXELBUS_8266_BB_TM2_4 NeoPixelBus<NeoBrgFeature, NeoEsp8266BitBangTm1829Method>
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
//RGBW
#define NEOPIXELBUS_32_RN_NEO_4 NeoPixelBus<NeoRgbwFeature, NeoEsp32RmtNWs2812xMethod>
#ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
#define NEOPIXELBUS_32_I0_NEO_4 NeoPixelBus<NeoRgbwFeature, NeoEsp32I2s0800KbpsMethod>
#endif
#ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
#define NEOPIXELBUS_32_I1_NEO_4 NeoPixelBus<NeoRgbwFeature, NeoEsp32I2s1800KbpsMethod>
#endif
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
#define NEOPIXELBUS_HS_DOT_3 NeoPixelBus<DotStarBgrFeature, DotStarSpi5MhzMethod> //hardware SPI
#define NEOPIXELBUS_SS_DOT_3 NeoPixelBus<DotStarBgrFeature, DotStarMethod>    //soft SPI

// //LPD8806
// #define NEOPIXELBUS_HS_LPD_3 NeoPixelBus<Lpd8806RgbFeature, Lpd8806SpiMethod>
// #define NEOPIXELBUS_SS_LPD_3 NeoPixelBus<Lpd8806RgbFeature, Lpd8806Method>

//LPD6803
#define NEOPIXELBUS_HS_LPO_3 NeoPixelBus<Lpd6803RgbFeature, Lpd6803SpiMethod>
#define NEOPIXELBUS_SS_LPO_3 NeoPixelBus<Lpd6803RgbFeature, Lpd6803Method>

// //WS2801
// #if defined(WLED_WS2801_SPEED_KHZ) && WLED_WS2801_SPEED_KHZ==40000
// #define NEOPIXELBUS_HS_WS1_3 NeoPixelBus<NeoRbgFeature, NeoWs2801Spi40MhzMethod>    // fastest bus speed (not existing method?)
// #elif defined(WLED_WS2801_SPEED_KHZ) && WLED_WS2801_SPEED_KHZ==20000
// #define NEOPIXELBUS_HS_WS1_3 NeoPixelBus<NeoRbgFeature, NeoWs2801Spi20MhzMethod>    // 20MHz
// #elif defined(WLED_WS2801_SPEED_KHZ) && WLED_WS2801_SPEED_KHZ==10000
// #define NEOPIXELBUS_HS_WS1_3 NeoPixelBus<NeoRbgFeature, NeoWs2801SpiMethod>         // 10MHz
// #elif defined(WLED_WS2801_SPEED_KHZ) && WLED_WS2801_SPEED_KHZ==2000
// #define NEOPIXELBUS_HS_WS1_3 NeoPixelBus<NeoRbgFeature, NeoWs2801Spi2MhzMethod>     //slower, more compatible
// #elif defined(WLED_WS2801_SPEED_KHZ) && WLED_WS2801_SPEED_KHZ==1000
// #define NEOPIXELBUS_HS_WS1_3 NeoPixelBus<NeoRbgFeature, NeoWs2801Spi1MhzMethod>     //slower, more compatible
// #elif defined(WLED_WS2801_SPEED_KHZ) && WLED_WS2801_SPEED_KHZ==500
// #define NEOPIXELBUS_HS_WS1_3 NeoPixelBus<NeoRbgFeature, NeoWs2801Spi500KhzMethod>   //slower, more compatible
// #else
// #define NEOPIXELBUS_HS_WS1_3 NeoPixelBus<NeoRbgFeature, NeoWs2801Spi2MhzMethod>     // 2MHz; slower, more compatible
// #endif
// #define NEOPIXELBUS_SS_WS1_3 NeoPixelBus<NeoRbgFeature, NeoWs2801Method>

// //P9813
// #define NEOPIXELBUS_HS_P98_3 NeoPixelBus<P9813BgrFeature, P9813SpiMethod>
// #define NEOPIXELBUS_SS_P98_3 NeoPixelBus<P9813BgrFeature, P9813Method>

//handles pointer type conversion for all possible bus types
class PolyBus {
  public:
  // Begin & initialize the PixelSettings for TM1814 strips.
  template <class T>
  static void beginTM1814(void* busPtr) {
    T tm1814_strip = static_cast<T>(busPtr);
    tm1814_strip->Begin();
    // Max current for each LED (22.5 mA).
    tm1814_strip->SetPixelSettings(NeoTm1814Settings(/*R*/225, /*G*/225, /*B*/225, /*W*/225));
  }
  static void begin(void* busPtr, uint8_t busType, uint8_t* pins) 
  {
    
    DEBUG_PRINTF("PolyBus::begin busPtr UNSET, busType %d, pin[0] %d\n\r", busType, pins[0]);

    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
      case BUSTYPE__8266_U0_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_U0_NEO_3*>(busPtr))->Begin(); break;
      case BUSTYPE__8266_U1_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_U1_NEO_3*>(busPtr))->Begin(); break;
      case BUSTYPE__8266_DM_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_DM_NEO_3*>(busPtr))->Begin(); break;
      case BUSTYPE__8266_BB_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_BB_NEO_3*>(busPtr))->Begin(); break;
      case BUSTYPE__8266_U0_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_U0_NEO_4*>(busPtr))->Begin(); break;
      case BUSTYPE__8266_U1_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_U1_NEO_4*>(busPtr))->Begin(); break;
      case BUSTYPE__8266_DM_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_DM_NEO_4*>(busPtr))->Begin(); break;
      case BUSTYPE__8266_BB_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_BB_NEO_4*>(busPtr))->Begin(); break;
      case BUSTYPE__8266_U0_400_3__ID: (static_cast<NEOPIXELBUS_8266_U0_400_3*>(busPtr))->Begin(); break;
      case BUSTYPE__8266_U1_400_3__ID: (static_cast<NEOPIXELBUS_8266_U1_400_3*>(busPtr))->Begin(); break;
      case BUSTYPE__8266_DM_400_3__ID: (static_cast<NEOPIXELBUS_8266_DM_400_3*>(busPtr))->Begin(); break;
      case BUSTYPE__8266_BB_400_3__ID: (static_cast<NEOPIXELBUS_8266_BB_400_3*>(busPtr))->Begin(); break;
      // case BUSTYPE__8266_U0_TM1_4__ID: beginTM1814<NEOPIXELBUS_8266_U0_TM1_4*>(busPtr); break;
      // case BUSTYPE__8266_U1_TM1_4__ID: beginTM1814<NEOPIXELBUS_8266_U1_TM1_4*>(busPtr); break;
      // case BUSTYPE__8266_DM_TM1_4__ID: beginTM1814<NEOPIXELBUS_8266_DM_TM1_4*>(busPtr); break;
      // case BUSTYPE__8266_BB_TM1_4__ID: beginTM1814<NEOPIXELBUS_8266_BB_TM1_4*>(busPtr); break;
      // case BUSTYPE__8266_U0_TM2_3__ID: (static_cast<NEOPIXELBUS_8266_U0_TM2_4*>(busPtr))->Begin(); break;
      // case BUSTYPE__8266_U1_TM2_3__ID: (static_cast<NEOPIXELBUS_8266_U1_TM2_4*>(busPtr))->Begin(); break;
      // case BUSTYPE__8266_DM_TM2_3__ID: (static_cast<NEOPIXELBUS_8266_DM_TM2_4*>(busPtr))->Begin(); break;
      // case BUSTYPE__8266_BB_TM2_3__ID: (static_cast<NEOPIXELBUS_8266_BB_TM2_4*>(busPtr))->Begin(); break;
      // case BUSTYPE__HS_DOT_3__ID: (static_cast<NEOPIXELBUS_HS_DOT_3*>(busPtr))->Begin(); break;
      // case BUSTYPE__HS_LPD_3__ID: (static_cast<NEOPIXELBUS_HS_LPD_3*>(busPtr))->Begin(); break;
      // case BUSTYPE__HS_LPO_3__ID: (static_cast<NEOPIXELBUS_HS_LPO_3*>(busPtr))->Begin(); break;
      // case BUSTYPE__HS_WS1_3__ID: (static_cast<NEOPIXELBUS_HS_WS1_3*>(busPtr))->Begin(); break;
      // case BUSTYPE__HS_P98_3__ID: (static_cast<NEOPIXELBUS_HS_P98_3*>(busPtr))->Begin(); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32
      case BUSTYPE__32_RN_NEO_3__ID: (static_cast<NEOPIXELBUS_32_RN_NEO_3*>(busPtr))->Begin(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_3__ID: (static_cast<NEOPIXELBUS_32_I0_NEO_3*>(busPtr))->Begin(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_3__ID: (static_cast<NEOPIXELBUS_32_I1_NEO_3*>(busPtr))->Begin(); break;
      #endif
      case BUSTYPE__32_RN_NEO_4__ID: (static_cast<NEOPIXELBUS_32_RN_NEO_4*>(busPtr))->Begin(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_4__ID: (static_cast<NEOPIXELBUS_32_I0_NEO_4*>(busPtr))->Begin(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_4__ID: (static_cast<NEOPIXELBUS_32_I1_NEO_4*>(busPtr))->Begin(); break;
      #endif
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
      // ESP32 can (and should, to avoid inadvertantly driving the chip select signal) specify the pins used for SPI, but only in begin()
      // case BUSTYPE__HS_DOT_3__ID: (static_cast<NEOPIXELBUS_HS_DOT_3*>(busPtr))->Begin(pins[1], -1, pins[0], -1); break;
      // case BUSTYPE__HS_LPD_3__ID: (static_cast<NEOPIXELBUS_HS_LPD_3*>(busPtr))->Begin(pins[1], -1, pins[0], -1); break;
      // case BUSTYPE__HS_LPO_3__ID: (static_cast<NEOPIXELBUS_HS_LPO_3*>(busPtr))->Begin(pins[1], -1, pins[0], -1); break;
      // case BUSTYPE__HS_WS1_3__ID: (static_cast<NEOPIXELBUS_HS_WS1_3*>(busPtr))->Begin(pins[1], -1, pins[0], -1); break;
      // case BUSTYPE__HS_P98_3__ID: (static_cast<NEOPIXELBUS_HS_P98_3*>(busPtr))->Begin(pins[1], -1, pins[0], -1); break;
    #endif
      // case BUSTYPE__SS_DOT_3__ID: (static_cast<NEOPIXELBUS_SS_DOT_3*>(busPtr))->Begin(); break;
      // case BUSTYPE__SS_LPD_3__ID: (static_cast<NEOPIXELBUS_SS_LPD_3*>(busPtr))->Begin(); break;
      // case BUSTYPE__SS_LPO_3__ID: (static_cast<NEOPIXELBUS_SS_LPO_3*>(busPtr))->Begin(); break;
      // case BUSTYPE__SS_WS1_3__ID: (static_cast<NEOPIXELBUS_SS_WS1_3*>(busPtr))->Begin(); break;
      // case BUSTYPE__SS_P98_3__ID: (static_cast<NEOPIXELBUS_SS_P98_3*>(busPtr))->Begin(); break;
    }
  };
  static void* create(uint8_t busType, uint8_t* pins, uint16_t len, uint8_t channel) 
  {
    DEBUG_PRINTF("PolyBus::create busType %d, pin[0] %d, len %d, channel %d\n\r", busType, pins[0], len, channel);
    
    void* busPtr = nullptr;
    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
      case BUSTYPE__8266_U0_NEO_3__ID: busPtr = new NEOPIXELBUS_8266_U0_NEO_3(len, pins[0]); break;
      case BUSTYPE__8266_U1_NEO_3__ID: busPtr = new NEOPIXELBUS_8266_U1_NEO_3(len, pins[0]); break;
      case BUSTYPE__8266_DM_NEO_3__ID: busPtr = new NEOPIXELBUS_8266_DM_NEO_3(len, pins[0]); break;
      case BUSTYPE__8266_BB_NEO_3__ID: busPtr = new NEOPIXELBUS_8266_BB_NEO_3(len, pins[0]); break;
      case BUSTYPE__8266_U0_NEO_4__ID: busPtr = new NEOPIXELBUS_8266_U0_NEO_4(len, pins[0]); break;
      case BUSTYPE__8266_U1_NEO_4__ID: busPtr = new NEOPIXELBUS_8266_U1_NEO_4(len, pins[0]); break;
      case BUSTYPE__8266_DM_NEO_4__ID: busPtr = new NEOPIXELBUS_8266_DM_NEO_4(len, pins[0]); break;
      case BUSTYPE__8266_BB_NEO_4__ID: busPtr = new NEOPIXELBUS_8266_BB_NEO_4(len, pins[0]); break;
      case BUSTYPE__8266_U0_400_3__ID: busPtr = new NEOPIXELBUS_8266_U0_400_3(len, pins[0]); break;
      case BUSTYPE__8266_U1_400_3__ID: busPtr = new NEOPIXELBUS_8266_U1_400_3(len, pins[0]); break;
      case BUSTYPE__8266_DM_400_3__ID: busPtr = new NEOPIXELBUS_8266_DM_400_3(len, pins[0]); break;
      case BUSTYPE__8266_BB_400_3__ID: busPtr = new NEOPIXELBUS_8266_BB_400_3(len, pins[0]); break;
      // case BUSTYPE__8266_U0_TM1_4__ID: busPtr = new NEOPIXELBUS_8266_U0_TM1_4(len, pins[0]); break;
      // case BUSTYPE__8266_U1_TM1_4__ID: busPtr = new NEOPIXELBUS_8266_U1_TM1_4(len, pins[0]); break;
      // case BUSTYPE__8266_DM_TM1_4__ID: busPtr = new NEOPIXELBUS_8266_DM_TM1_4(len, pins[0]); break;
      // case BUSTYPE__8266_BB_TM1_4__ID: busPtr = new NEOPIXELBUS_8266_BB_TM1_4(len, pins[0]); break;
      // case BUSTYPE__8266_U0_TM2_3__ID: busPtr = new NEOPIXELBUS_8266_U0_TM2_4(len, pins[0]); break;
      // case BUSTYPE__8266_U1_TM2_3__ID: busPtr = new NEOPIXELBUS_8266_U1_TM2_4(len, pins[0]); break;
      // case BUSTYPE__8266_DM_TM2_3__ID: busPtr = new NEOPIXELBUS_8266_DM_TM2_4(len, pins[0]); break;
      // case BUSTYPE__8266_BB_TM2_3__ID: busPtr = new NEOPIXELBUS_8266_BB_TM2_4(len, pins[0]); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32
      case BUSTYPE__32_RN_NEO_3__ID: busPtr = new NEOPIXELBUS_32_RN_NEO_3(len, pins[0], (NeoBusChannel)channel); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_3__ID: busPtr = new NEOPIXELBUS_32_I0_NEO_3(len, pins[0]); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_3__ID: busPtr = new NEOPIXELBUS_32_I1_NEO_3(len, pins[0]); break;
      #endif
      case BUSTYPE__32_RN_NEO_4__ID: busPtr = new NEOPIXELBUS_32_RN_NEO_4(len, pins[0], (NeoBusChannel)channel); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_4__ID: busPtr = new NEOPIXELBUS_32_I0_NEO_4(len, pins[0]); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_4__ID: busPtr = new NEOPIXELBUS_32_I1_NEO_4(len, pins[0]); break;
      #endif
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
      // for 2-wire: pins[1] is clk, pins[0] is dat.  begin expects (len, clk, dat)
      case BUSTYPE__HS_DOT_3__ID: busPtr = new NEOPIXELBUS_HS_DOT_3(len, pins[1], pins[0]); break;
      case BUSTYPE__SS_DOT_3__ID: busPtr = new NEOPIXELBUS_SS_DOT_3(len, pins[1], pins[0]); break;
      // case BUSTYPE__HS_LPD_3__ID: busPtr = new NEOPIXELBUS_HS_LPD_3(len, pins[1], pins[0]); break;
      // case BUSTYPE__SS_LPD_3__ID: busPtr = new NEOPIXELBUS_SS_LPD_3(len, pins[1], pins[0]); break;
      case BUSTYPE__HS_LPO_3__ID: busPtr = new NEOPIXELBUS_HS_LPO_3(len, pins[1], pins[0]); break;
      case BUSTYPE__SS_LPO_3__ID: busPtr = new NEOPIXELBUS_SS_LPO_3(len, pins[1], pins[0]); break;
      // case BUSTYPE__HS_WS1_3__ID: busPtr = new NEOPIXELBUS_HS_WS1_3(len, pins[1], pins[0]); break;
      // case BUSTYPE__SS_WS1_3__ID: busPtr = new NEOPIXELBUS_SS_WS1_3(len, pins[1], pins[0]); break;
      // case BUSTYPE__HS_P98_3__ID: busPtr = new NEOPIXELBUS_HS_P98_3(len, pins[1], pins[0]); break;
      // case BUSTYPE__SS_P98_3__ID: busPtr = new NEOPIXELBUS_SS_P98_3(len, pins[1], pins[0]); break;
    }
    begin(busPtr, busType, pins);
    return busPtr;
  };
  static void show(void* busPtr, uint8_t busType) 
  {
    
    // DEBUG_PRINTF("PolyBus::show busType %d\n\r", busType);

    
    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
      case BUSTYPE__8266_U0_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_U0_NEO_3*>(busPtr))->Show(); break;
      case BUSTYPE__8266_U1_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_U1_NEO_3*>(busPtr))->Show(); break;
      case BUSTYPE__8266_DM_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_DM_NEO_3*>(busPtr))->Show(); break;
      case BUSTYPE__8266_BB_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_BB_NEO_3*>(busPtr))->Show(); break;
      case BUSTYPE__8266_U0_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_U0_NEO_4*>(busPtr))->Show(); break;
      case BUSTYPE__8266_U1_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_U1_NEO_4*>(busPtr))->Show(); break;
      case BUSTYPE__8266_DM_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_DM_NEO_4*>(busPtr))->Show(); break;
      case BUSTYPE__8266_BB_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_BB_NEO_4*>(busPtr))->Show(); break;
      case BUSTYPE__8266_U0_400_3__ID: (static_cast<NEOPIXELBUS_8266_U0_400_3*>(busPtr))->Show(); break;
      case BUSTYPE__8266_U1_400_3__ID: (static_cast<NEOPIXELBUS_8266_U1_400_3*>(busPtr))->Show(); break;
      case BUSTYPE__8266_DM_400_3__ID: (static_cast<NEOPIXELBUS_8266_DM_400_3*>(busPtr))->Show(); break;
      case BUSTYPE__8266_BB_400_3__ID: (static_cast<NEOPIXELBUS_8266_BB_400_3*>(busPtr))->Show(); break;
      // case BUSTYPE__8266_U0_TM1_4__ID: (static_cast<NEOPIXELBUS_8266_U0_TM1_4*>(busPtr))->Show(); break;
      // case BUSTYPE__8266_U1_TM1_4__ID: (static_cast<NEOPIXELBUS_8266_U1_TM1_4*>(busPtr))->Show(); break;
      // case BUSTYPE__8266_DM_TM1_4__ID: (static_cast<NEOPIXELBUS_8266_DM_TM1_4*>(busPtr))->Show(); break;
      // case BUSTYPE__8266_BB_TM1_4__ID: (static_cast<NEOPIXELBUS_8266_BB_TM1_4*>(busPtr))->Show(); break;
      // case BUSTYPE__8266_U0_TM2_3__ID: (static_cast<NEOPIXELBUS_8266_U0_TM2_4*>(busPtr))->Show(); break;
      // case BUSTYPE__8266_U1_TM2_3__ID: (static_cast<NEOPIXELBUS_8266_U1_TM2_4*>(busPtr))->Show(); break;
      // case BUSTYPE__8266_DM_TM2_3__ID: (static_cast<NEOPIXELBUS_8266_DM_TM2_4*>(busPtr))->Show(); break;
      // case BUSTYPE__8266_BB_TM2_3__ID: (static_cast<NEOPIXELBUS_8266_BB_TM2_4*>(busPtr))->Show(); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32
      case BUSTYPE__32_RN_NEO_3__ID: (static_cast<NEOPIXELBUS_32_RN_NEO_3*>(busPtr))->Show(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_3__ID: (static_cast<NEOPIXELBUS_32_I0_NEO_3*>(busPtr))->Show(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_3__ID: (static_cast<NEOPIXELBUS_32_I1_NEO_3*>(busPtr))->Show(); break;
      #endif
      case BUSTYPE__32_RN_NEO_4__ID: (static_cast<NEOPIXELBUS_32_RN_NEO_4*>(busPtr))->Show(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_4__ID: (static_cast<NEOPIXELBUS_32_I0_NEO_4*>(busPtr))->Show(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_4__ID: (static_cast<NEOPIXELBUS_32_I1_NEO_4*>(busPtr))->Show(); break;
      #endif
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
      case BUSTYPE__HS_DOT_3__ID: (static_cast<NEOPIXELBUS_HS_DOT_3*>(busPtr))->Show(); break;
      case BUSTYPE__SS_DOT_3__ID: (static_cast<NEOPIXELBUS_SS_DOT_3*>(busPtr))->Show(); break;
      // case BUSTYPE__HS_LPD_3__ID: (static_cast<NEOPIXELBUS_HS_LPD_3*>(busPtr))->Show(); break;
      // case BUSTYPE__SS_LPD_3__ID: (static_cast<NEOPIXELBUS_SS_LPD_3*>(busPtr))->Show(); break;
      case BUSTYPE__HS_LPO_3__ID: (static_cast<NEOPIXELBUS_HS_LPO_3*>(busPtr))->Show(); break;
      case BUSTYPE__SS_LPO_3__ID: (static_cast<NEOPIXELBUS_SS_LPO_3*>(busPtr))->Show(); break;
      // case BUSTYPE__HS_WS1_3__ID: (static_cast<NEOPIXELBUS_HS_WS1_3*>(busPtr))->Show(); break;
      // case BUSTYPE__SS_WS1_3__ID: (static_cast<NEOPIXELBUS_SS_WS1_3*>(busPtr))->Show(); break;
      // case BUSTYPE__HS_P98_3__ID: (static_cast<NEOPIXELBUS_HS_P98_3*>(busPtr))->Show(); break;
      // case BUSTYPE__SS_P98_3__ID: (static_cast<NEOPIXELBUS_SS_P98_3*>(busPtr))->Show(); break;
    }


  };
  static bool canShow(void* busPtr, uint8_t busType) 
  {
    
    DEBUG_PRINTF("PolyBus::canShow busType %d\n\r", busType);

    switch (busType) {
      case BUSTYPE__NONE__ID: return true;
    #ifdef ESP8266
      case BUSTYPE__8266_U0_NEO_3__ID: return (static_cast<NEOPIXELBUS_8266_U0_NEO_3*>(busPtr))->CanShow(); break;
      case BUSTYPE__8266_U1_NEO_3__ID: return (static_cast<NEOPIXELBUS_8266_U1_NEO_3*>(busPtr))->CanShow(); break;
      case BUSTYPE__8266_DM_NEO_3__ID: return (static_cast<NEOPIXELBUS_8266_DM_NEO_3*>(busPtr))->CanShow(); break;
      case BUSTYPE__8266_BB_NEO_3__ID: return (static_cast<NEOPIXELBUS_8266_BB_NEO_3*>(busPtr))->CanShow(); break;
      case BUSTYPE__8266_U0_NEO_4__ID: return (static_cast<NEOPIXELBUS_8266_U0_NEO_4*>(busPtr))->CanShow(); break;
      case BUSTYPE__8266_U1_NEO_4__ID: return (static_cast<NEOPIXELBUS_8266_U1_NEO_4*>(busPtr))->CanShow(); break;
      case BUSTYPE__8266_DM_NEO_4__ID: return (static_cast<NEOPIXELBUS_8266_DM_NEO_4*>(busPtr))->CanShow(); break;
      case BUSTYPE__8266_BB_NEO_4__ID: return (static_cast<NEOPIXELBUS_8266_BB_NEO_4*>(busPtr))->CanShow(); break;
      case BUSTYPE__8266_U0_400_3__ID: return (static_cast<NEOPIXELBUS_8266_U0_400_3*>(busPtr))->CanShow(); break;
      case BUSTYPE__8266_U1_400_3__ID: return (static_cast<NEOPIXELBUS_8266_U1_400_3*>(busPtr))->CanShow(); break;
      case BUSTYPE__8266_DM_400_3__ID: return (static_cast<NEOPIXELBUS_8266_DM_400_3*>(busPtr))->CanShow(); break;
      case BUSTYPE__8266_BB_400_3__ID: return (static_cast<NEOPIXELBUS_8266_BB_400_3*>(busPtr))->CanShow(); break;
      // case BUSTYPE__8266_U0_TM1_4__ID: return (static_cast<NEOPIXELBUS_8266_U0_TM1_4*>(busPtr))->CanShow(); break;
      // case BUSTYPE__8266_U1_TM1_4__ID: return (static_cast<NEOPIXELBUS_8266_U1_TM1_4*>(busPtr))->CanShow(); break;
      // case BUSTYPE__8266_DM_TM1_4__ID: return (static_cast<NEOPIXELBUS_8266_DM_TM1_4*>(busPtr))->CanShow(); break;
      // case BUSTYPE__8266_BB_TM1_4__ID: return (static_cast<NEOPIXELBUS_8266_BB_TM1_4*>(busPtr))->CanShow(); break;
      // case BUSTYPE__8266_U0_TM2_3__ID: return (static_cast<NEOPIXELBUS_8266_U0_TM2_4*>(busPtr))->CanShow(); break;
      // case BUSTYPE__8266_U1_TM2_3__ID: return (static_cast<NEOPIXELBUS_8266_U1_TM2_4*>(busPtr))->CanShow(); break;
      // case BUSTYPE__8266_DM_TM2_3__ID: return (static_cast<NEOPIXELBUS_8266_DM_TM2_4*>(busPtr))->CanShow(); break;
      // case BUSTYPE__8266_BB_TM2_3__ID: return (static_cast<NEOPIXELBUS_8266_BB_TM2_4*>(busPtr))->CanShow(); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32
      case BUSTYPE__32_RN_NEO_3__ID: return (static_cast<NEOPIXELBUS_32_RN_NEO_3*>(busPtr))->CanShow(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_3__ID: return (static_cast<NEOPIXELBUS_32_I0_NEO_3*>(busPtr))->CanShow(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_3__ID: return (static_cast<NEOPIXELBUS_32_I1_NEO_3*>(busPtr))->CanShow(); break;
      #endif
      case BUSTYPE__32_RN_NEO_4__ID: return (static_cast<NEOPIXELBUS_32_RN_NEO_4*>(busPtr))->CanShow(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_4__ID: return (static_cast<NEOPIXELBUS_32_I0_NEO_4*>(busPtr))->CanShow(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_4__ID: return (static_cast<NEOPIXELBUS_32_I1_NEO_4*>(busPtr))->CanShow(); break;
      #endif
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
      case BUSTYPE__HS_DOT_3__ID: return (static_cast<NEOPIXELBUS_HS_DOT_3*>(busPtr))->CanShow(); break;
      case BUSTYPE__SS_DOT_3__ID: return (static_cast<NEOPIXELBUS_SS_DOT_3*>(busPtr))->CanShow(); break;
      // case BUSTYPE__HS_LPD_3__ID: return (static_cast<NEOPIXELBUS_HS_LPD_3*>(busPtr))->CanShow(); break;
      // case BUSTYPE__SS_LPD_3__ID: return (static_cast<NEOPIXELBUS_SS_LPD_3*>(busPtr))->CanShow(); break;
      case BUSTYPE__HS_LPO_3__ID: return (static_cast<NEOPIXELBUS_HS_LPO_3*>(busPtr))->CanShow(); break;
      case BUSTYPE__SS_LPO_3__ID: return (static_cast<NEOPIXELBUS_SS_LPO_3*>(busPtr))->CanShow(); break;
      // case BUSTYPE__HS_WS1_3__ID: return (static_cast<NEOPIXELBUS_HS_WS1_3*>(busPtr))->CanShow(); break;
      // case BUSTYPE__SS_WS1_3__ID: return (static_cast<NEOPIXELBUS_SS_WS1_3*>(busPtr))->CanShow(); break;
      // case BUSTYPE__HS_P98_3__ID: return (static_cast<NEOPIXELBUS_HS_P98_3*>(busPtr))->CanShow(); break;
      // case BUSTYPE__SS_P98_3__ID: return (static_cast<NEOPIXELBUS_SS_P98_3*>(busPtr))->CanShow(); break;
    }
    return true;
  };
  
  static void setPixelColor(void* busPtr, uint8_t busType, uint16_t pix, RgbcctColor colour_internal, COLOUR_ORDER_T colour_order = {0}) 
  {
    
    RgbcctColor colour_hardware = colour_internal; // Start with original
    colour_hardware = RgbcctColor(0); // To debug, clear conversion

    if(colour_order.red        != COLOUR_ORDER_DISABLED){ colour_hardware.raw[colour_order.red]         = colour_internal.R;  }
    if(colour_order.green      != COLOUR_ORDER_DISABLED){ colour_hardware.raw[colour_order.green]       = colour_internal.G;  }
    if(colour_order.blue       != COLOUR_ORDER_DISABLED){ colour_hardware.raw[colour_order.blue]        = colour_internal.B;  }
    if(colour_order.white_cold != COLOUR_ORDER_DISABLED){ colour_hardware.raw[colour_order.white_cold]  = colour_internal.WC; }
    if(colour_order.white_warm != COLOUR_ORDER_DISABLED){ colour_hardware.raw[colour_order.white_warm]  = colour_internal.WW; }
    #ifdef ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS
    if(pix==0){ // Just first pixel
      Serial.printf("set colour_order R=%d, G=%d, B=%d, CW=%d, WW=%d\n\r",
        colour_order.red,
        colour_order.green,
        colour_order.blue,
        colour_order.white_cold,
        colour_order.white_warm
      );
    }
    #endif // ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS
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


    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
      case BUSTYPE__8266_U0_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_U0_NEO_3*>(busPtr))->SetPixelColor(pix, RgbColor(colour_hardware.R,colour_hardware.G,colour_hardware.B)); break;
      case BUSTYPE__8266_U1_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_U1_NEO_3*>(busPtr))->SetPixelColor(pix, RgbColor(colour_hardware.R,colour_hardware.G,colour_hardware.B)); break;
      case BUSTYPE__8266_DM_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_DM_NEO_3*>(busPtr))->SetPixelColor(pix, RgbColor(colour_hardware.R,colour_hardware.G,colour_hardware.B)); break;
      case BUSTYPE__8266_BB_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_BB_NEO_3*>(busPtr))->SetPixelColor(pix, RgbColor(colour_hardware.R,colour_hardware.G,colour_hardware.B)); break;
      case BUSTYPE__8266_U0_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_U0_NEO_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      case BUSTYPE__8266_U1_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_U1_NEO_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      case BUSTYPE__8266_DM_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_DM_NEO_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      case BUSTYPE__8266_BB_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_BB_NEO_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      case BUSTYPE__8266_U0_400_3__ID: (static_cast<NEOPIXELBUS_8266_U0_400_3*>(busPtr))->SetPixelColor(pix, RgbColor(colour_hardware.R,colour_hardware.G,colour_hardware.B)); break;
      case BUSTYPE__8266_U1_400_3__ID: (static_cast<NEOPIXELBUS_8266_U1_400_3*>(busPtr))->SetPixelColor(pix, RgbColor(colour_hardware.R,colour_hardware.G,colour_hardware.B)); break;
      case BUSTYPE__8266_DM_400_3__ID: (static_cast<NEOPIXELBUS_8266_DM_400_3*>(busPtr))->SetPixelColor(pix, RgbColor(colour_hardware.R,colour_hardware.G,colour_hardware.B)); break;
      case BUSTYPE__8266_BB_400_3__ID: (static_cast<NEOPIXELBUS_8266_BB_400_3*>(busPtr))->SetPixelColor(pix, RgbColor(colour_hardware.R,colour_hardware.G,colour_hardware.B)); break;
      // case BUSTYPE__8266_U0_TM1_4__ID: (static_cast<NEOPIXELBUS_8266_U0_TM1_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      // case BUSTYPE__8266_U1_TM1_4__ID: (static_cast<NEOPIXELBUS_8266_U1_TM1_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      // case BUSTYPE__8266_DM_TM1_4__ID: (static_cast<NEOPIXELBUS_8266_DM_TM1_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      // case BUSTYPE__8266_BB_TM1_4__ID: (static_cast<NEOPIXELBUS_8266_BB_TM1_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      // case BUSTYPE__8266_U0_TM2_3__ID: (static_cast<NEOPIXELBUS_8266_U0_TM2_4*>(busPtr))->SetPixelColor(pix, RgbColor(colour_hardware.R,colour_hardware.G,colour_hardware.B)); break;
      // case BUSTYPE__8266_U1_TM2_3__ID: (static_cast<NEOPIXELBUS_8266_U1_TM2_4*>(busPtr))->SetPixelColor(pix, RgbColor(colour_hardware.R,colour_hardware.G,colour_hardware.B)); break;
      // case BUSTYPE__8266_DM_TM2_3__ID: (static_cast<NEOPIXELBUS_8266_DM_TM2_4*>(busPtr))->SetPixelColor(pix, RgbColor(colour_hardware.R,colour_hardware.G,colour_hardware.B)); break;
      // case BUSTYPE__8266_BB_TM2_3__ID: (static_cast<NEOPIXELBUS_8266_BB_TM2_4*>(busPtr))->SetPixelColor(pix, RgbColor(colour_hardware.R,colour_hardware.G,colour_hardware.B)); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32
      case BUSTYPE__32_RN_NEO_3__ID: (static_cast<NEOPIXELBUS_32_RN_NEO_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_3__ID: (static_cast<NEOPIXELBUS_32_I0_NEO_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_3__ID: (static_cast<NEOPIXELBUS_32_I1_NEO_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      case BUSTYPE__32_RN_NEO_4__ID: (static_cast<NEOPIXELBUS_32_RN_NEO_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_4__ID: (static_cast<NEOPIXELBUS_32_I0_NEO_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_4__ID: (static_cast<NEOPIXELBUS_32_I1_NEO_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
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
      case BUSTYPE__HS_DOT_3__ID: (static_cast<NEOPIXELBUS_HS_DOT_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      case BUSTYPE__SS_DOT_3__ID: (static_cast<NEOPIXELBUS_SS_DOT_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      // case BUSTYPE__HS_LPD_3__ID: (static_cast<NEOPIXELBUS_HS_LPD_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      // case BUSTYPE__SS_LPD_3__ID: (static_cast<NEOPIXELBUS_SS_LPD_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      case BUSTYPE__HS_LPO_3__ID: (static_cast<NEOPIXELBUS_HS_LPO_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      case BUSTYPE__SS_LPO_3__ID: (static_cast<NEOPIXELBUS_SS_LPO_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      // case BUSTYPE__HS_WS1_3__ID: (static_cast<NEOPIXELBUS_HS_WS1_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      // case BUSTYPE__SS_WS1_3__ID: (static_cast<NEOPIXELBUS_SS_WS1_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      // case BUSTYPE__HS_P98_3__ID: (static_cast<NEOPIXELBUS_HS_P98_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      // case BUSTYPE__SS_P98_3__ID: (static_cast<NEOPIXELBUS_SS_P98_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
    }
  };

  
  static RgbcctColor getPixelColor(void* busPtr, uint8_t busType, uint16_t pix, COLOUR_ORDER_T colour_order) 
  {

    // DEBUG_PRINTF("PolyBus::getPixelColor busType %d, pix %d, co %d\n\r", busType, pix, co);

    RgbcctColor col(0); 
    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
      case BUSTYPE__8266_U0_NEO_3__ID: col = (static_cast<NEOPIXELBUS_8266_U0_NEO_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_U1_NEO_3__ID: col = (static_cast<NEOPIXELBUS_8266_U1_NEO_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_DM_NEO_3__ID: col = (static_cast<NEOPIXELBUS_8266_DM_NEO_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_BB_NEO_3__ID: col = (static_cast<NEOPIXELBUS_8266_BB_NEO_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_U0_NEO_4__ID: col = (static_cast<NEOPIXELBUS_8266_U0_NEO_4*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_U1_NEO_4__ID: col = (static_cast<NEOPIXELBUS_8266_U1_NEO_4*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_DM_NEO_4__ID: col = (static_cast<NEOPIXELBUS_8266_DM_NEO_4*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_BB_NEO_4__ID: col = (static_cast<NEOPIXELBUS_8266_BB_NEO_4*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_U0_400_3__ID: col = (static_cast<NEOPIXELBUS_8266_U0_400_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_U1_400_3__ID: col = (static_cast<NEOPIXELBUS_8266_U1_400_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_DM_400_3__ID: col = (static_cast<NEOPIXELBUS_8266_DM_400_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_BB_400_3__ID: col = (static_cast<NEOPIXELBUS_8266_BB_400_3*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__8266_U0_TM1_4__ID: col = (static_cast<NEOPIXELBUS_8266_U0_TM1_4*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__8266_U1_TM1_4__ID: col = (static_cast<NEOPIXELBUS_8266_U1_TM1_4*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__8266_DM_TM1_4__ID: col = (static_cast<NEOPIXELBUS_8266_DM_TM1_4*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__8266_BB_TM1_4__ID: col = (static_cast<NEOPIXELBUS_8266_BB_TM1_4*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__8266_U0_TM2_3__ID: col = (static_cast<NEOPIXELBUS_8266_U0_TM2_4*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__8266_U1_TM2_3__ID: col = (static_cast<NEOPIXELBUS_8266_U1_TM2_4*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__8266_DM_TM2_3__ID: col = (static_cast<NEOPIXELBUS_8266_DM_TM2_4*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__8266_BB_TM2_3__ID: col = (static_cast<NEOPIXELBUS_8266_BB_TM2_4*>(busPtr))->GetPixelColor(pix); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32
      case BUSTYPE__32_RN_NEO_3__ID: col = (static_cast<NEOPIXELBUS_32_RN_NEO_3*>(busPtr))->GetPixelColor(pix); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_3__ID: col = (static_cast<NEOPIXELBUS_32_I0_NEO_3*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_3__ID: col = (static_cast<NEOPIXELBUS_32_I1_NEO_3*>(busPtr))->GetPixelColor(pix); break;
      #endif
      case BUSTYPE__32_RN_NEO_4__ID: col = (static_cast<NEOPIXELBUS_32_RN_NEO_4*>(busPtr))->GetPixelColor(pix); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_4__ID: col = (static_cast<NEOPIXELBUS_32_I0_NEO_4*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_4__ID: col = (static_cast<NEOPIXELBUS_32_I1_NEO_4*>(busPtr))->GetPixelColor(pix); break;
      #endif
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
      case BUSTYPE__HS_DOT_3__ID: col = (static_cast<NEOPIXELBUS_HS_DOT_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__SS_DOT_3__ID: col = (static_cast<NEOPIXELBUS_SS_DOT_3*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__HS_LPD_3__ID: col = (static_cast<NEOPIXELBUS_HS_LPD_3*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__SS_LPD_3__ID: col = (static_cast<NEOPIXELBUS_SS_LPD_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__HS_LPO_3__ID: col = (static_cast<NEOPIXELBUS_HS_LPO_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__SS_LPO_3__ID: col = (static_cast<NEOPIXELBUS_SS_LPO_3*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__HS_WS1_3__ID: col = (static_cast<NEOPIXELBUS_HS_WS1_3*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__SS_WS1_3__ID: col = (static_cast<NEOPIXELBUS_SS_WS1_3*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__HS_P98_3__ID: col = (static_cast<NEOPIXELBUS_HS_P98_3*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__SS_P98_3__ID: col = (static_cast<NEOPIXELBUS_SS_P98_3*>(busPtr))->GetPixelColor(pix); break;
    }
    
    RgbcctColor color_internal = col; 
  
    if(colour_order.red        != COLOUR_ORDER_DISABLED){ color_internal.raw[colour_order.red] = col.R; }
    if(colour_order.green      != COLOUR_ORDER_DISABLED){ color_internal.raw[colour_order.green] = col.G; }
    if(colour_order.blue       != COLOUR_ORDER_DISABLED){ color_internal.raw[colour_order.blue] = col.B; }
    if(colour_order.white_cold != COLOUR_ORDER_DISABLED){ color_internal.raw[colour_order.white_cold] = col.WW; }
    if(colour_order.white_warm != COLOUR_ORDER_DISABLED){ color_internal.raw[colour_order.white_warm] = col.WC; }
    #ifdef ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS
    if(pix==0){ // Just first pixel
      Serial.printf("get colour_order R=%d, G=%d, B=%d, CW=%d, WW=%d\n\r",
        colour_order.red,
        colour_order.green,
        colour_order.blue,
        colour_order.white_cold,
        colour_order.white_warm
      );
    }
    #endif // ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

    return color_internal;

  }

  static void cleanup(void* busPtr, uint8_t busType) 
  {

    DEBUG_PRINTF("PolyBus::cleanup busType %d\n\r", busType);
    
    if (busPtr == nullptr) return;
    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
      case BUSTYPE__8266_U0_NEO_3__ID: delete (static_cast<NEOPIXELBUS_8266_U0_NEO_3*>(busPtr)); break;
      case BUSTYPE__8266_U1_NEO_3__ID: delete (static_cast<NEOPIXELBUS_8266_U1_NEO_3*>(busPtr)); break;
      case BUSTYPE__8266_DM_NEO_3__ID: delete (static_cast<NEOPIXELBUS_8266_DM_NEO_3*>(busPtr)); break;
      case BUSTYPE__8266_BB_NEO_3__ID: delete (static_cast<NEOPIXELBUS_8266_BB_NEO_3*>(busPtr)); break;
      case BUSTYPE__8266_U0_NEO_4__ID: delete (static_cast<NEOPIXELBUS_8266_U0_NEO_4*>(busPtr)); break;
      case BUSTYPE__8266_U1_NEO_4__ID: delete (static_cast<NEOPIXELBUS_8266_U1_NEO_4*>(busPtr)); break;
      case BUSTYPE__8266_DM_NEO_4__ID: delete (static_cast<NEOPIXELBUS_8266_DM_NEO_4*>(busPtr)); break;
      case BUSTYPE__8266_BB_NEO_4__ID: delete (static_cast<NEOPIXELBUS_8266_BB_NEO_4*>(busPtr)); break;
      case BUSTYPE__8266_U0_400_3__ID: delete (static_cast<NEOPIXELBUS_8266_U0_400_3*>(busPtr)); break;
      case BUSTYPE__8266_U1_400_3__ID: delete (static_cast<NEOPIXELBUS_8266_U1_400_3*>(busPtr)); break;
      case BUSTYPE__8266_DM_400_3__ID: delete (static_cast<NEOPIXELBUS_8266_DM_400_3*>(busPtr)); break;
      case BUSTYPE__8266_BB_400_3__ID: delete (static_cast<NEOPIXELBUS_8266_BB_400_3*>(busPtr)); break;
      // case BUSTYPE__8266_U0_TM1_4__ID: delete (static_cast<NEOPIXELBUS_8266_U0_TM1_4*>(busPtr)); break;
      // case BUSTYPE__8266_U1_TM1_4__ID: delete (static_cast<NEOPIXELBUS_8266_U1_TM1_4*>(busPtr)); break;
      // case BUSTYPE__8266_DM_TM1_4__ID: delete (static_cast<NEOPIXELBUS_8266_DM_TM1_4*>(busPtr)); break;
      // case BUSTYPE__8266_BB_TM1_4__ID: delete (static_cast<NEOPIXELBUS_8266_BB_TM1_4*>(busPtr)); break;
      // case BUSTYPE__8266_U0_TM2_3__ID: delete (static_cast<NEOPIXELBUS_8266_U0_TM2_4*>(busPtr)); break;
      // case BUSTYPE__8266_U1_TM2_3__ID: delete (static_cast<NEOPIXELBUS_8266_U1_TM2_4*>(busPtr)); break;
      // case BUSTYPE__8266_DM_TM2_3__ID: delete (static_cast<NEOPIXELBUS_8266_DM_TM2_4*>(busPtr)); break;
      // case BUSTYPE__8266_BB_TM2_3__ID: delete (static_cast<NEOPIXELBUS_8266_BB_TM2_4*>(busPtr)); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32
      case BUSTYPE__32_RN_NEO_3__ID: delete (static_cast<NEOPIXELBUS_32_RN_NEO_3*>(busPtr)); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_3__ID: delete (static_cast<NEOPIXELBUS_32_I0_NEO_3*>(busPtr)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_3__ID: delete (static_cast<NEOPIXELBUS_32_I1_NEO_3*>(busPtr)); break;
      #endif
      case BUSTYPE__32_RN_NEO_4__ID: delete (static_cast<NEOPIXELBUS_32_RN_NEO_4*>(busPtr)); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_4__ID: delete (static_cast<NEOPIXELBUS_32_I0_NEO_4*>(busPtr)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_4__ID: delete (static_cast<NEOPIXELBUS_32_I1_NEO_4*>(busPtr)); break;
      #endif
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
      case BUSTYPE__HS_DOT_3__ID: delete (static_cast<NEOPIXELBUS_HS_DOT_3*>(busPtr)); break;
      case BUSTYPE__SS_DOT_3__ID: delete (static_cast<NEOPIXELBUS_SS_DOT_3*>(busPtr)); break;
      // case BUSTYPE__HS_LPD_3__ID: delete (static_cast<NEOPIXELBUS_HS_LPD_3*>(busPtr)); break;
      // case BUSTYPE__SS_LPD_3__ID: delete (static_cast<NEOPIXELBUS_SS_LPD_3*>(busPtr)); break;
      case BUSTYPE__HS_LPO_3__ID: delete (static_cast<NEOPIXELBUS_HS_LPO_3*>(busPtr)); break;
      case BUSTYPE__SS_LPO_3__ID: delete (static_cast<NEOPIXELBUS_SS_LPO_3*>(busPtr)); break;
      // case BUSTYPE__HS_WS1_3__ID: delete (static_cast<NEOPIXELBUS_HS_WS1_3*>(busPtr)); break;
      // case BUSTYPE__SS_WS1_3__ID: delete (static_cast<NEOPIXELBUS_SS_WS1_3*>(busPtr)); break;
      // case BUSTYPE__HS_P98_3__ID: delete (static_cast<NEOPIXELBUS_HS_P98_3*>(busPtr)); break;
      // case BUSTYPE__SS_P98_3__ID: delete (static_cast<NEOPIXELBUS_SS_P98_3*>(busPtr)); break;
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
        default: t=BUSTYPE__NONE__ID;
      }
      if (t > BUSTYPE__NONE__ID && isHSPI) t--; //hardware SPI has one smaller ID than software
      return t;
    } else {
      #ifdef ESP8266
      uint8_t offset = pins[0] -1; //for driver: 0 = uart0, 1 = uart1, 2 = dma, 3 = bitbang
      if (offset > 3) offset = 3;
      switch (busType) {
        case BUSTYPE_WS2812_RGB:
        case BUSTYPE_WS2812_WWA:
          return BUSTYPE__8266_U0_NEO_3__ID + offset;
        case BUSTYPE_SK6812_RGBW:
          return BUSTYPE__8266_U0_NEO_4__ID + offset;
        case BUSTYPE_WS2811_400KHZ:
          return BUSTYPE__8266_U0_400_3__ID + offset;
        case BUSTYPE_TM1814:
          return BUSTYPE__8266_U0_TM1_4__ID + offset;
        case BUSTYPE_TM1829:
          return BUSTYPE__8266_U0_TM2_3__ID + offset;
      }
      #else //ESP32
      uint8_t offset = 0; //0 = RMT (num 0-7) 8 = I2S0 9 = I2S1
      #if defined(CONFIG_IDF_TARGET_ESP32S2)
      // ESP32-S2 only has 4 RMT channels
      if (num > 4) return I_NONE;
      if (num > 3) offset = 1;  // only one I2S
      #elif defined(CONFIG_IDF_TARGET_ESP32C3)
      // On ESP32-C3 only the first 2 RMT channels are usable for transmitting
      if (num > 1) return I_NONE;
      //if (num > 1) offset = 1; // I2S not supported yet (only 1 I2S)
      #elif defined(CONFIG_IDF_TARGET_ESP32S3)
      // On ESP32-S3 only the first 4 RMT channels are usable for transmitting
      if (num > 3) return I_NONE;
      //if (num > 3) offset = num -4; // I2S not supported yet
      #else
        
        #ifdef ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD
        // I2S0 and I2S1 are 0 and 1
        // RMT0 to 7 as 2 to 9
        if(num < 2)
        {
          offset = num + 1; // to skip that RMT was entered first in enum
        }
        else
        if(num < 9)
        {
          offset = num - 7;
        }
        else
        {
          return BUSTYPE__NONE__ID;
        }
        #else // ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD
        // standard ESP32 has 8 RMT and 2 I2S channels
        if (num > 9) return BUSTYPE__NONE__ID;
        if (num > 7) offset = num -7;
        #endif // ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD

      #endif
      switch (busType) {
        case BUSTYPE_WS2812_RGB:
        case BUSTYPE_WS2812_WWA:
          return BUSTYPE__32_RN_NEO_3__ID + offset;
        case BUSTYPE_SK6812_RGBW:
          return BUSTYPE__32_RN_NEO_4__ID + offset;
        case BUSTYPE_WS2811_400KHZ:
          return BUSTYPE__32_RN_400_3__ID + offset;
        case BUSTYPE_TM1814:
          return BUSTYPE__32_RN_TM1_4__ID + offset;
        case BUSTYPE_TM1829:
          return BUSTYPE__32_RN_TM2_3__ID + offset;
      }
      #endif
    }
    return BUSTYPE__NONE__ID;
  }
};

#endif // USE_MODULE_LIGHTS_ANIMATOR

#endif // guard
