#include "mInterfaceLight.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE // interface is the gateway



int8_t mInterfaceLight::GetNearestColourMapIDFromColour(HsbColor hsb){

}

const char* mInterfaceLight::GetColourMapNameNearestbyColour(HsbColor c, char* buffer, uint8_t buflen){

// TOO MUCH MEMROY!!!
  return WARNING_NOTHANDLED_CTR;
// return "memory issue";

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




int8_t mInterfaceLight::GetColourMapIDbyName(const char* c){
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
const char* mInterfaceLight::GetColourMapNamebyID(uint8_t id, char* buffer, uint8_t buflen){

  return 0;
  // turn this into a dlist?

  switch(id){
    // Red
    case COLOUR_MAP_RED_ID:                    return D_COLOUR_MAP_RED_CTR;
    case COLOUR_MAP_RED_PASTEL95_ID:           return D_COLOUR_MAP_RED_PASTEL95_CTR;
    case COLOUR_MAP_RED_PASTEL90_ID:           return D_COLOUR_MAP_RED_PASTEL90_CTR;
    case COLOUR_MAP_RED_PASTEL80_ID:           return D_COLOUR_MAP_RED_PASTEL80_CTR;
    case COLOUR_MAP_RED_PASTEL70_ID:           return D_COLOUR_MAP_RED_PASTEL70_CTR;
    case COLOUR_MAP_RED_PASTEL60_ID:           return D_COLOUR_MAP_RED_PASTEL60_CTR;
    case COLOUR_MAP_RED_PASTEL50_ID:           return D_COLOUR_MAP_RED_PASTEL50_CTR;
    case COLOUR_MAP_RED_PASTEL40_ID:           return D_COLOUR_MAP_RED_PASTEL40_CTR;
    case COLOUR_MAP_RED_PASTEL20_ID:           return D_COLOUR_MAP_RED_PASTEL20_CTR;
    // Orange
    case COLOUR_MAP_ORANGE_ID:        return D_COLOUR_MAP_ORANGE_CTR;
    case COLOUR_MAP_ORANGE_PASTEL95_ID:        return D_COLOUR_MAP_ORANGE_PASTEL95_CTR;
    case COLOUR_MAP_ORANGE_PASTEL90_ID:        return D_COLOUR_MAP_ORANGE_PASTEL90_CTR;
    case COLOUR_MAP_ORANGE_PASTEL80_ID:        return D_COLOUR_MAP_ORANGE_PASTEL80_CTR;
    case COLOUR_MAP_ORANGE_PASTEL70_ID:        return D_COLOUR_MAP_ORANGE_PASTEL70_CTR;
    case COLOUR_MAP_ORANGE_PASTEL60_ID:        return D_COLOUR_MAP_ORANGE_PASTEL60_CTR;
    case COLOUR_MAP_ORANGE_PASTEL50_ID:        return D_COLOUR_MAP_ORANGE_PASTEL50_CTR;
    case COLOUR_MAP_ORANGE_PASTEL40_ID:        return D_COLOUR_MAP_ORANGE_PASTEL40_CTR;
    // Light Orange
    case COLOUR_MAP_LIGHTORANGE_ID:   return D_COLOUR_MAP_LIGHTORANGE_CTR;
    case COLOUR_MAP_LIGHTORANGE_PASTEL95_ID:   return D_COLOUR_MAP_LIGHTORANGE_PASTEL95_CTR;
    case COLOUR_MAP_LIGHTORANGE_PASTEL90_ID:   return D_COLOUR_MAP_LIGHTORANGE_PASTEL90_CTR;
    case COLOUR_MAP_LIGHTORANGE_PASTEL80_ID:   return D_COLOUR_MAP_LIGHTORANGE_PASTEL80_CTR;
    case COLOUR_MAP_LIGHTORANGE_PASTEL70_ID:   return D_COLOUR_MAP_LIGHTORANGE_PASTEL70_CTR;
    case COLOUR_MAP_LIGHTORANGE_PASTEL60_ID:   return D_COLOUR_MAP_LIGHTORANGE_PASTEL60_CTR;
    case COLOUR_MAP_LIGHTORANGE_PASTEL50_ID:   return D_COLOUR_MAP_LIGHTORANGE_PASTEL50_CTR;
    case COLOUR_MAP_LIGHTORANGE_PASTEL40_ID:   return D_COLOUR_MAP_LIGHTORANGE_PASTEL40_CTR;
    // White
    case COLOUR_MAP_WHITE_ID: return D_COLOUR_MAP_WHITE_CTR;
    case COLOUR_MAP_WHITE_PASTEL95_ID: return D_COLOUR_MAP_WHITE_PASTEL95_CTR;
    case COLOUR_MAP_WHITE_PASTEL90_ID: return D_COLOUR_MAP_WHITE_PASTEL90_CTR;
    case COLOUR_MAP_WHITE_PASTEL80_ID: return D_COLOUR_MAP_WHITE_PASTEL80_CTR;
    case COLOUR_MAP_WHITE_PASTEL70_ID: return D_COLOUR_MAP_WHITE_PASTEL70_CTR;
    case COLOUR_MAP_WHITE_PASTEL60_ID: return D_COLOUR_MAP_WHITE_PASTEL60_CTR;
    case COLOUR_MAP_WHITE_PASTEL50_ID: return D_COLOUR_MAP_WHITE_PASTEL50_CTR;
    case COLOUR_MAP_WHITE_PASTEL40_ID: return D_COLOUR_MAP_WHITE_PASTEL40_CTR;
    case COLOUR_MAP_WHITE_PASTEL20_ID: return D_COLOUR_MAP_WHITE_PASTEL20_CTR;
    // Yellow
    case COLOUR_MAP_YELLOW_ID:        return D_COLOUR_MAP_YELLOW_CTR;
    case COLOUR_MAP_YELLOW_PASTEL95_ID:        return D_COLOUR_MAP_YELLOW_PASTEL95_CTR;
    case COLOUR_MAP_YELLOW_PASTEL90_ID:        return D_COLOUR_MAP_YELLOW_PASTEL90_CTR;
    case COLOUR_MAP_YELLOW_PASTEL80_ID:        return D_COLOUR_MAP_YELLOW_PASTEL80_CTR;
    case COLOUR_MAP_YELLOW_PASTEL70_ID:        return D_COLOUR_MAP_YELLOW_PASTEL70_CTR;
    case COLOUR_MAP_YELLOW_PASTEL60_ID:        return D_COLOUR_MAP_YELLOW_PASTEL60_CTR;
    case COLOUR_MAP_YELLOW_PASTEL50_ID:        return D_COLOUR_MAP_YELLOW_PASTEL50_CTR;
    case COLOUR_MAP_YELLOW_PASTEL40_ID:        return D_COLOUR_MAP_YELLOW_PASTEL40_CTR;
    // Lime Green
    case COLOUR_MAP_LIMEGREEN_ID:     return D_COLOUR_MAP_LIMEGREEN_CTR;
    case COLOUR_MAP_LIMEGREEN_PASTEL95_ID:     return D_COLOUR_MAP_LIMEGREEN_PASTEL95_CTR;
    case COLOUR_MAP_LIMEGREEN_PASTEL90_ID:     return D_COLOUR_MAP_LIMEGREEN_PASTEL90_CTR;
    case COLOUR_MAP_LIMEGREEN_PASTEL80_ID:     return D_COLOUR_MAP_LIMEGREEN_PASTEL80_CTR;
    case COLOUR_MAP_LIMEGREEN_PASTEL70_ID:     return D_COLOUR_MAP_LIMEGREEN_PASTEL70_CTR;
    case COLOUR_MAP_LIMEGREEN_PASTEL60_ID:     return D_COLOUR_MAP_LIMEGREEN_PASTEL60_CTR;
    case COLOUR_MAP_LIMEGREEN_PASTEL50_ID:     return D_COLOUR_MAP_LIMEGREEN_PASTEL50_CTR;
    case COLOUR_MAP_LIMEGREEN_PASTEL40_ID:     return D_COLOUR_MAP_LIMEGREEN_PASTEL40_CTR;
    // Green
    case COLOUR_MAP_GREEN_ID:         return D_COLOUR_MAP_GREEN_CTR;
    case COLOUR_MAP_GREEN_PASTEL95_ID:         return D_COLOUR_MAP_GREEN_PASTEL95_CTR;
    case COLOUR_MAP_GREEN_PASTEL90_ID:         return D_COLOUR_MAP_GREEN_PASTEL90_CTR;
    case COLOUR_MAP_GREEN_PASTEL80_ID:         return D_COLOUR_MAP_GREEN_PASTEL80_CTR;
    case COLOUR_MAP_GREEN_PASTEL70_ID:         return D_COLOUR_MAP_GREEN_PASTEL70_CTR;
    case COLOUR_MAP_GREEN_PASTEL60_ID:         return D_COLOUR_MAP_GREEN_PASTEL60_CTR;
    case COLOUR_MAP_GREEN_PASTEL50_ID:         return D_COLOUR_MAP_GREEN_PASTEL50_CTR;
    case COLOUR_MAP_GREEN_PASTEL40_ID:         return D_COLOUR_MAP_GREEN_PASTEL40_CTR;
    // Cyan
    case COLOUR_MAP_CYAN_ID:          return D_COLOUR_MAP_CYAN_CTR;
    case COLOUR_MAP_CYAN_PASTEL95_ID:          return D_COLOUR_MAP_CYAN_PASTEL95_CTR;
    case COLOUR_MAP_CYAN_PASTEL90_ID:          return D_COLOUR_MAP_CYAN_PASTEL90_CTR;
    case COLOUR_MAP_CYAN_PASTEL80_ID:          return D_COLOUR_MAP_CYAN_PASTEL80_CTR;
    case COLOUR_MAP_CYAN_PASTEL70_ID:          return D_COLOUR_MAP_CYAN_PASTEL70_CTR;
    case COLOUR_MAP_CYAN_PASTEL60_ID:          return D_COLOUR_MAP_CYAN_PASTEL60_CTR;
    case COLOUR_MAP_CYAN_PASTEL50_ID:          return D_COLOUR_MAP_CYAN_PASTEL50_CTR;
    case COLOUR_MAP_CYAN_PASTEL40_ID:          return D_COLOUR_MAP_CYAN_PASTEL40_CTR;
    // Blue
    case COLOUR_MAP_BLUE_ID:          return D_COLOUR_MAP_BLUE_CTR;
    case COLOUR_MAP_BLUE_PASTEL95_ID:          return D_COLOUR_MAP_BLUE_PASTEL95_CTR;
    case COLOUR_MAP_BLUE_PASTEL90_ID:          return D_COLOUR_MAP_BLUE_PASTEL90_CTR;
    case COLOUR_MAP_BLUE_PASTEL80_ID:          return D_COLOUR_MAP_BLUE_PASTEL80_CTR;
    case COLOUR_MAP_BLUE_PASTEL70_ID:          return D_COLOUR_MAP_BLUE_PASTEL70_CTR;
    case COLOUR_MAP_BLUE_PASTEL60_ID:          return D_COLOUR_MAP_BLUE_PASTEL60_CTR;
    case COLOUR_MAP_BLUE_PASTEL50_ID:          return D_COLOUR_MAP_BLUE_PASTEL50_CTR;
    case COLOUR_MAP_BLUE_PASTEL40_ID:          return D_COLOUR_MAP_BLUE_PASTEL40_CTR;
    // Blue Purple
    case COLOUR_MAP_BLUEPURPLE_ID:    return D_COLOUR_MAP_BLUEPURPLE_CTR;
    case COLOUR_MAP_BLUEPURPLE_PASTEL95_ID:    return D_COLOUR_MAP_BLUEPURPLE_PASTEL95_CTR;
    case COLOUR_MAP_BLUEPURPLE_PASTEL90_ID:    return D_COLOUR_MAP_BLUEPURPLE_PASTEL90_CTR;
    case COLOUR_MAP_BLUEPURPLE_PASTEL80_ID:    return D_COLOUR_MAP_BLUEPURPLE_PASTEL80_CTR;
    case COLOUR_MAP_BLUEPURPLE_PASTEL70_ID:    return D_COLOUR_MAP_BLUEPURPLE_PASTEL70_CTR;
    case COLOUR_MAP_BLUEPURPLE_PASTEL60_ID:    return D_COLOUR_MAP_BLUEPURPLE_PASTEL60_CTR;
    case COLOUR_MAP_BLUEPURPLE_PASTEL50_ID:    return D_COLOUR_MAP_BLUEPURPLE_PASTEL50_CTR;
    case COLOUR_MAP_BLUEPURPLE_PASTEL40_ID:    return D_COLOUR_MAP_BLUEPURPLE_PASTEL40_CTR;
    // Purple
    case COLOUR_MAP_PURPLE_ID:        return D_COLOUR_MAP_PURPLE_CTR;
    case COLOUR_MAP_PURPLE_PASTEL95_ID:        return D_COLOUR_MAP_PURPLE_PASTEL95_CTR;
    case COLOUR_MAP_PURPLE_PASTEL90_ID:        return D_COLOUR_MAP_PURPLE_PASTEL90_CTR;
    case COLOUR_MAP_PURPLE_PASTEL80_ID:        return D_COLOUR_MAP_PURPLE_PASTEL80_CTR;
    case COLOUR_MAP_PURPLE_PASTEL70_ID:        return D_COLOUR_MAP_PURPLE_PASTEL70_CTR;
    case COLOUR_MAP_PURPLE_PASTEL60_ID:        return D_COLOUR_MAP_PURPLE_PASTEL60_CTR;
    case COLOUR_MAP_PURPLE_PASTEL50_ID:        return D_COLOUR_MAP_PURPLE_PASTEL50_CTR;
    case COLOUR_MAP_PURPLE_PASTEL40_ID:        return D_COLOUR_MAP_PURPLE_PASTEL40_CTR;
    // Pink
    case COLOUR_MAP_PINK_ID:          return D_COLOUR_MAP_PINK_CTR;
    case COLOUR_MAP_PINK_PASTEL95_ID:          return D_COLOUR_MAP_PINK_PASTEL95_CTR;
    case COLOUR_MAP_PINK_PASTEL90_ID:          return D_COLOUR_MAP_PINK_PASTEL90_CTR;
    case COLOUR_MAP_PINK_PASTEL80_ID:          return D_COLOUR_MAP_PINK_PASTEL80_CTR;
    case COLOUR_MAP_PINK_PASTEL70_ID:          return D_COLOUR_MAP_PINK_PASTEL70_CTR;
    case COLOUR_MAP_PINK_PASTEL60_ID:          return D_COLOUR_MAP_PINK_PASTEL60_CTR;
    case COLOUR_MAP_PINK_PASTEL50_ID:          return D_COLOUR_MAP_PINK_PASTEL50_CTR;
    case COLOUR_MAP_PINK_PASTEL40_ID:          return D_COLOUR_MAP_PINK_PASTEL40_CTR;
    // Hotpink
    case COLOUR_MAP_HOTPINK_ID:       return D_COLOUR_MAP_HOTPINK_CTR;
    case COLOUR_MAP_HOTPINK_PASTEL95_ID:       return D_COLOUR_MAP_HOTPINK_PASTEL95_CTR;
    case COLOUR_MAP_HOTPINK_PASTEL90_ID:       return D_COLOUR_MAP_HOTPINK_PASTEL90_CTR;
    case COLOUR_MAP_HOTPINK_PASTEL80_ID:       return D_COLOUR_MAP_HOTPINK_PASTEL80_CTR;
    case COLOUR_MAP_HOTPINK_PASTEL70_ID:       return D_COLOUR_MAP_HOTPINK_PASTEL70_CTR;
    case COLOUR_MAP_HOTPINK_PASTEL60_ID:       return D_COLOUR_MAP_HOTPINK_PASTEL60_CTR;
    case COLOUR_MAP_HOTPINK_PASTEL50_ID:       return D_COLOUR_MAP_HOTPINK_PASTEL50_CTR;
    case COLOUR_MAP_HOTPINK_PASTEL40_ID:       return D_COLOUR_MAP_HOTPINK_PASTEL40_CTR;
    // None ie not set/used
    case COLOUR_MAP_NONE_ID:       return D_COLOUR_MAP_NONE_CTR;
    case COLOUR_MAP_PALETTE_OPTIONS_END_ID: return D_COLOUR_MAP_PALETTE_OPTIONS_END_CTR;
  }
  return WARNING_NOTHANDLED_CTR;
  // return "Unset";
}


void mInterfaceLight::init_PresetColourPalettes(){
  
  
  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog_P(LOG_LEVEL_TEST,PSTR("init_PresetColourPalettes"));
  #endif
  

  for(uint8_t id=0;id<20;id++){
    init_PresetColourPalettes_UserFill(id);
    setdefault_PresetColourPalettes_UserFill(id);
    
  // // for(uint8_t id=0;id<400;id++){
  // AddLog_Array(LOG_LEVEL_TEST, "test3", 
  //   &pCONT_set->Settings.animation_settings.palette_user_colour_map_ids[0],
  //   (uint8_t)400);//palettelist.ptr->colour_map_size);
  // // }
  }

// AddLog_Array(LOG_LEVEL_TEST,PSTR("test=%d"),Settings.animation_settings.palette_user_colour_map_ids[palettelist.ptr->id][0])

  // memcpy_P(pCONT_set->Settings.animation_settings.palette_user_colour_map_ids,DEFAULT_COLOUR_PALETTE_USER_01_IDS,sizeof(DEFAULT_COLOUR_PALETTE_USER_01_IDS));

  // memcpy_P(palettelist.ptr->colour_map_id,DEFAULT_COLOUR_PALETTE_USER_01_IDS,sizeof(DEFAULT_COLOUR_PALETTE_USER_01_IDS));

  // init defaults
  init_ColourPalettes_Autumn_Red();
  init_ColourPalettes_Autumn();
  init_ColourPalettes_Winter(); 
  init_ColourPalettes_Ocean_01();     //add way to take gradient patterns and use thier colours only (not gradient indexes)
  init_ColourPalettes_Rainbow();
  init_ColourPalettes_RainbowInverted();
  init_ColourPalettes_Holloween_OP();
  init_ColourPalettes_Holloween_OGP();
  init_ColourPalettes_Shelf_Hearts();
  init_ColourPalettes_Pastel();
  init_ColourPalettes_Gradient_01();
  init_ColourPalettes_Gradient_02();
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
  init_ColourPalettes_Custom_User_01();

}



void mInterfaceLight::init_ColourPalettes_Holloween_OP(){
  palettelist.ptr     = &palettelist.holloween_op;  
  palettelist.ptr->id = PALETTELIST_STATIC_HOLLOWEEN_OP_ID;
  palettelist.ptr->colour_map_size    = sizeof(colour_map_holloween_op_id);
  palettelist.ptr->colour_map_id      = (uint8_t*)colour_map_holloween_op_id;
  palettelist.ptr->friendly_name_ctr  = (char*)PM_PALETTE_HOLLOWEEN_OP_NAME_CTR;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
}

void mInterfaceLight::init_ColourPalettes_Holloween_OGP(){
  palettelist.ptr     = &palettelist.holloween_ogp;  
  palettelist.ptr->id = PALETTELIST_STATIC_HOLLOWEEN_OGP_ID;
  palettelist.ptr->colour_map_size    = sizeof(colour_map_holloween_ogp_id);
  palettelist.ptr->colour_map_id      = (uint8_t*)colour_map_holloween_ogp_id;
  palettelist.ptr->friendly_name_ctr  = (char*)PM_PALETTE_HOLLOWEEN_OGP_NAME_CTR;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
}
void mInterfaceLight::init_ColourPalettes_Pastel(){
  palettelist.ptr     = &palettelist.pastel;  
  palettelist.ptr->id = PALETTELIST_STATIC_PASTEL_ID;
  palettelist.ptr->colour_map_size    = sizeof(COLOUR_PALETTE_PASTEL_MAP_IDS);
  palettelist.ptr->colour_map_id      = (uint8_t*)COLOUR_PALETTE_PASTEL_MAP_IDS;
  palettelist.ptr->friendly_name_ctr  = (char*)PM_PALETTE_PASTEL_NAME_CTR;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
}

void mInterfaceLight::init_ColourPalettes_Shelf_Hearts(){
  palettelist.ptr     = &palettelist.shelf_hearts;  
  palettelist.ptr->id = PALETTELIST_STATIC_SHELF_HEARTS_ID;
  palettelist.ptr->colour_map_size = sizeof(colour_map_shelf_hearts_id);
  palettelist.ptr->colour_map_id = (uint8_t*)colour_map_shelf_hearts_id;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_SHELF_HEARTS_NAME_CTR;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_ID;
  // Need a solution to remember these ie if shelf01, hearts are pink, else yellow
  
  #ifdef DONT_USE_OLD_PRESETS
  preset_colour_map[COLOUR_MAP_SPECIAL_ONEINMANY_MANY_ID] = preset_colour_map[COLOUR_MAP_YELLOW_ID]; 
  preset_colour_map[COLOUR_MAP_SPECIAL_ONEINMANY_ONE_ID]  = preset_colour_map[COLOUR_MAP_HOTPINK_ID]; 
  #endif
}

void mInterfaceLight::init_ColourPalettes_Gradient_01(){
  palettelist.ptr     = &palettelist.gradient_01;  
  palettelist.ptr->id = PALETTELIST_STATIC_GRADIENT_01_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_GRADIENT_01_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_GRADIENT_01_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_GRADIENT_01_NAME_CTR;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_100;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID;
  
  #ifdef ENABLE_LOG_LEVEL_INFO
  // CAUSED DEBUG MODE CRASH
  //AddLog_Array_P(LOG_LEVEL_TEST, "Grad01", palettelist.ptr->colour_map_id,palettelist.ptr->colour_map_size);
  #endif
}
void mInterfaceLight::init_ColourPalettes_Gradient_02(){
  palettelist.ptr     = &palettelist.gradient_02;  
  palettelist.ptr->id = PALETTELIST_STATIC_GRADIENT_02_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_GRADIENT_02_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_GRADIENT_02_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_GRADIENT_02_NAME_CTR;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_GRADIENT_ID;
  
  #ifdef ENABLE_LOG_LEVEL_INFO
  //AddLog_Array_P(LOG_LEVEL_TEST, "Grad02", palettelist.ptr->colour_map_id,palettelist.ptr->colour_map_size);
  #endif
}
void mInterfaceLight::init_ColourPalettes_Berry_Green(){
  palettelist.ptr     = &palettelist.berry_green;  
  palettelist.ptr->id = PALETTELIST_STATIC_BERRY_GREEN_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_BERRY_GREEN_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_BERRY_GREEN_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_BERRY_GREEN_NAME_CTR;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
  
  #ifdef ENABLE_LOG_LEVEL_INFO
  //AddLog_Array_P(LOG_LEVEL_TEST, "Grad02", palettelist.ptr->colour_map_id,palettelist.ptr->colour_map_size);
  #endif
}
void mInterfaceLight::init_ColourPalettes_Christmas_01(){
  palettelist.ptr     = &palettelist.christmas_01;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_01_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_01_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_01_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_01_NAME_CTR;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
  
  #ifdef ENABLE_LOG_LEVEL_INFO
  //AddLog_Array_P(LOG_LEVEL_TEST, "Grad02", palettelist.ptr->colour_map_id,palettelist.ptr->colour_map_size);
  #endif
}
void mInterfaceLight::init_ColourPalettes_Christmas_02(){
  palettelist.ptr     = &palettelist.christmas_02;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_02_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_02_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_02_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_02_NAME_CTR;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
  
  #ifdef ENABLE_LOG_LEVEL_INFO
  //AddLog_Array_P(LOG_LEVEL_TEST, "Grad02", palettelist.ptr->colour_map_id,palettelist.ptr->colour_map_size);
  #endif
}
void mInterfaceLight::init_ColourPalettes_Christmas_03(){
  palettelist.ptr     = &palettelist.christmas_03;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_03_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_03_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_03_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_03_NAME_CTR;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
  
  #ifdef ENABLE_LOG_LEVEL_INFO
  //AddLog_Array_P(LOG_LEVEL_TEST, "Grad02", palettelist.ptr->colour_map_id,palettelist.ptr->colour_map_size);
  #endif
}
void mInterfaceLight::init_ColourPalettes_Christmas_04(){
  palettelist.ptr     = &palettelist.christmas_04;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_04_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_04_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_04_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_04_NAME_CTR;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
  
  #ifdef ENABLE_LOG_LEVEL_INFO
  //AddLog_Array_P(LOG_LEVEL_TEST, "Grad02", palettelist.ptr->colour_map_id,palettelist.ptr->colour_map_size);
  #endif
}
void mInterfaceLight::init_ColourPalettes_Christmas_05(){
  palettelist.ptr     = &palettelist.christmas_05;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_05_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_05_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_05_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_05_NAME_CTR;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
  
  #ifdef ENABLE_LOG_LEVEL_INFO
  //AddLog_Array_P(LOG_LEVEL_TEST, "Grad02", palettelist.ptr->colour_map_id,palettelist.ptr->colour_map_size);
  #endif
}
void mInterfaceLight::init_ColourPalettes_Christmas_06(){
  palettelist.ptr     = &palettelist.christmas_06;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_06_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_06_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_06_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_06_NAME_CTR;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
  
  #ifdef ENABLE_LOG_LEVEL_INFO
  //AddLog_Array_P(LOG_LEVEL_TEST, "Grad02", palettelist.ptr->colour_map_id,palettelist.ptr->colour_map_size);
  #endif
}
void mInterfaceLight::init_ColourPalettes_Christmas_07(){
  palettelist.ptr     = &palettelist.christmas_07;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_07_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_07_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_07_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_07_NAME_CTR;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
  
  #ifdef ENABLE_LOG_LEVEL_INFO
  //AddLog_Array_P(LOG_LEVEL_TEST, "Grad02", palettelist.ptr->colour_map_id,palettelist.ptr->colour_map_size);
  #endif
}
void mInterfaceLight::init_ColourPalettes_Christmas_08(){
  palettelist.ptr     = &palettelist.christmas_08;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_08_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_08_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_08_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_08_NAME_CTR;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
  
  #ifdef ENABLE_LOG_LEVEL_INFO
  //AddLog_Array_P(LOG_LEVEL_TEST, "Grad02", palettelist.ptr->colour_map_id,palettelist.ptr->colour_map_size);
  #endif
}
void mInterfaceLight::init_ColourPalettes_Christmas_09(){
  palettelist.ptr     = &palettelist.christmas_09;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_09_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_09_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_09_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_09_NAME_CTR;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
  
  #ifdef ENABLE_LOG_LEVEL_INFO
  //AddLog_Array_P(LOG_LEVEL_TEST, "Grad02", palettelist.ptr->colour_map_id,palettelist.ptr->colour_map_size);
  #endif
}
void mInterfaceLight::init_ColourPalettes_Christmas_10(){
  palettelist.ptr     = &palettelist.christmas_10;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_10_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_10_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_10_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_10_NAME_CTR;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
  
  #ifdef ENABLE_LOG_LEVEL_INFO
  //AddLog_Array_P(LOG_LEVEL_TEST, "Grad02", palettelist.ptr->colour_map_id,palettelist.ptr->colour_map_size);
  #endif
}
void mInterfaceLight::init_ColourPalettes_Christmas_11(){
  palettelist.ptr     = &palettelist.christmas_11;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_11_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_11_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_11_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_11_NAME_CTR;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
  
  #ifdef ENABLE_LOG_LEVEL_INFO
  //AddLog_Array_P(LOG_LEVEL_TEST, "Grad02", palettelist.ptr->colour_map_id,palettelist.ptr->colour_map_size);
  #endif
}

void mInterfaceLight::init_ColourPalettes_Christmas_12(){
  palettelist.ptr     = &palettelist.christmas_12;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_12_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_12_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_12_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_12_NAME_CTR;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}

void mInterfaceLight::init_ColourPalettes_Christmas_13(){
  palettelist.ptr     = &palettelist.christmas_13;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_13_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_13_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_13_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_13_NAME_CTR;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}

void mInterfaceLight::init_ColourPalettes_Christmas_14(){
  palettelist.ptr     = &palettelist.christmas_14;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_14_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_14_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_14_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_14_NAME_CTR;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}

void mInterfaceLight::init_ColourPalettes_Christmas_15(){
  palettelist.ptr     = &palettelist.christmas_15;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_15_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_15_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_15_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_15_NAME_CTR;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}

void mInterfaceLight::init_ColourPalettes_Christmas_16(){
  palettelist.ptr     = &palettelist.christmas_16;  
  palettelist.ptr->id = PALETTELIST_STATIC_CHRISTMAS_16_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CHRISTMAS_16_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CHRISTMAS_16_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CHRISTMAS_16_NAME_CTR;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}


void mInterfaceLight::init_ColourPalettes_Sunrise_01(){
  palettelist.ptr     = &palettelist.sunrise_01;  
  palettelist.ptr->id = PALETTELIST_STATIC_SUNRISE_01_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_SUNRISE_01_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_SUNRISE_01_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_SUNRISE_01_NAME_CTR;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}



void mInterfaceLight::init_ColourPalettes_Custom_User_01(){
  palettelist.ptr     = &palettelist.custom_user_01;  
  palettelist.ptr->id = PALETTELIST_STATIC_CUSTOM_USER_01_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_CUSTOM_USER_01_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_CUSTOM_USER_01_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_CUSTOM_USER_01_NAME_CTR;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_NONE;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID;
}


void mInterfaceLight::init_ColourPalettes_Autumn_Red(){
  palettelist.ptr     = &palettelist.autumn_red;
  palettelist.ptr->id = PALETTELIST_STATIC_AUTUMN_RED_ID;
  palettelist.ptr->colour_map_size = sizeof(colour_map_autumnred_id);
  palettelist.ptr->colour_map_id = (uint8_t*)colour_map_autumnred_id;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_AUTUMN_RED_NAME_CTR;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = true;
}

void mInterfaceLight::init_ColourPalettes_Autumn(){
  palettelist.ptr     = &palettelist.autumn;
  palettelist.ptr->id = PALETTELIST_STATIC_AUTUMN_ID;
  palettelist.ptr->colour_map_size = sizeof(colour_map_autumn_id);
  palettelist.ptr->colour_map_id = (uint8_t*)colour_map_autumn_id;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_AUTUMN_NAME_CTR;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = true;
}

void mInterfaceLight::init_ColourPalettes_Winter(){
  palettelist.ptr     = &palettelist.winter;
  palettelist.ptr->id = PALETTELIST_STATIC_WINTER_ID;
  palettelist.ptr->colour_map_size = sizeof(COLOUR_PALETTE_WINTER_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)COLOUR_PALETTE_WINTER_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_WINTER_NAME_CTR;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = true;
}

void mInterfaceLight::init_ColourPalettes_Ocean_01(){
  palettelist.ptr     = &palettelist.ocean_01;
  palettelist.ptr->id = PALETTELIST_STATIC_OCEAN_01_ID;
  palettelist.ptr->colour_map_size = sizeof(colour_map_ocean_01_id);
  palettelist.ptr->colour_map_id = (uint8_t*)colour_map_ocean_01_id;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_OCEAN_01_NAME_CTR;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = true;
  // palettelist.ptr->flags.fOverride_Saturation_To_Random = true;
}

void mInterfaceLight::init_ColourPalettes_Rainbow(){
  palettelist.ptr     = &palettelist.rainbow;
  palettelist.ptr->id = PALETTELIST_STATIC_RAINBOW_ID;
  palettelist.ptr->colour_map_size = sizeof(colour_map_rainbow_id);
  palettelist.ptr->colour_map_id = (uint8_t*)colour_map_rainbow_id;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_RAINBOW_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID;
}

void mInterfaceLight::init_ColourPalettes_RainbowInverted(){
  palettelist.ptr     = &palettelist.rainbow_inverted;
  palettelist.ptr->id = PALETTELIST_STATIC_RAINBOW_INVERTED_ID;
  palettelist.ptr->colour_map_size = sizeof(colour_map_rainbow_inverted_id);
  palettelist.ptr->colour_map_id = (uint8_t*)colour_map_rainbow_inverted_id;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_RAINBOW_INVERTED_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID;
}



/*******************************************************************************************************************
********************************************************************************************************************
************ START OF PALETTE DEFINITIONS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/

HsbColor mInterfaceLight::GetHsbColour(uint8_t id){

  HsbColor colour = HsbColor(1,1,1);


  // Get Saturation
  uint8_t sat_val = id%12;
  // AddLog_P(LOG_LEVEL_INFO,PSTR("sat_val = %d   %d"), sat_val, id);


  switch(sat_val){
    default:
    case 0: colour.S = 1.0f; break;
    case 1: colour.S = 0.95f; break;
    case 2: colour.S = 0.9f; break;
    case 3: colour.S = 0.8f; break;
    case 4: colour.S = 0.7f; break;
    case 5: colour.S = 0.6f; break;
    case 6: colour.S = 0.5f; break;
    case 7: colour.S = 0.4f; break;
    case 8: colour.S = 0.3f; break;
    case 9: colour.S = 0.2f; break;
    case 10: colour.S = 0.1f; break;
    case 11: colour.S = 0.0f; break;
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

  // AddLog_P(LOG_LEVEL_INFO,PSTR("id mod 12 = %d   %d"), id%12, id);
  // colour.S = (12-sat_val)

  return colour;

}


int8_t mInterfaceLight::GetPaletteIDbyName(const char* c){
  if(c=='\0'){
    return -1;
  }

  PALETTELIST::PALETTE *ptr = nullptr;
  char buffer[50];
  int8_t index_found = -1;

  // Check for matches with variables names  
  if ((index_found = pCONT_sup->GetDListIDbyNameCtr(buffer, sizeof(buffer), c, pCONT_set->Settings.animation_settings.palette_user_variable_name_list_ctr)) >= 0) {
    // index_found = STATE_NUMBER_OFF_ID;    
      AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("index_found = %d"),index_found);    
      return index_found;
  }

  // Check against stored progmem static names
  for(uint8_t ii=0;ii<PALETTELIST_STATIC_LENGTH_ID;ii++){
    ptr = GetPalettePointerByID(ii);
    // AddLog_P(LOG_LEVEL_ERROR, PSTR("ptr->id %d"),ptr->id);
    // AddLog_P(LOG_LEVEL_ERROR, PSTR("ptr->friendly_name_ctr %s"),ptr->friendly_name_ctr);

    if(ptr->friendly_name_ctr == nullptr){ 
      #ifdef ENABLE_LOG_LEVEL_COMMANDS
      AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("ptr->friendly_name_ctr == nullptr %d %s"),ii,c);     //skipping names not set, including variables names which dont use pointer to name (unless I point to its place later, and include its name length?) Store variable name in dlist 
      // move variable name to join standard devicename and just include as indexed? ie 0-20 is their names?
      #endif // ENABLE_LOG_LEVEL_COMMANDS
    }
    if(ptr->friendly_name_ctr != nullptr){ 
      if(ii>UO_PALETTELIST_VARIABLE_USER_LENGTH_ID){
        if(strcmp_P(c,ptr->friendly_name_ctr)==0){
          AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("strcmp(c,ptr->friendly_name_ctr)=>%d"),ii);
          return ii;
        }
      }
    }
  }

  // Check for default user names 
  char name_ctr[20];
  for(uint8_t ii=0;ii<20;ii++){
    memset(name_ctr,0,sizeof(name_ctr));
    sprintf_P(name_ctr,PSTR("%s %02d\0"),D_PALETTE_USER_NAME_CTR,ii);
    // Default names
    if(strcmp(c,name_ctr)==0){
      return PALETTELIST_VARIABLE_USER_01_ID+ii;
    }
  }

  // Finally, check if palette name was simply its index as string
  uint8_t found_index = (!strlen(c)) ? 0 : atoi(c);
  if(WithinLimits(found_index, (uint8_t)0, (uint8_t)PALETTELIST_STATIC_LENGTH_ID)){
    return found_index;
  }

  return -1;

}


mInterfaceLight::PALETTELIST::PALETTE* mInterfaceLight::GetPalettePointerByID(uint8_t id){
  switch(id){
    default:
    case PALETTELIST_VARIABLE_USER_01_ID: return &palettelist.users[0];
    case PALETTELIST_VARIABLE_USER_02_ID: return &palettelist.users[1];
    case PALETTELIST_VARIABLE_USER_03_ID: return &palettelist.users[2];
    case PALETTELIST_VARIABLE_USER_04_ID: return &palettelist.users[3];
    case PALETTELIST_VARIABLE_USER_05_ID: return &palettelist.users[4];
    case PALETTELIST_VARIABLE_USER_06_ID: return &palettelist.users[5];
    case PALETTELIST_VARIABLE_USER_07_ID: return &palettelist.users[6];
    case PALETTELIST_VARIABLE_USER_08_ID: return &palettelist.users[7];
    case PALETTELIST_VARIABLE_USER_09_ID: return &palettelist.users[8];
    case PALETTELIST_VARIABLE_USER_10_ID: return &palettelist.users[9];
    case PALETTELIST_VARIABLE_USER_11_ID: return &palettelist.users[10];
    case PALETTELIST_VARIABLE_USER_12_ID: return &palettelist.users[11];
    case PALETTELIST_VARIABLE_USER_13_ID: return &palettelist.users[12];
    case PALETTELIST_VARIABLE_USER_14_ID: return &palettelist.users[13];
    case PALETTELIST_VARIABLE_USER_15_ID: return &palettelist.users[14];
    case PALETTELIST_VARIABLE_USER_16_ID: return &palettelist.users[15];
    case PALETTELIST_VARIABLE_USER_17_ID: return &palettelist.users[16];
    case PALETTELIST_VARIABLE_USER_18_ID: return &palettelist.users[17];
    case PALETTELIST_VARIABLE_USER_19_ID: return &palettelist.users[18];
    case PALETTELIST_VARIABLE_USER_20_ID: return &palettelist.users[19];
    case PALETTELIST_STATIC_SHELF_HEARTS_ID:    return &palettelist.shelf_hearts;
    case PALETTELIST_STATIC_HOLLOWEEN_OP_ID:    return &palettelist.holloween_op;
    case PALETTELIST_STATIC_HOLLOWEEN_OGP_ID:   return &palettelist.holloween_ogp;
    case PALETTELIST_STATIC_RAINBOW_ID:         return &palettelist.rainbow;
    case PALETTELIST_STATIC_RAINBOW_INVERTED_ID:return &palettelist.rainbow_inverted;
    case PALETTELIST_STATIC_PASTEL_ID:          return &palettelist.pastel;
    case PALETTELIST_STATIC_WINTER_ID:          return &palettelist.winter;
    case PALETTELIST_STATIC_AUTUMN_ID:          return &palettelist.autumn;
    case PALETTELIST_STATIC_AUTUMN_RED_ID:      return &palettelist.autumn_red;
    case PALETTELIST_STATIC_OCEAN_01_ID:        return &palettelist.ocean_01;
    case PALETTELIST_STATIC_GRADIENT_01_ID:     return &palettelist.gradient_01;
    case PALETTELIST_STATIC_GRADIENT_02_ID:     return &palettelist.gradient_02;
    case PALETTELIST_STATIC_BERRY_GREEN_ID:     return &palettelist.berry_green;
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
    case PALETTELIST_STATIC_CUSTOM_USER_01_ID: return &palettelist.custom_user_01;
    case PALETTELIST_STATIC_SUNRISE_01_ID:      return &palettelist.sunrise_01;
  }
}





int8_t mInterfaceLight::CheckPaletteIsEditable(PALETTELIST::PALETTE *ptr){
  if( (ptr->id>=PALETTELIST_VARIABLE_USER_01_ID)&&
      (ptr->id<=PALETTELIST_VARIABLE_USER_20_ID) ){
    return true;
  }
  return false;
}



// Check for friendly names first
const char* mInterfaceLight::GetPaletteFriendlyName(char* buffer, uint8_t buflen){
  #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
  return GetPaletteFriendlyNameByID(animation.palette_id, buffer, buflen);
  #else
  return GetPaletteFriendlyNameByID(0, buffer);
  #endif
} 
const char* mInterfaceLight::GetPaletteFriendlyNameByID(uint8_t id, char* buffer, uint8_t buflen){

  PALETTELIST::PALETTE *ptr = GetPalettePointerByID(id);
  
  if(id<PALETTELIST_VARIABLE_USER_LENGTH_ID){
    // dList
    return pCONT_sup->GetTextIndexed_P(buffer, buflen, id, pCONT_set->Settings.animation_settings.palette_user_variable_name_list_ctr);
  }
  else{
    // Progmem
    snprintf_P(buffer, buflen, "%S", ptr->friendly_name_ctr!=nullptr?ptr->friendly_name_ctr:WARNING_NOTHANDLED_CTR);
    return buffer;
  }

}



const char* mInterfaceLight::GetPaletteName(char* buffer, uint8_t buflen){
  
// #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
  return GetPaletteNameByID(animation.palette_id, buffer, buflen);
  // #else
  // return GetPaletteNameByID(0, buffer, buflen);
  // #endif
} 
const char* mInterfaceLight::GetPaletteNameByID(uint8_t id, char* buffer, uint8_t buflen){  
  PALETTELIST::PALETTE *ptr = GetPalettePointerByID(id);
  return ptr->friendly_name_ctr!=nullptr?ptr->friendly_name_ctr:WARNING_NOTHANDLED_CTR;
}


uint16_t mInterfaceLight::GetPixelsInMap(PALETTELIST::PALETTE *ptr, uint8_t pixel_width_contrained_limit){
  uint16_t pixel_width = 0;
  uint16_t pixel_length = 0;
  switch(ptr->flags.fMapIDs_Type){
    case MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID: pixel_width = 1; break;
    case MAPIDS_TYPE_HSBCOLOUR_NOINDEX_ID: pixel_width = 1; break;
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_ID: pixel_width = 2; break;
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_AND_SETALL_ID: pixel_width = 2; break;
    case MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID: pixel_width = 3; break;
    case MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX_ID: pixel_width = 5; break;
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_ID: pixel_width = 4; break;
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_AND_SETALL_ID: pixel_width = 4; break;
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID: pixel_width = 2; break;
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_GRADIENT_ID: pixel_width = 4; break;
  }
  // if(pixel_width){ //so we can divide if not 0
    pixel_length = ptr->colour_map_size/pixel_width; 
  // }else{
  //   pixel_length = ptr->colour_map_size/pixel_width
  // }
  //constrain limit if max
  // if(pixel_width_contrained_limit>=0){
  //   pixel_length = constrain(pixel_length,1,pixel_width_contrained_limit);
  //   // AddLog_P(LOG_LEVEL_TEST, PSTR("pixel_width_contrained_limit=%d, pixel_length=%d"),pixel_width_contrained_limit, pixel_length);
  // }
  return pixel_length;
}



const uint8_t* mInterfaceLight::GetDefaultColourPaletteUserIDs_P(uint8_t id){
  switch(id){
    default: 
    case PALETTELIST_VARIABLE_USER_01_ID: return DEFAULT_COLOUR_PALETTE_USER_01_IDS;
    case PALETTELIST_VARIABLE_USER_02_ID: return DEFAULT_COLOUR_PALETTE_USER_02_IDS;
    case PALETTELIST_VARIABLE_USER_03_ID: return DEFAULT_COLOUR_PALETTE_USER_03_IDS;
    case PALETTELIST_VARIABLE_USER_04_ID: return DEFAULT_COLOUR_PALETTE_USER_04_IDS;
    case PALETTELIST_VARIABLE_USER_05_ID: return DEFAULT_COLOUR_PALETTE_USER_05_IDS;
    case PALETTELIST_VARIABLE_USER_06_ID: return DEFAULT_COLOUR_PALETTE_USER_06_IDS;
    case PALETTELIST_VARIABLE_USER_07_ID: return DEFAULT_COLOUR_PALETTE_USER_07_IDS;
    case PALETTELIST_VARIABLE_USER_08_ID: return DEFAULT_COLOUR_PALETTE_USER_08_IDS;
    case PALETTELIST_VARIABLE_USER_09_ID: return DEFAULT_COLOUR_PALETTE_USER_09_IDS;
    case PALETTELIST_VARIABLE_USER_10_ID: return DEFAULT_COLOUR_PALETTE_USER_10_IDS;
    case PALETTELIST_VARIABLE_USER_11_ID: return DEFAULT_COLOUR_PALETTE_USER_11_IDS;
    case PALETTELIST_VARIABLE_USER_12_ID: return DEFAULT_COLOUR_PALETTE_USER_12_IDS;
    case PALETTELIST_VARIABLE_USER_13_ID: return DEFAULT_COLOUR_PALETTE_USER_13_IDS;
    case PALETTELIST_VARIABLE_USER_14_ID: return DEFAULT_COLOUR_PALETTE_USER_14_IDS;
    case PALETTELIST_VARIABLE_USER_15_ID: return DEFAULT_COLOUR_PALETTE_USER_15_IDS;
    case PALETTELIST_VARIABLE_USER_16_ID: return DEFAULT_COLOUR_PALETTE_USER_16_IDS;
    case PALETTELIST_VARIABLE_USER_17_ID: return DEFAULT_COLOUR_PALETTE_USER_17_IDS;
    case PALETTELIST_VARIABLE_USER_18_ID: return DEFAULT_COLOUR_PALETTE_USER_18_IDS;
    case PALETTELIST_VARIABLE_USER_19_ID: return DEFAULT_COLOUR_PALETTE_USER_19_IDS;
    case PALETTELIST_VARIABLE_USER_20_ID: return DEFAULT_COLOUR_PALETTE_USER_20_IDS;
  }
}


uint8_t mInterfaceLight::GetDefaultColourPaletteUserIDsCount(uint8_t id){


  // Serial.printf("GetDefaultColourPaletteUserIDsCount\n\r"); Serial.flush();

  auto id_ptr = GetDefaultColourPaletteUserIDs_P(id);
  // Serial.printf(GetDefaultColourPaletteUserIDs_P(id));

  
  // const uint8_t* id_ptr = DEFAULT_COLOUR_PALETTE_USER_01_IDS;

  // Serial.printf("GetDefaultColourPaletteUserIDsCountBB=%d\n\r",   pgm_read_byte(id_ptr)); Serial.flush();

  //search for COLOUR_MAP_NONE_ID to signify end, must be less than 20 in length

  uint8_t pixel_active = 0;
  uint8_t byte_read = 0;
  while(pixel_active<20){
    byte_read = pgm_read_byte(id_ptr+pixel_active);
    // Serial.printf("byte_read=%d",byte_read);
    if(byte_read!=COLOUR_MAP_NONE_ID){//} && byte_read<COLOUR_MAP_PALETTE_OPTIONS_END_ID){
      pixel_active++;
    }else{
      break;
    }
  }
  // Serial.printf("pixel_active=%d\n\r",   pixel_active); Serial.flush();

  return pixel_active>PALETTELIST_COLOUR_USERS_AMOUNT_MAX?PALETTELIST_COLOUR_USERS_AMOUNT_MAX:pixel_active;
}




// point things to memory locations
void mInterfaceLight::init_PresetColourPalettes_UserFill(uint8_t id){//} colour_ids, const uint8_t colour_ids_len){

  palettelist.ptr     = &palettelist.users[id];
  palettelist.ptr->id = id;

  // Point to memory location
  palettelist.ptr->colour_map_id = &pCONT_set->Settings.animation_settings.palette_user_colour_map_ids[id*20];//palettelist.ptr->id*20];//[0];

  // for()
  
  palettelist.ptr->colour_map_size = 0;//Get
  palettelist.ptr->friendly_name_ctr = nullptr;//pCONT_set->Settings.animation_settings.palette_user_variable_name_list_ctr;//&palettelist_variable_users_ctr[ptr->id][0];
  
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID;

  // Serial.printf("colour_ids[%d]\n\r",id); Serial.flush();
}

void mInterfaceLight::setdefault_PresetColourPalettes_UserFill(uint8_t id){//} colour_ids, const uint8_t colour_ids_len){

  palettelist.ptr     = &palettelist.users[id];

  // Overwrite memory locations
  // memset(&palettelist.ptr->colour_map_id,COLOUR_MAP_NONE_ID,sizeof(char)*PALETTELIST_VARIABLE_USER_LENGTH_ID); // assume >amount == NONE

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


  // palettelist.ptr->colour_map_id = test;

  // memcpy_P(palettelist.ptr->colour_map_id, id_ptr, sizeof(uint8_t)*1);

  // uint8_t test[20];
  // uint8_t* colour_map_id = test;
  // memcpy_P(colour_map_id, id_ptr, sizeof(uint8_t)*1);

  
  // Serial.printf("colour_map_id[%d]\n\r",palettelist.ptr->colour_map_id[0]); Serial.flush();
  // Serial.printf("test[%d]\n\r",palettelist.ptr->colour_map_id[0]); Serial.flush();
  // Serial.printf("colour_map_id[%d]\n\r",palettelist.ptr->colour_map_id[1]); Serial.flush();

}

// uint8_t mInterfaceLight::GetPaletteUserActivePixels()


void mInterfaceLight::SetPaletteListPtrFromID(uint8_t id){
  //AddLog_P(LOG_LEVEL_TEST,PSTR("SetPaletteListPtrFromID(%d)"),id);
  palettelist.ptr = GetPalettePointerByID(id);
}



RgbcctColor mInterfaceLight::GetColourFromPalette(PALETTELIST::PALETTE *ptr, uint16_t pixel_num, int16_t *pixel_position)
{

  RgbcctColor colour = RgbcctColor(0,255,0,0,0);
  uint8_t palette_elements[ptr->colour_map_size];
  uint16_t index_relative = 0; // get expected pixel position
  uint16_t expected_pixel_count = 0; // get expected pixel position

  if(ptr->id < PALETTELIST_VARIABLE_USER_LENGTH_ID){
    memcpy(&palette_elements,ptr->colour_map_id,sizeof(uint8_t)*ptr->colour_map_size);
  }
  // progmem
  else{
    memcpy_P(&palette_elements,ptr->colour_map_id,sizeof(uint8_t)*ptr->colour_map_size);
  }

  #ifdef ENABLE_LOG_LEVEL_DEBUG
  //AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "fMapIDs_Type=%d"),ptr->flags.fMapIDs_Type);
  #endif

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
    }
    break;
  }

  if(animation.flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique){
    HsbColor hsb = RgbColor(colour.R, colour.G, colour.B);
    hsb.S = GetRandomSaturationVariation(100,8,92,100)/100.0f;
    colour = hsb;
    // add to keep ww/cw
  }

  return colour;

} // end function

/*******************************************************************************************************************
********************************************************************************************************************
************ END OF PALETTE DEFINITIONS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/

#endif
