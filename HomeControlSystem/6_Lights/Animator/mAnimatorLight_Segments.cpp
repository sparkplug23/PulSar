#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

#ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

void mAnimatorLight::resetSegments() 
{

  bool supportWhite=0;
  uint16_t countPixels=50;
  bool skipFirst=0;
  // mEffects->init(0, ledCount, 0);

  // if (supportWhite == _useRgbw && countPixels == _length && _skipFirstMode == skipFirst) return;
  // RESET_RUNTIME;

  _useRgbw = supportWhite;
  _length = countPixels;
  _skipFirstMode = skipFirst;

  uint8_t ty = 1;
  if (supportWhite) ty = 2;
  _lengthRaw = _length;
  // if (_skipFirstMode) {
  //   _lengthRaw += LED_SKIP_AMOUNT;
  // }

  // bus->Begin((NeoPixelType)ty, _lengthRaw);
  
  _segments[0].start_pixel_index = 0;
  _segments[0].stop_pixel_index = _length;

  // // mainSegment = 0;
  // memset(_segments, 0, sizeof(_segments));
  // //memset(_segment_runtimes, 0, sizeof(_segment_runtimes));
  _segment_index = 0;
  _segments[0].mode = DEFAULT_MODE;
  _segments[0].colors[0] = DEFAULT_COLOR;
  _segments[0].start_pixel_index = 0;
  _segments[0].speed = DEFAULT_SPEED;
  _segments[0].stop_pixel_index = _length;
  _segments[0].grouping = 1;
  _segments[0].setOption(SEG_OPTION_SELECTED, 1);
  _segments[0].setOption(SEG_OPTION_ON, 1);
  _segments[0].opacity = 255;
  _segments[0].intensity = 127;
  _segment_runtimes[0].reset();

  for (uint16_t i = 1; i < MAX_NUM_SEGMENTS; i++)
  {
    _segments[i].colors[0] = 1000;//color_wheel(i*51);
    _segments[i].grouping = 1;
    _segments[i].setOption(SEG_OPTION_ON, 1);
    _segments[i].opacity = 255;
    // _segment_runtimes[i].reset();
  }
  
  _segments[0].start_pixel_index = 0;
  _segments[0].stop_pixel_index = 4;
  _segments[1].start_pixel_index = 5;
  _segments[1].stop_pixel_index = 9;
  _segments[2].start_pixel_index = 10;
  _segments[2].stop_pixel_index = 14;
  _segments[0].colors[0] = 0x00FF0000;
  _segments[1].colors[0] = 0x0000FF00;
  _segments[2].colors[0] = 0x000000FF;

  
  // _segments[0].colors[0] = random(0,10)*3000;
  // _segments[1].colors[0] = random(0,10)*3000;
  // _segments[0].colors[0] = random(0,10)*3000;
  // _segments[1].colors[0] = random(0,10)*3000;
  _segments[0].palette = 1;//random(0,35);
  _segments[1].palette = 1;//random(0,35);
  _segments[2].palette = 1;//random(0,35);
  /**
   * default forced values
   * */
  // _segments[0].start_pixel_index = 0;
  // _segments[0].stop_pixel_index = 50;

  _segment_runtimes[0].animator = new NeoPixelAnimator(10, NEO_MILLISECONDS);
  _segment_runtimes[1].animator = new NeoPixelAnimator(10, NEO_MILLISECONDS);
  _segment_runtimes[2].animator = new NeoPixelAnimator(10, NEO_MILLISECONDS);
  


}



/****
 * Changes pixels randomly to new colour, with slow blending
 * Requires new colour calculation each call
 */
void mAnimatorLight::Segment_SubTask_Flasher_Animate_Function__Slow_Glow(){

  // this should probably force order as random, then introduce static "inorder" option
  pCONT_iLight->animation.transition.order_id = TRANSITION_ORDER_RANDOM_ID;
  
  // So colour region does not need to change each loop to prevent colour crushing
  pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
  // Pick new colours
  UpdateDesiredColourFromPaletteSelected_Segments();
  // Check if output multiplying has been set, if so, change desiredcolour array
  // OverwriteUpdateDesiredColourIfMultiplierIsEnabled();
  // Get starting positions already on show
  UpdateStartingColourWithGetPixel_Segments();
  // Call the animator to blend from previous to new
  setAnimFunctionCallback([this](const AnimationParam& param){
    this->AnimationProcess_Generic_AnimationColour_LinearBlend_Segments(param);
  });


}

void mAnimatorLight::AnimationProcess_Generic_AnimationColour_LinearBlend_Segments(const AnimationParam& param)
{    
  
  uint16_t start_pixel = _segments[_segment_index].start_pixel_index;
  uint16_t end_pixel = _segments[_segment_index].stop_pixel_index;


  for (uint16_t pixel = start_pixel; pixel <= end_pixel; pixel++)
  {
    RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
        animation_colours[pixel].StartingColor,
        animation_colours[pixel].DesiredColour,
        param.progress);    
    SetPixelColor(pixel, updatedColor);
  }
  
}

void mAnimatorLight::UpdateStartingColourWithGetPixel_Segments(){

  
  uint16_t start_pixel = _segments[_segment_index].start_pixel_index;
  uint16_t end_pixel = _segments[_segment_index].stop_pixel_index;

  // AddLog(LOG_LEVEL_TEST, PSTR("start/end = %d|%d"),start_pixel,end_pixel);

  for (uint16_t pixel = start_pixel; pixel <= end_pixel; pixel++){

    // Test fix, if desired does not match getcolour, then use ...
    animation_colours[pixel].StartingColor = GetPixelColor(pixel);

  }

}


/***
 * Test 1: Apply 3 static colours between 0-4, 5-9 and 10 to 14
 * 
 * */
void mAnimatorLight::SubTask_Segments_Animation()
{
  
  uint8_t flag_animations_needing_updated = 0;

  /**
   * each animator needs to know what its pixel range to apply to bus is, though, this is probably handled in an blend function
   * */

  for (int i =0;i<MAX_NUM_SEGMENTS;i++)
  {
    if (_segment_runtimes[i].animator->IsAnimating()){
      _segment_runtimes[i].animator->UpdateAnimations();
      flag_animations_needing_updated++; // channels needing updated
    }
  }

  if(flag_animations_needing_updated)
  {
    if(stripbus->IsDirty()){
      if(stripbus->CanShow()){ 
        StripUpdate();
      }
    }
  }
  
  if(mTime::TimeReached(&tSaved_Test_Segment_Animation, 1000))
  {
      
      _segment_index = 0;
      
        AddLog(LOG_LEVEL_TEST, PSTR("Segment_SubTask_Flasher_Animate_Function__Slow_Glow %d"),millis());
      // case EFFECTS_FUNCTION_SLOW_GLOW_ID:
        Segment_SubTask_Flasher_Animate_Function__Slow_Glow();
      // break;
      

      // DEBUG_PRINT_FUNCTION_NAME_TEST;
      if(anim_function_callback){
        _segment_runtimes[0].animator->StartAnimation(0, 600, anim_function_callback );
      }



  
  }


 
  // uint32_t nowUp = millis(); // Be aware, millis() rolls over every 49 days
  // now = nowUp + timebase;
  // if (nowUp - _lastShow < MIN_SHOW_DELAY) {
  //   return;  
  // }
  // bool doShow = false;

  // for(uint8_t i = 0; i < MAX_NUM_SEGMENTS; i++)
  // {
  //   _segment_index = i;
  //   if (_segments[_segment_index].isActive())
  //   {
  //     if(
  //       (nowUp > _segment_runtimes[_segment_index].next_time) || _triggered || 
  //       (doShow && _segments[_segment_index].mode == 0)
  //     ) //last is temporary
  //     {

  //       // AddLog(LOG_LEVEL_TEST, PSTR("SubTask_Segments_Animation %d"),millis());

  //       if (_segments[_segment_index].grouping == 0) _segments[_segment_index].grouping = 1; //sanity check

  //       _virtualSegmentLength = _segments[_segment_index].virtualLength();
  //       doShow = true;

  //       // "handlepalette" moved inside effects
        
  //       /**
  //        * Animation call (using ptr), then return delay time until next call
  //        * */
  //       // uint16_t delay = (this->*_mode[_segments[_segment_index].mode])();
  //       // uint16_t delay = mode_static();
  //       // uint16_t delay = mode_static_pattern();
  //       uint16_t delay = 1000;//mode_static();

  //       // if(i==0)
  //       // {
  //         //delay = seg_mode_static();
  //       // }else{
  //       //   delay = mode_static_pattern();
  //       // }

  //       // AddLog(LOG_LEVEL_TEST, PSTR("delay %d"),delay);
  //       _segment_runtimes[_segment_index].next_time = nowUp + delay;

  //       /**
  //        *  Used as progress counter for animations eg rainbow across all hues
  //        * */
  //       _segment_runtimes[_segment_index].call++; // Used as progress counter for animations eg rainbow across all hues

  //     }
  //   }
  // }

  // _virtualSegmentLength = 0;

  // if(doShow) {
  //   yield();
  //   showWLED();
  // }
  // _triggered = false;



//handleTransitions()
// {
  
//   if (transitionActive && transitionDelayTemp > 0)
//   {
//     float tper = (millis() - transitionStartTime)/(float)transitionDelayTemp;
//     if (tper >= 1.0)
//     {
//       mEffects->setTransitionMode(false);
//       transitionActive = false;
//       tperLast = 0;
//       setLedsStandard();
//       return;
//     }
//     if (tper - tperLast < 0.004) return;
//     tperLast = tper;
//     for (byte i=0; i<4; i++)
//     {
//       colT[i] = colOld[i]+((col[i] - colOld[i])*tper);
//       colSecT[i] = colSecOld[i]+((colSec[i] - colSecOld[i])*tper);
//     }
//     briT    = briOld   +((bri    - briOld   )*tper);
    
//     setAllLeds();
//   }
// }


} // SubTask_Effects_PhaseOut




    #ifdef USE_DEVFEATURE_WLED_METHOD_ORIGINAL_ADDED_AS_EFFECT
/*
 * FastLED palette modes helper function. Limitation: Due to memory reasons, multiple active segments with FastLED will disable the Palette transitions
 */
void mAnimatorLight::handle_palette(void)
{
  bool singleSegmentMode = (_segment_index == _segment_index_palette_last);
  _segment_index_palette_last = _segment_index;

  byte paletteIndex = _segments[_segment_index].palette;
  if (paletteIndex == 0) //default palette. Differs depending on effect
  {
    switch (_segments[_segment_index].mode)
    {
      case FX_MODE_FIRE_2012  : paletteIndex = 35; break; //heat palette
      case FX_MODE_COLORWAVES : paletteIndex = 26; break; //landscape 33
      case FX_MODE_FILLNOISE8 : paletteIndex =  9; break; //ocean colors
      case FX_MODE_NOISE16_1  : paletteIndex = 20; break; //Drywet
      case FX_MODE_NOISE16_2  : paletteIndex = 43; break; //Blue cyan yellow
      case FX_MODE_NOISE16_3  : paletteIndex = 35; break; //heat palette
      case FX_MODE_NOISE16_4  : paletteIndex = 26; break; //landscape 33
      case FX_MODE_GLITTER    : paletteIndex = 11; break; //rainbow colors
      case FX_MODE_SUNRISE    : paletteIndex = 35; break; //heat palette
      case FX_MODE_FLOW       : paletteIndex =  6; break; //party
    }
  }
  if (_segments[_segment_index].mode >= FX_MODE_METEOR && paletteIndex == 0) paletteIndex = 4;
   
  // paletteIndex = 43;
  //Serial.printf("_segments[_segment_index].palette %d %d\n\r",_segments[_segment_index].palette, paletteIndex);

  switch (paletteIndex)
  {
    case 0: //default palette. Exceptions for specific effects above
      targetPalette = PartyColors_p; 
      
  //Serial.printf( "targetPalette = PartyColors_p \n\r");
      
      break;
    case 1: {//periodically replace palette with a random one. Doesn't work with multiple FastLED segments
      if (!singleSegmentMode)
      {
        targetPalette = PartyColors_p; break; //fallback
      }
      if (millis() - _lastPaletteChange > 1000 + ((uint32_t)(255-_segments[_segment_index].intensity))*100)
      {
        targetPalette = CRGBPalette16(
                        CHSV(random8(), 255, random8(128, 255)),
                        CHSV(random8(), 255, random8(128, 255)),
                        CHSV(random8(), 192, random8(128, 255)),
                        CHSV(random8(), 255, random8(128, 255)));
        _lastPaletteChange = millis();
      } break;}
    case 2: {//primary color only
      CRGB prim = pCONT_lAni->mEffects->col_to_crgb(SEGCOLOR(0));
      targetPalette = CRGBPalette16(prim); break;}
    case 3: {//primary + secondary
      CRGB prim = pCONT_lAni->mEffects->col_to_crgb(SEGCOLOR(0));
      CRGB sec  = pCONT_lAni->mEffects->col_to_crgb(SEGCOLOR(1));
      targetPalette = CRGBPalette16(prim,prim,sec,sec); break;}
    case 4: {//primary + secondary + tertiary
      CRGB prim = pCONT_lAni->mEffects->col_to_crgb(SEGCOLOR(0));
      CRGB sec  = pCONT_lAni->mEffects->col_to_crgb(SEGCOLOR(1));
      CRGB ter  = pCONT_lAni->mEffects->col_to_crgb(SEGCOLOR(2));
      targetPalette = CRGBPalette16(ter,sec,prim); break;}
    case 5: {//primary + secondary (+tert if not off), more distinct
      CRGB prim = pCONT_lAni->mEffects->col_to_crgb(SEGCOLOR(0));
      CRGB sec  = pCONT_lAni->mEffects->col_to_crgb(SEGCOLOR(1));
      if (SEGCOLOR(2)) {
        CRGB ter = pCONT_lAni->mEffects->col_to_crgb(SEGCOLOR(2));
        targetPalette = CRGBPalette16(prim,prim,prim,prim,prim,sec,sec,sec,sec,sec,ter,ter,ter,ter,ter,prim);
      } else {
        targetPalette = CRGBPalette16(prim,prim,prim,prim,prim,prim,prim,prim,sec,sec,sec,sec,sec,sec,sec,sec);
      }
      break;}
    case 6: //Party colors
      targetPalette = PartyColors_p; break;
    case 7: //Cloud colors
      targetPalette = CloudColors_p; break;
    case 8: //Lava colors
      targetPalette = LavaColors_p; break;
    case 9: //Ocean colors
      targetPalette = OceanColors_p; break;
    case 10: //Forest colors
      targetPalette = ForestColors_p; break;
    case 11: //Rainbow colors
      targetPalette = RainbowColors_p; break;
    case 12: //Rainbow stripe colors
      targetPalette = RainbowStripeColors_p; break;
    default: //progmem palettes
      pCONT_lAni->mEffects->load_gradient_palette(paletteIndex -13);
  }
  
  if (singleSegmentMode && paletteFade) //only blend if just one segment uses FastLED mode
  {
    nblendPaletteTowardPalette(currentPalette, targetPalette, 48);
  } else
  {
    currentPalette = targetPalette;
  }
}

    #endif // USE_DEVFEATURE_WLED_METHOD_ORIGINAL_ADDED_AS_EFFECT



// /****************************************************************************************************************************
//  **************************************************************************************************************************** 
//  * Static
//  ****************************************************************************************************************************
//  ****************************************************************************************************************************/



// /*
//  * No blinking. Just plain old static light.
//  */
// uint16_t mAnimatorLight::mode_static(void)
// {
//   fill(SEGCOLOR2(0));
//   return (_segments[_segment_index].getOption(SEG_OPTION_TRANSITIONAL)) ? FRAMETIME : 500; //update faster if in transition
// }

// //Speed slider sets amount of LEDs lit, intensity sets unlit
// uint16_t mAnimatorLight::mode_static_pattern()
// {
//   uint16_t lit = 1 + _segments[_segment_index].speed;
//   uint16_t unlit = 1 + _segments[_segment_index].intensity;
//   bool drawingLit = true;
//   uint16_t cnt = 0;

//   for (uint16_t i = 0; i < SEGLEN; i++) {
//     pCONT_lAni->mEffects->setPixelColor(i, 
//       (drawingLit) ? 
//       pCONT_lAni->mEffects->color_from_palette(i, true, PALETTE_SOLID_WRAP, 0) : 
//       SEGCOLOR2(1)
//     );
//     cnt++;
//     if (cnt >= ((drawingLit) ? lit : unlit)) {
//       cnt = 0;
//       drawingLit = !drawingLit;
//     }
//   }
  
//   return FRAMETIME;
// }


// uint16_t mAnimatorLight::mode_tri_static_pattern()
// {
//   uint8_t segSize = (_segments[_segment_index].intensity >> 5) +1;
//   uint8_t currSeg = 0;
//   uint16_t currSegCount = 0;

//   for (uint16_t i = 0; i < SEGLEN; i++) {
//     if ( currSeg % 3 == 0 ) {
//       pCONT_lAni->mEffects->setPixelColor(i, SEGCOLOR(0));
//     } else if( currSeg % 3 == 1) {
//       pCONT_lAni->mEffects->setPixelColor(i, SEGCOLOR(1));
//     } else {
//       pCONT_lAni->mEffects->setPixelColor(i, (SEGCOLOR(2) > 0 ? SEGCOLOR(2) : WHITE));
//     }
//     currSegCount += 1;
//     if (currSegCount >= segSize) {
//       currSeg +=1;
//       currSegCount = 0;
//     }
//   }

//   return FRAMETIME;
// }

// /****************************************************************************************************************************
//  **************************************************************************************************************************** 
//  * color_wipe
//  ****************************************************************************************************************************
//  ****************************************************************************************************************************/

// /*
//  * Color wipe function
//  * LEDs are turned on (color1) in sequence, then turned off (color2) in sequence.
//  * if (bool rev == true) then LEDs are turned off in reverse order
//  */
// uint16_t mAnimatorLight::color_wipe(bool rev, bool useRandomColors) 
// {

//   /**
//    * Generate new colours
//    * */
//   handle_palette();
  
//   uint32_t cycleTime = 750 + (255 - _segments[_segment_index].speed)*150;
//   uint32_t perc = now % cycleTime;
//   uint16_t prog = (perc * 65535) / cycleTime;
//   bool back = (prog > 32767);
//   if (back) {
//     prog -= 32767;
//     if (_segment_runtimes[_segment_index].step == 0) _segment_runtimes[_segment_index].step = 1;
//   } else {
//     if (_segment_runtimes[_segment_index].step == 2) _segment_runtimes[_segment_index].step = 3; //trigger color change
//   }

//   if (useRandomColors) {
//     if (_segment_runtimes[_segment_index].call == 0) {
//       _segment_runtimes[_segment_index].aux0 = random8();
//       _segment_runtimes[_segment_index].step = 3;
//     }
//     if (_segment_runtimes[_segment_index].step == 1) { //if flag set, change to new random color
//       _segment_runtimes[_segment_index].aux1 = pCONT_lAni->mEffects->get_random_wheel_index(_segment_runtimes[_segment_index].aux0);
//       _segment_runtimes[_segment_index].step = 2;
//     }
//     if (_segment_runtimes[_segment_index].step == 3) {
//       _segment_runtimes[_segment_index].aux0 = pCONT_lAni->mEffects->get_random_wheel_index(_segment_runtimes[_segment_index].aux1);
//       _segment_runtimes[_segment_index].step = 0;
//     }
//   }

//   uint16_t ledIndex = (prog * SEGLEN) >> 15;
//   uint16_t rem = 0;
//   rem = (prog * SEGLEN) * 2; //mod 0xFFFF
//   rem /= (_segments[_segment_index].intensity +1);
//   if (rem > 255) rem = 255;

//   uint32_t col1 = useRandomColors ? pCONT_lAni->mEffects->color_wheel(_segment_runtimes[_segment_index].aux1) : SEGCOLOR(1);
//   for (uint16_t i = 0; i < SEGLEN; i++)
//   {
//     uint16_t index = (rev && back)? SEGLEN -1 -i : i;
//     uint32_t col0 = useRandomColors? pCONT_lAni->mEffects->color_wheel(_segment_runtimes[_segment_index].aux0) : pCONT_lAni->mEffects->color_from_palette(index, true, PALETTE_SOLID_WRAP, 0);
    
//     if (i < ledIndex) 
//     {
//       pCONT_lAni->mEffects->setPixelColor(index, back? col1 : col0);
//     } else
//     {
//       pCONT_lAni->mEffects->setPixelColor(index, back? col0 : col1);
//       if (i == ledIndex) pCONT_lAni->mEffects->setPixelColor(index, pCONT_lAni->mEffects->color_blend(back? col0 : col1, back? col1 : col0, rem));
//     }
//   } 
//   return FRAMETIME;

// }


// /*
//  * Lights all LEDs one after another.
//  */
// uint16_t mAnimatorLight::mode_color_wipe(void) {
//   return color_wipe(false, false);
// }

// /*
//  * Turns all LEDs after each other to a random color.
//  * Then starts over with another color.
//  */
// uint16_t mAnimatorLight::mode_color_wipe_random(void) {
//   return color_wipe(false, true);
// }

// /*
//  * Lights all LEDs one after another. Turns off opposite
//  */
// uint16_t mAnimatorLight::mode_color_sweep(void) {
//   return color_wipe(true, false);
// }


// /*
//  * Random color introduced alternating from start and end of strip->
//  */
// uint16_t mAnimatorLight::mode_color_sweep_random(void) {
//   return color_wipe(true, true);
// }


// Name:UpdateDesiredColourFromPaletteSelected
// Task: 
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
void mAnimatorLight::UpdateDesiredColourFromPaletteSelected_Segments(void){

  // Update pointer of struct
  mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);

  // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "UpdateDesiredColourFromPaletteSelected fMapIDs_Type %d"),mPaletteI->palettelist.ptr->flags.fMapIDs_Type);// \"%s\""),GetPaletteFriendlyName());
  // AddLog(LOG_LEVEL_DEBUG_MORE
  
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

     // SetLEDOutAmountByPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val);

      //what is this?
      // does this only run if above did not? ie this is default?

      //AddLog(LOG_LEVEL_TEST, PSTR("strip_size_requiring_update=%d"),strip_size_requiring_update);
      
      // Other types with random led patterns, or,
      
      switch(pCONT_iLight->animation.transition.order_id){
        case TRANSITION_ORDER_RANDOM_ID:{

          // new transition, so force full update
          if(pCONT_iLight->animation.flags.NewAnimationRequiringCompleteRefresh){
            strip_size_requiring_update = STRIP_SIZE_MAX;
          }
        

          RefreshLEDIndexPattern();
          int16_t pixel_position = -2;
          for(ledout.index=0;ledout.index<strip_size_requiring_update;ledout.index++){

            // For random, desired pixel from map will also be random
            desired_pixel = random(0,mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr));
            
            RgbTypeColor colour = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel,&pixel_position);

            // desired_colour[ledout.pattern[ledout.index]] = colour; 
            // AddLog(LOG_LEVEL_TEST, PSTR(DEBUG_INSERT_PAGE_BREAK "colour=%d,%d,%d"),
            // colour.R,
            // colour.G,
            // colour.B);

            animation_colours[ledout.pattern[ledout.index]].DesiredColour = colour;

            #ifndef ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS
            if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
              animation_colours[ledout.pattern[ledout.index]].DesiredColour = ApplyBrightnesstoDesiredColourWithGamma(animation_colours[ledout.pattern[ledout.index]].DesiredColour,pCONT_iLight->getBriRGB_Global());
            }
            #endif // ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS

            // #ifdef ENABLE_DEBUG_MODULE_LIGHTS_ADDRESSABLE
            // AddLog(LOG_LEVEL_INFO, PSTR("colour=%d,%d,%d brt_set=%d"),
            //   animation_colours[ledout.pattern[ledout.index]].DesiredColour.R,
            //   animation_colours[ledout.pattern[ledout.index]].DesiredColour.G,
            //   animation_colours[ledout.pattern[ledout.index]].DesiredColour.B,
            //   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation
            // );
            // #endif 
          } //end for

        }break;
        case TRANSITION_ORDER_INORDER_ID:{
          // new transition, so force full update
          if(pCONT_iLight->animation.flags.NewAnimationRequiringCompleteRefresh){
            strip_size_requiring_update = STRIP_SIZE_MAX;
          }
          RefreshLEDIndexPattern();
          int16_t pixel_position = -2;
          desired_pixel=0;
          for(ledout.index=0;ledout.index<strip_size_requiring_update;ledout.index++){
            RgbcctColor colour = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel,&pixel_position);

            // RgbTypeColor colour = colourcct;
            // desired_colour[ledout.pattern[ledout.index]] = colour; 
            // AddLog(LOG_LEVEL_TEST, PSTR("colour=%d,%d,%d"),
            // colour.R,
            // colour.G,
            // colour.B); 
            
            // AddLog(LOG_LEVEL_TEST, PSTR( "%d colour=%d,%d,%d"),desired_pixel, // DEBUG_INSERT_PAGE_BREAK
            // colour.R,
            // colour.G,
            // colour.B);

            // animation_colours[ledout.pattern[ledout.index]].DesiredColour = ApplyBrightnesstoDesiredColour(colour,pCONT_iLight->getBriRGB());
              // RgbColor colourbefore = animation_colours[ledout.pattern[ledout.index]].DesiredColour;
            animation_colours[ledout.pattern[ledout.index]].DesiredColour = colour;
           #ifndef ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS
            if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
              animation_colours[ledout.pattern[ledout.index]].DesiredColour = ApplyBrightnesstoDesiredColourWithGamma(animation_colours[ledout.pattern[ledout.index]].DesiredColour, pCONT_iLight->getBriRGB_Global());
            }
            #endif //             ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS
// RgbColor colourafter = animation_colours[ledout.pattern[ledout.index]].DesiredColour;
// if(ledout.index<5){
//             RgbColor colour2 = colour;
//             AddLog(LOG_LEVEL_TEST, PSTR( "%d colourbefore colour=%d,%d,%d     %d,%d,%d"), // DEBUG_INSERT_PAGE_BREAK
//             ledout.index,colourbefore.R,colourbefore.G, colourbefore.B,
//             colourafter.R,
//             colourafter.G,
//             colourafter.B);
// }
            //   AddLog(LOG_LEVEL_TEST, PSTR("colou2=%d,%d,%d"),
            // animation_colours[ledout.pattern[ledout.index]].DesiredColour.R,
            // animation_colours[ledout.pattern[ledout.index]].DesiredColour.G,
            // animation_colours[ledout.pattern[ledout.index]].DesiredColour.B);

            
            #ifdef ENABLE_LOG_LEVEL_DEBUG
            // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "colour[p%d:d%d] = %d,%d,%d %d pp%d"),
            //   ledout.pattern[ledout.index],desired_pixel,
            //   pCONT_iLight->HueF2N(desired_colour[ledout.pattern[ledout.index]].H),pCONT_iLight->SatF2N(desired_colour[ledout.pattern[ledout.index]].S),pCONT_iLight->BrtF2N(desired_colour[ledout.pattern[ledout.index]].B),
            //   mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr),pixel_position
            // );
            #endif
            // AddLog(LOG_LEVEL_INFO, PSTR("colour=%d,%d,%d brt_set=%d"),
            //   animation_colours[ledout.pattern[ledout.index]].DesiredColour.R,
            //   animation_colours[ledout.pattern[ledout.index]].DesiredColour.G,
            //   animation_colours[ledout.pattern[ledout.index]].DesiredColour.B,
            //   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation
            // );

            if(++desired_pixel>=mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr)){desired_pixel=0;}
          } //end for
        }break;
        // case TRANSITION_ORDER_FIXED_ID:

        //   int16_t pixel_position = -2;
        //   //move across all encoded values
        //     // HsbColor colour = GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel,&pixel_position);

        //     // if(pixel_position>=0){
        //     //   desired_colour[pixel_position] = colour;
        //     // }else{
        //     //   desired_colour[ledout.pattern[ledout.index]] = colour;
        //     // }
            
        //     // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "colour[p%d:d%d] = %d,%d,%d %d pp%d"),
        //     //   ledout.pattern[ledout.index],desired_pixel,
        //     //   HueF2N(desired_colour[ledout.pattern[ledout.index]].H),SatF2N(desired_colour[ledout.pattern[ledout.index]].S),BrtF2N(desired_colour[ledout.pattern[ledout.index]].B),
        //     //   mPaletteI->palettelist.ptr->active_pixels_in_map,pixel_position
        //     // );    

        // break;
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

      for(uint8_t grad_pair_index=0;grad_pair_index<pixels_in_map-1;grad_pair_index++){
        
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

        switch(mPaletteI->palettelist.ptr->flags.fIndexs_Type){
          case INDEX_TYPE_DIRECT: break; //remains the same
          case INDEX_TYPE_SCALED_255: 
            start_pixel_position = map(start_pixel_position,0,255,0,ledout.length);
            end_pixel_position   = map(end_pixel_position,0,255,0,ledout.length);
            // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "255 %d %d"),start_pixel_position,end_pixel_position);
          break;
          case INDEX_TYPE_SCALED_100: 
            start_pixel_position = map(start_pixel_position,0,100,0,ledout.length);
            end_pixel_position   = map(end_pixel_position,0,100,0,ledout.length);          
          break;
        }


    
      // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "%d start_pixel_position %d"),grad_pair_index,start_pixel_position);
      // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "end_pixel_position %d"),end_pixel_position);


        float progress = 0;
        for(ledout.index=start_pixel_position;ledout.index<end_pixel_position;ledout.index++){
          progress = mSupport::mapfloat(ledout.index,start_pixel_position,end_pixel_position,0,1);
          animation_colours[ledout.index].DesiredColour = RgbcctColor::LinearBlend(start_colour, end_colour, progress);
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

  //  AddLog(LOG_LEVEL_DEBUG, PSTR("colour=%d,%d,%d"),
  //             animation_colours[0].DesiredColour.R,
  //             animation_colours[0].DesiredColour.G,
  //             animation_colours[0].DesiredColour.B
  //           );

  
  // Handle brightness levels == if not set, use default
  //if(!mPaletteI->palettelist.ptr->fOverride_animation_brightness){
  // ApplyBrightnesstoDesiredColour(pCONT_iLight->getBriRGB());
  //}

} //end function UpdateDesiredColourFromPaletteSelected();




// //fine tune power estimation constants for your setup                  
// #define MA_FOR_ESP        100 //how much mA does the ESP use (Wemos D1 about 80mA, ESP32 about 120mA)
//                               //you can set it to 0 if the ESP is powered by USB and the LEDs by external

void mAnimatorLight::showWLED(void) 
{
    
  //power limit calculation
  //each LED can draw up 195075 "power units" (approx. 53mA)
  //one PU is the power it takes to have 1 channel 1 step brighter per brightness step
  //so A=2,R=255,G=0,B=0 would use 510 PU per LED (1mA is about 3700 PU)
  // bool useWackyWS2815PowerModel = false;
  // byte actualMilliampsPerLed = milliampsPerLed;

  // if(milliampsPerLed == 255) {
  //   useWackyWS2815PowerModel = true;
  //   actualMilliampsPerLed = 12; // from testing an actual strip
  // }
  
  pCONT_lAni->stripbus->Show();
  _lastShow = millis();

}



#endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

#endif //USE_MODULE_LIGHTS_ANIMATOR




