#include "mPalette.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE

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
    
  /**************************************************************
   * 
   * "CRGBPALETTE16"
   * 
   * PALETTELIST_FIXED_CRGBPALETTE16__IDS
   * PALETTELIST_CRGBPALETTE16_GRADIENT___PALETTES__IDS
   * PALETTELIST_SEGMENTS_STORED_STATIC_CRGBPALETTE16_PALETTES__IDS
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
   * PALETTELIST_STATIC_SINGLE_COLOURS__IDS
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
   * PALETTELIST_FIXED__IDS
   * 
  ***************************************************************/
  else
  if(
    ((palette_id >= PALETTELIST_FIXED_COLOURFUL_DEFAULT__ID) && (palette_id < PALETTELIST_FIXED_LENGTH__ID))
  ){  

    uint16_t palette_adjusted_id = palette_id - PALETTELIST_FIXED_COLOURFUL_DEFAULT__ID; // adjust back into correct indexing

    colour = SubGet_Encoded_PaletteList_Colour_255(
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
        
    return SubGet_Encoded_UserPalette_Colour(
      palette_adjusted_id,
      palette_buffer,
      _pixel_position,  
      encoded_value
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


/*********************************************************************************************************************************************************************
 * *******************************************************************************************************************************************************************
 * *******************************************************************************************************************************************************************
 * *******************************************************************************************************************************************************************
 * *****************************************************************************************************************************************************************
 * *******************************************************************************************************************************************************************
 * *******************************************************************************************************************************************************************
 * *** SubGets Below ****************************************************************************************************************************************************************
 * *******************************************************************************************************************************************************************
 * *******************************************************************************************************************************************************************
 * *******************************************************************************************************************************************************************
 * *******************************************************************************************************************************************************************
 * *******************************************************************************************************************************************************************
 * *******************************************************************************************************************************************************************
 * *******************************************************************************************************************************************************************
 * *******************************************************************************************************************************************************************/



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
RgbcctColor mPalette::SubGet_Encoded_PaletteList_Colour_255(
  uint16_t palette_adjusted_id,
  uint8_t* palette_buffer,
  uint16_t _pixel_position,  
  uint8_t* encoded_value, // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
  bool     flag_spanned_segment, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
  bool     flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
  bool     flag_crgb_exact_colour,
  bool     flag_force_gradient
){

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

  RgbcctColor colour = RgbcctColor(0);

  if(palette_adjusted_id>mPaletteI->static_palettes.size())
  {
    ALOG_ERR(PSTR("PALETTE EXCEEDS VECTOR SIZE")); delay(2000);
    return colour;
  }

  /* indexing to loop around
  
  For 100 pixels, indexing request would be 0-99
  Normally this would be converted using modulo, so 5 colours in palette would be pixels%5  --> 0,1,2,3,4,0,1,2,3,4

  How can this be changed such that the SEGLEN gives the same
  So instead of 0-100, it is just 0-SEGLEN, the division should still apply

  */

  STATIC_PALETTE *ptr = &mPaletteI->static_palettes[palette_adjusted_id];

  uint8_t pixels_in_map = GetNumberOfColoursInPalette(palette_adjusted_id);  
  uint8_t colour_width  = GetEncodedColourWidth(ptr->encoding); 

  uint16_t pixel_position_adjust = _pixel_position;

  /******************************************************************************************************************************************************
   * *******************************************************************************************************************************************************************************
   * flag_crgb_exact_colour TYPES, no gradients
   * 
   * ******************************************************************************************************************************************************
   * ******************************************************************************************************************************************************************************* */


    bool is_forced_to_get_discrete = flag_crgb_exact_colour;
    bool is_not_gradient = (ptr->encoding.index_gradient == false);


  
  if(is_forced_to_get_discrete || is_not_gradient) // forced, or originally normal discrete
  {
    // 0-SEGLEN would need to become iterative of the palette    0-100    what if 0-100*pixels  so 0-500, then modulo by 100
    // o,1,2,3,4, 0,1,2,3,4, 0,1,2,3,4
    // but because of 255 range, these values should be scaled into 255
    //0-100 into 0-25500,    so 25500%100 would be 
    
    // pixel_position_adjust = (_pixel_position*255)/(pCONT_lAni->_virtualSegmentLength -1); 
    // pixel_position_adjust = map(pixel_position_adjust, 0,pCONT_lAni->SEGLEN, 0,pixels_in_map*pCONT_lAni->SEGLEN);//  pixels_in_map; // convert incoming pixels into repeating 0-15 numbers.


    
    if(flag_spanned_segment && !is_forced_to_get_discrete) // if incoming index is SEGWIDTH, then it needs converted first. Or, does it need converted back?
    {
      // pixel_position_adjust = (_pixel_position*255)/(pCONT_lAni->_virtualSegmentLength -1);  // This scales out segment_index to segment_length as 0 to 255
      // pixel_position_adjust = scale8(pixel_position_adjust, pixels_in_map); //gradient type when exact value is needed needs scaled into full range
    }
    
    uint16_t before = pixel_position_adjust;
    // if(not gradient)
    pixel_position_adjust %= pixels_in_map; // convert incoming pixels into repeating 0-15 numbers.
    // else // if gradient, then this same thing should happen but scale into 255 range
    ALOG_INF(PSTR("pixel_position_adjust %d/%d"), before, pixel_position_adjust);

    /**
      Colour below should be retrieved as pixel index or palette length (grad later)

      By here, indexs next to be converted into 0-colour_count
      This should therefore be where a subget calls, before this is conversions between methods.
    **/
    colour = Get_StaticPalette_Encoded_Colour_255Index(
      palette_adjusted_id, 
      palette_buffer, 
      pixel_position_adjust, 
      encoded_value
    );

    return colour;

  }

  /******************************************************************************************************************************************************
   * *******************************************************************************************************************************************************************************
   * flag_crgb_exact_colour TYPES, no gradients
   * ******************************************************************************************************************************************************
   * ******************************************************************************************************************************************************************************* */

  /**
   * @brief Convert incoming indes (0-SEGLEN) back into 255 range (0-255)
   * If it is mine, no matter what, convert into 255 range
   **/
  if(flag_spanned_segment)
  {
    pixel_position_adjust = (_pixel_position*255)/(pCONT_lAni->_virtualSegmentLength -1);  // This scales out segment_index to segment_length as 0 to 255
  }


  // forced spanned

  bool is_forced_to_be_gradient = flag_force_gradient;


  /**
   * @brief Construct a new if object
   * This section is forcing a gradient, but I need to want this. Or else it needs to be a gradient already.
   * 
   */
  if(ptr->encoding.index_gradient || is_forced_to_be_gradient) // if spanned, inside checks for gradient handling
  {
      
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

    uint8_t edge_size = pixels_in_map;
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
    if(ptr->encoding.index_gradient)
    {
      for(uint8_t pix_i=0; pix_i<pixels_in_map; pix_i++)
      {
        Get_StaticPalette_Encoded_Colour(palette_adjusted_id, palette_buffer, pix_i, &encoded_value2);
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

    RgbcctColor lower_colour = Get_StaticPalette_Encoded_Colour(palette_adjusted_id, palette_buffer, lower_boundary_i); 
    RgbcctColor upper_colour = Get_StaticPalette_Encoded_Colour(palette_adjusted_id, palette_buffer, upper_boundary_i);

    colour = RgbcctColor::LinearBlend(lower_colour, upper_colour, progress);

    // lower_colour.debug_print("lower_colour");
    // upper_colour.debug_print("upper_colour");
    // out.debug_print("  out_colour");
  }
  
  /**
   * @brief Not gradients
   * 
   */
  else{
    
      // ALOG_INF(PSTR("Get_StaticPalette_Encoded_Colour_255Index %d"), pixel_position_adjust);
      
    colour = Get_StaticPalette_Encoded_Colour_255Index(
      palette_adjusted_id, 
      palette_buffer, 
      pixel_position_adjust, 
      encoded_value
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
  Serial.println(encoding.index_gradient);
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

    if(encoding.index_gradient)
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

//////////////below sort

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
RgbcctColor mPalette::Get_StaticPalette_Encoded_Colour(
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
  Serial.println(ptr->encoding.index_gradient);
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
  //   if(ptr->encoding.index_gradient)
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

    if(ptr->encoding.index_gradient)
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
 * Future use of my code should always request colour from my palettes with 255 range
 * 
 * The colours in here should always be scaled into a gradient
 * 
 * @param palette_id 
 * @param seg_i 
 * @return RgbcctColor 
 */
RgbcctColor mPalette::Get_StaticPalette_Encoded_Colour_255Index(
  uint16_t palette_adjusted_id,
  uint8_t* palette_buffer,
  uint16_t colour_index,  
  uint8_t* encoded_value
){

  RgbcctColor colour = RgbcctColor(0);

  STATIC_PALETTE *ptr = &mPaletteI->static_palettes[palette_adjusted_id];

  uint8_t pixels_in_map = GetNumberOfColoursInPalette(palette_adjusted_id);  

  uint8_t palette_index = scale8(colour_index, pixels_in_map); //assumes original index is 0-255, and scales to 0-15

  // ALOG_INF(PSTR("pixel_position|palette_index %d->%d  %d %d"), colour_index, palette_index, palette_pixel_last_index, palette_index % 5);

  colour = Get_StaticPalette_Encoded_Colour(
    palette_adjusted_id,
    palette_buffer,
    palette_index,  
    encoded_value
  );

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
   * @brief Only "index_gradient" index is a gradient
   * 
   */
  if(ptr->encoding.index_gradient)
  {
    for(uint8_t pix_i=0; pix_i<pixels_in_map; pix_i++)
    {
      Get_StaticPalette_Encoded_Colour(palette_adjusted_id, palette_buffer, pix_i, &encoded_value2);
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

  RgbcctColor lower_colour = Get_StaticPalette_Encoded_Colour(palette_adjusted_id, palette_buffer, lower_boundary_i); 
  RgbcctColor upper_colour = Get_StaticPalette_Encoded_Colour(palette_adjusted_id, palette_buffer, upper_boundary_i);

  colour = RgbcctColor::LinearBlend(lower_colour, upper_colour, progress);

  // lower_colour.debug_print("lower_colour");
  // upper_colour.debug_print("upper_colour");
  // out.debug_print("  out_colour");
  
  return colour;

}





#endif // HEADERFILE_H_