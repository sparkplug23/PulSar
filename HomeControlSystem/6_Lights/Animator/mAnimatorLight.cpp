#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

const char* mAnimatorLight::PM_MODULE_LIGHTS_ANIMATOR_CTR = D_MODULE_LIGHTS_ANIMATOR_CTR;
const char* mAnimatorLight::PM_MODULE_LIGHTS_ANIMATOR_FRIENDLY_CTR = D_MODULE_LIGHTS_ANIMATOR_FRIENDLY_CTR;


int8_t mAnimatorLight::Tasker(uint8_t function, JsonParserObject obj)
{

  #ifdef ENABLE_DEVFEATURE_Tasker_Override_Forcing_Variables_Testing
  Tasker_Override_Forcing_Variables_Testing();
  #endif // ENABLE_DEVFEATURE_FLICKERING_TEST5

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case FUNC_PRE_INIT:
      Pre_Init();  
      break;
    case FUNC_INIT:
      Init();
      #ifdef USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
      Init_WLED();
      #endif
      break;
  }
  
  if(!settings.flags.EnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID;}
  
  switch(function){
    /************
     * SETTINGS SECTION * 
    *******************/
    // case FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE: 
    //   Settings_Load();
    // break;
    // case FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE: 
    //   Settings_Save();
    // break;
    // case FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES:
    // case FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT:
    //   Settings_Default();
    // break;
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_EVERY_SECOND:{
    
      //EverySecond();

      // const NeoRgbcctCurrentSettings settings(200,200,200,201,202);
      // uint32_t maTotal = stripbus->CalcTotalMilliAmpere(settings);

      // #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
      //   char buffer[50];
      //   AddLog(LOG_LEVEL_TEST, PSTR("GetEffectsModeNamebyID=%s"), 
      //     pCONT_lAni->GetEffectsModeNamebyID(mEffects->getMode(),buffer,sizeof(buffer))
      //   );
      // #endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

      // AddLog_Array(LOG_LEVEL_WARN, "colourmap0", mPaletteI->palettelist.rgbcct_users[0].colour_map_id, 5);
      // AddLog_Array(LOG_LEVEL_WARN, "colourmap1", mPaletteI->palettelist.rgbcct_users[1].colour_map_id, 5);
      // AddLog_Array(LOG_LEVEL_WARN, "colourmap2", mPaletteI->palettelist.rgbcct_users[2].colour_map_id, 5);
      // AddLog_Array(LOG_LEVEL_WARN, "colourmap3", mPaletteI->palettelist.rgbcct_users[3].colour_map_id, 5);
      // AddLog_Array(LOG_LEVEL_WARN, "colourmap4", mPaletteI->palettelist.rgbcct_users[4].colour_map_id, 5);





    }break;
    case FUNC_LOOP: 
      #ifdef USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
       SubTask_WLED_Animation_PhaseOut();
      #else
        EveryLoop();
      #endif
    break;    
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
     parse_JSONCommand(obj);
    break;
    /************
     * TRIGGERS SECTION * 
    *******************/
    case FUNC_EVENT_INPUT_STATE_CHANGED_ID:
      #ifdef USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS
      CommandSet_Physical_UserInput_Buttons();
      #endif
    break;
    /************
     * MQTT SECTION * 
    *******************/   
    #ifdef USE_MODULE_NETWORK_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
    case FUNC_MQTT_HANDLERS_RESET:
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_TelePeriod();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_RefreshAll();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }// switch(command)

  /************
   * WEBPAGE SECTION * 
  *******************/  
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  return Tasker_Web(function);
  #endif // USE_MODULE_NETWORK_WEBSERVER

} // END FUNCTION




/*******************************************************************************************************************
********************************************************************************************************************
************ START OF GENERAL INIT PROCESS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/

void mAnimatorLight::Pre_Init(void){ 
  
  // Allow runtime changes of animation size
  pCONT_iLight->settings.light_size_count = STRIP_SIZE_MAX; 
  pCONT_set->Settings.flag_animations.clear_on_reboot = false; //flag

}


void mAnimatorLight::Init_NeoPixelBus()
{

  // if pixel size changes, free and init again
  uint16_t strip_size_tmp = STRIP_SIZE_MAX;
  #ifdef STRIP_REPEAT_OUTPUT_MAX;//pCONT_iLight->settings.light_size_count<STRIP_SIZE_MAX?pCONT_iLight->settings.light_size_count:STRIP_SIZE_MAX; // Catch values exceeding limit
  strip_size_tmp = STRIP_REPEAT_OUTPUT_MAX;
  #endif
  // uint16_t strip_size_tmp = STRIP_REPEAT_OUTPUT_MAX;//pCONT_iLight->settings.light_size_count<STRIP_SIZE_MAX?pCONT_iLight->settings.light_size_count:STRIP_SIZE_MAX; // Catch values exceeding limit
  
  //step1:  moving the desired/starting colours into a buffer type, so it is dynamic
  //step2:  to become its own function, so strips can be changed at runtime
  stripbus = new NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType>(strip_size_tmp, 23);//19); 3 = rx0
  

}



void mAnimatorLight::Init(void){ 
    
  // ledout.length = STRIP_SIZE_MAX; 
  
  pCONT_iLight->settings.light_size_count = STRIP_SIZE_MAX;

  DEBUG_LINE;

  // stripbus->NeoRgbCurrentSettings(1,2,3);

  //step1:  moving the desired/starting colours into a buffer type, so it is dynamic
  //step2:  to become its own function, so strips can be changed at runtime
  // stripbus = new NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType>(strip_size_tmp, 23);//19); 3 = rx0

  Init_NeoPixelBus();
  

  // #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
  // mqtthandler_debug_animations_progress.tRateSecs = 1;
  // #endif


  pCONT_iLight->Init_NeoPixelAnimator(1, NEO_ANIMATION_TIMEBASE);  

  // #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
  // // pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val = 100;  
  // SetLEDOutAmountByPercentage(100);//pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val);  
  // #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT

  _segments[0].flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique = false;
  _segments[0].flags.Limit_Upper_Brightness_With_BrightnessRGB = false;
  _segments[0].flags.Apply_Upper_And_Lower_Brightness_Randomly_Ranged_To_Palette_Choice = false; // FIX
    
  #ifdef USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
    flashersettings.brightness_max = 255;
    flashersettings.brightness_min = 0;
  #endif // USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION

  // #ifdef ENABLE_DEVFEATURE_DIRECT_TEMPFIX_RANDOMISE_BRIGHTNESS_ON_PALETTE_GET // move to proper command options
  //   // pCONT_iLight->animation.flags.Apply_Upper_And_Lower_Brightness_Randomly_Ranged_To_Palette_Choice = true; // FIX
  //   pCONT_iLight->animation.flags.Limit_Upper_Brightness_With_BrightnessRGB = true;
  // #endif // ENABLE_DEVFEATURE_DIRECT_TEMPFIX_RANDOMISE_BRIGHTNESS_ON_PALETTE_GET

  #ifdef ENABLE_FEATURE_PIXEL_GROUP_MULTIPLIERS
  pixel_group.flags.fEnabled = true;
  pixel_group.flags.multiplier_mode_id = PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_UNIFORM_ID;
  pixel_group.multiplier = 10;
  pixel_group.mapped_array_data.values = editable_mapped_array_data_array;
  pixel_group.flags.mapped_array_editable_or_progmem = 0;
  pixel_group.mapped_array_data.length = 0;
  // flashersettings.flags.enable_startcolour_as_alternate = true;
  // flashersettings.flags.enable_endingcolour_as_alternate = true;
  #endif // ENABLE_FEATURE_PIXEL_GROUP_MULTIPLIERS
  
  // for(ledout.index=0;
  //   ledout.index<STRIP_SIZE_MAX;
  //   ledout.index++){ 
  //   ledout.pattern[ledout.index] = ledout.index; 
  // }

  DEBUG_LINE;

  #ifndef ENABLE_DEVFEATURE_PHASE_OUT_ANIMATIONCOLOUR_STRUCT
  // Clear stored light output
  memset(&animation_colours,0,sizeof(animation_colours));
  
  #endif //ifndef ENABLE_DEVFEATURE_PHASE_OUT_ANIMATIONCOLOUR_STRUCT


  // Start display
  stripbus->Begin();
  if(pCONT_set->Settings.flag_animations.clear_on_reboot){
    stripbus->ClearTo(0);
    pCONT_iLight->ShowInterface();
  }

  #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
    init_Notifications();
  #endif
  #ifdef ENABLE_PIXEL_FUNCTION_AMBILIGHT
  blocking_force_animate_to_complete = true; //animate to completion on boot (for short animations)
  init_Ambilight();
  #endif // ENABLE_PIXEL_FUNCTION_AMBILIGHT
  #ifdef ENABLE_PIXEL_FUNCTION_MIXER
  init_mixer_defaults();
  #endif

  settings.flags.EnableModule = true;

  #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  Init_Segments();
  #endif

  #ifdef USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS
  user_input.selected.palette_id = pCONT_set->Settings.animation_settings.xmas_controller_params[0];
  user_input.selected.brightness_id =  pCONT_set->Settings.animation_settings.xmas_controller_params[1];
  user_input.selected.effects_id =  pCONT_set->Settings.animation_settings.xmas_controller_params[2];
  user_input.selected.intensity_id = pCONT_set->Settings.animation_settings.xmas_controller_params[3];
  PhysicalController_Convert_IDs_to_Internal_Parameters();
  #endif

} //end "init"


void mAnimatorLight::Settings_Load(){
    
  // pCONT_iLight->animation.brightness = pCONT_set->Settings.light_settings.light_brightness_as_percentage/100.0f;
  
  pCONT_iLight->setBriRGB_Global(pCONT_set->Settings.light_settings.light_brightness_as_percentage);

  // uint8_t list = 0;
  // SetPaletteListPtrFromID(list);
  // AddLog(LOG_LEVEL_TEST,PSTR("LOADED %d amount = %d"),list,palettelist.ptr->colour_map_size);
  // for(uint8_t element=0;element<20;element++){
  //   AddLog(LOG_LEVEL_TEST,PSTR("LOADED %d i%d amount = %d"),list,element,pCONT_set->Settings.palette_user_colour_map_ids[list][element]);
  // }

  // Save colour map IDS
  // for(uint8_t list=0;list<20;list++){
  //   SetPaletteListPtrFromID(list);
  //   palettelist.ptr->colour_map_size = pCONT_set->Settings.animation_settings.palette_user_amounts[list]<20?pCONT_set->Settings.animation_settings.palette_user_amounts[list]:20;
  //   for(uint8_t element=0;element<20;element++){
  //     palettelist.ptr->colour_map_id[element] = pCONT_set->Settings.animation_settings.palette_user_colour_map_ids[list][element];
  //   }
  // }
  
  // for(uint8_t list=0;list<20;list++){
  //   memcpy(palettelist_variable_users_ctr,pCONT_set->Settings.animation_settings.palette_user_variable_name_ctr,sizeof(palettelist_variable_users_ctr));
  // }


// these need reversed!!!
  // pCONT_set->Settings.animation_settings.animation_mode               = _segments[0].mode_id;
  // pCONT_set->Settings.animation_settings.animation_palette            = _segments[0].palette.id;
  // pCONT_set->Settings.animation_settings.animation_transition_order   = _segments[0].transition.order_id;
  // pCONT_set->Settings.animation_settings.animation_transition_method  = _segments[0].transition.method_id;
  // pCONT_set->Settings.animation_settings.animation_transition_time_ms = _segments[0].transition.time_ms;
  // pCONT_set->Settings.animation_settings.animation_transition_rate_ms = _segments[0].transition.rate_ms;

#ifdef USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS
  user_input.selected.palette_id = pCONT_set->Settings.animation_settings.xmas_controller_params[0];
  user_input.selected.brightness_id =  pCONT_set->Settings.animation_settings.xmas_controller_params[1];
  user_input.selected.effects_id =  pCONT_set->Settings.animation_settings.xmas_controller_params[2];
  user_input.selected.intensity_id = pCONT_set->Settings.animation_settings.xmas_controller_params[3];
  AddLog(LOG_LEVEL_HIGHLIGHT, PSTR("LOADED user_input.selected.palette_id %d"),user_input.selected.palette_id);
#endif // USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS


}

void mAnimatorLight::Settings_Save(){
  
  // pCONT_set->Settings.light_settings.light_brightness_as_percentage = pCONT_iLight->animation.brightness*100;

  // #ifdef ENABLE_LOG_LEVEL_INFO
  // AddLog(LOG_LEVEL_TEST,PSTR("SAVED pCONT_iLight->animation.brightness = %d"),pCONT_set->Settings.light_settings.light_brightness_as_percentage);
  // #endif
  
  // Save colour map IDS
  // for(uint8_t list=0;list<20;list++){
  //   SetPaletteListPtrFromID(list);
  //   pCONT_set->Settings.animation_settings.palette_user_amounts[list] = palettelist.ptr->colour_map_size;
  //   for(uint8_t element=0;element<20;element++){
  //     pCONT_set->Settings.animation_settings.palette_user_colour_map_ids[list][element] = palettelist.ptr->colour_map_id[element];
  //   }
  // }

  // for(uint8_t list=0;list<20;list++){
  //   memcpy(pCONT_set->Settings.animation_settings.palette_user_variable_name_ctr,palettelist_variable_users_ctr,sizeof(palettelist_variable_users_ctr));
  // }

  pCONT_set->Settings.animation_settings.animation_mode               = _segments[0].mode_id;
  pCONT_set->Settings.animation_settings.animation_palette            = _segments[0].palette.id;
  pCONT_set->Settings.animation_settings.animation_transition_order   = _segments[0].transition.order_id;
  pCONT_set->Settings.animation_settings.animation_transition_method  = _segments[0].transition.method_id;
  pCONT_set->Settings.animation_settings.animation_transition_time_ms = _segments[0].transition.time_ms;
  pCONT_set->Settings.animation_settings.animation_transition_rate_ms = _segments[0].transition.rate_ms;

  // pCONT_set->Settings.animation_settings.animation_transition_pixels_to_update_as_percentage = pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val;

  // pCONT_set->Settings.animation_settings.xmas_controller_params[0] = user_input.selected.palette_id;
  // pCONT_set->Settings.animation_settings.xmas_controller_params[1] = user_input.selected.brightness_id;
  // pCONT_set->Settings.animation_settings.xmas_controller_params[2] = user_input.selected.effects_id;
  // pCONT_set->Settings.animation_settings.xmas_controller_params[3] = user_input.selected.intensity_id;
  
#ifdef USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS

  pCONT_set->Settings.animation_settings.xmas_controller_params[0] = user_input.selected.palette_id;
  pCONT_set->Settings.animation_settings.xmas_controller_params[1] = user_input.selected.brightness_id;
  pCONT_set->Settings.animation_settings.xmas_controller_params[2] = user_input.selected.effects_id;
  pCONT_set->Settings.animation_settings.xmas_controller_params[3] = user_input.selected.intensity_id;

  // PhysicalController_Convert_IDs_to_Internal_Parameters();

  pCONT_set->Settings.animation_settings.xmas_controller_params[5] = 6;

#endif // USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS


}



void mAnimatorLight::Settings_Default(){

DEBUG_LINE;
  
  init(); //default values

  #ifdef ENABLE_LOG_LEVEL_INFO
  // AddLog(LOG_LEVEL_TEST,PSTR("mAnimatorLight::Settings_Default"));
  #endif

  #ifdef USE_LIGHTING_TEMPLATE
    settings.flags.TemplateProvidedInProgMem = true;
  #endif
  
  if(settings.flags.TemplateProvidedInProgMem){// use template provided in progmem || override template
  
    //pCONT_iLight->Template_Load();

  }else{
    
    //pCONT_iLight->animation.transition.method_id = TRANSITION_METHOD_BLEND_ID;


    // // Check for user defined defaults
    // #ifdef DEFAULT_LIGHTING_TRANSITION_ORDER_ID
    //   pCONT_iLight->animation.transition.order_id = 6;/*DEFAULT_LIGHTING_TRANSITION_ORDER_ID<TRANSITION_ORDER_LENGTH_ID?
    //                                         DEFAULT_LIGHTING_TRANSITION_ORDER_ID:TRANSITION_ORDER_INORDER_ID;*/
    // #else
    //   pCONT_iLight->animation.transition.order_id = TRANSITION_ORDER_INORDER_ID;
    // #endif
    
    // #ifdef DEFAULT_LIGHTING_TRANSITION_TIME_MAP_SECS_ID
    //   // pCONT_iLight->animation.transition.time_ms.map_id = 10;
    //   /*DEFAULT_LIGHTING_TRANSITION_TIME_MAP_SECS_ID<TIME_MAP_SECS_LENGTH_ID?
    //                                             DEFAULT_LIGHTING_TRANSITION_TIME_MAP_SECS_ID:TIME_MAP_SECS_15_ID;*/
    //   pCONT_iLight->animation.transition.time_ms = 2000;//PROGMEM rate_map_secs[pCONT_iLight->animation.transition.time_ms.map_id]*1000;
    //   pCONT_iLight->animation.flags.ftime_use_map = true;
    // #else
    //   pCONT_iLight->animation.transition.time_ms.map_id = TIME_MAP_SECS_15_ID;
    //   pCONT_iLight->animation.transition.time_ms.val = 1000;//PROGMEM rate_map_secs[pCONT_iLight->animation.transition.time_ms.map_id]*1000;
    //   pCONT_iLight->animation.flags.ftime_use_map = true;
    // // #endif

    // // #ifdef DEFAULT_LIGHTING_TRANSITION_RATE_MAP_SECS_ID
    //   // pCONT_iLight->animation.transition.rate_ms.map_id = 6;
    //   /*DEFAULT_LIGHTING_TRANSITION_RATE_MAP_SECS_ID<RATE_MAP_SECS_LENGTH_ID?
    //                                             DEFAULT_LIGHTING_TRANSITION_RATE_MAP_SECS_ID:RATE_MAP_SECS_15_ID;*/
    //   pCONT_iLight->animation.transition.rate_ms = 10000;//PROGMEM rate_map_secs[pCONT_iLight->animation.transition.rate_ms.map_id]*1000;
    //   pCONT_iLight->animation.flags.frate_use_map = true;
    // // #else
    // //   pCONT_iLight->animation.transition.rate_ms.map_id = RATE_MAP_SECS_15_ID;
    // //   pCONT_iLight->animation.transition.rate_ms = PROGMEM rate_map_secs[pCONT_iLight->animation.transition.rate_ms.map_id]*1000;
    // //   pCONT_iLight->animation.flags.frate_use_map = true;
    // // #endif

    // // #ifdef DEFAULT_LIGHTING_PIXELS_UPDATE_PERCENTAGE_ID
    // //   pCONT_iLight->animation.transition.pixels_to_update_as_percentage.map_id = 4;/*DEFAULT_LIGHTING_PIXELS_UPDATE_PERCENTAGE_ID<PIXELS_UPDATE_PERCENTAGE_LENGTH_ID?
    // //                         DEFAULT_LIGHTING_PIXELS_UPDATE_PERCENTAGE_ID:PIXELS_UPDATE_PERCENTAGE_20_ID;*/
    // // #else
    // //   pCONT_iLight->animation.transition.pixels_to_update_as_percentage.map_id = PIXELS_UPDATE_PERCENTAGE_20_ID;
    // // #endif

    #ifdef DEFAULT_LIGHTING_BRIGHTNESS_PERCENTAGE
      // pCONT_iLight->animation.brightness = (float)DEFAULT_LIGHTING_BRIGHTNESS_PERCENTAGE/100.0f;//mSupport::WithinLimits(0,(float)DEFAULT_LIGHTING_BRIGHTNESS_PERCENTAGE/100.0f,1)
    #else
      // pCONT_iLight->animation.brightness = WithinLimits(0,(float)1/100.0f,1);
      // pCONT_iLight->animation.brightness = 1; //default ot 50% normally for power reasons
    #endif

    #ifndef DISABLE_WEBSERVER
    #ifdef DEFAULT_LIGHTING_LIVEVIEW_REFRESH_RATE_HZ
      liveview.refresh_rate = DEFAULT_LIGHTING_LIVEVIEW_REFRESH_RATE_HZ;
    #else
      pCONT_iLight->liveview.refresh_rate = 1000;
    #endif
    #endif // DISABLE_WEBSERVER
    
    
    // #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
    // #ifdef DEFAULT_LIGHTING_EFFECTS_FUNCTION_ID
    //   flashersettings.function = 1;/*DEFAULT_LIGHTING_EFFECTS_FUNCTION_ID<EFFECTS_FUNCTION_LENGTH_ID?
    //                         DEFAULT_LIGHTING_EFFECTS_FUNCTION_ID:EFFECTS_FUNCTION_SLOW_GLOW_ID;*/
    // #else
    //   flashersettings.function = EFFECTS_FUNCTION_SLOW_GLOW_ID;
    // #endif
    // #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT

  }


  // uint16_t buffer_length = 0;
  // char* buffer = pCONT_set->Settings.animation_settings.palette_user_variable_name_list_ctr;
  // for(uint8_t i=0;i<20;i++){
  //  buffer_length+=sprintf(buffer+buffer_length,"%s%02d|\0","User",i);
  // }
  // #ifndef DISABLE_SERIAL_LOGGING
  // //  Serial.println(buffer); log debuf more
  // #endif

  
  #ifdef ENABLE_PIXEL_FUNCTION_PIXELGROUPING
  memset(&editable_mapped_array_data_array,0,sizeof(editable_mapped_array_data_array));
  
  uint16_t single_row_count_array[] = {
    31,35,39,42,43,//31,44,39,42,43,
    43,43,42,42,42,
    41,39,38,36,33,
    30,29,28,25,25,
    23,22,22,20,18,
    19,17,18,16,14,
    14,13,10,10,11,
    11,8,4,4,5,6,6,
    5,5,5,5,5,
    0,3
  }; // 0 is filled last
  uint8_t single_row_count_array_size = 49;//sizeof(single_row_count_array)/single_row_count_array[0];

  // Serial.println(single_row_count_array_size);
  // delay(5000);
  
  uint16_t count = 0;
  for(int i=0;i<single_row_count_array_size;i++){
    count += single_row_count_array[i];
  }
  single_row_count_array[single_row_count_array_size-2] = 1050-count; // use 2nd last one as filler
  

  memcpy(editable_mapped_array_data_array, single_row_count_array, sizeof(single_row_count_array));


  pixel_group.mapped_array_data.length = ArrayCountUntilNull(editable_mapped_array_data_array, (uint8_t)D_MAPPED_ARRAY_DATA_MAXIMUM_LENGTH); //values before 0

DEBUG_LINE;
  #endif // ENABLE_PIXEL_FUNCTION_PIXELGROUPING


}







#ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK

uint8_t mAnimatorLight::ConstructJSON_Debug_Animations_Progress(uint8_t json_level)
{

  if(anim_progress_mqtt_function_callback)
  {
    JsonBuilderI->Start();
    anim_progress_mqtt_function_callback(); // Call the function
    return JsonBuilderI->End();
  }
  return 0; // None to send

}

#endif // USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK





/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/********************* Helper Functions ie rarely edited ************************************/
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/

const char* mAnimatorLight::GetAnimationStatusCtr(char* buffer, uint8_t buflen){

  if(pCONT_iLight->animator_controller->IsAnimating()){
    snprintf_P(buffer, buflen, PSTR("Animating"));
    return buffer;
  }
  if(_segments[0].flags.fEnable_Animation){
    // (millis since) + (next event millis)
    int16_t until_next_millis = _segments[0].transition.rate_ms-(millis()-pCONT_iLight->runtime.animation_changed_millis);
    int16_t secs_until_next_event = until_next_millis/1000;
    // secs_until_next_event/=1000;
    // Serial.println(secs_until_next_event);

    // char float_ctr[10];
    // dtostrf(round(secs_until_next_event),3,1,float_ctr);
    // Serial.println(float_ctr);

    // AddLog(LOG_LEVEL_INFO,PSTR("GetAnimationStatusCtr %d %d"),
    //   until_next_millis,
    //   //millis(),pCONT_iLight->runtime.animation_changed_millis,pCONT_iLight->animation.transition.rate_ms,
    //   secs_until_next_event
    // );  
    
    if(secs_until_next_event>=0){
      snprintf_P(buffer, buflen, PSTR("Enabled (in %d secs)"), secs_until_next_event);//float_ctr);
    }else{
      snprintf_P(buffer, buflen, PSTR("Not currently running"));
      //  sprintf(buffer,PSTR("Not currently running"));//float_ctr);
    }

    // sprintf(ctr,PSTR("Enabled (%d secs)\0"),secs_until_next_event);
    // sprintf(ctr,PSTR("Enabled (123 secs)\0"),secs_until_next_event);
    // Serial.println(ctr);
    return buffer;
  }else{
    return D_CSTRING_ERROR_MESSAGE_CTR;
    // return "Paused";
  }
  return D_CSTRING_ERROR_MESSAGE_CTR;
}


/**
 * Alternate SetPixelColor to make WLED conversion easier, but to be phased out later
 */

void mAnimatorLight::SetPixelColor(uint16_t indexPixel, uint8_t red, uint8_t green, uint8_t blue, uint16_t segment_length)
{
  SetPixelColor(indexPixel, RgbColor(red,green,blue));
}
  
void mAnimatorLight::SetPixelColor(uint16_t indexPixel, uint32_t color, uint16_t segment_length)
{
  RgbcctColor col = RgbcctColor(0);
  col.red =   (color >> 16 & 0xFF);
  col.green = (color >> 8  & 0xFF);
  col.blue =  (color       & 0xFF);

  // AddLog(LOG_LEVEL_DEBUG, PSTR("color=%d\t%X"),indexPixel,color);

  SetPixelColor(indexPixel, col);
}


void mAnimatorLight::SetPixelColor(uint16_t indexPixel, RgbcctColor color_internal, uint16_t segment_length)
{

  #ifdef ENABLE_FEATURE_PIXEL_GROUP_MULTIPLIERS
if(indexPixel<10)
{
    AddLog(LOG_LEVEL_DEBUG, PSTR("RGB(%d)=%d,%d,%d"),indexPixel,color_internal.R, color_internal.G, color_internal.B);
}
  #endif // ENABLE_FEATURE_PIXEL_GROUP_MULTIPLIERS


  // #ifdef ENABLE_FEATURE_PIXEL_GROUP_MULTIPLIERS
  // switch(indexPixel)
  // {
  //   default:
  //   case 0: color_internal = RgbcctColor(0,255,0,0,0); break;
  // //   case 1: color_internal = RgbcctColor(255,0,0,0,0); break;
  // //   case 2: color_internal = RgbcctColor(0,0,255,0,0); break;
  // //   case 3: color_internal = RgbcctColor(255,255,0,0,0); break;
  //   case 4: color_internal = RgbcctColor(0,255,255,0,0); break;
  // }
  // #endif // ENABLE_FEATURE_PIXEL_GROUP_MULTIPLIERS

  RgbcctColor color_hardware = color_internal;

  HARDWARE_ELEMENT_COLOUR_ORDER order = _segments[segment_length].hardware_element_colour_order;

  if(order.r != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){
    color_hardware[order.r] = color_internal.R;
  }
  if(order.g != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){
    color_hardware[order.g] = color_internal.G;
  }
  if(order.b != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){
    color_hardware[order.b] = color_internal.B;
  }
  if(order.w != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){
    color_hardware[order.w] = color_internal.WW;
  }
  if(order.c != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){
    color_hardware[order.c] = color_internal.WC;
  }

// DEBUG_LINE_HERE;
  //AddLog(LOG_LEVEL_DEBUG,PSTR("pixel[%d] = %d,%d,%d"),indexPixel,color_hardware.R,color_hardware.G,color_hardware.B);//,color_hardware.W);

  // Output methods here
  // Single: Direct pixel indexing, the default method (i.e., pixel index is exact)
  // Repeated: X pixels repeated after each other (i.e., the same repeated amount, given by a single multiplier red for 10, green for 10) (will require count index)
  // Index Ranges: Using a preset index where pixel_index is shown between ranges (i.e., red for 10 pixels, green for 1 pixel) (passed with a pointer to array, enabling different array for different bit mapping)
  // Using percentage linear blend between "Index Ranges" to allow not only horizontal tree lines, but gradients on each line

  #ifdef ENABLE_FEATURE_PIXEL_GROUP_MULTIPLIERS

    if(indexPixel == 0){
      setpixel_variable_index_counter = 0; // reset
      pixel_group.mapped_array_data.index = 0;
    }

    if(setpixel_variable_index_counter>50){ return; } // @note: The "50" is the largest repeated pixel

    if(pixel_group.flags.fEnabled)
    {

      uint8_t pixel_multiplier_count = 0;
      switch(pixel_group.flags.multiplier_mode_id){
        case PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_UNIFORM_ID:
          pixel_multiplier_count = pixel_group.multiplier;
            // AddLog(LOG_LEVEL_WARN, PSTR("PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_UNIFORM_ID %d"),pixel_multiplier_count);
          break;
        //case PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_RANDOM_ID:
          //pixel_multiplier_count = random(1,pixel_group.multiplier);
          //break;
        case PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID:
          if(pixel_group.mapped_array_data.values != nullptr)
          {
            // if any match, then jump index of lights forward
            uint8_t check_index = pixel_group.mapped_array_data.index;
            pixel_multiplier_count = pixel_group.mapped_array_data.values[check_index];
            
            AddLog(LOG_LEVEL_WARN, PSTR("PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID %d"),pixel_multiplier_count);
            pixel_group.mapped_array_data.index++;
            if(pixel_group.mapped_array_data.index >= 15){//pixel_group.mapped_array_data.length){
              pixel_group.mapped_array_data.index = 0;
            }

          }else{
            pixel_multiplier_count = pixel_group.multiplier;
          }
          break;
      }

      for(uint8_t ii = 0; ii < pixel_multiplier_count; ii++){
        if(setpixel_variable_index_counter>50){ return; }
        pCONT_iLight->SetPixelColourHardwareInterface(color_hardware,setpixel_variable_index_counter++);  
      }

    }else{
      pCONT_iLight->SetPixelColourHardwareInterface(color_hardware,indexPixel);
    } // pixel_group.flags.fEnabled

    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("indexPixel=%d,setpixel_variable_index_counterhere=%d"),indexPixel,setpixel_variable_index_counter);

  #else

    // pCONT_iLight->SetPixelColourHardwareInterface(indexPixel,color_hardware);
    pCONT_iLight->SetPixelColourHardwareInterface(color_hardware, indexPixel);

    //refer back to lighting interface which will pass to the right hardware

  #endif // ENABLE_FEATURE_PIXEL_GROUP_MULTIPLIERS

}

RgbcctColor mAnimatorLight::GetPixelColor(uint16_t indexPixel)
{
  DEBUG_LINE;
  if(stripbus == nullptr){    
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "stripbus == nullptr"));
    #endif
  }

  // Temp fix for WLED animations
  if(indexPixel > STRIP_SIZE_MAX)
  {
    return RgbcctColor(0);
  }

  
  #ifdef ENABLE_FEATURE_PIXEL_GROUP_MULTIPLIERS

  RgbcctColor color_hardware;

  // THIS IS ALL WRONG, ITS SETTING, NOT GETTING.

  /**
   * Depending on the mode, get must search differently, or, is get a simple "get?"
   * */

    // #ifdef USE_DEVFEATURE_PIXEL_OUTPUT_MULTIPLIER
    // indexPixel *= USE_DEVFEATURE_PIXEL_OUTPUT_MULTIPLIER;
    // #endif
  //   if(indexPixel == 0){
  //     setpixel_variable_index_counter = 0; // reset
  //     pixel_group.mapped_array_data.index = 0;
  //   }

  //   if(setpixel_variable_index_counter>50){ return RgbcctColor(0); }

    if(pixel_group.flags.fEnabled)
    {

      uint8_t pixel_multiplier_count = 0;
      switch(pixel_group.flags.multiplier_mode_id){
        case PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_UNIFORM_ID:
          // pixel_multiplier_count = pixel_group.multiplier;

          /***
           * Since the pixels are multipled out
           * 
           * **/


/**
 * probably too slow, but should work
 * */
// int converted_index = 0;
// for(int count=0;count<indexPixel; count++)
// {
//   if((count%pixel_group.multiplier)==0) // if remainder is 0, increment the pixel converted
//   {

//     converted_index++;

//   }wrong

// }



// AddLog(LOG_LEVEL_DEBUG, PSTR("indexPixel=%d -> %d"),indexPixel,converted_index);


// indexPixel /= pixel_group.multiplier; // Read the multipled region
color_hardware = pCONT_iLight->GetPixelColourHardwareInterface(indexPixel);

          break;
  //       case PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_RANDOM_ID:
  //         pixel_multiplier_count = random(1,pixel_group.multiplier);
  //         break;
  //       case PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID:
  //         if(pixel_group.mapped_array_data.values != nullptr){
  //           // if any match, then jump index of lights forward
  //           uint8_t check_index = pixel_group.mapped_array_data.index;
  //           pixel_multiplier_count = pixel_group.mapped_array_data.values[check_index];
            
  //           //AddLog(LOG_LEVEL_WARN, PSTR("PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID %d"),pixel_multiplier_count);
  //           pixel_group.mapped_array_data.index++;
  //           if(pixel_group.mapped_array_data.index >= 15){//pixel_group.mapped_array_data.length){
  //             pixel_group.mapped_array_data.index = 0;
  //           }

          // }else{
          //   pixel_multiplier_count = pixel_group.multiplier;
          // }
          // break;
      }

  //     for(uint8_t ii = 0; ii < pixel_multiplier_count; ii++){
  //       if(setpixel_variable_index_counter>50){ return RgbcctColor(0); }

  //       pCONT_iLight->SetPixelColourHardwareInterface(color_hardware,setpixel_variable_index_counter++);  
  //     }

    }else{
      color_hardware = pCONT_iLight->GetPixelColourHardwareInterface(indexPixel);
    } // pixel_group.flags.fEnabled

  //   AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("indexPixel=%d,setpixel_variable_index_counterhere=%d"),indexPixel,setpixel_variable_index_counter);

    // RgbcctColor color_hardware = pCONT_iLight->GetPixelColourHardwareInterface(indexPixel);

  #else

    RgbcctColor color_hardware = pCONT_iLight->GetPixelColourHardwareInterface(indexPixel);

  #endif

  RgbcctColor color_internal = color_hardware; // To catch white element if present
  DEBUG_LINE;

  // switch (pCONT_iLight->settings.pixel_hardware_color_order_id){
  //   case  PIXEL_HARDWARE_COLOR_ORDER_GRB_ID:  //0 = GRB, default
  //     color_internal.G = color_hardware.G; 
  //     color_internal.R = color_hardware.R; 
  //     color_internal.B = color_hardware.B; 
  //   break;
  //   case  PIXEL_HARDWARE_COLOR_ORDER_RGB_ID:  //1 = RGB, common for WS2811
  //     color_internal.R = color_hardware.G; 
  //     color_internal.G = color_hardware.R; 
  //     color_internal.B = color_hardware.B; 
  //   break;
  //   case  2: color_internal.B = color_hardware.G; color_hardware.R = color_hardware.R; color_internal.G = color_hardware.B; break; //2 = BRG
  //   case  3: color_internal.R = color_hardware.G; color_internal.B = color_hardware.R; color_internal.G = color_hardware.B; break; //3 = RBG
  //   case  4: color_internal.B = color_hardware.G; color_internal.G = color_hardware.R; color_internal.R = color_hardware.B; break; //4 = BGR
  //   default: color_internal.G = color_hardware.G; color_internal.B = color_hardware.R; color_internal.R = color_hardware.B; break; //5 = GBR
  // }
  // mInterfaceLight::HARDWARE_ELEMENT_COLOUR_ORDER order = pCONT_iLight->hardware_element_colour_order;

  HARDWARE_ELEMENT_COLOUR_ORDER order = _segments[0].hardware_element_colour_order;

  // if(indexPixel<100){
    // order = pCONT_iLight->hardware_element_colour_order;
  // }else{
  //   order = pCONT_iLight->hardware_element_colour_order[1];
  // }

  if(order.r != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){
    color_internal[order.r] = color_hardware.R;
  }
  if(order.g != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){
    color_internal[order.g] = color_hardware.G;
  }
  if(order.b != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){
    color_internal[order.b] = color_hardware.B;
  }
  if(order.w != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){
    color_internal[order.w] = color_hardware.WW;
  }
  if(order.c != D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE){
    color_internal[order.c] = color_hardware.WC;
  }




  return color_internal;
}



/*******************************************************************************************************************
********************************************************************************************************************
************CODE ABOVE THIS LINE IS PART OF THE NEWEST REWRITE********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/




/*******************************************************************************************************************
********************************************************************************************************************
************ANIMATION AND BLENDING**************************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/


RgbcctColor mAnimatorLight::ApplyBrightnesstoRgbcctColour(RgbcctColor full_range_colour, uint8_t brightness){

  RgbcctColor colour_adjusted_with_brightness = RgbcctColor(0);
  colour_adjusted_with_brightness.R  = mapvalue(full_range_colour.R,  0,255, 0,brightness);
  colour_adjusted_with_brightness.G  = mapvalue(full_range_colour.G,  0,255, 0,brightness);
  colour_adjusted_with_brightness.B  = mapvalue(full_range_colour.B,  0,255, 0,brightness);
  colour_adjusted_with_brightness.WW = mapvalue(full_range_colour.WW, 0,255, 0,brightness);
  colour_adjusted_with_brightness.WC = mapvalue(full_range_colour.WC, 0,255, 0,brightness);
  return colour_adjusted_with_brightness;

}

RgbcctColor mAnimatorLight::ApplyBrightnesstoRgbcctColour(RgbcctColor full_range_colour, uint8_t brightnessRGB, uint8_t brightnessCCT){

  RgbcctColor colour_adjusted_with_brightness = RgbcctColor(0);
  colour_adjusted_with_brightness.R  = mapvalue(full_range_colour.R,  0,255, 0,brightnessRGB);
  colour_adjusted_with_brightness.G  = mapvalue(full_range_colour.G,  0,255, 0,brightnessRGB);
  colour_adjusted_with_brightness.B  = mapvalue(full_range_colour.B,  0,255, 0,brightnessRGB);
  colour_adjusted_with_brightness.WW = mapvalue(full_range_colour.WW, 0,255, 0,brightnessCCT);
  colour_adjusted_with_brightness.WC = mapvalue(full_range_colour.WC, 0,255, 0,brightnessCCT);
  return colour_adjusted_with_brightness;

}


RgbcctColor mAnimatorLight::ApplyBrightnesstoDesiredColourWithGamma(RgbcctColor full_range_colour, uint8_t brightness){
  
  uint8_t new_brightness_255 = 0;

// Create new function of ApplyBrightnesstoDesiredColour to do this if statement generically
// if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){


  if(_segments[0].flags.Apply_Upper_And_Lower_Brightness_Randomly_Ranged_To_Palette_Choice)
  {

    #ifdef USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
    uint8_t temp_max_brightness = flashersettings.brightness_max;
    #else
    uint8_t temp_max_brightness = 255;
    #endif // USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION

    if(_segments[0].flags.Limit_Upper_Brightness_With_BrightnessRGB)
    {
      temp_max_brightness = pCONT_iLight->getBriRGB_Global();
    }
    #ifdef USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
    new_brightness_255 = random(flashersettings.brightness_min, temp_max_brightness);
    #else
    new_brightness_255 = random(0, temp_max_brightness);

    #endif // USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
  }
  // Default: apply global brightness
  else{
    new_brightness_255 = brightness;
  }

  // AddLog(LOG_LEVEL_TEST, PSTR("ledGamma=new_brightness_255=%d - >%d"), 
  //   new_brightness_255, pCONT_iLight->ledGamma(new_brightness_255)
  // );

  #ifdef ENABLE_GAMMA_BRIGHTNESS_ON_DESIRED_COLOUR_GENERATION
  if(_segments[0].flags.use_gamma_for_brightness)
  {
    new_brightness_255 = pCONT_iLight->ledGamma(new_brightness_255);
  }
  #endif // ENABLE_GAMMA_BRIGHTNESS_ON_DESIRED_COLOUR_GENERATION

  return ApplyBrightnesstoRgbcctColour(full_range_colour, new_brightness_255);

}


// #ifdef ENABLE_PIXEL_GENERAL_PHASEDOUT_CODE_TO_BE_REMOVED_IF_NOT_NEEDED

// // optional fromcolor to merge "FadeToNewColour" and "FadeBetweenColours"
// // If unset (as defined in header) the pCONT_iLight->animation will blend from current pixel colours with getpixel
// // void mAnimatorLight::FadeToNewColour(RgbTypeColor targetColor, uint16_t _time_to_newcolour,  RgbTypeColor fromcolor){ 
// void mAnimatorLight::FadeToNewColour(RgbcctColor targetColor, uint16_t _time_to_newcolour,  RgbcctColor fromcolor){ 

//   #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
//   AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "FadeToNewColour"));
//   #endif
  
//     #ifdef ENABLE_LOG_LEVEL_INFO
//   AddLog(LOG_LEVEL_TEST, PSTR("RgbcctColor=%d,%d,%d,%d,%d"),targetColor.R,targetColor.G,targetColor.B,targetColor.WW,targetColor.WC);
//     #endif// ENABLE_LOG_LEVEL_INFO
  
//   if(NEO_ANIMATION_TIMEBASE == NEO_CENTISECONDS){
//     _time_to_newcolour /= 1000;// ms to seconds
//   }

// #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT

//   // Overwriting single pCONT_iLight->animation methods, set, then clear
//   if(pCONT_iLight->animation_override.time_ms){
//     _time_to_newcolour = pCONT_iLight->animation_override.time_ms;
//     pCONT_iLight->animation_override.time_ms = 0;//reset overwrite
//   }

// #else
//   // Overwriting single pCONT_iLight->animation methods, set, then clear
//   if(segment_animation_override.time_ms){
//     _time_to_newcolour = segment_animation_override.time_ms;
//     segment_animation_override.time_ms = 0;//reset overwrite
//   }

// #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
//   // AnimEaseFunction easing = NeoEase::CubicIn;  

//   // Translation
//   // RgbcctColor fromcolor_npb = RgbcctColor(0);
//   // if(fromcolor != RgbcctColor(0)){ //? 
//   //   fromcolor_npb = fromcolor;
//   // }
//   RgbcctColor targetColor_npb = targetColor;//RgbcctColor(targetColor.R,targetColor.G,targetColor.B);

//   #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
//   // AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("fromcolor_npb=%d,%d,%d"),fromcolor_npb.R,fromcolor_npb.G,fromcolor_npb.B);
//   // AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("targetColor_npb=%d,%d,%d"),targetColor_npb.R,targetColor_npb.G,targetColor_npb.B);
//   #endif // ENABLE_LOG_LEVEL_DEBUG_MORE

//   //load start
//   for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
//     animation_colours[pixel].StartingColor = GetPixelColor(pixel);
//     animation_colours[pixel].DesiredColour = targetColor_npb;
//     //AddLog(LOG_LEVEL_TEST, PSTR("targetColor_npb=%d,%d,%d"),targetColor_npb.R,targetColor_npb.G,targetColor_npb.B);
//   }

  
//   pCONT_iLight->animator_controller->StartAnimation(0, _time_to_newcolour, [this](const AnimationParam& param){ this->AnimationProcess_Generic_AnimationColour_LinearBlend(param);} );
  
// } // END function



// void mAnimatorLight::StartAnimationAsBlendFromStartingColorToDesiredColor(){ 
  
//   #ifdef ENABLE_LOG_LEVEL_DEBUG
//   //AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::StartAnimationAsBlendFromStartingColorToDesiredColor"));
//   #endif

//   pCONT_iLight->runtime.animation_changed_millis = millis();

//   uint16_t time_tmp = 0;
//   // switch(pCONT_iLight->animation.transition.method_id){
//   //   default: 
//   //   case TRANSITION_METHOD_INSTANT_ID: time_tmp = 0; break;
//   //   case TRANSITION_METHOD_BLEND_ID:   
//     time_tmp = 
// #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
//   pCONT_iLight->animation
// #else
//   pCONT_lAni->_segments[0]
// #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
// .transition.time_ms; 
//   //   break;
//   // }

// #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT

//   // Overwriting single pCONT_iLight->animation methods, set, then clear
//   if(pCONT_iLight->animation_override.time_ms){
//     time_tmp = pCONT_iLight->animation_override.time_ms;
//     pCONT_iLight->animation_override.time_ms = 0;//reset overwrite
//   }

// #else
//   // Overwriting single pCONT_iLight->animation methods, set, then clear
//   if(segment_animation_override.time_ms){
//     time_tmp = segment_animation_override.time_ms;
//     segment_animation_override.time_ms = 0;//reset overwrite
//   }

// #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT

//   if(time_tmp>0){
//     if(NEO_ANIMATION_TIMEBASE == NEO_CENTISECONDS){
//       time_tmp /= 1000;// ms to seconds
//     }
//   }

//   //load start
//   for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
//     animation_colours[pixel].StartingColor = GetPixelColor(pixel);
//     // animation_colours[pixel].DesiredColour = GetPixelColor(pixel); // set elsewhere
    
  
// //  AddLog(LOG_LEVEL_TEST, PSTR("targetColor_npb=%d,%d,%d %d,%d,%d"),animation_colours[pixel].StartingColor.R,animation_colours[pixel].StartingColor.G,animation_colours[pixel].StartingColor.B,
//   // AddLog(LOG_LEVEL_TEST, PSTR("DesiredColour=%d,%d,%d"),
//   // animation_colours[pixel].DesiredColour.R,animation_colours[pixel].DesiredColour.G,animation_colours[pixel].DesiredColour.B);
  
  
  
//   }


//   pCONT_iLight->animator_controller->StartAnimation(0, time_tmp, [this](const AnimationParam& param){ this->AnimationProcess_Generic_AnimationColour_LinearBlend(param);} );




// } //end function



// /**
//  * Begin at "StartingColor" at 0% then return to "DesiredColor" at 100%
//  * */
// void mAnimatorLight::StartAnimationAsSwitchingFromStartingColorToDesiredColor(){ 
  
//   #ifdef ENABLE_LOG_LEVEL_DEBUG
//   //AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::StartAnimationAsBlendFromStartingColorToDesiredColor"));
//   #endif

//   pCONT_iLight->runtime.animation_changed_millis = millis();

//   uint16_t time_tmp = 0;
//   // switch(pCONT_iLight->animation.transition.method_id){
//   //   default: 
//   //   case TRANSITION_METHOD_INSTANT_ID: time_tmp = 0; break;
//     // case TRANSITION_METHOD_BLEND_ID:  
//      time_tmp = 
// #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
//   pCONT_iLight->animation
// #else
//   pCONT_lAni->_segments[0]
// #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
// .transition.time_ms; 
//   //    break;
//   // }

 
// #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT

//   // Overwriting single pCONT_iLight->animation methods, set, then clear
//   if(pCONT_iLight->animation_override.time_ms){
//     time_tmp = pCONT_iLight->animation_override.time_ms;
//     pCONT_iLight->animation_override.time_ms = 0;//reset overwrite
//   }

// #else
//   // Overwriting single pCONT_iLight->animation methods, set, then clear
//   if(segment_animation_override.time_ms){
//     time_tmp = segment_animation_override.time_ms;
//     segment_animation_override.time_ms = 0;//reset overwrite
//   }

// #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT

//   if(time_tmp>0){
//     if(NEO_ANIMATION_TIMEBASE == NEO_CENTISECONDS){
//       time_tmp /= 1000;// ms to seconds
//     }
//   }

//   // Note: Loading of startingcolor and desiredcolor are done elsewhere

//   pCONT_iLight->animator_controller->StartAnimation(0, time_tmp, [this](const AnimationParam& param){ this->AnimUpdate_ShowStartingThenDesiredColors(param);} );


// } //end function


// #endif // ENABLE_PIXEL_GENERAL_PHASEDOUT_CODE_TO_BE_REMOVED_IF_NOT_NEEDED




#ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
mAnimatorLight& mAnimatorLight::setCallback_ConstructJSONBody_Debug_Animations_Progress(ANIMIMATION_DEBUG_MQTT_FUNCTION_SIGNATURE) {
  this->anim_progress_mqtt_function_callback = anim_progress_mqtt_function_callback;
  return *this;
}
#endif // USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK


#ifdef ENABLE_FEATURE_PIXEL_GROUP_MULTIPLIERS
bool mAnimatorLight::OverwriteUpdateDesiredColourIfMultiplierIsEnabled(){

  //Use starting pixel as temporary buffer, record desired as already set
  for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    animation_colours[pixel].StartingColor = animation_colours[pixel].DesiredColour;
  } // END for


  for(uint16_t indexPixel=0; indexPixel<pCONT_iLight->settings.light_size_count; indexPixel++){
    
    uint8_t pixel_multiplier_count = 0;
    if(indexPixel == 0){
      setpixel_variable_index_counter = 0; // reset
      pixel_group.mapped_array_data.index = 0;
    }

    if(setpixel_variable_index_counter>pCONT_iLight->settings.light_size_count-1){ return true; }

    if(pixel_group.flags.fEnabled){

      switch(pixel_group.flags.multiplier_mode_id){
        default:
        case PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_UNIFORM_ID:
          pixel_multiplier_count = pixel_group.multiplier;
          break;
        //case PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_RANDOM_ID:
          //pixel_multiplier_count = random(1,pixel_group.multiplier);
          //break;
        case PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID:
          if(pixel_group.mapped_array_data.values != nullptr){
            // if any match, then jump index of lights forward
            pixel_multiplier_count = pixel_group.mapped_array_data.values[pixel_group.mapped_array_data.index];
            //AddLog(LOG_LEVEL_WARN, PSTR("PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID %d %d %d"),pixel_multiplier_count,pixel_group.mapped_array_data.index,pixel_group.mapped_array_data.length);
            if(pixel_group.mapped_array_data.index++ >= pixel_group.mapped_array_data.length-1){
              pixel_group.mapped_array_data.index = 0;
            }
          }else{
            pixel_multiplier_count = 1;//pixel_group.multiplier;
          }
          break;
      }
  AddLog(LOG_LEVEL_TEST, PSTR("pixel_multiplier_count=%d"),pixel_multiplier_count);

      for(uint8_t ii = 0; ii < pixel_multiplier_count; ii++){
        animation_colours[setpixel_variable_index_counter++].DesiredColour = animation_colours[indexPixel].StartingColor;
        if(setpixel_variable_index_counter>pCONT_iLight->settings.light_size_count-1){ return true; }
      }

    }
    else{
      animation_colours[indexPixel].DesiredColour = animation_colours[indexPixel].StartingColor;
    } // pixel_group.flags.fEnabled

    AddLog(LOG_LEVEL_TEST, PSTR("indexPixel=%d,setpixel_variable_index_counterhere=%d %d"),indexPixel,setpixel_variable_index_counter,pixel_multiplier_count);

  }

  return true; // returns if this function changed the desiredcolour

}

  #endif // ENABLE_FEATURE_PIXEL_GROUP_MULTIPLIERS







void mAnimatorLight::SetPixelColor_All(RgbcctColor colour){
  //stripbus->ClearTo(0);
  for(uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    SetPixelColor(pixel, colour);
  }
  pCONT_iLight->ShowInterface();
}






/***
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 *    WEBPAGE
 * 
 * 
 * 
 * 
 * 
 * 
 */


// enum CTypes { CT_HTML, CT_PLAIN, CT_XML, CT_JSON, CT_STREAM };
// const char kContentTypes[] PROGMEM = "text/html|text/plain|text/xml|application/json|application/octet-stream";
  


void mAnimatorLight::SetRefreshLEDs(){

  
  segment_animation_override.fRefreshAllPixels = true;
  segment_animation_override.time_ms = 10;
  _segments[0].flags.fForceUpdate = true;

}











/*******************************************************************************************************************
********************************************************************************************************************
************MQTT**************************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/






// Final output, but check for power limit
void mAnimatorLight::StripUpdate(){

  // STRIP_SIZE_REPEAT_MAX

  // if(settings.strip_size_repeat_animation)

  // Calculate output pixel size
  // uint16_t pixel_output_length = pCONT_iLight->settings.light_size_count;
  // if(pCONT_iLight->settings.light_size_count>STRIP_SIZE_MAX){
  //   pixel_output_length = STRIP_SIZE_MAX;
  // }
  // // Replicate SetPixel for repeated output
  // #ifdef STRIP_REPEAT_OUTPUT_MAX
  // pixel_output_length = STRIP_REPEAT_OUTPUT_MAX;  
  // #endif // STRIP_REPEAT_OUTPUT_MAX



// AddLog(LOG_LEVEL_WARN, PSTR("Disabled Replicate SetPixel for repeated output"));
  // // Replicate SetPixel for repeated output
  // #ifdef STRIP_REPEAT_OUTPUT_MAX
  // int pixels_existing_index = 0;
  // for(int i=0;i<STRIP_REPEAT_OUTPUT_MAX;i++){
  //   SetPixelColor(i,GetPixelColor(pixels_existing_index++));
  //   if(pixels_existing_index>=STRIP_SIZE_MAX){ pixels_existing_index = 0;}
  // }
  // #endif // STRIP_REPEAT_OUTPUT_MAX



  
#ifdef ENABLE_PIXEL_OUTPUT_POWER_ESTIMATION
if(mTime::TimeReached(&tSavedCalculatePowerUsage,1000)){
// #ifdef USE_SPEED_TESTING
  uint8_t channel_count = 3;

//ADD TO ONLY Check power level every 1 second to improve speed

  RgbTypeColor c;
  // float estimated_power_new_mA = 0;
  // uint32_t last_power = 0;
  // float pixel_units = 0;
  // float this_power = 0;
  // for(ledout.index=0;ledout.index<ledout.length;ledout.index++){ 
  //   // colour_tmp = desired_colour[ledout.index];
  //   // estimated_power_new +=  WebColorFromColourType(desired_colour[ledout.index]) * channel_count * 20;
  //   c = desired_colour[ledout.index];
  //   pixel_units = c.R + c.G + c.B;
  //   this_power = (pixel_units * power_rating.Average_mA_Per_Pixel_Step;
  //   estimated_power_new_mA +=  this_power;
  //   //last_power = (c.R + c.G + c.B) * power_rating.Average_mA_Per_Pixel_Step;
  //   // AddLog(LOG_LEVEL_TEST,PSTR("%d.%d.%d\t%d\t%d\t%d\t%d"),
  //   //   c.R,c.G,c.B,
  //   //   (int)pixel_units,
  //   //   (int)this_power,
  //   //   (int)(pixel_units * power_rating.Average_mA_Per_Pixel_Step),
  //   //   (int)estimated_power_new_mA
  //   // );
  
  //   // estimated_power_new +=  colour_tmp.W * channel_count * 20;
  // }
  // AddLog(LOG_LEVEL_TEST,PSTR("%d.%d.%d\t%d\t%d\t%d\t%d"),
  //     c.R,c.G,c.B,
  //     (int)pixel_units,
  //     (int)this_power,
  //     (int)(pixel_units * power_rating.Average_mA_Per_Pixel_Step),
  //     (int)estimated_power_new_mA
  //   );

  float estimated_power_new_mA = 0;
  uint32_t power = 0;
  float pixel_units = 0;
  float this_power = 0;
  for(ledout.index=0;ledout.index<ledout.length;ledout.index++){ 
    // colour_tmp = desired_colour[ledout.index];
    // estimated_power_new +=  WebColorFromColourType(desired_colour[ledout.index]) * channel_count * 20;
    c = desired_colour[ledout.index];, 19
    power += c.R + c.G + c.B;
    // this_power = (pixel_units * power_rating.Average_mA_Per_Pixel_Step;
    // estimated_power_new_mA +=  this_power;
    //last_power = (c.R + c.G + c.B) * power_rating.Average_mA_Per_Pixel_Step;
    // AddLog(LOG_LEVEL_TEST,PSTR("%d.%d.%d\t%d\t%d\t%d\t%d"),
    //   c.R,c.G,c.B,
    //   (int)pixel_units,
    //   (int)this_power,
    //   (int)(pixel_units * power_rating.Average_mA_Per_Pixel_Step),
    //   (int)estimated_power_new_mA
    // );
  
    // estimated_power_new +=  colour_tmp.W * channel_count * 20;
  }
  // AddLog(LOG_LEVEL_TEST,PSTR("%d.%d.%d\t%d\t%d\t%d\t%d"),
  //     c.R,c.G,c.B,
  //     (int)pixel_units,
  //     (int)this_power,
  //     (int)(pixel_units * power_rating.Average_mA_Per_Pixel_Step),
  //     (int)estimated_power_new_mA
  //   );
  // Store current power usage
  power_rating.current_mA = (float)(power);// * power_rating.Average_mA_Per_Pixel_Step);//estimated_power_new_mA;
  power_rating.power = (power_rating.current_mA/1000)*5;  // /1000*5

  // AddLog(LOG_LEVEL_TEST,PSTR("Estimated Power Consumption = %d (mA) p%d"),(int)estimated_power_new_mA,(int)last_power);
  
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("Estimated Power Consumption = %d (mA)"),(int)estimated_power_new_mA);
    #endif
}
#endif // ENABLE_PIXEL_OUTPUT_POWER_ESTIMATION


  pCONT_iLight->ShowInterface();

  // for (auto copies = 0; copies < 3; copies++) {
  //   image.Blt(stripbus, copies * image.PixelCount());
  // }
  
  /**
   * Replicate output if enabled
   * 
   * */
  #ifdef STRIP_REPEAT_OUTPUT_MAX
  uint16_t internal_length = pCONT_iLight->settings.light_size_count;
  uint16_t external_length = STRIP_OUTPUT_REPEATED_LENGTH;

  uint16_t pixel_output_length = pCONT_iLight->settings.light_size_count;
  if(pCONT_iLight->settings.light_size_count>STRIP_SIZE_MAX){
    pixel_output_length = STRIP_SIZE_MAX;
  }
  // Replicate SetPixel for repeated output
  #ifdef STRIP_REPEAT_OUTPUT_MAX
  pixel_output_length = STRIP_REPEAT_OUTPUT_MAX;  
  #endif // STRIP_REPEAT_OUTPUT_MAX

  // AddLog(LOG_LEVEL_WARN, PSTR("Disabled Replicate SetPixel for repeated output"));
  // Replicate SetPixel for repeated output
  #ifdef STRIP_REPEAT_OUTPUT_MAX
  int pixels_existing_index = 0;
  for(int i=0;i<STRIP_REPEAT_OUTPUT_MAX;i++){
    SetPixelColor(i,GetPixelColor(pixels_existing_index++));
    if(pixels_existing_index>=STRIP_SIZE_MAX){ pixels_existing_index = 0;}
  }
  #endif // STRIP_REPEAT_OUTPUT_MAX
  #endif // STRIP_REPEAT_OUTPUT_MAX


}






uint8_t mAnimatorLight::ConstructJSON_Settings(uint8_t json_method){

#ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS




return 0;
#endif

  // D_DATA_BUFFER_CLEAR();
  // pCONT_sup->WriteBuffer_P(data_buffer.payload.ctr,
  //   PSTR("{"
  //     "\"" "ledout"       "\":%d,"
  //     "\"" "pCONT_iLight->settings.light_size_count"        "\":%d,"
  //     "\"" "STRIP_SIZE_MAX"          "\":%d,"
  //     "\"" "ANIMATOR_SIZE_MAX"         "\":%d"
  //   "}"),
  //   ledout.length,
  //   pCONT_iLight->settings.light_size_count,
  //   STRIP_SIZE_MAX,
  //   ANIMATOR_SIZE_MAX
  // );

  // return strlen(data_buffer.payload.ctr) ? 1 : 0;

  JBI->Start();

    // JBI->Add("ledout_length", ledout.length);
    JBI->Add("light_size_count", pCONT_iLight->settings.light_size_count);

  return JBI->End();

}






uint8_t mAnimatorLight::ConstructJSON_Animation(uint8_t json_method){

  #ifdef ENABLE_LOG_LEVEL_DEBUG
  // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::ConstructJSON_Animation"));
  #endif

#ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
return 0;
#endif

  // StaticJsonDocument<600> doc;
  // JsonObject root = doc.to<JsonObject>();

  // root[D_JSON_ONOFF] = pCONT_iLight->light_power_state ? "ON" : "OFF";
  // root[D_JSON_MODE] = GetAnimationModeName();
  // root[D_JSON_COLOUR_PALETTE] = GetPaletteFriendlyName();
  // // root[D_JSON_BRIGHTNESS_PERCENTAGE] = pCONT_iLight->animation.brightness*100;
  // root[D_JSON_BRIGHTNESS] = pCONT_iLight->animation.brightness;


  // // Flags and states that are used during one transition and reset when completed
  // // JsonObject overridesobj = root.createNestedObject("transition_overrides"); 
  // //   overridesobj["fRefreshAllPixels"] = pCONT_iLight->animation_override.fRefreshAllPixels;
  
  // // root["ledout.length"] = ledout.length;

  // data_buffer.payload.len = measureJson(root)+1;
  // serializeJson(doc,data_buffer.payload.ctr);

  // return 0;

  
  JBI->Start();

    JBI->Add("light_power_state", pCONT_iLight->light_power_state ? "ON" : "OFF");
    // JBI->Add("light_size_count", pCONT_iLight->settings.light_size_count);

    JBI->Level_Start(D_JSON_TRANSITION);
    //   transitionobj[D_JSON_METHOD] = GetTransitionMethodName();
    //   // transitionobj[D_JSON_TIME] = mSupport::safeDivideInt(pCONT_iLight->animation.transition.time_ms.val,1000);
      JBI->Add(D_JSON_TIME_MS, _segments[0].transition.time_ms);
    //   transitionobj[D_JSON_TIME_MS] = ;
    //   // transitionobj[D_JSON_RATE] = mSupport::safeDivideInt(pCONT_iLight->animation.transition.rate_ms,1000);
    //   transitionobj[D_JSON_RATE_MS] = pCONT_iLight->animation.transition.rate_ms;
    //   // transitionobj[D_JSON_PIXELS_UPDATE_NUMBER] = GetPixelsToUpdateAsNumberFromPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val);
    //   transitionobj[D_JSON_PIXELS_UPDATE_PERCENTAGE] = pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val;
    //   transitionobj[D_JSON_ORDER] = GetTransitionOrderName();
    JBI->Level_End();




  return JBI->End();


}



uint8_t mAnimatorLight::ConstructJSON_Ambilight(uint8_t json_level){
  // Awaiting total redesign
  
  // #ifdef ENABLE_LOG_LEVEL_DEBUG
  // AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "f::ConstructJSON_Ambilight"));
  // #endif

  JBI->Start();

    // JBI->Add("light_power_state", pCONT_iLight->light_power_state ? "ON" : "OFF");
    // JBI->Add("light_size_count", pCONT_iLight->settings.light_size_count);

  return JBI->End();
}





uint8_t mAnimatorLight::ConstructJSON_State(uint8_t json_level){

#ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
return 0;
#endif

  uint8_t numpixels = pCONT_iLight->settings.light_size_count<100?pCONT_iLight->settings.light_size_count:100;
  RgbTypeColor c;

  char buffer[100];
  
  JsonBuilderI->Start();  
    JsonBuilderI->Add_P(PM_JSON_SIZE, pCONT_iLight->settings.light_size_count);
    JBI->Add("PaletteMaxID", (uint8_t)mPalette::PALETTELIST_STATIC_LENGTH_ID);
    JBI->Add("ColourPaletteID", pCONT_lAni->_segments[0].palette.id );
    JBI->Add("ColourPalette", mPaletteI->GetPaletteNameByID( _segments[0].palette.id, buffer, sizeof(buffer)));
    // JsonBuilderI->Array_Start("rgb");
    // for(int i=0;i<numpixels;i++){
    //   RgbTypeColor c = GetPixelColor(i);
    //   JsonBuilderI->Add_FV(PSTR("%02X%02X%02X"),c.R,c.G,c.B);
    // }
    // JsonBuilderI->Array_End();
  return JsonBuilderI->End();

}




//     #ifndef ENABLE_DEVFEATURE_PHASE_OUT_ANIMATIONCOLOUR_STRUCT
// void mAnimatorLight::Draw_DesiredColour_LinearGradientMirrored(RgbcctColor colour_center, 
//                                                  RgbcctColor colour_circumference, 
//                                                  uint16_t radius_pixel,
//                                                  uint16_t raduis_size,
//                                                  uint8_t center_repeat_size,
//                                                  bool colour_is_additive,
//                                                  bool values_are_percentage_of_strip_size
// ){


//       RgbcctColor colour_draw = RgbcctColor(0);
//   // animation_colours[index_generate].DesiredColour

//         //Draw test background
//          for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
//           animation_colours[index].DesiredColour = RgbcctColor(0,0,30,0,0);//,0,10,0,0);
//         }

//         // //Draw test background
//         //  for(uint16_t index=20; index<pCONT_iLight->settings.light_size_count-20; index++ ){
//         //   animation_colours[index].DesiredColour = RgbcctColor(0,0,10,0,0);
//         // }

//         // Draw circle
//         // Center pixel, then radius is by precentage of strop size
//         uint16_t index2 = 0;
//         //Get circle start 
//         // uint16_t index_circle_center = 50;
//         float progress = 0;
//         uint16_t start_pixel_position = 0;
//         uint16_t end_pixel_position = 10;
//         RgbcctColor colour_circle_center = RgbcctColor(D_RGB255_ORANGE_LIGHT 0,0);
//         RgbcctColor colour_circle_circumference = RgbcctColor(0,0,0,0,0);

        
//         // AddLog(LOG_LEVEL_TEST,PSTR("radius_pixel=%d"),radius_pixel);

//         //Make function to draw symbols, by passing colour stream
//         //use signed value to hold index_adjusted
//         int32_t index_generate = 0;
//         uint16_t pixel_position_center, pixel_position_circum, pixel_radius_as_number;

//         //Rethink, don't consider the output, just X pixels

//         pixel_radius_as_number = 30;
//         uint16_t pixel_center_repeated_as_number = 3; // the repeated colours before blending out
//         pixel_position_center = radius_pixel;
//         pixel_position_circum = radius_pixel+pixel_radius_as_number;
//         for(uint16_t index=0; index<pixel_radius_as_number; index++ ){

//           progress = mSupport::mapfloat(index,0,pixel_radius_as_number,0,1);

//           //Draw half, larger index side
//           colour_draw = RgbcctColor::LinearBlend(
//                                         colour_circle_center,
//                                         colour_circle_circumference, 
//                                         //RgbColor(255,0,0), RgbColor(0,255,0)
//                                         progress);

                                        
//           Serial.println(colour_draw.R);

//           //Only write into colour buffer if valid index. Otherwise, consider this an "overscan"
          
//           index_generate = radius_pixel+index;  
//           if((index_generate>=0)&&(index_generate<100)){
//             animation_colours[index_generate].DesiredColour += colour_draw;
//           } // if test
          
//           index_generate = radius_pixel-index;  
//           if((index_generate>=0)&&(index_generate<100)){
//             animation_colours[index_generate].DesiredColour += colour_draw;
//           } // if test

//         }



// }


// void mAnimatorLight::Draw_DesiredColour_LinearGradientMirrored2(
//                                                 RgbcctColor colour_center, 
//                                                 RgbcctColor colour_circumference, 
//                                                 uint16_t radius_pixel,
//                                                 uint16_t radius_size, //tail size
//                                                 uint8_t center_repeat_size,
//                                                 bool colour_is_additive,
//                                                 bool values_are_percentage_of_strip_size
// ){

//   RgbcctColor colour_draw = RgbcctColor(0);
//   // animation_colours[index_generate].DesiredColour

//   //Draw test background
//   //   for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
//   //   animation_colours[index].DesiredColour = RgbcctColor(0);//,0,30,0,0);//,0,10,0,0);
//   // }

//   // //Draw test background
//   //  for(uint16_t index=20; index<pCONT_iLight->settings.light_size_count-20; index++ ){
//   //   animation_colours[index].DesiredColour = RgbcctColor(0,0,10,0,0);
//   // }

//   // Draw circle
//   // Center pixel, then radius is by precentage of strop size
//   float progress = 0;
//   int32_t index_generate = 0;
  
//   uint16_t pixel_index_lower = radius_pixel-center_repeat_size;
//   uint16_t pixel_index_upper = radius_pixel+center_repeat_size;

//   // Draw repeated center region
//   for(uint16_t index=0; index<radius_size; index++ ){

//     index_generate = radius_pixel+index;  
//     if((index_generate>=0)&&(index_generate<100)){ animation_colours[index_generate].DesiredColour += colour_draw; } // if test

//     index_generate = radius_pixel-index;  
//     if((index_generate>=0)&&(index_generate<100)){ animation_colours[index_generate].DesiredColour += colour_draw; } // if test

//   }

//   // Draw tails
//   for(uint16_t index=0; index<radius_size; index++ ){

//     progress = mSupport::mapfloat(index,0,radius_size,0,1);

//     //Draw half, larger index side
//     colour_draw = RgbcctColor::LinearBlend(colour_center, colour_circumference, progress);

    
//     index_generate = pixel_index_lower+index;  
//     if((index_generate>=0)&&(index_generate<100)){ animation_colours[index_generate].DesiredColour += colour_draw; } // if test
    
//     index_generate = pixel_index_upper-index;  
//     if((index_generate>=0)&&(index_generate<100)){ animation_colours[index_generate].DesiredColour += colour_draw; } // if test

//   }

// }

// void mAnimatorLight::Remove_DesiredColour_LinearGradientMirrored(
//                                                 RgbcctColor colour_center, 
//                                                 RgbcctColor colour_circumference, 
//                                                 uint16_t radius_pixel,
//                                                 uint16_t radius_size, //tail size
//                                                 uint8_t center_repeat_size,
//                                                 bool colour_is_additive,
//                                                 bool values_are_percentage_of_strip_size
// ){

//   RgbcctColor colour_draw = RgbcctColor(0);
//   // animation_colours[index_generate].DesiredColour

//   //Draw test background
//   //   for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
//   //   animation_colours[index].DesiredColour = RgbcctColor(0);//,0,30,0,0);//,0,10,0,0);
//   // }

//   // //Draw test background
//   //  for(uint16_t index=20; index<pCONT_iLight->settings.light_size_count-20; index++ ){
//   //   animation_colours[index].DesiredColour = RgbcctColor(0,0,10,0,0);
//   // }

//   // Draw circle
//   // Center pixel, then radius is by precentage of strop size
//   float progress = 0;
//   int32_t index_generate = 0;
  
//   uint16_t pixel_index_lower = radius_pixel-center_repeat_size;
//   uint16_t pixel_index_upper = radius_pixel+center_repeat_size;

//   // Draw repeated center region
//   for(uint16_t index=0; index<radius_size; index++ ){

//     index_generate = radius_pixel+index;  
//     if((index_generate>=0)&&(index_generate<100)){ animation_colours[index_generate].DesiredColour -= colour_draw; } // if test

//     index_generate = radius_pixel-index;  
//     if((index_generate>=0)&&(index_generate<100)){ animation_colours[index_generate].DesiredColour -= colour_draw; } // if test

//   }

//   // Draw tails
//   for(uint16_t index=0; index<radius_size; index++ ){

//     progress = mSupport::mapfloat(index,0,radius_size,0,1);

//     //Draw half, larger index side
//     colour_draw = RgbcctColor::LinearBlend(colour_center, colour_circumference, progress);

    
//     index_generate = pixel_index_lower+index;  
//     if((index_generate>=0)&&(index_generate<100)){ animation_colours[index_generate].DesiredColour -= colour_draw; } // if test
    
//     index_generate = pixel_index_upper-index;  
//     if((index_generate>=0)&&(index_generate<100)){ animation_colours[index_generate].DesiredColour -= colour_draw; } // if test

//   }

// }

//     #endif // #ifndef ENABLE_DEVFEATURE_PHASE_OUT_ANIMATIONCOLOUR_STRUCT


















/******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
*** Subtask:   Mixer (for changing animations)   ***************************************************************************************************************************************************************************
**  @note:     to be renamed, "profiles?"  **************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************/





/******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
*** Animation Effect:   Notifications   ***************************************************************************************************************************************************************************
**  @note:     **************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************/





/******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
*** Animation Effect:   Ambilight   ***************************************************************************************************************************************************************************
**  @note:     **************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************/









/******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
*** Animation Effect:   WLED (Legacy)   ***************************************************************************************************************************************************************************
**  @note:    WLED initial port, running as its own effect. To be phased out into Segments **************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************/


/******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
*** Animation Effect:   Segments   ***************************************************************************************************************************************************************************
**  @note:    New effect that merges HACS and WLED with segments and dynamic animation storage **************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************/


// mAnimatorLight& mAnimatorLight::setAnimFunctionCallback_Segments(ANIM_FUNCTION_SIGNATURE_SEGMENT_INDEXED) {
//   // this->_segment_runtimes[segment_index].anim_function_callback = anim_function_callback;
//   // return *this;
// }

// // tmp method
// mAnimatorLight& mAnimatorLight::setAnimFunctionCallback_Segments_0(ANIM_FUNCTION_SIGNATURE_SEGMENT_INDEXED) {
//   uint8_t segment_index = 0;
//   // this->_segment_runtimes[segment_index].anim_function_callback_indexed = anim_function_callback;
//   return *this;
// }


#endif //USE_MODULE_LIGHTS_ANIMATOR




