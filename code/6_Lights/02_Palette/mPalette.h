#ifndef _M_PALETTE_H
#define _M_PALETTE_H 0.1

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE

#define CLEAR_PALETTELIST_WITH_PTR(x) memset(x,0,sizeof(PALETTELIST::PALETTE));


//https://www.bhencke.com/pixelblaze
//Add showing the entire palette example, beside the name
/**
 * 
Change option to make all palettes gradient mode or node, or how to use it
1) make palette gradient using provided value, or make them equal distant
2) ignore palette gradient if one provided, and I dont want it
*/

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#include "6_Lights/02_Palette/mPalette_Progmem.h"

#include "2_CoreSystem/11_Languages/mLanguageProgmem.h"
#include "1_TaskerManager/mTaskerManager.h"

#define FASTLED_INTERNAL // suppress pragma warning messages
#include "6_Lights/98_FastLED_Modified/FastLED.h"

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

    bool flag_started = false;

    uint8_t test_value = 123;


    void init_PresetColourPalettes(); 
    // First bytes stores length 
    void init_ColourPalettes_Party_Default();
    void init_ColourPalettes_Autumn_Red();
    void init_ColourPalettes_Autumn_Green();
    void init_ColourPalettes_Winter();
    void init_ColourPalettes_Pastel_01();
    void init_ColourPalettes_Pastel_02();
    void init_ColourPalettes_Ocean_01();
    void init_ColourPalettes_Rainbow();
    void init_ColourPalettes_RainbowInverted();
    void init_ColourPalettes_Holloween_OP();
    void init_ColourPalettes_Holloween_OGP();
    void init_ColourPalettes_Hot_Neon_Pink_With_Navy();
    void init_ColourPalettes_Flower_SweetPeas_01();
    void init_ColourPalettes_Flower_SweetPeas_02();
    void init_ColourPalettes_Pink_Purple_01();
    void init_ColourPalettes_Pink_Purple_02();
    void init_ColourPalettes_Pink_Purple_03();
    void init_ColourPalettes_Single_Fire_01();
    void init_ColourPalettes_Gradient_01();
    void init_ColourPalettes_Gradient_02();
    void init_ColourPalettes_Gradient_Fire_01();
    void init_ColourPalettes_Gradient_Pastel_Tones_Purple();
    void init_ColourPalettes_Berry_Green();
    void init_ColourPalettes_Christmas_01();
    void init_ColourPalettes_Christmas_02();
    void init_ColourPalettes_Christmas_03();
    void init_ColourPalettes_Christmas_04();
    void init_ColourPalettes_Christmas_05();
    void init_ColourPalettes_Christmas_06();
    void init_ColourPalettes_Christmas_07();
    void init_ColourPalettes_Christmas_08();
    void init_ColourPalettes_Christmas_09();
    void init_ColourPalettes_Christmas_10();
    void init_ColourPalettes_Christmas_11();
    void init_ColourPalettes_Christmas_12();
    void init_ColourPalettes_Christmas_13();
    void init_ColourPalettes_Christmas_14();
    void init_ColourPalettes_Christmas_15();
    void init_ColourPalettes_Christmas_16();
    void init_ColourPalettes_Christmas_17();
    void init_ColourPalettes_Christmas_18();
    void init_ColourPalettes_Christmas_19();
    void init_ColourPalettes_Christmas_20();
    void init_ColourPalettes_Christmas_21();
    void init_ColourPalettes_Christmas_22();
    void init_ColourPalettes_Christmas_23();
    void init_ColourPalettes_Christmas_24();
    void init_ColourPalettes_Christmas_25();
    void init_ColourPalettes_Christmas_26();
    void init_ColourPalettes_Christmas_27();
    void init_ColourPalettes_Christmas_28();
    void init_ColourPalettes_Christmas_29();
    void init_ColourPalettes_Christmas_30();
    void init_ColourPalettes_Sky_Glow_01();
    void init_ColourPalettes_Custom_User_01();
    void init_ColourPalettes_Sunrise_01();
    void init_ColourPalettes_Sunrise_02();
    void init_ColourPalettes_Sunrise_03();
    void init_ColourPalettes_Sunrise_04();
    void init_ColourPalettes_Sunset_01();
    void init_ColourPalettes_Sunset_02();
    void init_ColourPalettes_Candle_Flame_01();
    void init_ColourPalettes_Gradient_SunLevel_Group01_01();
    void init_ColourPalettes_Gradient_SunLevel_Group01_02();
    void init_ColourPalettes_Gradient_SunLevel_Group01_03();
    void init_ColourPalettes_Gradient_SunLevel_Group01_04();
    void init_ColourPalettes_Gradient_SunLevel_Group01_05();
    void init_ColourPalettes_Gradient_SunLevel_Group01_06();
    void init_ColourPalettes_Gradient_SunLevel_Group01_07();
    /**
     * Specialised palettes
     * */
    void init_ColourPalettes_Solid_RGBCCT_Sun_Elevation_With_Degrees_Index_01();
    void init_ColourPalettes_Solid_RGBCCT_Sun_Elevation_With_Degrees_Index_02();
    void init_ColourPalettes_Solid_RGBCCT_Sun_Elevation_With_Degrees_Index_03();
    void init_ColourPalettes_Solid_RGBCCT_Sun_Elevation_With_Degrees_Index_04();
    void init_ColourPalettes_Solid_RGBCCT_Sun_Elevation_With_Degrees_Index_Dual_Colours_01();

    
    /**************
     * COLOUR Designs by name
     * PALETTE are different colours by pixel
    **************/                            
    int16_t GetPaletteIDbyName(const char* c);
    const char* GetPaletteName(char* buffer, uint8_t buflen);  
    const char* GetPaletteNameByID(uint8_t id, char* buffer, uint8_t buflen);
    const char* GetPaletteFriendlyName(char* buffer, uint8_t buflen);

    /**
     * @brief Overview of palettes
     *
     * STATIC progmem mine
     * STATIC CRGB16 (no grads)
     * STATIC CRGB16 gradient
     * 
     * VARIABLE HSB
     * VARIABLE Segment Colour
     * VARIABLE Encoded 
     */
    
    enum PALETTELIST_STATIC__IDS
    {
      PALETTELIST_STATIC_PARTY_DEFAULT__ID = 0,  // Default
      PALETTELIST_STATIC_HOLLOWEEN_OP__ID,
      PALETTELIST_STATIC_HOLLOWEEN_OGP__ID,
      PALETTELIST_STATIC_HOT_PINK_NEON_WITH_NAVY__ID,
      PALETTELIST_STATIC_RAINBOW__ID,
      PALETTELIST_STATIC_RAINBOW_INVERTED__ID,
      PALETTELIST_STATIC_PASTEL_01__ID,
      PALETTELIST_STATIC_PASTEL_02__ID,
      PALETTELIST_STATIC_WINTER__ID,
      PALETTELIST_STATIC_AUTUMN_GREEN__ID,
      PALETTELIST_STATIC_AUTUMN_RED__ID,
      PALETTELIST_STATIC_GRADIENT_01__ID,
      PALETTELIST_STATIC_GRADIENT_02__ID,
      PALETTELIST_STATIC_GRADIENT_PASTEL_TONES_PURPLE__ID,
      PALETTELIST_STATIC_BERRY_GREEN__ID,
      PALETTELIST_STATIC_CHRISTMAS_01__ID,
      PALETTELIST_STATIC_CHRISTMAS_02__ID,
      PALETTELIST_STATIC_CHRISTMAS_03__ID,
      PALETTELIST_STATIC_CHRISTMAS_04__ID,
      PALETTELIST_STATIC_CHRISTMAS_05__ID,
      PALETTELIST_STATIC_CHRISTMAS_06__ID,
      PALETTELIST_STATIC_CHRISTMAS_07__ID,
      PALETTELIST_STATIC_CHRISTMAS_08__ID,
      PALETTELIST_STATIC_CHRISTMAS_09__ID,
      PALETTELIST_STATIC_CHRISTMAS_10__ID,
      PALETTELIST_STATIC_CHRISTMAS_11__ID,
      PALETTELIST_STATIC_CHRISTMAS_12__ID,
      PALETTELIST_STATIC_CHRISTMAS_13__ID,
      PALETTELIST_STATIC_CHRISTMAS_14__ID,
      PALETTELIST_STATIC_CHRISTMAS_15__ID,
      PALETTELIST_STATIC_CHRISTMAS_16__ID,
      PALETTELIST_STATIC_CHRISTMAS_17__ID,
      PALETTELIST_STATIC_CHRISTMAS_18__ID,
      PALETTELIST_STATIC_CHRISTMAS_19__ID,
      PALETTELIST_STATIC_CHRISTMAS_20__ID,
      PALETTELIST_STATIC_CHRISTMAS_21__ID,
      PALETTELIST_STATIC_CHRISTMAS_22__ID,
      PALETTELIST_STATIC_CHRISTMAS_23__ID,
      PALETTELIST_STATIC_CHRISTMAS_24__ID,
      PALETTELIST_STATIC_CHRISTMAS_25__ID,
      PALETTELIST_STATIC_CHRISTMAS_26__ID,
      PALETTELIST_STATIC_CHRISTMAS_27__ID,
      PALETTELIST_STATIC_CHRISTMAS_28__ID,
      PALETTELIST_STATIC_CHRISTMAS_29__ID,
      PALETTELIST_STATIC_CHRISTMAS_30__ID,
      PALETTELIST_STATIC_SUNRISE_01__ID,
      PALETTELIST_STATIC_SUNRISE_02__ID,
      PALETTELIST_STATIC_SUNRISE_03__ID,
      PALETTELIST_STATIC_SUNRISE_04__ID,
      PALETTELIST_STATIC_SUNSET_01__ID,
      PALETTELIST_STATIC_SUNSET_02__ID,
      PALETTELIST_STATIC_CANDLE_FLAME_01__ID,
      PALETTELIST_STATIC_GRADIENT_FIRE_01__ID,
      PALETTELIST_STATIC_SKY_GLOW_01__ID,
      /**
       * GRADIENT_SUNLEVEL_GROUP01 - RGBCCT, Gradient
       * Effect will go through these to form a rising/falling sun
       * */
      //add below horizon gradients (5 below), for black/blue
      PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_01__ID,
      PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_02__ID,
      PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_03__ID,
      PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_04__ID,
      PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_05__ID,
      PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_06__ID,
      PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_07__ID,
      /**
       * GRADIENT_SUNLEVEL_GROUP01 - RGBCCT, Gradient
       * */
      //Single sun azimuth rgbcct colour, allows AZ code to select colour as "As the sky looks"

      // Another palette which does the same, but applies CCT by AZ position 
      // Do as effect

      //like grad 1, order of using these will produce sunrising/falling
      //linear gradient sunlevel below (dark with no sun point above horizon)
      //linear gradient sunlevel low (dark red center, orange sides)
      //linear gradient sunlevel med (light red/orange center, orange, then yellow sides, pale blue on farest edges)
      //linear gradient sunlevel high (orange sun center, yellow slightly, largely light blue)
      //linear gradient sunlevel highest/midday (orange sun center, white edges... pale blue rest)


      PALETTELIST_STATIC_OCEAN_01__ID,
      PALETTELIST_STATIC_CUSTOM_USER_01__ID,
      PALETTELIST_STATIC_SINGLE_FIRE_01__ID,

      /**
       * Patterns for Gazebo
       * */
      PALETTELIST_STATIC_FLOWER_SWEATPEAS_01__ID,
      PALETTELIST_STATIC_FLOWER_SWEATPEAS_02__ID,
      PALETTELIST_STATIC_PINK_PURPLE_01__ID,
      PALETTELIST_STATIC_PINK_PURPLE_02__ID,
      PALETTELIST_STATIC_PINK_PURPLE_03__ID,

      /**
       * Create a few palettes that are updated/calculated using sun/azimuth
       * eg: 1) ambilight colours that are calculated to show sun brightness up top
       * */


      /**
       * Specialised palettes: Solid single colour
       * */
      PALETTELIST_STATIC__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01__ID,
      PALETTELIST_STATIC__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_02__ID,
      PALETTELIST_STATIC__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_03__ID,
      PALETTELIST_STATIC__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_04__ID,
      /**
       * Rgbcct colour pairs: For ambilight top/bottom arrangement
       * */
      PALETTELIST_STATIC_DUAL_COLOUR_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01__ID,
      

      PALETTELIST_STATIC_BLACK__ID, 
      // Count of total handlers and starting point for other modules
      PALETTELIST_STATIC_LENGTH__ID 
    };
    
    /**
     * @brief Only the static/progmem stuff below will be taken from inside palette.cpp
     * 
     */
    enum PALETTELIST_STATIC_CRGBPALETTE16__IDS{
      
      PALETTELIST_STATIC_CRGBPALETTE16__CLOUD_COLOURS__ID= PALETTELIST_STATIC_LENGTH__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__LAVA_COLOURS__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__OCEAN_COLOUR__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__FOREST_COLOUR__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_STRIPE_COLOUR__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__PARTY_COLOUR__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__HEAT_COLOUR__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID
    };

    enum PALETTELIST_CRGBPALETTE16_GRADIENT___PALETTES__IDS
    { //39 of them 
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID = PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__RIVENDELL__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__OCEAN_BREEZE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__RED_AND_BLUE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__YELLOWOUT__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__ANALOGOUS__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__PINK_SPLASH__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET_YELLOW__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__ANOTHER__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__BEECH__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__VINTAGE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__DEPARTURE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__LANDSCAPE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__BEACH__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__RAINBOW_SHERBET__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__HULT__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__HULT64__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__DRYWET__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__JUL__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__GRINTAGE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__REWHI__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__LAVA_FIRE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__ICE_FIRE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__CYANE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__LIGHT_PINK__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__AUTUMN__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__BLUE_MAGENTA__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__BLACK_MAGENTA_RED__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__BLACK_RED_MAGENTA_YELLOW__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__BLUE_CYAN_YELLOW__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__ORANGE_TEAL__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__TIAMAT__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__APRIL_NIGHT__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__ORANGERY__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__C9__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SAKURA__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__AURORA__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__ATLANTICA__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID
    };

    
    /**
     * @brief Variable lengths to be moved to end once things are stable
     * 
     */
    enum PALETTELIST_VARIABLE_HSBID__IDS{ // 10 TOTAL variable and can be deleted by the user, saved in memory
      PALETTELIST_VARIABLE_HSBID_01__ID = PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID,
      PALETTELIST_VARIABLE_HSBID_02__ID,
      PALETTELIST_VARIABLE_HSBID_03__ID,
      PALETTELIST_VARIABLE_HSBID_04__ID,
      PALETTELIST_VARIABLE_HSBID_05__ID,
      PALETTELIST_VARIABLE_HSBID_06__ID,
      PALETTELIST_VARIABLE_HSBID_07__ID,
      PALETTELIST_VARIABLE_HSBID_08__ID,
      PALETTELIST_VARIABLE_HSBID_09__ID,
      PALETTELIST_VARIABLE_HSBID_10__ID,
      PALETTELIST_VARIABLE_HSBID_LENGTH__ID  // Count of total handlers and starting point for other modules
    };
    
    enum PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR__IDS{
      PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_01__ID = PALETTELIST_VARIABLE_HSBID_LENGTH__ID, // New scene colour, static
      PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_02__ID,
      PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_03__ID,
      PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_04__ID,
      PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_05__ID,
      PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID
    };

    
    // One special buffer, block of memory allows any format of pallete (eg)
    // This will also require encoding of type of palette into the buffer somehow
    // shared, with overlapping memory
    // first X amounts of the original buffer will be used for encoding type, but data pointer will remain the same
    enum PALETTELIST_VARIABLE_GENERIC__IDS{
      PALETTELIST_VARIABLE_GENERIC_01__ID = PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID, // New scene colour, static
      // PALETTELIST_VARIABLE_GENERIC_02__ID, // New scene colour, static
      PALETTELIST_VARIABLE_GENERIC_LENGTH__ID    
    };

    enum  PALETTELIST_HTML_COLOUR__IDS{      
      PALETTELIST_HTML_COLOUR__AliceBlue__ID = PALETTELIST_VARIABLE_GENERIC_LENGTH__ID,
      PALETTELIST_HTML_COLOUR__AntiqueWhite__ID,
      PALETTELIST_HTML_COLOUR__Aqua__ID,
      PALETTELIST_HTML_COLOUR__Aquamarine__ID,
      PALETTELIST_HTML_COLOUR__Azure__ID,
      PALETTELIST_HTML_COLOUR__Beige__ID,
      PALETTELIST_HTML_COLOUR__Bisque__ID,
      PALETTELIST_HTML_COLOUR__Black__ID,
      PALETTELIST_HTML_COLOUR__BlanchedAlmond__ID,
      PALETTELIST_HTML_COLOUR__Blue__ID,
      PALETTELIST_HTML_COLOUR__BlueViolet__ID,
      PALETTELIST_HTML_COLOUR__Brown__ID,
      PALETTELIST_HTML_COLOUR__BurlyWood__ID,
      PALETTELIST_HTML_COLOUR__CadetBlue__ID,
      PALETTELIST_HTML_COLOUR__Chartreuse__ID,
      PALETTELIST_HTML_COLOUR__Chocolate__ID,
      PALETTELIST_HTML_COLOUR__Coral__ID,
      PALETTELIST_HTML_COLOUR__CornflowerBlue__ID,
      PALETTELIST_HTML_COLOUR__Cornsilk__ID,
      PALETTELIST_HTML_COLOUR__Crimson__ID,
      PALETTELIST_HTML_COLOUR__Cyan__ID,
      PALETTELIST_HTML_COLOUR__DarkBlue__ID,
      PALETTELIST_HTML_COLOUR__DarkCyan__ID,
      PALETTELIST_HTML_COLOUR__DarkGoldenrod__ID,
      PALETTELIST_HTML_COLOUR__DarkGray__ID,
      PALETTELIST_HTML_COLOUR__DarkGreen__ID,
      PALETTELIST_HTML_COLOUR__DarkGrey__ID,
      PALETTELIST_HTML_COLOUR__DarkKhaki__ID,
      PALETTELIST_HTML_COLOUR__DarkMagenta__ID,
      PALETTELIST_HTML_COLOUR__DarkOliveGreen__ID,
      PALETTELIST_HTML_COLOUR__DarkOrange__ID,
      PALETTELIST_HTML_COLOUR__DarkOrchid__ID,
      PALETTELIST_HTML_COLOUR__DarkRed__ID,
      PALETTELIST_HTML_COLOUR__DarkSalmon__ID,
      PALETTELIST_HTML_COLOUR__DarkSeaGreen__ID,
      PALETTELIST_HTML_COLOUR__DarkSlateBlue__ID,
      PALETTELIST_HTML_COLOUR__DarkSlateGray__ID,
      PALETTELIST_HTML_COLOUR__DarkSlateGrey__ID,
      PALETTELIST_HTML_COLOUR__DarkTurquoise__ID,
      PALETTELIST_HTML_COLOUR__DarkViolet__ID,
      PALETTELIST_HTML_COLOUR__DeepPink__ID,
      PALETTELIST_HTML_COLOUR__DeepSkyBlue__ID,
      PALETTELIST_HTML_COLOUR__DimGray__ID,
      PALETTELIST_HTML_COLOUR__DimGrey__ID,
      PALETTELIST_HTML_COLOUR__DodgerBlue__ID,
      PALETTELIST_HTML_COLOUR__Firebrick__ID,
      PALETTELIST_HTML_COLOUR__FloralWhite__ID,
      PALETTELIST_HTML_COLOUR__ForestGreen__ID,
      PALETTELIST_HTML_COLOUR__Fuchsia__ID,
      PALETTELIST_HTML_COLOUR__Gainsboro__ID,
      PALETTELIST_HTML_COLOUR__GhostWhite__ID,
      PALETTELIST_HTML_COLOUR__Gold__ID,
      PALETTELIST_HTML_COLOUR__Goldenrod__ID,
      PALETTELIST_HTML_COLOUR__Gray__ID,
      PALETTELIST_HTML_COLOUR__Green__ID,
      PALETTELIST_HTML_COLOUR__GreenYellow__ID,
      PALETTELIST_HTML_COLOUR__Grey__ID,
      PALETTELIST_HTML_COLOUR__Honeydew__ID,
      PALETTELIST_HTML_COLOUR__HotPink__ID,
      PALETTELIST_HTML_COLOUR__IndianRed__ID,
      PALETTELIST_HTML_COLOUR__Indigo__ID,
      PALETTELIST_HTML_COLOUR__Ivory__ID,
      PALETTELIST_HTML_COLOUR__Khaki__ID,
      PALETTELIST_HTML_COLOUR__Lavender__ID,
      PALETTELIST_HTML_COLOUR__LavenderBlush__ID,
      PALETTELIST_HTML_COLOUR__LawnGreen__ID,
      PALETTELIST_HTML_COLOUR__LemonChiffon__ID,
      PALETTELIST_HTML_COLOUR__LightBlue__ID,
      PALETTELIST_HTML_COLOUR__LightCoral__ID,
      PALETTELIST_HTML_COLOUR__LightCyan__ID,
      PALETTELIST_HTML_COLOUR__LightGoldenrodYellow__ID,
      PALETTELIST_HTML_COLOUR__LightGray__ID,
      PALETTELIST_HTML_COLOUR__LightGreen__ID,
      PALETTELIST_HTML_COLOUR__LightGrey__ID,
      PALETTELIST_HTML_COLOUR__LightPink__ID,
      PALETTELIST_HTML_COLOUR__LightSalmon__ID,
      PALETTELIST_HTML_COLOUR__LightSeaGreen__ID,
      PALETTELIST_HTML_COLOUR__LightSkyBlue__ID,
      PALETTELIST_HTML_COLOUR__LightSlateGray__ID,
      PALETTELIST_HTML_COLOUR__LightSlateGrey__ID,
      PALETTELIST_HTML_COLOUR__LightSteelBlue__ID,
      PALETTELIST_HTML_COLOUR__LightYellow__ID,
      PALETTELIST_HTML_COLOUR__Lime__ID,
      PALETTELIST_HTML_COLOUR__LimeGreen__ID,
      PALETTELIST_HTML_COLOUR__Linen__ID,
      PALETTELIST_HTML_COLOUR__Magenta__ID,
      PALETTELIST_HTML_COLOUR__Maroon__ID,
      PALETTELIST_HTML_COLOUR__MediumAquamarine__ID,
      PALETTELIST_HTML_COLOUR__MediumBlue__ID,
      PALETTELIST_HTML_COLOUR__MediumOrchid__ID,
      PALETTELIST_HTML_COLOUR__MediumPurple__ID,
      PALETTELIST_HTML_COLOUR__MediumSeagreen__ID,
      PALETTELIST_HTML_COLOUR__MediumSlateBlue__ID,
      PALETTELIST_HTML_COLOUR__MediumSpringGreen__ID,
      PALETTELIST_HTML_COLOUR__MediumTurquoise__ID,
      PALETTELIST_HTML_COLOUR__MediumVioletRed__ID,
      PALETTELIST_HTML_COLOUR__MidnightBlue__ID,
      PALETTELIST_HTML_COLOUR__MintCream__ID,
      PALETTELIST_HTML_COLOUR__MistyRose__ID,
      PALETTELIST_HTML_COLOUR__Moccasin__ID,
      PALETTELIST_HTML_COLOUR__NavajoWhite__ID,
      PALETTELIST_HTML_COLOUR__Navy__ID,
      PALETTELIST_HTML_COLOUR__OldLace__ID,
      PALETTELIST_HTML_COLOUR__Olive__ID,
      PALETTELIST_HTML_COLOUR__OliveDrab__ID,
      PALETTELIST_HTML_COLOUR__Orange__ID,
      PALETTELIST_HTML_COLOUR__OrangeRed__ID,
      PALETTELIST_HTML_COLOUR__Orchid__ID,
      PALETTELIST_HTML_COLOUR__PaleGoldenrod__ID,
      PALETTELIST_HTML_COLOUR__PaleGreen__ID,
      PALETTELIST_HTML_COLOUR__PaleTurquoise__ID,
      PALETTELIST_HTML_COLOUR__PaleVioletRed__ID,
      PALETTELIST_HTML_COLOUR__PapayaWhip__ID,
      PALETTELIST_HTML_COLOUR__PeachPuff__ID,
      PALETTELIST_HTML_COLOUR__Peru__ID,
      PALETTELIST_HTML_COLOUR__Pink__ID,
      PALETTELIST_HTML_COLOUR__Plum__ID,
      PALETTELIST_HTML_COLOUR__PowderBlue__ID,
      PALETTELIST_HTML_COLOUR__Purple__ID,
      PALETTELIST_HTML_COLOUR__Red__ID,
      PALETTELIST_HTML_COLOUR__RosyBrown__ID,
      PALETTELIST_HTML_COLOUR__RoyalBlue__ID,
      PALETTELIST_HTML_COLOUR__SaddleBrown__ID,
      PALETTELIST_HTML_COLOUR__Salmon__ID,
      PALETTELIST_HTML_COLOUR__SandyBrown__ID,
      PALETTELIST_HTML_COLOUR__SeaGreen__ID,
      PALETTELIST_HTML_COLOUR__SeaShell__ID,
      PALETTELIST_HTML_COLOUR__Sienna__ID,
      PALETTELIST_HTML_COLOUR__Silver__ID,
      PALETTELIST_HTML_COLOUR__SkyBlue__ID,
      PALETTELIST_HTML_COLOUR__SlateBlue__ID,
      PALETTELIST_HTML_COLOUR__SlateGray__ID,
      PALETTELIST_HTML_COLOUR__SlateGrey__ID,
      PALETTELIST_HTML_COLOUR__Snow__ID,
      PALETTELIST_HTML_COLOUR__SpringGreen__ID,
      PALETTELIST_HTML_COLOUR__SteelBlue__ID,
      PALETTELIST_HTML_COLOUR__Tan__ID,
      PALETTELIST_HTML_COLOUR__Teal__ID,
      PALETTELIST_HTML_COLOUR__Thistle__ID,
      PALETTELIST_HTML_COLOUR__Tomato__ID,
      PALETTELIST_HTML_COLOUR__Turquoise__ID,
      PALETTELIST_HTML_COLOUR__Violet__ID,
      PALETTELIST_HTML_COLOUR__Wheat__ID,
      PALETTELIST_HTML_COLOUR__White__ID,
      PALETTELIST_HTML_COLOUR__WhiteSmoke__ID,
      PALETTELIST_HTML_COLOUR__Yellow__ID,
      PALETTELIST_HTML_COLOUR__YellowGreen__ID,
      PALETTELIST_HTML_COLOUR__LENGTH__ID
    };


    /**
     * @brief These below to become fully added into my methods as another encoding type
     * to be merged and named in my static list
     * 
     */
    enum PALETTELIST_SEGMENTS_STORED_VARIABLE_CRGBPALETTE16_PALETTES__IDS{    // Not stored in memory, but changes when called, maybe grow with segment size, but not stored there? save as encoded?
      PALETTELIST_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_01__ID = PALETTELIST_HTML_COLOUR__LENGTH__ID,
      
      PALETTELIST_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_02__ID,
      PALETTELIST_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_03__ID,
      PALETTELIST_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_04__ID,
      PALETTELIST_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_05__ID,

      PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY__ID,
      PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY__ID,
      PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY_TERTIARY__ID,
      PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY_TERTIARY_REPEATED__ID,
      PALETTELIST_VARIABLE_CRGBPALETTE16__LENGTH__ID    
    };
    #define PALETTELIST_CRGBPALETTE_FIRST_ID PALETTELIST_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS__ID





    #define PALETTELIST_TOTAL_LENGTH PALETTELIST_VARIABLE_CRGBPALETTE16__LENGTH__ID

    #define PALETTELIST_COLOUR_AMOUNT_MAX 20//15 new max


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
        uint16_t index_byte_width : 3; // 3 bits wide, or 0b000 gives 9 value options


        uint16_t index_exact : 1;
        uint16_t index_scaled_to_segment : 1;
        uint16_t index_is_trigger_value_exact : 1;
        uint16_t index_is_trigger_value_scaled100 : 1;

        uint16_t encoded_as_hsb_ids : 1; //move this to other encoded types
        uint16_t encoded_as_crgb_palette_16 : 1;
        uint16_t encoded_as_crgb_palette_256 : 1;
        uint16_t palette_can_be_modified : 1;
      };
    } PALETTE_ENCODING_DATA;

    /**
     * @brief Should this really be its own list, its already inside the palette class?
     * 
     */
    struct PALETTELIST{
      struct PALETTE{ //6 bytes per palette
        // enum: Used to call for ctr of friendly name
        uint8_t id = 0;
        // Pointer to name
        char* friendly_name_ctr = nullptr;
        // Map IDs
        uint8_t* data; // pointer to saved memory (inc. progmem)
        // Active pixels
        uint8_t  data_length = 0; // total bytes in palette (includes index information)
        // Contains information on formatting of data buffer
        PALETTE_ENCODING_DATA encoding;
      };
      // Add static palettes here
      PALETTE party_default;
      PALETTE black;
      PALETTE holloween_op;
      PALETTE holloween_ogp;
      PALETTE hot_pink_neon_with_navy;
      PALETTE single_fire_01;
      PALETTE flower_sweetpeas_01;
      PALETTE flower_sweetpeas_02;
      PALETTE pink_purple_01;
      PALETTE pink_purple_02;
      PALETTE pink_purple_03;
      PALETTE winter;
      PALETTE autumn_green;
      PALETTE autumn_red;
      PALETTE rainbow;
      PALETTE rainbow_inverted;
      PALETTE pastel_01;
      PALETTE pastel_02;
      PALETTE ocean_01;
      PALETTE gradient_01;
      PALETTE gradient_02;
      PALETTE gradient_fire_01;
      PALETTE gradient_pastel_tones_purple;
      PALETTE sunrise_01;
      PALETTE sunrise_02;
      PALETTE sunrise_03;
      PALETTE sunrise_04;
      PALETTE sunset_01;
      PALETTE sunset_02;
      PALETTE sky_glow_01;
      PALETTE candle_flame_01;
      PALETTE berry_green;
      PALETTE gradient_sunlevel_group01_01;
      PALETTE gradient_sunlevel_group01_02;
      PALETTE gradient_sunlevel_group01_03;
      PALETTE gradient_sunlevel_group01_04;
      PALETTE gradient_sunlevel_group01_05;
      PALETTE gradient_sunlevel_group01_06;
      PALETTE gradient_sunlevel_group01_07;
      PALETTE gradient_solid_rgbcct_sun_elevation_with_degrees_in_index_01;
      PALETTE gradient_solid_rgbcct_sun_elevation_with_degrees_in_index_02;
      PALETTE gradient_solid_rgbcct_sun_elevation_with_degrees_in_index_03;
      PALETTE gradient_solid_rgbcct_sun_elevation_with_degrees_in_index_04;
      PALETTE gradient_solid_rgbcct_sun_elevation_with_degrees_in_index_dual_colours_01;
      // Christmas static patterns
      PALETTE christmas_01;
      PALETTE christmas_02;
      PALETTE christmas_03;
      PALETTE christmas_04;
      PALETTE christmas_05;
      PALETTE christmas_06;
      PALETTE christmas_07;
      PALETTE christmas_08;
      PALETTE christmas_09;
      PALETTE christmas_10;
      PALETTE christmas_11;
      PALETTE christmas_12;
      PALETTE christmas_13;
      PALETTE christmas_14;
      PALETTE christmas_15;
      PALETTE christmas_16;
      PALETTE christmas_17;
      PALETTE christmas_18;
      PALETTE christmas_19;
      PALETTE christmas_20;
      PALETTE christmas_21;
      PALETTE christmas_22;
      PALETTE christmas_23;
      PALETTE christmas_24;
      PALETTE christmas_25;
      PALETTE christmas_26;
      PALETTE christmas_27;
      PALETTE christmas_28;
      PALETTE christmas_29;
      PALETTE christmas_30;
      // Created for other people
      PALETTE custom_user_01;




      // Can be edited
      PALETTE hsbid_users[10];
      // Can be edited
      PALETTE rgbcct_segment_colour_users[5];    // Now contained within segment so is not needed here! (at least data is not, but name may be)
      // Generic variable palette 
      PALETTE encoded_users; // Should remain only option, to permit the largest buffer. Long term, it could be made dynamic



      // Active
      PALETTE *ptr = &rainbow;
    }palettelist;


    // #endif
        
    RgbcctColor GetActiveFirstColourFromCurrentPalette();
    
    RgbwColor Color32bit2RgbColour(uint32_t colour32bit);

    void UpdateSetOutputs();
    const char* GetColourMapNamebyID(uint8_t id, char* buffer, uint8_t buflen);
    int8_t      GetColourMapIDbyName(const char* c);

    const uint8_t* GetDefaultColourPaletteUserIDs_P(uint8_t id);
    uint8_t  GetDefaultColourPaletteUserIDsCount(uint8_t id);

    const char* GetPaletteNameFriendlyFirstByID(uint8_t id, char* buffer, uint8_t buflen);
    const char* GetPaletteFriendlyNameByID(uint8_t id, char* buffer, uint8_t buflen);    

    HsbColor GetHsbColour(uint8_t id);
          
    uint8_t GetColourMapSizeByPaletteID(uint8_t palette_id);
    uint16_t GetNumberOfColoursInPalette(uint16_t palette_id, uint8_t pixel_width_contrained_limit = 0);
    uint16_t GetNumberOfColoursInPalette(PALETTELIST::PALETTE *ptr = nullptr, uint8_t pixel_width_contrained_limit = 0);


    #ifdef ENABLE_DEVFEATURE_PALETTES_PRELOAD_STATIC_PALETTE_VARIABLES_WHEN_SETTING_CURRENT_PALLETTE
    void LoadPalette(uint16_t palette_id);
    #endif // ENABLE_DEVFEATURE_PALETTES_PRELOAD_STATIC_PALETTE_VARIABLES_WHEN_SETTING_CURRENT_PALLETTE


/**
 * @brief Needs to be moved so FastLED palettes can differ in each segment
 * 
 */
    CRGBPalette16 currentPalette;    

    uint8_t GetEncodedColourWidth( PALETTE_ENCODING_DATA encoded );

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
      uint8_t mcol = 0, // will be phased out
      bool     flag_convert_pixel_index_to_get_exact_crgb_colour = false,   // added by me, to make my effects work with CRGBPalette16
      uint8_t  brightness_scale = 255//, //255(default): No scaling, 0-255 scales the brightness of returned colour (remember all colours are saved in full 255 scale)
      // uint8_t* discrete_colours_in_palette = nullptr
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
      uint8_t mcol = 0, // will be phased out
      bool     flag_convert_pixel_index_to_get_exact_crgb_colour = false,   // added by me, to make my effects work with CRGBPalette16
      uint8_t  brightness_scale = 255//, //255(default): No scaling, 0-255 scales the brightness of returned colour (remember all colours are saved in full 255 scale)
      // uint8_t* discrete_colours_in_palette = nullptr
    );



    void LoadPalette_CRGBPalette16_Static(uint8_t palette_id = 0);
    PALETTELIST::PALETTE* GetPalettePointerByID(uint8_t id);
          
    void init_PresetColourPalettes_HSBID_UserFill(uint8_t id);
    void init_PresetColourPalettes_User_RGBCCT_Fill(uint8_t id);
    void init_PresetColourPalettes_User_Generic_Fill(uint8_t id);
   
    bool CheckPaletteIsEditable(PALETTELIST::PALETTE *ptr);
    bool CheckPaletteByIDIsEditable(uint8_t id);

    void SetPaletteListPtrFromID(uint8_t id);

    // friend class mPaletteContainer;

};

#define mPaletteI mPalette::GetInstance() // lets investigate making mPalette NOT a singleton, though, to be included inside palette controller, it might need to be so it only has once instance

#endif

#endif // _M_PALETTE_H


