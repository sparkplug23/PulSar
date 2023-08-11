/**
 * @file mAnimatorLight_Segments_Effects.cpp
 * @author Michael Doone (michaeldoonehub@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

// // color = RgbcctColor::GetU32Colour(SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE)); // Functionally the same
// color = RgbcctColor::GetU32Colour(SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON));     

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Solid Colour
 * @description:   : For full RGBCCT colour
 * 
 * @param intensity: None
 * @param speed    : None
 * @param rate     : None
 * @param time     : Blend time on first/only update
 * Time/Rate should be set elsewhere to use default amounts, and only when requested transition into this differently (eg playlist effects that use it for special visual blending)
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
void mAnimatorLight::EffectAnim__Solid_Colour()
{

  if (!SEGMENT.allocateData( GetSizeOfPixel(SEGMENT.colour_type) * 2) ){ return; } // Pixel_Width * Two_Channels

  RgbcctColor desired_colour = SEGMENT.GetPaletteColour();
  // desired_colour.debug_print("before brightness");
  desired_colour = ApplyBrightnesstoRgbcctColour( desired_colour, SEGMENT.getBrightnessRGB_WithGlobalApplied(), SEGMENT.getBrightnessCCT_WithGlobalApplied() );
  // desired_colour.debug_print("after brightness");

  RgbcctColor starting_colour = SEGMENT.GetPixelColor(0);
  
  SetTransitionColourBuffer_DesiredColour (SEGMENT.Data(), SEGMENT.DataLength(), 0, SEGMENT.colour_type, desired_colour); 
  SetTransitionColourBuffer_StartingColour(SEGMENT.Data(), SEGMENT.DataLength(), 0, SEGMENT.colour_type, starting_colour);

  #ifdef ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28
  ALOG_INF( PSTR("startin_colour = %d,%d,%d,%d,%d"), starting_colour.R,starting_colour.G,starting_colour.B,starting_colour.WC,starting_colour.WW);
  ALOG_INF( PSTR("desired_colour = %d,%d,%d,%d,%d"), desired_colour.R,desired_colour.G,desired_colour.B,desired_colour.WC,desired_colour.WW);
  #endif // ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28

  SetSegment_AnimFunctionCallback( SEGIDX, [this](const AnimationParam& param){ this->AnimationProcess_SingleColour_LinearBlend_Dynamic_Buffer(param); } );

}
static const char PM_EFFECT_CONFIG__SOLID_COLOUR[] PROGMEM = ",,,,,Time,Rate;!,!,!,!,!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Static Palette
 * @description:   : Palettes should be showed all as banded/descrete, regardless of type
 * 
 * @param intensity: None
 * @param speed    : None
 * @param rate     : None
 * @param time     : Blend time on first/only update
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
void mAnimatorLight::EffectAnim__Static_Palette()
{

  if (!SEGMENT.allocateData( GetSizeOfPixel(SEGMENT.colour_type) * 2 * SEGLEN )){ return; } // Pixel_Width * Two_Channels * Pixel_Count
    
  RgbcctColor colour = RgbcctColor(0);
  for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
  {
    colour = SEGMENT.GetPaletteColour(pixel, PALETTE_SPAN_OFF, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);
    colour = ApplyBrightnesstoDesiredColourWithGamma(colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type, colour);
  }

  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  SetSegment_AnimFunctionCallback( SEGIDX, [this](const AnimationParam& param){ this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); } );

}
static const char PM_EFFECT_CONFIG__STATIC_PALETTE[] PROGMEM = ",,,,,Time,Rate;!,!,!,!,!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Spanned Static Palette
 * @description:   : Palettes should all span the full segment, regardless of type
 * 
 * @param intensity: None
 * @param speed    : None
 * @param rate     : None
 * @param time     : Blend time on first/only update
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
void mAnimatorLight::EffectAnim__Spanned_Static_Palette()
{

  if (!SEGMENT.allocateData( GetSizeOfPixel(SEGMENT.colour_type) * 2 * SEGLEN )){ return; } // Pixel_Width * Two_Channels * Pixel_Count
    
  RgbcctColor colour = RgbcctColor(0);
  for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
  {    
    colour = SEGMENT.GetPaletteColour(pixel, PALETTE_SPAN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE);
    colour = ApplyBrightnesstoDesiredColourWithGamma(colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type, colour);
  }

  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  SetSegment_AnimFunctionCallback( SEGIDX, [this](const AnimationParam& param){ this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); } );

}
static const char PM_EFFECT_CONFIG__SPANNED_PALETTE[] PROGMEM = ",,,,,Time,Rate;!,!,!,!,!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Slow Glow 
 * @description:   : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param intensity: 0-255 is how many should pixels should randomly change (0-255 scaled to 0-pixel_count)
 * @param speed    : None
 * @param rate     : Period of time (ms) between updates
 * @param time     : Blend time
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
void mAnimatorLight::EffectAnim__Slow_Glow()
{

  if (!SEGMENT.allocateData( GetSizeOfPixel(SEGMENT.colour_type) * 2 * SEGMENT.virtualLength() )){ return; }
  
  /**
   * @brief Intensity describes the amount of pixels to update
   *  Intensity 0-255 ==> LED length 1 to length (since we cant have zero)
   **/
  uint16_t pixels_to_update = mapvalue(
                                        SEGMENT.intensity(), 
                                        0,255, 
                                        0,SEGMENT.virtualLength() // scaled over the virtual length
                                      );

  uint16_t pixels_in_map = GetNumberOfColoursInPalette(SEGMENT.palette.id);

  uint16_t pixel_index = 0;
  RgbcctColor colour;

  /**
   * @brief On first run, all leds need set once
   * */
  if(SEGMENT.flags.animator_first_run){ // replace random, so all pixels are placed on first run:: I may also want this as a flag only, as I may want to go from static_Effect to slow_glow with easy/slow transition
    pixels_to_update = SEGMENT.virtualLength(); // only the virtual length
  }


  for(uint16_t iter = 0; 
                iter < pixels_to_update; 
                iter++
  ){
    /**
     * @brief 
     * min: lower bound of the random value, inclusive.
     * max: upper bound of the random value, exclusive.
    **/
    pixel_index = random(0, SEGMENT.virtualLength()+1); // Indexing must be relative to buffer, virtual length only

    // On first run, I need a new way to set them all.
    if(SEGMENT.flags.animator_first_run){ // replace random, so all pixels are placed on first run:: I may also want this as a flag only, as I may want to go from static_Effect to slow_glow with easy/slow transition
      pixel_index = iter;
    }
    
    desired_pixel = random(0, pixels_in_map);
  
    colour = SEGMENT.GetPaletteColour(desired_pixel, PALETTE_SPAN_OFF, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);

    #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS
    ALOG_DBM(LOG_LEVEL_TEST, 
      PSTR("New palettePixel=%d, pixel_index=v%d SL%d | SVL%d | DL%d, colour=%d,%d,%dT%d"), 
        desired_pixel, pixel_index, 
        SEGMENT.length(), SEGMENT.virtualLength(), SEGMENT.DataLength(),
        colour.R, colour.G, colour.B,
        SEGMENT.colour_type
    ); 
    #endif // ENABLE__DEBUG_POINT__ANIMATION_EFFECTS

    colour = ApplyBrightnesstoDesiredColourWithGamma(colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());

    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel_index, SEGMENT.colour_type, colour);

  }

  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  SetSegment_AnimFunctionCallback( SEGIDX, [this](const AnimationParam& param){ this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); } );

}
static const char PM_EFFECT_CONFIG__SLOW_GLOW[] PROGMEM = ",,,,,Time,Rate;!,!,!,!,!;!"; // 7 sliders + 4 options before first ;
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Based on accidently effect
 * @description:   : Colours are added (with various methods) then fade off
 * @note           : 
 * 
 * Ideas:
 * - param to enable "instant" or popping in, or to have the introduction of new colours actually fade on before decaying away (hence, the fade direction may be needed? may be too complex)
 * - fade off method, to have all fade together (using intensity), intensity should be for how many to pop-in, speed for how quickly they fade away
 * - params could be used to signifying all fade together, or later, randomly pick fade rates. Though, fade together is probably visually nicer. Option would be good
 * 
 * 
 * @param intensity: Depth of variation from max/min brightness
 * @param speed    : How often it occurs
 * @param rate     : None
 * @param time     : None
 * @param aux0     : brightness saved
 * @param aux1     : target brightness
 * @param aux2     : (U) Second target palette
 * @param aux3     : Reserved for random palette refresh rate
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
/**
 * @description:   : Randomly redraw palette in the correct order, with fade always being calculated (though on next count so at least one step away)
 **/
void mAnimatorLight::EffectAnim__Popping_Decay_Palette()
{
  EffectAnim__Popping_Decay_Base(true);
}
static const char PM_EFFECT_CONFIG__POPPING_DECAY_PALETTE[] PROGMEM = ",,,,,Time,Rate;!,!,!,!,!;!"; // 7 sliders + 4 options before first ;
/**
 * @description:   : Randomly redraw random indexs, not ordered
 **/
void mAnimatorLight::EffectAnim__Popping_Decay_Random()
{
  EffectAnim__Popping_Decay_Base(false);
}
static const char PM_EFFECT_CONFIG__POPPING_DECAY_RANDOM[] PROGMEM = ",,,,,Time,Rate;!,!,!,!,!;!"; // 7 sliders + 4 options before first ;
/**
 * @description:   : Base function for flickering
 * */
void mAnimatorLight::EffectAnim__Popping_Decay_Base(bool draw_palette_inorder)
{

  ALOG_INF(PSTR("s=%d\t a0=%d\t a1=%d\t a2=%d"),    SEGMENT.step, SEGMENT.params_internal.aux0, SEGMENT.params_internal.aux1, SEGMENT.params_internal.aux2);

  uint8_t pixels_in_palette = GetNumberOfColoursInPalette(SEGMENT.palette.id);

  RgbcctColor colour_pri;
  RgbcctColor colour_sec;
  RgbcctColor colour_out;

  // if (draw_palette_inorder)
  // {
  //   uint16_t dataSize = (SEGLEN-1) *3;
  //   if(!SEGMENT.allocateData(dataSize) ){ return; }
  // }

  
  if (!SEGMENT.allocateData( GetSizeOfPixel(SEGMENT.colour_type) * 2 * SEGMENT.virtualLength() )){ return; } // Pixel_Width * Two_Channels * Pixel_Count
    
  RgbcctColor colour = RgbcctColor(0);



  //max. flicker range controlled by intensity()
  uint8_t valrange = SEGMENT.intensity();
  uint8_t rndval = valrange >> 1;

  #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS
  ALOG_DBG(PSTR("step=%d"),    SEGMENT.step);
  ALOG_DBG(PSTR("valrange=%d"),valrange);
  ALOG_DBG(PSTR("rndval=%d"),  rndval);
  #endif

  uint8_t pixel_palette_counter = 0;

  //step (how much to move closer to target per frame) coarsely set by speed()
  uint8_t speedFactor = 4;

    uint16_t i = 0;

    #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS
    ALOG_DBG(PSTR("i=%d|%d"),i,numCandles);
    #endif

    uint16_t d = 0; //data location

    uint8_t s        = SEGMENT.params_internal.aux0, 
            s_target = SEGMENT.params_internal.aux1, 
            fadeStep = SEGMENT.step;

    // if (i > 0) {
    //   d = (i-1) *3;
    //   s = SEGMENT.data[d]; 
    //   s_target = SEGMENT.data[d+1]; 
    //   fadeStep = SEGMENT.data[d+2];
    // }

    // if (fadeStep == 0) { //init vals
    //   s = 128; s_target = 130 + random8(4); fadeStep = 1;
    // }

    // bool newTarget = false;
    // if (s_target > s) { //fade up

    //   #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS
    //   ALOG_DBG(PSTR("fade up s_target > s %d=%d"), s_target, s);
    //   #endif

    //   s = qadd8(s, fadeStep);
    //   if (s >= s_target) newTarget = true;
    // } else {
    //   s = qsub8(s, fadeStep);
    //   if (s <= s_target) newTarget = true;
          
    //   #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS
    //   ALOG_DBG(PSTR("fade down=%d"),s);
    //   #endif

    // }

    // if (newTarget) {
    //   s_target = random8(rndval) + random8(rndval);
    //   if (s_target < (rndval >> 1)) s_target = (rndval >> 1) + random8(rndval);
    //   uint8_t offset = (255 - valrange) >> 1;
    //   s_target += offset;

    //   uint8_t dif = (s_target > s) ? s_target - s : s - s_target;
    
    //   fadeStep = dif >> speedFactor;
    //   if (fadeStep == 0) fadeStep = 1;
    // }

    /**
     * @brief aux2
     * 
     */

    bool redraw_all_pixels = false;
    bool draw_random_pixels = false;
    bool do_fade = false;

    uint16_t* drawmode = &SEGMENT.params_internal.aux2;
    // uint16_t* drawmode = &SEGMENT.params_internal.aux3;

// Redraw and fade should happen on alternate cycles

// Buffer mode not going to work, since I need this effect to be called more often than the fade amount
// Another counter should therefore only update on certain intervals



    /*
    @ redraw when aux2 is zero
    */
    if(*drawmode == 0)
    {
      redraw_all_pixels = false;
      do_fade = false;
      *drawmode = *drawmode + 1;
    }else
    if(*drawmode>10) // Reset
    {
      *drawmode = 0;
      do_fade = false;
      *drawmode = *drawmode + 1;
    }else{
      // do_fade = true;
      // draw_random_pixels = true;

      if(*drawmode % 2)
      {
        do_fade = true;
      }else{
        draw_random_pixels = true;
      }

      *drawmode = *drawmode + 1;

    }

    /**
     * @brief ReDraw all (either as option, or first run of animation)
     * 
     */
    if(redraw_all_pixels)
    {

      ALOG_INF(PSTR("redraw_all_pixels"));

      pixel_palette_counter = 0;      
      for(uint16_t pixel = 0; pixel < SEGMENT.virtualLength(); pixel++)
      {
        if(draw_palette_inorder)
        {
          // Cycle through palette index pal_i
          if(pixel_palette_counter++ >= pixels_in_palette-1){ pixel_palette_counter = 0; }
        }
        else
        {
          pixel_palette_counter = random(0, pixels_in_palette-1); // Randon colour from palette
        }

        colour = SEGMENT.GetPaletteColour(pixel_palette_counter);      
        colour = ApplyBrightnesstoDesiredColourWithGamma(colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());
        SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type, colour);
      }

    }


    /**
     * @brief ReDraw all (either as option, or first run of animation)
     * 
     * needs revamped, so that:
     * ** picks random index in range, and uses that to either show the original (palette static) or selects random colour
    * ** the intensity gives how many random colour should be updated in SEGLEN, 
     */
    if(draw_random_pixels)
    {

      ALOG_INF(PSTR("draw_random_pixels"));

      uint16_t redraw_count = map(random(0,SEGMENT.intensity()), 0,255, 0,SEGLEN);
      uint16_t random_pixel_to_update = 0;

      
      ALOG_INF(PSTR("redraw_count %d"), redraw_count);



      pixel_palette_counter = 0;      
      for(uint16_t pixel = 0; pixel < redraw_count; pixel++)
      {
        if(draw_palette_inorder)
        {
          pixel_palette_counter = pixel;   //pixel from palette
          random_pixel_to_update = pixel; //pixel into output, these should be the same when in order
        }
        else
        {
          pixel_palette_counter = random(0, pixels_in_palette-1); // Randon colour from palette
          random_pixel_to_update = map(random(0,SEGMENT.intensity()), 0,255, 0,SEGLEN); //repick a random pixel to change within segment
        }

        colour = SEGMENT.GetPaletteColour(pixel_palette_counter);      
        colour = ApplyBrightnesstoDesiredColourWithGamma(colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());
        SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), random_pixel_to_update, SEGMENT.colour_type, colour);
        do_fade = false ; // block fade on update cycle, since fade relies on getting from the commited neopixel bus
      }

    }

    /**
     * @brief Decay test
     * 
     */
    if(do_fade)
    {
      
      ALOG_INF(PSTR("do_fade"));

      for(uint16_t pixel = 0; pixel < SEGMENT.virtualLength(); pixel++)
      {
        colour_out = SEGMENT.GetPixelColor(pixel);
        colour_out.Fade(2);// = ApplyBrightnesstoRgbcctColour(colour_out, pCONT_iLight->getBriRGB_Global());       
        SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type, colour_out);
      }
    }


    SEGMENT.params_internal.aux0 = s; 
    SEGMENT.params_internal.aux1 = s_target; 
    SEGMENT.step = fadeStep;

  
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  SetSegment_AnimFunctionCallback( SEGIDX, [this](const AnimationParam& param){ this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); } );



  // SetSegment_AnimFunctionCallback_WithoutAnimator(SEGIDX);  

}
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Candle Flicker Base Function
 * @description:   : Randomly changes colours of pixels, and blends to the new one
 *                   Values close to 100 (speed) produce 5Hz flicker, which looks very candle-y
 *                   Inspired by https://github.com/avanhanegem/ArduinoCandleEffectNeoPixel
 *                   and https://cpldcpu.wordpress.com/2016/01/05/reverse-engineering-a-real-cand
 * @note           : derived from WLED effects
 * 
 * @param intensity: Depth of variation from max/min brightness
 * @param speed    : How often it occurs
 * @param rate     : None
 * @param time     : None
 * @param aux0     : brightness saved
 * @param aux1     : target brightness
 * @param aux2     : (U) Second target palette
 * @param aux3     : Reserved for random palette refresh rate
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
/**
 * @description:   : Flickers pixels by the same amount towards black
 **/
void mAnimatorLight::EffectAnim__Candle_Single()
{
  EffectAnim__Flicker_Base(false, mPalette::PALETTELIST_FIXED_SINGLE_COLOUR__BLACK__ID);
}
static const char PM_EFFECT_CONFIG__CANDLE_SINGLE[] PROGMEM = ",,,,,Time,Rate;!,!,!,!,!;!"; // 7 sliders + 4 options before first ;
/**
 * @description:   : Flickers by multiple levels towards black
 **/
void mAnimatorLight::EffectAnim__Candle_Multiple()
{
  EffectAnim__Flicker_Base(true,  mPalette::PALETTELIST_FIXED_SINGLE_COLOUR__BLACK__ID);
}
static const char PM_EFFECT_CONFIG__CANDLE_MULTIPLE[] PROGMEM = "Speed,Intensity,,,,Time,Rate;!,!,!,!,!;!"; // 7 sliders + 4 options before first ;
/**
 * @description:   : Flickers by multiple levels towards black
 **/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
void mAnimatorLight::EffectAnim__Shimmering_Palette_Saturation()
{
  EffectAnim__Flicker_Base(true,  mPalette::PALETTELIST_FIXED_SINGLE_COLOUR__COLDWHITE__ID);
}
static const char PM_EFFECT_CONFIG__SHIMMERING_PALETTE_SATURATION[] PROGMEM = ",,,,,Time,Rate;!,!,!,!,!;!"; // 7 sliders + 4 options before first ;
#endif
/**
 * @description:   : Flickers by multiple levels towards black
 **/
void mAnimatorLight::EffectAnim__Shimmering_Palette() // Same as Candle_Multiple
{
  EffectAnim__Flicker_Base(true,  mPalette::PALETTELIST_FIXED_SINGLE_COLOUR__BLACK__ID);
}
static const char PM_EFFECT_CONFIG__SHIMMERING_PALETTE[] PROGMEM = ",,,,,Time,Rate;!,!,!,!,!;!"; // 7 sliders + 4 options before first ;
/**
 * @description:   : Flicker between primary and secondary palette
 * 
     * Desc: Animation, that fades from selected palette to anothor palette,
     *       The intensity of fade (closer to palette B) will depend on intensity value
     *       ie intensity of 255 means Palette A (primary) can fade into palette B (set by option)
     * 
     *       New method to set options
     *        option8  for 255 value range.... ie allows animations to be configured and saved in their "aux0"
     *        option16 for 65000 value range
 * */
void mAnimatorLight::EffectAnim__Shimmering_Two_Palette() // Also add another here (or really segcolour is also it) to flicker into a second palette!! this will require direct load of second palette
{
  EffectAnim__Flicker_Base(true, SEGMENT.params_user.val0);
}
static const char PM_EFFECT_CONFIG__SHIMMERING_TWO_PALETTES[] PROGMEM = ",,,,,Time,Rate;!,!,!,!,!;!"; // 7 sliders + 4 options before first ;
/**
 * @description:   : Base function for flickering
 * */
void mAnimatorLight::EffectAnim__Flicker_Base(bool use_multi, uint16_t flicker_palette_id )
{

  uint8_t pixels_in_palette = GetNumberOfColoursInPalette(SEGMENT.palette.id);

  RgbcctColor colour_pri;
  RgbcctColor colour_sec;
  RgbcctColor colour_out;

  if (use_multi)
  {
    uint16_t dataSize = (SEGLEN -1) *3;
    if(!SEGMENT.allocateData(dataSize) ){ return; }
  }

  //max. flicker range controlled by intensity()
  uint8_t valrange = SEGMENT.intensity();
  uint8_t rndval = valrange >> 1;

  #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS
  ALOG_DBG(PSTR("step=%d"),    SEGMENT.step);
  ALOG_DBG(PSTR("valrange=%d"),valrange);
  ALOG_DBG(PSTR("rndval=%d"),  rndval);
  #endif

  uint8_t pixel_palette_counter = 0;

  //step (how much to move closer to target per frame) coarsely set by speed()
  uint8_t speedFactor = 4;
  if (SEGMENT.speed() > 252) { // epilepsy
    speedFactor = 1;
  } else 
  if (SEGMENT.speed() > 99) { // regular candle (mode called every ~25 ms, so 4 frames to have a new target every 100ms)
    speedFactor = 2;
  } else 
  if (SEGMENT.speed() > 49) { // slower fade
    speedFactor = 3;
  } //else 4 (slowest)

  uint16_t numCandles = (use_multi) ? SEGLEN : 1;
  #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS
  ALOG_DBG(PSTR("numCandles=%d"), numCandles);
  #endif

  for (uint16_t i = 0; i < numCandles; i++)
  {
    #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS
    ALOG_DBG(PSTR("i=%d|%d"),i,numCandles);
    #endif

    uint16_t d = 0; //data location

    uint8_t s        = SEGMENT.params_internal.aux0, 
            s_target = SEGMENT.params_internal.aux1, 
            fadeStep = SEGMENT.step;

    if (i > 0) {
      d = (i-1) *3;
      s = SEGMENT.data[d]; 
      s_target = SEGMENT.data[d+1]; 
      fadeStep = SEGMENT.data[d+2];
    }
    if (fadeStep == 0) { //init vals
      s = 128; s_target = 130 + random8(4); fadeStep = 1;
    }

    bool newTarget = false;
    if (s_target > s) { //fade up

      #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS
      ALOG_DBG(PSTR("fade up s_target > s %d=%d"), s_target, s);
      #endif

      s = qadd8(s, fadeStep);
      if (s >= s_target) newTarget = true;
    } else {
      s = qsub8(s, fadeStep);
      if (s <= s_target) newTarget = true;
          
      #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS
      ALOG_DBG(PSTR("fade down=%d"),s);
      #endif

    }

    if (newTarget) {
      s_target = random8(rndval) + random8(rndval);
      if (s_target < (rndval >> 1)) s_target = (rndval >> 1) + random8(rndval);
      uint8_t offset = (255 - valrange) >> 1;
      s_target += offset;

      uint8_t dif = (s_target > s) ? s_target - s : s - s_target;
    
      fadeStep = dif >> speedFactor;
      if (fadeStep == 0) fadeStep = 1;
    }

    // flicker_palette_id = mPalette::PALETTELIST_HTML_COLOUR__White__ID;

    /**
     * Apply colour to output: different per pixel
     **/
    if(i > 0) 
    {
      colour_pri = SEGMENT.GetPaletteColour(pixel_palette_counter);    
      // colour_pri = SEGMENT.GetPaletteColour(pixel_palette_counter, PALETTE_SPAN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE);

      colour_sec = GetColourFromUnloadedPalette2(flicker_palette_id);
      
      colour_out = ColourBlend(colour_pri, colour_sec, s); // s = flicker level (i.e. brightness)

      if(pixel_palette_counter++ >= pixels_in_palette-1)
      {
        pixel_palette_counter = 0;
      }

      SEGMENT.SetPixelColor(SEGMENT.pixel_range.start + i, colour_out);

      SEGMENT.data[d  ] = s; 
      SEGMENT.data[d+1] = s_target; 
      SEGMENT.data[d+2] = fadeStep;

    } 
    /**
     * Single mode, one colour applied across all leds??????????
     * */
    else
    {
      
      for(uint16_t p = SEGMENT.pixel_range.start;
                 p <= SEGMENT.pixel_range.stop;
                 p++
      ){

        colour_pri = SEGMENT.GetPaletteColour(pixel_palette_counter); 
        // colour_pri = SEGMENT.GetPaletteColour(pixel_palette_counter, PALETTE_SPAN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE);

        colour_sec = GetColourFromUnloadedPalette2(flicker_palette_id);

        colour_out = ColourBlend(colour_pri, colour_sec, s); // s = flicker level (i.e. brightness)

        if(pixel_palette_counter++ >= pixels_in_palette-1)
        {
          pixel_palette_counter = 0;
        }

        SEGMENT.SetPixelColor(p, colour_out);

      }

      SEGMENT.params_internal.aux0 = s; 
      SEGMENT.params_internal.aux1 = s_target; 
      SEGMENT.step = fadeStep;

    }
  }

  SetSegment_AnimFunctionCallback_WithoutAnimator(SEGIDX);  

}
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Static Gradient Palette
 * @note : Palette will be blended across total length of segment
 *         If Palette contains gradient information, these will be used as inflection points, otherwise, span with equal spacing
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 * line 2583
 * 
 * delete, replaced by spanned palette
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
void mAnimatorLight::EffectAnim__Static_Gradient_Palette()
{
  uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type) * 2 * SEGMENT.virtualLength();

    ALOG_ERR( "delete, replaced by spanned palette" );
  if (!SEGMENT.allocateData(dataSize)){    
    ALOG_ERR( PM_JSON_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = EFFECTS_FUNCTION__SOLID_COLOUR__ID;
    return;
  }
  
  uint16_t start_pixel = 0;
  uint16_t end_pixel = 100;
  RgbcctColor start_colour = RgbcctColor(0);
  RgbcctColor end_colour = RgbcctColor(0);
  RgbcctColor out_colour = RgbcctColor(0);
  uint8_t start_pixel_position = 255, end_pixel_position = 255;

  uint16_t pixels_in_map = GetNumberOfColoursInPalette(SEGMENT.palette.id);


  /**
   * @brief Move across by pixels in map, and then either use encoded value or just use pixel in map 
   * 
   */
    for(uint8_t grad_pair_index=0;
                grad_pair_index<pixels_in_map;
                grad_pair_index++
    ){

      uint8_t desired_index_upper = 0;
      if(grad_pair_index<pixels_in_map-1) // ie not last one
      {
        desired_index_upper = grad_pair_index+1; // use next
      }else{
        desired_index_upper = 0; //assume its the first and wrap back
      }
      
      // start_colour = mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette.id, grad_pair_index,   &start_pixel_position);      
      start_colour = SEGMENT.GetPaletteColour(grad_pair_index, PALETTE_SPAN_OFF, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, &start_pixel_position);      

      // end_colour   = mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette.id, desired_index_upper, &end_pixel_position);
      end_colour = SEGMENT.GetPaletteColour(desired_index_upper, PALETTE_SPAN_OFF, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, &end_pixel_position);

      // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "grad_pair_index %d|%d  %d|%d"),grad_pair_index,pixels_in_map, grad_pair_index,desired_index_upper);
      // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "%02d start_pixel_position %d"),grad_pair_index,start_pixel_position);
      // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "     end_pixel_position %d"),end_pixel_position);

      /**
       * @brief No start/stop mapping with segments I think
       *  */
      uint16_t start_pixel = 0;
      uint16_t stop_pixel  = SEGLEN;
      
      mPalette::STATIC_PALETTE* ptr = &mPaletteI->static_palettes[SEGMENT.palette.id];
      
      if(ptr->encoding.index_scaled_to_segment)
      {
        start_pixel_position = map(start_pixel_position, 0,255, start_pixel,stop_pixel);
        end_pixel_position   = map(end_pixel_position,   0,255, start_pixel,stop_pixel);
      }
      else
      { //
        start_pixel_position = map(grad_pair_index, 0,pixels_in_map, start_pixel,stop_pixel);
        end_pixel_position   = map(grad_pair_index+1, 0,pixels_in_map, start_pixel,stop_pixel);
      }

      // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "%d start_pixel_position %d"),grad_pair_index,start_pixel_position);
      // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "end_pixel_position %d"),end_pixel_position);


      float progress = 0;
      for(
        int 
        index=start_pixel_position;
        index<=end_pixel_position;
        index++){
        
          progress = mSupport::mapfloat(index,start_pixel_position,end_pixel_position,0,1);
          
          out_colour = RgbcctColor::LinearBlend(start_colour, end_colour, progress);

          out_colour = ApplyBrightnesstoDesiredColourWithGamma(out_colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());
     
          SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), index, SEGMENT.colour_type, out_colour);
      }

  }

  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // Call the animator to blend from previous to new
  SetSegment_AnimFunctionCallback(  SEGIDX, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    }
  );

}
static const char PM_EFFECT_CONFIG__STATIC_GRADIENT_PALETTE[] PROGMEM = ",,,,,Time,Rate;!,!,!,!,!;!"; // 7 sliders + 4 options before first ;
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Rotating Palette
 * @note : Moves 1D animation along 
 * 
 * @param aux0: direction
 * @param aux1: saved start position that is moved with each rotation
 * @param aux2 
 * @param intensity: 
 *        ** 0: No new colours will be redrawn. Previous state will be moved
 *        ** 1: Redraw palette EACH time, since randomised palettes may be updating in the background. Keep record of "starting_pixel_index" in aux1
 *        ** 2-255: With blending enabled, how large to rotate/step across the segment. 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
void mAnimatorLight::EffectAnim__Rotating_Palette_New()
{

  uint16_t* region_p             = &SEGMENT.params_internal.aux0;
  uint16_t* movement_direction_p = &SEGMENT.params_internal.aux1;
  
  switch(*region_p){
    case EFFECTS_REGION_COLOUR_SELECT_ID: //set colours
    {
      
      ALOG_DBM(PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
           
      ALOG_DBM( PSTR("Segment: %d\t(%d,%d),(%d)"), SEGIDX, SEGMENT.pixel_range.start, SEGMENT.pixel_range.stop, SEGMENT.palette.id);

      RgbcctColor colour = RgbcctColor(0);
      for (uint16_t pixel = 0; pixel < SEGLEN; pixel++)
      {

        colour = SEGMENT.GetPaletteColour(pixel); //mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette.id, pixel);
        
        colour = ApplyBrightnesstoDesiredColourWithGamma(colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());
        
        SEGMENT.SetPixelColor(pixel, colour, false);

        #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
        ALOG_INF( PSTR("sIndexIO=%d %d,%d\t%d,pC %d, R%d"), SEGIDX, SEGMENT.pixel_range.start, SEGMENT.pixel_range.stop, pixel, GetNumberOfColoursInPalette(mPaletteI->static_palettes.ptr), colour.R );
        #endif

      }
      
      *region_p = EFFECTS_REGION_ANIMATE_ID;
    }
    break; //not into next right away
    case EFFECTS_REGION_ANIMATE_ID: //shift along
    {

      ALOG_DBM(PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_REGION_ANIMATE_ID"));

// #define SEG_STOP_INDEX   strip->_virtualSegmentLength-1  //prob wrong now!
//       if(*movement_direction_p==1)
//       { // direction==1 move right ie AWAY from start
//         /**
//          * @brief Save first, move pixels towards last, assert last
//          **/
//         RgbcctColor colourlast = SEGMENT.GetPixelColor(SEG_STOP_INDEX); 
//         for(int32_t p = SEG_STOP_INDEX-1; p >= 0; p--){ //must be signed
//           SEGMENT.SetPixelColor(p, SEGMENT.GetPixelColor(p-1));
//         }
//         SEGMENT.SetPixelColor(0, colourlast);
//       }
//       else
//       {
//         /**
//          * @brief Save last, move pixels back to first, assert first
//          **/
//         RgbcctColor colourfirst = SEGMENT.GetPixelColor(0); 
//         for(uint16_t p = 0; p <= SEG_STOP_INDEX; p++){ 
//           SEGMENT.SetPixelColor(p, SEGMENT.GetPixelColor(p+1));
//         }
//         SEGMENT.SetPixelColor(SEG_STOP_INDEX, colourfirst);
//       }

    }
    break;
  }

  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();

}
static const char PM_EFFECT_CONFIG__ROTATING_PALETTE_NEW[] PROGMEM = ",,,,,Time,Rate;!,!,!,!,!;!"; // 7 sliders + 4 options before first ;
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC


#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC

    /**
     * Desc: pixels are rotated
     * Para: direction of motion, speed, instant or blend change
     * 
     * draw static palette, then use neopixel to rotate with animator, no need for dynamic animationpair
     * */

void mAnimatorLight::EffectAnim__Rotating_Palette()
{

  uint16_t* region_p             = &SEGMENT.params_internal.aux0;
  uint16_t* movement_direction_p = &SEGMENT.params_internal.aux1;
  
  switch(*region_p){
    case EFFECTS_REGION_COLOUR_SELECT_ID: //set colours
    {
      
      ALOG_DBM(PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
           
      ALOG_DBM( PSTR("Segment: %d\t(%d,%d),(%d)"), SEGIDX, SEGMENT.pixel_range.start, SEGMENT.pixel_range.stop, SEGMENT.palette.id);

      RgbcctColor colour = RgbcctColor(0);
      for (uint16_t pixel = 0; pixel < SEGLEN; pixel++)
      {

        // colour = mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette.id, pixel);
        colour = SEGMENT.GetPaletteColour(pixel);
        
        colour = ApplyBrightnesstoDesiredColourWithGamma(colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());
        
        SEGMENT.SetPixelColor(pixel, colour, false);

        #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
        ALOG_INF( PSTR("sIndexIO=%d %d,%d\t%d,pC %d, R%d"), SEGIDX, SEGMENT.pixel_range.start, SEGMENT.pixel_range.stop, pixel, GetNumberOfColoursInPalette(mPaletteI->static_palettes.ptr), colour.R );
        #endif

      }
      
      *region_p = EFFECTS_REGION_ANIMATE_ID;
    }
    break; //not into next right away
    case EFFECTS_REGION_ANIMATE_ID: //shift along
    {

      ALOG_DBM(PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_REGION_ANIMATE_ID"));

// #define SEG_STOP_INDEX   strip->_virtualSegmentLength-1  //prob wrong now!
//       if(*movement_direction_p==1)
//       { // direction==1 move right ie AWAY from start
//         /**
//          * @brief Save first, move pixels towards last, assert last
//          **/
//         RgbcctColor colourlast = SEGMENT.GetPixelColor(SEG_STOP_INDEX); 
//         for(int32_t p = SEG_STOP_INDEX-1; p >= 0; p--){ //must be signed
//           SEGMENT.SetPixelColor(p, SEGMENT.GetPixelColor(p-1));
//         }
//         SEGMENT.SetPixelColor(0, colourlast);
//       }
//       else
//       {
//         /**
//          * @brief Save last, move pixels back to first, assert first
//          **/
//         RgbcctColor colourfirst = SEGMENT.GetPixelColor(0); 
//         for(uint16_t p = 0; p <= SEG_STOP_INDEX; p++){ 
//           SEGMENT.SetPixelColor(p, SEGMENT.GetPixelColor(p+1));
//         }
//         SEGMENT.SetPixelColor(SEG_STOP_INDEX, colourfirst);
//       }

    }
    break;
  }

  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();

}
static const char PM_EFFECT_CONFIG__ROTATING_PALETTE[] PROGMEM = ",,,,,Time,Rate;!,!,!,!,!;!"; // 7 sliders + 4 options before first ;
#endif


#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC

    /**
     * Desc: pixels are rotated
     * Para: direction of motion, speed, instant or blend change
     * 
     * draw static palette, then use neopixel to rotate with animator, no need for dynamic animationpair
     * */

void mAnimatorLight::EffectAnim__Rotating_Previous_Animation()
{

  // ALOG_INF(PSTR("EffectAnim__Rotating_Previous_Animation"));

  uint16_t* movement_direction_p = &SEGMENT.params_internal.aux0;  

  // if(*movement_direction_p==1)
  // { // direction==1 move right ie AWAY from start
  //   /**
  //    * @brief Save first, move pixels towards last, assert last
  //    **/
  //   RgbcctColor colourlast = SEGMENT.GetPixelColor(SEG_STOP_INDEX); 
  //   for(int32_t p = SEG_STOP_INDEX-1; p >= 0; p--){ //must be signed
  //     SEGMENT.SetPixelColor(p, SEGMENT.GetPixelColor(p-1));
  //   }
  //   SEGMENT.SetPixelColor(0, colourlast);
  // }
  // else
  // {
    /**
     * @brief Save last, move pixels back to first, assert first
     **/
    RgbcctColor colourfirst = SEGMENT.GetPixelColor(0); 
    for(uint16_t p = 0; p < SEGLEN-1; p++) //move them all ONCE towards first pixel
    { 
      SEGMENT.SetPixelColor(p, SEGMENT.GetPixelColor(p+1));
    }
    SEGMENT.SetPixelColor(SEGLEN-1, colourfirst); // Insert saved first pixel into last pixel as "wrap around"
  // }

  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR(); // Change this to be function that sets transition up

}
static const char PM_EFFECT_CONFIG__ROTATING_PREVIOUS_ANIMATION[] PROGMEM = ",,,,,Time,Rate;!,!,!,!,!;!"; // 7 sliders + 4 options before first ;
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Step_Through_Palette
 * @note : Picks sequential colours from palette and steps through them ie Red, Green, Blue, Orange  would be (R,G) (B,G) (B,O) with the next palette colour in order, keeping the same colour for two steps 
 *  
 * Future Change: Make it so more than two can be shown, ie if 5 colours exist, then have "Intensity" (as percentage) select how many colours to remain visible
 * 
     * Desc: Show an exact amount of pixels only from a palette, where "show_length" would be pixel=0:pixel_length
     *       Stepping through them with a count, ie pixel 0/1 then 1/2 then 2/3, first pixel overwrite
     * Para: Amount of pixels to show from palette as it steps through (eg 2, 3 etc)
     * TODO: Add size of step as percentage ie to show 50% of 4 colours would be 2 of 4, 75% of 4 is 3
     * 
     * Note: allocate_buffer is used as transition data
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
void mAnimatorLight::EffectAnim__Stepping_Palette()
{

  uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type) * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  if (!SEGMENT.allocateData(dataSize)){    
    ALOG_ERR( PM_JSON_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID;
    return;
  }

  uint16_t* region_p          = &SEGMENT.params_internal.aux0;
  uint16_t* indexes_active_p  = &SEGMENT.params_internal.aux1; // shared_flasher_parameters_segments.indexes.active
  uint16_t* indexes_counter_p = &SEGMENT.params_internal.aux2; // shared_flasher_parameters_segments.indexes.counter

  desired_pixel=0;
    
  uint8_t pixel_position = 0;
  uint8_t pixels_in_map = GetNumberOfColoursInPalette(SEGMENT.palette.id);

  // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "pixels_in_map= %d"),pixels_in_map);
  
  desired_pixel = *indexes_active_p;
  uint8_t pixels_map_upper_limit = *indexes_active_p+1;
  uint8_t pixels_map_lower_limit = *indexes_active_p;

  uint8_t index_1, index_2;
  uint8_t counter = 0;
      
  //if last pixel, then include it and the first, else include it and the next
  if(*indexes_active_p == pixels_in_map-1){ //wrap wround
    index_1 = 0;
    index_2 = *indexes_active_p;
    counter = 0;
  }else{
    index_1 = *indexes_active_p;
    index_2 = *indexes_active_p+1;
    counter = 1;

  }

  *indexes_counter_p ^= 1;

  AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "counter = %d/%d/%d"), counter,index_1,index_2);

  RgbcctColor colour;

  for(uint16_t index=SEGMENT.pixel_range.start;
                index<=SEGMENT.pixel_range.stop;
                index++
  ){

    if(counter^=1){
      desired_pixel = *indexes_counter_p ? index_2 : index_1;
    }else{
      desired_pixel = *indexes_counter_p ? index_1 : index_2;
    }
    
    colour = SEGMENT.GetPaletteColour(desired_pixel, PALETTE_SPAN_OFF, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, &pixel_position); // mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette.id, desired_pixel, &pixel_position);
    
    colour = ApplyBrightnesstoRgbcctColour(colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());

    SetTransitionColourBuffer_DesiredColour(SEGMENT.data, SEGMENT.DataLength(), index, SEGMENT.colour_type, colour);
        
  } 

//messy
  if(++*indexes_active_p>pixels_in_map-1){
    *indexes_active_p=0;
  }
  
  
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();
  
  SetSegment_AnimFunctionCallback(  SEGIDX, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    }
  );

}
static const char PM_EFFECT_CONFIG__STEPPING_PALETTE[] PROGMEM = ",,,,,Time,Rate;!,!,!,!,!;!"; // 7 sliders + 4 options before first ;
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Blend Palette to White
 * @note : Using a static palette (in order), the effect will change from static_palette (100% saturation) to cold_white (0% saturation)
 *         The "0%", ie least saturated, will be set by the intensity value scaled from 255 to 100 range
 * 
 * @param  "Intensity" :: 0-255 value is mapped into 0-100 and controls the minimum saturation. 100% Intensity means it can fade to complete white
 * @param  "Time"      :: If unset (ie set to 0), then the speed will assume the DEFAULT_MAXIMUM_ANIMATION_SPEED_PERIOD as 10 seconds
 *         "Speed"     :: If the time is set to zero, then the Speed will scale from sweep of saturation time between DEFAULT_MINIMUM_ANIMATION_SPEED_PERIOD (1 second) and DEFAULT_MAXIMUM_ANIMATION_SPEED_PERIOD (10 second)
 *          
     * Desc: Spread palette across segment
     * If gradient value exists, then use it to spread colour across segment
     * If no index in palette, then spread palette equal distant across palette
     * */
    // #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    // EFFECTS_FUNCTION__STATIC_PALETTE_GRADIENT__ID,
    // #endif
    /**
     * Desc: Animation, that fades from selected palette to white,
     *       The intensity of fade (closer to white, ie loss of saturation) will depend on intensity value
     * Para: 
     * TODO: 
     * 
     * Note: allocate_buffer is used as transition data
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
void mAnimatorLight::EffectAnim__Palette_Colour_Fade_Saturation()
{

  uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type) * 2 * SEGMENT.length();

  if (!SEGMENT.allocateData(dataSize)){    
    ALOG_ERR( PM_JSON_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = EFFECTS_FUNCTION__SOLID_COLOUR__ID;
    return;
  }
  
  /**
   * @brief Using 
   * 
   */

  // if(aux0 < 1000)
  // {

  // }
  

  uint8_t saturation_255 = 200;
  float   saturation     = saturation_255/255.0f;
  HsbColor colour_hsb = HsbColor(RgbcctColor(0));
  
  RgbcctColor colour = RgbcctColor(0);
  for(uint16_t pixel = 0; 
               pixel < SEGLEN; 
               pixel++
  ){

    colour = SEGMENT.GetPaletteColour(pixel); // mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette.id, pixel);

    colour_hsb = RgbColor(colour); // to HSB
    colour_hsb.S = saturation;
    colour = colour_hsb;
    
    colour = ApplyBrightnesstoDesiredColourWithGamma(colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());

    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type, colour);

    #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    ALOG_INF( PSTR("sIndexIO=%d %d,%d\t%d,pC %d, R%d"), SEGIDX, SEGMENT.pixel_range.start, SEGMENT.pixel_range.stop, pixel, GetNumberOfColoursInPalette(mPaletteI->static_palettes.ptr), colour.R );
    #endif

  }
  
    //SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), 0, SEGMENT.colour_type, RgbColor(255));


  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // Call the animator to blend from previous to new
  SetSegment_AnimFunctionCallback(  SEGIDX, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    }
  );

}
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Blend Palette Between Another Palette
 * @note : Using a static palette (in order), the effect will change from static_palette (palette.id) to a different palette (palette_id saved as aux0)
 *         The "0%", ie least saturated, will be set by the intensity value scaled from 255 to 100 range
 * 
 * @param  "Intensity" :: 0-255 value is mapped into 0-100 and controls the amount to blend into the second one. ie 0%=palette_id and 100%=aux0_palette
 * @param  "Time"      :: If unset (ie set to 0), then the speed will assume the DEFAULT_MAXIMUM_ANIMATION_SPEED_PERIOD as 10 seconds
 *         "Speed"     :: If the time is set to zero, then the Speed will scale from sweep of saturation time between DEFAULT_MINIMUM_ANIMATION_SPEED_PERIOD (1 second) and DEFAULT_MAXIMUM_ANIMATION_SPEED_PERIOD (10 second)
 *          
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
void mAnimatorLight::EffectAnim__Blend_Two_Palettes()
{

  uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type) * 2 * SEGMENT.length(); //allocate space for 10 test pixels
  //AddLog(LOG_LEVEL_TEST, PSTR("dataSize = %d"), dataSize);

  if (!SEGMENT.allocateData(dataSize)){    
    ALOG_ERR( PM_JSON_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID;
    return;
  }
    
  /**
   * @brief Intensity describes the amount of pixels to update
   *  Intensity 0-255 ==> LED length 1 to length (since we cant have zero)
   **/
  uint16_t pixels_to_update = mapvalue(
                                        SEGMENT.intensity(), 
                                        0,255, 
                                        0,SEGMENT.length()
                                      );

  uint16_t pixels_in_map = GetNumberOfColoursInPalette(SEGMENT.palette.id);

  uint16_t pixel_index = 0;
  RgbcctColor colour = RgbcctColor(0);

  /**
   * @brief On first run, all leds need set once
   * 
   */
  if(SEGMENT.flags.animator_first_run){ // replace random, so all pixels are placed on first run:: I may also want this as a flag only, as I may want to go from static_Effect to slow_glow with easy/slow transition
    pixels_to_update = SEGMENT.length();
  }


  for(uint16_t iter = 0; 
                iter < pixels_to_update; 
                iter++
  ){
    /**
     * @brief 
     * min: lower bound of the random value, inclusive (optional).
     * max: upper bound of the random value, exclusive.
    **/
    pixel_index = random(0, SEGMENT.length()+1); // Indexing must be relative to buffer

    // On first run, I need a new way to set them all.
    if(SEGMENT.flags.animator_first_run){ // replace random, so all pixels are placed on first run:: I may also want this as a flag only, as I may want to go from static_Effect to slow_glow with easy/slow transition
      pixel_index = iter;
    }
    
    desired_pixel = random(0, pixels_in_map);
  
    colour = SEGMENT.GetPaletteColour(desired_pixel); // mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette.id, desired_pixel);

    #ifdef DEBUG_TRACE_ANIMATOR_SEGMENTS
    AddLog(LOG_LEVEL_TEST, PSTR("desiredpixel%d, colour=%d,%d,%d"), desired_pixel, colour.R, colour.G, colour.B); 
    #endif // DEBUG_TRACE_ANIMATOR_SEGMENTS

    colour = ApplyBrightnesstoDesiredColourWithGamma(colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());
    
    // 2us
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel_index, SEGMENT.colour_type, colour);

  }

  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // Call the animator to blend from previous to new
  SetSegment_AnimFunctionCallback(  SEGIDX, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    }
  );

}
static const char PM_EFFECT_CONFIG__BLEND_TWO_PALETTES[] PROGMEM = ",,,,,Time,Rate;!,!,!,!,!;!"; // 7 sliders + 4 options before first ;
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED





/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : NOTDONE Blend Palette Between Another Palette
 * @note : Using a static palette (in order), the effect will change from static_palette (palette.id) to a different palette (palette_id saved as aux0)
 *         The "0%", ie least saturated, will be set by the intensity value scaled from 255 to 100 range
 * 
 * @param  "Intensity" :: 0-255 value is mapped into 0-100 and controls the amount to blend into the second one. ie 0%=palette_id and 100%=aux0_palette
 * @param  "Time"      :: If unset (ie set to 0), then the speed will assume the DEFAULT_MAXIMUM_ANIMATION_SPEED_PERIOD as 10 seconds
 *         "Speed"     :: If the time is set to zero, then the Speed will scale from sweep of saturation time between DEFAULT_MINIMUM_ANIMATION_SPEED_PERIOD (1 second) and DEFAULT_MAXIMUM_ANIMATION_SPEED_PERIOD (10 second)
 *          
 * This effect later needs intensity to define the brightness difference between the base palette and the secondary "flash" palette
 * The speed must also be made to work like it does for WLED, instead of setting a manual ratems
 * 
     * Desc: Draws palette_primary in order, then randomly takes from a second palette (saved by aux0)
     *       This will allow white palettes to work, or else applying coloured twinkles over a white palette too
     * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
void mAnimatorLight::EffectAnim__Twinkle_Palette_Onto_Palette()
{
  uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type) * 2 * SEGMENT.virtualLength();

  if (!SEGMENT.allocateData(dataSize)){    
    ALOG_ERR( PM_JSON_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = EFFECTS_FUNCTION__SOLID_COLOUR__ID;
    return;
  }
  
  /**
   * @brief Step 1: Draw palette 1
   * 
   */
  RgbcctColor colour = RgbcctColor(0);
  for(uint16_t pixel = 0; 
                pixel < SEGMENT.virtualLength(); 
                pixel++
  ){

    colour = SEGMENT.GetPaletteColour(pixel); // mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette.id, pixel);
    
    colour = ApplyBrightnesstoDesiredColourWithGamma(colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());
    
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type, colour);

    #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    ALOG_INF( PSTR("sIndexIO=%d %d,%d\t%d,pC %d, R%d"), SEGIDX, SEGMENT.pixel_range.start, SEGMENT.pixel_range.stop, pixel, GetNumberOfColoursInPalette(mPaletteI->static_palettes.ptr), colour.R );
    #endif

  }

  /**
   * @brief Step 2: Draw over randomly with palette 2
   * Intensity will pick how many to randomly pick
   * 
   * For xmas2022, forced as white
   */
  colour = RgbcctColor(255,255,255);
  uint16_t random_pixel = 0;
  
  uint16_t pixels_to_update = mapvalue(
                                      SEGMENT.intensity(), 
                                      0,255, 
                                      0,SEGMENT.length()
                                    );

  for(uint16_t pixel = 0; 
                pixel < pixels_to_update; 
                pixel++
  ){

    // colour = RgbcctColor::GetU32Colour(mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette.id, pixel);
    
    // if(SEGMENT.flags.brightness_applied_during_colour_generation){
    //   colour = ApplyBrightnesstoDesiredColourWithGamma(colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());
    // }

    random_pixel = random(0, SEGMENT.length()); // Indexing must be relative to buffer

    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), random_pixel, SEGMENT.colour_type, colour);

    #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    ALOG_INF( PSTR("sIndexIO=%d %d,%d\t%d,pC %d, R%d"), SEGIDX, SEGMENT.pixel_range.start, SEGMENT.pixel_range.stop, pixel, GetNumberOfColoursInPalette(mPaletteI->static_palettes.ptr), colour.R );
    #endif

  }



  /**
   * @brief Update outputs
   * 
   */

  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // Call the animator to blend from previous to new
  SetSegment_AnimFunctionCallback(  SEGIDX, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    }
  );

}
static const char PM_EFFECT_CONFIG__TWINKLE_PALETTE_SEC_ON_ORDERED_PALETTE_PRI[] PROGMEM = ",,,,,Time,Rate;!,!,!,!,!;!"; // 7 sliders + 4 options before first ;
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : NOTDONE Blend Palette Between Another Palette
 * @note : 
     * Desc: Picks colours from palette and randomly adds them as full brightness
     *       With each call of the animation (as new colours are added), all pixels will
     *       decay in brightness (easiest by half each time until 0).
     *       Note: The decay must happen first, so the new draws are still at full brightness
     *       This will require saving the total output in the buffer.
     * 
     * Para: 
     * 
     * ^^ This is the one I aciddenly made on and recorded on video. 
     * 
     * I simply want random leds to turn on, then everything evenally fades together until off.
     * The amount of on/darkness will depend on intensity
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
void mAnimatorLight::EffectAnim__Twinkle_Decaying_Palette()
{
  uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type) * 2 * SEGMENT.virtualLength();

// effect not written yet, copied from another

  if (!SEGMENT.allocateData(dataSize)){    
    ALOG_ERR( PM_JSON_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = EFFECTS_FUNCTION__SOLID_COLOUR__ID;
    return;
  }
  
  /**
   * @brief Step 1: Draw palette 1
   * 
   */
  RgbcctColor colour = RgbcctColor(0);
  for(uint16_t pixel = 0; 
                pixel < SEGMENT.virtualLength(); 
                pixel++
  ){

    colour = SEGMENT.GetPaletteColour(pixel); // mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette.id, pixel);
    
    colour = ApplyBrightnesstoDesiredColourWithGamma(colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());
    
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type, colour);

    #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    ALOG_INF( PSTR("sIndexIO=%d %d,%d\t%d,pC %d, R%d"), SEGIDX, SEGMENT.pixel_range.start, SEGMENT.pixel_range.stop, pixel, GetNumberOfColoursInPalette(mPaletteI->static_palettes.ptr), colour.R );
    #endif

  }

  /**
   * @brief Step 2: Draw over randomly with palette 2
   * Intensity will pick how many to randomly pick
   * 
   * For xmas2022, forced as white
   */
  colour = RgbcctColor(255,255,255);
  uint16_t random_pixel = 0;
  
  uint16_t pixels_to_update = mapvalue(
                                      SEGMENT.intensity(), 
                                      0,255, 
                                      0,SEGMENT.length()
                                    );

  for(uint16_t pixel = 0; 
                pixel < pixels_to_update; 
                pixel++
  ){

    // colour = RgbcctColor::GetU32Colour(mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette.id, pixel);
    
    // if(SEGMENT.flags.brightness_applied_during_colour_generation){
    //   colour = ApplyBrightnesstoDesiredColourWithGamma(colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());
    // }

    random_pixel = random(0, SEGMENT.length()); // Indexing must be relative to buffer

    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), random_pixel, SEGMENT.colour_type, colour);

    #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    ALOG_INF( PSTR("sIndexIO=%d %d,%d\t%d,pC %d, R%d"), SEGIDX, SEGMENT.pixel_range.start, SEGMENT.pixel_range.stop, pixel, GetNumberOfColoursInPalette(mPaletteI->static_palettes.ptr), colour.R );
    #endif

  }



  /**
   * @brief Update outputs
   * 
   */

  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // Call the animator to blend from previous to new
  SetSegment_AnimFunctionCallback(  SEGIDX, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    }
  );

}
static const char PM_EFFECT_CONFIG__TWINKLE_DECAYING_PALETTE[] PROGMEM = ",,,,,Time,Rate;!,!,!,!,!;!"; // 7 sliders + 4 options before first ;
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : EffectAnim__SunPositions_Elevation_Palette_Progress_LinearBlend
 * @note : Randomly changes colours of pixels, and blends to the new one
 *  
 * Best to develop this with 12 pixel ring, esp32
 * Start with RGPBO palette
 * 
 * Add test command cpp file, that lets me set anything in struct for development with mqtt. Only include in development build
 * 
 * Step 1: Scale palette progress with elevation (manual) 
 * Step 2: Use user defined max/min elevations, or, enable suns max/min of the day (is this directly in the math, or does it need to be calculated? perhaps at midnight for the next day)
 *         Save as uint16_t as it uses less memory than float, ie 12.34 will be 1234, what about minus, signed int16_t gives 32k +-, for only 2 decimel places, this is enough is +/-32768 can store 327.68 
 * 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS 
void mAnimatorLight::EffectAnim__SunPositions_Elevation_Palette_Progress_Step()
{

// for sun thing
// I will still use this function to get the raw colour, I just need another intermediate function that does the conversion with sun elevation
// also add sun elevation and azimuth into settings struct, that way I can update it locally or via command 

  uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type) * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  //AddLog(LOG_LEVEL_TEST, PSTR("dataSize = %d"), dataSize);

  if (!SEGMENT.allocateData(dataSize))
  {
    #ifdef ENABLE_LOG_LEVEL_ERROR
    AddLog(LOG_LEVEL_TEST, PSTR("Not Enough Memory"));
    #endif // ENABLE_LOG_LEVEL_INFO
    SEGMENT.effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID; // Default
  }
  
  // this should probably force order as random, then introduce static "inorder" option
  SEGMENT.transition.order_id = TRANSITION_ORDER__RANDOM__ID;  
  // So colour region does not need to change each loop to prevent colour crushing
  SEGMENT.flags.brightness_applied_during_colour_generation = true;
  
  // Pick new colours
  DynamicBuffer_Segments_UpdateDesiredColourFromPaletteSelected(SEGMENT.palette.id, SEGIDX);
  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // Call the animator to blend from previous to new
  SetSegment_AnimFunctionCallback(  SEGIDX, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    }
  );

}
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : EffectAnim__SunPositions_Elevation_Palette_Progress_LinearBlend
 * @note : Randomly changes colours of pixels, and blends to the new one
 *  
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
void mAnimatorLight::EffectAnim__SunPositions_Elevation_Palette_Progress_LinearBlend()
{

  uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type) * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  //AddLog(LOG_LEVEL_TEST, PSTR("dataSize = %d"), dataSize);

  if (!SEGMENT.allocateData(dataSize))
  {
    #ifdef ENABLE_LOG_LEVEL_ERROR
    AddLog(LOG_LEVEL_TEST, PSTR("Not Enough Memory"));
    #endif // ENABLE_LOG_LEVEL_INFO
    SEGMENT.effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID; // Default
  }
  
  // this should probably force order as random, then introduce static "inorder" option
  SEGMENT.transition.order_id = TRANSITION_ORDER__RANDOM__ID;  
  // So colour region does not need to change each loop to prevent colour crushing
  SEGMENT.flags.brightness_applied_during_colour_generation = true;
  
  // Pick new colours
  DynamicBuffer_Segments_UpdateDesiredColourFromPaletteSelected(SEGMENT.palette.id, SEGIDX);
  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // Call the animator to blend from previous to new
  SetSegment_AnimFunctionCallback(  SEGIDX, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    }
  );

}
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS


// /**************************************************************************************************************************************************************
//  * @brief  Solid_Colour_Based_On_Sun_Elevation_02
//  * @note   From -10 to noon, CCT will range from yellow to daywhite
//  * @note   From -5 to dusk, blue will go from 0 to max_brightness 
//  * 
//  * @note   Gloabl brightness will be manual, or controlled indirectly eg via mqtt
//  * 
//  * @note   Using RgbcctColour palette that is designed for each point in elevation
//  * *************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
void mAnimatorLight::SubTask_Segment_Animate_Function__SunPositions_Elevation_Only_RGBCCT_Palette_Indexed_Positions_01()
{
 
//  #ifndef DISABLE_ANIMATION_COLOURS_FOR_RGBCCT_OLD_METHOD
//   // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_02"));

//   // pCONT_iLight->animation.palette.id = mPaletteI->PALETTELIST_FIXED_SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01_ID;

//   uint8_t segment_index = SEGIDX;
//   uint16_t start_pixel = SEGMENT.pixel_range.start;
//   uint16_t end_pixel = SEGMENT.pixel_range.stop;


//   // Set palette pointer
//   mPaletteI->SetPaletteListPtrFromID(SEGMENT.palette.id);
//   // Brightness is generated internally, and rgbcct solid palettes are output values
//   SEGMENT.flags.brightness_applied_during_colour_generation = false;

//   /**
//    * Solar data to use, defined here for testing or simulations
//    * */
// float sun_elevation = 0;
// #ifdef USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
//   #ifdef USE_DEVFEATURE_SUNPOSITION_ELEVATION_USE_TESTING_VALUE
//   sun_elevation = (float)pCONT_solar->solar_position_testing.elevation;
//   #else
//   sun_elevation = (float)pCONT_solar->solar_position.elevation;
//   #endif
// #endif
//   bool sun_is_ascending = true;//pCONT_solar->solar_position_testing.direction.is_ascending;
//   // Serial.printf("\n\r\n\rsun_elevation\t => %f\n\r", sun_elevation);

//   // delay(1000);

//   /**
//    * Sun elevation indexing is stored in palettes index location.
//    * The current sun elevation shall be searched against for nearest match, then depending on accesending or decending sun the nearest match and nearest next match will be linearblended as current show colour
//    * */

//   /**
//    * Get total pixels in palette
//    * */
//   mPalette::PALETTE* ptr = &mPaletteI->static_palettes[SEGMENT.palette.id];
//   uint8_t pixels_max = GetNumberOfColoursInPalette(palette_p);
//   // AddLog(LOG_LEVEL_INFO,PSTR("pixels_max=%d"),pixels_max);

//   // Lets assume we need a zero crossing index, thus, we can use it to identity AS and DE modes
//   uint8_t zero_crossing_index = 0;

//   struct INDEXES_MATCHES{
//     uint8_t previous = 0; //ie colour moving away from
//     uint8_t next = 0; //colour moving towards
//   }index;

//   /**
//    * Steps to finding index
//    * 1) Find the zero-crossing index from the palette (ie the colour where its index is 0)
//    * 2) Decide if elevation is pos or neg, begin searching that part of the array
//    * 3) Find index of closest in array
//    * 4) Next and previous index will depend on direction of sun, and will be equal to current index if error is exactly 0
//    * */

//   /**
//    * Step X: Find zero crossing point
//    * Step X: Find all differences
//    * */
//   int16_t indexing = 0;  
//   uint8_t lower_boundary_index = 13;
//   float lower_boundary_value = 45;
//   uint8_t upper_boundary_index = 14;  
//   float upper_boundary_value = 90;
//   float sun_positions_from_palette_index[pixels_max];  
//   uint8_t searching_matched_index = 0;

//   /**
//    * Ascending method for finding right region between points
//    * Check all, but once sun_elev is greater, then thats the current region
//    * */
//   for(int i=0;i<pixels_max;i++)
//   {
//     mPaletteI->GetColourFromPalette(palette_p, i, &indexing);
//     sun_positions_from_palette_index[i] = indexing - 90;
//     // Serial.printf("sun_pos=[%02d]=\t%f\n\r", i, sun_positions_from_palette_index[i]);
//   }


//   for(int i=0;i<pixels_max;i++)
//   {
//     // Serial.printf("sun=%f > index[%d]=%f\n\r", sun_elevation, i, sun_positions_from_palette_index[i]);
//     if(sun_elevation >= sun_positions_from_palette_index[i])
//     {
      
//       // searching_matched_index = i;
//       // Serial.printf("sun=%f > index[%d]=%f   MATCH=%d\n\r", 
//       //   sun_elevation, i, sun_positions_from_palette_index[i], searching_matched_index
//       // );
//       //Serial.printf("Still less\n\r");

//     }else{
      
//       searching_matched_index = i-1;
//       // Serial.printf("sun=%f > index[%d]=%f   MATCH=%d\n\r", 
//       //   sun_elevation, i, sun_positions_from_palette_index[i], searching_matched_index
//       // );
//       // Serial.printf("searching_matched_index = %d\n\r", searching_matched_index);
//       break;

//     }

//     // Directly, manually, check the last memory space

//     if(sun_elevation == sun_positions_from_palette_index[pixels_max-1])
//     {
//       searching_matched_index = i-1;
//       // Serial.printf("sun=%f > index[%d]=%f   MATCH=%d\n\r", 
//       //   sun_elevation, i, sun_positions_from_palette_index[i], searching_matched_index
//       // );
//       break;

//     }




//   }

//   lower_boundary_index = searching_matched_index;
//   upper_boundary_index = searching_matched_index+1;

//   /**
//    * Check ranges are valid, if not, reset to 0 and 1
//    * */
//   if(lower_boundary_index>=pixels_max)
//   {
//     lower_boundary_index = 0;
//     // Serial.printf("lower_boundary_index>=pixels_max\n\r");
//   }
//   if(upper_boundary_index>=pixels_max)
//   {
//     upper_boundary_index = pixels_max;
//     // Serial.printf("upper_boundary_index>=pixels_max\n\r");
//   }

//   lower_boundary_value = sun_positions_from_palette_index[lower_boundary_index];
//   upper_boundary_value = sun_positions_from_palette_index[upper_boundary_index];


//   float numer = sun_elevation        - lower_boundary_value;
//   float denum = upper_boundary_value - lower_boundary_value;
//   float progress_between_colours = numer/denum;

//   // Serial.printf("\n\r\n\r\n\rsun_elevation\t => %f\n\r", sun_elevation);
//   // Serial.printf("lower_boundary_value[%02d]=%f\n\r", lower_boundary_index, lower_boundary_value);
//   // Serial.printf("upper_boundary_value[%02d]=%f\n\r", upper_boundary_index, upper_boundary_value);
//   // Serial.printf("numer=\t%f\n\r",numer);
//   // Serial.printf("denum=\t%f\n\r",denum);
//   // Serial.printf("progress_between_colours=\t%f\n\r",progress_between_colours);

//   /**
//    * Showing the colours
//    * 1) previous
//    * 2) next
//    * 3) linearblend of the exact new colour
//    * */

//   RgbcctColor c_lower = mPaletteI->GetColourFromPalette(palette_p, lower_boundary_index);
//   RgbcctColor c_upper = mPaletteI->GetColourFromPalette(palette_p, upper_boundary_index);

//   // Serial.printf("progress_between_colours\t %f(%d)/%f(%d) => %f\n\r", 
//   //   lower_boundary_value, lower_boundary_index, 
//   //   upper_boundary_value, upper_boundary_index, progress_between_colours);

//   RgbcctColor c_blended = RgbcctColor::LinearBlend(c_lower, c_upper, progress_between_colours);

//   RgbcctColor c = c_lower; 
//   // AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_p\t%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WW,c.WC);
//   c = c_blended; 
//   // AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_b\t%d,%d,%d,%d,%d (progress %d"),c.R,c.G,c.B,c.WW,c.WC, (int)(progress_between_colours*100));
//   c = c_upper; 
//   // AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_n\t%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WW,c.WC);

//   /**
//    * Load new colour into animation
//    * */

//   SEGMENT.flags.fForceUpdate = true;

// //set desired colour
//   // SEGMENT.active_rgbcct_colour_p->  = c_blended;

//   // AddLog(LOG_LEVEL_TEST, PSTR("DesiredColour1=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   if(!SEGMENT.rgbcct_controller->getApplyBrightnessToOutput())
//   { // If not already applied, do it using global values
//     animation_colours_rgbcct.DesiredColour = ApplyBrightnesstoRgbcctColour(
//       animation_colours_rgbcct.DesiredColour, 
//       SEGMENT.rgbcct_controller->getBrightnessRGB_WithGlobalApplied(),
//       SEGMENT.rgbcct_controller->getBrightnessCCT255()
//     );
//   }

//   animation_colours_rgbcct.StartingColor = SEGMENT.GetPixelColor();

//   // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//   // this->setAnimFunctionCallback([this](const AnimationParam& param){
//   //     this->AnimationProcess_Generic_RGBCCT_Single_Colour_All(param); });

//         // Call the animator to blend from previous to new
//   SetSegment_AnimFunctionCallback(  SEGIDX, 
//     [this](const AnimationParam& param){
//       this->AnimationProcess_Generic_RGBCCT_LinearBlend_Segments(param);
//     }
//   );

//   #endif // DISABLE_ANIMATION_COLOURS_FOR_RGBCCT_OLD_METHOD
   
}
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
// /**************************************************************************************************************************************************************
//  * @brief  Solid_Colour_Based_On_Sun_Elevation_05
//  * 
//  * CCT_Mapped, day white to warm white around +-20, then >20 is max cct
//  * 
// This needs fixing, so multiple scene (rgbcct controllers) can be used together
//  * *************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS 
void mAnimatorLight::SubTask_Segment_Animate_Function__SunPositions_Elevation_Only_Controlled_CCT_Temperature_01()
{
 
  // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_05"));

// #ifndef DISABLE_ANIMATION_COLOURS_FOR_RGBCCT_OLD_METHOD


//   SEGMENT.palette.id = mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_01_ID;

//   mPaletteI->SetPaletteListPtrFromID(SEGMENT.palette.id);
//   // Set up colours
//   // Brightness is generated internally, and rgbcct solid palettes are output values

// float sun_elevation = 0;
// #ifdef USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
//   #ifdef USE_DEVFEATURE_SUNPOSITION_ELEVATION_USE_TESTING_VALUE
//   sun_elevation = (float)pCONT_solar->solar_position_testing.elevation;
//   #else
//   sun_elevation = (float)pCONT_solar->solar_position.elevation;
//   #endif
// #endif

//   if(sun_elevation < -20)
//   {
//     SEGMENT.rgbcct_controller->setCCT(pCONT_iLight->get_CTRangeMax());      
//   }else
//   if(sun_elevation > 20)
//   {
//     SEGMENT.rgbcct_controller->setCCT(pCONT_iLight->get_CTRangeMin());      
//   }else{
//     // Convert elevation into percentage
//     uint8_t elev_perc = map(sun_elevation,-20,20,0,100);
//     // Convert percentage into cct
//     uint16_t cct_val = mapvalue(elev_perc, 0,100, pCONT_iLight->get_CTRangeMax(),pCONT_iLight->get_CTRangeMin());
 
//     // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "cct_val=%d"),cct_val);
//     // Set the colour temp
//     SEGMENT.rgbcct_controller->setCCT(cct_val);    
//   }

//   SEGMENT.flags.brightness_applied_during_colour_generation = false;
//   animation_colours_rgbcct.DesiredColour  = mPaletteI->GetColourFromPalette(mPaletteI->static_palettes.ptr);
//   SEGMENT.flags.fForceUpdate = true;

//   // AddLog(LOG_LEVEL_TEST, PSTR("DesiredColour1=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   if(!SEGMENT.rgbcct_controller->getApplyBrightnessToOutput())
//   { // If not already applied, do it using global values
//     animation_colours_rgbcct.DesiredColour = ApplyBrightnesstoRgbcctColour(
//       animation_colours_rgbcct.DesiredColour, 
//       SEGMENT.rgbcct_controller->getBrightnessRGB_WithGlobalApplied(),
//       SEGMENT.rgbcct_controller->getBrightnessCCT255()
//     );
//   }

//   animation_colours_rgbcct.StartingColor = SEGMENT.GetPixelColor();

//   // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//   // this->setAnimFunctionCallback([this](const AnimationParam& param){
//   //     this->AnimationProcess_Generic_RGBCCT_Single_Colour_All(param); });

//         // Call the animator to blend from previous to new
//   SetSegment_AnimFunctionCallback(  SEGIDX, 
//     [this](const AnimationParam& param){
//       this->AnimationProcess_Generic_RGBCCT_LinearBlend_Segments(param);
//     }
//   );
   
// #endif // DISABLE_ANIMATION_COLOURS_FOR_RGBCCT_OLD_METHOD

}
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS 


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Fade_Gradient
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

// Fade solid colour from 0 to 75%, and a palette from 25 to 100% (50% overlap)
// void mAnimatorLight::SubTask_Flasher_Animate_Function_Fade_Gradient(){
    
// #ifdef ENABLE_EFFECTS_ANIMATE_FUNCTION_FADE_GRADIENT

//   flashersettings.flag_finish_flasher_pair = false;
//   flashersettings.flags.enable_random_rate = true;
  
//   // Apply green gradient, brightest at start

//   uint16_t start = pCONT_iLight->settings.light_size_count/2;
//   uint16_t end = pCONT_iLight->settings.light_size_count; 
//   RgbTypeColor colour_gradient = HsbColor(
//                                           pCONT_iLight->HUE_N2F(120),
//                                           pCONT_iLight->SatN2F(100),
//                                           pCONT_iLight->BrtN2F(map(pCONT_iLight->getBriRGB(),0,255,0,100))  
//                                           );
//   RgbTypeColor colour_random = RgbTypeColor(255,0,0); 
//   HsbColor colour_random_adjusted = HsbColor(RgbColor(0));
//   uint8_t gradient_end_percentage = 75;
//   uint16_t strip_size_gradient = pCONT_iLight->settings.light_size_count*(gradient_end_percentage/100.0f);
//   uint16_t strip_size_single   = pCONT_iLight->settings.light_size_count*(75/100.0f);
  
//   start = 0;
//   end = pCONT_iLight->settings.light_size_count;
//   for(ledout.index=start;ledout.index<end;ledout.index++){ 
//     animation_colours[ledout.index].DesiredColour = RgbTypeColor(0);
//   }
  
//   //0 to 75% 
//   start = 0;
//   end = map(75,0,100,0,pCONT_iLight->settings.light_size_count);
//   for(ledout.index=start;ledout.index<end;ledout.index++){ 
//     animation_colours[ledout.index].DesiredColour.R = pCONT_iLight->ledGamma(map(ledout.index,start,end,colour_gradient.R,0));
//     animation_colours[ledout.index].DesiredColour.G = pCONT_iLight->ledGamma(map(ledout.index,start,end,colour_gradient.G,0));
//     animation_colours[ledout.index].DesiredColour.B = pCONT_iLight->ledGamma(map(ledout.index,start,end,colour_gradient.B,0));
//   }

//   mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
//   uint8_t pixels = GetNumberOfColoursInPalette(mPaletteI->static_palettes.ptr);
//   uint8_t desired_pixel;
  
//   // 25 to 100%
//   start = map(25,0,100,0,pCONT_iLight->settings.light_size_count);
//   end = pCONT_iLight->settings.light_size_count;
//   for(ledout.index=start;ledout.index<end;ledout.index++){ 
//     desired_pixel = random(0,pixels-1);
//     colour_random = mPaletteI->GetColourFromPalette(mPaletteI->static_palettes.ptr,desired_pixel);
//     if((ledout.index%3)==0){
//       colour_random_adjusted = RgbTypeColor(colour_random);
//       colour_random_adjusted.B = pCONT_iLight->BrtN2F(map(pCONT_iLight->getBriRGB(),0,255,0,100));
//       animation_colours[ledout.index].DesiredColour = colour_random_adjusted;
//     }
//   }

//   #endif

// }



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Fireplace 1D
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

// /**
//  * 1D - meaning there is no significant geography to it, similar to slow glow.
//  * 2D - means an effect will be layered onto a matrix, where flames will be shown to go up the row-axis
//  * 3D - eg cubes, effects will be drawn in 3 dimensions
//  * 
//  * */


// /****
//  * Changes pixels randomly to new colour, with slow blending
//  * Requires new colour calculation each call
//  */
// void mAnimatorLight::SubTask_Flasher_Animate_Function__FirePlace_1D_01()
// {
//   // So colour region does not need to change each loop to prevent colour crushing
//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
//   // Pick new colours
//   //Display on all pixels
//   UpdateDesiredColourFromPaletteSelected();

//   // this should probably force order as random, then introduce static "inorder" option
//   pCONT_iLight->animation.transition.order_id = TRANSITION_ORDER_RANDOM_ID;
  

// #ifdef ENABLE_DEVFEATURE_FIREPLACE_SEGMENT_EXTRA_GENERATE
//   HsbColor colour_in = HsbColor(RgbColor(0));
  
//  #ifndef USE_WS28XX_FEATURE_4_PIXEL_TYPE
//   //Overwrite random brightness on special range
//   for(uint16_t index=256;index<300;index++){

//     colour_in = animation_colours[index].DesiredColour;

//     if(colour_in.B==0){ //if colour was off, I need to set the colour to a defined value or it willl turn up brightness to show white
//       colour_in.H = 0.0f;
//       colour_in.S = 1.0f;
//     }
//     colour_in.B = pCONT_iLight->BrtN2F(random(0,10)*10);

//     // colour_in.H = pCONT_iLight->BrtN2F(random(0,100));
  
//   // This will be the introduction of segments into my code!!
//     animation_colours[random(256,299)].DesiredColour = colour_in;

//     // animation_colours[random(40,49)].DesiredColour = colour_in;

//   }
  
// #endif 
// #endif // ENABLE_DEVFEATURE_FIREPLACE_SEGMENT_EXTRA_GENERATE

// AddLog(LOG_LEVEL_TEST,PSTR("SubTask_Flasher_Animate_Function__FirePlace_1D_01 %d"),animation_colours[0].DesiredColour.R);

//   // Get starting positions already on show
//   UpdateStartingColourWithGetPixel();
//   // Call the animator to blend from previous to new
//   this->setAnimFunctionCallback(
//     [this](const AnimationParam& param){
//       this->AnimationProcess_Generic_AnimationColour_LinearBlend(param);
//     }
//   );
// }



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : RGB CLock
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/


#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK

//add "LCDDisplay_" to relevant functions
void mAnimatorLight::LCDDisplay_displayTime(time_t t, byte color, byte colorSpacing) {
  byte posOffset = 0;                                                                     // this offset will be used to move hours and minutes...
  if ( LED_DIGITS / 2 > 2 ) posOffset = 2;                                                // ... to the left so we have room for the seconds when there's 6 digits available
  if ( displayMode == 0 ) {                                                               // if 12h mode is selected...
    if ( pCONT_time->hourFormat12(t) >= 10 ){
      LCDDisplay_showDigit(1, color + colorSpacing * 2, 3 + posOffset);   // ...and hour > 10, display 1 at position 3
    }
    LCDDisplay_showDigit((pCONT_time->hourFormat12(t) % 10), color + colorSpacing * 3, 2  + posOffset);          // display 2nd digit of HH anyways
  } else if ( displayMode == 1 ) {                                                        // if 24h mode is selected...
    if ( pCONT_time->hour(t) > 9 ) LCDDisplay_showDigit(pCONT_time->hour(t) / 10, color + colorSpacing * 2, 3 + posOffset);  // ...and hour > 9, show 1st digit at position 3 (this is to avoid a leading 0 from 0:00 - 9:00 in 24h mode)
    LCDDisplay_showDigit(pCONT_time->hour(t) % 10, color + colorSpacing * 3, 2 + posOffset);                     // again, display 2nd digit of HH anyways
  }
  LCDDisplay_showDigit((pCONT_time->minute(t) / 10), color + colorSpacing * 4, 1 + posOffset);                   // minutes thankfully don't differ between 12h/24h, so this can be outside the above if/else
  LCDDisplay_showDigit((pCONT_time->minute(t) % 10), color + colorSpacing * 5, 0 + posOffset);                   // each digit is drawn with an increasing color (*2, *3, *4, *5) (*6 and *7 for seconds only in HH:MM:SS)
  if ( posOffset > 0 ) {
    LCDDisplay_showDigit((pCONT_time->second(t) / 10), color + colorSpacing * 6, 1);
    LCDDisplay_showDigit((pCONT_time->second(t) % 10), color + colorSpacing * 7, 0);
  }
  // if ( second(t) % 2 == 0 ) 
  LCDDisplay_showDots(2, 5);//pCONT_time->second(t) * 4.25);                                // show : between hours and minutes on even seconds with the color cycling through the palette once per minute
  lastSecond = pCONT_time->second(t);
}

void mAnimatorLight::LCDDisplay_showSegment(byte segment, byte color_index, byte segDisplay) {
  
  // This shows the segments from top of the sketch on a given position (segDisplay).
  // pos 0 is the most right one (seen from the front) where data in is connected to the arduino
  byte leds_per_segment = 1 + abs( segGroups[segment][1] - segGroups[segment][0] );            // get difference between 2nd and 1st value in array to get led count for this segment
  if ( segDisplay % 2 != 0 ) segment += 7;                                                  // if segDisplay/position is odd add 7 to segment

  uint16_t pixel_index = 0;

  for (byte i = 0; i < leds_per_segment; i++) {                                             // fill all leds inside current segment with color
    // animation_colours[( segGroups[segment][0] + ( segDisplay / 2 ) * ( LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS ) ) + i].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);

    pixel_index = ( segGroups[segment][0] + ( segDisplay / 2 ) * ( LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS ) ) + i;

    RgbcctColor colour = RgbcctColor();
    colour = SEGMENT.GetColourFromPalette(color_index);      
    colour = ApplyBrightnesstoDesiredColourWithGamma(colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel_index, SEGMENT.colour_type, colour);


    // RgbcctColor colour = ColorFromPaletteLCD(SEGMENT_I(0).palette.id, color);//RgbColor(255,0,0);

    // // RgbcctColor colour = RgbwColor(1,2,3,4);

    // uint8_t segment_index = 0;


    // SetTransitionColourBuffer_DesiredColour(_segment_runtimes[segment_index].Data(), _segment_runtimes[segment_index].DataLength(), pixel_index, SEGMENT.colour_type, colour);
  




    // animation_colours[( segGroups[segment][0] + ( segDisplay / 2 ) * ( LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS ) ) + i].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);
  
  
  
  
  
  }
}

void mAnimatorLight::LCDDisplay_showDigit(byte digit, byte color, byte pos) {
  // This draws numbers using the according segments as defined on top of the sketch (0 - 9)
  for (byte i = 0; i < 7; i++) {
    if (digits[digit][i] != 0) LCDDisplay_showSegment(i, color, pos);
  }
}

// /**
//  * @brief Temporary method: Normal getcolour should work, but needs to include wrap functionality ie when index exceeds size then repeat pattern
//  * 
//  * @param palette_id 
//  * @param desired_index 
//  * @param apply_global_brightness 
//  * @return RgbcctColor 
//  */
// RgbcctColor mAnimatorLight::ColorFromPaletteLCD(uint16_t palette_id, uint8_t desired_index, bool apply_global_brightness)
// {
  
//   mPalette::PALETTE* ptr = &mPaletteI->static_palettes[SEGMENT.palette.id];

//   uint8_t pixels_max = GetNumberOfColoursInPalette(ptr);

//   if(desired_index >= pixels_max){
//     desired_index %= pixels_max;
//     // ALOG_INF(PSTR("desired_index2 = %d"), desired_index);
//   }
  
//   RgbcctColor colour = mPaletteI->GetColourFromPalette(mPaletteI->GetPalettePointerByID(palette_id), desired_index);

//   if(apply_global_brightness){
//     colour = ApplyBrightnesstoRgbcctColour( colour, SEGMENT.getBrightnessRGB_WithGlobalApplied(), SEGMENT.getBrightnessCCT_WithGlobalApplied() );
//   }

//   return colour;

// }



void mAnimatorLight::LCDDisplay_showDots(byte dots, byte color) {

  // // in 12h mode and while in setup upper dots resemble AM, all dots resemble PM
  // byte startPos = LED_PER_DIGITS_STRIP;
  // if ( LED_BETWEEN_DIGITS_STRIPS % 2 == 0 ) {                                                                 // only SE/TE should have a even amount here (0/2 leds between digits)
  //   animation_colours[startPos].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);
  //   if ( dots == 2 ) animation_colours[startPos + 1].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);
  // } else {                                                                                                    // Regular and XL have 5 leds between digits
  //   animation_colours[startPos].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);
  //   animation_colours[startPos + 1].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);
  //   if ( LED_DIGITS / 3 > 1 ) {
  //       animation_colours[startPos + LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);//colour;// = ColorFromPalette(SEGMENT.palette_container->CRGB16Palette16_Palette.data, color, brightness, LINEARBLEND);
  //       animation_colours[startPos + LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS + 1].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);//colour;// = ColorFromPalette(SEGMENT.palette_container->CRGB16Palette16_Palette.data, color, brightness, LINEARBLEND);
  //     }
  //   if ( dots == 2 ) {
  //     animation_colours[startPos + 3].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);
  //     animation_colours[startPos + 4].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);
  //     if ( LED_DIGITS / 3 > 1 ) {
  //       animation_colours[startPos + LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS + 3].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);//colour;// = ColorFromPalette(SEGMENT.palette_container->CRGB16Palette16_Palette.data, color, brightness, LINEARBLEND);
  //       animation_colours[startPos + LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS + 4].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);//colour;// = ColorFromPalette(SEGMENT.palette_container->CRGB16Palette16_Palette.data, color, brightness, LINEARBLEND);
  //     }
  //   }
  // }



}

/****
 * @brief Basic clock with no animations
 */
void mAnimatorLight::SubTask_Segment_Animate_Function__LCD_Clock_Time_Basic_01()
{

  AddLog(LOG_LEVEL_TEST, PSTR("_segments[%d].colour_type = %d"), SEGIDX, SEGMENT.colour_type);

    
  uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type) * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  ALOG_DBM(PSTR("dataSize = %d"), dataSize);

  if (!SEGMENT.allocateData(dataSize))
  {
    ALOG_ERR(PSTR("Not Enough Memory"));
    SEGMENT.effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID; // Default
  }

  // So colour region does not need to change each loop to prevent colour crushing
  // SEGMENT.flags.brightness_applied_during_colour_generation = true;
  
  // Will only work with first segment
  uint8_t segment_index=0;

  for(int i=0;i<SEGLEN;i++){
    // SetTransitionColourBuffer_DesiredColour(_segment_runtimes[segment_index].Data(), _segment_runtimes[segment_index].DataLength(), i, SEGMENT.colour_type, RgbwColor(0,0,0,0));
    
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), i, SEGMENT.colour_type, RgbcctColor(0));
  }

  // if(tempcol++>5){
    tempcol=0;
    // } //startcolour

  AddLog(LOG_LEVEL_TEST, PSTR("tempcol=%d"), tempcol);

  LCDDisplay_updateDisplay(tempcol, colorOffset);

  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // Call the animator to blend from previous to new
  SetSegment_AnimFunctionCallback(  SEGIDX, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    }
  );

  #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
  this->setCallback_ConstructJSONBody_Debug_Animations_Progress(
    [this](void){
      this->ConstructJSONBody_Animation_Progress__LCD_Clock_Time_Basic_01();
    }
  );
  #endif // USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK

}

void mAnimatorLight::ConstructJSONBody_Animation_Progress__LCD_Clock_Time_Basic_01()
{   
  
  JBI->Add("lcd_display_show_number", lcd_display_show_number);
  
}




/**************************************************************************************************************************************************************
***************************************************************************************************************************************************************
********* SubTask_Segment_Animate_Function__LCD_Clock_Time_Basic_02 *************************************************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************/


/**
 * Cycles over leds that are on and applies palette
 * */
void mAnimatorLight::LCDDisplay_colorOverlay() {                                                                                       // This "projects" colors on already drawn leds before showing leds in updateDisplay();
  // for (byte i = 0; i < LED_COUNT; i++) {                                                                    // check each led...
  //   if (animation_colours[i].DesiredColour.CalculateBrightness())  {
      
  //     animation_colours[i].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, startColor + (colorOffset * i));

  //   }
    
  //                                                                                             // ...and if it is lit...
  //     // leds[i] = ColorFromPalette(SEGMENT.palette_container->CRGB16Palette16_Palette.data, startColor + (colorOffset * i), brightness, LINEARBLEND);  // ...assign increasing color from current palette
  // }
}

void mAnimatorLight::LCDDisplay_updateDisplay(byte color, byte colorSpacing) {                                                         // this is what redraws the "screen"
  // FastLED.clear();                                                                                          // clear whatever the leds might have assigned currently...
  
  // displayTime(now(), color, colorSpacing);                                                                  // ...set leds to display the time...
  
  LCDDisplay_displayTime(pCONT_time->Rtc.local_time, color, colorSpacing);
  
  
  // if (overlayMode == 1) LCDDisplay_colorOverlay();                                                                     // ...and if using overlayMode = 1 draw custom colors over single leds
  
  
  
  // if (brightnessAuto == 1) {                                                                                // If brightness is adjusted automatically by using readLDR()...
  //   FastLED.setBrightness(avgLDR);                                                                          // ...set brightness to avgLDR
  // } else {                                                                                                  // ...otherwise...
  //   FastLED.setBrightness(brightness);                                                                      // ...assign currently selected brightness
  // }
}


/****
 * Changes pixels randomly to new colour, with slow blending
 * Requires new colour calculation each call
 * 02 trying lib method with mapping
 */
void mAnimatorLight::SubTask_Segment_Animate_Function__LCD_Clock_Time_Basic_02(){
//   // So colour region does not need to change each loop to prevent colour crushing
//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
//   // Pick new colours
//   //Display on all pixels
//   UpdateDesiredColourFromPaletteSelected();
  pCONT_set->Settings.light_settings.type = LT_ADDRESSABLE_SK6812;
    
  SEGMENT.colour_type = RgbcctColor::ColourType::LIGHT_TYPE__RGBW__ID;
  
  AddLog(LOG_LEVEL_TEST, PSTR("02    SEGMENT.colour_type = %d"), SEGMENT.colour_type);

    
  uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type) * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  AddLog(LOG_LEVEL_TEST, PSTR("dataSize = %d"), dataSize);

  if (!SEGMENT.allocateData(dataSize))
  {
    AddLog(LOG_LEVEL_TEST, PSTR("Not Enough Memory"));
    SEGMENT.effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID; // Default
  }

  
  // So colour region does not need to change each loop to prevent colour crushing
  // SEGMENT.flags.brightness_applied_during_colour_generation = true;
  
  
  // for(int i=0;i<93;i++){animation_colours[i].DesiredColour = RgbcctColor(0);}
  uint8_t segment_index=0;

  for(int i=0;i<93;i++){
    // animation_colours[i].DesiredColour = RgbcctColor(0);
    // }
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), i, SEGMENT.colour_type, RgbwColor(0,0,0,0));
  
  }

  // Pick new colours
  //DynamicBuffer_Segments_UpdateDesiredColourFromPaletteSelected(SEGMENT.palette.id, SEGIDX);


  // if(tempcol++>5){
    tempcol=0;
    // } //startcolour

  AddLog(LOG_LEVEL_TEST, PSTR("tempcol=%d"), tempcol);

  LCDDisplay_updateDisplay(tempcol, colorOffset);


  // LCDDisplay_showDigit((lcd_display_show_number / 10), 0+1, 1 );                   // minutes thankfully don't differ between 12h/24h, so this can be outside the above if/else
  // LCDDisplay_showDigit((lcd_display_show_number % 10), 0, 0 );                   // each digit is drawn with an increasing color (*2, *3, *4, *5) (*6 and *7 for seconds only in HH:MM:SS)
  

  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // Call the animator to blend from previous to new
  SetSegment_AnimFunctionCallback(  SEGIDX, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    }
  );

  #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
  this->setCallback_ConstructJSONBody_Debug_Animations_Progress(
    [this](void){
      this->ConstructJSONBody_Animation_Progress__LCD_Clock_Time_Basic_02();
    }
  );
  #endif // USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK


//   // LCDDisplay_displayTime(pCONT_time->Rtc.utc_time,tempcol,0);

// //       uint16_t tempnumber = testnum;

// //       // if(tempnumber%2){
// //       //   showDots();
// //       // }
// //       if ( second(0) % 2 == 0 ) showDots(2, second(0) * 4.25);  


// //       showDigit(
// //         (tempnumber/1)%10,
// //          64, 0);
// //       showDigit(
// //       (tempnumber/10)%10,
// //          64, 1);
// //       showDigit(
// //         (tempnumber/100)%10,
// //          64, 2);
// //       showDigit(
// //         (tempnumber/1000)%10,
// //          64, 3);


// //       // showDigit(11, 64, 2);
// //       // showDigit(12, 64, 1);

// // if(testnum++>9999){testnum=0;}

  
// //  #ifndef USE_WS28XX_FEATURE_4_PIXEL_TYPE
// //   //Overwrite random brightness on special range
// //   for(uint16_t index=256;index<300;index++){

// //     colour_in = animation_colours[index].DesiredColour;

// //     if(colour_in.B==0){ //if colour was off, I need to set the colour to a defined value or it willl turn up brightness to show white
// //       colour_in.H = 0.0f;
// //       colour_in.S = 1.0f;
// //     }
// //     colour_in.B = pCONT_iLight->BrtN2F(random(0,10)*10);

// //     // colour_in.H = pCONT_iLight->BrtN2F(random(0,100));
  
// //     animation_colours[random(256,299)].DesiredColour = colour_in;

// //     // animation_colours[random(40,49)].DesiredColour = colour_in;

// //   }
//   /*
//   if (  ( lastLoop - lastColorChange >= colorChangeInterval ) && ( overlayMode == 0 )         // if colorChangeInterval has been reached and overlayMode is disabled...
//      || ( lastLoop - lastColorChange >= overlayInterval ) && ( overlayMode == 1 ) ) {         // ...or if overlayInterval has been reached and overlayMode is enabled...
//     startColor++;                                                                             // increase startColor to "move" colors slowly across the digits/leds
//     updateDisplay(startColor, colorOffset);
//     lastColorChange = millis();
//   }
//   if ( lastSecond != second() ) {                                                             // if current second is different from last second drawn...
//     updateDisplay(startColor, colorOffset);                                                   // lastSecond will be set in displayTime() and will be used for
//     lastSecond = second();                                                                    // redrawing regardless the digits count (HH:MM or HH:MM:SS)
//   }
//   if ( lastKeyPressed == 1 ) {                                                                // if buttonA is pressed...
//     switchBrightness();                                                                       // ...switch to next brightness level
//     updateDisplay(startColor, colorOffset);
//     if ( btnRepeatCounter >= 20 ) {                                                           // if buttonA is held for a few seconds change overlayMode 0/1 (using colorOverlay())
//       if ( overlayMode == 0 ) overlayMode = 1; else overlayMode = 0;
//       updateDisplay(startColor, colorOffset);
//       EEPROM.put(3, overlayMode);                                                             // ...and write setting to eeprom
//       #ifdef nodeMCU                                                                          // on nodeMCU we need to commit the changes from ram to flash to make them permanent
//         EEPROM.commit();
//       #endif
//       btnRepeatStart = millis();
//     }
//   }
//   if ( lastKeyPressed == 2 ) {                                                                // if buttonB is pressed...
//     switchPalette();                                                                          // ...switch between color palettes
//     updateDisplay(startColor, colorOffset);
//     if ( btnRepeatCounter >= 20 ) {                                                           // if buttonB is held for a few seconds change displayMode 0/1 (12h/24h)...
//       if ( displayMode == 0 ) displayMode = 1; else displayMode = 0;
//       updateDisplay(startColor, colorOffset);
//       EEPROM.put(2, displayMode);                                                             // ...and write setting to eeprom
//       #ifdef nodeMCU
//         EEPROM.commit();
//       #endif
//       btnRepeatStart = millis();
//     }
//   }
//   if ( ( lastLoop - valueLDRLastRead >= intervalLDR ) && ( brightnessAuto == 1 ) ) {          // if LDR is enabled and sample interval has been reached...
//     readLDR();                                                                                // ...call readLDR();
//     if ( abs(avgLDR - lastAvgLDR) >= 5 ) {                                                    // only adjust current brightness if avgLDR has changed for more than +/- 5.
//       updateDisplay(startColor, colorOffset);
//       lastAvgLDR = avgLDR;
//       if ( dbg ) { Serial.print(F("Updated display with avgLDR of: ")); Serial.println(avgLDR); }
//     }
//     valueLDRLastRead = millis();
//   }
//   if ( lastKeyPressed == 12 ) {                                                               // if buttonA + buttonB are pushed at the same time....
//     #ifdef useWiFi                                                                            // ...and if using WiFi...
//       initWPS();                                                                              // ...start WPS
//     #else                                                                                     // otherwise (arduino + rtc or nodemcu + rtc)...
//       setupClock();                                                                           // ...start manual setup
//     #endif
//   }
//   #ifdef nodeMCU                                                                              // On Arduino SetSyncProvider will be used. So this will sync internal time to rtc/ntp on nodeMCU only
//     if ( ( hour() == 3 || hour() == 9 || hour() == 15 || hour() == 21 ) &&                    // if hour is 3, 9, 15 or 21 and...
//          ( minute() == 3 && second() == 0 ) ) {                                               // minute is 3 and second is 0....
//       if ( dbg ) Serial.print(F("Current time: ")); Serial.println(now());
//       #ifdef useWiFi
//         syncNTP();                                                                            // ...either sync using ntp or...
//       #else
//         setTime(Rtc.GetDateTime());                                                           // ...set internal time to rtc time...
//       #endif
//       if ( dbg ) Serial.print(F("New time: ")); Serial.println(now());
//     }
//     ESP.wdtFeed();                                                                            // feed the watchdog each time loop() is cycled through, just in case...
//   #endif
//   FastLED.show();                                                                             // run FastLED.show() every time to avoid color flickering at low brightness settings
//   lastKeyPressed = readButtons();
//   lastLoop = millis();
//   if ( dbg ) dbgInput();   */
// // #endif 

//   // Get starting positions already on show
//   UpdateStartingColourWithGetPixel();
//   // Call the animator to blend from previous to new
//   this->setAnimFunctionCallback(
//     [this](const AnimationParam& param){
//       this->AnimationProcess_Generic_AnimationColour_LinearBlend(param);
//     }
//   );
}

void mAnimatorLight::ConstructJSONBody_Animation_Progress__LCD_Clock_Time_Basic_02()
{   
  
  JBI->Add("lcd_display_show_number", lcd_display_show_number);
  
}


/****
 * Changes pixels randomly to new colour, with slow blending
 * Requires new colour calculation each call
 * 02 trying lib method with mapping
 */
void mAnimatorLight::SubTask_Flasher_Animate_LCD_Display_Show_Numbers_Basic_01()
{
  
  AddLog(LOG_LEVEL_TEST, PSTR("Numbers_Basic    _segments[].colour_type = %d"), SEGMENT.colour_type);
  AddLog(LOG_LEVEL_TEST, PSTR("lcd_display_show_number = %d"), lcd_display_show_number);


  

//  SEGMENT.colour_type = RgbcctColor::ColourType::LIGHT_TYPE__RGB__ID;
    


  
  uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type) * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  AddLog(LOG_LEVEL_TEST, PSTR("dataSize = %d"), dataSize);

  if (!SEGMENT.allocateData(dataSize))
  {
    AddLog(LOG_LEVEL_TEST, PSTR("Not Enough Memory"));
    SEGMENT.effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID; // Default
  }
  
  // So colour region does not need to change each loop to prevent colour crushing
  // SEGMENT.flags.brightness_applied_during_colour_generation = true;
  
  /**
   * @brief Reset all new colours so only new sections of clock are lit
   **/
  for(int i=0;i<SEGLEN;i++){
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), i, SEGMENT.colour_type, RgbwColor(0,0,0,0));
  }


  // Pick new colours
  //DynamicBuffer_Segments_UpdateDesiredColourFromPaletteSelected(SEGMENT.palette.id, SEGIDX);

/**
 * @brief Temporary force until handled in code
 * 
 */
  // pCONT_set->Settings.light_settings.type = LT_ADDRESSABLE_WS281X;
  // SEGMENT.colour_type = RgbcctColor_Controller::LightSubType::LIGHT_TYPE__RGB__ID;

  uint8_t digit_count = mSupport::NumDigits(lcd_display_show_number);

  // 2 digits only
  // if(digit_count <= 2)
  // {
    LCDDisplay_showDigit((lcd_display_show_number / 10), 0+1, 1 );                   // minutes thankfully don't differ between 12h/24h, so this can be outside the above if/else
    LCDDisplay_showDigit((lcd_display_show_number % 10),   0, 0 );                   // each digit is drawn with an increasing color (*2, *3, *4, *5) (*6 and *7 for seconds only in HH:MM:SS)
  // }
  // else
  // if(
  //   (digit_count <= 4) ||
  //   (digit_count >= 3)
  // ){ 
    
  //   printf("%d\n", score % 10);
  //   score /= 10;

  //   uint16_t num = lcd_display_show_number;




  //   LCDDisplay_showDigit((lcd_display_show_number / 10), 0+1, 1 );                   // minutes thankfully don't differ between 12h/24h, so this can be outside the above if/else
  //   LCDDisplay_showDigit((lcd_display_show_number % 10),   0, 0 );                   // each digit is drawn with an increasing color (*2, *3, *4, *5) (*6 and *7 for seconds only in HH:MM:SS)
  
  //   LCDDisplay_showDigit((lcd_display_show_number / 10), 0+1, 1 );                   // minutes thankfully don't differ between 12h/24h, so this can be outside the above if/else
  //   LCDDisplay_showDigit((lcd_display_show_number % 10),   0, 0 );                   // each digit is drawn with an increasing color (*2, *3, *4, *5) (*6 and *7 for seconds only in HH:MM:SS)
  


  // }


  // LCDDisplay_showDigit((lcd_display_show_number / 10), 0+1, 1 );                   // minutes thankfully don't differ between 12h/24h, so this can be outside the above if/else
  // LCDDisplay_showDigit((lcd_display_show_number % 10),   0, 0 );                   // each digit is drawn with an increasing color (*2, *3, *4, *5) (*6 and *7 for seconds only in HH:MM:SS)
  
  // lcd_display_show_number = 67;



  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // Call the animator to blend from previous to new
  SetSegment_AnimFunctionCallback(  SEGIDX, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    }
  );

}




/****
 * Changes pixels randomly to new colour, with slow blending
 * Requires new colour calculation each call
 * 02 trying lib method with mapping
 */
void mAnimatorLight::SubTask_Flasher_Animate_LCD_Display_Show_String_01()
{
  
  AddLog(LOG_LEVEL_TEST, PSTR("Numbers_Basic    _segments[].colour_type = %d"), SEGMENT.colour_type);
  AddLog(LOG_LEVEL_TEST, PSTR("lcd_display_show_number = %d"), lcd_display_show_number);


  

//  SEGMENT.colour_type = RgbcctColor::ColourType::LIGHT_TYPE__RGB__ID;
    


  
  uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type) * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  AddLog(LOG_LEVEL_TEST, PSTR("dataSize = %d"), dataSize);

  if (!SEGMENT.allocateData(dataSize))
  {
    AddLog(LOG_LEVEL_TEST, PSTR("Not Enough Memory"));
    SEGMENT.effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID; // Default
  }
  
  // So colour region does not need to change each loop to prevent colour crushing
  // SEGMENT.flags.brightness_applied_during_colour_generation = true;
  
  /**
   * @brief Reset all new colours so only new sections of clock are lit
   **/
  for(int i=0;i<SEGLEN;i++){
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), i, SEGMENT.colour_type, RgbwColor(0,0,0,0));
  }


  // Pick new colours
  //DynamicBuffer_Segments_UpdateDesiredColourFromPaletteSelected(SEGMENT.palette.id, SEGIDX);

/**
 * @brief Temporary force until handled in code
 * 
 */
  // pCONT_set->Settings.light_settings.type = LT_ADDRESSABLE_WS281X;
  // SEGMENT.colour_type = RgbcctColor_Controller::LightSubType::LIGHT_TYPE__RGB__ID;

  uint8_t digit_count = strlen(lcd_display_show_string);

  if(digit_count > 4)
  {
    ALOG_ERR(PSTR("too big"));
  }

  uint8_t pos = 0;
  uint8_t number_show = 0;

  for(int i=0;i<digit_count;i++)
  {
    pos = digit_count - 1 - i;
    if(lcd_display_show_string[i] != ' ')
    {
      LCDDisplay_showDigit(lcd_display_show_string[i]-48, 0, pos ); 
    }
  }
  
  
  
  



  // 2 digits only
  // if(digit_count <= 2)
  // // {
  //   LCDDisplay_showDigit((lcd_display_show_number / 10), 0+1, 1 );                   // minutes thankfully don't differ between 12h/24h, so this can be outside the above if/else
  //   LCDDisplay_showDigit((lcd_display_show_number % 10),   0, 0 );                   // each digit is drawn with an increasing color (*2, *3, *4, *5) (*6 and *7 for seconds only in HH:MM:SS)
  // // }
  // else
  // if(
  //   (digit_count <= 4) ||
  //   (digit_count >= 3)
  // ){ 
    
  //   printf("%d\n", score % 10);
  //   score /= 10;

  //   uint16_t num = lcd_display_show_number;




  //   LCDDisplay_showDigit((lcd_display_show_number / 10), 0+1, 1 );                   // minutes thankfully don't differ between 12h/24h, so this can be outside the above if/else
  //   LCDDisplay_showDigit((lcd_display_show_number % 10),   0, 0 );                   // each digit is drawn with an increasing color (*2, *3, *4, *5) (*6 and *7 for seconds only in HH:MM:SS)
  
  //   LCDDisplay_showDigit((lcd_display_show_number / 10), 0+1, 1 );                   // minutes thankfully don't differ between 12h/24h, so this can be outside the above if/else
  //   LCDDisplay_showDigit((lcd_display_show_number % 10),   0, 0 );                   // each digit is drawn with an increasing color (*2, *3, *4, *5) (*6 and *7 for seconds only in HH:MM:SS)
  


  // }


  // LCDDisplay_showDigit((lcd_display_show_number / 10), 0+1, 1 );                   // minutes thankfully don't differ between 12h/24h, so this can be outside the above if/else
  // LCDDisplay_showDigit((lcd_display_show_number % 10),   0, 0 );                   // each digit is drawn with an increasing color (*2, *3, *4, *5) (*6 and *7 for seconds only in HH:MM:SS)
  
  // lcd_display_show_number = 67;



  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // Call the animator to blend from previous to new
  SetSegment_AnimFunctionCallback(  SEGIDX, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    }
  );

}


#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Popping Brightness (the accidental "Pop fade")
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/



// /****
//  * Changes pixels randomly to new colour, with slow blending
//  * Requires new colour calculation each call
//  */
// void mAnimatorLight::SubTask_Flasher_Animate_Function_Popping_Palette_Brightness_From_Lower_To_Upper_Boundary(){

//    switch(flashersettings.region){
//     case EFFECTS_REGION_COLOUR_SELECT_ID: //set colours
//       AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
//       pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
//       // if(flashersettings.flags.enable_endingcolour_as_alternate){
//       //   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL flashersettings.flags.enable_endingcolour_as_alternate"));
//       //   UpdateDesiredColourWithSingleColour(RgbcctColor(0));
//       // }

//       SEGMENT.rgbcct_controller->setBrightnessRGB255(map(flashersettings.brightness_min, 0,100, 0,255));


//       UpdateDesiredColourFromPaletteSelected();
//       // if(flashersettings.flags.enable_startcolour_as_alternate){
//       //   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL flashersettings.flags.enable_startcolour_as_alternate"));
//       //   UpdateStartingColourWithSingleColour(RgbcctColor(0));
//       // }else{
//         UpdateStartingColourWithGetPixel();
//       // }
//       flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
//     break;
//     case EFFECTS_REGION_ANIMATE_ID: //shift along

    
//       UpdateStartingColourWithGetPixel();

//       uint16_t index_random = random(0,pCONT_iLight->settings.light_size_count);

//       //#ifndef USE_WS28XX_FEATURE_4_PIXEL_TYPE //tmp fix
//       //HsbColor hsb = RgbColor(SEGMENT.GetPixelColor(index_random));
//       //#else
//       HsbColor hsb = RgbColor(SEGMENT.GetPixelColor(index_random));
//       //#endif

//       uint8_t brightness_now = mapvalue(pCONT_iLight->BrtF2N(hsb.B),0,100,0,255);

//       AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE %d %d"), brightness_now, flashersettings.brightness_max);

//       if(brightness_now < flashersettings.brightness_max){ //+5 to margins so its not the same
//         hsb.B = pCONT_iLight->BrtN2F(flashersettings.brightness_max);
//         // animation_colours[index_random].DesiredColour = ApplyBrightnesstoDesiredColour(animation_colours[index_random].DesiredColour, flashersettings.brightness_max);
//         animation_colours[index_random].DesiredColour = hsb;
//       }

//       AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//       this->setAnimFunctionCallback([this](const AnimationParam& param){ this->AnimationProcess_Generic_AnimationColour_LinearBlend(param); });
//     break;
//   }

// }

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

// void mAnimatorLight::SubTask_Flasher_Animate_Function_Pulse_Random_On_Fade_Off(){

//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
  
//   flashersettings.flag_finish_flasher_pair = false;
//   flashersettings.flags.enable_random_rate = true;
  
//   do{ //must complete the pair together
//     switch(flashersettings.region){
//       case EFFECTS_REGION_COLOUR_SELECT_ID:{ //set colours
//         //#ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
//         AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "Random_On_2 EFFECTS_COLOUR_SELECT"));
//         //#endif

//         pCONT_iLight->settings.light_size_count = 100;

//         // Go through strip, randomly change some to coloured or black
//         mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
//         uint8_t pixels = GetNumberOfColoursInPalette(mPaletteI->static_palettes.ptr);
//         RgbTypeColor colour_random = RgbTypeColor(0);

//         uint16_t random_pixel_index = 0;

//         // Serial.printf("DesiredColour1=%d,%d,%d\n\r",animation_colours[0].DesiredColour.R,animation_colours[0].DesiredColour.G,animation_colours[0].DesiredColour.B);
//         //   animation_colours[0].DesiredColour = SEGMENT.GetPixelColor(0);
//         // Serial.printf("DesiredColour2=%d,%d,%d\n\r",animation_colours[0].DesiredColour.R,animation_colours[0].DesiredColour.G,animation_colours[0].DesiredColour.B);
//         // animation_colours[0].DesiredColour.Darken(20);
//         // Serial.printf("DesiredColour3=%d,%d,%d\n\r",animation_colours[0].DesiredColour.R,animation_colours[0].DesiredColour.G,animation_colours[0].DesiredColour.B);
//         // Serial.printf("DesiredColour1=%d,%d,%d\n\r",animation_colours[0].DesiredColour.R,animation_colours[0].DesiredColour.G,animation_colours[0].DesiredColour.B);
        
//         // for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
//         //   animation_colours[index].DesiredColour = SEGMENT.GetPixelColor(index);
//         //   // animation_colours[index].DesiredColour.Darken(20);//Darken(10);// = SEGMENT.GetPixelColor(index);
        
//         // }


//         UpdateDesiredColourWithGetPixel();

//         // randomly blank most of them out again
// //        uint8_t colour_or_black = 0;
//         uint16_t pixels_to_update = 5;//pCONT_iLight->GetPixelsToUpdateAsNumberFromPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val);
//         Serial.printf("pixels_to_clear=%d\n\r",pixels_to_update);
//         for(uint16_t index=0; index<pixels_to_update; index++ ){
//           random_pixel_index = index;//random(0,100);//pCONT_iLight->settings.light_size_count);
//           // colour_or_black = random(0,1);

//           // // Check if pixels is already on, if so, make dark
//           // if(pCONT_iLight->RgbColorto32bit(SEGMENT.GetPixelColor(random_pixel_index))>0){
//           //   animation_colours[random_pixel_index].DesiredColour = RgbColor(0,0,0);
//           // }else
//           // // Previously off, pick new state
//           // {
//             // if(colour_or_black){ // 80% in favour of black/off
//             //   animation_colours[random_pixel_index].DesiredColour = RgbColor(0,0,0);
//             // }else{
              
//               uint8_t desired_pixel = random(0,4);//pixels-1);
//               colour_random = mPaletteI->GetColourFromPalette(mPaletteI->static_palettes.ptr,desired_pixel);  

// //if already on, dont change the colour
//             if(!animation_colours[random_pixel_index].DesiredColour.CalculateBrightness()){// if off, allow new colour 
//               if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
//                 colour_random = ApplyBrightnesstoRgbcctColour(colour_random,SEGMENT.getBrightnessRGB_WithGlobalApplied());
//               }
//               animation_colours[random(0,10)].DesiredColour = colour_random;//RgbColor(0,0,255);//
//             }else{
              
//               animation_colours[index].DesiredColour.Darken(2);//Darken(10);// = SEGMENT.GetPixelColor(index);
//             }
           
//           // }
//         }
//         // if(test_flag){
//         //   animation_colours[random_pixel_index].DesiredColour =  RgbColor(0,0,255);
//         // }else{
//         //   animation_colours[random_pixel_index].DesiredColour =  RgbColor(0,255,0);
//         // }
//         //  for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
//         //   animation_colours[index].DesiredColour.Darken(10);//Darken(10);// = SEGMENT.GetPixelColor(index);
//         // }


//         AddLog(LOG_LEVEL_TEST, PSTR("From G,B=%d,%d"),animation_colours[random_pixel_index].DesiredColour.G,animation_colours[random_pixel_index].DesiredColour.B);

//         // Get starting positions already on show
//         UpdateStartingColourWithGetPixel();
//         // AddLog(LOG_LEVEL_TEST, PSTR("From G,B=%d,%d"),animation_colours[random_pixel_index].StartingColor.G,animation_colours[random_pixel_index].StartingColor.B);


//         // dont automatically run animate again, add timeout for it
//         flashersettings.region = EFFECTS_REGION_ANIMATE_ID;

//       }break;
//       case EFFECTS_REGION_ANIMATE_ID: //shift along
//         //#ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
//         AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "Random_On_2 EFFECTS_ANIMATE"));
//         //#endif

//         this->setAnimFunctionCallback(
//           [this](const AnimationParam& param){
//             this->AnimationProcess_Pulse_Random_On_Fade_Off(param);
//           }
//         );

//         if(flashersettings.flags.force_finish_flasher_pair_once){
//           ALOG_DBM( PSTR("flashersettings.flags.force_finish_flasher_pair_once WAS SET"));
//           flashersettings.flags.force_finish_flasher_pair_once = false;
//         }
        
//         flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID;
        
//       break;
//     }
//   }while(flashersettings.flag_finish_flasher_pair || flashersettings.flags.force_finish_flasher_pair_once);

// }


// // 50% progress is full brightness, 0 and 100% is off
// void mAnimatorLight::AnimationProcess_Pulse_Random_On_Fade_Off(const AnimationParam& param)
// {    
//  RgbTypeColor updatedColor;
//  pCONT_iLight->settings.light_size_count = 10;
//   for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    
//     if(pixel==0){
//       // Serial.printf("[%d]\t%d | %d\n\r",pixel,animation_colours[pixel].StartingColor.CalculateBrightness(),animation_colours[pixel].DesiredColour.CalculateBrightness());
//     }
//     //if desired is brighter than starting, don't blend, do instant (ie pulsed on not fading off)
//     if(animation_colours[pixel].StartingColor.CalculateBrightness() <= animation_colours[pixel].DesiredColour.CalculateBrightness()){
//       // Serial.println("<<<<<<<<<<<<<<<<<<<<<");
//       updatedColor = animation_colours[pixel].DesiredColour; // instant to brighter colour
//     }
//     // Fade down if new brightness is lower
//      else{
//        //Serial.println(">>>>>>>>>>>>>>>>>");
//       updatedColor = RgbTypeColor::LinearBlend(
//         animation_colours[pixel].StartingColor,
//         animation_colours[pixel].DesiredColour,
//         param.progress);    
//     }


//     SEGMENT.SetPixelColor(pixel, updatedColor);
//   } // END for
  
//   // AddLog(LOG_LEVEL_TEST, PSTR("-->> G,B=%d,%d"),updatedColor.G, updatedColor.B);

// }


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/



// void mAnimatorLight::SubTask_Flasher_Animate_Function_Pulse_Random_On(){

//   // So colour region does not need to change each loop to prevent colour crushing
//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
//   // Pick new colours
//   UpdateDesiredColourFromPaletteSelected();
//   // randomly blank most of them out again
//   uint16_t pixels_to_clear = 
//   pCONT_iLight->animation.transition.pixels_to_update_as_number;
//   //pCONT_iLight->settings.light_size_count-pCONT_iLight->GetPixelsToUpdateAsNumberFromPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage);
//   Serial.printf("pixels_to_clear=%d\n\r",pixels_to_clear);
//   for(uint16_t index=0; index<pixels_to_clear; index++ ){
//     animation_colours[random(0,pCONT_iLight->settings.light_size_count)].DesiredColour = RgbColor(0);
//   }
//   // Get starting positions already on show
//   // UpdateStartingColourWithGetPixel();
//   // Call the animator to blend from previous to new
        
//   this->setAnimFunctionCallback(
//     [this](const AnimationParam& param){
//       this->AnimationProcess_Pulse_Random_On(param);
//     }
//   );

// }


// // 50% progress is full brightness, 0 and 100% is off
// void mAnimatorLight::AnimationProcess_Pulse_Random_On(const AnimationParam& param)
// {    

//   // float progress_percentage = param.progress*100; 
//   float progress_half_range = param.progress < 0.5 ? 
//                                   mSupport::mapfloat(param.progress, 0,0.5, 0,1) : 
//                                   mSupport::mapfloat(param.progress, 0.5,1, 1,0) ;

//   for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
//     RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
//         RgbColor(0),
//         animation_colours[pixel].DesiredColour,
//         progress_half_range);    //0-50%
//     SEGMENT.SetPixelColor(pixel, updatedColor);
//   } // END for

// }



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/


// void mAnimatorLight::SubTask_Flasher_Animate_Function_Pulse_Random_On_2(){

//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
  
//   flashersettings.flag_finish_flasher_pair = false;
//   flashersettings.flags.enable_random_rate = true;
  
//   do{ //must complete the pair together
//     switch(flashersettings.region){
//       case EFFECTS_REGION_COLOUR_SELECT_ID:{ //set colours
//         #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
//         AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "Random_On_2 EFFECTS_COLOUR_SELECT"));
//         #endif


//         // test_flag^=1;

//         // Go through strip, randomly change some to coloured or black
//         mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
//         uint8_t pixels = GetNumberOfColoursInPalette(mPaletteI->static_palettes.ptr);
//         RgbTypeColor colour_random = RgbTypeColor(0);

//         uint16_t random_pixel_index = 0;
        
//         for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
//           animation_colours[index].DesiredColour = SEGMENT.GetPixelColor(index);
//         }

//         // randomly blank most of them out again
//         uint8_t colour_or_black = 0;
//         uint16_t pixels_to_update = pCONT_iLight->animation.transition.pixels_to_update_as_number;
//         // pCONT_iLight->GetPixelsToUpdateAsNumberFromPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage);
//         Serial.printf("pixels_to_clear=%d\n\r",pixels_to_update);
//         for(uint16_t index=0; index<pixels_to_update; index++ ){
//           random_pixel_index = random(0,pCONT_iLight->settings.light_size_count);
//           colour_or_black = random(0,1);

//           // // Check if pixels is already on, if so, make dark
//           // if(pCONT_iLight->RgbColorto32bit(SEGMENT.GetPixelColor(random_pixel_index))>0){
//           //   animation_colours[random_pixel_index].DesiredColour = RgbColor(0,0,0);
//           // }else
//           // // Previously off, pick new state
//           // {
//             if(colour_or_black){ // 80% in favour of black/off
//               animation_colours[random_pixel_index].DesiredColour = RgbColor(0,0,0);
//             }else{
//               uint8_t desired_pixel = random(0,pixels-1);
//               colour_random = mPaletteI->GetColourFromPalette(mPaletteI->static_palettes.ptr,desired_pixel);  
//               // if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
//               //   colour_random = ApplyBrightnesstoDesiredColour(colour_random,pCONT_iLight->getBriRGB());
//               // }
//               animation_colours[random_pixel_index].DesiredColour =  colour_random;//RgbColor(0,0,255);//
//            }
//           // }
//         }
//         // if(test_flag){
//         //   animation_colours[random_pixel_index].DesiredColour =  RgbColor(0,0,255);
//         // }else{
//         //   animation_colours[random_pixel_index].DesiredColour =  RgbColor(0,255,0);
//         // }

//         // AddLog(LOG_LEVEL_TEST, PSTR("From G,B=%d,%d"),animation_colours[random_pixel_index].DesiredColour.G,animation_colours[random_pixel_index].DesiredColour.B);

//         // Get starting positions already on show
//         UpdateStartingColourWithGetPixel();
//         // AddLog(LOG_LEVEL_TEST, PSTR("From G,B=%d,%d"),animation_colours[random_pixel_index].StartingColor.G,animation_colours[random_pixel_index].StartingColor.B);


//         // dont automatically run animate again, add timeout for it
//         flashersettings.region = EFFECTS_REGION_ANIMATE_ID;

//       }break;
//       case EFFECTS_REGION_ANIMATE_ID: //shift along
//         #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
//         AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "Random_On_2 EFFECTS_ANIMATE"));
//         #endif

//         this->setAnimFunctionCallback(
//           [this](const AnimationParam& param){
//             this->AnimationProcess_Pulse_Random_On_2(param);
//           }
//         );

//         if(flashersettings.flags.force_finish_flasher_pair_once){
//           ALOG_DBM( PSTR("flashersettings.flags.force_finish_flasher_pair_once WAS SET"));
//           flashersettings.flags.force_finish_flasher_pair_once = false;
//         }
        
//         flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID;
        
//       break;
//     }
//   }while(flashersettings.flag_finish_flasher_pair || flashersettings.flags.force_finish_flasher_pair_once);

// }


// // 50% progress is full brightness, 0 and 100% is off
// void mAnimatorLight::AnimationProcess_Pulse_Random_On_2(const AnimationParam& param)
// {    
//  RgbTypeColor updatedColor;
//   for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
//     updatedColor = RgbTypeColor::LinearBlend(
//         animation_colours[pixel].StartingColor,
//         animation_colours[pixel].DesiredColour,
//         param.progress);    
//     SEGMENT.SetPixelColor(pixel, updatedColor);
//   } // END for
  
//   // AddLog(LOG_LEVEL_TEST, PSTR("-->> G,B=%d,%d"),updatedColor.G, updatedColor.B);

// }


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Christmas style "slow fade brightness"
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Saturation fade (all together)
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Saturation fade (wipe)
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Saturatoin fade (randomly selected)
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

// //to be merged with slow glow maybe? or advanced slow glow with colour and/or brightness are glows, equally, saturation can be manipulated.
// // A struct with what aspects of the "glows" should be could be idenitified here (brt, sat, colours, palettes)


// void mAnimatorLight::SubTask_Flasher_Animate_Function_Slow_Glow_On_Brightness(){

//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = false;

//   flashersettings.flag_finish_flasher_pair = false;
//   flashersettings.flags.enable_random_rate = true;

//   // Best "inorder" to compute colour again then brightness
  
//   do{ //must complete the pair together //move inside functions
//     switch(flashersettings.region){
//       case EFFECTS_REGION_COLOUR_SELECT_ID: //set colours
//         #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
//         AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
//         #endif
//         UpdateDesiredColourFromPaletteSelected();
//         flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
//       break;
//       case EFFECTS_REGION_ANIMATE_ID: //shift along
//         #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
//         //AddLog(LOG_LEVEL_DEBUG_MORE_LOWLEVEL,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//         #endif
//         // RotateDesiredColour(1,flashersettings.flags.movement_direction);

//         // Calculate new pattern to change
//         RefreshLEDIndexPattern();

//         // Global brightness is already applied, and will be known as "max range"
//         // Min range will be another map change here
//         uint8_t max_brightness = SEGMENT.rgbcct_controller->getBrightnessRGB_WithGlobalApplied();
//         uint8_t min_brightness = flashersettings.brightness_min;
//         uint8_t random_brightness = 0;

//         if(min_brightness > max_brightness){
//           min_brightness = max_brightness;
//         }

//         // Change only selected pixel brightness levels
//         for(ledout.index=0;ledout.index<strip_size_requiring_update;ledout.index++){
//           random_brightness = random(min_brightness,max_brightness);          
//           animation_colours[ledout.pattern[ledout.index]].DesiredColour = ApplyBrightnesstoRgbcctColour(
//                                                                               animation_colours[ledout.pattern[ledout.index]].DesiredColour,
//                                                                               random_brightness);
//         }

//         if(flashersettings.flags.force_finish_flasher_pair_once){
//           ALOG_DBM( PSTR("flashersettings.flags.force_finish_flasher_pair_once WAS SET"));
//           flashersettings.flags.force_finish_flasher_pair_once = false;
//         }
//       break;
//     }
//   }while(flashersettings.flag_finish_flasher_pair || flashersettings.flags.force_finish_flasher_pair_once);


// }


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/



// // Make this an "Effects" which is public/friendly of animator, but not the same class
// void mAnimatorLight::SubTask_Flasher_Animate_Function_SunPositions_SunRise_Alarm_01(){

  
//       AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SubTask_Flasher_Animate_Function_SunPositions_SunRise_Alarm_01"));

//   //using group 01
//   // struct{}
//   //use struct and memcpy to get data, then store results

//   //effects_data_buffer
//   //each needs 64 byte
// // typedef struct particle {
// //   // CRGB     color;
// //   uint32_t birth  =0;
// //   uint32_t last   =0;
// //   float    vel    =0;
// //   uint16_t pos    =-1;
// //   float    fragment[12];
// // } star;

// /**
//  * How the effects_data_buffer is formatted during effect runtime
//  * */
// struct EFFECT_CONFIG {
//   uint8_t starting_palette_id = 0;
//   uint8_t ending_palette_id = 10;
//   uint8_t step_changes_remaining = 10;
//   uint8_t colour_select_progress_counter = 0;
//   uint8_t active_palette_id;

// } effect_config;


//   // star* stars = reinterpret_cast<star*>(pCONT_iLight->effects_data_buffer);
//   EFFECT_CONFIG* effect_config2 = reinterpret_cast<EFFECT_CONFIG*>(pCONT_iLight->effects_data_buffer);
//   //should be memcpy be used to insure size if not exceeded? or sizeof check? pointer is faster




//   // switch(flashersettings.region){
//   //   case EFFECTS_REGION_COLOUR_SELECT_ID:{ //set colours


//       // if(effect_config2->active_palette_id++>mPaletteI->PALETTELIST_FIXED_GRADIENT_SUNLEVEL_GROUP01_07_ID){
//       //   effect_config2->active_palette_id = mPaletteI->PALETTELIST_FIXED_GRADIENT_SUNLEVEL_GROUP01_01_ID;
//       // }
//       if(pCONT_iLight->animation.palette.id++>=mPaletteI->PALETTELIST_FIXED_GRADIENT_SUNLEVEL_GROUP01_07_ID){
//         pCONT_iLight->animation.palette.id = mPaletteI->PALETTELIST_FIXED_GRADIENT_SUNLEVEL_GROUP01_01_ID;
//       }


// AddLog(LOG_LEVEL_TEST,PSTR("pCONT_iLight->animation.palette.id=%d"),pCONT_iLight->animation.palette.id);


//       UpdateDesiredColourFromPaletteSelected();



//       UpdateStartingColourWithGetPixel();



//     //   flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
//     // // NO 
//     // }break;
//     // case EFFECTS_REGION_ANIMATE_ID: //shift along
//       AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//       this->setAnimFunctionCallback([this](const AnimationParam& param){ this->AnimationProcess_SunPositions_Gradient_Alarm_01(param); });
      
//   //     flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID;
//   //     break;
//   // }

// }

// void mAnimatorLight::AnimationProcess_SunPositions_Gradient_Alarm_01(const AnimationParam& param)
// {   
//   // Need to make rgbcct pixels possible in the future, with dynamic animation_state

//    for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
//     RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
//         animation_colours[pixel].StartingColor,
//         animation_colours[pixel].DesiredColour,
//         param.progress);    
//     SEGMENT.SetPixelColor(pixel, updatedColor);
//   } // END for

// }



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/


// // Make this an "Effects" which is public/friendly of animator, but not the same class
// void mAnimatorLight::SubTask_Flasher_Animate_Function_SunPositions_Gradient_Sun_Elevation_And_Azimuth_01(){

  
//       AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SunPositions_Gradient_Sun_Elevation_And_Azimuth_01"));

//   //using group 01
//   // struct{}
//   //use struct and memcpy to get data, then store results

//   //effects_data_buffer
//   //each needs 64 byte
// // typedef struct particle {
// //   // CRGB     color;
// //   uint32_t birth  =0;
// //   uint32_t last   =0;
// //   float    vel    =0;
// //   uint16_t pos    =-1;
// //   float    fragment[12];
// // } star;

// /**
//  * How the effects_data_buffer is formatted during effect runtime
//  * */
// struct EFFECT_CONFIG {
//   uint16_t index_circle_center;
  
//   uint8_t sun_width;
//   uint8_t sun_bloom;
//   RgbcctColor sky_near_colour;
//   RgbcctColor sky_far_colour;

//   uint8_t value = 0;





//   uint8_t starting_palette_id;
//   uint8_t ending_palette_id;
//   uint8_t step_changes_remaining;
//   uint8_t colour_select_progress_counter;
//   uint8_t active_palette_id;
//   uint8_t sun_elevation_degrees;
//   uint8_t sun_azimuth_degrees; //left/right from sunrise/sunset position
//   // uint8_t sun_bloom; //how wide the main sunlight is
//   //store colours in here too
//   // uint8_t sun_elevation_degrees;


//   uint8_t direction;

//   //map "visible" azimuth into far left/right of strip

// } effect_config;


//   // star* stars = reinterpret_cast<star*>(pCONT_iLight->effects_data_buffer);
//   EFFECT_CONFIG* effect_config2 = reinterpret_cast<EFFECT_CONFIG*>(pCONT_iLight->effects_data_buffer);
//   //should be memcpy be used to insure size if not exceeded? or sizeof check? pointer is faster

//   AddLog(LOG_LEVEL_TEST,PSTR("pCONT_iLight->animation.palette.id=%d"),pCONT_iLight->animation.palette.id);

//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;

//   pCONT_iLight->settings.light_size_count = 100;
  
//   // Get all the current state
//   // for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
//   //   animation_colours[index].DesiredColour = SEGMENT.GetPixelColor(index);
//   // }
//     for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
//     animation_colours[index].DesiredColour = RgbcctColor(0);//,0,30,0,0);//,0,10,0,0);
//   }

//   //scan back and forth
//   // if(effect_config2->index_circle_center<=0){
//   //   effect_config2->direction = 1;
//   // }else
//   // if(effect_config2->index_circle_center>100){
//   //   effect_config2->direction = 0;
//   // }

//   // if(effect_config2->direction){          
//   //   effect_config2->index_circle_center++;
//   // }else{
//   //   effect_config2->index_circle_center--;
//   // }

//   effect_config2->index_circle_center = 50;
  
//   if(effect_config2->value >= 0){
//     // Draw full sky
//     Draw_DesiredColour_LinearGradientMirrored2(
//       RgbcctColor(0,0,50,0,0), //center
//       RgbcctColor(0,0,0,0,0), //circum
//       effect_config2->index_circle_center, //
//       50,  // tail_size
//       5, // center region
//       true
//     );
//   }


//   // widden
//   if(effect_config2->value >= 1){

//     // Draw near sky
//     Draw_DesiredColour_LinearGradientMirrored2(
//       RgbcctColor(20,5,0,0,0), //center
//       RgbcctColor(0,0,0,0,0), //circum
//       effect_config2->index_circle_center, //
//       25,  // tail_size
//       10, // center region
//       true
//     );
//   }

//   if(effect_config2->value >= 2){
//     // Draw near sky
//     Draw_DesiredColour_LinearGradientMirrored2(
//       RgbcctColor(100,20,0,0,0), //center
//       RgbcctColor(0,0,0,0,0), //circum
//       effect_config2->index_circle_center, //
//       30,  // tail_size
//       2, // center region
//       true
//     );
//   }

//   // widden
//   if(effect_config2->value >= 3){

//     // Draw near sky
//     Draw_DesiredColour_LinearGradientMirrored2(
//       RgbcctColor(0,50,50,0,0), //center
//       RgbcctColor(0,100,75,0,0), //circum
//       effect_config2->index_circle_center, //
//       60,  // tail_size
//       10, // center region
//       true
//     );
//   }


//   if(effect_config2->value >= 4){
//     // Draw sun bloom
//     Draw_DesiredColour_LinearGradientMirrored2(
//       RgbcctColor(200,100,0,0,0), //center
//       RgbcctColor(50, 44, 3,     0,0), //circum
//       effect_config2->index_circle_center, //D_RGB_ORANGE_WARM_1
//       50,
//       5, 
//       true
//     );
//   }

//   // if(effect_config2->value == 4){
//   //   // Draw sun bloom
//   //   Remove_DesiredColour_LinearGradientMirrored(
//   //     RgbcctColor(0,0,  50,       0,0), //center
//   //     RgbcctColor(0, 0, 10,     0,0), //circum
//   //     effect_config2->index_circle_center, //D_RGB_ORANGE_WARM_1
//   //     50,
//   //     5, 
//   //     true
//   //   );
//   // }


  
//   if(effect_config2->value++ > 6){ effect_config2->value = 0; }

//         // pixel_radius_as_number = 10;
//         // pixel_position_center = effect_config2->index_circle_center;
//         // pixel_position_circum = effect_config2->index_circle_center-pixel_radius_as_number;
//         // for(uint16_t index=0; index<pixel_radius_as_number; index++ ){
//         //   // Create new index
//         //   index_generate = effect_config2->index_circle_center-index;              
//         //   //Only write into colour buffer if valid index. Otherwise, consider this an "overscan"
//         //   if((index_generate>=0)&&(index_generate<100)){
//         //     //Draw half, larger index side
//         //     progress = mSupport::mapfloat(index_generate,
//         //                                     pixel_position_center,pixel_position_circum,
//         //                                     0,1
//         //                                   );
//         //     animation_colours[index_generate].DesiredColour = RgbcctColor::LinearBlend(
//         //                                                                                 colour_circle_center,
//         //                                                                                 colour_circle_circumference, 
//         //                                                                                 //RgbColor(255,0,0), RgbColor(0,255,0)
//         //                                                                                 progress);
//         //   } // if test

//         // }





// // //use signed value to hold index_adjusted
// // int32_t index_generate = 0;
// // uint16_t pixel_position_center, pixel_position_circum, pixel_radius_as_number;

// //         pixel_radius_as_number = 10;
// //         pixel_position_center = effect_config2->index_circle_center;
// //         pixel_position_circum = effect_config2->index_circle_center+pixel_radius_as_number;
// //         for(uint16_t index=0; index<pixel_radius_as_number; index++ ){
// //           // Create new index
// //           index_generate = effect_config2->index_circle_center+index;   
          
// //           //Draw half, larger index side
// //           progress = mSupport::mapfloat(index_generate,
// //                                           pixel_position_center,pixel_position_circum,
// //                                           0,1
// //                                         );
// //           colour_draw = RgbcctColor::LinearBlend(
// //                                         colour_circle_center,
// //                                         colour_circle_circumference, 
// //                                         //RgbColor(255,0,0), RgbColor(0,255,0)
// //                                         progress);



// //           //Only write into colour buffer if valid index. Otherwise, consider this an "overscan"
// //           if((index_generate>=0)&&(index_generate<100)){
// //             animation_colours[index_generate].DesiredColour = colour_draw;
// //           } // if test

// //         }

// //         pixel_radius_as_number = 10;
// //         pixel_position_center = effect_config2->index_circle_center;
// //         pixel_position_circum = effect_config2->index_circle_center-pixel_radius_as_number;
// //         for(uint16_t index=0; index<pixel_radius_as_number; index++ ){
// //           // Create new index
// //           index_generate = effect_config2->index_circle_center-index;              
// //           //Only write into colour buffer if valid index. Otherwise, consider this an "overscan"
// //           if((index_generate>=0)&&(index_generate<100)){
// //             //Draw half, larger index side
// //             progress = mSupport::mapfloat(index_generate,
// //                                             pixel_position_center,pixel_position_circum,
// //                                             0,1
// //                                           );
// //             animation_colours[index_generate].DesiredColour = RgbcctColor::LinearBlend(
// //                                                                                         colour_circle_center,
// //                                                                                         colour_circle_circumference, 
// //                                                                                         //RgbColor(255,0,0), RgbColor(0,255,0)
// //                                                                                         progress);
// //           } // if test

// //         }





//           // index_generate = effect_config2->index_circle_center-index+10;
//           // start_pixel_position = index_generate;
//           // end_pixel_position = index_generate+10;
//           // if((index_generate>=0)&&(index_generate<100)){
//           //   //Draw half, samller index side
//           //   progress = mSupport::mapfloat(index_generate,start_pixel_position,end_pixel_position,0,1);
//           //   animation_colours[index_generate].DesiredColour = RgbcctColor::LinearBlend(RgbColor(255,0,255), RgbColor(0,0,255), progress);
              
              
//           //     // colour_circle_circumference, colour_circle_center, progress);
//           // }// out of range

//         // }


//         // randomly blank most of them out again
//         // uint8_t colour_or_black = 0;
//         // uint16_t pixels_to_update = pCONT_iLight->animation.transition.pixels_to_update_as_number;
//         // // pCONT_iLight->GetPixelsToUpdateAsNumberFromPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage);
//         // Serial.printf("pixels_to_clear=%d\n\r",pixels_to_update);
//         // for(uint16_t index=0; index<pixels_to_update; index++ ){
//         //   random_pixel_index = random(0,pCONT_iLight->settings.light_size_count);
//         //   colour_or_black = random(0,1);

//         //   // // Check if pixels is already on, if so, make dark
//         //   // if(pCONT_iLight->RgbColorto32bit(SEGMENT.GetPixelColor(random_pixel_index))>0){
//         //   //   animation_colours[random_pixel_index].DesiredColour = RgbColor(0,0,0);
//         //   // }else
//         //   // // Previously off, pick new state
//         //   // {
//         //     if(colour_or_black){ // 80% in favour of black/off
//         //       animation_colours[random_pixel_index].DesiredColour = RgbColor(0,255,0);
//         //     }else{
//         //       uint8_t desired_pixel = random(0,pixels-1);
//         //       colour_random = mPaletteI->GetColourFromPalette(mPaletteI->static_palettes.ptr,desired_pixel);  
//         //       // if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
//         //       //   colour_random = ApplyBrightnesstoDesiredColour(colour_random,pCONT_iLight->getBriRGB());
//         //       // }
//         //       animation_colours[random_pixel_index].DesiredColour =  colour_random;//RgbColor(0,0,255);//
//         //    }
//         //   // }
//         // }




//       // UpdateDesiredColourFromPaletteSelected();


//       UpdateStartingColourWithGetPixel();



//     //   flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
//     // // NO 
//     // }break;
//     // case EFFECTS_REGION_ANIMATE_ID: //shift along
//       AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//       this->setAnimFunctionCallback([this](const AnimationParam& param){ this->AnimationProcess_SunPositions_Gradient_Sun_Elevation_And_Azimuth_01(param); });
      
//   //     flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID;
//   //     break;
//   // }

// }

// void mAnimatorLight::AnimationProcess_SunPositions_Gradient_Sun_Elevation_And_Azimuth_01(const AnimationParam& param)
// {   
//   // Need to make rgbcct pixels possible in the future, with dynamic animation_state

//    for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
//     RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
//         animation_colours[pixel].StartingColor,
//         animation_colours[pixel].DesiredColour,
//         param.progress);    
//     SEGMENT.SetPixelColor(pixel, updatedColor);
//   } // END for

// }




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/


// /**************************************************************************************************************************************************************
//  * @brief  Solid_Colour_Based_On_Sun_Elevation_02
//  * @note   From -10 to noon, CCT will range from yellow to daywhite
//  * @note   From -5 to dusk, blue will go from 0 to max_brightness 
//  * 
//  * @note   Gloabl brightness will be manual, or controlled indirectly eg via mqtt
//  * 
//  * Instead of direct Elevation to palette, this animation will perform different versions of the mapping. Should this be one animation with subtypes? or different types
//  * 
//  * @note   Using RgbcctColour palette that is designed for each point in elevation
//  * *************************************************************************************************************************************************************/

// void mAnimatorLight::SubTask_Flasher_Animate_Function_SunPositions_Elevation_Only_RGBCCT_Palette_Indexed_Positions_With_Augmented_01()
// {

//    AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_02"));

//    /**
//     * Setting that an mqtt debug exists
//     * */
//    pCONT_iLight->animation.debug_mqtt_response_available = 1;

//   #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
//   if(pCONT_iLight->animation.debug_mqtt_response_available && pCONT_iLight->animation.flags.animator_first_run)
//   {
//     this->setCallback_ConstructJSONBody_Debug_Animations_Progress(
//       [this](void){
//         this->ConstructJSONBody_Animation_Progress__SunPositions_Elevation_Only_RGBCCT_Palette_Indexed_Positions_With_Augmented_01();
//       }
//     );
//   //   AddLog(LOG_LEVEL_INFO, PSTR("ConstructJSONBody_Animation_Progress__SunPositions_Elevation_Only_RGBCCT_Palette_Indexed_Positions_With_Augmented_01 FIRST RUN"));
//   //   delay(5000);
//   // }else{
//   //   AddLog(LOG_LEVEL_INFO, PSTR("NOT SETTING"));

//   }
//   #endif // USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
//   // delay(5000);

//   // pCONT_iLight->animation.palette.id = mPaletteI->PALETTELIST_FIXED_SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01_ID;

//   // Set palette pointer
//   mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
//   // Brightness is generated internally, and rgbcct solid palettes are output values

//   /**
//    * Solar data to use, defined here for testing or simulations
//    * */
 
// float sun_elevation = 0;
// #ifdef USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
//   #ifdef USE_DEVFEATURE_SUNPOSITION_ELEVATION_USE_TESTING_VALUE
//   sun_elevation = (float)pCONT_solar->solar_position_testing.elevation;
//   #else
//   sun_elevation = (float)pCONT_solar->solar_position.elevation;
//   #endif
// #endif
//   bool sun_is_ascending = true;//pCONT_solar->solar_position_testing.direction.is_ascending;
//   // Serial.printf("\n\r\n\rsun_elevation\t => %f\n\r", sun_elevation);

//   // delay(1000);

//   /**
//    * Sun elevation indexing is stored in palettes index location.
//    * The current sun elevation shall be searched against for nearest match, then depending on accesending or decending sun the nearest match and nearest next match will be linearblended as current show colour
//    * */

//   /**
//    * Get total pixels in palette
//    * */
//   mPalette::PALETTELIST::PALETTE *palette_p = mPaletteI->GetPalettePointerByID(pCONT_iLight->animation.palette.id);
//   uint8_t pixels_max = GetNumberOfColoursInPalette(palette_p);
//   // AddLog(LOG_LEVEL_INFO,PSTR("pixels_max=%d"),pixels_max);

//   // Lets assume we need a zero crossing index, thus, we can use it to identity AS and DE modes
//   uint8_t zero_crossing_index = 0;

//   struct INDEXES_MATCHES{
//     uint8_t previous = 0; //ie colour moving away from
//     uint8_t next = 0; //colour moving towards
//   }index;

//   /**
//    * Steps to finding index
//    * 1) Find the zero-crossing index from the palette (ie the colour where its index is 0)
//    * 2) Decide if elevation is pos or neg, begin searching that part of the array
//    * 3) Find index of closest in array
//    * 4) Next and previous index will depend on direction of sun, and will be equal to current index if error is exactly 0
//    * */

//   /**
//    * Step X: Find zero crossing point
//    * Step X: Find all differences
//    * */
//   int16_t indexing = 0;  
//   uint8_t lower_boundary_index = 13;
//   float lower_boundary_value = 45;
//   uint8_t upper_boundary_index = 14;  
//   float upper_boundary_value = 90;
//   float sun_positions_from_palette_index[pixels_max];  
//   uint8_t searching_matched_index = 0;

//   /**
//    * Ascending method for finding right region between points
//    * Check all, but once sun_elev is greater, then thats the current region
//    * */
//   for(int i=0;i<pixels_max;i++)
//   {
//     mPaletteI->GetColourFromPalette(palette_p, i, &indexing);
//     sun_positions_from_palette_index[i] = indexing - 90;
//     // Serial.printf("sun_pos=[%02d]=\t%f\n\r", i, sun_positions_from_palette_index[i]);
//   }


//   for(int i=0;i<pixels_max;i++)
//   {
//     // Serial.printf("sun=%f > index[%d]=%f\n\r", sun_elevation, i, sun_positions_from_palette_index[i]);
//     if(sun_elevation >= sun_positions_from_palette_index[i])
//     {
      
//       // searching_matched_index = i;
//       // Serial.printf("sun=%f > index[%d]=%f   MATCH=%d\n\r", 
//       //   sun_elevation, i, sun_positions_from_palette_index[i], searching_matched_index
//       // );
//       //Serial.printf("Still less\n\r");

//     }else{
      
//       searching_matched_index = i-1;
//       // Serial.printf("sun=%f > index[%d]=%f   MATCH=%d\n\r", 
//       //   sun_elevation, i, sun_positions_from_palette_index[i], searching_matched_index
//       // );
//       // Serial.printf("searching_matched_index = %d\n\r", searching_matched_index);
//       break;

//     }

//     // Directly, manually, check the last memory space

//     if(sun_elevation == sun_positions_from_palette_index[pixels_max-1])
//     {
//       searching_matched_index = i-1;
//       // Serial.printf("sun=%f > index[%d]=%f   MATCH=%d\n\r", 
//       //   sun_elevation, i, sun_positions_from_palette_index[i], searching_matched_index
//       // );
//       break;

//     }




//   }

//   lower_boundary_index = searching_matched_index;
//   upper_boundary_index = searching_matched_index+1;

//   /**
//    * Check ranges are valid, if not, reset to 0 and 1
//    * */
//   if(lower_boundary_index>=pixels_max)
//   {
//     lower_boundary_index = 0;
//     Serial.printf("lower_boundary_index>=pixels_max\n\r");
//   }
//   if(upper_boundary_index>=pixels_max)
//   {
//     upper_boundary_index = pixels_max;
//     Serial.printf("upper_boundary_index>=pixels_max\n\r");
//   }

//   lower_boundary_value = sun_positions_from_palette_index[lower_boundary_index];
//   upper_boundary_value = sun_positions_from_palette_index[upper_boundary_index];


//   float numer = sun_elevation        - lower_boundary_value;
//   float denum = upper_boundary_value - lower_boundary_value;
//   float progress_between_colours = numer/denum;

//   Serial.printf("\n\r\n\r\n\rsun_elevation\t => %f\n\r", sun_elevation);
//   Serial.printf("lower_boundary_value[%02d]=%f\n\r", lower_boundary_index, lower_boundary_value);
//   Serial.printf("upper_boundary_value[%02d]=%f\n\r", upper_boundary_index, upper_boundary_value);
//   Serial.printf("numer=\t%f\n\r",numer);
//   Serial.printf("denum=\t%f\n\r",denum);
//   Serial.printf("progress_between_colours=\t%f\n\r",progress_between_colours);

//   /**
//    * Showing the colours
//    * 1) previous
//    * 2) next
//    * 3) linearblend of the exact new colour
//    * */

//   RgbcctColor c_lower = mPaletteI->GetColourFromPalette(palette_p, lower_boundary_index);
//   RgbcctColor c_upper = mPaletteI->GetColourFromPalette(palette_p, upper_boundary_index);

//   // Serial.printf("progress_between_colours\t %f(%d)/%f(%d) => %f\n\r", 
//   //   lower_boundary_value, lower_boundary_index, 
//   //   upper_boundary_value, upper_boundary_index, progress_between_colours);

//   RgbcctColor c_blended = RgbcctColor::LinearBlend(c_lower, c_upper, progress_between_colours);

//   RgbcctColor c = c_lower; 
//   // AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_p\t%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WW,c.WC);
//   c = c_blended; 
//   // AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_b\t%d,%d,%d,%d,%d (progress %d"),c.R,c.G,c.B,c.WW,c.WC, (int)(progress_between_colours*100));
//   c = c_upper; 
//   // AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_n\t%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WW,c.WC);

//   /**
//    * Load new colour into animation
//    * */

//   pCONT_iLight->animation.flags.fForceUpdate = true;

//   animation_colours_rgbcct.DesiredColour  = c_blended;


//   /**
//    * Evening/Sunset, keep the brightness up
//    * Morning/Sunrise, wait until closer before increasing brighntess (adjust by elevation_adjust or by time_adjust)
//    * Instead of elevation mapping, use sunrise and sunset time to control animations
//    * */


//   // AddLog(LOG_LEVEL_TEST, PSTR("DesiredColour1=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   if(!SEGMENT.rgbcct_controller->getApplyBrightnessToOutput())
//   { // If not already applied, do it using global values
//     animation_colours_rgbcct.DesiredColour = ApplyBrightnesstoRgbcctColour(
//       animation_colours_rgbcct.DesiredColour, 
//       SEGMENT.rgbcct_controller->getBrightnessRGB_WithGlobalApplied(),
//       SEGMENT.rgbcct_controller->getBrightnessCCT255()
//     );
//   }

//   animation_colours_rgbcct.StartingColor = SEGMENT.GetPixelColor();

//   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//   this->setAnimFunctionCallback([this](const AnimationParam& param){
//       this->AnimationProcess_Generic_RGBCCT_Single_Colour_All(param); });

//   pCONT_iLight->animation.flags.animator_first_run = false; // process completed, so lets not redo things above
   
// }



// void mAnimatorLight::ConstructJSONBody_Animation_Progress__SunPositions_Elevation_Only_RGBCCT_Palette_Indexed_Positions_With_Augmented_01()
// {   
// // Serial.println("AnimationProcess_Generic_AnimationColour_LinearBlend");
//   // for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
//   //   RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
//   //       animation_colours[pixel].StartingColor,
//   //       animation_colours[pixel].DesiredColour,
//   //       param.progress);    
//   //   SEGMENT.SetPixelColor(pixel, updatedColor);
//   // } // END for

//   // DEBUG_LINE_HERE;

//   JBI->Add("test2","debug2");

//     // SEGMENT.SetPixelColor(0, RgbColor(0,random(0,255),0));
// }

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/




// // Make this an "Effects" which is public/friendly of animator, but not the same class
// void mAnimatorLight::SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_01(){

  
//       AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_01"));

//   //using group 01
//   // struct{}
//   //use struct and memcpy to get data, then store results

//   //effects_data_buffer
//   //each needs 64 byte
// // typedef struct particle {
// //   // CRGB     color;
// //   uint32_t birth  =0;
// //   uint32_t last   =0;
// //   float    vel    =0;
// //   uint16_t pos    =-1;
// //   float    fragment[12];
// // } star;

// /**
//  * How the effects_data_buffer is formatted during effect runtime
//  * */
// struct EFFECT_CONFIG {
//   uint16_t index_circle_center;
  
//   uint8_t sun_width;
//   uint8_t sun_bloom;
//   RgbcctColor sky_near_colour;
//   RgbcctColor sky_far_colour;

//   uint8_t value = 0;





//   uint8_t starting_palette_id;
//   uint8_t ending_palette_id;
//   uint8_t step_changes_remaining;
//   uint8_t colour_select_progress_counter;
//   uint8_t active_palette_id;
//   uint8_t sun_elevation_degrees;
//   uint8_t sun_azimuth_degrees; //left/right from sunrise/sunset position
//   // uint8_t sun_bloom; //how wide the main sunlight is
//   //store colours in here too
//   // uint8_t sun_elevation_degrees;


//   uint8_t direction;

//   //map "visible" azimuth into far left/right of strip

// } effect_config;

// float sun_elevation = 0;
// #ifdef USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
//   #ifdef USE_DEVFEATURE_SUNPOSITION_ELEVATION_USE_TESTING_VALUE
//   sun_elevation = (float)pCONT_solar->solar_position_testing.elevation;
//   #else
//   sun_elevation = (float)pCONT_solar->solar_position.elevation;
//   #endif
// #endif


//   // star* stars = reinterpret_cast<star*>(pCONT_iLight->effects_data_buffer);
//   EFFECT_CONFIG* effect_config2 = reinterpret_cast<EFFECT_CONFIG*>(pCONT_iLight->effects_data_buffer);
//   //should be memcpy be used to insure size if not exceeded? or sizeof check? pointer is faster

//   AddLog(LOG_LEVEL_TEST,PSTR("pCONT_iLight->animation.palette.id=%d"),pCONT_iLight->animation.palette.id);


//   // pCONT_iLight->settings.light_size_count = 1;
  
//   // Get all the current state
//   // for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
//   //   animation_colours[index].DesiredColour = SEGMENT.GetPixelColor(index);
//   // }
//   //   for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
//   //   animation_colours[index].DesiredColour = RgbcctColor(9,255,0,0,0);//,0,30,0,0);//,0,10,0,0);
//   // }


//   /**
//    * Colour temp of white channels when day time
//    * Amount of blue when sun is down
//    * */

//   if(sun_elevation)
//   {
//     /**
//      * Max daily elevation will need to be known later
//      * */
//     // Mapping elevation to CCT percentage

//     // pCONT_iLight->CommandSet_ActiveSolidPalette_ColourTemp_Percentage(percentage_cct);

//   }else{
    
//     for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
//       animation_colours[index].DesiredColour = RgbcctColor(0,0,255,0,0);//,0,30,0,0);//,0,10,0,0);
//     }
//   }

// /**
//  * colour channels
//  * */

//   if((sun_elevation>-50)&&(sun_elevation<10))
//   {
//     // HsbColor hsb = HsbColor(RgbColor(0));
//     // hsb.H = map(sun_elevation,-50,10,255,0)

//     uint8_t blue =  map(sun_elevation,-50,10,255,0);

//     SEGMENT.rgbcct_controller->setRGB(0,0,blue);

//     // AddLog(LOG_LEVEL_INFO, PSTR("elevation=%d, cct_temp=%d %d"),(int)sun_elevation, elev_perc, cct_val);




//     uint8_t brightness_255 = map(sun_elevation,-50,10,255,0);

//     SEGMENT.rgbcct_controller->setBrightnessRGB255(brightness_255);

//   }

// /**
//  * WHITE CHANNEL
//  * */
//   if((sun_elevation>-10)&&(sun_elevation<25))
//   {


//     // Convert elevation into percentage
//     uint8_t elev_perc = map(sun_elevation,-10,25,0,100);
//     // Convert percentage into cct
//     uint16_t cct_val = mapvalue(elev_perc, 0,100, pCONT_iLight->get_CTRangeMin(),pCONT_iLight->get_CTRangeMax());
//     // Set the colour temp
//     SEGMENT.rgbcct_controller->setCCT(cct_val);


//     uint8_t brightness_255 = map(sun_elevation,-10,25,0,255);

//     SEGMENT.rgbcct_controller->setBrightnessCCT255(brightness_255);

//   }




//   //scan back and forth
//   // if(effect_config2->index_circle_center<=0){
//   //   effect_config2->direction = 1;
//   // }else
//   // if(effect_config2->index_circle_center>100){
//   //   effect_config2->direction = 0;
//   // }

//   // if(effect_config2->direction){          
//   //   effect_config2->index_circle_center++;
//   // }else{
//   //   effect_config2->index_circle_center--;
//   // }

//   effect_config2->index_circle_center = 50;
  
//   // if(effect_config2->value >= 0){
//   //   // Draw full sky
//   //   Draw_DesiredColour_LinearGradientMirrored2(
//   //     RgbcctColor(0,0,50,0,0), //center
//   //     RgbcctColor(0,0,0,0,0), //circum
//   //     effect_config2->index_circle_center, //
//   //     50,  // tail_size
//   //     5, // center region
//   //     true
//   //   );
//   // }


//   // // widden
//   // if(effect_config2->value >= 1){

//   //   // Draw near sky
//   //   Draw_DesiredColour_LinearGradientMirrored2(
//   //     RgbcctColor(20,5,0,0,0), //center
//   //     RgbcctColor(0,0,0,0,0), //circum
//   //     effect_config2->index_circle_center, //
//   //     25,  // tail_size
//   //     10, // center region
//   //     true
//   //   );
//   // }

//   // if(effect_config2->value >= 2){
//   //   // Draw near sky
//   //   Draw_DesiredColour_LinearGradientMirrored2(
//   //     RgbcctColor(100,20,0,0,0), //center
//   //     RgbcctColor(0,0,0,0,0), //circum
//   //     effect_config2->index_circle_center, //
//   //     30,  // tail_size
//   //     2, // center region
//   //     true
//   //   );
//   // }

//   // // widden
//   // if(effect_config2->value >= 3){

//   //   // Draw near sky
//   //   Draw_DesiredColour_LinearGradientMirrored2(
//   //     RgbcctColor(0,50,50,0,0), //center
//   //     RgbcctColor(0,100,75,0,0), //circum
//   //     effect_config2->index_circle_center, //
//   //     60,  // tail_size
//   //     10, // center region
//   //     true
//   //   );
//   // }


//   // if(effect_config2->value >= 4){
//   //   // Draw sun bloom
//   //   Draw_DesiredColour_LinearGradientMirrored2(
//   //     RgbcctColor(200,100,0,0,0), //center
//   //     RgbcctColor(50, 44, 3,     0,0), //circum
//   //     effect_config2->index_circle_center, //D_RGB_ORANGE_WARM_1
//   //     50,
//   //     5, 
//   //     true
//   //   );
//   // }

//   // if(effect_config2->value == 4){
//   //   // Draw sun bloom
//   //   Remove_DesiredColour_LinearGradientMirrored(
//   //     RgbcctColor(0,0,  50,       0,0), //center
//   //     RgbcctColor(0, 0, 10,     0,0), //circum
//   //     effect_config2->index_circle_center, //D_RGB_ORANGE_WARM_1
//   //     50,
//   //     5, 
//   //     true
//   //   );
//   // }


  
//   if(effect_config2->value++ > 6){ effect_config2->value = 0; }

//         // pixel_radius_as_number = 10;
//         // pixel_position_center = effect_config2->index_circle_center;
//         // pixel_position_circum = effect_config2->index_circle_center-pixel_radius_as_number;
//         // for(uint16_t index=0; index<pixel_radius_as_number; index++ ){
//         //   // Create new index
//         //   index_generate = effect_config2->index_circle_center-index;              
//         //   //Only write into colour buffer if valid index. Otherwise, consider this an "overscan"
//         //   if((index_generate>=0)&&(index_generate<100)){
//         //     //Draw half, larger index side
//         //     progress = mSupport::mapfloat(index_generate,
//         //                                     pixel_position_center,pixel_position_circum,
//         //                                     0,1
//         //                                   );
//         //     animation_colours[index_generate].DesiredColour = RgbcctColor::LinearBlend(
//         //                                                                                 colour_circle_center,
//         //                                                                                 colour_circle_circumference, 
//         //                                                                                 //RgbColor(255,0,0), RgbColor(0,255,0)
//         //                                                                                 progress);
//         //   } // if test

//         // }





// // //use signed value to hold index_adjusted
// // int32_t index_generate = 0;
// // uint16_t pixel_position_center, pixel_position_circum, pixel_radius_as_number;

// //         pixel_radius_as_number = 10;
// //         pixel_position_center = effect_config2->index_circle_center;
// //         pixel_position_circum = effect_config2->index_circle_center+pixel_radius_as_number;
// //         for(uint16_t index=0; index<pixel_radius_as_number; index++ ){
// //           // Create new index
// //           index_generate = effect_config2->index_circle_center+index;   
          
// //           //Draw half, larger index side
// //           progress = mSupport::mapfloat(index_generate,
// //                                           pixel_position_center,pixel_position_circum,
// //                                           0,1
// //                                         );
// //           colour_draw = RgbcctColor::LinearBlend(
// //                                         colour_circle_center,
// //                                         colour_circle_circumference, 
// //                                         //RgbColor(255,0,0), RgbColor(0,255,0)
// //                                         progress);



// //           //Only write into colour buffer if valid index. Otherwise, consider this an "overscan"
// //           if((index_generate>=0)&&(index_generate<100)){
// //             animation_colours[index_generate].DesiredColour = colour_draw;
// //           } // if test

// //         }

// //         pixel_radius_as_number = 10;
// //         pixel_position_center = effect_config2->index_circle_center;
// //         pixel_position_circum = effect_config2->index_circle_center-pixel_radius_as_number;
// //         for(uint16_t index=0; index<pixel_radius_as_number; index++ ){
// //           // Create new index
// //           index_generate = effect_config2->index_circle_center-index;              
// //           //Only write into colour buffer if valid index. Otherwise, consider this an "overscan"
// //           if((index_generate>=0)&&(index_generate<100)){
// //             //Draw half, larger index side
// //             progress = mSupport::mapfloat(index_generate,
// //                                             pixel_position_center,pixel_position_circum,
// //                                             0,1
// //                                           );
// //             animation_colours[index_generate].DesiredColour = RgbcctColor::LinearBlend(
// //                                                                                         colour_circle_center,
// //                                                                                         colour_circle_circumference, 
// //                                                                                         //RgbColor(255,0,0), RgbColor(0,255,0)
// //                                                                                         progress);
// //           } // if test

// //         }





//           // index_generate = effect_config2->index_circle_center-index+10;
//           // start_pixel_position = index_generate;
//           // end_pixel_position = index_generate+10;
//           // if((index_generate>=0)&&(index_generate<100)){
//           //   //Draw half, samller index side
//           //   progress = mSupport::mapfloat(index_generate,start_pixel_position,end_pixel_position,0,1);
//           //   animation_colours[index_generate].DesiredColour = RgbcctColor::LinearBlend(RgbColor(255,0,255), RgbColor(0,0,255), progress);
              
              
//           //     // colour_circle_circumference, colour_circle_center, progress);
//           // }// out of range

//         // }


//         // randomly blank most of them out again
//         // uint8_t colour_or_black = 0;
//         // uint16_t pixels_to_update = pCONT_iLight->animation.transition.pixels_to_update_as_number;
//         // // pCONT_iLight->GetPixelsToUpdateAsNumberFromPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage);
//         // Serial.printf("pixels_to_clear=%d\n\r",pixels_to_update);
//         // for(uint16_t index=0; index<pixels_to_update; index++ ){
//         //   random_pixel_index = random(0,pCONT_iLight->settings.light_size_count);
//         //   colour_or_black = random(0,1);

//         //   // // Check if pixels is already on, if so, make dark
//         //   // if(pCONT_iLight->RgbColorto32bit(SEGMENT.GetPixelColor(random_pixel_index))>0){
//         //   //   animation_colours[random_pixel_index].DesiredColour = RgbColor(0,0,0);
//         //   // }else
//         //   // // Previously off, pick new state
//         //   // {
//         //     if(colour_or_black){ // 80% in favour of black/off
//         //       animation_colours[random_pixel_index].DesiredColour = RgbColor(0,255,0);
//         //     }else{
//         //       uint8_t desired_pixel = random(0,pixels-1);
//         //       colour_random = mPaletteI->GetColourFromPalette(mPaletteI->static_palettes.ptr,desired_pixel);  
//         //       //   colour_random = ApplyBrightnesstoDesiredColour(colour_random,pCONT_iLight->getBriRGB());
//         //       animation_colours[random_pixel_index].DesiredColour =  colour_random;//RgbColor(0,0,255);//
//         //    }
//         //   // }
//         // }

// pCONT_iLight->animation.palette.id = 10;

//  // AddLog(LOG_LEVEL_TEST, PSTR("SubTask_Flasher_Animate_Function__Solid_Static_Single_Colour"));
//   // Set palette pointer
//   mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
//   // Set up colours
//   // Brightness is generated internally, and rgbcct solid palettes are output values
//   animation_colours_rgbcct.DesiredColour  = mPaletteI->GetColourFromPalette(mPaletteI->static_palettes.ptr);

//   // AddLog(LOG_LEVEL_TEST, PSTR("DesiredColour1=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   if(!pCONT_iLight->rgbcct_controller.getApplyBrightnessToOutput()){ // If not already applied, do it using global values
//     animation_colours_rgbcct.DesiredColour = ApplyBrightnesstoRgbcctColour(
//       animation_colours_rgbcct.DesiredColour, 
//       pCONT_iLight->rgbcct_controller.getBrightnessRGB_WithGlobalApplied(),
//       pCONT_iLight->rgbcct_controller.getBrightnessCCT255()
//     );
//   }

//   // AddLog(LOG_LEVEL_TEST, PSTR("DesiredColour2=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   animation_colours_rgbcct.StartingColor = SEGMENT.GetPixelColor();
//   // GetPixelColourHardwareInterface();

//   // AddLog(LOG_LEVEL_TEST, PSTR("StartingColour2=%d,%d,%d,%d,%d"), animation_colours_rgbcct.StartingColor.R,animation_colours_rgbcct.StartingColor.G,animation_colours_rgbcct.StartingColor.B,animation_colours_rgbcct.StartingColor.WC,animation_colours_rgbcct.StartingColor.WW);
    

//   // // Call the animator to blend from previous to new
//   // this->setAnimFunctionCallback(
//   //   [this](const AnimationParam& param){
//   //     this->AnimationProcess_BlendStartingToDesiredColour_SOLID_COLOUR(param);
//   //   }
//   // );


//   //     // UpdateDesiredColourFromPaletteSelected();


//   //     UpdateStartingColourWithGetPixel();



//     //   flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
//     // // NO 
//     // }break;
//     // case EFFECTS_REGION_ANIMATE_ID: //shift along
//       AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//       this->setAnimFunctionCallback([this](const AnimationParam& param){ 
//         this->AnimationProcess_Generic_RGBCCT_Single_Colour_All(param); });
      
//   //     flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID;
//   //     break;
//   // }

// }

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/


// /**************************************************************************************************************************************************************
//  * @brief  Solid_Colour_Based_On_Sun_Elevation_02
//  * @note   From -10 to noon, CCT will range from yellow to daywhite
//  * @note   From -5 to dusk, blue will go from 0 to max_brightness 
//  * 
//  * @note   Gloabl brightness will be manual, or controlled indirectly eg via mqtt
//  * *************************************************************************************************************************************************************/

// void mAnimatorLight::SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_02()
// {
 
//   // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_02"));

//   // AddLog(LOG_LEVEL_TEST,PSTR("pCONT_iLight->animation.palette.id=%d"),pCONT_iLight->animation.palette.id);

// /**
//  * 
//  * 
//  * */


//   // pCONT_iLight->CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(10);

//   /**
//    * Solar data to use, defined here for testing or simulations
//    * */
// float sun_elevation = 0;
// #ifdef USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
//   #ifdef USE_DEVFEATURE_SUNPOSITION_ELEVATION_USE_TESTING_VALUE
//   sun_elevation = (float)pCONT_solar->solar_position_testing.elevation;
//   #else
//   sun_elevation = (float)pCONT_solar->solar_position.elevation;
//   #endif
// #endif
  
//   // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "sun_elevation=%d"), (int)sun_elevation);

//   // struct COLOUR_REGIONS{
//   //   uint8_t rgb_
//   // }

//   // uint8_t

//   /**
//    * Generate the rgb channels
//    * */
//   // Add yellow to red hue, with max to lowering rgbbrightness
//   if(sun_elevation>10)
//   {
//     pCONT_iLight->rgbcct_controller.setRGB(0,255,0);    

//   }else
//   if(
//     (sun_elevation<10)&&
//     (sun_elevation>-5)
//     )
//   {
//     HsbColor hsb = HsbColor(RgbColor(0));
//     hsb.H = pCONT_iLight->HUE_N2F(mapvalue(sun_elevation,10,-5,35,0)); // yellow to red
//     hsb.S = 1;

//     /**
//      * Calculate brightness gradient
//      * */
//     hsb.B = mapvalue(sun_elevation,10,-5,0.0f,1.0f);

//     uint8_t brightness_255 = map(sun_elevation,10,-5,0,255);
//     pCONT_iLight->rgbcct_controller.setBrightnessRGB255(brightness_255);


//     pCONT_iLight->rgbcct_controller.setHsbColor(hsb);

//     // uint8_t blue =  map(sun_elevation,-50,-5,255,0);

//     // pCONT_iLight->rgbcct_controller.setRGB(0,0,blue);

//     // AddLog(LOG_LEVEL_TEST,PSTR("sun_elevation=%d, blue=%d"), (int)sun_elevation, blue);

//     // AddLog(LOG_LEVEL_INFO, PSTR("elevation=%d, cct_temp=%d %d"),(int)sun_elevation, elev_perc, cct_val);

//     // uint8_t brightness_255 = map(sun_elevation,-50,-5,255,0);

//     // pCONT_iLight->rgbcct_controller.setBrightnessRGB255(brightness_255);

//   }else
//   // Just below hoeizon, sky blue to night blue
//   // "blue" remains bright
//  if(
//     (sun_elevation<=-5)&&
//     (sun_elevation>=-10)
//     )
//   {
//     // HsbColor hsb = HsbColor(RgbColor(0));
//     // hsb.H = map(sun_elevation,-50,10,255,0)

//     HsbColor hsb = HsbColor(RgbColor(0));
//     hsb.H =  pCONT_iLight->HUE_N2F(mapvalue(sun_elevation,-5,-10,180,240)); // yellow to red
//     hsb.S = 1;

//     /**
//      * Calculate brightness gradient
//      * */
//     hsb.B = mapvalue(sun_elevation,-5,-10,1.0f,1.0f);

//     uint8_t brightness_255 = map(sun_elevation,-5,-10,255,200);
//     pCONT_iLight->rgbcct_controller.setBrightnessRGB255(brightness_255);


//     pCONT_iLight->rgbcct_controller.setHsbColor(hsb);

//     // uint8_t blue =  map(sun_elevation,-50,-5,255,0);

//     // pCONT_iLight->rgbcct_controller.setRGB(0,0,blue);

//     // AddLog(LOG_LEVEL_TEST,PSTR("sun_elevation=%d, blue=%d"), (int)sun_elevation, blue);

//     // AddLog(LOG_LEVEL_INFO, PSTR("elevation=%d, cct_temp=%d %d"),(int)sun_elevation, elev_perc, cct_val);

//     // uint8_t brightness_255 = map(sun_elevation,-50,-5,255,0);

//     // pCONT_iLight->rgbcct_controller.setBrightnessRGB255(brightness_255);

//   }else
//   // "night" blue, slowing fade to low brightness
//  if(
//     (sun_elevation>=-50)&&
//     (sun_elevation<=-5)
//     )
//   {
//     HsbColor hsb = HsbColor(RgbColor(0));
//     hsb.H = pCONT_iLight->HUE_N2F(240); // yellow to red
//     hsb.S = 1;

//     /**
//      * Calculate brightness gradient
//      * */
//     hsb.B = mapvalue(sun_elevation,-5,-50,1.0f,0.0f);


//     pCONT_iLight->rgbcct_controller.setHsbColor(hsb);

//     uint8_t brightness_255 = map(sun_elevation,-5,-50,255,1);
//     pCONT_iLight->rgbcct_controller.setBrightnessRGB255(brightness_255);


//     // uint8_t blue =  map(sun_elevation,-50,-5,255,0);

//     // pCONT_iLight->rgbcct_controller.setRGB(0,0,blue);

//     // AddLog(LOG_LEVEL_TEST,PSTR("sun_elevation=%d, blue=%d"), (int)sun_elevation, blue);

//     // AddLog(LOG_LEVEL_INFO, PSTR("elevation=%d, cct_temp=%d %d"),(int)sun_elevation, elev_perc, cct_val);

//     // uint8_t brightness_255 = map(sun_elevation,-50,-5,255,0);

//     // pCONT_iLight->rgbcct_controller.setBrightnessRGB255(brightness_255);

//   }
  
//   // else
//   // if((sun_elevation>-50)&&(sun_elevation<10))
//   // {
//   //   // HsbColor hsb = HsbColor(RgbColor(0));
//   //   // hsb.H = map(sun_elevation,-50,10,255,0)

//   //   uint8_t blue =  map(sun_elevation,-50,10,255,0);

//   //   pCONT_iLight->rgbcct_controller.setRGB(0,0,blue);

//   //   // AddLog(LOG_LEVEL_INFO, PSTR("elevation=%d, cct_temp=%d %d"),(int)sun_elevation, elev_perc, cct_val);




//   //   // uint8_t brightness_255 = map(sun_elevation,-50,10,255,0);

//   //   // pCONT_iLight->rgbcct_controller.setBrightnessRGB255(brightness_255);

//   // }
//   // else
//   // if(
//   //   (sun_elevation>5)
//   //   )
//   // {
//   //   pCONT_iLight->rgbcct_controller.setRGB(1,0,0);    
//   // }
//   else
//   {
//     //unknown, therfore turn off
//     pCONT_iLight->rgbcct_controller.setRGB(0,0,0);
//   }



//   /**
//    * Generate the CCT channels
//    * */
//   if(
//     (sun_elevation<-5)
//     )
//   {

//     // Set the colour temp
//     pCONT_iLight->rgbcct_controller.setCCT(pCONT_iLight->get_CTRangeMin()); // set as daywhite

//     uint8_t brightness_255 = 0;//map(sun_elevation,-10,25,0,255);
//     pCONT_iLight->rgbcct_controller.setBrightnessCCT255(brightness_255);

//   }else
//   if(
//     (sun_elevation>=10)&&
//     (sun_elevation<=20)
//     )
//   {

//     // Convert elevation into percentage
//     uint8_t elev_perc = map(sun_elevation,10,20,0,100);
//     // Convert percentage into cct
//     uint16_t cct_val = mapvalue(elev_perc, 0,100, pCONT_iLight->get_CTRangeMin(),pCONT_iLight->get_CTRangeMax());
//     // Set the colour temp
//     pCONT_iLight->rgbcct_controller.setCCT(cct_val);

    
//     AddLog(LOG_LEVEL_TEST,PSTR("sun_elevation=%d, elev_perc=%d, cct_val=%d, cct=%d"), (int)sun_elevation, elev_perc, cct_val, pCONT_iLight->rgbcct_controller.getCCT());

//     uint8_t brightness_255 = map(sun_elevation,10,20,100,255);
//     pCONT_iLight->rgbcct_controller.setBrightnessCCT255(brightness_255);

//   }else
//   if(
//     (sun_elevation>=-5)&&
//     (sun_elevation<10)
//     )
//   {

//     // Convert elevation into percentage
//     uint8_t elev_perc = 0;//map(sun_elevation,-5,10,0,100);
//     // Convert percentage into cct
//     uint16_t cct_val = mapvalue(elev_perc, 0,100, pCONT_iLight->get_CTRangeMin(),pCONT_iLight->get_CTRangeMax());
//     // Set the colour temp
//     pCONT_iLight->rgbcct_controller.setCCT(cct_val);

    
//     AddLog(LOG_LEVEL_TEST,PSTR("sun_elevation=%d, elev_perc=%d, cct_val=%d, cct=%d"), (int)sun_elevation, elev_perc, cct_val, pCONT_iLight->rgbcct_controller.getCCT());

//     uint8_t brightness_255 = map(sun_elevation,-5,10,0,100);
//     pCONT_iLight->rgbcct_controller.setBrightnessCCT255(brightness_255);

//   }else
//   if(
//     (sun_elevation>20)
//     )
//   {

//     // Set the colour temp
//     pCONT_iLight->rgbcct_controller.setCCT(pCONT_iLight->get_CTRangeMax()); // set as daywhite

//     // uint8_t brightness_255 = map(sun_elevation,-10,25,0,255);
//     // pCONT_iLight->rgbcct_controller.setBrightnessCCT255(brightness_255);

//   }
  




// //  pCONT_iLight->rgbcct_controller.setCCT(0); // set as daywhite

  
//   pCONT_iLight->animation.flags.fForceUpdate = true;




//   pCONT_iLight->animation.palette.id = 10;

//  // AddLog(LOG_LEVEL_TEST, PSTR("SubTask_Flasher_Animate_Function__Solid_Static_Single_Colour"));
//   // Set palette pointer
//   mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
//   // Set up colours
//   // Brightness is generated internally, and rgbcct solid palettes are output values
//   animation_colours_rgbcct.DesiredColour  = mPaletteI->GetColourFromPalette(mPaletteI->static_palettes.ptr);

//   // AddLog(LOG_LEVEL_TEST, PSTR("DesiredColour1=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   if(!pCONT_iLight->rgbcct_controller.getApplyBrightnessToOutput()){ // If not already applied, do it using global values
//     animation_colours_rgbcct.DesiredColour = ApplyBrightnesstoRgbcctColour(
//       animation_colours_rgbcct.DesiredColour, 
//       pCONT_iLight->rgbcct_controller.getBrightnessRGB_WithGlobalApplied(),
//       pCONT_iLight->rgbcct_controller.getBrightnessCCT255()
//     );
//   }

//   // AddLog(LOG_LEVEL_TEST, PSTR("DesiredColour2=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   animation_colours_rgbcct.StartingColor = SEGMENT.GetPixelColor();
//   // GetPixelColourHardwareInterface();

//   // AddLog(LOG_LEVEL_TEST, PSTR("StartingColour2=%d,%d,%d,%d,%d"), animation_colours_rgbcct.StartingColor.R,animation_colours_rgbcct.StartingColor.G,animation_colours_rgbcct.StartingColor.B,animation_colours_rgbcct.StartingColor.WC,animation_colours_rgbcct.StartingColor.WW);
    

//   // // Call the animator to blend from previous to new
//   // this->setAnimFunctionCallback(
//   //   [this](const AnimationParam& param){
//   //     this->AnimationProcess_BlendStartingToDesiredColour_SOLID_COLOUR(param);
//   //   }
//   // );


//   //     // UpdateDesiredColourFromPaletteSelected();


//   //     UpdateStartingColourWithGetPixel();



//     //   flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
//     // // NO 
//     // }break;
//     // case EFFECTS_REGION_ANIMATE_ID: //shift along
//       AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//       this->setAnimFunctionCallback([this](const AnimationParam& param){ 
//         this->AnimationProcess_Generic_RGBCCT_Single_Colour_All(param); });
      
//   //     flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID;
//   //     break;
//   // }

// }


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/


// /**************************************************************************************************************************************************************
//  * @brief  Solid_Colour_Based_On_Sun_Elevation_02
//  * @note   From -10 to noon, CCT will range from yellow to daywhite
//  * @note   From -5 to dusk, blue will go from 0 to max_brightness 
//  * 
//  * @note   Gloabl brightness will be manual, or controlled indirectly eg via mqtt
//  * 
//  * @note   Using RgbcctColour palette that is designed for each point in elevation
//  * *************************************************************************************************************************************************************/

// void mAnimatorLight::SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_03()
// {
 
//   // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_02"));

//   pCONT_iLight->animation.palette.id = mPaletteI->PALETTELIST_FIXED_SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01_ID;

//  // AddLog(LOG_LEVEL_TEST, PSTR("SubTask_Flasher_Animate_Function__Solid_Static_Single_Colour"));
//   // Set palette pointer
//   mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
//   // Brightness is generated internally, and rgbcct solid palettes are output values
//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = false;



//   /**
//    * Solar data to use, defined here for testing or simulations
//    * */
// float sun_elevation = 0;
// #ifdef USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
//   #ifdef USE_DEVFEATURE_SUNPOSITION_ELEVATION_USE_TESTING_VALUE
//   sun_elevation = (float)pCONT_solar->solar_position_testing.elevation;
//   #else
//   sun_elevation = (float)pCONT_solar->solar_position.elevation;
//   #endif
// #endif
//   bool sun_is_ascending = true;//pCONT_solar->solar_position_testing.direction.is_ascending;

//   // Serial.printf("\n\r\n\rsun_elevation\t => %f\n\r", sun_elevation);
//   /**
//    * Sun elevation indexing is stored in palettes index location.
//    * The current sun elevation shall be searched against for nearest match, then depending on accesending or decending sun the nearest match and nearest next match will be linearblended as current show colour
//    * */

//   /**
//    * Get total pixels in palette
//    * */
//   mPalette::PALETTELIST::PALETTE *palette_p = mPaletteI->GetPalettePointerByID(mPaletteI->PALETTELIST_FIXED_SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01_ID);
//   uint8_t pixels_max = GetNumberOfColoursInPalette(palette_p);
//   AddLog(LOG_LEVEL_INFO,PSTR("pixels_max=%d"),pixels_max);

//   // Lets assume we need a zero crossing index, thus, we can use it to identity AS and DE modes
//   uint8_t zero_crossing_index = 0;

//   struct INDEXES_MATCHES{
//     uint8_t previous = 0; //ie colour moving away from
//     uint8_t next = 0; //colour moving towards
//   }index;

//   /**
//    * Steps to finding index
//    * 1) Find the zero-crossing index from the palette (ie the colour where its index is 0)
//    * 2) Decide if elevation is pos or neg, begin searching that part of the array
//    * 3) Find index of closest in array
//    * 4) Next and previous index will depend on direction of sun, and will be equal to current index if error is exactly 0
//    * */

//   /**
//    * Step X: Find zero crossing point
//    * Step X: Find all differences
//    * */
//   double last_index_difference = 0;
//   double current_index_difference = 0;
//   uint8_t lowest_difference_pixel_index = 0;
//   int16_t indexing = 0;
//   int16_t adjusted_index_back_to_minus90_to_plus90 = 0;

  
//   uint8_t lower_boundary_index = 13;
//   float lower_boundary_value = 45;
//   uint8_t upper_boundary_index = 14;
//   float upper_boundary_value = 90;




//   // Generate all the errors into an array
//   float sun_error[pixels_max];  
//   int16_t palette_indexes[pixels_max];  
//   // for(int i=0;i<pixels_max;i++)
//   // {
//   //   mPaletteI->GetColourFromPalette(palette_p, i, &indexing);
//   //   palette_indexes[i] = indexing - 90;
//   //   sun_error[i] = (float)sun_elevation - palette_indexes[i];
//   //   AddLog(LOG_LEVEL_INFO, PSTR("sun_error[%d] = %d %d \t %d"),i, indexing,  (int)sun_error[i], palette_indexes[i]);
//   //   // Also record zero-crossing point
//   //   if((indexing-90) == 0)
//   //   {
//   //     zero_crossing_index = i;
//   //     AddLog(LOG_LEVEL_INFO, PSTR("zero_crossing_index = %d FOUND"), zero_crossing_index);
//   //   }
//   // }

//   //     AddLog(LOG_LEVEL_INFO, PSTR(DEBUG_INSERT_PAGE_BREAK "zero_crossing_index = %d FOUND"), zero_crossing_index);
//   /**
//    * Ascending method for finding right region between points
//    * Check all, but once sun_elev is greater, then thats the current region
//    * */
//   float sun_positions_from_palette_index[pixels_max];    
//   for(int i=0;i<pixels_max;i++)
//   {
//     mPaletteI->GetColourFromPalette(palette_p, i, &indexing);
//     sun_positions_from_palette_index[i] = indexing - 90;
//     Serial.printf("sun_pos=[%02d]=\t%f\n\r", i, sun_positions_from_palette_index[i]);
//   }





//   uint8_t searching_matched_index = 0;
//   for(int i=0;i<pixels_max;i++)
//   {
//     Serial.printf("sun=%f > index[%d]=%f\n\r", sun_elevation, i, sun_positions_from_palette_index[i]);
//     if(sun_elevation >= sun_positions_from_palette_index[i])
//     {
      
//       // searching_matched_index = i;
//       // Serial.printf("sun=%f > index[%d]=%f   MATCH=%d\n\r", 
//       //   sun_elevation, i, sun_positions_from_palette_index[i], searching_matched_index
//       // );
//       Serial.printf("Still less\n\r");

//     }else{
      
//       searching_matched_index = i-1;
//       // Serial.printf("sun=%f > index[%d]=%f   MATCH=%d\n\r", 
//       //   sun_elevation, i, sun_positions_from_palette_index[i], searching_matched_index
//       // );
//       Serial.printf("searching_matched_index = %d\n\r", searching_matched_index);
//       break;

//     }
//   }

//   lower_boundary_index = searching_matched_index;
//   upper_boundary_index = searching_matched_index+1;
//   lower_boundary_value = sun_positions_from_palette_index[lower_boundary_index];
//   upper_boundary_value = sun_positions_from_palette_index[upper_boundary_index];

//   /**
//    * Check ranges are valid, if not, reset to 0 and 1
//    * */

//   float numer = sun_elevation        - lower_boundary_value;
//   float denum = upper_boundary_value - lower_boundary_value;
//   float progress_between_colours = numer/denum;

//   Serial.printf("\n\r\n\r\n\rsun_elevation\t => %f\n\r", sun_elevation);
//   Serial.printf("lower_boundary_value[%02d]=%f\n\r", lower_boundary_index, lower_boundary_value);
//   Serial.printf("upper_boundary_value[%02d]=%f\n\r", upper_boundary_index, upper_boundary_value);
//   Serial.printf("numer=\t%f\n\r",numer);
//   Serial.printf("denum=\t%f\n\r",denum);
//   Serial.printf("progress_between_colours=\t%f\n\r",progress_between_colours);










// //   float progress_between_colours = 0;

// //   /**
// //    * Change to working out the positive or negative side
// //    * */
// //   if(sun_elevation>=0)
// //   {

// //     AddLog(LOG_LEVEL_INFO, PSTR("Checking POSITIVE elevation\n\r"));

// //     uint8_t smallest_error_index = 0;
// //     float smallest_elev_error = fabs(sun_error[zero_crossing_index]);
// //     for(int i=zero_crossing_index+1;i<pixels_max;i++) // skip first
// //     {
// //       if(fabs(sun_error[i])<smallest_elev_error)
// //       {
// //         smallest_error_index = i;
// //         smallest_elev_error = fabs(sun_error[i]);
// //       }
// //       // AddLog(LOG_LEVEL_INFO, PSTR("sun_error[%d] = %d\t smallest_elev[%d]=%d"),
// //       //     i, (int)sun_error[i], smallest_error_index, (int)smallest_elev_error );
// //     }

// //     /**
// //      * previous colour will always equal the matched index (bring to tersary)
// //      * Work out the transitions
// //      * */
// //     if(sun_error[smallest_error_index] == 0) // EXACT ERROR
// //     {
// //       AddLog(LOG_LEVEL_INFO, PSTR("sun_error EXACT"));
// //       // next index depends on direction
// //       if (sun_is_ascending){
// //         index.previous = smallest_error_index; // equals this exact index
// //         index.next = smallest_error_index + 1;
// //         progress_between_colours = ((float)palette_indexes[index.next]/(float)palette_indexes[index.previous]);
// //       }else{
// //         index.previous = smallest_error_index; // equals this exact index
// //         index.next = smallest_error_index - 1;
// //         progress_between_colours = ((float)palette_indexes[index.previous]/(float)palette_indexes[index.next]);
// //       }
// //       Serial.printf("prev=%d, next=%d, progress=%d\n\r", index.previous, index.next, progress_between_colours);
// //     }else{                                    // CLOSE ERROR
// //       // depends on direction
// //       AddLog(LOG_LEVEL_INFO, PSTR("sun_error CLOSE"));
// //       if (sun_is_ascending){ // sun going up, means next will be bigger in elevation, in palette index, means index larger (since palette starts with negatives)
// //         AddLog(LOG_LEVEL_INFO, PSTR("sun_error[smallest_error_index] != 0 sun_is_ascending"));
        
// //         sun_elevation = 50;
// //         // smallest_error_index=11;

// //         float lower_boundary_index = 13;
// //         float lower_boundary_value = 45;
// //         float upper_boundary_index = 14;
// //         float upper_boundary_value = 90;


// //         // index.previous = lower_boundary_index; // equals this exact index
// //         // index.next = upper_boundary_index;



// //         // float numer = smallest_error_index - (float)palette_indexes[index.next];
// //         // float denum = smallest_error_index - (float)palette_indexes[index.previous];


// //         float numer = sun_elevation - lower_boundary_value;
// //         float denum = upper_boundary_value - lower_boundary_value;
// //         float ratio = numer/denum;



// //         Serial.printf("\n\r\n\r\n\rsun_elevation\t => %f\n\r", sun_elevation);
// //         Serial.printf("numer=%f\n\r",numer);
// //         Serial.printf("denum=%f\n\r",denum);
// //         Serial.printf("ratio=%f\n\r",ratio);





// //         // Serial.printf("%d %d %f %f\n\r", palette_indexes[index.next],palette_indexes[index.previous],numer,denum);
// //         // Serial.printf("%d %d %f %f\n\r", palette_indexes[index.next],palette_indexes[index.previous],numer,denum);

// //         /**
// //          * 10 ======= 20
// //          *      15
// //          * 
// //          * 20-10 = 10
// //          * 15-10 = 5
// //          * 5/10 = 0.5
// //          * 
// //          * 2-1 = 1
// //          * 1.5-1=0.5      * 
// //          * 0.5/1 => 0.5
// //          *  
// //          * 10*(1+0.5) = 15
// //          * 
// //          * In equation
// //          * 
// //          * full_range = next_max_val - prev_min_val
// //          * current_range = current_inbetween_val - prev_min_val
// //          * 
// //          * ratio_of_range = current_range/full_range
// //          * 
// //          * */

// //         for(int i=0;i<pixels_max;i++)
// //         {
// //           Serial.printf("palette_indexes[%02d] = %d\n\r",i, palette_indexes[i]);
// //         }
 
// //         float next_max_val = palette_indexes[index.next];
// //         float prev_min_val = palette_indexes[index.previous];
// //         float current_inbetween_val = smallest_elev_error;

// //         float full_range = next_max_val - prev_min_val;
// //         float current_range = current_inbetween_val - prev_min_val;
// //         float ratio_of_range = current_range/full_range;

// //         // Serial.printf("\n\r\n\r\n\rsun_elevation\t => %f\n\r", sun_elevation);
// //         // Serial.printf("smallest_error_index=%d\n\r",smallest_error_index);
// //         // Serial.printf("index.previous=%d\n\r",index.previous);
// //         // Serial.printf("index.next=%d\n\r",index.next);
// //         // Serial.printf("next_max_val=%f\n\r",next_max_val);
// //         // Serial.printf("prev_min_val=%f\n\r",prev_min_val);
// //         // Serial.printf("current_inbetween_val=%f\n\r",current_inbetween_val);
// //         // Serial.printf("full_range=%f\n\r",full_range);
// //         // Serial.printf("current_range=%f\n\r",current_range);
// //         // Serial.printf("ratio_of_range=%f\n\r",ratio_of_range);

       
// //         // AddLog_f

// //         progress_between_colours = (numer/denum);


// //       }else{
// //         index.previous = smallest_error_index; // equals this exact index
// //         index.next = smallest_error_index - 1;
// //         progress_between_colours = ((float)palette_indexes[index.previous]/(float)palette_indexes[index.next]);
// //       }
// //       Serial.printf("prev=%d, next=%d, progress=%f\n\r", index.previous, index.next, progress_between_colours);
// //     }


// //   }
// //   else // sun_elevation<0
// //   {

// //     AddLog(LOG_LEVEL_INFO, PSTR("Checking NEGATIVE elevation"));
    
// //     uint8_t smallest_error_index = 0;
// //     float smallest_elev_error = fabs(sun_error[0]);
// //     for(int i=1;i<zero_crossing_index;i++) // skip first
// //     {
// //       if(fabs(sun_error[i])<smallest_elev_error)
// //       {
// //         smallest_error_index = i;
// //         smallest_elev_error = fabs(sun_error[i]);
// //       }
// //       AddLog(LOG_LEVEL_INFO, PSTR("sun_error[%d] = %d smallest_elev_error%d smallest_error_index%d"),i, (int)sun_error[i], (int)smallest_elev_error, smallest_error_index);
// //     }

// //     /**
// //      * previous colour will always equal the matched index (bring to tersary)
// //      * Work out the transitions
// //      * */
// //     if(sun_error[smallest_error_index] == 0)
// //     {
// //       AddLog(LOG_LEVEL_INFO, PSTR("sun_error[smallest_error_index] == 0"));
// //       // next index depends on direction
// //       if (sun_is_ascending){
// //         index.previous = smallest_error_index; // equals this exact index
// //         index.next = smallest_error_index + 1;
// //         progress_between_colours = 0;//((float)palette_indexes[index.next]/(float)palette_indexes[index.previous]);
// //       }else{
// //         index.previous = smallest_error_index; // equals this exact index
// //         index.next = smallest_error_index - 1;
// //         progress_between_colours = 0;//((float)palette_indexes[index.previous]/(float)palette_indexes[index.next]);
// //       }
// //     }else{
// //       // depends on direction
// //       AddLog(LOG_LEVEL_INFO, PSTR("sun_error[smallest_error_index] != 0"));
// //       if (sun_is_ascending){ // sun going up, means next will be bigger in elevation, in palette index, means index larger (since palette starts with negatives)
// //       AddLog(LOG_LEVEL_INFO, PSTR("sun_error[smallest_error_index] != 0 sun_is_ascending"));
// //         index.previous = smallest_error_index; // equals this exact index
// //         index.next = smallest_error_index + 1;
// //         progress_between_colours = ((float)palette_indexes[index.next]/(float)palette_indexes[index.previous]);
// //       }else{
// //         index.previous = smallest_error_index; // equals this exact index
// //         index.next = smallest_error_index - 1;
// //         progress_between_colours = ((float)palette_indexes[index.previous]/(float)palette_indexes[index.next]);
// //       }
// //     }


// //   }

// /**
//  * Showing the colours
//  * 1) previous
//  * 2) next
//  * 3) linearblend of the exact new colour
//  * */

//     RgbcctColor c_prev = mPaletteI->GetColourFromPalette(palette_p, lower_boundary_index);
//     RgbcctColor c_next = mPaletteI->GetColourFromPalette(palette_p, upper_boundary_index);

//     Serial.printf("progress_between_colours\t %f(%d)/%f(%d) => %f\n\r", 
//       lower_boundary_value, lower_boundary_index, 
//       upper_boundary_value, upper_boundary_index, progress_between_colours);

//     RgbcctColor c_blended = RgbcctColor::LinearBlend(c_prev,c_next,progress_between_colours);

//     RgbcctColor c = c_prev; 
//     AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_p\t%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WW,c.WC);
//     c = c_blended; 
//     AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_b\t%d,%d,%d,%d,%d (progress %d"),c.R,c.G,c.B,c.WW,c.WC, (int)(progress_between_colours*100));
//     c = c_next; 
//     AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_n\t%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WW,c.WC);

//     /**
//      * Load new colour into animation
//      * */

// //   /**
// //    * Debug, show all colours
// //    * */
// //   int16_t adjusted_index = 0;
// //   for(int desired_index=0;desired_index<pixels_max;desired_index++)
// //   {
// //     RgbcctColor c = mPaletteI->GetColourFromPalette(mPaletteI->GetPalettePointerByID(mPaletteI->PALETTELIST_FIXED_SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01_ID), desired_index, &indexing);
// //     adjusted_index = indexing - 90;
// //     AddLog(LOG_LEVEL_INFO, PSTR("rgbcct[%d]=\t%d\t%d\t%d,%d,%d,%d,%d"),desired_index,indexing,adjusted_index,c.R,c.G,c.B,c.WW,c.WC);
// //   }

//   // delay(100);

  
//   pCONT_iLight->animation.flags.fForceUpdate = true;

//   animation_colours_rgbcct.DesiredColour  = c_blended;//mPaletteI->GetColourFromPalette(mPaletteI->static_palettes.ptr);

//   // AddLog(LOG_LEVEL_TEST, PSTR("DesiredColour1=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   if(!pCONT_iLight->rgbcct_controller.getApplyBrightnessToOutput()){ // If not already applied, do it using global values
//     animation_colours_rgbcct.DesiredColour = ApplyBrightnesstoRgbcctColour(
//       animation_colours_rgbcct.DesiredColour, 
//       pCONT_iLight->rgbcct_controller.getBrightnessRGB_WithGlobalApplied(),
//       pCONT_iLight->rgbcct_controller.getBrightnessCCT255()
//     );
//   }
  
//   animation_colours_rgbcct.StartingColor = SEGMENT.GetPixelColor();
  
//   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//   this->setAnimFunctionCallback([this](const AnimationParam& param){ 
//     this->AnimationProcess_Generic_RGBCCT_Single_Colour_All(param); });
   
// }




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/


// /**************************************************************************************************************************************************************
//  * @brief  Solid_Colour_Based_On_Sun_Elevation_02
//  * @note   From -10 to noon, CCT will range from yellow to daywhite
//  * @note   From -5 to dusk, blue will go from 0 to max_brightness 
//  * 
//  * @note   Gloabl brightness will be manual, or controlled indirectly eg via mqtt
//  * 
//  * @note   Using RgbcctColour palette that is designed for each point in elevation
//  * *************************************************************************************************************************************************************/

// void mAnimatorLight::SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_04()
// {
 
//   // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_02"));

//   // pCONT_iLight->animation.palette.id = mPaletteI->PALETTELIST_FIXED_SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01_ID;

//   // Set palette pointer
//   mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);

//   /**
//    * Solar data to use, defined here for testing or simulations
//    * */
// float sun_elevation = 0;
// #ifdef USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
//   #ifdef USE_DEVFEATURE_SUNPOSITION_ELEVATION_USE_TESTING_VALUE
//   sun_elevation = (float)pCONT_solar->solar_position_testing.elevation;
//   #else
//   sun_elevation = (float)pCONT_solar->solar_position.elevation;
//   #endif
// #endif
//   bool sun_is_ascending = true;//pCONT_solar->solar_position_testing.direction.is_ascending;
//   // Serial.printf("\n\r\n\rsun_elevation\t => %f\n\r", sun_elevation);

//   /**
//    * Sun elevation indexing is stored in palettes index location.
//    * The current sun elevation shall be searched against for nearest match, then depending on accesending or decending sun the nearest match and nearest next match will be linearblended as current show colour
//    * */

//   /**
//    * Get total pixels in palette
//    * */
//   mPalette::PALETTELIST::PALETTE *palette_p = mPaletteI->GetPalettePointerByID(pCONT_iLight->animation.palette.id);
//   uint8_t pixels_max = GetNumberOfColoursInPalette(palette_p);
//   // AddLog(LOG_LEVEL_INFO,PSTR("pixels_max=%d"),pixels_max);

//   // Lets assume we need a zero crossing index, thus, we can use it to identity AS and DE modes
//   uint8_t zero_crossing_index = 0;

//   struct INDEXES_MATCHES{
//     uint8_t previous = 0; //ie colour moving away from
//     uint8_t next = 0; //colour moving towards
//   }index;

//   /**
//    * Steps to finding index
//    * 1) Find the zero-crossing index from the palette (ie the colour where its index is 0)
//    * 2) Decide if elevation is pos or neg, begin searching that part of the array
//    * 3) Find index of closest in array
//    * 4) Next and previous index will depend on direction of sun, and will be equal to current index if error is exactly 0
//    * */

//   /**
//    * Step X: Find zero crossing point
//    * Step X: Find all differences
//    * */
//   int16_t indexing = 0;  
//   uint8_t lower_boundary_index = 13;
//   float lower_boundary_value = 45;
//   uint8_t upper_boundary_index = 14;  
//   float upper_boundary_value = 90;
//   float sun_positions_from_palette_index[pixels_max];  
//   uint8_t searching_matched_index = 0;

//   /**
//    * Ascending method for finding right region between points
//    * Check all, but once sun_elev is greater, then thats the current region
//    * */
//   for(int i=0;i<pixels_max;i++)
//   {
//     mPaletteI->GetColourFromPalette(palette_p, i, &indexing);
//     sun_positions_from_palette_index[i] = indexing - 90;
//     // Serial.printf("sun_pos=[%02d]=\t%f\n\r", i, sun_positions_from_palette_index[i]);
//   }


//   for(int i=0;i<pixels_max;i++)
//   {
//     // Serial.printf("sun=%f > index[%d]=%f\n\r", sun_elevation, i, sun_positions_from_palette_index[i]);
//     if(sun_elevation >= sun_positions_from_palette_index[i])
//     {
      
//       // searching_matched_index = i;
//       // Serial.printf("sun=%f > index[%d]=%f   MATCH=%d\n\r", 
//       //   sun_elevation, i, sun_positions_from_palette_index[i], searching_matched_index
//       // );
//       //Serial.printf("Still less\n\r");

//     }else{
      
//       searching_matched_index = i-1;
//       // Serial.printf("sun=%f > index[%d]=%f   MATCH=%d\n\r", 
//       //   sun_elevation, i, sun_positions_from_palette_index[i], searching_matched_index
//       // );
//       // Serial.printf("searching_matched_index = %d\n\r", searching_matched_index);
//       break;

//     }

//     // Directly, manually, check the last memory space

//     if(sun_elevation == sun_positions_from_palette_index[pixels_max-1])
//     {
//       searching_matched_index = i-1;
//       // Serial.printf("sun=%f > index[%d]=%f   MATCH=%d\n\r", 
//       //   sun_elevation, i, sun_positions_from_palette_index[i], searching_matched_index
//       // );
//       break;

//     }




//   }

//   lower_boundary_index = searching_matched_index;
//   upper_boundary_index = searching_matched_index+1;

//   /**
//    * Check ranges are valid, if not, reset to 0 and 1
//    * */
//   if(lower_boundary_index>=pixels_max)
//   {
//     lower_boundary_index = 0;
//     Serial.printf("lower_boundary_index>=pixels_max\n\r");
//   }
//   if(upper_boundary_index>=pixels_max)
//   {
//     upper_boundary_index = pixels_max;
//     Serial.printf("upper_boundary_index>=pixels_max\n\r");
//   }

//   lower_boundary_value = sun_positions_from_palette_index[lower_boundary_index];
//   upper_boundary_value = sun_positions_from_palette_index[upper_boundary_index];


//   float numer = sun_elevation        - lower_boundary_value;
//   float denum = upper_boundary_value - lower_boundary_value;
//   float progress_between_colours = numer/denum;

//   Serial.printf("\n\r\n\r\n\rsun_elevation\t => %f\n\r", sun_elevation);
//   Serial.printf("lower_boundary_value[%02d]=%f\n\r", lower_boundary_index, lower_boundary_value);
//   Serial.printf("upper_boundary_value[%02d]=%f\n\r", upper_boundary_index, upper_boundary_value);
//   Serial.printf("numer=\t%f\n\r",numer);
//   Serial.printf("denum=\t%f\n\r",denum);
//   Serial.printf("progress_between_colours=\t%f\n\r",progress_between_colours);

//   /**
//    * Showing the colours
//    * 1) previous
//    * 2) next
//    * 3) linearblend of the exact new colour
//    * */

//   RgbcctColor c_lower = mPaletteI->GetColourFromPalette(palette_p, lower_boundary_index);
//   RgbcctColor c_upper = mPaletteI->GetColourFromPalette(palette_p, upper_boundary_index);

//   // Serial.printf("progress_between_colours\t %f(%d)/%f(%d) => %f\n\r", 
//   //   lower_boundary_value, lower_boundary_index, 
//   //   upper_boundary_value, upper_boundary_index, progress_between_colours);

//   RgbcctColor c_blended = RgbcctColor::LinearBlend(c_lower, c_upper, progress_between_colours);

//   RgbcctColor c = c_lower; 
//   // AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_p\t%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WW,c.WC);
//   c = c_blended; 
//   // AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_b\t%d,%d,%d,%d,%d (progress %d"),c.R,c.G,c.B,c.WW,c.WC, (int)(progress_between_colours*100));
//   c = c_upper; 
//   // AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_n\t%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WW,c.WC);

//   /**
//    * Load new colour into animation
//    * */

//   pCONT_iLight->animation.flags.fForceUpdate = true;

//   animation_colours_rgbcct.DesiredColour  = c_blended;

//   // AddLog(LOG_LEVEL_TEST, PSTR("DesiredColour1=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   if(!pCONT_iLight->rgbcct_controller.getApplyBrightnessToOutput())
//   { // If not already applied, do it using global values
//     animation_colours_rgbcct.DesiredColour = ApplyBrightnesstoRgbcctColour(
//       animation_colours_rgbcct.DesiredColour, 
//       pCONT_iLight->rgbcct_controller.getBrightnessRGB_WithGlobalApplied(),
//       pCONT_iLight->rgbcct_controller.getBrightnessCCT255()
//     );
//   }

//   animation_colours_rgbcct.StartingColor = SEGMENT.GetPixelColor();

//   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//   this->setAnimFunctionCallback([this](const AnimationParam& param){
//       this->AnimationProcess_Generic_RGBCCT_Single_Colour_All(param); });
   
// }



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/


// /****
//  * Changes pixels randomly to new colour, with slow blending
//  * Requires new colour calculation each call
//  */
// void mAnimatorLight::SubTask_Flasher_Animate_Function_Twinkle_Palette_Brightness_From_Lower_To_Upper_And_Back(){

// // Finish this mode later in december
// // Split strip into parts
// // Take every X pixels as the animated pixel, then I only need to remember a count 0:X
// // Move across that each time (or make it random)
// // Use this, to manipulate brightness from low to high... on the next animate_id call, return birghtness back (with require another counter for "progress of animation")
// // Once that group has changed, or, even during it, pick another group to manipulate
// // Effect should be "Group of pixels pulsing on, while the rest remain at a darker brightness"

//   return;

//    switch(flashersettings.region){
//     case EFFECTS_REGION_COLOUR_SELECT_ID: //set colours
//       AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
//       pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
//       // if(flashersettings.flags.enable_endingcolour_as_alternate){
//       //   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL flashersettings.flags.enable_endingcolour_as_alternate"));
//       //   UpdateDesiredColourWithSingleColour(RgbcctColor(0));
//       // }

//       pCONT_iLight->rgbcct_controller.setBrightnessRGB255(map(flashersettings.brightness_min, 0,100, 0,255));


//       UpdateDesiredColourFromPaletteSelected();
//       // if(flashersettings.flags.enable_startcolour_as_alternate){
//       //   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL flashersettings.flags.enable_startcolour_as_alternate"));
//       //   UpdateStartingColourWithSingleColour(RgbcctColor(0));
//       // }else{
//         UpdateStartingColourWithGetPixel();
//       // }
//       flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
//     break;
//     case EFFECTS_REGION_ANIMATE_ID: //shift along




    
//         // UpdateStartingColourWithGetPixel();

//         // uint16_t index_random = random(0,pCONT_iLight->settings.light_size_count);

//         // HsbColor hsb = SEGMENT.GetPixelColor(index_random);

//         // if(hsb.B < pCONT_iLight->BrtN2F(flashersettings.brightness_max)){
//         //   hsb.B = pCONT_iLight->BrtN2F(flashersettings.brightness_max);
//         //   animation_colours[index_random].DesiredColour = hsb;
//         // }

//       AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//       this->setAnimFunctionCallback([this](const AnimationParam& param){ this->AnimationProcess_Twinkle_Palette_Brightness_From_Lower_To_Upper_And_Back(param); });
//       break;

      
//       // Stay on this until restarted
//   }

// }


// void mAnimatorLight::AnimationProcess_Twinkle_Palette_Brightness_From_Lower_To_Upper_And_Back(const AnimationParam& param)
// {   

//   // As integer so the if statement checks will not fail due to rounding errors
//   // uint8_t progress_percentage = param.progress*100; 
//   // uint8_t brightness_as_percentage = map(pCONT_iLight->getBriRGB(), 0,255, 0,100);
//   // uint8_t random_amount = map(shared_flasher_parameters.alternate_random_amount_as_percentage, 0,100, 0,pCONT_iLight->settings.light_size_count);

//   //       uint16_t index_random = random(0,pCONT_iLight->settings.light_size_count);

//   // /*
//   // 0-10    20-30   40-50   60-70    80-90      //coloured
//   //     10-20   30-40   50-60   70-80   90-0    //white
//   // */
//   // if(progress_percentage != shared_flasher_parameters.progress_percentage_last_animated_on){
//   //   shared_flasher_parameters.progress_percentage_last_animated_on = progress_percentage; //update stored value
//   //   switch(progress_percentage){
//   //     case 50:


//   //       HsbColor hsb = SEGMENT.GetPixelColor(index_random);

//   //       if(hsb.B < pCONT_iLight->BrtN2F(flashersettings.brightness_max)){
//   //         hsb.B = pCONT_iLight->BrtN2F(flashersettings.brightness_max);
//   //         // animation_colours[index_random].DesiredColour = hsb;
//   //       }

//   //       SEGMENT.SetPixelColor(index_random, hsb);



//   //       // for (uint16_t ii = 0; ii < random_amount; ii++){
//   //       //   SEGMENT.SetPixelColor(
//   //       //     random(0,pCONT_iLight->settings.light_size_count), 
//   //       //     HsbColor(pCONT_iLight->HUE_N2F(30),pCONT_iLight->SatN2F(90),pCONT_iLight->BrtN2F(random(0,brightness_as_percentage)))
//   //       //   );
//   //       // }
//   //     break;
//   //     case 0:
//   //     case 100:
//   //       for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
//   //         SEGMENT.SetPixelColor(pixel, animation_colours[pixel].DesiredColour);
//   //       }
//   //     break;
//   //   }
//   // }



//    RgbTypeColor updatedColor;
//   for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
//     updatedColor = RgbTypeColor::LinearBlend(
//         animation_colours[pixel].StartingColor,
//         animation_colours[pixel].DesiredColour,
//         param.progress);    
//     SEGMENT.SetPixelColor(pixel, updatedColor);
//   } // END for
  

// }


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/


// /**************************************************************************************************************************************************************
//  * @brief  Solid_Colour_Based_On_Sun_Elevation_05
//  * 
//  * CCT_Mapped, day white to warm white around +-20, then >20 is max cct
//  * 
//  * *************************************************************************************************************************************************************/

// void mAnimatorLight::SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_05()
// {
 
//   // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_05"));

//   pCONT_iLight->animation.palette.id = mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_01_ID;

//   mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
//   // Set up colours
//   // Brightness is generated internally, and rgbcct solid palettes are output values


// float sun_elevation = 0;
// #ifdef USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
//   #ifdef USE_DEVFEATURE_SUNPOSITION_ELEVATION_USE_TESTING_VALUE
//   sun_elevation = (float)pCONT_solar->solar_position_testing.elevation;
//   #else
//   sun_elevation = (float)pCONT_solar->solar_position.elevation;
//   #endif
// #endif
//   if(sun_elevation < -20)
//   {
//     pCONT_iLight->rgbcct_controller.setCCT(pCONT_iLight->get_CTRangeMax());      
//   }else
//   if(sun_elevation > 20)
//   {
//     pCONT_iLight->rgbcct_controller.setCCT(pCONT_iLight->get_CTRangeMin());      
//   }else{
//     // Convert elevation into percentage
//     uint8_t elev_perc = map(sun_elevation,-20,20,0,100);
//     // Convert percentage into cct
//     uint16_t cct_val = mapvalue(elev_perc, 0,100, pCONT_iLight->get_CTRangeMax(),pCONT_iLight->get_CTRangeMin());
 
//     // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "cct_val=%d"),cct_val);
//     // Set the colour temp
//     pCONT_iLight->rgbcct_controller.setCCT(cct_val);    
//   }

//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = false;
//   animation_colours_rgbcct.DesiredColour  = mPaletteI->GetColourFromPalette(mPaletteI->static_palettes.ptr);
//   pCONT_iLight->animation.flags.fForceUpdate = true;

//   // AddLog(LOG_LEVEL_TEST, PSTR("DesiredColour1=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   if(!pCONT_iLight->rgbcct_controller.getApplyBrightnessToOutput())
//   { // If not already applied, do it using global values
//     animation_colours_rgbcct.DesiredColour = ApplyBrightnesstoRgbcctColour(
//       animation_colours_rgbcct.DesiredColour, 
//       pCONT_iLight->rgbcct_controller.getBrightnessRGB_WithGlobalApplied(),
//       pCONT_iLight->rgbcct_controller.getBrightnessCCT255()
//     );
//   }

//   animation_colours_rgbcct.StartingColor = SEGMENT.GetPixelColor();

//   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//   this->setAnimFunctionCallback([this](const AnimationParam& param){
//       this->AnimationProcess_Generic_RGBCCT_Single_Colour_All(param); });
   
// }


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/



// void mAnimatorLight::SubTask_Flasher_Animate_Function_Twinkle_SingleColour_Random(){

//   switch(flashersettings.region){
//     case EFFECTS_REGION_COLOUR_SELECT_ID:{ //set colours
//       AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
//       UpdateDesiredColourFromPaletteSelected();
//       UpdateStartingColourWithGetPixel();
//       flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
//     // NO 
//     }break;
//     case EFFECTS_REGION_ANIMATE_ID: //shift along
//       AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//       this->setAnimFunctionCallback([this](const AnimationParam& param){ this->AnimationProcess_TwinkleSingleColourRandom(param); });
//       break;
//   }

// }

// void mAnimatorLight::AnimationProcess_TwinkleSingleColourRandom(const AnimationParam& param)
// {   

//   // As integer so the if statement checks will not fail due to rounding errors
//   uint8_t progress_percentage = param.progress*100; 
//   uint8_t brightness_as_percentage = map(pCONT_iLight->rgbcct_controller.getBrightnessRGB_WithGlobalApplied(), 0,255, 0,100);
//   uint8_t random_amount = map(shared_flasher_parameters.alternate_random_amount_as_percentage, 0,100, 0,pCONT_iLight->settings.light_size_count);

//   /*
//   0-10    20-30   40-50   60-70    80-90      //coloured
//       10-20   30-40   50-60   70-80   90-0    //white
//   */
//   if(progress_percentage != shared_flasher_parameters.progress_percentage_last_animated_on){
//     shared_flasher_parameters.progress_percentage_last_animated_on = progress_percentage; //update stored value
//     switch(progress_percentage){
//       case 10:
//       case 30:
//       case 50:
//       case 70:
//       case 90:
//         for (uint16_t ii = 0; ii < random_amount; ii++){
//           SEGMENT.SetPixelColor(
//             random(0,pCONT_iLight->settings.light_size_count), 
//             HsbColor(pCONT_iLight->HUE_N2F(30),pCONT_iLight->SatN2F(90),pCONT_iLight->BrtN2F(random(0,brightness_as_percentage)))
//           );
//         }
//       break;
//       case 0:
//       case 20:
//       case 40:
//       case 60:
//       case 80: //go back to coloured
//         for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
//           SEGMENT.SetPixelColor(pixel, animation_colours[pixel].DesiredColour);
//         }
//       break;
//     }
//   }

// }



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/



// void mAnimatorLight::SubTask_Flasher_Animate_Function_Twinkle_PaletteColour_Random(){
  
//   switch(flashersettings.region){
//     case EFFECTS_REGION_COLOUR_SELECT_ID: //set colours
//       AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
//       // if(flashersettings.flags.enable_endingcolour_as_alternate){
//       //   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL flashersettings.flags.enable_endingcolour_as_alternate"));
//       //   UpdateDesiredColourWithSingleColour(RgbcctColor(0));
//       // }
//       UpdateDesiredColourFromPaletteSelected();
//       // if(flashersettings.flags.enable_startcolour_as_alternate){
//       //   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL flashersettings.flags.enable_startcolour_as_alternate"));
//       //   UpdateStartingColourWithSingleColour(RgbcctColor(0));
//       // }else{
//         UpdateStartingColourWithGetPixel();
//       // }
//       flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
//     break;
//   //   case EFFECTS_REGION_ANIMATE_ID: //shift along
//   //     AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//   //     this->setAnimFunctionCallback([this](const AnimationParam& param){ this->AnimationProcess_TwinkleUsingPaletteColourRandom(param); });
//   //     break;
//   }

// }



// void mAnimatorLight::AnimationProcess_TwinkleUsingPaletteColourRandom(const AnimationParam& param)
// {   

//   // As integer so the if statement checks will not fail due to rounding errors
//   uint8_t progress_percentage = param.progress*100; 
//   // uint8_t brightness_as_percentage = map(pCONT_iLight->getBriRGB(), 0,255, 0,100);
//   uint8_t random_amount = map(shared_flasher_parameters.alternate_random_amount_as_percentage, 0,100, 0,pCONT_iLight->settings.light_size_count);
//   RgbTypeColor flash_colour = RgbTypeColor(0);
//   uint8_t desired_pixel = 0;
//   int16_t pixel_position = -1;
//   uint8_t flashed_brightness = 0; // use set brightness or flash brighter as option
  
//   // Update pointer of struct
//   mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);

  
//   // for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
//   //   RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
//   //       animation_colours[pixel].StartingColor,
//   //       animation_colours[pixel].DesiredColour,
//   //       param.progress);    
//   //   SEGMENT.SetPixelColor(pixel, updatedColor);
//   // } // END for

//   /*
//   0-10    20-30   40-50   60-70    80-90      //coloured
//       10-20   30-40   50-60   70-80   90-0    //white
//   */
//   if(progress_percentage != shared_flasher_parameters.progress_percentage_last_animated_on){
//     shared_flasher_parameters.progress_percentage_last_animated_on = progress_percentage; //update stored value
//       switch(progress_percentage){
//       case 10:
//       case 30:
//       case 50:
//       case 70:
//       case 90:
//         for (uint16_t ii = 0; ii < random_amount; ii++){
//           flashed_brightness = random(0,shared_flasher_parameters.alternate_brightness_max);        
//           // For random, desired pixel from map will also be random
//           desired_pixel = random(0,GetNumberOfColoursInPalette(mPaletteI->static_palettes.ptr));
//           // get colour from palette
//           flash_colour = mPaletteI->GetColourFromPalette(mPaletteI->static_palettes.ptr,desired_pixel,&pixel_position);
//           flash_colour = ApplyBrightnesstoRgbcctColour(flash_colour,flashed_brightness);
//           SEGMENT.SetPixelColor(
//             random(0,pCONT_iLight->settings.light_size_count), 
//             flash_colour
//           );
//         }
//       break;
//       case 0:
//       case 20:
//       case 40:
//       case 60:
//       case 80: //go back to coloured
//         for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
//           SEGMENT.SetPixelColor(pixel, animation_colours[pixel].DesiredColour);
//         }
//       break;
//     }
//   }

// }




/***
 * 
 * Create an animator for each segment, or do one by hand?
 * 
 * */


#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED


//each needs 12 bytes
//Spark type is used for popcorn, 1D fireworks, and drip
typedef struct Spark {
  float pos;
  float vel;
  uint16_t col;
  uint8_t colIndex;
} spark;


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Tri state pattern
 * @note : Converted from WLED Effects
 * 
 * @param intensity: Controls width of pattern
 * @param speed    : None
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Tri_Static_Pattern()
{

  uint8_t segSize = (SEGMENT.intensity() >> 5) +1;
  uint8_t currSeg = 0;
  uint16_t currSegCount = 0;

  for (uint16_t i = 0; i < SEGLEN; i++) 
  {

    if ( currSeg % 3 == 0 ) {
      SEGMENT.SetPixelColor(i, GetSegmentColour(0, SEGIDX), SET_BRIGHTNESS);
    } else if( currSeg % 3 == 1) {
      SEGMENT.SetPixelColor(i, GetSegmentColour(1, SEGIDX), SET_BRIGHTNESS);
    } else {
      SEGMENT.SetPixelColor(i, (GetSegmentColour(2, SEGIDX).CalculateBrightness() > 0 ? GetSegmentColour(2, SEGIDX) : WHITE), SET_BRIGHTNESS);
    }   

    currSegCount += 1;
    if (currSegCount >= segSize) {
      currSeg +=1;
      currSegCount = 0;
    }

  }

  SetSegment_AnimFunctionCallback_WithoutAnimator(SEGIDX);  

}
static const char PM_EFFECT_CONFIG__TRI_STATIC_PATTERN[] PROGMEM = ",Size;1,2,3;;;pal=0";



/********************************************************************************************************************************************************************************************************************
*******************************************************************************************************************************************************************************************************************
   @brief: Colour_Wipe_Base
   @note : Converted from WLED Effects
           LEDs are turned on (color1) in sequence, then turned off (color2) in sequence.
           if (bool rev == true) then LEDs are turned off in reverse order
   @note   Rate: FRAME_RATE

          aux0: is a 8bit (255) value that stores where in the colour wheel the (old) draw colour is coming from
          aux1: saves the 2nd colour (new) to be drawn
          step: saves what part of the animation is happening (e.g. create, move, rotate etc)

          Intensity: Smoothness [0=Instant, 255=Blended]

   @param rev             Reversed
   @param useRandomColors 
*******************************************************************************************************************************************************************************************************************
********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::BaseEffectAnim__Base_Colour_Wipe(bool rev, bool useRandomColors)
{

//speed of 128, cycletime = 19800
  uint32_t cycleTime = 750 + (255 - SEGMENT.speed())*150;
  // millis() % total_cycle_time ==> gives remainder, and therefore process of animation from 0->cycleTime is 0->100%
  uint32_t perc = millis() % cycleTime;
  // prog gets the process from 0->65535 again
  uint16_t prog = (perc * 65535) / cycleTime;
   
  // ALOG_INF(PSTR(" SEGMENT.speed() = %d"),  SEGMENT.speed());
  // ALOG_INF(PSTR("cycleTime = %d"), cycleTime);
  // ALOG_INF(PSTR("perc = %d"), perc);
  // ALOG_INF(PSTR("prog = %d"), prog);


  bool back = (prog > 32767); // if in second half of 16 bit, then take away the first half to scale the 2nd into the first as positive counter
  if (back) {
    prog -= 32767;
    if (SEGMENT.step == 0) SEGMENT.step = 1;
  } else {
    if (SEGMENT.step == 2) SEGMENT.step = 3; // Trigger color change
  }
  
  if (useRandomColors) {
    // First call ie create colours to later be animated
    if (SEGMENT.call == 0) {
      SEGMENT.params_internal.aux0 = random8();
      SEGMENT.step = 3;
    }
    if (SEGMENT.step == 1) { //if flag set, change to new random color
      SEGMENT.params_internal.aux1 = get_random_wheel_index(SEGMENT.params_internal.aux0);
      SEGMENT.step = 2;
    }
    if (SEGMENT.step == 3) {
      SEGMENT.params_internal.aux0 = get_random_wheel_index(SEGMENT.params_internal.aux1);
      SEGMENT.step = 0;
    }
  }

  uint16_t ledIndex = (prog * SEGLEN) >> 15;
  uint16_t rem = 0;
  rem = (prog * SEGLEN) * 2; //mod 0xFFFF
  rem /= (SEGMENT.intensity() +1);
  if (rem > 255) rem = 255;

  uint32_t col1 = useRandomColors ? color_wheel(SEGMENT.params_internal.aux1) : SEGCOLOR_U32(1);

  for (uint16_t i = 0; i < SEGLEN; i++)
  {
    uint16_t indexPixel = (rev && back)? SEGLEN -1 -i : i;

    uint32_t col0 = useRandomColors ? color_wheel(SEGMENT.params_internal.aux0) : SEGMENT.GetPaletteColour(indexPixel, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32();
    
    if (i < ledIndex) 
    {
      SEGMENT.SetPixelColor(indexPixel, back ? col1 : col0, true);
    } else
    {
      SEGMENT.SetPixelColor(indexPixel, back ? col0 : col1, true);
      if (i == ledIndex) SEGMENT.SetPixelColor(indexPixel, ColourBlend(back ? col0 : col1, back ? col1 : col0, rem), true);
    }
  } 

  SetSegment_AnimFunctionCallback_WithoutAnimator(SEGIDX);

}






/*
 * Lights all LEDs one after another.
 */
void mAnimatorLight::EffectAnim__Colour_Wipe()
{
  BaseEffectAnim__Base_Colour_Wipe(false, false);
}
static const char PM_EFFECT_CONFIG__COLOR_WIPE[] PROGMEM = "!,!;!,!;!";

/*
 * Turns all LEDs after each other to a random color.
 * Then starts over with another color.
 */
void mAnimatorLight::EffectAnim__Colour_Wipe_Random()
{
  BaseEffectAnim__Base_Colour_Wipe(false, true);
}
static const char PM_EFFECT_CONFIG__COLOR_WIPE_RANDOM[] PROGMEM = "!;;!";


/*
 * Lights all LEDs one after another. Turns off opposite
 */
void mAnimatorLight::EffectAnim__Colour_Sweep()
{
  BaseEffectAnim__Base_Colour_Wipe(true, false);
}
static const char PM_EFFECT_CONFIG__COLOR_SWEEP[] PROGMEM = "!,!;!,!;!";


/*
 * Random color introduced alternating from start and end of strip->
 */
void mAnimatorLight::EffectAnim__Colour_Sweep_Random()
{
  BaseEffectAnim__Base_Colour_Wipe(true, true);
}
static const char PM_EFFECT_CONFIG__COLOR_SWEEP_RANDOM[] PROGMEM = "!;;!";




// #ifndef ENABLE_DEVFEATURE_REMOVE__color_from_palette__
// /*
//  * Gets a single color from the currently selected palette.
//  * @param i Palette Index (if mapping is true, the full palette will be _virtualSegmentLength long, if false, 255). Will wrap around automatically.
//  * @param mapping if true, LED position in segment is considered for color
//  * @param wrap FastLED palettes will usally wrap back to the start smoothly. Set false to get a hard edge
//  * @param mcol If the default palette 0 is selected, return the standard color 0, 1 or 2 instead. If >2, Party palette is used instead
//  * @param pbri Value to scale the brightness of the returned color by. Default is 255. (no scaling)
//  * @returns Single color from palette
//  */
// uint32_t mAnimatorLight::color_from_palette(uint16_t i, bool mapping, bool wrap, uint8_t mcol, uint8_t pbri)
// {
  
//   #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   
//   ALOG_DBG(PSTR("f::color_from_palette to be phased out"));
//   #endif

//   if (SEGMENT.palette.id == 0 && mcol < 3) return SEGCOLOR_U32(mcol); //WS2812FX default
//   uint8_t paletteIndex = i;
//   if (mapping) paletteIndex = (i*255)/(_virtualSegmentLength -1);  // This scales out segment_index to SEGLEN as 0 to 255
//   // AddLog(LOG_LEVEL_TEST, PSTR("paletteIndex=%d"),paletteIndex);
//   if (!wrap) paletteIndex = scale8(paletteIndex, 240); //cut off blend at palette "end"
//   CRGB fastled_col;
//   fastled_col = ColorFromPalette( SEGMENT.palette_container->CRGB16Palette16_Palette.data, paletteIndex, pbri, (paletteBlend == 3)? NOBLEND:LINEARBLEND);
//   return  fastled_col.r*65536 +  fastled_col.g*256 +  fastled_col.b;

// }
// #endif // ENABLE_DEVFEATURE_REMOVE__color_from_palette__


/*
 * Put a value 0 to 255 in to get a color value.
 * The colours are a transition r -> g -> b -> back to r
 * Inspired by the Adafruit examples.
 */
uint32_t mAnimatorLight::color_wheel(uint8_t pos) {

#ifdef ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED

  if (SEGMENT.palette.id){ // when default, so it skips this, causes brightness error
    return SEGMENT.GetPaletteColour(pos, PALETTE_SPAN_OFF, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32();
  }else
  {

    ALOG_ERR(PSTR("Likely brightness error"));
  
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


#else
  float hue_f = mSupport::mapfloat(pos, 0, 255, 0.0f, 1.0f);
  HsbColor colh = HsbColor(hue_f,1,1);
  return RgbcctColor::GetU32Colour(colh);
#endif



}

#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED

/****************************************************************************************************************************
 **************************************************************************************************************************** 
 * Chase
 ****************************************************************************************************************************
 ****************************************************************************************************************************/

#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

// /*
//  * color chase function.
//  * color1 = background color
//  * color2 and color3 = colors of two adjacent leds
//  *
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Base_Chase(uint32_t color1, uint32_t color2, uint32_t color3, bool do_palette)
{
  
// void mAnimatorLight::chase(uint32_t color1, uint32_t color2, uint32_t color3, bool do_palette) {
  uint16_t counter = millis() * ((SEGMENT.speed() >> 2) + 1);
  uint16_t a = counter * SEGLEN  >> 16;

  bool chase_random = (SEGMENT.effect_id == EFFECTS_FUNCTION__WLED_CHASE_RANDOM__ID);
  if (chase_random) {
    if (a < SEGMENT.step) //we hit the start again, choose new color for Chase random
    {
      SEGMENT.params_internal.aux1 = SEGMENT.params_internal.aux0; //store previous random color
      SEGMENT.params_internal.aux0 = get_random_wheel_index(SEGMENT.params_internal.aux0);
    }
    color1 = color_wheel(SEGMENT.params_internal.aux0);
  }
  SEGMENT.step = a;

  // Use intensity() setting to vary chase up to 1/2 string length
  uint8_t size = 1 + (SEGMENT.intensity() * SEGLEN >> 10);

  uint16_t b = a + size; //"trail" of chase, filled with color1 
  if (b > SEGLEN) b -= SEGLEN;
  uint16_t c = b + size;
  if (c > SEGLEN) c -= SEGLEN;

  //background
  if (do_palette)
  {
    for(uint16_t i = 0; i < SEGLEN; i++) {
      SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
    }
  } else fill(color1);

  //if random, fill old background between a and end
  if (chase_random)
  {
    color1 = color_wheel(SEGMENT.params_internal.aux1);
    for (uint16_t i = a; i < SEGLEN; i++)
      SEGMENT.SetPixelColor(i, color1);
  }

  //fill between points a and b with color2
  if (a < b)
  {
    for (uint16_t i = a; i < b; i++)
      SEGMENT.SetPixelColor(i, color2);
  } else {
    for (uint16_t i = a; i < SEGLEN; i++) //fill until end
      SEGMENT.SetPixelColor(i, color2);
    for (uint16_t i = 0; i < b; i++) //fill from start until b
      SEGMENT.SetPixelColor(i, color2);
  }

  //fill between points b and c with color2
  if (b < c)
  {
    for (uint16_t i = b; i < c; i++)
      SEGMENT.SetPixelColor(i, color3);
  } else {
    for (uint16_t i = b; i < SEGLEN; i++) //fill until end
      SEGMENT.SetPixelColor(i, color3);
    for (uint16_t i = 0; i < c; i++) //fill from start until c
      SEGMENT.SetPixelColor(i, color3);
  }

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}


/*
 * Bicolor chase, more primary color.
 */
// void mAnimatorLight::mode_chase_color(void) {
//   return chase(SEGCOLOR_U32(1), (SEGCOLOR_U32(2)) ? SEGCOLOR_U32(2) : SEGCOLOR_U32(0), SEGCOLOR_U32(0), true);
// }

void mAnimatorLight::EffectAnim__Chase_Colour()
{
  EffectAnim__Base_Chase(SEGCOLOR_U32(1), (SEGCOLOR_U32(2)) ? SEGCOLOR_U32(2) : SEGCOLOR_U32(0), SEGCOLOR_U32(0), true);
}
static const char PM_EFFECT_CONFIG__CHASE_COLOR[] PROGMEM = "!,Width;!,!,!;!";



/*
 * Primary running followed by random color.
 */
// void mAnimatorLight::mode_chase_random(void) {
//   return chase(SEGCOLOR_U32(1), (SEGCOLOR_U32(2)) ? SEGCOLOR_U32(2) : SEGCOLOR_U32(0), SEGCOLOR_U32(0), false);
// }

// void mAnimatorLight::EffectAnim__Chase_Random()
// {
//   EffectAnim__Base_Chase(SEGCOLOR_U32(1), (SEGCOLOR_U32(2)) ? SEGCOLOR_U32(2) : SEGCOLOR_U32(0), SEGCOLOR_U32(0), false);
// }

/*
 * Primary, secondary running on rainbow.
 */
// void mAnimatorLight::mode_chase_rainbow(void) {
//   return chase();
// }
void mAnimatorLight::EffectAnim__Chase_Rainbow()
{
  uint8_t color_sep = 256 / SEGLEN;
  uint8_t color_index = SEGMENT.call & 0xFF;
  uint32_t color = color_wheel(((SEGMENT.step * color_sep) + color_index) & 0xFF);

  EffectAnim__Base_Chase(color, SEGCOLOR_U32(0), SEGCOLOR_U32(1), false);
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__CHASE_RAINBOW[] PROGMEM = "!,Width;!,!;!";


/*
 * Sec flashes running on prim.
 */
// #define FLASH_COUNT 4

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

// /*
//  * color chase function.
//  * color1 = background color
//  * color2 and color3 = colors of two adjacent leds
//  *

// void mAnimatorLight::mode_chase_flash(void) {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Chase_Flash()
// Base_Chase(uint32_t color1, uint32_t color2, uint32_t color3, bool do_palette)
{
  uint8_t flash_step = SEGMENT.call % ((FLASH_COUNT * 2) + 1);

  for(uint16_t i = 0; i < SEGLEN; i++) 
  {
    SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE)); // Functionally the same
  }

  uint16_t delay = 10 + ((30 * (uint16_t)(255 - SEGMENT.speed())) / SEGLEN);
  if(flash_step < (FLASH_COUNT * 2)) {
    if(flash_step % 2 == 0) {
      uint16_t n = SEGMENT.step;
      uint16_t m = (SEGMENT.step + 1) % SEGLEN;
      SEGMENT.SetPixelColor( n, SEGCOLOR_U32(1));
      SEGMENT.SetPixelColor( m, SEGCOLOR_U32(1));
      delay = 20;
    } else {
      delay = 30;
    }
  } else {
    SEGMENT.step = (SEGMENT.step + 1) % SEGLEN;
  }
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__CHASE_FLASH[] PROGMEM = "!;Bg,Fx;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
// /*
//  * Prim flashes running, followed by random color.
//  *
// void mAnimatorLight::mode_chase_flash_random(void) {

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Chase_Flash_Random()
{
  uint8_t flash_step = SEGMENT.call % ((FLASH_COUNT * 2) + 1);

  for(uint16_t i = 0; i < SEGMENT.step; i++) {
    SEGMENT.SetPixelColor(i, color_wheel(SEGMENT.params_internal.aux0));
  }

  uint16_t delay = 1 + ((10 * (uint16_t)(255 - SEGMENT.speed())) / SEGLEN);
  if(flash_step < (FLASH_COUNT * 2)) {
    uint16_t n = SEGMENT.step;
    uint16_t m = (SEGMENT.step + 1) % SEGLEN;
    if(flash_step % 2 == 0) {
      SEGMENT.SetPixelColor( n, SEGCOLOR_U32(0));
      SEGMENT.SetPixelColor( m, SEGCOLOR_U32(0));
      delay = 20;
    } else {
      SEGMENT.SetPixelColor( n, color_wheel(SEGMENT.params_internal.aux0));
      SEGMENT.SetPixelColor( m, SEGCOLOR_U32(1));
      delay = 30;
    }
  } else {
    SEGMENT.step = (SEGMENT.step + 1) % SEGLEN;

    if(SEGMENT.step == 0) {
      SEGMENT.params_internal.aux0 = get_random_wheel_index(SEGMENT.params_internal.aux0);
    }
  }
  // return delay;

  SEGMENT.animation_has_anim_callback = false; // When no animation callback is needed
          Serial.print("@4");
//  pCONT_iLight->ShowInterface();
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();

}
static const char PM_EFFECT_CONFIG__CHASE_FLASH_RANDOM[] PROGMEM = "!;!,!;!";


// /*
//  * Primary running on rainbow.
//  */
// void mAnimatorLight::mode_chase_rainbow_white(void) {
//   uint16_t n = SEGMENT.step;
//   uint16_t m = (SEGMENT.step + 1) % SEGLEN;
//   uint32_t color2 = color_wheel(((n * 256 / SEGLEN) + (SEGMENT.call & 0xFF)) & 0xFF);
//   uint32_t color3 = color_wheel(((m * 256 / SEGLEN) + (SEGMENT.call & 0xFF)) & 0xFF);

//   return chase(SEGCOLOR_U32(0), color2, color3, false);
// }
void mAnimatorLight::EffectAnim__Chase_Rainbow_White()
{

  uint16_t n = SEGMENT.step;
  uint16_t m = (SEGMENT.step + 1) % SEGLEN;
  uint32_t color2 = color_wheel(((n * 256 / SEGLEN) + (SEGMENT.call & 0xFF)) & 0xFF);
  uint32_t color3 = color_wheel(((m * 256 / SEGLEN) + (SEGMENT.call & 0xFF)) & 0xFF);

  EffectAnim__Base_Chase(SEGCOLOR_U32(0), color2, color3, false);
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__CHASE_RAINBOW_WHITE[] PROGMEM = "!,Size;Bg;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
/*
 * theater chase function
 *
void mAnimatorLight::theater_chase(uint32_t color1, uint32_t color2, bool do_palette) {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Base_Chase_Theater(uint32_t color1, uint32_t color2, bool do_palette)
{


  byte gap = 2 + ((255 - SEGMENT.intensity()) >> 5);
  uint32_t cycleTime = 50 + (255 - SEGMENT.speed())*2;
  uint32_t it = millis() / cycleTime;
  if (it != SEGMENT.step) //new color
  {
    SEGMENT.params_internal.aux0 = (SEGMENT.params_internal.aux0 +1) % gap;
    SEGMENT.step = it;
  }
  
  for(uint16_t i = 0; i < SEGLEN; i++) {
    if((i % gap) == SEGMENT.params_internal.aux0) {
      if (do_palette)
      {
        SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE)); // Functionally the same
      } else {
        SEGMENT.SetPixelColor(i, color1);
      }
    } else {
      SEGMENT.SetPixelColor(i, color2);
    }
  }
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  return;
}


/*
 * Theatre-style crawling lights.
 * Inspired by the Adafruit examples.
 */
void mAnimatorLight::EffectAnim__Chase_Theater(void) {
  return EffectAnim__Base_Chase_Theater(SEGCOLOR_U32(0), SEGCOLOR_U32(1), true);
}
static const char PM_EFFECT_CONFIG__THEATER_CHASE[] PROGMEM = "!,Gap size;!,!;!";


/*
 * Theatre-style crawling lights with rainbow effect.
 * Inspired by the Adafruit examples.
 */
void mAnimatorLight::EffectAnim__Chase_Theatre_Rainbow(void) {
  return EffectAnim__Base_Chase_Theater(color_wheel(SEGMENT.step), SEGCOLOR_U32(1), false);
}
static const char PM_EFFECT_CONFIG__THEATER_CHASE_RAINBOW[] PROGMEM = "!,Gap size;,!;!";




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
/*
 * Tricolor chase function
 *
void mAnimatorLight::tricolor_chase(uint32_t color1, uint32_t color2) {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Base_Chase_TriColour(uint32_t color1, uint32_t color2)
{
  
  uint32_t cycleTime = 50 + (255 - SEGMENT.speed())*2;
  uint32_t it = millis() / cycleTime;
  uint8_t width = (1 + SEGMENT.intensity()/32) * 3; //value of 1-8 for each colour
  uint8_t index = it % width;
  
  for(uint16_t i = 0; i < SEGLEN; i++, index++) {
    if(index > width-1) index = 0;

    uint32_t color = color1;
    if(index > width*2/3-1){
      // color = RgbcctColor::GetU32Colour(SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE)); // Functionally the same
      color = RgbcctColor::GetU32Colour(SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON));      
    }
    else if(index > width/3-1) color = color2;

    SEGMENT.SetPixelColor(SEGLEN - i -1, color);
  }

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  

}


/*
 * Tricolor chase mode
 */
void mAnimatorLight::EffectAnim__Chase_TriColour(void) {

  return EffectAnim__Base_Chase_TriColour(SEGCOLOR_U32(2), SEGCOLOR_U32(0));
}
static const char PM_EFFECT_CONFIG__TRICOLOR_CHASE[] PROGMEM = "!,Size;1,2,3;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
/*
 * Running random pixels
 * Custom mode by Keith Lord: https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/RandomChase.h
 *
void mAnimatorLight::mode_random_chase(void)
{
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Chase_Random()
{

  uint32_t cycleTime = 25 + (3 * (uint32_t)(255 - SEGMENT.speed()));
  uint32_t it = millis() / cycleTime;
  if (SEGMENT.step == it){return;}// return FRAMETIME_MS;

  for(uint16_t i = SEGLEN -1; i > 0; i--) {
    SEGMENT.SetPixelColor(i, SEGMENT.GetPixelColor(i-1));
  }
  uint32_t color = RgbcctColor::GetU32Colour(SEGMENT.GetPixelColor(0));
  if (SEGLEN > 1) color = RgbcctColor::GetU32Colour(SEGMENT.GetPixelColor( 1));
  uint8_t r = random8(6) != 0 ? (color >> 16 & 0xFF) : random8();
  uint8_t g = random8(6) != 0 ? (color >> 8  & 0xFF) : random8();
  uint8_t b = random8(6) != 0 ? (color       & 0xFF) : random8();
  SEGMENT.SetPixelColor(0, r, g, b);

  SEGMENT.step = it;
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  return;
}
static const char PM_EFFECT_CONFIG__CHASE_RANDOM[] PROGMEM = "!,Width;!,,!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Exploding fireworks effect
 * @note : Converted from WLED Effect
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
void mAnimatorLight::EffectAnim__Breath()
{

  uint16_t var = 0;
  uint16_t counter = (millis() * ((SEGMENT.speed() >> 3) +10));
  counter = (counter >> 2) + (counter >> 4); //0-16384 + 0-2048
  if (counter < 16384) {
    if (counter > 8192) counter = 8192 - (counter - 8192);
    var = sin16(counter) / 103; //close to parabolic in range 0-8192, max val. 23170
  }
  
  uint8_t lum = 30 + var;
  for(uint16_t i = 0; i < SEGLEN; i++) 
  {
    SEGMENT.SetPixelColor(i, ColourBlend(SEGCOLOR_RGBCCT(1), SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON), lum) );
  }

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__BREATH[] PROGMEM = "!;!,!;!;01";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Fade
 * @note : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
void mAnimatorLight::EffectAnim__Fade()
{
  uint16_t counter = (millis() * ((SEGMENT.speed() >> 3) +10));
  uint8_t lum = triwave16(counter) >> 8;

  for(uint16_t i = 0; i < SEGLEN; i++)
  {
    SEGMENT.SetPixelColor(
      i, 
      ColourBlend(SEGCOLOR_RGBCCT(1), SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON), lum)
    );
  }

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__FADE[] PROGMEM = "!;!,!;!;01";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE


#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Fireworks Base Function
 * @note : Converted from WLED Effects
 * 
 * @param aux0 Current center index of firework (which blurs outwards)
 * @param aux1 Previous location of center index of firework
 * @param aux2 Iter to wrap palette
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Fireworks()
{
  
  if (SEGMENT.call == 0) {
    SEGMENT.fill(SEGCOLOR_U32(1));
    SEGMENT.params_internal.aux0 = UINT16_MAX;
    SEGMENT.params_internal.aux1 = UINT16_MAX;
  }

  SEGMENT.fade_out(128);

  bool valid1 = (SEGMENT.params_internal.aux0 < SEGLEN);
  bool valid2 = (SEGMENT.params_internal.aux1 < SEGLEN);
  RgbcctColor sv1 = 0, sv2 = 0;
  if (valid1) sv1 = SEGMENT.GetPixelColor(SEGMENT.params_internal.aux0);
  if (valid2) sv2 = SEGMENT.GetPixelColor(SEGMENT.params_internal.aux1);
  blur(255-SEGMENT.speed(), SET_BRIGHTNESS);
  if (valid1)       SEGMENT.SetPixelColor(SEGMENT.params_internal.aux0, sv1);
  if (valid2)       SEGMENT.SetPixelColor(SEGMENT.params_internal.aux1, sv2);

  /**
   * @brief This actually only runs when a new colour is made, 
   * and its index is stored so the next animation call will propogate it out
   **/
  RgbcctColor colour;
  for(uint16_t i=0; i<MAX(1, SEGLEN/20); i++) 
  { 
    if(random8(129 - (SEGMENT.intensity() >> 1)) == 0) 
    {
      uint16_t index = random(SEGLEN);
      colour = SEGMENT.GetPaletteColour(random8(), PALETTE_SPAN_OFF, PALETTE_WRAP_OFF);
      SEGMENT.SetPixelColor(index, colour, SET_BRIGHTNESS);
      SEGMENT.params_internal.aux1 = SEGMENT.params_internal.aux0;
      SEGMENT.params_internal.aux0 = index;
    }
  }
  
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__FIREWORKS[] PROGMEM = ",Frequency;!,!;!;12;ix=192,pal=11";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Fireworks Starburst
 * @note : Converted from WLED Effects
 * 
 * @param aux0 Current center index of firework (which blurs outwards)
 * @param aux1 Previous location of center index of firework
 * @param aux2 Iter to wrap palette
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

#define STARBURST_MAX_FRAG 12

//each needs 64 byte
typedef struct particle {
  CRGB     color;
  uint32_t birth  =0;
  uint32_t last   =0;
  float    vel    =0;
  uint16_t pos    =-1;
  float    fragment[STARBURST_MAX_FRAG];
} star;

void mAnimatorLight::EffectAnim__Fireworks_Starburst()
{

  uint8_t numStars = 1 + (SEGLEN >> 3);
  if (numStars > 15) numStars = 15;
  uint16_t dataSize = sizeof(star) * numStars;

  if (!SEGMENT.allocateData(dataSize)){    
    ALOG_ERR( PM_JSON_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = DEFAULT_EFFECTS_FUNCTION;
    return;
  }
  
  uint32_t it = millis();
  
  star* stars = reinterpret_cast<star*>(SEGMENT.data);
  
  float          maxSpeed                = 375.0f;  // Max velocity
  float          particleIgnition        = 250.0f;  // How long to "flash"
  float          particleFadeTime        = 1500.0f; // Fade out time
     
  for (int j = 0; j < numStars; j++)
  {
    // speed() to adjust chance of a burst, max is nearly always.
    if (random8((144-(SEGMENT.speed() >> 1))) == 0 && stars[j].birth == 0)
    {
      // Pick a random color and location.  
      uint16_t startPos = random16(SEGLEN-1);
      float multiplier = (float)(random8())/255.0 * 1.0;

      stars[j].color = col_to_crgb(color_wheel(random8()));
      stars[j].pos = startPos; 
      stars[j].vel = maxSpeed * (float)(random8())/255.0 * multiplier;
      stars[j].birth = it;
      stars[j].last = it;
      // more fragments means larger burst effect
      int num = random8(3,6 + (SEGMENT.intensity() >> 5));

      for (int i=0; i < STARBURST_MAX_FRAG; i++) {
        if (i < num) stars[j].fragment[i] = startPos;
        else stars[j].fragment[i] = -1;
      }
    }
  }
  
  fill(SEGCOLOR_U32(1), SET_BRIGHTNESS);
  
  for (int j=0; j<numStars; j++)
  {
    if (stars[j].birth != 0) {
      float dt = (it-stars[j].last)/1000.0;

      for (int i=0; i < STARBURST_MAX_FRAG; i++) {
        int var = i >> 1;
        
        if (stars[j].fragment[i] > 0) {
          //all fragments travel right, will be mirrored on other side
          stars[j].fragment[i] += stars[j].vel * dt * (float)var/3.0;
        }
      }
      stars[j].last = it;
      stars[j].vel -= 3*stars[j].vel*dt;
    }
  
    CRGB c = stars[j].color;

    // If the star is brand new, it flashes white briefly.  
    // Otherwise it just fades over time.
    float fade = 0.0f;
    float age = it-stars[j].birth;

    if (age < particleIgnition) {
      c = col_to_crgb(ColourBlend(WHITE, crgb_to_col(c), 254.5f*((age / particleIgnition))));
    } else {
      // Figure out how much to fade and shrink the star based on 
      // its age relative to its lifetime
      if (age > particleIgnition + particleFadeTime) {
        fade = 1.0f;                  // Black hole, all faded out
        stars[j].birth = 0;
        c = col_to_crgb(SEGCOLOR_U32(1));
      } else {
        age -= particleIgnition;
        fade = (age / particleFadeTime);  // Fading star
        byte f = 254.5f*fade;
        c = col_to_crgb(ColourBlend(crgb_to_col(c), SEGCOLOR_U32(1), f));
      }
    }
    
    float particleSize = (1.0 - fade) * 2;

    for (uint8_t index=0; index < STARBURST_MAX_FRAG*2; index++) {
      bool mirrored = index & 0x1;
      uint8_t i = index >> 1;
      if (stars[j].fragment[i] > 0) {
        float loc = stars[j].fragment[i];
        if (mirrored) loc -= (loc-stars[j].pos)*2;
        int start = loc - particleSize;
        int end = loc + particleSize;
        if (start < 0) start = 0;
        if (start == end) end++;
        if (end > SEGLEN) end = SEGLEN;    
        for (int p = start; p < end; p++) {
          SEGMENT.SetPixelColor(p, c.r, c.g, c.b, SET_BRIGHTNESS);
        }
      }
    }
  }

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__STARBURST[] PROGMEM = "Chance,Fragments,,,,,,,Overlay;,!;!;;pal=11,m12=0";


/**
 * @brief To introduce much slower fade out, have it burst and fade out very slowly
 * 
 */
void mAnimatorLight::EffectAnim__Fireworks_Starburst_Glows()
{
 
  uint8_t numStars = 1 + (SEGLEN >> 3);
  if (numStars > 15) numStars = 15;
  uint16_t dataSize = sizeof(star) * numStars;

  if (!SEGMENT.allocateData(dataSize)){    
    ALOG_ERR( PM_JSON_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = DEFAULT_EFFECTS_FUNCTION;
    return;
  }
  
  uint32_t it = millis();
  
  star* stars = reinterpret_cast<star*>(SEGMENT.data);
  
  float          maxSpeed                = 375.0f;  // Max velocity
  float          particleIgnition        = 250.0f;  // How long to "flash"
  float          particleFadeTime        = 1500.0f; // Fade out time
     
  for (int j = 0; j < numStars; j++)
  {
    // speed() to adjust chance of a burst, max is nearly always.
    if (random8((144-(SEGMENT.speed() >> 1))) == 0 && stars[j].birth == 0)
    {
      // Pick a random color and location.  
      uint16_t startPos = random16(SEGLEN-1);
      float multiplier = (float)(random8())/255.0 * 1.0;

      stars[j].color = col_to_crgb(color_wheel(random8()));
      stars[j].pos = startPos; 
      stars[j].vel = maxSpeed * (float)(random8())/255.0 * multiplier;
      stars[j].birth = it;
      stars[j].last = it;
      // more fragments means larger burst effect
      int num = random8(3,6 + (SEGMENT.intensity() >> 5));

      for (int i=0; i < STARBURST_MAX_FRAG; i++) {
        if (i < num) stars[j].fragment[i] = startPos;
        else stars[j].fragment[i] = -1;
      }
    }
  }
  
  fill(SEGCOLOR_U32(1), SET_BRIGHTNESS);
  
  for (int j=0; j<numStars; j++)
  {
    if (stars[j].birth != 0) {
      float dt = (it-stars[j].last)/1000.0;

      for (int i=0; i < STARBURST_MAX_FRAG; i++) {
        int var = i >> 1;
        
        if (stars[j].fragment[i] > 0) {
          //all fragments travel right, will be mirrored on other side
          stars[j].fragment[i] += stars[j].vel * dt * (float)var/3.0;
        }
      }
      stars[j].last = it;
      stars[j].vel -= 3*stars[j].vel*dt;
    }
  
    CRGB c = stars[j].color;

    // If the star is brand new, it flashes white briefly.  
    // Otherwise it just fades over time.
    float fade = 0.0f;
    float age = it-stars[j].birth;

    if (age < particleIgnition) {
      c = col_to_crgb(ColourBlend(WHITE, crgb_to_col(c), 254.5f*((age / particleIgnition))));
    } else {
      // Figure out how much to fade and shrink the star based on 
      // its age relative to its lifetime
      if (age > particleIgnition + particleFadeTime) {
        fade = 1.0f;                  // Black hole, all faded out
        stars[j].birth = 0;
        c = col_to_crgb(SEGCOLOR_U32(1));
      } else {
        age -= particleIgnition;
        fade = (age / particleFadeTime);  // Fading star
        byte f = 254.5f*fade;
        c = col_to_crgb(ColourBlend(crgb_to_col(c), SEGCOLOR_U32(1), f));
      }
    }
    
    float particleSize = (1.0 - fade) * 2;

    for (uint8_t index=0; index < STARBURST_MAX_FRAG*2; index++) {
      bool mirrored = index & 0x1;
      uint8_t i = index >> 1;
      if (stars[j].fragment[i] > 0) {
        float loc = stars[j].fragment[i];
        if (mirrored) loc -= (loc-stars[j].pos)*2;
        int start = loc - particleSize;
        int end = loc + particleSize;
        if (start < 0) start = 0;
        if (start == end) end++;
        if (end > SEGLEN) end = SEGLEN;    
        for (int p = start; p < end; p++) {
          SEGMENT.SetPixelColor(p, c.r, c.g, c.b, SET_BRIGHTNESS);
        }
      }
    }
  }

  // SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__STARBURST_GLOWS[] PROGMEM = "Chance,Fragments,,,,,,,Overlay;,!;!;;pal=11,m12=0";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Exploding fireworks effect
 * @note : Converted from WLED Effects
 *
 * Exploding fireworks effect
 * adapted from: http://www.anirama.com/1000leds/1d-fireworks/
 * @param aux0 Current center index of firework (which blurs outwards)
 * @param aux1 Previous location of center index of firework
 * @param aux2 Iter to wrap palette
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Exploding_Fireworks()
{

  //allocate segment data
  uint16_t numSparks = 2 + (SEGLEN >> 1); 
  if (numSparks > 80) numSparks = 80;

  uint16_t dataSize = sizeof(spark) * numSparks;

  if (!SEGMENT.allocateData(dataSize)){    
    ALOG_ERR( PM_JSON_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = DEFAULT_EFFECTS_FUNCTION;
    return;
  }

  fill(BLACK, SET_BRIGHTNESS);
  
  bool actuallyReverse = SEGMENT.getOption(SEG_OPTION_REVERSED);
  //have fireworks start in either direction based on intensity()
  SEGMENT.setOption(SEG_OPTION_REVERSED, SEGMENT.step);
  
  Spark* sparks = reinterpret_cast<Spark*>(SEGMENT.data);
  Spark* flare = sparks; //first spark is flare data

  float gravity = -0.0004 - (SEGMENT.speed()/800000.0); // m/s/s
  gravity *= SEGLEN;
  
  if (SEGMENT.params_internal.aux0 < 2) { //FLARE
    if (SEGMENT.params_internal.aux0 == 0) { //init flare
      flare->pos = 0;
      uint16_t peakHeight = 75 + random8(180); //0-255
      peakHeight = (peakHeight * (SEGLEN -1)) >> 8;
      flare->vel = sqrt(-2.0 * gravity * peakHeight);
      flare->col = 255; //brightness
      SEGMENT.params_internal.aux0 = 1; 
    }
    
    // launch 
    if (flare->vel > 12 * gravity) {
      // flare
      SEGMENT.SetPixelColor(int(flare->pos),flare->col,flare->col,flare->col, SET_BRIGHTNESS);
  
      flare->pos += flare->vel;
      flare->pos = constrain(flare->pos, 0, SEGLEN-1);
      flare->vel += gravity;
      flare->col -= 2;
    } else {
      SEGMENT.params_internal.aux0 = 2;  // ready to explode
    }
  } else if (SEGMENT.params_internal.aux0 < 4) {
    /*
     * Explode!
     * 
     * Explosion happens where the flare ended.
     * Size is proportional to the height.
     */
    int nSparks = flare->pos;
    nSparks = constrain(nSparks, 0, numSparks);
    static float dying_gravity;
  
    // initialize sparks
    if (SEGMENT.params_internal.aux0 == 2) {
      for (int i = 1; i < nSparks; i++) { 
        sparks[i].pos = flare->pos; 
        sparks[i].vel = (float(random16(0, 20000)) / 10000.0) - 0.9; // from -0.9 to 1.1
        sparks[i].col = 345;//abs(sparks[i].vel * 750.0); // set colors before scaling velocity to keep them bright 
        //sparks[i].col = constrain(sparks[i].col, 0, 345); 
        sparks[i].colIndex = random8();
        sparks[i].vel *= flare->pos/SEGLEN; // proportional to height 
        sparks[i].vel *= -gravity *50;
      } 
      //sparks[1].col = 345; // this will be our known spark 
      dying_gravity = gravity/2; 
      SEGMENT.params_internal.aux0 = 3;
    }
  
    if (sparks[1].col > 4) {//&& sparks[1].pos > 0) { // as long as our known spark is lit, work with all the sparks
      for (int i = 1; i < nSparks; i++) { 
        sparks[i].pos += sparks[i].vel; 
        sparks[i].vel += dying_gravity; 
        if (sparks[i].col > 3) sparks[i].col -= 4; 

        if (sparks[i].pos > 0 && sparks[i].pos < SEGLEN) {
          uint16_t prog = sparks[i].col;
          uint32_t spColor = (SEGMENT.palette.id) ? color_wheel(sparks[i].colIndex) : SEGCOLOR_U32(0);
          // uint32_t spColor = RgbcctColor::GetU32Colour(RgbColor(0,255,10));
          CRGB c = HTMLColorCode::Black; //HeatColor(sparks[i].col);
          if (prog > 300) { //fade from white to spark color
            c = col_to_crgb(ColourBlend(spColor, WHITE, (prog - 300)*5));
          } else if (prog > 45) { //fade from spark color to black
            c = col_to_crgb(ColourBlend(BLACK, spColor, prog - 45));
            uint8_t cooling = (300 - prog) >> 5;
            c.g = qsub8(c.g, cooling);
            c.b = qsub8(c.b, cooling * 2);
          }
          SEGMENT.SetPixelColor(int(sparks[i].pos), c.red, c.green, c.blue, SET_BRIGHTNESS);
        }
      }
      dying_gravity *= .99; // as sparks burn out they fall slower
    } else {
      SEGMENT.params_internal.aux0 = 6 + random8(10); //wait for this many frames
    }
  } else {
    SEGMENT.params_internal.aux0--;
    if (SEGMENT.params_internal.aux0 < 4) {
      SEGMENT.params_internal.aux0 = 0; //back to flare
      SEGMENT.step = (SEGMENT.intensity() > random8()); //decide firing side
    }
  }

  SEGMENT.setOption(SEG_OPTION_REVERSED, actuallyReverse);
  
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();

}
static const char PM_EFFECT_CONFIG__EXPLODING_FIREWORKS[] PROGMEM = "Gravity,Firing side;!,!;!;12;pal=11,ix=128";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Exploding fireworks effect
 * @note : Converted from WLED Effects
 *
 * Exploding fireworks effect
 * adapted from: http://www.anirama.com/1000leds/1d-fireworks/
 * @param aux0 Current center index of firework (which blurs outwards)
 * @param aux1 Previous location of center index of firework
 * @param aux2 Iter to wrap palette
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Exploding_Fireworks_NoLaunch()
{

  //allocate segment data
  uint16_t numSparks = 2 + (SEGLEN >> 1); 
  if (numSparks > 80) numSparks = 80;

  uint16_t dataSize = sizeof(spark) * numSparks;

  if (!SEGMENT.allocateData(dataSize)){    
    ALOG_ERR( PM_JSON_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = DEFAULT_EFFECTS_FUNCTION;
    return;
  }

  fill(BLACK, SET_BRIGHTNESS);
  
  bool actuallyReverse = SEGMENT.getOption(SEG_OPTION_REVERSED);
  //have fireworks start in either direction based on intensity()
  SEGMENT.setOption(SEG_OPTION_REVERSED, SEGMENT.step);
  
  Spark* sparks = reinterpret_cast<Spark*>(SEGMENT.data);
  Spark* flare = sparks; //first spark is flare data

  float gravity = -0.0004 - (SEGMENT.speed()/800000.0); // m/s/s
  gravity *= SEGLEN;
  
  if (SEGMENT.params_internal.aux0 < 2) { //FLARE

    // if (SEGMENT.params_internal.aux0 == 0) { //init flare
      flare->pos = 0;
      uint16_t peakHeight = 75 + random8(180); //0-255
      peakHeight = (peakHeight * (SEGLEN -1)) >> 8;
      flare->vel = sqrt(-2.0 * gravity * peakHeight);
      flare->col = 255; //brightness
      SEGMENT.params_internal.aux0 = 1; 
    // }
    
    // // launch 
    // if (flare->vel > 12 * gravity) {
    //   // flare
    //   SEGMENT.SetPixelColor(int(flare->pos),flare->col,flare->col,flare->col, SET_BRIGHTNESS);
  
      flare->pos += flare->vel;
      flare->pos = constrain(flare->pos, 0, SEGLEN-1);
      flare->vel += gravity;
      flare->col -= 2;

      /**
       * @brief Starts in the middle
       **/
      // flare->pos = SEGLEN/2;
      /**
       * @brief Starts randomly
       **/
      flare->pos = random(0,SEGLEN);

    // } else {
      SEGMENT.params_internal.aux0 = 2;  // ready to explode
    // }

  } else if (SEGMENT.params_internal.aux0 < 4) {
    /*
     * Explode!
     * 
     * Explosion happens where the flare ended.
     * Size is proportional to the height.
     */
    int nSparks = flare->pos;
    nSparks = constrain(nSparks, 0, numSparks);
    static float dying_gravity;
  
    // initialize sparks
    if (SEGMENT.params_internal.aux0 == 2) {
      for (int i = 1; i < nSparks; i++) { 
        sparks[i].pos = flare->pos; 
        sparks[i].vel = (float(random16(0, 20000)) / 10000.0) - 0.9; // from -0.9 to 1.1
        sparks[i].col = 345;//abs(sparks[i].vel * 750.0); // set colors before scaling velocity to keep them bright 
        //sparks[i].col = constrain(sparks[i].col, 0, 345); 
        sparks[i].colIndex = random8();
        sparks[i].vel *= flare->pos/SEGLEN; // proportional to height 
        sparks[i].vel *= -gravity *50;
      } 
      //sparks[1].col = 345; // this will be our known spark 
      dying_gravity = gravity/2; 
      SEGMENT.params_internal.aux0 = 3;
    }
  
    if (sparks[1].col > 4) {//&& sparks[1].pos > 0) { // as long as our known spark is lit, work with all the sparks
      for (int i = 1; i < nSparks; i++) { 
        sparks[i].pos += sparks[i].vel; 
        sparks[i].vel += dying_gravity; 
        if (sparks[i].col > 3) sparks[i].col -= 4; 

        if (sparks[i].pos > 0 && sparks[i].pos < SEGLEN) {
          uint16_t prog = sparks[i].col;
          uint32_t spColor = (SEGMENT.palette.id) ? color_wheel(sparks[i].colIndex) : SEGCOLOR_U32(0);
          // uint32_t spColor = RgbcctColor::GetU32Colour(RgbColor(0,255,10));
          CRGB c = HTMLColorCode::Black; //HeatColor(sparks[i].col);
          if (prog > 300) { //fade from white to spark color
            c = col_to_crgb(ColourBlend(spColor, WHITE, (prog - 300)*5));
          } else if (prog > 45) { //fade from spark color to black
            c = col_to_crgb(ColourBlend(BLACK, spColor, prog - 45));
            uint8_t cooling = (300 - prog) >> 5;
            c.g = qsub8(c.g, cooling);
            c.b = qsub8(c.b, cooling * 2);
          }
          SEGMENT.SetPixelColor(int(sparks[i].pos), c.red, c.green, c.blue, SET_BRIGHTNESS);
        }
      }
      dying_gravity *= .99; // as sparks burn out they fall slower
    } else {
      SEGMENT.params_internal.aux0 = 6 + random8(10); //wait for this many frames
    }
  } else {
    SEGMENT.params_internal.aux0--;
    if (SEGMENT.params_internal.aux0 < 4) {
      SEGMENT.params_internal.aux0 = 0; //back to flare
      SEGMENT.step = (SEGMENT.intensity() > random8()); //decide firing side
    }
  }

  SEGMENT.setOption(SEG_OPTION_REVERSED, actuallyReverse);
  
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();

}
static const char PM_EFFECT_CONFIG__EXPLODING_FIREWORKS_NOLAUNCH[] PROGMEM = "Gravity,Firing side;!,!;!;12;pal=11,ix=128";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Rain effect, which uses firework
 * @note : Converted from WLED Effects
 * 
 * Exploding fireworks effect
 * adapted from: http://www.anirama.com/1000leds/1d-fireworks/
 *
 * Twinkling LEDs running. Inspired by https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/Rain.h
 @param aux0 Current center index of firework (which blurs outwards)
 @param aux1 Previous location of center index of firework
 @param aux2 Iter to wrap palette
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Rain()
{
  
  SEGMENT.step += FRAMETIME_MS;
  if (SEGMENT.step > SPEED_FORMULA_L) 
  {
    SEGMENT.step = 0;
    //shift all leds right
    RgbcctColor ctemp = SEGMENT.GetPixelColor(0);
    for(uint16_t i = 0; i < SEGLEN; i++) {
      SEGMENT.SetPixelColor(i, SEGMENT.GetPixelColor(i+1), true);
    }
    SEGMENT.SetPixelColor(SEGLEN - 1, ctemp, true);
    SEGMENT.params_internal.aux0++;
    SEGMENT.params_internal.aux1++;
    if (SEGMENT.params_internal.aux0 == 0)      SEGMENT.params_internal.aux0 = UINT16_MAX;
    if (SEGMENT.params_internal.aux1 == 0)      SEGMENT.params_internal.aux0 = UINT16_MAX;
    if (SEGMENT.params_internal.aux0 == SEGLEN) SEGMENT.params_internal.aux0 = 0;
    if (SEGMENT.params_internal.aux1 == SEGLEN) SEGMENT.params_internal.aux1 = 0;
  }
  
  EffectAnim__Fireworks();
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();  

}
static const char PM_EFFECT_CONFIG__RAIN[] PROGMEM = "!,Spawning rate;!,!;!;12;ix=128,pal=0";

#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED


#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

/****************************************************************************************************************************
 **************************************************************************************************************************** 
 * Sparkle/Twinkle
 ****************************************************************************************************************************
 ****************************************************************************************************************************/


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Rain effect, which uses firework
 * @note : Converted from WLED Effects
 * 
 * 
 * Blinks one LED at a time.
 * Inspired by www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/
 *
 *******************************************************************************************************************************************************************************************************************
 ******************************************************************************************************************/
void mAnimatorLight::EffectAnim__Sparkle() // Firework_Rain
{
  
  for(uint16_t i = 0; i < SEGLEN; i++) 
  {
    SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON));
  }
  uint32_t cycleTime = 10 + (255 - SEGMENT.speed())*2;
  uint32_t it = millis() / cycleTime;
  if (it != SEGMENT.step)
  {
    SEGMENT.params_internal.aux0 = random16(SEGLEN); // aux0 stores the random led index
    SEGMENT.step = it;
  }
  
  SEGMENT.SetPixelColor(SEGMENT.params_internal.aux0, SEGCOLOR_RGBCCT(0));  

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__SPARKLE[] PROGMEM = "!,,,,,,,,Overlay;!,!;!;;m12=0";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : 
 * @note : 
 * 
 * Lights all LEDs in the color. Flashes single col 1 pixels randomly. (List name: Sparkle Dark)
 * Inspired by www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Sparkle_Flash() // Firework_Rain
{
  
  for(uint16_t i = 0; i < SEGLEN; i++) {
    SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON));
  }

  if(random8(5) == 0) {
    SEGMENT.params_internal.aux0 = random16(SEGLEN); // aux0 stores the random led index
    SEGMENT.SetPixelColor(SEGMENT.params_internal.aux0, SEGCOLOR_RGBCCT(1));
    SEGMENT.transition.rate_ms = 20;//return 20;
  } 
  SEGMENT.transition.rate_ms =  20 + (uint16_t)(255-SEGMENT.speed());
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__FLASH_SPARKLE[] PROGMEM = "!,!,,,,,,,Overlay;Bg,Fx;!;;m12=0";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Rain effect, which uses firework
 * @note : Converted from WLED Effects
 * 
 * Like flash sparkle. With more flash.
 * Inspired by www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/
   Twinkling LEDs running. Inspired by https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/Rain.h
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Sparkle_Hyper() // Firework_Rain
{

  for(uint16_t i = 0; i < SEGLEN; i++) {
    SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON));
  }

  if(random8(5) < 2) {
    for(uint16_t i = 0; i < MAX(1, SEGLEN/3); i++) {
      SEGMENT.SetPixelColor(random16(SEGLEN), SEGCOLOR_U32(1));
    }
    SEGMENT.transition.rate_ms =  20;
  }
  SEGMENT.transition.rate_ms =  20 + (uint16_t)(255-SEGMENT.speed());
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__HYPER_SPARKLE[] PROGMEM = "!,!,,,,,,,Overlay;Bg,Fx;!;;m12=0";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Rain effect, which uses firework
 * @note : Converted from WLED Effects
 * Twinkling LEDs running. Inspired by https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/Rain.h
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
void mAnimatorLight::EffectAnim__Twinkle_Up() // Firework_Rain
{
  
  random16_set_seed(535);                                 // The randomizer needs to be re-set each time through the loop in order for the same 'random' numbers to be the same each time through.

  for (int i = 0; i<SEGLEN; i++) {
    uint8_t ranstart = random8();                         // The starting value (aka brightness) for each pixel. Must be consistent each time through the loop for this to work.
    uint8_t pixBri = sin8(ranstart + 16 * millis()/(256-SEGMENT.speed()));
    if (random8() > SEGMENT.intensity()) pixBri = 0;
    // SEGMENT.SetPixelColor(i, ColourBlend(SEGCOLOR_U32(1), RgbcctColor::GetU32Colour(mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette.id, i*20, nullptr, false, PALETTE_SOLID_WRAP, 0)), pixBri));
    // CustomPalettes not working now since i*20 is not scaled into 255, so only my first pixel is showing
    SEGMENT.SetPixelColor(i, ColourBlend(SEGCOLOR_RGBCCT(1), SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON), pixBri) );
  }

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__TWINKLEUP[] PROGMEM = "!,Intensity;!,!;!;;m12=0";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effect 
 * Lights all LEDs in one random color up. Then switches them to the next random color.
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
void mAnimatorLight::EffectAnim__Random_Colour()
{
  uint32_t cycleTime = 200 + (255 - SEGMENT.speed())*50;
  uint32_t it = millis() / cycleTime;
  uint32_t rem = millis() % cycleTime;
  uint16_t fadedur = (cycleTime * SEGMENT.intensity()) >> 8;

  uint32_t fade = 255;
  if (fadedur) {
    fade = (rem * 255) / fadedur;
    if (fade > 255) fade = 255;
  }

  if (SEGMENT.call == 0) {
    SEGMENT.params_internal.aux0 = random8();
    SEGMENT.step = 2;
  }
  if (it != SEGMENT.step) //new color
  {
    SEGMENT.params_internal.aux1 = SEGMENT.params_internal.aux0;
    SEGMENT.params_internal.aux0 = get_random_wheel_index(SEGMENT.params_internal.aux0); //aux0 will store our random color wheel index
    SEGMENT.step = it;
  }

  fill(ColourBlend(color_wheel(SEGMENT.params_internal.aux1), color_wheel(SEGMENT.params_internal.aux0), fade), true);
  
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__RANDOM_COLOR[] PROGMEM = "!,Fade time;;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
void mAnimatorLight::EffectAnim__Rainbow_Cycle()
{

/*
 * Cycles a rainbow over the entire string of LEDs.
 */
// void mAnimatorLight::mode_rainbow_cycle(void) {
  uint16_t counter = (millis() * ((SEGMENT.speed() >> 2) +2)) & 0xFFFF;
  counter = counter >> 8;
  
  for(uint16_t i = 0; i < SEGLEN; i++) {
    //intensity()/29 = 0 (1/16) 1 (1/8) 2 (1/4) 3 (1/2) 4 (1) 5 (2) 6 (4) 7 (8) 8 (16)
    uint8_t index = (i * (16 << (SEGMENT.intensity() /29)) / SEGLEN) + counter;
    SEGMENT.SetPixelColor(i, color_wheel(index));
  }

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__RAINBOW_CYCLE[] PROGMEM = "!,Size;;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Running
 * @description: Running lights effect with smooth sine transition base.
 * @note : Converted from WLED Effects
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
void mAnimatorLight::EffectAnim__Base_Running(bool saw)
{
  uint8_t x_scale = SEGMENT.intensity() >> 2;
  uint32_t counter = (millis() * SEGMENT.speed()) >> 9;

  for(uint16_t i = 0; i < SEGLEN; i++) {
    uint8_t s = 0;
    uint8_t a = i*x_scale - counter;
    if (saw) {
      if (a < 16)
      {
        a = 192 + a*8;
      } else {
        a = map(a,16,255,64,192);
      }
    }
    s = sin8(a);
    SEGMENT.SetPixelColor(
      i, 
      // ColourBlend(RgbcctColor::GetU32Colour(SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE)), SEGCOLOR_U32(1), s)
      ColourBlend(RgbcctColor::GetU32Colour(SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE)), SEGCOLOR_U32(1), s)
    );
  }
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Running
 * @description: Running lights effect with smooth sine transition.
 * @note : Converted from WLED Effects
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Running_Lights()
{
  EffectAnim__Base_Running(false);
}
static const char PM_EFFECT_CONFIG__RUNNING_LIGHTS[] PROGMEM = "!,Wave width;!,!;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Running
 * @description: Running lights effect with sawtooth transition.
 * @note : Converted from WLED Effects
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Saw()
{
  EffectAnim__Base_Running(true);
}
static const char PM_EFFECT_CONFIG__SAW[] PROGMEM = "!,Width;!,!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Running
 * @description: Blink several LEDs in random colors on, reset, repeat.
 *               Inspired by www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/
 * @note : Converted from WLED Effects
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
void mAnimatorLight::EffectAnim__Twinkle()
{
  fill(SEGCOLOR_U32(1));

  uint32_t cycleTime = 20 + (255 - SEGMENT.speed())*5;
  uint32_t it = millis() / cycleTime;
  if (it != SEGMENT.step)
  {
    uint16_t maxOn = map(SEGMENT.intensity(), 0, 255, 1, SEGLEN); // make sure at least one LED is on
    if (SEGMENT.params_internal.aux0 >= maxOn)
    {
      SEGMENT.params_internal.aux0 = 0;
      SEGMENT.params_internal.aux1 = random16(); // new seed for our PRNG
    }
    SEGMENT.params_internal.aux0++;
    SEGMENT.step = it;
  }
  
  uint16_t PRNG16 = SEGMENT.params_internal.aux1;

  for (uint16_t i = 0; i < SEGMENT.params_internal.aux0; i++)
  {
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 13849; // next 'random' number
    uint32_t p = (uint32_t)SEGLEN * (uint32_t)PRNG16;
    uint16_t j = p >> 16;
    SEGMENT.SetPixelColor(j, SEGMENT.GetPaletteColour(j, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
  }

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__TWINKLE[] PROGMEM = "!,!;!,!;!;;m12=0"; //pixels


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Running
 * @description: Dissolve function
 * @note : Converted from WLED Effects
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Base_Dissolve(uint32_t color)
{

//tmp added
uint8_t _brightness = pCONT_iLight->getBriRGB_Global();

// void mAnimatorLight::dissolve(uint32_t color) {
  bool wa = (SEGCOLOR_U32(1) != 0 && _brightness < 255); //workaround, can't compare getPixel to color if not full brightness
  
  for (uint16_t j = 0; j <= SEGLEN / 15; j++)
  {
    if (random8() <= SEGMENT.intensity()) {
      for (uint8_t times = 0; times < 10; times++) //attempt to spawn a new pixel 5 times
      {
        uint16_t i = random16(SEGLEN);
        if (SEGMENT.params_internal.aux0) { //dissolve to primary/palette
          if (RgbcctColor::GetU32Colour(SEGMENT.GetPixelColor(i)) == SEGCOLOR_U32(1) || wa)
          {
            if (color == SEGCOLOR_U32(0))
            {
              SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
            } else { SEGMENT.SetPixelColor(i, color); }     
            break; //only spawn 1 new pixel per frame per 50 LEDs
          }
        } else { //dissolve to secondary
          if (RgbcctColor::GetU32Colour(SEGMENT.GetPixelColor(i)) != SEGCOLOR_U32(1)) { SEGMENT.SetPixelColor(i, SEGCOLOR_U32(1)); break; }
        }
      }
    }
  }

  if (SEGMENT.call > (255 - SEGMENT.speed()) + 15) 
  {
    SEGMENT.params_internal.aux0 = !SEGMENT.params_internal.aux0;
    SEGMENT.call = 0;
  }
  
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Running
 * @description: Blink several LEDs on and then off
 * @note : Converted from WLED Effects
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Dissolve()
{
  EffectAnim__Base_Dissolve(SEGCOLOR_U32(0));
}
static const char PM_EFFECT_CONFIG__DISSOLVE[] PROGMEM = "Repeat speed,Dissolve speed,,,,Random;!,!;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Running
 * @description: Blink several LEDs on and then off in random colors
 * @note : Converted from WLED Effects
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Dissolve_Random()
{
  EffectAnim__Base_Dissolve(color_wheel(random8()));
}
static const char PM_EFFECT_CONFIG__DISSOLVE_RANDOM[] PROGMEM = "Repeat speed,Dissolve speed;,!;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * Android loading circle
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Android()
{
  
  for(uint16_t i = 0; i < SEGLEN; i++) {
    SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
  }

  if (SEGMENT.params_internal.aux1 > ((float)SEGMENT.intensity()/255.0)*(float)SEGLEN)
  {
    SEGMENT.params_internal.aux0 = 1;
  } else
  {
    if (SEGMENT.params_internal.aux1 < 2) SEGMENT.params_internal.aux0 = 0;
  }

  uint16_t a = SEGMENT.step;
  
  if (SEGMENT.params_internal.aux0 == 0)
  {
    if (SEGMENT.call %3 == 1) {a++;}
    else {SEGMENT.params_internal.aux1++;}
  } else
  {
    a++;
    if (SEGMENT.call %3 != 1) SEGMENT.params_internal.aux1--;
  }
  
  if (a >= SEGLEN) a = 0;

  if (a + SEGMENT.params_internal.aux1 < SEGLEN)
  {
    for(int i = a; i < a+SEGMENT.params_internal.aux1; i++) {
      SEGMENT.SetPixelColor(i, SEGCOLOR_U32(0));
    }
  } else
  {
    for(int i = a; i < SEGLEN; i++) {
      SEGMENT.SetPixelColor(i, SEGCOLOR_U32(0));
    }
    for(int i = 0; i < SEGMENT.params_internal.aux1 - (SEGLEN -a); i++) {
      SEGMENT.SetPixelColor(i, SEGCOLOR_U32(0));
    }
  }
  SEGMENT.step = a;
  
  SEGMENT.transition.rate_ms =  3 + ((8 * (uint32_t)(255 - SEGMENT.speed())) / SEGLEN);
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();

}
static const char PM_EFFECT_CONFIG__ANDROID[] PROGMEM = "!,Width;!,!;!;;m12=1"; //vertical


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * Red - Amber - Green - Blue lights running
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__ColourFul()
{

  uint8_t numColors = 4; //3, 4, or 5
  uint32_t cols[9]{0x00FF0000,0x00EEBB00,0x0000EE00,0x000077CC};
  if (SEGMENT.intensity() > 160 || SEGMENT.palette.id) { //palette or color
    // If RGBCCT palette, use the first 3 colors
    if (SEGMENT.palette.id == mPalette::PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_01__ID) {
      numColors = 5;
      for (size_t i = 0; i < 5; i++) cols[i] = SEGCOLOR_U32(i);
    } 
    else
    {
      uint16_t fac = 80;
      if (SEGMENT.palette.id == mPalette::PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__C9__ID) {numColors = 5; fac = 61;} //C9 2 has 5 colors
      for (size_t i = 0; i < numColors; i++) 
      {
        cols[i] = SEGMENT.GetPaletteColour(i*fac, PALETTE_SPAN_OFF, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32();  
      }
    }
  } 
  else 
  if (SEGMENT.intensity() < 80) //pastel (easter) colors
  {
    cols[0] = 0x00FF8040;
    cols[1] = 0x00E5D241;
    cols[2] = 0x0077FF77;
    cols[3] = 0x0077F0F0;
  }
  for (size_t i = numColors; i < numColors*2 -1U; i++) cols[i] = cols[i-numColors];

  uint32_t cycleTime = 50 + (8 * (uint32_t)(255 - SEGMENT.speed()));
  uint32_t it = _now / cycleTime;
  if (it != SEGMENT.step)
  {
    if (SEGMENT.speed() > 0) SEGMENT.params_internal.aux0++;
    if (SEGMENT.params_internal.aux0 >= numColors) SEGMENT.params_internal.aux0 = 0;
    SEGMENT.step = it;
  }

  for (int i = 0; i < SEGLEN; i+= numColors)
  {
    for (int j = 0; j < numColors; j++) SEGMENT.setPixelColor(i + j, cols[SEGMENT.params_internal.aux0 + j]);
  }
  
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__COLORFUL[] PROGMEM = "!,Saturation;1,2,3;!";



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
/*
 * Emulates a traffic light.
 *
// void mAnimatorLight::mode_traffic_light(void) {

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Traffic_Light()
{
  for(uint16_t i=0; i < SEGLEN; i++)
    SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
  uint32_t mdelay = 500;
  for (int i = 0; i < SEGLEN-2 ; i+=3)
  {
    switch (SEGMENT.params_internal.aux0)
    {
      case 0: SEGMENT.SetPixelColor(i, 0x00FF0000); mdelay = 150 + (100 * (uint32_t)(255 - SEGMENT.speed()));break;
      case 1: SEGMENT.SetPixelColor(i, 0x00FF0000); mdelay = 150 + (20 * (uint32_t)(255 - SEGMENT.speed())); SEGMENT.SetPixelColor(i+1, 0x00EECC00); break;
      case 2: SEGMENT.SetPixelColor(i+2, 0x0000FF00); mdelay = 150 + (100 * (uint32_t)(255 - SEGMENT.speed()));break;
      case 3: SEGMENT.SetPixelColor(i+1, 0x00EECC00); mdelay = 150 + (20 * (uint32_t)(255 - SEGMENT.speed()));break;
    }
  }

  if (millis() - SEGMENT.step > mdelay)
  {
    SEGMENT.params_internal.aux0++;
    if (SEGMENT.params_internal.aux0 == 1 && SEGMENT.intensity() > 140) SEGMENT.params_internal.aux0 = 2; //skip Red + Amber, to get US-style sequence
    if (SEGMENT.params_internal.aux0 > 3) SEGMENT.params_internal.aux0 = 0;
    SEGMENT.step = millis();
  }
  
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__TRAFFIC_LIGHT[] PROGMEM = "!,US style;,!;!";

#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE


#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

/*
 * Alternating pixels running function.
 *
// void mAnimatorLight::running(uint32_t color1, uint32_t color2) {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Base_Running(uint32_t color1, uint32_t color2)
{
  uint8_t pxw = 1 + (SEGMENT.intensity() >> 5);
  uint32_t cycleTime = 35 + (255 - SEGMENT.speed());
  uint32_t it = millis() / cycleTime;
  if (SEGMENT.speed() == 0) it = 0;

  for(uint16_t i = 0; i < SEGLEN; i++) {
    if((i + SEGMENT.params_internal.aux0) % (pxw*2) < pxw) {
      if (color1 == SEGCOLOR_U32(0))
      {
        SEGMENT.SetPixelColor(SEGLEN -i -1, SEGMENT.GetPaletteColour(SEGLEN -i -1, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE) );
      } else
      {
        SEGMENT.SetPixelColor(SEGLEN -i -1, color1);
      }
    } else {
      SEGMENT.SetPixelColor(SEGLEN -i -1, color2);
    }
  }

  if (it != SEGMENT.step )
  {
    SEGMENT.params_internal.aux0 = (SEGMENT.params_internal.aux0 +1) % (pxw*2);
    SEGMENT.step = it;
  }
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}

/*
 * Alternating color/sec pixels running.
 */
void mAnimatorLight::EffectAnim__Running_Colour(void) {
  return EffectAnim__Base_Running(SEGCOLOR_U32(0), SEGCOLOR_U32(1));
}
static const char PM_EFFECT_CONFIG__RUNNING_COLOR[] PROGMEM = "!,Width;!,!;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
/*
 * Random colored pixels running.
 *
// void mAnimatorLight::mode_running_random(void) {

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Running_Random()
{

  uint32_t cycleTime = 25 + (3 * (uint32_t)(255 - SEGMENT.speed()));
  uint32_t it = millis() / cycleTime;
  if (SEGMENT.params_internal.aux1 == it){return;}// return FRAMETIME_MS;

  for(uint16_t i=SEGLEN-1; i > 0; i--) {
    SEGMENT.SetPixelColor( i, SEGMENT.GetPixelColor( i - 1));
  }

  if(SEGMENT.step == 0) {
    SEGMENT.params_internal.aux0 = get_random_wheel_index(SEGMENT.params_internal.aux0);
    SEGMENT.SetPixelColor(0, color_wheel(SEGMENT.params_internal.aux0));
  }

  SEGMENT.step++;
  if (SEGMENT.step > ((255-SEGMENT.intensity()) >> 4))
  {
    SEGMENT.step = 0;
  }

  SEGMENT.params_internal.aux1 = it;
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__RUNNING_RANDOM[] PROGMEM = "!,Zone size;;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE




#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
/*
 * Gradient run base function
 *
// void mAnimatorLight::gradient_base(bool loading) {

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Base_Gradient(bool loading)
{

  uint16_t counter = millis() * ((SEGMENT.speed() >> 2) + 1);
  uint16_t pp = counter * SEGLEN >> 16;
  if (SEGMENT.call == 0) pp = 0;
  float val; //0.0 = sec 1.0 = pri
  float brd = loading ? SEGMENT.intensity() : SEGMENT.intensity()/2;
  if (brd <1.0) brd = 1.0;
  int p1 = pp-SEGLEN;
  int p2 = pp+SEGLEN;

  for(uint16_t i = 0; i < SEGLEN; i++)
  {
    if (loading)
    {
      val = abs(((i>pp) ? p2:pp) -i);
    } else {
      val = MIN(abs(pp-i),MIN(abs(p1-i),abs(p2-i)));
    }
    val = (brd > val) ? val/brd * 255 : 255;
    SEGMENT.SetPixelColor(
      i, 
      ColourBlend(SEGCOLOR_U32(0), SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32(), val)
    );
  }

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}


/*
 * Gradient run
 */
void mAnimatorLight::EffectAnim__Gradient(void) {
  return EffectAnim__Base_Gradient(false);
}
static const char PM_EFFECT_CONFIG__GRADIENT[] PROGMEM = "!,Spread;!,!;!;;ix=16";



/*
 * Gradient run with hard transition
 */
void mAnimatorLight::EffectAnim__Loading(void) {
  return EffectAnim__Base_Gradient(true);
}
static const char PM_EFFECT_CONFIG__LOADING[] PROGMEM = "!,Fade;!,!;!;;ix=16";



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * //American Police Light with all LEDs Red and Blue 

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Base_Police(uint32_t color1, uint32_t color2, bool all)
{

  uint16_t counter = millis() * ((SEGMENT.speed() >> 2) +1);
  uint16_t idexR = (counter * SEGLEN) >> 16;
  if (idexR >= SEGLEN) idexR = 0;

  uint16_t topindex = SEGLEN >> 1;
  uint16_t idexB = (idexR > topindex) ? idexR - topindex : idexR + topindex;
  if (SEGMENT.call == 0) SEGMENT.params_internal.aux0 = idexR;
  if (idexB >= SEGLEN) idexB = 0; //otherwise overflow on odd number of LEDs

  if (all) { //different algo, ensuring immediate fill
    if (idexB > idexR) {
      fill(color2);
      for (uint16_t i = idexR; i < idexB; i++) SEGMENT.SetPixelColor(i, color1);
    } else {
      fill(color1);
      for (uint16_t i = idexB; i < idexR; i++) SEGMENT.SetPixelColor(i, color2);
    } 
  } else { //regular dot-only mode
    uint8_t size = 1 + SEGMENT.intensity() >> 3;
    if (size > SEGLEN/2) size = 1+ SEGLEN/2;
    for (uint8_t i=0; i <= size; i++) {
      SEGMENT.SetPixelColor(idexR+i, color1);
      SEGMENT.SetPixelColor(idexB+i, color2);
    }
    if (SEGMENT.params_internal.aux0 != idexR) {
      uint8_t gap = (SEGMENT.params_internal.aux0 < idexR)? idexR - SEGMENT.params_internal.aux0:SEGLEN - SEGMENT.params_internal.aux0 + idexR;
      for (uint8_t i = 0; i <= gap ; i++) {
        if ((idexR - i) < 0) idexR = SEGLEN-1 + i;
        if ((idexB - i) < 0) idexB = SEGLEN-1 + i;
        SEGMENT.SetPixelColor(idexR-i, color1);
        SEGMENT.SetPixelColor(idexB-i, color2);
      }
      SEGMENT.params_internal.aux0 = idexR;
    }
  }
  
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}


//American Police Light with all LEDs Red and Blue 
void mAnimatorLight::EffectAnim__Polce_All()
{  
  return EffectAnim__Base_Police(RED, BLUE, true);
}
static const char PM_EFFECT_CONFIG__POLICE_ALL[] PROGMEM = "!,Width;,Bg;0";


//Police Lights Red and Blue 
void mAnimatorLight::EffectAnim__Police()
{
  fill(SEGCOLOR_U32(1));
  return EffectAnim__Base_Police(RED, BLUE, false);
}
static const char PM_EFFECT_CONFIG__POLICE[] PROGMEM = "!,Width;,Bg;0";


//Police All with custom colors
void mAnimatorLight::EffectAnim__Two_Areas()
{  
  return EffectAnim__Base_Police(SEGCOLOR_U32(0), SEGCOLOR_U32(1), true);
}
static const char PM_EFFECT_CONFIG__TWO_AREAS[] PROGMEM = "!,Dot size,,,,,,,Overlay;1,2,Bg;!";


//Police Lights with custom colors 
void mAnimatorLight::EffectAnim__Two_Dots()
{
  fill(SEGCOLOR_U32(2));
  uint32_t color2 = (SEGCOLOR_U32(1) == SEGCOLOR_U32(2)) ? SEGCOLOR_U32(0) : SEGCOLOR_U32(1);

  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
  return EffectAnim__Base_Police(SEGCOLOR_U32(0), color2, false);
}
static const char PM_EFFECT_CONFIG__TWO_DOTS[] PROGMEM = "!,Dot size,,,,,,,Overlay;1,2,Bg;!";

#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
/*
 * Custom mode by Aircoookie. Color Wipe, but with 3 colors
 *
// void mAnimatorLight::mode_tricolor_wipe(void)
// {

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__TriColour()
{
  uint32_t cycleTime = 1000 + (255 - SEGMENT.speed())*200;
  uint32_t perc = millis() % cycleTime;
  uint16_t prog = (perc * 65535) / cycleTime;
  uint16_t ledIndex = (prog * SEGLEN * 3) >> 16;
  uint16_t ledOffset = ledIndex;

  for (uint16_t i = 0; i < SEGLEN; i++)
  {
    SEGMENT.SetPixelColor(i, 
      // mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette.id, i, nullptr, true, PALETTE_SOLID_WRAP, 2)
      SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE)
    );
  }
  
  if(ledIndex < SEGLEN) { //wipe from 0 to 1
    for (uint16_t i = 0; i < SEGLEN; i++)
    {
      SEGMENT.SetPixelColor(i, (i > ledOffset)? SEGCOLOR_U32(0) : SEGCOLOR_U32(1));
    }
  } else if (ledIndex < SEGLEN*2) { //wipe from 1 to 2
    ledOffset = ledIndex - SEGLEN;
    for (uint16_t i = ledOffset +1; i < SEGLEN; i++)
    {
      SEGMENT.SetPixelColor(i, SEGCOLOR_U32(1));
    }
  } else //wipe from 2 to 0
  {
    ledOffset = ledIndex - SEGLEN*2;
    for (uint16_t i = 0; i <= ledOffset; i++)
    {
      SEGMENT.SetPixelColor(i, SEGCOLOR_U32(0));
    }
  }

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__TRICOLOR_WIPE[] PROGMEM = "!;1,2,3;!";



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
/*
 * Fades between 3 colors
 * Custom mode by Keith Lord: https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/TriFade.h
 * Modified by Aircoookie
 *
// void mAnimatorLight::mode_tricolor_fade(void)
// {

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Fade_TriColour()
{
  uint16_t counter = millis() * ((SEGMENT.speed() >> 3) +1);
  uint32_t prog = (counter * 768) >> 16;

  uint32_t color1 = 0, color2 = 0;
  byte stage = 0;

  if(prog < 256) {
    color1 = SEGCOLOR_U32(0);
    color2 = SEGCOLOR_U32(1);
    stage = 0;
  } else if(prog < 512) {
    color1 = SEGCOLOR_U32(1);
    color2 = SEGCOLOR_U32(2);
    stage = 1;
  } else {
    color1 = SEGCOLOR_U32(2);
    color2 = SEGCOLOR_U32(0);
    stage = 2;
  }

  byte stp = prog; // % 256
  uint32_t color = 0;
  for(uint16_t i = 0; i < SEGLEN; i++) {
    if (stage == 2) {
      color = ColourBlend(
        // RgbcctColor::GetU32Colour(mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette.id, i, nullptr, true, PALETTE_SOLID_WRAP, 2)),
        SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32(),        
        color2, stp);
    } else if (stage == 1) {
      color = ColourBlend(color1, 
        // RgbcctColor::GetU32Colour(mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette.id, i, nullptr, true, PALETTE_SOLID_WRAP, 2)),
        SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32(),        
        stp);
    } else {
      color = ColourBlend(color1, color2, stp);
    }
    SEGMENT.SetPixelColor(i, color);
  }

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__TRICOLOR_FADE[] PROGMEM = "!;1,2,3;!";



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
/*
 * Creates random comets
 * Custom mode by Keith Lord: https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/MultiComet.h
 *
// void mAnimatorLight::mode_multi_comet(void)
// {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Multi_Comet()
{

  uint32_t cycleTime = 10 + (uint32_t)(255 - SEGMENT.speed());
  uint32_t it = millis() / cycleTime;
  if (SEGMENT.step == it){
    SEGMENT.transition.rate_ms = FRAMETIME_MS;
    return;
  }
  if (!SEGMENT.allocateData(sizeof(uint16_t) * 8)){return;}
  
  fade_out(SEGMENT.intensity(), SET_BRIGHTNESS);
  
  uint16_t* comets = reinterpret_cast<uint16_t*>(SEGMENT.data);

  for(uint8_t i=0; i < 8; i++) {
    if(comets[i] < SEGLEN) {
      uint16_t index = comets[i];
      if (SEGCOLOR_U32(2) != 0)
      {
        SEGMENT.SetPixelColor(index, i % 2 ? SEGMENT.GetPaletteColour(index, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE) : SEGCOLOR_RGBCCT(2) );
      } else
      {
        SEGMENT.SetPixelColor(index, SEGMENT.GetPaletteColour(index, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE) );
      }
      comets[i]++;
    } else {
      if(!random(SEGLEN)) {
        comets[i] = 0;
      }
    }
  }

  SEGMENT.step = it;
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__MULTI_COMET[] PROGMEM = "";




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

/*
/  Oscillating bars of color, updated with standard framerate
*
// void mAnimatorLight::mode_oscillate(void)
// {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

typedef struct Oscillator {
  int16_t pos;
  int8_t  size;
  int8_t  dir;
  int8_t  speed;
} oscillator;

void mAnimatorLight::EffectAnim__Oscillate()
{

  uint8_t numOscillators = 3;
  uint16_t dataSize = sizeof(oscillator) * numOscillators;

  if (!SEGMENT.allocateData(dataSize)){    
    ALOG_ERR( PM_JSON_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID;
    return;
  }
  
  Oscillator* oscillators = reinterpret_cast<Oscillator*>(SEGMENT.data);

  if (SEGMENT.call == 0)
  {
    // oscillators[0] = {(uint16_t)SEGLEN/4,   SEGLEN/8,  1, 1};
    oscillators[0].pos = SEGLEN/4;
    oscillators[0].size = SEGLEN/8;
    oscillators[0].dir = 1;
    oscillators[0].speed = 1;
    // oscillators[1] = {SEGLEN/4*3, SEGLEN/8,  1, 2};
    oscillators[0].pos = SEGLEN/4*3;
    oscillators[0].size = SEGLEN/8;
    oscillators[0].dir = 1;
    oscillators[0].speed = 2;
    // oscillators[2] = {SEGLEN/4*2, SEGLEN/8, -1, 1};
    oscillators[0].pos = SEGLEN/4*2;
    oscillators[0].size = SEGLEN/8;
    oscillators[0].dir = -1;
    oscillators[0].speed = 1;

    
  }

  uint32_t cycleTime = 20 + (2 * (uint32_t)(255 - SEGMENT.speed()));
  uint32_t it = millis() / cycleTime;

  for(uint8_t i = 0; i < numOscillators; i++) {
    // if the counter has increased, move the oscillator by the random step
    if (it != SEGMENT.step) oscillators[i].pos += oscillators[i].dir * oscillators[i].speed;
    oscillators[i].size = SEGLEN/(3+SEGMENT.intensity()/8);
    if((oscillators[i].dir == -1) && (oscillators[i].pos <= 0)) {
      oscillators[i].pos = 0;
      oscillators[i].dir = 1;
      // make bigger steps for faster speeds
      oscillators[i].speed = SEGMENT.speed() > 100 ? random8(2, 4):random8(1, 3);
    }
    if((oscillators[i].dir == 1) && (oscillators[i].pos >= (SEGLEN - 1))) {
      oscillators[i].pos = SEGLEN - 1;
      oscillators[i].dir = -1;
      oscillators[i].speed = SEGMENT.speed() > 100 ? random8(2, 4):random8(1, 3);
    }
  }

  for(uint16_t i=0; i < SEGLEN; i++) {
    uint32_t color = BLACK;
    for(uint8_t j=0; j < numOscillators; j++) {
      if(i >= oscillators[j].pos - oscillators[j].size && i <= oscillators[j].pos + oscillators[j].size) {
        color = (color == BLACK) ? SEGCOLOR_U32(j) : ColourBlend(color, SEGCOLOR_U32(j), 128);
      }
    }
    SEGMENT.SetPixelColor(i, color);
  }
 
  SEGMENT.step = it;

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__OSCILLATE[] PROGMEM = "";




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
// Pride2015
// Animated, ever-changing rainbows.
// by Mark Kriegsman: https://gist.github.com/kriegsman/964de772d64c502760e5
// void mAnimatorLight::mode_pride_2015(void)
// {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Pride_2015()
{
  uint16_t duration = 10 + SEGMENT.speed();
  uint16_t sPseudotime = SEGMENT.step;
  uint16_t sHue16 = SEGMENT.params_internal.aux0;

  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  sPseudotime += duration * msmultiplier;
  sHue16 += duration * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;
  CRGB fastled_col;

  for (uint16_t i = 0 ; i < SEGLEN; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 >> 8;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV( hue8, sat8, bri8);
    fastled_col = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT.GetPixelColor(i)));

    nblend(fastled_col, newcolor, 64);
    SEGMENT.SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }
  SEGMENT.step = sPseudotime;
  SEGMENT.params_internal.aux0 = sHue16;
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__PRIDE_2015[] PROGMEM = "!;;";




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
//eight colored dots, weaving in and out of sync with each other
// void mAnimatorLight::mode_juggle(void){

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Juggle()
{

  // ALOG_INF(PSTR("EffectAnim__Juggle %d %dms"), SEGMENT.call, SEGMENT.transition.rate_ms);

  if (SEGMENT.call == 0) {
    SEGMENT.setUpLeds();  // lossless getPixelColor()
    SEGMENT.fill(BLACK);
  }

  SEGMENT.fadeToBlackBy(192 - (3*SEGMENT.intensity()/4));

  CRGB fastled_col;
  byte dothue = 0;
  for (int i = 0; i < 8; i++) {
    uint16_t index = 0 + beatsin88((16 + SEGMENT.speed())*(i + 7), 0, SEGLEN -1);
    fastled_col = CRGB(SEGMENT.getPixelColor(index));
    fastled_col |= (SEGMENT.palette.id==0)?CHSV(dothue, 220, 255):ColorFromPalette(SEGMENT.palette_container->CRGB16Palette16_Palette.data, dothue, 255);
    SEGMENT.setPixelColor(index, fastled_col);
    dothue += 32;
  }

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__JUGGLES[] PROGMEM = "!,Trail;;!;;sx=64,ix=128"; // Pixels, Beatsin



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

// void mAnimatorLight::mode_palette()
// {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Palette()
{
  uint16_t counter = 0;
  if (SEGMENT.speed() != 0) 
  {
    counter = (millis() * ((SEGMENT.speed() >> 3) +1)) & 0xFFFF;
    counter = counter >> 8;
  }
  
  bool noWrap = (paletteBlend == 2 || (paletteBlend == 0 && SEGMENT.speed() == 0));
  for (uint16_t i = 0; i < SEGLEN; i++)
  {
    uint8_t colorIndex = (i * 255 / SEGLEN) - counter;
    
    if (noWrap) colorIndex = map(colorIndex, 0, 255, 0, 240); //cut off blend at palette "end"
    
    SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(colorIndex, PALETTE_SPAN_OFF, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));   

  }
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__PALETTE[] PROGMEM = "Cycle speed;;!;;c3=0,o2=0";


// // WLED limitation: Analog Clock overlay will NOT work when Fire2012 is active
// // Fire2012 by Mark Kriegsman, July 2012
// // as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
// //// 
// // This basic one-dimensional 'fire' simulation works roughly as follows:
// // There's a underlying array of 'heat' cells, that model the temperature
// // at each point along the line.  Every cycle through the simulation, 
// // four steps are performed:
// //  1) All cells cool down a little bit, losing heat to the air
// //  2) The heat from each cell drifts 'up' and diffuses a little
// //  3) Sometimes randomly new 'sparks' of heat are added at the bottom
// //  4) The heat from each cell is rendered as a color into the leds array
// //     The heat-to-color mapping uses a black-body radiation approximation.
// //
// // Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
// //
// // This simulation scales it self a bit depending on NUM_LEDS; it should look
// // "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
// //
// // I recommend running this simulation at anywhere from 30-100 frames per second,
// // meaning an interframe delay of about 10-35 milliseconds.
// //
// // Looks best on a high-density LED setup (60+ pixels/meter).
// //
// //
// // There are two main parameters you can play with to control the look and
// // feel of your fire: COOLING (used in step 1 above) (Speed = COOLING), and SPARKING (used
// // in step 3 above) (Effect intensity() = Sparking).



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
// ColorWavesWithPalettes by Mark Kriegsman: https://gist.github.com/kriegsman/8281905786e8b2632aeb
// This function draws color waves with an ever-changing,
// widely-varying set of parameters, using a color palette.
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__ColourWaves()
{
  uint16_t duration = 10 + SEGMENT.speed();
  uint16_t sPseudotime = SEGMENT.step;
  uint16_t sHue16 = SEGMENT.params_internal.aux0;

  uint8_t brightdepth = beatsin88(341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  // uint16_t hueinc16 = beatsin88(113, 300, 1500);
  uint16_t hueinc16 = beatsin88(113, 60, 300)*SEGMENT.intensity()*10/255;  // Use the intensity() Slider for the hues

  sPseudotime += duration * msmultiplier;
  sHue16 += duration * beatsin88(400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;
  CRGB fastled_col;

  for ( uint16_t i = 0 ; i < SEGLEN; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 >> 8;
    uint16_t h16_128 = hue16 >> 7;
    if ( h16_128 & 0x100) {
      hue8 = 255 - (h16_128 >> 1);
    } else {
      hue8 = h16_128 >> 1;
    }

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = ColorFromPalette(SEGMENT.palette_container->CRGB16Palette16_Palette.data, hue8, bri8);
    fastled_col = RgbcctColor::GetU32Colour(SEGMENT.GetPixelColor(i));

    nblend(fastled_col, newcolor, 128);
    SEGMENT.SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }
  SEGMENT.step = sPseudotime;
  SEGMENT.params_internal.aux0 = sHue16;
  
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__COLORWAVES[] PROGMEM = "!,Hue;!;!";



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * colored stripes pulsing at a defined Beats-Per-Minute (BPM)
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__BPM()
{

  CRGB fastled_col;
  uint32_t stp = (millis() / 20) & 0xFF;
  uint8_t beat = beatsin8(SEGMENT.speed(), 64, 255);
  for (uint16_t i = 0; i < SEGLEN; i++) {
    fastled_col = ColorFromPalette(SEGMENT.palette_container->CRGB16Palette16_Palette.data, stp + (i * 2), beat - stp + (i * 10));
    SEGMENT.SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__BPM[] PROGMEM = "!;!;!;;sx=64";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * based on https://gist.github.com/kriegsman/5408ecd397744ba0393e
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Twinkle_Colour()
{
  uint16_t dataSize = (SEGLEN+7) >> 3; //1 bit per LED
  if (!SEGMENT.allocateData(dataSize)){    
    ALOG_ERR( PM_JSON_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID;
    return;
  }
  
  CRGB fastled_col, prev;
  fract8 fadeUpAmount = 8 + (SEGMENT.speed()/4), fadeDownAmount = 5 + (SEGMENT.speed()/7);
  for (uint16_t i = 0; i < SEGLEN; i++) {
    fastled_col = RgbcctColor::GetU32Colour(SEGMENT.GetPixelColor(i));
    prev = fastled_col;
    uint16_t index = i >> 3;
    uint8_t  bitNum = i & 0x07;
    bool fadeUp = bitRead(SEGMENT.data[index], bitNum);
    
    if (fadeUp) {
      CRGB incrementalColor = fastled_col;
      incrementalColor.nscale8_video( fadeUpAmount);
      fastled_col += incrementalColor;

      if (fastled_col.red == 255 || fastled_col.green == 255 || fastled_col.blue == 255) {
        bitWrite(SEGMENT.data[index], bitNum, false);
      }
      SEGMENT.SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);

      // if (RgbcctColor::GetU32Colour(SEGMENT.GetPixelColor(i)) == prev) //fix "stuck" pixels
      // {
      //   fastled_col += fastled_col;
      //   SEGMENT.SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
      // }
    } else {
      fastled_col.nscale8( 255 - fadeDownAmount);
      SEGMENT.SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
    }
  }

  for (uint16_t j = 0; j <= SEGLEN / 50; j++)
  {
    if (random8() <= SEGMENT.intensity()) {
      for (uint8_t times = 0; times < 5; times++) //attempt to spawn a new pixel 5 times
      {
        int i = random16(SEGLEN);
        if(RgbcctColor::GetU32Colour(SEGMENT.GetPixelColor(i)) == 0) {
          fastled_col = ColorFromPalette(SEGMENT.palette_container->CRGB16Palette16_Palette.data, random8(), 64, NOBLEND);
          uint16_t index = i >> 3;
          uint8_t  bitNum = i & 0x07;
          bitWrite(SEGMENT.data[index], bitNum, true);
          SEGMENT.SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
          break; //only spawn 1 new pixel per frame per 50 LEDs
        }
      }
    }
  }
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__COLORTWINKLE[] PROGMEM = "Fade speed,Spawn speed;;!;;m12=0"; //pixels



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

//Calm effect, like a lake at night
// void mAnimatorLight::mode_lake() {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Lake()
{
  uint8_t sp = SEGMENT.speed()/10;
  int wave1 = beatsin8(sp +2, -64,64);
  int wave2 = beatsin8(sp +1, -64,64);
  uint8_t wave3 = beatsin8(sp +2,   0,80);
  CRGB fastled_col;

  for (uint16_t i = 0; i < SEGLEN; i++)
  {
    int index = cos8((i*15)+ wave1)/2 + cubicwave8((i*23)+ wave2)/2;           
    uint8_t lum = (index > wave3) ? index - wave3 : 0;
    fastled_col = ColorFromPalette(SEGMENT.palette_container->CRGB16Palette16_Palette.data, map(index,0,255,0,240), lum, LINEARBLEND);
    SEGMENT.SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__LAKE[] PROGMEM = "!;Fx;!";



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * meteor effect
// send a meteor from begining to to the end of the strip with a trail that randomly decays.
// adapted from https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#LEDStripEffectMeteorRain
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Meteor()
{
  if (!SEGMENT.allocateData(SEGLEN)){    
    ALOG_ERR( PM_JSON_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID;
    return;
  }

  byte* trail = SEGMENT.data;
  
  byte meteorSize= 1+ SEGLEN / 10;
  uint16_t counter = millis() * ((SEGMENT.speed() >> 2) +8);
  uint16_t in = counter * SEGLEN >> 16;

  // fade all leds to colors[1] in LEDs one step
  for (uint16_t i = 0; i < SEGLEN; i++) {
    if (random8() <= 255 - SEGMENT.intensity())
    {
      byte meteorTrailDecay = 128 + random8(127);
      trail[i] = scale8(trail[i], meteorTrailDecay);
      SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).WithBrightness(trail[i]) );
    }
  }

  // draw meteor
  for(int j = 0; j < meteorSize; j++) {
    uint16_t index = in + j;
    if(index >= SEGLEN) {
      index = (in + j - SEGLEN);
    }
    trail[index] = 240;
    SEGMENT.SetPixelColor(index, SEGMENT.GetPaletteColour(index, PALETTE_SPAN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE) );
  }

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__METEOR[] PROGMEM = "!,Trail length;!;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * send a meteor from begining to to the end of the strip with a trail that randomly decays.
// adapted from https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#LEDStripEffectMeteorRain
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Metoer_Smooth()
{
  if (!SEGMENT.allocateData(SEGLEN)){return;}// return mode_static(); //allocation failed

  byte* trail = SEGMENT.data;
  
  byte meteorSize= 1+ SEGLEN / 10;
  uint16_t in = map((SEGMENT.step >> 6 & 0xFF), 0, 255, 0, SEGLEN -1);

  // fade all leds to colors[1] in LEDs one step
  for (uint16_t i = 0; i < SEGLEN; i++) {
    if (trail[i] != 0 && random8() <= 255 - SEGMENT.intensity())
    {
      int change = 3 - random8(12); //change each time between -8 and +3
      trail[i] += change;
      if (trail[i] > 245) trail[i] = 0;
      if (trail[i] > 240) trail[i] = 240;
      SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).WithBrightness(trail[i]) );
    }
  }
  
  // draw meteor
  for(int j = 0; j < meteorSize; j++) {  
    uint16_t index = in + j;   
    if(in + j >= SEGLEN) {
      index = (in + j - SEGLEN);
    }
    trail[index] = 240;
    SEGMENT.SetPixelColor(index, SEGMENT.GetPaletteColour(index, PALETTE_SPAN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE) );
  }

  SEGMENT.step += SEGMENT.speed() +1;
  
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__METEOR_SMOOTH[] PROGMEM = "!,Trail length;!;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * //  TwinkleFOX by Mark Kriegsman: https://gist.github.com/kriegsman/756ea6dcae8e30845b5a
//
//  TwinkleFOX: Twinkling 'holiday' lights that fade in and out.
//  Colors are chosen from a palette. Read more about this effect using the link above!
// If COOL_LIKE_INCANDESCENT is set to 1, colors will
// fade out slighted 'reddened', similar to how
// incandescent bulbs change color as they get dim down.
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#define COOL_LIKE_INCANDESCENT 1

CRGB mAnimatorLight::EffectAnim__Base_Twinkle_Fox_One_Twinkle(uint32_t ms, uint8_t salt, bool cat)
{
  // Overall twinkle speed() (changed)
  uint16_t ticks = ms / SEGMENT.params_internal.aux0;
  uint8_t fastcycle8 = ticks;
  uint16_t slowcycle16 = (ticks >> 8) + salt;
  slowcycle16 += sin8(slowcycle16);
  slowcycle16 = (slowcycle16 * 2053) + 1384;
  uint8_t slowcycle8 = (slowcycle16 & 0xFF) + (slowcycle16 >> 8);
  
  // Overall twinkle density.
  // 0 (NONE lit) to 8 (ALL lit at once).
  // Default is 5.
  uint8_t twinkleDensity = (SEGMENT.intensity() >> 5) +1;

  uint8_t bright = 0;
  if (((slowcycle8 & 0x0E)/2) < twinkleDensity) {
    uint8_t ph = fastcycle8;
    // This is like 'triwave8', which produces a
    // symmetrical up-and-down triangle sawtooth waveform, except that this
    // function produces a triangle wave with a faster attack and a slower decay
    if (cat) //twinklecat, variant where the leds instantly turn on
    {
      bright = 255 - ph;
    } else { //vanilla twinklefox
      if (ph < 86) {
      bright = ph * 3;
      } else {
        ph -= 86;
        bright = 255 - (ph + (ph/2));
      }
    }
  }

  uint8_t hue = slowcycle8 - salt;
  CRGB c;
  if (bright > 0) {
    c = ColorFromPalette(SEGMENT.palette_container->CRGB16Palette16_Palette.data, hue, bright, NOBLEND);
    if(COOL_LIKE_INCANDESCENT == 1) {
      // This code takes a pixel, and if its in the 'fading down'
      // part of the cycle, it adjusts the color a little bit like the
      // way that incandescent bulbs fade toward 'red' as they dim.
      if (fastcycle8 >= 128) 
      {
        uint8_t cooling = (fastcycle8 - 128) >> 4;
        c.g = qsub8(c.g, cooling);
        c.b = qsub8(c.b, cooling * 2);
      }
    }
  } else {
    c = HTMLColorCode::Black;
  }
  return c;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
  
}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
//  This function loops over each pixel, calculates the
//  adjusted 'clock' that this pixel should use, and calls
//  "CalculateOneTwinkle" on each pixel.  It then displays
//  either the twinkle color of the background color,
//  whichever is brighter.
// void mAnimatorLight::twinklefox_base(bool cat)
// {

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Base_Twinkle_Fox(bool cat)
{
  // "PRNG16" is the pseudorandom number generator
  // It MUST be reset to the same starting value each time
  // this function is called, so that the sequence of 'random'
  // numbers that it generates is (paradoxically) stable.
  uint16_t PRNG16 = 11337;

  // Calculate speed()
  if (SEGMENT.speed() > 100) SEGMENT.params_internal.aux0 = 3 + ((255 - SEGMENT.speed()) >> 3);
  else SEGMENT.params_internal.aux0 = 22 + ((100 - SEGMENT.speed()) >> 1);

  // Set up the background color, "bg".
  CRGB bg;
  bg = col_to_crgb(SEGCOLOR_U32(1));
  uint8_t bglight = bg.getAverageLight();
  if (bglight > 64) {
    bg.nscale8_video(16); // very bright, so scale to 1/16th
  } else if (bglight > 16) {
    bg.nscale8_video(64); // not that bright, so scale to 1/4th
  } else {
    bg.nscale8_video(86); // dim, scale to 1/3rd.
  }

  uint8_t backgroundBrightness = bg.getAverageLight();

  for (uint16_t i = 0; i < SEGLEN; i++) {
  
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
    uint16_t myclockoffset16= PRNG16; // use that number as clock offset
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
    // use that number as clock speed() adjustment factor (in 8ths, from 8/8ths to 23/8ths)
    uint8_t myspeedmultiplierQ5_3 =  ((((PRNG16 & 0xFF)>>4) + (PRNG16 & 0x0F)) & 0x0F) + 0x08;
    uint32_t myclock30 = (uint32_t)((millis() * myspeedmultiplierQ5_3) >> 3) + myclockoffset16;
    uint8_t  myunique8 = PRNG16 >> 8; // get 'salt' value for this pixel

    // We now have the adjusted 'clock' for this pixel, now we call
    // the function that computes what color the pixel should be based
    // on the "brightness = f( time )" idea.
    CRGB c = EffectAnim__Base_Twinkle_Fox_One_Twinkle(myclock30, myunique8, cat);

    uint8_t cbright = c.getAverageLight();
    int16_t deltabright = cbright - backgroundBrightness;
    if (deltabright >= 32 || (!bg)) {
      // If the new pixel is significantly brighter than the background color,
      // use the new color.
      SEGMENT.SetPixelColor(i, c.red, c.green, c.blue);
    } else if (deltabright > 0) {
      // If the new pixel is just slightly brighter than the background color,
      // mix a blend of the new color and the background color
      SEGMENT.SetPixelColor(i, ColourBlend(crgb_to_col(bg), crgb_to_col(c), deltabright * 8));
    } else {
      // if the new pixel is not at all brighter than the background color,
      // just use the background color.
      SEGMENT.SetPixelColor(i, bg.r, bg.g, bg.b);
    }
  }
  
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}

void mAnimatorLight::EffectAnim__Twinkle_Fox()
{
  return EffectAnim__Base_Twinkle_Fox(false);
}
static const char PM_EFFECT_CONFIG__TWINKLEFOX[] PROGMEM = "!,Twinkle rate;;!";

void mAnimatorLight::EffectAnim__Twinkle_Cat()
{
  return EffectAnim__Base_Twinkle_Fox(true);
}
static const char PM_EFFECT_CONFIG__TWINKLECAT[] PROGMEM = "!,Twinkle rate;;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Base_Spots(uint16_t threshold)
{

  fill(SEGCOLOR_U32(1));
  
  uint16_t maxZones = SEGLEN >> 2;
  uint16_t zones = 1 + ((SEGMENT.intensity() * maxZones) >> 8);
  uint16_t zoneLen = SEGLEN / zones;
  uint16_t offset = (SEGLEN - zones * zoneLen) >> 1;

  for (uint16_t z = 0; z < zones; z++)
  {
    uint16_t pos = offset + z * zoneLen;
    for (uint16_t i = 0; i < zoneLen; i++)
    {
      uint16_t wave = triwave16((i * 0xFFFF) / zoneLen);
      if (wave > threshold) {
        uint16_t index = 0 + pos + i;
        uint8_t s = (wave - threshold)*255 / (0xFFFF - threshold);
        SEGMENT.SetPixelColor(index, ColourBlend( SEGMENT.GetPaletteColour(index, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE), SEGCOLOR_RGBCCT(1), 255-s) );
      }
    }
  }
  
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}


//intensity() slider sets number of "lights", speed() sets LEDs per light
void mAnimatorLight::EffectAnim__Spots()
{
  EffectAnim__Base_Spots((255 - SEGMENT.speed()) << 8);
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__SPOTS[] PROGMEM = "Spread,Width,,,,,,,Overlay;!,!;!";


//intensity() slider sets number of "lights", LEDs per light fade in and out
void mAnimatorLight::EffectAnim__Fade_Spots()
{
  uint16_t counter = millis() * ((SEGMENT.speed() >> 2) +8);
  uint16_t t = triwave16(counter);
  uint16_t tr = (t >> 1) + (t >> 2);
//   return spots_base(tr);
  EffectAnim__Base_Spots(tr);
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__SPOTS_FADE[] PROGMEM = "Spread,Width,,,,Time,Rate,,Overlay;!,!;!"; // 7 sliders + 4 options before first ;


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::triwave16(uint16_t in)
{
  if (in < 0x8000) return in *2;
  return 0xFFFF - (in - 0x8000)*2;
}

uint16_t mAnimatorLight::mode_palette()
{
  uint16_t counter = 0;
  if (SEGMENT.speed() != 0) 
  {
    counter = (millis() * ((SEGMENT.speed() >> 3) +1)) & 0xFFFF;
    counter = counter >> 8;
  }
  
  bool noWrap = (paletteBlend == 2 || (paletteBlend == 0 && SEGMENT.speed() == 0));
  RgbcctColor colour;
  for (uint16_t i = 0; i < SEGLEN; i++)
  {
    uint8_t colorIndex = (i * 255 / SEGLEN) - counter;
    
    colour = SEGMENT.GetPaletteColour(colorIndex, PALETTE_SPAN_OFF, PALETTE_WRAP_ON);
    colour = ApplyBrightnesstoRgbcctColour( colour, SEGMENT.getBrightnessRGB_WithGlobalApplied(), SEGMENT.getBrightnessCCT_WithGlobalApplied() );
    
    SEGMENT.SetPixelColor(i, colour);
  }
  return FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}

void mAnimatorLight::EffectAnim__Glitter()
{

  mode_palette();

  if (SEGMENT.intensity() > random8())
  {
    SEGMENT.SetPixelColor(random16(SEGLEN), ULTRAWHITE);
  }
  
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__GLITTER[] PROGMEM = "!,!,,,,,,,Overlay;1,2,Glitter color;!;;pal=0,m12=0"; //pixels


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
/*
*  POPCORN
*  modified from https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/Popcorn.h
*
// void mAnimatorLight::mode_popcorn(void) {#
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Popcorn()
{

  //allocate segment data
  uint16_t maxNumPopcorn = 24; 
  uint16_t dataSize = sizeof(spark) * maxNumPopcorn;
  if (!SEGMENT.allocateData(dataSize))
  {
    return;
  }// return mode_static(); //allocation failed
  
  Spark* popcorn = reinterpret_cast<Spark*>(SEGMENT.data);

  float gravity = -0.0001 - (SEGMENT.speed()/200000.0); // m/s/s
  gravity *= SEGLEN;

  bool hasCol2 = SEGCOLOR_U32(2);
  fill(hasCol2 ? BLACK : SEGCOLOR_U32(1));

  uint8_t numPopcorn = SEGMENT.intensity()*maxNumPopcorn/255;

  if (numPopcorn == 0) numPopcorn = 1;

  for(uint8_t i = 0; i < numPopcorn; i++) {
    bool isActive = popcorn[i].pos >= 0.0f;

    if (isActive) { // if kernel is active, update its position
      popcorn[i].pos += popcorn[i].vel;
      popcorn[i].vel += gravity;
      uint32_t col = color_wheel(popcorn[i].colIndex);
      if (!SEGMENT.palette.id && popcorn[i].colIndex < NUM_COLORS)
      {
        col = SEGCOLOR_U32(popcorn[i].colIndex);
      }
      
      uint16_t ledIndex = popcorn[i].pos;
      if (ledIndex < SEGLEN) SEGMENT.SetPixelColor(ledIndex, col);
    } else { // if kernel is inactive, randomly pop it
      if (random8() < 2) { // POP!!!
        popcorn[i].pos = 0.01f;
        
        uint16_t peakHeight = 128 + random8(128); //0-255
        peakHeight = (peakHeight * (SEGLEN -1)) >> 8;
        popcorn[i].vel = sqrt(-2.0 * gravity * peakHeight);
        
        if (SEGMENT.palette.id)
        {
          popcorn[i].colIndex = random8();
        } else {
          byte col = random8(0, NUM_COLORS);
          if (!hasCol2 || !SEGCOLOR_U32(col)) col = 0;
          popcorn[i].colIndex = col;
        }
      }
    }
  }

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  

}
static const char PM_EFFECT_CONFIG__POPCORN[] PROGMEM = "!,!,,,,,,,Overlay;!,!,!;!;;m12=1"; //bar
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE


#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
/********************************************************************************************************************************************************************************************************************
*******************************************************************************************************************************************************************************************************************
   @brief: Plasma
   @note : Converted from WLED Effects
           Draws growing/shrinking spans of concentric faded light
           adapted from https://github.com/atuline/FastLED-Demos/blob/master/plasma/plasma.ino
   @note   Rate: FRAME_RATE

   @param rev             Reversed
   @param useRandomColors 
*******************************************************************************************************************************************************************************************************************
********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Plasma()
{

  uint8_t thisPhase = beatsin8(6,-64,64);                       // Setting phase change for a couple of waves.
  uint8_t thatPhase = beatsin8(7,-64,64);

  for (int i = 0; i < SEGMENT.length(); i++) {   // For each of the LED's in the strand, set color &  brightness based on a wave as follows:
    uint8_t colorIndex = cubicwave8((i*(1+ 3*(SEGMENT.speed() >> 5)))+(thisPhase) & 0xFF)/2   // factor=23 // Create a wave and add a phase change and add another wave with its own phase change.
                             + cos8((i*(1+ 2*(SEGMENT.speed() >> 5)))+(thatPhase) & 0xFF)/2;  // factor=15 // Hey, you can even change the frequencies if you wish.
    uint8_t thisBright = qsub8(colorIndex, beatsin8(6,0, (255 - SEGMENT.intensity())|0x01 ));
    CRGB color = ColorFromPalette(SEGMENT.palette_container->CRGB16Palette16_Palette.data, colorIndex, thisBright, LINEARBLEND);
    SEGMENT.SetPixelColor(i, color.red, color.green, color.blue);
  }
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
} 
static const char PM_EFFECT_CONFIG__PLASMA[] PROGMEM = "Phase,!;!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

/*
 * Percentage display
 * Intesity values from 0-100 turn on the leds.
 *
void mAnimatorLight::mode_percent(void) {

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
void mAnimatorLight::EffectAnim__Percent()
{
	uint8_t percent = MAX(0, MIN(200, SEGMENT.intensity()));
	uint16_t active_leds = (percent < 100) ? SEGLEN * percent / 100.0
                                         : SEGLEN * (200 - percent) / 100.0;
  
  uint8_t size = (1 + ((SEGMENT.speed() * SEGLEN) >> 11));
  if (SEGMENT.speed() == 255) size = 255;
    
  if (percent < 100) {
    for (uint16_t i = 0; i < SEGLEN; i++) {
	  	if (i < SEGMENT.step) {
        SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE), SET_BRIGHTNESS);
	  	}
	  	else {
        SEGMENT.SetPixelColor(i, SEGCOLOR_U32(1), SET_BRIGHTNESS);
	  	}
	  }
  } else {
    for (uint16_t i = 0; i < SEGLEN; i++) {
	  	if (i < (SEGLEN - SEGMENT.step)) {
        SEGMENT.SetPixelColor(i, SEGCOLOR_U32(1), SET_BRIGHTNESS);
	  	}
	  	else {
        SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE), SET_BRIGHTNESS);
	  	}
	  }
  }

  if(active_leds > SEGMENT.step) {  // smooth transition to the target value
    SEGMENT.step += size;
    if (SEGMENT.step > active_leds) SEGMENT.step = active_leds;
  } else if (active_leds < SEGMENT.step) {
    if (SEGMENT.step > size) SEGMENT.step -= size; else SEGMENT.step = 0;
    if (SEGMENT.step < active_leds) SEGMENT.step = active_leds;
  }

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  

}
static const char PM_EFFECT_CONFIG__PERCENT[] PROGMEM = ",% of fill,,,,One color;!,!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
//  "Pacifica"
//  Gentle, blue-green ocean waves.
//  December 2019, Mark Kriegsman and Mary Corey March.
//  For Dan.
//
//
// In this animation, there are four "layers" of waves of light.  
//
// Each layer moves independently, and each is scaled separately.
//
// All four wave layers are added together on top of each other, and then 
// another filter is applied that adds "whitecaps" of brightness where the 
// waves line up with each other more.  Finally, another pass is taken
// over the led array to 'deepen' (dim) the blues and greens.
//
// The speed() and scale and motion each layer varies slowly within independent 
// hand-chosen ranges, which is why the code has a lot of low-speed() 'beatsin8' functions
// with a lot of oddly specific numeric ranges.
//
// These three custom blue-green color palettes were inspired by the colors found in
// the waters off the southern coast of California, https://goo.gl/maps/QQgd97jjHesHZVxQ7
//
// Modified for WLED, based on https://github.com/FastLED/FastLED/blob/master/examples/Pacifica/Pacifica.ino
//

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

void mAnimatorLight::mode_pacifica()
{
/*
*  Bouncing Balls Effect
*
void mAnimatorLight::mode_bouncing_balls(void) {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Pacifica()
{
  CRGBPalette16 pacifica_palette_1 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50 };
  CRGBPalette16 pacifica_palette_2 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F };
  CRGBPalette16 pacifica_palette_3 = 
    { 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33, 
      0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };

  if (SEGMENT.palette.id) {
    pacifica_palette_1 = SEGMENT.palette_container->CRGB16Palette16_Palette.data;
    pacifica_palette_2 = SEGMENT.palette_container->CRGB16Palette16_Palette.data;
    pacifica_palette_3 = SEGMENT.palette_container->CRGB16Palette16_Palette.data;
  }

  // Increment the four "color index start" counters, one for each wave layer.
  // Each is incremented at a different speed(), and the speeds vary over time.
  uint16_t sCIStart1 = SEGMENT.params_internal.aux0, sCIStart2 = SEGMENT.params_internal.aux1, sCIStart3 = SEGMENT.step, sCIStart4 = SEGMENT.step >> 16;
  //static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
  uint32_t deltams = 26 + (SEGMENT.speed() >> 3);
  
  uint16_t speedfactor1 = beatsin16(3, 179, 269);
  uint16_t speedfactor2 = beatsin16(4, 179, 269);
  uint32_t deltams1 = (deltams * speedfactor1) / 256;
  uint32_t deltams2 = (deltams * speedfactor2) / 256;
  uint32_t deltams21 = (deltams1 + deltams2) / 2;
  sCIStart1 += (deltams1 * beatsin88(1011,10,13));
  sCIStart2 -= (deltams21 * beatsin88(777,8,11));
  sCIStart3 -= (deltams1 * beatsin88(501,5,7));
  sCIStart4 -= (deltams2 * beatsin88(257,4,6));
  SEGMENT.params_internal.aux0 = sCIStart1; SEGMENT.params_internal.aux1 = sCIStart2;
  SEGMENT.step = sCIStart4; SEGMENT.step = (SEGMENT.step << 16) + sCIStart3;

  // Clear out the LED array to a dim background blue-green
  //fill(132618);

  uint8_t basethreshold = beatsin8( 9, 55, 65);
  uint8_t wave = beat8( 7 );
  
  for( uint16_t i = 0; i < SEGLEN; i++) {
    CRGB c = CRGB(2, 6, 10);
    // Render each of four layers, with different scales and speeds, that vary over time
    c += pacifica_one_layer(i, pacifica_palette_1, sCIStart1, beatsin16(3, 11 * 256, 14 * 256), beatsin8(10, 70, 130), 0-beat16(301));
    c += pacifica_one_layer(i, pacifica_palette_2, sCIStart2, beatsin16(4,  6 * 256,  9 * 256), beatsin8(17, 40,  80),   beat16(401));
    c += pacifica_one_layer(i, pacifica_palette_3, sCIStart3,                         6 * 256 , beatsin8(9, 10,38)   , 0-beat16(503));
    c += pacifica_one_layer(i, pacifica_palette_3, sCIStart4,                         5 * 256 , beatsin8(8, 10,28)   ,   beat16(601));
    
    // Add extra 'white' to areas where the four layers of light have lined up brightly
    uint8_t threshold = scale8( sin8( wave), 20) + basethreshold;
    wave += 7;
    uint8_t l = c.getAverageLight();
    if (l > threshold) {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8(overage, overage);
      c += CRGB(overage, overage2, qadd8(overage2, overage2));
    }

    //deepen the blues and greens
    c.blue  = scale8(c.blue,  145); 
    c.green = scale8(c.green, 200); 
    c |= CRGB( 2, 5, 7);

    SEGMENT.SetPixelColor(i, c.red, c.green, c.blue);
  }

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__PACIFICA[] PROGMEM = "!,Angle;;!;;pal=51";

// Add one layer of waves into the led array
CRGB mAnimatorLight::pacifica_one_layer(uint16_t i, CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff)
{

  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale >> 1) + 20;
  
  waveangle += ((120 + SEGMENT.intensity()) * i); //original 250 * i
  uint16_t s16 = sin16(waveangle) + 32768;
  uint16_t cs = scale16(s16, wavescale_half) + wavescale_half;
  ci += (cs * i);
  uint16_t sindex16 = sin16(ci) + 32768;
  uint8_t sindex8 = scale16(sindex16, 240);
  return ColorFromPalette(p, sindex8, bri, LINEARBLEND);
}

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

//Solid colour background with glitter
void mAnimatorLight::mode_solid_glitter()
{
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Solid_Glitter()
{

  fill(SEGCOLOR_U32(0));

  if (SEGMENT.intensity() > random8())
  {
    SEGMENT.SetPixelColor(random16(SEGLEN), ULTRAWHITE);
  }
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__SOLID_GLITTER[] PROGMEM = ",!;Bg,,Glitter color;;;m12=0";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

/*
 * Mode simulates a gradual sunrise
 *
// void mAnimatorLight::mode_sunrise() {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Sunrise()
{
  
  //speed() 0 - static sun
  //speed() 1 - 60: sunrise time in minutes
  //speed() 60 - 120 : sunset time in minutes - 60;
  //speed() above: "breathing" rise and set
  if (SEGMENT.call == 0 || SEGMENT.speed() != SEGMENT.params_internal.aux0) {
	  SEGMENT.step = millis(); //save starting time, millis() because now can change from sync
    SEGMENT.params_internal.aux0 = SEGMENT.speed();
  }
  
  fill(0);
  uint16_t stage = 0xFFFF;
  
  uint32_t s10SinceStart = (millis() - SEGMENT.step) /100; //tenths of seconds
  
  if (SEGMENT.speed() > 120) { //quick sunrise and sunset
	  uint16_t counter = (millis() >> 1) * (((SEGMENT.speed() -120) >> 1) +1);
	  stage = triwave16(counter);
  } else if (SEGMENT.speed()) { //sunrise
	  uint8_t durMins = SEGMENT.speed();
	  if (durMins > 60) durMins -= 60;
	  uint32_t s10Target = durMins * 600;
	  if (s10SinceStart > s10Target) s10SinceStart = s10Target;
	  stage = map(s10SinceStart, 0, s10Target, 0, 0xFFFF);
	  if (SEGMENT.speed() > 60) stage = 0xFFFF - stage; //sunset
  }
  
  for (uint16_t i = 0; i <= SEGLEN/2; i++)
  {
    //default palette is Fire
    uint32_t c = SEGMENT.GetPaletteColour(0, PALETTE_SPAN_OFF, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32(); // background

    uint16_t wave = triwave16((i * stage) / SEGLEN);

    wave = (wave >> 8) + ((wave * SEGMENT.intensity()) >> 15);

    if (wave > 240) { //clipped, full white sun
      c = SEGMENT.GetPaletteColour(240, PALETTE_SPAN_OFF, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32();
    } else { //transition
      c = SEGMENT.GetPaletteColour(wave, PALETTE_SPAN_OFF, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32();
    }
    SEGMENT.SetPixelColor(i, c);
    SEGMENT.SetPixelColor(SEGLEN - i - 1, c);
  }

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__SUNRISE[] PROGMEM = "Time [min],Width;;!;;sx=60";

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 


// Sine waves that have controllable phase change speed(), frequency and cutoff. By Andrew Tuline.
// SEGMENT.speed() ->Speed, SEGMENT.intensity() -> Frequency (SEGMENT.fft1 -> Color change, SEGMENT.fft2 -> PWM cutoff)
//
void mAnimatorLight::mode_sinewave(void) {             // Adjustable sinewave. By Andrew Tuline

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Sinewave()
{
  
  //#define qsuba(x, b)  ((x>b)?x-b:0)               // Analog Unsigned subtraction macro. if result <0, then => 0

  uint16_t colorIndex = millis() /32;//(256 - SEGMENT.fft1);  // Amount of colour change.

  SEGMENT.step += SEGMENT.speed()/16;                   // Speed of animation.
  uint16_t freq = SEGMENT.intensity()/4;//SEGMENT.fft2/8;                       // Frequency of the signal.

  for (int i=0; i<SEGLEN; i++) {                    // For each of the LED's in the strand, set a brightness based on a wave as follows:
    int pixBri = cubicwave8((i*freq)+SEGMENT.step); //qsuba(cubicwave8((i*freq)+SEGMENT.step), (255-SEGMENT.intensity())); // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    //setPixCol(i, i*colorIndex/255, pixBri);
    SEGMENT.SetPixelColor(i, ColourBlend(SEGCOLOR_RGBCCT(1), SEGMENT.GetPaletteColour(i*colorIndex/255, PALETTE_SPAN_OFF, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE), pixBri) );
  }

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__SINEWAVE[] PROGMEM = "";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * Best of both worlds from Palette and Spot effects. By Aircoookie
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Flow()
{
  
  uint16_t counter = 0;
  if (SEGMENT.speed() != 0) 
  {
    counter = millis() * ((SEGMENT.speed() >> 2) +1);
    counter = counter >> 8;
  }
  
  uint16_t maxZones = SEGLEN / 6; //only looks good if each zone has at least 6 LEDs
  uint16_t zones = (SEGMENT.intensity() * maxZones) >> 8;
  if (zones & 0x01) zones++; //zones must be even
  if (zones < 2) zones = 2;
  uint16_t zoneLen = SEGLEN / zones;
  uint16_t offset = (SEGLEN - zones * zoneLen) >> 1;

  fill(SEGMENT.GetPaletteColour(-counter, PALETTE_SPAN_OFF, PALETTE_WRAP_ON, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE));

  for (uint16_t z = 0; z < zones; z++)
  {
    uint16_t pos = offset + z * zoneLen;
    for (uint16_t i = 0; i < zoneLen; i++)
    {
      uint8_t colorIndex = (i * 255 / zoneLen) - counter;
      uint16_t led = (z & 0x01) ? i : (zoneLen -1) -i;
      if (IS_REVERSE) led = (zoneLen -1) -led;
      SEGMENT.SetPixelColor(pos + led, SEGMENT.GetPaletteColour(colorIndex, PALETTE_SPAN_OFF, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
    }
  }

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__FLOW[] PROGMEM = "!,Zones;;!;;m12=1"; //vertical
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

/****************************************************************************************************************************
 **************************************************************************************************************************** 
 * Static
 ****************************************************************************************************************************
 ****************************************************************************************************************************/


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : 
 * @description: 
 * @note : Converted from WLED Effects (updated to use the first colour of any palette)
 * @note : Static Fill, uses single colours to fill palette
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
void mAnimatorLight::EffectAnim__Static_Solid()
{  
  fill(SEGMENT.GetPaletteColour(0, PALETTE_SPAN_OFF, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE), SET_BRIGHTNESS);
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
}
static const char PM_EFFECT_CONFIG__STATIC[] PROGMEM = "";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * Speed slider sets amount of LEDs lit, intensity() sets unlit
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
void mAnimatorLight::EffectAnim__Static_Pattern()
{
  
  uint16_t lit = 1 + SEGMENT.speed();
  uint16_t unlit = 1 + SEGMENT.intensity();
  bool drawingLit = true;
  uint16_t cnt = 0;

  for (uint16_t i = 0; i < SEGLEN; i++) {
    SEGMENT.SetPixelColor(i, 
      (drawingLit) ? RgbcctColor::GetU32Colour(SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE)) : SEGCOLOR_U32(1),
      true
    );
    cnt++;
    if (cnt >= ((drawingLit) ? lit : unlit)) {
      cnt = 0;
      drawingLit = !drawingLit;
    }
  }
  
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__STATIC_PATTERN[] PROGMEM = "Fg size,Bg size;Fg,!;!;;pal=0";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Blink/strobe function
 * @note : Converted from WLED Effects
 * 
 * Alternate between color1 and color2
 * if(strobe == true) then create a strobe effect
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
void mAnimatorLight::EffectAnim__Base_Blink(uint32_t color1, uint32_t color2, bool strobe, bool do_palette)
{

  uint32_t cycleTime = (255 - SEGMENT.speed())*20;
  uint32_t onTime = FRAMETIME;
  if (!strobe) onTime += ((cycleTime * SEGMENT.intensity()) >> 8);
  cycleTime += FRAMETIME*2;
  uint32_t it = _now / cycleTime;
  uint32_t rem = _now % cycleTime;

  bool on = false;
  if (it != SEGMENT.step //new iteration, force on state for one frame, even if set time is too brief
      || rem <= onTime) {
    on = true;
  }

  SEGMENT.step = it; //save previous iteration

  uint32_t color = on ? color1 : color2;
  if (color == color1 && do_palette)
  {
    for (int i = 0; i < SEGLEN; i++) {
      SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
    }
  }
  else
  {
    SEGMENT.fill(color); // alternates between colours passed in unless do_pal is set
  }
  
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 *  Normal blinking. 50% on/off time.
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Blink()
{
  EffectAnim__Base_Blink(SEGCOLOR_U32(0), SEGCOLOR_U32(1), false, true);  
}
static const char PM_EFFECT_CONFIG__BLINK[] PROGMEM = "!,Duty cycle;!,!;!;01";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * Classic Blink effect. Cycling through the rainbow.
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Blink_Rainbow()
{
  EffectAnim__Base_Blink(color_wheel(SEGMENT.call & 0xFF), SEGCOLOR_U32(1), false, false);  
}
static const char PM_EFFECT_CONFIG__BLINK_RAINBOW[] PROGMEM = "Frequency,Blink duration;!,!;!;01";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * Classic Strobe effect.
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Strobe()
{
  EffectAnim__Base_Blink(SEGCOLOR_U32(0), SEGCOLOR_U32(1), true, true);
}
static const char PM_EFFECT_CONFIG__STROBE[] PROGMEM = "!;!,!;!;01";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * Strobe effect with different strobe count and pause, controlled by speed().
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
void mAnimatorLight::EffectAnim__Strobe_Multi()
{
  for(uint16_t i = 0; i < SEGLEN; i++) {
    SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
  }

  uint16_t delay = 50 + 20*(uint16_t)(255-SEGMENT.speed());
  uint16_t count = 2 * ((SEGMENT.speed() / 10) + 1);
  if(SEGMENT.step < count) {
    if((SEGMENT.step & 1) == 0) {
      for(uint16_t i = 0; i < SEGLEN; i++) {
        SEGMENT.SetPixelColor(i, SEGCOLOR_U32(0));
      }
      delay = 20;
    } else {
      delay = 50;
    }
  }
  SEGMENT.step = (SEGMENT.step + 1) % (count + 1);  
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__MULTI_STROBE[] PROGMEM = "!,!;!,!;!;01";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : 
 * Classic Strobe effect. Cycling through the rainbow.
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Strobe_Rainbow()
{
  EffectAnim__Base_Blink(color_wheel(SEGMENT.call & 0xFF), SEGCOLOR_U32(1), true, false);
}
static const char PM_EFFECT_CONFIG__STROBE_RAINBOW[] PROGMEM = "!;,!;!;01";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * Cycles all LEDs at once through a rainbow.
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Rainbow()
{

  uint16_t counter = (millis() * ((SEGMENT.speed() >> 2) +2)) & 0xFFFF;
  counter = counter >> 8;

  if (SEGMENT.intensity() < 128){
    fill(ColourBlend(color_wheel(counter),WHITE,128-SEGMENT.intensity()));
  } else {
    fill(color_wheel(counter));
  }

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__RAINBOW[] PROGMEM = "!,Saturation;;!";

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Lightning()
{
 uint16_t ledstart = random16(SEGLEN);               // Determine starting location of flash
  uint16_t ledlen = 1 + random16(SEGLEN -ledstart);   // Determine length of flash (not to go beyond NUM_LEDS-1)
  uint8_t bri = 255/random8(1, 3);

  if (SEGMENT.params_internal.aux1 == 0) //init, leader flash
  {
    SEGMENT.params_internal.aux1 = random8(4, 4 + SEGMENT.intensity()/20); //number of flashes
    SEGMENT.params_internal.aux1 *= 2;

    bri = 52; //leader has lower brightness
    SEGMENT.params_internal.aux0 = 200; //200ms delay after leader
  }

  fill(SEGCOLOR_U32(1));

  if (SEGMENT.params_internal.aux1 > 3 && !(SEGMENT.params_internal.aux1 & 0x01)) { //flash on even number >2
    for (int i = ledstart; i < ledstart + ledlen; i++)
    {
      SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
    }
    SEGMENT.params_internal.aux1--;

    SEGMENT.step = millis();
    //return random8(4, 10); // each flash only lasts one frame/every 24ms... originally 4-10 milliseconds
  } else {
    if (millis() - SEGMENT.step > SEGMENT.params_internal.aux0) {
      SEGMENT.params_internal.aux1--;
      if (SEGMENT.params_internal.aux1 < 2) SEGMENT.params_internal.aux1 = 0;

      SEGMENT.params_internal.aux0 = (50 + random8(100)); //delay between flashes
      if (SEGMENT.params_internal.aux1 == 2) {
        SEGMENT.params_internal.aux0 = (random8(255 - SEGMENT.speed()) * 100); // delay between strikes
      }
      SEGMENT.step = millis();
    }
  }
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__LIGHTNING[] PROGMEM = "!,!,,,,,,,Overlay;!,!;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : mode_fire_2012()
 * 
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation, 
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on SEGLEN; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above) (Speed = COOLING), and SPARKING (used
// in step 3 above) (Effect Intensity = Sparking).
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Fire_2012()
{

  uint32_t it = millis() >> 5; //div 32

  if (!SEGMENT.allocateData(SEGLEN)){return;} // return mode_static(); //allocation failed
  
  byte* heat = SEGMENT.data;

  if (it != SEGMENT.step)
  {
    // Step 1.  Cool down every cell a little
    for (uint16_t i = 0; i < SEGLEN; i++) {
      SEGMENT.data[i] = qsub8(heat[i],  random8(0, (((20 + SEGMENT.speed() /3) * 10) / SEGLEN) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (uint16_t k= SEGLEN -1; k > 1; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if (random8() <= SEGMENT.intensity()) {
      uint8_t y = random8(7);
      if (y < SEGLEN) heat[y] = qadd8(heat[y], random8(160,255));
    }
    SEGMENT.step = it;
  }

  // Step 4.  Map from heat cells to LED colors
  for (uint16_t j = 0; j < SEGLEN; j++) {
    CRGB color = ColorFromPalette(SEGMENT.palette_container->CRGB16Palette16_Palette.data, MIN(heat[j],240), 255, LINEARBLEND);
    SEGMENT.SetPixelColor(j, color.red, color.green, color.blue);
  }
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__FIRE_2012[] PROGMEM = "Cooling,Spark rate,,,Boost;;!;1;sx=64,ix=160,m12=1";



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Railway Crossing / Christmas Fairy lights
 * @note : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Railway()
{

  uint16_t dur = 40 + (255 - SEGMENT.speed()) * 10;
  uint16_t rampdur = (dur * SEGMENT.intensity()) >> 8;
  if (SEGMENT.step > dur)
  {
    //reverse direction
    SEGMENT.step = 0;
    SEGMENT.params_internal.aux0 = !SEGMENT.params_internal.aux0;
  }
  uint8_t pos = 255;
  if (rampdur != 0)
  {
    uint16_t p0 = (SEGMENT.step * 255) / rampdur;
    if (p0 < 255) pos = p0;
  }
  if (SEGMENT.params_internal.aux0) pos = 255 - pos;
  for (uint16_t i = 0; i < SEGLEN; i += 2)
  {
    SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(255 - pos, PALETTE_SPAN_OFF, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
    if (i < SEGLEN -1)
    {
      SEGMENT.SetPixelColor(i + 1, SEGMENT.GetPaletteColour(pos, PALETTE_SPAN_OFF, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
    }
  }
  SEGMENT.step += FRAMETIME_MS;
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__RAILWAY[] PROGMEM = "!,Smoothness;1,2;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
/*
/ Modulates the brightness similar to a heartbeat
*
void mAnimatorLight::mode_heartbeat(void) {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Heartbeat()
{
  uint8_t bpm = 40 + (SEGMENT.speed() >> 4);
  uint32_t msPerBeat = (60000 / bpm);
  uint32_t secondBeat = (msPerBeat / 3);

  uint32_t bri_lower = SEGMENT.params_internal.aux1;
  bri_lower = bri_lower * 2042 / (2048 + SEGMENT.intensity());
  SEGMENT.params_internal.aux1 = bri_lower;

  unsigned long beatTimer = millis() - SEGMENT.step;
  if((beatTimer > secondBeat) && !SEGMENT.params_internal.aux0) { // time for the second beat?
    SEGMENT.params_internal.aux1 = UINT16_MAX; //full bri
    SEGMENT.params_internal.aux0 = 1;
  }
  if(beatTimer > msPerBeat) { // time to reset the beat timer?
    SEGMENT.params_internal.aux1 = UINT16_MAX; //full bri
    SEGMENT.params_internal.aux0 = 0;
    SEGMENT.step = millis();
  }

  for (uint16_t i = 0; i < SEGLEN; i++) {
    SEGMENT.SetPixelColor(i, ColourBlend(RgbcctColor::GetU32Colour(SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE)), SEGCOLOR_U32(1), 255 - (SEGMENT.params_internal.aux1 >> 8)));
  }

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__HEARTBEAT[] PROGMEM = "!,!;!,!;!;01;m12=1";


/****************************************************************************************************************************
 **************************************************************************************************************************** 
 * Noise
 ****************************************************************************************************************************
 ****************************************************************************************************************************/


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__FillNoise8()
{
  if (SEGMENT.call == 0) SEGMENT.step = random16(12345);
  CRGB fastled_col;
  for (uint16_t i = 0; i < SEGLEN; i++) {
    uint8_t index = inoise8(i * SEGLEN, SEGMENT.step + i * SEGLEN);
    fastled_col = ColorFromPalette(SEGMENT.palette_container->CRGB16Palette16_Palette.data, index, 255, LINEARBLEND);
    SEGMENT.SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }
  SEGMENT.step += beatsin8(SEGMENT.speed(), 1, 6); //10,1,4

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__FILLNOISE8[] PROGMEM = "!;!;!";

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Noise16_1()
{

  uint16_t scale = 320;                                      // the "zoom factor" for the noise
  CRGB fastled_col;
  SEGMENT.step += (1 + SEGMENT.speed()/16);

  for (uint16_t i = 0; i < SEGLEN; i++) {

    uint16_t shift_x = beatsin8(11);                           // the x position of the noise field swings @ 17 bpm
    uint16_t shift_y = SEGMENT.step/42;             // the y position becomes slowly incremented


    uint16_t real_x = (i + shift_x) * scale;                  // the x position of the noise field swings @ 17 bpm
    uint16_t real_y = (i + shift_y) * scale;                  // the y position becomes slowly incremented
    uint32_t real_z = SEGMENT.step;                          // the z position becomes quickly incremented

    uint8_t noise = inoise16(real_x, real_y, real_z) >> 8;   // get the noise data and scale it down

    uint8_t index = sin8(noise * 3);                         // map LED color based on noise data

    fastled_col = ColorFromPalette(SEGMENT.palette_container->CRGB16Palette16_Palette.data, index, 255, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
    SEGMENT.SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__NOISE16_1[] PROGMEM = "!;!;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Noise16_2()
{
  uint16_t scale = 1000;                                       // the "zoom factor" for the noise
  CRGB fastled_col;
  SEGMENT.step += (1 + (SEGMENT.speed() >> 1));

  for (uint16_t i = 0; i < SEGLEN; i++) {

    uint16_t shift_x = SEGMENT.step >> 6;                         // x as a function of time
    uint16_t shift_y = SEGMENT.step/42;

    uint32_t real_x = (i + shift_x) * scale;                  // calculate the coordinates within the noise field

    uint8_t noise = inoise16(real_x, 0, 4223) >> 8;    // get the noise data and scale it down

    uint8_t index = sin8(noise * 3);                          // map led color based on noise data

    fastled_col = ColorFromPalette(SEGMENT.palette_container->CRGB16Palette16_Palette.data, index, noise, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
    SEGMENT.SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__NOISE16_2[] PROGMEM = "!;!;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Noise16_3()
{
  uint16_t scale = 800;                                       // the "zoom factor" for the noise
  CRGB fastled_col;
  SEGMENT.step += (1 + SEGMENT.speed());

  for (uint16_t i = 0; i < SEGLEN; i++) {

    uint16_t shift_x = 4223;                                  // no movement along x and y
    uint16_t shift_y = 1234;

    uint32_t real_x = (i + shift_x) * scale;                  // calculate the coordinates within the noise field
    uint32_t real_y = (i + shift_y) * scale;                  // based on the precalculated positions
    uint32_t real_z = SEGMENT.step*8;  

    uint8_t noise = inoise16(real_x, real_y, real_z) >> 8;    // get the noise data and scale it down

    uint8_t index = sin8(noise * 3);                          // map led color based on noise data

    fastled_col = ColorFromPalette(SEGMENT.palette_container->CRGB16Palette16_Palette.data, index, noise, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
    SEGMENT.SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__NOISE16_3[] PROGMEM = "!;!;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : https://github.com/aykevl/ledstrip-spark/blob/master/ledstrip->ino
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Noise16_4()
{
  CRGB fastled_col;
  uint32_t stp = (millis() * SEGMENT.speed()) >> 7;
  for (uint16_t i = 0; i < SEGLEN; i++) {
    int16_t index = inoise16(uint32_t(i) << 12, stp);
    fastled_col = ColorFromPalette(SEGMENT.palette_container->CRGB16Palette16_Palette.data, index);
    SEGMENT.SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__NOISE16_4[] PROGMEM = "!;!;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Peaceful noise that's slow and with gradually changing palettes. Does not support WLED palettes or default colours or controls.
 *         Slow noise palette by Andrew Tuline.
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Noise_Pal()
{
  uint16_t scale = 15 + (SEGMENT.intensity() >> 2); //default was 30
  //#define scale 30

  uint16_t dataSize = sizeof(CRGBPalette16) * 2; //allocate space for 2 Palettes
  if (!SEGMENT.allocateData(dataSize)){return;}// return mode_static(); //allocation failed

  CRGBPalette16* palettes = reinterpret_cast<CRGBPalette16*>(SEGMENT.data);

  uint16_t changePaletteMs = 4000 + SEGMENT.speed() *10; //between 4 - 6.5sec
  if (millis() - SEGMENT.step > changePaletteMs)
  {
    SEGMENT.step = millis();

    uint8_t baseI = random8();
    palettes[1] = CRGBPalette16(CHSV(baseI+random8(64), 255, random8(128,255)), CHSV(baseI+128, 255, random8(128,255)), CHSV(baseI+random8(92), 192, random8(128,255)), CHSV(baseI+random8(92), 255, random8(128,255)));
  }

  CRGB color;

  //EVERY_N_MILLIS(10) { //(don't have to time this, effect function is only called every 24ms)
  nblendPaletteTowardPalette(palettes[0], palettes[1], 48);               // Blend towards the target palette over 48 iterations.

  if (SEGMENT.palette.id > 0) palettes[0] = SEGMENT.palette_container->CRGB16Palette16_Palette.data;

  for(int i = 0; i < SEGLEN; i++) {
    uint8_t index = inoise8(i*scale, SEGMENT.params_internal.aux0+i*scale);                // Get a value from the noise function. I'm using both x and y axis.
    color = ColorFromPalette(palettes[0], index, 255, LINEARBLEND);       // Use the my own palette.
    SEGMENT.SetPixelColor(i, color.red, color.green, color.blue);
  }

  SEGMENT.params_internal.aux0 += beatsin8(10,1,4);                                        // Moving along the distance. Vary it a bit with a sine wave.

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__NOISEPAL[] PROGMEM = "!,Scale;;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : We're making sine waves here. By Andrew Tuline.
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Base_Phased(uint8_t moder)
{
  uint8_t allfreq = 16;                                          // Base frequency.
  //float* phasePtr = reinterpret_cast<float*>(SEGMENT.step);       // Phase change value gets calculated.
  static float phase = 0;//phasePtr[0];
  uint8_t cutOff = (255-SEGMENT.intensity());                      // You can change the number of pixels.  AKA intensity() (was 192).
  uint8_t modVal = 5;//SEGMENT.fft1/8+1;                         // You can change the modulus. AKA FFT1 (was 5).

  uint8_t index = millis()/64;                                    // Set color rotation speed()
  phase += SEGMENT.speed()/32.0;                                   // You can change the speed() of the wave. AKA SPEED (was .4)
  //phasePtr[0] = phase; 

  for (int i = 0; i < SEGLEN; i++) {
    if (moder == 1) modVal = (inoise8(i*10 + i*10) /16);         // Let's randomize our mod length with some Perlin noise.
    uint16_t val = (i+1) * allfreq;                              // This sets the frequency of the waves. The +1 makes sure that leds[0] is used.
    if (modVal == 0) modVal = 1;
    val += phase * (i % modVal +1) /2;                           // This sets the varying phase change of the waves. By Andrew Tuline.
    uint8_t b = cubicwave8(val);                                 // Now we make an 8 bit sinewave.
    b = (b > cutOff) ? (b - cutOff) : 0;                         // A ternary operator to cutoff the light.
    SEGMENT.SetPixelColor(i, ColourBlend(SEGCOLOR_RGBCCT(1), SEGMENT.GetPaletteColour(index, PALETTE_SPAN_OFF, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE), b));
    index += 256 / SEGLEN;
  }
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}


void mAnimatorLight::EffectAnim__PhasedNoise(void) {
  return EffectAnim__Base_Phased(1);
}
static const char PM_EFFECT_CONFIG__PHASEDNOISE[] PROGMEM = "!,!;!,!;!";


void mAnimatorLight::EffectAnim__Phased(void) {
  return EffectAnim__Base_Phased(0);
}
static const char PM_EFFECT_CONFIG__PHASED[] PROGMEM = "!,!;!,!;!";



/****************************************************************************************************************************
 **************************************************************************************************************************** 
 * Scan
 ****************************************************************************************************************************
 ****************************************************************************************************************************/



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Base function for scanners
 * @note : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Base_Scan(bool dual)
{

  uint32_t cycleTime = 750 + (255 - SEGMENT.speed())*150;
  uint32_t perc = millis() % cycleTime;
  uint16_t prog = (perc * 65535) / cycleTime;
  uint16_t size = 1 + ((SEGMENT.intensity() * SEGLEN) >> 9);
  uint16_t ledIndex = (prog * ((SEGLEN *2) - size *2)) >> 16;

  fill(SEGCOLOR_U32(1));

  int led_offset = ledIndex - (SEGLEN - size);
  led_offset = abs(led_offset);

  if (dual) {
    for (uint16_t j = led_offset; j < led_offset + size; j++) {
      uint16_t i2 = SEGLEN -1 -j;
      SEGMENT.SetPixelColor(i2, SEGMENT.GetPaletteColour(i2, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE) ); // mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette.id, i2, nullptr, true, PALETTE_SOLID_WRAP, (SEGCOLOR_U32(2))? 2:0)
    }
  }

  for (uint16_t j = led_offset; j < led_offset + size; j++) {
    SEGMENT.SetPixelColor(j, SEGMENT.GetPaletteColour(j, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE) );
  }

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  

}


/*
 * Runs a single pixel back and forth.
 */
void mAnimatorLight::EffectAnim__Scan()
{
  EffectAnim__Base_Scan(false);
}
static const char PM_EFFECT_CONFIG__SCAN[] PROGMEM = "!,# of dots,,,,,,,Overlay;!,!,!;!";


/*
 * Runs two pixel back and forth in opposite directions.
 */
void mAnimatorLight::EffectAnim__Scan_Dual()
{
  EffectAnim__Base_Scan(true);
}
static const char PM_EFFECT_CONFIG__DUAL_SCAN[] PROGMEM = "!,# of dots,,,,,,,Overlay;!,!,!;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Base_Larson_Scanner(bool dual)
{
  
  uint16_t counter = millis() * ((SEGMENT.speed() >> 2) +8);
  uint16_t index = counter * SEGLEN  >> 16;

  fade_out(SEGMENT.intensity(), SET_BRIGHTNESS);

  if (SEGMENT.step > index && SEGMENT.step - index > SEGLEN/2) {
    SEGMENT.params_internal.aux0 = !SEGMENT.params_internal.aux0;
  }
  
  for (uint16_t i = SEGMENT.step; i < index; i++) {
    uint16_t j = (SEGMENT.params_internal.aux0)?i:SEGLEN-1-i;
    SEGMENT.SetPixelColor( j, SEGMENT.GetPaletteColour(j, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE) );
  }
  if (dual) {
    uint32_t c;
    if (SEGCOLOR_U32(2) != 0) {
      c = SEGCOLOR_U32(2);
    } else {
      c = SEGMENT.GetPaletteColour(index, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32();
    }

    for (uint16_t i = SEGMENT.step; i < index; i++) {
      uint16_t j = (SEGMENT.params_internal.aux0)?SEGLEN-1-i:i;
      SEGMENT.SetPixelColor(j, c);
    }
  }

  SEGMENT.step = index;
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}


/*
 * K.I.T.T.
 */
void mAnimatorLight::EffectAnim__Larson_Scanner()
{
  EffectAnim__Base_Larson_Scanner(false);
}
static const char PM_EFFECT_CONFIG__LARSON_SCANNER[] PROGMEM = "!,Fade rate;!,!;!;;m12=0";


/*
 * Creates two Larson scanners moving in opposite directions
 * Custom mode by Keith Lord: https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/DualLarson.h
 */
void mAnimatorLight::EffectAnim__Larson_Scanner_Dual(void){
  return EffectAnim__Base_Larson_Scanner(true);
}
static const char PM_EFFECT_CONFIG__DUAL_LARSON_SCANNER[] PROGMEM = "!,Fade rate;!,!,!;!;;m12=0";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__ICU()
{
  uint16_t dest = SEGMENT.step & 0xFFFF;
  uint8_t space = (SEGMENT.intensity() >> 3) +2;

  fill(SEGCOLOR_U32(1));

  byte pindex = map(dest, 0, SEGLEN-SEGLEN/space, 0, 255);
  uint32_t col = SEGMENT.GetPaletteColour(pindex, PALETTE_SPAN_OFF, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32();
  
  SEGMENT.SetPixelColor(dest, col);
  SEGMENT.SetPixelColor(dest + SEGLEN/space, col);

  if(SEGMENT.params_internal.aux0 == dest) { // pause between eye movements
    if(random8(6) == 0) { // blink once in a while
      SEGMENT.SetPixelColor(dest, SEGCOLOR_U32(1));
      SEGMENT.SetPixelColor(dest + SEGLEN/space, SEGCOLOR_U32(1));
      SEGMENT.transition.rate_ms = 200;
      return;
    }
    SEGMENT.params_internal.aux0 = random16(SEGLEN-SEGLEN/space);
    SEGMENT.transition.rate_ms = 1000 + random16(2000);
    return;
  }

  if(SEGMENT.params_internal.aux0 > SEGMENT.step) {
    SEGMENT.step++;
    dest++;
  } else if (SEGMENT.params_internal.aux0 < SEGMENT.step) {
    SEGMENT.step--;
    dest--;
  }

  SEGMENT.SetPixelColor(dest, col);
  SEGMENT.SetPixelColor(dest + SEGLEN/space, col);

  SEGMENT.transition.rate_ms = SPEED_FORMULA_L;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__ICU[] PROGMEM = "!,!,,,,,,,Overlay;!,!;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : //Water ripple
//propagation velocity from speed()
//drop rate from intensity()
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

//4 bytes
typedef struct Ripple {
  uint8_t state;
  uint8_t color;
  uint16_t pos;
} ripple;

void mAnimatorLight::EffectAnim__Base_Ripple(bool rainbow)
{

  uint16_t maxRipples = 1 + (SEGLEN >> 2);
  if (maxRipples > 100) maxRipples = 100;
  uint16_t dataSize = sizeof(ripple) * maxRipples;

  if (!SEGMENT.allocateData(dataSize)){return;} //return mode_static(); //allocation failed
 
  Ripple* ripples = reinterpret_cast<Ripple*>(SEGMENT.data);

  // ranbow background or chosen background, all very dim.
  if (rainbow) {
    if (SEGMENT.call ==0) {
      SEGMENT.params_internal.aux0 = random8();
      SEGMENT.params_internal.aux1 = random8();
    }
    if (SEGMENT.params_internal.aux0 == SEGMENT.params_internal.aux1) {
      SEGMENT.params_internal.aux1 = random8();
    }
    else if (SEGMENT.params_internal.aux1 > SEGMENT.params_internal.aux0) {
      SEGMENT.params_internal.aux0++;
    } else {
      SEGMENT.params_internal.aux0--;
    }
    fill(ColourBlend(color_wheel(SEGMENT.params_internal.aux0),BLACK,235));
  } else {
    fill(SEGCOLOR_U32(1));
  }
  
  //draw wave
  for (uint16_t i = 0; i < maxRipples; i++)
  {
    uint16_t ripplestate = ripples[i].state;
    if (ripplestate)
    {
      uint8_t rippledecay = (SEGMENT.speed() >> 4) +1; //faster decay if faster propagation
      uint16_t rippleorigin = ripples[i].pos;
      uint32_t col = SEGMENT.GetPaletteColour(ripples[i].color, PALETTE_SPAN_OFF, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32();

      uint16_t propagation = ((ripplestate/rippledecay -1) * SEGMENT.speed());
      int16_t propI = propagation >> 8;
      uint8_t propF = propagation & 0xFF;
      int16_t left = rippleorigin - propI -1;
      uint8_t amp = (ripplestate < 17) ? triwave8((ripplestate-1)*8) : map(ripplestate,17,255,255,2);

      for (int16_t v = left; v < left +4; v++)
      {
        uint8_t mag = scale8(cubicwave8((propF>>2)+(v-left)*64), amp);
        if (v < SEGLEN && v >= 0)
        {
          SEGMENT.SetPixelColor(v, ColourBlend(RgbcctColor::GetU32Colour(SEGMENT.GetPixelColor(v)), col, mag));
        }
        int16_t w = left + propI*2 + 3 -(v-left);
        if (w < SEGLEN && w >= 0)
        {
          SEGMENT.SetPixelColor(w, ColourBlend(RgbcctColor::GetU32Colour(SEGMENT.GetPixelColor(w)), col, mag));
        }
      }  
      ripplestate += rippledecay;
      ripples[i].state = (ripplestate > 254) ? 0 : ripplestate;
    } else //randomly create new wave
    {
      if (random16(IBN + 10000) <= SEGMENT.intensity())
      {
        ripples[i].state = 1;
        ripples[i].pos = random16(SEGLEN);
        ripples[i].color = random8(); //color
      }
    }
  }
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}

void mAnimatorLight::EffectAnim__Ripple(void) {
  return EffectAnim__Base_Ripple(false);
}
static const char PM_EFFECT_CONFIG__RIPPLE[] PROGMEM = "!,Wave #,,,,,,,Overlay;,!;!;12";

void mAnimatorLight::EffectAnim__Ripple_Rainbow(void) {
  return EffectAnim__Base_Ripple(true);
}
static const char PM_EFFECT_CONFIG__RIPPLE_RAINBOW[] PROGMEM = "!,Wave #;;!;12";



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Firing comets from one end. "Lighthouse"
 *
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Comet()
{

  uint16_t counter = millis() * ((SEGMENT.speed() >>2) +1);
  uint16_t index = counter * SEGLEN >> 16;
  if (SEGMENT.call == 0) SEGMENT.params_internal.aux0 = index;

  fade_out(SEGMENT.intensity(), SET_BRIGHTNESS);

  SEGMENT.SetPixelColor( index, SEGMENT.GetPaletteColour(index, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE) );
  
  if (index > SEGMENT.params_internal.aux0) {
    for (uint16_t i = SEGMENT.params_internal.aux0; i < index ; i++) {
       SEGMENT.SetPixelColor( i, SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
    }
  } else if (index < SEGMENT.params_internal.aux0 && index < 10) {
    for (uint16_t i = 0; i < index ; i++) {
       SEGMENT.SetPixelColor( i, SEGMENT.GetPaletteColour(i, PALETTE_SPAN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
    }      
  }
  SEGMENT.params_internal.aux0 = index++;

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__COMET[] PROGMEM = "!,Fade rate;!,!;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Dots waving around in a sine/pendulum motion.
 *         Little pixel birds flying in a circle. By Aircoookie
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Chunchun()
{
  fill(SEGCOLOR_U32(1));
  uint16_t counter = millis()*(6 + (SEGMENT.speed() >> 4));
  uint16_t numBirds = SEGLEN >> 2;
  uint16_t span = SEGMENT.intensity() << 8;

  for (uint16_t i = 0; i < numBirds; i++)
  {
    counter -= span/numBirds;
    int megumin = sin16(counter) + 0x8000;
    uint32_t bird = (megumin * SEGLEN) >> 16;
    SEGMENT.SetPixelColor(bird, SEGMENT.GetPaletteColour((i * 255)/ numBirds, PALETTE_SPAN_OFF, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
  }

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();

}
static const char PM_EFFECT_CONFIG__CHUNCHUN[] PROGMEM = "!,Gap size;!,!;!";

//each needs 12 bytes
//Spark type is used for popcorn and 1D fireworks
typedef struct Ball {
  unsigned long lastBounceTime;
  float impactVelocity;
  float height;
} ball;

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Bouncing Balls Effect
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Bouncing_Balls()
{
  //allocate segment data
  uint16_t maxNumBalls = 16; 
  uint16_t dataSize = sizeof(ball) * maxNumBalls;
  if (!SEGMENT.allocateData(dataSize)){return;}// return mode_static(); //allocation failed
  
  Ball* balls = reinterpret_cast<Ball*>(SEGMENT.data);
  
  // number of balls based on intensity() setting to max of 7 (cycles colors)
  // non-chosen color is a random color
  uint8_t numBalls = int(((SEGMENT.intensity() * (maxNumBalls - 0.8f)) / 255) + 1);
  
  float gravity                           = -9.81; // standard value of gravity
  float impactVelocityStart               = sqrt( -2 * gravity);

  unsigned long time = millis();

  if (SEGMENT.call == 0) {
    for (uint8_t i = 0; i < maxNumBalls; i++) balls[i].lastBounceTime = time;
  }
  
  bool hasCol2 = SEGCOLOR_U32(2);
  fill(hasCol2 ? BLACK : SEGCOLOR_U32(1));
  
  for (uint8_t i = 0; i < numBalls; i++) {
    float timeSinceLastBounce = (time - balls[i].lastBounceTime)/((255-SEGMENT.speed())*8/256 +1);
    balls[i].height = 0.5 * gravity * pow(timeSinceLastBounce/1000 , 2.0) + balls[i].impactVelocity * timeSinceLastBounce/1000;

    if (balls[i].height < 0) { //start bounce
      balls[i].height = 0;
      //damping for better effect using multiple balls
      float dampening = 0.90 - float(i)/pow(numBalls,2);
      balls[i].impactVelocity = dampening * balls[i].impactVelocity;
      balls[i].lastBounceTime = time;

      if (balls[i].impactVelocity < 0.015) {
        balls[i].impactVelocity = impactVelocityStart;
      }
    }
    
    uint32_t color = SEGCOLOR_U32(0);
    if (SEGMENT.palette.id) {
      color = color_wheel(i*(256/MAX(numBalls, 8)));
    } else if (hasCol2) {
      color = SEGCOLOR_U32(i % NUM_COLORS);
    }

    uint16_t pos = round(balls[i].height * (SEGLEN - 1));
    SEGMENT.SetPixelColor(pos, color);
  }

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__BOUNCINGBALLS[] PROGMEM = "Gravity,# of balls,,,,,,,Overlay;!,!,!;!;1;m12=1"; //bar



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Base_Sinelon(bool dual, bool rainbow)
{

  fade_out(SEGMENT.intensity(), SET_BRIGHTNESS);
  uint16_t pos = beatsin16(SEGMENT.speed()/10,0,SEGLEN-1);
  if (SEGMENT.call == 0) SEGMENT.params_internal.aux0 = pos;
  uint32_t color1 = SEGMENT.GetPaletteColour(pos, PALETTE_SPAN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32();

  uint32_t color2 = SEGCOLOR_U32(2);
  if (rainbow) {
    color1 = color_wheel((pos & 0x07) * 32);
  }
  SEGMENT.SetPixelColor(pos, color1);
  if (dual) {
    if (!color2) color2 = SEGMENT.GetPaletteColour(pos, PALETTE_SPAN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32();
    if (rainbow) color2 = color1; //rainbow
    SEGMENT.SetPixelColor(SEGLEN-1-pos, color2);
  }
  if (SEGMENT.params_internal.aux0 != pos) { 
    if (SEGMENT.params_internal.aux0 < pos) {
      for (uint16_t i = SEGMENT.params_internal.aux0; i < pos ; i++) {
        SEGMENT.SetPixelColor(i, color1);
        if (dual) SEGMENT.SetPixelColor(SEGLEN-1-i, color2);
      }
    } else {
      for (uint16_t i = SEGMENT.params_internal.aux0; i > pos ; i--) {
        SEGMENT.SetPixelColor(i, color1);
        if (dual) SEGMENT.SetPixelColor(SEGLEN-1-i, color2);
      }
    }
    SEGMENT.params_internal.aux0 = pos;
  }
  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}

void mAnimatorLight::EffectAnim__Sinelon(void) {
  return EffectAnim__Base_Sinelon(false);
}
static const char PM_EFFECT_CONFIG__SINELON[] PROGMEM = "!,Trail;!,!,!;!";

void mAnimatorLight::EffectAnim__Sinelon_Dual(void) {
  return EffectAnim__Base_Sinelon(true);
}
static const char PM_EFFECT_CONFIG__SINELON_DUAL[] PROGMEM = "!,Trail;!,!,!;!";

void mAnimatorLight::EffectAnim__Sinelon_Rainbow(void) {
  return EffectAnim__Base_Sinelon(true, true);
}
static const char PM_EFFECT_CONFIG__SINELON_RAINBOW[] PROGMEM = "!,Trail;,,!;!";



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Drip Effect
 * @note : Part of WLED Effects
 * @note : Drip Effect ported of: https://www.youtube.com/watch?v=sru2fXh4r7k

//values close to 100 produce 5Hz flicker, which looks very candle-y
//Inspired by https://github.com/avanhanegem/ArduinoCandleEffectNeoPixel
//and https://cpldcpu.wordpress.com/2016/01/05/reverse-engineering-a-real-candle/

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Drip()
{
  //allocate segment data
  uint16_t numDrops = 4; 
  uint16_t dataSize = sizeof(spark) * numDrops;
  if (!SEGMENT.allocateData(dataSize)) 
  {
    return;
   }
   // mode_static(); //allocation failed

  fill(SEGCOLOR_U32(1));
  
  Spark* drops = reinterpret_cast<Spark*>(SEGMENT.data);

  numDrops = 1 + (SEGMENT.intensity() >> 6);

  float gravity = -0.001 - (SEGMENT.speed()/50000.0);
  gravity *= SEGLEN;
  int sourcedrop = 12;

  for (int j=0;j<numDrops;j++) {
    if (drops[j].colIndex == 0) { //init
      drops[j].pos = SEGLEN-1;    // start at end
      drops[j].vel = 0;           // speed()
      drops[j].col = sourcedrop;  // brightness
      drops[j].colIndex = 1;      // drop state (0 init, 1 forming, 2 falling, 5 bouncing) 
    }
    
    SEGMENT.SetPixelColor(SEGLEN-1,ColourBlend(BLACK,SEGCOLOR_U32(0), sourcedrop));// water source
    if (drops[j].colIndex==1) {
      if (drops[j].col>255) drops[j].col=255;
      SEGMENT.SetPixelColor(int(drops[j].pos),ColourBlend(BLACK,SEGCOLOR_U32(0),drops[j].col));
      
      drops[j].col += map(SEGMENT.speed(), 0, 255, 1, 6); // swelling
      
      if (random8() < drops[j].col/10) {               // random drop
        drops[j].colIndex=2;               //fall
        drops[j].col=255;
      }
    }  
    if (drops[j].colIndex > 1) {           // falling
      if (drops[j].pos > 0) {              // fall until end of segment
        drops[j].pos += drops[j].vel;
        if (drops[j].pos < 0) drops[j].pos = 0;
        drops[j].vel += gravity;

        for (int i=1;i<7-drops[j].colIndex;i++) { // some minor math so we don't expand bouncing droplets
          SEGMENT.SetPixelColor(int(drops[j].pos)+i,ColourBlend(BLACK,SEGCOLOR_U32(0),drops[j].col/i)); //spread pixel with fade while falling
        }
        
        if (drops[j].colIndex > 2) {       // during bounce, some water is on the floor
          SEGMENT.SetPixelColor(0,ColourBlend(SEGCOLOR_U32(0),BLACK,drops[j].col));
        }
      } else {                             // we hit bottom
        if (drops[j].colIndex > 2) {       // already hit once, so back to forming
          drops[j].colIndex = 0;
          drops[j].col = sourcedrop;
          
        } else {

          if (drops[j].colIndex==2) {      // init bounce
            drops[j].vel = -drops[j].vel/4;// reverse velocity with damping 
            drops[j].pos += drops[j].vel;
          } 
          drops[j].col = sourcedrop*2;
          drops[j].colIndex = 5;           // bouncing
        }
      }
    }
  }

  SEGMENT.transition.rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__DRIP[] PROGMEM = "Gravity,# of drips,,,,,,,Overlay;!,!;!;;m12=1"; //bar
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Test case used for developing new animations
 * @note : Shows pixels from palette, in order. Gradients can either be displayed over total length of segment, or repeated by X pixels
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::SubTask_Flasher_Animate_Function_Tester_01()
{


 SEGMENT.palette_container->CRGB16Palette16_Palette.data = Test_p;
 pCONT_lAni->_virtualSegmentLength = 50;
 pCONT_lAni->paletteBlend = 3;
 
uint8_t index = 15;

uint8_t paletteIndex = 0; 

CRGB colour;
      uint8_t pbri = 255;

 uint16_t j = 0;
 uint16_t i = 0;
    pCONT_lAni->SEGMENT.SetPixelColor(j, mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette.id, i, nullptr, true, false, 10));
  for(uint16_t i = 0; i < 50; i++) {
    
    j = i;//map(i,0,50,0,255);
  //  pCONT_lAni->SEGMENT.SetPixelColor(j, mPaletteI->color_from_palette_internal(0, i, true, true, 10));
    // pCONT_lAni->SEGMENT.SetPixelColor(i, mPaletteI->color_from_palette_internal(i, true, true, 10));
  index = i;
  
paletteIndex = i*((255/16)-1);//map(i,);//((255/16)*index)-1; 
colour = ColorFromPalette( Test_p, paletteIndex, pbri, NOBLEND);

uint32_t col = colour.r*65536 +  colour.g*256 +  colour.b;


    pCONT_lAni->SEGMENT.SetPixelColor(i, col);
  
  }
    // pCONT_lAni->SEGMENT.SetPixelColor(0, RgbColor(255,0,0));
  SEGMENT.transition.rate_ms = FRAMETIME_MS;

// CRGB colour;
//  = ColorFromPalette( Test_p, paletteIndex, pbri, NOBLEND);
// AddLog(LOG_LEVEL_TEST, PSTR("colour %d %d,%d,%d"),paletteIndex,colour.r,colour.g,colour.b);







  // uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type) * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  // //AddLog(LOG_LEVEL_TEST, PSTR("dataSize = %d"), dataSize);

  // if (!SEGMENT.allocateData(dataSize))
  // {
  //   AddLog(LOG_LEVEL_TEST, PSTR("Not Enough Memory"));
  //   SEGMENT.effect_id = EFFECTS_FUNCTION_STATIC_PALETTE_ID; // Default
  // }
  
  
  // // Pick new colours
  // DynamicBuffer_Segments_UpdateDesiredColourFromPaletteSelected(SEGMENT.palette.id, SEGIDX);
  
  // // Get starting positions already on show
  // DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // // Call the animator to blend from previous to new
  // SetSegment_AnimFunctionCallback(  SEGIDX, 
  //   [this](const AnimationParam& param){ 
  //     this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
  //   }
  // );


  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  

}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Test case used for developing new animations
 * @note : Shows pixels from palette, in order. Gradients can either be displayed over total length of segment, or repeated by X pixels
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 * Currently using to test all palettes as they are converted into one method
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::SubTask_Flasher_Animate_Function_Tester_02()
{


  ALOG_INF( PSTR("SubTask_Flasher_Animate_Function_Tester_02") );


  // SEGMENT.palette_container->CRGB16Palette16_Palette.data = Test_p;
  // pCONT_lAni->_virtualSegmentLength = 50;
  // pCONT_lAni->paletteBlend = 3;

  // uint8_t index = 15;

  // uint8_t paletteIndex = 0; 

  // CRGB colour;
  // uint8_t pbri = 255;

  // uint16_t j = 0;
  // uint16_t i = 0;
  // pCONT_lAni->SEGMENT.SetPixelColor(j, mPaletteI->color_from_pale tte_Intermediate(i, true, false, 10));
  // for(uint16_t i = 0; i < 50; i++) {

  //   j = i;//map(i,0,50,0,255);
  //   //  pCONT_lAni->SEGMENT.SetPixelColor(j, mPaletteI->color_from_palette_internal(0, i, true, true, 10));
  //   // pCONT_lAni->SEGMENT.SetPixelColor(i, mPaletteI->color_from_palette_internal(i, true, true, 10));
  //   index = i;

  //   paletteIndex = i*((255/16)-1);//map(i,);//((255/16)*index)-1; 
  //   colour = ColorFromPalette( Test_p, paletteIndex, pbri, NOBLEND);

  //   uint32_t col = colour.r*65536 +  colour.g*256 +  colour.b;


  //   pCONT_lAni->SEGMENT.SetPixelColor(i, col, true);

  // }

  uint8_t colours_in_palette = 0;

  RgbcctColor colour = mPaletteI->GetColourFromPaletteAdvanced(pCONT_lAni->SEGMENT_I(0).palette.id,0,nullptr,true,true,false,255,&colours_in_palette);

  // RgbcctColor colour = mPaletteI->GetColourFromPaletteAdvanced(mPaletteI->PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY_TERTIARY__ID,0,nullptr,true,true,255,&colours_in_palette);

  // SEGMENT.palette.id = mPaletteI->PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY_TERTIARY__ID;

  ALOG_INF( PSTR("pID{%d}, colours_in_palette = %d"), pCONT_lAni->SEGMENT_I(0).palette.id, colours_in_palette );

  // SEGMENT.SetPixelColor(0,colour,true);
//  for(int i=0;i<50;i++)
//   {
//     SEGMENT.SetPixelColor(i, RgbColor(0));
//   }

  for(int i=0;i<256;i++)
  {

    colour = mPaletteI->GetColourFromPaletteAdvanced(pCONT_lAni->SEGMENT_I(0).palette.id,i,nullptr,true,true,true,255,&colours_in_palette);

    SEGMENT.SetPixelColor(i, colour, true);

    // ALOG_INF( PSTR("pID{%d}, colours_in_palette = %d"),pCONT_lAni->SEGMENT_I(0).palette.id, colours_in_palette );

    // if(i>colours_in_palette)
    // {
    //   break;
    // }


  }



    // pCONT_lAni->SEGMENT.SetPixelColor(0, RgbColor(255,0,0));
  // SEGMENT.transition.rate_ms = FRAMETIME_MS;

// CRGB colour;
//  = ColorFromPalette( Test_p, paletteIndex, pbri, NOBLEND);
// AddLog(LOG_LEVEL_TEST, PSTR("colour %d %d,%d,%d"),paletteIndex,colour.r,colour.g,colour.b);


// DEBUG_LINE_HERE;
#ifdef ENABLE_DEVFEATURE_DEBUG_SERIAL__ANIMATION_OUTPUT
          Serial.print("@1");
          #endif 
pCONT_iLight->ShowInterface();




  // uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type) * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  // //AddLog(LOG_LEVEL_TEST, PSTR("dataSize = %d"), dataSize);

  // if (!SEGMENT.allocateData(dataSize))
  // {
  //   AddLog(LOG_LEVEL_TEST, PSTR("Not Enough Memory"));
  //   SEGMENT.effect_id = EFFECTS_FUNCTION_STATIC_PALETTE_ID; // Default
  // }
  
  
  // // Pick new colours
  // DynamicBuffer_Segments_UpdateDesiredColourFromPaletteSelected(SEGMENT.palette.id, SEGIDX);
  
  // // Get starting positions already on show
  // DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // // Call the animator to blend from previous to new
  // SetSegment_AnimFunctionCallback(  SEGIDX, 
  //   [this](const AnimationParam& param){ 
  //     this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
  //   }
  // );


  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING


#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Notification style
 * @note : 
 *  
 * params_user
 *  - val0: period of blink (total time between new flashes)
 *  - val1: desired number of repeats
 *  - val2: percentage duty cycle, 0-100% is used to split ON and OFF half-cycle... blink/pulse mode may only differ depending on the animation time to new value, maybe merge modes when both work by themselves
 * 
 * 
 * fade on/off: time_ms is period, rate_ms is period, repeats or not, 
 * blink on/off: time_ms is 0, rate_ms is split_period
 * pulse on/off: time_ms is split period, rate_ms is split_period
 * 
 * 
 * << Example JSON Command >>
 
  {
    "Effects": {
      "Function": 22,
      "Intensity": 50,
      "Params": [
        1000,
        10,
        50
      ]
    },
    "BrightnessRGB": 100,
    "SegColour0": {
      "Hue": 120,
      "Sat": 100,
      "BrightnessRGB": 100
    }
  }

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::SubTask_Segment_Animate_Function__Notification_Static()
{
  SubTask_Segment_Animate_Function__Notification_Base(true, false, false); 
}
void mAnimatorLight::SubTask_Segment_Animate_Function__Notification_Blinking()
{
  SubTask_Segment_Animate_Function__Notification_Base(false, true, false); 
}
void mAnimatorLight::SubTask_Segment_Animate_Function__Notification_Fade()
{
  SubTask_Segment_Animate_Function__Notification_Base(false, false, true); 
}
void mAnimatorLight::SubTask_Segment_Animate_Function__Notification_Pulsing()
{
  SubTask_Segment_Animate_Function__Notification_Base(false, true, true); 
}
void mAnimatorLight::SubTask_Segment_Animate_Function__Notification_Base(bool flag_static, bool flag_alternate_onoff, bool flag_blend)
{

  if (!SEGMENT.allocateData( GetSizeOfPixel(SEGMENT.colour_type)*2) ){ return; }

  uint16_t   period_ms            = SEGMENT.params_user.val0;
  uint16_t*  repeats_desired      = &SEGMENT.params_user.val1;
  uint16_t*  duty_cycle_desired   = &SEGMENT.params_user.val2;
  uint16_t*  auto_seconds_timeout = &SEGMENT.params_user.val3;

  // ALOG_INF(PSTR("duty_cycle_desired=%d"),*duty_cycle_desired);
  ALOG_INF(PSTR(DEBUG_INSERT_PAGE_BREAK "SubTask_Segment_Animate_Function__Notification_Base %d of %d"),getCurrSegmentId(), segments.size());
  

  // if(getCurrSegmentId()==1){delay(5000);}
  
  uint16_t* repeat_counter_p        = &SEGMENT.params_internal.aux0;
  uint16_t* blink_state_p           = &SEGMENT.params_internal.aux1;
  uint16_t* blink_counter_p         = &SEGMENT.params_internal.aux2;
  uint16_t* autotimeout_last_millis = &SEGMENT.params_internal.aux3; // cant use tSaved as its U16, so just compare against previous millis

  RgbcctColor starting_colour     = RgbcctColor(0);
  RgbcctColor desired_colour      = SEGCOLOR_RGBCCT(0);
  bool        flag_set_animator   = false;

  /********************************************************************************************************
   * @brief Static
   ********************************************************************************************************/

  if(flag_static)
  {
    flag_set_animator = true;
  }

  // Default update rates
  SEGMENT.transition.time_ms     = 0;  
  SEGMENT.transition.rate_ms     = period_ms+FRAMETIME_MS;
  SEGMENT.tSaved_AnimateRunTime -= period_ms; // force into past to make it happen now, temp solution

  /********************************************************************************************************
   * @brief Blinking and Pulsing
   ********************************************************************************************************/

  // Alternate between primary and secondary colours as targets
  if(flag_alternate_onoff)
  {
    if(*blink_state_p)
    {
      starting_colour = RgbcctColor(0);
      desired_colour  = SEGCOLOR_RGBCCT(0);
      *blink_state_p = 0;
    }
    else
    {
      starting_colour = SEGCOLOR_RGBCCT(0);
      desired_colour  = RgbcctColor(0);
      *blink_state_p = 1;
    }
  }else{
    starting_colour = RgbcctColor(0);
    desired_colour  = SEGCOLOR_RGBCCT(0);
  }

  /**
   * @brief 
   * Assumes aux values where reset on JsonCommand
   **/
  if(*repeat_counter_p < *repeats_desired)
  {
    
    ALOG_DBM( PSTR("*repeat_counter_p %d < %d *repeats_desired"), *repeat_counter_p, *repeats_desired );

    /**
     * @brief Rough for now
     **/
    if(flag_alternate_onoff)
    {

      SEGMENT.tSaved_AnimateRunTime -= period_ms; // force into past to make it happen now, temp solution
      
      if(*duty_cycle_desired==0)
      {
        SEGMENT.transition.rate_ms     = period_ms/2;
        *repeat_counter_p += 1;
      }else{

        uint16_t duty_blink_t      = constrain(*duty_cycle_desired,0,100);
        float duty_blink_on_ratio  = duty_blink_t/100.0f;
        float duty_blink_off_ratio = (100-duty_blink_t)/100.0f;

        if(*blink_state_p == 1) // ON
        {
          SEGMENT.transition.rate_ms     = period_ms*duty_blink_on_ratio;
          *repeat_counter_p += 1;
        }else{
          SEGMENT.transition.rate_ms     = period_ms*duty_blink_off_ratio;
        }
        
        ALOG_DBM( PSTR("duty_blink %d/%d -> %d"), (uint16_t)period_ms*duty_blink_on_ratio, (uint16_t)period_ms*duty_blink_off_ratio, SEGMENT.transition.rate_ms);
      }

    }else{
      *repeat_counter_p += 1;
    }

    /**
     * @brief Blend time can only be set after "rate" is calculated
     **/
    if(flag_blend){
      SEGMENT.transition.time_ms     = SEGMENT.transition.rate_ms;   
    }else{
      SEGMENT.transition.time_ms     = 0; 
    }

    flag_set_animator = true;
    ALOG_DBM( PSTR("*repeat_counter_p %d < %d *repeats_desired"), *repeat_counter_p, *repeats_desired );
  }
  // If exceeded repeat counter, then reduce update rate
  else
  {
    ALOG_DBM( PSTR("*repeat_counter_p %d < %d *repeats_desired DONE"), *repeat_counter_p, *repeats_desired );
    SEGMENT.transition.rate_ms = 1000; // Reduce refresh rate
    SEGMENT.transition.time_ms = 0;
  }

  // Check timeout
  if((millis()-*autotimeout_last_millis)>1000) // 1 second tick
  {
    *autotimeout_last_millis = (uint16_t)(millis()); // update for next call
    if(*auto_seconds_timeout>1)
    {
      *auto_seconds_timeout -= 1;
      ALOG_DBM( PSTR("auto_seconds_timeout = %d, %d"), *autotimeout_last_millis, *auto_seconds_timeout );
    }else
    if(*auto_seconds_timeout==1)
    {
      ALOG_DBM( PSTR("auto_seconds_timeout = %d, %d   OFF"), *autotimeout_last_millis, *auto_seconds_timeout );
      desired_colour = RgbcctColor(0); // off
      SEGMENT.transition.time_ms = 500;
      SEGMENT.transition.rate_ms = 1000;
    }
  }

  if(flag_set_animator)
  {
    desired_colour.debug_print("desired_colour");
    starting_colour.debug_print("starting_colour");

    
    // ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K("effect >"D_JSON_HUE)), desired_colour.getHue360());


    // Serial.println(desired_colour.raw[0]);
    // Serial.println(desired_colour.raw[1]);
    // Serial.println(desired_colour.raw[2]);
    // Serial.println(desired_colour.raw[3]);
    // Serial.println(desired_colour.raw[4]);

    SetTransitionColourBuffer_DesiredColour (SEGMENT.Data(), SEGMENT.DataLength(), 0, SEGMENT.colour_type, desired_colour); 
    SetTransitionColourBuffer_StartingColour(SEGMENT.Data(), SEGMENT.DataLength(), 0, SEGMENT.colour_type, starting_colour);

    SetSegment_AnimFunctionCallback( SEGIDX, [this](const AnimationParam& param){ this->AnimationProcess_SingleColour_LinearBlend_Dynamic_Buffer(param); } );

  }

};

#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Box Gradient: Effect 1 (two edges are solid, with blending along the other edges. the solid/not edges are selectable via Val0)
 * @note : Uses a segments four RGBCCT colours to describe the four corners of a rear/edge lit backlight (e.g. a gradient ambilight)
 * 
 * @param aux0 
 * @param aux1 
 * @param aux2 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
void mAnimatorLight::SubTask_Segment_Animate_Function__BoxEdge_TwoColour_Gradient()
{
  
}
void mAnimatorLight::SubTask_Segment_Animate_Function__BoxEdge_FourColour_Gradient()
{
  
}
void mAnimatorLight::SubTask_Segment_Animate_Function__BoxEdge_FourColour_Solid() // No gradient
{
  
}
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS

// /*******************************************************************************************************************
// ********************************************************************************************************************
// ************ START OF AMBILIGHT DEFINITIONS ********************************************************************************************
// ********************************************************************************************************************
// ********************************************************************************************************************/
/**
 * @brief
 * Uses a segments four RGBCCT colours to describe the four corners of a rear/edge lit backlight (e.g. a gradient ambilight)
 **/

// // Limit ambilight to addressible type, else I will just use "scene"
// void mAnimatorLight::init_Ambilight(){

//   ambilightsettings.screens[SCREEN_CENTRE].top.colour    = HsbColor(pCONT_iLight->HUE_N2F(20),pCONT_iLight->SatN2F(95),pCONT_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].bottom.colour = HsbColor(pCONT_iLight->HUE_N2F(8),pCONT_iLight->SatN2F(95),pCONT_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].left.colour   = HsbColor(pCONT_iLight->HUE_N2F(240),pCONT_iLight->SatN2F(100),pCONT_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].right.colour  = HsbColor(pCONT_iLight->HUE_N2F(330),pCONT_iLight->SatN2F(100),pCONT_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].top.size = 33;
//   ambilightsettings.screens[SCREEN_CENTRE].bottom.size = 33;
//   ambilightsettings.screens[SCREEN_CENTRE].left.size = 19;
//   ambilightsettings.screens[SCREEN_CENTRE].right.size = 19;
//   ambilightsettings.screens[SCREEN_CENTRE].left.blend_between_sides_gradient_percentage = 50;

//   ambilightsettings.screens[SCREEN_CENTRE].top.colour    = HsbColor(pCONT_iLight->HUE_N2F(20),pCONT_iLight->SatN2F(95),pCONT_iLight->BrtN2F(0));
//   ambilightsettings.screens[SCREEN_CENTRE].bottom.colour    = HsbColor(pCONT_iLight->HUE_N2F(20),pCONT_iLight->SatN2F(95),pCONT_iLight->BrtN2F(50));



//   #ifdef   DEVICE_RGB_COMPUTER_SCREEN_DELL_P3222QE
  
//   ambilightsettings.screens[SCREEN_CENTRE].top.colour    = RgbcctColor(255,175,0,255,0);//HsbColor(pCONT_iLight->HUE_N2F(240),pCONT_iLight->SatN2F(100),pCONT_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].bottom.colour = RgbcctColor(0,0,0,100,0);//HsbColor(pCONT_iLight->HUE_N2F(0),pCONT_iLight->SatN2F(100),pCONT_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].left.colour   = HsbColor(pCONT_iLight->HUE_N2F(340),pCONT_iLight->SatN2F(100),pCONT_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].right.colour  = HsbColor(pCONT_iLight->HUE_N2F(120),pCONT_iLight->SatN2F(100),pCONT_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].top.size = 42;
//   ambilightsettings.screens[SCREEN_CENTRE].bottom.size = 44; // 2 extra pixels on centre inlay
//   ambilightsettings.screens[SCREEN_CENTRE].left.size = 23;
//   ambilightsettings.screens[SCREEN_CENTRE].right.size = 23;
//   ambilightsettings.screens[SCREEN_CENTRE].left.blend_between_sides_gradient_percentage = 0;
//   ambilightsettings.screens[SCREEN_CENTRE].right.blend_between_sides_gradient_percentage = 0;


//   #endif // DEVICE_RGB_COMPUTER_SCREEN_DELL_P3222QE

//   #ifdef DEVICE_RGB_COMPUTER_SCREEN_DELL_U2515H
//   ambilightsettings.screens[SCREEN_CENTRE].top.colour    = HsbColor(pCONT_iLight->HUE_N2F(20),pCONT_iLight->SatN2F(95),pCONT_iLight->BrtN2F(0));
//   ambilightsettings.screens[SCREEN_CENTRE].bottom.colour = HsbColor(pCONT_iLight->HUE_N2F(8),pCONT_iLight->SatN2F(95),pCONT_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].left.colour   = HsbColor(pCONT_iLight->HUE_N2F(240),pCONT_iLight->SatN2F(100),pCONT_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].right.colour  = HsbColor(pCONT_iLight->HUE_N2F(330),pCONT_iLight->SatN2F(100),pCONT_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].top.size = 33;
//   ambilightsettings.screens[SCREEN_CENTRE].bottom.size = 33;
//   ambilightsettings.screens[SCREEN_CENTRE].left.size = 19;
//   ambilightsettings.screens[SCREEN_CENTRE].right.size = 19;
//   ambilightsettings.screens[SCREEN_CENTRE].left.blend_between_sides_gradient_percentage = 50;

//   ambilightsettings.screens[SCREEN_CENTRE].top.colour    = HsbColor(pCONT_iLight->HUE_N2F(20),pCONT_iLight->SatN2F(95),pCONT_iLight->BrtN2F(0));
//   ambilightsettings.screens[SCREEN_CENTRE].bottom.colour    = HsbColor(pCONT_iLight->HUE_N2F(20),pCONT_iLight->SatN2F(95),pCONT_iLight->BrtN2F(50));




//   #endif // DEVICE_RGB_COMPUTER_SCREEN_DELL_U2515H
  

// }

// void mAnimatorLight::SubTask_Ambilight_Main(){

//   // Add mode to allow orientations, for when screens rotate so they respect top/bottom

//   // if(abs(millis()-ambilightsettings.tSavedUpdate)>ambilightsettings.ratemsSavedUpdate){ambilightsettings.tSavedUpdate=millis();
//   //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "ambilight"));
//   // }

  
//   // ambilightsettings.screens[SCREEN_CENTRE].top.colour    = RgbColor(255,0,0);
//   // ambilightsettings.screens[SCREEN_CENTRE].bottom.colour = RgbColor(0,255,0); 
//   // ambilightsettings.screens[SCREEN_CENTRE].left.colour   = RgbColor(0,0,255);
//   // ambilightsettings.screens[SCREEN_CENTRE].right.colour  = RgbColor(255,0,255);


//   //switch modes : USE presets
//   switch(ambilightsettings.ambilight_mode){
//     case AMBILIGHT_PRESETS_ID:

//     break;
//     case AMBILIGHT_SIDES_ID: // IE DELL of dual tone from the past
//       // not even splits, setting split point (ie bottom edge only 0%, 25% way up sides, half way 50%)
//       if(abs(millis()-ambilightsettings.tSavedUpdate)>ambilightsettings.ratemsSavedUpdate){ambilightsettings.tSavedUpdate=millis();
//         AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "ambilight"));
//         Ambilight_Sides();
//         StartAnimationAsBlendFromStartingColorToDesiredColor();
//       }
//     break;
//     case AMBILIGHT_INPUT_STREAM_ID:{
//       //serial input
//     }break;
//   }

// } // END function




// void mAnimatorLight::Ambilight_Sides(){
//   #ifdef ENABLE_LOG_LEVEL_DEBUG
//   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::Ambilight_Sides()"));
//   #endif


//   #ifdef USE_DEVFEATURE_PIXEL0_BOTTOM_RIGHT

//   float progress;
//   RgbcctColor colour_tmp;

//   uint8_t bottom_size = ambilightsettings.screens[SCREEN_CENTRE].bottom.size;
//   uint8_t bottom_start = 0;
//   for(int bottom=0;bottom<bottom_size;bottom++){
//     animation_colours[bottom_start+bottom].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].bottom.colour;
//   }

//   uint8_t left_size = ambilightsettings.screens[SCREEN_CENTRE].left.size;
//   uint8_t left_start = 33;
//   for(int left=0;left<left_size;left++){
//     //if(ambilightsettings.screens[SCREEN_CENTRE].left.blend_between_sides_gradient_percentage>=0){ //if set
//       progress = (float)(left)/(float)(left_size);
//       colour_tmp = RgbcctColor::LinearBlend(RgbcctColor(ambilightsettings.screens[SCREEN_CENTRE].bottom.colour),
//                                          RgbcctColor(ambilightsettings.screens[SCREEN_CENTRE].top.colour),
//                                          progress);
//     //}
//     animation_colours[left_start+left].DesiredColour = colour_tmp;//ambilightsettings.screens[SCREEN_CENTRE].left.colour;
//   }

//   uint8_t top_size = ambilightsettings.screens[SCREEN_CENTRE].top.size;
//   uint8_t top_start = 52;
//   for(int top=0;top<top_size;top++){
//     animation_colours[top_start+top].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].top.colour;
//   }

//   uint8_t right_size = ambilightsettings.screens[SCREEN_CENTRE].right.size;
//   uint8_t right_start = 85;
//   for(int right=0;right<right_size;right++){
//     progress = (float)(right)/(float)(right_size);
//     colour_tmp = RgbcctColor::LinearBlend(RgbcctColor(ambilightsettings.screens[SCREEN_CENTRE].top.colour),
//                                        RgbcctColor(ambilightsettings.screens[SCREEN_CENTRE].bottom.colour),
//                                        progress);
//     animation_colours[right_start+right].DesiredColour = colour_tmp;
//   }

  
//      // #ifndef ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS
//       for(int i=0;i<STRIP_SIZE_MAX;i++)
//       {
//         // if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
//           // animation_colours[i].DesiredColour = ApplyBrightnesstoDesiredColourWithGamma(animation_colours[i].DesiredColour, pCONT_iLight->getBriRGB_Global());

// animation_colours[i].DesiredColour = ApplyBrightnesstoRgbcctColour(animation_colours[i].DesiredColour, pCONT_iLight->getBriRGB_Global(), pCONT_iLight->getBriCCT_Global());

//         // }
//       }
//  //     #endif



//   #endif

  
//   #ifdef USE_DEVFEATURE_PIXEL0_BOTTOM_LEFT_ANTICLOCKWISE_TO_BE_FEATURE_OPTION


//   switch(1)
//   {
//     case 0: // method 4 different sides
//     { 

//       float progress;
//       RgbcctColor colour_tmp;

//       uint8_t bottom_size = ambilightsettings.screens[SCREEN_CENTRE].bottom.size;
//       uint8_t bottom_start = 0;
//       for(int bottom=0;bottom<bottom_size;bottom++){
//         animation_colours[bottom_start+bottom].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].bottom.colour;
//       }

//       uint8_t right_size = ambilightsettings.screens[SCREEN_CENTRE].right.size;
//       uint8_t right_start = ambilightsettings.screens[SCREEN_CENTRE].bottom.size;
//       for(int right=0;right<right_size;right++){
//         //if(ambilightsettings.screens[SCREEN_CENTRE].right.blend_between_sides_gradient_percentage>=0){ //if set
//           // progress = (float)(right)/(float)(right_size);
//           // colour_tmp = RgbColor::LinearBlend(RgbColor(ambilightsettings.screens[SCREEN_CENTRE].bottom.colour),
//           //                                    RgbColor(ambilightsettings.screens[SCREEN_CENTRE].top.colour),
//           //                                    progress);
//         //}
//         // animation_colours[right_start+right].DesiredColour = colour_tmp;//ambilightsettings.screens[SCREEN_CENTRE].right.colour;
//         animation_colours[right_start+right].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].right.colour;
//       }

//       uint8_t top_size = ambilightsettings.screens[SCREEN_CENTRE].top.size;
//       uint8_t top_start = ambilightsettings.screens[SCREEN_CENTRE].bottom.size+ambilightsettings.screens[SCREEN_CENTRE].right.size;
//       for(int top=0;top<top_size;top++){
//         animation_colours[top_start+top].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].top.colour;
//       }

//       uint8_t left_size = ambilightsettings.screens[SCREEN_CENTRE].left.size;
//       uint8_t left_start = top_start + top_size;
//       for(int left=0;left<left_size;left++){
//         // progress = (float)(left)/(float)(left_size);
//         // colour_tmp = RgbColor::LinearBlend(RgbColor(ambilightsettings.screens[SCREEN_CENTRE].top.colour),
//         //                                    RgbColor(ambilightsettings.screens[SCREEN_CENTRE].bottom.colour),
//         //                                    progress);
//         // animation_colours[left_start+left].DesiredColour = colour_tmp;
//         animation_colours[left_start+left].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].left.colour;;
//       }

//       // animation_colours[0].DesiredColour = RgbColor(255,0,0);
//       // animation_colours[43].DesiredColour = RgbColor(255,0,0);

//       // animation_colours[44].DesiredColour = RgbColor(0,255,0);
//       // animation_colours[66].DesiredColour = RgbColor(0,255,0);

//       // animation_colours[67].DesiredColour = RgbColor(0,0,255);
//       // animation_colours[108].DesiredColour = RgbColor(0,0,255);

//       // animation_colours[109].DesiredColour = RgbwColor(0,0,0,255);
//       // animation_colours[130].DesiredColour = RgbwColor(0,0,0,255);
      
//       // animation_colours[131].DesiredColour = RgbwColor(255,0,0,255);
//       // animation_colours[132].DesiredColour = RgbwColor(0,255,0,255);
//       // animation_colours[133].DesiredColour = RgbwColor(0,0,255,255);
//       // animation_colours[131].DesiredColour = RgbwColor(255,0,255,255);
      
//       }
//       break;
//       case 1: //blend top and bottom colour only, sides are linear
//       {


//       float progress;
//       RgbcctColor colour_tmp;

//       uint8_t bottom_size = ambilightsettings.screens[SCREEN_CENTRE].bottom.size;
//       uint8_t bottom_start = 0;
//       for(int bottom=0;bottom<bottom_size;bottom++){
//         animation_colours[bottom_start+bottom].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].bottom.colour;
//       }

//       uint8_t right_size = ambilightsettings.screens[SCREEN_CENTRE].right.size;
//       uint8_t right_start = ambilightsettings.screens[SCREEN_CENTRE].bottom.size;
//       for(int right=0;right<right_size;right++){
//         if(ambilightsettings.screens[SCREEN_CENTRE].right.blend_between_sides_gradient_percentage>=0){ //if set
//           progress = (float)(right)/(float)(right_size);
//           colour_tmp = RgbcctColor::LinearBlend(RgbcctColor(ambilightsettings.screens[SCREEN_CENTRE].bottom.colour),
//                                              RgbcctColor(ambilightsettings.screens[SCREEN_CENTRE].top.colour),
//                                              progress);
//         }
//         animation_colours[right_start+right].DesiredColour = colour_tmp;//ambilightsettings.screens[SCREEN_CENTRE].right.colour;
//         // animation_colours[right_start+right].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].right.colour;
//       }

//       uint8_t top_size = ambilightsettings.screens[SCREEN_CENTRE].top.size;
//       uint8_t top_start = ambilightsettings.screens[SCREEN_CENTRE].bottom.size+ambilightsettings.screens[SCREEN_CENTRE].right.size;
//       for(int top=0;top<top_size;top++){
//         animation_colours[top_start+top].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].top.colour;
//       }

//       uint8_t left_size = ambilightsettings.screens[SCREEN_CENTRE].left.size;
//       uint8_t left_start = top_start + top_size;
//       for(int left=0;left<left_size;left++){
//         progress = (float)(left)/(float)(left_size);
//         colour_tmp = RgbcctColor::LinearBlend(RgbcctColor(ambilightsettings.screens[SCREEN_CENTRE].top.colour),
//                                            RgbcctColor(ambilightsettings.screens[SCREEN_CENTRE].bottom.colour),
//                                            progress);
//         animation_colours[left_start+left].DesiredColour = colour_tmp;
//         // animation_colours[left_start+left].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].left.colour;;
//       }

//       #ifndef ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS
//       for(int i=0;i<STRIP_SIZE_MAX;i++)
//       {
//         // if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
//           // animation_colours[i].DesiredColour = ApplyBrightnesstoDesiredColourWithGamma(animation_colours[i].DesiredColour, pCONT_iLight->getBriRGB_Global());

// animation_colours[i].DesiredColour = ApplyBrightnesstoRgbcctColour(animation_colours[i].DesiredColour, pCONT_iLight->getBriRGB_Global(), pCONT_iLight->getBriCCT_Global());

//         // }
//       }
//       #endif

//       }
//       break;
//   }//end switch

//   #endif



// }


// // void mAnimatorLight::parsesub_ModeAmbilight(){

// //    // Create local dereferenced variable
// //   // JsonObject obj = (*_obj); 

// //   int8_t tmp_id = 0;
  

// //   char buffer[50];

// // // // #ifndef ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT

// // //   if(!obj[D_JSON_NAME].isNull()){ 
// // //     const char* scenectr = obj[D_JSON_NAME];
// // //     if((tmp_id=GetSceneIDbyName(scenectr))>=0){
// // //       scene.name_id = tmp_id;
// // //       pCONT_iLight->animation.mode_id = ANIMATION_MODE_SCENE_ID;
// // //       AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_NAME,GetSceneName(buffer));
// // //       // Response_mP(S_JSON_COMMAND_SVALUE,D_JSON_NAME,GetSceneName(buffer));
// // //       data_buffer.isserviced++;
// // //     }else{
// // //       AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_NAME,scenectr);
// // //     }
// // //   }

// // //   // #endif //ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT

// // //   // USe pointers
// // //   //side
// // //   // struct AMBILIGHT_SCREEN_SETTINGS.SCREENS[0]::
// // //   // ambilightsettings.screens[SCREEN_CENTRE].top
// // //   //screen
  
// // //   if(!obj[F("top")][F(D_JSON_HUE)].isNull()){ 
// // //     uint16_t hue = obj[F("top")][F(D_JSON_HUE)];
// // //     // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_HUE,hue);
// // //     ambilightsettings.screens[SCREEN_CENTRE].top.colour.H = pCONT_iLight->HUE_N2F(hue);
// // //     // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_HUE,ambilightsettings.screens[SCREEN_CENTRE].top.colour.H);
// // //     // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_HUE,ambilightsettings.screens[SCREEN_CENTRE].top.colour.H);
// // //     data_buffer.isserviced++;
// // //   }
// // //   if(!obj[F("top")][F(D_JSON_SAT)].isNull()){ 
// // //     uint8_t sat = obj[F("top")][F(D_JSON_SAT)];
// // //     // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_SAT,sat);
// // //     ambilightsettings.screens[SCREEN_CENTRE].top.colour.S = pCONT_iLight->SatN2F(sat);
// // //     // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_SAT,ambilightsettings.screens[SCREEN_CENTRE].top.colour.S);
// // //     // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_SAT,ambilightsettings.screens[SCREEN_CENTRE].top.colour.S);
// // //     data_buffer.isserviced++;
// // //   }
// // //   if(!obj[F("top")][F(D_JSON_BRT)].isNull()){ 
// // //     uint8_t brt = obj[F("top")][F(D_JSON_BRT)];
// // //     // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_BRT,brt);
// // //     ambilightsettings.screens[SCREEN_CENTRE].top.colour.B = pCONT_iLight->animation.brightness = pCONT_iLight->BrtN2F(brt);
// // //     // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_BRT,pCONT_iLight->animation.brightness);
// // //     // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_BRT,pCONT_iLight->animation.brightness);
// // //     data_buffer.isserviced++;
// // //   }



// // //   if(!obj[F("bottom")][F(D_JSON_HUE)].isNull()){ 
// // //     uint16_t hue = obj[F("bottom")][F(D_JSON_HUE)];
// // //     // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_HUE,hue);
// // //     ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.H = pCONT_iLight->HUE_N2F(hue);
// // //     // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_HUE,ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.H);
// // //     // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_HUE,ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.H);
// // //     data_buffer.isserviced++;
// // //   }
// // //   if(!obj[F("bottom")][F(D_JSON_SAT)].isNull()){ 
// // //     uint8_t sat = obj[F("bottom")][F(D_JSON_SAT)];
// // //     // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_SAT,sat);
// // //     ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.S = pCONT_iLight->SatN2F(sat);
// // //     // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_SAT,ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.S);
// // //     // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_SAT,ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.S);
// // //     data_buffer.isserviced++;
// // //   }
// // //   if(!obj[F("bottom")][F(D_JSON_BRT)].isNull()){ 
// // //     uint8_t brt = obj[F("bottom")][F(D_JSON_BRT)];
// // //     // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_BRT,brt);
// // //     ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.B = pCONT_iLight->animation.brightness = pCONT_iLight->BrtN2F(brt);
// // //     // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_BRT,pCONT_iLight->animation.brightness);
// // //     // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_BRT,pCONT_iLight->animation.brightness);
// // //     data_buffer.isserviced++;
// // //   }




// // //   if(!obj[D_JSON_RGB].isNull()){
// // //     const char* rgbpacked = obj[D_JSON_RGB];
// // //     uint32_t colour32bit = 0;
// // //     if(rgbpacked[0]=='#'){ colour32bit = (long) strtol( &rgbpacked[1], NULL, 16);
// // //     }else{ colour32bit = (long) strtol( &rgbpacked[0], NULL, 16); }
// // //     RgbColor rgb;
// // //     rgb.R = colour32bit >> 16; //RGB
// // //     rgb.G = colour32bit >> 8 & 0xFF; //RGB
// // //     rgb.B = colour32bit & 0xFF; //RGB
// // //     // #ifndef ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT
// // //     // scene.colour = HsbColor(RgbColor(rgb.R,rgb.G,rgb.B));
// // //     // // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_RGB ":%s " D_NEOPIXEL_RGB ":%d,%d,%d " D_NEOPIXEL_HSB ":%d,%d,%d"),
// // //     // //   rgbpacked,rgb.R,rgb.G,rgb.B,scene.colour.H,scene.colour.S,scene.colour.B);
// // //     // #endif //ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT
// // //   }

// // //   // TIME with different units
// // //   if(!obj[D_JSON_TIME].isNull()){ //default to secs
// // //     pCONT_iLight->animation.transition.time_ms.val = obj["time"];
// // //     pCONT_iLight->animation.transition.time_ms.val *= 1000;
// // //     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),pCONT_iLight->animation.transition.time_ms.val);  
// // //   }else
// // //   if(!obj[D_JSON_TIME].isNull()){
// // //     pCONT_iLight->animation.transition.time_ms.val = obj["time_secs"];
// // //     pCONT_iLight->animation.transition.time_ms.val *= 1000;
// // //     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),pCONT_iLight->animation.transition.time_ms.val);  
// // //   }else
// // //   if(!obj[D_JSON_TIME_MS].isNull()){
// // //     pCONT_iLight->animation.transition.time_ms.val = obj["time_ms"];
// // //     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),pCONT_iLight->animation.transition.time_ms.val);  
// // //   }

  
// // // Flash colour needs to NOT be a scene, but part of a manual direct mode
// // // if(strstr(scenectr,"FLASHCOLOUR")){
// // //       scene_reseting.name_id = scene.name_id; // remember scene to return to
// // //       scene.name_id = MODE_SINGLECOLOUR_FLASHCOLOUR_ID;
// // //     }
// // //     // Parse out flashcolour info if it exists
// // //     if(scene.name_id == MODE_SINGLECOLOUR_FLASHCOLOUR_ID){

// // //       if(tempctr = obj["hue"]){
// // //         scene_reseting.hue = obj["hue"];
// // //       }
// // //       if(tempctr = obj["sat"]){
// // //         scene_reseting.sat = obj["sat"];
// // //       }
// // //       if(tempctr = obj["brt"]){
// // //         scene_reseting.brt = obj["brt"];
// // //       }
// // //       if(tempctr = obj["time"]){
// // //         scene_reseting.tOnTime = obj["time"];
// // //       }
// // //     }
  
  



// // //   // create easier names
// // //   // char* topic_ctr = data_buffer.topic.ctr;
// // //   // uint8_t topic_len = data_buffer.topic.len;
// // //   // char* payload_ctr = data_buffer.payload.ctr;
// // //   // uint8_t payload_len = data_buffer.payload.len;

// // //   if(mSupport::memsearch(data_buffer.topic.ctr,data_buffer.topic.len,"/center",sizeof("/center")-1)>=0){pCONT->mso->MessagePrintln("MATCHED /center");

// // //     memset(&parsed,0,sizeof(parsed)); // clear parsing struct

// // //     uint16_t index = 0;

// // //     ambilightsettings.colour.found_idx = 0;

// // //     StaticJsonDocument<300> doc;
// // //     DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
// // //     JsonObject root = doc.as<JsonObject>();

// // //     if(root["RGB"].is<JsonArray>()){

// // //       JsonArray colourarray = root["RGB"];
// // //       int array_length = colourarray.size();

// // //       const char* pixelcolour;
// // //       for(JsonVariantConst v : colourarray) {
// // //         pixelcolour = v.as<const char*>();

// // //         if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
// // //         }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }

// // //           ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R = colour32bit >> 16; //RGB
// // //           ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G = colour32bit >> 8 & 0xFF; //RGB
// // //           ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B = colour32bit & 0xFF; //RGB


// // //         ambilightsettings.colour.found_idx++;
// // //         index++;
// // //       }
// // //       pCONT->mso->MessagePrint("ENDambilightsettings.col.found_idx");
// // //       pCONT->mso->MessagePrintln(ambilightsettings.colour.found_idx);
// // //       pCONT->mso->MessagePrintln(index);

// // //     }
// // //     // else if(root["RGB"].is<const char*>()){ //one colour = string
// // //     //
// // //     //   const char* pixelcolour;
// // //     //   pixelcolour = root["RGB"];
// // //     //
// // //     //   //Serial.println("pixelcolour"); Serial.println(pixelcolour);
// // //     //   if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
// // //     //   }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }
// // //     //
// // //     //   ambilightsettings.colour.r[ambilightsettings.colour.found_idx] = colour32bit >> 16; //RGB
// // //     //   ambilightsettings.colour.g[ambilightsettings.colour.found_idx] = colour32bit >> 8 & 0xFF; //RGB
// // //     //   ambilightsettings.colour.b[ambilightsettings.colour.found_idx] = colour32bit & 0xFF; //RGB
// // //     //
// // //     //   // Serial.println(parsed.col.r[parsed.col.found_idx]);
// // //     //   // Serial.println(parsed.col.g[parsed.col.found_idx]);
// // //     //   // Serial.println(parsed.col.b[parsed.col.found_idx]);
// // //     //
// // //     //   ambilightsettings.colour.found_idx++;
// // //     // }

// // //     pCONT->mso->MessagePrint("ambilightsettings.colour.found_idx");
// // //     pCONT->mso->MessagePrintln(ambilightsettings.colour.found_idx);
// // //     //pCONT_iLight->settings.light_size_count
// // //     //ambilightsettings.colour.found_idx
// // //     for(int i=0;i<index;i++){
// // //         SetPixelColor(i, RgbColor(ambilightsettings.colour.rgb[i].R,ambilightsettings.colour.rgb[i].G,ambilightsettings.colour.rgb[i].B));
// // //     }stripbus->Show();


// // // } // END center


// // // #ifdef DEVICE_RGBDELL

// // //   if(mSupport::memsearch(data_buffer.topic.ctr,data_buffer.topic.len,"/left",sizeof("/left")-1)>=0){pCONT->mso->MessagePrintln("MATCHED /left");

// // //     memset(&parsed,0,sizeof(parsed)); // clear parsing struct

// // //     ambilightsettings.colour.found_idx = 0;

// // //     StaticJsonDocument<300> doc;
// // //     DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
// // //     JsonObject root = doc.as<JsonObject>();

// // //   // PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM -

// // //     if(root["RGB"].is<JsonArray>()){
// // //       //pCONT->mso->MessagePrintln("colour arr ");//Serial.println(rgbname_ctr);

// // //       JsonArray colourarray = root["RGB"];
// // //       int array_length = colourarray.size();

// // //       const char* pixelcolour;
// // //       for(JsonVariantConst v : colourarray) {
// // //         pixelcolour = v.as<const char*>();

// // //         //pCONT->mso->MessagePrintln("pixelcolour"); //pCONT->mso->MessagePrintln(pixelcolour);

// // //         if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
// // //         }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }

// // //         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R = colour32bit >> 16; //RGB
// // //         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G = colour32bit >> 8 & 0xFF; //RGB
// // //         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B = colour32bit & 0xFF; //RGB

// // //          pCONT->mso->MessagePrint(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R);pCONT->mso->MessagePrint("-");
// // //          pCONT->mso->MessagePrint(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G);pCONT->mso->MessagePrint("-");
// // //          pCONT->mso->MessagePrintln(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B);

// // //         ambilightsettings.colour.found_idx++;
// // //       }

// // //     }else if(root["RGB"].is<const char*>()){ //one colour = string

// // //       const char* pixelcolour;
// // //       pixelcolour = root["RGB"];

// // //       //Serial.println("pixelcolour"); Serial.println(pixelcolour);
// // //       if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
// // //       }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }

// // //       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R = colour32bit >> 16; //RGB
// // //       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G = colour32bit >> 8 & 0xFF; //RGB
// // //       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B = colour32bit & 0xFF; //RGB

// // //       // Serial.println(parsed.col.r[parsed.col.found_idx]);
// // //       // Serial.println(parsed.col.g[parsed.col.found_idx]);
// // //       // Serial.println(parsed.col.b[parsed.col.found_idx]);

// // //       ambilightsettings.colour.found_idx++;
// // //     }

// // //     pCONT->mso->MessagePrintln(ambilightsettings.colour.found_idx);
// // //     //pCONT_iLight->settings.light_size_count
// // //     //ambilightsettings.colour.found_idx
// // //     //  for(int i=0;i<ambilightsettings.colour.found_idx;i++){
// // //     //    SetPixelColor(i,RgbColor(ambilightsettings.colour.r[i],ambilightsettings.colour.g[i],ambilightsettings.colour.b[i]));    //turn every third pixel on
// // //     //  }
// // //     // /stripbus->Show();

// // //     uint32_t c; //colourrgb
// // //     pinMode(RGB_DATA_LEFT_PIN,OUTPUT);
// // //     pinMode(RGB_CLOCK_LEFT_PIN,OUTPUT);

// // //     digitalWrite(RGB_DATA_LEFT_PIN,LOW);digitalWrite(RGB_CLOCK_LEFT_PIN,LOW);
// // //     for(int ii=0;ii<ambilightsettings.colour.found_idx;ii++){
// // //       shiftOut(RGB_DATA_LEFT_PIN, RGB_CLOCK_LEFT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].R);
// // //       shiftOut(RGB_DATA_LEFT_PIN, RGB_CLOCK_LEFT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].G);
// // //       shiftOut(RGB_DATA_LEFT_PIN, RGB_CLOCK_LEFT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].B);
// // //     }
// // //     digitalWrite(RGB_DATA_LEFT_PIN,LOW);digitalWrite(RGB_CLOCK_LEFT_PIN,LOW);



// // // } // END left



// // //   if(mSupport::memsearch(data_buffer.topic.ctr,data_buffer.topic.len,"/right",sizeof("/right")-1)>=0){pCONT->mso->MessagePrintln("MATCHED /right");

// // //     memset(&parsed,0,sizeof(parsed)); // clear parsing struct

// // //     ambilightsettings.colour.found_idx = 0;

// // //     StaticJsonDocument<300> doc;
// // //     DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
// // //     JsonObject root = doc.as<JsonObject>();

// // //   // PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM -

// // //     if(root["RGB"].is<JsonArray>()){
// // //       //pCONT->mso->MessagePrintln("colour arr ");//Serial.println(rgbname_ctr);

// // //       JsonArray colourarray = root["RGB"];
// // //       int array_length = colourarray.size();

// // //       const char* pixelcolour;
// // //       for(JsonVariantConst v : colourarray) {
// // //         pixelcolour = v.as<const char*>();

// // //         //pCONT->mso->MessagePrintln("pixelcolour"); //pCONT->mso->MessagePrintln(pixelcolour);

// // //         if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
// // //         }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }

// // //         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R = colour32bit >> 16; //RGB
// // //         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G = colour32bit >> 8 & 0xFF; //RGB
// // //         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B = colour32bit & 0xFF; //RGB

// // //          pCONT->mso->MessagePrint(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R);pCONT->mso->MessagePrint("-");
// // //          pCONT->mso->MessagePrint(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G);pCONT->mso->MessagePrint("-");
// // //          pCONT->mso->MessagePrintln(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B);

// // //         ambilightsettings.colour.found_idx++;
// // //       }

// // //     }else if(root["RGB"].is<const char*>()){ //one colour = string

// // //       const char* pixelcolour;
// // //       pixelcolour = root["RGB"];

// // //       //Serial.println("pixelcolour"); Serial.println(pixelcolour);
// // //       if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
// // //       }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }

// // //       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R = colour32bit >> 16; //RGB
// // //       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G = colour32bit >> 8 & 0xFF; //RGB
// // //       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B = colour32bit & 0xFF; //RGB

// // //       // Serial.println(parsed.col.r[parsed.col.found_idx]);
// // //       // Serial.println(parsed.col.g[parsed.col.found_idx]);
// // //       // Serial.println(parsed.col.b[parsed.col.found_idx]);

// // //       ambilightsettings.colour.found_idx++;
// // //     }

// // //     pCONT->mso->MessagePrintln(ambilightsettings.colour.found_idx);
// // //     //pCONT_iLight->settings.light_size_count
// // //     //ambilightsettings.colour.found_idx
// // //     // for(int i=0;i<ambilightsettings.colour.found_idx;i++){
// // //     //   mrgbneo_ani->setPixelColor(i,mrgbneo_ani->Color(ambilightsettings.colour.r[i],ambilightsettings.colour.g[i],ambilightsettings.colour.b[i]));    //turn every third pixel on
// // //     // }
// // //     // mrgbneo_ani->setBrightness(255);
// // //     // mrgbneo_ani->show();

// // //     uint32_t c; //colourrgb
// // //     pinMode(RGB_DATA_RIGHT_PIN,OUTPUT);
// // //     pinMode(RGB_CLOCK_RIGHT_PIN,OUTPUT);

// // //     digitalWrite(RGB_DATA_RIGHT_PIN,LOW);digitalWrite(RGB_CLOCK_RIGHT_PIN,LOW);
// // //     for(int ii=0;ii<ambilightsettings.colour.found_idx;ii++){
// // //       shiftOut(RGB_DATA_RIGHT_PIN, RGB_CLOCK_RIGHT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].R);
// // //       shiftOut(RGB_DATA_RIGHT_PIN, RGB_CLOCK_RIGHT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].G);
// // //       shiftOut(RGB_DATA_RIGHT_PIN, RGB_CLOCK_RIGHT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].B);
// // //     }
// // //     digitalWrite(RGB_DATA_RIGHT_PIN,LOW);digitalWrite(RGB_CLOCK_RIGHT_PIN,LOW);



// // // } // END left

// // //       //fShowPanelUpdate = true;
// // //     //}


// // // //  }

// // //   //switch modes : USE serial input stream
// // //   //send as json array?
// // //   //RGB = [[r,g,b],[r,g,b],[r,g,b]]
// // //   //SETTINGS = {pixel ratio, 17,10}{pixel direction, CW}{startposition, bottom right}{timeout,10}{minbrightness,10}{fade,0}

// // //   pCONT_iLight->animation.mode_id = MODE_AMBILIGHT_ID;
// // //   fForcePanelUpdate = true;

// //   // return 0;

// // } // END FUNCTION


// /*******************************************************************************************************************
// ********************************************************************************************************************
// ************ END OF AMBILIGHT DEFINITIONS ********************************************************************************************
// ********************************************************************************************************************
// ********************************************************************************************************************/






#endif //USE_MODULE_LIGHTS_ANIMATOR

  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
// const uint32_t MUSIC_TIMING[] = {

// 0,
// 7,
// 240,
// 248,
// 488,
// 500,
// 739,
// 747,
// 1239,
// 1247,
// 1738,
// 1746,
// 2478,
// 2486,
// 2726,
// 2738,
// 3218,
// 3225,
// 3717,
// 3725,
// 4217,
// 4224,
// 4716,
// 4724,
// 5216,
// 5223,
// 5715,
// 5723,
// 6455,
// 6463,
// 6704,
// 6714,
// 7202,
// 7214,
// 7694,
// 7702,
// 8694,
// 8701,
// 9273,
// 9281,
// 9853,
// 9861,
// 10433,
// 10441,
// 11013,
// 11020,
// 11592,
// 11600,
// 12172,
// 12180,
// 12471,
// 12479,
// 12752,
// 12759,
// 13331,
// 13339,
// 13911,
// 13918,
// 14490,
// 14498,
// 15070,
// 15077,
// 15649,
// 15657,
// 16229,
// 16236,
// 16816,
// 16828,
// 17108,
// 17116,
// 17396,
// 17408,
// 17976,
// 17987,
// 18556,
// 18567,
// 19136,
// 19146,
// 19726,
// 19733,
// 20006,
// 20013,
// 20305,
// 20313,
// 20586,
// 20593,
// 20885,
// 20893,
// 21465,
// 21472,
// 22044,
// 22052,
// 22324,
// 22332,
// 22624,
// 22632,
// 22904,
// 22912,
// 23203,
// 23211,
// 23783,
// 23791,
// 24363,
// 24371,
// 24643,
// 24650,
// 24942,
// 24950,
// 25223,
// 25230,
// 25522,
// 25530,
// 26102,
// 26110,
// 26390,
// 26401,
// 26682,
// 26689,
// 27261,
// 27269,
// 27841,
// 27849,
// 28420,
// 28428,
// 29000,
// 29008,
// 29579,
// 29588,
// 30159,
// 30167,
// 30739,
// 30747,
// 31319,
// 31326,
// 31898,
// 31906,
// 32478,
// 32485,
// 33636,
// 33645,
// 34304,
// 34316,
// 34635,
// 34644,
// 34975,
// 34983,
// 35464,
// 35475,
// 35635,
// 35642,
// 35954,
// 35963,
// 36295,
// 36302,
// 36634,
// 36641,
// 36954,
// 36962,
// 37293,
// 37301,
// 37614,
// 37621,
// 37953,
// 37961,
// 38282,
// 38293,
// 38952,
// 38960,
// 39272,
// 39280,
// 39612,
// 39620,
// 39940,
// 39952,
// 40271,
// 40279,
// 40611,
// 40618,
// 40931,
// 40939,
// 41271,
// 41278,
// 41590,
// 41598,
// 41930,
// 41938,
// 42270,
// 42277,
// 43589,
// 43597,
// 44168,
// 44176,
// 44456,
// 44468,
// 44748,
// 44756,
// 45036,
// 45048,
// 45907,
// 45915,
// 46196,
// 46207,
// 46486,
// 46494,
// 46786,
// 46793,
// 47365,
// 47373,
// 47646,
// 47653,
// 47945,
// 47953,
// 48225,
// 48232,
// 48524,
// 48532,
// 49384,
// 49392,
// 49683,
// 49691,
// 49964,
// 49971,
// 50263,
// 50271,
// 50843,
// 50850,
// 51422,
// 51430,
// 51862,
// 51869,
// 52002,
// 52010,
// 52581,
// 52589,
// 53161,
// 53169,
// 53601,
// 53608,
// 53740,
// 53748,
// 54320,
// 54328,
// 54609,
// 54620,
// 54900,
// 54907,
// 55189,
// 55199,
// 55479,
// 55487,
// 56059,
// 56067,
// 56639,
// 56646,
// 57218,
// 57226,
// 57798,
// 57805,
// 58457,
// 58465,
// 58797,
// 58805,
// 59125,
// 59136,
// 59796,
// 59803,
// 60116,
// 60124,
// 60456,
// 60463,
// 60783,
// 60795,
// 61115,
// 61123,
// 61774,
// 61783,
// 62434,
// 62442,
// 62774,
// 62782,
// 63103,
// 63114,
// 63433,
// 63441,
// 63773,
// 63780,
// 64093,
// 64101,
// 64433,
// 64440,
// 64761,
// 64772,
// 65092,
// 65100,
// 65751,
// 65759,
// 66419,
// 66431,
// 66750,
// 66758,
// 67090,
// 67097,
// 67410,
// 67418,
// 67750,
// 67757,
// 68069,
// 68077,
// 68410,
// 68417,
// 68738,
// 68749,
// 69069,
// 69077,
// 69728,
// 69737,
// 70396,
// 70408,
// 71068,
// 71075,
// 71727,
// 71735,
// 72387,
// 72395,
// 73047,
// 73054,
// 74374,
// 74385,
// 74866,
// 74873,
// 75625,
// 75632,
// 75865,
// 75872,
// 76364,
// 76372,
// 76864,
// 76872,
// 77364,
// 77372,
// 77864,
// 77871,
// 78352,
// 78363,
// 78603,
// 78611,
// 78843,
// 78851,
// 79103,
// 79110,
// 79343,
// 79351,
// 79603,
// 79610,
// 79842,
// 79850,
// 80342,
// 80350,
// 80582,
// 80590,
// 80842,
// 80849,
// 81341,
// 81349,
// 81841,
// 81848,
// 82329,
// 82341,
// 82821,
// 82828,
// 83580,
// 83588,
// 83820,
// 83828,
// 84320,
// 84328,
// 84560,
// 84567,
// 84819,
// 84827,
// 85559,
// 85567,
// 85819,
// 85826,
// 86307,
// 86318,
// 86558,
// 86566,
// 86798,
// 86806,
// 87058,
// 87066,
// 87298,
// 87306,
// 87797,
// 87806,
// 88047,
// 88058,
// 88297,
// 88305,
// 88797,
// 88805,
// 89296,
// 89304,
// 89796,
// 89804,
// 90285,
// 90296,
// 90776,
// 90784,
// 91275,
// 91284,
// 91535,
// 91543,
// 92025,
// 92035,
// 92275,
// 92283,
// 92774,
// 92783,
// 93274,
// 93282,
// 93774,
// 93782,
// 94262,
// 94274,
// 94753,
// 94761,
// 95253,
// 95261,
// 95513,
// 95521,
// 95753,
// 95760,
// 96002,
// 96012,
// 96253,
// 96260,
// 96752,
// 96760,
// 97252,
// 97259,
// 97751,
// 97759,
// 98240,
// 98251,
// 98731,
// 98739,
// 99231,
// 99238,
// 99490,
// 99498,
// 99990,
// 99997,
// 100069

// };



const uint32_t MUSIC_TIMING[] = {
0,
7.972,
499.402,
507.345,
1239.155,
1246.915,
1486.988,
1498.504,
1978.485,
1986.135,
2477.608,
2485.89,
2977.531,
2985.234,
3476.742,
3485.012,
3976.526,
3984.354,
4216.1,
4224.27,
4476.055,
4483.668,
4715.713,
4723.855,
4975.646,
4983.101,
5215.28,
5223.448,
5475.251,
5482.568,
5963.141,
5974.626,
6214.72,
6222.387,
6454.313,
6462.491,
6954.357,
6962.023,
7453.624,
7461.892,
7953.691,
7961.352,
8452.793,
8461.126,
9192.867,
9200.642,
9452.332,
9460.077,
9940.75,
9952.133,
10191.923,
10200.067,
10432.143,
10439.687,
11191.14,
11199.189,
11431.529,
11439.092,
11930.861,
11939.134,
12178.845,
12190.851,
12430.409,
12438.662,
12670.817,
12678.358,
12930.18,
12938.213,
13430.08,
13437.6,
13669.854,
13678.009,
13919.112,
13929.893,
14409.374,
14417.651,
14909.454,
14917.125,
15408.764,
15417.023,
15908.825,
15916.49,
16408.118,
16416.381,
16907.866,
16915.69,
17147.64,
17155.794,
17647.684,
17655.34,
17907.063,
17914.782,
18395.001,
18406.844,
18886.432,
18894.685,
19386.443,
19394.129,
19885.738,
19893.997,
20385.762,
20393.437,
20884.842,
20893.168,
21125.234,
21132.779,
21384.583,
21392.539,
21624.889,
21632.418,
21884.245,
21892.017,
22372.704,
22384.026,
22863.555,
22871.818,
23363.529,
23371.224,
23862.805,
23871.069,
24362.828,
24370.49,
24861.942,
24870.356,
25102.289,
25109.86,
25601.557,
25609.85,
25861.549,
25869.021,
26350.888,
26360.99,
26848.94,
26860.828,
27340.425,
27348.692,
27840.485,
27848.167,
28339.784,
28348.051,
28839.847,
28847.528,
29079.292,
29087.475,
29339.325,
29346.962,
29578.959,
29587.14,
29838.977,
29846.462,
30338.364,
30346.149,
30826.407,
30838.185,
31817.477,
31825.872,
32817.334,
32825.151,
33316.531,
33324.865,
33816.602,
33824.242,
34316.244,
34324.005,
34556.58,
34564.162,
34804.449,
34816.025,
35296.2,
35303.891,
35795.556,
35803.833,
36295.647,
36303.341,
36794.925,
36803.228,
37055.158,
37059.831,


};

  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Slow Glow 
 * @description:   : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param intensity: 0-255 is how many should pixels should randomly change (0-255 scaled to 0-pixel_count)
 * @param speed    : None
 * @param rate     : Period of time (ms) between updates
 * @param time     : Blend time
 * 
 * 
 * Method 1: Inside the music function, when aux0 is reseting, then "playTrack" should be started where another driver for the music should be called.
 *           Wait until finished, or with timeout, and then use the start time as the animation sync start and play the effect until the end.
 * 
 * Method 2: Could be started same as above. I may want to call any other effect and simple apply the music as a trigger. Or introduce animation trigger methods where
 *            - using speed/intensity
 *            - using sound reactive, which uses a microphone to then call each animation
 *            - using prerecorded tracks, ie christmas music controller
 * 
 *            Method 2 would allow animations to be applied to any current and future animation, and simply be triggering calls for the animation to update. 
 * 
 *            Playlists in this case could actually be used to literally link sound/animations together.
 * 
 * 
 * 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
void mAnimatorLight::EffectAnim__Christmas_Musical__01()
{

  uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type) * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  if (!SEGMENT.allocateData(dataSize)){    
    ALOG_ERR( PM_JSON_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID;
    return;
  }



  if(SEGMENT.params_internal.aux0 == 0)
  {

  }
  if(SEGMENT.params_internal.aux0 == ARRAY_SIZE(MUSIC_TIMING))
  {
    SEGMENT.params_internal.aux0 = 0;
    SEGMENT.tSaved_EffectStartReferenceTime = millis();
    
  }

  SEGMENT.params_internal.aux0++;


  uint32_t time_from_reference = millis() - SEGMENT.tSaved_EffectStartReferenceTime;


  // ALOG_INF(PSTR("EffectAnim__Christmas_Musical__01 aux0 = %d %d/%d"), SEGMENT.params_internal.aux0, 
  //   SEGMENT.tSaved_EffectStartReferenceTime,
  //   time_from_reference

  
  
  // );



  uint32_t next_time = 0;
  if(SEGMENT.params_internal.aux0 < ARRAY_SIZE(MUSIC_TIMING))
  {
    next_time = MUSIC_TIMING[SEGMENT.params_internal.aux0+1] - MUSIC_TIMING[SEGMENT.params_internal.aux0];

      
    // ALOG_INF(PSTR("EffectAnim__Christmas_Musical__01 aux0 = %d|%d next=%d"), 
    // MUSIC_TIMING[SEGMENT.params_internal.aux0+1], MUSIC_TIMING[SEGMENT.params_internal.aux0],
    // next_time
    // );





  }

  
  // for (uint16_t i = 0; i < c; i++) //fill from start until c
    // SEGMENT.SetPixelColor(0, RgbcctColor( SEGMENT.params_internal.aux0,0,0  ));
  // }

  SEGMENT.transition.rate_ms = next_time;
  SEGMENT.transition.time_ms = 0;
  // SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  



  // ALOG_INF(PSTR("EffectAnim__Christmas_Musical__01 aux0 = %d %d/%d"), SEGMENT.params_internal.aux0, 
  //   SEGMENT.tSaved_EffectStartReferenceTime,
  //   time_from_reference
  // );


  uint16_t* region_p          = &SEGMENT.params_internal.aux0;
  uint16_t* indexes_active_p  = &SEGMENT.params_internal.aux1; // shared_flasher_parameters_segments.indexes.active
  uint16_t* indexes_counter_p = &SEGMENT.params_internal.aux2; // shared_flasher_parameters_segments.indexes.counter

  desired_pixel=0;
    
  uint8_t pixel_position = 0;
  uint8_t pixels_in_map = GetNumberOfColoursInPalette(SEGMENT.palette.id);

  // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "pixels_in_map= %d"),pixels_in_map);
  
  desired_pixel = *indexes_active_p;
  uint8_t pixels_map_upper_limit = *indexes_active_p+1;
  uint8_t pixels_map_lower_limit = *indexes_active_p;

  uint8_t index_1, index_2;
  uint8_t counter = 0;
      
  //if last pixel, then include it and the first, else include it and the next
  if(*indexes_active_p == pixels_in_map-1){ //wrap wround
    index_1 = 0;
    index_2 = *indexes_active_p;
    counter = 0;
  }else{
    index_1 = *indexes_active_p;
    index_2 = *indexes_active_p+1;
    counter = 1;

  }

  *indexes_counter_p ^= 1;

  // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "counter = %d/%d/%d"), counter,index_1,index_2);

  RgbcctColor colour;

  for(uint16_t index=SEGMENT.pixel_range.start;
                index<=SEGMENT.pixel_range.stop;
                index++
  ){

    if(counter^=1){
      desired_pixel = *indexes_counter_p ? index_2 : index_1;
    }else{
      desired_pixel = *indexes_counter_p ? index_1 : index_2;
    }
    
    colour = SEGMENT.GetPaletteColour(desired_pixel, PALETTE_SPAN_OFF, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, &pixel_position);
    
    colour = ApplyBrightnesstoRgbcctColour(colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());

    SetTransitionColourBuffer_DesiredColour(SEGMENT.data, SEGMENT.DataLength(), index, SEGMENT.colour_type, colour);
        
  } 

//messy
  if(++*indexes_active_p>pixels_in_map-1){
    *indexes_active_p=0;
  }
  
  
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();
  
  SetSegment_AnimFunctionCallback(  SEGIDX, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    }
  );


}
static const char PM_EFFECT_CONFIG__CHRISTMAS_MUSICAL_01[] PROGMEM = ",,,,,Time,Rate;!,!,!,!,!;!"; // 7 sliders + 4 options before first ;
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME





void mAnimatorLight::LoadEffects()
{

  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
  addEffect3(EFFECTS_FUNCTION__SOLID_COLOUR__ID,                  &mAnimatorLight::EffectAnim__Solid_Colour,                    PM_EFFECTS_FUNCTION__SOLID_COLOUR__NAME_CTR,                                   PM_EFFECT_CONFIG__SOLID_COLOUR);
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
  addEffect3(EFFECTS_FUNCTION__STATIC_PALETTE__ID,                &mAnimatorLight::EffectAnim__Static_Palette,                  PM_EFFECTS_FUNCTION__STATIC_PALETTE__NAME_CTR,                                 PM_EFFECT_CONFIG__STATIC_PALETTE);
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
  addEffect3(EFFECTS_FUNCTION__SPANNED_PALETTE__ID,                            &mAnimatorLight::EffectAnim__Spanned_Static_Palette,  PM_EFFECTS_FUNCTION__SPANNED_PALETTE__NAME_CTR,                           PM_EFFECT_CONFIG__SPANNED_PALETTE);
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
  addEffect3(EFFECTS_FUNCTION__SLOW_GLOW__ID,                     &mAnimatorLight::EffectAnim__Slow_Glow,                       PM_EFFECTS_FUNCTION__SLOW_GLOW__NAME_CTR,                                      PM_EFFECT_CONFIG__SLOW_GLOW);
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
  addEffect3(EFFECTS_FUNCTION__WLED_CANDLE_SINGLE__ID,            &mAnimatorLight::EffectAnim__Candle_Single,                   PM_EFFECTS_FUNCTION__WLED_CANDLE_SINGLE__NAME_CTR,                             PM_EFFECT_CONFIG__CANDLE_SINGLE);  
  addEffect3(EFFECTS_FUNCTION__WLED_CANDLE_MULTIPLE__ID,          &mAnimatorLight::EffectAnim__Candle_Multiple,                 PM_EFFECTS_FUNCTION__WLED_CANDLE_MULTIPLE__NAME_CTR,                           PM_EFFECT_CONFIG__CANDLE_MULTIPLE);
  addEffect3(EFFECTS_FUNCTION__SHIMMERING_PALETTE__ID,            &mAnimatorLight::EffectAnim__Shimmering_Palette,              PM_EFFECTS_FUNCTION__SHIMMERING_PALETTE__NAME_CTR,                             PM_EFFECT_CONFIG__SHIMMERING_PALETTE);
  addEffect3(EFFECTS_FUNCTION__SHIMMERING_PALETTE_DOUBLE__ID,     &mAnimatorLight::EffectAnim__Shimmering_Two_Palette,          PM_EFFECTS_FUNCTION__SHIMMERING_TWO_PALETTES__NAME_CTR,                        PM_EFFECT_CONFIG__SHIMMERING_TWO_PALETTES);
  #endif  
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  addEffect3(EFFECTS_FUNCTION__SHIMMERING_PALETTE_SATURATION__ID, &mAnimatorLight::EffectAnim__Shimmering_Palette_Saturation,   PM_EFFECTS_FUNCTION__SHIMMERING_PALETTE_SATURATION__NAME_CTR,                  PM_EFFECT_CONFIG__SHIMMERING_PALETTE_SATURATION);
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  addEffect3(EFFECTS_FUNCTION__STATIC_GRADIENT_PALETTE__ID,       &mAnimatorLight::EffectAnim__Static_Gradient_Palette,         PM_EFFECTS_FUNCTION__STATIC_GRADIENT_PALETTE__NAME_CTR,                        PM_EFFECT_CONFIG__STATIC_GRADIENT_PALETTE);
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  addEffect3(EFFECTS_FUNCTION__ROTATING_PALETTE_NEW__ID,          &mAnimatorLight::EffectAnim__Rotating_Palette_New,            PM_EFFECTS_FUNCTION__ROTATING_PALETTE_NEW__NAME_CTR,                           PM_EFFECT_CONFIG__ROTATING_PALETTE_NEW);
  #endif 
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  addEffect3(EFFECTS_FUNCTION__ROTATING_PALETTE__ID,              &mAnimatorLight::EffectAnim__Rotating_Palette,                PM_EFFECTS_FUNCTION__ROTATING_PALETTE__NAME_CTR,                               PM_EFFECT_CONFIG__ROTATING_PALETTE);
  #endif 
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  addEffect3(EFFECTS_FUNCTION__ROTATING_PREVIOUS_ANIMATION__ID,   &mAnimatorLight::EffectAnim__Rotating_Previous_Animation,     PM_EFFECTS_FUNCTION__ROTATING_PREVIOUS_ANIMATION__NAME_CTR,                    PM_EFFECT_CONFIG__ROTATING_PREVIOUS_ANIMATION);
  #endif 
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  addEffect3(EFFECTS_FUNCTION__STEPPING_PALETTE__ID,                           &mAnimatorLight::EffectAnim__Stepping_Palette,                      PM_EFFECTS_FUNCTION__STEPPING_PALETTE__NAME_CTR,                                  PM_EFFECT_CONFIG__STEPPING_PALETTE);
  #endif 
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
  addEffect3(EFFECTS_FUNCTION__PALETTE_COLOUR_FADE_SATURATION__ID,             &mAnimatorLight::EffectAnim__Palette_Colour_Fade_Saturation,        PM_EFFECTS_FUNCTION__POPPING_DECAY_PALETTE__NAME_CTR,                             PM_EFFECT_CONFIG__POPPING_DECAY_PALETTE);
  #endif 
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
  addEffect3(EFFECTS_FUNCTION__BLEND_PALETTE_BETWEEN_ANOTHER_PALETTE__ID,      &mAnimatorLight::EffectAnim__Blend_Two_Palettes,                    PM_EFFECTS_FUNCTION__BLEND_TWO_PALETTES__NAME_CTR,                                PM_EFFECT_CONFIG__BLEND_TWO_PALETTES);
  #endif 
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
  addEffect3(EFFECTS_FUNCTION__TWINKLE_PALETTE_SEC_ON_ORDERED_PALETTE_PRI__ID, &mAnimatorLight::EffectAnim__Twinkle_Palette_Onto_Palette,          PM_EFFECTS_FUNCTION__TWINKLE_PALETTE_SEC_ON_ORDERED_PALETTE_PRI__NAME_CTR,        PM_EFFECT_CONFIG__TWINKLE_PALETTE_SEC_ON_ORDERED_PALETTE_PRI);
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
  addEffect3(EFFECTS_FUNCTION__TWINKLE_DECAYING_PALETTE__ID,                   &mAnimatorLight::EffectAnim__Twinkle_Decaying_Palette,              PM_EFFECTS_FUNCTION__TWINKLE_DECAYING_PALETTE__NAME_CTR,                          PM_EFFECT_CONFIG__TWINKLE_DECAYING_PALETTE);
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  addEffect3(EFFECTS_FUNCTION__POPPING_DECAY_PALETTE__ID,                      &mAnimatorLight::EffectAnim__Popping_Decay_Palette,                 PM_EFFECTS_FUNCTION__POPPING_DECAY_PALETTE__NAME_CTR,                             PM_EFFECT_CONFIG__POPPING_DECAY_PALETTE);
  #endif 
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  addEffect3(EFFECTS_FUNCTION__POPPING_DECAY_RANDOM__ID,                       &mAnimatorLight::EffectAnim__Popping_Decay_Random,                  PM_EFFECTS_FUNCTION__POPPING_DECAY_RANDOM__NAME_CTR,                              PM_EFFECT_CONFIG__POPPING_DECAY_RANDOM);
  #endif 
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  addEffect3(EFFECTS_FUNCTION__CHRISTMAS_MUSICAL__01_ID,                       &mAnimatorLight::EffectAnim__Christmas_Musical__01,                 PM_EFFECTS_FUNCTION__CHRISTMAS_MUSICAL__01__NAME_CTR,                             PM_EFFECT_CONFIG__CHRISTMAS_MUSICAL_01);
  #endif 
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING   
  addEffect3(EFFECTS_FUNCTION__STATIC_PALETTE_SPANNING_SEGMENT__ID,            &mAnimatorLight::SubTask_Flasher_Animate_Function__Static_Palette_Spanning_Segment, PM_EFFECTS_FUNCTION__STATIC_PALETTE_SPANNING_SEGMENT__NAME_CTR); 
  #endif        
  /**
   * Static
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
  addEffect3(EFFECTS_FUNCTION__WLED_STATIC__ID,               &mAnimatorLight::EffectAnim__Static_Solid,                          PM_EFFECTS_FUNCTION__WLED_STATIC__NAME_CTR,                      PM_EFFECT_CONFIG__STATIC);
  addEffect3(EFFECTS_FUNCTION__WLED_STATIC_PATTERN__ID,       &mAnimatorLight::EffectAnim__Static_Pattern,                        PM_EFFECTS_FUNCTION__WLED_STATIC_PATTERN__NAME_CTR,              PM_EFFECT_CONFIG__STATIC_PATTERN);
  addEffect3(EFFECTS_FUNCTION__WLED_TRI_STATIC_PATTERN__ID,   &mAnimatorLight::EffectAnim__Tri_Static_Pattern,                    PM_EFFECTS_FUNCTION__WLED_TRI_STATIC_PATTERN__NAME_CTR,          PM_EFFECT_CONFIG__TRI_STATIC_PATTERN);
  #endif
  addEffect3(EFFECTS_FUNCTION__WLED_SPOTS__ID,                &mAnimatorLight::EffectAnim__Spots,                                 PM_EFFECTS_FUNCTION__WLED_SPOTS__NAME_CTR,                       PM_EFFECT_CONFIG__SPOTS);
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  addEffect3(EFFECTS_FUNCTION__WLED_PERCENT__ID,              &mAnimatorLight::EffectAnim__Percent,                               PM_EFFECTS_FUNCTION__WLED_PERCENT__NAME_CTR,                     PM_EFFECT_CONFIG__PERCENT);
  #endif
  /**
   * One Colour
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  addEffect3(EFFECTS_FUNCTION__WLED_RANDOM_COLOR__ID,         &mAnimatorLight::EffectAnim__Random_Colour,                         PM_EFFECTS_FUNCTION__WLED_RANDOM_COLOR__NAME_CTR,                PM_EFFECT_CONFIG__RANDOM_COLOR);
  #endif
  /**
   * Wipe/Sweep/Runners 
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  addEffect3(EFFECTS_FUNCTION__WLED_COLOR_WIPE__ID,           &mAnimatorLight::EffectAnim__Colour_Wipe,                           PM_EFFECTS_FUNCTION__WLED_COLOR_WIPE__NAME_CTR,                  PM_EFFECT_CONFIG__COLOR_WIPE);
  addEffect3(EFFECTS_FUNCTION__WLED_COLOR_WIPE_RANDOM__ID,    &mAnimatorLight::EffectAnim__Colour_Wipe_Random,                    PM_EFFECTS_FUNCTION__WLED_COLOR_WIPE_RANDOM__NAME_CTR,           PM_EFFECT_CONFIG__COLOR_WIPE_RANDOM);
  addEffect3(EFFECTS_FUNCTION__WLED_COLOR_SWEEP__ID,          &mAnimatorLight::EffectAnim__Colour_Sweep,                          PM_EFFECTS_FUNCTION__WLED_COLOR_SWEEP__NAME_CTR,                 PM_EFFECT_CONFIG__COLOR_SWEEP);
  addEffect3(EFFECTS_FUNCTION__WLED_COLOR_SWEEP_RANDOM__ID,   &mAnimatorLight::EffectAnim__Colour_Sweep_Random,                   PM_EFFECTS_FUNCTION__WLED_COLOR_SWEEP_RANDOM__NAME_CTR,          PM_EFFECT_CONFIG__COLOR_SWEEP_RANDOM);
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  /**
   * Fireworks
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
  addEffect3(EFFECTS_FUNCTION__WLED_FIREWORKS__ID,                     &mAnimatorLight::EffectAnim__Fireworks,                    PM_EFFECTS_FUNCTION__WLED_FIREWORKS__NAME_CTR,                      PM_EFFECT_CONFIG__FIREWORKS);
  addEffect3(EFFECTS_FUNCTION__WLED_FIREWORKS_EXPLODING__ID,           &mAnimatorLight::EffectAnim__Exploding_Fireworks,          PM_EFFECTS_FUNCTION__WLED_EXPLODING_FIREWORKS__NAME_CTR,            PM_EFFECT_CONFIG__EXPLODING_FIREWORKS);
  addEffect3(EFFECTS_FUNCTION__WLED_FIREWORKS_STARBURST__ID,           &mAnimatorLight::EffectAnim__Fireworks_Starburst,          PM_EFFECTS_FUNCTION__WLED_STARBURST__NAME_CTR,                      PM_EFFECT_CONFIG__STARBURST);
  addEffect3(EFFECTS_FUNCTION__WLED_FIREWORKS_STARBURST_GLOWS__ID,     &mAnimatorLight::EffectAnim__Fireworks_Starburst_Glows,    PM_EFFECTS_FUNCTION__WLED_STARBURST_GLOWS__NAME_CTR,                PM_EFFECT_CONFIG__STARBURST_GLOWS);
  addEffect3(EFFECTS_FUNCTION__WLED_RAIN__ID,                          &mAnimatorLight::EffectAnim__Rain,                         PM_EFFECTS_FUNCTION__WLED_RAIN__NAME_CTR,                           PM_EFFECT_CONFIG__RAIN);
  addEffect3(EFFECTS_FUNCTION__WLED_FIREWORKS_EXPLODING_NO_LAUNCH__ID, &mAnimatorLight::EffectAnim__Exploding_Fireworks_NoLaunch, PM_EFFECTS_FUNCTION__WLED_EXPLODING_FIREWORKS_NO_LAUNCH__NAME_CTR,  PM_EFFECT_CONFIG__EXPLODING_FIREWORKS_NOLAUNCH);
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  addEffect3(EFFECTS_FUNCTION__WLED_TRICOLOR_WIPE__ID,         &mAnimatorLight::EffectAnim__TriColour,          PM_EFFECTS_FUNCTION__WLED_TRICOLOR_WIPE__NAME_CTR,              PM_EFFECT_CONFIG__TRICOLOR_WIPE);
  addEffect3(EFFECTS_FUNCTION__WLED_ANDROID__ID,               &mAnimatorLight::EffectAnim__Android,            PM_EFFECTS_FUNCTION__WLED_ANDROID__NAME_CTR,                    PM_EFFECT_CONFIG__ANDROID);
  addEffect3(EFFECTS_FUNCTION__WLED_RUNNING_COLOR__ID,         &mAnimatorLight::EffectAnim__Running_Colour,     PM_EFFECTS_FUNCTION__WLED_RUNNING_COLOR__NAME_CTR,              PM_EFFECT_CONFIG__RUNNING_COLOR);
  addEffect3(EFFECTS_FUNCTION__WLED_RUNNING_RANDOM__ID,        &mAnimatorLight::EffectAnim__Running_Random,     PM_EFFECTS_FUNCTION__WLED_RUNNING_RANDOM__NAME_CTR,             PM_EFFECT_CONFIG__RUNNING_RANDOM);
  addEffect3(EFFECTS_FUNCTION__WLED_GRADIENT__ID,              &mAnimatorLight::EffectAnim__Gradient,           PM_EFFECTS_FUNCTION__WLED_GRADIENT__NAME_CTR,                   PM_EFFECT_CONFIG__GRADIENT);
  addEffect3(EFFECTS_FUNCTION__WLED_LOADING__ID,               &mAnimatorLight::EffectAnim__Loading,            PM_EFFECTS_FUNCTION__WLED_LOADING__NAME_CTR,                    PM_EFFECT_CONFIG__LOADING);
  addEffect3(EFFECTS_FUNCTION__WLED_POLICE__ID,                &mAnimatorLight::EffectAnim__Police,             PM_EFFECTS_FUNCTION__WLED_POLICE__NAME_CTR,                     PM_EFFECT_CONFIG__POLICE);
  addEffect3(EFFECTS_FUNCTION__WLED_POLICE_ALL__ID,            &mAnimatorLight::EffectAnim__Polce_All,          PM_EFFECTS_FUNCTION__WLED_POLICE_ALL__NAME_CTR,                 PM_EFFECT_CONFIG__POLICE_ALL);
  addEffect3(EFFECTS_FUNCTION__WLED_TWO_DOTS__ID,              &mAnimatorLight::EffectAnim__Two_Dots,           PM_EFFECTS_FUNCTION__WLED_TWO_DOTS__NAME_CTR,                   PM_EFFECT_CONFIG__TWO_DOTS);
  addEffect3(EFFECTS_FUNCTION__WLED_TWO_AREAS__ID,             &mAnimatorLight::EffectAnim__Two_Areas,          PM_EFFECTS_FUNCTION__WLED_TWO_AREAS__NAME_CTR,                  PM_EFFECT_CONFIG__TWO_AREAS);
  addEffect3(EFFECTS_FUNCTION__WLED_MULTI_COMET__ID,           &mAnimatorLight::EffectAnim__Multi_Comet,        PM_EFFECTS_FUNCTION__WLED_MULTI_COMET__NAME_CTR,                PM_EFFECT_CONFIG__MULTI_COMET);
  addEffect3(EFFECTS_FUNCTION__WLED_OSCILLATE__ID,             &mAnimatorLight::EffectAnim__Oscillate,          PM_EFFECTS_FUNCTION__WLED_OSCILLATE__NAME_CTR,                  PM_EFFECT_CONFIG__OSCILLATE);
  addEffect3(EFFECTS_FUNCTION__WLED_BPM__ID,                   &mAnimatorLight::EffectAnim__BPM,                PM_EFFECTS_FUNCTION__WLED_BPM__NAME_CTR,                        PM_EFFECT_CONFIG__BPM);
  addEffect3(EFFECTS_FUNCTION__WLED_JUGGLE__ID,                &mAnimatorLight::EffectAnim__Juggle,             PM_EFFECTS_FUNCTION__WLED_JUGGLE__NAME_CTR,                     PM_EFFECT_CONFIG__JUGGLES);
  addEffect3(EFFECTS_FUNCTION__WLED_PALETTE__ID,               &mAnimatorLight::EffectAnim__Palette,            PM_EFFECTS_FUNCTION__WLED_PALETTE__NAME_CTR,                    PM_EFFECT_CONFIG__PALETTE);
  addEffect3(EFFECTS_FUNCTION__WLED_COLORWAVES__ID,            &mAnimatorLight::EffectAnim__ColourWaves,        PM_EFFECTS_FUNCTION__WLED_COLORWAVES__NAME_CTR,                 PM_EFFECT_CONFIG__COLORWAVES);
  addEffect3(EFFECTS_FUNCTION__WLED_LAKE__ID,                  &mAnimatorLight::EffectAnim__Lake,               PM_EFFECTS_FUNCTION__WLED_LAKE__NAME_CTR,                       PM_EFFECT_CONFIG__LAKE);
  addEffect3(EFFECTS_FUNCTION__WLED_GLITTER__ID,               &mAnimatorLight::EffectAnim__Glitter,            PM_EFFECTS_FUNCTION__WLED_GLITTER__NAME_CTR,                    PM_EFFECT_CONFIG__GLITTER);
  addEffect3(EFFECTS_FUNCTION__WLED_METEOR__ID,                &mAnimatorLight::EffectAnim__Meteor,             PM_EFFECTS_FUNCTION__WLED_METEOR__NAME_CTR,                     PM_EFFECT_CONFIG__METEOR);
  addEffect3(EFFECTS_FUNCTION__WLED_METEOR_SMOOTH__ID,         &mAnimatorLight::EffectAnim__Metoer_Smooth,      PM_EFFECTS_FUNCTION__WLED_METEOR_SMOOTH__NAME_CTR,              PM_EFFECT_CONFIG__METEOR_SMOOTH);
  addEffect3(EFFECTS_FUNCTION__WLED_PRIDE_2015__ID,            &mAnimatorLight::EffectAnim__Pride_2015,         PM_EFFECTS_FUNCTION__WLED_PRIDE_2015__NAME_CTR,                 PM_EFFECT_CONFIG__PRIDE_2015);
  addEffect3(EFFECTS_FUNCTION__WLED_PACIFICA__ID,              &mAnimatorLight::EffectAnim__Pacifica,           PM_EFFECTS_FUNCTION__WLED_PACIFICA__NAME_CTR,                   PM_EFFECT_CONFIG__PACIFICA);
  addEffect3(EFFECTS_FUNCTION__WLED_SUNRISE__ID,               &mAnimatorLight::EffectAnim__Sunrise,            PM_EFFECTS_FUNCTION__WLED_SUNRISE__NAME_CTR,                    PM_EFFECT_CONFIG__SUNRISE);
  addEffect3(EFFECTS_FUNCTION__WLED_SINEWAVE__ID,              &mAnimatorLight::EffectAnim__Sinewave,           PM_EFFECTS_FUNCTION__WLED_SINEWAVE__NAME_CTR,                   PM_EFFECT_CONFIG__SINEWAVE);
  addEffect3(EFFECTS_FUNCTION__WLED_FLOW__ID,                  &mAnimatorLight::EffectAnim__Flow,               PM_EFFECTS_FUNCTION__WLED_FLOW__NAME_CTR,                       PM_EFFECT_CONFIG__FLOW);
  addEffect3(EFFECTS_FUNCTION__WLED_RUNNING_LIGHTS__ID,        &mAnimatorLight::EffectAnim__Running_Lights,     PM_EFFECTS_FUNCTION__WLED_RUNNING_LIGHTS__NAME_CTR,             PM_EFFECT_CONFIG__RUNNING_LIGHTS);
  addEffect3(EFFECTS_FUNCTION__WLED_RAINBOW_CYCLE__ID,         &mAnimatorLight::EffectAnim__Rainbow_Cycle,      PM_EFFECTS_FUNCTION__WLED_RAINBOW_CYCLE__NAME_CTR,              PM_EFFECT_CONFIG__RAINBOW_CYCLE);
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  /**
   * Chase
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  addEffect3(EFFECTS_FUNCTION__WLED_CHASE_COLOR__ID,           &mAnimatorLight::EffectAnim__Chase_Colour,          PM_EFFECTS_FUNCTION__WLED_CHASE_COLOR__NAME_CTR,             PM_EFFECT_CONFIG__CHASE_COLOR);
  addEffect3(EFFECTS_FUNCTION__WLED_CHASE_RANDOM__ID,          &mAnimatorLight::EffectAnim__Chase_Random,          PM_EFFECTS_FUNCTION__WLED_CHASE_RANDOM__NAME_CTR,            PM_EFFECT_CONFIG__CHASE_RANDOM);
  addEffect3(EFFECTS_FUNCTION__WLED_CHASE_RAINBOW__ID,         &mAnimatorLight::EffectAnim__Chase_Rainbow,         PM_EFFECTS_FUNCTION__WLED_CHASE_RAINBOW__NAME_CTR,           PM_EFFECT_CONFIG__CHASE_RAINBOW);
  addEffect3(EFFECTS_FUNCTION__WLED_CHASE_FLASH__ID,           &mAnimatorLight::EffectAnim__Chase_Flash,           PM_EFFECTS_FUNCTION__WLED_CHASE_FLASH__NAME_CTR,             PM_EFFECT_CONFIG__CHASE_FLASH);
  addEffect3(EFFECTS_FUNCTION__WLED_CHASE_FLASH_RANDOM__ID,    &mAnimatorLight::EffectAnim__Chase_Flash_Random,    PM_EFFECTS_FUNCTION__WLED_CHASE_FLASH_RANDOM__NAME_CTR,      PM_EFFECT_CONFIG__CHASE_FLASH_RANDOM);
  addEffect3(EFFECTS_FUNCTION__WLED_CHASE_RAINBOW_WHITE__ID,   &mAnimatorLight::EffectAnim__Chase_Rainbow_White,   PM_EFFECTS_FUNCTION__WLED_CHASE_RAINBOW_WHITE__NAME_CTR,     PM_EFFECT_CONFIG__CHASE_RAINBOW_WHITE);
  addEffect3(EFFECTS_FUNCTION__WLED_CHASE_THEATER__ID,         &mAnimatorLight::EffectAnim__Chase_Theater,         PM_EFFECTS_FUNCTION__WLED_THEATER_CHASE__NAME_CTR,           PM_EFFECT_CONFIG__THEATER_CHASE);
  addEffect3(EFFECTS_FUNCTION__WLED_CHASE_THEATER_RAINBOW__ID, &mAnimatorLight::EffectAnim__Chase_Theatre_Rainbow, PM_EFFECTS_FUNCTION__WLED_THEATER_CHASE_RAINBOW__NAME_CTR,   PM_EFFECT_CONFIG__THEATER_CHASE_RAINBOW);
  addEffect3(EFFECTS_FUNCTION__WLED_CHASE_TRICOLOR__ID,        &mAnimatorLight::EffectAnim__Chase_TriColour,       PM_EFFECTS_FUNCTION__WLED_TRICOLOR_CHASE__NAME_CTR,          PM_EFFECT_CONFIG__TRICOLOR_CHASE);
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  /**
   *  Breathe/Fade/Pulse
   **/    
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  addEffect3(EFFECTS_FUNCTION__WLED_BREATH__ID,                &mAnimatorLight::EffectAnim__Breath,                PM_EFFECTS_FUNCTION__WLED_BREATH__NAME_CTR,                  PM_EFFECT_CONFIG__BREATH);
  addEffect3(EFFECTS_FUNCTION__WLED_FADE__ID,                  &mAnimatorLight::EffectAnim__Fade,                  PM_EFFECTS_FUNCTION__WLED_FADE__NAME_CTR,                    PM_EFFECT_CONFIG__FADE);
  addEffect3(EFFECTS_FUNCTION__WLED_FADE_TRICOLOR__ID,         &mAnimatorLight::EffectAnim__Fade_TriColour,        PM_EFFECTS_FUNCTION__WLED_TRICOLOR_FADE__NAME_CTR,           PM_EFFECT_CONFIG__TRICOLOR_FADE);
  addEffect3(EFFECTS_FUNCTION__WLED_FADE_SPOTS__ID,            &mAnimatorLight::EffectAnim__Fade_Spots,            PM_EFFECTS_FUNCTION__WLED_SPOTS_FADE__NAME_CTR,              PM_EFFECT_CONFIG__SPOTS_FADE);
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  /**
   * Sparkle/Twinkle
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  addEffect3(EFFECTS_FUNCTION__WLED_SOLID_GLITTER__ID,   &mAnimatorLight::EffectAnim__Solid_Glitter,               PM_EFFECTS_FUNCTION__WLED_SOLID_GLITTER__NAME_CTR,           PM_EFFECT_CONFIG__SOLID_GLITTER);
  addEffect3(EFFECTS_FUNCTION__WLED_POPCORN__ID,         &mAnimatorLight::EffectAnim__Popcorn,                     PM_EFFECTS_FUNCTION__WLED_POPCORN__NAME_CTR,                 PM_EFFECT_CONFIG__POPCORN);
  addEffect3(EFFECTS_FUNCTION__WLED_PLASMA__ID,          &mAnimatorLight::EffectAnim__Plasma,                      PM_EFFECTS_FUNCTION__WLED_PLASMA__NAME_CTR,                  PM_EFFECT_CONFIG__PLASMA);
  addEffect3(EFFECTS_FUNCTION__WLED_SPARKLE__ID,         &mAnimatorLight::EffectAnim__Sparkle,                     PM_EFFECTS_FUNCTION__WLED_SPARKLE__NAME_CTR,                 PM_EFFECT_CONFIG__SPARKLE);
  addEffect3(EFFECTS_FUNCTION__WLED_FLASH_SPARKLE__ID,   &mAnimatorLight::EffectAnim__Sparkle_Flash,               PM_EFFECTS_FUNCTION__WLED_FLASH_SPARKLE__NAME_CTR,           PM_EFFECT_CONFIG__FLASH_SPARKLE);
  addEffect3(EFFECTS_FUNCTION__WLED_HYPER_SPARKLE__ID,   &mAnimatorLight::EffectAnim__Sparkle_Hyper,               PM_EFFECTS_FUNCTION__WLED_HYPER_SPARKLE__NAME_CTR,           PM_EFFECT_CONFIG__HYPER_SPARKLE);
  addEffect3(EFFECTS_FUNCTION__WLED_TWINKLE__ID,         &mAnimatorLight::EffectAnim__Twinkle,                     PM_EFFECTS_FUNCTION__WLED_TWINKLE__NAME_CTR,                 PM_EFFECT_CONFIG__TWINKLE);
  addEffect3(EFFECTS_FUNCTION__WLED_COLORTWINKLE__ID,    &mAnimatorLight::EffectAnim__Twinkle_Colour,              PM_EFFECTS_FUNCTION__WLED_COLORTWINKLE__NAME_CTR,            PM_EFFECT_CONFIG__COLORTWINKLE);
  addEffect3(EFFECTS_FUNCTION__WLED_TWINKLEFOX__ID,      &mAnimatorLight::EffectAnim__Twinkle_Fox,                 PM_EFFECTS_FUNCTION__WLED_TWINKLEFOX__NAME_CTR,              PM_EFFECT_CONFIG__TWINKLEFOX);
  addEffect3(EFFECTS_FUNCTION__WLED_TWINKLECAT__ID,      &mAnimatorLight::EffectAnim__Twinkle_Cat,                 PM_EFFECTS_FUNCTION__WLED_TWINKLECAT__NAME_CTR,              PM_EFFECT_CONFIG__TWINKLECAT);
  addEffect3(EFFECTS_FUNCTION__WLED_TWINKLEUP__ID,       &mAnimatorLight::EffectAnim__Twinkle_Up,                  PM_EFFECTS_FUNCTION__WLED_TWINKLEUP__NAME_CTR,               PM_EFFECT_CONFIG__TWINKLEUP);
  addEffect3(EFFECTS_FUNCTION__WLED_SAW__ID,             &mAnimatorLight::EffectAnim__Saw,                         PM_EFFECTS_FUNCTION__WLED_SAW__NAME_CTR,                     PM_EFFECT_CONFIG__SAW);
  addEffect3(EFFECTS_FUNCTION__WLED_DISSOLVE__ID,        &mAnimatorLight::EffectAnim__Dissolve,                    PM_EFFECTS_FUNCTION__WLED_DISSOLVE__NAME_CTR,                PM_EFFECT_CONFIG__DISSOLVE);
  addEffect3(EFFECTS_FUNCTION__WLED_DISSOLVE_RANDOM__ID, &mAnimatorLight::EffectAnim__Dissolve_Random,             PM_EFFECTS_FUNCTION__WLED_DISSOLVE_RANDOM__NAME_CTR,         PM_EFFECT_CONFIG__DISSOLVE_RANDOM);
  addEffect3(EFFECTS_FUNCTION__WLED_COLORFUL__ID,        &mAnimatorLight::EffectAnim__ColourFul,                   PM_EFFECTS_FUNCTION__WLED_COLORFUL__NAME_CTR,                PM_EFFECT_CONFIG__COLORFUL);
  addEffect3(EFFECTS_FUNCTION__WLED_TRAFFIC_LIGHT__ID,   &mAnimatorLight::EffectAnim__Traffic_Light,               PM_EFFECTS_FUNCTION__WLED_TRAFFIC_LIGHT__NAME_CTR,           PM_EFFECT_CONFIG__TRAFFIC_LIGHT);
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  /**
   * Blink/Strobe
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  addEffect3(EFFECTS_FUNCTION__WLED_BLINK__ID,           &mAnimatorLight::EffectAnim__Blink,                       PM_EFFECTS_FUNCTION__WLED_BLINK__NAME_CTR,                   PM_EFFECT_CONFIG__BLINK);
  addEffect3(EFFECTS_FUNCTION__WLED_BLINK_RAINBOW__ID,   &mAnimatorLight::EffectAnim__Blink_Rainbow,               PM_EFFECTS_FUNCTION__WLED_BLINK_RAINBOW__NAME_CTR,           PM_EFFECT_CONFIG__BLINK_RAINBOW);
  addEffect3(EFFECTS_FUNCTION__WLED_STROBE__ID,          &mAnimatorLight::EffectAnim__Strobe,                      PM_EFFECTS_FUNCTION__WLED_STROBE__NAME_CTR,                  PM_EFFECT_CONFIG__STROBE);
  addEffect3(EFFECTS_FUNCTION__WLED_MULTI_STROBE__ID,    &mAnimatorLight::EffectAnim__Strobe_Multi,                PM_EFFECTS_FUNCTION__WLED_MULTI_STROBE__NAME_CTR,            PM_EFFECT_CONFIG__MULTI_STROBE);
  addEffect3(EFFECTS_FUNCTION__WLED_STROBE_RAINBOW__ID,  &mAnimatorLight::EffectAnim__Strobe_Rainbow,              PM_EFFECTS_FUNCTION__WLED_STROBE_RAINBOW__NAME_CTR,          PM_EFFECT_CONFIG__STROBE_RAINBOW);
  addEffect3(EFFECTS_FUNCTION__WLED_RAINBOW__ID,         &mAnimatorLight::EffectAnim__Rainbow,                     PM_EFFECTS_FUNCTION__WLED_RAINBOW__NAME_CTR,                 PM_EFFECT_CONFIG__RAINBOW);
  addEffect3(EFFECTS_FUNCTION__WLED_LIGHTNING__ID,       &mAnimatorLight::EffectAnim__Lightning,                   PM_EFFECTS_FUNCTION__WLED_LIGHTNING__NAME_CTR,               PM_EFFECT_CONFIG__LIGHTNING);
  addEffect3(EFFECTS_FUNCTION__WLED_FIRE_2012__ID,       &mAnimatorLight::EffectAnim__Fire_2012,                   PM_EFFECTS_FUNCTION__WLED_FIRE_2012__NAME_CTR,               PM_EFFECT_CONFIG__FIRE_2012);
  addEffect3(EFFECTS_FUNCTION__WLED_RAILWAY__ID,         &mAnimatorLight::EffectAnim__Railway,                     PM_EFFECTS_FUNCTION__WLED_RAILWAY__NAME_CTR,                 PM_EFFECT_CONFIG__RAILWAY);
  addEffect3(EFFECTS_FUNCTION__WLED_HEARTBEAT__ID,       &mAnimatorLight::EffectAnim__Heartbeat,                   PM_EFFECTS_FUNCTION__WLED_HEARTBEAT__NAME_CTR,               PM_EFFECT_CONFIG__HEARTBEAT);
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  /**
   * Noise
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  addEffect3(EFFECTS_FUNCTION__WLED_FILLNOISE8__ID,      &mAnimatorLight::EffectAnim__FillNoise8,                  PM_EFFECTS_FUNCTION__WLED_FILLNOISE8__NAME_CTR,              PM_EFFECT_CONFIG__FILLNOISE8);
  addEffect3(EFFECTS_FUNCTION__WLED_NOISE16_1__ID,       &mAnimatorLight::EffectAnim__Noise16_1,                   PM_EFFECTS_FUNCTION__WLED_NOISE16_1__NAME_CTR,               PM_EFFECT_CONFIG__NOISE16_1);
  addEffect3(EFFECTS_FUNCTION__WLED_NOISE16_2__ID,       &mAnimatorLight::EffectAnim__Noise16_2,                   PM_EFFECTS_FUNCTION__WLED_NOISE16_2__NAME_CTR,               PM_EFFECT_CONFIG__NOISE16_2);
  addEffect3(EFFECTS_FUNCTION__WLED_NOISE16_3__ID,       &mAnimatorLight::EffectAnim__Noise16_3,                   PM_EFFECTS_FUNCTION__WLED_NOISE16_3__NAME_CTR,               PM_EFFECT_CONFIG__NOISE16_3);
  addEffect3(EFFECTS_FUNCTION__WLED_NOISE16_4__ID,       &mAnimatorLight::EffectAnim__Noise16_4,                   PM_EFFECTS_FUNCTION__WLED_NOISE16_4__NAME_CTR,               PM_EFFECT_CONFIG__NOISE16_4);
  addEffect3(EFFECTS_FUNCTION__WLED_NOISEPAL__ID,        &mAnimatorLight::EffectAnim__Noise_Pal,                   PM_EFFECTS_FUNCTION__WLED_NOISEPAL__NAME_CTR,                PM_EFFECT_CONFIG__NOISEPAL);
  addEffect3(EFFECTS_FUNCTION__WLED_PHASEDNOISE__ID,     &mAnimatorLight::EffectAnim__PhasedNoise,                 PM_EFFECTS_FUNCTION__WLED_PHASEDNOISE__NAME_CTR,             PM_EFFECT_CONFIG__PHASEDNOISE);
  addEffect3(EFFECTS_FUNCTION__WLED_PHASED__ID,          &mAnimatorLight::EffectAnim__Phased,                      PM_EFFECTS_FUNCTION__WLED_PHASED__NAME_CTR,                  PM_EFFECT_CONFIG__PHASED);
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  /**
   * Scan
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  addEffect3(EFFECTS_FUNCTION__WLED_SCAN__ID,                &mAnimatorLight::EffectAnim__Scan,                    PM_EFFECTS_FUNCTION__WLED_SCAN__NAME_CTR,                    PM_EFFECT_CONFIG__SCAN);
  addEffect3(EFFECTS_FUNCTION__WLED_DUAL_SCAN__ID,           &mAnimatorLight::EffectAnim__Scan_Dual,               PM_EFFECTS_FUNCTION__WLED_DUAL_SCAN__NAME_CTR,               PM_EFFECT_CONFIG__DUAL_SCAN);
  addEffect3(EFFECTS_FUNCTION__WLED_LARSON_SCANNER__ID,      &mAnimatorLight::EffectAnim__Larson_Scanner,          PM_EFFECTS_FUNCTION__WLED_LARSON_SCANNER__NAME_CTR,          PM_EFFECT_CONFIG__LARSON_SCANNER);
  addEffect3(EFFECTS_FUNCTION__WLED_DUAL_LARSON_SCANNER__ID, &mAnimatorLight::EffectAnim__Larson_Scanner_Dual,     PM_EFFECTS_FUNCTION__WLED_DUAL_LARSON_SCANNER__NAME_CTR,     PM_EFFECT_CONFIG__DUAL_LARSON_SCANNER);
  addEffect3(EFFECTS_FUNCTION__WLED_ICU__ID,                 &mAnimatorLight::EffectAnim__ICU,                     PM_EFFECTS_FUNCTION__WLED_ICU__NAME_CTR,                     PM_EFFECT_CONFIG__ICU);
  addEffect3(EFFECTS_FUNCTION__WLED_RIPPLE__ID,              &mAnimatorLight::EffectAnim__Ripple,                  PM_EFFECTS_FUNCTION__WLED_RIPPLE__NAME_CTR,                  PM_EFFECT_CONFIG__RIPPLE);
  addEffect3(EFFECTS_FUNCTION__WLED_RIPPLE_RAINBOW__ID,      &mAnimatorLight::EffectAnim__Ripple_Rainbow,          PM_EFFECTS_FUNCTION__WLED_RIPPLE_RAINBOW__NAME_CTR,          PM_EFFECT_CONFIG__RIPPLE_RAINBOW);
  addEffect3(EFFECTS_FUNCTION__WLED_COMET__ID,               &mAnimatorLight::EffectAnim__Comet,                   PM_EFFECTS_FUNCTION__WLED_COMET__NAME_CTR,                   PM_EFFECT_CONFIG__COMET);
  addEffect3(EFFECTS_FUNCTION__WLED_CHUNCHUN__ID,            &mAnimatorLight::EffectAnim__Chunchun,                PM_EFFECTS_FUNCTION__WLED_CHUNCHUN__NAME_CTR,                PM_EFFECT_CONFIG__CHUNCHUN);
  addEffect3(EFFECTS_FUNCTION__WLED_BOUNCINGBALLS__ID,       &mAnimatorLight::EffectAnim__Bouncing_Balls,          PM_EFFECTS_FUNCTION__WLED_BOUNCINGBALLS__NAME_CTR,           PM_EFFECT_CONFIG__BOUNCINGBALLS);
  addEffect3(EFFECTS_FUNCTION__WLED_SINELON__ID,             &mAnimatorLight::EffectAnim__Sinelon,                 PM_EFFECTS_FUNCTION__WLED_SINELON__NAME_CTR,                 PM_EFFECT_CONFIG__SINELON);
  addEffect3(EFFECTS_FUNCTION__WLED_SINELON_DUAL__ID,        &mAnimatorLight::EffectAnim__Sinelon_Dual,            PM_EFFECTS_FUNCTION__WLED_SINELON_DUAL__NAME_CTR,            PM_EFFECT_CONFIG__SINELON_DUAL);
  addEffect3(EFFECTS_FUNCTION__WLED_SINELON_RAINBOW__ID,     &mAnimatorLight::EffectAnim__Sinelon_Rainbow,         PM_EFFECTS_FUNCTION__WLED_SINELON_RAINBOW__NAME_CTR,         PM_EFFECT_CONFIG__SINELON_RAINBOW);
  addEffect3(EFFECTS_FUNCTION__WLED_DRIP__ID,                &mAnimatorLight::EffectAnim__Drip,                    PM_EFFECTS_FUNCTION__WLED_DRIP__NAME_CTR,                    PM_EFFECT_CONFIG__DRIP);
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
  case EFFECTS_FUNCTION__SUNPOSITION_ELEVATION_PALETTE_PROGRESS_STEP__ID:   //pick nearest
    EffectAnim__SunPositions_Elevation_Palette_Progress_Step();
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
  case EFFECTS_FUNCTION__LCD_DISPLAY_MANUAL_STRING_01__ID:
    SubTask_Flasher_Animate_LCD_Display_Show_String_01();
  break;
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK     
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS 
  case EFFECTS_FUNCTION__NOTIFICATION_STATIC__ID:
    SubTask_Segment_Animate_Function__Notification_Static();
  break;
  case EFFECTS_FUNCTION__NOTIFICATION_FADE__ID:
    SubTask_Segment_Animate_Function__Notification_Fade();
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
    EffectAnim__Static_Palette_New();
  break;
  case EFFECTS_FUNCTION__SLOW_GLOW_NEW__ID:
    EffectAnim__Slow_Glow_New();
  break;
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING
}

