#include "../../mAnimatorLight.h"

#ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT



/**************************************************************************************************************************************************************
***************************************************************************************************************************************************************
********* SubTask_Flasher_Animate_Function_Popping_Palette_Brightness_From_Lower_To_Upper_Boundary *************************************************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************/


/****
 * Changes pixels randomly to new colour, with slow blending
 * Requires new colour calculation each call
 */
void mAnimatorLight::SubTask_Flasher_Animate_Function_Popping_Palette_Brightness_From_Lower_To_Upper_Boundary(){

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

    
      UpdateStartingColourWithGetPixel();

      uint16_t index_random = random(0,pCONT_iLight->settings.light_size_count);

      //#ifndef USE_WS28XX_FEATURE_4_PIXEL_TYPE //tmp fix
      //HsbColor hsb = RgbColor(GetPixelColor(index_random));
      //#else
      HsbColor hsb = RgbColor(GetPixelColor(index_random));
      //#endif

      uint8_t brightness_now = mapvalue(pCONT_iLight->BrtF2N(hsb.B),0,100,0,255);

      AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE %d %d"), brightness_now, flashersettings.brightness_max);

      if(brightness_now < flashersettings.brightness_max){ //+5 to margins so its not the same
        hsb.B = pCONT_iLight->BrtN2F(flashersettings.brightness_max);
        // animation_colours[index_random].DesiredColour = ApplyBrightnesstoDesiredColour(animation_colours[index_random].DesiredColour, flashersettings.brightness_max);
        animation_colours[index_random].DesiredColour = hsb;
      }

      AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
      this->setAnimFunctionCallback([this](const AnimationParam& param){ this->AnimationProcess_Generic_AnimationColour_LinearBlend(param); });
    break;
  }

}

#endif