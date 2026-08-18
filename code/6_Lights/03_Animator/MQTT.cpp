#include "_AnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR



/******************************************************************************************************************
 * mInterfaceLight_ConstructJSON.cpp
*******************************************************************************************************************/



uint8_t mAnimatorLight::ConstructJSON_Settings(uint8_t json_level, bool json_appending)
{

  JBI->Start();

    // JBI->Add("light_size_count", settings.light_size_count);

    // JBI->Add("BriRGB_Global",  getBriRGB_Global());
    // JBI->Add("BriCCT_Global",  getBriCCT_Global());

  return JBI->End();

}

/**
 * @brief Unlike debug_segments, only the useful info here
 * 
 * @param json_level 
 * @param json_appending 
 * @return uint8_t 
 */
uint8_t mAnimatorLight::ConstructJSON_Segments(uint8_t json_level, bool json_appending)
{

  char buffer[120];

  JBI->Start();


    JBI->Add("MinShowDelay", MIN_SHOW_DELAY);

    uint8_t seg_count = getSegmentsNum();

    JBI->Add("SegmentCount", seg_count);


    // JBI->Add("millis", millis());

    // for(uint8_t seg_i =0; seg_i < getSegmentsNum(); seg_i++)
    // {
    //   JBI->Add("Start", SEGMENT_I(seg_i).start);
    //   JBI->Add("Stop",  SEGMENT_I(seg_i).stop);
    //   JBI->Add("StartY", SEGMENT_I(seg_i).startY);
    //   JBI->Add("StopY",  SEGMENT_I(seg_i).stopY);
    //   JBI->Add("EffectMicros",   SEGMENT_I(seg_i).performance.effect_build_us);

    // }




    JBI->Add("Brightness_Master",    tkr_iLight->getBri_Global());
    JBI->Add("BrightnessRGB_Master", tkr_iLight->getBriRGB_Global());
    JBI->Add("BrightnessCCT_Master", tkr_iLight->getBriCCT_Global());

  JBI->Add("FPS", getFps());

    seg_count = seg_count < 4 ? seg_count : 4; //limit memory overrun, or else later instead of reducing the seg count, reduce the data shared in another topic as overview

    for(uint8_t seg_i =0; seg_i < seg_count; seg_i++)
    {

      JBI->Object_Start_F("Segment%d", seg_i);

        // JBI->Add("BrightnessRGB", SEGMENT_I(seg_i).getBrightnessRGB());
        // JBI->Add("BrightnessCCT", SEGMENT_I(seg_i).getBrightnessCCT());
        
        // JBI->Add("BrightnessRGB_wMaster", SEGMENT_I(seg_i).getBrightnessRGB_WithGlobalApplied());
        // JBI->Add("BrightnessCCT_wMaster", SEGMENT_I(seg_i).getBrightnessCCT_WithGlobalApplied());
        
        JBI->Array_Start("PixelRange");
          JBI->Add(SEGMENT_I(seg_i).start);
          JBI->Add(SEGMENT_I(seg_i).stop);
          JBI->Add(SEGMENT_I(seg_i).startY);
          JBI->Add(SEGMENT_I(seg_i).stopY);
        JBI->Array_End();
        JBI->Add("Effect",    SEGMENT_I(seg_i).effect_id);
        JBI->Add("EffectName",    GetFlasherFunctionNamebyID( SEGMENT_I(seg_i).effect_id , buffer, sizeof(buffer), true) );
        JBI->Add("Offset",    SEGMENT_I(seg_i).offset);
        JBI->Add("Speed",     SEGMENT_I(seg_i).speed);
        JBI->Add("Intensity", SEGMENT_I(seg_i).intensity);
        JBI->Object_Start("Options");
          JBI->Add("Selected",     SEGMENT_I(seg_i).selected);
          JBI->Add("Reverse",      SEGMENT_I(seg_i).reverse);
          JBI->Add("On",           SEGMENT_I(seg_i).on);
          JBI->Add("Mirror",       SEGMENT_I(seg_i).mirror);
          JBI->Add("Freeze",       SEGMENT_I(seg_i).freeze);
          JBI->Add("Spacing",       SEGMENT_I(seg_i).spacing);
          JBI->Add("Grouping",       SEGMENT_I(seg_i).grouping);
          JBI->Add("Decimate",       SEGMENT_I(seg_i).decimate);
          if(seg_i<3)
          {
            JBI->Add("Reset",        SEGMENT_I(seg_i).reset);
            JBI->Add("Transitional", SEGMENT_I(seg_i).transitional);
            JBI->Add("Reverse_y",    SEGMENT_I(seg_i).reverse_y);
            JBI->Add("Mirror_y",     SEGMENT_I(seg_i).mirror_y);
            JBI->Add("Transpose",    SEGMENT_I(seg_i).transpose);
            JBI->Add("Map1D2D",      SEGMENT_I(seg_i).map1D2D);
            JBI->Add("SoundSim",     SEGMENT_I(seg_i).soundSim);
          }
        JBI->Object_End();
        JBI->Add("ColourType",     (uint8_t)SEGMENT_I(seg_i).colour_width__used_in_effect_generate);
        JBI->Object_Start("Transition");
          JBI->Add("Rate",         SEGMENT_I(seg_i).cycle_time__rate_ms);
          JBI->Add("Time",         SEGMENT_I(seg_i).animator_blend_time_ms() );
        JBI->Object_End();
        // JBI->Object_Start("RgbcctColours");
        // for(uint8_t rgb_i = 0; rgb_i<2; rgb_i++)
        // {
        //   JBI->Array_Start_P("Colour%d", rgb_i);
        //   for(uint8_t c_i=0;c_i<5;c_i++)
        //   {
        //     // JBI->Add(SEGMENT_I(seg_i).segcol[rgb_i].raw[c_i]);
        //   }
           


        //   JBI->Array_End();
        //   JBI->Object_Start("ColourTemp");
        //     // JBI->Add("Min",      SEGMENT_I(seg_i).segcol[rgb_i].get_CTRangeMin());
        //     // JBI->Add("Max",      SEGMENT_I(seg_i).segcol[rgb_i].get_CTRangeMax());
        //     // JBI->Add("Set",      SEGMENT_I(seg_i).segcol[rgb_i].getCCT());
        //   JBI->Object_End();
        // }
        // JBI->Object_End();

        // for(uint8_t seg_col = 0; seg_col < 5; seg_col++)
        //   {
        //     JBI->Array_Start_P(PSTR("SegColour%d"), seg_col);
        //       // for(uint8_t p=0;p<5;p++)
        //       // { 
        //         JBI->Add(segments[seg_i].segcol[seg_col].colour.R); 
        //         JBI->Add(segments[seg_i].segcol[seg_col].colour.G); 
        //         JBI->Add(segments[seg_i].segcol[seg_col].colour.B); 
        //         JBI->Add(segments[seg_i].segcol[seg_col].colour.WW); 
        //         JBI->Add(segments[seg_i].segcol[seg_col].colour.CW); 
        //       // }
        //     JBI->Array_End();
        //   }
        for(uint8_t seg_col = 0; seg_col < 5; seg_col++)
        {
          JBI->Array_Start_P(PSTR("SegBrtRGB%d"), seg_col);
            for(uint8_t p=0;p<5;p++)
            { 
              // JBI->Add(segments[seg_i].segcol[seg_col].bri_rgb); 
            }
          JBI->Array_End();
        }
          
      JBI->Object_End();

    } // END seg_i





  return JBI->End();

}

/**
 * @brief Unlike debug_segments, only the useful info here
 * 
 * @param json_level 
 * @param json_appending 
 * @return uint8_t 
 */
#ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS  
uint8_t mAnimatorLight::ConstructJSON_Matrix(uint8_t json_level, bool json_appending)
{

  JBI->Start();

    JBI->Add("isMatrix", isMatrix);
    JBI->Add("panels", panels);

    // for(uint8_t seg_i =0; seg_i < getSegmentsNum(); seg_i++)
    // {
    //   JBI->Add("Start", SEGMENT_I(seg_i).start);
    //   JBI->Add("Stop",  SEGMENT_I(seg_i).stop);
    //   JBI->Add("EffectMicros",   SEGMENT_I(seg_i).performance.effect_build_us);

    // }

    uint8_t i = 0;
    JBI->Object_Start_F("Panel");
    for (Panel p : panel)
    {
      JBI->Object_Start_F("%d", i++);
        JBI->Add("Width", p.width);
        JBI->Add("Height", p.height);
        JBI->Add("xOffset", p.xOffset);
        JBI->Add("yOffset", p.yOffset);
        JBI->Add("bottomStart", p.bottomStart);
        JBI->Add("rightStart", p.rightStart);
        JBI->Add("vertical", p.vertical);
        JBI->Add("serpentine", p.serpentine);
      JBI->Object_End();
    }
    JBI->Object_End();







  return JBI->End();

}
#endif // ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS  



uint8_t mAnimatorLight::ConstructJSON_Playlist(uint8_t json_level, bool json_appending)
{

  JBI->Start();
  
    #ifdef ENABLE_FEATURE_LIGHTS__PLAYLISTS
    JBI->Add("Length", playlistLen);

    JBI->Array_Start("Loaded");
    for(int i=0;i<playlistLen;i++)
    {
      JBI->Object_Start();
        JBI->Add("preset",  playlistEntries[i].preset);
        JBI->Add("dur",     playlistEntries[i].dur);
        JBI->Add("tr",      playlistEntries[i].tr);
      JBI->Object_End();
    }
    JBI->Array_End();
    #endif 




    JBI->Add("millis", millis());

  return JBI->End();

}



#ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
uint8_t mAnimatorLight::ConstructJSON_Mode_Ambilight(uint8_t json_level, bool json_appending)
{

  JBI->Start();

    JBI->Add("millis", millis());

  return JBI->End();

}
#endif


#ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
uint8_t mAnimatorLight::ConstructJSON_Mode_SetManual(uint8_t json_level, bool json_appending)
{

JBI->Start();

  JBI->Add("millis", millis());

return JBI->End();

}
#endif


/**
 * @brief Debug 
 */
#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
uint8_t mAnimatorLight::ConstructJSON_Debug_Palette(uint8_t json_level, bool json_appending)
{

char buffer[100];

JBI->Start();

    JBI->Add("AvailablePalettes", (uint16_t)mPaletteI->GetPaletteListLength() );

    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    for(uint8_t seg_i = 0; seg_i<  segments.size(); seg_i++)
    {
      JBI->Object_Start_F("Segment%d",seg_i);      
        JBI->Add("dataLen", SEGMENT_I(seg_i).palette_loaded->pData.size());
        JBI->Array_Start("data");
        for(uint8_t i=0;i<SEGMENT_I(seg_i).palette_loaded->pData.size();i++)
        {
          JBI->Add(SEGMENT_I(seg_i).palette_loaded->pData[i]);
        }
        JBI->Array_End();
      JBI->Object_End();
    }
    #endif// ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER

    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE
    uint8_t seg_i = 0;

    if(segments.size())
    {

      JBI->Array_Start("CRGB16Palette16");   

      for(uint8_t elem_i=0;elem_i<16;elem_i++)
      {
        JBI->Array_Start();
          JBI->Add( SEGMENT_I(seg_i).palette_loaded->CRGB16Palette16_Palette.data[elem_i].r );
          JBI->Add( SEGMENT_I(seg_i).palette_loaded->CRGB16Palette16_Palette.data[elem_i].g );
          JBI->Add( SEGMENT_I(seg_i).palette_loaded->CRGB16Palette16_Palette.data[elem_i].b );
        JBI->Array_End();          
      }
      JBI->Array_End();

      JBI->Array_Start("CRGB16Palette16MAN");
      for(uint8_t elem_i=0;elem_i<16;elem_i++)
      {
        JBI->Array_Start();
          JBI->Add( SEGMENT_I(seg_i).palette_loaded->CRGB16Palette16_Palette.data[elem_i].r );
          JBI->Add( SEGMENT_I(seg_i).palette_loaded->CRGB16Palette16_Palette.data[elem_i].g );
          JBI->Add( SEGMENT_I(seg_i).palette_loaded->CRGB16Palette16_Palette.data[elem_i].b );
        JBI->Array_End();          
      }
      JBI->Array_End();

    }
    #endif// ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE


    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    JBI->Object_Start("Encoding");
      for(uint8_t palette_id=mPalette::PALETTELIST_STATIC_HOLLOWEEN_OP__ID;palette_id<mPaletteI->PALETTELIST_STATIC_CHRISTMAS_01__ID;palette_id++)
      {
        // JBI->Array_Start_P("%s", mPaletteI->GetPaletteNameByID( palette_id, buffer, sizeof(buffer) ));
        JBI->Array_Start_P("P_%d", palette_id );        
          mPalette::PALETTELIST::PALETTE *ptr = mPaletteI->GetPalettePointerByID(palette_id);
          uint16_t value = ptr->encoding.data;
          char buffer[33] = {0}; //null terminated
          for(uint8_t i=0;i<16;i++)
          {
            buffer[15-i] = bitRead(value,i) ? '1' : '0';
          }
          JBI->Add(buffer);
        JBI->Array_End();
      }
    JBI->Object_End();
    #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING


    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    JBI->Array_Start_P("DataLength");
      for(uint8_t palette_id=mPalette::PALETTELIST_VARIABLE_HSBID_01__ID;palette_id<mPaletteI->PALETTELIST_STATIC_CHRISTMAS_28__ID;palette_id++)
      {
        mPalette::PALETTELIST::PALETTE *ptr = mPaletteI->GetPalettePointerByID(palette_id);
        JBI->Add(ptr->data_length);
      }
    JBI->Array_End();
    #endif  // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH

    // #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
    // JBI->Array_Start_P("PaletteNames");
    //   for(uint8_t palette_id=mPalette::PALETTELIST_VARIABLE_HSBID_01__ID;palette_id<mPaletteI->PALETTELIST_VARIABLE_GENERIC_LENGTH__ID;palette_id++) // Some
    //   // for(uint16_t palette_id=mPalette::PALETTELIST_VARIABLE_HSBID_01__ID;palette_id<mPaletteI->PALETTELIST_VARIABLE_GENERIC_LENGTH__ID;palette_id++) // All
    //   {
    //     JBI->Add(GetPaletteNameByID(palette_id, buffer, sizeof(buffer)));
    //     // ALOG_INF(PSTR("Name[%d] = %s"),palette_id,GetPaletteNameByID(palette_id, buffer, sizeof(buffer)));
    //   }
    // JBI->Array_End();
    // #endif  // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
    JBI->Array_Start_P("PaletteNames");
      for(uint16_t palette_id=mPalette::PALETTELIST_VARIABLE_HSBID_01__ID;palette_id<mPaletteI->PALETTELIST_STATIC_CHRISTMAS_28__ID;palette_id++) // Some
      // for(uint8_t palette_id=mPalette::PALETTELIST_VARIABLE_HSBID_01__ID;palette_id<mPaletteI->PALETTELIST_TOTAL_LENGTH;palette_id++) // All
      {
        // Option 
        // ALOG_INF(PSTR("Name[%d] = %s"),GetPaletteNameByID(palette_id, buffer, sizeof(buffer)));

        // JBI->Add(GetPaletteNameByID(palette_id, buffer, sizeof(buffer)));
      }
    JBI->Array_End();
    #endif  // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST


    uint16_t id = 0;
    uint16_t pixel = 0;
    uint8_t encoded_value = 0;
    
    // // uint16_t count   = mPaletteI->GetLengthFromPaletteAdvanced(id,pixel,&encoded_value,true,true,255);
    // RgbcctColor colour = mPaletteI->GetColourFromPaletteAdvanced(id,pixel,&encoded_value,true,true,255);

    // JBI->Array_Start("Palette");
    // for(int i=0;i<MAX_NUM_SEGMENTS;i++)
    // {
    //   JBI->Add(encoded_value);
    //   JBI->Add(colour.R);
    //   JBI->Add(colour.G);
    //   JBI->Add(colour.B);
    //   JBI->Add(colour.W1);
    //   JBI->Add(colour.W2);
    // }
    // JBI->Array_End();
    
//   JBI->Start();  
//     JBI->Add_P(PM_SIZE, tkr_iLight->settings.light_size_count);
//     JBI->Add("PaletteMaxID", (uint8_t)mPalette::PALETTELIST_STATIC_LENGTH__ID);
//     JBI->Add("ColourPaletteID", tkr_anim->SEGMENT_I(0).palette_id );
//     JBI->Add("ColourPalette", mPaletteI->GetPaletteNameByID( SEGMENT_I(0).palette_id, buffer, sizeof(buffer)));
//     // JBI->Array_Start("rgb");
//     // for(int i=0;i<numpixels;i++){
//     //   RgbTypeColor c = getPixelColor(i);
//     //   JBI->Add_FV(PSTR("%02X%02X%02X"),c.R,c.G,c.B);
//     // }
//     // JBI->Array_End();
//   return JBI->End();


    /**
     * @brief Moving towards preloading palettes from memory into ram/heap for speed (then iram will work)
     * 
     */
    JBI->Object_Start("LoadPalette");

      uint8_t segment_index = 0;

    JBI->Object_End();
    

return JBI->End();

}

#endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE


#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_CUSTOM_MAPPING_TABLE
uint8_t mAnimatorLight::ConstructJSON_Debug__CustomMappingTable(uint8_t json_level, bool json_appending)
{
  JBI->Start();

  JBI->Add("pixleng", _pixels_length);
  JBI->Add("pixleng2", getLengthTotal());

    JBI->Add("customMappingSize",    customMappingSize);

    if(customMappingSize)
    {
      JBI->Array_Start("MappingTable");
      uint16_t send_size = customMappingSize < 400 ? customMappingSize : 400;
      for(int i=0;i<send_size;i++)
      {
        JBI->Add(customMappingTable[i]);
      }
      JBI->Array_End();
    }

  return JBI->End();
}
#endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_CUSTOM_MAPPING_TABLE


#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS

/**
 * @brief Multiple large segments may require mutiple topics
 */
uint8_t mAnimatorLight::ConstructJSON_Debug_Segments(uint8_t json_level, bool json_appending)
{
  JBI->Start();

  JBI->Add("Brightness",    tkr_iLight->getBri_Global());
  JBI->Add("BrightnessRGB", tkr_iLight->getBriRGB_Global());
  JBI->Add("BrightnessCCT", tkr_iLight->getBriCCT_Global());
  
  uint8_t seg_count = getSegmentsNum();
  seg_count = seg_count < 3 ? seg_count : 3; //limit memory overrun, or else later instead of reducing the seg count, reduce the data shared in another topic as overview

  for(uint8_t seg_i =0; seg_i < seg_count; seg_i++)
  {

    JBI->Object_Start_F("Segment%d", seg_i);

      // JBI->Add("BrightnessRGB", SEGMENT_I(seg_i).getBrightnessRGB());
      // JBI->Add("BrightnessCCT", SEGMENT_I(seg_i).getBrightnessCCT());
      
      // JBI->Add("BrightnessRGB_wMaster", SEGMENT_I(seg_i).getBrightnessRGB_WithGlobalApplied());
      // JBI->Add("BrightnessCCT_wMaster", SEGMENT_I(seg_i).getBrightnessCCT_WithGlobalApplied());
      
      JBI->Array_Start("PixelRange");
        JBI->Add(SEGMENT_I(seg_i).start);
        JBI->Add(SEGMENT_I(seg_i).stop);
        JBI->Add(SEGMENT_I(seg_i).startY);
        JBI->Add(SEGMENT_I(seg_i).stopY);
      JBI->Array_End();
      JBI->Add("Offset",    SEGMENT_I(seg_i).offset);
      JBI->Add("Speed",     SEGMENT_I(seg_i).speed);
      JBI->Add("Intensity", SEGMENT_I(seg_i).intensity);
      JBI->Object_Start("Options");
        JBI->Add("Selected",     SEGMENT_I(seg_i).selected);
        JBI->Add("Reverse",      SEGMENT_I(seg_i).reverse);
        JBI->Add("On",           SEGMENT_I(seg_i).on);
        JBI->Add("Mirror",       SEGMENT_I(seg_i).mirror);
        JBI->Add("Freeze",       SEGMENT_I(seg_i).freeze);
        JBI->Add("Reset",        SEGMENT_I(seg_i).reset);
        JBI->Add("Transitional", SEGMENT_I(seg_i).transitional);
        JBI->Add("Reverse_y",    SEGMENT_I(seg_i).reverse_y);
        JBI->Add("Mirror_y",     SEGMENT_I(seg_i).mirror_y);
        JBI->Add("Transpose",    SEGMENT_I(seg_i).transpose);
        JBI->Add("Map1D2D",      SEGMENT_I(seg_i).map1D2D);
        JBI->Add("SoundSim",     SEGMENT_I(seg_i).soundSim);
      JBI->Object_End();
      JBI->Add("ColourType",     (uint8_t)SEGMENT_I(seg_i).colour_width__used_in_effect_generate);
      JBI->Object_Start("Transition");
        JBI->Add("Rate",         SEGMENT_I(seg_i).cycle_time__rate_ms);
        JBI->Add("Time",         SEGMENT_I(seg_i).animator_blend_time_ms() );
      JBI->Object_End();
      JBI->Object_Start("SegmentColours");
      for(uint8_t rgb_i = 0; rgb_i<2; rgb_i++)
      {
        JBI->Array_Start_P("Colour%d", rgb_i);
        for(uint8_t c_i=0;c_i<5;c_i++)
        {
          // JBI->Add(SEGMENT_I(seg_i).segcol[rgb_i].raw[c_i]);
        }
        JBI->Array_End();
        JBI->Object_Start("ColourTemp");
          // JBI->Add("Min",      SEGMENT_I(seg_i).segcol[rgb_i].get_CTRangeMin());
          // JBI->Add("Max",      SEGMENT_I(seg_i).segcol[rgb_i].get_CTRangeMax());
          // JBI->Add("Set",      SEGMENT_I(seg_i).segcol[rgb_i].getCCT());
        JBI->Object_End();
      }
      JBI->Object_End();
        
    JBI->Object_End();

  } // END seg_i

  return JBI->End();

}

#endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS


#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR

/**
 * @brief Multiple large segments may require mutiple topics
 */
uint8_t mAnimatorLight::ConstructJSON_Debug_Palette_Vector(uint8_t json_level, bool json_appending)
{

  JBI->Start();
  JBI->Add("size", mPaletteI->static_palettes.size());


    char lineBuffer[100] = {0};
    bool flag_get_first_name_only = true;

    JBI->Array_Start("pals");
    // for(uint8_t i=0;i<20;i++)//auto& pal: mPaletteI->palettelist)

    uint16_t i = 0;

    for(auto& pal: mPaletteI->static_palettes)
    {
      // mPalette::PALETTE pal = mPaletteI->palettelist[i];
      JBI->Object_Start();
          
          // tkr_anim->GetPaletteNameByID(i, lineBuffer, sizeof(lineBuffer));
          // // snprintf_P(lineBuffer,sizeof(lineBuffer),"%S",pal.friendly_name_ctr); 
          // if(flag_get_first_name_only)
          // {    
          //   char* dataPtr = strchr(lineBuffer,'|');
          //   if (dataPtr) *dataPtr = 0; // replace name dividor with null termination early
          // }
          // // ALOG_INF(PSTR("pal=\"%s\""), lineBuffer);
          // // ALOG_INF(PSTR("pal[%d]=\"%s\""), i, lineBuffer);
          // if(i<10)
          //   JBI->Add("n",lineBuffer);



        // JBI->Add("n", pal.friendly_name_ctr);
        // JBI->Add("i", pal.id);
        
        // uint8_t colours_in_palette = tkr_anim->GetNumberOfColoursInUNLOADEDPalette(i);
        // JBI->Add("s",colours_in_palette);



    
        // JBI->Add_FV("e", "%4X", pal.encoding);
        // JBI->Array_Start("d");
        //   for(uint8_t i=0;i<pal.data_length;i++)
        //   {
        //     JBI->Add(pal.data[i]);
        //   }
        // JBI->Array_End();
      JBI->Object_End();

      i++;
    }
    JBI->Array_End();

  return JBI->End();

}

#endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR



#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PERFORMANCE
uint8_t mAnimatorLight::ConstructJSON_Debug_Performance(uint8_t json_level, bool json_appending)
{
  
  #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PERFORMANCE_FAST_MQTT_UPDATE
<<<<<<< HEAD
  mqtthandler_debug__performance.tRateSecs = 1; // Force update to 1 second
=======
  telemetry_debug__performance.tRateSecs = 1; // Force update to 1 second
>>>>>>> 5c7962ae6a38a6d065993ed30fd13d2e994344f6
  #endif

  JBI->Start();  

    JBI->Add("targetFPS", getTargetFps() );
    JBI->Add("FPS", getFps());
    
    #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PERFORMANCE_METRICS_SAFE_IN_RELEASE_MODE


    // Time: Generate Pixels
    // JBI->Array_Start_P("EffectCall_us");
    // for (segment &seg : segments){ JBI->Add(seg.performance.effect_build_us); }
    // JBI->Array_End();
    JBI->Array_Start_P("EffectCall_ms");
    for (segment &seg : segments){ JBI->Add(seg.performance.effect_build_us/1000); }
    JBI->Array_End();


    // Time: Bus Writing Single
    JBI->Array_Start_P("BusWrite_us");
    for (segment &seg : segments){ JBI->Add(seg.performance.bus_write_single_us); }
    JBI->Array_End();
    JBI->Array_Start_P("BusRead_us");
    for (segment &seg : segments){ JBI->Add(seg.performance.bus_read_single_us); }
    JBI->Array_End();

    // JBI->Array_Start_P("BusWrite_ms");
    // for (segment &seg : segments){ JBI->Add(seg.performance.bus_write_us/1000); }
    // JBI->Array_End();
    // JBI->Array_Start_P("BusRead_ms");
    // for (segment &seg : segments){ JBI->Add(seg.performance.bus_read_us/1000); }
    // JBI->Array_End();


    // Time: Bus Writing Complate (From Static Palette Effect)
    // JBI->Array_Start_P("BusWriteTotal_us");
    // for (segment &seg : segments){ JBI->Add(seg.performance.bus_write_total_us); }
    // JBI->Array_End();
    // JBI->Array_Start_P("BusReadTotal_us");
    // for (segment &seg : segments){ JBI->Add(seg.performance.bus_read_total_us); }
    // JBI->Array_End();

    JBI->Array_Start_P("BusWriteTotal_ms");
    for (segment &seg : segments){ JBI->Add(seg.performance.bus_write_total_us/1000); }
    JBI->Array_End();
    JBI->Array_Start_P("BusReadTotal_ms");
    for (segment &seg : segments){ JBI->Add(seg.performance.bus_read_total_us/1000); }
    JBI->Array_End();
    #endif

  
    #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PERFORMANCE_METRICS_SAFE_IN_RELEASE_MODE    
    JBI->Array_Start_P("elapsed_last_show");
    for (segment &seg : segments){ JBI->Add(seg.performance.elapsed_last_show); }
    JBI->Array_End();
    JBI->Array_Start_P("millis_last_show");
    for (segment &seg : segments){ JBI->Add(seg.performance.millis_last_show); }
    JBI->Array_End();
    JBI->Array_Start_P("fps");
    for (segment &seg : segments){ JBI->Add(seg.performance.fps); }
    JBI->Array_End();
    #endif 

    // Show allocated data from segments
    JBI->Array_Start_P("dataSize");
    for (segment &seg : segments){ JBI->Add(seg.dataSize()); }
    JBI->Array_End();

    JBI->Array_Start_P("seglen");
    for (segment &seg : segments){ JBI->Add(seg.virtualLength()); }
    JBI->Array_End();


  return JBI->End();

}

#endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PERFORMANCE



/******************************************************************************************************************
 * mInterfaceLight_MQTT.cpp
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

<<<<<<< HEAD
void mAnimatorLight::MQTTHandler_Init()
{

  struct handler<mAnimatorLight>* ptr;
  
  ptr = &mqtthandler_settings;
=======
void mAnimatorLight::Telemetry_Init()
{
    
  struct telemetry_handler<mAnimatorLight>* ptr;
  
  ptr = &telemetry_settings;
>>>>>>> 5c7962ae6a38a6d065993ed30fd13d2e994344f6
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = tkr_mqtt->dt.configperiod_secs; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
<<<<<<< HEAD
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_segments_teleperiod;
=======
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Settings;
  telemetry_list.push_back(ptr);

  ptr = &telemetry_segments_teleperiod;
>>>>>>> 5c7962ae6a38a6d065993ed30fd13d2e994344f6
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = tkr_mqtt->dt.teleperiod_secs; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
<<<<<<< HEAD
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__SEGMENTS_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Segments;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_playlists_teleperiod;
=======
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC__SEGMENTS_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Segments;
  telemetry_list.push_back(ptr);

  ptr = &telemetry_playlists_teleperiod;
>>>>>>> 5c7962ae6a38a6d065993ed30fd13d2e994344f6
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = tkr_mqtt->dt.ifchanged_secs; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
<<<<<<< HEAD
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__PLAYLISTS_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Playlist;
  mqtthandler_list.push_back(ptr);

  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  ptr = &mqtthandler_matrix_teleperiod;
=======
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC__PLAYLISTS_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Playlist;
  telemetry_list.push_back(ptr);

  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  ptr = &telemetry_matrix_teleperiod;
>>>>>>> 5c7962ae6a38a6d065993ed30fd13d2e994344f6
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 120;//tkr_mqtt->dt.teleperiod_secs; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
<<<<<<< HEAD
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__MATRIX_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Matrix;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  
  #ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
  ptr = &mqtthandler_mode_ambilight_teleperiod;
=======
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC__MATRIX_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Matrix;
  telemetry_list.push_back(ptr);
  #endif // ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  
  #ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
  ptr = &telemetry_mode_ambilight_teleperiod;
>>>>>>> 5c7962ae6a38a6d065993ed30fd13d2e994344f6
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = tkr_mqtt->dt.ifchanged_secs; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
<<<<<<< HEAD
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__MODE_AMBILIGHT__CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Mode_Ambilight;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT

  #ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
  ptr = &mqtthandler_manual_setpixel;
=======
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC__MODE_AMBILIGHT__CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Mode_Ambilight;
  telemetry_list.push_back(ptr);
  #endif // ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT

  #ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
  ptr = &telemetry_manual_setpixel;
>>>>>>> 5c7962ae6a38a6d065993ed30fd13d2e994344f6
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = tkr_mqtt->dt.teleperiod_secs; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
<<<<<<< HEAD
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__MODE_MANUAL_SETPIXEL_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Mode_SetManual;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
    
  // #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  // ptr = &mqtthandler_automation_presets;
=======
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC__MODE_MANUAL_SETPIXEL_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Mode_SetManual;
  telemetry_list.push_back(ptr);
  #endif // ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
    
  // #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  // ptr = &telemetry_automation_presets;
>>>>>>> 5c7962ae6a38a6d065993ed30fd13d2e994344f6
  // ptr->tSavedLastSent = 0;
  // ptr->flags.PeriodicEnabled = true;
  // ptr->flags.SendNow = true;
  // ptr->tRateSecs = 1;//tkr_mqtt->dt.teleperiod_secs; 
  // ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  // ptr->flags.json_level = JSON_LEVEL_DETAILED;
<<<<<<< HEAD
  // ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__AUTOMATION_PRESETS_CTR;
  // ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Auto_Presets;
  // mqtthandler_list.push_back(ptr);
  // #endif // ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    
  #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PLAYLISTS
  ptr = &mqtthandler_manual_setpixel;
=======
  // ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC__AUTOMATION_PRESETS_CTR;
  // ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Auto_Presets;
  // telemetry_list.push_back(ptr);
  // #endif // ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    
  #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PLAYLISTS
  ptr = &telemetry_manual_setpixel;
>>>>>>> 5c7962ae6a38a6d065993ed30fd13d2e994344f6
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = tkr_mqtt->dt.teleperiod_secs; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
<<<<<<< HEAD
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__MODE_MANUAL_SETPIXEL_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Mode_SetManual;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_FEATURE_PIXEL__AUTOMATION_PLAYLISTS

  #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
  ptr = &mqtthandler_debug_palette;
=======
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC__MODE_MANUAL_SETPIXEL_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Mode_SetManual;
  telemetry_list.push_back(ptr);
  #endif // ENABLE_FEATURE_PIXEL__AUTOMATION_PLAYLISTS

  #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
  ptr = &telemetry_debug_palette;
>>>>>>> 5c7962ae6a38a6d065993ed30fd13d2e994344f6
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = tkr_mqtt->dt.ifchanged_secs; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
<<<<<<< HEAD
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_PALETTE__CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Debug_Palette;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE

  #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_CUSTOM_MAPPING_TABLE
  ptr = &mqtthandler_debug__custom_mapping_table;
=======
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_PALETTE__CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Debug_Palette;
  telemetry_list.push_back(ptr);
  #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE

  #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_CUSTOM_MAPPING_TABLE
  ptr = &telemetry_debug__custom_mapping_table;
>>>>>>> 5c7962ae6a38a6d065993ed30fd13d2e994344f6
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = tkr_mqtt->dt.ifchanged_secs; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
<<<<<<< HEAD
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_PALETTE__CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Debug_Palette;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE

  #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_CUSTOM_MAPPING_TABLE
  ptr                         = &mqtthandler_debug_segments;
=======
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_PALETTE__CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Debug_Palette;
  telemetry_list.push_back(ptr);
  #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE

  #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_CUSTOM_MAPPING_TABLE
  ptr                         = &telemetry_debug_segments;
>>>>>>> 5c7962ae6a38a6d065993ed30fd13d2e994344f6
  ptr->tSavedLastSent         = millis();
  ptr->flags.PeriodicEnabled  = true;
  ptr->flags.SendNow          = true;
  ptr->tRateSecs              = tkr_mqtt->dt.ifchanged_secs; 
  ptr->flags.topic_type             = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level             = JSON_LEVEL_DETAILED;
<<<<<<< HEAD
  ptr->postfix_topic          = PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_CUSTOM_MAPPING_TABLE__CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Debug__CustomMappingTable;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_CUSTOM_MAPPING_TABLE

  #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
  ptr                         = &mqtthandler_debug_palette_vector;
=======
  ptr->key          = PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_CUSTOM_MAPPING_TABLE__CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Debug__CustomMappingTable;
  telemetry_list.push_back(ptr);
  #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_CUSTOM_MAPPING_TABLE

  #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
  ptr                         = &telemetry_debug_palette_vector;
>>>>>>> 5c7962ae6a38a6d065993ed30fd13d2e994344f6
  ptr->tSavedLastSent         = millis();
  ptr->flags.PeriodicEnabled  = true;
  ptr->flags.SendNow          = true;
  ptr->tRateSecs              = tkr_mqtt->dt.ifchanged_secs; 
  ptr->flags.topic_type             = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level             = JSON_LEVEL_DETAILED;
<<<<<<< HEAD
  ptr->postfix_topic          = PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_PALETTE_VECTOR__CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Debug_Palette_Vector;
  mqtthandler_list.push_back(ptr);
=======
  ptr->key          = PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_PALETTE_VECTOR__CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Debug_Palette_Vector;
  telemetry_list.push_back(ptr);
>>>>>>> 5c7962ae6a38a6d065993ed30fd13d2e994344f6
  #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR


  #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PERFORMANCE
<<<<<<< HEAD
  ptr                         = &mqtthandler_debug__performance;
=======
  ptr                         = &telemetry_debug__performance;
>>>>>>> 5c7962ae6a38a6d065993ed30fd13d2e994344f6
  ptr->tSavedLastSent         = millis();
  ptr->flags.PeriodicEnabled  = true;
  ptr->flags.SendNow          = true;
  ptr->tRateSecs              = 1; 
  ptr->flags.topic_type             = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level             = JSON_LEVEL_DETAILED;
<<<<<<< HEAD
  ptr->postfix_topic          = PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_PERFORMANCE__CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Debug_Performance;
  mqtthandler_list.push_back(ptr);
=======
  ptr->key          = PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_PERFORMANCE__CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Debug_Performance;
  telemetry_list.push_back(ptr);
>>>>>>> 5c7962ae6a38a6d065993ed30fd13d2e994344f6
  #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PERFORMANCE

} 
  
#endif// USE_MODULE_NETWORK_MQTT



#endif