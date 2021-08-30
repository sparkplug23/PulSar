#include "../../mAnimatorLight.h"

#ifdef ENABLE_PIXEL_FUNCTION_EFFECTS




void mAnimatorLight::SubTask_Flasher_Animate_Function_Slow_Fade_Brightness_All(){
     
      
//       case EFFECTS_FUNCTION_SLOW_FADE_BRIGHTNESS_ALL_ID:
//         switch(flashersettings.region){
//           case EFFECTS_REGION_COLOUR_SELECT_ID: //set colours
//             AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
//             UpdateDesiredColourFromPaletteSelected();
//             flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
//           break;
//           case EFFECTS_REGION_ANIMATE_ID: //shift along
//             AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//             // Change brightness from 0 to 100% (rotate)
//             uint8_t direction = flashersettings.function_seq.rate_index%2;//(flashersettings.function_slow_fade.direction^=1);
//             for(ledout.index=0;ledout.index<pCONT_iLight->settings.light_size_count;ledout.index++){ desired_colour[ledout.index].B = direction; }            
//             // Change pCONT_iLight->animation speed
//             if(mTime::TimeReached(&flashersettings.function_seq.tSavedNewSpeedUpdate,(random(3,10)*100))){
//               uint8_t values[8] = {1000,1000,2000,2000,6000,6000,3000,3000}; //off,on,off
//               pCONT_iLight->animation.transition.rate_ms = values[flashersettings.function_seq.rate_index++]*50;
//               if(flashersettings.function_seq.rate_index>=8) flashersettings.function_seq.rate_index=0;
//               pCONT_iLight->animation.transition.time_ms = pCONT_iLight->animation.transition.rate_ms;///4; // brightness shift takes 100% of the time (no colour shift)
//             }
//           break;
//         }
//       break;
}



#endif
