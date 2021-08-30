#include "../../mAnimatorLight.h"


#ifdef ENABLE_PIXEL_FUNCTION_EFFECTS


// Make this an "Effects" which is public/friendly of animator, but not the same class
void mAnimatorLight::SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_01(){

  
      AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_01"));

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
  uint16_t index_circle_center;
  
  uint8_t sun_width;
  uint8_t sun_bloom;
  RgbcctColor sky_near_colour;
  RgbcctColor sky_far_colour;

  uint8_t value = 0;





  uint8_t starting_palette_id;
  uint8_t ending_palette_id;
  uint8_t step_changes_remaining;
  uint8_t colour_select_progress_counter;
  uint8_t active_palette_id;
  uint8_t sun_elevation_degrees;
  uint8_t sun_azimuth_degrees; //left/right from sunrise/sunset position
  // uint8_t sun_bloom; //how wide the main sunlight is
  //store colours in here too
  // uint8_t sun_elevation_degrees;


  uint8_t direction;

  //map "visible" azimuth into far left/right of strip

} effect_config;

float sun_elevation = 0;
#ifdef USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
  #ifdef USE_DEVFEATURE_SUNPOSITION_ELEVATION_USE_TESTING_VALUE
  float sun_elevation = (float)pCONT_solar->solar_position_testing.elevation;
  #else
  float sun_elevation = (float)pCONT_solar->solar_position.elevation;
  #endif
#endif


  // star* stars = reinterpret_cast<star*>(pCONT_iLight->effects_data_buffer);
  EFFECT_CONFIG* effect_config2 = reinterpret_cast<EFFECT_CONFIG*>(pCONT_iLight->effects_data_buffer);
  //should be memcpy be used to insure size if not exceeded? or sizeof check? pointer is faster

  AddLog(LOG_LEVEL_TEST,PSTR("pCONT_iLight->animation.palette.id=%d"),pCONT_iLight->animation.palette.id);

  pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;

  // pCONT_iLight->settings.light_size_count = 1;
  
  // Get all the current state
  // for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
  //   animation_colours[index].DesiredColour = GetPixelColor(index);
  // }
  //   for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
  //   animation_colours[index].DesiredColour = RgbcctColor(9,255,0,0,0);//,0,30,0,0);//,0,10,0,0);
  // }


  /**
   * Colour temp of white channels when day time
   * Amount of blue when sun is down
   * */

  if(sun_elevation)
  {
    /**
     * Max daily elevation will need to be known later
     * */
    // Mapping elevation to CCT percentage

    // pCONT_iLight->CommandSet_ActiveSolidPalette_ColourTemp_Percentage(percentage_cct);

  }else{
    
    for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
      animation_colours[index].DesiredColour = RgbcctColor(0,0,255,0,0);//,0,30,0,0);//,0,10,0,0);
    }
  }

/**
 * colour channels
 * */

  if((sun_elevation>-50)&&(sun_elevation<10))
  {
    // HsbColor hsb = HsbColor(RgbColor(0));
    // hsb.H = map(sun_elevation,-50,10,255,0)

    uint8_t blue =  map(sun_elevation,-50,10,255,0);

    pCONT_iLight->rgbcct_controller.setRGB(0,0,blue);

    // AddLog(LOG_LEVEL_INFO, PSTR("elevation=%d, cct_temp=%d %d"),(int)sun_elevation, elev_perc, cct_val);




    uint8_t brightness_255 = map(sun_elevation,-50,10,255,0);

    pCONT_iLight->rgbcct_controller.setBrightnessRGB255(brightness_255);

  }

/**
 * WHITE CHANNEL
 * */
  if((sun_elevation>-10)&&(sun_elevation<25))
  {


    // Convert elevation into percentage
    uint8_t elev_perc = map(sun_elevation,-10,25,0,100);
    // Convert percentage into cct
    uint16_t cct_val = mapvalue(elev_perc, 0,100, pCONT_iLight->get_CTRangeMin(),pCONT_iLight->get_CTRangeMax());
    // Set the colour temp
    pCONT_iLight->rgbcct_controller.setCCT(cct_val);


    uint8_t brightness_255 = map(sun_elevation,-10,25,0,255);

    pCONT_iLight->rgbcct_controller.setBrightnessCCT255(brightness_255);

  }




  //scan back and forth
  // if(effect_config2->index_circle_center<=0){
  //   effect_config2->direction = 1;
  // }else
  // if(effect_config2->index_circle_center>100){
  //   effect_config2->direction = 0;
  // }

  // if(effect_config2->direction){          
  //   effect_config2->index_circle_center++;
  // }else{
  //   effect_config2->index_circle_center--;
  // }

  effect_config2->index_circle_center = 50;
  
  // if(effect_config2->value >= 0){
  //   // Draw full sky
  //   Draw_DesiredColour_LinearGradientMirrored2(
  //     RgbcctColor(0,0,50,0,0), //center
  //     RgbcctColor(0,0,0,0,0), //circum
  //     effect_config2->index_circle_center, //
  //     50,  // tail_size
  //     5, // center region
  //     true
  //   );
  // }


  // // widden
  // if(effect_config2->value >= 1){

  //   // Draw near sky
  //   Draw_DesiredColour_LinearGradientMirrored2(
  //     RgbcctColor(20,5,0,0,0), //center
  //     RgbcctColor(0,0,0,0,0), //circum
  //     effect_config2->index_circle_center, //
  //     25,  // tail_size
  //     10, // center region
  //     true
  //   );
  // }

  // if(effect_config2->value >= 2){
  //   // Draw near sky
  //   Draw_DesiredColour_LinearGradientMirrored2(
  //     RgbcctColor(100,20,0,0,0), //center
  //     RgbcctColor(0,0,0,0,0), //circum
  //     effect_config2->index_circle_center, //
  //     30,  // tail_size
  //     2, // center region
  //     true
  //   );
  // }

  // // widden
  // if(effect_config2->value >= 3){

  //   // Draw near sky
  //   Draw_DesiredColour_LinearGradientMirrored2(
  //     RgbcctColor(0,50,50,0,0), //center
  //     RgbcctColor(0,100,75,0,0), //circum
  //     effect_config2->index_circle_center, //
  //     60,  // tail_size
  //     10, // center region
  //     true
  //   );
  // }


  // if(effect_config2->value >= 4){
  //   // Draw sun bloom
  //   Draw_DesiredColour_LinearGradientMirrored2(
  //     RgbcctColor(200,100,0,0,0), //center
  //     RgbcctColor(50, 44, 3,     0,0), //circum
  //     effect_config2->index_circle_center, //D_RGB255_ORANGE_DEEP
  //     50,
  //     5, 
  //     true
  //   );
  // }

  // if(effect_config2->value == 4){
  //   // Draw sun bloom
  //   Remove_DesiredColour_LinearGradientMirrored(
  //     RgbcctColor(0,0,  50,       0,0), //center
  //     RgbcctColor(0, 0, 10,     0,0), //circum
  //     effect_config2->index_circle_center, //D_RGB255_ORANGE_DEEP
  //     50,
  //     5, 
  //     true
  //   );
  // }


  
  if(effect_config2->value++ > 6){ effect_config2->value = 0; }

        // pixel_radius_as_number = 10;
        // pixel_position_center = effect_config2->index_circle_center;
        // pixel_position_circum = effect_config2->index_circle_center-pixel_radius_as_number;
        // for(uint16_t index=0; index<pixel_radius_as_number; index++ ){
        //   // Create new index
        //   index_generate = effect_config2->index_circle_center-index;              
        //   //Only write into colour buffer if valid index. Otherwise, consider this an "overscan"
        //   if((index_generate>=0)&&(index_generate<100)){
        //     //Draw half, larger index side
        //     progress = mSupport::mapfloat(index_generate,
        //                                     pixel_position_center,pixel_position_circum,
        //                                     0,1
        //                                   );
        //     animation_colours[index_generate].DesiredColour = RgbcctColor::LinearBlend(
        //                                                                                 colour_circle_center,
        //                                                                                 colour_circle_circumference, 
        //                                                                                 //RgbColor(255,0,0), RgbColor(0,255,0)
        //                                                                                 progress);
        //   } // if test

        // }





// //use signed value to hold index_adjusted
// int32_t index_generate = 0;
// uint16_t pixel_position_center, pixel_position_circum, pixel_radius_as_number;

//         pixel_radius_as_number = 10;
//         pixel_position_center = effect_config2->index_circle_center;
//         pixel_position_circum = effect_config2->index_circle_center+pixel_radius_as_number;
//         for(uint16_t index=0; index<pixel_radius_as_number; index++ ){
//           // Create new index
//           index_generate = effect_config2->index_circle_center+index;   
          
//           //Draw half, larger index side
//           progress = mSupport::mapfloat(index_generate,
//                                           pixel_position_center,pixel_position_circum,
//                                           0,1
//                                         );
//           colour_draw = RgbcctColor::LinearBlend(
//                                         colour_circle_center,
//                                         colour_circle_circumference, 
//                                         //RgbColor(255,0,0), RgbColor(0,255,0)
//                                         progress);



//           //Only write into colour buffer if valid index. Otherwise, consider this an "overscan"
//           if((index_generate>=0)&&(index_generate<100)){
//             animation_colours[index_generate].DesiredColour = colour_draw;
//           } // if test

//         }

//         pixel_radius_as_number = 10;
//         pixel_position_center = effect_config2->index_circle_center;
//         pixel_position_circum = effect_config2->index_circle_center-pixel_radius_as_number;
//         for(uint16_t index=0; index<pixel_radius_as_number; index++ ){
//           // Create new index
//           index_generate = effect_config2->index_circle_center-index;              
//           //Only write into colour buffer if valid index. Otherwise, consider this an "overscan"
//           if((index_generate>=0)&&(index_generate<100)){
//             //Draw half, larger index side
//             progress = mSupport::mapfloat(index_generate,
//                                             pixel_position_center,pixel_position_circum,
//                                             0,1
//                                           );
//             animation_colours[index_generate].DesiredColour = RgbcctColor::LinearBlend(
//                                                                                         colour_circle_center,
//                                                                                         colour_circle_circumference, 
//                                                                                         //RgbColor(255,0,0), RgbColor(0,255,0)
//                                                                                         progress);
//           } // if test

//         }





          // index_generate = effect_config2->index_circle_center-index+10;
          // start_pixel_position = index_generate;
          // end_pixel_position = index_generate+10;
          // if((index_generate>=0)&&(index_generate<100)){
          //   //Draw half, samller index side
          //   progress = mSupport::mapfloat(index_generate,start_pixel_position,end_pixel_position,0,1);
          //   animation_colours[index_generate].DesiredColour = RgbcctColor::LinearBlend(RgbColor(255,0,255), RgbColor(0,0,255), progress);
              
              
          //     // colour_circle_circumference, colour_circle_center, progress);
          // }// out of range

        // }


        // randomly blank most of them out again
        // uint8_t colour_or_black = 0;
        // uint16_t pixels_to_update = pCONT_iLight->animation.transition.pixels_to_update_as_number;
        // // pCONT_iLight->GetPixelsToUpdateAsNumberFromPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage);
        // Serial.printf("pixels_to_clear=%d\n\r",pixels_to_update);
        // for(uint16_t index=0; index<pixels_to_update; index++ ){
        //   random_pixel_index = random(0,pCONT_iLight->settings.light_size_count);
        //   colour_or_black = random(0,1);

        //   // // Check if pixels is already on, if so, make dark
        //   // if(pCONT_iLight->RgbColorto32bit(GetPixelColor(random_pixel_index))>0){
        //   //   animation_colours[random_pixel_index].DesiredColour = RgbColor(0,0,0);
        //   // }else
        //   // // Previously off, pick new state
        //   // {
        //     if(colour_or_black){ // 80% in favour of black/off
        //       animation_colours[random_pixel_index].DesiredColour = RgbColor(0,255,0);
        //     }else{
        //       uint8_t desired_pixel = random(0,pixels-1);
        //       colour_random = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel);  
        //       // if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
        //       //   colour_random = ApplyBrightnesstoDesiredColour(colour_random,pCONT_iLight->getBriRGB());
        //       // }
        //       animation_colours[random_pixel_index].DesiredColour =  colour_random;//RgbColor(0,0,255);//
        //    }
        //   // }
        // }

pCONT_iLight->animation.palette.id = 10;

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
    

  // // Call the animator to blend from previous to new
  // this->setAnimFunctionCallback(
  //   [this](const AnimationParam& param){
  //     this->AnimationProcess_BlendStartingToDesiredColour_SOLID_COLOUR(param);
  //   }
  // );


  //     // UpdateDesiredColourFromPaletteSelected();


  //     UpdateStartingColourWithGetPixel();



    //   flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
    // // NO 
    // }break;
    // case EFFECTS_REGION_ANIMATE_ID: //shift along
      AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
      this->setAnimFunctionCallback([this](const AnimationParam& param){ 
        this->AnimationProcess_Generic_RGBCCT_Single_Colour_All(param); });
      
  //     flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID;
  //     break;
  // }

}

#endif