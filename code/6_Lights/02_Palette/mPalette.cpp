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

  addPalette({
    PALETTELIST_FIXED_COLOURFUL_DEFAULT__ID, 
    PM_PALETTE_COLOURFUL_DEFAULT_NAME_CTR,
    PM_PALETTE_COLOURFUL_DEFAULT_RGB__DATA, 
    sizeof(PM_PALETTE_COLOURFUL_DEFAULT_RGB__DATA),
    D_PALETTE_COLOURFUL_DEFAULT_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_HOLLOWEEN_OP__ID, 
    PM_PALETTE_HOLLOWEEN_OP_NAME_CTR,
    PM_PALETTE_HOLLOWEEN_OP_RGB__DATA, 
    sizeof(PM_PALETTE_HOLLOWEEN_OP_RGB__DATA),
    D_PALETTE_HOLLOWEEN_OP_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_HOLLOWEEN_OGP__ID, 
    PM_PALETTE_HOLLOWEEN_OGP_NAME_CTR,
    PM_PALETTE_HOLLOWEEN_OGP_RGB__DATA, 
    sizeof(PM_PALETTE_HOLLOWEEN_OGP_RGB__DATA),
    D_PALETTE_HOLLOWEEN_OGP_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_HOT_PINK_NEON_WITH_NAVY__ID, 
    PM_PALETTE_HOT_PINK_NEON_WITH_NAVY_NAME_CTR,
    PM_PALETTE_HOT_PINK_NEON_WITH_NAVY_RGB__DATA, 
    sizeof(PM_PALETTE_HOT_PINK_NEON_WITH_NAVY_RGB__DATA),
    D_PALETTE_HOT_PINK_NEON_WITH_NAVY_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_RAINBOW__ID, 
    PM_PALETTE_RAINBOW_NAME_CTR,
    PM_PALETTE_RAINBOW_RGB__DATA, 
    sizeof(PM_PALETTE_RAINBOW_RGB__DATA),
    D_PALETTE_RAINBOW_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_RAINBOW_INVERTED__ID, 
    PM_PALETTE_RAINBOW_INVERTED_NAME_CTR,
    PM_PALETTE_RAINBOW_INVERTED_RGB__DATA, 
    sizeof(PM_PALETTE_RAINBOW_INVERTED_RGB__DATA),
    D_PALETTE_RAINBOW_INVERTED_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_PASTEL_01__ID, 
    PM_PALETTE_PASTEL_01_NAME_CTR,
    PM_PALETTE_PASTEL_01_RGB__DATA, 
    sizeof(PM_PALETTE_PASTEL_01_RGB__DATA),
    D_PALETTE_PASTEL_01_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_PASTEL_02__ID, 
    PM_PALETTE_PASTEL_02_NAME_CTR,
    PM_PALETTE_PASTEL_02_RGB__DATA, 
    sizeof(PM_PALETTE_PASTEL_02_RGB__DATA),
    D_PALETTE_PASTEL_02_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_PASTEL_03__ID, 
    PM_PALETTE_PASTEL_03_NAME_CTR,
    PM_PALETTE_PASTEL_03_RGB__DATA, 
    sizeof(PM_PALETTE_PASTEL_03_RGB__DATA),
    D_PALETTE_PASTEL_03_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_PASTEL_04__ID, 
    PM_PALETTE_PASTEL_04_NAME_CTR,
    PM_PALETTE_PASTEL_04_RGB__DATA, 
    sizeof(PM_PALETTE_PASTEL_04_RGB__DATA),
    D_PALETTE_PASTEL_04_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_PASTEL_05__ID, 
    PM_PALETTE_PASTEL_05_NAME_CTR,
    PM_PALETTE_PASTEL_05_RGB__DATA, 
    sizeof(PM_PALETTE_PASTEL_05_RGB__DATA),
    D_PALETTE_PASTEL_05_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_WINTER_01__ID, 
    PM_PALETTE_WINTER_01_NAME_CTR,
    PM_PALETTE_WINTER_01_RGB__DATA, 
    sizeof(PM_PALETTE_WINTER_01_RGB__DATA),
    D_PALETTE_WINTER_01_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_WINTER_02__ID, 
    PM_PALETTE_WINTER_02_NAME_CTR,
    PM_PALETTE_WINTER_02_RGB__DATA, 
    sizeof(PM_PALETTE_WINTER_02_RGB__DATA),
    D_PALETTE_WINTER_02_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_WINTER_03__ID, 
    PM_PALETTE_WINTER_03_NAME_CTR,
    PM_PALETTE_WINTER_03_RGB__DATA, 
    sizeof(PM_PALETTE_WINTER_03_RGB__DATA),
    D_PALETTE_WINTER_03_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_WINTER_04__ID, 
    PM_PALETTE_WINTER_04_NAME_CTR,
    PM_PALETTE_WINTER_04_RGB__DATA, 
    sizeof(PM_PALETTE_WINTER_04_RGB__DATA),
    D_PALETTE_WINTER_04_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_AUTUMN_GREEN__ID, 
    PM_PALETTE_AUTUMN_GREEN_NAME_CTR,
    PM_PALETTE_AUTUMN_GREEN_RGB__DATA, 
    sizeof(PM_PALETTE_AUTUMN_GREEN_RGB__DATA),
    D_PALETTE_AUTUMN_GREEN_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_AUTUMN_RED__ID, 
    PM_PALETTE_AUTUMN_RED_NAME_CTR,
    PM_PALETTE_AUTUMN_RED_RGB__DATA, 
    sizeof(PM_PALETTE_AUTUMN_RED_RGB__DATA),
    D_PALETTE_AUTUMN_RED_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_GRADIENT_01__ID, 
    PM_PALETTE_GRADIENT_01_NAME_CTR,
    PM_PALETTE_GRADIENT_01_RGB__DATA, 
    sizeof(PM_PALETTE_GRADIENT_01_RGB__DATA),
    D_PALETTE_GRADIENT_01_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_GRADIENT_02__ID, 
    PM_PALETTE_GRADIENT_02_NAME_CTR,
    PM_PALETTE_GRADIENT_02_RGB__DATA, 
    sizeof(PM_PALETTE_GRADIENT_02_RGB__DATA),
    D_PALETTE_GRADIENT_02_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_GRADIENT_PASTEL_TONES_PURPLE__ID, 
    PM_PALETTE_GRADIENT_PASTEL_TONES_PURPLE_NAME_CTR,
    PM_PALETTE_GRADIENT_PASTEL_TONES_PURPLE_RGB__DATA, 
    sizeof(PM_PALETTE_GRADIENT_PASTEL_TONES_PURPLE_RGB__DATA),
    D_PALETTE_GRADIENT_PASTEL_TONES_PURPLE_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_BERRY_GREEN__ID, 
    PM_PALETTE_BERRY_GREEN_NAME_CTR,
    PM_PALETTE_BERRY_GREEN_RGB__DATA, 
    sizeof(PM_PALETTE_BERRY_GREEN_RGB__DATA),
    D_PALETTE_BERRY_GREEN_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_01__ID, 
    PM_PALETTE_CHRISTMAS_01_NAME_CTR,
    PM_PALETTE_CHRISTMAS_01_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_01_RGB__DATA),
    D_PALETTE_CHRISTMAS_01_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_02__ID, 
    PM_PALETTE_CHRISTMAS_02_NAME_CTR,
    PM_PALETTE_CHRISTMAS_02_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_02_RGB__DATA),
    D_PALETTE_CHRISTMAS_02_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_03__ID, 
    PM_PALETTE_CHRISTMAS_03_NAME_CTR,
    PM_PALETTE_CHRISTMAS_03_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_03_RGB__DATA),
    D_PALETTE_CHRISTMAS_03_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_04__ID, 
    PM_PALETTE_CHRISTMAS_04_NAME_CTR,
    PM_PALETTE_CHRISTMAS_04_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_04_RGB__DATA),
    D_PALETTE_CHRISTMAS_04_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_05__ID, 
    PM_PALETTE_CHRISTMAS_05_NAME_CTR,
    PM_PALETTE_CHRISTMAS_05_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_05_RGB__DATA),
    D_PALETTE_CHRISTMAS_05_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_06__ID, 
    PM_PALETTE_CHRISTMAS_06_NAME_CTR,
    PM_PALETTE_CHRISTMAS_06_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_06_RGB__DATA),
    D_PALETTE_CHRISTMAS_06_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_07__ID, 
    PM_PALETTE_CHRISTMAS_07_NAME_CTR,
    PM_PALETTE_CHRISTMAS_07_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_07_RGB__DATA),
    D_PALETTE_CHRISTMAS_07_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_08__ID, 
    PM_PALETTE_CHRISTMAS_08_NAME_CTR,
    PM_PALETTE_CHRISTMAS_08_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_08_RGB__DATA),
    D_PALETTE_CHRISTMAS_08_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_09__ID, 
    PM_PALETTE_CHRISTMAS_09_NAME_CTR,
    PM_PALETTE_CHRISTMAS_09_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_09_RGB__DATA),
    D_PALETTE_CHRISTMAS_09_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_10__ID, 
    PM_PALETTE_CHRISTMAS_10_NAME_CTR,
    PM_PALETTE_CHRISTMAS_10_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_10_RGB__DATA),
    D_PALETTE_CHRISTMAS_10_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_11__ID, 
    PM_PALETTE_CHRISTMAS_11_NAME_CTR,
    PM_PALETTE_CHRISTMAS_11_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_11_RGB__DATA),
    D_PALETTE_CHRISTMAS_11_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_12__ID, 
    PM_PALETTE_CHRISTMAS_12_NAME_CTR,
    PM_PALETTE_CHRISTMAS_12_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_12_RGB__DATA),
    D_PALETTE_CHRISTMAS_12_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_13__ID, 
    PM_PALETTE_CHRISTMAS_13_NAME_CTR,
    PM_PALETTE_CHRISTMAS_13_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_13_RGB__DATA),
    D_PALETTE_CHRISTMAS_13_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_14__ID, 
    PM_PALETTE_CHRISTMAS_14_NAME_CTR,
    PM_PALETTE_CHRISTMAS_14_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_14_RGB__DATA),
    D_PALETTE_CHRISTMAS_14_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_15__ID, 
    PM_PALETTE_CHRISTMAS_15_NAME_CTR,
    PM_PALETTE_CHRISTMAS_15_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_15_RGB__DATA),
    D_PALETTE_CHRISTMAS_15_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_16__ID, 
    PM_PALETTE_CHRISTMAS_16_NAME_CTR,
    PM_PALETTE_CHRISTMAS_16_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_16_RGB__DATA),
    D_PALETTE_CHRISTMAS_16_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_17__ID, 
    PM_PALETTE_CHRISTMAS_17_NAME_CTR,
    PM_PALETTE_CHRISTMAS_17_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_17_RGB__DATA),
    D_PALETTE_CHRISTMAS_17_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_18__ID, 
    PM_PALETTE_CHRISTMAS_18_NAME_CTR,
    PM_PALETTE_CHRISTMAS_18_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_18_RGB__DATA),
    D_PALETTE_CHRISTMAS_18_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_19__ID, 
    PM_PALETTE_CHRISTMAS_19_NAME_CTR,
    PM_PALETTE_CHRISTMAS_19_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_19_RGB__DATA),
    D_PALETTE_CHRISTMAS_19_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_20__ID, 
    PM_PALETTE_CHRISTMAS_20_NAME_CTR,
    PM_PALETTE_CHRISTMAS_20_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_20_RGB__DATA),
    D_PALETTE_CHRISTMAS_20_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_21__ID, 
    PM_PALETTE_CHRISTMAS_21_NAME_CTR,
    PM_PALETTE_CHRISTMAS_21_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_21_RGB__DATA),
    D_PALETTE_CHRISTMAS_21_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_22__ID, 
    PM_PALETTE_CHRISTMAS_22_NAME_CTR,
    PM_PALETTE_CHRISTMAS_22_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_22_RGB__DATA),
    D_PALETTE_CHRISTMAS_22_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_23__ID, 
    PM_PALETTE_CHRISTMAS_23_NAME_CTR,
    PM_PALETTE_CHRISTMAS_23_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_23_RGB__DATA),
    D_PALETTE_CHRISTMAS_23_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_24__ID, 
    PM_PALETTE_CHRISTMAS_24_NAME_CTR,
    PM_PALETTE_CHRISTMAS_24_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_24_RGB__DATA),
    D_PALETTE_CHRISTMAS_24_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_25__ID, 
    PM_PALETTE_CHRISTMAS_25_NAME_CTR,
    PM_PALETTE_CHRISTMAS_25_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_25_RGB__DATA),
    D_PALETTE_CHRISTMAS_25_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_26__ID, 
    PM_PALETTE_CHRISTMAS_26_NAME_CTR,
    PM_PALETTE_CHRISTMAS_26_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_26_RGB__DATA),
    D_PALETTE_CHRISTMAS_26_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_27__ID, 
    PM_PALETTE_CHRISTMAS_27_NAME_CTR,
    PM_PALETTE_CHRISTMAS_27_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_27_RGB__DATA),
    D_PALETTE_CHRISTMAS_27_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_28__ID, 
    PM_PALETTE_CHRISTMAS_28_NAME_CTR,
    PM_PALETTE_CHRISTMAS_28_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_28_RGB__DATA),
    D_PALETTE_CHRISTMAS_28_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_29__ID, 
    PM_PALETTE_CHRISTMAS_29_NAME_CTR,
    PM_PALETTE_CHRISTMAS_29_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_29_RGB__DATA),
    D_PALETTE_CHRISTMAS_29_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CHRISTMAS_30__ID, 
    PM_PALETTE_CHRISTMAS_30_NAME_CTR,
    PM_PALETTE_CHRISTMAS_30_RGB__DATA, 
    sizeof(PM_PALETTE_CHRISTMAS_30_RGB__DATA),
    D_PALETTE_CHRISTMAS_30_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_SUNRISE_01__ID, 
    PM_PALETTE_SUNRISE_01_NAME_CTR,
    PM_PALETTE_SUNRISE_01_RGB__DATA, 
    sizeof(PM_PALETTE_SUNRISE_01_RGB__DATA),
    D_PALETTE_SUNRISE_01_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_SUNRISE_02__ID, 
    PM_PALETTE_SUNRISE_02_NAME_CTR,
    PM_PALETTE_SUNRISE_02_RGB__DATA, 
    sizeof(PM_PALETTE_SUNRISE_02_RGB__DATA),
    D_PALETTE_SUNRISE_02_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_SUNRISE_03__ID, 
    PM_PALETTE_SUNRISE_03_NAME_CTR,
    PM_PALETTE_SUNRISE_03_RGB__DATA, 
    sizeof(PM_PALETTE_SUNRISE_03_RGB__DATA),
    D_PALETTE_SUNRISE_03_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_SUNRISE_04__ID, 
    PM_PALETTE_SUNRISE_04_NAME_CTR,
    PM_PALETTE_SUNRISE_04_RGB__DATA, 
    sizeof(PM_PALETTE_SUNRISE_04_RGB__DATA),
    D_PALETTE_SUNRISE_04_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_SUNSET_01__ID, 
    PM_PALETTE_SUNSET_01_NAME_CTR,
    PM_PALETTE_SUNSET_01_RGB__DATA, 
    sizeof(PM_PALETTE_SUNSET_01_RGB__DATA),
    D_PALETTE_SUNSET_01_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_SUNSET_02__ID, 
    PM_PALETTE_SUNSET_02_NAME_CTR,
    PM_PALETTE_SUNSET_02_RGB__DATA, 
    sizeof(PM_PALETTE_SUNSET_02_RGB__DATA),
    D_PALETTE_SUNSET_02_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CANDLE_FLAME_01__ID, 
    PM_PALETTE_CANDLE_FLAME_01_NAME_CTR,
    PM_PALETTE_CANDLE_FLAME_01_RGB__DATA, 
    sizeof(PM_PALETTE_CANDLE_FLAME_01_RGB__DATA),
    D_PALETTE_CANDLE_FLAME_01_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_GRADIENT_FIRE_01__ID, 
    PM_PALETTE_GRADIENT_FIRE_01_NAME_CTR,
    PM_PALETTE_GRADIENT_FIRE_01_RGB__DATA, 
    sizeof(PM_PALETTE_GRADIENT_FIRE_01_RGB__DATA),
    D_PALETTE_GRADIENT_FIRE_01_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_SKY_GLOW_01__ID, 
    PM_PALETTE_SKY_GLOW_01_NAME_CTR,
    PM_PALETTE_SKY_GLOW_01_RGB__DATA, 
    sizeof(PM_PALETTE_SKY_GLOW_01_RGB__DATA),
    D_PALETTE_SKY_GLOW_01_ENCODING
  });

  #ifdef ENABLE_DEVFEATURE_SUNLEVEL_PALETTES
  addPalette({
    PALETTELIST_FIXED_GRADIENT_SUNLEVEL_GROUP01_01__ID, 
    PM_PALETTE_GRADIENT_SUNLEVEL_GROUP01_01_NAME_CTR,
    PM_PALETTE_GRADIENT_SUNLEVEL_GROUP01_01_RGB__DATA, 
    sizeof(PM_PALETTE_GRADIENT_SUNLEVEL_GROUP01_01_RGB__DATA),
    D_PALETTE_GRADIENT_SUNLEVEL_GROUP01_01_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_GRADIENT_SUNLEVEL_GROUP01_02__ID, 
    PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_02_NAME_CTR,
    PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_02_RGB__DATA, 
    sizeof(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_02_RGB__DATA),
    D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_02_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_GRADIENT_SUNLEVEL_GROUP01_03__ID, 
    PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_03_NAME_CTR,
    PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_03_RGB__DATA, 
    sizeof(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_03_RGB__DATA),
    D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_03_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_GRADIENT_SUNLEVEL_GROUP01_04__ID, 
    PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_04_NAME_CTR,
    PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_04_RGB__DATA, 
    sizeof(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_04_RGB__DATA),
    D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_04_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_GRADIENT_SUNLEVEL_GROUP01_05__ID, 
    PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_05_NAME_CTR,
    PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_05_RGB__DATA, 
    sizeof(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_05_RGB__DATA),
    D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_05_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_GRADIENT_SUNLEVEL_GROUP01_06__ID, 
    PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_06_NAME_CTR,
    PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_06_RGB__DATA, 
    sizeof(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_06_RGB__DATA),
    D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_06_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_GRADIENT_SUNLEVEL_GROUP01_07__ID, 
    PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_NAME_CTR,
    PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_RGB__DATA, 
    sizeof(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_RGB__DATA),
    D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_ENCODING
  });
  #endif // ENABLE_DEVFEATURE_SUNLEVEL_PALETTES

  #ifdef ENABLE_DEVFEATURE_SUNLEVEL_PALETTES
  addPalette({
    PALETTELIST_FIXED__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01__ID, 
    PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_01__CTR,
    PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_01__DATA, 
    sizeof(PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_01__DATA),
    D__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_01__ENCODING
  });

  addPalette({
    PALETTELIST_FIXED__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_02__ID, 
    PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_02__CTR,
    PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_02__DATA, 
    sizeof(PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_02__DATA),
    D__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_02__ENCODING
  });

  addPalette({
    PALETTELIST_FIXED__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_03__ID, 
    PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_03__CTR,
    PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_03__DATA, 
    sizeof(PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_03__DATA),
    D__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_03__ENCODING
  });

  addPalette({
    PALETTELIST_FIXED__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_04__ID, 
    PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_04__CTR,
    PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_04__DATA, 
    sizeof(PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_04__DATA),
    D__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_04__ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_DUAL_COLOUR_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01__ID, 
    PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_01__CTR,
    PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_01__DATA, 
    sizeof(PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_01__DATA),
    D__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_01__ENCODING
  });
  #endif // ENABLE_DEVFEATURE_SUNLEVEL_PALETTES

  addPalette({
    PALETTELIST_FIXED_OCEAN_01__ID, 
    PM_PALETTE_OCEAN_01_NAME_CTR,
    PM_PALETTE_OCEAN_01_RGB__DATA, 
    sizeof(PM_PALETTE_OCEAN_01_RGB__DATA),
    D_PALETTE_OCEAN_01_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_CUSTOM_USER_01__ID, 
    PM_PALETTE_CUSTOM_USER_01_NAME_CTR,
    PM_PALETTE_CUSTOM_USER_01_RGB__DATA, 
    sizeof(PM_PALETTE_CUSTOM_USER_01_RGB__DATA),
    D_PALETTE_CUSTOM_USER_01_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_SINGLE_FIRE_01__ID, 
    PM_PALETTE_SINGLE_FIRE_01_NAME_CTR,
    PM_PALETTE_SINGLE_FIRE_01_RGB__DATA, 
    sizeof(PM_PALETTE_SINGLE_FIRE_01_RGB__DATA),
    D_PALETTE_SINGLE_FIRE_01_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_FLOWER_SWEATPEAS_01__ID, 
    PM_PALETTE_FLOWER_SWEATPEAS_01_NAME_CTR,
    PM_PALETTE_FLOWER_SWEATPEAS_01_RGB__DATA, 
    sizeof(PM_PALETTE_FLOWER_SWEATPEAS_01_RGB__DATA),
    D_PALETTE_FLOWER_SWEATPEAS_01_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_FLOWER_SWEATPEAS_02__ID, 
    PM_PALETTE_FLOWER_SWEATPEAS_02_NAME_CTR,
    PM_PALETTE_FLOWER_SWEATPEAS_02_RGB__DATA, 
    sizeof(PM_PALETTE_FLOWER_SWEATPEAS_02_RGB__DATA),
    D_PALETTE_FLOWER_SWEATPEAS_02_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_PINK_PURPLE_01__ID, 
    PM_PALETTE_PINK_PURPLE_01_NAME_CTR,
    PM_PALETTE_PINK_PURPLE_01_RGB__DATA, 
    sizeof(PM_PALETTE_PINK_PURPLE_01_RGB__DATA),
    D_PALETTE_PINK_PURPLE_01_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_PINK_PURPLE_02__ID, 
    PM_PALETTE_PINK_PURPLE_02_NAME_CTR,
    PM_PALETTE_PINK_PURPLE_02_RGB__DATA, 
    sizeof(PM_PALETTE_PINK_PURPLE_02_RGB__DATA),
    D_PALETTE_PINK_PURPLE_02_ENCODING
  });

  addPalette({
    PALETTELIST_FIXED_PINK_PURPLE_03__ID, 
    PM_PALETTE_PINK_PURPLE_03_NAME_CTR,
    PM_PALETTE_PINK_PURPLE_03_RGB__DATA, 
    sizeof(PM_PALETTE_PINK_PURPLE_03_RGB__DATA),
    D_PALETTE_PINK_PURPLE_03_ENCODING
  });


  /****************
   * CustomPalettes
   ****************/

    DEBUG_LINE_HERE;
    DEBUG_LINE_HERE;
    DEBUG_LINE_HERE;
    DEBUG_LINE_HERE;
    DEBUG_LINE_HERE;
    DEBUG_LINE_HERE;
    DEBUG_LINE_HERE;
    DEBUG_LINE_HERE;
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
mPalette::GetColourFromPreloadedPaletteBuffer2(
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
    
  /**************************************************************
   * 
   * PALETTELIST_FIXED_CRGBPALETTE16__IDS
   * PALETTELIST_CRGBPALETTE16_GRADIENT___PALETTES__IDS
   * 
   * 
   * THIS SHOULD BE CHECKING FOR "CRGBPALETTE16 Encoded", all are saved into the loaded crb16palette
   * 
  ***************************************************************/
  if(
    ((palette_id >= PALETTELIST_FIXED_CRGBPALETTE16__RAINBOW_COLOUR__ID) && (palette_id < PALETTELIST_FIXED_CRGBPALETTE16__LENGTH__ID)) ||
    ((palette_id >= PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__SUNSET__ID)    && (palette_id < PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT_LENGTH__ID))  ||
    ((palette_id >= PALETTELIST_FIXED_CRGBPALETTE16__RANDOMISE_COLOURS_01__ID)    && (palette_id < PALETTELIST_FIXED_CRGBPALETTE16_USER__LENGTH__ID))
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
      if(pixel_position_adjust < pSEGMENT_I(segIdx).palette_container->CRGB16Palette16_Palette.encoded_index.size())
      {
        pixel_position_adjust = pSEGMENT_I(segIdx).palette_container->CRGB16Palette16_Palette.encoded_index[pixel_position_adjust];
      }
      // else{
      //   ALOG_INF(PSTR("Err %d"), pixel_position_adjust);
      // }
      // pixel_position_adjust = map(pixel_position_adjust, 0,pixels_in_crgb16palette-1, 0,255); //gradient type when exact value is needed needs scaled into full range
      // ALOG_ERR(PSTR("pixels_in_crgb16palette = %d"), pixels_in_crgb16palette);
    }
    
    
    if(flag_spanned_segment)
    {
      pixel_position_adjust = (_pixel_position*255)/(pCONT_lAni->_virtualSegmentLength -1);  // This scales out segment_index to segment_length as 0 to 255
    }

    // else{
    //   uint8_t pixels_in_crgb16palette = GetColoursInCRGB16Palette(palette_id);
    //   pixel_position_adjust = _pixel_position % pixels_in_crgb16palette; // convert incoming pixels into repeating 0-15 numbers.
    //   if(pixel_position_adjust < pSEGMENT_I(segIdx).palette_container->CRGB16Palette16_Palette.encoded_index.size()){
    //     pixel_position_adjust = pSEGMENT_I(segIdx).palette_container->CRGB16Palette16_Palette.encoded_index[pixel_position_adjust];
    //   }
    //   // pixel_position_adjust = map(pixel_position_adjust, 0,pixels_in_crgb16palette-1, 0,255);
    // }
    

    // if(flag_wrap_hard_edge==false) // If OFF, then cut end of palette off (rescale to 240)
    // {
    //   pixel_position_adjust = scale8(pixel_position_adjust, 240); // cut off blend at palette "end", 240, or 15/16*255=>240/255, so drop last 16th (15 to wrapped 0) gradient of colour
    // }

    // ALOG_INF(PSTR("pixel_position_adjust %d|%d"), _pixel_position, pixel_position_adjust);
    // FastLED Get ColorFromPalette that is CRGBPalette16
    fastled_col = ColorFromPalette( pSEGMENT_I(segIdx).palette_container->CRGB16Palette16_Palette.data, pixel_position_adjust, 255, NOBLEND);// (pCONT_lAni->paletteBlend == 3)? NOBLEND:LINEARBLEND);
  
    colour = RgbcctColor(fastled_col.r, fastled_col.g, fastled_col.b) ;// RgbcctColor::GetRgbcctFromU32Colour(fastled_col32);

    // if(_pixel_position < pSEGMENT_I(segIdx).palette_container->CRGB16Palette16_Palette.encoded_index.size())
    // {
    //   uint8_t enc = pSEGMENT_I(segIdx).palette_container->CRGB16Palette16_Palette.encoded_index[_pixel_position];
    //   // ALOG_ERR(PSTR("enc[%d] = %d|%d,%d,%d"),_pixel_position,enc,colour.R,colour.G,colour.B);
    // }


    // if(_pixel_position<20)
    // ALOG_INF(PSTR("colour[%d|%d] = %d,%d,%d"), _pixel_position, pixel_position_adjust, colour.R,colour.G,colour.B);

    //   uint8_t pixels_in_crgb16palette2 = GetColoursInCRGB16Palette(palette_id);
    // if(_pixel_position==0)
    // {
    //   for(uint16_t pi=0;pi<256;pi++)
    //   {
    //     fastled_col = ColorFromPalette( pSEGMENT_I(segIdx).palette_container->CRGB16Palette16_Palette.data, pi, brightness_scale, NOBLEND);// (pCONT_lAni->paletteBlend == 3)? NOBLEND:LINEARBLEND);
    //     RgbcctColor c1 = RgbcctColor(fastled_col.r, fastled_col.g, fastled_col.b) ;// RgbcctColor::GetRgbcctFromU32Colour(fastled_col32);
    //     ALOG_INF(PSTR("c1[%d|%d] = \t%d\t%d\t%d"), pixels_in_crgb16palette2, pi, c1.R,c1.G,c1.B);
    //   }
    // }

    return colour;

  } // END of CRGBPalette's


  /**************************************************************
   * 
   * PALETTELIST_FIXED__IDS
   * 
  ***************************************************************/
  else
  if(
    ((palette_id >= PALETTELIST_FIXED_COLOURFUL_DEFAULT__ID) && (palette_id < PALETTELIST_FIXED_LENGTH__ID))
  ){  

    uint16_t palette_adjusted_id = palette_id - PALETTELIST_FIXED_COLOURFUL_DEFAULT__ID; // adjust back into correct indexing
    
    /**
     * @brief If scaling is needed
     **/
    if(flag_spanned_segment)
    {

      colour = SubGet_Encoded_PaletteList_Colour_Gradient(
        palette_adjusted_id,
        palette_buffer,
        _pixel_position,  
        encoded_value,
        flag_spanned_segment,
        flag_wrap_hard_edge,
        flag_crgb_exact_colour
      );

    }
    /****
     * Get colours the direct way by reading the memory address
    */
    else
    {

      colour = SubGet_Encoded_PaletteList_Colour(
        palette_adjusted_id,
        palette_buffer,
        _pixel_position,  
        encoded_value
      );
    }

    return colour;

  } // end of my palettes



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
    
    // if(flag_spanned_segment) // added to fix twinkleup, likely cause other issues
    // {
    //   _pixel_position = (_pixel_position*255)/(pCONT_lAni->_virtualSegmentLength -1);  // This scales out segment_index to segment_length as 0 to 255
    // }
    
    return SubGet_Encoded_UserPalette_Colour(
      palette_adjusted_id,
      palette_buffer,
      _pixel_position,  
      encoded_value
    );
    
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

    if(adjusted_id < RGBCCTCOLOURS_SIZE)
    {
      return pCONT_lAni->segments[segIdx].rgbcctcolors[adjusted_id];
    }
  }


  /**************************************************************
   * 
   * PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR__IDS
   * 
  ***************************************************************/
  else
  if(
    (palette_id >= PALETTELIST_FIXED_SINGLE_COLOUR__RED__ID) && (palette_id < PALETTELIST_FIXED_SINGLE_COLOUR__LENGTH__ID)
  ){  

    uint8_t adjusted_id  = palette_id - PALETTELIST_FIXED_SINGLE_COLOUR__RED__ID;
    const uint8_t* data  = PM_STATIC_SINGLE_COLOURS__DATA;
    uint8_t adjust_buf_i =  adjusted_id*3;
    return RgbcctColor(data[adjust_buf_i], data[adjust_buf_i+1], data[adjust_buf_i+2]);

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






















/**
 * @brief private function
 * 
 * By having this as a subfunction, it should enable iterative multiple calls function the main GetColour
 * 
 * @param palette_id 
 * @param seg_i 
 * @return RgbcctColor 
 */
RgbcctColor mPalette::SubGet_Encoded_PaletteList_Colour(
  uint16_t palette_adjusted_id,
  uint8_t* palette_buffer,
  uint16_t _pixel_position,  
  uint8_t* encoded_value
){

  RgbcctColor colour = RgbcctColor(0);

  if(palette_adjusted_id>mPaletteI->static_palettes.size())
  {
    ALOG_ERR(PSTR("PALETTE EXCEEDS VECTOR SIZE")); delay(2000);
    return colour;
  }

  STATIC_PALETTE *ptr = &mPaletteI->static_palettes[palette_adjusted_id];

  uint8_t pixels_in_map = GetNumberOfColoursInPalette(palette_adjusted_id);  
  uint8_t colour_width  = GetEncodedColourWidth(ptr->encoding); 


  // Serial.println(pixels_in_map);
  uint8_t remainder = 0;
  if(pixels_in_map != 0)
  {
    remainder = _pixel_position%pixels_in_map;
  }
  // uint8_t remainder_scaled255 = map(remainder, 0,pixels_in_segment-1, 0,255);

  // Reminder gives me each pixel, no gradient
  uint16_t pixel_position = remainder;   // THIS IS BEING PASSED IN AND REUSED??

  
  //something like this does need adding, but the previous and next pixels will be needed for the blend so perhaps make another if
  // if (flag_map_scaling) pixel_position = (_pixel_position*255)/(pCONT_lAni->_virtualSegmentLength -1);  

  // / Perhaps I want to make it, if it should wrap then load up to the first 16 pixels into the CRGBPalette16 gradient and then exact per the same method as below.



  uint16_t index_relative = 0; // get expected pixel position

  #ifdef ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
  Serial.println(ptr->encoding.data, BIN);
  Serial.println(ptr->encoding.encoded_as_hsb_ids);
  Serial.println(ptr->encoding.index_scaled_to_segment);
  #endif // ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
  
  // /**
  //   Encoded as HSBID, must handle all index methods
  // **/
  // if(
  //   (ptr->encoding.encoded_as_hsb_ids)
  // ){

  //   // Serial.println("MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX__ID"); 

  //   // Get Start of Colour Information by adjusting for indexing
  //   index_relative = pixel_position*GetEncodedColourWidth(ptr->encoding); // get expected pixel position
  //   if(ptr->encoding.index_scaled_to_segment)
  //   {
  //     // If desired, return the index value
  //     if(encoded_value != nullptr){
  //       *encoded_value = palette_buffer[index_relative];
  //     }
  //     // Serial.println(*encoded_value);
  //     // Set the index to move beyond the indexing information
  //     index_relative++;
  //   };

  //   colour = RgbcctColor(
  //     GetHsbColour(palette_buffer[index_relative])
  //   );

  //   // if(pixel_position==0)
  //   // Serial.printf("%d|%d c %d %d %d\n\r", palette_id, pCONT_lAni->_segment_index_primary, colour.R, colour.G, colour.B);
    
  // }

  /**
    Encoded as Colour (RGBCCT or parts), must handle all index methods
  **/
  // else 
  if( // MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_GRADIENT__ID              //switch to bit masking
    (ptr->encoding.red_enabled)||
    (ptr->encoding.green_enabled)||
    (ptr->encoding.blue_enabled)||
    (ptr->encoding.white_cold_enabled)||
    (ptr->encoding.white_warm_enabled)
  ){
    
    // ALOG_INF(PSTR("enabled %d,%d,%d,%d,%d"), ptr->encoding.red_enabled, ptr->encoding.green_enabled, ptr->encoding.blue_enabled, ptr->encoding.white_cold_enabled, ptr->encoding.white_warm_enabled);
    // ALOG_INF(PSTR("palette_elements %d"), palette_elements[0]);   
    // ALOG_INF(PSTR("p = %d, r = %d, v = %d|%d, w=%d"), pixel_position, index_relative, palette_elements[index_relative], palette_elements[index_relative+1],colour_width);

    // Get Start of Colour Information by adjusting for indexing
    index_relative = pixel_position*GetEncodedColourWidth(ptr->encoding); // get expected pixel position
      
    // ALOG_INF(PSTR("index_relativeA=%d"),index_relative);

    if(ptr->encoding.index_scaled_to_segment)
    {
      // ALOG_INF(PSTR("index_relativeB=%d"),index_relative);
      if(encoded_value != nullptr){
        *encoded_value = palette_buffer[index_relative];
      }
      // Set the index to move beyond the indexing information
      index_relative++;
    }
      // ALOG_INF(PSTR("encoded_valueC=%d"), *encoded_value);
    
    // Get colour
    colour = RgbcctColor(
      ptr->encoding.red_enabled         ? palette_buffer[index_relative  ] : 0,
      ptr->encoding.green_enabled       ? palette_buffer[index_relative+1] : 0,
      ptr->encoding.blue_enabled        ? palette_buffer[index_relative+2] : 0,
      ptr->encoding.white_cold_enabled  ? palette_buffer[index_relative+3] : 0,
      ptr->encoding.white_warm_enabled  ? palette_buffer[index_relative+4] : 0
    );

  }


  return colour;

}


/**
 * @brief private function
 * 
 * By having this as a subfunction, it should enable iterative multiple calls function the main GetColour
 * 
 * @param palette_id 
 * @param seg_i 
 * @return RgbcctColor 
 */
RgbcctColor mPalette::SubGet_Encoded_PaletteList_Colour_Gradient
(
  uint16_t palette_adjusted_id,
  uint8_t* palette_buffer,
  uint16_t _pixel_position,
  uint8_t* encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
  bool     flag_map_scaling, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
  bool     flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
  bool     flag_crgb_exact_colour
){

  RgbcctColor colour = RgbcctColor(0);

  STATIC_PALETTE *ptr = &mPaletteI->static_palettes[palette_adjusted_id];

  uint8_t pixels_in_map = GetNumberOfColoursInPalette(palette_adjusted_id);  
  uint8_t colour_width  = GetEncodedColourWidth(ptr->encoding); 

  uint8_t pixel_position_adjust = _pixel_position;

  if (flag_map_scaling) pixel_position_adjust = (_pixel_position*255)/(pCONT_lAni->_virtualSegmentLength -1);  // This scales out segment_index to segment_length as 0 to 255
  // if (!flag_wrap_hard_edge){
  //   // uint8_t pixel_end_boundary = map(pixels_in_map-1, 0,pixels_in_map-1, 0,255); // depends on the number of pixels
  //   pixel_position_adjust = scale8(pixel_position_adjust, 240); //cut off blend at palette "end", 240, or 15/16*255=>240/255, so drop last 16th (15 to wrapped 0) gradient of colour
  // }
    // ALOG_INF(PSTR("_pixel_position %d/%d/%d"), _pixel_position, pCONT_lAni->_virtualSegmentLength, pixel_position_adjust);

  uint8_t edge_size = 255/pixels_in_map;
  uint8_t lower_limit = edge_size/2;
  uint8_t upper_limit = 255-(edge_size/2);

  bool palette_contains_gradient_indexes = false;

  std::vector<uint8_t> gradient_palettes;
  uint8_t encoded_value2 = 0;

  /**
   * @brief Only "index_scaled_to_segment" index is a gradient
   * 
   */
  if(ptr->encoding.index_scaled_to_segment)
  {
    for(uint8_t pix_i=0; pix_i<pixels_in_map; pix_i++)
    {
      SubGet_Encoded_PaletteList_Colour(palette_adjusted_id, palette_buffer, pix_i, &encoded_value2);
      gradient_palettes.push_back(encoded_value2);
    }
    // Serial.println("Aencoded_value2"); for(uint8_t v=0;v<gradient_palettes.size();v++){ Serial.printf("%d,",gradient_palettes[v]); }
  }else{
    for(uint8_t pix_i=0; pix_i<pixels_in_map; pix_i++)
    {
      gradient_palettes.push_back(map(pix_i, 0,pixels_in_map-1, lower_limit,upper_limit));
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
    // Stick with first colour
    lower_boundary_i = 0;
    upper_boundary_i = 1;
        lower_boundary_v = gradient_palettes[lower_boundary_i];
        upper_boundary_v = gradient_palettes[upper_boundary_i];
    progress = 0;
  }else
  if(desired_pixel_scaled > upper_limit)
  {
    // Stick with last colour
    lower_boundary_i = gradient_palettes.size()-1;
    upper_boundary_i = gradient_palettes.size(); //ignored
        lower_boundary_v = gradient_palettes[lower_boundary_i];
        upper_boundary_v = gradient_palettes[upper_boundary_i];
    progress = 0;
  }
  else // Search
  {
    lower_boundary_i = 0; //default for errors
    upper_boundary_i = 1;
    for(uint8_t v=0;v<gradient_palettes.size()-1;v++) // Using the indexes expect the final one
    {      
      // ALOG_INF(PSTR("v>>>>>>>> [%d]  %d<%d<%d"), v, gradient_palettes[v], pixel_position_adjust, gradient_palettes[v+1]);
      if(
        (pixel_position_adjust >= gradient_palettes[v])&&    // Greater than lower/current boundary
        (pixel_position_adjust < gradient_palettes[v+1])     // Smaller than upper/next boundary
      ){
        lower_boundary_i = v;
        upper_boundary_i = v+1;
        lower_boundary_v = gradient_palettes[lower_boundary_i];
        upper_boundary_v = gradient_palettes[upper_boundary_i];
        progress = mSupport::mapfloat(desired_pixel_scaled, lower_boundary_v,upper_boundary_v, 0.0f, 1.0f);
        // ALOG_INF(PSTR("WITHIN ======================BREAK %d   ||| %d larger %d? (%d|%d)"), v, desired_pixel_scaled, gradient_palettes[v], lower_boundary_i, upper_boundary_i);
        break; // found lower boundary index
      }
      // else{
      //   ALOG_INF(PSTR("OUTSIDE %d is still lower than %d for index %d"), gradient_palettes[v], desired_pixel_scaled, v);
      // }
    }
  }

  // ALOG_INF(PSTR("v>>>>>>>> [%d|%d]  %d|%d p%d"), _pixel_position, desired_pixel_scaled, lower_boundary_v, upper_boundary_v, (int)(progress*100));

  RgbcctColor lower_colour = SubGet_Encoded_PaletteList_Colour(palette_adjusted_id, palette_buffer, lower_boundary_i); 
  RgbcctColor upper_colour = SubGet_Encoded_PaletteList_Colour(palette_adjusted_id, palette_buffer, upper_boundary_i);

  colour = RgbcctColor::LinearBlend(lower_colour, upper_colour, progress);

  // lower_colour.debug_print("lower_colour");
  // upper_colour.debug_print("upper_colour");
  // out.debug_print("  out_colour");
  
  return colour;

}








int16_t mPalette::Get_Static_PaletteIDbyName(const char* c)
{

  if(*c=='\0'){
    return -1;
  }

  char buffer[100] = {0};
  int16_t index_found = -1;

  /**************************************************************
   * 
   * PALETTELIST_FIXED_CRGBPALETTE16__LENGTH__ID
   * 
  ***************************************************************/
  int16_t id = -1;
  for(
    uint8_t ii=0;
            ii<(PALETTELIST_FIXED_CRGBPALETTE16__LENGTH__ID-PALETTELIST_FIXED_CRGBPALETTE16__RAINBOW_COLOUR__ID);
            ii++
  ){    
    ALOG_INF( PSTR("s> %d %s \"%S\""), ii, c, PM_STATIC_CRGBPALETTE16_NAMES_CTR ); 
    if((id=mSupport::GetCommandID16_P(c, PM_STATIC_CRGBPALETTE16_NAMES_CTR))>=0)
    {
      ALOG_INF( PSTR("MATCH \"%s\" %d %d"), c, ii, id ); 
      return id+PALETTELIST_FIXED_CRGBPALETTE16__RAINBOW_COLOUR__ID;            
    }
  }

  /**************************************************************
   * 
   * PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT_LENGTH__ID
   * 
  ***************************************************************/
  for(
    uint8_t ii=0;
            ii<(PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT_LENGTH__ID-PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__SUNSET__ID);
            ii++
  ){    
    ALOG_INF( PSTR("s> %d %s \"%S\""), ii, c, PM_STATIC_CRGBPALETTE16_GRADIENT_NAMES_CTR ); 
    if((id=mSupport::GetCommandID16_P(c, PM_STATIC_CRGBPALETTE16_GRADIENT_NAMES_CTR))>=0)
    {
      ALOG_INF( PSTR("MATCH \"%s\" %d %d"), c, ii, id ); 
      return id+PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__SUNSET__ID;            
    }
  }

  /**************************************************************
   * 
   * PALETTELIST_FIXED_CRGBPALETTE16_USER__LENGTH__ID
   * 
  ***************************************************************/
  for(
    uint8_t ii=0;
            ii<(PALETTELIST_FIXED_CRGBPALETTE16_USER__LENGTH__ID-PALETTELIST_FIXED_CRGBPALETTE16__RANDOMISE_COLOURS_01__ID);
            ii++
  ){    
    ALOG_INF( PSTR("s> %d %s \"%S\""), ii, c, PM_STATIC_CRGBPALETTE16_RANDOMISED_NAMES_CTR ); 
    if((id=mSupport::GetCommandID16_P(c, PM_STATIC_CRGBPALETTE16_RANDOMISED_NAMES_CTR))>=0)
    {
      ALOG_INF( PSTR("MATCH \"%s\" %d %d"), c, ii, id ); 
      return id+PALETTELIST_FIXED_CRGBPALETTE16__RANDOMISE_COLOURS_01__ID;            
    }
  }

  /**************************************************************
   * 
   * PALETTELIST_FIXED_SINGLE_COLOUR__LENGTH__ID
   * 
  ***************************************************************/
  for(
    uint8_t ii=0;
            ii<(PALETTELIST_FIXED_SINGLE_COLOUR__LENGTH__ID-PALETTELIST_FIXED_SINGLE_COLOUR__RED__ID);
            ii++
  ){    
    ALOG_INF( PSTR("s> %d %s \"%S\""), ii, c, PM_STATIC_SINGLE_COLOUR_NAMES_CTR ); 
    if((id=mSupport::GetCommandID16_P(c, PM_STATIC_SINGLE_COLOUR_NAMES_CTR))>=0)
    {
      ALOG_INF( PSTR("MATCH \"%s\" %d %d"), c, ii, id ); 
      return id+PALETTELIST_FIXED_SINGLE_COLOUR__RED__ID;            
    }
  }

  /**************************************************************
   * 
   * PALETTELIST_FIXED__IDS
   * 
  ***************************************************************/
  for( // loops relative to exact palette id
    uint8_t ii=PALETTELIST_FIXED_COLOURFUL_DEFAULT__ID;
            ii<(PALETTELIST_FIXED_LENGTH__ID - PALETTELIST_FIXED_COLOURFUL_DEFAULT__ID);
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
  // if(WithinLimits(found_index, (uint8_t)0, (uint8_t)PALETTELIST_FIXED_LENGTH__ID)){
  //   return found_index;
  // }

  return -1; // Must be -1 to show name not found
}

void mPalette::addPalette(STATIC_PALETTE palette)
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


const char* mPalette::GetPaletteNameByID(uint8_t palette_id, char* buffer, uint8_t buflen)
{

  /**************************************************************
   * 
   * PALETTELIST_FIXED_CRGBPALETTE16__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= mPalette::PALETTELIST_FIXED_CRGBPALETTE16__RAINBOW_COLOUR__ID) && (palette_id < mPalette::PALETTELIST_FIXED_CRGBPALETTE16__LENGTH__ID))
  ){  
    uint16_t palette_id_adj = palette_id - mPalette::PALETTELIST_FIXED_CRGBPALETTE16__RAINBOW_COLOUR__ID;
    pCONT_sup->GetTextIndexed_P(buffer, buflen, palette_id_adj, PM_STATIC_CRGBPALETTE16_NAMES_CTR);   
    ALOG_DBG( PSTR("BName id%d|a%d \"%s\""), palette_id,palette_id_adj, buffer );
  }

  /**************************************************************
   * 
   * PALETTELIST_CRGBPALETTE16_GRADIENT___PALETTES__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= mPalette::PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__SUNSET__ID)    && (palette_id < mPalette::PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT_LENGTH__ID))
  ){  
    uint16_t adjusted_id = palette_id - mPalette::PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__SUNSET__ID;
    pCONT_sup->GetTextIndexed_P(buffer, buflen, adjusted_id, PM_STATIC_CRGBPALETTE16_GRADIENT_NAMES_CTR);   
  }

  /**************************************************************
   * 
   * PALETTELIST_SEGMENTS_STORED_FIXED_CRGBPALETTE16_PALETTES__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= mPalette::PALETTELIST_FIXED_CRGBPALETTE16__RANDOMISE_COLOURS_01__ID) && (palette_id < mPalette::PALETTELIST_FIXED_CRGBPALETTE16_USER__LENGTH__ID))
  ){  
    uint16_t adjusted_id = palette_id - mPalette::PALETTELIST_FIXED_CRGBPALETTE16__RANDOMISE_COLOURS_01__ID;
    pCONT_sup->GetTextIndexed_P(buffer, buflen, adjusted_id, PM_STATIC_CRGBPALETTE16_RANDOMISED_NAMES_CTR);   
  }

  /**************************************************************
   * 
   * PALETTELIST_FIXED_CRGBPALETTE16__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= mPalette::PALETTELIST_FIXED_SINGLE_COLOUR__RED__ID) && (palette_id < mPalette::PALETTELIST_FIXED_SINGLE_COLOUR__LENGTH__ID))
  ){  
    uint16_t palette_id_adj = palette_id - mPalette::PALETTELIST_FIXED_SINGLE_COLOUR__RED__ID;
    pCONT_sup->GetTextIndexed_P(buffer, buflen, palette_id_adj, PM_STATIC_SINGLE_COLOUR_NAMES_CTR);   
    ALOG_DBG( PSTR("BName id%d|a%d \"%s\""), palette_id,palette_id_adj, buffer );
  }

  /**************************************************************
   * 
   * PALETTELIST_FIXED__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= mPalette::PALETTELIST_FIXED_COLOURFUL_DEFAULT__ID) && (palette_id < mPalette::PALETTELIST_FIXED_LENGTH__ID))
  ){           
    uint16_t palette_id_adj = palette_id - mPalette::PALETTELIST_FIXED_COLOURFUL_DEFAULT__ID;
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



uint8_t mPalette::GetColoursInCRGB16Palette(uint16_t palette_id)
{

  if(
    ((palette_id >= mPalette::PALETTELIST_FIXED_CRGBPALETTE16__RAINBOW_COLOUR__ID) && (palette_id < mPalette::PALETTELIST_FIXED_CRGBPALETTE16__LENGTH__ID))
  ){  
    uint16_t palette_id_adj = palette_id - mPalette::PALETTELIST_FIXED_CRGBPALETTE16__RAINBOW_COLOUR__ID;
    // pCONT_sup->GetTextIndexed_P(buffer, buflen, palette_id_adj, PM_STATIC_CRGBPALETTE16_NAMES_CTR);   
    // ALOG_DBG( PSTR("BName id%d|a%d \"%s\""), palette_id,palette_id_adj, buffer );

    uint8_t colour_count = pSEGMENT_I(0).palette_container->CRGB16Palette16_Palette.encoded_index.size();

    // ALOG_INF(PSTR("colour_count=%d, pal%d"), colour_count, palette_id);

    return colour_count;

  }


  if(
    ((palette_id >= mPalette::PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__SUNSET__ID) && (palette_id < mPalette::PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT_LENGTH__ID))
  ){  
    uint16_t palette_id_adj = palette_id - mPalette::PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__SUNSET__ID;
    // pCONT_sup->GetTextIndexed_P(buffer, buflen, palette_id_adj, PM_STATIC_CRGBPALETTE16_NAMES_CTR);   
    // ALOG_DBG( PSTR("BName id%d|a%d \"%s\""), palette_id,palette_id_adj, buffer );

    return pSEGMENT_I(0).palette_container->CRGB16Palette16_Palette.encoded_index.size();

  }

  return 16;

}




/**
 * @brief private function
 * 
 * By having this as a subfunction, it should enable iterative multiple calls function the main GetColour
 * 
 * @param palette_id 
 * @param seg_i 
 * @return RgbcctColor 
 */
RgbcctColor mPalette::SubGet_Encoded_UserPalette_Colour(
  uint16_t custom_id,
  uint8_t* palette_buffer,
  uint16_t _pixel_position,  
  uint8_t* encoded_value
){

  RgbcctColor colour = RgbcctColor(0);

  // ALOG_INF(PSTR("SubGet_Encoded_UserPalette_Colour %d"), custom_id);

  uint8_t colour_width  = GetEncodedColourWidth(custom_palettes[custom_id].encoding); 
  uint8_t pixels_in_map = custom_palettes[custom_id].data.size()/colour_width;

  palette_buffer = &custom_palettes[custom_id].data[0];

  PALETTE_ENCODING_DATA encoding = custom_palettes[custom_id].encoding;
  
  // Serial.println(pixels_in_map);

  if(pixels_in_map != 0){
    _pixel_position = _pixel_position%pixels_in_map;  // /ERROR, REUSING NAME!!!
  }
  
  uint16_t index_relative = 0; // get expected pixel position

  #ifdef ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
  Serial.println(encoding.data, BIN);
  Serial.println(encoding.index_scaled_to_segment);
  #endif // ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
  

  if(
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
    index_relative = _pixel_position*GetEncodedColourWidth(encoding); // get expected pixel position
      
    // ALOG_INF(PSTR("index_relative=%d"), index_relative);

    if(encoding.index_scaled_to_segment)
    {
      // ALOG_INF(PSTR("index_relativeB=%d"),index_relative);
      if(encoded_value != nullptr){
        *encoded_value = palette_buffer[index_relative];
      }
      // Set the index to move beyond the indexing information
      index_relative++;
    }
      // ALOG_INF(PSTR("encoded_valueC=%d"), *encoded_value);
    
    // Get colour
    colour = RgbcctColor(
      encoding.red_enabled         ? palette_buffer[index_relative  ] : 0,
      encoding.green_enabled       ? palette_buffer[index_relative+1] : 0,
      encoding.blue_enabled        ? palette_buffer[index_relative+2] : 0,
      encoding.white_cold_enabled  ? palette_buffer[index_relative+3] : 0,
      encoding.white_warm_enabled  ? palette_buffer[index_relative+4] : 0
    );

  }


  return colour;

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

  if(encoding.index_exact){ encoded_colour_width++; }
  if(encoding.index_scaled_to_segment){ encoded_colour_width++; }
  if(encoding.index_is_trigger_value_exact){ encoded_colour_width++; }
  if(encoding.index_is_trigger_value_scaled100){ encoded_colour_width++; }
  
  if(encoding.encoded_as_hsb_ids){ encoded_colour_width++; }
  if(encoding.encoded_as_crgb_palette_16){ encoded_colour_width++; }
  if(encoding.encoded_as_crgb_palette_256){ encoded_colour_width++; }
  if(encoding.palette_can_be_modified){ encoded_colour_width++; }

  // ALOG_INF(PSTR("encoded_colour_width=%d"),encoded_colour_width);

  return encoded_colour_width;

}




#endif // header gaurd
