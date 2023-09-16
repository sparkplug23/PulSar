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

void mPalette::Init_Palettes()
{
  
  ALOG_DBM( PSTR("Init_Palettes") );
  
  /****************
   * StaticPalettes
   ****************/

  addStaticPalette({
    PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID, 
    PM_PALETTE_COLOURFUL_DEFAULT_NAME_CTR,
    PM_PALETTE_COLOURFUL_DEFAULT__DATA, 
    sizeof(PM_PALETTE_COLOURFUL_DEFAULT__DATA),
    D_PALETTE_COLOURFUL_DEFAULT_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC_HOLLOWEEN_OP__ID, 
    PM_PALETTE_HOLLOWEEN_OP_NAME_CTR,
    PM_PALETTE_HOLLOWEEN_OP__DATA, 
    sizeof(PM_PALETTE_HOLLOWEEN_OP__DATA),
    D_PALETTE_HOLLOWEEN_OP_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC_HOLLOWEEN_OGP__ID, 
    PM_PALETTE_HOLLOWEEN_OGP_NAME_CTR,
    PM_PALETTE_HOLLOWEEN_OGP__DATA, 
    sizeof(PM_PALETTE_HOLLOWEEN_OGP__DATA),
    D_PALETTE_HOLLOWEEN_OGP_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC_RAINBOW__ID, 
    PM_PALETTE_RAINBOW_NAME_CTR,
    PM_PALETTE_RAINBOW__DATA, 
    sizeof(PM_PALETTE_RAINBOW__DATA),
    D_PALETTE_RAINBOW_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC_COMPRESSED_RAINBOW__ID, 
    PM_PALETTE_COMPRESSED_RAINBOW_NAME_CTR,
    PM_PALETTE_COMPRESSED_RAINBOW__DATA, 
    sizeof(PM_PALETTE_COMPRESSED_RAINBOW__DATA),
    D_PALETTE_COMPRESSED_RAINBOW_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC_RAINBOW_INVERTED__ID, 
    PM_PALETTE_RAINBOW_INVERTED_NAME_CTR,
    PM_PALETTE_RAINBOW_INVERTED__DATA, 
    sizeof(PM_PALETTE_RAINBOW_INVERTED__DATA),
    D_PALETTE_RAINBOW_INVERTED_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC_PASTEL_01__ID, 
    PM_PALETTE_PASTEL_01_NAME_CTR,
    PM_PALETTE_PASTEL_01__DATA, 
    sizeof(PM_PALETTE_PASTEL_01__DATA),
    D_PALETTE_PASTEL_01_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC_PASTEL_02__ID, 
    PM_PALETTE_PASTEL_02_NAME_CTR,
    PM_PALETTE_PASTEL_02__DATA, 
    sizeof(PM_PALETTE_PASTEL_02__DATA),
    D_PALETTE_PASTEL_02_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC_PASTEL_03__ID, 
    PM_PALETTE_PASTEL_03_NAME_CTR,
    PM_PALETTE_PASTEL_03__DATA, 
    sizeof(PM_PALETTE_PASTEL_03__DATA),
    D_PALETTE_PASTEL_03_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC_PASTEL_04__ID, 
    PM_PALETTE_PASTEL_04_NAME_CTR,
    PM_PALETTE_PASTEL_04__DATA, 
    sizeof(PM_PALETTE_PASTEL_04__DATA),
    D_PALETTE_PASTEL_04_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC_PASTEL_05__ID, 
    PM_PALETTE_PASTEL_05_NAME_CTR,
    PM_PALETTE_PASTEL_05__DATA, 
    sizeof(PM_PALETTE_PASTEL_05__DATA),
    D_PALETTE_PASTEL_05_ENCODING
  });
  
  addStaticPalette({
    PALETTELIST_STATIC_GRADIENT_PASTEL_TONES_PURPLE__ID, 
    PM_PALETTE_GRADIENT_PASTEL_TONES_PURPLE_NAME_CTR,
    PM_PALETTE_GRADIENT_PASTEL_TONES_PURPLE__DATA, 
    sizeof(PM_PALETTE_GRADIENT_PASTEL_TONES_PURPLE__DATA),
    D_PALETTE_GRADIENT_PASTEL_TONES_PURPLE_ENCODING
  });
  
  addStaticPalette({
    PALETTELIST_STATIC_HOT_PINK_NEON_WITH_NAVY__ID, 
    PM_PALETTE_HOT_PINK_NEON_WITH_NAVY_NAME_CTR,
    PM_PALETTE_HOT_PINK_NEON_WITH_NAVY__DATA, 
    sizeof(PM_PALETTE_HOT_PINK_NEON_WITH_NAVY__DATA),
    D_PALETTE_HOT_PINK_NEON_WITH_NAVY_ENCODING
  });


  addStaticPalette({
    PALETTELIST_STATIC_WINTER_01__ID, 
    PM_PALETTE_WINTER_01_NAME_CTR,
    PM_PALETTE_WINTER_01__DATA, 
    sizeof(PM_PALETTE_WINTER_01__DATA),
    D_PALETTE_WINTER_01_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC_WINTER_02__ID, 
    PM_PALETTE_WINTER_02_NAME_CTR,
    PM_PALETTE_WINTER_02__DATA, 
    sizeof(PM_PALETTE_WINTER_02__DATA),
    D_PALETTE_WINTER_02_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC_WINTER_03__ID, 
    PM_PALETTE_WINTER_03_NAME_CTR,
    PM_PALETTE_WINTER_03__DATA, 
    sizeof(PM_PALETTE_WINTER_03__DATA),
    D_PALETTE_WINTER_03_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC_WINTER_04__ID, 
    PM_PALETTE_WINTER_04_NAME_CTR,
    PM_PALETTE_WINTER_04__DATA, 
    sizeof(PM_PALETTE_WINTER_04__DATA),
    D_PALETTE_WINTER_04_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC_AUTUMN_GREEN__ID, 
    PM_PALETTE_AUTUMN_GREEN_NAME_CTR,
    PM_PALETTE_AUTUMN_GREEN__DATA, 
    sizeof(PM_PALETTE_AUTUMN_GREEN__DATA),
    D_PALETTE_AUTUMN_GREEN_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC_AUTUMN_RED__ID, 
    PM_PALETTE_AUTUMN_RED_NAME_CTR,
    PM_PALETTE_AUTUMN_RED__DATA, 
    sizeof(PM_PALETTE_AUTUMN_RED__DATA),
    D_PALETTE_AUTUMN_RED_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC__CHRISTMAS_TRADITIONAL_RGPBO__ID, 
    PM_PALETTE__CHRISTMAS_TRADITIONAL_RGPBO__NAME_CTR,
    PM_PALETTE__CHRISTMAS_TRADITIONAL_RGPBO__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_TRADITIONAL_RGPBO__DATA),
    D_PALETTE__CHRISTMAS_TRADITIONAL_RGPBO__ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC__CHRISTMAS_TRADITIONAL_RGPBY__ID, 
    PM_PALETTE__CHRISTMAS_TRADITIONAL_RGPBY__NAME_CTR,
    PM_PALETTE__CHRISTMAS_TRADITIONAL_RGPBY__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_TRADITIONAL_RGPBY__DATA),
    D_PALETTE__CHRISTMAS_TRADITIONAL_RGPBY__ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC__CHRISTMAS_TRADITIONAL_ROGPBY__ID, 
    PM_PALETTE__CHRISTMAS_TRADITIONAL_ROGPBY__NAME_CTR,
    PM_PALETTE__CHRISTMAS_TRADITIONAL_ROGPBY__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_TRADITIONAL_ROGPBY__DATA),
    D_PALETTE__CHRISTMAS_TRADITIONAL_ROGPBY__ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC__CHRISTMAS_TRADITIONAL_RGBO__ID, 
    PM_PALETTE__CHRISTMAS_TRADITIONAL_RGBO__NAME_CTR,
    PM_PALETTE__CHRISTMAS_TRADITIONAL_RGBO__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_TRADITIONAL_RGBO__DATA),
    D_PALETTE__CHRISTMAS_TRADITIONAL_RGBO__ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC__CHRISTMAS_TRADITIONAL_RGBY__ID, 
    PM_PALETTE__CHRISTMAS_TRADITIONAL_RGBY__NAME_CTR,
    PM_PALETTE__CHRISTMAS_TRADITIONAL_RGBY__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_TRADITIONAL_RGBY__DATA),
    D_PALETTE__CHRISTMAS_TRADITIONAL_RGBY__ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC__CHRISTMAS_COLD_WHITE__ID, 
    PM_PALETTE__CHRISTMAS_COLD_WHITE__NAME_CTR,
    PM_PALETTE__CHRISTMAS_COLD_WHITE__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_COLD_WHITE__DATA),
    D_PALETTE__CHRISTMAS_COLD_WHITE__ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC__CHRISTMAS_WARM_WHITE__ID, 
    PM_PALETTE__CHRISTMAS_WARM_WHITE__NAME_CTR,
    PM_PALETTE__CHRISTMAS_WARM_WHITE__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_WARM_WHITE__DATA),
    D_PALETTE__CHRISTMAS_WARM_WHITE__ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC__CHRISTMAS_VINTAGE_MINIBELLS__ID, 
    PM_PALETTE__CHRISTMAS_VINTAGE_MINIBELLS__NAME_CTR,
    PM_PALETTE__CHRISTMAS_VINTAGE_MINIBELLS__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_VINTAGE_MINIBELLS__DATA),
    D_PALETTE__CHRISTMAS_VINTAGE_MINIBELLS__ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC__CHRISTMAS_VINTAGE_MERRYLITES__ID, 
    PM_PALETTE__CHRISTMAS_VINTAGE_MERRYLITES__NAME_CTR,
    PM_PALETTE__CHRISTMAS_VINTAGE_MERRYLITES__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_VINTAGE_MERRYLITES__DATA),
    D_PALETTE__CHRISTMAS_VINTAGE_MERRYLITES__ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC__CHRISTMAS_VINTAGE_AGED_BULBS__ID, 
    PM_PALETTE__CHRISTMAS_VINTAGE_AGED_BULBS__NAME_CTR,
    PM_PALETTE__CHRISTMAS_VINTAGE_AGED_BULBS__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_VINTAGE_AGED_BULBS__DATA),
    D_PALETTE__CHRISTMAS_VINTAGE_AGED_BULBS__ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC__CHRISTMAS_SNOWY_COLOURS_01__ID, 
    PM_PALETTE__CHRISTMAS_SNOWY_COLOURS_01__NAME_CTR,
    PM_PALETTE__CHRISTMAS_SNOWY_COLOURS_01__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_SNOWY_COLOURS_01__DATA),
    D_PALETTE__CHRISTMAS_SNOWY_COLOURS_01__ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC__CHRISTMAS_SNOWY_COLOURS_02__ID, 
    PM_PALETTE__CHRISTMAS_SNOWY_COLOURS_02__NAME_CTR,
    PM_PALETTE__CHRISTMAS_SNOWY_COLOURS_02__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_SNOWY_COLOURS_02__DATA),
    D_PALETTE__CHRISTMAS_SNOWY_COLOURS_02__ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC__CHRISTMAS_BERRY_YELLOW__ID, 
    PM_PALETTE__CHRISTMAS_BERRY_YELLOW__NAME_CTR,
    PM_PALETTE__CHRISTMAS_BERRY_YELLOW__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_BERRY_YELLOW__DATA),
    D_PALETTE__CHRISTMAS_BERRY_YELLOW__ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC__CHRISTMAS_BERRY_ORANGE__ID, 
    PM_PALETTE__CHRISTMAS_BERRY_ORANGE__NAME_CTR,
    PM_PALETTE__CHRISTMAS_BERRY_ORANGE__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_BERRY_ORANGE__DATA),
    D_PALETTE__CHRISTMAS_BERRY_ORANGE__ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC__CHRISTMAS_BERRY_GREEN__ID, 
    PM_PALETTE__CHRISTMAS_BERRY_GREEN__NAME_CTR,
    PM_PALETTE__CHRISTMAS_BERRY_GREEN__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_BERRY_GREEN__DATA),
    D_PALETTE__CHRISTMAS_BERRY_GREEN__ENCODING
  });


  // END Christmas

  addStaticPalette({
    PALETTELIST_STATIC__COLOURFUL_PAIRS_01__ID, 
    PM_PALETTE__COLOURFUL_PAIRS_01__NAME_CTR,
    PM_PALETTE__COLOURFUL_PAIRS_01__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_PAIRS_01__DATA),
    D_PALETTE__COLOURFUL_PAIRS_01__ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC__COLOURFUL_COLOUR_WHITE_STRIPE__ID, 
    PM_PALETTE__COLOURFUL_COLOUR_WHITE_STRIPE__NAME_CTR,
    PM_PALETTE__COLOURFUL_COLOUR_WHITE_STRIPE__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_COLOUR_WHITE_STRIPE__DATA),
    D_PALETTE__COLOURFUL_COLOUR_WHITE_STRIPE__ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC__COLOURFUL_COLOUR_WARMWHITE_STRIPE__ID, 
    PM_PALETTE__COLOURFUL_COLOUR_WARMWHITE_STRIPE__NAME_CTR,
    PM_PALETTE__COLOURFUL_COLOUR_WARMWHITE_STRIPE__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_COLOUR_WARMWHITE_STRIPE__DATA),
    D_PALETTE__COLOURFUL_COLOUR_WARMWHITE_STRIPE__ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC__COLOURFUL_BLUE_WHITE__ID, 
    PM_PALETTE__COLOURFUL_BLUE_WHITE__NAME_CTR,
    PM_PALETTE__COLOURFUL_BLUE_WHITE__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_BLUE_WHITE__DATA),
    D_PALETTE__COLOURFUL_BLUE_WHITE__ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC__COLOURFUL_WHITE_MIXED__ID, 
    PM_PALETTE__COLOURFUL_WHITE_MIXED__NAME_CTR,
    PM_PALETTE__COLOURFUL_WHITE_MIXED__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_WHITE_MIXED__DATA),
    D_PALETTE__COLOURFUL_WHITE_MIXED__ENCODING
  });


  addStaticPalette({
    PALETTELIST_STATIC__COLOURFUL_PINK_RED_YELLOW__ID, 
    PM_PALETTE__COLOURFUL_PINK_RED_YELLOW__NAME_CTR,
    PM_PALETTE__COLOURFUL_PINK_RED_YELLOW__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_PINK_RED_YELLOW__DATA),
    D_PALETTE__COLOURFUL_PINK_RED_YELLOW__ENCODING
  });


  addStaticPalette({
    PALETTELIST_STATIC__COLOURFUL_PEACHY_ORANGE__ID, 
    PM_PALETTE__COLOURFUL_PEACHY_ORANGE__NAME_CTR,
    PM_PALETTE__COLOURFUL_PEACHY_ORANGE__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_PEACHY_ORANGE__DATA),
    D_PALETTE__COLOURFUL_PEACHY_ORANGE__ENCODING
  });


  addStaticPalette({
    PALETTELIST_STATIC__COLOURFUL_PEACHY_YELLOW__ID, 
    PM_PALETTE__COLOURFUL_PEACHY_YELLOW__NAME_CTR,
    PM_PALETTE__COLOURFUL_PEACHY_YELLOW__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_PEACHY_YELLOW__DATA),
    D_PALETTE__COLOURFUL_PEACHY_YELLOW__ENCODING
  });


  addStaticPalette({
    PALETTELIST_STATIC__COLOURFUL_GREENLESS__ID, 
    PM_PALETTE__COLOURFUL_GREENLESS__NAME_CTR,
    PM_PALETTE__COLOURFUL_GREENLESS__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_GREENLESS__DATA),
    D_PALETTE__COLOURFUL_GREENLESS__ENCODING
  });
  
  addStaticPalette({
    PALETTELIST_STATIC__GOLDEN__ID, 
    PM_PALETTE__GOLDEN__NAME_CTR,
    PM_PALETTE__GOLDEN__DATA, 
    sizeof(PM_PALETTE__GOLDEN__DATA),
    D_PALETTE__GOLDEN__ENCODING
  });


  addStaticPalette({
    PALETTELIST_STATIC_SUNRISE_01__ID, 
    PM_PALETTE_SUNRISE_01_NAME_CTR,
    PM_PALETTE_SUNRISE_01__DATA, 
    sizeof(PM_PALETTE_SUNRISE_01__DATA),
    D_PALETTE_SUNRISE_01_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC_SUNRISE_02__ID, 
    PM_PALETTE_SUNRISE_02_NAME_CTR,
    PM_PALETTE_SUNRISE_02__DATA, 
    sizeof(PM_PALETTE_SUNRISE_02__DATA),
    D_PALETTE_SUNRISE_02_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC_SUNRISE_03__ID, 
    PM_PALETTE_SUNRISE_03_NAME_CTR,
    PM_PALETTE_SUNRISE_03__DATA, 
    sizeof(PM_PALETTE_SUNRISE_03__DATA),
    D_PALETTE_SUNRISE_03_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC_SUNRISE_04__ID, 
    PM_PALETTE_SUNRISE_04_NAME_CTR,
    PM_PALETTE_SUNRISE_04__DATA, 
    sizeof(PM_PALETTE_SUNRISE_04__DATA),
    D_PALETTE_SUNRISE_04_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC_SUNSET_01__ID, 
    PM_PALETTE_SUNSET_01_NAME_CTR,
    PM_PALETTE_SUNSET_01__DATA, 
    sizeof(PM_PALETTE_SUNSET_01__DATA),
    D_PALETTE_SUNSET_01_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC_SUNSET_02__ID, 
    PM_PALETTE_SUNSET_02_NAME_CTR,
    PM_PALETTE_SUNSET_02__DATA, 
    sizeof(PM_PALETTE_SUNSET_02__DATA),
    D_PALETTE_SUNSET_02_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC_SUNSET_RED__ID, 
    PM_PALETTE_SUNSET_RED_NAME_CTR,
    PM_PALETTE_SUNSET_RED__DATA, 
    sizeof(PM_PALETTE_SUNSET_RED__DATA),
    D_PALETTE_SUNSET_RED_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC_SUNSET_BEACH__ID, 
    PM_PALETTE_SUNSET_BEACH_NAME_CTR,
    PM_PALETTE_SUNSET_BEACH__DATA, 
    sizeof(PM_PALETTE_SUNSET_BEACH__DATA),
    D_PALETTE_SUNSET_BEACH_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC_SKY_GLOW_01__ID, 
    PM_PALETTE_SKY_GLOW_01_NAME_CTR,
    PM_PALETTE_SKY_GLOW_01__DATA, 
    sizeof(PM_PALETTE_SKY_GLOW_01__DATA),
    D_PALETTE_SKY_GLOW_01_ENCODING
  });
  
  addStaticPalette({
    PALETTELIST_STATIC_CANDLE_FLAME_01__ID, 
    PM_PALETTE_CANDLE_FLAME_01_NAME_CTR,
    PM_PALETTE_CANDLE_FLAME_01__DATA, 
    sizeof(PM_PALETTE_CANDLE_FLAME_01__DATA),
    D_PALETTE_CANDLE_FLAME_01_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC_GRADIENT_FIRE_01__ID, 
    PM_PALETTE_GRADIENT_FIRE_01_NAME_CTR,
    PM_PALETTE_GRADIENT_FIRE_01__DATA, 
    sizeof(PM_PALETTE_GRADIENT_FIRE_01__DATA),
    D_PALETTE_GRADIENT_FIRE_01_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC_OCEAN_01__ID, 
    PM_PALETTE_OCEAN_01_NAME_CTR,
    PM_PALETTE_OCEAN_01__DATA, 
    sizeof(PM_PALETTE_OCEAN_01__DATA),
    D_PALETTE_OCEAN_01_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC_FLOWER_SWEATPEA__ID, 
    PM_PALETTE_FLOWER_SWEATPEA_NAME_CTR,
    PM_PALETTE_FLOWER_SWEATPEA__DATA, 
    sizeof(PM_PALETTE_FLOWER_SWEATPEA__DATA),
    D_PALETTE_FLOWER_SWEATPEA_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC_PINK_PURPLE__ID, 
    PM_PALETTE_PINK_PURPLE_NAME_CTR,
    PM_PALETTE_PINK_PURPLE__DATA, 
    sizeof(PM_PALETTE_PINK_PURPLE__DATA),
    D_PALETTE_PINK_PURPLE_ENCODING
  });

  addStaticPalette({
    PALETTELIST_STATIC_PURPLE_PINK__ID, 
    PM_PALETTE_PURPLE_PINK_NAME_CTR,
    PM_PALETTE_PURPLE_PINK__DATA, 
    sizeof(PM_PALETTE_PURPLE_PINK__DATA),
    D_PALETTE_PURPLE_PINK_ENCODING
  });

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







int16_t mPalette::Get_Static_PaletteIDbyName(const char* c)
{

  if(*c=='\0'){
    return -1;
  }

  char buffer[100] = {0};
  int16_t index_found = -1;
  int16_t id = -1;

  /**************************************************************
   * 
   * PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR__IDS - undefined user names, default naming
   * 
  ***************************************************************/ 
  for(
    uint8_t ii=0;
            ii<(PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID-PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_01__ID);
            ii++
  ){
    memset(buffer,0,sizeof(buffer));
    sprintf_P(buffer, PSTR(D_DEFAULT_DYNAMIC_PALETTE_NAMES__VARIABLE_RGBCCT__NAME_CTR), ii + 1); // names are 1-10
    ALOG_INF( PSTR(DEBUG_INSERT_PAGE_BREAK "Searching with \"%s\" for \"%s\""),buffer,c );
    if(strcmp(c,buffer)==0)
    {
      return ii+PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_01__ID;
    }
  }

  /**************************************************************
   * 
   * PALETTELIST_DYNAMIC_CRGBPALETTE16_USER__LENGTH__ID
   * 
  ***************************************************************/
  for(
    uint8_t ii=0;
            ii<(PALETTELIST_DYNAMIC_CRGBPALETTE16_USER__LENGTH__ID-PALETTELIST_DYNAMIC_CRGBPALETTE16__CUSTOM_COLOURS_PAIRED_TWO_12__ID);
            ii++
  ){    
    ALOG_INF( PSTR("s> %d %s \"%S\""), ii, c, PM_DYNAMIC_CRGBPALETTE16_RANDOMISED_NAMES_CTR ); 
    if((id=mSupport::GetCommandID16_P(c, PM_DYNAMIC_CRGBPALETTE16_RANDOMISED_NAMES_CTR))>=0)
    {
      ALOG_INF( PSTR("MATCH \"%s\" %d %d"), c, ii, id ); 
      return id+PALETTELIST_DYNAMIC_CRGBPALETTE16__CUSTOM_COLOURS_PAIRED_TWO_12__ID;            
    }
  }

  /**************************************************************
   * 
   * PALETTELIST_DYNAMIC__COLOUR__IDS
   * 
  ***************************************************************/
  for(
    uint8_t ii=0;
            ii<(PALETTELIST_DYNAMIC__LENGTH__ID-PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID);
            ii++
  ){    
    ALOG_INF( PSTR("s> %d %s \"%S\""), ii, c, PM_DYNAMIC_RGBCCT_NAMES_CTR ); 
    if((id=mSupport::GetCommandID16_P(c, PM_DYNAMIC_RGBCCT_NAMES_CTR))>=0)
    {
      ALOG_INF( PSTR("MATCH \"%s\" %d %d"), c, ii, id ); 
      return id+PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID;            
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
    ALOG_INF( PSTR("s> %d %s \"%S\""), ii, c, PM_STATIC_CRGBPALETTE16_NAMES_CTR ); 
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
    ALOG_INF( PSTR("s> %d %s \"%S\""), ii, c, PM_STATIC_CRGBPALETTE16_GRADIENT_NAMES_CTR ); 
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
    ALOG_INF( PSTR("s> %d %s \"%S\""), ii, c, PM_STATIC_SINGLE_COLOUR_NAMES_CTR ); 
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
    uint8_t ii=PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID;
            ii<(PALETTELIST_STATIC_LENGTH__ID - PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID);
            ii++
  ){
    if(mSupport::CheckCommand_P(c, static_palettes[ii].friendly_name_ctr))
    {
      ALOG_DBM( PSTR("MATCH \"%c\" %d"), c, ii ); 
      return ii;            
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
    
    ALOG_DBG( PSTR("s> \"%s\""), buffer ); 
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

void mPalette::addStaticPalette(STATIC_PALETTE palette)
{

  if (palette.id < static_palettes.size()) {
    static_palettes[palette.id] = palette;
  } else {
    static_palettes.push_back(palette);
  }

}


void mPalette::addCustomPalette(uint16_t id, const uint8_t* data, const uint8_t length, uint16_t encoding)
{

  CUSTOM_PALETTE palette_tmp;

  #ifdef ESP32
  for(int i=0;i<length;i++){ palette_tmp.data.push_back(data[i]); }
  #else // ESP8266
  uint8_t data_t[length];
  memcpy_P(data_t, data, length);
  for(int i=0;i<length;i++){ palette_tmp.data.push_back(data_t[i]); }
  #endif

  palette_tmp.encoding = {encoding};

  if (id < custom_palettes.size()) {
    custom_palettes[id] = palette_tmp;
  } else {
    custom_palettes.push_back(palette_tmp);
  }

}


void mPalette::addDynamicPalette(uint16_t id, const uint8_t* data, const uint8_t length, uint16_t encoding)
{

  DYNAMIC_PALETTE palette_tmp;

  #ifdef ESP32
  for(int i=0;i<length;i++){ palette_tmp.data.push_back(data[i]); }
  #else // ESP8266
  uint8_t data_t[length];
  memcpy_P(data_t, data, length);
  for(int i=0;i<length;i++){ palette_tmp.data.push_back(data_t[i]); }
  #endif

  palette_tmp.encoding = {encoding};

  if (id < dynamic_palettes.size()) {
    dynamic_palettes[id] = palette_tmp;
  } else {
    dynamic_palettes.push_back(palette_tmp);
  }

}


const char* mPalette::GetPaletteNameByID(uint8_t palette_id, char* buffer, uint8_t buflen)
{

  /**************************************************************
   * 
   * PALETTELIST_SEGMENTS_STORED_FIXED_CRGBPALETTE16_PALETTES__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= mPalette::PALETTELIST_DYNAMIC_CRGBPALETTE16__CUSTOM_COLOURS_PAIRED_TWO_12__ID) && (palette_id < mPalette::PALETTELIST_DYNAMIC_CRGBPALETTE16_USER__LENGTH__ID))
  ){  
    uint16_t adjusted_id = palette_id - mPalette::PALETTELIST_DYNAMIC_CRGBPALETTE16__CUSTOM_COLOURS_PAIRED_TWO_12__ID;
    pCONT_sup->GetTextIndexed_P(buffer, buflen, adjusted_id, PM_DYNAMIC_CRGBPALETTE16_RANDOMISED_NAMES_CTR);   
  }

  /**************************************************************
   * 
   * PALETTELIST_STATIC_CRGBPALETTE16__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID) && (palette_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID))
  ){  
    uint16_t palette_id_adj = palette_id - mPalette::PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID;
    pCONT_sup->GetTextIndexed_P(buffer, buflen, palette_id_adj, PM_STATIC_CRGBPALETTE16_NAMES_CTR);   
    ALOG_DBG( PSTR("BName id%d|a%d \"%s\""), palette_id,palette_id_adj, buffer );
  }

  /**************************************************************
   * 
   * PALETTELIST_CRGBPALETTE16_GRADIENT___PALETTES__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID)    && (palette_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID))
  ){  
    uint16_t adjusted_id = palette_id - mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID;
    pCONT_sup->GetTextIndexed_P(buffer, buflen, adjusted_id, PM_STATIC_CRGBPALETTE16_GRADIENT_NAMES_CTR);   
  }

  /**************************************************************
   * 
   * PALETTELIST_DYNAMIC_
   * 
  ***************************************************************/
  if(
    ((palette_id >= mPalette::PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID) && (palette_id < mPalette::PALETTELIST_DYNAMIC__LENGTH__ID))
  ){           
    uint16_t adjusted_id = palette_id - mPalette::PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID;
    pCONT_sup->GetTextIndexed_P(buffer, buflen, adjusted_id, PM_DYNAMIC_RGBCCT_NAMES_CTR);
  }


  /**************************************************************
   * 
   * PALETTELIST_STATIC_CRGBPALETTE16__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= mPalette::PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID) && (palette_id < mPalette::PALETTELIST_STATIC_SINGLE_COLOUR__LENGTH__ID))
  ){  
    uint16_t palette_id_adj = palette_id - mPalette::PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID;
    pCONT_sup->GetTextIndexed_P(buffer, buflen, palette_id_adj, PM_STATIC_SINGLE_COLOUR_NAMES_CTR);   
    ALOG_DBG( PSTR("BName id%d|a%d \"%s\""), palette_id,palette_id_adj, buffer );
  }

  /**************************************************************
   * 
   * PALETTELIST_STATIC__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= mPalette::PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID) && (palette_id < mPalette::PALETTELIST_STATIC_LENGTH__ID))
  ){           
    uint16_t palette_id_adj = palette_id - mPalette::PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID;
    ALOG_DBG( PSTR("AName id%d|a%d \"%S\""), palette_id,palette_id_adj,static_palettes[palette_id_adj].friendly_name_ctr );
    snprintf_P(buffer, buflen, static_palettes[palette_id_adj].friendly_name_ctr);
  }

  /**************************************************************
   * 
   * PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID
   * 
  ***************************************************************/
  if(
    ((palette_id >= mPalette::PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_01__ID)    && (palette_id < mPalette::PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID))
  ){  
    uint16_t adjusted_id = palette_id - mPalette::PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_01__ID;
    sprintf_P(buffer, PSTR(D_DEFAULT_DYNAMIC_PALETTE_NAMES__VARIABLE_RGBCCT__NAME_CTR), adjusted_id); // names are 1-10
  }

  /**************************************************************
   * 
   * PALETTELIST_VARIABLE_GENERIC__IDS - undefined user names, default naming
   * 
  ***************************************************************/
 if(
    ((palette_id >= mPalette::PALETTELIST_LENGTH_OF_STATIC_IDS)    && (palette_id < GetPaletteListLength()))
  ){  
    uint16_t adjusted_id = palette_id - mPalette::PALETTELIST_LENGTH_OF_STATIC_IDS;
    sprintf_P(buffer, PSTR(D_DEFAULT_MODIFIABLE_PALETTE_NAMES__USER_CREATED__NAME_CTR), adjusted_id); // names are 1-10
  }
    
  return buffer;

}


/**
 * Refresh value stored in palette, and return new value
 * */
uint8_t mPalette::GetColourMapSizeByPaletteID(uint8_t palette_id){
  
  uint8_t new_size = 1; // assumed 1 at least
  
  if((palette_id>=PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_01__ID)&&(palette_id<PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID)){

    mPalette::STATIC_PALETTE *ptr_tmp = &mPaletteI->static_palettes[palette_id];

    new_size = 5; // only 1*5
    
    ptr_tmp->data_length = new_size;

  }else
  if((palette_id>=PALETTELIST_LENGTH_OF_STATIC_IDS)&&(palette_id<GetPaletteListLength())){

    // mPalette::PALETTE *ptr_tmp = &mPaletteI->static_palettes[palette_id];
    
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








uint16_t mPalette::GetNumberOfColoursInPalette(uint16_t palette_id, uint8_t pixel_width_contrained_limit)
{
  STATIC_PALETTE pal = static_palettes[palette_id];
  return GetNumberOfColoursFromEncoded(pal.encoding, pal.data_length);
}


uint16_t mPalette::GetNumberOfColoursFromEncoded(PALETTE_ENCODING_DATA encoding, uint8_t data_in_palette)
{
  uint16_t encoded_colour_width = GetEncodedColourWidth(encoding);
  return data_in_palette/encoded_colour_width;   
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
  bool     flag_spanned_segment, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
  bool     flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
  bool     flag_crgb_exact_colour
){

  RgbcctColor colour = RgbcctColor(0);

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
    ((palette_id >= PALETTELIST_DYNAMIC_CRGBPALETTE16__CUSTOM_COLOURS_PAIRED_TWO_12__ID)    && (palette_id < PALETTELIST_DYNAMIC_CRGBPALETTE16_USER__LENGTH__ID))
  ){

    uint8_t segIdx = pCONT_lAni->_segment_index_primary;

    CRGB fastled_col;
    
    // ALOG_INF(PSTR("id_adjusted %d  \tpixel_pos%d in_palette{%d}"), palette_id_adjusted, _pixel_position, colours_in_palette);

    uint8_t pixel_position_adjust = _pixel_position;    


    if(flag_crgb_exact_colour)
    {
      uint8_t pixels_in_crgb16palette = GetColoursInCRGB16Palette(palette_id);
      pixel_position_adjust = _pixel_position % pixels_in_crgb16palette; // convert incoming pixels into repeating 0-15 numbers.
      /**
       * @brief If exact, instead of scale I should use the index to get the position
       * Convert index (that wraps by amount of colours) to get scaled 255 version
       **/
      // ALOG_INF(PSTR("Cpixel_position_adjust %d|%d==%d"), _pixel_position, pixel_position_adjust, pSEGMENT_I(segIdx).palette_container->CRGB16Palette16_Palette.encoded_index.size());
      if(pixel_position_adjust < pSEGMENT_I(segIdx).palette_container->CRGB16Palette16_Palette.encoded_index.size())
      {
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

    return colour;

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
    return RgbcctColor(data[adjust_buf_i], data[adjust_buf_i+1], data[adjust_buf_i+2]);

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

    colour = Get_Encoded_StaticPalette_Colour(
        palette_adjusted_id,
        palette_buffer,
        _pixel_position,  
        encoded_value,
        flag_spanned_segment,
        flag_wrap_hard_edge,
        flag_crgb_exact_colour
      );

    return colour;

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

    colour = Get_Encoded_DynamicPalette_Colour(
        palette_adjusted_id,
        palette_buffer,
        _pixel_position,  
        encoded_value,
        flag_spanned_segment,
        flag_wrap_hard_edge,
        flag_crgb_exact_colour
      );

    return colour;

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

    uint8_t adjusted_id = palette_id - PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_01__ID;
    uint8_t segIdx = pCONT_lAni->_segment_index_primary;

    if(adjusted_id < RGBCCTCOLOURS_SIZE)
    {
      return pCONT_lAni->segments[segIdx].rgbcctcolors[adjusted_id];
    }
  }


  /**************************************************************
   * 
   * Custom palettes: custom_palettes 
   * 
  ***************************************************************/
  else
  if(
    ((palette_id >= mPalette::PALETTELIST_LENGTH_OF_STATIC_IDS)  && (palette_id < mPaletteI->GetPaletteListLength())) // Custom palettes
  ){   
    
    uint16_t palette_adjusted_id = palette_id - mPalette::PALETTELIST_LENGTH_OF_STATIC_IDS;        

    return SubGet_Encoded_CustomPalette_Colour(
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

  return colour;

}




#endif // header gaurd
