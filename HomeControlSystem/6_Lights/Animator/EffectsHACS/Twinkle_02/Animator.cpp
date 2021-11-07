#include "../../mAnimatorLight.h"


#ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT



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
  //     this->setAnimFunctionCallback([this](const AnimationParam& param){ this->AnimationProcess_TwinkleUsingPaletteColourRandom(param); });
  //     break;
  }

}



void mAnimatorLight::AnimationProcess_TwinkleUsingPaletteColourRandom(const AnimationParam& param)
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


#endif