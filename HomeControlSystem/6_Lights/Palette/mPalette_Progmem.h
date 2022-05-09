// /*
//  * Color palettes for FastLED effects (65-73).
//  */

// // From ColorWavesWithPalettes by Mark Kriegsman: https://gist.github.com/kriegsman/8281905786e8b2632aeb
// // Unfortunaltely, these are stored in RAM!

// // Gradient palette "ib_jul01_gp", originally from
// // http://soliton.vm.bytemark.co.uk/pub/cpt-city/ing/xmas/tn/ib_jul01.png.index.html
// // converted for FastLED with gammas (2.6, 2.2, 2.5)
// // Size: 16 bytes of program space.

//https://www.espressif.com/sites/default/files/documentation/save_esp8266ex_ram_with_progmem_en.pdf

#ifndef mPalette_ProgmemPALETTES_H
#define mPalette_ProgmemPALETTES_H

#include "stdint.h"
#include "2_CoreSystem/mGlobalMacros.h"

// enum fMapIDs_Type__IDS_Copies{
//   D_MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX__ID=0,
//   D_MAPIDS_TYPE_HSBCOLOUR_NOINDEX__ID,
//   D_MAPIDS_TYPE_HSBCOLOUR_WITHINDEX__ID,
//   D_MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_AND_SETALL__ID, //phase out
  
//   D_MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT__ID,
//   D_MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_GRADIENT__ID,
  
//   D_MAPIDS_TYPE_RGBCOLOUR_NOINDEX__ID,
//   D_MAPIDS_TYPE_RGBCOLOUR_WITHINDEX__ID,
//   D_MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_AND_SETALL__ID, //phase out, assume max index IS set all

//   D_MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID,
//   D_MAPIDS_TYPE_RGBCCTCOLOUR_WITHINDEX_GRADIENT__ID,

//   D_MAPIDS_TYPE_RGBCOLOUR_LENGTH__ID
// };

/***
 * Palette ideas to add
 * 
 * */

/**
 * Red, Green, Blue, Yellow(255,255,0), Purple(0,255,255)
 * */


    // Predefined RGB colors
    typedef enum {
        AliceBlue=0xF0F8FF,
        Amethyst=0x9966CC,
        AntiqueWhite=0xFAEBD7,
        Aqua=0x00FFFF,
        Aquamarine=0x7FFFD4,
        Azure=0xF0FFFF,
        Beige=0xF5F5DC,
        Bisque=0xFFE4C4,
        Black=0x000000,
        BlanchedAlmond=0xFFEBCD,
        Blue=0x0000FF,
        BlueViolet=0x8A2BE2,
        Brown=0xA52A2A,
        BurlyWood=0xDEB887,
        CadetBlue=0x5F9EA0,
        Chartreuse=0x7FFF00,
        Chocolate=0xD2691E,
        Coral=0xFF7F50,
        CornflowerBlue=0x6495ED,
        Cornsilk=0xFFF8DC,
        Crimson=0xDC143C,
        Cyan=0x00FFFF,
        DarkBlue=0x00008B,
        DarkCyan=0x008B8B,
        DarkGoldenrod=0xB8860B,
        DarkGray=0xA9A9A9,
        DarkGrey=0xA9A9A9,
        DarkGreen=0x006400,
        DarkKhaki=0xBDB76B,
        DarkMagenta=0x8B008B,
        DarkOliveGreen=0x556B2F,
        DarkOrange=0xFF8C00,
        DarkOrchid=0x9932CC,
        DarkRed=0x8B0000,
        DarkSalmon=0xE9967A,
        DarkSeaGreen=0x8FBC8F,
        DarkSlateBlue=0x483D8B,
        DarkSlateGray=0x2F4F4F,
        DarkSlateGrey=0x2F4F4F,
        DarkTurquoise=0x00CED1,
        DarkViolet=0x9400D3,
        DeepPink=0xFF1493,
        DeepSkyBlue=0x00BFFF,
        DimGray=0x696969,
        DimGrey=0x696969,
        DodgerBlue=0x1E90FF,
        FireBrick=0xB22222,
        FloralWhite=0xFFFAF0,
        ForestGreen=0x228B22,
        Fuchsia=0xFF00FF,
        Gainsboro=0xDCDCDC,
        GhostWhite=0xF8F8FF,
        Gold=0xFFD700,
        Goldenrod=0xDAA520,
        Gray=0x808080,
        Grey=0x808080,
        Green=0x008000,
        GreenYellow=0xADFF2F,
        Honeydew=0xF0FFF0,
        HotPink=0xFF69B4,
        IndianRed=0xCD5C5C,
        Indigo=0x4B0082,
        Ivory=0xFFFFF0,
        Khaki=0xF0E68C,
        Lavender=0xE6E6FA,
        LavenderBlush=0xFFF0F5,
        LawnGreen=0x7CFC00,
        LemonChiffon=0xFFFACD,
        LightBlue=0xADD8E6,
        LightCoral=0xF08080,
        LightCyan=0xE0FFFF,
        LightGoldenrodYellow=0xFAFAD2,
        LightGreen=0x90EE90,
        LightGrey=0xD3D3D3,
        LightPink=0xFFB6C1,
        LightSalmon=0xFFA07A,
        LightSeaGreen=0x20B2AA,
        LightSkyBlue=0x87CEFA,
        LightSlateGray=0x778899,
        LightSlateGrey=0x778899,
        LightSteelBlue=0xB0C4DE,
        LightYellow=0xFFFFE0,
        Lime=0x00FF00,
        LimeGreen=0x32CD32,
        Linen=0xFAF0E6,
        Magenta=0xFF00FF,
        Maroon=0x800000,
        MediumAquamarine=0x66CDAA,
        MediumBlue=0x0000CD,
        MediumOrchid=0xBA55D3,
        MediumPurple=0x9370DB,
        MediumSeaGreen=0x3CB371,
        MediumSlateBlue=0x7B68EE,
        MediumSpringGreen=0x00FA9A,
        MediumTurquoise=0x48D1CC,
        MediumVioletRed=0xC71585,
        MidnightBlue=0x191970,
        MintCream=0xF5FFFA,
        MistyRose=0xFFE4E1,
        Moccasin=0xFFE4B5,
        NavajoWhite=0xFFDEAD,
        Navy=0x000080,
        OldLace=0xFDF5E6,
        Olive=0x808000,
        OliveDrab=0x6B8E23,
        Orange=0xFFA500,
        OrangeRed=0xFF4500,
        Orchid=0xDA70D6,
        PaleGoldenrod=0xEEE8AA,
        PaleGreen=0x98FB98,
        PaleTurquoise=0xAFEEEE,
        PaleVioletRed=0xDB7093,
        PapayaWhip=0xFFEFD5,
        PeachPuff=0xFFDAB9,
        Peru=0xCD853F,
        Pink=0xFFC0CB,
        Plaid=0xCC5533,
        Plum=0xDDA0DD,
        PowderBlue=0xB0E0E6,
        Purple=0x800080,
        Red=0xFF0000,
        RosyBrown=0xBC8F8F,
        RoyalBlue=0x4169E1,
        SaddleBrown=0x8B4513,
        Salmon=0xFA8072,
        SandyBrown=0xF4A460,
        SeaGreen=0x2E8B57,
        Seashell=0xFFF5EE,
        Sienna=0xA0522D,
        Silver=0xC0C0C0,
        SkyBlue=0x87CEEB,
        SlateBlue=0x6A5ACD,
        SlateGray=0x708090,
        SlateGrey=0x708090,
        Snow=0xFFFAFA,
        SpringGreen=0x00FF7F,
        SteelBlue=0x4682B4,
        Tan=0xD2B48C,
        Teal=0x008080,
        Thistle=0xD8BFD8,
        Tomato=0xFF6347,
        Turquoise=0x40E0D0,
        Violet=0xEE82EE,
        Wheat=0xF5DEB3,
        White=0xFFFFFF,
        WhiteSmoke=0xF5F5F5,
        Yellow=0xFFFF00,
        YellowGreen=0x9ACD32,

        // LED RGB color that roughly approximates
        // the color of incandescent fairy lights,
        // assuming that you're using FastLED
        // color correction on your LEDs (recommended).
        FairyLight=0xFFE42D,
        // If you are using no color correction, use this
        FairyLightNCC=0xFF9D2A

    } HTMLColorCode;

    /** List of generic colours I may use to build rgb palettes with
     * Key words
     * FADED is colours at lower brightness levels
     * PASTEL is a lower saturation colour (more cold white)
     * WARM is colours with slightly reduce saturation, so they are not exact wavelength (more warm white)
     * DEEP is a rich colour tone eg a "more" orange, orange
     * ALT is alternate colour, looking aged for slight variance in the colours
     * ###2, ###3 is increasing levels of the above effects, ie more faded, warmer
     */
    #define D_RGB255_RED             255, 0, 0,
    #define D_RGB255_RED_PASTEL      236, 19, 19,
    #define D_RGB255_RED_ALT         255, 5, 5,
    #define D_RGB255_ORANGE_LIGHT    255, 60, 0,  
    #define D_RGB255_ORANGE_DEEP     252, 44, 3, 
    #define D_RGB255_YELLOW_WARM     252, 157, 3,
    #define D_RGB255_GREEN           0, 255, 0,
    #define D_RGB255_GREEN_FADED2_2  0, 200, 0,
    #define D_RGB255_GREEN_FADED     0, 150, 0,
    #define D_RGB255_GREEN_FADED2    0, 100, 0,
    #define D_RGB255_GREEN_FADED3    0, 80, 0,
    #define D_RGB255_GREEN_PASTEL    25, 112, 25,
    #define D_RGB255_GREEN_PASTEL_FADED    15, 90, 15,

    #define D_RGB255_CYAN            0, 255, 255,
    #define D_RGB255_CYAN_FADED2      0, 100, 100,
    #define D_RGB255_CYAN_FADED3      0, 60, 60,
    #define D_RGB255_CYAN_FADED4      0, 30, 30,


    #define D_RGB255_BLUE            0, 0, 255,
    #define D_RGB255_BLUE_FADED      0, 0, 200,
    #define D_RGB255_BLUE_FADED2     0, 0, 150,
    #define D_RGB255_BLUE_FADED3     0, 0, 100,


    #define D_RGB255_PURPLE_BLUE     120, 16, 144,
    #define D_RGB255_PURPLE_RED      255, 16, 44,


    #define D_RGB255_PURPLE_DEEP     21,0,26,

    #define D_RGB255_PINK_WARM       255, 26, 64,
    #define D_RGB255_PINK_HOT        255, 16, 44,
    #define D_RGB255_PINK_HOT2       255, 0, 72,
    #define D_RGB255_PINK_HOT3       255, 0, 43,
    #define D_RGB255_PINK            255, 0, 200,

    #define D_RGB255_BLACK      0, 0, 0,

    #define D_RGB255_WHITE          255, 255, 255,
    #define D_RGB255_WHITE_FADED    150, 150, 150,
    #define D_RGB255_WHITE_WARM     0xFF,0x52,0x18,//255,200,0,

    #define D_RGB255_WHITE_WARM1    255, 140, 26,

    /**
     * Warm white looks different on different string types, so these are visually confirmed by myself (personal perference)
     * 
     */
    #define D_RGB255_WHITE_WARM_MICROLEDS_50SET    255, 140, 26,



    #define COLOUR_FLOAT_HUE_RED            0.0000f //0
    #define COLOUR_FLOAT_HUE_ORANGE         0.0389f //14
    #define COLOUR_FLOAT_HUE_LIGHTORANGE    0.0556f //20
    #define COLOUR_FLOAT_HUE_WHITE          0.0833f //30
    #define COLOUR_FLOAT_HUE_YELLOW         0.1389f //50
    #define COLOUR_FLOAT_HUE_LIMEGREEN      0.2778f //100
    #define COLOUR_FLOAT_HUE_GREEN          120.0f/360.0f
    #define COLOUR_FLOAT_HUE_CYAN           180.0f/360.0f
    #define COLOUR_FLOAT_HUE_BLUE           240.0f/360.0f
    #define COLOUR_FLOAT_HUE_BLUEPURPLE     280.0f/360.0f
    #define COLOUR_FLOAT_HUE_PURPLE         300.0f/360.0f
    #define COLOUR_FLOAT_HUE_PINKE          340.0f/360.0f
    #define COLOUR_FLOAT_HUE_HOTPINK        350.0f/360.0f
    
    const float colour_float_map_values[] PROGMEM = {
        COLOUR_FLOAT_HUE_RED,
        COLOUR_FLOAT_HUE_ORANGE,
        COLOUR_FLOAT_HUE_LIGHTORANGE,
        COLOUR_FLOAT_HUE_WHITE,
        COLOUR_FLOAT_HUE_YELLOW,
        COLOUR_FLOAT_HUE_LIMEGREEN,
        COLOUR_FLOAT_HUE_GREEN,
        COLOUR_FLOAT_HUE_CYAN,
        COLOUR_FLOAT_HUE_BLUE,
        COLOUR_FLOAT_HUE_BLUEPURPLE,
        COLOUR_FLOAT_HUE_PURPLE,
        COLOUR_FLOAT_HUE_PINKE,
        COLOUR_FLOAT_HUE_HOTPINK
    };

    
    enum PRESET_COLOUR_MAP__IDS{
      // Red
      COLOUR_MAP_RED__ID=0,
      COLOUR_MAP_RED_SATURATION95__ID,
      COLOUR_MAP_RED_SATURATION90__ID,
      COLOUR_MAP_RED_SATURATION80__ID,
      COLOUR_MAP_RED_SATURATION70__ID,
      COLOUR_MAP_RED_SATURATION60__ID,
      COLOUR_MAP_RED_SATURATION50__ID,
      COLOUR_MAP_RED_SATURATION40__ID,
      COLOUR_MAP_RED_SATURATION30__ID,
      COLOUR_MAP_RED_SATURATION20__ID,
      COLOUR_MAP_RED_SATURATION10__ID,
      COLOUR_MAP_RED_SATURATION0__ID,
      // Orange
      COLOUR_MAP_ORANGE__ID,
      COLOUR_MAP_ORANGE_SATURATION95__ID,
      COLOUR_MAP_ORANGE_SATURATION90__ID,
      COLOUR_MAP_ORANGE_SATURATION80__ID,
      COLOUR_MAP_ORANGE_SATURATION70__ID,
      COLOUR_MAP_ORANGE_SATURATION60__ID,
      COLOUR_MAP_ORANGE_SATURATION50__ID,
      COLOUR_MAP_ORANGE_SATURATION40__ID,
      COLOUR_MAP_ORANGE_SATURATION30__ID,
      COLOUR_MAP_ORANGE_SATURATION20__ID,
      COLOUR_MAP_ORANGE_SATURATION10__ID,
      COLOUR_MAP_ORANGE_SATURATION0__ID,
      // Light Orange
      COLOUR_MAP_LIGHTORANGE__ID,
      COLOUR_MAP_LIGHTORANGE_SATURATION95__ID,
      COLOUR_MAP_LIGHTORANGE_SATURATION90__ID,
      COLOUR_MAP_LIGHTORANGE_SATURATION80__ID,
      COLOUR_MAP_LIGHTORANGE_SATURATION70__ID,
      COLOUR_MAP_LIGHTORANGE_SATURATION60__ID,
      COLOUR_MAP_LIGHTORANGE_SATURATION50__ID,
      COLOUR_MAP_LIGHTORANGE_SATURATION40__ID,
      COLOUR_MAP_LIGHTORANGE_SATURATION30__ID,
      COLOUR_MAP_LIGHTORANGE_SATURATION20__ID,
      COLOUR_MAP_LIGHTORANGE_SATURATION10__ID,
      COLOUR_MAP_LIGHTORANGE_SATURATION0__ID,
      // White
      COLOUR_MAP_WHITE__ID,
      COLOUR_MAP_WHITE_SATURATION95__ID,
      COLOUR_MAP_WHITE_SATURATION90__ID,
      COLOUR_MAP_WHITE_SATURATION80__ID,
      COLOUR_MAP_WHITE_SATURATION70__ID,
      COLOUR_MAP_WHITE_SATURATION60__ID,
      COLOUR_MAP_WHITE_SATURATION50__ID,
      COLOUR_MAP_WHITE_SATURATION40__ID,
      COLOUR_MAP_WHITE_SATURATION30__ID,
      COLOUR_MAP_WHITE_SATURATION20__ID,
      COLOUR_MAP_WHITE_SATURATION10__ID,
      COLOUR_MAP_WHITE_SATURATION0__ID,
      // 
      COLOUR_MAP_YELLOW__ID,
      COLOUR_MAP_YELLOW_SATURATION95__ID,
      COLOUR_MAP_YELLOW_SATURATION90__ID,
      COLOUR_MAP_YELLOW_SATURATION80__ID,
      COLOUR_MAP_YELLOW_SATURATION70__ID,
      COLOUR_MAP_YELLOW_SATURATION60__ID,
      COLOUR_MAP_YELLOW_SATURATION50__ID,
      COLOUR_MAP_YELLOW_SATURATION40__ID,
      COLOUR_MAP_YELLOW_SATURATION30__ID,
      COLOUR_MAP_YELLOW_SATURATION20__ID,
      COLOUR_MAP_YELLOW_SATURATION10__ID,
      COLOUR_MAP_YELLOW_SATURATION0__ID,
      // 
      COLOUR_MAP_LIMEGREEN__ID,
      COLOUR_MAP_LIMEGREEN_SATURATION95__ID,
      COLOUR_MAP_LIMEGREEN_SATURATION90__ID,
      COLOUR_MAP_LIMEGREEN_SATURATION80__ID,
      COLOUR_MAP_LIMEGREEN_SATURATION70__ID,
      COLOUR_MAP_LIMEGREEN_SATURATION60__ID,
      COLOUR_MAP_LIMEGREEN_SATURATION50__ID,
      COLOUR_MAP_LIMEGREEN_SATURATION40__ID,
      COLOUR_MAP_LIMEGREEN_SATURATION30__ID,
      COLOUR_MAP_LIMEGREEN_SATURATION20__ID,
      COLOUR_MAP_LIMEGREEN_SATURATION10__ID,
      COLOUR_MAP_LIMEGREEN_SATURATION0__ID,
      // 
      COLOUR_MAP_GREEN__ID,
      COLOUR_MAP_GREEN_SATURATION95__ID,
      COLOUR_MAP_GREEN_SATURATION90__ID,
      COLOUR_MAP_GREEN_SATURATION80__ID,
      COLOUR_MAP_GREEN_SATURATION70__ID,
      COLOUR_MAP_GREEN_SATURATION60__ID,
      COLOUR_MAP_GREEN_SATURATION50__ID,
      COLOUR_MAP_GREEN_SATURATION40__ID,
      COLOUR_MAP_GREEN_SATURATION30__ID,
      COLOUR_MAP_GREEN_SATURATION20__ID,
      COLOUR_MAP_GREEN_SATURATION10__ID,
      COLOUR_MAP_GREEN_SATURATION0__ID,
      // Cyan
      COLOUR_MAP_CYAN__ID,
      COLOUR_MAP_CYAN_SATURATION95__ID,
      COLOUR_MAP_CYAN_SATURATION90__ID,
      COLOUR_MAP_CYAN_SATURATION80__ID,
      COLOUR_MAP_CYAN_SATURATION70__ID,
      COLOUR_MAP_CYAN_SATURATION60__ID,
      COLOUR_MAP_CYAN_SATURATION50__ID,
      COLOUR_MAP_CYAN_SATURATION40__ID,
      COLOUR_MAP_CYAN_SATURATION30__ID,
      COLOUR_MAP_CYAN_SATURATION20__ID,
      COLOUR_MAP_CYAN_SATURATION10__ID,
      COLOUR_MAP_CYAN_SATURATION0__ID,
      // Blue
      COLOUR_MAP_BLUE__ID,
      COLOUR_MAP_BLUE_SATURATION95__ID,
      COLOUR_MAP_BLUE_SATURATION90__ID,
      COLOUR_MAP_BLUE_SATURATION80__ID,
      COLOUR_MAP_BLUE_SATURATION70__ID,
      COLOUR_MAP_BLUE_SATURATION60__ID,
      COLOUR_MAP_BLUE_SATURATION50__ID,
      COLOUR_MAP_BLUE_SATURATION40__ID,
      COLOUR_MAP_BLUE_SATURATION30__ID,
      COLOUR_MAP_BLUE_SATURATION20__ID,
      COLOUR_MAP_BLUE_SATURATION10__ID,
      COLOUR_MAP_BLUE_SATURATION0__ID,
      // Blue Purple
      COLOUR_MAP_BLUEPURPLE__ID,
      COLOUR_MAP_BLUEPURPLE_SATURATION95__ID,
      COLOUR_MAP_BLUEPURPLE_SATURATION90__ID,
      COLOUR_MAP_BLUEPURPLE_SATURATION80__ID,
      COLOUR_MAP_BLUEPURPLE_SATURATION70__ID,
      COLOUR_MAP_BLUEPURPLE_SATURATION60__ID,
      COLOUR_MAP_BLUEPURPLE_SATURATION50__ID,
      COLOUR_MAP_BLUEPURPLE_SATURATION40__ID,
      COLOUR_MAP_BLUEPURPLE_SATURATION30__ID,
      COLOUR_MAP_BLUEPURPLE_SATURATION20__ID,
      COLOUR_MAP_BLUEPURPLE_SATURATION10__ID,
      COLOUR_MAP_BLUEPURPLE_SATURATION0__ID,
      // Purple
      COLOUR_MAP_PURPLE__ID,
      COLOUR_MAP_PURPLE_SATURATION95__ID,
      COLOUR_MAP_PURPLE_SATURATION90__ID,
      COLOUR_MAP_PURPLE_SATURATION80__ID,
      COLOUR_MAP_PURPLE_SATURATION70__ID,
      COLOUR_MAP_PURPLE_SATURATION60__ID,
      COLOUR_MAP_PURPLE_SATURATION50__ID,
      COLOUR_MAP_PURPLE_SATURATION40__ID,
      COLOUR_MAP_PURPLE_SATURATION30__ID,
      COLOUR_MAP_PURPLE_SATURATION20__ID,
      COLOUR_MAP_PURPLE_SATURATION10__ID,
      COLOUR_MAP_PURPLE_SATURATION0__ID,
      // Pink
      COLOUR_MAP_PINK__ID,
      COLOUR_MAP_PINK_SATURATION95__ID,
      COLOUR_MAP_PINK_SATURATION90__ID,
      COLOUR_MAP_PINK_SATURATION80__ID,
      COLOUR_MAP_PINK_SATURATION70__ID,
      COLOUR_MAP_PINK_SATURATION60__ID,
      COLOUR_MAP_PINK_SATURATION50__ID,
      COLOUR_MAP_PINK_SATURATION40__ID,
      COLOUR_MAP_PINK_SATURATION30__ID,
      COLOUR_MAP_PINK_SATURATION20__ID,
      COLOUR_MAP_PINK_SATURATION10__ID,
      COLOUR_MAP_PINK_SATURATION0__ID,
      // Hot Pink
      COLOUR_MAP_HOTPINK__ID,
      COLOUR_MAP_HOTPINK_SATURATION95__ID,
      COLOUR_MAP_HOTPINK_SATURATION90__ID,
      COLOUR_MAP_HOTPINK_SATURATION80__ID,
      COLOUR_MAP_HOTPINK_SATURATION70__ID,
      COLOUR_MAP_HOTPINK_SATURATION60__ID,
      COLOUR_MAP_HOTPINK_SATURATION50__ID,
      COLOUR_MAP_HOTPINK_SATURATION40__ID,
      COLOUR_MAP_HOTPINK_SATURATION30__ID,
      COLOUR_MAP_HOTPINK_SATURATION20__ID,
      COLOUR_MAP_HOTPINK_SATURATION10__ID,
      COLOUR_MAP_HOTPINK_SATURATION0__ID,
      // End
      COLOUR_MAP_NONE__ID,
      //
      COLOUR_MAP_LENGTH__ID, //new end
      COLOUR_MAP_BUFFER_DELIMETER__ID=255
    };

/***
 * Allow more customisable palettes in the esp32
 * Also, can these be combined and stored as one buffer
 * Add that first pixel contains palette type? the type of encoding, yes do, this
 * must , COLOUR_MAP_NONE__ID finish with none, this is now length
 * */

  const uint8_t DEFAULT_COLOUR_PALETTE_HSBID_01__IDS[] PROGMEM = {
    COLOUR_MAP_RED__ID, COLOUR_MAP_GREEN__ID, COLOUR_MAP_PINK__ID, COLOUR_MAP_BLUE__ID, COLOUR_MAP_ORANGE__ID, 
    COLOUR_MAP_NONE__ID
  }; 
  const uint8_t DEFAULT_COLOUR_PALETTE_HSBID_02__IDS[] PROGMEM = {
    COLOUR_MAP_YELLOW__ID, COLOUR_MAP_RED__ID, COLOUR_MAP_ORANGE__ID, COLOUR_MAP_GREEN__ID, COLOUR_MAP_PINK__ID, COLOUR_MAP_BLUE__ID, 
    COLOUR_MAP_NONE__ID
  }; 
  const uint8_t DEFAULT_COLOUR_PALETTE_HSBID_03__IDS[] PROGMEM {
    COLOUR_MAP_GREEN__ID, COLOUR_MAP_PINK__ID, COLOUR_MAP_ORANGE__ID, COLOUR_MAP_BLUE__ID, COLOUR_MAP_YELLOW__ID, COLOUR_MAP_PURPLE__ID, COLOUR_MAP_RED__ID, 
    COLOUR_MAP_NONE__ID
  }; 
  const uint8_t DEFAULT_COLOUR_PALETTE_HSBID_04__IDS[] PROGMEM {
    COLOUR_MAP_PURPLE_SATURATION95__ID, COLOUR_MAP_HOTPINK__ID, COLOUR_MAP_ORANGE_SATURATION90__ID, COLOUR_MAP_BLUE_SATURATION95__ID, COLOUR_MAP_LIMEGREEN_SATURATION95__ID,
    COLOUR_MAP_PURPLE__ID, COLOUR_MAP_HOTPINK_SATURATION95__ID, COLOUR_MAP_CYAN__ID, COLOUR_MAP_PURPLE__ID, COLOUR_MAP_BLUE_SATURATION80__ID, 
    COLOUR_MAP_YELLOW_SATURATION95__ID, 
    COLOUR_MAP_NONE__ID
  }; 
  const uint8_t DEFAULT_COLOUR_PALETTE_HSBID_05__IDS[] PROGMEM {
    COLOUR_MAP_YELLOW__ID, COLOUR_MAP_GREEN__ID, COLOUR_MAP_PINK__ID, COLOUR_MAP_BLUE__ID, COLOUR_MAP_RED__ID, 
    COLOUR_MAP_NONE__ID
  }; 
  const uint8_t DEFAULT_COLOUR_PALETTE_HSBID_06__IDS[] PROGMEM {
    COLOUR_MAP_WHITE_SATURATION40__ID, COLOUR_MAP_GREEN__ID, COLOUR_MAP_PINK__ID, COLOUR_MAP_BLUE__ID, COLOUR_MAP_RED__ID,
    COLOUR_MAP_ORANGE__ID, COLOUR_MAP_NONE__ID
  }; 
  const uint8_t DEFAULT_COLOUR_PALETTE_HSBID_07__IDS[] PROGMEM {
    COLOUR_MAP_GREEN__ID, COLOUR_MAP_PINK__ID, COLOUR_MAP_ORANGE__ID, COLOUR_MAP_BLUE__ID, COLOUR_MAP_YELLOW__ID,
    COLOUR_MAP_PURPLE__ID, COLOUR_MAP_RED__ID, COLOUR_MAP_NONE__ID
  }; 
  const uint8_t DEFAULT_COLOUR_PALETTE_HSBID_08__IDS[] PROGMEM {
    COLOUR_MAP_WHITE_SATURATION80__ID, COLOUR_MAP_NONE__ID
  }; 
  const uint8_t DEFAULT_COLOUR_PALETTE_HSBID_09__IDS[] PROGMEM {
    COLOUR_MAP_WHITE_SATURATION20__ID, COLOUR_MAP_WHITE_SATURATION20__ID, COLOUR_MAP_CYAN_SATURATION60__ID, COLOUR_MAP_WHITE_SATURATION20__ID, COLOUR_MAP_WHITE_SATURATION70__ID,
    COLOUR_MAP_BLUE_SATURATION60__ID, COLOUR_MAP_WHITE_SATURATION70__ID, COLOUR_MAP_CYAN_SATURATION40__ID, COLOUR_MAP_WHITE_SATURATION60__ID, COLOUR_MAP_CYAN__ID, COLOUR_MAP_NONE__ID
  }; 
  const uint8_t DEFAULT_COLOUR_PALETTE_HSBID_10__IDS[] PROGMEM { // Outside tree, "few"
    COLOUR_MAP_HOTPINK__ID, COLOUR_MAP_PINK_SATURATION90__ID, COLOUR_MAP_PURPLE__ID, COLOUR_MAP_HOTPINK_SATURATION95__ID,
    COLOUR_MAP_NONE__ID
  }; 
  


  #ifndef D_PALETTE_WINTER_NAME_CTR
  #define D_PALETTE_WINTER_NAME_CTR        "Winter"   
  #endif
  const char PM_PALETTE_WINTER_NAME_CTR[] PROGMEM = D_PALETTE_WINTER_NAME_CTR;
  const uint8_t COLOUR_PALETTE_WINTER_MAP__IDS[] PROGMEM = {
    COLOUR_MAP_CYAN__ID, COLOUR_MAP_WHITE__ID, COLOUR_MAP_CYAN__ID, COLOUR_MAP_BLUE_SATURATION60__ID,
    COLOUR_MAP_WHITE__ID
  };
  
  #ifndef D_PALETTE_PASTEL_NAME_CTR
  #define D_PALETTE_PASTEL_NAME_CTR        "Pastel Easter|Pastel|Easter"   
  #endif
  const char PM_PALETTE_PASTEL_NAME_CTR[] PROGMEM = D_PALETTE_PASTEL_NAME_CTR;
  const uint8_t COLOUR_PALETTE_PASTEL_MAP__IDS[] PROGMEM = {
    COLOUR_MAP_CYAN_SATURATION80__ID, COLOUR_MAP_PINK_SATURATION80__ID, 
    COLOUR_MAP_LIGHTORANGE_SATURATION80__ID,
    COLOUR_MAP_PINK_SATURATION30__ID,
    COLOUR_MAP_GREEN_SATURATION40__ID,
    COLOUR_MAP_WHITE_SATURATION70__ID

  };

  #ifndef D_PALETTE_OCEAN_01_NAME_CTR
  #define D_PALETTE_OCEAN_01_NAME_CTR        "Ocean 01"   
  #endif
  const char PM_PALETTE_OCEAN_01_NAME_CTR[] PROGMEM = D_PALETTE_OCEAN_01_NAME_CTR;
  const uint8_t colour_map_ocean_01_id[] PROGMEM = {
    COLOUR_MAP_GREEN__ID,COLOUR_MAP_CYAN__ID,COLOUR_MAP_BLUE__ID,COLOUR_MAP_WHITE__ID
  };

  #ifndef D_PALETTE_RAINBOW_NAME_CTR
  #define D_PALETTE_RAINBOW_NAME_CTR        "Rainbow"   
  #endif          
  const char PM_PALETTE_RAINBOW_NAME_CTR[] PROGMEM = D_PALETTE_RAINBOW_NAME_CTR;
  const uint8_t colour_map_rainbow_id[] PROGMEM = {
    // to make it visually nicer, I will compress the blue region
    0,   COLOUR_MAP_RED__ID, //0
    14,  COLOUR_MAP_LIGHTORANGE__ID, //20
    35,  COLOUR_MAP_YELLOW__ID, //50
    85,  COLOUR_MAP_GREEN__ID, //120
    127,  COLOUR_MAP_CYAN__ID, //180
    170,  COLOUR_MAP_BLUE__ID, //240
    213 ,  COLOUR_MAP_PURPLE__ID, //300
    241 ,  COLOUR_MAP_PINK__ID, //340
    255, COLOUR_MAP_RED__ID //0
  };
  
  #ifndef D_PALETTE_RAINBOW_INVERTED_NAME_CTR
  #define D_PALETTE_RAINBOW_INVERTED_NAME_CTR        "Rainbow Inverted"   
  #endif          
  const char PM_PALETTE_RAINBOW_INVERTED_NAME_CTR[] PROGMEM = D_PALETTE_RAINBOW_INVERTED_NAME_CTR;
  const uint8_t colour_map_rainbow_inverted_id[] PROGMEM = {
    0,   COLOUR_MAP_BLUE__ID, //0
    14,  COLOUR_MAP_PURPLE__ID, //20
    35,  COLOUR_MAP_PINK__ID, //50
    85,  COLOUR_MAP_RED__ID, //120
    127,  COLOUR_MAP_LIGHTORANGE__ID, //180
    170,  COLOUR_MAP_YELLOW__ID, //240
    213 ,  COLOUR_MAP_GREEN__ID, //300
    241 ,  COLOUR_MAP_CYAN__ID, //340
    255, COLOUR_MAP_BLUE__ID //0
  };
  
  #ifndef D_PALETTE_AUTUMN_RED_NAME_CTR
  #define D_PALETTE_AUTUMN_RED_NAME_CTR        "Autumn Red"   
  #endif
  const char PM_PALETTE_AUTUMN_RED_NAME_CTR[] PROGMEM = D_PALETTE_AUTUMN_RED_NAME_CTR;
  const uint8_t colour_map_autumnred_id[] PROGMEM = {
    COLOUR_MAP_RED__ID,COLOUR_MAP_RED__ID,
    COLOUR_MAP_YELLOW__ID,COLOUR_MAP_YELLOW_SATURATION90__ID,
    COLOUR_MAP_ORANGE__ID
  };
  
  #ifndef D_PALETTE_AUTUMN_NAME_CTR
  #define D_PALETTE_AUTUMN_NAME_CTR        "Autumn"   
  #endif
  const char PM_PALETTE_AUTUMN_NAME_CTR[] PROGMEM = D_PALETTE_AUTUMN_NAME_CTR;
  const uint8_t colour_map_autumn_id[] PROGMEM = {
    COLOUR_MAP_RED__ID,COLOUR_MAP_GREEN_SATURATION70__ID,
    COLOUR_MAP_YELLOW__ID,COLOUR_MAP_YELLOW_SATURATION90__ID,
    COLOUR_MAP_ORANGE__ID,COLOUR_MAP_LIMEGREEN_SATURATION90__ID
  };

  
    #ifndef D_PALETTE_HOLLOWEEN_OP_NAME_CTR
    #define D_PALETTE_HOLLOWEEN_OP_NAME_CTR        "Holloween Orange-Purple"   
    #endif
    const char PM_PALETTE_HOLLOWEEN_OP_NAME_CTR[] PROGMEM = D_PALETTE_HOLLOWEEN_OP_NAME_CTR;
    const uint8_t colour_map_holloween_op_id[] PROGMEM = {
      255, 30, 0, //lihgt orange
      D_RGB255_PURPLE_BLUE //purple
    };

    #ifndef D_PALETTE_HOLLOWEEN_OGP_NAME_CTR
    #define D_PALETTE_HOLLOWEEN_OGP_NAME_CTR        "Holloween Orange-Green-Purple"   
    #endif
    const char PM_PALETTE_HOLLOWEEN_OGP_NAME_CTR[] PROGMEM = D_PALETTE_HOLLOWEEN_OGP_NAME_CTR;
    const uint8_t colour_map_holloween_ogp_id[] PROGMEM = {
      255, 30, 0, //lihgt orange
      D_RGB255_PURPLE_BLUE //purple
      20, 155, 0    //dim green
    };


/**
 * This will be changed as some template method for telling layout
 * 
 * 
 * This will be removed, and instead should be stored as a user defined palette where the "gradient" value is known as the position
 * ie pos 1,6,10,16 are heart colours
 * pos 255 is "infill the rest" as this colour
 * */
    #ifndef D_PALETTE_SHELF_HEARTS_NAME_CTR
    #define D_PALETTE_SHELF_HEARTS_NAME_CTR        "Shelf Hearts"   
    #endif
    #define WARM_WHITE_RGB_SHELF  D_RGB255_PINK_WARM
    #define WARM_WHITE_RGB_SHELF_HEART  D_RGB255_PURPLE_BLUE
    const char PM_PALETTE_SHELF_HEARTS_NAME_CTR[] PROGMEM = D_PALETTE_SHELF_HEARTS_NAME_CTR;
    const uint8_t colour_map_shelf_hearts_id[] PROGMEM = { //position/colour pairs {pos0,col0, pos1,col1, pos2,col2, ... pos255,col} ie set all
      WARM_WHITE_RGB_SHELF
      WARM_WHITE_RGB_SHELF_HEART //1
      WARM_WHITE_RGB_SHELF
      WARM_WHITE_RGB_SHELF
      WARM_WHITE_RGB_SHELF
      WARM_WHITE_RGB_SHELF
      WARM_WHITE_RGB_SHELF_HEART //6
      WARM_WHITE_RGB_SHELF
      WARM_WHITE_RGB_SHELF
      WARM_WHITE_RGB_SHELF
      WARM_WHITE_RGB_SHELF_HEART //10
      WARM_WHITE_RGB_SHELF
      WARM_WHITE_RGB_SHELF
      WARM_WHITE_RGB_SHELF
      WARM_WHITE_RGB_SHELF
      WARM_WHITE_RGB_SHELF
      WARM_WHITE_RGB_SHELF_HEART //16
      WARM_WHITE_RGB_SHELF
      WARM_WHITE_RGB_SHELF
      WARM_WHITE_RGB_SHELF
      WARM_WHITE_RGB_SHELF
      WARM_WHITE_RGB_SHELF_HEART //21
      WARM_WHITE_RGB_SHELF
      WARM_WHITE_RGB_SHELF
      WARM_WHITE_RGB_SHELF
      WARM_WHITE_RGB_SHELF_HEART //25
      WARM_WHITE_RGB_SHELF
      WARM_WHITE_RGB_SHELF
      WARM_WHITE_RGB_SHELF
      WARM_WHITE_RGB_SHELF
      WARM_WHITE_RGB_SHELF
      WARM_WHITE_RGB_SHELF_HEART //31
      WARM_WHITE_RGB_SHELF
      WARM_WHITE_RGB_SHELF
    };

    // optional gradient positions to linearbelnd between, first position is percentage    

    // lets remove the "as a percentage" option, it offers no benefit and wastes precision
    #ifndef D_PALETTE_GRADIENT_01_NAME_CTR
    #define D_PALETTE_GRADIENT_01_NAME_CTR        "Gradient 01 - Compressed Rainbow"   
    #endif
    DEF_PGM_CTR  (PM_PALETTE_GRADIENT_01_NAME_CTR) D_PALETTE_GRADIENT_01_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_GRADIENT_01_COLOUR_MAP__IDS) 
    { // pos%, HSB id
      0,   COLOUR_MAP_RED__ID,
      20,  COLOUR_MAP_ORANGE__ID,
      35,  COLOUR_MAP_GREEN__ID,
      50,  COLOUR_MAP_CYAN__ID,
      65,  COLOUR_MAP_BLUE__ID,
      80,  COLOUR_MAP_PINK__ID,
      255, COLOUR_MAP_HOTPINK__ID
    };

    #ifndef D_PALETTE_GRADIENT_02_NAME_CTR
    #define D_PALETTE_GRADIENT_02_NAME_CTR        "Gradient 02"   
    #endif
    DEF_PGM_CTR  (PM_PALETTE_GRADIENT_02_NAME_CTR)        D_PALETTE_GRADIENT_02_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_GRADIENT_02_COLOUR_MAP__IDS)
    { // pos%,R,G,B
      0,  26,  1,  1,
      51,  67,  4,  1,
      84, 118, 14,  1,
      104, 137,152, 52,
      112, 113, 65,  1,
      122, 133,149, 59,
      124, 137,152, 52,
      135, 113, 65,  1,
      142, 139,154, 46,
      163, 113, 13,  1,
      204,  55,  3,  1,
      249,  17,  1,  1,
      255,  17,  1,  1
    };

    #ifndef D_PALETTE_BERRY_GREEN_NAME_CTR
    #define D_PALETTE_BERRY_GREEN_NAME_CTR        "Berry Green"   
    #endif
    DEF_PGM_CTR  (PM_PALETTE_BERRY_GREEN_NAME_CTR)        D_PALETTE_BERRY_GREEN_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_BERRY_GREEN_COLOUR_MAP__IDS)
    { // pos%,R,G,B
      D_RGB255_RED
      0, 40, 0,
      0, 40, 0,
      0, 40, 0
    };

    /**
     * Based on chrimstas light colours * 
     * 
     * */

    /**
     * @note: Traditional lights with orange
     * */
    #ifndef D_PALETTE_CHRISTMAS_01_NAME_CTR
    #define D_PALETTE_CHRISTMAS_01_NAME_CTR        "Christmas Traditional - RGPBO|Christmas 01"   
    #endif    
    DEF_PGM_CTR  (PM_PALETTE_CHRISTMAS_01_NAME_CTR)        D_PALETTE_CHRISTMAS_01_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_CHRISTMAS_01_COLOUR_MAP__IDS)
    { // R,G,B
      D_RGB255_RED
      D_RGB255_GREEN
      D_RGB255_PINK_HOT
      D_RGB255_BLUE_FADED3
      D_RGB255_ORANGE_LIGHT
    };

    /**
     * @note: Traditional lights with yellow
     * */
    #ifndef D_PALETTE_CHRISTMAS_02_NAME_CTR
    #define D_PALETTE_CHRISTMAS_02_NAME_CTR        "Christmas Traditional - RGPBY|Christmas 02"   
    #endif
    DEF_PGM_CTR  (PM_PALETTE_CHRISTMAS_02_NAME_CTR)        D_PALETTE_CHRISTMAS_02_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_CHRISTMAS_02_COLOUR_MAP__IDS)
    { // R,G,B
      D_RGB255_RED
      D_RGB255_GREEN
      D_RGB255_PINK_HOT
      D_RGB255_BLUE_FADED3
      D_RGB255_YELLOW_WARM
    };

    /**
     * @note: Traditional lights
     * */
    #ifndef D_PALETTE_CHRISTMAS_03_NAME_CTR
    #define D_PALETTE_CHRISTMAS_03_NAME_CTR        "Christmas Traditional - ROGPBY|Christmas 03"   
    #endif
    DEF_PGM_CTR  (PM_PALETTE_CHRISTMAS_03_NAME_CTR)        D_PALETTE_CHRISTMAS_03_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_CHRISTMAS_03_COLOUR_MAP__IDS)
    { // R,G,B
      D_RGB255_RED
      D_RGB255_ORANGE_LIGHT
      D_RGB255_GREEN
      D_RGB255_PINK_HOT
      D_RGB255_BLUE_FADED3
      D_RGB255_YELLOW_WARM
    };



    #ifndef D_PALETTE_CHRISTMAS_04_NAME_CTR
    #define D_PALETTE_CHRISTMAS_04_NAME_CTR        "Christmas Pifco MiniBells|Christmas 04"// GPYpBPp"   
    #endif
    DEF_PGM_CTR  (PM_PALETTE_CHRISTMAS_04_NAME_CTR)        D_PALETTE_CHRISTMAS_04_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_CHRISTMAS_04_COLOUR_MAP__IDS)
    { // R,G,B
      D_RGB255_RED
      D_RGB255_GREEN
      D_RGB255_PINK
      D_RGB255_BLUE
      255, 213, 0,     //COLOUR_FLOAT_HUE_YELLOW       
      255, 60, 50     //COLOUR_FLOAT_HUE_ORANGE         
    };



    #ifndef D_PALETTE_CHRISTMAS_05_NAME_CTR
    #define D_PALETTE_CHRISTMAS_05_NAME_CTR        "Christmas Multi Warmer Less Green|Christmas 05"//"Christmas Multi with Warm White"   
    #endif
    DEF_PGM_CTR  (PM_PALETTE_CHRISTMAS_05_NAME_CTR)        D_PALETTE_CHRISTMAS_05_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_CHRISTMAS_05_COLOUR_MAP__IDS)
    { // R,G,B
      D_RGB255_RED
      D_RGB255_GREEN_FADED2
      D_RGB255_PINK_WARM
      D_RGB255_BLUE_FADED3
      D_RGB255_ORANGE_LIGHT       
      D_RGB255_GREEN_PASTEL_FADED
      D_RGB255_CYAN_FADED3
      D_RGB255_PINK_HOT
      D_RGB255_RED_PASTEL
      D_RGB255_ORANGE_DEEP
      D_RGB255_YELLOW_WARM
      // Repeated with tweaks
      D_RGB255_RED_ALT
      D_RGB255_GREEN_FADED2
      D_RGB255_PINK_WARM
      D_RGB255_BLUE_FADED3
      D_RGB255_ORANGE_LIGHT   
      // D_RGB255_GREEN_PASTEL_FADED
      D_RGB255_PINK_HOT2
      D_RGB255_CYAN_FADED3
      D_RGB255_RED_PASTEL
      D_RGB255_ORANGE_DEEP
      D_RGB255_YELLOW_WARM
    };





    #ifndef D_PALETTE_CHRISTMAS_06_NAME_CTR
    #define D_PALETTE_CHRISTMAS_06_NAME_CTR                       "Coloured MultiColoured Warmer|Christmas MultiColoured Warmer|Christmas 06"   
    #endif
    #define        D_PALETTE_CHRISTMAS_06_COLOUR_MAP__ID_ENCODING  MAPIDS_TYPE_RGBCOLOUR_NOINDEX__ID
    DEF_PGM_CTR  (PM_PALETTE_CHRISTMAS_06_NAME_CTR)               D_PALETTE_CHRISTMAS_06_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_CHRISTMAS_06_COLOUR_MAP__IDS)
    { // R,G,B       
      D_RGB255_RED
      D_RGB255_GREEN_FADED2
      D_RGB255_PINK_WARM
      D_RGB255_BLUE_FADED3
      D_RGB255_ORANGE_LIGHT       
      D_RGB255_GREEN_PASTEL_FADED
      D_RGB255_CYAN_FADED3
      D_RGB255_PINK_HOT
      D_RGB255_RED_PASTEL
      D_RGB255_ORANGE_DEEP
      D_RGB255_YELLOW_WARM
      // Repeated with tweaks
      D_RGB255_RED_ALT
      D_RGB255_GREEN_FADED2
      D_RGB255_PINK_WARM
      D_RGB255_BLUE_FADED3
      D_RGB255_ORANGE_LIGHT   
      D_RGB255_GREEN_PASTEL_FADED
      D_RGB255_PINK_HOT2
      D_RGB255_CYAN_FADED3
      D_RGB255_RED_PASTEL
      D_RGB255_ORANGE_DEEP
      D_RGB255_YELLOW_WARM
    };


    #ifndef D_PALETTE_CHRISTMAS_07_NAME_CTR
    #define D_PALETTE_CHRISTMAS_07_NAME_CTR        "Solid Warm White|Christmas Warm White|Christmas 07"   
    #endif
    DEF_PGM_CTR  (PM_PALETTE_CHRISTMAS_07_NAME_CTR)        D_PALETTE_CHRISTMAS_07_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_CHRISTMAS_07_COLOUR_MAP__IDS)
    { // R,G,B
      255, 140, 26 // Warm White         
    };

    #ifndef D_PALETTE_CHRISTMAS_08_NAME_CTR
    #define D_PALETTE_CHRISTMAS_08_NAME_CTR        "Christmas Cold White|Christmas 08"   
    #endif
    DEF_PGM_CTR  (PM_PALETTE_CHRISTMAS_08_NAME_CTR)        D_PALETTE_CHRISTMAS_08_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_CHRISTMAS_08_COLOUR_MAP__IDS)
    { // R,G,B
      255, 255, 255
    };




    #ifndef D_PALETTE_CHRISTMAS_09_NAME_CTR
    #define D_PALETTE_CHRISTMAS_09_NAME_CTR        "Christmas Outside Tree Few Multi|Christmas 09"   
    #endif
    DEF_PGM_CTR  (PM_PALETTE_CHRISTMAS_09_NAME_CTR)        D_PALETTE_CHRISTMAS_09_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_CHRISTMAS_09_COLOUR_MAP__IDS)
    { // R,G,B
      D_RGB255_RED
      D_RGB255_GREEN_FADED
      D_RGB255_PINK_WARM
      D_RGB255_BLUE
      D_RGB255_ORANGE_LIGHT     //COLOUR_FLOAT_HUE_ORANGE 

      //test with dimmer colours
      D_RGB255_RED
      D_RGB255_GREEN
      D_RGB255_PINK_WARM
      0, 0, 150,
      D_RGB255_ORANGE_LIGHT     //COLOUR_FLOAT_HUE_ORANGE 

      //test with dimmer colours
      D_RGB255_RED
      0, 155, 0,
      D_RGB255_PINK_WARM
      0, 0, 200,
      255, 67, 10     //COLOUR_FLOAT_HUE_ORANGE 

      
      




//use my phone, take pictures of real christmas lights, then use colour picker in photo shop to get rgb!
      
    };




    #ifndef D_PALETTE_CHRISTMAS_10_NAME_CTR
    #define D_PALETTE_CHRISTMAS_10_NAME_CTR        "Christmas Outside Tree Many Multi|Christmas 10"   
    #endif
    DEF_PGM_CTR  (PM_PALETTE_CHRISTMAS_10_NAME_CTR)        D_PALETTE_CHRISTMAS_10_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_CHRISTMAS_10_COLOUR_MAP__IDS)
    { // R,G,B
      D_RGB255_RED
      D_RGB255_GREEN_FADED
      D_RGB255_PINK_WARM
      D_RGB255_BLUE
      D_RGB255_ORANGE_LIGHT     //COLOUR_FLOAT_HUE_ORANGE 

      //test with dimmer colours
      D_RGB255_RED
      D_RGB255_GREEN
      D_RGB255_PINK_WARM
      0, 0, 150,
      D_RGB255_ORANGE_LIGHT     //COLOUR_FLOAT_HUE_ORANGE 

      //test with dimmer colours
      D_RGB255_RED
      0, 155, 0,
      D_RGB255_PINK_WARM
      0, 0, 200,
      255, 67, 10,     //COLOUR_FLOAT_HUE_ORANGE   
  
  
      255, 5, 5,
      255, 213, 0,
      0, 100, 200,
      157, 0, 255,
      255, 0, 128,
      
      0, 255,255 // cyan
  
  
    };

    #ifndef D_PALETTE_CHRISTMAS_11_NAME_CTR
    #define D_PALETTE_CHRISTMAS_11_NAME_CTR        "Christmas Outside Many Multi Very different colours|Christmas 11"   
    #endif
    DEF_PGM_CTR  (PM_PALETTE_CHRISTMAS_11_NAME_CTR)        D_PALETTE_CHRISTMAS_11_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_CHRISTMAS_11_COLOUR_MAP__IDS)
    { // R,G,B
      D_RGB255_RED
      D_RGB255_GREEN_FADED
      D_RGB255_PINK_WARM
      D_RGB255_BLUE
      D_RGB255_ORANGE_LIGHT     //COLOUR_FLOAT_HUE_ORANGE 

      //test with dimmer colours
      D_RGB255_RED
      D_RGB255_GREEN
      D_RGB255_PINK_WARM
      0, 0, 150,
      D_RGB255_ORANGE_LIGHT     //COLOUR_FLOAT_HUE_ORANGE 

      //test with dimmer colours
      D_RGB255_RED
      0, 155, 0,
      D_RGB255_PINK_WARM
      0, 0, 200,
      255, 67, 10,     //COLOUR_FLOAT_HUE_ORANGE   
  
  
      255, 5, 5,
      255, 213, 0,
      0, 100, 200,                                                                                                                                   
      157, 0, 255,
      255, 0, 128,
      
      0, 255,255 // cyan
  
  
    };



    #ifndef D_PALETTE_CHRISTMAS_12_NAME_CTR
    #define D_PALETTE_CHRISTMAS_12_NAME_CTR        "Christmas Snow Tree|Christmas 12" //Keep this name   
    #endif
    DEF_PGM_CTR  (PM_PALETTE_CHRISTMAS_12_NAME_CTR)        D_PALETTE_CHRISTMAS_12_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_CHRISTMAS_12_COLOUR_MAP__IDS)
    { // R,G,B
      D_RGB255_RED
      D_RGB255_GREEN_FADED
      D_RGB255_PINK_WARM
      D_RGB255_BLUE_FADED
      D_RGB255_ORANGE_LIGHT   
      D_RGB255_GREEN_PASTEL
      D_RGB255_CYAN_FADED2
      D_RGB255_PINK_HOT2
      D_RGB255_RED_PASTEL
      D_RGB255_ORANGE_DEEP
      D_RGB255_YELLOW_WARM 
    };



    #ifndef D_PALETTE_CHRISTMAS_13_NAME_CTR
    #define D_PALETTE_CHRISTMAS_13_NAME_CTR        "Christmas Outside Tree Step through|Christmas 13"   
    #endif
    DEF_PGM_CTR  (PM_PALETTE_CHRISTMAS_13_NAME_CTR)        D_PALETTE_CHRISTMAS_13_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_CHRISTMAS_13_COLOUR_MAP__IDS)
    { // R,G,B
      D_RGB255_RED
      D_RGB255_GREEN_FADED
      D_RGB255_PINK_WARM   // Pink
      D_RGB255_BLUE_FADED2
      D_RGB255_ORANGE_LIGHT    // Orange
      // 25, 112, 25, //pastel green 
      0, 255,180,     // green-Cyan  **
      D_RGB255_BLUE

      D_RGB255_RED
      D_RGB255_GREEN_FADED
      D_RGB255_PINK_WARM   // Pink
      D_RGB255_BLUE_FADED2
      D_RGB255_ORANGE_LIGHT    // Orange
      // 25, 112, 25, //pastel green
      0, 255,0,     // green-Cyan      ** 
      D_RGB255_BLUE
    };




    #ifndef D_PALETTE_CHRISTMAS_14_NAME_CTR
    #define D_PALETTE_CHRISTMAS_14_NAME_CTR        "Lots of darker colours, few bright|Christmas 14"//"Christmas Multi with Warm White"   
    #endif
    DEF_PGM_CTR  (PM_PALETTE_CHRISTMAS_14_NAME_CTR)        D_PALETTE_CHRISTMAS_14_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_CHRISTMAS_14_COLOUR_MAP__IDS)
    { // R,G,B
      D_RGB255_RED
      D_RGB255_GREEN_FADED
      D_RGB255_PINK_WARM
      D_RGB255_BLUE
      D_RGB255_ORANGE_LIGHT     //COLOUR_FLOAT_HUE_ORANGE 
      //test with dimmer colours
      D_RGB255_RED
      D_RGB255_GREEN
      D_RGB255_PINK_WARM
      0, 0, 150,
      D_RGB255_ORANGE_LIGHT     //COLOUR_FLOAT_HUE_ORANGE 
      //test with dimmer colours
      D_RGB255_RED
      0, 155, 0,
      D_RGB255_PINK_WARM
      0, 0, 200,
      255, 67, 10,     //COLOUR_FLOAT_HUE_ORANGE 
      0,255,255 // cyan
    };



    #ifndef D_PALETTE_CHRISTMAS_15_NAME_CTR
    #define D_PALETTE_CHRISTMAS_15_NAME_CTR        "Cherry Yellow|Christmas 15"//ristmas Outside 13 with brightness matching colours"   
    #endif
    DEF_PGM_CTR  (PM_PALETTE_CHRISTMAS_15_NAME_CTR)        D_PALETTE_CHRISTMAS_15_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_CHRISTMAS_15_COLOUR_MAP__IDS)
    { // R,G,B
      D_RGB255_RED
      D_RGB255_GREEN_FADED2
      D_RGB255_PINK_HOT2
      D_RGB255_CYAN_FADED2
      D_RGB255_YELLOW_WARM
    };



    #ifndef D_PALETTE_CHRISTMAS_16_NAME_CTR
    #define D_PALETTE_CHRISTMAS_16_NAME_CTR        "Cherry Orange|Christmas 16"   
    #endif
    DEF_PGM_CTR  (PM_PALETTE_CHRISTMAS_16_NAME_CTR)        D_PALETTE_CHRISTMAS_16_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_CHRISTMAS_16_COLOUR_MAP__IDS)
    { // R,G,B    
      D_RGB255_RED
      D_RGB255_GREEN_FADED2
      D_RGB255_PINK_HOT2
      D_RGB255_CYAN_FADED2
      D_RGB255_ORANGE_DEEP
    };


    /**
     * Make lights that have both coloured and some warm white white aka having both white/coloured on trees
     * */

    #ifndef D_PALETTE_CHRISTMAS_17_NAME_CTR
    #define D_PALETTE_CHRISTMAS_17_NAME_CTR        "warm whites Christmas Outside Many Multi Very different colours|Christmas 17"   
    #endif
    DEF_PGM_CTR  (PM_PALETTE_CHRISTMAS_17_NAME_CTR)        D_PALETTE_CHRISTMAS_17_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_CHRISTMAS_17_COLOUR_MAP__IDS)
    { // R,G,B      
      D_RGB255_RED
      D_RGB255_WHITE_WARM1
      D_RGB255_GREEN_FADED2
      D_RGB255_WHITE_WARM1
      D_RGB255_PINK_WARM
      D_RGB255_WHITE_WARM1
      D_RGB255_BLUE_FADED3
      D_RGB255_WHITE_WARM1
      D_RGB255_ORANGE_LIGHT    
      D_RGB255_WHITE_WARM1   
      D_RGB255_GREEN_PASTEL_FADED
      D_RGB255_WHITE_WARM1
      D_RGB255_CYAN_FADED3
      D_RGB255_WHITE_WARM1
      D_RGB255_PINK_HOT
      D_RGB255_WHITE_WARM1
      D_RGB255_RED_PASTEL
      D_RGB255_WHITE_WARM1
      D_RGB255_ORANGE_DEEP
      D_RGB255_WHITE_WARM1
      D_RGB255_YELLOW_WARM
      D_RGB255_WHITE_WARM1
      // Repeated with tweaks
      D_RGB255_RED_ALT
      D_RGB255_WHITE_WARM1
      D_RGB255_GREEN_FADED2
      D_RGB255_WHITE_WARM1
      D_RGB255_PINK_WARM
      D_RGB255_WHITE_WARM1
      D_RGB255_BLUE_FADED3
      D_RGB255_WHITE_WARM1
      D_RGB255_ORANGE_LIGHT   
      D_RGB255_WHITE_WARM1
      D_RGB255_GREEN_PASTEL_FADED
      D_RGB255_WHITE_WARM1
      D_RGB255_PINK_HOT2
      D_RGB255_WHITE_WARM1
      D_RGB255_CYAN_FADED3
      D_RGB255_WHITE_WARM1
      D_RGB255_RED_PASTEL
      D_RGB255_WHITE_WARM1
      D_RGB255_ORANGE_DEEP
      D_RGB255_WHITE_WARM1
      D_RGB255_YELLOW_WARM
      D_RGB255_WHITE_WARM1  
    };

    /**
     * Make lights like the berry lights 
     * */



    #ifndef D_PALETTE_CHRISTMAS_18_NAME_CTR
    #define D_PALETTE_CHRISTMAS_18_NAME_CTR        "Woolworths berry lights Red,Green.Yellow,purple,cyan|Christmas 18"   
    #endif
    DEF_PGM_CTR  (PM_PALETTE_CHRISTMAS_18_NAME_CTR)        D_PALETTE_CHRISTMAS_18_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_CHRISTMAS_18_COLOUR_MAP__IDS)
    { // R,G,B      
    
      D_RGB255_RED
      D_RGB255_GREEN_FADED2
      D_RGB255_BLUE_FADED3
      D_RGB255_ORANGE_LIGHT
      D_RGB255_PURPLE_DEEP
    
    };

    #ifndef D_PALETTE_CHRISTMAS_19_NAME_CTR
    #define D_PALETTE_CHRISTMAS_19_NAME_CTR        "tbd|Christmas 19"   
    #endif
    DEF_PGM_CTR  (PM_PALETTE_CHRISTMAS_19_NAME_CTR)        D_PALETTE_CHRISTMAS_19_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_CHRISTMAS_19_COLOUR_MAP__IDS)
    { // R,G,B      
    
      D_RGB255_RED
      D_RGB255_GREEN_FADED2
      D_RGB255_BLUE_FADED3
      D_RGB255_ORANGE_LIGHT
    
    };

    // Confirmed at 2021 Tree colours
    #ifndef D_PALETTE_CHRISTMAS_20_NAME_CTR
    #define D_PALETTE_CHRISTMAS_20_NAME_CTR        "MultiColoured Snow Tree Dinning Room|Christmas 20"   
    #endif
    // #define PM_PALETTE_CHRISTMAS_20_DESCRIPTION_CTR   "Dining room 2021 Palette" // new optional (for memory) descriptions
    DEF_PGM_CTR  (PM_PALETTE_CHRISTMAS_20_NAME_CTR)        D_PALETTE_CHRISTMAS_20_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_CHRISTMAS_20_COLOUR_MAP__IDS)
    { // R,G,B      
      D_RGB255_RED
      D_RGB255_GREEN_FADED2
      D_RGB255_PINK_WARM
      D_RGB255_BLUE_FADED3
      D_RGB255_ORANGE_LIGHT       
      D_RGB255_GREEN_PASTEL_FADED
      D_RGB255_CYAN_FADED3
      D_RGB255_PINK_HOT
      D_RGB255_RED_PASTEL
      D_RGB255_ORANGE_DEEP
      D_RGB255_YELLOW_WARM
      // Repeated with tweaks
      D_RGB255_RED_ALT
      D_RGB255_GREEN_FADED2
      D_RGB255_PINK_WARM
      D_RGB255_BLUE_FADED3
      D_RGB255_GREEN_PASTEL_FADED
      D_RGB255_PINK_HOT2
      D_RGB255_CYAN_FADED3
      D_RGB255_RED_PASTEL
      D_RGB255_ORANGE_DEEP
      D_RGB255_PURPLE_DEEP
      D_RGB255_PINK_HOT3
      D_RGB255_CYAN_FADED3      
    };


    #ifndef D_PALETTE_CHRISTMAS_21_NAME_CTR
    #define D_PALETTE_CHRISTMAS_21_NAME_CTR        "Chrismtas White Warm80Cold20|Christmas 21"   
    #endif
    // #define PM_PALETTE_CHRISTMAS_21_DESCRIPTION_CTR   "Mostly white white with some cold white" // new optional (for memory) descriptions
    DEF_PGM_CTR  (PM_PALETTE_CHRISTMAS_21_NAME_CTR)        D_PALETTE_CHRISTMAS_21_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_CHRISTMAS_21_COLOUR_MAP__IDS)
    { // R,G,B      

    // I think "cold white" should actually be much dimmer than warm white, this will "even" the brightness
    
      D_RGB255_WHITE_WARM_MICROLEDS_50SET
      D_RGB255_WHITE_WARM_MICROLEDS_50SET
      D_RGB255_WHITE_WARM_MICROLEDS_50SET
      D_RGB255_WHITE_WARM_MICROLEDS_50SET
      D_RGB255_WHITE_FADED
    
    };




    #ifndef D_PALETTE_CUSTOM_USER_01_NAME_CTR
    #define D_PALETTE_CUSTOM_USER_01_NAME_CTR        "Custom User 01"   
    #endif
    DEF_PGM_CTR  (PM_PALETTE_CUSTOM_USER_01_NAME_CTR)        D_PALETTE_CUSTOM_USER_01_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_CUSTOM_USER_01_COLOUR_MAP__IDS)
    { // R,G,B
     
      #if defined(USE_CUSTOM_USER_PAULA)
      // R,G,B       
      D_RGB255_RED
      D_RGB255_GREEN_FADED2
      D_RGB255_PINK_WARM
      D_RGB255_BLUE_FADED3
      D_RGB255_ORANGE_LIGHT       
      D_RGB255_GREEN_PASTEL_FADED
      D_RGB255_CYAN_FADED3
      D_RGB255_PINK_HOT
      D_RGB255_RED_PASTEL
      D_RGB255_ORANGE_DEEP
      D_RGB255_YELLOW_WARM
      // Repeated with tweaks
      D_RGB255_RED_ALT
      D_RGB255_GREEN_FADED2
      D_RGB255_PINK_WARM
      D_RGB255_BLUE_FADED3
      D_RGB255_ORANGE_LIGHT   
      D_RGB255_GREEN_PASTEL_FADED
      D_RGB255_PINK_HOT2
      D_RGB255_CYAN_FADED3
      D_RGB255_RED_PASTEL
      D_RGB255_ORANGE_DEEP
      D_RGB255_YELLOW_WARM
      #elif defined(USE_CUSTOM_USER_JACQUELINE)
      // R,G,B       
      D_RGB255_RED
      D_RGB255_GREEN_FADED2
      D_RGB255_PINK_WARM
      D_RGB255_BLUE_FADED3
      D_RGB255_ORANGE_LIGHT       
      D_RGB255_GREEN_PASTEL_FADED
      D_RGB255_CYAN_FADED3
      D_RGB255_PINK_HOT
      D_RGB255_RED_PASTEL
      D_RGB255_ORANGE_DEEP
      D_RGB255_YELLOW_WARM
      // Repeated with tweaks
      D_RGB255_RED_ALT
      D_RGB255_GREEN_FADED2
      D_RGB255_PINK_WARM
      D_RGB255_BLUE_FADED3
      D_RGB255_ORANGE_LIGHT   
      D_RGB255_GREEN_PASTEL_FADED
      D_RGB255_PINK_HOT2
      D_RGB255_CYAN_FADED3
      D_RGB255_RED_PASTEL
      D_RGB255_ORANGE_DEEP
      D_RGB255_YELLOW_WARM
      #else 
      /**
       * @brief Personal Default
       **/
      D_RGB255_RED
      D_RGB255_GREEN_FADED2
      D_RGB255_PINK_WARM
      D_RGB255_BLUE_FADED3
      D_RGB255_ORANGE_LIGHT       
      D_RGB255_GREEN_PASTEL_FADED
      D_RGB255_CYAN_FADED3
      D_RGB255_PINK_HOT
      D_RGB255_RED_PASTEL
      D_RGB255_ORANGE_DEEP
      D_RGB255_YELLOW_WARM
      D_RGB255_RED_ALT
      D_RGB255_GREEN_FADED2
      D_RGB255_PINK_WARM
      D_RGB255_BLUE_FADED3
      D_RGB255_GREEN_PASTEL_FADED
      D_RGB255_PINK_HOT2
      D_RGB255_CYAN_FADED3
      D_RGB255_RED_PASTEL
      D_RGB255_ORANGE_DEEP
      D_RGB255_PURPLE_DEEP
      D_RGB255_PINK_HOT3
      D_RGB255_CYAN_FADED3
      #endif


    };



/**
 * 
 * end of christmas ones
 * 
 * */


    /** GRADIENT_FIRE
     * Warmer sunrise, blue with yellows midway
     * DARKEST, basically night
     * */
    #ifndef D_PALETTE_STATIC_GRADIENT_FIRE_01_NAME_CTR
    #define D_PALETTE_STATIC_GRADIENT_FIRE_01_NAME_CTR        "Gradient Fire 01|Gradient Flame 01"   
    #endif
    //MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
    DEF_PGM_CTR  (PM_PALETTE_STATIC_GRADIENT_FIRE_01_NAME_CTR)        D_PALETTE_STATIC_GRADIENT_FIRE_01_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_STATIC_GRADIENT_FIRE_01_COLOUR_MAP__IDS)
    { //grad range 0-255, R,G,B,WW,CW
      0,   255,   0,   0,  0, 0,
      100, 0, 255, 0,  0, 0,
      200, 0, 0, 255, 0, 0,
      // 127, 30, 24, 82, 0, 0, //CENTRE
      // 135, 20, 14, 82, 0, 0,
      // 145,   20, 14, 82, 0, 0,
      255,   255,   30,   0,  0, 0,   
    };

    
    
    #ifndef D_PALETTE_SINGLE_FIRE_01_NAME_CTR
    #define D_PALETTE_SINGLE_FIRE_01_NAME_CTR        "Single Fire 01|Single Flame 01"     
    #endif
    //MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
    DEF_PGM_CTR  (PM_PALETTE_SINGLE_FIRE_01_NAME_CTR)        D_PALETTE_SINGLE_FIRE_01_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_SINGLE_FIRE_01_COLOUR_MAP__IDS)
    { // 0-255, R,G,B
      D_RGB255_RED
      D_RGB255_RED_ALT
      D_RGB255_ORANGE_DEEP  
      D_RGB255_RED
      D_RGB255_RED
      D_RGB255_ORANGE_DEEP  
      D_RGB255_RED
      D_RGB255_ORANGE_DEEP  
      25, 2, 0   
    };



    
    #ifndef D_PALETTE_FLOWER_SWEATPEAS_01_NAME_CTR
    #define D_PALETTE_FLOWER_SWEATPEAS_01_NAME_CTR        "Flower Sweetpeas 01|Sweetpeas 01"     
    #endif
    //MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
    DEF_PGM_CTR  (PM_PALETTE_FLOWER_SWEATPEAS_01_NAME_CTR)        D_PALETTE_FLOWER_SWEATPEAS_01_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_FLOWER_SWEATPEAS_01_COLOUR_MAP__IDS)
    { // 0-255, R,G,B
      
      D_RGB255_PINK_HOT
      D_RGB255_PINK
      D_RGB255_RED_ALT
      D_RGB255_PINK_HOT2  
      D_RGB255_CYAN_FADED3
      D_RGB255_PINK_WARM
      D_RGB255_RED_ALT
      D_RGB255_PURPLE_BLUE
      D_RGB255_PURPLE_RED
      D_RGB255_CYAN_FADED4
      D_RGB255_WHITE_FADED
      D_RGB255_RED_PASTEL
    };
    #ifndef D_PALETTE_FLOWER_SWEATPEAS_02_NAME_CTR
    #define D_PALETTE_FLOWER_SWEATPEAS_02_NAME_CTR        "Flower Sweetpeas 02|Sweetpeas 02"     
    #endif
    //MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
    DEF_PGM_CTR  (PM_PALETTE_FLOWER_SWEATPEAS_02_NAME_CTR)        D_PALETTE_FLOWER_SWEATPEAS_02_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_FLOWER_SWEATPEAS_02_COLOUR_MAP__IDS)
    { // 0-255, R,G,B
      D_RGB255_PINK_HOT
      D_RGB255_PINK
      D_RGB255_CYAN_FADED3
      D_RGB255_PINK_HOT2  
      D_RGB255_PURPLE_BLUE
      D_RGB255_PINK_WARM
      D_RGB255_WHITE_FADED
      D_RGB255_PURPLE_RED
      D_RGB255_BLUE_FADED3
    };

    #ifndef D_PALETTE_PINK_PURPLE_01_NAME_CTR
    #define D_PALETTE_PINK_PURPLE_01_NAME_CTR        "Pink purple 01"     
    #endif
    //MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
    DEF_PGM_CTR  (PM_PALETTE_PINK_PURPLE_01_NAME_CTR)        D_PALETTE_PINK_PURPLE_01_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_PINK_PURPLE_01_COLOUR_MAP__IDS)
    { // 0-255, R,G,B
      D_RGB255_PINK_HOT
      D_RGB255_PINK
      D_RGB255_PINK_HOT2  
      D_RGB255_PINK_WARM
      D_RGB255_PURPLE_BLUE
      D_RGB255_PURPLE_RED
    };

    #ifndef D_PALETTE_PINK_PURPLE_02_NAME_CTR
    #define D_PALETTE_PINK_PURPLE_02_NAME_CTR        "Pink purple 02"     
    #endif
    //MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
    DEF_PGM_CTR  (PM_PALETTE_PINK_PURPLE_02_NAME_CTR)        D_PALETTE_PINK_PURPLE_02_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_PINK_PURPLE_02_COLOUR_MAP__IDS)
    { // 0-255, R,G,B
      D_RGB255_PINK_HOT
      D_RGB255_PINK
      D_RGB255_PINK_HOT2  
      D_RGB255_PINK_WARM
      D_RGB255_PURPLE_BLUE
      D_RGB255_PURPLE_RED
      D_RGB255_RED_PASTEL
    };

    #ifndef D_PALETTE_PURPLE_PINK_80_20_NAME_CTR
    #define D_PALETTE_PURPLE_PINK_80_20_NAME_CTR        "Purple Pink 80/20"     
    #endif
    //MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
    DEF_PGM_CTR  (PM_PALETTE_PURPLE_PINK_80_20_NAME_CTR)        D_PALETTE_PURPLE_PINK_80_20_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_PURPLE_PINK_80_20_COLOUR_MAP__IDS)
    { // 0-255, R,G,B
      D_RGB255_PINK_HOT
      D_RGB255_PURPLE_BLUE
      D_RGB255_PURPLE_RED
      D_RGB255_PINK
      D_RGB255_PURPLE_BLUE
      D_RGB255_PURPLE_RED
      D_RGB255_PINK_HOT2  
      D_RGB255_PURPLE_BLUE
      D_RGB255_PURPLE_RED
      D_RGB255_PINK_WARM
      D_RGB255_PURPLE_BLUE
      D_RGB255_PURPLE_RED
    };




    #ifndef D_PALETTE_MICROLEDS_PASTEL_01_NAME_CTR
    #define D_PALETTE_MICROLEDS_PASTEL_01_NAME_CTR        "MicroLEDs Pastel 01"     
    #endif
    //MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
    DEF_PGM_CTR  (PM_PALETTE_MICROLEDS_PASTEL_01_NAME_CTR)        D_PALETTE_MICROLEDS_PASTEL_01_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_MICROLEDS_PASTEL_01_COLOUR_MAP__IDS)
    { // 0-255, R,G,B
      D_RGB255_PINK_HOT
      D_RGB255_GREEN
      D_RGB255_BLUE  
      D_RGB255_WHITE
      D_RGB255_WHITE_WARM
    };





    /** GRADIENT_SUNLEVEL_GROUP01
     * Warmer sunrise, blue with yellows midway
     * DARKEST, basically night
     * */
    #ifndef D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_01_NAME_CTR
    #define D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_01_NAME_CTR        "Sunlevel Group01 Below01|Sunlevel Group01 -10 Degrees"   
    #endif
    //MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
    DEF_PGM_CTR  (PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_01_NAME_CTR)        D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_01_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_01_COLOUR_MAP__IDS)
    { //grad range 0-255, R,G,B,WW,CW
      0,     0,   0,   0,  0, 0,
      100,    20, 14, 82,  0, 0,
      120, 20, 14, 82, 0, 0,
      127, 30, 24, 82, 0, 0, //CENTRE
      135, 20, 14, 82, 0, 0,
      145,   20, 14, 82, 0, 0,
      255,   0,   0,   0,  0, 0,   
    };

    /** GRADIENT_SUNLEVEL_GROUP01
     * just before sunrise
     * */
    #ifndef D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_02_NAME_CTR
    #define D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_02_NAME_CTR        "Sunlevel Group01 Below01|Sunlevel Group01 -10 Degrees"   
    #endif
    //MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
    DEF_PGM_CTR  (PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_02_NAME_CTR)        D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_02_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_02_COLOUR_MAP__IDS)
    { //grad range 0-255, R,G,B,WW,CW
      0,     0,   0,   0,  0, 0,
      50,    20, 14, 82,  0, 0,
      //120, 30, 24, 150, 0, 0,
      127, 30, 24, 255, 0, 0, //CENTRE
      //135, 30, 24, 150, 0, 0,
      200,   20, 14, 82, 0, 0,
      255,   0,   0,   0,  0, 0,   


    };

    /** GRADIENT_SUNLEVEL_GROUP01
     * Warmer sunrise, blue with yellows midway
     * */
    #ifndef D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_03_NAME_CTR
    #define D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_03_NAME_CTR        "Sunlevel Group01 Below01|Sunlevel Group01 -10 Degrees"   
    #endif
    //MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
    DEF_PGM_CTR  (PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_03_NAME_CTR)        D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_03_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_03_COLOUR_MAP__IDS)
    { //grad range 0-255, R,G,B,WW,CW

      0,  0,   0,   0,  0, 0,

      30, 10, 7, 42, 0, 0,
      50, 84, 39, 27,  0, 0,
      60, 80, 12, 3,0,0,      
      80, 100, 20, 3,0,0,
      120,100, 20, 3,0,0,
      //CENTRE
      127, 255, 44, 10, 0, 0, 
      //CENTRE
      134,100, 20, 3,0,0,   
      174, 100, 20, 3,0,0,
      194, 80, 12, 3,0,0, 
      204, 84, 39, 27,  0, 0,  
      225, 10, 7, 42, 0, 0,  

      255,   0,   0,   0,  0, 0,   

    };

    /** GRADIENT_SUNLEVEL_GROUP01
     * Warmer sunrise, blue with yellows midway
     * */
    #ifndef D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_04_NAME_CTR
    #define D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_04_NAME_CTR        "Sunlevel Group01 Below01|Sunlevel Group01 -10 Degrees"   
    #endif
    //MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
    DEF_PGM_CTR  (PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_04_NAME_CTR)        D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_04_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_04_COLOUR_MAP__IDS)
    { //grad range 0-255, R,G,B,WW,CW
    
      0,  0,   0,   0,  0, 0,

      30, 84, 39, 27, 0, 0,
      50, 84, 39, 27,  0, 0,
      60, 100, 20, 3,0,0,      
      80, 100, 20, 3,0,0,
      120,255, 44, 10, 0, 0, 
      //CENTRE
      127, 255, 100, 10, 0, 0, 
      //CENTRE
      134,255, 44, 10, 0, 0,  
      174, 100, 20, 3,0,0,
      194, 100, 20, 3,0,0, 
      204, 84, 39, 27,  0, 0,  
      225, 84, 39, 27, 0, 0,  
      
      255,   0,   0,   0,  0, 0,   


    };

    /** GRADIENT_SUNLEVEL_GROUP01
     * Warmer sunrise, blue with yellows midway
     * */
    #ifndef D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_05_NAME_CTR
    #define D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_05_NAME_CTR        "Sunlevel Group01 Below01|Sunlevel Group01 -10 Degrees"   
    #endif
    //MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
    DEF_PGM_CTR  (PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_05_NAME_CTR)        D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_05_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_05_COLOUR_MAP__IDS)
    { //grad range 0-255, R,G,B,WW,CW
      
      0,  84, 39, 27,  0, 0,

      30, 100, 20, 3, 0, 0,
      50, 100, 20, 3,  0, 0,
      60, 255, 44, 10,0,0,      
      80, 255, 44, 10,0,0,
      120,218, 230, 9, 0, 0, 
      //CENTRE
      127, 255, 255, 255, 0, 0, 
      //CENTRE
      134,218, 230, 9, 0, 0,  
      174, 255, 44, 10,0,0,
      194, 255, 44, 10,0,0, 
      204, 100, 20, 3,  0, 0,  
      225, 100, 20, 3, 0, 0,  
      
      255,  84, 39, 27,  0, 0,   


    };

    /** GRADIENT_SUNLEVEL_GROUP01
     * Warmer sunrise, blue with yellows midway
     * */
    #ifndef D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_06_NAME_CTR
    #define D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_06_NAME_CTR        "Sunlevel Group01 Below01|Sunlevel Group01 -10 Degrees"   
    #endif
    //MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
    DEF_PGM_CTR  (PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_06_NAME_CTR)        D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_06_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_06_COLOUR_MAP__IDS)
    { //grad range 0-255, R,G,B,WW,CW
      0,  84, 39, 27,  0, 0,

      30, 255, 44, 10, 0, 0,
      50, 255, 44, 10,  0, 0,
      60, 218, 230, 9,0,0,      
      80, 218, 230, 9,0,0,
      120,255, 255, 255, 0, 0, 
      //CENTRE
      127, 255, 255, 255, 0, 0, 
      //CENTRE
      134,255, 255, 255, 0, 0,  
      174, 218, 230, 9,0,0,
      194, 218, 230, 9,0,0, 
      204, 255, 44, 10,  0, 0,  
      225, 255, 44, 10, 0, 0,  
      
      255,  84, 39, 27,  0, 0,   




    };

    /** GRADIENT_SUNLEVEL_GROUP01
     * Warmer sunrise, blue with yellows midway
     * */
    #ifndef D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_NAME_CTR
    #define D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_NAME_CTR        "Sunlevel Group01 Below07|Sunlevel Group01 -10 Degrees"   
    #endif
    //MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
    DEF_PGM_CTR  (PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_NAME_CTR)        D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_COLOUR_MAP__IDS)
    { //grad range 0-255, R,G,B,WW,CW
      0,  0, 255, 255,  0, 0,

      // 30, 255, 44, 10, 0, 0,
      // 50, 255, 44, 10,  0, 0,
      // 60, 218, 230, 9,0,0,      
      119, 255, 255, 255,  0,0,    //cyan
      120,255, 100, 10, 0, 0, //yellow
      //CENTRE 
      127, 255, 100, 10, 0, 0,     //yellow
      //CENTRE
      134,255, 100, 10, 0, 0,  //yellow
      135, 255, 255, 255,  0,0, //cyan
      // 194, 218, 230, 9,0,0, 
      // 204, 255, 44, 10,  0, 0,  
      // 225, 255, 44, 10, 0, 0,  
      
      255,  0, 255, 255,  0, 0,   




    };







    /**
     * Deep purple to light blue then white
     * */
    #ifndef D_PALETTE_SUNRISE_01_NAME_CTR
    #define D_PALETTE_SUNRISE_01_NAME_CTR        "Sunrise 01|Sunrise Step 01"   
    #endif
    //MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
    DEF_PGM_CTR  (PM_PALETTE_SUNRISE_01_NAME_CTR)        D_PALETTE_SUNRISE_01_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_SUNRISE_01_COLOUR_MAP__IDS)
    { //R,G,B,WW,CW
      // 0x06, 0x2B, 0x79, 0, 0, // Dark Blue
      // 0x16, 0x49, 0x8A, 0, 0,
      // 0x16, 0x49, 0x8A, 25, 25,
      // 0x16, 0x49, 0x8A, 50, 50,
      // 0x59, 0x95, 0xB7, 100, 100,
      // 0x59, 0x95, 0xB7, 175, 175,
      // 255, 255,   255, 255, 255,
      255, 0, 0, 0, 0,
      0, 255, 0, 0, 0,
      D_RGB255_BLUE 0, 0,
      255, 0, 255, 0, 0,
      0, 255, 255, 0, 0,
      255, 255, 255, 0, 0,
    };

    /**
     * Warmer sunrise, blue with yellows midway
     * */
    #ifndef D_PALETTE_SUNRISE_02_NAME_CTR
    #define D_PALETTE_SUNRISE_02_NAME_CTR        "Sunrise 02|Sunrise Step 02"   
    #endif
    //MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
    DEF_PGM_CTR  (PM_PALETTE_SUNRISE_02_NAME_CTR)        D_PALETTE_SUNRISE_02_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_SUNRISE_02_COLOUR_MAP__IDS)
    { //R,G,B,WW,CW
      0x06, 0x2B, 0x79, 0, 0, // Dark Blue
      0x16, 0x49, 0x8A, 0, 0,
      0x16, 0x49, 0x8A, 25, 25,
      0x16, 0x49, 0x8A, 50, 50,
      0x59, 0x95, 0xB7, 100, 100,
      0x59, 0x95, 0xB7, 175, 175,
      255, 255,   255, 255, 255,
    };

    /**
     * Warmer sunrise, blue with yellows midway
     * */
    #ifndef D_PALETTE_SUNRISE_03_NAME_CTR
    #define D_PALETTE_SUNRISE_03_NAME_CTR        "Sunrise 03|Sunrise Gradient 01"   
    #endif
    //MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
    DEF_PGM_CTR  (PM_PALETTE_SUNRISE_03_NAME_CTR)        D_PALETTE_SUNRISE_03_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_SUNRISE_03_COLOUR_MAP__IDS)
    { //grad range 0-255, R,G,B,WW,CW
      // 0, 0x06, 0x2B, 0x79, 0, 0, // Dark Blue
      // 10, 0x16, 0x49, 0x8A, 0, 0,
      // 40, 0x16, 0x49, 0x8A, 25, 25,
      // 100, 0x16, 0x49, 0x8A, 50, 50,
      // 120, 0x59, 0x95, 0xB7, 100, 100,
      // 140, 0x59, 0x95, 0xB7, 175, 175,
      // 255, 255, 255,   255, 255, 255,

      // Use gradient on "step_though_2" that creates colour via gradient using colours either side
      // This will allow smoother sunrise effects

      // 0,255, 0, 0, 0, 0,
      // 10,0, 255, 0, 0, 0,
      // 50,D_RGB255_BLUE 0, 0,
      // 100,255, 0, 255, 0, 0,
      // 120,0, 255, 255, 0, 0,
      // 255,255, 255, 255, 0, 0,

      0,255, 245, 123, 0, 0,
      10,255, 228, 105, 0, 0,
      50,254, 204, 81, 0, 0,
      100,252, 176, 51, 0, 0,
      120,250, 150, 27, 0, 0,
      255,250, 134, 7, 0, 0,



      


    };
    // https://www.schemecolor.com/

    /**
     * Warmer sunrise, blue with yellows midway
     * */
    #ifndef D_PALETTE_SUNRISE_04_NAME_CTR
    #define D_PALETTE_SUNRISE_04_NAME_CTR        "Sunrise 04|Sunrise Gradient 02"   
    #endif
    //MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
    DEF_PGM_CTR  (PM_PALETTE_SUNRISE_04_NAME_CTR)        D_PALETTE_SUNRISE_04_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_SUNRISE_04_COLOUR_MAP__IDS)
    { //grad range 0-255, R,G,B,WW,CW
      0, 0x06, 0x2B, 0x79, 0, 0, // Dark Blue
      10, 0x16, 0x49, 0x8A, 0, 0,
      40, 0x16, 0x49, 0x8A, 25, 25,
      100, 0x16, 0x49, 0x8A, 50, 50,
      120, 0x59, 0x95, 0xB7, 100, 100,
      140, 0x59, 0x95, 0xB7, 175, 175,
      255, 255, 255,   255, 255, 255,
    };

    /**
     * Warmer sunrise, blue with yellows midway
     * */
    #ifndef D_PALETTE_SUNSET_01_NAME_CTR
    #define D_PALETTE_SUNSET_01_NAME_CTR        "Sunset 01|Sunset Step 01"   
    #endif
    //MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
    DEF_PGM_CTR  (PM_PALETTE_SUNSET_01_NAME_CTR)        D_PALETTE_SUNSET_01_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_SUNSET_01_COLOUR_MAP__IDS)
    { //R,G,B,WW,CW
      0x06, 0x2B, 0x79, 0, 0, // Dark Blue
      0x16, 0x49, 0x8A, 0, 0,
      0x16, 0x49, 0x8A, 25, 25,
      0x16, 0x49, 0x8A, 50, 50,
      0x59, 0x95, 0xB7, 100, 100,
      0x59, 0x95, 0xB7, 175, 175,
      255, 255,   255, 255, 255,
    };

    /**
     * Warmer sunrise, blue with yellows midway
     * */
    #ifndef D_PALETTE_SUNSET_02_NAME_CTR
    #define D_PALETTE_SUNSET_02_NAME_CTR        "Sunset 02|Sunrise Gradient 01"   
    #endif
    //MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
    DEF_PGM_CTR  (PM_PALETTE_SUNSET_02_NAME_CTR)        D_PALETTE_SUNSET_02_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_SUNSET_02_COLOUR_MAP__IDS)
    { //grad range 0-255, R,G,B,WW,CW
      0, 0x06, 0x2B, 0x79, 0, 0, // Dark Blue
      10, 0x16, 0x49, 0x8A, 0, 0,
      40, 0x16, 0x49, 0x8A, 25, 25,
      100, 0x16, 0x49, 0x8A, 50, 50,
      120, 0x59, 0x95, 0xB7, 100, 100,
      140, 0x59, 0x95, 0xB7, 175, 175,
      255, 255, 255,   255, 255, 255,
    };


    /**
     * Candle Flame for Candle Animation
     * */
    #ifndef D_PALETTE_CANDLE_FLAME_01_NAME_CTR
    #define D_PALETTE_CANDLE_FLAME_01_NAME_CTR        "Candle|Candle Flame 01"   
    #endif
    //MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
    DEF_PGM_CTR  (PM_PALETTE_CANDLE_FLAME_01_NAME_CTR)        D_PALETTE_CANDLE_FLAME_01_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_CANDLE_FLAME_01_COLOUR_MAP__IDS)
    { //R,G,B,WW,CW
      // 0x06, 0x2B, 0x79, 0, 0, // Dark Blue
      // 0x16, 0x49, 0x8A, 0, 0,
      // 0x16, 0x49, 0x8A, 25, 25,
      // 0x16, 0x49, 0x8A, 50, 50,
      // 0x59, 0x95, 0xB7, 100, 100,
      // 0x59, 0x95, 0xB7, 175, 175,
      // 255, 255,   255, 255, 255,

      /**
       * Animations can work by either using only first two colours, or randomly picking them all
       * 
       */
D_RGB255_WHITE_WARM 0, 0, 
      
      // D_RGB255_RED 0, 0, 
      D_RGB255_ORANGE_DEEP   0, 0, 
      // D_RGB255_RED_ALT 0, 0, 
      // D_RGB255_ORANGE_DEEP   0, 0, 
      // D_RGB255_RED 0, 0, 
      // D_RGB255_RED 0, 0, 
      // D_RGB255_ORANGE_DEEP   0, 0, 
      // D_RGB255_RED 0, 0, 
      // 25, 2, 0,    0, 0, 


    };

    /**
     * from aliexpress photo
     * needs changed to indexed
     * */
    #ifndef D_PALETTE_SKY_GLOW_01_NAME_CTR
    #define D_PALETTE_SKY_GLOW_01_NAME_CTR        "Orange White Blue"   
    #endif
    //MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
    DEF_PGM_CTR  (PM_PALETTE_SKY_GLOW_01_NAME_CTR)        D_PALETTE_SKY_GLOW_01_NAME_CTR;
    DEF_PGM_UINT8(PM_PALETTE_SKY_GLOW_01_COLOUR_MAP__IDS)
    { //R,G,B,WW,CW
      // 0,  255, 40, 0, 0, 0,
      // // 10,  255, 40, 0, 0, 0,
      // 50, 255, 40, 0, 0, 0,
      // // 51, 255, 153, 0, 0, 0,
      // 61, 255, 153, 0, 255, 255,

      0,  255, 100, 0, 0, 0,
      // 10,  255, 40, 0, 0, 0,
      50, 255, 130, 0, 0, 0,
      // 51, 255, 153, 0, 0, 0,
      90, 255, 153, 0, 255, 255,

      127, 255, 255,  255, 255, 255,

      // 200, 0x06, 0x2B, 0x79, 0, 0, // Dark Blue
      // 210, 0x16, 0x49, 0x8A, 0, 0,
      // 211, 0x16, 0x49, 0x8A, 25, 25,
      // 212, 0x16, 0x49, 0x8A, 50, 50,
      // 213, 0x59, 0x95, 0xB7, 100, 100,
      // 214, 0x59, 0x95, 0xB7, 175, 175,
      170, 0, 0,  255, 0, 0,
      255, 0, 0,  255, 0, 0,
    };




// const RgbColor Palette[] PROGMEM = {HsbColor(0.5f, 0.5f, 0.5f), HsbColor(1.0f, 0.5f, 0.5f)};

//     #ifndef D_PALETTE_TEST_NAME_CTR
//     #define D_PALETTE_TEST_NAME_CTR        "Test"   
//     #endif      
//     static const char* PM_PALETTE_TEST_NAME_CTR = D_PALETTE_TEST_NAME_CTR;
//     static uint8_t colour_map_test_id[12] = {
//       0,  255,  0,  0,
//       50,  0, 255,  0,
//       100, 255, 0,  0
//     };

//     //others here

// };


/**
 * Palette group below are gradients from "https://www.schemecolor.com/"
 * */

/**
 * https://www.schemecolor.com/pastel-color-tones.php
 * */
#ifndef D_PALETTE_GRADIENT_PASTEL_TONES_PURPLE_NAME_CTR
#define D_PALETTE_GRADIENT_PASTEL_TONES_PURPLE_NAME_CTR        "Pastel Colour Tones (Purple)"   
#endif
//MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
DEF_PGM_CTR  (PM_PALETTE_GRADIENT_PASTEL_TONES_PURPLE_NAME_CTR)        D_PALETTE_GRADIENT_PASTEL_TONES_PURPLE_NAME_CTR;
DEF_PGM_UINT8(PM_PALETTE_GRADIENT_PASTEL_TONES_PURPLE_COLOUR_MAP__IDS)
{ //grad range 0-255, R,G,B
  0,224, 40, 228, // est. Thistle RGB: (224, 187, 228)
  64,100, 20, 100, // est. Lavender Purple RGB: (149, 125, 173)
  127,200, 20, 180, // est. Pastel Violet RGB: (210, 145, 188)
  190,250, 25, 194, // est. Cotton Candy RGB: (254, 200, 216)
  255,254, 118, 78  //est. Name: Lumber RGB: (255, 223, 211)     
};


/**
 * https://www.schemecolor.com/hot-pink-neon-with-navy.php
 * */
#ifndef D_PALETTE_SINGLE_HOT_PINK_NEON_WITH_NAVY_NAME_CTR
#define D_PALETTE_SINGLE_HOT_PINK_NEON_WITH_NAVY_NAME_CTR        "Hot Pink Neon With Navy"   
#endif
//MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
DEF_PGM_CTR  (PM_PALETTE_SINGLE_HOT_PINK_NEON_WITH_NAVY_NAME_CTR)        D_PALETTE_SINGLE_HOT_PINK_NEON_WITH_NAVY_NAME_CTR;
DEF_PGM_UINT8(PM_PALETTE_SINGLE_HOT_PINK_NEON_WITH_NAVY_COLOUR_MAP__IDS)
{ // 0-255, R,G,B
  255, 105, 179, // est. Hot Pink,      RGB: (255, 105, 179)
  236, 52, 153, // est.  Royal Pink,   RGB: (236, 52, 153)
  0, 0, 129, // est. Navy Blue RGB: (0, 0, 129)
  21, 70, 199, // est. New Car RGB: (21, 70, 199)
  25, 116, 209  //est. Bright Navy Blue RGB: (25, 116, 209)   
};





/**
 * The indexes are packed with the elevation in degrees that equate to the colour
 * 
 * 1) Full colour, cyan in daylight
 * 2) only colours of red/orange during sunset
 * 3) CCT only, full brightness
 * */
#ifndef D_PALETTE__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_01__NAME_CTR
#define D_PALETTE__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_01__NAME_CTR   "Solid RGBCCT Sun Elevation 01"
#endif
//MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
DEF_PGM_CTR  (PM_PALETTE__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_01__NAME_CTR)
        D_PALETTE__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_01__NAME_CTR;
DEF_PGM_UINT8(PM_SOLID__RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_01__COLOUR_MAP__IDS)
{ // R,G,B,WW,CW
//Index values are stored relative to -90, so -90 to 180 is 0 to 180

  // Night time          ww, cw
  90 -90, 0, 0, 1,        0, 0, // Peak dusk
  90 -45, 0, 0, 1,        0, 0,
  90 -15, 0, 0, 30,       0, 0,
  90 -10, 0, 0, 100,      0, 0,
  90 -5, 0, 0, 20,       20, 0,
  90 -2, 0, 0, 15,      10, 0,
  90 -1, 50, 10, 10,     5, 0,
  90 +0, 255, 3, 0,       2, 0,  // Transition
  90 +1, D_RGB255_ORANGE_DEEP       1, 20,
  90 +2, D_RGB255_YELLOW_WARM     0, 100,
  90 +5, 8, 10, 0,    0, 150,
  90 +10, 0, 20, 0,  0, 200,
  90 +15, 0, 30, 0,  0, 255,
  90 +45, 200, 40, 0,   0, 255,
  90 +90, 255, 50, 0,   0, 255  // Noon

  // 90 -7, 0, 1,  0, 0, 0, // Peak dusk
  // 90 -6, 1, 1, 0, 0, 0,
  // 90 -5, 2, 1, 0, 0, 0,
  // 90 -4, 3, 1, 0, 0, 0,
  // 90 -3, 4, 1, 0, 0, 0,
  // 90 -2, 5, 1, 0, 0, 0,
  // 90 -1, 5, 1, 0, 0, 0,
  // 90 +0, 6, 0, 1, 0, 0,  // Transition
  // 90 +1, 7, 0, 1, 0, 0,
  // 90 +2, 7, 0, 1, 0, 0,
  // 90 +3, 8, 0, 1, 0, 0,
  // 90 +4, 9, 0, 1, 0, 0,
  // 90 +5, 10, 0, 1, 0, 0,
  // 90+ 6, 11, 0, 1, 0, 0,
  // 90 +7, 12, 0, 1, 0, 0  // Noon



  // 90 -90, D_RGB255_RED 0,0,//7, 0, // Peak dusk
  // 90 -45, D_RGB255_ORANGE_LIGHT 0,0,//6, 0,
  // 90 -15, D_RGB255_ORANGE_DEEP 0,0,//5, 0,
  // 90 -10, D_RGB255_YELLOW_WARM 0,0,//4, 0,
  // 90 -5, D_RGB255_GREEN 0,0,//3, 0,
  // 90 -2, D_RGB255_CYAN 0,0,//2, 0,
  // 90 -1, D_RGB255_BLUE 0,0,//1, 0,
  // 90 +0, D_RGB255_RED 0,0,//0, 0,  // Transition
  // 90 +1, D_RGB255_PURPLE_BLUE 0,0,//0, 1,
  // 90 +2, D_RGB255_PINK_HOT 0,0,//0, 2,
  // 90 +5, D_RGB255_RED 0,0,//0, 3,
  // 90 +10, D_RGB255_ORANGE_LIGHT 0,0,//0, 4,
  // 90 +15, D_RGB255_YELLOW_WARM 0,0,//0, 5,
  // 90+ 45, D_RGB255_GREEN 0,0,//0, 6,
  // 90 +90, D_RGB255_CYAN 0,0,//0, 7  // Noon

  /**
   * Sun effect modes
   * 1) full colour, with cyan at midday
   * 1b) colour sunset transition, day white in day time, warm white at night... but yellow to red during sunset
   * 2) CCT only, aligned with sun directly, day to warm white but full brightness
   * 
   * CCT only should be another animation mode, direct mapping of cct_temp
   * 
   * 
   * */

};





/**
 * The indexes are packed with the elevation in degrees that equate to the colour
 * 
 * 1) Full colour, cyan in daylight
 * 2) only colours of red/orange during sunset
 * 3) CCT only, full brightness
 * */


/**
 * RGBCCT
 * Day:     Cyan
 * Sunset:  yellow, orange, red
 * Night:   dark blue (level 1)
 * */
#ifndef D_PALETTE__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_02__NAME_CTR
#define D_PALETTE__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_02__NAME_CTR   "Solid RGBCCT Sun Elevation 02: Dark Blue" // CCT ONLY
#endif
//MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
DEF_PGM_CTR  (PM_PALETTE__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_02__NAME_CTR)
        D_PALETTE__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_02__NAME_CTR;
DEF_PGM_UINT8(PM_SOLID__RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_02__COLOUR_MAP__IDS)
{ // R,G,B,WW,CW
  //Index values are stored relative to -90, so -90 to 180 is 0 to 180

  // Night time          ww, cw
  90 -90, 0, 0, 0,        255, 0, // Peak dusk
  90 -45, 0, 0, 1,        0, 0,
  90 -15, 0, 0, 30,       0, 0,
  90 -10, 0, 0, 100,      0, 0,
  90 -5, 0, 0, 20,       20, 0,
  90 -2, 0, 0, 15,      10, 0,
  90 -1, 50, 10, 10,     5, 0,
  90 +0, 255, 3, 0,       2, 0,  // Transition
  90 +1, D_RGB255_ORANGE_DEEP       1, 20,
  90 +2, D_RGB255_YELLOW_WARM     0, 100,
  90 +5, 8, 10, 0,    0, 150,
  90 +10, 0, 20, 0,  0, 200,
  90 +15, 0, 30, 0,  0, 255,
  90 +45, 200, 40, 0,   0, 255,
  90 +90, 255, 50, 0,   0, 255  // Noon
  
};

/**
 * RGBCCT
 * Day:     Cyan
 * Sunset:  yellow, orange, red
 * Night:   deep red (level 1)
 * */
#ifndef D_PALETTE__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_03__NAME_CTR
#define D_PALETTE__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_03__NAME_CTR   "Solid RGBCCT Sun Elevation 03: Dark Red" // CCT ONLY
#endif
//MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
DEF_PGM_CTR  (PM_PALETTE__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_03__NAME_CTR)
        D_PALETTE__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_03__NAME_CTR;
DEF_PGM_UINT8(PM_SOLID__RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_03__COLOUR_MAP__IDS)
{ // R,G,B,WW,CW
  //Index values are stored relative to -90, so -90 to 180 is 0 to 180

  // Night time          ww, cw
  90 -90, 1, 0, 0,        0, 0, // Peak dusk
  90 -45, 1, 0, 0,        0, 0,
  90 -15, 1, 0, 0,       0, 0,
  90 -10, 1, 0, 0,      0, 0,
  90 -5, 2, 0, 0,       20, 0,
  90 -2, 3, 0, 0,      10, 0,
  90 -1, 100, 0, 0,     5, 0,
  90 +0, 255, 3, 0,       2, 0,  // Transition
  90 +1, D_RGB255_ORANGE_DEEP       1, 20,
  90 +2, D_RGB255_YELLOW_WARM     0, 100,
  90 +5, 8, 10, 0,    0, 150,
  90 +10, 0, 20, 0,  0, 200,
  90 +15, 0, 30, 0,  0, 255,
  90 +45, 200, 40, 0,   0, 255,
  90 +90, 255, 50, 0,   0, 255  // Noon
  
};


/**
 * RGBCCT: Kitchen light tester
 * Day:     Pink
 * Sunset:  Pink and white (pale pink)
 * Night:   Warm white
 * */
#ifndef D_PALETTE__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_04__NAME_CTR
#define D_PALETTE__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_04__NAME_CTR   "Solid RGBCCT Sun Elevation 04: Kitchen Pink, CW, WW" // CCT ONLY
#endif
//MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
DEF_PGM_CTR  (PM_PALETTE__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_04__NAME_CTR)
        D_PALETTE__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_04__NAME_CTR;
DEF_PGM_UINT8(PM_SOLID__RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_04__COLOUR_MAP__IDS)
{ // R,G,B,WW,CW
  //Index values are stored relative to -90, so -90 to 180 is 0 to 180

  // Night time          ww, cw
  90 -90, 0, 0, 0,      255, 0, // Peak dusk
  90 -45, 0, 0, 0,      255, 0,
  90 -15, 0, 0, 0,      255, 0,
  90 -10, 0, 0, 0,      255, 150,
  90 -5,  0, 0, 0,      100, 255,
  90 -2,  0, 0, 0,      20,  200,
  90 -1, 100, 0, 25,     0,  150,
  90 +0, 150, 0, 50,     0,  50,  // Transition
  90 +1, 200, 0, 100,     0, 20,
  90 +2,  D_RGB255_PINK_HOT3   0, 10,
  90 +5,  D_RGB255_PINK_HOT3   0, 0,
  90 +10, D_RGB255_PINK_HOT3   0, 0,
  90 +15, D_RGB255_PINK_HOT3   0, 0,
  90 +45, D_RGB255_PINK_HOT3   0, 0,
  90 +90, D_RGB255_PINK_HOT3   0, 0  // Noon
  
};


// Figure out how to roll this into its own 
/**
 * RGBCCT Pairs
 * Day:     Cyan
 * Sunset:  yellow, orange, red
 * Night:   deep red (level 1)
 * */
#ifndef D_PALETTE__DUAL_COLOUR_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01__NAME_CTR
#define D_PALETTE__DUAL_COLOUR_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01__NAME_CTR   "Solid RGBCCT Sun Elevation 01 Dual Colours: Dark Red" // CCT ONLY
#endif
//MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID
DEF_PGM_CTR  (PM_PALETTE__DUAL_COLOUR_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01__NAME_CTR)
        D_PALETTE__DUAL_COLOUR_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01__NAME_CTR;
DEF_PGM_UINT8(PM_SOLID__DUAL_COLOUR_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01__COLOUR_MAP__IDS)
{ // R,G,B,WW,CW
  // Index values are stored relative to -90, so -90 to 180 is 0 to 180
  // Indexes must be replicated to give pairs (maybe 3 and 4 in groups later?)

  // Night time          ww, cw
  90 -90, 1, 0, 0,        0, 0, // Peak dusk
  90 -90, 1, 0, 0,        0, 0, // Peak dusk
  90 -45, 1, 0, 0,        0, 0,
  90 -45, 1, 0, 0,        0, 0,
  90 -15, 1, 0, 0,       0, 0,
  90 -15, 1, 0, 0,       0, 0,
  90 -10, 1, 0, 0,      0, 0,
  90 -10, 1, 0, 0,      0, 0,
  90 -5, 2, 0, 0,       20, 0,
  90 -5, 2, 0, 0,       20, 0,
  90 -2, 3, 0, 0,      10, 0,
  90 -2, 3, 0, 0,      10, 0,
  90 -1, 100, 0, 0,     5, 0,
  90 -1, 100, 0, 0,     5, 0,
  90 +0, 255, 3, 0,       2, 0,  // Transition
  90 +0, 255, 3, 0,       2, 0,  // Transition
  90 +1, D_RGB255_ORANGE_DEEP       1, 20,
  90 +1, D_RGB255_ORANGE_DEEP       1, 20,
  90 +2, D_RGB255_YELLOW_WARM     0, 100,
  90 +2, D_RGB255_YELLOW_WARM     0, 100,
  90 +5, 8, 10, 0,    0, 150,
  90 +5, 8, 10, 0,    0, 150,
  90 +10, 0, 20, 0,  0, 200,
  90 +10, 0, 20, 0,  0, 200,
  90 +15, 0, 30, 0,  0, 255,
  90 +15, 0, 30, 0,  0, 255,
  90 +45, 200, 40, 0,   0, 255,
  90 +45, 200, 40, 0,   0, 255,
  90 +90, 255, 50, 0,   0, 255,  // Noon
  90 +90, 255, 50, 0,   0, 255  // Noon
  
};



















// #ifdef ENABLE_CRGBPALETTES_IN_PROGMEM



      
#define GRADIENT_PALETTE_COUNT 39

const byte ib_jul01_gp[] PROGMEM = {
    0, 194,  1,  1,
   94,   1, 29, 18,
  132,  57,131, 28,
  255, 113,  1,  1};

// Gradient palette "es_vintage_57_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/vintage/tn/es_vintage_57.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

const byte es_vintage_57_gp[] PROGMEM = {
    0,   2,  1,  1,
   53,  18,  1,  0,
  104,  69, 29,  1,
  153, 167,135, 10,
  255,  46, 56,  4};


// Gradient palette "es_vintage_01_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/vintage/tn/es_vintage_01.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 32 bytes of program space.

const byte es_vintage_01_gp[] PROGMEM = {
    0,   4,  1,  1,
   51,  16,  0,  1,
   76,  97,104,  3,
  101, 255,131, 19,
  127,  67,  9,  4,
  153,  16,  0,  1,
  229,   4,  1,  1,
  255,   4,  1,  1};


// Gradient palette "es_rivendell_15_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/rivendell/tn/es_rivendell_15.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

const byte es_rivendell_15_gp[] PROGMEM = {
    0,   1, 14,  5,
  101,  16, 36, 14,
  165,  56, 68, 30,
  242, 150,156, 99,
  255, 150,156, 99};


// Gradient palette "rgi_15_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ds/rgi/tn/rgi_15.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 36 bytes of program space.
// Edited to be brighter

const byte rgi_15_gp[] PROGMEM = {
    0,   4,  1, 70,
   31,  55,  1, 30,
   63, 255,  4,  7,
   95,  59,  2, 29,
  127,  11,  3, 50,
  159,  39,  8, 60,
  191, 112, 19, 40,
  223,  78, 11, 39,
  255,  29,  8, 59};


// Gradient palette "retro2_16_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ma/retro2/tn/retro2_16.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 8 bytes of program space.

const byte retro2_16_gp[] PROGMEM = {
    0, 188,135,  1,
  255,  46,  7,  1};


// Gradient palette "Analogous_1_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/red/tn/Analogous_1.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

const byte Analogous_1_gp[] PROGMEM = {
    0,   3,  0,255,
   63,  23,  0,255,
  127,  67,  0,255,
  191, 142,  0, 45,
  255, 255,  0,  0};


// Gradient palette "es_pinksplash_08_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/pink_splash/tn/es_pinksplash_08.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

const byte es_pinksplash_08_gp[] PROGMEM = {
    0, 126, 11,255,
  127, 197,  1, 22,
  175, 210,157,172,
  221, 157,  3,112,
  255, 157,  3,112};


// Gradient palette "es_ocean_breeze_036_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/ocean_breeze/tn/es_ocean_breeze_036.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 16 bytes of program space.

const byte es_ocean_breeze_036_gp[] PROGMEM = {
    0,   1,  6,  7,
   89,   1, 99,111,
  153, 144,209,255,
  255,   0, 73, 82};


// Gradient palette "departure_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/mjf/tn/departure.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 88 bytes of program space.

const byte departure_gp[] PROGMEM = {
    0,   8,  3,  0,
   42,  23,  7,  0,
   63,  75, 38,  6,
   84, 169, 99, 38,
  106, 213,169,119,
  116, 255,255,255,
  138, 135,255,138,
  148,  22,255, 24,
  170,   0,255,  0,
  191,   0,136,  0,
  212,   0, 55,  0,
  255,   0, 55,  0};


// Gradient palette "es_landscape_64_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/landscape/tn/es_landscape_64.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 36 bytes of program space.

const byte es_landscape_64_gp[] PROGMEM = {
    0,   0,  0,  0,
   37,   2, 25,  1,
   76,  15,115,  5,
  127,  79,213,  1,
  128, 126,211, 47,
  130, 188,209,247,
  153, 144,182,205,
  204,  59,117,250,
  255,   1, 37,192};


// Gradient palette "es_landscape_33_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/landscape/tn/es_landscape_33.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.

const byte es_landscape_33_gp[] PROGMEM = {
    0,   1,  5,  0,
   19,  32, 23,  1,
   38, 161, 55,  1,
   63, 229,144,  1,
   66,  39,142, 74,
  255,   1,  4,  1};


// Gradient palette "rainbowsherbet_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ma/icecream/tn/rainbowsherbet.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

const byte rainbowsherbet_gp[] PROGMEM = {
    0, 255, 33,  4,
   43, 255, 68, 25,
   86, 255,  7, 25,
  127, 255, 82,103,
  170, 255,255,242,
  209,  42,255, 22,
  255,  87,255, 65};


// Gradient palette "gr65_hult_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/hult/tn/gr65_hult.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.

const byte gr65_hult_gp[] PROGMEM = {
    0, 247,176,247,
   48, 255,136,255,
   89, 220, 29,226,
  160,   7, 82,178,
  216,   1,124,109,
  255,   1,124,109};


// Gradient palette "gr64_hult_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/hult/tn/gr64_hult.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 32 bytes of program space.

const byte gr64_hult_gp[] PROGMEM = {
    0,   1,124,109,
   66,   1, 93, 79,
  104,  52, 65,  1,
  130, 115,127,  1,
  150,  52, 65,  1,
  201,   1, 86, 72,
  239,   0, 55, 45,
  255,   0, 55, 45};


// Gradient palette "GMT_drywet_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/gmt/tn/GMT_drywet.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

const byte GMT_drywet_gp[] PROGMEM = {
    0,  47, 30,  2,
   42, 213,147, 24,
   84, 103,219, 52,
  127,   3,219,207,
  170,   1, 48,214,
  212,   1,  1,111,
  255,   1,  7, 33};


// Gradient palette "ib15_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ing/general/tn/ib15.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.

const byte ib15_gp[] PROGMEM = {
    0, 113, 91,147,
   72, 157, 88, 78,
   89, 208, 85, 33,
  107, 255, 29, 11,
  141, 137, 31, 39,
  255,  59, 33, 89};


// Gradient palette "Tertiary_01_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/vermillion/tn/Tertiary_01.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

const byte Tertiary_01_gp[] PROGMEM = {
    0,   0,  1,255,
   63,   3, 68, 45,
  127,  23,255,  0,
  191, 100, 68,  1,
  255, 255,  1,  4};


// Gradient palette "lava_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/neota/elem/tn/lava.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 52 bytes of program space.

const byte lava_gp[] PROGMEM = {
    0,   0,  0,  0,
   46,  18,  0,  0,
   96, 113,  0,  0,
  108, 142,  3,  1,
  119, 175, 17,  1,
  146, 213, 44,  2,
  174, 255, 82,  4,
  188, 255,115,  4,
  202, 255,156,  4,
  218, 255,203,  4,
  234, 255,255,  4,
  244, 255,255, 71,
  255, 255,255,255};


// Gradient palette "fierce_ice_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/neota/elem/tn/fierce-ice.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

const byte fierce_ice_gp[] PROGMEM = {
    0,   0,  0,  0,
   59,   0,  9, 45,
  119,   0, 38,255,
  149,   3,100,255,
  180,  23,199,255,
  217, 100,235,255,
  255, 255,255,255};


// Gradient palette "Colorfull_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/atmospheric/tn/Colorfull.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 44 bytes of program space.

const byte Colorfull_gp[] PROGMEM = {
    0,  10, 85,  5,
   25,  29,109, 18,
   60,  59,138, 42,
   93,  83, 99, 52,
  106, 110, 66, 64,
  109, 123, 49, 65,
  113, 139, 35, 66,
  116, 192,117, 98,
  124, 255,255,137,
  168, 100,180,155,
  255,  22,121,174};


// Gradient palette "Pink_Purple_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/atmospheric/tn/Pink_Purple.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 44 bytes of program space.

const byte Pink_Purple_gp[] PROGMEM = {
    0,  19,  2, 39,
   25,  26,  4, 45,
   51,  33,  6, 52,
   76,  68, 62,125,
  102, 118,187,240,
  109, 163,215,247,
  114, 217,244,255,
  122, 159,149,221,
  149, 113, 78,188,
  183, 128, 57,155,
  255, 146, 40,123};


// Gradient palette "Sunset_Real_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/atmospheric/tn/Sunset_Real.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

const byte Sunset_Real_gp[] PROGMEM = {
    0, 120,  0,  0,
   22, 179, 22,  0,
   51, 255,104,  0,
   85, 167, 22, 18,
  135, 100,  0,103,
  198,  16,  0,130,
  255,   0,  0,160};


// Gradient palette "Sunset_Yellow_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/atmospheric/tn/Sunset_Yellow.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 44 bytes of program space.

const byte Sunset_Yellow_gp[] PROGMEM = {
    0,  10, 62,123,
   36,  56,130,103,
   87, 153,225, 85,
  100, 199,217, 68,
  107, 255,207, 54,
  115, 247,152, 57,
  120, 239,107, 61,
  128, 247,152, 57,
  180, 255,207, 54,
  223, 255,227, 48,
  255, 255,248, 42};


// Gradient palette "Beech_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/atmospheric/tn/Beech.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 60 bytes of program space.

const byte Beech_gp[] PROGMEM = {
    0, 255,252,214,
   12, 255,252,214,
   22, 255,252,214,
   26, 190,191,115,
   28, 137,141, 52,
   28, 112,255,205,
   50,  51,246,214,
   71,  17,235,226,
   93,   2,193,199,
  120,   0,156,174,
  133,   1,101,115,
  136,   1, 59, 71,
  136,   7,131,170,
  208,   1, 90,151,
  255,   0, 56,133};


// Gradient palette "Another_Sunset_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/atmospheric/tn/Another_Sunset.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 32 bytes of program space.

const byte Another_Sunset_gp[] PROGMEM = {
    0, 110, 49, 11,
   29,  55, 34, 10,
   68,  22, 22,  9,
   68, 239,124,  8,
   97, 220,156, 27,
  124, 203,193, 61,
  178,  33, 53, 56,
  255,   0,  1, 52};





// Gradient palette "es_autumn_19_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/autumn/tn/es_autumn_19.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 52 bytes of program space.

const byte es_autumn_19_gp[] PROGMEM = {
    0,  26,  1,  1,
   51,  67,  4,  1,
   84, 118, 14,  1,
  104, 137,152, 52,
  112, 113, 65,  1,
  122, 133,149, 59,
  124, 137,152, 52,
  135, 113, 65,  1,
  142, 139,154, 46,
  163, 113, 13,  1,
  204,  55,  3,  1,
  249,  17,  1,  1,
  255,  17,  1,  1};


// Gradient palette "BlacK_Blue_Magenta_White_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/basic/tn/BlacK_Blue_Magenta_White.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

const byte BlacK_Blue_Magenta_White_gp[] PROGMEM = {
    0,   0,  0,  0,
   42,   0,  0, 45,
   84,   0,  0,255,
  127,  42,  0,255,
  170, 255,  0,255,
  212, 255, 55,255,
  255, 255,255,255};


// Gradient palette "BlacK_Magenta_Red_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/basic/tn/BlacK_Magenta_Red.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

const byte BlacK_Magenta_Red_gp[] PROGMEM = {
    0,   0,  0,  0,
   63,  42,  0, 45,
  127, 255,  0,255,
  191, 255,  0, 45,
  255, 255,  0,  0};


// Gradient palette "BlacK_Red_Magenta_Yellow_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/basic/tn/BlacK_Red_Magenta_Yellow.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

const byte BlacK_Red_Magenta_Yellow_gp[] PROGMEM = {
    0,   0,  0,  0,
   42,  42,  0,  0,
   84, 255,  0,  0,
  127, 255,  0, 45,
  170, 255,  0,255,
  212, 255, 55, 45,
  255, 255,255,  0};


// Gradient palette "Blue_Cyan_Yellow_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/basic/tn/Blue_Cyan_Yellow.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

const byte Blue_Cyan_Yellow_gp[] PROGMEM = {
    0,   0,  0,255,
   63,   0, 55,255,
  127,   0,255,255,
  191,  42,255, 45,
  255, 255,255,  0};


//Custom palette by Aircoookie

const byte Orange_Teal_gp[] PROGMEM = {
    0,   0,150, 92,
   55,   0,150, 92,
  200, 255, 72,  0,
  255, 255, 72,  0};

//Custom palette by Aircoookie

const byte Tiamat_gp[] PROGMEM = {
    0,   1,  2, 14, //gc
   33,   2,  5, 35, //gc from 47, 61,126
  100,  13,135, 92, //gc from 88,242,247
  120,  43,255,193, //gc from 135,255,253
  140, 247,  7,249, //gc from 252, 69,253
  160, 193, 17,208, //gc from 231, 96,237
  180,  39,255,154, //gc from 130, 77,213
  200,   4,213,236, //gc from 57,122,248
  220,  39,252,135, //gc from 177,254,255
  240, 193,213,253, //gc from 203,239,253
  255, 255,249,255};
  
//Custom palette by Aircoookie

const byte April_Night_gp[] PROGMEM = {
    0,   1,  5, 45, //deep blue
   10,   1,  5, 45,
   25,   5,169,175, //light blue
   40,   1,  5, 45,
   61,   1,  5, 45,
   76,  45,175, 31, //green
   91,   1,  5, 45,
  112,   1,  5, 45,
  127, 249,150,  5, //yellow
  143,   1,  5, 45,
  162,   1,  5, 45,
  178, 255, 92,  0, //pastel orange
  193,   1,  5, 45,
  214,   1,  5, 45,
  229, 223, 45, 72, //pink
  244,   1,  5, 45,
  255,   1,  5, 45};

const byte Orangery_gp[] PROGMEM = {
    0, 255, 95, 23,
   30, 255, 82,  0,
   60, 223, 13,  8,
   90, 144, 44,  2,
  120, 255,110, 17,
  150, 255, 69,  0,
  180, 158, 13, 11,
  210, 241, 82, 17,
  255, 213, 37,  4};

//inspired by Mark Kriegsman https://gist.github.com/kriegsman/756ea6dcae8e30845b5a
const byte C9_gp[] PROGMEM = {
    0, 184,  4,  0, //red
   60, 184,  4,  0,
   65, 144, 44,  2, //amber
  125, 144, 44,  2,
  130,   4, 96,  2, //green
  190,   4, 96,  2,
  195,   7,  7, 88, //blue
  255,   7,  7, 88};

const byte Sakura_gp[] PROGMEM = {
    0, 196, 19, 10,
   65, 255, 69, 45,
  130, 223, 45, 72,
  195, 255, 82,103,
  255, 223, 13, 17};

const byte Aurora_gp[] PROGMEM = {
    0,   1,  5, 45, //deep blue
   64,   0,200, 23,
  128,   0,255,  0, //green
  170,   0,243, 45,
  200,   0,135,  7,
  255,   1,  5, 45};//deep blue

const byte Atlantica_gp[] PROGMEM = {
    0,   0, 28,112, //#001C70
   50,  32, 96,255, //#2060FF
  100,   0,243, 45,
  150,  12, 95, 82, //#0C5F52
  200,  25,190, 95, //#19BE5F
  255,  40,170, 80};//#28AA50
  

// Single array of defined cpt-city color palettes.
// This will let us programmatically choose one based on
// a number, rather than having to activate each explicitly
// by name every time.
const byte* const gGradientPalettes[] PROGMEM = {
  Sunset_Real_gp,               //13-00 Sunset
  es_rivendell_15_gp,           //14-01 Rivendell
  es_ocean_breeze_036_gp,       //15-02 Breeze
  rgi_15_gp,                    //16-03 Red & Blue
  retro2_16_gp,                 //17-04 Yellowout
  Analogous_1_gp,               //18-05 Analogous
  es_pinksplash_08_gp,          //19-06 Splash
  Sunset_Yellow_gp,             //20-07 Pastel
  Another_Sunset_gp,            //21-08 Sunset2
  Beech_gp,                     //22-09 Beech
  es_vintage_01_gp,             //23-10 Vintage
  departure_gp,                 //24-11 Departure
  es_landscape_64_gp,           //25-12 Landscape
  es_landscape_33_gp,           //26-13 Beach
  rainbowsherbet_gp,            //27-14 Sherbet
  gr65_hult_gp,                 //28-15 Hult
  gr64_hult_gp,                 //29-16 Hult64
  GMT_drywet_gp,                //30-17 Drywet
  ib_jul01_gp,                  //31-18 Jul
  es_vintage_57_gp,             //32-19 Grintage
  ib15_gp,                      //33-20 Rewhi
  Tertiary_01_gp,               //34-21 Tertiary
  lava_gp,                      //35-22 Fire
  fierce_ice_gp,                //36-23 Icefire
  Colorfull_gp,                 //37-24 Cyane
  Pink_Purple_gp,               //38-25 Light Pink
  es_autumn_19_gp,              //39-26 Autumn
  BlacK_Blue_Magenta_White_gp,  //40-27 Magenta
  BlacK_Magenta_Red_gp,         //41-28 Magred
  BlacK_Red_Magenta_Yellow_gp,  //42-29 Yelmag
  Blue_Cyan_Yellow_gp,          //43-30 Yelblu
  Orange_Teal_gp,               //44-31 Orange & Teal
  Tiamat_gp,                    //45-32 Tiamat
  April_Night_gp,               //46-33 April Night
  Orangery_gp,                  //47-34 Orangery
  C9_gp,                        //48-35 C9
  Sakura_gp,                    //49-36 Sakura
  Aurora_gp,                    //50-37 Aurora
  Atlantica_gp,                 //51-38 Atlantica
};

// #endif // ENABLE_CRGBPALETTES_IN_PROGMEM



#endif
