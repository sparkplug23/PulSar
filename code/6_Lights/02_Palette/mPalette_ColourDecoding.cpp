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


/*********************************************************************************************************************************************************************
 * *******************************************************************************************************************************************************************
 * *******************************************************************************************************************************************************************
 * *******************************************************************************************************************************************************************
 * *******************************************************************************************************************************************************************
 * *******************************************************************************************************************************************************************
 * *** SubGets Below *************************************************************************************************************************************************
 * *******************************************************************************************************************************************************************
 * *******************************************************************************************************************************************************************
 * *******************************************************************************************************************************************************************
 * *******************************************************************************************************************************************************************
 * *******************************************************************************************************************************************************************
 * *******************************************************************************************************************************************************************/







#endif // HEADERFILE_H_