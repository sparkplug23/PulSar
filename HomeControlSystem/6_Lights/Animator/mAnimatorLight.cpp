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
      #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
        init();
      #endif
      #ifdef ENABLE_PIXEL_FUNCTION_WLED_PHASEOUT
        Init_WLED();
      #endif // ENABLE_PIXEL_FUNCTION_WLED_PHASEOUT
      break;
    case FUNC_OVERRIDE_BOOT_INIT:

      // stripbus->NeoRgbCurrentSettings(1,2,3);

      #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
      mqtthandler_debug_animations_progress.tRateSecs = 1;
      #endif

    break;
  }
  
  if(!settings.flags.EnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID;}
  
  switch(function){
    /************
     * SETTINGS SECTION * 
    *******************/
    case FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE: 
      Settings_Load();
    break;
    case FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE: 
      Settings_Save();
    break;
    case FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES:
    case FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT:
      Settings_Default();
    break;
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_EVERY_SECOND:{
    
      //EverySecond();

      // const NeoRgbcctCurrentSettings settings(200,200,200,201,202);
      // uint32_t maTotal = stripbus->CalcTotalMilliAmpere(settings);

      #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
        char buffer[50];
        AddLog(LOG_LEVEL_TEST, PSTR("GetEffectsModeNamebyID=%s"), 
          pCONT_lAni->GetEffectsModeNamebyID(mEffects->getMode(),buffer,sizeof(buffer))
        );
      #endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

    }break;
    case FUNC_LOOP: 
      EveryLoop();

      #ifdef ENABLE_PIXEL_FUNCTION_WLED_PHASEOUT
        SubTask_WLED_Animation_PhaseOut();
      #endif // ENABLE_PIXEL_FUNCTION_WLED_PHASEOUT

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
  animator_strip_size = ANIMATOR_SIZE_MAX<=STRIP_SIZE_MAX?ANIMATOR_SIZE_MAX:STRIP_SIZE_MAX; 
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



void mAnimatorLight::init(void){ 
    
  ledout.length = STRIP_SIZE_MAX; 
  
  pCONT_iLight->settings.light_size_count = STRIP_SIZE_MAX;

  DEBUG_LINE;
  
  //step1:  moving the desired/starting colours into a buffer type, so it is dynamic
  //step2:  to become its own function, so strips can be changed at runtime
  // stripbus = new NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType>(strip_size_tmp, 23);//19); 3 = rx0

  Init_NeoPixelBus();
  
  
  uint16_t animator_strip_size_tmp = 1;//animator_strip_size<ANIMATOR_SIZE_MAX?animator_strip_size:ANIMATOR_SIZE_MAX; // Catch values exceeding limit

  pCONT_iLight->Init_NeoPixelAnimator(animator_strip_size_tmp, NEO_ANIMATION_TIMEBASE);  

  // pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val = 100;  
  SetLEDOutAmountByPercentage(100);//pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val);  

  pCONT_iLight->animation.flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique = false;
  pCONT_iLight->animation.flags.Limit_Upper_Brightness_With_BrightnessRGB = false;

  pCONT_iLight->animation.flags.Apply_Upper_And_Lower_Brightness_Randomly_Ranged_To_Palette_Choice = false; // FIX
  flashersettings.brightness_max = 255;
  flashersettings.brightness_min = 0;

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
  
  for(ledout.index=0;
    ledout.index<STRIP_SIZE_MAX;
    ledout.index++){ 
    ledout.pattern[ledout.index] = ledout.index; 
  }

  DEBUG_LINE;


  // Clear stored light output
  memset(&animation_colours,0,sizeof(animation_colours));
  // Start display
  stripbus->Begin();
  if(pCONT_set->Settings.flag_animations.clear_on_reboot){
    stripbus->ClearTo(0);
    pCONT_iLight->ShowInterface();
  }

  pCONT_iLight->animation_override.fRefreshAllPixels = true;

  GenerateAnimationPixelAmountMaps();


  #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
    init_Notifications();
  #endif
  #ifdef ENABLE_PIXEL_FUNCTION_AMBILIGHT
  blocking_force_animate_to_complete = true; //animate to completion on boot (for short animations)
  init_Ambilight();
  #endif // ENABLE_PIXEL_FUNCTION_AMBILIGHT
  #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
  init_flasher_settings();
  #endif
  #ifdef ENABLE_PIXEL_FUNCTION_MIXER
  init_mixer_defaults();
  #endif

  settings.flags.EnableModule = true;

  
  #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  resetSegments();
  #endif


  #ifdef USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS

  
  user_input.selected.palette_id = pCONT_set->Settings.animation_settings.xmas_controller_params[0];
  user_input.selected.brightness_id =  pCONT_set->Settings.animation_settings.xmas_controller_params[1];
  user_input.selected.effects_id =  pCONT_set->Settings.animation_settings.xmas_controller_params[2];
  user_input.selected.intensity_id = pCONT_set->Settings.animation_settings.xmas_controller_params[3];


  PhysicalController_Convert_IDs_to_Internal_Parameters();
  #endif

  // DrawTailPixels();

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

  pCONT_iLight->animation.mode_id = pCONT_set->Settings.animation_settings.animation_mode;
  pCONT_iLight->animation.palette.id = pCONT_set->Settings.animation_settings.animation_palette;
  pCONT_iLight->animation.transition.order_id   = pCONT_set->Settings.animation_settings.animation_transition_order;
  pCONT_iLight->animation.transition.method_id  = pCONT_set->Settings.animation_settings.animation_transition_method;
  pCONT_iLight->animation.transition.time_ms = pCONT_set->Settings.animation_settings.animation_transition_time_ms;
  pCONT_iLight->animation.transition.rate_ms = pCONT_set->Settings.animation_settings.animation_transition_rate_ms;
  pCONT_iLight->animation.intensity = 127;//pCONT_set->Settings.animation_settings.animation_transition_rate_ms;

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

  pCONT_set->Settings.animation_settings.animation_mode = pCONT_iLight->animation.mode_id;
  pCONT_set->Settings.animation_settings.animation_palette = pCONT_iLight->animation.palette.id;
  pCONT_set->Settings.animation_settings.animation_transition_order = pCONT_iLight->animation.transition.order_id;
  pCONT_set->Settings.animation_settings.animation_transition_method = pCONT_iLight->animation.transition.method_id;
  pCONT_set->Settings.animation_settings.animation_transition_time_ms = pCONT_iLight->animation.transition.time_ms;
  pCONT_set->Settings.animation_settings.animation_transition_rate_ms = pCONT_iLight->animation.transition.rate_ms;
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

    #ifdef DEFAULT_LIGHTING_LIVEVIEW_REFRESH_RATE_HZ
      liveview.refresh_rate = DEFAULT_LIGHTING_LIVEVIEW_REFRESH_RATE_HZ;
    #else
      pCONT_iLight->liveview.refresh_rate = 1000;
    #endif
    
    
      #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
    #ifdef DEFAULT_LIGHTING_EFFECTS_FUNCTION_ID
      flashersettings.function = 1;/*DEFAULT_LIGHTING_EFFECTS_FUNCTION_ID<EFFECTS_FUNCTION_LENGTH_ID?
                            DEFAULT_LIGHTING_EFFECTS_FUNCTION_ID:EFFECTS_FUNCTION_SLOW_GLOW_ID;*/
    #else
      flashersettings.function = EFFECTS_FUNCTION_SLOW_GLOW_ID;
    #endif
    #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
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


void mAnimatorLight::GenerateAnimationPixelAmountMaps(){

  // for(int val=0;val<sizeof(pixels_to_update_as_number_map);val++){
  //   pixels_to_update_as_number_map[val] = ((float)PROGMEM pixels_to_update_as_percentage_map[val]/100.0f)*pCONT_iLight->settings.light_size_count;
  //   if(pixels_to_update_as_number_map[val]==0) pixels_to_update_as_number_map[val] = 1;
  // }

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
  if(pCONT_iLight->animation.flags.fEnable_Animation){
    // (millis since) + (next event millis)
    int16_t until_next_millis = pCONT_iLight->animation.transition.rate_ms-(millis()-pCONT_iLight->runtime.animation_changed_millis);
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


void mAnimatorLight::SetPixelColor(uint16_t indexPixel, RgbcctColor color_internal)
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

  mInterfaceLight::HARDWARE_ELEMENT_COLOUR_ORDER order = pCONT_iLight->hardware_element_colour_order;

  // if(indexPixel<100){




    order = pCONT_iLight->hardware_element_colour_order;
  // }else{
  //   order = pCONT_iLight->hardware_element_colour_order[1];
  // }


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
  mInterfaceLight::HARDWARE_ELEMENT_COLOUR_ORDER order = pCONT_iLight->hardware_element_colour_order;


  // if(indexPixel<100){
    order = pCONT_iLight->hardware_element_colour_order;
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

void mAnimatorLight::ApplyBrightnesstoDesiredColour(uint8_t brightness){

  for(uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++ ){
    animation_colours[pixel].DesiredColour.R = mapvalue(animation_colours[pixel].DesiredColour.R, 0, 255, 0, brightness);
    animation_colours[pixel].DesiredColour.G = mapvalue(animation_colours[pixel].DesiredColour.G, 0, 255, 0, brightness);
    animation_colours[pixel].DesiredColour.B = mapvalue(animation_colours[pixel].DesiredColour.B, 0, 255, 0, brightness);
    // should be white channel here depending on type!
  }

}


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


  if(pCONT_iLight->animation.flags.Apply_Upper_And_Lower_Brightness_Randomly_Ranged_To_Palette_Choice){
    uint8_t temp_max_brightness = flashersettings.brightness_max;
    if(pCONT_iLight->animation.flags.Limit_Upper_Brightness_With_BrightnessRGB){
      temp_max_brightness = pCONT_iLight->getBriRGB_Global();
    }
    new_brightness_255 = random(flashersettings.brightness_min, temp_max_brightness);
  }
  // Default: apply global brightness
  else{
    new_brightness_255 = brightness;
  }

  // AddLog(LOG_LEVEL_TEST, PSTR("ledGamma=new_brightness_255=%d - >%d"), 
  //   new_brightness_255, pCONT_iLight->ledGamma(new_brightness_255)
  // );

  #ifdef ENABLE_GAMMA_BRIGHTNESS_ON_DESIRED_COLOUR_GENERATION
  if(pCONT_iLight->animation.flags.use_gamma_for_brightness){
    new_brightness_255 = pCONT_iLight->ledGamma(new_brightness_255);
  }
  #endif // ENABLE_GAMMA_BRIGHTNESS_ON_DESIRED_COLOUR_GENERATION

  return ApplyBrightnesstoRgbcctColour(full_range_colour, new_brightness_255);

}


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
  switch(pCONT_iLight->animation.transition.order_id){
    case TRANSITION_ORDER_INORDER_ID:

      SetLEDOutAmountByPercentage(100);

      for(ledout.index=0;ledout.index<ledout.length;ledout.index++){ 
        ledout.pattern[ledout.index] = ledout.index; 
      }


        // Serial.println("TRANSITION_ORDER_INORDER_ID");
      //for(int jj=0;jj<ledout.length;jj++){ Serial.printf("%d, ",ledout.pattern[jj]); } Serial.println();
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
    case TRANSITION_ORDER_CENTRE_OUT_ID:{
      // uint16_t middle_index = length_local/2;
      //0,1,2,3,4,5,4,3,2,1,0
      // for(ledout.index=0;ledout.index<length_local;ledout.index++){ 
      //   index_pattern[ledout.index] = ledout.index; 
      // }
    }
    break;
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


#endif // USE_DEVFEATURE_FUNCTION_UPGRADE_CORE_TRANSITIONMETHODS



// optional fromcolor to merge "FadeToNewColour" and "FadeBetweenColours"
// If unset (as defined in header) the pCONT_iLight->animation will blend from current pixel colours with getpixel
// void mAnimatorLight::FadeToNewColour(RgbTypeColor targetColor, uint16_t _time_to_newcolour,  RgbTypeColor fromcolor){ 
void mAnimatorLight::FadeToNewColour(RgbcctColor targetColor, uint16_t _time_to_newcolour,  RgbcctColor fromcolor){ 

  #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "FadeToNewColour"));
  #endif
  
    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_TEST, PSTR("RgbcctColor=%d,%d,%d,%d,%d"),targetColor.R,targetColor.G,targetColor.B,targetColor.WW,targetColor.WC);
    #endif// ENABLE_LOG_LEVEL_INFO
  
  if(NEO_ANIMATION_TIMEBASE == NEO_CENTISECONDS){
    _time_to_newcolour /= 1000;// ms to seconds
  }

  // Overwriting single pCONT_iLight->animation methods, set, then clear
  if(pCONT_iLight->animation_override.time_ms){
    _time_to_newcolour = pCONT_iLight->animation_override.time_ms;
    pCONT_iLight->animation_override.time_ms = 0;//reset overwrite
  }
  
  // AnimEaseFunction easing = NeoEase::CubicIn;  

  // Translation
  // RgbcctColor fromcolor_npb = RgbcctColor(0);
  // if(fromcolor != RgbcctColor(0)){ //? 
  //   fromcolor_npb = fromcolor;
  // }
  RgbcctColor targetColor_npb = targetColor;//RgbcctColor(targetColor.R,targetColor.G,targetColor.B);

  #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
  // AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("fromcolor_npb=%d,%d,%d"),fromcolor_npb.R,fromcolor_npb.G,fromcolor_npb.B);
  // AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("targetColor_npb=%d,%d,%d"),targetColor_npb.R,targetColor_npb.G,targetColor_npb.B);
  #endif // ENABLE_LOG_LEVEL_DEBUG_MORE

  //load start
  for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    animation_colours[pixel].StartingColor = GetPixelColor(pixel);
    animation_colours[pixel].DesiredColour = targetColor_npb;
    //AddLog(LOG_LEVEL_TEST, PSTR("targetColor_npb=%d,%d,%d"),targetColor_npb.R,targetColor_npb.G,targetColor_npb.B);
  }

  
  pCONT_iLight->animator_controller->StartAnimation(0, _time_to_newcolour, [this](const AnimationParam& param){ this->AnimationProcess_Generic_AnimationColour_LinearBlend(param);} );
  
} // END function



void mAnimatorLight::StartAnimationAsBlendFromStartingColorToDesiredColor(){ 
  
  #ifdef ENABLE_LOG_LEVEL_DEBUG
  //AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::StartAnimationAsBlendFromStartingColorToDesiredColor"));
  #endif

  pCONT_iLight->runtime.animation_changed_millis = millis();

  uint16_t time_tmp = 0;
  // switch(pCONT_iLight->animation.transition.method_id){
  //   default: 
  //   case TRANSITION_METHOD_INSTANT_ID: time_tmp = 0; break;
  //   case TRANSITION_METHOD_BLEND_ID:   
    time_tmp = pCONT_iLight->animation.transition.time_ms; 
  //   break;
  // }

  // Overwriting single pCONT_iLight->animation methods, set, then clear
  if(pCONT_iLight->animation_override.time_ms){
    time_tmp = pCONT_iLight->animation_override.time_ms;
    pCONT_iLight->animation_override.time_ms = 0;//reset overwrite
  }

  if(time_tmp>0){
    if(NEO_ANIMATION_TIMEBASE == NEO_CENTISECONDS){
      time_tmp /= 1000;// ms to seconds
    }
  }

  //load start
  for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    animation_colours[pixel].StartingColor = GetPixelColor(pixel);
    // animation_colours[pixel].DesiredColour = GetPixelColor(pixel); // set elsewhere
    
  
//  AddLog(LOG_LEVEL_TEST, PSTR("targetColor_npb=%d,%d,%d %d,%d,%d"),animation_colours[pixel].StartingColor.R,animation_colours[pixel].StartingColor.G,animation_colours[pixel].StartingColor.B,
  // AddLog(LOG_LEVEL_TEST, PSTR("DesiredColour=%d,%d,%d"),
  // animation_colours[pixel].DesiredColour.R,animation_colours[pixel].DesiredColour.G,animation_colours[pixel].DesiredColour.B);
  
  
  
  }


  pCONT_iLight->animator_controller->StartAnimation(0, time_tmp, [this](const AnimationParam& param){ this->AnimationProcess_Generic_AnimationColour_LinearBlend(param);} );




} //end function



/**
 * Begin at "StartingColor" at 0% then return to "DesiredColor" at 100%
 * */
void mAnimatorLight::StartAnimationAsSwitchingFromStartingColorToDesiredColor(){ 
  
  #ifdef ENABLE_LOG_LEVEL_DEBUG
  //AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::StartAnimationAsBlendFromStartingColorToDesiredColor"));
  #endif

  pCONT_iLight->runtime.animation_changed_millis = millis();

  uint16_t time_tmp = 0;
  // switch(pCONT_iLight->animation.transition.method_id){
  //   default: 
  //   case TRANSITION_METHOD_INSTANT_ID: time_tmp = 0; break;
    // case TRANSITION_METHOD_BLEND_ID:  
     time_tmp = pCONT_iLight->animation.transition.time_ms; 
  //    break;
  // }

  // Overwriting single pCONT_iLight->animation methods, set, then clear
  if(pCONT_iLight->animation_override.time_ms){
    time_tmp = pCONT_iLight->animation_override.time_ms;
    pCONT_iLight->animation_override.time_ms = 0;//reset overwrite
  }

  if(time_tmp>0){
    if(NEO_ANIMATION_TIMEBASE == NEO_CENTISECONDS){
      time_tmp /= 1000;// ms to seconds
    }
  }

  // Note: Loading of startingcolor and desiredcolor are done elsewhere

  pCONT_iLight->animator_controller->StartAnimation(0, time_tmp, [this](const AnimationParam& param){ this->AnimUpdate_ShowStartingThenDesiredColors(param);} );


} //end function





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
    time_tmp = pCONT_iLight->animation.transition.time_ms; 
  //   break;
  // }

  // Overwriting single pCONT_iLight->animation methods, set, then clear
  if(pCONT_iLight->animation_override.time_ms){
    time_tmp = pCONT_iLight->animation_override.time_ms;
    pCONT_iLight->animation_override.time_ms = 0;//reset overwrite
  }
  //clear forced once only flags
  if(pCONT_iLight->animation.flags.NewAnimationRequiringCompleteRefresh){
    pCONT_iLight->animation.flags.NewAnimationRequiringCompleteRefresh = false;    
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







void mAnimatorLight::TurnLEDsOff(){
  //stripbus->ClearTo(0);
  for(uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    SetPixelColor(pixel, 0);
  }
  pCONT_iLight->ShowInterface();
}


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

  pCONT_iLight->animation.flags.fForceUpdate = true;
  pCONT_iLight->animation_override.fRefreshAllPixels = true;
  pCONT_iLight->animation_override.time_ms = 10;

  // #ifndef ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT

  // // Additional flags to force scene updates
  // if(pCONT_iLight->animation.mode_id == ANIMATION_MODE_SCENE_ID){
  //   scene.name_id = MODE_SINGLECOLOUR_COLOURSCENE_ID;
  // }
  // #endif //ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT

}



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

  // D_DATA_BUFFER_CLEAR();
  // pCONT_sup->WriteBuffer_P(data_buffer.payload.ctr,
  //   PSTR("{"
  //     "\"" "ledout"       "\":%d,"
  //     "\"" "pCONT_iLight->settings.light_size_count"        "\":%d,"
  //     "\"" "STRIP_SIZE_MAX"          "\":%d,"
  //     "\"" "animator_strip_size"  "\":%d,"
  //     "\"" "ANIMATOR_SIZE_MAX"         "\":%d"
  //   "}"),
  //   ledout.length,
  //   pCONT_iLight->settings.light_size_count,
  //   STRIP_SIZE_MAX,
  //   animator_strip_size,
  //   ANIMATOR_SIZE_MAX
  // );

  // return strlen(data_buffer.payload.ctr) ? 1 : 0;

  JBI->Start();

    JBI->Add("ledout_length", ledout.length);
    JBI->Add("light_size_count", pCONT_iLight->settings.light_size_count);

  return JBI->End();

}






uint8_t mAnimatorLight::ConstructJSON_Animation(uint8_t json_method){

  #ifdef ENABLE_LOG_LEVEL_DEBUG
  // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::ConstructJSON_Animation"));
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
      JBI->Add(D_JSON_TIME_MS, pCONT_iLight->animation.transition.time_ms);
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

  uint8_t numpixels = pCONT_iLight->settings.light_size_count<100?pCONT_iLight->settings.light_size_count:100;
  RgbTypeColor c;

  char buffer[100];
  
  JsonBuilderI->Start();  
    JsonBuilderI->Add_P(PM_JSON_SIZE, pCONT_iLight->settings.light_size_count);
    JBI->Add("PaletteMaxID", (uint8_t)mPalette::PALETTELIST_STATIC_LENGTH_ID);
    JBI->Add("ColourPaletteID", pCONT_iLight->animation.palette.id );
    JBI->Add("ColourPalette", mPaletteI->GetPaletteNameByID(pCONT_iLight->animation.palette.id, buffer, sizeof(buffer)));
    // JsonBuilderI->Array_Start("rgb");
    // for(int i=0;i<numpixels;i++){
    //   RgbTypeColor c = GetPixelColor(i);
    //   JsonBuilderI->Add_FV(PSTR("%02X%02X%02X"),c.R,c.G,c.B);
    // }
    // JsonBuilderI->Array_End();
  return JsonBuilderI->End();

}




void mAnimatorLight::Draw_DesiredColour_LinearGradientMirrored(RgbcctColor colour_center, 
                                                 RgbcctColor colour_circumference, 
                                                 uint16_t radius_pixel,
                                                 uint16_t raduis_size,
                                                 uint8_t center_repeat_size,
                                                 bool colour_is_additive,
                                                 bool values_are_percentage_of_strip_size
){


      RgbcctColor colour_draw = RgbcctColor(0);
  // animation_colours[index_generate].DesiredColour

        //Draw test background
         for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
          animation_colours[index].DesiredColour = RgbcctColor(0,0,30,0,0);//,0,10,0,0);
        }

        // //Draw test background
        //  for(uint16_t index=20; index<pCONT_iLight->settings.light_size_count-20; index++ ){
        //   animation_colours[index].DesiredColour = RgbcctColor(0,0,10,0,0);
        // }

        // Draw circle
        // Center pixel, then radius is by precentage of strop size
        uint16_t index2 = 0;
        //Get circle start 
        // uint16_t index_circle_center = 50;
        float progress = 0;
        uint16_t start_pixel_position = 0;
        uint16_t end_pixel_position = 10;
        RgbcctColor colour_circle_center = RgbcctColor(D_RGB255_ORANGE_LIGHT 0,0);
        RgbcctColor colour_circle_circumference = RgbcctColor(0,0,0,0,0);

        
        // AddLog(LOG_LEVEL_TEST,PSTR("radius_pixel=%d"),radius_pixel);

        //Make function to draw symbols, by passing colour stream
        //use signed value to hold index_adjusted
        int32_t index_generate = 0;
        uint16_t pixel_position_center, pixel_position_circum, pixel_radius_as_number;

        //Rethink, don't consider the output, just X pixels

        pixel_radius_as_number = 30;
        uint16_t pixel_center_repeated_as_number = 3; // the repeated colours before blending out
        pixel_position_center = radius_pixel;
        pixel_position_circum = radius_pixel+pixel_radius_as_number;
        for(uint16_t index=0; index<pixel_radius_as_number; index++ ){

          progress = mSupport::mapfloat(index,0,pixel_radius_as_number,0,1);

          //Draw half, larger index side
          colour_draw = RgbcctColor::LinearBlend(
                                        colour_circle_center,
                                        colour_circle_circumference, 
                                        //RgbColor(255,0,0), RgbColor(0,255,0)
                                        progress);

                                        
          Serial.println(colour_draw.R);

          //Only write into colour buffer if valid index. Otherwise, consider this an "overscan"
          
          index_generate = radius_pixel+index;  
          if((index_generate>=0)&&(index_generate<100)){
            animation_colours[index_generate].DesiredColour += colour_draw;
          } // if test
          
          index_generate = radius_pixel-index;  
          if((index_generate>=0)&&(index_generate<100)){
            animation_colours[index_generate].DesiredColour += colour_draw;
          } // if test

        }



}


void mAnimatorLight::Draw_DesiredColour_LinearGradientMirrored2(
                                                RgbcctColor colour_center, 
                                                RgbcctColor colour_circumference, 
                                                uint16_t radius_pixel,
                                                uint16_t radius_size, //tail size
                                                uint8_t center_repeat_size,
                                                bool colour_is_additive,
                                                bool values_are_percentage_of_strip_size
){

  RgbcctColor colour_draw = RgbcctColor(0);
  // animation_colours[index_generate].DesiredColour

  //Draw test background
  //   for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
  //   animation_colours[index].DesiredColour = RgbcctColor(0);//,0,30,0,0);//,0,10,0,0);
  // }

  // //Draw test background
  //  for(uint16_t index=20; index<pCONT_iLight->settings.light_size_count-20; index++ ){
  //   animation_colours[index].DesiredColour = RgbcctColor(0,0,10,0,0);
  // }

  // Draw circle
  // Center pixel, then radius is by precentage of strop size
  float progress = 0;
  int32_t index_generate = 0;
  
  uint16_t pixel_index_lower = radius_pixel-center_repeat_size;
  uint16_t pixel_index_upper = radius_pixel+center_repeat_size;

  // Draw repeated center region
  for(uint16_t index=0; index<radius_size; index++ ){

    index_generate = radius_pixel+index;  
    if((index_generate>=0)&&(index_generate<100)){ animation_colours[index_generate].DesiredColour += colour_draw; } // if test

    index_generate = radius_pixel-index;  
    if((index_generate>=0)&&(index_generate<100)){ animation_colours[index_generate].DesiredColour += colour_draw; } // if test

  }

  // Draw tails
  for(uint16_t index=0; index<radius_size; index++ ){

    progress = mSupport::mapfloat(index,0,radius_size,0,1);

    //Draw half, larger index side
    colour_draw = RgbcctColor::LinearBlend(colour_center, colour_circumference, progress);

    
    index_generate = pixel_index_lower+index;  
    if((index_generate>=0)&&(index_generate<100)){ animation_colours[index_generate].DesiredColour += colour_draw; } // if test
    
    index_generate = pixel_index_upper-index;  
    if((index_generate>=0)&&(index_generate<100)){ animation_colours[index_generate].DesiredColour += colour_draw; } // if test

  }

}

void mAnimatorLight::Remove_DesiredColour_LinearGradientMirrored(
                                                RgbcctColor colour_center, 
                                                RgbcctColor colour_circumference, 
                                                uint16_t radius_pixel,
                                                uint16_t radius_size, //tail size
                                                uint8_t center_repeat_size,
                                                bool colour_is_additive,
                                                bool values_are_percentage_of_strip_size
){

  RgbcctColor colour_draw = RgbcctColor(0);
  // animation_colours[index_generate].DesiredColour

  //Draw test background
  //   for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
  //   animation_colours[index].DesiredColour = RgbcctColor(0);//,0,30,0,0);//,0,10,0,0);
  // }

  // //Draw test background
  //  for(uint16_t index=20; index<pCONT_iLight->settings.light_size_count-20; index++ ){
  //   animation_colours[index].DesiredColour = RgbcctColor(0,0,10,0,0);
  // }

  // Draw circle
  // Center pixel, then radius is by precentage of strop size
  float progress = 0;
  int32_t index_generate = 0;
  
  uint16_t pixel_index_lower = radius_pixel-center_repeat_size;
  uint16_t pixel_index_upper = radius_pixel+center_repeat_size;

  // Draw repeated center region
  for(uint16_t index=0; index<radius_size; index++ ){

    index_generate = radius_pixel+index;  
    if((index_generate>=0)&&(index_generate<100)){ animation_colours[index_generate].DesiredColour -= colour_draw; } // if test

    index_generate = radius_pixel-index;  
    if((index_generate>=0)&&(index_generate<100)){ animation_colours[index_generate].DesiredColour -= colour_draw; } // if test

  }

  // Draw tails
  for(uint16_t index=0; index<radius_size; index++ ){

    progress = mSupport::mapfloat(index,0,radius_size,0,1);

    //Draw half, larger index side
    colour_draw = RgbcctColor::LinearBlend(colour_center, colour_circumference, progress);

    
    index_generate = pixel_index_lower+index;  
    if((index_generate>=0)&&(index_generate<100)){ animation_colours[index_generate].DesiredColour -= colour_draw; } // if test
    
    index_generate = pixel_index_upper-index;  
    if((index_generate>=0)&&(index_generate<100)){ animation_colours[index_generate].DesiredColour -= colour_draw; } // if test

  }

}


// Adds colour with boundary checks
void mAnimatorLight::DesiredColourWrite_Safe(RgbcctColor colour, uint16_t index){

  // animation_colours[index_generate].DesiredColour


}


















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
*** Animation Effect:   HACS (Legacy)   ***************************************************************************************************************************************************************************
**  @note:    HACS animation, to be phased out into Segments **************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************/


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
      #ifdef ENABLE_DEVFEATURE_RGB_CLOCK
      case EFFECTS_FUNCTION_LCD_CLOCK_BASIC_01_ID:
        SubTask_Flasher_Animate_LCD_Clock_Time_Basic_01();
      break;
      // case EFFECTS_FUNCTION_LCD_CLOCK_BASIC_02_ID:
      //   SubTask_Flasher_Animate_LCD_Clock_Time_Basic_02();
      // break;
      // case EFFECTS_FUNCTION_LCD_DISPLAY_BASIC_01_ID:
      //   SubTask_Flasher_Animate_LCD_Display_Show_Numbers_Basic_01();
      // break;
      #endif // ENABLE_DEVFEATURE_RGB_CLOCK
      /**
       * These other modes must be moved into the new segments class
       * */
      //  case EFFECTS_FUNCTION_SLOW_GLOW_ON_BRIGHTNESS_ID:
      //    SubTask_Flasher_Animate_Function_Slow_Glow_On_Brightness();
      //  break;
      //  case EFFECTS_FUNCTION_FLASH_TWINKLE_SINGLE_COLOUR_RANDOM_ID:
      //    SubTask_Flasher_Animate_Function_Twinkle_SingleColour_Random();
      //  break;
      //  case EFFECTS_FUNCTION_FLASH_TWINKLE_PALETTE_COLOUR_RANDOM_ID:
      //    SubTask_Flasher_Animate_Function_Twinkle_PaletteColour_Random();
      //  break;
      //  case EFFECTS_FUNCTION_SLOW_FADE_BRIGHTNESS_ALL_ID:
      //    SubTask_Flasher_Animate_Function_Slow_Fade_Brightness_All();
      //  break;
      //  case EFFECTS_FUNCTION_SLOW_FADE_SATURATION_ALL_ID:
      //    SubTask_Flasher_Animate_Function_Slow_Fade_Saturation_All();
      //  break;
      //  case EFFECTS_FUNCTION_FADE_GRADIENT_ID:
      //    SubTask_Flasher_Animate_Function_Fade_Gradient();
      //  break;
      //  case EFFECTS_FUNCTION_PULSE_RANDOM_ON:
      //    SubTask_Flasher_Animate_Function_Pulse_Random_On();
      //  break;
      //  case EFFECTS_FUNCTION_PULSE_RANDOM_ON_TWO_ID:
      //    SubTask_Flasher_Animate_Function_Pulse_Random_On_2();
      //  break;
      // EFFECTS_FUNCTION_PULSE_RANDOM_ON_FADE_OFF_ID
      //  case EFFECTS_FUNCTION_PULSE_RANDOM_ON_FADE_OFF_ID:
      //    SubTask_Flasher_Animate_Function_Pulse_Random_On_Fade_Off();
      //  break;
      //  case EFFECTS_FUNCTION_POPPING_PALETTE_BRIGHTNESS_FROM_LOWER_TO_UPPER_BOUNDERY:
      //    SubTask_Flasher_Animate_Function_Popping_Palette_Brightness_From_Lower_To_Upper_Boundary();
      //  break;
      //  case EFFECTS_FUNCTION_TWINKLE_PALETTE_BRIGHTNESS_FROM_LOWER_TO_UPPER_AND_BACK:
      //    SubTask_Flasher_Animate_Function_Twinkle_Palette_Brightness_From_Lower_To_Upper_And_Back();
      //  break;
      //  case EFFECTS_FUNCTION_SUNPOSITIONS_SUNRISE_ALARM_01:
      //    SubTask_Flasher_Animate_Function_SunPositions_SunRise_Alarm_01();
      //  break;
      //  case EFFECTS_FUNCTION_SUNPOSITIONS_GRADIENT_SUN_ELEVATION_AND_AZIMUTH_01:
      //    SubTask_Flasher_Animate_Function_SunPositions_Gradient_Sun_Elevation_And_Azimuth_01();
      //  break;
      //  case EFFECTS_FUNCTION_SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_01_ID:
      //    SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_01();
      //  break;
      //  case EFFECTS_FUNCTION_SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_02_ID:
      //    SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_02();
      //  break;
      //  case EFFECTS_FUNCTION_SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_03_ID:
      //    SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_03();
      //  break;
      //  case EFFECTS_FUNCTION_SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_04_ID:
      //    SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_04();
      //  break;
      //  case EFFECTS_FUNCTION_SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_05_ID:
      //    SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_05();
      //  break;
      //  case EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_WITH_AUGMENTED_TRANSITIONS_01_ID:
      //    SubTask_Flasher_Animate_Function_SunPositions_Elevation_Only_RGBCCT_Palette_Indexed_Positions_With_Augmented_01();
      //  break;
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











/******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
*** Animation Effect:   WLED (Legacy)   ***************************************************************************************************************************************************************************
**  @note:    WLED initial port, running as its own effect. To be phased out into Segments **************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************/

#ifdef ENABLE_PIXEL_FUNCTION_WLED_PHASEOUT


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

// #ifdef USE_DEVFEATURE_WLED_METHOD_ORIGINAL_ADDED_AS_EFFECT

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


#ifdef ENABLE_PIXEL_FUNCTION_WLED_PHASEOUT
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

  mEffects->init(0, ledCount, 0);
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

    // #endif // USE_DEVFEATURE_WLED_METHOD_ORIGINAL_ADDED_AS_EFFECT


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
 #endif//define ENABLE_PIXEL_FUNCTION_WLED_PHASEOUT

#endif // ENABLE_PIXEL_FUNCTION_WLED_PHASEOUT

/******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
*** Animation Effect:   Segments   ***************************************************************************************************************************************************************************
**  @note:    New effect that merges HACS and WLED with segments and dynamic animation storage **************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************/





#endif //USE_MODULE_LIGHTS_ANIMATOR




