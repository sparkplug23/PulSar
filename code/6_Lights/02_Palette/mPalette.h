#ifndef _M_PALETTE_H
#define _M_PALETTE_H

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#include "6_Lights/02_Palette/mPalette_Progmem.h"
#include "2_CoreSystem/11_Languages/mLanguageProgmem.h"
#include "1_TaskerManager/mTaskerManager.h"

#define FASTLED_INTERNAL // suppress pragma warning messages
#include "6_Lights/00_Interface/FastLED/FastLED.h"

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

    void init_PresetColourPalettes(); 

    /**************
     * COLOUR Designs by name
     * PALETTE are different colours by pixel
    **************/                            
    int16_t Get_Static_PaletteIDbyName(const char* c);
    const char* GetPaletteNameByID(uint8_t id, char* buffer, uint8_t buflen);

    /************************************************************************************************************************************
     * ************************************************************************************************************************************
     * @brief  Static palettes read from inside this class
     ************************************************************************************************************************************
     ***************************************************************************************************************************************/
   
    /**
     * @brief Only the static/progmem stuff below will be taken from inside palette.cpp
     **/
    enum PALETTELIST_FIXED_CRGBPALETTE16__IDS{
      
      PALETTELIST_FIXED_CRGBPALETTE16__RAINBOW_COLOUR__ID = 0,
      PALETTELIST_FIXED_CRGBPALETTE16__CLOUD_COLOURS__ID,
      PALETTELIST_FIXED_CRGBPALETTE16__LAVA_COLOURS__ID,
      PALETTELIST_FIXED_CRGBPALETTE16__OCEAN_COLOUR__ID,
      PALETTELIST_FIXED_CRGBPALETTE16__FOREST_COLOUR__ID,
      PALETTELIST_FIXED_CRGBPALETTE16__RAINBOW_STRIPE_COLOUR__ID,
      PALETTELIST_FIXED_CRGBPALETTE16__PARTY_COLOUR__ID,
      PALETTELIST_FIXED_CRGBPALETTE16__HEAT_COLOUR__ID,
      PALETTELIST_FIXED_CRGBPALETTE16__LENGTH__ID
    };

    enum PALETTELIST_CRGBPALETTE16_GRADIENT___PALETTES__IDS
    { //39 of them 
      // 0-9
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__SUNSET__ID = PALETTELIST_FIXED_CRGBPALETTE16__LENGTH__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__RIVENDELL__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__OCEAN_BREEZE__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__RED_AND_BLUE__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__YELLOWOUT__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__ANALOGOUS__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__PINK_SPLASH__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__SUNSET_YELLOW__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__ANOTHER__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__BEECH__ID,
      // 10-19
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__VINTAGE__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__DEPARTURE__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__LANDSCAPE__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__BEACH__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__RAINBOW_SHERBET__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__HULT__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__HULT64__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__DRYWET__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__JUL__ID,
      // 20-29
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__GRINTAGE__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__REWHI__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__LAVA_FIRE__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__ICE_FIRE__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__CYANE__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__LIGHT_PINK__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__AUTUMN__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__BLUE_MAGENTA__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__BLACK_MAGENTA_RED__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__BLACK_RED_MAGENTA_YELLOW__ID,
      // 30-39
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__BLUE_CYAN_YELLOW__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__ORANGE_TEAL__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__TIAMAT__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__APRIL_NIGHT__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__ORANGERY__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__C9__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__SAKURA__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__AURORA__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__ATLANTICA__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__C9_TWO__ID,
      // 
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__C9_NEW__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__TEMPERATURE__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__AURORA_2__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__RETRO_CLOWN__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__CANDY__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__TOXY_REAF__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__FAIRY_REAF__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__SEMI_BLUE__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__PINK_CANDY__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__RED_REAF__ID,
      // 
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__AQUA_FRESH__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__YELLOW_BLUE_HOT__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__LITE_LIGHT__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__RED_FLASH__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__BLINK_RED__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__RED_SHIFT__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__RED_TIDE__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__CANDY_2__ID,
      //
      PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT_LENGTH__ID
    };

    /**
     * @brief These below to become fully added into my methods as another encoding type
     * to be merged and named in my static list
     * 
     * 
     * These need to be added into my palette method at least by name only, data can be ignored
     * Or, the data for CRGB16 is saved in the data field and then cast back to use?? This would reduce the need to keep 
     * a static version of it. OR, simply do the math in C16 but convert back? or, do equivalent of these with mine.
     */
    enum PALETTELIST_SEGMENTS_STORED_STATIC_CRGBPALETTE16_PALETTES__IDS{    // Not stored in memory, but changes when called, maybe grow with segment size, but not stored there? save as encoded?
      PALETTELIST_FIXED_CRGBPALETTE16__RANDOMISE_COLOURS_01__ID = PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT_LENGTH__ID,
      
      PALETTELIST_FIXED_CRGBPALETTE16__RANDOMISE_COLOURS_02__ID,
      PALETTELIST_FIXED_CRGBPALETTE16__RANDOMISE_COLOURS_03__ID,
      PALETTELIST_FIXED_CRGBPALETTE16__RANDOMISE_COLOURS_04__ID,
      PALETTELIST_FIXED_CRGBPALETTE16__RANDOMISE_COLOURS_05__ID,

      PALETTELIST_FIXED_CRGBPALETTE16__BASIC_COLOURS_PRIMARY__ID,
      PALETTELIST_FIXED_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY__ID,
      PALETTELIST_FIXED_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY_TERTIARY__ID,
      PALETTELIST_FIXED_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY_TERTIARY_REPEATED__ID,
      PALETTELIST_FIXED_CRGBPALETTE16_USER__LENGTH__ID    
    };


///// START of stored in vector

    enum PALETTELIST_FIXED__IDS
    {
      PALETTELIST_FIXED_COLOURFUL_DEFAULT__ID = PALETTELIST_FIXED_CRGBPALETTE16_USER__LENGTH__ID,
      PALETTELIST_FIXED_HOLLOWEEN_OP__ID,
      PALETTELIST_FIXED_HOLLOWEEN_OGP__ID,
      PALETTELIST_FIXED_HOT_PINK_NEON_WITH_NAVY__ID,
      PALETTELIST_FIXED_RAINBOW__ID,
      PALETTELIST_FIXED_RAINBOW_INVERTED__ID,
      PALETTELIST_FIXED_PASTEL_01__ID,
      PALETTELIST_FIXED_PASTEL_02__ID,
      PALETTELIST_FIXED_PASTEL_03__ID,
      PALETTELIST_FIXED_PASTEL_04__ID,
      PALETTELIST_FIXED_PASTEL_05__ID,
      PALETTELIST_FIXED_WINTER_01__ID,
      PALETTELIST_FIXED_WINTER_02__ID,
      PALETTELIST_FIXED_WINTER_03__ID,
      PALETTELIST_FIXED_WINTER_04__ID,
      PALETTELIST_FIXED_AUTUMN_GREEN__ID,
      PALETTELIST_FIXED_AUTUMN_RED__ID,
      PALETTELIST_FIXED_GRADIENT_01__ID,
      PALETTELIST_FIXED_GRADIENT_02__ID,
      PALETTELIST_FIXED_GRADIENT_PASTEL_TONES_PURPLE__ID,
      PALETTELIST_FIXED_BERRY_GREEN__ID,
      PALETTELIST_FIXED_CHRISTMAS_01__ID,
      PALETTELIST_FIXED_CHRISTMAS_02__ID,
      PALETTELIST_FIXED_CHRISTMAS_03__ID,
      PALETTELIST_FIXED_CHRISTMAS_04__ID,
      PALETTELIST_FIXED_CHRISTMAS_05__ID,
      PALETTELIST_FIXED_CHRISTMAS_06__ID,
      PALETTELIST_FIXED_CHRISTMAS_07__ID,
      PALETTELIST_FIXED_CHRISTMAS_08__ID,
      PALETTELIST_FIXED_CHRISTMAS_09__ID,
      PALETTELIST_FIXED_CHRISTMAS_10__ID,
      PALETTELIST_FIXED_CHRISTMAS_11__ID,
      PALETTELIST_FIXED_CHRISTMAS_12__ID,
      PALETTELIST_FIXED_CHRISTMAS_13__ID,
      PALETTELIST_FIXED_CHRISTMAS_14__ID,
      PALETTELIST_FIXED_CHRISTMAS_15__ID,
      PALETTELIST_FIXED_CHRISTMAS_16__ID,
      PALETTELIST_FIXED_CHRISTMAS_17__ID,
      PALETTELIST_FIXED_CHRISTMAS_18__ID,
      PALETTELIST_FIXED_CHRISTMAS_19__ID,
      PALETTELIST_FIXED_CHRISTMAS_20__ID,
      PALETTELIST_FIXED_CHRISTMAS_21__ID,
      PALETTELIST_FIXED_CHRISTMAS_22__ID,
      PALETTELIST_FIXED_CHRISTMAS_23__ID,
      PALETTELIST_FIXED_CHRISTMAS_24__ID,
      PALETTELIST_FIXED_CHRISTMAS_25__ID,
      PALETTELIST_FIXED_CHRISTMAS_26__ID,
      PALETTELIST_FIXED_CHRISTMAS_27__ID,
      PALETTELIST_FIXED_CHRISTMAS_28__ID,
      PALETTELIST_FIXED_CHRISTMAS_29__ID,
      PALETTELIST_FIXED_CHRISTMAS_30__ID,
      PALETTELIST_FIXED_SUNRISE_01__ID,
      PALETTELIST_FIXED_SUNRISE_02__ID,
      PALETTELIST_FIXED_SUNRISE_03__ID,
      PALETTELIST_FIXED_SUNRISE_04__ID,
      PALETTELIST_FIXED_SUNSET_01__ID,
      PALETTELIST_FIXED_SUNSET_02__ID,
      PALETTELIST_FIXED_CANDLE_FLAME_01__ID,
      PALETTELIST_FIXED_GRADIENT_FIRE_01__ID,
      PALETTELIST_FIXED_SKY_GLOW_01__ID,
      /**
       * GRADIENT_SUNLEVEL_GROUP01 - RGBCCT, Gradient
       *       like grad 1, order of using these will produce sunrising/falling
       *       linear gradient sunlevel below (dark with no sun point above horizon)
       *       linear gradient sunlevel low (dark red center, orange sides)
       *       linear gradient sunlevel med (light red/orange center, orange, then yellow sides, pale blue on farest edges)
       *       linear gradient sunlevel high (orange sun center, yellow slightly, largely light blue)
       *       linear gradient sunlevel highest/midday (orange sun center, white edges... pale blue rest)
       * */
      //add below horizon gradients (5 below), for black/blue
      PALETTELIST_FIXED_GRADIENT_SUNLEVEL_GROUP01_01__ID,
      PALETTELIST_FIXED_GRADIENT_SUNLEVEL_GROUP01_02__ID,
      PALETTELIST_FIXED_GRADIENT_SUNLEVEL_GROUP01_03__ID,
      PALETTELIST_FIXED_GRADIENT_SUNLEVEL_GROUP01_04__ID,
      PALETTELIST_FIXED_GRADIENT_SUNLEVEL_GROUP01_05__ID,
      PALETTELIST_FIXED_GRADIENT_SUNLEVEL_GROUP01_06__ID,
      PALETTELIST_FIXED_GRADIENT_SUNLEVEL_GROUP01_07__ID,

      PALETTELIST_FIXED_OCEAN_01__ID,
      PALETTELIST_FIXED_CUSTOM_USER_01__ID,
      PALETTELIST_FIXED_SINGLE_FIRE_01__ID,

      /**
       * Patterns for Gazebo
       * */
      PALETTELIST_FIXED_FLOWER_SWEATPEAS_01__ID,
      PALETTELIST_FIXED_FLOWER_SWEATPEAS_02__ID,
      PALETTELIST_FIXED_PINK_PURPLE_01__ID,
      PALETTELIST_FIXED_PINK_PURPLE_02__ID,
      PALETTELIST_FIXED_PINK_PURPLE_03__ID,

      /**
       * Create a few palettes that are updated/calculated using sun/azimuth
       * eg: 1) ambilight colours that are calculated to show sun brightness up top
       * */

      /**
       * Specialised palettes: Solid single colour
       * */
      PALETTELIST_FIXED__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01__ID,
      PALETTELIST_FIXED__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_02__ID,
      PALETTELIST_FIXED__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_03__ID,
      PALETTELIST_FIXED__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_04__ID,
      /**
       * Rgbcct colour pairs: For ambilight top/bottom arrangement
       * */
      PALETTELIST_FIXED_DUAL_COLOUR_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01__ID,      

      // Count of total handlers and starting point for other modules
      PALETTELIST_FIXED_LENGTH__ID 
    };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Single colour user options, up to all 5 rgbcct elements
    // Single RGBCCT above where they are then used internally to make new palettes
    /**
     * @brief Notes
     * 
     * Internally should be stored here as user changable palettes should be at the end of the list
     * On the WebUI, it should be sorted to the top.
     * 
     */
    enum PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR__IDS{
      PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_01__ID = PALETTELIST_FIXED_LENGTH__ID, // New scene colour, static
      PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_02__ID,
      PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_03__ID,
      PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_04__ID,
      PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_05__ID,
      PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID
    };




    // enum PALETTELIST_VARIABLE_ENCODED__IDS{
    //   PALETTELIST_VARIABLE_ENCODED_01__ID = PALETTELIST_VARIABLE_HSBID_LENGTH__ID, // New scene colour, static
    //   // Similar to WLED, another value should be used to remember user defined ones and use the getpalettelength to get the size
    //   PALETTELIST_VARIABLE_ENCODED_01__ID

    // };


///// END of stored in vector

    /****************************************************************************************************************************************
     *****************************************************************************************************************************************
      @brief 
     * @NOTE: Only palettes below this are fixed in memory (WLED types)
     ****************************************************************************************************************************************
     *****************************************************************************************************************************************/
    #define PALETTELIST_LENGTH_OF_STATIC_IDS  PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID
    #define MAX_USER_DEFINED_ENCODED_PALETTES 10
    uint8_t user_defined_palette_count = 10;

    uint16_t GetPaletteListLength(){ return PALETTELIST_LENGTH_OF_STATIC_IDS + user_defined_palette_count; }

    typedef union {
      uint16_t data; // allows full manipulating
      struct { 
        uint16_t red_enabled : 1;
        uint16_t green_enabled : 1;
        uint16_t blue_enabled : 1;
        uint16_t white_warm_enabled : 1;

        uint16_t white_cold_enabled : 1;        
        /**
         * @brief Index can take up to three bytes
         **/
        uint16_t encoded_value_byte_width : 3; // 3 bits wide, or 0b000 gives 9 value options

        uint16_t index_exact : 1;                       // Specialised, maybe also could be removed as not useful. The effect itself should treat this index as special
        uint16_t index_scaled_to_segment : 1;           // To rename, again, "index_scaled_to_segment" worded as effect style, whereas it should simply be "index_gradient"
        uint16_t index_is_trigger_value_exact : 1;
        uint16_t index_is_trigger_value_scaled100 : 1;  //probably remove this, why bother having 100% when 0-255 is the same

        uint16_t encoded_as_hsb_ids : 1; //move this to other encoded types   //deleting!!!!
        uint16_t encoded_as_crgb_palette_16 : 1;
        uint16_t encoded_as_crgb_palette_256 : 1;
        uint16_t palette_can_be_modified : 1;
      };
    } PALETTE_ENCODING_DATA;

    struct STATIC_PALETTE{ //6 bytes per palette
      // enum: Used to call for ctr of friendly name
      uint8_t id;
      // Pointer to name
      char* friendly_name_ctr;
      // colour bytes
      uint8_t* data; // pointer to progmem
      // Active pixels
      uint8_t  data_length; // total bytes in palette (includes index information)
      // Contains information on formatting of data buffer
      PALETTE_ENCODING_DATA encoding;
    };
    std::vector<STATIC_PALETTE> palettelist;

    struct CUSTOM_PALETTE{
      std::vector<uint8_t> data;
      PALETTE_ENCODING_DATA encoding;
    };
    std::vector<CUSTOM_PALETTE> custom_palettes;

          
    uint8_t GetColourMapSizeByPaletteID(uint8_t palette_id);
    uint16_t GetNumberOfColoursInPalette(uint16_t palette_id, uint8_t pixel_width_contrained_limit = 0); 
    uint8_t GetEncodedColourWidth( PALETTE_ENCODING_DATA encoded );
    uint16_t GetNumberOfColoursFromEncoded(PALETTE_ENCODING_DATA encoding, uint8_t data_in_palette);


    RgbcctColor 
    #ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
    IRAM_ATTR 
    #endif 
    GetColourFromPreloadedPalette(
      uint16_t palette_id = 0,
      uint16_t desired_index_from_palette = 0,
      uint8_t* encoded_index = nullptr,
      bool     flag_map_scaling = true, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
      bool     flag_wrap = true,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
      bool     flag_convert_pixel_index_to_get_exact_crgb_colour = false,   // added by me, to make my effects work with CRGBPalette16
      uint8_t  brightness_scale = 255//, //255(default): No scaling, 0-255 scales the brightness of returned colour (remember all colours are saved in full 255 scale)
    );


    RgbcctColor 
    #ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
    IRAM_ATTR 
    #endif 
    GetColourFromPreloadedPaletteBuffer(
      uint16_t palette_id = 0,
      uint8_t* palette_elements = nullptr,
      uint16_t desired_index_from_palette = 0,
      uint8_t* encoded_index = nullptr,
      bool     flag_map_scaling = true, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
      bool     flag_wrap = true,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
      bool     flag_convert_pixel_index_to_get_exact_crgb_colour = false,   // added by me, to make my effects work with CRGBPalette16
      uint8_t  brightness_scale = 255 // 255 No scaling, 0-255 scales the brightness of returned colour (remember all colours are saved in full 255 scale)
    );


    RgbcctColor SubGet_Encoded_PaletteList_Colour(
      uint16_t palette_id = 0,
      uint8_t* palette_elements = nullptr,
      uint16_t desired_index_from_palette = 0,
      uint8_t* encoded_index = nullptr,
      bool     flag_map_scaling = true, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
      bool     flag_wrap = true,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
      bool     flag_convert_pixel_index_to_get_exact_crgb_colour = false,   // added by me, to make my effects work with CRGBPalette16
      uint8_t  brightness_scale = 255 // 255 No scaling, 0-255 scales the brightness of returned colour (remember all colours are saved in full 255 scale)
    );


    RgbcctColor SubGet_Encoded_UserPalette_Colour(
      uint16_t palette_id = 0,
      uint8_t* palette_elements = nullptr,
      uint16_t desired_index_from_palette = 0,
      uint8_t* encoded_index = nullptr,
      bool     flag_map_scaling = true, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
      bool     flag_wrap = true,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
      bool     flag_convert_pixel_index_to_get_exact_crgb_colour = false,   // added by me, to make my effects work with CRGBPalette16
      uint8_t  brightness_scale = 255 // 255 No scaling, 0-255 scales the brightness of returned colour (remember all colours are saved in full 255 scale)
    );


    RgbcctColor SubGet_Encoded_PaletteList_Colour_WithoutScaling(
      uint16_t palette_id = 0,
      uint8_t* palette_elements = nullptr,
      uint16_t desired_index_from_palette = 0,
      uint8_t* encoded_index = nullptr
    );


    RgbcctColor SubGet_Encoded_PaletteList_Colour_Gradient(
      uint16_t palette_id = 0,
      uint8_t* palette_elements = nullptr,
      uint16_t desired_index_from_palette = 0,
      uint8_t* encoded_index = nullptr,
      bool     flag_map_scaling = true, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
      bool     flag_wrap = true,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
      bool     flag_convert_pixel_index_to_get_exact_crgb_colour = false,   // added by me, to make my effects work with CRGBPalette16
      uint8_t  brightness_scale = 255 // 255 No scaling, 0-255 scales the brightness of returned colour (remember all colours are saved in full 255 scale)
    );
    
};

#define mPaletteI mPalette::GetInstance() // lets investigate making mPalette NOT a singleton, though, to be included inside palette controller, it might need to be so it only has once instance

#endif

#endif // _M_PALETTE_H


