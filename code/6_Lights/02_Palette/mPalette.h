#ifndef _M_PALETTE_H
#define _M_PALETTE_H

// Create Version 2 of mPalette, to allow for more features and better code structure
/***
 * 
 * U32 palettes should be default, unless forced to allow RGBWW with GENERATE
 * Refactor "exact" and "forced_gradient", so U8 can be used to determine [default mode as defined, forced discrete, forced gradient by effects]
 * GETAS_DEFAULT
 * GETAS_DISCRETE
 * GETAS_GRADIENT
  


 */

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#include "6_Lights/02_Palette/mPalette_Progmem.h"
#include "2_CoreSystem/11_Languages/mLanguageProgmem.h"
#include "1_TaskerManager/mTaskerManager.h"

#define FASTLED_INTERNAL // suppress pragma warning messages
#include "6_Lights/03_Animator/fastled_slim/fastled_slim.h"

#include "mPalette_Encoding_Options.h"


class mPalette 
{
  private:
    /* Prevent others from being created */
    mPalette(mPalette const& other) = delete;
    mPalette(mPalette&& other) = delete;
    /* Private constructor to prevent instancing. */
    mPalette(){ 
      // Serial.println("DEBUG_LINE_HERE_PAUSE"); delay(2000); 
      Init_Palettes();
    };
  public:
    // External function to get instance
    static mPalette* GetInstance();
    /* Here will be the instance stored. */
    static mPalette* instance;

    void Init_Palettes();

    /**************
     * COLOUR Designs by name
     * PALETTE are different colours by pixel
    **************/                            
    int16_t Get_Static_PaletteIDbyName(const char* c);
    const char* GetPaletteNameByID(uint8_t id, char* buffer, uint8_t buflen);

    
    /************************************************************************************************************************************
     * ************************************************************************************************************************************
     * @brief Palette ID groups
     *
     * Palette groups are separated by their actual runtime/source behaviour.
     *
     * 1) SEGMENT__SEGMENT_COLOUR
     *    Direct references to the segment's 1–5 user colours.
     *
     * 2) SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES
     *    CRGBPalette16 palettes generated from the segment colours.
     *
     * 3) PROCEDURAL
     *    Stateless palettes calculated directly from the requested palette index.
     *    No stored palette data is required.
     *
     * 4) STATIC_CRGBPALETTE16
     *    Static indexed RGB gradient definitions loaded into CRGBPalette16.
     *    Includes the former FastLED fixed palettes, WLED gradient palettes and MATLAB colour maps.
     *
     * 5) STATIC_SINGLE_COLOURS
     *    Named constant single-colour sources.
     *
     * 6) STATIC
     *    Native PulSar encoded/vector palettes supporting RGB/RGBWW/CCT,
     *    discrete colours, indexed gradients and other encoded data.
     *
     * 7) DYNAMIC__COLOUR_CRGBPALETTE
     *    Runtime-generated CRGBPalette16 palettes.
     *
     * 8) DYNAMIC__COLOUR
     *    Runtime/context-generated palettes driven by time, solar position,
     *    segment colours or other external state.
     *
     ************************************************************************************************************************************
    ***************************************************************************************************************************************/


    /**
     * @brief
     * Single colour user options, up to all 5 RGBCCT elements.
     */
    enum PALETTELIST_SEGMENT__SEGMENT_COLOUR__IDS
    {
      PALETTELIST_SEGMENT__SEGMENT_COLOUR_01__ID = 0,
      PALETTELIST_SEGMENT__SEGMENT_COLOUR_02__ID,
      PALETTELIST_SEGMENT__SEGMENT_COLOUR_03__ID,
      PALETTELIST_SEGMENT__SEGMENT_COLOUR_04__ID,
      PALETTELIST_SEGMENT__SEGMENT_COLOUR_05__ID,

      PALETTELIST_SEGMENT__SEGMENT_COLOUR_LENGTH__ID
    };


    /**
     * @brief
     * CRGBPalette16 palettes generated from the segment colours.
     */
    enum PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__IDS
    {
      PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID = PALETTELIST_SEGMENT__SEGMENT_COLOUR_LENGTH__ID,
      PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_THREE_123__ID,
      PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_FOUR_1234__ID,
      PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_FIVE_12345__ID,
      PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_REPEATED_ACTIVE__ID,

      PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__LENGTH__ID
    };


    /**
     * @brief
     * Procedural palettes calculated directly from the requested palette index.
     * No palette data is stored or loaded.
     */
    enum PALETTELIST_PROCEDURAL__IDS
    {
      PALETTELIST_PROCEDURAL__COLOUR_WHEEL__ID = PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__LENGTH__ID,

      PALETTELIST_PROCEDURAL__LENGTH__ID
    };


    /**
     * @brief
     * Static indexed RGB palettes loaded into CRGBPalette16.
     *
     * This is now one unified group containing:
     * - Former FastLED fixed CRGBPalette16 palettes
     * - WLED / cpt-city indexed gradient palettes
     * - Additional PulSar/WLED-style gradients
     * - MATLAB colour maps
     *
     * All are stored as indexed byte gradients and use the same load path.
     */
    enum PALETTELIST_STATIC_CRGBPALETTE16__IDS
    {
      // 0-9
      PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID = PALETTELIST_PROCEDURAL__LENGTH__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_STRIPE_COLOUR__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__PARTY_COLOUR__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__CLOUD_COLOURS__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__LAVA_COLOURS__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__OCEAN_COLOUR__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__FOREST_COLOUR__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__SUNSET__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__RIVENDELL__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__OCEAN_BREEZE__ID,

      // 10-19
      PALETTELIST_STATIC_CRGBPALETTE16__RED_AND_BLUE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__YELLOWOUT__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__ANALOGOUS__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__PINK_SPLASH__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__SUNSET_YELLOW__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__ANOTHER_SUNSET__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__BEECH__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__VINTAGE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__DEPARTURE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__LANDSCAPE__ID,

      // 20-29
      PALETTELIST_STATIC_CRGBPALETTE16__BEACH__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_SHERBET__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__HULT__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__HULT64__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__DRYWET__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__JUL__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__GRINTAGE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__REWHI__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__TERTIARY__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__LAVA_FIRE__ID,

      // 30-39
      PALETTELIST_STATIC_CRGBPALETTE16__ICE_FIRE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__CYANE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__LIGHT_PINK__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__AUTUMN_HOT__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__BLUE_MAGENTA__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__BLACK_MAGENTA_RED__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__BLACK_RED_MAGENTA_YELLOW__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__BLUE_CYAN_YELLOW__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__ORANGE_TEAL__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__TIAMAT__ID,

      // 40-49
      PALETTELIST_STATIC_CRGBPALETTE16__APRIL_NIGHT__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__ORANGERY__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__SAKURA__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__AURORA__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__ATLANTICA__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__TEMPERATURE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__AURORA_2__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__RETRO_CLOWN__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__CANDY__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__TOXY_REAF__ID,

      // 50-59
      PALETTELIST_STATIC_CRGBPALETTE16__FAIRY_REAF__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__SEMI_BLUE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__PINK_CANDY__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__RED_REAF__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__AQUA_FRESH__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__YELLOW_BLUE_HOT__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__LITE_LIGHT__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__RED_FLASH__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__BLINK_RED__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__RED_SHIFT__ID,

      // 60-69
      PALETTELIST_STATIC_CRGBPALETTE16__CANDY_2__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__PINK_PURPLE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__PINK_WHITE_PURPLE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__COLOURMAP_PARULA__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__COLOURMAP_TURBO__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__COLOURMAP_HOT__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__COLOURMAP_COOL__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__COLOURMAP_SPRING__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__COLOURMAP_AUTUMN__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__COLOURMAP_JET__ID,

      PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID
    };


    /**
     * @brief
     * Instead of HTML colours, define some popular colours for easy switching
     * without using Custom Colour ##. Eg, Black for effects.
     */
    enum PALETTELIST_STATIC_SINGLE_COLOURS__IDS
    {
      PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID = PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__ORANGE__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__LIGHTORANGE__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__YELLOW__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__LIMEGREEN__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__GREEN__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__CYAN__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__BLUE__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__BLUEPURPLE__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__PURPLE__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__PINK__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__HOTPINK__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__WARMWHITE__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__COLDWHITE__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__BLACK__ID,

      PALETTELIST_STATIC_SINGLE_COLOUR__LENGTH__ID
    };


    ///// START of stored in vector

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    /**
     * @brief
     * Native PulSar static encoded palettes.
     *
     * Naming:
     *   PALETTELIST_STATIC__<UNIQUE_PALETTE_NAME>__ID
     *
     * The double underscore marks the beginning of the unique palette name.
     */
    enum PALETTELIST_STATIC__IDS
    {
      PALETTELIST_STATIC__COLOURFUL_DEFAULT__ID = PALETTELIST_STATIC_SINGLE_COLOUR__LENGTH__ID,
      PALETTELIST_STATIC__HOLLOWEEN_OP__ID,
      PALETTELIST_STATIC__HOLLOWEEN_OGP__ID,
      PALETTELIST_STATIC__HOT_PINK_NEON_WITH_NAVY__ID,

      PALETTELIST_STATIC__RAINBOW__ID,
      PALETTELIST_STATIC__RAINBOW_WARM__ID,
      PALETTELIST_STATIC__RAINBOW_INVERTED__ID,

      PALETTELIST_STATIC__PASTEL_01__ID,
      PALETTELIST_STATIC__PASTEL_02__ID,
      PALETTELIST_STATIC__PASTEL_03__ID,
      PALETTELIST_STATIC__PASTEL_04__ID,
      PALETTELIST_STATIC__PASTEL_05__ID,

      PALETTELIST_STATIC__WINTER_01__ID,
      PALETTELIST_STATIC__WINTER_02__ID,
      PALETTELIST_STATIC__WINTER_03__ID,
      PALETTELIST_STATIC__WINTER_04__ID,

      PALETTELIST_STATIC__AUTUMN_GREEN__ID,
      PALETTELIST_STATIC__AUTUMN_RED__ID,

      PALETTELIST_STATIC__GRADIENT_PASTEL_TONES_PURPLE__ID,
      PALETTELIST_STATIC__FLOWER_SWEATPEA__ID,
      PALETTELIST_STATIC__PINK_PURPLE__ID,
      PALETTELIST_STATIC__PURPLE_PINK__ID,

      PALETTELIST_STATIC__FESTIVE_TRADITIONAL_RGPBO__ID,
      PALETTELIST_STATIC__FESTIVE_TRADITIONAL_RGPBY__ID,
      PALETTELIST_STATIC__FESTIVE_TRADITIONAL_ROGPBY__ID,
      PALETTELIST_STATIC__FESTIVE_TRADITIONAL_RGBO__ID,
      PALETTELIST_STATIC__FESTIVE_TRADITIONAL_RGBY__ID,

      PALETTELIST_STATIC__FESTIVE_VINTAGE_MINIBELLS__ID,
      PALETTELIST_STATIC__FESTIVE_VINTAGE_MERRYLITES__ID,
      PALETTELIST_STATIC__FESTIVE_VINTAGE_AGED_BULBS__ID,

      PALETTELIST_STATIC__FESTIVE_SNOWY_COLOURS_01__ID,
      PALETTELIST_STATIC__FESTIVE_SNOWY_COLOURS_02__ID,
      PALETTELIST_STATIC__FESTIVE_SNOWY_COLOURS_03__ID,

      PALETTELIST_STATIC__FESTIVE_BERRY_YELLOW__ID,
      PALETTELIST_STATIC__FESTIVE_BERRY_ORANGE__ID,
      PALETTELIST_STATIC__FESTIVE_BERRY_GREEN__ID,

      PALETTELIST_STATIC__COLOURFUL_PAIRS_01__ID,
      PALETTELIST_STATIC__COLOURFUL_COLOUR_WHITE_STRIPE__ID,
      PALETTELIST_STATIC__COLOURFUL_COLOUR_WARMWHITE_STRIPE__ID,
      PALETTELIST_STATIC__COLOURFUL_BLUE_WHITE__ID,
      PALETTELIST_STATIC__COLOURFUL_WHITE_MIXED__ID,
      PALETTELIST_STATIC__COLOURFUL_PINK_RED_YELLOW__ID,
      PALETTELIST_STATIC__COLOURFUL_PEACHY_ORANGE__ID,
      PALETTELIST_STATIC__COLOURFUL_PEACHY_YELLOW__ID,
      PALETTELIST_STATIC__COLOURFUL_GREENLESS__ID,

      PALETTELIST_STATIC__GOLDEN__ID,

      PALETTELIST_STATIC__SUNRISE_01__ID,
      PALETTELIST_STATIC__SUNRISE_02__ID,
      PALETTELIST_STATIC__SUNRISE_03__ID,
      PALETTELIST_STATIC__SUNRISE_04__ID,

      PALETTELIST_STATIC__SUNSET_01__ID,
      PALETTELIST_STATIC__SUNSET_02__ID,
      PALETTELIST_STATIC__SUNSET_RED__ID,
      PALETTELIST_STATIC__SUNSET_BEACH__ID,

      PALETTELIST_STATIC__SKY_GLOW_01__ID,

      PALETTELIST_STATIC__COLOURFUL_WITH_CCT_01__ID,

      PALETTELIST_STATIC__CANDLE_FLAME_01__ID,
      PALETTELIST_STATIC__GRADIENT_FIRE_01__ID,
      PALETTELIST_STATIC__OCEAN_01__ID,

      PALETTELIST_STATIC__LENGTH__ID
    };


    /**
     * @brief
     * Dynamic CRGBPalette16 palettes generated at runtime.
     */
    enum PALETTELIST_DYNAMIC__COLOUR_CRGBPALETTE__IDS
    {
      /****
       *
       * Lets rework these
       * 1) Keep (100% saturation, random hue)
       * 2) Washed out (always pastels)
       * 3) Hues, and pastels (can I force at least one to be pastel?)
       * 4) Hues, and pastels, but also allow wide swings in brightness
       * 5) Have start tied to off, middle mid bightness, end to full brightness
       *    (so gradient of rising colour, think how it would look on the tree)
       */

      PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__RANDOMISE_COLOURS_01__ID = PALETTELIST_STATIC__LENGTH__ID,
      PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__RANDOMISE_COLOURS_02__ID,
      PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__RANDOMISE_COLOURS_03__ID,
      PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__RANDOMISE_COLOURS_04__ID,
      PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__RANDOMISE_COLOURS_05__ID,

      PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__LENGTH__ID
    };


    /**
     * @brief
     * Dynamic/contextual palettes generated from external state.
     *
     * SolarPalettes
     * * "White colour temp = day range" cold/warm white as max elev, and sunset=0deg
     * * "White colour temp = day range" cold/warm white above 10deg, and sunset=0deg
     * * "White colour temp = twilight range" cold/warm white max day, warm at dusk/dawn
     * * SegColor 1/2 is "Solar SegColour RiseSet"
     *
     * * Solid Sun White Warm (warm/cold change from night/day)
     * * Solid Sun Seg Colour (seg1/seg2 change from night/day)
     *
     * Any palette shifters of sun positions are handled by effects.
     * Azimuth currently has no use.
     */
    #define PALETTELIST_DYNAMIC__COLOUR__ID_START PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__LENGTH__ID

    enum PALETTELIST_DYNAMIC__COLOUR__IDS
    {
      PALETTELIST_DYNAMIC__SOLAR_ELEVATION__WHITE_COLOUR_TEMPERATURE_01__ID = PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__LENGTH__ID,

      PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SEGMENT_COLOUR_BLEND_DAYTIME_01__ID,
      PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SEGMENT_COLOUR_BLEND_DAWNDUSKTIME_01__ID,
      PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SEGMENT_COLOUR_BLEND_NIGHTTIME_01__ID,

      PALETTELIST_DYNAMIC__TIMEREACTIVE__SEGMENT_COLOUR__MINUTE_BLEND__ID,
      PALETTELIST_DYNAMIC__TIMEREACTIVE__SEGMENT_COLOUR__HOUR_BLEND__ID,

      PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SOLID_COLOUR_OF_SKY__ID,
      PALETTELIST_DYNAMIC__SOLAR_ELEVATION__GRADIENT_COLOUR_OF_SKY__ID,

      PALETTELIST_DYNAMIC__ELAPSEDTIME_PALIX__SEGCOLOUR_CYCLE_IMMEDIATE_01__ID,
      PALETTELIST_DYNAMIC__ELAPSEDTIME_PALIX__SEGCOLOUR_CYCLE_BLENDING_02__ID,

      PALETTELIST_DYNAMIC__LENGTH__ID
    };


    /****************************************************************************************************************************************
     *****************************************************************************************************************************************
    * @brief Palette range limits
    ****************************************************************************************************************************************
    *****************************************************************************************************************************************/

    #define PALETTELIST_LENGTH_OF_STATIC_IDS PALETTELIST_STATIC__LENGTH__ID

    #define PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED PALETTELIST_DYNAMIC__LENGTH__ID

    #define MAX_USER_DEFINED_ENCODED_PALETTES 10
    uint8_t user_defined_palette_count = 10;

    uint16_t GetPaletteListLength(){ return PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED + user_defined_palette_count; }

    bool IsPaletteGradient(uint16_t palette_id);

    /**
     * @brief IMPORTANT
     * 
     * The bit order here is CRITICAL, as it aligns with the DEFINES used in the header.
     * 
     * Reorder the defines when new bit usage is decided
     * 
     */
    typedef union {
      uint16_t data; // allows full manipulating
      struct { 
        uint16_t red_enabled                      : 1; // bit 15
        uint16_t green_enabled                    : 1; // bit 14
        uint16_t blue_enabled                     : 1; // bit 13
        uint16_t white_warm_enabled               : 1; // bit 12
        uint16_t white_cold_enabled               : 1; // bit 11       
        uint16_t encoded_value_byte_width         : 3; // bit 10-8 (3 bits wide, 9 value options)
        uint16_t gamma_skip_send_raw              : 1; // bit 7 Pulsar palettes by default are saved as they are intended to reach the pixels without correction        
        uint16_t index_gradient                   : 1; // bit 6 // To rename, again, "index_gradient" worded as effect style, whereas it should simply be "index_gradient" // Rename from "index_ scaled_to_segment" to "index_gradient"
        uint16_t index_is_trigger_value_exact     : 1; // bit 5
        uint16_t reserved1                        : 1; // bit 4 UNUSED
        uint16_t reserved2                        : 1; // bit 3 UNUSED
        uint16_t encoded_as_crgb_palette_16       : 1; // bit 2
        uint16_t encoded_as_crgb_palette_256      : 1; // bit 1
        uint16_t palette_can_be_modified          : 1; // bit 0  - probably remove.       
      };
    } PALETTE_ENCODING_DATA;

    /**
     * Palette Data Structure
     **/
    struct PALETTE_DATA{ // 6 bytes per palette
      uint16_t palettelist_id;      
      std::vector<uint8_t> data;       // colour bytes       
      uint8_t number_of_colours;       // Moving away from colour_width to reduce calculation per pixel      
      PALETTE_ENCODING_DATA encoding;  // Contains information on formatting of data buffer
    };

    std::vector<PALETTE_DATA> static_palettes;
    std::vector<PALETTE_DATA> dynamic_palettes;
    std::vector<PALETTE_DATA> custom_palettes;

    void addStaticPalette (uint16_t id, const uint8_t* data, const uint8_t length, uint16_t encoding);
    void addCustomPalette (uint16_t id, const uint8_t* data, const uint8_t length, uint16_t encoding);
    void addDynamicPalette(uint16_t id, const uint8_t* data, const uint8_t length, uint16_t encoding);

    uint8_t GetEncodedColourWidth( PALETTE_ENCODING_DATA encoded );
    uint8_t GetColoursInPalette(uint16_t palette_id);
    PALETTE_ENCODING_DATA findPaletteEncoding(uint16_t id);

    // --- sequence tracking state for discrete + 0..255 input ---
    uint16_t tracked_previous_palette_index = 0; // current slot cursor 0..(N-1)
    uint8_t  tracked_prev_v                = 0; // last 0..255 input
    uint8_t  tracked_frac                  = 0; // fractional accumulator (Bresenham-style)


     [[gnu::hot]] static uint32_t ColorFromPalette16(const CRGBPalette16 &pal, unsigned index, uint8_t brightness = (uint8_t)255U, TBlendType blendType = LINEARBLEND);

    #ifdef ENABLE_FEATURE_PALETTE__RGBWW_COLOURS
    
    uint8_t colour32_white_cold = 0; // R,G,B, W1, then W2 is temp per function call below, to allow one function does both
    IRAM_ATTR [[gnu::hot]] RgbwwColor     GetColourFromPreloadedPaletteBuffer_RGBWW
    (
      uint16_t id = 0,
      // Pass preloaded palette data buffer. If nullptr, and "id" does not match any preloaded palette, then it will force a reload of the palette data.
      uint8_t* data = nullptr,
      // In discrete mode, this index will automatically modulo and repeat the palette over infinite length (MAXU16). In Gradient mode, must be scaled in 0 to 255 range.
      uint16_t desired_index = 0,
      // If the palette is encoded, then this returns encoded value at [desired_index] point. NOTE: Only in discrete mode.
      uint8_t* encoded_index = nullptr,
      // Providing the index in range 0 to 255, this enabled will internally rescale the index to the segment length, so that the index is always in range 0 to segment_length-1
      uint8_t  palette_index__format = 1,
      // CRGBPalette defaults gradient (index 240 to 255) wraps to blend with colour as index 0. This rescales to limit to 240, hence, removes wrap around blending.
      uint8_t  rescale_index_wrap_for_hardedge = 1,
      // 0 = default, 1 = "Forced Discrete", 2 = "Forced Gradient"
      uint8_t  force_palette_mode = 0, // flag_crgb_exact_colour = 0, // true: "CRGB exact colour", false: "U32 colour"
      // Requesting preview: Live palettes must respond with preview for UI
      bool flag_request_is_for_full_visual_output = false
    );

    // includes the same args are main function, so should not really exist? is this a subfunction, if so needs named that way
    IRAM_ATTR [[gnu::hot]] RgbwwColor      SubGet_Encoded_Palette_Colour_RGBWW
    (
      uint8_t* palette_elements = nullptr,
      uint16_t desired_index_from_palette = 0,
      uint8_t encoded_colour_width = 0,
      uint8_t colours_in_palette = 0,
      PALETTE_ENCODING_DATA encoding = {0},
      uint8_t* encoded_index = nullptr,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
      bool     palette_index__format = 1, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
      bool     flag_wrap_hard_edge = false,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
      uint8_t  force_palette_mode = false,
      bool     flag_forced_gradient = false
    );
    
    /**
     * HOT PATH – FORCE INLINE
     *
     * This function is intentionally defined `static inline` in the header and
     * marked `always_inline` so the compiler can:
     *   - Inline it at the call site (no call/return overhead)
     *   - Eliminate repeated parameter passing in tight inner loops
     *   - Enable constant-propagation of encoding flags
     *
     * This sits on the deepest palette read path and may be executed
     * per-pixel, per-frame. Do NOT move to a .cpp unless performance
     * has been re-verified.
     */
    static inline __attribute__((always_inline)) IRAM_ATTR
    RgbwwColor mPalette::SubGet_Encoded_Colour_ReadBuffer_RGBWW(
      const uint8_t* __restrict palette_buffer,
      uint16_t pixel_position,
      uint8_t* __restrict return_encoded_value,
      const PALETTE_ENCODING_DATA encoding,
      uint8_t encoded_colour_width
    ) {
      // Base byte index into packed palette
      uint16_t idx = (uint16_t)(pixel_position * encoded_colour_width);

      // Optional gradient byte at start of entry
      if (encoding.index_gradient) {
        if (return_encoded_value) {
          *return_encoded_value = palette_buffer[idx];
        }
        ++idx;
      }

      // Read components (only touch bytes that exist for this encoding)
      const uint8_t r  = encoding.red_enabled        ? palette_buffer[idx + 0] : 0;
      const uint8_t g  = encoding.green_enabled      ? palette_buffer[idx + 1] : 0;
      const uint8_t b  = encoding.blue_enabled       ? palette_buffer[idx + 2] : 0;
      const uint8_t wc = encoding.white_cold_enabled ? palette_buffer[idx + 3] : 0;
      const uint8_t ww = encoding.white_warm_enabled ? palette_buffer[idx + 4] : 0;

      return RgbwwColor(r, g, b, wc, ww);
    }

    #else
    IRAM_ATTR [[gnu::hot]] uint32_t      SubGet_Encoded_Palette_Colour_U32
    (
      uint8_t* palette_elements = nullptr,
      uint16_t desired_index_from_palette = 0,
      uint8_t encoded_colour_width = 0,
      uint8_t colours_in_palette = 0,
      PALETTE_ENCODING_DATA encoding = {0},
      uint8_t* encoded_index = nullptr,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
      uint8_t  palette_index__format = 1, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
      bool     flag_wrap_hard_edge = false,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
      uint8_t  force_palette_mode = false,
      bool     flag_forced_gradient = false
    );

    /**
     * HOT PATH – FORCE INLINE
     *
     * This function is intentionally defined `static inline` in the header and
     * marked `always_inline` so the compiler can:
     *   - Inline it at the call site (no call/return overhead)
     *   - Eliminate repeated parameter passing in tight inner loops
     *   - Enable constant-propagation of encoding flags
     *
     * This sits on the deepest palette read path and may be executed
     * per-pixel, per-frame. Do NOT move to a .cpp unless performance
     * has been re-verified.
     */
    static inline __attribute__((always_inline)) IRAM_ATTR
    uint32_t SubGet_Encoded_Colour_ReadBuffer_U32(
      const uint8_t* __restrict palette_buffer,
      uint16_t pixel_position,
      uint8_t* __restrict return_encoded_value,
      const PALETTE_ENCODING_DATA encoding,
      uint8_t encoded_colour_width
    ) {
      uint16_t idx = (uint16_t)(pixel_position * encoded_colour_width);

      if (encoding.index_gradient) {
        if (return_encoded_value) *return_encoded_value = palette_buffer[idx];
        idx++;
      }

      const uint8_t r  = encoding.red_enabled        ? palette_buffer[idx + 0] : 0;
      const uint8_t g  = encoding.green_enabled      ? palette_buffer[idx + 1] : 0;
      const uint8_t b  = encoding.blue_enabled       ? palette_buffer[idx + 2] : 0;
      const uint8_t wc = encoding.white_cold_enabled ? palette_buffer[idx + 3] : 0;

      return RGBW32(r, g, b, wc);
    }
    #endif
    
    // A wrapper can be used to the calls below work as is. The internals of both of these will use ifdefs to block them when not needed.
    IRAM_ATTR [[gnu::hot]] uint32_t       GetColourFromPreloadedPaletteBuffer_U32
    (
      uint16_t id = 0,
      // Pass preloaded palette data buffer. If nullptr, and "id" does not match any preloaded palette, then it will force a reload of the palette data.
      uint8_t* data = nullptr,
      // In discrete mode, this index will automatically modulo and repeat the palette over infinite length (MAXU16). In Gradient mode, must be scaled in 0 to 255 range.
      uint16_t desired_index = 0,
      // If the palette is encoded, then this returns encoded value at [desired_index] point. NOTE: Only in discrete mode.
      uint8_t* encoded_index = nullptr,
      // Providing the index in range 0 to 255, this enabled will internally rescale the index to the segment length, so that the index is always in range 0 to segment_length-1
      uint8_t  palette_index__format = 1,
      // CRGBPalette defaults gradient (index 240 to 255) wraps to blend with colour as index 0. This rescales to limit to 240, hence, removes wrap around blending.
      uint8_t  rescale_index_wrap_for_hardedge = 1,
      // 0 = default, 1 = "Forced Discrete", 2 = "Forced Gradient"
      uint8_t  force_palette_mode = 0, // flag_crgb_exact_colour = 0, // true: "CRGB exact colour", false: "U32 colour"
      // Requesting preview: Live palettes must respond with preview for UI
      bool flag_request_is_for_full_visual_output = false
    );

};

#define mPaletteI mPalette::GetInstance() // lets investigate making mPalette NOT a singleton, though, to be included inside palette controller, it might need to be so it only has once instance

#endif

#endif // _M_PALETTE_H
