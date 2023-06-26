#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR


#ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL


void mAnimatorLight::SubTask_Manual_SetPixel(){

  // AddLog(LOG_LEVEL_TEST, PSTR("SubTask_Manual_SetPixel"));

  _segments[0].transition.rate_ms = 1000;
  _segments[0].transition.time_ms = 0;

  // SubTask_Flasher_Animate_Function__Slow_Glow();

  
  // for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++)
  // {
  //   SetPixelColor(pixel, animation_colours[pixel].DesiredColour);
  // }

  
  // _segment_runtimes[0].animation_has_anim_callback = false; // When no animation callback is needed


  // //run function once

  // pCONT_iLight->animation.mode_id = pCONT_iLight->ANIMATION_MODE_NONE_ID;

  
// {
//   "AnimationMode": 5,
//   "Strip":{"ClearTo":[0,0,0,0,0]},
//   "ManualSetPixelToScene":[0,30,65,104,146,189,232,275,317,359,401,442,481,519,555,588,618,647,675,700,725,748,770,792,830,849,866,884,900,914,928,941,951,961,972,983,991,995,999,1004,1010,1016,1021]
// }


//   "ManualSetPixelToScene":
/**
 * 
 
 [
   0,30,   //31
   31,65,  //44 <<34!
   66,104  //39
   105,146 //42
   147,189 //43
   190,232 //43
   233,275 //43
   276,317 //42
   318,359 //42
   360,401 //42
   402,442 //41
   443,481 //39
   482,519 //38
   520,555 //36
   556,588 //33
   589,618 //30
   619,647 //29
   648,675 //28
   676,700 //25
   701,725 //25
   726,748 //23
   749,770 //22
   771,792 //22
   793,830 //38   ** 38/2   = 20/18   21-17       pred. 20
                //missed row     ?                pred. 18
   831,849 //19
   850,866 //17
   867,884 //18
   885,900 //16
   901,914 //14
   915,928 //14
   929,941 //13
   942,951 //10
   952,961 //10
   962,972 //11
   973,983 //11
   984,991 //8
   992,995 //4
   996,999 //4
   1000,1004 //5
   1005,1010 //6
   1011,1016 //6
   1017,1021 //5

   //push into array, then have them added during init
   uint16_t single_row_count_array[] = {31,44,39,42,43,43,43,42,42,42,41,39,38,36,33,30,29,28,25,25,23,22,22,20,18,19,17,18,16,14,14,13,10,10,11,11,8,4,4,5,6,6,5};  //sum=1031 r19
   uint16_t single_row_count_array[] = {31,44,39,42,43,43,43,42,42,42,41,39,38,36,33,30,29,28,25,25,23,22,22,20,18,19,17,18,16,14,14,13,10,10,11,11,8,4,4,5,6,6,5,16,3};  //sum=1031 r19  predicted 16 so top 3 are different





*/

}


uint8_t mAnimatorLight::ConstructJSON_Manual_SetPixel(uint8_t json_level){

  JsonBuilderI->Start();

  AddLog(LOG_LEVEL_TEST, PSTR("ConstructJSON_Manual_SetPixel"));

    // // JsonBuilderI->Level_Start(D_JSON_MIXER);
    // JsonBuilderI->Add(D_JSON_ENABLED, mixer.flags.Enabled);
    // JsonBuilderI->Add("decounter", mixer.group[mixer.running_id].time_on_secs_decounter);







    

    // JsonBuilderI->Level_Start(D_JSON_MODE);
    //   JsonBuilderI->Add(D_JSON_RUNNING_ID, mixer.running_id);
    //   JsonBuilderI->Array_AddArray(D_JSON_TIME_ON_SECS, mixer.mode.time_on_secs, EFFECTS_FUNCTION_MIXER_MAX);
    //   JsonBuilderI->Add(D_JSON_TIME_ON_SECS "_Active", mixer.mode.time_on_secs_active);
    //   JsonBuilderI->Add("run_time_duration_scaler_as_percentage", mixer.run_time_duration_scaler_as_percentage);
    // JsonBuilderI->Level_End();
    
    // JsonBuilderI->Level_Start(D_JSON_GROUP);
    //   JsonBuilderI->Add(D_JSON_TRANSITION "_" D_JSON_TIME, mixer.running_id);

        
      uint16_t pixels_added = 0;

      JsonBuilderI->Array_Start("PixelsOn");
      for(uint16_t i=0;i<_segments[0].length();i++){ 

        if(i>20){ break; } // causes crash, really I need to make so JBI cant add more than its length

        if(pCONT_iLight->RgbColorto32bit(GetPixelColor(i))){
          JsonBuilderI->Add(i); 
          // if(pixels_added++>50){
          //   break; // to stop overflow
          // }
        }      
      }
      JsonBuilderI->Array_End();  



    //   JsonBuilderI->Array_Start("ifenabled_forced_brightness_level_percentage");
    //   for(uint8_t i=0;i<EFFECTS_FUNCTION_MIXER_MAX;i++){ JsonBuilderI->Add(mixer.group[i].ifenabled_forced_brightness_level_percentage); }
    //   JsonBuilderI->Array_End(); 
    //   JsonBuilderI->Array_Start("pixels_to_update_as_percentage");
    //   for(uint8_t i=0;i<EFFECTS_FUNCTION_MIXER_MAX;i++){ JsonBuilderI->Add(mixer.group[i].pixels_to_update_as_percentage); }
    //   JsonBuilderI->Array_End();
    //   JsonBuilderI->Array_Start("transition_time");
    //   for(uint8_t i=0;i<EFFECTS_FUNCTION_MIXER_MAX;i++){ JsonBuilderI->Add(mixer.group[i].transition.time); }
    //   JsonBuilderI->Array_End();
    //   JsonBuilderI->Array_Start("transition_rate");
    //   for(uint8_t i=0;i<EFFECTS_FUNCTION_MIXER_MAX;i++){ JsonBuilderI->Add(mixer.group[i].transition.rate); }
    //   JsonBuilderI->Array_End();



    // JsonBuilderI->Level_End();

    // JsonBuilderI->Add("EnabledMixerCount", mixer.enabled_mixer_count);

    // // JsonBuilderI->Level_End();

    // JsonBuilderI->Add("update_as_percentage", pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val);






//   root["enabled"] = mixer.flags.Enabled;
//   root["running_id"] = mixer.running_id;
//   root["seconds"] = mixer.group[mixer.running_id].time_on_secs_decounter;//round((mixer.tSavedMillisToChangeAt-millis())/1000);
  
//   root["time_scaler"] = mixer.time_scaler;

//   JsonObject timesobj = root.createNestedObject("times");

//     // char time_on[40]; memset(time_on,0,sizeof(time_on));
//     // sprintf(time_on, "%02d:%02d:%02d",mixer.mode.times.flashing_starttime.hour,
//     //                                   mixer.mode.times.flashing_starttime.minute,
//     //                                   mixer.mode.times.flashing_starttime.second);
//     // timesobj["flashing_starttime"] = time_on;

//     // char time_off[40]; memset(time_off,0,sizeof(time_off));
//     // sprintf(time_off, "%02d:%02d:%02d",mixer.mode.times.flashing_endtime.hour,
//     //                                   mixer.mode.times.flashing_endtime.minute,
//     //                                   mixer.mode.times.flashing_endtime.second);
//     // timesobj["flashing_endtime"] = time_off;

//     //timesobj["skip_restricted_by_time_isactive"] = mixer.skip_restricted_by_time_isactive;

//     timesobj["running_friendly_name_ctr"] = mixer.running_friendly_name_ctr;

  return JsonBuilderI->End();

}


#endif // ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL

#endif //USE_MODULE_LIGHTS_ANIMATOR




