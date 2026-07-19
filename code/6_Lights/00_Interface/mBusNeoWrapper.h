#ifndef BusWrapper_h
#define BusWrapper_h

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR


#ifdef ENABLE_DEVFEATURE_NEOBUS__RMT_AS_PRIMARY

#undef ENABLE_FEATURE_LIGHTING__I2S_SINGLE_AND_PARALLEL_AUTO_DETECT


#endif



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
// #define P_8266_HS_MOSI 13
// #define P_8266_HS_CLK  14
// #define P_32_HS_MOSI   13
// #define P_32_HS_CLK    14
// #define P_32_VS_MOSI   23
// #define P_32_VS_CLK    18


// Define masks for the RGB and white channel bits
#define COLOUR_ORDER_RGB_MASK 0x0F // Lower 4 bits for RGB swapping
#define COLOUR_ORDER_WHITE_MASK 0xF0 // Upper 4 bits for white channel swapping


enum EM_BUS_TYPE
{
  BUSTYPE__NONE__ID = 0,  
  /*************************** ESP8266 Neopixel methods ***/
  // RGB
  BUSTYPE__8266_U0_3__ID = 1,
  BUSTYPE__8266_U1_3__ID = 2,
  BUSTYPE__8266_DM_3__ID = 3,
  // RGBW
  BUSTYPE__8266_U0_4__ID = 5,
  BUSTYPE__8266_U1_4__ID = 6,
  BUSTYPE__8266_DM_4__ID = 7,  
  /**************************** ESP32 Neopixel methods ***/
  // RGB
  BUSTYPE__32_RN_3__ID = 23,
  BUSTYPE__32_I0_3__ID,
  BUSTYPE__32_I1_3__ID,
  BUSTYPE__32_I1_3P__ID,
  BUSTYPE__32_I0_3P__ID,
  // RGBW
  BUSTYPE__32_RN_4__ID,
  BUSTYPE__32_I0_4__ID,
  BUSTYPE__32_I1_4__ID,
  BUSTYPE__32_I1_4P__ID,
  BUSTYPE__32_I0_4P__ID,
  // RGBWW
  BUSTYPE__32_RN_5__ID, 
  BUSTYPE__32_I0_5__ID,
  BUSTYPE__32_I1_5__ID,
  BUSTYPE__32_I1_5P__ID,
  BUSTYPE__32_I0_5P__ID,
  // 400Kbps
  BUSTYPE__32_RN_400_3__ID,
  BUSTYPE__32_I0_400_3__ID,
  BUSTYPE__32_I1_400_3__ID,
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


#define ENABLE_PIXELBUS_8266_U0_TYPES
#define ENABLE_PIXELBUS_8266_U1_TYPES
#define ENABLE_PIXELBUS_8266_DM_TYPES

#include "NeoPixelBus.h"


/*** ESP8266 Neopixel methods ***/
#ifdef ESP8266
//RGB
#ifdef ENABLE_PIXELBUS_8266_U0_TYPES
#define PIXELBUS_8266_U0_3 NeoPixelBus<NeoRgbFeature, NeoEsp8266Uart0Ws2813Method> //3 chan, esp8266, gpio1
#endif
#ifdef ENABLE_PIXELBUS_8266_U1_TYPES
#define PIXELBUS_8266_U1_3 NeoPixelBus<NeoRgbFeature, NeoEsp8266Uart1Ws2813Method> //3 chan, esp8266, gpio2
#endif 
#ifdef ENABLE_PIXELBUS_8266_DM_TYPES
#define PIXELBUS_8266_DM_3 NeoPixelBus<NeoRgbFeature, NeoEsp8266Dma800KbpsMethod>  //3 chan, esp8266, gpio3
#endif
// #define PIXELBUS_8266_BB_3 NeoPixelBus<NeoRgbFeature, NeoEsp8266BitBang800KbpsMethod> //3 chan, esp8266, bb (any pin but 16)
//RGBW
#ifdef ENABLE_PIXELBUS_8266_U0_TYPES
#define PIXELBUS_8266_U0_4 NeoPixelBus<NeoRgbwFeature, NeoEsp8266Uart0Ws2813Method>   //4 chan, esp8266, gpio1
#endif 
#ifdef ENABLE_PIXELBUS_8266_U1_TYPES
#define PIXELBUS_8266_U1_4 NeoPixelBus<NeoRgbwFeature, NeoEsp8266Uart1Ws2813Method>   //4 chan, esp8266, gpio2
#endif
#ifdef ENABLE_PIXELBUS_8266_DM_TYPES
#define PIXELBUS_8266_DM_4 NeoPixelBus<NeoRgbwFeature, NeoEsp8266Dma800KbpsMethod>    //4 chan, esp8266, gpio3
#endif
#endif

// /*** ESP32 Neopixel methods ***/
// #ifdef ARDUINO_ARCH_ESP32

//   /**********************************************************************
//    * I2S typedef selection (keep this exactly aligned with NeoPixelBus)
//    **********************************************************************/
//   #if defined(CONFIG_IDF_TARGET_ESP32S3)
//     // S3 uses LCD-style parallel types
//     typedef X8Ws2812xMethod   X1Ws2812xMethod;
//     typedef X8Sk6812Method    X1Sk6812Method;
//     typedef X8400KbpsMethod   X1400KbpsMethod;
//     typedef X8800KbpsMethod   X1800KbpsMethod;
//     typedef X8Tm1814Method    X1Tm1814Method;
//     typedef X8Tm1829Method    X1Tm1829Method;
//     typedef X8Apa106Method    X1Apa106Method;
//     typedef X8Ws2805Method    X1Ws2805Method;
//     typedef X8Tm1914Method    X1Tm1914Method;

//   #elif defined(CONFIG_IDF_TARGET_ESP32S2)
//     // S2 uses I2S0
//     typedef NeoEsp32I2s0Ws2812xMethod X1Ws2812xMethod;
//     typedef NeoEsp32I2s0Sk6812Method  X1Sk6812Method;
//     typedef NeoEsp32I2s0400KbpsMethod X1400KbpsMethod;
//     typedef NeoEsp32I2s0800KbpsMethod X1800KbpsMethod;
//     typedef NeoEsp32I2s0Tm1814Method  X1Tm1814Method;
//     typedef NeoEsp32I2s0Tm1829Method  X1Tm1829Method;
//     typedef NeoEsp32I2s0Apa106Method  X1Apa106Method;
//     typedef NeoEsp32I2s0Ws2805Method  X1Ws2805Method;
//     typedef NeoEsp32I2s0Tm1914Method  X1Tm1914Method;

//   #elif !defined(CONFIG_IDF_TARGET_ESP32C3)
//     // Classic ESP32 uses I2S1
//     typedef NeoEsp32I2s1Ws2812xMethod X1Ws2812xMethod;
//     typedef NeoEsp32I2s1Sk6812Method  X1Sk6812Method;
//     typedef NeoEsp32I2s1400KbpsMethod X1400KbpsMethod;
//     typedef NeoEsp32I2s1800KbpsMethod X1800KbpsMethod;
//     typedef NeoEsp32I2s1Tm1814Method  X1Tm1814Method;
//     typedef NeoEsp32I2s1Tm1829Method  X1Tm1829Method;
//     typedef NeoEsp32I2s1Apa106Method  X1Apa106Method;
//     typedef NeoEsp32I2s1Ws2805Method  X1Ws2805Method;
//     typedef NeoEsp32I2s1Tm1914Method  X1Tm1914Method;
//   #endif


//   /**********************************************************************
//    * RMT method selection (WLED-style)
//    *
//    * You are adding NeoEsp32RmtHI locally.
//    * - Xtensa (ESP32/S2/S3): prefer HI unless WLED_USE_SHARED_RMT is defined
//    * - RISC-V (C3): use N methods (HI generally not used there)
//    **********************************************************************/
//   #if !defined(__riscv) && !defined(WLED_USE_SHARED_RMT)
//     #include <NeoEsp32RmtHIMethod.h>
//     #define NEOPIXELBUS_ESP32_RMT_METHOD(x) NeoEsp32RmtHIN ## x ## Method
//   #else
//     // fallback to the standard NeoPixelBus "N" channel-capable RMT methods
//     #define NEOPIXELBUS_ESP32_RMT_METHOD(x) NeoEsp32RmtN ## x ## Method
//   #endif


//   /**********************************************************************
//    * BUS DEFINITIONS
//    *
//    * IMPORTANT:
//    * - RN_3 / RN_4 / RN_400_3 are RMT-capable and accept (len,pin,NeoBusChannel)
//    * - RN_5 (WS2805 RGBWW) is NOT RMT; keep it I2S-only and DO NOT pass channel
//    * - For targets where you disable I2S, we alias I0/I1/*P to RN so code compiles.
//    **********************************************************************/

//   // ---------- Common RGB / RGBW ----------
//   #define PIXELBUS_32_RN_3       NeoPixelBus<NeoRgbFeature,  NEOPIXELBUS_ESP32_RMT_METHOD(Ws2812x)>
//   #define PIXELBUS_32_RN_4       NeoPixelBus<NeoRgbwFeature, NEOPIXELBUS_ESP32_RMT_METHOD(Sk6812)>

//   // 400K (if enabled)
//   #ifndef NEOPIXEL_DISABLE_400_PIXELBUS
//     #define PIXELBUS_32_RN_400_3 NeoPixelBus<NeoRgbFeature,  NEOPIXELBUS_ESP32_RMT_METHOD(400Kbps)>
//   #endif

//   // ---------- RGBWW (WS2805) ----------
//   // No RMT method. Keep it as I2S based on target’s X1Ws2805Method typedef.
//   #define PIXELBUS_32_RN_5       NeoPixelBus<NeoRgbwwFeature, X1Ws2805Method>


//   // ---------- Target mapping ----------
//   #if defined(CONFIG_IDF_TARGET_ESP32C3)
//     // C3: your policy = RMT only (no I2S WS2812 paths)
//     #define NEOPIXEL_DISABLE_400_PIXELBUS

//     // map "I*" names so the rest of your wrapper compiles
//     #define PIXELBUS_32_I0_3   PIXELBUS_32_RN_3
//     #define PIXELBUS_32_I1_3   PIXELBUS_32_RN_3
//     #define PIXELBUS_32_I0_3P  PIXELBUS_32_RN_3
//     #define PIXELBUS_32_I1_3P  PIXELBUS_32_RN_3

//     #define PIXELBUS_32_I0_4   PIXELBUS_32_RN_4
//     #define PIXELBUS_32_I1_4   PIXELBUS_32_RN_4
//     #define PIXELBUS_32_I0_4P  PIXELBUS_32_RN_4
//     #define PIXELBUS_32_I1_4P  PIXELBUS_32_RN_4

//     // RGBWW aliases (still compiles, but is I2S-type; if you truly don't support it on C3, leave it unused)
//     #define PIXELBUS_32_I0_5   PIXELBUS_32_RN_5
//     #define PIXELBUS_32_I1_5   PIXELBUS_32_RN_5
//     #define PIXELBUS_32_I0_5P  PIXELBUS_32_RN_5
//     #define PIXELBUS_32_I1_5P  PIXELBUS_32_RN_5

//     // 400K aliases are disabled above

//   #elif defined(CONFIG_IDF_TARGET_ESP32S3)
//     // S3: keep I2S disabled if you already do that globally; otherwise map I* to X1/X8
//     #define PIXELBUS_32_I0_3   NeoPixelBus<NeoRgbFeature,  X1Ws2812xMethod>
//     #define PIXELBUS_32_I1_3   NeoPixelBus<NeoRgbFeature,  X1Ws2812xMethod>
//     #define PIXELBUS_32_I0_3P  NeoPixelBus<NeoRgbFeature,  X8Ws2812xMethod>
//     #define PIXELBUS_32_I1_3P  NeoPixelBus<NeoRgbFeature,  X8Ws2812xMethod>

//     #define PIXELBUS_32_I0_4   NeoPixelBus<NeoRgbwFeature, X1Sk6812Method>
//     #define PIXELBUS_32_I1_4   NeoPixelBus<NeoRgbwFeature, X1Sk6812Method>
//     #define PIXELBUS_32_I0_4P  NeoPixelBus<NeoRgbwFeature, X8Sk6812Method>
//     #define PIXELBUS_32_I1_4P  NeoPixelBus<NeoRgbwFeature, X8Sk6812Method>

//     #define PIXELBUS_32_I0_5   NeoPixelBus<NeoRgbwwFeature, X1Ws2805Method>
//     #define PIXELBUS_32_I1_5   NeoPixelBus<NeoRgbwwFeature, X1Ws2805Method>
//     #define PIXELBUS_32_I0_5P  NeoPixelBus<NeoRgbwwFeature, X8Ws2805Method>
//     #define PIXELBUS_32_I1_5P  NeoPixelBus<NeoRgbwwFeature, X8Ws2805Method>

//     #ifndef NEOPIXEL_DISABLE_400_PIXELBUS
//       #define PIXELBUS_32_I0_400_3 NeoPixelBus<NeoRgbFeature, X1400KbpsMethod>
//       #define PIXELBUS_32_I1_400_3 NeoPixelBus<NeoRgbFeature, X1400KbpsMethod>
//     #endif

//   #elif defined(CONFIG_IDF_TARGET_ESP32S2)
//     // S2: I2S0 only, map I1->I0 for compatibility
//     #define PIXELBUS_32_I0_3   NeoPixelBus<NeoRgbFeature,  X1Ws2812xMethod>
//     #define PIXELBUS_32_I1_3   PIXELBUS_32_I0_3
//     #define PIXELBUS_32_I0_3P  PIXELBUS_32_I0_3
//     #define PIXELBUS_32_I1_3P  PIXELBUS_32_I0_3

//     #define PIXELBUS_32_I0_4   NeoPixelBus<NeoRgbwFeature, X1Sk6812Method>
//     #define PIXELBUS_32_I1_4   PIXELBUS_32_I0_4
//     #define PIXELBUS_32_I0_4P  PIXELBUS_32_I0_4
//     #define PIXELBUS_32_I1_4P  PIXELBUS_32_I0_4

//     #define PIXELBUS_32_I0_5   NeoPixelBus<NeoRgbwwFeature, X1Ws2805Method>
//     #define PIXELBUS_32_I1_5   PIXELBUS_32_I0_5
//     #define PIXELBUS_32_I0_5P  PIXELBUS_32_I0_5
//     #define PIXELBUS_32_I1_5P  PIXELBUS_32_I0_5

//     #ifndef NEOPIXEL_DISABLE_400_PIXELBUS
//       #define PIXELBUS_32_I0_400_3 NeoPixelBus<NeoRgbFeature, X1400KbpsMethod>
//       #define PIXELBUS_32_I1_400_3 PIXELBUS_32_I0_400_3
//     #endif

//   #else
//     // Classic ESP32: keep your old scheme (I0/I1 + parallel)
//     #define PIXELBUS_32_I0_3   NeoPixelBus<NeoRgbFeature,  NeoEsp32I2s0Sk6812Method>
//     #define PIXELBUS_32_I1_3   NeoPixelBus<NeoRgbFeature,  X1Ws2812xMethod>
//     #define PIXELBUS_32_I0_3P  NeoPixelBus<NeoRgbFeature,  X8Ws2812xMethod>
//     #define PIXELBUS_32_I1_3P  NeoPixelBus<NeoRgbFeature,  X8Ws2812xMethod>

//     #define PIXELBUS_32_I0_4   NeoPixelBus<NeoRgbwFeature, NeoEsp32I2s0Sk6812Method>
//     #define PIXELBUS_32_I1_4   NeoPixelBus<NeoRgbwFeature, X1Sk6812Method>
//     #define PIXELBUS_32_I0_4P  NeoPixelBus<NeoRgbwFeature, X8Sk6812Method>
//     #define PIXELBUS_32_I1_4P  NeoPixelBus<NeoRgbwFeature, X8Sk6812Method>

//     #define PIXELBUS_32_I0_5   NeoPixelBus<NeoRgbwwFeature, X1Ws2805Method>
//     #define PIXELBUS_32_I1_5   NeoPixelBus<NeoRgbwwFeature, X1Ws2805Method>
//     #define PIXELBUS_32_I0_5P  NeoPixelBus<NeoRgbwwFeature, X8Ws2805Method>
//     #define PIXELBUS_32_I1_5P  NeoPixelBus<NeoRgbwwFeature, X8Ws2805Method>

//     #ifndef NEOPIXEL_DISABLE_400_PIXELBUS
//       #define PIXELBUS_32_I0_400_3 NeoPixelBus<NeoRgbFeature, NeoEsp32I2s0400KbpsMethod>
//       #define PIXELBUS_32_I1_400_3 NeoPixelBus<NeoRgbFeature, NeoEsp32I2s1400KbpsMethod>
//     #endif

//   #endif // target select

// #endif // ARDUINO_ARCH_ESP32

// #ifdef ARDUINO_ARCH_ESP32
//   #pragma message "ARDUINO_ARCH_ESP32 is defined"
// #endif

// #ifdef CONFIG_IDF_TARGET_ESP32C3
//   #pragma message "CONFIG_IDF_TARGET_ESP32C3 is defined"
// #endif
// #ifdef CONFIG_IDF_TARGET_ESP32
//   #pragma message "CONFIG_IDF_TARGET_ESP32 is defined"
// #endif
// #ifdef CONFIG_IDF_TARGET_ESP32S3
//   #pragma message "CONFIG_IDF_TARGET_ESP32S3 is defined"
// #endif
// #ifdef CONFIG_IDF_TARGET_ESP32S2
//   #pragma message "CONFIG_IDF_TARGET_ESP32S2 is defined"
// #endif

// #ifdef ARDUINO_ESP32C3_DEV
//   #pragma message "ARDUINO_ESP32C3_DEV is defined"
// #endif
// #ifdef ESP32C3
//   #pragma message "ESP32C3 is defined"
// #endif


/* =================================================================================================
 * ESP32 family quick-reference (NeoPixelBus / NeoPixelBus-RMT-HI planning)
 *
 * Source for I2S + RMT peripheral counts: ESP-IDF Chip Series Comparison (v5.0). :contentReference[oaicite:0]{index=0}
 *
 * Notes:
 * - “I2S count” is the number of I2S peripherals on the SoC (not “works for NeoPixelBus I2S method”).
 * - “RMT” is the SoC peripheral count; some targets split TX/RX (C3/S3/S2) per Espressif table.
 * - “HI Method” here means your NeoEsp32RmtHI add-on (interrupt/ISR-driven) *availability by arch*.
 *   In WLED, HI is *disabled* on __riscv (C3), so C3 defaults to “N” methods.
 * - “Mini form factor” = commonly available tiny modules/boards in the ecosystem (subjective, not SoC).
 * =================================================================================================
 *
 * | Chip Series     | Arch / Core            | I2S peripherals | RMT peripheral (total / TX / RX)         | HI Method usable? | Mini modules common? |
 * |----------------|-------------------------|-----------------|------------------------------------------|-------------------|----------------------|
 * | ESP32 (classic) | Xtensa LX6 (dual/single)| 2               | 8 channels (TX/RX flexible)              | YES (Xtensa)      | YES                  |
 * | ESP32-S2        | Xtensa LX7 (single)     | 1               | 4 channels (TX/RX configurable)          | YES (Xtensa)      | SOME                 |
 * | ESP32-C3        | RISC-V (single)         | 1               | 4 channels (2 TX, 2 RX)                  | NO* (WLED disables)| YES (SuperMini etc.) |
 * | ESP32-S3        | Xtensa LX7 (dual)       | 2               | 8 channels (4 TX, 4 RX)                  | YES (Xtensa)      | YES (many “mini”)    |
 *
 * *C3 note: your flicker problem is on RMT “N” paths; WLED chooses N on __riscv, not HI.
 * ================================================================================================= */

#ifdef ARDUINO_ARCH_ESP32

  #include <sdkconfig.h>

  /**********************************************************************
   * I2S typedef selection (keep aligned with NeoPixelBus internals)
   **********************************************************************/
  #if defined(CONFIG_IDF_TARGET_ESP32S3)
    // S3 uses LCD-style parallel types
    typedef X8Ws2812xMethod   X1Ws2812xMethod;
    typedef X8Sk6812Method    X1Sk6812Method;
    typedef X8400KbpsMethod   X1400KbpsMethod;
    typedef X8800KbpsMethod   X1800KbpsMethod;
    typedef X8Tm1814Method    X1Tm1814Method;
    typedef X8Tm1829Method    X1Tm1829Method;
    typedef X8Apa106Method    X1Apa106Method;
    typedef X8Ws2805Method    X1Ws2805Method;
    typedef X8Tm1914Method    X1Tm1914Method;

  #elif defined(CONFIG_IDF_TARGET_ESP32S2)
    // S2 uses I2S0
    typedef NeoEsp32I2s0Ws2812xMethod X1Ws2812xMethod;
    typedef NeoEsp32I2s0Sk6812Method  X1Sk6812Method;
    typedef NeoEsp32I2s0400KbpsMethod X1400KbpsMethod;
    typedef NeoEsp32I2s0800KbpsMethod X1800KbpsMethod;
    typedef NeoEsp32I2s0Tm1814Method  X1Tm1814Method;
    typedef NeoEsp32I2s0Tm1829Method  X1Tm1829Method;
    typedef NeoEsp32I2s0Apa106Method  X1Apa106Method;
    typedef NeoEsp32I2s0Ws2805Method  X1Ws2805Method;
    typedef NeoEsp32I2s0Tm1914Method  X1Tm1914Method;

  #elif defined(CONFIG_IDF_TARGET_ESP32C3)
    // C3: no NeoPixelBus I2S typedefs used here (RMT-only in your policy)

  #else
    // Classic ESP32 uses I2S1
    typedef NeoEsp32I2s1Ws2812xMethod X1Ws2812xMethod;
    typedef NeoEsp32I2s1Sk6812Method  X1Sk6812Method;
    typedef NeoEsp32I2s1400KbpsMethod X1400KbpsMethod;
    typedef NeoEsp32I2s1800KbpsMethod X1800KbpsMethod;
    typedef NeoEsp32I2s1Tm1814Method  X1Tm1814Method;
    typedef NeoEsp32I2s1Tm1829Method  X1Tm1829Method;
    typedef NeoEsp32I2s1Apa106Method  X1Apa106Method;
    typedef NeoEsp32I2s1Ws2805Method  X1Ws2805Method;
    typedef NeoEsp32I2s1Tm1914Method  X1Tm1914Method;
  #endif


  /**********************************************************************
   * RMT method selection + BUS DEFINITIONS (explicit per target)
   **********************************************************************/

  // ====================================================================
  // ESP32-C3 (RISC-V) - NO I2S, RMT Only
  // ====================================================================
  #if defined(CONFIG_IDF_TARGET_ESP32C3)

    // Your policy: RMT-only on C3
    #define NEOPIXEL_DISABLE_400_PIXELBUS

    // C3: use upstream "N" RMT methods
    #define NEOPIXELBUS_ESP32_RMT_METHOD(x) NeoEsp32RmtN ## x ## Method

    // ---------- RGB / RGBW ----------
    #define PIXELBUS_32_RN_3  NeoPixelBus<NeoRgbFeature,  NEOPIXELBUS_ESP32_RMT_METHOD(Ws2812x)>
    #define PIXELBUS_32_RN_4  NeoPixelBus<NeoRgbwFeature, NEOPIXELBUS_ESP32_RMT_METHOD(Sk6812)>

    // ---------- 400K ----------
    // disabled above

    // ---------- RGBWW (WS2805) ----------
    #define PIXELBUS_32_RN_5  NeoPixelBus<NeoRgbwwFeature, NeoWs2812xMethod>  // TEMP: RGBWW placeholder on C3 (not true WS2805 timing). You will change later.

    // Map I2S names to RMT so the rest of the wrapper compiles
    #define PIXELBUS_32_I0_3   PIXELBUS_32_RN_3
    #define PIXELBUS_32_I1_3   PIXELBUS_32_RN_3
    #define PIXELBUS_32_I0_3P  PIXELBUS_32_RN_3
    #define PIXELBUS_32_I1_3P  PIXELBUS_32_RN_3

    #define PIXELBUS_32_I0_4   PIXELBUS_32_RN_4
    #define PIXELBUS_32_I1_4   PIXELBUS_32_RN_4
    #define PIXELBUS_32_I0_4P  PIXELBUS_32_RN_4
    #define PIXELBUS_32_I1_4P  PIXELBUS_32_RN_4

    #define PIXELBUS_32_I0_5   PIXELBUS_32_RN_5
    #define PIXELBUS_32_I1_5   PIXELBUS_32_RN_5
    #define PIXELBUS_32_I0_5P  PIXELBUS_32_RN_5
    #define PIXELBUS_32_I1_5P  PIXELBUS_32_RN_5


  // ====================================================================
  // ESP32-S3 (Xtensa)
  // ====================================================================
  #elif defined(CONFIG_IDF_TARGET_ESP32S3)

    // Keep EXACT HI-selection logic (as requested)
    #if !defined(__riscv) && !defined(WLED_USE_SHARED_RMT)
      #include <NeoEsp32RmtHIMethod.h>
      #define NEOPIXELBUS_ESP32_RMT_METHOD(x) NeoEsp32RmtHIN ## x ## Method
    #else
      #define NEOPIXELBUS_ESP32_RMT_METHOD(x) NeoEsp32RmtN ## x ## Method
    #endif

    // ---------- RGB / RGBW ----------
    #define PIXELBUS_32_RN_3  NeoPixelBus<NeoRgbFeature,  NEOPIXELBUS_ESP32_RMT_METHOD(Ws2812x)>
    #define PIXELBUS_32_RN_4  NeoPixelBus<NeoRgbwFeature, NEOPIXELBUS_ESP32_RMT_METHOD(Sk6812)>

    // ---------- 400K ----------
    #ifndef NEOPIXEL_DISABLE_400_PIXELBUS
      #define PIXELBUS_32_RN_400_3 NeoPixelBus<NeoRgbFeature, NEOPIXELBUS_ESP32_RMT_METHOD(400Kbps)>
    #endif

    // ---------- RGBWW (WS2805) ----------
    #define PIXELBUS_32_RN_5  NeoPixelBus<NeoRgbwwFeature, NEOPIXELBUS_ESP32_RMT_METHOD(Ws2805)>

    // I2S mappings (single + parallel)
    #define PIXELBUS_32_I0_3   NeoPixelBus<NeoRgbFeature,  X1Ws2812xMethod>
    #define PIXELBUS_32_I1_3   NeoPixelBus<NeoRgbFeature,  X1Ws2812xMethod>
    #define PIXELBUS_32_I0_3P  NeoPixelBus<NeoRgbFeature,  X8Ws2812xMethod>
    #define PIXELBUS_32_I1_3P  NeoPixelBus<NeoRgbFeature,  X8Ws2812xMethod>

    #define PIXELBUS_32_I0_4   NeoPixelBus<NeoRgbwFeature, X1Sk6812Method>
    #define PIXELBUS_32_I1_4   NeoPixelBus<NeoRgbwFeature, X1Sk6812Method>
    #define PIXELBUS_32_I0_4P  NeoPixelBus<NeoRgbwFeature, X8Sk6812Method>
    #define PIXELBUS_32_I1_4P  NeoPixelBus<NeoRgbwFeature, X8Sk6812Method>

    #define PIXELBUS_32_I0_5   NeoPixelBus<NeoRgbwwFeature, X1Ws2805Method>
    #define PIXELBUS_32_I1_5   NeoPixelBus<NeoRgbwwFeature, X1Ws2805Method>
    #define PIXELBUS_32_I0_5P  NeoPixelBus<NeoRgbwwFeature, X8Ws2805Method>
    #define PIXELBUS_32_I1_5P  NeoPixelBus<NeoRgbwwFeature, X8Ws2805Method>

    #ifndef NEOPIXEL_DISABLE_400_PIXELBUS
      #define PIXELBUS_32_I0_400_3 NeoPixelBus<NeoRgbFeature, X1400KbpsMethod>
      #define PIXELBUS_32_I1_400_3 NeoPixelBus<NeoRgbFeature, X1400KbpsMethod>
    #endif


  // ====================================================================
  // ESP32-S2 (Xtensa)
  // ====================================================================
  #elif defined(CONFIG_IDF_TARGET_ESP32S2)

    // Keep EXACT HI-selection logic (as requested)
    #if !defined(__riscv) && !defined(WLED_USE_SHARED_RMT)
      #include <NeoEsp32RmtHIMethod.h>
      #define NEOPIXELBUS_ESP32_RMT_METHOD(x) NeoEsp32RmtHIN ## x ## Method
    #else
      #define NEOPIXELBUS_ESP32_RMT_METHOD(x) NeoEsp32RmtN ## x ## Method
    #endif

    // ---------- RGB / RGBW ----------
    #define PIXELBUS_32_RN_3  NeoPixelBus<NeoRgbFeature,  NEOPIXELBUS_ESP32_RMT_METHOD(Ws2812x)>
    #define PIXELBUS_32_RN_4  NeoPixelBus<NeoRgbwFeature, NEOPIXELBUS_ESP32_RMT_METHOD(Sk6812)>

    // ---------- 400K ----------
    #ifndef NEOPIXEL_DISABLE_400_PIXELBUS
      #define PIXELBUS_32_RN_400_3 NeoPixelBus<NeoRgbFeature, NEOPIXELBUS_ESP32_RMT_METHOD(400Kbps)>
    #endif

    // ---------- RGBWW (WS2805) ----------
    #define PIXELBUS_32_RN_5  NeoPixelBus<NeoRgbwwFeature, X1Ws2805Method>  // WS2805 uses I2S0, not RMT

    // I2S0 only; map I1 -> I0
    #define PIXELBUS_32_I0_3   NeoPixelBus<NeoRgbFeature,  X1Ws2812xMethod>
    #define PIXELBUS_32_I1_3   PIXELBUS_32_I0_3
    #define PIXELBUS_32_I0_3P  PIXELBUS_32_I0_3
    #define PIXELBUS_32_I1_3P  PIXELBUS_32_I0_3

    #define PIXELBUS_32_I0_4   NeoPixelBus<NeoRgbwFeature, X1Sk6812Method>
    #define PIXELBUS_32_I1_4   PIXELBUS_32_I0_4
    #define PIXELBUS_32_I0_4P  PIXELBUS_32_I0_4
    #define PIXELBUS_32_I1_4P  PIXELBUS_32_I0_4

    #define PIXELBUS_32_I0_5   NeoPixelBus<NeoRgbwwFeature, X1Ws2805Method>
    #define PIXELBUS_32_I1_5   PIXELBUS_32_I0_5
    #define PIXELBUS_32_I0_5P  PIXELBUS_32_I0_5
    #define PIXELBUS_32_I1_5P  PIXELBUS_32_I0_5

    #ifndef NEOPIXEL_DISABLE_400_PIXELBUS
      #define PIXELBUS_32_I0_400_3 NeoPixelBus<NeoRgbFeature, X1400KbpsMethod>
      #define PIXELBUS_32_I1_400_3 PIXELBUS_32_I0_400_3
    #endif


  // ====================================================================
  // Classic ESP32 (Xtensa)
  // ====================================================================
  #else

    // Keep EXACT HI-selection logic (as requested)
    #if !defined(__riscv) && !defined(WLED_USE_SHARED_RMT)
      #include <NeoEsp32RmtHIMethod.h>
      #define NEOPIXELBUS_ESP32_RMT_METHOD(x) NeoEsp32RmtHIN ## x ## Method
    #else
      #define NEOPIXELBUS_ESP32_RMT_METHOD(x) NeoEsp32RmtN ## x ## Method
    #endif

    // ---------- RGB / RGBW ----------
    #define PIXELBUS_32_RN_3  NeoPixelBus<NeoRgbFeature,  NEOPIXELBUS_ESP32_RMT_METHOD(Ws2812x)>
    #define PIXELBUS_32_RN_4  NeoPixelBus<NeoRgbwFeature, NEOPIXELBUS_ESP32_RMT_METHOD(Sk6812)>

    // ---------- 400K ----------
    #ifndef NEOPIXEL_DISABLE_400_PIXELBUS
      #define PIXELBUS_32_RN_400_3 NeoPixelBus<NeoRgbFeature, NEOPIXELBUS_ESP32_RMT_METHOD(400Kbps)>
    #endif

    // ---------- RGBWW (WS2805) ----------
    #define PIXELBUS_32_RN_5  NeoPixelBus<NeoRgbwwFeature, NEOPIXELBUS_ESP32_RMT_METHOD(Ws2805)>  // WS2805 via RMT (channel-selectable). If HI is available it will pick HI, else N.

    // I2S + parallel (your historical naming)
    #define PIXELBUS_32_I0_3   NeoPixelBus<NeoRgbFeature,  NeoEsp32I2s0Sk6812Method>
    #define PIXELBUS_32_I1_3   NeoPixelBus<NeoRgbFeature,  X1Ws2812xMethod>
    #define PIXELBUS_32_I0_3P  NeoPixelBus<NeoRgbFeature,  X8Ws2812xMethod>
    #define PIXELBUS_32_I1_3P  NeoPixelBus<NeoRgbFeature,  X8Ws2812xMethod>

    #define PIXELBUS_32_I0_4   NeoPixelBus<NeoRgbwFeature, NeoEsp32I2s0Sk6812Method>
    #define PIXELBUS_32_I1_4   NeoPixelBus<NeoRgbwFeature, X1Sk6812Method>
    #define PIXELBUS_32_I0_4P  NeoPixelBus<NeoRgbwFeature, X8Sk6812Method>
    #define PIXELBUS_32_I1_4P  NeoPixelBus<NeoRgbwFeature, X8Sk6812Method>

    #define PIXELBUS_32_I0_5   NeoPixelBus<NeoRgbwwFeature, X1Ws2805Method>
    #define PIXELBUS_32_I1_5   NeoPixelBus<NeoRgbwwFeature, X1Ws2805Method>
    #define PIXELBUS_32_I0_5P  NeoPixelBus<NeoRgbwwFeature, X8Ws2805Method>
    #define PIXELBUS_32_I1_5P  NeoPixelBus<NeoRgbwwFeature, X8Ws2805Method>

    #ifndef NEOPIXEL_DISABLE_400_PIXELBUS
      #define PIXELBUS_32_I0_400_3 NeoPixelBus<NeoRgbFeature, NeoEsp32I2s0400KbpsMethod>
      #define PIXELBUS_32_I1_400_3 NeoPixelBus<NeoRgbFeature, NeoEsp32I2s1400KbpsMethod>
    #endif

  #endif // target

#endif // ARDUINO_ARCH_ESP32

// 48bit & 64bit to 24bit & 32bit RGB(W) conversion
#define toRGBW32(c) (RGBW32((c>>40)&0xFF, (c>>24)&0xFF, (c>>8)&0xFF, (c>>56)&0xFF))
#define RGBW32(r,g,b,w) (uint32_t((byte(w) << 24) | (byte(r) << 16) | (byte(g) << 8) | (byte(b))))

#define RGBWW_TO_RGBW32(c) toRGBW32(c)

// handles pointer type conversion for all possible bus types
class PolyBus 
{
  private:

    /**
     * @brief 
     * 0 : No parallel, uses I0, then I1, then RMT 0 to 7 channels for 10 total
     * 1 : Uses X8 parallel on I2S1
     * 2 : Uses X16 parallel on IS20/I2S1 jointly
     */
    static bool useParallelI2S;
    static uint8_t required_channels;
    
    static uint8_t _bri_rgb;

    #ifdef ENABLE_FEATURE_LIGHTING__REDUCED_PHYSICAL_OUTPUT_PIXELS_RENDERED
    static uint16_t reduced_physical_render_length;  // 0 = disabled
    static uint16_t total_virtual_length;  // 0 = disabled
    static inline uint16_t MapToReduced(uint16_t pix) 
    {
      if (reduced_physical_render_length > 0) {
        return pix = (uint16_t)((uint32_t)pix * reduced_physical_render_length / total_virtual_length);
      }
    }
    #endif

  public:
  
    static inline void useParallelOutput(bool b = true) { useParallelI2S = b; }
    static inline bool isParallelOutput(void) { return useParallelI2S; }
    static inline void setRequiredChannels(uint8_t channels){ required_channels = channels; }

    #ifdef ENABLE_FEATURE_LIGHTING__REDUCED_PHYSICAL_OUTPUT_PIXELS_RENDERED
    static inline void SetReducedPhysicalRenderLength(uint16_t n) { reduced_physical_render_length = n; }
    #endif



    // inline static uint8_t _elementDim(uint8_t value, uint8_t ratio)
    // {
    //   return (static_cast<uint16_t>(value) * (static_cast<uint16_t>(ratio) + 1)) >> 8;
    // }





  static void begin(void* busPtr, uint8_t busType, uint8_t* pins) 
  {
    Serial.printf("========================BEGIN: busType=%u pin=%u\n", busType, pins ? pins[0] : 255);
    #ifdef ENABLE_DEBUGFEATURE__16PIN_PARALLEL_OUTPUT
    DEBUG_PRINTF("PolyBus::begin busPtr UNSET, busType %d, pin[0] %d\n\r", busType, pins[0]);
    #endif

    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
      case BUSTYPE__8266_U0_3__ID: (static_cast<PIXELBUS_8266_U0_3*>(busPtr))->Begin(); break;
      case BUSTYPE__8266_U1_3__ID: (static_cast<PIXELBUS_8266_U1_3*>(busPtr))->Begin(); break;
      case BUSTYPE__8266_DM_3__ID: (static_cast<PIXELBUS_8266_DM_3*>(busPtr))->Begin(); break;
      case BUSTYPE__8266_U0_4__ID: (static_cast<PIXELBUS_8266_U0_4*>(busPtr))->Begin(); break;
      case BUSTYPE__8266_U1_4__ID: (static_cast<PIXELBUS_8266_U1_4*>(busPtr))->Begin(); break;
      case BUSTYPE__8266_DM_4__ID: (static_cast<PIXELBUS_8266_DM_4*>(busPtr))->Begin(); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32
      case BUSTYPE__32_RN_3__ID: (static_cast<PIXELBUS_32_RN_3*>(busPtr))->Begin(); break;
      case BUSTYPE__32_RN_4__ID: (static_cast<PIXELBUS_32_RN_4*>(busPtr))->Begin(); break;
      #ifndef NEOPIXEL_DISABLE_400_PIXELBUS
      case BUSTYPE__32_RN_400_3__ID: (static_cast<PIXELBUS_32_RN_400_3*>(busPtr))->Begin(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_3__ID: (static_cast<PIXELBUS_32_I0_3*>(busPtr))->Begin(); break;
      case BUSTYPE__32_I0_4__ID: (static_cast<PIXELBUS_32_I0_4*>(busPtr))->Begin(); break;
      case BUSTYPE__32_I0_5__ID: (static_cast<PIXELBUS_32_I0_5*>(busPtr))->Begin(); break;
      case BUSTYPE__32_I0_400_3__ID: (static_cast<PIXELBUS_32_I0_400_3*>(busPtr))->Begin(); break;
      case BUSTYPE__32_I0_3P__ID: (static_cast<PIXELBUS_32_I0_3P*>(busPtr))->Begin(); break;
      case BUSTYPE__32_I0_4P__ID: (static_cast<PIXELBUS_32_I0_4P*>(busPtr))->Begin(); break;
      case BUSTYPE__32_I0_5P__ID: (static_cast<PIXELBUS_32_I0_5P*>(busPtr))->Begin(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_3__ID: (static_cast<PIXELBUS_32_I1_3*>(busPtr))->Begin(); break;
      case BUSTYPE__32_I1_4__ID: (static_cast<PIXELBUS_32_I1_4*>(busPtr))->Begin(); break;
      case BUSTYPE__32_I1_5__ID: (static_cast<PIXELBUS_32_I1_5*>(busPtr))->Begin(); break;
      case BUSTYPE__32_I1_400_3__ID: (static_cast<PIXELBUS_32_I1_400_3*>(busPtr))->Begin(); break;
      case BUSTYPE__32_I1_3P__ID: (static_cast<PIXELBUS_32_I1_3P*>(busPtr))->Begin(); break;
      case BUSTYPE__32_I1_4P__ID: (static_cast<PIXELBUS_32_I1_4P*>(busPtr))->Begin(); break;
      case BUSTYPE__32_I1_5P__ID: (static_cast<PIXELBUS_32_I1_5P*>(busPtr))->Begin(); break;
      #endif
    #endif
    }
  };

static void* create(uint8_t busType, uint8_t* pins, uint16_t len, uint8_t channel)
{
  DEBUG_PRINTF("PolyBus::create busType %d, pin[0] %d, len %d, channel %d\n\r", busType, pins[0], len, channel);

  #ifdef ENABLE_FEATURE_LIGHTING__REDUCED_PHYSICAL_OUTPUT_PIXELS_RENDERED
    total_virtual_length = len;
  #endif

  void* busPtr = nullptr;

  switch (busType) {
    case BUSTYPE__NONE__ID:
      break;

  #ifdef ESP8266
    case BUSTYPE__8266_U0_3__ID: busPtr = new PIXELBUS_8266_U0_3(len, pins[0]); break;
    case BUSTYPE__8266_U1_3__ID: busPtr = new PIXELBUS_8266_U1_3(len, pins[0]); break;
    case BUSTYPE__8266_DM_3__ID: busPtr = new PIXELBUS_8266_DM_3(len, pins[0]); break;

    case BUSTYPE__8266_U0_4__ID: busPtr = new PIXELBUS_8266_U0_4(len, pins[0]); break;
    case BUSTYPE__8266_U1_4__ID: busPtr = new PIXELBUS_8266_U1_4(len, pins[0]); break;
    case BUSTYPE__8266_DM_4__ID: busPtr = new PIXELBUS_8266_DM_4(len, pins[0]); break;
  #endif

  #ifdef ARDUINO_ARCH_ESP32
    // ------------------------
    // RMT-capable (needs channel)
    // ------------------------
    case BUSTYPE__32_RN_3__ID:
      busPtr = new PIXELBUS_32_RN_3(len, pins[0], (NeoBusChannel)channel);
      break;

    case BUSTYPE__32_RN_4__ID:
      busPtr = new PIXELBUS_32_RN_4(len, pins[0], (NeoBusChannel)channel);
      break;

    #ifndef NEOPIXEL_DISABLE_400_PIXELBUS
    case BUSTYPE__32_RN_400_3__ID:
      busPtr = new PIXELBUS_32_RN_400_3(len, pins[0], (NeoBusChannel)channel);
      break;
    #endif

    // ------------------------
    // RGBWW
    // ------------------------
    case BUSTYPE__32_RN_5__ID:
      busPtr = new PIXELBUS_32_RN_5(len, pins[0], (NeoBusChannel)channel);
      break;

    // ------------------------
    // I2S families (no channel)
    // ------------------------
    #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
    case BUSTYPE__32_I0_3__ID:     busPtr = new PIXELBUS_32_I0_3(len, pins[0]); break;
    case BUSTYPE__32_I0_4__ID:     busPtr = new PIXELBUS_32_I0_4(len, pins[0]); break;
    case BUSTYPE__32_I0_5__ID:     busPtr = new PIXELBUS_32_I0_5(len, pins[0]); break;
    case BUSTYPE__32_I0_3P__ID:    busPtr = new PIXELBUS_32_I0_3P(len, pins[0]); break;
    case BUSTYPE__32_I0_4P__ID:    busPtr = new PIXELBUS_32_I0_4P(len, pins[0]); break;
    case BUSTYPE__32_I0_5P__ID:    busPtr = new PIXELBUS_32_I0_5P(len, pins[0]); break;
    #ifndef NEOPIXEL_DISABLE_400_PIXELBUS
    case BUSTYPE__32_I0_400_3__ID: busPtr = new PIXELBUS_32_I0_400_3(len, pins[0]); break;
    #endif
    #endif

    #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
    case BUSTYPE__32_I1_3__ID:     busPtr = new PIXELBUS_32_I1_3(len, pins[0]); break;
    case BUSTYPE__32_I1_4__ID:     busPtr = new PIXELBUS_32_I1_4(len, pins[0]); break;
    case BUSTYPE__32_I1_5__ID:     busPtr = new PIXELBUS_32_I1_5(len, pins[0]); break;
    case BUSTYPE__32_I1_3P__ID:    busPtr = new PIXELBUS_32_I1_3P(len, pins[0]); break;
    case BUSTYPE__32_I1_4P__ID:    busPtr = new PIXELBUS_32_I1_4P(len, pins[0]); break;
    case BUSTYPE__32_I1_5P__ID:    busPtr = new PIXELBUS_32_I1_5P(len, pins[0]); break;
    #ifndef NEOPIXEL_DISABLE_400_PIXELBUS
    case BUSTYPE__32_I1_400_3__ID: busPtr = new PIXELBUS_32_I1_400_3(len, pins[0]); break;
    #endif
    #endif
  #endif
  }

  return busPtr;
}
//   static void* create(uint8_t busType, uint8_t* pins, uint16_t len, uint8_t channel) 
//   {
//     // #ifdef ENABLE_DEBUGFEATURE__16PIN_PARALLEL_OUTPUT
//     DEBUG_PRINTF("PolyBus::create busType %d, pin[0] %d, len %d, channel %d\n\r", busType, pins[0], len, channel);
//     // #endif
        
//     #ifdef ENABLE_FEATURE_LIGHTING__REDUCED_PHYSICAL_OUTPUT_PIXELS_RENDERED
//       total_virtual_length = len;  // whatever variable holds total LED count
//     #endif
    
//     void* busPtr = nullptr;
//     switch (busType) {
//       case BUSTYPE__NONE__ID: break;
//     #ifdef ESP8266
//       case BUSTYPE__8266_U0_3__ID: busPtr = new PIXELBUS_8266_U0_3(len, pins[0]); break;
//       case BUSTYPE__8266_U1_3__ID: busPtr = new PIXELBUS_8266_U1_3(len, pins[0]); break;
//       case BUSTYPE__8266_DM_3__ID: busPtr = new PIXELBUS_8266_DM_3(len, pins[0]); break;
//       case BUSTYPE__8266_U0_4__ID: busPtr = new PIXELBUS_8266_U0_4(len, pins[0]); break;
//       case BUSTYPE__8266_U1_4__ID: busPtr = new PIXELBUS_8266_U1_4(len, pins[0]); break;
//       case BUSTYPE__8266_DM_4__ID: busPtr = new PIXELBUS_8266_DM_4(len, pins[0]); break;
//     #endif
//     #ifdef ARDUINO_ARCH_ESP32
//       case BUSTYPE__32_RN_3__ID:
//         busPtr = new PIXELBUS_32_RN_3(len, pins[0], (NeoBusChannel)channel);
//         break;

//       case BUSTYPE__32_RN_4__ID:
//         busPtr = new PIXELBUS_32_RN_4(len, pins[0], (NeoBusChannel)channel);
//         break;

//       case BUSTYPE__32_RN_5__ID:
//         // RGBWW has no RMT "N" method; do NOT pass NeoBusChannel.
//         busPtr = new PIXELBUS_32_RN_5(len, pins[0]);
//         break;

//       #ifndef NEOPIXEL_DISABLE_400_PIXELBUS
//       case BUSTYPE__32_RN_400_3__ID:
//         busPtr = new PIXELBUS_32_RN_400_3(len, pins[0], (NeoBusChannel)channel);
//         break;
//       #endif

//       #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
//       case BUSTYPE__32_I0_3__ID:     busPtr = new PIXELBUS_32_I0_3(len, pins[0]); break;
//       case BUSTYPE__32_I0_4__ID:     busPtr = new PIXELBUS_32_I0_4(len, pins[0]); break;
//       case BUSTYPE__32_I0_5__ID:     busPtr = new PIXELBUS_32_I0_5(len, pins[0]); break;
//       case BUSTYPE__32_I0_400_3__ID: busPtr = new PIXELBUS_32_I0_400_3(len, pins[0]); break;
//       case BUSTYPE__32_I0_3P__ID:    busPtr = new PIXELBUS_32_I0_3P(len, pins[0]); break;
//       case BUSTYPE__32_I0_4P__ID:    busPtr = new PIXELBUS_32_I0_4P(len, pins[0]); break;
//       case BUSTYPE__32_I0_5P__ID:    busPtr = new PIXELBUS_32_I0_5P(len, pins[0]); break;
//       #endif

//       #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
//       case BUSTYPE__32_I1_3__ID:     busPtr = new PIXELBUS_32_I1_3(len, pins[0]); break;
//       case BUSTYPE__32_I1_4__ID:     busPtr = new PIXELBUS_32_I1_4(len, pins[0]); break;
//       case BUSTYPE__32_I1_5__ID:     busPtr = new PIXELBUS_32_I1_5(len, pins[0]); break;
//       case BUSTYPE__32_I1_400_3__ID: busPtr = new PIXELBUS_32_I1_400_3(len, pins[0]); break;
//       case BUSTYPE__32_I1_3P__ID:    busPtr = new PIXELBUS_32_I1_3P(len, pins[0]); break;
//       case BUSTYPE__32_I1_4P__ID:    busPtr = new PIXELBUS_32_I1_4P(len, pins[0]); break;
//       case BUSTYPE__32_I1_5P__ID:    busPtr = new PIXELBUS_32_I1_5P(len, pins[0]); break;
//       #endif
// #endif
//     }
      

//     // #ifndef ENABLE_DEVFEATURE_LIGHTING__BEGIN_MUST_HAPPEN_AFTER_ALL_BUSSES_ARE_CREATED
//     // begin(busPtr, busType, pins);
//     // #endif
    
//     return busPtr;
//   };

  static void show(void* busPtr, uint8_t busType, bool consistent = true) 
  {
    
    #ifdef ENABLE_DEBUGFEATURE__16PIN_PARALLEL_OUTPUT
    DEBUG_PRINTF("PolyBus::show busType %d\n\r", busType);
    #endif 
    
    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
      case BUSTYPE__8266_U0_3__ID: (static_cast<PIXELBUS_8266_U0_3*>(busPtr))->Show(consistent); break;
      case BUSTYPE__8266_U1_3__ID: (static_cast<PIXELBUS_8266_U1_3*>(busPtr))->Show(consistent); break;
      case BUSTYPE__8266_DM_3__ID: (static_cast<PIXELBUS_8266_DM_3*>(busPtr))->Show(consistent); break;
      case BUSTYPE__8266_U0_4__ID: (static_cast<PIXELBUS_8266_U0_4*>(busPtr))->Show(consistent); break;
      case BUSTYPE__8266_U1_4__ID: (static_cast<PIXELBUS_8266_U1_4*>(busPtr))->Show(consistent); break;
      case BUSTYPE__8266_DM_4__ID: (static_cast<PIXELBUS_8266_DM_4*>(busPtr))->Show(consistent); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32
      case BUSTYPE__32_RN_3__ID: (static_cast<PIXELBUS_32_RN_3*>(busPtr))->Show(consistent);   break;
      case BUSTYPE__32_RN_4__ID: (static_cast<PIXELBUS_32_RN_4*>(busPtr))->Show(consistent); break;  
      case BUSTYPE__32_RN_5__ID: (static_cast<PIXELBUS_32_RN_5*>(busPtr))->Show(consistent); break;
      #ifndef NEOPIXEL_DISABLE_400_PIXELBUS
      case BUSTYPE__32_RN_400_3__ID: (static_cast<PIXELBUS_32_RN_400_3*>(busPtr))->Show(consistent); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_3__ID: (static_cast<PIXELBUS_32_I0_3*>(busPtr))->Show(consistent); break;
      case BUSTYPE__32_I0_4__ID: (static_cast<PIXELBUS_32_I0_4*>(busPtr))->Show(consistent); break;
      case BUSTYPE__32_I0_5__ID: (static_cast<PIXELBUS_32_I0_5*>(busPtr))->Show(consistent); break;
      case BUSTYPE__32_I0_400_3__ID: (static_cast<PIXELBUS_32_I0_400_3*>(busPtr))->Show(consistent); break;
      case BUSTYPE__32_I0_3P__ID: (static_cast<PIXELBUS_32_I0_3P*>(busPtr))->Show(consistent); break;
      case BUSTYPE__32_I0_4P__ID: (static_cast<PIXELBUS_32_I0_4P*>(busPtr))->Show(consistent); break;
      case BUSTYPE__32_I0_5P__ID: (static_cast<PIXELBUS_32_I0_5P*>(busPtr))->Show(consistent); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_3__ID: (static_cast<PIXELBUS_32_I1_3*>(busPtr))->Show(consistent); break;
      case BUSTYPE__32_I1_4__ID: (static_cast<PIXELBUS_32_I1_4*>(busPtr))->Show(consistent); break;
      case BUSTYPE__32_I1_5__ID: (static_cast<PIXELBUS_32_I1_5*>(busPtr))->Show(consistent); break;
      case BUSTYPE__32_I1_400_3__ID: (static_cast<PIXELBUS_32_I1_400_3*>(busPtr))->Show(consistent); break;
      case BUSTYPE__32_I1_3P__ID: (static_cast<PIXELBUS_32_I1_3P*>(busPtr))->Show(consistent); break;
      case BUSTYPE__32_I1_4P__ID: (static_cast<PIXELBUS_32_I1_4P*>(busPtr))->Show(consistent); break;
      case BUSTYPE__32_I1_5P__ID: (static_cast<PIXELBUS_32_I1_5P*>(busPtr))->Show(consistent); break;
      #endif
    #endif
    }

  };


  static bool canShow(void* busPtr, uint8_t busType) 
  {
    
    #ifdef ENABLE_DEBUGFEATURE__16PIN_PARALLEL_OUTPUT
    DEBUG_PRINTF("PolyBus::canShow busType %d\n\r", busType);
    #endif

    switch (busType) {
      case BUSTYPE__NONE__ID: return true;
    #ifdef ESP8266    
      case BUSTYPE__8266_U0_3__ID: return (static_cast<PIXELBUS_8266_U0_3*>(busPtr))->CanShow(); break;
      case BUSTYPE__8266_U1_3__ID: return (static_cast<PIXELBUS_8266_U1_3*>(busPtr))->CanShow(); break;
      case BUSTYPE__8266_DM_3__ID: return (static_cast<PIXELBUS_8266_DM_3*>(busPtr))->CanShow(); break;
      // case BUSTYPE__8266_BB_3__ID: return (static_cast<PIXELBUS_8266_BB_3*>(busPtr))->CanShow(); break;
      case BUSTYPE__8266_U0_4__ID: return (static_cast<PIXELBUS_8266_U0_4*>(busPtr))->CanShow(); break;
      case BUSTYPE__8266_U1_4__ID: return (static_cast<PIXELBUS_8266_U1_4*>(busPtr))->CanShow(); break;
      case BUSTYPE__8266_DM_4__ID: return (static_cast<PIXELBUS_8266_DM_4*>(busPtr))->CanShow(); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32
      case BUSTYPE__32_RN_3__ID: return (static_cast<PIXELBUS_32_RN_3*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_RN_4__ID: return (static_cast<PIXELBUS_32_RN_4*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_RN_5__ID:  return (static_cast<PIXELBUS_32_RN_5*>(busPtr))->CanShow(); break;
      #ifndef NEOPIXEL_DISABLE_400_PIXELBUS
      case BUSTYPE__32_RN_400_3__ID: return (static_cast<PIXELBUS_32_RN_400_3*>(busPtr))->CanShow(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_3__ID: return (static_cast<PIXELBUS_32_I0_3*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_I0_4__ID: return (static_cast<PIXELBUS_32_I0_4*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_I0_5__ID: return (static_cast<PIXELBUS_32_I0_5*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_I0_400_3__ID: return (static_cast<PIXELBUS_32_I0_400_3*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_I0_3P__ID: return (static_cast<PIXELBUS_32_I0_3P*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_I0_4P__ID: return (static_cast<PIXELBUS_32_I0_4P*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_I0_5P__ID: return (static_cast<PIXELBUS_32_I0_5P*>(busPtr))->CanShow(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_3__ID: return (static_cast<PIXELBUS_32_I1_3*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_I1_4__ID: return (static_cast<PIXELBUS_32_I1_4*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_I1_5__ID: return (static_cast<PIXELBUS_32_I1_5*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_I1_400_3__ID: return (static_cast<PIXELBUS_32_I1_400_3*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_I1_3P__ID: return (static_cast<PIXELBUS_32_I1_3P*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_I1_4P__ID: return (static_cast<PIXELBUS_32_I1_4P*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_I1_5P__ID: return (static_cast<PIXELBUS_32_I1_5P*>(busPtr))->CanShow(); break;
      #endif
    #endif

    }
    return true;
  };

  #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE

  // Function to set pixel color based on color ordering
  static void setPixelColor(void* busPtr, uint8_t busType, uint16_t pix, RgbwwColor c, uint8_t co, uint16_t wwcw = 0)
  {
    
    // c.WW = c.CW = 0; // Clear WW and CW for now;

    #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE_DEBUG
    if(pix==0)
      Serial.printf("    rgbww set bt%d               = %d,%d,%d,%d,%d\n\r", busType, c.R, c.G, c.B, c.WW, c.CW); Serial.flush();
    #endif

  // DEBUG_LINE_HERE;
    // #ifdef ENABLE_DEVFEATURE_LIGHTING__TEMPORARY_DISABLE_CODE_FOR_SPEED_TESTING
    // DEBUG_TIME__START
    // uint32_t __debug_time_start__ = micros();
    // #endif

    // Debug feature to map a large number of virtual pixels to a smaller physical display
    #ifdef ENABLE_FEATURE_LIGHTING__REDUCED_PHYSICAL_OUTPUT_PIXELS_RENDERED
      pix = MapToReduced(pix);
    #endif


    RgbwwColor col = c;

    // Reorder RGB channels based on the provided color order
    switch (co & COLOUR_ORDER_RGB_MASK) {
      default: break;                                      // Default: GRB
      case 1:  std::swap(col.R, col.G); break;            // RGB
      case 2:  std::swap(col.R, col.B); break;            // BRG
      case 3:  std::swap(col.G, col.B); break;            // RBG
      case 4:  std::swap(col.R, col.B); std::swap(col.G, col.R); break; // BGR
      case 5:  std::swap(col.R, col.B); break;            // GBR
    }

    // // Handle White channel reordering (upper nibble)
    // switch (co >> COLOUR_ORDER_WHITE_MASK) {
    //   case 1: col.CW = col.WW; col.WW = 0; break;         // Only CW used
    //   case 2: col.WW = col.CW; col.CW = 0; break;         // Only WW used
    //   case 3: col.WW = 0; col.CW = 0; break;              // Disable both whites
    //   case 4: std::swap(col.WW, col.CW); break;           // Swap WW and CW
    //   default: break;                                     // No swap
    // }

    #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE_DEBUG
    // if(pix==0)
      Serial.printf("    rgbww setw bt%d              = %d,%d,%d,%d,%d\n\r", busType, col.R, col.G, col.B, col.WW, col.CW); Serial.flush();
    #endif
    
// Log the resulting white channels
#ifdef ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS
Serial.printf("After white channel processing: WW=%d, CW=%d\n", col.WW, col.CW);
#endif

    // Optional logging for debugging
    #ifdef ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS
    if(pix == 0){ // Just log for the first pixel
        Serial.printf("set colour R=%d, G=%d, B=%d, CW=%d, WW=%d %d/%d/%d/%d/%d\n\r",
            (colour_order & 0x07),   // Red
            ((colour_order >> 3) & 0x07), // Green
            ((colour_order >> 6) & 0x07), // Blue
            colour_hardware.CW,
            colour_hardware.WW,
            colour_internal.R,
            colour_internal.G,
            colour_internal.B,
            colour_internal.CW,
            colour_internal.WW
        );
    }
    #endif    
    #ifdef ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS // Debug pixel color value log
    if (pix < 1) { // Just log for the first pixel
        Serial.printf("Polybus::setPixelColor[%d] R=%d, G=%d, B=%d, WW=%d, CW=%d\n\r", pix, 
            col.R,col.G,col.B,col.WW,col.CW
        );
    }
    #endif
    
    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
      case BUSTYPE__8266_U0_3__ID: (static_cast<PIXELBUS_8266_U0_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__8266_U1_3__ID: (static_cast<PIXELBUS_8266_U1_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__8266_DM_3__ID: (static_cast<PIXELBUS_8266_DM_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__8266_U0_4__ID: (static_cast<PIXELBUS_8266_U0_4*>(busPtr))->SetPixelColor(pix, col); break;
      case BUSTYPE__8266_U1_4__ID: (static_cast<PIXELBUS_8266_U1_4*>(busPtr))->SetPixelColor(pix, col); break;
      case BUSTYPE__8266_DM_4__ID: (static_cast<PIXELBUS_8266_DM_4*>(busPtr))->SetPixelColor(pix, col); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32
      case BUSTYPE__32_RN_3__ID:      (static_cast<PIXELBUS_32_RN_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__32_RN_4__ID:      (static_cast<PIXELBUS_32_RN_4*>(busPtr))->SetPixelColor(pix, RgbwColor(col)); break;
      case BUSTYPE__32_RN_5__ID:   
      


      (static_cast<PIXELBUS_32_RN_5*>(busPtr))->SetPixelColor(pix, col);
      
      
       break;
      case BUSTYPE__32_RN_400_3__ID:  (static_cast<PIXELBUS_32_RN_400_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_3__ID:      (static_cast<PIXELBUS_32_I0_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__32_I0_4__ID:      (static_cast<PIXELBUS_32_I0_4*>(busPtr))->SetPixelColor(pix, RgbwColor(col)); break;
      case BUSTYPE__32_I0_5__ID:      
      {
      

    #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE_DEBUG
      
      Serial.printf("%d SetPixelColor[%d] R=%d, G=%d, B=%d, WW=%d, CW=%d\n\r", busType,pix, 
            col.R,col.G,col.B,col.WW,col.CW
        );
      #endif
      // (static_cast<PIXELBUS_32_I0_5*>(busPtr))->SetLuminance(255); 
      
      (static_cast<PIXELBUS_32_I0_5*>(busPtr))->SetPixelColor(pix, col); 
      
    #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE_DEBUG
      RgbwwColor col2 = (static_cast<PIXELBUS_32_I0_5*>(busPtr))->GetPixelColor(pix); 
      
      Serial.printf("%d GetPixelColor[%d] R=%d, G=%d, B=%d, WW=%d, CW=%d\n\r", busType,pix, 
            col2.R,col2.G,col2.B,col2.WW,col2.CW
        );
        #endif
      
      }
      break;
      case BUSTYPE__32_I0_400_3__ID:  (static_cast<PIXELBUS_32_I0_400_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__32_I0_3P__ID:     (static_cast<PIXELBUS_32_I0_3P*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__32_I0_4P__ID:     (static_cast<PIXELBUS_32_I0_4P*>(busPtr))->SetPixelColor(pix, RgbwColor(col)); break;
      case BUSTYPE__32_I0_5P__ID:     (static_cast<PIXELBUS_32_I0_5P*>(busPtr))->SetPixelColor(pix, col); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_3__ID: (static_cast<PIXELBUS_32_I1_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__32_I1_4__ID: (static_cast<PIXELBUS_32_I1_4*>(busPtr))->SetPixelColor(pix, RgbwColor(col)); break;
      case BUSTYPE__32_I1_5__ID: (static_cast<PIXELBUS_32_I1_5*>(busPtr))->SetPixelColor(pix, col); break;
      case BUSTYPE__32_I1_400_3__ID: (static_cast<PIXELBUS_32_I1_400_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__32_I1_3P__ID: (static_cast<PIXELBUS_32_I1_3P*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__32_I1_4P__ID: (static_cast<PIXELBUS_32_I1_4P*>(busPtr))->SetPixelColor(pix, RgbwColor(col)); break;
      case BUSTYPE__32_I1_5P__ID: (static_cast<PIXELBUS_32_I1_5P*>(busPtr))->SetPixelColor(pix, col); break;
      #endif
    #endif

    }

    // #ifdef ENABLE_DEVFEATURE_LIGHTING__TEMPORARY_DISABLE_CODE_FOR_SPEED_TESTING
    // #define SERIAL_DEBUG Serial
    // if(pix==0)
    // SERIAL_DEBUG.printf("(%s:%d) %luus %dms\n\r", \
    //                         __FILE__, __LINE__, micros() - __debug_time_start__, (micros() - __debug_time_start__)/1000); \
    //     SERIAL_DEBUG.flush();
    // #endif
}


static RgbwwColor getPixelColor(void* busPtr, uint8_t busType, uint16_t pix, uint8_t co)
{

  #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE_DEBUG
  Serial.printf("getPixelColor[%d] busType %d\n\r", pix, busType);
  #endif

  RgbwwColor col;

#ifdef ENABLE_FEATURE_LIGHTING__REDUCED_PHYSICAL_OUTPUT_PIXELS_RENDERED
  pix = MapToReduced(pix);
#endif


    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266    
      case BUSTYPE__8266_U0_3__ID: col = (static_cast<PIXELBUS_8266_U0_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_U1_3__ID: col = (static_cast<PIXELBUS_8266_U1_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_DM_3__ID: col = (static_cast<PIXELBUS_8266_DM_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_U0_4__ID: col = (static_cast<PIXELBUS_8266_U0_4*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_U1_4__ID: col = (static_cast<PIXELBUS_8266_U1_4*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_DM_4__ID: col = (static_cast<PIXELBUS_8266_DM_4*>(busPtr))->GetPixelColor(pix); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32
      case BUSTYPE__32_RN_3__ID: col = (static_cast<PIXELBUS_32_RN_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_RN_4__ID: col = (static_cast<PIXELBUS_32_RN_4*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_RN_5__ID:  col = (static_cast<PIXELBUS_32_RN_5*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_RN_400_3__ID: col = (static_cast<PIXELBUS_32_RN_400_3*>(busPtr))->GetPixelColor(pix); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_3__ID: col = (static_cast<PIXELBUS_32_I0_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I0_4__ID: col = (static_cast<PIXELBUS_32_I0_4*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I0_5__ID: 
      
      col = (static_cast<PIXELBUS_32_I0_5*>(busPtr))->GetPixelColor(pix); 
            
    #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE_DEBUG
      Serial.printf("%dFGetPixelColor[%d] R=%d, G=%d, B=%d, WW=%d, CW=%d\n\r", busType,pix, 
            col.R,col.G,col.B,col.WW,col.CW
        );
        #endif
      
      break;
      case BUSTYPE__32_I0_400_3__ID: col = (static_cast<PIXELBUS_32_I0_400_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I0_3P__ID: col = (static_cast<PIXELBUS_32_I0_3P*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I0_4P__ID: col = (static_cast<PIXELBUS_32_I0_4P*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I0_5P__ID: col = (static_cast<PIXELBUS_32_I0_5P*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_3__ID: col = (static_cast<PIXELBUS_32_I1_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I1_4__ID: col = (static_cast<PIXELBUS_32_I1_4*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I1_5__ID: col = (static_cast<PIXELBUS_32_I1_5*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I1_400_3__ID: col = (static_cast<PIXELBUS_32_I1_400_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I1_3P__ID: col = (static_cast<PIXELBUS_32_I1_3P*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I1_4P__ID: col = (static_cast<PIXELBUS_32_I1_4P*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I1_5P__ID: col = (static_cast<PIXELBUS_32_I1_5P*>(busPtr))->GetPixelColor(pix); break;
      #endif
    #endif
    }

    // Reorder RGB channels back to their original order
    switch (co & COLOUR_ORDER_RGB_MASK) {
      default: break;                                      // Default: GRB
      case 1:  std::swap(col.R, col.G); break;            // RGB
      case 2:  std::swap(col.R, col.B); break;            // BRG
      case 3:  std::swap(col.G, col.B); break;            // RBG
      case 4:  std::swap(col.G, col.R); std::swap(col.R, col.B); break; // BGR
      case 5:  std::swap(col.R, col.B); break;            // GBR
    }

    // // Reorder White channels back to their original order
    // switch (co >> COLOUR_ORDER_WHITE_MASK) {
    //   case 1: col.WW = col.CW; col.CW = 0; break;         // Only CW was used
    //   case 2: col.CW = col.WW; col.WW = 0; break;         // Only WW was used
    //   case 3: col.WW = 0; col.CW = 0; break;              // Both whites were disabled
    //   case 4: std::swap(col.WW, col.CW); break;           // Swap WW and CW back
    //   default: break;                                     // No swap
    // }
    
    #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE_DEBUG
    // if(pix==0)
      Serial.printf("     rgbww get bt%d                %d,%d,%d,%d,%d\n\r", busType, col.R, col.G, col.B, col.WW, col.CW); Serial.flush();
#endif
      // col = RgbwwColor(21,22,23,20);

    #ifdef ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS
    if (pix < 5) { // Just first few pixels
      Serial.printf("get colour_order R=%d, G=%d, B=%d, CW=%d, WW=%d\n\r",
        rgb_order,
        white_order,
        color_internal.R,
        color_internal.G,
        color_internal.B,
        color_internal.CW,
        color_internal.WW
      );
    }
    #endif // ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

    // col.WW = col.CW = 0; // Clear WW and CW for now;

    return col;
}


  #else

  // Function to set pixel color based on color ordering
  static void setPixelColor(void* busPtr, uint8_t busType, uint16_t pix, uint32_t c, uint8_t co, uint16_t wwcw = 0)
  {

    // #ifdef ENABLE_DEVFEATURE_LIGHTING__TEMPORARY_DISABLE_CODE_FOR_SPEED_TESTING
    // DEBUG_TIME__START
    // uint32_t __debug_time_start__ = micros();
    // #endif

    /***
     * KEEP: Excellent feature I need to test more
     * Allows testing performance of 2000+ LEDs but visually seeing them on smaller connected hardware
     * Should be tweaked that it always sends and gets from the bus, so say 20 virtual pixels are 5 real pixels
     * index transmit 0 0 0 0 0 1 1 1 1 1 2 2 2 2 2 .... 4 4 4 4 4 // hence bus timing is accurate, but virtually its condensed
     */
    // Debug feature to map a large number of virtual pixels to a smaller physical display
#ifdef ENABLE_FEATURE_LIGHTING__REDUCED_PHYSICAL_OUTPUT_PIXELS_RENDERED
  pix = MapToReduced(pix);
#endif

    uint8_t r = c >> 16;
    uint8_t g = c >> 8;
    uint8_t b = c >> 0;
    uint8_t w = c >> 24;
    uint8_t cctWW =  wwcw     & 0xFF;
    uint8_t cctCW = (wwcw>>8) & 0xFF;

    RgbwColor col;

    // reorder channels to selected order
    switch (co & COLOUR_ORDER_RGB_MASK) {
      default: col.G = g; col.R = r; col.B = b; break; //0 = GRB, default
      case  1: col.G = r; col.R = g; col.B = b; break; //1 = RGB, common for WS2811
      case  2: col.G = b; col.R = r; col.B = g; break; //2 = BRG
      case  3: col.G = r; col.R = b; col.B = g; break; //3 = RBG
      case  4: col.G = b; col.R = g; col.B = r; break; //4 = BGR
      case  5: col.G = g; col.R = b; col.B = r; break; //5 = GBR
    }
    // upper nibble contains W swap information
    // switch (co >> COLOUR_ORDER_WHITE_MASK) {
    //   default: col.W = w;                break; // no swapping
    //   case  1: col.W = col.B; col.B = w; break; // swap W & B
    //   case  2: col.W = col.G; col.G = w; break; // swap W & G
    //   case  3: col.W = col.R; col.R = w; break; // swap W & R
    //   case  4: std::swap(cctWW, cctCW);  break; // swap WW & CW
    // }
    col.W = w; 

   
    #ifdef ENABLE_DEBUGFEATURE_LIGHTING__TRACE_PIXEL_SET_GET_SHOW_FIRST_NUMBER_LOGGED_WITH_VALUE
    if (pix < ENABLE_DEBUGFEATURE_LIGHTING__TRACE_PIXEL_SET_GET_SHOW_FIRST_NUMBER_LOGGED_WITH_VALUE) { // Just first few pixels
      Serial.printf("wrap:Set%d[%d] pixel \t\t%d,%d,%d,%d\n\r", busType, pix,col.R,col.G,col.B,col.W);
    }
    #endif

    // if(pix==52)
    //   col = RgbwColor(0,255,0,0);
    
    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
      case BUSTYPE__8266_U0_3__ID: (static_cast<PIXELBUS_8266_U0_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__8266_U1_3__ID: (static_cast<PIXELBUS_8266_U1_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__8266_DM_3__ID: (static_cast<PIXELBUS_8266_DM_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__8266_U0_4__ID: (static_cast<PIXELBUS_8266_U0_4*>(busPtr))->SetPixelColor(pix, col); break;
      case BUSTYPE__8266_U1_4__ID: (static_cast<PIXELBUS_8266_U1_4*>(busPtr))->SetPixelColor(pix, col); break;
      case BUSTYPE__8266_DM_4__ID: (static_cast<PIXELBUS_8266_DM_4*>(busPtr))->SetPixelColor(pix, col); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32
      case BUSTYPE__32_RN_3__ID:      (static_cast<PIXELBUS_32_RN_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__32_RN_4__ID:      (static_cast<PIXELBUS_32_RN_4*>(busPtr))->SetPixelColor(pix, col); break;
      case BUSTYPE__32_RN_5__ID:   (static_cast<PIXELBUS_32_RN_5*>(busPtr))->SetPixelColor(pix, col); break;
      #ifndef NEOPIXEL_DISABLE_400_PIXELBUS
      case BUSTYPE__32_RN_400_3__ID:  (static_cast<PIXELBUS_32_RN_400_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_3__ID:      (static_cast<PIXELBUS_32_I0_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__32_I0_4__ID:      (static_cast<PIXELBUS_32_I0_4*>(busPtr))->SetPixelColor(pix, col); break;
      case BUSTYPE__32_I0_5__ID:      (static_cast<PIXELBUS_32_I0_5*>(busPtr))->SetPixelColor(pix, RgbwwColor(col)); break;
      case BUSTYPE__32_I0_400_3__ID:  (static_cast<PIXELBUS_32_I0_400_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__32_I0_3P__ID:     (static_cast<PIXELBUS_32_I0_3P*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__32_I0_4P__ID:     (static_cast<PIXELBUS_32_I0_4P*>(busPtr))->SetPixelColor(pix, col); break;
      case BUSTYPE__32_I0_5P__ID:     (static_cast<PIXELBUS_32_I0_5P*>(busPtr))->SetPixelColor(pix, RgbwwColor(col)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_3__ID: (static_cast<PIXELBUS_32_I1_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__32_I1_4__ID: (static_cast<PIXELBUS_32_I1_4*>(busPtr))->SetPixelColor(pix, col); break;
      case BUSTYPE__32_I1_5__ID: (static_cast<PIXELBUS_32_I1_5*>(busPtr))->SetPixelColor(pix, RgbwwColor(col)); break;
      case BUSTYPE__32_I1_400_3__ID: (static_cast<PIXELBUS_32_I1_400_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__32_I1_3P__ID: (static_cast<PIXELBUS_32_I1_3P*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__32_I1_4P__ID: (static_cast<PIXELBUS_32_I1_4P*>(busPtr))->SetPixelColor(pix, col); break;
      case BUSTYPE__32_I1_5P__ID: (static_cast<PIXELBUS_32_I1_5P*>(busPtr))->SetPixelColor(pix, RgbwwColor(col)); break;
      #endif
    #endif

    }

    // #ifdef ENABLE_DEVFEATURE_LIGHTING__TEMPORARY_DISABLE_CODE_FOR_SPEED_TESTING
    // #define SERIAL_DEBUG Serial
    // if(pix==0)
    // SERIAL_DEBUG.printf("(%s:%d) %luus %dms\n\r", \
    //                         __FILE__, __LINE__, micros() - __debug_time_start__, (micros() - __debug_time_start__)/1000); \
    //     SERIAL_DEBUG.flush();
    // #endif
}


static uint32_t getPixelColor(void* busPtr, uint8_t busType, uint16_t pix, uint8_t co)
{
  RgbwColor col(0,0,0,0);

  // #ifdef ENABLE_DEBUGFEATURE__LIGHTING__MATCH_FEWER_PHYSICAL_PIXELS
  // // Modulo operation for debugging larger pixel counts on a smaller display
  // pix = pix % ENABLE_DEBUGFEATURE__LIGHTING__MATCH_FEWER_PHYSICAL_PIXELS; // Map to fewer physical pixels
  // #endif

  #ifdef ENABLE_FEATURE_LIGHTING__REDUCED_PHYSICAL_OUTPUT_PIXELS_RENDERED
    pix = MapToReduced(pix);
  #endif




    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266    
      case BUSTYPE__8266_U0_3__ID: col = (static_cast<PIXELBUS_8266_U0_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_U1_3__ID: col = (static_cast<PIXELBUS_8266_U1_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_DM_3__ID: col = (static_cast<PIXELBUS_8266_DM_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_U0_4__ID: col = (static_cast<PIXELBUS_8266_U0_4*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_U1_4__ID: col = (static_cast<PIXELBUS_8266_U1_4*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_DM_4__ID: col = (static_cast<PIXELBUS_8266_DM_4*>(busPtr))->GetPixelColor(pix); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32
      case BUSTYPE__32_RN_3__ID: col = (static_cast<PIXELBUS_32_RN_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_RN_4__ID: col = (static_cast<PIXELBUS_32_RN_4*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__32_RN_5__ID:  col = (static_cast<PIXELBUS_32_RN_5*>(busPtr))->GetPixelColor(pix); break;
      #ifndef NEOPIXEL_DISABLE_400_PIXELBUS
      case BUSTYPE__32_RN_400_3__ID: col = (static_cast<PIXELBUS_32_RN_400_3*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_3__ID: col = (static_cast<PIXELBUS_32_I0_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I0_4__ID: col = (static_cast<PIXELBUS_32_I0_4*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I0_5__ID: { RgbwwColor c = (static_cast<PIXELBUS_32_I0_5*>(busPtr))->GetPixelColor(pix); col = RGBW32(c.R,c.G,c.B,max(c.WW,c.CW)); } break; // will not return original W
      case BUSTYPE__32_I0_400_3__ID: col = (static_cast<PIXELBUS_32_I0_400_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I0_3P__ID: col = (static_cast<PIXELBUS_32_I0_3P*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I0_4P__ID: col = (static_cast<PIXELBUS_32_I0_4P*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I0_5P__ID: { RgbwwColor c = (static_cast<PIXELBUS_32_I0_5P*>(busPtr))->GetPixelColor(pix); col = RGBW32(c.R,c.G,c.B,max(c.WW,c.CW)); } break; // will not return original W
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_3__ID: col = (static_cast<PIXELBUS_32_I1_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I1_4__ID: col = (static_cast<PIXELBUS_32_I1_4*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I1_5__ID: { RgbwwColor c = (static_cast<PIXELBUS_32_I1_5*>(busPtr))->GetPixelColor(pix); col = RGBW32(c.R,c.G,c.B,max(c.WW,c.CW)); } break; // will not return original W
      case BUSTYPE__32_I1_400_3__ID: col = (static_cast<PIXELBUS_32_I1_400_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I1_3P__ID: col = (static_cast<PIXELBUS_32_I1_3P*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I1_4P__ID: col = (static_cast<PIXELBUS_32_I1_4P*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I1_5P__ID: { RgbwwColor c = (static_cast<PIXELBUS_32_I1_5P*>(busPtr))->GetPixelColor(pix); col = RGBW32(c.R,c.G,c.B,max(c.WW,c.CW)); } break; // will not return original W
      #endif
    #endif
    }

    #ifdef ENABLE_DEBUGFEATURE_LIGHTING__TRACE_PIXEL_SET_GET_SHOW_FIRST_NUMBER_LOGGED_WITH_VALUE
    if (pix < ENABLE_DEBUGFEATURE_LIGHTING__TRACE_PIXEL_SET_GET_SHOW_FIRST_NUMBER_LOGGED_WITH_VALUE) { // Just first few pixels
      Serial.printf("wrap:get pixel \t\t\t%d,%d,%d,%d\n\r",col.R,col.G,col.B,col.W);
    }
    #endif

    // upper nibble contains W swap information
    uint8_t w = col.W;
    // switch (co >> COLOUR_ORDER_WHITE_MASK) {
    //   case 1: col.W = col.B; col.B = w; break; // swap W & B
    //   case 2: col.W = col.G; col.G = w; break; // swap W & G
    //   case 3: col.W = col.R; col.R = w; break; // swap W & R
    // }

    switch (co & COLOUR_ORDER_RGB_MASK) {
      //                    W               G              R               B
      default: return ((col.W << 24) | (col.G << 8) | (col.R << 16) | (col.B)); //0 = GRB, default
      case  1: return ((col.W << 24) | (col.R << 8) | (col.G << 16) | (col.B)); //1 = RGB, common for WS2811
      case  2: return ((col.W << 24) | (col.B << 8) | (col.R << 16) | (col.G)); //2 = BRG
      case  3: return ((col.W << 24) | (col.B << 8) | (col.G << 16) | (col.R)); //3 = RBG
      case  4: return ((col.W << 24) | (col.R << 8) | (col.B << 16) | (col.G)); //4 = BGR
      case  5: return ((col.W << 24) | (col.G << 8) | (col.B << 16) | (col.R)); //5 = GBR
    }

    return 0;
}

#endif // ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE

  static void setBrightness(void* busPtr, uint8_t busType, uint8_t b) 
  {
    #ifdef ENABLE_FEATURE_LIGHTING__USE_NEOPIXELBUS_LIGHT_GAMMA_LG
    
    #ifdef ENABLE_DEBUGFEATURE__16PIN_PARALLEL_OUTPUT
    DEBUG_PRINTF("PolyBus::show busType %d\n\r", busType);
    #endif 
    
    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
      case BUSTYPE__8266_U0_3__ID: (static_cast<PIXELBUS_8266_U0_3*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__8266_U1_3__ID: (static_cast<PIXELBUS_8266_U1_3*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__8266_DM_3__ID: (static_cast<PIXELBUS_8266_DM_3*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__8266_U0_4__ID: (static_cast<PIXELBUS_8266_U0_4*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__8266_U1_4__ID: (static_cast<PIXELBUS_8266_U1_4*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__8266_DM_4__ID: (static_cast<PIXELBUS_8266_DM_4*>(busPtr))->SetLuminance(b); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32
      case BUSTYPE__32_RN_3__ID: (static_cast<PIXELBUS_32_RN_3*>(busPtr))->SetLuminance(b);   
      case BUSTYPE__32_RN_4__ID: (static_cast<PIXELBUS_32_RN_4*>(busPtr))->SetLuminance(b); break;  
      case BUSTYPE__32_RN_5__ID: (static_cast<PIXELBUS_32_RN_5*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__32_RN_400_3__ID: (static_cast<PIXELBUS_32_RN_400_3*>(busPtr))->SetLuminance(b); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_3__ID: (static_cast<PIXELBUS_32_I0_3*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__32_I0_4__ID: (static_cast<PIXELBUS_32_I0_4*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__32_I0_5__ID: (static_cast<PIXELBUS_32_I0_5*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__32_I0_400_3__ID: (static_cast<PIXELBUS_32_I0_400_3*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__32_I0_3P__ID: (static_cast<PIXELBUS_32_I0_3P*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__32_I0_4P__ID: (static_cast<PIXELBUS_32_I0_4P*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__32_I0_5P__ID: (static_cast<PIXELBUS_32_I0_5P*>(busPtr))->SetLuminance(b); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_3__ID: (static_cast<PIXELBUS_32_I1_3*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__32_I1_4__ID: (static_cast<PIXELBUS_32_I1_4*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__32_I1_5__ID: (static_cast<PIXELBUS_32_I1_5*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__32_I1_400_3__ID: (static_cast<PIXELBUS_32_I1_400_3*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__32_I1_3P__ID: (static_cast<PIXELBUS_32_I1_3P*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__32_I1_4P__ID: (static_cast<PIXELBUS_32_I1_4P*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__32_I1_5P__ID: (static_cast<PIXELBUS_32_I1_5P*>(busPtr))->SetLuminance(b); break;
      #endif
    #endif
    }

    #else
    _bri_rgb = b;

    #endif // ENABLE_FEATURE_LIGHTING__USE_NEOPIXELBUS_LIGHT_GAMMA_LG

  };



  static void cleanup(void* busPtr, uint8_t busType) 
  {

    // DEBUG_PRINTF("PolyBus::cleanup busType %d\n\r", busType);
    
    if (busPtr == nullptr) return;
    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266    
      case BUSTYPE__8266_U0_3__ID: delete (static_cast<PIXELBUS_8266_U0_3*>(busPtr)); break;
      case BUSTYPE__8266_U1_3__ID: delete (static_cast<PIXELBUS_8266_U1_3*>(busPtr)); break;
      case BUSTYPE__8266_DM_3__ID: delete (static_cast<PIXELBUS_8266_DM_3*>(busPtr)); break;
      case BUSTYPE__8266_U0_4__ID: delete (static_cast<PIXELBUS_8266_U0_4*>(busPtr)); break;
      case BUSTYPE__8266_U1_4__ID: delete (static_cast<PIXELBUS_8266_U1_4*>(busPtr)); break;
      case BUSTYPE__8266_DM_4__ID: delete (static_cast<PIXELBUS_8266_DM_4*>(busPtr)); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32
      case BUSTYPE__32_RN_3__ID: delete (static_cast<PIXELBUS_32_RN_3*>(busPtr)); break;
      case BUSTYPE__32_RN_4__ID: delete (static_cast<PIXELBUS_32_RN_4*>(busPtr)); break;
      // case BUSTYPE__32_RN_5__ID: delete (static_cast<PIXELBUS_32_RN_5*>(busPtr)); break
      #ifndef NEOPIXEL_DISABLE_400_PIXELBUS
      case BUSTYPE__32_RN_400_3__ID: delete (static_cast<PIXELBUS_32_RN_400_3*>(busPtr)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_3__ID: delete (static_cast<PIXELBUS_32_I0_3*>(busPtr)); break;
      case BUSTYPE__32_I0_4__ID: delete (static_cast<PIXELBUS_32_I0_4*>(busPtr)); break;
      case BUSTYPE__32_I0_5__ID: delete (static_cast<PIXELBUS_32_I0_5*>(busPtr)); break;
      case BUSTYPE__32_I0_400_3__ID: delete (static_cast<PIXELBUS_32_I0_400_3*>(busPtr)); break;
      case BUSTYPE__32_I0_3P__ID: delete (static_cast<PIXELBUS_32_I0_3P*>(busPtr)); break;
      case BUSTYPE__32_I0_4P__ID: delete (static_cast<PIXELBUS_32_I0_4P*>(busPtr)); break;
      case BUSTYPE__32_I0_5P__ID: delete (static_cast<PIXELBUS_32_I0_5P*>(busPtr)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_3__ID: delete (static_cast<PIXELBUS_32_I1_3*>(busPtr)); break;
      case BUSTYPE__32_I1_4__ID: delete (static_cast<PIXELBUS_32_I1_4*>(busPtr)); break;
      case BUSTYPE__32_I1_5__ID: delete (static_cast<PIXELBUS_32_I1_5*>(busPtr)); break;
      case BUSTYPE__32_I1_400_3__ID: delete (static_cast<PIXELBUS_32_I1_400_3*>(busPtr)); break;
      case BUSTYPE__32_I1_3P__ID: delete (static_cast<PIXELBUS_32_I1_3P*>(busPtr)); break;
      case BUSTYPE__32_I1_4P__ID: delete (static_cast<PIXELBUS_32_I1_4P*>(busPtr)); break;
      case BUSTYPE__32_I1_5P__ID: delete (static_cast<PIXELBUS_32_I1_5P*>(busPtr)); break;
      #endif
    #endif
    }
  }

static unsigned memUsage(unsigned count, unsigned busType)
{
  unsigned size = count * 3; // Default RGB: 3 bytes per pixel

  switch (busType)
  {
    case BUSTYPE__NONE__ID:
      size = 0;
      break;

#ifdef ESP8266

    // UART methods: one RGB pixel buffer
    case BUSTYPE__8266_U0_3__ID:
    case BUSTYPE__8266_U1_3__ID:
      break;

    // UART methods: one RGBW pixel buffer
    case BUSTYPE__8266_U0_4__ID:
    case BUSTYPE__8266_U1_4__ID:
      size += count;
      break;

    // DMA methods: pixel buffer plus expanded DMA buffer
    case BUSTYPE__8266_DM_3__ID:
      size *= 5;
      break;

    case BUSTYPE__8266_DM_4__ID:
      size = (size + count) * 5;
      break;

#else // ESP32

    // RMT: front and back buffers
    case BUSTYPE__32_RN_3__ID:
    case BUSTYPE__32_RN_400_3__ID:
      size *= 2;
      break;

    case BUSTYPE__32_RN_4__ID:
      size = (size + count) * 2;
      break;

    case BUSTYPE__32_RN_5__ID:
      size = (size + 2 * count) * 2;
      break;

#ifndef CONFIG_IDF_TARGET_ESP32C3

    // I2S: one front buffer; DMA storage is not included here
    case BUSTYPE__32_I0_3__ID:
    case BUSTYPE__32_I1_3__ID:
    case BUSTYPE__32_I1_3P__ID:
    case BUSTYPE__32_I0_3P__ID:
    case BUSTYPE__32_I0_400_3__ID:
    case BUSTYPE__32_I1_400_3__ID:
      break;

    case BUSTYPE__32_I0_4__ID:
    case BUSTYPE__32_I1_4__ID:
    case BUSTYPE__32_I1_4P__ID:
    case BUSTYPE__32_I0_4P__ID:
      size += count;
      break;

    case BUSTYPE__32_I0_5__ID:
    case BUSTYPE__32_I1_5__ID:
    case BUSTYPE__32_I1_5P__ID:
    case BUSTYPE__32_I0_5P__ID:
      size += 2 * count;
      break;

#endif // !CONFIG_IDF_TARGET_ESP32C3

    // Clocked buses: retain the conservative two-buffer estimate for now
    case BUSTYPE__HS_DOT_3__ID:
    case BUSTYPE__SS_DOT_3__ID:
    case BUSTYPE__HS_LPD_3__ID:
    case BUSTYPE__SS_LPD_3__ID:
    case BUSTYPE__HS_WS1_3__ID:
    case BUSTYPE__SS_WS1_3__ID:
    case BUSTYPE__HS_P98_3__ID:
    case BUSTYPE__SS_P98_3__ID:
    case BUSTYPE__HS_LPO_3__ID:
    case BUSTYPE__SS_LPO_3__ID:
      size *= 2;
      break;

    default:
      size *= 2;
      break;

#endif // ESP8266

  }

  return size;
}


  //gives back the internal type index (I_XX_XXX_X above) for the input 
static    
#ifdef USE_DEVFEATURE_IRAM__PIXEL_BUS_INTERFACING
IRAM_ATTR
#endif 
uint8_t getI(uint8_t busType, const uint8_t* pins, uint8_t num = 0) 
{
  // DEBUG_PRINTF("PolyBus::getI busType %d\n\r", busType);

  if (!IS_BUSTYPE_DIGITAL(busType)) return BUSTYPE__NONE__ID;

  if (IS_BUSTYPE_2PIN(busType)) { //SPI LED chips
      bool isHSPI = false;

      #ifdef ESP8266
          if (pins[0] == P_8266_HS_MOSI && pins[1] == P_8266_HS_CLK) isHSPI = true;
      #else
          // temporary hack to limit use of hardware SPI to a single SPI peripheral (HSPI): 
          // only allow ESP32 hardware serial on segment 0
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
                  return BUSTYPE__8266_U0_3__ID + offset_method_inside_group;
              case BUSTYPE_SK6812_RGBW:
                  return BUSTYPE__8266_U0_4__ID + offset_method_inside_group;
          }
      #else //ESP32
          uint8_t offset_method_inside_group = 0;

          #if defined(CONFIG_IDF_TARGET_ESP32S2)
              // ESP32-S2 only has 4 RMT channels
              if (num > 4) return BUSTYPE__NONE__ID;
              if (num > 3) offset_method_inside_group = 1;  // only one I2S

          #elif defined(CONFIG_IDF_TARGET_ESP32C3)
              // On ESP32-C3 only the first 2 RMT channels are usable for transmitting
              if (num > 1) return BUSTYPE__NONE__ID;

              // need to force RMT on C3, as I2S is not supported for WS2812
              offset_method_inside_group = 0; // force RMT method

              //if (num > 1) offset_method_inside_group = 1; // I2S not supported yet (only 1 I2S)

          #elif defined(CONFIG_IDF_TARGET_ESP32S3)
              // On ESP32-S3 only the first 4 RMT channels are usable for transmitting
              if (num > 3) return BUSTYPE__NONE__ID;
              //if (num > 3) offset_method_inside_group = num -4; // I2S not supported yet

          #else

              #ifdef ENABLE_FEATURE_LIGHTING__I2S_SINGLE_AND_PARALLEL_AUTO_DETECT

                if(useParallelI2S)
                {
                  Serial.println("BUS DETECT: useParallelI2S");
                  Serial.printf("required_channels %d %d\n\r", required_channels, num);
                  if(required_channels <= 8 && num < 8)
                  { 
                    offset_method_inside_group = 3;  // Handled inside library automatically for I2S1 types
                  }else
                  if(required_channels <= 16 && num < 16)
                  {
                    offset_method_inside_group = 4;  // Handled inside library automatically for I2S1 types
                  }
                  else {
                    return BUSTYPE__NONE__ID;
                  }
                  Serial.printf("offset_method_inside_group %d\n\r", offset_method_inside_group);
                  
                }
                else // I0, I1, RMT0 to RMT7
                {
                  if(num == 0){
                    offset_method_inside_group = 1; // I2S0 preffered( RMT0, I2S0, I2S1)
                    Serial.println("BUS DETECT: I2S0 preffered");
                  }else 
                  if (num < 2) { // Channel 0 and 1, will be I2S0 and I2S1
                    offset_method_inside_group = num + 1; // +1 to skip RMT method
                    Serial.printf("BUS DETECT: RMT num%d, busType%d\n\r", num, offset_method_inside_group);
                  } else if (num < 9) {
                    offset_method_inside_group = num; // Use RMT0 to 7
                    Serial.printf("BUS DETECT: I2S num%d, busType%d\n\r", num, offset_method_inside_group);
                  } else {
                    return BUSTYPE__NONE__ID;
                  }
                  Serial.printf("BUS DETECT: No Parallel num%d, busType%d\n\r^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\r", num, offset_method_inside_group);
                }

              #else
                  #if defined(ENABLE_PIXELBUS_BUSMETHODS__I2S_SINGLE_CHANNELS_THEN_8_RMT_CHANNELS)
                      if (num < 2) {
                          offset_method_inside_group = num + 1;  // To skip that RMT was entered first in enum
                      } else if (num < 9) {
                          offset_method_inside_group = num - 7;
                      } else {
                          return BUSTYPE__NONE__ID;
                      }
                  #elif defined(ENABLE_PIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE)
                      if (num < 8) {
                          offset_method_inside_group = 4;  // Handled inside library automatically for I2S1 types
                      } else {
                          return BUSTYPE__NONE__ID;
                      }
                  #elif defined(ENABLE_PIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE)
                      if (num < 16) {
                          offset_method_inside_group = 5;  // Handled inside library automatically for I2S1 types
                      } else {
                          return BUSTYPE__NONE__ID;
                      }
                  #elif defined(ENABLE_PIXELBUS_BUSMETHODS__RMT_8_CHANNELS_THEN_I2S_DUAL_CHANNELS)
                      if (num > 9) {
                          return BUSTYPE__NONE__ID;
                      }
                      if (num > 7) {
                          offset_method_inside_group = num - 7;
                      }
                      #warning "RMT methods cause flickering on ESP32, use I2S methods instead -- needs debugging"
                  #else
                  #error "2024: No method defined"
                  #endif
              #endif // ENABLE_PIXELBUS_BUSMETHODS__I2S_AUTO_CHANNEL_SWITCHING

          #endif

          switch (busType) {
              case BUSTYPE_WS2812_RGB:
              case BUSTYPE_WS2812_WWA:
                  return BUSTYPE__32_RN_3__ID + offset_method_inside_group;
              case BUSTYPE_SK6812_RGBW:
                  return BUSTYPE__32_RN_4__ID + offset_method_inside_group;
              case BUSTYPE_WS2805_RGBWW:
                  return BUSTYPE__32_RN_5__ID + offset_method_inside_group;
              case BUSTYPE_WS2811_400KHZ:
                  return BUSTYPE__32_RN_400_3__ID + offset_method_inside_group;
          }

      #endif
  }

  return BUSTYPE__NONE__ID;
}



//   static
// #ifdef USE_DEVFEATURE_IRAM__PIXEL_BUS_INTERFACING
// IRAM_ATTR
// #endif
// uint8_t getI(uint8_t busType, uint8_t* pins, uint8_t num = 0)
// {
//   if (!IS_BUSTYPE_DIGITAL(busType)) return BUSTYPE__NONE__ID;

//   // ---------------------------------------------------------------------------
//   // 2-pin SPI LED chips (kept as-is: you had HSPI/SSPI mapping here)
//   // ---------------------------------------------------------------------------
//   if (IS_BUSTYPE_2PIN(busType)) {
//     bool isHSPI = false;

//     #ifdef ESP8266
//       if (pins[0] == P_8266_HS_MOSI && pins[1] == P_8266_HS_CLK) isHSPI = true;
//     #else
//       if (!num) isHSPI = true;
//     #endif

//     uint8_t t = BUSTYPE__NONE__ID;
//     switch (busType) {
//       case BUSTYPE_APA102:  t = BUSTYPE__SS_DOT_3__ID; break;
//       case BUSTYPE_LPD8806: t = BUSTYPE__SS_LPD_3__ID; break;
//       case BUSTYPE_LPD6803: t = BUSTYPE__SS_LPO_3__ID; break;
//       case BUSTYPE_WS2801:  t = BUSTYPE__SS_WS1_3__ID; break;
//       case BUSTYPE_P9813:   t = BUSTYPE__SS_P98_3__ID; break;
//       default:              t = BUSTYPE__NONE__ID;     break;
//     }
//     if (t > BUSTYPE__NONE__ID && isHSPI) t--; // HW SPI has one smaller ID than SW
//     return t;
//   }

//   // ---------------------------------------------------------------------------
//   // ESP8266 digital (unchanged behaviour: U0/U1/DM based on pin)
//   // ---------------------------------------------------------------------------
//   #ifdef ESP8266
//     uint8_t offset = pins[0] - 1;
//     if (offset > 3) offset = 3;

//     switch (busType) {
//       case BUSTYPE_WS2812_RGB:
//       case BUSTYPE_WS2812_WWA:
//         return BUSTYPE__8266_U0_3__ID + offset;

//       case BUSTYPE_SK6812_RGBW:
//         return BUSTYPE__8266_U0_4__ID + offset;

//       default:
//         return BUSTYPE__NONE__ID;
//     }
//   #else
//   // ---------------------------------------------------------------------------
//   // ESP32 digital: IMPORTANT
//   // - Your enum encodes METHOD FAMILY, not RMT channel.
//   // - RMT channel is selected by "num" passed into PolyBus::create(..., channel).
//   // - Therefore: return RN/I0/I1/I*P ONLY. No offset games.
//   // ---------------------------------------------------------------------------

//     // Target-specific hard limits (optional; keeps you honest)
//     #if defined(CONFIG_IDF_TARGET_ESP32C3)
//       // You said: only first 2 RMT channels usable for TX on your C3 setup
//       if (num > 1) return BUSTYPE__NONE__ID;

//       // Force RMT family on C3 (your policy)
//       switch (busType) {
//         case BUSTYPE_WS2812_RGB:
//         case BUSTYPE_WS2812_WWA:
//           return BUSTYPE__32_RN_3__ID;
//         case BUSTYPE_SK6812_RGBW:
//           return BUSTYPE__32_RN_4__ID;
//         case BUSTYPE_WS2805_RGBWW:
//           return BUSTYPE__32_RN_5__ID;
//         case BUSTYPE_WS2811_400KHZ:
//           return BUSTYPE__32_RN_400_3__ID;
//         default:
//           return BUSTYPE__NONE__ID;
//       }

//     #elif defined(CONFIG_IDF_TARGET_ESP32S3)
//       // If you want to restrict RMT TX channels on S3 (you previously said 4), do it here:
//       // if (num > 3) return BUSTYPE__NONE__ID;

//       // If you want RMT primary, return RN; otherwise pick I2S family below.
//       #ifdef ENABLE_DEVFEATURE_NEOBUS__RMT_AS_PRIMARY
//         switch (busType) {
//           case BUSTYPE_WS2812_RGB:
//           case BUSTYPE_WS2812_WWA:   return BUSTYPE__32_RN_3__ID;
//           case BUSTYPE_SK6812_RGBW:  return BUSTYPE__32_RN_4__ID;
//           case BUSTYPE_WS2805_RGBWW: return BUSTYPE__32_RN_5__ID;
//           case BUSTYPE_WS2811_400KHZ:return BUSTYPE__32_RN_400_3__ID;
//           default:                   return BUSTYPE__NONE__ID;
//         }
//       #else
//         // Default: single-output I2S family (your X1 typedefs handle S3 internally)
//         switch (busType) {
//           case BUSTYPE_WS2812_RGB:
//           case BUSTYPE_WS2812_WWA:   return BUSTYPE__32_I1_3__ID;
//           case BUSTYPE_SK6812_RGBW:  return BUSTYPE__32_I1_4__ID;
//           case BUSTYPE_WS2805_RGBWW: return BUSTYPE__32_I1_5__ID;
//           case BUSTYPE_WS2811_400KHZ:return BUSTYPE__32_I1_400_3__ID;
//           default:                   return BUSTYPE__NONE__ID;
//         }
//       #endif

//     #elif defined(CONFIG_IDF_TARGET_ESP32S2)
//       // S2: if you want to restrict RMT channels, do it here (optional)
//       // if (num > 3) return BUSTYPE__NONE__ID;

//       #ifdef ENABLE_DEVFEATURE_NEOBUS__RMT_AS_PRIMARY
//         switch (busType) {
//           case BUSTYPE_WS2812_RGB:
//           case BUSTYPE_WS2812_WWA:   return BUSTYPE__32_RN_3__ID;
//           case BUSTYPE_SK6812_RGBW:  return BUSTYPE__32_RN_4__ID;
//           case BUSTYPE_WS2805_RGBWW: return BUSTYPE__32_RN_5__ID;
//           case BUSTYPE_WS2811_400KHZ:return BUSTYPE__32_RN_400_3__ID;
//           default:                   return BUSTYPE__NONE__ID;
//         }
//       #else
//         // S2 uses I2S0 family
//         switch (busType) {
//           case BUSTYPE_WS2812_RGB:
//           case BUSTYPE_WS2812_WWA:   return BUSTYPE__32_I0_3__ID;
//           case BUSTYPE_SK6812_RGBW:  return BUSTYPE__32_I0_4__ID;
//           case BUSTYPE_WS2805_RGBWW: return BUSTYPE__32_I0_5__ID;
//           case BUSTYPE_WS2811_400KHZ:return BUSTYPE__32_I0_400_3__ID;
//           default:                   return BUSTYPE__NONE__ID;
//         }
//       #endif

//     #else
//       // Classic ESP32
//       // If RMT is primary, always return RN family. Channel comes from "num".
//       #ifdef ENABLE_DEVFEATURE_NEOBUS__RMT_AS_PRIMARY
//         switch (busType) {
//           case BUSTYPE_WS2812_RGB:
//           case BUSTYPE_WS2812_WWA:   return BUSTYPE__32_RN_3__ID;
//           case BUSTYPE_SK6812_RGBW:  return BUSTYPE__32_RN_4__ID;
//           case BUSTYPE_WS2805_RGBWW: return BUSTYPE__32_RN_5__ID;
//           case BUSTYPE_WS2811_400KHZ:return BUSTYPE__32_RN_400_3__ID;
//           default:                   return BUSTYPE__NONE__ID;
//         }
//       #else
//         // Otherwise, use your existing auto-detect / parallel policy.
//         // Here’s a sane, minimal policy: I0 for bus 0, I1 for bus 1, then RN thereafter.
//         // If you want parallel I2S selection, put it here, but do NOT encode RMT channel in type.
//         switch (busType) {
//           case BUSTYPE_WS2812_RGB:
//           case BUSTYPE_WS2812_WWA:
//             if (num == 0) return BUSTYPE__32_I0_3__ID;
//             if (num == 1) return BUSTYPE__32_I1_3__ID;
//             return BUSTYPE__32_RN_3__ID;

//           case BUSTYPE_SK6812_RGBW:
//             if (num == 0) return BUSTYPE__32_I0_4__ID;
//             if (num == 1) return BUSTYPE__32_I1_4__ID;
//             return BUSTYPE__32_RN_4__ID;

//           case BUSTYPE_WS2805_RGBWW:
//             if (num == 0) return BUSTYPE__32_I0_5__ID;
//             if (num == 1) return BUSTYPE__32_I1_5__ID;
//             return BUSTYPE__32_RN_5__ID;

//           case BUSTYPE_WS2811_400KHZ:
//             if (num == 0) return BUSTYPE__32_I0_400_3__ID;
//             if (num == 1) return BUSTYPE__32_I1_400_3__ID;
//             return BUSTYPE__32_RN_400_3__ID;

//           default:
//             return BUSTYPE__NONE__ID;
//         }
//       #endif

//     #endif // target select

//   #endif // ESP32/!ESP8266
// }

};

#endif // USE_MODULE_LIGHTS_ANIMATOR

#endif // guard
