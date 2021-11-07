#include "../../mAnimatorLight.h"


#ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT


/**
 * 1D - meaning there is no significant geography to it, similar to slow glow.
 * 2D - means an effect will be layered onto a matrix, where flames will be shown to go up the row-axis
 * 3D - eg cubes, effects will be drawn in 3 dimensions
 * 
 * */


/****
 * Changes pixels randomly to new colour, with slow blending
 * Requires new colour calculation each call
 */
void mAnimatorLight::SubTask_Flasher_Animate_Function__FirePlace_1D_01()
{
  // So colour region does not need to change each loop to prevent colour crushing
  pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
  // Pick new colours
  //Display on all pixels
  UpdateDesiredColourFromPaletteSelected();

  // this should probably force order as random, then introduce static "inorder" option
  pCONT_iLight->animation.transition.order_id = TRANSITION_ORDER_RANDOM_ID;
  

#ifdef ENABLE_DEVFEATURE_FIREPLACE_SEGMENT_EXTRA_GENERATE
  HsbColor colour_in = HsbColor(RgbColor(0));
  
 #ifndef USE_WS28XX_FEATURE_4_PIXEL_TYPE
  //Overwrite random brightness on special range
  for(uint16_t index=256;index<300;index++){

    colour_in = animation_colours[index].DesiredColour;

    if(colour_in.B==0){ //if colour was off, I need to set the colour to a defined value or it willl turn up brightness to show white
      colour_in.H = 0.0f;
      colour_in.S = 1.0f;
    }
    colour_in.B = pCONT_iLight->BrtN2F(random(0,10)*10);

    // colour_in.H = pCONT_iLight->BrtN2F(random(0,100));
  
  // This will be the introduction of segments into my code!!
    animation_colours[random(256,299)].DesiredColour = colour_in;

    // animation_colours[random(40,49)].DesiredColour = colour_in;

  }
  
#endif 
#endif // ENABLE_DEVFEATURE_FIREPLACE_SEGMENT_EXTRA_GENERATE

AddLog(LOG_LEVEL_TEST,PSTR("SubTask_Flasher_Animate_Function__FirePlace_1D_01 %d"),animation_colours[0].DesiredColour.R);

  // Check if output multiplying has been set, if so, change desiredcolour array
  // OverwriteUpdateDesiredColourIfMultiplierIsEnabled();
  // Get starting positions already on show
  UpdateStartingColourWithGetPixel();
  // Call the animator to blend from previous to new
  this->setAnimFunctionCallback(
    [this](const AnimationParam& param){
      this->AnimationProcess_Generic_AnimationColour_LinearBlend(param);
    }
  );
}

#endif