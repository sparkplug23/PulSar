#include "mPalette.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE

/***
 * 
 * 
 * Move buffer for memory thats in settings, to be protected by getters/setters to remove the need for passing as many classes as possible


 */

/* Null, because instance will be initialized on demand. */
mPalette* mPalette::instance = nullptr;

mPalette* mPalette::GetInstance(){
  if (instance == nullptr){
    instance = new mPalette();
    // PRINT_FLUSHED("instance = new JsonBuilder()");
  }
  return instance;
}

/*******************************************************************************************************************
********************************************************************************************************************
************ START OF PALETTE INITS *********************************************************************************
********************************************************************************************************************
********************************************************************************************************************/

void mPalette::init_PresetColourPalettes(){
  
  #ifdef ENABLE_LOG_LEVEL_DEBUG
  AddLog(LOG_LEVEL_DEBUG,PSTR("init_PresetColourPalettes"));
  #endif
  
  /**
   *  Variable HSBID 
   * */
  for(uint8_t id=0;id<(PALETTELIST_VARIABLE_HSBID_LENGTH_ID-PALETTELIST_VARIABLE_HSBID_01_ID);id++){
    init_PresetColourPalettes_UserFill(id);
  }

  /**
   *  Variable HSBID 
   * */
  for(uint8_t id=0;id<(PALETTELIST_VARIABLE_RGBCCT_LENGTH_ID-PALETTELIST_VARIABLE_RGBCCT_COLOUR_01_ID);id++){
    init_PresetColourPalettes_User_RGBCCT_Fill(id);
  }

  /**
   *  Variable Encoded Generic 
   * */
  for(uint8_t id=0;id<(PALETTELIST_VARIABLE_GENERIC_LENGTH_ID-PALETTELIST_VARIABLE_GENERIC_01_ID);id++){
    init_PresetColourPalettes_User_Generic_Fill(0);
  }

  // DEBUG_LINE_HERE;
  /**
   *  Static 
   * */
  init_ColourPalettes_Autumn_Red();
  init_ColourPalettes_Autumn();
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
  init_ColourPalettes_MicroLEDS_Pastel_01();
  init_ColourPalettes_Shelf_Hearts();
  init_ColourPalettes_Pastel();
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
  init_ColourPalettes_Sunrise_01();
  init_ColourPalettes_Sunrise_02();
  init_ColourPalettes_Sunrise_03();
  init_ColourPalettes_Sunrise_04();
  init_ColourPalettes_Sunset_01();
  init_ColourPalettes_Sunset_02();
  init_ColourPalettes_Custom_User_01();
  init_ColourPalettes_Gradient_SunLevel_Group01_01();
  init_ColourPalettes_Gradient_SunLevel_Group01_02();
  init_ColourPalettes_Gradient_SunLevel_Group01_03();
  init_ColourPalettes_Gradient_SunLevel_Group01_04();
  init_ColourPalettes_Gradient_SunLevel_Group01_05();
  init_ColourPalettes_Gradient_SunLevel_Group01_06();
  init_ColourPalettes_Gradient_SunLevel_Group01_07();

  // DEBUG_LINE_HERE;
}


void mPalette::init_ColourPalettes_Holloween_OP(){
  palettelist.ptr     = &palettelist.holloween_op;  
  CLEAR_PALETTELIST_WITH_PTR(palettelist.ptr);
  palettelist.ptr->id = PALETTELIST_STATIC_HOLLOWEEN_OP_ID;
  palettelist.ptr->colour_map_size    = sizeof(colour_map_holloween_op_id);
  palettelist.ptr->colour_map_id      = (uint8_t*)colour_map_holloween_op_id;
  palettelist.ptr->friendly_name_ctr  = (char*)PM_PALETTE_HOLLOWEEN_OP_NAME_CTR;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}

void mPalette::init_ColourPalettes_Holloween_OGP(){
  palettelist.ptr     = &palettelist.holloween_ogp;    
  CLEAR_PALETTELIST_WITH_PTR(palettelist.ptr);
  palettelist.ptr->id = PALETTELIST_STATIC_HOLLOWEEN_OGP_ID;
  palettelist.ptr->colour_map_size    = sizeof(colour_map_holloween_ogp_id);
  palettelist.ptr->colour_map_id      = (uint8_t*)colour_map_holloween_ogp_id;
  palettelist.ptr->friendly_name_ctr  = (char*)PM_PALETTE_HOLLOWEEN_OGP_NAME_CTR;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}


void mPalette::init_ColourPalettes_Hot_Neon_Pink_With_Navy(){
  palettelist.ptr     = &palettelist.hot_pink_neon_with_navy;  
  CLEAR_PALETTELIST_WITH_PTR(palettelist.ptr);
  palettelist.ptr->id = PALETTELIST_STATIC_HOT_PINK_NEON_WITH_NAVY_ID;
  palettelist.ptr->colour_map_size    = sizeof(PM_PALETTE_SINGLE_HOT_PINK_NEON_WITH_NAVY_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id      = (uint8_t*)PM_PALETTE_SINGLE_HOT_PINK_NEON_WITH_NAVY_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr  = (char*)PM_PALETTE_SINGLE_HOT_PINK_NEON_WITH_NAVY_NAME_CTR;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}

void mPalette::init_ColourPalettes_Single_Fire_01(){
  palettelist.ptr     = &palettelist.single_fire_01;  
  CLEAR_PALETTELIST_WITH_PTR(palettelist.ptr);
  palettelist.ptr->id = PALETTELIST_STATIC_SINGLE_FIRE_01_ID;
  palettelist.ptr->colour_map_size    = sizeof(PM_PALETTE_SINGLE_FIRE_01_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id      = (uint8_t*)PM_PALETTE_SINGLE_FIRE_01_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr  = (char*)PM_PALETTE_SINGLE_FIRE_01_NAME_CTR;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}

void mPalette::init_ColourPalettes_Flower_SweetPeas_01(){
  palettelist.ptr     = &palettelist.flower_sweetpeas_01;  
  CLEAR_PALETTELIST_WITH_PTR(palettelist.ptr);
  palettelist.ptr->id = PALETTELIST_STATIC_FLOWER_SWEATPEAS_01_ID;
  palettelist.ptr->colour_map_size    = sizeof(PM_PALETTE_FLOWER_SWEATPEAS_01_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id      = (uint8_t*)PM_PALETTE_FLOWER_SWEATPEAS_01_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr  = (char*)PM_PALETTE_FLOWER_SWEATPEAS_01_NAME_CTR;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}
void mPalette::init_ColourPalettes_Flower_SweetPeas_02(){
  palettelist.ptr     = &palettelist.flower_sweetpeas_02;  
  CLEAR_PALETTELIST_WITH_PTR(palettelist.ptr);
  palettelist.ptr->id = PALETTELIST_STATIC_FLOWER_SWEATPEAS_02_ID;
  palettelist.ptr->colour_map_size    = sizeof(PM_PALETTE_FLOWER_SWEATPEAS_02_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id      = (uint8_t*)PM_PALETTE_FLOWER_SWEATPEAS_02_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr  = (char*)PM_PALETTE_FLOWER_SWEATPEAS_02_NAME_CTR;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}
void mPalette::init_ColourPalettes_Pink_Purple_01(){
  palettelist.ptr     = &palettelist.pink_purple_01;  
  CLEAR_PALETTELIST_WITH_PTR(palettelist.ptr);
  palettelist.ptr->id = PALETTELIST_STATIC_PINK_PURPLE_01_ID;
  palettelist.ptr->colour_map_size    = sizeof(PM_PALETTE_PINK_PURPLE_01_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id      = (uint8_t*)PM_PALETTE_PINK_PURPLE_01_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr  = (char*)PM_PALETTE_PINK_PURPLE_01_NAME_CTR;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}
void mPalette::init_ColourPalettes_MicroLEDS_Pastel_01(){
  palettelist.ptr     = &palettelist.microleds_pastel_01;  
  CLEAR_PALETTELIST_WITH_PTR(palettelist.ptr);
  palettelist.ptr->id = PALETTELIST_STATIC_MICROLEDS_PASTEL_01_ID;
  palettelist.ptr->colour_map_size    = sizeof(PM_PALETTE_MICROLEDS_PASTEL_01_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id      = (uint8_t*)PM_PALETTE_MICROLEDS_PASTEL_01_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr  = (char*)PM_PALETTE_MICROLEDS_PASTEL_01_NAME_CTR;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}

void mPalette::init_ColourPalettes_Pastel(){
  palettelist.ptr     = &palettelist.pastel;    
  CLEAR_PALETTELIST_WITH_PTR(palettelist.ptr);
  palettelist.ptr->id = PALETTELIST_STATIC_PASTEL_ID;
  palettelist.ptr->colour_map_size    = sizeof(COLOUR_PALETTE_PASTEL_MAP_IDS);
  palettelist.ptr->colour_map_id      = (uint8_t*)COLOUR_PALETTE_PASTEL_MAP_IDS;
  palettelist.ptr->friendly_name_ctr  = (char*)PM_PALETTE_PASTEL_NAME_CTR;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID;
}

void mPalette::init_ColourPalettes_Shelf_Hearts(){
  palettelist.ptr     = &palettelist.shelf_hearts;   
  CLEAR_PALETTELIST_WITH_PTR(palettelist.ptr); 
  palettelist.ptr->id = PALETTELIST_STATIC_SHELF_HEARTS_ID;
  palettelist.ptr->colour_map_size = sizeof(colour_map_shelf_hearts_id);
  palettelist.ptr->colour_map_id = (uint8_t*)colour_map_shelf_hearts_id;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_SHELF_HEARTS_NAME_CTR;  
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}

void mPalette::init_ColourPalettes_Gradient_01(){
  palettelist.ptr     = &palettelist.gradient_01;  
  palettelist.ptr->id = PALETTELIST_STATIC_GRADIENT_01_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_GRADIENT_01_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_GRADIENT_01_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_GRADIENT_01_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_100;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID;
}

void mPalette::init_ColourPalettes_Gradient_02(){
  palettelist.ptr     = &palettelist.gradient_02;  
  palettelist.ptr->id = PALETTELIST_STATIC_GRADIENT_02_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_GRADIENT_02_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_GRADIENT_02_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_GRADIENT_02_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_GRADIENT_ID;
}

void mPalette::init_ColourPalettes_Gradient_Fire_01(){
  palettelist.ptr     = &palettelist.gradient_fire_01;  
  palettelist.ptr->id = PALETTELIST_STATIC_GRADIENT_FIRE_01_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_STATIC_GRADIENT_FIRE_01_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_STATIC_GRADIENT_FIRE_01_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_STATIC_GRADIENT_FIRE_01_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_GRADIENT_ID;
}

void mPalette::init_ColourPalettes_Gradient_Pastel_Tones_Purple(){
  palettelist.ptr     = &palettelist.gradient_pastel_tones_purple;  
  palettelist.ptr->id = PALETTELIST_STATIC_GRADIENT_PASTEL_TONES_PURPLE_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_GRADIENT_PASTEL_TONES_PURPLE_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_GRADIENT_PASTEL_TONES_PURPLE_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_GRADIENT_PASTEL_TONES_PURPLE_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_GRADIENT_ID;
}

void mPalette::init_ColourPalettes_Berry_Green(){
  palettelist.ptr     = &palettelist.berry_green;  
  palettelist.ptr->id = PALETTELIST_STATIC_BERRY_GREEN_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_BERRY_GREEN_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_BERRY_GREEN_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_BERRY_GREEN_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}

void mPalette::init_ColourPalettes_Christmas_01(){
  palettelist.ptr     = &palettelist.christmas_01;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_01_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_01_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_01_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_01_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}

void mPalette::init_ColourPalettes_Christmas_02(){
  palettelist.ptr     = &palettelist.christmas_02;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_02_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_02_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_02_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_02_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}

void mPalette::init_ColourPalettes_Christmas_03(){
  palettelist.ptr     = &palettelist.christmas_03;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_03_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_03_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_03_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_03_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}

void mPalette::init_ColourPalettes_Christmas_04(){
  palettelist.ptr     = &palettelist.christmas_04;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_04_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_04_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_04_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_04_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}

void mPalette::init_ColourPalettes_Christmas_05(){
  palettelist.ptr     = &palettelist.christmas_05;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_05_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_05_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_05_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_05_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}

void mPalette::init_ColourPalettes_Christmas_06(){
  palettelist.ptr     = &palettelist.christmas_06;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_06_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_06_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_06_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_06_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}

void mPalette::init_ColourPalettes_Christmas_07(){
  palettelist.ptr     = &palettelist.christmas_07;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_07_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_07_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_07_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_07_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}

void mPalette::init_ColourPalettes_Christmas_08(){
  palettelist.ptr     = &palettelist.christmas_08;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_08_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_08_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_08_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_08_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}

void mPalette::init_ColourPalettes_Christmas_09(){
  palettelist.ptr     = &palettelist.christmas_09;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_09_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_09_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_09_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_09_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}

void mPalette::init_ColourPalettes_Christmas_10(){
  palettelist.ptr     = &palettelist.christmas_10;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_10_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_10_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_10_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_10_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}

void mPalette::init_ColourPalettes_Christmas_11(){
  palettelist.ptr     = &palettelist.christmas_11;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_11_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_11_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_11_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_11_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}

void mPalette::init_ColourPalettes_Christmas_12(){
  palettelist.ptr     = &palettelist.christmas_12;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_12_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_12_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_12_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_12_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}

void mPalette::init_ColourPalettes_Christmas_13(){
  palettelist.ptr     = &palettelist.christmas_13;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_13_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_13_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_13_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_13_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}

void mPalette::init_ColourPalettes_Christmas_14(){
  palettelist.ptr     = &palettelist.christmas_14;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_14_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_14_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_14_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_14_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}

void mPalette::init_ColourPalettes_Christmas_15(){
  palettelist.ptr     = &palettelist.christmas_15;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_15_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_15_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_15_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_15_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}

void mPalette::init_ColourPalettes_Christmas_16(){
  palettelist.ptr     = &palettelist.christmas_16;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_16_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_16_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_16_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_16_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}

void mPalette::init_ColourPalettes_Sunrise_01(){
  palettelist.ptr     = &palettelist.sunrise_01;  
  palettelist.ptr->id = PALETTELIST_STATIC_SUNRISE_01_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_SUNRISE_01_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_SUNRISE_01_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_SUNRISE_01_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX_ID;
}

void mPalette::init_ColourPalettes_Sunrise_02(){
  palettelist.ptr     = &palettelist.sunrise_02;  
  palettelist.ptr->id = PALETTELIST_STATIC_SUNRISE_02_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_SUNRISE_02_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_SUNRISE_02_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_SUNRISE_02_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX_ID;
}

void mPalette::init_ColourPalettes_Sunrise_03(){
  palettelist.ptr     = &palettelist.sunrise_03;  
  palettelist.ptr->id = PALETTELIST_STATIC_SUNRISE_03_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_SUNRISE_03_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_SUNRISE_03_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_SUNRISE_03_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCCTCOLOUR_WITHINDEX_GRADIENT_ID;
}

void mPalette::init_ColourPalettes_Sunrise_04(){
  palettelist.ptr     = &palettelist.sunrise_04;  
  palettelist.ptr->id = PALETTELIST_STATIC_SUNRISE_04_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_SUNRISE_04_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_SUNRISE_04_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_SUNRISE_04_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCCTCOLOUR_WITHINDEX_GRADIENT_ID;
}

void mPalette::init_ColourPalettes_Sunset_01(){
  palettelist.ptr     = &palettelist.sunset_01;  
  palettelist.ptr->id = PALETTELIST_STATIC_SUNSET_01_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_SUNSET_01_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_SUNSET_01_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_SUNSET_01_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX_ID;
}

void mPalette::init_ColourPalettes_Sunset_02(){
  palettelist.ptr     = &palettelist.sunset_02;  
  palettelist.ptr->id = PALETTELIST_STATIC_SUNSET_02_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_SUNSET_02_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_SUNSET_02_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_SUNSET_02_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCCTCOLOUR_WITHINDEX_GRADIENT_ID;
}

void mPalette::init_ColourPalettes_Custom_User_01(){
  palettelist.ptr     = &palettelist.custom_user_01;  
  palettelist.ptr->id = PALETTELIST_STATIC_CUSTOM_USER_01_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CUSTOM_USER_01_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CUSTOM_USER_01_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CUSTOM_USER_01_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}

void mPalette::init_ColourPalettes_Autumn_Red(){
  palettelist.ptr     = &palettelist.autumn_red;
  palettelist.ptr->id = PALETTELIST_STATIC_AUTUMN_RED_ID;
  palettelist.ptr->colour_map_size = sizeof(colour_map_autumnred_id);
  palettelist.ptr->colour_map_id = (uint8_t*)colour_map_autumnred_id;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_AUTUMN_RED_NAME_CTR;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = true;
}

void mPalette::init_ColourPalettes_Autumn(){
  palettelist.ptr     = &palettelist.autumn;
  palettelist.ptr->id = PALETTELIST_STATIC_AUTUMN_ID;
  palettelist.ptr->colour_map_size = sizeof(colour_map_autumn_id);
  palettelist.ptr->colour_map_id = (uint8_t*)colour_map_autumn_id;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_AUTUMN_NAME_CTR;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = true;
}

void mPalette::init_ColourPalettes_Winter(){
  palettelist.ptr     = &palettelist.winter;
  palettelist.ptr->id = PALETTELIST_STATIC_WINTER_ID;
  palettelist.ptr->colour_map_size = sizeof(COLOUR_PALETTE_WINTER_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)COLOUR_PALETTE_WINTER_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_WINTER_NAME_CTR;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = true;
}

void mPalette::init_ColourPalettes_Ocean_01(){
  palettelist.ptr     = &palettelist.ocean_01;
  palettelist.ptr->id = PALETTELIST_STATIC_OCEAN_01_ID;
  palettelist.ptr->colour_map_size = sizeof(colour_map_ocean_01_id);
  palettelist.ptr->colour_map_id = (uint8_t*)colour_map_ocean_01_id;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_OCEAN_01_NAME_CTR;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = true;
}

void mPalette::init_ColourPalettes_Rainbow(){
  palettelist.ptr     = &palettelist.rainbow;
  palettelist.ptr->id = PALETTELIST_STATIC_RAINBOW_ID;
  palettelist.ptr->colour_map_size = sizeof(colour_map_rainbow_id);
  palettelist.ptr->colour_map_id = (uint8_t*)colour_map_rainbow_id;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_RAINBOW_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID;
}

void mPalette::init_ColourPalettes_RainbowInverted(){
  palettelist.ptr     = &palettelist.rainbow_inverted;
  palettelist.ptr->id = PALETTELIST_STATIC_RAINBOW_INVERTED_ID;
  palettelist.ptr->colour_map_size = sizeof(colour_map_rainbow_inverted_id);
  palettelist.ptr->colour_map_id = (uint8_t*)colour_map_rainbow_inverted_id;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_RAINBOW_INVERTED_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID;
}



void mPalette::init_ColourPalettes_Gradient_SunLevel_Group01_01(){
  palettelist.ptr     = &palettelist.gradient_sunlevel_group01_01;  
  palettelist.ptr->id = PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_01_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_01_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_01_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_01_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCCTCOLOUR_WITHINDEX_GRADIENT_ID;
}
void mPalette::init_ColourPalettes_Gradient_SunLevel_Group01_02(){
  palettelist.ptr     = &palettelist.gradient_sunlevel_group01_02;  
  palettelist.ptr->id = PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_02_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_02_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_02_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_02_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCCTCOLOUR_WITHINDEX_GRADIENT_ID;
}
void mPalette::init_ColourPalettes_Gradient_SunLevel_Group01_03(){
  palettelist.ptr     = &palettelist.gradient_sunlevel_group01_03;  
  palettelist.ptr->id = PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_03_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_03_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_03_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_03_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCCTCOLOUR_WITHINDEX_GRADIENT_ID;
}
void mPalette::init_ColourPalettes_Gradient_SunLevel_Group01_04(){
  palettelist.ptr     = &palettelist.gradient_sunlevel_group01_04;  
  palettelist.ptr->id = PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_04_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_04_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_04_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_04_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCCTCOLOUR_WITHINDEX_GRADIENT_ID;
}
void mPalette::init_ColourPalettes_Gradient_SunLevel_Group01_05(){
  palettelist.ptr     = &palettelist.gradient_sunlevel_group01_05;  
  palettelist.ptr->id = PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_05_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_05_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_05_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_05_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCCTCOLOUR_WITHINDEX_GRADIENT_ID;
}
void mPalette::init_ColourPalettes_Gradient_SunLevel_Group01_06(){
  palettelist.ptr     = &palettelist.gradient_sunlevel_group01_06;  
  palettelist.ptr->id = PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_06_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_06_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_06_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_06_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCCTCOLOUR_WITHINDEX_GRADIENT_ID;
}

void mPalette::init_ColourPalettes_Gradient_SunLevel_Group01_07(){
  palettelist.ptr     = &palettelist.gradient_sunlevel_group01_07;  
  palettelist.ptr->id = PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCCTCOLOUR_WITHINDEX_GRADIENT_ID;
}






void mPalette::init_PresetColourPalettes_UserFill(uint8_t id){

  palettelist.ptr     = &palettelist.hsbid_users[id];
  palettelist.ptr->id = id+PALETTELIST_VARIABLE_HSBID_01_ID;

  // Point to memory location
  palettelist.ptr->colour_map_id = &pCONT_set->Settings.animation_settings.palette_hsbid_users_colour_map[id*20];

  palettelist.ptr->colour_map_size = 20;//Get

  // to be moved into devicelist
  palettelist.ptr->friendly_name_ctr = nullptr; // Used DeviceName
  
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID;

  // fill memory location to defaults
  uint8_t get_pixels_used = GetDefaultColourPaletteUserIDsCount(id);
  // char* test = "test"; //GetDefaultColourPaletteUserIDs_P(id)

  auto id_ptr = GetDefaultColourPaletteUserIDs_P(id);
  // Serial.printf("get_pixels_used=%d\n\r",get_pixels_used); Serial.flush();

  //clear back to none_id
  memset(palettelist.ptr->colour_map_id,COLOUR_MAP_NONE_ID,sizeof(uint8_t)*20);

  // Read from PROGMEM the defaults
  memcpy_P(palettelist.ptr->colour_map_id,id_ptr,sizeof(uint8_t)*get_pixels_used);

  //set pixels in palette
  palettelist.ptr->colour_map_size = get_pixels_used;

}


void mPalette::init_PresetColourPalettes_User_RGBCCT_Fill(uint8_t id){

  palettelist.ptr     = &palettelist.rgbcct_users[id];
  palettelist.ptr->id = id+PALETTELIST_VARIABLE_RGBCCT_COLOUR_01_ID;

  // Point to memory location
  palettelist.ptr->colour_map_id = &pCONT_set->Settings.animation_settings.palette_rgbcct_users_colour_map[id*5];
  palettelist.ptr->colour_map_size = 5; // each is 5 bytes wide, 1 pixel = 5 bytes total

  // to be moved into devicelist
  palettelist.ptr->friendly_name_ctr = nullptr;//pCONT_set->Settings.animation_settings.palette_user_variable_name_list_ctr;//&palettelist_variable_users_ctr[ptr->id][0];
  
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX_ID;

}


void mPalette::init_PresetColourPalettes_User_Generic_Fill(uint8_t id){

  /**
   * [0] pixel_size, used with map_type to generate map_size (ie colour_map_size = pixel_width*pixel_count)
   * [1] map_type      eg (6=rgb no_index, )
   * [2] 
   * [3]
   * [4]
   * [5->length] Encoded palette colour values
   * */

  palettelist.ptr     = &palettelist.encoded_users;
  palettelist.ptr->id = id+PALETTELIST_VARIABLE_GENERIC_01_ID;

  //colour map size be calculated by map type * pixel count?

  // palettelist.ptr->colour_map_size = pCONT_set->Settings.animation_settings.palette_encoded_users_colour_map[0] < 100 ?  pCONT_set->Settings.animation_settings.palette_encoded_users_colour_map[0] : 0; 
  palettelist.ptr->flags.fMapIDs_Type = pCONT_set->Settings.animation_settings.palette_encoded_users_colour_map[1]; 

  uint8_t pixel_width = GetPixelsWithByMapIDType(palettelist.ptr->flags.fMapIDs_Type);
  // AddLog(LOG_LEVEL_TEST, PSTR("pixel_width %d"),pixel_width);
  palettelist.ptr->colour_map_size = pixel_width*pCONT_set->Settings.animation_settings.palette_encoded_users_colour_map[0];
  


  // palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  // palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX_ID;

  //need to create "palette encoded" to "saved buffer" function ie decoder
  //Starts after the encoded info is taken out
  palettelist.ptr->colour_map_id = &pCONT_set->Settings.animation_settings.palette_encoded_users_colour_map[5];//id*5];

  // to be moved into devicelist
  palettelist.ptr->friendly_name_ctr = nullptr;//pCONT_set->Settings.animation_settings.palette_user_variable_name_list_ctr;//&palettelist_variable_users_ctr[ptr->id][0];
  
  // AddLog(LOG_LEVEL_TEST, PSTR("init_PresetColourPalettes_User_Generic_Fill %d"),palettelist.ptr->colour_map_id[0]);
  // AddLog(LOG_LEVEL_TEST, PSTR("init_PresetColourPalettes_User_Generic_Fill size %d"),palettelist.ptr->colour_map_size);

  // AddLog(LOG_LEVEL_TEST, PSTR("pixels in map test size %d"),GetPixelsInMap(palettelist.ptr));
  
  //     RgbcctColor colour = mPaletteI->GetColourFromPalette(mPaletteI->GetPalettePointerByID(15),0);

  //     AddLog(LOG_LEVEL_TEST, PSTR("colour_pal  =%d,%d,%d,%d,%d"), colour.R, colour.G, colour.B, colour.WW, colour.WC);


}


/*******************************************************************************************************************
********************************************************************************************************************
************ START OF PALETTE INITS *********************************************************************************
********************************************************************************************************************
********************************************************************************************************************/



/*******************************************************************************************************************
********************************************************************************************************************
************ START OF Shared functions ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/


mPalette::PALETTELIST::PALETTE* mPalette::GetPalettePointerByID(uint8_t id){

  switch(id){
    default:
    AddLog(LOG_LEVEL_WARN, PSTR("GetPalettePointerByID=%d DEFAULT"),id);
    /**
     * Variable User HSBID
     * */
    case PALETTELIST_VARIABLE_HSBID_01_ID: return &palettelist.hsbid_users[0];
    case PALETTELIST_VARIABLE_HSBID_02_ID: return &palettelist.hsbid_users[1];
    case PALETTELIST_VARIABLE_HSBID_03_ID: return &palettelist.hsbid_users[2];
    case PALETTELIST_VARIABLE_HSBID_04_ID: return &palettelist.hsbid_users[3];
    case PALETTELIST_VARIABLE_HSBID_05_ID: return &palettelist.hsbid_users[4];
    case PALETTELIST_VARIABLE_HSBID_06_ID: return &palettelist.hsbid_users[5];
    case PALETTELIST_VARIABLE_HSBID_07_ID: return &palettelist.hsbid_users[6];
    case PALETTELIST_VARIABLE_HSBID_08_ID: return &palettelist.hsbid_users[7];
    case PALETTELIST_VARIABLE_HSBID_09_ID: return &palettelist.hsbid_users[8];
    case PALETTELIST_VARIABLE_HSBID_10_ID: return &palettelist.hsbid_users[9];
    #ifdef ENABLE_INTERFACE_LIGHT_HSBID_USERS_EXTENDED
    case PALETTELIST_VARIABLE_HSBID_11_ID: return &palettelist.hsbid_users[10];
    case PALETTELIST_VARIABLE_HSBID_12_ID: return &palettelist.hsbid_users[11];
    case PALETTELIST_VARIABLE_HSBID_13_ID: return &palettelist.hsbid_users[12];
    case PALETTELIST_VARIABLE_HSBID_14_ID: return &palettelist.hsbid_users[13];
    case PALETTELIST_VARIABLE_HSBID_15_ID: return &palettelist.hsbid_users[14];
    case PALETTELIST_VARIABLE_HSBID_16_ID: return &palettelist.hsbid_users[15];
    case PALETTELIST_VARIABLE_HSBID_17_ID: return &palettelist.hsbid_users[16];
    case PALETTELIST_VARIABLE_HSBID_18_ID: return &palettelist.hsbid_users[17];
    case PALETTELIST_VARIABLE_HSBID_19_ID: return &palettelist.hsbid_users[18];
    case PALETTELIST_VARIABLE_HSBID_20_ID: return &palettelist.hsbid_users[19];
    #endif // ENABLE_INTERFACE_LIGHT_HSBID_USERS_EXTENDED
    /**
     * Variable User RGBCCT
     * */
    case PALETTELIST_VARIABLE_RGBCCT_COLOUR_01_ID: return &palettelist.rgbcct_users[0];
    case PALETTELIST_VARIABLE_RGBCCT_COLOUR_02_ID: return &palettelist.rgbcct_users[1];
    case PALETTELIST_VARIABLE_RGBCCT_COLOUR_03_ID: return &palettelist.rgbcct_users[2];
    case PALETTELIST_VARIABLE_RGBCCT_COLOUR_04_ID: return &palettelist.rgbcct_users[3];
    case PALETTELIST_VARIABLE_RGBCCT_COLOUR_05_ID: return &palettelist.rgbcct_users[4];
    /**
     * Variable User Encoded Generic
     * */
    case PALETTELIST_VARIABLE_GENERIC_01_ID:       return &palettelist.encoded_users;
    /**
     * Variable User HSBID
     * */
    case PALETTELIST_STATIC_SHELF_HEARTS_ID:    return &palettelist.shelf_hearts;
    case PALETTELIST_STATIC_HOLLOWEEN_OP_ID:    return &palettelist.holloween_op;
    case PALETTELIST_STATIC_HOLLOWEEN_OGP_ID:   return &palettelist.holloween_ogp;
    case PALETTELIST_STATIC_HOT_PINK_NEON_WITH_NAVY_ID:    return &palettelist.hot_pink_neon_with_navy;
    case PALETTELIST_STATIC_SINGLE_FIRE_01_ID:    return &palettelist.single_fire_01;
    case PALETTELIST_STATIC_FLOWER_SWEATPEAS_01_ID:    return &palettelist.flower_sweetpeas_01;
    case PALETTELIST_STATIC_FLOWER_SWEATPEAS_02_ID:    return &palettelist.flower_sweetpeas_02;
    case PALETTELIST_STATIC_PINK_PURPLE_01_ID:    return &palettelist.pink_purple_01;
    case PALETTELIST_STATIC_MICROLEDS_PASTEL_01_ID:    return &palettelist.microleds_pastel_01;
    case PALETTELIST_STATIC_RAINBOW_ID:         return &palettelist.rainbow;
    case PALETTELIST_STATIC_RAINBOW_INVERTED_ID:return &palettelist.rainbow_inverted;
    case PALETTELIST_STATIC_PASTEL_ID:          return &palettelist.pastel;
    case PALETTELIST_STATIC_WINTER_ID:          return &palettelist.winter;
    case PALETTELIST_STATIC_AUTUMN_ID:          return &palettelist.autumn;
    case PALETTELIST_STATIC_AUTUMN_RED_ID:      return &palettelist.autumn_red;
    case PALETTELIST_STATIC_OCEAN_01_ID:        return &palettelist.ocean_01;
    case PALETTELIST_STATIC_GRADIENT_01_ID:     return &palettelist.gradient_01;
    case PALETTELIST_STATIC_GRADIENT_02_ID:     return &palettelist.gradient_02;
    case PALETTELIST_STATIC_GRADIENT_FIRE_01_ID:     return &palettelist.gradient_fire_01;
    case PALETTELIST_STATIC_GRADIENT_PASTEL_TONES_PURPLE_ID:     return &palettelist.gradient_pastel_tones_purple;
    case PALETTELIST_STATIC_BERRY_GREEN_ID:     return &palettelist.berry_green;
    case PALETTELIST_STATIC_SUNRISE_01_ID:      return &palettelist.sunrise_01;
    case PALETTELIST_STATIC_SUNRISE_02_ID:      return &palettelist.sunrise_02;
    case PALETTELIST_STATIC_SUNRISE_03_ID:      return &palettelist.sunrise_03;
    case PALETTELIST_STATIC_SUNRISE_04_ID:      return &palettelist.sunrise_04;
    case PALETTELIST_STATIC_SUNSET_01_ID:      return &palettelist.sunset_01;
    case PALETTELIST_STATIC_SUNSET_02_ID:      return &palettelist.sunset_02;
    case PALETTELIST_STATIC_CHRISTMAS_01_ID:     return &palettelist.christmas_01;
    case PALETTELIST_STATIC_CHRISTMAS_02_ID:     return &palettelist.christmas_02;
    case PALETTELIST_STATIC_CHRISTMAS_03_ID:     return &palettelist.christmas_03;
    case PALETTELIST_STATIC_CHRISTMAS_04_ID:     return &palettelist.christmas_04;
    case PALETTELIST_STATIC_CHRISTMAS_05_ID:     return &palettelist.christmas_05;
    case PALETTELIST_STATIC_CHRISTMAS_06_ID:     return &palettelist.christmas_06;
    case PALETTELIST_STATIC_CHRISTMAS_07_ID:     return &palettelist.christmas_07;
    case PALETTELIST_STATIC_CHRISTMAS_08_ID:     return &palettelist.christmas_08;
    case PALETTELIST_STATIC_CHRISTMAS_09_ID:     return &palettelist.christmas_09;
    case PALETTELIST_STATIC_CHRISTMAS_10_ID:     return &palettelist.christmas_10;
    case PALETTELIST_STATIC_CHRISTMAS_11_ID:     return &palettelist.christmas_11;
    case PALETTELIST_STATIC_CHRISTMAS_12_ID:     return &palettelist.christmas_12;
    case PALETTELIST_STATIC_CHRISTMAS_13_ID:     return &palettelist.christmas_13;
    case PALETTELIST_STATIC_CHRISTMAS_14_ID:     return &palettelist.christmas_14;
    case PALETTELIST_STATIC_CHRISTMAS_15_ID:     return &palettelist.christmas_15;
    case PALETTELIST_STATIC_CHRISTMAS_16_ID:     return &palettelist.christmas_16;
    case PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_01_ID:     return &palettelist.gradient_sunlevel_group01_01;
    case PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_02_ID:     return &palettelist.gradient_sunlevel_group01_02;
    case PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_03_ID:     return &palettelist.gradient_sunlevel_group01_03;
    case PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_04_ID:     return &palettelist.gradient_sunlevel_group01_04;
    case PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_05_ID:     return &palettelist.gradient_sunlevel_group01_05;
    case PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_06_ID:     return &palettelist.gradient_sunlevel_group01_06;
    case PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_ID:     return &palettelist.gradient_sunlevel_group01_07;
    case PALETTELIST_STATIC_CUSTOM_USER_01_ID: return &palettelist.custom_user_01;
  }

}


//?
// RgbcctColor mPalette::GetColourFromPaletteAsPercentage(PALETTELIST::PALETTE *ptr, uint16_t pixel_num, int16_t *pixel_position)
// {


// }

RgbcctColor mPalette::GetColourFromPalette(PALETTELIST::PALETTE *ptr, uint16_t pixel_num, int16_t *pixel_position)
{

  RgbcctColor colour = RgbcctColor(0);
  // AddLog(LOG_LEVEL_TEST, PSTR("ptr->colour_map_size=%d"),ptr->colour_map_size );
  uint8_t palette_elements[ptr->colour_map_size];
  uint16_t index_relative = 0; // get expected pixel position
  uint16_t expected_pixel_count = 0; // get expected pixel position

  if(pixel_position == nullptr){
    // if none passed, create dummy one to write in to
    int16_t dummy_value;
    pixel_position = &dummy_value;
  }

  if(ptr->id < PALETTELIST_VARIABLE_HSBID_LENGTH_ID){
    memcpy(&palette_elements,ptr->colour_map_id,sizeof(uint8_t)*ptr->colour_map_size);
  }else
  if(ptr->id < PALETTELIST_VARIABLE_RGBCCT_LENGTH_ID){
    memcpy(&palette_elements,ptr->colour_map_id,sizeof(uint8_t)*ptr->colour_map_size);
  }else
  if(ptr->id < PALETTELIST_VARIABLE_GENERIC_LENGTH_ID){
    memcpy(&palette_elements,ptr->colour_map_id,sizeof(uint8_t)*ptr->colour_map_size);
  }else{ // progmem
    memcpy_P(&palette_elements,ptr->colour_map_id,sizeof(uint8_t)*ptr->colour_map_size);
  }

  //#ifdef ENABLE_LOG_LEVEL_DEBUG
  // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "fMapIDs_Type[%d]=%d"),ptr->id,ptr->flags.fMapIDs_Type);
  // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "colour_map_size[%d]=%d"),ptr->id,ptr->colour_map_size);
  // #endif

  switch(ptr->flags.fMapIDs_Type){
    default:
    case MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID:{
      if(pixel_position != nullptr){ *pixel_position = 1; }
      colour = RgbcctColor(
        GetHsbColour(palette_elements[pixel_num])
      );
    }
    break;
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_GRADIENT_ID:{

      index_relative = pixel_num*4; // get expected pixel position
      expected_pixel_count = ptr->colour_map_size/4; // get expected pixel position
      
      if(pixel_position != nullptr){ *pixel_position = palette_elements[index_relative]; }
      // Get colour
      colour = RgbcctColor(
        palette_elements[index_relative+1],
        palette_elements[index_relative+2],
        palette_elements[index_relative+3],
        0,
        0
      );

    }break;
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID:
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_AND_SETALL_ID: //0,h,s,b (only if uint16 is allowed for hue>255)
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_ID:{
      
      index_relative = pixel_num*2; // get expected pixel position
      expected_pixel_count = ptr->colour_map_size/2; // get expected pixel position
      
      if(pixel_position != nullptr){ *pixel_position = palette_elements[index_relative]; }

      colour = RgbcctColor(
        GetHsbColour(palette_elements[index_relative+1])
      );
      
    }
    break;
    case MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID:{
      // Get expected pixel position
      index_relative = pixel_num*3;
      // Get colour
      colour = RgbcctColor(
        palette_elements[index_relative  ],
        palette_elements[index_relative+1],
        palette_elements[index_relative+2],
        0,
        0
      );
    }
    break;
    case MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX_ID:{
      // Get expected pixel position
      index_relative = pixel_num*5;
      // Get colour
      colour = RgbcctColor(
        palette_elements[index_relative  ],
        palette_elements[index_relative+1],
        palette_elements[index_relative+2],
        palette_elements[index_relative+3],
        palette_elements[index_relative+4]
      );
      // Serial.printf("MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX_ID %d\n\r", index_relative); 
      //AddLog_Array(LOG_LEVEL_COMMANDS, "palette_elements all", palette_elements, 5);
    }
    break;
    case MAPIDS_TYPE_RGBCCTCOLOUR_WITHINDEX_GRADIENT_ID:{

      index_relative = pixel_num*6; // get expected pixel position
      
      expected_pixel_count = ptr->colour_map_size/6; // get expected pixel position
      
      if(pixel_position != nullptr){ *pixel_position = palette_elements[index_relative]; }
      // Get colour
      colour = RgbcctColor(
        palette_elements[index_relative+1],
        palette_elements[index_relative+2],
        palette_elements[index_relative+3],
        palette_elements[index_relative+4],
        palette_elements[index_relative+5]
      );
      // Serial.println(colour.R);

    }break;
  }

  if(pCONT_iLight->animation.flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique){
    HsbColor hsb = RgbColor(colour.R, colour.G, colour.B);
    hsb.S = GetRandomSaturationVariation(100,8,92,100)/100.0f;
    colour = hsb;
    // add to keep ww/cw
  }

  return colour;

} // end function


HsbColor mPalette::GetHsbColour(uint8_t id){

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
  if(id>=COLOUR_MAP_RED_ID    && id<=COLOUR_MAP_RED_PASTEL0_ID){
    colour.H = pgm_read_float(&colour_float_map_values[0]); // progmem here might not be useful
  }else
  if(id>=COLOUR_MAP_ORANGE_ID && id<=COLOUR_MAP_ORANGE_PASTEL0_ID){
    colour.H = pgm_read_float(colour_float_map_values+1);
  }else
  if(id>=COLOUR_MAP_LIGHTORANGE_ID && id<=COLOUR_MAP_LIGHTORANGE_PASTEL0_ID){
    colour.H = pgm_read_float(colour_float_map_values+2);
  }else
  if(id>=COLOUR_MAP_WHITE_ID && id<=COLOUR_MAP_WHITE_PASTEL0_ID){
    colour.H = pgm_read_float(colour_float_map_values+3);
  }else
  if(id>=COLOUR_MAP_YELLOW_ID && id<=COLOUR_MAP_YELLOW_PASTEL0_ID){
    colour.H = pgm_read_float(colour_float_map_values+4);
  }else
  if(id>=COLOUR_MAP_LIMEGREEN_ID && id<=COLOUR_MAP_LIMEGREEN_PASTEL0_ID){
    colour.H = pgm_read_float(colour_float_map_values+5);
  }else
  if(id>=COLOUR_MAP_GREEN_ID && id<=COLOUR_MAP_GREEN_PASTEL0_ID){
    colour.H = pgm_read_float(colour_float_map_values+6);
  }else
  if(id>=COLOUR_MAP_CYAN_ID && id<=COLOUR_MAP_CYAN_PASTEL0_ID){
    colour.H = pgm_read_float(colour_float_map_values+7);
  }else
  if(id>=COLOUR_MAP_BLUE_ID && id<=COLOUR_MAP_BLUE_PASTEL0_ID){
    colour.H = pgm_read_float(colour_float_map_values+8);
  }else
  if(id>=COLOUR_MAP_BLUEPURPLE_ID && id<=COLOUR_MAP_BLUEPURPLE_PASTEL0_ID){
    colour.H = pgm_read_float(colour_float_map_values+9);
  }else
  if(id>=COLOUR_MAP_PURPLE_ID && id<=COLOUR_MAP_PURPLE_PASTEL0_ID){
    colour.H = pgm_read_float(colour_float_map_values+10);
  }else
  if(id>=COLOUR_MAP_PINK_ID && id<=COLOUR_MAP_PINK_PASTEL0_ID){
    colour.H = pgm_read_float(colour_float_map_values+11);
  }else
  if(id>=COLOUR_MAP_HOTPINK_ID && id<=COLOUR_MAP_HOTPINK_PASTEL0_ID){
    colour.H = pgm_read_float(colour_float_map_values+12);
  }

  // AddLog(LOG_LEVEL_INFO,PSTR("id mod 12 = %d   %d"), id%12, id);
  // colour.S = (12-sat_val)

  return colour;

}


int8_t mPalette::CheckPaletteIsEditable(PALETTELIST::PALETTE *ptr){
  if(( (ptr->id>=PALETTELIST_VARIABLE_HSBID_01_ID)&&
      (ptr->id<PALETTELIST_VARIABLE_HSBID_LENGTH_ID) )
    || 
    ( (ptr->id>=PALETTELIST_VARIABLE_RGBCCT_COLOUR_01_ID)&&
      (ptr->id<PALETTELIST_VARIABLE_RGBCCT_LENGTH_ID) )
    ){
    return true;
  }
  return false;
}

bool mPalette::CheckPaletteByIDIsEditable(uint8_t id){

  if((id>=PALETTELIST_VARIABLE_HSBID_01_ID)&&(id<PALETTELIST_VARIABLE_HSBID_LENGTH_ID)){
    return true;
  }
  if((id>=PALETTELIST_VARIABLE_RGBCCT_COLOUR_01_ID)&&(id<PALETTELIST_VARIABLE_RGBCCT_LENGTH_ID)){
    return true;
  }
  if((id>=PALETTELIST_VARIABLE_GENERIC_01_ID)&&(id<PALETTELIST_VARIABLE_GENERIC_LENGTH_ID)){
    return true;
  }
  


  return false;
}


int8_t mPalette::GetPaletteIDbyName(const char* c){
  if(c=='\0'){
    return -1;
  }

  PALETTELIST::PALETTE *ptr = nullptr;
  char buffer[50];
  int8_t index_found = -1;

  // Check for matches with variables names  
  // if ((index_found = pCONT_sup->GetDListIDbyNameCtr(buffer, sizeof(buffer), c, pCONT_set->Settings.animation_settings.palette_user_variable_name_list_ctr)) >= 0) {
  //   // index_found = STATE_NUMBER_OFF_ID;    
  //     AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("index_found = %d"),index_found);    
  //     return index_found;
  // }

  // Check against stored progmem static names
  for(uint8_t ii=0;ii<PALETTELIST_STATIC_LENGTH_ID;ii++){
    ptr = GetPalettePointerByID(ii);
    // AddLog(LOG_LEVEL_ERROR, PSTR("ptr->id %d"),ptr->id);
    // AddLog(LOG_LEVEL_ERROR, PSTR("ptr->friendly_name_ctr %s"),ptr->friendly_name_ctr);

    if(ptr->friendly_name_ctr == nullptr){ 
      #ifdef ENABLE_LOG_LEVEL_COMMANDS
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("ptr->friendly_name_ctr == nullptr %d %s"),ii,c);     //skipping names not set, including variables names which dont use pointer to name (unless I point to its place later, and include its name length?) Store variable name in dlist 
      // move variable name to join standard devicename and just include as indexed? ie 0-20 is their names?
      #endif // ENABLE_LOG_LEVEL_COMMANDS
    }
    if(ptr->friendly_name_ctr != nullptr){ 
      if(ii>PALETTELIST_VARIABLE_HSBID_LENGTH_ID){
        // if(strcmp_P(c,ptr->friendly_name_ctr)==0){
        //   AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("strcmp(c,ptr->friendly_name_ctr)=>%d"),ii);
        //   return ii;
        // }

        if(mSupport::CheckCommand_P(c, ptr->friendly_name_ctr)){ 

          AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("strcmp(c,ptr->friendly_name_ctr)=>%d"),ii);
          return ii;            
           
          }
  




      }
    }
  }

  // Check for default user names 
  char name_ctr[20];
  for(uint8_t ii=0;ii<(PALETTELIST_VARIABLE_HSBID_LENGTH_ID-PALETTELIST_VARIABLE_HSBID_01_ID);ii++){
    memset(name_ctr,0,sizeof(name_ctr));
    sprintf_P(name_ctr,PSTR("%s %02d\0"),D_PALETTE_HSBID_NAME_CTR,ii);
    // Default names
    if(strcmp(c,name_ctr)==0){
      return PALETTELIST_VARIABLE_HSBID_01_ID+ii;
    }
  }

  // Check for default user names 
  for(uint8_t ii=0;ii<(PALETTELIST_VARIABLE_RGBCCT_LENGTH_ID-PALETTELIST_VARIABLE_RGBCCT_COLOUR_01_ID);ii++){
    memset(name_ctr,0,sizeof(name_ctr));
    sprintf_P(name_ctr,PSTR("%s %02d\0"),D_PALETTE_RGBCCT_COLOURS_NAME_CTR,ii);
    // Default names

    AddLog(LOG_LEVEL_TEST, PSTR("Searching \"%s\""),name_ctr);

    if(strcmp(c,name_ctr)==0){
      return PALETTELIST_VARIABLE_RGBCCT_COLOUR_01_ID+ii;
    }
  }

  // Finally, check if palette name was simply its index as string
  uint8_t found_index = (!strlen(c)) ? 0 : atoi(c);
  if(WithinLimits(found_index, (uint8_t)0, (uint8_t)PALETTELIST_STATIC_LENGTH_ID)){
    return found_index;
  }

  return -1;

}


// Check for friendly names first
const char* mPalette::GetPaletteFriendlyName(char* buffer, uint8_t buflen){
  // #ifdef USE_MODULE_LIGHTS_ANIMATOR
  return GetPaletteFriendlyNameByID(pCONT_iLight->animation.palette.id, buffer, buflen);
  // #else
  // return GetPaletteFriendlyNameByID(0, buffer);
  // #endif
} 
const char* mPalette::GetPaletteFriendlyNameByID(uint8_t id, char* buffer, uint8_t buflen){

  PALETTELIST::PALETTE *ptr = GetPalettePointerByID(id);
  
  if(id<PALETTELIST_VARIABLE_GENERIC_LENGTH_ID){
    // dList
    return pCONT_sup->GetTextIndexed(buffer, buflen, id, pCONT_set->Settings.device_name_buffer.name_buffer);

    // return pCONT_sup->GetTextIndexed_P(buffer, buflen, id, pCONT_set->Settings.animation_settings.palette_user_variable_name_list_ctr);
  }
  else{
    // Progmem
    snprintf_P(buffer, buflen, "%S", ptr->friendly_name_ctr!=nullptr?ptr->friendly_name_ctr:WARNING_NOTHANDLED_CTR);
    return buffer;
  }

}


const char* mPalette::GetPaletteName(char* buffer, uint8_t buflen){
  
// #ifdef USE_MODULE_LIGHTS_ANIMATOR
  return GetPaletteFriendlyNameByID(pCONT_iLight->animation.palette.id, buffer, buflen);
  // return GetPaletteNameByID(pCONT_iLight->animation.palette.id, buffer, buflen);
  // #else
  // return GetPaletteNameByID(0, buffer, buflen);
  // #endif
} 
const char* mPalette::GetPaletteNameByID(uint8_t id, char* buffer, uint8_t buflen){  
  PALETTELIST::PALETTE *ptr = GetPalettePointerByID(id);
  return GetPaletteFriendlyNameByID(id, buffer, buflen);
  // return ptr->friendly_name_ctr!=nullptr?ptr->friendly_name_ctr:WARNING_NOTHANDLED_CTR;
}


uint8_t mPalette::GetPixelsWithByMapIDType(uint8_t fMapIDs_Type){
  uint16_t pixel_width = 0;

  switch(fMapIDs_Type){
    case MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID: pixel_width = 1; break;
    case MAPIDS_TYPE_HSBCOLOUR_NOINDEX_ID: pixel_width = 1; break;
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_ID: pixel_width = 2; break;
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_AND_SETALL_ID: pixel_width = 2; break;
    case MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID: pixel_width = 3; break;
    case MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX_ID: pixel_width = 5; break;
    case MAPIDS_TYPE_RGBCCTCOLOUR_WITHINDEX_GRADIENT_ID: pixel_width = 6; break;
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_ID: pixel_width = 4; break;
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_AND_SETALL_ID: pixel_width = 4; break;
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID: pixel_width = 2; break;
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_GRADIENT_ID: pixel_width = 4; break;
  }

  return pixel_width;
}


uint16_t mPalette::GetPixelsInMap(PALETTELIST::PALETTE *ptr, uint8_t pixel_width_contrained_limit){
  uint16_t pixel_width = 0;
  uint16_t pixel_length = 0;
  switch(ptr->flags.fMapIDs_Type){
    default:
    
    AddLog(LOG_LEVEL_ERROR, PSTR("ERROR pixel_length=%d"),pixel_length);

    break;
    case MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID: pixel_width = 1; break;
    case MAPIDS_TYPE_HSBCOLOUR_NOINDEX_ID: pixel_width = 1; break;
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_ID: pixel_width = 2; break;
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_AND_SETALL_ID: pixel_width = 2; break;
    case MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID: pixel_width = 3; break;
    case MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX_ID: pixel_width = 5; break;
    case MAPIDS_TYPE_RGBCCTCOLOUR_WITHINDEX_GRADIENT_ID: pixel_width = 6; break;
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_ID: pixel_width = 4; break;
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_AND_SETALL_ID: pixel_width = 4; break;
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID: pixel_width = 2; break;
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_GRADIENT_ID: pixel_width = 4; break;
  }

  // recalculate size, it might not be the same
  // ptr->colour_map_size = 
  // GetDefaultColourPaletteUserIDsCount
  // GetColourMapSize()
  // Dont do this everytime, as its costly on memory, only update on change

  // if(pixel_width){ //so we can divide if not 0
    pixel_length = ptr->colour_map_size/pixel_width; 
    // AddLog(LOG_LEVEL_TEST, PSTR("pixel_length=%d %d %d"),pixel_length,ptr->colour_map_size,pixel_width);
  // }else{
  //   pixel_length = ptr->colour_map_size/pixel_width
  // }
  //constrain limit if max
  // if(pixel_width_contrained_limit>=0){
  //   pixel_length = constrain(pixel_length,1,pixel_width_contrained_limit);
  //   // AddLog(LOG_LEVEL_TEST, PSTR("pixel_width_contrained_limit=%d, pixel_length=%d"),pixel_width_contrained_limit, pixel_length);
  // }
  return pixel_length;
}


/**
 * Refresh value stored in palette, and return new value
 * */
uint8_t mPalette::GetColourMapSizeByPaletteID(uint8_t palette_id){

  uint8_t new_size = 0;
  PALETTELIST::PALETTE *ptr_tmp = GetPalettePointerByID(palette_id);

  if(palette_id<PALETTELIST_VARIABLE_HSBID_LENGTH_ID){
    //Count until "not set value"  
    // new_size = GetDefaultColourPaletteUserIDsCount(palette.id);

      uint8_t pixel_active = 0;
      uint8_t byte_read = 0;
      while(pixel_active<20){
        // byte_read = pgm_read_byte(id_ptr+pixel_active); // This doesnt seem right??
        byte_read = pCONT_set->Settings.animation_settings.palette_hsbid_users_colour_map[(20*palette_id)+pixel_active];
        // Serial.printf("byte_read=%d",byte_read);
        if(byte_read!=COLOUR_MAP_NONE_ID){//} && byte_read<COLOUR_MAP_PALETTE_OPTIONS_END_ID){
          pixel_active++;
        }else{
          break;
        }
      }
      new_size = pixel_active; // new count from local stored memory this time

  
  ptr_tmp->colour_map_size = new_size;


  }else
  if((palette_id>=PALETTELIST_VARIABLE_RGBCCT_COLOUR_01_ID)&&(palette_id<PALETTELIST_VARIABLE_RGBCCT_LENGTH_ID)){

    new_size = 5; // only 1*5
    

  ptr_tmp->colour_map_size = new_size;

  }else
  if((palette_id>=PALETTELIST_VARIABLE_GENERIC_01_ID)&&(palette_id<PALETTELIST_VARIABLE_GENERIC_LENGTH_ID)){

    new_size = ptr_tmp->colour_map_size; // currently refreshed when init is triggered, about this may need changing, leaving unchanged for now
  }

  return new_size;
}



const uint8_t* mPalette::GetDefaultColourPaletteUserIDs_P(uint8_t id){
  switch(id){
    default: 
    case PALETTELIST_VARIABLE_HSBID_01_ID: return DEFAULT_COLOUR_PALETTE_HSBID_01_IDS;
    case PALETTELIST_VARIABLE_HSBID_02_ID: return DEFAULT_COLOUR_PALETTE_HSBID_02_IDS;
    case PALETTELIST_VARIABLE_HSBID_03_ID: return DEFAULT_COLOUR_PALETTE_HSBID_03_IDS;
    case PALETTELIST_VARIABLE_HSBID_04_ID: return DEFAULT_COLOUR_PALETTE_HSBID_04_IDS;
    case PALETTELIST_VARIABLE_HSBID_05_ID: return DEFAULT_COLOUR_PALETTE_HSBID_05_IDS;
    case PALETTELIST_VARIABLE_HSBID_06_ID: return DEFAULT_COLOUR_PALETTE_HSBID_06_IDS;
    case PALETTELIST_VARIABLE_HSBID_07_ID: return DEFAULT_COLOUR_PALETTE_HSBID_07_IDS;
    case PALETTELIST_VARIABLE_HSBID_08_ID: return DEFAULT_COLOUR_PALETTE_HSBID_08_IDS;
    case PALETTELIST_VARIABLE_HSBID_09_ID: return DEFAULT_COLOUR_PALETTE_HSBID_09_IDS;
    case PALETTELIST_VARIABLE_HSBID_10_ID: return DEFAULT_COLOUR_PALETTE_HSBID_10_IDS;
    // case PALETTELIST_VARIABLE_HSBID_11_ID: return DEFAULT_COLOUR_PALETTE_HSBID_11_IDS;
    // case PALETTELIST_VARIABLE_HSBID_12_ID: return DEFAULT_COLOUR_PALETTE_HSBID_12_IDS;
    // case PALETTELIST_VARIABLE_HSBID_13_ID: return DEFAULT_COLOUR_PALETTE_HSBID_13_IDS;
    // case PALETTELIST_VARIABLE_HSBID_14_ID: return DEFAULT_COLOUR_PALETTE_HSBID_14_IDS;
    // case PALETTELIST_VARIABLE_HSBID_15_ID: return DEFAULT_COLOUR_PALETTE_HSBID_15_IDS;
    // case PALETTELIST_VARIABLE_HSBID_16_ID: return DEFAULT_COLOUR_PALETTE_HSBID_16_IDS;
    // case PALETTELIST_VARIABLE_HSBID_17_ID: return DEFAULT_COLOUR_PALETTE_HSBID_17_IDS;
    // case PALETTELIST_VARIABLE_HSBID_18_ID: return DEFAULT_COLOUR_PALETTE_HSBID_18_IDS;
    // case PALETTELIST_VARIABLE_HSBID_19_ID: return DEFAULT_COLOUR_PALETTE_HSBID_19_IDS;
    // case PALETTELIST_VARIABLE_HSBID_20_ID: return DEFAULT_COLOUR_PALETTE_HSBID_20_IDS;
  }
}


uint8_t mPalette::GetDefaultColourPaletteUserIDsCount(uint8_t id){


  // Serial.printf("GetDefaultColourPaletteUserIDsCount\n\r"); Serial.flush();

  auto id_ptr = GetDefaultColourPaletteUserIDs_P(id);
  // Serial.printf(GetDefaultColourPaletteUserIDs_P(id));

  
  // const uint8_t* id_ptr = DEFAULT_COLOUR_PALETTE_HSBID_01_IDS;

  // Serial.printf("GetDefaultColourPaletteUserIDsCountBB=%d\n\r",   pgm_read_byte(id_ptr)); Serial.flush();

  //search for COLOUR_MAP_NONE_ID to signify end, must be less than 20 in length

  uint8_t pixel_active = 0;
  uint8_t byte_read = 0;
  while(pixel_active<20){
    byte_read = pgm_read_byte(id_ptr+pixel_active); // This doesnt seem right??
    // Serial.printf("byte_read=%d",byte_read);
    if(byte_read!=COLOUR_MAP_NONE_ID){//} && byte_read<COLOUR_MAP_PALETTE_OPTIONS_END_ID){
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


int8_t mPalette::GetNearestColourMapIDFromColour(HsbColor hsb){

}

const char* mPalette::GetColourMapNameNearestbyColour(HsbColor c, char* buffer, uint8_t buflen){

  snprintf_P(buffer, sizeof(buffer), WARNING_NOTHANDLED_CTR);
  return buffer;

  // uint32_t colour32bit_tofind = WebColorFromColourType(RgbColor(c));

  // uint64_t colour32bit_tosearch[PRESET_COLOUR_MAP_INDEXES_MAX];
  
  // for(int i=0; i<PRESET_COLOUR_MAP_INDEXES_MAX; i++) {
  //   colour32bit_tosearch[i] = (WebColorFromColourType(RgbColor(c))-colour32bit_tofind);
  // }

  // uint64_t temp = colour32bit_tosearch[0];
  // uint16_t index = 0;
  // for(int i=0; i<PRESET_COLOUR_MAP_INDEXES_MAX; i++) {
  //     if(temp>colour32bit_tosearch[i]) {
  //        temp=colour32bit_tosearch[i];
  //        index = i;
  //     }
  // }

  // return GetColourMapNamebyID(index);

}


int8_t mPalette::GetColourMapIDbyName(const char* c){
  if(c=='\0') return -1;
  // if(strstr(c,D_COLOUR_MAP_RED_CTR))          return COLOUR_MAP_RED_ID;
  // if(strstr(c,D_COLOUR_MAP_RED_PASTEL80_CTR))          return COLOUR_MAP_RED_PASTEL80_ID;
  // if(strstr(c,D_COLOUR_MAP_RED_PASTEL60_CTR))          return COLOUR_MAP_RED_PASTEL60_ID;
  // if(strstr(c,D_COLOUR_MAP_ORANGE_CTR))       return COLOUR_MAP_ORANGE_ID;
  // if(strstr(c,D_COLOUR_MAP_LIGHTORANGE_CTR))  return COLOUR_MAP_LIGHTORANGE_ID;
  // if(strstr(c,D_COLOUR_MAP_YELLOW_CTR))       return COLOUR_MAP_YELLOW_ID;
  // if(strstr(c,D_COLOUR_MAP_LIMEGREEN_CTR))    return COLOUR_MAP_LIMEGREEN_ID;
  // if(strstr(c,D_COLOUR_MAP_GREEN_CTR))        return COLOUR_MAP_GREEN_ID;
  // if(strstr(c,D_COLOUR_MAP_CYAN_CTR))         return COLOUR_MAP_CYAN_ID;
  // if(strstr(c,D_COLOUR_MAP_BLUE_CTR))         return COLOUR_MAP_BLUE_ID;
  // if(strstr(c,D_COLOUR_MAP_BLUEPURPLE_CTR))   return COLOUR_MAP_BLUEPURPLE_ID;
  // if(strstr(c,D_COLOUR_MAP_PURPLE_CTR))       return COLOUR_MAP_PURPLE_ID;
  // if(strstr(c,D_COLOUR_MAP_PINK_CTR))         return COLOUR_MAP_PINK_ID;
  // if(strstr(c,D_COLOUR_MAP_HOTPINK_CTR))      return COLOUR_MAP_HOTPINK_ID;
  // if(strstr(c,D_COLOUR_MAP_NONE_CTR))      return COLOUR_MAP_NONE_ID;
  // if(strstr(c,D_COLOUR_MAP_PALETTE_OPTIONS_END_CTR))      return COLOUR_MAP_PALETTE_OPTIONS_END_ID;

  
  return -1;
}
const char* mPalette::GetColourMapNamebyID(uint8_t id, char* buffer, uint8_t buflen){

  // use switch method I generate hsb to also generate the name
  snprintf_P(buffer, sizeof(buffer), WARNING_NOTHANDLED_CTR);
  return buffer;
  
}



#endif
