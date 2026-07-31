#ifndef _MANIMATORLIGHT_H
#define _MANIMATORLIGHT_H

#include "1_TaskerManager/mTaskerManager.h"

#define D_UNIQUE_MODULE_LIGHTS_ANIMATOR_ID 6003 // [(Folder_Number*100)+ID_File]

#ifdef USE_MODULE_LIGHTS_ANIMATOR

#define Network WiFi

#include "6_Lights/00_Interface/mBusManager.h"

#include "6_Lights/03_Animator/fastled_slim/fastled_slim.h"

#include "dynarray.h"

#if !(defined(WLED_DISABLE_PARTICLESYSTEM2D) && defined(WLED_DISABLE_PARTICLESYSTEM1D))
  #include "ParticleSystem.h" // include particle system code only if at least one system is enabled
  #ifdef WLED_DISABLE_PARTICLESYSTEM2D
    #define WLED_PS_DONT_REPLACE_2D_FX
  #endif
  #ifdef WLED_DISABLE_PARTICLESYSTEM1D
    #define WLED_PS_DONT_REPLACE_1D_FX
  #endif
  #ifdef ESP8266
    #if !defined(WLED_DISABLE_PARTICLESYSTEM2D) && !defined(WLED_DISABLE_PARTICLESYSTEM1D)
      #error ESP8266 does not support 1D and 2D particle systems simultaneously. Please disable one of them.
    #endif
  #endif
#else
  #define WLED_PS_DONT_REPLACE_1D_FX
  #define WLED_PS_DONT_REPLACE_2D_FX
#endif
#ifdef WLED_PS_DONT_REPLACE_FX
  #define WLED_PS_DONT_REPLACE_1D_FX
  #define WLED_PS_DONT_REPLACE_2D_FX
#endif


#include "prng.h"
#include "2_CoreSystem/07_Time/Toki.h"

#include "DynamicBuffer.h"

#include "fcn_declare.h"

#define inoise8 perlin8   // fastled legacy alias
#define inoise16 perlin16 // fastled legacy alias


// #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
// #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Should nearly always be enabled as default/minimal cases
// #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
// #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
// #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
// #define ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
// #define ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__BORDER_WALLPAPERS
// #define ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__SUN_POSITIONS
// #define ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__NOTIFICATIONS

#ifdef ESP32
#define PIXEL_RANGE_LIMIT 4000
#else
#define PIXEL_RANGE_LIMIT 1000
#endif 


#ifndef BUSCONFIG_MAX_PINS_FOR_PARALLEL_I2S
#define BUSCONFIG_MAX_PINS_FOR_PARALLEL_I2S 300
#endif 


// Temporary fixing for neopixelbusLg issue
#ifndef PHASEIN_ANIM_BRIGHTNESS_REQUIRED_AS_TRUE
#define  false
#error "dont be default yet"
#endif

#define ALOG_COL32(c,i) ALOG_INF(PSTR("%d c %d,%d,%d,%d"), i, R(c), G(c), B(c), W(c));

#ifdef ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
  #ifndef PIXEL_COUNTS
    #define PIXEL_COUNTS DEFAULT_LED_COUNT
  #endif
  #ifndef DATA_PINS
    #define DATA_PINS LEDPIN
  #endif
  #define DATA_PINS_BUSSES 23, 22
  #ifndef DEFAULT_LED_TYPE
    #define DEFAULT_LED_TYPE BUSTYPE_WS2812_RGB
  #endif
  #ifndef DEFAULT_LED_COLOR_ORDER
    #define DEFAULT_LED_COLOR_ORDER COL_ORDER_GRB  //default to GRB
  #endif
#endif // ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT


// ─────────────────────────────────────────────────────────────────────────────
// Palette wrap behaviour flag (used by *_ModeWrap APIs)
//
// Concept:
//   A palette lookup takes an 8-bit index (0..255). Some effects want the
//   gradient to be *circular* (255 blends back into 0), others want a *hard*
//   stop at the end (255 is the end, no blend back to 0).
//
// Legacy (WLED):
//   WLED’s color_from_palette(..., wrap=true/false) used a boolean “wrap”.
//     - wrap=true  → indices wrap around; the gradient is circular
//     - wrap=false → indices clamp at ends; no circular blend
//
// Legacy (our code, older names):
//   PALETTE_WRAP_ON  == circular/smooth wrap
//   PALETTE_WRAP_OFF == hard edge / no wrap
//
// Current names:
//   PALETTE_WRAP_SMOOTH   → circular wrap (255→0 blends)
//   PALETTE_WRAP_HARDEDGE → no wrap (clamp at the ends)
// ─────────────────────────────────────────────────────────────────────────────
#define PALETTE_WRAP_SMOOTH     true   // circular wrap: 255 blends to 0 (full 0..255)
                                       // - Best for continuous/looping gradients
                                       // - Equivalent to WLED wrap=true (legacy PALETTE_WRAP_ON)
                                       // - Visual: no “seam” where the gradient restarts

#define PALETTE_WRAP_HARDEDGE   false  // hard edge: clamp at 0 and 255, no blend back to start
                                       // - Best for one-way ramps or when you *don’t* want 255→0 mixing
                                       // - Equivalent to WLED wrap=false (legacy PALETTE_WRAP_OFF)
                                       // - Tip: If you want to avoid touching the 255→0 seam entirely,
                                       //        pre-scale indices to 0..240 upstream (keeps a margin)

// ─────────────────────────────────────────────────────────────────────────────
// PALETTE_INDEX ENCODING TYPES
//
// What does your per-pixel “index” represent before the palette lookup?
//   - 0: The index is already in the palette’s native 0..255 range (WLED default).
//   - 1: The index was derived from SEGLEN (e.g., i in 0..SEGLEN-1) and needs
//        scaling to 0..255 inside the palette function (WLED compatible path).
//   - 2: Request a *precise* palette tap at a specific index without any mapping
//        (useful for fixed swatches/markers).
//
// Legacy (our code):
//   PALETTE_INDEX__IS_SEGLEN_RANGE used to be PALETTE_INDEX_SPANS_SEGLEN_ON
//   PALETTE_INDEX__IS_EXACT_COLOUR used to be PALETTE_INDEX_IS_INDEX_IN_PALETTE
//
// Legacy (WLED):
//   WLED generally treats the palette index as 0..255. Many effects compute
//   i→(i*256/SEGLEN) themselves; this enum lets us centralize that behavior.
// ─────────────────────────────────────────────────────────────────────────────
#define PALETTE_INDEX__IS_255_RANGE       0  // “Raw 0..255” (WLED default semantics).
                                             // - Use when you already computed an 8-bit palette index.
                                             // - No internal scaling is applied.

#define PALETTE_INDEX__IS_SEGLEN_RANGE    1  // “Index spans SEGLEN; please scale to 0..255”.
                                             // - Input i in [0..SEGLEN-1] will be mapped to [0..255].
                                             // - WLED-compatible for effects that used segment length
                                             //   as their domain (saves you from manual scaling).

#define PALETTE_INDEX__IS_EXACT_COLOUR    2  // “Exact tap” into the palette (no scaling or wrapping).
                                             // - Use for fixed palette slot access (e.g., swatches).
                                             // - Caller guarantees the index semantics; no automatic
                                             //   remap or safety applied.

// ─────────────────────────────────────────────────────────────────────────────
// PALETTE_MODE — how to interpret the palette when sampling
//
// Palettes can be treated as continuous *gradients* (interpolated) or as
// *discrete* steps (nearest entries only). WLED historically exposed this via
// palette blending settings (e.g., “linear” vs “none/discrete”). Here we make
// it explicit per lookup when needed.
//
//   DEFAULT         → Use the palette’s native style (respect its definition)
//   FORCE_DISCRETE  → Stepwise; no interpolation between entries
//   FORCE_GRADIENT  → Force interpolation even if the palette is discrete
//
// Legacy (our code):
//   DEFAULT used to be PALETTE_DISCRETE_OFF
//   FORCE_DISCRETE used to be PALETTE_DISCRETE_ON
// ─────────────────────────────────────────────────────────────────────────────
#define PALETTE_MODE__DEFAULT          0  // Use palette as defined (gradient or discrete).
                                          // - Honors the palette’s own interpolation metadata.
                                          // - Closest to WLED’s normal behavior given palette & blend mode.

#define PALETTE_MODE__FORCE_DISCRETE   1  // Force discrete steps; no in-between blending.
                                          // - Good for “pixelated”/posterized looks, or when you want
                                          //   exact banding without soft transitions.
                                          // - In WLED terms, similar to “no palette blending”.

#define PALETTE_MODE__FORCE_GRADIENT   2  // Force gradient interpolation between entries.
                                          // - Smooths out discrete palettes.
                                          // - In WLED terms, akin to “linear palette blending”.


/**
 * @brief 1D and 2D level of development
 * Show the max level of devstage only when 2D is active, otherwise, ignore the 2D level
 */
#ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
#define EFFECT_DEVSTAGE_12D_CHECK(X,Y) max(X,Y) // Take the highest devstage
#else
#define EFFECT_DEVSTAGE_12D_CHECK(X,Y) X // Take only the 1D devstage, ignore the 2D devstage.
#endif

// ─────────────────────────────────────────────────────────────────────────────
// Legacy-to-new compatibility shims
// Keep old flags compiling while everything migrates to the new enums.
// ─────────────────────────────────────────────────────────────────────────────

// OLD: PALETTE_SPAN_OFF  (commented as PALETTE_INDEX_IS_INDEX_IN_PALETTE)
// Meaning used in your codebase: “use the palette index *as-is*, don’t remap
// by SEGLEN; I want the exact palette tap I specified.”
// → Map to our explicit “exact tap” encoding.
#define PALETTE_SPAN_OFF                PALETTE_INDEX__IS_EXACT_COLOUR
// (If any callsites actually meant “0..255 raw” instead of a literal slot,
//  change those sites to PALETTE_INDEX__IS_255_RANGE; this alias assumes the
//  stricter ‘exact colour’ semantics you documented.)

// OLD: WLED_PALETTE_MAPPING_ARG_FALSE
// WLED’s color_from_palette(i, mapping=bool, ...) used `mapping=false` to say:
// “index is already 0..255; don’t scale i by SEGLEN.”
// → That’s our 0..255 raw path.
#define WLED_PALETTE_MAPPING_ARG_FALSE  PALETTE_INDEX__IS_255_RANGE
// (If you still have a TRUE case somewhere, use PALETTE_INDEX__IS_SEGLEN_RANGE.)

// OLD: PALETTE_SOLID_WRAP (was `(paletteBlend == 1 || paletteBlend == 3)`)
// Historically: if palette blending mode implied continuous gradients, allow
// circular wrap (255→0). In the new API, wrap is explicit.
// → Default this legacy macro to SMOOTH wrap; callers that need hard edges
//   should pass PALETTE_WRAP_HARDEDGE directly.
#define PALETTE_SOLID_WRAP              PALETTE_WRAP_SMOOTH

// ─────────────────────────────────────────────────────────────────────────────
// Sentinel / state flags
// These are used as *self-documenting booleans* or *sentinel values* in APIs,
// instead of passing raw true/false or nullptr.
// ─────────────────────────────────────────────────────────────────────────────

#define NO_ENCODED_VALUE                nullptr
// Meaning: No encoded colour/index/value provided.
// Use when an API normally takes a pointer to encoded data but you want to
// signal “none” (i.e. default handling). Equivalent to NULL in older code.
// WLED: not explicit — this is a project-specific helper.

#define SET_BRIGHTNESS                  true
// Meaning: Apply brightness scaling inside this API call.
// Used to avoid passing a “magic true” — name makes intent explicit.

#define BRIGHTNESS_ALREADY_SET          true
#define BRIGHTNESS_NOT_YET_SET          false
// Distinguishes code paths that *already applied brightness* from those
// that have not yet. Lets helpers skip a redundant scale.
// Legacy: WLED often scaled brightness at the final stage; you’re
// separating “did we apply it yet?” into explicit flags.

#define WITH_BRIGHTNESS_APPLIED         true
// Used in signatures where you pass an “apply brightness?” argument.
// Semantically overlaps with SET_BRIGHTNESS; keep whichever form
// makes the callsite most readable (e.g., `DrawPixel(...,WITH_BRIGHTNESS_APPLIED)`).


#define RgbwwColorU32(c)  RGBW32(c.R,c.G,c.B,c.WW) 



// #define FLASH_COUNT 4 
#define LED_SKIP_AMOUNT  0
// #define MIN_SHOW_DELAY  15
#ifndef MIN_SHOW_DELAY
#define MIN_SHOW_DELAY   20//(_frametime < 16 ? 8 : 15) // minimum show delay based on frametime. <10ms will cause flicker, especially on parallel methods
#endif
#define DEFAULT_LED_COUNT 100


#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))


#define WLED_FPS         42
#define FRAMETIME_FIXED  (1000/WLED_FPS)
#if defined(ARDUINO_ARCH_ESP32) && !defined(CONFIG_IDF_TARGET_ESP32C3) && !defined(CONFIG_IDF_TARGET_ESP32S2)
  #define MIN_FRAME_DELAY  2                                              // minimum wait between repaints, to keep other functions like WiFi alive 
#elif defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32C3)
  #define MIN_FRAME_DELAY  3                                              // S2/C3 are slower than normal esp32, and only have one core
#else
  #define MIN_FRAME_DELAY  8                                              // 8266 legacy MIN_SHOW_DELAY
#endif

// Primary frame time (ms) for most animated effects.
// In practice this sets a ~40 FPS update rate (1000/25 = 40).
#define FRAMETIME             25

// Static effects (no animation) — update once per second.
#define FRAMETIME_STATIC      1000

// Special marker for "no FPS limit" (effect runs as fast as possible).
#define FPS_UNLIMITED         0

//       /**
//        * @brief Temporary fix to enable phasing out of animator
// I believe this is to be phased out
//        **/
#define USE_ANIMATOR 0 // tmp fix to return as zero, to enable the effect call to keep the animator running


// FPS calculation (can be defined as compile flag for debugging)
#ifndef FPS_CALC_AVG
#define FPS_CALC_AVG 7 // average FPS calculation over this many frames (moving average)
#endif
#ifndef FPS_MULTIPLIER
#define FPS_MULTIPLIER 1 // dev option: multiplier to get sub-frame FPS without floats
#endif
#define FPS_CALC_SHIFT 7 // bit shift for fixed point math



extern bool realtimeRespectLedMaps; // used in getMappedPixelIndex()

/* Each segment uses 52 bytes of SRAM memory, so if you're application fails because of insufficient memory, decreasing MAX_NUM_SEGMENTS may help */
#ifdef ESP8266
  #define MAX_NUM_SEGMENTS    3
  /* How much data bytes all segments combined may allocate */
  #define MAX_SEGMENT_DATA  1000
#else
  #ifndef MAX_NUM_SEGMENTS
    #define MAX_NUM_SEGMENTS  6
  #endif
  #define MAX_SEGMENT_DATA  42767
#endif

#define IBN 5100

/* How much data bytes each segment should max allocate to leave enough space for other segments,
  assuming each segment uses the same amount of data. 256 for ESP8266, 640 for ESP32. */
#define FAIR_DATA_PER_SEG (MAX_SEGMENT_DATA / getMaxSegments())

#define indexToVStrip(index, stripNr) ((index) | (int((stripNr)+1)<<16))

// a few constants needed for AudioReactive effects
// for 22Khz sampling
#define MAX_FREQUENCY   11025    // sample frequency / 2 (as per Nyquist criterion)
#define MAX_FREQ_LOG10  4.04238f // log10(MAX_FREQUENCY)
// for 20Khz sampling
//#define MAX_FREQUENCY   10240
//#define MAX_FREQ_LOG10  4.0103f
// for 10Khz sampling
//#define MAX_FREQUENCY   5120
//#define MAX_FREQ_LOG10  3.71f

/* How much data bytes each segment should max allocate to leave enough space for other segments,
  assuming each segment uses the same amount of data. 256 for ESP8266, 640 for ESP32. */
// #define FAIR_DATA_PER_SEG (MAX_SEGMENT_DATA / strip.getMaxSegments())

// options
// bit    7: segment is in transition mode
// bits 4-6: TBD
// bit    3: mirror effect within segment
// bit    2: segment is on
// bit    1: reverse segment
// bit    0: segment is selected
// #define NO_OPTIONS   (uint8_t)0x00
// #define TRANSITIONAL (uint8_t)0x80
// #define MIRROR       (uint8_t)0x08
#define SEGMENT_ON   (uint8_t)0x04
// #define REVERSE      (uint8_t)0x02
#define SELECTED     (uint8_t)0x01
// #define IS_TRANSITIONAL ((SEGMENT_I(segment_index).options & TRANSITIONAL) == TRANSITIONAL)
// #define IS_MIRROR       ((SEGMENT_I(segment_index).options & MIRROR      ) == MIRROR      )
// #define IS_SEGMENT_ON   ((SEGMENT_I(segment_index).options & SEGMENT_ON  ) == SEGMENT_ON  )
// #define IS_REVERSE      ((SEGMENT_I(segment_index).options & REVERSE     ) == REVERSE     )
// #define IS_SELECTED     ((SEGMENT_I(segment_index).options & SELECTED    ) == SELECTED    )

/**
 * SECTION: Set defaults for segments
 * 
 */

#define FLASH_COUNT 4 
#define LED_SKIP_AMOUNT  0
// #define MIN_SHOW_DELAY  15
#define DEFAULT_LED_COUNT 30

#define DEFAULT_BRIGHTNESS (uint8_t)127
#define DEFAULT_MODE       (uint8_t)0
#define DEFAULT_SPEED      (uint8_t)128
#define DEFAULT_INTENSITY  (uint8_t)128
#define DEFAULT_COLOR      (uint32_t)0xFFAA00
#define DEFAULT_C1         (uint8_t)128
#define DEFAULT_C2         (uint8_t)128
#define DEFAULT_C3         (uint8_t)16



// ─────────────────────────────────────────────────────────────────────────────
// Segment colour accessors
// ─────────────────────────────────────────────────────────────────────────────

// Return color (as u32) from the current segment’s colour slot [x].
#define SEGCOLOR_U32(x)        segments[getCurrSegmentId()].segcol[x].getU32()

// Return color (with brightness scaling applied) from the current segment’s colour slot [x].
#define SEGCOLOR_RGBCCT(x)     segments[getCurrSegmentId()].segcol[x].WithBrightness()

// Unified SEGCOLOR macro — returns u32 (matches old SEGCOLOR_U32).
// Legacy note: previously there was also .getU32Raw().
#define SEGCOLOR(x)            segments[getCurrSegmentId()].segcol[x].colour  //.getU32()

// Pointer-context version (used with tkr_anim).
#define pSEGCOLOR(x)           pSEGMENT.segcol[x].colour                      // getU32()


// ─────────────────────────────────────────────────────────────────────────────
// Segment object accessors
// ─────────────────────────────────────────────────────────────────────────────

// Current segment (main handle).
#define SEGMENT                segments[getCurrSegmentId()]
#define SEGENV SEGMENT

// Current segment in pointer-context (via tkr_anim).
#define pSEGMENT               tkr_anim->segments[tkr_anim->getCurrSegmentId()]

// Indexed segment by ID (raw array access).
// NOTE: Consider refactoring to a getSegment(X) helper for bounds safety.
#define SEGMENT_I(X)           segments[X]

// Indexed segment by ID in pointer-context.
#define pSEGMENT_I(X)          tkr_anim->segments[X]


// ─────────────────────────────────────────────────────────────────────────────
// Segment length / geometry
// ─────────────────────────────────────────────────────────────────────────────

// Virtual segment length (set externally before effect execution).
#define SEGLEN                 SEGMENT.vLength() //_virtualSegmentLength

// Pointer-context virtual segment length.
#define pSEGLEN                pSEGMENT.vLength() //tkr_anim->Segment::vLength() //_virtualSegmentLength

// Segment width/height helpers.
#define SEG_W                  segments[getCurrSegmentId()].vWidth()
#define SEG_H                  segments[getCurrSegmentId()].vHeight()




// ─────────────────────────────────────────────────────────────────────────────
// Palette accessors
// ─────────────────────────────────────────────────────────────────────────────

// Current segment’s palette data (CRGB16Palette16).
#define SEGPALETTE             SEGMENT.palette_loaded->CRGB16Palette16_Palette.data

// Pointer-context palette data.
#define pSEGPALETTE            pSEGMENT.palette_loaded->CRGB16Palette16_Palette.data


// ─────────────────────────────────────────────────────────────────────────────
// Index helpers
// ─────────────────────────────────────────────────────────────────────────────

// Current segment index.
#define SEGIDX                 getCurrSegmentId()



//Segment option byte bits
#define SEG_OPTION_SELECTED       0
#define SEG_OPTION_REVERSED       1
#define SEG_OPTION_ON             2
#define SEG_OPTION_MIRROR         3            //Indicates that the effect will be mirrored within the segment
#define SEG_OPTION_NONUNITY       4            //Indicates that the effect does not use FRAMETIME_MS or needs getPixelColor
#define SEG_OPTION_TRANSITIONAL   7


#define SPEED_FORMULA_L  5U + (50U*(255U - SEGMENT.speed))/SEGLEN

// Macro to calculate frametime based on SEGLEN and max time in milliseconds (speed = 0 gives max time)
#define FRAMETIME_WITH_SPEED_MAX_MS(max_ms) (5U + (max_ms * (255U - SEGMENT.speed)) / SEGLEN)

// Macro to calculate frametime with a speed-dependent range between min and max times (in milliseconds)
#define FRAMETIME_WITH_SPEED(min_ms, max_ms) \
    (min_ms + ((max_ms - min_ms) * (255U - SEGMENT.speed)) / 255U)


#include "6_Lights/02_Palette/mPalette_Progmem.h"
#include "6_Lights/02_Palette/mPalette.h"
#include "6_Lights/02_Palette/mPaletteLoaded.h"

#include "6_Lights/00_Interface/mInterfaceLight.h"

#ifdef ESP32
  #include <WiFi.h>
  #ifndef DISABLE_NETWORK
  #ifdef USE_MODULE_NETWORK_WEBSERVER
    #include <AsyncTCP.h>
    #include <ESPAsyncWebServer.h>
    #include <SPIFFSEditor.h>
  #endif // USE_MODULE_NETWORK_WEBSERVER
  #endif // DISABLE_NETWORK
#elif defined(ESP8266)
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  #endif // USE_MODULE_NETWORK_WEBSERVER
#endif

#include <DNSServer.h>

#include "math.h"
#include "fastmath.h"

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#include "6_Lights/02_Palette/mPaletteLoaded.h"
class mPaletteLoaded;

#define FASTLED_INTERNAL // suppress pragma warning messages
// #include "6_Lights/00_Interface/FastLED/FastLED.h"


#ifndef WLED_DEFINE_GLOBAL_VARS
  // #define extern
  #define _INIT(x)
  #define _INIT_N(x)
  #define _INIT_PROGMEM(x)
#else
  #define WLED_GLOBAL
  #define _INIT(x) = x
  //needed to ignore commas in array definitions
  #define UNPACK( ... ) __VA_ARGS__
  #define _INIT_N(x) UNPACK x
  #define _INIT_PROGMEM(x) PROGMEM = x
#endif


DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__SEGMENTS_CTR)       "segments";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__MATRIX_CTR)       "matrix";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__PLAYLISTS_CTR)              "playlists";
#ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__MODE_AMBILIGHT__CTR)        "mode_ambilight";
#endif
#ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__MODE_MANUAL_SETPIXEL_CTR)   "mode_setpixel";
#endif
#ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__AUTOMATION_PRESETS_CTR)   "presets";
#endif
#ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PLAYLISTS
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__AUTOMATION_PLAYLISTS_CTR)   "playlists";
#endif
#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_PALETTE__CTR)         "debug/palette";
#endif
#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_CUSTOM_MAPPING_TABLE
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_CUSTOM_MAPPING_TABLE__CTR)        "debug/mapping_table";
#endif
#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_SEGMENTS__CTR)        "debug/segments";
#endif 
#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_PALETTE_VECTOR__CTR)        "debug/palette_vector";
#endif 
#ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__ANIMATIONS_PROGRESS_CTR)    "debug/animation_progress";
#endif 
#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PERFORMANCE
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_PERFORMANCE__CTR)        "debug/performance";
#endif 


#define ARDUINOJSON_DECODE_UNICODE 0
#include "3_Network/21_WebServer/ArduinoJson-v6.h"


#ifdef ENABLE_FEATURE_LIGHTING__WEBUI
  #include "3_Network/21_WebServer/AsyncJson-v6.h"
  #include "webpages_generated/html_ui.h"
  #ifdef WLED_ENABLE_SIMPLE_UI
    #include "webpages_generated/html_simple.h"
  #endif
  #include "webpages_generated/html_settings.h"
  #include "webpages_generated/html_other.h"
  #ifdef WLED_ENABLE_PIXART
    #include "webpages_generated/html_pixart.h"
  #endif
  #ifndef WLED_DISABLE_PXMAGIC
    #include "webpages_generated/html_pxmagic.h"
  #endif
  #include "webpages_generated/html_cpal.h"
#endif // ENABLE_FEATURE_LIGHTING__WEBUI

#include "colors.h"


int16_t sin16_t(uint16_t theta);
int16_t cos16_t(uint16_t theta);
uint8_t sin8_t(uint8_t theta);
uint8_t cos8_t(uint8_t theta);
float sin_approx(float theta); // uses integer math (converted to float), accuracy +/-0.0015 (compared to sinf())
float cos_approx(float theta);
float tan_approx(float x);
float atan2_t(float y, float x);
float acos_t(float x);
float asin_t(float x);
template <typename T> T atan_t(T x);
float floor_t(float x);
float fmod_t(float num, float denom);
#define sin_t sin_approx
#define cos_t cos_approx
#define tan_t tan_approx

#include <functional>
#define ANIM_FUNCTION_SIGNATURE                             std::function<void(const AnimationParam& param)>                              anim_function_callback
#define ANIMIMATION_DEBUG_MQTT_FUNCTION_SIGNATURE           std::function<void()>                                                         anim_progress_mqtt_function_callback
#define ANIM_FUNCTION_SIGNATURE_SEGMENT_INDEXED             std::function<void(const uint8_t segment_index, const AnimationParam& param)> anim_function_callback_indexed
// #define SET_DIRECT_MODE()                                   SEGMENT.anim_function_callback = nullptr 
// #define DIRECT_MODE(x)                                      SEGMENT.anim_function_callback = nullptr; SEGMENT.cycle_time__rate_ms = x;

#define SET_DIRECT_MODE()                                   //SEGMENT.anim_function_callback = nullptr 
#define DIRECT_MODE(x)                                      //SEGMENT.anim_function_callback = nullptr; SEGMENT.cycle_time__rate_ms = x;



#ifdef ESP8266
#define HW_RND_REGISTER RANDOM_REG32
#else // ESP32 family
#include "soc/wdev_reg.h"
#define HW_RND_REGISTER REG_READ(WDEV_RND_REG)
#endif

#define EFFECT_DEFAULT() EffectAnim__Static_Palette()
#define FX_FALLBACK_STATIC EffectAnim__Static_Palette()

class mAnimatorLight :
  public mTaskerInterface
{

  public:
  /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mAnimatorLight(){};
    void   Init(void);
    void   Pre_Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    #if defined(ENABLE_DEBUGFEATURE_TASKER__DEVELOPMENT_TASKS__ANIMATOR) && defined(ENABLE_DEBUGFEATURE_TASKER__DEVELOPMENT_TASKS)
    int8_t Tasker_DevCode(uint8_t function, JsonParserObject obj = 0);
    #endif
    void   parse_JSONCommand(JsonParserObject obj);
    
    static constexpr const char* PM_MODULE_LIGHTS_ANIMATOR_CTR = D_MODULE_LIGHTS_ANIMATOR_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_LIGHTS_ANIMATOR_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_LIGHTS_ANIMATOR_ID; }
    
    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    struct DATA_STORAGE
    {

    }
    dt;


    struct DATA_RUNTIME
    {

    }
    rt;

    byte errorFlag = 0;
    byte briS                = 128;//(128);           // default brightness
    // byte bri                 = 128;//(briS);          // global brightness (set)
    byte briOld              = 0;             // global brightness while in transition loop (previous iteration)
    byte briT                = 0;             // global brightness during transition
    byte briLast             = 128;//(128);           // brightness before turned off. Used for toggle function
    byte whiteLast           = 128;//(128);           // white channel before turned off. Used for toggle function in ir.cpp




    bool doInitBusses = false; // debug
    bool     doSerializeConfig = false; // debug
    int8_t loadLedmap = -1;
    uint8_t currentLedmap = 0;
    uint16_t desired_pixel;

    #ifdef ENABLE_DEBUGFEATURE_LIGHTING__EFFECT_LOOP_TIME_SERIAL
    uint32_t tSaved_LoopTime = millis();
    #endif

    void Load_Module(bool erase);
    void Save_Module(void);
    bool Default_Module(void);
    void FileSystem_JsonAppend_Save_Module();

    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/

    #define STRINGIFY(X) #X
    #define TOSTRING(X) STRINGIFY(X)

    // #define WLED_VERSION 0.12.0
    #ifndef WLED_RELEASE_NAME
      #define WLED_RELEASE_NAME "Custom"
    #endif

    char versionString[16] = TOSTRING(PROJECT_VERSION);
    char releaseString[7] = WLED_RELEASE_NAME; // must include the quotes when defining, e.g -D WLED_RELEASE_NAME=\"ESP32_MULTI_USREMODS\"





    void reset(); // tmp use wled reboot, later make sure to use the system reboot method
            
    #ifdef ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    byte correctionRGB[4] = {0,0,0,0};
    uint16_t lastKelvin = 0;
    static void colorKtoRGB(uint16_t kelvin, byte* rgb);
    uint32_t colorBalanceFromKelvin(uint16_t kelvin, uint32_t rgb);
    static uint16_t approximateKelvinFromRGB(uint32_t rgb);
    #endif // ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT

    unsigned long presetsModifiedTime = 0;
    const char *getPresetsFileName(bool persistent = true);

    void EveryLoop();    
     
    void EverySecond_AutoOff();
    void BootMessage();

    #ifdef ENABLE_FEATURE_LIGHTS__KEY_INPUT_CONTROLS
    // This should be made into a custom controller
    struct KeyInput_ControlLights_s{
      /**
       * @brief 
       * 0: Disabled
       * 1: Single button demo/tester
       * 2: Single button multifunction controller
       * 3: Two button controler (mode, brightness)
       */
      uint8_t mode = 1; 
      uint8_t demo_state = 0;
    }keyinput_control;
    void KeyInput__ControlLights();
    #endif
    #ifdef ENABLE_FEATURE_LIGHTS__DEMO_MODE
    void SubTask_Demo();
    #endif
                
    static int DebugFindPixelPeriod(const uint32_t* pixels, size_t length)
{
  if (!pixels || length < 2) return 0;

  for (size_t period = 1; period <= 64 && period < length; period++)
  {
    bool matches = true;

    for (size_t i = period; i < length; i++)
    {
      if (pixels[i] != pixels[i % period])
      {
        matches = false;
        break;
      }
    }

    if (matches) return period;
  }

  return 0;
}
    

    /************************************************************************************************
     * SECTION: Modes
     ************************************************************************************************/
    enum ANIMATION_MODE
    {    
      ANIMATION_MODE__EFFECTS = 0, // Not realtime
      ANIMATION_MODE__REALTIME_MQTT_SETPIXEL, // Using json encoded message to set the pixels directly
      ANIMATION_MODE__REALTIME_UDP,
      ANIMATION_MODE__REALTIME_HYPERION,
      ANIMATION_MODE__REALTIME_E131,
      ANIMATION_MODE__REALTIME_ADALIGHT,
      ANIMATION_MODE__REALTIME_ARTNET,
      ANIMATION_MODE__REALTIME_TPM2NET,
      ANIMATION_MODE__REALTIME_DDP,  
      ANIMATION_MODE__INTERNAL_CONTROL_FROM_ANOTHER_MODULE, // eg controller modules doing direct setPixelColor
      ANIMATION_MODE__LENGTH_ID
    };   
    #ifdef ENABLE_FEATURE_LIGHTING__REALTIME_MODES 
    void SubTask_RealTime_SetPixel();
    int8_t GetAnimationModeIDbyName(const char* c);
    const char* GetAnimationModeName(char* buffer, uint16_t buflen);
    const char* GetAnimationModeNameByID(uint8_t id, char* buffer, uint16_t buflen);
    void CommandSet_AnimationModeID(uint8_t value);
    #endif // ENABLE_FEATURE_LIGHTING__REALTIME_MODES


    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/

    void subparse_JSONCommand(JsonParserObject obj, uint8_t segment_index = 0);    
    
    #ifdef ENABLE_DEVFEATURE_LIGHTING__COMMANDS_NOTIFICATION_SHORTCUT
    void parsesub_json_object_notification_shortcut(JsonParserObject obj);
    #endif

    void CommandSet_CustomPalette(uint8_t index, uint16_t encoding, uint8_t* data, uint8_t data_length);
   
    void CommandSet_Effect_ColourTypeID(uint8_t id, uint8_t segment_index = 0);

    #ifdef ENABLE_DEVFEATURE_LIGHTING__COLOURHEATMAP_PALETTE
    void CommandSet_ColourHeatMap_Palette(float* array_val, uint8_t array_length, uint8_t style_index = 0, uint8_t palette_id = 255);
    #endif

    void CommandSet_PaletteID(uint16_t value, uint8_t segment_index = 0);
    void CommandSet_Palette2ID(uint16_t value, uint8_t segment_index = 0);

    void CommandSet_Flasher_FunctionID(uint8_t value, uint8_t segment_index = 0);
    int16_t GetFlasherFunctionIDbyName(const char* f);
    const char* GetFlasherFunctionName(char* buffer, uint8_t buflen, uint8_t segment_index = 0);
    const char* GetFlasherFunctionNamebyID(uint8_t id, char* buffer, uint8_t buflen, bool return_first_option_if_not_found = false);

    void CommandSet_Flasher_UpdateColourRegion_RefreshSecs(uint8_t value, uint8_t segment_index = 0);

    void CommandSet_HardwareColourOrderTypeByStr(const char* value, uint8_t segment_index = 0);
    void CommandSet_ColourTypeByStr(const char* value, uint8_t segment_index = 0);
    void CommandSet_Animation_Transition_Rate_Ms(uint16_t value, uint8_t segment_index= 0);
  
    void CommandSet_Effect_Intensity(uint8_t value, uint8_t segment_index = 0);
    void CommandSet_Effect_Speed(uint8_t value, uint8_t segment_index = 0);
    
    
    void CommandSet_SegColour_RgbcctColour_Hue_360(uint16_t hue_new, uint8_t colour_index = 0, uint8_t segment_index = 0);
    void CommandSet_SegColour_RgbcctColour_Sat_255(uint8_t sat_new , uint8_t colour_index = 0, uint8_t segment_index = 0);
    void CommandSet_SegColour_RgbcctColour_ColourTemp_Kelvin(uint16_t ct, uint8_t colour_index = 0, uint8_t segment_index = 0);
    void CommandSet_SegColour_RgbcctColour_BrightnessRGB(uint8_t brightness, uint8_t colour_index = 0, uint8_t segment_index = 0);
    void CommandSet_SegColour_RgbcctColour_BrightnessCCT(uint8_t brightness, uint8_t colour_index = 0, uint8_t segment_index = 0);


    uint8_t GetNumberOfColoursInUNLOADEDPalette(uint16_t palette_id);

    // 1:1 replacement of fastled function optimized for ESP, slightly faster, more accurate and uses less flash (~ -200bytes)
    uint32_t ColorFromPaletteCRGB16Fast(const CRGBPalette16& pal, unsigned index, uint8_t brightness, TBlendType blendType) {
      if (blendType == LINEARBLEND_NOWRAP) {
        index = (index * 0xF0) >> 8; // Blend range is affected by lo4 blend of values, remap to avoid wrapping
      }
      unsigned hi4 = byte(index) >> 4;
      unsigned lo4 = (index & 0x0F);
      const CRGB* entry = (CRGB*)&(pal[0]) + hi4;
      unsigned red1   = entry->r;
      unsigned green1 = entry->g;
      unsigned blue1  = entry->b;
      if (lo4 && blendType != NOBLEND) {
        if (hi4 == 15) entry = &(pal[0]);
        else ++entry;
        unsigned f2 = (lo4 << 4);
        unsigned f1 = 256 - f2;
        red1   = (red1   * f1 + (unsigned)entry->r * f2) >> 8; // note: using color_blend() is slower
        green1 = (green1 * f1 + (unsigned)entry->g * f2) >> 8;
        blue1  = (blue1  * f1 + (unsigned)entry->b * f2) >> 8;
      }
      if (brightness < 255) { // note: zero checking could be done to return black but that is hardly ever used so it is omitted
        // actually same as color_fade(), using color_fade() is slower
        uint32_t scale = brightness + 1; // adjust for rounding (bitshift)
        red1   = (red1   * scale) >> 8;
        green1 = (green1 * scale) >> 8;
        blue1  = (blue1  * scale) >> 8;
      }
      return RGBW32(red1,green1,blue1,0);
    }
    
    // WLED-compat wrapper: ignores 'pal' and uses SEGMENT.palette_id via SEGMENT.GetPaletteColour()
    // Keeps WLED call-sites working: ColorFromPalette(SEGPALETTE, idx, bri, blend)
    inline uint32_t ColorFromPaletteRedirect(
      const CRGBPalette16& /*pal_ignored*/,
      uint8_t index,
      uint8_t brightness = 255,
      TBlendType blendType = NOBLEND
    ){

      // WARNING: Forced fast redirect
      // Bypassing complex palette handling to improve performance. Will only work on already loaded CRGB16Palettes 
      // return ColorFromPaletteCRGB16Fast(SEGMENT.palette_loaded->CRGB16Palette16_Palette.data, index, brightness, blendType);

      // Map WLED blend to your palette mode
      const uint8_t force_mode = (blendType == NOBLEND) 
                                 ? PALETTE_MODE__FORCE_DISCRETE
                                 : PALETTE_MODE__FORCE_GRADIENT;

      // Pull from segment-selected palette (auto-load happens inside GetPaletteColour)
      uint32_t c = SEGMENT.GetPaletteColour(
          index,                      // 0-255
          PALETTE_INDEX__IS_255_RANGE, // WLED-style indexing
          force_mode,
          PALETTE_WRAP_HARDEDGE,       // keep consistent with your current shim usage
          nullptr,
          /*apply_brightness*/ false,  // IMPORTANT: wrapper handles WLED 'brightness'
          255, // Brightness at maximum for now, handled below
          0
      );

      // Apply WLED brightness argument (independent of seg/global brightness)
      // The brightness is built into the "GetPaletteColour" function above, so this can likely be removed by simply passing pbri
      if (brightness < 255) {
        const uint16_t scale = uint16_t(brightness) + 1;
        c = RGBW32(
          (R(c) * scale) >> 8,
          (G(c) * scale) >> 8,
          (B(c) * scale) >> 8,
          (W(c) * scale) >> 8
        );
      }

      return c;
    }

    /******************************************************************************************************************************************************************************
    **** Pixel buffers ***************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/
    
    void updatePixelBuffer();                        // (re)allocate memory for _pixels[]

    /******************************************************************************************************************************************************************************
    *******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Animation Effect:   Segments   ***************************************************************************************************************************************************************************
    **  @note:    New effect that merges HACS and WLED with segments and dynamic animation storage **************************************************************************************************************************************************************************
    ********************************************************************************************************************************************************************************
    *****************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/

    static uint32_t ColourBlend(uint32_t color1, uint32_t color2, uint8_t blend);
    
    void Init_Busses();

    bool LightingBusConfig_CheckPinsAvailable(
      const BusConfig& bus_config,
      uint8_t bus_i,
      uint8_t lighting_digital_index,
      uint8_t lighting_clock_index,
      uint8_t lighting_pwm_index,
      uint8_t lighting_onoff_index,
      bool override_existing
    );

    bool Lighting_AllocatePin_WithOverride(
      uint8_t real_pin,
      uint16_t packed_id,
      uint16_t owner_id,
      bool override_existing
    );

    void LightingBusConfig_AllocatePins(
      const BusConfig& bus_config,
      uint8_t bus_i,
      uint8_t& lighting_digital_index,
      uint8_t& lighting_clock_index,
      uint8_t& lighting_pwm_index,
      uint8_t& lighting_onoff_index,
      bool override_existing
    );
    
// // Time CONFIG
// #ifndef WLED_NTP_ENABLED
//   #define WLED_NTP_ENABLED false
// #endif
// #ifndef WLED_TIMEZONE
//   #define WLED_TIMEZONE 0
// #endif
// #ifndef WLED_UTC_OFFSET
//   #define WLED_UTC_OFFSET 0
// #endif
// bool ntpEnabled      _INIT(WLED_NTP_ENABLED); // get internet time. Only required if you use clock overlays or time-activated macros
bool useAMPM         = false; //_INIT(false);            // 12h/24h clock format
// byte currentTimezone _INIT(WLED_TIMEZONE);    // Timezone ID. Refer to timezones array in wled10_ntp.ino
// int utcOffsetSecs    _INIT(WLED_UTC_OFFSET);  // Seconds to offset from UTC before timzone calculation

// byte overlayCurrent _INIT(0);    // 0: no overlay 1: analog clock 2: was single-digit clock 3: was cronixie
// byte overlayMin _INIT(0), overlayMax _INIT(DEFAULT_LED_COUNT - 1);   // boundaries of overlay mode

// byte analogClock12pixel _INIT(0);               // The pixel in your strip where "midnight" would be
// bool analogClockSecondsTrail _INIT(false);      // Display seconds as trail of LEDs instead of a single pixel
// bool analogClock5MinuteMarks _INIT(false);      // Light pixels at every 5-minute position
// bool analogClockSolidBlack _INIT(false);        // Show clock overlay only if all LEDs are solid black (effect is 0 and color is black)

bool countdownMode _INIT(false);                         // Clock will count down towards date
byte countdownYear _INIT(20), countdownMonth _INIT(1);   // Countdown target date, year is last two digits
byte countdownDay  _INIT(1) , countdownHour  _INIT(0);
byte countdownMin  _INIT(0) , countdownSec   _INIT(0);

// uint16_t serialBaud _INIT(1152); // serial baud rate, multiply by 100
// bool     serialCanRX _INIT(false);
// bool     serialCanTX _INIT(false);
// #ifndef WLED_MAX_BUTTONS
//   #ifdef ESP8266
//     #define WLED_MAX_BUTTONS 2
//   #else
//     #define WLED_MAX_BUTTONS 4
//   #endif
// #else
//   #if WLED_MAX_BUTTONS < 2
//     #undef WLED_MAX_BUTTONS
//     #define WLED_MAX_BUTTONS 2
//   #endif
// #endif
// byte macroNl   _INIT(0);        // after nightlight delay over
// byte macroCountdown _INIT(0);
// byte macroAlexaOn _INIT(0), macroAlexaOff _INIT(0);
// byte macroButton[WLED_MAX_BUTTONS]        _INIT({0});
// byte macroLongPress[WLED_MAX_BUTTONS]     _INIT({0});
// byte macroDoublePress[WLED_MAX_BUTTONS]   _INIT({0});

// // Security CONFIG
bool otaLock     _INIT(false);  // prevents OTA firmware updates without password. ALWAYS enable if system exposed to any public networks
bool wifiLock    _INIT(false);  // prevents access to WiFi settings when OTA lock is enabled
// bool aOtaEnabled _INIT(true);   // ArduinoOTA allows easy updates directly from the IDE. Careful, it does not auto-disable when OTA lock is on
// char settingsPIN[5] _INIT("");  // PIN for settings pages
bool correctPIN     _INIT(true);
unsigned long lastEditTime _INIT(0);


// uint16_t userVar0 _INIT(0), userVar1 _INIT(0); //available for use in usermod

// // countdown
// unsigned long countdownTime _INIT(1514764800L);
bool countdownOverTriggered _INIT(true);

uint8_t blendingStyle = 0;
bool          transitionActive         _INIT(false);
uint16_t transitionDelay          _INIT(750);    // global transition duration
uint16_t      transitionDelayDefault   _INIT(750);    // default transition time (stored in cfg.json)
unsigned long transitionStartTime;


  // #define setValuesFromMainSeg()          setValuesFromSegment(getMainSegmentId())
  // #define setValuesFromFirstSelectedSeg() setValuesFromSegment(getFirstSelectedSegId())
  void  applyBri();
  void  applyFinalBri();

  void stateUpdated(byte callMode);

  byte scaledBri(byte in);



// // timer
// byte lastTimerMinute = 0;
// byte timerHours[10]     = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
// int8_t timerMinutes[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
// byte timerMacro[10]     = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
// // weekdays to activate on, bit pattern of arr elem: 0b11111111: sun,sat,fri,thu,wed,tue,mon,validity
// byte timerWeekday[10]   = { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 };
// // upper 4 bits start, lower 4 bits end month (default 28: start month 1 and end month 12)
// byte timerMonth[8]      = { 28, 28, 28, 28, 28, 28, 28, 28 };
// byte timerDay[8]        = { 1, 1, 1, 1, 1, 1, 1, 1 };
// byte timerDayEnd[8]     = { 31, 31, 31, 31, 31, 31, 31, 31 };
bool doAdvancePlaylist  = false;


    void Init_Segments();


    #ifdef ENABLE_FEATURE_LIGHTS__PRESETS
    volatile byte presetToApply = 0;
    volatile byte callModeToApply = 0;
    volatile byte presetToSave = 0;
    volatile int8_t saveLedmap = -1;
    char quickLoad[9] = {0};
    char saveName[33] = {0};
    bool includeBri = true, segBounds = true, selectedOnly = false, playlistSave = false;;

    //Notifier callMode
    #define CALL_MODE_INIT           0     //no updates on init, can be used to disable updates
    #define CALL_MODE_DIRECT_CHANGE  1
    #define CALL_MODE_BUTTON         2     //default button actions applied to selected segments
    #define CALL_MODE_NOTIFICATION   3
    #define CALL_MODE_NIGHTLIGHT     4
    #define CALL_MODE_NO_NOTIFY      5
    #define CALL_MODE_FX_CHANGED     6     //no longer used
    #define CALL_MODE_HUE            7
    #define CALL_MODE_PRESET_CYCLE   8
    #define CALL_MODE_BLYNK          9     //no longer used
    #define CALL_MODE_ALEXA         10
    #define CALL_MODE_WS_SEND       11     //special call mode, not for notifier, updates websocket only
    #define CALL_MODE_BUTTON_PRESET 12     //button/IR JSON preset/macro

    
    void doSaveState();
    bool getPresetName(byte index, String& name);
    void initPresetsFile();
    bool applyPreset(byte index, byte callMode = CALL_MODE_DIRECT_CHANGE);
    void applyPresetWithFallback(uint8_t presetID, uint8_t callMode, uint8_t effectID = 0, uint8_t paletteID = 0);
    void SubTask_Presets();
    inline bool applyTemporaryPreset() {return applyPreset(255);};

    void savePreset(byte index, const char* pname = nullptr, JsonObject saveobj = JsonObject());
    inline void saveTemporaryPreset() {savePreset(255);};

    void deletePreset(byte index);
    #endif // ENABLE_FEATURE_LIGHTS__PRESETS



    /******************************************************************************************************************************************************************************
    **** Playlists ***************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/
    #ifdef ENABLE_FEATURE_LIGHTS__PLAYLISTS

    typedef struct PlaylistEntry 
    {
      uint8_t preset; //ID of the preset to apply, since they can be any order
      uint16_t dur;   //Duration of the entry (in tenths of seconds)
      uint16_t tr;    //Duration of the transition TO this entry (in tenths of seconds)
    } ple;


    byte           playlistRepeat = 1;        //how many times to repeat the playlist (0 = infinitely)
    byte           playlistEndPreset = 0;     //what preset to apply after playlist end (0 = stay on last preset)
    byte           playlistOptions = 0;       //bit 0: shuffle playlist after each iteration. bits 1-7 TBD

    PlaylistEntry *playlistEntries = nullptr;
    byte           playlistLen = 0;               //number of playlist entries
    int8_t         playlistIndex = -1;
    uint16_t       playlistEntryDur = 0;      //duration of the current entry in tenths of seconds
    
    int16_t currentPlaylist = -1;

    uint32_t tSaved_playlist_debug = 0;


    void shufflePlaylist();
    void unloadPlaylist();
    int16_t loadPlaylist(JsonObject playlistObj, byte presetId);
    void SubTask_Playlist();
    void serializePlaylist(JsonObject sObj);

    void ScanPresetsFile_GeneratePlaylistIDsFromPSN_2();

    #ifdef ENABLE_FEATURE_LIGHTING__PRESET_FILE_METADATA
    struct PresetFileMeta {
      bool     enablePsn;     // 0/1: PSN helper enabled
      bool     enablePlaylistTimeLocks; // NEW
      uint8_t  parserVersion; // version of PSN parser logic
      uint32_t lastScanMs;    // last PSN scan duration (ms)
    };
    bool LoadPresetFileMeta(PresetFileMeta &meta);
    bool SavePresetFileMeta(const PresetFileMeta &meta);
    bool IsPlaylistTimeLocksEnabled();
    static const uint8_t kPresetMetaParserVersion = 2;
    // Date Modified: 13Dec25
    #ifdef ENABLE_FEATURE_LIGHTING__PLAYLIST_TIMELOCKS
    // todStart/todEnd in HHMM (e.g. 1400 = 14:00), 0 => no lock.
    // nowHHMM in HHMM form (hour*100 + minute).
    static bool playlistEntryAllowedAtTime(int16_t todStart, int16_t todEnd, uint16_t nowHHMM)
    {
      // No lock if both are zero
      if (todStart == 0 && todEnd == 0) return true;

      // Basic range sanity; treat invalid as unlocked
      if (todStart < 0 || todStart > 2359 || todEnd < 0 || todEnd > 2359) return true;

      // Normal case: window does not wrap midnight
      if (todStart <= todEnd) {
        return (nowHHMM >= todStart && nowHHMM < todEnd);
      }

      // Wraps midnight: e.g. 2200 -> 0200
      return (nowHHMM >= todStart || nowHHMM < todEnd);
    }
    uint8_t Playlist_SelectAllowedIndexByTime(JsonObject playlist, uint8_t currentIndex, uint16_t nowHHMM);
    #endif

    #endif


    #endif // ENABLE_FEATURE_LIGHTS__PLAYLISTS

    void Handle_FileSave_Edits();


    bool isAsterisksOnly(const char* str, byte maxLen);


    #ifdef ENABLE_FEATURE_LIGHTING__WEBUI
    void SettingsPages__ParseForm(AsyncWebServerRequest *request, byte subPage);
    bool handle__HTTP__GET_QueryAPI(AsyncWebServerRequest *request, const String& req, bool apply=true);
    #endif // ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS

    /******************************************************************************************************************************************************************************
    *******************************************************************************************************************************************************************************
    ****** Effect Functions *************************************************************************************************************************************************************************
    *******************************************************************************************************************************************************************************
    *******************************************************************************************************************************************************************************
    *******************************************************************************************************************************************************************************
    *******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    void EffectAnim__Solid_Colour(); 
    #endif
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    void EffectAnim__Static_Palette();
    #endif
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    void EffectAnim__Firefly();
    #endif
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    void EffectAnim__Flicker_Base(bool use_multi = false, uint16_t flicker_palette = 0);
    void EffectAnim__Candle_Single();
    void EffectAnim__Candle_Multiple();
    #endif
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    void EffectAnim__Shimmering_Two_Palette();
    #endif
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    void EffectAnim__Shimmering_Palette_Saturation();
    #endif
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    void EffectAnim__Gradient_Palette_SegWidth();
    #endif
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    void EffectAnim__Stepping_Palette();
    #endif
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    void EffectAnim__TimeBased__HourProgress();
    #endif
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    void EffectAnim__Palette_Variation();
    #endif
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    void EffectAnim__Stepping_Palette_With_Background();
    #endif
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
    void EffectAnim__Twinkle_Out();
    void EffectAnim__Twinkle_Decay();
    void EffectAnim__Twinkle_Glow();
    void EffectAnim__Twinkle_Base(uint8_t mode);
    #endif 
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    void EffectAnim__Bands_Palette_SegWidth();
    #endif
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    void EffectAnim__Randomise_Gradient_Palette_SegWidth();
    #endif
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    void EffectAnim__Rotate_Base(uint16_t movement_amount=1, bool direction=false);
    void EffectAnim__Rotating_Palette();
    void EffectAnim__Rotating_Previous_Animation();
    #endif
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    void Segments_RotateDesiredColour(uint8_t pixels_amount_to_shift, uint8_t direction);
    #endif
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    void EffectAnim__Blend_Two_Palettes();
    #endif
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    void EffectAnim__Twinkle_Palette_Onto_Palette();
    #endif
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    // Static
    void EffectAnim__Palette_Lit_Pattern();
    void EffectAnim__TriSegCol_Lit_Pattern();
    void EffectAnim__Palettes_Interleaved_Lit_Pattern();
    void EffectAnim__Palettes_Interleaved();
    void EffectAnim__Base_Spots(uint16_t threshold);
    void EffectAnim__Spots();
    void EffectAnim__Percent();
    // One colour changes
    void EffectAnim__Random_Colour();
    // Wipe/Sweep/Runners 
    void BaseEffectAnim__Base_Colour_Wipe(bool rev, bool useRandomColors, bool useIterateOverPalette = false);
    void EffectAnim__Colour_Wipe();
    void EffectAnim__Colour_Wipe_Random();
    void EffectAnim__Colour_Wipe_Palette();
    void EffectAnim__Colour_Sweep();
    void EffectAnim__Colour_Sweep_Random();
    void EffectAnim__Colour_Sweep_Palette();
    void EffectAnim__Dynamic();
    void EffectAnim__Dynamic_Smooth();
    void EffectAnim__TriColour_Wipe();
    void EffectAnim__Android();
    void EffectAnim__Base_RunningWaves(bool saw, bool dual=false);
    void EffectAnim__Running_Colour();
    void EffectAnim__Running_Random();
    void EffectAnim__Base_Gradient(bool loading);
    void EffectAnim__Gradient();
    void EffectAnim__Loading();
    void EffectAnim__Rolling_Balls();
    void EffectAnim__Fairy();
    void EffectAnim__Twinkle_Fairy();
    void EffectAnim__Running_Dual();
    void EffectAnim__Two_Dots();
    void EffectAnim__Two_Areas();
    void EffectAnim__Multi_Comet();
    void EffectAnim__Oscillate();
    void EffectAnim__BPM();
    void EffectAnim__Juggle();
    void EffectAnim__Palette();
    void EffectAnim__ColourWaves();
    void EffectAnim__Lake();
    void EffectAnim__Glitter_Base(uint8_t intensity, uint32_t col = ULTRAWHITE);
    void EffectAnim__Glitter();
    void EffectAnim__Meteor();
    void EffectAnim__Pride_2015();    
    CRGB EffectAnim__Pacifica_Base_OneLayer(uint16_t i, CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff);   
    void EffectAnim__Pacifica();    
    void EffectAnim__Sunrise();    
    void EffectAnim__Sinewave();    
    void EffectAnim__Flow();    
    void EffectAnim__Base_Phased(uint8_t moder);
    void EffectAnim__PhasedNoise();    
    void EffectAnim__Phased();    
    void EffectAnim__Running_Lights();    
    void EffectAnim__Rainbow_Cycle();    
    // Chase    
    void EffectAnim__Base_Chase(uint32_t color1, uint32_t color2, uint32_t color3, bool do_palette);
    void EffectAnim__Chase_Colour();
    void EffectAnim__Chase_Random();
    void EffectAnim__Chase_Rainbow();
    void EffectAnim__Base_Chase_Theater(uint32_t color1, uint32_t color2, bool do_palette);
    void EffectAnim__Chase_Theater();
    void EffectAnim__Chase_Theatre_Rainbow();
    void EffectAnim__Chase_Flash();
    void EffectAnim__Chase_Flash_Random();
    void EffectAnim__Chase_Rainbow_White();
    void EffectAnim__Base_Chase_TriColour(uint32_t color1, uint32_t color2);
    void EffectAnim__Chase_TriColour();
    // Breathe/Fade/Pulse
    void EffectAnim__Breath();
    void EffectAnim__Fade();
    void EffectAnim__Fade_TriColour();
    void EffectAnim__Fade_Spots();
    // Fireworks
    void EffectAnim__Fireworks();
    void EffectAnim__Exploding_Fireworks();
    void EffectAnim__Fireworks_Starburst();
    void EffectAnim__Rain();
    void EffectAnim__Tetrix();
    void EffectAnim__Fire_Flicker();
    void EffectAnim__Exploding_Fireworks_NoLaunch();
    // Sparkle/Twinkle
    void EffectAnim__Solid_Glitter();
    void EffectAnim__Popcorn();
    void EffectAnim__GlowSpots();
    void EffectAnim__Plasma();
    void EffectAnim__Sparkle();
    void EffectAnim__Sparkle_Flash();
    void EffectAnim__Sparkle_Hyper();
    void EffectAnim__Twinkle();
    CRGB EffectAnim__Base_Twinkle_Smooth_One_Twinkle(uint32_t ms, uint8_t salt, bool cat);
    void EffectAnim__Base_Twinkle_Smooth(bool cat);
    void EffectAnim__Twinkle_Colour();
    void EffectAnim__Twinkle_Smooth();
    void EffectAnim__Twinkle_Spark();
    void EffectAnim__Twinkle_Rise();
    void EffectAnim__Halloween_Eyes();
    void EffectAnim__Saw();
    void EffectAnim__Base_Dissolve(uint32_t color);
    void EffectAnim__Dissolve();
    void EffectAnim__Dissolve_Random();
    void EffectAnim__TriPops();
    // Blink/Strobe
    void EffectAnim__Base_Blink(uint32_t color1, uint32_t color2, bool strobe, bool do_palette);
    void EffectAnim__Blink();
    void EffectAnim__Blink_Rainbow();
    void EffectAnim__Strobe();
    void EffectAnim__Strobe_Multi();
    void EffectAnim__Strobe_Rainbow();
    void EffectAnim__Rainbow();
    void EffectAnim__Lightning();
    void EffectAnim__Fire_2012();
    void EffectAnim__Railway();
    void EffectAnim__Heartbeat();
    //Noise
    void EffectAnim__FillNoise8();
    void EffectAnim__Noise16_1();
    void EffectAnim__Noise16_2();
    void EffectAnim__Noise16_3();
    void EffectAnim__Noise16_4();
    void EffectAnim__Noise_Pal();
    // Scan
    void EffectAnim__Base_Scan(bool dual);
    void EffectAnim__Scan();
    void EffectAnim__Scan_Dual();
    void EffectAnim__Larson_Scanner();
    void EffectAnim__Larson_Scanner_Dual();
    void EffectAnim__ICU();
    void EffectAnim__Base_Ripple(uint8_t blurAmount = 0);
    void EffectAnim__Ripple();
    void EffectAnim__Ripple_Rainbow(); 
    void EffectAnim__Comet();
    void EffectAnim__Chunchun();
    void EffectAnim__Dancing_Shadows();
    void EffectAnim__Washing_Machine();
    void EffectAnim__Blends();
    void EffectAnim__TV_Simulator();
    void EffectAnim__Bouncing_Balls();
    void EffectAnim__Base_Sinelon(bool dual, bool rainbow=false);
    void EffectAnim__Sinelon();
    void EffectAnim__Sinelon_Dual();
    void EffectAnim__Sinelon_Rainbow();
    void EffectAnim__Drip();
    void EffectAnim__FlowStripe();
    void EffectAnim__WaveSins();
    #endif // ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__HARDWARE_TESTING
    void EffectAnim__Hardware__Show_Bus();
    void EffectAnim__Hardware__Show_Bus_Dotted();
    void EffectAnim__Hardware__Manual_Pixel_Counting();
    void EffectAnim__Hardware__View_Pixel_Range();
    void EffectAnim__Hardware__Light_Sensor_Pixel_Indexing();
    void LightSensorIndexing__SaveResults_To_File();
    void LightSensorIndexing__LoadResults_To_File();
    void EffectAnim__Hardware__Light_Sensor_Pixel_Indexing_Button_Triggered();
    #endif // ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__HARDWARE_TESTING
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__SUN_POSITIONS
    void EffectAnim__SunPositions__Sunrise_Alarm_01();
    void EffectAnim__SunPositions__Azimuth_Selects_Gradient_Of_Palette_01();
    void EffectAnim__SunPositions__Sunset_Blended_Palettes_01();
    void EffectAnim__SunPositions__NoonBurst_Base(uint8_t speed, uint16_t center);
    void EffectAnim__SunPositions__DrawSun_1D_Elevation_Base(bool include_duskdawn);
    void EffectAnim__SunPositions__DrawSun_1D_Elevation_01();
    void EffectAnim__SunPositions__DrawSun_1D_Elevation_02();
    void EffectAnim__SunPositions__DrawSun_1D_Azimuth_Base(bool include_duskdawn);
    void EffectAnim__SunPositions__DrawSun_1D_Azimuth_01();
    void EffectAnim__SunPositions__DrawSun_1D_Azimuth_02();
    void EffectAnim__SunPositions__DrawSun_2D_Elevation_And_Azimuth_01();
    void EffectAnim__SunPositions__White_Colour_Temperature_CCT_Based_On_Elevation_01();
    #endif // ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__SUN_POSITIONS
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__CONTROLLED_FROM_ANOTHER_MODULE
    void EffectAnim__Manual__ControlledFromAnotherModule();
    #endif // ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__CONTROLLED_FROM_ANOTHER_MODULE
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__CHRISTMAS_MULTIFUNCTION_CONTROLLER
    void EffectAnim__Christmas_Slo_Glo__01();
    void EffectAnim__Christmas_Sequential__01();
    void EffectAnim__Christmas_InWaves__01();
    void EffectAnim__Christmas_ChasingFlash__01();
    void EffectAnim__Christmas_TwinkleFlash__01();
    void EffectAnim__Christmas_Twinkle_Thermal();
    #endif
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__CHRISTMAS_MULTIFUNCTION_CONTROLLER_DEV
    void EffectAnim__Christmas_Slo_Glo__02();
    void EffectAnim__Christmas_Sequential__02();
    void EffectAnim__Christmas_InWaves__02();
    void EffectAnim__Christmas_ChasingFlash__02();
    void EffectAnim__Christmas_TwinkleFlash__02();
    void EffectAnim__Christmas_Slo_Glo__02();    
    void EffectAnim__Christmas_Sequential_And_Slo_Glo_Plus__Base(bool is_slo_glo);
    void EffectAnim__Christmas_Slo_Glo_Plus__01();
    void EffectAnim__Christmas_Sequential_Plus__01();
    #endif
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL0_DEVELOPING
    void SubTask_Flasher_Animate_Function_Tester_01();
    void SubTask_Flasher_Animate_Function_Tester_02();
    #endif // ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL0_DEVELOPING
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__BORDER_WALLPAPERS
    void EffectAnim__BorderWallpaper__TwoColour_Gradient();
    void EffectAnim__BorderWallpaper__FourColour_Gradient();
    void EffectAnim__BorderWallpaper__FourColour_Solid();
    #endif // ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__BORDER_WALLPAPERS
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__SUN_TRACKING
    void EffectAnim__SolarTriggers__Sunrise_01();
    #endif // ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__SUN_TRACKING
    
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL0_DEVELOPING
    void EffectAnim__Christmas_Musical__01();
    #endif 
        
    /****************************************************************************************************************************************************************************
    *** Specialised: 2D (No Audio) **********************************************************************************************************************************************
    **  Requires:     ***********************************************************************************************************************************************************
    *****************************************************************************************************************************************************************************/
    #ifdef ENABLE_FEATURE_LIGHTING__2D_MATRIX
    void EffectAnim__2D__Blackhole();
    void EffectAnim__2D__ColouredBursts();
    void EffectAnim__2D__DNA();
    void EffectAnim__2D__DNASpiral();
    void EffectAnim__2D__Drift();
    void EffectAnim__2D__FireNoise();
    void EffectAnim__2D__Frizzles();
    void EffectAnim__2D__GameOfLife();
    void EffectAnim__2D__Hipnotic();
    void EffectAnim__2D__Julia();
    void EffectAnim__2D__Lissajous();
    void EffectAnim__2D__Matrix();
    void EffectAnim__2D__Metaballs();
    void EffectAnim__2D__Noise();
    void EffectAnim__2D__PlasmaBall();
    void EffectAnim__2D__PolarLights();
    void EffectAnim__2D__Pulser();
    void EffectAnim__2D__SinDots();
    void EffectAnim__2D__SqauredSwirl();
    void EffectAnim__2D__SunRadiation();
    void EffectAnim__2D__Tartan();
    void EffectAnim__2D__SpaceShips();
    void EffectAnim__2D__CrazyBees();
    void EffectAnim__2D__GhostRider();
    void EffectAnim__2D__FloatingBlobs();
    void EffectAnim__2D__DriftRose();
    void EffectAnim__2D__DistortionWaves();
    void EffectAnim__2D__Soap();
    void     EffectAnim__2D__Soap_Base(bool isRow, uint8_t *noise3d, CRGB *pixels);
    void EffectAnim__2D__Octopus();
    void EffectAnim__2D__WavingCell();
    void EffectAnim__2D__ScrollingText();
    void EffectAnim__2D__ScrollingText_With_Baseline();
    void EffectAnim__2D__DigitalClock();
    void EffectAnim__2D__PlasmaRotoZoom();
    #endif
    /****************************************************************************************************************************************************************************
    *** Specialised: 1D (Audio Reactive) ****************************************************************************************************************************************
    **  Requires:     ***********************************************************************************************************************************************************
    *****************************************************************************************************************************************************************************/
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT__AUDIO_REACTIVE__1D
    void EffectAnim__AudioReactive__1D__FFT_Ripple_Peak();
    void EffectAnim__AudioReactive__1D__FFT_Perline_Move();
    void EffectAnim__AudioReactive__1D__FFT_Aurora();
    void EffectAnim__AudioReactive__1D__FFT_Juggles();
    void EffectAnim__AudioReactive__1D__FFT_Matripix();
    void EffectAnim__AudioReactive__1D__FFT_MidNoise();
    void EffectAnim__AudioReactive__1D__FFT_NoiseFire();
    void EffectAnim__AudioReactive__1D__FFT_NoiseMeter();
    void EffectAnim__AudioReactive__1D__FFT_PixelWave();
    void EffectAnim__AudioReactive__1D__FFT_Plasmoid();
    void EffectAnim__AudioReactive__1D__FFT_Puddle__Base(bool peakdetect);
    void EffectAnim__AudioReactive__1D__FFT_PuddlePeak();
    void EffectAnim__AudioReactive__1D__FFT_Puddles();
    void EffectAnim__AudioReactive__1D__FFT_Pixels();
    void EffectAnim__AudioReactive__1D__FFT_Blurz();
    void EffectAnim__AudioReactive__1D__FFT_DJLight();
    void EffectAnim__AudioReactive__1D__FFT_FreqMap();
    void EffectAnim__AudioReactive__1D__FFT_FreqMatrix();
    void EffectAnim__AudioReactive__1D__FFT_FreqPixels();
    void EffectAnim__AudioReactive__1D__FFT_Grav__Base(unsigned mode);
    void EffectAnim__AudioReactive__1D__FFT_GravCenter();
    void EffectAnim__AudioReactive__1D__FFT_GravCentric();
    void EffectAnim__AudioReactive__1D__FFT_GravMeter();
    void EffectAnim__AudioReactive__1D__FFT_GravFreq();
    void EffectAnim__AudioReactive__1D__FFT_NoiseMove();
    void EffectAnim__AudioReactive__1D__FFT_RockTaves();
    void EffectAnim__AudioReactive__1D__FFT_FreqWave();
    void EffectAnim__AudioReactive__1D__FFT_Waterfall();
    #endif
    /****************************************************************************************************************************************************************************
    *** Specialised: 2D (Audio Reactive) ****************************************************************************************************************************************
    **  Requires:     ***********************************************************************************************************************************************************
    *****************************************************************************************************************************************************************************/
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT__AUDIO_REACTIVE__2D
    void EffectAnim__AudioReactive__2D__Swirl();
    void EffectAnim__AudioReactive__2D__Waverly();
    void EffectAnim__AudioReactive__2D__FFT_GED();
    void EffectAnim__AudioReactive__2D__FFT_FunkyPlank();
    void EffectAnim__AudioReactive__2D__FFT_Akemi();
    #endif
    /****************************************************************************************************************************************************************************
    *** Specialised: Notifcations ****************************************************************************************************************************************
    **  Requires:     ***********************************************************************************************************************************************************
    *****************************************************************************************************************************************************************************/
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__NOTIFICATIONS
    void SubTask_Segment_Animate_Function__Notification_Static();
    void SubTask_Segment_Animate_Function__Notification_Fade();
    void SubTask_Segment_Animate_Function__Notification_Blinking();
    void SubTask_Segment_Animate_Function__Notification_Pulsing();
    void SubTask_Segment_Animate_Function__Notification_Base(bool flag_static = false, bool flag_blink = false, bool flag_pulse = false);
    #endif // ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__NOTIFICATIONS
    /****************************************************************************************************************************************************************************
    *** Specialised: Notifcations ****************************************************************************************************************************************
    **  Requires:     ***********************************************************************************************************************************************************
    *****************************************************************************************************************************************************************************/
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
    
    #define LED_DIGITS 4                             // 4 or 6 digits, can only be an even number as...
    // #define LED_PER_DIGITS_STRIP 47                  // ...two digits are made out of one piece of led strip with 47 leds...
    #define LED_PER_DIGITS_STRIP 44                  // OR...two digits are made out of TWO pieces of led strip with 44 leds...
    #define LED_BETWEEN_DIGITS_STRIPS 5              // 5 leds between above strips - and all this gives us LED_COUNT... :D
    #define LED_COUNT ( LED_DIGITS / 2 ) * LED_PER_DIGITS_STRIP + ( LED_DIGITS / 3 ) * LED_BETWEEN_DIGITS_STRIPS

    /* Segment order, seen from the front:

        <  A  >
      /\       /\
      F        B
      \/       \/
        <  G  >
      /\       /\
      E        C
      \/       \/
        <  D  >

    */

    byte segGroups[14][2] = {         // 14 segments per strip, each segment has 1-x led(s). So lets assign them in a way we get something similar for both digits
      // right (seen from front) digit. This is which led(s) can be seen in which of the 7 segments (two numbers: First and last led inside the segment, same on TE):
      { 13, 15 },                     // top, a
      { 10, 12 },                     // top right, b
      {  6, 8  },                     // bottom right, c
      {  3, 5  },                     // bottom, d
      {  0, 2  },                     // bottom left, e
      { 16, 18 },                     // top left, f
      { 19, 21 },                     // center, g
      // left (seen from front) digit
      { 35, 37 },                     // top, a
      { 38, 40 },                     // top right, b
      { 22, 24 },                     // bottom right, c
      { 25, 27 },                     // bottom, d
      { 28, 30 },                     // bottom left, e
      { 32, 34 },                     // top left, f
      { 41, 43 }                      // center, g
    };

    // Using above arrays it's very easy to "talk" to the segments. Simply use 0-6 for the first 7 segments, add 7 (7-13) for the following ones per strip/two digits
    byte digits[14][7] = {                    // Lets define 10 numbers (0-9) with 7 segments each, 1 = segment is on, 0 = segment is off
      {   1,   1,   1,   1,   1,   1,   0 },  // 0 -> Show segments a - f, don't show g (center one)
      {   0,   1,   1,   0,   0,   0,   0 },  // 1 -> Show segments b + c (top and bottom right), nothing else
      {   1,   1,   0,   1,   1,   0,   1 },  // 2 -> and so on...
      {   1,   1,   1,   1,   0,   0,   1 },  // 3
      {   0,   1,   1,   0,   0,   1,   1 },  // 4
      {   1,   0,   1,   1,   0,   1,   1 },  // 5
      {   1,   0,   1,   1,   1,   1,   1 },  // 6
      {   1,   1,   1,   0,   0,   0,   0 },  // 7
      {   1,   1,   1,   1,   1,   1,   1 },  // 8
      {   1,   1,   1,   1,   0,   1,   1 },  // 9
      {   0,   0,   0,   1,   1,   1,   1 },  // t -> some letters from here on (index 10-13, so this won't interfere with using digits 0-9 by using index 0-9
      {   0,   0,   0,   0,   1,   0,   1 },  // r
      {   0,   1,   1,   1,   0,   1,   1 },  // y
      {   0,   1,   1,   1,   1,   0,   1 }   // d
    };


    uint16_t lcd_display_show_number = 0;
    char lcd_display_show_string[5] = {0}; //temporary solution, will be removed once newer commend to save effect runtime struct works
    byte displayMode = 1;                            // 0 = 12h, 1 = 24h (will be saved to EEPROM once set using buttons)
    byte lastSecond = 0;
    byte startColor = 0;                             // "index" for the palette color used for drawing
    byte colorOffset = 32;                           // default distance between colors on the color palette used between digits/leds (in overlayMode)


    void LCDDisplay_displayTime(time_t t, byte color, byte colorSpacing);
    void LCDDisplay_showDigit(byte digit, byte color, byte pos);
    void LCDDisplay_showSegment(byte segment, byte color, byte segDisplay);
    void LCDDisplay_showDots(byte dots, byte color);

    void EffectAnim__7SegmentDisplay__ClockTime_01();
    void EffectAnim__7SegmentDisplay__ClockTime_02();
    void EffectAnim__7SegmentDisplay__ManualNumber_01();
    void EffectAnim__7SegmentDisplay__ManualString_01();
    void ConstructJSONBody_Animation_Progress__LCD_Clock_Time_Basic_01();
    void ConstructJSONBody_Animation_Progress__LCD_Clock_Time_Basic_02();

    #endif // ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__LED_SEGMENT_CLOCK

    
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL5_PARTICLE_SYSTEM

    /****************************************************************************************************************************************************************************
    *** Particle System: 1D ******************************************************************************************************************************************************
    *****************************************************************************************************************************************************************************/
    void EffectAnim__Particle__1D__Drip();
    void EffectAnim__Particle__1D__Pinball();
    void EffectAnim__Particle__1D__DancingShadows();
    void EffectAnim__Particle__1D__Fireworks();
    void EffectAnim__Particle__1D__Sparkler();
    void EffectAnim__Particle__1D__Hourglass();
    void EffectAnim__Particle__1D__Spray();
    void EffectAnim__Particle__1D__Balance();
    void EffectAnim__Particle__1D__Chase();
    void EffectAnim__Particle__1D__Starburst();
    void EffectAnim__Particle__1D__GEQ();
    void EffectAnim__Particle__1D__Fire();
    void EffectAnim__Particle__1D__SonicStream();
    void EffectAnim__Particle__1D__SonicBoom();
    void EffectAnim__Particle__1D__Springy();


    /****************************************************************************************************************************************************************************
    *** Particle System: 2D ******************************************************************************************************************************************************
    **  Requires: ENABLE_FEATURE_LIGHTING__2D_MATRIX                                                                                                                             
    *****************************************************************************************************************************************************************************/
    #ifdef ENABLE_FEATURE_LIGHTING__2D_MATRIX
    void EffectAnim__Particle__2D__Volcano();
    void EffectAnim__Particle__2D__Fire();
    void EffectAnim__Particle__2D__Fireworks();
    void EffectAnim__Particle__2D__Vortex();
    void EffectAnim__Particle__2D__Perlin();
    void EffectAnim__Particle__2D__Pit();
    void EffectAnim__Particle__2D__Box();
    void EffectAnim__Particle__2D__Attractor();
    void EffectAnim__Particle__2D__Impact();
    void EffectAnim__Particle__2D__Waterfall();
    void EffectAnim__Particle__2D__Spray();
    void EffectAnim__Particle__2D__GEQ();
    void EffectAnim__Particle__2D__CenterGEQ();
    void EffectAnim__Particle__2D__GhostRider();
    void EffectAnim__Particle__2D__Blobs();
    void EffectAnim__Particle__2D__Galaxy();
    #endif // ENABLE_FEATURE_LIGHTING__2D_MATRIX

    #endif // ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL5_PARTICLE_SYSTEM
    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__RAY_TRACING

    void EffectAnim__RayTracing__Shooting_And_Bouncing();
    void EffectAnim__RayTracing__Coverage();
    void EffectAnim__RayTracing__MobileRX();

    #endif

    /******************************************************************************************************************************************************************************
    *******************************************************************************************************************************************************************************
    ****** Effect Enums *************************************************************************************************************************************************************************
    *******************************************************************************************************************************************************************************
    *******************************************************************************************************************************************************************************
    *******************************************************************************************************************************************************************************
    *******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/

    /***
     * Effect Ideas:
     * * (1) "ScatterBurst" 
     *        - A burst of light that scatters in all directions, like fireworks.
     *        - New spanned burst points will appear equidistant along the string, with random bursting that either
     *          allows overlap, or isolated (ends short) of the neighbouring burst points.
     *        - Option Multi:
     *           = Dying fades will either; immediate die, flicker out, or fade out.
     *    (2) ScatterBurst Random
     *        - Similar to (1), but the burst points are randomly distributed along the string. (eg similar to firework without launch)
     *        - Option Multi:
     *           = Dying fades will either; immediate die, flicker out, or fade out.
     *        - (1) and (2) should share base
     *    (3) Work on PWM effects for XMAS controller emulation.
     *        - Using a custom PWM/H321 build with 4 outputs, can I completely emulate the XMAS controller?
     *        - Later, a custom board could be made that works on mains voltage, and has 4 outputs.
     *        - Make a test board for this, with a jumper enabling 3V to a row of coloured LEDs. Those LED grounds, will be tied to PWM shifters, so I can test with LEDs or view full sets.
     *     (4) "Pop #"
     *        - Likely exists already but can be refined.
     *        - Random Palette colours will go full brightness On, then decay to secondary palette
     *          = Using base, effect will be added to have fade to black default, decay to white (ie saturation decay) or decay to another palette.
     *          = Decay method, will again be either; immediate die, flicker out, or fade out.
     * 
     */

    enum EFFECTS_FUNCTION__IDS
    {
      // General Level 1 Minimal Home Effects
      #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
      EFFECTS_FUNCTION__SOLID_COLOUR__ID,
      EFFECTS_FUNCTION__STATIC_PALETTE__ID,
      EFFECTS_FUNCTION__BANDS_PALETTE_SEGWIDTH__ID,
      EFFECTS_FUNCTION__FIREFLY__ID,
      EFFECTS_FUNCTION__CANDLE_SINGLE__ID,
      EFFECTS_FUNCTION__CANDLE_MULTIPLE__ID,
      EFFECTS_FUNCTION__RANDOMISE_GRADIENT_PALETTE_SEGWIDTH__ID,
      // EFFECTS_FUNCTION__RANDOMISE_GRADIENT_ANY_PALETTE_WITH_ZOOM_AND_SQUEEZE__ID, Make new effect that will take any palette, with or without grad indexs, and will zoom/stretch them. Ie 4 colours, which would have [0, 90,190, 255] as centre points, will have the 90 and 190 distances move around. This will depend on number of colours in a palette.
      EFFECTS_FUNCTION__PALETTE_VARIATION__ID,
      EFFECTS_FUNCTION__GRADIENT_PALETTE_SEGWIDTH__ID,
      #endif

      // General Level 2 Flashing Basic Effects
      #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
      EFFECTS_FUNCTION__SHIMMERING_PALETTE_DOUBLE__ID,
      EFFECTS_FUNCTION__SHIMMERING_PALETTE_SATURATION__ID,
      EFFECTS_FUNCTION__ROTATING_PALETTE__ID,
      EFFECTS_FUNCTION__ROTATING_PREVIOUS_ANIMATION__ID,
      EFFECTS_FUNCTION__STEPPING_PALETTE_WITH_BACKGROUND__ID,
      EFFECTS_FUNCTION__STEPPING_PALETTE__ID,
      EFFECTS_FUNCTION__BLEND_PALETTE_BETWEEN_ANOTHER_PALETTE__ID,
      EFFECTS_FUNCTION__TWINKLE_PALETTE_SEC_ON_ORDERED_PALETTE_PRI__ID,
      EFFECTS_FUNCTION__TIMEBASED__HOUR_PROGRESS__ID,
      #endif

      // General Level 3 Flashing Extended Effects
      #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
      EFFECTS_FUNCTION__TWINKLE_OUT__ID,
      EFFECTS_FUNCTION__TWINKLE_DECAY__ID,
      EFFECTS_FUNCTION__TWINKLE_GLOW__ID,
      EFFECTS_FUNCTION__PALETTE_LIT_PATTERN__ID,
      EFFECTS_FUNCTION__TRISEGCOL_LIT_PATTERN__ID,
      EFFECTS_FUNCTION__PALETTES_INTERLEAVED_LIT_PATTERN__ID,
      EFFECTS_FUNCTION__PALETTES_INTERLEAVED__ID,
      #endif

      #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

      // General Level 4 Flashing Complete Effects
      EFFECTS_FUNCTION__SPOTS__ID,
      EFFECTS_FUNCTION__PERCENT__ID,
      EFFECTS_FUNCTION__RANDOM_COLOR__ID,       
      EFFECTS_FUNCTION__TRICOLOR_WIPE__ID,
      /**
       * Wipe/Sweep/Runners 
       **/
      EFFECTS_FUNCTION__COLOR_WIPE__ID,
      EFFECTS_FUNCTION__COLOR_WIPE_RANDOM__ID,
      EFFECTS_FUNCTION__COLOR_WIPE_PALETTE__ID,
      EFFECTS_FUNCTION__COLOR_SWEEP__ID,
      EFFECTS_FUNCTION__COLOR_SWEEP_RANDOM__ID,
      EFFECTS_FUNCTION__COLOR_SWEEP_PALETTE__ID,       
      EFFECTS_FUNCTION__DYNAMIC__ID,                  
      EFFECTS_FUNCTION__DYNAMIC_SMOOTH__ID,                
      EFFECTS_FUNCTION__RUNNING_COLOR__ID,
      EFFECTS_FUNCTION__RUNNING_RANDOM__ID,      
      EFFECTS_FUNCTION__ANDROID__ID,               
      EFFECTS_FUNCTION__GRADIENT__ID,
      EFFECTS_FUNCTION__LOADING__ID,
      EFFECTS_FUNCTION__ROLLINGBALLS__ID,
      EFFECTS_FUNCTION__FAIRY__ID,
      EFFECTS_FUNCTION__TWO_DOTS__ID,
      EFFECTS_FUNCTION__TWO_AREAS__ID,                
      EFFECTS_FUNCTION__TWINKLE_FAIRY__ID,                
      EFFECTS_FUNCTION__RUNNING_DUAL__ID,   
      EFFECTS_FUNCTION__MULTI_COMET__ID,
      EFFECTS_FUNCTION__OSCILLATE__ID,
      EFFECTS_FUNCTION__BPM__ID,
      EFFECTS_FUNCTION__JUGGLE__ID,
      EFFECTS_FUNCTION__PALETTE__ID,
      EFFECTS_FUNCTION__COLORWAVES__ID,
      EFFECTS_FUNCTION__LAKE__ID,
      EFFECTS_FUNCTION__GLITTER__ID,
      EFFECTS_FUNCTION__METEOR__ID,
      EFFECTS_FUNCTION__PRIDE_2015__ID,
      EFFECTS_FUNCTION__PACIFICA__ID,
      EFFECTS_FUNCTION__SUNRISE__ID,
      EFFECTS_FUNCTION__SINEWAVE__ID,
      EFFECTS_FUNCTION__FLOW__ID,
      EFFECTS_FUNCTION__RUNNING_LIGHTS__ID,
      EFFECTS_FUNCTION__RAINBOW_CYCLE__ID,

      /**
       * Chase
       **/
      EFFECTS_FUNCTION__CHASE_COLOR__ID,
      EFFECTS_FUNCTION__CHASE_RANDOM__ID,
      EFFECTS_FUNCTION__CHASE_RAINBOW__ID,
      EFFECTS_FUNCTION__CHASE_FLASH__ID,
      EFFECTS_FUNCTION__CHASE_FLASH_RANDOM__ID,
      EFFECTS_FUNCTION__CHASE_RAINBOW_WHITE__ID,
      EFFECTS_FUNCTION__CHASE_THEATER__ID,
      EFFECTS_FUNCTION__CHASE_THEATER_RAINBOW__ID,
      EFFECTS_FUNCTION__CHASE_TRICOLOR__ID,

      /**
       *  Breathe/Fade/Pulse
       **/      
      EFFECTS_FUNCTION__BREATH__ID,
      EFFECTS_FUNCTION__FADE__ID,
      EFFECTS_FUNCTION__FADE_TRICOLOR__ID,
      EFFECTS_FUNCTION__FADE_SPOTS__ID,
      
      /**
       * Sparkle/Twinkle
       **/
      EFFECTS_FUNCTION__SOLID_GLITTER__ID,
      EFFECTS_FUNCTION__POPCORN__ID,
      EFFECTS_FUNCTION__GLOWSPOTS__ID,
      EFFECTS_FUNCTION__PLASMA__ID,
      EFFECTS_FUNCTION__SPARKLE__ID,
      EFFECTS_FUNCTION__FLASH_SPARKLE__ID,
      EFFECTS_FUNCTION__HYPER_SPARKLE__ID,
      EFFECTS_FUNCTION__TWINKLE__ID,
      EFFECTS_FUNCTION__TWINKLE_COLOUR__ID,
      EFFECTS_FUNCTION__TWINKLE_SMOOTH__ID,
      EFFECTS_FUNCTION__TWINKLE_SPARK__ID,
      EFFECTS_FUNCTION__TWINKLE_RISE__ID,
      EFFECTS_FUNCTION__HALLOWEEN_EYES__ID,
      EFFECTS_FUNCTION__SAW__ID,
      EFFECTS_FUNCTION__DISSOLVE__ID,
      EFFECTS_FUNCTION__DISSOLVE_RANDOM__ID,
      EFFECTS_FUNCTION__TRIPOPS__ID,

      /**
       * Fireworks
       **/
      EFFECTS_FUNCTION__FIREWORKS__ID,                
      EFFECTS_FUNCTION__FIREWORKS_EXPLODING__ID, 
      EFFECTS_FUNCTION__FIREWORKS_STARBURST__ID,
      EFFECTS_FUNCTION__FIREWORKS_STARBURST_GLOWS__ID, 
      EFFECTS_FUNCTION__RAIN__ID,                       
      EFFECTS_FUNCTION__TETRIX__ID,                 
      EFFECTS_FUNCTION__FIRE_FLICKER__ID,                  
      EFFECTS_FUNCTION__FIREWORKS_EXPLODING_NO_LAUNCH__ID,     

      /**
       * Blink/Strobe
       **/
      EFFECTS_FUNCTION__BLINK__ID,
      EFFECTS_FUNCTION__BLINK_RAINBOW__ID,
      EFFECTS_FUNCTION__STROBE__ID,
      EFFECTS_FUNCTION__MULTI_STROBE__ID,
      EFFECTS_FUNCTION__STROBE_RAINBOW__ID,
      EFFECTS_FUNCTION__RAINBOW__ID,
      EFFECTS_FUNCTION__LIGHTNING__ID,
      EFFECTS_FUNCTION__FIRE_2012__ID,
      EFFECTS_FUNCTION__RAILWAY__ID,
      EFFECTS_FUNCTION__HEARTBEAT__ID,

      /**
       * Noise
       **/
      EFFECTS_FUNCTION__FILLNOISE8__ID,
      EFFECTS_FUNCTION__NOISE16_1__ID,
      EFFECTS_FUNCTION__NOISE16_2__ID,
      EFFECTS_FUNCTION__NOISE16_3__ID,
      EFFECTS_FUNCTION__NOISE16_4__ID,
      EFFECTS_FUNCTION__NOISEPAL__ID,
      EFFECTS_FUNCTION__PHASEDNOISE__ID,
      EFFECTS_FUNCTION__PHASED__ID,

      /**
       * Scan
       **/
      EFFECTS_FUNCTION__SCAN__ID,
      EFFECTS_FUNCTION__DUAL_SCAN__ID,
      EFFECTS_FUNCTION__LARSON_SCANNER__ID,
      EFFECTS_FUNCTION__DUAL_LARSON_SCANNER__ID,
      EFFECTS_FUNCTION__ICU__ID,
      EFFECTS_FUNCTION__RIPPLE__ID,
      EFFECTS_FUNCTION__RIPPLE_RAINBOW__ID,
      EFFECTS_FUNCTION__COMET__ID,
      EFFECTS_FUNCTION__CHUNCHUN__ID,
      EFFECTS_FUNCTION__DANCING_SHADOWS__ID,
      EFFECTS_FUNCTION__WASHING_MACHINE__ID,
      EFFECTS_FUNCTION__BLENDS__ID,
      EFFECTS_FUNCTION__TV_SIMULATOR__ID,
      EFFECTS_FUNCTION__BOUNCINGBALLS__ID,
      EFFECTS_FUNCTION__SINELON__ID,
      EFFECTS_FUNCTION__SINELON_DUAL__ID,
      EFFECTS_FUNCTION__SINELON_RAINBOW__ID,
      EFFECTS_FUNCTION__DRIP__ID,
      EFFECTS_FUNCTION__FLOWSTRIPE__ID, 
      EFFECTS_FUNCTION__WAVESINS__ID, 

      #endif // ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

      /**
       * Hardware Installation Helpers
       **/
      #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__HARDWARE_TESTING
      EFFECTS_FUNCTION__HARDWARE__SHOW_BUS__ID,
      EFFECTS_FUNCTION__HARDWARE__SHOW_BUS_DOTTED__ID,
      EFFECTS_FUNCTION__HARDWARE__MANUAL_PIXEL_COUNTING__ID,
      EFFECTS_FUNCTION__HARDWARE__VIEW_PIXEL_RANGE__ID,
      #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__HARDWARE_TESTING__EXTERNAL_LIGHT_SENSING
      EFFECTS_FUNCTION__HARDWARE__LIGHT_SENSOR_PIXEL_INDEXING__ID,
      EFFECTS_FUNCTION__HARDWARE__LIGHT_SENSOR_PIXEL_INDEXING_BTN__ID,
      #endif
      #endif

      /**
       * Sun Position
       **/
      #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__SUN_POSITIONS
      EFFECTS_FUNCTION__SUNPOSITIONS_SUNRISE_ALARM_01__ID,
      EFFECTS_FUNCTION__SUNPOSITIONS_AZIMUTH_SELECTS_GRADIENT_OF_PALETTE_01__ID,
      EFFECTS_FUNCTION__SUNPOSITIONS_SUNSET_BLENDED_PALETTES_01__ID,
      EFFECTS_FUNCTION__SUNPOSITIONS_DRAWSUN_1D_ELEVATION_01__ID,
      EFFECTS_FUNCTION__SUNPOSITIONS_DRAWSUN_1D_ELEVATION_02__ID,
      EFFECTS_FUNCTION__SUNPOSITIONS_DRAWSUN_1D_AZIMUTH_01__ID,
      EFFECTS_FUNCTION__SUNPOSITIONS_DRAWSUN_1D_AZIMUTH_02__ID,
      EFFECTS_FUNCTION__SUNPOSITIONS_DRAWSUN_2D_ELEVATION_AND_AZIMUTH_01__ID,
      EFFECTS_FUNCTION__SUNPOSITIONS_WHITE_COLOUR_TEMPERATURE_CCT_BASED_ON_ELEVATION_01__ID,
      #endif

      /**
       * LED Segment Clock
       **/
      #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
      EFFECTS_FUNCTION__LCD_CLOCK_BASIC_01__ID,
      EFFECTS_FUNCTION__LCD_CLOCK_BASIC_02__ID,
      EFFECTS_FUNCTION__LCD_DISPLAY_MANUAL_NUMBER_01__ID,
      EFFECTS_FUNCTION__LCD_DISPLAY_MANUAL_STRING_01__ID,
      #endif
      
      /**
       * Notifications
       **/
      #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__NOTIFICATIONS
      EFFECTS_FUNCTION__NOTIFICATION_STATIC__ID,
      EFFECTS_FUNCTION__NOTIFICATION_FADE__ID,
      EFFECTS_FUNCTION__NOTIFICATION_BLINKING__ID,
      EFFECTS_FUNCTION__NOTIFICATION_PULSING__ID,
      #endif

      /**
       * Border/Frame/Edge Wallpapers
       **/
      #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__BORDER_WALLPAPERS
      EFFECTS_FUNCTION__BORDER_WALLPAPER__TWOCOLOUR_GRADIENT__ID,
      EFFECTS_FUNCTION__BORDER_WALLPAPER__FOURCOLOUR_GRADIENT__ID,
      EFFECTS_FUNCTION__BORDER_WALLPAPER__FOURCOLOUR_SOLID__ID,
      #endif

      /**
       * Manual Pixel: Keeping as legacy, but mode change to realtime will remove this
       * Removing this, Feb25, it should be handled by a realtime mode (ie not effects)
       **/
      #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__CONTROLLED_FROM_ANOTHER_MODULE
      EFFECTS_FUNCTION__MANUAL__CONTROLLED_FROM_ANOTHER_MODULE__ID,
      #endif

      /**
       * Christmas Multifunction Controller: Replication of vintage 8 function controllers
       * Replicate how traditional 2/4 ouput controllers and their effects look
       **/
      #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__CHRISTMAS_MULTIFUNCTION_CONTROLLER
      EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__SEQUENTIAL_01_ID,
      EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__SLO_GLO_01_ID,
      EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__INWAVES_01_ID,
      EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__CHASING_FLASH_01_ID,
      EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__TWINKLE_FLASH_01_ID,
      EFFECTS_FUNCTION__CHRISTMAS_TWINKLE_THERMAL__ID,
      #endif
      #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__CHRISTMAS_MULTIFUNCTION_CONTROLLER_DEV
      // EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__COMBINATION_ID,
      EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__SEQUENTIAL_02_ID,
      EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__SLO_GLO_02_ID,
      EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__INWAVES_02_ID,
      EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__CHASING_FLASH_02_ID,
      EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__TWINKLE_FLASH_02_ID,
      EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__SLO_GLO_02_ID,
      EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__SLO_GLO_PLUS__ID,
      EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__SEQUENTIAL_PLUS__ID,
      #endif
        

      /**
       * 2D (No Audio)
       **/
      #ifdef ENABLE_FEATURE_LIGHTING__2D_MATRIX
      EFFECTS_FUNCTION__2D__BLACK_HOLE__ID,      
      EFFECTS_FUNCTION__2D__HIPHOTIC__ID,              
      EFFECTS_FUNCTION__2D__COLOURED_BURSTS__ID,         
      EFFECTS_FUNCTION__2D__DNA__ID,        
      EFFECTS_FUNCTION__2D__DNA_SPIRAL__ID,              
      EFFECTS_FUNCTION__2D__DRIFT__ID,            
      EFFECTS_FUNCTION__2D__FIRE_NOISE__ID,   
      EFFECTS_FUNCTION__2D__FRIZZLES__ID,    
      EFFECTS_FUNCTION__2D__GAME_OF_LIFE__ID, 
      EFFECTS_FUNCTION__2D__HIPNOTIC__ID,      
      EFFECTS_FUNCTION__2D__JULIA__ID,       
      EFFECTS_FUNCTION__2D__LISSAJOUS__ID,   
      EFFECTS_FUNCTION__2D__MATRIX__ID,      
      EFFECTS_FUNCTION__2D__METABALLS__ID,    
      EFFECTS_FUNCTION__2D__NOISE__ID,        
      EFFECTS_FUNCTION__2D__PLASMA_BALL__ID,   
      EFFECTS_FUNCTION__2D__POLAR_LIGHTS__ID,  
      EFFECTS_FUNCTION__2D__PULSER__ID,           
      EFFECTS_FUNCTION__2D__SIN_DOTS__ID,      
      EFFECTS_FUNCTION__2D__SQUARED_SWIRL__ID,   
      EFFECTS_FUNCTION__2D__SUN_RADIATION__ID,    
      EFFECTS_FUNCTION__2D__TARTAN__ID,          
      EFFECTS_FUNCTION__2D__PLASMAROTOZOOM__ID,   
      EFFECTS_FUNCTION__2D__SPACESHIPS__ID,        
      EFFECTS_FUNCTION__2D__CRAZY_BEES__ID,       
      EFFECTS_FUNCTION__2D__GHOST_RIDER__ID,            
      EFFECTS_FUNCTION__2D__FLOATING_BLOBS__ID,    
      EFFECTS_FUNCTION__2D__DRIFT_ROSE__ID,        
      EFFECTS_FUNCTION__2D__DISTORTION_WAVES__ID,       
      EFFECTS_FUNCTION__2D__SOAP__ID,   
      EFFECTS_FUNCTION__2D__OCTOPUS__ID,    
      EFFECTS_FUNCTION__2D__WAVING_CELL__ID,  
      EFFECTS_FUNCTION__2D__SCROLLING_TEXT__ID,         
      EFFECTS_FUNCTION__2D__SCROLLING_TEXT_WITH_BASELINE__ID,
      EFFECTS_FUNCTION__2D__DIGITAL_CLOCK__ID,    
      // NEw effect idea, lava lamp, both 1D and matrix. Have lgihting "gravity" then also "heating" for uplifting of random colours.

      #endif
      

      /**
       * Audio Reactive 1D
       **/
      #ifdef ENABLE_FEATURE_LIGHTS__EFFECT__AUDIO_REACTIVE__1D
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_RIPPLE_PEAK__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_PERLINE_MOVE__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_AURORA__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_GRAV_CENTER__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_GRAV_CENTRIC__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_GRAVI_METER__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_JUGGLES__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_MATRIPIX__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_MID_NOISE__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_NOISE_FIRE__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_NOISE_METER__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_PIXEL_WAVE__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_PLASMOID__ID, 
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_PUDDLE_PEAK__ID, 
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_PUDDLES__ID,  
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_PIXELS__ID,  
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_BLURZ__ID,  
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_ROCKTAVES__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_DJ_LIGHT__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_FREQ_MAP__ID, 
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_FREQ_MATRIX__ID,  
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_FREQ_PIXELS__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_FREQ_WAVE__ID,  
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_GRAV_FREQ__ID,  
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_NOISE_MOVE__ID,  
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_ROCK_TAVES__ID,        
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_WATERFALL__ID,   
      #endif

      /**
       * Christmas Musical
       **/
      #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL0_DEVELOPING
      EFFECTS_FUNCTION__CHRISTMAS_MUSICAL__01_ID,
      // pulsing lights to music as music mode, instead of off or waves, go from 30% to 100% brightness in time with music beat
      #endif

      /**
       * Audio Reactive 2D
       **/
      #ifdef ENABLE_FEATURE_LIGHTS__EFFECT__AUDIO_REACTIVE__2D
      EFFECTS_FUNCTION__AUDIOREACTIVE__2D__SWIRL__ID,         
      EFFECTS_FUNCTION__AUDIOREACTIVE__2D__WAVERLY__ID,   
      EFFECTS_FUNCTION__AUDIOREACTIVE__2D__FFT_GED__ID,         
      EFFECTS_FUNCTION__AUDIOREACTIVE__2D__FFT_FUNKY_PLANK__ID,         
      EFFECTS_FUNCTION__AUDIOREACTIVE__2D__FFT_AKEMI__ID,   
      #endif

      #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL5_PARTICLE_SYSTEM

      /**
       * Particle System 1D
       **/
      EFFECTS_FUNCTION__PARTICLE__1D__DRIP__ID,
      EFFECTS_FUNCTION__PARTICLE__1D__PINBALL__ID,
      EFFECTS_FUNCTION__PARTICLE__1D__DANCING_SHADOWS__ID,
      EFFECTS_FUNCTION__PARTICLE__1D__FIREWORKS__ID,
      EFFECTS_FUNCTION__PARTICLE__1D__SPARKLER__ID,
      EFFECTS_FUNCTION__PARTICLE__1D__HOURGLASS__ID,
      EFFECTS_FUNCTION__PARTICLE__1D__SPRAY__ID,
      EFFECTS_FUNCTION__PARTICLE__1D__BALANCE__ID,
      EFFECTS_FUNCTION__PARTICLE__1D__CHASE__ID,
      EFFECTS_FUNCTION__PARTICLE__1D__STARBURST__ID,
      EFFECTS_FUNCTION__PARTICLE__1D__GEQ__ID,
      EFFECTS_FUNCTION__PARTICLE__1D__FIRE__ID,
      EFFECTS_FUNCTION__PARTICLE__1D__SONIC_STREAM__ID,
      EFFECTS_FUNCTION__PARTICLE__1D__SONIC_BOOM__ID,
      EFFECTS_FUNCTION__PARTICLE__1D__SPRINGY__ID,

      /**
       * Particle System 2D
       **/
      #ifdef ENABLE_FEATURE_LIGHTING__2D_MATRIX
      EFFECTS_FUNCTION__PARTICLE__2D__VOLCANO__ID,
      EFFECTS_FUNCTION__PARTICLE__2D__FIRE__ID,
      EFFECTS_FUNCTION__PARTICLE__2D__FIREWORKS__ID,
      EFFECTS_FUNCTION__PARTICLE__2D__VORTEX__ID,
      EFFECTS_FUNCTION__PARTICLE__2D__PERLIN__ID,
      EFFECTS_FUNCTION__PARTICLE__2D__PIT__ID,
      EFFECTS_FUNCTION__PARTICLE__2D__BOX__ID,
      EFFECTS_FUNCTION__PARTICLE__2D__ATTRACTOR__ID,
      EFFECTS_FUNCTION__PARTICLE__2D__IMPACT__ID,
      EFFECTS_FUNCTION__PARTICLE__2D__WATERFALL__ID,
      EFFECTS_FUNCTION__PARTICLE__2D__SPRAY__ID,
      EFFECTS_FUNCTION__PARTICLE__2D__GEQ__ID,
      EFFECTS_FUNCTION__PARTICLE__2D__CENTER_GEQ__ID,
      EFFECTS_FUNCTION__PARTICLE__2D__GHOST_RIDER__ID,
      EFFECTS_FUNCTION__PARTICLE__2D__BLOBS__ID,
      EFFECTS_FUNCTION__PARTICLE__2D__GALAXY__ID,
      #endif // ENABLE_FEATURE_LIGHTING__2D_MATRIX

      #endif // ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL5_PARTICLE_SYSTEM

      #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__RAY_TRACING
      EFFECTS_FUNCTION__RAY_TRACING__SHOOTING_AND_BOUNCING__ID,
      EFFECTS_FUNCTION__RAY_TRACING__COVERAGE__ID,
      EFFECTS_FUNCTION__RAY_TRACING__MOBILE_RX__ID,
      #endif

      EFFECTS_FUNCTION__LENGTH__ID
    };

  
  #define WLED_GROUP_IDS_FIRST  EFFECTS_FUNCTION__PALETTE_LIT_PATTERN__ID
  #define WLED_GROUP_IDS_LAST   EFFECTS_FUNCTION__DRIP__ID


  #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
  #define DEFAULT_EFFECTS_FUNCTION    EFFECTS_FUNCTION__STATIC_PALETTE__ID
  #else
  #define DEFAULT_EFFECTS_FUNCTION    0
  #endif

  uint16_t getEffectsAmount(){ return effects.config.size(); }

  void SubTask_Effects();
  void Segments_RefreshLEDIndexPattern(uint8_t segment_index = 0);

  
  #ifdef ENABLE_FEATURE_LIGHTS__ANIMATION_MODE__INTERNAL_CONTROL_FROM_ANOTHER_MODULE
  void SubTask_AnimationMode__InternalControlFromAnotherModule();
  #endif 
  


  byte realtimeMode = REALTIME_MODE_INACTIVE;
  bool realtimeRespectLedMaps; // used in getMappedPixelIndex()

  typedef union {
    uint16_t data; // allows full manipulating
    struct { 
      // uint16_t fForceUpdate : 1; // now using trigger, any update causes them all to update
      uint16_t fRunning : 1;
      uint16_t animator_first_run : 1;
      // Reserved
      uint16_t reserved : 12;
    };
  } ANIMATION_FLAGS;



  enum ColourType{ 
      // COLOUR_TYPE__NONE__ID=0, 
      COLOUR_TYPE__SINGLE__ID=1, // likely never used for me, remove
      COLOUR_TYPE__COLDWARM__ID=2,  //CCT Only
      COLOUR_TYPE__RGB__ID=3, //3   
      COLOUR_TYPE__RGBW__ID=4, //4
      COLOUR_TYPE__RGBWW__ID=5 // 5 
  };

  // inline uint8_t GetSizeOfPixel(ColourType colour_type)
  // {
  //   switch(colour_type)
  //   {
  //     default:
  //     case ColourType::COLOUR_TYPE__RGB__ID:     return 3;
  //     case ColourType::COLOUR_TYPE__RGBW__ID:    return 4;
  //     case ColourType::COLOUR_TYPE__RGBWW__ID:  return 5;
  //   }
  // }


  mAnimatorLight& SetSegment_AnimFunctionCallback(uint8_t segment_index, ANIM_FUNCTION_SIGNATURE);
  void StartSegmentAnimation_AsAnimUpdateMemberFunction(uint8_t segment_index = 0);
  
  
  
  void Segments_SetPixelColor_To_Static_Pallete(uint16_t palette_id);
  void Segments_UpdateDesiredColourFromPaletteSelected(uint16_t segment_index = 0);
  void AnimationProcess_Generic_AnimationColour_LinearBlend_Segments(const AnimationParam& param);




inline RgbwwColor AdjustColourWithBrightness(const RgbwwColor& colour, uint8_t brightnessRGB) {
    // Pre-calculate the scale factor (bit-shift scaling for division by 255)
    uint16_t scale = brightnessRGB + 1; // Adding 1 to avoid division by zero and maintain full range

    // Apply brightness scaling
    RgbwwColor adjusted;
    adjusted.R = (colour.R * scale) >> 8;
    adjusted.G = (colour.G * scale) >> 8;
    adjusted.B = (colour.B * scale) >> 8;
    adjusted.WW = (colour.WW * scale) >> 8;
    adjusted.CW = (colour.CW * scale) >> 8;

    return adjusted;
}
inline uint32_t AdjustColourWithBrightness(uint32_t colour32, uint8_t brightnessRGB) {
    // Pre-calculate the scale factor (bit-shift scaling for division by 255)
    uint16_t scale = brightnessRGB + 1; // Adding 1 to avoid division by zero and maintain full range

    // Extract individual color components
    uint8_t r = (colour32 >> 16) & 0xFF; // Red
    uint8_t g = (colour32 >> 8) & 0xFF;  // Green
    uint8_t b = colour32 & 0xFF;         // Blue
    uint8_t w = (colour32 >> 24) & 0xFF; // White (if applicable)

    // Apply brightness scaling
    r = (r * scale) >> 8;
    g = (g * scale) >> 8;
    b = (b * scale) >> 8;
    w = (w * scale) >> 8;

    // Recombine components into a single uint32_t color
    return (w << 24) | (r << 16) | (g << 8) | b;
}

inline static uint32_t FadeU32(uint32_t colour32, uint8_t fade) {
    // Extract individual color components
    uint8_t r = (colour32 >> 16) & 0xFF; // Red
    uint8_t g = (colour32 >> 8) & 0xFF;  // Green
    uint8_t b = colour32 & 0xFF;         // Blue
    uint8_t w = (colour32 >> 24) & 0xFF; // White (if applicable)

    // Apply fade
    r = (r > fade) ? (r / fade) : 0;
    g = (g > fade) ? (g / fade) : 0;
    b = (b > fade) ? (b / fade) : 0;
    w = (w > fade) ? (w / fade) : 0;

    // Recombine components into a single uint32_t color
    return (w << 24) | (r << 16) | (g << 8) | b;
}


  #ifdef ENABLE_DEBUGFEATURE_LIGHTING__TIME_CRITICAL_RECORDING
  struct TimeCriticalLogging{
    uint32_t start_value[7]; // use as temp to record start of measurment, use here for speed

    uint32_t dynamic_buffer__starting_colour;
    uint32_t dynamic_buffer__starting_colour_part1; // 5
    uint32_t dynamic_buffer__starting_colour_part2; // 6

    uint32_t dynamic_buffer__desired_colour;
    uint32_t effect_call;
    uint32_t segment_effects;

    bool time_unit_output_ms = false;
  }lighting_time_critical_logging;
  #endif // ENABLE_DEBUGFEATURE_LIGHTING__TIME_CRITICAL_RECORDING


  #ifdef ENABLE_DEVFEATURE_LIGHT__INCLUDE_AUDIOREACTIVE_USERMOD
  //um_manager.cpp
  typedef enum UM_Data_Types {
    UMT_BYTE = 0,
    UMT_UINT16,
    UMT_INT16,
    UMT_UINT32,
    UMT_INT32,
    UMT_FLOAT,
    UMT_DOUBLE,
    UMT_BYTE_ARR,
    UMT_UINT16_ARR,
    UMT_INT16_ARR,
    UMT_UINT32_ARR,
    UMT_INT32_ARR,
    UMT_FLOAT_ARR,
    UMT_DOUBLE_ARR
  } um_types_t;
  typedef struct UM_Exchange_Data {
    // should just use: size_t arr_size, void **arr_ptr, byte *ptr_type
    size_t       u_size;                 // size of u_data array
    um_types_t  *u_type;                 // array of data types
    void       **u_data;                 // array of pointers to data
    UM_Exchange_Data() {
      u_size = 0;
      u_type = nullptr;
      u_data = nullptr;
    }
    ~UM_Exchange_Data() {
      if (u_type) delete[] u_type;
      if (u_data) delete[] u_data;
    }
  } um_data_t;
  const unsigned int um_data_size = sizeof(um_data_t);  // 12 bytes
  um_data_t* simulateSound(uint8_t simulationId);
  static um_data_t* getAudioData();
      
  //Usermod IDs
  #define USERMOD_ID_RESERVED               0     //Unused. Might indicate no usermod present
  #define USERMOD_ID_UNSPECIFIED            1     //Default value for a general user mod that does not specify a custom ID
  #define USERMOD_ID_EXAMPLE                2     //Usermod "usermod_v2_example.h"
  #define USERMOD_ID_TEMPERATURE            3     //Usermod "usermod_temperature.h"
  #define USERMOD_ID_FIXNETSERVICES         4     //Usermod "usermod_Fix_unreachable_netservices.h"
  #define USERMOD_ID_PIRSWITCH              5     //Usermod "usermod_PIR_sensor_switch.h"
  #define USERMOD_ID_IMU                    6     //Usermod "usermod_mpu6050_imu.h"
  #define USERMOD_ID_FOUR_LINE_DISP         7     //Usermod "usermod_v2_four_line_display.h
  #define USERMOD_ID_ROTARY_ENC_UI          8     //Usermod "usermod_v2_rotary_encoder_ui.h"
  #define USERMOD_ID_AUTO_SAVE              9     //Usermod "usermod_v2_auto_save.h"
  #define USERMOD_ID_DHT                   10     //Usermod "usermod_dht.h"
  #define USERMOD_ID_MODE_SORT             11     //Usermod "usermod_v2_mode_sort.h"
  #define USERMOD_ID_VL53L0X               12     //Usermod "usermod_vl53l0x_gestures.h"
  #define USERMOD_ID_MULTI_RELAY           13     //Usermod "usermod_multi_relay.h"
  #define USERMOD_ID_ANIMATED_STAIRCASE    14     //Usermod "Animated_Staircase.h"
  #define USERMOD_ID_RTC                   15     //Usermod "usermod_rtc.h"
  #define USERMOD_ID_ELEKSTUBE_IPS         16     //Usermod "usermod_elekstube_ips.h"
  #define USERMOD_ID_SN_PHOTORESISTOR      17     //Usermod "usermod_sn_photoresistor.h"
  #define USERMOD_ID_BATTERY               18     //Usermod "usermod_v2_battery.h"
  #define USERMOD_ID_PWM_FAN               19     //Usermod "usermod_PWM_fan.h"
  #define USERMOD_ID_BH1750                20     //Usermod "usermod_bh1750.h"
  #define USERMOD_ID_SEVEN_SEGMENT_DISPLAY 21     //Usermod "usermod_v2_seven_segment_display.h"
  #define USERMOD_RGB_ROTARY_ENCODER       22     //Usermod "rgb-rotary-encoder.h"
  #define USERMOD_ID_QUINLED_AN_PENTA      23     //Usermod "quinled-an-penta.h"
  #define USERMOD_ID_SSDR                  24     //Usermod "usermod_v2_seven_segment_display_reloaded.h"
  #define USERMOD_ID_CRONIXIE              25     //Usermod "usermod_cronixie.h"
  #define USERMOD_ID_WIZLIGHTS             26     //Usermod "wizlights.h"
  #define USERMOD_ID_WORDCLOCK             27     //Usermod "usermod_v2_word_clock.h"
  #define USERMOD_ID_MY9291                28     //Usermod "usermod_MY9291.h"
  #define USERMOD_ID_SI7021_MQTT_HA        29     //Usermod "usermod_si7021_mqtt_ha.h"
  #define USERMOD_ID_BME280                30     //Usermod "usermod_bme280.h
  #define USERMOD_ID_SMARTNEST             31     //Usermod "usermod_smartnest.h"
  #define USERMOD_ID_AUDIOREACTIVE         32     //Usermod "audioreactive.h"
  #define USERMOD_ID_ANALOG_CLOCK          33     //Usermod "Analog_Clock.h"
  #define USERMOD_ID_PING_PONG_CLOCK       34     //Usermod "usermod_v2_ping_pong_clock.h"
  #define USERMOD_ID_ADS1115               35     //Usermod "usermod_ads1115.h"
  #define USERMOD_ID_BOBLIGHT              36     //Usermod "boblight.h"
  #define USERMOD_ID_SD_CARD               37     //Usermod "usermod_sd_card.h"
  #define USERMOD_ID_PWM_OUTPUTS           38     //Usermod "usermod_pwm_outputs.h
  #define USERMOD_ID_SHT                   39     //Usermod "usermod_sht.h
  #define USERMOD_ID_KLIPPER               40     //Usermod Klipper percentage
  #define USERMOD_ID_WIREGUARD             41     //Usermod "wireguard.h"
  #define USERMOD_ID_INTERNAL_TEMPERATURE  42     //Usermod "usermod_internal_temperature.h"
  #define USERMOD_ID_LDR_DUSK_DAWN         43     //Usermod "usermod_LDR_Dusk_Dawn_v2.h"
  #define USERMOD_ID_STAIRWAY_WIPE         44     //Usermod "stairway-wipe-usermod-v2.h"

  class Usermod {
    protected:
      um_data_t *um_data; // um_data should be allocated using new in (derived) Usermod's setup() or constructor
    public:
      Usermod() { um_data = nullptr; }
      virtual ~Usermod() { if (um_data) delete um_data; }
      virtual void setup() = 0; // pure virtual, has to be overriden
      virtual void loop() = 0;  // pure virtual, has to be overriden
      virtual void handleOverlayDraw() {}                                      // called after all effects have been processed, just before strip.show()
      virtual bool handleButton(uint8_t b) { return false; }                   // button overrides are possible here
      virtual bool getUMData(um_data_t **data) { if (data) *data = nullptr; return false; }; // usermod data exchange [see examples for audio effects]
      virtual void connected() {}                                              // called when WiFi is (re)connected
      virtual void appendConfigData() {}                                       // helper function called from usermod settings page to add metadata for entry fields
      virtual void addToJsonState(JsonObject& obj) {}                          // add JSON objects for WLED state
      virtual void addToJsonInfo(JsonObject& obj) {}                           // add JSON objects for UI Info page
      virtual void readFromJsonState(JsonObject& obj) {}                       // process JSON messages received from web server
      virtual void addToConfig(JsonObject& obj) {}                             // add JSON entries that go to cfg.json
      virtual bool readFromConfig(JsonObject& obj) { return true; } // Note as of 2021-06 readFromConfig() now needs to return a bool, see usermod_v2_example.h
      virtual void onMqttConnect(bool sessionPresent) {}                       // fired when MQTT connection is established (so usermod can subscribe)
      virtual bool onMqttMessage(char* topic, char* payload) { return false; } // fired upon MQTT message received (wled topic)
      virtual void onUpdateBegin(bool) {}                                      // fired prior to and after unsuccessful firmware update
      virtual void onStateChange(uint8_t mode) {}                              // fired upon WLED state change
      virtual uint16_t getId() {return USERMOD_ID_UNSPECIFIED;}
  };

  #define WLED_MAX_USERMODS 4

  class UsermodManager {
    private:
      Usermod* ums[WLED_MAX_USERMODS];
      byte numMods = 0;

    public:
      void loop();
      void handleOverlayDraw();
      bool handleButton(uint8_t b);
      bool getUMData(um_data_t **um_data, uint8_t mod_id = USERMOD_ID_RESERVED); // USERMOD_ID_RESERVED will poll all usermods
      void setup();
      void connected();
      void appendConfigData();
      void addToJsonState(JsonObject& obj);
      void addToJsonInfo(JsonObject& obj);
      void readFromJsonState(JsonObject& obj);
      void addToConfig(JsonObject& obj);
      bool readFromConfig(JsonObject& obj);
      void onMqttConnect(bool sessionPresent);
      bool onMqttMessage(char* topic, char* payload);
      void onUpdateBegin(bool);
      void onStateChange(uint8_t);
      bool add(Usermod* um);
      Usermod* lookup(uint16_t mod_id);
      byte getModCount() {return numMods;};
  };

  UsermodManager usermods = UsermodManager();

  #endif // ENABLE_DEVFEATURE_LIGHT__INCLUDE_AUDIOREACTIVE_USERMOD


    /*****************************************************************************************************************************************************************************
    ********************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Animation Effect:   Ambilight   ***************************************************************************************************************************************************************************
    **  @note:     **************************************************************************************************************************************************************************
    ************************************************************************************************************************************************************************************
    **********************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/

    #ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
    // Move completely into its own class, with its own tasker
    /**************
     * Ambilight is light patterns around screens or pictures
     * PRESETS - patterns
     * INPUT_STREAM - set leds directly through mqtt (wallpaper) or serial lighting
    **************/ 
    enum AMBILIGHT_MODES_IDS{
      AMBILIGHT_PRESETS_ID=0,
      AMBILIGHT_SIDES_ID,
      AMBILIGHT_INPUT_STREAM_ID,
      AMBILIGHT_LENGTH_ID
    };
    int8_t GetAmbilightModeIDbyName(const char* c);
    const char* GetAmbilightModeName(char* buffer);

    /*******AMBILIGHT*********************************************************************************************/

    void SubTask_Ambilight_Main();
    void Ambilight_Sides();
    void Ambilight_Presets();
    void Ambilight_InputStream();
    void init_Ambilight();
    #define AMBILIGHT_SCREENS_CONNECTED 1

    /**
     * 
     * Long term, multiple screens will be used as their own subclass, containing all the info about that screen instead of struct arrays
     * */
    enum AMBILIGHT_SCREEN_INDEX{SCREEN_CENTRE=0,SCREEN_LEFT=1,SCREEN_RIGHT=2};

    struct AMBILIGHT_SCREEN_SETTINGS{
      uint8_t ambilight_mode = AMBILIGHT_SIDES_ID;
      uint32_t tSavedUpdate = millis();
      uint32_t ratemsSavedUpdate = 1000;
      uint8_t fUpdate = false;
      struct SCREENS{
        struct EDGE{
          RgbwwColor colour;
          int8_t blend_between_sides_gradient_percentage = -1; // -1 is unset/none/solid, 0% is bottom/left, 100% is top/right
          uint8_t size = 5;
        };
        struct EDGE top;
        struct EDGE bottom;
        struct EDGE left;
        struct EDGE right;
        struct FIRST_PIXEL{
          //uint8_t bottom_right_clockwise = store as packed bit? 
        }start_pixel;
      }screens[AMBILIGHT_SCREENS_CONNECTED]; //0,1,2 where 0 is centre and only screen

    }ambilightsettings;

    #endif

    /*****************************************************************************************************************************************************************************
    ********************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Palette related   ***************************************************************************************************************************************************************************
    **  @note:     **************************************************************************************************************************************************************************
    ************************************************************************************************************************************************************************************
    **********************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/

    // RgbwwColor IRAM_ATTR
    // GetPaletteColour_WithTemporaryLoad(
    //   uint16_t palette_id,
    //   uint16_t desired_index_from_palette = 0,
    //   bool     flag_spanned_segment = true, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
    //   bool     flag_wrap_hard_edge = true,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
    //   bool     flag_crgb_exact_colour = false,
    //   uint8_t* encoded_index = nullptr,
    //   bool flag_request_is_for_full_visual_output = false
    // );

    // RgbwwColor IRAM_ATTR GetUnloadedPaletteColour_ModeWrap( "modewrap" was the improved version, removing its name on newest version
    //   uint16_t palette_id,
    //   uint16_t index_or_pos = 0,
    //   uint8_t  palette_index_mode = 0,           // e.g. PALETTE_INDEX__IS_EXACT_COLOUR, PALETTE_INDEX__IS_SEGLEN_RANGE
    //   uint8_t  palette_mode = 0,                 // e.g. PALETTE_MODE__DEFAULT / __EXACT / __DISCRETE, etc.
    //   uint8_t  palette_wrap = 0,                 // e.g. PALETTE_WRAP_OFF / __ON
    //   uint8_t* encoded_value = nullptr,                // pass NO_ENCODED_VALUE if unused
    //   bool     flag_request_is_for_full_visual_output = false
    // );

    // uint32_t IRAM_ATTR
    // GetColourFromUnloadedPalette3_U32(
    //   uint16_t palette_id,
    //   uint16_t desired_index_from_palette = 0,
    //   bool     flag_spanned_segment = true, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
    //   bool     flag_wrap_hard_edge = true,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
    //   bool     flag_crgb_exact_colour = false,
    //   uint8_t* encoded_index = nullptr,
    //   bool flag_request_is_for_full_visual_output = false
    // ){
    //   RgbwwColor crgb_ = GetPaletteColour_WithTemporaryLoad(palette_id, desired_index_from_palette, flag_spanned_segment, flag_wrap_hard_edge, flag_crgb_exact_colour, encoded_index, flag_request_is_for_full_visual_output);
    //   return ((uint32_t)crgb_.CW << 24) | ((uint32_t)crgb_.R << 16) | ((uint32_t)crgb_.G << 8) | (uint32_t)crgb_.B;
    // }


    CRGB ColorFromPalette_WithLoad(const CRGBPalette16 &pal, uint8_t index, uint8_t brightness = (uint8_t)255U, TBlendType blendType = LINEARBLEND);
    

    uint32_t IRAM_ATTR GetPaletteColour_WithTemporaryLoad(
      uint16_t palette_id,
      uint16_t _pixel_position = 0,
      bool     flag_spanned_segment = true,
      bool     flag_wrap_hard_edge = true,
      bool     flag_crgb_exact_colour = false,
      uint8_t* encoded_value = nullptr,
      bool     flag_request_is_for_full_visual_output = false
    );


    const char* GetPaletteNameByID(uint16_t palette_id, char* buffer = nullptr, uint8_t buflen = 0);
    int16_t GetPaletteIDbyName(char* buffer);
    
    /*****************************************************************************************************************************************************************************
    ********************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Helper functions   ***************************************************************************************************************************************************************************
    **  @note:     **************************************************************************************************************************************************************************
    ************************************************************************************************************************************************************************************
    **********************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/
    
    inline uint16_t crc16(const unsigned char* data_p, size_t length) {
      uint8_t x;
      uint16_t crc = 0xFFFF;
      if (!length) return 0x1D0F;
      while (length--) {
        x = crc >> 8 ^ *data_p++;
        x ^= x>>4;
        crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x <<5)) ^ ((uint16_t)x);
      }
      return crc;
    }

    // // fastled beatsin: 1:1 replacements to remove the use of fastled sin16()
    // // Generates a 16-bit sine wave at a given BPM that oscillates within a given range. see fastled for details.
    // inline uint16_t beatsin88_t(accum88 beats_per_minute_88, uint16_t lowest = 0, uint16_t highest = 65535, uint32_t timebase = 0, uint16_t phase_offset = 0)
    // {
    //     uint16_t beat = beat88( beats_per_minute_88, timebase);
    //     uint16_t beatsin (sin16_t( beat + phase_offset) + 32768);
    //     uint16_t rangewidth = highest - lowest;
    //     uint16_t scaledbeat = scale16( beatsin, rangewidth);
    //     uint16_t result = lowest + scaledbeat;
    //     return result;
    // }

    // // Generates a 16-bit sine wave at a given BPM that oscillates within a given range. see fastled for details.
    // inline uint16_t beatsin16_t(accum88 beats_per_minute, uint16_t lowest = 0, uint16_t highest = 65535, uint32_t timebase = 0, uint16_t phase_offset = 0)
    // {
    //     uint16_t beat = beat16( beats_per_minute, timebase);
    //     uint16_t beatsin = (sin16_t( beat + phase_offset) + 32768);
    //     uint16_t rangewidth = highest - lowest;
    //     uint16_t scaledbeat = scale16( beatsin, rangewidth);
    //     uint16_t result = lowest + scaledbeat;
    //     return result;
    // }

    // // Generates an 8-bit sine wave at a given BPM that oscillates within a given range. see fastled for details.
    // inline uint8_t beatsin8_t(accum88 beats_per_minute, uint8_t lowest = 0, uint8_t highest = 255, uint32_t timebase = 0, uint8_t phase_offset = 0)
    // {
    //     uint8_t beat = beat8( beats_per_minute, timebase);
    //     uint8_t beatsin = sin8_t( beat + phase_offset);
    //     uint8_t rangewidth = highest - lowest;
    //     uint8_t scaledbeat = scale8( beatsin, rangewidth);
    //     uint8_t result = lowest + scaledbeat;
    //     return result;
    // }

    // Temporary helper functions to be cleaned up and converted
    uint32_t crgb_to_col(CRGB crgb);
    CRGB col_to_crgb(uint32_t);

    uint8_t get_random_wheel_index(uint8_t pos);
    uint8_t sin_gap(uint16_t in);
    uint16_t triwave16(uint16_t in);
    int8_t tristate_square8(uint8_t x, uint8_t pulsewidth, uint8_t attdec);
    
    // void colorFromUint32(uint32_t in, bool secondary = false);
    // void colorFromUint24(uint32_t in, bool secondary = false);
    // void relativeChangeWhite(int8_t amount, byte lowerBoundary = 0);
    // void colorHStoRGB(uint16_t hue, byte sat, byte* rgb); //hue, sat to rgb
    // void colorCTtoRGB(uint16_t mired, byte* rgb); //white spectrum to rgb
    // void colorFromDecOrHexString(byte* rgb, char* in);
    // void colorRGBtoRGBW(byte* rgb); //rgb to rgbw (http://codewelt.com/rgbw). (RGBW_MODE_LEGACY)

    // int32_t perlin1D_raw(uint32_t x, bool is16bit = false);
    // int32_t perlin2D_raw(uint32_t x, uint32_t y, bool is16bit = false);
    // int32_t perlin3D_raw(uint32_t x, uint32_t y, uint32_t z, bool is16bit = false);
    // uint16_t perlin16(uint32_t x);
    // uint16_t perlin16(uint32_t x, uint32_t y);
    // uint16_t perlin16(uint32_t x, uint32_t y, uint32_t z);
    // uint8_t perlin8(uint16_t x);
    // uint8_t perlin8(uint16_t x, uint16_t y);
    // uint8_t perlin8(uint16_t x, uint16_t y, uint16_t z);

    
/*
 * Fixed point integer based Perlin noise functions by @dedehai
 * Note: optimized for speed and to mimic fastled inoise functions, not for accuracy or best randomness
 */
#define PERLIN_SHIFT 1

// calculate gradient for corner from hash value
static inline __attribute__((always_inline)) int32_t hashToGradient(uint32_t h) {
  // using more steps yields more "detailed" perlin noise but looks less like the original fastled version (adjust PERLIN_SHIFT to compensate, also changes range and needs proper adustment)
  // return (h & 0xFF) - 128; // use PERLIN_SHIFT 7
  // return (h & 0x0F) - 8; // use PERLIN_SHIFT 3
  // return (h & 0x07) - 4; // use PERLIN_SHIFT 2
  return (h & 0x03) - 2; // use PERLIN_SHIFT 1 -> closest to original fastled version
}

// Gradient functions for 1D, 2D and 3D Perlin noise  note: forcing inline produces smaller code and makes it 3x faster!
static inline __attribute__((always_inline)) int32_t gradient1D(uint32_t x0, int32_t dx) {
  uint32_t h = x0 * 0x27D4EB2D;
  h ^= h >> 15;
  h *= 0x92C3412B;
  h ^= h >> 13;
  h ^= h >> 7;
  return (hashToGradient(h) * dx) >> PERLIN_SHIFT;
}

static inline __attribute__((always_inline)) int32_t gradient2D(uint32_t x0, int32_t dx, uint32_t y0, int32_t dy) {
  uint32_t h = (x0 * 0x27D4EB2D) ^ (y0 * 0xB5297A4D);
  h ^= h >> 15;
  h *= 0x92C3412B;
  h ^= h >> 13;
  return (hashToGradient(h) * dx + hashToGradient(h>>PERLIN_SHIFT) * dy) >> (1 + PERLIN_SHIFT);
}

static inline __attribute__((always_inline)) int32_t gradient3D(uint32_t x0, int32_t dx, uint32_t y0, int32_t dy, uint32_t z0, int32_t dz) {
  // fast and good entropy hash from corner coordinates
  uint32_t h = (x0 * 0x27D4EB2D) ^ (y0 * 0xB5297A4D) ^ (z0 * 0x1B56C4E9);
  h ^= h >> 15;
  h *= 0x92C3412B;
  h ^= h >> 13;
  return ((hashToGradient(h) * dx + hashToGradient(h>>(1+PERLIN_SHIFT)) * dy + hashToGradient(h>>(1 + 2*PERLIN_SHIFT)) * dz) * 85) >> (8 + PERLIN_SHIFT); // scale to 16bit, x*85 >> 8 = x/3
}

// fast cubic smoothstep: t*(3 - 2t²), optimized for fixed point, scaled to avoid overflows
static uint32_t smoothstep(const uint32_t t) {
  uint32_t t_squared = (t * t) >> 16;
  uint32_t factor = (3 << 16) - ((t << 1));
  return (t_squared * factor) >> 18; // scale to avoid overflows and give best resolution
}

// simple linear interpolation for fixed-point values, scaled for perlin noise use
static inline int32_t lerpPerlin(int32_t a, int32_t b, int32_t t) {
    return a + (((b - a) * t) >> 14); // match scaling with smoothstep to yield 16.16bit values
}

// 1D Perlin noise function that returns a value in range of -24691 to 24689
int32_t perlin1D_raw(uint32_t x, bool is16bit = false) {
  // integer and fractional part coordinates
  int32_t x0 = x >> 16;
  int32_t x1 = x0 + 1;
  if(is16bit) x1 = x1 & 0xFF; // wrap back to zero at 0xFF instead of 0xFFFF

  int32_t dx0 = x & 0xFFFF;
  int32_t dx1 = dx0 - 0x10000;
  // gradient values for the two corners
  int32_t g0 = gradient1D(x0, dx0);
  int32_t g1 = gradient1D(x1, dx1);
  // interpolate and smooth function
  int32_t tx = smoothstep(dx0);
  int32_t noise = lerpPerlin(g0, g1, tx);
  return noise;
}

// 2D Perlin noise function that returns a value in range of -20633 to 20629
int32_t perlin2D_raw(uint32_t x, uint32_t y, bool is16bit = false) {
  int32_t x0 = x >> 16;
  int32_t y0 = y >> 16;
  int32_t x1 = x0 + 1;
  int32_t y1 = y0 + 1;

  if(is16bit) {
    x1 = x1 & 0xFF; // wrap back to zero at 0xFF instead of 0xFFFF
    y1 = y1 & 0xFF;
  }

  int32_t dx0 = x & 0xFFFF;
  int32_t dy0 = y & 0xFFFF;
  int32_t dx1 = dx0 - 0x10000;
  int32_t dy1 = dy0 - 0x10000;

  int32_t g00 = gradient2D(x0, dx0, y0, dy0);
  int32_t g10 = gradient2D(x1, dx1, y0, dy0);
  int32_t g01 = gradient2D(x0, dx0, y1, dy1);
  int32_t g11 = gradient2D(x1, dx1, y1, dy1);

  uint32_t tx = smoothstep(dx0);
  uint32_t ty = smoothstep(dy0);

  int32_t nx0 = lerpPerlin(g00, g10, tx);
  int32_t nx1 = lerpPerlin(g01, g11, tx);

  int32_t noise = lerpPerlin(nx0, nx1, ty);
  return noise;
}

// 3D Perlin noise function that returns a value in range of -16788 to 16381
int32_t perlin3D_raw(uint32_t x, uint32_t y, uint32_t z, bool is16bit = false) {
  int32_t x0 = x >> 16;
  int32_t y0 = y >> 16;
  int32_t z0 = z >> 16;
  int32_t x1 = x0 + 1;
  int32_t y1 = y0 + 1;
  int32_t z1 = z0 + 1;

  if(is16bit) {
    x1 = x1 & 0xFF; // wrap back to zero at 0xFF instead of 0xFFFF
    y1 = y1 & 0xFF;
    z1 = z1 & 0xFF;
  }

  int32_t dx0 = x & 0xFFFF;
  int32_t dy0 = y & 0xFFFF;
  int32_t dz0 = z & 0xFFFF;
  int32_t dx1 = dx0 - 0x10000;
  int32_t dy1 = dy0 - 0x10000;
  int32_t dz1 = dz0 - 0x10000;

  int32_t g000 = gradient3D(x0, dx0, y0, dy0, z0, dz0);
  int32_t g001 = gradient3D(x0, dx0, y0, dy0, z1, dz1);
  int32_t g010 = gradient3D(x0, dx0, y1, dy1, z0, dz0);
  int32_t g011 = gradient3D(x0, dx0, y1, dy1, z1, dz1);
  int32_t g100 = gradient3D(x1, dx1, y0, dy0, z0, dz0);
  int32_t g101 = gradient3D(x1, dx1, y0, dy0, z1, dz1);
  int32_t g110 = gradient3D(x1, dx1, y1, dy1, z0, dz0);
  int32_t g111 = gradient3D(x1, dx1, y1, dy1, z1, dz1);

  uint32_t tx = smoothstep(dx0);
  uint32_t ty = smoothstep(dy0);
  uint32_t tz = smoothstep(dz0);

  int32_t nx0 = lerpPerlin(g000, g100, tx);
  int32_t nx1 = lerpPerlin(g010, g110, tx);
  int32_t nx2 = lerpPerlin(g001, g101, tx);
  int32_t nx3 = lerpPerlin(g011, g111, tx);
  int32_t ny0 = lerpPerlin(nx0, nx1, ty);
  int32_t ny1 = lerpPerlin(nx2, nx3, ty);

  int32_t noise = lerpPerlin(ny0, ny1, tz);
  return noise;
}

// scaling functions for fastled replacement
uint16_t perlin16(uint32_t x) {
  return ((perlin1D_raw(x) * 1159) >> 10) + 32803; //scale to 16bit and offset (fastled range: about 4838 to 60766)
}

uint16_t perlin16(uint32_t x, uint32_t y) {
 return ((perlin2D_raw(x, y) * 1537) >> 10) + 32725; //scale to 16bit and offset (fastled range: about 1748 to 63697)
}

uint16_t perlin16(uint32_t x, uint32_t y, uint32_t z) {
  return ((perlin3D_raw(x, y, z) * 1731) >> 10) + 33147; //scale to 16bit and offset (fastled range: about 4766 to 60840)
}

uint8_t perlin8(uint16_t x) {
  return (((perlin1D_raw((uint32_t)x << 8, true) * 1353) >> 10) + 32769) >> 8; //scale to 16 bit, offset, then scale to 8bit
}

uint8_t perlin8(uint16_t x, uint16_t y) {
  return (((perlin2D_raw((uint32_t)x << 8, (uint32_t)y << 8, true) * 1620) >> 10) + 32771) >> 8; //scale to 16 bit, offset, then scale to 8bit
}

uint8_t perlin8(uint16_t x, uint16_t y, uint16_t z) {
  return (((perlin3D_raw((uint32_t)x << 8, (uint32_t)y << 8, (uint32_t)z << 8, true) * 2015) >> 10) + 33168) >> 8; //scale to 16 bit, offset, then scale to 8bit
}


    /*****************************************************************************************************************************************************************************
    ********************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** To sort   ***************************************************************************************************************************************************************************
    **  @note:     **************************************************************************************************************************************************************************
    ************************************************************************************************************************************************************************************
    **********************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/




  void Segment_SubTask_Flasher_Animate_Function__TEST_SolidRandom();
  void Segments_SetLEDOutAmountByPercentage(uint8_t percentage, uint8_t segment_index = 0); 
  


  void setValuesFromMainSeg();
  
  inline void resetTimebase()                               { timebase = 0UL - millis(); }
    

  void setAllLeds();
  void setLedsStandard(bool justColors = false);
  bool colorChanged();

  void colorUpdated(byte callMode);

  


  #ifdef ENABLE_FEATURE_LIGHTING__WEBUI
  void updateInterfaces(uint8_t callMode);
  #endif

  void handleTransitions();

  
  float minf2(float v, float w);
  float maxf2(float v, float w);

  void Segment_AppendNew(uint16_t start_pixel, uint16_t stop_pixel, uint8_t seg_index = 0);

  void SetSegment_AnimFunctionCallback_WithoutAnimator(uint8_t seg_i = 0);

  int16_t extractModeDefaults(uint8_t mode, const char *segVar);
  bool extractModeDefaults(uint8_t mode, const char *segVar, char *outBuffer, size_t bufferSize);

    void Reset_CustomPalette_NamesDefault();

  /**
   * @brief 
   *
   * 
  * Segment, 72 bytes
  **/

  // #include "6_Lights/03_Animator/SubHeader_Segments.h" // Include the segment header file section, doing this to keep it easier to see
// #pragma     once

/**
 * @brief Stores at least 5 full RgbcctColours with all internal manipulations as needed
 * @NOTE: Replaces WLED 3 colour options
 **/
#ifndef CCT_MIN_DEFAULT
  #define CCT_MIN_DEFAULT 153          // 6500K
#endif
#ifndef CCT_MAX_DEFAULT
  #define CCT_MAX_DEFAULT 500          // 2000K
#endif

struct SegmentColour
{
  uint32_t colour = 0; // packed RGBW
  uint8_t cct = 127;   // 0 = warm, 255 = cold

  SegmentColour(
    uint8_t r = 1,
    uint8_t g = 2,
    uint8_t b = 3,
    uint8_t w = 4,
    uint8_t cct_balance = 127
  )
    : colour(RGBW32(r, g, b, w)),
      cct(cct_balance)
  {
  }


  SegmentColour(uint32_t colour_rgbw, uint8_t cct_balance = 127)
    : colour(colour_rgbw),
      cct(cct_balance)
  {
  }


  // -------------------------------------------------------------------------
  // Raw packed colour
  // -------------------------------------------------------------------------

  void setU32(uint32_t value)
  {
    colour = value;
  }


  uint32_t getU32() const
  {
    return colour;
  }


  uint32_t getU32Raw() const
  {
    return colour;
  }


  // -------------------------------------------------------------------------
  // Complete colour assignment
  // -------------------------------------------------------------------------

  void setRGB(uint8_t r, uint8_t g, uint8_t b)
  {
    colour = RGBW32(r, g, b, getWhite());
  }


  void setRGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w)
  {
    colour = RGBW32(r, g, b, w);
  }


  void setColour(uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0)
  {
    colour = RGBW32(r, g, b, w);
  }


  // -------------------------------------------------------------------------
  // Individual RGBW channels
  // -------------------------------------------------------------------------

  void setRed(uint8_t value)
  {
    colour = RGBW32(value, getGreen(), getBlue(), getWhite());
  }


  void setGreen(uint8_t value)
  {
    colour = RGBW32(getRed(), value, getBlue(), getWhite());
  }


  void setBlue(uint8_t value)
  {
    colour = RGBW32(getRed(), getGreen(), value, getWhite());
  }


  void setWhite(uint8_t value)
  {
    colour = RGBW32(getRed(), getGreen(), getBlue(), value);
  }


  uint8_t getRed() const
  {
    return R(colour);
  }


  uint8_t getGreen() const
  {
    return G(colour);
  }


  uint8_t getBlue() const
  {
    return B(colour);
  }


  uint8_t getWhite() const
  {
    return W(colour);
  }


  // -------------------------------------------------------------------------
  // CCT balance
  //
  // WLED convention:
  //   0   = warm white
  //   255 = cold white
  // -------------------------------------------------------------------------

  void setCCT(uint8_t cct_balance)
  {
    cct = cct_balance;
  }


  uint8_t getCCT() const
  {
    return cct;
  }


  uint8_t getCCTBalance() const
  {
    return cct;
  }


  /*
   * Compatibility conversion for old code that used a mired range:
   *
   *   153 = cold, approximately 6536 K
   *   500 = warm, 2000 K
   *
   * The old function was named setCCT_Kelvin(), but the supplied range
   * was actually mired rather than Kelvin.
   */
  void setCCT_Kelvin(uint16_t legacy_cct)
  {
    legacy_cct = constrain(legacy_cct, CCT_MIN_DEFAULT, CCT_MAX_DEFAULT);

    cct = static_cast<uint8_t>(
      map(
        legacy_cct,
        CCT_MIN_DEFAULT,
        CCT_MAX_DEFAULT,
        255,
        0
      )
    );
  }


  /*
   * Temporary compatibility method.
   *
   * Returns the old mired representation, despite the historic Kelvin name.
   */
  uint16_t getCCT_Kelvin() const
  {
    return static_cast<uint16_t>(
      map(
        cct,
        255,
        0,
        CCT_MIN_DEFAULT,
        CCT_MAX_DEFAULT
      )
    );
  }


  // -------------------------------------------------------------------------
  // Hue and saturation
  // -------------------------------------------------------------------------

  void setHue(uint16_t hue)
  {
    hue %= 360;

    HsbColor hsb = HsbColor(
      RgbColor(
        getRed(),
        getGreen(),
        getBlue()
      )
    );

    hsb.H = static_cast<float>(hue) / 360.0f;

    const RgbColor rgb = RgbColor(hsb);

    setRGB(
      rgb.R,
      rgb.G,
      rgb.B
    );
  }


  uint16_t getHue() const
  {
    const HsbColor hsb = HsbColor(
      RgbColor(
        getRed(),
        getGreen(),
        getBlue()
      )
    );

    return static_cast<uint16_t>(hsb.H * 360.0f);
  }


  void setSaturation(uint8_t saturation)
  {
    HsbColor hsb = HsbColor(
      RgbColor(
        getRed(),
        getGreen(),
        getBlue()
      )
    );

    hsb.S = static_cast<float>(saturation) / 255.0f;

    const RgbColor rgb = RgbColor(hsb);

    setRGB(
      rgb.R,
      rgb.G,
      rgb.B
    );
  }


  uint8_t getSaturation() const
  {
    const HsbColor hsb = HsbColor(
      RgbColor(
        getRed(),
        getGreen(),
        getBlue()
      )
    );

    return static_cast<uint8_t>(hsb.S * 255.0f);
  }


  // -------------------------------------------------------------------------
  // Temporary RgbwwColor compatibility
  // -------------------------------------------------------------------------

  /*
   * Convert the packed RGBW + CCT representation into the former
   * RgbwwColor representation.
   *
   * The packed W value is divided between WW and CW according to cct.
   */
  RgbwwColor getRgbwwColor() const
  {
    const uint8_t white = getWhite();

    const uint8_t cw = static_cast<uint8_t>(
      (static_cast<uint16_t>(white) * (static_cast<uint16_t>(cct) + 1U)) >> 8
    );

    const uint8_t ww = static_cast<uint8_t>(
      (static_cast<uint16_t>(white) * (256U - static_cast<uint16_t>(cct))) >> 8
    );

    return RgbwwColor(
      getRed(),
      getGreen(),
      getBlue(),
      ww,
      cw
    );
  }


  /*
   * Temporary conversion from the former RgbwwColor representation.
   *
   * WW + CW is reduced to one packed W channel. The relative CW share
   * becomes the CCT balance.
   */
  void setRgbwwColor(const RgbwwColor& value)
  {
    const uint16_t white_total =
      static_cast<uint16_t>(value.WW) +
      static_cast<uint16_t>(value.CW);

    const uint8_t packed_white =
      static_cast<uint8_t>(MIN(white_total, 255U));

    if (white_total > 0)
    {
      cct = static_cast<uint8_t>(
        (
          static_cast<uint32_t>(value.CW) *
          255U
        ) /
        white_total
      );
    }

    colour = RGBW32(
      value.R,
      value.G,
      value.B,
      packed_white
    );
  }


  /*
   * Temporary replacement for old code expecting WithBrightness().
   *
   * Per-colour RGB/CCT brightness is no longer applied here. Segment
   * opacity and global brightness are applied later in the WLED pipeline.
   */
  RgbwwColor WithBrightness() const
  {
    return getRgbwwColor();
  }


  // -------------------------------------------------------------------------
  // Temporary brightness compatibility
  //
  // These methods deliberately do not alter the colour.
  // They allow old callers to compile while brightness handling is migrated
  // to Segment::opacity and the global bus brightness.
  // -------------------------------------------------------------------------

  void setBrightnessRGB(uint8_t brightness)
  {
    (void)brightness;
  }


  uint8_t getBrightnessRGB() const
  {
    return 255;
  }


  void setBrightnessWW(uint8_t brightness)
  {
    (void)brightness;
  }


  uint8_t getBrightnessWW() const
  {
    return 255;
  }


  void setBrightnessCCT(uint8_t brightness)
  {
    (void)brightness;
  }


  uint8_t getBrightnessCCT() const
  {
    return 255;
  }
};



// typedef struct Segment 
class Segment 
{
  public:
    uint16_t start = 0; // start means first led index within segment : start index / start X coordinate 2D (left)
    uint16_t stop  = 0; // stop means total leds within the segment (not the index of last pixel) : stop index / stop X coordinate 2D (right); segment is invalid if stop == 0
    uint16_t offset = 0;

    uint8_t  speed               = DEFAULT_SPEED; // in my effect, may be the new time_ms
    uint8_t  intensity           = 127;           // should not relate to blending, only what the effect is doing
    uint16_t cycle_time__rate_ms = 2000; // EffectTimePeriod ie EffectCycleTime

    /**
     * @brief To enable Rgbcct to U32 conversions 
     */
    bool flag_brightness_set_in_effect = false;
    
    // Speed = 0 means blending is slowest and full cycle time is spent blending
    // Speed = 255 means blending is instant
    uint16_t animator_blend_time_ms(){ return (cycle_time__rate_ms * (255-speed))/255; } // time_ms
    

    uint16_t palette_id = 0; 
    uint16_t palette2_id = 0; // never loaded, but used as secondary instead of slider access
    uint8_t effect_id = 0;    
    uint8_t effect_id_next = 0;   //e.g. For rotating effect, preload the initial animation and then rotate it/


    /**
     * @brief Note with union here not having a name, all options are accesible directly in Segment
     **/
    union {
      uint16_t options; //bit pattern: msb first: [transposed mirrorY reverseY] transitional (tbd) paused needspixelstate mirrored on reverse selected
      struct {
        bool    selected    : 1;  //     0 : selected
        bool    reverse     : 1;  //     1 : reversed
        bool    on          : 1;  //     2 : is On
        bool    mirror      : 1;  //     3 : mirrored
        bool    freeze      : 1;  //     4 : paused/frozen
        bool    reset       : 1;  //     5 : indicates that Segment runtime requires reset
        bool    transitional: 1;  //     6 : transitional (there is transition occuring)
        bool    reverse_y   : 1;  //     7 : reversed Y (2D)
        bool    mirror_y    : 1;  //     8 : mirrored Y (2D)
        bool    transpose   : 1;  //     9 : transposed (2D, swapped X & Y)
        uint8_t map1D2D     : 3;  // 10-12 : mapping for 1D effect on 2D (0-use as strip, 1-expand vertically, 2-circular/arc, 3-rectangular/corner, ...)
        uint8_t soundSim    : 3;  // 13-15 : 0-7 sound simulation types
      };
    };

    /**
     * @brief New setting to reduce pixels generated for speed, and will replicate the data out to proper seglen
     * For example, with 1000 led segment, if the same pattern is generated every 10 leds, then only 100 leds will be generated and pushed out 10 times on the bus
     * 
     * Decimate is a multiplier, so 0 means none, with each value being a divisor. Decimate 10 would be only generate 10% of the leds, then repeat it 10 times on the output
     */
    uint8_t decimate = 0;
    uint8_t grouping = 1;
    uint8_t  spacing = 0;
    uint8_t  opacity = 255,  cct;       // 0==1900K, 255==10091K

    inline void grouping_set(uint8_t g){ grouping = g > 0 ? g : 1; } // Can never be ZERO
    inline uint8_t grouping_get(void){ return grouping; } // Can never be ZERO

    /**
     * @brief
     * The colours that are generated directly within the effect function, and is used to define the type of storage buffered required to hold the colour buffer
     * This should work in tandem with another variable, which can define how the white components are handled
     * Option 1: Only RGB is created, white is ignored
     * Option 2: Effect creates colour and white components
     * Option 3: Effects creates colours, but white is generated after based on different options
     * 
     * While it describes RGB, WRGB, RGBWW, WWA etc, in reality, it is treated as the byte width for buffers directly. 
     **/
    ColourType colour_width__used_in_effect_generate = ColourType::COLOUR_TYPE__RGB__ID; 
    
    // Define the size of the color array
    // #define NUMBER_SEGMENT_COLOURS 5

    // // Initialize the array with default values
    // SegmentColour segcol[NUMBER_SEGMENT_COLOURS] =  // colors[i] -> colors[i]
    // {
    //   SegmentColour(255, 0, 0, 0, 0), // Red
    //   SegmentColour(0, 255, 0, 0, 0), // Green
    //   SegmentColour(0, 0, 255, 0, 0), // Blue
    //   SegmentColour(255, 0, 255, 0, 0), // Magenta
    //   SegmentColour(255, 255, 0, 0, 0)  // Yellow
    // };
    // uint32_t colors[NUM_COLORS] = {255}; // temporary replicate to make wled compile

    // void set_colors(uint8_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t ww, uint8_t cw, uint8_t br_rgb = 255, uint8_t br_ww = 255) {
    //     if (index >= NUMBER_SEGMENT_COLOURS) {
    //         Serial.println("ERROR: Index out of bounds");
    //         return;
    //     }

    //     segcol[index] = SegmentColour(r, g, b, ww, cw, br_rgb, br_ww);
    // }

    // Define the size of the colour array
    #define NUMBER_SEGMENT_COLOURS 5

    // Primary segment colour storage
    SegmentColour segcol[NUMBER_SEGMENT_COLOURS] =
    {
      SegmentColour(255,   0,   0, 0, 127), // Red
      SegmentColour(  0, 255,   0, 0, 127), // Green
      SegmentColour(  0,   0, 255, 0, 127), // Blue
      SegmentColour(255,   0, 255, 0, 127), // Magenta
      SegmentColour(255, 255,   0, 0, 127)  // Yellow
    };

    void set_colors(
      uint8_t index,
      uint8_t r,
      uint8_t g,
      uint8_t b,
      uint8_t w,
      uint8_t cct = 127,
      uint8_t br_rgb = 255,
      uint8_t br_ww = 255
    )
    {
      if (index >= NUMBER_SEGMENT_COLOURS)
      {
        Serial.println("ERROR: Index out of bounds");
        return;
      }

      // New representation
      segcol[index].setRGBW(r, g, b, w);
      segcol[index].setCCT(cct);

      // Temporary compatibility only.
      // These currently do nothing and can be removed once old callers are migrated.
      segcol[index].setBrightnessRGB(br_rgb);
      segcol[index].setBrightnessWW(br_ww);

    }



    ANIMATION_FLAGS flags;
    

    // Effects (Scenes & Flasher), Ambilight, Adalight    
    uint8_t animation_mode_id = 0; // rename to "effect_id"

    
    // removing, as name is ambiguous now without neopixel animator
    uint16_t get_transition_rate_ms() // Effect that require call for every update, must be called at FRAMETIME_MS, otherwise, can manually be set
    {
      // #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
      // if(effect_id >= WLED_GROUP_IDS_FIRST)
      // {
        return FRAMETIME;
      // }
      // else
      //   return cycle_time__rate_ms;
      // #endif
      //   return cycle_time__rate_ms;
    };

    uint16_t get_effect_period()
    {
      return cycle_time__rate_ms;
    }

    
    // /**
    //  * This allows the segment to have optional brightness per segment, by default is set to maximum
    //  * and will therefore have no effect 
    //  **/
    // uint8_t _brightness_rgb = 255;
    // uint8_t _brightness_cct = 255;

    // #define _segBri _brightness_rgb //wled fix

    // uint8_t _brightness_rgb_combined = 255;
    // uint8_t _brightness_cct_combined = 255;
    // IRAM_ATTR void UpdateBrightness();
    
    // IRAM_ATTR void setBrightnessRGB(uint8_t b)
    // {
    //   #ifdef ENABLE_DEBUGFEATURE_LIGHTS__SEGMENT_BRIGHTNESS_LIMIT_VALUE
    //   if(b > ENABLE_DEBUGFEATURE_LIGHTS__SEGMENT_BRIGHTNESS_LIMIT_VALUE)
    //   {
    //     b = ENABLE_DEBUGFEATURE_LIGHTS__SEGMENT_BRIGHTNESS_LIMIT_VALUE;
    //   }
    //   #endif
    //   _brightness_rgb = b;
    // }

    // IRAM_ATTR void setBrightnessCCT(uint8_t bri_cct)
    // {
    //   _brightness_cct = bri_cct;
    // }

    // IRAM_ATTR uint8_t getBrightnessRGB() const
    // {
    //   return _brightness_rgb;
    // };

    // IRAM_ATTR uint8_t getBrightnessCCT() const
    // {
    //   return _brightness_cct;
    // };

    // IRAM_ATTR inline uint8_t getBrightnessRGB_WithGlobalApplied() const
    // {
    //   return _brightness_rgb_combined;
    // }

    // IRAM_ATTR inline uint8_t getBrightnessCCT_WithGlobalApplied() const
    // {
    //   return _brightness_cct_combined;
    // }

    
    uint8_t  currentCCT() const; // current segment's CCT (blended while in transition)
    uint8_t  currentBri() const; // current segment's opacity/brightness (blended while in transition)


    // Flags and states that are used during one transition and reset when completed
    struct ANIMATION_SINGLE_USE_OVERRIDES_ANYTIME
    {
      // uint8_t fRefreshAllPixels = false;
      /**
       * Can't be zero, as that means not active
       * */
      uint16_t time_ms = 1000; //on boot
      uint16_t cycle_time__rate_ms__save_state = 0;
      // uint16_t cycle_time__rate_ms = 1000;
    }single_animation_override; // ie "oneshot" variables that get checked and executed one time only

    /**
     * @brief These should be moved elsewhere as the optional defaults for times, that are loaded into the above override when needed
     * 
     */
    struct ANIMATION_SINGLE_USE_OVERRIDES_TURNING_OFF
    {
      // uint8_t fRefreshAllPixels = false;
      /**
       * Can't be zero, as that means not active
       * */
      uint16_t time_ms = 0; //on boot
      // uint16_t cycle_time__rate_ms = 1000;
    }single_animation_override_turning_off; // ie "oneshot" variables that get checked and executed one time only

    

    // uint16_t groupLength() { return grouping + spacing; }
    // uint16_t virtualLength()
    // {
    //   uint16_t groupLen = groupLength();
    //   uint16_t vLength = (length() + groupLen - 1) / groupLen;
    //   if(mirror) vLength = (vLength + 1) /2;  // divide by 2 if mirror, leave at least a signle LED

    //   #ifdef ENABLE_FEATURE_LIGHTS__DECIMATE_PIXELS
    //   if(decimate > 1)   // only shrink when factor >= 2. 0 and 1 means no decimation
    //   {
    //     vLength = (vLength + decimate - 1) / decimate;
    //   }
    //   #endif // ENABLE_FEATURE_LIGHTS__DECIMATE_PIXELS

    //   return vLength;
    // }
    // #define vLength() virtualLength() //tmp fix    
    // inline static unsigned vWidth()                        { return virtualLength(); }
    
    inline unsigned nrOfVStrips() const {        // returns number of virtual vertical strips in 2D matrix (used to expand 1D effects into 2D)
    #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
      return (is2D() &&  map1D2D == M12_pBar) ? virtualWidth() : 1;
    #else
      return 1;
    #endif
    }


    #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PERFORMANCE_METRICS_SAFE_IN_RELEASE_MODE
    struct PERFORMANCE{

      uint32_t effect_build_us = 0;
      uint32_t bus_write_single_us = 0; // SetPixelColour to NPB (ignore decimate, includes initial write then times decimate amount)
      uint32_t bus_read_single_us = 0;  // GetPixelColour from NPB (ignores decimate, includes full time)
      uint32_t bus_write_total_us = 0; // SetPixelColour to NPB (ignore decimate, includes initial write then times decimate amount)
      uint32_t bus_read_total_us = 0;  // GetPixelColour from NPB (ignores decimate, includes full time)

      uint32_t millis_last_show = 0;
      uint32_t elapsed_last_show = 0;
      float fps = 0;

    }performance;
    #endif // ENABLE_DEBUGFEATURE_LIGHTING__PERFORMANCE_METRICS_SAFE_IN_RELEASE_MODE

    void setUp(uint16_t i1, uint16_t i2, uint8_t grp=1, uint8_t spc=0, uint16_t ofs=UINT16_MAX, uint16_t i1Y=0, uint16_t i2Y=1);
    
    uint8_t  cct_slider;                 //0==1900K, 255==10091K


    uint8_t custom1; //3c1=Custom1
    uint8_t custom2; //4c2=Custom2   // custom FX parameters/sliders  
    struct {
      uint8_t custom3 : 5;        //5c3=Custom3    // reduced range slider (0-31)
      bool    check1  : 1;        //6cbPal=Check1  // checkmark 1 PaletteIcon
      bool    check2  : 1;        //7cbLay=Check2  // checkmark 2 ?
      bool    check3  : 1;        //8cbFav=Check3  // checkmark 3 OverlayIcon
    };
    
    uint8_t startY;  // start Y coodrinate 2D (top); there should be no more than 255 rows
    uint8_t stopY;   // stop Y coordinate 2D (bottom); there should be no more than 255 rows
    uint8_t   blendMode;          // segment blending modes: top, bottom, add, subtract, difference, average, multiply, divide, lighten, darken, screen, overlay, hardlight, softlight, dodge, burn, stencil
    char *name = nullptr; // Keep, segment name to be added later by me
    char* getName() { return name; } // Get the name of the segment
    
    // runtime data
    // unsigned long next_time;  // millis() of next update
    uint32_t tSaved_EffectStartReferenceTime = 0;
    uint32_t step;  // custom "step" var
    uint32_t call;  // call counter
    uint32_t effect_init_runtime = 0; // when effect was initialised

    uint32_t tSaved_AnimateRunTime = 0; //tmp fix ofr xmas24 then remove

    bool     _colorScaled;             // color has been scaled prior to setPixelColor() call
    // static bool          _modeBlend;// = true;          // mode/effect blending semaphore

    
    uint16_t aux0 = 0;  // custom var
    uint16_t aux1 = 0;  // custom var
    uint16_t aux2 = 0;
    uint32_t aux3 = 0; // Also used for random CRGBPALETTE16 timing
    uint16_t aux4 = 0; // New when it is needed but not worth a struct data


    // uint32_t live_pal_timing = 0; //for live palette updates was previously aux3
    // uint8_t palette_live_intensity = 127;
   struct LivePalette{
    uint32_t timing1 = 0;   // shared: randomise last-update OR segcol-cycle anchor
    uint8_t  intensity = 127;
  } live_palette;


    
    // transition data, holds values during transition (76 bytes/28 bytes)
    struct Transition {
      Segment      *_oldSegment;          // previous segment environment (may be nullptr if effect did not change)
      unsigned long _start;               // must accommodate millis()
      uint32_t      _colors[5];  // current colors
      #ifndef WLED_SAVE_RAM
      CRGBPalette16 _palT;                // temporary palette (slowly being morphed from old to new)
      #endif
      uint16_t      _dur;                 // duration of transition in ms
      uint16_t      _progress;            // transition progress (0-65535); pre-calculated from _start & _dur in updateTransitionProgress()
      uint8_t       _prevPaletteBlends;   // number of previous palette blends (there are max 255 blends possible)
      uint8_t       _palette, _bri, _cct; // palette ID, brightness and CCT at the start of transition (brightness will be 0 if segment was off)
      Transition(uint16_t dur=750)
      : _oldSegment(nullptr)
      , _start(millis())
      , _colors{0,0,0}
      #ifndef WLED_SAVE_RAM
      , _palT(CRGBPalette16())
      #endif
      , _dur(dur)
      , _progress(0)
      , _prevPaletteBlends(0)
      , _palette(0)
      , _bri(0)
      , _cct(0)
      {}
      ~Transition() {
        //DEBUGFX_PRINTF_P(PSTR("-- Destroying transition: %p\n"), this);
        if (_oldSegment) delete _oldSegment;
        _oldSegment = nullptr;
      }
    } *_t;



    Decounter<uint16_t> auto_timeoff = Decounter<uint16_t>();

    /**
     * @brief 
     * External user controllable multi-use variables
     */
    uint16_t params_user[4] = {0};

    uint32_t *pixels;                 // pixel data
    uint16_t pixels_allocated_bytes = 0;
    
    
    static uint16_t maxWidth, maxHeight;  // these define matrix width & height (max. segment dimensions)
    
  // private:
    union {
      uint8_t  _capabilities;
      struct {
        bool    _isRGB    : 1;
        bool    _hasW     : 1;
        bool    _isCCT    : 1;
        bool    _manualW  : 1;
        uint8_t _reserved : 4;
      };
    };

    // static variables are use to speed up effect calculations by stashing common pre-calculated values
    // static unsigned      _usedSegmentData;    // amount of data used by all segments
    static unsigned      _vLength;            // 1D dimension used for current effect
    static unsigned      _vWidth, _vHeight;   // 2D dimensions used for current effect
    static uint32_t      _currentColors[NUM_COLORS]; // colors used for current effect (faster access from effect functions)
    static CRGBPalette16 _currentPalette;     // palette used for current effect (includes transition, used in color_from_palette())
    static CRGBPalette16 _randomPalette;      // actual random palette
    static CRGBPalette16 _newRandomPalette;   // target random palette
    static uint16_t      _lastPaletteChange;  // last random palette change time (in seconds)
    static uint16_t      _nextPaletteBlend;   // next due time for random palette morph (in millis())
    static bool          _modeBlend;          // mode/effect blending semaphore
    // clipping rectangle used for blending
    static uint16_t      _clipStart, _clipStop;
    static uint8_t       _clipStartY, _clipStopY;
        
    /***
     * Effect datastorage
     ***/
    byte* data;     // effect data pointer
    uint16_t _dataLen;
    static uint16_t _usedSegmentData;
    inline byte* Data(){ return data; };
    inline uint16_t DataLength(){ return _dataLen; };

    mPaletteLoaded* palette_loaded = nullptr;

    mPaletteLoaded* palette2_loaded = nullptr;
    bool EnsurePalette2Loaded(uint16_t palette_id);
    void ReleasePalette2();
    uint32_t GetPalette2Colour(
      uint16_t pixel_position,
      uint8_t palette_index__format = PALETTE_INDEX__IS_255_RANGE,
      uint8_t force_palette_mode = PALETTE_MODE__DEFAULT,
      uint8_t flag_wrap_hard_edge = PALETTE_WRAP_HARDEDGE,
      uint8_t* encoded_value = NO_ENCODED_VALUE,
      bool apply_brightness = false,
      uint8_t pbri = 255,
      uint8_t mcol = 0
    );

    void Update_LivePalettes(
      uint16_t pal_id        = 0xFFFF,  // default: use segment.palette_id
      uint16_t preview_index = 0,       // default: unused unless preview_mode=true
      bool     preview_mode  = false    // default: normal runtime update
    );
    void Update_LivePalette(
      uint16_t pal_id,
      mPaletteLoaded* palette_container,
      uint16_t preview_index = 0,
      bool preview_mode = false
    );



    uint8_t palette = 1;
    
    uint8_t GetNumberOfColoursInPalette(){ return palette_loaded->colours_in_palette; };
  
    bool LoadPalette_AsyncLock = false;
    void LoadPalette(uint8_t palette_id, mPaletteLoaded* palette = nullptr);

    uint32_t tSaved_LastUpdated = millis();
    uint32_t tTick_maximum_call_ms = 10;

    // Segment(uint16_t sStart=0, uint16_t sStop=30, const char* segment_name = nullptr) :
    //   start(sStart),
    //   stop(sStop),
    //   offset(0),
    //   speed(DEFAULT_SPEED),
    //   intensity(DEFAULT_INTENSITY),
    //   effect_id(DEFAULT_MODE),
    //   options(SELECTED | SEGMENT_ON),
    //   grouping(1),
    //   spacing(0),
    //   cct_slider(127),
    //   custom1(DEFAULT_C1),
    //   custom2(DEFAULT_C2),
    //   custom3(DEFAULT_C3),
    //   check1(false),
    //   check2(false),
    //   check3(false),
    //   startY(0),
    //   stopY(1),
    //   name(nullptr),
    //   next_time(0),
    //   step(0),
    //   call(0),
    //   data(nullptr),
    //   coldata(nullptr),
    //   pixels(nullptr),
    //   palette_loaded(nullptr),
    //   _t(nullptr),
    //   _capabilities(0),
    //   _dataLen(0),
    //   _coldataLen(0)
    // {
    //     #ifdef ENABLE_DEBUGFEATURE_LIGHT__SEGMENTS
    //     Serial.printf(
    //       "-- Creating segment: S=%p [%u,%u:%u,%u] T[%p]\n",
    //       this,
    //       (unsigned)start,
    //       (unsigned)stop,
    //       (unsigned)startY,
    //       (unsigned)stopY,
    //       (void*)_t
    //     );
    //     #endif

    //     refreshLightCapabilities();

    //     if (segment_name)
    //     {
    //       name = new(std::nothrow) char[strlen(segment_name) + 1];

    //       if (name)
    //       {
    //         strcpy(name, segment_name);
    //       }
    //     }

    //     aux0 = 0;
    //     aux1 = 0;
    //     aux2 = 0;
    //     aux3 = 0;

    //     palette_loaded = new(std::nothrow) mPaletteLoaded();
    //     // animator = new(std::nothrow) NeoPixelAnimator(1, NEO_MILLISECONDS);

    //     pixels = static_cast<uint32_t*>(
    //       allocate_buffer(
    //         length() * sizeof(uint32_t),
    //         BFRALLOC_PREFER_PSRAM |
    //         BFRALLOC_NOBYTEACCESS |
    //         BFRALLOC_CLEAR
    //       )
    //     );

    //     if (!pixels)
    //     {
    //       DEBUG_PRINTLN(F("!!! Not enough RAM for pixel buffer !!!"));
    //       stop = 0;
    //     }
    // }
Segment(
  uint16_t sStart = 0,
  uint16_t sStop = 30,
  const char* segment_name = nullptr
) :
  start(sStart),
  stop(sStop),
  offset(0),
  speed(DEFAULT_SPEED),
  intensity(DEFAULT_INTENSITY),
  effect_id(DEFAULT_MODE),
  options(SELECTED | SEGMENT_ON),
  grouping(1),
  spacing(0),
  cct_slider(127),
  custom1(DEFAULT_C1),
  custom2(DEFAULT_C2),
  custom3(DEFAULT_C3),
  check1(false),
  check2(false),
  check3(false),
  startY(0),
  stopY(1),
  name(nullptr),
  // next_time(0),
  step(0),
  call(0),
  data(nullptr),
  pixels(nullptr),
  palette_loaded(nullptr),
  _t(nullptr),
  _capabilities(0),
  _dataLen(0)
{
  #ifdef ENABLE_DEBUGFEATURE_LIGHT__SEGMENTS
  Serial.printf(
    "-- Creating segment: S=%p [%u,%u:%u,%u] T[%p]\n",
    this,
    (unsigned)start,
    (unsigned)stop,
    (unsigned)startY,
    (unsigned)stopY,
    (void*)_t
  );
  #endif

  refreshLightCapabilities();

  if (segment_name)
  {
    name = new(std::nothrow) char[strlen(segment_name) + 1];

    if (name)
    {
      strcpy(name, segment_name);
    }
  }

  aux0 = 0;
  aux1 = 0;
  aux2 = 0;
  aux3 = 0;

  /*
   * PulSar addition.
   *
   * WLED does not own this palette container as part of Segment.
   */
  palette_loaded = new(std::nothrow) mPaletteLoaded();

  pixels = static_cast<uint32_t*>(
    allocate_buffer(
      static_cast<size_t>(length()) * sizeof(uint32_t),
      BFRALLOC_PREFER_PSRAM |
      BFRALLOC_NOBYTEACCESS |
      BFRALLOC_CLEAR
    )
  );

  if (!pixels)
  {
    DEBUG_PRINTLN(F("!!! Not enough RAM for pixel buffer !!!"));
    stop = 0;
  }

  if (!palette_loaded)
  {
    DEBUG_PRINTLN(F("!!! Not enough RAM for palette container !!!"));
  }
}




    // Segment(uint16_t sStartX, uint16_t sStopX, uint16_t sStartY, uint16_t sStopY) : Segment(sStartX, sStopX) {
    //   startY = sStartY;
    //   stopY  = sStopY;
    // }

    Segment(
  uint16_t sStartX,
  uint16_t sStopX,
  uint16_t sStartY,
  uint16_t sStopY
) :
  Segment(sStartX, sStopX)
{
  startY = sStartY;
  stopY  = sStopY;

  /*
   * The delegated constructor allocated pixels while height() was still 1.
   * Reallocate using the completed 2D geometry.
   */
  p_free(pixels);
  pixels = nullptr;

  pixels = static_cast<uint32_t*>(
    allocate_buffer(
      static_cast<size_t>(length()) * sizeof(uint32_t),
      BFRALLOC_PREFER_PSRAM |
      BFRALLOC_NOBYTEACCESS |
      BFRALLOC_CLEAR
    )
  );

  if (!pixels)
  {
    DEBUG_PRINTLN(F("!!! Not enough RAM for 2D pixel buffer !!!"));
    stop = 0;
    stopY = 0;
  }
}

    // Segment(const Segment &orig) // copy constructor
    // {
    //   #ifdef ENABLE_DEBUGFEATURE_LIGHT__SEGMENTS
    //   Serial.println(F("-- Copy segment constructor --"));
    //   #endif

    //   memcpy(this, &orig, sizeof(Segment));
    //   _t   = nullptr; // copied segment cannot be in transition
    //   name = nullptr;
      
    //   if (orig.name){ 
    //     name = new char[strlen(orig.name)+1]; 
    //     if (name){
    //       strcpy(name, orig.name); 
    //     }
    //   }

    //   data = nullptr; _dataLen = 0;
    //   if (orig.data){ 
    //     if (allocateData(orig._dataLen)) 
    //     {
    //       memcpy(data, orig.data, orig._dataLen); 
    //     }
    //   }
      
    //   coldata = nullptr; _coldataLen = 0;
    //   if (orig.coldata) {
    //     if(allocateColourData(orig._coldataLen)){
    //       memcpy(coldata, orig.coldata, orig._coldataLen);
    //     }
    //   }

    // };
    // Segment(const Segment& orig)
    // {
    //     #ifdef ENABLE_DEBUGFEATURE_LIGHT__SEGMENTS
    //     Serial.printf(
    //         "-- Copy segment constructor: %p -> %p\n",
    //         (const void*)&orig,
    //         (void*)this
    //     );
    //     #endif

    //     /*
    //     * Copy the complete scalar/non-owning state first.
    //     *
    //     * Every owned pointer must immediately be detached before any operation
    //     * which can return or fail.
    //     */
    //     memcpy(this, &orig, sizeof(Segment));

    //     /*
    //     * A copied segment must not share ownership of runtime allocations.
    //     */
    //     _t             = nullptr;
    //     name           = nullptr;
    //     data           = nullptr;
    //     coldata        = nullptr;
    //     pixels         = nullptr;
    //     palette_loaded = nullptr;

    //     _dataLen    = 0;
    //     _coldataLen = 0;

    //     /*
    //     * Inactive source: retain copied configuration, but do not allocate
    //     * runtime buffers.
    //     */
    //     if (!orig.stop)
    //     {
    //         return;
    //     }

    //     /*
    //     * Deep-copy the segment pixel-render buffer.
    //     */
    //     if (orig.pixels && orig.length() > 0)
    //     {
    //         const size_t pixel_bytes =
    //         static_cast<size_t>(orig.length()) * sizeof(uint32_t);

    //         pixels = static_cast<uint32_t*>(
    //         allocate_buffer(
    //             pixel_bytes,
    //             BFRALLOC_PREFER_PSRAM |
    //             BFRALLOC_NOBYTEACCESS
    //         )
    //         );

    //         if (pixels)
    //         {
    //         memcpy(pixels, orig.pixels, pixel_bytes);
    //         }
    //         else
    //         {
    //         DEBUG_PRINTLN(F("!!! Not enough RAM for copied segment pixel buffer !!!"));
    //         stop = 0;
    //         return;
    //         }
    //     }
    //     else
    //     {
    //         stop = 0;
    //         return;
    //     }

    //     /*
    //     * Deep-copy the segment name.
    //     */
    //     if (orig.name)
    //     {
    //         name = new(std::nothrow) char[strlen(orig.name) + 1];

    //         if (name)
    //         {
    //         strcpy(name, orig.name);
    //         }
    //     }

    //     /*
    //     * Deep-copy effect runtime data.
    //     */
    //     if (orig.data && orig._dataLen > 0)
    //     {
    //         if (allocateData(orig._dataLen))
    //         {
    //         memcpy(data, orig.data, orig._dataLen);
    //         }
    //     }

    //     /*
    //     * Deep-copy colour runtime data.
    //     */
    //     if (orig.coldata && orig._coldataLen > 0)
    //     {
    //         if (allocateColourData(orig._coldataLen))
    //         {
    //         memcpy(coldata, orig.coldata, orig._coldataLen);
    //         }
    //     }

    //     /*
    //     * palette_loaded appears to be owned by the Segment.
    //     *
    //     * This assumes mPaletteLoaded has a valid copy constructor.
    //     */
    //     if (orig.palette_loaded)
    //     {
    //         palette_loaded =
    //         new(std::nothrow) mPaletteLoaded(*orig.palette_loaded);
    //     }
    //     else
    //     {
    //         palette_loaded =
    //         new(std::nothrow) mPaletteLoaded();
    //     }
    // }

    Segment(const Segment& orig)
{
  #ifdef ENABLE_DEBUGFEATURE_LIGHT__SEGMENTS
  // Serial.println(F("-- Copy segment constructor --"));
  #endif

  /*
   * WLED pattern:
   * copy complete scalar/non-owning object state first.
   */
  memcpy(this, &orig, sizeof(Segment));

  /*
   * A copied segment cannot share the source transition or owned allocations.
   */
  _t             = nullptr;
  name           = nullptr;
  data           = nullptr;
  pixels         = nullptr;
  palette_loaded = nullptr;
  
  _dataLen    = 0;

  /*
   * Inactive segments do not require runtime allocations.
   */
  if (!orig.stop)
  {
    return;
  }

  /*
   * Deep-copy segment name.
   */
  if (orig.name)
  {
    name = new(std::nothrow) char[strlen(orig.name) + 1];

    if (name)
    {
      strcpy(name, orig.name);
    }
  }

  /*
   * Deep-copy WLED effect runtime data.
   */
  if (orig.data && orig._dataLen > 0)
  {
    if (allocateData(orig._dataLen))
    {
      memcpy(data, orig.data, orig._dataLen);
    }
  }

  /*
   * Deep-copy the segment framebuffer.
   */
  if (orig.pixels && orig.length() > 0)
  {
    const size_t pixel_bytes =
      static_cast<size_t>(orig.length()) * sizeof(uint32_t);

    pixels = static_cast<uint32_t*>(
      allocate_buffer(
        pixel_bytes,
        BFRALLOC_PREFER_PSRAM |
        BFRALLOC_NOBYTEACCESS
      )
    );

    if (pixels)
    {
      memcpy(pixels, orig.pixels, pixel_bytes);
    }
    else
    {
      DEBUG_PRINTLN(F("!!! Not enough RAM for copied segment pixel buffer !!!"));
      stop = 0;
      return;
    }
  }

  /*
   * PulSar addition:
   * palette_loaded is an owned object and must not remain shallow-copied.
   */
  if (orig.palette_loaded)
  {
    palette_loaded =
      new(std::nothrow) mPaletteLoaded(*orig.palette_loaded);
  }
  else
  {
    palette_loaded =
      new(std::nothrow) mPaletteLoaded();
  }

  // Serial.printf(("SEG ALLOCATIONS: obj=%p name=%p nameActual=%u palette=%p paletteActual=%u pixels=%p pixelActual=%u\n\r"), (void*)this, (void*)name, name ? (unsigned)heap_caps_get_allocated_size(name) : 0, (void*)palette_loaded, palette_loaded ? (unsigned)heap_caps_get_allocated_size(palette_loaded) : 0, (void*)pixels, pixels ? (unsigned)heap_caps_get_allocated_size(pixels) : 0);


}

    void NameUpdate(const char* new_name)
    {
      if (name) {
        p_free(name);
name = nullptr;
      }

      if (new_name) {
        size_t len = strlen(new_name);
        name = new char[len + 1];
        if (name) {
          strcpy(name, new_name);
        }
      }
    }


    // Segment(Segment &&orig) noexcept // move constructor
    // {

    //   #ifdef ENABLE_DEBUGFEATURE_LIGHT__SEGMENTS
    //   Serial.println(F("-- Move segment constructor --"));
    //   #endif

    //   memcpy((void*)this, (void*)&orig, sizeof(mAnimatorLight::Segment));

    //   orig._t   = nullptr; // old segment cannot be in transition any more

    //   orig.name = nullptr;
    //   orig.data = nullptr;      orig._dataLen = 0;
    //   orig.coldata = nullptr;   orig._coldataLen = 0;

    // }
    // Segment(Segment&& orig) noexcept
    // {
    //     #ifdef ENABLE_DEBUGFEATURE_LIGHT__SEGMENTS
    //     Serial.printf(
    //         "-- Move segment constructor: %p -> %p\n",
    //         (void*)&orig,
    //         (void*)this
    //     );
    //     #endif

    //     /*
    //     * Transfer complete object state, including owned pointers.
    //     */
    //     memcpy(this, &orig, sizeof(Segment));

    //     /*
    //     * Remove ownership from the moved-from object.
    //     */
    //     orig._t             = nullptr;
    //     orig.name           = nullptr;
    //     orig.data           = nullptr;
    //     orig.coldata        = nullptr;
    //     orig.pixels         = nullptr;
    //     orig.palette_loaded = nullptr;

    //     orig._dataLen    = 0;
    //     orig._coldataLen = 0;

    //     /*
    //     * Leave moved-from geometry inactive so it cannot be rendered before
    //     * destruction.
    //     */
    //     orig.start = 0;
    //     orig.stop  = 0;
    // }

    Segment(Segment&& orig) noexcept
{
  #ifdef ENABLE_DEBUGFEATURE_LIGHT__SEGMENTS
  Serial.println(F("-- Move segment constructor --"));
  #endif

  /*
   * WLED pattern:
   * transfer complete object state, including owned pointers.
   */
  memcpy(this, &orig, sizeof(Segment));

  /*
   * Remove ownership from the moved-from object.
   */
  orig._t             = nullptr;
  orig.name           = nullptr;
  orig.data           = nullptr;
  orig.pixels         = nullptr;
  orig.palette_loaded = nullptr;
  orig._dataLen    = 0;

  /*
   * Leave moved-from segment inactive.
   */
  orig.start = 0;
  orig.stop  = 0;

}

    // ~Segment() // deconstructor
    // {
    //   #ifdef ENABLE_DEBUGFEATURE_LIGHT__SEGMENTS
    //     Serial.print(F("Destroying segment:"));
    //     if (name) Serial.printf(" %s (%p)", name, name);
    //     if (data) Serial.printf(" %d (%p)", (int)_dataLen, data);
    //     Serial.println();
    //   #endif
    //   stopTransition();   // deallocate "_t" (transition) and with it "_segOld" note: _segOld has _t=null, see copy constructor
      
    //   if (name) delete[] name;
    //   deallocateData();
    //   deallocateColourData();
    //   p_free(pixels);
    // }
   ~Segment()
{
  #ifdef ENABLE_DEBUGFEATURE_LIGHT__SEGMENTS
  Serial.print(F("Destroying segment:"));

  if (name)
  {
    Serial.printf(" %s (%p)", name, name);
  }

  if (data)
  {
    Serial.printf(" data=%u (%p)", (unsigned)_dataLen, data);
  }

  if (pixels)
  {
    Serial.printf(" pixels=%p", pixels);
  }

  if (palette_loaded)
  {
    Serial.printf(" palette=%p", palette_loaded);
  }

  Serial.println();
  #endif

  /*
   * Deallocate _t and its old Segment.
   * The copied old Segment has _t == nullptr.
   */
  stopTransition();

  if (name)
  {
    delete[] name;
    name = nullptr;
  }

  deallocateData();

  if (pixels)
  {
    p_free(pixels);
    pixels = nullptr;
  }

  /*
   * PulSar addition.
   */
  if (palette_loaded)
  {
    delete palette_loaded;
    palette_loaded = nullptr;
  }

}


    Segment& operator= (const Segment &orig); // copy assignment
    Segment& operator= (Segment &&orig) noexcept; // move assignment

    #ifdef ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES
    size_t getSize() const { return sizeof(Segment) + (data?_dataLen:0) + (name?strlen(name):0) + (_t?sizeof(Transition):0) + (pixels?length()*sizeof(uint32_t):0); }
    #endif

    inline bool     getOption(uint8_t n) const { return ((options >> n) & 0x01); }
    inline bool     isSelected(void)     const { return selected; }
    inline bool     isInTransition()     const { return _t != nullptr; }
    inline bool     isActive()           const { return stop > start && pixels; }
    inline uint16_t width(void)          const { return stop - start; }       // segment width in physical pixels (length if 1D)
    inline uint16_t height(void)         const { return stopY - startY; }     // segment height (if 2D) in physical pixels
    inline uint16_t length(void)         const { return width() * height(); } // segment length (count) in physical pixels
    inline uint16_t groupLength(void)    const { return grouping + spacing; }
    inline uint8_t  getLightCapabilities(void) const { return 0xFF; }// force all default on _capabilities; }
    inline void     deactivate()                 { setGeometry(0,0); }
    inline Segment &clearName()                  { p_free(name); name = nullptr; return *this; }
    inline Segment &setName(const String &name)  { return setName(name.c_str()); }

    inline static unsigned vLength()                       { return Segment::_vLength; }
    inline static unsigned vWidth()                        { return Segment::_vWidth; }
    inline static unsigned vHeight()                       { return Segment::_vHeight; }
    inline static uint32_t getCurrentColor(unsigned i)     { return Segment::_currentColors[i<NUM_COLORS?i:0]; }
    inline static const CRGBPalette16 &getCurrentPalette() { return Segment::_currentPalette; }

    inline void setDrawDimensions() const { Segment::_vWidth = virtualWidth(); Segment::_vHeight = virtualHeight(); Segment::_vLength = virtualLength(); }

    void    beginDraw(uint16_t prog = 0xFFFFU);         // set up parameters for current effect
    void    setGeometry(uint16_t i1, uint16_t i2, uint8_t grp=1, uint8_t spc=0, uint16_t ofs=UINT16_MAX, uint16_t i1Y=0, uint16_t i2Y=1, uint8_t m12=0);
    Segment &setColor(uint8_t slot, uint32_t c);
    Segment &setCCT(uint16_t k);
    Segment &setOpacity(uint8_t o);
    Segment &setOption(uint8_t n, bool val);
    Segment &setEffect(uint8_t fx, bool loadDefaults = false);
    Segment &setPalette(uint8_t pal);
    Segment &setName(const char* name);
    void    refreshLightCapabilities() const;

    inline uint32_t *getPixels() const                              { return pixels; }
    inline void     setPixelColorRaw(unsigned i, uint32_t c) const  { pixels[i] = c; }
    inline uint32_t getPixelColorRaw(unsigned i) const              { return pixels[i]; };
  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
    inline void     setPixelColorXYRaw(unsigned x, unsigned y, uint32_t c) const  { auto XY = [](unsigned X, unsigned Y){ return X + Y*Segment::vWidth(); }; pixels[XY(x,y)] = c; }
    inline uint32_t getPixelColorXYRaw(unsigned x, unsigned y) const              { auto XY = [](unsigned X, unsigned Y){ return X + Y*Segment::vWidth(); }; return pixels[XY(x,y)]; };
  #endif

  void    refreshGeometry();

  
  // transition functions
    void stopTransition();                  // ends transition mode by destroying transition structure (does nothing if not in transition)
    void updateTransitionProgress() const;  // sets transition progress (0-65535) based on time passed since transition start
    inline void handleTransition() {
      updateTransitionProgress();
      if (isInTransition() && progress() == 0xFFFFU) stopTransition();
    }
    inline uint16_t progress() const          { return isInTransition() ? _t->_progress : 0xFFFFU; } // relies on handleTransition()/updateTransitionProgress() to update progression variable
    inline Segment *getOldSegment() const     { return isInTransition() ? _t->_oldSegment : nullptr; }

    inline static void modeBlend(bool blend)  { Segment::_modeBlend = blend; }  // for isPreviousMode()
    inline static void setClippingRect(int startX, int stopX, int startY = 0, int stopY = 1) { _clipStart = startX; _clipStop = stopX; _clipStartY = startY; _clipStopY = stopY; };
    inline static bool isPreviousMode()       { return Segment::_modeBlend; }    // needed for determining CCT/opacity during non-TRANSITION_FADE transition


    static uint16_t getUsedSegmentData(void)    { return _usedSegmentData; }
    static void     addUsedSegmentData(int len) { _usedSegmentData += len; }


    bool    parseSegColorHex(const char* in, uint8_t& R, uint8_t& G, uint8_t& B, uint8_t& WW, uint8_t& CW);
    uint8_t differs(const Segment& b) const;
    

    static uint32_t   color_blend(uint32_t,uint32_t,uint16_t,bool b16=false);
    static RgbwwColor color_blend(RgbwwColor,RgbwwColor,uint16_t,bool b16=false);
    static uint32_t   color_add(uint32_t,uint32_t, bool fast=false);
    static uint32_t   color_add(RgbwwColor,RgbwwColor, bool fast=false);
    static RgbwwColor color_fade(RgbwwColor c1, uint8_t amount, bool video=false);
    static uint32_t   color_fade(uint32_t c1, uint8_t amount, bool video=false);

    void setRandomColor(byte* rgb);
    void colorHStoRGB(uint16_t hue, byte sat, byte* rgb);
    void colorKtoRGB(uint16_t kelvin, byte* rgb);    
    void colorCTtoRGB(uint16_t mired, byte* rgb);
    void colorXYtoRGB(float x, float y, byte* rgb);
    void colorRGBtoXY(byte* rgb, float* xy);
    void colorFromDecOrHexString(byte* rgb, char* in);
    bool colorFromHexString(byte* rgb, const char* in);

    // fast scaling function for colors, performs color*scale/256 for all four channels, speed over accuracy
    // note: inlining uses less code than actual function calls
    static inline uint32_t fast_color_scale(const uint32_t c, const uint8_t scale) {
      uint32_t rb = (((c     & 0x00FF00FF) * scale) >> 8) &  0x00FF00FF;
      uint32_t wg = (((c>>8) & 0x00FF00FF) * scale)       & ~0x00FF00FF;
      return rb | wg;
    }



    // runtime data functions
    inline uint16_t dataSize(void) const { return _dataLen; }
    bool allocateData(size_t len);
    void deallocateData(void);

    
    #ifdef ENABLE_DEVFEATURE_LIGHT__PIXELS_BUFFER_RAW    
    inline uint32_t *getPixels() const                              { return pixels; }
    inline void     setPixelColorRaw(unsigned i, uint32_t c) const  { pixels[i] = c; }
    inline uint32_t getPixelColorRaw(unsigned i) const              { return pixels[i]; };
    #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
    inline void     setPixelColorXYRaw(unsigned x, unsigned y, uint32_t c) const  { auto XY = [](unsigned X, unsigned Y){ return X + Y*Segment::vWidth(); }; pixels[XY(x,y)] = c; }
    inline uint32_t getPixelColorXYRaw(unsigned x, unsigned y) const              { auto XY = [](unsigned X, unsigned Y){ return X + Y*Segment::vWidth(); }; return pixels[XY(x,y)]; };
    #endif
    #endif


    void resetIfRequired(void);

    void loadPalette(CRGBPalette16 &tgt, uint8_t pal);

    /** 
      * Flags that before the next effect is calculated,
      * the internal segment state should be reset. 
      * Call resetIfRequired before calling the next effect function.
      * Safe to call from interrupts and network requests.
      */
    inline void markForReset(void) 
    {
      reset = true; 
    }

    
    void startTransition(uint16_t dur, bool segmentCopy = true);    // transition has to start before actual segment values change
    
    // 1D strip
    uint16_t virtualLength() const;
    uint16_t maxMappingLength() const;

    [[gnu::hot]] void setPixelColor(int n, uint32_t c,bool brightness_already_set = false);
    void setPixelColor(int n, RgbwwColor c){ setPixelColor(n, RGBW32(c.R, c.G, c.B, c.WW)); } 
    void setPixelColor(unsigned n, uint32_t c){ setPixelColor((int)n, c); } // to keep compatibility with RGBWW
    void setPixelColor(uint16_t n, uint32_t c){ setPixelColor((int)n, c); } // to keep compatibility with RGBWW
    void setPixelColor(int n, byte r, byte g, byte b, byte w = 0) {      setPixelColor(n, RGBW32(r,g,b,w));    }
    inline void setPixelColor(int n, CRGB c) const                             { setPixelColor(n, RGBW32(c.r,c.g,c.b,0)); }
    // Anti-aliasing functions
    void setPixelColor(float i, uint32_t c, bool aa = true);
    void setPixelColor(float i, uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0, bool aa = true) { setPixelColor(i, RGBW32(r,g,b,w), aa); }
    void setPixelColor(float i, CRGB c, bool aa = true)                                         { setPixelColor(i, RGBW32(c.r,c.g,c.b,0), aa); }

    [[gnu::hot]] bool isPixelClipped(int i) const;  
    [[gnu::hot]] uint32_t getPixelColor(int i) const;      
    // 1D support functions (some implement 2D as well)
    void blur(uint8_t blur_amount, bool smear = false);
    void fill_ranged(uint32_t c);
    void clear() const { fill(BLACK); } // clear segment
    void fill(uint32_t c) const;


    void fade_out(uint8_t r);
    void fadeToBlackBy(uint8_t fadeBy);
    void blendPixelColor(int n, uint32_t color, uint8_t blend);
    void blendPixelColor(int n, CRGB c, uint8_t blend)            { blendPixelColor(n, RGBW32(c.r,c.g,c.b,0), blend); }
    void addPixelColor(int n, uint32_t color);
    void addPixelColor(int n, byte r, byte g, byte b, byte w = 0) { addPixelColor(n, RGBW32(r,g,b,w)); } // automatically inline
    void addPixelColor(int n, CRGB c)                             { addPixelColor(n, RGBW32(c.r,c.g,c.b,0)); } // automatically inline
    inline void fadePixelColor(uint16_t n, uint8_t fade) const                     { setPixelColor(n, color_fade(getPixelColor(n), fade, true)); }
    

    uint8_t get_random_wheel_index(uint8_t pos);
    uint32_t color_wheel(uint8_t pos);




    /** SECTION start ****************************************************************************************************************
    * * Matrix : Inside each segment  *********************************************************************************
    * *****************************************************************************************************************
    * *****************************************************************************************************************
    * *****************************************************************************************************************
    **/

    uint16_t virtualWidth(void)  const;
    uint16_t virtualHeight(void) const;


  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS

    inline bool is2D() const                                                            { return (width()>1 && height()>1); }
    [[gnu::hot]] void setPixelColorXY(int x, int y, uint32_t c) const; // set relative pixel within segment with color
    inline void setPixelColorXY(unsigned x, unsigned y, uint32_t c) const               { setPixelColorXY(int(x), int(y), c); }
    inline void setPixelColorXY(int x, int y, byte r, byte g, byte b, byte w = 0) const { setPixelColorXY(x, y, RGBW32(r,g,b,w)); }
    inline void setPixelColorXY(int x, int y, CRGB c) const                             { setPixelColorXY(x, y, RGBW32(c.r,c.g,c.b,0)); }
    inline void setPixelColorXY(unsigned x, unsigned y, CRGB c) const                   { setPixelColorXY(int(x), int(y), RGBW32(c.r,c.g,c.b,0)); }
    #ifdef WLED_USE_AA_PIXELS
    void setPixelColorXY(float x, float y, uint32_t c, bool aa = true) const;
    inline void setPixelColorXY(float x, float y, byte r, byte g, byte b, byte w = 0, bool aa = true) const { setPixelColorXY(x, y, RGBW32(r,g,b,w), aa); }
    inline void setPixelColorXY(float x, float y, CRGB c, bool aa = true) const                             { setPixelColorXY(x, y, RGBW32(c.r,c.g,c.b,0), aa); }
    #endif
    [[gnu::hot]] bool isPixelXYClipped(int x, int y) const;
    [[gnu::hot]] uint32_t getPixelColorXY(int x, int y) const;
    // 2D support functions
    inline void blendPixelColorXY(uint16_t x, uint16_t y, uint32_t color, uint8_t blend) const { setPixelColorXY(x, y, color_blend(getPixelColorXY(x,y), color, blend)); }
    inline void blendPixelColorXY(uint16_t x, uint16_t y, CRGB c, uint8_t blend) const         { blendPixelColorXY(x, y, RGBW32(c.r,c.g,c.b,0), blend); }
    inline void addPixelColorXY(int x, int y, uint32_t color, bool preserveCR = true) const    { setPixelColorXY(x, y, color_add(getPixelColorXY(x,y), color, preserveCR)); }
    inline void addPixelColorXY(int x, int y, byte r, byte g, byte b, byte w = 0, bool preserveCR = true)
                                                                                               { addPixelColorXY(x, y, RGBW32(r,g,b,w), preserveCR); }
    inline void addPixelColorXY(int x, int y, CRGB c, bool preserveCR = true) const            { addPixelColorXY(x, y, RGBW32(c.r,c.g,c.b,0), preserveCR); }
    inline void fadePixelColorXY(uint16_t x, uint16_t y, uint8_t fade) const                   { setPixelColorXY(x, y, color_fade(getPixelColorXY(x,y), fade, true)); }
    inline void blurCols(uint8_t blur_amount, bool smear = false) const                         { blur2D(0, blur_amount, smear); } // blur all columns (50% faster than full 2D blur)
    inline void blurRows(uint8_t blur_amount, bool smear = false) const                         { blur2D(blur_amount, 0, smear); } // blur all rows (50% faster than full 2D blur)
    //void box_blur(unsigned r = 1U, bool smear = false); // 2D box blur
    void blur2D(uint8_t blur_x, uint8_t blur_y, bool smear = false) const;
    void moveX(int delta, bool wrap = false) const;
    void moveY(int delta, bool wrap = false) const;
    void move(unsigned dir, unsigned delta, bool wrap = false) const;
    void drawCircle(uint16_t cx, uint16_t cy, uint8_t radius, uint32_t c, bool soft = false) const;
    void fillCircle(uint16_t cx, uint16_t cy, uint8_t radius, uint32_t c, bool soft = false) const;
    void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t c, bool soft = false) const;
    void wu_pixel(uint32_t x, uint32_t y, CRGB c) const;
    inline void drawCircle(uint16_t cx, uint16_t cy, uint8_t radius, CRGB c, bool soft = false) const { drawCircle(cx, cy, radius, RGBW32(c.r,c.g,c.b,0), soft); }
    inline void fillCircle(uint16_t cx, uint16_t cy, uint8_t radius, CRGB c, bool soft = false) const { fillCircle(cx, cy, radius, RGBW32(c.r,c.g,c.b,0), soft); }
    inline void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, CRGB c, bool soft = false) const { drawLine(x0, y0, x1, y1, RGBW32(c.r,c.g,c.b,0), soft); } // automatic inline
    inline void fill_solid(CRGB c) const { fill(RGBW32(c.r,c.g,c.b,0)); }

    void drawCharacter(unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, uint32_t color, uint32_t col2 = 0, int8_t rotate = 0, bool usePalGrad = false);
    inline void drawCharacter(unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, CRGB c) { drawCharacter(chr, x, y, w, h, RGBW32(c.r,c.g,c.b,0)); } // automatic inline
    inline void drawCharacter(unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, CRGB c, CRGB c2, int8_t rotate = 0, bool usePalGrad = false) { drawCharacter(chr, x, y, w, h, RGBW32(c.r,c.g,c.b,0), RGBW32(c2.r,c2.g,c2.b,0), rotate, usePalGrad); } // automatic inline
    void drawCharacter_UsingGradientPalletes(
      unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, 
      int8_t rotate, bool solidPerChar, bool horizontalGradient, bool backgroundGradientHorizontal
    );


    // // [[gnu::hot]] uint16_t XY(int x, int y);      // support function to get relative index within segment

    // // XY(x,y) - gets pixel index within current segment (often used to reference leds[] array element)
    // uint16_t [[gnu::hot]] XY(int x, int y) const 
    // {
    //   uint16_t width  = virtualWidth();   // segment width in logical pixels (can be 0 if segment is inactive)
    //   uint16_t height = virtualHeight();  // segment height in logical pixels (is always >= 1)
    //   return isActive() ? (x%width) + (y%height) * width : 0;
    // }

    // // void     Segment::setPixelColorXY(int x, int y, uint32_t col) const;
    // // uint32_t Segment::getPixelColorXY(int x, int y) const;
    // // inline void     setPixelColorXYRaw(unsigned x, unsigned y, uint32_t c) const  { auto XY = [](unsigned X, unsigned Y){ return X + Y*vWidth(); }; pixels[XY(x,y)] = c; }
    // // inline uint32_t getPixelColorXYRaw(unsigned x, unsigned y) const              { auto XY = [](unsigned X, unsigned Y){ return X + Y*Segment::vWidth(); }; return pixels[XY(x,y)]; };


    // void setPixelColorXY(float x, float y, uint32_t c, bool aa = true);
    // void setPixelColorXY(int x, int y, uint32_t c); // set relative pixel within segment with color


    // void setPixelColorXY(unsigned x, unsigned y, uint32_t c)
    // { 
    //   // Serial.println(__LINE__);
    //   setPixelColorXY(int(x), int(y), c); 
    // }
    // void setPixelColorXY(int x, int y, byte r, byte g, byte b, byte w = 0)
    // { 
    //   // Serial.println(__LINE__);
    //   setPixelColorXY(x, y, RGBW32(r,g,b,w)); 
    // } // automatically inline
    // // void setPixelColorXY_CRGB(int x, int y, CRGB c)
    // // { 
    // //   Serial.println(__LINE__);
    // //   setPixelColorXY(x, y, RGBW32(c.r,c.g,c.b,0)); 
    // // } // automatically inline  
    // void setPixelColorXY(int x, int y, CRGB c)
    // { 
    //   // Serial.println(__LINE__);
    //   setPixelColorXY(x, y, RGBW32(c.r,c.g,c.b,0)); 
    // } // automatically inline  
    // void setPixelColorXY(float x, float y, byte r, byte g, byte b, byte w = 0, bool aa = true)
    // { 
    //   // Serial.println(__LINE__);
    //   setPixelColorXY(x, y, RGBW32(r,g,b,w), aa); 
    // }
    // void setPixelColorXY(float x, float y, CRGB c, bool aa = true)
    // { 
    //   // Serial.println(__LINE__);
    //   setPixelColorXY(x, y, RGBW32(c.r,c.g,c.b,0), aa); 
    // }

    // [[gnu::hot]] bool isPixelXYClipped(int x, int y) const;
    // uint32_t getPixelColorXY(uint16_t x, uint16_t y) const;
    
    // void blur2D(uint8_t blur_x, uint8_t blur_y, bool smear);

    // // 2D support functions
    // void blendPixelColorXY(uint16_t x, uint16_t y, uint32_t color, uint8_t blend);
    // void blendPixelColorXY(uint16_t x, uint16_t y, CRGB c, uint8_t blend)  { blendPixelColorXY(x, y, RGBW32(c.r,c.g,c.b,0), blend); }
    // void addPixelColorXY(int x, int y, uint32_t color, bool fast = false);
    // void addPixelColorXY(int x, int y, byte r, byte g, byte b, byte w = 0, bool fast = false) { addPixelColorXY(x, y, RGBW32(r,g,b,w), fast); } // automatically inline
    // void addPixelColorXY(int x, int y, CRGB c, bool fast = false)                             { addPixelColorXY(x, y, RGBW32(c.r,c.g,c.b,0), fast); }
    // void fadePixelColorXY(uint16_t x, uint16_t y, uint8_t fade);
    // void box_blur(uint16_t i, bool vertical, fract8 blur_amount); // 1D box blur (with weight)
    // void blurRow(uint16_t row, fract8 blur_amount);
    // void blurCol(uint16_t col, fract8 blur_amount);
    // void moveX(int8_t delta, bool wrap = false);
    // void moveY(int8_t delta, bool wrap = false);
    // void move(uint8_t dir, uint8_t delta, bool wrap = false);
    
    // void drawCircle(uint16_t cx, uint16_t cy, uint8_t radius, uint32_t c, bool soft = false);
    // inline void drawCircle(uint16_t cx, uint16_t cy, uint8_t radius, CRGB c, bool soft = false) { drawCircle(cx, cy, radius, RGBW32(c.r,c.g,c.b,0), soft); }
    
    // void fillCircle(uint16_t cx, uint16_t cy, uint8_t radius, uint32_t c, bool soft = false);
    // inline void fillCircle(uint16_t cx, uint16_t cy, uint8_t radius, CRGB c, bool soft = false) { fillCircle(cx, cy, radius, RGBW32(c.r,c.g,c.b,0), soft); }
    
    // void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t c);
    // void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, CRGB c) { drawLine(x0, y0, x1, y1, RGBW32(c.r,c.g,c.b,0)); } // automatic inline
    
    // void drawCharacter(unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, uint32_t color, uint32_t col2 = 0, int8_t rotate = 0, bool usePalGrad = false);
    // inline void drawCharacter(unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, CRGB c) { drawCharacter(chr, x, y, w, h, RGBW32(c.r,c.g,c.b,0)); } // automatic inline
    // inline void drawCharacter(unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, CRGB c, CRGB c2, int8_t rotate = 0, bool usePalGrad = false) { drawCharacter(chr, x, y, w, h, RGBW32(c.r,c.g,c.b,0), RGBW32(c2.r,c2.g,c2.b,0), rotate, usePalGrad); } // automatic inline
    
    
    // inline void blurCols(uint8_t blur_amount, bool smear = false) const                         { blur2D(0, blur_amount, smear); } // blur all columns (50% faster than full 2D blur)
    // inline void blurRows(uint8_t blur_amount, bool smear = false) const                         { blur2D(blur_amount, 0, smear); } // blur all rows (50% faster than full 2D blur)
    
    // void drawCharacter_UsingGradientPalletes(
    //   unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, 
    //   int8_t rotate, bool solidPerChar, bool horizontalGradient, bool backgroundGradientHorizontal
    // );//unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, int8_t rotate, bool solidPerChar, bool horizontalGradient, bool backgroundHorizontalGradient);
  
  
    // void wu_pixel(uint32_t x, uint32_t y, CRGB c);
    // void blur1d(fract8 blur_amount); // blur all rows in 1 dimension
    // void blur2d(fract8 blur_amount) { blur((uint8_t)blur_amount); }
    // void fill_solid(CRGB c) { fill(RGBW32(c.r,c.g,c.b,0)); }
    // void nscale8(uint8_t scale);
  #else
  
  
    inline bool is2D() const                                               { return false; }
    uint16_t XY(uint16_t x, uint16_t y)                                    { return x; }
    void setPixelColorXY(int x, int y, uint32_t c)                         { setPixelColor(x, c); }
    void setPixelColorXY(int x, int y, byte r, byte g, byte b, byte w = 0) { setPixelColor(x, RGBW32(r,g,b,w)); }
    void setPixelColorXY(int x, int y, CRGB c)                             { setPixelColor(x, RGBW32(c.r,c.g,c.b,0)); }
    #ifdef USE_AA_PIXELS
    void setPixelColorXY(float x, float y, uint32_t c, bool aa = true)     { setPixelColor(x, c, aa); }
    void setPixelColorXY(float x, float y, byte r, byte g, byte b, byte w = 0, bool aa = true) { setPixelColor(x, RGBW32(r,g,b,w), aa); }
    void setPixelColorXY(float x, float y, CRGB c, bool aa = true)         { setPixelColor(x, RGBW32(c.r,c.g,c.b,0), aa); }
    #endif
    inline bool isPixelXYClipped(int x, int y)     const                               { return isPixelClipped(x); }
    uint32_t getPixelColorXY(uint16_t x, uint16_t y)                       { return getPixelColor(x); }
    void blendPixelColorXY(uint16_t x, uint16_t y, uint32_t c, uint8_t blend) { blendPixelColor(x, c, blend); }
    void blendPixelColorXY(uint16_t x, uint16_t y, CRGB c, uint8_t blend)  { blendPixelColor(x, RGBW32(c.r,c.g,c.b,0), blend); }
    void addPixelColorXY(int x, int y, uint32_t color)                     { addPixelColor(x, color); }
    void addPixelColorXY(int x, int y, byte r, byte g, byte b, byte w = 0) { addPixelColor(x, RGBW32(r,g,b,w)); }
    void addPixelColorXY(int x, int y, CRGB c)                             { addPixelColor(x, RGBW32(c.r,c.g,c.b,0)); }
    void fadePixelColorXY(uint16_t x, uint16_t y, uint8_t fade)            { fadePixelColor(x, fade); }
    inline void blur2D(uint8_t blur_x, uint8_t blur_y, bool smear = false) {}
    inline void blurRows(uint8_t blur_amount, bool smear = false) {}
    inline void blurCols(uint8_t blur_amount, bool smear = false) {}
    inline void moveX(int delta, bool wrap = false) {}
    inline void moveY(int delta, bool wrap = false) {}
    inline void move(uint8_t dir, uint8_t delta, bool wrap = false) {}
    inline void drawCircle(uint16_t cx, uint16_t cy, uint8_t radius, uint32_t c, bool soft = false) {}
    inline void drawCircle(uint16_t cx, uint16_t cy, uint8_t radius, CRGB c, bool soft = false) {}
    inline void fillCircle(uint16_t cx, uint16_t cy, uint8_t radius, uint32_t c, bool soft = false) {}
    inline void fillCircle(uint16_t cx, uint16_t cy, uint8_t radius, CRGB c, bool soft = false) {}
    inline void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t c, bool soft = false) {}
    inline void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, CRGB c, bool soft = false) {}
    inline void wu_pixel(uint32_t x, uint32_t y, CRGB c) {}
    void drawCharacter(unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, uint32_t color) {}
    void drawCharacter(unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, CRGB color) {}
  #endif

  /** SECTION end ****************************************************************************************************************
  * * Matrix : Inside each segment  *********************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  **/


  
    /******************************************************************************************************************************************************
     * U32 vs RGBWW
     * In normal mode (<300 pixels RGBWW performance is good)
     *  * RGBWW can be used directly (though for most effects, a define will convert it to U32 anyway)
     * In performance mode (>300 pixels RGBWW performance is bad)
     *  * U32 is used directly, RGBWW methods are not used
     * In Hybrid mode 
     *  * RGBWW can be used directly, but U32 methods are used for performance
     * 
     ******************************************************************************************************************************************************/


    /**
     * @brief Depending on the build settings later, I will want to keep a Rgbcct and U32 palette method
     * Hence a new U32 palette structure will exist that always foregoes the Rgbcct and handles in U32 format
     * This may be hardcorded with a define, or use if to switch
     * #ifdef XX
     * #define GetPaletteColour_Legacy GetPaletteColour_U32
     * #else
     * #define GetPaletteColour_Legacy GetPaletteColourRGBCCT
     * #endif
     **/
    uint8_t white_warm_GetPaletteColour = 0;
    [[gnu::hot]] uint32_t GetPaletteColour_Legacy(
      /**
       * @brief _pixel_position
       * ** [0-SEGLEN]
       * ** [0-255]   
       */
      uint16_t pixel_position = 0,
      /**
       * @brief flag_position_scaled255
       * ** [true] : pixel_position should be between 0-255
       * ** [false]: pixel is exact, and will automatically wrap around (ie 5 pixels inside palette will be 0,1,2,3,4,0,1,2,3,4)
       */
      uint8_t     flag_position_scaled255 = false,
      /**
       * @brief flag_wrap_hard_edge
       * ** [true] : 16 palette gradients will not blend from 15 back to 0. ie 0-255 does not become 0-240 (where 0,15,31,47,63,79,95,111,127,143,159,175,191,207,223,239)
       * ** [false]: Palette16 with 16 elements, as 0-255 pixel_position, will blend around smoothly using built-in CRGBPalette16
       */
      uint8_t     flag_wrap_hard_edge = false,
      /**
       * @brief flag_crgb_exact_colour
       * ** [true] : 16 palette gradients will not blend from 15 back to 0. ie 0-255 does not become 0-240 (where 0,15,31,47,63,79,95,111,127,143,159,175,191,207,223,239)
       * ** [false]: Palette16 with 16 elements, as 0-255 pixel_position, will blend around smoothly using built-in CRGBPalette16
       */
      uint8_t     flag_crgb_exact_colour = false,
      /**
       * @brief encoded_value
       * ** [uint32_t*] : encoded value from palette
       */
      uint8_t* encoded_value = nullptr, // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
      /**
       * @brief apply_brightness
       * ** [false] : Apply brightness to the colour
       * ** [true]  : Get the "full" 255 range colour object
       */
      bool apply_brightness = false,

      uint8_t pbri = 255,

      uint8_t mcol = 0
    );


    // /*****
    //  * Some effects allow for RGBWW to be generated, but this has performance implications
    //  *****/
    // [[gnu::hot]] RgbwwColor GetPaletteColour_RGBWW(
    //   /**
    //    * @brief _pixel_position
    //    * ** [0-SEGLEN]
    //    * ** [0-255]   
    //    */
    //   uint16_t pixel_position = 0,
    //   /**
    //    * @brief flag_position_scaled255
    //    * ** [true] : pixel_position should be between 0-255
    //    * ** [false]: pixel is exact, and will automatically wrap around (ie 5 pixels inside palette will be 0,1,2,3,4,0,1,2,3,4)
    //    */
    //   uint8_t     flag_position_scaled255 = false,
    //   /**
    //    * @brief flag_wrap_hard_edge
    //    * ** [true] : 16 palette gradients will not blend from 15 back to 0. ie 0-255 does not become 0-240 (where 0,15,31,47,63,79,95,111,127,143,159,175,191,207,223,239)
    //    * ** [false]: Palette16 with 16 elements, as 0-255 pixel_position, will blend around smoothly using built-in CRGBPalette16
    //    */
    //   uint8_t     flag_wrap_hard_edge = false,
    //   /**
    //    * @brief flag_crgb_exact_colour
    //    * ** [true] : 16 palette gradients will not blend from 15 back to 0. ie 0-255 does not become 0-240 (where 0,15,31,47,63,79,95,111,127,143,159,175,191,207,223,239)
    //    * ** [false]: Palette16 with 16 elements, as 0-255 pixel_position, will blend around smoothly using built-in CRGBPalette16
    //    */
    //   uint8_t     flag_crgb_exact_colour = false,
    //   /**
    //    * @brief encoded_value
    //    * ** [uint32_t*] : encoded value from palette
    //    */
    //   uint8_t* encoded_value = nullptr, // Must be passed in as something other than 0, or else nullptr will not be checked inside properly

      

    //   bool apply_brightness = false
    // );

    
    /**
     * @brief Depending on the build settings later, I will want to keep a Rgbcct and U32 palette method
     * Hence a new U32 palette structure will exist that always foregoes the Rgbcct and handles in U32 format
     * This may be hardcorded with a define, or use if to switch
     * #ifdef XX
     * #define GetPaletteColour_Legacy GetPaletteColour_U32
     * #else
     * #define GetPaletteColour_Legacy GetPaletteColourRGBCCT
     * #endif
     **/
    // uint8_t white_warm_GetPaletteColour = 0;
    [[gnu::hot]] uint32_t GetPaletteColour( // GetPaletteColour_ModeWrap was temporary as this, should be converted.
      /**
       * @brief _pixel_position
       * ** [0-SEGLEN]
       * ** [0-255]   
       */
      uint16_t pixel_position = 0,
      /**
       * @brief flag_position_scaled255
       * ** [true] : pixel_position should be between 0-255
       * ** [false]: pixel is exact, and will automatically wrap around (ie 5 pixels inside palette will be 0,1,2,3,4,0,1,2,3,4)
       */
      uint8_t     flag_position_scaled255 = false,
      /**
       * @brief force_palette_mode flag_crgb_exact_colour
       * ** [true] : 16 palette gradients will not blend from 15 back to 0. ie 0-255 does not become 0-240 (where 0,15,31,47,63,79,95,111,127,143,159,175,191,207,223,239)
       * ** [false]: Palette16 with 16 elements, as 0-255 pixel_position, will blend around smoothly using built-in CRGBPalette16
       */
      uint8_t     force_palette_mode = false,
      /**
       * @brief flag_wrap_hard_edge
       * ** [true] : 16 palette gradients will not blend from 15 back to 0. ie 0-255 does not become 0-240 (where 0,15,31,47,63,79,95,111,127,143,159,175,191,207,223,239)
       * ** [false]: Palette16 with 16 elements, as 0-255 pixel_position, will blend around smoothly using built-in CRGBPalette16
       */
      uint8_t     flag_wrap_hard_edge = false,
      /**
       * @brief encoded_value
       * ** [uint32_t*] : encoded value from palette
       */
      uint8_t* encoded_value = nullptr, // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
      /**
       * @brief apply_brightness
       * ** [false] : Apply brightness to the colour
       * ** [true]  : Get the "full" 255 range colour object
       */
      bool apply_brightness = false,

      uint8_t pbri = 255,

      uint8_t mcol = 0
    );

    
    /**
     * WLED Palette Conversion
     * 
     * Gets a single color from the currently selected palette.
     * @param i Palette Index (if mapping is true, the full palette will be _virtualSegmentLength long, if false, 255). Will wrap around automatically.
     * @param mapping if true, LED position in segment is considered for color
     * @param wrap FastLED palettes will usually wrap back to the start smoothly. Set false to get a hard edge
     * @param mcol If the default palette 0 is selected, return the standard color 0, 1 or 2 instead. If >2, Party palette is used instead
     * @param pbri Value to scale the brightness of the returned color by. Default is 255. (no scaling)
     * @returns Single color from palette
     * Since inline functions are expanded at compile time and do not incur runtime overhead, you can use an inline function in a header file
     * alternatively could seak DEFINE remaps
     * return GetPaletteColour_Legacy(i, mapping, wrap, /*crgb exact skip arg* false, /*encoded value skip arg* nullptr, /*apply brightness skip arg* true, pbri, mcol); August2025, pbri not applied correctly this way, needs fixed later
     * uint32_t c = GetPaletteColour_Legacy(i, mapping, wrap, /*crgb exact skip arg* false, /*encoded value skip arg* nullptr, /*apply brightness skip arg: fix: must apply pix brightness by effect after this function* false, pbri, mcol);
     * color_from_palette_forced_gradient is really teh default, all WLED acts on CRGBPalette16 and assumes never discrete/exact colour sampling so we should default mine to that too.
    */
    inline uint32_t color_from_palette(uint16_t i, bool mapping, bool wrap, uint8_t mcol, uint8_t pbri = 255) {
      

      // Error here, I believe this mode between WLED/CRGBPalette16 and my descrite to be converted is opposing each other 
      const uint8_t idxMode   = mapping ? PALETTE_INDEX__IS_SEGLEN_RANGE : PALETTE_INDEX__IS_EXACT_COLOUR;
      const uint8_t wrapMode  = wrap    ? PALETTE_WRAP_SMOOTH           : PALETTE_WRAP_HARDEDGE;
      const uint8_t discrete  = PALETTE_MODE__FORCE_GRADIENT; // ← force gradient interpolation

      uint32_t c = GetPaletteColour(
          i,
          idxMode,
          discrete,
          wrapMode,
          nullptr,
          /*apply brightness*/ false,
          255, // to be removed, handled below
          mcol
      );


      if(pbri != 255) { // apply brightness if not already done
        byte r = R(c), g = G(c), b = B(c), w = W(c);
        r = (uint16_t(r) * pbri) >> 8;
        g = (uint16_t(g) * pbri) >> 8;
        b = (uint16_t(b) * pbri) >> 8;
        w = (uint16_t(w) * pbri) >> 8;
        c = RGBW32(r, g, b, w);
      }

      return c;
    }

    #ifdef ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL5_PARTICLE_SYSTEM
    class ParticleSystem1D;
    class ParticleSystem2D;
    #endif

} segment;



#include <stdint.h>

inline uint32_t HueSatBrt(uint16_t hue, uint8_t sat, uint8_t brt, bool white_from_sat = false) {
  // Normalize hue to 0-360 range
  hue %= 360;

  // Scale hue to 0-255 for conversion to RGB (divide 360 into 256 steps)
  uint8_t hue_8bit = (hue * 255) / 360;

  // Calculate RGB components from Hue
  uint8_t region = hue_8bit / 43;
  uint8_t remainder = (hue_8bit - (region * 43)) * 6;

  uint8_t p = (brt * (255 - sat)) >> 8;
  uint8_t q = (brt * (255 - ((sat * remainder) >> 8))) >> 8;
  uint8_t t = (brt * (255 - ((sat * (255 - remainder)) >> 8))) >> 8;

  uint8_t r, g, b;
  switch (region) {
    case 0:      r = brt; g = t; b = p;      break;
    case 1:      r = q; g = brt; b = p;      break;
    case 2:      r = p; g = brt; b = t;      break;
    case 3:      r = p; g = q; b = brt;      break;
    case 4:      r = t; g = p; b = brt;      break;
    default:     r = brt; g = p; b = q;      break;
  }

  // Calculate white channel based on brightness and saturation
  uint8_t w = 0;
  if (white_from_sat) {
    w = brt * (255 - sat) / 255;
  }

  // Pack into RGBW32 format (WWWWRRRRGGGGBBBB)
  return (w << 24) | (r << 16) | (g << 8) | b;
}









  RgbwwColor ColourBlend(RgbwwColor color1, RgbwwColor color2, uint8_t blend);

  typedef void (*mode_ptr)(void); // pointer to mode function


  typedef void (*show_callback)(void); // pre show callback
  typedef struct ModeData {
    uint8_t     _id;   // mode (effect) id
    mode_ptr    _fcn;  // mode (effect) function
    const char *_data; // mode (effect) name and its UI control data
    ModeData(uint8_t id, void (*fcn)(void), const char *data) : _id(id), _fcn(fcn), _data(data) {}
  } mode_data_t;

  void finalizeInit();
  void service(void);
  void setEffect(uint8_t segid, uint8_t m);
  void setColor(uint8_t slot, uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0);
  void setColor(uint8_t slot, uint32_t c);
  void setCCT(uint16_t k);
  void setBrightness(uint8_t b, bool direct = false);
  void setRange(uint16_t i, uint16_t i2, uint32_t col);
  
  void purgeSegments(); // removes inactive segments from RAM (may incure penalty and memory fragmentation but reduces vector footprint)
  void setSegment(uint8_t n, uint16_t start, uint16_t stop, uint8_t grouping = 1, uint8_t spacing = 0, uint16_t offset = UINT16_MAX, uint16_t startY=0, uint16_t stopY=1);
  void setMainSegmentId(uint8_t n);
  void restartRuntime();
  void resetSegments();
  void makeAutoSegments(bool forceReset = false);
  void fixInvalidSegments();
  void blendSegment(const Segment &topSegment) const;//,    // blends topSegment into pixels
  void show(void);
  void setTargetFps(uint8_t fps);

  [[gnu::hot]] uint32_t getPixelColor(uint32_t i) const;
  void     setPixelColor(uint32_t n, uint32_t c);
  // using public variables to reduce code size increase due to inline function getSegment() (with bounds checking) and color transitions
  uint32_t _colors_t_PHASE_OUT[3]; // color used for effect (includes transition)
  // uint16_t _virtualSegmentLength; // delete, edit everywhere it is used to use Segment:vLength() then safely remove

  std::vector<Segment> segments;
  friend class Segment;

  uint16_t _length;
  uint8_t  _brightness;

  uint8_t  _targetFps;
  uint16_t _frametime;

    uint32_t *_pixels = nullptr;

    uint8_t  *_pixelCCT = nullptr;

    uint16_t _pixels_length = 0;

    
    Segment *_currentSegment;

    volatile bool _suspend;

    uint16_t _transitionDur = 750;

  // Fixed-point = store a real number as an integer with an implicit scale.
  //   Scale = 1 << FPS_CALC_SHIFT = 128.
  //   _cumulativeFps holds FPS × 128 (no decimals stored, just an int).
  //   This is often written as Q7 (7 fractional bits).
  uint16_t _cumulativeFps; // _cumulativeFps is fixed-point in units of FPS × 2^FPS_CALC_SHIFT (default 128)

  // will require only 1 byte
  struct {
    bool _isServicing          : 1;
    bool _isOffRefreshRequired : 1; //periodic refresh is required for the strip to remain off.
    bool _hasWhiteChannel      : 1;
    bool _triggered : 1;//_triggered : 1; //_triggered            : 1;
  };

  inline void force_update(void) { _triggered = true; } // Forces the next frame to be computed on all active segments.


  void LoadEffects();

  show_callback _callback;

  uint16_t* customMappingTable;
  uint16_t  customMappingSize;
  
  uint32_t _lastShow;
  
  uint8_t segment_index;
  uint8_t _mainSegment;


  void fill2(uint32_t c) { for (int i = 0; i < _length; i++) setPixelColor(i, c); } // fill whole strip with color (inline)


  /**
   * @brief I need to add a way to either only transmit these to the webui based on a level, or add a selector for it. Just to be used for testing
   * 
   */
  enum Effect_DevStage
  {
    Release=0, // Full working for all string types (RGB, WRGB, RGBWW, 1D and 2D), should be used for production
    Beta=1,    // Working in most cases, but not  tested fully, bug fixing only
    Alpha=2,   // Works in some exact cases, still under modifications
    Dev=3,     // Incomplete and not working, for development only
    Unstable=4 // Likely will crash, has some issue that needs to be fixed
  };


    typedef void (mAnimatorLight::*EffectFunction)();    
    
    #ifdef ENABLE_EFFECT_DESCRIPTIONS    
    void addEffect(uint8_t id, EffectFunction function, const char* config = nullptr, const char* effect_description = nullptr, uint8_t development_stage = Effect_DevStage::Dev); // add effect to the list; defined in FX.cpp
    #else
    void addEffect(uint8_t id, EffectFunction function, const char* config = nullptr, uint8_t development_stage=3);// = Effect_DevStage::Dev); // add effect to the list; defined in FX.cpp
    #endif

    struct EFFECTS
    {
      std::vector<EffectFunction>     function;    // SRAM footprint: 4 bytes per element
      std::vector<uint8_t>            development_stage; // 0:stable, 1:beta, 2:alpha, 3:dev
      std::vector<uint8_t>            id;          //
      std::vector<const char*>        config;      //
      #ifdef ENABLE_EFFECT_DESCRIPTIONS
      std::vector<const char*>        description;       //       
      #endif
    }effects;
    
    // Optional compile-time knobs:
    // #define EFFECTS_SORT_PROMOTE_DEV    1   // put Dev effects first
    // #define EFFECTS_SORT_PROMOTE_ALPHA  1   // put Alpha effects before others (Dev may still come first if both are defined)

    #if defined(ENABLE_DEBUG_FEATURE__SORTING_EFFECTS_PROMOTE_DEV) || defined(ENABLE_DEBUG_FEATURE__SORTING_EFFECTS_PROMOTE_ALPHA)
    // Reorder effects by development stage, promoting a chosen stage to the front.
    // - promote_first: the Effect_DevStage to bring to the front (e.g., Effect_DevStage::Dev).
    //   All entries with this stage keep their relative order (stable).
    // - The remaining entries are ordered by their stage value (Release=0 … Unstable=4), stable within ties.
    void sortEffects(Effect_DevStage promote_first);
    #endif

    void waitForIt();                                // wait until frame is over (service() has finished or time for 1 frame has passed)

    inline void setShowCallback(show_callback cb) { _callback = cb; }

    inline void setTransition(uint16_t t)                     { _transitionDur = t; } // sets transition time (in ms)
    
    void setTransitionMode(bool t);
    
    inline void suspend()                                     { _suspend = true; }    // will suspend (and canacel) strip.service() execution
    inline void resume()                                      { _suspend = false; }   // will resume strip.service() execution
    
    inline void trigger()                                     { _triggered = true; }  // Forces the next frame to be computed on all active segments.
    
    
    inline void appendSegment(uint16_t sStart=0, uint16_t sStop=30, uint16_t sStartY = 0, uint16_t sStopY = 1){ if (segments.size() < getMaxSegments()) segments.emplace_back(sStart,sStop,sStartY,sStopY); }


    bool checkSegmentAlignment() const;
    bool hasRGBWBus() const;
    bool hasCCTBus() const;
    bool deserializeMap(unsigned n = 0);

    inline bool isUpdating() const           { return !BusManager::canAllShow(); } // return true if the strip is being sent pixel updates
    inline bool isServicing() const          { return _isServicing; }           // returns true if strip.service() is executing
    inline bool hasWhiteChannel() const      { return _hasWhiteChannel; }       // returns true if strip contains separate white chanel
    inline bool isOffRefreshRequired() const { return _isOffRefreshRequired; }  // returns true if strip requires regular updates (i.e. TM1814 chipset)
    inline bool isSuspended() const          { return _suspend; }               // returns true if strip.service() execution is suspended
    inline bool needsUpdate() const          { return _triggered; }             // returns true if strip received a trigger() request


    bool useLedsArray = false;
      
    

    inline bool isServicing(void) { return _isServicing; }
    inline bool hasWhiteChannel(void) {return _hasWhiteChannel;}
    inline bool isOffRefreshRequired(void) {return _isOffRefreshRequired;}

    uint8_t
      paletteFade,
      paletteBlend,
      milliampsPerLed,
      cctBlending;
      
    uint8_t getActiveSegmentsNum() const;
    uint8_t getFirstSelectedSegId() const;
    uint8_t getLastActiveSegmentId() const;
    uint8_t getActiveSegsLightCapabilities(bool selectedOnly = false) const;


    inline uint8_t getBrightness(void) { return _brightness; }
    inline uint8_t getMaxSegments(void) { return MAX_NUM_SEGMENTS; }  // returns maximum number of supported segments (fixed value)
    inline uint8_t getSegmentsNum(void) { return segments.size(); }  // returns currently present segments
    inline uint8_t getCurrSegmentId(void) { return segment_index; }
    inline uint8_t getMainSegmentId(void) { return _mainSegment; }
    inline uint8_t getPaletteCount() { return 13 + GRADIENT_PALETTE_COUNT; }  // will only return built-in palette count
    inline uint8_t getTargetFps() { return _targetFps; }
    inline uint8_t getEffectCount() const { return static_cast<uint8_t>(effects.id.size()); }

    uint16_t getLengthPhysical() const;
    uint16_t getLengthTotal() const; // will include virtual/nonexistent pixels in matrix

    #define FPS_FIXED_SCALE       (1u << FPS_CALC_SHIFT)
    #define FPS_FIXED_TO_INT(fx)  static_cast<uint16_t>(((fx) + (FPS_FIXED_SCALE/2)) >> FPS_CALC_SHIFT) // rounded
    #define FPS_FIXED_TO_FLOAT(fx) ((fx) * (1.0f / FPS_FIXED_SCALE))
    // If you want to expose the raw value for debugging:
    uint32_t getFpsFixed() const { return _cumulativeFps; }
    // Human-readable (integer, rounded)
    inline uint16_t getFps() {
      if (millis() - _lastShow > 2000) return 0;
      return FPS_FIXED_TO_INT(_cumulativeFps);
    }
    // inline uint16_t getFps() const          { return (millis() - _lastShow > 2000) ? 0 : (FPS_MULTIPLIER * _cumulativeFps) >> FPS_CALC_SHIFT; } // Returns the refresh rate of the LED strip (_cumulativeFps is stored in fixed point)
    inline uint16_t getFrameTime() const    { return _frametime; }        // returns amount of time a frame should take (in ms)
    inline uint16_t getMinShowDelay() const { return MIN_FRAME_DELAY; }   // returns minimum amount of time strip.service() can be delayed (constant)
    // inline uint16_t getLength() const       { return _length; }           // returns actual amount of LEDs on a strip (2D matrix may have less LEDs than W*H)
    inline uint16_t getTransition() const   { return _transitionDur; }    // returns currently set transition time (in ms)
   

    uint16_t ablMilliampsMax;
    uint16_t currentMilliamps;



    // Human-readable (float)
    float getFpsFloat() const {
      if (millis() - _lastShow > 2000) return 0.0f;
      return FPS_FIXED_TO_FLOAT(_cumulativeFps);
    }


    
    inline uint16_t getMappedPixelIndex(uint16_t index) const {           // convert logical address to physical
      if (index < customMappingSize && (realtimeMode == REALTIME_MODE_INACTIVE || realtimeRespectLedMaps)) 
        index = customMappingTable[index];
      return index; // default is to return raw, so not the error
    };

    inline uint32_t getPixelColor(unsigned n) { return (getMappedPixelIndex(n) < getLengthTotal()) ? _pixels[n] : 0; } // returns color of pixel n, black if out of (mapped) bounds
    inline uint32_t getPixelColorNoMap(unsigned n) { return (n < getLengthTotal()) ? _pixels[n] : 0; } // ignores mapping table
    


    uint32_t effect_start_time; // WLED "now", strip.now
    uint32_t timebase;
    uint32_t currentColor(uint32_t colorNew, uint8_t tNr);

    inline uint32_t getLastShow(void) { return _lastShow; }
    inline uint32_t segColor(uint8_t i) { return _colors_t_PHASE_OUT[i]; }

    const char* getModeData(uint8_t id = 0) { return (id && id<getEffectCount()) ? effects.config[id] : PSTR("Solid"); }
    const char* getModeData_Config(uint8_t id = 0) { return (id<getEffectCount()) ? effects.config[id] : PSTR("Unknown"); }

    const char** getModeDataSrc(void) { return &(effects.config[0]); } // vectors use arrays for underlying data

    Segment&        getSegment(uint8_t id);
    inline Segment& getFirstSelectedSeg(void) { return segments[getFirstSelectedSegId()]; }
    inline Segment& getMainSegment(void)      { return segments[getMainSegmentId()]; }
    inline Segment* getSegments(void)         { return &(segments[0]); }

  /******************************************************************************************************************
  * * Matrix : Global 2D settings ****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  **/
 
  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
    bool isMatrix = true;
  #else
    bool isMatrix = false;
  #endif

  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
    #define WLED_MAX_PANELS 64
    uint8_t  panels;

    typedef struct panel_t {
      uint16_t xOffset; // x offset relative to the top left of matrix in LEDs
      uint16_t yOffset; // y offset relative to the top left of matrix in LEDs
      uint8_t  width;   // width of the panel
      uint8_t  height;  // height of the panel
      union {
        uint8_t options;
        struct {
          bool bottomStart : 1; // starts at bottom?
          bool rightStart  : 1; // starts on right?
          bool vertical    : 1; // is vertical?
          bool serpentine  : 1; // is serpentine?
        };
      };
      panel_t()
        : xOffset(0)
        , yOffset(0)
        , width(8)
        , height(8)
        , options(0)
      {}
    } Panel;
    std::vector<Panel> panel;

    void setUpMatrix();

    void subparse_MatrixConfig(JsonParserObject obj);

    // outsmart the compiler :) by correctly overloading
    inline void setPixelColorXY(int x, int y, uint32_t c){    
      // Serial.println(__LINE__);   
      // if(c)
      // Serial.printf("rgb %d,%d,%d\n\r", R(c),G(c),B(c));
      setPixelColor(y * Segment::maxWidth + x, c); 
    }
    inline void setPixelColorXY(int x, int y, byte r, byte g, byte b, byte w = 0) { 
      // Serial.println(__LINE__);
      setPixelColorXY(x, y, RGBW32(r,g,b,w)); }
    inline void setPixelColorXY(int x, int y, CRGB c)       { 
      // Serial.println(__LINE__);
      setPixelColorXY(x, y, RGBW32(c.r,c.g,c.b,0)); }

    inline uint32_t getPixelColorXY(uint16_t x, uint16_t y) { return getPixelColor(isMatrix ? y * Segment::maxWidth + x : x);}

  #endif


  /******************************************************************************************************************
  * * ?? ****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  **/


    void loadCustomPalettes(void);
    std::vector<CRGBPalette16> customPalettes; // TODO: move custom palettes out of WS2812FX class

    void estimateCurrentAndLimitBri(void);

    // network time
    #ifndef WLED_LAT
      #define WLED_LAT 0.0f
    #endif
    #ifndef WLED_LON
      #define WLED_LON 0.0f
    #endif
    float longitude _INIT(WLED_LON);
    float latitude _INIT(WLED_LAT);
    time_t sunrise _INIT(0);
    time_t sunset _INIT(0);
    Toki toki = Toki();


    WiFiUDP notifierUdp, rgbUdp, notifier2Udp;
    bool e131NewData = false;
    byte currentPreset = 0;


    /*
    * color blend function, based on FastLED blend function
    * the calculation for each color is: result = (A*(amountOfA) + A + B*(amountOfB) + B) / 256 with amountOfA = 255 - amountOfB
    
    2025 version
    */
    inline uint32_t color_blend(uint32_t color1, uint32_t color2, uint8_t blend) {
      // min / max blend checking is omitted: calls with 0 or 255 are rare, checking lowers overall performance
      uint32_t rb1 = color1 & 0x00FF00FF;
      uint32_t wg1 = (color1>>8) & 0x00FF00FF;
      uint32_t rb2 = color2 & 0x00FF00FF;
      uint32_t wg2 = (color2>>8) & 0x00FF00FF;
      uint32_t rb3 = ((((rb1 << 8) | rb2) + (rb2 * blend) - (rb1 * blend)) >> 8) & 0x00FF00FF;
      uint32_t wg3 = ((((wg1 << 8) | wg2) + (wg2 * blend) - (wg1 * blend))) & 0xFF00FF00;
      return rb3 | wg3;
    }


    #ifndef WLED_USE_REAL_MATH
      // template <typename T> T atan_t(T x);
      static float cos_t(float phi);
      static float sin_t(float x);
      static float tan_t(float x);
      static float acos_t(float x);
      static float asin_t(float x);
      static float floor_t(float x);
      static float fmod_t(float num, float denom);
    #else
      #include <math.h>
      #define sin_t sin
      #define cos_t cos
      #define tan_t tan
      #define asin_t asin
      #define acos_t acos
      #define atan_t atan
      #define fmod_t fmod
      #define floor_t floor
    #endif

    #include <map>
    #include <IPAddress.h>

    #define NODE_TYPE_ID_UNDEFINED        0
    #define NODE_TYPE_ID_ESP8266         82
    #define NODE_TYPE_ID_ESP32           32
    #define NODE_TYPE_ID_ESP32S2         33
    #define NODE_TYPE_ID_ESP32S3         34
    #define NODE_TYPE_ID_ESP32C3         35

    /*********************************************************************************************\
    * NodeStruct
    \*********************************************************************************************/
    struct NodeStruct
    {
      String    nodeName;
      IPAddress ip;
      uint8_t   age;
      uint8_t   nodeType;
      uint32_t  build;

      NodeStruct() : age(0), nodeType(0), build(0)
      {
        for (uint8_t i = 0; i < 4; ++i) { ip[i] = 0; }
      }
    };
    typedef std::map<uint8_t, NodeStruct> NodesMap;


    #define ARDUINOJSON_DECODE_UNICODE 0
    #include "3_Network/21_WebServer/AsyncJson-v6.h"
    #include "3_Network/21_WebServer/ArduinoJson-v6.h"

void serializeNetworks(JsonObject root);
    
void toggleOnOff();


//udp.cpp
void notify(byte callMode, bool followUp=false);
uint8_t realtimeBroadcast(uint8_t type, IPAddress client, uint16_t length, uint8_t *buffer, uint8_t bri=255, bool isRGBW=false);
void realtimeLock(uint32_t timeoutMs, byte md = REALTIME_MODE_GENERIC);
void exitRealtime();
void handleNotifications();
void setRealtimePixel(uint16_t i, byte r, byte g, byte b, byte w);
void refreshNodeList();
void sendSysInfoUDP();


bool deserializeConfig(JsonObject doc, bool fromFS = false);


void getStringFromJson(char* dest, const char* src, size_t len);
// Temp buffer
char* obuf;
uint16_t olen = 0;

bool oappend(const char* txt);
bool oappendi(int i);
void sappends(char stype, const char* key, char* val);
void sappend(char stype, const char* key, int val);



#ifdef ENABLE_FEATURE_LIGHTING__WEBUI
void serveSettingsJS(AsyncWebServerRequest* request);
void serveSettings(AsyncWebServerRequest* request, bool post = false);
// bool handleIfNoneMatchCacheHeader(AsyncWebServerRequest* request);
// void setStaticContentCacheHeaders(AsyncWebServerResponse *response);
void serveIndex(AsyncWebServerRequest* request);

#ifdef ENABLE_FEATURE_LIGHTING__XML_REQUESTS
// void getSettingsJS(byte subPage, Print& settingsScript);
#else
void getSettingsJS(byte subPage, char* dest); // phase out
#endif
void WebPage_Root_AddHandlers();
#endif // USE_MODULE_NETWORK_WEBSERVER


void serializeSegment(JsonObject& root, mAnimatorLight::Segment& seg, byte id, bool forPreset = false, bool segmentBounds = true);
void serializeState(JsonObject root, bool forPreset = false, bool includeBri = true, bool segmentBounds = true, bool selectedSegmentsOnly = false);
void serializeInfo(JsonObject root);
void serializeModeData(JsonArray root);
void serializePalettes(JsonObject root, int page);



    
//Notifier callMode
#define CALL_MODE_INIT           0     //no updates on init, can be used to disable updates
#define CALL_MODE_DIRECT_CHANGE  1
#define CALL_MODE_BUTTON         2     //default button actions applied to selected segments
#define CALL_MODE_NOTIFICATION   3
#define CALL_MODE_NIGHTLIGHT     4
#define CALL_MODE_NO_NOTIFY      5
#define CALL_MODE_FX_CHANGED     6     //no longer used
#define CALL_MODE_HUE            7
#define CALL_MODE_PRESET_CYCLE   8
#define CALL_MODE_BLYNK          9     //no longer used
#define CALL_MODE_ALEXA         10
#define CALL_MODE_WS_SEND       11     //special call mode, not for notifier, updates websocket only
#define CALL_MODE_BUTTON_PRESET 12     //button/IR JSON preset/macro



void serializeModeNames(JsonArray arr, bool flag_get_first_name_only = true);


#ifdef ENABLE_FEATURE_LIGHTING__WEBUI
void handleUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final);
bool serveLiveLeds(AsyncWebServerRequest* request, uint32_t wsClient = 0);

void serveJson(AsyncWebServerRequest* request);

void handleWs();

AsyncWebSocket* websocket_lights = nullptr;
#endif








void setPaletteColors(JsonArray json, CRGBPalette16 palette);

bool deserializeSegment(JsonObject elem, byte it, byte presetId = 0);

int getNumVal(const String* req, uint16_t pos);
void parseNumber(const char* str, byte &val, byte minv=0, byte maxv=255);
bool getVal(JsonVariant elem, byte &val, byte vmin=0, byte vmax=255); // getVal supports inc/decrementing and random ("X~Y(r|[w]~[-][Z])" form)

bool getBoolVal(JsonVariant elem, bool dflt);
bool updateVal(const char* req, const char* key, byte &val, byte minv=0, byte maxv=255);
size_t printSetFormCheckbox(Print& settingsScript, const char* key, int val);
size_t printSetFormValue(Print& settingsScript, const char* key, int val);
size_t printSetFormIndex(Print& settingsScript, const char* key, int index);
size_t printSetFormValue(Print& settingsScript, const char* key, const char* val);
size_t printSetClassElementHTML(Print& settingsScript, const char* key, const int index, const char* val);

size_t printSetFormInput(Print& settingsScript, const char* key, const char* selector, int value) ;


bool colorFromHexString(byte* rgb, const char* in);
bool deserializeState(JsonObject root, byte callMode = CALL_MODE_DIRECT_CHANGE, byte presetId = 0);

bool isIp(String str);


#define JSON_PATH_STATE      1
#define JSON_PATH_INFO       2
#define JSON_PATH_STATE_INFO 3
#define JSON_PATH_NODES      4
#define JSON_PATH_PALETTES   5
#define JSON_PATH_FXDATA     6
#define JSON_PATH_NETWORKS   7
#define JSON_PATH_EFFECTS    8


bool doReboot = false;

//realtime modes
#define REALTIME_MODE_INACTIVE    0
#define REALTIME_MODE_GENERIC     1
#define REALTIME_MODE_UDP         2
#define REALTIME_MODE_HYPERION    3
#define REALTIME_MODE_E131        4
#define REALTIME_MODE_ADALIGHT    5
#define REALTIME_MODE_ARTNET      6
#define REALTIME_MODE_TPM2NET     7
#define REALTIME_MODE_DDP         8

#ifndef WLED_MAX_BUTTONS
  #ifdef ESP8266
    #define WLED_MAX_BUTTONS 2
  #else
    #define WLED_MAX_BUTTONS 4
  #endif
#endif

#ifndef WLED_MAX_SEGNAME_LEN
  #ifdef ESP8266
    #define WLED_MAX_SEGNAME_LEN 32
  #else
    #define WLED_MAX_SEGNAME_LEN 64
  #endif
#else
  #if WLED_MAX_SEGNAME_LEN<32
    #undef WLED_MAX_SEGNAME_LEN
    #define WLED_MAX_SEGNAME_LEN 32
  #else
    #warning WLED UI does not support modified maximum segment name length!
  #endif
#endif

#define WLED_GLOBAL

// #ifndef WLED_DEFINE_GLOBAL_VARS
// # define extern
// # define _INIT(x)
// # define _INIT_N(x)
// #else
# define WLED_GLOBAL
# define _INIT(x) = x

//needed to ignore commas in array definitions
#define UNPACK( ... ) __VA_ARGS__
#define _INIT_N(x) UNPACK x
// #endif

#ifdef ENABLE_DEBUGFEATURE_WEBUI__SHOW_BUILD_DATETIME_IN_FOOTER
char serverDescription[80];
#else
char serverDescription[40];
#endif


bool syncToggleReceive     _INIT(false);   // UIs which only have a single button for sync should toggle send+receive if this is true, only send otherwise
bool simplifiedUI          _INIT(false);   // enable simplified UI

#ifndef WLED_DISABLE_ESPNOW
bool enable_espnow_remote _INIT(false);
char linked_remote[13] = {0};//   _INIT("");
char last_signal_src[13] = {0};//   _INIT("");
#endif


char ntpServerName[33] = {0};//  _INIT("0.wled.pool.ntp.org");   // NTP server to use

// #define MDNS_NAME DEVICENAME_CTR ".local"
#define CLIENT_SSID "HACS2400"
#define CLIENT_PASS "af4d8bc9ab"
//Access point behavior
#define AP_BEHAVIOR_BOOT_NO_CONN          0     //Open AP when no connection after boot
#define AP_BEHAVIOR_NO_CONN               1     //Open when no connection (either after boot or if connection is lost)
#define AP_BEHAVIOR_ALWAYS                2     //Always open
#define AP_BEHAVIOR_BUTTON_ONLY           3     //Only when button pressed for 6 sec

// AP and OTA default passwords (for maximum security change them!)
char apPass[65]  = {0};//  _INIT(CLIENT_SSID);
char otaPass[33]  = {0};// _INIT("");

// WiFi CONFIG (all these can be changed via web UI, no need to set them here)
char clientSSID[33]   = {0};//_INIT(CLIENT_SSID);
char clientPass[65]  = {0};// _INIT(CLIENT_PASS);
// char cmDNS[] _INIT(MDNS_NAME);                       // mDNS address (*.local, replaced by wledXXXXXX if default is used)
char apSSID[33]  = {0};// _INIT("");                             // AP off by default (unless setup)
byte apChannel _INIT(1);                               // 2.4GHz WiFi AP channel (1-13)
byte apHide    _INIT(0);                               // hidden AP SSID
byte apBehavior _INIT(AP_BEHAVIOR_BOOT_NO_CONN);       // access point opens when no connection after boot by default
IPAddress staticIP   ;//   _INIT_N(((  0,   0,  0,  0))); // static IP of ESP
IPAddress staticGateway ;//_INIT_N(((  0,   0,  0,  0))); // gateway (router) IP
IPAddress staticSubnet;//  _INIT_N(((255, 255, 255, 0))); // most common subnet in home networks
#ifdef ARDUINO_ARCH_ESP32
bool noWifiSleep _INIT(true);                          // disabling modem sleep modes will increase heat output and power usage, but may help with connection issues
#else
bool noWifiSleep _INIT(false);
#endif
typedef enum mapping1D2D {
  M12_Pixels = 0,
  M12_pBar = 1,
  M12_pArc = 2,
  M12_pCorner = 3,
  M12_sPinwheel = 4
} mapping1D2D_t;



#ifdef ENABLE_FEATURE_LIGHTING__XML_REQUESTS

static void extractPin(Print& settingsScript, JsonObject &obj, const char *key);
void XML_response(Print& dest);
static void fillUMPins(Print& settingsScript, JsonObject &mods);
void appendGPIOinfo(Print& settingsScript);
void getSettingsJS(byte subPage, Print& settingsScript);

#endif

// Settings sub page IDs
#define SUBPAGE_MENU              0
#define SUBPAGE_WIFI              1
#define SUBPAGE_LEDS              2
#define SUBPAGE_UI                3
#define SUBPAGE_SYNC              4
#define SUBPAGE_TIME              5
#define SUBPAGE_SEC               6
#define SUBPAGE_DMX               7
#define SUBPAGE_UM                8
#define SUBPAGE_UPDATE            9
#define SUBPAGE_2D               10
#define SUBPAGE_LOCK            251
#define SUBPAGE_PINREQ          252
#define SUBPAGE_CSS             253
#define SUBPAGE_JS              254

// string temp buffer (now stored in stack locally)
#ifdef ESP8266
#define SETTINGS_STACK_BUF_SIZE 2048
#else
#define SETTINGS_STACK_BUF_SIZE 3608  // warning: quite a large value for stack
#endif

#ifdef WLED_USE_ETHERNET
  #ifdef WLED_ETH_DEFAULT                                          // default ethernet board type if specified
    int ethernetType _INIT(WLED_ETH_DEFAULT);          // ethernet board type
  #else
    int ethernetType _INIT(WLED_ETH_NONE);             // use none for ethernet board type if default not defined
  #endif
#endif

// Timer mode types
#define NL_MODE_SET               0            //After nightlight time elapsed, set to target brightness
#define NL_MODE_FADE              1            //Fade to target brightness gradually
#define NL_MODE_COLORFADE         2            //Fade to target brightness and secondary color gradually
#define NL_MODE_SUN               3            //Sunrise/sunset. Target brightness is set immediately, then Sunrise effect is started. Max 60 min.

// LED CONFIG
bool turnOnAtBoot _INIT(true);                // turn on LEDs at power-up
byte bootPreset   _INIT(0);                   // save preset to load after power-up

//if true, a segment per bus will be created on boot and LED settings save
//if false, only one segment spanning the total LEDs is created,
//but not on LED settings save if there is more than one segment currently
bool autoSegments    _INIT(false);
bool correctWB       _INIT(false); // CCT color correction of RGB color
bool cctFromRgb      _INIT(false); // CCT is calculated from RGB instead of using seg.cct
// bool gammaCorrectCol _INIT(true ); // use gamma correction on colors
bool gammaCorrectBri _INIT(false); // use gamma correction on brightness
float gammaCorrectVal _INIT(2.8f); // gamma correction value


#ifdef WLED_USE_IC_CCT
bool cctICused          _INIT(true);  // CCT IC used (Athom 15W bulbs)
#else
bool cctICused          _INIT(false); // CCT IC used (Athom 15W bulbs)
#endif



byte col[4]    _INIT_N(({ 255, 160, 0, 0 }));  // current RGB(W) primary color. col[] should be updated if you want to change the color.
byte colSec[4] = UNPACK ({ 0, 0, 0, 0 });      // current RGB(W) secondary color


byte nightlightTargetBri _INIT(0);      // brightness after nightlight is over
byte nightlightDelayMins _INIT(60);
byte nightlightMode      _INIT(NL_MODE_FADE); // See const.h for available modes. Was nightlightFade
bool fadeTransition      _INIT(true);   // enable crossfading color transition
// uint16_t transitionDelay _INIT(750);    // default crossfade duration in ms

byte briMultiplier _INIT(100);          // % of brightness to set (to limit power, if you set it to 50 and set bri to 255, actual brightness will be 127)

// // Sync CONFIG
// NodesMap Nodes;
bool nodeListEnabled _INIT(true);
bool nodeBroadcastEnabled _INIT(true);

// byte buttonType[WLED_MAX_BUTTONS]  _INIT({BTN_TYPE_PUSH});
#if defined(IRTYPE) && defined(IRPIN)
byte irEnabled      _INIT(IRTYPE); // Infrared receiver
#else
byte irEnabled      _INIT(0);     // Infrared receiver disabled
#endif
bool irApplyToAllSelected _INIT(true); //apply IR to all selected segments

uint16_t udpPort    _INIT(21324); // WLED notifier default port
uint16_t udpPort2   _INIT(65506); // WLED notifier supplemental port
uint16_t udpRgbPort _INIT(19446); // Hyperion port

uint8_t syncGroups    _INIT(0x01);                    // sync groups this instance syncs (bit mapped)
uint8_t receiveGroups _INIT(0x01);                    // sync receive groups this instance belongs to (bit mapped)
bool receiveNotificationBrightness _INIT(true);       // apply brightness from incoming notifications
bool receiveNotificationColor      _INIT(true);       // apply color
bool receiveNotificationEffects    _INIT(true);       // apply effects setup
bool receiveSegmentOptions         _INIT(false);      // apply segment options
bool receiveSegmentBounds          _INIT(false);      // apply segment bounds (start, stop, offset)
bool notifyDirect _INIT(false);                       // send notification if change via UI or HTTP API
bool notifyButton _INIT(false);                       // send if updated by button or infrared remote
bool notifyAlexa  _INIT(false);                       // send notification if updated via Alexa
bool notifyMacro  _INIT(false);                       // send notification for macro
bool notifyHue    _INIT(true);                        // send notification if Hue light changes
uint8_t udpNumRetries _INIT(0);                       // Number of times a UDP sync message is retransmitted. Increase to increase reliability

uint16_t realtimeTimeoutMs _INIT(2500);               // ms timeout of realtime mode before returning to normal mode
int arlsOffset _INIT(0);                              // realtime LED offset
bool receiveDirect _INIT(true);                       // receive UDP realtime
bool arlsDisableGammaCorrection _INIT(true);          // activate if gamma correction is handled by the source
#ifdef ENABLE_FEATURE_LIGHTING__SKIP_GAMMA_CORRECTION_ON_PULSAR_PALETTES
bool frame_use_gamma_correction = true;
#endif
bool arlsForceMaxBri _INIT(false);                    // enable to force max brightness if source has very dark colors that would be black

#ifdef WLED_USE_ETHERNET
  #define E131_MAX_UNIVERSE_COUNT 20
#else
  #ifdef ESP8266
    #define E131_MAX_UNIVERSE_COUNT 9
  #else
    #define E131_MAX_UNIVERSE_COUNT 12
  #endif
#endif

//E1.31 DMX modes
#define DMX_MODE_DISABLED         0            //not used
#define DMX_MODE_SINGLE_RGB       1            //all LEDs same RGB color (3 channels)
#define DMX_MODE_SINGLE_DRGB      2            //all LEDs same RGB color and master dimmer (4 channels)
#define DMX_MODE_EFFECT           3            //trigger standalone effects of WLED (11 channels)
#define DMX_MODE_MULTIPLE_RGB     4            //every LED is addressed with its own RGB (ledCount * 3 channels)
#define DMX_MODE_MULTIPLE_DRGB    5            //every LED is addressed with its own RGB and share a master dimmer (ledCount * 3 + 1 channels)
#define DMX_MODE_MULTIPLE_RGBW    6            //every LED is addressed with its own RGBW (ledCount * 4 channels)

#ifdef ENABLE_FEATURE_LIGHTING__DMX
 #ifdef ESP8266
  DMXESPSerial dmx;
 #else //ESP32
  SparkFunDMX dmx;
 #endif
uint16_t e131ProxyUniverse _INIT(0);                  // output this E1.31 (sACN) / ArtNet universe via MAX485 (0 = disabled)
#endif
uint16_t e131Universe _INIT(1);                       // settings for E1.31 (sACN) protocol (only DMX_MODE_MULTIPLE_* can span over consequtive universes)
uint16_t e131Port _INIT(5568);                        // DMX in port. E1.31 default is 5568, Art-Net is 6454
// byte e131Priority _INIT(0);                           // E1.31 port priority (if != 0 priority handling is active)
// E131Priority highPriority _INIT(3);                   // E1.31 highest priority tracking, init = timeout in seconds
byte DMXMode _INIT(DMX_MODE_MULTIPLE_RGB);            // DMX mode (s.a.)
uint16_t DMXAddress _INIT(1);                         // DMX start address of fixture, a.k.a. first Channel [for E1.31 (sACN) protocol]
uint16_t DMXSegmentSpacing _INIT(0);                  // Number of void/unused channels between each segments DMX channels
byte e131LastSequenceNumber[E131_MAX_UNIVERSE_COUNT]; // to detect packet loss
bool e131Multicast _INIT(false);                      // multicast or unicast
bool e131SkipOutOfSequence _INIT(false);              // freeze instead of flickering
uint16_t pollReplyCount _INIT(0);                     // count number of replies for ArtPoll node report

// // Time CONFIG
time_t localTime _INIT(0);
// bool ntpEnabled _INIT(false);    // get internet time. Only required if you use clock overlays or time-activated macros
// byte currentTimezone _INIT(0);   // Timezone ID. Refer to timezones array in wled10_ntp.ino
// int utcOffsetSecs _INIT(0);      // Seconds to offset from UTC before timzone calculation


// Segment capability byte
#define SEG_CAPABILITY_RGB     0x01
#define SEG_CAPABILITY_W       0x02
#define SEG_CAPABILITY_CCT     0x04

// byte overlayCurrent _INIT(0);    // 0: no overlay 1: analog clock 2: was single-digit clock 3: was cronixie
// byte overlayMin _INIT(0), overlayMax _INIT(DEFAULT_LED_COUNT - 1);   // boundaries of overlay mode

// // byte analogClock12pixel _INIT(0);               // The pixel in your strip where "midnight" would be
// // bool analogClockSecondsTrail _INIT(false);      // Display seconds as trail of LEDs instead of a single pixel
// // bool analogClock5MinuteMarks _INIT(false);      // Light pixels at every 5-minute position

// // byte macroNl   _INIT(0);        // after nightlight delay over
// // byte macroCountdown _INIT(0);
// // byte macroAlexaOn _INIT(0), macroAlexaOff _INIT(0);
// // byte macroButton[WLED_MAX_BUTTONS]        _INIT({0});
// // byte macroLongPress[WLED_MAX_BUTTONS]     _INIT({0});
// // byte macroDoublePress[WLED_MAX_BUTTONS]   _INIT({0});

// // uint16_t userVar0 _INIT(0), userVar1 _INIT(0); //available for use in usermod

// #ifdef ENABLE_FEATURE_LIGHTING__DMX
//   // dmx CONFIG
//   byte DMXChannels _INIT(7);        // number of channels per fixture
//   byte DMXFixtureMap[15] _INIT_N(({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }));
//   // assigns the different channels to different functions. See wled21_dmx.ino for more information.
//   uint16_t DMXGap _INIT(10);          // gap between the fixtures. makes addressing easier because you don't have to memorize odd numbers when climbing up onto a rig.
//   uint16_t DMXStart _INIT(10);        // start address of the first fixture
//   uint16_t DMXStartLED _INIT(0);      // LED from which DMX fixtures start
// #endif

// internal global variable declarations
// wifi
bool apActive _INIT(false);
bool forceReconnect _INIT(false);
uint32_t lastReconnectAttempt _INIT(0);
bool interfacesInited _INIT(false);
bool wasConnected _INIT(false);

// color
byte lastRandomIndex _INIT(0);        // used to save last random color so the new one is not the same

// REMOVED
// // transitions
// bool          transitionActive        _INIT(false);
// uint16_t      transitionDelayDefault  _INIT(transitionDelay); // default transition time (storec in cfg.json)
// uint16_t      transitionDelayTemp     _INIT(transitionDelay); // actual transition duration (overrides transitionDelay in certain cases)
// unsigned long transitionStartTime;
float         tperLast                _INIT(0.0f);            // crossfade transition progress, 0.0f - 1.0f
bool          jsonTransitionOnce      _INIT(false);           // flag to override transitionDelay (playlist, JSON API: "live" & "seg":{"i"} & "tt")
uint8_t       randomPaletteChangeTime _INIT(5);               // amount of time [s] between random palette changes (min: 1s, max: 255s)

// nightlight
bool nightlightActive _INIT(false);
bool nightlightActiveOld _INIT(false);
uint32_t nightlightDelayMs _INIT(10);
byte nightlightDelayMinsDefault _INIT(nightlightDelayMins);
unsigned long nightlightStartTime;
byte briNlT _INIT(0);                     // current nightlight brightness
byte colNlT[4] _INIT_N(({ 0, 0, 0, 0 }));        // current nightlight color

// brightness
unsigned long lastOnTime _INIT(0);
bool offMode             _INIT(!turnOnAtBoot);

#define TOUCH_THRESHOLD 32 // limit to recognize a touch, higher value means more sensitive

// button
bool buttonPublishMqtt                            _INIT(false);
bool buttonPressedBefore[WLED_MAX_BUTTONS]        _INIT({false});
bool buttonLongPressed[WLED_MAX_BUTTONS]          _INIT({false});
unsigned long buttonPressedTime[WLED_MAX_BUTTONS] _INIT({0});
unsigned long buttonWaitTime[WLED_MAX_BUTTONS]    _INIT({0});
bool disablePullUp                                _INIT(false);
byte touchThreshold                               _INIT(TOUCH_THRESHOLD);

//Notifier callMode
#define CALL_MODE_INIT           0     //no updates on init, can be used to disable updates
#define CALL_MODE_DIRECT_CHANGE  1
#define CALL_MODE_BUTTON         2     //default button actions applied to selected segments
#define CALL_MODE_NOTIFICATION   3
#define CALL_MODE_NIGHTLIGHT     4
#define CALL_MODE_NO_NOTIFY      5
#define CALL_MODE_FX_CHANGED     6     //no longer used
#define CALL_MODE_HUE            7
#define CALL_MODE_PRESET_CYCLE   8
#define CALL_MODE_BLYNK          9     //no longer used
#define CALL_MODE_ALEXA         10
#define CALL_MODE_WS_SEND       11     //special call mode, not for notifier, updates websocket only
#define CALL_MODE_BUTTON_PRESET 12     //button/IR JSON preset/macro

unsigned long lastInterfaceUpdate _INIT(0);
byte interfaceUpdateCallMode _INIT(CALL_MODE_INIT);


// notifications
bool notifyDirectDefault _INIT(notifyDirect);
bool receiveNotifications _INIT(true);
unsigned long notificationSentTime _INIT(0);
byte notificationSentCallMode _INIT(CALL_MODE_INIT);
uint8_t notificationCount _INIT(0);

// effects
byte effectCurrent =0;
byte effectSpeed =128;
byte effectIntensity =128;
byte effectPalette =0;
bool stateChanged =false;

// network
bool udpConnected _INIT(false), udp2Connected _INIT(false), udpRgbConnected _INIT(false);

// ui style
bool showWelcomePage _INIT(false);

//playlists
// int16_t currentPlaylist _INIT(-1);
//still used for "PL=~" HTTP API command
byte presetCycCurr _INIT(0);
byte presetCycMin _INIT(1);
byte presetCycMax _INIT(5);


//realtime override modes
#define REALTIME_OVERRIDE_NONE    0
#define REALTIME_OVERRIDE_ONCE    1
#define REALTIME_OVERRIDE_ALWAYS  2

// realtime
byte realtimeOverride _INIT(REALTIME_OVERRIDE_NONE);
IPAddress realtimeIP;// _INIT_N(((0, 0, 0, 0)));
unsigned long realtimeTimeout _INIT(0);
uint8_t tpmPacketCount _INIT(0);
uint16_t tpmPayloadFrameSize _INIT(0);
bool useMainSegmentOnly _INIT(false);



// #ifdef ENABLE_FEATURE_LIGHTING__STANDBY_VIRTUAL_PRESET

// // ---------- Standby meta ----------
// enum class STBY_SRC : uint8_t { Unknown=0, Template, Uploaded, Current, FS };

// struct STANDBY {
//   // runtime status
//   bool     active        = false;
//   bool     haveResumeRAM = false;
//   uint8_t  callModeLast  = CALL_MODE_NO_NOTIFY;

//   // fades
//   uint16_t fade_in_ms    = 600;
//   uint16_t fade_out_ms   = 600;

//   // inactivity timeout (optional)
//   uint32_t timeout_ms    = 0;
//   uint32_t deadline_ms   = 0;

//   // profile + snapshot blobs
//   char*    profileRAM    = nullptr;
//   size_t   profileLen    = 0;
//   char*    resumeRAM     = nullptr;
//   size_t   resumeLen     = 0;

//   // provenance
//   STBY_SRC last_src      = STBY_SRC::Unknown;
//   int      last_ver      = -1;

//   // delayed-start scheduling
//   bool     delayedStartPending = false;
//   uint32_t delayedStartAtMs    = 0;
//   uint8_t  delayedStartCallMode= CALL_MODE_NO_NOTIFY;
// };

// public:
//   // ---------- Standby state ----------
//   STANDBY standby;

//   // ---------- Profile management ----------
//   bool Standby_Init();  // load/gate profile, enforce template version if compiled
//   bool Standby_ReloadTemplate(bool persist = true);
//   bool Standby_SetProfileFromJson(const char* json,
//                                   STBY_SRC src = STBY_SRC::Uploaded);
//   bool Standby_SaveProfileToFS();

//   // ---------- Start/Stop ----------
//   bool Standby_Start(uint16_t fadeMs = 0, uint8_t callMode = CALL_MODE_NO_NOTIFY);
//   bool Standby_Stop (uint16_t fadeMs = 0, uint8_t callMode = CALL_MODE_NO_NOTIFY);
//   inline bool Standby_IsActive() const { return standby.active; }

//   // ---------- Timeout / activity hooks ----------
//   void Standby_OnActivity();  // resets deadline based on timeout_ms
//   void EverySecond_Standby();        // call each loop to handle timeout & delayed start

//   // ---------- Delayed start API ----------
//   bool Standby_ScheduleStart(uint32_t delay_ms,
//                              uint8_t callMode = CALL_MODE_NO_NOTIFY);
//   void Standby_CancelScheduledStart();

//   // ---------- Snapshot & validation ----------
//   bool FileSave__State(bool includeBounds = true, bool includeBri = true,
//                        bool selectedOnly = false, bool fullGlobals = false);
//   bool FileLoad__State(uint8_t callMode = CALL_MODE_NO_NOTIFY);
//   bool ValidateJSON(const char* json_str);

// private:
//   // ---------- Helpers ----------
//   bool Standby_CaptureResumeToRAM();
//   bool Standby_JsonCommand_Run(const char* json, size_t len, uint8_t callMode);

//   // ---------- FS paths ----------
//   static const char kStandbyProfilePath[]  PROGMEM; // "/lgt_standby.json"
//   static const char kStateSnapshotPath[]   PROGMEM; // "/lgt_state.json"
//   #ifdef ENABLE_DEBUGFEATURE_LIGHTING__STANDBY_STATE_SNAPSHOT_MIRROR_FILESYSTEM
//   static const char kResumeSnapshotPath[] PROGMEM;  // "/lgt_state_resume.json"
//   #endif

//   // ---------- FS I/O ----------
//   bool Standby_LoadProfileFromFS();
//   bool Standby_WriteProfileToFS(const char* json);

//   // ---------- Template loader ----------
//   bool Load_StandbyTemplate_Into_ProfileRAM(bool injectTemplateId = true);

//   // ---------- Logging ----------
//   static const char* StbySrcName(STBY_SRC s);
// #endif


#ifdef ENABLE_FEATURE_LIGHTING__STANDBY_VIRTUAL_PRESET
#include "Decounter.h"   // your header shown earlier

enum class STBY_SRC : uint8_t { Unknown=0, Template, Uploaded, Current, FS };

struct STANDBY {
  bool     active        = false;
  bool     haveResumeRAM = false;
  uint8_t  callModeLast  = CALL_MODE_NO_NOTIFY;

  uint16_t fade_in_ms    = 600;
  uint16_t fade_out_ms   = 600;

  uint32_t timeout_ms    = 0;     // (unused with Decounter; keep if you want)
  uint32_t deadline_ms   = 0;     // (unused with Decounter; keep if you want)

  // runtime RAM blobs
  char*    profileRAM    = nullptr;
  size_t   profileLen    = 0;
  char*    resumeRAM     = nullptr;
  size_t   resumeLen     = 0;

  STBY_SRC last_src      = STBY_SRC::Unknown;
  int      last_ver      = -1;

  // --- New: single remain-awake countdown ---
  // When running, and it hits 0, enter Standby.
  Decounter<uint32_t> tick__awake;     // seconds
  uint16_t            fade_override_ms = 0;  // optional override on Standby_Stop (Wake)
 
  // ctor-like defaults for clarity
  STANDBY() : active(false), haveResumeRAM(false), callModeLast(CALL_MODE_NO_NOTIFY) {}
};

public:
  STANDBY standby;

  // profile mgmt
  bool Standby_Init();
  bool Standby_ReloadTemplate(bool persist = true);
  bool Standby_SetProfileFromJson(const char* json, STBY_SRC src = STBY_SRC::Uploaded);
  bool Standby_SaveProfileToFS();

  // start/stop
  bool Standby_Start(uint16_t fadeMs = 0, uint8_t callMode = CALL_MODE_NO_NOTIFY);
  bool Standby_Stop (uint16_t fadeMs = 0, uint8_t callMode = CALL_MODE_NO_NOTIFY);
  inline bool Standby_IsActive() const { return standby.active; }

  // remain-awake control (single timer)
  inline void Standby_SetRemainAwake(uint32_t secs) { standby.tick__awake.Start(secs); }
  inline void Standby_CancelRemainAwake()          { standby.tick__awake.Stop(); }
  void        EverySecond_Standby();  // call this from your 1 Hz task

  // hooks / helpers
  void Standby_OnActivity();      // e.g. reset remain-awake on motion, if desired
  bool Load_StandbyTemplate_Into_ProfileRAM(bool injectTemplateId = true);
  bool FileSave__State(bool includeBounds = true, bool includeBri = true,
                       bool selectedOnly = false, bool fullGlobals = false);
  bool FileLoad__State(uint8_t callMode = CALL_MODE_NO_NOTIFY);
  bool ValidateJSON(const char* json_str);

private:
  bool Standby_CaptureResumeToRAM();
  bool Standby_JsonCommand_Run(const char* json, size_t len, uint8_t callMode);
  bool Standby_LoadProfileFromFS();
  bool Standby_WriteProfileToFS(const char* json);
  static const char* StbySrcName(STBY_SRC s);

  static const char kStandbyProfilePath[]  PROGMEM;
  static const char kStateSnapshotPath[]   PROGMEM;
  #ifdef ENABLE_DEBUGFEATURE_LIGHTING__STANDBY_STATE_SNAPSHOT_MIRROR_FILESYSTEM
  static const char kResumeSnapshotPath[] PROGMEM;
  #endif
#endif



    /************************************************************************************************
     * SECTION: ConstructJSON
     ************************************************************************************************/

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Segments(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Playlist(uint8_t json_level = 0, bool json_appending = true);
    /**
     * @brief Each mode
     */
    #ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
    uint8_t ConstructJSON_Mode_Ambilight(uint8_t json_level = 0, bool json_appending = true);
    #endif
    #ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
    uint8_t ConstructJSON_Mode_SetManual(uint8_t json_level = 0, bool json_appending = true); // probably falls into the E131 type, but here set my mqtt
    #endif
    #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    uint8_t ConstructJSON_Auto_Presets(uint8_t json_level = 0, bool json_appending = true);
    #endif 
    #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS 
    uint8_t ConstructJSON_Matrix(uint8_t json_level = 0, bool json_appending = true);
    #endif
    /**
     * @brief Debug 
     */
    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    uint8_t ConstructJSON_Debug_Palette(uint8_t json_level = 0, bool json_appending = true);
    #endif
    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    uint8_t ConstructJSON_Debug_Segments(uint8_t json_level = 0, bool json_appending = true);
    #endif
    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_CUSTOM_MAPPING_TABLE
    uint8_t ConstructJSON_Debug__CustomMappingTable(uint8_t json_level = 0, bool json_appending = true);
    #endif
    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
    uint8_t ConstructJSON_Debug_Palette_Vector(uint8_t json_level = 0, bool json_appending = true);
    #endif
    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PERFORMANCE
    uint8_t ConstructJSON_Debug_Performance(uint8_t json_level = 0, bool json_appending = true);
    #endif 
    #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
      uint8_t ConstructJSON_Debug_Animations_Progress(uint8_t json_level = 0, bool json_appending = true);  
      ANIMIMATION_DEBUG_MQTT_FUNCTION_SIGNATURE;
      mAnimatorLight& setCallback_ConstructJSONBody_Debug_Animations_Progress(ANIMIMATION_DEBUG_MQTT_FUNCTION_SIGNATURE);  
    #endif

    /************************************************************************************************
     * SECTION: MQTT
     ************************************************************************************************/
    
    #ifdef USE_MODULE_NETWORK_MQTT
      void MQTTHandler_Init();
      
      std::vector<struct handler<mAnimatorLight>*> mqtthandler_list;
    
      struct handler<mAnimatorLight> mqtthandler_settings;    
      struct handler<mAnimatorLight> mqtthandler_segments_teleperiod;  
      struct handler<mAnimatorLight> mqtthandler_playlists_teleperiod;
      /**
       * @brief Each mode
       **/
      #ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
      struct handler<mAnimatorLight> mqtthandler_mode_ambilight_teleperiod;
      #endif
      #ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
      struct handler<mAnimatorLight> mqtthandler_manual_setpixel;
      #endif
      #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
      struct handler<mAnimatorLight> mqtthandler_automation_presets;
      #endif
      #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PLAYLISTS
      struct handler<mAnimatorLight> mqtthandler_automation_playlists;
      #endif
      #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS 
      struct handler<mAnimatorLight> mqtthandler_matrix_teleperiod;
      #endif
      /**
       * @brief Debug
       **/
      #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
      struct handler<mAnimatorLight> mqtthandler_debug_palette;
      #endif
      #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
      struct handler<mAnimatorLight> mqtthandler_debug_segments;
      #endif
      #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_CUSTOM_MAPPING_TABLE
      struct handler<mAnimatorLight> mqtthandler_debug__custom_mapping_table;
      #endif
      #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
      struct handler<mAnimatorLight> mqtthandler_debug_palette_vector;
      #endif
      #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PERFORMANCE
      struct handler<mAnimatorLight> mqtthandler_debug__performance;
      #endif
      #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
      struct handler<mAnimatorLight> mqtthandler_debug_animations_progress;
      #endif
      
    #endif // USE_MODULE_NETWORK_MQTT

    

    /************************************************************************************************
     * SECTION: Development Code
     ************************************************************************************************/

    #ifdef ENABLE_DEVFEATURE_LIGHTING__DEVELOPING_CODE
    void TestCode_AddBus1();
    void TestCode_Add16ParallelBus1();
    #endif // ENABLE_DEVFEATURE_LIGHTING__DEVELOPING_CODE

};

#ifdef ENABLE_FEATURE_LIGHTS__GLOBAL_ANIMATOR_LIGHT_CLASS_ACCESS
// Extern declaration of the global instance
extern mAnimatorLight* tkr_extern_lAni;  // global instance of the mAnimatorLight class for performance reasons
#endif


#endif

#endif

/**
 * ─────────────────────────────────────────────────────────────────────────────
 * WLED → PulSar Conversion Notes
 * ─────────────────────────────────────────────────────────────────────────────
 *
 * Purpose:
 *   These defines and notes are temporary shims that allow PulSar code to
 *   interoperate with legacy WLED patterns until all callsites are ported.
 *   They should be removed once everything consistently uses the new
 *   GetPaletteColour_* APIs.
 *
 * Context:
 *   WLED and PulSar differ in how palette indices, wrapping, and modes are
 *   handled. WLED had booleans for "mapping" and "wrap"; PulSar has explicit
 *   enums for index encoding, wrap mode, and palette mode.
 *
 * Original WLED function:
 *     CRGB color_from_palette(i, mapping, wrap, mcol, pbri=255)
 *
 * Parameters:
 *   i       → Palette index (0..255, or scaled by segment length if mapping=true)
 *   mapping → if true, i spans SEGLEN and is mapped to 0..255
 *   wrap    → if true, circular blend at 255→0; if false, clamp (hard edge)
 *   mcol    → if palette 0 selected, returns SEGCOLOR[0..2]; >2 → Party palette
 *   pbri    → brightness scale (255=no scale)
 *
 * PulSar equivalent:
 *     GetPaletteColour_Legacy(i, indexEncoding, wrapMode, paletteMode, encodedVal)
 *
 * Example mapping:
 *   WLED:
 *     SEGMENT.color_from_palette(band*35, false, PALETTE_SOLID_WRAP, 0);
 *
 *   PulSar:
 *     SEGMENT.GetPaletteColour_Legacy(
 *         band*35,
 *         WLED_PALETTE_MAPPING_ARG_FALSE,  // index already 0..255
 *         PALETTE_WRAP_SMOOTH,             // wrap=on
 *         PALETTE_MODE__DEFAULT,           // palette mode
 *         NO_ENCODED_VALUE                 // no encoded override
 *     ).getU32();
 *
 * Conversion reference:
 *   WLED                 →   PulSar
 *   --------------------     ------------------------------
 *   SEGENV                   SEGMENT
 *   isMatrix                 SEGMENT.isMatrix
 *   SEGCOLOR                 SEGCOLOR_U32
 *   color_from_palette()     GetPaletteColour_Legacy().getU32()
 *   aux1                     SEGMENT.params_internal.aux1
 *
 * Legacy aliases (for compatibility only):
 *
 * #define color_from_palette(i,mapping,wrap,mcol) \
 *  GetPaletteColour_Legacy(i, mapping, wrap, PALETTE_MODE__DEFAULT, NO_ENCODED_VALUE).getU32()
 */

 

/**
 * @brief With the above or by other means
 * 
 * P+ Pop ON, then fade off
 * P+ Fade on, then fade off
 * P+ Pop Cycle (Amount to pop on, then off should be in sine wave)
 * 
 * Twinkle Smooth, is closer to pops
 * 
 * Twinkle smooth = fade on, then off
 * Twinkle Spark = instant on, then fade off
 * Twinkle rise, unknown, but in same as the above
 * 
 * 
 * Twinkle=flickers, Pops=Graduals
 */
