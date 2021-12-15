#include "../mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

/******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
*** Animation Effect:   HACS (Legacy)   ***************************************************************************************************************************************************************************
**  @note:    HACS animation, to be phased out into Segments **************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************/

#ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT


void mAnimatorLight::TurnLEDsOff(){
  //stripbus->ClearTo(0);
  for(uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    SetPixelColor(pixel, 0);
  }
  pCONT_iLight->ShowInterface();
}

void mAnimatorLight::ApplyBrightnesstoDesiredColour(uint8_t brightness){

  for(uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++ ){
    animation_colours[pixel].DesiredColour.R = mapvalue(animation_colours[pixel].DesiredColour.R, 0, 255, 0, brightness);
    animation_colours[pixel].DesiredColour.G = mapvalue(animation_colours[pixel].DesiredColour.G, 0, 255, 0, brightness);
    animation_colours[pixel].DesiredColour.B = mapvalue(animation_colours[pixel].DesiredColour.B, 0, 255, 0, brightness);
    // should be white channel here depending on type!
  }

}

/**
 * Duplicate parameter, needs merging with above in long term if it is really the same
 * */
uint16_t mAnimatorLight::SetLEDOutAmountByPercentage(uint8_t percentage){

  strip_size_requiring_update = mapvalue(percentage, 0,100, 0,pCONT_iLight->settings.light_size_count);

// AddLog(LOG_LEVEL_TEST, PSTR(DEBUG_INSERT_PAGE_BREAK "SetLEDOutAmountByPercentage = %d"),strip_size_requiring_update);

  return strip_size_requiring_update; // also return the count

}


/**
 * Begin at "StartingColor" at 0% then return to "DesiredColor" at 100%
 * */
void mAnimatorLight::StartAnimation_AsAnimUpdateMemberFunction(){ 
  
  #ifdef ENABLE_LOG_LEVEL_DEBUG
  //AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::StartAnimationAsBlendFromStartingColorToDesiredColor"));
  #endif

  #ifndef ENABLE_DEVFEATURE_FLICKERING_TEST3

  pCONT_iLight->runtime.animation_changed_millis = millis();

  uint16_t time_tmp = 0;
  // switch(pCONT_iLight->animation.transition.method_id){
  //   default: 
  //   case TRANSITION_METHOD_INSTANT_ID: time_tmp = 0;

  //   Serial.println("TRANSITION_METHOD_INSTANT_ID");
    
    
    
  //    break;
    // case TRANSITION_METHOD_BLEND_ID:   
    time_tmp = 
#ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
  pCONT_iLight->animation
#else
  pCONT_lAni->_segments[0]
#endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
.transition.time_ms; 
  //   break;
  // }



  
#ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT

  // Overwriting single pCONT_iLight->animation methods, set, then clear
  if(pCONT_iLight->animation_override.time_ms){
    time_tmp = pCONT_iLight->animation_override.time_ms;
    pCONT_iLight->animation_override.time_ms = 0;//reset overwrite
  }

#else
  // Overwriting single pCONT_iLight->animation methods, set, then clear
  if(segment_animation_override.time_ms){
    time_tmp = segment_animation_override.time_ms;
    segment_animation_override.time_ms = 0;//reset overwrite
  }

#endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT



  //clear forced once only flags
  if(
#ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
  pCONT_iLight->animation
#else
  pCONT_lAni->_segments[0]
#endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
.flags.NewAnimationRequiringCompleteRefresh){
    
#ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
  pCONT_iLight->animation
#else
  pCONT_lAni->_segments[0]
#endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
.flags.NewAnimationRequiringCompleteRefresh = false;    
  }
  
    // Serial.printf("TRANSITION_METHOD_INSTANT_ID = %d\n\r",time_tmp);

  if(time_tmp>0){
    if(NEO_ANIMATION_TIMEBASE == NEO_CENTISECONDS){
      time_tmp /= 1000;// ms to seconds
    }
  }

  // Note: Loading of startingcolor and desiredcolor are done elsewhere

  


  // DEBUG_PRINT_FUNCTION_NAME_TEST;
  if(anim_function_callback){
    pCONT_iLight->animator_controller->StartAnimation(0, time_tmp, anim_function_callback );
  }

  #else
    pCONT_iLight->animator_controller->StartAnimation(0, 900, 
      [this](const AnimationParam& param){
        this->AnimationProcess_Tester(param);
      }
    );

  #endif // ENABLE_DEVFEATURE_FLICKERING_TEST3

  // DEBUG_PRINT_FUNCTION_NAME_TEST;

} //end function


mAnimatorLight& mAnimatorLight::setAnimFunctionCallback(ANIM_FUNCTION_SIGNATURE) {
  this->anim_function_callback = anim_function_callback;
  return *this;
}

// // Adds colour with boundary checks
// void mAnimatorLight::DesiredColourWrite_Safe(RgbcctColor colour, uint16_t index){

//   // animation_colours[index_generate].DesiredColour


// }


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
       * Basic methods, until segments is fully working
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


#ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT



#ifdef USE_DEVFEATURE_FUNCTION_UPGRADE_CORE_TRANSITIONMETHODS

/**
 * @brief: For animations that require specialised indexing options, this function generates them
 * @note : Instead of using indexing from 0 to STRIP_LENGTH, this function will change that pattern through various methods
 * 
 * */
void mAnimatorLight::RefreshLEDIndexPattern()
{

  //Serial.printf("animation.transition.order_id=%d\n\r",pCONT_iLight->animation.transition.order_id);

  // Generate lighting pattern
  switch(pCONT_iLight->animation.transition.order_id){

    /**
     * indexing is just incremental: 0 -> SEGMENT_LENGTH
     * */
    case TRANSITION_ORDER_INORDER_ID:

      // So other parts of code understand how many are changing
      SetLEDOutAmountByPercentage(100);

      for(ledout.index=0;ledout.index<strip_size_requiring_update;ledout.index++){    // strip_size_requiring_update TBC SEGMENT_LENGTH
        ledout.pattern[ledout.index] = ledout.index; 
      }

      #ifdef DEBUG_ANIMATIONS_REFRESHPIXELINDEXING
      // #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog(LOG_LEVEL_DEBUG, PSTR("Transition = Inorder, segment length = %d"), strip_size_requiring_update);
      // #endif
      #endif

    break;
    case TRANSITION_ORDER_RANDOM_ID:
      switch(transition_order_random_type){

        default: // forced method for now
        
        case TRANSITION_ORDER_RANDOM_METHOD_REPEATING_WITH_PERCENTAGE:{ // test case, which includes percentage count to update

          /**
           * Get pixels that indexing should be calculated for
           * */
          uint16_t pixel_count_to_change = 10;
  

// ledout.pattern[

//           uint16_t pixels_to_update_count = map(
//             pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val, 0,100, 0, pCONT_iLight->settings.light_size_count
//           );


      // SetLEDOutAmountByPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val);

      // AddLog(LOG_LEVEL_TEST, PSTR("strip_size_requiring_update=%d"),strip_size_requiring_update);

            // Pick random indexes, from the entire pixel count, but only fill up pattern as needed
            // for(ledout.index=0;ledout.index<strip_size_requiring_update;ledout.index++){ 
            //   ledout.pattern[ledout.index] = random(0,pCONT_iLight->settings.light_size_count); 
            // }

            // Here, only X pattern count will change, and I should only update by this amount

            

            for(ledout.index=0;ledout.index<pixel_count_to_change;ledout.index++){ 
              ledout.pattern[ledout.index] = random(0,pCONT_iLight->settings.light_size_count); 
            }




// pixels_to_update_count

//           for(uint16_t index=0;index<pixels_to_update_count; index++){ //for as many as I want to update

//           for(ledout.index=0;ledout.index<ledout.length;ledout.index++){ 


//             ledout.pattern[ledout.index] = random(0,ledout.length); 

// strip_size_requiring_update
//           }




      }break;


    //     case TRANSITION_ORDER_RANDOM_METHOD_REPEATING:     //  Serial.println("TRANSITION_ORDER_INORDER_ID1");
         
          
    //       #ifdef ENABLE_DEVFEATURE_STDSHUFFLE_PIXEL_RANDOM
    //         for(ledout.index=0;ledout.index<ledout.length;ledout.index++){ 
    //           ledout.pattern[ledout.index] = ledout.index;//random(0,ledout.length); 
    //         }
    //         std::shuffle(ledout.pattern.begin(),ledout.pattern.end(),std::default_random_engine(analogRead(0)));
    //       #else 
    //         for(ledout.index=0;ledout.index<ledout.length;ledout.index++){ 
    //           ledout.pattern[ledout.index] = random(0,ledout.length); 
    //         }
    //       #endif // ENABLE_DEVFEATURE_STDSHUFFLE_PIXEL_RANDOM

    //       SetLEDOutAmountByPercentage(100);


    //     break;
    //     case TRANSITION_ORDER_RANDOM_METHOD_NONREPEATING: default:  {     //  Serial.println("TRANSITION_ORDER_INORDER_ID2");
    //       // Generate a full list inorder
    //       uint16_t pick_list[ledout.length];
    //       uint16_t pick_list_remaining = ledout.length;
    //       uint16_t pick_list_index = 0;     
    //       uint16_t pick_list_val = 0;     
    //       for(ledout.index=0;ledout.index<ledout.length;ledout.index++){ 
    //         pick_list[ledout.index] = ledout.index;
    //       }
    //       // Pick a index, fill output, remove option
    //       ledout.index = 0; //reset output indexing

    //       while(pick_list_remaining)
    //       {
    //         pick_list_index = random(0,pick_list_remaining);
    //         pick_list_val = pick_list[pick_list_index];
    //        // AddLog(LOG_LEVEL_TEST,PSTR("pick_list_remaining=%d [%d:%d]"),pick_list_remaining,pick_list_index,pick_list_val);

    //         ledout.pattern[ledout.index++] = pick_list_val;

    //         // Remove pick_list_index
    //         for(int ii=0;ii<pick_list_remaining-1;ii++){
    //           //check if we have matched
    //           if(ii >= pick_list_index){         
    //             pick_list[ii] = pick_list[ii+1]; //shift by 1 index
    //             // Serial.printf("%d, ",pick_list[ii+1]);
    //           }
    //         }
    //     // Serial.println();

    //       pick_list_remaining--;
    //       }

    //       SetLEDOutAmountByPercentage(100);

    //     }
    //     break;
    //   }
    // break;
    // // case TRANSITION_ORDER_FIXED_ID:
    // //   // pattern is contained within colour map
    // // break;
    // case TRANSITION_ORDER_CENTRE_OUT_ID:{
    //   // uint16_t middle_index = length_local/2;
    //   //0,1,2,3,4,5,4,3,2,1,0
    //   // for(ledout.index=0;ledout.index<length_local;ledout.index++){ 
    //   //   index_pattern[ledout.index] = ledout.index; 
    //   // }
    // }
    // break;
  }

} //end function "RefreshLEDIndexPattern"


// Name:UpdateDesiredColourFromPaletteSelected
// Task: 
/**
 * This needs redoing, with a flag, to enable how I want the palette presented, and not how its encoded
 * "palette_pattern"
 * Gradient (either needs to get from palette, or equally generate it based on palette element count)
 * Single   (gets each colour in the palette, with ability to ignore indexs if they are present)
 * */
void mAnimatorLight::UpdateDesiredColourFromPaletteSelected(void){

  // Update pointer of struct
  mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);

  // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "UpdateDesiredColourFromPaletteSelected fMapIDs_Type %d"),mPaletteI->palettelist.ptr->flags.fMapIDs_Type);// \"%s\""),GetPaletteFriendlyName());
  // AddLog(LOG_LEVEL_DEBUG_MORE
  
  /**
   * Handle the retrieval of colours from palettes depending on the palette encoding type
   * */
  switch(mPaletteI->palettelist.ptr->flags.fMapIDs_Type){
    // default:
    //   #ifdef ENABLE_LOG_LEVEL_DEBUG
    //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "default"));
    //   #endif
    // case MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID:

    default:
    case MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID:
    case MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID:
    case MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX_ID: ///to be tested
    {
    //get colour above

      //apply to positiion below

     // SetLEDOutAmountByPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val);

      //what is this?
      // does this only run if above did not? ie this is default?

      //AddLog(LOG_LEVEL_TEST, PSTR("strip_size_requiring_update=%d"),strip_size_requiring_update);
      
      // Other types with random led patterns, or,
      
      switch(pCONT_iLight->animation.transition.order_id){
        case TRANSITION_ORDER_RANDOM_ID:{

          // new transition, so force full update
          if(pCONT_iLight->animation.flags.NewAnimationRequiringCompleteRefresh){
            strip_size_requiring_update = STRIP_SIZE_MAX;
          }
        

          RefreshLEDIndexPattern();


          int16_t pixel_position = -2;
          for(ledout.index=0;ledout.index<strip_size_requiring_update;ledout.index++){

            // For random, desired pixel from map will also be random
            desired_pixel = random(0,mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr));
            
            RgbTypeColor colour = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel,&pixel_position);

            // desired_colour[ledout.pattern[ledout.index]] = colour; 
            // AddLog(LOG_LEVEL_TEST, PSTR(DEBUG_INSERT_PAGE_BREAK "colour=%d,%d,%d"),
            // colour.R,
            // colour.G,
            // colour.B);

            animation_colours[ledout.pattern[ledout.index]].DesiredColour = colour;

            #ifndef ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS
            if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
              animation_colours[ledout.pattern[ledout.index]].DesiredColour = ApplyBrightnesstoDesiredColourWithGamma(animation_colours[ledout.pattern[ledout.index]].DesiredColour,pCONT_iLight->getBriRGB_Global());
            }
            #endif // ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS

            // #ifdef ENABLE_DEBUG_MODULE_LIGHTS_ADDRESSABLE
            // AddLog(LOG_LEVEL_INFO, PSTR("colour=%d,%d,%d brt_set=%d"),
            //   animation_colours[ledout.pattern[ledout.index]].DesiredColour.R,
            //   animation_colours[ledout.pattern[ledout.index]].DesiredColour.G,
            //   animation_colours[ledout.pattern[ledout.index]].DesiredColour.B,
            //   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation
            // );
            // #endif 
          } //end for

        }break;
        case TRANSITION_ORDER_INORDER_ID:{
          // new transition, so force full update
          if(pCONT_iLight->animation.flags.NewAnimationRequiringCompleteRefresh){
            strip_size_requiring_update = STRIP_SIZE_MAX;
          }
          RefreshLEDIndexPattern();
          int16_t pixel_position = -2;
          desired_pixel=0;
          for(ledout.index=0;ledout.index<strip_size_requiring_update;ledout.index++){
            RgbcctColor colour = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel,&pixel_position);

            // RgbTypeColor colour = colourcct;
            // desired_colour[ledout.pattern[ledout.index]] = colour; 
            // AddLog(LOG_LEVEL_TEST, PSTR("colour=%d,%d,%d"),
            // colour.R,
            // colour.G,
            // colour.B); 
            
            // AddLog(LOG_LEVEL_TEST, PSTR( "%d colour=%d,%d,%d"),desired_pixel, // DEBUG_INSERT_PAGE_BREAK
            // colour.R,
            // colour.G,
            // colour.B);

            // animation_colours[ledout.pattern[ledout.index]].DesiredColour = ApplyBrightnesstoDesiredColour(colour,pCONT_iLight->getBriRGB());
              // RgbColor colourbefore = animation_colours[ledout.pattern[ledout.index]].DesiredColour;
            animation_colours[ledout.pattern[ledout.index]].DesiredColour = colour;
           #ifndef ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS
            if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
              animation_colours[ledout.pattern[ledout.index]].DesiredColour = ApplyBrightnesstoDesiredColourWithGamma(animation_colours[ledout.pattern[ledout.index]].DesiredColour, pCONT_iLight->getBriRGB_Global());
            }
            #endif //             ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS
// RgbColor colourafter = animation_colours[ledout.pattern[ledout.index]].DesiredColour;
// if(ledout.index<5){
//             RgbColor colour2 = colour;
//             AddLog(LOG_LEVEL_TEST, PSTR( "%d colourbefore colour=%d,%d,%d     %d,%d,%d"), // DEBUG_INSERT_PAGE_BREAK
//             ledout.index,colourbefore.R,colourbefore.G, colourbefore.B,
//             colourafter.R,
//             colourafter.G,
//             colourafter.B);
// }
            //   AddLog(LOG_LEVEL_TEST, PSTR("colou2=%d,%d,%d"),
            // animation_colours[ledout.pattern[ledout.index]].DesiredColour.R,
            // animation_colours[ledout.pattern[ledout.index]].DesiredColour.G,
            // animation_colours[ledout.pattern[ledout.index]].DesiredColour.B);

            
            #ifdef ENABLE_LOG_LEVEL_DEBUG
            // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "colour[p%d:d%d] = %d,%d,%d %d pp%d"),
            //   ledout.pattern[ledout.index],desired_pixel,
            //   pCONT_iLight->HueF2N(desired_colour[ledout.pattern[ledout.index]].H),pCONT_iLight->SatF2N(desired_colour[ledout.pattern[ledout.index]].S),pCONT_iLight->BrtF2N(desired_colour[ledout.pattern[ledout.index]].B),
            //   mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr),pixel_position
            // );
            #endif
            // AddLog(LOG_LEVEL_INFO, PSTR("colour=%d,%d,%d brt_set=%d"),
            //   animation_colours[ledout.pattern[ledout.index]].DesiredColour.R,
            //   animation_colours[ledout.pattern[ledout.index]].DesiredColour.G,
            //   animation_colours[ledout.pattern[ledout.index]].DesiredColour.B,
            //   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation
            // );

            if(++desired_pixel>=mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr)){desired_pixel=0;}
          } //end for
        }break;
        // case TRANSITION_ORDER_FIXED_ID:

        //   int16_t pixel_position = -2;
        //   //move across all encoded values
        //     // HsbColor colour = GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel,&pixel_position);

        //     // if(pixel_position>=0){
        //     //   desired_colour[pixel_position] = colour;
        //     // }else{
        //     //   desired_colour[ledout.pattern[ledout.index]] = colour;
        //     // }
            
        //     // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "colour[p%d:d%d] = %d,%d,%d %d pp%d"),
        //     //   ledout.pattern[ledout.index],desired_pixel,
        //     //   HueF2N(desired_colour[ledout.pattern[ledout.index]].H),SatF2N(desired_colour[ledout.pattern[ledout.index]].S),BrtF2N(desired_colour[ledout.pattern[ledout.index]].B),
        //     //   mPaletteI->palettelist.ptr->active_pixels_in_map,pixel_position
        //     // );    

        // break;
      }//end switch


    }break;
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_ID:
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_ID:
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_AND_SETALL_ID:
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_AND_SETALL_ID:{

      // Get active pixels in map
      uint16_t active_pixels_in_map = mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr); //width 2

    #ifdef ENABLE_LOG_LEVEL_INFO
      // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "active_pixels_in_map=%d"),active_pixels_in_map);
    #endif// ENABLE_LOG_LEVEL_INFO

      // Move across map
      int16_t pixel_position = -2;
      for(uint16_t desired_pixel=0;desired_pixel<active_pixels_in_map;desired_pixel++){
        RgbcctColor colour = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel,&pixel_position);
        
        #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
        //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "dp%d, pp%d, %d,%d %d"),desired_pixel, pixel_position,pCONT_iLight->HueF2N(colour.H),pCONT_iLight->SatF2N(colour.S),pCONT_iLight->BrtF2N(colour.B));
        #endif
        
        if(pixel_position>=0){
          // Set output to this "many" colour
          if(pixel_position == 255){
            for(uint16_t temp=0;temp<ledout.length;temp++){ 
              animation_colours[temp].DesiredColour = ApplyBrightnesstoRgbcctColour(colour,pCONT_iLight->getBriRGB_Global());
            }            
            #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
            // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "set ALL %d,%d %d"),pCONT_iLight->HueF2N(colour.H),pCONT_iLight->SatF2N(colour.S),pCONT_iLight->BrtF2N(colour.B));
            #endif
          }else{
            colour.R = 1;colour.R = 2;colour.R = 3;
            // Serial.println("colour.R = 1;colour.R = 2;colour.R = 3;");
            animation_colours[pixel_position].DesiredColour = ApplyBrightnesstoRgbcctColour(colour,pCONT_iLight->getBriRGB_Global());
            #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
            //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "set %d %d,%d %d"), pixel_position,pCONT_iLight->HueF2N(colour.H),pCONT_iLight->SatF2N(colour.S),pCONT_iLight->BrtF2N(colour.B));
            #endif
          }
        }else{          
          #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
          AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "return; //end early %d"),pixel_position);
          #endif
        }

      }
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "return; //end early %d"),pixel_position);
    #endif// ENABLE_LOG_LEVEL_INFO
      break;
      // return;
    }
    break;
    case MAPIDS_TYPE_RGBCCTCOLOUR_WITHINDEX_GRADIENT_ID:
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_GRADIENT_ID:
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID:{

      //#ifdef ENABLE_LOG_LEVEL_DEBUG
      // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID"));
      //#endif

      uint16_t start_pixel = 0;
      uint16_t end_pixel = 100;
      RgbcctColor start_colour = RgbcctColor(0);
      RgbcctColor end_colour = RgbcctColor(0);
      uint16_t desired_pixel = 0;
      int16_t start_pixel_position = -1, end_pixel_position = -1;
      uint8_t pixels_in_map = mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr);

      //#ifdef ENABLE_LOG_LEVEL_DEBUG
      // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "pixels_in_map %d"),pixels_in_map);
      //#endif


      // Add flag to enable ignoring gradient, and filling as normal palette (here?)

      for(uint8_t grad_pair_index=0;grad_pair_index<pixels_in_map-1;grad_pair_index++){
        
        start_colour = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel,  &start_pixel_position);
        end_colour   = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel+1,&end_pixel_position);

        
      // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "%d start_pixel_position %d"),grad_pair_index,start_pixel_position);
      // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "end_pixel_position %d"),end_pixel_position);

#ifndef ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS
        start_colour = ApplyBrightnesstoRgbcctColour(start_colour,pCONT_iLight->getBriRGB_Global());
        end_colour   = ApplyBrightnesstoRgbcctColour(end_colour,pCONT_iLight->getBriRGB_Global());
#endif // ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS

        // #ifdef ENABLE_LOG_LEVEL_DEBUG
        //  AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "s%d,%d %d %d"),pCONT_iLight->HueF2N(start_colour.H),pCONT_iLight->SatF2N(start_colour.S),pCONT_iLight->BrtF2N(start_colour.B),start_pixel_position);
        //  AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "e%d,%d %d %d"),HueF2N(end_colour.H),SatF2N(end_colour.S),BrtF2N(end_colour.B),end_pixel_position);
        // #endif

        switch(mPaletteI->palettelist.ptr->flags.fIndexs_Type){
          case INDEX_TYPE_DIRECT: break; //remains the same
          case INDEX_TYPE_SCALED_255: 
            start_pixel_position = map(start_pixel_position,0,255,0,ledout.length);
            end_pixel_position   = map(end_pixel_position,0,255,0,ledout.length);
            // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "255 %d %d"),start_pixel_position,end_pixel_position);
          break;
          case INDEX_TYPE_SCALED_100: 
            start_pixel_position = map(start_pixel_position,0,100,0,ledout.length);
            end_pixel_position   = map(end_pixel_position,0,100,0,ledout.length);          
          break;
        }


    
      // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "%d start_pixel_position %d"),grad_pair_index,start_pixel_position);
      // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "end_pixel_position %d"),end_pixel_position);


        float progress = 0;
        for(ledout.index=start_pixel_position;ledout.index<end_pixel_position;ledout.index++){
          progress = mSupport::mapfloat(ledout.index,start_pixel_position,end_pixel_position,0,1);
          animation_colours[ledout.index].DesiredColour = RgbcctColor::LinearBlend(start_colour, end_colour, progress);
        }
        desired_pixel++;
      }

      // Colour applied to entire DesiredColour, leaving now
      break;
      // return; //succcesfully handled

    }break;
    
  }

  #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "DONE UpdateDesiredColourFromPaletteSelected fMapIDs_Type "));
  #endif  // LOG_LEVEL_DEBUG_MORE

  //  AddLog(LOG_LEVEL_DEBUG, PSTR("colour=%d,%d,%d"),
  //             animation_colours[0].DesiredColour.R,
  //             animation_colours[0].DesiredColour.G,
  //             animation_colours[0].DesiredColour.B
  //           );

  
  // Handle brightness levels == if not set, use default
  //if(!mPaletteI->palettelist.ptr->fOverride_animation_brightness){
  // ApplyBrightnesstoDesiredColour(pCONT_iLight->getBriRGB());
  //}

} //end function UpdateDesiredColourFromPaletteSelected();



#else


/**
 * Creating a second file, to do the same thing, to replace this with testing.
 * It should resolve the "pixels to update each time issue"
 * */
void mAnimatorLight::RefreshLEDIndexPattern()
{

  //Serial.printf("animation.transition.order_id=%d\n\r",pCONT_iLight->animation.transition.order_id);

  // Generate lighting pattern
  switch(
#ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
  pCONT_iLight->animation
#else
  pCONT_lAni->_segments[0]
#endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
.transition.order_id){
    case TRANSITION_ORDER_INORDER_ID:

      SetLEDOutAmountByPercentage(100);

      for(ledout.index=0;ledout.index<ledout.length;ledout.index++){ 
        ledout.pattern[ledout.index] = ledout.index; 
      }


      //   Serial.printf("TRANSITION_ORDER_INORDER_ID %d \n\r", segment_index);
      // for(int jj=0;jj<ledout.length;jj++){ Serial.printf("%d, ",ledout.pattern[jj]); } Serial.println();
// AddLog_Array(LOG_LEVEL_TEST,&ledout.pattern[0],10);

    break;
    case TRANSITION_ORDER_RANDOM_ID:
      switch(transition_order_random_type){
        
        case TRANSITION_ORDER_RANDOM_METHOD_REPEATING_WITH_PERCENTAGE:{ // test case, which includes percentage count to update

// ledout.pattern[

//           uint16_t pixels_to_update_count = map(
//             pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val, 0,100, 0, pCONT_iLight->settings.light_size_count
//           );


      // SetLEDOutAmountByPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val);

      // AddLog(LOG_LEVEL_TEST, PSTR("strip_size_requiring_update=%d"),strip_size_requiring_update);

            // Pick random indexes, from the entire pixel count, but only fill up pattern as needed
            for(ledout.index=0;ledout.index<strip_size_requiring_update;ledout.index++){ 
              ledout.pattern[ledout.index] = random(0,pCONT_iLight->settings.light_size_count); 
            }




// pixels_to_update_count

//           for(uint16_t index=0;index<pixels_to_update_count; index++){ //for as many as I want to update

//           for(ledout.index=0;ledout.index<ledout.length;ledout.index++){ 


//             ledout.pattern[ledout.index] = random(0,ledout.length); 

// strip_size_requiring_update
//           }




      }break;
        case TRANSITION_ORDER_RANDOM_METHOD_REPEATING:     //  Serial.println("TRANSITION_ORDER_INORDER_ID1");
         
          
          #ifdef ENABLE_DEVFEATURE_STDSHUFFLE_PIXEL_RANDOM
            for(ledout.index=0;ledout.index<ledout.length;ledout.index++){ 
              ledout.pattern[ledout.index] = ledout.index;//random(0,ledout.length); 
            }
            std::shuffle(ledout.pattern.begin(),ledout.pattern.end(),std::default_random_engine(analogRead(0)));
          #else 
            for(ledout.index=0;ledout.index<ledout.length;ledout.index++){ 
              ledout.pattern[ledout.index] = random(0,ledout.length); 
            }
          #endif // ENABLE_DEVFEATURE_STDSHUFFLE_PIXEL_RANDOM

          SetLEDOutAmountByPercentage(100);


        break;
        case TRANSITION_ORDER_RANDOM_METHOD_NONREPEATING: default:  {     //  Serial.println("TRANSITION_ORDER_INORDER_ID2");
          // Generate a full list inorder
          uint16_t pick_list[ledout.length];
          uint16_t pick_list_remaining = ledout.length;
          uint16_t pick_list_index = 0;     
          uint16_t pick_list_val = 0;     
          for(ledout.index=0;ledout.index<ledout.length;ledout.index++){ 
            pick_list[ledout.index] = ledout.index;
          }
          // Pick a index, fill output, remove option
          ledout.index = 0; //reset output indexing

          while(pick_list_remaining)
          {
            pick_list_index = random(0,pick_list_remaining);
            pick_list_val = pick_list[pick_list_index];
           // AddLog(LOG_LEVEL_TEST,PSTR("pick_list_remaining=%d [%d:%d]"),pick_list_remaining,pick_list_index,pick_list_val);

            ledout.pattern[ledout.index++] = pick_list_val;

            // Remove pick_list_index
            for(int ii=0;ii<pick_list_remaining-1;ii++){
              //check if we have matched
              if(ii >= pick_list_index){         
                pick_list[ii] = pick_list[ii+1]; //shift by 1 index
                // Serial.printf("%d, ",pick_list[ii+1]);
              }
            }
        // Serial.println();

          pick_list_remaining--;
          }

          SetLEDOutAmountByPercentage(100);

        }
        break;
      }
    break;
    // case TRANSITION_ORDER_FIXED_ID:
    //   // pattern is contained within colour map
    // break;
    // case TRANSITION_ORDER_CENTRE_OUT_ID:{
    //   // uint16_t middle_index = length_local/2;
    //   //0,1,2,3,4,5,4,3,2,1,0
    //   // for(ledout.index=0;ledout.index<length_local;ledout.index++){ 
    //   //   index_pattern[ledout.index] = ledout.index; 
    //   // }
    // }
    // break;
  }

} //end function "RefreshLEDIndexPattern"


// Name:UpdateDesiredColourFromPaletteSelected
// Task: 
/**
 * This needs redoing, with a flag, to enable how I want the palette presented, and not how its encoded
 * "palette_pattern"
 * Gradient (either needs to get from palette, or equally generate it based on palette element count)
 * Single   (gets each colour in the palette, with ability to ignore indexs if they are present)
 * */
void mAnimatorLight::UpdateDesiredColourFromPaletteSelected(void){

  // Update pointer of struct
  mPaletteI->SetPaletteListPtrFromID(
#ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
  pCONT_iLight->animation
#else
  pCONT_lAni->_segments[0]
#endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
.palette.id);

  // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "UpdateDesiredColourFromPaletteSelected fMapIDs_Type %d"),mPaletteI->palettelist.ptr->flags.fMapIDs_Type);// \"%s\""),GetPaletteFriendlyName());
  // AddLog(LOG_LEVEL_DEBUG_MORE
  
  /**
   * Handle the retrieval of colours from palettes depending on the palette encoding type
   * */
  switch(mPaletteI->palettelist.ptr->flags.fMapIDs_Type){
    // default:
    //   #ifdef ENABLE_LOG_LEVEL_DEBUG
    //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "default"));
    //   #endif
    // case MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID:

    default:
    case MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID:
    case MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID:
    case MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX_ID: ///to be tested
    {
    //get colour above

      //apply to positiion below

     // SetLEDOutAmountByPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val);

      //what is this?
      // does this only run if above did not? ie this is default?

      //AddLog(LOG_LEVEL_TEST, PSTR("strip_size_requiring_update=%d"),strip_size_requiring_update);
      
      // Other types with random led patterns, or,
      
      switch(
#ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
  pCONT_iLight->animation
#else
  pCONT_lAni->_segments[0]
#endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
.transition.order_id){
        case TRANSITION_ORDER_RANDOM_ID:{

          // new transition, so force full update
          if(
#ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
  pCONT_iLight->animation
#else
  pCONT_lAni->_segments[0]
#endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
.flags.NewAnimationRequiringCompleteRefresh){
            strip_size_requiring_update = STRIP_SIZE_MAX;
          }
        

          RefreshLEDIndexPattern();


          int16_t pixel_position = -2;
          for(ledout.index=0;ledout.index<strip_size_requiring_update;ledout.index++){

            // For random, desired pixel from map will also be random
            desired_pixel = random(0,mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr));
            
            RgbTypeColor colour = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel,&pixel_position);

            // desired_colour[ledout.pattern[ledout.index]] = colour; 
            // AddLog(LOG_LEVEL_TEST, PSTR(DEBUG_INSERT_PAGE_BREAK "colour=%d,%d,%d"),
            // colour.R,
            // colour.G,
            // colour.B);

            animation_colours[ledout.pattern[ledout.index]].DesiredColour = colour;

            #ifndef ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS
            if(
#ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
  pCONT_iLight->animation
#else
  pCONT_lAni->_segments[0]
#endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
.flags.brightness_applied_during_colour_generation){
              animation_colours[ledout.pattern[ledout.index]].DesiredColour = ApplyBrightnesstoDesiredColourWithGamma(animation_colours[ledout.pattern[ledout.index]].DesiredColour,pCONT_iLight->getBriRGB_Global());
            }
            #endif // ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS

            // #ifdef ENABLE_DEBUG_MODULE_LIGHTS_ADDRESSABLE
            // AddLog(LOG_LEVEL_INFO, PSTR("colour=%d,%d,%d brt_set=%d"),
            //   animation_colours[ledout.pattern[ledout.index]].DesiredColour.R,
            //   animation_colours[ledout.pattern[ledout.index]].DesiredColour.G,
            //   animation_colours[ledout.pattern[ledout.index]].DesiredColour.B,
            //   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation
            // );
            // #endif 
          } //end for

        }break;
        case TRANSITION_ORDER_INORDER_ID:{
          // new transition, so force full update
          if(
#ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
  pCONT_iLight->animation
#else
  pCONT_lAni->_segments[0]
#endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
.flags.NewAnimationRequiringCompleteRefresh){
            strip_size_requiring_update = STRIP_SIZE_MAX;
          }
          RefreshLEDIndexPattern();
          int16_t pixel_position = -2;
          desired_pixel=0;
          for(ledout.index=0;ledout.index<strip_size_requiring_update;ledout.index++){
            RgbcctColor colour = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel,&pixel_position);

            // RgbTypeColor colour = colourcct;
            // desired_colour[ledout.pattern[ledout.index]] = colour; 
            // AddLog(LOG_LEVEL_TEST, PSTR("colour=%d,%d,%d"),
            // colour.R,
            // colour.G,
            // colour.B); 
            
            // AddLog(LOG_LEVEL_TEST, PSTR( "%d colour=%d,%d,%d"),desired_pixel, // DEBUG_INSERT_PAGE_BREAK
            // colour.R,
            // colour.G,
            // colour.B);

            // animation_colours[ledout.pattern[ledout.index]].DesiredColour = ApplyBrightnesstoDesiredColour(colour,pCONT_iLight->getBriRGB());
              // RgbColor colourbefore = animation_colours[ledout.pattern[ledout.index]].DesiredColour;
            animation_colours[ledout.pattern[ledout.index]].DesiredColour = colour;
           #ifndef ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS
            if(
#ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
  pCONT_iLight->animation
#else
  pCONT_lAni->_segments[0]
#endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
.flags.brightness_applied_during_colour_generation){
              animation_colours[ledout.pattern[ledout.index]].DesiredColour = ApplyBrightnesstoDesiredColourWithGamma(animation_colours[ledout.pattern[ledout.index]].DesiredColour, pCONT_iLight->getBriRGB_Global());
            }
            #endif //             ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS
// RgbColor colourafter = animation_colours[ledout.pattern[ledout.index]].DesiredColour;
// if(ledout.index<5){
//             RgbColor colour2 = colour;
//             AddLog(LOG_LEVEL_TEST, PSTR( "%d colourbefore colour=%d,%d,%d     %d,%d,%d"), // DEBUG_INSERT_PAGE_BREAK
//             ledout.index,colourbefore.R,colourbefore.G, colourbefore.B,
//             colourafter.R,
//             colourafter.G,
//             colourafter.B);
// }
            //   AddLog(LOG_LEVEL_TEST, PSTR("colou2=%d,%d,%d"),
            // animation_colours[ledout.pattern[ledout.index]].DesiredColour.R,
            // animation_colours[ledout.pattern[ledout.index]].DesiredColour.G,
            // animation_colours[ledout.pattern[ledout.index]].DesiredColour.B);

            
            #ifdef ENABLE_LOG_LEVEL_DEBUG
            // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "colour[p%d:d%d] = %d,%d,%d %d pp%d"),
            //   ledout.pattern[ledout.index],desired_pixel,
            //   pCONT_iLight->HueF2N(desired_colour[ledout.pattern[ledout.index]].H),pCONT_iLight->SatF2N(desired_colour[ledout.pattern[ledout.index]].S),pCONT_iLight->BrtF2N(desired_colour[ledout.pattern[ledout.index]].B),
            //   mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr),pixel_position
            // );
            #endif
            // AddLog(LOG_LEVEL_INFO, PSTR("colour=%d,%d,%d brt_set=%d"),
            //   animation_colours[ledout.pattern[ledout.index]].DesiredColour.R,
            //   animation_colours[ledout.pattern[ledout.index]].DesiredColour.G,
            //   animation_colours[ledout.pattern[ledout.index]].DesiredColour.B,
            //   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation
            // );

            if(++desired_pixel>=mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr)){desired_pixel=0;}
          } //end for
        }break;
        // case TRANSITION_ORDER_FIXED_ID:

        //   int16_t pixel_position = -2;
        //   //move across all encoded values
        //     // HsbColor colour = GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel,&pixel_position);

        //     // if(pixel_position>=0){
        //     //   desired_colour[pixel_position] = colour;
        //     // }else{
        //     //   desired_colour[ledout.pattern[ledout.index]] = colour;
        //     // }
            
        //     // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "colour[p%d:d%d] = %d,%d,%d %d pp%d"),
        //     //   ledout.pattern[ledout.index],desired_pixel,
        //     //   HueF2N(desired_colour[ledout.pattern[ledout.index]].H),SatF2N(desired_colour[ledout.pattern[ledout.index]].S),BrtF2N(desired_colour[ledout.pattern[ledout.index]].B),
        //     //   mPaletteI->palettelist.ptr->active_pixels_in_map,pixel_position
        //     // );    

        // break;
      }//end switch


    }break;
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_ID:
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_ID:
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_AND_SETALL_ID:
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_AND_SETALL_ID:{

      // Get active pixels in map
      uint16_t active_pixels_in_map = mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr); //width 2

    #ifdef ENABLE_LOG_LEVEL_INFO
      // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "active_pixels_in_map=%d"),active_pixels_in_map);
    #endif// ENABLE_LOG_LEVEL_INFO

      // Move across map
      int16_t pixel_position = -2;
      for(uint16_t desired_pixel=0;desired_pixel<active_pixels_in_map;desired_pixel++){
        RgbcctColor colour = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel,&pixel_position);
        
        #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
        //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "dp%d, pp%d, %d,%d %d"),desired_pixel, pixel_position,pCONT_iLight->HueF2N(colour.H),pCONT_iLight->SatF2N(colour.S),pCONT_iLight->BrtF2N(colour.B));
        #endif
        
        if(pixel_position>=0){
          // Set output to this "many" colour
          if(pixel_position == 255){
            for(uint16_t temp=0;temp<ledout.length;temp++){ 
              animation_colours[temp].DesiredColour = ApplyBrightnesstoRgbcctColour(colour,pCONT_iLight->getBriRGB_Global());
            }            
            #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
            // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "set ALL %d,%d %d"),pCONT_iLight->HueF2N(colour.H),pCONT_iLight->SatF2N(colour.S),pCONT_iLight->BrtF2N(colour.B));
            #endif
          }else{
            colour.R = 1;colour.R = 2;colour.R = 3;
            // Serial.println("colour.R = 1;colour.R = 2;colour.R = 3;");
            animation_colours[pixel_position].DesiredColour = ApplyBrightnesstoRgbcctColour(colour,pCONT_iLight->getBriRGB_Global());
            #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
            //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "set %d %d,%d %d"), pixel_position,pCONT_iLight->HueF2N(colour.H),pCONT_iLight->SatF2N(colour.S),pCONT_iLight->BrtF2N(colour.B));
            #endif
          }
        }else{          
          #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
          AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "return; //end early %d"),pixel_position);
          #endif
        }

      }
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "return; //end early %d"),pixel_position);
    #endif// ENABLE_LOG_LEVEL_INFO
      break;
      // return;
    }
    break;
    case MAPIDS_TYPE_RGBCCTCOLOUR_WITHINDEX_GRADIENT_ID:
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_GRADIENT_ID:
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID:{

      //#ifdef ENABLE_LOG_LEVEL_DEBUG
      // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID"));
      //#endif

      uint16_t start_pixel = 0;
      uint16_t end_pixel = 100;
      RgbcctColor start_colour = RgbcctColor(0);
      RgbcctColor end_colour = RgbcctColor(0);
      uint16_t desired_pixel = 0;
      int16_t start_pixel_position = -1, end_pixel_position = -1;
      uint8_t pixels_in_map = mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr);

      //#ifdef ENABLE_LOG_LEVEL_DEBUG
      // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "pixels_in_map %d"),pixels_in_map);
      //#endif


      // Add flag to enable ignoring gradient, and filling as normal palette (here?)

      for(uint8_t grad_pair_index=0;grad_pair_index<pixels_in_map-1;grad_pair_index++){
        
        start_colour = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel,  &start_pixel_position);
        end_colour   = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel+1,&end_pixel_position);

        
      // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "%d start_pixel_position %d"),grad_pair_index,start_pixel_position);
      // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "end_pixel_position %d"),end_pixel_position);

#ifndef ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS
        start_colour = ApplyBrightnesstoRgbcctColour(start_colour,pCONT_iLight->getBriRGB_Global());
        end_colour   = ApplyBrightnesstoRgbcctColour(end_colour,pCONT_iLight->getBriRGB_Global());
#endif // ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS

        // #ifdef ENABLE_LOG_LEVEL_DEBUG
        //  AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "s%d,%d %d %d"),pCONT_iLight->HueF2N(start_colour.H),pCONT_iLight->SatF2N(start_colour.S),pCONT_iLight->BrtF2N(start_colour.B),start_pixel_position);
        //  AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "e%d,%d %d %d"),HueF2N(end_colour.H),SatF2N(end_colour.S),BrtF2N(end_colour.B),end_pixel_position);
        // #endif

        switch(mPaletteI->palettelist.ptr->flags.fIndexs_Type){
          case INDEX_TYPE_DIRECT: break; //remains the same
          case INDEX_TYPE_SCALED_255: 
            start_pixel_position = map(start_pixel_position,0,255,0,ledout.length);
            end_pixel_position   = map(end_pixel_position,0,255,0,ledout.length);
            // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "255 %d %d"),start_pixel_position,end_pixel_position);
          break;
          case INDEX_TYPE_SCALED_100: 
            start_pixel_position = map(start_pixel_position,0,100,0,ledout.length);
            end_pixel_position   = map(end_pixel_position,0,100,0,ledout.length);          
          break;
        }


    
      // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "%d start_pixel_position %d"),grad_pair_index,start_pixel_position);
      // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "end_pixel_position %d"),end_pixel_position);


        float progress = 0;
        for(ledout.index=start_pixel_position;ledout.index<end_pixel_position;ledout.index++){
          progress = mSupport::mapfloat(ledout.index,start_pixel_position,end_pixel_position,0,1);
          animation_colours[ledout.index].DesiredColour = RgbcctColor::LinearBlend(start_colour, end_colour, progress);
        }
        desired_pixel++;
      }

      // Colour applied to entire DesiredColour, leaving now
      break;
      // return; //succcesfully handled

    }break;
    
  }

  #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "DONE UpdateDesiredColourFromPaletteSelected fMapIDs_Type "));
  #endif  // LOG_LEVEL_DEBUG_MORE

  //  AddLog(LOG_LEVEL_DEBUG, PSTR("colour=%d,%d,%d"),
  //             animation_colours[0].DesiredColour.R,
  //             animation_colours[0].DesiredColour.G,
  //             animation_colours[0].DesiredColour.B
  //           );

  
  // Handle brightness levels == if not set, use default
  //if(!mPaletteI->palettelist.ptr->fOverride_animation_brightness){
  // ApplyBrightnesstoDesiredColour(pCONT_iLight->getBriRGB());
  //}

} //end function UpdateDesiredColourFromPaletteSelected();


#endif // USE_DEVFEATURE_FUNCTION_UPGRADE_CORE_TRANSITIONMETHODS


// // Update struct that shows overview, always sends
uint8_t mAnimatorLight::ConstructJSON_Flasher(uint8_t json_level){

#ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
return 0;
#endif

  char buffer[100];

  JsonBuilderI->Start();

  JBI->Add("flashermillis",millis()-flashersettings.tSaved.Update);

//   // root[D_JSON_ONOFF] = pCONT_iLight->light_power_state ? "ON" : "OFF";

#ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  JBI->Add(PM_JSON_MODE, pCONT_iLight->GetAnimationModeName(buffer, sizeof(buffer)));
  JBI->Add(PM_JSON_FUNCTION, GetFlasherFunctionName(buffer, sizeof(buffer)));
  
#endif// ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS


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


#ifdef USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION


float mAnimatorLight::minf2(float v, float w)
{
  if (w > v) return v;
  return w;
}

float mAnimatorLight::maxf2(float v, float w)
{
  if (w > v) return w;
  return v;
}

void mAnimatorLight::colorRGBtoRGBW(byte* rgb) //rgb to rgbw (http://codewelt.com/rgbw). (RGBW_MODE_LEGACY)
{
  float low = minf2(rgb[0],minf2(rgb[1],rgb[2]));
  float high = maxf2(rgb[0],maxf2(rgb[1],rgb[2]));
  if (high < 0.1f) return;
  float sat = 100.0f * ((high - low) / high);;   // maximum saturation is 100  (corrected from 255)
  rgb[3] = (byte)((255.0f - sat) / 255.0f * (rgb[0] + rgb[1] + rgb[2]) / 3);
}

void mAnimatorLight::colorFromDecOrHexString(byte* rgb, char* in)
{
  if (in[0] == 0) return;
  char first = in[0];
  uint32_t c = 0;
  
  if (first == '#' || first == 'h' || first == 'H') //is HEX encoded
  {
    c = strtoul(in +1, NULL, 16);
  } else
  {
    c = strtoul(in, NULL, 10);
  }

  rgb[3] = (c >> 24) & 0xFF;
  rgb[0] = (c >> 16) & 0xFF;
  rgb[1] = (c >>  8) & 0xFF;
  rgb[2] =  c        & 0xFF;
}

/*
 * Color conversion methods
 */
void mAnimatorLight::colorFromUint32(uint32_t in, bool secondary)
{
  if (secondary) {
    colSec[3] = in >> 24 & 0xFF;
    colSec[0] = in >> 16 & 0xFF;
    colSec[1] = in >> 8  & 0xFF;
    colSec[2] = in       & 0xFF;
  } else {
    col[3] = in >> 24 & 0xFF;
    col[0] = in >> 16 & 0xFF;
    col[1] = in >> 8  & 0xFF;
    col[2] = in       & 0xFF;
  }
}

//load a color without affecting the white channel
void mAnimatorLight::colorFromUint24(uint32_t in, bool secondary)
{
  if (secondary) {
    colSec[0] = in >> 16 & 0xFF;
    colSec[1] = in >> 8  & 0xFF;
    colSec[2] = in       & 0xFF;
  } else {
    col[0] = in >> 16 & 0xFF;
    col[1] = in >> 8  & 0xFF;
    col[2] = in       & 0xFF;
  }
}

//relatively change white brightness, minumum A=5
void mAnimatorLight::relativeChangeWhite(int8_t amount, byte lowerBoundary)
{
  int16_t new_val = (int16_t) col[3] + amount;
  if (new_val > 0xFF) new_val = 0xFF;
  else if (new_val < lowerBoundary) new_val = lowerBoundary;
  col[3] = new_val;
}


// #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

// #ifdef USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION

void mAnimatorLight::colorHStoRGB(uint16_t hue, byte sat, byte* rgb) //hue, sat to rgb
{
  float h = ((float)hue)/65535.0;
  float s = ((float)sat)/255.0;
  byte i = floor(h*6);
  float f = h * 6-i;
  float p = 255 * (1-s);
  float q = 255 * (1-f*s);
  float t = 255 * (1-(1-f)*s);
  switch (i%6) {
    case 0: rgb[0]=255,rgb[1]=t,rgb[2]=p;break;
    case 1: rgb[0]=q,rgb[1]=255,rgb[2]=p;break;
    case 2: rgb[0]=p,rgb[1]=255,rgb[2]=t;break;
    case 3: rgb[0]=p,rgb[1]=q,rgb[2]=255;break;
    case 4: rgb[0]=t,rgb[1]=p,rgb[2]=255;break;
    case 5: rgb[0]=255,rgb[1]=p,rgb[2]=q;
  }
  if (useRGBW && mEffects->rgbwMode == RGBW_MODE_LEGACY) colorRGBtoRGBW(col);
}

void mAnimatorLight::colorCTtoRGB(uint16_t mired, byte* rgb) //white spectrum to rgb
{
  //this is only an approximation using WS2812B with gamma correction enabled
  if (mired > 475) {
    rgb[0]=255;rgb[1]=199;rgb[2]=92;//500
  } else if (mired > 425) {
    rgb[0]=255;rgb[1]=213;rgb[2]=118;//450
  } else if (mired > 375) {
    rgb[0]=255;rgb[1]=216;rgb[2]=118;//400
  } else if (mired > 325) {
    rgb[0]=255;rgb[1]=234;rgb[2]=140;//350
  } else if (mired > 275) {
    rgb[0]=255;rgb[1]=243;rgb[2]=160;//300
  } else if (mired > 225) {
    rgb[0]=250;rgb[1]=255;rgb[2]=188;//250
  } else if (mired > 175) {
    rgb[0]=247;rgb[1]=255;rgb[2]=215;//200
  } else {
    rgb[0]=237;rgb[1]=255;rgb[2]=239;//150
  }
  if (useRGBW && mEffects->rgbwMode == RGBW_MODE_LEGACY) colorRGBtoRGBW(col);
}


#ifdef USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
void mAnimatorLight::Init_WLED(void)
{

  pCONT_iLight->Init_NeoPixelAnimator(1, NEO_ANIMATION_TIMEBASE); // NeoPixel animation management object

  ledCount = 50;

  mEffects = new WS2812FX();

  // Class constructor of WS2812FX is defining the animations and the size at the start

  DEBUG_PRINT("esp8266 ");
  DEBUG_PRINTLN(ESP.getCoreVersion());
  
  int heapPreAlloc = ESP.getFreeHeap();
  DEBUG_PRINT("heap ");
  DEBUG_PRINTLN(ESP.getFreeHeap());

  mEffects->Init_WLED_Conversion(0, ledCount, 0);
  mEffects->setBrightness(0);

  DEBUG_PRINT("LEDs inited. heap usage ~");
  DEBUG_PRINTLN(heapPreAlloc - ESP.getFreeHeap());

  colorUpdated(NOTIFIER_CALL_MODE_INIT);
  
  mEffects->service();
  
  //mEffects->setEffectConfig(effectCurrent, effectSpeed, effectIntensity, effectPalette);
  mEffects->setEffectConfig(FX_MODE_FADE, 100, 100, 11);

}


void mAnimatorLight::SubTask_WLED_Animation_PhaseOut()
{
  
  handleTransitions();
  mEffects->service();

}




void mAnimatorLight::resetTimebase()
{
  mEffects->timebase = 0 - millis();
}


void mAnimatorLight::toggleOnOff()
{
  if (bri == 0)
  {
    bri = briLast;
  } else
  {
    briLast = bri;
    bri = 0;
  }
}


void mAnimatorLight::setAllLeds()
{
  if (!realtimeMode || !arlsForceMaxBri)
  {
    double d = briT*briMultiplier;
    int val = d/100;
    if (val > 255) val = 255;
    mEffects->setBrightness(val);
  }
  if (useRGBW && mEffects->rgbwMode == RGBW_MODE_LEGACY)
  {
    colorRGBtoRGBW(colT);
    colorRGBtoRGBW(colSecT);
  }
  mEffects->setColor(0, colT[0], colT[1], colT[2], colT[3]);
  mEffects->setColor(1, colSecT[0], colSecT[1], colSecT[2], colSecT[3]);
}




/*
 * LED methods
 */
void mAnimatorLight::setValuesFromMainSeg()
{
  WS2812FX::Segment& seg = mEffects->getSegment(mEffects->getMainSegmentId());
  colorFromUint32(seg.colors[0]);
  colorFromUint32(seg.colors[1], true);
  effectCurrent = seg.mode;
  effectSpeed = seg.speed;
  effectIntensity = seg.intensity;
  effectPalette = seg.palette;
}

void mAnimatorLight::setLedsStandard(bool justColors)
{
  for (byte i=0; i<4; i++)
  {
    colOld[i] = col[i];
    colT[i] = col[i];
    colSecOld[i] = colSec[i];
    colSecT[i] = colSec[i];
  }
  if (justColors) return;
  briOld = bri;
  briT = bri;
  setAllLeds();
}


bool mAnimatorLight::colorChanged()
{
  for (byte i=0; i<4; i++)
  {
    if (col[i] != colIT[i]) return true;
    if (colSec[i] != colSecIT[i]) return true;
  }
  if (bri != briIT) return true;
  return false;
}


void mAnimatorLight::colorUpdated(int callMode)
{
  //call for notifier -> 0: init 1: direct change 2: button 3: notification 4: nightlight 5: other (No notification)
  //                     6: fx changed 7: hue 8: preset cycle 9: blynk 10: alexa
  if (callMode != NOTIFIER_CALL_MODE_INIT && 
      callMode != NOTIFIER_CALL_MODE_DIRECT_CHANGE && 
      callMode != NOTIFIER_CALL_MODE_NO_NOTIFY) mEffects->applyToAllSelected = true; //if not from JSON api, which directly sets segments
  
  bool fxChanged = mEffects->setEffectConfig(effectCurrent, effectSpeed, effectIntensity, effectPalette);
  bool colChanged = colorChanged();

 // Serial.println("HERE");

  if (fxChanged || colChanged)
  {
    if (realtimeTimeout == UINT32_MAX) realtimeTimeout = 0;
    if (isPreset) {isPreset = false;}
        else {currentPreset = -1;}
        
    //notify(callMode);
    
    //set flag to update blynk and mqtt
    //if (callMode != NOTIFIER_CALL_MODE_PRESET_CYCLE) interfaceUpdateCallMode = callMode;
  } else {
    // if (nightlightActive && !nightlightActiveOld && 
    //     callMode != NOTIFIER_CALL_MODE_NOTIFICATION && 
    //     callMode != NOTIFIER_CALL_MODE_NO_NOTIFY)
    // {
    //   //notify(NOTIFIER_CALL_MODE_NIGHTLIGHT); 
    //   //interfaceUpdateCallMode = NOTIFIER_CALL_MODE_NIGHTLIGHT;
    // }
  }
  
  if (!colChanged) return; //following code is for e.g. initiating transitions
  
  // if (callMode != NOTIFIER_CALL_MODE_NO_NOTIFY && nightlightActive && (nightlightMode == NL_MODE_FADE || nightlightMode == NL_MODE_COLORFADE))
  // {
  //   briNlT = bri;
  //   nightlightDelayMs -= (millis() - nightlightStartTime);
  //   nightlightStartTime = millis();
  // }
  for (byte i=0; i<4; i++)
  {
    colIT[i] = col[i];
    colSecIT[i] = colSec[i];
  }
  if (briT == 0)
  {
    setLedsStandard(true);                                            //do not color transition if starting from off
    if (callMode != NOTIFIER_CALL_MODE_NOTIFICATION) resetTimebase(); //effect start from beginning
  }

  briIT = bri;
  if (bri > 0) briLast = bri;

  //deactivate nightlight if target brightness is reached
  // if (bri == nightlightTargetBri && callMode != NOTIFIER_CALL_MODE_NO_NOTIFY) nightlightActive = false;
  
  if (fadeTransition)
  {
    //set correct delay if not using notification delay
    if (callMode != NOTIFIER_CALL_MODE_NOTIFICATION && !jsonTransitionOnce) transitionDelayTemp = transitionDelay;
    jsonTransitionOnce = false;
    if (transitionDelayTemp == 0) {setLedsStandard(); mEffects->trigger(); return;}
    
    if (transitionActive)
    {
      for (byte i=0; i<4; i++)
      {
        colOld[i] = colT[i];
        colSecOld[i] = colSecT[i];
      }
      briOld = briT;
      tperLast = 0;
    }
    mEffects->setTransitionMode(true);
    transitionActive = true;
    transitionStartTime = millis();
  } else
  {
    setLedsStandard();
    mEffects->trigger();  //run now
  }
}



void mAnimatorLight::handleTransitions()
{
  
  if (transitionActive && transitionDelayTemp > 0)
  {
    float tper = (millis() - transitionStartTime)/(float)transitionDelayTemp;
    if (tper >= 1.0)
    {
      mEffects->setTransitionMode(false);
      transitionActive = false;
      tperLast = 0;
      setLedsStandard();
      return;
    }
    if (tper - tperLast < 0.004) return;
    tperLast = tper;
    for (byte i=0; i<4; i++)
    {
      colT[i] = colOld[i]+((col[i] - colOld[i])*tper);
      colSecT[i] = colSecOld[i]+((colSec[i] - colSecOld[i])*tper);
    }
    briT    = briOld   +((bri    - briOld   )*tper);
    
    setAllLeds();
  }
}

    // #endif // USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION


//enable custom per-LED mapping. This can allow for better effects on matrices or special displays
//#define WLED_CUSTOM_LED_MAPPING
#ifdef WLED_CUSTOM_LED_MAPPING
//this is just an example (30 LEDs). It will first set all even, then all uneven LEDs.
const uint16_t customMappingTable[] = {
  0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28,
  1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29};

//another example. Switches direction every 5 LEDs.
/*const uint16_t customMappingTable[] = {
  0, 1, 2, 3, 4, 9, 8, 7, 6, 5, 10, 11, 12, 13, 14,
  19, 18, 17, 16, 15, 20, 21, 22, 23, 24, 29, 28, 27, 26, 25};*/

const uint16_t customMappingSize = sizeof(customMappingTable)/sizeof(uint16_t); //30 in example
#endif


const char* mAnimatorLight::GetEffectsModeNamebyID(uint8_t id, char* buffer, uint8_t buflen)
{
  pCONT_sup->GetTextIndexed_P(buffer, buflen, id, PM_FX_MODES_NAMES_CTR);
  return buffer;
}

// #endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
 #endif//define USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION

#endif // USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
#endif// ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT

#endif// ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT


#endif //USE_MODULE_LIGHTS_ANIMATOR




