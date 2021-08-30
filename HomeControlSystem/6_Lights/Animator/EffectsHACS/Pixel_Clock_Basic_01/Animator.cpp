#include "../../mAnimatorLight.h"


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
      this->AnimationProcess_Generic_AnimationColour_LinearBlend(param);
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
      this->AnimationProcess_Generic_AnimationColour_LinearBlend(param);
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
      this->AnimationProcess_Generic_AnimationColour_LinearBlend(param);
    }
  );
}
#endif // ENABLE_DEVFEATURE_RGB_CLOCK
