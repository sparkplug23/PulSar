#include "../../mAnimatorLight.h"



#ifdef ENABLE_PIXEL_FUNCTION_EFFECTS



/**************************************************************************************************************************************************************
 * @brief  Solid_Colour_Based_On_Sun_Elevation_02
 * @note   From -10 to noon, CCT will range from yellow to daywhite
 * @note   From -5 to dusk, blue will go from 0 to max_brightness 
 * 
 * @note   Gloabl brightness will be manual, or controlled indirectly eg via mqtt
 * *************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_02()
{
 
  // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_02"));

  // AddLog(LOG_LEVEL_TEST,PSTR("pCONT_iLight->animation.palette.id=%d"),pCONT_iLight->animation.palette.id);

/**
 * 
 * 
 * */


  // pCONT_iLight->CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(10);

  pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;

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
  
  // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "sun_elevation=%d"), (int)sun_elevation);

  // struct COLOUR_REGIONS{
  //   uint8_t rgb_
  // }

  // uint8_t

  /**
   * Generate the rgb channels
   * */
  // Add yellow to red hue, with max to lowering rgbbrightness
  if(sun_elevation>10)
  {
    pCONT_iLight->rgbcct_controller.setRGB(0,255,0);    

  }else
  if(
    (sun_elevation<10)&&
    (sun_elevation>-5)
    )
  {
    HsbColor hsb = HsbColor(RgbColor(0));
    hsb.H = pCONT_iLight->HueN2F(mapvalue(sun_elevation,10,-5,35,0)); // yellow to red
    hsb.S = 1;

    /**
     * Calculate brightness gradient
     * */
    hsb.B = mapvalue(sun_elevation,10,-5,0.0f,1.0f);

    uint8_t brightness_255 = map(sun_elevation,10,-5,0,255);
    pCONT_iLight->rgbcct_controller.setBrightnessRGB255(brightness_255);


    pCONT_iLight->rgbcct_controller.setHsbColor(hsb);

    // uint8_t blue =  map(sun_elevation,-50,-5,255,0);

    // pCONT_iLight->rgbcct_controller.setRGB(0,0,blue);

    // AddLog(LOG_LEVEL_TEST,PSTR("sun_elevation=%d, blue=%d"), (int)sun_elevation, blue);

    // AddLog(LOG_LEVEL_INFO, PSTR("elevation=%d, cct_temp=%d %d"),(int)sun_elevation, elev_perc, cct_val);

    // uint8_t brightness_255 = map(sun_elevation,-50,-5,255,0);

    // pCONT_iLight->rgbcct_controller.setBrightnessRGB255(brightness_255);

  }else
  // Just below hoeizon, sky blue to night blue
  // "blue" remains bright
 if(
    (sun_elevation<=-5)&&
    (sun_elevation>=-10)
    )
  {
    // HsbColor hsb = HsbColor(RgbColor(0));
    // hsb.H = map(sun_elevation,-50,10,255,0)

    HsbColor hsb = HsbColor(RgbColor(0));
    hsb.H =  pCONT_iLight->HueN2F(mapvalue(sun_elevation,-5,-10,180,240)); // yellow to red
    hsb.S = 1;

    /**
     * Calculate brightness gradient
     * */
    hsb.B = mapvalue(sun_elevation,-5,-10,1.0f,1.0f);

    uint8_t brightness_255 = map(sun_elevation,-5,-10,255,200);
    pCONT_iLight->rgbcct_controller.setBrightnessRGB255(brightness_255);


    pCONT_iLight->rgbcct_controller.setHsbColor(hsb);

    // uint8_t blue =  map(sun_elevation,-50,-5,255,0);

    // pCONT_iLight->rgbcct_controller.setRGB(0,0,blue);

    // AddLog(LOG_LEVEL_TEST,PSTR("sun_elevation=%d, blue=%d"), (int)sun_elevation, blue);

    // AddLog(LOG_LEVEL_INFO, PSTR("elevation=%d, cct_temp=%d %d"),(int)sun_elevation, elev_perc, cct_val);

    // uint8_t brightness_255 = map(sun_elevation,-50,-5,255,0);

    // pCONT_iLight->rgbcct_controller.setBrightnessRGB255(brightness_255);

  }else
  // "night" blue, slowing fade to low brightness
 if(
    (sun_elevation>=-50)&&
    (sun_elevation<=-5)
    )
  {
    HsbColor hsb = HsbColor(RgbColor(0));
    hsb.H = pCONT_iLight->HueN2F(240); // yellow to red
    hsb.S = 1;

    /**
     * Calculate brightness gradient
     * */
    hsb.B = mapvalue(sun_elevation,-5,-50,1.0f,0.0f);


    pCONT_iLight->rgbcct_controller.setHsbColor(hsb);

    uint8_t brightness_255 = map(sun_elevation,-5,-50,255,1);
    pCONT_iLight->rgbcct_controller.setBrightnessRGB255(brightness_255);


    // uint8_t blue =  map(sun_elevation,-50,-5,255,0);

    // pCONT_iLight->rgbcct_controller.setRGB(0,0,blue);

    // AddLog(LOG_LEVEL_TEST,PSTR("sun_elevation=%d, blue=%d"), (int)sun_elevation, blue);

    // AddLog(LOG_LEVEL_INFO, PSTR("elevation=%d, cct_temp=%d %d"),(int)sun_elevation, elev_perc, cct_val);

    // uint8_t brightness_255 = map(sun_elevation,-50,-5,255,0);

    // pCONT_iLight->rgbcct_controller.setBrightnessRGB255(brightness_255);

  }
  
  // else
  // if((sun_elevation>-50)&&(sun_elevation<10))
  // {
  //   // HsbColor hsb = HsbColor(RgbColor(0));
  //   // hsb.H = map(sun_elevation,-50,10,255,0)

  //   uint8_t blue =  map(sun_elevation,-50,10,255,0);

  //   pCONT_iLight->rgbcct_controller.setRGB(0,0,blue);

  //   // AddLog(LOG_LEVEL_INFO, PSTR("elevation=%d, cct_temp=%d %d"),(int)sun_elevation, elev_perc, cct_val);




  //   // uint8_t brightness_255 = map(sun_elevation,-50,10,255,0);

  //   // pCONT_iLight->rgbcct_controller.setBrightnessRGB255(brightness_255);

  // }
  // else
  // if(
  //   (sun_elevation>5)
  //   )
  // {
  //   pCONT_iLight->rgbcct_controller.setRGB(1,0,0);    
  // }
  else
  {
    //unknown, therfore turn off
    pCONT_iLight->rgbcct_controller.setRGB(0,0,0);
  }



  /**
   * Generate the CCT channels
   * */
  if(
    (sun_elevation<-5)
    )
  {

    // Set the colour temp
    pCONT_iLight->rgbcct_controller.setCCT(pCONT_iLight->get_CTRangeMin()); // set as daywhite

    uint8_t brightness_255 = 0;//map(sun_elevation,-10,25,0,255);
    pCONT_iLight->rgbcct_controller.setBrightnessCCT255(brightness_255);

  }else
  if(
    (sun_elevation>=10)&&
    (sun_elevation<=20)
    )
  {

    // Convert elevation into percentage
    uint8_t elev_perc = map(sun_elevation,10,20,0,100);
    // Convert percentage into cct
    uint16_t cct_val = mapvalue(elev_perc, 0,100, pCONT_iLight->get_CTRangeMin(),pCONT_iLight->get_CTRangeMax());
    // Set the colour temp
    pCONT_iLight->rgbcct_controller.setCCT(cct_val);

    
    AddLog(LOG_LEVEL_TEST,PSTR("sun_elevation=%d, elev_perc=%d, cct_val=%d, cct=%d"), (int)sun_elevation, elev_perc, cct_val, pCONT_iLight->rgbcct_controller.getCCT());

    uint8_t brightness_255 = map(sun_elevation,10,20,100,255);
    pCONT_iLight->rgbcct_controller.setBrightnessCCT255(brightness_255);

  }else
  if(
    (sun_elevation>=-5)&&
    (sun_elevation<10)
    )
  {

    // Convert elevation into percentage
    uint8_t elev_perc = 0;//map(sun_elevation,-5,10,0,100);
    // Convert percentage into cct
    uint16_t cct_val = mapvalue(elev_perc, 0,100, pCONT_iLight->get_CTRangeMin(),pCONT_iLight->get_CTRangeMax());
    // Set the colour temp
    pCONT_iLight->rgbcct_controller.setCCT(cct_val);

    
    AddLog(LOG_LEVEL_TEST,PSTR("sun_elevation=%d, elev_perc=%d, cct_val=%d, cct=%d"), (int)sun_elevation, elev_perc, cct_val, pCONT_iLight->rgbcct_controller.getCCT());

    uint8_t brightness_255 = map(sun_elevation,-5,10,0,100);
    pCONT_iLight->rgbcct_controller.setBrightnessCCT255(brightness_255);

  }else
  if(
    (sun_elevation>20)
    )
  {

    // Set the colour temp
    pCONT_iLight->rgbcct_controller.setCCT(pCONT_iLight->get_CTRangeMax()); // set as daywhite

    // uint8_t brightness_255 = map(sun_elevation,-10,25,0,255);
    // pCONT_iLight->rgbcct_controller.setBrightnessCCT255(brightness_255);

  }
  




//  pCONT_iLight->rgbcct_controller.setCCT(0); // set as daywhite

  
  pCONT_iLight->animation.flags.fForceUpdate = true;




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