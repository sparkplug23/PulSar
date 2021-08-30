#include "../../mAnimatorLight.h"

#ifdef ENABLE_PIXEL_FUNCTION_EFFECTS


// Make this an "Effects" which is public/friendly of animator, but not the same class
void mAnimatorLight::SubTask_Flasher_Animate_Function_SunPositions_SunRise_Alarm_01(){

  
      AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SubTask_Flasher_Animate_Function_SunPositions_SunRise_Alarm_01"));

  //using group 01
  // struct{}
  //use struct and memcpy to get data, then store results

  //effects_data_buffer
  //each needs 64 byte
// typedef struct particle {
//   // CRGB     color;
//   uint32_t birth  =0;
//   uint32_t last   =0;
//   float    vel    =0;
//   uint16_t pos    =-1;
//   float    fragment[12];
// } star;

/**
 * How the effects_data_buffer is formatted during effect runtime
 * */
struct EFFECT_CONFIG {
  uint8_t starting_palette_id = 0;
  uint8_t ending_palette_id = 10;
  uint8_t step_changes_remaining = 10;
  uint8_t colour_select_progress_counter = 0;
  uint8_t active_palette_id;

} effect_config;


  // star* stars = reinterpret_cast<star*>(pCONT_iLight->effects_data_buffer);
  EFFECT_CONFIG* effect_config2 = reinterpret_cast<EFFECT_CONFIG*>(pCONT_iLight->effects_data_buffer);
  //should be memcpy be used to insure size if not exceeded? or sizeof check? pointer is faster




  // switch(flashersettings.region){
  //   case EFFECTS_REGION_COLOUR_SELECT_ID:{ //set colours


      // if(effect_config2->active_palette_id++>mPaletteI->PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_ID){
      //   effect_config2->active_palette_id = mPaletteI->PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_01_ID;
      // }
      if(pCONT_iLight->animation.palette.id++>=mPaletteI->PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_ID){
        pCONT_iLight->animation.palette.id = mPaletteI->PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_01_ID;
      }


AddLog(LOG_LEVEL_TEST,PSTR("pCONT_iLight->animation.palette.id=%d"),pCONT_iLight->animation.palette.id);


      pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
      UpdateDesiredColourFromPaletteSelected();



      UpdateStartingColourWithGetPixel();



    //   flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
    // // NO 
    // }break;
    // case EFFECTS_REGION_ANIMATE_ID: //shift along
      AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
      this->setAnimFunctionCallback([this](const AnimationParam& param){ this->AnimationProcess_SunPositions_Gradient_Alarm_01(param); });
      
  //     flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID;
  //     break;
  // }

}

void mAnimatorLight::AnimationProcess_SunPositions_Gradient_Alarm_01(const AnimationParam& param)
{   
  // Need to make rgbcct pixels possible in the future, with dynamic animation_state

   for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
        animation_colours[pixel].StartingColor,
        animation_colours[pixel].DesiredColour,
        param.progress);    
    SetPixelColor(pixel, updatedColor);
  } // END for

}


#endif
