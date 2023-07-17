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
  
  PALETTE pal = {0};
  uint8_t adjusted_id = 0;

  if(palettelist.size() > 0)
  {    
    ALOG_INF( PSTR("init_PresetColourPalettes - ALREADY SET, DO NOT RUN AGAIN") );
    return;
  }
  

  /**
   *  Static 
   * 
   * init_PresetColourPalettes_User_RGBCCT_Fill(uint8_t id)
   * 
   * */

  for(
    uint8_t id=0; 
            id<(PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID-PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_01__ID); 
            id++
  ){

    ALOG_ERR(PSTR("THESE ARE NOT HERE ANYMORE, keeping to permit compile"));

    pal.id = id+PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_01__ID;
    pal.friendly_name_ctr = nullptr;
    pal.data = &pCONT_set->Settings.animation_settings.palette_rgbcct_users_colour_map[id*5]; // Point to memory location
    pal.data_length = 5; // each is 5 bytes wide, 1 pixel = 5 bytes total    
    pal.encoding.data       = PALETTE_ENCODING_INCLUDES_RGBCCT_NO_INDEX;

    palettelist.push_back(pal); ALOG_INF( PSTR("palettelist %d|%d"), __LINE__, palettelist.size() );

  }


  /**
   *  Static 
   * 
   * Move these all into this function directly, no more subfunctions, will make adding more easier and take up less codes
   * 
   * Similar to mqtt handler, can this also be vectorised? or like WLED effects?
   * ie push a new palette into the vectorised version. This would make all of the palette and functions much easier to get with functions without long switch cases
   * 
   * */

  for(
    uint8_t id=0; 
            id<(PALETTELIST_VARIABLE_HSBID_LENGTH__ID-PALETTELIST_VARIABLE_HSBID_01__ID);
            id++
  ){
          
    // Step 1: Set the memory location that holds the palette
    pal.id = id+PALETTELIST_VARIABLE_HSBID_01__ID;
    pal.friendly_name_ctr = nullptr; // Saved in DeviceName
    pal.data = &hsbid_pals[id].encoded_data[0];     // Point to memory location
    pal.data_length = 20;
    pal.encoding.data       = D_PALETTE_HSBID_01_ENCODING;

    // Step 2: Load in the default states (step 3 later will override these from filesystem read)
    auto    default_data_ptr    = GetDefaultColourPaletteUserIDs_P(id);
    uint8_t default_data_length = GetDefaultColourPaletteUserIDsCount(id);    
    memset(pal.data, COLOUR_MAP_NONE__ID, sizeof(uint8_t)*20); // Clear back to none_id    
    memcpy_P(pal.data, default_data_ptr, sizeof(uint8_t)*default_data_length); // Read from PROGMEM the defaults
    pal.data_length = default_data_length; // set pixels in palette

    palettelist.push_back(pal); ALOG_INF( PSTR("palettelist %d|%d"), __LINE__, palettelist.size() );

  }

// void mPalette::init_PresetColourPalettes_User_Generic_Fill(uint8_t id)
// {

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

  for(
    uint8_t id=0; 
            id<(PALETTELIST_VARIABLE_GENERIC_LENGTH__ID-PALETTELIST_VARIABLE_GENERIC_01__ID); 
            id++
  ){
    
    // palettelist.ptr     = &palettelist.encoded_users;
    pal.id = id+PALETTELIST_VARIABLE_GENERIC_01__ID;

    // palettelist.ptr->data_length = pCONT_set->Settings.animation_settings.palette_encoded_users_colour_map[0] < 100 ?  pCONT_set->Settings.animation_settings.palette_encoded_users_colour_map[0] : 0; 
    // palettelist.ptr->flags.fMapIDs_Type = pCONT_set->Settings.animation_settings.palette_encoded_users_colour_map[1]; 
    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ This will need to be extended to probably use both of the first bytes

    uint8_t pixel_width = GetEncodedColourWidth(pal.encoding);
    // AddLog(LOG_LEVEL_TEST, PSTR("pixel_width %d"),pixel_width);
    pal.data_length = pixel_width*pCONT_set->Settings.animation_settings.palette_encoded_users_colour_map[0];
    


    // 
    // palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID;

    //need to create "palette encoded" to "saved buffer" function ie decoder
    //Starts after the encoded info is taken out
    pal.data = &pCONT_set->Settings.animation_settings.palette_encoded_users_colour_map[5];//id*5];
        
    // to be moved into devicelist
    pal.friendly_name_ctr = nullptr;//pCONT_set->Settings.animation_settings.palette_user_variable_name_list_ctr;//&palettelist_variable_users_ctr[ptr->id][0];
    
    // AddLog(LOG_LEVEL_TEST, PSTR("init_PresetColourPalettes_User_Generic_Fill %d"),palettelist.ptr->data[0]);
    // AddLog(LOG_LEVEL_TEST, PSTR("init_PresetColourPalettes_User_Generic_Fill size %d"),palettelist.ptr->data_length);

    // AddLog(LOG_LEVEL_TEST, PSTR("pixels in map test size %d"),GetNumberOfColoursInPalette(palettelist.ptr));
    
    //     RgbcctColor colour = mPaletteI->GetColourFromPalette(mPaletteI->GetPalettePointerByID(15),0);

    //     AddLog(LOG_LEVEL_TEST, PSTR("colour_pal  =%d,%d,%d,%d,%d"), colour.R, colour.G, colour.B, colour.WW, colour.WC);

    palettelist.push_back(pal); ALOG_INF( PSTR("palettelist %d|%d"), __LINE__, palettelist.size() );

  }

  /**
   * @brief These below to become fully added into my methods as another encoding type
   * to be merged and named in my static list
   * 
   * 
   * These need to be added into my palette method at least by name only, data can be ignored
   * Or, the data for CRGB16 is saved in the data field and then cast back to use?? This would reduce the need to keep 
   * a static version of it. OR, simply do the math in C16 but convert back? or, do equivalent of these with mine.
   */

  palettelist.push_back(
    {
      PALETTELIST_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_01__ID, 
      (char*)PM_PALETTE_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_01_NAME_CTR,
      nullptr, // Calculated at runtime
      0,       // Calculated at runtime
      D_PALETTE_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_01_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_02__ID, 
      (char*)PM_PALETTE_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_02_NAME_CTR,
      nullptr, // Calculated at runtime
      0,       // Calculated at runtime
      D_PALETTE_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_02_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_03__ID, 
      (char*)PM_PALETTE_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_03_NAME_CTR,
      nullptr, // Calculated at runtime
      0,       // Calculated at runtime
      D_PALETTE_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_03_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_04__ID, 
      (char*)PM_PALETTE_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_04_NAME_CTR,
      nullptr, // Calculated at runtime
      0,       // Calculated at runtime
      D_PALETTE_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_04_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_05__ID, 
      (char*)PM_PALETTE_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_05_NAME_CTR,
      nullptr, // Calculated at runtime
      0,       // Calculated at runtime
      D_PALETTE_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_05_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY__ID, 
      (char*)PM_PALETTE_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_NAME_CTR,
      nullptr, // Calculated at runtime
      0,       // Calculated at runtime
      D_PALETTE_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY__ID, 
      (char*)PM_PALETTE_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY_NAME_CTR,
      nullptr, // Calculated at runtime
      0,       // Calculated at runtime
      D_PALETTE_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY_TERTIARY__ID, 
      (char*)PM_PALETTE_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY_TERTIARY_NAME_CTR,
      nullptr, // Calculated at runtime
      0,       // Calculated at runtime
      D_PALETTE_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY_TERTIARY_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY_TERTIARY_REPEATED__ID, 
      (char*)D_PALETTE_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY_TERTIARY_REPEAT_NAME_CTR,
      nullptr, // Calculated at runtime
      0,       // Calculated at runtime
      D_PALETTE_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY_TERTIARY_REPEAT_ENCODING
    }
  );


  palettelist.push_back(
    {
      PALETTELIST_STATIC_PARTY_DEFAULT__ID, 
      (char*)PM_PALETTE_PARTY_DEFAULT_NAME_CTR,
      (uint8_t*)PM_PALETTE_PARTY_DEFAULT_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_PARTY_DEFAULT_COLOUR_MAP__DATA),
      D_PALETTE_PARTY_DEFAULT_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_HOLLOWEEN_OP__ID, 
      (char*)PM_PALETTE_HOLLOWEEN_OP_NAME_CTR,
      (uint8_t*)PM_PALETTE_HOLLOWEEN_OP_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_HOLLOWEEN_OP_COLOUR_MAP__DATA),
      D_PALETTE_HOLLOWEEN_OP_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_HOLLOWEEN_OGP__ID, 
      (char*)PM_PALETTE_HOLLOWEEN_OGP_NAME_CTR,
      (uint8_t*)PM_PALETTE_HOLLOWEEN_OGP_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_HOLLOWEEN_OGP_COLOUR_MAP__DATA),
      D_PALETTE_HOLLOWEEN_OGP_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_HOT_PINK_NEON_WITH_NAVY__ID, 
      (char*)PM_PALETTE_HOT_PINK_NEON_WITH_NAVY_NAME_CTR,
      (uint8_t*)PM_PALETTE_HOT_PINK_NEON_WITH_NAVY_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_HOT_PINK_NEON_WITH_NAVY_COLOUR_MAP__DATA),
      D_PALETTE_HOT_PINK_NEON_WITH_NAVY_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_RAINBOW__ID, 
      (char*)PM_PALETTE_RAINBOW_NAME_CTR,
      (uint8_t*)PM_PALETTE_RAINBOW_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_RAINBOW_COLOUR_MAP__DATA),
      D_PALETTE_RAINBOW_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_RAINBOW_INVERTED__ID, 
      (char*)PM_PALETTE_RAINBOW_INVERTED_NAME_CTR,
      (uint8_t*)PM_PALETTE_RAINBOW_INVERTED_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_RAINBOW_INVERTED_COLOUR_MAP__DATA),
      D_PALETTE_RAINBOW_INVERTED_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_PASTEL_01__ID, 
      (char*)PM_PALETTE_PASTEL_01_NAME_CTR,
      (uint8_t*)PM_PALETTE_PASTEL_01_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_PASTEL_01_COLOUR_MAP__DATA),
      D_PALETTE_PASTEL_01_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_PASTEL_02__ID, 
      (char*)PM_PALETTE_PASTEL_02_NAME_CTR,
      (uint8_t*)PM_PALETTE_PASTEL_02_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_PASTEL_02_COLOUR_MAP__DATA),
      D_PALETTE_PASTEL_02_ENCODING
    }
  );
  palettelist.push_back(
    {
      PALETTELIST_STATIC_PASTEL_03__ID, 
      (char*)PM_PALETTE_PASTEL_03_NAME_CTR,
      (uint8_t*)PM_PALETTE_PASTEL_03_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_PASTEL_03_COLOUR_MAP__DATA),
      D_PALETTE_PASTEL_03_ENCODING
    }
  );
  palettelist.push_back(
    {
      PALETTELIST_STATIC_PASTEL_04__ID, 
      (char*)PM_PALETTE_PASTEL_04_NAME_CTR,
      (uint8_t*)PM_PALETTE_PASTEL_04_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_PASTEL_04_COLOUR_MAP__DATA),
      D_PALETTE_PASTEL_04_ENCODING
    }
  );
  palettelist.push_back(
    {
      PALETTELIST_STATIC_PASTEL_05__ID, 
      (char*)PM_PALETTE_PASTEL_05_NAME_CTR,
      (uint8_t*)PM_PALETTE_PASTEL_05_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_PASTEL_05_COLOUR_MAP__DATA),
      D_PALETTE_PASTEL_05_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_WINTER__ID, 
      (char*)PM_PALETTE_WINTER_NAME_CTR,
      (uint8_t*)PM_PALETTE_WINTER_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_WINTER_COLOUR_MAP__DATA),
      D_PALETTE_WINTER_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_AUTUMN_GREEN__ID, 
      (char*)PM_PALETTE_AUTUMN_GREEN_NAME_CTR,
      (uint8_t*)PM_PALETTE_AUTUMN_GREEN_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_AUTUMN_GREEN_COLOUR_MAP__DATA),
      D_PALETTE_AUTUMN_GREEN_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_AUTUMN_RED__ID, 
      (char*)PM_PALETTE_AUTUMN_RED_NAME_CTR,
      (uint8_t*)PM_PALETTE_AUTUMN_RED_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_AUTUMN_RED_COLOUR_MAP__DATA),
      D_PALETTE_AUTUMN_RED_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_GRADIENT_01__ID, 
      (char*)PM_PALETTE_GRADIENT_01_NAME_CTR,
      (uint8_t*)PM_PALETTE_GRADIENT_01_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_GRADIENT_01_COLOUR_MAP__DATA),
      D_PALETTE_GRADIENT_01_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_GRADIENT_02__ID, 
      (char*)PM_PALETTE_GRADIENT_02_NAME_CTR,
      (uint8_t*)PM_PALETTE_GRADIENT_02_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_GRADIENT_02_COLOUR_MAP__DATA),
      D_PALETTE_GRADIENT_02_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_GRADIENT_PASTEL_TONES_PURPLE__ID, 
      (char*)PM_PALETTE_GRADIENT_PASTEL_TONES_PURPLE_NAME_CTR,
      (uint8_t*)PM_PALETTE_GRADIENT_PASTEL_TONES_PURPLE_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_GRADIENT_PASTEL_TONES_PURPLE_COLOUR_MAP__DATA),
      D_PALETTE_GRADIENT_PASTEL_TONES_PURPLE_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_BERRY_GREEN__ID, 
      (char*)PM_PALETTE_BERRY_GREEN_NAME_CTR,
      (uint8_t*)PM_PALETTE_BERRY_GREEN_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_BERRY_GREEN_COLOUR_MAP__DATA),
      D_PALETTE_BERRY_GREEN_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_01__ID, 
      (char*)PM_PALETTE_CHRISTMAS_01_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_01_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_01_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_01_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_02__ID, 
      (char*)PM_PALETTE_CHRISTMAS_02_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_02_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_02_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_02_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_03__ID, 
      (char*)PM_PALETTE_CHRISTMAS_03_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_03_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_03_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_03_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_04__ID, 
      (char*)PM_PALETTE_CHRISTMAS_04_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_04_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_04_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_04_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_05__ID, 
      (char*)PM_PALETTE_CHRISTMAS_05_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_05_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_05_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_05_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_06__ID, 
      (char*)PM_PALETTE_CHRISTMAS_06_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_06_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_06_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_06_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_07__ID, 
      (char*)PM_PALETTE_CHRISTMAS_07_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_07_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_07_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_07_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_08__ID, 
      (char*)PM_PALETTE_CHRISTMAS_08_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_08_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_08_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_08_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_09__ID, 
      (char*)PM_PALETTE_CHRISTMAS_09_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_09_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_09_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_09_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_10__ID, 
      (char*)PM_PALETTE_CHRISTMAS_10_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_10_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_10_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_10_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_11__ID, 
      (char*)PM_PALETTE_CHRISTMAS_11_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_11_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_11_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_11_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_12__ID, 
      (char*)PM_PALETTE_CHRISTMAS_12_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_12_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_12_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_12_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_13__ID, 
      (char*)PM_PALETTE_CHRISTMAS_13_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_13_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_13_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_13_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_14__ID, 
      (char*)PM_PALETTE_CHRISTMAS_14_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_14_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_14_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_14_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_15__ID, 
      (char*)PM_PALETTE_CHRISTMAS_15_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_15_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_15_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_15_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_16__ID, 
      (char*)PM_PALETTE_CHRISTMAS_16_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_16_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_16_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_16_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_17__ID, 
      (char*)PM_PALETTE_CHRISTMAS_17_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_17_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_17_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_17_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_18__ID, 
      (char*)PM_PALETTE_CHRISTMAS_18_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_18_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_18_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_18_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_19__ID, 
      (char*)PM_PALETTE_CHRISTMAS_19_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_19_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_19_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_19_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_20__ID, 
      (char*)PM_PALETTE_CHRISTMAS_20_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_20_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_20_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_20_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_21__ID, 
      (char*)PM_PALETTE_CHRISTMAS_21_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_21_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_21_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_21_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_22__ID, 
      (char*)PM_PALETTE_CHRISTMAS_22_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_22_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_22_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_22_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_23__ID, 
      (char*)PM_PALETTE_CHRISTMAS_23_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_23_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_23_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_23_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_24__ID, 
      (char*)PM_PALETTE_CHRISTMAS_24_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_24_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_24_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_24_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_25__ID, 
      (char*)PM_PALETTE_CHRISTMAS_25_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_25_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_25_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_25_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_26__ID, 
      (char*)PM_PALETTE_CHRISTMAS_26_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_26_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_26_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_26_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_27__ID, 
      (char*)PM_PALETTE_CHRISTMAS_27_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_27_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_27_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_27_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_28__ID, 
      (char*)PM_PALETTE_CHRISTMAS_28_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_28_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_28_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_28_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_29__ID, 
      (char*)PM_PALETTE_CHRISTMAS_29_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_29_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_29_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_29_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CHRISTMAS_30__ID, 
      (char*)PM_PALETTE_CHRISTMAS_30_NAME_CTR,
      (uint8_t*)PM_PALETTE_CHRISTMAS_30_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CHRISTMAS_30_COLOUR_MAP__DATA),
      D_PALETTE_CHRISTMAS_30_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_SUNRISE_01__ID, 
      (char*)PM_PALETTE_SUNRISE_01_NAME_CTR,
      (uint8_t*)PM_PALETTE_SUNRISE_01_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_SUNRISE_01_COLOUR_MAP__DATA),
      D_PALETTE_SUNRISE_01_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_SUNRISE_02__ID, 
      (char*)PM_PALETTE_SUNRISE_02_NAME_CTR,
      (uint8_t*)PM_PALETTE_SUNRISE_02_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_SUNRISE_02_COLOUR_MAP__DATA),
      D_PALETTE_SUNRISE_02_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_SUNRISE_03__ID, 
      (char*)PM_PALETTE_SUNRISE_03_NAME_CTR,
      (uint8_t*)PM_PALETTE_SUNRISE_03_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_SUNRISE_03_COLOUR_MAP__DATA),
      D_PALETTE_SUNRISE_03_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_SUNRISE_04__ID, 
      (char*)PM_PALETTE_SUNRISE_04_NAME_CTR,
      (uint8_t*)PM_PALETTE_SUNRISE_04_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_SUNRISE_04_COLOUR_MAP__DATA),
      D_PALETTE_SUNRISE_04_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_SUNSET_01__ID, 
      (char*)PM_PALETTE_SUNSET_01_NAME_CTR,
      (uint8_t*)PM_PALETTE_SUNSET_01_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_SUNSET_01_COLOUR_MAP__DATA),
      D_PALETTE_SUNSET_01_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_SUNSET_02__ID, 
      (char*)PM_PALETTE_SUNSET_02_NAME_CTR,
      (uint8_t*)PM_PALETTE_SUNSET_02_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_SUNSET_02_COLOUR_MAP__DATA),
      D_PALETTE_SUNSET_02_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CANDLE_FLAME_01__ID, 
      (char*)PM_PALETTE_CANDLE_FLAME_01_NAME_CTR,
      (uint8_t*)PM_PALETTE_CANDLE_FLAME_01_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CANDLE_FLAME_01_COLOUR_MAP__DATA),
      D_PALETTE_CANDLE_FLAME_01_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_GRADIENT_FIRE_01__ID, 
      (char*)PM_PALETTE_GRADIENT_FIRE_01_NAME_CTR,
      (uint8_t*)PM_PALETTE_GRADIENT_FIRE_01_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_GRADIENT_FIRE_01_COLOUR_MAP__DATA),
      D_PALETTE_GRADIENT_FIRE_01_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_SKY_GLOW_01__ID, 
      (char*)PM_PALETTE_SKY_GLOW_01_NAME_CTR,
      (uint8_t*)PM_PALETTE_SKY_GLOW_01_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_SKY_GLOW_01_COLOUR_MAP__DATA),
      D_PALETTE_SKY_GLOW_01_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_01__ID, 
      (char*)PM_PALETTE_GRADIENT_SUNLEVEL_GROUP01_01_NAME_CTR,
      (uint8_t*)PM_PALETTE_GRADIENT_SUNLEVEL_GROUP01_01_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_GRADIENT_SUNLEVEL_GROUP01_01_COLOUR_MAP__DATA),
      D_PALETTE_GRADIENT_SUNLEVEL_GROUP01_01_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_02__ID, 
      (char*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_02_NAME_CTR,
      (uint8_t*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_02_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_02_COLOUR_MAP__DATA),
      D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_02_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_03__ID, 
      (char*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_03_NAME_CTR,
      (uint8_t*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_03_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_03_COLOUR_MAP__DATA),
      D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_03_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_04__ID, 
      (char*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_04_NAME_CTR,
      (uint8_t*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_04_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_04_COLOUR_MAP__DATA),
      D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_04_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_05__ID, 
      (char*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_05_NAME_CTR,
      (uint8_t*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_05_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_05_COLOUR_MAP__DATA),
      D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_05_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_06__ID, 
      (char*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_06_NAME_CTR,
      (uint8_t*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_06_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_06_COLOUR_MAP__DATA),
      D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_06_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_07__ID, 
      (char*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_NAME_CTR,
      (uint8_t*)PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_COLOUR_MAP__DATA),
      D_PALETTE_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_OCEAN_01__ID, 
      (char*)PM_PALETTE_OCEAN_01_NAME_CTR,
      (uint8_t*)PM_PALETTE_OCEAN_01_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_OCEAN_01_COLOUR_MAP__DATA),
      D_PALETTE_OCEAN_01_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_CUSTOM_USER_01__ID, 
      (char*)PM_PALETTE_CUSTOM_USER_01_NAME_CTR,
      (uint8_t*)PM_PALETTE_CUSTOM_USER_01_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_CUSTOM_USER_01_COLOUR_MAP__DATA),
      D_PALETTE_CUSTOM_USER_01_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_SINGLE_FIRE_01__ID, 
      (char*)PM_PALETTE_SINGLE_FIRE_01_NAME_CTR,
      (uint8_t*)PM_PALETTE_SINGLE_FIRE_01_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_SINGLE_FIRE_01_COLOUR_MAP__DATA),
      D_PALETTE_SINGLE_FIRE_01_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_FLOWER_SWEATPEAS_01__ID, 
      (char*)PM_PALETTE_FLOWER_SWEATPEAS_01_NAME_CTR,
      (uint8_t*)PM_PALETTE_FLOWER_SWEATPEAS_01_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_FLOWER_SWEATPEAS_01_COLOUR_MAP__DATA),
      D_PALETTE_FLOWER_SWEATPEAS_01_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_FLOWER_SWEATPEAS_02__ID, 
      (char*)PM_PALETTE_FLOWER_SWEATPEAS_02_NAME_CTR,
      (uint8_t*)PM_PALETTE_FLOWER_SWEATPEAS_02_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_FLOWER_SWEATPEAS_02_COLOUR_MAP__DATA),
      D_PALETTE_FLOWER_SWEATPEAS_02_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_PINK_PURPLE_01__ID, 
      (char*)PM_PALETTE_PINK_PURPLE_01_NAME_CTR,
      (uint8_t*)PM_PALETTE_PINK_PURPLE_01_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_PINK_PURPLE_01_COLOUR_MAP__DATA),
      D_PALETTE_PINK_PURPLE_01_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_PINK_PURPLE_02__ID, 
      (char*)PM_PALETTE_PINK_PURPLE_02_NAME_CTR,
      (uint8_t*)PM_PALETTE_PINK_PURPLE_02_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_PINK_PURPLE_02_COLOUR_MAP__DATA),
      D_PALETTE_PINK_PURPLE_02_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_PINK_PURPLE_03__ID, 
      (char*)PM_PALETTE_PINK_PURPLE_03_NAME_CTR,
      (uint8_t*)PM_PALETTE_PINK_PURPLE_03_COLOUR_MAP__DATA, 
      sizeof(PM_PALETTE_PINK_PURPLE_03_COLOUR_MAP__DATA),
      D_PALETTE_PINK_PURPLE_03_ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01__ID, 
      (char*)PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_01__CTR,
      (uint8_t*)PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_01__DATA, 
      sizeof(PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_01__DATA),
      D__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_01__ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_02__ID, 
      (char*)PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_02__CTR,
      (uint8_t*)PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_02__DATA, 
      sizeof(PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_02__DATA),
      D__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_02__ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_03__ID, 
      (char*)PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_03__CTR,
      (uint8_t*)PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_03__DATA, 
      sizeof(PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_03__DATA),
      D__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_03__ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_04__ID, 
      (char*)PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_04__CTR,
      (uint8_t*)PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_04__DATA, 
      sizeof(PM__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_04__DATA),
      D__SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_IN_INDEX_04__ENCODING
    }
  );

  palettelist.push_back(
    {
      PALETTELIST_STATIC_DUAL_COLOUR_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01__ID, 
      (char*)PM__DUAL_COLOUR_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01__CTR,
      (uint8_t*)PM__DUAL_COLOUR_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01__DATA, 
      sizeof(PM__DUAL_COLOUR_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01__DATA),
      D__DUAL_COLOUR_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01__ENCODING
    }
  );



  // DEBUG_LINE_HERE;
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


// bool mPalette::CheckPaletteIsEditable(PALETTELIST::PALETTE *ptr)
// {
//   return CheckPaletteByIDIsEditable(ptr->id);
// }
// bool mPalette::CheckPaletteByIDIsEditable(uint8_t id)
// {
//   if(( (id>=PALETTELIST_VARIABLE_HSBID_01__ID)&&
//       (id<PALETTELIST_VARIABLE_HSBID_LENGTH__ID) )
//     || 
//     ( (id>=PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_01__ID)&&
//       (id<PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID) )
//     ){
//     return true;
//   }
//   return false;
// }


int16_t mPalette::Get_Static_PaletteIDbyName(const char* c){
  if(*c=='\0'){
    return -1;
  }

  // PALETTELIST::PALETTE *ptr = nullptr;
  char buffer[100] = {0};
  int16_t index_found = -1;

  /**************************************************************
   * 
   * PALETTELIST_VARIABLE_HSBID__IDS - undefined user names, default naming
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
   * PALETTELIST_VARIABLE_GENERIC__IDS - undefined user names, default naming
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
   * PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR__IDS - undefined user names, default naming
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
   * PALETTELIST_STATIC__IDS
   * 
  ***************************************************************/

  for( // loops relative to exact palette id
    uint8_t ii=PALETTELIST_STATIC_PARTY_DEFAULT__ID;
            ii<(PALETTELIST_STATIC_LENGTH__ID - PALETTELIST_STATIC_PARTY_DEFAULT__ID);
            ii++
  ){
    // ptr = GetPalettePointerByID(ii);

    // // DEBUG_LINE_HERE;

    // // ALOG_INF( PSTR("Name \"%s\"=?\"%S\""), c, ptr->friendly_name_ctr );

    // if(ptr->friendly_name_ctr == nullptr)
    // {
    //   ALOG_DBM( PSTR("ptr->friendly_name_ctr == nullptr %d %s"), ii, c );
    // }

    // if(ptr->friendly_name_ctr != nullptr)
    // { 
      // ALOG_DBM( PSTR("ptr->friendly_name_ctr == %d %s %S"), ii, c, ptr->friendly_name_ctr );


      if(mSupport::CheckCommand_P(c, palettelist[ii].friendly_name_ctr))
      {
        ALOG_DBM( PSTR("MATCH \"%c\" %d"), c, ii ); 
        return ii;            
      }
    // }




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
   * PALETTELIST_STATIC_HTML_COLOUR_CODES__IDS
   * 
  ***************************************************************/
  // if(
  //   (palette_id >= mPalette::PALETTELIST_HTML_COLOUR__AliceBlue__ID) && (palette_id < mPalette::PALETTELIST_HTML_COLOUR__LENGTH__ID)
  // ){  

  //   uint16_t adjusted_id = palette_id - mPalette::PALETTELIST_HTML_COLOUR__AliceBlue__ID;
  //   uint8_t segIdx = pCONT_lAni->_segment_index_primary;

  //   const HtmlColorPair* colorPair = HtmlColorNames::Pair(adjusted_id);
  //   PGM_P searchName = reinterpret_cast<PGM_P>(pgm_read_ptr(&(colorPair->Name)));
  //   RgbcctColor colour = (HtmlColor)colorPair->Color;
  //   memcpy_P(buffer, searchName, sizeof(char)*strlen_P(searchName)+1);



  //   // char buff[] = "lightyellow";
    
  //   // uint8_t buffSize = sizeof(buff);

  //   // HtmlColor colour;
  //   // colour.Parse<HtmlColorNames>(buff, sizeof(buff));
    
  //   // char name[50] = {0};
  //   // for (uint8_t indexName = 0; indexName < HtmlColorNames::Count(); indexName++)
  //   // {
  //   //   const HtmlColorPair* colorPair = HtmlColorNames::Pair(indexName);
  //   //   PGM_P searchName = reinterpret_cast<PGM_P>(pgm_read_ptr(&(colorPair->Name)));
  //   //   RgbColor rgb = (HtmlColor)colorPair->Color;
  //   //   ALOG_INF(PSTR("[%d] \"%S\" {%d} = %d,%d,%d"), indexName, searchName, colorPair->Color, rgb.R, rgb.G, rgb.B);
  //   // }
    
  //   //     Serial.printf("ColourHTML=%s", colour.ToString<HtmlColorNames>(buff, buffSize));


  //   // ALOG_INF(PSTR(DEBUG_INSERT_PAGE_BREAK "Segment Count %d "), segments.size());



  //   ALOG_DBM(PSTR("[%d] \"%S\" {%d} = %d,%d,%d"), adjusted_id, searchName, colorPair->Color, colour.R, colour.G, colour.B);
   
  // }

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

  // if(palette_id < PALETTELIST_VARIABLE_GENERIC_LENGTH__ID)
  // {
  //   ALOG_ERR(PSTR("Function must be called from AnimatorLight"));
  //   return PM_SEARCH_NOMATCH;
  // }
    
    // ALOG_ERR(PSTR("palette_id = %d of %d"), palette_id, palettelist.size());
    // DEBUG_LINE_HERE;
  /**************************************************************
   * 
   * PALETTELIST_STATIC__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= mPalette::PALETTELIST_STATIC_PARTY_DEFAULT__ID) && (palette_id < mPalette::PALETTELIST_STATIC_LENGTH__ID))
  ){       
    // mPalette::PALETTELIST::PALETTE *ptr = mPaletteI->GetPalettePointerByID(palette_id); 

    // ALOG_ERR(PSTR("palette_id = %d of %d"), palette_id, palettelist.size());
    // DEBUG_LINE_HERE;

    ALOG_DBM( PSTR("Name \"%S\""), palettelist[palette_id].friendly_name_ctr );
    // DEBUG_LINE_HERE;
    snprintf_P(buffer, buflen, palettelist[palette_id].friendly_name_ctr);
  }
  
  /**************************************************************
   * 
   * PALETTELIST_SEGMENTS_STORED_VARIABLE_CRGBPALETTE16_PALETTES__IDS
   * 
  ***************************************************************/
  if(
    (
      (palette_id >= mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_01__ID) && 
      (palette_id < mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__LENGTH__ID))
  ){  
    // uint16_t adjusted_id = palette_id - mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__RANDOMISE_COLOURS_01__ID;
    // pCONT_sup->GetTextIndexed_P(buffer, buflen, adjusted_id, PM_SEGMENTS_STORED_VARIABLE_CRGBPALETTE16_PALETTES_NAMES_CTR);   
    
    ALOG_DBM( PSTR("Name \"%S\""), palettelist[palette_id].friendly_name_ctr );
    // DEBUG_LINE_HERE;
    snprintf_P(buffer, buflen, palettelist[palette_id].friendly_name_ctr);

  }
   
  /**************************************************************
   * 
   * PALETTELIST_STATIC_CRGBPALETTE16__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16__CLOUD_COLOURS__ID) && (palette_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID))
  ){  
    uint16_t adjusted_id = palette_id - mPalette::PALETTELIST_STATIC_CRGBPALETTE16__CLOUD_COLOURS__ID;
    pCONT_sup->GetTextIndexed_P(buffer, buflen, adjusted_id, PM_STATIC_CRGBPALETTE16_NAMES_CTR);   
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
   * PALETTELIST_STATIC_HTML_COLOUR_CODES__IDS
   * 
  ***************************************************************/
  if(
    (palette_id >= mPalette::PALETTELIST_HTML_COLOUR__AliceBlue__ID) && (palette_id < mPalette::PALETTELIST_HTML_COLOUR__LENGTH__ID)
  ){  

    uint16_t adjusted_id = palette_id - mPalette::PALETTELIST_HTML_COLOUR__AliceBlue__ID;
    uint8_t segIdx = pCONT_lAni->_segment_index_primary;

    const HtmlColorPair* colorPair = HtmlColorNames::Pair(adjusted_id);
    PGM_P searchName = reinterpret_cast<PGM_P>(pgm_read_ptr(&(colorPair->Name)));
    RgbcctColor colour = (HtmlColor)colorPair->Color;
    memcpy_P(buffer, searchName, sizeof(char)*strlen_P(searchName)+1);

    ALOG_DBM(PSTR("[%d] \"%S\" {%d} = %d,%d,%d"), adjusted_id, searchName, colorPair->Color, colour.R, colour.G, colour.B);
   
  }
  
  return buffer;

}

uint16_t mPalette::GetNumberOfColoursInPalette(uint16_t palette_id, uint8_t pixel_width_contrained_limit)
{

  // return GetNumberOfColoursInPalette(GetPalettePointerByID(palette_id), pixel_width_contrained_limit);



// /**
//  * @brief Get elements in a colour type
//  * 
//  * @param ptr 
//  * @param pixel_width_contrained_limit 
//  * @return uint16_t 
//  */
// uint16_t mPalette::GetNumberOfColoursInPalette(PALETTELIST::PALETTE *ptr, uint8_t pixel_width_contrained_limit){
  
  uint16_t encoded_colour_width = 0;
  uint16_t pixel_length = 0;

  //   // DEBUG_LINE_HERE; // this should be a fallback only
  // if(ptr == nullptr)
  // {
  //   // DEBUG_LINE_HERE; // this should be a fallback only
  //   ptr = palettelist.ptr; // use internal if not set already
  // }

  PALETTE pal = palettelist[palette_id];

  // Serial.println(ptr->encoding.data, BIN);

    // With this enabled, only the christmas palettes will be stable!!

    if(pal.encoding.red_enabled){ encoded_colour_width++; }
    if(pal.encoding.green_enabled){ encoded_colour_width++; }
    if(pal.encoding.blue_enabled){ encoded_colour_width++; }
    if(pal.encoding.white_warm_enabled){ encoded_colour_width++; }

    if(pal.encoding.white_cold_enabled){ encoded_colour_width++; }
    if(pal.encoding.encoded_value_byte_width){ encoded_colour_width += pal.encoding.encoded_value_byte_width; }

    if(pal.encoding.index_exact){ encoded_colour_width++; }
    if(pal.encoding.index_scaled_to_segment){ encoded_colour_width++; }
    if(pal.encoding.index_is_trigger_value_exact){ encoded_colour_width++; }
    if(pal.encoding.index_is_trigger_value_scaled100){ encoded_colour_width++; }
    
    if(pal.encoding.encoded_as_hsb_ids){ encoded_colour_width++; }
    if(pal.encoding.encoded_as_crgb_palette_16){ encoded_colour_width++; }
    if(pal.encoding.encoded_as_crgb_palette_256){ encoded_colour_width++; }
    if(pal.encoding.palette_can_be_modified){ encoded_colour_width++; }

    // ALOG_INF(PSTR("encoded_colour_width = %d"),encoded_colour_width);
   

  // recalculate size, it might not be the same
  // ptr->data_length = 
  // GetDefaultColourPaletteUserIDsCount
  // GetColourMapSize()
  // Dont do this everytime, as its costly on memory, only update on change

  if(encoded_colour_width==0)
  {
    ALOG_ERR(PSTR("encoded_colour_width==0, crash errorAA =%S"), pal.friendly_name_ctr);
    return 1;
  }
  
  // Serial.println(encoded_colour_width); Serial.flush();
  // if(encoded_colour_width){ //so we can divide if not 0
    pixel_length = pal.data_length/encoded_colour_width; 
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
// }




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


/**
 * Refresh value stored in palette, and return new value
 * */
uint8_t mPalette::GetColourMapSizeByPaletteID(uint8_t palette_id){
  
  uint8_t new_size = 1; // assumed 1 at least
  // PALETTELIST::PALETTE *ptr_tmp = GetPalettePointerByID(palette_id);

  
    mPalette::PALETTE *ptr_tmp = &mPaletteI->palettelist[palette_id];

  if((palette_id>=PALETTELIST_VARIABLE_HSBID_01__ID)&&(palette_id<PALETTELIST_VARIABLE_HSBID_LENGTH__ID))
  {
    //Count until "not set value"  
    // new_size = GetDefaultColourPaletteUserIDsCount(palette.id);

    uint8_t pixel_active = 0;
    uint8_t byte_read = 0;
    while(pixel_active<20){
      // byte_read = pgm_read_byte(id_ptr+pixel_active); // This doesnt seem right??
      // byte_read = pCONT_set->Settings.animation_settings.palette_hsbid_users_colour_map[(20*palette_id)+pixel_active];
      byte_read = hsbid_pals[palette_id].encoded_data[pixel_active];// pCONT_set->Settings.animation_settings.palette_hsbid_users_colour_map[(20*palette_id)+pixel_active];
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
  uint8_t  mcol, // will be phased out
  bool     flag_convert_pixel_index_to_get_exact_crgb_colour,   // added by me, to make my effects work with CRGBPalette16
  uint8_t  brightness_scale //255(default): No scaling, 0-255 scales the brightness of returned colour (remember all colours are saved in full 255 scale)
){ 
  
  uint8_t* palette_elements = pCONT_lAni->segments[pCONT_lAni->_segment_index_primary].palette_container->pData.data(); // If function gets internalised to strip, it wont need to define which data set

  /**
   * @brief If requested palette_id does not match preloaded, make sure to reload first
   **/
  if(palette_id != pCONT_lAni->segments[pCONT_lAni->_segment_index_primary].palette_container->loaded_palette_id)
  {
    pCONT_lAni->segments[pCONT_lAni->_segment_index_primary].palette_container->loaded_palette_id = palette_id;
    ALOG_INF(PSTR("GetColourFromPreloadedPalette Reload"));
  }

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
  
  //encoded value needs to be U32 since they can be 3 bytes wide
  uint8_t* encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
  bool     flag_map_scaling, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
  bool     flag_wrap,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
  
  
  uint8_t mcol, // will be phased out


  // Should be controlled by user option 
  bool     flag_convert_pixel_index_to_get_exact_crgb_colour,   // added by me, to make my effects work with CRGBPalette16
  
  
  // unless needed by WLED effects, phase out and apply inside effect
  uint8_t  brightness_scale //255(default): No scaling, 0-255 scales the brightness of returned colour (remember all colours are saved in full 255 scale)
  // uint8_t* discrete_colours_in_palette //ie length of palette as optional return
){

  RgbcctColor colour = RgbcctColor(0);

  #ifdef ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
  Serial.println(palette_id);
  DEBUG_LINE_HERE;
  #endif

    // ALOG_INF(PSTR("palette_id %d"), palette_id);
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
  )
  {  

    // ALOG_INF(PSTR("MINE %d"), palette_id);

    if(palette_id>mPaletteI->palettelist.size())
    {
      ALOG_ERR(PSTR("PALETTE EXCEEDS VECTOR SIZE")); delay(2000);
      return colour;
    }

    PALETTE *ptr = &mPaletteI->palettelist[palette_id];

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

    uint8_t pixels_in_map = GetNumberOfColoursInPalette(palette_id);  
    uint8_t colour_width  = GetEncodedColourWidth(ptr->encoding); 
    // Serial.println(pixels_in_map);
    uint8_t remainder = 0;
    if(pixels_in_map != 0)
    {
      remainder = _pixel_position%pixels_in_map;  // /ERROR, REUSING NAME!!!
    }
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

    const HtmlColorPair* colorPair = HtmlColorNames::Pair(adjusted_id);
    PGM_P searchName = reinterpret_cast<PGM_P>(pgm_read_ptr(&(colorPair->Name)));
    RgbcctColor colour = (HtmlColor)colorPair->Color;
   
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

    if(adjusted_id < RGBCCTCOLOURS_SIZE)
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

      // DEBUG_LINE_HERE;
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

// if(flag_wrap)
// {
// ALOG_INF(PSTR("%d flag_wrap{%d}"), _pixel_position, flag_wrap);
// }
    
    // ALOG_INF(PSTR("id_adjusted %d  \tpixel_pos%d in_palette{%d}"), palette_id_adjusted, _pixel_position, colours_in_palette);

    uint8_t pixel_position_adjust = _pixel_position;

    // DEBUG_LINE_HERE;

    
    // if(flag_convert_pixel_index_to_get_exact_crgb_colour)  // change this to auto switch between mPalette::PALETTELIST_VARIABLE_FASTLED_SEGMENT__COLOUR_01__ID && _pixel_position < 3) and this new method
    // {
    //   uint8_t pixels_in_segment = colours_in_palette;
    //   uint8_t remainder = _pixel_position%pixels_in_segment;
    //   uint8_t remainder_scaled255 = map(remainder, 0,pixels_in_segment-1, 0,255); //change to "scale8" for speed
    //   pixel_position_adjust = remainder_scaled255;

    if(flag_convert_pixel_index_to_get_exact_crgb_colour)
    {
      pixel_position_adjust = map(pixel_position_adjust, 0,15, 0,255); //gradient type when exact value is needed needs scaled into full range
    }

      
      
    //   // Serial.printf("pixel = %d|%d \t %d||%d\n\r", pixel_position, pixel_position_adjust , remainder, remainder_scaled255);

    // }
    // else //standard WLED method
    // {
      
      // if (SEGMENT.palette.id == 0 && mcol < 3) return SEGCOLOR_U32(mcol); //WS2812FX default
      // if(palette_id == 0 && _pixel_position < 3)
      // {        
      //   ALOG_ERR(PSTR("PHASED OUT! Not sure it is still needed?"));
      //   // return pCONT_lAni->SEGMENT_I(pCONT_lAni->getCurrSegmentId()).rgbcctcolors[_pixel_position]; 
      // }
      // 
      // DEBUG_LINE_HERE;
      if (flag_map_scaling) pixel_position_adjust = (_pixel_position*255)/(pCONT_lAni->_virtualSegmentLength -1);  // This scales out segment_index to segment_length as 0 to 255
      
      // DEBUG_LINE_HERE;
      
      // AddLog(LOG_LEVEL_TEST, PSTR("paletteIndex=%d"),paletteIndex);
      if (!flag_wrap) pixel_position_adjust = scale8(pixel_position_adjust, 240); //cut off blend at palette "end", 240, or 15/16*255=>240/255, so drop last 16th (15 to wrapped 0) gradient of colour

    // DEBUG_LINE_HERE;
    // }
        
    // CRGBPalette16 curPal;
    // if (transitional && _t) curPal = _t->_palT;
    // else                    loadPalette(curPal, palette);
    
    uint8_t segIdx = pCONT_lAni->_segment_index_primary;

    // ALOG_INF(PSTR("pixel_position_adjust %d"), pixel_position_adjust);
    // FastLED Get ColorFromPalette that is CRGBPalette16
    fastled_col = ColorFromPalette( pSEGMENT_I(segIdx).palette_container->CRGB16Palette16_Palette.data, pixel_position_adjust, brightness_scale, NOBLEND);// (pCONT_lAni->paletteBlend == 3)? NOBLEND:LINEARBLEND);
  
    colour = RgbcctColor(fastled_col.r, fastled_col.g, fastled_col.b) ;// RgbcctColor::GetRgbcctFromU32Colour(fastled_col32);


    if(_pixel_position < pSEGMENT_I(segIdx).palette_container->CRGB16Palette16_Palette.encoded_index.size())
    {

      uint8_t enc = pSEGMENT_I(segIdx).palette_container->CRGB16Palette16_Palette.encoded_index[_pixel_position];
      // ALOG_ERR(PSTR("enc[%d] = %d|%d,%d,%d"),_pixel_position,enc,colour.R,colour.G,colour.B);
      
    }

    // ALOG_ERR(PSTR("colour[%d] = %d,%d,%d"),pixel_position_adjust,colour.R,colour.G,colour.B);

  } // END of CRGBPalette's
  else
  {
    ALOG_INF(PSTR("Missing %d"), palette_id);
  }

  return colour;
}


void mPalette::LoadPalette_CRGBPalette16_Static(uint8_t palette_id, uint8_t seg_i)
{

  ALOG_INF(PSTR("LoadPalette_CRGBPalette16_Static %d"), palette_id);

  /******************************************************
   * PALETTELIST_STATIC_CRGBPALETTE16__IDS
   * No gradient information in palette bytes, CRGB16 will scale equally
   ******************************************************/
  switch (palette_id)
  {
    default:
    case PALETTELIST_STATIC_CRGBPALETTE16__PARTY_COLOUR__ID:    pSEGMENT_I(seg_i).palette_container->CRGB16Palette16_Palette.data = PartyColors_p;   break;
    case PALETTELIST_STATIC_CRGBPALETTE16__CLOUD_COLOURS__ID:   pSEGMENT_I(seg_i).palette_container->CRGB16Palette16_Palette.data = CloudColors_p;   break;
    case PALETTELIST_STATIC_CRGBPALETTE16__LAVA_COLOURS__ID:    pSEGMENT_I(seg_i).palette_container->CRGB16Palette16_Palette.data = LavaColors_p;    break;
    case PALETTELIST_STATIC_CRGBPALETTE16__OCEAN_COLOUR__ID:    pSEGMENT_I(seg_i).palette_container->CRGB16Palette16_Palette.data = OceanColors_p;   break;
    case PALETTELIST_STATIC_CRGBPALETTE16__FOREST_COLOUR__ID:   pSEGMENT_I(seg_i).palette_container->CRGB16Palette16_Palette.data = ForestColors_p;  break;
    case PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID:  pSEGMENT_I(seg_i).palette_container->CRGB16Palette16_Palette.data = RainbowColors_p; break;
    case PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_STRIPE_COLOUR__ID: pSEGMENT_I(seg_i).palette_container->CRGB16Palette16_Palette.data = RainbowStripeColors_p; break;
  }

  pSEGMENT_I(seg_i).palette_container->CRGB16Palette16_Palette.encoded_index.clear();
  for(uint8_t i=0;i<16;i++)
  {
    pSEGMENT_I(seg_i).palette_container->CRGB16Palette16_Palette.encoded_index.push_back(map(i, 0,15, 0, 255));
  }


  /******************************************************
   * PALETTELIST_CRGBPALETTE16_GRADIENT___PALETTES__IDS
   ******************************************************/
  if(IsWithinRange(palette_id, PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID, PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID))
  {
    uint16_t gradient_id = palette_id - PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID;
    byte tcp[72]; //support gradient palettes with up to 18 entries
    memcpy_P(tcp, (byte*)pgm_read_dword(&(gGradientPalettes[gradient_id])), 72);

    /**
     * @brief Loading uses the CRGBPalette to get the colours
     **/
    pSEGMENT_I(seg_i).palette_container->CRGB16Palette16_Palette.data.loadDynamicGradientPalette(tcp);

    /**
     * @brief To get the gradients data exactly, manually parse them 
     **/    
    pSEGMENT_I(seg_i).palette_container->CRGB16Palette16_Palette.encoded_index.clear();

    TRGBGradientPaletteEntryUnion* ent = (TRGBGradientPaletteEntryUnion*)(tcp);
    TRGBGradientPaletteEntryUnion u;

    // Count entries
    uint16_t count = 0;
    do {
        u = *(ent + count);
        count++;
    } while ( u.index != 255);

    u = *ent;
    int indexstart = 0;
    while( indexstart < 255) {
      indexstart = u.index;

      pSEGMENT_I(seg_i).palette_container->CRGB16Palette16_Palette.encoded_index.push_back(u.index);

      ent++;
      u = *ent;
    }

    ALOG_INF(PSTR("palette_container%d (seg%d) %d %d %d"), gradient_id, seg_i, pSEGMENT_I(seg_i).palette_container->CRGB16Palette16_Palette.data[0].r, pSEGMENT_I(seg_i).palette_container->CRGB16Palette16_Palette.data[0].g, pSEGMENT_I(seg_i).palette_container->CRGB16Palette16_Palette.data[0].b );
  }
  
}

#endif // header gaurd
