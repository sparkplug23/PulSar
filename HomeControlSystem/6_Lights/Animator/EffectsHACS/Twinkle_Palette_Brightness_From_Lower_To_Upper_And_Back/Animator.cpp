#include "../../mAnimatorLight.h"


#ifdef ENABLE_PIXEL_FUNCTION_EFFECTS


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
      this->setAnimFunctionCallback([this](const AnimationParam& param){ this->AnimationProcess_Twinkle_Palette_Brightness_From_Lower_To_Upper_And_Back(param); });
      break;

      
      // Stay on this until restarted
  }

}


void mAnimatorLight::AnimationProcess_Twinkle_Palette_Brightness_From_Lower_To_Upper_And_Back(const AnimationParam& param)
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

#endif