#include "mRGBAnimator.h"

#ifdef USE_MODULE_LIGHTS_ADDRESSABLE


#ifdef ENABLE_PIXEL_FUNCTION_FLASHER



void mRGBAnimator::SubTask_Flasher_Animate_Function_Slow_Glow_On_Brightness(){

  pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = false;

  flashersettings.flag_finish_flasher_pair = false;
  flashersettings.flags.enable_random_rate = true;

  // Best "inorder" to compute colour again then brightness
  
  do{ //must complete the pair together //move inside functions
    switch(flashersettings.region){
      case FLASHER_REGION_COLOUR_SELECT_ID: //set colours
        #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
        AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_COLOUR_SELECT"));
        #endif
        UpdateDesiredColourFromPaletteSelected();
        flashersettings.region = FLASHER_REGION_ANIMATE_ID;
      break;
      case FLASHER_REGION_ANIMATE_ID: //shift along
        #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
        AddLog_P(LOG_LEVEL_DEBUG_MORE_LOWLEVEL,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_ANIMATE"));
        #endif
        // RotateDesiredColour(1,flashersettings.flags.movement_direction);

        // Calculate new pattern to change
        RefreshLEDIndexPattern();

        // Global brightness is already applied, and will be known as "max range"
        // Min range will be another map change here
        uint8_t max_brightness = pCONT_iLight->getBriRGB();
        uint8_t min_brightness = flashersettings.brightness_min;
        uint8_t random_brightness = 0;

        if(min_brightness > max_brightness){
          min_brightness = max_brightness;
        }

        // Change only selected pixel brightness levels
        for(ledout.index=0;ledout.index<strip_size_requiring_update;ledout.index++){
          random_brightness = random(min_brightness,max_brightness);          
          animation_colours[ledout.pattern[ledout.index]].DesiredColour = ApplyBrightnesstoDesiredColour(
                                                                              animation_colours[ledout.pattern[ledout.index]].DesiredColour,
                                                                              random_brightness);
        }

        if(flashersettings.flags.force_finish_flasher_pair_once){
          AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("flashersettings.flags.force_finish_flasher_pair_once WAS SET"));
          flashersettings.flags.force_finish_flasher_pair_once = false;
        }
      break;
    }
  }while(flashersettings.flag_finish_flasher_pair || flashersettings.flags.force_finish_flasher_pair_once);


}


// Fade solid colour from 0 to 75%, and a palette from 25 to 100% (50% overlap)
void mRGBAnimator::SubTask_Flasher_Animate_Function_Fade_Gradient(){
    
#ifdef ENABLE_FLASHER_ANIMATE_FUNCTION_FADE_GRADIENT

  flashersettings.flag_finish_flasher_pair = false;
  flashersettings.flags.enable_random_rate = true;
  
  // Apply green gradient, brightest at start

  uint16_t start = strip_size/2;
  uint16_t end = strip_size; 
  RgbTypeColor colour_gradient = HsbColor(
                                          pCONT_iLight->HueN2F(120),
                                          pCONT_iLight->SatN2F(100),
                                          pCONT_iLight->BrtN2F(map(pCONT_iLight->getBriRGB(),0,255,0,100))  
                                          );
  RgbTypeColor colour_random = RgbTypeColor(255,0,0); 
  HsbColor colour_random_adjusted = HsbColor(0);
  uint8_t gradient_end_percentage = 75;
  uint16_t strip_size_gradient = strip_size*(gradient_end_percentage/100.0f);
  uint16_t strip_size_single   = strip_size*(75/100.0f);
  
  start = 0;
  end = strip_size;
  for(ledout.index=start;ledout.index<end;ledout.index++){ 
    animation_colours[ledout.index].DesiredColour = RgbTypeColor(0);
  }
  
  //0 to 75% 
  start = 0;
  end = map(75,0,100,0,strip_size);
  for(ledout.index=start;ledout.index<end;ledout.index++){ 
    animation_colours[ledout.index].DesiredColour.R = pCONT_iLight->ledGamma(map(ledout.index,start,end,colour_gradient.R,0));
    animation_colours[ledout.index].DesiredColour.G = pCONT_iLight->ledGamma(map(ledout.index,start,end,colour_gradient.G,0));
    animation_colours[ledout.index].DesiredColour.B = pCONT_iLight->ledGamma(map(ledout.index,start,end,colour_gradient.B,0));
  }

  pCONT_iLight->SetPaletteListPtrFromID(pCONT_iLight->animation.palette_id);
  uint8_t pixels = pCONT_iLight->GetPixelsInMap(pCONT_iLight->palettelist.ptr);
  uint8_t desired_pixel;
  
  // 25 to 100%
  start = map(25,0,100,0,strip_size);
  end = strip_size;
  for(ledout.index=start;ledout.index<end;ledout.index++){ 
    desired_pixel = random(0,pixels-1);
    colour_random = pCONT_iLight->GetColourFromPalette(pCONT_iLight->palettelist.ptr,desired_pixel);
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
void mRGBAnimator::SubTask_Flasher_Animate_Function_Slow_Glow(){
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
void mRGBAnimator::AnimUpdateMemberFunction_BlendStartingToDesiredColour(const AnimationParam& param)
{    
// Serial.println("AnimUpdateMemberFunction_BlendStartingToDesiredColour");
  for (uint16_t pixel = 0; pixel < strip_size; pixel++){
    RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
        animation_colours[pixel].StartingColor,
        animation_colours[pixel].DesiredColour,
        param.progress);    
    SetPixelColor(pixel, updatedColor);
  } // END for

}




/**************************************************************************************************************************************************************
***************************************************************************************************************************************************************
********* Function_Slow_Glow *************************************************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************/

void mRGBAnimator::SubTask_Flasher_Animate_Function_Sequential(){

  pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
  
  flashersettings.flag_finish_flasher_pair = false;
  flashersettings.flags.enable_random_rate = true;
  
  do{ //must complete the pair together //move inside functions
    switch(flashersettings.region){
      case FLASHER_REGION_COLOUR_SELECT_ID: //set colours
        #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
        AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_COLOUR_SELECT"));
        #endif
        UpdateDesiredColourFromPaletteSelected();
        // Check if output multiplying has been set, if so, change desiredcolour array
        OverwriteUpdateDesiredColourIfMultiplierIsEnabled();
        // Get starting positions already on show
        UpdateStartingColourWithGetPixel();
        

    // Draw the tail that will be rotated through all the rest of the pixels
    // DrawTailPixels();


        flashersettings.region = FLASHER_REGION_ANIMATE_ID;
      break;
      case FLASHER_REGION_ANIMATE_ID: //shift along
        #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
        AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_ANIMATE"));
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
          AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("flashersettings.flags.force_finish_flasher_pair_once WAS SET"));
          flashersettings.flags.force_finish_flasher_pair_once = false;
        }
      break;
    }
  }while(flashersettings.flag_finish_flasher_pair || flashersettings.flags.force_finish_flasher_pair_once);

}


// simple blend function
void mRGBAnimator::AnimUpdateMemberFunction_Sequential(const AnimationParam& param)
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

        // AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL AnimUpdateMemberFunction_Sequential"));
  
  for (uint16_t pixel = 0; pixel < strip_size; pixel++){
    RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
        animation_colours[pixel].StartingColor,
        animation_colours[pixel].DesiredColour,
        param.progress);    
    SetPixelColor(pixel, updatedColor);
  } // END for

}

// void mRGBAnimator::DrawTailPixels()
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
void mRGBAnimator::SubTask_Flasher_Animate_Function_Slow_Glow_Partial_Palette_Step_Through(){
  // So colour region does not need to change each loop to prevent colour crushing
  pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
  // Pick new colours

  switch(flashersettings.region){
    case FLASHER_REGION_COLOUR_SELECT_ID:{ //set colours
      AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_COLOUR_SELECT"));

      pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
      // UpdateDesiredColourFromPaletteSelected();
      // UpdateStartingColourWithGetPixel();
      flashersettings.region = FLASHER_REGION_ANIMATE_ID;
    // NO 

        pCONT_iLight->SetPaletteListPtrFromID(pCONT_iLight->animation.palette_id);
        
      int16_t pixel_position = -2;
      uint8_t pixels_in_map = pCONT_iLight->GetPixelsInMap(pCONT_iLight->palettelist.ptr);
      
      
       AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "pixels_in_map= %d"),pixels_in_map);


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

      
      AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "shared_flasher_parameters = %d/%d/%d"),shared_flasher_parameters.indexes.active,index_1,index_2);
  
      
    

      for(uint16_t index=0;index<strip_size;index++){


          if(counter^=1){
            //using %2 means alternate leds replace the previous position, not the current
            desired_pixel = shared_flasher_parameters.indexes.counter ? index_2 : index_1;
          }else{
            desired_pixel = shared_flasher_parameters.indexes.counter ? index_1 : index_2;
          }
        
        colour = pCONT_iLight->GetColourFromPalette(pCONT_iLight->palettelist.ptr,desired_pixel,&pixel_position);

        if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
          colour = ApplyBrightnesstoDesiredColour(colour,pCONT_iLight->getBriRGB());
        }



        animation_colours[index].DesiredColour = colour;
         
        //AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "desired_pixel= %d/%d/%d"),pixels_map_lower_limit,desired_pixel,pixels_map_upper_limit);
  
        // if(++desired_pixel>pixels_map_upper_limit){
        //   desired_pixel = pixels_map_lower_limit;
        // }

      } 

      //progress active index by 1 or reset
      AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "shared_flasher_parameters.indexes.active=%d"),shared_flasher_parameters.indexes.active);
      if(++shared_flasher_parameters.indexes.active>pixels_in_map-1){
        shared_flasher_parameters.indexes.active=0;
      }
      
  
    }break;
    case FLASHER_REGION_ANIMATE_ID: //shift along
      AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_ANIMATE"));

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

      flashersettings.region = FLASHER_REGION_COLOUR_SELECT_ID;


      break;
  }



            // desired_colour[ledout.pattern[ledout.index]] = colour; 
            // AddLog_P(LOG_LEVEL_TEST, PSTR("colour=%d,%d,%d"),
            // colour.R,
            // colour.G,
            // colour.B);
            
            // animation_colours[ledout.pattern[ledout.index]].DesiredColour = ApplyBrightnesstoDesiredColour(colour,pCONT_iLight->getBriRGB());
              
          //   if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
          //     animation_colours[ledout.pattern[ledout.index]].DesiredColour = ApplyBrightnesstoDesiredColour(animation_colours[ledout.pattern[ledout.index]].DesiredColour,pCONT_iLight->getBriRGB());
          //   }

          //   //   AddLog_P(LOG_LEVEL_TEST, PSTR("colou2=%d,%d,%d"),
          //   // animation_colours[ledout.pattern[ledout.index]].DesiredColour.R,
          //   // animation_colours[ledout.pattern[ledout.index]].DesiredColour.G,
          //   // animation_colours[ledout.pattern[ledout.index]].DesiredColour.B);

            
          //   #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
          //   // AddLog_P(LOG_LEVEL_DEBUG_MORE_MORE,PSTR(D_LOG_NEO "colour[p%d:d%d] = %d,%d,%d %d pp%d"),
          //   //   ledout.pattern[ledout.index],desired_pixel,
          //   //   pCONT_iLight->HueF2N(desired_colour[ledout.pattern[ledout.index]].H),pCONT_iLight->SatF2N(desired_colour[ledout.pattern[ledout.index]].S),pCONT_iLight->BrtF2N(desired_colour[ledout.pattern[ledout.index]].B),
          //   //   pCONT_iLight->GetPixelsInMap(pCONT_iLight->palettelist.ptr),pixel_position
          //   // );
          //   #endif

          //   if(++desired_pixel>=pCONT_iLight->GetPixelsInMap(pCONT_iLight->palettelist.ptr)){desired_pixel=0;}
          // } //end for




  
}


// simple blend function
void mRGBAnimator::AnimUpdateMemberFunction_Slow_Glow_Partial_Palette_Step_Through(const AnimationParam& param)
{    

  for (uint16_t pixel = 0; pixel < strip_size; pixel++){
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

void mRGBAnimator::SubTask_Flasher_Animate_Function_Tester(){

  uint16_t test_strip_size = strip_size;
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
void mRGBAnimator::AnimUpdateMemberFunction_Tester(const AnimationParam& param)
{    

  for (uint16_t pixel = 0; pixel < strip_size; pixel++){
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
void mRGBAnimator::SubTask_Flasher_Animate_Function_Popping_Palette_Brightness_From_Lower_To_Upper_Boundary(){

   switch(flashersettings.region){
    case FLASHER_REGION_COLOUR_SELECT_ID: //set colours
      AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_COLOUR_SELECT"));
      pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
      // if(flashersettings.flags.enable_endingcolour_as_alternate){
      //   AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL flashersettings.flags.enable_endingcolour_as_alternate"));
      //   UpdateDesiredColourWithSingleColour(RgbcctColor(0));
      // }

      pCONT_iLight->setBriRGB_As_Percentage(flashersettings.brightness_min);


      UpdateDesiredColourFromPaletteSelected();
      // if(flashersettings.flags.enable_startcolour_as_alternate){
      //   AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL flashersettings.flags.enable_startcolour_as_alternate"));
      //   UpdateStartingColourWithSingleColour(RgbcctColor(0));
      // }else{
        UpdateStartingColourWithGetPixel();
      // }
      flashersettings.region = FLASHER_REGION_ANIMATE_ID;
    break;
    case FLASHER_REGION_ANIMATE_ID: //shift along

    
        UpdateStartingColourWithGetPixel();

        uint16_t index_random = random(0,strip_size);

        HsbColor hsb = GetPixelColor(index_random);

        uint8_t brightness_now = mapvalue(pCONT_iLight->BrtF2N(hsb.B),0,100,0,255);

        
      AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_ANIMATE %d %d"), brightness_now, flashersettings.brightness_max);

        if(brightness_now < flashersettings.brightness_max){ //+5 to margins so its not the same
          hsb.B = pCONT_iLight->BrtN2F(flashersettings.brightness_max);

  // animation_colours[index_random].DesiredColour = ApplyBrightnesstoDesiredColour(animation_colours[index_random].DesiredColour, flashersettings.brightness_max);
            

          animation_colours[index_random].DesiredColour = hsb;
        }

      AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_ANIMATE"));
      this->setAnimFunctionCallback([this](const AnimationParam& param){ this->AnimUpdateMemberFunction_BlendStartingToDesiredColour(param); });
      break;

      
      // Stay on this until restarted
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
void mRGBAnimator::SubTask_Flasher_Animate_Function_Twinkle_Palette_Brightness_From_Lower_To_Upper_And_Back(){

// Finish this mode later in december
// Split strip into parts
// Take every X pixels as the animated pixel, then I only need to remember a count 0:X
// Move across that each time (or make it random)
// Use this, to manipulate brightness from low to high... on the next animate_id call, return birghtness back (with require another counter for "progress of animation")
// Once that group has changed, or, even during it, pick another group to manipulate
// Effect should be "Group of pixels pulsing on, while the rest remain at a darker brightness"

  return;

   switch(flashersettings.region){
    case FLASHER_REGION_COLOUR_SELECT_ID: //set colours
      AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_COLOUR_SELECT"));
      pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
      // if(flashersettings.flags.enable_endingcolour_as_alternate){
      //   AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL flashersettings.flags.enable_endingcolour_as_alternate"));
      //   UpdateDesiredColourWithSingleColour(RgbcctColor(0));
      // }

      pCONT_iLight->setBriRGB_As_Percentage(flashersettings.brightness_min);


      UpdateDesiredColourFromPaletteSelected();
      // if(flashersettings.flags.enable_startcolour_as_alternate){
      //   AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL flashersettings.flags.enable_startcolour_as_alternate"));
      //   UpdateStartingColourWithSingleColour(RgbcctColor(0));
      // }else{
        UpdateStartingColourWithGetPixel();
      // }
      flashersettings.region = FLASHER_REGION_ANIMATE_ID;
    break;
    case FLASHER_REGION_ANIMATE_ID: //shift along




    
        // UpdateStartingColourWithGetPixel();

        // uint16_t index_random = random(0,strip_size);

        // HsbColor hsb = GetPixelColor(index_random);

        // if(hsb.B < pCONT_iLight->BrtN2F(flashersettings.brightness_max)){
        //   hsb.B = pCONT_iLight->BrtN2F(flashersettings.brightness_max);
        //   animation_colours[index_random].DesiredColour = hsb;
        // }

      AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_ANIMATE"));
      this->setAnimFunctionCallback([this](const AnimationParam& param){ this->AnimUpdateMemberFunction_Twinkle_Palette_Brightness_From_Lower_To_Upper_And_Back(param); });
      break;

      
      // Stay on this until restarted
  }

}


void mRGBAnimator::AnimUpdateMemberFunction_Twinkle_Palette_Brightness_From_Lower_To_Upper_And_Back(const AnimationParam& param)
{   

  // As integer so the if statement checks will not fail due to rounding errors
  // uint8_t progress_percentage = param.progress*100; 
  // uint8_t brightness_as_percentage = map(pCONT_iLight->getBriRGB(), 0,255, 0,100);
  // uint8_t random_amount = map(shared_flasher_parameters.alternate_random_amount_as_percentage, 0,100, 0,strip_size);

  //       uint16_t index_random = random(0,strip_size);

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
  //       //     random(0,strip_size), 
  //       //     HsbColor(pCONT_iLight->HueN2F(30),pCONT_iLight->SatN2F(90),pCONT_iLight->BrtN2F(random(0,brightness_as_percentage)))
  //       //   );
  //       // }
  //     break;
  //     case 0:
  //     case 100:
  //       for (uint16_t pixel = 0; pixel < strip_size; pixel++){
  //         SetPixelColor(pixel, animation_colours[pixel].DesiredColour);
  //       }
  //     break;
  //   }
  // }



   RgbTypeColor updatedColor;
  for (uint16_t pixel = 0; pixel < strip_size; pixel++){
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

void mRGBAnimator::SubTask_Flasher_Animate_Function_Pulse_Random_On(){

  // So colour region does not need to change each loop to prevent colour crushing
  pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
  // Pick new colours
  UpdateDesiredColourFromPaletteSelected();
  // randomly blank most of them out again
  uint16_t pixels_to_clear = strip_size-GetPixelsToUpdateAsNumberFromPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val);
  Serial.printf("pixels_to_clear=%d\n\r",pixels_to_clear);
  for(uint16_t index=0; index<pixels_to_clear; index++ ){
    animation_colours[random(0,strip_size)].DesiredColour = RgbColor(0);
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
void mRGBAnimator::AnimUpdateMemberFunction_Pulse_Random_On(const AnimationParam& param)
{    

  // float progress_percentage = param.progress*100; 
  float progress_half_range = param.progress < 0.5 ? 
                                  mSupport::mapfloat(param.progress, 0,0.5, 0,1) : 
                                  mSupport::mapfloat(param.progress, 0.5,1, 1,0) ;

  for (uint16_t pixel = 0; pixel < strip_size; pixel++){
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

void mRGBAnimator::SubTask_Flasher_Animate_Function_Pulse_Random_On_2(){

  pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
  
  flashersettings.flag_finish_flasher_pair = false;
  flashersettings.flags.enable_random_rate = true;
  
  do{ //must complete the pair together
    switch(flashersettings.region){
      case FLASHER_REGION_COLOUR_SELECT_ID:{ //set colours
        #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
        AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "Random_On_2 FLASHER_COLOUR_SELECT"));
        #endif

        // Check if output multiplying has been set, if so, change desiredcolour array
        // OverwriteUpdateDesiredColourIfMultiplierIsEnabled();

        test_flag^=1;

        // Go through strip, randomly change some to coloured or black
        pCONT_iLight->SetPaletteListPtrFromID(pCONT_iLight->animation.palette_id);
        uint8_t pixels = pCONT_iLight->GetPixelsInMap(pCONT_iLight->palettelist.ptr);
        RgbTypeColor colour_random = RgbTypeColor(0);

        uint16_t random_pixel_index = 0;
        
        for(uint16_t index=0; index<strip_size; index++ ){
          animation_colours[index].DesiredColour = GetPixelColor(index);
        }

        // randomly blank most of them out again
        uint8_t colour_or_black = 0;
        uint16_t pixels_to_update = GetPixelsToUpdateAsNumberFromPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val);
        Serial.printf("pixels_to_clear=%d\n\r",pixels_to_update);
        for(uint16_t index=0; index<pixels_to_update; index++ ){
          random_pixel_index = random(0,strip_size);
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
              colour_random = pCONT_iLight->GetColourFromPalette(pCONT_iLight->palettelist.ptr,desired_pixel);  
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

        // AddLog_P(LOG_LEVEL_TEST, PSTR("From G,B=%d,%d"),animation_colours[random_pixel_index].DesiredColour.G,animation_colours[random_pixel_index].DesiredColour.B);

        // Get starting positions already on show
        UpdateStartingColourWithGetPixel();
        // AddLog_P(LOG_LEVEL_TEST, PSTR("From G,B=%d,%d"),animation_colours[random_pixel_index].StartingColor.G,animation_colours[random_pixel_index].StartingColor.B);


        // dont automatically run animate again, add timeout for it
        flashersettings.region = FLASHER_REGION_ANIMATE_ID;

      }break;
      case FLASHER_REGION_ANIMATE_ID: //shift along
        #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
        AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "Random_On_2 FLASHER_ANIMATE"));
        #endif

        this->setAnimFunctionCallback(
          [this](const AnimationParam& param){
            this->AnimUpdateMemberFunction_Pulse_Random_On_2(param);
          }
        );

        if(flashersettings.flags.force_finish_flasher_pair_once){
          AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("flashersettings.flags.force_finish_flasher_pair_once WAS SET"));
          flashersettings.flags.force_finish_flasher_pair_once = false;
        }
        
        flashersettings.region = FLASHER_REGION_COLOUR_SELECT_ID;
        
      break;
    }
  }while(flashersettings.flag_finish_flasher_pair || flashersettings.flags.force_finish_flasher_pair_once);

}


// 50% progress is full brightness, 0 and 100% is off
void mRGBAnimator::AnimUpdateMemberFunction_Pulse_Random_On_2(const AnimationParam& param)
{    
 RgbTypeColor updatedColor;
  for (uint16_t pixel = 0; pixel < strip_size; pixel++){
    updatedColor = RgbTypeColor::LinearBlend(
        animation_colours[pixel].StartingColor,
        animation_colours[pixel].DesiredColour,
        param.progress);    
    SetPixelColor(pixel, updatedColor);
  } // END for
  
  // AddLog_P(LOG_LEVEL_TEST, PSTR("-->> G,B=%d,%d"),updatedColor.G, updatedColor.B);

}




/**************************************************************************************************************************************************************
***************************************************************************************************************************************************************
********* Twinkle_SingleColour_Random *************************************************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************/

void mRGBAnimator::SubTask_Flasher_Animate_Function_Twinkle_SingleColour_Random(){

  switch(flashersettings.region){
    case FLASHER_REGION_COLOUR_SELECT_ID:{ //set colours
      AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_COLOUR_SELECT"));
      pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
      UpdateDesiredColourFromPaletteSelected();
      UpdateStartingColourWithGetPixel();
      flashersettings.region = FLASHER_REGION_ANIMATE_ID;
    // NO 
    }break;
    case FLASHER_REGION_ANIMATE_ID: //shift along
      AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_ANIMATE"));
      this->setAnimFunctionCallback([this](const AnimationParam& param){ this->AnimUpdateMemberFunction_TwinkleSingleColourRandom(param); });
      break;
  }

}

void mRGBAnimator::AnimUpdateMemberFunction_TwinkleSingleColourRandom(const AnimationParam& param)
{   

  // As integer so the if statement checks will not fail due to rounding errors
  uint8_t progress_percentage = param.progress*100; 
  uint8_t brightness_as_percentage = map(pCONT_iLight->getBriRGB(), 0,255, 0,100);
  uint8_t random_amount = map(shared_flasher_parameters.alternate_random_amount_as_percentage, 0,100, 0,strip_size);

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
            random(0,strip_size), 
            HsbColor(pCONT_iLight->HueN2F(30),pCONT_iLight->SatN2F(90),pCONT_iLight->BrtN2F(random(0,brightness_as_percentage)))
          );
        }
      break;
      case 0:
      case 20:
      case 40:
      case 60:
      case 80: //go back to coloured
        for (uint16_t pixel = 0; pixel < strip_size; pixel++){
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


void mRGBAnimator::SubTask_Flasher_Animate_Function_Twinkle_PaletteColour_Random(){
  
  switch(flashersettings.region){
    case FLASHER_REGION_COLOUR_SELECT_ID: //set colours
      AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_COLOUR_SELECT"));
      pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
      // if(flashersettings.flags.enable_endingcolour_as_alternate){
      //   AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL flashersettings.flags.enable_endingcolour_as_alternate"));
      //   UpdateDesiredColourWithSingleColour(RgbcctColor(0));
      // }
      UpdateDesiredColourFromPaletteSelected();
      // if(flashersettings.flags.enable_startcolour_as_alternate){
      //   AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL flashersettings.flags.enable_startcolour_as_alternate"));
      //   UpdateStartingColourWithSingleColour(RgbcctColor(0));
      // }else{
        UpdateStartingColourWithGetPixel();
      // }
      flashersettings.region = FLASHER_REGION_ANIMATE_ID;
    break;
    case FLASHER_REGION_ANIMATE_ID: //shift along
      AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_ANIMATE"));
      this->setAnimFunctionCallback([this](const AnimationParam& param){ this->AnimUpdateMemberFunction_TwinkleUsingPaletteColourRandom(param); });
      break;
  }

}



void mRGBAnimator::AnimUpdateMemberFunction_TwinkleUsingPaletteColourRandom(const AnimationParam& param)
{   

  // As integer so the if statement checks will not fail due to rounding errors
  uint8_t progress_percentage = param.progress*100; 
  // uint8_t brightness_as_percentage = map(pCONT_iLight->getBriRGB(), 0,255, 0,100);
  uint8_t random_amount = map(shared_flasher_parameters.alternate_random_amount_as_percentage, 0,100, 0,strip_size);
  RgbTypeColor flash_colour = RgbTypeColor(0);
  uint8_t desired_pixel = 0;
  int16_t pixel_position = -1;
  uint8_t flashed_brightness = 0; // use set brightness or flash brighter as option
  
  // Update pointer of struct
  pCONT_iLight->SetPaletteListPtrFromID(pCONT_iLight->animation.palette_id);

  
  // for (uint16_t pixel = 0; pixel < strip_size; pixel++){
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
          desired_pixel = random(0,pCONT_iLight->GetPixelsInMap(pCONT_iLight->palettelist.ptr));
          // get colour from palette
          flash_colour = pCONT_iLight->GetColourFromPalette(pCONT_iLight->palettelist.ptr,desired_pixel,&pixel_position);
          flash_colour = ApplyBrightnesstoDesiredColour(flash_colour,flashed_brightness);
          SetPixelColor(
            random(0,strip_size), 
            flash_colour
          );
        }
      break;
      case 0:
      case 20:
      case 40:
      case 60:
      case 80: //go back to coloured
        for (uint16_t pixel = 0; pixel < strip_size; pixel++){
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








void mRGBAnimator::SubTask_Flasher_Animate_Function_Slow_Fade_Brightness_All(){
     
      
//       case FLASHER_FUNCTION_SLOW_FADE_BRIGHTNESS_ALL_ID:
//         switch(flashersettings.region){
//           case FLASHER_REGION_COLOUR_SELECT_ID: //set colours
//             AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_COLOUR_SELECT"));
//             UpdateDesiredColourFromPaletteSelected();
//             flashersettings.region = FLASHER_REGION_ANIMATE_ID;
//           break;
//           case FLASHER_REGION_ANIMATE_ID: //shift along
//             AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_ANIMATE"));
//             // Change brightness from 0 to 100% (rotate)
//             uint8_t direction = flashersettings.function_seq.rate_index%2;//(flashersettings.function_slow_fade.direction^=1);
//             for(ledout.index=0;ledout.index<strip_size;ledout.index++){ desired_colour[ledout.index].B = direction; }            
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






void mRGBAnimator::SubTask_Flasher_Animate_Function_Slow_Fade_Saturation_All(){
      
//       case FLASHER_FUNCTION_SLOW_FADE_SATURATION_ALL_ID:
//         switch(flashersettings.region){
//           case FLASHER_REGION_COLOUR_SELECT_ID: //set colours
//             AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_COLOUR_SELECT"));
//             UpdateDesiredColourFromPaletteSelected();
//             flashersettings.region = FLASHER_REGION_ANIMATE_ID;
//           break;
//           case FLASHER_REGION_ANIMATE_ID: //shift along
//             AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_ANIMATE"));
//             // Change brightness from 0 to 100% (rotate)
//             uint8_t direction = flashersettings.function_seq.rate_index%2;//(flashersettings.function_slow_fade.direction^=1);
//             float random_saturation = direction ? 1 : ((float)random(0,100)/(float)100);

//             float adjusted_brightness = random_saturation;// mSupport::mapfloat(random_saturation,
//             if(random_saturation<0.6){
//               adjusted_brightness = mSupport::mapfloat(random_saturation,
//                                                                     0,  0.6,
//                                                                     0.4,0.6);
//             }
                        
//             for(ledout.index=0;ledout.index<strip_size;ledout.index++){ 
//               desired_colour[ledout.index].S = random_saturation; 
//               desired_colour[ledout.index].B = adjusted_brightness;//random_saturation<0.5?pCONT_iLight->animation.brightness*0.5:pCONT_iLight->animation.brightness; //test, pair "whiter" less bright (maybe /2)  
//             }            
//             // Change pCONT_iLight->animation speed
//             //if(mTime::TimeReached(&flashersettings.function_seq.tSavedNewSpeedUpdate,(random(3,10)*100))){
//               uint16_t values[8] = {7,7,9,9,7,7,4,4}; //off,on,off
//               pCONT_iLight->animation.transition.rate_ms = values[flashersettings.function_seq.rate_index++]*1000;
//               if(flashersettings.function_seq.rate_index>=8) flashersettings.function_seq.rate_index=0;
//               // holds colour more
//               //pCONT_iLight->animation.transition.time_ms = direction ? (pCONT_iLight->animation.transition.rate_ms.val/4) : (pCONT_iLight->animation.transition.rate_ms.val); // brightness shift takes 100% of the time (no colour shift)
//               pCONT_iLight->animation.transition.time_ms = random(0,1) ? (pCONT_iLight->animation.transition.rate_ms.val/4) : (pCONT_iLight->animation.transition.rate_ms.va;); // brightness shift takes 100% of the time (no colour shift)
//             //}
//           break;
//         }
//       break;
//       //flasher_start_white_to_Colour: 
//         // method that will start all leds on one colour (white?) and slowly add colours in with fade in
//       //flasher_change_saturation_between 0 and 100, "fade of saturation", "fade of brightness"

}



#endif //USE_MODULE_LIGHTS_ADDRESSABLE




#endif// ENABLE_PIXEL_FUNCTION_FLASHER
