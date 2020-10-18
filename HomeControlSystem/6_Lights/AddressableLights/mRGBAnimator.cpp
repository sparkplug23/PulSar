#include "mRGBAnimator.h"

#ifdef USE_MODULE_LIGHTS_ADDRESSABLE

/*******************************************************************************************************************
********************************************************************************************************************
************TASKER**************************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/

int8_t mRGBAnimator::Tasker(uint8_t function){ 

  int8_t function_result = 0;

  /************
   * INIT SECTION * 
  *******************/
  if(function == FUNC_PRE_INIT){
    pre_init();
  }else
  if(function == FUNC_INIT){
    init();
  }

  if(pCONT_set->Settings.light_settings.type != LT_WS2812){ return 0; }
  // if(!settings.flags.EnableModule){ return 0;}
  
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
      Settings_Default();
      // pCONT_iLight->Module_Init(); // User defined values in progmem, later to be parsed via webcommand
    break;
    case FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT:
      Settings_Default();
      pCONT_set->SettingsSave(2);
    break;
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_EVERY_SECOND:{
      //EverySecond();
      //Settings_Default();

      // NeoPixelAnimator animator_controller = NeoPixelAnimator(50, NEO_ANIMATION_TIMEBASE); // NeoPixel animation management object

      //AddLog_P(LOG_LEVEL_TEST, PSTR("sizeof(animator_controller 50) %d"), sizeof(NeoPixelAnimator));


      // AddLog_P(LOG_LEVEL_TEST, PSTR("sizeof(hsbcolour 50) %d"), sizeof(hsbcolour));

      // RgbColor rgbcolor1[50];
      // AddLog_P(LOG_LEVEL_TEST, PSTR("sizeof(rgbcolor 50) %d"), sizeof(rgbcolor1));
      // RgbwColor rgbcolor2[50];
      // AddLog_P(LOG_LEVEL_TEST, PSTR("sizeof(rgbWcolor 50) %d"), sizeof(rgbcolor2));

    }break;
    case FUNC_LOOP: 
      EveryLoop();
    break;    
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_COMMAND:

    break;
    case FUNC_SET_POWER:
      // LightSetPower();
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MQTT
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
    #endif //USE_MQTT
    // Other stuff
    case FUNC_STATUS_MESSAGE_APPEND:
      Append_Hardware_Status_Message();
    break;
  }// switch(command)

  /************
   * WEBPAGE SECTION * 
  *******************/  
  #ifdef USE_MODULE_CORE_WEBSERVER
  return Tasker_Web(function);
  #endif // USE_MODULE_CORE_WEBSERVER

} // END FUNCTION
int8_t mRGBAnimator::Tasker(uint8_t function, JsonObjectConst obj){
  switch(function){
    case FUNC_JSON_COMMAND_OBJECT:
      parsesub_CheckAll(obj);
    break;
    case FUNC_JSON_COMMAND_OBJECT_WITH_TOPIC:
      return CheckAndExecute_JSONCommands(obj);
    break;
  }
}




/*******************************************************************************************************************
********************************************************************************************************************
************ START OF GENERAL INIT PROCESS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/

void mRGBAnimator::pre_init(void){ 
  
  // Allow runtime changes of animation size
  strip_size = STRIP_SIZE_MAX; 
  animator_strip_size = ANIMATOR_SIZE_MAX<=STRIP_SIZE_MAX?ANIMATOR_SIZE_MAX:STRIP_SIZE_MAX; 
  pCONT_set->Settings.flag_animations.clear_on_reboot = false; //flag

}

void mRGBAnimator::init(void){ 
    
  // if pixel size changes, free and init again
  uint16_t strip_size_tmp = STRIP_REPEAT_OUTPUT_MAX;//strip_size<STRIP_SIZE_MAX?strip_size:STRIP_SIZE_MAX; // Catch values exceeding limit
  ledout.length = STRIP_SIZE_MAX; 
  
  #ifdef USE_WS28XX_HARDWARE_WS2801
    stripbus = new NeoPixelBus<DotStarBgrFeature, DotStarMethod>(strip_size_tmp, pin_clock, pCONT_set->pin[GPIO_RGB_DATA_ID]);
  #else
    #ifdef ENABLE_DEBUG_ESP_DECODER
      stripbus = new NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType>(strip_size_tmp, 5);
    #else
      stripbus = new NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType>(strip_size_tmp, pCONT_set->pin[GPIO_RGB_DATA_ID]);
    #endif
  #endif //USE_WS28XX_HARDWARE_WS2801

  uint16_t animator_strip_size_tmp = animator_strip_size<ANIMATOR_SIZE_MAX?animator_strip_size:ANIMATOR_SIZE_MAX; // Catch values exceeding limit

  #ifndef ENABLE_DEVFEATURE_SINGLE_ANIMATOR_INTERFACE
  animator_controller = new NeoPixelAnimator(animator_strip_size_tmp, NEO_ANIMATION_TIMEBASE); // NeoPixel animation management object
  #else
  pCONT_iLight->Init_NeoPixelAnimator(animator_strip_size_tmp, NEO_ANIMATION_TIMEBASE);  
  #endif // ENABLE_DEVFEATURE_SINGLE_ANIMATOR_INTERFACE


  pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val = 100;
  
  
  randomSeed(analogRead(0));
  
  for(ledout.index=0;ledout.index<sizeof(ledout.pattern);ledout.index++){ 
    ledout.pattern[ledout.index] = ledout.index; 
  }

  // Clear stored light output
  memset(&animation_colours,0,sizeof(animation_colours));
  // Start display
  stripbus->Begin();
  if(pCONT_set->Settings.flag_animations.clear_on_reboot){
    #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
      stripbus->ClearTo(RgbwColor(0,255,0,0));
    #else
      stripbus->ClearTo(RgbColor(0,255,0));
    #endif
    stripbus->Show();
  }

  pCONT_iLight->animation_override.fRefreshAllPixels = true;

  GenerateAnimationPixelAmountMaps();

  // New sub init that configures subfunctions
  // init_ColourMap();
  
  uint16_t buffer_length = 0;
  char* buffer = pCONT_set->Settings.animation_settings.palette_user_variable_name_list_ctr;

  for(uint8_t i=0;i<20;i++){
   buffer_length+=sprintf(buffer+buffer_length,"%s%02d|\0","User",i);
  }
   
  #ifndef DISABLE_SERIAL_LOGGING
   Serial.println(buffer);
  #endif

  #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
    init_NotificationPanel();
  #endif
  
  #ifdef USE_PIXEL_ANIMATION_MODE_PIXEL
  blocking_force_animate_to_complete = true; //animate to completion on boot (for short animations)
  init_Ambilight();
  #endif
  #ifdef ENABLE_PIXEL_FUNCTION_FLASHER
  init_flasher_settings();
  #endif
  #ifdef ENABLE_PIXEL_FUNCTION_MIXER
  init_mixer_defaults();
  #endif

  settings.flags.EnableModule = true;

  //create copy for pCONT_iLight->animation stored
  memcpy(&pCONT_iLight->animation_stored,&pCONT_iLight->animation,sizeof(pCONT_iLight->animation_stored));// RESTORE copy of state

} //end "init"


void mRGBAnimator::Settings_Load(){
    
  // pCONT_iLight->animation.brightness = pCONT_set->Settings.light_settings.light_brightness_as_percentage/100.0f;
  
  pCONT_iLight->setBriRGB(pCONT_set->Settings.light_settings.light_brightness_as_percentage);

  // uint8_t list = 0;
  // SetPaletteListPtrFromID(list);
  // AddLog_P(LOG_LEVEL_TEST,PSTR("LOADED %d amount = %d"),list,palettelist.ptr->colour_map_size);
  // for(uint8_t element=0;element<20;element++){
  //   AddLog_P(LOG_LEVEL_TEST,PSTR("LOADED %d i%d amount = %d"),list,element,pCONT_set->Settings.palette_user_colour_map_ids[list][element]);
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
  pCONT_iLight->animation.palette_id = pCONT_set->Settings.animation_settings.animation_palette;
  pCONT_iLight->animation.transition.order_id   = pCONT_set->Settings.animation_settings.animation_transition_order;
  pCONT_iLight->animation.transition.method_id  = pCONT_set->Settings.animation_settings.animation_transition_method;
  pCONT_iLight->animation.transition.time_ms.val = pCONT_set->Settings.animation_settings.animation_transition_time_ms;
  pCONT_iLight->animation.transition.rate_ms.val = pCONT_set->Settings.animation_settings.animation_transition_rate_ms;
  pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val = pCONT_set->Settings.animation_settings.animation_transition_pixels_to_update_as_percentage;

}

void mRGBAnimator::Settings_Save(){
  
  // pCONT_set->Settings.light_settings.light_brightness_as_percentage = pCONT_iLight->animation.brightness*100;

  // #ifdef ENABLE_LOG_LEVEL_INFO
  // AddLog_P(LOG_LEVEL_TEST,PSTR("SAVED pCONT_iLight->animation.brightness = %d"),pCONT_set->Settings.light_settings.light_brightness_as_percentage);
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
  pCONT_set->Settings.animation_settings.animation_palette = pCONT_iLight->animation.palette_id;
  pCONT_set->Settings.animation_settings.animation_transition_order = pCONT_iLight->animation.transition.order_id;
  pCONT_set->Settings.animation_settings.animation_transition_method = pCONT_iLight->animation.transition.method_id;
  pCONT_set->Settings.animation_settings.animation_transition_time_ms = pCONT_iLight->animation.transition.time_ms.val;
  pCONT_set->Settings.animation_settings.animation_transition_rate_ms = pCONT_iLight->animation.transition.rate_ms.val;
  pCONT_set->Settings.animation_settings.animation_transition_pixels_to_update_as_percentage = pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val;

}



void mRGBAnimator::Settings_Default(){

  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog_P(LOG_LEVEL_TEST,PSTR("mRGBAnimator::Settings_Default"));
  #endif

  #ifdef USE_LIGHTING_TEMPLATE
    settings.flags.TemplateProvidedInProgMem = true;
  #endif
  
  if(settings.flags.TemplateProvidedInProgMem){// use template provided in progmem || override template
  
    pCONT_iLight->Template_Load();

  }else{
    
    // Check for user defined defaults
    #ifdef DEFAULT_LIGHTING_TRANSITION_ORDER_ID
      pCONT_iLight->animation.transition.order_id = DEFAULT_LIGHTING_TRANSITION_ORDER_ID<TRANSITION_ORDER_LENGTH_ID?
                                            DEFAULT_LIGHTING_TRANSITION_ORDER_ID:TRANSITION_ORDER_INORDER_ID;
    #else
      pCONT_iLight->animation.transition.order_id = TRANSITION_ORDER_INORDER_ID;
    #endif
    
    #ifdef DEFAULT_LIGHTING_TRANSITION_TIME_MAP_SECS_ID
      pCONT_iLight->animation.transition.time_ms.map_id = DEFAULT_LIGHTING_TRANSITION_TIME_MAP_SECS_ID<TIME_MAP_SECS_LENGTH_ID?
                                                DEFAULT_LIGHTING_TRANSITION_TIME_MAP_SECS_ID:TIME_MAP_SECS_15_ID;
      pCONT_iLight->animation.transition.time_ms.val = 2000;//PROGMEM rate_map_secs[pCONT_iLight->animation.transition.time_ms.map_id]*1000;
      pCONT_iLight->animation.flags.ftime_use_map = true;
    #else
      pCONT_iLight->animation.transition.time_ms.map_id = TIME_MAP_SECS_15_ID;
      pCONT_iLight->animation.transition.time_ms.val = PROGMEM rate_map_secs[pCONT_iLight->animation.transition.time_ms.map_id]*1000;
      pCONT_iLight->animation.flags.ftime_use_map = true;
    #endif

    #ifdef DEFAULT_LIGHTING_TRANSITION_RATE_MAP_SECS_ID
      pCONT_iLight->animation.transition.rate_ms.map_id = DEFAULT_LIGHTING_TRANSITION_RATE_MAP_SECS_ID<RATE_MAP_SECS_LENGTH_ID?
                                                DEFAULT_LIGHTING_TRANSITION_RATE_MAP_SECS_ID:RATE_MAP_SECS_15_ID;
      pCONT_iLight->animation.transition.rate_ms.val = 10000;//PROGMEM rate_map_secs[pCONT_iLight->animation.transition.rate_ms.map_id]*1000;
      pCONT_iLight->animation.flags.frate_use_map = true;
    #else
      pCONT_iLight->animation.transition.rate_ms.map_id = RATE_MAP_SECS_15_ID;
      pCONT_iLight->animation.transition.rate_ms.val = PROGMEM rate_map_secs[pCONT_iLight->animation.transition.rate_ms.map_id]*1000;
      pCONT_iLight->animation.flags.frate_use_map = true;
    #endif

    #ifdef DEFAULT_LIGHTING_PIXELS_UPDATE_PERCENTAGE_ID
      pCONT_iLight->animation.transition.pixels_to_update_as_percentage.map_id = DEFAULT_LIGHTING_PIXELS_UPDATE_PERCENTAGE_ID<PIXELS_UPDATE_PERCENTAGE_LENGTH_ID?
                            DEFAULT_LIGHTING_PIXELS_UPDATE_PERCENTAGE_ID:PIXELS_UPDATE_PERCENTAGE_20_ID;
    #else
      pCONT_iLight->animation.transition.pixels_to_update_as_percentage.map_id = PIXELS_UPDATE_PERCENTAGE_20_ID;
    #endif

    #ifdef DEFAULT_LIGHTING_BRIGHTNESS_PERCENTAGE
      // pCONT_iLight->animation.brightness = (float)DEFAULT_LIGHTING_BRIGHTNESS_PERCENTAGE/100.0f;//mSupport::WithinLimits(0,(float)DEFAULT_LIGHTING_BRIGHTNESS_PERCENTAGE/100.0f,1)
    #else
      // pCONT_iLight->animation.brightness = WithinLimits(0,(float)1/100.0f,1);
      pCONT_iLight->animation.brightness = 1; //default ot 50% normally for power reasons
    #endif

    #ifdef DEFAULT_LIGHTING_LIVEVIEW_REFRESH_RATE_HZ
      liveview.refresh_rate = DEFAULT_LIGHTING_LIVEVIEW_REFRESH_RATE_HZ;
    #else
      pCONT_iLight->liveview.refresh_rate = 1000;
    #endif
    
    #ifdef DEFAULT_LIGHTING_FLASHER_FUNCTION_ID
      flashersettings.function = DEFAULT_LIGHTING_FLASHER_FUNCTION_ID<FLASHER_FUNCTION_LENGTH_ID?
                            DEFAULT_LIGHTING_FLASHER_FUNCTION_ID:FLASHER_FUNCTION_SLOW_GLOW_ID;
    #else
      flashersettings.function = FLASHER_FUNCTION_SLOW_GLOW_ID;
    #endif
  }

}


void mRGBAnimator::Settings_Reset(){
  
  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog_P(LOG_LEVEL_INFO,PSTR("RESETING SAVED VALUES"));
  #endif
  init(); //default values
  Settings_Default();
}


void mRGBAnimator::GenerateAnimationPixelAmountMaps(){

  // for(int val=0;val<sizeof(pixels_to_update_as_number_map);val++){
  //   pixels_to_update_as_number_map[val] = ((float)PROGMEM pixels_to_update_as_percentage_map[val]/100.0f)*strip_size;
  //   if(pixels_to_update_as_number_map[val]==0) pixels_to_update_as_number_map[val] = 1;
  // }

}

uint16_t mRGBAnimator::GetPixelsToUpdateAsNumberFromPercentage(uint8_t percentage){
  return map(percentage,0,100,0,strip_size);
}
uint8_t mRGBAnimator::GetPixelsToUpdateAsPercentageFromNumber(uint16_t number){
  return map(number,0,strip_size,0,100);
}


/*******************************************************************************************************************
********************************************************************************************************************
************ END OF GENERAL INIT ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/



#ifdef USE_PIXEL_ANIMATION_MODE_PIXEL

/*******************************************************************************************************************
********************************************************************************************************************
************ START OF AMBILIGHT DEFINITIONS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/

// Limit ambilight to addressible type, else I will just use "scene"
void mRGBAnimator::init_Ambilight(){

  ambilightsettings.screens[SCREEN_CENTRE].top.colour    = HsbColor(pCONT_iLight->HueN2F(20),pCONT_iLight->SatN2F(95),pCONT_iLight->BrtN2F(100));
  ambilightsettings.screens[SCREEN_CENTRE].bottom.colour = HsbColor(pCONT_iLight->HueN2F(8),pCONT_iLight->SatN2F(95),pCONT_iLight->BrtN2F(100));
  ambilightsettings.screens[SCREEN_CENTRE].left.colour   = HsbColor(pCONT_iLight->HueN2F(240),pCONT_iLight->SatN2F(100),pCONT_iLight->BrtN2F(100));
  ambilightsettings.screens[SCREEN_CENTRE].right.colour  = HsbColor(pCONT_iLight->HueN2F(330),pCONT_iLight->SatN2F(100),pCONT_iLight->BrtN2F(100));
  ambilightsettings.screens[SCREEN_CENTRE].top.size = 33;
  ambilightsettings.screens[SCREEN_CENTRE].bottom.size = 33;
  ambilightsettings.screens[SCREEN_CENTRE].left.size = 19;
  ambilightsettings.screens[SCREEN_CENTRE].right.size = 19;
  ambilightsettings.screens[SCREEN_CENTRE].left.blend_between_sides_gradient_percentage = 50;

}

void mRGBAnimator::SubTask_Ambilight(){

  // Add mode to allow orientations, for when screens rotate so they respect top/bottom

  // if(abs(millis()-ambilightsettings.tSavedUpdate)>ambilightsettings.ratemsSavedUpdate){ambilightsettings.tSavedUpdate=millis();
  //   AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "ambilight"));
  // }

  //switch modes : USE presets
  switch(ambilightsettings.ambilight_mode){
    case AMBILIGHT_PRESETS_ID:

    break;
    case AMBILIGHT_SIDES_ID: // IE DELL of dual tone from the past
      // not even splits, setting split point (ie bottom edge only 0%, 25% way up sides, half way 50%)
      if(abs(millis()-ambilightsettings.tSavedUpdate)>ambilightsettings.ratemsSavedUpdate){ambilightsettings.tSavedUpdate=millis();
        //AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "ambilight"));
        Ambilight_Sides();
        ConfigureLEDTransitionAnimation();
      }
    break;
    case AMBILIGHT_INPUT_STREAM_ID:{
      //serial input
    }break;
  }

} // END function


void mRGBAnimator::Ambilight_Sides(){
  #ifdef ENABLE_LOG_LEVEL_DEBUG
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::Ambilight_Sides()"));
  #endif

  float progress;
  HsbColor colour_tmp;

  uint8_t bottom_size = ambilightsettings.screens[SCREEN_CENTRE].bottom.size;
  uint8_t bottom_start = 0;
  for(int bottom=0;bottom<bottom_size;bottom++){
    desired_colour[bottom_start+bottom] = ambilightsettings.screens[SCREEN_CENTRE].bottom.colour;
  }

  uint8_t left_size = ambilightsettings.screens[SCREEN_CENTRE].left.size;
  uint8_t left_start = 33;
  for(int left=0;left<left_size;left++){
    //if(ambilightsettings.screens[SCREEN_CENTRE].left.blend_between_sides_gradient_percentage>=0){ //if set
      progress = (float)(left)/(float)(left_size);
      colour_tmp = RgbColor::LinearBlend(RgbColor(ambilightsettings.screens[SCREEN_CENTRE].bottom.colour),
                                         RgbColor(ambilightsettings.screens[SCREEN_CENTRE].top.colour),
                                         progress);
    //}
    desired_colour[left_start+left] = colour_tmp;//ambilightsettings.screens[SCREEN_CENTRE].left.colour;
  }

  uint8_t top_size = ambilightsettings.screens[SCREEN_CENTRE].top.size;
  uint8_t top_start = 52;
  for(int top=0;top<top_size;top++){
    desired_colour[top_start+top] = ambilightsettings.screens[SCREEN_CENTRE].top.colour;
  }

  uint8_t right_size = ambilightsettings.screens[SCREEN_CENTRE].right.size;
  uint8_t right_start = 85;
  for(int right=0;right<right_size;right++){
    progress = (float)(right)/(float)(right_size);
    colour_tmp = RgbColor::LinearBlend(RgbColor(ambilightsettings.screens[SCREEN_CENTRE].top.colour),
                                       RgbColor(ambilightsettings.screens[SCREEN_CENTRE].bottom.colour),
                                       progress);
    desired_colour[right_start+right] = colour_tmp;
  }

}



//previous heating panel
void mRGBAnimator::parsesub_ModeAmbilight(JsonObjectConst obj){

   // Create local dereferenced variable
  // JsonObject obj = (*_obj); 

  int8_t tmp_id = 0;
  

  char buffer[50];

// #ifndef ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT

  if(!obj[D_JSON_NAME].isNull()){ 
    const char* scenectr = obj[D_JSON_NAME];
    if((tmp_id=GetSceneIDbyName(scenectr))>=0){
      scene.name_id = tmp_id;
      pCONT_iLight->animation.mode_id = ANIMATION_MODE_SCENE_ID;
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_NAME,GetSceneName(buffer));
      Response_mP(S_JSON_COMMAND_SVALUE,D_JSON_NAME,GetSceneName(buffer));
      data_buffer2.isserviced++;
    }else{
      AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_NAME,scenectr);
    }
  }

  // #endif //ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT

  // USe pointers
  //side
  // struct AMBILIGHT_SCREEN_SETTINGS.SCREENS[0]::
  // ambilightsettings.screens[SCREEN_CENTRE].top
  //screen
  
  if(!obj[F("top")][F(D_JSON_HUE)].isNull()){ 
    uint16_t hue = obj[F("top")][F(D_JSON_HUE)];
    // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_HUE,hue);
    ambilightsettings.screens[SCREEN_CENTRE].top.colour.H = pCONT_iLight->HueN2F(hue);
    // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_HUE,ambilightsettings.screens[SCREEN_CENTRE].top.colour.H);
    // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_HUE,ambilightsettings.screens[SCREEN_CENTRE].top.colour.H);
    data_buffer2.isserviced++;
  }
  if(!obj[F("top")][F(D_JSON_SAT)].isNull()){ 
    uint8_t sat = obj[F("top")][F(D_JSON_SAT)];
    // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_SAT,sat);
    ambilightsettings.screens[SCREEN_CENTRE].top.colour.S = pCONT_iLight->SatN2F(sat);
    // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_SAT,ambilightsettings.screens[SCREEN_CENTRE].top.colour.S);
    // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_SAT,ambilightsettings.screens[SCREEN_CENTRE].top.colour.S);
    data_buffer2.isserviced++;
  }
  if(!obj[F("top")][F(D_JSON_BRT)].isNull()){ 
    uint8_t brt = obj[F("top")][F(D_JSON_BRT)];
    // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_BRT,brt);
    ambilightsettings.screens[SCREEN_CENTRE].top.colour.B = pCONT_iLight->animation.brightness = pCONT_iLight->BrtN2F(brt);
    // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_BRT,pCONT_iLight->animation.brightness);
    // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_BRT,pCONT_iLight->animation.brightness);
    data_buffer2.isserviced++;
  }



  if(!obj[F("bottom")][F(D_JSON_HUE)].isNull()){ 
    uint16_t hue = obj[F("bottom")][F(D_JSON_HUE)];
    // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_HUE,hue);
    ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.H = pCONT_iLight->HueN2F(hue);
    // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_HUE,ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.H);
    // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_HUE,ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.H);
    data_buffer2.isserviced++;
  }
  if(!obj[F("bottom")][F(D_JSON_SAT)].isNull()){ 
    uint8_t sat = obj[F("bottom")][F(D_JSON_SAT)];
    // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_SAT,sat);
    ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.S = pCONT_iLight->SatN2F(sat);
    // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_SAT,ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.S);
    // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_SAT,ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.S);
    data_buffer2.isserviced++;
  }
  if(!obj[F("bottom")][F(D_JSON_BRT)].isNull()){ 
    uint8_t brt = obj[F("bottom")][F(D_JSON_BRT)];
    // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_BRT,brt);
    ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.B = pCONT_iLight->animation.brightness = pCONT_iLight->BrtN2F(brt);
    // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_BRT,pCONT_iLight->animation.brightness);
    // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_BRT,pCONT_iLight->animation.brightness);
    data_buffer2.isserviced++;
  }




  if(!obj[D_JSON_RGB].isNull()){
    const char* rgbpacked = obj[D_JSON_RGB];
    uint32_t colour32bit = 0;
    if(rgbpacked[0]=='#'){ colour32bit = (long) strtol( &rgbpacked[1], NULL, 16);
    }else{ colour32bit = (long) strtol( &rgbpacked[0], NULL, 16); }
    RgbColor rgb;
    rgb.R = colour32bit >> 16; //RGB
    rgb.G = colour32bit >> 8 & 0xFF; //RGB
    rgb.B = colour32bit & 0xFF; //RGB
    // #ifndef ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT
    // scene.colour = HsbColor(RgbColor(rgb.R,rgb.G,rgb.B));
    // // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_RGB ":%s " D_NEOPIXEL_RGB ":%d,%d,%d " D_NEOPIXEL_HSB ":%d,%d,%d"),
    // //   rgbpacked,rgb.R,rgb.G,rgb.B,scene.colour.H,scene.colour.S,scene.colour.B);
    // #endif //ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT
  }

  // TIME with different units
  if(!obj[D_JSON_TIME].isNull()){ //default to secs
    pCONT_iLight->animation.transition.time_ms.val = obj["time"];
    pCONT_iLight->animation.transition.time_ms.val *= 1000;
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),pCONT_iLight->animation.transition.time_ms.val);  
  }else
  if(!obj[D_JSON_TIME].isNull()){
    pCONT_iLight->animation.transition.time_ms.val = obj["time_secs"];
    pCONT_iLight->animation.transition.time_ms.val *= 1000;
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),pCONT_iLight->animation.transition.time_ms.val);  
  }else
  if(!obj[D_JSON_TIME_MS].isNull()){
    pCONT_iLight->animation.transition.time_ms.val = obj["time_ms"];
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),pCONT_iLight->animation.transition.time_ms.val);  
  }

  
// Flash colour needs to NOT be a scene, but part of a manual direct mode
// if(strstr(scenectr,"FLASHCOLOUR")){
//       scene_reseting.name_id = scene.name_id; // remember scene to return to
//       scene.name_id = MODE_SINGLECOLOUR_FLASHCOLOUR_ID;
//     }
//     // Parse out flashcolour info if it exists
//     if(scene.name_id == MODE_SINGLECOLOUR_FLASHCOLOUR_ID){

//       if(tempctr = obj["hue"]){
//         scene_reseting.hue = obj["hue"];
//       }
//       if(tempctr = obj["sat"]){
//         scene_reseting.sat = obj["sat"];
//       }
//       if(tempctr = obj["brt"]){
//         scene_reseting.brt = obj["brt"];
//       }
//       if(tempctr = obj["time"]){
//         scene_reseting.tOnTime = obj["time"];
//       }
//     }
  
  



//   // create easier names
//   // char* topic_ctr = data_buffer2.topic.ctr;
//   // uint8_t topic_len = data_buffer2.topic.len;
//   // char* payload_ctr = data_buffer2.payload.ctr;
//   // uint8_t payload_len = data_buffer2.payload.len;

//   if(mSupport::memsearch(data_buffer2.topic.ctr,data_buffer2.topic.len,"/center",sizeof("/center")-1)>=0){pCONT->mso->MessagePrintln("MATCHED /center");

//     memset(&parsed,0,sizeof(parsed)); // clear parsing struct

//     uint16_t index = 0;

//     ambilightsettings.colour.found_idx = 0;

//     StaticJsonDocument<300> doc;
//     DeserializationError error = deserializeJson(doc, data_buffer2.payload.ctr);
//     JsonObject root = doc.as<JsonObject>();

//     if(root["RGB"].is<JsonArray>()){

//       JsonArray colourarray = root["RGB"];
//       int array_length = colourarray.size();

//       const char* pixelcolour;
//       for(JsonVariantConst v : colourarray) {
//         pixelcolour = v.as<const char*>();

//         if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
//         }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }

//           ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R = colour32bit >> 16; //RGB
//           ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G = colour32bit >> 8 & 0xFF; //RGB
//           ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B = colour32bit & 0xFF; //RGB


//         ambilightsettings.colour.found_idx++;
//         index++;
//       }
//       pCONT->mso->MessagePrint("ENDambilightsettings.col.found_idx");
//       pCONT->mso->MessagePrintln(ambilightsettings.colour.found_idx);
//       pCONT->mso->MessagePrintln(index);

//     }
//     // else if(root["RGB"].is<const char*>()){ //one colour = string
//     //
//     //   const char* pixelcolour;
//     //   pixelcolour = root["RGB"];
//     //
//     //   //Serial.println("pixelcolour"); Serial.println(pixelcolour);
//     //   if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
//     //   }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }
//     //
//     //   ambilightsettings.colour.r[ambilightsettings.colour.found_idx] = colour32bit >> 16; //RGB
//     //   ambilightsettings.colour.g[ambilightsettings.colour.found_idx] = colour32bit >> 8 & 0xFF; //RGB
//     //   ambilightsettings.colour.b[ambilightsettings.colour.found_idx] = colour32bit & 0xFF; //RGB
//     //
//     //   // Serial.println(parsed.col.r[parsed.col.found_idx]);
//     //   // Serial.println(parsed.col.g[parsed.col.found_idx]);
//     //   // Serial.println(parsed.col.b[parsed.col.found_idx]);
//     //
//     //   ambilightsettings.colour.found_idx++;
//     // }

//     pCONT->mso->MessagePrint("ambilightsettings.colour.found_idx");
//     pCONT->mso->MessagePrintln(ambilightsettings.colour.found_idx);
//     //strip_size
//     //ambilightsettings.colour.found_idx
//     for(int i=0;i<index;i++){
//         SetPixelColor(i, RgbColor(ambilightsettings.colour.rgb[i].R,ambilightsettings.colour.rgb[i].G,ambilightsettings.colour.rgb[i].B));
//     }stripbus->Show();


// } // END center


// #ifdef DEVICE_RGBDELL

//   if(mSupport::memsearch(data_buffer2.topic.ctr,data_buffer2.topic.len,"/left",sizeof("/left")-1)>=0){pCONT->mso->MessagePrintln("MATCHED /left");

//     memset(&parsed,0,sizeof(parsed)); // clear parsing struct

//     ambilightsettings.colour.found_idx = 0;

//     StaticJsonDocument<300> doc;
//     DeserializationError error = deserializeJson(doc, data_buffer2.payload.ctr);
//     JsonObject root = doc.as<JsonObject>();

//   // PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM -

//     if(root["RGB"].is<JsonArray>()){
//       //pCONT->mso->MessagePrintln("colour arr ");//Serial.println(rgbname_ctr);

//       JsonArray colourarray = root["RGB"];
//       int array_length = colourarray.size();

//       const char* pixelcolour;
//       for(JsonVariantConst v : colourarray) {
//         pixelcolour = v.as<const char*>();

//         //pCONT->mso->MessagePrintln("pixelcolour"); //pCONT->mso->MessagePrintln(pixelcolour);

//         if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
//         }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }

//         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R = colour32bit >> 16; //RGB
//         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G = colour32bit >> 8 & 0xFF; //RGB
//         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B = colour32bit & 0xFF; //RGB

//          pCONT->mso->MessagePrint(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R);pCONT->mso->MessagePrint("-");
//          pCONT->mso->MessagePrint(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G);pCONT->mso->MessagePrint("-");
//          pCONT->mso->MessagePrintln(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B);

//         ambilightsettings.colour.found_idx++;
//       }

//     }else if(root["RGB"].is<const char*>()){ //one colour = string

//       const char* pixelcolour;
//       pixelcolour = root["RGB"];

//       //Serial.println("pixelcolour"); Serial.println(pixelcolour);
//       if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
//       }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }

//       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R = colour32bit >> 16; //RGB
//       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G = colour32bit >> 8 & 0xFF; //RGB
//       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B = colour32bit & 0xFF; //RGB

//       // Serial.println(parsed.col.r[parsed.col.found_idx]);
//       // Serial.println(parsed.col.g[parsed.col.found_idx]);
//       // Serial.println(parsed.col.b[parsed.col.found_idx]);

//       ambilightsettings.colour.found_idx++;
//     }

//     pCONT->mso->MessagePrintln(ambilightsettings.colour.found_idx);
//     //strip_size
//     //ambilightsettings.colour.found_idx
//     //  for(int i=0;i<ambilightsettings.colour.found_idx;i++){
//     //    SetPixelColor(i,RgbColor(ambilightsettings.colour.r[i],ambilightsettings.colour.g[i],ambilightsettings.colour.b[i]));    //turn every third pixel on
//     //  }
//     // /stripbus->Show();

//     uint32_t c; //colourrgb
//     pinMode(RGB_DATA_LEFT_PIN,OUTPUT);
//     pinMode(RGB_CLOCK_LEFT_PIN,OUTPUT);

//     digitalWrite(RGB_DATA_LEFT_PIN,LOW);digitalWrite(RGB_CLOCK_LEFT_PIN,LOW);
//     for(int ii=0;ii<ambilightsettings.colour.found_idx;ii++){
//       shiftOut(RGB_DATA_LEFT_PIN, RGB_CLOCK_LEFT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].R);
//       shiftOut(RGB_DATA_LEFT_PIN, RGB_CLOCK_LEFT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].G);
//       shiftOut(RGB_DATA_LEFT_PIN, RGB_CLOCK_LEFT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].B);
//     }
//     digitalWrite(RGB_DATA_LEFT_PIN,LOW);digitalWrite(RGB_CLOCK_LEFT_PIN,LOW);



// } // END left



//   if(mSupport::memsearch(data_buffer2.topic.ctr,data_buffer2.topic.len,"/right",sizeof("/right")-1)>=0){pCONT->mso->MessagePrintln("MATCHED /right");

//     memset(&parsed,0,sizeof(parsed)); // clear parsing struct

//     ambilightsettings.colour.found_idx = 0;

//     StaticJsonDocument<300> doc;
//     DeserializationError error = deserializeJson(doc, data_buffer2.payload.ctr);
//     JsonObject root = doc.as<JsonObject>();

//   // PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM -

//     if(root["RGB"].is<JsonArray>()){
//       //pCONT->mso->MessagePrintln("colour arr ");//Serial.println(rgbname_ctr);

//       JsonArray colourarray = root["RGB"];
//       int array_length = colourarray.size();

//       const char* pixelcolour;
//       for(JsonVariantConst v : colourarray) {
//         pixelcolour = v.as<const char*>();

//         //pCONT->mso->MessagePrintln("pixelcolour"); //pCONT->mso->MessagePrintln(pixelcolour);

//         if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
//         }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }

//         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R = colour32bit >> 16; //RGB
//         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G = colour32bit >> 8 & 0xFF; //RGB
//         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B = colour32bit & 0xFF; //RGB

//          pCONT->mso->MessagePrint(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R);pCONT->mso->MessagePrint("-");
//          pCONT->mso->MessagePrint(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G);pCONT->mso->MessagePrint("-");
//          pCONT->mso->MessagePrintln(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B);

//         ambilightsettings.colour.found_idx++;
//       }

//     }else if(root["RGB"].is<const char*>()){ //one colour = string

//       const char* pixelcolour;
//       pixelcolour = root["RGB"];

//       //Serial.println("pixelcolour"); Serial.println(pixelcolour);
//       if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
//       }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }

//       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R = colour32bit >> 16; //RGB
//       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G = colour32bit >> 8 & 0xFF; //RGB
//       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B = colour32bit & 0xFF; //RGB

//       // Serial.println(parsed.col.r[parsed.col.found_idx]);
//       // Serial.println(parsed.col.g[parsed.col.found_idx]);
//       // Serial.println(parsed.col.b[parsed.col.found_idx]);

//       ambilightsettings.colour.found_idx++;
//     }

//     pCONT->mso->MessagePrintln(ambilightsettings.colour.found_idx);
//     //strip_size
//     //ambilightsettings.colour.found_idx
//     // for(int i=0;i<ambilightsettings.colour.found_idx;i++){
//     //   mrgbneo_ani->setPixelColor(i,mrgbneo_ani->Color(ambilightsettings.colour.r[i],ambilightsettings.colour.g[i],ambilightsettings.colour.b[i]));    //turn every third pixel on
//     // }
//     // mrgbneo_ani->setBrightness(255);
//     // mrgbneo_ani->show();

//     uint32_t c; //colourrgb
//     pinMode(RGB_DATA_RIGHT_PIN,OUTPUT);
//     pinMode(RGB_CLOCK_RIGHT_PIN,OUTPUT);

//     digitalWrite(RGB_DATA_RIGHT_PIN,LOW);digitalWrite(RGB_CLOCK_RIGHT_PIN,LOW);
//     for(int ii=0;ii<ambilightsettings.colour.found_idx;ii++){
//       shiftOut(RGB_DATA_RIGHT_PIN, RGB_CLOCK_RIGHT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].R);
//       shiftOut(RGB_DATA_RIGHT_PIN, RGB_CLOCK_RIGHT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].G);
//       shiftOut(RGB_DATA_RIGHT_PIN, RGB_CLOCK_RIGHT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].B);
//     }
//     digitalWrite(RGB_DATA_RIGHT_PIN,LOW);digitalWrite(RGB_CLOCK_RIGHT_PIN,LOW);



// } // END left

//       //fShowPanelUpdate = true;
//     //}


// //  }

//   //switch modes : USE serial input stream
//   //send as json array?
//   //RGB = [[r,g,b],[r,g,b],[r,g,b]]
//   //SETTINGS = {pixel ratio, 17,10}{pixel direction, CW}{startposition, bottom right}{timeout,10}{minbrightness,10}{fade,0}

//   pCONT_iLight->animation.mode_id = MODE_AMBILIGHT_ID;
//   fForcePanelUpdate = true;

  return 0;

} // END FUNCTION


/*******************************************************************************************************************
********************************************************************************************************************
************ END OF AMBILIGHT DEFINITIONS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/

#endif


void mRGBAnimator::init_flasher_settings(){
  
// // #ifdef DEVICE_OUTSIDETREE
//   // #define FLASHER_FUNCTION_MIXER_MAX 10
//   // enum FLASHER_FUNCTION_MIXER{FLASHER_FUNCTION_MIXER_1_ID=0,
//   //                             FLASHER_FUNCTION_MIXER_2_ID,
//   //                             FLASHER_FUNCTION_MIXER_3_ID,
//   //                             FLASHER_FUNCTION_MIXER_4_ID,
//   //                             FLASHER_FUNCTION_MIXER_5_ID,
//   //                             FLASHER_FUNCTION_MIXER_6_ID
//   // };

//   mixer.mode.running_id = FLASHER_FUNCTION_MIXER_0_ID;

//   // Set time at mode
//   mixer.mode.time_on_secs[FLASHER_FUNCTION_MIXER_0_ID] = 10;
//   mixer.mode.time_on_secs[FLASHER_FUNCTION_MIXER_1_ID] = 10;
//   mixer.mode.time_on_secs[FLASHER_FUNCTION_MIXER_2_ID] = 10;
//   mixer.mode.time_on_secs[FLASHER_FUNCTION_MIXER_3_ID] = 10;
//   mixer.mode.time_on_secs[FLASHER_FUNCTION_MIXER_4_ID] = 10;
//   mixer.mode.time_on_secs[FLASHER_FUNCTION_MIXER_5_ID] = 10;
//   mixer.mode.time_on_secs[FLASHER_FUNCTION_MIXER_6_ID] = 10;

//   mixer.mode.list_len = FLASHER_FUNCTION_MIXER_6_ID; //last mode

//   mixer.tSavedMillisToChangeAt = 0; //start at 0

//   flash_twinkle_random.flash_colour = HsbColor(0,0,1); //white

//   mixer.mode.times.flashing_starttime.Mday = 6; // set to today with __DATE__ later
//   mixer.mode.times.flashing_starttime.month = mTime::MONTH_AUGUST;
//   mixer.mode.times.flashing_starttime.year = 2019;
//   mixer.mode.times.flashing_starttime.hour = 16;
//   mixer.mode.times.flashing_starttime.minute = 0;
//   mixer.mode.times.flashing_starttime.second = 0;

//   mixer.mode.times.flashing_endtime.Mday = 6; // set to today with __DATE__ later
//   mixer.mode.times.flashing_endtime.month = mTime::MONTH_AUGUST;
//   mixer.mode.times.flashing_endtime.year = 2019;
//   mixer.mode.times.flashing_endtime.hour = 20;
//   mixer.mode.times.flashing_endtime.minute = 0;
//   mixer.mode.times.flashing_endtime.second = 0;

//   memset(mixer.mode.running_friendly_name_ctr,0,sizeof(mixer.mode.running_friendly_name_ctr));

//   mixer.mode.times.enable_skip_restricted_by_time[FLASHER_FUNCTION_MIXER_0_ID] = false;
//   mixer.mode.times.enable_skip_restricted_by_time[FLASHER_FUNCTION_MIXER_1_ID] = false;
//   mixer.mode.times.enable_skip_restricted_by_time[FLASHER_FUNCTION_MIXER_2_ID] = false;
//   mixer.mode.times.enable_skip_restricted_by_time[FLASHER_FUNCTION_MIXER_3_ID] = false;
//   mixer.mode.times.enable_skip_restricted_by_time[FLASHER_FUNCTION_MIXER_4_ID] = false;
//   mixer.mode.times.enable_skip_restricted_by_time[FLASHER_FUNCTION_MIXER_5_ID] = false;


        
  // flashersettings.random_transitions.array_index_length = 0;
  // flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 20;
  // flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 40;
  // flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 50;
  // flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 75;
  // flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 100;
  // flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 75;
  // flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 50;
  // flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 40;
  
}


#ifdef ENABLE_PIXEL_FUNCTION_MIXER

void mRGBAnimator::init_mixer_defaults(){

//   // Use switch case to all (no arg as reset all or target a specific one)

//   // Group 0
//   mixer.group[FLASHER_FUNCTION_MIXER_0_ID].time_on_secs_decounter = -1;
//   mixer.group[FLASHER_FUNCTION_MIXER_0_ID].starttime.hour = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_0_ID].starttime.minute = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_0_ID].starttime.second = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_0_ID].starttime.Wday = 0;
//   mixer.group[FLASHER_FUNCTION_MIXER_0_ID].endtime.hour = 17;
//     mixer.group[FLASHER_FUNCTION_MIXER_0_ID].endtime.minute = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_0_ID].endtime.second = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_0_ID].endtime.Wday = 0;
//   mixer.group[FLASHER_FUNCTION_MIXER_0_ID].enable_skip_restricted_by_time = 0;
//   mixer.group[FLASHER_FUNCTION_MIXER_0_ID].isenabled = true;
//   mixer.group[FLASHER_FUNCTION_MIXER_0_ID].transition_time_sec = 1;
//   mixer.group[FLASHER_FUNCTION_MIXER_0_ID].transition_rate_sec = 1;
//   mixer.group[FLASHER_FUNCTION_MIXER_0_ID].animation_transition_order = TRANSITION_ORDER_RANDOM_ID;
//   mixer.group[FLASHER_FUNCTION_MIXER_0_ID].flashersettings_function = FLASHER_FUNCTION_PRESET_ID;
//   mixer.group[FLASHER_FUNCTION_MIXER_0_ID].run_time_duration_sec = 60;
//   mixer.group[FLASHER_FUNCTION_MIXER_0_ID].animation_palette = PALETTELIST_VARIABLE_USER_16_ID;
//   // Group 1
//   mixer.group[FLASHER_FUNCTION_MIXER_1_ID].time_on_secs_decounter = -1;
//   mixer.group[FLASHER_FUNCTION_MIXER_1_ID].starttime.hour = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_1_ID].starttime.minute = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_1_ID].starttime.second = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_1_ID].starttime.Wday = 0;
//   mixer.group[FLASHER_FUNCTION_MIXER_1_ID].endtime.hour = 17;
//     mixer.group[FLASHER_FUNCTION_MIXER_1_ID].endtime.minute = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_1_ID].endtime.second = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_1_ID].endtime.Wday = 0;
//   mixer.group[FLASHER_FUNCTION_MIXER_1_ID].enable_skip_restricted_by_time = 0;
//   mixer.group[FLASHER_FUNCTION_MIXER_1_ID].isenabled = true;
//   mixer.group[FLASHER_FUNCTION_MIXER_1_ID].transition_time_sec = 1;
//   mixer.group[FLASHER_FUNCTION_MIXER_1_ID].transition_rate_sec = 1;
//   mixer.group[FLASHER_FUNCTION_MIXER_1_ID].animation_transition_order = TRANSITION_ORDER_RANDOM_ID;
//   mixer.group[FLASHER_FUNCTION_MIXER_1_ID].flashersettings_function = FLASHER_FUNCTION_PRESET_ID;
//   mixer.group[FLASHER_FUNCTION_MIXER_1_ID].run_time_duration_sec = 60;
//   mixer.group[FLASHER_FUNCTION_MIXER_1_ID].animation_palette = PALETTELIST_VARIABLE_USER_17_ID;
//   // Group 1
//   mixer.group[FLASHER_FUNCTION_MIXER_2_ID].time_on_secs_decounter = -1;
//   mixer.group[FLASHER_FUNCTION_MIXER_2_ID].starttime.hour = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_2_ID].starttime.minute = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_2_ID].starttime.second = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_2_ID].starttime.Wday = 0;
//   mixer.group[FLASHER_FUNCTION_MIXER_2_ID].endtime.hour = 17;
//     mixer.group[FLASHER_FUNCTION_MIXER_2_ID].endtime.minute = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_2_ID].endtime.second = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_2_ID].endtime.Wday = 0;
//   mixer.group[FLASHER_FUNCTION_MIXER_2_ID].enable_skip_restricted_by_time = 0;
//   mixer.group[FLASHER_FUNCTION_MIXER_2_ID].isenabled = true;
//   mixer.group[FLASHER_FUNCTION_MIXER_2_ID].transition_time_sec = 1;
//   mixer.group[FLASHER_FUNCTION_MIXER_2_ID].transition_rate_sec = 1;
//   mixer.group[FLASHER_FUNCTION_MIXER_2_ID].animation_transition_order = TRANSITION_ORDER_INORDER_ID;
//   mixer.group[FLASHER_FUNCTION_MIXER_2_ID].flashersettings_function = FLASHER_FUNCTION_PRESET_ID;
//   mixer.group[FLASHER_FUNCTION_MIXER_2_ID].run_time_duration_sec = 60;
//   mixer.group[FLASHER_FUNCTION_MIXER_2_ID].animation_palette = PALETTELIST_VARIABLE_USER_14_ID;
//   // Group 1
//   mixer.group[FLASHER_FUNCTION_MIXER_3_ID].time_on_secs_decounter = -1;
//   mixer.group[FLASHER_FUNCTION_MIXER_3_ID].starttime.hour = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_3_ID].starttime.minute = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_3_ID].starttime.second = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_3_ID].starttime.Wday = 0;
//   mixer.group[FLASHER_FUNCTION_MIXER_3_ID].endtime.hour = 17;
//     mixer.group[FLASHER_FUNCTION_MIXER_3_ID].endtime.minute = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_3_ID].endtime.second = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_3_ID].endtime.Wday = 0;
//   mixer.group[FLASHER_FUNCTION_MIXER_3_ID].enable_skip_restricted_by_time = 0;
//   mixer.group[FLASHER_FUNCTION_MIXER_3_ID].isenabled = true;
//   mixer.group[FLASHER_FUNCTION_MIXER_3_ID].transition_time_sec = 1;
//   mixer.group[FLASHER_FUNCTION_MIXER_3_ID].transition_rate_sec = 1;
//   mixer.group[FLASHER_FUNCTION_MIXER_3_ID].animation_transition_order = TRANSITION_ORDER_INORDER_ID;
//   mixer.group[FLASHER_FUNCTION_MIXER_3_ID].flashersettings_function = FLASHER_FUNCTION_PRESET_ID;
//   mixer.group[FLASHER_FUNCTION_MIXER_3_ID].run_time_duration_sec = 60;
//   mixer.group[FLASHER_FUNCTION_MIXER_3_ID].animation_palette = PALETTELIST_VARIABLE_USER_15_ID;
//   // Group 1
//   mixer.group[FLASHER_FUNCTION_MIXER_4_ID].time_on_secs_decounter = -1;
//   mixer.group[FLASHER_FUNCTION_MIXER_4_ID].starttime.hour = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_4_ID].starttime.minute = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_4_ID].starttime.second = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_4_ID].starttime.Wday = 0;
//   mixer.group[FLASHER_FUNCTION_MIXER_4_ID].endtime.hour = 17;
//     mixer.group[FLASHER_FUNCTION_MIXER_4_ID].endtime.minute = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_4_ID].endtime.second = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_4_ID].endtime.Wday = 0;
//   mixer.group[FLASHER_FUNCTION_MIXER_4_ID].enable_skip_restricted_by_time = 0;
//   mixer.group[FLASHER_FUNCTION_MIXER_4_ID].isenabled = true;
//   mixer.group[FLASHER_FUNCTION_MIXER_4_ID].transition_time_sec = 1;
//   mixer.group[FLASHER_FUNCTION_MIXER_4_ID].transition_rate_sec = 1;
//   mixer.group[FLASHER_FUNCTION_MIXER_4_ID].animation_transition_order = TRANSITION_ORDER_INORDER_ID;
//   mixer.group[FLASHER_FUNCTION_MIXER_4_ID].flashersettings_function = FLASHER_FUNCTION_PRESET_ID;
//   mixer.group[FLASHER_FUNCTION_MIXER_4_ID].run_time_duration_sec = 60;
//   mixer.group[FLASHER_FUNCTION_MIXER_4_ID].animation_palette = PALETTELIST_VARIABLE_USER_08_ID;
//   // Group 1
//   mixer.group[FLASHER_FUNCTION_MIXER_5_ID].time_on_secs_decounter = -1;
//   mixer.group[FLASHER_FUNCTION_MIXER_5_ID].starttime.hour = 16;
//     mixer.group[FLASHER_FUNCTION_MIXER_5_ID].starttime.minute = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_5_ID].starttime.second = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_5_ID].starttime.Wday = 0;
//   mixer.group[FLASHER_FUNCTION_MIXER_5_ID].endtime.hour = 17;
//     mixer.group[FLASHER_FUNCTION_MIXER_5_ID].endtime.minute = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_5_ID].endtime.second = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_5_ID].endtime.Wday = 0;
//   mixer.group[FLASHER_FUNCTION_MIXER_5_ID].enable_skip_restricted_by_time = 0;
//   mixer.group[FLASHER_FUNCTION_MIXER_5_ID].isenabled = true;
//   mixer.group[FLASHER_FUNCTION_MIXER_5_ID].transition_time_sec = 1;
//   mixer.group[FLASHER_FUNCTION_MIXER_5_ID].transition_rate_sec = 1;
//   mixer.group[FLASHER_FUNCTION_MIXER_5_ID].animation_transition_order = TRANSITION_ORDER_INORDER_ID;
//   mixer.group[FLASHER_FUNCTION_MIXER_5_ID].flashersettings_function = FLASHER_FUNCTION_PRESET_ID;
//   mixer.group[FLASHER_FUNCTION_MIXER_5_ID].run_time_duration_sec = 60;
//   mixer.group[FLASHER_FUNCTION_MIXER_5_ID].animation_palette = PALETTELIST_VARIABLE_USER_01_ID;
//   // Group 1
//   mixer.group[FLASHER_FUNCTION_MIXER_6_ID].time_on_secs_decounter = -1;
//   mixer.group[FLASHER_FUNCTION_MIXER_6_ID].starttime.hour = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_6_ID].starttime.minute = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_6_ID].starttime.second = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_6_ID].starttime.Wday = 0;
//   mixer.group[FLASHER_FUNCTION_MIXER_6_ID].endtime.hour = 17;
//     mixer.group[FLASHER_FUNCTION_MIXER_6_ID].endtime.minute = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_6_ID].endtime.second = 0;
//     mixer.group[FLASHER_FUNCTION_MIXER_6_ID].endtime.Wday = 0;
//   mixer.group[FLASHER_FUNCTION_MIXER_6_ID].enable_skip_restricted_by_time = 0;
//   mixer.group[FLASHER_FUNCTION_MIXER_6_ID].isenabled = true;
//   mixer.group[FLASHER_FUNCTION_MIXER_6_ID].transition_time_sec = 1;
//   mixer.group[FLASHER_FUNCTION_MIXER_6_ID].transition_rate_sec = 1;
//   mixer.group[FLASHER_FUNCTION_MIXER_6_ID].animation_transition_order = TRANSITION_ORDER_INORDER_ID;
//   mixer.group[FLASHER_FUNCTION_MIXER_6_ID].flashersettings_function = FLASHER_FUNCTION_PRESET_ID;
//   mixer.group[FLASHER_FUNCTION_MIXER_6_ID].run_time_duration_sec = 60;
//   mixer.group[FLASHER_FUNCTION_MIXER_6_ID].animation_palette = PALETTELIST_VARIABLE_USER_17_ID;


}



// void mRGBAnimator::SubTask_Flasher_Animate_Mixer(){

//   uint8_t flag_reset = false;

//   // Run every second
//   if(mSupport::TimeReachedNonReset(&mixer.tSavedTrigger,1000)||pCONT_set->fSystemRestarted){

//     if(mixer.group[mixer.running_id].time_on_secs_decounter == 0){ 
      
//       AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEO "mixer.group[mixer.running_id].time_on_secs_decounter =%d END"),mixer.group[mixer.running_id].time_on_secs_decounter);

//       uint8_t new_group_found = false;
//       //mixer.fForceSendMQTT = true;
      
//       // Next mode 
//       mixer.running_id++;
//       // wrap around
//       if(mixer.running_id>FLASHER_FUNCTION_MIXER_NONE_ID){
//         mixer.running_id = FLASHER_FUNCTION_MIXER_0_ID;
//       }

//       if(!pCONT->mt->CheckBetween_Day_DateTimesShort(&mixer.group[mixer.running_id].starttime,
//                                                    &mixer.group[mixer.running_id].endtime)){
//         //add here to pass if the running_id is restricted
//         if(mixer.group[mixer.running_id].enable_skip_restricted_by_time){ //if true, skip
//           do{ //skip until we reach an acceptable mixer      
//             mixer.running_id++;     
//             if(mixer.running_id>FLASHER_FUNCTION_MIXER_NONE_ID){
//               mixer.running_id = FLASHER_FUNCTION_MIXER_0_ID;
//             }
//             AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"Skipping restricted mode = WHILE LO OP MULTIPLE");
//           }while(mixer.group[mixer.running_id].enable_skip_restricted_by_time);
//           AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"Skipping restricted mode");
//           //mixer.times.skip_restricted_by_time_isactive = true;
//         }else{
//           //mixer.times.skip_restricted_by_time_isactive = false;
//           AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"NOT Skipping restricted mode");
//         }
//         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"OUTSIDE restricted CheckBetween_Day_DateTimes");
//       }else{
//         //mixer.mode.times.skip_restricted_by_time_isactive = false;
//         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"INSIDE restricted CheckBetween_Day_DateTimes");
//       }


//       // Progress to next active mode IF ACTIVE
//       // for(id_next=id;id_next<FLASHER_FUNCTION_MIXER_NONE_ID;id_next){
//       //   if(mixer.group[id_next].isenabled){
//       //     // IF time is valid
//       //     //if(pCONT->mt->mtime.isvalid){
//       //       // IF is time restricted AND is not between those times THEN skip it
//       //     //   if(mixer.group[id_next].enable_skip_restricted_by_time
//                    //   &&(!pCONT->mt->CheckBetween_Day_DateTimes(&mixer.mode.times.flashing_starttime,&mixer.mode.times.flashing_endtime))){// If time restrictions are not prohibating group
//       //     //     id_next++;
//       //     //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"if enable_skip_restricted_by_time");
//       //     //     break; //finish for lop with current id_next
//       //     //   }else{
//       //          //AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"ELSE enable_skip_restricted_by_time");
//       //     //   }
//       //     // }else{ // If time is not valid, but restrictions are enabled, then skip REGARDLESS of time periods
//       //          id_next++;
//       //     //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"ELSE time invalid");
//       //     //     break; //finish for lop with current id_next
//       //     // }// end if(isvalid)
//       //   }else{// end if(isenabled)
//       //     id_next++; // do move on
//       //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"ELSE not enabled");
//       //   }
//       // }// end FOR



//       // Progress to next active mode IF ACTIVE
//       // for(id_next=id;id_next<FLASHER_FUNCTION_MIXER_NONE_ID;id_next){
//        // do{
//       //   if(mixer.group[id_next].isenabled){
//       //     // IF time is valid
//       //     //if(pCONT->mt->mtime.isvalid){
//       //       // IF is time restricted AND is not between those times THEN skip it
//       //     //   if(mixer.group[id_next].enable_skip_restricted_by_time&&(!pCONT->mt->CheckBetween_Day_DateTimes(&mixer.mode.times.flashing_starttime,&mixer.mode.times.flashing_endtime))){// If time restrictions are not prohibating group
//       //     //     id_next++;
//       //     //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"if enable_skip_restricted_by_time");
//       //     //     break; //finish for lop with current id_next
//       //     //   }else{
//       //          //AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"ELSE enable_skip_restricted_by_time");
//       //     //   }
//       //     // }else{ // If time is not valid, but restrictions are enabled, then skip REGARDLESS of time periods
//       //          id_next++;
//       //     //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"ELSE time invalid");
//       //     //     break; //finish for lop with current id_next
//       //     // }// end if(isvalid)
//       //   }else{// end if(isenabled)
//       //     id_next++; // do move on
//       //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"ELSE not enabled");
//       //   }
//       // }// end FOR
      
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\"=%d"),"mixer.running_id",mixer.running_id);

//       // if(mixer.running_id >= FLASHER_FUNCTION_MIXER_NONE_ID){ //lop around, move forward, but retry function on next go around
//       //   mixer.running_id = FLASHER_FUNCTION_MIXER_0_ID;
//       //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "\"%s\""),"mixer.running_id = FLASHER_FUNCTION_MIXER_0_ID;");
//       //   return; // Quit early
//       // }
      
//       // Reset after return, so this function will be called multiple times until it passes this point
//       mixer.group[mixer.running_id].time_on_secs_decounter = -1; //STOP
//       mixer.tSavedTrigger = millis();

//       // Only runs if new state was found
//       update_all = true; // change all leds on new refresh
//       flashersettings.region = FLASHER_REGION_COLOUR_SELECT_ID; // refresh by calling init colour generator

//       AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEO "mixer.running_id = %d"),mixer.running_id);

//       pCONT_iLight->animation.palette_id = mixer.group[mixer.running_id].animation_palette;          
//       pCONT_iLight->animation.transition.order_id = mixer.group[mixer.running_id].animation_transition_order;
//       flashersettings.function = mixer.group[mixer.running_id].flashersettings_function;          
//       mixer.group[mixer.running_id].time_on_secs_decounter = mixer.group[mixer.running_id].run_time_duration_sec*((float)mixer.run_time_duration_scaler_as_percentage/100);
//       //mixer.time_on_secs_active = mixer.group[mixer.running_id].run_time_duration_sec;
//       pCONT_iLight->animation.transition.time_ms = mixer.group[mixer.running_id].transition_time_sec*1000;
//       pCONT_iLight->animation.transition.rate_ms = mixer.group[mixer.running_id].transition_rate_sec*1000;
      
//       // "palette|order|function" eg "USER17|RANDOM|SEQUENTIAL"
//       sprintf(mixer.running_friendly_name_ctr,"%s|%s|%s\0",
//                 GetPaletteFriendlyName(),
//                 GetTransitionMethodName(),
//                 GetFlasherFunctionName()                  
//               ); // palette|blend|flasher|

//     }else{ 
//       mixer.group[mixer.running_id].time_on_secs_decounter--; //if on
//       mixer.tSavedTrigger = millis(); //reset if this was reached
//       //AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEO "Time until MillisReached = %d"),mixer.tSavedMillisToChangeAt-millis());
//       AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEO "mixer.group[mixer.running_id].time_on_secs_decounter = %d"),mixer.group[mixer.running_id].time_on_secs_decounter);  
    
//       if(mixer.group[mixer.running_id].time_on_secs_decounter < -2){
//         mixer.group[mixer.running_id].time_on_secs_decounter = 0;
//         AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEO "RESET < -1 mixer.group[mixer.running_id].time_on_secs_decounter = %d"),mixer.group[mixer.running_id].time_on_secs_decounter);  
//       }
//     }
    
//     //mixer.fForceSendMQTT = true;

//   } //tSavedTrigger  

// } // SubTask_Flasher_Animate


#endif //ENABLE_PIXEL_FUNCTION_MIXER

int8_t mRGBAnimator::GetFlasherFunctionIDbyName(const char* c){
  if(c=='\0') return -1;
//   if(strstr(c,D_FLASHER_FUNCTION_STATIC_NAME_CTR))        return FLASHER_FUNCTION_STATIC_ID;
//   if(strstr(c,D_FLASHER_FUNCTION_SEQUENTIAL_NAME_CTR))     return FLASHER_FUNCTION_SEQUENTIAL_ID;
//   if(strstr(c,D_FLASHER_FUNCTION_SLO_GLO_NAME_CTR))        return FLASHER_FUNCTION_SLO_GLO_ID;
//   //if(strstr(c,D_FLASHER_FUNCTION_CHASING_FLASH_NAME_CTR))  return FLASHER_FUNCTION_CHASING_FLASH_ID;
//   if(strstr(c,D_FLASHER_FUNCTION_SLOW_FADE_BRIGHTNESS_ALL_NAME_CTR))      return FLASHER_FUNCTION_SLOW_FADE_BRIGHTNESS_ALL_ID;
//   if(strstr(c,D_FLASHER_FUNCTION_SLOW_FADE_BRIGHTNESS_RANDOM_NAME_CTR))      return FLASHER_FUNCTION_SLOW_FADE_BRIGHTNESS_RANDOM_ID;
//   if(strstr(c,D_FLASHER_FUNCTION_SLOW_FADE_SATURATION_ALL_NAME_CTR))      return FLASHER_FUNCTION_SLOW_FADE_SATURATION_ALL_ID;
//   if(strstr(c,D_FLASHER_FUNCTION_SLOW_FADE_SATURATION_RANDOM_NAME_CTR))      return FLASHER_FUNCTION_SLOW_FADE_SATURATION_RANDOM_ID;
//   if(strstr(c,D_FLASHER_FUNCTION_FLASH_TWINKLE_RANDOM_NAME_CTR))  return FLASHER_FUNCTION_FLASH_TWINKLE_RANDOM_ID;
//   if(strstr(c,D_FLASHER_FUNCTION_FLASH_TWINKLE_SEQUENTIAL_NAME_CTR))  return FLASHER_FUNCTION_FLASH_TWINKLE_SEQUENTIAL_ID;
  return -1;
}
const char* mRGBAnimator::GetFlasherFunctionName(char* buffer){
  return GetFlasherFunctionNamebyID(flashersettings.function, buffer);
}
const char* mRGBAnimator::GetFlasherFunctionNamebyID(uint8_t id, char* buffer){
  switch(id){
    default:
    case FLASHER_FUNCTION_SEQUENTIAL_ID:  memcpy_P(buffer, PM_FLASHER_FUNCTION_SEQUENTIAL_NAME_CTR, sizeof(PM_FLASHER_FUNCTION_SEQUENTIAL_NAME_CTR)); break;
    case FLASHER_FUNCTION_SLOW_GLOW_ID:   memcpy_P(buffer, PM_ANIMATION_MODE_AMBILIGHT_NAME_CTR, sizeof(PM_ANIMATION_MODE_AMBILIGHT_NAME_CTR)); break;


  //   case FLASHER_FUNCTION_SLOW_FADE_BRIGHTNESS_ALL_ID:      return D_FLASHER_FUNCTION_SLOW_FADE_BRIGHTNESS_ALL_NAME_CTR;
  //   case FLASHER_FUNCTION_SLOW_FADE_BRIGHTNESS_RANDOM_ID:      return D_FLASHER_FUNCTION_SLOW_FADE_BRIGHTNESS_RANDOM_NAME_CTR;
  //   case FLASHER_FUNCTION_SLOW_FADE_SATURATION_ALL_ID:      return D_FLASHER_FUNCTION_SLOW_FADE_SATURATION_ALL_NAME_CTR;
  //   case FLASHER_FUNCTION_SLOW_FADE_SATURATION_RANDOM_ID:      return D_FLASHER_FUNCTION_SLOW_FADE_BRIGHTNESS_RANDOM_NAME_CTR;
  //   case FLASHER_FUNCTION_FLASH_TWINKLE_RANDOM_ID:  return D_FLASHER_FUNCTION_FLASH_TWINKLE_RANDOM_NAME_CTR;
  //   case FLASHER_FUNCTION_FLASH_TWINKLE_SEQUENTIAL_ID:  return D_FLASHER_FUNCTION_FLASH_TWINKLE_SEQUENTIAL_NAME_CTR;
  }
  return buffer;
}


int8_t mRGBAnimator::GetFlasherRegionIDbyName(const char* c){
  if(c=='\0') return -1;
  if(     strcmp_P(c,PM_FLASHER_REGION_COLOUR_SELECT_NAME_CTR)==0){ return FLASHER_REGION_COLOUR_SELECT_ID; }
  else if(strcmp_P(c,PM_FLASHER_REGION_ANIMATE_NAME_CTR)==0){ return FLASHER_REGION_ANIMATE_ID; }
  return -1;
}
const char* mRGBAnimator::GetFlasherRegionName(char* buffer){
  return GetFlasherRegionNamebyID(flashersettings.region, buffer);
}
const char* mRGBAnimator::GetFlasherRegionNamebyID(uint8_t id, char* buffer){
  switch(id){
    case FLASHER_REGION_COLOUR_SELECT_ID:    memcpy_P(buffer,PM_FLASHER_REGION_COLOUR_SELECT_NAME_CTR,sizeof(PM_FLASHER_REGION_COLOUR_SELECT_NAME_CTR));
  case FLASHER_REGION_ANIMATE_ID:    memcpy_P(buffer,PM_FLASHER_REGION_ANIMATE_NAME_CTR,sizeof(PM_FLASHER_REGION_ANIMATE_NAME_CTR));
  }
  return buffer;
}

void mRGBAnimator::SubTask_Flasher_Main(){

  // if(mixer.enabled){
  //  pCONT_set->Settings.enable_sleep = false; // Turn sleep function off during flasher
  //  SubTask_Flasher_Animate_Mixer();
  // }

  SubTask_Flasher_Animate();

} // SubTask_Flasher_Main




void mRGBAnimator::SubTask_Flasher_Animate(){
  
  if((mSupport::TimeReached(&flashersettings.tSaved.Update,pCONT_iLight->animation.transition.rate_ms.val))||(pCONT_iLight->animation.flags.fForceUpdate)){

    if(pCONT_iLight->animation.flags.fForceUpdate){ pCONT_iLight->animation.flags.fForceUpdate=false;
      flashersettings.tSaved.Update = millis();
    }

    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "flashersettings.tSaved.Update"));
    // #endif

    flashersettings.function = FLASHER_FUNCTION_SLOW_GLOW_ID;

    switch(flashersettings.function){
      default:
      case FLASHER_FUNCTION_SLOW_GLOW_ID:   //previously presets method and replaces random
        SubTask_Flasher_Animate_Function_Slow_Glow();
      break;
      case FLASHER_FUNCTION_SEQUENTIAL_ID:
        SubTask_Flasher_Animate_Function_Sequential();
      break;
      case FLASHER_FUNCTION_FLASH_TWINKLE_RANDOM_ID:
        SubTask_Flasher_Animate_Function_Twinkle_Random();
      break;
      case FLASHER_FUNCTION_FLASH_TWINKLE_SEQUENTIAL_ID:
        SubTask_Flasher_Animate_Function_Twinkle_Sequential();
      break;
      case FLASHER_FUNCTION_SLOW_FADE_BRIGHTNESS_ALL_ID:
        SubTask_Flasher_Animate_Function_Slow_Fade_Brightness_All();
      break;
      case FLASHER_FUNCTION_SLOW_FADE_SATURATION_ALL_ID:
        SubTask_Flasher_Animate_Function_Slow_Fade_Saturation_All();
      break;
    } //end SWITCH

    ConfigureLEDTransitionAnimation();

  }//end if update reached

  // Generate randomness to transitions if flag is set
  // if(flashersettings.flags.enable_random_rate){
  //   if(mSupport::TimeReached(&flashersettings.random_transitions.tSavedNewSpeedUpdate,flashersettings.random_transitions.rate_ms)){
  //     flashersettings.random_transitions.rate_ms = (random(flashersettings.random_transitions.lower_secs,flashersettings.random_transitions.upper_secs)*1000);
  //     pCONT_iLight->animation.transition.rate_ms.val = flashersettings.random_transitions.array[flashersettings.random_transitions.array_index++]*10;
  //     if(flashersettings.random_transitions.array_index>=flashersettings.random_transitions.array_index_length) flashersettings.random_transitions.array_index=0;
  //     pCONT_iLight->animation.transition.time_ms.val = pCONT_iLight->animation.transition.rate_ms.val/4; //75% of time spent on desired colour
  //   }
  // }

} //end flasher_animate



void mRGBAnimator::SubTask_Flasher_Animate_Function_Slow_Glow(){
  RefreshLED_Presets();
}

void mRGBAnimator::SubTask_Flasher_Animate_Function_Sequential(){
  
  flashersettings.flag_finish_flasher_pair = false;
  flashersettings.flags.enable_random_rate = true;
  
  do{//must complete the pair together //move inside functions
    switch(flashersettings.region){
      case FLASHER_REGION_COLOUR_SELECT_ID: //set colours
        #ifdef ENABLE_LOG_LEVEL_DEBUG
        AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_COLOUR_SELECT"));
        #endif
        RefreshLED_Presets();
        flashersettings.region = FLASHER_REGION_ANIMATE_ID;
      break;
      case FLASHER_REGION_ANIMATE_ID: //shift along
        #ifdef ENABLE_LOG_LEVEL_DEBUG
        AddLog_P(LOG_LEVEL_DEBUG_LOWLEVEL,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_ANIMATE"));
        #endif
        // Shift colours (rotate)
        RgbTypeColor colourfirst = animation_colours[0].DesiredColour;//desired_colour[0];
        // Shift towards first pixel
        for(ledout.index=0;ledout.index<strip_size;ledout.index++){ 
          animation_colours[ledout.index].DesiredColour = animation_colours[ledout.index+1].DesiredColour;
        }
        // Put first pixel on the end (wrap around)
        animation_colours[ledout.index-1].DesiredColour = colourfirst;

        // Make this auto changing park of "mixer", some sort of changing mode flag

        // if(mSupport::TimeReached(&flashersettings.function_seq.tSavedNewSpeedUpdate,(random(3,10)*1000))){
        //   uint8_t values[8] = {4,8,10,17,20,17,10,8};
        //   pCONT_iLight->animation.transition.rate_ms = values[flashersettings.function_seq.rate_index++]*50;
        //   if(flashersettings.function_seq.rate_index>8) flashersettings.function_seq.rate_index=0;
        //   pCONT_iLight->animation.transition.time_ms = pCONT_iLight->animation.transition.rate_ms/4; //75% of time spent on desired colour
        // }
      break;
    }
  }while(flashersettings.flag_finish_flasher_pair);

  
// // if(mSupport::TimeReached(&flashersettings.tSaved.Update2,60000)){
// //   flashersettings.region=FLASHER_REGION_COLOUR_SELECT_ID;
// //   AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEO "flashersettings TEMP FIX METHOD"));
// // }

}


void mRGBAnimator::SubTask_Flasher_Animate_Function_Twinkle_Random(){

#ifdef ENABLE_ADVANCED_MODE_TWINKLE // creating this to reduce "white_leds_index" size

  switch(flashersettings.region){
    case FLASHER_REGION_COLOUR_SELECT_ID: //set colours
      AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_COLOUR_SELECT"));
      RefreshLED_Presets();
      flashersettings.region = FLASHER_REGION_ANIMATE_ID;
      pCONT_iLight->animation.transition.order_id = TRANSITION_ORDER_INORDER_ID;

      flash_twinkle_random.white_pixel_amount = 8; //how often the leds are
      // by percentage
      //flash_twinkle_random.white_pixel_amount = strip_size/5;

      // Generate ledindex for white flashers
      flash_twinkle_random.white_total_index = 0;
      for(ledout.index=0;ledout.index<strip_size;ledout.index++){ 
        //map type of led they are by numnber 0=off,1=white,2=colour
        if(!(ledout.index%flash_twinkle_random.white_pixel_amount)){
          flash_twinkle_random.white_leds_index[flash_twinkle_random.white_total_index] = ledout.index;
          flash_twinkle_random.stored_colours_index[flash_twinkle_random.white_total_index] = desired_colour[ledout.index];
          //AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "flash_twinkle_random.white_leds_index %d ledout.index %d"),flash_twinkle_random.white_total_index,ledout.index);
          flash_twinkle_random.white_total_index++;
        }              
      }   
                  
      //flash_twinkle_random.flash_colour = HsbColor(0,0,1); //white

      // Test clear
      // for(int ledout.index=0;ledout.index<strip_size;ledout.index++){ desired_colour[ledout.index] = HsbColor(RgbColor(0,0,0)); }

      flashersettings.flag_finish_flasher_pair = true; // must complete regions
      
    break;

    case FLASHER_REGION_ANIMATE_ID: //shift along
      AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_ANIMATE"));

      // Reset all back to colour first 
      for(int jj=0;jj<flash_twinkle_random.white_total_index;jj++){
        desired_colour[flash_twinkle_random.white_leds_index[jj]] = flash_twinkle_random.stored_colours_index[jj];
      }

      // Pick 10 random of the leds to turn to white

      
      for(int jj=0;jj<10;jj++){
        uint16_t led_rand = random(0,flash_twinkle_random.white_total_index);
        desired_colour[flash_twinkle_random.white_leds_index[led_rand]] = flash_twinkle_random.flash_colour;
      }

      flash_twinkle_random.white_on_index++;
      if(flash_twinkle_random.white_on_index>=flash_twinkle_random.white_pixel_amount){
        flash_twinkle_random.white_on_index = 0;
        flashersettings.region = FLASHER_REGION_COLOUR_SELECT_ID; // Wrap back
      }

      flashersettings.flag_finish_flasher_pair = false; // must complete regions

      // Change pCONT_iLight->animation speed
      if(mSupport::TimeReached(&flashersettings.function_flash_twinkle.tSavedNewSpeedUpdate,random(0,1)?5000:10000)){//(random(5,10)*1000))){
        pCONT_iLight->animation.transition.rate_ms.val = random(0,1)?10:150; // will need to be very fast
        pCONT_iLight->animation.transition.time_ms.val = 0; //instant
      }
    break;
  }

  #endif

}


void mRGBAnimator::SubTask_Flasher_Animate_Function_Twinkle_Sequential(){
  
//       case FLASHER_FUNCTION_FLASH_TWINKLE_SEQUENTIAL_ID:
//         switch(flashersettings.region){
//           case FLASHER_REGION_COLOUR_SELECT_ID: //set colours
//             AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_COLOUR_SELECT"));
//             RefreshLED_Presets();
//             flashersettings.region = FLASHER_REGION_ANIMATE_ID;
//             pCONT_iLight->animation.transition.order_id = TRANSITION_ORDER_INORDER_ID;

//             flash_twinkle_random.white_pixel_amount = 7; //how often the leds are
//             // by percentage
//             //flash_twinkle_random.white_pixel_amount = strip_size/5;

//             // Generate ledindex for white flashers
//             flash_twinkle_random.white_total_index = 0;
//             for(ledout.index=0;ledout.index<strip_size;ledout.index++){ 
//               //map type of led they are by numnber 0=off,1=white,2=colour
//               if(!(ledout.index%flash_twinkle_random.white_pixel_amount)){
//                 flash_twinkle_random.white_leds_index[flash_twinkle_random.white_total_index] = ledout.index;
//                 flash_twinkle_random.stored_colours_index[flash_twinkle_random.white_total_index] = desired_colour[ledout.index];
//                 //AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "flash_twinkle_random.white_leds_index %d ledout.index %d"),flash_twinkle_random.white_total_index,ledout.index);
//                 flash_twinkle_random.white_total_index++;
//               }              
//             }   
                        
//             //flash_twinkle_random.flash_colour = HsbColor(0,0,1); //white
//             //flash_twinkle_random.flash_colour = HsbColor(HueN2F(HUE_HOTPINK),SatN2F(100),BrtN2F(100));

//             // Test clear
//             // for(int ledout.index=0;ledout.index<strip_size;ledout.index++){ desired_colour[ledout.index] = HsbColor(RgbColor(0,0,0)); }

//             flashersettings.flag_finish_flasher_pair = true; // must complete regions
            
//           break;
//           case FLASHER_REGION_ANIMATE_ID: //shift along
//             AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_ANIMATE"));
    
            
//             for(int jj=0;jj<flash_twinkle_random.white_total_index;jj++){
//               if((jj%flash_twinkle_random.white_pixel_amount)==flash_twinkle_random.white_on_index){
//                 desired_colour[flash_twinkle_random.white_leds_index[jj]] = flash_twinkle_random.flash_colour;
//               }else{
//                 desired_colour[flash_twinkle_random.white_leds_index[jj]] = flash_twinkle_random.stored_colours_index[jj];//offcolour;
//               }
//               // AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "leds_index=%d on/amount=%d/%d on=%d"),
//               // flash_twinkle_random.white_leds_index[jj],
//               // flash_twinkle_random.white_on_index,
//               // flash_twinkle_random.white_pixel_amount,
//               // (jj%flash_twinkle_random.white_pixel_amount)==0?1:0);
//             }

//             flash_twinkle_random.white_on_index++;
//             if(flash_twinkle_random.white_on_index>=flash_twinkle_random.white_pixel_amount){
//               flash_twinkle_random.white_on_index = 0;
//               flashersettings.region = FLASHER_REGION_COLOUR_SELECT_ID; // Wrap back
//             }

//             flashersettings.flag_finish_flasher_pair = false; // must complete regions

//             // Change pCONT_iLight->animation speed
//             if(mSupport::TimeReached(&flashersettings.function_flash_twinkle.tSavedNewSpeedUpdate,random(0,1)?5000:10000)){//(random(5,10)*1000))){
//               pCONT_iLight->animation.transition.rate_ms = 90;//random(0,1)?30:30; // will need to be very fast
//               pCONT_iLight->animation.transition.time_ms = 0; //instant
//             }
//           break;
//         }
//       break;

}

void mRGBAnimator::SubTask_Flasher_Animate_Function_Slow_Fade_Brightness_All(){
     
      
//       case FLASHER_FUNCTION_SLOW_FADE_BRIGHTNESS_ALL_ID:
//         switch(flashersettings.region){
//           case FLASHER_REGION_COLOUR_SELECT_ID: //set colours
//             AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_COLOUR_SELECT"));
//             RefreshLED_Presets();
//             flashersettings.region = FLASHER_REGION_ANIMATE_ID;
//           break;
//           case FLASHER_REGION_ANIMATE_ID: //shift along
//             AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_ANIMATE"));
//             // Change brightness from 0 to 100% (rotate)
//             uint8_t direction = flashersettings.function_seq.rate_index%2;//(flashersettings.function_slow_fade.direction^=1);
//             for(ledout.index=0;ledout.index<strip_size;ledout.index++){ desired_colour[ledout.index].B = direction; }            
//             // Change pCONT_iLight->animation speed
//             if(mSupport::TimeReached(&flashersettings.function_seq.tSavedNewSpeedUpdate,(random(3,10)*100))){
//               uint8_t values[8] = {1000,1000,2000,2000,6000,6000,3000,3000}; //off,on,off
//               pCONT_iLight->animation.transition.rate_ms = values[flashersettings.function_seq.rate_index++]*50;
//               if(flashersettings.function_seq.rate_index>=8) flashersettings.function_seq.rate_index=0;
//               pCONT_iLight->animation.transition.time_ms = pCONT_iLight->animation.transition.rate_ms;///4; // brightness shift takes 100% of the time (no colour shift)
//             }
//           break;
//         }
//       break;
}


void mRGBAnimator::SubTask_Flasher_Animate_Function_Slow_Fade_Saturation_All(){
      
//       case FLASHER_FUNCTION_SLOW_FADE_SATURATION_ALL_ID:
//         switch(flashersettings.region){
//           case FLASHER_REGION_COLOUR_SELECT_ID: //set colours
//             AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_COLOUR_SELECT"));
//             RefreshLED_Presets();
//             flashersettings.region = FLASHER_REGION_ANIMATE_ID;
//           break;
//           case FLASHER_REGION_ANIMATE_ID: //shift along
//             AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_ANIMATE"));
//             // Change brightness from 0 to 100% (rotate)
//             uint8_t direction = flashersettings.function_seq.rate_index%2;//(flashersettings.function_slow_fade.direction^=1);
//             float random_saturation = direction ? 1 : ((float)random(0,100)/(float)100);

//             float adjusted_brightness = random_saturation;// mSupport::mapfloat(random_saturation,
//             if(random_saturation<0.6){
//               adjusted_brightness = mSupport::mapfloat(random_saturation,
//                                                                     0,  0.6,
//                                                                     0.4,0.6);
//             }
                        
//             for(ledout.index=0;ledout.index<strip_size;ledout.index++){ 
//               desired_colour[ledout.index].S = random_saturation; 
//               desired_colour[ledout.index].B = adjusted_brightness;//random_saturation<0.5?pCONT_iLight->animation.brightness*0.5:pCONT_iLight->animation.brightness; //test, pair "whiter" less bright (maybe /2)  
//             }            
//             // Change pCONT_iLight->animation speed
//             //if(mSupport::TimeReached(&flashersettings.function_seq.tSavedNewSpeedUpdate,(random(3,10)*100))){
//               uint16_t values[8] = {7,7,9,9,7,7,4,4}; //off,on,off
//               pCONT_iLight->animation.transition.rate_ms = values[flashersettings.function_seq.rate_index++]*1000;
//               if(flashersettings.function_seq.rate_index>=8) flashersettings.function_seq.rate_index=0;
//               // holds colour more
//               //pCONT_iLight->animation.transition.time_ms = direction ? (pCONT_iLight->animation.transition.rate_ms.val/4) : (pCONT_iLight->animation.transition.rate_ms.val); // brightness shift takes 100% of the time (no colour shift)
//               pCONT_iLight->animation.transition.time_ms = random(0,1) ? (pCONT_iLight->animation.transition.rate_ms.val/4) : (pCONT_iLight->animation.transition.rate_ms.va;); // brightness shift takes 100% of the time (no colour shift)
//             //}
//           break;
//         }
//       break;
//       //flasher_start_white_to_Colour: 
//         // method that will start all leds on one colour (white?) and slowly add colours in with fade in
//       //flasher_change_saturation_between 0 and 100, "fade of saturation", "fade of brightness"

}


void mRGBAnimator::parsesub_Flasher(JsonObjectConst obj){
  
  int8_t tmp_id = 0;
  char buffer[50];

  if(!obj["function"].isNull()){ 
    const char* functionctr = obj["function"];
    if((tmp_id=GetFlasherFunctionIDbyName(functionctr))>=0){
      flashersettings.function = tmp_id;

      //new function, then generate its 
      flashersettings.region = FLASHER_REGION_COLOUR_SELECT_ID;
      #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),"function",GetFlasherRegionName(buffer));
      #endif // ENABLE_LOG_LEVEL_INFO_PARSING
      Response_mP(S_JSON_COMMAND_SVALUE,"function",GetFlasherFunctionName(buffer));
      data_buffer2.isserviced++;
    }else{
      #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
      AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_NAME,functionctr);
      #endif // #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    }
  }


  if(!obj["flasher"]["function"].isNull()){ 
    flashersettings.function = obj["flasher"]["function"];
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),"flasherfunction",flashersettings.function);
    #endif // ENABLE_LOG_LEVEL_INFO_PARSING
  }
   
  
  #ifdef ENABLE_PIXEL_FUNCTION_MIXER
  if(!obj["mixer"]["enabled"].isNull()){ 
    mixer.enabled = obj["mixer"]["enabled"];
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),"mixer.enabled",mixer.enabled);
    #endif // #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
  }
   
  if(!obj["time_scaler"].isNull()){ 
    mixer.time_scaler = obj["time_scaler"];
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),"mixer.time_scaler",mixer.time_scaler);
    #endif //#ifdef ENABLE_LOG_LEVEL_INFO_PARSING
  }
  #endif //ENABLE_PIXEL_FUNCTION_MIXER

  // TIME with different units
  if(!obj[D_JSON_TIME].isNull()){ //default to secs
    pCONT_iLight->animation.transition.time_ms.val = obj["time"];
    pCONT_iLight->animation.transition.time_ms.val *= 1000;
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),pCONT_iLight->animation.transition.time_ms.val);  
    #endif // ENABLE_LOG_LEVEL_INFO_PARSING
  }else
  if(!obj[D_JSON_TIME].isNull()){
    pCONT_iLight->animation.transition.time_ms.val = obj["time_secs"];
    pCONT_iLight->animation.transition.time_ms.val *= 1000;
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),pCONT_iLight->animation.transition.time_ms.val);  
    #endif // ENABLE_LOG_LEVEL_INFO_PARSING
  }else
  if(!obj[D_JSON_TIME_MS].isNull()){
    pCONT_iLight->animation.transition.time_ms.val = obj["time_ms"];
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),pCONT_iLight->animation.transition.time_ms.val);  
    #endif // ENABLE_LOG_LEVEL_INFO_PARSING
  }

} // END FUNCTION


// // Update struct that shows overview, always sends
uint8_t mRGBAnimator::ConstructJSON_Flasher(uint8_t json_level){


//   AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::ConstructJSON_Flasher"));
//   DynamicJsonDocument doc(1500);
//   JsonObject root = doc.to<JsonObject>();

//   // root[D_JSON_ONOFF] = pCONT_iLight->light_power ? "ON" : "OFF";
//   // root[D_JSON_MODE] = GetAnimationModeName();
//   root["function"] = GetFlasherFunctionName();
//   root["region"] = GetFlasherRegionName();
//   // root[D_JSON_COLOUR_PALETTE] = GetPaletteFriendlyName();
//   // root[D_JSON_BRIGHTNESS_PERCENTAGE] = pCONT_iLight->animation.brightness*100;
//   // root[D_JSON_BRIGHTNESS] = pCONT_iLight->animation.brightness;

//   // JsonObject transitionobj = root.createNestedObject(D_JSON_TRANSITION);
//   //   transitionobj[D_JSON_METHOD] = GetTransitionMethodName();
//   //   transitionobj[D_JSON_TIME] = mSupport::safeDivideInt(pCONT_iLight->animation.transition.time_ms,1000);
//   //   transitionobj[D_JSON_TIME_MS] = pCONT_iLight->animation.transition.time_ms;
//   //   transitionobj[D_JSON_RATE] = mSupport::safeDivideInt(pCONT_iLight->animation.transition.rate_ms,1000);
//   //   transitionobj[D_JSON_RATE_MS] = pCONT_iLight->animation.transition.rate_ms;
//   //   transitionobj["function"] = GetFlasherFunctionName();

//   JsonObject seq_obj = root.createNestedObject("sequential");
//     seq_obj["rate_index"] = flashersettings.function_seq.rate_index;

//   JsonObject sloglo_obj = root.createNestedObject("slo_glo");
//     sloglo_obj["rate_index"] = flashersettings.function_slo_glo.rate_index;



//   // Flags and states that are used during one transition and reset when completed
//   // JsonObject overridesobj = root.createNestedObject("transition_overrides"); 
//   //   overridesobj["fRefreshAllPixels"] = pCONT_iLight->animation_override.fRefreshAllPixels;
  
//   // root["ledout.length"] = ledout.length;

//   data_buffer2.payload.len = measureJson(root)+1;
//   serializeJson(doc,data_buffer2.payload.ctr);

//   return (data_buffer2.payload.len>3?1:0);
  return 0;
}



uint8_t mRGBAnimator::ConstructJSON_Mixer(uint8_t json_level){

//   AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::ConstructJSON_Mixer"));

//   DynamicJsonDocument doc(1400);
//   JsonObject root = doc.to<JsonObject>();

//   //active values

//   //all mixer values in other mqtt

//   root["enabled"] = mixer.enabled;
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



    
//     JsonArray enable_skip_restricted_by_time_arr = timesobj.createNestedArray("enable_skip_restricted_by_time");
//     for(int i=0;i<FLASHER_FUNCTION_MIXER_MAX;i++){
//       //enable_skip_restricted_by_time_arr.add(mixer.group[i].times.enable_skip_restricted_by_time);
//     }

//     JsonArray time_on_secs_arr = timesobj.createNestedArray("time_on_secs");
//     for(int i=0;i<FLASHER_FUNCTION_MIXER_MAX;i++){
//       //time_on_secs_arr.add(mixer.mode.time_on_secs[i]);
//     }

//   data_buffer2.payload.len = measureJson(root)+1;
//   serializeJson(doc,data_buffer2.payload.ctr);

// return (data_buffer2.payload.len>3?1:0);
  return 0;

}



// #endif // USE_FLASHER_AND_MIXER




/*******************************************************************************************************************
********************************************************************************************************************
************ END OF ANIMATION/FLASHER/MIXER DEFINITIONS ********************************************************************************************
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

const char* mRGBAnimator::GetAnimationStatusCtr(char* buffer){

  if(pCONT_iLight->animator_controller->IsAnimating()){
    sprintf(buffer, "%s", "Animating");
    return buffer;
  }
  if(pCONT_iLight->animation.flags.fEnable_Animation){
    // (millis since) + (next event millis)
    int until_next_millis = pCONT_iLight->animation.transition.rate_ms.val-(millis()-pCONT_iLight->animation_changed_millis);
    float secs_until_next_event = (float)(until_next_millis);
    secs_until_next_event/=1000;
    // Serial.println(secs_until_next_event);

    char float_ctr[10];
    dtostrf(secs_until_next_event,3,1,float_ctr);
    // Serial.println(float_ctr);

    // AddLog_P(LOG_LEVEL_INFO,PSTR("GetAnimationStatusCtr %d %d"),
    //   until_next_millis,
    //   //millis(),pCONT_iLight->animation_changed_millis,pCONT_iLight->animation.transition.rate_ms.val,
    //   secs_until_next_event
    // );  
    
     sprintf(buffer,PSTR("Enabled (in %s secs)"),float_ctr);
    // sprintf(ctr,PSTR("Enabled (%d secs)\0"),secs_until_next_event);
    // sprintf(ctr,PSTR("Enabled (123 secs)\0"),secs_until_next_event);
    // Serial.println(ctr);
    return buffer;
  }else{
    return WARNING_NOTHANDLED_CTR;
    // return "Paused";
  }
  return 0;
}


void mRGBAnimator::SetPixelColor(uint16_t indexPixel, RgbTypeColor color_internal)
{

  RgbTypeColor color_hardware = color_internal; //to keep white component if available
  switch (settings.pixel_hardware_color_order_id){
    case  PIXEL_HARDWARE_COLOR_ORDER_GRB_ID:  //0 = GRB, default
      color_hardware.G = color_internal.G; 
      color_hardware.R = color_internal.R; 
      color_hardware.B = color_internal.B; 
    break;
    case  PIXEL_HARDWARE_COLOR_ORDER_RGB_ID:  //1 = RGB, common for WS2811
      color_hardware.G = color_internal.R; 
      color_hardware.R = color_internal.G; 
      color_hardware.B = color_internal.B; 
    break;
    case  2: color_hardware.G = color_internal.B; color_hardware.R = color_internal.R; color_hardware.B = color_internal.G; break; //2 = BRG
    case  3: color_hardware.G = color_internal.R; color_hardware.R = color_internal.B; color_hardware.B = color_internal.G; break; //3 = RBG
    case  4: color_hardware.G = color_internal.B; color_hardware.R = color_internal.G; color_hardware.B = color_internal.R; break; //4 = BGR
    default: color_hardware.G = color_internal.G; color_hardware.R = color_internal.B; color_hardware.B = color_internal.R; break; //5 = GBR
  }

  //AddLog_P(LOG_LEVEL_DEBUG,PSTR("pixel[%d] = %d,%d,%d"),indexPixel,color_hardware.R,color_hardware.G,color_hardware.B);//,color_hardware.W);

  stripbus->SetPixelColor(indexPixel,color_hardware);

}

RgbTypeColor mRGBAnimator::GetPixelColor(uint16_t indexPixel)
{
  DEBUG_LINE;
  if(stripbus == nullptr){    
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "stripbus == nullptr"));
    #endif
  }
  RgbTypeColor color_hardware = stripbus->GetPixelColor(indexPixel);
  RgbTypeColor color_internal = color_hardware; // To catch white element if present
  DEBUG_LINE;

  switch (settings.pixel_hardware_color_order_id){
    case  PIXEL_HARDWARE_COLOR_ORDER_GRB_ID:  //0 = GRB, default
      color_internal.G = color_hardware.G; 
      color_internal.R = color_hardware.R; 
      color_internal.B = color_hardware.B; 
    break;
    case  PIXEL_HARDWARE_COLOR_ORDER_RGB_ID:  //1 = RGB, common for WS2811
      color_internal.R = color_hardware.G; 
      color_internal.G = color_hardware.R; 
      color_internal.B = color_hardware.B; 
    break;
    case  2: color_internal.B = color_hardware.G; color_hardware.R = color_hardware.R; color_internal.G = color_hardware.B; break; //2 = BRG
    case  3: color_internal.R = color_hardware.G; color_internal.B = color_hardware.R; color_internal.G = color_hardware.B; break; //3 = RBG
    case  4: color_internal.B = color_hardware.G; color_internal.G = color_hardware.R; color_internal.R = color_hardware.B; break; //4 = BGR
    default: color_internal.G = color_hardware.G; color_internal.B = color_hardware.R; color_internal.R = color_hardware.B; break; //5 = GBR
  }

  return color_internal;
}








////////////////////// START OF MQTT /////////////////////////

void mRGBAnimator::MQTTHandler_Init(){

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
  mqtthandler_ptr->ConstructJSON_function = &mRGBAnimator::ConstructJSON_Settings;
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
  mqtthandler_ptr->ConstructJSON_function = &mRGBAnimator::ConstructJSON_Animation;

  mqtthandler_ptr = &mqtthandler_ambilight_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_AMBILIGHT_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_AMBILIGHT_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mRGBAnimator::ConstructJSON_Ambilight;
  
  #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS      //make this optional, as it uses extra data and is only for special cases
  mqtthandler_ptr = &mqtthandler_notifications_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_NOTIFICATION_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_notifications;
  mqtthandler_ptr->ConstructJSON_function = &mRGBAnimator::ConstructJSON_Notifications;
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
  mqtthandler_ptr->ConstructJSON_function = &mRGBAnimator::ConstructJSON_State;
  
  #ifdef ENABLE_PIXEL_FUNCTION_FLASHER
  mqtthandler_ptr = &mqtthandler_flasher_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_FLASHER_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_FLASHER_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mRGBAnimator::ConstructJSON_Flasher;
  #endif

#ifdef ENABLE_PIXEL_FUNCTION_MIXER
  mqtthandler_ptr = &mqtthandler_mixer_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_MIXER_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_mixer;
  mqtthandler_ptr->ConstructJSON_function = &mRGBAnimator::ConstructJSON_Mixer;
  #endif
  
  // mqtt_handler_ids
  //  = {
  //   MQTT_HANDLER_SETTINGS_ID, MQTT_HANDLER_MODULE_ANIMATION_TELEPERIOD_ID, MQTT_HANDLER_MODULE_AMBILIGHT_TELEPERIOD_ID,
  //   MQTT_HANDLER_MODULE_STATE_TELEPERIOD_ID
  //   #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS 
  //     MQTT_HANDLER_MODULE_NOTIFICATION_TELEPERIOD_ID,
  //   #endif
  //   #ifdef ENABLE_PIXEL_FUNCTION_FLASHER
  //     MQTT_HANDLER_MODULE_FLASHER_TELEPERIOD_ID, MQTT_HANDLER_MODULE_MIXER_TELEPERIOD_ID
  //   #endif
  // };
  
  // struct handler<mRGBAnimator>* mqtthandler_list_ptr[];
  // 4] = {
  //   &mqtthandler_settings_teleperiod, &mqtthandler_animation_teleperiod, &mqtthandler_ambilight_teleperiod,
  //   &mqtthandler_state_teleperiod,
  //   #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS 
  //     &mqtthandler_notifications_teleperiod,
  //   #endif
  //   #ifdef ENABLE_PIXEL_FUNCTION_FLASHER
  //     &mqtthandler_flasher_teleperiod, &mqtthandler_mixer_teleperiod
  //   #endif
  // };



} //end "MQTTHandler_Init"


void mRGBAnimator::MQTTHandler_Set_fSendNow(){

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


void mRGBAnimator::MQTTHandler_Set_TelePeriod(){

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


void mRGBAnimator::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t mqtthandler_list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID, MQTT_HANDLER_MODULE_ANIMATION_TELEPERIOD_ID, MQTT_HANDLER_MODULE_AMBILIGHT_TELEPERIOD_ID,
    MQTT_HANDLER_MODULE_STATE_TELEPERIOD_ID,
    #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS 
      MQTT_HANDLER_MODULE_NOTIFICATION_TELEPERIOD_ID,
    #endif
    #ifdef ENABLE_PIXEL_FUNCTION_FLASHER
      MQTT_HANDLER_MODULE_FLASHER_TELEPERIOD_ID, 
    #endif
    #ifdef ENABLE_PIXEL_FUNCTION_MIXER
      MQTT_HANDLER_MODULE_MIXER_TELEPERIOD_ID
    #endif
  };
  
  struct handler<mRGBAnimator>* mqtthandler_list_ptr[] = {
    &mqtthandler_settings_teleperiod, &mqtthandler_animation_teleperiod, &mqtthandler_ambilight_teleperiod,
    &mqtthandler_state_teleperiod,
    #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS 
      &mqtthandler_notifications_teleperiod,
    #endif
    #ifdef ENABLE_PIXEL_FUNCTION_FLASHER
      &mqtthandler_flasher_teleperiod, 
    #endif
    #ifdef ENABLE_PIXEL_FUNCTION_MIXER
      &mqtthandler_mixer_teleperiod
    #endif
  };

  // AddLog_P(LOG_LEVEL_TEST, PSTR("MQTTHandler_Sender ids = %d"),sizeof(mqtthandler_list_ids));
  // AddLog_P(LOG_LEVEL_TEST, PSTR("MQTTHandler_Sender ptr = %d"),sizeof(mqtthandler_list_ptr));

  // Loop through them all
  for(uint8_t id=0;id<sizeof(mqtthandler_list_ptr)/sizeof(mqtthandler_list_ptr[0]);id++){    
    if(mqtthandler_list_ptr[id]->handler_id == mqtthandler_list_ids[id]){ 
      pCONT_mqtt->MQTTHandler_Command(*this,D_MODULE_LIGHTS_ADDRESSABLE_ID,mqtthandler_list_ptr[id]);
      if(mqtt_handler_id == MQTT_HANDLER_ALL_ID){ break; } //stop if it was to only send this one
    }
  }

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

void mRGBAnimator::ApplyBrightnesstoDesiredColour(uint8_t brightness){

  for(uint16_t pixel = 0; pixel < strip_size; pixel++ ){
    animation_colours[pixel].DesiredColour.R = mSupport::changeUIntScale(animation_colours[pixel].DesiredColour.R, 0, 255, 0, brightness);
    animation_colours[pixel].DesiredColour.G = mSupport::changeUIntScale(animation_colours[pixel].DesiredColour.G, 0, 255, 0, brightness);
    animation_colours[pixel].DesiredColour.B = mSupport::changeUIntScale(animation_colours[pixel].DesiredColour.B, 0, 255, 0, brightness);
  }

}

// CASES: for colour type
// Substate?: for how they are updated, randomly, all
void mRGBAnimator::RefreshLED_Presets(void){ 
  
  //#ifdef ENABLE_LOG_LEVEL_DEBUG
  AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "f::RefreshLED_Presets"));
  //#endif

  // Update pointer of struct
  pCONT_iLight->SetPaletteListPtrFromID(pCONT_iLight->animation.palette_id);

  // things that use indexes
  RefreshLEDOutputStream();

  // Handle brightness levels == if not set, use default
  //for(ledout.index=0;ledout.index<ledout.length;ledout.index++){ 
    //if(!pCONT_iLight->palettelist.ptr->fOverride_animation_brightness){
      ApplyBrightnesstoDesiredColour(pCONT_iLight->getBriRGB());
      // animation_colours[ledout.index].DesiredColour = RgbTypeColor(255);// pCONT_iLight->getBriRGB();
    //}
  //}

} //end function

void mRGBAnimator::RefreshLEDIndexPattern(){

  // Somehow this orders need to become flasher... which I will rename pCONT_iLight->animation

  // Generate lighting pattern
  switch(pCONT_iLight->animation.transition.order_id){
    case TRANSITION_ORDER_INORDER_ID:
      for(ledout.index=0;ledout.index<ledout.length;ledout.index++){ 
        ledout.pattern[ledout.index] = ledout.index; 
      }

        Serial.println("TRANSITION_ORDER_INORDER_ID");
      for(int jj=0;jj<ledout.length;jj++){ Serial.printf("%d, ",ledout.pattern[jj]); } Serial.println();
// AddLog_Array(LOG_LEVEL_TEST,&ledout.pattern[0],10);

    break;
    case TRANSITION_ORDER_RANDOM_ID:
      switch(transition_order_random_type){
        case TRANSITION_ORDER_RANDOM_METHOD_REPEATING:     //  Serial.println("TRANSITION_ORDER_INORDER_ID1");
          for(ledout.index=0;ledout.index<ledout.length;ledout.index++){ 
            ledout.pattern[ledout.index] = random(0,ledout.length); 
          }
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
          
  // AddLog_P(LOG_LEVEL_TEST, PSTR("BEFORE pick_list_remaining=%d"), pick_list_remaining);
          while(pick_list_remaining){


  // AddLog_Array(LOG_LEVEL_TEST, "pick_list_remaining", pick_list,pick_list_remaining);
  // AddLog_P(LOG_LEVEL_TEST, PSTR("pick_list_remaining=%d"), pick_list_remaining);


      //   Serial.println("pick_list_remaining");
      // for(int jj=0;jj<pick_list_remaining;jj++){ Serial.printf("%d, ",pick_list[jj]); } Serial.println();


            pick_list_index = random(0,pick_list_remaining);
            pick_list_val = pick_list[pick_list_index];
           // AddLog_P(LOG_LEVEL_TEST,PSTR("pick_list_remaining=%d [%d:%d]"),pick_list_remaining,pick_list_index,pick_list_val);

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

        }
        break;
      }
    break;
    case TRANSITION_ORDER_FIXED_ID:
      // pattern is contained within colour map
    break;
    case TRANSITION_ORDER_CENTRE_OUT_ID:{
      // uint16_t middle_index = length_local/2;
      //0,1,2,3,4,5,4,3,2,1,0
      // for(ledout.index=0;ledout.index<length_local;ledout.index++){ 
      //   index_pattern[ledout.index] = ledout.index; 
      // }
    }
    break;
    case TRANSITION_ORDER_ROTATE_ID:
// x = new index 0
//       [0,1,2,x,4,5] start
//       [0,1,2] //remember <x
//       [x,4,5] // shift down
//       [x,4,5,0,1,2] end

// ROTATE IS HANDLED BY NEOPIXEBUS FUNCTIONS, USE THEM!!

      uint16_t rotate_amount = 1;
      // Store last value
      uint16_t last_index = ledout.pattern[ledout.length-1];
      Serial.println();
      for(int jj=0;jj<ledout.length;jj++){ Serial.printf("%d, ",ledout.pattern[jj]); } Serial.println();

      // shift all values by 1
      for(ledout.index=0;ledout.index<ledout.length/2-1;ledout.index++){ 
        swap(
          ledout.pattern[ledout.length/2-ledout.index],
          ledout.pattern[ledout.length/2+ledout.index]
        );
      }
      // Move into first

      for(int jj=0;jj<ledout.length;jj++){ Serial.printf("%d, ",ledout.pattern[jj]); } Serial.println();

      // Use existing pattern and rotate it
      // for(ledout.index=0;ledout.index<length_local;ledout.index++){ 
      //   index_pattern[ledout.index] = ledout.index; 
      // }
    break;
  }

} //end function "RefreshLEDIndexPattern"


// Using ledout.pattern, fill colours
void mRGBAnimator::RefreshLEDOutputStream(void){

  // AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "RefreshLED_Presets pCONT_iLight->animation.palette_id  \"%s\""),GetPaletteFriendlyName());
  AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "RefreshLED_Presets fMapIDs_Type  %d"),pCONT_iLight->palettelist.ptr->flags.fMapIDs_Type);

  // Indexed types 
  //TRANSITION_ORDER_ROTATE_ID, just rotate them, types th
  switch(pCONT_iLight->palettelist.ptr->flags.fMapIDs_Type){
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_ID:
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_ID:
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_AND_SETALL_ID:
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_AND_SETALL_ID:{

      // Get active pixels in map
      uint16_t active_pixels_in_map = pCONT_iLight->GetPixelsInMap(pCONT_iLight->palettelist.ptr); //width 2

      // Move across map
      int16_t pixel_position = -2;
      for(uint16_t desired_pixel=0;desired_pixel<active_pixels_in_map;desired_pixel++){
        RgbTypeColor colour = RgbTypeColor(pCONT_iLight->GetColourFromPalette(pCONT_iLight->palettelist.ptr,desired_pixel,&pixel_position));
        
        #ifdef ENABLE_LOG_LEVEL_DEBUG
        //AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "dp%d, pp%d, %d,%d %d"),desired_pixel, pixel_position,pCONT_iLight->HueF2N(colour.H),pCONT_iLight->SatF2N(colour.S),pCONT_iLight->BrtF2N(colour.B));
        #endif
        
        if(pixel_position>=0){

          // Set output to this "many" colour
          if(pixel_position == 255){
            for(uint16_t temp=0;temp<ledout.length;temp++){ 
              animation_colours[temp].DesiredColour
              
              // desired_colour[temp] 
              = colour; }
            
            #ifdef ENABLE_LOG_LEVEL_DEBUG
            //AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "set ALL %d,%d %d"),pCONT_iLight->HueF2N(colour.H),pCONT_iLight->SatF2N(colour.S),pCONT_iLight->BrtF2N(colour.B));
            #endif

          }else{
            // desired_colour[pixel_position] = colour;
             animation_colours[pixel_position].DesiredColour
              
              // desired_colour[temp] 
              = colour; 
            #ifdef ENABLE_LOG_LEVEL_DEBUG
            //AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "set %d %d,%d %d"), pixel_position,pCONT_iLight->HueF2N(colour.H),pCONT_iLight->SatF2N(colour.S),pCONT_iLight->BrtF2N(colour.B));
            #endif

          }

        }else{
          
          #ifdef ENABLE_LOG_LEVEL_DEBUG
          AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "return; //end early %d"),pixel_position);
          #endif
          //return; //end early
        }
      }
      return;
    }
    break;
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_GRADIENT_ID:
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID:{

// Get active pixels in map

      // Move across map
      // int16_t pixel_position = -2;
      // for(uint16_t desired_pixel=0;desired_pixel<active_pixels_in_map;desired_pixel++){
      //   HsbColor colour = GetColourFromPalette(palettelist.ptr,desired_pixel,&pixel_position);
      //   AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "dp%d, pp%d, %d,%d %d"),desired_pixel, pixel_position,HueF2N(colour.H),SatF2N(colour.S),BrtF2N(colour.B));

      //   if(pixel_position>=0){

      //     // Set output to this "many" colour
      //     if(pixel_position == 255){
      //       for(uint16_t temp=0;temp<ledout.length;temp++){ desired_colour[temp] = colour; }
      //       AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "set ALL %d,%d %d"),HueF2N(colour.H),SatF2N(colour.S),BrtF2N(colour.B));
      //     }else{
      //       desired_colour[pixel_position] = colour;
      //       AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "set %d %d,%d %d"), pixel_position, HueF2N(colour.H),SatF2N(colour.S),BrtF2N(colour.B));
      //     }

      //   }else{
      //     AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "return; //end early %d"),pixel_position);
      //     //return; //end early
      //   }
      // }
      // return;

// AnimUpdateCallback animUpdate = [=](const AnimationParam& param){
//         float progress = easing(param.progress);  // progress will start at 0.0 and end at 1.0 we convert to the curve we want
//         RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(fromcolor, targetColor, progress);  // use the curve value to apply to the pCONT_iLight->animation
//         SetPixelColor(pixel, updatedColor);
//       };


  #ifdef ENABLE_LOG_LEVEL_DEBUG
          AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID"));
          #endif

      uint16_t start_pixel = 0;
      uint16_t end_pixel = 100;
      RgbTypeColor start_colour = RgbTypeColor(0);//preset_colour_map[COLOUR_MAP_RED_ID];
      RgbTypeColor end_colour = RgbTypeColor(0);//preset_colour_map[COLOUR_MAP_GREEN_ID];
      char float_ctr[10];
      uint16_t desired_pixel = 0;
      int16_t start_pixel_position = -1, end_pixel_position = -1;
      uint8_t pixels_in_map = pCONT_iLight->GetPixelsInMap(pCONT_iLight->palettelist.ptr);


  #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "pixels_in_map %d"),pixels_in_map);
      #endif

      for(uint8_t grad_pair_index=0;grad_pair_index<pixels_in_map;grad_pair_index++){
        
        start_colour = pCONT_iLight->GetColourFromPalette(pCONT_iLight->palettelist.ptr,desired_pixel,  &start_pixel_position);
        end_colour   = pCONT_iLight->GetColourFromPalette(pCONT_iLight->palettelist.ptr,desired_pixel+1,&end_pixel_position);

//   #ifdef ENABLE_LOG_LEVEL_DEBUG
//         AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "s%d,%d %d %d"),pCONT_iLight->HueF2N(start_colour.H),pCONT_iLight->SatF2N(start_colour.S),pCONT_iLight->BrtF2N(start_colour.B),start_pixel_position);
//         //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "e%d,%d %d %d"),HueF2N(end_colour.H),SatF2N(end_colour.S),BrtF2N(end_colour.B),end_pixel_position);
// #endif

        switch(pCONT_iLight->palettelist.ptr->flags.fIndexs_Type){
          case INDEX_TYPE_DIRECT: break; //remains the same
          case INDEX_TYPE_SCALED_255: 
            start_pixel_position = map(start_pixel_position,0,255,0,ledout.length);
            end_pixel_position   = map(end_pixel_position,0,255,0,ledout.length);
            // AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "255 %d %d"),start_pixel_position,end_pixel_position);
          break;
          case INDEX_TYPE_SCALED_100: 
            start_pixel_position = map(start_pixel_position,0,100,0,ledout.length);
            end_pixel_position   = map(end_pixel_position,0,100,0,ledout.length);          
          break;
        }

        for(ledout.index=start_pixel_position;ledout.index<end_pixel_position;ledout.index++){
          // float progress = (float)start_pixel/(float)end_pixel;
          float progress = mSupport::mapfloat(ledout.index,start_pixel_position,end_pixel_position,0,1);          
          // memset(float_ctr,0,sizeof(float_ctr));
          // dtostrf(progress,3,1,float_ctr);
          // AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "progress=%s"),float_ctr);
          // RgbColor rgb = RgbTypeColor::LinearBlend(start_colour, end_colour, progress);
          // AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "rgb=%03d,%03d,%03d"),rgb.R,rgb.G,rgb.B);
          #ifndef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
          // desired_colour[ledout.index] = 
          animation_colours[ledout.index].DesiredColour =


          //HsbColor(
            RgbTypeColor::LinearBlend(start_colour, end_colour, progress)
          //)
          ;
          #else

          RgbTypeColor rgb = RgbTypeColor::LinearBlend(start_colour, end_colour, progress);
          desired_colour[ledout.index] = HsbColor(RgbColor(rgb.R,rgb.G,rgb.B));

          // desired_colour[ledout.index] = RgbwColor(0);
          
          //HsbColor(RgbColor(RgbwColor(0)));

          //   RgbTypeColor::LinearBlend(start_colour, end_colour, progress)
          // );
          #endif


        }

        desired_pixel++;

      }

      return; //succcesfully handled

    }break;
    default:
      #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "default"));
      #endif
    break;
  }
  
  // Other types with random led patterns, or,
  desired_pixel=0;
  switch(pCONT_iLight->animation.transition.order_id){
    case TRANSITION_ORDER_INORDER_ID:
    case TRANSITION_ORDER_RANDOM_ID:{
      RefreshLEDIndexPattern();
      int16_t pixel_position = -2;
      for(ledout.index=0;ledout.index<ledout.length;ledout.index++){
        RgbTypeColor colour = pCONT_iLight->GetColourFromPalette(pCONT_iLight->palettelist.ptr,desired_pixel,&pixel_position);

        // desired_colour[ledout.pattern[ledout.index]] = colour;
        
        animation_colours[ledout.pattern[ledout.index]].DesiredColour = colour;
        
        #ifdef ENABLE_LOG_LEVEL_DEBUG
        // AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "colour[p%d:d%d] = %d,%d,%d %d pp%d"),
        //   ledout.pattern[ledout.index],desired_pixel,
        //   pCONT_iLight->HueF2N(desired_colour[ledout.pattern[ledout.index]].H),pCONT_iLight->SatF2N(desired_colour[ledout.pattern[ledout.index]].S),pCONT_iLight->BrtF2N(desired_colour[ledout.pattern[ledout.index]].B),
        //   pCONT_iLight->GetPixelsInMap(pCONT_iLight->palettelist.ptr),pixel_position
        // );
        #endif

        if(++desired_pixel>=pCONT_iLight->GetPixelsInMap(pCONT_iLight->palettelist.ptr)){desired_pixel=0;}
      } //end for
    }break;
    case TRANSITION_ORDER_FIXED_ID:

      int16_t pixel_position = -2;
      //move across all encoded values
        // HsbColor colour = GetColourFromPalette(pCONT_iLight->palettelist.ptr,desired_pixel,&pixel_position);

        // if(pixel_position>=0){
        //   desired_colour[pixel_position] = colour;
        // }else{
        //   desired_colour[ledout.pattern[ledout.index]] = colour;
        // }
        
        // AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "colour[p%d:d%d] = %d,%d,%d %d pp%d"),
        //   ledout.pattern[ledout.index],desired_pixel,
        //   HueF2N(desired_colour[ledout.pattern[ledout.index]].H),SatF2N(desired_colour[ledout.pattern[ledout.index]].S),BrtF2N(desired_colour[ledout.pattern[ledout.index]].B),
        //   pCONT_iLight->palettelist.ptr->active_pixels_in_map,pixel_position
        // );    

    break;
  }//end switch

   AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "DONE RefreshLED_Presets fMapIDs_Type "));


} //end function RefreshLEDOutputStream();



// optional fromcolor to merge "FadeToNewColour" and "FadeBetweenColours"
// If unset (as defined in header) the pCONT_iLight->animation will blend from current pixel colours with getpixel
// void mRGBAnimator::FadeToNewColour(RgbTypeColor targetColor, uint16_t _time_to_newcolour,  RgbTypeColor fromcolor){ 
void mRGBAnimator::FadeToNewColour(RgbcctColor targetColor, uint16_t _time_to_newcolour,  RgbcctColor fromcolor){ 

  #ifdef ENABLE_LOG_LEVEL_DEBUG
    //AddLog_P(LOG_LEVEL_DEBUG_LOWLEVEL, PSTR(D_LOG_NEO "FadeToNewColour"));
  #endif
  
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("RgbcctColor=%d,%d,%d"),targetColor.R,targetColor.G,targetColor.B);
  
  if(NEO_ANIMATION_TIMEBASE == NEO_CENTISECONDS){
    _time_to_newcolour /= 1000;// ms to seconds
  }

  // Overwriting single pCONT_iLight->animation methods, set, then clear
  if(pCONT_iLight->animation_override.time_ms){
    _time_to_newcolour =pCONT_iLight->animation_override.time_ms;
    pCONT_iLight->animation_override.time_ms = 0;//reset overwrite
  }
  
  // AnimEaseFunction easing = NeoEase::CubicIn;  

  // Translation
  #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
    RgbTypeColor fromcolor_npb = RgbwColor(fromcolor.R,fromcolor.G,fromcolor.B,fromcolor.CW);
    RgbTypeColor targetColor_npb = RgbwColor(targetColor.R,targetColor.G,targetColor.B,targetColor.CW);
  #else
    RgbcctColor fromcolor_npb = RgbcctColor(0);
    if(fromcolor != RgbcctColor(0)){
      fromcolor_npb = RgbcctColor(fromcolor.R,fromcolor.G,fromcolor.B);
    }
    RgbcctColor targetColor_npb = RgbcctColor(targetColor.R,targetColor.G,targetColor.B);

    AddLog_P(LOG_LEVEL_DEBUG, PSTR("fromcolor_npb=%d,%d,%d"),fromcolor_npb.R,fromcolor_npb.G,fromcolor_npb.B);
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("targetColor_npb=%d,%d,%d"),targetColor_npb.R,targetColor_npb.G,targetColor_npb.B);

  #endif

  //load start
  for (uint16_t pixel = 0; pixel < strip_size; pixel++){

    if(!(fromcolor_npb.R+fromcolor_npb.G+fromcolor_npb.B)){ // This might cause a bug during fully off to on
      fromcolor_npb = GetPixelColor(pixel);
    }
    animation_colours[pixel].StartingColor = fromcolor_npb;//GetPixelColor(pixel);

    animation_colours[pixel].DesiredColour = targetColor_npb;
  }
  
  #ifdef ENABLE_PIXEL_SINGLE_ANIMATION_CHANNEL
  uint16_t pixel = 0;
  #else
  for (uint16_t pixel = 0; pixel < strip_size; pixel++){
  #endif
    pCONT_iLight->animator_controller->StartAnimation(pixel, _time_to_newcolour, [this](const AnimationParam& param){ this->BlendAnimUpdate(param);} );
  #ifndef ENABLE_PIXEL_SINGLE_ANIMATION_CHANNEL
  }
  #endif


} // END function



// simple blend function
void mRGBAnimator::BlendAnimUpdate(const AnimationParam& param)
{    
  #ifdef ENABLE_PIXEL_SINGLE_ANIMATION_CHANNEL
  for (uint16_t pixel = 0; pixel < strip_size; pixel++){
  #else
  uint16_t pixel = param.index;
  #endif

    RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
        animation_colours[pixel].StartingColor,
        animation_colours[pixel].DesiredColour,
        param.progress);
    SetPixelColor(pixel, updatedColor);
    
  #ifdef ENABLE_PIXEL_SINGLE_ANIMATION_CHANNEL
  } // END for
  #endif
}

void mRGBAnimator::ConfigureLEDTransitionAnimation(){ 
  
  #ifdef ENABLE_LOG_LEVEL_DEBUG
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::ConfigureLEDTransitionAnimation"));
  #endif

  pCONT_iLight->animation_changed_millis = millis();

  uint16_t time_tmp = 0;
  switch(pCONT_iLight->animation.transition.method_id){
    default: 
    case TRANSITION_METHOD_INSTANT_ID: time_tmp = 0; break;
    case TRANSITION_METHOD_BLEND_ID:   time_tmp = pCONT_iLight->animation.transition.time_ms.val; break;
  }

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
  for (uint16_t pixel = 0; pixel < strip_size; pixel++){
    animation_colours[pixel].StartingColor = GetPixelColor(pixel);
    // animation_colours[pixel].DesiredColour = GetPixelColor(pixel);
  }

  // for (uint16_t pixel = 0; pixel < strip_size; pixel++){
  //   animator_controller->StartAnimation(pixel, time_tmp, 
  //     [this](const AnimationParam& param){ this->BlendAnimUpdate(param);}
  //   );
  // }

 
  #ifdef ENABLE_PIXEL_SINGLE_ANIMATION_CHANNEL
  uint16_t pixel = 0;
  #else
  for (uint16_t pixel = 0; pixel < strip_size; pixel++){
  #endif
    pCONT_iLight->animator_controller->StartAnimation(pixel, time_tmp, [this](const AnimationParam& param){ this->BlendAnimUpdate(param);} );
  #ifndef ENABLE_PIXEL_SINGLE_ANIMATION_CHANNEL
  }
  #endif

} //end function







void mRGBAnimator::TurnLEDsOff(){
  //stripbus->ClearTo(0);
  for(uint16_t pixel = 0; pixel < strip_size; pixel++){
    SetPixelColor(pixel, 0);
  }
  stripbus->Show();
}

// void mRGBAnimator::SetRGBwithNeoPixel(){
//   for(uint16_t pixel = 0; pixel < strip_size; pixel++){
//     // SetPixelColor(pixel,RgbTypeColor(desired_colour[pixel]));//.H/360.0f, desired_colour[pixel].S/100.0f, desired_colour[pixel].B/100.0f));
//   }
//   stripbus->Show();
// }

// void mRGBAnimator::SetRGBwithNeoPixelImageTest(){

  
//     // for (auto copies = 0; copies < 3; copies++) {
//     //   image.Blt(stripbus, copies * image.PixelCount());
      
//     // }


//   for(uint16_t pixel = 0; pixel < strip_size; pixel++){
//     // SetPixelColor(pixel,RgbTypeColor(desired_colour[pixel]));//.H/360.0f, desired_colour[pixel].S/100.0f, desired_colour[pixel].B/100.0f));
//   }
//   stripbus->Show();
// }





void mRGBAnimator::Append_Hardware_Status_Message(){

  // struct STATUSHARDWARE{
  //   char ctr[200];
  //   uint8_t len = 0;
  //   uint8_t importance = 0; //0 low, 1 med, 2 high
  // };

  // int numpixels = strip_size;
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


// void mRGBAnimator::AddToJsonObject_AddHardware(JsonObject root){

//   // JsonObject neopixels = root.createNestedObject("neopixels");

//   // #ifdef USE_WS28XX_HARDWARE_WS2801
//   //   neopixels["type"] = "WS2801";
//   // #else
//   //   #ifdef USE_WS2812_DMA
//   //     neopixels["type"] = "WS2812_DMA";
//   //   #else
//   //     neopixels["type"] = "WS2812_PIN_ANY";
//   //   #endif
//   // #endif
//   // #ifdef RGB_DATA_PIN
//   // neopixels["datapin"] = RGB_DATA_PIN;
//   // #endif
//   // neopixels["clockpin"] = 1;//RGB_CLOCK_PIN;
//   // neopixels["size"] = strip_size;

// }


// Single calls that configure Tasker into correct mode
// ie Set Autumn configures preset mode
// void mRGBAnimator::SetMode_UpdateColour(uint8_t colour){
//   pCONT_iLight->animation.palette_id = colour;
//   pCONT_iLight->animation.mode_id = ANIMATION_MODE_PRESETS_ID;
// }

// H ue360toFloat

// make these static and global supports, requiring no object ? inline??




const char* mRGBAnimator::GetHardwareColourTypeName(char* buffer){
  return GetHardwareColourTypeNameByID(pCONT_iLight->animation.mode_id, buffer);
}
const char* mRGBAnimator::GetHardwareColourTypeNameByID(uint8_t id, char* buffer){
  switch(id){
    default:
    case PIXEL_HARDWARE_COLOR_ORDER_GRB_ID: memcpy_P(buffer, PM_PIXEL_HARDWARE_COLOR_ORDER_GRB_CTR, sizeof(PM_PIXEL_HARDWARE_COLOR_ORDER_GRB_CTR)); break;
    case PIXEL_HARDWARE_COLOR_ORDER_RGB_ID: memcpy_P(buffer, PM_PIXEL_HARDWARE_COLOR_ORDER_RGB_CTR, sizeof(PM_PIXEL_HARDWARE_COLOR_ORDER_RGB_CTR)); break;
    case PIXEL_HARDWARE_COLOR_ORDER_BRG_ID: memcpy_P(buffer, PM_PIXEL_HARDWARE_COLOR_ORDER_BRG_CTR, sizeof(PM_PIXEL_HARDWARE_COLOR_ORDER_BRG_CTR)); break;
    case PIXEL_HARDWARE_COLOR_ORDER_RBG_ID: memcpy_P(buffer, PM_PIXEL_HARDWARE_COLOR_ORDER_RBG_CTR, sizeof(PM_PIXEL_HARDWARE_COLOR_ORDER_RBG_CTR)); break;
  }
  return buffer;
}
int8_t mRGBAnimator::GetHardwareColourTypeIDbyName(const char* c){
  if(!c){ return -1; }
  //3 PIXEL TYPE
  if(     strcmp_P(c,PM_PIXEL_HARDWARE_COLOR_ORDER_GRB_CTR)==0){ return PIXEL_HARDWARE_COLOR_ORDER_GRB_ID; }
  else if(strcmp_P(c,PM_PIXEL_HARDWARE_COLOR_ORDER_RGB_CTR)==0){ return PIXEL_HARDWARE_COLOR_ORDER_RGB_ID; }
  else if(strcmp_P(c,PM_PIXEL_HARDWARE_COLOR_ORDER_BRG_CTR)==0){ return PIXEL_HARDWARE_COLOR_ORDER_BRG_ID; }
  else if(strcmp_P(c,PM_PIXEL_HARDWARE_COLOR_ORDER_RBG_CTR)==0){ return PIXEL_HARDWARE_COLOR_ORDER_RBG_ID; }   

  return -1;
}


// Searches for friendly names (e.g. saved user) first, if not, calls normal GetPaletteNameByID 
// const char* mRGBAnimator::GetPaletteNameFriendlyFirstByID(uint8_t id){

//   switch(id){
//     case PALETTELIST_VARIABLE_USER_01_ID : return  D_PALETTE_USER1_NAME_CTR ;
//     case PALETTELIST_VARIABLE_USER_02_ID : return  D_PALETTE_USER2_NAME_CTR ;
//     case PALETTELIST_VARIABLE_USER_03_ID : return  D_PALETTE_USER3_NAME_CTR ;
//     case PALETTELIST_VARIABLE_USER_04_ID : return  D_PALETTE_USER4_NAME_CTR ;
//     case PALETTELIST_VARIABLE_USER_05_ID : return  D_PALETTE_USER5_NAME_CTR ;
//     case PALETTELIST_VARIABLE_USER_06_ID : return  D_PALETTE_USER6_NAME_CTR ;
//     case PALETTELIST_VARIABLE_USER_07_ID : return  D_PALETTE_USER7_NAME_CTR ;
//     case PALETTELIST_VARIABLE_USER_08_ID : return  D_PALETTE_USER8_NAME_CTR ;
//     case PALETTELIST_VARIABLE_USER_09_ID : return  D_PALETTE_USER9_NAME_CTR ;
//     case PALETTELIST_VARIABLE_USER_10_ID : return  D_PALETTE_USER10_NAME_CTR ;
//     case PALETTELIST_VARIABLE_USER_11_ID : return  D_PALETTE_USER11_NAME_CTR ;
//     case PALETTELIST_VARIABLE_USER_12_ID : return  D_PALETTE_USER12_NAME_CTR ;
//     case PALETTELIST_VARIABLE_USER_13_ID : return  D_PALETTE_USER13_NAME_CTR ;
//     case PALETTELIST_VARIABLE_USER_14_ID : return  D_PALETTE_USER14_NAME_CTR ;
//     case PALETTELIST_VARIABLE_USER_15_ID : return  D_PALETTE_USER15_NAME_CTR ;
//     case PALETTELIST_VARIABLE_USER_16_ID : return  D_PALETTE_USER16_NAME_CTR ;
//     case PALETTELIST_VARIABLE_USER_17_ID : return  D_PALETTE_USER17_NAME_CTR ;
//     case PALETTELIST_VARIABLE_USER_18_ID : return  D_PALETTE_USER18_NAME_CTR ;
//     case PALETTELIST_VARIABLE_USER_19_ID : return  D_PALETTE_USER19_NAME_CTR ;
//     case PALETTELIST_VARIABLE_USER_20_ID : return  D_PALETTE_USER20_NAME_CTR ;
//     default: return GetPaletteNameByID(id); //
//   }

// }





// HsbColor mRGBAnimator::GetHsbColour(uint8_t id){


//     // int colour_id = 7*5;

//     uint16_t byte_position = id*12;
//     byte_position = byte_position<sizeof(COLOUR_MAP_BYTE_ARRAY)?byte_position:sizeof(COLOUR_MAP_BYTE_ARRAY);


//     char hsb_bytes[12];

//   // Read from PROGMEM the defaults
//   memcpy_P(hsb_bytes,&COLOUR_MAP_BYTE_ARRAY[byte_position],sizeof(HsbColor));


//       // uint8_t* o
    
//       // uint8_t* my_s_bytes = reinterpret_cast<uint8_t*>(hsb_bytes);
//       // uint8_t* my_s_bytes = reinterpret_cast<uint8_t*>(hsb_bytes);

//       HsbColor* hsb2 = reinterpret_cast<HsbColor*>(hsb_bytes);

//       return *hsb2;

// }




const char* mRGBAnimator::GetTransitionMethodName(char* buffer){
  return GetTransitionMethodNameByID(pCONT_iLight->animation.transition.method_id,buffer);
}
const char* mRGBAnimator::GetTransitionMethodNameByID(uint8_t id, char* buffer){
  switch(id){ default:   
    case TRANSITION_METHOD_NONE_ID: memcpy_P(buffer,PM_TRANSITION_METHOD_NONE_NAME_CTR,sizeof(PM_TRANSITION_METHOD_NONE_NAME_CTR)); break; // smooth shift between them
    case TRANSITION_METHOD_BLEND_ID: memcpy_P(buffer,PM_TRANSITION_METHOD_BLEND_NAME_CTR,sizeof(PM_TRANSITION_METHOD_BLEND_NAME_CTR)); break; 
    case TRANSITION_METHOD_INSTANT_ID: memcpy_P(buffer,PM_TRANSITION_METHOD_INSTANT_NAME_CTR,sizeof(PM_TRANSITION_METHOD_INSTANT_NAME_CTR)); break;  // smooth shift between them
    case TRANSITION_METHOD_TWINKLE_ID: memcpy_P(buffer,PM_TRANSITION_METHOD_TWINKLE_NAME_CTR,sizeof(PM_TRANSITION_METHOD_TWINKLE_NAME_CTR));  break; // smooth shift between them
    case TRANSITION_METHOD_GLIMMER_ID: memcpy_P(buffer,PM_TRANSITION_METHOD_GLIMMER_NAME_CTR,sizeof(PM_TRANSITION_METHOD_GLIMMER_NAME_CTR));  break; // smooth shift between them
  }
  return buffer;
}
int8_t mRGBAnimator::GetTransitionMethodIDbyName(const char* c){
  if(c=='\0'){
    return -1;
  }
  // if(strstr(c,D_TRANSITION_METHOD_BLEND_NAME_CTR)){
  //   return TRANSITION_METHOD_BLEND_ID;
  // }else if(strstr(c,D_TRANSITION_METHOD_INSTANT_NAME_CTR)){
  //   return TRANSITION_METHOD_INSTANT_ID;
  // }else if(strstr(c,D_TRANSITION_METHOD_TWINKLE_NAME_CTR)){
  //   return TRANSITION_METHOD_TWINKLE_ID;
  // }else if(strstr(c,D_TRANSITION_METHOD_GLIMMER_NAME_CTR)){
  //   return TRANSITION_METHOD_GLIMMER_ID;
  // }
  else{
    return -1;
  }
}








const char* mRGBAnimator::GetTransitionOrderName(char* buffer){
  return GetTransitionOrderNameByID(pCONT_iLight->animation.transition.order_id, buffer);
}
const char* mRGBAnimator::GetTransitionOrderNameByID(uint8_t id, char* buffer){
  switch(id){  default:    
    case TRANSITION_ORDER_NONE_ID:       memcpy_P(buffer, PM_TRANSITION_ORDER_NONE_NAME_CTR, sizeof(PM_TRANSITION_ORDER_NONE_NAME_CTR)); break;// smooth shift between them
    case TRANSITION_ORDER_RANDOM_ID:     memcpy_P(buffer, PM_TRANSITION_ORDER_RANDOM_NAME_CTR, sizeof(PM_TRANSITION_ORDER_RANDOM_NAME_CTR)); break;// smooth shift between them
    case TRANSITION_ORDER_INORDER_ID:    memcpy_P(buffer, PM_TRANSITION_ORDER_INORDER_NAME_CTR, sizeof(PM_TRANSITION_ORDER_INORDER_NAME_CTR)); break;// instant shift
    case TRANSITION_ORDER_CENTRE_OUT_ID: memcpy_P(buffer, PM_TRANSITION_ORDER_CENTRE_OUT_NAME_CTR, sizeof(PM_TRANSITION_ORDER_CENTRE_OUT_NAME_CTR)); break;
    case TRANSITION_ORDER_ROTATE_ID:     memcpy_P(buffer, PM_TRANSITION_ORDER_ROTATE_NAME_CTR, sizeof(PM_TRANSITION_ORDER_ROTATE_NAME_CTR)); break;
    case TRANSITION_ORDER_FIXED_ID:     memcpy_P(buffer, PM_TRANSITION_ORDER_FIXED_NAME_CTR, sizeof(PM_TRANSITION_ORDER_FIXED_NAME_CTR)); break;// blend shift with random twinkles on random number of leds
    }
  return buffer;
}
int8_t mRGBAnimator::GetTransitionOrderIDbyName(const char* c){
  if(c=='\0'){ return -1; }

  // strcmp_P

  if(strstr_P(c,PM_TRANSITION_ORDER_RANDOM_NAME_CTR)){
    return TRANSITION_ORDER_RANDOM_ID;
  }else if(strstr_P(c,PM_TRANSITION_ORDER_INORDER_NAME_CTR)){
    return TRANSITION_ORDER_INORDER_ID;
  }

  // if(strstr(c,D_TRANSITION_ORDER_RANDOM_NAME_CTR)){ return TRANSITION_ORDER_RANDOM_ID; }
  // if(strstr(c,D_TRANSITION_ORDER_CENTRE_OUT_NAME_CTR)){ return TRANSITION_ORDER_CENTRE_OUT_ID; }
  // if(strstr(c,D_TRANSITION_ORDER_INORDER_NAME_CTR)){ return TRANSITION_ORDER_INORDER_ID; }
  // if(strstr(c,D_TRANSITION_ORDER_FIXED_NAME_CTR)){ return TRANSITION_ORDER_FIXED_ID; }
  // if(strstr(c,D_TRANSITION_ORDER_ROTATE_NAME_CTR)){ return TRANSITION_ORDER_ROTATE_ID; }
  return -1;
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
  


void mRGBAnimator::SetRefreshLEDs(){

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



void mRGBAnimator::EveryLoop(){

  //something smarter for delays
  // pCONT_set->Settings.enable_sleep = (pCONT_iLight->animation.transition.rate_ms.val<pCONT_set->Settings.sleep) ? 0 : 1;

  if(pCONT_iLight->animation.flags.fEnable_Animation){
  // while(blocking_force_animate_to_complete){
    if (pCONT_iLight->animator_controller->IsAnimating()){ //Serial.print("~a"); // the normal lop just needs these two to run the active animations_fadeall
      pCONT_iLight->animator_controller->UpdateAnimations();
      StripUpdate();
      if(!pCONT_iLight->animation.flags.fRunning){   
        #ifdef ENABLE_LOG_LEVEL_DEBUG
        AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "Animation Started"));
        #endif
      }
      pCONT_iLight->animation.flags.fRunning = true; 
      fPixelsUpdated = true;
      pCONT_set->Settings.enable_sleep = false;    //Make this a function, pause sleep during animations
      //AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "~a"));
    }else{
      if(pCONT_iLight->animation.flags.fRunning){ 
        #ifdef ENABLE_LOG_LEVEL_DEBUG
        AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "Animation Finished")); 
        #endif
        fAnyLEDsOnOffCount = 0;
        for(int i=0;i<strip_size;i++){ 
          if(GetPixelColor(i)!=0){ fAnyLEDsOnOffCount++; }
        }          
      }
      pCONT_iLight->animation.flags.fRunning = false;
      pCONT_set->Settings.enable_sleep = true;
      if(blocking_force_animate_to_complete){ //if it was running
        #ifdef ENABLE_LOG_LEVEL_DEBUG
        AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "Animation blocking_force_animate_to_complete"));
        #endif
        blocking_force_animate_to_complete = false;
      }
    }
  //   if(mSupport::TimeReached(&tSavedBlocking_ForceAnimateToComplete,5000)) {blocking_force_animate_to_complete = false; break; }//exit blocking lo op
  // }
  }//pCONT_iLight->animation.flags.fEnable_Animation



  // #ifndef ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT
  // // scene.colour.B = pCONT_iLight->animation.brightness; // pCONT_iLight->animation.brightness is master
  // // move to have tasmota way to update brightness stored
  // #endif //ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT

  // control relay power
  //(pCONT_iLight->light_power)
    if(pCONT_iLight->light_power!=pCONT_iLight->light_power_Saved){
      pCONT_iLight->light_power_Saved = pCONT_iLight->light_power;
      // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "pCONT_iLight->light_power_Saved"));
      //pCONT->mry->SetRelay(0,pCONT_iLight->light_power);
    }

    //AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "file%sline%d"),__FILE__,__LINE__);
    DEBUG_LINE;

  if(mSupport::TimeReached(&tSavedCheckAnyLEDsOnOff,1000)){
    fAnyLEDsOnOffCount = 0;
    for(int i=0;i<strip_size;i++){
      #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "fAnyLEDsOnOffCount %d"),fAnyLEDsOnOffCount);
      #endif
      if(GetPixelColor(i)!=0){
        fAnyLEDsOnOffCount++;
      }
    }
    if(fAnyLEDsOnOffCount>0){
      #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "fAnyLEDsOnOffCount %d"),fAnyLEDsOnOffCount);
      #endif
    }
    // #ifdef USE_MODULE_DRIVERS_RELAY
    //   //pCONT->mry->SetRelay(0,fAnyLEDsOnOffCount>0?1:0);
    // #endif
  }
  DEBUG_LINE;
// DEBUG_LINE_HERE;

}



//
// changeUIntScale
// Change a value for range a..b to c..d, using only unsigned int math
//
// New version, you don't need the "to_min < to_max" precondition anymore
//
// PRE-CONDITIONS (if not satisfied, you may 'halt and catch fire')
//    from_min < from_max  (not checked)
//    from_min <= num <= from-max  (chacked)
// POST-CONDITIONS
//    to_min <= result <= to_max
//

// uint16_t mRGBAnimator::changeUIntScale(uint16_t inum, uint16_t ifrom_min, uint16_t ifrom_max,
//                                        uint16_t ito_min, uint16_t ito_max) {
//   // guard-rails
//   if (ifrom_min >= ifrom_max) {
//     if (ito_min > ito_max) {
//       return ito_max;
//     } else {
//       return ito_min;  // invalid input, return arbitrary value
//     }
//   }
//   // convert to uint31, it's more verbose but code is more compact
//   uint32_t num = inum;
//   uint32_t from_min = ifrom_min;
//   uint32_t from_max = ifrom_max;
//   uint32_t to_min = ito_min;
//   uint32_t to_max = ito_max;

//   // check source range
//   num = (num > from_max ? from_max : (num < from_min ? from_min : num));

//   // check to_* order
//   if (to_min > to_max) {
//     // reverse order
//     num = (from_max - num) + from_min;
//     to_min = ito_max;
//     to_max = ito_min;
//   }

//   uint32_t numerator = (num - from_min) * (to_max - to_min);
//   uint32_t result;
//   if (numerator >= 0x80000000L) {
//     // don't do rounding as it would create an overflow
//     result = numerator / (from_max - from_min) + to_min;
//   } else {
//     result = (((numerator * 2) / (from_max - from_min)) + 1) / 2 + to_min;
//   }
//   return (uint32_t) (result > to_max ? to_max : (result < to_min ? to_min : result));
// }







  









/*******************************************************************************************************************
********************************************************************************************************************
************MQTT**************************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/


// CheckAndExecute_JSONCommands


int8_t mRGBAnimator::CheckAndExecute_JSONCommands(JsonObjectConst obj){

  // Check if instruction is for me
  if(mSupport::mSearchCtrIndexOf(data_buffer2.topic.ctr,"set/pixels")>=0){
      #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_PIXELS));
      #endif // #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
      pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
      parsesub_TopicCheck_JSONCommand(obj);
      return FUNCTION_RESULT_HANDLED_ID;
  }else{
    return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
  }

}

void mRGBAnimator::parsesub_TopicCheck_JSONCommand(JsonObjectConst obj){
  
  #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
    if(strstr(data_buffer2.topic.ctr,"/notif")){
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "notif"));    
    #endif
      parsesub_NotificationPanel(obj);
    }else 
  #endif
  if(mSupport::memsearch(data_buffer2.topic.ctr,data_buffer2.topic.len,"/manual",sizeof("/manual")-1)>=0){
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "manual"));    
    #endif
    parsesub_ModeManual(obj);
  }else 
  if(mSupport::memsearch(data_buffer2.topic.ctr,data_buffer2.topic.len,"/animation",sizeof("/animation")-1)>=0){
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "animation"));
    #endif    
    parsesub_ModeAnimation(obj);
  }else
  #ifdef USE_PIXEL_ANIMATION_MODE_PIXEL
  if(mSupport::memsearch(data_buffer2.topic.ctr,data_buffer2.topic.len,"/ambilight",sizeof("/ambilight")-1)>=0){
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "ambilight")); 
    #endif   
    parsesub_ModeAmbilight(obj);
  }else
  #endif
  if(mSupport::memsearch(data_buffer2.topic.ctr,data_buffer2.topic.len,"/flasher",sizeof("/flasher")-1)>=0){
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "flasher"));    
    #endif
    parsesub_Flasher(obj);
  }else
  // if(mSupport::memsearch(data_buffer2.topic.ctr,data_buffer2.topic.len,"/hardware",sizeof("/hardware")-1)>=0){
  //   
    // #ifdef ENABLE_LOG_LEVEL_INFO
    // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "flasher"));    
  //   parsesub_Flasher(obj);
  // }else
  {
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "INVALID")); 
    #endif // #ifdef ENABLE_LOG_LEVEL_INFO_PARSING   
  }

  pCONT_iLight->animation.flags.fForceUpdate = true;
  
  if(data_buffer2.isserviced){ //update string, move to shared place
    SetRefreshLEDs(); // implement in 1 second 
  }

  // t_mqtthandler_status_animation.flags.SendNow = true;

  

}

void mRGBAnimator::parsesub_CheckAll(JsonObjectConst obj){
//   Serial.println("DISABLED");
// return 0;

  
    // Serial.println("parsesub_CheckAll1"); Serial.flush();
  parsesub_ModeManual(obj);
    // Serial.println("parsesub_CheckAll2"); Serial.flush();
    
  #ifdef USE_PIXEL_ANIMATION_MODE_PIXEL
  parsesub_ModeAmbilight(obj);
  #endif
    // Serial.println("parsesub_CheckAll3"); Serial.flush();
  parsesub_ModeAnimation(obj);
    // Serial.println("parsesub_CheckAll4"); Serial.flush();
  pCONT_iLight->parsesub_ModeScene(obj);
    // Serial.println("parsesub_CheckAll5"); Serial.flush();
  parsesub_Flasher(obj);
    // Serial.println("parsesub_CheckAll6"); Serial.flush();


  if(data_buffer2.isserviced){ //update string, move to shared place
    SetRefreshLEDs(); // implement in 1 second 
  }
}



// Directly change and control flags/elements without changing everything ie update brightness without changing to scene
void mRGBAnimator::parsesub_ModeManual(JsonObjectConst obj){

  // #ifdef USE_JSON_TO_FLASH_MEMORY_TEST
  if(!obj["external_power_onoff"].isNull()){ 
    const char* onoff = obj[D_JSON_ONOFF];
    if(strstr(onoff,"ON")){ 
      #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED "\"external_power_onoff\"=\"ON\""));
      #endif
      data_buffer2.isserviced++;
    }else 
    if(strstr(onoff,"OFF")){
      #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED "\"external_power_onoff\"=\"OFF\""));
      #endif
      data_buffer2.isserviced++;
    }else{
      AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_NOMATCH));
    }
  }

  DEBUG_LINE;

  //external_power_mode = external_power.mode = manual, auto (turn off when off and pCONT_iLight->animation finished)

  if(!obj[D_JSON_ONOFF].isNull()){ 
    const char* onoff = obj[D_JSON_ONOFF];
    if(strstr(onoff,"ON")){ 
      
      #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED "\"onoff\"=\"ON\""));
      #endif
      
      #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "MODE_TURN_ON_ID"));
      #endif
      // Add this as "SAVE" state then "LOAD" state
      memcpy(&pCONT_iLight->animation,&pCONT_iLight->animation_stored,sizeof(pCONT_iLight->animation));// RESTORE copy of state

      pCONT_iLight->SetAnimationProfile(pCONT_iLight->ANIMATION_PROFILE_TURN_ON_ID);
      pCONT_iLight->light_power = true;

      //pCONT_iLight->animation.mode_id = MODE_TURN_ON_ID;
      data_buffer2.isserviced++;
    }else if(strstr(onoff,"OFF")){
      #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED "\"onoff\"=\"OFF\""));
      #endif
      #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "MODE_TURN_OFF_ID"));
      #endif
      memcpy(&pCONT_iLight->animation_stored,&pCONT_iLight->animation,sizeof(pCONT_iLight->animation)); // STORE copy of state
      pCONT_iLight->SetAnimationProfile(pCONT_iLight->ANIMATION_PROFILE_TURN_OFF_ID);
      pCONT_iLight->light_power = false;

      //pCONT_iLight->animation.mode_id = MODE_TURN_OFF_ID;
      data_buffer2.isserviced++;
    }else{
      #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
      AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_NOMATCH));
      #endif // #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    }
  }


  if(!obj[D_JSON_ANIMATIONENABLE].isNull()){ 
    const char* onoff = obj[D_JSON_ANIMATIONENABLE];
    uint8_t state = pCONT_sup->GetStateNumber(onoff);
    if(state==2){
      pCONT_iLight->animation.flags.fEnable_Animation ^= 1;
    }else{
      pCONT_iLight->animation.flags.fEnable_Animation = state;
    }
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "fEnable_Animation=%d"),pCONT_iLight->animation.flags.fEnable_Animation);    
    #endif //#ifdef ENABLE_LOG_LEVEL_INFO_PARSING
  }

  
  // TIME on duration for autooff
  if(!obj[D_JSON_TIME_ON].isNull()){ //default to secs
    pCONT_iLight->auto_time_off_secs = obj[D_JSON_TIME_ON];
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),pCONT_iLight->auto_time_off_secs);  
    #endif
  }else
  if(!obj[D_JSON_TIME_ON_SECS].isNull()){
    pCONT_iLight->auto_time_off_secs = obj[D_JSON_TIME_ON_SECS];
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),pCONT_iLight->auto_time_off_secs);  
    #endif
  }else
  if(!obj[D_JSON_TIME_ON_MS].isNull()){
    pCONT_iLight->auto_time_off_secs = obj[D_JSON_TIME_ON_MS];
    pCONT_iLight->auto_time_off_secs /= 1000;
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),pCONT_iLight->auto_time_off_secs);  
    #endif
  }

// #endif

  DEBUG_LINE;

  

} // END FUNCTION


void mRGBAnimator::parsesub_ModeAnimation(JsonObjectConst obj){

  char buffer[40];
  int8_t tmp_id = 0;

DEBUG_LINE;

  // #ifdef USE_JSON_TO_FLASH_MEMORY_TEST
DEBUG_LINE;
  if(obj.containsKey(D_JSON_COLOUR_PALETTE)){ 
DEBUG_LINE;


    // if(obj[D_JSON_COLOUR_PALETTE].is<const char*>()){

      const char* colour = obj[D_JSON_COLOUR_PALETTE];
      if((tmp_id=pCONT_iLight->GetPaletteIDbyName(colour))>=0){
        pCONT_iLight->animation.palette_id = tmp_id;

        #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
        AddLog_P(LOG_LEVEL_INFO, PSTR(DEBUG_INSERT_PAGE_BREAK "GetPaletteIDbyName=%d"),tmp_id);
        #endif // #ifdef ENABLE_LOG_LEVEL_INFO_PARSING

  DEBUG_LINE;
        #ifdef ENABLE_PIXEL_FUNCTION_MIXER
        if(pCONT_iLight->animation.mode_id == ANIMATION_MODE_FLASHER_ID){
          flashersettings.region = FLASHER_REGION_COLOUR_SELECT_ID; //update colours in use
        }
        #endif
        #ifdef ENABLE_PALETTE_FORCED_MODE
        //  pCONT_iLight->animation.mode_id = ANIMATION_MODE_PRESETS_ID;
        #endif
        // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_COLOUR_PALETTE,GetPaletteFriendlyName());
        if(pCONT_iLight->animation_override.fRefreshAllPixels){
          first_set = true; //refresh all
        }
        data_buffer2.isserviced++;
      // }
    }else{

    // }
    // if(obj[D_JSON_COLOUR_PALETTE].is<int>()){


      uint8_t colour = obj[D_JSON_COLOUR_PALETTE];
      pCONT_iLight->animation.palette_id = colour < pCONT_iLight->PALETTELIST_STATIC_LENGTH_ID ? colour : 0;

      char buffer[50];

      #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
        AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_NOMATCH D_JSON_COMMAND_NVALUE),D_JSON_COLOUR_PALETTE,pCONT_iLight->animation.palette_id);
        AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_NOMATCH D_JSON_COMMAND_SVALUE),D_JSON_COLOUR_PALETTE,pCONT_iLight->GetPaletteNameByID(pCONT_iLight->animation.palette_id, buffer));
      #endif // #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
      

    }
//     else{
// DEBUG_LINE;
// // #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
// //       AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_NOMATCH D_JSON_COMMAND_SVALUE),D_JSON_COLOUR_PALETTE,colour);
// //     #endif // #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    
//     }
  }

  // //temp method, roll into above when its a number
  // if(obj.containsKey(D_JSON_COLOUR_PALETTE "_Number")){ 


  // }



    //{"colour_selector":{"red":"on"}}
    //{"colour_selector":{"all":"on"}} 
    //{"colour_selector":"reset"} //all off
    //{"colour_selector":"all"} //all off
    //what about colour order? on/off or number for index?
  
      // int randnum = random(0,selectorlist.amount); 
      // desired_colour[ledsindex[ledout.index]].H = selectorlist.colour[randnum].H;
      
  // if(!obj["colour_selector"].isNull()){ 
  //   const char* onoff = obj["colour_selector"];

  // // if(!obj[D_JSON_ONOFF].isNull()){ 
  // //   const char* onoff = obj[D_JSON_ONOFF];
  // //   if(strstr(onoff,"ON")){ 
  // //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED "\"onoff\"=\"ON\""));

  // }
    

  //LEGACY METHOD  
  // if(!obj[D_JSON_MODE].isNull()){ 
  //   const char* mode = obj[D_JSON_MODE];
  //   if((tmp_id=pCONT_iLight->GetAnimationModeIDbyName(mode))>=0){
  //     pCONT_iLight->animation.mode_id = tmp_id;
  //   #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
  //     AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_MODE,pCONT_iLight->GetAnimationModeName(buffer, sizeof(buffer)));
  //    #endif
  //     // Response_mP(S_JSON_COMMAND_SVALUE,D_JSON_MODE,GetAnimationModeName());
  //     data_buffer2.isserviced++;
  //   }else{
  //     #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
  //     AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_MODE,mode);
  //     #endif // #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
  //   }
  // }
  // //NEW METHOD  
  if(!obj[D_JSON_ANIMATIONMODE].isNull()){ 
    const char* mode = obj[D_JSON_ANIMATIONMODE];
    if((tmp_id=pCONT_iLight->GetAnimationModeIDbyName(mode))>=0){
      pCONT_iLight->animation.mode_id = tmp_id;
      #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
      AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_ANIMATIONMODE,pCONT_iLight->GetAnimationModeName(buffer, sizeof(buffer)));
      #endif
      // Response_mP(S_JSON_COMMAND_SVALUE,D_JSON_ANIMATIONMODE,GetAnimationModeName());
      data_buffer2.isserviced++;
    }else{
      #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
      AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_ANIMATIONMODE,mode);
      #endif // #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    }
  }





  if(!obj[D_JSON_RGB_COLOUR_ORDER].isNull()){ 
    const char* mode = obj[D_JSON_RGB_COLOUR_ORDER];
    if((tmp_id=GetHardwareColourTypeIDbyName(mode))>=0){
      settings.pixel_hardware_color_order_id = tmp_id;
      #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
      AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_RGB_COLOUR_ORDER,GetHardwareColourTypeName(buffer));
      #endif
      // Response_mP(S_JSON_COMMAND_SVALUE,D_JSON_MODE,GetAnimationModeName());
      data_buffer2.isserviced++;
    }else{
      #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
      AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_MODE,mode);
      #endif // #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    }
  }


  if(!obj[D_JSON_STRIP_SIZE].isNull()){ 
    int amount = obj[D_JSON_STRIP_SIZE];
    // Also convert to percentage equivalent
    strip_size = amount;
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_STRIP_SIZE,strip_size);
    #endif
    // Response_mP(S_JSON_COMMAND_SVALUE_NVALUE,D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE,pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val);
    data_buffer2.isserviced++;
  }
  
  
DEBUG_LINE;

  // struct TRANSITIONSETTINGS

  if(!obj[D_JSON_TRANSITION][D_JSON_PIXELS_UPDATE_NUMBER].isNull()){ 
    int amount = obj[D_JSON_TRANSITION][D_JSON_PIXELS_UPDATE_NUMBER];
    // Also convert to percentage equivalent
    pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val = GetPixelsToUpdateAsPercentageFromNumber(amount);
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE,pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val);
    #endif
    // Response_mP(S_JSON_COMMAND_SVALUE_NVALUE,D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE,pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val);
    data_buffer2.isserviced++;
  }else
  if(!obj[D_JSON_TRANSITION][D_JSON_PIXELS_UPDATE_PERCENTAGE].isNull()){ 
    int percentage = obj[D_JSON_TRANSITION][D_JSON_PIXELS_UPDATE_PERCENTAGE];
    pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val = constrain(percentage,0,100);
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE,pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val);
    #endif
    Response_mP(S_JSON_COMMAND_SVALUE_NVALUE,D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE,pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val);
    // Also convert to number equivalent
    data_buffer2.isserviced++;
  }

DEBUG_LINE;

  
  // Transition time in seconds or ms
  if(!obj[D_JSON_TRANSITION][D_JSON_TIME].isNull()){ 
    pCONT_iLight->animation.transition.time_ms.val = obj[D_JSON_TRANSITION][D_JSON_TIME];
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_TIME,pCONT_iLight->animation.transition.time_ms.val);   
    #endif
    pCONT_iLight->animation.transition.time_ms.val *= 1000; // map into ms
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_TIME,pCONT_iLight->animation.transition.time_ms.val);
    #endif
    Response_mP(S_JSON_COMMAND_NVALUE, D_JSON_TIME_MS,pCONT_iLight->animation.transition.time_ms.val);
    data_buffer2.isserviced++;
    //check that rate>pCONT_iLight->animation time
    if(pCONT_iLight->animation.transition.rate_ms.val<pCONT_iLight->animation.transition.time_ms.val){ pCONT_iLight->animation.transition.time_ms.val = pCONT_iLight->animation.transition.rate_ms.val;}
  }else
  if(!obj[D_JSON_TRANSITION][D_JSON_TIME_MS].isNull()){ 
    pCONT_iLight->animation.transition.time_ms.val = obj[D_JSON_TRANSITION][D_JSON_TIME_MS];
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_TIME_MS,pCONT_iLight->animation.transition.time_ms.val);
    #endif
    Response_mP(S_JSON_COMMAND_NVALUE, D_JSON_TIME_MS,pCONT_iLight->animation.transition.time_ms.val);
    data_buffer2.isserviced++;
    //check that rate>pCONT_iLight->animation time
    if(pCONT_iLight->animation.transition.rate_ms.val<pCONT_iLight->animation.transition.time_ms.val){ pCONT_iLight->animation.transition.time_ms.val = pCONT_iLight->animation.transition.rate_ms.val;}
  }

DEBUG_LINE;

  // Rate in seconds as default or ms
  if(!obj[D_JSON_TRANSITION][D_JSON_RATE].isNull()){ 
    pCONT_iLight->animation.transition.rate_ms.val = obj[D_JSON_TRANSITION][D_JSON_RATE];
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_RATE,pCONT_iLight->animation.transition.time_ms.val);
    #endif
    pCONT_iLight->animation.transition.rate_ms.val *= 1000; //seconds to milliseconds
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_RATE,pCONT_iLight->animation.transition.time_ms.val);
    Response_mP(S_JSON_COMMAND_NVALUE, D_JSON_TIME_MS,pCONT_iLight->animation.transition.rate_ms.val);
    #endif // #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    data_buffer2.isserviced++;
    //check that rate>pCONT_iLight->animation time
    if(pCONT_iLight->animation.transition.rate_ms.val<pCONT_iLight->animation.transition.time_ms.val){ pCONT_iLight->animation.transition.time_ms.val = pCONT_iLight->animation.transition.rate_ms.val;}
  }else
  if(!obj[D_JSON_TRANSITION][D_JSON_RATE_MS].isNull()){ 
    pCONT_iLight->animation.transition.rate_ms.val = obj[D_JSON_TRANSITION][D_JSON_RATE_MS];
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_RATE_MS,pCONT_iLight->animation.transition.rate_ms.val);
    #endif
    Response_mP(S_JSON_COMMAND_NVALUE, D_JSON_TIME_MS,pCONT_iLight->animation.transition.rate_ms.val);
    data_buffer2.isserviced++;
    //check that rate>pCONT_iLight->animation time
    if(pCONT_iLight->animation.transition.rate_ms.val<pCONT_iLight->animation.transition.time_ms.val){ pCONT_iLight->animation.transition.time_ms.val = pCONT_iLight->animation.transition.rate_ms.val;}
  }

  // probably bring transition_order into one command
  if(!obj[D_JSON_TRANSITION][D_JSON_ORDER].isNull()){ 
    const char* order = obj[D_JSON_TRANSITION][D_JSON_ORDER];
    if((tmp_id=GetTransitionOrderIDbyName(order))>=0){
      pCONT_iLight->animation.transition.order_id = tmp_id;
      // AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_SVALUE),D_JSON_TRANSITION,D_JSON_ORDER,GetTransitionOrderName());
      // Response_mP(S_JSON_COMMAND_SVALUE,D_JSON_ORDER,GetTransitionOrderName());
      data_buffer2.isserviced++;
    }else{
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_NOMATCH D_JSON_COMMAND_SVALUE_SVALUE),D_JSON_TRANSITION,D_JSON_ORDER,order);
    #endif
    }
  }

  if(!obj[D_JSON_BRIGHTNESS].isNull()){
    uint8_t brt = obj[D_JSON_BRIGHTNESS];
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_BRIGHTNESS,brt);
    #endif
    // pCONT_iLight->animation.brightness = pCONT_iLight->BrtN2F(brt);
    pCONT_iLight->setBriRGB(brt);
    // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_BRIGHTNESS,pCONT_iLight->animation.brightness);
    // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_BRIGHTNESS,pCONT_iLight->animation.brightness);
    data_buffer2.isserviced++;
  }
  

  if(!obj[D_JSON_PIXELSGROUPED].isNull()){
    pCONT_iLight->animation.pixelgrouped = obj[D_JSON_PIXELSGROUPED];
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_PIXELSGROUPED,pCONT_iLight->animation.pixelgrouped);
    #endif
  }

  //override commands that run for one pCONT_iLight->animation cycle then are cleared to 0
  if(!obj[D_JSON_REFRESH_ALLPIXELS].isNull()){
    pCONT_iLight->animation_override.fRefreshAllPixels = obj[D_JSON_REFRESH_ALLPIXELS];
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_REFRESH_ALLPIXELS,pCONT_iLight->animation_override.fRefreshAllPixels);
    #endif
  }

  
  
  // TIME on duration for autooff
  if(!obj[D_JSON_TIME_ON].isNull()){ //default to secs
    pCONT_iLight->auto_time_off_secs = obj[D_JSON_TIME_ON];
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),pCONT_iLight->auto_time_off_secs);  
    #endif
  }else
  if(!obj[D_JSON_TIME_ON_SECS].isNull()){
    pCONT_iLight->auto_time_off_secs = obj[D_JSON_TIME_ON_SECS];
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),pCONT_iLight->auto_time_off_secs);  
    #endif
  }else
  if(!obj[D_JSON_TIME_ON_MS].isNull()){
    pCONT_iLight->auto_time_off_secs = obj[D_JSON_TIME_ON_MS];
    pCONT_iLight->auto_time_off_secs /= 1000;
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),pCONT_iLight->auto_time_off_secs);  
    #endif
  }

  //pCONT_iLight->animation_override.time_ms = 1000; // implement in 1 second 


// #endif



DEBUG_LINE;

  

} // END FUNCTION



// Final output, but check for power limit
void mRGBAnimator::StripUpdate(){

  // STRIP_SIZE_REPEAT_MAX

  // if(settings.strip_size_repeat_animation)
  
  // Replicate SetPixel for repeated output
  #ifdef STRIP_REPEAT_OUTPUT_MAX
  int pixels_existing_index = 0;
  for(int i=0;i<STRIP_REPEAT_OUTPUT_MAX;i++){
    SetPixelColor(i,GetPixelColor(pixels_existing_index++));
    if(pixels_existing_index>=STRIP_SIZE_MAX){ pixels_existing_index = 0;}
  }
  #endif // STRIP_REPEAT_OUTPUT_MAX
  
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
  //   // AddLog_P(LOG_LEVEL_TEST,PSTR("%d.%d.%d\t%d\t%d\t%d\t%d"),
  //   //   c.R,c.G,c.B,
  //   //   (int)pixel_units,
  //   //   (int)this_power,
  //   //   (int)(pixel_units * power_rating.Average_mA_Per_Pixel_Step),
  //   //   (int)estimated_power_new_mA
  //   // );
  
  //   // estimated_power_new +=  colour_tmp.W * channel_count * 20;
  // }
  // AddLog_P(LOG_LEVEL_TEST,PSTR("%d.%d.%d\t%d\t%d\t%d\t%d"),
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
    c = desired_colour[ledout.index];
    power += c.R + c.G + c.B;
    // this_power = (pixel_units * power_rating.Average_mA_Per_Pixel_Step;
    // estimated_power_new_mA +=  this_power;
    //last_power = (c.R + c.G + c.B) * power_rating.Average_mA_Per_Pixel_Step;
    // AddLog_P(LOG_LEVEL_TEST,PSTR("%d.%d.%d\t%d\t%d\t%d\t%d"),
    //   c.R,c.G,c.B,
    //   (int)pixel_units,
    //   (int)this_power,
    //   (int)(pixel_units * power_rating.Average_mA_Per_Pixel_Step),
    //   (int)estimated_power_new_mA
    // );
  
    // estimated_power_new +=  colour_tmp.W * channel_count * 20;
  }
  // AddLog_P(LOG_LEVEL_TEST,PSTR("%d.%d.%d\t%d\t%d\t%d\t%d"),
  //     c.R,c.G,c.B,
  //     (int)pixel_units,
  //     (int)this_power,
  //     (int)(pixel_units * power_rating.Average_mA_Per_Pixel_Step),
  //     (int)estimated_power_new_mA
  //   );
  // Store current power usage
  power_rating.current_mA = (float)(power);// * power_rating.Average_mA_Per_Pixel_Step);//estimated_power_new_mA;
  power_rating.power = (power_rating.current_mA/1000)*5;  // /1000*5

  // AddLog_P(LOG_LEVEL_TEST,PSTR("Estimated Power Consumption = %d (mA) p%d"),(int)estimated_power_new_mA,(int)last_power);
  
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR("Estimated Power Consumption = %d (mA)"),(int)estimated_power_new_mA);
    #endif
}
#endif // ENABLE_PIXEL_OUTPUT_POWER_ESTIMATION

  // for (auto copies = 0; copies < 3; copies++) {
  //   image.Blt(stripbus, copies * image.PixelCount());
  // }

  stripbus->Show();

}









// Final hardware output function, map the stored RGB into the correct layout
// Map white types
// void mRGBAnimator::SetPixelColor(uint16_t indexPixel, NeoGrbwFeature color_internal){
//   SetPixelColor(uint16_t indexPixel, RgbTypeColor color_internal)
// }





uint8_t mRGBAnimator::ConstructJSON_Settings(uint8_t json_method){

  memset(&data_buffer2,0,sizeof(data_buffer2));
  pCONT_sup->WriteBuffer_P(data_buffer2.payload.ctr,
    PSTR("{"
      "\"" "ledout"       "\":%d,"
      "\"" "strip_size"        "\":%d,"
      "\"" "STRIP_SIZE_MAX"          "\":%d,"
      "\"" "animator_strip_size"  "\":%d,"
      "\"" "ANIMATOR_SIZE_MAX"         "\":%d"
    "}"),
    ledout.length,
    strip_size,
    STRIP_SIZE_MAX,
    animator_strip_size,
    ANIMATOR_SIZE_MAX
  );

  return strlen(data_buffer2.payload.ctr) ? 1 : 0;

}






uint8_t mRGBAnimator::ConstructJSON_Animation(uint8_t json_method){

  #ifdef ENABLE_LOG_LEVEL_DEBUG
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::ConstructJSON_Animation"));
  #endif

  // StaticJsonDocument<600> doc;
  // JsonObject root = doc.to<JsonObject>();

  // root[D_JSON_ONOFF] = pCONT_iLight->light_power ? "ON" : "OFF";
  // root[D_JSON_MODE] = GetAnimationModeName();
  // root[D_JSON_COLOUR_PALETTE] = GetPaletteFriendlyName();
  // // root[D_JSON_BRIGHTNESS_PERCENTAGE] = pCONT_iLight->animation.brightness*100;
  // root[D_JSON_BRIGHTNESS] = pCONT_iLight->animation.brightness;

  // JsonObject transitionobj = root.createNestedObject(D_JSON_TRANSITION);
  //   transitionobj[D_JSON_METHOD] = GetTransitionMethodName();
  //   // transitionobj[D_JSON_TIME] = mSupport::safeDivideInt(pCONT_iLight->animation.transition.time_ms.val,1000);
  //   transitionobj[D_JSON_TIME_MS] = pCONT_iLight->animation.transition.time_ms.val;
  //   // transitionobj[D_JSON_RATE] = mSupport::safeDivideInt(pCONT_iLight->animation.transition.rate_ms.val,1000);
  //   transitionobj[D_JSON_RATE_MS] = pCONT_iLight->animation.transition.rate_ms.val;
  //   // transitionobj[D_JSON_PIXELS_UPDATE_NUMBER] = GetPixelsToUpdateAsNumberFromPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val);
  //   transitionobj[D_JSON_PIXELS_UPDATE_PERCENTAGE] = pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val;
  //   transitionobj[D_JSON_ORDER] = GetTransitionOrderName();

  // // Flags and states that are used during one transition and reset when completed
  // // JsonObject overridesobj = root.createNestedObject("transition_overrides"); 
  // //   overridesobj["fRefreshAllPixels"] = pCONT_iLight->animation_override.fRefreshAllPixels;
  
  // // root["ledout.length"] = ledout.length;

  // data_buffer2.payload.len = measureJson(root)+1;
  // serializeJson(doc,data_buffer2.payload.ctr);

  return 0;

}



uint8_t mRGBAnimator::ConstructJSON_Ambilight(uint8_t json_level){
  // Awaiting total redesign
  
  #ifdef ENABLE_LOG_LEVEL_DEBUG
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::ConstructJSON_Ambilight"));
  #endif

  
  // DynamicJsonDocument doc(1500);
  // JsonObject root = doc.to<JsonObject>();

  // root["ambilight"] =0;//ledout.length;

  // data_buffer2.payload.len = measureJson(root)+1;
  // serializeJson(doc,data_buffer2.payload.ctr);

  return 0;//data_buffer2.payload.len;

}





uint8_t mRGBAnimator::ConstructJSON_State(uint8_t json_level){

  uint8_t numpixels = strip_size<100?strip_size:100;
  RgbTypeColor c;
  
  JsonBuilderI->Start();  
    JsonBuilderI->Add(D_JSON_SIZE, strip_size);
    JsonBuilderI->Array_Start("rgb");
    for(int i=0;i<numpixels;i++){
      RgbTypeColor c = GetPixelColor(i);
      JsonBuilderI->Add_FP(PSTR("%02X%02X%02X"),c.R,c.G,c.B);
    }
    JsonBuilderI->Array_End();
  return JsonBuilderI->End();

}









/*******************************************************************************************************************
********************************************************************************************************************
************ START OF NOTIFICATION DEFINITIONS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/

#ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS

void mRGBAnimator::init_NotificationPanel(){

  for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){
    notif.flags.fForcePanelUpdate = true; //clear presets
    notif.pixel[i].mode = NOTIF_MODE_OFF_ID;
    notif.pixel[i].colour.H = (i*30)/360.0f;
    notif.pixel[i].colour.S = 1;
    notif.pixel[i].colour.B = 1;
    #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
    notif.pixel[i].colourWhite = 0;
    #endif
  }
  notif.flags.fWhenOnlyWhiteExistsSetBrightnessOfHSBtoZero = true;

} //end "init_NotificationPanel"

//void mRGBAnimator::SubTask_HeatingPanel(){
void mRGBAnimator::SubTask_NotificationPanel(){

  if(mSupport::TimeReached(&notif.tSaved.ForceUpdate,120000)||(notif.flags.fForcePanelUpdate)){
    notif.flags.fForcePanelUpdate = true;
  }

  //Enable a flag to auto turn everything off if inactive for 2 minutes
  if(notif.flags.fEnableTimeoutAll){
    if(mSupport::TimeReached(&notif.tSaved.TimeoutCounter,30000)){  
      #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "Seconds until notif timeout: [%d/%d]"),(millis()-notif.tSaved.TimeoutCounter)/1000,120);
      #endif
    }
    if(mSupport::TimeReached(&notif.tSaved.Timeout,120000)){
        #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "tNotifPanelTimeout"));
      #endif
      TurnLEDsOff();
    }
  }

char buffer[50];

  for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){
    if(
        mSupport::TimeReached(&notif.pixel[i].tSavedUpdate,notif.pixel[i].tRateUpdate)
        ||(notif.flags.fForcePanelUpdate)
      ){ notif.flags.fForcePanelUpdate = false;
      
        #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "Notif mode %d:%s"),i,GetNotificationModeNamebyID(notif.pixel[i].mode, buffer));
        #endif
      
      RgbTypeColor colour = RgbTypeColor(0);

      switch(notif.pixel[i].mode){
        default:
        case NOTIF_MODE_OFF_ID:
        case NOTIF_MODE_STATIC_OFF_ID:
          colour = RgbTypeColor(0);
        break;
        case NOTIF_MODE_STATIC_ON_ID:{
          colour = HsbColor(notif.pixel[i].colour);
          #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
            colour.W = notif.pixel[i].colourWhite;
          #endif
        }break;
        case NOTIF_MODE_FADE_ON_ID: //slow transition to static ON
          if(notif.pixel[i].transition_progess<100){
            notif.pixel[i].transition_progess++;
          }else{
            notif.pixel[i].mode = NOTIF_MODE_STATIC_ON_ID;
          }
          notif.pixel[i].tRateUpdate = (notif.pixel[i].period_ms/100);
          #ifdef ENABLE_LOG_LEVEL_DEBUG
          AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "FADING on progress [%d]"),notif.pixel[i].transition_progess); 
          #endif
          notif.pixel[i].colour.B = BrtN2F(notif.pixel[i].transition_progess);
          colour = HsbColor(notif.pixel[i].colour);
          #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
            colour.W = map(notif.pixel[i].transition_progess,0,100,0,notif.pixel[i].colourWhite);
          #endif
        break;
        case NOTIF_MODE_FADE_OFF_ID:{
          if(notif.pixel[i].transition_progess<100){
            notif.pixel[i].transition_progess++;
          }else{
            notif.pixel[i].mode = NOTIF_MODE_STATIC_OFF_ID;
          }
          notif.pixel[i].tRateUpdate = (notif.pixel[i].period_ms/100);
          #ifdef ENABLE_LOG_LEVEL_DEBUG
          //AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "FADING off progress [%d]"),notif.pixel[i].transition_progess); 
          #endif
          notif.pixel[i].colour.B = (100-notif.pixel[i].transition_progess)/100.0f;
          colour = HsbColor(notif.pixel[i].colour);
          #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
            colour.W = map(notif.pixel[i].transition_progess,0,100,notif.pixel[i].colourWhite,0);
          #endif  
        }break;
        case NOTIF_MODE_BLINKING_OFF_ID:
          colour = RgbTypeColor(0);
          notif.pixel[i].mode = NOTIF_MODE_BLINKING_ON_ID;
          notif.pixel[i].tRateUpdate = (notif.pixel[i].period_ms/2);
        break;
        case NOTIF_MODE_BLINKING_ON_ID:
          colour = HsbColor(notif.pixel[i].colour);
          #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
            colour.W = notif.pixel[i].colourWhite;
          #endif      
          notif.pixel[i].mode = NOTIF_MODE_BLINKING_OFF_ID;
          notif.pixel[i].tRateUpdate = (notif.pixel[i].period_ms/2);
          break;
        case NOTIF_MODE_PULSING_OFF_ID:
          if(notif.pixel[i].transition_progess<100){
            notif.pixel[i].transition_progess++;
          }else{
            notif.pixel[i].mode = NOTIF_MODE_PULSING_ON_ID;
          }
          notif.pixel[i].tRateUpdate = (notif.pixel[i].period_ms/200);
          #ifdef ENABLE_LOG_LEVEL_DEBUG
          AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "PULSING progress [%d]"),notif.pixel[i].transition_progess); 
          #endif
          notif.pixel[i].colour.B = notif.pixel[i].transition_progess/100.0f;
          colour = HsbColor(notif.pixel[i].colour);
          #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
            colour.W = notif.pixel[i].colourWhite;
          #endif
        break;
        case NOTIF_MODE_PULSING_ON_ID:
          if(notif.pixel[i].transition_progess>0){
            notif.pixel[i].transition_progess--;
          }else{
            notif.pixel[i].mode = NOTIF_MODE_PULSING_OFF_ID;
          }
          notif.pixel[i].tRateUpdate = (notif.pixel[i].period_ms/200);
          #ifdef ENABLE_LOG_LEVEL_DEBUG
          AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "PULSING progress [%d]"),notif.pixel[i].transition_progess); 
          #endif
          notif.pixel[i].colour.B = notif.pixel[i].transition_progess/100.0f;
          colour = HsbColor(notif.pixel[i].colour);
          #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
            colour.W = notif.pixel[i].colourWhite;
          #endif            
        break;
      }
      SetPixelColor(i,colour);    
      notif.flags.fShowPanelUpdate = true;
    } //end if
  } //end timer check

  //Auto turn off
  if(mSupport::TimeReached(&notif.tSaved.AutoOff,1000)){// if 1 second past
    for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ //check all
      #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "Notif tSaved.AutoOff [%d]"),notif.pixel[i].pCONT_iLight->auto_time_off_secs);
      #endif
      if(notif.pixel[i].pCONT_iLight->auto_time_off_secs==1){ //if =1 then turn off and clear to 0
        SetPixelColor(i,0);
        notif.pixel[i].pCONT_iLight->auto_time_off_secs = 0;
        notif.pixel[i].mode = NOTIF_MODE_OFF_ID;
        #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "Notif tSaved.AutoOff to OFF[%d]"),notif.pixel[i].pCONT_iLight->auto_time_off_secs);
        #endif
      }else
      if(notif.pixel[i].pCONT_iLight->auto_time_off_secs>1){ //if =1 then turn off and clear to 0
        notif.pixel[i].pCONT_iLight->auto_time_off_secs--; //decrease
      }
    }// END for
  }

  // Update
  if(notif.flags.fShowPanelUpdate){notif.flags.fShowPanelUpdate=false;
    stripbus->Show();
    notif.tSaved.ForceUpdate = millis(); // so we dont have flasher clashes
  }

} // END FUNCTION

/** Sets to control per pixel for notification purposes. Examples of setting:
 * {"pixel_num":[0],"hue":[0],"brt":[0],  "flasher_ms":[0]  ,"pulser_ms":[1]}
 * {"pixel_num":[0,1],"hue":[0],"brt":[0],"flasher_secs":[0],"pulser_secs":[1]}
 * {"pixel_num":0,"hue":[0],"brt":[0],    "flasher":[0]}
 * {"pixel_num":"all","hue":[0],"brt":[0],"flasher":[0]}
 * 
 * 
 * */
void mRGBAnimator::parsesub_NotificationPanel(JsonObjectConst obj){ 

   // Create local dereferenced variable
  // JsonObject obj = (*_obj); 

  struct PARSED{
    // pixel index being targetted
    struct PIXELNUMS{
      uint8_t val[STRIP_NOTIFICATION_SIZE];
      uint8_t found_idx=0;
    }pixelnum;
    // Setting as Hsb directly (sent as either rgb or hsb but stored as hsb)
    struct COL{
      HsbColor val[STRIP_NOTIFICATION_SIZE];
      uint8_t found_idx = 0;
    }hsb;
    // Hues and brightness with sat 100
    struct HUES{
      uint16_t val[STRIP_NOTIFICATION_SIZE];
      uint8_t found_idx=0;
    }hue;
    struct SATURATION{
      uint8_t val[STRIP_NOTIFICATION_SIZE];
      uint8_t found_idx=0;
    }sat;
    struct BRIGHTNESS{
      uint8_t val[STRIP_NOTIFICATION_SIZE];
      uint8_t found_idx=0;
    }brt;
    #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
    struct RGB_WHITE{
      uint8_t val[STRIP_NOTIFICATION_SIZE];
      uint8_t found_idx=0;
    }white;
    #endif
    // Set blink mode with val being period
    struct FLASHER{
      uint16_t val[STRIP_NOTIFICATION_SIZE];
      uint8_t found_idx=0;
    }flasher;
    // Set pulse mode with val being period
    struct PULSER{
      uint16_t val[STRIP_NOTIFICATION_SIZE];
      uint8_t found_idx=0;
    }pulser;
    // Set fade on/off to gradually change between static modes
    struct FADER{
      uint16_t val[STRIP_NOTIFICATION_SIZE];
      uint8_t found_idx=0;
    }fade;
    // Set timer until led automatically turns off
    struct TIMEON_MS{ 
      uint16_t val[STRIP_NOTIFICATION_SIZE];
      uint8_t found_idx=0;
    }timeon_ms;
  }parsed;

  Serial.printf("parsed struct size = %d", sizeof(parsed));

  memset(&parsed,0,sizeof(parsed)); // clear parsing struct
  
  

  // StaticJsonDocument<300> doc;
  // DeserializationError error = deserializeJson(doc, data_buffer2.payload.ctr);
  // JsonObject obj = doc.as<JsonObject>();

  /**************************************************************************************************
  PIXELNUM: formats "all",1,[1] (string,int,array of ints)
  ***************************************************************************************************/

  // Check pixel num exists (exit if it doesn't)
  if(!obj[D_JSON_PIXELNUM].isNull()){
    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS  " " D_PARSING_MATCHED D_JSON_PIXELNUM));  
    if(obj[D_JSON_PIXELNUM].is<JsonArray>()){   
      JsonArrayConst array = obj[D_JSON_PIXELNUM];
      for(JsonVariantConst v : array) {
        int val = v.as<int>();
        parsed.pixelnum.val[parsed.pixelnum.found_idx++] = val;
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PIXELNUM " JsonArray " " [i%d:v%d]"),parsed.pixelnum.found_idx-1,val);          
      }
      data_buffer2.isserviced++;
    }else
    if(obj[D_JSON_PIXELNUM].is<const char*>()){
      const char* name = obj[D_JSON_PIXELNUM];
      if(strstr(name,"all")){
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PIXELNUM " const char* " "all"));     
        for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ parsed.pixelnum.val[parsed.pixelnum.found_idx++] = i; }
      }
      data_buffer2.isserviced++;
    }else 
    if(obj[D_JSON_PIXELNUM].is<int>()){
      int val = obj[D_JSON_PIXELNUM];
      parsed.pixelnum.val[parsed.pixelnum.found_idx++] = val;
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PIXELNUM " int" " [i%d:v%d]"),parsed.pixelnum.found_idx-1,val);    
      data_buffer2.isserviced++;
    }else{
      AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PIXELNUM "Not Found"));
    }
  } //end pixelnum
  
  /**************************************************************************************************
  COLOUR: formats "RRGGBB" (string,array of strings)
  ***************************************************************************************************/
//rgbw? either or scenario
  if(!obj[D_JSON_RGB].isNull()){
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_RGB)); 
    #endif
    uint32_t colour32bit;  
    // Arrays
    if(obj[D_JSON_RGB].is<JsonArray>()){ 
      JsonArrayConst array = obj[D_JSON_RGB];
      for(JsonVariantConst v : array) {
        const char* val = v.as<const char*>();
        if(val[0]=='#'){ colour32bit = (long) strtol( &val[1], NULL, 16); }else{ colour32bit = (long) strtol( &val[0], NULL, 16); }
        parsed.hsb.val[parsed.hue.found_idx++] = HsbColor(RgbColor((colour32bit>>16),(colour32bit>>8&0xFF),(colour32bit&0xFF)));
        #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_RGB " JsonArray " " [i%d:v%s]"),parsed.hsb.found_idx-1,val);    
        #endif
      }
      data_buffer2.isserviced++;
    }else
    if(obj[D_JSON_RGB].is<const char*>()){
      const char* val = obj[D_JSON_RGB];
      if(val[0]=='#'){ colour32bit = (long) strtol( &val[1], NULL, 16); }else{ colour32bit = (long) strtol( &val[0], NULL, 16); }
      parsed.hsb.val[parsed.hue.found_idx++] = HsbColor(RgbColor((colour32bit>>16),(colour32bit>>8&0xFF),(colour32bit&0xFF)));
      #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_RGB " JsonArray " " [i%d:v%s]"),parsed.hsb.found_idx-1,val);    
      #endif
      data_buffer2.isserviced++;
    }else{
      #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_RGB "Not Found"));
      #endif
    }
  } //end hue


  /**************************************************************************************************
  HUE: formats "white/green/blue",1,[1] (string,int,array of ints)
  ***************************************************************************************************/

  // Check pixel num exists (exit if it doesn't)
  if(!obj[D_JSON_HUE].isNull()){
    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_HUE));  
    // Arrays
    if(obj[D_JSON_HUE].is<JsonArray>()){   
      JsonArrayConst array = obj[D_JSON_HUE];
      for(JsonVariantConst v : array) {
        int val = v.as<int>();
        parsed.hue.val[parsed.hue.found_idx++] = val;
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_HUE " JsonArray " " [i%d:v%d]"),parsed.hue.found_idx-1,val);    
      }
      data_buffer2.isserviced++;
    }else
    if(obj[D_JSON_HUE].is<const char*>()){
      const char* colour = obj[D_JSON_HUE];
      // FUTURE : Add a get hue by colour name function
      // if(strstr(name,"all")){
      //   AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS D_JSON_PIXELNUM " const char* " "all"));     
      //   for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ parsed.pixelnum.val[parsed.pixelnum.found_idx++] = i; }
      // }
      data_buffer2.isserviced++;
    }else 
    if(obj[D_JSON_HUE].is<int>()){
      int val = obj[D_JSON_HUE];
      parsed.hue.val[parsed.hue.found_idx++] = val;
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_HUE " int" " [i%d:v%d]"),parsed.hue.found_idx-1,val);    
      data_buffer2.isserviced++;
    }else{
      #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_HUE "Not Found"));
      #endif
    }
  } //end hue
  

  /**************************************************************************************************
  SAT: formats 1,[1] (int,array of ints)
  ***************************************************************************************************/

  if(!obj[D_JSON_SAT].isNull()){
    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_SAT));  
    // Arrays
    if(obj[D_JSON_SAT].is<JsonArray>()){   
      JsonArrayConst array = obj[D_JSON_SAT];
      for(JsonVariantConst v : array) {
        int val = v.as<int>();
        parsed.sat.val[parsed.sat.found_idx++] = val;
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_BRT " JsonArray " " [i%d:v%d]"),parsed.sat.found_idx-1,val);    
      }
      data_buffer2.isserviced++;
    }else
    if(obj[D_JSON_BRT].is<const char*>()){
      const char* name = obj[D_JSON_BRT];
      // FUTURE : Add a get brt by name function
      // if(strstr(name,"all")){
      //   AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS D_JSON_PIXELNUM " const char* " "all"));     
      //   for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ parsed.pixelnum.val[parsed.pixelnum.found_idx++] = i; }
      // }
      data_buffer2.isserviced++;
    }else 
    if(obj[D_JSON_SAT].is<int>()){
      int val = obj[D_JSON_SAT];
      parsed.sat.val[parsed.sat.found_idx++] = val;
      #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_SAT " int" " [i%d:v%d]"),parsed.sat.found_idx-1,val);    
      #endif
      data_buffer2.isserviced++;
    }else{
      #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_SAT "Not Found"));
      #endif
    }
  } //end brt
  
  /**************************************************************************************************
  BRT: formats "low/med/high",1,[1] (string,int,array of ints)
  ***************************************************************************************************/

  if(!obj[D_JSON_BRT].isNull()){
    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_BRT));  
    // Arrays
    if(obj[D_JSON_BRT].is<JsonArray>()){   
      JsonArrayConst array = obj[D_JSON_BRT];
      for(JsonVariantConst v : array) {
        int val = v.as<int>();
        parsed.brt.val[parsed.brt.found_idx++] = val;
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_BRT " JsonArray " " [i%d:v%d]"),parsed.brt.found_idx-1,val);    
      }
      data_buffer2.isserviced++;
    }else
    if(obj[D_JSON_BRT].is<const char*>()){
      const char* name = obj[D_JSON_BRT];
      // FUTURE : Add a get brt by name function
      // if(strstr(name,"all")){
      //   AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS D_JSON_PIXELNUM " const char* " "all"));     
      //   for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ parsed.pixelnum.val[parsed.pixelnum.found_idx++] = i; }
      // }
      data_buffer2.isserviced++;
    }else 
    if(obj[D_JSON_BRT].is<int>()){
      int val = obj[D_JSON_BRT];
      parsed.brt.val[parsed.brt.found_idx++] = val;
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_BRT " int" " [i%d:v%d]"),parsed.brt.found_idx-1,val);    
      data_buffer2.isserviced++;
    }else{
      #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_BRT "Not Found"));
      #endif
    }
  } //end brt
  
  #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
  /**************************************************************************************************
  WHITE of rgbW: formats "low/med/high",1,[1] (string,int,array of ints)
  ***************************************************************************************************/

  if(!obj[D_JSON_WHITE].isNull()){
    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_WHITE));  
    // Arrays
    if(obj[D_JSON_WHITE].is<JsonArray>()){   
      JsonArrayConst array = obj[D_JSON_WHITE];
      for(JsonVariantConst v : array) {
        int val = v.as<int>();
        // Serial.println(parsed.white.found_idx);
        parsed.white.val[parsed.white.found_idx++] = val;//map(val,0,100,0,255); //only 0-100 accepted
        // Serial.println(parsed.white.found_idx);
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_WHITE " JsonArray " " [i%d:v%d]"),parsed.white.found_idx-1,val);    
      }
      data_buffer2.isserviced++;
    }else
    if(obj[D_JSON_WHITE].is<const char*>()){
      const char* name = obj[D_JSON_WHITE];
      // FUTURE : Add a get brt by name function
      // if(strstr(name,"all")){
      //   AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS D_JSON_PIXELNUM " const char* " "all"));     
      //   for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ parsed.pixelnum.val[parsed.pixelnum.found_idx++] = i; }
      // }
      data_buffer2.isserviced++;
    }else 
    if(obj[D_JSON_WHITE].is<int>()){
      int val = obj[D_JSON_WHITE];
      parsed.white.val[parsed.white.found_idx++] = val;
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_WHITE " int" " [i%d:v%d]"),parsed.white.found_idx-1,val);    
      data_buffer2.isserviced++;
    }else{
      #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_WHITE "Not Found"));
      #endif
    }
  } //end brt
  #endif
  

  /**************************************************************************************************
  FLASHER: formats "slow/fast",1,[1] (string,int,array of ints)
  ***************************************************************************************************/

  if(!obj[D_JSON_FLASHER].isNull()){
    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_FLASHER));  
    // Arrays
    if(obj[D_JSON_FLASHER].is<JsonArray>()){   
      JsonArrayConst array = obj[D_JSON_FLASHER];
      for(JsonVariantConst v : array) {
        int val = v.as<int>();
        parsed.flasher.val[parsed.flasher.found_idx++] = val;
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_FLASHER " JsonArray " " [i%d:v%d]"),parsed.flasher.found_idx-1,val);    
      }
      data_buffer2.isserviced++;
    }else
    if(obj[D_JSON_FLASHER].is<int>()){
      int val = obj[D_JSON_FLASHER];
      parsed.flasher.val[parsed.flasher.found_idx++] = val;
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_FLASHER " int" " [i%d:v%d]"),parsed.flasher.found_idx-1,val);    
      data_buffer2.isserviced++;
    }else{
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_FLASHER "Not Found"));
      #endif
    }
  } //end flasher
  

  /**************************************************************************************************
  PULSER: formats "slow/fast",1,[1] (string,int,array of ints)
  ***************************************************************************************************/

  // Check pixel num exists (exit if it doesn't)
  if(!obj[D_JSON_PULSER].isNull()){
    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_PULSER));  
    // Arrays
    if(obj[D_JSON_PULSER].is<JsonArray>()){   
      JsonArrayConst array = obj[D_JSON_PULSER];
      for(JsonVariantConst v : array) {
        int val = v.as<int>();
        parsed.pulser.val[parsed.pulser.found_idx++] = val;
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PULSER " JsonArray " " [i%d:v%d]"),parsed.pulser.found_idx-1,val);    
      }
      data_buffer2.isserviced++;
    }else
    if(obj[D_JSON_PULSER].is<int>()){
      int val = obj[D_JSON_PULSER];
      parsed.pulser.val[parsed.pulser.found_idx++] = val;
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PULSER " int" " [i%d:v%d]"),parsed.pulser.found_idx-1,val);    
      data_buffer2.isserviced++;
    }else{
      AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PULSER "Not Found"));
    }
  } //end pulser
  

  /**************************************************************************************************
  FADE: formats "slow/fast",1,[1] (string,int,array of ints)
  ***************************************************************************************************/

  // Check pixel num exists (exit if it doesn't)
  if(!obj[D_JSON_FADE].isNull()){
    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_FADE));  
    // Arrays
    if(obj[D_JSON_FADE].is<JsonArray>()){   
      JsonArrayConst array = obj[D_JSON_FADE];
      for(JsonVariantConst v : array) {
        int val = v.as<int>();
        parsed.fade.val[parsed.fade.found_idx++] = val;
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_FADE " JsonArray " " [i%d:v%d]"),parsed.fade.found_idx-1,val);    
      }
      data_buffer2.isserviced++;
    }else
    if(obj[D_JSON_FADE].is<int>()){
      int val = obj[D_JSON_FADE];
      parsed.fade.val[parsed.fade.found_idx++] = val;
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_FADE " int" " [i%d:v%d]"),parsed.fade.found_idx-1,val);    
      data_buffer2.isserviced++;
    }else{
      AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_FADE "Not Found"));
    }
  } //end fade
  

  /**************************************************************************************************
  TIMEON: "time_secs" or "time_ms" formats 1,[1] (int,array of ints)
  ***************************************************************************************************/

  // Check pixel num exists (exit if it doesn't)
  if(!obj[D_JSON_TIME_SECS].isNull()){
    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_TIME_SECS));  
    // Arrays
    if(obj[D_JSON_TIME_SECS].is<JsonArray>()){   
      JsonArrayConst array = obj[D_JSON_TIME_SECS];
      for(JsonVariantConst v : array) {
        int val = v.as<int>();
        parsed.timeon_ms.val[parsed.timeon_ms.found_idx++] = val*1000; //secs2ms
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_SECS " JsonArray " " [i%d:v%d]"),parsed.timeon_ms.found_idx-1,val);    
      }
      data_buffer2.isserviced++;
    }else
    //no string command
    if(obj[D_JSON_TIME_SECS].is<int>()){
      int val = obj[D_JSON_TIME_SECS];
      parsed.timeon_ms.val[parsed.timeon_ms.found_idx++] = val*1000; //secs2ms
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_SECS " int" " [i%d:v%d]"),parsed.timeon_ms.found_idx-1,val);    
      data_buffer2.isserviced++;
    }else{
      AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_SECS "Not Found"));
    }
  }else
  if(!obj[D_JSON_TIME_MS].isNull()){
    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_TIME_MS));  
    // Arrays
    if(obj[D_JSON_TIME_MS].is<JsonArray>()){   
      JsonArrayConst array = obj[D_JSON_TIME_MS];
      for(JsonVariantConst v : array) {
        int val = v.as<int>();
        parsed.timeon_ms.val[parsed.timeon_ms.found_idx++] = val;
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_MS " JsonArray " " [i%d:v%d]"),parsed.timeon_ms.found_idx-1,val);    
      }
      data_buffer2.isserviced++;
    }else
    //no string command
    if(obj[D_JSON_TIME_MS].is<int>()){
      int val = obj[D_JSON_TIME_MS];
      parsed.timeon_ms.val[parsed.timeon_ms.found_idx++] = val;
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_MS " int" " [i%d:v%d]"),parsed.timeon_ms.found_idx-1,val);    
      data_buffer2.isserviced++;
    }else{
      AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_MS "Not Found"));
    }
  } 
  
  
  /***********************************************************************************************************************************************************
   * ***********************************************************************************************************************************************************
   * Fill commands using parsed structs *************************************************************************************************************************
   * ***********************************************************************************************************************************************************
   * ***********************************************************************************************************************************************************/

  int pixelidx = 0;
  int subidx = 0;

  if(parsed.hsb.found_idx){
    pixelidx = 0; subidx = 0;
    while(pixelidx<parsed.pixelnum.found_idx){  // Step across all pixels
      int pixelnum = parsed.pixelnum.val[pixelidx++];
      notif.pixel[pixelnum].colour = parsed.hsb.val[subidx];
      if(subidx<parsed.hsb.found_idx-1){subidx++;}
    }
  }
  
  if(parsed.hue.found_idx){
    pixelidx = 0; subidx = 0;
    while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
      int pixelnum = parsed.pixelnum.val[pixelidx++];
      notif.pixel[pixelnum].colour.H = HueN2F(parsed.hue.val[subidx]);
      notif.pixel[pixelnum].mode = NOTIF_MODE_STATIC_ON_ID; //default
      if(subidx<parsed.hue.found_idx-1){subidx++;}
    }
  }

  if(parsed.sat.found_idx){
    pixelidx = 0; subidx = 0;
    while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
      int pixelnum = parsed.pixelnum.val[pixelidx++];
      notif.pixel[pixelnum].colour.S = SatN2F(parsed.sat.val[subidx]);
      notif.pixel[pixelnum].mode = NOTIF_MODE_STATIC_ON_ID; //default
      if(subidx<parsed.sat.found_idx-1){subidx++;}
    }
  }

  if(parsed.brt.found_idx){
    pixelidx = 0; subidx = 0;
    while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
      int pixelnum = parsed.pixelnum.val[pixelidx++];
      notif.pixel[pixelnum].colour.B = BrtN2F(parsed.brt.val[subidx]);
      notif.pixel[pixelnum].mode = NOTIF_MODE_STATIC_ON_ID; //default
      if(subidx<parsed.brt.found_idx-1){subidx++;}
    }
  }
  
  #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
  // RGB method - white part
  if(parsed.white.found_idx){
    pixelidx = 0; subidx = 0;
    while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
      int pixelnum = parsed.pixelnum.val[pixelidx++];
      notif.pixel[pixelnum].colourWhite = parsed.white.val[subidx];
      notif.pixel[pixelnum].mode = NOTIF_MODE_STATIC_ON_ID; //default
      if(subidx<parsed.white.found_idx-1){subidx++;}
      if(notif.flags.fWhenOnlyWhiteExistsSetBrightnessOfHSBtoZero){
        if(!parsed.brt.found_idx){ //no brightness defined
          notif.pixel[pixelnum].colour.B = 0;        
        }
      }

    }
  }
  #endif

  if(parsed.flasher.found_idx){
    pixelidx = 0; subidx = 0;
    while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
      int pixelnum = parsed.pixelnum.val[pixelidx++];
      notif.pixel[pixelnum].period_ms = parsed.flasher.val[subidx]; 
      notif.pixel[pixelnum].mode = NOTIF_MODE_BLINKING_ON_ID; 
      // Step across the subset indexes if they exist, else last/only is used for all pixelnum
      if(subidx<parsed.flasher.found_idx-1){subidx++;}
    }
  }

  if(parsed.pulser.found_idx){
    pixelidx = 0; subidx = 0;
    while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
      int pixelnum = parsed.pixelnum.val[pixelidx++];
      notif.pixel[pixelnum].period_ms = parsed.pulser.val[subidx]; 
      notif.pixel[pixelnum].mode = NOTIF_MODE_PULSING_ON_ID; 
      if(subidx<parsed.pulser.found_idx-1){subidx++;}
    }
  }

  char buffer[30];

  if(parsed.fade.found_idx){
    pixelidx = 0; subidx = 0;
    while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
      int pixelnum = parsed.pixelnum.val[pixelidx++];
      notif.pixel[pixelnum].period_ms = parsed.fade.val[subidx];
      notif.pixel[pixelnum].period_ms = constrain(notif.pixel[pixelnum].period_ms,100,65000);
      // If pixel was off, then turn on else turn off
      
      Serial.printf("notif.pixel[%d].mode = %s %d;\n\r",pixelnum,GetNotificationModeNamebyID(notif.pixel[pixelnum].mode, buffer),notif.pixel[pixelnum].period_ms);

      if(
        (notif.pixel[pixelnum].mode==NOTIF_MODE_OFF_ID)||
        (notif.pixel[pixelnum].mode==NOTIF_MODE_STATIC_OFF_ID)||
        (notif.pixel[pixelnum].mode==NOTIF_MODE_PULSING_OFF_ID)||
        (notif.pixel[pixelnum].mode==NOTIF_MODE_FADE_OFF_ID)
        // (notif.pixel[pixelnum].colour.B)
        // ||(notif.pixel[pixelnum].colourWhite)
        ){
          notif.pixel[pixelnum].mode = NOTIF_MODE_FADE_ON_ID; 
          Serial.printf("notif.pixel[%d].mode = NOTIF_MODE_FADE_ON_ID;\n\r",pixelnum);
        }else{
          notif.pixel[pixelnum].mode = NOTIF_MODE_FADE_OFF_ID; 
          Serial.printf("notif.pixel[%d].mode = NOTIF_MODE_FADE_OFF_ID;\n\r",pixelnum);
        }
        notif.pixel[pixelnum].transition_progess = 0; //0 to 100%
      if(subidx<parsed.fade.found_idx-1){subidx++;}
    }
  }

  // If neither flasher or pulser defined, set as static as default
  if((!parsed.flasher.found_idx)&&(!parsed.pulser.found_idx)&&(!parsed.fade.found_idx)){
    pixelidx = 0; subidx = 0;
    while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
      int pixelnum = parsed.pixelnum.val[pixelidx++];
      notif.pixel[pixelnum].period_ms = 1000; // not used, but set to something reasonable 
      notif.pixel[pixelnum].mode = NOTIF_MODE_STATIC_ON_ID; 
    }
  }

  if(parsed.timeon_ms.found_idx){
    pixelidx = 0; subidx = 0;
    while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels

    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS "timeon_ms [%d i%d:v%d]"),pixelidx,parsed.timeon_ms.found_idx,parsed.pixelnum.val[pixelidx]);    
      
      int pixelnum = parsed.pixelnum.val[pixelidx++];
      notif.pixel[pixelnum].pCONT_iLight->auto_time_off_secs = (parsed.timeon_ms.val[subidx]/1000); 
      if(subidx<parsed.timeon_ms.found_idx-1){subidx++;}

      //also reset timer millis so they align and run together
      notif.pixel[pixelnum].tSavedUpdate = millis();
    }
  }else{
    Serial.println("parsed.timeon_ms.found_idx NOT FOUND");
  }

  // for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){
  //   //you diAddLog_mP2(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_NEO_MODE_NOTIFICATIONS "Index [%d] RgbColor %d\t%d\t%d\t flash [%d]"),i,setcolour[i].r,setcolour[i].g,setcolour[i].b,setcolour[i].flash_enabled);
  // }

  notif.tSaved.Timeout = millis(); // resets timeout counter
  notif.tSaved.TimeoutCounter = millis();
  pCONT_iLight->animation.mode_id = ANIMATION_MODE_NOTIFICATIONS_ID;
  notif.flags.fForcePanelUpdate = true;

  


} // END FUNCTION

const char* mRGBAnimator::GetNotificationModeNamebyID(uint8_t id, char* buffer){
  // return 
  //     (id == NOTIF_MODE_OFF_ID          ? D_NOTIF_MODE_OFF_NAME_CTR  :
  //     (id == NOTIF_MODE_STATIC_OFF_ID   ? D_NOTIF_MODE_STATIC_OFF_NAME_CTR :
  //     (id == NOTIF_MODE_STATIC_ON_ID    ? D_NOTIF_MODE_STATIC_ON_NAME_CTR :
  //     (id == NOTIF_MODE_BLINKING_OFF_ID ? D_NOTIF_MODE_BLINKING_OFF_NAME_CTR :
  //     (id == NOTIF_MODE_BLINKING_ON_ID  ? D_NOTIF_MODE_BLINKING_ON_NAME_CTR :
  //     (id == NOTIF_MODE_PULSING_OFF_ID  ? D_NOTIF_MODE_PULSING_OFF_NAME_CTR :
  //     (id == NOTIF_MODE_PULSING_ON_ID   ? D_NOTIF_MODE_PULSING_ON_NAME_CTR :
  //     "NoMatch")))))));
  return WARNING_NOTHANDLED_CTR;
}
int8_t mRGBAnimator::GetNotificationModeIDbyName(const char* c){
  if(c=='\0'){ 
    return -1;
  }
  // if(strstr(c,D_NOTIF_MODE_OFF_NAME_CTR)){
  //   return NOTIF_MODE_OFF_ID;
  // }else 
  // if(strstr(c,D_NOTIF_MODE_STATIC_OFF_NAME_CTR)){
  //   return NOTIF_MODE_STATIC_OFF_ID;
  // }else 
  // if(strstr(c,D_NOTIF_MODE_STATIC_ON_NAME_CTR)){
  //   return NOTIF_MODE_STATIC_ON_ID;
  // }else 
  // if(strstr(c,D_NOTIF_MODE_BLINKING_OFF_NAME_CTR)){
  //   return NOTIF_MODE_BLINKING_OFF_ID;
  // }else 
  // if(strstr(c,D_NOTIF_MODE_BLINKING_ON_NAME_CTR)){
  //   return NOTIF_MODE_BLINKING_ON_ID;
  // }else 
  // if(strstr(c,D_NOTIF_MODE_PULSING_OFF_NAME_CTR)){
  //   return NOTIF_MODE_PULSING_OFF_ID;
  // }else 
  // if(strstr(c,D_NOTIF_MODE_PULSING_ON_NAME_CTR)){
  //   return NOTIF_MODE_PULSING_ON_ID;
  // }
  else{
    return -1;
  }
}

// Update struct that shows overview, always sends
uint8_t mRGBAnimator::ConstructJSON_Notifications(uint8_t json_level){

  #ifdef ENABLE_LOG_LEVEL_DEBUG
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::ConstructJSON_Notifications"));
  #endif

  // DynamicJsonDocument doc(1400);
  // JsonObject root = doc.to<JsonObject>();

  // root["fForcePanelUpdate"] = notif.flags.fForcePanelUpdate;
  // root["fShowPanelUpdate"] = notif.flags.fShowPanelUpdate;
  // root["fEnableTimeoutAll"] = notif.flags.fEnableTimeoutAll;
      
      
      
  // JsonArray mode_arr = root.createNestedArray("pCONT_iLight->auto_time_off_secs");
  // for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){
  //   mode_arr.add(notif.pixel[i].pCONT_iLight->auto_time_off_secs);
  // }


  // // JsonArray mode_arr = root.createNestedArray("mode");
  // // for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){
  // //   mode_arr.add(GetNotificationModeNamebyID(notif.pixel[i].mode));
  // // }
  // // JsonArray period_ms_arr = root.createNestedArray("period_ms");
  // // for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){
  // //   period_ms_arr.add(notif.pixel[i].period_ms);
  // // }
  // // JsonArray transition_progess_arr = root.createNestedArray(PSTR("transition_progess"));
  // // for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){
  // //   transition_progess_arr.add(notif.pixel[i].transition_progess);
  // // }
  // // JsonArray hsb_arr = root.createNestedArray("hsb");
  // // for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ 
  // //   char tmpctr[10];  memset(tmpctr,0,sizeof(tmpctr));
  // //   sprintf(tmpctr,PSTR("%d,%d,%d"),HueF2N(notif.pixel[i].colour.H),
  // //                             SatF2N(notif.pixel[i].colour.S),
  // //                             BrtF2N(notif.pixel[i].colour.B));
  // //   hsb_arr.add(tmpctr);
  // // }
  // //     pixelobj["tSavedUpdate"] = notif.pixel[i].tSavedUpdate;
  // //     pixelobj["tRateUpdate"] = notif.pixel[i].tRateUpdate;

  // data_buffer2.payload.len = measureJson(root)+1;
  // serializeJson(doc,data_buffer2.payload.ctr);
  return 0;

}
#endif

/*******************************************************************************************************************
********************************************************************************************************************
************ END OF NOTIFICATION DEFINITIONS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/




#endif //USE_MODULE_LIGHTS_ADDRESSABLE




