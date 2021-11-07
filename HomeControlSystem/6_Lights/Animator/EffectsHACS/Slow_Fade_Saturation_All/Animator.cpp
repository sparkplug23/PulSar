#include "../../mAnimatorLight.h"

#ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT




void mAnimatorLight::SubTask_Flasher_Animate_Function_Slow_Fade_Saturation_All(){
      
//       case EFFECTS_FUNCTION_SLOW_FADE_SATURATION_ALL_ID:
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
//             float random_saturation = direction ? 1 : ((float)random(0,100)/(float)100);

//             float adjusted_brightness = random_saturation;// mSupport::mapfloat(random_saturation,
//             if(random_saturation<0.6){
//               adjusted_brightness = mSupport::mapfloat(random_saturation,
//                                                                     0,  0.6,
//                                                                     0.4,0.6);
//             }
                        
//             for(ledout.index=0;ledout.index<pCONT_iLight->settings.light_size_count;ledout.index++){ 
//               desired_colour[ledout.index].S = random_saturation; 
//               desired_colour[ledout.index].B = adjusted_brightness;//random_saturation<0.5?pCONT_iLight->animation.brightness*0.5:pCONT_iLight->animation.brightness; //test, pair "whiter" less bright (maybe /2)  
//             }            
//             // Change pCONT_iLight->animation speed
//             //if(mTime::TimeReached(&flashersettings.function_seq.tSavedNewSpeedUpdate,(random(3,10)*100))){
//               uint16_t values[8] = {7,7,9,9,7,7,4,4}; //off,on,off
//               pCONT_iLight->animation.transition.rate_ms = values[flashersettings.function_seq.rate_index++]*1000;
//               if(flashersettings.function_seq.rate_index>=8) flashersettings.function_seq.rate_index=0;
//               // holds colour more
//               //pCONT_iLight->animation.transition.time_ms = direction ? (pCONT_iLight->animation.transition.rate_ms/4) : (pCONT_iLight->animation.transition.rate_ms); // brightness shift takes 100% of the time (no colour shift)
//               pCONT_iLight->animation.transition.time_ms = random(0,1) ? (pCONT_iLight->animation.transition.rate_ms/4) : (pCONT_iLight->animation.transition.rate_ms.va;); // brightness shift takes 100% of the time (no colour shift)
//             //}
//           break;
//         }
//       break;
//       //flasher_start_white_to_Colour: 
//         // method that will start all leds on one colour (white?) and slowly add colours in with fade in
//       //flasher_change_saturation_between 0 and 100, "fade of saturation", "fade of brightness"

}




#endif
