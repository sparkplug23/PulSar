#include "../mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

#ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

uint16_t mAnimatorLight::_usedSegmentData = 0;


void mAnimatorLight::Init_Segments()
{
  
  resetSegments();

  Init_Segments_RgbcctControllers();

  // Serial.println("To get past failed flashing");
  // Serial.println("To get past failed flashing");
  // Serial.println("To get past failed flashing");
  // Serial.println("To get past failed flashing");
  // Serial.println("To get past failed flashing");
  // Serial.println("To get past failed flashing");
  // Serial.println("To get past failed flashing");
  // Serial.println("To get past failed flashing");
  // Serial.println("To get past failed flashing");
  


  // _segments[0].palette.id = mPaletteI->PALETTELIST_STATIC_CHRISTMAS_03_ID;
  // _segments[1].palette.id = mPaletteI->PALETTELIST_STATIC_CHRISTMAS_01_ID;
  // _segments[2].palette.id = mPaletteI->PALETTELIST_STATIC_CHRISTMAS_02_ID;
  // _segments[3].palette.id = mPaletteI->PALETTELIST_STATIC_SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01_ID;
  // _segments[4].palette.id = mPaletteI->PALETTELIST_STATIC_SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_04_ID  ;

  // _segments[0].transition.rate_ms = 10000;
  // _segments[1].transition.rate_ms = 10000;
  // _segments[2].transition.rate_ms = 10000;
  // _segments[3].transition.rate_ms = 1000;
  // _segments[4].transition.rate_ms = 1000;

  // _segments[0].transition.time_ms = 3000;
  // _segments[1].transition.time_ms = 900;
  // _segments[2].transition.time_ms = 900;
  // _segments[3].transition.time_ms = 0;
  // _segments[4].transition.time_ms = 0;

  // _segments[0].transition.order_id = TRANSITION_ORDER_INORDER_ID;
  // _segments[1].transition.order_id = TRANSITION_ORDER_INORDER_ID;
  // _segments[2].transition.order_id = TRANSITION_ORDER_INORDER_ID;
  // _segments[3].transition.order_id = TRANSITION_ORDER_RANDOM_ID;
  // _segments[4].transition.order_id = TRANSITION_ORDER_RANDOM_ID;

  // _segments[0].effect_id = EFFECTS_FUNCTION_SLOW_GLOW_ID;
  // _segments[1].effect_id = EFFECTS_FUNCTION_STEP_THROUGH_PALETTE_ID;
  // _segments[2].effect_id = EFFECTS_FUNCTION_STATIC_PALETTE_ID;
  // _segments[3].effect_id = EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01_ID;
  // _segments[4].effect_id = EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01_ID;

  // _segment_runtimes[0].aux0 = EFFECTS_REGION_COLOUR_SELECT_ID;
  // _segment_runtimes[1].aux0 = EFFECTS_REGION_COLOUR_SELECT_ID;
  // _segment_runtimes[2].aux0 = EFFECTS_REGION_COLOUR_SELECT_ID;
  // _segment_runtimes[3].aux0 = EFFECTS_REGION_COLOUR_SELECT_ID;
  // _segment_runtimes[4].aux0 = EFFECTS_REGION_COLOUR_SELECT_ID;


}

void mAnimatorLight::Init_Segments_RgbcctControllers()
{


  // PALETTELIST_STATIC_CHRISTMAS_01_ID
  /**
   * Init the Rgbcct palettes
   * */
  
  _segment_runtimes[0].rgbcct_controller->setRgbcctColourOutputAddress(mPaletteI->palettelist.rgbcct_users[0].colour_map_id);
  _segment_runtimes[1].rgbcct_controller->setRgbcctColourOutputAddress(mPaletteI->palettelist.rgbcct_users[1].colour_map_id);
  _segment_runtimes[2].rgbcct_controller->setRgbcctColourOutputAddress(mPaletteI->palettelist.rgbcct_users[2].colour_map_id);
  _segment_runtimes[3].rgbcct_controller->setRgbcctColourOutputAddress(mPaletteI->palettelist.rgbcct_users[3].colour_map_id);
  _segment_runtimes[4].rgbcct_controller->setRgbcctColourOutputAddress(mPaletteI->palettelist.rgbcct_users[4].colour_map_id);

  // DEBUG_LINE_HERE; 

  // CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_01_ID, 0);

  
  for (uint16_t i = 1; i < MAX_NUM_SEGMENTS; i++)
  {
  _segment_runtimes[i].rgbcct_controller->setSubType(LST_RGBCW);
  _segment_runtimes[i].rgbcct_controller->setApplyBrightnessToOutput(false);
  if(pCONT_set->Settings.light_settings.type == LT_ADDRESSABLE){ //RGB only
    _segment_runtimes[i].rgbcct_controller->setColorMode(LCM_RGB);
  }else{
    _segment_runtimes[i].rgbcct_controller->setColorMode(LCM_BOTH);
  }
  _segment_runtimes[i].rgbcct_controller->Sync();    // calculate the initial values (#8058)
  // RGB parts
  _segment_runtimes[i].rgbcct_controller->setRGB(1,2,3);
  // CCT parts
  _segment_runtimes[i].rgbcct_controller->setRGBCCTLinked(false);
  _segment_runtimes[i].rgbcct_controller->setCCT(153);
  }



  CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_01_ID, 0);
  CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_02_ID, 1);
  CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_03_ID, 2);
  CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_04_ID, 3);
  CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_05_ID, 4);

  CommandSet_ActiveSolidPalette_Hue_360(123, 0);
  CommandSet_ActiveSolidPalette_Hue_360(120, 1);
  CommandSet_ActiveSolidPalette_Hue_360(180, 2);
  CommandSet_ActiveSolidPalette_Hue_360(240, 3);
  CommandSet_ActiveSolidPalette_Hue_360(330, 4);

  CommandSet_ActiveSolidPalette_Sat_255(map(90, 0,100, 0,255), 0);
  CommandSet_ActiveSolidPalette_Sat_255(map(90, 0,100, 0,255), 1);
  CommandSet_ActiveSolidPalette_Sat_255(map(90, 0,100, 0,255), 2);
  CommandSet_ActiveSolidPalette_Sat_255(map(90, 0,100, 0,255), 3);
  CommandSet_ActiveSolidPalette_Sat_255(map(90, 0,100, 0,255), 4);

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

  // uint8_t ty = 1;
  // if (supportWhite) ty = 2;
  _lengthRaw = _length;
  // if (_skipFirstMode) {
  //   _lengthRaw += LED_SKIP_AMOUNT;
  // }


  // mainSegment = 0;
  // memset(_segments, 0, sizeof(_segments));
  // memset(_segment_runtimes, 0, sizeof(_segment_runtimes));
  // segment_iters.index = 0;
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
    _segments[i].colors[0] = color_wheel(i*51);
    _segments[i].colors[1] = color_wheel(i*56);
    _segments[i].colors[2] = color_wheel(i*81);
    _segments[i].grouping = 1;
    // _segments[i].setOption(SEG_OPTION_ON, 1);
    // _segments[i].opacity = 255;
    _segment_runtimes[i].reset();
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
  _segments[0].pixel_range.stop = STRIP_SIZE_MAX;
  // _segments[1].pixel_range.start = 10;
  // _segments[1].pixel_range.stop = 19;
  // _segments[2].pixel_range.start = 20;
  // _segments[2].pixel_range.stop = 29;
  // _segments[3].pixel_range.start = 30;
  // _segments[3].pixel_range.stop = 39;
  // _segments[4].pixel_range.start = 40;
  // _segments[4].pixel_range.stop = 49;


  // _segment_runtimes[0].animator = new NeoPixelAnimator(1, NEO_MILLISECONDS);
  // _segment_runtimes[1].animator = new NeoPixelAnimator(1, NEO_MILLISECONDS);
  // _segment_runtimes[2].animator = new NeoPixelAnimator(1, NEO_MILLISECONDS);
  // _segment_runtimes[3].animator = new NeoPixelAnimator(1, NEO_MILLISECONDS);
  // _segment_runtimes[4].animator = new NeoPixelAnimator(1, NEO_MILLISECONDS);

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
      segment_iters.index = 0; 
      segment_iters.index<MAX_NUM_SEGMENTS; 
      segment_iters.index++
    ){
      // if(mTime::TimeReached(&tSaved_Test_Segment_Animation, 2000))
      // {
        
      if(_segments[segment_iters.index].isActive())
      {


        if((mTime::TimeReached(&_segments[segment_iters.index].tSaved_AnimateRunTime, _segments[segment_iters.index].transition.rate_ms))||(_segments[0].flags.fForceUpdate))
        {

          if(_segments[0].flags.fForceUpdate){ _segments[0].flags.fForceUpdate=false;
            _segments[segment_iters.index].tSaved_AnimateRunTime = millis();
          }
          
        _virtualSegmentLength = _segments[segment_iters.index].virtualLength();
          
        // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "flashersettings.function=segI%d %d %s"),segment_iters.index, flashersettings_segments.function,GetFlasherFunctionNamebyID(flashersettings_segments.function, buffer));

        // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "flashersettings.function=segI%d %d %d"),segment_iters.index, _segments[segment_iters.index].effect_id);
        // DEBUG_LINE_HERE;

        // if( _segment_runtimes[segment_iters.index].animation_has_anim_callback == false)
        // {
        //   Serial.printf("IFWorking%d %d\n\r", segment_iters.index , _segments[segment_iters.index].effect_id);
        // }else{
        //   Serial.printf("ELWorking%d %d\n\r", segment_iters.index , _segments[segment_iters.index].effect_id);
        // }

      
        switch(_segments[segment_iters.index].effect_id){
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
          /**
           * Static
           **/
          case EFFECTS_FUNCTION_WLED_STATIC_ID:
            void SubTask_Segment_Flasher_Animate_Function__Static();
          break;
          case EFFECTS_FUNCTION_WLED_STATIC_PATTERN_ID:
            SubTask_Segment_Flasher_Animate_Function__Static_Pattern();
          break;
          case EFFECTS_FUNCTION_WLED_TRI_STATIC_PATTERN_ID:
            SubTask_Segment_Flasher_Animate_Function__Tri_Static_Pattern();
          break;
          case EFFECTS_FUNCTION_WLED_SPOTS_ID:
            SubTask_Segment_Flasher_Animate_Function__Spots();
          break;
          case EFFECTS_FUNCTION_WLED_PERCENT_ID:
            SubTask_Segment_Flasher_Animate_Function__Percent();
          break;
          /**
           * One Colour
           **/
          case EFFECTS_FUNCTION_WLED_RANDOM_COLOR_ID:
            SubTask_Segment_Flasher_Animate_Function__Random_Colour();
          break;
          /**
           * Wipe/Sweep/Runners 
           **/
          case EFFECTS_FUNCTION_WLED_COLOR_WIPE_ID:
            SubTask_Segment_Flasher_Animate_Function__Colour_Wipe();
          break;
          case EFFECTS_FUNCTION_WLED_COLOR_WIPE_RANDOM_ID:
            SubTask_Segment_Flasher_Animate_Function__Colour_Wipe_Random();
          break;
          case EFFECTS_FUNCTION_WLED_COLOR_SWEEP_ID:
            SubTask_Segment_Flasher_Animate_Function__Colour_Sweep();
          break;
          case EFFECTS_FUNCTION_WLED_COLOR_SWEEP_RANDOM_ID:
            SubTask_Segment_Flasher_Animate_Function__Colour_Sweep_Random();
          break;
          case EFFECTS_FUNCTION_WLED_TRICOLOR_WIPE_ID:
            SubTask_Segment_Flasher_Animate_Function__TriColour();
          break;
          case EFFECTS_FUNCTION_WLED_ANDROID_ID:
            SubTask_Segment_Flasher_Animate_Function__Android();
          break;
          case EFFECTS_FUNCTION_WLED_RUNNING_RED_BLUE_ID:
            SubTask_Segment_Flasher_Animate_Function__Running_Red_Blue();
          break;
          case EFFECTS_FUNCTION_WLED_RUNNING_COLOR_ID:
            SubTask_Segment_Flasher_Animate_Function__Running_Colour();
          break;
          case EFFECTS_FUNCTION_WLED_RUNNING_RANDOM_ID:
            SubTask_Segment_Flasher_Animate_Function__Running_Random();
          break;
          case EFFECTS_FUNCTION_WLED_GRADIENT_ID:
            SubTask_Segment_Flasher_Animate_Function__Gradient();
          break;
          case EFFECTS_FUNCTION_WLED_LOADING_ID:
            SubTask_Segment_Flasher_Animate_Function__Loading();
          break;
          case EFFECTS_FUNCTION_WLED_POLICE_ID:
            SubTask_Segment_Flasher_Animate_Function__Police();
          break;
          case EFFECTS_FUNCTION_WLED_POLICE_ALL_ID:
            SubTask_Segment_Flasher_Animate_Function__Polce_All();
          break;
          case EFFECTS_FUNCTION_WLED_TWO_DOTS_ID:
            SubTask_Segment_Flasher_Animate_Function__Two_Dots();
          break;
          case EFFECTS_FUNCTION_WLED_TWO_AREAS_ID:
            SubTask_Segment_Flasher_Animate_Function__Two_Areas();
          break;
          case EFFECTS_FUNCTION_WLED_MULTI_COMET_ID:
            SubTask_Segment_Flasher_Animate_Function__Multi_Comet();
          break;
          case EFFECTS_FUNCTION_WLED_OSCILLATE_ID:
            SubTask_Segment_Flasher_Animate_Function__Oscillate();
          break;
          case EFFECTS_FUNCTION_WLED_BPM_ID:
            SubTask_Segment_Flasher_Animate_Function__BPM();
          break;
          case EFFECTS_FUNCTION_WLED_JUGGLE_ID:
            SubTask_Segment_Flasher_Animate_Function__Juggle();
          break;
          case EFFECTS_FUNCTION_WLED_PALETTE_ID:
            SubTask_Segment_Flasher_Animate_Function__Palette();
          break;
          case EFFECTS_FUNCTION_WLED_COLORWAVES_ID:
            SubTask_Segment_Flasher_Animate_Function__ColourWaves();
          break;
          case EFFECTS_FUNCTION_WLED_LAKE_ID:
            SubTask_Segment_Flasher_Animate_Function__Lake();
          break;
          case EFFECTS_FUNCTION_WLED_GLITTER_ID:
            SubTask_Segment_Flasher_Animate_Function__Glitter();
          break;
          case EFFECTS_FUNCTION_WLED_METEOR_ID:
            SubTask_Segment_Flasher_Animate_Function__Meteor();
          break;
          case EFFECTS_FUNCTION_WLED_METEOR_SMOOTH_ID:
            SubTask_Segment_Flasher_Animate_Function__Metoer_Smooth();
          break;
          case EFFECTS_FUNCTION_WLED_PRIDE_2015_ID:
            SubTask_Segment_Flasher_Animate_Function__Pride_2015();
          break;
          case EFFECTS_FUNCTION_WLED_RIPPLE_RAINBOW_ID:
            SubTask_Segment_Flasher_Animate_Function__Ripple_Rainbow();
          break;
          case EFFECTS_FUNCTION_WLED_PACIFICA_ID:
            SubTask_Segment_Flasher_Animate_Function__Pacifica();
          break;
          case EFFECTS_FUNCTION_WLED_SUNRISE_ID:
            SubTask_Segment_Flasher_Animate_Function__Sunrise();
          break;
          case EFFECTS_FUNCTION_WLED_SINEWAVE_ID:
            SubTask_Segment_Flasher_Animate_Function__Sinewave();
          break;
          case EFFECTS_FUNCTION_WLED_FLOW_ID:
            SubTask_Segment_Flasher_Animate_Function__Flow();
          break;
          case EFFECTS_FUNCTION_WLED_PHASEDNOISE_ID:
            SubTask_Segment_Flasher_Animate_Function__PhasedNoise();
          break;
          case EFFECTS_FUNCTION_WLED_PHASED_ID:
            SubTask_Segment_Flasher_Animate_Function__Phased();
          break;
          case EFFECTS_FUNCTION_WLED_RUNNING_LIGHTS_ID:
            SubTask_Segment_Flasher_Animate_Function__Running_Lights();
          break;
          case EFFECTS_FUNCTION_WLED_RAINBOW_CYCLE_ID:
            SubTask_Segment_Flasher_Animate_Function__Rainbow_Cycle();
          break;
          case EFFECTS_FUNCTION_WLED_MERRY_CHRISTMAS_ID:
            SubTask_Segment_Flasher_Animate_Function__Merry_Christmas();
          break;
          case EFFECTS_FUNCTION_WLED_HALLOWEEN_ID:
            SubTask_Segment_Flasher_Animate_Function__Halloween();
          break;
          /**
           * Chase
           **/
          case EFFECTS_FUNCTION_WLED_CHASE_COLOR_ID:
            SubTask_Segment_Flasher_Animate_Function__Chase_Colour();
          break;
          case EFFECTS_FUNCTION_WLED_CHASE_RANDOM_ID:
            SubTask_Segment_Flasher_Animate_Function__Chase_Random();
          break;
          case EFFECTS_FUNCTION_WLED_CHASE_RAINBOW_ID:
            SubTask_Segment_Flasher_Animate_Function__Chase_Rainbow();
          break;
          case EFFECTS_FUNCTION_WLED_CHASE_FLASH_ID:
            SubTask_Segment_Flasher_Animate_Function__Chase_Flash();
          break;
          case EFFECTS_FUNCTION_WLED_CHASE_FLASH_RANDOM_ID:
            SubTask_Segment_Flasher_Animate_Function__Chase_Flash_Random();
          break;
          case EFFECTS_FUNCTION_WLED_CHASE_RAINBOW_WHITE_ID:
            SubTask_Segment_Flasher_Animate_Function__Chase_Rainbow_White();
          break;
          case EFFECTS_FUNCTION_WLED_CHASE_THEATER_ID:
            SubTask_Segment_Flasher_Animate_Function__Chase_Theater();
          break;
          case EFFECTS_FUNCTION_WLED_CHASE_THEATER_RAINBOW_ID:
            SubTask_Segment_Flasher_Animate_Function__Chase_Theatre_Rainbow();
          break;
          case EFFECTS_FUNCTION_WLED_CHASE_TRICOLOR_ID:
            SubTask_Segment_Flasher_Animate_Function__Chase_TriColour();
          break;
          case EFFECTS_FUNCTION_WLED_CIRCUS_COMBUSTUS_ID:
            SubTask_Segment_Flasher_Animate_Function__Circus_Combustus();
          break;
          /**
           *  Breathe/Fade/Pulse
           **/    
          case EFFECTS_FUNCTION_WLED_BREATH_ID:
            SubTask_Segment_Flasher_Animate_Function__Breath();
          break;
          case EFFECTS_FUNCTION_WLED_FADE_ID:
            SubTask_Segment_Flasher_Animate_Function__Fade();
          break;
          case EFFECTS_FUNCTION_WLED_FADE_TRICOLOR_ID:
            SubTask_Segment_Flasher_Animate_Function__Fade_TriColour();
          break;
          case EFFECTS_FUNCTION_WLED_FADE_SPOTS_ID:
            SubTask_Segment_Flasher_Animate_Function__Fade_Spots();
          break;
          /**
           * Fireworks
           **/
          case EFFECTS_FUNCTION_WLED_FIREWORKS_ID:
            SubTask_Segment_Flasher_Animate_Function__Fireworks();
          break;
          case EFFECTS_FUNCTION_WLED_FIREWORKS_EXPLODING_ID:
            SubTask_Segment_Flasher_Animate_Function__Exploding_Fireworks();
          break;
          case EFFECTS_FUNCTION_WLED_FIREWORKS_STARBURST_ID:
            SubTask_Segment_Flasher_Animate_Function__Fireworks_Starburst();
          break;
          case EFFECTS_FUNCTION_WLED_RAIN_ID:
            SubTask_Segment_Flasher_Animate_Function__Rain();
          break;
          /**
           * Sparkle/Twinkle
           **/
          case EFFECTS_FUNCTION_WLED_SOLID_GLITTER_ID:
            SubTask_Segment_Flasher_Animate_Function__Solid_Glitter();
          break;
          case EFFECTS_FUNCTION_WLED_POPCORN_ID:
            SubTask_Segment_Flasher_Animate_Function__Popcorn();
          break;
          case EFFECTS_FUNCTION_WLED_PLASMA_ID:
            SubTask_Segment_Flasher_Animate_Function__Plasma();
          break;
          case EFFECTS_FUNCTION_WLED_SPARKLE_ID:
            SubTask_Segment_Flasher_Animate_Function__Sparkle();
          break;
          case EFFECTS_FUNCTION_WLED_FLASH_SPARKLE_ID:
            SubTask_Segment_Flasher_Animate_Function__Sparkle_Flash();
          break;
          case EFFECTS_FUNCTION_WLED_HYPER_SPARKLE_ID:
            SubTask_Segment_Flasher_Animate_Function__Sparkle_Hyper();
          break;
          case EFFECTS_FUNCTION_WLED_TWINKLE_ID:
            SubTask_Segment_Flasher_Animate_Function__Twinkle();
          break;
          case EFFECTS_FUNCTION_WLED_COLORTWINKLE_ID:
            SubTask_Segment_Flasher_Animate_Function__Twinkle_Colour();
          break;
          case EFFECTS_FUNCTION_WLED_TWINKLEFOX_ID:
            SubTask_Segment_Flasher_Animate_Function__Twinkle_Fox();
          break;
          case EFFECTS_FUNCTION_WLED_TWINKLECAT_ID:
            SubTask_Segment_Flasher_Animate_Function__Twinkle_Cat();
          break;
          case EFFECTS_FUNCTION_WLED_TWINKLEUP_ID:
            SubTask_Segment_Flasher_Animate_Function__Twinkle_Up();
          break;
          case EFFECTS_FUNCTION_WLED_DYNAMIC_ID:
            SubTask_Segment_Flasher_Animate_Function__Dynamic();
          break;
          case EFFECTS_FUNCTION_WLED_SAW_ID:
            SubTask_Segment_Flasher_Animate_Function__Saw();
          break;
          case EFFECTS_FUNCTION_WLED_DISSOLVE_ID:
            SubTask_Segment_Flasher_Animate_Function__Dissolve();
          break;
          case EFFECTS_FUNCTION_WLED_DISSOLVE_RANDOM_ID:
            SubTask_Segment_Flasher_Animate_Function__Dissolve_Random();
          break;
          case EFFECTS_FUNCTION_WLED_COLORFUL_ID:
            SubTask_Segment_Flasher_Animate_Function__ColourFul();
          break;
          case EFFECTS_FUNCTION_WLED_TRAFFIC_LIGHT_ID:
            SubTask_Segment_Flasher_Animate_Function__Traffic_Light();
          break;
          case EFFECTS_FUNCTION_WLED_CANDLE_SINGLE_ID:
            SubTask_Segment_Flasher_Animate_Function__Candle_Single();
          break;     
          case EFFECTS_FUNCTION_WLED_CANDLE_MULTI_ID:
            SubTask_Segment_Flasher_Animate_Function__Candle_Multi();
          break;     
          case EFFECTS_FUNCTION_WLED_FIRE_FLICKER_ID:
            SubTask_Segment_Flasher_Animate_Function__Fire_Flicker();
          break;
          case EFFECTS_FUNCTION_WLED_SHIMMERING_PALETTE_ID:
            SubTask_Segment_Flasher_Animate_Function__Shimmering_Palette();
          break;   
          /**
           * Blink/Strobe
           **/
          case EFFECTS_FUNCTION_WLED_BLINK_ID:
            SubTask_Segment_Flasher_Animate_Function__Blink();
          break;
          case EFFECTS_FUNCTION_WLED_BLINK_RAINBOW_ID:
            SubTask_Segment_Flasher_Animate_Function__Blink_Rainbow();
          break;
          case EFFECTS_FUNCTION_WLED_STROBE_ID:
            SubTask_Segment_Flasher_Animate_Function__Strobe();
          break;
          case EFFECTS_FUNCTION_WLED_MULTI_STROBE_ID:
            SubTask_Segment_Flasher_Animate_Function__Strobe_Multi();
          break;
          case EFFECTS_FUNCTION_WLED_STROBE_RAINBOW_ID:
            SubTask_Segment_Flasher_Animate_Function__Strobe_Rainbow();
          break;
          case EFFECTS_FUNCTION_WLED_RAINBOW_ID:
            SubTask_Segment_Flasher_Animate_Function__Rainbow();
          break;
          case EFFECTS_FUNCTION_WLED_LIGHTNING_ID:
            SubTask_Segment_Flasher_Animate_Function__Lightning();
          break;
          case EFFECTS_FUNCTION_WLED_FIRE_2012_ID:
            SubTask_Segment_Flasher_Animate_Function__Fire_2012();
          break;
          case EFFECTS_FUNCTION_WLED_RAILWAY_ID:
            SubTask_Segment_Flasher_Animate_Function__Railway();
          break;
          case EFFECTS_FUNCTION_WLED_HEARTBEAT_ID:
            SubTask_Segment_Flasher_Animate_Function__Heartbeat();
          break;
          /**
           * Noise
           **/
          case EFFECTS_FUNCTION_WLED_FILLNOISE8_ID:
            SubTask_Segment_Flasher_Animate_Function__FillNoise8();
          break;
          case EFFECTS_FUNCTION_WLED_NOISE16_1_ID:
            SubTask_Segment_Flasher_Animate_Function__Noise16_1();
          break;
          case EFFECTS_FUNCTION_WLED_NOISE16_2_ID:
            SubTask_Segment_Flasher_Animate_Function__Noise16_2();
          break;
          case EFFECTS_FUNCTION_WLED_NOISE16_3_ID:
            SubTask_Segment_Flasher_Animate_Function__Noise16_3();
          break;
          case EFFECTS_FUNCTION_WLED_NOISE16_4_ID:
            SubTask_Segment_Flasher_Animate_Function__Noise16_4();
          break;
          case EFFECTS_FUNCTION_WLED_NOISEPAL_ID:
            SubTask_Segment_Flasher_Animate_Function__Noise_Pal();
          break;
          /**
           * Scan
           **/
          case EFFECTS_FUNCTION_WLED_SCAN_ID:
            SubTask_Segment_Flasher_Animate_Function__Scan();
          break;
          case EFFECTS_FUNCTION_WLED_DUAL_SCAN_ID:
            SubTask_Segment_Flasher_Animate_Function__Scan_Dual();
          break;
          case EFFECTS_FUNCTION_WLED_LARSON_SCANNER_ID:
            SubTask_Segment_Flasher_Animate_Function__Larson_Scanner();
          break;
          case EFFECTS_FUNCTION_WLED_DUAL_LARSON_SCANNER_ID:
            SubTask_Segment_Flasher_Animate_Function__Larson_Scanner_Dual();
          break;
          case EFFECTS_FUNCTION_WLED_ICU_ID:
            SubTask_Segment_Flasher_Animate_Function__ICU();
          break;
          case EFFECTS_FUNCTION_WLED_RIPPLE_ID:
            SubTask_Segment_Flasher_Animate_Function__Ripple();
          break;
          case EFFECTS_FUNCTION_WLED_COMET_ID:
            SubTask_Segment_Flasher_Animate_Function__Comet();
          break;
          case EFFECTS_FUNCTION_WLED_CHUNCHUN_ID:
            SubTask_Segment_Flasher_Animate_Function__Chunchun();
          break;
          case EFFECTS_FUNCTION_WLED_BOUNCINGBALLS_ID:
            SubTask_Segment_Flasher_Animate_Function__Bouncing_Balls();
          break;
          case EFFECTS_FUNCTION_WLED_SINELON_ID:
            SubTask_Segment_Flasher_Animate_Function__Sinelon();
          break;
          case EFFECTS_FUNCTION_WLED_SINELON_DUAL_ID:
            SubTask_Segment_Flasher_Animate_Function__Sinelon_Dual();
          break;
          case EFFECTS_FUNCTION_WLED_SINELON_RAINBOW_ID:
            SubTask_Segment_Flasher_Animate_Function__Sinelon_Rainbow();
          break;
          case EFFECTS_FUNCTION_WLED_DRIP_ID:
            SubTask_Segment_Flasher_Animate_Function__Drip();
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

        /**
         * If effect is from WLED, then disable animator and write output
         **/
        if(
          (_segments[segment_iters.index].effect_id >= EFFECTS_FUNCTION_WLED_STATIC_ID) &&
          (_segments[segment_iters.index].effect_id <= EFFECTS_FUNCTION_WLED_DRIP_ID)
        ){          
          _segment_runtimes[segment_iters.index].animation_has_anim_callback = false; // When no animation callback is needed
          StripUpdate();
        }

        // Configure animator to show output
        StartSegmentAnimation_AsAnimUpdateMemberFunction(segment_iters.index);       

        _segment_runtimes[segment_iters.index].call++; // Used as progress counter for animations eg rainbow across all hues
                 
      }//end if update reached

    } //  if(_segments[segment_iters.index].isActive())

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
    for (segment_iters.index =0;segment_iters.index<MAX_NUM_SEGMENTS;segment_iters.index++)
    {

      if(_segments[segment_iters.index].isActive())
      {

        if (_segment_runtimes[segment_iters.index].animator->IsAnimating())
        {
          // for now, a global segment_index needs set before each animation update

          // DEBUG_LINE_HERE;
          _segment_runtimes[segment_iters.index].animator->UpdateAnimations(segment_iters.index);
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
  
  _segment_runtimes[segment_index].animation_has_anim_callback = true;

  return *this;
}



void mAnimatorLight::Segments_UpdateStartingColourWithGetPixel()
{
  
  uint16_t start_pixel = _segments[segment_iters.index].pixel_range.start;
  uint16_t end_pixel = _segments[segment_iters.index].pixel_range.stop;
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

  if(_segment_runtimes[segment_index].animation_has_anim_callback == true)
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

  uint8_t segment_index = segment_iters.index;
  
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

        uint16_t start_pixel = _segments[segment_iters.index].pixel_range.start;
        uint16_t stop_pixel = _segments[segment_iters.index].pixel_range.stop;

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
 * Bypasses the need to indexing and desiredcolour arrays, simply sets output to be inorder repeats of palette
 * 
 * */
void mAnimatorLight::Segments_SetPixelColor_To_Static_Pallete(uint16_t palette_id)
{
  // Update pointer of struct
  mPaletteI->SetPaletteListPtrFromID(palette_id);
  // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "UpdateDesiredColourFromPaletteSelected fMapIDs_Type %d"),mPaletteI->palettelist.ptr->flags.fMapIDs_Type);// \"%s\""),GetPaletteFriendlyName());
  // AddLog(LOG_LEVEL_DEBUG_MORE

  uint8_t segment_index = segment_iters.index;
  
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

    //   //apply to positiion below

    //  // SetLEDOutAmountByPercentage(_segments[0].transition.pixels_to_update_as_percentage.val);

    //   //what is this?
    //   // does this only run if above did not? ie this is default?

    //   //AddLog(LOG_LEVEL_TEST, PSTR("strip_size_requiring_update=%d"),strip_size_requiring_update);
      
    //   // Other types with random led patterns, or,
      
    //   switch(_segments[segment_index].transition.order_id)
    //   {
    //     case TRANSITION_ORDER_RANDOM_ID:
    //     {

    //       Segments_RefreshLEDIndexPattern(segment_index);

    //       // new transition, so force full update of all segment pixels
    //       if(_segments[0].flags.NewAnimationRequiringCompleteRefresh){
    //         _segments[segment_index].pixels_to_update_this_cycle = _segments[segment_index].length();
    //       }    

    //       int16_t pixel_position = -2;
    //       for(
    //         ledout.index=0;
    //         ledout.index<_segments[segment_index].pixels_to_update_this_cycle;
    //         ledout.index++
    //       ){

    //         // For random, desired pixel from map will also be random
    //         desired_pixel = random(0, mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr));
            
    //         RgbTypeColor colour = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel,&pixel_position);

    //         #ifdef DEBUG_TRACE_ANIMATOR_SEGMENTS
    //         AddLog(LOG_LEVEL_TEST, PSTR("desiredpixel%d, colour=%d,%d,%d"), desired_pixel, colour.R, colour.G, colour.B); 
    //         #endif // DEBUG_TRACE_ANIMATOR_SEGMENTS

    //         animation_colours[ledout.pattern[ledout.index]].DesiredColour = colour;

    //         if(_segments[0].flags.brightness_applied_during_colour_generation){
    //           animation_colours[ledout.pattern[ledout.index]].DesiredColour = ApplyBrightnesstoDesiredColourWithGamma(animation_colours[ledout.pattern[ledout.index]].DesiredColour,pCONT_iLight->getBriRGB_Global());
    //         }

    //       } // end for

    //     }break;
    //     default: // testing
    //     // Serial.println("default: // testing");
    //     case TRANSITION_ORDER_INORDER_ID:{
                
          // Segments_RefreshLEDIndexPattern(segment_index);

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

      //   }break;
      // }//end switch


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

        uint16_t start_pixel = _segments[segment_iters.index].pixel_range.start;
        uint16_t stop_pixel = _segments[segment_iters.index].pixel_range.stop;

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
// DEBUG_LINE_HERE;

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

  
    JsonBuilderI->Add_P(PM_JSON_HUE, _segment_runtimes[0].rgbcct_controller->getHue360());
    JsonBuilderI->Add_P(PM_JSON_SAT, _segment_runtimes[0].rgbcct_controller->getSat255());
    JsonBuilderI->Add_P(PM_JSON_BRIGHTNESS_RGB, _segment_runtimes[0].rgbcct_controller->getBrightnessRGB255());




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


/*
 * FastLED palette modes helper function. Limitation: Due to memory reasons, multiple active segments with FastLED will disable the Palette transitions
 */
void mAnimatorLight::handle_palette(void)
{

  currentPalette =  PartyColors_p; //just use this for now

  // bool singleSegmentMode = (_segment_index == _segment_index_palette_last);
  // _segment_index_palette_last = _segment_index;

  // byte paletteIndex = _segments[_segment_index].palette;
  // if (paletteIndex == 0) //default palette. Differs depending on effect
  // {
  //   switch (_segments[_segment_index].mode)
  //   {
  //     case FX_MODE_FIRE_2012  : paletteIndex = 35; break; //heat palette
  //     case FX_MODE_COLORWAVES : paletteIndex = 26; break; //landscape 33
  //     case FX_MODE_FILLNOISE8 : paletteIndex =  9; break; //ocean colors
  //     case FX_MODE_NOISE16_1  : paletteIndex = 20; break; //Drywet
  //     case FX_MODE_NOISE16_2  : paletteIndex = 43; break; //Blue cyan yellow
  //     case FX_MODE_NOISE16_3  : paletteIndex = 35; break; //heat palette
  //     case FX_MODE_NOISE16_4  : paletteIndex = 26; break; //landscape 33
  //     case FX_MODE_GLITTER    : paletteIndex = 11; break; //rainbow colors
  //     case FX_MODE_SUNRISE    : paletteIndex = 35; break; //heat palette
  //     case FX_MODE_FLOW       : paletteIndex =  6; break; //party
  //   }
  // }
  // if (_segments[_segment_index].mode >= FX_MODE_METEOR && paletteIndex == 0) paletteIndex = 4;
   
  // // paletteIndex = 43;
  // //Serial.printf("_segments[_segment_index].palette %d %d\n\r",_segments[_segment_index].palette, paletteIndex);

  // switch (paletteIndex)
  // {
  //   case 0: //default palette. Exceptions for specific effects above
  //     targetPalette = PartyColors_p; 
      
  // //Serial.printf( "targetPalette = PartyColors_p \n\r");
      
  //     break;
  //   case 1: {//periodically replace palette with a random one. Doesn't work with multiple FastLED segments
  //     if (!singleSegmentMode)
  //     {
  //       targetPalette = PartyColors_p; break; //fallback
  //     }
  //     if (millis() - _lastPaletteChange > 1000 + ((uint32_t)(255-_segments[_segment_index].intensity))*100)
  //     {
  //       targetPalette = CRGBPalette16(
  //                       CHSV(random8(), 255, random8(128, 255)),
  //                       CHSV(random8(), 255, random8(128, 255)),
  //                       CHSV(random8(), 192, random8(128, 255)),
  //                       CHSV(random8(), 255, random8(128, 255)));
  //       _lastPaletteChange = millis();
  //     } break;}
  //   case 2: {//primary color only
  //     CRGB prim = col_to_crgb(SEGCOLOR(0));
  //     targetPalette = CRGBPalette16(prim); break;}
  //   case 3: {//primary + secondary
  //     CRGB prim = col_to_crgb(SEGCOLOR(0));
  //     CRGB sec  = col_to_crgb(SEGCOLOR(1));
  //     targetPalette = CRGBPalette16(prim,prim,sec,sec); break;}
  //   case 4: {//primary + secondary + tertiary
  //     CRGB prim = col_to_crgb(SEGCOLOR(0));
  //     CRGB sec  = col_to_crgb(SEGCOLOR(1));
  //     CRGB ter  = col_to_crgb(SEGCOLOR(2));
  //     targetPalette = CRGBPalette16(ter,sec,prim); break;}
  //   case 5: {//primary + secondary (+tert if not off), more distinct
  //     CRGB prim = col_to_crgb(SEGCOLOR(0));
  //     CRGB sec  = col_to_crgb(SEGCOLOR(1));
  //     if (SEGCOLOR(2)) {
  //       CRGB ter = col_to_crgb(SEGCOLOR(2));
  //       targetPalette = CRGBPalette16(prim,prim,prim,prim,prim,sec,sec,sec,sec,sec,ter,ter,ter,ter,ter,prim);
  //     } else {
  //       targetPalette = CRGBPalette16(prim,prim,prim,prim,prim,prim,prim,prim,sec,sec,sec,sec,sec,sec,sec,sec);
  //     }
  //     break;}
  //   case 6: //Party colors
  //     targetPalette = PartyColors_p; break;
  //   case 7: //Cloud colors
  //     targetPalette = CloudColors_p; break;
  //   case 8: //Lava colors
  //     targetPalette = LavaColors_p; break;
  //   case 9: //Ocean colors
  //     targetPalette = OceanColors_p; break;
  //   case 10: //Forest colors
  //     targetPalette = ForestColors_p; break;
  //   case 11: //Rainbow colors
  //     targetPalette = RainbowColors_p; break;
  //   case 12: //Rainbow stripe colors
  //     targetPalette = RainbowStripeColors_p; break;
  //   default: //progmem palettes
  //     load_gradient_palette(paletteIndex -13);
  // }
  
  // if (singleSegmentMode && paletteFade) //only blend if just one segment uses FastLED mode
  // {
  //   nblendPaletteTowardPalette(currentPalette, targetPalette, 48);
  // } else
  // {
  //   currentPalette = targetPalette;
  // }


}





/*
 * Returns a new, random wheel index with a minimum distance of 42 from pos.
 */
uint8_t mAnimatorLight::get_random_wheel_index(uint8_t pos) {
  uint8_t r = 0, x = 0, y = 0, d = 0;

  while(d < 42) {
    r = random8();
    x = abs(pos - r);
    y = 255 - x;
    d = MIN(x, y);
  }
  return r;
}














uint16_t mAnimatorLight::triwave16(uint16_t in)
{
  if (in < 0x8000) return in *2;
  return 0xFFFF - (in - 0x8000)*2;
}

uint16_t mAnimatorLight::mode_palette()
{
  uint8_t segment_index  = segment_iters.index;
  uint8_t segment_length = _segments[segment_index].length();
  uint16_t start_pixel   = _segments[segment_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint16_t counter = 0;
  if (_segments[segment_iters.index].speed() != 0) 
  {
    counter = (now * ((_segments[segment_index].speed() >> 3) +1)) & 0xFFFF;
    counter = counter >> 8;
  }
  
  bool noWrap = (paletteBlend == 2 || (paletteBlend == 0 && _segments[segment_index].speed() == 0));
  for (uint16_t i = 0; i < segment_length; i++)
  {
    uint8_t colorIndex = (i * 255 / segment_length) - counter;
    
    if (noWrap) colorIndex = map(colorIndex, 0, 255, 0, 240); //cut off blend at palette "end"
    
    SetPixelColor(i, color_from_palette(colorIndex, false, true, 255));
  }
  return FRAMETIME;
}



/********************************************************************************************************************************
**********Flasher Function ******************************************************************************************************
********************************************************************************************************************************/
void mAnimatorLight::CommandSet_Flasher_FunctionID(uint8_t value, uint8_t segment_index ){
  

  _segment_runtimes[segment_index].reset();

  _segments[segment_index].effect_id = value;      //make function "changeFlasherFunction" so then the region is automatically updated internally
  _segment_runtimes[segment_index].aux0 = EFFECTS_REGION_COLOUR_SELECT_ID;
  _segments[segment_index].flags.animator_first_run= true; // first run, so do extra things

  
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




