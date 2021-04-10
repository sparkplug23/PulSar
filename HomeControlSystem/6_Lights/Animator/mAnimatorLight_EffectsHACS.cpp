#include "mAnimatorLight.h"

#ifndef flasherfunction_h
#define flasherfunction_h

#ifdef USE_MODULE_LIGHTS_ANIMATOR


#ifdef ENABLE_PIXEL_FUNCTION_EFFECTS



void mAnimatorLight::SubTask_Flasher_Animate_Function_Slow_Glow_On_Brightness(){

  pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = false;

  flashersettings.flag_finish_flasher_pair = false;
  flashersettings.flags.enable_random_rate = true;

  // Best "inorder" to compute colour again then brightness
  
  do{ //must complete the pair together //move inside functions
    switch(flashersettings.region){
      case EFFECTS_REGION_COLOUR_SELECT_ID: //set colours
        #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
        AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
        #endif
        UpdateDesiredColourFromPaletteSelected();
        flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
      break;
      case EFFECTS_REGION_ANIMATE_ID: //shift along
        #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
        //AddLog(LOG_LEVEL_DEBUG_MORE_LOWLEVEL,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
        #endif
        // RotateDesiredColour(1,flashersettings.flags.movement_direction);

        // Calculate new pattern to change
        RefreshLEDIndexPattern();

        // Global brightness is already applied, and will be known as "max range"
        // Min range will be another map change here
        uint8_t max_brightness = pCONT_iLight->rgbcct_controller.getBrightnessRGB255();
        uint8_t min_brightness = flashersettings.brightness_min;
        uint8_t random_brightness = 0;

        if(min_brightness > max_brightness){
          min_brightness = max_brightness;
        }

        // Change only selected pixel brightness levels
        for(ledout.index=0;ledout.index<strip_size_requiring_update;ledout.index++){
          random_brightness = random(min_brightness,max_brightness);          
          animation_colours[ledout.pattern[ledout.index]].DesiredColour = ApplyBrightnesstoRgbcctColour(
                                                                              animation_colours[ledout.pattern[ledout.index]].DesiredColour,
                                                                              random_brightness);
        }

        if(flashersettings.flags.force_finish_flasher_pair_once){
          AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("flashersettings.flags.force_finish_flasher_pair_once WAS SET"));
          flashersettings.flags.force_finish_flasher_pair_once = false;
        }
      break;
    }
  }while(flashersettings.flag_finish_flasher_pair || flashersettings.flags.force_finish_flasher_pair_once);


}


// Fade solid colour from 0 to 75%, and a palette from 25 to 100% (50% overlap)
void mAnimatorLight::SubTask_Flasher_Animate_Function_Fade_Gradient(){
    
#ifdef ENABLE_EFFECTS_ANIMATE_FUNCTION_FADE_GRADIENT

  flashersettings.flag_finish_flasher_pair = false;
  flashersettings.flags.enable_random_rate = true;
  
  // Apply green gradient, brightest at start

  uint16_t start = pCONT_iLight->settings.light_size_count/2;
  uint16_t end = pCONT_iLight->settings.light_size_count; 
  RgbTypeColor colour_gradient = HsbColor(
                                          pCONT_iLight->HueN2F(120),
                                          pCONT_iLight->SatN2F(100),
                                          pCONT_iLight->BrtN2F(map(pCONT_iLight->getBriRGB(),0,255,0,100))  
                                          );
  RgbTypeColor colour_random = RgbTypeColor(255,0,0); 
  HsbColor colour_random_adjusted = HsbColor(RgbColor(0));
  uint8_t gradient_end_percentage = 75;
  uint16_t strip_size_gradient = pCONT_iLight->settings.light_size_count*(gradient_end_percentage/100.0f);
  uint16_t strip_size_single   = pCONT_iLight->settings.light_size_count*(75/100.0f);
  
  start = 0;
  end = pCONT_iLight->settings.light_size_count;
  for(ledout.index=start;ledout.index<end;ledout.index++){ 
    animation_colours[ledout.index].DesiredColour = RgbTypeColor(0);
  }
  
  //0 to 75% 
  start = 0;
  end = map(75,0,100,0,pCONT_iLight->settings.light_size_count);
  for(ledout.index=start;ledout.index<end;ledout.index++){ 
    animation_colours[ledout.index].DesiredColour.R = pCONT_iLight->ledGamma(map(ledout.index,start,end,colour_gradient.R,0));
    animation_colours[ledout.index].DesiredColour.G = pCONT_iLight->ledGamma(map(ledout.index,start,end,colour_gradient.G,0));
    animation_colours[ledout.index].DesiredColour.B = pCONT_iLight->ledGamma(map(ledout.index,start,end,colour_gradient.B,0));
  }

  mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
  uint8_t pixels = mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr);
  uint8_t desired_pixel;
  
  // 25 to 100%
  start = map(25,0,100,0,pCONT_iLight->settings.light_size_count);
  end = pCONT_iLight->settings.light_size_count;
  for(ledout.index=start;ledout.index<end;ledout.index++){ 
    desired_pixel = random(0,pixels-1);
    colour_random = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel);
    if((ledout.index%3)==0){
      colour_random_adjusted = RgbTypeColor(colour_random);
      colour_random_adjusted.B = pCONT_iLight->BrtN2F(map(pCONT_iLight->getBriRGB(),0,255,0,100));
      animation_colours[ledout.index].DesiredColour = colour_random_adjusted;
    }
  }

  #endif

}


/**************************************************************************************************************************************************************
***************************************************************************************************************************************************************
********* Function_Slow_Glow *************************************************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************/


/****
 * Changes pixels randomly to new colour, with slow blending
 * Requires new colour calculation each call
 */
void mAnimatorLight::SubTask_Flasher_Animate_Function_Slow_Glow(){
  // So colour region does not need to change each loop to prevent colour crushing
  pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
  // Pick new colours
  UpdateDesiredColourFromPaletteSelected();
  // Check if output multiplying has been set, if so, change desiredcolour array
  OverwriteUpdateDesiredColourIfMultiplierIsEnabled();
  // Get starting positions already on show
  UpdateStartingColourWithGetPixel();
  // Call the animator to blend from previous to new
  this->setAnimFunctionCallback(
    [this](const AnimationParam& param){
      this->AnimUpdateMemberFunction_BlendStartingToDesiredColour(param);
    }
  );
}


// simple blend function
void mAnimatorLight::AnimUpdateMemberFunction_BlendStartingToDesiredColour(const AnimationParam& param)
{    
// Serial.println("AnimUpdateMemberFunction_BlendStartingToDesiredColour");
  for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
        animation_colours[pixel].StartingColor,
        animation_colours[pixel].DesiredColour,
        param.progress);    
    SetPixelColor(pixel, updatedColor);
  } // END for

    // SetPixelColor(0, RgbColor(0,random(0,255),0));
}


/**************************************************************************************************************************************************************
***************************************************************************************************************************************************************
********* Function_Slow_Glow *************************************************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************/


/****
 * Changes pixels randomly to new colour, with slow blending
 * Requires new colour calculation each call
 */
void mAnimatorLight::SubTask_Flasher_Animate_Function_FirePlace_01(){
  // So colour region does not need to change each loop to prevent colour crushing
  pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
  // Pick new colours
  //Display on all pixels
  UpdateDesiredColourFromPaletteSelected();

  HsbColor colour_in = HsbColor(RgbColor(0));
  
 #ifndef USE_WS28XX_FEATURE_4_PIXEL_TYPE
  //Overwrite random brightness on special range
  for(uint16_t index=256;index<300;index++){

    colour_in = animation_colours[index].DesiredColour;

    if(colour_in.B==0){ //if colour was off, I need to set the colour to a defined value or it willl turn up brightness to show white
      colour_in.H = 0.0f;
      colour_in.S = 1.0f;
    }
    colour_in.B = pCONT_iLight->BrtN2F(random(0,10)*10);

    // colour_in.H = pCONT_iLight->BrtN2F(random(0,100));
  
    animation_colours[random(256,299)].DesiredColour = colour_in;

    // animation_colours[random(40,49)].DesiredColour = colour_in;

  }
  
#endif 

  // Check if output multiplying has been set, if so, change desiredcolour array
  // OverwriteUpdateDesiredColourIfMultiplierIsEnabled();
  // Get starting positions already on show
  UpdateStartingColourWithGetPixel();
  // Call the animator to blend from previous to new
  this->setAnimFunctionCallback(
    [this](const AnimationParam& param){
      this->AnimUpdateMemberFunction_BlendStartingToDesiredColour(param);
    }
  );
}


/**************************************************************************************************************************************************************
***************************************************************************************************************************************************************
********* SubTask_Flasher_Animate_LCD_Clock_Time_Basic_01 *************************************************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************/

#ifdef ENABLE_DEVFEATURE_RGB_CLOCK

// #define hour(t) pCONT_time->RtcTime.hour
// // #define minute(t) pCONT_time->minute(t) // pCONT_time->RtcTime.minute
// #define second(t) pCONT_time->RtcTime.second
// #define hourFormat12(t) (1)


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
  //if ( second(t) % 2 == 0 ) 
  LCDDisplay_showDots(2, 5);//pCONT_time->second(t) * 4.25);                                // show : between hours and minutes on even seconds with the color cycling through the palette once per minute
  lastSecond = pCONT_time->second(t);
}

void mAnimatorLight::LCDDisplay_showSegment(byte segment, byte color, byte segDisplay) {
  
  // This shows the segments from top of the sketch on a given position (segDisplay).
  // pos 0 is the most right one (seen from the front) where data in is connected to the arduino
  byte leds_per_segment = 1 + abs( segGroups[segment][1] - segGroups[segment][0] );            // get difference between 2nd and 1st value in array to get led count for this segment
  if ( segDisplay % 2 != 0 ) segment += 7;                                                  // if segDisplay/position is odd add 7 to segment
  for (byte i = 0; i < leds_per_segment; i++) {                                             // fill all leds inside current segment with color
    animation_colours[( segGroups[segment][0] + ( segDisplay / 2 ) * ( LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS ) ) + i].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);
  }
}

void mAnimatorLight::LCDDisplay_showDigit(byte digit, byte color, byte pos) {
  // This draws numbers using the according segments as defined on top of the sketch (0 - 9)
  for (byte i = 0; i < 7; i++) {
    if (digits[digit][i] != 0) LCDDisplay_showSegment(i, color, pos);
  }
}

//tmp method
RgbcctColor mAnimatorLight::ColorFromPalette(uint16_t palette_id, uint8_t desired_index, bool apply_global_brightness){

  //tmp fix, colour index not working with mine, I need to modulu it with palette size so it repeats along its length
  // mPaletteI->GetPalettePointerByID(palette_id)

  // AddLog(LOG_LEVEL_TEST, PSTR("desired_index=%d"), desired_index);

  
  mPalette::PALETTELIST::PALETTE *ptr = mPaletteI->GetPalettePointerByID(palette_id);

  uint8_t pixels_max = mPaletteI->GetPixelsInMap(ptr);

  if(desired_index > pixels_max){
    desired_index %= pixels_max;
  }
  

  RgbcctColor colour = mPaletteI->GetColourFromPalette(mPaletteI->GetPalettePointerByID(palette_id), desired_index);

  // switch(desired_index){
  //   case 0: colour = RgbcctColor(255,0,0,0,0); break;
  //   case 1: colour = RgbcctColor(0,255,0,0,0); break;
  //   case 2: colour = RgbcctColor(0,0,255,0,0); break;
  //   case 3: colour = RgbcctColor(255,0,255,0,0); break;
  //   case 4: colour = RgbcctColor(0,255,255,0,0); break;
  //   default:
  //   case 5: colour = RgbcctColor(255,255,255,255,255); break;
  // }

  if(apply_global_brightness){
    pCONT_iLight->ApplyGlobalBrightnesstoColour(&colour);
  }

  return colour;

}



void mAnimatorLight::LCDDisplay_showDots(byte dots, byte color) {

  // in 12h mode and while in setup upper dots resemble AM, all dots resemble PM
  byte startPos = LED_PER_DIGITS_STRIP;
  if ( LED_BETWEEN_DIGITS_STRIPS % 2 == 0 ) {                                                                 // only SE/TE should have a even amount here (0/2 leds between digits)
    animation_colours[startPos].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);
    if ( dots == 2 ) animation_colours[startPos + 1].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);
  } else {                                                                                                    // Regular and XL have 5 leds between digits
    animation_colours[startPos].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);
    animation_colours[startPos + 1].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);
    if ( LED_DIGITS / 3 > 1 ) {
        animation_colours[startPos + LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);//colour;// = ColorFromPalette(currentPalette, color, brightness, LINEARBLEND);
        animation_colours[startPos + LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS + 1].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);//colour;// = ColorFromPalette(currentPalette, color, brightness, LINEARBLEND);
      }
    if ( dots == 2 ) {
      animation_colours[startPos + 3].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);
      animation_colours[startPos + 4].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);
      if ( LED_DIGITS / 3 > 1 ) {
        animation_colours[startPos + LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS + 3].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);//colour;// = ColorFromPalette(currentPalette, color, brightness, LINEARBLEND);
        animation_colours[startPos + LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS + 4].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);//colour;// = ColorFromPalette(currentPalette, color, brightness, LINEARBLEND);
      }
    }
  }
}

/****
 * Changes pixels randomly to new colour, with slow blending
 * Requires new colour calculation each call
 */
void mAnimatorLight::SubTask_Flasher_Animate_LCD_Clock_Time_Basic_01(){
  // So colour region does not need to change each loop to prevent colour crushing
  pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
  // Pick new colours
  //Display on all pixels
  UpdateDesiredColourFromPaletteSelected();

  HsbColor colour_in = HsbColor(RgbColor(0));

  animation_colours[0].DesiredColour = RgbcctColor(0,0,255,255,255);

  for(int i=0;i<93;i++){animation_colours[i].DesiredColour = RgbcctColor(0);}

  //test method
  // animation_colours[0].DesiredColour = RgbcctColor(0,0,255,0,0);
  // animation_colours[1].DesiredColour = RgbcctColor(0,0,255,0,0);
  // animation_colours[2].DesiredColour = RgbcctColor(0,0,255,0,0);

  // animation_colours[3].DesiredColour = RgbcctColor(0,255,0,0,0);
  // animation_colours[4].DesiredColour = RgbcctColor(0,255,0,0,0);
  // animation_colours[5].DesiredColour = RgbcctColor(0,255,0,0,0);



  animation_colours[0].DesiredColour = RgbcctColor(255,0,0,0,0);
  animation_colours[22].DesiredColour = RgbcctColor(0,255,0,0,0);
  animation_colours[44].DesiredColour = RgbcctColor(0,0,255,0,0);
  animation_colours[49].DesiredColour = RgbcctColor(255,0,0,0,0);
  animation_colours[71].DesiredColour = RgbcctColor(0,255,0,0,0);



  // /**
  //  *     A
  //  *  F     B
  //  *     G
  //  *  E     C
  //  *     D
  //  * */

  // // 7-segment, 1/4, A
  // animation_colours[13].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[14].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[15].DesiredColour = RgbcctColor(255,0,0,0,0);
  // // 7-segment, 1/4, B
  // animation_colours[16].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[17].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[18].DesiredColour = RgbcctColor(255,0,0,0,0);
  // // 7-segment, 1/4, C
  // animation_colours[0].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[1].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[2].DesiredColour = RgbcctColor(255,0,0,0,0);
  // // 7-segment, 1/4, D
  // animation_colours[3].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[4].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[5].DesiredColour = RgbcctColor(255,0,0,0,0);
  // // 7-segment, 1/4, E
  // animation_colours[6].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[7].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[8].DesiredColour = RgbcctColor(255,0,0,0,0);
  // // 7-segment, 1/4, F
  // animation_colours[10].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[11].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[12].DesiredColour = RgbcctColor(255,0,0,0,0);
  // // 7-segment, 1/4, G
  // animation_colours[19].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[20].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[21].DesiredColour = RgbcctColor(255,0,0,0,0);


  // // 7-segment, 2/4, A
  // animation_colours[35].DesiredColour = RgbcctColor(255,255,0,0,0);
  // animation_colours[36].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[37].DesiredColour = RgbcctColor(255,0,0,0,0);
  // // 7-segment, 2/4, B
  // animation_colours[32].DesiredColour = RgbcctColor(255,255,0,0,0);
  // animation_colours[33].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[34].DesiredColour = RgbcctColor(255,0,0,0,0);
  // // 7-segment, 2/4, C
  // animation_colours[28].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[29].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[30].DesiredColour = RgbcctColor(255,0,0,0,0);
  // // 7-segment, 2/4, D
  // animation_colours[25].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[26].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[27].DesiredColour = RgbcctColor(255,0,0,0,0);
  // // 7-segment, 2/4, E
  // animation_colours[22].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[23].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[24].DesiredColour = RgbcctColor(255,0,0,0,0);
  // // 7-segment, 2/4, F
  // animation_colours[38].DesiredColour = RgbcctColor(255,255,0,0,0);
  // animation_colours[39].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[40].DesiredColour = RgbcctColor(255,0,0,0,0);
  // // 7-segment, 2/4, G
  // animation_colours[41].DesiredColour = RgbcctColor(255,255,0,0,0);
  // animation_colours[42].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[43].DesiredColour = RgbcctColor(255,0,0,0,0);
  
  // // Colon, 1/1, top
  // animation_colours[44].DesiredColour = RgbcctColor(0,0,255,0,0);
  // animation_colours[45].DesiredColour = RgbcctColor(0,0,255,0,0);
  // // Colon, 1/1, bottom
  // animation_colours[47].DesiredColour = RgbcctColor(255,0,255,0,0);
  // animation_colours[48].DesiredColour = RgbcctColor(255,0,255,0,0);
  

  // // 7-segment, 3/4, A
  // animation_colours[84].DesiredColour = RgbcctColor(255,100,0,0,0);
  // animation_colours[85].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[86].DesiredColour = RgbcctColor(255,0,0,0,0);
  // // 7-segment, 3/4, B
  // animation_colours[87].DesiredColour = RgbcctColor(255,100,100,0,0);
  // animation_colours[88].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[89].DesiredColour = RgbcctColor(255,0,0,0,0);
  // // 7-segment, 3/4, C
  // animation_colours[71].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[72].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[73].DesiredColour = RgbcctColor(255,0,0,0,0);
  // // 7-segment, 3/4, D
  // animation_colours[74].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[75].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[76].DesiredColour = RgbcctColor(255,0,0,0,0);
  // // 7-segment, 3/4, E
  // animation_colours[77].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[78].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[79].DesiredColour = RgbcctColor(255,0,0,0,0);
  // // 7-segment, 3/4, F
  // animation_colours[81].DesiredColour = RgbcctColor(255,100,0,0,0);
  // animation_colours[82].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[83].DesiredColour = RgbcctColor(255,0,0,0,0);
  // // 7-segment, 3/4, G
  // animation_colours[90].DesiredColour = RgbcctColor(255,100,100,0,0);
  // animation_colours[91].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[92].DesiredColour = RgbcctColor(255,0,0,0,0);


  // // 7-segment, 4/4, A
  // animation_colours[62].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[63].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[64].DesiredColour = RgbcctColor(255,0,0,0,0);
  // // 7-segment, 4/4, B
  // animation_colours[59].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[60].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[61].DesiredColour = RgbcctColor(255,0,0,0,0);
  // // 7-segment, 4/4, C
  // animation_colours[55].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[56].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[57].DesiredColour = RgbcctColor(255,0,0,0,0);
  // // 7-segment, 4/4, D
  // animation_colours[52].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[53].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[54].DesiredColour = RgbcctColor(255,0,0,0,0);
  // // 7-segment, 4/4, E
  // animation_colours[49].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[50].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[51].DesiredColour = RgbcctColor(255,0,0,0,0);
  // // 7-segment, 4/4, F
  // animation_colours[65].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[66].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[67].DesiredColour = RgbcctColor(255,0,0,0,0);
  // // 7-segment, 4/4, G
  // animation_colours[68].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[69].DesiredColour = RgbcctColor(255,0,0,0,0);
  // animation_colours[70].DesiredColour = RgbcctColor(255,0,0,0,0);




  for(int i=0;i<93;i++){animation_colours[i].DesiredColour = RgbcctColor(0);}

      uint16_t tempnumber = testnum;

      // if(tempnumber%2){
      //   showDots();
      // }
      if ( pCONT_time->second(0) % 2 == 0 ) LCDDisplay_showDots(2, pCONT_time->second(0) * 4.25);  


      LCDDisplay_showDigit(
        (tempnumber/1)%10,
         64, 0);
      LCDDisplay_showDigit(
      (tempnumber/10)%10,
         64, 1);
      LCDDisplay_showDigit(
        (tempnumber/100)%10,
         64, 2);
      LCDDisplay_showDigit(
        (tempnumber/1000)%10,
         64, 3);


      // showDigit(11, 64, 2);
      // showDigit(12, 64, 1);

if(testnum++>9999){testnum=0;}



  
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
  
//     animation_colours[random(256,299)].DesiredColour = colour_in;

//     // animation_colours[random(40,49)].DesiredColour = colour_in;

//   }
  
// #endif 

  // Check if output multiplying has been set, if so, change desiredcolour array
  // OverwriteUpdateDesiredColourIfMultiplierIsEnabled();
  // Get starting positions already on show
  UpdateStartingColourWithGetPixel();
  // Call the animator to blend from previous to new
  this->setAnimFunctionCallback(
    [this](const AnimationParam& param){
      this->AnimUpdateMemberFunction_BlendStartingToDesiredColour(param);
    }
  );
}




/**************************************************************************************************************************************************************
***************************************************************************************************************************************************************
********* SubTask_Flasher_Animate_LCD_Clock_Time_Basic_02 *************************************************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************/


/**
 * Cycles over leds that are on and applies palette
 * */
void mAnimatorLight::LCDDisplay_colorOverlay() {                                                                                       // This "projects" colors on already drawn leds before showing leds in updateDisplay();
  for (byte i = 0; i < LED_COUNT; i++) {                                                                    // check each led...
    if (animation_colours[i].DesiredColour.CalculateBrightness())  {
      
      animation_colours[i].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, startColor + (colorOffset * i));

    }
    
                                                                                              // ...and if it is lit...
      // leds[i] = ColorFromPalette(currentPalette, startColor + (colorOffset * i), brightness, LINEARBLEND);  // ...assign increasing color from current palette
  }
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
void mAnimatorLight::SubTask_Flasher_Animate_LCD_Clock_Time_Basic_02(){
  // So colour region does not need to change each loop to prevent colour crushing
  pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
  // Pick new colours
  //Display on all pixels
  UpdateDesiredColourFromPaletteSelected();

  for(int i=0;i<93;i++){animation_colours[i].DesiredColour = RgbcctColor(0);}

  // if(tempcol++>5){
    tempcol=0;
    // } //startcolour

  AddLog(LOG_LEVEL_TEST, PSTR("tempcol=%d"), tempcol);

  LCDDisplay_updateDisplay(tempcol, colorOffset);
    

  // LCDDisplay_displayTime(pCONT_time->Rtc.utc_time,tempcol,0);

//       uint16_t tempnumber = testnum;

//       // if(tempnumber%2){
//       //   showDots();
//       // }
//       if ( second(0) % 2 == 0 ) showDots(2, second(0) * 4.25);  


//       showDigit(
//         (tempnumber/1)%10,
//          64, 0);
//       showDigit(
//       (tempnumber/10)%10,
//          64, 1);
//       showDigit(
//         (tempnumber/100)%10,
//          64, 2);
//       showDigit(
//         (tempnumber/1000)%10,
//          64, 3);


//       // showDigit(11, 64, 2);
//       // showDigit(12, 64, 1);

// if(testnum++>9999){testnum=0;}

  
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
  
//     animation_colours[random(256,299)].DesiredColour = colour_in;

//     // animation_colours[random(40,49)].DesiredColour = colour_in;

//   }
  /*
  if (  ( lastLoop - lastColorChange >= colorChangeInterval ) && ( overlayMode == 0 )         // if colorChangeInterval has been reached and overlayMode is disabled...
     || ( lastLoop - lastColorChange >= overlayInterval ) && ( overlayMode == 1 ) ) {         // ...or if overlayInterval has been reached and overlayMode is enabled...
    startColor++;                                                                             // increase startColor to "move" colors slowly across the digits/leds
    updateDisplay(startColor, colorOffset);
    lastColorChange = millis();
  }
  if ( lastSecond != second() ) {                                                             // if current second is different from last second drawn...
    updateDisplay(startColor, colorOffset);                                                   // lastSecond will be set in displayTime() and will be used for
    lastSecond = second();                                                                    // redrawing regardless the digits count (HH:MM or HH:MM:SS)
  }
  if ( lastKeyPressed == 1 ) {                                                                // if buttonA is pressed...
    switchBrightness();                                                                       // ...switch to next brightness level
    updateDisplay(startColor, colorOffset);
    if ( btnRepeatCounter >= 20 ) {                                                           // if buttonA is held for a few seconds change overlayMode 0/1 (using colorOverlay())
      if ( overlayMode == 0 ) overlayMode = 1; else overlayMode = 0;
      updateDisplay(startColor, colorOffset);
      EEPROM.put(3, overlayMode);                                                             // ...and write setting to eeprom
      #ifdef nodeMCU                                                                          // on nodeMCU we need to commit the changes from ram to flash to make them permanent
        EEPROM.commit();
      #endif
      btnRepeatStart = millis();
    }
  }
  if ( lastKeyPressed == 2 ) {                                                                // if buttonB is pressed...
    switchPalette();                                                                          // ...switch between color palettes
    updateDisplay(startColor, colorOffset);
    if ( btnRepeatCounter >= 20 ) {                                                           // if buttonB is held for a few seconds change displayMode 0/1 (12h/24h)...
      if ( displayMode == 0 ) displayMode = 1; else displayMode = 0;
      updateDisplay(startColor, colorOffset);
      EEPROM.put(2, displayMode);                                                             // ...and write setting to eeprom
      #ifdef nodeMCU
        EEPROM.commit();
      #endif
      btnRepeatStart = millis();
    }
  }
  if ( ( lastLoop - valueLDRLastRead >= intervalLDR ) && ( brightnessAuto == 1 ) ) {          // if LDR is enabled and sample interval has been reached...
    readLDR();                                                                                // ...call readLDR();
    if ( abs(avgLDR - lastAvgLDR) >= 5 ) {                                                    // only adjust current brightness if avgLDR has changed for more than +/- 5.
      updateDisplay(startColor, colorOffset);
      lastAvgLDR = avgLDR;
      if ( dbg ) { Serial.print(F("Updated display with avgLDR of: ")); Serial.println(avgLDR); }
    }
    valueLDRLastRead = millis();
  }
  if ( lastKeyPressed == 12 ) {                                                               // if buttonA + buttonB are pushed at the same time....
    #ifdef useWiFi                                                                            // ...and if using WiFi...
      initWPS();                                                                              // ...start WPS
    #else                                                                                     // otherwise (arduino + rtc or nodemcu + rtc)...
      setupClock();                                                                           // ...start manual setup
    #endif
  }
  #ifdef nodeMCU                                                                              // On Arduino SetSyncProvider will be used. So this will sync internal time to rtc/ntp on nodeMCU only
    if ( ( hour() == 3 || hour() == 9 || hour() == 15 || hour() == 21 ) &&                    // if hour is 3, 9, 15 or 21 and...
         ( minute() == 3 && second() == 0 ) ) {                                               // minute is 3 and second is 0....
      if ( dbg ) Serial.print(F("Current time: ")); Serial.println(now());
      #ifdef useWiFi
        syncNTP();                                                                            // ...either sync using ntp or...
      #else
        setTime(Rtc.GetDateTime());                                                           // ...set internal time to rtc time...
      #endif
      if ( dbg ) Serial.print(F("New time: ")); Serial.println(now());
    }
    ESP.wdtFeed();                                                                            // feed the watchdog each time loop() is cycled through, just in case...
  #endif
  FastLED.show();                                                                             // run FastLED.show() every time to avoid color flickering at low brightness settings
  lastKeyPressed = readButtons();
  lastLoop = millis();
  if ( dbg ) dbgInput();   */
// #endif 

  // Check if output multiplying has been set, if so, change desiredcolour array
  // OverwriteUpdateDesiredColourIfMultiplierIsEnabled();
  // Get starting positions already on show
  UpdateStartingColourWithGetPixel();
  // Call the animator to blend from previous to new
  this->setAnimFunctionCallback(
    [this](const AnimationParam& param){
      this->AnimUpdateMemberFunction_BlendStartingToDesiredColour(param);
    }
  );
}



/****
 * Changes pixels randomly to new colour, with slow blending
 * Requires new colour calculation each call
 * 02 trying lib method with mapping
 */
void mAnimatorLight::SubTask_Flasher_Animate_LCD_Display_Show_Numbers_Basic_01(){
  // So colour region does not need to change each loop to prevent colour crushing
  pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
  // Pick new colours
  //Display on all pixels
  UpdateDesiredColourFromPaletteSelected();

  for(int i=0;i<93;i++){animation_colours[i].DesiredColour = RgbcctColor(0);}

  // if(tempcol++>5){
    // tempcol=0;
    // } //startcolour

// lcd_display_show_number ++;

  // AddLog(LOG_LEVEL_TEST, PSTR("tempcol=%d"), tempcol);

  // LCDDisplay_updateDisplay(tempcol, colorOffset);

  
  // LCDDisplay_displayTime(pCONT_time->Rtc.utc_time, color, colorSpacing);
  // LCDDisplay_showDigit(23,0,0);
  LCDDisplay_showDigit((lcd_display_show_number / 10), 0+1, 1 );                   // minutes thankfully don't differ between 12h/24h, so this can be outside the above if/else
  LCDDisplay_showDigit((lcd_display_show_number % 10), 0, 0 );                   // each digit is drawn with an increasing color (*2, *3, *4, *5) (*6 and *7 for seconds only in HH:MM:SS)
  

  // Check if output multiplying has been set, if so, change desiredcolour array
  // OverwriteUpdateDesiredColourIfMultiplierIsEnabled();
  // Get starting positions already on show
  UpdateStartingColourWithGetPixel();
  // Call the animator to blend from previous to new
  this->setAnimFunctionCallback(
    [this](const AnimationParam& param){
      this->AnimUpdateMemberFunction_BlendStartingToDesiredColour(param);
    }
  );
}
#endif // ENABLE_DEVFEATURE_RGB_CLOCK



/**************************************************************************************************************************************************************
***************************************************************************************************************************************************************
********* Function_Solid_RGBCCT ie New Scene mode for cct strips **********************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************/


/****
 * Changes pixels randomly to new colour, with slow blending
 * Requires new colour calculation each call
 */
void mAnimatorLight::SubTask_Flasher_Animate_Function_Solid_RGBCCT(){
  // AddLog(LOG_LEVEL_TEST, PSTR("SubTask_Flasher_Animate_Function_Solid_RGBCCT"));
  // Set palette pointer
  mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
  // Set up colours
  // Brightness is generated internally, and rgbcct solid palettes are output values
  pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = false;
  animation_colours_rgbcct.DesiredColour  = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr);

  AddLog(LOG_LEVEL_TEST, PSTR("DesiredColour1=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
  if(!pCONT_iLight->rgbcct_controller.getApplyBrightnessToOutput()){ // If not already applied, do it using global values
    animation_colours_rgbcct.DesiredColour = ApplyBrightnesstoRgbcctColour(
      animation_colours_rgbcct.DesiredColour, 
      pCONT_iLight->rgbcct_controller.getBrightnessRGB255(),
      pCONT_iLight->rgbcct_controller.getBrightnessCCT255()
    );
  }

  AddLog(LOG_LEVEL_TEST, PSTR("DesiredColour2=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
  animation_colours_rgbcct.StartingColor = GetPixelColor();
  // GetPixelColourHardwareInterface();

  AddLog(LOG_LEVEL_TEST, PSTR("StartingColour2=%d,%d,%d,%d,%d"), animation_colours_rgbcct.StartingColor.R,animation_colours_rgbcct.StartingColor.G,animation_colours_rgbcct.StartingColor.B,animation_colours_rgbcct.StartingColor.WC,animation_colours_rgbcct.StartingColor.WW);
    

  // Call the animator to blend from previous to new
  this->setAnimFunctionCallback(
    [this](const AnimationParam& param){
      this->AnimUpdateMemberFunction_BlendStartingToDesiredColour_Solid_RGBCCT(param);
    }
  );
}


// simple blend function
void mAnimatorLight::AnimUpdateMemberFunction_BlendStartingToDesiredColour_Solid_RGBCCT(const AnimationParam& param)
{   

  RgbcctColor output_colour = RgbcctColor::LinearBlend(
    animation_colours_rgbcct.StartingColor,
    animation_colours_rgbcct.DesiredColour,
    param.progress);

  for(int ii=0;ii<pCONT_iLight->settings.light_size_count;ii++){
    SetPixelColor(ii,output_colour);
  }

  // pCONT_iLight->SetPixelColourHardwareInterface(output_colour,0,true);
  
}




/**************************************************************************************************************************************************************
***************************************************************************************************************************************************************
********* Function_Slow_Glow *************************************************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Flasher_Animate_Function_Sequential(){

  pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
  
  flashersettings.flag_finish_flasher_pair = false;
  flashersettings.flags.enable_random_rate = true;
  
  do{ //must complete the pair together //move inside functions
    switch(flashersettings.region){
      case EFFECTS_REGION_COLOUR_SELECT_ID: //set colours
        #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
        AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
        #endif
        UpdateDesiredColourFromPaletteSelected();
        // Check if output multiplying has been set, if so, change desiredcolour array
        OverwriteUpdateDesiredColourIfMultiplierIsEnabled();
        // Get starting positions already on show
        UpdateStartingColourWithGetPixel();
        

    // Draw the tail that will be rotated through all the rest of the pixels
    // DrawTailPixels();


        flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
      break;
      case EFFECTS_REGION_ANIMATE_ID: //shift along
        #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
        AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
        #endif
        RotateDesiredColour(1,flashersettings.flags.movement_direction);
// Get starting positions already on show
        UpdateStartingColourWithGetPixel();
        

        this->setAnimFunctionCallback(
          [this](const AnimationParam& param){
            this->AnimUpdateMemberFunction_Sequential(param);
          }
        );

        if(flashersettings.flags.force_finish_flasher_pair_once){
          AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("flashersettings.flags.force_finish_flasher_pair_once WAS SET"));
          flashersettings.flags.force_finish_flasher_pair_once = false;
        }
      break;
    }
  }while(flashersettings.flag_finish_flasher_pair || flashersettings.flags.force_finish_flasher_pair_once);

}


// simple blend function
void mAnimatorLight::AnimUpdateMemberFunction_Sequential(const AnimationParam& param)
{    
 // wait for this animation to complete,
//     // we are using it as a timer of sorts
//     if (param.state == AnimationState_Completed)
//     {
//         // done, time to restart this position tracking animation/timer
//         pCONT_iLight->animator_controller->RestartAnimation(param.index);

//         // rotate the complete strip one pixel to the right on every update
//         // stripbus->RotateRight(1);

//          RotateDesiredColour(1,flashersettings.flags.movement_direction);
// // Get starting positions already on show
//         UpdateStartingColourWithGetPixel();

//     }

        // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL AnimUpdateMemberFunction_Sequential"));
  
  for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
        animation_colours[pixel].StartingColor,
        animation_colours[pixel].DesiredColour,
        param.progress);    
    SetPixelColor(pixel, updatedColor);
  } // END for

}

// void mAnimatorLight::DrawTailPixels()
// {
//     // using Hsl as it makes it easy to pick from similiar saturated colors
//     float hue = random(360) / 360.0f;
//     for (uint16_t index = 0; index < stripbus->PixelCount() && index <= TailLength; index++)
//     {
//         float lightness = index * MaxLightness / TailLength;
//         RgbColor color = HslColor(hue, 1.0f, lightness);

//         stripbus->SetPixelColor(index, colorGamma.Correct(color));
//     }
// }



/**************************************************************************************************************************************************************
***************************************************************************************************************************************************************
********* SubTask_Flasher_Animate_Function_Slow_Glow_Partial_Palette_Step_Through *************************************************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************/


/****
 * Changes pixels randomly to new colour, with slow blending
 * Requires new colour calculation each call
 */
void mAnimatorLight::SubTask_Flasher_Animate_Function_Slow_Glow_Partial_Palette_Step_Through(){
  // So colour region does not need to change each loop to prevent colour crushing
  pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
  // Pick new colours

  switch(flashersettings.region){
    case EFFECTS_REGION_COLOUR_SELECT_ID:{ //set colours
      AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));

      pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
      // UpdateDesiredColourFromPaletteSelected();
      // UpdateStartingColourWithGetPixel();
      flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
    // NO 

        mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
        
      int16_t pixel_position = -2;
      uint8_t pixels_in_map = mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr);
      
      
       AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "pixels_in_map= %d"),pixels_in_map);


      RgbTypeColor colour;
      
      desired_pixel = shared_flasher_parameters.indexes.active;
      uint8_t pixels_map_upper_limit = shared_flasher_parameters.indexes.active+1;
      uint8_t pixels_map_lower_limit = shared_flasher_parameters.indexes.active;

      uint8_t index_1, index_2;

      uint8_t counter = 0;

          
      //if last pixel, then include it and the first, else include it and the next
      if(shared_flasher_parameters.indexes.active == pixels_in_map-1){ //wrap wround
        index_1 = 0;
        index_2 = shared_flasher_parameters.indexes.active;
        counter = 0;
      }else{
        index_1 = shared_flasher_parameters.indexes.active;
        index_2 = shared_flasher_parameters.indexes.active+1;
        counter = 1;

      }

      shared_flasher_parameters.indexes.counter ^= 1;

      
      AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "shared_flasher_parameters = %d/%d/%d"),shared_flasher_parameters.indexes.active,index_1,index_2);
  
      
    

      for(uint16_t index=0;index<pCONT_iLight->settings.light_size_count;index++){


          if(counter^=1){
            //using %2 means alternate leds replace the previous position, not the current
            desired_pixel = shared_flasher_parameters.indexes.counter ? index_2 : index_1;
          }else{
            desired_pixel = shared_flasher_parameters.indexes.counter ? index_1 : index_2;
          }
        
        colour = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel,&pixel_position);

        if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
          colour = ApplyBrightnesstoRgbcctColour(colour,pCONT_iLight->rgbcct_controller.getBrightnessRGB255());
        }



        animation_colours[index].DesiredColour = colour;
         
        //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "desired_pixel= %d/%d/%d"),pixels_map_lower_limit,desired_pixel,pixels_map_upper_limit);
  
        // if(++desired_pixel>pixels_map_upper_limit){
        //   desired_pixel = pixels_map_lower_limit;
        // }

      } 

      //progress active index by 1 or reset
      AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "shared_flasher_parameters.indexes.active=%d"),shared_flasher_parameters.indexes.active);
      if(++shared_flasher_parameters.indexes.active>pixels_in_map-1){
        shared_flasher_parameters.indexes.active=0;
      }
      
  
    }break;
    case EFFECTS_REGION_ANIMATE_ID: //shift along
      AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));

      // Check if output multiplying has been set, if so, change desiredcolour array
      // OverwriteUpdateDesiredColourIfMultiplierIsEnabled();
      // Get starting positions already on show
      UpdateStartingColourWithGetPixel();
      // Call the animator to blend from previous to new
      this->setAnimFunctionCallback(
        [this](const AnimationParam& param){
          this->AnimUpdateMemberFunction_BlendStartingToDesiredColour(param);
        }
      );

      flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID;


      break;
  }



            // desired_colour[ledout.pattern[ledout.index]] = colour; 
            // AddLog(LOG_LEVEL_TEST, PSTR("colour=%d,%d,%d"),
            // colour.R,
            // colour.G,
            // colour.B);
            
            // animation_colours[ledout.pattern[ledout.index]].DesiredColour = ApplyBrightnesstoDesiredColour(colour,pCONT_iLight->getBriRGB());
              
          //   if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
          //     animation_colours[ledout.pattern[ledout.index]].DesiredColour = ApplyBrightnesstoDesiredColour(animation_colours[ledout.pattern[ledout.index]].DesiredColour,pCONT_iLight->getBriRGB());
          //   }

          //   //   AddLog(LOG_LEVEL_TEST, PSTR("colou2=%d,%d,%d"),
          //   // animation_colours[ledout.pattern[ledout.index]].DesiredColour.R,
          //   // animation_colours[ledout.pattern[ledout.index]].DesiredColour.G,
          //   // animation_colours[ledout.pattern[ledout.index]].DesiredColour.B);

            
          //   #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
          //   // AddLog(LOG_LEVEL_DEBUG_MORE_MORE,PSTR(D_LOG_NEO "colour[p%d:d%d] = %d,%d,%d %d pp%d"),
          //   //   ledout.pattern[ledout.index],desired_pixel,
          //   //   pCONT_iLight->HueF2N(desired_colour[ledout.pattern[ledout.index]].H),pCONT_iLight->SatF2N(desired_colour[ledout.pattern[ledout.index]].S),pCONT_iLight->BrtF2N(desired_colour[ledout.pattern[ledout.index]].B),
          //   //   mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr),pixel_position
          //   // );
          //   #endif

          //   if(++desired_pixel>=mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr)){desired_pixel=0;}
          // } //end for




  
}


// simple blend function
void mAnimatorLight::AnimUpdateMemberFunction_Slow_Glow_Partial_Palette_Step_Through(const AnimationParam& param)
{    

  for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
        animation_colours[pixel].StartingColor,
        animation_colours[pixel].DesiredColour,
        param.progress);    
    SetPixelColor(pixel, updatedColor);
  } // END for

}





/**************************************************************************************************************************************************************
***************************************************************************************************************************************************************
********* Function_Tester *************************************************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Flasher_Animate_Function_Tester(){

  uint16_t test_strip_size = pCONT_iLight->settings.light_size_count;
  test_strip_size = 50;

  // clear
  for(uint16_t index=0; index<test_strip_size; index++ ){
    animation_colours[index].DesiredColour = RgbColor(0);
  }

  // set one pixel on
  animation_colours[counter_test].DesiredColour = RgbColor(0,0,255);
  animation_colours[counter_test+5].DesiredColour = RgbColor(0,0,255);
  animation_colours[counter_test+10].DesiredColour = RgbColor(0,0,255);
  // Serial.printf("counter_test=%d\n\r",counter_test);
  if(counter_test++ > 40){
    counter_test = 0;
  }

  for (uint16_t pixel = 0; pixel < test_strip_size; pixel++){
    animation_colours[pixel].StartingColor = stripbus->GetPixelColor(pixel);
  }
        
  this->setAnimFunctionCallback(
    [this](const AnimationParam& param){
      this->AnimUpdateMemberFunction_Tester(param);
    }
  );

}


// simple blend function
void mAnimatorLight::AnimUpdateMemberFunction_Tester(const AnimationParam& param)
{    

  for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
        animation_colours[pixel].StartingColor,
        animation_colours[pixel].DesiredColour,
        param.progress);    
    SetPixelColor(pixel, updatedColor);
    
    // stripbus->SetPixelColor(pixel, updatedColor);//animation_colours[pixel].DesiredColour);

  } // END for

}



/**************************************************************************************************************************************************************
***************************************************************************************************************************************************************
********* SubTask_Flasher_Animate_Function_Popping_Palette_Brightness_From_Lower_To_Upper_Boundary *************************************************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************/


/****
 * Changes pixels randomly to new colour, with slow blending
 * Requires new colour calculation each call
 */
void mAnimatorLight::SubTask_Flasher_Animate_Function_Popping_Palette_Brightness_From_Lower_To_Upper_Boundary(){

   switch(flashersettings.region){
    case EFFECTS_REGION_COLOUR_SELECT_ID: //set colours
      AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
      pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
      // if(flashersettings.flags.enable_endingcolour_as_alternate){
      //   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL flashersettings.flags.enable_endingcolour_as_alternate"));
      //   UpdateDesiredColourWithSingleColour(RgbcctColor(0));
      // }

      pCONT_iLight->rgbcct_controller.setBrightnessRGB255(map(flashersettings.brightness_min, 0,100, 0,255));


      UpdateDesiredColourFromPaletteSelected();
      // if(flashersettings.flags.enable_startcolour_as_alternate){
      //   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL flashersettings.flags.enable_startcolour_as_alternate"));
      //   UpdateStartingColourWithSingleColour(RgbcctColor(0));
      // }else{
        UpdateStartingColourWithGetPixel();
      // }
      flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
    break;
    case EFFECTS_REGION_ANIMATE_ID: //shift along

    
      UpdateStartingColourWithGetPixel();

      uint16_t index_random = random(0,pCONT_iLight->settings.light_size_count);

      //#ifndef USE_WS28XX_FEATURE_4_PIXEL_TYPE //tmp fix
      //HsbColor hsb = RgbColor(GetPixelColor(index_random));
      //#else
      HsbColor hsb = RgbColor(GetPixelColor(index_random));
      //#endif

      uint8_t brightness_now = mapvalue(pCONT_iLight->BrtF2N(hsb.B),0,100,0,255);

      AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE %d %d"), brightness_now, flashersettings.brightness_max);

      if(brightness_now < flashersettings.brightness_max){ //+5 to margins so its not the same
        hsb.B = pCONT_iLight->BrtN2F(flashersettings.brightness_max);
        // animation_colours[index_random].DesiredColour = ApplyBrightnesstoDesiredColour(animation_colours[index_random].DesiredColour, flashersettings.brightness_max);
        animation_colours[index_random].DesiredColour = hsb;
      }

      AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
      this->setAnimFunctionCallback([this](const AnimationParam& param){ this->AnimUpdateMemberFunction_BlendStartingToDesiredColour(param); });
    break;
  }

}

/**************************************************************************************************************************************************************
***************************************************************************************************************************************************************
********* SubTask_Flasher_Animate_Function_Twinkle_Palette_Brightness_From_Lower_To_Upper_And_Back *************************************************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************/


/****
 * Changes pixels randomly to new colour, with slow blending
 * Requires new colour calculation each call
 */
void mAnimatorLight::SubTask_Flasher_Animate_Function_Twinkle_Palette_Brightness_From_Lower_To_Upper_And_Back(){

// Finish this mode later in december
// Split strip into parts
// Take every X pixels as the animated pixel, then I only need to remember a count 0:X
// Move across that each time (or make it random)
// Use this, to manipulate brightness from low to high... on the next animate_id call, return birghtness back (with require another counter for "progress of animation")
// Once that group has changed, or, even during it, pick another group to manipulate
// Effect should be "Group of pixels pulsing on, while the rest remain at a darker brightness"

  return;

   switch(flashersettings.region){
    case EFFECTS_REGION_COLOUR_SELECT_ID: //set colours
      AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
      pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
      // if(flashersettings.flags.enable_endingcolour_as_alternate){
      //   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL flashersettings.flags.enable_endingcolour_as_alternate"));
      //   UpdateDesiredColourWithSingleColour(RgbcctColor(0));
      // }

      pCONT_iLight->rgbcct_controller.setBrightnessRGB255(map(flashersettings.brightness_min, 0,100, 0,255));


      UpdateDesiredColourFromPaletteSelected();
      // if(flashersettings.flags.enable_startcolour_as_alternate){
      //   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL flashersettings.flags.enable_startcolour_as_alternate"));
      //   UpdateStartingColourWithSingleColour(RgbcctColor(0));
      // }else{
        UpdateStartingColourWithGetPixel();
      // }
      flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
    break;
    case EFFECTS_REGION_ANIMATE_ID: //shift along




    
        // UpdateStartingColourWithGetPixel();

        // uint16_t index_random = random(0,pCONT_iLight->settings.light_size_count);

        // HsbColor hsb = GetPixelColor(index_random);

        // if(hsb.B < pCONT_iLight->BrtN2F(flashersettings.brightness_max)){
        //   hsb.B = pCONT_iLight->BrtN2F(flashersettings.brightness_max);
        //   animation_colours[index_random].DesiredColour = hsb;
        // }

      AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
      this->setAnimFunctionCallback([this](const AnimationParam& param){ this->AnimUpdateMemberFunction_Twinkle_Palette_Brightness_From_Lower_To_Upper_And_Back(param); });
      break;

      
      // Stay on this until restarted
  }

}


void mAnimatorLight::AnimUpdateMemberFunction_Twinkle_Palette_Brightness_From_Lower_To_Upper_And_Back(const AnimationParam& param)
{   

  // As integer so the if statement checks will not fail due to rounding errors
  // uint8_t progress_percentage = param.progress*100; 
  // uint8_t brightness_as_percentage = map(pCONT_iLight->getBriRGB(), 0,255, 0,100);
  // uint8_t random_amount = map(shared_flasher_parameters.alternate_random_amount_as_percentage, 0,100, 0,pCONT_iLight->settings.light_size_count);

  //       uint16_t index_random = random(0,pCONT_iLight->settings.light_size_count);

  // /*
  // 0-10    20-30   40-50   60-70    80-90      //coloured
  //     10-20   30-40   50-60   70-80   90-0    //white
  // */
  // if(progress_percentage != shared_flasher_parameters.progress_percentage_last_animated_on){
  //   shared_flasher_parameters.progress_percentage_last_animated_on = progress_percentage; //update stored value
  //   switch(progress_percentage){
  //     case 50:


  //       HsbColor hsb = GetPixelColor(index_random);

  //       if(hsb.B < pCONT_iLight->BrtN2F(flashersettings.brightness_max)){
  //         hsb.B = pCONT_iLight->BrtN2F(flashersettings.brightness_max);
  //         // animation_colours[index_random].DesiredColour = hsb;
  //       }

  //       SetPixelColor(index_random, hsb);



  //       // for (uint16_t ii = 0; ii < random_amount; ii++){
  //       //   SetPixelColor(
  //       //     random(0,pCONT_iLight->settings.light_size_count), 
  //       //     HsbColor(pCONT_iLight->HueN2F(30),pCONT_iLight->SatN2F(90),pCONT_iLight->BrtN2F(random(0,brightness_as_percentage)))
  //       //   );
  //       // }
  //     break;
  //     case 0:
  //     case 100:
  //       for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
  //         SetPixelColor(pixel, animation_colours[pixel].DesiredColour);
  //       }
  //     break;
  //   }
  // }



   RgbTypeColor updatedColor;
  for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    updatedColor = RgbTypeColor::LinearBlend(
        animation_colours[pixel].StartingColor,
        animation_colours[pixel].DesiredColour,
        param.progress);    
    SetPixelColor(pixel, updatedColor);
  } // END for
  

}




/**************************************************************************************************************************************************************
***************************************************************************************************************************************************************
********* SubTask_Flasher_Animate_Function_Pulse_Random_On *************************************************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Flasher_Animate_Function_Pulse_Random_On(){

  // So colour region does not need to change each loop to prevent colour crushing
  pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
  // Pick new colours
  UpdateDesiredColourFromPaletteSelected();
  // randomly blank most of them out again
  uint16_t pixels_to_clear = 
  pCONT_iLight->animation.transition.pixels_to_update_as_number;
  //pCONT_iLight->settings.light_size_count-pCONT_iLight->GetPixelsToUpdateAsNumberFromPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage);
  Serial.printf("pixels_to_clear=%d\n\r",pixels_to_clear);
  for(uint16_t index=0; index<pixels_to_clear; index++ ){
    animation_colours[random(0,pCONT_iLight->settings.light_size_count)].DesiredColour = RgbColor(0);
  }
  // Check if output multiplying has been set, if so, change desiredcolour array
  OverwriteUpdateDesiredColourIfMultiplierIsEnabled();
  // Get starting positions already on show
  // UpdateStartingColourWithGetPixel();
  // Call the animator to blend from previous to new
        
  this->setAnimFunctionCallback(
    [this](const AnimationParam& param){
      this->AnimUpdateMemberFunction_Pulse_Random_On(param);
    }
  );

}


// 50% progress is full brightness, 0 and 100% is off
void mAnimatorLight::AnimUpdateMemberFunction_Pulse_Random_On(const AnimationParam& param)
{    

  // float progress_percentage = param.progress*100; 
  float progress_half_range = param.progress < 0.5 ? 
                                  mSupport::mapfloat(param.progress, 0,0.5, 0,1) : 
                                  mSupport::mapfloat(param.progress, 0.5,1, 1,0) ;

  for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
        RgbColor(0),
        animation_colours[pixel].DesiredColour,
        progress_half_range);    //0-50%
    SetPixelColor(pixel, updatedColor);
  } // END for

}



/**************************************************************************************************************************************************************
***************************************************************************************************************************************************************
********* SubTask_Flasher_Animate_Function_Pulse_Random_On_2 *************************************************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Flasher_Animate_Function_Pulse_Random_On_2(){

  pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
  
  flashersettings.flag_finish_flasher_pair = false;
  flashersettings.flags.enable_random_rate = true;
  
  do{ //must complete the pair together
    switch(flashersettings.region){
      case EFFECTS_REGION_COLOUR_SELECT_ID:{ //set colours
        #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
        AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "Random_On_2 EFFECTS_COLOUR_SELECT"));
        #endif

        // Check if output multiplying has been set, if so, change desiredcolour array
        // OverwriteUpdateDesiredColourIfMultiplierIsEnabled();

        test_flag^=1;

        // Go through strip, randomly change some to coloured or black
        mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
        uint8_t pixels = mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr);
        RgbTypeColor colour_random = RgbTypeColor(0);

        uint16_t random_pixel_index = 0;
        
        for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
          animation_colours[index].DesiredColour = GetPixelColor(index);
        }

        // randomly blank most of them out again
        uint8_t colour_or_black = 0;
        uint16_t pixels_to_update = pCONT_iLight->animation.transition.pixels_to_update_as_number;
        // pCONT_iLight->GetPixelsToUpdateAsNumberFromPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage);
        Serial.printf("pixels_to_clear=%d\n\r",pixels_to_update);
        for(uint16_t index=0; index<pixels_to_update; index++ ){
          random_pixel_index = random(0,pCONT_iLight->settings.light_size_count);
          colour_or_black = random(0,1);

          // // Check if pixels is already on, if so, make dark
          // if(pCONT_iLight->RgbColorto32bit(GetPixelColor(random_pixel_index))>0){
          //   animation_colours[random_pixel_index].DesiredColour = RgbColor(0,0,0);
          // }else
          // // Previously off, pick new state
          // {
            if(colour_or_black){ // 80% in favour of black/off
              animation_colours[random_pixel_index].DesiredColour = RgbColor(0,0,0);
            }else{
              uint8_t desired_pixel = random(0,pixels-1);
              colour_random = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel);  
              // if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
              //   colour_random = ApplyBrightnesstoDesiredColour(colour_random,pCONT_iLight->getBriRGB());
              // }
              animation_colours[random_pixel_index].DesiredColour =  colour_random;//RgbColor(0,0,255);//
           }
          // }
        }
        // if(test_flag){
        //   animation_colours[random_pixel_index].DesiredColour =  RgbColor(0,0,255);
        // }else{
        //   animation_colours[random_pixel_index].DesiredColour =  RgbColor(0,255,0);
        // }

        // AddLog(LOG_LEVEL_TEST, PSTR("From G,B=%d,%d"),animation_colours[random_pixel_index].DesiredColour.G,animation_colours[random_pixel_index].DesiredColour.B);

        // Get starting positions already on show
        UpdateStartingColourWithGetPixel();
        // AddLog(LOG_LEVEL_TEST, PSTR("From G,B=%d,%d"),animation_colours[random_pixel_index].StartingColor.G,animation_colours[random_pixel_index].StartingColor.B);


        // dont automatically run animate again, add timeout for it
        flashersettings.region = EFFECTS_REGION_ANIMATE_ID;

      }break;
      case EFFECTS_REGION_ANIMATE_ID: //shift along
        #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
        AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "Random_On_2 EFFECTS_ANIMATE"));
        #endif

        this->setAnimFunctionCallback(
          [this](const AnimationParam& param){
            this->AnimUpdateMemberFunction_Pulse_Random_On_2(param);
          }
        );

        if(flashersettings.flags.force_finish_flasher_pair_once){
          AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("flashersettings.flags.force_finish_flasher_pair_once WAS SET"));
          flashersettings.flags.force_finish_flasher_pair_once = false;
        }
        
        flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID;
        
      break;
    }
  }while(flashersettings.flag_finish_flasher_pair || flashersettings.flags.force_finish_flasher_pair_once);

}


// 50% progress is full brightness, 0 and 100% is off
void mAnimatorLight::AnimUpdateMemberFunction_Pulse_Random_On_2(const AnimationParam& param)
{    
 RgbTypeColor updatedColor;
  for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    updatedColor = RgbTypeColor::LinearBlend(
        animation_colours[pixel].StartingColor,
        animation_colours[pixel].DesiredColour,
        param.progress);    
    SetPixelColor(pixel, updatedColor);
  } // END for
  
  // AddLog(LOG_LEVEL_TEST, PSTR("-->> G,B=%d,%d"),updatedColor.G, updatedColor.B);

}



/**************************************************************************************************************************************************************
***************************************************************************************************************************************************************
********* SubTask_Flasher_Animate_Function_Pulse_Random_On_Fade_Off *************************************************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Flasher_Animate_Function_Pulse_Random_On_Fade_Off(){

  pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
  
  flashersettings.flag_finish_flasher_pair = false;
  flashersettings.flags.enable_random_rate = true;
  
  do{ //must complete the pair together
    switch(flashersettings.region){
      case EFFECTS_REGION_COLOUR_SELECT_ID:{ //set colours
        //#ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
        AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "Random_On_2 EFFECTS_COLOUR_SELECT"));
        //#endif

        pCONT_iLight->settings.light_size_count = 100;

        // Go through strip, randomly change some to coloured or black
        mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
        uint8_t pixels = mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr);
        RgbTypeColor colour_random = RgbTypeColor(0);

        uint16_t random_pixel_index = 0;

        // Serial.printf("DesiredColour1=%d,%d,%d\n\r",animation_colours[0].DesiredColour.R,animation_colours[0].DesiredColour.G,animation_colours[0].DesiredColour.B);
        //   animation_colours[0].DesiredColour = GetPixelColor(0);
        // Serial.printf("DesiredColour2=%d,%d,%d\n\r",animation_colours[0].DesiredColour.R,animation_colours[0].DesiredColour.G,animation_colours[0].DesiredColour.B);
        // animation_colours[0].DesiredColour.Darken(20);
        // Serial.printf("DesiredColour3=%d,%d,%d\n\r",animation_colours[0].DesiredColour.R,animation_colours[0].DesiredColour.G,animation_colours[0].DesiredColour.B);
        // Serial.printf("DesiredColour1=%d,%d,%d\n\r",animation_colours[0].DesiredColour.R,animation_colours[0].DesiredColour.G,animation_colours[0].DesiredColour.B);
        
        // for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
        //   animation_colours[index].DesiredColour = GetPixelColor(index);
        //   // animation_colours[index].DesiredColour.Darken(20);//Darken(10);// = GetPixelColor(index);
        
        // }


        UpdateDesiredColourWithGetPixel();

        // randomly blank most of them out again
//        uint8_t colour_or_black = 0;
        uint16_t pixels_to_update = 5;//pCONT_iLight->GetPixelsToUpdateAsNumberFromPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val);
        Serial.printf("pixels_to_clear=%d\n\r",pixels_to_update);
        for(uint16_t index=0; index<pixels_to_update; index++ ){
          random_pixel_index = index;//random(0,100);//pCONT_iLight->settings.light_size_count);
          // colour_or_black = random(0,1);

          // // Check if pixels is already on, if so, make dark
          // if(pCONT_iLight->RgbColorto32bit(GetPixelColor(random_pixel_index))>0){
          //   animation_colours[random_pixel_index].DesiredColour = RgbColor(0,0,0);
          // }else
          // // Previously off, pick new state
          // {
            // if(colour_or_black){ // 80% in favour of black/off
            //   animation_colours[random_pixel_index].DesiredColour = RgbColor(0,0,0);
            // }else{
              
              uint8_t desired_pixel = random(0,4);//pixels-1);
              colour_random = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel);  

//if already on, dont change the colour
            if(!animation_colours[random_pixel_index].DesiredColour.CalculateBrightness()){// if off, allow new colour 
              if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
                colour_random = ApplyBrightnesstoRgbcctColour(colour_random,pCONT_iLight->getBriRGB_Global());
              }
              animation_colours[random(0,10)].DesiredColour = colour_random;//RgbColor(0,0,255);//
            }else{
              
              animation_colours[index].DesiredColour.Darken(2);//Darken(10);// = GetPixelColor(index);
            }
           
          // }
        }
        // if(test_flag){
        //   animation_colours[random_pixel_index].DesiredColour =  RgbColor(0,0,255);
        // }else{
        //   animation_colours[random_pixel_index].DesiredColour =  RgbColor(0,255,0);
        // }
        //  for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
        //   animation_colours[index].DesiredColour.Darken(10);//Darken(10);// = GetPixelColor(index);
        // }


        AddLog(LOG_LEVEL_TEST, PSTR("From G,B=%d,%d"),animation_colours[random_pixel_index].DesiredColour.G,animation_colours[random_pixel_index].DesiredColour.B);

        // Get starting positions already on show
        UpdateStartingColourWithGetPixel();
        // AddLog(LOG_LEVEL_TEST, PSTR("From G,B=%d,%d"),animation_colours[random_pixel_index].StartingColor.G,animation_colours[random_pixel_index].StartingColor.B);


        // dont automatically run animate again, add timeout for it
        flashersettings.region = EFFECTS_REGION_ANIMATE_ID;

      }break;
      case EFFECTS_REGION_ANIMATE_ID: //shift along
        //#ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
        AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "Random_On_2 EFFECTS_ANIMATE"));
        //#endif

        this->setAnimFunctionCallback(
          [this](const AnimationParam& param){
            this->AnimUpdateMemberFunction_Pulse_Random_On_Fade_Off(param);
          }
        );

        if(flashersettings.flags.force_finish_flasher_pair_once){
          AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("flashersettings.flags.force_finish_flasher_pair_once WAS SET"));
          flashersettings.flags.force_finish_flasher_pair_once = false;
        }
        
        flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID;
        
      break;
    }
  }while(flashersettings.flag_finish_flasher_pair || flashersettings.flags.force_finish_flasher_pair_once);

}


// 50% progress is full brightness, 0 and 100% is off
void mAnimatorLight::AnimUpdateMemberFunction_Pulse_Random_On_Fade_Off(const AnimationParam& param)
{    
 RgbTypeColor updatedColor;
 pCONT_iLight->settings.light_size_count = 10;
  for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    
    if(pixel==0){
      // Serial.printf("[%d]\t%d | %d\n\r",pixel,animation_colours[pixel].StartingColor.CalculateBrightness(),animation_colours[pixel].DesiredColour.CalculateBrightness());
    }
    //if desired is brighter than starting, don't blend, do instant (ie pulsed on not fading off)
    if(animation_colours[pixel].StartingColor.CalculateBrightness() <= animation_colours[pixel].DesiredColour.CalculateBrightness()){
      // Serial.println("<<<<<<<<<<<<<<<<<<<<<");
      updatedColor = animation_colours[pixel].DesiredColour; // instant to brighter colour
    }
    // Fade down if new brightness is lower
     else{
       //Serial.println(">>>>>>>>>>>>>>>>>");
      updatedColor = RgbTypeColor::LinearBlend(
        animation_colours[pixel].StartingColor,
        animation_colours[pixel].DesiredColour,
        param.progress);    
    }


    SetPixelColor(pixel, updatedColor);
  } // END for
  
  // AddLog(LOG_LEVEL_TEST, PSTR("-->> G,B=%d,%d"),updatedColor.G, updatedColor.B);

}




/**************************************************************************************************************************************************************
***************************************************************************************************************************************************************
********* Twinkle_SingleColour_Random *************************************************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Flasher_Animate_Function_Twinkle_SingleColour_Random(){

  switch(flashersettings.region){
    case EFFECTS_REGION_COLOUR_SELECT_ID:{ //set colours
      AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
      pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
      UpdateDesiredColourFromPaletteSelected();
      UpdateStartingColourWithGetPixel();
      flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
    // NO 
    }break;
    case EFFECTS_REGION_ANIMATE_ID: //shift along
      AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
      this->setAnimFunctionCallback([this](const AnimationParam& param){ this->AnimUpdateMemberFunction_TwinkleSingleColourRandom(param); });
      break;
  }

}

void mAnimatorLight::AnimUpdateMemberFunction_TwinkleSingleColourRandom(const AnimationParam& param)
{   

  // As integer so the if statement checks will not fail due to rounding errors
  uint8_t progress_percentage = param.progress*100; 
  uint8_t brightness_as_percentage = map(pCONT_iLight->rgbcct_controller.getBrightnessRGB255(), 0,255, 0,100);
  uint8_t random_amount = map(shared_flasher_parameters.alternate_random_amount_as_percentage, 0,100, 0,pCONT_iLight->settings.light_size_count);

  /*
  0-10    20-30   40-50   60-70    80-90      //coloured
      10-20   30-40   50-60   70-80   90-0    //white
  */
  if(progress_percentage != shared_flasher_parameters.progress_percentage_last_animated_on){
    shared_flasher_parameters.progress_percentage_last_animated_on = progress_percentage; //update stored value
    switch(progress_percentage){
      case 10:
      case 30:
      case 50:
      case 70:
      case 90:
        for (uint16_t ii = 0; ii < random_amount; ii++){
          SetPixelColor(
            random(0,pCONT_iLight->settings.light_size_count), 
            HsbColor(pCONT_iLight->HueN2F(30),pCONT_iLight->SatN2F(90),pCONT_iLight->BrtN2F(random(0,brightness_as_percentage)))
          );
        }
      break;
      case 0:
      case 20:
      case 40:
      case 60:
      case 80: //go back to coloured
        for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
          SetPixelColor(pixel, animation_colours[pixel].DesiredColour);
        }
      break;
    }
  }

}


/**************************************************************************************************************************************************************
***************************************************************************************************************************************************************
********* Twinkle_PaletteColour_Random *************************************************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************/


void mAnimatorLight::SubTask_Flasher_Animate_Function_Twinkle_PaletteColour_Random(){
  
  switch(flashersettings.region){
    case EFFECTS_REGION_COLOUR_SELECT_ID: //set colours
      AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
      pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
      // if(flashersettings.flags.enable_endingcolour_as_alternate){
      //   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL flashersettings.flags.enable_endingcolour_as_alternate"));
      //   UpdateDesiredColourWithSingleColour(RgbcctColor(0));
      // }
      UpdateDesiredColourFromPaletteSelected();
      // if(flashersettings.flags.enable_startcolour_as_alternate){
      //   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL flashersettings.flags.enable_startcolour_as_alternate"));
      //   UpdateStartingColourWithSingleColour(RgbcctColor(0));
      // }else{
        UpdateStartingColourWithGetPixel();
      // }
      flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
    break;
  //   case EFFECTS_REGION_ANIMATE_ID: //shift along
  //     AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
  //     this->setAnimFunctionCallback([this](const AnimationParam& param){ this->AnimUpdateMemberFunction_TwinkleUsingPaletteColourRandom(param); });
  //     break;
  }

}



void mAnimatorLight::AnimUpdateMemberFunction_TwinkleUsingPaletteColourRandom(const AnimationParam& param)
{   

  // As integer so the if statement checks will not fail due to rounding errors
  uint8_t progress_percentage = param.progress*100; 
  // uint8_t brightness_as_percentage = map(pCONT_iLight->getBriRGB(), 0,255, 0,100);
  uint8_t random_amount = map(shared_flasher_parameters.alternate_random_amount_as_percentage, 0,100, 0,pCONT_iLight->settings.light_size_count);
  RgbTypeColor flash_colour = RgbTypeColor(0);
  uint8_t desired_pixel = 0;
  int16_t pixel_position = -1;
  uint8_t flashed_brightness = 0; // use set brightness or flash brighter as option
  
  // Update pointer of struct
  mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);

  
  // for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
  //   RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
  //       animation_colours[pixel].StartingColor,
  //       animation_colours[pixel].DesiredColour,
  //       param.progress);    
  //   SetPixelColor(pixel, updatedColor);
  // } // END for

  /*
  0-10    20-30   40-50   60-70    80-90      //coloured
      10-20   30-40   50-60   70-80   90-0    //white
  */
  if(progress_percentage != shared_flasher_parameters.progress_percentage_last_animated_on){
    shared_flasher_parameters.progress_percentage_last_animated_on = progress_percentage; //update stored value
      switch(progress_percentage){
      case 10:
      case 30:
      case 50:
      case 70:
      case 90:
        for (uint16_t ii = 0; ii < random_amount; ii++){
          flashed_brightness = random(0,shared_flasher_parameters.alternate_brightness_max);        
          // For random, desired pixel from map will also be random
          desired_pixel = random(0,mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr));
          // get colour from palette
          flash_colour = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel,&pixel_position);
          flash_colour = ApplyBrightnesstoRgbcctColour(flash_colour,flashed_brightness);
          SetPixelColor(
            random(0,pCONT_iLight->settings.light_size_count), 
            flash_colour
          );
        }
      break;
      case 0:
      case 20:
      case 40:
      case 60:
      case 80: //go back to coloured
        for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
          SetPixelColor(pixel, animation_colours[pixel].DesiredColour);
        }
      break;
    }
  }

}



/**************************************************************************************************************************************************************
***************************************************************************************************************************************************************
********* Slow_Fade_Brightness_All *************************************************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************/



/**************************************************************************************************************************************************************
***************************************************************************************************************************************************************
********* Slow_Fade_Brightness_All *************************************************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************/









/**************************************************************************************************************************************************************
***************************************************************************************************************************************************************
********* Slow_Fade_Brightness_All *************************************************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************/








void mAnimatorLight::SubTask_Flasher_Animate_Function_Slow_Fade_Brightness_All(){
     
      
//       case EFFECTS_FUNCTION_SLOW_FADE_BRIGHTNESS_ALL_ID:
//         switch(flashersettings.region){
//           case EFFECTS_REGION_COLOUR_SELECT_ID: //set colours
//             AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
//             UpdateDesiredColourFromPaletteSelected();
//             flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
//           break;
//           case EFFECTS_REGION_ANIMATE_ID: //shift along
//             AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//             // Change brightness from 0 to 100% (rotate)
//             uint8_t direction = flashersettings.function_seq.rate_index%2;//(flashersettings.function_slow_fade.direction^=1);
//             for(ledout.index=0;ledout.index<pCONT_iLight->settings.light_size_count;ledout.index++){ desired_colour[ledout.index].B = direction; }            
//             // Change pCONT_iLight->animation speed
//             if(mTime::TimeReached(&flashersettings.function_seq.tSavedNewSpeedUpdate,(random(3,10)*100))){
//               uint8_t values[8] = {1000,1000,2000,2000,6000,6000,3000,3000}; //off,on,off
//               pCONT_iLight->animation.transition.rate_ms = values[flashersettings.function_seq.rate_index++]*50;
//               if(flashersettings.function_seq.rate_index>=8) flashersettings.function_seq.rate_index=0;
//               pCONT_iLight->animation.transition.time_ms = pCONT_iLight->animation.transition.rate_ms;///4; // brightness shift takes 100% of the time (no colour shift)
//             }
//           break;
//         }
//       break;
}



/**************************************************************************************************************************************************************
***************************************************************************************************************************************************************
********* Slow_Fade_Saturation_All *************************************************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************/






void mAnimatorLight::SubTask_Flasher_Animate_Function_Slow_Fade_Saturation_All(){
      
//       case EFFECTS_FUNCTION_SLOW_FADE_SATURATION_ALL_ID:
//         switch(flashersettings.region){
//           case EFFECTS_REGION_COLOUR_SELECT_ID: //set colours
//             AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
//             UpdateDesiredColourFromPaletteSelected();
//             flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
//           break;
//           case EFFECTS_REGION_ANIMATE_ID: //shift along
//             AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//             // Change brightness from 0 to 100% (rotate)
//             uint8_t direction = flashersettings.function_seq.rate_index%2;//(flashersettings.function_slow_fade.direction^=1);
//             float random_saturation = direction ? 1 : ((float)random(0,100)/(float)100);

//             float adjusted_brightness = random_saturation;// mSupport::mapfloat(random_saturation,
//             if(random_saturation<0.6){
//               adjusted_brightness = mSupport::mapfloat(random_saturation,
//                                                                     0,  0.6,
//                                                                     0.4,0.6);
//             }
                        
//             for(ledout.index=0;ledout.index<pCONT_iLight->settings.light_size_count;ledout.index++){ 
//               desired_colour[ledout.index].S = random_saturation; 
//               desired_colour[ledout.index].B = adjusted_brightness;//random_saturation<0.5?pCONT_iLight->animation.brightness*0.5:pCONT_iLight->animation.brightness; //test, pair "whiter" less bright (maybe /2)  
//             }            
//             // Change pCONT_iLight->animation speed
//             //if(mTime::TimeReached(&flashersettings.function_seq.tSavedNewSpeedUpdate,(random(3,10)*100))){
//               uint16_t values[8] = {7,7,9,9,7,7,4,4}; //off,on,off
//               pCONT_iLight->animation.transition.rate_ms = values[flashersettings.function_seq.rate_index++]*1000;
//               if(flashersettings.function_seq.rate_index>=8) flashersettings.function_seq.rate_index=0;
//               // holds colour more
//               //pCONT_iLight->animation.transition.time_ms = direction ? (pCONT_iLight->animation.transition.rate_ms/4) : (pCONT_iLight->animation.transition.rate_ms); // brightness shift takes 100% of the time (no colour shift)
//               pCONT_iLight->animation.transition.time_ms = random(0,1) ? (pCONT_iLight->animation.transition.rate_ms/4) : (pCONT_iLight->animation.transition.rate_ms.va;); // brightness shift takes 100% of the time (no colour shift)
//             //}
//           break;
//         }
//       break;
//       //flasher_start_white_to_Colour: 
//         // method that will start all leds on one colour (white?) and slowly add colours in with fade in
//       //flasher_change_saturation_between 0 and 100, "fade of saturation", "fade of brightness"

}




// simple blend function
void mAnimatorLight::AnimUpdateMemberFunction(const AnimationParam& param)
{    
  // if(param.progress == 0){ // 0%   = Starting
  //   for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
  //     SetPixelColor(pixel, animation_colours[pixel].StartingColor);
  //   }
  // }else
  // if(param.progress == 1){ // 100% = Finishing
  //   for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
  //     SetPixelColor(pixel, animation_colours[pixel].DesiredColour);
  //   }
  // }
}

void mAnimatorLight::AnimUpdateMemberFunction_TraditionalChasing(const AnimationParam& param)
{    

  // As integer so the if statement checks will not fail due to rounding errors
  uint8_t progress_percentage = param.progress*100; 


  // if(progress_percentage == 0){ // 0%   = Starting
  //   for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
  //     SetPixelColor(pixel, RgbTypeColor(255,0,0));//animation_colours[pixel].StartingColor);
  //   }
  // }else
  // if(progress_percentage == 25){ // 100% = Finishing
  //   for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
  //     SetPixelColor(pixel, RgbTypeColor(0,255,0));
  //   }
  // }else
  // if(progress_percentage == 50){ // 100% = Finishing
  //   for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
  //     SetPixelColor(pixel, RgbTypeColor(0,0,255));
  //   }
  // }else
  // if(progress_percentage == 75){ // 100% = Finishing
  //   for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
  //     SetPixelColor(pixel, RgbTypeColor(255,0,255));
  //   }
  // }


    // for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    //   SetPixelColor(pixel, RgbTypeColor(0,0,0));
    // }


  if(progress_percentage == 0){ // 0%   = Starting
    for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
      switch(pixel%4){
        case 0: SetPixelColor(pixel, RgbTypeColor(255,0,0)); break;
        case 1: SetPixelColor(pixel, RgbTypeColor(0,255,0)); break;
        case 2: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
        case 3: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
      }
      
    }
  }else
  if(progress_percentage == 25){ // 100% = Finishing
    for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
      switch(pixel%4){
        case 0: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
        case 1: SetPixelColor(pixel, RgbTypeColor(0,255,0)); break;
        case 2: SetPixelColor(pixel, RgbTypeColor(0,0,255)); break;
        case 3: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
      }
    }
  }else
  if(progress_percentage == 50){ // 100% = Finishing
    for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
      switch(pixel%4){
        case 0: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
        case 1: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
        case 2: SetPixelColor(pixel, RgbTypeColor(0,0,255)); break;
        case 3: SetPixelColor(pixel, RgbTypeColor(255, 153, 0)); break;
      }
    }
  }else
  if(progress_percentage == 75){ // 100% = Finishing
    for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
      switch(pixel%4){
        case 0: SetPixelColor(pixel, RgbTypeColor(255,0,0)); break;
        case 1: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
        case 2: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
        case 3: SetPixelColor(pixel, RgbTypeColor(255, 153, 0)); break;
      }
    }
  }



  // else
  // if(param.progress == 1){ // 100% = Finishing
  //   for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
  //     SetPixelColor(pixel, RgbTypeColor(255,255,255));
  //   }
  // }

  
    // for (uint16_t pixel = 1; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    //   SetPixelColor(pixel, RgbTypeColor(0,0,0));
    // }



}


void mAnimatorLight::AnimUpdateMemberFunction_TraditionalChasing2(const AnimationParam& param)
{    

  // As integer so the if statement checks will not fail due to rounding errors
  uint8_t progress_percentage = param.progress*100; 

    // for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    //   SetPixelColor(pixel, RgbTypeColor(0,0,0));
    // }

  if(progress_percentage == 0){ // 0%   = Starting
    for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
      SetPixelColor(pixel, RgbTypeColor(255,0,0));//animation_colours[pixel].StartingColor);
    }
  }else
  if(progress_percentage == 25){ // 100% = Finishing
    for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
      SetPixelColor(pixel, RgbTypeColor(0,255,0));
    }
  }else
  if(progress_percentage == 50){ // 100% = Finishing
    for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
      SetPixelColor(pixel, RgbTypeColor(0,0,255));
    }
  }else
  if(progress_percentage == 75){ // 100% = Finishing
    for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
      SetPixelColor(pixel, RgbTypeColor(255,0,255));
    }
  }




  // if(progress_percentage == 0){ // 0%   = Starting
  //   for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
  //     switch(pixel%4){
  //       case 0: SetPixelColor(pixel, RgbTypeColor(255,0,0)); break;
  //       case 1: SetPixelColor(pixel, RgbTypeColor(0,255,0)); break;
  //       case 2: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
  //       case 3: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
  //     }
      
  //   }
  // }else
  // if(progress_percentage == 25){ // 100% = Finishing
  //   for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
  //     switch(pixel%4){
  //       case 0: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
  //       case 1: SetPixelColor(pixel, RgbTypeColor(0,255,0)); break;
  //       case 2: SetPixelColor(pixel, RgbTypeColor(0,0,255)); break;
  //       case 3: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
  //     }
  //   }
  // }else
  // if(progress_percentage == 50){ // 100% = Finishing
  //   for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
  //     switch(pixel%4){
  //       case 0: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
  //       case 1: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
  //       case 2: SetPixelColor(pixel, RgbTypeColor(0,0,255)); break;
  //       case 3: SetPixelColor(pixel, RgbTypeColor(255, 153, 0)); break;
  //     }
  //   }
  // }else
  // if(progress_percentage == 75){ // 100% = Finishing
  //   for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
  //     switch(pixel%4){
  //       case 0: SetPixelColor(pixel, RgbTypeColor(255,0,0)); break;
  //       case 1: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
  //       case 2: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
  //       case 3: SetPixelColor(pixel, RgbTypeColor(255, 153, 0)); break;
  //     }
  //   }
  // }



  // else
  // if(param.progress == 1){ // 100% = Finishing
  //   for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
  //     SetPixelColor(pixel, RgbTypeColor(255,255,255));
  //   }
  // }

  
    // for (uint16_t pixel = 1; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    //   SetPixelColor(pixel, RgbTypeColor(0,0,0));
    // }



}






// simple blend function
void mAnimatorLight::AnimUpdate_ShowStartingThenDesiredColors(const AnimationParam& param)
{    
  // if(param.progress == 0){ // 0%   = Starting
  //   for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
  //     SetPixelColor(pixel, animation_colours[pixel].StartingColor);
  //   }
  // }else
  // if(param.progress == 1){ // 100% = Finishing
  //   for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
  //     SetPixelColor(pixel, animation_colours[pixel].DesiredColour);
  //   }
  // }
}



// simple blend function
void mAnimatorLight::BlendAnimUpdate(const AnimationParam& param)
{    
  for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
        animation_colours[pixel].StartingColor,
        animation_colours[pixel].DesiredColour,
        param.progress);
    SetPixelColor(pixel, updatedColor);
  } // END for
}






/**************************************************************************************************************************************************************
***************************************************************************************************************************************************************
********* Twinkle_SingleColour_Random *************************************************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************/

// Make this an "Effects" which is public/friendly of animator, but not the same class
void mAnimatorLight::SubTask_Flasher_Animate_Function_SunPositions_Gradient_Alarm_01(){

  
      AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SubTask_Flasher_Animate_Function_SunPositions_Gradient_Alarm_01"));

  //using group 01
  // struct{}
  //use struct and memcpy to get data, then store results

  //effects_data_buffer
  //each needs 64 byte
// typedef struct particle {
//   // CRGB     color;
//   uint32_t birth  =0;
//   uint32_t last   =0;
//   float    vel    =0;
//   uint16_t pos    =-1;
//   float    fragment[12];
// } star;

/**
 * How the effects_data_buffer is formatted during effect runtime
 * */
struct EFFECT_CONFIG {
  uint8_t starting_palette_id = 0;
  uint8_t ending_palette_id = 10;
  uint8_t step_changes_remaining = 10;
  uint8_t colour_select_progress_counter = 0;
  uint8_t active_palette_id;

} effect_config;


  // star* stars = reinterpret_cast<star*>(pCONT_iLight->effects_data_buffer);
  EFFECT_CONFIG* effect_config2 = reinterpret_cast<EFFECT_CONFIG*>(pCONT_iLight->effects_data_buffer);
  //should be memcpy be used to insure size if not exceeded? or sizeof check? pointer is faster




  // switch(flashersettings.region){
  //   case EFFECTS_REGION_COLOUR_SELECT_ID:{ //set colours


      // if(effect_config2->active_palette_id++>mPaletteI->PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_ID){
      //   effect_config2->active_palette_id = mPaletteI->PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_01_ID;
      // }
      if(pCONT_iLight->animation.palette.id++>=mPaletteI->PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_ID){
        pCONT_iLight->animation.palette.id = mPaletteI->PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_01_ID;
      }


AddLog(LOG_LEVEL_TEST,PSTR("pCONT_iLight->animation.palette.id=%d"),pCONT_iLight->animation.palette.id);


      pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
      UpdateDesiredColourFromPaletteSelected();



      UpdateStartingColourWithGetPixel();



    //   flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
    // // NO 
    // }break;
    // case EFFECTS_REGION_ANIMATE_ID: //shift along
      AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
      this->setAnimFunctionCallback([this](const AnimationParam& param){ this->AnimUpdateMemberFunction_SunPositions_Gradient_Alarm_01(param); });
      
  //     flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID;
  //     break;
  // }

}

void mAnimatorLight::AnimUpdateMemberFunction_SunPositions_Gradient_Alarm_01(const AnimationParam& param)
{   
  // Need to make rgbcct pixels possible in the future, with dynamic animation_state

   for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
        animation_colours[pixel].StartingColor,
        animation_colours[pixel].DesiredColour,
        param.progress);    
    SetPixelColor(pixel, updatedColor);
  } // END for

}

/**************************************************************************************************************************************************************
***************************************************************************************************************************************************************
********* Twinkle_SingleColour_Random *************************************************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************/

// Make this an "Effects" which is public/friendly of animator, but not the same class
void mAnimatorLight::SubTask_Flasher_Animate_Function_SunPositions_Gradient_Sun_Elevation_And_Azimuth_01(){

  
      AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SunPositions_Gradient_Sun_Elevation_And_Azimuth_01"));

  //using group 01
  // struct{}
  //use struct and memcpy to get data, then store results

  //effects_data_buffer
  //each needs 64 byte
// typedef struct particle {
//   // CRGB     color;
//   uint32_t birth  =0;
//   uint32_t last   =0;
//   float    vel    =0;
//   uint16_t pos    =-1;
//   float    fragment[12];
// } star;

/**
 * How the effects_data_buffer is formatted during effect runtime
 * */
struct EFFECT_CONFIG {
  uint16_t index_circle_center;
  
  uint8_t sun_width;
  uint8_t sun_bloom;
  RgbcctColor sky_near_colour;
  RgbcctColor sky_far_colour;

  uint8_t value = 0;





  uint8_t starting_palette_id;
  uint8_t ending_palette_id;
  uint8_t step_changes_remaining;
  uint8_t colour_select_progress_counter;
  uint8_t active_palette_id;
  uint8_t sun_elevation_degrees;
  uint8_t sun_azimuth_degrees; //left/right from sunrise/sunset position
  // uint8_t sun_bloom; //how wide the main sunlight is
  //store colours in here too
  // uint8_t sun_elevation_degrees;


  uint8_t direction;

  //map "visible" azimuth into far left/right of strip

} effect_config;


  // star* stars = reinterpret_cast<star*>(pCONT_iLight->effects_data_buffer);
  EFFECT_CONFIG* effect_config2 = reinterpret_cast<EFFECT_CONFIG*>(pCONT_iLight->effects_data_buffer);
  //should be memcpy be used to insure size if not exceeded? or sizeof check? pointer is faster

  AddLog(LOG_LEVEL_TEST,PSTR("pCONT_iLight->animation.palette.id=%d"),pCONT_iLight->animation.palette.id);

  pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;

  pCONT_iLight->settings.light_size_count = 100;
  
  // Get all the current state
  // for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
  //   animation_colours[index].DesiredColour = GetPixelColor(index);
  // }
    for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
    animation_colours[index].DesiredColour = RgbcctColor(0);//,0,30,0,0);//,0,10,0,0);
  }

  //scan back and forth
  // if(effect_config2->index_circle_center<=0){
  //   effect_config2->direction = 1;
  // }else
  // if(effect_config2->index_circle_center>100){
  //   effect_config2->direction = 0;
  // }

  // if(effect_config2->direction){          
  //   effect_config2->index_circle_center++;
  // }else{
  //   effect_config2->index_circle_center--;
  // }

  effect_config2->index_circle_center = 50;
  
  if(effect_config2->value >= 0){
    // Draw full sky
    Draw_DesiredColour_LinearGradientMirrored2(
      RgbcctColor(0,0,50,0,0), //center
      RgbcctColor(0,0,0,0,0), //circum
      effect_config2->index_circle_center, //
      50,  // tail_size
      5, // center region
      true
    );
  }


  // widden
  if(effect_config2->value >= 1){

    // Draw near sky
    Draw_DesiredColour_LinearGradientMirrored2(
      RgbcctColor(20,5,0,0,0), //center
      RgbcctColor(0,0,0,0,0), //circum
      effect_config2->index_circle_center, //
      25,  // tail_size
      10, // center region
      true
    );
  }

  if(effect_config2->value >= 2){
    // Draw near sky
    Draw_DesiredColour_LinearGradientMirrored2(
      RgbcctColor(100,20,0,0,0), //center
      RgbcctColor(0,0,0,0,0), //circum
      effect_config2->index_circle_center, //
      30,  // tail_size
      2, // center region
      true
    );
  }

  // widden
  if(effect_config2->value >= 3){

    // Draw near sky
    Draw_DesiredColour_LinearGradientMirrored2(
      RgbcctColor(0,50,50,0,0), //center
      RgbcctColor(0,100,75,0,0), //circum
      effect_config2->index_circle_center, //
      60,  // tail_size
      10, // center region
      true
    );
  }


  if(effect_config2->value >= 4){
    // Draw sun bloom
    Draw_DesiredColour_LinearGradientMirrored2(
      RgbcctColor(200,100,0,0,0), //center
      RgbcctColor(50, 44, 3,     0,0), //circum
      effect_config2->index_circle_center, //D_RGB255_ORANGE_DEEP
      50,
      5, 
      true
    );
  }

  // if(effect_config2->value == 4){
  //   // Draw sun bloom
  //   Remove_DesiredColour_LinearGradientMirrored(
  //     RgbcctColor(0,0,  50,       0,0), //center
  //     RgbcctColor(0, 0, 10,     0,0), //circum
  //     effect_config2->index_circle_center, //D_RGB255_ORANGE_DEEP
  //     50,
  //     5, 
  //     true
  //   );
  // }


  
  if(effect_config2->value++ > 6){ effect_config2->value = 0; }

        // pixel_radius_as_number = 10;
        // pixel_position_center = effect_config2->index_circle_center;
        // pixel_position_circum = effect_config2->index_circle_center-pixel_radius_as_number;
        // for(uint16_t index=0; index<pixel_radius_as_number; index++ ){
        //   // Create new index
        //   index_generate = effect_config2->index_circle_center-index;              
        //   //Only write into colour buffer if valid index. Otherwise, consider this an "overscan"
        //   if((index_generate>=0)&&(index_generate<100)){
        //     //Draw half, larger index side
        //     progress = mSupport::mapfloat(index_generate,
        //                                     pixel_position_center,pixel_position_circum,
        //                                     0,1
        //                                   );
        //     animation_colours[index_generate].DesiredColour = RgbcctColor::LinearBlend(
        //                                                                                 colour_circle_center,
        //                                                                                 colour_circle_circumference, 
        //                                                                                 //RgbColor(255,0,0), RgbColor(0,255,0)
        //                                                                                 progress);
        //   } // if test

        // }





// //use signed value to hold index_adjusted
// int32_t index_generate = 0;
// uint16_t pixel_position_center, pixel_position_circum, pixel_radius_as_number;

//         pixel_radius_as_number = 10;
//         pixel_position_center = effect_config2->index_circle_center;
//         pixel_position_circum = effect_config2->index_circle_center+pixel_radius_as_number;
//         for(uint16_t index=0; index<pixel_radius_as_number; index++ ){
//           // Create new index
//           index_generate = effect_config2->index_circle_center+index;   
          
//           //Draw half, larger index side
//           progress = mSupport::mapfloat(index_generate,
//                                           pixel_position_center,pixel_position_circum,
//                                           0,1
//                                         );
//           colour_draw = RgbcctColor::LinearBlend(
//                                         colour_circle_center,
//                                         colour_circle_circumference, 
//                                         //RgbColor(255,0,0), RgbColor(0,255,0)
//                                         progress);



//           //Only write into colour buffer if valid index. Otherwise, consider this an "overscan"
//           if((index_generate>=0)&&(index_generate<100)){
//             animation_colours[index_generate].DesiredColour = colour_draw;
//           } // if test

//         }

//         pixel_radius_as_number = 10;
//         pixel_position_center = effect_config2->index_circle_center;
//         pixel_position_circum = effect_config2->index_circle_center-pixel_radius_as_number;
//         for(uint16_t index=0; index<pixel_radius_as_number; index++ ){
//           // Create new index
//           index_generate = effect_config2->index_circle_center-index;              
//           //Only write into colour buffer if valid index. Otherwise, consider this an "overscan"
//           if((index_generate>=0)&&(index_generate<100)){
//             //Draw half, larger index side
//             progress = mSupport::mapfloat(index_generate,
//                                             pixel_position_center,pixel_position_circum,
//                                             0,1
//                                           );
//             animation_colours[index_generate].DesiredColour = RgbcctColor::LinearBlend(
//                                                                                         colour_circle_center,
//                                                                                         colour_circle_circumference, 
//                                                                                         //RgbColor(255,0,0), RgbColor(0,255,0)
//                                                                                         progress);
//           } // if test

//         }





          // index_generate = effect_config2->index_circle_center-index+10;
          // start_pixel_position = index_generate;
          // end_pixel_position = index_generate+10;
          // if((index_generate>=0)&&(index_generate<100)){
          //   //Draw half, samller index side
          //   progress = mSupport::mapfloat(index_generate,start_pixel_position,end_pixel_position,0,1);
          //   animation_colours[index_generate].DesiredColour = RgbcctColor::LinearBlend(RgbColor(255,0,255), RgbColor(0,0,255), progress);
              
              
          //     // colour_circle_circumference, colour_circle_center, progress);
          // }// out of range

        // }


        // randomly blank most of them out again
        // uint8_t colour_or_black = 0;
        // uint16_t pixels_to_update = pCONT_iLight->animation.transition.pixels_to_update_as_number;
        // // pCONT_iLight->GetPixelsToUpdateAsNumberFromPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage);
        // Serial.printf("pixels_to_clear=%d\n\r",pixels_to_update);
        // for(uint16_t index=0; index<pixels_to_update; index++ ){
        //   random_pixel_index = random(0,pCONT_iLight->settings.light_size_count);
        //   colour_or_black = random(0,1);

        //   // // Check if pixels is already on, if so, make dark
        //   // if(pCONT_iLight->RgbColorto32bit(GetPixelColor(random_pixel_index))>0){
        //   //   animation_colours[random_pixel_index].DesiredColour = RgbColor(0,0,0);
        //   // }else
        //   // // Previously off, pick new state
        //   // {
        //     if(colour_or_black){ // 80% in favour of black/off
        //       animation_colours[random_pixel_index].DesiredColour = RgbColor(0,255,0);
        //     }else{
        //       uint8_t desired_pixel = random(0,pixels-1);
        //       colour_random = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel);  
        //       // if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
        //       //   colour_random = ApplyBrightnesstoDesiredColour(colour_random,pCONT_iLight->getBriRGB());
        //       // }
        //       animation_colours[random_pixel_index].DesiredColour =  colour_random;//RgbColor(0,0,255);//
        //    }
        //   // }
        // }




      // UpdateDesiredColourFromPaletteSelected();


      UpdateStartingColourWithGetPixel();



    //   flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
    // // NO 
    // }break;
    // case EFFECTS_REGION_ANIMATE_ID: //shift along
      AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
      this->setAnimFunctionCallback([this](const AnimationParam& param){ this->AnimUpdateMemberFunction_SunPositions_Gradient_Sun_Elevation_And_Azimuth_01(param); });
      
  //     flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID;
  //     break;
  // }

}

void mAnimatorLight::AnimUpdateMemberFunction_SunPositions_Gradient_Sun_Elevation_And_Azimuth_01(const AnimationParam& param)
{   
  // Need to make rgbcct pixels possible in the future, with dynamic animation_state

   for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
        animation_colours[pixel].StartingColor,
        animation_colours[pixel].DesiredColour,
        param.progress);    
    SetPixelColor(pixel, updatedColor);
  } // END for

}



// void mAnimatorLight::SubTask_Flasher_Animate_Function_Sequential(){

//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
  
//   flashersettings.flag_finish_flasher_pair = false;
//   flashersettings.flags.enable_random_rate = true;
  
//   do{ //must complete the pair together //move inside functions
//     switch(flashersettings.region){
//       case EFFECTS_REGION_COLOUR_SELECT_ID: //set colours
//         #ifdef ENABLE_LOG_LEVEL_DEBUG
//         AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
//         #endif
//         UpdateDesiredColourFromPaletteSelected();
//         flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
//       break;
//       case EFFECTS_REGION_ANIMATE_ID: //shift along
//         #ifdef ENABLE_LOG_LEVEL_DEBUG
//         AddLog(LOG_LEVEL_DEBUG_LOWLEVEL,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//         #endif
//         RotateDesiredColour(1,flashersettings.flags.movement_direction);
//         if(flashersettings.flags.force_finish_flasher_pair_once){
//           AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("flashersettings.flags.force_finish_flasher_pair_once WAS SET"));
//           flashersettings.flags.force_finish_flasher_pair_once = false;
//         }
//       break;
//     }
//   }while(flashersettings.flag_finish_flasher_pair || flashersettings.flags.force_finish_flasher_pair_once);

// }

// void mAnimatorLight::SubTask_Flasher_Animate_Function_Slow_Glow_On_Brightness(){

//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = false;

//   flashersettings.flag_finish_flasher_pair = false;
//   flashersettings.flags.enable_random_rate = true;

//   // Best "inorder" to compute colour again then brightness
  
//   do{ //must complete the pair together //move inside functions
//     switch(flashersettings.region){
//       case EFFECTS_REGION_COLOUR_SELECT_ID: //set colours
//         #ifdef ENABLE_LOG_LEVEL_DEBUG
//         AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
//         #endif
//         UpdateDesiredColourFromPaletteSelected();
//         flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
//       break;
//       case EFFECTS_REGION_ANIMATE_ID: //shift along
//         #ifdef ENABLE_LOG_LEVEL_DEBUG
//         AddLog(LOG_LEVEL_DEBUG_LOWLEVEL,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//         #endif
//         // RotateDesiredColour(1,flashersettings.flags.movement_direction);

//         // Calculate new pattern to change
//         RefreshLEDIndexPattern();

//         // Global brightness is already applied, and will be known as "max range"
//         // Min range will be another map change here
//         uint8_t max_brightness = pCONT_iLight->getBriRGB();
//         uint8_t min_brightness = flashersettings.brightness_min;
//         uint8_t random_brightness = 0;

//         if(min_brightness > max_brightness){
//           min_brightness = max_brightness;
//         }

//         // Change only selected pixel brightness levels
//         for(ledout.index=0;ledout.index<strip_size_requiring_update;ledout.index++){
//           random_brightness = random(min_brightness,max_brightness);          
//           animation_colours[ledout.pattern[ledout.index]].DesiredColour = ApplyBrightnesstoDesiredColour(
//                                                                               animation_colours[ledout.pattern[ledout.index]].DesiredColour,
//                                                                               random_brightness);
//         }

//         if(flashersettings.flags.force_finish_flasher_pair_once){
//           AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("flashersettings.flags.force_finish_flasher_pair_once WAS SET"));
//           flashersettings.flags.force_finish_flasher_pair_once = false;
//         }
//       break;
//     }
//   }while(flashersettings.flag_finish_flasher_pair || flashersettings.flags.force_finish_flasher_pair_once);


// }


// // Fade solid colour from 0 to 75%, and a palette from 25 to 100% (50% overlap)
// void mAnimatorLight::SubTask_Flasher_Animate_Function_Fade_Gradient(){
    
// #ifdef ENABLE_EFFECTS_ANIMATE_FUNCTION_FADE_GRADIENT

//   flashersettings.flag_finish_flasher_pair = false;
//   flashersettings.flags.enable_random_rate = true;
  
//   // Apply green gradient, brightest at start

//   uint16_t start = pCONT_iLight->settings.light_size_count/2;
//   uint16_t end = pCONT_iLight->settings.light_size_count; 
//   RgbTypeColor colour_gradient = HsbColor(
//                                           pCONT_iLight->HueN2F(120),
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

//   pCONT_iLight->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
//   uint8_t pixels = mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr);
//   uint8_t desired_pixel;
  
//   // 25 to 100%
//   start = map(25,0,100,0,pCONT_iLight->settings.light_size_count);
//   end = pCONT_iLight->settings.light_size_count;
//   for(ledout.index=start;ledout.index<end;ledout.index++){ 
//     desired_pixel = random(0,pixels-1);
//     colour_random = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel);
//     if((ledout.index%3)==0){
//       colour_random_adjusted = RgbTypeColor(colour_random);
//       colour_random_adjusted.B = pCONT_iLight->BrtN2F(map(pCONT_iLight->getBriRGB(),0,255,0,100));
//       animation_colours[ledout.index].DesiredColour = colour_random_adjusted;
//     }
//   }

//   #endif

// }


// void mAnimatorLight::SubTask_Flasher_Animate_Function_Twinkle_Random(){

// // #ifdef ENABLE_ADVANCED_MODE_TWINKLE // creating this to reduce "white_leds_index" size

//   switch(flashersettings.region){
//     case EFFECTS_REGION_COLOUR_SELECT_ID: //set colours
//       AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
//       UpdateDesiredColourFromPaletteSelected();
//       flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
//       pCONT_iLight->animation.transition.order_id = TRANSITION_ORDER_INORDER_ID;

//       flash_twinkle_random.white_pixel_amount = 8; //how often the leds are
//       // by percentage
//       //flash_twinkle_random.white_pixel_amount = pCONT_iLight->settings.light_size_count/5;

//       // Generate ledindex for white flashers
//       flash_twinkle_random.white_total_index = 0;
//       for(ledout.index=0;ledout.index<pCONT_iLight->settings.light_size_count;ledout.index++){ 
//         //map type of led they are by numnber 0=off,1=white,2=colour
//         if(!(ledout.index%flash_twinkle_random.white_pixel_amount)){
//           flash_twinkle_random.white_leds_index[flash_twinkle_random.white_total_index] = ledout.index;
//           flash_twinkle_random.stored_colours_index[flash_twinkle_random.white_total_index] = desired_colour[ledout.index];
//           //AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "flash_twinkle_random.white_leds_index %d ledout.index %d"),flash_twinkle_random.white_total_index,ledout.index);
//           flash_twinkle_random.white_total_index++;
//         }              
//       }   
                  
//       //flash_twinkle_random.flash_colour = HsbColor(0,0,1); //white

//       // Test clear
//       // for(int ledout.index=0;ledout.index<pCONT_iLight->settings.light_size_count;ledout.index++){ desired_colour[ledout.index] = HsbColor(RgbColor(0,0,0)); }

//       flashersettings.flag_finish_flasher_pair = true; // must complete regions
      
//     break;

//     case EFFECTS_REGION_ANIMATE_ID: //shift along
//       AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));

//       // Reset all back to colour first 
//       for(int jj=0;jj<flash_twinkle_random.white_total_index;jj++){
//         desired_colour[flash_twinkle_random.white_leds_index[jj]] = flash_twinkle_random.stored_colours_index[jj];
//       }

//       // Pick 10 random of the leds to turn to white

      
//       for(int jj=0;jj<10;jj++){
//         uint16_t led_rand = random(0,flash_twinkle_random.white_total_index);
//         desired_colour[flash_twinkle_random.white_leds_index[led_rand]] = flash_twinkle_random.flash_colour;
//       }

//       flash_twinkle_random.white_on_index++;
//       if(flash_twinkle_random.white_on_index>=flash_twinkle_random.white_pixel_amount){
//         flash_twinkle_random.white_on_index = 0;
//         flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID; // Wrap back
//       }

//       flashersettings.flag_finish_flasher_pair = false; // must complete regions

//       // Change pCONT_iLight->animation speed
//       if(mTime::TimeReached(&flashersettings.function_flash_twinkle.tSavedNewSpeedUpdate,random(0,1)?5000:10000)){//(random(5,10)*1000))){
//         pCONT_iLight->animation.transition.rate_ms = random(0,1)?10:150; // will need to be very fast
//         pCONT_iLight->animation.transition.time_ms.val = 0; //instant
//       }
//     break;
//   }

//   // #endif

// }


// void mAnimatorLight::SubTask_Flasher_Animate_Function_Twinkle_Sequential(){
  
// //       case EFFECTS_FUNCTION_FLASH_TWINKLE_SEQUENTIAL_ID:
// //         switch(flashersettings.region){
// //           case EFFECTS_REGION_COLOUR_SELECT_ID: //set colours
// //             AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
// //             UpdateDesiredColourFromPaletteSelected();
// //             flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
// //             pCONT_iLight->animation.transition.order_id = TRANSITION_ORDER_INORDER_ID;

// //             flash_twinkle_random.white_pixel_amount = 7; //how often the leds are
// //             // by percentage
// //             //flash_twinkle_random.white_pixel_amount = pCONT_iLight->settings.light_size_count/5;

// //             // Generate ledindex for white flashers
// //             flash_twinkle_random.white_total_index = 0;
// //             for(ledout.index=0;ledout.index<pCONT_iLight->settings.light_size_count;ledout.index++){ 
// //               //map type of led they are by numnber 0=off,1=white,2=colour
// //               if(!(ledout.index%flash_twinkle_random.white_pixel_amount)){
// //                 flash_twinkle_random.white_leds_index[flash_twinkle_random.white_total_index] = ledout.index;
// //                 flash_twinkle_random.stored_colours_index[flash_twinkle_random.white_total_index] = desired_colour[ledout.index];
// //                 //AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "flash_twinkle_random.white_leds_index %d ledout.index %d"),flash_twinkle_random.white_total_index,ledout.index);
// //                 flash_twinkle_random.white_total_index++;
// //               }              
// //             }   
                        
// //             //flash_twinkle_random.flash_colour = HsbColor(0,0,1); //white
// //             //flash_twinkle_random.flash_colour = HsbColor(HueN2F(HUE_HOTPINK),SatN2F(100),BrtN2F(100));

// //             // Test clear
// //             // for(int ledout.index=0;ledout.index<pCONT_iLight->settings.light_size_count;ledout.index++){ desired_colour[ledout.index] = HsbColor(RgbColor(0,0,0)); }

// //             flashersettings.flag_finish_flasher_pair = true; // must complete regions
            
// //           break;
// //           case EFFECTS_REGION_ANIMATE_ID: //shift along
// //             AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
    
            
// //             for(int jj=0;jj<flash_twinkle_random.white_total_index;jj++){
// //               if((jj%flash_twinkle_random.white_pixel_amount)==flash_twinkle_random.white_on_index){
// //                 desired_colour[flash_twinkle_random.white_leds_index[jj]] = flash_twinkle_random.flash_colour;
// //               }else{
// //                 desired_colour[flash_twinkle_random.white_leds_index[jj]] = flash_twinkle_random.stored_colours_index[jj];//offcolour;
// //               }
// //               // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "leds_index=%d on/amount=%d/%d on=%d"),
// //               // flash_twinkle_random.white_leds_index[jj],
// //               // flash_twinkle_random.white_on_index,
// //               // flash_twinkle_random.white_pixel_amount,
// //               // (jj%flash_twinkle_random.white_pixel_amount)==0?1:0);
// //             }

// //             flash_twinkle_random.white_on_index++;
// //             if(flash_twinkle_random.white_on_index>=flash_twinkle_random.white_pixel_amount){
// //               flash_twinkle_random.white_on_index = 0;
// //               flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID; // Wrap back
// //             }

// //             flashersettings.flag_finish_flasher_pair = false; // must complete regions

// //             // Change pCONT_iLight->animation speed
// //             if(mTime::TimeReached(&flashersettings.function_flash_twinkle.tSavedNewSpeedUpdate,random(0,1)?5000:10000)){//(random(5,10)*1000))){
// //               pCONT_iLight->animation.transition.rate_ms = 90;//random(0,1)?30:30; // will need to be very fast
// //               pCONT_iLight->animation.transition.time_ms = 0; //instant
// //             }
// //           break;
// //         }
// //       break;

// }

// void mAnimatorLight::SubTask_Flasher_Animate_Function_Slow_Fade_Brightness_All(){
     
      
// //       case EFFECTS_FUNCTION_SLOW_FADE_BRIGHTNESS_ALL_ID:
// //         switch(flashersettings.region){
// //           case EFFECTS_REGION_COLOUR_SELECT_ID: //set colours
// //             AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
// //             UpdateDesiredColourFromPaletteSelected();
// //             flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
// //           break;
// //           case EFFECTS_REGION_ANIMATE_ID: //shift along
// //             AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
// //             // Change brightness from 0 to 100% (rotate)
// //             uint8_t direction = flashersettings.function_seq.rate_index%2;//(flashersettings.function_slow_fade.direction^=1);
// //             for(ledout.index=0;ledout.index<pCONT_iLight->settings.light_size_count;ledout.index++){ desired_colour[ledout.index].B = direction; }            
// //             // Change pCONT_iLight->animation speed
// //             if(mTime::TimeReached(&flashersettings.function_seq.tSavedNewSpeedUpdate,(random(3,10)*100))){
// //               uint8_t values[8] = {1000,1000,2000,2000,6000,6000,3000,3000}; //off,on,off
// //               pCONT_iLight->animation.transition.rate_ms = values[flashersettings.function_seq.rate_index++]*50;
// //               if(flashersettings.function_seq.rate_index>=8) flashersettings.function_seq.rate_index=0;
// //               pCONT_iLight->animation.transition.time_ms = pCONT_iLight->animation.transition.rate_ms;///4; // brightness shift takes 100% of the time (no colour shift)
// //             }
// //           break;
// //         }
// //       break;
// }


// void mAnimatorLight::SubTask_Flasher_Animate_Function_Slow_Fade_Saturation_All(){
      
// //       case EFFECTS_FUNCTION_SLOW_FADE_SATURATION_ALL_ID:
// //         switch(flashersettings.region){
// //           case EFFECTS_REGION_COLOUR_SELECT_ID: //set colours
// //             AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
// //             UpdateDesiredColourFromPaletteSelected();
// //             flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
// //           break;
// //           case EFFECTS_REGION_ANIMATE_ID: //shift along
// //             AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
// //             // Change brightness from 0 to 100% (rotate)
// //             uint8_t direction = flashersettings.function_seq.rate_index%2;//(flashersettings.function_slow_fade.direction^=1);
// //             float random_saturation = direction ? 1 : ((float)random(0,100)/(float)100);

// //             float adjusted_brightness = random_saturation;// mSupport::mapfloat(random_saturation,
// //             if(random_saturation<0.6){
// //               adjusted_brightness = mSupport::mapfloat(random_saturation,
// //                                                                     0,  0.6,
// //                                                                     0.4,0.6);
// //             }
                        
// //             for(ledout.index=0;ledout.index<pCONT_iLight->settings.light_size_count;ledout.index++){ 
// //               desired_colour[ledout.index].S = random_saturation; 
// //               desired_colour[ledout.index].B = adjusted_brightness;//random_saturation<0.5?pCONT_iLight->animation.brightness*0.5:pCONT_iLight->animation.brightness; //test, pair "whiter" less bright (maybe /2)  
// //             }            
// //             // Change pCONT_iLight->animation speed
// //             //if(mTime::TimeReached(&flashersettings.function_seq.tSavedNewSpeedUpdate,(random(3,10)*100))){
// //               uint16_t values[8] = {7,7,9,9,7,7,4,4}; //off,on,off
// //               pCONT_iLight->animation.transition.rate_ms = values[flashersettings.function_seq.rate_index++]*1000;
// //               if(flashersettings.function_seq.rate_index>=8) flashersettings.function_seq.rate_index=0;
// //               // holds colour more
// //               //pCONT_iLight->animation.transition.time_ms = direction ? (pCONT_iLight->animation.transition.rate_ms/4) : (pCONT_iLight->animation.transition.rate_ms); // brightness shift takes 100% of the time (no colour shift)
// //               pCONT_iLight->animation.transition.time_ms = random(0,1) ? (pCONT_iLight->animation.transition.rate_ms/4) : (pCONT_iLight->animation.transition.rate_ms.va;); // brightness shift takes 100% of the time (no colour shift)
// //             //}
// //           break;
// //         }
// //       break;
// //       //flasher_start_white_to_Colour: 
// //         // method that will start all leds on one colour (white?) and slowly add colours in with fade in
// //       //flasher_change_saturation_between 0 and 100, "fade of saturation", "fade of brightness"

// }
#endif //USE_MODULE_LIGHTS_ANIMATOR




#endif// ENABLE_PIXEL_FUNCTION_EFFECTS

#endif // flasherfunction_h