#include "mPalette.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE


/* Null, because instance will be initialized on demand. */
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
  init_PresetColourPalettes();
}

/*******************************************************************************************************************
********************************************************************************************************************
************ START OF PALETTE INITS *********************************************************************************
********************************************************************************************************************
********************************************************************************************************************/

void mPalette::init_PresetColourPalettes()
{
  
  ALOG_DBM( PSTR("init_PresetColourPalettes") );
  
  /**
   *  Static 
   * */
  init_ColourPalettes_Party_Default();
  init_ColourPalettes_Autumn_Red();
  init_ColourPalettes_Autumn_Green();
  init_ColourPalettes_Winter(); 
  init_ColourPalettes_Ocean_01();
  init_ColourPalettes_Rainbow();
  init_ColourPalettes_RainbowInverted();
  init_ColourPalettes_Holloween_OP();
  init_ColourPalettes_Holloween_OGP();
  init_ColourPalettes_Hot_Neon_Pink_With_Navy();
  init_ColourPalettes_Single_Fire_01();
  init_ColourPalettes_Flower_SweetPeas_01();
  init_ColourPalettes_Flower_SweetPeas_02();
  init_ColourPalettes_Pink_Purple_01();
  init_ColourPalettes_Pink_Purple_02();
  init_ColourPalettes_Pink_Purple_03();
  init_ColourPalettes_Pastel_01();
  init_ColourPalettes_Pastel_02();
  init_ColourPalettes_Gradient_01();
  init_ColourPalettes_Gradient_02();
  init_ColourPalettes_Gradient_Fire_01();
  init_ColourPalettes_Gradient_Pastel_Tones_Purple();
  init_ColourPalettes_Berry_Green();
  init_ColourPalettes_Christmas_01();
  init_ColourPalettes_Christmas_02();
  init_ColourPalettes_Christmas_03();
  init_ColourPalettes_Christmas_04();
  init_ColourPalettes_Christmas_05();
  init_ColourPalettes_Christmas_06();
  init_ColourPalettes_Christmas_07();
  init_ColourPalettes_Christmas_08();
  init_ColourPalettes_Christmas_09();
  init_ColourPalettes_Christmas_10();
  init_ColourPalettes_Christmas_11();
  init_ColourPalettes_Christmas_12();
  init_ColourPalettes_Christmas_13();
  init_ColourPalettes_Christmas_14();
  init_ColourPalettes_Christmas_15();
  init_ColourPalettes_Christmas_16();
  init_ColourPalettes_Christmas_17();
  init_ColourPalettes_Christmas_18();
  init_ColourPalettes_Christmas_19();
  init_ColourPalettes_Christmas_20();
  init_ColourPalettes_Christmas_21();
  init_ColourPalettes_Christmas_22();
  init_ColourPalettes_Christmas_23();
  init_ColourPalettes_Christmas_24();
  init_ColourPalettes_Christmas_25();
  init_ColourPalettes_Christmas_26();
  init_ColourPalettes_Christmas_27();
  init_ColourPalettes_Christmas_28();
  init_ColourPalettes_Christmas_29();
  init_ColourPalettes_Christmas_30();
  init_ColourPalettes_Sky_Glow_01();
  init_ColourPalettes_Sunrise_01();
  init_ColourPalettes_Sunrise_02();
  init_ColourPalettes_Sunrise_03();
  init_ColourPalettes_Sunrise_04();
  init_ColourPalettes_Sunset_01();
  init_ColourPalettes_Sunset_02();
  init_ColourPalettes_Candle_Flame_01();
  init_ColourPalettes_Custom_User_01();
  init_ColourPalettes_Gradient_SunLevel_Group01_01();
  init_ColourPalettes_Gradient_SunLevel_Group01_02();
  init_ColourPalettes_Gradient_SunLevel_Group01_03();
  init_ColourPalettes_Gradient_SunLevel_Group01_04();
  init_ColourPalettes_Gradient_SunLevel_Group01_05();
  init_ColourPalettes_Gradient_SunLevel_Group01_06();
  init_ColourPalettes_Gradient_SunLevel_Group01_07();
  init_ColourPalettes_Solid_RGBCCT_Sun_Elevation_With_Degrees_Index_01();
  init_ColourPalettes_Solid_RGBCCT_Sun_Elevation_With_Degrees_Index_02();
  init_ColourPalettes_Solid_RGBCCT_Sun_Elevation_With_Degrees_Index_03();
  init_ColourPalettes_Solid_RGBCCT_Sun_Elevation_With_Degrees_Index_04();
  init_ColourPalettes_Solid_RGBCCT_Sun_Elevation_With_Degrees_Index_Dual_Colours_01();
  
  /**
   *  Variable HSBID 
   * */
  for(uint8_t id=0;id<(PALETTELIST_VARIABLE_HSBID_LENGTH__ID-PALETTELIST_VARIABLE_HSBID_01__ID);id++){
    init_PresetColourPalettes_HSBID_UserFill(id);
  }

  /**
   *  Variable HSBID 
   * */
  for(uint8_t id=0;id<(PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID-PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_01__ID);id++){
    init_PresetColourPalettes_User_RGBCCT_Fill(id);
  }

  /**
   *  Variable Encoded Generic 
   * */
  for(uint8_t id=0;id<(PALETTELIST_VARIABLE_GENERIC_LENGTH__ID-PALETTELIST_VARIABLE_GENERIC_01__ID);id++){
    init_PresetColourPalettes_User_Generic_Fill(0);
  }

  // DEBUG_LINE_HERE;
}


void mPalette::init_ColourPalettes_Party_Default(){
  palettelist.ptr                      = &palettelist.party_default;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_PARTY_DEFAULT__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_PARTY_DEFAULT_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_PARTY_DEFAULT_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_PARTY_DEFAULT_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_PARTY_DEFAULT_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Holloween_OP(){
  palettelist.ptr                      = &palettelist.holloween_op;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_HOLLOWEEN_OP__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_HOLLOWEEN_OP_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_HOLLOWEEN_OP_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_HOLLOWEEN_OP_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_HOLLOWEEN_OP_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Holloween_OGP(){
  palettelist.ptr                      = &palettelist.holloween_ogp;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_HOLLOWEEN_OGP__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_HOLLOWEEN_OGP_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_HOLLOWEEN_OGP_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_HOLLOWEEN_OGP_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_HOLLOWEEN_OGP_NAME_CTR; 
}


void mPalette::init_ColourPalettes_Hot_Neon_Pink_With_Navy(){
  palettelist.ptr                      = &palettelist.hot_pink_neon_with_navy;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_HOT_PINK_NEON_WITH_NAVY__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_HOT_PINK_NEON_WITH_NAVY_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_HOT_PINK_NEON_WITH_NAVY_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_HOT_PINK_NEON_WITH_NAVY_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_HOT_PINK_NEON_WITH_NAVY_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Single_Fire_01(){
  palettelist.ptr                      = &palettelist.single_fire_01;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_SINGLE_FIRE_01__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_SINGLE_FIRE_01_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_SINGLE_FIRE_01_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_SINGLE_FIRE_01_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_SINGLE_FIRE_01_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Flower_SweetPeas_01(){
  palettelist.ptr                      = &palettelist.flower_sweetpeas_01;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_FLOWER_SWEATPEAS_01__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_FLOWER_SWEATPEAS_01_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_FLOWER_SWEATPEAS_01_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_FLOWER_SWEATPEAS_01_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_FLOWER_SWEATPEAS_01_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Flower_SweetPeas_02(){
  palettelist.ptr                      = &palettelist.flower_sweetpeas_02;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_FLOWER_SWEATPEAS_02__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_FLOWER_SWEATPEAS_02_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_FLOWER_SWEATPEAS_02_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_FLOWER_SWEATPEAS_02_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_FLOWER_SWEATPEAS_02_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Pink_Purple_01(){
  palettelist.ptr                      = &palettelist.pink_purple_01;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_PINK_PURPLE_01__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_PINK_PURPLE_01_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_PINK_PURPLE_01_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_PINK_PURPLE_01_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_PINK_PURPLE_01_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Pink_Purple_02(){
  palettelist.ptr                      = &palettelist.pink_purple_02;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_PINK_PURPLE_02__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_PINK_PURPLE_02_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_PINK_PURPLE_02_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_PINK_PURPLE_02_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_PINK_PURPLE_02_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Pink_Purple_03(){
  palettelist.ptr                      = &palettelist.pink_purple_03;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_PINK_PURPLE_03__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_PINK_PURPLE_03_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_PINK_PURPLE_03_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_PINK_PURPLE_03_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_PINK_PURPLE_03_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Pastel_01(){
  palettelist.ptr                      = &palettelist.pastel_01;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_PASTEL_01__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_PASTEL_01_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_PASTEL_01_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_PASTEL_01_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_PASTEL_01_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Pastel_02(){
  palettelist.ptr                      = &palettelist.pastel_02;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_PASTEL_02__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_PASTEL_02_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_PASTEL_02_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_PASTEL_02_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_PASTEL_02_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Gradient_01(){
  palettelist.ptr                      = &palettelist.gradient_01;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_GRADIENT_01__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_GRADIENT_01_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_GRADIENT_01_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_GRADIENT_01_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_GRADIENT_01_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Gradient_02(){
  palettelist.ptr                      = &palettelist.gradient_02;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_GRADIENT_02__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_GRADIENT_02_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_GRADIENT_02_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_GRADIENT_02_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_GRADIENT_02_NAME_CTR;
}

void mPalette::init_ColourPalettes_Gradient_Fire_01(){
  palettelist.ptr                      = &palettelist.gradient_fire_01;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_GRADIENT_FIRE_01__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_GRADIENT_FIRE_01_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_GRADIENT_FIRE_01_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_GRADIENT_FIRE_01_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_GRADIENT_FIRE_01_NAME_CTR;
}

void mPalette::init_ColourPalettes_Gradient_Pastel_Tones_Purple(){  
  palettelist.ptr                      = &palettelist.gradient_pastel_tones_purple;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_GRADIENT_PASTEL_TONES_PURPLE__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_GRADIENT_PASTEL_TONES_PURPLE_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_GRADIENT_PASTEL_TONES_PURPLE_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_GRADIENT_PASTEL_TONES_PURPLE_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_GRADIENT_PASTEL_TONES_PURPLE_NAME_CTR;
}

void mPalette::init_ColourPalettes_Berry_Green(){
  palettelist.ptr                      = &palettelist.berry_green;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_BERRY_GREEN__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_BERRY_GREEN_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_BERRY_GREEN_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_BERRY_GREEN_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_BERRY_GREEN_NAME_CTR; 
}


void mPalette::init_ColourPalettes_Christmas_01(){
  palettelist.ptr                      = &palettelist.christmas_01;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_01__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_01_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_01_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_01_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_01_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_02(){
  palettelist.ptr                      = &palettelist.christmas_02;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_02__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_02_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_02_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_02_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_02_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_03(){
  palettelist.ptr                      = &palettelist.christmas_03;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_03__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_03_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_03_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_03_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_03_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_04(){
  palettelist.ptr                      = &palettelist.christmas_04;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_04__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_04_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_04_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_04_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_04_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_05(){
  palettelist.ptr                      = &palettelist.christmas_05;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_05__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_05_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_05_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_05_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_05_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_06(){
  palettelist.ptr                      = &palettelist.christmas_06;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_06__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_06_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_06_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_06_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_06_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_07(){
  palettelist.ptr                      = &palettelist.christmas_07;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_07__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_07_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_07_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_07_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_07_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_08(){
  palettelist.ptr                      = &palettelist.christmas_08;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_08__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_08_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_08_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_08_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_08_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_09(){
  palettelist.ptr                      = &palettelist.christmas_09;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_09__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_09_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_09_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_09_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_09_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_10(){
  palettelist.ptr                      = &palettelist.christmas_10;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_10__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_10_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_10_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_10_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_10_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_11(){
  palettelist.ptr                      = &palettelist.christmas_11;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_11__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_11_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_11_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_11_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_11_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_12(){
  palettelist.ptr                      = &palettelist.christmas_12;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_12__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_12_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_12_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_12_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_12_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_13(){
  palettelist.ptr                      = &palettelist.christmas_13;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_13__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_13_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_13_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_13_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_13_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_14(){
  palettelist.ptr                      = &palettelist.christmas_14;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_14__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_14_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_14_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_14_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_14_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_15(){
  palettelist.ptr                      = &palettelist.christmas_15;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_15__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_15_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_15_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_15_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_15_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_16(){
  palettelist.ptr                      = &palettelist.christmas_16;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_16__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_16_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_16_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_16_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_16_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_17(){
  palettelist.ptr                      = &palettelist.christmas_17;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_17__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_17_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_17_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_17_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_17_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_18(){
  palettelist.ptr                      = &palettelist.christmas_18;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_18__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_18_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_18_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_18_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_18_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_19(){
  palettelist.ptr                      = &palettelist.christmas_19;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_19__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_19_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_19_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_19_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_19_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_20(){
  palettelist.ptr                      = &palettelist.christmas_20;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_20__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_20_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_20_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_20_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_20_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_21(){
  palettelist.ptr                      = &palettelist.christmas_21;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_21__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_21_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_21_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_21_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_21_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_22(){
  palettelist.ptr                      = &palettelist.christmas_22;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_22__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_22_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_22_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_22_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_22_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_23(){
  palettelist.ptr                      = &palettelist.christmas_23;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_23__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_23_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_23_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_23_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_23_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_24(){
  palettelist.ptr                      = &palettelist.christmas_24;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_24__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_24_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_24_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_24_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_24_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_25(){
  palettelist.ptr                      = &palettelist.christmas_25;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_25__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_25_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_25_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_25_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_25_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_26(){
  palettelist.ptr                      = &palettelist.christmas_26;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_26__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_26_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_26_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_26_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_26_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_27(){
  palettelist.ptr                      = &palettelist.christmas_27;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_27__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_27_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_27_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_27_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_27_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_28(){
  palettelist.ptr                      = &palettelist.christmas_28;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_28__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_28_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_28_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_28_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_28_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_29(){
  palettelist.ptr                      = &palettelist.christmas_29;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_29__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_29_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_29_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_29_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_29_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Christmas_30(){
  palettelist.ptr                      = &palettelist.christmas_30;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CHRISTMAS_30__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CHRISTMAS_30_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CHRISTMAS_30_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CHRISTMAS_30_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CHRISTMAS_30_NAME_CTR; 
}


void mPalette::init_ColourPalettes_Sunrise_01(){  
  palettelist.ptr                      = &palettelist.sunrise_01;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_SUNRISE_01__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_SUNRISE_01_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_SUNRISE_01_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_SUNRISE_01_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_SUNRISE_01_NAME_CTR;
}

void mPalette::init_ColourPalettes_Sunrise_02(){
  palettelist.ptr                      = &palettelist.sunrise_02;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_SUNRISE_02__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_SUNRISE_02_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_SUNRISE_02_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_SUNRISE_02_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_SUNRISE_02_NAME_CTR;
}

void mPalette::init_ColourPalettes_Sunrise_03(){
  palettelist.ptr                      = &palettelist.sunrise_03;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_SUNRISE_03__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_SUNRISE_03_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_SUNRISE_03_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_SUNRISE_03_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_SUNRISE_03_NAME_CTR;
}

void mPalette::init_ColourPalettes_Sunrise_04(){
  palettelist.ptr                      = &palettelist.sunrise_04;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_SUNRISE_04__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_SUNRISE_04_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_SUNRISE_04_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_SUNRISE_04_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_SUNRISE_04_NAME_CTR;
}

void mPalette::init_ColourPalettes_Sunset_01(){
  palettelist.ptr                      = &palettelist.sunset_01;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_SUNSET_01__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_SUNSET_01_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_SUNSET_01_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_SUNSET_01_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_SUNSET_01_NAME_CTR;
}

void mPalette::init_ColourPalettes_Sunset_02(){
  palettelist.ptr                      = &palettelist.sunset_02;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_SUNSET_02__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_SUNSET_02_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_SUNSET_02_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_SUNSET_02_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_SUNSET_02_NAME_CTR;
}

void mPalette::init_ColourPalettes_Sky_Glow_01(){
  palettelist.ptr                      = &palettelist.sky_glow_01;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_SKY_GLOW_01__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_SKY_GLOW_01_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_SKY_GLOW_01_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_SKY_GLOW_01_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_SKY_GLOW_01_NAME_CTR;
}

void mPalette::init_ColourPalettes_Candle_Flame_01(){
  palettelist.ptr                      = &palettelist.candle_flame_01;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CANDLE_FLAME_01__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CANDLE_FLAME_01_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CANDLE_FLAME_01_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CANDLE_FLAME_01_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CANDLE_FLAME_01_NAME_CTR;
}

void mPalette::init_ColourPalettes_Custom_User_01(){
  palettelist.ptr                      = &palettelist.custom_user_01;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_CUSTOM_USER_01__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_CUSTOM_USER_01_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_CUSTOM_USER_01_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_CUSTOM_USER_01_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_CUSTOM_USER_01_NAME_CTR;
}

void mPalette::init_ColourPalettes_Autumn_Red(){
  palettelist.ptr                      = &palettelist.autumn_red;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_AUTUMN_RED__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_AUTUMN_RED_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_AUTUMN_RED_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_AUTUMN_RED_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_AUTUMN_RED_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Autumn_Green(){
  palettelist.ptr                      = &palettelist.autumn_green;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_AUTUMN_GREEN__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_AUTUMN_GREEN_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_AUTUMN_GREEN_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_AUTUMN_GREEN_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_AUTUMN_GREEN_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Winter(){  
  palettelist.ptr                      = &palettelist.winter;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_WINTER__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_WINTER_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_WINTER_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_WINTER_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_WINTER_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Ocean_01(){
  palettelist.ptr                      = &palettelist.ocean_01;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_OCEAN_01__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_OCEAN_01_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_OCEAN_01_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_OCEAN_01_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_OCEAN_01_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Rainbow(){
  palettelist.ptr                      = &palettelist.rainbow;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_RAINBOW__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_RAINBOW_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_RAINBOW_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_RAINBOW_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_RAINBOW_NAME_CTR; 
}

void mPalette::init_ColourPalettes_RainbowInverted(){
  palettelist.ptr                      = &palettelist.rainbow_inverted;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_RAINBOW_INVERTED__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_RAINBOW_INVERTED_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_RAINBOW_INVERTED_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_RAINBOW_INVERTED_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_RAINBOW_INVERTED_NAME_CTR; 
}

void mPalette::init_ColourPalettes_Gradient_SunLevel_Group01_01(){
  palettelist.ptr                      = &palettelist.gradient_sunlevel_group01_01;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_01__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_GRADIENT_SUNLEVEL_GROUP01_01_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_GRADIENT_SUNLEVEL_GROUP01_01_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_GRADIENT_SUNLEVEL_GROUP01_01_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_GRADIENT_SUNLEVEL_GROUP01_01_NAME_CTR;
}

void mPalette::init_ColourPalettes_Gradient_SunLevel_Group01_02(){
  palettelist.ptr                      = &palettelist.gradient_sunlevel_group01_02;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_02__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_02_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_02_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_02_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_02_NAME_CTR;
}

void mPalette::init_ColourPalettes_Gradient_SunLevel_Group01_03(){
  palettelist.ptr                      = &palettelist.gradient_sunlevel_group01_03;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_03__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_03_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_03_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_03_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_03_NAME_CTR;
}

void mPalette::init_ColourPalettes_Gradient_SunLevel_Group01_04(){
  palettelist.ptr                      = &palettelist.gradient_sunlevel_group01_04;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_04__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_04_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_04_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_04_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_04_NAME_CTR;
}

void mPalette::init_ColourPalettes_Gradient_SunLevel_Group01_05(){
  palettelist.ptr                      = &palettelist.gradient_sunlevel_group01_05;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_05__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_05_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_05_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_05_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_05_NAME_CTR;
}

void mPalette::init_ColourPalettes_Gradient_SunLevel_Group01_06(){
  palettelist.ptr                      = &palettelist.gradient_sunlevel_group01_06;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_06__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_06_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_06_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_06_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_06_NAME_CTR;
}

void mPalette::init_ColourPalettes_Gradient_SunLevel_Group01_07(){
  palettelist.ptr                      = &palettelist.gradient_sunlevel_group01_07;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_07__ID;
  palettelist.ptr->data                = (uint8_t*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_COLOUR_MAP__DATA;
  palettelist.ptr->data_length         = sizeof(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_COLOUR_MAP__DATA);
  palettelist.ptr->encoding.data       = D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_NAME_CTR;
}

void mPalette::init_ColourPalettes_Solid_RGBCCT_Sun_Elevation_With_Degrees_Index_01(){
  palettelist.ptr                      = &palettelist.gradient_solid_rgbcct_sun_elevation_with_degrees_in_index_01;  
  palettelist.ptr->id                  = PALETTELIST_STATIC__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01__ID;
  palettelist.ptr->data                = (uint8_t*)PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_01__DATA;
  palettelist.ptr->data_length         = sizeof(PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_01__DATA);
  palettelist.ptr->encoding.data       = D__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_01__ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_01__DATA;
}

void mPalette::init_ColourPalettes_Solid_RGBCCT_Sun_Elevation_With_Degrees_Index_02(){
  palettelist.ptr                      = &palettelist.gradient_solid_rgbcct_sun_elevation_with_degrees_in_index_02;  
  palettelist.ptr->id                  = PALETTELIST_STATIC__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_02__ID;
  palettelist.ptr->data                = (uint8_t*)PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_02__DATA;
  palettelist.ptr->data_length         = sizeof(PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_02__DATA);
  palettelist.ptr->encoding.data       = D__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_02__ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_02__DATA;
}

void mPalette::init_ColourPalettes_Solid_RGBCCT_Sun_Elevation_With_Degrees_Index_03(){
  palettelist.ptr                      = &palettelist.gradient_solid_rgbcct_sun_elevation_with_degrees_in_index_03;  
  palettelist.ptr->id                  = PALETTELIST_STATIC__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_03__ID;
  palettelist.ptr->data                = (uint8_t*)PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_03__DATA;
  palettelist.ptr->data_length         = sizeof(PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_03__DATA);
  palettelist.ptr->encoding.data       = D__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_03__ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_03__DATA;
}

void mPalette::init_ColourPalettes_Solid_RGBCCT_Sun_Elevation_With_Degrees_Index_04(){
  palettelist.ptr                      = &palettelist.gradient_solid_rgbcct_sun_elevation_with_degrees_in_index_04;  
  palettelist.ptr->id                  = PALETTELIST_STATIC__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_04__ID;
  palettelist.ptr->data                = (uint8_t*)PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_04__DATA;
  palettelist.ptr->data_length         = sizeof(PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_04__DATA);
  palettelist.ptr->encoding.data       = D__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_04__ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_04__DATA;
}


void mPalette::init_ColourPalettes_Solid_RGBCCT_Sun_Elevation_With_Degrees_Index_Dual_Colours_01(){
  palettelist.ptr                      = &palettelist.gradient_solid_rgbcct_sun_elevation_with_degrees_in_index_dual_colours_01;  
  palettelist.ptr->id                  = PALETTELIST_STATIC_DUAL_COLOUR_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01__ID;
  palettelist.ptr->data                = (uint8_t*)PM__DUAL_COLOUR_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01__DATA;
  palettelist.ptr->data_length         = sizeof(PM__DUAL_COLOUR_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01__DATA);
  palettelist.ptr->encoding.data       = D__DUAL_COLOUR_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01__ENCODING;
  palettelist.ptr->friendly_name_ctr   = (char*)PM__DUAL_COLOUR_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01__CTR;
}






void mPalette::init_PresetColourPalettes_HSBID_UserFill(uint8_t id){

  palettelist.ptr     = &palettelist.hsbid_users[id];
  palettelist.ptr->id = id+PALETTELIST_VARIABLE_HSBID_01__ID;

  // Point to memory location
  // This should be changed, the settings removed out of this. Instead, have them preloaded with internal (to palette.h) defaults and then another function that loads in these (double load, but makes the class self contained!!)
  palettelist.ptr->data = &pCONT_set->Settings.animation_settings.palette_hsbid_users_colour_map[id*20];

  palettelist.ptr->data_length = 20;//Get

  // to be moved into devicelist
  palettelist.ptr->friendly_name_ctr = nullptr; // Used DeviceName
  
  palettelist.ptr->encoding.data       = D_PALETTE_HSBID_01_ENCODING;
  // fill memory location to defaults
  uint8_t get_pixels_used = GetDefaultColourPaletteUserIDsCount(id);

  auto id_ptr = GetDefaultColourPaletteUserIDs_P(id);

  //clear back to none_id
  memset(palettelist.ptr->data,COLOUR_MAP_NONE__ID,sizeof(uint8_t)*20);

  // Read from PROGMEM the defaults
  memcpy_P(palettelist.ptr->data,id_ptr,sizeof(uint8_t)*get_pixels_used);

  //set pixels in palette
  palettelist.ptr->data_length = get_pixels_used;

}


void mPalette::init_PresetColourPalettes_User_RGBCCT_Fill(uint8_t id){

  palettelist.ptr     = &palettelist.rgbcct_segment_colour_users[id];
  palettelist.ptr->id = id+PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_01__ID;

  // Point to memory location
  palettelist.ptr->data = &pCONT_set->Settings.animation_settings.palette_rgbcct_users_colour_map[id*5];
  palettelist.ptr->data_length = 5; // each is 5 bytes wide, 1 pixel = 5 bytes total

  // to be moved into devicelist
  palettelist.ptr->friendly_name_ctr = nullptr;
  
  palettelist.ptr->encoding.data       = PALETTE_ENCODING_INCLUDES_RGBCCT_NO_INDEX;

}


void mPalette::init_PresetColourPalettes_User_Generic_Fill(uint8_t id)
{

/*
{
  "ColourPalette": 15,
  "PaletteEdit": {
    "ColourPalette": 15,
    "Data": [
      3,
      6,
      0,
      0,
      0,
      255,0,0,
      0,255,0,
      0,0,255
    ]
  },
  "AnimationMode": "Effects",
  "Effects": {
    "Function": "Static"
  },
  "Rate":1
}
*/

  /**
   * [0] pixel_size, used with map_type to generate map_size (ie data_length = pixel_width*pixel_count)
   * [1] map_type      eg (6=rgb no_index, )
   * [2] 
   * [3]
   * [4]
   * [5->length] Encoded palette colour values
   * */

  palettelist.ptr     = &palettelist.encoded_users;
  palettelist.ptr->id = id+PALETTELIST_VARIABLE_GENERIC_01__ID;

  //colour map size be calculated by map type * pixel count?

  // palettelist.ptr->data_length = pCONT_set->Settings.animation_settings.palette_encoded_users_colour_map[0] < 100 ?  pCONT_set->Settings.animation_settings.palette_encoded_users_colour_map[0] : 0; 
  // palettelist.ptr->flags.fMapIDs_Type = pCONT_set->Settings.animation_settings.palette_encoded_users_colour_map[1]; 
  //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ This will need to be extended to probably use both of the first bytes

  uint8_t pixel_width = GetEncodedColourWidth(palettelist.ptr->encoding);
  // AddLog(LOG_LEVEL_TEST, PSTR("pixel_width %d"),pixel_width);
  palettelist.ptr->data_length = pixel_width*pCONT_set->Settings.animation_settings.palette_encoded_users_colour_map[0];
  


  // 
  // palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID;

  //need to create "palette encoded" to "saved buffer" function ie decoder
  //Starts after the encoded info is taken out
  palettelist.ptr->data = &pCONT_set->Settings.animation_settings.palette_encoded_users_colour_map[5];//id*5];

  // to be moved into devicelist
  palettelist.ptr->friendly_name_ctr = nullptr;//pCONT_set->Settings.animation_settings.palette_user_variable_name_list_ctr;//&palettelist_variable_users_ctr[ptr->id][0];
  
  // AddLog(LOG_LEVEL_TEST, PSTR("init_PresetColourPalettes_User_Generic_Fill %d"),palettelist.ptr->data[0]);
  // AddLog(LOG_LEVEL_TEST, PSTR("init_PresetColourPalettes_User_Generic_Fill size %d"),palettelist.ptr->data_length);

  // AddLog(LOG_LEVEL_TEST, PSTR("pixels in map test size %d"),GetNumberOfColoursInPalette(palettelist.ptr));
  
  //     RgbcctColor colour = mPaletteI->GetColourFromPalette(mPaletteI->GetPalettePointerByID(15),0);

  //     AddLog(LOG_LEVEL_TEST, PSTR("colour_pal  =%d,%d,%d,%d,%d"), colour.R, colour.G, colour.B, colour.WW, colour.WC);


}



/*******************************************************************************************************************
********************************************************************************************************************
************ START OF Shared functions ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/


mPalette::PALETTELIST::PALETTE* mPalette::GetPalettePointerByID(uint8_t id)
{

    // ALOG_INF(PSTR("GetPalettePointerByID=%d DEFAULT"),id);

  switch(id){
    default:
    
    #ifdef ENABLE_LOG_LEVEL_ERROR
    ALOG_DBM(PSTR("GetPalettePointerByID=%d DEFAULT"),id);
    #endif // ENABLE_LOG_LEVEL_INFO


    
    /**
     * Variable User HSBID
     * */
    case PALETTELIST_VARIABLE_HSBID_01__ID: return &palettelist.hsbid_users[0];
    case PALETTELIST_VARIABLE_HSBID_02__ID: return &palettelist.hsbid_users[1];
    case PALETTELIST_VARIABLE_HSBID_03__ID: return &palettelist.hsbid_users[2];
    case PALETTELIST_VARIABLE_HSBID_04__ID: return &palettelist.hsbid_users[3];
    case PALETTELIST_VARIABLE_HSBID_05__ID: return &palettelist.hsbid_users[4];
    case PALETTELIST_VARIABLE_HSBID_06__ID: return &palettelist.hsbid_users[5];
    case PALETTELIST_VARIABLE_HSBID_07__ID: return &palettelist.hsbid_users[6];
    case PALETTELIST_VARIABLE_HSBID_08__ID: return &palettelist.hsbid_users[7];
    case PALETTELIST_VARIABLE_HSBID_09__ID: return &palettelist.hsbid_users[8];
    case PALETTELIST_VARIABLE_HSBID_10__ID: return &palettelist.hsbid_users[9];
    /**
     * Variable User RGBCCT
     * */
    case PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_01__ID: return &palettelist.rgbcct_segment_colour_users[0];
    case PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_02__ID: return &palettelist.rgbcct_segment_colour_users[1];
    case PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_03__ID: return &palettelist.rgbcct_segment_colour_users[2];
    case PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_04__ID: return &palettelist.rgbcct_segment_colour_users[3];
    case PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_05__ID: return &palettelist.rgbcct_segment_colour_users[4];
    /**
     * Variable User Encoded Generic
     * */
    case PALETTELIST_VARIABLE_GENERIC_01__ID:       return &palettelist.encoded_users;



    /**
     * Static Encoded Palettes
     * */
    case PALETTELIST_STATIC_PARTY_DEFAULT__ID:    return &palettelist.party_default;
    case PALETTELIST_STATIC_HOLLOWEEN_OP__ID:    return &palettelist.holloween_op;
    case PALETTELIST_STATIC_HOLLOWEEN_OGP__ID:   return &palettelist.holloween_ogp;
    case PALETTELIST_STATIC_HOT_PINK_NEON_WITH_NAVY__ID:    return &palettelist.hot_pink_neon_with_navy;
    case PALETTELIST_STATIC_SINGLE_FIRE_01__ID:    return &palettelist.single_fire_01;
    case PALETTELIST_STATIC_FLOWER_SWEATPEAS_01__ID:    return &palettelist.flower_sweetpeas_01;
    case PALETTELIST_STATIC_FLOWER_SWEATPEAS_02__ID:    return &palettelist.flower_sweetpeas_02;
    case PALETTELIST_STATIC_PINK_PURPLE_01__ID:    return &palettelist.pink_purple_01;
    case PALETTELIST_STATIC_PINK_PURPLE_02__ID:    return &palettelist.pink_purple_02;
    case PALETTELIST_STATIC_PINK_PURPLE_03__ID:    return &palettelist.pink_purple_03;
    case PALETTELIST_STATIC_RAINBOW__ID:         return &palettelist.rainbow;
    case PALETTELIST_STATIC_RAINBOW_INVERTED__ID:return &palettelist.rainbow_inverted;
    case PALETTELIST_STATIC_PASTEL_01__ID:          return &palettelist.pastel_01;
    case PALETTELIST_STATIC_PASTEL_02__ID:          return &palettelist.pastel_02;
    case PALETTELIST_STATIC_WINTER__ID:          return &palettelist.winter;
    case PALETTELIST_STATIC_AUTUMN_GREEN__ID:          return &palettelist.autumn_green;
    case PALETTELIST_STATIC_AUTUMN_RED__ID:      return &palettelist.autumn_red;
    case PALETTELIST_STATIC_OCEAN_01__ID:        return &palettelist.ocean_01;
    case PALETTELIST_STATIC_GRADIENT_01__ID:     return &palettelist.gradient_01;
    case PALETTELIST_STATIC_GRADIENT_02__ID:     return &palettelist.gradient_02;
    case PALETTELIST_STATIC_GRADIENT_FIRE_01__ID:     return &palettelist.gradient_fire_01;
    case PALETTELIST_STATIC_GRADIENT_PASTEL_TONES_PURPLE__ID:     return &palettelist.gradient_pastel_tones_purple;
    case PALETTELIST_STATIC_BERRY_GREEN__ID:     return &palettelist.berry_green;
    case PALETTELIST_STATIC_SUNRISE_01__ID:      return &palettelist.sunrise_01;
    case PALETTELIST_STATIC_SUNRISE_02__ID:      return &palettelist.sunrise_02;
    case PALETTELIST_STATIC_SUNRISE_03__ID:      return &palettelist.sunrise_03;
    case PALETTELIST_STATIC_SUNRISE_04__ID:      return &palettelist.sunrise_04;
    case PALETTELIST_STATIC_SUNSET_01__ID:      return &palettelist.sunset_01;
    case PALETTELIST_STATIC_SUNSET_02__ID:      return &palettelist.sunset_02;
    case PALETTELIST_STATIC_SKY_GLOW_01__ID:      return &palettelist.sky_glow_01;
    case PALETTELIST_STATIC_CANDLE_FLAME_01__ID:      return &palettelist.candle_flame_01;
    case PALETTELIST_STATIC_CHRISTMAS_01__ID:     return &palettelist.christmas_01;
    case PALETTELIST_STATIC_CHRISTMAS_02__ID:     return &palettelist.christmas_02;
    case PALETTELIST_STATIC_CHRISTMAS_03__ID:     return &palettelist.christmas_03;
    case PALETTELIST_STATIC_CHRISTMAS_04__ID:     return &palettelist.christmas_04;
    case PALETTELIST_STATIC_CHRISTMAS_05__ID:     return &palettelist.christmas_05;
    case PALETTELIST_STATIC_CHRISTMAS_06__ID:     return &palettelist.christmas_06;
    case PALETTELIST_STATIC_CHRISTMAS_07__ID:     return &palettelist.christmas_07;
    case PALETTELIST_STATIC_CHRISTMAS_08__ID:     return &palettelist.christmas_08;
    case PALETTELIST_STATIC_CHRISTMAS_09__ID:     return &palettelist.christmas_09;
    case PALETTELIST_STATIC_CHRISTMAS_10__ID:     return &palettelist.christmas_10;
    case PALETTELIST_STATIC_CHRISTMAS_11__ID:     return &palettelist.christmas_11;
    case PALETTELIST_STATIC_CHRISTMAS_12__ID:     return &palettelist.christmas_12;
    case PALETTELIST_STATIC_CHRISTMAS_13__ID:     return &palettelist.christmas_13;
    case PALETTELIST_STATIC_CHRISTMAS_14__ID:     return &palettelist.christmas_14;
    case PALETTELIST_STATIC_CHRISTMAS_15__ID:     return &palettelist.christmas_15;
    case PALETTELIST_STATIC_CHRISTMAS_16__ID:     return &palettelist.christmas_16;
    case PALETTELIST_STATIC_CHRISTMAS_17__ID:     return &palettelist.christmas_17;
    case PALETTELIST_STATIC_CHRISTMAS_18__ID:     return &palettelist.christmas_18;
    case PALETTELIST_STATIC_CHRISTMAS_19__ID:     return &palettelist.christmas_19;
    case PALETTELIST_STATIC_CHRISTMAS_20__ID:     return &palettelist.christmas_20;
    case PALETTELIST_STATIC_CHRISTMAS_21__ID:     return &palettelist.christmas_21;
    case PALETTELIST_STATIC_CHRISTMAS_22__ID:     return &palettelist.christmas_22;
    case PALETTELIST_STATIC_CHRISTMAS_23__ID:     return &palettelist.christmas_23;
    case PALETTELIST_STATIC_CHRISTMAS_24__ID:     return &palettelist.christmas_24;
    case PALETTELIST_STATIC_CHRISTMAS_25__ID:     return &palettelist.christmas_25;
    case PALETTELIST_STATIC_CHRISTMAS_26__ID:     return &palettelist.christmas_26;
    case PALETTELIST_STATIC_CHRISTMAS_27__ID:     return &palettelist.christmas_27;
    case PALETTELIST_STATIC_CHRISTMAS_28__ID:     return &palettelist.christmas_28;
    case PALETTELIST_STATIC_CHRISTMAS_29__ID:     return &palettelist.christmas_29;
    case PALETTELIST_STATIC_CHRISTMAS_30__ID:     return &palettelist.christmas_30;
    case PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_01__ID:     return &palettelist.gradient_sunlevel_group01_01;
    case PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_02__ID:     return &palettelist.gradient_sunlevel_group01_02;
    case PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_03__ID:     return &palettelist.gradient_sunlevel_group01_03;
    case PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_04__ID:     return &palettelist.gradient_sunlevel_group01_04;
    case PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_05__ID:     return &palettelist.gradient_sunlevel_group01_05;
    case PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_06__ID:     return &palettelist.gradient_sunlevel_group01_06;
    case PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_07__ID:     return &palettelist.gradient_sunlevel_group01_07;
    case PALETTELIST_STATIC__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01__ID:     return &palettelist.gradient_solid_rgbcct_sun_elevation_with_degrees_in_index_01;
    case PALETTELIST_STATIC__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_02__ID:     return &palettelist.gradient_solid_rgbcct_sun_elevation_with_degrees_in_index_02;
    case PALETTELIST_STATIC__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_03__ID:     return &palettelist.gradient_solid_rgbcct_sun_elevation_with_degrees_in_index_03;
    case PALETTELIST_STATIC__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_04__ID:     return &palettelist.gradient_solid_rgbcct_sun_elevation_with_degrees_in_index_04;
    case PALETTELIST_STATIC_DUAL_COLOUR_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01__ID:     return &palettelist.gradient_solid_rgbcct_sun_elevation_with_degrees_in_index_dual_colours_01;
    case PALETTELIST_STATIC_CUSTOM_USER_01__ID: return &palettelist.custom_user_01;
    
  }

}


HsbColor mPalette::GetHsbColour(uint8_t id){

  // id = COLOUR_MAP_GREEN_SATURATION90__ID;

  HsbColor colour = HsbColor(1,1,1);

  // Get Saturation
  uint8_t sat_val = id%12;
  // AddLog(LOG_LEVEL_INFO,PSTR("sat_val = %d   %d"), sat_val, id);

  switch(sat_val){
    default:
    case 0:  colour.S = 1.0f; break;
    case 1:  colour.S = 0.95f;break;
    case 2:  colour.S = 0.9f; break;
    case 3:  colour.S = 0.8f; break;
    case 4:  colour.S = 0.7f; break;
    case 5:  colour.S = 0.6f; break;
    case 6:  colour.S = 0.5f; break;
    case 7:  colour.S = 0.4f; break;
    case 8:  colour.S = 0.3f; break;
    case 9:  colour.S = 0.2f; break;
    case 10: colour.S = 0.1f; break;
    case 11: colour.S = 0.0f; break; // not needed, identical for all
  }

  // Get hue
  if(id>=COLOUR_MAP_RED__ID    && id<=COLOUR_MAP_RED_SATURATION0__ID){
    colour.H = pgm_read_float(&colour_float_map_values[0]); // progmem here might not be useful
  }else
  if(id>=COLOUR_MAP_ORANGE__ID && id<=COLOUR_MAP_ORANGE_SATURATION0__ID){
    colour.H = pgm_read_float(colour_float_map_values+1);
  }else
  if(id>=COLOUR_MAP_LIGHTORANGE__ID && id<=COLOUR_MAP_LIGHTORANGE_SATURATION0__ID){
    colour.H = pgm_read_float(colour_float_map_values+2);
  }else
  if(id>=COLOUR_MAP_WHITE__ID && id<=COLOUR_MAP_WHITE_SATURATION0__ID){
    colour.H = pgm_read_float(colour_float_map_values+3);
  }else
  if(id>=COLOUR_MAP_YELLOW__ID && id<=COLOUR_MAP_YELLOW_SATURATION0__ID){
    colour.H = pgm_read_float(colour_float_map_values+4);
  }else
  if(id>=COLOUR_MAP_LIMEGREEN__ID && id<=COLOUR_MAP_LIMEGREEN_SATURATION0__ID){
    colour.H = pgm_read_float(colour_float_map_values+5);
  }else
  if(id>=COLOUR_MAP_GREEN__ID && id<=COLOUR_MAP_GREEN_SATURATION0__ID){
    colour.H = pgm_read_float(colour_float_map_values+6);
  }else
  if(id>=COLOUR_MAP_CYAN__ID && id<=COLOUR_MAP_CYAN_SATURATION0__ID){
    colour.H = pgm_read_float(colour_float_map_values+7);
  }else
  if(id>=COLOUR_MAP_BLUE__ID && id<=COLOUR_MAP_BLUE_SATURATION0__ID){
    colour.H = pgm_read_float(colour_float_map_values+8);
  }else
  if(id>=COLOUR_MAP_BLUEPURPLE__ID && id<=COLOUR_MAP_BLUEPURPLE_SATURATION0__ID){
    colour.H = pgm_read_float(colour_float_map_values+9);
  }else
  if(id>=COLOUR_MAP_PURPLE__ID && id<=COLOUR_MAP_PURPLE_SATURATION0__ID){
    colour.H = pgm_read_float(colour_float_map_values+10);
  }else
  if(id>=COLOUR_MAP_PINK__ID && id<=COLOUR_MAP_PINK_SATURATION0__ID){
    colour.H = pgm_read_float(colour_float_map_values+11);
  }else
  if(id>=COLOUR_MAP_HOTPINK__ID && id<=COLOUR_MAP_HOTPINK_SATURATION0__ID){
    colour.H = pgm_read_float(colour_float_map_values+12);
  }

  // ALOG_INF(PSTR("GetHsbColour(%D) H=%d,S=%d,B=%d"), id, uint16_t(colour.H*100), uint8_t(colour.S*100), uint8_t(colour.B*100));

  // Serial.println(colour.H);
  // Serial.println(colour.S);
  // Serial.println(colour.B);

  // AddLog(LOG_LEVEL_INFO,PSTR("id mod 12 = %d   %d"), id%12, id);
  // colour.S = (12-sat_val)

  return colour;

}


bool mPalette::CheckPaletteIsEditable(PALETTELIST::PALETTE *ptr)
{
  return CheckPaletteByIDIsEditable(ptr->id);
}
bool mPalette::CheckPaletteByIDIsEditable(uint8_t id)
{
  if(( (id>=PALETTELIST_VARIABLE_HSBID_01__ID)&&
      (id<PALETTELIST_VARIABLE_HSBID_LENGTH__ID) )
    || 
    ( (id>=PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_01__ID)&&
      (id<PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID) )
    ){
    return true;
  }
  return false;
}


int16_t mPalette::GetPaletteIDbyName(const char* c){
  if(*c=='\0'){
    return -1;
  }

  PALETTELIST::PALETTE *ptr = nullptr;
  char buffer[100] = {0};
  int16_t index_found = -1;

  /**************************************************************
   * 
   * PALETTELIST_STATIC__IDS
   * 
  ***************************************************************/

  for( // loops relative to exact palette id
    uint8_t ii=PALETTELIST_STATIC_PARTY_DEFAULT__ID;
            ii<(PALETTELIST_STATIC_LENGTH__ID - PALETTELIST_STATIC_PARTY_DEFAULT__ID);
            ii++
  ){
    ptr = GetPalettePointerByID(ii);

    // DEBUG_LINE_HERE;

    // ALOG_INF( PSTR("Name \"%s\"=?\"%S\""), c, ptr->friendly_name_ctr );

    if(ptr->friendly_name_ctr == nullptr)
    {
      ALOG_DBM( PSTR("ptr->friendly_name_ctr == nullptr %d %s"), ii, c );
    }

    if(ptr->friendly_name_ctr != nullptr)
    { 
      ALOG_DBM( PSTR("ptr->friendly_name_ctr == %d %s %S"), ii, c, ptr->friendly_name_ctr );
      if(mSupport::CheckCommand_P(c, ptr->friendly_name_ctr))
      {
        ALOG_DBM( PSTR("MATCH \"%c\" %d"), c, ii ); 
        return ii;            
      }
    }
  }

  /**************************************************************
   * 
   * PALETTELIST_VARIABLE_HSBID__IDS
   * 
  ***************************************************************/
  
  for( // loops relative to 0
    uint8_t ii=0;
            ii<(PALETTELIST_VARIABLE_HSBID_LENGTH__ID - PALETTELIST_VARIABLE_HSBID_01__ID);
            ii++
  ){

    memset(buffer,0,sizeof(buffer));
    sprintf_P(buffer, PSTR(D_DEFAULT_DYNAMIC_PALETTE_NAMES__VARIABLE_HSBID__NAME_CTR), ii + 1); // Names are 1-10
    
    ALOG_DBM( PSTR("s> \"%s\""), buffer ); 
    /**
     * @brief Default Names
     */
    if(strcmp(c,buffer)==0){
      return PALETTELIST_VARIABLE_HSBID_01__ID+ii;
    }
    /**
     * @brief Future user defined names
     * ALOG_WRN( PSTR("GetPaletteIDbyName: Not searching DeviceNameList") );
     */
    
  }
  
  /**************************************************************
   * 
   * PALETTELIST_VARIABLE_GENERIC__IDS
   * 
  ***************************************************************/
  for( // loops relative to 0
    uint8_t ii=0;
            ii<(PALETTELIST_VARIABLE_GENERIC_LENGTH__ID - PALETTELIST_VARIABLE_GENERIC_01__ID);
            ii++
  ){

    memset(buffer,0,sizeof(buffer));
    sprintf_P(buffer, PSTR(D_DEFAULT_DYNAMIC_PALETTE_NAMES__VARIABLE_HSBID__NAME_CTR), ii + 1);
    
    ALOG_DBG( PSTR("s> \"%s\""), buffer ); 
    // Default names
    if(strcmp(c,buffer)==0){
      return ii+PALETTELIST_VARIABLE_HSBID_01__ID;
    }
    
  }

  /**************************************************************
   * 
   * PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR__IDS
   * 
  ***************************************************************/ 
  for(
    uint8_t ii=0;
            ii<(PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID-PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_01__ID);
            ii++
  ){
    memset(buffer,0,sizeof(buffer));
    // sprintf_P(name_ctr,PSTR("%s %02d\0"),D_PALETTE_RGBCCT_COLOURS_NAME_CTR,ii);
    // Default names
    sprintf_P(buffer, PSTR(D_DEFAULT_DYNAMIC_PALETTE_NAMES__VARIABLE_RGBCCT__NAME_CTR), ii + 1); // names are 1-10
        ALOG_INF( PSTR(DEBUG_INSERT_PAGE_BREAK "Searching with \"%s\" for \"%s\""),buffer,c );
    if(strcmp(c,buffer)==0)
    {
      return ii+PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_01__ID;
    }
  }
    
  /**************************************************************
   * 
   * PALETTELIST_STATIC_CRGBPALETTE16__IDS
   * 
  ***************************************************************/
  int16_t id = -1;
 for(
    uint8_t ii=0;
            ii<(PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID-PALETTELIST_STATIC_CRGBPALETTE16__CLOUD_COLOURS__ID);
            ii++
  ){
    
    ALOG_INF( PSTR("s> %d %s \"%S\""), ii, c, PM_STATIC_CRGBPALETTE16_NAMES_CTR ); 

     if((id=mSupport::GetCommandID16_P(c, PM_STATIC_CRGBPALETTE16_NAMES_CTR))>=0)
      {
        ALOG_INF( PSTR("MATCH \"%s\" %d %d"), c, ii, id ); 
        return id+PALETTELIST_STATIC_CRGBPALETTE16__CLOUD_COLOURS__ID;            
      }



  }
 
  /**************************************************************
   * 
   * PALETTELIST_STATIC_CRGBPALETTE16__IDS
   * PALETTELIST_CRGBPALETTE16_GRADIENT___PALETTES__IDS
   * 
  ***************************************************************/
  // if(
  //   ((palette_id >= PALETTELIST_STATIC_CRGBPALETTE16__CLOUD_COLOURS__ID) && (palette_id < PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID)) ||
  //   ((palette_id >= PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID)    && (palette_id < PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID))
  // ){  


  // }

  /**************************************************************
   * 
   * Final check, palette id was given as string number
   * 
  ***************************************************************/
  uint8_t found_index = (!strlen(c)) ? 0 : atoi(c);
  if(WithinLimits(found_index, (uint8_t)0, (uint8_t)PALETTELIST_STATIC_LENGTH__ID)){
    return found_index;
  }

  return -1; // Must be -1 to show name not found
}


// Check for friendly names first
const char* mPalette::GetPaletteFriendlyName(char* buffer, uint8_t buflen){
  // #ifdef USE_MODULE_LIGHTS_ANIMATOR
  return GetPaletteFriendlyNameByID(pCONT_lAni->SEGMENT_I(0).palette.id, buffer, buflen);
  // #else
  // return GetPaletteFriendlyNameByID(0, buffer);
  // #endif
} 
const char* mPalette::GetPaletteFriendlyNameByID(uint8_t id, char* buffer, uint8_t buflen){

  PALETTELIST::PALETTE *ptr = GetPalettePointerByID(id);

  // AddLog(LOG_LEVEL_HIGHLIGHT,PSTR(" %d GetPaletteFriendlyNameByID"),id);

  // DEBUG_LINE_HERE;

  if(id<PALETTELIST_VARIABLE_GENERIC_LENGTH__ID){
    // dList


    // #ifdef ENABLE_DEVFEATURE_GETPALETTE_ID_FROM_NAME_V2

    // char buffer[100]; // temp fix, will just use pointer, need another function below to do the same without buffer input


  // I need to offset where it starts


// unstable, fix next.
// No more flashing of devices until id searching per class is found to be stable and working

    // const char* name_p = DLI->GetDeviceNameWithEnumNumber( E M_MODULE_LIGHTS_INTERFACE_ID, id,buffer,sizeof(buffer)); 

    const char* name_p = DLI->GetDeviceName_WithModuleUniqueID( pCONT_iLight->GetModuleUniqueID(), id, buffer, buflen); 

    
    // ALOG_INF( PSTR(" %d = \"%S\""),id,name_p);

    if(name_p==nullptr)
    {
      return PM_SEARCH_NOMATCH;
    }

    return name_p;


    // #else

    
    // const char* name_p = pCONT_sup->GetTextIndexed(buffer, buflen, id, pCONT_set->Settings.device_name_buffer.name_buffer);
    // AddLog(LOG_LEVEL_HIGHLIGHT,PSTR(" %d = \"%s\""),id,name_p);

    // // This assumes indexing begins from start (ie lights MUST be the first class_id), needs fixing
    // return name_p;

    // #endif 

    // return pCONT_sup->GetTextIndexed_P(buffer, buflen, id, pCONT_set->Settings.animation_settings.palette_user_variable_name_list_ctr);
  }
  else{
    // Progmem
    snprintf_P(buffer, buflen, "%S", ptr->friendly_name_ctr!=nullptr?ptr->friendly_name_ctr:WARNING_NOTHANDLED_CTR);
    return buffer;
  }

}


const char* mPalette::GetPaletteName(char* buffer, uint8_t buflen){
  return GetPaletteFriendlyNameByID(pCONT_lAni->SEGMENT_I(0).palette.id, buffer, buflen);
} 
const char* mPalette::GetPaletteNameByID(uint8_t id, char* buffer, uint8_t buflen){  
  PALETTELIST::PALETTE *ptr = GetPalettePointerByID(id);
  return GetPaletteFriendlyNameByID(id, buffer, buflen);
}



uint16_t mPalette::GetNumberOfColoursInPalette(uint16_t palette_id, uint8_t pixel_width_contrained_limit){

  return GetNumberOfColoursInPalette(GetPalettePointerByID(palette_id), pixel_width_contrained_limit);

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
  if(encoding.index_byte_width){ encoded_colour_width += encoding.index_byte_width; }

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

/**
 * @brief Get elements in a colour type
 * 
 * @param ptr 
 * @param pixel_width_contrained_limit 
 * @return uint16_t 
 */
uint16_t mPalette::GetNumberOfColoursInPalette(PALETTELIST::PALETTE *ptr, uint8_t pixel_width_contrained_limit){
  
  uint16_t encoded_colour_width = 0;
  uint16_t pixel_length = 0;

    // DEBUG_LINE_HERE; // this should be a fallback only
  if(ptr == nullptr)
  {
    // DEBUG_LINE_HERE; // this should be a fallback only
    ptr = palettelist.ptr; // use internal if not set already
  }

  // Serial.println(ptr->encoding.data, BIN);

    // With this enabled, only the christmas palettes will be stable!!

    if(ptr->encoding.red_enabled){ encoded_colour_width++; }
    if(ptr->encoding.green_enabled){ encoded_colour_width++; }
    if(ptr->encoding.blue_enabled){ encoded_colour_width++; }
    if(ptr->encoding.white_warm_enabled){ encoded_colour_width++; }

    if(ptr->encoding.white_cold_enabled){ encoded_colour_width++; }
    if(ptr->encoding.index_byte_width){ encoded_colour_width += ptr->encoding.index_byte_width; }

    if(ptr->encoding.index_exact){ encoded_colour_width++; }
    if(ptr->encoding.index_scaled_to_segment){ encoded_colour_width++; }
    if(ptr->encoding.index_is_trigger_value_exact){ encoded_colour_width++; }
    if(ptr->encoding.index_is_trigger_value_scaled100){ encoded_colour_width++; }
    
    if(ptr->encoding.encoded_as_hsb_ids){ encoded_colour_width++; }
    if(ptr->encoding.encoded_as_crgb_palette_16){ encoded_colour_width++; }
    if(ptr->encoding.encoded_as_crgb_palette_256){ encoded_colour_width++; }
    if(ptr->encoding.palette_can_be_modified){ encoded_colour_width++; }

    // ALOG_INF(PSTR("encoded_colour_width = %d"),encoded_colour_width);
   

  // recalculate size, it might not be the same
  // ptr->data_length = 
  // GetDefaultColourPaletteUserIDsCount
  // GetColourMapSize()
  // Dont do this everytime, as its costly on memory, only update on change

  if(encoded_colour_width==0)
  {
    ALOG_ERR(PSTR("encoded_colour_width==0, crash errorAA =%S"), ptr->friendly_name_ctr);
    return 1;
  }
  
  // Serial.println(encoded_colour_width); Serial.flush();
  // if(encoded_colour_width){ //so we can divide if not 0
    pixel_length = ptr->data_length/encoded_colour_width; 
    // AddLog(LOG_LEVEL_TEST, PSTR("pixel_length=%d %d %d ID%d"),pixel_length,ptr->data_length,encoded_colour_width, ptr->id);
  // }else{
  //   pixel_length = ptr->data_length/encoded_colour_width
  // }
  //constrain limit if max
  // if(encoded_colour_width_contrained_limit>=0){
  //   pixel_length = constrain(pixel_length,1,encoded_colour_width_contrained_limit);
  //   // AddLog(LOG_LEVEL_TEST, PSTR("encoded_colour_width_contrained_limit=%d, pixel_length=%d"),encoded_colour_width_contrained_limit, pixel_length);
  // }
  return pixel_length;
}


/**
 * Refresh value stored in palette, and return new value
 * */
uint8_t mPalette::GetColourMapSizeByPaletteID(uint8_t palette_id){
  
  uint8_t new_size = 1; // assumed 1 at least
  PALETTELIST::PALETTE *ptr_tmp = GetPalettePointerByID(palette_id);

  if((palette_id>=PALETTELIST_VARIABLE_HSBID_01__ID)&&(palette_id<PALETTELIST_VARIABLE_HSBID_LENGTH__ID))
  {
    //Count until "not set value"  
    // new_size = GetDefaultColourPaletteUserIDsCount(palette.id);

    uint8_t pixel_active = 0;
    uint8_t byte_read = 0;
    while(pixel_active<20){
      // byte_read = pgm_read_byte(id_ptr+pixel_active); // This doesnt seem right??
      byte_read = pCONT_set->Settings.animation_settings.palette_hsbid_users_colour_map[(20*palette_id)+pixel_active];
      // Serial.printf("byte_read=%d",byte_read);
      if(byte_read!=COLOUR_MAP_NONE__ID){//} && byte_read<COLOUR_MAP_PALETTE_OPTIONS_END__ID){
        pixel_active++;
      }else{
        break;
      }
    }
    new_size = pixel_active; // new count from local stored memory this time

    ptr_tmp->data_length = new_size;

  }else
  if((palette_id>=PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_01__ID)&&(palette_id<PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID)){

    new_size = 5; // only 1*5
    
    ptr_tmp->data_length = new_size;

  }else
  if((palette_id>=PALETTELIST_VARIABLE_GENERIC_01__ID)&&(palette_id<PALETTELIST_VARIABLE_GENERIC_LENGTH__ID)){

    new_size = ptr_tmp->data_length; // currently refreshed when init is triggered, about this may need changing, leaving unchanged for now
  }
  else
  {

    new_size = ptr_tmp->data_length;
    ALOG_INF(PSTR("GetColourMapSizeByPaletteID ELSE"));
    DEBUG_LINE_HERE;

  }

  return new_size;
}


/**
 * @brief Do I really need this? Should it just be directly set like "christmas"?
 * Or is this loading defaults nicely 
 * 
 * @param id 
 * @return const uint8_t* 
 */
const uint8_t* mPalette::GetDefaultColourPaletteUserIDs_P(uint8_t id){
  switch(id){
    default: 
    case PALETTELIST_VARIABLE_HSBID_01__ID: return DEFAULT_COLOUR_PALETTE_HSBID_01__DATA;
    case PALETTELIST_VARIABLE_HSBID_02__ID: return DEFAULT_COLOUR_PALETTE_HSBID_02__DATA;
    case PALETTELIST_VARIABLE_HSBID_03__ID: return DEFAULT_COLOUR_PALETTE_HSBID_03__DATA;
    case PALETTELIST_VARIABLE_HSBID_04__ID: return DEFAULT_COLOUR_PALETTE_HSBID_04__DATA;
    case PALETTELIST_VARIABLE_HSBID_05__ID: return DEFAULT_COLOUR_PALETTE_HSBID_05__DATA;
    case PALETTELIST_VARIABLE_HSBID_06__ID: return DEFAULT_COLOUR_PALETTE_HSBID_06__DATA;
    case PALETTELIST_VARIABLE_HSBID_07__ID: return DEFAULT_COLOUR_PALETTE_HSBID_07__DATA;
    case PALETTELIST_VARIABLE_HSBID_08__ID: return DEFAULT_COLOUR_PALETTE_HSBID_08__DATA;
    case PALETTELIST_VARIABLE_HSBID_09__ID: return DEFAULT_COLOUR_PALETTE_HSBID_09__DATA;
    case PALETTELIST_VARIABLE_HSBID_10__ID: return DEFAULT_COLOUR_PALETTE_HSBID_10__DATA;
  }
}


uint8_t mPalette::GetDefaultColourPaletteUserIDsCount(uint8_t id){


  // Serial.printf("GetDefaultColourPaletteUserIDsCount\n\r"); Serial.flush();

  auto id_ptr = GetDefaultColourPaletteUserIDs_P(id);
  // Serial.printf(GetDefaultColourPaletteUserIDs_P(id));

  
  // const uint8_t* id_ptr = DEFAULT_COLOUR_PALETTE_HSBID_01__DATA;

  // Serial.printf("GetDefaultColourPaletteUserIDsCountBB=%d\n\r",   pgm_read_byte(id_ptr)); Serial.flush();

  //search for COLOUR_MAP_NONE__ID to signify end, must be less than 20 in length

  uint8_t pixel_active = 0;
  uint8_t byte_read = 0;
  while(pixel_active<20){
    byte_read = pgm_read_byte(id_ptr+pixel_active); // This doesnt seem right??
    // Serial.printf("byte_read=%d",byte_read);
    if(byte_read!=COLOUR_MAP_NONE__ID){//} && byte_read<COLOUR_MAP_PALETTE_OPTIONS_END__ID){
      pixel_active++;
    }else{
      break;
    }
  }
  // Serial.printf("pixel_active=%d\n\r",   pixel_active); Serial.flush();

  return pixel_active>PALETTELIST_COLOUR_HSBID_AMOUNT_MAX?PALETTELIST_COLOUR_HSBID_AMOUNT_MAX:pixel_active;
}


void mPalette::SetPaletteListPtrFromID(uint8_t id){
  //AddLog(LOG_LEVEL_TEST,PSTR("SetPaletteListPtrFromID(%d)"),id);
  palettelist.ptr = GetPalettePointerByID(id);
}



int8_t mPalette::GetColourMapIDbyName(const char* c){
  if(*c=='\0') return -1;
  // if(strstr(c,D_COLOUR_MAP_RED_CTR))          return COLOUR_MAP_RED__ID;
  // if(strstr(c,D_COLOUR_MAP_RED_PASTEL80_CTR))          return COLOUR_MAP_RED_PASTEL80__ID;
  // if(strstr(c,D_COLOUR_MAP_RED_PASTEL60_CTR))          return COLOUR_MAP_RED_PASTEL60__ID;
  // if(strstr(c,D_COLOUR_MAP_ORANGE_CTR))       return COLOUR_MAP_ORANGE__ID;
  // if(strstr(c,D_COLOUR_MAP_LIGHTORANGE_CTR))  return COLOUR_MAP_LIGHTORANGE__ID;
  // if(strstr(c,D_COLOUR_MAP_YELLOW_CTR))       return COLOUR_MAP_YELLOW__ID;
  // if(strstr(c,D_COLOUR_MAP_LIMEGREEN_CTR))    return COLOUR_MAP_LIMEGREEN__ID;
  // if(strstr(c,D_COLOUR_MAP_GREEN_CTR))        return COLOUR_MAP_GREEN__ID;
  // if(strstr(c,D_COLOUR_MAP_CYAN_CTR))         return COLOUR_MAP_CYAN__ID;
  // if(strstr(c,D_COLOUR_MAP_BLUE_CTR))         return COLOUR_MAP_BLUE__ID;
  // if(strstr(c,D_COLOUR_MAP_BLUEPURPLE_CTR))   return COLOUR_MAP_BLUEPURPLE__ID;
  // if(strstr(c,D_COLOUR_MAP_PURPLE_CTR))       return COLOUR_MAP_PURPLE__ID;
  // if(strstr(c,D_COLOUR_MAP_PINK_CTR))         return COLOUR_MAP_PINK__ID;
  // if(strstr(c,D_COLOUR_MAP_HOTPINK_CTR))      return COLOUR_MAP_HOTPINK__ID;
  // if(strstr(c,D_COLOUR_MAP_NONE_CTR))      return COLOUR_MAP_NONE__ID;
  // if(strstr(c,D_COLOUR_MAP_PALETTE_OPTIONS_END_CTR))      return COLOUR_MAP_PALETTE_OPTIONS_END__ID;

  
  return -1;
}
const char* mPalette::GetColourMapNamebyID(uint8_t id, char* buffer, uint8_t buflen){

  // use switch method I generate hsb to also generate the name
  snprintf_P(buffer, sizeof(buffer), WARNING_NOTHANDLED_CTR);
  return buffer;
  
}

// Legacy method to phase out!
RgbcctColor 
#ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
IRAM_ATTR 
#endif 
mPalette::GetColourFromPreloadedPalette(
  uint16_t palette_id,
  uint16_t _pixel_position,//desired_index_from_palette,  
  uint8_t* encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
  bool     flag_map_scaling, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
  bool     flag_wrap,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
  uint8_t mcol, // will be phased out
  bool     flag_convert_pixel_index_to_get_exact_crgb_colour,   // added by me, to make my effects work with CRGBPalette16
  uint8_t  brightness_scale //255(default): No scaling, 0-255 scales the brightness of returned colour (remember all colours are saved in full 255 scale)
  // uint8_t* discrete_colours_in_palette //ie length of palette as optional return
){ 
  
  uint8_t* palette_elements = pCONT_lAni->segments[pCONT_lAni->_segment_index_primary].palette_container->pData.data(); // If function gets internalised to strip, it wont need to define which data set

  return mPaletteI->GetColourFromPreloadedPaletteBuffer( //"buffer" name to be removed when properly integrated
    palette_id,
    palette_elements,
    _pixel_position,//desired_index_from_palette,  
    encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
    flag_map_scaling, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
    flag_wrap,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
    mcol, // will be phased out
    flag_convert_pixel_index_to_get_exact_crgb_colour,   // added by me, to make my effects work with CRGBPalette16
    brightness_scale //255(default): No scaling, 0-255 scales the brightness of returned colour (remember all colours are saved in full 255 scale)
    // uint8_t* discrete_colours_in_palette //ie length of palette as optional return
  );

}

/** NEW METHOD: To test this, creating a json constructor that can produce each palette under its desired ways
 * Gets a single color from the currently selected palette.
 * @param id                  Palette Index, default (0) will be random from colour wheel
 * @param index_from_palette  [0..x] where x is total colours in palette. If `flag_mapping` is enabled, palette will be scaled out to be Y length >>> Palette Index (if mapping is true, the full palette will be _virtualSegmentLength long, if false, 255). Will wrap around automatically.
 * @param flag_mapping        if true, LED position in segment is considered for color
 * @param flag_wrap           FastLED palettes will usally wrap back to the start smoothly. Set false to get a hard edge
 * @param brightness_scale255 Value to scale the brightness of the returned color by. Default is 255. (no scaling)
 * @param encoded_index*      If format of palette includes encoded data (ie sun_value,R,G,B) then return by inserting into pointer position
 * 
 * @returns RgbCCTColour      Single color from palette in RgbCCT format. Use intermediate function to convert to UINT32_T if WRGB is needed for WLED effect
 
 without IRAM_ATTR, function takes 10.5 per call
 with IRAM_ATTR, STILL 10.5 per call

Conversion map
  GetColourFromPalette_Intermediate(palette_id, pixel_num,       *pixel_position)
  GetColourFromPreloadedPalette    (palette_id, _pixel_position,  encoded_value)
Conversion map
  color_from_palette_Intermediate  (                 uint16_t i,                     bool mapping, bool wrap, uint8_t mcol,                                          uint8_t pbri                              )
  GetColourFromPreloadedPalette    (UNSET,      _pixel_position, *encoded_value, flag_map_scaling, flag_wrap, flag_convert_pixel_index_to_get_exact_crgb_colour, brightness_scale, *discrete_colours_in_palette) 
     mcol is now mPalette::PALETTELIST_VARIABLE_FASTLED_SEGMENT__COLOUR_01__ID
     mPaletteI->GetColourFromPreloadedPaletteU32(SEGMENT.palette.id, A, nullptr, B, C, D)  <<<< copy and fill in as needed
 */
RgbcctColor 
#ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
IRAM_ATTR 
#endif 
mPalette::GetColourFromPreloadedPaletteBuffer(
  uint16_t palette_id,
  uint8_t* palette_buffer,
  uint16_t _pixel_position,//desired_index_from_palette,  
  uint8_t* encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
  bool     flag_map_scaling, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
  bool     flag_wrap,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
  uint8_t mcol, // will be phased out
  bool     flag_convert_pixel_index_to_get_exact_crgb_colour,   // added by me, to make my effects work with CRGBPalette16
  uint8_t  brightness_scale //255(default): No scaling, 0-255 scales the brightness of returned colour (remember all colours are saved in full 255 scale)
  // uint8_t* discrete_colours_in_palette //ie length of palette as optional return
){

  RgbcctColor colour = RgbcctColor(0);

  #ifdef ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
  Serial.println(palette_id);
  DEBUG_LINE_HERE;
  #endif

  /**************************************************************
   * 
   * PALETTELIST_STATIC__IDS
   * PALETTELIST_VARIABLE_HSBID__IDS
   * PALETTELIST_VARIABLE_GENERIC__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= PALETTELIST_STATIC_PARTY_DEFAULT__ID) && (palette_id < PALETTELIST_STATIC_LENGTH__ID)) ||
    ((palette_id >= PALETTELIST_VARIABLE_HSBID_01__ID)    && (palette_id < PALETTELIST_VARIABLE_HSBID_LENGTH__ID)) ||
    ((palette_id >= PALETTELIST_VARIABLE_GENERIC_01__ID)  && (palette_id < PALETTELIST_VARIABLE_GENERIC_LENGTH__ID))
  ){  

    // ALOG_INF(PSTR("MINE %d"), palette_id);

    PALETTELIST::PALETTE *ptr = GetPalettePointerByID(palette_id);

    // this will not work, since each segment will need its preloaded. 
    // Need a drastic rethink on this, perhaps another subpalette class that stores the "runtime palette"
    //probably best to worry about speed optimising until after WLED palettes are merged fully

    // AddLog(LOG_LEVEL_TEST, PSTR("ptr->data_length=%d"),ptr->data_length );

    // uint8_t* palette_elements = pCONT_lAni->segments[pCONT_lAni->_segment_index_primary].palette_container->pData.data(); // If function gets internalised to strip, it wont need to define which data set

    if(palette_buffer==nullptr)
    {
      ALOG_ERR(PSTR("palette_elements unset"));
      return RgbcctColor(0);
    }

    uint8_t pixels_in_map = GetNumberOfColoursInPalette(ptr);  
    uint8_t colour_width  = GetEncodedColourWidth(ptr->encoding); 
    // Serial.println(pixels_in_map);
    uint8_t remainder = _pixel_position%pixels_in_map;  // /ERROR, REUSING NAME!!!
    // uint8_t remainder_scaled255 = map(remainder, 0,pixels_in_segment-1, 0,255);
    uint16_t pixel_position = remainder;   // THIS IS BEING PASSED IN AND REUSED??

    uint16_t index_relative = 0; // get expected pixel position

    #ifdef ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    Serial.println(ptr->encoding.data, BIN);
    Serial.println(ptr->encoding.encoded_as_hsb_ids);
    Serial.println(ptr->encoding.index_scaled_to_segment);
    #endif // ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    
    /**
      Encoded as HSBID, must handle all index methods
    **/
    if(
      (ptr->encoding.encoded_as_hsb_ids)
    ){

    // DEBUG_LINE_HERE;
      // DEBUG_LINE_HERE;
      // Serial.println("MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX__ID"); 

      // Get Start of Colour Information by adjusting for indexing
      index_relative = pixel_position*GetEncodedColourWidth(ptr->encoding); // get expected pixel position
      if(ptr->encoding.index_scaled_to_segment)
      {
        // If desired, return the index value
        if(encoded_value != nullptr){
          *encoded_value = palette_buffer[index_relative];
        }
        // Serial.println(*encoded_value);
        // Set the index to move beyond the indexing information
        index_relative++;
      };

      colour = RgbcctColor(
        GetHsbColour(palette_buffer[index_relative])
      );

      // if(pixel_position==0)
      // Serial.printf("%d|%d c %d %d %d\n\r", palette_id, pCONT_lAni->_segment_index_primary, colour.R, colour.G, colour.B);
      // DEBUG_LINE_HERE;
            
    }

    /**
      Encoded as Colour (RGBCCT or parts), must handle all index methods
    **/
    else if( // MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_GRADIENT__ID              //switch to bit masking
      (ptr->encoding.red_enabled)||
      (ptr->encoding.green_enabled)||
      (ptr->encoding.blue_enabled)||
      (ptr->encoding.white_cold_enabled)||
      (ptr->encoding.white_warm_enabled)
    ){
      
      // ALOG_INF(PSTR("enabled %d,%d,%d,%d,%d"), ptr->encoding.red_enabled, ptr->encoding.green_enabled, ptr->encoding.blue_enabled, ptr->encoding.white_cold_enabled, ptr->encoding.white_warm_enabled);
      // ALOG_INF(PSTR("palette_elements %d"), palette_elements[0]);

    // DEBUG_LINE_HERE;
      // DEBUG_LINE_HERE;      
      // ALOG_INF(PSTR("p = %d, r = %d, v = %d|%d, w=%d"), pixel_position, index_relative, palette_elements[index_relative], palette_elements[index_relative+1],colour_width);

      // Get Start of Colour Information by adjusting for indexing
      index_relative = pixel_position*GetEncodedColourWidth(ptr->encoding); // get expected pixel position
        // ALOG_INF(PSTR("index_relative=%d"),index_relative);
      if(ptr->encoding.index_scaled_to_segment)
      {
    // DEBUG_LINE_HERE;
    //     ALOG_INF(PSTR("index_relative=%d"),index_relative);
        // If desired, return the index value
        // if(encoded_value != nullptr){ 
        if(encoded_value != nullptr){
          *encoded_value = palette_buffer[index_relative];
        }
          // }
        // Set the index to move beyond the indexing information
        index_relative++;
      }
      
    // DEBUG_LINE_HERE;
      // Get colour
      colour = RgbcctColor(
        ptr->encoding.red_enabled         ? palette_buffer[index_relative  ] : 0,
        ptr->encoding.green_enabled       ? palette_buffer[index_relative+1] : 0,
        ptr->encoding.blue_enabled        ? palette_buffer[index_relative+2] : 0,
        ptr->encoding.white_cold_enabled  ? palette_buffer[index_relative+3] : 0,
        ptr->encoding.white_warm_enabled  ? palette_buffer[index_relative+4] : 0
      );

    }

    // DEBUG_LINE_HERE;

    // if(discrete_colours_in_palette != nullptr){ *discrete_colours_in_palette = 1; } //phase out

    // DEBUG_LINE_HERE;
    return colour;

  } // end of my palettes

    // DEBUG_LINE_HERE;

  /**************************************************************
   * 
   * PALETTELIST_STATIC_HTML_COLOUR_CODES__IDS
   * 
  ***************************************************************/
  if(
    (palette_id >= PALETTELIST_HTML_COLOUR__AliceBlue__ID) && (palette_id < PALETTELIST_HTML_COLOUR__LENGTH__ID)
  ){  

    uint16_t adjusted_id = palette_id - PALETTELIST_HTML_COLOUR__AliceBlue__ID;
    uint8_t segIdx = pCONT_lAni->_segment_index_primary;

    // for (
      // uint8_t indexName = 0; 
    //   indexName < HtmlColorNames::Count(); indexName++)
    // {
      const HtmlColorPair* colorPair = HtmlColorNames::Pair(adjusted_id);
      PGM_P searchName = reinterpret_cast<PGM_P>(pgm_read_ptr(&(colorPair->Name)));
      RgbcctColor colour = (HtmlColor)colorPair->Color;
      // ALOG_INF(PSTR("[%d] \"%S\" {%d} = %d,%d,%d"), adjusted_id, searchName, colorPair->Color, colour.R, colour.G, colour.B);
    // }


    // RgbcctColor colour = RgbcctColor::GetRgbcctFromU32Colour(0);//HtmlColor::)

   
    return colour;
  }

  /**************************************************************
   * 
   * PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR__IDS
   * 
  ***************************************************************/
  if(
    (palette_id >= PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_01__ID) && (palette_id < PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID)
  ){  

    uint8_t adjusted_id = palette_id - PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_01__ID;
    uint8_t segIdx = pCONT_lAni->_segment_index_primary;

    if(adjusted_id < pCONT_lAni->segments[segIdx].rgbcctcolors.size())
    {
      return pCONT_lAni->segments[segIdx].rgbcctcolors[adjusted_id];
    }
    return RgbcctColor(0);
  }

  /**************************************************************
   * 
   * PALETTELIST_STATIC_CRGBPALETTE16__IDS
   * PALETTELIST_CRGBPALETTE16_GRADIENT___PALETTES__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= PALETTELIST_STATIC_CRGBPALETTE16__CLOUD_COLOURS__ID) && (palette_id < PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID)) ||
    ((palette_id >= PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID)    && (palette_id < PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID)) ||
    ((palette_id >= PALETTELIST_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_01__ID)    && (palette_id < PALETTELIST_VARIABLE_CRGBPALETTE16__LENGTH__ID))
  ){  

    // ALOG_INF(PSTR("WS2812FX %d"), palette_id);
    // ALOG_INF(PSTR("WS2812FX %d"), palette_id);

    // Assumes other function has already set and loaded it? into "currentPalette", which is part of "loading" palette

    // uint8_t colours_in_palette = 0;

    /**
     * @brief This should be moved out of this, and only ran when the palette changes (note, this wont happen for the time varying, so leave here for now)
     **/
    // mPaletteI->LoadPalette_CRGBPalette16_Static(&colours_in_palette);




    //palette_id will not matter here, as its set/loaded "UpdatePalette_FastLED_TargetPalette"
    CRGB fastled_col;

    // uint16_t palette_id_adjusted = 0;     //palette_id-PALETTELIST_CRGBPALETTE_FIRST_ID;
    
    // if((palette_id >= PALETTELIST_STATIC_CRGBPALETTE16__CLOUD_COLOURS__ID) && (palette_id < PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID))
    // {
    //   palette_id_adjusted = palette_id-PALETTELIST_STATIC_CRGBPALETTE16__CLOUD_COLOURS__ID;
    // }
    // if((palette_id >= PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID)    && (palette_id < PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID))
    // {
    //   palette_id_adjusted = palette_id-PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID;
    // }
    // if((palette_id >= PALETTELIST_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS__ID)    && (palette_id < PALETTELIST_VARIABLE_CRGBPALETTE16__LENGTH__ID))
    // {
    //   palette_id_adjusted = palette_id-PALETTELIST_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS__ID;
    // }  



// if(flag_wrap)
// {
// ALOG_INF(PSTR("%d flag_wrap{%d}"), _pixel_position, flag_wrap);
// }
    
    // ALOG_INF(PSTR("id_adjusted %d  \tpixel_pos%d in_palette{%d}"), palette_id_adjusted, _pixel_position, colours_in_palette);

    uint8_t pixel_position_adjust = _pixel_position;

    
    // if(flag_convert_pixel_index_to_get_exact_crgb_colour)  // change this to auto switch between mPalette::PALETTELIST_VARIABLE_FASTLED_SEGMENT__COLOUR_01__ID && _pixel_position < 3) and this new method
    // {
    //   uint8_t pixels_in_segment = colours_in_palette;
    //   uint8_t remainder = _pixel_position%pixels_in_segment;
    //   uint8_t remainder_scaled255 = map(remainder, 0,pixels_in_segment-1, 0,255); //change to "scale8" for speed
    //   pixel_position_adjust = remainder_scaled255;

      
      
    //   // Serial.printf("pixel = %d|%d \t %d||%d\n\r", pixel_position, pixel_position_adjust , remainder, remainder_scaled255);

    // }
    // else //standard WLED method
    // {
      
      // if (SEGMENT.palette.id == 0 && mcol < 3) return SEGCOLOR(mcol); //WS2812FX default
      if(palette_id == 0 && _pixel_position < 3)
      {        
        ALOG_ERR(PSTR("PHASED OUT! Not sure it is still needed?"));
        // return pCONT_lAni->SEGMENT_I(pCONT_lAni->getCurrSegmentId()).rgbcctcolors[_pixel_position]; 
      }
      
      if (flag_map_scaling) pixel_position_adjust = (_pixel_position*255)/(pCONT_lAni->_virtualSegmentLength -1);  // This scales out segment_index to segment_length as 0 to 255
      // AddLog(LOG_LEVEL_TEST, PSTR("paletteIndex=%d"),paletteIndex);
      if (!flag_wrap) pixel_position_adjust = scale8(pixel_position_adjust, 240); //cut off blend at palette "end", 240, or 15/16*255=>240/255, so drop last 16th (15 to wrapped 0) gradient of colour

    // }
    
    
    // CRGBPalette16 curPal;
    // if (transitional && _t) curPal = _t->_palT;
    // else                    loadPalette(curPal, palette);
    
    // Should only need to be loaded once, but added here to make the decoding easier
    // UpdatePalette_FastLED_TargetPalette();  // Cant be here, since it updates with EACH pixel, not just once



  // uint32_t colour0 = RgbcctColor::GetU32Colour(pCONT_lAni->segments[0].rgbcctcolors[0]);
  // uint32_t colour1 = RgbcctColor::GetU32Colour(pCONT_lAni->segments[0].rgbcctcolors[1]);
  // uint32_t colour2 = RgbcctColor::GetU32Colour(pCONT_lAni->segments[0].rgbcctcolors[2]);
  //     CRGB prim = pCONT_lAni->col_to_crgb(colour0);
  //     CRGB sec  = pCONT_lAni->col_to_crgb(colour1);
  //     CRGB tri  = pCONT_lAni->col_to_crgb(colour2);



  //     currentPalette = CRGBPalette16(prim,sec,tri); 



    // ALOG_INF(PSTR("pixel_position_adjust %d"), pixel_position_adjust);
    // FastLED Get ColorFromPalette that is CRGBPalette16
    fastled_col = ColorFromPalette( currentPalette, pixel_position_adjust, brightness_scale, NOBLEND);// (pCONT_lAni->paletteBlend == 3)? NOBLEND:LINEARBLEND);
  
    // uint32_t fastled_col32 = 0;
    // fastled_col32 = fastled_col.r*65536 +  fastled_col.g*256 +  fastled_col.b;
    
    // if(discrete_colours_in_palette != nullptr){ *discrete_colours_in_palette = colours_in_palette; }

    colour = RgbcctColor(fastled_col.r, fastled_col.g, fastled_col.b) ;// RgbcctColor::GetRgbcctFromU32Colour(fastled_col32);

  } // END of CRGBPalette's

  return colour;
}


void mPalette::LoadPalette_CRGBPalette16_Static(uint8_t palette_id)
{

  /******************************************************
   * PALETTELIST_STATIC_CRGBPALETTE16__IDS
   ******************************************************/
  switch (palette_id)
  {
    default:
    case PALETTELIST_STATIC_CRGBPALETTE16__PARTY_COLOUR__ID:    currentPalette = PartyColors_p;   break;
    case PALETTELIST_STATIC_CRGBPALETTE16__CLOUD_COLOURS__ID:   currentPalette = CloudColors_p;   break;
    case PALETTELIST_STATIC_CRGBPALETTE16__LAVA_COLOURS__ID:    currentPalette = LavaColors_p;    break;
    case PALETTELIST_STATIC_CRGBPALETTE16__OCEAN_COLOUR__ID:    currentPalette = OceanColors_p;   break;
    case PALETTELIST_STATIC_CRGBPALETTE16__FOREST_COLOUR__ID:   currentPalette = ForestColors_p;  break;
    case PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID:  currentPalette = RainbowColors_p; break;
    case PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_STRIPE_COLOUR__ID: currentPalette = RainbowStripeColors_p; break;
  }

  /******************************************************
   * PALETTELIST_CRGBPALETTE16_GRADIENT___PALETTES__IDS
   ******************************************************/
  if(IsWithinRange(palette_id, PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID, PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__ATLANTICA__ID))
  {
    uint16_t gradient_id = palette_id - PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID;
    byte tcp[72]; //support gradient palettes with up to 18 entries
    memcpy_P(tcp, (byte*)pgm_read_dword(&(gGradientPalettes[gradient_id])), 72);
    currentPalette.loadDynamicGradientPalette(tcp);
  }
  
}

#endif // header gaurd
