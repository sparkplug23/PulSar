#include "../../mAnimatorLight.h"


#ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT



/**************************************************************************************************************************************************************
 * @brief  Solid_Colour_Based_On_Sun_Elevation_02
 * @note   From -10 to noon, CCT will range from yellow to daywhite
 * @note   From -5 to dusk, blue will go from 0 to max_brightness 
 * 
 * @note   Gloabl brightness will be manual, or controlled indirectly eg via mqtt
 * 
 * @note   Using RgbcctColour palette that is designed for each point in elevation
 * *************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_03()
{
 
  // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_02"));

  pCONT_iLight->animation.palette.id = mPaletteI->PALETTELIST_STATIC_SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01_ID;

 // AddLog(LOG_LEVEL_TEST, PSTR("SubTask_Flasher_Animate_Function__Solid_Static_Single_Colour"));
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
  sun_elevation = (float)pCONT_solar->solar_position_testing.elevation;
  #else
  sun_elevation = (float)pCONT_solar->solar_position.elevation;
  #endif
#endif
  bool sun_is_ascending = true;//pCONT_solar->solar_position_testing.direction.is_ascending;

  // Serial.printf("\n\r\n\rsun_elevation\t => %f\n\r", sun_elevation);
  /**
   * Sun elevation indexing is stored in palettes index location.
   * The current sun elevation shall be searched against for nearest match, then depending on accesending or decending sun the nearest match and nearest next match will be linearblended as current show colour
   * */

  /**
   * Get total pixels in palette
   * */
  mPalette::PALETTELIST::PALETTE *palette_p = mPaletteI->GetPalettePointerByID(mPaletteI->PALETTELIST_STATIC_SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01_ID);
  uint8_t pixels_max = mPaletteI->GetPixelsInMap(palette_p);
  AddLog(LOG_LEVEL_INFO,PSTR("pixels_max=%d"),pixels_max);

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
  double last_index_difference = 0;
  double current_index_difference = 0;
  uint8_t lowest_difference_pixel_index = 0;
  int16_t indexing = 0;
  int16_t adjusted_index_back_to_minus90_to_plus90 = 0;

  
  uint8_t lower_boundary_index = 13;
  float lower_boundary_value = 45;
  uint8_t upper_boundary_index = 14;
  float upper_boundary_value = 90;




  // Generate all the errors into an array
  float sun_error[pixels_max];  
  int16_t palette_indexes[pixels_max];  
  // for(int i=0;i<pixels_max;i++)
  // {
  //   mPaletteI->GetColourFromPalette(palette_p, i, &indexing);
  //   palette_indexes[i] = indexing - 90;
  //   sun_error[i] = (float)sun_elevation - palette_indexes[i];
  //   AddLog(LOG_LEVEL_INFO, PSTR("sun_error[%d] = %d %d \t %d"),i, indexing,  (int)sun_error[i], palette_indexes[i]);
  //   // Also record zero-crossing point
  //   if((indexing-90) == 0)
  //   {
  //     zero_crossing_index = i;
  //     AddLog(LOG_LEVEL_INFO, PSTR("zero_crossing_index = %d FOUND"), zero_crossing_index);
  //   }
  // }

  //     AddLog(LOG_LEVEL_INFO, PSTR(DEBUG_INSERT_PAGE_BREAK "zero_crossing_index = %d FOUND"), zero_crossing_index);
  /**
   * Ascending method for finding right region between points
   * Check all, but once sun_elev is greater, then thats the current region
   * */
  float sun_positions_from_palette_index[pixels_max];    
  for(int i=0;i<pixels_max;i++)
  {
    mPaletteI->GetColourFromPalette(palette_p, i, &indexing);
    sun_positions_from_palette_index[i] = indexing - 90;
    Serial.printf("sun_pos=[%02d]=\t%f\n\r", i, sun_positions_from_palette_index[i]);
  }





  uint8_t searching_matched_index = 0;
  for(int i=0;i<pixels_max;i++)
  {
    Serial.printf("sun=%f > index[%d]=%f\n\r", sun_elevation, i, sun_positions_from_palette_index[i]);
    if(sun_elevation >= sun_positions_from_palette_index[i])
    {
      
      // searching_matched_index = i;
      // Serial.printf("sun=%f > index[%d]=%f   MATCH=%d\n\r", 
      //   sun_elevation, i, sun_positions_from_palette_index[i], searching_matched_index
      // );
      Serial.printf("Still less\n\r");

    }else{
      
      searching_matched_index = i-1;
      // Serial.printf("sun=%f > index[%d]=%f   MATCH=%d\n\r", 
      //   sun_elevation, i, sun_positions_from_palette_index[i], searching_matched_index
      // );
      Serial.printf("searching_matched_index = %d\n\r", searching_matched_index);
      break;

    }
  }

  lower_boundary_index = searching_matched_index;
  upper_boundary_index = searching_matched_index+1;
  lower_boundary_value = sun_positions_from_palette_index[lower_boundary_index];
  upper_boundary_value = sun_positions_from_palette_index[upper_boundary_index];

  /**
   * Check ranges are valid, if not, reset to 0 and 1
   * */

  float numer = sun_elevation        - lower_boundary_value;
  float denum = upper_boundary_value - lower_boundary_value;
  float progress_between_colours = numer/denum;

  Serial.printf("\n\r\n\r\n\rsun_elevation\t => %f\n\r", sun_elevation);
  Serial.printf("lower_boundary_value[%02d]=%f\n\r", lower_boundary_index, lower_boundary_value);
  Serial.printf("upper_boundary_value[%02d]=%f\n\r", upper_boundary_index, upper_boundary_value);
  Serial.printf("numer=\t%f\n\r",numer);
  Serial.printf("denum=\t%f\n\r",denum);
  Serial.printf("progress_between_colours=\t%f\n\r",progress_between_colours);










//   float progress_between_colours = 0;

//   /**
//    * Change to working out the positive or negative side
//    * */
//   if(sun_elevation>=0)
//   {

//     AddLog(LOG_LEVEL_INFO, PSTR("Checking POSITIVE elevation\n\r"));

//     uint8_t smallest_error_index = 0;
//     float smallest_elev_error = fabs(sun_error[zero_crossing_index]);
//     for(int i=zero_crossing_index+1;i<pixels_max;i++) // skip first
//     {
//       if(fabs(sun_error[i])<smallest_elev_error)
//       {
//         smallest_error_index = i;
//         smallest_elev_error = fabs(sun_error[i]);
//       }
//       // AddLog(LOG_LEVEL_INFO, PSTR("sun_error[%d] = %d\t smallest_elev[%d]=%d"),
//       //     i, (int)sun_error[i], smallest_error_index, (int)smallest_elev_error );
//     }

//     /**
//      * previous colour will always equal the matched index (bring to tersary)
//      * Work out the transitions
//      * */
//     if(sun_error[smallest_error_index] == 0) // EXACT ERROR
//     {
//       AddLog(LOG_LEVEL_INFO, PSTR("sun_error EXACT"));
//       // next index depends on direction
//       if (sun_is_ascending){
//         index.previous = smallest_error_index; // equals this exact index
//         index.next = smallest_error_index + 1;
//         progress_between_colours = ((float)palette_indexes[index.next]/(float)palette_indexes[index.previous]);
//       }else{
//         index.previous = smallest_error_index; // equals this exact index
//         index.next = smallest_error_index - 1;
//         progress_between_colours = ((float)palette_indexes[index.previous]/(float)palette_indexes[index.next]);
//       }
//       Serial.printf("prev=%d, next=%d, progress=%d\n\r", index.previous, index.next, progress_between_colours);
//     }else{                                    // CLOSE ERROR
//       // depends on direction
//       AddLog(LOG_LEVEL_INFO, PSTR("sun_error CLOSE"));
//       if (sun_is_ascending){ // sun going up, means next will be bigger in elevation, in palette index, means index larger (since palette starts with negatives)
//         AddLog(LOG_LEVEL_INFO, PSTR("sun_error[smallest_error_index] != 0 sun_is_ascending"));
        
//         sun_elevation = 50;
//         // smallest_error_index=11;

//         float lower_boundary_index = 13;
//         float lower_boundary_value = 45;
//         float upper_boundary_index = 14;
//         float upper_boundary_value = 90;


//         // index.previous = lower_boundary_index; // equals this exact index
//         // index.next = upper_boundary_index;



//         // float numer = smallest_error_index - (float)palette_indexes[index.next];
//         // float denum = smallest_error_index - (float)palette_indexes[index.previous];


//         float numer = sun_elevation - lower_boundary_value;
//         float denum = upper_boundary_value - lower_boundary_value;
//         float ratio = numer/denum;



//         Serial.printf("\n\r\n\r\n\rsun_elevation\t => %f\n\r", sun_elevation);
//         Serial.printf("numer=%f\n\r",numer);
//         Serial.printf("denum=%f\n\r",denum);
//         Serial.printf("ratio=%f\n\r",ratio);





//         // Serial.printf("%d %d %f %f\n\r", palette_indexes[index.next],palette_indexes[index.previous],numer,denum);
//         // Serial.printf("%d %d %f %f\n\r", palette_indexes[index.next],palette_indexes[index.previous],numer,denum);

//         /**
//          * 10 ======= 20
//          *      15
//          * 
//          * 20-10 = 10
//          * 15-10 = 5
//          * 5/10 = 0.5
//          * 
//          * 2-1 = 1
//          * 1.5-1=0.5      * 
//          * 0.5/1 => 0.5
//          *  
//          * 10*(1+0.5) = 15
//          * 
//          * In equation
//          * 
//          * full_range = next_max_val - prev_min_val
//          * current_range = current_inbetween_val - prev_min_val
//          * 
//          * ratio_of_range = current_range/full_range
//          * 
//          * */

//         for(int i=0;i<pixels_max;i++)
//         {
//           Serial.printf("palette_indexes[%02d] = %d\n\r",i, palette_indexes[i]);
//         }
 
//         float next_max_val = palette_indexes[index.next];
//         float prev_min_val = palette_indexes[index.previous];
//         float current_inbetween_val = smallest_elev_error;

//         float full_range = next_max_val - prev_min_val;
//         float current_range = current_inbetween_val - prev_min_val;
//         float ratio_of_range = current_range/full_range;

//         // Serial.printf("\n\r\n\r\n\rsun_elevation\t => %f\n\r", sun_elevation);
//         // Serial.printf("smallest_error_index=%d\n\r",smallest_error_index);
//         // Serial.printf("index.previous=%d\n\r",index.previous);
//         // Serial.printf("index.next=%d\n\r",index.next);
//         // Serial.printf("next_max_val=%f\n\r",next_max_val);
//         // Serial.printf("prev_min_val=%f\n\r",prev_min_val);
//         // Serial.printf("current_inbetween_val=%f\n\r",current_inbetween_val);
//         // Serial.printf("full_range=%f\n\r",full_range);
//         // Serial.printf("current_range=%f\n\r",current_range);
//         // Serial.printf("ratio_of_range=%f\n\r",ratio_of_range);

       
//         // AddLog_f

//         progress_between_colours = (numer/denum);


//       }else{
//         index.previous = smallest_error_index; // equals this exact index
//         index.next = smallest_error_index - 1;
//         progress_between_colours = ((float)palette_indexes[index.previous]/(float)palette_indexes[index.next]);
//       }
//       Serial.printf("prev=%d, next=%d, progress=%f\n\r", index.previous, index.next, progress_between_colours);
//     }


//   }
//   else // sun_elevation<0
//   {

//     AddLog(LOG_LEVEL_INFO, PSTR("Checking NEGATIVE elevation"));
    
//     uint8_t smallest_error_index = 0;
//     float smallest_elev_error = fabs(sun_error[0]);
//     for(int i=1;i<zero_crossing_index;i++) // skip first
//     {
//       if(fabs(sun_error[i])<smallest_elev_error)
//       {
//         smallest_error_index = i;
//         smallest_elev_error = fabs(sun_error[i]);
//       }
//       AddLog(LOG_LEVEL_INFO, PSTR("sun_error[%d] = %d smallest_elev_error%d smallest_error_index%d"),i, (int)sun_error[i], (int)smallest_elev_error, smallest_error_index);
//     }

//     /**
//      * previous colour will always equal the matched index (bring to tersary)
//      * Work out the transitions
//      * */
//     if(sun_error[smallest_error_index] == 0)
//     {
//       AddLog(LOG_LEVEL_INFO, PSTR("sun_error[smallest_error_index] == 0"));
//       // next index depends on direction
//       if (sun_is_ascending){
//         index.previous = smallest_error_index; // equals this exact index
//         index.next = smallest_error_index + 1;
//         progress_between_colours = 0;//((float)palette_indexes[index.next]/(float)palette_indexes[index.previous]);
//       }else{
//         index.previous = smallest_error_index; // equals this exact index
//         index.next = smallest_error_index - 1;
//         progress_between_colours = 0;//((float)palette_indexes[index.previous]/(float)palette_indexes[index.next]);
//       }
//     }else{
//       // depends on direction
//       AddLog(LOG_LEVEL_INFO, PSTR("sun_error[smallest_error_index] != 0"));
//       if (sun_is_ascending){ // sun going up, means next will be bigger in elevation, in palette index, means index larger (since palette starts with negatives)
//       AddLog(LOG_LEVEL_INFO, PSTR("sun_error[smallest_error_index] != 0 sun_is_ascending"));
//         index.previous = smallest_error_index; // equals this exact index
//         index.next = smallest_error_index + 1;
//         progress_between_colours = ((float)palette_indexes[index.next]/(float)palette_indexes[index.previous]);
//       }else{
//         index.previous = smallest_error_index; // equals this exact index
//         index.next = smallest_error_index - 1;
//         progress_between_colours = ((float)palette_indexes[index.previous]/(float)palette_indexes[index.next]);
//       }
//     }


//   }

/**
 * Showing the colours
 * 1) previous
 * 2) next
 * 3) linearblend of the exact new colour
 * */

    RgbcctColor c_prev = mPaletteI->GetColourFromPalette(palette_p, lower_boundary_index);
    RgbcctColor c_next = mPaletteI->GetColourFromPalette(palette_p, upper_boundary_index);

    Serial.printf("progress_between_colours\t %f(%d)/%f(%d) => %f\n\r", 
      lower_boundary_value, lower_boundary_index, 
      upper_boundary_value, upper_boundary_index, progress_between_colours);

    RgbcctColor c_blended = RgbcctColor::LinearBlend(c_prev,c_next,progress_between_colours);

    RgbcctColor c = c_prev; 
    AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_p\t%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WW,c.WC);
    c = c_blended; 
    AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_b\t%d,%d,%d,%d,%d (progress %d"),c.R,c.G,c.B,c.WW,c.WC, (int)(progress_between_colours*100));
    c = c_next; 
    AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_n\t%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WW,c.WC);

    /**
     * Load new colour into animation
     * */

//   /**
//    * Debug, show all colours
//    * */
//   int16_t adjusted_index = 0;
//   for(int desired_index=0;desired_index<pixels_max;desired_index++)
//   {
//     RgbcctColor c = mPaletteI->GetColourFromPalette(mPaletteI->GetPalettePointerByID(mPaletteI->PALETTELIST_STATIC_SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01_ID), desired_index, &indexing);
//     adjusted_index = indexing - 90;
//     AddLog(LOG_LEVEL_INFO, PSTR("rgbcct[%d]=\t%d\t%d\t%d,%d,%d,%d,%d"),desired_index,indexing,adjusted_index,c.R,c.G,c.B,c.WW,c.WC);
//   }

  // delay(100);

  
  pCONT_iLight->animation.flags.fForceUpdate = true;

  animation_colours_rgbcct.DesiredColour  = c_blended;//mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr);

  // AddLog(LOG_LEVEL_TEST, PSTR("DesiredColour1=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
  if(!pCONT_iLight->rgbcct_controller.getApplyBrightnessToOutput()){ // If not already applied, do it using global values
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