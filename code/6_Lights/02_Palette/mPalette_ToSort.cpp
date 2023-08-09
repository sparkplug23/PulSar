#include "mPalette.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE












































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
  for(int i=0;i<length;i++){ palette_tmp.data.push_back(data[i]); }
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


uint16_t mPalette::GetNumberOfColoursInPalette(uint16_t palette_id, uint8_t pixel_width_contrained_limit)
{
  
  uint16_t encoded_colour_width = 0;
  uint16_t pixel_length = 0;

  if(palette_id>static_palettes.size())
  {
    ALOG_ERR(PSTR("if(palette_id>static_palettes.size())"));
    return 1;//
  }

  STATIC_PALETTE pal = static_palettes[palette_id];

  // Serial.println(ptr->encoding.data, BIN);

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



uint16_t mPalette::GetNumberOfColoursFromEncoded(PALETTE_ENCODING_DATA encoding, uint8_t data_in_palette)
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
  //   ptr = static_palettes.ptr; // use internal if not set already
  // }

  // if(palette_id>static_palettes.size())
  // {
  //   ALOG_ERR(PSTR("if(palette_id>static_palettes.size())"));
  //   return 1;//
  // }

  // PALETTE pal = static_palettes[palette_id];

  // Serial.println(ptr->encoding.data, BIN);

    // With this enabled, only the christmas palettes will be stable!!

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

    // ALOG_INF(PSTR("encoded_colour_width = %d"),encoded_colour_width);
   

  // recalculate size, it might not be the same
  // ptr->data_length = 
  // GetDefaultColourPaletteUserIDsCount
  // GetColourMapSize()
  // Dont do this everytime, as its costly on memory, only update on change

  // if(encoded_colour_width==0)
  // {
  //   ALOG_ERR(PSTR("encoded_colour_width==0, crash errorAA =%S"), pal.friendly_name_ctr);
  //   return 1;
  // }
  
  // Serial.println(encoded_colour_width); Serial.flush();
  // if(encoded_colour_width){ //so we can divide if not 0
    pixel_length = data_in_palette/encoded_colour_width; 
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

  

  // if((palette_id>=PALETTELIST_VARIABLE_HSBID_01__ID)&&(palette_id<PALETTELIST_VARIABLE_HSBID_LENGTH__ID))
  // {
  //   //Count until "not set value"  
  //   // new_size = GetDefaultColourPaletteUserIDsCount(palette.id);

  //   uint8_t pixel_active = 0;
  //   uint8_t byte_read = 0;
  //   while(pixel_active<20){
  //     // byte_read = pgm_read_byte(id_ptr+pixel_active); // This doesnt seem right??
  //     // byte_read = pCONT_set->Settings.animation_settings.palette_hsbid_users_colour_map[(20*palette_id)+pixel_active];
  //     byte_read = hsbid_pals[palette_id].encoded_data[pixel_active];// pCONT_set->Settings.animation_settings.palette_hsbid_users_colour_map[(20*palette_id)+pixel_active];
  //     // Serial.printf("byte_read=%d",byte_read);
  //     if(byte_read!=COLOUR_MAP_NONE__ID){//} && byte_read<COLOUR_MAP_PALETTE_OPTIONS_END__ID){
  //       pixel_active++;
  //     }else{
  //       break;
  //     }
  //   }
  //   new_size = pixel_active; // new count from local stored memory this time

  //   ptr_tmp->data_length = new_size;

  // }else
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
  bool     flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
  bool     flag_crgb_exact_colour,   // added by me, to make my effects work with CRGBPalette16
  uint8_t  brightness_scale //255(default): No scaling, 0-255 scales the brightness of returned colour (remember all colours are saved in full 255 scale)
){ 
  
  ALOG_INF(PSTR("GetColourFromPreloadedPalette PO")); 

  return pSEGMENT_I(0).GetColourFromPalette2(
    _pixel_position,
    flag_map_scaling,
    flag_wrap_hard_edge,
    flag_crgb_exact_colour    
  );

  uint8_t* palette_elements = pCONT_lAni->segments[pCONT_lAni->_segment_index_primary].palette_container->pData.data(); // If function gets internalised to strip, it wont need to define which data set

  /**
   * @brief If requested palette_id does not match preloaded, make sure to reload first
   **/
  if(palette_id != pCONT_lAni->segments[pCONT_lAni->_segment_index_primary].palette_container->loaded_palette_id)
  {
    pCONT_lAni->segments[pCONT_lAni->_segment_index_primary].palette_container->loaded_palette_id = palette_id;
    ALOG_INF(PSTR("GetColourFromPreloadedPalette Reload")); 
    DEBUG_LINE_HERE;
  }

  return mPaletteI->GetColourFromPreloadedPaletteBuffer2( //"buffer" name to be removed when properly integrated
    palette_id,
    palette_elements,
    _pixel_position,//desired_index_from_palette,  
    encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
    flag_map_scaling, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
    flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
    flag_crgb_exact_colour
    
  );

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
  //encoded value needs to be U32 since they can be 3 bytes wide
  uint8_t* encoded_value
  // ,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
  // bool     flag_map_scaling, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
  // bool     flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
  // bool     flag_crgb_exact_colour//,   // added by me, to make my effects work with CRGBPalette16
  // uint8_t  brightness_scale //255(default): No scaling, 0-255 scales the brightness of returned colour (remember all colours are saved in full 255 scale)
)
{

  RgbcctColor colour = RgbcctColor(0);

    // if(palette_id>mPaletteI->static_palettes.size())
    // {
    //   ALOG_ERR(PSTR("PALETTE EXCEEDS VECTOR SIZE")); delay(2000);
    //   return colour;
    // }

    // PALETTE *ptr = &mPaletteI->static_palettes[palette_id];

    // uint8_t pixels_in_map = GetNumberOfColoursInPalette(palette_id);  
    // uint8_t colour_width  = GetEncodedColourWidth(ptr->encoding); 

    uint8_t colour_width  = GetEncodedColourWidth(custom_palettes[custom_id].encoding); 
    uint8_t pixels_in_map = custom_palettes[custom_id].data.size()/colour_width;

    palette_buffer = &custom_palettes[custom_id].data[0];

    PALETTE_ENCODING_DATA encoding = custom_palettes[custom_id].encoding;
    

    // Serial.println(pixels_in_map);
    uint8_t remainder = 0;
    if(pixels_in_map != 0)
    {
      remainder = _pixel_position%pixels_in_map;  // /ERROR, REUSING NAME!!!
    }
    // uint8_t remainder_scaled255 = map(remainder, 0,pixels_in_segment-1, 0,255);

    // Reminder gives me each pixel, no gradient
    uint16_t pixel_position = remainder;   // THIS IS BEING PASSED IN AND REUSED??

    
    //something like this does need adding, but the previous and next pixels will be needed for the blend so perhaps make another if
    // if (flag_map_scaling) pixel_position = (_pixel_position*255)/(pCONT_lAni->_virtualSegmentLength -1);  

    // / Perhaps I want to make it, if it should wrap then load up to the first 16 pixels into the CRGBPalette16 gradient and then exact per the same method as below.



    uint16_t index_relative = 0; // get expected pixel position

    #ifdef ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    Serial.println(encoding.data, BIN);
    Serial.println(encoding.encoded_as_hsb_ids);
    Serial.println(encoding.index_scaled_to_segment);
    #endif // ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    
    // /**
    //   Encoded as HSBID, must handle all index methods
    // **/
    // if(
    //   (encoding.encoded_as_hsb_ids)
    // ){

    //   // Serial.println("MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX__ID"); 

    //   // Get Start of Colour Information by adjusting for indexing
    //   index_relative = pixel_position*GetEncodedColourWidth(encoding); // get expected pixel position
    //   if(encoding.index_scaled_to_segment)
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
      index_relative = pixel_position*GetEncodedColourWidth(encoding); // get expected pixel position
        
      // ALOG_INF(PSTR("index_relativeA=%d"),index_relative);

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




#endif // header gaurd
