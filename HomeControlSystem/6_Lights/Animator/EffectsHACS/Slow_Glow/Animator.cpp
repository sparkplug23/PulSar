#include "../../mAnimatorLight.h"


#ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT

// IMPORTANT: STORAGE of animation_colours may only be needed for certain animation types (ie this one) and otherwise can be produced on each loop like WLED
// in that case, I can reduce hardcoded ram usage (or make it dynamic) for only when needed. Otherwise retrieve from animator output


// /**************************************************************************************************************************************************************
//  * @brief  Solid_Colour_Based_On_Sun_Elevation_02
//  * @note   From -10 to noon, CCT will range from yellow to daywhite
//  * @note   From -5 to dusk, blue will go from 0 to max_brightness 
//  * 
//  * @note   Gloabl brightness will be manual, or controlled indirectly eg via mqtt
//  * 
//  * Instead of direct Elevation to palette, this animation will perform different versions of the mapping. Should this be one animation with subtypes? or different types
//  * 
//  * @note   Using RgbcctColour palette that is designed for each point in elevation
//  * *************************************************************************************************************************************************************/


/****
 * Changes pixels randomly to new colour, with slow blending
 * Requires new colour calculation each call
 */
void mAnimatorLight::SubTask_Flasher_Animate_Function__Slow_Glow(){

  // this should probably force order as random, then introduce static "inorder" option

  // this should probably force order as random, then introduce static "inorder" option
  pCONT_iLight->animation.transition.order_id = TRANSITION_ORDER_RANDOM_ID;
  
  #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
  if(pCONT_iLight->animation.flags.animator_first_run)
  {
    setCallback_ConstructJSONBody_Debug_Animations_Progress([this](void){
      this->ConstructJSONBody_Animation_Progress__Slow_Glow();
    });
  }
  #endif

  // So colour region does not need to change each loop to prevent colour crushing
  pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
  // Pick new colours
  UpdateDesiredColourFromPaletteSelected();
  
  #ifdef ENABLE_PIXEL_FUNCTION_PIXELGROUPING
  // Check if output multiplying has been set, if so, change desiredcolour array
  OverwriteUpdateDesiredColourIfMultiplierIsEnabled();
  #endif // ENABLE_PIXEL_FUNCTION_PIXELGROUPING
  
  // Get starting positions already on show
  UpdateStartingColourWithGetPixel();
  // Call the animator to blend from previous to new
  setAnimFunctionCallback([this](const AnimationParam& param){
    this->AnimationProcess_Generic_AnimationColour_LinearBlend(param);
  });

}


void mAnimatorLight::ConstructJSONBody_Animation_Progress__Slow_Glow()
{   

  // JBI->Add("CalcTotalMilliAmpere", stripbus->CalcTotalMilliAmpere());
  JBI->Add("PixelCount", stripbus->PixelCount());
  
}


#endif