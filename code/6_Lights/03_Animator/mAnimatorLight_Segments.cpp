#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

uint16_t mAnimatorLight::_usedSegmentData = 0;


void mAnimatorLight::Init_Segments()
{
  
  resetSegments();

  Init_Segments_RgbcctControllers();

  
#ifdef ENABLE_DEVFEATURE_MOVING_GETCOLOUR_AND_PALETTE_TO_RAM
uint8_t segment_index = 0;
  // loadPalette_Michael(_segments[segment_index].palette.id, segment_index);
  loadPalette_Michael(0, segment_index); //defualt to 0 for now
#endif // ENABLE_DEVFEATURE_MOVING_GETCOLOUR_AND_PALETTE_TO_RAM

}

void mAnimatorLight::Init_Segments_RgbcctControllers()
{

  ALOG_WRN(PSTR("This likely needs changed, since rgbcct can be in ANY segment, needs new method"))
  
  _segment_runtimes[0].rgbcct_controller->setRgbcctColourOutputAddress(mPaletteI->palettelist.rgbcct_users[0].data);
  _segment_runtimes[1].rgbcct_controller->setRgbcctColourOutputAddress(mPaletteI->palettelist.rgbcct_users[1].data);
  _segment_runtimes[2].rgbcct_controller->setRgbcctColourOutputAddress(mPaletteI->palettelist.rgbcct_users[2].data);
  _segment_runtimes[3].rgbcct_controller->setRgbcctColourOutputAddress(mPaletteI->palettelist.rgbcct_users[3].data);
  _segment_runtimes[4].rgbcct_controller->setRgbcctColourOutputAddress(mPaletteI->palettelist.rgbcct_users[4].data);

  // DEBUG_LINE_HERE; 

  // CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_01_ID, 0);

  
  for (uint16_t i = 0; i < MAX_NUM_SEGMENTS; i++)
  {
    _segment_runtimes[i].rgbcct_controller->setSubType(RgbcctColor_Controller::LightSubType::LIGHT_TYPE__RGBCCT__ID);
    _segment_runtimes[i].rgbcct_controller->setApplyBrightnessToOutput(false);
    if(pCONT_set->Settings.light_settings.type == LT_ADDRESSABLE_WS281X){ //RGB only
      _segment_runtimes[i].rgbcct_controller->setColorMode(RgbcctColor_Controller::LightColorModes::LIGHT_MODE_RGB);
    }else{
      _segment_runtimes[i].rgbcct_controller->setColorMode(RgbcctColor_Controller::LightColorModes::LIGHT_MODE_BOTH);
    }
    _segment_runtimes[i].rgbcct_controller->Sync();    // calculate the initial values (#8058)
    // RGB parts
    _segment_runtimes[i].rgbcct_controller->setRGB(1,2,3);
    // CCT parts
    _segment_runtimes[i].rgbcct_controller->setRGBCCTLinked(false);
    _segment_runtimes[i].rgbcct_controller->setCCT(153);

    // Set first colour as default
    //  _segment_runtimes[i].rgbcct_controller->setRgbcctColourOutputAddress(mPaletteI->palettelist.rgbcct_users[0].data);


    _segments[i].flags.brightness_applied_during_colour_generation = true;
  }



  CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_01__ID, 0);
  CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_02__ID, 1);
  CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_03__ID, 2);
  CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_04__ID, 3);
  CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_05__ID, 4);

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
  //reset segment runtimes
  for (uint8_t i = 0; i < MAX_NUM_SEGMENTS; i++) {
    _segment_runtimes[i].markForReset();
    _segment_runtimes[i].resetIfRequired();
  }


  for (uint16_t i = 0; i < MAX_NUM_SEGMENTS; i++)
  {
    _segments[i].hardware_element_colour_order.red = 1;    // GRB most popular
    _segments[i].hardware_element_colour_order.green = 0;  // GRB most popular
    _segments[i].hardware_element_colour_order.blue = 2;
    _segments[i].hardware_element_colour_order.white_cold = 3;
    _segments[i].hardware_element_colour_order.white_warm = 4;
  }


  for (uint16_t i = 0; i < MAX_NUM_SEGMENTS; i++)
  {
    _segments[i].colors[0] = RED;
    _segments[i].colors[1] = GREEN;
    _segments[i].colors[2] = BLUE;

    _segments[i].grouping = 1;
    // _segments[i].setOption(SEG_OPTION_ON, 1);
    // _segments[i].opacity = 255;
    
  }

  _segments[0].pixel_range.start = 0;
  #ifdef ENABLE_DEVFEATURE_FIX_STRIPSIZE_LENGTH_ISSUE_XMAS2022
  _segments[0].pixel_range.stop = STRIP_SIZE_MAX; //since STRIP_SIZE_MAX is total pixels, the value in "stop" will also be STRIP_SIZE_MAX
  #else
  _segments[0].pixel_range.stop = STRIP_SIZE_MAX-1; //not sure why this had -1? prob wled issue
  #endif // ENABLE_DEVFEATURE_FIX_STRIPSIZE_LENGTH_ISSUE_XMAS2022

  #ifdef USE_MODULE_LIGHTS_PWM
  _segments[0].pixel_range.stop = 2; 
  ALOG_WRN(PSTR("Force fix the stop pixel size"));
  #endif // USE_MODULE_LIGHTS_PWM

}



void mAnimatorLight::SubTask_Segments_Animation(uint8_t segment_index)
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
    //     flashersettings.function = EFFECTS_FUNCTION__SLOW_GLOW_ID;
    // #endif

  
  /**
   * Update animation "rate"
   * 
   * Test1: Apply slow glow on segment 0-50%, apply random solid to 50-100%
   * */
  
  /**
   * Subtask to handle by each segment ie "flasher" function for each segment
   * */
  // for( 
  //   int ii = 0; 
  //   ii<MAX_NUM_SEGMENTS; 
  //   ii++
  // ){

    segment_active_index = segment_index; // Important! Otherwise segment_active_index can be incremented out of scope when a later callback is called

    /**
     * @brief 
     * Temp fix that will override how isActive will not be valid is segment is a single pixel, or at length of 1, is it as 1 |= 0?
     * 
     */
    
    // #ifdef DEBUG_TARGET_ANIMATOR_SEGMENTS
    //   AddLog(LOG_LEVEL_DEBUG, PSTR("_segments[%d].isActive()=%d"),segment_active_index,_segments[segment_active_index].isActive());
    //   AddLog(LOG_LEVEL_DEBUG, PSTR("_segments[%d].istart/stop=%d %d"),segment_active_index,_segments[segment_active_index].pixel_range.start,_segments[segment_active_index].pixel_range.stop);
    // #endif

    // reset the segment runtime data if needed, called before isActive to ensure deleted
    // segment's buffers are cleared
    SEGENV.resetIfRequired();
    
    // AddLog(LOG_LEVEL_TEST, PSTR("_segments[segment_active_index].isActive() %d"),_segments[segment_active_index].isActive());

    if(_segments[segment_active_index].isActive())// || (pCONT_set->Settings.light_settings.type < LT_LIGHT_INTERFACE_END))
    {     

      if(
        #ifdef ENABLE_DEVFEATURE_FORCED_FRAMERATE_FOR_TRANSITION_SPEED_WITH_WLED_EFFECTS
        (mTime::TimeReached(&SEGENV.tSaved_AnimateRunTime, SEGMENT.get_transition_rate_ms() ))||
        #else
        (mTime::TimeReached(&SEGENV.tSaved_AnimateRunTime, _segments[segment_active_index].transition.rate_ms))||
        #endif // ENABLE_DEVFEATURE_FORCED_FRAMERATE_FOR_TRANSITION_SPEED_WITH_WLED_EFFECTS
        (_segments[0].flags.fForceUpdate)
      ){
        DEBUG_PIN1_SET(LOW);

        ALOG_DBM(PSTR("seg=%d,rate=%d,%d"),segment_active_index ,_segments[segment_active_index].transition.rate_ms, _segments[0].flags.fForceUpdate);

        /**
         * @brief Add flag that checks if animation is still running from previous call before we start another, 
         * hence, detect when frame_rate is not being met (time_ms not finishing before rate_ms)
         * */

        if(SEGMENT.flags.fForceUpdate){ 
          SEGMENT.flags.fForceUpdate=false;
          SEGENV.tSaved_AnimateRunTime = millis(); // Not reset inside TimeReached
        }
        
        // This maybe cant be global and needs placed inside each effect?
        // Not valid for long term, needs to be changed
        Set_Segment_ColourType(segment_active_index, pCONT_set->Settings.light_settings.type);
        
        /**
         * @brief To be safe, always clear and if the effect has a callback, it will be activated inside its function again.
        **/
        #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
        setCallback_ConstructJSONBody_Debug_Animations_Progress(nullptr); // clear to be reset
        #endif

        _virtualSegmentLength = _segments[segment_active_index].virtualLength();
          
        /**
         * If effect is from WLED, then Generate new colours
         **/
        #ifdef ENABLE_WLED_EFFECTS
        if(
          (_segments[segment_active_index].effect_id >= EFFECTS_FUNCTION__WLED_STATIC__ID) &&
          (_segments[segment_active_index].effect_id <= EFFECTS_FUNCTION__WLED_LENGTH__ID)
        ){          
          mPaletteI->UpdatePalette_FastLED_TargetPalette();
          // SEGMENT.transition.rate_ms = FRAMETIME_MS; // 
        }
        #endif // ENABLE_WLED_EFFECTS

        ALOG_DBM( PSTR("_segments[%d].effect_id=%d \t%d"),segment_active_index, _segments[segment_active_index].effect_id, millis()); 

        switch(_segments[segment_active_index].effect_id){
          default:
            ALOG_ERR(PSTR("Unknown Effect %d"), SEGMENT.effect_id);
          #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
          case EFFECTS_FUNCTION__SOLID_COLOUR__ID:
            SubTask_Segment_Animate_Function__Solid_Single_Colour();
          break;
          #endif
          #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
          case EFFECTS_FUNCTION__STATIC_PALETTE__ID:
            SubTask_Segment_Animate_Function__Static_Palette();
          break;
          #endif
          #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
          case EFFECTS_FUNCTION__SLOW_GLOW__ID:
            SubTask_Segment_Animate_Function__Slow_Glow();
          break;
          #endif
          #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
          case EFFECTS_FUNCTION__STATIC_GRADIENT_PALETTE__ID:
            SubTask_Segment_Animate_Function__Static_Gradient_Palette();
          break;
          #endif
          #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
          case EFFECTS_FUNCTION__WLED_CANDLE_SINGLE__ID:
            SubTask_Segment_Animation__Candle_Single();
          break;     
          case EFFECTS_FUNCTION__WLED_SHIMMERING_PALETTE__ID:
            SubTask_Segment_Animation__Shimmering_Palette();
          break;   
          #endif
          #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
          case EFFECTS_FUNCTION__ROTATING_PALETTE__ID:
            SubTask_Segment_Animation__Rotating_Palette();
          break;
          case EFFECTS_FUNCTION__ROTATING_PREVIOUS_ANIMATION__ID:
            SubTask_Segment_Animation__Rotating_Previous_Animation();
          break;
          case EFFECTS_FUNCTION__STEPPING_PALETTE__ID:
            SubTask_Segment_Animation__Stepping_Palette();
          break;
          #endif
          #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
          case EFFECTS_FUNCTION__BLEND_PALETTE_SATURATION_TO_WHITE__ID:
            SubTask_Segment_Animation__Blend_Palette_To_White();
          break;
          #endif
          #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
          case EFFECTS_FUNCTION__BLEND_PALETTE_BETWEEN_ANOTHER_PALETTE__ID:
            SubTask_Segment_Animation__Blend_Palette_Between_Another_Palette();
          break;
          #endif
          #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
          case EFFECTS_FUNCTION__TWINKLE_PALETTE_SEC_ON_ORDERED_PALETTE_PRI__ID:
            SubTask_Segment_Animation__Twinkle_Palette_Onto_Palette();
          break;
          #endif

          #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING    
          case EFFECTS_FUNCTION__STATIC_PALETTE_SPANNING_SEGMENT__ID:
            SubTask_Flasher_Animate_Function__Static_Palette_Spanning_Segment();
          break; 
          #endif        
          

          /**
           * Static
           **/
          #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
          case EFFECTS_FUNCTION__WLED_STATIC__ID:
            SubTask_Segment_Animation__Static();
          break;
          case EFFECTS_FUNCTION__WLED_STATIC_PATTERN__ID:
            SubTask_Segment_Animation__Static_Pattern();
          break;
          case EFFECTS_FUNCTION__WLED_TRI_STATIC_PATTERN__ID:
            SubTask_Segment_Animation__Tri_Static_Pattern();
          break;
          #ifdef ENABLE_EXTRA_WLED_EFFECTS
          case EFFECTS_FUNCTION__WLED_SPOTS__ID:
            SubTask_Segment_Animation__Spots();
          break;
          #endif // ENABLE_EXTRA_WLED_EFFECTS
          case EFFECTS_FUNCTION__WLED_PERCENT__ID:
            SubTask_Segment_Animation__Percent();
          break;
          /**
           * One Colour
           **/
          case EFFECTS_FUNCTION__WLED_RANDOM_COLOR__ID:
            SubTask_Segment_Animation__Random_Colour();
          break;
          /**
           * Wipe/Sweep/Runners 
           **/
          case EFFECTS_FUNCTION__WLED_COLOR_WIPE__ID:
            SubTask_Segment_Animation__Colour_Wipe();
          break;
          case EFFECTS_FUNCTION__WLED_COLOR_WIPE_RANDOM__ID:
            SubTask_Segment_Animation__Colour_Wipe_Random();
          break;
          case EFFECTS_FUNCTION__WLED_COLOR_SWEEP__ID:
            SubTask_Segment_Animation__Colour_Sweep();
          break;
          case EFFECTS_FUNCTION__WLED_COLOR_SWEEP_RANDOM__ID:
            SubTask_Segment_Animation__Colour_Sweep_Random();
          break;
          /**
           * Fireworks
           **/
          case EFFECTS_FUNCTION__WLED_FIREWORKS__ID:
            SubTask_Segment_Animation__Fireworks();
          break;
          case EFFECTS_FUNCTION__WLED_FIREWORKS_EXPLODING__ID:
            SubTask_Segment_Animation__Exploding_Fireworks();
          break;
          case EFFECTS_FUNCTION__WLED_FIREWORKS_STARBURST__ID:
            SubTask_Segment_Animation__Fireworks_Starburst();
          break;
          case EFFECTS_FUNCTION__WLED_FIREWORKS_STARBURST_GLOWS__ID:
            SubTask_Segment_Animation__Fireworks_Starburst_Glows();
          break;
          case EFFECTS_FUNCTION__WLED_RAIN__ID:
            SubTask_Segment_Animation__Rain();
          break;
          case EFFECTS_FUNCTION__WLED_FIREWORKS_EXPLODING_NO_LAUNCH__ID:
            SubTask_Segment_Animation__Exploding_Fireworks_NoLaunch();
          break;
          #ifdef ENABLE_EXTRA_WLED_EFFECTS
          case EFFECTS_FUNCTION__WLED_TRICOLOR_WIPE__ID:
            SubTask_Segment_Animation__TriColour();
          break;
          case EFFECTS_FUNCTION__WLED_ANDROID__ID:
            SubTask_Segment_Animation__Android();
          break;
          case EFFECTS_FUNCTION__WLED_RUNNING_RED_BLUE__ID:
            SubTask_Segment_Animation__Running_Red_Blue();
          break;
          case EFFECTS_FUNCTION__WLED_RUNNING_COLOR__ID:
            SubTask_Segment_Animation__Running_Colour();
          break;
          case EFFECTS_FUNCTION__WLED_RUNNING_RANDOM__ID:
            SubTask_Segment_Animation__Running_Random();
          break;
          case EFFECTS_FUNCTION__WLED_GRADIENT__ID:
            SubTask_Segment_Animation__Gradient();
          break;
          case EFFECTS_FUNCTION__WLED_LOADING__ID:
            SubTask_Segment_Animation__Loading();
          break;
          case EFFECTS_FUNCTION__WLED_POLICE__ID:
            SubTask_Segment_Animation__Police();
          break;
          case EFFECTS_FUNCTION__WLED_POLICE_ALL__ID:
            SubTask_Segment_Animation__Polce_All();
          break;
          case EFFECTS_FUNCTION__WLED_TWO_DOTS__ID:
            SubTask_Segment_Animation__Two_Dots();
          break;
          case EFFECTS_FUNCTION__WLED_TWO_AREAS__ID:
            SubTask_Segment_Animation__Two_Areas();
          break;
          case EFFECTS_FUNCTION__WLED_MULTI_COMET__ID:
            SubTask_Segment_Animation__Multi_Comet();
          break;
          case EFFECTS_FUNCTION__WLED_OSCILLATE__ID:
            SubTask_Segment_Animation__Oscillate();
          break;
          case EFFECTS_FUNCTION__WLED_BPM__ID:
            SubTask_Segment_Animation__BPM();
          break;
          case EFFECTS_FUNCTION__WLED_JUGGLE__ID:
            SubTask_Segment_Animation__Juggle();
          break;
          case EFFECTS_FUNCTION__WLED_PALETTE__ID:
            SubTask_Segment_Animation__Palette();
          break;
          case EFFECTS_FUNCTION__WLED_COLORWAVES__ID:
            SubTask_Segment_Animation__ColourWaves();
          break;
          case EFFECTS_FUNCTION__WLED_LAKE__ID:
            SubTask_Segment_Animation__Lake();
          break;
          case EFFECTS_FUNCTION__WLED_GLITTER__ID:
            SubTask_Segment_Animation__Glitter();
          break;
          case EFFECTS_FUNCTION__WLED_METEOR__ID:
            SubTask_Segment_Animation__Meteor();
          break;
          case EFFECTS_FUNCTION__WLED_METEOR_SMOOTH__ID:
            SubTask_Segment_Animation__Metoer_Smooth();
          break;
          case EFFECTS_FUNCTION__WLED_PRIDE_2015__ID:
            SubTask_Segment_Animation__Pride_2015();
          break;
          case EFFECTS_FUNCTION__WLED_PACIFICA__ID:
            SubTask_Segment_Animation__Pacifica();
          break;
          case EFFECTS_FUNCTION__WLED_SUNRISE__ID:
            SubTask_Segment_Animation__Sunrise();
          break;
          case EFFECTS_FUNCTION__WLED_SINEWAVE__ID:
            SubTask_Segment_Animation__Sinewave();
          break;
          case EFFECTS_FUNCTION__WLED_FLOW__ID:
            SubTask_Segment_Animation__Flow();
          break;
          case EFFECTS_FUNCTION__WLED_RUNNING_LIGHTS__ID:
            SubTask_Segment_Animation__Running_Lights();
          break;
          case EFFECTS_FUNCTION__WLED_RAINBOW_CYCLE__ID:
            SubTask_Segment_Animation__Rainbow_Cycle();
          break;
          case EFFECTS_FUNCTION__WLED_MERRY_CHRISTMAS__ID:
            SubTask_Segment_Animation__Merry_Christmas();
          break;
          case EFFECTS_FUNCTION__WLED_HALLOWEEN__ID:
            SubTask_Segment_Animation__Halloween();
          break;
          /**
           * Chase
           **/
          case EFFECTS_FUNCTION__WLED_CHASE_COLOR__ID:
            SubTask_Segment_Animation__Chase_Colour();
          break;
          case EFFECTS_FUNCTION__WLED_CHASE_RANDOM__ID:
            SubTask_Segment_Animation__Chase_Random();
          break;
          case EFFECTS_FUNCTION__WLED_CHASE_RAINBOW__ID:
            SubTask_Segment_Animation__Chase_Rainbow();
          break;
          case EFFECTS_FUNCTION__WLED_CHASE_FLASH__ID:
            SubTask_Segment_Animation__Chase_Flash();
          break;
          case EFFECTS_FUNCTION__WLED_CHASE_FLASH_RANDOM__ID:
            SubTask_Segment_Animation__Chase_Flash_Random();
          break;
          case EFFECTS_FUNCTION__WLED_CHASE_RAINBOW_WHITE__ID:
            SubTask_Segment_Animation__Chase_Rainbow_White();
          break;
          case EFFECTS_FUNCTION__WLED_CHASE_THEATER__ID:
            SubTask_Segment_Animation__Chase_Theater();
          break;
          case EFFECTS_FUNCTION__WLED_CHASE_THEATER_RAINBOW__ID:
            SubTask_Segment_Animation__Chase_Theatre_Rainbow();
          break;
          case EFFECTS_FUNCTION__WLED_CHASE_TRICOLOR__ID:
            SubTask_Segment_Animation__Chase_TriColour();
          break;
          case EFFECTS_FUNCTION__WLED_CIRCUS_COMBUSTUS__ID:
            SubTask_Segment_Animation__Circus_Combustus();
          break;
          /**
           *  Breathe/Fade/Pulse
           **/    
          case EFFECTS_FUNCTION__WLED_BREATH__ID:
            SubTask_Segment_Animation__Breath();
          break;
          case EFFECTS_FUNCTION__WLED_FADE__ID:
            SubTask_Segment_Animation__Fade();
          break;
          case EFFECTS_FUNCTION__WLED_FADE_TRICOLOR__ID:
            SubTask_Segment_Animation__Fade_TriColour();
          break;
          case EFFECTS_FUNCTION__WLED_FADE_SPOTS__ID:
            SubTask_Segment_Animation__Fade_Spots();
          break;
          #endif // ENABLE_EXTRA_WLED_EFFECTS
          /**
           * Sparkle/Twinkle
           **/
          #ifdef ENABLE_EXTRA_WLED_EFFECTS
          case EFFECTS_FUNCTION__WLED_SOLID_GLITTER__ID:
            SubTask_Segment_Animation__Solid_Glitter();
          break;
          case EFFECTS_FUNCTION__WLED_POPCORN__ID:
            SubTask_Segment_Animation__Popcorn();
          break;
          case EFFECTS_FUNCTION__WLED_PLASMA__ID:
            SubTask_Segment_Animation__Plasma();
          break;
          case EFFECTS_FUNCTION__WLED_SPARKLE__ID:
            SubTask_Segment_Animation__Sparkle();
          break;
          case EFFECTS_FUNCTION__WLED_FLASH_SPARKLE__ID:
            SubTask_Segment_Animation__Sparkle_Flash();
          break;
          case EFFECTS_FUNCTION__WLED_HYPER_SPARKLE__ID:
            SubTask_Segment_Animation__Sparkle_Hyper();
          break;
          case EFFECTS_FUNCTION__WLED_TWINKLE__ID:
            SubTask_Segment_Animation__Twinkle();
          break;
          case EFFECTS_FUNCTION__WLED_COLORTWINKLE__ID:
            SubTask_Segment_Animation__Twinkle_Colour();
          break;
          case EFFECTS_FUNCTION__WLED_TWINKLEFOX__ID:
            SubTask_Segment_Animation__Twinkle_Fox();
          break;
          case EFFECTS_FUNCTION__WLED_TWINKLECAT__ID:
            SubTask_Segment_Animation__Twinkle_Cat();
          break;
          case EFFECTS_FUNCTION__WLED_TWINKLEUP__ID:
            SubTask_Segment_Animation__Twinkle_Up();
          break;
          case EFFECTS_FUNCTION__WLED_DYNAMIC__ID:
            SubTask_Segment_Animation__Dynamic();
          break;
          case EFFECTS_FUNCTION__WLED_SAW__ID:
            SubTask_Segment_Animation__Saw();
          break;
          case EFFECTS_FUNCTION__WLED_DISSOLVE__ID:
            SubTask_Segment_Animation__Dissolve();
          break;
          case EFFECTS_FUNCTION__WLED_DISSOLVE_RANDOM__ID:
            SubTask_Segment_Animation__Dissolve_Random();
          break;
          case EFFECTS_FUNCTION__WLED_COLORFUL__ID:
            SubTask_Segment_Animation__ColourFul();
          break;
          case EFFECTS_FUNCTION__WLED_TRAFFIC_LIGHT__ID:
            SubTask_Segment_Animation__Traffic_Light();
          break;
          #endif // ENABLE_EXTRA_WLED_EFFECTS      
          #ifdef ENABLE_EXTRA_WLED_EFFECTS
          /**
           * Blink/Strobe
           **/
          case EFFECTS_FUNCTION__WLED_BLINK__ID:
            SubTask_Segment_Animation__Blink();
          break;
          case EFFECTS_FUNCTION__WLED_BLINK_RAINBOW__ID:
            SubTask_Segment_Animation__Blink_Rainbow();
          break;
          case EFFECTS_FUNCTION__WLED_STROBE__ID:
            SubTask_Segment_Animation__Strobe();
          break;
          case EFFECTS_FUNCTION__WLED_MULTI_STROBE__ID:
            SubTask_Segment_Animation__Strobe_Multi();
          break;
          case EFFECTS_FUNCTION__WLED_STROBE_RAINBOW__ID:
            SubTask_Segment_Animation__Strobe_Rainbow();
          break;
          case EFFECTS_FUNCTION__WLED_RAINBOW__ID:
            SubTask_Segment_Animation__Rainbow();
          break;
          case EFFECTS_FUNCTION__WLED_LIGHTNING__ID:
            SubTask_Segment_Animation__Lightning();
          break;
          case EFFECTS_FUNCTION__WLED_FIRE_2012__ID:
            SubTask_Segment_Animation__Fire_2012();
          break;
          case EFFECTS_FUNCTION__WLED_RAILWAY__ID:
            SubTask_Segment_Animation__Railway();
          break;
          case EFFECTS_FUNCTION__WLED_HEARTBEAT__ID:
            SubTask_Segment_Animation__Heartbeat();
          break;
          /**
           * Noise
           **/
          case EFFECTS_FUNCTION__WLED_FILLNOISE8__ID:
            SubTask_Segment_Animation__FillNoise8();
          break;
          case EFFECTS_FUNCTION__WLED_NOISE16_1__ID:
            SubTask_Segment_Animation__Noise16_1();
          break;
          case EFFECTS_FUNCTION__WLED_NOISE16_2__ID:
            SubTask_Segment_Animation__Noise16_2();
          break;
          case EFFECTS_FUNCTION__WLED_NOISE16_3__ID:
            SubTask_Segment_Animation__Noise16_3();
          break;
          case EFFECTS_FUNCTION__WLED_NOISE16_4__ID:
            SubTask_Segment_Animation__Noise16_4();
          break;
          case EFFECTS_FUNCTION__WLED_NOISEPAL__ID:
            SubTask_Segment_Animation__Noise_Pal();
          break;
          case EFFECTS_FUNCTION__WLED_PHASEDNOISE__ID:
            SubTask_Segment_Animation__PhasedNoise();
          break;
          case EFFECTS_FUNCTION__WLED_PHASED__ID:
            SubTask_Segment_Animation__Phased();
          break;
          /**
           * Scan
           **/
          case EFFECTS_FUNCTION__WLED_SCAN__ID:
            SubTask_Segment_Animation__Scan();
          break;
          case EFFECTS_FUNCTION__WLED_DUAL_SCAN__ID:
            SubTask_Segment_Animation__Scan_Dual();
          break;
          case EFFECTS_FUNCTION__WLED_LARSON_SCANNER__ID:
            SubTask_Segment_Animation__Larson_Scanner();
          break;
          case EFFECTS_FUNCTION__WLED_DUAL_LARSON_SCANNER__ID:
            SubTask_Segment_Animation__Larson_Scanner_Dual();
          break;
          case EFFECTS_FUNCTION__WLED_ICU__ID:
            SubTask_Segment_Animation__ICU();
          break;
          case EFFECTS_FUNCTION__WLED_RIPPLE__ID:
            SubTask_Segment_Animation__Ripple();
          break;
          case EFFECTS_FUNCTION__WLED_RIPPLE_RAINBOW__ID:
            SubTask_Segment_Animation__Ripple_Rainbow();
          break;
          case EFFECTS_FUNCTION__WLED_COMET__ID:
            SubTask_Segment_Animation__Comet();
          break;
          case EFFECTS_FUNCTION__WLED_CHUNCHUN__ID:
            SubTask_Segment_Animation__Chunchun();
          break;
          case EFFECTS_FUNCTION__WLED_BOUNCINGBALLS__ID:
            SubTask_Segment_Animation__Bouncing_Balls();
          break;
          case EFFECTS_FUNCTION__WLED_SINELON__ID:
            SubTask_Segment_Animation__Sinelon();
          break;
          case EFFECTS_FUNCTION__WLED_SINELON_DUAL__ID:
            SubTask_Segment_Animation__Sinelon_Dual();
          break;
          case EFFECTS_FUNCTION__WLED_SINELON_RAINBOW__ID:
            SubTask_Segment_Animation__Sinelon_Rainbow();
          break;
          case EFFECTS_FUNCTION__WLED_DRIP__ID:
            SubTask_Segment_Animation__Drip();
          break;
          #endif // ENABLE_EXTRA_WLED_EFFECTS
          #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
          #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
          case EFFECTS_FUNCTION__SUNPOSITION_ELEVATION_PALETTE_PROGRESS_LINEAR__ID: //blend between colours
            SubTask_Segment_Animation__SunPositions_Elevation_Palette_Progress_LinearBlend();
          break;
          #endif
          #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
          case EFFECTS_FUNCTION__SUNPOSITION_ELEVATION_PALETTE_PROGRESS_STEP__ID:   //pick nearest
            SubTask_Segment_Animation__SunPositions_Elevation_Palette_Progress_Step();
          break;
          #endif
          #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS          
          case SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01__ID: // Solar elevation gives progress along palette ( using max/min elevation on that day, as start and end of palette)
            SubTask_Segment_Animate_Function__SunPositions_Elevation_Only_RGBCCT_Palette_Indexed_Positions_01();
          break;
          #endif
          #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS    
          case EFFECTS_FUNCTION__SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01__ID:
            SubTask_Segment_Animate_Function__SunPositions_Elevation_Only_RGBCCT_Palette_Indexed_Positions_01();
          break;
          #endif
          #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS    
          case EFFECTS_FUNCTION__SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_CCT_TEMPERATURE_01__ID:
            SubTask_Segment_Animate_Function__SunPositions_Elevation_Only_Controlled_CCT_Temperature_01();
          break;   
          #endif 
          // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
          //  case EFFECTS_FUNCTION__SUNPOSITIONS_SUNRISE_ALARM_01:
          //    SubTask_Flasher_Animate_Function_SunPositions_SunRise_Alarm_01();
          //  break;
          //  case EFFECTS_FUNCTION__SUNPOSITIONS_GRADIENT_SUN_ELEVATION_AND_AZIMUTH_01:
          //    SubTask_Flasher_Animate_Function_SunPositions_Gradient_Sun_Elevation_And_Azimuth_01();
          //  break;
          //  case EFFECTS_FUNCTION__SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_01__ID:
          //    SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_01();
          //  break;
          //  case EFFECTS_FUNCTION__SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_02__ID:
          //    SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_02();
          //  break;
          //  case EFFECTS_FUNCTION__SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_03__ID:
          //    SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_03();
          //  break;
          //  case EFFECTS_FUNCTION__SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_04__ID:
          //    SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_04();
          //  break;
          //  case EFFECTS_FUNCTION__SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_05__ID:
          //    SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_05();
          //  break;
          //  case EFFECTS_FUNCTION__SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_WITH_AUGMENTED_TRANSITIONS_01__ID:
          //    SubTask_Flasher_Animate_Function_SunPositions_Elevation_Only_RGBCCT_Palette_Indexed_Positions_With_Augmented_01();
          //  break;
          /**
           * Development effects without full code 
           **/          
          //  case EFFECTS_FUNCTION__SLOW_GLOW_ON_BRIGHTNESS__ID:
          //    SubTask_Flasher_Animate_Function_Slow_Glow_On_Brightness();
          //  break;
          //  case EFFECTS_FUNCTION__FLASH_TWINKLE_SINGLE_COLOUR_RANDOM__ID:
          //    SubTask_Flasher_Animate_Function_Twinkle_SingleColour_Random();
          //  break;
          //  case EFFECTS_FUNCTION__FLASH_TWINKLE_PALETTE_COLOUR_RANDOM__ID:
          //    SubTask_Flasher_Animate_Function_Twinkle_PaletteColour_Random();
          //  break;
          //  case EFFECTS_FUNCTION__SLOW_FADE_BRIGHTNESS_ALL__ID:
          //    SubTask_Flasher_Animate_Function_Slow_Fade_Brightness_All();
          //  break;
          //  case EFFECTS_FUNCTION__SLOW_FADE_SATURATION_ALL__ID:
          //    SubTask_Flasher_Animate_Function_Slow_Fade_Saturation_All();
          //  break;
          //  case EFFECTS_FUNCTION__FADE_GRADIENT__ID:
          //    SubTask_Flasher_Animate_Function_Fade_Gradient();
          //  break;
          //  case EFFECTS_FUNCTION__PULSE_RANDOM_ON:
          //    SubTask_Flasher_Animate_Function_Pulse_Random_On();
          //  break;
          //  case EFFECTS_FUNCTION__PULSE_RANDOM_ON_TWO__ID:
          //    SubTask_Flasher_Animate_Function_Pulse_Random_On_2();
          //  break;
          // EFFECTS_FUNCTION__PULSE_RANDOM_ON_FADE_OFF_ID
          //  case EFFECTS_FUNCTION__PULSE_RANDOM_ON_FADE_OFF__ID:
          //    SubTask_Flasher_Animate_Function_Pulse_Random_On_Fade_Off();
          //  break;
          //  case EFFECTS_FUNCTION__POPPING_PALETTE_BRIGHTNESS_FROM_LOWER_TO_UPPER_BOUNDERY:
          //    SubTask_Flasher_Animate_Function_Popping_Palette_Brightness_From_Lower_To_Upper_Boundary();
          //  break;
          //  case EFFECTS_FUNCTION__TWINKLE_PALETTE_BRIGHTNESS_FROM_LOWER_TO_UPPER_AND_BACK:
          //    SubTask_Flasher_Animate_Function_Twinkle_Palette_Brightness_From_Lower_To_Upper_And_Back();
          //  break;
          
          #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
          case EFFECTS_FUNCTION__LCD_CLOCK_BASIC_01__ID:            
            SubTask_Segment_Animate_Function__LCD_Clock_Time_Basic_01();
          break;
          case EFFECTS_FUNCTION__LCD_CLOCK_BASIC_02__ID:
            SubTask_Segment_Animate_Function__LCD_Clock_Time_Basic_02();
          break;
          case EFFECTS_FUNCTION__LCD_DISPLAY_MANUAL_NUMBER_01__ID:
            SubTask_Flasher_Animate_LCD_Display_Show_Numbers_Basic_01();
          break;
          #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK     
          #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS 
          case EFFECTS_FUNCTION__NOTIFICATION_STATIC_ON__ID:
            SubTask_Segment_Animate_Function__Notification_Static_On();
          break;
          case EFFECTS_FUNCTION__NOTIFICATION_STATIC_OFF__ID:
            SubTask_Segment_Animate_Function__Notification_Static_Off();
          break;
          case EFFECTS_FUNCTION__NOTIFICATION_FADE_ON__ID:
            SubTask_Segment_Animate_Function__Notification_Fade_On();
          break;
          case EFFECTS_FUNCTION__NOTIFICATION_FADE_OFF__ID:
            SubTask_Segment_Animate_Function__Notification_Fade_Off();
          break;
          case EFFECTS_FUNCTION__NOTIFICATION_BLINKING__ID:
            SubTask_Segment_Animate_Function__Notification_Blinking();
          break;
          case EFFECTS_FUNCTION__NOTIFICATION_PULSING__ID:
            SubTask_Segment_Animate_Function__Notification_Pulsing();
          break;
          #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS

          #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING
          case EFFECTS_FUNCTION__TESTER_01__ID:
            SubTask_Flasher_Animate_Function_Tester_01();
          break; 
          case EFFECTS_FUNCTION__TESTER_02__ID:
            SubTask_Flasher_Animate_Function_Tester_02();
          break; 
          // testing to replacement
          case EFFECTS_FUNCTION__STATIC_PALETTE_NEW__ID:
            SubTask_Segment_Animate_Function__Static_Palette_New();
          break;
          case EFFECTS_FUNCTION__SLOW_GLOW_NEW__ID:
            SubTask_Segment_Animate_Function__Slow_Glow_New();
          break;
          #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING
        } //end SWITCH

        /**
         * If effect is from WLED, then disable animator and write output
         **/
        // #ifdef ENABLE_WLED_EFFECTS
        if(
          (_segments[segment_active_index].effect_id >= EFFECTS_FUNCTION__WLED_STATIC__ID) &&
          (_segments[segment_active_index].effect_id <= EFFECTS_FUNCTION__WLED_LENGTH__ID)
        ){          
          _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed 
        }
        // #endif // ENABLE_WLED_EFFECTS

        /**
         * @brief If callback is not active, then assume its StipUpdate needs to be called
         * 
         */
        if(
          (!_segment_runtimes[segment_active_index].animation_has_anim_callback)||
          (SEGENV.anim_function_callback == nullptr) // assumes direct update
        ){
          StripUpdate();         
          SEGMENT.flags.animator_first_run = false; // CHANGE to function: reset here for all WLED methods
          // ALOG_INF(PSTR("Calling"));
        }
        else
        { // Configure animator to show output
          StartSegmentAnimation_AsAnimUpdateMemberFunction(segment_active_index);   
          // First run must be reset after StartAnimation is first called 
        }
                
        _segment_runtimes[segment_active_index].call++; // Used as progress counter for animations eg rainbow across all hues

        DEBUG_PIN1_SET(HIGH);
                 
      }//end if update reached

    } //  if(_segments[segment_active_index].isActive())
    // else{
      
    //     #ifdef DEBUG_TARGET_ANIMATOR_SEGMENTS
    //       AddLog(LOG_LEVEL_DEBUG, PSTR("ELSE isActive pCONT_lAni->_segments[0].mode_id=%d"),pCONT_lAni->_segments[0].mode_id);
    //     #endif
    // }

  
  // }

} // SubTask_Effects_PhaseOut



/**
 * @brief New allocated buffers must contain colour info
 * 
 * @param param 
 */
void mAnimatorLight::AnimationProcess_Generic_AnimationColour_LinearBlend_Segments_Dynamic_Buffer(const AnimationParam& param)
{    

  RgbcctColor updatedColor = RgbcctColor(0);
  TransitionColourPairs colour_pairs;

// uint8_t randomv = random(0,10)*25;

// for(int ii=0;ii<2;ii++){
// pCONT_lAni->stripbus->SetPixelColor(ii,RgbColor(randomv,0,0));
// }
// for(int ii=9;ii<10;ii++){
// pCONT_lAni->stripbus->SetPixelColor(ii,RgbColor(0,0,randomv));
// }

  for (uint16_t pixel = 0; 
                pixel < SEGMENT.virtualLength();
                pixel++
  ){  
    GetTransitionColourBuffer(SEGENV.Data(), SEGENV.DataLength(), pixel, SEGMENT.colour_type, &colour_pairs);

    updatedColor = RgbTypeColor::LinearBlend(colour_pairs.StartingColour, colour_pairs.DesiredColour, param.progress);  


    // AddLog(LOG_LEVEL_TEST, PSTR("SEGMENT.length_m() %d %d"),segment_active_index,SEGMENT.length_m());

    SetPixelColor(pixel, updatedColor);

  }

  // #ifdef ENABLE_DEVFEATURE_DEBUG_FREEZING_SK6812
  //   SetPixelColor(0, RgbColor(0));
  // #endif // ENABLE_DEVFEATURE_DEBUG_FREEZING_SK6812


}



/**
 * @brief New allocated buffers must contain colour info
 * @brief Replicate one colour across whole string (ie scences for h801 or addressable)
 * 
 * @param param 
 */
void mAnimatorLight::AnimationProcess_Generic_SingleColour_AnimationColour_LinearBlend_Segments_Dynamic_Buffer(const AnimationParam& param)
{    

  if(SEGENV.data == nullptr)
  { 
    ALOG_ERR( PSTR("_segment_runtimes[%d].Data() == nullptr = %d"), segment_active_index);
    return;
  }

  RgbcctColor updatedColor = RgbcctColor(0);
  TransitionColourPairs colour_pairs;
  GetTransitionColourBuffer(SEGENV.Data(), SEGENV.DataLength(), 0, SEGMENT.colour_type, &colour_pairs);
  
  updatedColor = RgbcctColor::LinearBlend(colour_pairs.StartingColour, colour_pairs.DesiredColour, param.progress);    
  
  ALOG_DBM( PSTR("SEGMENT.colour_type=%d, desired_colour1=%d,%d,%d,%d,%d"),SEGMENT.colour_type,updatedColor.R,updatedColor.G,updatedColor.B,updatedColor.WC,updatedColor.WW);

  // AddLog(LOG_LEVEL_TEST, PSTR("Acolour_pairs.StartingColour=%d,%d,%d,%d,%d"),colour_pairs.StartingColour.R,colour_pairs.StartingColour.G,colour_pairs.StartingColour.B,colour_pairs.StartingColour.WC,colour_pairs.StartingColour.WW);
  // AddLog(LOG_LEVEL_TEST, PSTR("Acolour_pairs.DesiredColour=%d,%d,%d,%d,%d"),colour_pairs.DesiredColour.R,colour_pairs.DesiredColour.G,colour_pairs.DesiredColour.B,colour_pairs.DesiredColour.WC,colour_pairs.DesiredColour.WW);
    
  for (uint16_t pixel = 0; 
                pixel < SEGLEN; 
                pixel++
  ){  
    SetPixelColor(pixel, updatedColor, false);
  }
  
}




RgbcctColor mAnimatorLight::GetSegmentColour(uint8_t colour_index, uint8_t segment_index)
{

  // DEBUG_LINE_HERE;

  // #ifdef ENABLE_LOG_LEVEL_INFO
  // AddLog(LOG_LEVEL_TEST, PSTR("segment_index=%d"),segment_index);
  // #endif // ENABLE_LOG_LEVEL_INFO

  RgbcctColor colour = RgbcctColor(0);

  uint8_t white = (uint8_t)(_segments[segment_index].colors[colour_index] >> 24);
  uint8_t red   = (uint8_t)(_segments[segment_index].colors[colour_index] >> 16);
  uint8_t green = (uint8_t)(_segments[segment_index].colors[colour_index] >> 8);
  uint8_t blue  = (uint8_t)(_segments[segment_index].colors[colour_index] >> 0);

  colour = RgbcctColor(red, green, blue, white, white);

  return colour;

}




void mAnimatorLight::Set_Segment_ColourType(uint8_t segment_index, uint8_t light_type)
{

  switch(light_type)
  {
    case LT_PWM5:
      _segments[segment_index].colour_type = RgbcctColor_Controller::LightSubType::LIGHT_TYPE__RGBCCT__ID; 
    break;
    case LT_ADDRESSABLE_SK6812:
      _segments[segment_index].colour_type = RgbcctColor_Controller::LightSubType::LIGHT_TYPE__RGBW__ID; 
    break;
    default:
    case LT_ADDRESSABLE_WS281X:
      _segments[segment_index].colour_type = RgbcctColor_Controller::LightSubType::LIGHT_TYPE__RGB__ID;
    break;
  }

}



uint8_t mAnimatorLight::GetSizeOfPixel(RgbcctColor_Controller::LightSubType colour_type)
{
  switch(colour_type)
  {
    case RgbcctColor_Controller::LightSubType::LIGHT_TYPE__RGB__ID:     return 3;
    case RgbcctColor_Controller::LightSubType::LIGHT_TYPE__RGBW__ID:    
    // case RgbcctColor_Controller::LightSubType::LIGHT_TYPE_RGBC__ID:      
    return 4;
    case RgbcctColor_Controller::LightSubType::LIGHT_TYPE__RGBCCT__ID:  return 5;
  }

}

bool mAnimatorLight::SetTransitionColourBuffer_StartingColour(
  byte* buffer, 
  uint16_t buflen, 
  uint16_t pixel_index, 
  RgbcctColor_Controller::LightSubType pixel_type, 
  RgbcctColor starting_colour
){

  if(buffer == nullptr)
  {
    #ifdef ENABLE_LOG_LEVEL_ERROR
    AddLog(LOG_LEVEL_TEST, PSTR("buffer == nullptr"));
    #endif // ENABLE_LOG_LEVEL_INFO
    return false;
  } 

// note right now it is storing the full rgbcct value



    // AddLog(LOG_LEVEL_TEST, PSTR("pixel_type=%d"), pixel_type);

// note right now it is storing the full rgbcct value
uint16_t pixel_start_i = 0;


    // AddLog(LOG_LEVEL_TEST, PSTR("pixel_start_i/buflen=%d\t%d"),pixel_start_i,buflen);
  switch(pixel_type)
  {
    case RgbcctColor_Controller::LightSubType::LIGHT_TYPE__RGB__ID:
    
      pixel_start_i = pixel_index*6;

      if(pixel_start_i + 6 <= buflen)
      {
        byte* start_of_pixel_pair = &buffer[pixel_start_i];

        start_of_pixel_pair[0] = starting_colour.R;
        start_of_pixel_pair[1] = starting_colour.G;
        start_of_pixel_pair[2] = starting_colour.B;

        // start_of_pixel_pair[3] = desired_colour.R;
        // start_of_pixel_pair[4] = desired_colour.G;
        // start_of_pixel_pair[5] = desired_colour.B;
      }

    break;
    case RgbcctColor_Controller::LightSubType::LIGHT_TYPE__RGBW__ID:
    
      pixel_start_i = pixel_index*8;

      if(pixel_start_i + 8 <= buflen)
      {
        byte* start_of_pixel_pair = &buffer[pixel_start_i];

        start_of_pixel_pair[0] = starting_colour.R;
        start_of_pixel_pair[1] = starting_colour.G;
        start_of_pixel_pair[2] = starting_colour.B;
        start_of_pixel_pair[3] = starting_colour.W1;

        // start_of_pixel_pair[3] = desired_colour.R;
        // start_of_pixel_pair[4] = desired_colour.G;
        // start_of_pixel_pair[5] = desired_colour.B;
        
    // AddLog(LOG_LEVEL_TEST, PSTR("SetTransitionColourBuffer_StartingColour"));


      }

    break;
    case RgbcctColor_Controller::LightSubType::LIGHT_TYPE__RGBCCT__ID:
    
      pixel_start_i = pixel_index*10;

      if(pixel_start_i + 10 <= buflen)
      {
        byte* start_of_pixel_pair = &buffer[pixel_start_i];

        start_of_pixel_pair[0] = starting_colour.R;
        start_of_pixel_pair[1] = starting_colour.G;
        start_of_pixel_pair[2] = starting_colour.B;
        start_of_pixel_pair[3] = starting_colour.WW;
        start_of_pixel_pair[4] = starting_colour.WC;

        // start_of_pixel_pair[5] = desired_colour.R;
        // start_of_pixel_pair[6] = desired_colour.G;
        // start_of_pixel_pair[7] = desired_colour.B;
        // start_of_pixel_pair[8] = desired_colour.WW;
        // start_of_pixel_pair[9] = desired_colour.WC;
      }

    break;
    default: return false;
  }



  // uint16_t pixel_start_i =pixel_index*sizeof(TransitionColourPairs);


  //   // AddLog(LOG_LEVEL_TEST, PSTR("pixel_start_i/buflen=%d\t%d"),pixel_start_i,buflen);

  // if(pixel_start_i + 10 <= buflen)
  // {
  //   byte* start_of_pixel_pair = &buffer[pixel_start_i];

  //   start_of_pixel_pair[0] = starting_colour.R;
  //   start_of_pixel_pair[1] = starting_colour.G;
  //   start_of_pixel_pair[2] = starting_colour.B;
  //   start_of_pixel_pair[3] = starting_colour.WW;
  //   start_of_pixel_pair[4] = starting_colour.WC;

  //   // start_of_pixel_pair[5] = desired_colour.R;
  //   // start_of_pixel_pair[6] = desired_colour.G;
  //   // start_of_pixel_pair[7] = desired_colour.B;
  //   // start_of_pixel_pair[8] = desired_colour.WW;
  //   // start_of_pixel_pair[9] = desired_colour.WC;
  // }else{
  //   AddLog(LOG_LEVEL_TEST, PSTR("NOT enough memory"));
  // }
  
  // // AddLog_Array(LOG_LEVEL_TEST, PSTR("buffer"), &buffer[pixel_start_i    ] , 10);

  // // AddLog(LOG_LEVEL_TEST, PSTR("SET colour_pair[%d] = %d,%d,%d, %d,%d,%d"), 
  // //   pixel_index,
  // //   colour_pair->DesiredColour.R,
  // //   colour_pair->DesiredColour.G,
  // //   colour_pair->DesiredColour.B,
  // //   colour_pair->StartingColour.R,
  // //   colour_pair->StartingColour.G,
  // //   colour_pair->StartingColour.B
  // // );


}


bool mAnimatorLight::SetTransitionColourBuffer_DesiredColour(byte* buffer, uint16_t buflen, uint16_t pixel_index, 
RgbcctColor_Controller::LightSubType pixel_type, RgbcctColor desired_colour)
{

  if(buffer == nullptr)
  {
    #ifdef ENABLE_LOG_LEVEL_ERROR
    AddLog(LOG_LEVEL_TEST, PSTR("buffer == nullptr"));
    #endif // ENABLE_LOG_LEVEL_INFO
    return false;
  } 

// note right now it is storing the full rgbcct value
uint16_t pixel_start_i = 0;
        byte* start_of_pixel_pair = nullptr;

#ifdef ENABLE_DEVFEATURE_PIXEL_MATRIX
    AddLog(LOG_LEVEL_TEST, PSTR("[%d] \t pixel_start_i/buflen=%d\t%d"),pixel_index, pixel_start_i,buflen);
#endif 

  switch(pixel_type)
  {
    case RgbcctColor_Controller::LightSubType::LIGHT_TYPE__RGB__ID:
    
      pixel_start_i = pixel_index*6;

      if(pixel_start_i + 6 <= buflen)
      {
        start_of_pixel_pair = &buffer[pixel_start_i];

        // start_of_pixel_pair[0] = starting_colour.R;
        // start_of_pixel_pair[1] = starting_colour.G;
        // start_of_pixel_pair[2] = starting_colour.B;

        start_of_pixel_pair[3] = desired_colour.R;
        start_of_pixel_pair[4] = desired_colour.G;
        start_of_pixel_pair[5] = desired_colour.B;
      }

    break;
    case RgbcctColor_Controller::LightSubType::LIGHT_TYPE__RGBW__ID:
    
      pixel_start_i = pixel_index*8; // rgbw * 2

      if(pixel_start_i + 8 <= buflen)
      {
        start_of_pixel_pair = &buffer[pixel_start_i];

        // start_of_pixel_pair[0] = starting_colour.R;
        // start_of_pixel_pair[1] = starting_colour.G;
        // start_of_pixel_pair[2] = starting_colour.B;

        start_of_pixel_pair[4] = desired_colour.R;
        start_of_pixel_pair[5] = desired_colour.G;
        start_of_pixel_pair[6] = desired_colour.B;
        start_of_pixel_pair[7] = desired_colour.W1;
      }

    break;
    case RgbcctColor_Controller::LightSubType::LIGHT_TYPE__RGBCCT__ID:
    
      pixel_start_i = pixel_index*10;

      if(pixel_start_i + 10 <= buflen)
      {
        start_of_pixel_pair = &buffer[pixel_start_i];

        // start_of_pixel_pair[0] = starting_colour.R;
        // start_of_pixel_pair[1] = starting_colour.G;
        // start_of_pixel_pair[2] = starting_colour.B;
        // start_of_pixel_pair[3] = starting_colour.WW;
        // start_of_pixel_pair[4] = starting_colour.WC;

        start_of_pixel_pair[5] = desired_colour.R;
        start_of_pixel_pair[6] = desired_colour.G;
        start_of_pixel_pair[7] = desired_colour.B;
        start_of_pixel_pair[8] = desired_colour.WW;
        start_of_pixel_pair[9] = desired_colour.WC;
      }

    break;
    default: return false;
  }

  
  // else{
  //   AddLog(LOG_LEVEL_TEST, PSTR("NOT enough memory"));
  // }
  

//   if(start_of_pixel_pair != nullptr)
//   { 
//     // byte a = &start_of_pixel_pair[0];

//     // Serial.println(a);

// AddLog_Array(LOG_LEVEL_TEST, PSTR("start_of_pixel_pair"), start_of_pixel_pair, 8);

//   }




  // AddLog_Array(LOG_LEVEL_TEST, PSTR("buffer"), &buffer[pixel_start_i    ] , 10);

  // AddLog(LOG_LEVEL_TEST, PSTR("SET colour_pair[%d] = %d,%d,%d, %d,%d,%d"), 
  //   pixel_index,
  //   colour_pair->DesiredColour.R,
  //   colour_pair->DesiredColour.G,
  //   colour_pair->DesiredColour.B,
  //   colour_pair->StartingColour.R,
  //   colour_pair->StartingColour.G,
  //   colour_pair->StartingColour.B
  // );


}





/**
 * @brief 
 * 
 * @param allocated_buffer 
 * @param pixel_index 
 * @param pixel_type 
 * @param starting_colour 
 * @param desired_colour 
 * @return true succesfully added to buffer
 * @return false 
 */

  /**
buffer = [length_of_pixel_type * length_of_pixels * copies_of_that_pixel]

length_of_pixel_type 
     rgb = 3
     rgbw = 4
     rgbcct = 5
copies_of_that_pixel 
    ie starting and desired, so 2
length_of_pixels
    number of pixel in that segment up to (100?) then replicate and wrap output*/

bool mAnimatorLight::SetTransitionColourBuffer(byte* buffer, uint16_t buflen, uint16_t pixel_index, RgbcctColor_Controller::LightSubType pixel_type, 
RgbcctColor starting_colour, RgbcctColor desired_colour)
{

  if(buffer == nullptr)
  {
    #ifdef ENABLE_LOG_LEVEL_ERROR
    AddLog(LOG_LEVEL_TEST, PSTR("buffer == nullptr"));
    #endif // ENABLE_LOG_LEVEL_INFO
    return false;
  } 

// note right now it is storing the full rgbcct value

  uint16_t pixel_start_i =pixel_index*sizeof(TransitionColourPairs);


    // AddLog(LOG_LEVEL_TEST, PSTR("pixel_start_i/buflen=%d\t%d"),pixel_start_i,buflen);

  if(pixel_start_i + 10 <= buflen)
  {
    byte* start_of_pixel_pair = &buffer[pixel_start_i];

    start_of_pixel_pair[0] = starting_colour.R;
    start_of_pixel_pair[1] = starting_colour.G;
    start_of_pixel_pair[2] = starting_colour.B;
    start_of_pixel_pair[3] = starting_colour.WW;
    start_of_pixel_pair[4] = starting_colour.WC;

    start_of_pixel_pair[5] = desired_colour.R;
    start_of_pixel_pair[6] = desired_colour.G;
    start_of_pixel_pair[7] = desired_colour.B;
    start_of_pixel_pair[8] = desired_colour.WW;
    start_of_pixel_pair[9] = desired_colour.WC;
  }else{
    
    #ifdef ENABLE_LOG_LEVEL_WARN
    AddLog(LOG_LEVEL_TEST, PSTR("NOT enough memory"));
    #endif // ENABLE_LOG_LEVEL_INFO
  }
  
  // AddLog_Array(LOG_LEVEL_TEST, PSTR("buffer"), &buffer[pixel_start_i    ] , 10);

  // AddLog(LOG_LEVEL_TEST, PSTR("SET colour_pair[%d] = %d,%d,%d, %d,%d,%d"), 
  //   pixel_index,
  //   colour_pair->DesiredColour.R,
  //   colour_pair->DesiredColour.G,
  //   colour_pair->DesiredColour.B,
  //   colour_pair->StartingColour.R,
  //   colour_pair->StartingColour.G,
  //   colour_pair->StartingColour.B
  // );


    
  return true;


}


/**
 * @brief 
 * New method for getting colours from dynamic buffers
 * 
 * @param ptr 
 * @param pixel_num 
 * @param pixel_position 
 * @return RgbcctColor 
 */
// mAnimatorLight::TransitionColourPairs* 
void mAnimatorLight::GetTransitionColourBuffer(
  byte* buffer, 
  uint16_t buflen, 
  uint16_t pixel_index, 
  RgbcctColor_Controller::LightSubType pixel_type, 
  mAnimatorLight::TransitionColourPairs* colour  // Get and Set colours will still only store in rgb or rgbw format, for now, limited to rgb
){


  if(buffer == nullptr)
  {
    return;
  }  

uint16_t pixel_start_i = 0;

//   uint16_t pixel_pair_index_location_in_buffer = pixel_index*sizeof(TransitionColourPairs);

// // AddLog(LOG_LEVEL_TEST, PSTR("GetTransitionColourBuffer=%d \t%d"),pixel_index, pixel_pair_index_location_in_buffer );
// AddLog(LOG_LEVEL_TEST, PSTR("pixel_type=%d"),pixel_type );

    byte* c = nullptr;
//   byte* start_of_pair = &buffer[pixel_pair_index_location_in_buffer];

  // AddLog_Array(LOG_LEVEL_TEST, PSTR("Abuffer"), &start_of_pair[0] , 10);

  switch(pixel_type)
  {
    default: return;
    case RgbcctColor_Controller::LightSubType::LIGHT_TYPE__RGB__ID:
      pixel_start_i = pixel_index*6;
      c = &buffer[pixel_start_i];
      colour->StartingColour = RgbColor(c[0], c[1], c[2]);
      colour->DesiredColour  = RgbColor(c[3], c[4], c[5]);
      break;
    case RgbcctColor_Controller::LightSubType::LIGHT_TYPE__RGBW__ID:
      pixel_start_i = pixel_index*8;
      c = &buffer[pixel_start_i];
      colour->StartingColour = RgbwColor(c[0], c[1], c[2], c[3]);
      colour->DesiredColour  = RgbwColor(c[4], c[5], c[6], c[7]);
      break;
    case RgbcctColor_Controller::LightSubType::LIGHT_TYPE__RGBCCT__ID:
      pixel_start_i = pixel_index*10;
      c = &buffer[pixel_start_i];
      colour->StartingColour = RgbcctColor(c[0], c[1], c[2], c[3], c[4]);
      colour->DesiredColour  = RgbcctColor(c[5], c[6], c[7], c[8], c[9]);
    break;
  }

// AddLog(LOG_LEVEL_TEST, PSTR("colour->StartingColour=%d,%d,%d,%d,%d"),colour->StartingColour.R,colour->StartingColour.G,colour->StartingColour.B,colour->StartingColour.WC,colour->StartingColour.WW);
// AddLog(LOG_LEVEL_TEST, PSTR("colour->DesiredColour=%d,%d,%d,%d,%d"),colour->DesiredColour.R,colour->DesiredColour.G,colour->DesiredColour.B,colour->DesiredColour.WC,colour->DesiredColour.WW);

      
  // AddLog_Array(LOG_LEVEL_TEST, PSTR("Abuffer"), &start_of_pair[pixel_pair_index_location_in_buffer    ] , 10);

  // AddLog(LOG_LEVEL_TEST, PSTR("GET DesiredColour[%d] = %d,%d,%d,%d,%d"), 
  //   pixel_index,
  //   colour->DesiredColour.R,
  //   colour->DesiredColour.G,
  //   colour->DesiredColour.B,
  //   colour->DesiredColour.WW,
  //   colour->DesiredColour.WC
  // );


  return ;


} // end function


/**
 * Cleaned version for segments
 * */
void mAnimatorLight::EveryLoop()
{

  uint8_t flag_animations_needing_updated = 0;
    
  for(
    int ii = 0; 
    ii<MAX_NUM_SEGMENTS; 
    ii++
  ){

    segment_active_index = ii; // Important! Otherwise segment_active_index can be incremented out of scope when a later callback is called

    if(_segments[ii].isActive())
    {

      /**
       * @brief I might want to add a minimal backoff to stop checking this
       * So know when each animation/segment was last called
       * 
       */

      if (_segment_runtimes[ii].animator->IsAnimating())
      {

        /**
         * @brief A Backoff time is needed per animation so the DMA is not overloaded
         * 
        **/
        if(mTime::TimeReached(&_segment_runtimes[ii].tSaved_AnimateRunTime, 10))
        {
          _segment_runtimes[ii].animator->UpdateAnimations(ii);
          flag_animations_needing_updated++; // channels needing updated
        } //end TimeReached
  
        #ifdef ENABLE_DEVFEATURE_FORCED_REMOVE_091122
        // first run needs moved into runtime
        SEGMENT.flags.animator_first_run = false; // CHANGE to function: reset here for all my methods
        #endif // ENABLE_DEVFEATURE_FORCED_REMOVE_091122
      }
      else // not animating
      { 
        //TBA: flags per segment
      }

    }//isactive

  } //loop segments

  /**
   * @brief If any change has happened, update!?
   * IsDirty checks to only update if needed
   */
  if(flag_animations_needing_updated)
  {
    #ifdef ENABLE_DEVFEATURE_DEBUG_SERIAL__ANIMATION_OUTPUT
    ALOG_INF(PSTR("flag_animations_needing_updated=%d"),flag_animations_needing_updated);
    #endif

    if(stripbus->IsDirty()){    // Only update if LEDs have changed
      if(stripbus->CanShow()){  
        StripUpdate();
      }
    }
    
    #ifndef ENABLE_DEVFEATURE_FORCED_REMOVE_091122
    /**
     * THESE ALL NEED PUT INTO SEGMENTS
     * THESE ARE COMMANDS IF ANY IS ANIMATING
     * checking if animation state has changed from before (maybe check via animation state?)
     * */
    if(!_segments[0].flags.fRunning)
    {   
      #ifdef ENABLE_LOG_LEVEL_DEBUG
      ALOG_DBM( PSTR(D_LOG_NEO "Animation Started"));
      #endif
    }
    _segments[0].flags.fRunning = true; 
    fPixelsUpdated = true;
    pCONT_set->Settings.enable_sleep = false;    //Make this a function, pause sleep during animations
    //AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "~a"));
  }
  else // none need animating
  {
    
      #ifdef ENABLE_DEVFEATURE_DEBUG_FREEZING_SK6812
      // Serial.print("E");
      #endif // ENABLE_DEVFEATURE_DEBUG_FREEZING_SK6812


    if(_segments[0].flags.fRunning)
    { // Was running
      #ifdef ENABLE_LOG_LEVEL_DEBUG
      ALOG_DBM( PSTR(D_LOG_NEO "Animation Finished")); 
      #endif


      fAnyLEDsOnOffCount = 0;

/*****
 *  DISABLING THE ABILITY TO KNOW WHEN IT IS ON OR OFF, NEEDS ADDED BACK IN AGAIN WITH CHECKING ALL SEGMENTS        
 ***
  
  */
      for(int i=0;i<pCONT_iLight->settings.light_size_count;i++){ 
        if(GetPixelColor(i)!=0){ fAnyLEDsOnOffCount++; }
      }          

    }
    _segments[0].flags.fRunning = false;
    pCONT_set->Settings.enable_sleep = true;
    if(blocking_force_animate_to_complete){ //if it was running
      #ifdef ENABLE_LOG_LEVEL_DEBUG
      ALOG_DBM( PSTR(D_LOG_NEO "Animation blocking_force_animate_to_complete"));
      #endif
      blocking_force_animate_to_complete = false;
    }
#endif // ENABLE_DEVFEATURE_FORCED_REMOVE_091122
  } // flag_animations_needing_updated


// } //enabeled



  /**
   *  control relay power
   * */
  // //(pCONT_iLight->light_power_state)
  //   if(pCONT_iLight->light_power_state!=pCONT_iLight->light_power_Saved){
  //     pCONT_iLight->light_power_Saved = pCONT_iLight->light_power_state;
  //     // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "pCONT_iLight->light_power_Saved"));
  //     //pCONT->mry->CommandSet_Relay_Power(0,pCONT_iLight->light_power_state);
  //   }

  //   //ALOG_DBM( PSTR(D_LOG_NEO "file%sline%d"),__FILE__,__LINE__);
  //   DEBUG_LINE;

/**
 * Check any lights are on
 * */
  // if(mTime::TimeReached(&tSavedCheckAnyLEDsOnOff,1000)){
  //   fAnyLEDsOnOffCount = 0;
  //   for(int i=0;i<pCONT_iLight->settings.light_size_count;i++){
  //     #ifdef ENABLE_LOG_LEVEL_DEBUG
  //     ALOG_DBM( PSTR(D_LOG_NEO "fAnyLEDsOnOffCount %d"),fAnyLEDsOnOffCount);
  //     #endif
  //     if(GetPixelColor(i)!=0){
  //       fAnyLEDsOnOffCount++;
  //     }
  //   }
  //   if(fAnyLEDsOnOffCount>0){
  //     #ifdef ENABLE_LOG_LEVEL_DEBUG
  //     ALOG_DBM( PSTR(D_LOG_NEO "fAnyLEDsOnOffCount %d"),fAnyLEDsOnOffCount);
  //     #endif
  //   }
  //   // #ifdef USE_MODULE_DRIVERS_RELAY
  //   //   //pCONT->mry->CommandSet_Relay_Power(0,fAnyLEDsOnOffCount>0?1:0);
  //   // #endif
  // }
  // DEBUG_LINE;
// 

}






mAnimatorLight& mAnimatorLight::SetSegment_AnimFunctionCallback(uint8_t segment_index, ANIM_FUNCTION_SIGNATURE)
{
  // AddLog(LOG_LEVEL_INFO, PSTR("SetSegment_AnimFunctionCallback segment_index=%d"));
  this->_segment_runtimes[segment_index].anim_function_callback = anim_function_callback;
  
  _segment_runtimes[segment_index].animation_has_anim_callback = true;

  return *this;
}


/**
 * @note The indexing here relates to the buffer storage, and should NOT be confused with pixel indexing
 * 
 * This is retrieving the colour from the neopixelbuffer, so index within segment needs to know the start_index to work out pixel_index from neopixel buffer
 * ie Segment index 10, could be neopixel_index 20 for a segment start_pixel index of 10
 */
void mAnimatorLight::DynamicBuffer_Segments_UpdateStartingColourWithGetPixel()
{
// DEBUG_LINE_HERE;
  for(int pixel=0;
          // #ifdef ENABLE_DEVFEATURE_GROUPING_FIX_DEC2022
          pixel<SEGMENT.virtualLength();
          // #else
          // pixel<SEGMENT.length();
          // #endif
          pixel++
  ){

    SetTransitionColourBuffer_StartingColour( SEGENV.Data(), 
                                              SEGENV.DataLength(),
                                              pixel, 
                                              SEGMENT.colour_type, 
                                              RgbcctColor(GetPixelColor(pixel))
                                            );
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

  /**
   * @brief Before starting animation, check the override states to see if they are active
   * 
   */
  // Overwriting single _segments[0] methods, set, then clear
  if(_segments[segment_index].single_animation_override.time_ms>0)
  {
    time_tmp = _segments[segment_index].single_animation_override.time_ms;
    _segments[segment_index].single_animation_override.time_ms = 0; // reset overwrite
    ALOG_INF(PSTR("Override: TimeMs: %d"), time_tmp);
  }

  //clear forced once only flags
  if(_segments[0].flags.NewAnimationRequiringCompleteRefresh){
    _segments[0].flags.NewAnimationRequiringCompleteRefresh = false;    
  }
  
    // Serial.printf("TRANSITION_METHOD_INSTANT_ID = %d\n\r",time_tmp);

  if(time_tmp>0){
    if(NEO_ANIMATION_TIMEBASE == NEO_CENTISECONDS){
      time_tmp /= 1000;// ms to seconds
    }
  }

  if(_segment_runtimes[segment_index].animation_has_anim_callback == true)
  {
    _segment_runtimes[segment_index].animator->StartAnimation(0, time_tmp, _segment_runtimes[segment_index].anim_function_callback);
  }


} //end function


/**
 * Duplicate parameter, needs merging with above in long term if it is really the same
 * */
void mAnimatorLight::Segments_SetLEDOutAmountByPercentage(uint8_t percentage, uint8_t segment_index)
{

  //   strip_size_requiring_update = mapvalue(percentage, 0,100, 0,pCONT_iLight->settings.light_size_count);

  // // AddLog(LOG_LEVEL_TEST, PSTR(DEBUG_INSERT_PAGE_BREAK "SetLEDOutAmountByPercentage = %d"),strip_size_requiring_update);

  uint16_t pixels_in_segment = _segments[segment_index].length();

  _segments[segment_index].pixels_to_update_this_cycle = mapvalue(percentage, 0,100, 0,pixels_in_segment);

  // AddLog(LOG_LEVEL_TEST, PSTR(DEBUG_INSERT_PAGE_BREAK "SetLEDOutAmountByPercentage = %d"),strip_size_requiring_update);

}



// #ifndef ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS

// /*
//  * FastLED palette modes helper function. Limitation: Due to memory reasons, multiple active segments with FastLED will disable the Palette transitions

//  Really this is the same as my "Setpalette" as it simply changes the pointer to the new palette
//  */
// void mAnimatorLight::UpdatePalette_FastLED_TargetPalette(void)
// {

//   bool singleSegmentMode = (segment_active_index == segment_iters.index_palette_last);
//   segment_iters.index_palette_last = segment_active_index;

//   byte paletteIndex = _segments[segment_active_index].palette.id;
  
//   AddLog(LOG_LEVEL_TEST, PSTR("paletteIndex=%d"),paletteIndex);

//   /**
//    * @brief If effect should use its default, then this will internally fix to the desired type based on effect (I likely want to phase this out or move elsewhere)
//    */
//   if (paletteIndex == mPalette::PALETTELIST__DEFAULT__ID) //default palette. Differs depending on effect
//   {
//     switch (_segments[segment_active_index].mode_id)
//     {
//       case EFFECTS_FUNCTION__WLED_COLORWAVES_ID : paletteIndex = mPalette::PALETTELIST_STATIC_WLED_GRADIENT__BEACH__ID; break; //landscape 33
//       case EFFECTS_FUNCTION__WLED_GLITTER_ID    : paletteIndex = mPalette::PALETTELIST_STATIC_WLED_GRADIENT__RAINBOW_SHERBET__ID; break; //rainbow colors
//       case EFFECTS_FUNCTION__WLED_SUNRISE_ID    : paletteIndex = mPalette::PALETTELIST_STATIC_FASTLED_HEAT_COLOUR_ID; break; //heat palette
//       case EFFECTS_FUNCTION__WLED_FLOW_ID       : paletteIndex = mPalette::PALETTELIST_STATIC_FASTLED_PARTY_COLOUR_ID; break; //party
//       #ifdef ENABLE_EXTRA_WLED_EFFECTS
//       case FX_MODE_FIRE_2012  : paletteIndex = 35; break; //heat palette
//       case EFFECTS_FUNCTION__WLED_FILLNOISE8_ID : paletteIndex =  9; break; //ocean colors
//       case EFFECTS_FUNCTION__WLED_NOISE16_1_ID  : paletteIndex = 20; break; //Drywet
//       case EFFECTS_FUNCTION__WLED_NOISE16_2_ID  : paletteIndex = 43; break; //Blue cyan yellow
//       case EFFECTS_FUNCTION__WLED_NOISE16_3_ID  : paletteIndex = 35; break; //heat palette
//       case EFFECTS_FUNCTION__WLED_NOISE16_4_ID  : paletteIndex = 26; break; //landscape 33
//       #endif // ENABLE_EXTRA_WLED_EFFECTS
//     }
//   }
//   if (_segments[segment_active_index].mode_id >= EFFECTS_FUNCTION__WLED_METEOR_ID && paletteIndex == mPalette::PALETTELIST__DEFAULT__ID) paletteIndex = mPalette::PALETTELIST_STATIC_FASTLED_FOREST_COLOUR_ID;
   
//   // paletteIndex = 43;
//   //Serial.printf("_segments[_segment_index].palette %d %d\n\r",_segments[_segment_index].palette, paletteIndex);

//   switch (paletteIndex)
//   {
//     /**
//      * @brief WLED palettes
//      * 
//      */
//     default:
//     case mPalette::PALETTELIST_STATIC_FASTLED_PARTY_COLOUR__ID: //Party colors
//       targetPalette = PartyColors_p; 
//     break;
//     case mPalette::PALETTELIST_STATIC_FASTLED_CLOUD_COLOURS__ID: //Cloud colors
//       targetPalette = CloudColors_p; 
//     break;
//     case mPalette::PALETTELIST_STATIC_FASTLED_LAVA_COLOURS__ID: //Lava colors
//       targetPalette = LavaColors_p; 
//     break;
//     case mPalette::PALETTELIST_STATIC_FASTLED_OCEAN_COLOUR__ID: //Ocean colors
//       targetPalette = OceanColors_p; 
//     break;
//     case mPalette::PALETTELIST_STATIC_FASTLED_FOREST_COLOUR__ID: //Forest colors
//       targetPalette = ForestColors_p; 
//     break;
//     case mPalette::PALETTELIST_STATIC_FASTLED_RAINBOW_COLOUR__ID: //Rainbow colors
//       targetPalette = RainbowColors_p; 
//     break;
//     case mPalette::PALETTELIST_STATIC_FASTLED_RAINBOW_STRIBE_COLOUR__ID: //Rainbow stripe colors
//       targetPalette = RainbowStripeColors_p;
//     break;
//     case mPalette::PALETTELIST_STATIC_FASTLED_RANDOMISE_COLOURS__ID:
//     {// periodically replace palette with a random one. Doesn't work with multiple FastLED segments
//       if (!singleSegmentMode)
//       {
//         targetPalette = PartyColors_p; break; //fallback
//       }
//       if (millis() - _lastPaletteChange > 1000 + ((uint32_t)(255-_segments[segment_active_index].intensity()))*100)
//       {
//         targetPalette = CRGBPalette16(
//                         CHSV(random8(), 255, random8(128, 255)),
//                         CHSV(random8(), 255, random8(128, 255)),
//                         CHSV(random8(), 192, random8(128, 255)),
//                         CHSV(random8(), 255, random8(128, 255)));
//         _lastPaletteChange = millis();
//       }
//       break;
//     }
//     case mPalette::PALETTELIST_STATIC_FASTLED__BASIC_COLOURS_PRIMARY__ID: 
//     { //primary color only
//       CRGB prim = col_to_crgb(SEGCOLOR(0)); //is this stable to do? maybe since its not a pointer but instead an instance of a class
//       targetPalette = CRGBPalette16(prim); 
//     }
//     break;
//     case mPalette::PALETTELIST_STATIC_FASTLED__BASIC_COLOURS_PRIMARY_SECONDARY__ID:
//     { //primary + secondary
//       CRGB prim = col_to_crgb(SEGCOLOR(0));
//       CRGB sec  = col_to_crgb(SEGCOLOR(1));
//       targetPalette = CRGBPalette16(prim,prim,sec,sec); 
//     }
//     break;
//     case mPalette::PALETTELIST_STATIC_FASTLED__BASIC_COLOURS_PRIMARY_SECONDARY_TERTIARY__ID:
//     { //primary + secondary + tertiary
//       CRGB prim = col_to_crgb(SEGCOLOR(0));
//       CRGB sec  = col_to_crgb(SEGCOLOR(1));
//       CRGB ter  = col_to_crgb(SEGCOLOR(2));
//       targetPalette = CRGBPalette16(ter,sec,prim); 
//     }
//     break;    
//     case  mPalette::PALETTELIST_STATIC_FASTLED__BASIC_COLOURS_PRIMARY_SECONDARY_TERTIARY_REPEATED__ID:
//     { //primary + secondary (+tert if not off), more distinct
//       CRGB prim = col_to_crgb(SEGCOLOR(0));
//       CRGB sec  = col_to_crgb(SEGCOLOR(1));
//       if (SEGCOLOR(2)) {
//         CRGB ter = col_to_crgb(SEGCOLOR(2));
//         targetPalette = CRGBPalette16(prim,prim,prim,prim,prim,sec,sec,sec,sec,sec,ter,ter,ter,ter,ter,prim);
//       } else {
//         targetPalette = CRGBPalette16(prim,prim,prim,prim,prim,prim,prim,prim,sec,sec,sec,sec,sec,sec,sec,sec);
//       }
//     }
//     break;
//   }//end fo switch

//   /**
//    * @brief Fastled palettes
//    * 
//    */
//   if(IsWithinRange(paletteIndex, mPalette::PALETTELIST_STATIC_WLED_GRADIENT__SUNSET__ID,mPalette::PALETTELIST_STATIC_WLED_GRADIENT__ATLANTICA__ID))
//   {
//     uint16_t gradient_id = paletteIndex - mPalette::PALETTELIST_STATIC_WLED_GRADIENT__SUNSET__ID;
//     // AddLog(LOG_LEVEL_TEST, PSTR("gradient_id=%d"),gradient_id);
//     #ifdef ENABLE_CRGBPALETTES_IN_PROGMEM
//     load_gradient_palette(gradient_id);
//     #endif // ENABLE_CRGBPALETTES_IN_PROGMEM
//   }
  
//   if (singleSegmentMode && paletteFade) // Only blend if just one segment uses FastLED mode
//   {
//     nblendPaletteTowardPalette(currentPalette, targetPalette, 48);
//   } else
//   {
//     currentPalette = targetPalette;
//   }

//   // AddLog(LOG_LEVEL_DEBUG, PSTR("UpdatePalette_FastLED_TargetPalette paletteIndex=%d"),paletteIndex);

// }


// #ifdef ENABLE_CRGBPALETTES_IN_PROGMEM
// void mAnimatorLight::load_gradient_palette(uint8_t index)
// {
//   byte i = constrain(index, 0, GRADIENT_PALETTE_COUNT -1);
//   byte tcp[72]; //support gradient palettes with up to 18 entries
//   memcpy_P(tcp, (byte*)pgm_read_dword(&(gGradientPalettes[i])), 72);
//   targetPalette.loadDynamicGradientPalette(tcp);
// }
// #endif // ENABLE_CRGBPALETTES_IN_PROGMEM

// #endif // ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS





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
















/********************************************************************************************************************************
**********Flasher Function ******************************************************************************************************
********************************************************************************************************************************/
void mAnimatorLight::CommandSet_Flasher_FunctionID(uint8_t value, uint8_t segment_index ){
  

  // _segment_runtimes[segment_index].reset(); // Do not reset, as this makes consecutive commands lose the animation already running. 
  // It should only reset if the animation has changed?
  if(_segments[segment_index].effect_id != value)
  {
    _segment_runtimes[segment_index].markForReset();

    _segments[segment_index].effect_id = value;      //make function "changeFlasherFunction" so then the region is automatically updated internally


  }


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

  // if(f=='\0') return -2;
  if(f==0) return -2;
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
  if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION__SOLID_COLOUR__NAME_CTR)){    return EFFECTS_FUNCTION__SOLID_COLOUR__ID; }
  #endif 
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
  if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION__STATIC_PALETTE__NAME_CTR)){  return EFFECTS_FUNCTION__STATIC_PALETTE__ID; }
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
  if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION__SLOW_GLOW__NAME_CTR)){       return EFFECTS_FUNCTION__SLOW_GLOW__ID; }
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
  if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION__STATIC_PALETTE__NAME_CTR)){  return EFFECTS_FUNCTION__STATIC_PALETTE__ID; }
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED    
  if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION__STEP_THROUGH_PALETTE__NAME_CTR)){ return EFFECTS_FUNCTION__STEPPING_PALETTE__ID; }
  #endif
  #ifdef ENABLE_WLED_EFFECTS
  if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION__SHIMMERING_PALETTE__NAME_CTR)){ return EFFECTS_FUNCTION__WLED_SHIMMERING_PALETTE__ID; }
  #endif // ENABLE_WLED_EFFECTS

  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
  if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION__LCD_CLOCK_BASIC_01__NAME_CTR)){ return EFFECTS_FUNCTION__LCD_CLOCK_BASIC_01__ID; }
  if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION__SEVEN_SEGMENT_DISPLAY_MANUAL_NUMBER__NAME_CTR)){ return EFFECTS_FUNCTION__LCD_DISPLAY_MANUAL_NUMBER_01__ID; }
  #endif // 

  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     
  if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION__FIREPLACE_1D_01__NAME_CTR)){ return EFFECTS_FUNCTION__FIREPLACE_1D_01__ID; }
  #endif

  
  // if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION__SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01_NAME_CTR)){ return EFFECTS_FUNCTION__SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01_ID; }
  // if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION__SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_WITH_AUGMENTED_TRANSITIONS_01_NAME_CTR)){ return EFFECTS_FUNCTION__SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_WITH_AUGMENTED_TRANSITIONS_01_ID; }
  // if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION__SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_CCT_TEMPERATURE_01_NAME_CTR)){ return EFFECTS_FUNCTION__SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_CCT_TEMPERATURE_01_ID; }
  


    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
    if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION__NOTIFICATION_STATIC_ON__NAME_CTR)){ return EFFECTS_FUNCTION__NOTIFICATION_STATIC_ON__ID; }
    // EFFECTS_FUNCTION__NOTIFICATION_STATIC_ON__ID,
    // EFFECTS_FUNCTION__NOTIFICATION_STATIC_OFF__ID,
    // EFFECTS_FUNCTION__NOTIFICATION_FADE_ON__ID,
    // EFFECTS_FUNCTION__NOTIFICATION_FADE_OFF__ID,
    // EFFECTS_FUNCTION__NOTIFICATION_BLINKING__ID,
    // EFFECTS_FUNCTION__NOTIFICATION_PULSING__ID,
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS // SELECTIVE meaning optional extras then "of type notification"
 


  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING
  if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION__TESTER__NAME_CTR)){ return EFFECTS_FUNCTION__TESTER_01__ID; }
  #endif

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
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    case EFFECTS_FUNCTION__SOLID_COLOUR__ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION__SOLID_COLOUR__NAME_CTR);  break;
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    case EFFECTS_FUNCTION__STATIC_PALETTE__ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION__STATIC_PALETTE__NAME_CTR);  break;
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    case EFFECTS_FUNCTION__SLOW_GLOW__ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION__SLOW_GLOW__NAME_CTR);  break;
    #endif
    // case EFFECTS_FUNCTION__ROTATING_PALETTE__ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION__ROTATING_PALETTE_CTR);  break;
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
    case EFFECTS_FUNCTION__STEPPING_PALETTE__ID:  snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION__SEQUENTIAL__NAME_CTR); break;
    #endif


    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
    case EFFECTS_FUNCTION__FIREPLACE_1D_01__ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION__FIREPLACE_1D_01__NAME_CTR);  break;
    #endif

    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
    case EFFECTS_FUNCTION__LCD_CLOCK_BASIC_01__ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION__LCD_CLOCK_BASIC_01__NAME_CTR);  break;
    #endif

    // case EFFECTS_FUNCTION__SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01__ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION__SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01_NAME_CTR);  break;
    // case EFFECTS_FUNCTION__SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_WITH_AUGMENTED_TRANSITIONS_01__ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION__SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_WITH_AUGMENTED_TRANSITIONS_01_NAME_CTR);  break;
    // case EFFECTS_FUNCTION__SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_CCT_TEMPERATURE_01__ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION__SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_CCT_TEMPERATURE_01_NAME_CTR);  break;
  
  
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
    case EFFECTS_FUNCTION__LCD_DISPLAY_MANUAL_NUMBER_01__ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION__SEVEN_SEGMENT_DISPLAY_MANUAL_NUMBER__NAME_CTR);  break;
    #endif

    #ifdef ENABLE_WLED_EFFECTS
    case EFFECTS_FUNCTION__WLED_SHIMMERING_PALETTE__ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION__SHIMMERING_PALETTE__NAME_CTR);  break;
    #endif

    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
    case EFFECTS_FUNCTION__NOTIFICATION_STATIC_ON__ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION__NOTIFICATION_STATIC_ON__NAME_CTR);  break;
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS 


    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING
    case EFFECTS_FUNCTION__TESTER_01__ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION__TESTER__NAME_CTR);  break;
    #endif
  
  }
  return buffer;
}


/********************************************************************************************************************************
**********Flasher Region ******************************************************************************************************
********************************************************************************************************************************/


void mAnimatorLight::CommandSet_Flasher_UpdateColourRegion_RefreshSecs(uint8_t value, uint8_t segment_index)
{
  // flashersettings_segments.update_colour_region.refresh_secs = value; 
  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_JSON_COLOUR_REFRESH_RATE)), flashersettings_segments.update_colour_region.refresh_secs);
  // #endif // ENABLE_LOG_LEVEL_COMMANDS
}

#endif //USE_MODULE_LIGHTS_ANIMATOR




