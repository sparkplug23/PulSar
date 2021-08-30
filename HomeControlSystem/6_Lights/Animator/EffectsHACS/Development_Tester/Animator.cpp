#include "../../mAnimatorLight.h"

#ifdef ENABLE_PIXEL_FUNCTION_EFFECTS



void mAnimatorLight::SubTask_Flasher_Animate_Function_Tester(){

  uint16_t test_strip_size = pCONT_iLight->settings.light_size_count;
  // test_strip_size = 50;

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
      this->AnimationProcess_Tester(param);
    }
  );

}


// simple blend function
void mAnimatorLight::AnimationProcess_Tester(const AnimationParam& param)
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

#endif