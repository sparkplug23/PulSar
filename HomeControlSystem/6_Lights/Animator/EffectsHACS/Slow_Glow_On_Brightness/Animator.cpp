#include "../../mAnimatorLight.h"


#ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT


//to be merged with slow glow maybe? or advanced slow glow with colour and/or brightness are glows, equally, saturation can be manipulated.
// A struct with what aspects of the "glows" should be could be idenitified here (brt, sat, colours, palettes)


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

#endif