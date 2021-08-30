#include "../../mAnimatorLight.h"


#ifdef ENABLE_PIXEL_FUNCTION_EFFECTS


/****
 * Changes pixels randomly to new colour, with slow blending
 * Requires new colour calculation each call
 */
void mAnimatorLight::SubTask_Flasher_Animate_Function__Palette_Step_Through_Palette(){
  // So colour region does not need to change each loop to prevent colour crushing
  pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
  // Pick new colours

  switch(flashersettings.region){
    case EFFECTS_REGION_COLOUR_SELECT_ID:{ //set colours
      AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));

      pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
      // UpdateDesiredColourFromPaletteSelected();
      // UpdateStartingColourWithGetPixel();
      flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
    // NO 

        mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
        
      int16_t pixel_position = -2;
      uint8_t pixels_in_map = mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr);
      
      
       AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "pixels_in_map= %d"),pixels_in_map);


      RgbTypeColor colour;
      
      desired_pixel = shared_flasher_parameters.indexes.active;
      uint8_t pixels_map_upper_limit = shared_flasher_parameters.indexes.active+1;
      uint8_t pixels_map_lower_limit = shared_flasher_parameters.indexes.active;

      uint8_t index_1, index_2;

      uint8_t counter = 0;

          
      //if last pixel, then include it and the first, else include it and the next
      if(shared_flasher_parameters.indexes.active == pixels_in_map-1){ //wrap wround
        index_1 = 0;
        index_2 = shared_flasher_parameters.indexes.active;
        counter = 0;
      }else{
        index_1 = shared_flasher_parameters.indexes.active;
        index_2 = shared_flasher_parameters.indexes.active+1;
        counter = 1;

      }

      shared_flasher_parameters.indexes.counter ^= 1;

      
      AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "shared_flasher_parameters = %d/%d/%d"),shared_flasher_parameters.indexes.active,index_1,index_2);
  
      
    

      for(uint16_t index=0;index<pCONT_iLight->settings.light_size_count;index++){


          if(counter^=1){
            //using %2 means alternate leds replace the previous position, not the current
            desired_pixel = shared_flasher_parameters.indexes.counter ? index_2 : index_1;
          }else{
            desired_pixel = shared_flasher_parameters.indexes.counter ? index_1 : index_2;
          }
        
        colour = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel,&pixel_position);

        if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
          colour = ApplyBrightnesstoRgbcctColour(colour,pCONT_iLight->rgbcct_controller.getBrightnessRGB255());
        }



        animation_colours[index].DesiredColour = colour;
         
        //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "desired_pixel= %d/%d/%d"),pixels_map_lower_limit,desired_pixel,pixels_map_upper_limit);
  
        // if(++desired_pixel>pixels_map_upper_limit){
        //   desired_pixel = pixels_map_lower_limit;
        // }

      } 

      //progress active index by 1 or reset
      AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "shared_flasher_parameters.indexes.active=%d"),shared_flasher_parameters.indexes.active);
      if(++shared_flasher_parameters.indexes.active>pixels_in_map-1){
        shared_flasher_parameters.indexes.active=0;
      }
      
  
    }break;
    case EFFECTS_REGION_ANIMATE_ID: //shift along
      AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));

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

      flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID;


      break;
  }



            // desired_colour[ledout.pattern[ledout.index]] = colour; 
            // AddLog(LOG_LEVEL_TEST, PSTR("colour=%d,%d,%d"),
            // colour.R,
            // colour.G,
            // colour.B);
            
            // animation_colours[ledout.pattern[ledout.index]].DesiredColour = ApplyBrightnesstoDesiredColour(colour,pCONT_iLight->getBriRGB());
              
          //   if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
          //     animation_colours[ledout.pattern[ledout.index]].DesiredColour = ApplyBrightnesstoDesiredColour(animation_colours[ledout.pattern[ledout.index]].DesiredColour,pCONT_iLight->getBriRGB());
          //   }

          //   //   AddLog(LOG_LEVEL_TEST, PSTR("colou2=%d,%d,%d"),
          //   // animation_colours[ledout.pattern[ledout.index]].DesiredColour.R,
          //   // animation_colours[ledout.pattern[ledout.index]].DesiredColour.G,
          //   // animation_colours[ledout.pattern[ledout.index]].DesiredColour.B);

            
          //   #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
          //   // AddLog(LOG_LEVEL_DEBUG_MORE_MORE,PSTR(D_LOG_NEO "colour[p%d:d%d] = %d,%d,%d %d pp%d"),
          //   //   ledout.pattern[ledout.index],desired_pixel,
          //   //   pCONT_iLight->HueF2N(desired_colour[ledout.pattern[ledout.index]].H),pCONT_iLight->SatF2N(desired_colour[ledout.pattern[ledout.index]].S),pCONT_iLight->BrtF2N(desired_colour[ledout.pattern[ledout.index]].B),
          //   //   mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr),pixel_position
          //   // );
          //   #endif

          //   if(++desired_pixel>=mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr)){desired_pixel=0;}
          // } //end for




  
}


// simple blend function
void mAnimatorLight::AnimationProcess_Slow_Glow_Partial_Palette_Step_Through(const AnimationParam& param)
{    

  for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
        animation_colours[pixel].StartingColor,
        animation_colours[pixel].DesiredColour,
        param.progress);    
    SetPixelColor(pixel, updatedColor);
  } // END for

}
#endif