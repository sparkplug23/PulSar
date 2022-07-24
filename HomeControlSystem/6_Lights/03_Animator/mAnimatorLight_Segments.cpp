#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

uint16_t mAnimatorLight::_usedSegmentData = 0;


void mAnimatorLight::Init_Segments()
{
  
  resetSegments();

  Init_Segments_RgbcctControllers();

}

void mAnimatorLight::Init_Segments_RgbcctControllers()
{

  ALOG_WRN(PSTR("This likely needs changed, since rgbcct can be in ANY segment, needs new method"))
  
  _segment_runtimes[0].rgbcct_controller->setRgbcctColourOutputAddress(mPaletteI->palettelist.rgbcct_users[0].colour_map_id);
  _segment_runtimes[1].rgbcct_controller->setRgbcctColourOutputAddress(mPaletteI->palettelist.rgbcct_users[1].colour_map_id);
  _segment_runtimes[2].rgbcct_controller->setRgbcctColourOutputAddress(mPaletteI->palettelist.rgbcct_users[2].colour_map_id);
  _segment_runtimes[3].rgbcct_controller->setRgbcctColourOutputAddress(mPaletteI->palettelist.rgbcct_users[3].colour_map_id);
  _segment_runtimes[4].rgbcct_controller->setRgbcctColourOutputAddress(mPaletteI->palettelist.rgbcct_users[4].colour_map_id);

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
    //  _segment_runtimes[i].rgbcct_controller->setRgbcctColourOutputAddress(mPaletteI->palettelist.rgbcct_users[0].colour_map_id);


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
  _segments[0].pixel_range.stop = STRIP_SIZE_MAX-1;

}



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
  for(
    int ii = 0; 
    ii<MAX_NUM_SEGMENTS; 
    ii++
  ){

    segment_active_index = ii; // Important! Otherwise segment_active_index can be incremented out of scope when a later callback is called

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

    if(_segments[segment_active_index].isActive())// || (pCONT_set->Settings.light_settings.type < LT_LIGHT_INTERFACE_END))
    {     

      if((mTime::TimeReached(&SEGENV.tSaved_AnimateRunTime, _segments[segment_active_index].transition.rate_ms))||(_segments[0].flags.fForceUpdate))
      {

        if(SEGMENT.flags.fForceUpdate){ SEGMENT.flags.fForceUpdate=false;
          SEGENV.tSaved_AnimateRunTime = millis();
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
          #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
          case EFFECTS_FUNCTION__ROTATING_PALETTE__ID:
            SubTask_Segment_Animation__Rotating_Palette();
          break;
          #endif
          #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
          case EFFECTS_FUNCTION__STEPPING_PALETTE__ID:
            SubTask_Segment_Animation__Stepping_Palette();
          break;
          #endif
          #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
          case EFFECTS_FUNCTION__FIREPLACE_1D_01__ID:
            SubTask_Segment_Animate__Fireplace_1D_01();
          break;
          #endif
          #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE    
          case EFFECTS_FUNCTION__STATIC_PALETTE_SPANNING_SEGMENT__ID:
            SubTask_Flasher_Animate_Function__Static_Palette_Spanning_Segment();
          break; 
          #endif
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
          #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
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
          case EFFECTS_FUNCTION__WLED_RAIN__ID:
            SubTask_Segment_Animation__Rain();
          break;
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
          case EFFECTS_FUNCTION__WLED_CANDLE_SINGLE__ID:
            SubTask_Segment_Animation__Candle_Single();
          break;     
          case EFFECTS_FUNCTION__WLED_CANDLE_MULTI__ID:
            SubTask_Segment_Animation__Candle_Multi();
          break;     
          case EFFECTS_FUNCTION__WLED_FIRE_FLICKER__ID:
            SubTask_Segment_Animation__Fire_Flicker();
          break;
          case EFFECTS_FUNCTION__WLED_SHIMMERING_PALETTE__ID:
            SubTask_Segment_Animation__Shimmering_Palette();
          break;             
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
        #ifdef ENABLE_WLED_EFFECTS
        if(
          (_segments[segment_active_index].effect_id >= EFFECTS_FUNCTION__WLED_STATIC__ID) &&
          (_segments[segment_active_index].effect_id <= EFFECTS_FUNCTION__WLED_LENGTH__ID)
        ){          
          _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed 
        }
        #endif // ENABLE_WLED_EFFECTS

        /**
         * @brief If callback is not active, then assume its StipUpdate needs to be called
         * 
         */
        if(!_segment_runtimes[segment_active_index].animation_has_anim_callback)
        {
          Serial.print("=");
          StripUpdate();         
          SEGMENT.flags.animator_first_run = false; // CHANGE to function: reset here for all WLED methods
        }
        else
        { // Configure animator to show output
          StartSegmentAnimation_AsAnimUpdateMemberFunction(segment_active_index);    
        }
                
        _segment_runtimes[segment_active_index].call++; // Used as progress counter for animations eg rainbow across all hues
                 
      }//end if update reached

    } //  if(_segments[segment_active_index].isActive())
    // else{
      
    //     #ifdef DEBUG_TARGET_ANIMATOR_SEGMENTS
    //       AddLog(LOG_LEVEL_DEBUG, PSTR("ELSE isActive pCONT_lAni->_segments[0].mode_id=%d"),pCONT_lAni->_segments[0].mode_id);
    //     #endif
    // }

  
  }

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
  uint16_t pixel_within_data_iter = 0;


  for (uint16_t pixel = 0; 
                pixel < SEGMENT.length(); 
                pixel++
  ){  
    GetTransitionColourBuffer(SEGENV.Data(), SEGENV.DataLength(), pixel, SEGMENT.colour_type, &colour_pairs);

    updatedColor = RgbTypeColor::LinearBlend(colour_pairs.StartingColour, colour_pairs.DesiredColour, param.progress);  

    // AddLog(LOG_LEVEL_TEST, PSTR("SEGMENT.length_m() %d %d"),segment_active_index,SEGMENT.length_m());

    SetPixelColor(pixel, updatedColor);

  }
  
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
void mAnimatorLight::EveryLoop(){

  
// _segments[0].effect_id = EFFECTS_FUNCTION__SEQUENTIAL_ID;
// _segments[1].effect_id = EFFECTS_FUNCTION__STEPPING_PALETTE_ID;
// _segments[2].effect_id = EFFECTS_FUNCTION__STATIC_PALETTE_ID;
// _segments[3].effect_id = EFFECTS_FUNCTION__STEPPING_PALETTE_ID;
// _segments[4].effect_id = EFFECTS_FUNCTION__STATIC_PALETTE_ID;


  if(_segments[0].flags.fEnable_Animation)
  {

    uint8_t flag_animations_needing_updated = 0;

    /**
     * Each ACTIVE segment will have its own animator to run
     * */
    // for (segment_active_index =0;segment_active_index<MAX_NUM_SEGMENTS;segment_active_index++)
    // { 
      
    for(
      int ii = 0; 
      ii<MAX_NUM_SEGMENTS; 
      ii++
    ){

      segment_active_index = ii; // Important! Otherwise segment_active_index can be incremented out of scope when a later callback is called

      if(_segments[ii].isActive())
      {

        if (_segment_runtimes[ii].animator->IsAnimating())
        {
        
          // ALOG_INF(PSTR("_segments[%d].IsAnimating()=%d\tAD%d"), ii, _segments[ii].isActive(), _segments[ii].transition.time_ms);

          _segment_runtimes[ii].animator->UpdateAnimations(ii);
          flag_animations_needing_updated++; // channels needing updated
          SEGMENT.flags.animator_first_run = false; // CHANGE to function: reset here for all my methods

          // Serial.print("@");

          //TBA: flags per segment

        }
        else // not animating
        { 
          //TBA: flags per segment
      
        }

      }//isactive

    } //loop segments

// if(flag_animations_needing_updated>0)
//     ALOG_INF(PSTR("flag_animations_needing_updated=%d"),flag_animations_needing_updated);

    if(flag_animations_needing_updated)
    {

      #ifndef ENABLE_DEVFEATURE_REMOVE_STRIPBUS_ISDIRTY_CHECK
      if(stripbus->IsDirty()){
        if(stripbus->CanShow()){ 
          // ALOG_INF(PSTR("CanShow"));
      #endif // ENABLE_DEVFEATURE_REMOVE_STRIPBUS_ISDIRTY_CHECK
      
          // Serial.print("!");
          StripUpdate();          
      #ifndef ENABLE_DEVFEATURE_REMOVE_STRIPBUS_ISDIRTY_CHECK
        }
      }
      #endif // ENABLE_DEVFEATURE_REMOVE_STRIPBUS_ISDIRTY_CHECK
      
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
      if(_segments[0].flags.fRunning)
      { // Was running
        #ifdef ENABLE_LOG_LEVEL_DEBUG
        ALOG_DBM( PSTR(D_LOG_NEO "Animation Finished")); 
        #endif
        fAnyLEDsOnOffCount = 0;
/*****
 * 
 * 
 * 
 * 
 * 
 * 
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
          pixel<SEGMENT.length();
          pixel++
  ){

    /**
     * @brief Convert segment_pixelindex to neopixel_pixelindex
     **/
// Serial.printf("pixel=%d\n\r", pixel);


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
 * This needs redoing, with a flag, to enable how I want the palette presented, and not how its encoded
 * "palette_pattern"
 * Gradient (either needs to get from palette, or equally generate it based on palette element count)
 * Single   (gets each colour in the palette, with ability to ignore indexs if they are present)
 * 
 * NEW: Segments
 * 
 * Updated method to try work with new segments, including index ranges
 * 
 * New method that relies on the data created inside segment runtime, for now, only support inorder
 * 
 * I need to figure out how to remove the ledout pattern, if I can succesfully pull the parts in that function into this, then it should not be needed
 * 
 * DELETING THIS FUNCTION
 * 
 * */
void mAnimatorLight::DynamicBuffer_Segments_UpdateDesiredColourFromPaletteSelected(uint16_t palette_id, uint8_t runtime_segment_index)
{

  /**
   * @brief 
   * Check if segment runtime has allocated buffer of expected size
   * 
   */
  if(_segment_runtimes[runtime_segment_index].Data() == nullptr)
  {
    return;
  }

  // ALOG_INF( PSTR("DynamicBuffer_Segments_UpdateDesiredColourFromPaletteSelected=%d"), palette_id );

  // Update pointer of struct
  mPaletteI->SetPaletteListPtrFromID(palette_id);
  // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "UpdateDesiredColourFromPaletteSelected fMapIDs_Type %d"),mPaletteI->palettelist.ptr->flags.fMapIDs_Type);// \"%s\""),GetPaletteFriendlyName());
  // AddLog(LOG_LEVEL_DEBUG_MORE

  uint8_t segment_index = segment_active_index;
  

//tmp force
  // _segments[segment_index].transition.order_id = TRANSITION_ORDER_INORDER_ID;

  /**
   * Handle the retrieval of colours from palettes depending on the palette encoding type
   * 
   * 
   * 
   * FLIP THESE
   * 
   * If effect comes first, then maptype, then map can be pushed into GetColourPalette and then is doesnt care 
   * 
   * 
   * 
   * */
  switch(mPaletteI->palettelist.ptr->flags.fMapIDs_Type){
    // default:
    //   #ifdef ENABLE_LOG_LEVEL_DEBUG
      // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "default"));
    //   #endif
    // case MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX__ID:

    default:
    case MAPIDS_TYPE_RGBCOLOUR_NOINDEX__ID:
    case MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX__ID:
    case MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID: ///to be tested
    {
    //get colour above

      //apply to positiion below

     // SetLEDOutAmountByPercentage(_segments[0].transition.pixels_to_update_as_percentage.val);

      //what is this?
      // does this only run if above did not? ie this is default?

      // AddLog(LOG_LEVEL_TEST, PSTR("strip_size_requiring_update=%d"),strip_size_requiring_update);
      
      // Other types with random led patterns, or,
      

/**
 * @brief Should INORDER/RANDOMM even be an option anymore, its basically an effect?
 * This can also be summed up with a passing of variable for if the indexing should be inroder/random
 * 
 */

// #ifndef ENABLE_DEVFEATURE_PHASEOUT_TRANSITION_ORDER
//       switch(_segments[segment_index].transition.order_id)
//       {
//         case TRANSITION_ORDER__RANDOM__ID:
//         {
// #endif // ENABLE_DEVFEATURE_PHASEOUT_TRANSITION_ORDER
          // Segments_RefreshLEDIndexPattern(segment_index);

          // new transition, so force full update of all segment pixels
          if(_segments[0].flags.NewAnimationRequiringCompleteRefresh){ // A GOOD WAY AROUND THIS WOULD BE (IF ANIMATION FIRST RUN) THEN USE INORDER METHOD TO FIRST DRAW ALL PIXELS, then start chaning
            _segments[segment_index].pixels_to_update_this_cycle = _segments[segment_index].length();
          }    

          uint16_t start_pixel = _segments[segment_index].pixel_range.start;
          uint16_t end_pixel = _segments[segment_index].pixel_range.stop;


/**
 * @brief 
 * Intensity should be "percentage" in the future
 **/

          uint8_t percentage = 20; // forced 20, but there is a command for this

          #ifdef ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
          percentage = constrain(_segments[segment_index].intensity(),0,100); // limit to be "percentage"
          #endif // ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS

          #ifdef ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
          uint16_t pixels_in_segment = _segments[segment_index].length_m(); //7-0 = 7, but this is 8 pixels
          #else
          uint16_t pixels_in_segment = _segments[segment_index].length();
          #endif // ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
          uint16_t pixels_to_update_as_number = mapvalue(percentage, 0,100, 0,pixels_in_segment);
          // _segments[segment_index].pixels_to_update_this_cycle = ;

          #ifdef ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
          //ALOG_INF( PSTR("percentage{%d},intensity{%d},pixels_in_segment{%d},pixels_to_update_as_number{%d}"), percentage, _segments[segment_index].intensity(), pixels_in_segment, pixels_to_update_as_number);
          #endif // ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS


          uint8_t pixel_position = 0;
          uint16_t pixel_index = 0;
          desired_pixel=0;
          for (uint16_t iter = 0; iter < pixels_to_update_as_number; iter++)
          {
            /**
             * @brief 
             * min: lower bound of the random value, inclusive (optional).
             * max: upper bound of the random value, exclusive.
            **/
            #ifdef ENABLE_DEVFEATURE_ENABLE_SEGMENT_PIXEL_INDEX_SHIFT
            pixel_index = random(start_pixel, end_pixel+1);
            #else
            pixel_index = random(0, pixels_in_segment+1); //indexing must be relative to buffer
            #endif

            #ifdef ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
            //ALOG_INF( PSTR("[%d] pixel_index{%d} = random( start_pixel{%d}, end_pixel{%d})"), iter, pixel_index, start_pixel, end_pixel );
            #endif // ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS

            // For random, desired pixel from map will also be random
            desired_pixel = random(0, mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr));
            
            RgbTypeColor colour = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel,&pixel_position);

            #ifdef DEBUG_TRACE_ANIMATOR_SEGMENTS
            AddLog(LOG_LEVEL_TEST, PSTR("desiredpixel%d, colour=%d,%d,%d"), desired_pixel, colour.R, colour.G, colour.B); 
            #endif // DEBUG_TRACE_ANIMATOR_SEGMENTS

            if(_segments[segment_active_index].flags.brightness_applied_during_colour_generation){
              colour = ApplyBrightnesstoDesiredColourWithGamma(colour, pCONT_iLight->getBriRGB_Global());
            }

            #ifdef ENABLE_DEVFEATURE_ENABLE_SEGMENT_PIXEL_INDEX_SHIFT
            pixel_index = pixel_index + start_pixel; // The "pixel_index" and "data/dataLen" are indexed inside the buffer, but needs shifting for complete output relative to start_pixel index
            #endif

            SetTransitionColourBuffer_DesiredColour(_segment_runtimes[segment_index].Data(), _segment_runtimes[segment_index].DataLength(), pixel_index, _segments[segment_index].colour_type, colour);
            // SetTransitionColourBuffer_DesiredColour(_segment_runtimes[segment_index].Data(), _segment_runtimes[segment_index].DataLength(), 7, _segments[segment_index].colour_type, RgbColor(50));
   }

//   #ifndef ENABLE_DEVFEATURE_PHASEOUT_TRANSITION_ORDER
         

//         }break;
//         default: // testing
//         // Serial.println("default: // testing");
//         case TRANSITION_ORDER__INORDER__ID:{

//           /**
//            * @brief Construct a new Segments_RefreshLEDIndexPattern object
//            * If this is being done inorder, then simply update ALL pixels, the index is not relevant
//            * 
//            */
                
//           // Segments_RefreshLEDIndexPattern(segment_index);

//           // new transition, so force full update of all segment pixels
//           if(_segments[0].flags.NewAnimationRequiringCompleteRefresh){
//             _segments[segment_index].pixels_to_update_this_cycle = _segments[segment_index].length();
//           }   

//           // AddLog(LOG_LEVEL_TEST, PSTR("_segments[segment_index].pixels_to_update_this_cycle =%d"), _segments[segment_index].pixels_to_update_this_cycle);

//           uint16_t start_pixel = _segments[segment_index].pixel_range.start;
//           uint16_t end_pixel = _segments[segment_index].pixel_range.stop;
              
//           #ifdef ENABLE_DEVFEATURE_PIXEL_MATRIX
//             ALOG_INF( PSTR("Segment: %d\t(%d,%d),(%d)"),
//               segment_index,
//               start_pixel,
//               end_pixel,
//               palette_id
//             );
//           #endif

//           #ifdef ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
//           uint16_t pixels_in_segment = _segments[segment_index].length_m(); //7-0 = 7, but this is 8 pixels
//           #endif


//           uint8_t pixel_position = 0;
//           uint16_t pixel_index = 0;
//           desired_pixel=0;
//           // for (uint16_t pixel = start_pixel; pixel <= end_pixel; pixel++)
//           for (uint16_t iter = 0; iter < pixels_in_segment; iter++) // keep internal segment index as iter
//           {

//             RgbcctColor colour = mPaletteI->GetColourFromPalette_Intermediate(mPaletteI->palettelist.ptr,desired_pixel,&pixel_position);
            
//             #ifdef DEBUG_TRACE_ANIMATOR_SEGMENTS
//             AddLog(LOG_LEVEL_TEST, PSTR("desiredpixel%d, colour=%d,%d,%d"), desired_pixel, colour.R, colour.G, colour.B); 
//             #endif // DEBUG_TRACE_ANIMATOR_SEGMENTS
            
//             // animation_colours[ledout.pattern[ledout.index]].DesiredColour = colour;

//             pixel_index  = iter;

//             if(_segments[segment_active_index].flags.brightness_applied_during_colour_generation){
//               colour = ApplyBrightnesstoDesiredColourWithGamma(colour, pCONT_iLight->getBriRGB_Global());
//             }
//             SetTransitionColourBuffer_DesiredColour(_segment_runtimes[segment_index].Data(), _segment_runtimes[segment_index].DataLength(), pixel_index, _segments[segment_index].colour_type, colour);

//             #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
//             ALOG_INF( PSTR("sIndexIO=%d %d,%d\t%d,pC %d, R%d"), segment_index, start_pixel, end_pixel, pixel_index, mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr), colour.R );
//             #endif

//             if(++desired_pixel>=mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr)){desired_pixel=0;}

//           }

//         }break;
//       }//end switch
// #endif // ENABLE_DEVFEATURE_PHASEOUT_TRANSITION_ORDER

    }break;
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX__ID:
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX__ID:
    // case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_AND_SETALL__ID:
    // case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_AND_SETALL__ID:
    {

      // Get active pixels in map
      uint16_t active_pixels_in_map = mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr); //width 2

    #ifdef ENABLE_LOG_LEVEL_INFO
      // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "active_pixels_in_map=%d"),active_pixels_in_map);
    #endif// ENABLE_LOG_LEVEL_INFO

    ALOG_INF(PSTR("Needs phased out, index getting comes from GetColourFromPaletteAdvanced and effects will decide to use the indexes or not. "));
    

      // Move across map
      uint8_t pixel_position = -2;
      for(uint16_t desired_pixel=0;desired_pixel<active_pixels_in_map;desired_pixel++){

        RgbcctColor colour = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel,&pixel_position);
        
        #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
        //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "dp%d, pp%d, %d,%d %d"),desired_pixel, pixel_position,pCONT_iLight->HueF2N(colour.H),pCONT_iLight->SatF2N(colour.S),pCONT_iLight->BrtF2N(colour.B));
        #endif
        
        if(pixel_position>=0){
          // Set output to this "many" colour
          if(pixel_position == 255){ 

ALOG_INF(PSTR("This should become an animation in its own right using mappings"));

            for(uint16_t temp=0;temp<_segments[segment_index].length();temp++){//}  ledout.length;temp++){ 
              // animation_colours[temp].DesiredColour = ApplyBrightnesstoRgbcctColour(colour,pCONT_iLight->getBriRGB_Global());
              
            if(_segments[segment_active_index].flags.brightness_applied_during_colour_generation){
              colour = ApplyBrightnesstoDesiredColourWithGamma(colour, pCONT_iLight->getBriRGB_Global());
            }
            SetTransitionColourBuffer_DesiredColour(_segment_runtimes[segment_index].Data(), _segment_runtimes[segment_index].DataLength(), temp,  _segments[segment_index].colour_type, colour);
  
            }            
            #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
            // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "set ALL %d,%d %d"),pCONT_iLight->HueF2N(colour.H),pCONT_iLight->SatF2N(colour.S),pCONT_iLight->BrtF2N(colour.B));
            #endif
          }else{
            colour.R = 1;colour.R = 2;colour.R = 3;
            // Serial.println("colour.R = 1;colour.R = 2;colour.R = 3;");
            // animation_colours[pixel_position].DesiredColour = ApplyBrightnesstoRgbcctColour(colour,pCONT_iLight->getBriRGB_Global());
            
            if(_segments[segment_active_index].flags.brightness_applied_during_colour_generation){
              colour = ApplyBrightnesstoDesiredColourWithGamma(colour, pCONT_iLight->getBriRGB_Global());
            }
            SetTransitionColourBuffer_DesiredColour(_segment_runtimes[segment_index].Data(), _segment_runtimes[segment_index].DataLength(), pixel_position,  _segments[segment_index].colour_type, colour);
  
            #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
            //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "set %d %d,%d %d"), pixel_position,pCONT_iLight->HueF2N(colour.H),pCONT_iLight->SatF2N(colour.S),pCONT_iLight->BrtF2N(colour.B));
            #endif
          }
        }else{          
          #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
          ALOG_DBM( PSTR(D_LOG_NEO "return; //end early %d"),pixel_position);
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
    case MAPIDS_TYPE_RGBCCTCOLOUR_WITHINDEX_GRADIENT__ID:
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_GRADIENT__ID:
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT__ID:{

      /***
       * Section needs thought out again to work in segments
       * Output, needs to be relative to segment only (ie 0..SEGLEN)
       * 
       * 0,255 from gradient index needs mapped in 0..SEGLEN
       * 
       * **/

      #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID NOT WORKING, needs converted to work inside segments"));
      #endif

      uint16_t start_pixel = 0;
      uint16_t end_pixel = 100;
      RgbcctColor start_colour = RgbcctColor(0);
      RgbcctColor end_colour = RgbcctColor(0);
      uint16_t desired_pixel = 0;
      uint8_t start_pixel_position = -1, end_pixel_position = -1;
      uint8_t pixels_in_map = mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr);

      //#ifdef ENABLE_LOG_LEVEL_DEBUG
      // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "pixels_in_map %d"),pixels_in_map);
      //#endif


      // Add flag to enable ignoring gradient, and filling as normal palette (here?)

      for(uint8_t grad_pair_index=0;grad_pair_index<pixels_in_map;grad_pair_index++)
      {
        
        start_colour = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel,  &start_pixel_position);
        end_colour   = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel+1,&end_pixel_position);

        AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "%d start_pixel_position %d"),grad_pair_index,start_pixel_position);
        AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "end_pixel_position %d"),end_pixel_position);

        #ifndef ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS
        start_colour = ApplyBrightnesstoRgbcctColour(start_colour,pCONT_iLight->getBriRGB_Global());
        end_colour   = ApplyBrightnesstoRgbcctColour(end_colour,pCONT_iLight->getBriRGB_Global());
        #endif // ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS

        // #ifdef ENABLE_LOG_LEVEL_DEBUG
        //  AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "s%d,%d %d %d"),pCONT_iLight->HueF2N(start_colour.H),pCONT_iLight->SatF2N(start_colour.S),pCONT_iLight->BrtF2N(start_colour.B),start_pixel_position);
        //  AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "e%d,%d %d %d"),HueF2N(end_colour.H),SatF2N(end_colour.S),BrtF2N(end_colour.B),end_pixel_position);
        // #endif

        /**
         * @brief No start/stop mapping with segments I think
         * 
         * 
         */
        uint16_t start_pixel = 0;//_segments[segment_active_index].pixel_range.start;
        uint16_t stop_pixel = SEGLEN;//_segments[segment_active_index].pixel_range.stop;

        // switch(mPaletteI->palettelist.ptr->flags.fIndexs_Type){
        //   case INDEX_TYPE_DIRECT: break; //remains the same
        //   case INDEX_TYPE_SCALED_255: 
            start_pixel_position = map(start_pixel_position,0,255,start_pixel,stop_pixel);
            end_pixel_position   = map(end_pixel_position,0,255,start_pixel,stop_pixel);
        //     // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "255 %d %d"),start_pixel_position,end_pixel_position);
        //   break;
        //   case INDEX_TYPE_SCALED_100: 
        //     start_pixel_position = map(start_pixel_position,0,100,start_pixel,stop_pixel);
        //     end_pixel_position   = map(end_pixel_position,0,100,start_pixel,stop_pixel);          
        //   break;
        // }

        // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "%d start_pixel_position %d"),grad_pair_index,start_pixel_position);
        // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "end_pixel_position %d"),end_pixel_position);

        RgbcctColor colour;

        float progress = 0;
        for(
          int 
          index=start_pixel_position;
          index<=end_pixel_position;
          index++){
        // for(
        //   int 
        //   index=0;
        //   index<SEGLEN;
        //   index++){
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
            
              // progress = mSupport::mapfloat(index,0,SEGLEN,0,1);
              progress = mSupport::mapfloat(index,start_pixel_position,end_pixel_position,0,1);
              // animation_colours[ledout.index].DesiredColour = RgbcctColor::LinearBlend(start_colour, end_colour, progress);
              colour = RgbcctColor::LinearBlend(start_colour, end_colour, progress);

              if(_segments[segment_active_index].flags.brightness_applied_during_colour_generation){
                colour = ApplyBrightnesstoDesiredColourWithGamma(colour, pCONT_iLight->getBriRGB_Global());
              }
              
              SetTransitionColourBuffer_DesiredColour(_segment_runtimes[segment_index].Data(), _segment_runtimes[segment_index].DataLength(), index,  _segments[segment_index].colour_type, colour);
  


            // }
            AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "ledout.index%d %d %d"),index,start_pixel_position,end_pixel_position);
        }

        desired_pixel++;

      }

      // Colour applied to entire DesiredColour, leaving now
      break;
      // return; //succcesfully handled

    }break;
    
  }

  #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
  ALOG_DBM( PSTR(D_LOG_NEO "DONE UpdateDesiredColourFromPaletteSelected fMapIDs_Type "));
  #endif  // LOG_LEVEL_DEBUG_MORE

} //end function UpdateDesiredColourFromPaletteSelected();


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














uint16_t mAnimatorLight::triwave16(uint16_t in)
{
  if (in < 0x8000) return in *2;
  return 0xFFFF - (in - 0x8000)*2;
}

uint16_t mAnimatorLight::mode_palette()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_index].length();
  uint16_t start_pixel   = _segments[segment_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint16_t counter = 0;
  if (_segments[segment_active_index].speed() != 0) 
  {
    counter = (millis() * ((_segments[segment_index].speed() >> 3) +1)) & 0xFFFF;
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

  if(f=='\0') return -2;
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
  flashersettings_segments.update_colour_region.refresh_secs = value; 
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_JSON_COLOUR_REFRESH_RATE)), flashersettings_segments.update_colour_region.refresh_secs);
  #endif // ENABLE_LOG_LEVEL_COMMANDS
}

#endif //USE_MODULE_LIGHTS_ANIMATOR




