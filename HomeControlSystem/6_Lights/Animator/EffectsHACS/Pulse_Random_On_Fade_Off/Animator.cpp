#include "../../mAnimatorLight.h"

#ifdef ENABLE_PIXEL_FUNCTION_EFFECTS




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
            this->AnimationProcess_Pulse_Random_On_Fade_Off(param);
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
void mAnimatorLight::AnimationProcess_Pulse_Random_On_Fade_Off(const AnimationParam& param)
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




#endif
