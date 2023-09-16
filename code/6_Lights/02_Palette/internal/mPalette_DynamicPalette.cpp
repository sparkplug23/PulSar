#include "6_Lights/02_Palette/mPalette.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE

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
RgbcctColor mPalette::Get_Encoded_DynamicPalette_Colour(
  uint16_t palette_adjusted_id,
  uint8_t* palette_buffer,
  uint16_t _pixel_position,  
  uint8_t* encoded_value, // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
  bool     flag_spanned_segment, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
  bool     flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
  bool     flag_crgb_exact_colour,
  bool     flag_force_gradient
){

// Here, I likely want to work conversions, then call the other methods that are needed.
// For simplicity, the required colours will be requested from the static options that are in progmem.

// ALOG_INF(PSTR("HERE %d"), palette_adjusted_id);

  switch(palette_adjusted_id + PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID) //switch back to palette_NOT_adjusted_id
  { 
    case PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID:{

      float azimuth = pCONT_solar->solar_position.azimuth;

      float progress = mSupport::mapfloat(azimuth, 0,360, 0.0f, 1.0f);
      
      RgbcctColor colour1 = pSEGMENT_I(0).rgbcctcolors[0];
      RgbcctColor colour2 = pSEGMENT_I(0).rgbcctcolors[1];      

      Serial.println(azimuth);
      Serial.println(progress);

      return RgbcctColor::LinearBlend(colour1, colour2, progress);

    }
    break;
    
    /**
     * @brief Only around the +-10% of horizon should the transition occur. Anything above/below should remain at fully cold/warm white
     * 
     */
    case PALETTELIST_DYNAMIC__SOLAR_ELEVATION__WHITE_COLOUR_TEMPERATURE_01__ID:
    {
    
      float elevation = pCONT_solar->GetElevation();

      RgbcctColor colour_out = RgbcctColor(0);  

      float elevation_transition_degrees = 10; // degrees above/below horizon to transition between warm/cold white


      if(fabs(elevation) > elevation_transition_degrees) // get amplitude of elevation from horizon
      {

        if(elevation >= 0){ // daytime
          colour_out.setCCT(CCT_MIN_DEFAULT); // Cold White
          colour_out.setRGB(255,255,255);
          // ALOG_INF(PSTR("++++++ elevation %d"), (int)(elevation*100) );
        }else{ //nighttime
          colour_out.setCCT(CCT_MAX_DEFAULT); // Warm White
          colour_out.setRGB(0xFF,0x52,0x18);
          // ALOG_INF(PSTR("------ elevation %d"), (int)(elevation*100) );
        }
      }else{ // transitioning, dusk/dawn
        float progress = mSupport::mapfloat(
                                              elevation, 
                                              elevation_transition_degrees,    -1*elevation_transition_degrees, 
                                              0.0f,                            1.0f
                                            );

        RgbcctColor colour1 = RgbcctColor(0);
        colour1.setCCT(CCT_MIN_DEFAULT); // Cold White
        colour1.setRGB(255,255,255); // Cold White with RGB (Full)
        // colour1.setRGB(255,0,0); // Debugging

        RgbcctColor colour2 = RgbcctColor(0);
        colour2.setCCT(CCT_MAX_DEFAULT); // Warm White
        colour2.setRGB(0xFF,0x52,0x18);  // Warm White with RGB (White with reduced G,B)
        // colour2.setRGB(0,0,255); // Debugging

        colour_out = RgbcctColor::LinearBlend(colour1, colour2, progress);

      }
      Serial.println(elevation);
      colour_out.debug_print("colour_out");

      return colour_out;

       

      // Serial.println(elevation);
      // Serial.println(progress);
      
    }
    break;
    case PALETTELIST_DYNAMIC__SOLAR_ELEVATION__RGBCCT_PRIMARY_TO_SECONDARY_01__ID:
    {
        // 0 = peak day   ratio 1
        // 1 = sunset     ratio 0
        // 2 = peak night ratio 1



      // float elevation = pCONT_solar->solar_position.elevation;

      RgbcctColor colour1 = pSEGMENT_I(0).rgbcctcolors[0];
      RgbcctColor colour2 = pSEGMENT_I(0).rgbcctcolors[1]; 
      // RgbcctColor colour_out = pSEGMENT_I(0).rgbcctcolors[1];     

      // if(elevation > 0) //daytime
      // {
      //   colour1 = pSEGMENT_I(0).rgbcctcolors[0];
      //   colour2 = pSEGMENT_I(0).rgbcctcolors[1];  
      //   float progress = mSupport::mapfloat(elevation, 0, pCONT_solar->solar_position.elevation_max, 1.0f, 0.0f);
      //   colour_out = RgbcctColor::LinearBlend(colour1, colour2, progress);
      // }else{ //nighttime      
      //   colour1 = pSEGMENT_I(0).rgbcctcolors[1];
      //   colour2 = pSEGMENT_I(0).rgbcctcolors[2];  
      //   float progress = mSupport::mapfloat(elevation, 0, pCONT_solar->solar_position.elevation_min, 0.0f, 1.0f);
      //   colour_out = RgbcctColor::LinearBlend(colour1, colour2, progress);
      // }

      // return colour_out;


      
      float elevation = pCONT_solar->solar_position.elevation;

      RgbcctColor colour_out = RgbcctColor(0);  

      float elevation_transition_degrees = 10; // degrees above/below horizon to transition between warm/cold white


      if(fabs(elevation) > elevation_transition_degrees) // get amplitude of elevation from horizon
      {

        if(elevation >= 0){ // daytime
          colour_out = colour1;
          // ALOG_INF(PSTR("++++++ elevation %d"), (int)(elevation*100) );
        }else{ //nighttime
          colour_out = colour2;
          // ALOG_INF(PSTR("------ elevation %d"), (int)(elevation*100) );
        }
      }else{ // transitioning, dusk/dawn
        float progress = mSupport::mapfloat(
                                              elevation, 
                                              elevation_transition_degrees,    -1*elevation_transition_degrees, 
                                              0.0f,                            1.0f
                                            );

        colour_out = RgbcctColor::LinearBlend(colour1, colour2, progress);

      }
      Serial.println(elevation);
      colour_out.debug_print("colour_out");

      return colour_out;

       

      // Serial.println(elevation);
      // Serial.println(progress);
      
    }
    break;
    case PALETTELIST_DYNAMIC__TIMEREACTIVE__RGBCCT_PRIMARY_TO_SECONDARY_WITH_SECONDS_IN_MINUTE_01__ID:
    {

      float progress = mSupport::mapfloat(pCONT_time->RtcTime.second, 0,59, 0.0f, 1.0f);
      Serial.println(progress);

      RgbcctColor colour1 = pSEGMENT_I(0).rgbcctcolors[0];
      RgbcctColor colour2 = pSEGMENT_I(0).rgbcctcolors[1];      

      return RgbcctColor::LinearBlend(colour1, colour2, progress);

    }
    break;
  }

  return RgbcctColor(10);

}


#endif // HEADERFILE_H_