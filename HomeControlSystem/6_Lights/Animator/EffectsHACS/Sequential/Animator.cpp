#include "../../mAnimatorLight.h"


#ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT


void mAnimatorLight::SubTask_Flasher_Animate_Function__Sequential(){

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
            this->AnimationProcess_Sequential(param);
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
void mAnimatorLight::AnimationProcess_Sequential(const AnimationParam& param)
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

        // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL AnimationProcess_Sequential"));
  
  for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
        animation_colours[pixel].StartingColor,
        animation_colours[pixel].DesiredColour,
        param.progress);    
    SetPixelColor(pixel, updatedColor);
  } // END for

}

#endif

