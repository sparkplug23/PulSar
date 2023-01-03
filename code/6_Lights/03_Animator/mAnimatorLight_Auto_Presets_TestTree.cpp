#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR


#ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS

#ifdef USE_DEVFEATURE_PRESETS_MANUALUSERCUSTOM_TESTTREE

/**
 * @brief Temporary solution for christmas, design of outside tree
 * Will be tested at 1/minute
 * 
 * 
 * Instead of limiting effects by time here, I should just make a second group to be called the other times of the day
 * 
 * Flashing effects 4-7pm
 * Subtle effects 8-10pm then off
 * 
 * 
 * 
 * @param id 
 */
void mAnimatorLight::LoadPreset_ManualUserCustom_ByID(uint8_t id)
{

  ALOG_INF(PSTR("LoadPreset_ManualUserCustom_ByID=%d"),id);


  /**
   * @brief
   * Effect:  Static Palette
   * Palette: Christmas Colourful for Outside Tree (Many, but 2021 colours)
   * 
   */
  if(
    (id == 0) &&
    (true) // Time Allowed, true for always
  ){
  
    CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__STATIC_PALETTE__ID);
    CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_24__ID);
    CommandSet_Animation_Transition_Time_Ms(0);
    CommandSet_Animation_Transition_Rate_Ms(1000);   
    SEGMENT_I(0).set_intensity(30);
    SEGMENT_I(0).set_speed(255);
    SEGMENT_I(0).grouping = 1;
    CommandSet_Global_BrtRGB_255(255);
  
  }else
  /**
   * @brief
   * Effect: Static Palette
   * Palette: Christmas Colourful for Outside Tree (New, Extreme Colours, With Warm Tone)
   * 
   */
  if(
    (id == 1) &&
    (true) // Time Allowed, true for always
  ){
    
    CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__WLED_SHIMMERING_PALETTE__ID);
    CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_24__ID);
    CommandSet_Animation_Transition_Time_Ms(0);
    CommandSet_Animation_Transition_Rate_Ms(23);    
    SEGMENT_I(0).set_intensity(255);
    SEGMENT_I(0).set_speed(200);
    SEGMENT_I(0).grouping = 1;
    CommandSet_Global_BrtRGB_255(255);

  }else
  /**
   * @brief
   * Effect: Random Palette
   * Palette: Outside 1 (Many)
   * 
   */
  if(
    (id == 2) &&
    (true) // Time Allowed, true for always
  ){   

    CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__SLOW_GLOW__ID);
    CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_24__ID);
    CommandSet_Animation_Transition_Time_Ms(500);
    CommandSet_Animation_Transition_Rate_Ms(1000); 
    SEGMENT_I(0).set_intensity(30);
    SEGMENT_I(0).set_speed(255);
    SEGMENT_I(0).grouping = 1;
    CommandSet_Global_BrtRGB_255(255);

  }else
  /**
   * @brief
   * Effect: Shimmering Palette (Fast enough to be noticed!)
   * Palette: Christmas Colourful for Outside Tree (Many, but 2021 colours)
   * 
   * Fast enough and intense enough to be noticed
   * 
   */
  
  if(
    (id == 3) &&
    (true) // Time Allowed, true for always
  ){

    CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__SLOW_GLOW__ID);
    CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_24__ID);
    CommandSet_Animation_Transition_Time_Ms(0);
    CommandSet_Animation_Transition_Rate_Ms(50); 
    SEGMENT_I(0).set_intensity(20);
    SEGMENT_I(0).set_speed(255);
    SEGMENT_I(0).grouping = 1;
    CommandSet_Global_BrtRGB_255(255);

  }else
  /**
   * @brief
   * Effect: White Twinkle (Fast to be noticed!) = new effect, which can allow another palette to twinkle over another (ie palette 10 to white only, vs others with many.... eg white tree with coloured flashing)
   * Palette: Christmas Colourful for Outside Tree (Many, but 2021 colours)
   * It basically randomly takes a colour from palette_sec and twinkles it over palette_pri
   * 
   */
  if(
    (id == 4) &&
    (true) // Time Allowed, true for always
  ){

    CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__TWINKLE_PALETTE_SEC_ON_ORDERED_PALETTE_PRI__ID);
    CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_24__ID);
    CommandSet_Animation_Transition_Time_Ms(0);
    CommandSet_Animation_Transition_Rate_Ms(100); 
    SEGMENT_I(0).set_intensity(3);
    SEGMENT_I(0).set_speed(127);
    SEGMENT_I(0).grouping = 1;
    CommandSet_Global_BrtRGB_255(255);

  }else
  /**
   * @brief
   * Effect: White Twinkle (Fast to be noticed!) = new effect, which can allow another palette to twinkle over another (ie palette 10 to white only, vs others with many.... eg white tree with coloured flashing)
   * Palette: Christmas Colourful for Outside Tree (Many, but 2021 colours)
   * It basically randomly takes a colour from palette_sec and twinkles it over palette_pri
   * 
   */
  if(
    (id == 5) &&
    (true) // Time Allowed, true for always
  ){

    CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__WLED_FIREWORKS_STARBURST__ID);
    CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_24__ID);
    CommandSet_Animation_Transition_Time_Ms(0);
    CommandSet_Animation_Transition_Rate_Ms(50); 
    SEGMENT_I(0).set_intensity(255);
    SEGMENT_I(0).set_speed(127);
    SEGMENT_I(0).grouping = 3;
    CommandSet_Global_BrtRGB_255(255);

    SEGMENT_I(0).set_colors(1, 50,50,50, 0);
    // [0] = ((uint32_t)0 << 24) | ((uint32_t)50 << 16) | ((uint32_t)50 << 8) | (uint32_t)50);

  }else
  /**
   * @brief
   * 
   */
  if(
    (id == 6) &&
    (true) // Time Allowed, true for always
  ){

    CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__STEPPING_PALETTE__ID);
    CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_24__ID);
    CommandSet_Animation_Transition_Time_Ms(1500);
    CommandSet_Animation_Transition_Rate_Ms(3000); 
    SEGMENT_I(0).set_intensity(20);
    SEGMENT_I(0).set_speed(255);
    SEGMENT_I(0).grouping = 1;
    CommandSet_Global_BrtRGB_255(255);

  }else
  /**
   * @brief
   * 
   */
  if(
    (id == 7) &&
    (true) // Time Allowed, true for always
  ){

    CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__STEPPING_PALETTE__ID);
    CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_24__ID);
    CommandSet_Animation_Transition_Time_Ms(1500);
    CommandSet_Animation_Transition_Rate_Ms(3000); 
    SEGMENT_I(0).set_intensity(20);
    SEGMENT_I(0).set_speed(255);
    SEGMENT_I(0).grouping = 100;
    CommandSet_Global_BrtRGB_255(255);

  }else


// I want rows of all "24 colours", then rotate them up the tree

  if(
    (id == 8) &&
    (true) // Time Allowed, true for always
  ){

    CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__STATIC_PALETTE__ID);
    CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_25__ID);   // no purple
    CommandSet_Animation_Transition_Time_Ms(10000);
    CommandSet_Animation_Transition_Rate_Ms(20000); 
    SEGMENT_I(0).set_intensity(20);
    SEGMENT_I(0).set_speed(255);
    SEGMENT_I(0).grouping = 100;
    CommandSet_Global_BrtRGB_255(255);

  }else



  /**
   * @brief
   * 
   */
  if(
    (id == 9) &&
    (true) // Time Allowed, true for always
  ){

    CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__ROTATING_PREVIOUS_ANIMATION__ID);
    CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_25__ID); //no purple
    CommandSet_Animation_Transition_Time_Ms(50);
    CommandSet_Animation_Transition_Rate_Ms(100); 
    SEGMENT_I(0).set_intensity(20);
    SEGMENT_I(0).set_speed(255);
    SEGMENT_I(0).grouping = 1;
    CommandSet_Global_BrtRGB_255(255);


  }else



  /**
   * @brief
   * 
   */
  if(
    (id == 10) &&
    (true) // Time Allowed, true for always
  ){

    CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__WLED_COLOR_WIPE_RANDOM__ID);
    CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_24__ID);
    CommandSet_Animation_Transition_Time_Ms(0);
    CommandSet_Animation_Transition_Rate_Ms(23); 
    SEGMENT_I(0).set_intensity(255); //smoothness // https://github.com/Aircoookie/WLED/wiki/List-of-effects-and-palettes
    SEGMENT_I(0).set_speed(127);
    SEGMENT_I(0).grouping = 1;
    CommandSet_Global_BrtRGB_255(255);

  }else
  /**
   * @brief
   * 
   */
  if(
    (id == 11) &&
    (true) // Time Allowed, true for always
  ){

    CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__WLED_FIREWORKS_EXPLODING__ID);
    CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_24__ID);
    CommandSet_Animation_Transition_Time_Ms(0);
    CommandSet_Animation_Transition_Rate_Ms(23); 
    SEGMENT_I(0).set_intensity(255); //smoothness // https://github.com/Aircoookie/WLED/wiki/List-of-effects-and-palettes
    SEGMENT_I(0).set_speed(126);
    SEGMENT_I(0).grouping = 1;
    CommandSet_Global_BrtRGB_255(255);

  }else
  /**
   * @brief
   * 
   */
  if(
    (id == 12) &&
    (true) // Time Allowed, true for always
  ){

    CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__WLED_FIREWORKS_EXPLODING_NO_LAUNCH__ID);
    CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_24__ID);
    CommandSet_Animation_Transition_Time_Ms(0);
    CommandSet_Animation_Transition_Rate_Ms(23); 
    SEGMENT_I(0).set_intensity(255); //smoothness // https://github.com/Aircoookie/WLED/wiki/List-of-effects-and-palettes
    SEGMENT_I(0).set_speed(126);
    SEGMENT_I(0).grouping = 1;
    CommandSet_Global_BrtRGB_255(255);

  }else

  /////////// BELOW ARE TEST but not considered part of normal operation //////////////////////////////////////////////////////////////////////////////////

  /**
   * @brief
   * Test to view with white
   */
  if(
    (id == 13) &&
    (true) // Time Allowed, true for always
  ){

    CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__STATIC_PALETTE__ID);
    CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_27__ID);   // no purple
    CommandSet_Animation_Transition_Time_Ms(10000);
    CommandSet_Animation_Transition_Rate_Ms(20000); 
    SEGMENT_I(0).set_intensity(20);
    SEGMENT_I(0).set_speed(255);
    SEGMENT_I(0).grouping = 100;
    CommandSet_Global_BrtRGB_255(255);


  }

} // END Function


  // /**
  //  * @brief
  //  * Effect: Dim White (brightness corrected) with colours as bars chasing it
  //  *          ie fully white set, with random pixels chosen as center for pixel "worms"
  //  *          worms will need to store as stuct
  //  *                          pixel center location
  //  *                          pixel length to grow to
  //  *                          progress of worm length in effect
  //  *          Worms will start at random pixels, with 100% saturation, then slowly get longer with the tails fading to white
  //  * Palette: Christmas Colourful for Outside Tree (Many, but 2021 colours)
  //  * 
  //  * 
  //  * 
  //  */
  // if(
  //   (id == 5) &&
  //   (true) // Time Allowed, true for always
  // ){


  // }else
  // /**
  //  * @brief
  //  * Effect: Stepping Palette (Double length only)
  //  * 
  //  * Palette: Christmas Colourful for Outside Tree (Many, but 2021 colours)
  //  * 
  //  */
  // if(
  //   (id == 6) &&
  //   (true) // Time Allowed, true for always
  // ){

  //   CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__STEPPING_PALETTE__ID);
  //   CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_13__ID);
  //   CommandSet_Animation_Transition_Time_Ms(0);
  //   CommandSet_Animation_Transition_Rate_Ms(1000);
  //   SEGMENT_I(0).grouping = 1;

  // }else
  // /**
  //  * @brief
  //  * Effect: Stepping Palette (Make a new, to replace original, that allows amount of pixels in palette to be stepped through)
  //  *              12121212
  //  *              32323232
  //  *              31313131
  //  * 
  //  *              need to think how a 3rd, 4th etc will work
  //  * 
  //  * Palette: Christmas Colourful for Outside Tree (Many, but 2021 colours)
  //  * For 2022, lets step through as 3 colours only
  //  * 
  //  * Always replace first colour added with new one
  //  * Intensity, will say how many of palette should be shown, with minimal always 2 (ie 0 of 255, means still 2)
  //  * 
  //  */  
  // if(
  //   (id == 7) &&
  //   (true) // Time Allowed, true for always
  // ){


  // }else
  // /**
  //  * @brief
  //  * Effect: Static Palette with large grouping
  //  * Palette: Christmas Colourful for Outside Tree (New, Extreme Colours, With Warm Tone)
  //  * 
  //  */
  // if(
  //   (id == 8) &&
  //   (true) // Time Allowed, true for always
  // ){ 

  //   CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__STATIC_PALETTE__ID);
  //   CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_06__ID);
  //   CommandSet_Animation_Transition_Time_Ms(500);
  //   CommandSet_Animation_Transition_Rate_Ms(1000);
  //   SEGMENT_I(0).grouping = 4; // to be like 1ft of tree (maybe 200?)

  // }else
  // /**
  //  * @brief
  //  * Effect: Slow Glow with large grouping
  //  * Palette: Christmas Colourful for Outside Tree (New, Extreme Colours, With Warm Tone)
  //  * 
  //  */
  // if(
  //   (id == 9) &&
  //   (true) // Time Allowed, true for always
  // ){

  //   CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__SLOW_GLOW__ID);
  //   CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_06__ID);
  //   CommandSet_Animation_Transition_Time_Ms(500);
  //   CommandSet_Animation_Transition_Rate_Ms(1000);
  //   SEGMENT_I(0).grouping = 4; // to be like 1ft of tree (maybe 200?) 

  // }else
  // /**
  //  * @brief
  //  * Effect: Rotating static palette with large grouping. Perhaps "rotating" needs two versions, one to be "Rotating Palette" and "Rotating Current"
  //  * Palette: Christmas Colourful for Outside Tree (New, Extreme Colours, With Warm Tone)
  //  * 
  //  */
  // if(
  //   (id == 10) &&
  //   (true) // Time Allowed, true for always
  // ){

  //   CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__SLOW_GLOW__ID);
  //   CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_06__ID);
  //   CommandSet_Animation_Transition_Time_Ms(500);
  //   CommandSet_Animation_Transition_Rate_Ms(1000);
  //   SEGMENT_I(0).grouping = 4; // to be like 1ft of tree (maybe 200?) 

  // }else

  // // other wled?
  // /**
  //  * @brief
  //  * Effect: Firework 2D 
  //  * Palette: Christmas Colourful for Outside Tree (New, Extreme Colours, With Warm Tone)
  //  * 
  //  */
  // if(
  //   (id == 11) &&
  //   (true) // Time Allowed, true for always
  // ){
  
  //   CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__WLED_FIREWORKS_EXPLODING__ID);
  //   CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_06__ID);
  //   CommandSet_Animation_Transition_Time_Ms(0);
  //   CommandSet_Animation_Transition_Rate_Ms(23);
  //   SEGMENT_I(0).grouping = 1;

  // }else
  // /**
  //  * @brief
  //  * Effect: EFFECTS_FUNCTION__WLED_COLOR_SWEEP_RANDOM__ID
  //  * Palette: Christmas Colourful for Outside Tree (New, Extreme Colours, With Warm Tone)
  //  * 
  //  * I want the one that sweeps from top to bottom only, not back again (always top down for a "waterfall/choc fountain" effect)
  //  * 
  //  */
  // if(
  //   (id == 12) &&
  //   (true) // Time Allowed, true for always
  // ){

  //   CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__WLED_COLOR_SWEEP_RANDOM__ID);
  //   CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_06__ID);
  //   CommandSet_Animation_Transition_Time_Ms(0);
  //   CommandSet_Animation_Transition_Rate_Ms(23);
  //   SEGMENT_I(0).grouping = 1;


  // }



// }


#endif // USE_DEVFEATURE_PRESETS_MANUALUSERCUSTOM_TESTTREE

#endif // ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS

#endif // USE_MODULE_LIGHTS_ANIMATOR

