#include "../../mAnimatorLight.h"


#ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT



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

        // test_flag^=1;

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
            this->AnimationProcess_Pulse_Random_On_2(param);
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
void mAnimatorLight::AnimationProcess_Pulse_Random_On_2(const AnimationParam& param)
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


#endif