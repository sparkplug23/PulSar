#include "mAnimatorLight.h" 

#ifdef USE_MODULE_LIGHTS_ANIMATOR 


uint8_t mAnimatorLight::ConstructJSON_Settings(uint8_t json_method){

  JBI->Start();

    JBI->Add("light_size_count", pCONT_iLight->settings.light_size_count);

  return JBI->End();

}


uint8_t mAnimatorLight::ConstructJSON_Animation_Active(uint8_t json_level)
{

  JBI->Start();

    JBI->Add("millis", millis());

    JBI->Add("Start", SEGMENT_I(0).pixel_range.start);
    JBI->Add("Stop", SEGMENT_I(0).pixel_range.stop);

  return JBI->End();

}



uint8_t mAnimatorLight::ConstructJSON_Playlist(uint8_t json_level)
{

  JBI->Start();

    JBI->Add("millis", millis());

  return JBI->End();

}



#ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
uint8_t mAnimatorLight::ConstructJSON_Mode_Ambilight(uint8_t json_level)
{

  JBI->Start();

    JBI->Add("millis", millis());

  return JBI->End();

}
#endif


#ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
uint8_t mAnimatorLight::ConstructJSON_Mode_SetManual(uint8_t json_level)
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
uint8_t mAnimatorLight::ConstructJSON_Debug_Palette(uint8_t json_level)
{

char buffer[100];

JBI->Start();

    JBI->Add("AvailablePalettes", (uint16_t)mPaletteI->PALETTELIST_TOTAL_LENGTH );

    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER

    for(uint8_t seg_i = 0; seg_i<  segments.size(); seg_i++)
    {
      JBI->Level_Start_F("Segment%d",seg_i);
      
        JBI->Add("dataLen", SEGMENT_I(seg_i).palette_container->pData.size());

        JBI->Array_Start("data");
        for(uint8_t i=0;i<SEGMENT_I(seg_i).palette_container->pData.size();i++){
          JBI->Add(SEGMENT_I(seg_i).palette_container->pData[i]);
        }
        JBI->Array_End();

      JBI->Level_End();
    }



    #endif// ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER

    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING

    JBI->Level_Start("Encoding");


      // JBI->Add("encoding",mPaletteI->palettelist.christmas_01.encoding_type);


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

    JBI->Level_End();



    #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING


    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH

JBI->Array_Start_P("DataLength" );
    for(uint8_t palette_id=mPalette::PALETTELIST_VARIABLE_HSBID_01__ID;palette_id<mPaletteI->PALETTELIST_STATIC_CHRISTMAS_28__ID;palette_id++)
    {

      mPalette::PALETTELIST::PALETTE *ptr = mPaletteI->GetPalettePointerByID(palette_id);


      JBI->Add(ptr->data_length);



    }

        JBI->Array_End();


    #endif  // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH

    


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
    
    /**
     * @brief Moving towards preloading palettes from memory into ram/heap for speed (then iram will work)
     * 
     */
    JBI->Level_Start("LoadPalette");

      uint8_t segment_index = 0;

    JBI->Level_End();
    

return JBI->End();

}

#endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE


#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
uint8_t mAnimatorLight::ConstructJSON_Debug_Hardware(uint8_t json_level)
{

  JBI->Start();

  for(uint8_t seg_i = 0; seg_i<  segments.size(); seg_i++)
  {
    JBI->Level_Start_F("Segment%d", seg_i);

      JBI->Add("ColourOrderHex", SEGMENT_I(seg_i).hardware_element_colour_order.data);
    
      JBI->Array_Start("ColourOrder");
        JBI->Add(SEGMENT_I(seg_i).hardware_element_colour_order.red);
        JBI->Add(SEGMENT_I(seg_i).hardware_element_colour_order.green);
        JBI->Add(SEGMENT_I(seg_i).hardware_element_colour_order.blue);
        JBI->Add(SEGMENT_I(seg_i).hardware_element_colour_order.white_cold);
        JBI->Add(SEGMENT_I(seg_i).hardware_element_colour_order.white_warm);
      JBI->Array_End();

    JBI->Level_End();
  }

  return JBI->End();

}

#endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE


#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
uint8_t mAnimatorLight::ConstructJSON_Debug_Segments(uint8_t json_level)
{

JBI->Start();

  JBI->Add("millis", millis());

  JBI->Add("EnableAnimation",SEGMENT_I(0).flags.fEnable_Animation);
  JBI->Add("SEGMENT_I(0).flags.fRunning", SEGMENT_I(0).flags.fRunning);
  JBI->Add("CanShow",stripbus->CanShow());
  JBI->Add("StripSize", STRIP_SIZE_MAX);


// for(uint8_t seg_i = 0; seg_i<2; seg_i++)
// {
//   JBI->Level_Start_F("Segment%d",seg_i);

//     JBI->Add("isActive", _segments[seg_i].isActive());
//       JBI->Level_Start("Transition");
//         JBI->Add("TimeMs", _segments[seg_i].transition.time_ms);
//         JBI->Add("RateMs", _segments[seg_i].transition.rate_ms);
//         JBI->Add("PixelStart", _segments[seg_i].pixel_range.start);
//         JBI->Add("PixelStop", _segments[seg_i].pixel_range.stop);
//       JBI->Level_End();
//     JBI->Add("virtualLength", _segments[seg_i].virtualLength());
    
  
//   JBI->Level_End();


// }


return JBI->End();

}

#endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS


#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
uint8_t mAnimatorLight::ConstructJSON_Debug_Segments_New(uint8_t json_level)
{
  char buffer[50];

  JBI->Start(); 

  JBI->Add("size", segments.size() );

  for(uint8_t seg_i = 0; seg_i<  segments.size(); seg_i++)
  {
    JBI->Level_Start_F("Segment%d",seg_i);

      // Config of segment (New stuff)
      JBI->Add("sizeof", sizeof(segments[seg_i]));

      JBI->Add("ColourPalette", mPaletteI->GetPaletteNameByID(SEGMENT_I(seg_i).palette.id, buffer, sizeof(buffer)));

      // Same values as I am using
      JBI->Add("Start", segments[seg_i].pixel_range.start);
      JBI->Add("Stop", segments[seg_i].pixel_range.stop);
      JBI->Add("SEGLEN", SEGLEN);
      JBI->Add("virtualLength", segments[seg_i].virtualLength());
      JBI->Level_Start("Transition");
        JBI->Add("rate", SEGMENT_I(seg_i).transition.rate_ms);
        JBI->Add("time", SEGMENT_I(seg_i).transition.time_ms);
      JBI->Level_End();
      JBI->Level_Start("Effects");
        JBI->Add("Function", SEGMENT_I(seg_i).effect_id);
        JBI->Add("Speed", SEGMENT_I(seg_i).speed_value);
        JBI->Add("Intensity", SEGMENT_I(seg_i).intensity_value);
        JBI->Add("Grouping", SEGMENT_I(seg_i).grouping);
      JBI->Level_End();
      JBI->Level_Start("Palette");
        JBI->Add("Size", SEGMENT_I(seg_i).palette_container->pData.size());
        JBI->Array_Start("Data");
          for(uint8_t i=0;i<SEGMENT_I(seg_i).palette_container->pData.size();i++)
          {
            JBI->Add(SEGMENT_I(seg_i).palette_container->pData[i]);
          }
        JBI->Array_End();
        // DEBUG_LINE_HERE;
      JBI->Level_Start("DataBuffer");
        // JBI->Array_Start("Data");
        //   uint8_t *p = SEGMENT.Data();
        //   uint8_t len = 5;//SEGMENT_I(seg_i).DataLength()>10?10:SEGMENT_I(seg_i).DataLength();
        // DEBUG_LINE_HERE;
        //   if(len)
        //   {
        // DEBUG_LINE_HERE;
        //     for(uint8_t i=0;i<len;i++)
        //     {
        // DEBUG_LINE_HERE;
        //       if(p==nullptr)
        //         JBI->Add("n");
        //       else
        //         JBI->Add(p[i]);
        //     }
        // DEBUG_LINE_HERE;
        //   }
        // JBI->Array_End();

        JBI->Add("DataLength", SEGMENT_I(seg_i).DataLength());
  
      
      
      
      JBI->Level_End();



      JBI->Level_End();


      JBI->Level_Start("RgbcctColours");
      for(uint8_t rgb_i = 0; rgb_i<  segments[seg_i].rgbcctcolors.size(); rgb_i++)
      {
        JBI->Array_Start_P("Colour%d", rgb_i);
        for(uint8_t i=0;i<5;i++)
        {
          JBI->Add(segments[seg_i].rgbcctcolors[rgb_i].raw[i]); //might need to get rid of operator!
          // JBI->Add(segments[seg_i].rgbcctcolors[rgb_i].G); //might need to get rid of operator!
          // JBI->Add(segments[seg_i].rgbcctcolors[rgb_i].B); //might need to get rid of operator!
          // JBI->Add(segments[seg_i].rgbcctcolors[rgb_i].W1); //might need to get rid of operator!
          // JBI->Add(segments[seg_i].rgbcctcolors[rgb_i].W2); //might need to get rid of operator!
        }
        JBI->Array_End();
      }
      JBI->Level_End();
    
    JBI->Level_End(); // Segment


  }

  return JBI->End();

}
#endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW


#ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
//   uint8_t mAnimatorLight::ConstructJSON_Debug_Animations_Progress(uint8_t json_level)
//   {

// JBI->Start();

//   JBI->Add("millis", millis());

// return JBI->End();

// }

  ANIMIMATION_DEBUG_MQTT_FUNCTION_SIGNATURE;
  mAnimatorLight& setCallback_ConstructJSONBody_Debug_Animations_Progress(ANIMIMATION_DEBUG_MQTT_FUNCTION_SIGNATURE);  
#endif // USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK


#ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK

uint8_t mAnimatorLight::ConstructJSON_Debug_Animations_Progress(uint8_t json_level)
{

  if(anim_progress_mqtt_function_callback)
  {
    JsonBuilderI->Start();
    anim_progress_mqtt_function_callback(); // Call the function
    return JsonBuilderI->End();
  }
  return false;

}

#endif // USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK






// uint8_t mAnimatorLight::ConstructJSON_Animation(uint8_t json_method){

//   #ifdef ENABLE_LOG_LEVEL_DEBUG
//   // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::ConstructJSON_Animation"));
//   #endif
  
//   JBI->Start();

//     JBI->Add("light_power_state", pCONT_iLight->light_power_state ? "ON" : "OFF");
//     // JBI->Add("light_size_count", pCONT_iLight->settings.light_size_count);

//     JBI->Level_Start(D_JSON_TRANSITION);
//     //   transitionobj[D_JSON_METHOD] = GetTransitionMethodName();
//     //   // transitionobj[D_JSON_TIME] = mSupport::safeDivideInt(pCONT_iLight->animation.transition.time_ms.val,1000);
    
// #ifndef USE_DEVFEATURE_METHOD_WLED_BUILD
//       JBI->Add(D_JSON_TIME_MS, SEGMENT_I(0).transition.time_ms);   
// #endif //  USE_DEVFEATURE_METHOD_WLED_BUILD
//     //   transitionobj[D_JSON_TIME_MS] = ;
//     //   // transitionobj[D_JSON_RATE] = mSupport::safeDivideInt(pCONT_iLight->animation.transition.rate_ms,1000);
//     //   transitionobj[D_JSON_RATE_MS] = pCONT_iLight->animation.transition.rate_ms;
//     //   // transitionobj[D_JSON_PIXELS_UPDATE_NUMBER] = GetPixelsToUpdateAsNumberFromPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val);
//     //   transitionobj[D_JSON_PIXELS_UPDATE_PERCENTAGE] = pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val;
//     //   transitionobj[D_JSON_ORDER] = GetTransitionOrderName();
//     JBI->Level_End();




//   return JBI->End();


// }



// uint8_t mAnimatorLight::ConstructJSON_Ambilight(uint8_t json_level){
//   // Awaiting total redesign
  
//   // #ifdef ENABLE_LOG_LEVEL_DEBUG
//   // ALOG_DBM( PSTR(D_LOG_NEO "f::ConstructJSON_Ambilight"));
//   // #endif

//   JBI->Start();

//     // JBI->Add("light_power_state", pCONT_iLight->light_power_state ? "ON" : "OFF");
//     // JBI->Add("light_size_count", pCONT_iLight->settings.light_size_count);

//   return JBI->End();
// }


// // unless this is a debug, only one needs to exist (animation)
// uint8_t mAnimatorLight::ConstructJSON_Segments(uint8_t json_level)
// {
//   // Awaiting total redesign

//   char buffer[30];
  
//   // #ifdef ENABLE_LOG_LEVEL_DEBUG
//   // ALOG_DBM( PSTR(D_LOG_NEO "f::ConstructJSON_Ambilight"));
//   // #endif

//   JBI->Start();

//     // JBI->Add("SegmentCount", )

//     JBI->Array_Start("PixelRange");
//     for(int i=0;i<MAX_NUM_SEGMENTS;i++)
//     {
//       JBI->Add(_segments[i].pixel_range.start);
//       JBI->Add(_segments[i].pixel_range.stop);
//     }
//     JBI->Array_End();


//     JBI->Array_Start("SegmentActive");
//     for(int i=0;i<MAX_NUM_SEGMENTS;i++)
//     {
//       JBI->Add(_segments[i].isActive());
//     }
//     JBI->Array_End();

//     JBI->Array_Start("PaletteID");
//     for(int i=0;i<MAX_NUM_SEGMENTS;i++)
//     {
//       JBI->Add(_segments[i].palette.id);
//     }
//     JBI->Array_End();


//     // JBI->Array_Start("HardwareOrder");
//     //   for(int i=0;i<MAX_NUM_SEGMENTS;i++)
//     //   {
//     //     // sprintf(buffer, "%d", i);
//     //     JBI->Array_Start();
//     //       JBI->Add(_segments[i].hardware_element_colour_order.r);
//     //       JBI->Add(_segments[i].hardware_element_colour_order.g);
//     //       JBI->Add(_segments[i].hardware_element_colour_order.b);
//     //       JBI->Add(_segments[i].hardware_element_colour_order.w1);
//     //       JBI->Add(_segments[i].hardware_element_colour_order.w2);
//     //     JBI->Array_End();
//     //   }
//     // JBI->Array_End();

//     // JBI->Add("light_power_state", pCONT_iLight->light_power_state ? "ON" : "OFF");
//     // JBI->Add("light_size_count", pCONT_iLight->settings.light_size_count);

//   return JBI->End();
// }




// /**
//  * @brief 
//  * definitely remove "state" or make is something basic, minimal info should come from the light_interface not aniamtor
//  * 
//  *
//  * @brief Fix this first, what is running all in here for now until I clean the others up (and remove topics not needed?)
//  * 
//  * @param json_level 
//  * @return uint8_t 
//  */
// uint8_t mAnimatorLight::ConstructJSON_State(uint8_t json_level)
// {

//   uint8_t numpixels = pCONT_iLight->settings.light_size_count<100?pCONT_iLight->settings.light_size_count:100;
//   RgbTypeColor c;

//   char buffer[100];
  
//   JsonBuilderI->Start();  
//     JsonBuilderI->Add_P(PM_JSON_SIZE, pCONT_iLight->settings.light_size_count);
//     JBI->Add("PaletteMaxID", (uint8_t)mPalette::PALETTELIST_STATIC_LENGTH__ID);
//     JBI->Add("ColourPaletteID", pCONT_lAni->SEGMENT_I(0).palette.id );
//     JBI->Add("ColourPalette", mPaletteI->GetPaletteNameByID( SEGMENT_I(0).palette.id, buffer, sizeof(buffer)));
//     // JsonBuilderI->Array_Start("rgb");
//     // for(int i=0;i<numpixels;i++){
//     //   RgbTypeColor c = GetPixelColor(i);
//     //   JsonBuilderI->Add_FV(PSTR("%02X%02X%02X"),c.R,c.G,c.B);
//     // }
//     // JsonBuilderI->Array_End();
//   return JsonBuilderI->End();

// }

// // // Update struct that shows overview, always sends
// uint8_t mAnimatorLight::ConstructJSON_Flasher(uint8_t json_level)
// {

//   char buffer[100];

//   JsonBuilderI->Start();

  
//     JsonBuilderI->Add_P(PM_JSON_HUE, _segment_runtimes[0].rgbcct_controller->getHue360());
//     JsonBuilderI->Add_P(PM_JSON_SAT, _segment_runtimes[0].rgbcct_controller->getSat255());
//     JsonBuilderI->Add_P(PM_JSON_BRIGHTNESS_RGB, _segment_runtimes[0].rgbcct_controller->getBrightnessRGB());

//     JBI->Add("colour_type", (uint8_t)SEGMENT_I(0).colour_type);


//   //   JBI->Level_Start("Segments");

//   //   // limit the size of array
//   //   uint16_t length = _segment_runtimes[0].DataLength();

//   //   length = length < 100 ? length : 100;

//   //   JBI->Array_Start("data0");
//   //     for(int i=0;i<length;i++)
//   //     {
//   //       if(_segment_runtimes[0].Data() != nullptr)
//   //       {
//   //         JBI->Add(_segment_runtimes[0].Data()[i]);
//   //       }
//   //     }
//   //   JBI->Array_End();

//   //   length = _segment_runtimes[1].DataLength();
//   //   length = length < 100 ? length : 100;

//   //   JBI->Array_Start("data1");
//   //   for(int i=0;i<length;i++)
//   //   {
//   //     if(_segment_runtimes[1].Data() != nullptr)
//   //     {
//   //       JBI->Add(_segment_runtimes[1].Data()[i]);
//   //     }
//   //   }
//   //   JBI->Array_End();

//   // JBI->Level_End();

//   JBI->Add("segment_settings_size", sizeof(segment_settings));
//   JBI->Add("segment_runtime_size", sizeof(segment_runtime));

// //   JBI->Add("flashermillis",millis()-flashersettings.tSaved.Update);

// // root[D_JSON_ONOFF] = pCONT_iLight->light_power_state ? "ON" : "OFF";
// // JBI->Add(PM_JSON_MODE, pCONT_iLight->GetAnimationModeName(buffer, sizeof(buffer)));
// // JBI->Add(PM_JSON_FUNCTION, GetFlasherFunctionName(buffer, sizeof(buffer)));
// // root["region"] = GetFlasherRegionName();
// // root[D_JSON_COLOUR_PALETTE] = GetPaletteFriendlyName();
// // root[D_JSON_BRIGHTNESS_PERCENTAGE] = SEGMENT_I(0).brightness*100;
// // root[D_JSON_BRIGHTNESS] = SEGMENT_I(0).brightness;

// //   // JsonObject transitionobj = root.createNestedObject(D_JSON_TRANSITION);
// //   //   transitionobj[D_JSON_METHOD] = GetTransitionMethodName();
// //   //   transitionobj[D_JSON_TIME] = mSupport::safeDivideInt(SEGMENT_I(0).transition.time_ms,1000);
// //   //   transitionobj[D_JSON_TIME_MS] = SEGMENT_I(0).transition.time_ms;
// //   //   transitionobj[D_JSON_RATE] = mSupport::safeDivideInt(SEGMENT_I(0).transition.rate_ms,1000);
// //   //   transitionobj[D_JSON_RATE_MS] = SEGMENT_I(0).transition.rate_ms;
// //   //   transitionobj[D_JSON_FUNCTION] = GetFlasherFunctionName();

// //   JsonObject seq_obj = root.createNestedObject("sequential");
// //     seq_obj["rate_index"] = flashersettings.function_seq.rate_index;

// // JsonBuilderI->Level_Start("slow_glow");
// //   JsonBuilderI->Add("rate_index", flashersettings.function_slo_glo.rate_index);
// // JsonBuilderI->Level_End();

//   return JsonBuilderI->End();
// }


// #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
// uint8_t mAnimatorLight::ConstructJSON_Debug_Palette(uint8_t json_level)
// {

//   char buffer[100];

//   JsonBuilderI->Start();

  
//   return JsonBuilderI->End();
// }
// #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE



#endif
