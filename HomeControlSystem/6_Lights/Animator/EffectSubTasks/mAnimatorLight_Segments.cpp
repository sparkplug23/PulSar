#include "../mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

#ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

uint16_t mAnimatorLight::_usedSegmentData = 0;


void mAnimatorLight::Init_Segments()
{
  
  resetSegments();


  // _segment_runtimes[0].allocateData(10);
  // _segment_runtimes[0].data[0] = 1;

  // _segment_runtimes[1].allocateData(10);
  // if(_segment_runtimes[1].data != nullptr)
  // {
  //   _segment_runtimes[1].data[0] = 2;
  // }

  // PALETTELIST_STATIC_CHRISTMAS_01_ID
  /**
   * Init the Rgbcct palettes
   * */
  pCONT_iLight->CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_01_ID);
  pCONT_iLight->CommandSet_ActiveSolidPalette_Hue_360(25);
  pCONT_iLight->CommandSet_ActiveSolidPalette_Sat_255(map(90, 0,100, 0,255));

  _segments[0].palette.id = mPaletteI->PALETTELIST_STATIC_CHRISTMAS_03_ID;
  _segments[1].palette.id = mPaletteI->PALETTELIST_STATIC_CHRISTMAS_01_ID;
  _segments[2].palette.id = mPaletteI->PALETTELIST_STATIC_CHRISTMAS_02_ID;
  _segments[3].palette.id = mPaletteI->PALETTELIST_STATIC_SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01_ID;
  _segments[4].palette.id = mPaletteI->PALETTELIST_STATIC_SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_04_ID
  ;

  _segments[0].transition.rate_ms = 10000;
  _segments[1].transition.rate_ms = 10000;
  _segments[2].transition.rate_ms = 10000;
  _segments[3].transition.rate_ms = 1000;
  _segments[4].transition.rate_ms = 1000;

  _segments[0].transition.time_ms = 3000;
  _segments[1].transition.time_ms = 900;
  _segments[2].transition.time_ms = 900;
  _segments[3].transition.time_ms = 0;
  _segments[4].transition.time_ms = 0;

  _segments[0].transition.order_id = TRANSITION_ORDER_INORDER_ID;
  _segments[1].transition.order_id = TRANSITION_ORDER_INORDER_ID;
  _segments[2].transition.order_id = TRANSITION_ORDER_INORDER_ID;
  _segments[3].transition.order_id = TRANSITION_ORDER_RANDOM_ID;
  _segments[4].transition.order_id = TRANSITION_ORDER_RANDOM_ID;

  _segments[0].effect_id = EFFECTS_FUNCTION_SLOW_GLOW_ID;
  _segments[1].effect_id = EFFECTS_FUNCTION_STEP_THROUGH_PALETTE_ID;
  _segments[2].effect_id = EFFECTS_FUNCTION_STATIC_PALETTE_ID;
  _segments[3].effect_id = EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01_ID;
  _segments[4].effect_id = EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01_ID;


  _segment_runtimes[0].aux0 = EFFECTS_REGION_COLOUR_SELECT_ID;
  _segment_runtimes[1].aux0 = EFFECTS_REGION_COLOUR_SELECT_ID;
  _segment_runtimes[2].aux0 = EFFECTS_REGION_COLOUR_SELECT_ID;
  _segment_runtimes[3].aux0 = EFFECTS_REGION_COLOUR_SELECT_ID;
  _segment_runtimes[4].aux0 = EFFECTS_REGION_COLOUR_SELECT_ID;


}


void mAnimatorLight::resetSegments() 
{

  bool supportWhite=0;
  uint16_t countPixels=50;
  // bool skipFirst=0;
  // mEffects->init(0, ledCount, 0);

  // if (supportWhite == _useRgbw && countPixels == _length && _skipFirstMode == skipFirst) return;
  // RESET_RUNTIME;

  _useRgbw = supportWhite;
  _length = countPixels;
  // _skipFirstMode = skipFirst;

  uint8_t ty = 1;
  if (supportWhite) ty = 2;
  _lengthRaw = _length;
  // if (_skipFirstMode) {
  //   _lengthRaw += LED_SKIP_AMOUNT;
  // }


  // mainSegment = 0;
  // memset(_segments, 0, sizeof(_segments));
  // memset(_segment_runtimes, 0, sizeof(_segment_runtimes));
  // segment_iters._segment_index = 0;
  // _segments[0].mode = DEFAULT_MODE;
  // _segments[0].colors[0] = DEFAULT_COLOR;
  // _segments[0].pixel_range.start = 0;
  // _segments[0].speed = DEFAULT_SPEED;
  // _segments[0].pixel_range.stop = _length;
  // _segments[0].grouping = 1;
  // _segments[0].setOption(SEG_OPTION_SELECTED, 1);
  // _segments[0].setOption(SEG_OPTION_ON, 1);
  // _segments[0].opacity = 255;
  // _segments[0].intensity = 127;
  // _segment_runtimes[0].reset();

  
  for (uint16_t i = 1; i < MAX_NUM_SEGMENTS; i++)
  {
    _segments[i].hardware_element_colour_order.red = 0;
    _segments[i].hardware_element_colour_order.green = 1;
    _segments[i].hardware_element_colour_order.blue = 2;
    _segments[i].hardware_element_colour_order.white_cold = 3;
    _segments[i].hardware_element_colour_order.white_warm = 4;
  }


  for (uint16_t i = 1; i < MAX_NUM_SEGMENTS; i++)
  {
    // _segments[i].colors[0] = 1000;//color_wheel(i*51);
    _segments[i].grouping = 1;
    // _segments[i].setOption(SEG_OPTION_ON, 1);
    // _segments[i].opacity = 255;
    _segment_runtimes[i].reset();
    // _segment_runtimes[0].reset();
  }
  
  // _segments[0].pixel_range.start = 0;
  // _segments[0].pixel_range.stop = 9;
  // _segments[1].pixel_range.start = 10;
  // _segments[1].pixel_range.stop = 19;
  // _segments[2].pixel_range.start = 20;
  // _segments[2].pixel_range.stop = 29;
  // _segments[3].pixel_range.start = 30;
  // _segments[3].pixel_range.stop = 39;
  // _segments[4].pixel_range.start = 40;
  // _segments[4].pixel_range.stop = 49;

  _segments[0].pixel_range.start = 0;
  _segments[0].pixel_range.stop = 49;
  // _segments[1].pixel_range.start = 10;
  // _segments[1].pixel_range.stop = 19;
  // _segments[2].pixel_range.start = 20;
  // _segments[2].pixel_range.stop = 29;
  // _segments[3].pixel_range.start = 30;
  // _segments[3].pixel_range.stop = 39;
  // _segments[4].pixel_range.start = 40;
  // _segments[4].pixel_range.stop = 49;


  _segment_runtimes[0].animator = new NeoPixelAnimator(1, NEO_MILLISECONDS);
  _segment_runtimes[1].animator = new NeoPixelAnimator(1, NEO_MILLISECONDS);
  _segment_runtimes[2].animator = new NeoPixelAnimator(1, NEO_MILLISECONDS);
  _segment_runtimes[3].animator = new NeoPixelAnimator(1, NEO_MILLISECONDS);
  _segment_runtimes[4].animator = new NeoPixelAnimator(1, NEO_MILLISECONDS);

Serial.println("segemnted booted");

Serial.println("segemnted booted");

Serial.println("segemnted booted");

// Serial.println("segemnted booted");

// Serial.println("segemnted booted");

// Serial.println("segemnted booted");

// Serial.println("segemnted booted");

// Serial.println("segemnted booted");


}



/***
 * Test 1: Apply 3 static colours between 0-4, 5-9 and 10 to 14
 * 
 * */
void mAnimatorLight::SubTask_Segments_Animation()
{

// char buffer[100];
  // /**
  //  * Timer (seconds) to update the EFFECTS_REGION_COLOUR_SELECT_ID when otherwise remains static
  //  * */
  // SubTask_Flasher_Animate_Parameter_Check_Update_Timer_Change_Colour_Region();
  // /**
  //  * Timer (seconds) to randomise the rates of change. Methods include (random between range, linear map, exponential (longer slow), exponential (longer fast))
  //  * */  
  // SubTask_Flasher_Animate_Parameter_Check_Update_Timer_Transition_Rate();
  // /**
  //  * Timer (seconds) to randomise the transition time of change. Methods include (random between range, linear map, exponential (longer slow), exponential (longer fast))
  //  * */    
  // SubTask_Flasher_Animate_Parameter_Check_Update_Timer_Transition_Time();


  // if((mTime::TimeReached(&flashersettings.tSaved.Update,_segments[0].transition.rate_ms))||(_segments[0].flags.fForceUpdate))
  // {

  //   if(_segments[0].flags.fForceUpdate){ _segments[0].flags.fForceUpdate=false;
  //     flashersettings.tSaved.Update = millis();
  //   }
    // #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
    // char buffer[100];
    // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "flashersettings.function=%d %s"),flashersettings.function,GetFlasherFunctionNamebyID(flashersettings.function, buffer));
    // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "flashersettings.function=%d"),flashersettings.function);
    //#endif
    // #ifdef DEVICE_RGBFIREPLACE_TESTER
    //     flashersettings.function = EFFECTS_FUNCTION_SLOW_GLOW_ID;
    // #endif

  
  /**
   * Update animation "rate"
   * 
   * Test1: Apply slow glow on segment 0-50%, apply random solid to 50-100%
   * */
  
    /**
     * Subtask to handle by each segment ie "flasher" function for each segment
     * */
    for(
      segment_iters._segment_index = 0; 
      segment_iters._segment_index<MAX_NUM_SEGMENTS; 
      segment_iters._segment_index++
    ){
      // if(mTime::TimeReached(&tSaved_Test_Segment_Animation, 2000))
      // {
        
      if(_segments[segment_iters._segment_index].isActive())
      {


        if((mTime::TimeReached(&_segments[segment_iters._segment_index].tSaved_AnimateRunTime, _segments[segment_iters._segment_index].transition.rate_ms))||(_segments[0].flags.fForceUpdate))
        {

          if(_segments[0].flags.fForceUpdate){ _segments[0].flags.fForceUpdate=false;
            _segments[segment_iters._segment_index].tSaved_AnimateRunTime = millis();
          }
          
        // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "flashersettings.function=segI%d %d %s"),segment_iters._segment_index, flashersettings_segments.function,GetFlasherFunctionNamebyID(flashersettings_segments.function, buffer));

        // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "flashersettings.function=segI%d %d %d"),segment_iters._segment_index, _segments[segment_iters._segment_index].effect_id);
        // DEBUG_LINE_HERE;

      
        switch(_segments[segment_iters._segment_index].effect_id){
          default:
          case EFFECTS_FUNCTION_SOLID_COLOUR_ID:
            SubTask_Segment_Animate_Function__Solid_Static_Single_Colour();
          break;
          case EFFECTS_FUNCTION_STATIC_PALETTE_ID:
            SubTask_Segment_Animate_Function__Static_Palette();
          break;
          case EFFECTS_FUNCTION_SLOW_GLOW_ID:
            SubTask_Segment_Animate_Function__Slow_Glow();
          break;
          case EFFECTS_FUNCTION_STEP_THROUGH_PALETTE_ID:
            SubTask_Segment_Animate_Function__Step_Through_Palette();
          break;
          case EFFECTS_FUNCTION_SEQUENTIAL_ID:
            SubTask_Segment_Flasher_Animate_Function__Sequential();
          break;
          case EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01_ID:
            SubTask_Segment_Animate_Function__SunPositions_Elevation_Only_RGBCCT_Palette_Indexed_Positions_01();
          break;
          case EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_CCT_TEMPERATURE_01_ID:
            SubTask_Segment_Animate_Function__SunPositions_Elevation_Only_Controlled_CCT_Temperature_01();
          break;        
          #ifdef ENABLE_DEVFEATURE_RGB_CLOCK
          case EFFECTS_FUNCTION_LCD_CLOCK_BASIC_01_ID:
            SubTask_Flasher_Animate_LCD_Clock_Time_Basic_01();
          break;
          // case EFFECTS_FUNCTION_LCD_CLOCK_BASIC_02_ID:
          //   SubTask_Flasher_Animate_LCD_Clock_Time_Basic_02();
          // break;
          // case EFFECTS_FUNCTION_LCD_DISPLAY_BASIC_01_ID:
          //   SubTask_Flasher_Animate_LCD_Display_Show_Numbers_Basic_01();
          // break;
          #endif // ENABLE_DEVFEATURE_RGB_CLOCK
          
          #ifdef ENABLE_DEVFEATURE_WLED_CONVERTED_TO_SEGMENTS
          case EFFECTS_FUNCTION_WLED_CANDLE_SINGLE_ID:
            SubTask_Segment_Flasher_Animate_Function__Candle_Single();
          break;     
          case EFFECTS_FUNCTION_WLED_CANDLE_MULTI_ID:
            SubTask_Segment_Flasher_Animate_Function__Candle_Multi();
          break;     

          #endif // ENABLE_DEVFEATURE_WLED_CONVERTED_TO_SEGMENTS

          //  case EFFECTS_FUNCTION_SLOW_GLOW_ON_BRIGHTNESS_ID:
          //    SubTask_Flasher_Animate_Function_Slow_Glow_On_Brightness();
          //  break;
          //  case EFFECTS_FUNCTION_FLASH_TWINKLE_SINGLE_COLOUR_RANDOM_ID:
          //    SubTask_Flasher_Animate_Function_Twinkle_SingleColour_Random();
          //  break;
          //  case EFFECTS_FUNCTION_FLASH_TWINKLE_PALETTE_COLOUR_RANDOM_ID:
          //    SubTask_Flasher_Animate_Function_Twinkle_PaletteColour_Random();
          //  break;
          //  case EFFECTS_FUNCTION_SLOW_FADE_BRIGHTNESS_ALL_ID:
          //    SubTask_Flasher_Animate_Function_Slow_Fade_Brightness_All();
          //  break;
          //  case EFFECTS_FUNCTION_SLOW_FADE_SATURATION_ALL_ID:
          //    SubTask_Flasher_Animate_Function_Slow_Fade_Saturation_All();
          //  break;
          //  case EFFECTS_FUNCTION_FADE_GRADIENT_ID:
          //    SubTask_Flasher_Animate_Function_Fade_Gradient();
          //  break;
          //  case EFFECTS_FUNCTION_PULSE_RANDOM_ON:
          //    SubTask_Flasher_Animate_Function_Pulse_Random_On();
          //  break;
          //  case EFFECTS_FUNCTION_PULSE_RANDOM_ON_TWO_ID:
          //    SubTask_Flasher_Animate_Function_Pulse_Random_On_2();
          //  break;
          // EFFECTS_FUNCTION_PULSE_RANDOM_ON_FADE_OFF_ID
          //  case EFFECTS_FUNCTION_PULSE_RANDOM_ON_FADE_OFF_ID:
          //    SubTask_Flasher_Animate_Function_Pulse_Random_On_Fade_Off();
          //  break;
          //  case EFFECTS_FUNCTION_POPPING_PALETTE_BRIGHTNESS_FROM_LOWER_TO_UPPER_BOUNDERY:
          //    SubTask_Flasher_Animate_Function_Popping_Palette_Brightness_From_Lower_To_Upper_Boundary();
          //  break;
          //  case EFFECTS_FUNCTION_TWINKLE_PALETTE_BRIGHTNESS_FROM_LOWER_TO_UPPER_AND_BACK:
          //    SubTask_Flasher_Animate_Function_Twinkle_Palette_Brightness_From_Lower_To_Upper_And_Back();
          //  break;
          //  case EFFECTS_FUNCTION_SUNPOSITIONS_SUNRISE_ALARM_01:
          //    SubTask_Flasher_Animate_Function_SunPositions_SunRise_Alarm_01();
          //  break;
          //  case EFFECTS_FUNCTION_SUNPOSITIONS_GRADIENT_SUN_ELEVATION_AND_AZIMUTH_01:
          //    SubTask_Flasher_Animate_Function_SunPositions_Gradient_Sun_Elevation_And_Azimuth_01();
          //  break;
          //  case EFFECTS_FUNCTION_SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_01_ID:
          //    SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_01();
          //  break;
          //  case EFFECTS_FUNCTION_SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_02_ID:
          //    SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_02();
          //  break;
          //  case EFFECTS_FUNCTION_SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_03_ID:
          //    SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_03();
          //  break;
          //  case EFFECTS_FUNCTION_SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_04_ID:
          //    SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_04();
          //  break;
          //  case EFFECTS_FUNCTION_SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_05_ID:
          //    SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_05();
          //  break;
          //  case EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_WITH_AUGMENTED_TRANSITIONS_01_ID:
          //    SubTask_Flasher_Animate_Function_SunPositions_Elevation_Only_RGBCCT_Palette_Indexed_Positions_With_Augmented_01();
          //  break;
          case EFFECTS_FUNCTION_TESTER_ID:
            SubTask_Flasher_Animate_Function_Tester();
          break; 
        } //end SWITCH

        // Configure animator to show output
        StartSegmentAnimation_AsAnimUpdateMemberFunction(segment_iters._segment_index);                

      }//end if update reached

      } //  if(_segments[segment_iters._segment_index].isActive())


   _segments[0].flags.animator_first_run = false;


  
  }


} // SubTask_Effects_PhaseOut



/**
 * Cleaned version for segments
 * */
void mAnimatorLight::EveryLoop(){

  
// _segments[0].effect_id = EFFECTS_FUNCTION_SEQUENTIAL_ID;
// _segments[1].effect_id = EFFECTS_FUNCTION_STEP_THROUGH_PALETTE_ID;
// _segments[2].effect_id = EFFECTS_FUNCTION_STATIC_PALETTE_ID;
// _segments[3].effect_id = EFFECTS_FUNCTION_STEP_THROUGH_PALETTE_ID;
// _segments[4].effect_id = EFFECTS_FUNCTION_STATIC_PALETTE_ID;


  if(_segments[0].flags.fEnable_Animation)
  {

    uint8_t flag_animations_needing_updated = 0;

    /**
     * Each ACTIVE segment will have its own animator to run
     * */
    for (segment_iters._segment_index =0;segment_iters._segment_index<MAX_NUM_SEGMENTS;segment_iters._segment_index++)
    {

      if(_segments[segment_iters._segment_index].isActive())
      {

        if (_segment_runtimes[segment_iters._segment_index].animator->IsAnimating())
        {
          // for now, a global segment_index needs set before each animation update

          // DEBUG_LINE_HERE;
          _segment_runtimes[segment_iters._segment_index].animator->UpdateAnimations(segment_iters._segment_index);
          // DEBUG_LINE_HERE;
          flag_animations_needing_updated++; // channels needing updated


          //TBA: flags per segment

        }
        else // not animating
        { 
          //TBA: flags per segment
      
        }

      }//isactive

    } //loop segments

    if(flag_animations_needing_updated)
    {
      if(stripbus->IsDirty()){
        if(stripbus->CanShow()){ 
          StripUpdate();
        }
      }
      
      /**
       * THESE ALL NEED PUT INTO SEGMENTS
       * THESE ARE COMMANDS IF ANY IS ANIMATING
       * checking if animation state has changed from before (maybe check via animation state?)
       * */
      if(!_segments[0].flags.fRunning)
      {   
        //#ifdef ENABLE_LOG_LEVEL_DEBUG
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "Animation Started"));
        //#endif
      }
      _segments[0].flags.fRunning = true; 
      fPixelsUpdated = true;
      pCONT_set->Settings.enable_sleep = false;    //Make this a function, pause sleep during animations
      //AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "~a"));
    }
    else // none need animating
    {
      if(_segments[0].flags.fRunning)
      { // Was running
        #ifdef ENABLE_LOG_LEVEL_DEBUG
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "Animation Finished")); 
        #endif
        fAnyLEDsOnOffCount = 0;
        for(int i=0;i<pCONT_iLight->settings.light_size_count;i++){ 
          if(GetPixelColor(i)!=0){ fAnyLEDsOnOffCount++; }
        }          
      }
      _segments[0].flags.fRunning = false;
      pCONT_set->Settings.enable_sleep = true;
      if(blocking_force_animate_to_complete){ //if it was running
        #ifdef ENABLE_LOG_LEVEL_DEBUG
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "Animation blocking_force_animate_to_complete"));
        #endif
        blocking_force_animate_to_complete = false;
      }
    }
  } //enabeled


  // #ifndef ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT
  // // scene.colour.B = _segments[0].brightness; // _segments[0].brightness is master
  // // move to have tasmota way to update brightness stored
  // #endif //ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT

  /**
   *  control relay power
   * */
  // //(pCONT_iLight->light_power_state)
  //   if(pCONT_iLight->light_power_state!=pCONT_iLight->light_power_Saved){
  //     pCONT_iLight->light_power_Saved = pCONT_iLight->light_power_state;
  //     // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "pCONT_iLight->light_power_Saved"));
  //     //pCONT->mry->CommandSet_Relay_Power(0,pCONT_iLight->light_power_state);
  //   }

  //   //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "file%sline%d"),__FILE__,__LINE__);
  //   DEBUG_LINE;

/**
 * Check any lights are on
 * */
  // if(mTime::TimeReached(&tSavedCheckAnyLEDsOnOff,1000)){
  //   fAnyLEDsOnOffCount = 0;
  //   for(int i=0;i<pCONT_iLight->settings.light_size_count;i++){
  //     #ifdef ENABLE_LOG_LEVEL_DEBUG
  //     AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "fAnyLEDsOnOffCount %d"),fAnyLEDsOnOffCount);
  //     #endif
  //     if(GetPixelColor(i)!=0){
  //       fAnyLEDsOnOffCount++;
  //     }
  //   }
  //   if(fAnyLEDsOnOffCount>0){
  //     #ifdef ENABLE_LOG_LEVEL_DEBUG
  //     AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "fAnyLEDsOnOffCount %d"),fAnyLEDsOnOffCount);
  //     #endif
  //   }
  //   // #ifdef USE_MODULE_DRIVERS_RELAY
  //   //   //pCONT->mry->CommandSet_Relay_Power(0,fAnyLEDsOnOffCount>0?1:0);
  //   // #endif
  // }
  // DEBUG_LINE;
// 

}


mAnimatorLight& mAnimatorLight::setAnimFunctionCallback_Segments_Indexed(uint8_t segment_index, ANIM_FUNCTION_SIGNATURE)
{
  // AddLog(LOG_LEVEL_INFO, PSTR("setAnimFunctionCallback_Segments_Indexed segment_index=%d"));
  this->_segment_runtimes[segment_index].anim_function_callback = anim_function_callback;
  return *this;
}



void mAnimatorLight::Segments_UpdateStartingColourWithGetPixel()
{
  
  uint16_t start_pixel = _segments[segment_iters._segment_index].pixel_range.start;
  uint16_t end_pixel = _segments[segment_iters._segment_index].pixel_range.stop;
  for (uint16_t pixel = start_pixel; pixel <= end_pixel; pixel++)
  {
    animation_colours[pixel].StartingColor = GetPixelColor(pixel);
  }

}


/**
 * Begin at "StartingColor" at 0% then return to "DesiredColor" at 100%
 * */
void mAnimatorLight::StartSegmentAnimation_AsAnimUpdateMemberFunction(uint8_t segment_index)
{ 
  
  #ifdef ENABLE_LOG_LEVEL_DEBUG
  //AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::StartAnimationAsBlendFromStartingColorToDesiredColor"));
  #endif

  // pCONT_iLight->runtime.animation_changed_millis = millis();

  uint16_t time_tmp = 0;    
  time_tmp = _segments[segment_index].transition.time_ms; 

  // // Overwriting single _segments[0] methods, set, then clear
  // if(_segments[0]_override.time_ms){
  //   time_tmp = _segments[0]_override.time_ms;
  //   _segments[0]_override.time_ms = 0;//reset overwrite
  // }
  // //clear forced once only flags
  // if(_segments[0].flags.NewAnimationRequiringCompleteRefresh){
  //   _segments[0].flags.NewAnimationRequiringCompleteRefresh = false;    
  // }
  
  //   // Serial.printf("TRANSITION_METHOD_INSTANT_ID = %d\n\r",time_tmp);

  // if(time_tmp>0){
  //   if(NEO_ANIMATION_TIMEBASE == NEO_CENTISECONDS){
  //     time_tmp /= 1000;// ms to seconds
  //   }
  // }

  if(_segment_runtimes[segment_index].anim_function_callback)
  {
    _segment_runtimes[segment_index].animator->StartAnimation(0, time_tmp, _segment_runtimes[segment_index].anim_function_callback);
  }


} //end function


/**
 * This needs redoing, with a flag, to enable how I want the palette presented, and not how its encoded
 * "palette_pattern"
 * Gradient (either needs to get from palette, or equally generate it based on palette element count)
 * Single   (gets each colour in the palette, with ability to ignore indexs if they are present)
 * 
 * NEW: Segments
 * 
 * Updated method to try work with new segments, including index ranges
 * 
 * */
void mAnimatorLight::Segments_UpdateDesiredColourFromPaletteSelected(uint16_t palette_id){

  // Update pointer of struct
  mPaletteI->SetPaletteListPtrFromID(palette_id);
  // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "UpdateDesiredColourFromPaletteSelected fMapIDs_Type %d"),mPaletteI->palettelist.ptr->flags.fMapIDs_Type);// \"%s\""),GetPaletteFriendlyName());
  // AddLog(LOG_LEVEL_DEBUG_MORE

  uint8_t segment_index = segment_iters._segment_index;
  
  /**
   * Handle the retrieval of colours from palettes depending on the palette encoding type
   * */
  switch(mPaletteI->palettelist.ptr->flags.fMapIDs_Type){
    // default:
    //   #ifdef ENABLE_LOG_LEVEL_DEBUG
    //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "default"));
    //   #endif
    // case MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID:

    default:
    case MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID:
    case MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID:
    case MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX_ID: ///to be tested
    {
    //get colour above

      //apply to positiion below

     // SetLEDOutAmountByPercentage(_segments[0].transition.pixels_to_update_as_percentage.val);

      //what is this?
      // does this only run if above did not? ie this is default?

      //AddLog(LOG_LEVEL_TEST, PSTR("strip_size_requiring_update=%d"),strip_size_requiring_update);
      
      // Other types with random led patterns, or,
      
      switch(_segments[segment_index].transition.order_id)
      {
        case TRANSITION_ORDER_RANDOM_ID:
        {

          Segments_RefreshLEDIndexPattern(segment_index);

          // new transition, so force full update of all segment pixels
          if(_segments[0].flags.NewAnimationRequiringCompleteRefresh){
            _segments[segment_index].pixels_to_update_this_cycle = _segments[segment_index].length();
          }    

          int16_t pixel_position = -2;
          for(
            ledout.index=0;
            ledout.index<_segments[segment_index].pixels_to_update_this_cycle;
            ledout.index++
          ){

            // For random, desired pixel from map will also be random
            desired_pixel = random(0, mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr));
            
            RgbTypeColor colour = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel,&pixel_position);

            #ifdef DEBUG_TRACE_ANIMATOR_SEGMENTS
            AddLog(LOG_LEVEL_TEST, PSTR("desiredpixel%d, colour=%d,%d,%d"), desired_pixel, colour.R, colour.G, colour.B); 
            #endif // DEBUG_TRACE_ANIMATOR_SEGMENTS

            animation_colours[ledout.pattern[ledout.index]].DesiredColour = colour;

            if(_segments[0].flags.brightness_applied_during_colour_generation){
              animation_colours[ledout.pattern[ledout.index]].DesiredColour = ApplyBrightnesstoDesiredColourWithGamma(animation_colours[ledout.pattern[ledout.index]].DesiredColour,pCONT_iLight->getBriRGB_Global());
            }

          } // end for

        }break;
        default: // testing
        // Serial.println("default: // testing");
        case TRANSITION_ORDER_INORDER_ID:{
                
          Segments_RefreshLEDIndexPattern(segment_index);

          // new transition, so force full update of all segment pixels
          if(_segments[0].flags.NewAnimationRequiringCompleteRefresh){
            _segments[segment_index].pixels_to_update_this_cycle = _segments[segment_index].length();
          }   

          // AddLog(LOG_LEVEL_TEST, PSTR("_segments[segment_index].pixels_to_update_this_cycle =%d"), _segments[segment_index].pixels_to_update_this_cycle);

          int16_t pixel_position = -2;
          desired_pixel=0;
          for(
            ledout.index=0;
            ledout.index<_segments[segment_index].pixels_to_update_this_cycle;
            ledout.index++
          ){
            RgbcctColor colour = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel,&pixel_position);

            #ifdef DEBUG_TRACE_ANIMATOR_SEGMENTS
            AddLog(LOG_LEVEL_TEST, PSTR("desiredpixel%d, colour=%d,%d,%d"), desired_pixel, colour.R, colour.G, colour.B); 
            #endif // DEBUG_TRACE_ANIMATOR_SEGMENTS
            
            animation_colours[ledout.pattern[ledout.index]].DesiredColour = colour;

            if(_segments[0].flags.brightness_applied_during_colour_generation){
              animation_colours[ledout.pattern[ledout.index]].DesiredColour = ApplyBrightnesstoDesiredColourWithGamma(animation_colours[ledout.pattern[ledout.index]].DesiredColour, pCONT_iLight->getBriRGB_Global());
            }

            if(++desired_pixel>=mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr)){desired_pixel=0;}

          } //end for

        }break;
      }//end switch


    }break;
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_ID:
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_ID:
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_AND_SETALL_ID:
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_AND_SETALL_ID:{

      // Get active pixels in map
      uint16_t active_pixels_in_map = mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr); //width 2

    #ifdef ENABLE_LOG_LEVEL_INFO
      // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "active_pixels_in_map=%d"),active_pixels_in_map);
    #endif// ENABLE_LOG_LEVEL_INFO

      // Move across map
      int16_t pixel_position = -2;
      for(uint16_t desired_pixel=0;desired_pixel<active_pixels_in_map;desired_pixel++){

        RgbcctColor colour = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel,&pixel_position);
        
        #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
        //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "dp%d, pp%d, %d,%d %d"),desired_pixel, pixel_position,pCONT_iLight->HueF2N(colour.H),pCONT_iLight->SatF2N(colour.S),pCONT_iLight->BrtF2N(colour.B));
        #endif
        
        if(pixel_position>=0){
          // Set output to this "many" colour
          if(pixel_position == 255){
            for(uint16_t temp=0;temp<ledout.length;temp++){ 
              animation_colours[temp].DesiredColour = ApplyBrightnesstoRgbcctColour(colour,pCONT_iLight->getBriRGB_Global());
            }            
            #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
            // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "set ALL %d,%d %d"),pCONT_iLight->HueF2N(colour.H),pCONT_iLight->SatF2N(colour.S),pCONT_iLight->BrtF2N(colour.B));
            #endif
          }else{
            colour.R = 1;colour.R = 2;colour.R = 3;
            // Serial.println("colour.R = 1;colour.R = 2;colour.R = 3;");
            animation_colours[pixel_position].DesiredColour = ApplyBrightnesstoRgbcctColour(colour,pCONT_iLight->getBriRGB_Global());
            #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
            //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "set %d %d,%d %d"), pixel_position,pCONT_iLight->HueF2N(colour.H),pCONT_iLight->SatF2N(colour.S),pCONT_iLight->BrtF2N(colour.B));
            #endif
          }
        }else{          
          #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
          AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "return; //end early %d"),pixel_position);
          #endif
        }

      }
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "return; //end early %d"),pixel_position);
    #endif// ENABLE_LOG_LEVEL_INFO
      break;
      // return;
    }
    break;
    case MAPIDS_TYPE_RGBCCTCOLOUR_WITHINDEX_GRADIENT_ID:
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_GRADIENT_ID:
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID:{

      //#ifdef ENABLE_LOG_LEVEL_DEBUG
      // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID"));
      //#endif

      uint16_t start_pixel = 0;
      uint16_t end_pixel = 100;
      RgbcctColor start_colour = RgbcctColor(0);
      RgbcctColor end_colour = RgbcctColor(0);
      uint16_t desired_pixel = 0;
      int16_t start_pixel_position = -1, end_pixel_position = -1;
      uint8_t pixels_in_map = mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr);

      //#ifdef ENABLE_LOG_LEVEL_DEBUG
      // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "pixels_in_map %d"),pixels_in_map);
      //#endif


      // Add flag to enable ignoring gradient, and filling as normal palette (here?)

      for(uint8_t grad_pair_index=0;grad_pair_index<pixels_in_map;grad_pair_index++)
      {
        
        start_colour = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel,  &start_pixel_position);
        end_colour   = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel+1,&end_pixel_position);

        // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "%d start_pixel_position %d"),grad_pair_index,start_pixel_position);
        // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "end_pixel_position %d"),end_pixel_position);

        #ifndef ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS
        start_colour = ApplyBrightnesstoRgbcctColour(start_colour,pCONT_iLight->getBriRGB_Global());
        end_colour   = ApplyBrightnesstoRgbcctColour(end_colour,pCONT_iLight->getBriRGB_Global());
        #endif // ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS

        // #ifdef ENABLE_LOG_LEVEL_DEBUG
        //  AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "s%d,%d %d %d"),pCONT_iLight->HueF2N(start_colour.H),pCONT_iLight->SatF2N(start_colour.S),pCONT_iLight->BrtF2N(start_colour.B),start_pixel_position);
        //  AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "e%d,%d %d %d"),HueF2N(end_colour.H),SatF2N(end_colour.S),BrtF2N(end_colour.B),end_pixel_position);
        // #endif

        uint16_t start_pixel = _segments[segment_iters._segment_index].pixel_range.start;
        uint16_t stop_pixel = _segments[segment_iters._segment_index].pixel_range.stop;

        switch(mPaletteI->palettelist.ptr->flags.fIndexs_Type){
          case INDEX_TYPE_DIRECT: break; //remains the same
          case INDEX_TYPE_SCALED_255: 
            start_pixel_position = map(start_pixel_position,0,255,start_pixel,stop_pixel);
            end_pixel_position   = map(end_pixel_position,0,255,start_pixel,stop_pixel);
            // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "255 %d %d"),start_pixel_position,end_pixel_position);
          break;
          case INDEX_TYPE_SCALED_100: 
            start_pixel_position = map(start_pixel_position,0,100,start_pixel,stop_pixel);
            end_pixel_position   = map(end_pixel_position,0,100,start_pixel,stop_pixel);          
          break;
        }

        // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "%d start_pixel_position %d"),grad_pair_index,start_pixel_position);
        // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "end_pixel_position %d"),end_pixel_position);

        float progress = 0;
        for(
          ledout.index=start_pixel_position;
          ledout.index<=end_pixel_position;
          ledout.index++){
            // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "ledout.index%d %d %d"),ledout.index,start_pixel_position,end_pixel_position);

            // if(start_pixel_position == end_pixel_position)
            // {
            //   // preference to the lower pixel directly, no blend
            //   if(start_pixel_position == start_pixel){ //start, then chose start
            //     animation_colours[ledout.index].DesiredColour = start_colour;
            // // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO " _position == start_pix ledout.index%d %d %d"),ledout.index,start_pixel_position,end_pixel_position);
            //   }else{//end
              
            //   animation_colours[ledout.index].DesiredColour = start_colour;
            // // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO " lse{//end ledout.index%d %d %d"),ledout.index,start_pixel_position,end_pixel_position);
            //   }
            // }
            // else{
            
              progress = mSupport::mapfloat(ledout.index,start_pixel_position,end_pixel_position,0,1);
              animation_colours[ledout.index].DesiredColour = RgbcctColor::LinearBlend(start_colour, end_colour, progress);
            // }
            //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "ledout.index%d %d %d"),ledout.index,start_pixel_position,end_pixel_position);
        }

        desired_pixel++;

      }

      // Colour applied to entire DesiredColour, leaving now
      break;
      // return; //succcesfully handled

    }break;
    
  }

  #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "DONE UpdateDesiredColourFromPaletteSelected fMapIDs_Type "));
  #endif  // LOG_LEVEL_DEBUG_MORE

} //end function UpdateDesiredColourFromPaletteSelected();


/**
 * @name : Segments_RefreshLEDIndexPattern
 * @brief: Generates the index pattern needed for the output
 * 
 * @note : I may need to move away from this method, or absorb it into the segment_runtime so its not always used. This
 *         will require removing/changing how the DesiredColour struct works. Since it is only needed for some animations, it may be easier to remove it 
 * */
void mAnimatorLight::Segments_RefreshLEDIndexPattern(uint8_t segment_index)
{
DEBUG_LINE_HERE;

  // Generate lighting pattern
  switch(_segments[segment_index].transition.order_id){
    default:
    /**
     * @note: The pattern simply puts increasing number, starting and ending with the segments pixel index
     * */
    case TRANSITION_ORDER_INORDER_ID:

      // Update for the entire length of segment ie 100%
      Segments_SetLEDOutAmountByPercentage(100, segment_index);

      // Starting from segment pixel index, until length of pixels
      for(uint16_t ii=0;
                   ii<_segments[segment_index].pixels_to_update_this_cycle;
                   ii++
      ){ 
        ledout.pattern[ii] = _segments[segment_index].pixel_range.start + ii; 
      }

      #ifdef DEBUG_TRACE_ANIMATOR_SEGMENTS
      Serial.printf("TRANSITION_ORDER_INORDER_ID %d>>", segment_index);
      for(int jj=0;jj<segment_length;jj++){ Serial.printf("%d, ",ledout.pattern[jj]); } Serial.println();
      #endif // DEBUG_TRACE_ANIMATOR_SEGMENTS

    break;
    /**
     * @note: Randomly pick which indexes to update within the total segment length
     * */
    case TRANSITION_ORDER_RANDOM_ID:
    
      // Update for the entire length of segment ie 100%
      Segments_SetLEDOutAmountByPercentage(100, segment_index);

      for(uint16_t ii=0;
                   ii<_segments[segment_index].pixels_to_update_this_cycle;
                   ii++
      ){ 
        ledout.pattern[ii] = random(_segments[segment_index].pixel_range.start, _segments[segment_index].pixel_range.stop); 
      }

    break;
  }

} 


/**
 * Duplicate parameter, needs merging with above in long term if it is really the same
 * */
void mAnimatorLight::Segments_SetLEDOutAmountByPercentage(uint8_t percentage, uint8_t segment_index){

  //   strip_size_requiring_update = mapvalue(percentage, 0,100, 0,pCONT_iLight->settings.light_size_count);

  // // AddLog(LOG_LEVEL_TEST, PSTR(DEBUG_INSERT_PAGE_BREAK "SetLEDOutAmountByPercentage = %d"),strip_size_requiring_update);

  uint16_t pixels_in_segment = _segments[segment_index].length();

  _segments[segment_index].pixels_to_update_this_cycle = mapvalue(percentage, 0,100, 0,pixels_in_segment);

  // AddLog(LOG_LEVEL_TEST, PSTR(DEBUG_INSERT_PAGE_BREAK "SetLEDOutAmountByPercentage = %d"),strip_size_requiring_update);

}


// // Update struct that shows overview, always sends
uint8_t mAnimatorLight::ConstructJSON_Flasher(uint8_t json_level){

// #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
// return 0;
// #endif

  char buffer[100];

  JsonBuilderI->Start();

    JBI->Level_Start("Segments");

    // limit the size of array
    uint16_t length = _segment_runtimes[0]._dataLen;

    length = length < 100 ? length : 100;

    JBI->Array_Start("data0");
      for(int i=0;i<length;i++)
      {
        if(_segment_runtimes[0].data != nullptr)
        {
          JBI->Add(_segment_runtimes[0].data[i]);
        }
      }
    JBI->Array_End();

    length = _segment_runtimes[1]._dataLen;
    length = length < 100 ? length : 100;

    JBI->Array_Start("data1");
    for(int i=0;i<length;i++)
    {
      if(_segment_runtimes[1].data != nullptr)
      {
        JBI->Add(_segment_runtimes[1].data[i]);
      }
    }
    JBI->Array_End();

  JBI->Level_End();

  JBI->Add("segment_settings_size", sizeof(segment_settings));
  JBI->Add("segment_runtime_size", sizeof(segment_runtime));

//   JBI->Add("flashermillis",millis()-flashersettings.tSaved.Update);

// root[D_JSON_ONOFF] = pCONT_iLight->light_power_state ? "ON" : "OFF";
// JBI->Add(PM_JSON_MODE, pCONT_iLight->GetAnimationModeName(buffer, sizeof(buffer)));
// JBI->Add(PM_JSON_FUNCTION, GetFlasherFunctionName(buffer, sizeof(buffer)));
// root["region"] = GetFlasherRegionName();
// root[D_JSON_COLOUR_PALETTE] = GetPaletteFriendlyName();
// root[D_JSON_BRIGHTNESS_PERCENTAGE] = _segments[0].brightness*100;
// root[D_JSON_BRIGHTNESS] = _segments[0].brightness;

//   // JsonObject transitionobj = root.createNestedObject(D_JSON_TRANSITION);
//   //   transitionobj[D_JSON_METHOD] = GetTransitionMethodName();
//   //   transitionobj[D_JSON_TIME] = mSupport::safeDivideInt(_segments[0].transition.time_ms,1000);
//   //   transitionobj[D_JSON_TIME_MS] = _segments[0].transition.time_ms;
//   //   transitionobj[D_JSON_RATE] = mSupport::safeDivideInt(_segments[0].transition.rate_ms,1000);
//   //   transitionobj[D_JSON_RATE_MS] = _segments[0].transition.rate_ms;
//   //   transitionobj[D_JSON_FUNCTION] = GetFlasherFunctionName();

//   JsonObject seq_obj = root.createNestedObject("sequential");
//     seq_obj["rate_index"] = flashersettings.function_seq.rate_index;

// JsonBuilderI->Level_Start("slow_glow");
//   JsonBuilderI->Add("rate_index", flashersettings.function_slo_glo.rate_index);
// JsonBuilderI->Level_End();

  return JsonBuilderI->End();
}


/********************************************************************************************************************************
**********Flasher Function ******************************************************************************************************
********************************************************************************************************************************/
void mAnimatorLight::CommandSet_Flasher_FunctionID(uint8_t value, uint8_t segment_index ){
  
  _segments[segment_index].effect_id = value;      //make function "changeFlasherFunction" so then the region is automatically updated internally
  _segment_runtimes[0].aux0 = EFFECTS_REGION_COLOUR_SELECT_ID;
  _segments[0].flags.animator_first_run= true; // first run, so do extra things
  
  #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
  setCallback_ConstructJSONBody_Debug_Animations_Progress(nullptr); // clear to be reset
  #endif
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  char buffer[30];
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_JSON_COMMAND_SVALUE_SVALUE_K(D_JSON_EFFECTS, D_JSON_FUNCTION)), GetFlasherFunctionName(buffer, sizeof(buffer)));
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}

int8_t mAnimatorLight::GetFlasherFunctionIDbyName(const char* f)
{

  if(f=='\0') return -2;
  if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_SOLID_COLOUR_NAME_CTR)){ return EFFECTS_FUNCTION_SOLID_COLOUR_ID; }
  if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_STATIC_PALETTE_NAME_CTR)){  return EFFECTS_FUNCTION_STATIC_PALETTE_ID; }
  if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_SLOW_GLOW_NAME_CTR)){  return EFFECTS_FUNCTION_SLOW_GLOW_ID; }
  if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_STATIC_PALETTE_NAME_CTR)){  return EFFECTS_FUNCTION_STATIC_PALETTE_ID; }
  if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_FIREPLACE_1D_01_NAME_CTR)){ return EFFECTS_FUNCTION_FIREPLACE_1D_01_ID; }
  if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_STEP_THROUGH_PALETTE_CTR)){ return EFFECTS_FUNCTION_STEP_THROUGH_PALETTE_ID; }
  
  if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01_NAME_CTR)){ return EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01_ID; }
  if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_WITH_AUGMENTED_TRANSITIONS_01_NAME_CTR)){ return EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_WITH_AUGMENTED_TRANSITIONS_01_ID; }
  if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_CCT_TEMPERATURE_01_NAME_CTR)){ return EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_CCT_TEMPERATURE_01_ID; }
  
  if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_TESTER_NAME_CTR)){ return EFFECTS_FUNCTION_TESTER_ID; }

  return -1;

}
const char* mAnimatorLight::GetFlasherFunctionName(char* buffer, uint8_t buflen, uint8_t segment_index )
{
  return GetFlasherFunctionNamebyID(_segments[segment_index].effect_id, buffer, buflen);
}
const char* mAnimatorLight::GetFlasherFunctionNamebyID(uint8_t id, char* buffer, uint8_t buflen)
{
  switch(id){
    default:  snprintf_P(buffer, buflen, PM_SEARCH_NOMATCH);  break;
    case EFFECTS_FUNCTION_STATIC_PALETTE_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_STATIC_PALETTE_NAME_CTR);  break;
    case EFFECTS_FUNCTION_SLOW_GLOW_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_SLOW_GLOW_NAME_CTR);  break;
    case EFFECTS_FUNCTION_STEP_THROUGH_PALETTE_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_STEP_THROUGH_PALETTE_CTR);  break;
    case EFFECTS_FUNCTION_SEQUENTIAL_ID:  snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_SEQUENTIAL_NAME_CTR); break;
    case EFFECTS_FUNCTION_SOLID_COLOUR_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_SOLID_COLOUR_NAME_CTR);  break;
    case EFFECTS_FUNCTION_FIREPLACE_1D_01_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_FIREPLACE_1D_01_NAME_CTR);  break;

    case EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01_NAME_CTR);  break;
    case EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_WITH_AUGMENTED_TRANSITIONS_01_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_WITH_AUGMENTED_TRANSITIONS_01_NAME_CTR);  break;
    case EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_CCT_TEMPERATURE_01_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_CCT_TEMPERATURE_01_NAME_CTR);  break;
  
  
    case EFFECTS_FUNCTION_TESTER_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_TESTER_NAME_CTR);  break;

  }
  return buffer;
}


/********************************************************************************************************************************
**********Flasher Region ******************************************************************************************************
********************************************************************************************************************************/


void mAnimatorLight::CommandSet_Flasher_UpdateColourRegion_RefreshSecs(uint8_t value, uint8_t segment_index)
{
  flashersettings_segments.update_colour_region.refresh_secs = value; 
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_JSON_COLOUR_REFRESH_RATE)), flashersettings_segments.update_colour_region.refresh_secs);
  #endif // ENABLE_LOG_LEVEL_COMMANDS
}




#endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

#endif //USE_MODULE_LIGHTS_ANIMATOR




