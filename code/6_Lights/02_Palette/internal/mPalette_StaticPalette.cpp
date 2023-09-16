#include "6_Lights/02_Palette/mPalette.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE


uint8_t mPalette::GetColoursInCRGB16Palette(uint16_t palette_id)
{

  if(
    ((palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID) && (palette_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID))
  ){  
    uint16_t palette_id_adj = palette_id - mPalette::PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID;
    // pCONT_sup->GetTextIndexed_P(buffer, buflen, palette_id_adj, PM_STATIC_CRGBPALETTE16_NAMES_CTR);   
    // ALOG_DBG( PSTR("BName id%d|a%d \"%s\""), palette_id,palette_id_adj, buffer );

    uint8_t colour_count = pSEGMENT_I(0).palette_container->CRGB16Palette16_Palette.encoded_index.size();

    // ALOG_INF(PSTR("colour_count=%d, pal%d"), colour_count, palette_id);

    return colour_count;

  }


  if(
    ((palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID) && (palette_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID))
  ){  
    uint16_t palette_id_adj = palette_id - mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID;
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
RgbcctColor mPalette::Get_Encoded_StaticPalette_Colour(
  uint16_t palette_adjusted_id,
  uint8_t* palette_buffer,
  uint16_t _pixel_position,  
  uint8_t* encoded_value, // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
  bool     flag_spanned_segment, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
  bool     flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
  bool     flag_crgb_exact_colour,
  bool     flag_force_gradient
){


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

  RgbcctColor colour = RgbcctColor(0);
  if(palette_adjusted_id>mPaletteI->static_palettes.size())
  {
    ALOG_ERR(PSTR("PALETTE EXCEEDS VECTOR SIZE"));
    return colour;
  }

  STATIC_PALETTE *ptr = &mPaletteI->static_palettes[palette_adjusted_id];
  


  uint8_t pixels_in_map = GetNumberOfColoursInPalette(palette_adjusted_id);  
  uint8_t colour_width  = GetEncodedColourWidth(ptr->encoding); 

  uint16_t pixel_position_adjust = _pixel_position;

  /******************************************************************************************************************************************************
   * *******************************************************************************************************************************************************************************
   * 
   * DISCRETE SEQUENCE, is_basic_sequence_palette_with_increasing_indexing
   * 
   * ******************************************************************************************************************************************************
   * ******************************************************************************************************************************************************************************* */
  bool is_forced_to_get_discrete = flag_crgb_exact_colour;
  bool is_not_gradient = (ptr->encoding.index_gradient == false);
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
    pixel_position_adjust %= pixels_in_map; // convert incoming pixels into repeating 0-15 numbers.
    // else // if gradient, then this same thing should happen but scale into 255 range
    // ALOG_INF(PSTR("pixel_position_adjust %d/%d"), before, pixel_position_adjust);

    /**
      Colour below should be retrieved as pixel index or palette length (grad later)

      By here, indexs next to be converted into 0-colour_count
      This should therefore be where a subget calls, before this is conversions between methods.
    **/
    colour = Get_StaticPalette_Encoded_Colour_IterativeIndex(
      palette_adjusted_id, 
      palette_buffer, 
      pixel_position_adjust, 
      encoded_value
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
  if(ptr->encoding.index_gradient || flag_force_gradient) // if spanned, inside checks for gradient handling
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
        Get_StaticPalette_Encoded_Colour_ReadBuffer(palette_adjusted_id, palette_buffer, pix_i, &encoded_value2);
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

    RgbcctColor lower_colour = Get_StaticPalette_Encoded_Colour_ReadBuffer(palette_adjusted_id, palette_buffer, lower_boundary_i); 
    RgbcctColor upper_colour = Get_StaticPalette_Encoded_Colour_ReadBuffer(palette_adjusted_id, palette_buffer, upper_boundary_i);

    colour = RgbcctColor::LinearBlend(lower_colour, upper_colour, progress);

    // lower_colour.debug_print("lower_colour");
    // upper_colour.debug_print("upper_colour");
    // out.debug_print("  out_colour");

    return colour;

  }

  
  /******************************************************************************************************************************************************
   * *******************************************************************************************************************************************************************************
   * 
   * Not a gradient palette, but needs to be spanned => Just show across the span without blending
   * 
   * ******************************************************************************************************************************************************
   * ******************************************************************************************************************************************************************************* */

  // ALOG_INF(PSTR("Get_StaticPalette_Encoded_Colour_255Index %d"), pixel_position_adjust);

  // Serial.println("What is this for then? It was either discrete, or gradient, so what is this?");
      
  if(flag_spanned_segment)
  {
    pixel_position_adjust = (_pixel_position*255)/(pCONT_lAni->_virtualSegmentLength -1);  // This scales out segment_index to segment_length as 0 to 255
  }

  colour = Get_StaticPalette_Encoded_Colour_255Index(
    palette_adjusted_id, 
    palette_buffer, 
    pixel_position_adjust, 
    encoded_value
  );


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
RgbcctColor mPalette::Get_StaticPalette_Encoded_Colour_ReadBuffer(
  uint16_t palette_adjusted_id,
  uint8_t* palette_buffer,
  uint16_t _pixel_position,  
  uint8_t* encoded_value
){

  RgbcctColor colour = RgbcctColor(0);

  if(palette_adjusted_id>mPaletteI->static_palettes.size())
  {
    ALOG_ERR(PSTR("PALETTE EXCEEDS VECTOR SIZE"));
    return colour;
  }

  STATIC_PALETTE *ptr = &mPaletteI->static_palettes[palette_adjusted_id];

  uint8_t pixels_in_map = GetNumberOfColoursInPalette(palette_adjusted_id);  
  uint8_t colour_width  = GetEncodedColourWidth(ptr->encoding); 

  uint16_t pixel_position = _pixel_position;
  uint16_t index_relative = 0; // get expected pixel position

  #ifdef ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
  Serial.println(ptr->encoding.data, BIN);
  Serial.println(ptr->encoding.encoded_as_hsb_ids);
  Serial.println(ptr->encoding.index_gradient);
  #endif // ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
  

  if( // Switch to bit masking
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

  colour = Get_StaticPalette_Encoded_Colour_ReadBuffer(
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
 * Future use of my code should always request colour from my palettes with 255 range
 * 
 * The colours in here should always be scaled into a gradient
 * 
 * @param palette_id 
 * @param seg_i 
 * @return RgbcctColor 
 */
RgbcctColor mPalette::Get_StaticPalette_Encoded_Colour_IterativeIndex(
  uint16_t palette_adjusted_id,
  uint8_t* palette_buffer,
  uint16_t colour_index,  
  uint8_t* encoded_value
){

  RgbcctColor colour = RgbcctColor(0);

  STATIC_PALETTE *ptr = &mPaletteI->static_palettes[palette_adjusted_id];

  uint8_t pixels_in_map = GetNumberOfColoursInPalette(palette_adjusted_id);  

  uint8_t palette_index = colour_index;// scale8(colour_index, pixels_in_map); //assumes original index is 0-255, and scales to 0-15

  // ALOG_INF(PSTR("pixel_position|palette_index %d->%d  %d %d"), colour_index, palette_index, palette_pixel_last_index, palette_index % 5);

  colour = Get_StaticPalette_Encoded_Colour_ReadBuffer(
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
      Get_StaticPalette_Encoded_Colour_ReadBuffer(palette_adjusted_id, palette_buffer, pix_i, &encoded_value2);
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

  RgbcctColor lower_colour = Get_StaticPalette_Encoded_Colour_ReadBuffer(palette_adjusted_id, palette_buffer, lower_boundary_i); 
  RgbcctColor upper_colour = Get_StaticPalette_Encoded_Colour_ReadBuffer(palette_adjusted_id, palette_buffer, upper_boundary_i);

  colour = RgbcctColor::LinearBlend(lower_colour, upper_colour, progress);

  // lower_colour.debug_print("lower_colour");
  // upper_colour.debug_print("upper_colour");
  // out.debug_print("  out_colour");
  
  return colour;

}








#endif // USE_MODULE_LIGHTS_INTERFACE