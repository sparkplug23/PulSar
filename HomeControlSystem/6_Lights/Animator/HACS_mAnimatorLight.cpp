#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

/******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
*** Animation Effect:   HACS (Legacy)   ***************************************************************************************************************************************************************************
**  @note:    HACS animation, to be phased out into Segments **************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************/

#ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT



void mAnimatorLight::EveryLoop(){

  //something smarter for delays
  // pCONT_set->Settings.enable_sleep = (pCONT_iLight->animation.transition.rate_ms<pCONT_set->Settings.sleep) ? 0 : 1;

  if(pCONT_iLight->animation.flags.fEnable_Animation){
  // while(blocking_force_animate_to_complete){
    if (pCONT_iLight->animator_controller->IsAnimating()){ //Serial.print("~a"); // the normal lop just needs these two to run the active animations_fadeall
      pCONT_iLight->animator_controller->UpdateAnimations();
  // DEBUG_PRINT_FUNCTION_NAME_TEST;
  // if(stripbus->CanShow()){
  //     pCONT_iLight->ShowInterface();
  //   }

// stripbus->IsDirty
    
      // StripUpdate();
  // DEBUG_PRINT_FUNCTION_NAME_TEST;
      if(!pCONT_iLight->animation.flags.fRunning){   
        #ifdef ENABLE_LOG_LEVEL_DEBUG
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "Animation Started"));
        #endif
      }
      pCONT_iLight->animation.flags.fRunning = true; 
      fPixelsUpdated = true;
      pCONT_set->Settings.enable_sleep = false;    //Make this a function, pause sleep during animations
      //AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "~a"));
    }else{
      if(pCONT_iLight->animation.flags.fRunning){ // Was running
        #ifdef ENABLE_LOG_LEVEL_DEBUG
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "Animation Finished")); 
        #endif
        fAnyLEDsOnOffCount = 0;
        for(int i=0;i<pCONT_iLight->settings.light_size_count;i++){ 
          if(GetPixelColor(i)!=0){ fAnyLEDsOnOffCount++; }
        }          
      }
      pCONT_iLight->animation.flags.fRunning = false;
      pCONT_set->Settings.enable_sleep = true;
      if(blocking_force_animate_to_complete){ //if it was running
        #ifdef ENABLE_LOG_LEVEL_DEBUG
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "Animation blocking_force_animate_to_complete"));
        #endif
        blocking_force_animate_to_complete = false;
      }
    }
  //   if(mTime::TimeReached(&tSavedBlocking_ForceAnimateToComplete,5000)) {blocking_force_animate_to_complete = false; break; }//exit blocking lo op
  // }
  }//pCONT_iLight->animation.flags.fEnable_Animation


  if(stripbus->IsDirty()){
    if(stripbus->CanShow()){ 
      StripUpdate();
      // pCONT_iLight->ShowInterface();
    }
  }



  // #ifndef ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT
  // // scene.colour.B = pCONT_iLight->animation.brightness; // pCONT_iLight->animation.brightness is master
  // // move to have tasmota way to update brightness stored
  // #endif //ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT

  // control relay power
  //(pCONT_iLight->light_power_state)
    if(pCONT_iLight->light_power_state!=pCONT_iLight->light_power_Saved){
      pCONT_iLight->light_power_Saved = pCONT_iLight->light_power_state;
      // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "pCONT_iLight->light_power_Saved"));
      //pCONT->mry->CommandSet_Relay_Power(0,pCONT_iLight->light_power_state);
    }

    //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "file%sline%d"),__FILE__,__LINE__);
    DEBUG_LINE;

  if(mTime::TimeReached(&tSavedCheckAnyLEDsOnOff,1000)){
    fAnyLEDsOnOffCount = 0;
    for(int i=0;i<pCONT_iLight->settings.light_size_count;i++){
      #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "fAnyLEDsOnOffCount %d"),fAnyLEDsOnOffCount);
      #endif
      if(GetPixelColor(i)!=0){
        fAnyLEDsOnOffCount++;
      }
    }
    if(fAnyLEDsOnOffCount>0){
      #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "fAnyLEDsOnOffCount %d"),fAnyLEDsOnOffCount);
      #endif
    }
    // #ifdef USE_MODULE_DRIVERS_RELAY
    //   //pCONT->mry->CommandSet_Relay_Power(0,fAnyLEDsOnOffCount>0?1:0);
    // #endif
  }
  DEBUG_LINE;
// 

}






void mAnimatorLight::init_flasher_settings(){

  flashersettings.flags.movement_direction = 0;
  


// A "wipe" or "sine wave" that applies a saturation change across the string. So either, from all christmas coloured to slowly blending from the top of the tree to the bottom, to white (or another solid colour), or apply the saturation change in a loop/rotation. 


  
}



/**
 * @brief: Single colour, replicated for all output values (all pixels for ws2811 or pwm for h801)
 * */
void mAnimatorLight::AnimationProcess_Generic_RGBCCT_Single_Colour_All(const AnimationParam& param)
{   

  RgbcctColor output_colour = RgbcctColor::LinearBlend(
    animation_colours_rgbcct.StartingColor,
    animation_colours_rgbcct.DesiredColour,
    param.progress);

  for(int ii=0;ii<pCONT_iLight->settings.light_size_count;ii++){
    SetPixelColor(ii,output_colour);
  }

}

/**
 * @brief: Ws2811 style output, single pixels
 * */
void mAnimatorLight::AnimationProcess_Generic_AnimationColour_LinearBlend(const AnimationParam& param)
{    
  for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++)
  {
    RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
        animation_colours[pixel].StartingColor,
        animation_colours[pixel].DesiredColour,
        param.progress);    
    SetPixelColor(pixel, updatedColor);
  }
}













void mAnimatorLight::SubTask_Effects_PhaseOut(){

  #ifdef ENABLE_PIXEL_FUNCTION_MIXER
  if(mixer.flags.Enabled){
   SubTask_Flasher_Animate_Mixer();
  }
  #endif // ENABLE_PIXEL_FUNCTION_MIXER

  SubTask_Flasher_Animate();

} // SubTask_Effects_PhaseOut


/**
 * 
 * Change this to be indexed to recall from _segment
 * */
void mAnimatorLight::SubTask_Flasher_Animate(){
  
  /**
   * Timer (seconds) to update the EFFECTS_REGION_COLOUR_SELECT_ID when otherwise remains static
   * */
  SubTask_Flasher_Animate_Parameter_Check_Update_Timer_Change_Colour_Region();
  /**
   * Timer (seconds) to randomise the rates of change. Methods include (random between range, linear map, exponential (longer slow), exponential (longer fast))
   * */  
  SubTask_Flasher_Animate_Parameter_Check_Update_Timer_Transition_Rate();
  /**
   * Timer (seconds) to randomise the transition time of change. Methods include (random between range, linear map, exponential (longer slow), exponential (longer fast))
   * */    
  SubTask_Flasher_Animate_Parameter_Check_Update_Timer_Transition_Time();


  if((mTime::TimeReached(&flashersettings.tSaved.Update,pCONT_iLight->animation.transition.rate_ms))||(pCONT_iLight->animation.flags.fForceUpdate))
  {

    if(pCONT_iLight->animation.flags.fForceUpdate){ pCONT_iLight->animation.flags.fForceUpdate=false;
      flashersettings.tSaved.Update = millis();
    }

    // #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
    // char buffer[100];
    // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "flashersettings.function=%d %s"),flashersettings.function,GetFlasherFunctionNamebyID(flashersettings.function, buffer));
    // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "flashersettings.function=%d"),flashersettings.function);
    //#endif
    // #ifdef DEVICE_RGBFIREPLACE_TESTER
    //     flashersettings.function = EFFECTS_FUNCTION_SLOW_GLOW_ID;
    // #endif

    switch(flashersettings.function){
      default:
      /**
       * Basic methods for christmas kept as legacy, until segments is fully working
       * */
      case EFFECTS_FUNCTION_SOLID_COLOUR_ID:
        SubTask_Flasher_Animate_Function__Solid_Static_Single_Colour();
      break;
      case EFFECTS_FUNCTION_STATIC_PALETTE_ID:
        SubTask_Flasher_Animate_Function__Static_Glow();
      break;
      case EFFECTS_FUNCTION_SLOW_GLOW_ID:
        SubTask_Flasher_Animate_Function__Slow_Glow();
      break;
      case EFFECTS_FUNCTION_SEQUENTIAL_ID:
        SubTask_Flasher_Animate_Function__Sequential();
      break;
      case EFFECTS_FUNCTION_STEP_THROUGH_PALETTE_ID:
        SubTask_Flasher_Animate_Function__Step_Through_Palette();
      break;
      case EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01_ID:
        SubTask_Flasher_Animate_Function__SunPositions_Elevation_Only_RGBCCT_Palette_Indexed_Positions_01();
      break;
      case EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_CCT_TEMPERATURE_01_ID:
        SubTask_Flasher_Animate_Function__SunPositions_Elevation_Only_Controlled_CCT_Temperature_01();
      break;
      case EFFECTS_FUNCTION_FIREPLACE_1D_01_ID:
        SubTask_Flasher_Animate_Function__FirePlace_1D_01();
      break;
      case EFFECTS_FUNCTION_TESTER_ID:
        SubTask_Flasher_Animate_Function_Tester();
      break; 
    } //end SWITCH

    // Configure animator to show output
    StartAnimation_AsAnimUpdateMemberFunction();

  }//end if update reached

   pCONT_iLight->animation.flags.animator_first_run = false;

} //end flasher_animate


/**
 * Timer (seconds) to update the EFFECTS_REGION_COLOUR_SELECT_ID when otherwise remains static
 * */
void mAnimatorLight::SubTask_Flasher_Animate_Parameter_Check_Update_Timer_Change_Colour_Region(){

  if(flashersettings.update_colour_region.refresh_secs || flashersettings.update_colour_region.refresh_decounter_secs){ // >0 is active
    if(mTime::TimeReached(&flashersettings.update_colour_region.tSaved, 1000)){
      AddLog(LOG_LEVEL_TEST, PSTR("rate_colours_secs=%d, rate_decounter=%d"),flashersettings.update_colour_region.refresh_secs, flashersettings.update_colour_region.refresh_decounter_secs);
      if(flashersettings.update_colour_region.refresh_decounter_secs == 1){
        flashersettings.update_colour_region.refresh_decounter_secs = flashersettings.update_colour_region.refresh_secs; // reset
        flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID;
        flashersettings.flags.force_finish_flasher_pair_once = 1;
      }else
      if(flashersettings.update_colour_region.refresh_decounter_secs > 1){
        flashersettings.update_colour_region.refresh_decounter_secs--;
      }
      //
      else{
        flashersettings.update_colour_region.refresh_decounter_secs = flashersettings.update_colour_region.refresh_secs; // reset
      }
    }
  }

}

 /**
   * Timer (seconds) to randomise the rates of change. Methods include (random between range, linear map, exponential (longer slow), exponential (longer fast))
   * */  
  void mAnimatorLight::SubTask_Flasher_Animate_Parameter_Check_Update_Timer_Transition_Rate(){
  // Generate randomness to transitions if flag is set
  // if(flashersettings.flags.enable_random_rate){
  //   if(mTime::TimeReached(&flashersettings.random_transitions.tSavedNewSpeedUpdate,flashersettings.random_transitions.rate_ms)){
  //     flashersettings.random_transitions.rate_ms = (random(flashersettings.random_transitions.lower_secs,flashersettings.random_transitions.upper_secs)*1000);
  //     pCONT_iLight->animation.transition.rate_ms = flashersettings.random_transitions.array[flashersettings.random_transitions.array_index++]*10;
  //     if(flashersettings.random_transitions.array_index>=flashersettings.random_transitions.array_index_length) flashersettings.random_transitions.array_index=0;
  //     pCONT_iLight->animation.transition.time_ms.val = pCONT_iLight->animation.transition.rate_ms/4; //75% of time spent on desired colour
  //   }
  // }
  }

  /**
   * Timer (seconds) to randomise the transition time of change. Methods include (random between range, linear map, exponential (longer slow), exponential (longer fast))
   * */ 
  void mAnimatorLight::SubTask_Flasher_Animate_Parameter_Check_Update_Timer_Transition_Time(){
  // if(mTime::TimeReached(&flashersettings.function_seq.tSavedNewSpeedUpdate,(random(3,10)*1000))){
  //   uint8_t values[8] = {4,8,10,17,20,17,10,8};
  //   pCONT_iLight->animation.transition.rate_ms = values[flashersettings.function_seq.rate_index++]*50;
  //   if(flashersettings.function_seq.rate_index>8) flashersettings.function_seq.rate_index=0;
  //   pCONT_iLight->animation.transition.time_ms = pCONT_iLight->animation.transition.rate_ms/4; //75% of time spent on desired colour
  // }
  }


/**
 * This can be gotten from animator if start does not need to be saved
 * 
 * 3 per pixel, 1000 leds would need 3000 RGB bytes
 * 
 * */
void mAnimatorLight::UpdateStartingColourWithGetPixel(){

  for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){

    // Test fix, if desired does not match getcolour, then use ...


    animation_colours[pixel].StartingColor = GetPixelColor(pixel);
  }

}

void mAnimatorLight::UpdateDesiredColourWithGetPixel(){

  for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){

    // Test fix, if desired does not match getcolour, then use ...


    animation_colours[pixel].DesiredColour = GetPixelColor(pixel);
  }

}

void mAnimatorLight::UpdateStartingColourWithSingleColour(RgbcctColor colour){

  for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    animation_colours[pixel].StartingColor = colour;
  }

}

void mAnimatorLight::UpdateDesiredColourWithSingleColour(RgbcctColor colour){

  for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    animation_colours[pixel].DesiredColour = colour;
  }

}


void mAnimatorLight::RotateDesiredColour(uint8_t pixels_amount_to_shift, uint8_t direction){

//pixels_amount_to_shift loop this many times

  if(direction){ // direction==1 move right ie AWAY from start

    // Shift colours (rotate)
    RgbcctColor colourlast = animation_colours[pCONT_iLight->settings.light_size_count-1].DesiredColour;//desired_colour[0];
    // Shift towards first pixel
    for(ledout.index=pCONT_iLight->settings.light_size_count-1; //last to first
        ledout.index>0;
        ledout.index--
      ){ //+1?
      // move backwards
      animation_colours[ledout.index].DesiredColour = animation_colours[ledout.index-1].DesiredColour;
    }
    // Put first pixel on the end (wrap around)
    animation_colours[0].DesiredColour = colourlast;

  }else{

    // Shift colours (rotate)
    RgbcctColor colourfirst = animation_colours[0].DesiredColour;//desired_colour[0];
    // Shift towards first pixel
    for(ledout.index=0;ledout.index<pCONT_iLight->settings.light_size_count;ledout.index++){ 
      animation_colours[ledout.index].DesiredColour = animation_colours[ledout.index+1].DesiredColour;
    }
    // Put first pixel on the end (wrap around)
    animation_colours[ledout.index-1].DesiredColour = colourfirst;
  }

}



// // Update struct that shows overview, always sends
uint8_t mAnimatorLight::ConstructJSON_Flasher(uint8_t json_level){

#ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
return 0;
#endif

  char buffer[100];

  JsonBuilderI->Start();

  JBI->Add("flashermillis",millis()-flashersettings.tSaved.Update);

//   // root[D_JSON_ONOFF] = pCONT_iLight->light_power_state ? "ON" : "OFF";
  JBI->Add(PM_JSON_MODE, pCONT_iLight->GetAnimationModeName(buffer, sizeof(buffer)));
  JBI->Add(PM_JSON_FUNCTION, GetFlasherFunctionName(buffer, sizeof(buffer)));
  // root["region"] = GetFlasherRegionName();
  // root[D_JSON_COLOUR_PALETTE] = GetPaletteFriendlyName();
  // root[D_JSON_BRIGHTNESS_PERCENTAGE] = pCONT_iLight->animation.brightness*100;
  // root[D_JSON_BRIGHTNESS] = pCONT_iLight->animation.brightness;

//   // JsonObject transitionobj = root.createNestedObject(D_JSON_TRANSITION);
//   //   transitionobj[D_JSON_METHOD] = GetTransitionMethodName();
//   //   transitionobj[D_JSON_TIME] = mSupport::safeDivideInt(pCONT_iLight->animation.transition.time_ms,1000);
//   //   transitionobj[D_JSON_TIME_MS] = pCONT_iLight->animation.transition.time_ms;
//   //   transitionobj[D_JSON_RATE] = mSupport::safeDivideInt(pCONT_iLight->animation.transition.rate_ms,1000);
//   //   transitionobj[D_JSON_RATE_MS] = pCONT_iLight->animation.transition.rate_ms;
//   //   transitionobj[D_JSON_FUNCTION] = GetFlasherFunctionName();

//   JsonObject seq_obj = root.createNestedObject("sequential");
//     seq_obj["rate_index"] = flashersettings.function_seq.rate_index;

// JsonBuilderI->Level_Start("slow_glow");
//   JsonBuilderI->Add("rate_index", flashersettings.function_slo_glo.rate_index);
// JsonBuilderI->Level_End();



//   // Flags and states that are used during one transition and reset when completed
//   // JsonObject overridesobj = root.createNestedObject("transition_overrides"); 
//   //   overridesobj["fRefreshAllPixels"] = pCONT_iLight->animation_override.fRefreshAllPixels;
  
//   // root["ledout.length"] = ledout.length;

//   data_buffer.payload.len = measureJson(root)+1;
//   serializeJson(doc,data_buffer.payload.ctr);

//   return (data_buffer.payload.len>3?1:0);
  return JsonBuilderI->End();
}



#endif// ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT


#endif //USE_MODULE_LIGHTS_ANIMATOR




