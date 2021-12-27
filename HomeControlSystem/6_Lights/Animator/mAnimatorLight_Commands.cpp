#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

#ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

void mAnimatorLight::parse_JSONCommand(JsonParserObject obj){

  DEBUG_LINE_HERE;

  AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_LIGHT D_TOPIC "Checking all commands mAnimatorLight::parse_JSONCommand"));
  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  char buffer[50];

  uint8_t segments_found = 0;

  uint8_t segment_index = 0;

  for(uint8_t segment_i = 0; segment_i < MAX_NUM_SEGMENTS; segment_i++)
  {
    snprintf(buffer, sizeof(buffer), "Segment%d", segment_i);
    if(jtok = obj[buffer]){ 



      data_buffer.isserviced += subparse_JSONCommand(jtok.getObject(), segment_i);
      AddLog(LOG_LEVEL_TEST, PSTR("Seg: \"%s\""),buffer);
      segments_found++;

      // If segment commands updated, some effects may need reset
      _segment_runtimes[segment_i].call = 0; 

    }
  }

  if(segments_found == 0)
  {
    data_buffer.isserviced += subparse_JSONCommand(obj); // Legacy commands
  }
  
  if(data_buffer.isserviced){ //update string, move to shared place
   //add flag on this, so its not always forced
   // SetRefreshLEDs(); // implement in 1 second 
  }

  #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
  parsesub_Notifications(obj);
  #endif // USE_TASK_RGBLIGHTING_NOTIFICATIONS

}




/**
 * @brief Allows new segment controls, by calling this function for each segment.
 *        Calling it without any set index, will default to segment of 0 index and turn the other segments off 
 * @param obj 
 */
uint8_t mAnimatorLight::subparse_JSONCommand(JsonParserObject obj, uint8_t segment_index)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

  /**
   * @brief Critical this is always parsed first, as many other commands rely on this value
   * 
   */
  if(segment_index == 255)
  {
    AddLog(LOG_LEVEL_TEST, PSTR("segment_index was unset/default, setting segment_index to 0 for backwards compatability!!"));
    segment_index = 0;

    /**
     * @brief Add flag to make this optional later 
     * Assumes by not providing a segment, only 1 segment is required so disabling the test
     */
    for(uint8_t i = 0; i < MAX_NUM_SEGMENTS; i++)
    {
      _segments[i].pixel_range.start = 0;
      _segments[i].pixel_range.stop  = 0;
    }
    _segments[0].pixel_range.start = 0;
    _segments[0].pixel_range.stop  = STRIP_SIZE_MAX-1;
  }

  /**
   * @brief Important this remains above other commands, as some others rely on states being set (eg. Rgbcct user palettes)
   * 
   */
  if(jtok = obj[PM_JSON_COLOUR_PALETTE]){
    if(jtok.isStr()){
      if((tmp_id=mPaletteI->GetPaletteIDbyName(jtok.getStr()))>=0){
        CommandSet_PaletteID(tmp_id, segment_index);
        data_buffer.isserviced++;
      }
    }else
    if(jtok.isNum()){
      CommandSet_PaletteID(jtok.getInt(), segment_index);
      data_buffer.isserviced++;
    }
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }
  
  if(jtok = obj["PixelRange"]){ 
    if(jtok.isArray()){
      uint8_t array[2];
      uint8_t arrlen = 0;
      JsonParserArray arrobj = jtok;
      for(auto v : arrobj) {
        if(arrlen > 2){ break; }

        

        switch(arrlen)
        {
          case 0: _segments[segment_index].pixel_range.start = v.getInt(); break;
          case 1: _segments[segment_index].pixel_range.stop  = v.getInt(); break;
        }

        array[arrlen++] = v.getInt();
        // #ifdef ENABLE_LOG_LEVEL_DEBUG
        AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "PixelRange" " [i%d:v%d]"),arrlen-1,array[arrlen-1]);
        AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "PixelRange Segment[%d] = %d -> %d"),
          segment_index,
          _segments[segment_index].pixel_range.start,
          _segments[segment_index].pixel_range.stop
        );
        



        // #endif// ENABLE_LOG_LEVEL_DEBUG          
      }
      // CommandSet_PixelGrouping_MappedMultiplierData(array, arrlen);

      if(_segments[segment_index].pixel_range.stop > STRIP_SIZE_MAX)
      {
        AddLog(LOG_LEVEL_ERROR, PSTR("_segments[segment_index].pixel_range.stop exceeds max"), _segments[segment_index].pixel_range.stop, STRIP_SIZE_MAX);
        _segments[segment_index].pixel_range.stop = STRIP_SIZE_MAX;
      }


      
      data_buffer.isserviced++;
    }
  }

  if(jtok = obj[PM_JSON_EFFECTS].getObject()[PM_JSON_FUNCTION]){
    if(jtok.isStr()){
      if((tmp_id=GetFlasherFunctionIDbyName(jtok.getStr()))>=0){
        CommandSet_Flasher_FunctionID(tmp_id, segment_index);
        data_buffer.isserviced++;
      }
    }else
    if(jtok.isNum()){
      CommandSet_Flasher_FunctionID(jtok.getInt(), segment_index);
      data_buffer.isserviced++;
    }
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_K(D_JSON_FUNCTION)), GetFlasherFunctionName(buffer, sizeof(buffer)));
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }
  

  if(jtok = obj[PM_JSON_EFFECTS].getObject()[PM_JSON_COLOUR_REFRESH_RATE])
  { 
    CommandSet_Flasher_UpdateColourRegion_RefreshSecs(jtok.getInt(), segment_index);
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_JSON_COLOUR_REFRESH_RATE)), flashersettings.update_colour_region.refresh_secs);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }
  
/**
 * @brief 
 * *WLED added
 * 
 */
if(jtok = obj[PM_JSON_EFFECTS].getObject()["Intensity"])
  { 

    _segments[segment_index].set_intensity(jtok.getInt());
    
    
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_JSON_COLOUR_REFRESH_RATE)), flashersettings.update_colour_region.refresh_secs);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }
  
if(jtok = obj[PM_JSON_EFFECTS].getObject()["Speed"])
  { 

    _segments[segment_index].set_speed(jtok.getInt());
    
    
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_JSON_COLOUR_REFRESH_RATE)), flashersettings.update_colour_region.refresh_secs);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }
  


  // if(jtok = obj[PM_JSON_EFFECTS].getObject()[PM_JSON_DIRECTION])
  // { 
  //   CommandSet_Flasher_Flags_Movement_Direction(jtok.getInt());
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_DIRECTION)), flashersettings.flags.movement_direction);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }
  

  // I want a segment_brightness value, not just global, if set, then it is used. Or, it is also scaled, and thus if set to max then global only has control, or else both can be scalers
  // if(jtok = obj[PM_JSON_EFFECTS].getObject()[BRIGHTNESS]){  
  //   pCONT_iLight->animation.flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique = jtok.getInt();
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_AGED_COLOURING)), pCONT_iLight->animation.flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }

  // if(jtok = obj[PM_JSON_EFFECTS].getObject()[PM_JSON_ALTERNATE_BRIGHTNESS_MIN]){ 
  //   CommandSet_Flasher_Alternate_Brightness_Min(jtok.getInt());
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_ALTERNATE_BRIGHTNESS_MIN)), shared_flasher_parameters.alternate_brightness_min);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }

  // if(jtok = obj[PM_JSON_EFFECTS].getObject()[PM_JSON_ALTERNATE_BRIGHTNESS_MAX]){ 
  //   CommandSet_Flasher_Alternate_Brightness_Max(jtok.getInt());
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_ALTERNATE_BRIGHTNESS_MAX)), shared_flasher_parameters.alternate_brightness_max);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }

  // if(jtok = obj[PM_JSON_EFFECTS].getObject()[PM_JSON_ALTERNATE_AMOUNT]){ 
  //   CommandSet_Flasher_Alternate_RandomAmountPercentage(jtok.getInt());
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_ALTERNATE D_JSON_RANDOM_AMOUNT)), shared_flasher_parameters.alternate_random_amount_as_percentage);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }

  // if(jtok = obj[PM_JSON_EFFECTS].getObject()[PM_JSON_AGED_COLOURING]){
  //   pCONT_iLight->animation.flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique = jtok.getInt();
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_AGED_COLOURING)), pCONT_iLight->animation.flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }

  

  if(jtok = obj[PM_JSON_RGB_COLOUR_ORDER]){
    if(jtok.isStr()){
      CommandSet_HardwareColourOrderTypeByStr(jtok.getStr(), segment_index);
      data_buffer.isserviced++;
    }
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_RGB_COLOUR_ORDER)), GetHardwareColourTypeName(buffer, sizeof(buffer)));
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }
  
  // #ifdef ENABLE_PIXEL_FUNCTION_PIXELGROUPING
  // /**
  //  * Pixel output grouping settings
  //  * */
  // if(jtok = obj[PM_JSON_PIXELGROUPING].getObject()[PM_JSON_AGED_COLOURING]){
  //   CommandSet_PixelGrouping_Flag_AgedColouring(jtok.getInt());
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_PIXELGROUPING D_JSON_ENABLED)), pixel_group.flags.fEnabled);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }

  // if(jtok = obj[PM_JSON_PIXELGROUPING].getObject()[PM_JSON_MODE]){
  //   CommandSet_PixelGrouping_Flag_ModeID(jtok.getInt());
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_PIXELGROUPING D_JSON_MODE)), pixel_group.flags.multiplier_mode_id);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }

  // if(jtok = obj[PM_JSON_PIXELGROUPING].getObject()[PM_JSON_MULTIPLIER]){ 
  //   CommandSet_PixelGrouping_Flag_Multiplier(jtok.getInt());
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_PIXELGROUPING D_JSON_MULTIPLIER)), pixel_group.multiplier);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }

  // if(jtok = obj[PM_JSON_PIXELGROUPING].getObject()[PM_JSON_MAPPED_MULTIPLIER_DATA]){ 
  //   // CommandSet_PixelGrouping_MappedMultiplierData();
  //   if(jtok.isArray()){
  //     uint8_t array[60];
  //     uint8_t arrlen = 0;
  //     JsonParserArray arrobj = obj[PM_JSON_PIXELGROUPING].getObject()[PM_JSON_MAPPED_MULTIPLIER_DATA];
  //     for(auto v : arrobj) {
  //       if(arrlen > D_MAPPED_ARRAY_DATA_MAXIMUM_LENGTH){ break; }
  //       array[arrlen++] = v.getInt();
  //       #ifdef ENABLE_LOG_LEVEL_DEBUG
  //       AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_JSON_MAPPED_MULTIPLIER_DATA " [i%d:v%d]"),arrlen-1,array[arrlen-1]);
  //       #endif// ENABLE_LOG_LEVEL_DEBUG          
  //     }
  //     CommandSet_PixelGrouping_MappedMultiplierData(array, arrlen);
  //     data_buffer.isserviced++;
  //   }
  // }
  // #endif // ENABLE_PIXEL_FUNCTION_PIXELGROUPING

  // #ifdef ENABLE_PIXEL_FUNCTION_MIXER
  // if(jtok = obj[PM_JSON_MIXER].getObject()[PM_JSON_ENABLED]){ 
  //   // CommandSet_Mixer_Flags_Enabled(jtok.getInt());
  //   data_buffer.isserviced++;
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_MIXER D_JSON_ENABLED)), mixer.flags.Enabled);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }
   
  // // if(jtok = obj[PM_JSON_MIXER].getObject()[PM_JSON_TIME_SCALER_AS_PERCENTAGE]){ 
  // //   mixer.time_scaler = jtok.getInt();
  // //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  // //   AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_TIME_SCALER_AS_PERCENTAGE)), mixer.time_scaler);
  // //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // // }

  // if(jtok = obj[PM_JSON_MIXER].getObject()[PM_JSON_RUNTIME_DURATION_SCALER_PERCENTAGE]){ 
  //   // CommandSet_Mixer_RunTimeScalerPercentage(jtok.getInt());
  //   data_buffer.isserviced++;
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_MIXER D_JSON_RUNTIME_DURATION_SCALER_PERCENTAGE)), mixer.run_time_duration_scaler_as_percentage);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }

  // if(jtok = obj[PM_JSON_MIXER].getObject()[PM_JSON_RUNNING_ID]){  
  //   // CommandSet_Mixer_RunningID(jtok.getInt());
  //   data_buffer.isserviced++;
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_RUNNING_ID)), val);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }
  // #endif //ENABLE_PIXEL_FUNCTION_MIXER


  
  // #ifdef USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
  //   if(jtok = obj[PM_JSON_BRIGHTNESS_MIN]){ 
  //     CommandSet_Brightness_Min(jtok.getInt());
  //     #ifdef ENABLE_LOG_LEVEL_DEBUG
  //     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_MIN)), flashersettings.brightness_min);
  //     #endif // ENABLE_LOG_LEVEL_DEBUG
  //   }

  //   if(jtok = obj[PM_JSON_BRIGHTNESS_MAX]){ 
  //     CommandSet_Brightness_Max(jtok.getInt());
  //     #ifdef ENABLE_LOG_LEVEL_DEBUG
  //     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_MAX)), flashersettings.brightness_max);
  //     #endif // ENABLE_LOG_LEVEL_DEBUG
  //   }



  //   if(jtok = obj[PM_JSON_PALETTE_GENERATION].getObject()[PM_JSON_RANDOMISE_BRIGHTNESS_MODE]){
  //     CommandSet_Palette_Generation_Randomise_Brightness_Mode(jtok.getInt());
  //     #ifdef ENABLE_LOG_LEVEL_DEBUG
  //     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_MIN)), flashersettings.brightness_min);
  //     #endif // ENABLE_LOG_LEVEL_DEBUG
  //   }
    


  // #ifdef ENABLE_DEVFEATURE_RGB_CLOCK
  // if(jtok = obj[PM_JSON_RGB_CLOCK].getObject()[PM_JSON_MANUAL_NUMBER]){
  //   lcd_display_show_number = jtok.getInt();
  //   // CommandSet_Palette_Generation_Randomise_Brightness_Mode(jtok.getInt());
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_MIN)), flashersettings.brightness_min);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }
  // #endif // ENABLE_DEVFEATURE_RGB_CLOCK


  // if(jtok = obj[PM_RGB_DATA_STREAM]){  // PM_RGB_DATA_STREAM

  // // maybe to be done with a topic directly so its faster, straight buffer into setpixel

  //   // uint8_t colour_array[5];
  //   // memset(colour_array,0,sizeof(colour_array));
  //   // uint8_t jsonpair_count = jtok.size();

  //   // for(int index = 0; index < jsonpair_count; index++){
  //   //   jtok.nextOne(); //skip start of object
  //   //   Serial.println(jtok.getInt());
  //   //   colour_array[index] = jtok.getInt();
  //   // }
    
  //   // CommandSet_ActiveSolidPalette_Raw(colour_array);
  //   // data_buffer.isserviced++;

  //   // #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   // snprintf_P(buffer, sizeof(buffer), PSTR("[%d,%d,%d,%d,%d]"),colour_array[0],colour_array[1],colour_array[2],colour_array[3],colour_array[4]);
  //   // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_SVALUE_K(D_JSON_SCENE,D_JSON_COLOUR)), buffer);
  //   // #endif // ENABLE_LOG_LEVEL_DEBUG
  // }



#ifdef ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL


  if(jtok = obj["Strip"].getObject()["ClearTo"]){ 
    if(jtok.isArray()){
      RgbcctColor colour = RgbcctColor(0);

      uint8_t index = 0;
      JsonParserArray array = obj["Strip"].getObject()["ClearTo"];
      for(auto v : array) {
        switch(index){
          case 0: colour.R = v.getInt();
          case 1: colour.G = v.getInt();
          case 2: colour.B = v.getInt();
          case 3: colour.WC = v.getInt();
          case 4: colour.WW = v.getInt();
        }
        int val = v.getInt();
        // if(index > D_MAPPED_ARRAY_DATA_MAXIMUM_LENGTH){ break; }
        // editable_mapped_array_data_array[index++] = val;
        index++;
        // pCONT_iLight->animation.mode_id =  pCONT_iLight->ANIMATION_MODE_MANUAL_SETPIXEL_ID;
        // SetPixelColor(val, colour);
        // animation_colours[val].DesiredColour = colour;
    #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_MAPPED_MULTIPLIER_DATA " [i%d:v%d]"),index-1,val);
    #endif// ENABLE_LOG_LEVEL_INFO          
      }
      SetPixelColor_All(colour);
    }

  }

  if(jtok = obj["Strip"].getObject()[PM_JSON_MANUAL_SETPIXEL_TO_SCENE]){ 
    if(jtok.isArray()){
      //testing, froce it
      RgbcctColor colour = RgbcctColor(255,255,255);
      uint8_t index = 0;
      JsonParserArray array = obj["Strip"].getObject()[PM_JSON_MANUAL_SETPIXEL_TO_SCENE];//jtok;//obj[PM_JSON_MANUAL_SETPIXEL_TO_SCENE];
      for(auto v : array) {
        int val = v.getInt();
        // if(index > D_MAPPED_ARRAY_DATA_MAXIMUM_LENGTH){ break; }
        // editable_mapped_array_data_array[index++] = val;
        index++;

        //Count active pixels
        uint16_t pixels_on = 0;
        for(uint16_t i=0;i<pCONT_iLight->settings.light_size_count;i++){ 
          if(pCONT_iLight->RgbColorto32bit(GetPixelColor(i))){
            pixels_on++;
          }      
        }

        if(pixels_on <= 5){
          colour = RgbcctColor(255,0,0);
        }else
        if(pixels_on <= 10){
          colour = RgbcctColor(0,255,0);
        }else
        if(pixels_on <= 15){
          colour = RgbcctColor(0,0,255);
        }else
        if(pixels_on <= 20){
          colour = RgbcctColor(255,0,255);
        }else
        if(pixels_on <= 25){
          colour = RgbcctColor(255,50,0);
        }else
        if(pixels_on <= 30){
          colour = RgbcctColor(0,255,55);
        }else
        if(pixels_on <= 35){
          colour = RgbcctColor(0,255,255);
        }else{
          colour = RgbcctColor(255,255,255);
        }

        _segments[0].mode_id =  pCONT_iLight->ANIMATION_MODE_MANUAL_SETPIXEL_ID;

        SetPixelColor(val, colour, 0);

        //animation_colours[val].DesiredColour = colour;

    #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_MAPPED_MULTIPLIER_DATA " [i%d:v%d]"),index-1,val);
    #endif// ENABLE_LOG_LEVEL_INFO          
      }
      // pixel_group.mapped_array_data.length = index;

      pCONT_iLight->ShowInterface();

      data_buffer.isserviced++;
    }
  }



#endif // ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL




  // if(jtok = obj[PM_JSON_PIXELSGROUPED]){
  //   animation.pixelgrouped = obj[CFLASH(PM_JSON_PIXELSGROUPED];
  //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE),D_JSON_PIXELSGROUPED,animation.pixelgrouped);
  //   #endif
  // }

  //override commands that run for one animation cycle then are cleared to 0
  // if(!obj[CFLASH(PM_JSON_REFRESH_ALLPIXELS].isNull()){
  //   animation_override.fRefreshAllPixels = obj[CFLASH(PM_JSON_REFRESH_ALLPIXELS];
  //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE),D_JSON_REFRESH_ALLPIXELS,animation_override.fRefreshAllPixels);
  //   #endif
  // }

// CHANGE TO USE RELAYS INSTEAD
  // if(!obj[CFLASH(PM_JSON_EXTERNAL_POWER_ONOFF)].isNull()){ 
  //   const char* onoff = obj[CFLASH(PM_JSON_EXTERNAL_POWER_ONOFF)];    
  //   uint8_t relay_state_new = pCONT_sup->GetStateNumber(onoff);
  //   uint8_t relay_state_current = 0;
  //   if(relay_state_new==STATE_NUMBER_TOGGLE_ID){
  //     // pCONT_iLight->animation.flags.fEnable_Animation ^= 1;
  //   }else{
  //     // pCONT_iLight->animation.flags.fEnable_Animation = state;
  //   }
  //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_EXTERNAL_POWER_ONOFF,relay_state_new);
  //   #endif
  //   data_buffer.isserviced++;
  // }


  // if(!obj[CFLASH(PM_JSON_ONOFF].isNull()){ 
  //   const char* onoff = obj[CFLASH(PM_JSON_ONOFF];
  //   if(strstr(onoff,"ON")){ 
      
  //     #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED "\"onoff\"=\"ON\""));
  //     #endif
      
  //     #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "MODE_TURN_ON_ID"));
  //     #endif
  //     // Add this as "SAVE" state then "LOAD" state
  //     memcpy(&pCONT_iLight->animation,&pCONT_iLight->animation_stored,sizeof(pCONT_iLight->animation));// RESTORE copy of state

  //     pCONT_iLight->SetAnimationProfile(pCONT_iLight->ANIMATION_PROFILE_TURN_ON_ID);
  //     pCONT_iLight->light_power_state = true;

  //     //pCONT_iLight->animation.mode_id = MODE_TURN_ON_ID;
  //     data_buffer.isserviced++;
  //   }else if(strstr(onoff,"OFF")){
  //     #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED "\"onoff\"=\"OFF\""));
  //     #endif
  //     #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "MODE_TURN_OFF_ID"));
  //     #endif
  //     memcpy(&pCONT_iLight->animation_stored,&pCONT_iLight->animation,sizeof(pCONT_iLight->animation)); // STORE copy of state
  //     pCONT_iLight->SetAnimationProfile(pCONT_iLight->ANIMATION_PROFILE_TURN_OFF_ID);
  //     pCONT_iLight->light_power_state = false;

  //     //pCONT_iLight->animation.mode_id = MODE_TURN_OFF_ID;
  //     data_buffer.isserviced++;
  //   }else{
  //     #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //     AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_NOMATCH));
  //     #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   }
  // }

  
  if(jtok = obj[PM_JSON_TRANSITION].getObject()[PM_JSON_TIME]){ // default to secs
    CommandSet_Animation_Transition_Time_Ms(jtok.getInt()*1000, segment_index);
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_TIME)),animation.transition.time_ms.val);  
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }else
  if(jtok = obj[PM_JSON_TRANSITION].getObject()[PM_JSON_TIME_MS]){
    CommandSet_Animation_Transition_Time_Ms(jtok.getInt(), segment_index);
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_TIME)),animation.transition.time_ms.val);  
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }
  
  if(jtok = obj[PM_JSON_TRANSITION].getObject()[PM_JSON_RATE]){ // default to secs
    CommandSet_Animation_Transition_Rate_Ms(jtok.getInt()*1000, segment_index);
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_RATE)),animation.transition.rate_ms);  
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }else
  if(jtok = obj[PM_JSON_TRANSITION].getObject()[PM_JSON_RATE_MS]){
    CommandSet_Animation_Transition_Rate_Ms(jtok.getInt(), segment_index);
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_RATE_MS)),animation.transition.rate_ms);  
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }
  
  if(jtok = obj[PM_JSON_TRANSITION].getObject()[PM_JSON_PIXELS_UPDATE_NUMBER]){
    CommandSet_LightsCountToUpdateAsNumber(jtok.getInt(), segment_index);
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE)), animation.transition.pixels_to_update_as_percentage.val);
    #endif
  }else
  if(jtok = obj[PM_JSON_TRANSITION].getObject()[PM_JSON_PIXELS_UPDATE_PERCENTAGE]){ 
    CommandSet_LightsCountToUpdateAsPercentage(jtok.getInt(), segment_index);
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_NUMBER)), animation.transition.pixels_to_update_as_number);
    #endif
  }
  
  if(jtok = obj[PM_JSON_TRANSITION].getObject()[PM_JSON_ORDER]){
    if(jtok.isStr()){
      if((tmp_id=GetTransitionOrderIDbyName(jtok.getStr()))>=0){
        CommandSet_TransitionOrderID(tmp_id, segment_index);
        data_buffer.isserviced++;
      }
    }else
    if(jtok.isNum()){
      CommandSet_TransitionOrderID(jtok.getInt(), segment_index);
      data_buffer.isserviced++;
    }
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_SVALUE_K(D_JSON_TRANSITION,D_JSON_ORDER)), GetTransitionOrderName(buffer, sizeof(buffer)));
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  /******************************
   * Rgbcct controller that will be added into segments (phasing out of interlight?)
   * 
   * *******************************/
  

  // if(jtok = obj[PM_JSON_RGBCCT_LINKED]){
  //   CommandSet_ActiveSolidPalette_RGBCT_Linked(jtok.getInt()); //needs function
  //   data_buffer.isserviced++;
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_RGBCCT_LINKED)), value);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // } 


  if(jtok = obj[PM_JSON_HUE]){ // Assume range 0-359
    CommandSet_ActiveSolidPalette_Hue_360(jtok.getInt(), segment_index);
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_HUE)), getHue());
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  if(jtok = obj[PM_JSON_SAT]){ // Assume range 0-100
    CommandSet_ActiveSolidPalette_Sat_255(mapvalue(jtok.getInt(), 0,100, 0,255), segment_index);
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_SAT)), getSat());
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }else
  if(jtok = obj[PM_JSON_SAT_255]){ // alternate full range 0-255
    CommandSet_ActiveSolidPalette_Sat_255(jtok.getInt(), segment_index);
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_SAT_255)), getSat());
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  // if(jtok = obj[PM_JSON_BRIGHTNESS]){ // Assume range 0-100
  //   CommandSet_Brt_255(mapvalue(jtok.getInt(), 0,100, 0,255));
  //   data_buffer.isserviced++;
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS)), brt);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }else
  // if(jtok = obj[PM_JSON_BRIGHTNESS_255]){ // alternate full range 0-255
  //   CommandSet_Brt_255(jtok.getInt());
  //   data_buffer.isserviced++;
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_255)), getBri());
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }

  if(jtok = obj[PM_JSON_BRIGHTNESS_RGB]){ // Assume range 0-100
    CommandSet_BrtRGB_255(mapvalue(jtok.getInt(), 0,100, 0,255), segment_index);
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_RGB)), getBriRGB());
    #endif //#ifdef ENABLE_LOG_LEVEL_DEBUG
  }else
  if(jtok = obj[PM_JSON_BRIGHTNESS_RGB_255]){ // alternate full range 0-255
    CommandSet_BrtRGB_255(jtok.getInt(), segment_index);
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_RGB_255)), getBriRGB());
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  if(jtok = obj[PM_JSON_BRIGHTNESS_CCT]){ // Assume range 0-100
    CommandSet_BrtCT_255(mapvalue(jtok.getInt(), 0,100, 0,255), segment_index);
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_CCT)), getBriCT());
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }else
  if(jtok = obj[PM_JSON_BRIGHTNESS_CCT_255]){ // alternate full range 0-255
    CommandSet_BrtCT_255(jtok.getInt(), segment_index);
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_CCT_255)), getBriCT());
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }


// Flip these so default is 0 to 100%, and the other is range controlled
  if(jtok = obj[PM_JSON_CCT_PERCENTAGE]){ // Assume range 0-100
    CommandSet_ActiveSolidPalette_ColourTemp(mapvalue(jtok.getInt(), 0,100, pCONT_iLight->_ct_min_range,pCONT_iLight->_ct_max_range), segment_index);
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_CCT_TEMP)), LightGetColorTemp());
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }else
  if(jtok = obj[PM_JSON_CCT_TEMP]){ // Assume range 0-100
    CommandSet_ActiveSolidPalette_ColourTemp(jtok.getInt(), segment_index);
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_CCT_TEMP)), LightGetColorTemp());
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

//   if(jtok = obj[PM_JSON_RGBCCT_LINKED]){
//     CommandSet_ActiveSolidPalette_RGBCT_Linked(jtok.getInt()); //needs function
//     data_buffer.isserviced++;
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_RGBCCT_LINKED)), value);
//     #endif // ENABLE_LOG_LEVEL_DEBUG
//   } 



// #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
//   if(jtok = obj[PM_JSON_BRIGHTNESS_CCT]){ // Assume range 0-100
//     CommandSet_BrtCT_255(mapvalue(jtok.getInt(), 0,100, 0,255));
//     data_buffer.isserviced++;
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_CCT)), getBriCT());
//     #endif // ENABLE_LOG_LEVEL_DEBUG
//   }else
//   if(jtok = obj[PM_JSON_BRIGHTNESS_CCT_255]){ // alternate full range 0-255
//     CommandSet_BrtCT_255(jtok.getInt());
//     data_buffer.isserviced++;
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_CCT_255)), getBriCT());
//     #endif // ENABLE_LOG_LEVEL_DEBUG
//   }


// // Flip these so default is 0 to 100%, and the other is range controlled
//   if(jtok = obj[PM_JSON_CCT_PERCENTAGE]){ // Assume range 0-100
//     CommandSet_ActiveSolidPalette_ColourTemp(mapvalue(jtok.getInt(), 0,100, _ct_min_range,_ct_max_range));
//     data_buffer.isserviced++;
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_CCT_TEMP)), LightGetColorTemp());
//     #endif // ENABLE_LOG_LEVEL_DEBUG
//   }else
//   if(jtok = obj[PM_JSON_CCT_TEMP]){ // Assume range 0-100
//     CommandSet_ActiveSolidPalette_ColourTemp(jtok.getInt());
//     data_buffer.isserviced++;
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_CCT_TEMP)), LightGetColorTemp());
//     #endif // ENABLE_LOG_LEVEL_DEBUG
//   }

//   if(jtok = obj[PM_JSON_RGBCCT_LINKED]){
//     CommandSet_ActiveSolidPalette_RGBCT_Linked(jtok.getInt()); //needs function
//     data_buffer.isserviced++;
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_RGBCCT_LINKED)), value);
//     #endif // ENABLE_LOG_LEVEL_DEBUG
//   } 

//   if(jtok = obj[PM_JSON_TRANSITION].getObject()[PM_JSON_TIME]){ // default to secs
//     CommandSet_Animation_Transition_Time_Ms(jtok.getInt()*1000);
//     data_buffer.isserviced++;
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_TIME)),animation.transition.time_ms.val);  
//     #endif // ENABLE_LOG_LEVEL_DEBUG
//   }else
//   if(jtok = obj[PM_JSON_TRANSITION].getObject()[PM_JSON_TIME_MS]){
//     CommandSet_Animation_Transition_Time_Ms(jtok.getInt());
//     data_buffer.isserviced++;
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_TIME)),animation.transition.time_ms.val);  
//     #endif // ENABLE_LOG_LEVEL_DEBUG
//   }
  
//   if(jtok = obj[PM_JSON_TRANSITION].getObject()[PM_JSON_RATE]){ // default to secs
//     CommandSet_Animation_Transition_Rate_Ms(jtok.getInt()*1000);
//     data_buffer.isserviced++;
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_RATE)),animation.transition.rate_ms);  
//     #endif // ENABLE_LOG_LEVEL_DEBUG
//   }else
//   if(jtok = obj[PM_JSON_TRANSITION].getObject()[PM_JSON_RATE_MS]){
//     CommandSet_Animation_Transition_Rate_Ms(jtok.getInt());
//     data_buffer.isserviced++;
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_RATE_MS)),animation.transition.rate_ms);  
//     #endif // ENABLE_LOG_LEVEL_DEBUG
//   }
  
//   if(jtok = obj[PM_JSON_TRANSITION].getObject()[PM_JSON_PIXELS_UPDATE_NUMBER]){
//     CommandSet_LightsCountToUpdateAsNumber(jtok.getInt());
//     data_buffer.isserviced++;
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE)), animation.transition.pixels_to_update_as_percentage.val);
//     #endif
//   }else
//   if(jtok = obj[PM_JSON_TRANSITION].getObject()[PM_JSON_PIXELS_UPDATE_PERCENTAGE]){ 
//     CommandSet_LightsCountToUpdateAsPercentage(jtok.getInt());
//     data_buffer.isserviced++;
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_NUMBER)), animation.transition.pixels_to_update_as_number);
//     #endif
//   }
  
//   if(jtok = obj[PM_JSON_TRANSITION].getObject()[PM_JSON_ORDER]){
//     if(jtok.isStr()){
//       if((tmp_id=GetTransitionOrderIDbyName(jtok.getStr()))>=0){
//         CommandSet_TransitionOrderID(tmp_id);
//         data_buffer.isserviced++;
//       }
//     }else
//     if(jtok.isNum()){
//       CommandSet_TransitionOrderID(jtok.getInt());
//       data_buffer.isserviced++;
//     }
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_SVALUE_K(D_JSON_TRANSITION,D_JSON_ORDER)), GetTransitionOrderName(buffer, sizeof(buffer)));
//     #endif // ENABLE_LOG_LEVEL_DEBUG
//   }
// #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT


// #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
//   if(jtok = obj[PM_JSON_RGB_COLOUR_ORDER]){
//     if(jtok.isStr()){
//       // if((tmp_id=GetHardwareColourTypeIDbyName(jtok.getStr()))>=0){
//         CommandSet_HardwareColourOrderTypeByStr(jtok.getStr());
//         data_buffer.isserviced++;
//       // }
//     }
//     // else
//     // if(jtok.isNum()){      
//     //   CommandSet_HardwareColourOrderTypeID(jtok.getInt());
//     //   data_buffer.isserviced++;
//     // }
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_RGB_COLOUR_ORDER)), GetHardwareColourTypeName(buffer, sizeof(buffer)));
//     #endif // ENABLE_LOG_LEVEL_DEBUG
//   }
// #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT

  // if(jtok = obj[PM_JSON_RGB_COLOUR_ORDER2]){
  //   if(jtok.isStr()){
  //     if((tmp_id=GetHardwareColourTypeIDbyName2(jtok.getStr()))>=0){
  //       CommandSet_HardwareColourOrderTypeID2(tmp_id);
  //       data_buffer.isserviced++;
  //     }
  //   }else
  //   if(jtok.isNum()){      
  //     CommandSet_HardwareColourOrderTypeID2(jtok.getInt());
  //     data_buffer.isserviced++;
  //   }
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_RGB_COLOUR_ORDER)), GetHardwareColourTypeName2(buffer, sizeof(buffer)));
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }

}


// void mAnimatorLight::CommandSet_EffectsModeID(uint8_t mode, uint8_t segment)
// {
//   #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  
// #ifdef USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
//   char buffer[50];
//   mEffects->setMode(segment, mode);
//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_HARDWARE_TYPE)), GetEffectsModeName(buffer, sizeof(buffer)));
//   #endif // ENABLE_LOG_LEVEL_COMMANDS
// #endif // USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION

//   #endif
// } 



// #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT

//   /********************************************************************************************************************************
//   **********Flasher Function ******************************************************************************************************
//   ********************************************************************************************************************************/
// void mAnimatorLight::CommandSet_Flasher_FunctionID(uint8_t value){
  
//   flashersettings.function = value;      //make function "changeFlasherFunction" so then the region is automatically updated internally
//   flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID;
//   pCONT_iLight->animation.flags.animator_first_run= true; // first run, so do extra things

  
  
//   #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
//   setCallback_ConstructJSONBody_Debug_Animations_Progress(nullptr); // clear to be reset
//   #endif
  
//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   char buffer[30];
//   // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_JSON_COMMAND_SVALUE_SVALUE_K(D_JSON_EFFECTS, D_JSON_FUNCTION)), GetFlasherFunctionName(buffer, sizeof(buffer)));
//   #endif // ENABLE_LOG_LEVEL_COMMANDS

// }

// int8_t mAnimatorLight::GetFlasherFunctionIDbyName(const char* f){

//   if(f=='\0') return -2;
//   if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_SOLID_COLOUR_NAME_CTR)){ return EFFECTS_FUNCTION_SOLID_COLOUR_ID; }
//   if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_STATIC_PALETTE_NAME_CTR)){  return EFFECTS_FUNCTION_STATIC_PALETTE_ID; }
//   if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_SLOW_GLOW_NAME_CTR)){  return EFFECTS_FUNCTION_SLOW_GLOW_ID; }
//   if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_STATIC_PALETTE_NAME_CTR)){  return EFFECTS_FUNCTION_STATIC_PALETTE_ID; }
//   if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_FIREPLACE_1D_01_NAME_CTR)){ return EFFECTS_FUNCTION_FIREPLACE_1D_01_ID; }
//   if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_STEP_THROUGH_PALETTE_CTR)){ return EFFECTS_FUNCTION_SEQUENTIAL_PALETTE_ID; }
  
//   if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01_NAME_CTR)){ return EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01_ID; }
//   if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_WITH_AUGMENTED_TRANSITIONS_01_NAME_CTR)){ return EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_WITH_AUGMENTED_TRANSITIONS_01_ID; }
//   if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_CCT_TEMPERATURE_01_NAME_CTR)){ return EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_CCT_TEMPERATURE_01_ID; }
  
//   if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_TESTER_NAME_CTR)){ return EFFECTS_FUNCTION_TESTER_ID; }

//   return -1;

// }
// const char* mAnimatorLight::GetFlasherFunctionName(char* buffer, uint8_t buflen){
//   return GetFlasherFunctionNamebyID(flashersettings.function, buffer, buflen);
// }
// const char* mAnimatorLight::GetFlasherFunctionNamebyID(uint8_t id, char* buffer, uint8_t buflen){
//   switch(id){
//     default:  snprintf_P(buffer, buflen, PM_SEARCH_NOMATCH);  break;
//     case EFFECTS_FUNCTION_STATIC_PALETTE_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_STATIC_PALETTE_NAME_CTR);  break;
//     case EFFECTS_FUNCTION_SLOW_GLOW_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_SLOW_GLOW_NAME_CTR);  break;
//     case EFFECTS_FUNCTION_SEQUENTIAL_PALETTE_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_STEP_THROUGH_PALETTE_CTR);  break;
//     case EFFECTS_FUNCTION_SEQUENTIAL_ID:  snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_SEQUENTIAL_NAME_CTR); break;
//     case EFFECTS_FUNCTION_SOLID_COLOUR_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_SOLID_COLOUR_NAME_CTR);  break;
//     case EFFECTS_FUNCTION_FIREPLACE_1D_01_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_FIREPLACE_1D_01_NAME_CTR);  break;

//     case EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01_NAME_CTR);  break;
//     case EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_WITH_AUGMENTED_TRANSITIONS_01_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_WITH_AUGMENTED_TRANSITIONS_01_NAME_CTR);  break;
//     case EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_CCT_TEMPERATURE_01_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_CCT_TEMPERATURE_01_NAME_CTR);  break;
  
  
//    case EFFECTS_FUNCTION_TESTER_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_TESTER_NAME_CTR);  break;

//   }
//   return buffer;
// }

//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/


// int8_t mAnimatorLight::GetFlasherRegionIDbyName(const char* c){
//   if(c=='\0') return -1;
//   if(     strcmp_P(c,PM_EFFECTS_REGION_COLOUR_SELECT_NAME_CTR)==0){ return EFFECTS_REGION_COLOUR_SELECT_ID; }
//   else if(strcmp_P(c,PM_EFFECTS_REGION_ANIMATE_NAME_CTR)==0){       return EFFECTS_REGION_ANIMATE_ID; }
//   return -1;
// }
// const char* mAnimatorLight::GetFlasherRegionName(char* buffer){
//   return GetFlasherRegionNamebyID(flashersettings.region, buffer);
// }
// const char* mAnimatorLight::GetFlasherRegionNamebyID(uint8_t id, char* buffer){
//   switch(id){
//     case EFFECTS_REGION_COLOUR_SELECT_ID:   memcpy_P(buffer,PM_EFFECTS_REGION_COLOUR_SELECT_NAME_CTR,sizeof(PM_EFFECTS_REGION_COLOUR_SELECT_NAME_CTR));
//     case EFFECTS_REGION_ANIMATE_ID:         memcpy_P(buffer,PM_EFFECTS_REGION_ANIMATE_NAME_CTR,sizeof(PM_EFFECTS_REGION_ANIMATE_NAME_CTR));
//   }
//   return buffer;
// }





//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/




// void mAnimatorLight::CommandSet_Flasher_UpdateColourRegion_RefreshSecs(uint8_t value){
  
//   flashersettings.update_colour_region.refresh_secs = value; 
  
//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_JSON_COLOUR_REFRESH_RATE)), flashersettings.update_colour_region.refresh_secs);
//   #endif // ENABLE_LOG_LEVEL_COMMANDS

// }

//   /********************************************************************************************************************************
//   ********** Palette_Generation_Randomise_Brightness_Enable ******************************************************************************************************
//   ********************************************************************************************************************************/


// void mAnimatorLight::CommandSet_Palette_Generation_Randomise_Brightness_Mode(uint8_t value){

//   //instead of simple on and off, it will contain a list of modes
  
//     // 0, 1, 2
//     // 0 = off, 1 = between range, 2 = switch between max and min randomly, 3 = switch between max and min alternatively 

//   pCONT_iLight->animation.flags.Apply_Upper_And_Lower_Brightness_Randomly_Ranged_To_Palette_Choice = value;
// pCONT_iLight->animation.flags.Limit_Upper_Brightness_With_BrightnessRGB = value; // phase out, as its probably not needed to have both methods
  
//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_PALETTE_GENERATION, D_JSON_RANDOMISE_BRIGHTNESS_MODE)), value);
//   #endif // ENABLE_LOG_LEVEL_COMMANDS

// }

// #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT


/******************************************************************************************************************************
*******************************************************************************************************************************
****************** HardwareColourType (RGB) *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mAnimatorLight::CommandSet_HardwareColourOrderTypeByStr(const char* c, uint8_t segment_index)
{

  if(!c){ return; }
  if(strlen(c)<=5){
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("Valid Length"));
  }else{
    AddLog(LOG_LEVEL_ERROR, PSTR("INVALID Length"));
    return;
  }

  _segments[segment_index].hardware_element_colour_order.red = D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE;
  _segments[segment_index].hardware_element_colour_order.green = D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE;
  _segments[segment_index].hardware_element_colour_order.blue = D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE;
  _segments[segment_index].hardware_element_colour_order.white_cold = D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE;
  _segments[segment_index].hardware_element_colour_order.white_warm = D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE;

  for(uint8_t index=0;index<strlen(c);index++)
  {
    // if(c[index]==0){ break; }
    if((c[index]=='R')||(c[index]=='r')){
      _segments[segment_index].hardware_element_colour_order.r = index;
    }else
    if((c[index]=='G')||(c[index]=='g')){
      _segments[segment_index].hardware_element_colour_order.g = index;
    }else
    if((c[index]=='B')||(c[index]=='b')){
      _segments[segment_index].hardware_element_colour_order.b = index;
    }else
    if((c[index]=='C')||(c[index]=='c')){
      _segments[segment_index].hardware_element_colour_order.c = index;
    }else
    if((c[index]=='W')||(c[index]=='w')){
      _segments[segment_index].hardware_element_colour_order.w = index;
    }
  }

  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR("segment_index%d == red=%d, green=%d, blue=%d, cold_white=%d, warm_white=%d"),
    segment_index,
    _segments[segment_index].hardware_element_colour_order.r,
    _segments[segment_index].hardware_element_colour_order.g,
    _segments[segment_index].hardware_element_colour_order.b,
    _segments[segment_index].hardware_element_colour_order.c,
    _segments[segment_index].hardware_element_colour_order.w
  );
  #endif

}



// const char* mAnimatorLight::GetHardwareColourTypeName(char* buffer, uint8_t buflen, uint8_t segment_index)
// {
//   return GetHardwareColourTypeNameByID(_segments[segment_index].mode_id, buffer, buflen, segment_index);
// }
// const char* mAnimatorLight::GetHardwareColourTypeNameByID(uint8_t id, char* buffer, uint8_t buflen, uint8_t segment_index){
//   sprintf(buffer, D_NO_MATCH_CTR);
//   // switch(id){
//   //   default:
//   //   case PIXEL_HARDWARE_COLOR_ORDER_GRB_ID: memcpy_P(buffer, PM_PIXEL_HARDWARE_COLOR_ORDER_GRB_CTR, sizeof(PM_PIXEL_HARDWARE_COLOR_ORDER_GRB_CTR)); break;
//   //   case PIXEL_HARDWARE_COLOR_ORDER_RGB_ID: memcpy_P(buffer, PM_PIXEL_HARDWARE_COLOR_ORDER_RGB_CTR, sizeof(PM_PIXEL_HARDWARE_COLOR_ORDER_RGB_CTR)); break;
//   //   case PIXEL_HARDWARE_COLOR_ORDER_BRG_ID: memcpy_P(buffer, PM_PIXEL_HARDWARE_COLOR_ORDER_BRG_CTR, sizeof(PM_PIXEL_HARDWARE_COLOR_ORDER_BRG_CTR)); break;
//   //   case PIXEL_HARDWARE_COLOR_ORDER_RBG_ID: memcpy_P(buffer, PM_PIXEL_HARDWARE_COLOR_ORDER_RBG_CTR, sizeof(PM_PIXEL_HARDWARE_COLOR_ORDER_RBG_CTR)); break;
//   // }
//   return buffer;
// }




//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/

// void mAnimatorLight::CommandSet_Flasher_Flags_Movement_Direction(uint8_t value){

//   flashersettings.flags.movement_direction = value;
//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_DIRECTION)), flashersettings.flags.movement_direction);
//   #endif // ENABLE_LOG_LEVEL_COMMANDS

// }

//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/

// void mAnimatorLight::CommandSet_Brightness_Min(uint8_t value){
  
//   flashersettings.brightness_min = value;
  
//     #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_MIN)), flashersettings.brightness_min);
//     #endif // ENABLE_LOG_LEVEL_COMMANDS

// }

//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/

// void mAnimatorLight::CommandSet_Brightness_Max(uint8_t value){
  
//   flashersettings.brightness_max = value;
  
//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_MAX)), flashersettings.brightness_max);
//   #endif // ENABLE_LOG_LEVEL_COMMANDS

// }

//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/

// void mAnimatorLight::CommandSet_Flasher_Alternate_Brightness_Min(uint8_t value){
  
//   shared_flasher_parameters.alternate_brightness_min = value;
  
//     #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_ALTERNATE_BRIGHTNESS_MIN)), shared_flasher_parameters.alternate_brightness_min);
//     #endif // ENABLE_LOG_LEVEL_COMMANDS

// }

//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/


// void mAnimatorLight::CommandSet_Flasher_Alternate_Brightness_Max(uint8_t value){
  
//   shared_flasher_parameters.alternate_brightness_max = value;
  
//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_ALTERNATE_BRIGHTNESS_MAX)), shared_flasher_parameters.alternate_brightness_max);
//   #endif // ENABLE_LOG_LEVEL_COMMANDS

// }


//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/

// void mAnimatorLight::CommandSet_Flasher_Alternate_RandomAmountPercentage(uint8_t value){
//     shared_flasher_parameters.alternate_random_amount_as_percentage = value;
//     #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_ALTERNATE D_JSON_RANDOM_AMOUNT)), shared_flasher_parameters.alternate_random_amount_as_percentage);
//     #endif // ENABLE_LOG_LEVEL_COMMANDS
// }

//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/


// void mAnimatorLight::CommandSet_Flasher_Flags_ApplySaturationRandomnessOnPaletteColours(uint8_t value){
//     pCONT_iLight->animation.flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique = value;
//     #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_AGED_COLOURING)), pCONT_iLight->animation.flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique);
//     #endif // ENABLE_LOG_LEVEL_COMMANDS
// }
//   #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT


//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/


// #ifdef ENABLE_PIXEL_FUNCTION_PIXELGROUPING
// void mAnimatorLight::CommandSet_PixelGrouping_Flag_AgedColouring(uint8_t value){
//     pixel_group.flags.fEnabled = value;
//     #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_PIXELGROUPING D_JSON_ENABLED)), pixel_group.flags.fEnabled);
//     #endif // ENABLE_LOG_LEVEL_COMMANDS
// }

//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/

// void mAnimatorLight::CommandSet_PixelGrouping_Flag_ModeID(uint8_t value){
//     pixel_group.flags.multiplier_mode_id = value;
//     #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_PIXELGROUPING D_JSON_MODE)), pixel_group.flags.multiplier_mode_id);
//     #endif // ENABLE_LOG_LEVEL_COMMANDS
// }
//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/


// void mAnimatorLight::CommandSet_PixelGrouping_Flag_Multiplier(uint8_t value){
//     pixel_group.multiplier = value;
//     #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_PIXELGROUPING,D_JSON_MULTIPLIER)), pixel_group.multiplier);
//     #endif // ENABLE_LOG_LEVEL_COMMANDS
// }

//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/

// void mAnimatorLight::CommandSet_PixelGrouping_MappedMultiplierData(uint8_t* value, uint8_t length){

//   memcpy(editable_mapped_array_data_array, value, sizeof(uint8_t)*length);

//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   for(uint8_t index=0;index<length;index++){
//     AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_JSON_MAPPED_MULTIPLIER_DATA " [i%d:v%d]"),index,editable_mapped_array_data_array[index]);
//   }
//   #endif// ENABLE_LOG_LEVEL_COMMANDS         
  
// }
// #endif // ENABLE_PIXEL_FUNCTION_PIXELGROUPING


//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/

//   #ifdef ENABLE_PIXEL_FUNCTION_MIXER
// //  void mAnimatorLight:: CommandSet_Mixer_Flags_Enabled
// //   if(jtok = obj[PM_JSON_MIXER].getObject()[PM_JSON_ENABLED]){ 
// //     mixer.flags.Enabled = jtok.getInt();
// //     #ifdef ENABLE_LOG_LEVEL_COMMANDS
// //     AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_MIXER D_JSON_ENABLED)), mixer.flags.Enabled);
// //     #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
// //   }
   
//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/

//   // if(jtok = obj[PM_JSON_MIXER].getObject()[PM_JSON_TIME_SCALER_AS_PERCENTAGE]){ 
//   //   mixer.time_scaler = jtok.getInt();
//   //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   //   AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_TIME_SCALER_AS_PERCENTAGE)), mixer.time_scaler);
//   //   #endif //#ifdef ENABLE_LOG_LEVEL_COMMANDS
//   // }

//   // if(jtok = obj[PM_JSON_MIXER].getObject()[PM_JSON_RUNTIME_DURATION_SCALER_PERCENTAGE]){
//   // void mAnimatorLight::  CommandSet_Mixer_RunTimeScalerPercentage 
//   //   mixer.run_time_duration_scaler_as_percentage = jtok.getInt();
//   //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   //   AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_MIXER D_JSON_RUNTIME_DURATION_SCALER_PERCENTAGE)), mixer.run_time_duration_scaler_as_percentage);
//   //   #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   // }
//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/


//   // if(jtok = obj[PM_JSON_MIXER].getObject()[PM_JSON_RUNNING_ID]){  
//   //   void mAnimatorLight::
//   //   CommandSet_Mixer_RunningID(jtok.getInt());
//   //   uint8_t val = jtok.getInt();
//   //   mixer.running_id = val;
//   //   LoadMixerGroupByID(val);
//   //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   //   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_RUNNING_ID)), val);
//   //   #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   // }
//   #endif //ENABLE_PIXEL_FUNCTION_MIXER


//   /********************************************************************************************************************************
//   **********Flasher Region ******************************************************************************************************
//   ********************************************************************************************************************************/


/******************************************************************************************************************************
*******************************************************************************************************************************
****************** PaletteID *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mAnimatorLight::CommandSet_PaletteID(uint8_t value, uint8_t segment_index)
{

  char buffer[50];

  _segments[segment_index].palette.id = value < mPalette::PALETTELIST_STATIC_LENGTH_ID ? value : 0;

  //If "id" is in the range of rgbcct, make sure to automatically make internal_rgbctt track it
  if((value>=mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_01_ID)
  &&(value<mPaletteI->PALETTELIST_VARIABLE_RGBCCT_LENGTH_ID))
  {
    CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(value, segment_index);
  }

  // #ifdef ENABLE_PIXEL_FUNCTION_MIXER
  // if(animation.mode_id == ANIMATION_MODE_EFFECTS_ID){
  //     pCONT_ladd->flashersettings.region = pCONT_ladd->EFFECTS_REGION_COLOUR_SELECT_ID; //update colours in use
  // }
  // #endif
  #ifdef ENABLE_PALETTE_FORCED_MODE
  //  animation.mode_id = ANIMATION_MODE_PRESETS_ID;
  #endif
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_COLOUR_PALETTE)), _segments[segment_index].palette.id);
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), mPaletteI->GetPaletteNameByID(_segments[segment_index].palette.id, buffer, sizeof(buffer)));
  #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS

}


/******************************************************************************************************************************
*******************************************************************************************************************************
****************** Animation_Transition_Time_Ms *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mAnimatorLight::CommandSet_Animation_Transition_Time_Ms(uint16_t value, uint8_t segment_index){
    
  _segments[segment_index].transition.time_ms = value;

  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_TIME_MS)),animation.transition.time_ms);  
  // #endif

}

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** Animation_Transition_Rate_Ms *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mAnimatorLight::CommandSet_Animation_Transition_Rate_Ms(uint16_t value, uint8_t segment_index){
    
  _segments[segment_index].transition.rate_ms = value;

  if(_segments[segment_index].transition.rate_ms<_segments[segment_index].transition.time_ms){ 
    _segments[segment_index].transition.time_ms = _segments[segment_index].transition.rate_ms;
  }

  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_RATE_MS)),animation.transition.time_ms);  
  // #endif

}


/******************************************************************************************************************************
*******************************************************************************************************************************
****************** TransitionOrder *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mAnimatorLight::CommandSet_TransitionOrderID(uint8_t value, uint8_t segment_index)
{
  _segments[segment_index].transition.order_id = value;

  char buffer[50];
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_SVALUE_K(D_JSON_TRANSITION,D_JSON_ORDER)), GetTransitionOrderName(buffer, sizeof(buffer)));
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}

const char* mAnimatorLight::GetTransitionOrderName(char* buffer, uint8_t buflen, uint8_t segment_index)
{
  return GetTransitionOrderNameByID(_segments[segment_index].transition.order_id, buffer, buflen);
}

const char* mAnimatorLight::GetTransitionOrderNameByID(uint8_t id, char* buffer, uint8_t buflen)
{
  switch(id){  default:    
    case TRANSITION_ORDER_NONE_ID:       memcpy_P(buffer, PM_TRANSITION_ORDER_NONE_NAME_CTR, sizeof(PM_TRANSITION_ORDER_NONE_NAME_CTR)); break;// smooth shift between them
    case TRANSITION_ORDER_RANDOM_ID:     memcpy_P(buffer, PM_TRANSITION_ORDER_RANDOM_NAME_CTR, sizeof(PM_TRANSITION_ORDER_RANDOM_NAME_CTR)); break;// smooth shift between them
    case TRANSITION_ORDER_INORDER_ID:    memcpy_P(buffer, PM_TRANSITION_ORDER_INORDER_NAME_CTR, sizeof(PM_TRANSITION_ORDER_INORDER_NAME_CTR)); break;// instant shift
    // case TRANSITION_ORDER_CENTRE_OUT_ID: memcpy_P(buffer, PM_TRANSITION_ORDER_CENTRE_OUT_NAME_CTR, sizeof(PM_TRANSITION_ORDER_CENTRE_OUT_NAME_CTR)); break;
    // case TRANSITION_ORDER_ROTATE_ID:     memcpy_P(buffer, PM_TRANSITION_ORDER_ROTATE_NAME_CTR, sizeof(PM_TRANSITION_ORDER_ROTATE_NAME_CTR)); break;
    // case TRANSITION_ORDER_FIXED_ID:      memcpy_P(buffer, PM_TRANSITION_ORDER_FIXED_NAME_CTR, sizeof(PM_TRANSITION_ORDER_FIXED_NAME_CTR)); break;// blend shift with random twinkles on random number of leds
    }
  return buffer;
}
int8_t mAnimatorLight::GetTransitionOrderIDbyName(const char* c)
{
  if(c=='\0'){ return -1; }
  if(strstr_P(c,PM_TRANSITION_ORDER_RANDOM_NAME_CTR)){
    return TRANSITION_ORDER_RANDOM_ID;
  }else if(strstr_P(c,PM_TRANSITION_ORDER_INORDER_NAME_CTR)){
    return TRANSITION_ORDER_INORDER_ID;
  }

  // if(strstr(c,D_TRANSITION_ORDER_RANDOM_NAME_CTR)){ return TRANSITION_ORDER_RANDOM_ID; }
  // if(strstr(c,D_TRANSITION_ORDER_CENTRE_OUT_NAME_CTR)){ return TRANSITION_ORDER_CENTRE_OUT_ID; }
  // if(strstr(c,D_TRANSITION_ORDER_INORDER_NAME_CTR)){ return TRANSITION_ORDER_INORDER_ID; }
  // if(strstr(c,D_TRANSITION_ORDER_FIXED_NAME_CTR)){ return TRANSITION_ORDER_FIXED_ID; }
  // if(strstr(c,D_TRANSITION_ORDER_ROTATE_NAME_CTR)){ return TRANSITION_ORDER_ROTATE_ID; }
  return -1;
}


/******************************************************************************************************************************
*******************************************************************************************************************************
****************** LightsCountToUpdateAsNumber DUPLICATED EFFECT 1/2*****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mAnimatorLight::CommandSet_LightsCountToUpdateAsNumber(uint16_t value, uint8_t segment_index)
{
  
  _segments[segment_index].transition.pixels_to_update_as_number = value;

  // animation.transition.pixels_to_update_as_percentage.val = GetPixelsToUpdateAsPercentageFromNumber(value);

  // Tmp fix until I merge them - UNTESTED HOW THIS WILL AFFECT CODE
  // strip_size_requiring_update = animation.transition.pixels_to_update_as_number ;
  // AddLog(LOG_LEVEL_WARN, PSTR("strip_size_requiring_update = animation.transition.pixels_to_update_as_number ; COULD CAUSE ERROR "));
  // SetLEDOutAmountByPercentage(animation.transition.pixels_to_update_as_number)

  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_NUMBER)), value);
  #endif

}


/******************************************************************************************************************************
*******************************************************************************************************************************
****************** LightsCountToUpdateAsPercentage DUPLICATED EFFECT 2/2*****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mAnimatorLight::CommandSet_LightsCountToUpdateAsPercentage(uint8_t value, uint8_t segment_index)
{
  
  _segments[segment_index].transition.pixels_to_update_as_number 
    = GetPixelsToUpdateAsPercentageFromNumber(value);
  // animation.transition.pixels_to_update_as_percentage = value;
  
  // TEMP FIX!! - UNTESTED HOW THIS WILL AFFECT CODE
  // AddLog(LOG_LEVEL_WARN, PSTR("strip_size_requiring_update = animation.transition.pixels_to_update_as_number COULD CAUSE ERROR "));
  // strip_size_requiring_update = animation.transition.pixels_to_update_as_number ;


  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE)), value);
  #endif

}

uint16_t mAnimatorLight::GetPixelsToUpdateAsNumberFromPercentage(uint8_t percentage, uint8_t segment_index)
{
  return mapvalue(percentage, 0,100, 0, _segments[segment_index].length());
}
uint8_t mAnimatorLight::GetPixelsToUpdateAsPercentageFromNumber(uint16_t number, uint8_t segment_index)
{
  return mapvalue(number ,0,_segments[segment_index].length(), 0,100);
}


/******************************************************************************************************************************
*******************************************************************************************************************************
****************** RGbcct Controller options from iLight *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/
/******************************************************************************************************************************
*******************************************************************************************************************************
******************  *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/
/******************************************************************************************************************************
*******************************************************************************************************************************
******************  *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/
/******************************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/
/******************************************************************************************************************************
*******************************************************************************************************************************
***************** *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** ActiveRgbcctColourPaletteIDUsedAsScene *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mAnimatorLight::CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(uint8_t palette_id, uint8_t segment_index)
{

  uint8_t palette_id_adjusted_to_array_index = palette_id - mPaletteI->PALETTELIST_VARIABLE_HSBID_LENGTH_ID;  
  
  // mPaletteI->active_scene_palette_id = palette_id; // not used?

/**
 * Temp conversion fix, set the original pointers in ilight
 * 
 */

#ifndef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
                   pCONT_iLight->active_rgbcct_colour_p = reinterpret_cast<RgbcctColor*>(&pCONT_set->Settings.animation_settings.palette_rgbcct_users_colour_map[5*palette_id_adjusted_to_array_index]); // use first for now

#endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

_segment_runtimes[segment_index].active_rgbcct_colour_p = reinterpret_cast<RgbcctColor*>(&pCONT_set->Settings.animation_settings.palette_rgbcct_users_colour_map[5*palette_id_adjusted_to_array_index]); // use first for now
  
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT "CommandSet_ActiveRgbcctColourPalette(%d) as %d"),palette_id,palette_id_adjusted_to_array_index);
}


/******************************************************************************************************************************
*******************************************************************************************************************************
****************** Hue *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mAnimatorLight::CommandSet_ActiveSolidPalette_Hue_360(uint16_t hue_new, uint8_t segment_index)
{
  _segment_runtimes[segment_index].rgbcct_controller->setHue360(hue_new);
  _segments[segment_index].flags.fForceUpdate = true;
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT "SEGEMTNS" D_JSON_COMMAND_NVALUE_K(D_JSON_HUE)), _segment_runtimes[segment_index].rgbcct_controller->getHue360());
  #endif // ENABLE_LOG_LEVEL_COMMANDS
  
  RgbcctColor c = _segment_runtimes[segment_index].rgbcct_controller->GetColourFullRange();
  AddLog(LOG_LEVEL_TEST, PSTR("desired_colour=%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WC,c.WW);
  
  #ifdef USE_MODULE_NETWORK_MQTT
  pCONT_lAni->mqtthandler_flasher_teleperiod.flags.SendNow = true;
  #endif // USE_MODULE_NETWORK_MQTT

}


/******************************************************************************************************************************
*******************************************************************************************************************************
****************** Sat *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mAnimatorLight::CommandSet_ActiveSolidPalette_Sat_255(uint8_t sat_new, uint8_t segment_index)
{
  _segment_runtimes[segment_index].rgbcct_controller->setSat255(sat_new);
  _segments[segment_index].flags.fForceUpdate = true;
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_SAT)), _segment_runtimes[segment_index].rgbcct_controller->getSat255());
  #endif // ENABLE_LOG_LEVEL_COMMANDS
}

// /******************************************************************************************************************************
// *******************************************************************************************************************************
// ****************** Brt *****************************************************************************************
// *******************************************************************************************************************************
// *******************************************************************************************************************************/

// void mInterfaceLight::CommandSet_Brt_255(uint8_t brt_new, uint8_t segment_index)
// {
//   rgbcct_controller.setBrightness255(brt_new);
// #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
//   animation.flags.fForceUpdate = true;
// #else
//   pCONT_lAni->_segments[0].flags.fForceUpdate = true;
// #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
//   setBriRGB_Global(brt_new);
//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS)), rgbcct_controller.getBrightness255());
//   #endif // ENABLE_LOG_LEVEL_COMMANDS
// }

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** BrtRGB *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mAnimatorLight::CommandSet_BrtRGB_255(uint8_t bri, uint8_t segment_index)
{
  _segment_runtimes[segment_index].rgbcct_controller->setBrightnessRGB255(bri);
  _segments[segment_index].flags.fForceUpdate = true;

  pCONT_iLight->_briRGB_Global = bri;
  pCONT_iLight->setBriRGB_Global(bri);
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT "SEGMENTS" D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS)), _segment_runtimes[segment_index].rgbcct_controller->getBrightnessRGB255());
  #endif // ENABLE_LOG_LEVEL_COMMANDS
}

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** BrtCCT *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mAnimatorLight::CommandSet_BrtCT_255(uint8_t bri, uint8_t segment_index) {
  _segment_runtimes[segment_index].rgbcct_controller->setBrightnessCCT255(bri);
  // pCONT_iLight->_briCT_Global = bri;
  pCONT_lAni->_segments[segment_index].flags.fForceUpdate = true;
  
  pCONT_iLight->setBriCT_Global(bri);
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_CCT)), _segment_runtimes[segment_index].rgbcct_controller->getBrightnessCCT255());
  #endif // ENABLE_LOG_LEVEL_COMMANDS
}


/******************************************************************************************************************************
*******************************************************************************************************************************
****************** BrtCCT *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mAnimatorLight::CommandSet_ActiveSolidPalette_ColourTemp(uint16_t ct, uint8_t segment_index) {
  _segment_runtimes[segment_index].rgbcct_controller->setCCT(ct);
  _segments[segment_index].flags.fForceUpdate = true;

  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_CCT_TEMP)), _segment_runtimes[segment_index].rgbcct_controller->getCCT());
  #endif // ENABLE_LOG_LEVEL_COMMANDS
}

// /**
//  * direct percentage version
//  * */
// void mInterfaceLight::CommandSet_ActiveSolidPalette_ColourTemp_Percentage(uint8_t percentage) {

//   CommandSet_ActiveSolidPalette_ColourTemp(mapvalue(percentage, 0,100, _ct_min_range,_ct_max_range));

//   // rgbcct_controller.setCCT(ct);
//   // animation.flags.fForceUpdate = true;
//   // #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_CCT_TEMP)), rgbcct_controller.getCCT());
//   // #endif // ENABLE_LOG_LEVEL_COMMANDS
// }

// /**
//  * "InternalSet" ie direct control, does not have Addlog feedback like commandset
//  * */
// void mInterfaceLight::InternalSet_ActiveSolidPalette_ColourTemp(uint16_t ct) {
//   rgbcct_controller.setCCT(ct);
// #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
//   animation.flags.fForceUpdate = true;
// #else
//   pCONT_lAni->_segments[0].flags.fForceUpdate = true;
// #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
// }

// /******************************************************************************************************************************
// *******************************************************************************************************************************
// ****************** CommandSet_RGBCT_Linked *****************************************************************************************
// *******************************************************************************************************************************
// *******************************************************************************************************************************/

// bool mInterfaceLight::CommandSet_ActiveSolidPalette_RGBCT_Linked(uint16_t ct_rgb_linked) {
//   rgbcct_controller.setRGBCCTLinked(ct_rgb_linked);
// #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
//   animation.flags.fForceUpdate = true;
// #else
//   pCONT_lAni->_segments[0].flags.fForceUpdate = true;
// #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_RGBCCT_LINKED)), rgbcct_controller.getRGBCCTLinked());
//   #endif // ENABLE_LOG_LEVEL_COMMANDS
// }





// /******************************************************************************************************************************
// *******************************************************************************************************************************
// ****************** ActiveSolidPalette *****************************************************************************************
// *******************************************************************************************************************************
// *******************************************************************************************************************************/

// void mInterfaceLight::CommandSet_ActiveSolidPalette_Raw(uint8_t r,uint8_t g,uint8_t b,uint8_t ww,uint8_t wc){
//   rgbcct_controller.setChannelsRaw(r,g,b,ww,wc);  
// #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
//   animation.flags.fForceUpdate = true;
// #else
//   pCONT_lAni->_segments[0].flags.fForceUpdate = true;
// #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
//   // #ifdef ENABLE_LOG_LEVEL_INFO
//   // char buffer[30];
//   // snprintf_P(buffer, sizeof(buffer), PSTR("[%d,%d,%d,%d,%d]"),values[0],values[1],values[2],values[3],values[4]);
//   // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_SVALUE_K(D_JSON_SCENE,D_JSON_COLOUR)), buffer);
//   // #endif // ENABLE_LOG_LEVEL_DEBUG
// }

// void mInterfaceLight::CommandSet_ActiveSolidPalette_Raw(uint8_t* values){
//   rgbcct_controller.setChannelsRaw(values);    
// #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
//   animation.flags.fForceUpdate = true;
// #else
//   pCONT_lAni->_segments[0].flags.fForceUpdate = true;
// #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
//   #ifdef ENABLE_LOG_LEVEL_INFO
//   char buffer[30];
//   snprintf_P(buffer, sizeof(buffer), PSTR("[%d,%d,%d,%d,%d]"),values[0],values[1],values[2],values[3],values[4]);
//   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_SVALUE_K(D_JSON_SCENE,D_JSON_COLOUR)), buffer);
//   #endif // ENABLE_LOG_LEVEL_DEBUG
// }


#endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT


#endif //USE_MODULE_LIGHTS_ANIMATOR




