#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

uint16_t mAnimatorLight::_usedSegmentData = 0;





void mAnimatorLight::Init_Segments()
{ 

  //reset segment runtimes
  for (segment_new &seg : segments) 
  {
    seg.markForReset();
    seg.resetIfRequired();
    
  }

  
  // // for the lack of better place enumerate ledmaps here
  // // if we do it in json.cpp (serializeInfo()) we are getting flashes on LEDs
  // // unfortunately this means we do not get updates after uploads
  // enumerateLedmaps();

  // _hasWhiteChannel = _isOffRefreshRequired = false;

  #ifdef ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
  // if busses failed to load, add default (fresh install, FS issue, ...)
  if (busses->getNumBusses() == 0) {
    DEBUG_PRINTLN(F("No busses, init default"));
    const uint8_t defDataPins[] = {DATA_PINS};
    const uint16_t defCounts[] = {PIXEL_COUNTS};
    const uint8_t defNumBusses = ((sizeof defDataPins) / (sizeof defDataPins[0]));
    const uint8_t defNumCounts = ((sizeof defCounts)   / (sizeof defCounts[0]));
    uint16_t prevLen = 0;
    for (uint8_t i = 0; i < defNumBusses && i < WLED_MAX_BUSSES; i++) {
      uint8_t defPin[] = {defDataPins[i]};
      uint16_t start = prevLen;
      uint16_t count = defCounts[(i < defNumCounts) ? i : defNumCounts -1];
      prevLen += count;
      BusConfig defCfg = BusConfig(DEFAULT_LED_TYPE, defPin, start, count, DEFAULT_LED_COLOR_ORDER, false, 0, RGBW_MODE_MANUAL_ONLY);
      busses->add(defCfg);
    }
  }

  _length = 0; // Is sstrip already set?
  for (uint8_t i=0; i<busses->getNumBusses(); i++) {
    Bus *bus = busses->getBus(i);
    if (bus == nullptr) continue;
    if (bus->getStart() + bus->getLength() > MAX_LEDS) break;
    //RGBW mode is enabled if at least one of the strips is RGBW
    _hasWhiteChannel |= bus->isRgbw();
    //refresh is required to remain off if at least one of the strips requires the refresh.
    _isOffRefreshRequired |= bus->isOffRefreshRequired();
    uint16_t busEnd = bus->getStart() + bus->getLength();
    if (busEnd > _length) _length = busEnd;
    #ifdef ESP8266
    if ((!IS_DIGITAL(bus->getType()) || IS_2PIN(bus->getType()))) continue;
    uint8_t pins[5];
    if (!bus->getPins(pins)) continue;
    BusDigital* bd = static_cast<BusDigital*>(bus);
    if (pins[0] == 3) bd->reinit();
    #endif
  }

  //initialize leds array. TBD: realloc if nr of leds change
  // if (Segment::_globalLeds) {
  //   purgeSegments(true);
  //   free(Segment::_globalLeds);
  //   Segment::_globalLeds = nullptr;
  // }
  // if (useLedsArray) {
  //   #if defined(ARDUINO_ARCH_ESP32) && defined(WLED_USE_PSRAM)
  //   if (psramFound())
  //     Segment::_globalLeds = (CRGB*) ps_malloc(sizeof(CRGB) * _length);
  //   else
  //   #endif
  //     Segment::_globalLeds = (CRGB*) malloc(sizeof(CRGB) * _length);
  //   memset(Segment::_globalLeds, 0, sizeof(CRGB) * _length);
  // }

  // //segments are created in makeAutoSegments();
  // loadCustomPalettes(); // (re)load all custom palettes
  // deserializeMap();     // (re)load default ledmap


  #endif // ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT



  

  
  resetSegments1();

  
  Init_Segments_RgbcctControllers();

  

  
#ifdef ENABLE_DEVFEATURE_MOVING_GETCOLOUR_AND_PALETTE_TO_RAM
ALOG_WRN(PSTR("Removed, might be unstable"));
// uint8_t segment_index = 0;
//   // loadPalette_Michael(SEGMENT_I(segment_index).palette.id, segment_index);
//   loadPalette_Michael(0, segment_index); //defualt to 0 for now
#endif // ENABLE_DEVFEATURE_MOVING_GETCOLOUR_AND_PALETTE_TO_RAM

}

void mAnimatorLight::Segment_AppendNew(uint16_t start_pixel, uint16_t stop_pixel, uint8_t seg_index)
{

  ALOG_INF(PSTR("Segment_AppendNew:: getSegmentsNum() %d|%d"), seg_index, getSegmentsNum());


  for (size_t s = 0; s < getSegmentsNum(); s++) 
  {

    if (seg_index >= getSegmentsNum()) {

      Serial.println(SEGMENT_I(seg_index).hardware_element_colour_order.data);


      appendSegment(Segment_New(start_pixel, stop_pixel));
      seg_index = getSegmentsNum()-1; // segments are added at the end of list
      ALOG_INF(PSTR("Segment_AppendNew::new seg_index %d"), seg_index);
    
      SEGMENT_I(seg_index).palette_container = new mPaletteContainer(0); // Needs internalised
      
      SEGMENT_I(seg_index).rgbcctcolors.clear();
      SEGMENT_I(seg_index).rgbcctcolors.push_back(RgbcctColor(255,0,0,1,2));
      SEGMENT_I(seg_index).rgbcctcolors.push_back(RgbcctColor(0,255,0,3,4));
      SEGMENT_I(seg_index).rgbcctcolors.push_back(RgbcctColor(0,0,255,5,6));
      SEGMENT_I(seg_index).rgbcctcolors.push_back(RgbcctColor(255,0,255,7,8));
      SEGMENT_I(seg_index).rgbcctcolors.push_back(RgbcctColor(255,255,0,9,10));
      
      Serial.println(SEGMENT_I(seg_index).hardware_element_colour_order.data);
    
    }

  }

}








/**
 * @brief Disables animator neopixel and configures so effect function will be called again
 * 
 * @param seg_i 
 */
void mAnimatorLight::SetSegment_AnimFunctionCallback_WithoutAnimator(uint8_t seg_i)
{
  SEGMENT_I(seg_i).anim_function_callback = nullptr; // disabled
  SEGMENT_I(seg_i).transitional = true;              // Set that basic manual animator will be called
  SEGMENT_I(seg_i).transition.rate_ms = FRAMETIME;
}




void mAnimatorLight::Init_Segments_RgbcctControllers()
{
  
  CommandSet_SegColour_RgbcctColour_Hue_360(123, 0, 0);
  CommandSet_SegColour_RgbcctColour_Sat_255(255, 0, 0);
  CommandSet_SegColour_RgbcctColour_ColourTemp_Kelvin(153, 0, 0);
  
}


void mAnimatorLight::resetSegments1() 
{

  //reset segment runtimes
  for (segment_new &seg : segments) 
  {
    seg.markForReset();
    seg.resetIfRequired();
  }

  // for (segment_new &seg : segments) 
  // {
  //   seg.hardware_element_colour_order.red = 1;    // GRB most popular
  //   seg.hardware_element_colour_order.green = 0;  // GRB most popular
  //   seg.hardware_element_colour_order.blue = 2;
  //   seg.hardware_element_colour_order.white_cold = 3;
  //   seg.hardware_element_colour_order.white_warm = 4;
  // }


  for (segment_new &seg : segments) 
  {
    // seg.colors[0] = RED;
    // seg.colors[1] = GREEN;
    // seg.colors[2] = BLUE;
    
    /**
     * @brief Adding 5 colours for each segment
     * 
     */
    seg.rgbcctcolors.clear();
    seg.rgbcctcolors.push_back(RgbcctColor(255,0,0,1,2));
    seg.rgbcctcolors.push_back(RgbcctColor(0,255,0,3,4));
    seg.rgbcctcolors.push_back(RgbcctColor(0,0,255,5,6));
    seg.rgbcctcolors.push_back(RgbcctColor(255,0,255,7,8));
    seg.rgbcctcolors.push_back(RgbcctColor(255,255,0,9,10));

    seg.grouping = 1;
    // SEGMENT_I(i).setOption(SEG_OPTION_ON, 1);
    // SEGMENT_I(i).opacity = 255;    
  }

  SEGMENT_I(0).pixel_range.start = 0;
  SEGMENT_I(0).pixel_range.stop = STRIP_SIZE_MAX; //since STRIP_SIZE_MAX is total pixels, the value in "stop" will also be STRIP_SIZE_MAX
  
  #ifdef USE_MODULE_LIGHTS_PWM
  SEGMENT_I(0).pixel_range.stop = 2; 
  ALOG_WRN(PSTR("Force fix the stop pixel size"));
  #endif // USE_MODULE_LIGHTS_PWM

}



void mAnimatorLight::SubTask_Segments_Animation()
{
  
  // Need to also have a general backoff timer so the outputs are never hit faster than neopixelbus can update
  if(!mTime::TimeReached(&tSaved_MinimumAnimateRunTime, MINIMUM_SHOW_BACKOFF_PERIOD_MS)){ return; } // Note: Override must still abide by this backoff

  _isServicing = true;
  _segment_index_primary = 0;
  for (segment_new &seg : segments) 
  {
    // #ifdef DEBUG_TARGET_ANIMATOR_SEGMENTS
    //   AddLog(LOG_LEVEL_DEBUG, PSTR("_segments[%d].isActive()=%d"),_segment_index_primary,_segments[_segment_index_primary].isActive());
    //   AddLog(LOG_LEVEL_DEBUG, PSTR("_segments[%d].istart/stop=%d %d"),_segment_index_primary,_segments[_segment_index_primary].pixel_range.start,_segments[_segment_index_primary].pixel_range.stop);
    // #endif

    // reset the segment runtime data if needed, called before isActive to ensure deleted segment's buffers are cleared
    seg.resetIfRequired();

    if (!seg.isActive()) continue;    

    if(
      (mTime::TimeReached(&seg.tSaved_AnimateRunTime, seg.get_transition_rate_ms())) ||
      (seg.flags.fForceUpdate)
    ){

      DEBUG_PIN1_SET(LOW);

      if (seg.grouping == 0) seg.grouping = 1; //sanity check == move this into wherever it gets used (ie struct functions)
    
      // ALOG_INF(PSTR("seg=%d,rate=%d,%d"),_segment_index_primary, seg.transition.rate_ms, seg.flags.fForceUpdate);

      // if (!seg.freeze) { //only run effect function if not frozen

      /**
       * @brief Add flag that checks if animation is still running from previous call before we start another, 
       * hence, detect when frame_rate is not being met (time_ms not finishing before rate_ms)
       * */

      if(seg.flags.fForceUpdate){ 
        seg.flags.fForceUpdate=false;
        seg.tSaved_AnimateRunTime = millis(); // Not reset inside TimeReached
      }

      
      #ifdef ENABLE_DEVFEATURE_ALWAYS_LOAD_PALETTE_WHEN_NOT_TRANSITIONING
      // if(!seg.transitional)  // Needs better integration before it can be used
      // {
        loadPalette_Michael(seg.palette.id, _segment_index_primary);
      // }
      #endif // ENABLE_DEVFEATURE_ALWAYS_LOAD_PALETTE_WHEN_NOT_TRANSITIONING

      
      // This maybe cant be global and needs placed inside each effect?
      // Not valid for long term, needs to be changed
      Set_Segment_ColourType(_segment_index_primary, pCONT_set->Settings.light_settings.type);
      
      /**
       * @brief To be safe, always clear and if the effect has a callback, it will be activated inside its function again.
      **/
      #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
      setCallback_ConstructJSONBody_Debug_Animations_Progress(nullptr); // clear to be reset
      #endif

      _virtualSegmentLength = seg.virtualLength();

      // ALOG_DBM( PSTR("_segments[%d].effect_id=%d \t%d"),_segment_index_primary, _segments[_segment_index_primary].effect_id, millis()); 
      // ALOG_INF( PSTR("_segments[%d].effect_id=%d \t%d"),_segment_index_primary, seg.effect_id, millis()); 

      switch(seg.effect_id){
        default:
          ALOG_ERR(PSTR("Unknown Effect %d"), seg.effect_id);
        #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
        case EFFECTS_FUNCTION__SOLID_COLOUR__ID:
          SubTask_Segment_Animate_Function__Solid_Colour();
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
        #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
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
        #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
        #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
        case EFFECTS_FUNCTION__ROTATING_PALETTE_NEW__ID:
          SubTask_Segment_Animation__Rotating_Palette_New();
        break;
        case EFFECTS_FUNCTION__ROTATING_PALETTE__ID:
          SubTask_Segment_Animation__Rotating_Palette(); // phase out
        break;
        case EFFECTS_FUNCTION__ROTATING_PREVIOUS_ANIMATION__ID:
          SubTask_Segment_Animation__Rotating_Previous_Animation(); // phase out
        break;
        case EFFECTS_FUNCTION__ROTATING_PREVIOUS_ANIMATION_BLENDED__ID:
          SubTask_Segment_Animation__Rotating_Previous_Animation(); // phase out
        break;
        case EFFECTS_FUNCTION__STEPPING_PALETTE__ID:
          SubTask_Segment_Animation__Stepping_Palette();
        break;
        #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
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
        #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
        case EFFECTS_FUNCTION__WLED_SPOTS__ID:
          SubTask_Segment_Animation__Spots();
        break;
        #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
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
        #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
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
        #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
        /**
         * Sparkle/Twinkle
         **/
        #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
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
        #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE      
        #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
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
        #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
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
       * @brief Only calls Animator if effects are not directly handled
       **/
      if(SEGMENT.anim_function_callback == nullptr) // assumes direct update
      {
        pCONT_iLight->ShowInterface();    // I cant do doShow here, since direct method (no callback) needs to happen now, whereas AnimUpdate will need to call show later. So call them when needed (though, still worth working out a better/unified show/solution)     
        seg.flags.animator_first_run = false; // CHANGE to function: reset here for all WLED methods
        ALOG_DBM(PSTR("Calling"));
      }
      else
      { 
        StartSegmentAnimation_AsAnimUpdateMemberFunction(_segment_index_primary);   
        // First run must be reset after StartAnimation is first called 
      }
              
      SEGMENT.call++; // Used as progress counter for animations eg rainbow across all hues

      DEBUG_PIN1_SET(HIGH);
                
    }//end if update reached

    /**
     * @brief Now iter forward on active segment
     * 
     */
    _segment_index_primary++;
  
  } // END for

} // SubTask_Effects_PhaseOut



/**
 * @brief New allocated buffers must contain colour info
 * 
 * @param param 
 */
void mAnimatorLight::AnimationProcess_LinearBlend_Dynamic_Buffer(const AnimationParam& param)
{    

  RgbcctColor updatedColor = RgbcctColor(0);
  TransitionColourPairs colour_pairs;

  for (uint16_t pixel = 0; 
                pixel < SEGMENT.virtualLength();
                pixel++
  ){  
    GetTransitionColourBuffer(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type, &colour_pairs);

    updatedColor = RgbTypeColor::LinearBlend(colour_pairs.StartingColour, colour_pairs.DesiredColour, param.progress);  


    //AddLog(LOG_LEVEL_TEST, PSTR("SI%d,seg_len%d, RGB[%d] %d,%d,%d"),_segment_index_primary,SEGMENT.length_m(), pixel, updatedColor.R, updatedColor.G, updatedColor.B);

//wrong in here!
    SEGMENT.SetPixelColor(pixel, updatedColor);

  }

}



/**
 * @brief New allocated buffers must contain colour info
 * @brief Replicate one colour across whole string (ie scences for h801 or addressable)
 * 
 * @param param 
 */
void mAnimatorLight::AnimationProcess_SingleColour_LinearBlend_Dynamic_Buffer(const AnimationParam& param)
{    

  RgbcctColor updatedColor = RgbcctColor(0);
  TransitionColourPairs colour_pairs;
  GetTransitionColourBuffer(SEGMENT.Data(), SEGMENT.DataLength(), 0, SEGMENT.colour_type, &colour_pairs);
  
  updatedColor = RgbcctColor::LinearBlend(colour_pairs.StartingColour, colour_pairs.DesiredColour, param.progress);    
  
  ALOG_DBM( PSTR("SEGMENT.colour_type=%d, desired_colour1=%d,%d,%d,%d,%d"),SEGMENT.colour_type,updatedColor.R,updatedColor.G,updatedColor.B,updatedColor.WC,updatedColor.WW);

  // AddLog(LOG_LEVEL_TEST, PSTR("Acolour_pairs.StartingColour=%d,%d,%d,%d,%d"),colour_pairs.StartingColour.R,colour_pairs.StartingColour.G,colour_pairs.StartingColour.B,colour_pairs.StartingColour.WC,colour_pairs.StartingColour.WW);
  // AddLog(LOG_LEVEL_TEST, PSTR("Acolour_pairs.DesiredColour=%d,%d,%d,%d,%d"),colour_pairs.DesiredColour.R,colour_pairs.DesiredColour.G,colour_pairs.DesiredColour.B,colour_pairs.DesiredColour.WC,colour_pairs.DesiredColour.WW);
    
  for (uint16_t pixel = 0; 
                pixel < SEGLEN; 
                pixel++
  ){  
    SEGMENT.SetPixelColor(pixel, updatedColor, false);
  }
  
}




RgbcctColor mAnimatorLight::GetSegmentColour(uint8_t colour_index, uint8_t segment_index)
{
  return SEGMENT_I(segment_index).rgbcctcolors[colour_index];
}




void mAnimatorLight::Set_Segment_ColourType(uint8_t segment_index, uint8_t light_type)
{

  switch(light_type)
  {
    case LT_PWM5:
      SEGMENT_I(segment_index).colour_type = RgbcctColor::LightSubType::LIGHT_TYPE__RGBCCT__ID; 
    break;
    case LT_ADDRESSABLE_SK6812:
      SEGMENT_I(segment_index).colour_type = RgbcctColor::LightSubType::LIGHT_TYPE__RGBW__ID; 
    break;
    default:
    case LT_ADDRESSABLE_WS281X:
      SEGMENT_I(segment_index).colour_type = RgbcctColor::LightSubType::LIGHT_TYPE__RGB__ID;
    break;
  }

}



uint8_t mAnimatorLight::GetSizeOfPixel(RgbcctColor::LightSubType colour_type)
{
  switch(colour_type)
  {
    case RgbcctColor::LightSubType::LIGHT_TYPE__RGB__ID:     return 3;
    case RgbcctColor::LightSubType::LIGHT_TYPE__RGBW__ID:    
    // case RgbcctColor::LightSubType::LIGHT_TYPE_RGBC__ID:      
    return 4;
    case RgbcctColor::LightSubType::LIGHT_TYPE__RGBCCT__ID:  return 5;
  }

}

bool mAnimatorLight::SetTransitionColourBuffer_StartingColour(
  byte* buffer, 
  uint16_t buflen, 
  uint16_t pixel_index, 
  RgbcctColor::LightSubType pixel_type, 
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
    case RgbcctColor::LightSubType::LIGHT_TYPE__RGB__ID:
    
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
    case RgbcctColor::LightSubType::LIGHT_TYPE__RGBW__ID:
    
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
    case RgbcctColor::LightSubType::LIGHT_TYPE__RGBCCT__ID:
    
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



/*
 * Fills segment with color
 */
void mAnimatorLight::fill(uint32_t c, bool apply_brightness) 
{
  for(uint16_t i = 0; i < _virtualSegmentLength; i++) 
  {
    SEGMENT.SetPixelColor(i, c, apply_brightness);
  }
}



void mAnimatorLight::fill_ranged(uint32_t c, bool apply_brightness) 
{

  for(uint16_t i = SEGMENT.pixel_range.start; i <= SEGMENT.pixel_range.stop; i++) {
    SEGMENT.SetPixelColor(i, c, apply_brightness);
  }

}

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Fireworks
 * @note : Converted from WLED Effects
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/


uint32_t mAnimatorLight::crgb_to_col(CRGB fastled)
{
  uint32_t colour = 0; // suppress White colour (MSB)
  colour = (((uint32_t)fastled.red << 16) | ((uint32_t)fastled.green << 8) | fastled.blue);
  return colour;
}


CRGB mAnimatorLight::col_to_crgb(uint32_t color)
{
  CRGB fastled_col;
  fastled_col.red =   (color >> 16 & 0xFF);
  fastled_col.green = (color >> 8  & 0xFF);
  fastled_col.blue =  (color       & 0xFF);
  return fastled_col;
}

/*
 * blurs segment content, source: FastLED colorutils.cpp
 */
void mAnimatorLight::blur(uint8_t blur_amount, bool set_brightness)
{
  uint8_t keep = 255 - blur_amount;
  uint8_t seep = blur_amount >> 1;
  CRGB carryover = HTMLColorCode::Black;

  for(uint16_t i = 0; i < _virtualSegmentLength; i++)
  {

    CRGB cur = col_to_crgb( RgbcctColor::GetU32Colour(SEGMENT.GetPixelColor(i)));
    CRGB part = cur;
    part.nscale8(seep);
    cur.nscale8(keep);
    cur += carryover;
    if(i > 0) {
      uint32_t c = RgbcctColor::GetU32Colour(SEGMENT.GetPixelColor(i-1));
      uint8_t r = (c >> 16 & 0xFF);
      uint8_t g = (c >> 8  & 0xFF);
      uint8_t b = (c       & 0xFF);
      SEGMENT.SetPixelColor(i-1, RgbColor(qadd8(r, part.red), qadd8(g, part.green), qadd8(b, part.blue)), set_brightness);
    }
    SEGMENT.SetPixelColor(i, RgbColor(cur.red, cur.green, cur.blue), set_brightness);
    carryover = part;
  }

  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
  
}


// void mAnimatorLight::blur(uint8_t blur_amount)
// {
//   uint8_t keep = 255 - blur_amount;
//   uint8_t seep = blur_amount >> 1;
  
//   // CRGB carryover = HTMLColorCode::Black;
//   RgbcctColor carryover = RgbcctColor(0);

//   for(uint16_t i = 0; i < _virtualSegmentLength; i++)
//   {

//     // CRGB cur = col_to_crgb( RgbcctColor::GetU32Colour(test));//SEGMENT.GetPixelColor(i)));
//     RgbcctColor cur = SEGMENT.GetPixelColor(i);
//     RgbcctColor part = cur;
//     part.nscale8(seep);
//     cur.nscale8(keep);
//     cur += carryover;

//     if(i > 0) {
//       RgbcctColor c = SEGMENT.GetPixelColor(i-1);
//       SEGMENT.SetPixelColor(i-1, RgbColor(qadd8(c.R, part.red), qadd8(c.G, part.green), qadd8(c.B, part.blue)));
//     }
//     SEGMENT.SetPixelColor(i, RgbColor(cur.red, cur.green, cur.blue));
//     carryover = part;
//   }
// }

/*
 * fade out function, higher rate = quicker fade
 Adding this helper function temporarily until I convert it to linearblend methods
 */
void mAnimatorLight::fade_out(uint8_t rate, bool set_brightness) 
{
  rate = (255-rate) >> 1;
  float mappedRate = float(rate) +1.1;

  uint32_t color = RgbcctColor::GetU32Colour(SEGMENT.rgbcctcolors[1]); // target color

  int w2 = (color >> 24) & 0xff;
  int r2 = (color >> 16) & 0xff;
  int g2 = (color >>  8) & 0xff;
  int b2 =  color        & 0xff;

  // r2 = 255;

  for(uint16_t i = 0; i < _virtualSegmentLength; i++) {
    color = RgbcctColor::GetU32Colour(SEGMENT.GetPixelColor(i));
    int w1 = (color >> 24) & 0xff;
    int r1 = (color >> 16) & 0xff;
    int g1 = (color >>  8) & 0xff;
    int b1 =  color        & 0xff;

    int wdelta = (w2 - w1) / mappedRate;  //linearblend
    int rdelta = (r2 - r1) / mappedRate;
    int gdelta = (g2 - g1) / mappedRate;
    int bdelta = (b2 - b1) / mappedRate;

    // if fade isn't complete, make sure delta is at least 1 (fixes rounding issues)
    wdelta += (w2 == w1) ? 0 : (w2 > w1) ? 1 : -1;
    rdelta += (r2 == r1) ? 0 : (r2 > r1) ? 1 : -1;
    gdelta += (g2 == g1) ? 0 : (g2 > g1) ? 1 : -1;
    bdelta += (b2 == b1) ? 0 : (b2 > b1) ? 1 : -1;

    SEGMENT.SetPixelColor(i, RgbwColor(r1 + rdelta, g1 + gdelta, b1 + bdelta, w1 + wdelta), set_brightness);
  }

  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}


bool mAnimatorLight::SetTransitionColourBuffer_DesiredColour(byte* buffer, uint16_t buflen, uint16_t pixel_index, 
RgbcctColor::LightSubType pixel_type, RgbcctColor desired_colour)
{   
  
  // #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    //ALOG_INF( PSTR("EFFECT sIndexIO=I%d start%d,stop%d\tp%d,PL %d, RGB:%d,%d,%d"), SEGIDX, SEGMENT.pixel_range.start, SEGMENT.pixel_range.stop, pixel, mPaletteI->GetNumberOfColoursInPalette(SEGMENT.palette.id), colour.R, colour.G, colour.B );
    // #endif

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
    case RgbcctColor::LightSubType::LIGHT_TYPE__RGB__ID:
    
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
    case RgbcctColor::LightSubType::LIGHT_TYPE__RGBW__ID:
    
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
    case RgbcctColor::LightSubType::LIGHT_TYPE__RGBCCT__ID:
    
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

bool mAnimatorLight::SetTransitionColourBuffer(byte* buffer, uint16_t buflen, uint16_t pixel_index, RgbcctColor::LightSubType pixel_type, 
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
  RgbcctColor::LightSubType pixel_type, 
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
    case RgbcctColor::LightSubType::LIGHT_TYPE__RGB__ID:
      pixel_start_i = pixel_index*6;
      c = &buffer[pixel_start_i];
      colour->StartingColour = RgbColor(c[0], c[1], c[2]);
      colour->DesiredColour  = RgbColor(c[3], c[4], c[5]);
      break;
    case RgbcctColor::LightSubType::LIGHT_TYPE__RGBW__ID:
      pixel_start_i = pixel_index*8;
      c = &buffer[pixel_start_i];
      colour->StartingColour = RgbwColor(c[0], c[1], c[2], c[3]);
      colour->DesiredColour  = RgbwColor(c[4], c[5], c[6], c[7]);
      break;
    case RgbcctColor::LightSubType::LIGHT_TYPE__RGBCCT__ID:
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

  // Move below into function
  /**
   * @brief All addressible style things, if it really is other types, call them from interface
   * 
   */
  if(
    (pCONT_set->Settings.light_settings.type < LT_LIGHT_INTERFACE_END)||
     (pCONT_set->Settings.light_settings.type == LT_ADDRESSABLE_WS281X)||
     (pCONT_set->Settings.light_settings.type == LT_ADDRESSABLE_SK6812)
  ){

    #ifdef USE_MODULE_LIGHTS_ANIMATOR
    
    switch(pCONT_lAni->SEGMENT_I(0).animation_mode_id)    // needs to know the id 
    {
      #ifdef ENABLE_ANIMATION_MODE__EFFECTS
      case ANIMATION_MODE__EFFECTS:
        SubTask_Segments_Animation();
      break;
      #endif
      #ifdef ENABLE_ANIMATION_MQTT_SETPIXEL
      case ANIMATION_MODE__MQTT_SETPIXEL:
        SubTask_Segments_Animation();
      break;
      #endif
      #ifdef ENABLE_ANIMATION_REALTIME_UDP
      case ANIMATION_MODE__REALTIME_UDP:
        SubTask_Segments_Animation();
      break;
      #endif
      #ifdef ENABLE_ANIMATION_MODE__REALTIME_HYPERION
      case ANIMATION_MODE__REALTIME_HYPERION:
        SubTask_Segments_Animation();
      break;
      #endif
      #ifdef ENABLE_ANIMATION_MODE__REALTIME_E131
      case ANIMATION_MODE__REALTIME_E131:
        SubTask_Segments_Animation();
      break;
      #endif
      #ifdef ENABLE_ANIMATION_MODE__REALTIME_ADALIGHT
      case ANIMATION_MODE__REALTIME_ADALIGHT:
        SubTask_Segments_Animation();
      break;
      #endif
      #ifdef ENABLE_ANIMATION_MODE__REALTIME_ARTNET
      case ANIMATION_MODE__REALTIME_ARTNET:
        SubTask_Segments_Animation();
      break;
      #endif
      #ifdef ENABLE_ANIMATION_MODE__REALTIME_TPM2NET
      case ANIMATION_MODE__REALTIME_TPM2NET:
        SubTask_Segments_Animation();
      break;
      #endif
      #ifdef ENABLE_ANIMATION_MODE__REALTIME_DDP
      case ANIMATION_MODE__REALTIME_DDP:
        SubTask_Segments_Animation();
      break;
      #endif
      case ANIMATION_MODE__DISABLED: default: return; // Leave function
    }
    #endif
      
  }
  else
  {
    #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
    ALOG_DBM( PSTR("Invalid Light Type"));
    #endif
  }


  uint8_t flag_animations_needing_updated = 0;
    
  // for(
  //   int ii = 0; 
  //   ii<MAX_NUM_SEGMENTS; 
  //   ii++
  // ){
  for (uint16_t seg_i = 0; seg_i < segments.size(); seg_i++)
  {

    _segment_index_primary = seg_i; // Important! Otherwise _segment_index_primary can be incremented out of scope when a later callback is called

    if(SEGMENT_I(seg_i).isActive())
    {
      if (SEGMENT_I(seg_i).animator->IsAnimating())
      {
        /**
         * @brief A Backoff time is needed per animation so the DMA is not overloaded
        **/
        if(mTime::TimeReached(&SEGMENT_I(seg_i).tSaved_AnimateRunTime, 10))
        {
          SEGMENT_I(seg_i).animator->UpdateAnimations(seg_i);
          flag_animations_needing_updated++; // channels needing updated
        } //end TimeReached
  
        SEGMENT.flags.animator_first_run = false;     // CHANGE to function: reset here for all my methods

        /**
         * @brief If it completes, reset
         *
         * Need proper checks here to only reset if it was running before
         */
        if(!SEGMENT_I(seg_i).animator->IsAnimationActive(0))
        {
          SEGMENT_I(seg_i).transitional = false; // Since this is already inside "IsAnimating" then it should only be set if it was animating but is now stopping
        }

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
        pCONT_iLight->ShowInterface();
      }
    }
    
//     #ifndef ENABLE_DEVFEATURE_FORCED_REMOVE_091122
//     /**
//      * THESE ALL NEED PUT INTO SEGMENTS
//      * THESE ARE COMMANDS IF ANY IS ANIMATING
//      * checking if animation state has changed from before (maybe check via animation state?)
//      * */
//     if(!SEGMENT_I(0).flags.fRunning)
//     {   
//       #ifdef ENABLE_LOG_LEVEL_DEBUG
//       ALOG_DBM( PSTR(D_LOG_NEO "Animation Started"));
//       #endif
//     }
//     SEGMENT_I(0).flags.fRunning = true; 
//     fPixelsUpdated = true;
//     pCONT_set->Settings.enable_sleep = false;    //Make this a function, pause sleep during animations
//     //AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "~a"));
//   }
//   else // none need animating
//   {
    
//       #ifdef ENABLE_DEVFEATURE_DEBUG_FREEZING_SK6812
//       // Serial.print("E");
//       #endif // ENABLE_DEVFEATURE_DEBUG_FREEZING_SK6812


//     if(SEGMENT_I(0).flags.fRunning)
//     { // Was running
//       #ifdef ENABLE_LOG_LEVEL_DEBUG
//       ALOG_DBM( PSTR(D_LOG_NEO "Animation Finished")); 
//       #endif


//       fAnyLEDsOnOffCount = 0;

// /*****
//  *  DISABLING THE ABILITY TO KNOW WHEN IT IS ON OR OFF, NEEDS ADDED BACK IN AGAIN WITH CHECKING ALL SEGMENTS        
//  ***
  
//   */
//       for(int i=0;i<pCONT_iLight->settings.light_size_count;i++){ 
//         if(SEGMENT.GetPixelColor(i)!=0){ fAnyLEDsOnOffCount++; }
//       }          

//     }
//     SEGMENT_I(0).flags.fRunning = false;
//     pCONT_set->Settings.enable_sleep = true;
//     if(blocking_force_animate_to_complete){ //if it was running
//       #ifdef ENABLE_LOG_LEVEL_DEBUG
//       ALOG_DBM( PSTR(D_LOG_NEO "Animation blocking_force_animate_to_complete"));
//       #endif
//       blocking_force_animate_to_complete = false;
//     }
// #endif // ENABLE_DEVFEATURE_FORCED_REMOVE_091122
  } // flag_animations_needing_updated
  else
  {
    /**
     * @brief If no animations are running, with backoff, lets check the current output (though, this may be bad since I want to limit BEFORE hitting max)
     **/
    // stripbus->SetPixelSettings(NeoRgbCurrentSettings(1,2,3))
    // ALOG_INF(PSTR("power %d",stripbus->CalcTotalMilliAmpere());


    // float power_level = 0;
    // for(uint16_t i=0;i<STRIP_SIZE_MAX;i++){
    //   stripbus->SEGMENT.GetPixelColor
    // }

  }


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
  //     if(SEGMENT.GetPixelColor(i)!=0){
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
  this->SEGMENT_I(segment_index).anim_function_callback = anim_function_callback;
  SEGMENT_I(segment_index).animation_has_anim_callback = true;
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
// 
  for(int pixel=0;
          // #ifdef ENABLE_DEVFEATURE_GROUPING_FIX_DEC2022
          pixel<SEGMENT.virtualLength();
          // #else
          // pixel<SEGMENT.length();
          // #endif
          pixel++
  ){

    SetTransitionColourBuffer_StartingColour( SEGMENT.Data(), 
                                              SEGMENT.DataLength(),
                                              pixel, 
                                              SEGMENT.colour_type, 
                                              RgbcctColor(SEGMENT.GetPixelColor(pixel))
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
  time_tmp = SEGMENT_I(segment_index).transition.time_ms;//    SEGMENT_I(segment_index).transition.time_ms; 

  /**
   * @brief Before starting animation, check the override states to see if they are active
   * 
   */
  // Overwriting single SEGMENT_I(0) methods, set, then clear
  if(SEGMENT_I(segment_index).single_animation_override.time_ms>0)
  {
    time_tmp = SEGMENT_I(segment_index).single_animation_override.time_ms;
    SEGMENT_I(segment_index).single_animation_override.time_ms = 0; // reset overwrite
    ALOG_INF(PSTR("Override: TimeMs: %d"), time_tmp);
  }

  // Serial.printf("TRANSITION_METHOD_INSTANT_ID = %d\n\r",time_tmp);

  if(time_tmp>0){
    if(NEO_ANIMATION_TIMEBASE == NEO_CENTISECONDS){
      time_tmp /= 1000;// ms to seconds
    }
  }

  if(SEGMENT_I(segment_index).animation_has_anim_callback == true)
  {
    SEGMENT_I(segment_index).animator->StartAnimation(0, time_tmp, SEGMENT_I(segment_index).anim_function_callback);
  }

  SEGMENT_I(segment_index).transitional = true;


} //end function





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
  

  // SEGMENT_I(segment_index).reset(); // Do not reset, as this makes consecutive commands lose the animation already running. 
  // It should only reset if the animation has changed?
  if(SEGMENT_I(segment_index).effect_id != value)
  {
    SEGMENT_I(segment_index).markForReset();
    SEGMENT_I(segment_index).effect_id = value;      //make function "changeFlasherFunction" so then the region is automatically updated internally
  }
  SEGMENT_I(segment_index).aux0 = EFFECTS_REGION_COLOUR_SELECT_ID;
  SEGMENT_I(segment_index).flags.animator_first_run= true; // first run, so do extra things

  

  if(segments[segment_index].effect_id != value)
  {
    segments[segment_index].markForReset();
    segments[segment_index].effect_id = value;      //make function "changeFlasherFunction" so then the region is automatically updated internally
  }
  segments[segment_index].aux0 = EFFECTS_REGION_COLOUR_SELECT_ID;
  segments[segment_index].flags.animator_first_run= true; // first run, so do extra things

  ALOG_INF(PSTR("segments[segment_index].effect_id=%d"),segments[segment_index].effect_id);

  
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
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION__SHIMMERING_PALETTE__NAME_CTR)){ return EFFECTS_FUNCTION__WLED_SHIMMERING_PALETTE__ID; }
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

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
  return GetFlasherFunctionNamebyID(SEGMENT_I(segment_index).effect_id, buffer, buflen);
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

    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
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


void mAnimatorLight::CommandSet_Effect_Intensity(uint8_t value, uint8_t segment_index)
{

  SEGMENT_I(segment_index).intensity_value = value;

}


void mAnimatorLight::CommandSet_Effect_Speed(uint8_t value, uint8_t segment_index)
{

  SEGMENT_I(segment_index).speed_value = value;

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






///////////////////////////////////////////////////////////////////////////////
// Segment_New class implementation
///////////////////////////////////////////////////////////////////////////////
uint16_t mAnimatorLight::Segment_New::_usedSegmentData = 0U; // amount of RAM all segments use for their data[]
CRGB    *mAnimatorLight::Segment_New::_globalLeds = nullptr;
uint16_t mAnimatorLight::Segment_New::maxWidth = DEFAULT_LED_COUNT;
uint16_t mAnimatorLight::Segment_New::maxHeight = 1;

// copy constructor
// mAnimatorLight::Segment_New(const mAnimatorLight::Segment_New &orig) {
//   //DEBUG_PRINTLN(F("-- Copy segment constructor --"));
//   memcpy(this, &orig, sizeof(mAnimatorLight::Segment_New));
//   name = nullptr;
//   data = nullptr;
//   _dataLen = 0;
//   _t = nullptr;
//   if (leds && !mAnimatorLight::Segment_New::_globalLeds) leds = nullptr;
//   if (orig.name) { name = new char[strlen(orig.name)+1]; if (name) strcpy(name, orig.name); }
//   if (orig.data) { if (allocateData(orig._dataLen)) memcpy(data, orig.data, orig._dataLen); }
//   if (orig._t)   { _t = new Transition(orig._t->_dur, orig._t->_briT, orig._t->_cctT, orig._t->_colorT); }
//   if (orig.leds && !mAnimatorLight::Segment_New::_globalLeds) { leds = (CRGB*)malloc(sizeof(CRGB)*length()); if (leds) memcpy(leds, orig.leds, sizeof(CRGB)*length()); }
// }

// // move constructor
// mAnimatorLight::Segment_New(mAnimatorLight::Segment_New &&orig) noexcept {
//   //DEBUG_PRINTLN(F("-- Move segment constructor --"));
//   memcpy(this, &orig, sizeof(mAnimatorLight::Segment_New));
//   orig.name = nullptr;
//   orig.data = nullptr;
//   orig._dataLen = 0;
//   orig._t   = nullptr;
//   orig.leds = nullptr;
// }

// copy assignment
mAnimatorLight::Segment_New& mAnimatorLight::Segment_New::operator= (const mAnimatorLight::Segment_New &orig) {
  //DEBUG_PRINTLN(F("-- Copying segment --"));
  if (this != &orig) {
    // clean destination
    if (name) delete[] name;
    if (_t)   delete _t;
    if (leds && !mAnimatorLight::Segment_New::_globalLeds) free(leds);
    deallocateData();
    // copy source
    memcpy(this, &orig, sizeof(mAnimatorLight::Segment_New));
    // erase pointers to allocated data
    name = nullptr;
    data = nullptr;
    _dataLen = 0;
    _t = nullptr;
    if (!mAnimatorLight::Segment_New::_globalLeds) leds = nullptr;
    // copy source data
    if (orig.name) { name = new char[strlen(orig.name)+1]; if (name) strcpy(name, orig.name); }
    if (orig.data) { if (allocateData(orig._dataLen)) memcpy(data, orig.data, orig._dataLen); }
    if (orig._t)   { _t = new Transition(orig._t->_dur, orig._t->_briT, orig._t->_cctT, orig._t->_colorT); }
    if (orig.leds && !mAnimatorLight::Segment_New::_globalLeds) { leds = (CRGB*)malloc(sizeof(CRGB)*length()); if (leds) memcpy(leds, orig.leds, sizeof(CRGB)*length()); }
  }
  return *this;
}

// move assignment
mAnimatorLight::Segment_New& mAnimatorLight::Segment_New::operator= (mAnimatorLight::Segment_New &&orig) noexcept {
  //DEBUG_PRINTLN(F("-- Moving segment --"));
  if (this != &orig) {
    if (name) delete[] name; // free old name
    deallocateData(); // free old runtime data
    if (_t) delete _t;
    if (leds && !mAnimatorLight::Segment_New::_globalLeds) free(leds);
    memcpy(this, &orig, sizeof(mAnimatorLight::Segment_New));
    orig.name = nullptr;
    orig.data = nullptr;
    orig._dataLen = 0;
    orig._t   = nullptr;
    orig.leds = nullptr;
  }
  return *this;
}

bool mAnimatorLight::Segment_New::allocateData(size_t len) {
  if (data && _dataLen == len) return true; //already allocated
  deallocateData();
  if (mAnimatorLight::Segment_New::getUsedSegmentData() + len > MAX_SEGMENT_DATA){ 
    ALOG_ERR( PM_JSON_MEMORY_INSUFFICIENT ); // This is the base case, none will be fallback
    effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID;
    return false; //not enough memory
  }
  // if possible use SPI RAM on ESP32
  #if defined(ARDUINO_ARCH_ESP32) && defined(WLED_USE_PSRAM)
  if (psramFound())
    data = (byte*) ps_malloc(len);
  else
  #endif
    data = (byte*) malloc(len);
  if (!data){
    ALOG_ERR( PM_JSON_MEMORY_INSUFFICIENT ); // This is the base case, none will be fallback
    effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID;
    return false; //allocation failed
  }
  // Serial.printf("NEW_MEMORY of %d for segment index %d\n\r\n\r\n\r\n\r", len, palette.id ); delay(2000);
  addUsedSegmentData(len);
  _dataLen = len;
  memset(data, 0, len);
  return true;
}

void mAnimatorLight::Segment_New::deallocateData() {
  if (!data) return;
  free(data);
  data = nullptr;
  addUsedSegmentData(-_dataLen);
  _dataLen = 0;
}

/** 
  * If reset of this segment was requested, clears runtime
  * settings of this segment.
  * Must not be called while an effect mode function is running
  * because it could access the data buffer and this method 
  * may free that data buffer.
  */
void mAnimatorLight::Segment_New::resetIfRequired() {
  if (reset) {
    if (leds && !mAnimatorLight::Segment_New::_globalLeds) { free(leds); leds = nullptr; }
    //if (_t) { delete _t; _t = nullptr; transitional = false; }
    next_time = 0; step = 0; call = 0; aux0 = 0; aux1 = 0; 
    reset = false; // setOption(SEG_OPTION_RESET, false);
  }
}

void mAnimatorLight::Segment_New::setUpLeds() {
  // deallocation happens in resetIfRequired() as it is called when segment changes or in destructor
  if (mAnimatorLight::Segment_New::_globalLeds)
    #ifndef WLED_DISABLE_2D
    leds = &mAnimatorLight::Segment_New::_globalLeds[pixel_range.start + startY*mAnimatorLight::Segment_New::maxWidth];
    #else
    leds = &mAnimatorLight::Segment_New::_globalLeds[pixel_range.start];
    #endif
  else if (!leds) {
    #if defined(ARDUINO_ARCH_ESP32) && defined(WLED_USE_PSRAM)
    if (psramFound())
      leds = (CRGB*)ps_malloc(sizeof(CRGB)*length());
    else
    #endif
      leds = (CRGB*)malloc(sizeof(CRGB)*length());
  }
}

CRGBPalette16 &mAnimatorLight::Segment_New::loadPalette(CRGBPalette16 &targetPalette, uint8_t pal) {
  static unsigned long _lastPaletteChange = 0; // perhaps it should be per segment
  static CRGBPalette16 randomPalette = CRGBPalette16(DEFAULT_COLOR);
  // byte tcp[72];
  // if (pal < 245 && pal > GRADIENT_PALETTE_COUNT+13) pal = 0;
  // if (pal > 245 && (strip.customPalettes.size() == 0 || 255U-pal > strip.customPalettes.size()-1)) pal = 0;
  // //default palette. Differs depending on effect
  // if (pal == 0) switch (mode) {
  //   case FX_MODE_FIRE_2012  : pal = 35; break; // heat palette
  //   case FX_MODE_COLORWAVES : pal = 26; break; // landscape 33
  //   case FX_MODE_FILLNOISE8 : pal =  9; break; // ocean colors
  //   case FX_MODE_NOISE16_1  : pal = 20; break; // Drywet
  //   case FX_MODE_NOISE16_2  : pal = 43; break; // Blue cyan yellow
  //   case FX_MODE_NOISE16_3  : pal = 35; break; // heat palette
  //   case FX_MODE_NOISE16_4  : pal = 26; break; // landscape 33
  //   case FX_MODE_GLITTER    : pal = 11; break; // rainbow colors
  //   case FX_MODE_SUNRISE    : pal = 35; break; // heat palette
  //   case FX_MODE_FLOW       : pal =  6; break; // party
  // }
  // switch (pal) {
  //   case 0: //default palette. Exceptions for specific effects above
  //     targetPalette = PartyColors_p; break;
  //   case 1: //periodically replace palette with a random one. Doesn't work with multiple FastLED segments
  //     if (millis() - _lastPaletteChange > 5000 /*+ ((uint32_t)(255-intensity))*100*/) {
  //       randomPalette = CRGBPalette16(
  //                       CHSV(random8(), random8(160, 255), random8(128, 255)),
  //                       CHSV(random8(), random8(160, 255), random8(128, 255)),
  //                       CHSV(random8(), random8(160, 255), random8(128, 255)),
  //                       CHSV(random8(), random8(160, 255), random8(128, 255)));
  //       _lastPaletteChange = millis();
  //     }
  //     targetPalette = randomPalette; break;
  //   case 2: {//primary color only
  //     CRGB prim = gamma32(colors[0]);
  //     targetPalette = CRGBPalette16(prim); break;}
  //   case 3: {//primary + secondary
  //     CRGB prim = gamma32(colors[0]);
  //     CRGB sec  = gamma32(colors[1]);
  //     targetPalette = CRGBPalette16(prim,prim,sec,sec); break;}
  //   case 4: {//primary + secondary + tertiary
  //     CRGB prim = gamma32(colors[0]);
  //     CRGB sec  = gamma32(colors[1]);
  //     CRGB ter  = gamma32(colors[2]);
  //     targetPalette = CRGBPalette16(ter,sec,prim); break;}
  //   case 5: {//primary + secondary (+tert if not off), more distinct
  //     CRGB prim = gamma32(colors[0]);
  //     CRGB sec  = gamma32(colors[1]);
  //     if (colors[2]) {
  //       CRGB ter = gamma32(colors[2]);
  //       targetPalette = CRGBPalette16(prim,prim,prim,prim,prim,sec,sec,sec,sec,sec,ter,ter,ter,ter,ter,prim);
  //     } else {
  //       targetPalette = CRGBPalette16(prim,prim,prim,prim,prim,prim,prim,prim,sec,sec,sec,sec,sec,sec,sec,sec);
  //     }
  //     break;}
  //   case 6: //Party colors
      targetPalette = PartyColors_p;// break;
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
  //     if (pal>245) {
  //       targetPalette = strip.customPalettes[255-pal]; // we checked bounds above
  //     } else {
  //       memcpy_P(tcp, (byte*)pgm_read_dword(&(gGradientPalettes[pal-13])), 72);
  //       targetPalette.loadDynamicGradientPalette(tcp);
  //     }
  //     break;
  // }
  return targetPalette;
}

void mAnimatorLight::Segment_New::startTransition(uint16_t dur) {
  if (transitional || _t) return; // already in transition no need to store anything

  // starting a transition has to occur before change so we get current values 1st
  uint8_t _briT = currentBri(on ? opacity : 0);
  uint8_t _cctT = currentBri(cct, true);
  CRGBPalette16 _palT; loadPalette(_palT, palette_wled);
  uint8_t _modeP = effect_id;
  uint32_t _colorT[NUM_COLORS];
  
  for (size_t i=0; i<NUM_COLORS; i++) _colorT[i] = currentColor(i, RgbcctColor::GetU32Colour(rgbcctcolors[i]));

  if (!_t) _t = new Transition(dur); // no previous transition running
  if (!_t) return; // failed to allocate data
  _t->_briT  = _briT;
  _t->_cctT  = _cctT;
  _t->_palT  = _palT;
  _t->_modeP = _modeP;
  for (size_t i=0; i<NUM_COLORS; i++) _t->_colorT[i] = _colorT[i];
  transitional = true; // setOption(SEG_OPTION_TRANSITIONAL, true);
}

// transition progression between 0-65535
uint16_t mAnimatorLight::Segment_New::progress() {
  if (!transitional || !_t) return 0xFFFFU;
  uint32_t timeNow = millis();
  if (timeNow - _t->_start > _t->_dur || _t->_dur == 0) return 0xFFFFU;
  return (timeNow - _t->_start) * 0xFFFFU / _t->_dur;
}

uint8_t mAnimatorLight::Segment_New::currentBri(uint8_t briNew, bool useCct) {
  if (transitional && _t) {
    uint32_t prog = progress() + 1;
    if (useCct) return ((briNew * prog) + _t->_cctT * (0x10000 - prog)) >> 16;
    else        return ((briNew * prog) + _t->_briT * (0x10000 - prog)) >> 16;
  } else {
    return briNew;
  }
}

uint8_t mAnimatorLight::Segment_New::currentMode(uint8_t newMode) {
  return (progress()>32767U) ? newMode : _t->_modeP; // change effect in the middle of transition
}

uint32_t mAnimatorLight::Segment_New::currentColor(uint8_t slot, uint32_t colorNew) {
 // return transitional && _t ? ColourBlendU32(_t->_colorT[slot], colorNew, progress(), true) : colorNew;
}

CRGBPalette16 &mAnimatorLight::Segment_New::currentPalette(CRGBPalette16 &targetPalette, uint8_t pal) {
  loadPalette(targetPalette, pal);
  if (transitional && _t && progress() < 0xFFFFU) {
    // blend palettes
    // there are about 255 blend passes of 48 "blends" to completely blend two palettes (in _dur time)
    // minimum blend time is 100ms maximum is 65535ms
    uint32_t timeMS = millis() - _t->_start;
    uint16_t noOfBlends = (255U * timeMS / _t->_dur) - _t->_prevPaletteBlends;
    for (int i=0; i<noOfBlends; i++, _t->_prevPaletteBlends++) nblendPaletteTowardPalette(_t->_palT, targetPalette, 48);
    targetPalette = _t->_palT; // copy transitioning/temporary palette
  }
  return targetPalette;
}

void mAnimatorLight::Segment_New::handleTransition() {
  // if (!transitional) return;
  // unsigned long maxWait = millis() + 20;
  // if (mode == EFFECTS_FUNCTION__LENGTH__ID && next_time > maxWait) next_time = maxWait;
  // if (progress() == 0xFFFFU) {
  //   if (_t) {
  //     if (_t->_modeP != mode) markForReset();
  //     delete _t;
  //     _t = nullptr;
  //   }
  //   transitional = false; // finish transitioning segment
  // }
}

bool mAnimatorLight::Segment_New::setColor(uint8_t slot, uint32_t c) { //returns true if changed
  // if (slot >= NUM_COLORS || c == colors[slot]) return false;
  // if (fadeTransition) startTransition(strip.getTransition()); // start transition prior to change
  // colors[slot] = c;
  // stateChanged = true; // send UDP/WS broadcast
  // return true;
}

void mAnimatorLight::Segment_New::setCCT(uint16_t k) {
  // if (k > 255) { //kelvin value, convert to 0-255
  //   if (k < 1900)  k = 1900;
  //   if (k > 10091) k = 10091;
  //   k = (k - 1900) >> 5;
  // }
  // if (cct == k) return;
  // if (fadeTransition) startTransition(strip.getTransition()); // start transition prior to change
  // cct = k;
  // stateChanged = true; // send UDP/WS broadcast
}

void mAnimatorLight::Segment_New::setOpacity(uint8_t o) {
  // if (opacity == o) return;
  // if (fadeTransition) startTransition(strip.getTransition()); // start transition prior to change
  // opacity = o;
  // stateChanged = true; // send UDP/WS broadcast
}

void mAnimatorLight::Segment_New::setOption(uint8_t n, bool val) {
  bool prevOn = on;
  // if (fadeTransition && n == SEG_OPTION_ON && val != prevOn) startTransition(strip.getTransition()); // start transition prior to change
  if (val) options |=   0x01 << n;
  else     options &= ~(0x01 << n);
  // if (!(n == SEG_OPTION_SELECTED || n == SEG_OPTION_RESET || n == SEG_OPTION_TRANSITIONAL)) stateChanged = true; // send UDP/WS broadcast
}

void mAnimatorLight::Segment_New::setMode(uint8_t fx, bool loadDefaults) {
  // // if we have a valid mode & is not reserved
  // if (fx < strip.getModeCount() && strncmp_P("RSVD", strip.getModeData(fx), 4)) {
  //   if (fx != mode) {
  //     startTransition(strip.getTransition()); // set effect transitions
  //     //markForReset(); // transition will handle this
  //     mode = fx;

  //     // load default values from effect string
  //     if (loadDefaults) {
  //       int16_t sOpt;
  //       sOpt = extractModeDefaults(fx, "sx");   if (sOpt >= 0) speed     = sOpt;
  //       sOpt = extractModeDefaults(fx, "ix");   if (sOpt >= 0) intensity = sOpt;
  //       sOpt = extractModeDefaults(fx, "c1");   if (sOpt >= 0) custom1   = sOpt;
  //       sOpt = extractModeDefaults(fx, "c2");   if (sOpt >= 0) custom2   = sOpt;
  //       sOpt = extractModeDefaults(fx, "c3");   if (sOpt >= 0) custom3   = sOpt;
  //       sOpt = extractModeDefaults(fx, "m12");  if (sOpt >= 0) map1D2D   = constrain(sOpt, 0, 7);
  //       sOpt = extractModeDefaults(fx, "si");   if (sOpt >= 0) soundSim  = constrain(sOpt, 0, 7);
  //       sOpt = extractModeDefaults(fx, "rev");  if (sOpt >= 0) reverse   = (bool)sOpt;
  //       sOpt = extractModeDefaults(fx, "mi");   if (sOpt >= 0) mirror    = (bool)sOpt; // NOTE: setting this option is a risky business
  //       sOpt = extractModeDefaults(fx, "rY");   if (sOpt >= 0) reverse_y = (bool)sOpt;
  //       sOpt = extractModeDefaults(fx, "mY");   if (sOpt >= 0) mirror_y  = (bool)sOpt; // NOTE: setting this option is a risky business
  //       sOpt = extractModeDefaults(fx, "pal");  if (sOpt >= 0) setPalette(sOpt);
  //     }
  //     stateChanged = true; // send UDP/WS broadcast
  //   }
  // }
}

void mAnimatorLight::Segment_New::setPalette(uint8_t pal) {
  // if (pal < 245 && pal > GRADIENT_PALETTE_COUNT+13) pal = 0; // built in palettes
  // if (pal > 245 && (strip.customPalettes.size() == 0 || 255U-pal > strip.customPalettes.size()-1)) pal = 0; // custom palettes
  // if (pal != palette) {
  //   if (strip.paletteFade) startTransition(strip.getTransition());
  //   palette = pal;
  //   stateChanged = true; // send UDP/WS broadcast
  // }
}

// 2D matrix
uint16_t mAnimatorLight::Segment_New::virtualWidth() const {
  uint16_t groupLen = groupLength();
  uint16_t vWidth = ((transpose ? height() : width()) + groupLen - 1) / groupLen;
  if (mirror) vWidth = (vWidth + 1) /2;  // divide by 2 if mirror, leave at least a single LED
  return vWidth;
}

uint16_t mAnimatorLight::Segment_New::virtualHeight() const {
  uint16_t groupLen = groupLength();
  uint16_t vHeight = ((transpose ? width() : height()) + groupLen - 1) / groupLen;
  if (mirror_y) vHeight = (vHeight + 1) /2;  // divide by 2 if mirror, leave at least a single LED
  return vHeight;
}

uint16_t mAnimatorLight::Segment_New::nrOfVStrips() const {
  uint16_t vLen = 1;
#ifndef WLED_DISABLE_2D
  if (is2D()) {
    switch (map1D2D) {
      case M12_pBar:
        vLen = virtualWidth();
        break;
    }
  }
#endif
  return vLen;
}

// 1D strip
uint16_t mAnimatorLight::Segment_New::virtualLength() const {
#ifndef WLED_DISABLE_2D
  if (is2D()) {
    uint16_t vW = virtualWidth();
    uint16_t vH = virtualHeight();
    uint16_t vLen = vW * vH; // use all pixels from segment
    switch (map1D2D) {
      case M12_pBar:
        vLen = vH;
        break;
      case M12_pCorner:
      case M12_pArc:
        vLen = max(vW,vH); // get the longest dimension
        break;
    }
    return vLen;
  }
#endif
  uint16_t groupLen = groupLength();
  uint16_t vLength = (length() + groupLen - 1) / groupLen;
  if (mirror) vLength = (vLength + 1) /2;  // divide by 2 if mirror, leave at least a single LED
  return vLength;
}

void IRAM_ATTR mAnimatorLight::Segment_New::setPixelColor(int i, uint32_t col)
{
//   int vStrip = i>>16; // hack to allow running on virtual strips (2D segment columns/rows)
//   i &= 0xFFFF;

//   if (i >= virtualLength() || i<0) return;  // if pixel would fall out of segment just exit

// #ifndef WLED_DISABLE_2D
//   if (is2D()) {
//     uint16_t vH = virtualHeight();  // segment height in logical pixels
//     uint16_t vW = virtualWidth();
//     switch (map1D2D) {
//       case M12_Pixels:
//         // use all available pixels as a long strip
//         setPixelColorXY(i % vW, i / vW, col);
//         break;
//       case M12_pBar:
//         // expand 1D effect vertically or have it play on virtual strips
//         if (vStrip>0) setPixelColorXY(vStrip - 1, vH - i - 1, col);
//         else          for (int x = 0; x < vW; x++) setPixelColorXY(x, vH - i - 1, col);
//         break;
//       case M12_pArc:
//         // expand in circular fashion from center
//         if (i==0)
//           setPixelColorXY(0, 0, col);
//         else {
//           float step = HALF_PI / (2.85f*i);
//           for (float rad = 0.0f; rad <= HALF_PI+step/2; rad += step) {
//             // may want to try float version as well (with or without antialiasing)
//             int x = roundf(sin_t(rad) * i);
//             int y = roundf(cos_t(rad) * i);
//             setPixelColorXY(x, y, col);
//           }
//         }
//         break;
//       case M12_pCorner:
//         for (int x = 0; x <= i; x++) setPixelColorXY(x, i, col);
//         for (int y = 0; y <  i; y++) setPixelColorXY(i, y, col);
//         break;
//     }
//     return;
//   } else if (mAnimatorLight::Segment_New::maxHeight!=1 && (width()==1 || height()==1)) {
//     // we have a vertical or horizontal 1D segment (WARNING: virtual...() may be transposed)
//     int x = 0, y = 0;
//     if (virtualHeight()>1) y = i;
//     if (virtualWidth() >1) x = i;
//     setPixelColorXY(x, y, col);
//     return;
//   }
// #endif

//   if (leds) leds[i] = col;

//   uint16_t len = length();
//   uint8_t _bri_t = currentBri(on ? opacity : 0);
//   if (_bri_t < 255) {
//     byte r = scale8(R(col), _bri_t);
//     byte g = scale8(G(col), _bri_t);
//     byte b = scale8(B(col), _bri_t);
//     byte w = scale8(W(col), _bri_t);
//     col = RGBW32(r, g, b, w);
//   }

//   // expand pixel (taking into account start, grouping, spacing [and offset])
//   i = i * groupLength();
//   if (reverse) { // is segment reversed?
//     if (mirror) { // is segment mirrored?
//       i = (len - 1) / 2 - i;  //only need to index half the pixels
//     } else {
//       i = (len - 1) - i;
//     }
//   }
//   i += start; // starting pixel in a group

//   // set all the pixels in the group
//   for (int j = 0; j < grouping; j++) {
//     uint16_t indexSet = i + ((reverse) ? -j : j);
//     if (indexSet >= start && indexSet < stop) {
//       if (mirror) { //set the corresponding mirrored pixel
//         uint16_t indexMir = stop - indexSet + start - 1;          
//         indexMir += offset; // offset/phase
//         if (indexMir >= stop) indexMir -= len; // wrap
//         strip.setPixelColor(indexMir, col);
//       }
//       indexSet += offset; // offset/phase
//       if (indexSet >= stop) indexSet -= len; // wrap
//       strip.setPixelColor(indexSet, col);
//     }
//   }
}

// anti-aliased normalized version of setPixelColor()
void mAnimatorLight::Segment_New::setPixelColor(float i, uint32_t col, bool aa)
{
  // int vStrip = int(i/10.0f); // hack to allow running on virtual strips (2D segment columns/rows)
  // i -= int(i);

  // if (i<0.0f || i>1.0f) return; // not normalized

  // float fC = i * (virtualLength()-1);
  // if (aa) {
  //   uint16_t iL = roundf(fC-0.49f);
  //   uint16_t iR = roundf(fC+0.49f);
  //   float    dL = (fC - iL)*(fC - iL);
  //   float    dR = (iR - fC)*(iR - fC);
  //   uint32_t cIL = getPixelColor(iL | (vStrip<<16));
  //   uint32_t cIR = getPixelColor(iR | (vStrip<<16));
  //   if (iR!=iL) {
  //     // blend L pixel
  //     cIL = ColourBlendU32(col, cIL, uint8_t(dL*255.0f));
  //     setPixelColor(iL | (vStrip<<16), cIL);
  //     // blend R pixel
  //     cIR = ColourBlendU32(col, cIR, uint8_t(dR*255.0f));
  //     setPixelColor(iR | (vStrip<<16), cIR);
  //   } else {
  //     // exact match (x & y land on a pixel)
  //     setPixelColor(iL | (vStrip<<16), col);
  //   }
  // } else {
  //   setPixelColor(uint16_t(roundf(fC)) | (vStrip<<16), col);
  // }
}

uint32_t mAnimatorLight::Segment_New::getPixelColor(int i)
{
//   int vStrip = i>>16;
//   i &= 0xFFFF;

// #ifndef WLED_DISABLE_2D
//   if (is2D()) {
//     uint16_t vH = virtualHeight();  // segment height in logical pixels
//     uint16_t vW = virtualWidth();
//     switch (map1D2D) {
//       case M12_Pixels:
//         return getPixelColorXY(i % vW, i / vW);
//         break;
//       case M12_pBar:
//         if (vStrip>0) return getPixelColorXY(vStrip - 1, vH - i -1);
//         else          return getPixelColorXY(0, vH - i -1);
//         break;
//       case M12_pArc:
//       case M12_pCorner:
//         // use longest dimension
//         return vW>vH ? getPixelColorXY(i, 0) : getPixelColorXY(0, i);
//         break;
//     }
//     return 0;
//   }
// #endif

//   if (leds) return RGBW32(leds[i].r, leds[i].g, leds[i].b, 0);

//   if (reverse) i = virtualLength() - i - 1;
//   i *= groupLength();
//   i += start;
//   /* offset/phase */
//   i += offset;
//   if (i >= stop) i -= length();
//   return strip.getPixelColor(i);
}

uint8_t mAnimatorLight::Segment_New::differs(mAnimatorLight::Segment_New& b) const {
  // uint8_t d = 0;
  // if (start != b.start)         d |= SEG_DIFFERS_BOUNDS;
  // if (stop != b.stop)           d |= SEG_DIFFERS_BOUNDS;
  // if (offset != b.offset)       d |= SEG_DIFFERS_GSO;
  // if (grouping != b.grouping)   d |= SEG_DIFFERS_GSO;
  // if (spacing != b.spacing)     d |= SEG_DIFFERS_GSO;
  // if (opacity != b.opacity)     d |= SEG_DIFFERS_BRI;
  // if (mode != b.mode)           d |= SEG_DIFFERS_FX;
  // if (speed != b.speed)         d |= SEG_DIFFERS_FX;
  // if (intensity != b.intensity) d |= SEG_DIFFERS_FX;
  // if (palette != b.palette)     d |= SEG_DIFFERS_FX;
  // if (custom1 != b.custom1)     d |= SEG_DIFFERS_FX;
  // if (custom2 != b.custom2)     d |= SEG_DIFFERS_FX;
  // if (custom3 != b.custom3)     d |= SEG_DIFFERS_FX;
  // if (startY != b.startY)       d |= SEG_DIFFERS_BOUNDS;
  // if (stopY != b.stopY)         d |= SEG_DIFFERS_BOUNDS;

  // //bit pattern: (msb first) sound:3, mapping:3, transposed, mirrorY, reverseY, [transitional, reset,] paused, mirrored, on, reverse, [selected]
  // if ((options & 0b1111111110011110U) != (b.options & 0b1111111110011110U)) d |= SEG_DIFFERS_OPT;
  // if ((options & 0x0001U) != (b.options & 0x0001U))                         d |= SEG_DIFFERS_SEL;
  // for (uint8_t i = 0; i < NUM_COLORS; i++) if (colors[i] != b.colors[i])    d |= SEG_DIFFERS_COL;

  // return d;
}

void mAnimatorLight::Segment_New::refreshLightCapabilities() {
  // uint8_t capabilities = 0x01;

  // for (uint8_t b = 0; b < busses.getNumBusses(); b++) {
  //   Bus *bus = busses.getBus(b);
  //   if (bus == nullptr || bus->getLength()==0) break;
  //   if (!bus->isOk()) continue;
  //   if (bus->getStart() >= stop) continue;
  //   if (bus->getStart() + bus->getLength() <= start) continue;

  //   uint8_t type = bus->getType();
  //   if (type == TYPE_ANALOG_1CH || (!cctFromRgb && type == TYPE_ANALOG_2CH)) capabilities &= 0xFE; // does not support RGB
  //   if (bus->isRgbw()) capabilities |= 0x02; // segment supports white channel
  //   if (!cctFromRgb) {
  //     switch (type) {
  //       case TYPE_ANALOG_5CH:
  //       case TYPE_ANALOG_2CH:
  //         capabilities |= 0x04; //segment supports white CCT 
  //     }
  //   }
  //   if (correctWB && type != TYPE_ANALOG_1CH) capabilities |= 0x04; //white balance correction (uses CCT slider)
  //   uint8_t aWM = Bus::getAutoWhiteMode()<255 ? Bus::getAutoWhiteMode() : bus->getAWMode();
  //   bool whiteSlider = (aWM == RGBW_MODE_DUAL || aWM == RGBW_MODE_MANUAL_ONLY); // white slider allowed
  //   if (bus->isRgbw() && (whiteSlider || !(capabilities & 0x01))) capabilities |= 0x08; // allow white channel adjustments (AWM allows or is not RGB)
  // }
  // _capabilities = capabilities;
}

/*
 * Fills segment with color
 */
void mAnimatorLight::Segment_New::fill(uint32_t c) {
  // const uint16_t cols = is2D() ? virtualWidth() : virtualLength();
  // const uint16_t rows = virtualHeight(); // will be 1 for 1D
  // for(uint16_t y = 0; y < rows; y++) for (uint16_t x = 0; x < cols; x++) {
  //   if (is2D()) setPixelColorXY(x, y, c);
  //   else        setPixelColor(x, c);
  // }
}

// Blends the specified color with the existing pixel color.
void mAnimatorLight::Segment_New::blendPixelColor(int n, uint32_t color, uint8_t blend) {
  // setPixelColor(n, ColourBlendU32(getPixelColor(n), color, blend));
}

// Adds the specified color with the existing pixel color perserving color balance.
void mAnimatorLight::Segment_New::addPixelColor(int n, uint32_t color) {
  // setPixelColor(n, color_add(getPixelColor(n), color));
}

void mAnimatorLight::Segment_New::fadePixelColor(uint16_t n, uint8_t fade) {
  // CRGB pix = CRGB(getPixelColor(n)).nscale8_video(fade);
  // setPixelColor(n, pix);
}

/*
 * fade out function, higher rate = quicker fade
 */
void mAnimatorLight::Segment_New::fade_out(uint8_t rate) {
  // const uint16_t cols = is2D() ? virtualWidth() : virtualLength();
  // const uint16_t rows = virtualHeight(); // will be 1 for 1D

  // rate = (255-rate) >> 1;
  // float mappedRate = float(rate) +1.1;

  // uint32_t color = colors[1]; // SEGCOLOR(1); // target color
  // int w2 = W(color);
  // int r2 = R(color);
  // int g2 = G(color);
  // int b2 = B(color);

  // for (uint16_t y = 0; y < rows; y++) for (uint16_t x = 0; x < cols; x++) {
  //   color = is2D() ? getPixelColorXY(x, y) : getPixelColor(x);
  //   int w1 = W(color);
  //   int r1 = R(color);
  //   int g1 = G(color);
  //   int b1 = B(color);

  //   int wdelta = (w2 - w1) / mappedRate;
  //   int rdelta = (r2 - r1) / mappedRate;
  //   int gdelta = (g2 - g1) / mappedRate;
  //   int bdelta = (b2 - b1) / mappedRate;

  //   // if fade isn't complete, make sure delta is at least 1 (fixes rounding issues)
  //   wdelta += (w2 == w1) ? 0 : (w2 > w1) ? 1 : -1;
  //   rdelta += (r2 == r1) ? 0 : (r2 > r1) ? 1 : -1;
  //   gdelta += (g2 == g1) ? 0 : (g2 > g1) ? 1 : -1;
  //   bdelta += (b2 == b1) ? 0 : (b2 > b1) ? 1 : -1;

  //   if (is2D()) setPixelColorXY(x, y, r1 + rdelta, g1 + gdelta, b1 + bdelta, w1 + wdelta);
  //   else        setPixelColor(x, r1 + rdelta, g1 + gdelta, b1 + bdelta, w1 + wdelta);
  // }
}

// fades all pixels to black using nscale8()
void mAnimatorLight::Segment_New::fadeToBlackBy(uint8_t fadeBy) {
  // const uint16_t cols = is2D() ? virtualWidth() : virtualLength();
  // const uint16_t rows = virtualHeight(); // will be 1 for 1D

  // for (uint16_t y = 0; y < rows; y++) for (uint16_t x = 0; x < cols; x++) {
  //   if (is2D()) setPixelColorXY(x, y, CRGB(getPixelColorXY(x,y)).nscale8(255-fadeBy));
  //   else        setPixelColor(x, CRGB(getPixelColor(x)).nscale8(255-fadeBy));
  // }
}

/*
 * blurs segment content, source: FastLED colorutils.cpp
 */
void mAnimatorLight::Segment_New::blur(uint8_t blur_amount)
{
// #ifndef WLED_DISABLE_2D
//   if (is2D()) {
//     // compatibility with 2D
//     const uint16_t cols = virtualWidth();
//     const uint16_t rows = virtualHeight();
//     for (uint16_t i = 0; i < rows; i++) blurRow(i, blur_amount); // blur all rows
//     for (uint16_t k = 0; k < cols; k++) blurCol(k, blur_amount); // blur all columns
//     return;
//   }
// #endif
//   uint8_t keep = 255 - blur_amount;
//   uint8_t seep = blur_amount >> 1;
//   CRGB carryover = CRGB::Black;
//   for(uint16_t i = 0; i < virtualLength(); i++)
//   {
//     CRGB cur = CRGB(getPixelColor(i));
//     CRGB part = cur;
//     part.nscale8(seep);
//     cur.nscale8(keep);
//     cur += carryover;
//     if(i > 0) {
//       uint32_t c = getPixelColor(i-1);
//       uint8_t r = R(c);
//       uint8_t g = G(c);
//       uint8_t b = B(c);
//       setPixelColor(i-1, qadd8(r, part.red), qadd8(g, part.green), qadd8(b, part.blue));
//     }
//     setPixelColor(i,cur.red, cur.green, cur.blue);
//     carryover = part;
//   }
}

/*
 * Put a value 0 to 255 in to get a color value.
 * The colours are a transition r -> g -> b -> back to r
 * Inspired by the Adafruit examples.
 */
uint32_t mAnimatorLight::Segment_New::color_wheel(uint8_t pos) { // TODO
  if (palette_wled) return color_from_palette(pos, false, true, 0);
  pos = 255 - pos;
  if(pos < 85) {
    return ((uint32_t)(255 - pos * 3) << 16) | ((uint32_t)(0) << 8) | (pos * 3);
  } else if(pos < 170) {
    pos -= 85;
    return ((uint32_t)(0) << 16) | ((uint32_t)(pos * 3) << 8) | (255 - pos * 3);
  } else {
    pos -= 170;
    return ((uint32_t)(pos * 3) << 16) | ((uint32_t)(255 - pos * 3) << 8) | (0);
  }
}

/*
 * Returns a new, random wheel index with a minimum distance of 42 from pos.
 */
uint8_t mAnimatorLight::Segment_New::get_random_wheel_index(uint8_t pos) {
  uint8_t r = 0, x = 0, y = 0, d = 0;

  while(d < 42) {
    r = random8();
    x = abs(pos - r);
    y = 255 - x;
    d = MIN(x, y);
  }
  return r;
}

uint8_t mAnimatorLight::Segment_New::getBrightnessRGB()
{

  uint8_t brightness = pCONT_iLight->getBriRGB_Global();
  if(seg_brightness!=255)
  {
    brightness = scale8(brightness, seg_brightness);
  }

  return brightness;

  
}


uint8_t mAnimatorLight::Segment_New::getBrightnessCCT()
{

  uint8_t brightness = pCONT_iLight->getBriCCT_Global();
  if(seg_brightness!=255)
  {
    brightness = scale8(brightness, seg_brightness);
  }

  return brightness;

  
}




/*
 * Gets a single color from the currently selected palette.
 * @param i Palette Index (if mapping is true, the full palette will be _virtualSegmentLength long, if false, 255). Will wrap around automatically.
 * @param mapping if true, LED position in segment is considered for color
 * @param wrap FastLED palettes will usally wrap back to the start smoothly. Set false to get a hard edge
 * @param mcol If the default palette 0 is selected, return the standard color 0, 1 or 2 instead. If >2, Party palette is used instead
 * @param pbri Value to scale the brightness of the returned color by. Default is 255. (no scaling)
 * @returns Single color from palette
 */
uint32_t mAnimatorLight::Segment_New::color_from_palette(uint16_t i, bool mapping, bool wrap, uint8_t mcol, uint8_t pbri)
{
  // // default palette or no RGB support on segment
  // if ((palette == 0 && mcol < NUM_COLORS) || !_isRGB) {
  //   uint32_t color = currentColor(mcol, colors[mcol]);
  //   color = gamma32(color);
  //   if (pbri == 255) return color;
  //   return RGBW32(scale8_video(R(color),pbri), scale8_video(G(color),pbri), scale8_video(B(color),pbri), scale8_video(W(color),pbri));
  // }

  // uint8_t paletteIndex = i;
  // if (mapping && virtualLength() > 1) paletteIndex = (i*255)/(virtualLength() -1);
  // if (!wrap) paletteIndex = scale8(paletteIndex, 240); //cut off blend at palette "end"
  // CRGB fastled_col;
  // CRGBPalette16 curPal;
  // if (transitional && _t) curPal = _t->_palT;
  // else                    loadPalette(curPal, palette);
  // fastled_col = ColorFromPalette(curPal, paletteIndex, pbri, (strip.paletteBlend == 3)? NOBLEND:LINEARBLEND); // NOTE: paletteBlend should be global

  // return RGBW32(fastled_col.r, fastled_col.g, fastled_col.b, 0);
}


///////////////////////////////////////////////////////////////////////////////
// mAnimatorLight::WS2812FX class implementation
///////////////////////////////////////////////////////////////////////////////

static const char _data_RESERVED[] PROGMEM = "RSVD";
static const char _data_FX_MODE_STATIC[] PROGMEM = "Solid";
uint16_t mode_static(void) {
  // SEGMENT.fill(SEGCOLOR(0));
  return 350;
}

// add (or replace reserved) effect mode and data into vector
// use id==255 to find unallocatd gaps (with "Reserved" data string)
// if vector size() is smaller than id (single) data is appended at the end (regardless of id)
void mAnimatorLight::addEffect(uint8_t id, mode_ptr mode_fn, const char *mode_name) {
  if (id == 255) { // find empty slot
    for (size_t i=1; i<_mode.size(); i++) if (_modeData[i] == _data_RESERVED) { id = i; break; }
  }
  if (id < _mode.size()) {
    if (_modeData[id] != _data_RESERVED) return; // do not overwrite alerady added effect
    _mode[id]     = mode_fn;
    _modeData[id] = mode_name;
  } else {
    _mode.push_back(mode_fn);
    _modeData.push_back(mode_name);
    if (_modeCount < _mode.size()) _modeCount++;
  }
}

void mAnimatorLight::setupEffectData() {
  // Solid must be first! (assuming vector is empty upon call to setup)
  _mode.push_back(&mode_static);
  _modeData.push_back(_data_FX_MODE_STATIC);
  // fill reserved word in case there will be any gaps in the array
  for (size_t i=1; i<_modeCount; i++) {
    _mode.push_back(&mode_static);
    _modeData.push_back(_data_RESERVED);
  }
  //all effects set here

}


//do not call this method from system context (network callback)
void mAnimatorLight::finalizeInit(void)
{
  //reset segment runtimes
  for (segment_new &seg : segments) {
    seg.markForReset();
    seg.resetIfRequired();
  }

  // // for the lack of better place enumerate ledmaps here
  // // if we do it in json.cpp (serializeInfo()) we are getting flashes on LEDs
  // // unfortunately this means we do not get updates after uploads
  // enumerateLedmaps();

  // _hasWhiteChannel = _isOffRefreshRequired = false;

  // //if busses failed to load, add default (fresh install, FS issue, ...)
  // if (busses.getNumBusses() == 0) {
  //   DEBUG_PRINTLN(F("No busses, init default"));
  //   const uint8_t defDataPins[] = {DATA_PINS};
  //   const uint16_t defCounts[] = {PIXEL_COUNTS};
  //   const uint8_t defNumBusses = ((sizeof defDataPins) / (sizeof defDataPins[0]));
  //   const uint8_t defNumCounts = ((sizeof defCounts)   / (sizeof defCounts[0]));
  //   uint16_t prevLen = 0;
  //   for (uint8_t i = 0; i < defNumBusses && i < WLED_MAX_BUSSES; i++) {
  //     uint8_t defPin[] = {defDataPins[i]};
  //     uint16_t start = prevLen;
  //     uint16_t count = defCounts[(i < defNumCounts) ? i : defNumCounts -1];
  //     prevLen += count;
  //     BusConfig defCfg = BusConfig(DEFAULT_LED_TYPE, defPin, start, count, DEFAULT_LED_COLOR_ORDER, false, 0, RGBW_MODE_MANUAL_ONLY);
  //     busses.add(defCfg);
  //   }
  // }

  // _length = 0;
  // for (uint8_t i=0; i<busses.getNumBusses(); i++) {
  //   Bus *bus = busses.getBus(i);
  //   if (bus == nullptr) continue;
  //   if (bus->getStart() + bus->getLength() > MAX_LEDS) break;
  //   //RGBW mode is enabled if at least one of the strips is RGBW
  //   _hasWhiteChannel |= bus->isRgbw();
  //   //refresh is required to remain off if at least one of the strips requires the refresh.
  //   _isOffRefreshRequired |= bus->isOffRefreshRequired();
  //   uint16_t busEnd = bus->getStart() + bus->getLength();
  //   if (busEnd > _length) _length = busEnd;
  //   #ifdef ESP8266
  //   if ((!IS_DIGITAL(bus->getType()) || IS_2PIN(bus->getType()))) continue;
  //   uint8_t pins[5];
  //   if (!bus->getPins(pins)) continue;
  //   BusDigital* bd = static_cast<BusDigital*>(bus);
  //   if (pins[0] == 3) bd->reinit();
  //   #endif
  // }

  //initialize leds array. TBD: realloc if nr of leds change
  if (mAnimatorLight::Segment_New::_globalLeds) {
    purgeSegments(true);
    free(mAnimatorLight::Segment_New::_globalLeds);
    mAnimatorLight::Segment_New::_globalLeds = nullptr;
  }
  if (useLedsArray) {
    #if defined(ARDUINO_ARCH_ESP32) && defined(WLED_USE_PSRAM)
    if (psramFound())
      mAnimatorLight::Segment_New::_globalLeds = (CRGB*) ps_malloc(sizeof(CRGB) * _length);
    else
    #endif
      mAnimatorLight::Segment_New::_globalLeds = (CRGB*) malloc(sizeof(CRGB) * _length);
    memset(mAnimatorLight::Segment_New::_globalLeds, 0, sizeof(CRGB) * _length);
  }

  //segments are created in makeAutoSegments();
  loadCustomPalettes(); // (re)load all custom palettes
  // deserializeMap();     // (re)load default ledmap
}

void mAnimatorLight::service() {
  // uint32_t nowUp = millis(); // Be aware, millis() rolls over every 49 days
  // now = nowUp + timebase;
  // if (nowUp - _lastShow < MIN_SHOW_DELAY) return;
  // bool doShow = false;

  _isServicing = true;
  _segment_index_primary = 0;
  for (segment_new &seg : segments) {
    // reset the segment runtime data if needed
    seg.resetIfRequired();

    if (!seg.isActive()) continue;

  //   // last condition ensures all solid segments are updated at the same time
  //   if(nowUp > seg.next_time || _triggered || (doShow && seg.mode == EFFECTS_FUNCTION__LENGTH__ID))
  //   {
  //     if (seg.grouping == 0) seg.grouping = 1; //sanity check
  //     doShow = true;
  //     uint16_t delay = FRAMETIME;

  //     if (!seg.freeze) { //only run effect function if not frozen
  //       _virtualSegmentLength = seg.virtualLength();
  //       _colors_t[0] = seg.currentColor(0, seg.colors[0]);
  //       _colors_t[1] = seg.currentColor(1, seg.colors[1]);
  //       _colors_t[2] = seg.currentColor(2, seg.colors[2]);
  //       seg.currentPalette(_currentPalette, seg.palette);

  //       if (!cctFromRgb || correctWB) busses.setSegmentCCT(seg.currentBri(seg.cct, true), correctWB);
  //       for (uint8_t c = 0; c < NUM_COLORS; c++) _colors_t[c] = gamma32(_colors_t[c]);

  //       // effect blending (execute previous effect)
  //       // actual code may be a bit more involved as effects have runtime data including allocated memory
  //       //if (seg.transitional && seg._modeP) (*_mode[seg._modeP])(progress());
  //       delay = (*_mode[seg.currentMode(seg.mode)])();
  //       if (seg.mode != FX_MODE_HALLOWEEN_EYES) seg.call++;
  //       if (seg.transitional && delay > FRAMETIME) delay = FRAMETIME; // foce faster updates during transition

  //       seg.handleTransition();
  //     }

  //     seg.next_time = nowUp + delay;
  //   }
    _segment_index_primary++;
  }
  // _virtualSegmentLength = 0;
  // busses.setSegmentCCT(-1);
  // if(doShow) {
  //   yield();
  //   show();
  // }
  // _triggered = false;
  // _isServicing = false;
}

// IS contained within segment so busses etc are known (ie the right pin will be used)
void IRAM_ATTR mAnimatorLight::setPixelColor(int i, uint32_t col)
{
  if (i >= _length) return;
  if (i < customMappingSize) i = customMappingTable[i];
  // busses.setPixelColor(i, col);
}

uint32_t mAnimatorLight::getPixelColor(uint16_t i)
{
  // if (i >= _length) return 0;
  // if (i < customMappingSize) i = customMappingTable[i];
  // return busses.getPixelColor(i);
}


//DISCLAIMER
//The following function attemps to calculate the current LED power usage,
//and will limit the brightness to stay below a set amperage threshold.
//It is NOT a measurement and NOT guaranteed to stay within the ablMilliampsMax margin.
//Stay safe with high amperage and have a reasonable safety margin!
//I am NOT to be held liable for burned down garages!

//fine tune power estimation constants for your setup                  
#define MA_FOR_ESP        100 //how much mA does the ESP use (Wemos D1 about 80mA, ESP32 about 120mA)
                              //you can set it to 0 if the ESP is powered by USB and the LEDs by external

void mAnimatorLight::estimateCurrentAndLimitBri() {
  //power limit calculation
  //each LED can draw up 195075 "power units" (approx. 53mA)
  //one PU is the power it takes to have 1 channel 1 step brighter per brightness step
  //so A=2,R=255,G=0,B=0 would use 510 PU per LED (1mA is about 3700 PU)
  bool useWackyWS2815PowerModel = false;
  byte actualMilliampsPerLed = milliampsPerLed;

  // if(milliampsPerLed == 255) {
  //   useWackyWS2815PowerModel = true;
  //   actualMilliampsPerLed = 12; // from testing an actual strip
  // }

  // if (ablMilliampsMax < 150 || actualMilliampsPerLed == 0) { //0 mA per LED and too low numbers turn off calculation
  //   currentMilliamps = 0;
  //   busses.setBrightness(_brightness);
  //   return;
  // }

  // uint16_t pLen = getLengthPhysical();
  // uint32_t puPerMilliamp = 195075 / actualMilliampsPerLed;
  // uint32_t powerBudget = (ablMilliampsMax - MA_FOR_ESP) * puPerMilliamp; //100mA for ESP power
  // if (powerBudget > puPerMilliamp * pLen) { //each LED uses about 1mA in standby, exclude that from power budget
  //   powerBudget -= puPerMilliamp * pLen;
  // } else {
  //   powerBudget = 0;
  // }

  // uint32_t powerSum = 0;

  // for (uint8_t b = 0; b < busses.getNumBusses(); b++) {
  //   Bus *bus = busses.getBus(b);
  //   if (bus->getType() >= TYPE_NET_DDP_RGB) continue; //exclude non-physical network busses
  //   uint16_t len = bus->getLength();
  //   uint32_t busPowerSum = 0;
  //   for (uint16_t i = 0; i < len; i++) { //sum up the usage of each LED
  //     uint32_t c = bus->getPixelColor(i);
  //     byte r = R(c), g = G(c), b = B(c), w = W(c);

  //     if(useWackyWS2815PowerModel) { //ignore white component on WS2815 power calculation
  //       busPowerSum += (MAX(MAX(r,g),b)) * 3;
  //     } else {
  //       busPowerSum += (r + g + b + w);
  //     }
  //   }

  //   if (bus->isRgbw()) { //RGBW led total output with white LEDs enabled is still 50mA, so each channel uses less
  //     busPowerSum *= 3;
  //     busPowerSum = busPowerSum >> 2; //same as /= 4
  //   }
  //   powerSum += busPowerSum;
  // }

  // uint32_t powerSum0 = powerSum;
  // powerSum *= _brightness;
  
  // if (powerSum > powerBudget) //scale brightness down to stay in current limit
  // {
  //   float scale = (float)powerBudget / (float)powerSum;
  //   uint16_t scaleI = scale * 255;
  //   uint8_t scaleB = (scaleI > 255) ? 255 : scaleI;
  //   uint8_t newBri = scale8(_brightness, scaleB);
  //   busses.setBrightness(newBri); //to keep brightness uniform, sets virtual busses too
  //   currentMilliamps = (powerSum0 * newBri) / puPerMilliamp;
  // } else {
  //   currentMilliamps = powerSum / puPerMilliamp;
  //   busses.setBrightness(_brightness);
  // }
  // currentMilliamps += MA_FOR_ESP; //add power of ESP back to estimate
  // currentMilliamps += pLen; //add standby power back to estimate
}

void mAnimatorLight::show(void) {

  // avoid race condition, caputre _callback value
  show_callback callback = _callback;
  if (callback) callback();

  estimateCurrentAndLimitBri();
  
  // some buses send asynchronously and this method will return before
  // all of the data has been sent.
  // See https://github.com/Makuna/NeoPixelBus/wiki/ESP32-NeoMethods#neoesp32rmt-methods
  // busses.show();
  unsigned long now = millis();
  unsigned long diff = now - _lastShow;
  uint16_t fpsCurr = 200;
  if (diff > 0) fpsCurr = 1000 / diff;
  _cumulativeFps = (3 * _cumulativeFps + fpsCurr) >> 2;
  _lastShow = now;
}

/**
 * Returns a true value if any of the strips are still being updated.
 * On some hardware (ESP32), strip updates are done asynchronously.
 */
bool mAnimatorLight::isUpdating() {
  // return !busses.canAllShow();
}

/**
 * Returns the refresh rate of the LED strip. Useful for finding out whether a given setup is fast enough.
 * Only updates on show() or is set to 0 fps if last show is more than 2 secs ago, so accurary varies
 */
uint16_t mAnimatorLight::getFps() {
  if (millis() - _lastShow > 2000) return 0;
  return _cumulativeFps +1;
}

void mAnimatorLight::setTargetFps(uint8_t fps) {
  if (fps > 0 && fps <= 120) _targetFps = fps;
  _frametime = 1000 / _targetFps;
}

void mAnimatorLight::setMode(uint8_t segid, uint8_t m) {
  if (segid >= segments.size()) return;
   
  if (m >= getModeCount()) m = getModeCount() - 1;

  if (segments[segid].animation_mode_id != m) {
    segments[segid].startTransition(_transitionDur); // set effect transitions
    //segments[segid].markForReset();
    segments[segid].animation_mode_id = m;
  }
}

//applies to all active and selected segments
void mAnimatorLight::setColor(uint8_t slot, uint32_t c) {
  if (slot >= NUM_COLORS) return;

  for (segment_new &seg : segments) {
    if (seg.isActive() && seg.isSelected()) {
      seg.setColor(slot, c);
    }
  }
}

void mAnimatorLight::setCCT(uint16_t k) {
  for (segment_new &seg : segments) {
    if (seg.isActive() && seg.isSelected()) {
      seg.setCCT(k);
    }
  }
}

void mAnimatorLight::setBrightness(uint8_t b, bool direct) {
  // if (gammaCorrectBri) b = gamma8(b);  
  if (_brightness == b) return;
  _brightness = b;
  if (_brightness == 0) { //unfreeze all segments on power off
    for (segment_new &seg : segments) {
      seg.freeze = false;
    }
  }
  if (direct) {
    // would be dangerous if applied immediately (could exceed ABL), but will not output until the next show()
    // busses.setBrightness(b);
  } else {
    unsigned long t = millis();
    if (segments[0].next_time > t + 22 && t - _lastShow > MIN_SHOW_DELAY) show(); //apply brightness change immediately if no refresh soon
  }
}

uint8_t mAnimatorLight::getFirstSelectedSegId(void)
{
  size_t i = 0;
  for (segment_new &seg : segments) {
    if (seg.isActive() && seg.isSelected()) return i;
    i++;
  }
  // if none selected, use the main segment
  return getMainSegmentId();
}

void mAnimatorLight::setMainSegmentId(uint8_t n) {
  _mainSegment = 0;
  if (n < segments.size()) {
    _mainSegment = n;
  }
  return;
}

uint8_t mAnimatorLight::getLastActiveSegmentId(void) {
  for (size_t i = segments.size() -1; i > 0; i--) {
    if (segments[i].isActive()) return i;
  }
  return 0;
}

uint8_t mAnimatorLight::getActiveSegmentsNum(void) {
  uint8_t c = 0;
  for (size_t i = 0; i < segments.size(); i++) {
    if (segments[i].isActive()) c++;
  }
  return c;
}

uint16_t mAnimatorLight::getLengthPhysical(void) {
  uint16_t len = 0;
  // for (size_t b = 0; b < busses.getNumBusses(); b++) {
  //   Bus *bus = busses.getBus(b);
  //   if (bus->getType() >= TYPE_NET_DDP_RGB) continue; //exclude non-physical network busses
  //   len += bus->getLength();
  // }
  return len;
}

//used for JSON API info.leds.rgbw. Little practical use, deprecate with info.leds.rgbw.
//returns if there is an RGBW bus (supports RGB and White, not only white)
//not influenced by auto-white mode, also true if white slider does not affect output white channel
bool mAnimatorLight::hasRGBWBus(void) {
  // for (size_t b = 0; b < busses.getNumBusses(); b++) {
  //   Bus *bus = busses.getBus(b);
  //   if (bus == nullptr || bus->getLength()==0) break;
  //   switch (bus->getType()) {
  //     case TYPE_SK6812_RGBW:
  //     case TYPE_TM1814:
  //     case TYPE_ANALOG_4CH:
  //       return true;
  //   }
  // }
  // return false;
}

bool mAnimatorLight::hasCCTBus(void) {
  // if (cctFromRgb && !correctWB) return false;
  // for (size_t b = 0; b < busses.getNumBusses(); b++) {
  //   Bus *bus = busses.getBus(b);
  //   if (bus == nullptr || bus->getLength()==0) break;
  //   switch (bus->getType()) {
  //     case TYPE_ANALOG_5CH:
  //     case TYPE_ANALOG_2CH:
  //       return true;
  //   }
  // }
  // return false;
}

void mAnimatorLight::purgeSegments(bool force) 
{
  ALOG_INF(PSTR("purgeSegments(%d)"),force);
  ALOG_INF(PSTR("segments.size=%d"),segments.size());
  // remove all inactive segments (from the back)
  int deleted = 0;
  if (segments.size() <= 1) return;

  for (size_t i = segments.size()-1; i > 0; i--)
  {
    ALOG_INF(PSTR("i=%d"),i);
    if (segments[i].pixel_range.stop == 0 || force) {
      DEBUG_PRINT(F("Purging segment segment: ")); DEBUG_PRINTLN(i);
      deleted++;
      segments.erase(segments.begin() + i);
    }
  }
  if (deleted) {
    segments.shrink_to_fit();
    if (_mainSegment >= segments.size()) setMainSegmentId(0);
  }
}

mAnimatorLight::Segment_New& mAnimatorLight::getSegment(uint8_t id) {
  return segments[id >= segments.size() ? getMainSegmentId() : id]; // vectors
}

/**
 * @brief For turning on a new effect/segment
 * 
 * @param n 
 * @param i1 
 * @param i2 
 * @param grouping 
 * @param spacing 
 * @param offset 
 * @param startY 
 * @param stopY 
 */
void mAnimatorLight::setSegment(uint8_t n, uint16_t i1, uint16_t i2, uint8_t grouping, uint8_t spacing, uint16_t offset, uint16_t startY, uint16_t stopY) {
  if (n >= segments.size()) return;
  mAnimatorLight::Segment_New& seg = segments[n];

  //return if neither bounds nor grouping have changed
  bool boundsUnchanged = (seg.pixel_range.start == i1 && seg.pixel_range.stop == i2);
  if (isMatrix) {
    boundsUnchanged &= (seg.startY == startY && seg.stopY == stopY);
  }
  if (boundsUnchanged
      && (!grouping || (seg.grouping == grouping && seg.spacing == spacing))
      && (offset == UINT16_MAX || offset == seg.offset)) return;

  //if (seg.stop) setRange(seg.start, seg.stop -1, BLACK); //turn old segment range off
  if (seg.pixel_range.stop) seg.fill(BLACK); //turn old segment range off
  if (i2 <= i1) //disable segment
  {
    // disabled segments should get removed using purgeSegments()
    DEBUG_PRINT(F("-- mAnimatorLight::Segment_New ")); DEBUG_PRINT(n); DEBUG_PRINTLN(F(" marked inactive."));
    seg.pixel_range.stop = 0;
    //if (seg.name) {
    //  delete[] seg.name;
    //  seg.name = nullptr;
    //}
    // if main segment is deleted, set first active as main segment
    if (n == _mainSegment) setMainSegmentId(0);
    seg.markForReset();
    return;
  }
  if (isMatrix) {
    #ifndef WLED_DISABLE_2D
    if (i1 < mAnimatorLight::Segment_New::maxWidth) seg.start = i1;
    seg.stop = i2 > mAnimatorLight::Segment_New::maxWidth ? mAnimatorLight::Segment_New::maxWidth : i2;
    if (startY < mAnimatorLight::Segment_New::maxHeight) seg.startY = startY;
    seg.stopY = stopY > mAnimatorLight::Segment_New::maxHeight ? mAnimatorLight::Segment_New::maxHeight : MAX(1,stopY);
    #endif
  } else {
    if (i1 < _length) seg.pixel_range.start = i1;
    seg.pixel_range.stop = i2 > _length ? _length : i2;
    seg.startY = 0;
    seg.stopY  = 1;
  }
  if (grouping) {
    seg.grouping = grouping;
    seg.spacing = spacing;
  }
  if (offset < UINT16_MAX) seg.offset = offset;
  seg.markForReset();
  if (!boundsUnchanged) seg.refreshLightCapabilities();
}

void mAnimatorLight::restartRuntime() {
  for (segment_new &seg : segments) seg.markForReset();
}

void mAnimatorLight::resetSegments2() {
  segments.clear(); // destructs all mAnimatorLight::Segment_New as part of clearing
  #ifndef WLED_DISABLE_2D
  segment_new seg = isMatrix ? mAnimatorLight::Segment_New(0, mAnimatorLight::Segment_New::maxWidth, 0, mAnimatorLight::Segment_New::maxHeight) : mAnimatorLight::Segment_New(0, _length);
  #else
  segment_new seg = mAnimatorLight::Segment_New(0, _length);
  #endif
  segments.push_back(seg);
  _mainSegment = 0;
}

void mAnimatorLight::makeAutoSegments(bool forceReset) {
  if (isMatrix) {
    #ifndef WLED_DISABLE_2D
    // only create 1 2D segment
    if (forceReset || getSegmentsNum() == 0) resetSegments2(); // initialises 1 segment
    else if (getActiveSegmentsNum() == 1) {
      size_t i = getLastActiveSegmentId();
      segments[i].start  = 0;
      segments[i].stop   = mAnimatorLight::Segment_New::maxWidth;
      segments[i].startY = 0;
      segments[i].stopY  = mAnimatorLight::Segment_New::maxHeight;
      segments[i].grouping = 1;
      segments[i].spacing  = 0;
      _mainSegment = i;
    }
    #endif
  } else 
  // if (autoSegments) { //make one segment per bus
  //   uint16_t segStarts[MAX_NUM_SEGMENTS] = {0};
  //   uint16_t segStops [MAX_NUM_SEGMENTS] = {0};
  //   uint8_t s = 0;
  //   // for (uint8_t i = 0; i < busses.getNumBusses(); i++) {
  //   //   Bus* b = busses.getBus(i);

  //   //   segStarts[s] = b->getStart();
  //   //   segStops[s]  = segStarts[s] + b->getLength();

  //   //   //check for overlap with previous segments
  //   //   for (size_t j = 0; j < s; j++) {
  //   //     if (segStops[j] > segStarts[s] && segStarts[j] < segStops[s]) {
  //   //       //segments overlap, merge
  //   //       segStarts[j] = min(segStarts[s],segStarts[j]);
  //   //       segStops [j] = max(segStops [s],segStops [j]); segStops[s] = 0;
  //   //       s--;
  //   //     }
  //   //   }
  //   //   s++;
  //   // }
  //   segments.clear();
  //   for (size_t i = 0; i < s; i++) {
  //     mAnimatorLight::Segment_New seg = mAnimatorLight::Segment_New(segStarts[i], segStops[i]);
  //     seg.selected = true;
  //     segments.push_back(seg);
  //   }
  //   _mainSegment = 0;
  // } else 
  {
    if (forceReset || getSegmentsNum() == 0) resetSegments2();
    //expand the main seg to the entire length, but only if there are no other segments, or reset is forced
    else if (getActiveSegmentsNum() == 1) {
      size_t i = getLastActiveSegmentId();
      segments[i].pixel_range.start = 0;
      segments[i].pixel_range.stop  = _length;
      _mainSegment = 0;
    }
  }

  fixInvalidSegments();
}

void mAnimatorLight::fixInvalidSegments() {
  //make sure no segment is longer than total (sanity check)
  for (size_t i = getSegmentsNum()-1; i > 0; i--) {
    if (segments[i].pixel_range.start >= _length) { segments.erase(segments.begin()+i); continue; }
    if (segments[i].pixel_range.stop  >  _length) segments[i].pixel_range.stop = _length;
    // this is always called as the last step after finalizeInit(), update covered bus types
    segments[i].refreshLightCapabilities();
  }
}

//true if all segments align with a bus, or if a segment covers the total length
bool mAnimatorLight::checkSegmentAlignment() {
  bool aligned = false;
  for (segment_new &seg : segments) {
    // for (uint8_t b = 0; b<busses.getNumBusses(); b++) {
    //   Bus *bus = busses.getBus(b);
    //   if (seg.start == bus->getStart() && seg.stop == bus->getStart() + bus->getLength()) aligned = true;
    // }
    if (seg.pixel_range.start == 0 && seg.pixel_range.stop == _length) aligned = true;
    if (!aligned) return false;
  }
  return true;
}

//After this function is called, setPixelColor() will use that segment (offsets, grouping, ... will apply)
//Note: If called in an interrupt (e.g. JSON API), original segment must be restored,
//otherwise it can lead to a crash on ESP32 because _segment_index is modified while in use by the main thread
uint8_t mAnimatorLight::setPixelSegment(uint8_t n)
{
  uint8_t prevSegId = _segment_index_primary;
  if (n < segments.size()) {
    _segment_index_primary = n;
    _virtualSegmentLength = segments[_segment_index_primary].virtualLength();
  }
  return prevSegId;
}

void mAnimatorLight::setRange(uint16_t i, uint16_t i2, uint32_t col)
{
  if (i2 >= i)
  {
    for (uint16_t x = i; x <= i2; x++) setPixelColor(x, col);
  } else
  {
    for (uint16_t x = i2; x <= i; x++) setPixelColor(x, col);
  }
}

void mAnimatorLight::setTransitionMode(bool t)
{
  // for (segment &seg : segments) if (!seg.transitional) seg.startTransition(t ? _transitionDur : 0);
}

#ifdef ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES
void mAnimatorLight::printSize()
{
  size_t size = 0;
  for (const mAnimatorLight::Segment_New &seg : segments) size += seg.getSize();
  DEBUG_PRINTF("Segments: %d -> %uB\n", segments.size(), size);
  DEBUG_PRINTF("Modes: %d*%d=%uB\n", sizeof(mode_ptr), _mode.size(), (_mode.capacity()*sizeof(mode_ptr)));
  DEBUG_PRINTF("Data: %d*%d=%uB\n", sizeof(const char *), _modeData.size(), (_modeData.capacity()*sizeof(const char *)));
  DEBUG_PRINTF("Map: %d*%d=%uB\n", sizeof(uint16_t), (int)customMappingSize, customMappingSize*sizeof(uint16_t));
  if (useLedsArray) DEBUG_PRINTF("Buffer: %d*%d=%uB\n", sizeof(CRGB), (int)_length, _length*sizeof(CRGB));
}
#endif

void mAnimatorLight::loadCustomPalettes()
{
  byte tcp[72]; //support gradient palettes with up to 18 entries
  CRGBPalette16 targetPalette;
  customPalettes.clear(); // start fresh
  // for (int index = 0; index<10; index++) {
  //   char fileName[32];
  //   sprintf_P(fileName, PSTR("/palette%d.json"), index);

  //   StaticJsonDocument<1536> pDoc; // barely enough to fit 72 numbers
  //   if (WLED_FS.exists(fileName)) {
  //     DEBUG_PRINT(F("Reading palette from "));
  //     DEBUG_PRINTLN(fileName);

  //     if (readObjectFromFile(fileName, nullptr, &pDoc)) {
  //       JsonArray pal = pDoc[F("palette")];
  //       if (!pal.isNull() && pal.size()>7) { // not an empty palette (at least 2 entries)
  //         size_t palSize = MIN(pal.size(), 72);
  //         palSize -= palSize % 4; // make sure size is multiple of 4
  //         for (size_t i=0; i<palSize && pal[i].as<int>()<256; i+=4) {
  //           tcp[ i ] = (uint8_t) pal[ i ].as<int>(); // index
  //           tcp[i+1] = (uint8_t) pal[i+1].as<int>(); // R
  //           tcp[i+2] = (uint8_t) pal[i+2].as<int>(); // G
  //           tcp[i+3] = (uint8_t) pal[i+3].as<int>(); // B
  //           DEBUG_PRINTF("%d(%d) : %d %d %d\n", i, int(tcp[i]), int(tcp[i+1]), int(tcp[i+2]), int(tcp[i+3]));
  //         }
  //         customPalettes.push_back(targetPalette.loadDynamicGradientPalette(tcp));
  //       }
  //     }
  //   } else {
  //     break;
  //   }
  // }
}

RgbcctColor 
#ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
IRAM_ATTR 
#endif 
mAnimatorLight::Segment_New::GetColourFromPalette(
  uint16_t _pixel_position,//desired_index_from_palette,  
  uint8_t* encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
  bool     flag_map_scaling, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
  bool     flag_wrap,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
  uint8_t mcol, // will be phased out
  bool     flag_convert_pixel_index_to_get_exact_crgb_colour,   // added by me, to make my effects work with CRGBPalette16
  uint8_t  brightness_scale //255(default): No scaling, 0-255 scales the brightness of returned colour (remember all colours are saved in full 255 scale)
  // uint8_t* discrete_colours_in_palette //ie length of palette as optional return
){

  return mPaletteI->GetColourFromPreloadedPaletteBuffer(
    palette.id,
    (uint8_t*)palette_container->pData.data(),//desired_index_from_palette,  
    _pixel_position,
    encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
    flag_map_scaling, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
    flag_wrap,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
    mcol, // will be phased out
    flag_convert_pixel_index_to_get_exact_crgb_colour,   // added by me, to make my effects work with CRGBPalette16
    brightness_scale //255(default): No scaling, 0-255 scales the brightness of returned colour (remember all colours are saved in full 255 scale)
    // uint8_t* discrete_colours_in_palette //ie length of palette as optional return
  );

}







//load custom mapping table from JSON file (called from finalizeInit() or deserializeState())
void mAnimatorLight::deserializeMap(uint8_t n) {
  // if (isMatrix) return; // 2D support creates its own ledmap

  // char fileName[32];
  // strcpy_P(fileName, PSTR("/ledmap"));
  // if (n) sprintf(fileName +7, "%d", n);
  // strcat(fileName, ".json");
  // bool isFile = WLED_FS.exists(fileName);

  // if (!isFile) {
  //   // erase custom mapping if selecting nonexistent ledmap.json (n==0)
  //   if (!n && customMappingTable != nullptr) {
  //     customMappingSize = 0;
  //     delete[] customMappingTable;
  //     customMappingTable = nullptr;
  //   }
  //   return;
  // }

  // if (!requestJSONBufferLock(7)) return;

  // DEBUG_PRINT(F("Reading LED map from "));
  // DEBUG_PRINTLN(fileName);

  // if (!readObjectFromFile(fileName, nullptr, &doc)) {
  //   releaseJSONBufferLock();
  //   return; //if file does not exist just exit
  // }

  // // erase old custom ledmap
  // if (customMappingTable != nullptr) {
  //   customMappingSize = 0;
  //   delete[] customMappingTable;
  //   customMappingTable = nullptr;
  // }

  // JsonArray map = doc[F("map")];
  // if (!map.isNull() && map.size()) {  // not an empty map
  //   customMappingSize  = map.size();
  //   customMappingTable = new uint16_t[customMappingSize];
  //   for (uint16_t i=0; i<customMappingSize; i++) {
  //     customMappingTable[i] = (uint16_t) map[i];
  //   }
  // }

  // releaseJSONBufferLock();
}


// //Bus static member definition, would belong in bus_manager.cpp

#ifdef ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
//Bus static member definition, would belong in bus_manager.cpp
int16_t Bus::_cct = -1;
uint8_t Bus::_cctBlend = 0;
uint8_t Bus::_gAWM = 255;
#endif // ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT


// const char JSON_mode_names[] PROGMEM = R"=====(["FX names moved"])=====";
// const char JSON_palette_names[] PROGMEM = R"=====([
// "Default","* Random Cycle","* Color 1","* Colors 1&2","* Color Gradient","* Colors Only","Party","Cloud","Lava","Ocean",
// "Forest","Rainbow","Rainbow Bands","Sunset","Rivendell","Breeze","Red & Blue","Yellowout","Analogous","Splash",
// "Pastel","Sunset 2","Beach","Vintage","Departure","Landscape","Beech","Sherbet","Hult","Hult 64",
// "Drywet","Jul","Grintage","Rewhi","Tertiary","Fire","Icefire","Cyane","Light Pink","Autumn",
// "Magenta","Magred","Yelmag","Yelblu","Orange & Teal","Tiamat","April Night","Orangery","C9","Sakura",
// "Aurora","Atlantica","C9 2","C9 New","Temperature","Aurora 2","Retro Clown","Candy","Toxy Reaf","Fairy Reaf",
// "Semi Blue","Pink Candy","Red Reaf","Aqua Flash","Yelblu Hot","Lite Light","Red Flash","Blink Red","Red Shift","Red Tide",
// "Candy2"
// ])=====";



void IRAM_ATTR mAnimatorLight::Segment_New::SetPixelColor_All(RgbcctColor colour)
{
  for(uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    SetPixelColor(pixel, colour);
  }
  pCONT_iLight->ShowInterface();
}


void IRAM_ATTR mAnimatorLight::Segment_New::SetPixelColor(uint16_t indexPixel, uint8_t red, uint8_t green, uint8_t blue, bool brightness_needs_applied)
{
  SetPixelColor(indexPixel, RgbColor(red,green,blue), brightness_needs_applied);
}
 
  
void IRAM_ATTR mAnimatorLight::Segment_New::SetPixelColor(uint16_t indexPixel, uint32_t color, bool brightness_needs_applied)
{
  RgbcctColor col = RgbcctColor(0);
  col.red =   (color >> 16 & 0xFF);
  col.green = (color >> 8  & 0xFF);
  col.blue =  (color       & 0xFF);
  col.W1 =    (color >> 24 & 0xFF);
  col.W2 =    (color >> 24 & 0xFF);
  SetPixelColor(indexPixel, col, brightness_needs_applied);
}


/**
 * @brief 
 * 
 * @param indexPixel 
 * @param color_internal 
 * @param brightness_needs_applied 
 */
void IRAM_ATTR mAnimatorLight::Segment_New::SetPixelColor(uint16_t indexPixel, RgbcctColor color_internal, bool brightness_needs_applied)
{

  int vStrip = indexPixel>>16; // hack to allow running on virtual strips (2D segment columns/rows)
  indexPixel &= 0xFFFF;

  if (indexPixel >= virtualLength() || indexPixel<0) return;  // if pixel would fall out of segment just exit

  #ifndef WLED_DISABLE_2D
    if (is2D()) {
      uint16_t vH = virtualHeight();  // segment height in logical pixels
      uint16_t vW = virtualWidth();
      switch (map1D2D) {
        case M12_Pixels:
          // use all available pixels as a long strip
          setPixelColorXY(i % vW, i / vW, col);
          break;
        case M12_pBar:
          // expand 1D effect vertically or have it play on virtual strips
          if (vStrip>0) setPixelColorXY(vStrip - 1, vH - i - 1, col);
          else          for (int x = 0; x < vW; x++) setPixelColorXY(x, vH - i - 1, col);
          break;
        case M12_pArc:
          // expand in circular fashion from center
          if (i==0)
            setPixelColorXY(0, 0, col);
          else {
            float step = HALF_PI / (2.85f*i);
            for (float rad = 0.0f; rad <= HALF_PI+step/2; rad += step) {
              // may want to try float version as well (with or without antialiasing)
              int x = roundf(sin_t(rad) * i);
              int y = roundf(cos_t(rad) * i);
              setPixelColorXY(x, y, col);
            }
          }
          break;
        case M12_pCorner:
          for (int x = 0; x <= i; x++) setPixelColorXY(x, i, col);
          for (int y = 0; y <  i; y++) setPixelColorXY(i, y, col);
          break;
      }
      return;
    } else if (Segment::maxHeight!=1 && (width()==1 || height()==1)) {
      // we have a vertical or horizontal 1D segment (WARNING: virtual...() may be transposed)
      int x = 0, y = 0;
      if (virtualHeight()>1) y = i;
      if (virtualWidth() >1) x = i;
      setPixelColorXY(x, y, col);
      return;
    }
  #endif

  // if (leds) leds[i] = col; // custom led mapping

  // uint16_t len = length();


  /**
   * @brief Apply brightness if required
   * 
   */
  if(brightness_needs_applied){
    // uint8_t _bri_t = currentBri(on ? opacity : 0);
    uint8_t brightness = pCONT_iLight->getBriRGB_Global();

    if(seg_brightness!=255)
    {
      brightness = scale8(brightness, seg_brightness);
    }

    color_internal.R  = scale8(color_internal.R,  brightness);
    color_internal.G  = scale8(color_internal.G,  brightness);
    color_internal.B  = scale8(color_internal.B,  brightness);
    color_internal.W1 = scale8(color_internal.W1, brightness);
    color_internal.W2 = scale8(color_internal.W2, brightness);
    
  }

  /**
   * @brief Apply Pixel hardware colour mapping from internal to hardware order
   **/
  RgbcctColor color_hardware = color_internal; // Copy
  if(hardware_element_colour_order.red        != D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE){ color_hardware.raw[hardware_element_colour_order.red]         = color_internal.R;  }
  if(hardware_element_colour_order.green      != D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE){ color_hardware.raw[hardware_element_colour_order.green]       = color_internal.G;  }
  if(hardware_element_colour_order.blue       != D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE){ color_hardware.raw[hardware_element_colour_order.blue]        = color_internal.B;  }
  if(hardware_element_colour_order.white_cold != D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE){ color_hardware.raw[hardware_element_colour_order.white_cold]  = color_internal.WC; }
  if(hardware_element_colour_order.white_warm != D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE){ color_hardware.raw[hardware_element_colour_order.white_warm]  = color_internal.WW; }

  uint16_t physical_indexPixel = indexPixel; // Going from virtual/internal index to physical/external index
  uint16_t segment_length      = length();

  /**
   * @brief Modify pixel index if required: start, grouping, spacing, offset
   **/
  physical_indexPixel = physical_indexPixel * groupLength();
  if (reverse) { // is segment reversed?
    if (mirror) { // is segment mirrored?
      physical_indexPixel = (segment_length - 1) / 2 - physical_indexPixel;  //only need to index half the pixels
    } else {
      physical_indexPixel = (segment_length - 1) - physical_indexPixel;
    }
  }
  physical_indexPixel += pixel_range.start;

  #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
  ALOG_INF( PSTR("pIndex=%d,%d"), physical_indexPixel-pixel_range.start, physical_indexPixel);
  #endif // ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR


  /**
   * @brief Set all pixels in the group
   **/
  for (uint16_t j = 0; j < grouping; j++) 
  {

    uint16_t indexSet = physical_indexPixel + ((reverse) ? -j : j);
    
    if (
      indexSet >= pixel_range.start && 
      indexSet <  pixel_range.stop
    ){

      if (mirror) 
      { //set the corresponding mirrored pixel
        uint16_t indexMir = pixel_range.stop - indexSet + pixel_range.start - 1;          
        indexMir += offset; // offset/phase

        if (indexMir >= pixel_range.stop) indexMir -= segment_length; // Wrap
        pCONT_iLight->SetPixelColourHardwareInterface(color_hardware, indexMir);

      }
      indexSet += offset; // offset/phase

      if (indexSet >= pixel_range.stop) indexSet -= segment_length; // Wrap
      pCONT_iLight->SetPixelColourHardwareInterface(color_hardware, indexSet);
    }

  }

}

/**
 * @brief 
 * 
 * @param indexPixel 
 * @return RgbcctColor 
 */
RgbcctColor IRAM_ATTR mAnimatorLight::Segment_New::GetPixelColor(uint16_t indexPixel)
{
  
  int vStrip = indexPixel>>16;
  indexPixel &= 0xFFFF;

#ifndef WLED_DISABLE_2D
  if (is2D()) {
    uint16_t vH = virtualHeight();  // segment height in logical pixels
    uint16_t vW = virtualWidth();
    switch (map1D2D) {
      case M12_Pixels:
        return getPixelColorXY(i % vW, i / vW);
        break;
      case M12_pBar:
        if (vStrip>0) return getPixelColorXY(vStrip - 1, vH - i -1);
        else          return getPixelColorXY(0, vH - i -1);
        break;
      case M12_pArc:
      case M12_pCorner:
        // use longest dimension
        return vW>vH ? getPixelColorXY(i, 0) : getPixelColorXY(0, i);
        break;
    }
    return 0;
  }
#endif

  // Custom mapping
  // if (leds) return RGBW32(leds[i].r, leds[i].g, leds[i].b, 0);

  uint16_t physical_indexPixel = indexPixel;

  
  if (reverse) physical_indexPixel = virtualLength() - physical_indexPixel - 1;
  physical_indexPixel *= groupLength();
  physical_indexPixel += pixel_range.start;
  /* offset/phase */
  physical_indexPixel += offset;
  if (physical_indexPixel >= pixel_range.stop) physical_indexPixel -= length();
  
  RgbcctColor colour_hardware = pCONT_iLight->GetPixelColourHardwareInterface(physical_indexPixel);
  RgbcctColor colour_internal = colour_hardware;

  /**
   * @brief Convert from external colour back into internal colour (This step is not required when using WLED busses, as it is set by the neopixel method)
   **/
  if(hardware_element_colour_order.red        != D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE){ colour_internal.raw[hardware_element_colour_order.red]         = colour_hardware.R;  }
  if(hardware_element_colour_order.green      != D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE){ colour_internal.raw[hardware_element_colour_order.green]       = colour_hardware.G;  }
  if(hardware_element_colour_order.blue       != D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE){ colour_internal.raw[hardware_element_colour_order.blue]        = colour_hardware.B;  }
  if(hardware_element_colour_order.white_cold != D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE){ colour_internal.raw[hardware_element_colour_order.white_cold]  = colour_hardware.WC; }
  if(hardware_element_colour_order.white_warm != D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE){ colour_internal.raw[hardware_element_colour_order.white_warm]  = colour_hardware.WW; }

  return colour_internal;

}














#endif //USE_MODULE_LIGHTS_ANIMATOR




