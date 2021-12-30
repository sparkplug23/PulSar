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

#include "6_Lights/Palette/mPalette_Progmem.h"

#include "2_CoreSystem/Languages/mLanguageProgmem.h"
#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_DEVFEATURE_METHOD_WLED_BUILD
#define FASTLED_INTERNAL // suppress pragma warning messages
#include "6_Lights/FastLED_Modified/FastLED.h"
#endif // USE_DEVFEATURE_METHOD_WLED_BUILD
#ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
#define FASTLED_INTERNAL // suppress pragma warning messages
#include "6_Lights/FastLED_Modified/FastLED.h"
#endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

//Remember to sync with what is in palette_progmem.h
enum fMapIDs_Type__IDS{
  MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX__ID=0,
  MAPIDS_TYPE_HSBCOLOUR_NOINDEX__ID,
  MAPIDS_TYPE_HSBCOLOUR_WITHINDEX__ID,
  MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_AND_SETALL__ID, //phase out
  
  MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT__ID,
  MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_GRADIENT__ID,
  
  MAPIDS_TYPE_RGBCOLOUR_NOINDEX__ID,
  MAPIDS_TYPE_RGBCOLOUR_WITHINDEX__ID,
  MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_AND_SETALL__ID, //phase out, assume max index IS set all

  MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID,
  MAPIDS_TYPE_RGBCCTCOLOUR_WITHINDEX_GRADIENT__ID,

  MAPIDS_TYPE_RGBCOLOUR_LENGTH__ID
};

/**
 * @brief 
 * Likely phase out, as any effect that knows to treat the userdefined palette as "direct" (eg shelf hearts) will just know that the gradient positions are simply positions, and does not need a type to say this
 * This will reduce memory waste
 * 
 */
enum fIndexs_Type__IDS{
  // INDEX_TYPE_NONE = 0,
  INDEX_TYPE_SCALED_255 = 0, //default option (to be used elsewhere, as preset is cleared to 0, these will be defaults)
  INDEX_TYPE_DIRECT,
  INDEX_TYPE_SCALED_100 
};

// #define ENABLE_DEVFEATURE_CHANGE_MPALETTE_TO_NON_STATIC

class mPalette 

// #ifdef ENABLE_DEVFEATURE_CHANGE_MPALETTE_TO_NON_STATIC
// // :
// //   public mTaskerInterface
// {
//   // private:
//     /* Prevent others from being created */
//     // mPalette(mPalette const& other) = delete;
//     // mPalette(mPalette&& other) = delete;
//     /* Private constructor to prevent instancing. */
//   public:
//     // // External function to get instance
//     // static mPalette* GetInstance();
//     // /* Here will be the instance stored. */
//     // static mPalette* instance;

//     mPalette(){};






//     #else

 
{
  private:
    /* Prevent others from being created */
    mPalette(mPalette const& other) = delete;
    mPalette(mPalette&& other) = delete;
    /* Private constructor to prevent instancing. */
    mPalette(){};
  public:
    // External function to get instance
    static mPalette* GetInstance();
    /* Here will be the instance stored. */
    static mPalette* instance;


    // #endif
        
    RgbcctColor GetActiveFirstColourFromCurrentPalette();
    
    RgbwColor Color32bit2RgbColour(uint32_t colour32bit);

    void UpdateSetOutputs();
    //
    void init_PresetColourPalettes(); 
    // First bytes stores length 
    void init_ColourPalettes_Autumn_Red();
    void init_ColourPalettes_Autumn();
    void init_ColourPalettes_Winter();
    void init_ColourPalettes_Pastel();
    void init_ColourPalettes_Ocean_01();
    void init_ColourPalettes_Rainbow();
    void init_ColourPalettes_RainbowInverted();
    void init_ColourPalettes_Holloween_OP();
    void init_ColourPalettes_Holloween_OGP();
    void init_ColourPalettes_Hot_Neon_Pink_With_Navy();
    void init_ColourPalettes_Flower_SweetPeas_01();
    void init_ColourPalettes_Flower_SweetPeas_02();
    void init_ColourPalettes_Pink_Purple_01();
    void init_ColourPalettes_MicroLEDS_Pastel_01();
    void init_ColourPalettes_Single_Fire_01();
    void init_ColourPalettes_Shelf_Hearts();
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

      

    enum SHELFFLIES_HEART_INDEX{ //right to left
      SHELFFLIES_HEART_INDEX1=1,
      SHELFFLIES_HEART_INDEX2=6,
      SHELFFLIES_HEART_INDEX3=10,
      SHELFFLIES_HEART_INDEX4=16,
      SHELFFLIES_HEART_INDEX5=21,
      SHELFFLIES_HEART_INDEX6=25,
      SHELFFLIES_HEART_INDEX7=31
    };


    /**************
     * COLOUR Designs by name
     * PALETTE are different colours by pixel
    **************/                            
    int8_t GetPaletteIDbyName(const char* c);
    const char* GetPaletteName(char* buffer, uint8_t buflen);  
    const char* GetPaletteNameByID(uint8_t id, char* buffer, uint8_t buflen);
    const char* GetPaletteFriendlyName(char* buffer, uint8_t buflen);

  // #define D_PALETTE_HSBID_DEFAULT_NAME_CTR "User"

  //Using new buffer, which allvariable palettes write in to, but pointer is used

  // 20*10 = 200 bytes
  // 5*5   = 20 bytes
  //       = 100 bytes
  enum PALETTELIST_VARIABLE_HSBID__IDS{ // 10 TOTAL variable and can be deleted by the user, saved in memory
    PALETTELIST_VARIABLE_HSBID_01__ID = 0,
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
  enum PALETTELIST_VARIABLE_RGBCCT__IDS{
    PALETTELIST_VARIABLE_RGBCCT_COLOUR_01__ID = PALETTELIST_VARIABLE_HSBID_LENGTH__ID, // New scene colour, static
    PALETTELIST_VARIABLE_RGBCCT_COLOUR_02__ID,
    PALETTELIST_VARIABLE_RGBCCT_COLOUR_03__ID,
    PALETTELIST_VARIABLE_RGBCCT_COLOUR_04__ID,
    PALETTELIST_VARIABLE_RGBCCT_COLOUR_05__ID,
    PALETTELIST_VARIABLE_RGBCCT_LENGTH__ID
  };
  // Primary use of this is segments colours, and thus default will be 3 colours per option
  // enum PALETTELIST_VARIABLE_RGBCCT_LIST__IDS{
  //   PALETTELIST_VARIABLE_RGBCCT_COLOUR_LIST_01__ID = PALETTELIST_VARIABLE_HSBID_LENGTH__ID, // New scene colour, static
  //   PALETTELIST_VARIABLE_RGBCCT_COLOUR_LIST_02__ID,
  //   PALETTELIST_VARIABLE_RGBCCT_COLOUR_LIST_03__ID,
  //   PALETTELIST_VARIABLE_RGBCCT_COLOUR_LIST_04__ID,
  //   PALETTELIST_VARIABLE_RGBCCT_COLOUR_LIST_05__ID,
  //   PALETTELIST_VARIABLE_RGBCCT_LENGTH_LIST__ID
  // };
  // One special buffer, block of memory allows any format of pallete (eg)
  // This will also require encoding of type of palette into the buffer somehow
  // shared, with overlapping memory
  // first X amounts of the original buffer will be used for encoding type, but colour_map_id pointer will remain the same
  enum PALETTELIST_VARIABLE_GENERIC__IDS{
    PALETTELIST_VARIABLE_GENERIC_01__ID = PALETTELIST_VARIABLE_RGBCCT_LENGTH__ID, // New scene colour, static
    // PALETTELIST_VARIABLE_GENERIC_02__ID, // New scene colour, static
    PALETTELIST_VARIABLE_GENERIC_LENGTH__ID    
  };
  enum PALETTELIST_STATIC__IDS{
    // Shelf Lights ie has some static leds
    PALETTELIST_STATIC_SHELF_HEARTS__ID = PALETTELIST_VARIABLE_GENERIC_LENGTH__ID,//PALETTELIST_VARIABLE_HSBID_LENGTH__ID,
    
    // to be moved to top of list later
    PALETTELIST__DEFAULT___ID,
    
    // Special patterns
    PALETTELIST_STATIC_HOLLOWEEN_OP__ID,
    PALETTELIST_STATIC_HOLLOWEEN_OGP__ID,
    PALETTELIST_STATIC_HOT_PINK_NEON_WITH_NAVY__ID,
    PALETTELIST_STATIC_RAINBOW__ID,
    PALETTELIST_STATIC_RAINBOW_INVERTED__ID,
    PALETTELIST_STATIC_PASTEL__ID, //PASTEL
    PALETTELIST_STATIC_WINTER__ID,
    PALETTELIST_STATIC_AUTUMN__ID,
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
    PALETTELIST_STATIC_SUNRISE_01__ID,
    PALETTELIST_STATIC_SUNRISE_02__ID,
    PALETTELIST_STATIC_SUNRISE_03__ID,
    PALETTELIST_STATIC_SUNRISE_04__ID,
    PALETTELIST_STATIC_SUNSET_01__ID,
    PALETTELIST_STATIC_SUNSET_02__ID,
    PALETTELIST_STATIC_CANDLE_FLAME_01__ID,
    PALETTELIST_STATIC_GRADIENT_FIRE_01__ID,
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
    PALETTELIST_STATIC_MICROLEDS_PASTEL_01__ID,

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

    // Count of total handlers and starting point for other modules
    PALETTELIST_STATIC_LENGTH__ID 
  };
  enum PALETTELIST_VARIABLE_FASTLED_PALETTES__IDS{ // to be renamed/moved, these palettes are colours stored inside segments
    PALETTELIST_VARIABLE_FASTLED_SEGMENT__COLOUR_01__ID = PALETTELIST_STATIC_LENGTH__ID,
    PALETTELIST_VARIABLE_FASTLED_SEGMENT__COLOUR_02__ID, // this will likely some how be merged with rgbcct user palettes later, especially if it needs to be variable to segment count
    PALETTELIST_VARIABLE_FASTLED_SEGMENT__COLOUR_03__ID,
    //to be added
    PALETTELIST_VARIABLE_FASTLED__LENGTH__ID    
  };
  enum PALETTELIST_SEGMENTS_STORED_VARIABLE_CRGBPALETTE16_PALETTES__IDS{    // Not stored in memory, but changes when called, maybe grow with segment size, but not stored there? save as encoded?
    PALETTELIST_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS__ID = PALETTELIST_VARIABLE_FASTLED__LENGTH__ID,
    PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY__ID,
    PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY__ID,
    PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY_TERTIARY__ID,
    PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY_TERTIARY_REPEATED__ID,
    PALETTELIST_VARIABLE_CRGBPALETTE16__LENGTH__ID    
  };
  enum PALETTELIST_STATIC_CRGBPALETTE16__IDS{
    
    PALETTELIST_STATIC_CRGBPALETTE16__CLOUD_COLOURS__ID= PALETTELIST_VARIABLE_CRGBPALETTE16__LENGTH__ID,
    PALETTELIST_STATIC_CRGBPALETTE16__LAVA_COLOURS__ID,
    PALETTELIST_STATIC_CRGBPALETTE16__OCEAN_COLOUR__ID,
    PALETTELIST_STATIC_CRGBPALETTE16__FOREST_COLOUR__ID,
    PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID,
    PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_STRIBE_COLOUR__ID,
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

  #define PALETTELIST_TOTAL_LENGTH PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID

    #define PALETTELIST_COLOUR_AMOUNT_MAX 20//15 new max

    
    typedef union {
      uint16_t data; // allows full manipulating
      struct { 
        //random palettes will require estimations of nearest hues
        //uint16_t fColours_from_mapTBR : 1;
         // randomise between the hue and saturation values
        uint16_t fRandomise_Between_Colour_Pairs : 1;                                     // should this really be a palette option, and not an effect option?
         // Scaled between first and second colour, ignores the rest
        //uint16_t fLinearBlend_Between_Colour_Pairs : 1; //in gradient?
        // Enable the brightness parameter from colourmap, scale with animation.brightness
        uint16_t fUse_Brightness_From_ColourMap_As_Ratio_Of_Global_Brightness : 1;        // should this really be a palette option, and not an effect option?      
        // Use mapped IDs as RGB values, {R,G,B...}
        //uint16_t fMapIDs_Are_RGB_TripletsTBR : 1; 
        // (max 255 length string, if string>255, assume percentage)
        //uint16_t fIndexs_Are_Percentage : 1;
        // Override Saturation and randomise, skewed high for visual similarity.
        uint16_t fOverride_Saturation_To_Random : 1;                                      // should this really be a palette option, and not an effect option?
        // Blends colours between indexs (used for fMapIDs_Type=4)
        //uint16_t fIndexs_Are_Gradient : 1;
        // Hold type of map
        uint16_t fMapIDs_Type : 7; //9 possible types
          // 0 - HsbColors directly, no index eg {hsb_color_map1, hsb_color_map2, hsb_color_map3,...}
          // 1 - HsbColors with indexes {index1,hsb_color_map1, index1,hsb_color_map1,...}
          // 2 - RgbColors only, triplets eg {R1,G1,B1, R2,G2,B2,...}
          // 3 - RgbColors triplets with indexed eg {index1,R1,G1,B1, index2,R2,G2,B2,...}
          // 4 - RgbColors triplets with indexed and largest index sets all as option eg {index1,R1,G1,B1, index2,R2,G2,B2,... MAX,R_all,G_all,B_all}
          // 5 - HsbColors only
          // 6 - HsbColors only
          // 7 - HsbColors only      
          // 8- 
          // 9 - MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
          // phase down to be only RGB, HSB?  
        //uint16_t fIndexs_Type : 2; // these shall be removed and assumed ALWAYS to be 255, unless the effect knows to treat them as indexes (eg shelf hearts)
        // 0 - exact
        // 1 - gradient range 0-255
        // 2 - gradient range 0-100%
        // 3 - reserved
        // Reserved
        uint16_t reserved : 6;
      };
    } PALETTE_FLAGS;

    // Consider template split
    struct PALETTELIST{
      struct PALETTE{ //6 bytes per palette
        // 16 bits
        PALETTE_FLAGS flags;        
        // enum: Used to call for ctr of friendly name
        uint8_t id = 0;
        // Pointer to name
        char* friendly_name_ctr = nullptr;
        // Map IDs
        uint8_t* colour_map_id; 
        // Active pixels
        uint8_t colour_map_size = 0;
      };
      // Can be edited
      PALETTE hsbid_users[10]; //reduce to 10, as "hsb_colour_ids"
      // Can be edited
      PALETTE rgbcct_users[5]; 
      // Generic variable palette
      PALETTE encoded_users; //single large buffer
      // Add static palettes here
      PALETTE holloween_op;
      PALETTE holloween_ogp;
      PALETTE hot_pink_neon_with_navy;
      PALETTE single_fire_01;
      PALETTE flower_sweetpeas_01;
      PALETTE flower_sweetpeas_02;
      PALETTE pink_purple_01;
      PALETTE microleds_pastel_01;
      PALETTE winter;
      PALETTE autumn;
      PALETTE autumn_red;
      PALETTE rainbow;
      PALETTE rainbow_inverted;
      PALETTE pastel;
      PALETTE ocean_01;
      PALETTE shelf_hearts;
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
      // Created for other people
      PALETTE custom_user_01;
      PALETTE *ptr = &rainbow;
    }palettelist;

    // One of the variable rgbcct is used as the function colour manipulators
    // uint8_t active_scene_palette_id = PALETTELIST_VARIABLE_RGBCCT_COLOUR_01__ID;

    const char* GetColourMapNamebyID(uint8_t id, char* buffer, uint8_t buflen);
    const char* GetColourMapNameNearestbyColour(HsbColor c, char* buffer, uint8_t buflen);
    int8_t      GetColourMapIDbyName(const char* c);
    int8_t GetNearestColourMapIDFromColour(HsbColor hsb);

    // void setdefault_PresetColourPalettes_UserFill(PALETTELIST::PALETTE *ptr, uint8_t* colour_ids, uint8_t colour_ids_len);

    const uint8_t* GetDefaultColourPaletteUserIDs_P(uint8_t id);
    uint8_t  GetDefaultColourPaletteUserIDsCount(uint8_t id);

    const char* GetPaletteNameFriendlyFirstByID(uint8_t id, char* buffer, uint8_t buflen);
    const char* GetPaletteFriendlyNameByID(uint8_t id, char* buffer, uint8_t buflen);    

     HsbColor GetHsbColour(uint8_t id);
      
    void ApplyGlobalBrightnesstoColour(RgbcctColor* colour);

    
    uint8_t GetColourMapSizeByPaletteID(uint8_t palette_id);
    uint16_t GetPixelsInMap(PALETTELIST::PALETTE *ptr = nullptr, uint8_t pixel_width_contrained_limit = 0);

    uint8_t GetPixelsWithByMapIDType(uint8_t fMapIDs_Type);

    CRGBPalette16 currentPalette;
    CRGBPalette16 targetPalette;

    RgbcctColor GetColourFromPalette(uint16_t palette_id,       uint16_t pixel_num = 0, int16_t *pixel_position = nullptr);
    RgbcctColor GetColourFromPalette(PALETTELIST::PALETTE *ptr, uint16_t pixel_num = 0, int16_t *pixel_position = nullptr);

    /**
     * @brief Next method to merge fastled and my methods together
     * 
     */
    #ifdef ENABLE_DEVFEATURE_PALETTE_ADVANCED_METHODS_GEN2
    uint32_t color_from_palette_internal(uint16_t palette_id, uint16_t i, bool mapping, bool wrap, uint8_t mcol, uint8_t pbri = 255, uint16_t palette_spread_length__virtualSegmentLength = 50);  
    RgbcctColor GetColourFromPalette_Joint(uint16_t palette_id, uint16_t pixel_num = 0, int16_t *pixel_position = nullptr, /*uint16_t i, is pixel_id*/ bool mapping = false, bool wrap = true, uint8_t mcol = 0, uint8_t pbri = 255, TBlendType blendType=NOBLEND, uint16_t palette_spread_length__virtualSegmentLength = 50);
    RgbcctColor GetColourFromPalette_Gradient(uint16_t palette_id, uint16_t pixel_num = 0, int16_t *pixel_position = nullptr, /*uint16_t i, is pixel_id*/ bool mapping = false, bool wrap = true, uint8_t mcol = 0, uint8_t pbri = 255, TBlendType blendType=NOBLEND, uint16_t palette_spread_length__virtualSegmentLength = 50);
    #endif //ENABLE_DEVFEATURE_PALETTE_ADVANCED_METHODS_GEN2


    // #ifdef ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS

    void UpdatePalette_FastLED_TargetPalette(void);
    void load_gradient_palette(uint8_t index);

    // #endif // ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS


    PALETTELIST::PALETTE* GetPalettePointerByID(uint8_t id);
          
    // void setdefault_PresetColourPalettes_UserFill(uint8_t id);
    void init_PresetColourPalettes_UserFill(uint8_t id);
    void init_PresetColourPalettes_User_RGBCCT_Fill(uint8_t id);
    void init_PresetColourPalettes_User_Generic_Fill(uint8_t id);
    // void setdefault_PresetColourPalettes_UserFill(PALETTELIST::PALETTE *ptr, const uint8_t* colour_ids, const uint8_t colour_ids_len); //declare

    int8_t CheckPaletteIsEditable(PALETTELIST::PALETTE *ptr);
    bool   CheckPaletteByIDIsEditable(uint8_t id);
    void SetPaletteListPtrFromID(uint8_t id);

    
    // One of the variable rgbcct is used as the function colour manipulators
    // uint8_t active_scene_palette_id = PALETTELIST_VARIABLE_RGBCCT_COLOUR_01__ID;

//     const char* GetColourMapNamebyID(uint8_t id, char* buffer, uint8_t buflen);
//     const char* GetColourMapNameNearestbyColour(HsbColor c, char* buffer, uint8_t buflen);
//     int8_t      GetColourMapIDbyName(const char* c);
//     int8_t GetNearestColourMapIDFromColour(HsbColor hsb);

    
// uint8_t GetColourMapSizeByPaletteID(uint8_t palette.id);

              
//     // void setdefault_PresetColourPalettes_UserFill(uint8_t id);
//     void init_PresetColourPalettes_UserFill(uint8_t id);
//     void init_PresetColourPalettes_User_RGBCCT_Fill(uint8_t id);
//     void init_PresetColourPalettes_User_Generic_Fill(uint8_t id);
//     // void setdefault_PresetColourPalettes_UserFill(PALETTELIST::PALETTE *ptr, const uint8_t* colour_ids, const uint8_t colour_ids_len); //declare

//     int8_t CheckPaletteIsEditable(PALETTELIST::PALETTE *ptr);
//     bool   CheckPaletteByIDIsEditable(uint8_t id);
//     void SetPaletteListPtrFromID(uint8_t id);





};


// #ifndef ENABLE_DEVFEATURE_CHANGE_MPALETTE_TO_NON_STATIC
#define mPaletteI mPalette::GetInstance()
// #endif  // ENABLE_DEVFEATURE_CHANGE_MPALETTE_TO_NON_STATIC



#endif

#endif // _M_PALETTE_H


