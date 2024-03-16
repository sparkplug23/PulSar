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
    PALETTELIST_STATIC_COMPRESSED_RAINBOW__ID, 
    PM_PALETTE_COMPRESSED_RAINBOW__DATA, 
    sizeof(PM_PALETTE_COMPRESSED_RAINBOW__DATA),
    D_PALETTE_COMPRESSED_RAINBOW_ENCODING
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
    PALETTELIST_STATIC__CHRISTMAS_TRADITIONAL_RGPBO__ID, 
    PM_PALETTE__CHRISTMAS_TRADITIONAL_RGPBO__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_TRADITIONAL_RGPBO__DATA),
    D_PALETTE__CHRISTMAS_TRADITIONAL_RGPBO__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__CHRISTMAS_TRADITIONAL_RGPBY__ID, 
    PM_PALETTE__CHRISTMAS_TRADITIONAL_RGPBY__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_TRADITIONAL_RGPBY__DATA),
    D_PALETTE__CHRISTMAS_TRADITIONAL_RGPBY__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__CHRISTMAS_TRADITIONAL_ROGPBY__ID, 
    PM_PALETTE__CHRISTMAS_TRADITIONAL_ROGPBY__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_TRADITIONAL_ROGPBY__DATA),
    D_PALETTE__CHRISTMAS_TRADITIONAL_ROGPBY__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__CHRISTMAS_TRADITIONAL_RGBO__ID, 
    PM_PALETTE__CHRISTMAS_TRADITIONAL_RGBO__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_TRADITIONAL_RGBO__DATA),
    D_PALETTE__CHRISTMAS_TRADITIONAL_RGBO__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__CHRISTMAS_TRADITIONAL_RGBY__ID, 
    PM_PALETTE__CHRISTMAS_TRADITIONAL_RGBY__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_TRADITIONAL_RGBY__DATA),
    D_PALETTE__CHRISTMAS_TRADITIONAL_RGBY__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__CHRISTMAS_COLD_WHITE__ID, 
    PM_PALETTE__CHRISTMAS_COLD_WHITE__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_COLD_WHITE__DATA),
    D_PALETTE__CHRISTMAS_COLD_WHITE__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__CHRISTMAS_WARM_WHITE__ID, 
    PM_PALETTE__CHRISTMAS_WARM_WHITE__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_WARM_WHITE__DATA),
    D_PALETTE__CHRISTMAS_WARM_WHITE__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__CHRISTMAS_VINTAGE_MINIBELLS__ID, 
    PM_PALETTE__CHRISTMAS_VINTAGE_MINIBELLS__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_VINTAGE_MINIBELLS__DATA),
    D_PALETTE__CHRISTMAS_VINTAGE_MINIBELLS__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__CHRISTMAS_VINTAGE_MERRYLITES__ID, 
    PM_PALETTE__CHRISTMAS_VINTAGE_MERRYLITES__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_VINTAGE_MERRYLITES__DATA),
    D_PALETTE__CHRISTMAS_VINTAGE_MERRYLITES__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__CHRISTMAS_VINTAGE_AGED_BULBS__ID, 
    PM_PALETTE__CHRISTMAS_VINTAGE_AGED_BULBS__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_VINTAGE_AGED_BULBS__DATA),
    D_PALETTE__CHRISTMAS_VINTAGE_AGED_BULBS__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__CHRISTMAS_SNOWY_COLOURS_01__ID, 
    PM_PALETTE__CHRISTMAS_SNOWY_COLOURS_01__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_SNOWY_COLOURS_01__DATA),
    D_PALETTE__CHRISTMAS_SNOWY_COLOURS_01__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__CHRISTMAS_SNOWY_COLOURS_02__ID, 
    PM_PALETTE__CHRISTMAS_SNOWY_COLOURS_02__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_SNOWY_COLOURS_02__DATA),
    D_PALETTE__CHRISTMAS_SNOWY_COLOURS_02__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__CHRISTMAS_SNOWY_COLOURS_03__ID, 
    PM_PALETTE__CHRISTMAS_SNOWY_COLOURS_03__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_SNOWY_COLOURS_03__DATA),
    D_PALETTE__CHRISTMAS_SNOWY_COLOURS_03__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__CHRISTMAS_BERRY_YELLOW__ID, 
    PM_PALETTE__CHRISTMAS_BERRY_YELLOW__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_BERRY_YELLOW__DATA),
    D_PALETTE__CHRISTMAS_BERRY_YELLOW__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__CHRISTMAS_BERRY_ORANGE__ID, 
    PM_PALETTE__CHRISTMAS_BERRY_ORANGE__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_BERRY_ORANGE__DATA),
    D_PALETTE__CHRISTMAS_BERRY_ORANGE__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__CHRISTMAS_BERRY_GREEN__ID, 
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

  /****************
   * DynamicPalettes : Do all dynamic palettes have no data? actually, I may want to encode data to be used for these (not RGB data, but byte encoded data), so keep
   ****************/

  addDynamicPalette(
    PALETTELIST_DYNAMIC_CRGBPALETTE16__RANDOMISE_COLOURS_01_RANDOM_HUE__ID, 
    nullptr, 
    0, 
    PALETTE_ENCODING_TYPE_CRGBPalette16
  );

  addDynamicPalette(
    PALETTELIST_DYNAMIC_CRGBPALETTE16__RANDOMISE_COLOURS_02_RANDOM_HUE_80TO100_SATURATIONS__ID, 
    nullptr, 
    0, 
    PALETTE_ENCODING_TYPE_CRGBPalette16
  );
  
  addDynamicPalette(
    PALETTELIST_DYNAMIC_CRGBPALETTE16__RANDOMISE_COLOURS_03_RANDOM_HUE_60TO100_SATURATIONS__ID, 
    nullptr, 
    0, 
    PALETTE_ENCODING_TYPE_CRGBPalette16
  );
  
  addDynamicPalette(
    PALETTELIST_DYNAMIC_CRGBPALETTE16__RANDOMISE_COLOURS_04_RANDOM_HUE_60TO85_SATURATIONS__ID, 
    nullptr, 
    0, 
    PALETTE_ENCODING_TYPE_CRGBPalette16
  );
  
  addDynamicPalette(
    PALETTELIST_DYNAMIC_CRGBPALETTE16__RANDOMISE_COLOURS_05_RANDOM_HUE_00TO100_SATURATIONS__ID, 
    nullptr, 
    0, 
    PALETTE_ENCODING_TYPE_CRGBPalette16
  );
  
  addDynamicPalette(
    PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID, 
    nullptr, 
    0, 
    PALETTE_ENCODING_TYPE_CRGBPalette16
  );
  
  addDynamicPalette(
    PALETTELIST_DYNAMIC__SOLAR_ELEVATION__WHITE_COLOUR_TEMPERATURE_01__ID, 
    nullptr, 
    0, 
    PALETTE_ENCODING_TYPE_CRGBPalette16
  );
  
  addDynamicPalette(
    PALETTELIST_DYNAMIC__SOLAR_ELEVATION__RGBCCT_PRIMARY_TO_SECONDARY_01__ID, 
    nullptr, 
    0, 
    PALETTE_ENCODING_TYPE_CRGBPalette16
  );
  
  addDynamicPalette(
    PALETTELIST_DYNAMIC__TIMEREACTIVE__RGBCCT_PRIMARY_TO_SECONDARY_WITH_SECONDS_IN_MINUTE_01__ID, 
    nullptr, 
    0, 
    PALETTE_ENCODING_TYPE_CRGBPalette16
  );

  /****************
   * CustomPalettes
   ****************/

    DEBUG_LINE_HERE;

  addCustomPalette(
    0, 
    PM_PALETTE_CUSTOM_PALETTE_DEFAULT_01__DATA, 
    sizeof(PM_PALETTE_CUSTOM_PALETTE_DEFAULT_01__DATA), 
    D_PALETTE_CUSTOM_PALETTE_DEFAULT_01_ENCODING
  );
    DEBUG_LINE_HERE;
  
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
  
  addCustomPalette(
    9, 
    PM_PALETTE_CUSTOM_PALETTE_DEFAULT_10__DATA, 
    sizeof(PM_PALETTE_CUSTOM_PALETTE_DEFAULT_10__DATA), 
    D_PALETTE_CUSTOM_PALETTE_DEFAULT_10_ENCODING
  );

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

  // palette_tmp.friendly_name_ctr = name;
  palette_tmp.encoding = {encoding};
  palette_tmp.palettelist_id = id;
  
  uint8_t encoded_colour_width  = GetEncodedColourWidth( palette_tmp.encoding );  
  palette_tmp.number_of_colours = length / encoded_colour_width;

  uint16_t id_adj = id - PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_01__ID; // First static ID

  if (id_adj < custom_palettes.size()) {
    static_palettes[id_adj] = palette_tmp;
  } else {
    static_palettes.push_back(palette_tmp);
  }

}

void mPalette::addDynamicPalette(uint16_t id, const uint8_t* data, const uint8_t length, uint16_t encoding)
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
  uint8_t id_adj = id - PALETTELIST_DYNAMIC_CRGBPALETTE16__RANDOMISE_COLOURS_01_RANDOM_HUE__ID;

  if (id_adj < dynamic_palettes.size()) {
    dynamic_palettes[id_adj] = palette_tmp;
  } else {
    dynamic_palettes.push_back(palette_tmp);
  }

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

}


/*********************************************************************************************************************************************************************************
 *********************************************************************************************************************************************************************************
 * SECTION: Main requests to get colours from palettes
 *********************************************************************************************************************************************************************************
 *********************************************************************************************************************************************************************************/


/** NEW METHOD: 
 * 
 * With my thought, I may need to switch the indexing of all my palettes to be 0-255, regardless of length. Hence,
 * 5 pixels in a palette would address as 0,50,100,150,200 (the 255/5), no longer as 0,1,2,3,4. This would make things more uniform when getting colours from any palette
 * * Note: The 255 indexing would be reguired to fix the issue with Effect "Colorful" where only the start of palettes are got, and hence short (e.g. 5) colour palettes appear static.
 * 
 * To test this, creating a json constructor that can produce each palette under its desired ways
 * Gets a single color from the currently selected palette.
 * @param id                  Palette Index, default (0) will be random from colour wheel
 * @param index_from_palette  [0..x] where x is total colours in palette. If `flag_mapping` is enabled, palette will be scaled out to be Y length >>> Palette Index (if mapping is true, the full palette will be _virtualSegmentLength long, if false, 255). Will wrap around automatically.
 * @param flag_mapping        if true, LED position in segment is considered for color
 * @param flag_wrap_hard_edge           FastLED palettes will usally wrap back to the start smoothly. Set false to get a hard edge
 * @param brightness_scale255 Value to scale the brightness of the returned color by. Default is 255. (no scaling)
 * @param encoded_index*      If format of palette includes encoded data (ie sun_value,R,G,B) then return by inserting into pointer position
 * @param flag_crgb_exact_colour If true, the index_from_palette is the exact pixel index, if false, the index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)
 */
RgbcctColor 
#ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
IRAM_ATTR 
#endif 
mPalette::GetColourFromPreloadedPaletteBuffer_2023(
  uint16_t palette_id,
  uint8_t* palette_buffer,
  uint16_t _pixel_position,    
  uint8_t* encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly //encoded value needs to be U32 since they can be 3 bytes wide
  uint8_t     flag_spanned_segment, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
  uint8_t     flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
  uint8_t     flag_crgb_exact_colour
){
  
  DEBUG_PIN4_SET(0);

  // ALOG_INF(PSTR("palette_id  = %d %d"), palette_id, palette_id);
  RgbcctColor colour = RgbcctColor(0);

  // DEBUG_LINE_HERE;

  #ifdef ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
  Serial.println(palette_id);
  DEBUG_LINE_HERE;
  #endif

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
    ((palette_id >= PALETTELIST_MODIFIABLE__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID)    && (palette_id < PALETTELIST_MODIFIABLE__RGBCCT_CRGBPALETTE16_PALETTES__LENGTH__ID))
  ){

    uint8_t segIdx = pCONT_lAni->_segment_index_primary;
    if(segIdx >= pCONT_lAni->segments.size() ){ segIdx = 0; } 

    CRGB fastled_col;
    
    uint16_t pixel_position_adjust = _pixel_position;    

    if(flag_crgb_exact_colour)
    {
      if(pixel_position_adjust < pSEGMENT_I(segIdx).palette_container->CRGB16Palette16_Palette.encoded_index.size())
      {
        uint8_t pixels_in_crgb16palette = pSEGMENT_I(segIdx).palette_container->CRGB16Palette16_Palette.encoded_index.size();
        pixel_position_adjust = _pixel_position % pixels_in_crgb16palette;
        // ALOG_INF(PSTR("Cpixel_position_adjust %d|%d==%d"), _pixel_position, pixel_position_adjust, pSEGMENT_I(segIdx).palette_container->CRGB16Palette16_Palette.encoded_index.size());
        pixel_position_adjust = pSEGMENT_I(segIdx).palette_container->CRGB16Palette16_Palette.encoded_index[pixel_position_adjust];
      }
    }
    
    if(flag_spanned_segment)
    {
      pixel_position_adjust = (_pixel_position*255)/(pCONT_lAni->_virtualSegmentLength -1);  // This scales out segment_index to segment_length as 0 to 255
    }

    if(flag_wrap_hard_edge==true) // If OFF, then cut end of palette off (rescale to 240)
    {
      pixel_position_adjust = scale8(pixel_position_adjust, 240); // cut off blend at palette "end", 240, or 15/16*255=>240/255, so drop last 16th (15 to wrapped 0) gradient of colour
    }

    // if(palette_id==72)
    //     ALOG_INF(PSTR("pixel_position_adjust %d|%d"), _pixel_position, pixel_position_adjust);
    
    fastled_col = ColorFromPalette( pSEGMENT_I(segIdx).palette_container->CRGB16Palette16_Palette.data, pixel_position_adjust, 255, NOBLEND);
  
    colour = RgbcctColor(fastled_col.r, fastled_col.g, fastled_col.b);

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

    uint8_t adjusted_id  = palette_id - PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID;
    const uint8_t* data  = PM_STATIC_SINGLE_COLOURS__DATA;
    uint8_t adjust_buf_i =  adjusted_id*3;
    colour = RgbcctColor(data[adjust_buf_i], data[adjust_buf_i+1], data[adjust_buf_i+2]);

  }

  /**************************************************************
   * 
   * PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR__IDS
   * 
  ***************************************************************/
  else
  if(
    (palette_id >= PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_01__ID) && (palette_id < PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID)
  ){  

    uint8_t adjusted_id = palette_id - PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_01__ID;
    uint8_t segIdx = pCONT_lAni->_segment_index_primary;

    if(segIdx >= pCONT_lAni->segments.size() ){ segIdx = 0; } 
    if(adjusted_id < RGBCCTCOLOURS_SIZE)
    {
      colour = pCONT_lAni->segments[segIdx].rgbcctcolors[adjusted_id];
    }
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

  DEBUG_PIN5_SET(0);
    colour = Get_Encoded_StaticPalette_Colour( // Making this easy with a single function call is probably okay, but everything inside should be self contained
        palette_adjusted_id,
        palette_buffer,
        _pixel_position,  
        encoded_value,
        flag_spanned_segment,
        flag_wrap_hard_edge,
        flag_crgb_exact_colour
      );
  DEBUG_PIN5_SET(1);

  } // end of my palettes

  /**************************************************************
   * 
   * PALETTELIST_DYNAMIC
   * 
  ***************************************************************/
  else
  if(
    ((palette_id >= PALETTELIST_DYNAMIC_CRGBPALETTE16__RANDOMISE_COLOURS_01_RANDOM_HUE__ID) && (palette_id < PALETTELIST_DYNAMIC__LENGTH__ID))
  ){  

    uint16_t palette_adjusted_id = palette_id - PALETTELIST_DYNAMIC_CRGBPALETTE16__RANDOMISE_COLOURS_01_RANDOM_HUE__ID; // adjust back into correct indexing

    colour = Get_Encoded_DynamicPalette_Colour(
        palette_adjusted_id,
        palette_buffer,
        _pixel_position,  
        encoded_value,
        flag_spanned_segment,
        flag_wrap_hard_edge,
        flag_crgb_exact_colour
      );

  } // end of my palettes


  /**************************************************************
   * 
   * Custom palettes: custom_palettes 
   * 
  ***************************************************************/
  else
  if(
    ((palette_id >= PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED)  && (palette_id < GetPaletteListLength())) // Custom palettes
  ){   
    
    uint16_t palette_adjusted_id = palette_id - PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED;        

    colour = SubGet_Encoded_CustomPalette_Colour(
      palette_adjusted_id,
      palette_buffer,
      _pixel_position,  
      encoded_value,
      flag_spanned_segment,
      flag_wrap_hard_edge,
      flag_crgb_exact_colour
    );
    
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

  // DEBUG_LINE_HERE;


  DEBUG_PIN4_SET(1);
  return colour;

}



/*********************************************************************************************************************************************************************************
 *********************************************************************************************************************************************************************************
 * SECTION: private/internal requests to get colours from palettes
 *********************************************************************************************************************************************************************************
 *********************************************************************************************************************************************************************************/


/**
 * @brief private function
 * 
 * Attempting to make addressing of all palettes into 255 range so it works easier with WLED
 * 0-255, regardless of length. Hence, 5 pixels in a palette would address as 0,50,100,150,200 (the 255/5), no longer as 0,1,2,3,4. This would make things more uniform when getting colours from any palette
 * 
 * 
 * Dont bother with subfunctions, keep it tight with this even if code has to repeat. Code cost without function call penalty
 * 
 * @param palette_id 
 * @param seg_i 
 * @return RgbcctColor 
 */
RgbcctColor  
#ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
IRAM_ATTR 
#endif 
mPalette::Get_Encoded_StaticPalette_Colour(
  uint16_t palette_adjusted_id,
  uint8_t* palette_buffer,
  uint16_t _pixel_position,  
  uint8_t* encoded_value, // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
  bool     flag_map_scaling, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
  bool     flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
  bool     flag_crgb_exact_colour,
  bool     flag_force_gradient
){
    
  // ALOG_ERR(PSTR("Get_Encoded_StaticPalette_Colour (%d,%d,%d,%d,%d,%d,%d)"), palette_adjusted_id, palette_buffer, _pixel_position, encoded_value, flag_spanned_segment, flag_wrap_hard_edge, flag_crgb_exact_colour, flag_force_gradient );

  uint8_t encoded_colour_width  = GetEncodedColourWidth(static_palettes[palette_adjusted_id].encoding);   
  uint8_t colours_in_palette = static_palettes[palette_adjusted_id].data.size()/encoded_colour_width;
  palette_buffer = &static_palettes[palette_adjusted_id].data[0];
  PALETTE_ENCODING_DATA encoding = static_palettes[palette_adjusted_id].encoding;

  return Get_Encoded_Palette_Colour(
    palette_buffer,
    _pixel_position,
    encoded_colour_width,
    colours_in_palette,
    encoding,
    encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
    flag_map_scaling, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
    flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
    flag_crgb_exact_colour,
    flag_force_gradient
  );

}



/**
 * @brief private function
 * 
 * Attempting to make addressing of all palettes into 255 range so it works easier with WLED
 * 0-255, regardless of length. Hence, 5 pixels in a palette would address as 0,50,100,150,200 (the 255/5), no longer as 0,1,2,3,4. This would make things more uniform when getting colours from any palette
 * 
 * 
 * Dont bother with subfunctions, keep it tight with this even if code has to repeat. Code cost without function call penalty
 * 
 * @param palette_id 
 * @param seg_i 
 * @return RgbcctColor 
 */
RgbcctColor mPalette::Get_Encoded_DynamicPalette_Colour(
  uint16_t palette_adjusted_id,
  uint8_t* palette_buffer,
  uint16_t _pixel_position,  
  uint8_t* encoded_value, // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
  bool     flag_spanned_segment, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
  bool     flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
  bool     flag_crgb_exact_colour,
  bool     flag_force_gradient
){

// Here, I likely want to work conversions, then call the other methods that are needed.
// For simplicity, the required colours will be requested from the static options that are in progmem.

// ALOG_INF(PSTR("HERE %d"), palette_adjusted_id);

/*
// Should I make it here that one of the matlab colourmaps can be loaded into here, and then some the float vector is actually saved with the palettes
// rather than the strip code? oh!
// 1) matlab palettes should be called through this, 
// 2) if the vector has been set, then it will enable the palette to be rescaled? though, this may not enable the temperature celius, or really its just added by command.
// This would limit scalled palettes to be only 1, which is probably enough. so lets just use a vector that is cleared when not using matlab palettes. 
// Should any palette be allowed to use this rescale? may be too complex and best to stick with matlab ones, otherwise the palette creation tool will be used
// and this is just for exact mapping from a value into colourmap.
// maybe it makes more sense to have another GetColourFromPalette that does the colour mapping, so it really is another functionality.


GetColourFromPalette_WithColourMapScale(normal colour map, pass in vector float list))


*/

  switch(palette_adjusted_id + PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID) //switch back to palette_NOT_adjusted_id
  { 
    default:

ALOG_ERR(PSTR("Get_Encoded_DynamicPalette_Colour: The other randomise palettes should be loaded here instead of the other function"));
      
      

    case PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID:{

      float azimuth = pCONT_solar->solar_position.azimuth;

      float progress = mSupport::mapfloat(azimuth, 0,360, 0.0f, 1.0f);
      
      RgbcctColor colour1 = pSEGMENT_I(0).rgbcctcolors[0];
      RgbcctColor colour2 = pSEGMENT_I(0).rgbcctcolors[1];      

      Serial.println(azimuth);
      Serial.println(progress);

      return RgbcctColor::LinearBlend(colour1, colour2, progress);

    }
    break;
    
    /**
     * @brief Only around the +-10% of horizon should the transition occur. Anything above/below should remain at fully cold/warm white
     * 
     */
    case PALETTELIST_DYNAMIC__SOLAR_ELEVATION__WHITE_COLOUR_TEMPERATURE_01__ID:
    {
    
      float elevation = pCONT_solar->GetElevation();

      RgbcctColor colour_out = RgbcctColor(0);  

      float elevation_transition_degrees = 10; // degrees above/below horizon to transition between warm/cold white


      if(fabs(elevation) > elevation_transition_degrees) // get amplitude of elevation from horizon
      {

        if(elevation >= 0){ // daytime
          colour_out.setCCT(CCT_MIN_DEFAULT); // Cold White
          colour_out.setRGB(255,255,255);
          // ALOG_INF(PSTR("++++++ elevation %d"), (int)(elevation*100) );
        }else{ //nighttime
          colour_out.setCCT(CCT_MAX_DEFAULT); // Warm White
          colour_out.setRGB(0xFF,0x52,0x18);
          // ALOG_INF(PSTR("------ elevation %d"), (int)(elevation*100) );
        }
      }else{ // transitioning, dusk/dawn
        float progress = mSupport::mapfloat(
                                              elevation, 
                                              elevation_transition_degrees,    -1*elevation_transition_degrees, 
                                              0.0f,                            1.0f
                                            );

        RgbcctColor colour1 = RgbcctColor(0);
        colour1.setCCT(CCT_MIN_DEFAULT); // Cold White
        colour1.setRGB(255,255,255); // Cold White with RGB (Full)
        // colour1.setRGB(255,0,0); // Debugging

        RgbcctColor colour2 = RgbcctColor(0);
        colour2.setCCT(CCT_MAX_DEFAULT); // Warm White
        colour2.setRGB(0xFF,0x52,0x18);  // Warm White with RGB (White with reduced G,B)
        // colour2.setRGB(0,0,255); // Debugging

        colour_out = RgbcctColor::LinearBlend(colour1, colour2, progress);

      }
      Serial.println(elevation);
      colour_out.debug_print("colour_out");

      return colour_out;

       

      // Serial.println(elevation);
      // Serial.println(progress);
      
    }
    break;
    case PALETTELIST_DYNAMIC__SOLAR_ELEVATION__RGBCCT_PRIMARY_TO_SECONDARY_01__ID:
    {
        // 0 = peak day   ratio 1
        // 1 = sunset     ratio 0
        // 2 = peak night ratio 1



      // float elevation = pCONT_solar->solar_position.elevation;

      RgbcctColor colour1 = pSEGMENT_I(0).rgbcctcolors[0];
      RgbcctColor colour2 = pSEGMENT_I(0).rgbcctcolors[1]; 
      // RgbcctColor colour_out = pSEGMENT_I(0).rgbcctcolors[1];     

      // if(elevation > 0) //daytime
      // {
      //   colour1 = pSEGMENT_I(0).rgbcctcolors[0];
      //   colour2 = pSEGMENT_I(0).rgbcctcolors[1];  
      //   float progress = mSupport::mapfloat(elevation, 0, pCONT_solar->solar_position.elevation_max, 1.0f, 0.0f);
      //   colour_out = RgbcctColor::LinearBlend(colour1, colour2, progress);
      // }else{ //nighttime      
      //   colour1 = pSEGMENT_I(0).rgbcctcolors[1];
      //   colour2 = pSEGMENT_I(0).rgbcctcolors[2];  
      //   float progress = mSupport::mapfloat(elevation, 0, pCONT_solar->solar_position.elevation_min, 0.0f, 1.0f);
      //   colour_out = RgbcctColor::LinearBlend(colour1, colour2, progress);
      // }

      // return colour_out;


      
      float elevation = pCONT_solar->solar_position.elevation;

      RgbcctColor colour_out = RgbcctColor(0);  

      float elevation_transition_degrees = 10; // degrees above/below horizon to transition between warm/cold white


      if(fabs(elevation) > elevation_transition_degrees) // get amplitude of elevation from horizon
      {

        if(elevation >= 0){ // daytime
          colour_out = colour1;
          // ALOG_INF(PSTR("++++++ elevation %d"), (int)(elevation*100) );
        }else{ //nighttime
          colour_out = colour2;
          // ALOG_INF(PSTR("------ elevation %d"), (int)(elevation*100) );
        }
      }else{ // transitioning, dusk/dawn
        float progress = mSupport::mapfloat(
                                              elevation, 
                                              elevation_transition_degrees,    -1*elevation_transition_degrees, 
                                              0.0f,                            1.0f
                                            );

        colour_out = RgbcctColor::LinearBlend(colour1, colour2, progress);

      }
      Serial.println(elevation);
      colour_out.debug_print("colour_out");

      return colour_out;

      // Serial.println(elevation);
      // Serial.println(progress);
      
    }
    break;
    case PALETTELIST_DYNAMIC__TIMEREACTIVE__RGBCCT_PRIMARY_TO_SECONDARY_WITH_SECONDS_IN_MINUTE_01__ID:
    {

      float progress = mSupport::mapfloat(pCONT_time->RtcTime.second, 0,59, 0.0f, 1.0f);
      Serial.println(progress);

      RgbcctColor colour1 = pSEGMENT_I(0).rgbcctcolors[0];
      RgbcctColor colour2 = pSEGMENT_I(0).rgbcctcolors[1];      

      return RgbcctColor::LinearBlend(colour1, colour2, progress);

    }
    break;
  }

  return RgbcctColor(10);

}




/**
 * @brief private function
 * 
 * Attempting to make addressing of all palettes into 255 range so it works easier with WLED
 * 0-255, regardless of length. Hence, 5 pixels in a palette would address as 0,50,100,150,200 (the 255/5), no longer as 0,1,2,3,4. This would make things more uniform when getting colours from any palette
 * 
 * 
 * Dont bother with subfunctions, keep it tight with this even if code has to repeat. Code cost without function call penalty
 * 
 * @param palette_id 
 * @param seg_i 
 * @return RgbcctColor 
 */
RgbcctColor mPalette::SubGet_Encoded_CustomPalette_Colour(
  uint16_t palette_adjusted_id,
  uint8_t* palette_buffer,
  uint16_t _pixel_position,  
  uint8_t* encoded_value, // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
  bool     flag_spanned_segment, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
  bool     flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
  bool     flag_crgb_exact_colour,
  bool     flag_force_gradient
){

  uint8_t encoded_colour_width  = GetEncodedColourWidth(custom_palettes[palette_adjusted_id].encoding);   
  uint8_t colours_in_palette = custom_palettes[palette_adjusted_id].data.size()/encoded_colour_width;
  palette_buffer = &custom_palettes[palette_adjusted_id].data[0];
  PALETTE_ENCODING_DATA encoding = custom_palettes[palette_adjusted_id].encoding;

  return Get_Encoded_Palette_Colour(
    palette_buffer,
    _pixel_position,
    encoded_colour_width,
    colours_in_palette,
    encoding,
    encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
    flag_spanned_segment, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
    flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
    flag_crgb_exact_colour,
    flag_force_gradient
  );

}


/*********************************************************************************************************************************************************************************
 *********************************************************************************************************************************************************************************
 * SECTION: Main requests to get/set name/id of palettes
 *********************************************************************************************************************************************************************************
 *********************************************************************************************************************************************************************************/


int16_t mPalette::Get_Static_PaletteIDbyName(const char* c)
{

  if(*c=='\0'){
    return -1;
  }

  DEBUG_LINE_HERE

  char buffer[100] = {0};
  int16_t index_found = -1;
  int16_t id = -1;


  for(
    uint8_t ii=0;
            ii<(PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID-PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_01__ID);
            ii++
  ){    
    ALOG_DBM( PSTR("s> %d %s \"%S\""), ii, c, PM_MODIFIABLE__RGBCCT_SOLID_COLOUR__NAMES_CTR ); 
    if((id=mSupport::GetCommandID16_P(c, PM_MODIFIABLE__RGBCCT_SOLID_COLOUR__NAMES_CTR))>=0)
    {
      ALOG_INF( PSTR("MATCH \"%s\" %d %d"), c, ii, id ); 
      return id+PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_01__ID;            
    }
  }



  /**************************************************************
   * 
   * PALETTELIST_DYNAMIC_CRGBPALETTE16_USER__LENGTH__ID
   * 
  ***************************************************************/


  for(
    uint8_t ii=0;
            ii<(PALETTELIST_MODIFIABLE__RGBCCT_CRGBPALETTE16_PALETTES__LENGTH__ID-PALETTELIST_MODIFIABLE__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID);
            ii++
  ){    
    ALOG_DBM( PSTR("s> %d %s \"%S\""), ii, c, PM_MODIFIABLE__RGBCCT_CRGBPALETTE16_PALETTES_NAMES_CTR ); 
    if((id=mSupport::GetCommandID16_P(c, PM_MODIFIABLE__RGBCCT_CRGBPALETTE16_PALETTES_NAMES_CTR))>=0)
    {
      ALOG_INF( PSTR("MATCH \"%s\" %d %d"), c, ii, id ); 
      return id+PALETTELIST_MODIFIABLE__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID;            
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
  for( // loops relative to exact palette id
    uint8_t ii=0;
            ii<(PALETTELIST_STATIC_LENGTH__ID - PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID);
            ii++
  ){
    ALOG_DBM( PSTR("s> %d %s \"%S\""), ii, c, PM_STATIC__PALETTES_NAMES_CTR ); 
    if((id=mSupport::GetCommandID16_P(c, PM_STATIC__PALETTES_NAMES_CTR))>=0)
    {
      ALOG_INF( PSTR("MATCH \"%s\" %d %d"), c, ii, id ); 
      return id+PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID;            
    }
  }

  /**************************************************************
   * 
   * PALETTELIST_DYNAMIC__COLOUR__IDS
   * 
  ***************************************************************/
  for(
    uint8_t ii=0;
            ii<(PALETTELIST_DYNAMIC__LENGTH__ID-PALETTELIST_DYNAMIC_CRGBPALETTE16__RANDOMISE_COLOURS_01_RANDOM_HUE__ID);
            ii++
  ){    
    ALOG_DBM( PSTR("s> %d %s \"%S\""), ii, c, PM_DYNAMIC_PALETTES_NAMES_CTR ); 
    if((id=mSupport::GetCommandID16_P(c, PM_DYNAMIC_PALETTES_NAMES_CTR))>=0)
    {
      ALOG_INF( PSTR("MATCH \"%s\" %d %d"), c, ii, id ); 
      return id+PALETTELIST_DYNAMIC_CRGBPALETTE16__RANDOMISE_COLOURS_01_RANDOM_HUE__ID;            
    }
  }



  /**************************************************************
   * 
   * PALETTELIST_VARIABLE_GENERIC__IDS - undefined user names, default naming
   * 
  ***************************************************************/
  for( // loops relative to 0
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

  /**************************************************************
   * 
   * Final check, palette id was given as string number
   * 
  ***************************************************************/
  // uint8_t found_index = (!strlen(c)) ? 0 : atoi(c);
  // if(WithinLimits(found_index, (uint8_t)0, (uint8_t)PALETTELIST_STATIC_LENGTH__ID)){
  //   return found_index;
  // }

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
    ((palette_id >= PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_01__ID) && (palette_id < PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID))
  ){  
    uint16_t adjusted_id = palette_id - PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_01__ID;
    mSupport::GetTextIndexed_P(buffer, buflen, adjusted_id, PM_MODIFIABLE__RGBCCT_SOLID_COLOUR__NAMES_CTR);   
  }


  /**************************************************************
   * 
   * PALETTELIST_SEGMENTS_STORED_FIXED_CRGBPALETTE16_PALETTES__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= PALETTELIST_MODIFIABLE__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID) && (palette_id < PALETTELIST_MODIFIABLE__RGBCCT_CRGBPALETTE16_PALETTES__LENGTH__ID))
  ){  
    uint16_t adjusted_id = palette_id - PALETTELIST_MODIFIABLE__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID;
    mSupport::GetTextIndexed_P(buffer, buflen, adjusted_id, PM_MODIFIABLE__RGBCCT_CRGBPALETTE16_PALETTES_NAMES_CTR);   
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
    ALOG_DBG( PSTR("BName id%d|a%d \"%s\""), palette_id,palette_id_adj, buffer );
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
  }

  /**************************************************************
   * 
   * PALETTELIST_DYNAMIC_
   * 
  ***************************************************************/
  if(
    ((palette_id >= PALETTELIST_DYNAMIC_CRGBPALETTE16__RANDOMISE_COLOURS_01_RANDOM_HUE__ID) && (palette_id < PALETTELIST_DYNAMIC__LENGTH__ID))
  ){           
    uint16_t adjusted_id = palette_id - PALETTELIST_DYNAMIC_CRGBPALETTE16__RANDOMISE_COLOURS_01_RANDOM_HUE__ID;
    mSupport::GetTextIndexed_P(buffer, buflen, adjusted_id, PM_DYNAMIC_PALETTES_NAMES_CTR);
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
    ALOG_DBG( PSTR("BName id%d|a%d \"%s\""), palette_id,palette_id_adj, buffer );
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
   * PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID
   * 
  ***************************************************************/
  if(
    ((palette_id >= PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_01__ID)    && (palette_id < PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID))
  ){  
    uint16_t adjusted_id = palette_id - PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_01__ID;
    sprintf_P(buffer, PSTR(D_DEFAULT_DYNAMIC_PALETTE_NAMES__VARIABLE_RGBCCT__NAME_CTR), adjusted_id); // names are 1-10
  }

  /**************************************************************
   * 
   * PALETTELIST_VARIABLE_GENERIC__IDS - undefined user names, default naming
   * 
  ***************************************************************/
  // If stored in RAM, and user editable, then it resides in DeviceNameBuffer and not within mPalette class


  return buffer;

}



/*********************************************************************************************************************************************************************************
 *********************************************************************************************************************************************************************************
 * SECTION: Internal helpers
 *********************************************************************************************************************************************************************************
 *********************************************************************************************************************************************************************************/


/**
 * Refresh value stored in palette, and return new value
 * */
uint8_t mPalette::GetColourMapSizeByPaletteID(uint8_t palette_id){
  
  uint8_t new_size = 1; // assumed 1 at least
  
  if((palette_id>=PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_01__ID)&&(palette_id<PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID)){

    mPalette::PALETTE_DATA *ptr_tmp = &static_palettes[palette_id];

    new_size = 5; // only 1*5

    // ptr_tmp
    ALOG_ERR(PSTR("Removed code, needs refactored"));
    
    // ptr_tmp->data_length = new_size;

  }else
  if((palette_id>=PALETTELIST_LENGTH_OF_STATIC_IDS)&&(palette_id<GetPaletteListLength())){

    // PALETTE *ptr_tmp = &static_palettes[palette_id];
    
    new_size = 5; // ASSUME 10 RIGHT NOW  
    ALOG_ERR(PSTR("ERROR ON SIZE GET £$%"));
    // ptr_tmp->data_length; // currently refreshed when init is triggered, about this may need changing, leaving unchanged for now
  }
  else
  {

    new_size = 1;//ptr_tmp->data_length;
    ALOG_INF(PSTR("GetColourMapSizeByPaletteID ELSE  ERORRRRRRRRRRRRRRRRRRRRRRR"));
    DEBUG_LINE_HERE;

  }

  return new_size;
}




uint8_t mPalette::GetColoursInCRGB16Palette(uint16_t palette_id)
{

  if(
    ((palette_id >= PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID) && (palette_id < PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID))
  ){  
    uint16_t palette_id_adj = palette_id - PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID;
    // mSupport::GetTextIndexed_P(buffer, buflen, palette_id_adj, PM_STATIC_CRGBPALETTE16_NAMES_CTR);   
    // ALOG_DBG( PSTR("BName id%d|a%d \"%s\""), palette_id,palette_id_adj, buffer );

    uint8_t colour_count = pSEGMENT_I(0).palette_container->CRGB16Palette16_Palette.encoded_index.size();

    // ALOG_INF(PSTR("colour_count=%d, pal%d"), colour_count, palette_id);

    return colour_count;

  }


  if(
    ((palette_id >= PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID) && (palette_id < PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID))
  ){  
    uint16_t palette_id_adj = palette_id - PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID;
    // mSupport::GetTextIndexed_P(buffer, buflen, palette_id_adj, PM_STATIC_CRGBPALETTE16_NAMES_CTR);   
    // ALOG_DBG( PSTR("BName id%d|a%d \"%s\""), palette_id,palette_id_adj, buffer );

    return pSEGMENT_I(0).palette_container->CRGB16Palette16_Palette.encoded_index.size();

  }

  return 16;

}







// uint16_t 
// #ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
// IRAM_ATTR 
// #endif 
// mPalette::GetNumberOfColoursInPalette(uint16_t palette_id, uint8_t pixel_width_contrained_limit)
// {
//   PALETTE_DATA pal = static_palettes[palette_id];
//   return GetNumberOfColoursFromEncoded(pal.encoding, pal.data.size());
// }


// uint16_t  
// #ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
// IRAM_ATTR 
// #endif 
// mPalette::GetNumberOfColoursFromEncoded(PALETTE_ENCODING_DATA encoding, uint8_t data_in_palette)
// {
//   uint16_t encoded_colour_width = GetEncodedColourWidth(encoding);
//   return data_in_palette/encoded_colour_width;   
// }


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
    ((palette_id >= PALETTELIST_MODIFIABLE__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID)    && (palette_id < PALETTELIST_MODIFIABLE__RGBCCT_CRGBPALETTE16_PALETTES__LENGTH__ID))
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
    ((palette_id >= PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID) && (palette_id < PALETTELIST_DYNAMIC__LENGTH__ID))
  ){  
    
    uint16_t palette_adjusted_id = palette_id - PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID; // adjust back into correct indexing
    return dynamic_palettes[palette_adjusted_id].number_of_colours;  

  } // end of my palettes


  /**************************************************************
   * 
   * PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR__IDS
   * 
  ***************************************************************/
  else
  if(
    (palette_id >= PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_01__ID) && (palette_id < PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID)
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


}



/**
 * @brief private function
 * 
 * By having this as a subfunction, it should enable iterative multiple calls function the main GetColour
 * The modulo inside is mostly to stop overruns, with proper indexing it should not be needed
 * 
 * @param palette_id 
 * @param seg_i 
 * @return RgbcctColor 
 */
RgbcctColor mPalette::Get_Encoded_Colour_ReadBuffer_Fast(
  uint8_t* palette_buffer,
  uint16_t pixel_position,  
  uint8_t* return_encoded_value,
  PALETTE_ENCODING_DATA encoding,
  uint8_t encoded_colour_width
){

  uint16_t index_relative = 0; // get expected pixel position

  #ifdef ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
  Serial.println(encoding.data, BIN);
  Serial.println(encoding.encoded_as_hsb_ids);
  Serial.println(encoding.index_gradient);
  #endif // ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
  

  if( // Switch to bit masking
    (encoding.red_enabled)||
    (encoding.green_enabled)||
    (encoding.blue_enabled)||
    (encoding.white_cold_enabled)||
    (encoding.white_warm_enabled)
  ){
    
    // ALOG_INF(PSTR("enabled %d,%d,%d,%d,%d"), encoding.red_enabled, encoding.green_enabled, encoding.blue_enabled, encoding.white_cold_enabled, encoding.white_warm_enabled);
    // ALOG_INF(PSTR("palette_elements %d"), palette_elements[0]);   
    // ALOG_INF(PSTR("p = %d, r = %d, v = %d|%d, w=%d"), pixel_position, index_relative, palette_elements[index_relative], palette_elements[index_relative+1],colour_width);

    // Get Start of Colour Information by adjusting for indexing
    index_relative = pixel_position*encoded_colour_width; // get expected pixel position
      
    // ALOG_INF(PSTR("index_relativeA=%d"),index_relative);

    if(encoding.index_gradient)
    {
      // ALOG_INF(PSTR("index_relativeB=%d"),index_relative);
      if(return_encoded_value != nullptr){
        *return_encoded_value = palette_buffer[index_relative];
      }
      // Set the index to move beyond the indexing information
      index_relative++;
    }
    
    return RgbcctColor(
      encoding.red_enabled         ? palette_buffer[index_relative  ] : 0,
      encoding.green_enabled       ? palette_buffer[index_relative+1] : 0,
      encoding.blue_enabled        ? palette_buffer[index_relative+2] : 0,
      encoding.white_cold_enabled  ? palette_buffer[index_relative+3] : 0,
      encoding.white_warm_enabled  ? palette_buffer[index_relative+4] : 0
    );

  }

  return RgbcctColor(0);

}








/**
 * @brief private function
 * 
 * Attempting to make addressing of all palettes into 255 range so it works easier with WLED
 * 0-255, regardless of length. Hence, 5 pixels in a palette would address as 0,50,100,150,200 (the 255/5), no longer as 0,1,2,3,4. This would make things more uniform when getting colours from any palette
 * 
 * 
 * Dont bother with subfunctions, keep it tight with this even if code has to repeat. Code cost without function call penalty
 * 
 * @param palette_id 
 * @param seg_i 
 * @return RgbcctColor 
 */
RgbcctColor  
#ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
IRAM_ATTR 
#endif 
mPalette::Get_Encoded_Palette_Colour(
  uint8_t* palette_buffer,
  uint16_t _pixel_position, 
  uint8_t encoded_colour_width,
  uint8_t colours_in_palette,
  PALETTE_ENCODING_DATA encoding,
  uint8_t* encoded_value, // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
  bool     flag_spanned_segment, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
  bool     flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
  bool     flag_crgb_exact_colour,
  bool     flag_force_gradient
){
    
  // ALOG_ERR(PSTR("Get_Encoded_StaticPalette_Colour (%d,%d,%d,%d,%d,%d,%d)"), palette_adjusted_id, palette_buffer, _pixel_position, encoded_value, flag_spanned_segment, flag_wrap_hard_edge, flag_crgb_exact_colour, flag_force_gradient );


// The issue now is that "SPAN" should not be forcing my palettes to show across the thing, but react based on their type. If they are gradient, then span, if they are not, then should still be sequential

  /**
   * @brief 
   * 
   * 
   * I think I need a new flag, "flag_spanned_segment" is confusing. 
   * Right now it means to scale the incoming index, which is the SEGLEN, into the 255 palette range.
   * 
   * But, what happens when the incoming data is the same, but the effect wants to force a normal pallete to be gradient.
   * I maybe need a "flag_force_gradient"? That way an effect can ask for it? Though in theory I do not want this, the effect
   * should want the gradient/discrete palette to be the selection of the user, and not be forcing it. No palette forced conversion should happen other than to make sure
   * the effect is visually correct. So no "FORCE it" should happen.
   * 
   */

  RgbcctColor colour;
  // if(palette_adjusted_id>static_palettes.size())
  // {
  //   ALOG_ERR(PSTR("PALETTE EXCEEDS VECTOR SIZE"));
  //   return colour;
  // }

  // PALETTE_DATA *ptr = &static_palettes[palette_adjusted_id];  
  // uint8_t pixels_in_map = GetNumberOfColoursInPalette(palette_adjusted_id);  
  // uint8_t colour_width  = GetEncodedColourWidth(ptr->encoding); 


  // uint8_t colour_width  = GetEncodedColourWidth(static_palettes[palette_adjusted_id].encoding);   
  // uint8_t pixels_in_map = static_palettes[palette_adjusted_id].data.size()/colour_width;
  // palette_buffer = &static_palettes[palette_adjusted_id].data[0];
  // PALETTE_ENCODING_DATA encoding = static_palettes[palette_adjusted_id].encoding;

  uint16_t pixel_position_adjust = _pixel_position;

  /******************************************************************************************************************************************************
   * *******************************************************************************************************************************************************************************
   * 
   * DISCRETE SEQUENCE, is_basic_sequence_palette_with_increasing_indexing
   * 
   * ******************************************************************************************************************************************************
   * ******************************************************************************************************************************************************************************* */
  bool is_forced_to_get_discrete = flag_crgb_exact_colour;
  bool is_not_gradient = (encoding.index_gradient == false);
  /**
   * @brief NOTE
   * 
   * For original sequential palettes that are not gradients, the incoming index must remain always "increasing integer iterator"  (e.g., 0,1,2,3,4,5..LEN).
   * Therefore, to get the palette colour, the modulo can simply be used to get the sequence of colours.
   * If segment_span is on, then the incoming index is scaled into 255 range, and may not be a "increasing integer iterator" and must first be converted back.
   *  - Should this ever happen? If spanned is set, then is a sequence even the desired effect? or, should it just be displayed as 0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3...? So ranges across it. Likely dont want this.
   * 
   * 
   */
  bool is_basic_sequence_palette_with_increasing_indexing = is_forced_to_get_discrete || is_not_gradient && !flag_spanned_segment;

  // ALOG_INF(PSTR("flags %d %d %d"), is_forced_to_get_discrete, is_not_gradient, is_basic_sequence_palette_with_increasing_indexing);

  if(is_basic_sequence_palette_with_increasing_indexing) // forced, or originally normal discrete
  {
    // 0-SEGLEN would need to become iterative of the palette    0-100    what if 0-100*pixels  so 0-500, then modulo by 100
    // o,1,2,3,4, 0,1,2,3,4, 0,1,2,3,4
    // but because of 255 range, these values should be scaled into 255
    //0-100 into 0-25500,    so 25500%100 would be 

    // Serial.println("Checking here it is not a gradient, if it is not, then the incoming 0-SEGLEN needs to still be treating to get the sequential of my palettes");
    // Serial.println("spanned segment needs better definition, it is confusing. It should be OFF most of the time as default. The incoming 0-SEGLEN should be assumed to be scaled to 255");
    
    // pixel_position_adjust = (_pixel_position*255)/(pCONT_lAni->_virtualSegmentLength -1); 
    // pixel_position_adjust = map(pixel_position_adjust, 0,255, 0,pCONT_lAni->SEGLEN);//  pixels_in_map; // convert incoming pixels into repeating 0-15 numbers.

    if(flag_spanned_segment && !is_forced_to_get_discrete) // if incoming index is SEGWIDTH, then it needs converted first. Or, does it need converted back?
    {
      // pixel_position_adjust = (_pixel_position*255)/(pCONT_lAni->_virtualSegmentLength -1);  // This scales out segment_index to segment_length as 0 to 255
      // pixel_position_adjust = scale8(pixel_position_adjust, pixels_in_map); //gradient type when exact value is needed needs scaled into full range
    }
    
    uint16_t before = pixel_position_adjust;
    // if(not gradient)
    pixel_position_adjust %= colours_in_palette; // convert incoming pixels into repeating 0-15 numbers.
    // else // if gradient, then this same thing should happen but scale into 255 range
    // ALOG_INF(PSTR("pixel_position_adjust %d/%d"), before, pixel_position_adjust);
    colour = Get_Encoded_Colour_ReadBuffer_Fast(
      palette_buffer,
      pixel_position_adjust,  
      encoded_value,
      encoding,
      encoded_colour_width
    );
    
    return colour;

  }

  /******************************************************************************************************************************************************
   * *******************************************************************************************************************************************************************************
   * 
   * Now, just handle the GRADIENTS
   * 
   * ******************************************************************************************************************************************************
   * ******************************************************************************************************************************************************************************* */
  if(encoding.index_gradient || flag_force_gradient) // if spanned, inside checks for gradient handling
  {
    
    /**
     * @brief Convert incoming indes (0-SEGLEN) back into 255 range (0-255)
     * If it is mine, no matter what, convert into 255 range
     **/
    if(flag_spanned_segment)
    {
      pixel_position_adjust = (_pixel_position*255)/(pCONT_lAni->_virtualSegmentLength -1);  // This scales out segment_index to segment_length as 0 to 255
    }

      
      // ALOG_INF(PSTR("index_gradient"));
      
    // uint8_t pixel_position_adjust = _pixel_position;

    //   if (flag_map_scaling) pixel_position_adjust = (_pixel_position*255)/(pCONT_lAni->_virtualSegmentLength -1);  // This scales out segment_index to segment_length as 0 to 255
    // if (!flag_wrap_hard_edge){
    //   // uint8_t pixel_end_boundary = map(pixels_in_map-1, 0,pixels_in_map-1, 0,255); // depends on the number of pixels
    //   pixel_position_adjust = scale8(pixel_position_adjust, 240); //cut off blend at palette "end", 240, or 15/16*255=>240/255, so drop last 16th (15 to wrapped 0) gradient of colour
    // }
      // ALOG_INF(PSTR("_pixel_position %d/%d/%d"), _pixel_position, pCONT_lAni->_virtualSegmentLength, pixel_position_adjust);

      
    /**
     *  Last step, convert back
     * **/
    // pixel_position_adjust = map(pixel_position_adjust, 0,255, 0,pixels_in_map-1);
    // pixel_position_adjust = constrain(pixel_position_adjust, 0,pixels_in_map-1); // to protect again out of bounds



      // ALOG_INF(PSTR("here2 pixel_position_adjust %d"), pixel_position_adjust);

    uint8_t edge_size = colours_in_palette;
    uint8_t lower_limit = edge_size/2;
    uint8_t upper_limit = 255-(edge_size/2);

    // uint8_t edge_size = 1;//255/pixels_in_map;
    // uint8_t lower_limit = 0;//edge_size/2;
    // uint8_t upper_limit = 255;//255-(edge_size/2);

    bool palette_contains_gradient_indexes = false;

    std::vector<uint8_t> gradient_palettes;
    uint8_t encoded_value2 = 0;

    /**
     * @brief Only "index_gradient" index is a gradient
     * 
     */
    if(encoding.index_gradient)
    {
      for(uint8_t pix_i=0; pix_i<colours_in_palette; pix_i++)
      {
        Get_Encoded_Colour_ReadBuffer_Fast(
            palette_buffer,
            pix_i,  
            &encoded_value2,
            encoding,
            encoded_colour_width
          );
        gradient_palettes.push_back(encoded_value2);
      }
      // Serial.println("Aencoded_value2"); for(uint8_t v=0;v<gradient_palettes.size();v++){ Serial.printf("%d,",gradient_palettes[v]); }
    }else{
      for(uint8_t pix_i=0; pix_i<colours_in_palette; pix_i++)
      {
        gradient_palettes.push_back(map(pix_i, 0,colours_in_palette-1, lower_limit,upper_limit));
      }
      // Serial.print("B!!!!!!!!!!!!!!!!!!!!!!!encoded_value2==="); for(uint8_t v=0;v<gradient_palettes.size();v++){ Serial.printf("%d,%s",gradient_palettes[v], v<gradient_palettes.size()-1?"":"\n\r"); }
    }
    
    // ALOG_INF(PSTR("###################################_pixel_position pixel_position_adjust %d %d"), _pixel_position), pixel_position_adjust;
    // ALOG_INF(PSTR("pixel_position_adjust %d"), pixel_position_adjust);


    // Search for lower boundary
    uint8_t desired_pixel_scaled = pixel_position_adjust;
    uint8_t lower_boundary_i = 0;
    uint8_t upper_boundary_i = 0;
    uint8_t lower_boundary_v = 0;
    uint8_t upper_boundary_v = 0;
    bool lower_boundary_found = 0;

    float progress = 0;

    /**
     * @brief 
     * Uses current scaled gradient value, and searches the palettes scaled boundaries to get the region of palette to scale between
     * >>Example<<
     * Palette of 5 colours, index 0,  1,   2,   3,   4
     * Scaled to 255       , grad  0, 64, 128, 191, 255
     * Desired scaled value (200)                  ^        191-255 should be scaled into 0.0f to 1.0f, and the colour between these as LinearBlend(colour_lower, colour_upper, ratio)    
     * 
     * Since the for loop checks the current and next index in the same loop, it will check all but the final/end boundary
     * Therefore, this will always be the end/255 value, and should be checked first. Only if not the final, should the loop be used to search for the boundary.
     * Based on fastled, there may be a divisor was to achieve this?? Note that above I am creating the range, then searching with it. Surely I simply need to assume the index and then create its equiavlent mapped scaled value just once?
     * Leave as optimise problem.
     * 
     * If pixels of 5 is the palette
     * and 150 (as 250 range) is asked for as gradient.
     * map(150, 0,255, 0,5-1) would give lower boundary?     
     * 
     */
    if(desired_pixel_scaled < lower_limit)   
    {
      // ALOG_INF(PSTR("(desired_pixel_scaled < lower_limit) "));
      // Stick with first colour
      lower_boundary_i = 0;
      upper_boundary_i = 1;
          lower_boundary_v = gradient_palettes[lower_boundary_i];
          upper_boundary_v = gradient_palettes[upper_boundary_i];
      progress = 0;
    }else
    if(desired_pixel_scaled > upper_limit)
    {
      // ALOG_INF(PSTR("(desired_pixel_scaled > lower_limit) "));
      // Stick with last colour
      lower_boundary_i = gradient_palettes.size()-1;
      upper_boundary_i = gradient_palettes.size(); //ignored
          lower_boundary_v = gradient_palettes[lower_boundary_i];
          upper_boundary_v = gradient_palettes[upper_boundary_i];
      progress = 0;
    }
    else // Search
    {
      // ALOG_INF(PSTR("(desired_pixel_scaled   lower_limit) else "));
      lower_boundary_i = 0; //default for errors
      upper_boundary_i = 1;
      bool found_match = false;
      for(uint8_t v=0;v<gradient_palettes.size()-1;v++) // Using the indexes expect the final one
      {      
        // ALOG_INF(PSTR("v>>>>>>>> [%d]  %d<%d<%d"), v, gradient_palettes[v], pixel_position_adjust, gradient_palettes[v+1]);

        uint8_t lower_index = v;
        uint8_t upper_index = v+1;

        if(
          (pixel_position_adjust >= gradient_palettes[lower_index])&&    // Greater than lower/current boundary
          (pixel_position_adjust < gradient_palettes[upper_index])     // Smaller than upper/next boundary
        ){
          lower_boundary_i = lower_index;
          upper_boundary_i = upper_index;
          lower_boundary_v = gradient_palettes[lower_boundary_i];
          upper_boundary_v = gradient_palettes[upper_boundary_i];
          progress = mSupport::mapfloat(desired_pixel_scaled, lower_boundary_v,upper_boundary_v, 0.0f, 1.0f);
          // ALOG_INF(PSTR("WITHIN ======================BREAK %d   ||| %d larger %d? (%d|%d)"), v, desired_pixel_scaled, gradient_palettes[v], lower_boundary_i, upper_boundary_i);
          found_match = true;
          break; // found lower boundary index
        }
        else{
          // ALOG_INF(PSTR("OUTSIDE %d is still lower than %d for index %d"), gradient_palettes[v], desired_pixel_scaled, v);
        }
      }
      if(!found_match)
      {
        // ALOG_INF(PSTR("ERROR: No match found for %d"), desired_pixel_scaled);
        // Assume its at the top range, so just force the last grad pair
          lower_boundary_i = gradient_palettes.size()-2;
          upper_boundary_i = gradient_palettes.size()-1;
          lower_boundary_v = gradient_palettes[lower_boundary_i];
          upper_boundary_v = gradient_palettes[upper_boundary_i];
          progress = mSupport::mapfloat(desired_pixel_scaled, lower_boundary_v,upper_boundary_v, 0.0f, 1.0f);



      }
    }
    // Serial.print("gradient_palettes"); for(uint8_t v=0;v<gradient_palettes.size();v++){ Serial.printf("%d,",gradient_palettes[v]); } Serial.println();
    // ALOG_INF(PSTR("v>>>>>>>> [%d|%d]  %d|%d p%d"), _pixel_position, desired_pixel_scaled, lower_boundary_v, upper_boundary_v, (int)(progress*100));

    // RgbcctColor lower_colour = Get_StaticPalette_Encoded_Colour_ReadBuffer(palette_adjusted_id, palette_buffer, lower_boundary_i); 
    // RgbcctColor upper_colour = Get_StaticPalette_Encoded_Colour_ReadBuffer(palette_adjusted_id, palette_buffer, upper_boundary_i);
    
    RgbcctColor lower_colour = Get_Encoded_Colour_ReadBuffer_Fast(
      palette_buffer,
      lower_boundary_i,  
      nullptr,
      encoding,
      encoded_colour_width
    );
    RgbcctColor upper_colour = Get_Encoded_Colour_ReadBuffer_Fast(
      palette_buffer,
      upper_boundary_i,  
      nullptr,
      encoding,
      encoded_colour_width
    );

    colour = RgbcctColor::LinearBlend(lower_colour, upper_colour, progress);

    // lower_colour.debug_print("lower_colour");
    // upper_colour.debug_print("upper_colour");
    // out.debug_print("  out_colour");

    return colour;

  } // if(ptr->encoding.index_gradient || flag_force_gradient) 

  
  /******************************************************************************************************************************************************
   * *******************************************************************************************************************************************************************************
   * 
   * Not a gradient palette, but needs to be spanned => Just show across the span without blending
   * 
   * ******************************************************************************************************************************************************
   * ******************************************************************************************************************************************************************************* */

  // Serial.println("What is this for then? It was either discrete, or gradient, so what is this?");
      
  if(flag_spanned_segment)
  {
    pixel_position_adjust = (_pixel_position*255)/(pCONT_lAni->_virtualSegmentLength -1);  // This scales out segment_index to segment_length as 0 to 255
  }

  // PALETTE_DATA *ptr = &static_palettes[palette_adjusted_id];
  // uint8_t pixels_in_map = GetNumberOfColoursInPalette(palette_adjusted_id);  

  uint8_t palette_index = scale8(pixel_position_adjust, colours_in_palette-1); //assumes original index is 0-255, and scales to 0-15

  // ALOG_INF(PSTR("pixel_position|palette_index %d->%d  %d %d"), pixel_position_adjust, palette_index, pixels_in_map, palette_index % 5);
  
  colour = Get_Encoded_Colour_ReadBuffer_Fast(
    palette_buffer,
    palette_index,  
    encoded_value,
    encoding,
    encoded_colour_width
  );
  

  return colour;

}


#endif // header gaurd
