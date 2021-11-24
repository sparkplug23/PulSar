#include "../../mAnimatorLight.h"

#ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT



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
  #ifdef ENABLE_PIXEL_FUNCTION_PIXELGROUPING
  // Check if output multiplying has been set, if so, change desiredcolour array
  OverwriteUpdateDesiredColourIfMultiplierIsEnabled();
  #endif// ENABLE_PIXEL_FUNCTION_PIXELGROUPING
  // Get starting positions already on show
  // UpdateStartingColourWithGetPixel();
  // Call the animator to blend from previous to new
        
  this->setAnimFunctionCallback(
    [this](const AnimationParam& param){
      this->AnimationProcess_Pulse_Random_On(param);
    }
  );

}


// 50% progress is full brightness, 0 and 100% is off
void mAnimatorLight::AnimationProcess_Pulse_Random_On(const AnimationParam& param)
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

#endif