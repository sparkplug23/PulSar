#include "../../mAnimatorLight.h"



#ifdef ENABLE_PIXEL_FUNCTION_EFFECTS


/****
 * Changes pixels randomly to new colour, with slow blending
 * Requires new colour calculation each call
 */
void mAnimatorLight::SubTask_Flasher_Animate_Function__Solid_Static_Single_Colour(){
  // AddLog(LOG_LEVEL_TEST, PSTR("SubTask_Flasher_Animate_Function__Solid_Static_Single_Colour"));
  // Set palette pointer
  mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
  // Set up colours
  // Brightness is generated internally, and rgbcct solid palettes are output values
  pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = false;
  animation_colours_rgbcct.DesiredColour  = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr);

  // AddLog(LOG_LEVEL_TEST, PSTR("DesiredColour1=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
  if(!pCONT_iLight->rgbcct_controller.getApplyBrightnessToOutput()){ // If not already applied, do it using global values
    animation_colours_rgbcct.DesiredColour = ApplyBrightnesstoRgbcctColour(
      animation_colours_rgbcct.DesiredColour, 
      pCONT_iLight->rgbcct_controller.getBrightnessRGB255(),
      pCONT_iLight->rgbcct_controller.getBrightnessCCT255()
    );
  }

  // AddLog(LOG_LEVEL_TEST, PSTR("DesiredColour2=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
  animation_colours_rgbcct.StartingColor = GetPixelColor();
  // GetPixelColourHardwareInterface();

  // AddLog(LOG_LEVEL_TEST, PSTR("StartingColour2=%d,%d,%d,%d,%d"), animation_colours_rgbcct.StartingColor.R,animation_colours_rgbcct.StartingColor.G,animation_colours_rgbcct.StartingColor.B,animation_colours_rgbcct.StartingColor.WC,animation_colours_rgbcct.StartingColor.WW);
    

  // Call the animator to blend from previous to new
  this->setAnimFunctionCallback(
    [this](const AnimationParam& param){
      this->AnimationProcess_BlendStartingToDesiredColour_SOLID_COLOUR(param);
    }
  );
}


// simple blend function
void mAnimatorLight::AnimationProcess_BlendStartingToDesiredColour_SOLID_COLOUR(const AnimationParam& param)
{   

  RgbcctColor output_colour = RgbcctColor::LinearBlend(
    animation_colours_rgbcct.StartingColor,
    animation_colours_rgbcct.DesiredColour,
    param.progress);

  for(int ii=0;ii<pCONT_iLight->settings.light_size_count;ii++){
    SetPixelColor(ii,output_colour);
  }

  // pCONT_iLight->SetPixelColourHardwareInterface(output_colour,0,true);
  
}



#endif