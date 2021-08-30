#include "../../mAnimatorLight.h"


#ifdef ENABLE_PIXEL_FUNCTION_EFFECTS


/**************************************************************************************************************************************************************
 * @brief  Solid_Colour_Based_On_Sun_Elevation_05
 * 
 * CCT_Mapped, day white to warm white around +-20, then >20 is max cct
 * 
 * *************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_05()
{
 
  // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_05"));

  pCONT_iLight->animation.palette.id = mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_01_ID;

  mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
  // Set up colours
  // Brightness is generated internally, and rgbcct solid palettes are output values


float sun_elevation = 0;
#ifdef USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
  #ifdef USE_DEVFEATURE_SUNPOSITION_ELEVATION_USE_TESTING_VALUE
  float sun_elevation = (float)pCONT_solar->solar_position_testing.elevation;
  #else
  float sun_elevation = (float)pCONT_solar->solar_position.elevation;
  #endif
#endif
  if(sun_elevation < -20)
  {
    pCONT_iLight->rgbcct_controller.setCCT(pCONT_iLight->get_CTRangeMax());      
  }else
  if(sun_elevation > 20)
  {
    pCONT_iLight->rgbcct_controller.setCCT(pCONT_iLight->get_CTRangeMin());      
  }else{
    // Convert elevation into percentage
    uint8_t elev_perc = map(sun_elevation,-20,20,0,100);
    // Convert percentage into cct
    uint16_t cct_val = mapvalue(elev_perc, 0,100, pCONT_iLight->get_CTRangeMax(),pCONT_iLight->get_CTRangeMin());
 
    // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "cct_val=%d"),cct_val);
    // Set the colour temp
    pCONT_iLight->rgbcct_controller.setCCT(cct_val);    
  }

  pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = false;
  animation_colours_rgbcct.DesiredColour  = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr);
  pCONT_iLight->animation.flags.fForceUpdate = true;

  // AddLog(LOG_LEVEL_TEST, PSTR("DesiredColour1=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
  if(!pCONT_iLight->rgbcct_controller.getApplyBrightnessToOutput())
  { // If not already applied, do it using global values
    animation_colours_rgbcct.DesiredColour = ApplyBrightnesstoRgbcctColour(
      animation_colours_rgbcct.DesiredColour, 
      pCONT_iLight->rgbcct_controller.getBrightnessRGB255(),
      pCONT_iLight->rgbcct_controller.getBrightnessCCT255()
    );
  }

  animation_colours_rgbcct.StartingColor = GetPixelColor();

  AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
  this->setAnimFunctionCallback([this](const AnimationParam& param){
      this->AnimationProcess_Generic_RGBCCT_Single_Colour_All(param); });
   
}
#endif