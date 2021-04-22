#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

const char* mAnimatorLight::PM_MODULE_LIGHTS_ANIMATOR_CTR = D_MODULE_LIGHTS_ANIMATOR_CTR;
const char* mAnimatorLight::PM_MODULE_LIGHTS_ANIMATOR_FRIENDLY_CTR = D_MODULE_LIGHTS_ANIMATOR_FRIENDLY_CTR;

/*******************************************************************************************************************
********************************************************************************************************************
************TASKER**************************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/

int8_t mAnimatorLight::Tasker(uint8_t function, JsonParserObject obj){ 

  int8_t function_result = 0;

  #ifdef ENABLE_DEVFEATURE_Tasker_Override_Forcing_Variables_Testing
  Tasker_Override_Forcing_Variables_Testing();
  #endif // ENABLE_DEVFEATURE_FLICKERING_TEST5

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case FUNC_PRE_INIT:
      pre_init();  
      break;
    case FUNC_INIT:
      init();
      break;
  }
  
  if(!settings.flags.EnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID;}
  
  switch(function){
    /************
     * SETTINGS SECTION * 
    *******************/
    case FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE: 
      // Settings_Load();
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
      //Settings_Default();
      // pCONT_iLight->ShowInterface();

      // #ifdef USE_PM_OUTSIDE_TREE_MIXER_DESCRIPTION
      //   char result[100];
      //   pCONT_sup->GetTextIndexed_P(result, sizeof(result), mixer.running_id, PM_OUTSIDE_TREE_MIXER_DESCRIPTION);  // should this be _P?
      //   AddLog(LOG_LEVEL_INFO, PSTR("Mixer routine \"%s\""), result);          
      // #endif // USE_PM_OUTSIDE_TREE_MIXER_DESCRIPTION

      #ifdef ENABLE_DEVFEATURE_DEBUG_GET_PIXEL_ZERO
      RgbTypeColor col = stripbus->GetPixelColor(0);

        AddLog(LOG_LEVEL_TEST, PSTR("getpixel[0] = %d,%d,%d"),col.R,col.G,col.B);


      #endif

    }break;
    case FUNC_EVERY_MINUTE:

      // pCONT_iLight->animation.flags.use_gamma_for_brightness ^= 1;

    break;
    case FUNC_LOOP: 
      EveryLoop();
    break;    
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
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
      MQTTHandler_Set_fSendNow();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
    // Other stuff
    case FUNC_STATUS_MESSAGE_APPEND:
      Append_Hardware_Status_Message();
    break;
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

void mAnimatorLight::pre_init(void){ 
  
  // Allow runtime changes of animation size
  pCONT_iLight->settings.light_size_count = STRIP_SIZE_MAX; 
  animator_strip_size = ANIMATOR_SIZE_MAX<=STRIP_SIZE_MAX?ANIMATOR_SIZE_MAX:STRIP_SIZE_MAX; 
  pCONT_set->Settings.flag_animations.clear_on_reboot = false; //flag

}

void mAnimatorLight::init(void){ 
    
  // if pixel size changes, free and init again
  uint16_t strip_size_tmp = STRIP_SIZE_MAX;
  #ifdef STRIP_REPEAT_OUTPUT_MAX;//pCONT_iLight->settings.light_size_count<STRIP_SIZE_MAX?pCONT_iLight->settings.light_size_count:STRIP_SIZE_MAX; // Catch values exceeding limit
  strip_size_tmp = STRIP_REPEAT_OUTPUT_MAX;
  #endif
  // uint16_t strip_size_tmp = STRIP_REPEAT_OUTPUT_MAX;//pCONT_iLight->settings.light_size_count<STRIP_SIZE_MAX?pCONT_iLight->settings.light_size_count:STRIP_SIZE_MAX; // Catch values exceeding limit
  
  
  ledout.length = STRIP_SIZE_MAX; 
  
  pCONT_iLight->settings.light_size_count = STRIP_SIZE_MAX;

  DEBUG_LINE;
  // #ifdef USE_WS28XX_HARDWARE_WS2801
  //   stripbus = new NeoPixelBus<DotStarBgrFeature, DotStarMethod>(strip_size_tmp, pin_clock, pCONT_pins->GetPin(GPIO_RGB_DATA_ID));
  // #else
  //   #ifdef ENABLE_DEBUG_ESP_DECODER
  //     stripbus = new NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType>(strip_size_tmp, 5);
  //   #else

  //     #ifdef ESP8266
  //       stripbus = new NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType>(strip_size_tmp, pCONT_pins->GetPin(GPIO_RGB_DATA_ID));
  //     #else //esp32 testing
        stripbus = new NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType>(strip_size_tmp, 23);//19); 3 = rx0
  //     #endif //esp8266
  //   #endif
  // #endif //USE_WS28XX_HARDWARE_WS2801

  DEBUG_LINE;

  uint16_t animator_strip_size_tmp = 1;//animator_strip_size<ANIMATOR_SIZE_MAX?animator_strip_size:ANIMATOR_SIZE_MAX; // Catch values exceeding limit

  pCONT_iLight->Init_NeoPixelAnimator(animator_strip_size_tmp, NEO_ANIMATION_TIMEBASE);  

  // delay(5000);
  // SetPixelColor_All(RgbcctColor(0,10,0,0,0));
  // delay(5000);

  // pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val = 100;  
      SetLEDOutAmountByPercentage(100);//pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val);  

  pCONT_iLight->animation.flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique = false;
pCONT_iLight->animation.flags.Limit_Upper_Brightness_With_BrightnessRGB = false;
  
  pCONT_iLight->animation.flags.Apply_Upper_And_Lower_Brightness_Randomly_Ranged_To_Palette_Choice = false; // FIX
  flashersettings.brightness_max = 255;
  flashersettings.brightness_min = 0;

#ifdef ENABLE_DEVFEATURE_DIRECT_TEMPFIX_RANDOMISE_BRIGHTNESS_ON_PALETTE_GET
  pCONT_iLight->animation.flags.Apply_Upper_And_Lower_Brightness_Randomly_Ranged_To_Palette_Choice = true; // FIX
  pCONT_iLight->animation.flags.Limit_Upper_Brightness_With_BrightnessRGB = true;
#endif // ENABLE_DEVFEATURE_DIRECT_TEMPFIX_RANDOMISE_BRIGHTNESS_ON_PALETTE_GET


  pixel_group.flags.fEnabled = false;
  pixel_group.flags.multiplier_mode_id = PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_UNIFORM_ID;
  pixel_group.multiplier = 2;
  pixel_group.mapped_array_data.values = editable_mapped_array_data_array;
  pixel_group.flags.mapped_array_editable_or_progmem = 0;
  pixel_group.mapped_array_data.length = 0;
  // flashersettings.flags.enable_startcolour_as_alternate = true;
  // flashersettings.flags.enable_endingcolour_as_alternate = true;
  
  
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
  #ifdef ENABLE_PIXEL_FUNCTION_EFFECTS
  init_flasher_settings();
  #endif
  #ifdef ENABLE_PIXEL_FUNCTION_MIXER
  init_mixer_defaults();
  #endif

  settings.flags.EnableModule = true;

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
    
    
      #ifdef ENABLE_PIXEL_FUNCTION_EFFECTS
    #ifdef DEFAULT_LIGHTING_EFFECTS_FUNCTION_ID
      flashersettings.function = 1;/*DEFAULT_LIGHTING_EFFECTS_FUNCTION_ID<EFFECTS_FUNCTION_LENGTH_ID?
                            DEFAULT_LIGHTING_EFFECTS_FUNCTION_ID:EFFECTS_FUNCTION_SLOW_GLOW_ID;*/
    #else
      flashersettings.function = EFFECTS_FUNCTION_SLOW_GLOW_ID;
    #endif
    #endif // ENABLE_PIXEL_FUNCTION_EFFECTS
  }


  // uint16_t buffer_length = 0;
  // char* buffer = pCONT_set->Settings.animation_settings.palette_user_variable_name_list_ctr;
  // for(uint8_t i=0;i<20;i++){
  //  buffer_length+=sprintf(buffer+buffer_length,"%s%02d|\0","User",i);
  // }
  // #ifndef DISABLE_SERIAL_LOGGING
  // //  Serial.println(buffer); log debuf more
  // #endif

  
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


}


void mAnimatorLight::GenerateAnimationPixelAmountMaps(){

  // for(int val=0;val<sizeof(pixels_to_update_as_number_map);val++){
  //   pixels_to_update_as_number_map[val] = ((float)PROGMEM pixels_to_update_as_percentage_map[val]/100.0f)*pCONT_iLight->settings.light_size_count;
  //   if(pixels_to_update_as_number_map[val]==0) pixels_to_update_as_number_map[val] = 1;
  // }

}



/*******************************************************************************************************************
********************************************************************************************************************
************ END OF GENERAL INIT ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/


#ifdef ENABLE_PIXEL_FUNCTION_EFFECTS

void mAnimatorLight::init_flasher_settings(){

  flashersettings.flags.movement_direction = 0;
  


// A "wipe" or "sine wave" that applies a saturation change across the string. So either, from all christmas coloured to slowly blending from the top of the tree to the bottom, to white (or another solid colour), or apply the saturation change in a loop/rotation. 


  
}





void mAnimatorLight::SubTask_Flasher_Main(){

#ifdef ENABLE_PIXEL_FUNCTION_MIXER
  if(mixer.flags.Enabled){
   SubTask_Flasher_Animate_Mixer();
  }
  #endif // ENABLE_PIXEL_FUNCTION_MIXER

  SubTask_Flasher_Animate();

} // SubTask_Flasher_Main



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


  if((mTime::TimeReached(&flashersettings.tSaved.Update,pCONT_iLight->animation.transition.rate_ms))||(pCONT_iLight->animation.flags.fForceUpdate)){

    // #ifdef ENABLE_DEVFEATURE_FLICKERING_TEST2
    // if(pCONT_iLight->animator_controller->IsAnimating()){
    //   AddLog(LOG_LEVEL_TEST, PSTR("Blocking new animation"));
    //   return;
    // }else{
      // flashersettings.tSaved.Update = millis();
    // }
    // #endif // ENABLE_DEVFEATURE_FLICKERING_TEST2

    if(pCONT_iLight->animation.flags.fForceUpdate){ pCONT_iLight->animation.flags.fForceUpdate=false;
      flashersettings.tSaved.Update = millis();
    }


    // #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
    // char buffer[100];
    // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "flashersettings.function=%d %s"),flashersettings.function,GetFlasherFunctionNamebyID(flashersettings.function, buffer));
    AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "flashersettings.function=%d"),flashersettings.function);
    //#endif
// #ifdef DEVICE_RGBFIREPLACE_TESTER
//     flashersettings.function = EFFECTS_FUNCTION_SLOW_GLOW_ID;
// #endif

    switch(flashersettings.function){
      default:
      case EFFECTS_FUNCTION_SLOW_GLOW_ID:
        SubTask_Flasher_Animate_Function_Slow_Glow();
      break;
       case EFFECTS_FUNCTION_SOLID_COLOUR_ID:
         SubTask_Flasher_Animate_Function_SOLID_COLOUR();
       break;
       case EFFECTS_FUNCTION_SLOW_GLOW_ON_BRIGHTNESS_ID:
         SubTask_Flasher_Animate_Function_Slow_Glow_On_Brightness();
       break;
       case EFFECTS_FUNCTION_SLOW_GLOW_PARTIAL_PALETTE_STEP_THROUGH_ID:
         SubTask_Flasher_Animate_Function_Slow_Glow_Partial_Palette_Step_Through();
       break;
       case EFFECTS_FUNCTION_SEQUENTIAL_ID:
         SubTask_Flasher_Animate_Function_Sequential();
       break;
       case EFFECTS_FUNCTION_FLASH_TWINKLE_SINGLE_COLOUR_RANDOM_ID:
         SubTask_Flasher_Animate_Function_Twinkle_SingleColour_Random();
       break;
       case EFFECTS_FUNCTION_FLASH_TWINKLE_PALETTE_COLOUR_RANDOM_ID:
         SubTask_Flasher_Animate_Function_Twinkle_PaletteColour_Random();
       break;
       case EFFECTS_FUNCTION_SLOW_FADE_BRIGHTNESS_ALL_ID:
         SubTask_Flasher_Animate_Function_Slow_Fade_Brightness_All();
       break;
       case EFFECTS_FUNCTION_SLOW_FADE_SATURATION_ALL_ID:
         SubTask_Flasher_Animate_Function_Slow_Fade_Saturation_All();
       break;
       case EFFECTS_FUNCTION_FADE_GRADIENT_ID:
         SubTask_Flasher_Animate_Function_Fade_Gradient();
       break;
       case EFFECTS_FUNCTION_TESTER_ID:
         SubTask_Flasher_Animate_Function_Tester();
       break; 
       case EFFECTS_FUNCTION_PULSE_RANDOM_ON:
         SubTask_Flasher_Animate_Function_Pulse_Random_On();
       break;
       case EFFECTS_FUNCTION_PULSE_RANDOM_ON_TWO_ID:
         SubTask_Flasher_Animate_Function_Pulse_Random_On_2();
       break;
       case EFFECTS_FUNCTION_PULSE_RANDOM_ON_FADE_OFF_ID:
         SubTask_Flasher_Animate_Function_Pulse_Random_On_Fade_Off();
       break;
       case EFFECTS_FUNCTION_POPPING_PALETTE_BRIGHTNESS_FROM_LOWER_TO_UPPER_BOUNDERY:
         SubTask_Flasher_Animate_Function_Popping_Palette_Brightness_From_Lower_To_Upper_Boundary();
       break;
       case EFFECTS_FUNCTION_TWINKLE_PALETTE_BRIGHTNESS_FROM_LOWER_TO_UPPER_AND_BACK:
         SubTask_Flasher_Animate_Function_Twinkle_Palette_Brightness_From_Lower_To_Upper_And_Back();
       break;
       case EFFECTS_FUNCTION_SUNPOSITIONS_GRADIENT_ALARM_01:
         SubTask_Flasher_Animate_Function_SunPositions_Gradient_Alarm_01();
       break;
       case EFFECTS_FUNCTION_SUNPOSITIONS_GRADIENT_SUN_ELEVATION_AND_AZIMUTH_01:
         SubTask_Flasher_Animate_Function_SunPositions_Gradient_Sun_Elevation_And_Azimuth_01();
       break;
       case EFFECTS_FUNCTION_FIREPLACE_01_ID:
         SubTask_Flasher_Animate_Function_FirePlace_01();
       break;
       #ifdef ENABLE_DEVFEATURE_RGB_CLOCK
      case EFFECTS_FUNCTION_LCD_CLOCK_BASIC_01_ID:
        SubTask_Flasher_Animate_LCD_Clock_Time_Basic_01();
      break;
      case EFFECTS_FUNCTION_LCD_CLOCK_BASIC_02_ID:
        SubTask_Flasher_Animate_LCD_Clock_Time_Basic_02();
      break;
      case EFFECTS_FUNCTION_LCD_DISPLAY_BASIC_01_ID:
        SubTask_Flasher_Animate_LCD_Display_Show_Numbers_Basic_01();
      break;
      #endif // ENABLE_DEVFEATURE_RGB_CLOCK


  //     case EFFECTS_FUNCTION_SHOWING_MULTIPLES_OF_COLOURS_ID:
    //     SubTask_Flasher_Animate_Function_Slow_Glow_Showing_Multiples_Of_Colours();
      // break;

//new pastel random ie change pixel colour 90-100% saturation with slow blend time and randomly
       //case EFFECTS_FUNCTION_FADE_GRADIENT_ID:
         //SubTask_Flasher_Animate_Function_Fade_Gradient();
       //break;

      // New animation
      // slowly random change between palette paired colours, ie red, green, blue, pink in pallette, first red/green, then blue/pink, or with flag, only skip one colour for each pair, red/green, green/blue, blue/pink etc


//pulse will use the "multiplier" number, and every X will turn on, moving by a few each time, fading off after instant on 
// instant on or pulsing on? 
// maybe I need to make "transition waveforms", square (instant), sine wave, pulsing, saw tooth (rising edge, falling edge)

     

      //https://photos.google.com/search/green%20light/photo/AF1QipONCC4dPcCk3AUmff17venI28Jih0iAsLwwX-Po
      //gradient light

      //apply saturation rotation or "wipe"

    } //end SWITCH

    // // Configure animator to show output
    // StartAnimationAsBlendFromStartingColorToDesiredColor();
    StartAnimation_AsAnimUpdateMemberFunction();

  }//end if update reached

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


// /****
//  * Changes pixels randomly to new colour, with slow blending
//  * Requires new colour calculation each call
//  */
// void mAnimatorLight::SubTask_Flasher_Animate_Function_Slow_Glow(){
//   // So colour region does not need to change each loop to prevent colour crushing
//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
//   UpdateDesiredColourFromPaletteSelected();
// }



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



#endif // ENABLE_PIXEL_FUNCTION_EFFECTS




/*******************************************************************************************************************
********************************************************************************************************************
************ END OF ANIMATION/EFFECTS/MIXER DEFINITIONS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/





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

  RgbcctColor color_hardware = color_internal;

  mInterfaceLight::HARDWARE_ELEMENT_COLOUR_ORDER order = pCONT_iLight->hardware_element_colour_order[0];

  // if(indexPixel<100){
    order = pCONT_iLight->hardware_element_colour_order[0];
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

  #ifdef ENABLE_DEVFEATURE_SETPIXELOUTPUT_VARIABLE

    if(indexPixel == 0){
      setpixel_variable_index_counter = 0; // reset
      pixel_group.mapped_array_data.index = 0;
    }

    if(setpixel_variable_index_counter>50){ return; }

    if(pixel_group.flags.fEnabled){

      uint8_t pixel_multiplier_count = 0;
      switch(pixel_group.flags.multiplier_mode_id){
        case PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_UNIFORM_ID:
          pixel_multiplier_count = pixel_group.multiplier;
          break;
        case PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_RANDOM_ID:
          pixel_multiplier_count = random(1,pixel_group.multiplier);
          break;
        case PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID:
          if(pixel_group.mapped_array_data.values != nullptr){
            // if any match, then jump index of lights forward
            uint8_t check_index = pixel_group.mapped_array_data.index;
            pixel_multiplier_count = pixel_group.mapped_array_data.values[check_index];
            
            //AddLog(LOG_LEVEL_WARN, PSTR("PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID %d"),pixel_multiplier_count);
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

  #endif // ENABLE_DEVFEATURE_SETPIXELOUTPUT_VARIABLE

}

RgbcctColor mAnimatorLight::GetPixelColor(uint16_t indexPixel)
{
  DEBUG_LINE;
  if(stripbus == nullptr){    
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "stripbus == nullptr"));
    #endif
  }

  
  #ifdef ENABLE_DEVFEATURE_SETPIXELOUTPUT_VARIABLE

  #ifdef USE_DEVFEATURE_PIXEL_OUTPUT_MULTIPLIER
  indexPixel *= USE_DEVFEATURE_PIXEL_OUTPUT_MULTIPLIER;
  #endif
    if(indexPixel == 0){
      setpixel_variable_index_counter = 0; // reset
      pixel_group.mapped_array_data.index = 0;
    }

    if(setpixel_variable_index_counter>50){ return RgbcctColor(0); }

    if(pixel_group.flags.fEnabled){

      uint8_t pixel_multiplier_count = 0;
      switch(pixel_group.flags.multiplier_mode_id){
        case PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_UNIFORM_ID:
          pixel_multiplier_count = pixel_group.multiplier;
          break;
        case PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_RANDOM_ID:
          pixel_multiplier_count = random(1,pixel_group.multiplier);
          break;
        case PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID:
          if(pixel_group.mapped_array_data.values != nullptr){
            // if any match, then jump index of lights forward
            uint8_t check_index = pixel_group.mapped_array_data.index;
            pixel_multiplier_count = pixel_group.mapped_array_data.values[check_index];
            
            //AddLog(LOG_LEVEL_WARN, PSTR("PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID %d"),pixel_multiplier_count);
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
        if(setpixel_variable_index_counter>50){ return RgbcctColor(0); }

        pCONT_iLight->SetPixelColourHardwareInterface(color_hardware,setpixel_variable_index_counter++);  
      }

    }else{
      pCONT_iLight->SetPixelColourHardwareInterface(color_hardware,indexPixel);
    } // pixel_group.flags.fEnabled

    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("indexPixel=%d,setpixel_variable_index_counterhere=%d"),indexPixel,setpixel_variable_index_counter);

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
  mInterfaceLight::HARDWARE_ELEMENT_COLOUR_ORDER order = pCONT_iLight->hardware_element_colour_order[0];


  // if(indexPixel<100){
    order = pCONT_iLight->hardware_element_colour_order[0];
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






////////////////////// START OF MQTT /////////////////////////

void mAnimatorLight::MQTTHandler_Init(){

  uint8_t ii = 0;

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_SETTINGS_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.configperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Settings;
  // mqtthandler_list_ids[ii] = mqtthandler_ptr->handler_id;
  // mqtthandler_list_ptr[ii] = &mqtthandler_ptr;

  mqtthandler_ptr = &mqtthandler_animation_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_ANIMATION_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_ANIMATION_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Animation;

  mqtthandler_ptr = &mqtthandler_ambilight_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_AMBILIGHT_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_AMBILIGHT_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Ambilight;
  
  #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS      //make this optional, as it uses extra data and is only for special cases
  mqtthandler_ptr = &mqtthandler_notifications_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_NOTIFICATION_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_NOTIFICATIONS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Notifications;
  #endif
    
  mqtthandler_ptr = &mqtthandler_state_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_STATE_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_State;
  
  #ifdef ENABLE_PIXEL_FUNCTION_EFFECTS
  mqtthandler_ptr = &mqtthandler_flasher_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_EFFECTS_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_EFFECTS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Flasher;
  #endif

#ifdef ENABLE_PIXEL_FUNCTION_MIXER
  mqtthandler_ptr = &mqtthandler_mixer_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_MIXER_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_MIXER_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Mixer;
  #endif
  
  // mqtt_handler_ids
  //  = {
  //   MQTT_HANDLER_SETTINGS_ID, MQTT_HANDLER_MODULE_ANIMATION_TELEPERIOD_ID, MQTT_HANDLER_MODULE_AMBILIGHT_TELEPERIOD_ID,
  //   MQTT_HANDLER_MODULE_STATE_TELEPERIOD_ID
  //   #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS 
  //     MQTT_HANDLER_MODULE_NOTIFICATION_TELEPERIOD_ID,
  //   #endif
  //   #ifdef ENABLE_PIXEL_FUNCTION_EFFECTS
  //     MQTT_HANDLER_MODULE_EFFECTS_TELEPERIOD_ID, MQTT_HANDLER_MODULE_MIXER_TELEPERIOD_ID
  //   #endif
  // };
  
  // struct handler<mAnimatorLight>* mqtthandler_list_ptr[];
  // 4] = {
  //   &mqtthandler_settings_teleperiod, &mqtthandler_animation_teleperiod, &mqtthandler_ambilight_teleperiod,
  //   &mqtthandler_state_teleperiod,
  //   #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS 
  //     &mqtthandler_notifications_teleperiod,
  //   #endif
  //   #ifdef ENABLE_PIXEL_FUNCTION_EFFECTS
  //     &mqtthandler_flasher_teleperiod, &mqtthandler_mixer_teleperiod
  //   #endif
  // };


#ifdef ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
  mqtthandler_ptr = &mqtthandler_manual_setpixel;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_MIXER_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETPIXEL_MANUALLY_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Manual_SetPixel;  
#endif // ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL



} //end "MQTTHandler_Init"


void mAnimatorLight::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  mqtthandler_animation_teleperiod.flags.SendNow = true;
  mqtthandler_ambilight_teleperiod.flags.SendNow = true;
  #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS 
  mqtthandler_notifications_teleperiod.flags.SendNow = true;
  #endif
  mqtthandler_state_teleperiod.flags.SendNow = true;
  mqtthandler_flasher_teleperiod.flags.SendNow = true;
  mqtthandler_mixer_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mAnimatorLight::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_animation_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_ambilight_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS 
  mqtthandler_notifications_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  #endif
  mqtthandler_state_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_flasher_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_mixer_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mAnimatorLight::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t mqtthandler_list_ids[] = {
    MQTT_HANDLER_MODULE_ANIMATION_TELEPERIOD_ID, MQTT_HANDLER_MODULE_AMBILIGHT_TELEPERIOD_ID,
    MQTT_HANDLER_MODULE_STATE_TELEPERIOD_ID,
    #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS 
      MQTT_HANDLER_MODULE_NOTIFICATION_TELEPERIOD_ID,
    #endif
    #ifdef ENABLE_PIXEL_FUNCTION_EFFECTS
      MQTT_HANDLER_MODULE_EFFECTS_TELEPERIOD_ID, 
    #endif
    #ifdef ENABLE_PIXEL_FUNCTION_MIXER
      MQTT_HANDLER_MODULE_MIXER_TELEPERIOD_ID,
    #endif
    #ifdef ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
      MQTT_HANDLER_MODULE_MANUAL_SETPIXEL_TELEPERIOD_ID,
    #endif
    MQTT_HANDLER_SETTINGS_ID // Always last, as always included
  };
  
  // Could this be put into a function? to allow me to set things to all in this, or by ID
  struct handler<mAnimatorLight>* mqtthandler_list_ptr[] = {
    &mqtthandler_animation_teleperiod, &mqtthandler_ambilight_teleperiod,
    &mqtthandler_state_teleperiod,
    #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS 
      &mqtthandler_notifications_teleperiod,
    #endif
    #ifdef ENABLE_PIXEL_FUNCTION_EFFECTS
      &mqtthandler_flasher_teleperiod, 
    #endif
    #ifdef ENABLE_PIXEL_FUNCTION_MIXER
      &mqtthandler_mixer_teleperiod,
    #endif
    #ifdef ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
      &mqtthandler_manual_setpixel,
    #endif
    &mqtthandler_settings_teleperiod
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, EM_MODULE_LIGHTS_ANIMATOR_ID,
    mqtthandler_list_ptr, mqtthandler_list_ids,
    sizeof(mqtthandler_list_ptr)/sizeof(mqtthandler_list_ptr[0]),
    mqtt_handler_id
  );

}

////////////////////// END OF MQTT /////////////////////////

// #endif

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



uint16_t mAnimatorLight::SetLEDOutAmountByPercentage(uint8_t percentage){

  strip_size_requiring_update = mapvalue(percentage, 0,100, 0,pCONT_iLight->settings.light_size_count);

// AddLog(LOG_LEVEL_TEST, PSTR(DEBUG_INSERT_PAGE_BREAK "SetLEDOutAmountByPercentage = %d"),strip_size_requiring_update);

  return strip_size_requiring_update; // also return the count

}



void mAnimatorLight::RefreshLEDIndexPattern(){

  // AddLog(LOG_LEVEL_TEST, PSTR("f::%s"),__FUNCTION__);

  // DEBUG_PRINT_FUNCTION_NAME_TEST;
  // DEBUG_PRINT_FUNCTION_NAME;

  // Somehow this orders need to become flasher... which I will rename pCONT_iLight->animation

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
  // for(int remaining=pick_list_remaining;remaining>0;remaining--){
          
  // AddLog(LOG_LEVEL_TEST, PSTR("BEFORE pick_list_remaining=%d"), pick_list_remaining);
          while(pick_list_remaining){


  // AddLog_Array(LOG_LEVEL_TEST, "pick_list_remaining", pick_list,pick_list_remaining);
  // AddLog(LOG_LEVEL_TEST, PSTR("pick_list_remaining=%d"), pick_list_remaining);


      //   Serial.println("pick_list_remaining");
      // for(int jj=0;jj<pick_list_remaining;jj++){ Serial.printf("%d, ",pick_list[jj]); } Serial.println();


            pick_list_index = random(0,pick_list_remaining);
            pick_list_val = pick_list[pick_list_index];
           // AddLog(LOG_LEVEL_TEST,PSTR("pick_list_remaining=%d [%d:%d]"),pick_list_remaining,pick_list_index,pick_list_val);

            ledout.pattern[ledout.index++] = pick_list_val;

        // Serial.println("REMOVING");
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
//     case TRANSITION_ORDER_ROTATE_ID:
// // x = new index 0
// //       [0,1,2,x,4,5] start
// //       [0,1,2] //remember <x
// //       [x,4,5] // shift down
// //       [x,4,5,0,1,2] end

// // ROTATE IS HANDLED BY NEOPIXEBUS FUNCTIONS, USE THEM!!

//       uint16_t rotate_amount = 1;
//       // Store last value
//       uint16_t last_index = ledout.pattern[ledout.length-1];
//       // Serial.println();
//       // for(int jj=0;jj<ledout.length;jj++){ Serial.printf("%d, ",ledout.pattern[jj]); } Serial.println();

//       // shift all values by 1
//       for(ledout.index=0;ledout.index<ledout.length/2-1;ledout.index++){ 
//         swap(
//           ledout.pattern[ledout.length/2-ledout.index],
//           ledout.pattern[ledout.length/2+ledout.index]
//         );
//       }
//       // Move into first

//       // for(int jj=0;jj<ledout.length;jj++){ Serial.printf("%d, ",ledout.pattern[jj]); } Serial.println();

//       // Use existing pattern and rotate it
//       // for(ledout.index=0;ledout.index<length_local;ledout.index++){ 
//       //   index_pattern[ledout.index] = ledout.index; 
//       // }
//     break;
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

  
  pCONT_iLight->animator_controller->StartAnimation(0, _time_to_newcolour, [this](const AnimationParam& param){ this->BlendAnimUpdate(param);} );
  
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


  pCONT_iLight->animator_controller->StartAnimation(0, time_tmp, [this](const AnimationParam& param){ this->BlendAnimUpdate(param);} );




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
        this->AnimUpdateMemberFunction_Tester(param);
      }
    );

  #endif // ENABLE_DEVFEATURE_FLICKERING_TEST3

  // DEBUG_PRINT_FUNCTION_NAME_TEST;

} //end function


mAnimatorLight& mAnimatorLight::setAnimFunctionCallback(ANIM_FUNCTION_SIGNATURE) {
    this->anim_function_callback = anim_function_callback;
    return *this;
}




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
        // default:
        case PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_UNIFORM_ID:
          pixel_multiplier_count = pixel_group.multiplier;
          break;
        case PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_RANDOM_ID:
          pixel_multiplier_count = random(1,pixel_group.multiplier);
          break;
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

      for(uint8_t ii = 0; ii < pixel_multiplier_count; ii++){
        animation_colours[setpixel_variable_index_counter++].DesiredColour = animation_colours[indexPixel].StartingColor;
        if(setpixel_variable_index_counter>pCONT_iLight->settings.light_size_count-1){ return true; }
      }

    }
    else{
      animation_colours[indexPixel].DesiredColour = animation_colours[indexPixel].StartingColor;
    } // pixel_group.flags.fEnabled

    //AddLog(LOG_LEVEL_TEST, PSTR("indexPixel=%d,setpixel_variable_index_counterhere=%d %d"),indexPixel,setpixel_variable_index_counter,pixel_multiplier_count);

  }

  return true; // returns if this function changed the desiredcolour

}








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






void mAnimatorLight::Append_Hardware_Status_Message(){

  // struct STATUSHARDWARE{
  //   char ctr[200];
  //   uint8_t len = 0;
  //   uint8_t importance = 0; //0 low, 1 med, 2 high
  // };

  // int numpixels = pCONT_iLight->settings.light_size_count;
  // if(numpixels>55){numpixels=55;}

  uint32_t colourcount = 0;

  //for(int i=0;i<numpixels;i++){
    RgbTypeColor c = GetPixelColor(0);
    colourcount += c.R+c.G+c.B;
  //}

  if(colourcount){

    // pCONT_tel->hardwarestatus.len += pCONT_sup->WriteBuffer_P(pCONT_tel->hardwarestatus.ctr+pCONT_tel->hardwarestatus.len,
    //   PSTR("%s%02X%02X%02X ON"),
    //     pCONT_tel->hardwarestatus.len>0?"":"', ",
    //     c.R,c.G,c.B
    // );

    // SetAndKeepHighestNumber(&pCONT_tel->hardwarestatus.importance, (uint8_t) HARDWARE_STATUS_IMPORTANCE_MEDIUM_ID);

  }

  //status on


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

  D_DATA_BUFFER_CLEAR();
  pCONT_sup->WriteBuffer_P(data_buffer.payload.ctr,
    PSTR("{"
      "\"" "ledout"       "\":%d,"
      "\"" "pCONT_iLight->settings.light_size_count"        "\":%d,"
      "\"" "STRIP_SIZE_MAX"          "\":%d,"
      "\"" "animator_strip_size"  "\":%d,"
      "\"" "ANIMATOR_SIZE_MAX"         "\":%d"
    "}"),
    ledout.length,
    pCONT_iLight->settings.light_size_count,
    STRIP_SIZE_MAX,
    animator_strip_size,
    ANIMATOR_SIZE_MAX
  );

  return strlen(data_buffer.payload.ctr) ? 1 : 0;

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

  // JsonObject transitionobj = root.createNestedObject(D_JSON_TRANSITION);
  //   transitionobj[D_JSON_METHOD] = GetTransitionMethodName();
  //   // transitionobj[D_JSON_TIME] = mSupport::safeDivideInt(pCONT_iLight->animation.transition.time_ms.val,1000);
  //   transitionobj[D_JSON_TIME_MS] = pCONT_iLight->animation.transition.time_ms.val;
  //   // transitionobj[D_JSON_RATE] = mSupport::safeDivideInt(pCONT_iLight->animation.transition.rate_ms,1000);
  //   transitionobj[D_JSON_RATE_MS] = pCONT_iLight->animation.transition.rate_ms;
  //   // transitionobj[D_JSON_PIXELS_UPDATE_NUMBER] = GetPixelsToUpdateAsNumberFromPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val);
  //   transitionobj[D_JSON_PIXELS_UPDATE_PERCENTAGE] = pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val;
  //   transitionobj[D_JSON_ORDER] = GetTransitionOrderName();

  // // Flags and states that are used during one transition and reset when completed
  // // JsonObject overridesobj = root.createNestedObject("transition_overrides"); 
  // //   overridesobj["fRefreshAllPixels"] = pCONT_iLight->animation_override.fRefreshAllPixels;
  
  // // root["ledout.length"] = ledout.length;

  // data_buffer.payload.len = measureJson(root)+1;
  // serializeJson(doc,data_buffer.payload.ctr);

  return 0;

}



uint8_t mAnimatorLight::ConstructJSON_Ambilight(uint8_t json_level){
  // Awaiting total redesign
  
  #ifdef ENABLE_LOG_LEVEL_DEBUG
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "f::ConstructJSON_Ambilight"));
  #endif

  
  // DynamicJsonDocument doc(1500);
  // JsonObject root = doc.to<JsonObject>();

  // root["ambilight"] =0;//ledout.length;

  // data_buffer.payload.len = measureJson(root)+1;
  // serializeJson(doc,data_buffer.payload.ctr);

  return 0;//data_buffer.payload.len;

}





uint8_t mAnimatorLight::ConstructJSON_State(uint8_t json_level){

  uint8_t numpixels = pCONT_iLight->settings.light_size_count<100?pCONT_iLight->settings.light_size_count:100;
  RgbTypeColor c;
  
  JsonBuilderI->Start();  
    JsonBuilderI->Add_P(PM_JSON_SIZE, pCONT_iLight->settings.light_size_count);
    JsonBuilderI->Array_Start("rgb");
    for(int i=0;i<numpixels;i++){
      RgbTypeColor c = GetPixelColor(i);
      JsonBuilderI->Add_FV(PSTR("%02X%02X%02X"),c.R,c.G,c.B);
    }
    JsonBuilderI->Array_End();
  return JsonBuilderI->End();

}


#ifdef ENABLE_DEVFEATURE_Tasker_Override_Forcing_Variables_Testing

  Tasker_Override_Forcing_Variables_Testing();
  // pCONT_iLight->_briRGB = 255;
  // flashersettings.function = EFFECTS_FUNCTION_SLOW_GLOW_PARTIAL_PALETTE_STEP_THROUGH_ID;//EFFECTS_FUNCTION_PULSE_RANDOM_ON
  // pCONT_iLight->animation.mode_id = pCONT_iLight->ANIMATION_MODE_EFFECTS_ID;
  // pCONT_iLight->animation.transition.rate_ms = 400; // not sure if this will work? I probably have to force a check somewhere
  // pCONT_iLight->animation.transition.time_ms.val = 2;
  // pCONT_iLight->animation.transition.order_id = TRANSITION_ORDER_INORDER_ID;
  // pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val = 50;
  // pCONT_iLight->animation.palette.id = pCONT_iLight->PALETTELIST_VARIABLE_USER_01_ID;
  // pCONT_iLight->animation.palette_index_range_max_limit = 2;
  // mixer.running_id = 10;
  // mixer.flags.Enabled = false;


  // pCONT_iLight->_briRGB = 255;
  // flashersettings.function = EFFECTS_FUNCTION_TESTER_ID;//EFFECTS_FUNCTION_PULSE_RANDOM_ON
  // pCONT_iLight->animation.mode_id = pCONT_iLight->ANIMATION_MODE_EFFECTS_ID;
  // pCONT_iLight->animation.transition.rate_ms = 400; // not sure if this will work? I probably have to force a check somewhere
  // pCONT_iLight->animation.transition.time_ms.val = 350;
  // pCONT_iLight->animation.transition.order_id = TRANSITION_ORDER_INORDER_ID;
  // pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val = 50;
  // pCONT_iLight->animation.palette.id = pCONT_iLight->PALETTELIST_VARIABLE_USER_01_ID;
  // pCONT_iLight->animation.palette_index_range_max_limit = 2;
  // mixer.running_id = 10;
  // mixer.flags.Enabled = false;

#endif // ENABLE_DEVFEATURE_Tasker_Override_Forcing_Variables_Testing



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



#endif //USE_MODULE_LIGHTS_ANIMATOR




