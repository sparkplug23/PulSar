#include "6_Lights/03_Animator/mAnimatorLight.h"

// #ifdef USE_MODULE_LIGHTS_ANIMATOR_OUTSIDE

// #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS

#ifdef USE_DEVFEATURE_PRESETS_MANUALUSERCUSTOM_OUTSIDETREE

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

  ALOG_INF(PSTR("HEEEEEEEEEEEEEEEEEEEEEEEEERE LoadPreset_ManualUserCustom_ByID=%d"),id);

//   #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

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
  
    // CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__STATIC_PALETTE__ID);
    // CommandSet_PaletteID(mPalette::PALETTELIST_STATIC__FESTIVE_SNOWY_COLOURS_02__ID);
    // CommandSet_Animation_Transition_Time_Ms(0);
    // CommandSet_Animation_Transition_Rate_Ms(1000);   
    // SEGMENT_I(0).set_intensity(30);
    // SEGMENT_I(0).set_speed(255);
    // SEGMENT_I(0).grouping = 1;
    // pCONT_iLight->CommandSet_Global_BrtRGB_255(255);
  
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
    
    // CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__SHIMMERING_PALETTE__ID);
    // CommandSet_PaletteID(mPalette::PALETTELIST_STATIC__FESTIVE_SNOWY_COLOURS_02__ID);
    // CommandSet_Animation_Transition_Time_Ms(0);
    // CommandSet_Animation_Transition_Rate_Ms(23);    
    // SEGMENT_I(0).set_intensity(255);
    // SEGMENT_I(0).set_speed(200);
    // SEGMENT_I(0).grouping = 1;
    // pCONT_iLight->CommandSet_Global_BrtRGB_255(255);

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

    // CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__SLOW_GLOW__ID);
    // CommandSet_PaletteID(mPalette::PALETTELIST_STATIC__FESTIVE_SNOWY_COLOURS_02__ID);
    // CommandSet_Animation_Transition_Time_Ms(500);
    // CommandSet_Animation_Transition_Rate_Ms(1000); 
    // SEGMENT_I(0).set_intensity(30);
    // SEGMENT_I(0).set_speed(255);
    // SEGMENT_I(0).grouping = 1;
    // pCONT_iLight->CommandSet_Global_BrtRGB_255(255);

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

    // CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__SLOW_GLOW__ID);
    // CommandSet_PaletteID(mPalette::PALETTELIST_STATIC__FESTIVE_SNOWY_COLOURS_02__ID);
    // CommandSet_Animation_Transition_Time_Ms(0);
    // CommandSet_Animation_Transition_Rate_Ms(50); 
    // SEGMENT_I(0).set_intensity(20);
    // SEGMENT_I(0).set_speed(255);
    // SEGMENT_I(0).grouping = 1;
    // pCONT_iLight->CommandSet_Global_BrtRGB_255(255);

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

    // CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__TWINKLE_PALETTE_SEC_ON_ORDERED_PALETTE_PRI__ID);
    // CommandSet_PaletteID(mPalette::PALETTELIST_STATIC__FESTIVE_SNOWY_COLOURS_02__ID);
    // CommandSet_Animation_Transition_Time_Ms(0);
    // CommandSet_Animation_Transition_Rate_Ms(100); 
    // SEGMENT_I(0).set_intensity(3);
    // SEGMENT_I(0).set_speed(127);
    // SEGMENT_I(0).grouping = 1;
    // pCONT_iLight->CommandSet_Global_BrtRGB_255(255);

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
// make my own for this
    // CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__FIREWORKS_STARBURST_GLOWS__ID);
    // CommandSet_PaletteID(mPalette::PALETTELIST_STATIC__FESTIVE_SNOWY_COLOURS_02__ID);
    // CommandSet_Animation_Transition_Time_Ms(0);
    // CommandSet_Animation_Transition_Rate_Ms(100); 
    // SEGMENT_I(0).set_intensity(255);
    // SEGMENT_I(0).set_speed(255);
    // SEGMENT_I(0).grouping = 3;
    // pCONT_iLight->CommandSet_Global_BrtRGB_255(255);

    // SEGMENT_I(0).set_colors(1, 50,50,50, 0);
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

    // CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__STEPPING_PALETTE__ID);
    // CommandSet_PaletteID(mPalette::PALETTELIST_STATIC__COLOURFUL_PAIRS_01__ID);
    // CommandSet_Animation_Transition_Time_Ms(1500);
    // CommandSet_Animation_Transition_Rate_Ms(3000); 
    // SEGMENT_I(0).set_intensity(20);
    // SEGMENT_I(0).set_speed(255);
    // SEGMENT_I(0).grouping = 1;
    // pCONT_iLight->CommandSet_Global_BrtRGB_255(255);

  }else
  /**
   * @brief
   * 
   */
  if(
    (id == 7) &&
    (true) // Time Allowed, true for always
  ){

    // CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__STEPPING_PALETTE__ID);
    // CommandSet_PaletteID(mPalette::PALETTELIST_STATIC__FESTIVE_SNOWY_COLOURS_02__ID);
    // CommandSet_Animation_Transition_Time_Ms(1500);
    // CommandSet_Animation_Transition_Rate_Ms(3000); 
    // SEGMENT_I(0).set_intensity(20);
    // SEGMENT_I(0).set_speed(255);
    // SEGMENT_I(0).grouping = 100;
    // pCONT_iLight->CommandSet_Global_BrtRGB_255(255);

  }else


// I want rows of all "24 colours", then rotate them up the tree

  if(
    (id == 8) &&
    (true) // Time Allowed, true for always
  ){

    // CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__STATIC_PALETTE__ID);
    // CommandSet_PaletteID(mPalette::PALETTELIST_STATIC__FESTIVE_SNOWY_COLOURS_02__ID);   // no purple
    // CommandSet_Animation_Transition_Time_Ms(10000);
    // CommandSet_Animation_Transition_Rate_Ms(20000); 
    // SEGMENT_I(0).set_intensity(20);
    // SEGMENT_I(0).set_speed(255);
    // SEGMENT_I(0).grouping = 100;
    // pCONT_iLight->CommandSet_Global_BrtRGB_255(255);

  }else



  /**
   * @brief
   * 
   */
  if(
    (id == 9) &&
    (true) // Time Allowed, true for always
  ){

    // CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__ROTATING_PREVIOUS_ANIMATION__ID);
    // CommandSet_PaletteID(mPalette::PALETTELIST_STATIC__FESTIVE_SNOWY_COLOURS_02__ID); //no purple
    // CommandSet_Animation_Transition_Time_Ms(50);
    // CommandSet_Animation_Transition_Rate_Ms(100); 
    // SEGMENT_I(0).set_intensity(20);
    // SEGMENT_I(0).set_speed(255);
    // SEGMENT_I(0).grouping = 1;
    // pCONT_iLight->CommandSet_Global_BrtRGB_255(255);


  }else



  /**
   * @brief
   * 
   */
  if(
    (id == 10) &&
    (true) // Time Allowed, true for always
  ){

    // CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__COLOR_WIPE_RANDOM__ID);
    // CommandSet_PaletteID(mPalette::PALETTELIST_STATIC__FESTIVE_SNOWY_COLOURS_02__ID);
    // CommandSet_Animation_Transition_Time_Ms(0);
    // CommandSet_Animation_Transition_Rate_Ms(23); 
    // SEGMENT_I(0).set_intensity(255); //smoothness // https://github.com/Aircoookie/WLED/wiki/List-of-effects-and-palettes
    // SEGMENT_I(0).set_speed(100);
    // SEGMENT_I(0).grouping = 30; //added smoothness for around the tree
    // pCONT_iLight->CommandSet_Global_BrtRGB_255(255);

  }else
  /**
   * @brief
   * 
   */
  if(
    (id == 11) &&
    (true) // Time Allowed, true for always
  ){

    // CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__FIREWORKS_EXPLODING__ID);
    // CommandSet_PaletteID(mPalette::PALETTELIST_STATIC__FESTIVE_SNOWY_COLOURS_02__ID);
    // CommandSet_Animation_Transition_Time_Ms(0);
    // CommandSet_Animation_Transition_Rate_Ms(23); 
    // SEGMENT_I(0).set_intensity(255); //smoothness // https://github.com/Aircoookie/WLED/wiki/List-of-effects-and-palettes
    // SEGMENT_I(0).set_speed(126);
    // SEGMENT_I(0).grouping = 1;
    // pCONT_iLight->CommandSet_Global_BrtRGB_255(255);

  }else
  /**
   * @brief
   * 
   */
  if(
    (id == 12) &&
    (true) // Time Allowed, true for always
  ){

    // CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__FIREWORKS_EXPLODING_NO_LAUNCH__ID);
    // CommandSet_PaletteID(mPalette::PALETTELIST_STATIC__FESTIVE_SNOWY_COLOURS_02__ID);
    // CommandSet_Animation_Transition_Time_Ms(0);
    // CommandSet_Animation_Transition_Rate_Ms(23); 
    // SEGMENT_I(0).set_intensity(255); //smoothness // https://github.com/Aircoookie/WLED/wiki/List-of-effects-and-palettes
    // SEGMENT_I(0).set_speed(126);
    // SEGMENT_I(0).grouping = 1;
    // pCONT_iLight->CommandSet_Global_BrtRGB_255(255);

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

    // CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__STATIC_PALETTE__ID);
    // CommandSet_PaletteID(mPalette::PALETTELIST_STATIC__FESTIVE_SNOWY_COLOURS_02__ID);   // no purple
    // CommandSet_Animation_Transition_Time_Ms(10000);
    // CommandSet_Animation_Transition_Rate_Ms(20000); 
    // SEGMENT_I(0).set_intensity(20);
    // SEGMENT_I(0).set_speed(255);
    // SEGMENT_I(0).grouping = 100;
    // pCONT_iLight->CommandSet_Global_BrtRGB_255(255);


  }else

    // Grouped but nicer colour options

  if(
    (id == 14) &&
    (true) // Time Allowed, true for always
  ){

    // CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__STATIC_PALETTE__ID);
    // CommandSet_PaletteID(mPalette::PALETTELIST_STATIC__FESTIVE_SNOWY_COLOURS_02__ID);   // Custom for this tree 2022
    // CommandSet_Animation_Transition_Time_Ms(10000);
    // CommandSet_Animation_Transition_Rate_Ms(20000); 
    // SEGMENT_I(0).set_intensity(20);
    // SEGMENT_I(0).set_speed(255);
    // SEGMENT_I(0).grouping = 100;
    // pCONT_iLight->CommandSet_Global_BrtRGB_255(255);

  }else

  /**
   * @brief Aiming to take current G=100 blocked colours that rotate, and instead do blocks of 
   *        Repeated 4 colours that rotate with blending on the tree
   * 
   * This will still not work!
   * 
   * This close to the end of christmas, lets just make up manual new animations and input via openhab
   * 
   * @param id 
   */
  if(
    (id == 15) &&
    (true) // Time Allowed, true for always
  ){
    

    CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__STATIC_PALETTE__ID);
    CommandSet_PaletteID(mPalette::PALETTELIST_STATIC__FESTIVE_SNOWY_COLOURS_02__ID);  
    
    CommandSet_Animation_Transition_Time_Ms(10000);
    CommandSet_Animation_Transition_Rate_Ms(20000); 
    SEGMENT_I(0).set_intensity(20);
    SEGMENT_I(0).set_speed(255);
    SEGMENT_I(0).grouping = 25;
    pCONT_iLight->CommandSet_Global_BrtRGB_255(255);

    ALOG_INF(PSTR("Static,Snow02,t10000,r20000,i20,g25"));

  }else
  /**
   * @brief
   * 
   */
  if(
    (id == 16) &&
    (true) // Time Allowed, true for always
  ){

    CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__ROTATING_PREVIOUS_ANIMATION__ID);
    CommandSet_PaletteID(mPalette::PALETTELIST_STATIC__FESTIVE_SNOWY_COLOURS_02__ID); //no purple
    CommandSet_Animation_Transition_Time_Ms(900);
    CommandSet_Animation_Transition_Rate_Ms(1000); 
    SEGMENT_I(0).set_intensity(20);
    SEGMENT_I(0).set_speed(255);
    SEGMENT_I(0).grouping = 25;
    pCONT_iLight->CommandSet_Global_BrtRGB_255(255);


  }else

  {}


//   #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE  

} // END Function




// /**
//  * @brief Temporary solution for christmas, design of outside tree
//  * Will be tested at 1/minute
//  * 
//  * 
//  * Instead of limiting effects by time here, I should just make a second group to be called the other times of the day
//  * 
//  * Flashing effects 4-7pm
//  * Subtle effects 8-10pm then off
//  * 
//  * 
//  * 
//  * @param id 
//  */
// void mAnimatorLight::LoadPreset_ManualUserCustom_ByID(uint8_t id)
// {

//   ALOG_INF(PSTR("LoadPreset_ManualUserCustom_ByID=%d"),id);

//   #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

//   /**
//    * @brief
//    * Effect:  Static Palette
//    * Palette: Christmas Colourful for Outside Tree (Many, but 2021 colours)
//    * 
//    */
//   if(
//     (id == 0) &&
//     (true) // Time Allowed, true for always
//   ){
  
//     CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__STATIC_PALETTE__ID);
//     CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_24__ID);
//     CommandSet_Animation_Transition_Time_Ms(0);
//     CommandSet_Animation_Transition_Rate_Ms(1000);   
//     SEGMENT_I(0).set_intensity(30);
//     SEGMENT_I(0).set_speed(255);
//     SEGMENT_I(0).grouping = 1;
//     CommandSet_Global_BrtRGB_255(255);
  
//   }else
//   /**
//    * @brief
//    * Effect: Static Palette
//    * Palette: Christmas Colourful for Outside Tree (New, Extreme Colours, With Warm Tone)
//    * 
//    */
//   if(
//     (id == 1) &&
//     (true) // Time Allowed, true for always
//   ){
    
//     CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__SHIMMERING_PALETTE__ID);
//     CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_24__ID);
//     CommandSet_Animation_Transition_Time_Ms(0);
//     CommandSet_Animation_Transition_Rate_Ms(23);    
//     SEGMENT_I(0).set_intensity(255);
//     SEGMENT_I(0).set_speed(200);
//     SEGMENT_I(0).grouping = 1;
//     CommandSet_Global_BrtRGB_255(255);

//   }else
//   /**
//    * @brief
//    * Effect: Random Palette
//    * Palette: Outside 1 (Many)
//    * 
//    */
//   if(
//     (id == 2) &&
//     (true) // Time Allowed, true for always
//   ){   

//     CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__SLOW_GLOW__ID);
//     CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_24__ID);
//     CommandSet_Animation_Transition_Time_Ms(500);
//     CommandSet_Animation_Transition_Rate_Ms(1000); 
//     SEGMENT_I(0).set_intensity(30);
//     SEGMENT_I(0).set_speed(255);
//     SEGMENT_I(0).grouping = 1;
//     CommandSet_Global_BrtRGB_255(255);

//   }else
//   /**
//    * @brief
//    * Effect: Shimmering Palette (Fast enough to be noticed!)
//    * Palette: Christmas Colourful for Outside Tree (Many, but 2021 colours)
//    * 
//    * Fast enough and intense enough to be noticed
//    * 
//    */
  
//   if(
//     (id == 3) &&
//     (true) // Time Allowed, true for always
//   ){

//     CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__SLOW_GLOW__ID);
//     CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_24__ID);
//     CommandSet_Animation_Transition_Time_Ms(0);
//     CommandSet_Animation_Transition_Rate_Ms(50); 
//     SEGMENT_I(0).set_intensity(20);
//     SEGMENT_I(0).set_speed(255);
//     SEGMENT_I(0).grouping = 1;
//     CommandSet_Global_BrtRGB_255(255);

//   }else
//   /**
//    * @brief
//    * Effect: White Twinkle (Fast to be noticed!) = new effect, which can allow another palette to twinkle over another (ie palette 10 to white only, vs others with many.... eg white tree with coloured flashing)
//    * Palette: Christmas Colourful for Outside Tree (Many, but 2021 colours)
//    * It basically randomly takes a colour from palette_sec and twinkles it over palette_pri
//    * 
//    */
//   if(
//     (id == 4) &&
//     (true) // Time Allowed, true for always
//   ){

//     CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__TWINKLE_PALETTE_SEC_ON_ORDERED_PALETTE_PRI__ID);
//     CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_24__ID);
//     CommandSet_Animation_Transition_Time_Ms(0);
//     CommandSet_Animation_Transition_Rate_Ms(100); 
//     SEGMENT_I(0).set_intensity(3);
//     SEGMENT_I(0).set_speed(127);
//     SEGMENT_I(0).grouping = 1;
//     CommandSet_Global_BrtRGB_255(255);

//   }else
//   /**
//    * @brief
//    * Effect: White Twinkle (Fast to be noticed!) = new effect, which can allow another palette to twinkle over another (ie palette 10 to white only, vs others with many.... eg white tree with coloured flashing)
//    * Palette: Christmas Colourful for Outside Tree (Many, but 2021 colours)
//    * It basically randomly takes a colour from palette_sec and twinkles it over palette_pri
//    * 
//    */
//   if(
//     (id == 5) &&
//     (true) // Time Allowed, true for always
//   ){
// // make my own for this
//     CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__FIREWORKS_STARBURST_GLOWS__ID);
//     CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_24__ID);
//     CommandSet_Animation_Transition_Time_Ms(0);
//     CommandSet_Animation_Transition_Rate_Ms(100); 
//     SEGMENT_I(0).set_intensity(255);
//     SEGMENT_I(0).set_speed(255);
//     SEGMENT_I(0).grouping = 3;
//     CommandSet_Global_BrtRGB_255(255);

//     SEGMENT_I(0).set_colors(1, 50,50,50, 0);
//     // [0] = ((uint32_t)0 << 24) | ((uint32_t)50 << 16) | ((uint32_t)50 << 8) | (uint32_t)50);

//   }else
//   /**
//    * @brief
//    * 
//    */
//   if(
//     (id == 6) &&
//     (true) // Time Allowed, true for always
//   ){

//     CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__STEPPING_PALETTE__ID);
//     CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_10__ID);
//     CommandSet_Animation_Transition_Time_Ms(1500);
//     CommandSet_Animation_Transition_Rate_Ms(3000); 
//     SEGMENT_I(0).set_intensity(20);
//     SEGMENT_I(0).set_speed(255);
//     SEGMENT_I(0).grouping = 1;
//     CommandSet_Global_BrtRGB_255(255);

//   }else
//   /**
//    * @brief
//    * 
//    */
//   if(
//     (id == 7) &&
//     (true) // Time Allowed, true for always
//   ){

//     CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__STEPPING_PALETTE__ID);
//     CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_24__ID);
//     CommandSet_Animation_Transition_Time_Ms(1500);
//     CommandSet_Animation_Transition_Rate_Ms(3000); 
//     SEGMENT_I(0).set_intensity(20);
//     SEGMENT_I(0).set_speed(255);
//     SEGMENT_I(0).grouping = 100;
//     CommandSet_Global_BrtRGB_255(255);

//   }else


// // I want rows of all "24 colours", then rotate them up the tree

//   if(
//     (id == 8) &&
//     (true) // Time Allowed, true for always
//   ){

//     CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__STATIC_PALETTE__ID);
//     CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_25__ID);   // no purple
//     CommandSet_Animation_Transition_Time_Ms(10000);
//     CommandSet_Animation_Transition_Rate_Ms(20000); 
//     SEGMENT_I(0).set_intensity(20);
//     SEGMENT_I(0).set_speed(255);
//     SEGMENT_I(0).grouping = 100;
//     CommandSet_Global_BrtRGB_255(255);

//   }else



//   /**
//    * @brief
//    * 
//    */
//   if(
//     (id == 9) &&
//     (true) // Time Allowed, true for always
//   ){

//     CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__ROTATING_PREVIOUS_ANIMATION__ID);
//     CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_25__ID); //no purple
//     CommandSet_Animation_Transition_Time_Ms(50);
//     CommandSet_Animation_Transition_Rate_Ms(100); 
//     SEGMENT_I(0).set_intensity(20);
//     SEGMENT_I(0).set_speed(255);
//     SEGMENT_I(0).grouping = 1;
//     CommandSet_Global_BrtRGB_255(255);


//   }else



//   /**
//    * @brief
//    * 
//    */
//   if(
//     (id == 10) &&
//     (true) // Time Allowed, true for always
//   ){


// // {
// //   "BrightnessRGB": 100,
// //   "ColourPalette": "Christmas 10",
// //   "Transition": {
// //     "RateMs": 23,
// //     "TimeMs": 0
// //   },
// //   "Effects": {
// //     "Function": 21,
// //     "Grouping": 30,
// //     "Speed": 100,
// //     "Intensity": 255
// //   },
// //   "WLED":{"Colour1":[0,0,0,0]}
// // }


//     // CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__COLOR_WIPE_RANDOM__ID);
//     // CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_24__ID);
//     // CommandSet_Animation_Transition_Time_Ms(0);
//     // CommandSet_Animation_Transition_Rate_Ms(23); 
//     // SEGMENT_I(0).set_intensity(255); //smoothness // https://github.com/Aircoookie/WLED/wiki/List-of-effects-and-palettes
//     // SEGMENT_I(0).set_speed(127);
//     // SEGMENT_I(0).grouping = 1;
//     // CommandSet_Global_BrtRGB_255(255);

//     CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__COLOR_WIPE_RANDOM__ID);
//     CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_24__ID);
//     CommandSet_Animation_Transition_Time_Ms(0);
//     CommandSet_Animation_Transition_Rate_Ms(23); 
//     SEGMENT_I(0).set_intensity(255); //smoothness // https://github.com/Aircoookie/WLED/wiki/List-of-effects-and-palettes
//     SEGMENT_I(0).set_speed(100);
//     SEGMENT_I(0).grouping = 30; //added smoothness for around the tree
//     CommandSet_Global_BrtRGB_255(255);

//   }else
//   /**
//    * @brief
//    * 
//    */
//   if(
//     (id == 11) &&
//     (true) // Time Allowed, true for always
//   ){

//     CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__FIREWORKS_EXPLODING__ID);
//     CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_24__ID);
//     CommandSet_Animation_Transition_Time_Ms(0);
//     CommandSet_Animation_Transition_Rate_Ms(23); 
//     SEGMENT_I(0).set_intensity(255); //smoothness // https://github.com/Aircoookie/WLED/wiki/List-of-effects-and-palettes
//     SEGMENT_I(0).set_speed(126);
//     SEGMENT_I(0).grouping = 1;
//     CommandSet_Global_BrtRGB_255(255);

//   }else
//   /**
//    * @brief
//    * 
//    */
//   if(
//     (id == 12) &&
//     (true) // Time Allowed, true for always
//   ){

//     CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__FIREWORKS_EXPLODING_NO_LAUNCH__ID);
//     CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_24__ID);
//     CommandSet_Animation_Transition_Time_Ms(0);
//     CommandSet_Animation_Transition_Rate_Ms(23); 
//     SEGMENT_I(0).set_intensity(255); //smoothness // https://github.com/Aircoookie/WLED/wiki/List-of-effects-and-palettes
//     SEGMENT_I(0).set_speed(126);
//     SEGMENT_I(0).grouping = 1;
//     CommandSet_Global_BrtRGB_255(255);

//   }else

//   /////////// BELOW ARE TEST but not considered part of normal operation //////////////////////////////////////////////////////////////////////////////////

//   /**
//    * @brief
//    * Test to view with white
//    */
//   if(
//     (id == 13) &&
//     (true) // Time Allowed, true for always
//   ){

//     CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__STATIC_PALETTE__ID);
//     CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_25__ID);   // no purple
//     CommandSet_Animation_Transition_Time_Ms(10000);
//     CommandSet_Animation_Transition_Rate_Ms(20000); 
//     SEGMENT_I(0).set_intensity(20);
//     SEGMENT_I(0).set_speed(255);
//     SEGMENT_I(0).grouping = 100;
//     CommandSet_Global_BrtRGB_255(255);


//   }else

//     // Grouped but nicer colour options

//   if(
//     (id == 14) &&
//     (true) // Time Allowed, true for always
//   ){

//     CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__STATIC_PALETTE__ID);
//     CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_11__ID);   // Custom for this tree 2022
//     CommandSet_Animation_Transition_Time_Ms(10000);
//     CommandSet_Animation_Transition_Rate_Ms(20000); 
//     SEGMENT_I(0).set_intensity(20);
//     SEGMENT_I(0).set_speed(255);
//     SEGMENT_I(0).grouping = 100;
//     CommandSet_Global_BrtRGB_255(255);

//   }else

//   /**
//    * @brief Aiming to take current G=100 blocked colours that rotate, and instead do blocks of 
//    *        Repeated 4 colours that rotate with blending on the tree
//    * 
//    * This will still not work!
//    * 
//    * This close to the end of christmas, lets just make up manual new animations and input via openhab
//    * 
//    * @param id 
//    */
//   if(
//     (id == 15) &&
//     (true) // Time Allowed, true for always
//   ){

//     CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__STATIC_PALETTE__ID);
//     CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_29__ID);  
    
//     CommandSet_Animation_Transition_Time_Ms(10000);
//     CommandSet_Animation_Transition_Rate_Ms(20000); 
//     SEGMENT_I(0).set_intensity(20);
//     SEGMENT_I(0).set_speed(255);
//     SEGMENT_I(0).grouping = 25;
//     CommandSet_Global_BrtRGB_255(255);

//   }else
//   /**
//    * @brief
//    * 
//    */
//   if(
//     (id == 16) &&
//     (true) // Time Allowed, true for always
//   ){

//     CommandSet_Flasher_FunctionID(EFFECTS_FUNCTION__ROTATING_PREVIOUS_ANIMATION__ID);
//     CommandSet_PaletteID(mPalette::PALETTELIST_STATIC_CHRISTMAS_25__ID); //no purple
//     CommandSet_Animation_Transition_Time_Ms(900);
//     CommandSet_Animation_Transition_Rate_Ms(1000); 
//     SEGMENT_I(0).set_intensity(20);
//     SEGMENT_I(0).set_speed(255);
//     SEGMENT_I(0).grouping = 25;
//     CommandSet_Global_BrtRGB_255(255);


//   }else

//   {}


//   #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE  

// } // END Function



#endif // USE_DEVFEATURE_PRESETS_MANUALUSERCUSTOM_OUTSIDETREE

// #endif // ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS

// #endif // USE_MODULE_LIGHTS_ANIMATOR

