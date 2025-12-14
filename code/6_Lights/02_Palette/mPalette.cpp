#include "mPalette.h"


#ifdef USE_MODULE_LIGHTS_INTERFACE


mPalette* mPalette::instance = nullptr;

mPalette* mPalette::GetInstance()
{
  if (instance == nullptr){
    instance = new mPalette();
  }
  return instance;
}


/*********************************************************************************************************************************************************************************
 *********************************************************************************************************************************************************************************
 * SECTION: Init
 *********************************************************************************************************************************************************************************
 *********************************************************************************************************************************************************************************/


void mPalette::Init_Palettes()
{
  
  ALOG_DBM( PSTR("Init_Palettes") );
  
  /****************
   * StaticPalettes
   ****************/

  addStaticPalette(
    PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID, 
    PM_PALETTE_COLOURFUL_DEFAULT__DATA, 
    sizeof(PM_PALETTE_COLOURFUL_DEFAULT__DATA),
    D_PALETTE_COLOURFUL_DEFAULT_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_HOLLOWEEN_OP__ID, 
    PM_PALETTE_HOLLOWEEN_OP__DATA, 
    sizeof(PM_PALETTE_HOLLOWEEN_OP__DATA),
    D_PALETTE_HOLLOWEEN_OP_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_HOLLOWEEN_OGP__ID, 
    PM_PALETTE_HOLLOWEEN_OGP__DATA, 
    sizeof(PM_PALETTE_HOLLOWEEN_OGP__DATA),
    D_PALETTE_HOLLOWEEN_OGP_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_RAINBOW__ID, 
    PM_PALETTE_RAINBOW__DATA, 
    sizeof(PM_PALETTE_RAINBOW__DATA),
    D_PALETTE_RAINBOW_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_RAINBOW_WARM__ID, 
    PM_PALETTE_RAINBOW_WARM__DATA, 
    sizeof(PM_PALETTE_RAINBOW_WARM__DATA),
    D_PALETTE_RAINBOW_WARM_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_RAINBOW_INVERTED__ID, 
    PM_PALETTE_RAINBOW_INVERTED__DATA, 
    sizeof(PM_PALETTE_RAINBOW_INVERTED__DATA),
    D_PALETTE_RAINBOW_INVERTED_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_PASTEL_01__ID, 
    PM_PALETTE_PASTEL_01__DATA, 
    sizeof(PM_PALETTE_PASTEL_01__DATA),
    D_PALETTE_PASTEL_01_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_PASTEL_02__ID, 
    PM_PALETTE_PASTEL_02__DATA, 
    sizeof(PM_PALETTE_PASTEL_02__DATA),
    D_PALETTE_PASTEL_02_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_PASTEL_03__ID, 
    PM_PALETTE_PASTEL_03__DATA, 
    sizeof(PM_PALETTE_PASTEL_03__DATA),
    D_PALETTE_PASTEL_03_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_PASTEL_04__ID, 
    PM_PALETTE_PASTEL_04__DATA, 
    sizeof(PM_PALETTE_PASTEL_04__DATA),
    D_PALETTE_PASTEL_04_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_PASTEL_05__ID, 
    PM_PALETTE_PASTEL_05__DATA, 
    sizeof(PM_PALETTE_PASTEL_05__DATA),
    D_PALETTE_PASTEL_05_ENCODING
  );
  
  addStaticPalette(
    PALETTELIST_STATIC_GRADIENT_PASTEL_TONES_PURPLE__ID, 
    PM_PALETTE_GRADIENT_PASTEL_TONES_PURPLE__DATA, 
    sizeof(PM_PALETTE_GRADIENT_PASTEL_TONES_PURPLE__DATA),
    D_PALETTE_GRADIENT_PASTEL_TONES_PURPLE_ENCODING
  );
  
  addStaticPalette(
    PALETTELIST_STATIC_FLOWER_SWEATPEA__ID, 
    PM_PALETTE_FLOWER_SWEATPEA__DATA, 
    sizeof(PM_PALETTE_FLOWER_SWEATPEA__DATA),
    D_PALETTE_FLOWER_SWEATPEA_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_PINK_PURPLE__ID, 
    PM_PALETTE_PINK_PURPLE__DATA, 
    sizeof(PM_PALETTE_PINK_PURPLE__DATA),
    D_PALETTE_PINK_PURPLE_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_PURPLE_PINK__ID, 
    PM_PALETTE_PURPLE_PINK__DATA, 
    sizeof(PM_PALETTE_PURPLE_PINK__DATA),
    D_PALETTE_PURPLE_PINK_ENCODING
  );
  
  addStaticPalette(
    PALETTELIST_STATIC_HOT_PINK_NEON_WITH_NAVY__ID, 
    PM_PALETTE_HOT_PINK_NEON_WITH_NAVY__DATA, 
    sizeof(PM_PALETTE_HOT_PINK_NEON_WITH_NAVY__DATA),
    D_PALETTE_HOT_PINK_NEON_WITH_NAVY_ENCODING
  );


  addStaticPalette(
    PALETTELIST_STATIC_WINTER_01__ID, 
    PM_PALETTE_WINTER_01__DATA, 
    sizeof(PM_PALETTE_WINTER_01__DATA),
    D_PALETTE_WINTER_01_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_WINTER_02__ID, 
    PM_PALETTE_WINTER_02__DATA, 
    sizeof(PM_PALETTE_WINTER_02__DATA),
    D_PALETTE_WINTER_02_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_WINTER_03__ID, 
    PM_PALETTE_WINTER_03__DATA, 
    sizeof(PM_PALETTE_WINTER_03__DATA),
    D_PALETTE_WINTER_03_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_WINTER_04__ID, 
    PM_PALETTE_WINTER_04__DATA, 
    sizeof(PM_PALETTE_WINTER_04__DATA),
    D_PALETTE_WINTER_04_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_AUTUMN_GREEN__ID, 
    PM_PALETTE_AUTUMN_GREEN__DATA, 
    sizeof(PM_PALETTE_AUTUMN_GREEN__DATA),
    D_PALETTE_AUTUMN_GREEN_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_AUTUMN_RED__ID, 
    PM_PALETTE_AUTUMN_RED__DATA, 
    sizeof(PM_PALETTE_AUTUMN_RED__DATA),
    D_PALETTE_AUTUMN_RED_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__FESTIVE_TRADITIONAL_RGPBO__ID, 
    PM_PALETTE__CHRISTMAS_TRADITIONAL_RGPBO__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_TRADITIONAL_RGPBO__DATA),
    D_PALETTE__CHRISTMAS_TRADITIONAL_RGPBO__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__FESTIVE_TRADITIONAL_RGPBY__ID, 
    PM_PALETTE__CHRISTMAS_TRADITIONAL_RGPBY__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_TRADITIONAL_RGPBY__DATA),
    D_PALETTE__CHRISTMAS_TRADITIONAL_RGPBY__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__FESTIVE_TRADITIONAL_ROGPBY__ID, 
    PM_PALETTE__CHRISTMAS_TRADITIONAL_ROGPBY__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_TRADITIONAL_ROGPBY__DATA),
    D_PALETTE__CHRISTMAS_TRADITIONAL_ROGPBY__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__FESTIVE_TRADITIONAL_RGBO__ID, 
    PM_PALETTE__CHRISTMAS_TRADITIONAL_RGBO__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_TRADITIONAL_RGBO__DATA),
    D_PALETTE__CHRISTMAS_TRADITIONAL_RGBO__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__FESTIVE_TRADITIONAL_RGBY__ID, 
    PM_PALETTE__CHRISTMAS_TRADITIONAL_RGBY__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_TRADITIONAL_RGBY__DATA),
    D_PALETTE__CHRISTMAS_TRADITIONAL_RGBY__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__FESTIVE_VINTAGE_MINIBELLS__ID, 
    PM_PALETTE__CHRISTMAS_VINTAGE_MINIBELLS__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_VINTAGE_MINIBELLS__DATA),
    D_PALETTE__CHRISTMAS_VINTAGE_MINIBELLS__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__FESTIVE_VINTAGE_MERRYLITES__ID, 
    PM_PALETTE__CHRISTMAS_VINTAGE_MERRYLITES__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_VINTAGE_MERRYLITES__DATA),
    D_PALETTE__CHRISTMAS_VINTAGE_MERRYLITES__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__FESTIVE_VINTAGE_AGED_BULBS__ID, 
    PM_PALETTE__CHRISTMAS_VINTAGE_AGED_BULBS__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_VINTAGE_AGED_BULBS__DATA),
    D_PALETTE__CHRISTMAS_VINTAGE_AGED_BULBS__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__FESTIVE_SNOWY_COLOURS_01__ID, 
    PM_PALETTE__CHRISTMAS_SNOWY_COLOURS_01__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_SNOWY_COLOURS_01__DATA),
    D_PALETTE__CHRISTMAS_SNOWY_COLOURS_01__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__FESTIVE_SNOWY_COLOURS_02__ID, 
    PM_PALETTE__CHRISTMAS_SNOWY_COLOURS_02__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_SNOWY_COLOURS_02__DATA),
    D_PALETTE__CHRISTMAS_SNOWY_COLOURS_02__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__FESTIVE_SNOWY_COLOURS_03__ID, 
    PM_PALETTE__CHRISTMAS_SNOWY_COLOURS_03__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_SNOWY_COLOURS_03__DATA),
    D_PALETTE__CHRISTMAS_SNOWY_COLOURS_03__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__FESTIVE_BERRY_YELLOW__ID, 
    PM_PALETTE__CHRISTMAS_BERRY_YELLOW__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_BERRY_YELLOW__DATA),
    D_PALETTE__CHRISTMAS_BERRY_YELLOW__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__FESTIVE_BERRY_ORANGE__ID, 
    PM_PALETTE__CHRISTMAS_BERRY_ORANGE__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_BERRY_ORANGE__DATA),
    D_PALETTE__CHRISTMAS_BERRY_ORANGE__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__FESTIVE_BERRY_GREEN__ID, 
    PM_PALETTE__CHRISTMAS_BERRY_GREEN__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_BERRY_GREEN__DATA),
    D_PALETTE__CHRISTMAS_BERRY_GREEN__ENCODING
  );


  // END Christmas

  addStaticPalette(
    PALETTELIST_STATIC__COLOURFUL_PAIRS_01__ID, 
    PM_PALETTE__COLOURFUL_PAIRS_01__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_PAIRS_01__DATA),
    D_PALETTE__COLOURFUL_PAIRS_01__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__COLOURFUL_COLOUR_WHITE_STRIPE__ID, 
    PM_PALETTE__COLOURFUL_COLOUR_WHITE_STRIPE__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_COLOUR_WHITE_STRIPE__DATA),
    D_PALETTE__COLOURFUL_COLOUR_WHITE_STRIPE__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__COLOURFUL_COLOUR_WARMWHITE_STRIPE__ID, 
    PM_PALETTE__COLOURFUL_COLOUR_WARMWHITE_STRIPE__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_COLOUR_WARMWHITE_STRIPE__DATA),
    D_PALETTE__COLOURFUL_COLOUR_WARMWHITE_STRIPE__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__COLOURFUL_BLUE_WHITE__ID, 
    PM_PALETTE__COLOURFUL_BLUE_WHITE__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_BLUE_WHITE__DATA),
    D_PALETTE__COLOURFUL_BLUE_WHITE__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__COLOURFUL_WHITE_MIXED__ID, 
    PM_PALETTE__COLOURFUL_WHITE_MIXED__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_WHITE_MIXED__DATA),
    D_PALETTE__COLOURFUL_WHITE_MIXED__ENCODING
  );


  addStaticPalette(
    PALETTELIST_STATIC__COLOURFUL_PINK_RED_YELLOW__ID, 
    PM_PALETTE__COLOURFUL_PINK_RED_YELLOW__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_PINK_RED_YELLOW__DATA),
    D_PALETTE__COLOURFUL_PINK_RED_YELLOW__ENCODING
  );


  addStaticPalette(
    PALETTELIST_STATIC__COLOURFUL_PEACHY_ORANGE__ID, 
    PM_PALETTE__COLOURFUL_PEACHY_ORANGE__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_PEACHY_ORANGE__DATA),
    D_PALETTE__COLOURFUL_PEACHY_ORANGE__ENCODING
  );


  addStaticPalette(
    PALETTELIST_STATIC__COLOURFUL_PEACHY_YELLOW__ID, 
    PM_PALETTE__COLOURFUL_PEACHY_YELLOW__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_PEACHY_YELLOW__DATA),
    D_PALETTE__COLOURFUL_PEACHY_YELLOW__ENCODING
  );


  addStaticPalette(
    PALETTELIST_STATIC__COLOURFUL_GREENLESS__ID, 
    PM_PALETTE__COLOURFUL_GREENLESS__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_GREENLESS__DATA),
    D_PALETTE__COLOURFUL_GREENLESS__ENCODING
  );
  
  addStaticPalette(
    PALETTELIST_STATIC__GOLDEN__ID, 
    PM_PALETTE__GOLDEN__DATA, 
    sizeof(PM_PALETTE__GOLDEN__DATA),
    D_PALETTE__GOLDEN__ENCODING
  );


  addStaticPalette(
    PALETTELIST_STATIC_SUNRISE_01__ID, 
    PM_PALETTE_SUNRISE_01__DATA, 
    sizeof(PM_PALETTE_SUNRISE_01__DATA),
    D_PALETTE_SUNRISE_01_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_SUNRISE_02__ID, 
    PM_PALETTE_SUNRISE_02__DATA, 
    sizeof(PM_PALETTE_SUNRISE_02__DATA),
    D_PALETTE_SUNRISE_02_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_SUNRISE_03__ID, 
    PM_PALETTE_SUNRISE_03__DATA, 
    sizeof(PM_PALETTE_SUNRISE_03__DATA),
    D_PALETTE_SUNRISE_03_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_SUNRISE_04__ID, 
    PM_PALETTE_SUNRISE_04__DATA, 
    sizeof(PM_PALETTE_SUNRISE_04__DATA),
    D_PALETTE_SUNRISE_04_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_SUNSET_01__ID, 
    PM_PALETTE_SUNSET_01__DATA, 
    sizeof(PM_PALETTE_SUNSET_01__DATA),
    D_PALETTE_SUNSET_01_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_SUNSET_02__ID, 
    PM_PALETTE_SUNSET_02__DATA, 
    sizeof(PM_PALETTE_SUNSET_02__DATA),
    D_PALETTE_SUNSET_02_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_SUNSET_RED__ID, 
    PM_PALETTE_SUNSET_RED__DATA, 
    sizeof(PM_PALETTE_SUNSET_RED__DATA),
    D_PALETTE_SUNSET_RED_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_SUNSET_BEACH__ID, 
    PM_PALETTE_SUNSET_BEACH__DATA, 
    sizeof(PM_PALETTE_SUNSET_BEACH__DATA),
    D_PALETTE_SUNSET_BEACH_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_SKY_GLOW_01__ID, 
    PM_PALETTE_SKY_GLOW_01__DATA, 
    sizeof(PM_PALETTE_SKY_GLOW_01__DATA),
    D_PALETTE_SKY_GLOW_01_ENCODING
  );
  
  addStaticPalette(
    PALETTELIST_STATIC__COLOURFUL_WITH_CCT_01__ID, 
    PM_PALETTE__COLOURFUL_WITH_CCT_01__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_WITH_CCT_01__DATA),
    D_PALETTE__COLOURFUL_WITH_CCT_01__ENCODING
  );
  
  addStaticPalette(
    PALETTELIST_STATIC_CANDLE_FLAME_01__ID, 
    PM_PALETTE_CANDLE_FLAME_01__DATA, 
    sizeof(PM_PALETTE_CANDLE_FLAME_01__DATA),
    D_PALETTE_CANDLE_FLAME_01_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_GRADIENT_FIRE_01__ID, 
    PM_PALETTE_GRADIENT_FIRE_01__DATA, 
    sizeof(PM_PALETTE_GRADIENT_FIRE_01__DATA),
    D_PALETTE_GRADIENT_FIRE_01_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_OCEAN_01__ID, 
    PM_PALETTE_OCEAN_01__DATA, 
    sizeof(PM_PALETTE_OCEAN_01__DATA),
    D_PALETTE_OCEAN_01_ENCODING
  );


  /****************
   * DynamicPalettes : Do all dynamic palettes have no data? actually, I may want to encode data to be used for these (not RGB data, but byte encoded data), so keep
   ****************/
  
   addDynamicPalette(
    PALETTELIST_DYNAMIC__SOLAR_ELEVATION__WHITE_COLOUR_TEMPERATURE_01__ID, 
    PALETTELIST_DYNAMIC__SOLAR_ELEVATION__WHITE_COLOUR_TEMPERATURE_01__DATA, 
    sizeof(PALETTELIST_DYNAMIC__SOLAR_ELEVATION__WHITE_COLOUR_TEMPERATURE_01__DATA),
    PALETTELIST_DYNAMIC__SOLAR_ELEVATION__WHITE_COLOUR_TEMPERATURE_01__ENCODING
  );
  
  addDynamicPalette(
    PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SEGMENT_COLOUR_BLEND_DAYTIME_01__ID, 
    PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SEGMENT_COLOUR_BLEND_DAYTIME_01__DATA, 
    sizeof(PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SEGMENT_COLOUR_BLEND_DAYTIME_01__DATA),
    PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SEGMENT_COLOUR_BLEND_DAYTIME_01__ENCODING
  );
  
  addDynamicPalette(
    PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SEGMENT_COLOUR_BLEND_DAWNDUSKTIME_01__ID, 
    PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SEGMENT_COLOUR_BLEND_DAWNDUSKTIME_01__DATA, 
    sizeof(PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SEGMENT_COLOUR_BLEND_DAWNDUSKTIME_01__DATA),
    PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SEGMENT_COLOUR_BLEND_DAWNDUSKTIME_01__ENCODING
  );
  
  addDynamicPalette(
    PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SEGMENT_COLOUR_BLEND_NIGHTTIME_01__ID, 
    PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SEGMENT_COLOUR_BLEND_NIGHTTIME_01__DATA, 
    sizeof(PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SEGMENT_COLOUR_BLEND_NIGHTTIME_01__DATA),
    PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SEGMENT_COLOUR_BLEND_NIGHTTIME_01__ENCODING
  );
  
  addDynamicPalette(
    PALETTELIST_DYNAMIC__TIMEREACTIVE__SEGMENT_COLOUR__MINUTE_BLEND__ID, 
    PALETTELIST_DYNAMIC__TIMEREACTIVE__SEGMENT_COLOUR__MINUTE_BLEND__DATA, 
    sizeof(PALETTELIST_DYNAMIC__TIMEREACTIVE__SEGMENT_COLOUR__MINUTE_BLEND__DATA),
    PALETTELIST_DYNAMIC__TIMEREACTIVE__SEGMENT_COLOUR__MINUTE_BLEND__ENCODING
  );
  
  addDynamicPalette(
    PALETTELIST_DYNAMIC__TIMEREACTIVE__SEGMENT_COLOUR__HOUR_BLEND__ID, 
    PALETTELIST_DYNAMIC__TIMEREACTIVE__SEGMENT_COLOUR__HOUR_BLEND__DATA, 
    sizeof(PALETTELIST_DYNAMIC__TIMEREACTIVE__SEGMENT_COLOUR__HOUR_BLEND__DATA),
    PALETTELIST_DYNAMIC__TIMEREACTIVE__SEGMENT_COLOUR__HOUR_BLEND__ENCODING
  );
  
  addDynamicPalette(
    PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SOLID_COLOUR_OF_SKY__ID, 
    PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SOLID_COLOUR_OF_SKY__DATA, 
    sizeof(PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SOLID_COLOUR_OF_SKY__DATA),
    PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SOLID_COLOUR_OF_SKY__ENCODING
  );

  addDynamicPalette(
    PALETTELIST_DYNAMIC__SOLAR_ELEVATION__GRADIENT_COLOUR_OF_SKY__ID, 
    PALETTELIST_DYNAMIC__SOLAR_ELEVATION__GRADIENT_COLOUR_OF_SKY__DATA, 
    sizeof(PALETTELIST_DYNAMIC__SOLAR_ELEVATION__GRADIENT_COLOUR_OF_SKY__DATA),
    PALETTELIST_DYNAMIC__SOLAR_ELEVATION__GRADIENT_COLOUR_OF_SKY__ENCODING
  );
  

  /****************
   * CustomPalettes
   ****************/


  addCustomPalette(
    0, 
    PM_PALETTE_CUSTOM_PALETTE_DEFAULT_01__DATA, 
    sizeof(PM_PALETTE_CUSTOM_PALETTE_DEFAULT_01__DATA), 
    D_PALETTE_CUSTOM_PALETTE_DEFAULT_01_ENCODING
  );
    
  
  addCustomPalette(
    1, 
    PM_PALETTE_CUSTOM_PALETTE_DEFAULT_02__DATA, 
    sizeof(PM_PALETTE_CUSTOM_PALETTE_DEFAULT_02__DATA), 
    D_PALETTE_CUSTOM_PALETTE_DEFAULT_02_ENCODING
  );
  
  addCustomPalette(
    2, 
    PM_PALETTE_CUSTOM_PALETTE_DEFAULT_03__DATA, 
    sizeof(PM_PALETTE_CUSTOM_PALETTE_DEFAULT_03__DATA), 
    D_PALETTE_CUSTOM_PALETTE_DEFAULT_03_ENCODING
  );
  
  addCustomPalette(
    3, 
    PM_PALETTE_CUSTOM_PALETTE_DEFAULT_04__DATA, 
    sizeof(PM_PALETTE_CUSTOM_PALETTE_DEFAULT_04__DATA), 
    D_PALETTE_CUSTOM_PALETTE_DEFAULT_04_ENCODING
  );
  
  addCustomPalette(
    4, 
    PM_PALETTE_CUSTOM_PALETTE_DEFAULT_05__DATA, 
    sizeof(PM_PALETTE_CUSTOM_PALETTE_DEFAULT_05__DATA), 
    D_PALETTE_CUSTOM_PALETTE_DEFAULT_05_ENCODING
  );
  
  addCustomPalette(
    5, 
    PM_PALETTE_CUSTOM_PALETTE_DEFAULT_06__DATA, 
    sizeof(PM_PALETTE_CUSTOM_PALETTE_DEFAULT_06__DATA), 
    D_PALETTE_CUSTOM_PALETTE_DEFAULT_06_ENCODING
  );
  
  addCustomPalette(
    6, 
    PM_PALETTE_CUSTOM_PALETTE_DEFAULT_07__DATA, 
    sizeof(PM_PALETTE_CUSTOM_PALETTE_DEFAULT_07__DATA), 
    D_PALETTE_CUSTOM_PALETTE_DEFAULT_07_ENCODING
  );
  
  addCustomPalette(
    7, 
    PM_PALETTE_CUSTOM_PALETTE_DEFAULT_08__DATA, 
    sizeof(PM_PALETTE_CUSTOM_PALETTE_DEFAULT_08__DATA), 
    D_PALETTE_CUSTOM_PALETTE_DEFAULT_08_ENCODING
  );
  
  addCustomPalette(
    8, 
    PM_PALETTE_CUSTOM_PALETTE_DEFAULT_09__DATA, 
    sizeof(PM_PALETTE_CUSTOM_PALETTE_DEFAULT_09__DATA), 
    D_PALETTE_CUSTOM_PALETTE_DEFAULT_09_ENCODING
  );
  
  #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_NO_GRADIENT  
  uint8_t test_data2[] = { // later use one of the static as a preload/default
    1,1,10, // 0
    2,2,20, // 1
    3,3,30, // 2
    4,4,40, // 3
    5,5,50, // 4
  };
  addDynamicPalette(                           // init this always with something basic, later to be changed and reloaded
    9, 
    test_data, 
    sizeof(test_data), 
    PALETTE_ENCODING_TYPE_RGB_NO_INDEX
  );
  #elif defined(ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_WITH_GRADIENT)
  uint8_t test_data2[] = { // later use one of the static as a preload/default
    0,   255,0,0, // 0
    1,   0,255,0, // 0
    2,   0,0,255, // 0
    49,  255,0,0, // 1
    55,  0,255,0, // 1
    150, 0,0,255, // 2
    200, 255,0,255, // 3
    255, 0,255,255, // 4
  };
  addCustomPalette(                           // init this always with something basic, later to be changed and reloaded
    9, 
    test_data2, 
    sizeof(test_data2), 
    PALETTE_ENCODING_TYPE_RGB_WITHINDEX_GRADIENT
  );
  #else
  addCustomPalette(
    9, 
    PM_PALETTE_RAINBOW_WARM__DATA, 
    sizeof(PM_PALETTE_RAINBOW_WARM__DATA), 
    D_PALETTE_RAINBOW_WARM_ENCODING
  );
  #endif

}



void mPalette::addStaticPalette(uint16_t id, const uint8_t* data, const uint8_t length, uint16_t encoding)
{

  PALETTE_DATA palette_tmp;

  #ifdef ESP32
  for(int i=0;i<length;i++){ palette_tmp.data.push_back(data[i]); }
  #else // ESP8266
  uint8_t data_t[length];
  memcpy_P(data_t, data, length);
  for(int i=0;i<length;i++){ palette_tmp.data.push_back(data_t[i]); }
  #endif

  palette_tmp.encoding = {encoding};
  palette_tmp.palettelist_id = id;
  
  uint8_t encoded_colour_width  = GetEncodedColourWidth( palette_tmp.encoding );  
  palette_tmp.number_of_colours = length / encoded_colour_width;

  uint16_t id_adj = id - PALETTELIST_SEGMENT__SEGMENT_COLOUR_01__ID; // First static ID

  if (id_adj < custom_palettes.size()) {
    static_palettes[id_adj] = palette_tmp;
  } else {
    static_palettes.push_back(palette_tmp);
  }

}



void mPalette::addDynamicPalette(uint16_t id, const uint8_t* data, const uint8_t length, uint16_t encoding)
{

  // ALOG_INF(PSTR("addDynamicPalette %d"), id);

  PALETTE_DATA palette_tmp;

  #ifdef ESP32
  for(int i=0;i<length;i++){ palette_tmp.data.push_back(data[i]); }
  #else // ESP8266
  uint8_t data_t[length];
  memcpy_P(data_t, data, length);
  for(int i=0;i<length;i++){ palette_tmp.data.push_back(data_t[i]); }
  #endif
  
  #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
  ALOG_INF(PSTR("addDynamicPalette bytes added %d"), palette_tmp.data.size());
  #endif

  palette_tmp.encoding = {encoding};
  palette_tmp.palettelist_id = id;

  // Should be changed to a funciton that calculates the number of colours from the length and encoding, but only called at boottime
  if(length != 0)
  {
    uint8_t encoded_colour_width  = GetEncodedColourWidth( palette_tmp.encoding );  
    palette_tmp.number_of_colours = length / encoded_colour_width;
  }else{
    if(palette_tmp.encoding.encoded_as_crgb_palette_16)
    {
      palette_tmp.number_of_colours = 16; // to be moved into some kind of default value
    }
  }

  // dynamic id is offset by the number of static palettes
  uint8_t id_adj = id - PALETTELIST_DYNAMIC__COLOUR__ID_START;

  if (id_adj < dynamic_palettes.size()) {
    dynamic_palettes[id_adj] = palette_tmp;
  } else {
    dynamic_palettes.push_back(palette_tmp);
  }

  #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
  ALOG_INF(PSTR("addDynamicPalette bytes added[3] %d"), dynamic_palettes[3].data.size());
  for(int i=0;i<length;i++){ Serial.print( dynamic_palettes[3].data[i]);Serial.print( "," ); } Serial.println();
  #endif

}


void mPalette::addCustomPalette(uint16_t id, const uint8_t* data, const uint8_t length, uint16_t encoding)
{

  PALETTE_DATA palette_tmp;

  #ifdef ESP32
  for(int i=0;i<length;i++){ palette_tmp.data.push_back(data[i]); }
  #else // ESP8266
  uint8_t data_t[length];
  memcpy_P(data_t, data, length);
  for(int i=0;i<length;i++){ palette_tmp.data.push_back(data_t[i]); }
  #endif

  palette_tmp.encoding = {encoding};
  palette_tmp.palettelist_id = id;
  
  uint8_t encoded_colour_width  = GetEncodedColourWidth( palette_tmp.encoding );  
  palette_tmp.number_of_colours = length / encoded_colour_width;

  uint16_t id_adj = id;// - PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED; // First static ID

  if (id_adj < custom_palettes.size()) {
    custom_palettes[id_adj] = palette_tmp;
  } else {
    custom_palettes.push_back(palette_tmp);
  }

  #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
  ALOG_INF(PSTR("addCustomPalette bytes added[3] %d"), custom_palettes[3].data.size());
  for(int i=0;i<length;i++){ Serial.print( custom_palettes[3].data[i]);Serial.print( "," ); } Serial.println();
  #endif

}


mPalette::PALETTE_ENCODING_DATA mPalette::findPaletteEncoding(uint16_t id)
{
  // Search in static palettes
  for (const auto& palette : static_palettes) {
    if (palette.palettelist_id == id) {
      ALOG_INF(PSTR("Found in static palettes with ID: %d"), id);
      return palette.encoding;
    }
  }

  // Search in dynamic palettes
  for (const auto& palette : dynamic_palettes) {
    if (palette.palettelist_id == id) {
      ALOG_INF(PSTR("Found in dynamic palettes with ID: %d"), id);
      return palette.encoding;
    }
  }

  // Search in custom palettes
  for (const auto& palette : custom_palettes) {
    if (palette.palettelist_id == id) {
      ALOG_INF(PSTR("Found in custom palettes with ID: %d"), id);
      return palette.encoding;
    }
  }

  // If not found, return a default encoding type or error code
  ALOG_INF(PSTR("Palette with ID: %d not found"), id);
  return {0}; // You could also return an error code here if needed
}

// #define ENABLE_FEATURE_PALETTE__RGBWW_COLOURS


/*********************************************************************************************************************************************************************************
 *********************************************************************************************************************************************************************************
 * SECTION: Main requests to get colours from palettes
 *********************************************************************************************************************************************************************************
 *********************************************************************************************************************************************************************************/

/**
 * @brief Thought, maybe make a U8 for just the other white object, it would remove the need for two functions like this. 
 * This optionally could be like ignore, or repeat it across them all, again to speed things up.
 * 
 * Could use flag in segment of "WW enabled", so it can be disabled for processing speed when not needed.
 */

IRAM_ATTR [[gnu::hot]] uint32_t      mPalette::GetColourFromPreloadedPaletteBuffer_U32
(
  uint16_t id,
  // Pass preloaded palette data buffer. If nullptr, and "id" does not match any preloaded palette, then it will force a reload of the palette data.
  uint8_t* data,
  // In discrete mode, this index will automatically modulo and repeat the palette over infinite length (MAXU16). In Gradient mode, must be scaled in 0 to 255 range.
  uint16_t desired_index,
  // If the palette is encoded, then this returns encoded value at [desired_index] point. NOTE: Only in discrete mode.
  uint8_t* encoded_index,
  // Providing the index in range 0 to 255, this enabled will internally rescale the index to the segment length, so that the index is always in range 0 to segment_length-1
  uint8_t  palette_index__format,
  // CRGBPalette defaults gradient (index 240 to 255) wraps to blend with colour as index 0. This rescales to limit to 240, hence, removes wrap around blending.
  uint8_t  rescale_index_wrap_for_hardedge,
  // 0 = default, 1 = "Forced Discrete", 2 = "Forced Gradient"
  uint8_t  force_palette_mode, // flag_crgb_exact_colour = 0, // true: "CRGB exact colour", false: "U32 colour"
  // Requesting preview: Live palettes must respond with preview for UI
  bool flag_request_is_for_full_visual_output
){

  RgbwwColor colourRGBWW;
  uint32_t      colour32;

  /**************************************************************
   * 
   * CRGBPALETTE16 palette
   * * Preloaded into CRGB16Palette16_Palette
   * * Default requires getting from pal_index in 0 to 255 range
   * * * If palette_index__format is true, then it will rescale the index to segment length
   * * * If rescale_index_wrap_for_hardedge is true, then it will rescale the index to 240, so that it does not wrap around.
   * * * If override_default_encoding is set to PALETTE_ENCODING_OVERRIDE__FORCED_DISCRETE, then it will use the encoded_index to get the exact colour from CRGB16Palette16_Palette by rescaling 0-16 into 0-240 range
   * 
  ***************************************************************/
  // if(
  //   ((id >= PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID)                      && (id < PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID)) ||
  //   ((id >= PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID)                     && (id < PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID))  ||
  //   ((id >= PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID)     && (id < PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__LENGTH__ID)) ||
  //   ((id >= PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__RANDOMISE_COLOURS_01__ID) && (id < PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__LENGTH__ID))
  // ){
    
  //   uint16_t pixel_position_adjust = desired_index;

  //   TBlendType blend = (force_palette_mode == PALETTE_MODE__FORCE_DISCRETE) ? NOBLEND : LINEARBLEND;

  //   // Handle FORCE_DISCRETE mode — exact color steps only
  //   if (force_palette_mode == PALETTE_MODE__FORCE_DISCRETE) {
  //     if (palette_index__format == PALETTE_INDEX_IS_INDEX_IN_PALETTE) {
  //       // Classic "encoded discrete"
  //       uint8_t discrete_index = desired_index % 16;
  //       pixel_position_adjust = pSEGMENT.palette->CRGB16Palette16_Palette.encoded_index[discrete_index];
  //     } else {
  //       // Treat as exact palette index, map 0–15 → 0–240
  //       // Why 240 instead of 255?
  //       // Because:
  //       //   The FastLED CRGBPalette16 spans 0 to 255, but it's divided into 16 color slots.
  //       //   Each slot occupies 16 positions (256 / 16 = 16).
  //       //   The last index 15 should map to 240 to land on the start of the last color band.
  //       //   If you go to 255, you hit the wraparound (blending back to index 0).
  //       pixel_position_adjust = (desired_index * 240) / 15;
  //     }
  //   }
  //   // Handle full gradient mode
  //   else{
  //     if (palette_index__format) {
  //       pixel_position_adjust = (tkr_anim->_virtualSegmentLength == 1) ? 0 : (desired_index * 255) / (tkr_anim->_virtualSegmentLength - 1);
  //     }

  //     if (rescale_index_wrap_for_hardedge) {
  //       pixel_position_adjust = scale8(pixel_position_adjust, 240);  // Avoid wraparound
  //     }
  //   }

  //   CRGB fastled_col = ColorFromPaletteU32(pSEGMENT.palette->CRGB16Palette16_Palette.data, pixel_position_adjust, 255, blend);
  //   colour32 = RGBW32(fastled_col.r, fastled_col.g, fastled_col.b, 0);
  //   #ifdef ENABLE_FEATURE_PALETTE__RGBWW_COLOURS
  //   colour32_white_cold = 0; // No white in CRGB16Palette16_Palette
  //   #endif

  // } 
  if(
    ((id >= PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID)                      && (id < PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID)) ||
    ((id >= PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID)                     && (id < PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID))  ||
    ((id >= PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID)     && (id < PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__LENGTH__ID)) ||
    ((id >= PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__RANDOMISE_COLOURS_01__ID) && (id < PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__LENGTH__ID))
  ){
    
    uint16_t pixel_position_adjust = desired_index;

    TBlendType blend = (force_palette_mode == PALETTE_MODE__FORCE_DISCRETE) ? NOBLEND : LINEARBLEND;

    // Handle FORCE_DISCRETE mode — exact color steps only
    if (force_palette_mode == PALETTE_MODE__FORCE_DISCRETE) {
      if (palette_index__format == PALETTE_INDEX__IS_EXACT_COLOUR) {
        // Classic "encoded discrete"
        uint8_t discrete_index = desired_index % 16;
        pixel_position_adjust = pSEGMENT.palette->CRGB16Palette16_Palette.encoded_index[discrete_index];
      } else {
        // Treat as exact palette index, map 0–15 → 0–240
        // Why 240 instead of 255?
        // Because:
        //   The FastLED CRGBPalette16 spans 0 to 255, but it's divided into 16 color slots.
        //   Each slot occupies 16 positions (256 / 16 = 16).
        //   The last index 15 should map to 240 to land on the start of the last color band.
        //   If you go to 255, you hit the wraparound (blending back to index 0).
        pixel_position_adjust = (desired_index * 240) / 15;
      }
    }
    // Handle full gradient mode
    else{
      if (palette_index__format) {
        pixel_position_adjust = (tkr_anim->_virtualSegmentLength == 1) ? 0 : (desired_index * 255) / (tkr_anim->_virtualSegmentLength - 1);
      }

      if (rescale_index_wrap_for_hardedge) {
        pixel_position_adjust = scale8(pixel_position_adjust, 240);  // Avoid wraparound
        // if(pixel_position_adjust > 235)
        // Serial.printf("rescale_index_wrap_for_hardedge %d \n\r", pixel_position_adjust);
      }
    }

    CRGB fastled_col = ColorFromPaletteU32(pSEGMENT.palette->CRGB16Palette16_Palette.data, pixel_position_adjust, 255, blend);
    colour32 = RGBW32(fastled_col.r, fastled_col.g, fastled_col.b, 0);
    #ifdef ENABLE_FEATURE_PALETTE__RGBWW_COLOURS
    colour32_white_cold = 0; // No white in CRGB16Palette16_Palette
    #endif

  } 


  /**************************************************************
   * 
   * Single Colour Palettes
   * * Byte RGB stored array of colours
   * 
  ***************************************************************/
  else
  if(
    (id >= PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID) && (id < PALETTELIST_STATIC_SINGLE_COLOUR__LENGTH__ID)
  ){    
    uint8_t adjusted_id  = id - PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID;
    const uint8_t* data  = PM_STATIC_SINGLE_COLOURS__DATA;
    uint8_t adjust_buf_i =  adjusted_id*3;
    colour32 = RGBW32(data[adjust_buf_i], data[adjust_buf_i+1], data[adjust_buf_i+2], 0);
    #ifdef ENABLE_FEATURE_PALETTE__RGBWW_COLOURS
    colour32_white_cold = 0; // No white in PALETTELIST_STATIC_SINGLE_COLOUR
    #endif
  }  
  /**************************************************************
   * 
   * Segment Colours
   * * Stored as RGBWW in segement data (currently 5)
   * 
  ***************************************************************/
  else
  if(
    (id >= PALETTELIST_SEGMENT__SEGMENT_COLOUR_01__ID) && (id < PALETTELIST_SEGMENT__SEGMENT_COLOUR_LENGTH__ID)
  ){  
    uint8_t adjusted_id = id - PALETTELIST_SEGMENT__SEGMENT_COLOUR_01__ID;
    colourRGBWW = pSEGMENT.segcol[adjusted_id].colour;
    colour32 = RGBW32(colourRGBWW.R, colourRGBWW.G, colourRGBWW.B, colourRGBWW.WW);
    #ifdef ENABLE_FEATURE_PALETTE__RGBWW_COLOURS
    colour32_white_cold = 0; // No white in CRGB16Palette16_Palette
    #endif
  }
  /**************************************************************
   * 
   * Static Palettes
   * * Mixture of RGB and RGBWW encoded palettes, with and without index encoding for discrete and gradient modes
   * 
  ***************************************************************/
  else if (id >= PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID && 
           id < PALETTELIST_STATIC_LENGTH__ID) {


    uint16_t palette_adjusted_id = id - PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID; 
    data = &static_palettes[palette_adjusted_id].data[0];

    #ifdef ENABLE_FEATURE_PALETTE__RGBWW_COLOURS
    colourRGBWW = SubGet_Encoded_Palette_Colour_RGBWW( // should make a U32 version for improved performance when only in RGB mode
      data,
      desired_index,
      pSEGMENT.palette->encoded_colour_width,
      pSEGMENT.palette->colours_in_palette,
      static_palettes[palette_adjusted_id].encoding,
      encoded_index,
      palette_index__format,
      rescale_index_wrap_for_hardedge,
      force_palette_mode,
      false
    );
    colour32 = RGBW32(colourRGBWW.R, colourRGBWW.G, colourRGBWW.B, colourRGBWW.WW); colour32_white_cold = colourRGBWW.CW;
    #else // Fast RGBW support only
    colour32 = SubGet_Encoded_Palette_Colour_U32( // should make a U32 version for improved performance when only in RGB mode
      data,
      desired_index,
      pSEGMENT.palette->encoded_colour_width,
      pSEGMENT.palette->colours_in_palette,
      static_palettes[palette_adjusted_id].encoding,
      encoded_index,
      palette_index__format,
      rescale_index_wrap_for_hardedge,
      force_palette_mode, // DEPRECIATE: was called override_default_encoding
      false               // DEPRECIATE: was called flag_crgb_exact_colour = false (true: "CRGB exact colour", false: "U32 colour")
    );
    #endif
  }
  /**************************************************************
   * 
   * Custom Palettes
   * * User defined palettes, stored in flash, and can be modified by the user
   * 
  ***************************************************************/
  else if (id >= PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED && 
           id < GetPaletteListLength()) {

    uint16_t palette_adjusted_id = id - PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED;
    data = &custom_palettes[palette_adjusted_id].data[0];

    // Serial.printf("pal %d %d \n\r", id, palette_adjusted_id);

    #ifdef ENABLE_FEATURE_PALETTE__RGBWW_COLOURS
    colourRGBWW = SubGet_Encoded_Palette_Colour_RGBWW(
      data,
      desired_index,
      pSEGMENT.palette->encoded_colour_width,
      pSEGMENT.palette->colours_in_palette,
      custom_palettes[palette_adjusted_id].encoding,
      encoded_index,
      palette_index__format,
      rescale_index_wrap_for_hardedge,
      force_palette_mode,
      false
    );
    colour32 = RGBW32(colourRGBWW.R, colourRGBWW.G, colourRGBWW.B, colourRGBWW.WW); colour32_white_cold = colourRGBWW.CW;
    #else
    colour32 = SubGet_Encoded_Palette_Colour_U32(
      data,
      desired_index,
      pSEGMENT.palette->encoded_colour_width,
      pSEGMENT.palette->colours_in_palette,
      custom_palettes[palette_adjusted_id].encoding,
      encoded_index,
      palette_index__format,
      rescale_index_wrap_for_hardedge,
      force_palette_mode,
      false
    );
    #endif
  }
  /**************************************************************
   * 
   * Dynamic Palettes
   * * Dynamic, or Live palettes, that are generated at runtime (e.g. solar elevation, time of day) .// FIX: the CRGBPalette random needs renamed or moved into this group to make it clear its "Live"
   * 
  ***************************************************************/
  else 
  if(
    ((id >= PALETTELIST_DYNAMIC__COLOUR__ID_START) && (id < PALETTELIST_DYNAMIC__LENGTH__ID))
  ){  
    uint8_t palette_adjusted_id_rel0 = id - PALETTELIST_DYNAMIC__COLOUR__ID_START;
    data = &dynamic_palettes[palette_adjusted_id_rel0].data[0];;

    switch(id) 
    {
      case PALETTELIST_DYNAMIC__SOLAR_ELEVATION__WHITE_COLOUR_TEMPERATURE_01__ID: {

        #if defined(USE_MODULE_SENSORS_SUN_TRACKING) || defined(USE_MODULE_SENSORS_SUN_TRACKING__BASIC_ESTIMATE)
          float elevation = tkr_solar->Get_Elevation();
          float el_min = (ELEVATION_NIGHT_THRESHOLD != 0) ? ELEVATION_NIGHT_THRESHOLD : tkr_solar->Get_Elevation_Min();
          float el_max = (ELEVATION_DAY_THRESHOLD != 0)   ? ELEVATION_DAY_THRESHOLD   : tkr_solar->Get_Elevation_Max();
        #else
          float elevation = 0;
          float el_min = -10;
          float el_max = 10;
        #endif

        mAnimatorLight::SegmentColour colour_out = 0;
        float eval_elevation;

        if (flag_request_is_for_full_visual_output) {
          uint16_t pixel_length = tkr_anim->_virtualSegmentLength;
          eval_elevation = mSupport::mapfloat(desired_index, 0.0f, 16.0f, el_min, el_max);
          ALOG_INF(PSTR("Full Visual Output: Pixel Position: %d, Mapped Elevation: %d"), desired_index, (int)eval_elevation);
        } else {
          eval_elevation = elevation;
        }

        if (eval_elevation <= el_min) {
          colour_out.setCCT_Kelvin(CCT_MAX_DEFAULT);         // Warm white
          colour_out.setRGB(0xFF, 0x52, 0x18);
        } else if (eval_elevation >= el_max) {
          colour_out.setCCT_Kelvin(CCT_MIN_DEFAULT);         // Cold white
          colour_out.setRGB(255, 255, 255);
        } else {
          float progress = mSupport::mapfloat(eval_elevation, el_min, el_max, 0.0f, 1.0f);

          mAnimatorLight::SegmentColour warm = 0;
          warm.setCCT_Kelvin(CCT_MAX_DEFAULT);
          warm.setRGB(0xFF, 0x52, 0x18);

          mAnimatorLight::SegmentColour cold = 0;
          cold.setCCT_Kelvin(CCT_MIN_DEFAULT);
          cold.setRGB(255, 255, 255);

          colour_out.colour = RgbwwColor::LinearBlend(warm.colour, cold.colour, progress);
        }

        #ifdef ENABLE_DEBUGFEATURE_LIGHT__PALETTE_RELOAD_LOGGING
          // Serial.println(eval_elevation);
          // colour_out.debug_print("colour_out");
        #endif

        colourRGBWW = colour_out.colour;
        colour32 = RGBW32(colourRGBWW.R, colourRGBWW.G, colourRGBWW.B, colourRGBWW.WW);
        #ifdef ENABLE_FEATURE_PALETTE__RGBWW_COLOURS
        colour32_white_cold = 0; // No white in CRGB16Palette16_Palette
        #endif

      break;
      }
      case PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SEGMENT_COLOUR_BLEND_DAYTIME_01__ID: {

        #if defined(USE_MODULE_SENSORS_SUN_TRACKING) || defined(USE_MODULE_SENSORS_SUN_TRACKING__BASIC_ESTIMATE)
          float elevation = tkr_solar->Get_Elevation();
          float el_min = 0.0f;
          float el_max = (ELEVATION_DAY_THRESHOLD != 0) ? ELEVATION_DAY_THRESHOLD : tkr_solar->Get_Elevation_Max();
        #else
          float elevation = 0;
          float el_min = 0;
          float el_max = 10;
        #endif
      
        float eval_elevation;
        if (flag_request_is_for_full_visual_output) {
          eval_elevation = mSupport::mapfloat(desired_index, 0.0f, 16.0f, el_min, el_max);
        } else {
          eval_elevation = elevation;
        }
    
        eval_elevation = constrain(eval_elevation, el_min, el_max);
        float progress = mSupport::mapfloat(eval_elevation, el_min, el_max, 0.0f, 1.0f);
    
        RgbwwColor colour1 = pSEGMENT.segcol[0].colour;
        RgbwwColor colour2 = pSEGMENT.segcol[1].colour;
        colourRGBWW = RgbwwColor::LinearBlend(colour1, colour2, progress);
        colour32 = RGBW32(colourRGBWW.R, colourRGBWW.G, colourRGBWW.B, colourRGBWW.WW);
        #ifdef ENABLE_FEATURE_PALETTE__RGBWW_COLOURS
        colour32_white_cold = 0; // No white in CRGB16Palette16_Palette
        #endif
          
      break;
      }
      case PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SEGMENT_COLOUR_BLEND_DAWNDUSKTIME_01__ID: {

        #if defined(USE_MODULE_SENSORS_SUN_TRACKING) || defined(USE_MODULE_SENSORS_SUN_TRACKING__BASIC_ESTIMATE)
          float elevation = tkr_solar->Get_Elevation();
          float el_min = (ELEVATION_NIGHT_THRESHOLD != 0) ? ELEVATION_NIGHT_THRESHOLD : tkr_solar->Get_Elevation_Min();
          float el_max = (ELEVATION_DAY_THRESHOLD != 0) ? ELEVATION_DAY_THRESHOLD : tkr_solar->Get_Elevation_Max();
        #else
          float elevation = 0;
          float el_min = -10;
          float el_max = 10;
        #endif
      
        float eval_elevation;
        if (flag_request_is_for_full_visual_output) {
          eval_elevation = mSupport::mapfloat(desired_index, 0.0f, 16.0f, el_min, el_max);
        } else {
          eval_elevation = elevation;
        }
    
        eval_elevation = constrain(eval_elevation, el_min, el_max);
        float progress = mSupport::mapfloat(eval_elevation, el_min, el_max, 0.0f, 1.0f);
    
        RgbwwColor colour1 = pSEGMENT.segcol[0].colour;
        RgbwwColor colour2 = pSEGMENT.segcol[1].colour;
        colourRGBWW = RgbwwColor::LinearBlend(colour1, colour2, progress);
        colour32 = RGBW32(colourRGBWW.R, colourRGBWW.G, colourRGBWW.B, colourRGBWW.WW);
        #ifdef ENABLE_FEATURE_PALETTE__RGBWW_COLOURS
        colour32_white_cold = 0; // No white in CRGB16Palette16_Palette
        #endif
          
      break;
      }
      case PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SEGMENT_COLOUR_BLEND_NIGHTTIME_01__ID: {

        #if defined(USE_MODULE_SENSORS_SUN_TRACKING) || defined(USE_MODULE_SENSORS_SUN_TRACKING__BASIC_ESTIMATE)
          float elevation = tkr_solar->Get_Elevation();
          float el_max = (ELEVATION_NIGHT_THRESHOLD != 0) ? ELEVATION_NIGHT_THRESHOLD : -10.0f;
          float el_min = tkr_solar->Get_Elevation_Min();
        #else
          float elevation = 0;
          float el_min = -30;
          float el_max = -10;
        #endif
      
        float eval_elevation;
        if (flag_request_is_for_full_visual_output) {
          eval_elevation = mSupport::mapfloat(desired_index, 0.0f, 16.0f, el_min, el_max);
        } else {
          eval_elevation = elevation;
        }
    
        eval_elevation = constrain(eval_elevation, el_min, el_max);
        float progress = mSupport::mapfloat(eval_elevation, el_min, el_max, 0.0f, 1.0f);
    
        RgbwwColor colour1 = pSEGMENT.segcol[0].colour;
        RgbwwColor colour2 = pSEGMENT.segcol[1].colour;
        colourRGBWW = RgbwwColor::LinearBlend(colour1, colour2, progress);
        colour32 = RGBW32(colourRGBWW.R, colourRGBWW.G, colourRGBWW.B, colourRGBWW.WW);
        #ifdef ENABLE_FEATURE_PALETTE__RGBWW_COLOURS
        colour32_white_cold = 0; // No white in CRGB16Palette16_Palette
        #endif
    
      break;
      }    
      case PALETTELIST_DYNAMIC__TIMEREACTIVE__SEGMENT_COLOUR__MINUTE_BLEND__ID:
      {
        /***
         * Sawtooth style blending, from 0->1->0 of segment colours. Stops hard transition of time rollover
         */
        float progress;
        if (tkr_time->RtcTime.second < 30) {
          progress = mSupport::mapfloat(tkr_time->RtcTime.second, 0, 29, 0.0f, 1.0f);
        } else {
          progress = mSupport::mapfloat(tkr_time->RtcTime.second, 30, 59, 1.0f, 0.0f);
        }       
        #ifdef ENABLE_DEBUGFEATURE_LIGHT__PALETTE_RELOAD_LOGGING
        Serial.println(progress);
        #endif        
        RgbwwColor colour1 = pSEGMENT.segcol[0].colour;
        RgbwwColor colour2 = pSEGMENT.segcol[1].colour;
        colourRGBWW = RgbwwColor::LinearBlend(colour1, colour2, progress);   
        colour32 = RGBW32(colourRGBWW.R, colourRGBWW.G, colourRGBWW.B, colourRGBWW.WW);
        #ifdef ENABLE_FEATURE_PALETTE__RGBWW_COLOURS
        colour32_white_cold = 0; // No white in CRGB16Palette16_Palette
        #endif
      }
      break;
      case PALETTELIST_DYNAMIC__TIMEREACTIVE__SEGMENT_COLOUR__HOUR_BLEND__ID:
      {
        /***
         * Sawtooth style blending, from 0->1->0 of segment colours. Stops hard transition of time rollover
         */
        float progress;
        if (tkr_time->RtcTime.hour < 30) {
          progress = mSupport::mapfloat(tkr_time->RtcTime.hour, 0, 29, 0.0f, 1.0f);
        } else {
          progress = mSupport::mapfloat(tkr_time->RtcTime.hour, 30, 59, 1.0f, 0.0f);
        }
        #ifdef ENABLE_DEBUGFEATURE_LIGHT__PALETTE_RELOAD_LOGGING
        Serial.println(progress);
        #endif        
        RgbwwColor colour1 = pSEGMENT.segcol[0].colour;
        RgbwwColor colour2 = pSEGMENT.segcol[1].colour;
        colourRGBWW = RgbwwColor::LinearBlend(colour1, colour2, progress);    
        colour32 = RGBW32(colourRGBWW.R, colourRGBWW.G, colourRGBWW.B, colourRGBWW.WW);
        #ifdef ENABLE_FEATURE_PALETTE__RGBWW_COLOURS
        colour32_white_cold = 0; // No white in CRGB16Palette16_Palette
        #endif
      }
      break;
      case PALETTELIST_DYNAMIC__SOLAR_ELEVATION__GRADIENT_COLOUR_OF_SKY__ID: {
        #if defined(USE_MODULE_SENSORS_SUN_TRACKING) || defined(USE_MODULE_SENSORS_SUN_TRACKING__BASIC_ESTIMATE)
          float elevation = tkr_solar->Get_Elevation();
          float el_min = tkr_solar->Get_Elevation_Min();
          float el_max = tkr_solar->Get_Elevation_Max();
        #else
          float elevation = 0.0f;
          float el_min = -45.0f;
          float el_max = 45.0f;
        #endif
      
        uint16_t pixel_length = tkr_anim->_virtualSegmentLength;
        uint16_t rescaled_palette_index;
      
        if (flag_request_is_for_full_visual_output) {
          rescaled_palette_index = desired_index;
        } else {
          float zoom_ratio = pSEGMENT.custom1 / 255.0f;
          zoom_ratio = constrain(zoom_ratio, 0.01f, 1.0f);  // Prevent zero or too narrow
      
          float zoom_range = (el_max - el_min) * zoom_ratio;
          float el_start = elevation - (zoom_range / 2.0f);
          float el_end   = elevation + (zoom_range / 2.0f);
      
          el_start = constrain(el_start, el_min, el_max);
          el_end   = constrain(el_end, el_min, el_max);

          uint16_t pixel_position_adjust = (pixel_length > 1) ? (desired_index * 255) / (pixel_length - 1) : 0;
          uint16_t palette_start = (uint16_t)mSupport::mapfloat(el_start, el_min, el_max, 0.0f, 255.0f);
          uint16_t palette_end   = (uint16_t)mSupport::mapfloat(el_end,   el_min, el_max, 0.0f, 255.0f);
      
          if (palette_start >= palette_end)
            palette_start = (palette_end > 0) ? palette_end - 1 : 0;
      
          rescaled_palette_index = (uint16_t)mSupport::mapfloat(
            pixel_position_adjust, 0.0f, 255.0f, palette_start, palette_end);
          rescaled_palette_index = constrain(rescaled_palette_index, 0, 255);
        }
      
        bool flag_force_gradient = false;// missing as it was in 
      
        #ifdef ENABLE_FEATURE_PALETTE__RGBWW_COLOURS    
        colourRGBWW = SubGet_Encoded_Palette_Colour_RGBWW(
          data,
          rescaled_palette_index,
          pSEGMENT.palette->encoded_colour_width,
          pSEGMENT.palette->colours_in_palette,
          dynamic_palettes[palette_adjusted_id_rel0].encoding,
          encoded_index,
          false,
          rescale_index_wrap_for_hardedge,
          override_default_encoding,
          flag_force_gradient
        );
        colour32 = RGBW32(colourRGBWW.R, colourRGBWW.G, colourRGBWW.B, colourRGBWW.WW); colour32_white_cold = colourRGBWW.CW;
        #else        
        colourRGBWW = SubGet_Encoded_Palette_Colour_U32(
          data,
          rescaled_palette_index,
          pSEGMENT.palette->encoded_colour_width,
          pSEGMENT.palette->colours_in_palette,
          dynamic_palettes[palette_adjusted_id_rel0].encoding,
          encoded_index,
          false,
          rescale_index_wrap_for_hardedge,
          force_palette_mode,
          flag_force_gradient
        );
        #endif
      }
      break;
      case PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SOLID_COLOUR_OF_SKY__ID: {
        #if defined(USE_MODULE_SENSORS_SUN_TRACKING) || defined(USE_MODULE_SENSORS_SUN_TRACKING__BASIC_ESTIMATE)
          float elevation = tkr_solar->Get_Elevation();
          float el_min = tkr_solar->Get_Elevation_Min();
          float el_max = tkr_solar->Get_Elevation_Max();
        #else
          float elevation = 0.0f;
          float el_min = -45.0f;
          float el_max = 45.0f;
        #endif
      
        float eval_elevation = constrain(elevation, el_min, el_max);
        uint16_t palette_index = (uint16_t)mSupport::mapfloat(eval_elevation, el_min, el_max, 0.0f, 255.0f);
        palette_index = constrain(palette_index, 0, 255);
      
        bool flag_force_gradient = false;// missing as it was in 
      
        #ifdef ENABLE_FEATURE_PALETTE__RGBWW_COLOURS    
        colourRGBWW = SubGet_Encoded_Palette_Colour_RGBWW(
          data,
          palette_index,
          pSEGMENT.palette->encoded_colour_width,
          pSEGMENT.palette->colours_in_palette,
          dynamic_palettes[palette_adjusted_id_rel0].encoding,
          encoded_index,
          false,
          rescale_index_wrap_for_hardedge,
          override_default_encoding,
          flag_force_gradient
        );
        colour32 = RGBW32(colourRGBWW.R, colourRGBWW.G, colourRGBWW.B, colourRGBWW.WW); colour32_white_cold = colourRGBWW.CW;
        #else // Fast RGBW support only        
        colour32 = SubGet_Encoded_Palette_Colour_U32(
          data,
          palette_index,
          pSEGMENT.palette->encoded_colour_width,
          pSEGMENT.palette->colours_in_palette,
          dynamic_palettes[palette_adjusted_id_rel0].encoding,
          encoded_index,
          false,
          rescale_index_wrap_for_hardedge,
          force_palette_mode,
          flag_force_gradient
        );
        #endif

      }
      break;
      
      
      default:
        ALOG_ERR(PSTR("Bad Palette ID %d"), id);
        return 0; // Default to black if palette is not found
    }
  }
  else {
    ALOG_INF(PSTR("Missing %d"), id);
  }
  
  return colour32;

}


// 1:1 replacement of fastled function optimized for ESP, slightly faster, more accurate and uses less flash (~ -200bytes)
uint32_t mPalette::ColorFromPaletteU32(const CRGBPalette16& pal, unsigned index, uint8_t brightness, TBlendType blendType)
{
  if (blendType == LINEARBLEND_NOWRAP) {
    index = (index*240) >> 8; // Blend range is affected by lo4 blend of values, remap to avoid wrapping
  }
  unsigned hi4 = byte(index) >> 4;
  const CRGB* entry = (CRGB*)((uint8_t*)(&(pal[0])) + (hi4 * sizeof(CRGB)));
  unsigned red1   = entry->r;
  unsigned green1 = entry->g;
  unsigned blue1  = entry->b;
  if (blendType != NOBLEND) {
    if (hi4 == 15) entry = &(pal[0]);
    else ++entry;
    unsigned f2 = ((index & 0x0F) << 4) + 1; // +1 so we scale by 256 as a max value, then result can just be shifted by 8
    unsigned f1 = (257 - f2); // f2 is 1 minimum, so this is 256 max
    red1   = (red1 * f1 + (unsigned)entry->r * f2) >> 8;
    green1 = (green1 * f1 + (unsigned)entry->g * f2) >> 8;
    blue1  = (blue1 * f1 + (unsigned)entry->b * f2) >> 8;
  }
  if (brightness < 255) { // note: zero checking could be done to return black but that is hardly ever used so it is omitted
    uint32_t scale = brightness + 1; // adjust for rounding (bitshift)
    red1   = (red1 * scale) >> 8;
    green1 = (green1 * scale) >> 8;
    blue1  = (blue1 * scale) >> 8;
  }
  return RGBW32(red1,green1,blue1,0);
}

#ifdef ENABLE_FEATURE_PALETTE__RGBWW_COLOURS

IRAM_ATTR [[gnu::hot]] RgbwwColor      mPalette::GetColourFromPreloadedPaletteBuffer_RGBWW
(
  uint16_t id,
  // Pass preloaded palette data buffer. If nullptr, and "id" does not match any preloaded palette, then it will force a reload of the palette data.
  uint8_t* data,
  // In discrete mode, this index will automatically modulo and repeat the palette over infinite length (MAXU16). In Gradient mode, must be scaled in 0 to 255 range.
  uint16_t desired_index,
  // If the palette is encoded, then this returns encoded value at [desired_index] point. NOTE: Only in discrete mode.
  uint8_t* encoded_index,
  // Providing the index in range 0 to 255, this enabled will internally rescale the index to the segment length, so that the index is always in range 0 to segment_length-1
  uint8_t  palette_index__format,
  // CRGBPalette defaults gradient (index 240 to 255) wraps to blend with colour as index 0. This rescales to limit to 240, hence, removes wrap around blending.
  uint8_t  rescale_index_wrap_for_hardedge,
  // 0 = default, 1 = "Forced Discrete", 2 = "Forced Gradient"
  uint8_t  override_default_encoding, // flag_crgb_exact_colour = 0, // true: "CRGB exact colour", false: "U32 colour"
  // Requesting preview: Live palettes must respond with preview for UI
  bool flag_request_is_for_full_visual_output  
){

  /***
   * Keep the function signature the same, but internally call GetColourFromPreloadedPaletteBuffer_U32
   * Use external variable to hold additional white component generated in U32 version
   * This allows _U32 to be centralised generation of colour (i.e. makes it easier to maintain, particularly for dynamic palettes)
   */
  uint32_t rgbw = GetColourFromPreloadedPaletteBuffer_U32(
                                                            id,
                                                            data,
                                                            desired_index,
                                                            encoded_index,
                                                            palette_index__format,
                                                            rescale_index_wrap_for_hardedge,
                                                            override_default_encoding,
                                                            flag_request_is_for_full_visual_output
                                                          );
                                                        
  return RgbwwColor(
    (rgbw >> 16) & 0xFF, // Red
    (rgbw >>  8) & 0xFF, // Green
    (rgbw >>  0) & 0xFF, // Blue
    (rgbw >> 24) & 0xFF, // White component
    colour32_white_cold  // To get around U32 performance with more than 32bit wide, the 5th component is updated inside _U32 function then appended here.
  );

}

#endif

/*********************************************************************************************************************************************************************************
 *********************************************************************************************************************************************************************************
 * SECTION: Main requests to get/set name/id of palettes
 *********************************************************************************************************************************************************************************
 *********************************************************************************************************************************************************************************/


int16_t mPalette::Get_Static_PaletteIDbyName(const char* c)
{

  char buffer[100] = {0};
  int16_t id = -1;

  for(
    uint8_t ii=0;
            ii<(PALETTELIST_SEGMENT__SEGMENT_COLOUR_LENGTH__ID-PALETTELIST_SEGMENT__SEGMENT_COLOUR_01__ID);
            ii++
  ){    
    ALOG_DBM( PSTR("s> %d %s \"%S\""), ii, c, PM_SEGMENT__RGBCCT_SOLID_COLOUR__NAMES_CTR ); 
    if((id=mSupport::GetCommandID16_P(c, PM_SEGMENT__RGBCCT_SOLID_COLOUR__NAMES_CTR))>=0)
    {
      ALOG_INF( PSTR("MATCH \"%s\" %d %d"), c, ii, id ); 
      return id+PALETTELIST_SEGMENT__SEGMENT_COLOUR_01__ID;            
    }
  }

  /**************************************************************
   * 
   * PALETTELIST_DYNAMIC_CRGBPALETTE16_USER__LENGTH__ID
   * 
  ***************************************************************/
  for(
    uint8_t ii=0;
            ii<(PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__LENGTH__ID-PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID);
            ii++
  ){    
    ALOG_DBM( PSTR("s> %d %s \"%S\""), ii, c, PM_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES_NAMES_CTR ); 
    if((id=mSupport::GetCommandID16_P(c, PM_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES_NAMES_CTR))>=0)
    {
      ALOG_INF( PSTR("MATCH \"%s\" %d %d"), c, ii, id ); 
      return id+PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID;            
    }
  }

  /**************************************************************
   * 
   * PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID
   * 
  ***************************************************************/
  for(
    uint8_t ii=0;
            ii<(PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID-PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID);
            ii++
  ){    
    ALOG_DBM( PSTR("s> %d %s \"%S\""), ii, c, PM_STATIC_CRGBPALETTE16_NAMES_CTR ); 
    if((id=mSupport::GetCommandID16_P(c, PM_STATIC_CRGBPALETTE16_NAMES_CTR))>=0)
    {
      ALOG_INF( PSTR("MATCH \"%s\" %d %d"), c, ii, id ); 
      return id+PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID;            
    }
  }

  /**************************************************************
   * 
   * PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID
   * 
  ***************************************************************/
  for(
    uint8_t ii=0;
            ii<(PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID-PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID);
            ii++
  ){    
    ALOG_DBM( PSTR("s> %d %s \"%S\""), ii, c, PM_STATIC_CRGBPALETTE16_GRADIENT_NAMES_CTR ); 
    if((id=mSupport::GetCommandID16_P(c, PM_STATIC_CRGBPALETTE16_GRADIENT_NAMES_CTR))>=0)
    {
      ALOG_INF( PSTR("MATCH \"%s\" %d %d"), c, ii, id ); 
      return id+PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID;            
    }
  }

  /**************************************************************
   * 
   * PALETTELIST_STATIC_SINGLE_COLOUR__LENGTH__ID
   * 
  ***************************************************************/
  for(
    uint8_t ii=0;
            ii<(PALETTELIST_STATIC_SINGLE_COLOUR__LENGTH__ID-PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID);
            ii++
  ){    
    ALOG_DBM( PSTR("s> %d %s \"%S\""), ii, c, PM_STATIC_SINGLE_COLOUR_NAMES_CTR ); 
    if((id=mSupport::GetCommandID16_P(c, PM_STATIC_SINGLE_COLOUR_NAMES_CTR))>=0)
    {
      ALOG_INF( PSTR("MATCH \"%s\" %d %d"), c, ii, id ); 
      return id+PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID;            
    }
  }

  /**************************************************************
   * 
   * PALETTELIST_STATIC__IDS
   * 
  ***************************************************************/
  for(
    uint8_t ii=0;
            ii<(PALETTELIST_STATIC_LENGTH__ID - PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID);
            ii++
  ){
    // ALOG_INF( PSTR("s> %d %s \"%S\"\n\r\n\r"), ii, c, PM_STATIC__PALETTES_NAMES_CTR ); 
    if((id=mSupport::GetCommandID16_MultipleSubMatches_P(c, PM_STATIC__PALETTES_NAMES_CTR))>=0)
    {
      ALOG_INF( PSTR("MATCH \"%s\" %d %d"), c, ii, id ); 
      return id+PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID;            
    }
  }


  /**************************************************************
   * 
   * PM_DYNAMIC__CRGBPALETTE16_PALETTES_NAMES_CTR
   * 
  ***************************************************************/
 for(
    uint8_t ii=0;
            ii<(PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__LENGTH__ID-PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__RANDOMISE_COLOURS_01__ID);
            ii++
  ){    
    ALOG_DBM( PSTR("s> %d %s \"%S\""), ii, c, PM_DYNAMIC__CRGBPALETTE16_PALETTES_NAMES_CTR ); 
    if((id=mSupport::GetCommandID16_P(c, PM_DYNAMIC__CRGBPALETTE16_PALETTES_NAMES_CTR))>=0)
    {
      ALOG_INF( PSTR("MATCH \"%s\" %d %d"), c, ii, id ); 
      return id+PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__RANDOMISE_COLOURS_01__ID;            
    }
  }

  /**************************************************************
   * 
   * PALETTELIST_DYNAMIC__COLOUR__IDS
   * 
  ***************************************************************/
  for(
    uint8_t ii=0;
            ii<(PALETTELIST_DYNAMIC__LENGTH__ID-PALETTELIST_DYNAMIC__COLOUR__ID_START);
            ii++
  ){  
    ALOG_DBM(PSTR("s> %d %s \"%S\""), ii, c, PM_DYNAMIC_PALETTES_NAMES_CTR); 

    // Check if "c" starts with "Live "
    const char* adjusted_c = c;
    const char* live_prefix = "Live ";
    size_t live_prefix_len = strlen(live_prefix);

    if (strncmp(c, live_prefix, live_prefix_len) == 0) {
        adjusted_c = c + live_prefix_len;  // Skip the "Live " prefix
    }

    // Call GetCommandID16_P with the adjusted string
    if ((id = mSupport::GetCommandID16_P(adjusted_c, PM_DYNAMIC_PALETTES_NAMES_CTR)) >= 0) {
        ALOG_INF(PSTR("MATCH \"%s\" %d %d"), adjusted_c, ii, id); 
        return id + PALETTELIST_DYNAMIC__COLOUR__ID_START;            
    }
  }

  /**************************************************************
   * 
   * PALETTELIST_VARIABLE_GENERIC__IDS - undefined user names, default naming
   * User names should have been captured in mAnimatorLight::GetPaletteNameByID before calling this function.
   * This safe guard makes sure the default exists should this be called directly.
   * 
  ***************************************************************/
  for(
    uint8_t ii=0;
            ii<(MAX_USER_DEFINED_ENCODED_PALETTES);
            ii++
  ){

    memset(buffer,0,sizeof(buffer));
    sprintf_P(buffer, PSTR(D_DEFAULT_MODIFIABLE_PALETTE_NAMES__USER_CREATED__NAME_CTR), ii + 1);
    
    ALOG_DBM( PSTR("s> \"%s\""), buffer ); 
    // Default names
    if(strcmp(c,buffer)==0){
      return ii+PALETTELIST_LENGTH_OF_STATIC_IDS; // new palettes start at the end of the predefined ones
    }
    
  }

  return -1; // Must be -1 to show name not found
}


const char* mPalette::GetPaletteNameByID(uint8_t palette_id, char* buffer, uint8_t buflen)
{

  /**************************************************************
   * 
   * PALETTELIST_SEGMENTS_STORED_FIXED_CRGBPALETTE16_PALETTES__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= PALETTELIST_SEGMENT__SEGMENT_COLOUR_01__ID) && (palette_id < PALETTELIST_SEGMENT__SEGMENT_COLOUR_LENGTH__ID))
  ){  
    uint16_t adjusted_id = palette_id - PALETTELIST_SEGMENT__SEGMENT_COLOUR_01__ID;
    mSupport::GetTextIndexed_P(buffer, buflen, adjusted_id, PM_SEGMENT__RGBCCT_SOLID_COLOUR__NAMES_CTR);   
  }


  /**************************************************************
   * 
   * PALETTELIST_SEGMENTS_STORED_FIXED_CRGBPALETTE16_PALETTES__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID) && (palette_id < PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__LENGTH__ID))
  ){  
    uint16_t adjusted_id = palette_id - PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID;
    mSupport::GetTextIndexed_P(buffer, buflen, adjusted_id, PM_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES_NAMES_CTR);   
  }

  /**************************************************************
   * 
   * PALETTELIST_STATIC_CRGBPALETTE16__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID) && (palette_id < PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID))
  ){  
    uint16_t palette_id_adj = palette_id - PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID;
    mSupport::GetTextIndexed_P(buffer, buflen, palette_id_adj, PM_STATIC_CRGBPALETTE16_NAMES_CTR);   
    // ALOG_DBG( PSTR("BName id%d|a%d \"%s\""), palette_id,palette_id_adj, buffer );
  }

  /**************************************************************
   * 
   * PALETTELIST_CRGBPALETTE16_GRADIENT___PALETTES__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID)    && (palette_id < PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID))
  ){  
    uint16_t adjusted_id = palette_id - PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID;
    mSupport::GetTextIndexed_P(buffer, buflen, adjusted_id, PM_STATIC_CRGBPALETTE16_GRADIENT_NAMES_CTR);   
    // Serial.printf("BName id%d|a%d \"%s\"\n", palette_id,adjusted_id, buffer );
  }

  /**************************************************************
   * 
   * PALETTELIST_DYNAMIC_
   * 
  ***************************************************************/
 if(
  ((palette_id >= PALETTELIST_DYNAMIC__COLOUR__ID_START) && (palette_id < PALETTELIST_DYNAMIC__LENGTH__ID))
  ){           
    uint16_t adjusted_id = palette_id - PALETTELIST_DYNAMIC__COLOUR__ID_START;
    // Prefixing "Live" to these palettes
    sprintf(buffer, "Live ");
    mSupport::GetTextIndexed_P(&buffer[5], buflen-5, adjusted_id, PM_DYNAMIC_PALETTES_NAMES_CTR);
  }
  /**************************************************************
   * 
   * PALETTELIST_DYNAMIC_
   * 
  ***************************************************************/
  if(
    ((palette_id >= PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__RANDOMISE_COLOURS_01__ID) && (palette_id < PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__LENGTH__ID))
  ){           
    uint16_t adjusted_id = palette_id - PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__RANDOMISE_COLOURS_01__ID;
    // Prefixing "Live" to these palettes
    sprintf(buffer, "Live ");
    mSupport::GetTextIndexed_P(&buffer[5], buflen-5, adjusted_id, PM_DYNAMIC__CRGBPALETTE16_PALETTES_NAMES_CTR);
  }


  /**************************************************************
   * 
   * PALETTELIST_STATIC_CRGBPALETTE16__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID) && (palette_id < PALETTELIST_STATIC_SINGLE_COLOUR__LENGTH__ID))
  ){  
    uint16_t palette_id_adj = palette_id - PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID;
    mSupport::GetTextIndexed_P(buffer, buflen, palette_id_adj, PM_STATIC_SINGLE_COLOUR_NAMES_CTR);   
    // ALOG_DBG( PSTR("BName id%d|a%d \"%s\""), palette_id,palette_id_adj, buffer );
  }

  /**************************************************************
   * 
   * PALETTELIST_STATIC__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID) && (palette_id < PALETTELIST_STATIC_LENGTH__ID))
  ){           
    uint16_t palette_id_adj = palette_id - PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID;
    mSupport::GetTextIndexed_P(buffer, buflen, palette_id_adj, PM_STATIC__PALETTES_NAMES_CTR);   
  }

  /**************************************************************
   * 
   * PALETTELIST_SEGMENT__SEGMENT_COLOUR_LENGTH__ID
   * 
  ***************************************************************/
  if(
    ((palette_id >= PALETTELIST_SEGMENT__SEGMENT_COLOUR_01__ID)    && (palette_id < PALETTELIST_SEGMENT__SEGMENT_COLOUR_LENGTH__ID))
  ){  
    uint16_t adjusted_id = palette_id - PALETTELIST_SEGMENT__SEGMENT_COLOUR_01__ID;
    sprintf_P(buffer, PSTR(D_DEFAULT_DYNAMIC_PALETTE_NAMES__VARIABLE_RGBCCT__NAME_CTR), adjusted_id + 1); // names are 1-10
  }

  /**************************************************************
   * 
   * PALETTELIST_VARIABLE_GENERIC__IDS - undefined user names, default naming
   * 
  ***************************************************************/
  // If stored in RAM, and user editable, then it resides in DeviceNameBuffer and not within mPalette class

  return buffer;

}


bool mPalette::IsPaletteGradient(uint16_t palette_id) {
  PALETTE_ENCODING_DATA encoding = {0};

  // Segment colours (discrete, not gradients)
  if (palette_id >= PALETTELIST_SEGMENT__SEGMENT_COLOUR_01__ID &&
      palette_id < PALETTELIST_SEGMENT__SEGMENT_COLOUR_LENGTH__ID) {
    return false;
  }
  // Static single colours (discrete, not gradients)
  else if (palette_id >= PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID &&
           palette_id < PALETTELIST_STATIC_SINGLE_COLOUR__LENGTH__ID) {
    return false;
  }
  // Static CRGB palettes (for example, rainbow colours)
  else if (palette_id >= PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID &&
           palette_id < PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID) {
    uint16_t index = palette_id - PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID;
    return true;
  }
  // Static CRGB gradient palettes (e.g., sunset gradients)
  else if (palette_id >= PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID &&
           palette_id < PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID) {
    uint16_t index = palette_id - PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID;
    return true;
  }
  // Other static palettes (like colourful defaults)
  else if (palette_id >= PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID &&
           palette_id < PALETTELIST_STATIC_LENGTH__ID) {
    uint16_t index = palette_id - PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID;
    encoding = static_palettes[index].encoding;
  }
  // Dynamic palettes
  else if (palette_id >= PALETTELIST_DYNAMIC__COLOUR__ID_START &&
           palette_id < PALETTELIST_DYNAMIC__LENGTH__ID) {
    uint8_t index = palette_id - PALETTELIST_DYNAMIC__COLOUR__ID_START;
    encoding = dynamic_palettes[index].encoding;
  }
  // Custom palettes
  else if (palette_id >= PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED &&
           palette_id < GetPaletteListLength()) {
    uint16_t index = palette_id - PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED;
    encoding = custom_palettes[index].encoding;
  }
  else {
    // If the palette ID doesn't match any known type, assume it's not a gradient.
    return false;
  }

  // Return true if the encoding indicates a gradient.
  return encoding.index_gradient;
}


/**
 * @brief Get elements in a colour type
 * 
 * @param ptr 
 * @param pixel_width_contrained_limit 
 * @return uint16_t 
 */
uint8_t mPalette::GetEncodedColourWidth( PALETTE_ENCODING_DATA encoding )
{

  uint16_t encoded_colour_width = 0;

  if(encoding.red_enabled){ encoded_colour_width++; }
  if(encoding.green_enabled){ encoded_colour_width++; }
  if(encoding.blue_enabled){ encoded_colour_width++; }
  if(encoding.white_warm_enabled){ encoded_colour_width++; }

  if(encoding.white_cold_enabled){ encoded_colour_width++; }
  if(encoding.encoded_value_byte_width){ encoded_colour_width += encoding.encoded_value_byte_width; }

  // if(encoding.index_exact){ encoded_colour_width++; }
  if(encoding.index_gradient){ encoded_colour_width++; }
  if(encoding.index_is_trigger_value_exact){ encoded_colour_width++; }
  if(encoding.index_is_trigger_value_scaled100){ encoded_colour_width++; }
  
  // if(encoding.encoded_as_hsb_ids){ encoded_colour_width++; }
  if(encoding.encoded_as_crgb_palette_16){ encoded_colour_width++; }
  if(encoding.encoded_as_crgb_palette_256){ encoded_colour_width++; }
  if(encoding.palette_can_be_modified){ encoded_colour_width++; }

  // ALOG_INF(PSTR("encoded_colour_width=%d"),encoded_colour_width);

  return encoded_colour_width;

}

uint8_t mPalette::GetColoursInPalette(uint16_t palette_id)
{


  // ALOG_INF(PSTR("Bflag_crgb_exact_colour=%d"), flag_crgb_exact_colour);
    
  /**************************************************************
   * 
   * "CRGBPALETTE16"
   * 
   * PALETTELIST_STATIC_CRGBPALETTE16__IDS
   * PALETTELIST_CRGBPALETTE16_GRADIENT___PALETTES__IDS
   * PALETTELIST_DYNAMIC_CRGBPALETTE16_PALETTES__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID) && (palette_id < PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID)) ||
    ((palette_id >= PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID)    && (palette_id < PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID))  ||
    ((palette_id >= PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID)    && (palette_id < PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__LENGTH__ID))  ||
    ((palette_id >= PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__RANDOMISE_COLOURS_01__ID)    && (palette_id < PALETTELIST_DYNAMIC__ELASPEDTIME__CRGBPALETTE16__LENGTH__ID))
  ){
    return 16;
  } // END of CRGBPalette's


  /**************************************************************
   * 
   * PALETTELIST_STATIC_SINGLE_COLOURS__IDS
   * 
  ***************************************************************/
  else
  if(
    (palette_id >= PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID) && (palette_id < PALETTELIST_STATIC_SINGLE_COLOUR__LENGTH__ID)
  ){  
    return 1;

  }

  /**************************************************************
   * 
   * PALETTELIST_STATIC__IDS
   * 
  ***************************************************************/
  else
  if(
    ((palette_id >= PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID) && (palette_id < PALETTELIST_STATIC_LENGTH__ID))
  ){  
    uint16_t palette_adjusted_id = palette_id - PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID; // adjust back into correct indexing
    return static_palettes[palette_adjusted_id].number_of_colours;  
  } // end of my palettes

  /**************************************************************
   * 
   * PALETTELIST_STATIC__IDS
   * 
  ***************************************************************/
  else
  if(
    ((palette_id >= PALETTELIST_DYNAMIC__COLOUR__ID_START) && (palette_id < PALETTELIST_DYNAMIC__LENGTH__ID))
  ){  
    
    uint16_t palette_adjusted_id = palette_id - PALETTELIST_DYNAMIC__COLOUR__ID_START; // adjust back into correct indexing
    return dynamic_palettes[palette_adjusted_id].number_of_colours;  

  } // end of my palettes


  /**************************************************************
   * 
   * PALETTELIST_SEGMENT__SEGMENT_COLOUR__IDS
   * 
  ***************************************************************/
  else
  if(
    (palette_id >= PALETTELIST_SEGMENT__SEGMENT_COLOUR_01__ID) && (palette_id < PALETTELIST_SEGMENT__SEGMENT_COLOUR_LENGTH__ID)
  ){  
    return 1;
  }


  /**************************************************************
   * 
   * Custom palettes: custom_palettes 
   * 
  ***************************************************************/
  else
  if(
    ((palette_id >= PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED)  && (palette_id < GetPaletteListLength())) // Custom palettes
  ){   

    uint16_t palette_adjusted_id = palette_id - PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED; // adjust back into correct indexing
    return custom_palettes[palette_adjusted_id].number_of_colours;  
      
  }

  
  /**************************************************************
   * 
   * Errors
   * 
  ***************************************************************/
  else
  {
    ALOG_INF(PSTR("Missing %d"), palette_id);
  }

  return 1; // error!


}

#ifdef ENABLE_FEATURE_PALETTE__RGBWW_COLOURS

IRAM_ATTR [[gnu::hot]] RgbwwColor      mPalette::SubGet_Encoded_Colour_ReadBuffer_RGBWW(
  uint8_t* palette_buffer,
  uint16_t pixel_position,  
  uint8_t* return_encoded_value,
  PALETTE_ENCODING_DATA encoding,
  uint8_t encoded_colour_width
) {
  // Precompute the index where the color starts (considering the gradient byte)
  uint16_t index_relative = pixel_position * encoded_colour_width;

  // Handle the gradient byte if gradient is enabled
  if (encoding.index_gradient) {
    // If the gradient is enabled, read it and skip over it to access the color data
    if (return_encoded_value != nullptr) {
      *return_encoded_value = palette_buffer[index_relative];  // store gradient value
    }
    index_relative++; // Move past the gradient byte to reach color data
  }

  // Fetch the color components based on the encoding flags
  uint8_t red        = (encoding.red_enabled)        ? palette_buffer[index_relative]   : 0;
  uint8_t green      = (encoding.green_enabled)      ? palette_buffer[index_relative+1] : 0;
  uint8_t blue       = (encoding.blue_enabled)       ? palette_buffer[index_relative+2] : 0;
  uint8_t white_cold = (encoding.white_cold_enabled) ? palette_buffer[index_relative+3] : 0;
  uint8_t white_warm = (encoding.white_warm_enabled) ? palette_buffer[index_relative+4] : 0;

  return RgbwwColor(red, green, blue, white_cold, white_warm);
}


/*
Simplified branching:
    Consolidated the logic for determining if the palette should act as a gradient or a discrete sequence into fewer branches.
    Made sure the key flags (flag_spanned_segment, flag_force_gradient, flag_crgb_exact_colour) are handled clearly and minimally.
Optimized handling of gradient palettes:
    The gradient logic is separated from the discrete palette logic, improving clarity.
    The loop for searching within the gradient range is simplified, and gradient scaling is handled directly using mapfloat as before.
Performance improvements:
    Eliminated redundant calculations, such as unnecessary recalculations of pixel_position_adjust.
    The gradient palette vector is now populated in a single pass, and boundary conditions are checked in an efficient loop.
*/
IRAM_ATTR [[gnu::hot]] RgbwwColor      mPalette::SubGet_Encoded_Palette_Colour_RGBWW(
  uint8_t* palette_buffer,
  uint16_t _pixel_position, 
  uint8_t encoded_colour_width,
  uint8_t colours_in_palette,
  PALETTE_ENCODING_DATA encoding,
  uint8_t* encoded_value, // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
  bool     flag_spanned_segment, 
  bool     flag_wrap_hard_edge,        
  bool     flag_crgb_exact_colour,
  bool     flag_force_gradient
){
  RgbwwColor colour;
  uint16_t pixel_position_adjust = _pixel_position;

  // Handling discrete sequence palettes (non-gradient)
  bool is_forced_to_get_discrete = flag_crgb_exact_colour;
  bool is_not_gradient = (encoding.index_gradient == false);
  bool is_basic_sequence_palette = is_forced_to_get_discrete || (is_not_gradient && !flag_spanned_segment);

  if (is_basic_sequence_palette) {
    // Handle non-gradient palette colors, with or without segment spanning
    if (flag_spanned_segment && !is_forced_to_get_discrete) {
      pixel_position_adjust = (_pixel_position * 255) / (tkr_anim->_virtualSegmentLength - 1);
    }

    // Map pixel position to color index in the palette
    pixel_position_adjust %= colours_in_palette;

    colour = SubGet_Encoded_Colour_ReadBuffer_RGBWW(
      palette_buffer,
      pixel_position_adjust,  
      encoded_value,
      encoding,
      encoded_colour_width
    );
    return colour;
  }

  // Handle gradient palettes or forced gradient
  if (encoding.index_gradient || flag_force_gradient) {
    if (flag_spanned_segment) {
      pixel_position_adjust = (_pixel_position * 255) / (tkr_anim->_virtualSegmentLength - 1);
    }

    // Set boundaries for gradient mapping
    uint8_t lower_limit = colours_in_palette / 2;
    uint8_t upper_limit = 255 - (colours_in_palette / 2);
    
    std::vector<uint8_t> gradient_palettes(colours_in_palette);
    if (encoding.index_gradient) {
      for (uint8_t i = 0; i < colours_in_palette; ++i) {
        SubGet_Encoded_Colour_ReadBuffer_RGBWW(
          palette_buffer,
          i,  
          &gradient_palettes[i],
          encoding,
          encoded_colour_width
        );
      }
    } else {
      for (uint8_t i = 0; i < colours_in_palette; ++i) {
        gradient_palettes[i] = map(i, 0, colours_in_palette - 1, lower_limit, upper_limit);
      }
    }

    // Search for lower and upper boundaries within gradient
    uint8_t lower_boundary_i = 0, upper_boundary_i = 0;
    uint8_t lower_boundary_v = 0, upper_boundary_v = 0;
    float progress = 0;

    if (pixel_position_adjust < lower_limit) {
      lower_boundary_i = 0;
      upper_boundary_i = 1;
      lower_boundary_v = gradient_palettes[lower_boundary_i];
      upper_boundary_v = gradient_palettes[upper_boundary_i];
      progress = 0;
    } else if (pixel_position_adjust > upper_limit) {
      lower_boundary_i = gradient_palettes.size() - 1;
      upper_boundary_i = gradient_palettes.size(); // ignored
      lower_boundary_v = gradient_palettes[lower_boundary_i];
      upper_boundary_v = gradient_palettes[upper_boundary_i];
      progress = 0;
    } else {
      for (uint8_t i = 0; i < gradient_palettes.size() - 1; ++i) {
        if (pixel_position_adjust >= gradient_palettes[i] && pixel_position_adjust < gradient_palettes[i + 1]) {
          lower_boundary_i = i;
          upper_boundary_i = i + 1;
          lower_boundary_v = gradient_palettes[lower_boundary_i];
          upper_boundary_v = gradient_palettes[upper_boundary_i];
          progress = mSupport::mapfloat(pixel_position_adjust, lower_boundary_v, upper_boundary_v, 0.0f, 1.0f);
          break;
        }
      }
    }

    // Blend between lower and upper color boundaries
    RgbwwColor lower_colour = SubGet_Encoded_Colour_ReadBuffer_RGBWW(
      palette_buffer,
      lower_boundary_i,  
      nullptr,
      encoding,
      encoded_colour_width
    );
    RgbwwColor upper_colour = SubGet_Encoded_Colour_ReadBuffer_RGBWW(
      palette_buffer,
      upper_boundary_i,  
      nullptr,
      encoding,
      encoded_colour_width
    );

    colour = RgbwwColor::LinearBlend(lower_colour, upper_colour, progress);

    // Set the encoded value if applicable
    if (encoded_value != nullptr) {
      *encoded_value = (pixel_position_adjust < 255) ? lower_boundary_v : upper_boundary_v;
    }

    return colour;
  }

  // Handle simple spanned palettes
  if (flag_spanned_segment) {
    pixel_position_adjust = (_pixel_position * 255) / (tkr_anim->_virtualSegmentLength - 1);
  }

  uint8_t palette_index = scale8(pixel_position_adjust, colours_in_palette - 1);
  
  colour = SubGet_Encoded_Colour_ReadBuffer_RGBWW(
    palette_buffer,
    palette_index,  
    encoded_value,
    encoding,
    encoded_colour_width
  );

  return colour;
}

#else

IRAM_ATTR [[gnu::hot]] uint32_t      mPalette::SubGet_Encoded_Colour_ReadBuffer_U32(
  uint8_t* palette_buffer,
  uint16_t pixel_position,  
  uint8_t* return_encoded_value,
  PALETTE_ENCODING_DATA encoding,
  uint8_t encoded_colour_width
) {
  // Precompute the index where the color starts (considering the gradient byte)
  uint16_t index_relative = pixel_position * encoded_colour_width;

  // Handle the gradient byte if gradient is enabled
  if (encoding.index_gradient) {
    // If the gradient is enabled, read it and skip over it to access the color data
    if (return_encoded_value != nullptr) {
      *return_encoded_value = palette_buffer[index_relative];  // store gradient value
    }
    index_relative++; // Move past the gradient byte to reach color data
  }

  // Fetch the color components based on the encoding flags
  uint8_t red        = (encoding.red_enabled)        ? palette_buffer[index_relative]   : 0;
  uint8_t green      = (encoding.green_enabled)      ? palette_buffer[index_relative+1] : 0;
  uint8_t blue       = (encoding.blue_enabled)       ? palette_buffer[index_relative+2] : 0;
  uint8_t white_cold = (encoding.white_cold_enabled) ? palette_buffer[index_relative+3] : 0;
  // uint8_t white_warm = (encoding.white_warm_enabled) ? palette_buffer[index_relative+4] : 0; // IGNORED IN RGBWW

  return RGBW32(red, green, blue, white_cold);
}



// IRAM_ATTR [[gnu::hot]] uint32_t      mPalette::SubGet_Encoded_Palette_Colour_U32(
//   uint8_t* palette_buffer,
//   uint16_t _pixel_position, 
//   uint8_t encoded_colour_width,
//   uint8_t colours_in_palette,
//   PALETTE_ENCODING_DATA encoding,
//   uint8_t* encoded_value, // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
//   bool     palette_index__format, 
//   bool     flag_wrap_hard_edge,        
//   uint8_t  force_palette_mode, //needs changed to uint8_t
//   bool     depreciated //flag_force_gradient
// ){
//   uint32_t colour;
//   uint16_t pixel_position_adjust = _pixel_position;

//   if(depreciated)
//   {
//     Serial.println("flag_force_gradient");
//   }

//   // Handling discrete sequence palettes (non-gradient)
//   bool is_forced_to_get_discrete = (force_palette_mode == PALETTE_MODE__FORCE_DISCRETE);
//   bool is_not_gradient = (encoding.index_gradient == false);
//   // bool is_basic_sequence_palette = is_forced_to_get_discrete || (is_not_gradient);

//   bool is_forced_to_get_gradient = (force_palette_mode == PALETTE_MODE__FORCE_GRADIENT);
//   bool is_basic_sequence_palette = is_forced_to_get_discrete || (is_not_gradient && !is_forced_to_get_gradient);


//   /**
//    * The check for !palette_index__format prevents discrete palettes without gradient info from entering the discrete logic under PALETTE_MODE__DEFAULT. 
//    * But your new logic expects all non-gradient palettes to behave discrete by default — regardless of segment spanning.
//    **/

//   // Serial.printf("flags %d:%d:%d\n\r", is_forced_to_get_discrete, is_not_gradient, is_basic_sequence_palette);

//   /**
//    * DESC: Discrete palette (no index)
//    **/
//   if (is_basic_sequence_palette) 
//   {
//     // Handle non-gradient palette colors, with or without segment spanning
//     if (palette_index__format && !is_forced_to_get_discrete && !is_not_gradient) {
//       pixel_position_adjust = (_pixel_position * 255) / (tkr_anim->_virtualSegmentLength - 1);
//     }
    
//     // Map pixel position to color index in the palette
//     pixel_position_adjust %= colours_in_palette;

//     // if(_pixel_position<30){
//     //   Serial.printf("palette_index %d %d:%d %d\n\r",  pixel_position_adjust, _pixel_position, force_palette_mode, colours_in_palette);
//     // }

//     colour = SubGet_Encoded_Colour_ReadBuffer_U32(
//       palette_buffer,
//       pixel_position_adjust,  
//       encoded_value,
//       encoding,
//       encoded_colour_width
//     );
//     return colour;
//   }

//   /**
//    * DESC: Gradient palette (contains index, or forced into gradient with equal spacing)
//    **/
//   if (encoding.index_gradient || (force_palette_mode == PALETTE_MODE__FORCE_GRADIENT)) 
//   {
//     // Serial.println("FG");
//     if (palette_index__format) {
//       pixel_position_adjust = (_pixel_position * 255) / (tkr_anim->_virtualSegmentLength - 1);
//     }

//     // Set boundaries for gradient mapping
//     uint8_t gradient_scan_lower = 0;
//     uint8_t gradient_scan_upper = 255;

//     if (!encoding.index_gradient) {
//       gradient_scan_lower = 0;
//       gradient_scan_upper = 255;
//     }

    
//     std::vector<uint8_t> gradient_palettes(colours_in_palette);
//     if (encoding.index_gradient) {
//       for (uint8_t i = 0; i < colours_in_palette; ++i) {
//         SubGet_Encoded_Colour_ReadBuffer_U32(
//           palette_buffer,
//           i,  
//           &gradient_palettes[i],
//           encoding,
//           encoded_colour_width
//         );
//       }
//     } else {
//       for (uint8_t i = 0; i < colours_in_palette; ++i) {
//         gradient_palettes[i] = map(i, 0, colours_in_palette - 1, gradient_scan_lower, gradient_scan_upper );
//       }
//     }
    
//     for (uint8_t i = 0; i < colours_in_palette; ++i) { Serial.printf("%d,",gradient_palettes[i]);} Serial.println();

//     // Search for lower and upper boundaries within gradient
//     uint8_t lower_boundary_i = 0, upper_boundary_i = 0;
//     uint8_t lower_boundary_v = 0, upper_boundary_v = 0;
//     uint8_t progress = 0;

//     if (pixel_position_adjust < gradient_scan_lower) {
//       lower_boundary_i = 0;
//       upper_boundary_i = 1;
//       lower_boundary_v = gradient_palettes[lower_boundary_i];
//       upper_boundary_v = gradient_palettes[upper_boundary_i];
//       progress = 0;
//     } else if (pixel_position_adjust > gradient_scan_upper ) {
//       lower_boundary_i = gradient_palettes.size() - 1;
//       upper_boundary_i = gradient_palettes.size(); // ignored
//       lower_boundary_v = gradient_palettes[lower_boundary_i];
//       upper_boundary_v = gradient_palettes[upper_boundary_i];
//       progress = 0;
//     } else {
//       for (uint8_t i = 0; i < gradient_palettes.size() - 1; ++i) {
//         if (pixel_position_adjust >= gradient_palettes[i] && pixel_position_adjust < gradient_palettes[i + 1]) {
//           lower_boundary_i = i;
//           upper_boundary_i = i + 1;
//           lower_boundary_v = gradient_palettes[lower_boundary_i];
//           upper_boundary_v = gradient_palettes[upper_boundary_i];
//           // progress = mSupport::mapfloat(pixel_position_adjust, lower_boundary_v, upper_boundary_v, 0.0f, 1.0f);
//           progress = map(pixel_position_adjust, lower_boundary_v, upper_boundary_v, 0, 255);
//           break;
//         }
//       }
//     }

//     // Blend between lower and upper color boundaries
//     uint32_t lower_colour = SubGet_Encoded_Colour_ReadBuffer_U32(
//       palette_buffer,
//       lower_boundary_i,  
//       nullptr,
//       encoding,
//       encoded_colour_width
//     );
//     uint32_t upper_colour = SubGet_Encoded_Colour_ReadBuffer_U32(
//       palette_buffer,
//       upper_boundary_i,  
//       nullptr,
//       encoding,
//       encoded_colour_width
//     );

//     colour = mAnimatorLight::ColourBlend(lower_colour, upper_colour, progress);

//     // Set the encoded value if applicable
//     if (encoded_value != nullptr) {
//       *encoded_value = (pixel_position_adjust < 255) ? lower_boundary_v : upper_boundary_v;
//     }

//     return colour;
//   }

//   /**
//    * DESC: Spanned palette ("Band" mode, causes discrete palette to repeat into sections)
//    **/
//   // Handle simple spanned palettes
//   if (palette_index__format) {
//     pixel_position_adjust = (_pixel_position * 255) / (tkr_anim->_virtualSegmentLength - 1);
//   }

//   uint16_t palette_index = scale8(pixel_position_adjust, colours_in_palette - 1); // ?? is scale the right term, surely it should be modulo??
  
//   // uint16_t palette_index = pixel_position_adjust % (colours_in_palette); // ?? is scale the right term, surely it should be modulo??
  
//   // if(_pixel_position<30)
//   // {
//   //   Serial.printf("palette_index %d %d:%d:%d %d\n\r",  pixel_position_adjust, _pixel_position, palette_index, force_palette_mode, colours_in_palette);
//   // }

//   colour = SubGet_Encoded_Colour_ReadBuffer_U32(
//     palette_buffer,
//     palette_index,  
//     encoded_value,
//     encoding,
//     encoded_colour_width
//   );

//   return colour;
// }


// IRAM_ATTR [[gnu::hot]] uint32_t mPalette::SubGet_Encoded_Palette_Colour_U32(
//   uint8_t* palette_buffer,
//   uint16_t _pixel_position, 
//   uint8_t encoded_colour_width,
//   uint8_t colours_in_palette,
//   PALETTE_ENCODING_DATA encoding,
//   uint8_t* encoded_value, // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
//   bool     palette_index__format, 
//   bool     flag_wrap_hard_edge,        
//   uint8_t  force_palette_mode,
//   bool     depreciated
// ) {
//   uint32_t colour;
//   uint16_t pixel_position_adjust = _pixel_position;

//   if (depreciated) {
//     Serial.println("flag_force_gradient");
//   }

//   bool is_forced_to_get_discrete = (force_palette_mode == PALETTE_MODE__FORCE_DISCRETE);
//   bool is_forced_to_get_gradient = (force_palette_mode == PALETTE_MODE__FORCE_GRADIENT);
//   bool is_not_gradient = (encoding.index_gradient == false);
//   bool is_basic_sequence_palette = is_forced_to_get_discrete || (is_not_gradient && !is_forced_to_get_gradient);

//   /**
//    * DESC: Discrete palette (no index)
//    **/
//   if (is_basic_sequence_palette) {
    // if (palette_index__format && !is_forced_to_get_discrete && !is_not_gradient) {
    //   pixel_position_adjust = (_pixel_position * 255) / (tkr_anim->_virtualSegmentLength - 1);
    // }

//     pixel_position_adjust %= colours_in_palette;

//     return SubGet_Encoded_Colour_ReadBuffer_U32(
//       palette_buffer,
//       pixel_position_adjust,
//       encoded_value,
//       encoding,
//       encoded_colour_width
//     );
//   }

//   /**
//    * DESC: Gradient palette (contains index, or forced into gradient with equal spacing)
//    **/
//   if (encoding.index_gradient || is_forced_to_get_gradient) {
//     if (palette_index__format) {
//       pixel_position_adjust = (_pixel_position * 255) / (tkr_anim->_virtualSegmentLength - 1);
//     }

//     std::vector<uint8_t> gradient_palettes(colours_in_palette);
//     if (encoding.index_gradient) {
//       for (uint8_t i = 0; i < colours_in_palette; ++i) {
//         SubGet_Encoded_Colour_ReadBuffer_U32(
//           palette_buffer,
//           i,
//           &gradient_palettes[i],
//           encoding,
//           encoded_colour_width
//         );
//       }
//     } else {
//       for (uint8_t i = 0; i < colours_in_palette; ++i) {
//         if (flag_wrap_hard_edge) {
//           gradient_palettes[i] = map(i, 0, colours_in_palette - 1, 0, 255);
//         } else {
//           gradient_palettes[i] = map(i, 0, colours_in_palette, 0, 255);
//         }
//       }
//     }

//     uint8_t lower_i = 0, upper_i = 0;
//     uint8_t lower_v = 0, upper_v = 0;
//     uint8_t progress = 0;

//     if (pixel_position_adjust < gradient_palettes[0]) {
//       lower_i = 0;
//       upper_i = 1;
//       lower_v = gradient_palettes[lower_i];
//       upper_v = gradient_palettes[upper_i];
//       progress = 0;
//     }
//     else if (pixel_position_adjust >= gradient_palettes[colours_in_palette - 1]) {
//       lower_i = colours_in_palette - 1;
//       if (flag_wrap_hard_edge || encoding.index_gradient) {
//         upper_i = lower_i;
//         lower_v = gradient_palettes[lower_i];
//         upper_v = gradient_palettes[lower_i];
//         progress = 0;
//       } else {
//         upper_i = 0;
//         lower_v = gradient_palettes[lower_i];
//         upper_v = 255;
//         progress = map(pixel_position_adjust, lower_v, upper_v, 0, 255);
//       }
//     }
//     else {
//       for (uint8_t i = 0; i < colours_in_palette - 1; ++i) {
//         if (pixel_position_adjust >= gradient_palettes[i] && pixel_position_adjust < gradient_palettes[i + 1]) {
//           lower_i = i;
//           upper_i = i + 1;
//           lower_v = gradient_palettes[lower_i];
//           upper_v = gradient_palettes[upper_i];
//           progress = map(pixel_position_adjust, lower_v, upper_v, 0, 255);
//           break;
//         }
//       }
//     }

//     uint32_t lower_colour = SubGet_Encoded_Colour_ReadBuffer_U32(
//       palette_buffer,
//       lower_i,
//       nullptr,
//       encoding,
//       encoded_colour_width
//     );
//     uint32_t upper_colour = SubGet_Encoded_Colour_ReadBuffer_U32(
//       palette_buffer,
//       upper_i,
//       nullptr,
//       encoding,
//       encoded_colour_width
//     );

//     colour = mAnimatorLight::ColourBlend(lower_colour, upper_colour, progress);

//     if (encoded_value != nullptr) {
//       *encoded_value = (pixel_position_adjust < 255) ? lower_v : upper_v;
//     }

//     return colour;
//   }

//   /**
//    * DESC: Spanned palette ("Band" mode, causes discrete palette to repeat into sections)
//    **/
//   if (palette_index__format) {
//     pixel_position_adjust = (_pixel_position * 255) / (tkr_anim->_virtualSegmentLength - 1);
//   }

//   uint16_t palette_index = scale8(pixel_position_adjust, colours_in_palette - 1);

//   return SubGet_Encoded_Colour_ReadBuffer_U32(
//     palette_buffer,
//     palette_index,
//     encoded_value,
//     encoding,
//     encoded_colour_width
//   );
// }

/***
 * Notes

palette_index__format is now a uint8_t and is expected to be one of:
PALETTE_INDEX__IS_255_RANGE (treat _pixel_position as 0..255),
PALETTE_INDEX__IS_SEGLEN_RANGE (map 0..SEGLEN-1 → 0..255 or → entry index),
PALETTE_INDEX__IS_EXACT_COLOUR (use _pixel_position directly as the palette entry for discrete, or as 0..255 coordinate for gradient/banded).
Discrete branch always resolves a final entry index (0..colours-1) and fetches it.
Gradient branch normalizes to 0..255 (per mode), finds surrounding stops, and blends.
Banded (“spanned”) branch normalizes to 0..255 then scales into an entry index.
No other behavior changes; all your existing encoding paths are preserved.
 */
// IRAM_ATTR [[gnu::hot]] uint32_t mPalette::SubGet_Encoded_Palette_Colour_U32(
//   uint8_t* palette_buffer,
//   uint16_t _pixel_position, 
//   uint8_t  encoded_colour_width,
//   uint8_t  colours_in_palette,
//   PALETTE_ENCODING_DATA encoding,
//   uint8_t* encoded_value,              // Must be a valid address if you want encoded out; nullptr ok if not needed
//   uint8_t  palette_index__format,      // PALETTE_INDEX__IS_255_RANGE / __IS_SEGLEN_RANGE / __IS_EXACT_COLOUR
//   bool     flag_wrap_hard_edge,
//   uint8_t  force_palette_mode,         // PALETTE_MODE__FORCE_DEFAULT / __FORCE_DISCRETE / __FORCE_GRADIENT
//   bool     depreciated
// ) {
//   uint32_t colour;
//   uint16_t pixel_position_adjust = _pixel_position;

//   if (depreciated) {
//     Serial.println("flag_force_gradient"); // legacy hook
//   }

//   const bool is_forced_discrete = (force_palette_mode == PALETTE_MODE__FORCE_DISCRETE);
//   const bool is_forced_gradient = (force_palette_mode == PALETTE_MODE__FORCE_GRADIENT);
//   const bool is_not_gradient    = (encoding.index_gradient == false);

//   // "Basic sequence" == treat as discrete steps unless explicitly forced to gradient
//   const bool is_basic_sequence_palette = is_forced_discrete || (is_not_gradient && !is_forced_gradient);

//   /**************************************************************
//    * DISCRETE PALETTES (no index) OR forced discrete
//    * - Functional parity with your original logic.
//    * - CRITICAL line is preserved: when the caller passes a SEGLEN-based
//    *   index *and* the underlying palette is actually gradient and NOT forced
//    *   discrete, we first rescale 0..SEGLEN-1 → 0..255 (same as your code).
//    * - Final selection always folds to 0..(colours_in_palette-1).
//    **************************************************************/
//   if (is_basic_sequence_palette) {
//     uint16_t pixel_position_adjust = _pixel_position;

//     // *** CRITICAL: keep your original rescale-to-255 condition ***
//     // Original: if (palette_index__format && !is_forced_to_get_discrete && !is_not_gradient)
//     // With tri-state, “mapping happened” == PALETTE_INDEX__IS_SEGLEN_RANGE.
//     if (palette_index__format == PALETTE_INDEX__IS_SEGLEN_RANGE
//         && !is_forced_discrete
//         && !is_not_gradient) {
//       pixel_position_adjust = (tkr_anim->_virtualSegmentLength > 1)
//         ? (uint16_t)((uint32_t)_pixel_position * 255u / (tkr_anim->_virtualSegmentLength - 1))
//         : 0;
//     }

//     // Normalize other input formats (no proportional scaling here).
//     //  - 255-range: use low byte (like before), then modulo below.
//     //  - exact-colour: leave as-is; modulo below.
//     if (palette_index__format == PALETTE_INDEX__IS_255_RANGE) {
//       pixel_position_adjust &= 0xFFu;
//     }
//     // PALETTE_INDEX__IS_EXACT_COLOUR → leave pixel_position_adjust as-is

//     // Final fold into the palette entry range (unchanged behavior).
//     pixel_position_adjust %= colours_in_palette;

//     return SubGet_Encoded_Colour_ReadBuffer_U32(
//         palette_buffer,
//         (uint8_t)pixel_position_adjust,
//         encoded_value,
//         encoding,
//         encoded_colour_width
//     );
//   }


//   /**************************************************************
//    * GRADIENT PALETTES (has index) OR forced gradient
//    * - Preserve original: rescale to 0..255 ONLY when caller passed
//    *   a SEGLEN-based index.
//    **************************************************************/
//   if (encoding.index_gradient || is_forced_gradient) {
//     uint16_t pixel_position_adjust = _pixel_position;

//     // Original behavior: when "mapping happened" (old boolean true),
//     // convert 0..SEGLEN-1 → 0..255.
//     if (palette_index__format == PALETTE_INDEX__IS_SEGLEN_RANGE) {
//       pixel_position_adjust = (tkr_anim->_virtualSegmentLength > 1)
//         ? (uint16_t)((uint32_t)_pixel_position * 255u / (tkr_anim->_virtualSegmentLength - 1))
//         : 0;
//     } else if (palette_index__format == PALETTE_INDEX__IS_255_RANGE) {
//       // Caller already gave a 0..255 domain. Keep it in range (parity with old code).
//       pixel_position_adjust &= 0xFFu;
//     }
//     // EXACT_COLOUR → leave as provided (parity: old "false" path used value as-is)

//     // Build positions of the gradient stops
//     std::vector<uint8_t> gradient_positions(colours_in_palette);
//     if (encoding.index_gradient) {
//       for (uint8_t i = 0; i < colours_in_palette; ++i) {
//         SubGet_Encoded_Colour_ReadBuffer_U32(
//           palette_buffer, i, &gradient_positions[i],
//           encoding, encoded_colour_width
//         );
//       }
//     } else {
//       // Forced gradient → evenly spaced stops (optionally hard-edge limited)
//       for (uint8_t i = 0; i < colours_in_palette; ++i) {
//         gradient_positions[i] = flag_wrap_hard_edge
//           ? map(i, 0, colours_in_palette - 1, 0, 255)
//           : map(i, 0, colours_in_palette,     0, 255);
//       }
//     }

//     // Locate [lower, upper] span and compute progress
//     uint8_t lower_i = 0, upper_i = 0;
//     uint8_t lower_v = 0, upper_v = 0;
//     uint8_t progress = 0;

//     if (pixel_position_adjust < gradient_positions[0]) {
//       lower_i = 0; upper_i = 1;
//       lower_v = gradient_positions[lower_i];
//       upper_v = gradient_positions[upper_i];
//       progress = 0;
//     } else if (pixel_position_adjust >= gradient_positions[colours_in_palette - 1]) {
//       lower_i = colours_in_palette - 1;
//       if (flag_wrap_hard_edge || encoding.index_gradient) {
//         upper_i = lower_i;
//         lower_v = gradient_positions[lower_i];
//         upper_v = gradient_positions[lower_i];
//         progress = 0;
//       } else {
//         upper_i = 0;
//         lower_v = gradient_positions[lower_i];
//         upper_v = 255;
//         progress = map(pixel_position_adjust, lower_v, upper_v, 0, 255);
//       }
//     } else {
//       for (uint8_t i = 0; i < colours_in_palette - 1; ++i) {
//         if (pixel_position_adjust >= gradient_positions[i] &&
//             pixel_position_adjust <  gradient_positions[i + 1]) {
//           lower_i = i; upper_i = i + 1;
//           lower_v = gradient_positions[lower_i];
//           upper_v = gradient_positions[upper_i];
//           progress = map(pixel_position_adjust, lower_v, upper_v, 0, 255);
//           break;
//         }
//       }
//     }

//     // Fetch the bounding colours and blend
//     const uint32_t lower_colour = SubGet_Encoded_Colour_ReadBuffer_U32(
//         palette_buffer, lower_i, nullptr, encoding, encoded_colour_width);
//     const uint32_t upper_colour = SubGet_Encoded_Colour_ReadBuffer_U32(
//         palette_buffer, upper_i, nullptr, encoding, encoded_colour_width);

//     colour = mAnimatorLight::ColourBlend(lower_colour, upper_colour, progress);

//     if (encoded_value != nullptr) {
//       *encoded_value = (pixel_position_adjust < 255) ? lower_v : upper_v;
//     }
//     return colour;
//   }

//   /**************************************************************
//    * SPANNED (“band”) PALETTE
//    * - Original: when caller had mapping (old true), convert to 0..255.
//    * - Then scale 0..255 → 0..(colours-1).
//    **************************************************************/
//   {
//     uint16_t pixel_position_adjust = _pixel_position;

//     if (palette_index__format == PALETTE_INDEX__IS_SEGLEN_RANGE) {
//       pixel_position_adjust = (tkr_anim->_virtualSegmentLength > 1)
//         ? (uint16_t)((uint32_t)_pixel_position * 255u / (tkr_anim->_virtualSegmentLength - 1))
//         : 0;
//     } else if (palette_index__format == PALETTE_INDEX__IS_255_RANGE) {
//       pixel_position_adjust &= 0xFFu; // already 0..255
//     }
//     // EXACT_COLOUR → use as provided (parity with old false-path)

//     const uint16_t palette_index = scale8(pixel_position_adjust, colours_in_palette - 1);

//     return SubGet_Encoded_Colour_ReadBuffer_U32(
//       palette_buffer,
//       (uint8_t)palette_index,
//       encoded_value,
//       encoding,
//       encoded_colour_width
//     );
//   }

// }

IRAM_ATTR [[gnu::hot]] uint32_t mPalette::SubGet_Encoded_Palette_Colour_U32(
  uint8_t* palette_buffer,
  uint16_t _pixel_position, 
  uint8_t  encoded_colour_width,
  uint8_t  colours_in_palette,
  PALETTE_ENCODING_DATA encoding,
  uint8_t* encoded_value,              // Must be a valid address if you want encoded out; nullptr ok if not needed
  uint8_t  palette_index__format,      // PALETTE_INDEX__IS_255_RANGE / __IS_SEGLEN_RANGE / __IS_EXACT_COLOUR
  bool     flag_wrap_hard_edge,
  uint8_t  force_palette_mode,         // PALETTE_MODE__FORCE_DEFAULT / __FORCE_DISCRETE / __FORCE_GRADIENT
  bool     depreciated
) {
  uint32_t colour;

  if (depreciated) {
    Serial.println("flag_force_gradient"); // legacy hook
  }

  const bool is_forced_discrete = (force_palette_mode == PALETTE_MODE__FORCE_DISCRETE);
  const bool is_forced_gradient = (force_palette_mode == PALETTE_MODE__FORCE_GRADIENT);
  const bool is_not_gradient    = (encoding.index_gradient == false);

  // "Basic sequence" == treat as discrete steps unless explicitly forced to gradient
  const bool is_basic_sequence_palette = is_forced_discrete || (is_not_gradient && !is_forced_gradient);

  /**************************************************************
   * DISCRETE PALETTES (no index) OR forced discrete
   * - Functional parity with your original logic.
   * - CRITICAL line is preserved: when the caller passes a SEGLEN-based
   *   index *and* the underlying palette is actually gradient and NOT forced
   *   discrete, we first rescale 0..SEGLEN-1 → 0..255 (same as your code).
   * - Final selection always folds to 0..(colours_in_palette-1).
   **************************************************************/
  if (is_basic_sequence_palette) {
    uint16_t pixel_position_adjust = _pixel_position;

    // Serial.print("DP ");
    // Serial.print(_pixel_position);
    // Serial.flush();
    // delay(100);

    // *** CRITICAL: keep your original rescale-to-255 condition ***
    // Original: if (palette_index__format && !is_forced_to_get_discrete && !is_not_gradient)
    // With tri-state, “mapping happened” == PALETTE_INDEX__IS_SEGLEN_RANGE.
    // if (palette_index__format == PALETTE_INDEX__IS_SEGLEN_RANGE
    //     && !is_forced_discrete
    //     && !is_not_gradient) { // HERE! is this not a contradiction? if its in seglen range, isnt it always not not gradient?
    //   pixel_position_adjust = (tkr_anim->_virtualSegmentLength > 1)
    //     ? (uint16_t)((uint32_t)_pixel_position * 255u / (tkr_anim->_virtualSegmentLength - 1))
    //     : 0;
    // }

#ifdef ENABLE_DEVFEATURE_LIGHT__GRADIENT_PATCH_4DEC25
    // NEW: allow 0..SEGLEN rescaling only if the underlying palette *is* gradient.
    if (palette_index__format == PALETTE_INDEX__IS_SEGLEN_RANGE
        && encoding.index_gradient
        && !is_forced_discrete)
    {
      pixel_position_adjust = (tkr_anim->_virtualSegmentLength > 1)
        ? (uint16_t)((uint32_t)_pixel_position * 255u / (tkr_anim->_virtualSegmentLength - 1))
        : 0;
    }
#else
    // OLD (broken / unreachable) logic for reference
    if (palette_index__format == PALETTE_INDEX__IS_SEGLEN_RANGE
        && !is_forced_discrete
        && !is_not_gradient)
    {
      pixel_position_adjust = (tkr_anim->_virtualSegmentLength > 1)
        ? (uint16_t)((uint32_t)_pixel_position * 255u / (tkr_anim->_virtualSegmentLength - 1))
        : 0;
    }
#endif










    // Serial.print("DP2 ");
    // Serial.print(_pixel_position);
    // Serial.flush();
    // delay(100);


    // // Normalize other input formats (no proportional scaling here).
    // //  - 255-range: use low byte (like before), then modulo below.
    // //  - exact-colour: leave as-is; modulo below.
    // if (palette_index__format == PALETTE_INDEX__IS_255_RANGE) {
    //   pixel_position_adjust &= 0xFFu;
    // }
    // // PALETTE_INDEX__IS_EXACT_COLOUR → leave pixel_position_adjust as-is

    // B) Caller passed 0..255 → simple sequence tracking:
    //    - if v==0: reset sequence
    //    - else   : advance by 1 (wrap by N)
    //    This yields RGBORGBO… cadence irrespective of the exact 0..255 value.
    if (palette_index__format == PALETTE_INDEX__IS_255_RANGE) {
      const uint8_t v = (uint8_t)(_pixel_position & 0xFFu);

      if (colours_in_palette == 0) {
        // Safety: nothing to index
        return SubGet_Encoded_Colour_ReadBuffer_U32(
          palette_buffer, 0, encoded_value, encoding, encoded_colour_width
        );
      }

      if (v == 0) {
        // Reset sequence at band/loop start
        tracked_previous_palette_index = 0;
      } else {
        // Advance one step each call, rolling over palette length
        tracked_previous_palette_index =
          (uint16_t)((tracked_previous_palette_index + 1) % colours_in_palette);
      }

      return SubGet_Encoded_Colour_ReadBuffer_U32(
        palette_buffer,
        (uint8_t)tracked_previous_palette_index,
        encoded_value,
        encoding,
        encoded_colour_width
      );
    }


    // Final fold into the palette entry range (unchanged behavior).
    pixel_position_adjust %= colours_in_palette;

    return SubGet_Encoded_Colour_ReadBuffer_U32(
        palette_buffer,
        (uint8_t)pixel_position_adjust,
        encoded_value,
        encoding,
        encoded_colour_width
    );
  }

  /**************************************************************
   * GRADIENT PALETTES (has index) OR forced gradient
   * - Preserve original: rescale to 0..255 ONLY when caller passed
   *   a SEGLEN-based index. Otherwise use 0..255 as-is.
   **************************************************************/
  if (encoding.index_gradient || is_forced_gradient) {
    uint16_t pixel_position_adjust = _pixel_position;

    if (palette_index__format == PALETTE_INDEX__IS_SEGLEN_RANGE) {
      pixel_position_adjust = (tkr_anim->_virtualSegmentLength > 1)
        ? (uint16_t)((uint32_t)_pixel_position * 255u / (tkr_anim->_virtualSegmentLength - 1))
        : 0;
    } else if (palette_index__format == PALETTE_INDEX__IS_255_RANGE) {
      pixel_position_adjust &= 0xFFu;
    }
    // EXACT_COLOUR → leave as provided

    // Build positions of the gradient stops
    std::vector<uint8_t> gradient_positions(colours_in_palette);
    if (encoding.index_gradient) {
      for (uint8_t i = 0; i < colours_in_palette; ++i) {
        SubGet_Encoded_Colour_ReadBuffer_U32(
          palette_buffer, i, &gradient_positions[i],
          encoding, encoded_colour_width
        );
      }
    } else {
      // Forced gradient → evenly spaced stops (optionally hard-edge limited)
      for (uint8_t i = 0; i < colours_in_palette; ++i) {
        gradient_positions[i] = flag_wrap_hard_edge
          ? map(i, 0, colours_in_palette - 1, 0, 255)
          : map(i, 0, colours_in_palette,     0, 255);
      }
    }

    // Locate [lower, upper] span and compute progress
    uint8_t lower_i = 0, upper_i = 0;
    uint8_t lower_v = 0, upper_v = 0;
    uint8_t progress = 0;

    if (pixel_position_adjust < gradient_positions[0]) {
      lower_i = 0; upper_i = 1;
      lower_v = gradient_positions[lower_i];
      upper_v = gradient_positions[upper_i];
      progress = 0;
    } else if (pixel_position_adjust >= gradient_positions[colours_in_palette - 1]) {
      lower_i = colours_in_palette - 1;
      if (flag_wrap_hard_edge || encoding.index_gradient) {
        upper_i = lower_i;
        lower_v = gradient_positions[lower_i];
        upper_v = gradient_positions[lower_i];
        progress = 0;
      } else {
        upper_i = 0;
        lower_v = gradient_positions[lower_i];
        upper_v = 255;
        progress = map(pixel_position_adjust, lower_v, upper_v, 0, 255);
      }
    } else {
      for (uint8_t i = 0; i < colours_in_palette - 1; ++i) {
        if (pixel_position_adjust >= gradient_positions[i] &&
            pixel_position_adjust <  gradient_positions[i + 1]) {
          lower_i = i; upper_i = i + 1;
          lower_v = gradient_positions[lower_i];
          upper_v = gradient_positions[upper_i];
          progress = map(pixel_position_adjust, lower_v, upper_v, 0, 255);
          break;
        }
      }
    }

    // Fetch the bounding colours and blend
    const uint32_t lower_colour = SubGet_Encoded_Colour_ReadBuffer_U32(
        palette_buffer, lower_i, nullptr, encoding, encoded_colour_width);
    const uint32_t upper_colour = SubGet_Encoded_Colour_ReadBuffer_U32(
        palette_buffer, upper_i, nullptr, encoding, encoded_colour_width);

    colour = mAnimatorLight::ColourBlend(lower_colour, upper_colour, progress);

    if (encoded_value != nullptr) {
      *encoded_value = (pixel_position_adjust < 255) ? lower_v : upper_v;
    }
    return colour;
  }

  /**************************************************************
   * SPANNED (“band”) PALETTE
   * - Original: when caller had mapping (old true), convert to 0..255.
   * - Then scale 0..255 → 0..(colours-1).
   **************************************************************/
  {
    uint16_t pixel_position_adjust = _pixel_position;

    if (palette_index__format == PALETTE_INDEX__IS_SEGLEN_RANGE) {
      pixel_position_adjust = (tkr_anim->_virtualSegmentLength > 1)
        ? (uint16_t)((uint32_t)_pixel_position * 255u / (tkr_anim->_virtualSegmentLength - 1))
        : 0;
    } else if (palette_index__format == PALETTE_INDEX__IS_255_RANGE) {
      pixel_position_adjust &= 0xFFu; // already 0..255
    }
    // EXACT_COLOUR → use as provided

    const uint16_t palette_index = scale8(pixel_position_adjust, colours_in_palette - 1);

    return SubGet_Encoded_Colour_ReadBuffer_U32(
      palette_buffer,
      (uint8_t)palette_index,
      encoded_value,
      encoding,
      encoded_colour_width
    );
  }
}


#endif


#endif // header gaurd
