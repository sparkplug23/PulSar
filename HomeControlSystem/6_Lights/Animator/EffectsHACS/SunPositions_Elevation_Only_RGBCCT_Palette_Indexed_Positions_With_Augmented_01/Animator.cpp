
#include "../../mAnimatorLight.h"


#ifdef ENABLE_PIXEL_FUNCTION_EFFECTS



/**************************************************************************************************************************************************************
 * @brief  Solid_Colour_Based_On_Sun_Elevation_02
 * @note   From -10 to noon, CCT will range from yellow to daywhite
 * @note   From -5 to dusk, blue will go from 0 to max_brightness 
 * 
 * @note   Gloabl brightness will be manual, or controlled indirectly eg via mqtt
 * 
 * Instead of direct Elevation to palette, this animation will perform different versions of the mapping. Should this be one animation with subtypes? or different types
 * 
 * @note   Using RgbcctColour palette that is designed for each point in elevation
 * *************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Flasher_Animate_Function_SunPositions_Elevation_Only_RGBCCT_Palette_Indexed_Positions_With_Augmented_01()
{

   AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_02"));

   /**
    * Setting that an mqtt debug exists
    * */
   pCONT_iLight->animation.debug_mqtt_response_available = 1;

  #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
  if(pCONT_iLight->animation.debug_mqtt_response_available && pCONT_iLight->animation.flags.animator_first_run)
  {
    this->setCallback_ConstructJSONBody_Debug_Animations_Progress(
      [this](void){
        this->ConstructJSONBody_Animation_Progress__SunPositions_Elevation_Only_RGBCCT_Palette_Indexed_Positions_With_Augmented_01();
      }
    );
  //   AddLog(LOG_LEVEL_INFO, PSTR("ConstructJSONBody_Animation_Progress__SunPositions_Elevation_Only_RGBCCT_Palette_Indexed_Positions_With_Augmented_01 FIRST RUN"));
  //   delay(5000);
  // }else{
  //   AddLog(LOG_LEVEL_INFO, PSTR("NOT SETTING"));

  }
  #endif // USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
  // delay(5000);

  // pCONT_iLight->animation.palette.id = mPaletteI->PALETTELIST_STATIC_SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01_ID;

  // Set palette pointer
  mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
  // Brightness is generated internally, and rgbcct solid palettes are output values
  pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = false;

  /**
   * Solar data to use, defined here for testing or simulations
   * */
 
float sun_elevation = 0;
#ifdef USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
  #ifdef USE_DEVFEATURE_SUNPOSITION_ELEVATION_USE_TESTING_VALUE
  float sun_elevation = (float)pCONT_solar->solar_position_testing.elevation;
  #else
  float sun_elevation = (float)pCONT_solar->solar_position.elevation;
  #endif
#endif
  bool sun_is_ascending = true;//pCONT_solar->solar_position_testing.direction.is_ascending;
  // Serial.printf("\n\r\n\rsun_elevation\t => %f\n\r", sun_elevation);

  // delay(1000);

  /**
   * Sun elevation indexing is stored in palettes index location.
   * The current sun elevation shall be searched against for nearest match, then depending on accesending or decending sun the nearest match and nearest next match will be linearblended as current show colour
   * */

  /**
   * Get total pixels in palette
   * */
  mPalette::PALETTELIST::PALETTE *palette_p = mPaletteI->GetPalettePointerByID(pCONT_iLight->animation.palette.id);
  uint8_t pixels_max = mPaletteI->GetPixelsInMap(palette_p);
  // AddLog(LOG_LEVEL_INFO,PSTR("pixels_max=%d"),pixels_max);

  // Lets assume we need a zero crossing index, thus, we can use it to identity AS and DE modes
  uint8_t zero_crossing_index = 0;

  struct INDEXES_MATCHES{
    uint8_t previous = 0; //ie colour moving away from
    uint8_t next = 0; //colour moving towards
  }index;

  /**
   * Steps to finding index
   * 1) Find the zero-crossing index from the palette (ie the colour where its index is 0)
   * 2) Decide if elevation is pos or neg, begin searching that part of the array
   * 3) Find index of closest in array
   * 4) Next and previous index will depend on direction of sun, and will be equal to current index if error is exactly 0
   * */

  /**
   * Step X: Find zero crossing point
   * Step X: Find all differences
   * */
  int16_t indexing = 0;  
  uint8_t lower_boundary_index = 13;
  float lower_boundary_value = 45;
  uint8_t upper_boundary_index = 14;  
  float upper_boundary_value = 90;
  float sun_positions_from_palette_index[pixels_max];  
  uint8_t searching_matched_index = 0;

  /**
   * Ascending method for finding right region between points
   * Check all, but once sun_elev is greater, then thats the current region
   * */
  for(int i=0;i<pixels_max;i++)
  {
    mPaletteI->GetColourFromPalette(palette_p, i, &indexing);
    sun_positions_from_palette_index[i] = indexing - 90;
    // Serial.printf("sun_pos=[%02d]=\t%f\n\r", i, sun_positions_from_palette_index[i]);
  }


  for(int i=0;i<pixels_max;i++)
  {
    // Serial.printf("sun=%f > index[%d]=%f\n\r", sun_elevation, i, sun_positions_from_palette_index[i]);
    if(sun_elevation >= sun_positions_from_palette_index[i])
    {
      
      // searching_matched_index = i;
      // Serial.printf("sun=%f > index[%d]=%f   MATCH=%d\n\r", 
      //   sun_elevation, i, sun_positions_from_palette_index[i], searching_matched_index
      // );
      //Serial.printf("Still less\n\r");

    }else{
      
      searching_matched_index = i-1;
      // Serial.printf("sun=%f > index[%d]=%f   MATCH=%d\n\r", 
      //   sun_elevation, i, sun_positions_from_palette_index[i], searching_matched_index
      // );
      // Serial.printf("searching_matched_index = %d\n\r", searching_matched_index);
      break;

    }

    // Directly, manually, check the last memory space

    if(sun_elevation == sun_positions_from_palette_index[pixels_max-1])
    {
      searching_matched_index = i-1;
      // Serial.printf("sun=%f > index[%d]=%f   MATCH=%d\n\r", 
      //   sun_elevation, i, sun_positions_from_palette_index[i], searching_matched_index
      // );
      break;

    }




  }

  lower_boundary_index = searching_matched_index;
  upper_boundary_index = searching_matched_index+1;

  /**
   * Check ranges are valid, if not, reset to 0 and 1
   * */
  if(lower_boundary_index>=pixels_max)
  {
    lower_boundary_index = 0;
    Serial.printf("lower_boundary_index>=pixels_max\n\r");
  }
  if(upper_boundary_index>=pixels_max)
  {
    upper_boundary_index = pixels_max;
    Serial.printf("upper_boundary_index>=pixels_max\n\r");
  }

  lower_boundary_value = sun_positions_from_palette_index[lower_boundary_index];
  upper_boundary_value = sun_positions_from_palette_index[upper_boundary_index];


  float numer = sun_elevation        - lower_boundary_value;
  float denum = upper_boundary_value - lower_boundary_value;
  float progress_between_colours = numer/denum;

  Serial.printf("\n\r\n\r\n\rsun_elevation\t => %f\n\r", sun_elevation);
  Serial.printf("lower_boundary_value[%02d]=%f\n\r", lower_boundary_index, lower_boundary_value);
  Serial.printf("upper_boundary_value[%02d]=%f\n\r", upper_boundary_index, upper_boundary_value);
  Serial.printf("numer=\t%f\n\r",numer);
  Serial.printf("denum=\t%f\n\r",denum);
  Serial.printf("progress_between_colours=\t%f\n\r",progress_between_colours);

  /**
   * Showing the colours
   * 1) previous
   * 2) next
   * 3) linearblend of the exact new colour
   * */

  RgbcctColor c_lower = mPaletteI->GetColourFromPalette(palette_p, lower_boundary_index);
  RgbcctColor c_upper = mPaletteI->GetColourFromPalette(palette_p, upper_boundary_index);

  // Serial.printf("progress_between_colours\t %f(%d)/%f(%d) => %f\n\r", 
  //   lower_boundary_value, lower_boundary_index, 
  //   upper_boundary_value, upper_boundary_index, progress_between_colours);

  RgbcctColor c_blended = RgbcctColor::LinearBlend(c_lower, c_upper, progress_between_colours);

  RgbcctColor c = c_lower; 
  // AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_p\t%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WW,c.WC);
  c = c_blended; 
  // AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_b\t%d,%d,%d,%d,%d (progress %d"),c.R,c.G,c.B,c.WW,c.WC, (int)(progress_between_colours*100));
  c = c_upper; 
  // AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_n\t%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WW,c.WC);

  /**
   * Load new colour into animation
   * */

  pCONT_iLight->animation.flags.fForceUpdate = true;

  animation_colours_rgbcct.DesiredColour  = c_blended;


  /**
   * Evening/Sunset, keep the brightness up
   * Morning/Sunrise, wait until closer before increasing brighntess (adjust by elevation_adjust or by time_adjust)
   * Instead of elevation mapping, use sunrise and sunset time to control animations
   * */


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

  pCONT_iLight->animation.flags.animator_first_run = false; // process completed, so lets not redo things above
   
}



void mAnimatorLight::ConstructJSONBody_Animation_Progress__SunPositions_Elevation_Only_RGBCCT_Palette_Indexed_Positions_With_Augmented_01()
{   
// Serial.println("AnimationProcess_Generic_AnimationColour_LinearBlend");
  // for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
  //   RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
  //       animation_colours[pixel].StartingColor,
  //       animation_colours[pixel].DesiredColour,
  //       param.progress);    
  //   SetPixelColor(pixel, updatedColor);
  // } // END for

  // DEBUG_LINE_HERE;

  JBI->Add("test2","debug2");

    // SetPixelColor(0, RgbColor(0,random(0,255),0));
}


#endif