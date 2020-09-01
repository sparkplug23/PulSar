#include "mRGBAnimator.h"

#ifdef USE_MODULE_LIGHTS_ADDRESSABLE

/*******************************************************************************************************************
********************************************************************************************************************
************TASKER**************************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/

//overload fix when only one parameter is called
// int8_t mRGBAnimator::Tasker(uint8_t function, uint8_t param1){  
//   JsonObjectConst dummy; return Tasker(function, dummy);
// }
// template<typename T>
int8_t mRGBAnimator::Tasker(uint8_t function){//}, T param1){  

    // return 0;
  int8_t function_result = 0;

// INIT SECTIONS ARE ALWAYS OUTSIDE MAIN SWITCHCASE

    /************
     * INIT SECTION * 
    *******************/
  if(function == FUNC_PRE_INIT){
    pre_init();
  }else
  if(function == FUNC_INIT){
    init();
  }


  // Check if light is being handled by another function eg ws2812 (long term probably included into this as commands pipe into this)
  if(pCONT_set->light_type != LT_WS2812){ 
    //Serial.println("light_type != LT_WS2812"); Serial.flush();
    
    #ifdef ENABLE_DEBUG_BOOT_DELAYS
    delay(2000);
    #endif
    //return 0; 
  }
  #ifdef DISABLE_TEMPORARY_RGBANIMATOR
    return 0;
  #endif

  // if animations are enabled
  // pCONT_set->Settings.enable_sleep = false;

  // if(!settings.flags.EnableModule){ return 0;}
  
    // PRINT_FLUSHED("FUNC\n\r\n\rJSON\n\r\n\rCOMMAND\n\r\n function");
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
    case FUNC_EVERY_SECOND:
      //EverySecond();
      //Settings_Default();

      // for(int i=0;i<30;i++){
      //   HsbColor hsb = GetHsbColour(i);
      //   AddLog_P(LOG_LEVEL_INFO,PSTR("hsb %d,%d,%d"), HueF2N(hsb.H), SatF2N(hsb.S), BrtF2N(hsb.B));
      // }

    break;
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
      MQTTHandler_Init(); 
    break;
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
  return Tasker_Web(function);

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
  
  // if (pCONT_set->pin[GPIO_RGB_DATA_ID] < 99) {  // not set when 255
  //   //pin_data = pCONT_set->pin[GPIO_RGB_DATA_ID];
  //   // #ifdef ENABLE_LOG_LEVEL_INFO
  //   //   AddLog_P(LOG_LEVEL_INFO,PSTR("DIO D: " "Pin %s = GPIO%02d"),"GPIO_RGB_DATA",pin_data);
  //   // #endif
  // }
  // if (pCONT_set->pin[GPIO_RGB_CLOCK_ID] < 99) {  // not set when 255
  //   // pin_clock = pCONT_set->pin[GPIO_RGB_CLOCK_ID];
  //   // #ifdef ENABLE_LOG_LEVEL_INFO
  //   //   AddLog_P(LOG_LEVEL_INFO,PSTR("DIO C: " "Pin %s = GPIO%02d"),"GPIO_RGB_CLOCK",pin_clock);
  //   // #endif
  // }

  // Allow runtime changes of animation size
  strip_size = STRIP_SIZE_MAX; 
  animator_strip_size = ANIMATOR_SIZE_MAX<=STRIP_SIZE_MAX?ANIMATOR_SIZE_MAX:STRIP_SIZE_MAX; 
  pCONT_set->Settings.flag_animations.clear_on_reboot = false; //flag

}

void mRGBAnimator::init(void){ 
  
  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "f::init"));
  #endif
  
  // if pixel size changes, free and init again
  uint16_t strip_size_tmp = STRIP_REPEAT_OUTPUT_MAX;//strip_size<STRIP_SIZE_MAX?strip_size:STRIP_SIZE_MAX; // Catch values exceeding limit
  #ifdef USE_WS28XX_HARDWARE_WS2801
    stripbus = new NeoPixelBus<DotStarBgrFeature, DotStarMethod>(strip_size_tmp, pin_clock, pCONT_set->pin[GPIO_RGB_DATA_ID]);
  #else
    #ifdef ENABLE_DEBUG_ESP_DECODER
      stripbus = new NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType>(strip_size_tmp, 5);//pCONT_set->pin[GPIO_RGB_DATA_ID]);
    #else
      stripbus = new NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType>(strip_size_tmp, pCONT_set->pin[GPIO_RGB_DATA_ID]);
    #endif
  #endif //USE_WS28XX_HARDWARE_WS2801

  uint16_t animator_strip_size_tmp = animator_strip_size<ANIMATOR_SIZE_MAX?animator_strip_size:ANIMATOR_SIZE_MAX; // Catch values exceeding limit
  animations_control = new NeoPixelAnimator(animator_strip_size_tmp, NEO_ANIMATION_TIMEBASE); // NeoPixel animation management object

  animation.transition.pixels_to_update_as_percentage.val = 100;
  ledout.length = animator_strip_size_tmp; //ledout.length needs split out to allow mutiple repeated anaimations to output string

  // #ifdef PIXEL_HARDWARE_COLOR_ORDER_GRB
  //   settings.pixel_hardware_color_order_id = PIXEL_HARDWARE_COLOR_ORDER_GRB_ID;
  // #else
  //   settings.pixel_hardware_color_order_id = PIXEL_HARDWARE_COLOR_ORDER_RGB_ID;
  // #endif//PIXEL_HARDWARE_COLOR_ORDER_GRB

  // // If defined in header directly then override
  // #ifdef PIXEL_HARDWARE_COLOR_ORDER_CUSTOM_ID
  //   settings.pixel_hardware_color_order_id = PIXEL_HARDWARE_COLOR_ORDER_CUSTOM_ID;
  // #endif //PIXEL_HARDWARE_COLOR_ORDER_CUSTOM_ID

  randomSeed(analogRead(0));
  
  for(ledout.index=0;ledout.index<ledout.length;ledout.index++){ 
    ledout.pattern[ledout.index] = ledout.index; 
  }

  // Clear stored light output
  memset(&hsbcolour,0,sizeof(hsbcolour));
// pCONT_set->Settings.flag_animations.clear_on_reboot = 1;
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




// delay(3000);

  blocking_force_animate_to_complete = true; //animate to completion on boot (for short animations)
  //ON FIRST BOOT -- MOVE ELSEWHERE
  animation_override.fRefreshAllPixels = true;

// Serial.println("1"); Serial.flush();

  GenerateAnimationPixelAmountMaps();

  init_Animations();

  // New sub init that configures subfunctions
  // init_ColourMap();
  
// Serial.println("2"); Serial.flush();

  uint16_t buffer_length = 0;
  char* buffer = pCONT_set->Settings.animation_settings.palette_user_variable_name_list_ctr;

  for(uint8_t i=0;i<20;i++){
   buffer_length+=sprintf(buffer+buffer_length,"%s%02d|\0","User",i);
   }
   
  #ifndef DISABLE_SERIAL_LOGGING
   Serial.println(buffer);
   #endif


// Serial.println("3"); Serial.flush();
  #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
    init_NotificationPanel();
  #endif
  init_Scenes();
  init_Ambilight();
  #ifdef USE_TASK_RGBLIGHTING_FLASHER_AND_MIXER
  init_flasher_settings();
  init_mixer_defaults();
  #endif

  animation.auto_time_off_secs = 0; //disable

  settings.flags.EnableModule = true;

  //create copy for animation stored

  memcpy(&animation_stored,&animation,sizeof(animation_stored));// RESTORE copy of state

} //end "init"


void mRGBAnimator::Settings_Load(){
  
  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog_P(LOG_LEVEL_TEST,PSTR("mRGBAnimator::Settings_Load"));
  #endif
  
  animation.brightness = pCONT_set->Settings.light_settings.light_brightness_as_percentage/100.0f;
  
  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog_P(LOG_LEVEL_TEST,PSTR("LOADED animation.brightness = %d"),pCONT_set->Settings.light_settings.light_brightness_as_percentage);
  #endif

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

  animation.mode_id = pCONT_set->Settings.animation_settings.animation_mode;
  animation.palette_id = pCONT_set->Settings.animation_settings.animation_palette;
  animation.transition.order_id   = pCONT_set->Settings.animation_settings.animation_transition_order;
  animation.transition.method_id  = pCONT_set->Settings.animation_settings.animation_transition_method;
  animation.transition.time_ms.val = pCONT_set->Settings.animation_settings.animation_transition_time_ms;
  animation.transition.rate_ms.val = pCONT_set->Settings.animation_settings.animation_transition_rate_ms;
  animation.transition.pixels_to_update_as_percentage.val = pCONT_set->Settings.animation_settings.animation_transition_pixels_to_update_as_percentage;

}

void mRGBAnimator::Settings_Save(){
        #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog_P(LOG_LEVEL_TEST,PSTR("mRGBAnimator::Settings_Save"));
  #endif
  
  pCONT_set->Settings.light_settings.light_brightness_as_percentage = animation.brightness*100;
        #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog_P(LOG_LEVEL_TEST,PSTR("SAVED animation.brightness = %d"),pCONT_set->Settings.light_settings.light_brightness_as_percentage);
#endif
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

  pCONT_set->Settings.animation_settings.animation_mode = animation.mode_id;
  pCONT_set->Settings.animation_settings.animation_palette = animation.palette_id;
  pCONT_set->Settings.animation_settings.animation_transition_order = animation.transition.order_id;
  pCONT_set->Settings.animation_settings.animation_transition_method = animation.transition.method_id;
  pCONT_set->Settings.animation_settings.animation_transition_time_ms = animation.transition.time_ms.val;
  pCONT_set->Settings.animation_settings.animation_transition_rate_ms = animation.transition.rate_ms.val;
  pCONT_set->Settings.animation_settings.animation_transition_pixels_to_update_as_percentage = animation.transition.pixels_to_update_as_percentage.val;

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
      animation.transition.order_id = DEFAULT_LIGHTING_TRANSITION_ORDER_ID<TRANSITION_ORDER_LENGTH_ID?
                                            DEFAULT_LIGHTING_TRANSITION_ORDER_ID:TRANSITION_ORDER_INORDER_ID;
    #else
      animation.transition.order_id = TRANSITION_ORDER_INORDER_ID;
    #endif
    
    #ifdef DEFAULT_LIGHTING_TRANSITION_TIME_MAP_SECS_ID
      animation.transition.time_ms.map_id = DEFAULT_LIGHTING_TRANSITION_TIME_MAP_SECS_ID<TIME_MAP_SECS_LENGTH_ID?
                                                DEFAULT_LIGHTING_TRANSITION_TIME_MAP_SECS_ID:TIME_MAP_SECS_15_ID;
      animation.transition.time_ms.val = time_map_secs[animation.transition.time_ms.map_id]*1000;
      animation.flags.ftime_use_map = true;
    #else
      animation.transition.time_ms.map_id = TIME_MAP_SECS_15_ID;
      animation.transition.time_ms.val = time_map_secs[animation.transition.time_ms.map_id]*1000;
      animation.flags.ftime_use_map = true;
    #endif

    #ifdef DEFAULT_LIGHTING_TRANSITION_RATE_MAP_SECS_ID
      animation.transition.rate_ms.map_id = DEFAULT_LIGHTING_TRANSITION_RATE_MAP_SECS_ID<RATE_MAP_SECS_LENGTH_ID?
                                                DEFAULT_LIGHTING_TRANSITION_RATE_MAP_SECS_ID:RATE_MAP_SECS_15_ID;
      animation.transition.rate_ms.val = rate_map_secs[animation.transition.rate_ms.map_id]*1000;
      animation.flags.frate_use_map = true;
    #else
      animation.transition.rate_ms.map_id = RATE_MAP_SECS_15_ID;
      animation.transition.rate_ms.val = rate_map_secs[animation.transition.rate_ms.map_id]*1000;
      animation.flags.frate_use_map = true;
    #endif

    // #ifdef DEFAULT_LIGHTING_ANIMATION_PALETTE_ID
    //   animation.palette_id = DEFAULT_LIGHTING_ANIMATION_PALETTE_ID<PALETTELIST_STATIC_LENGTH_ID?
    //                           DEFAULT_LIGHTING_ANIMATION_PALETTE_ID:PALETTELIST_VARIABLE_USER_01_ID;
    // #else
    //   animation.palette_id = PALETTELIST_VARIABLE_USER_01_ID;
    // #endif

    #ifdef DEFAULT_LIGHTING_ANIMATION_MODE_ID
      animation.mode_id = DEFAULT_LIGHTING_ANIMATION_MODE_ID<ANIMATION_MODE_LENGTH_ID?
                            DEFAULT_LIGHTING_ANIMATION_MODE_ID:ANIMATION_MODE_FLASHER_ID;
    #else
      animation.mode_id = ANIMATION_MODE_FLASHER_ID;
    #endif

    #ifdef DEFAULT_LIGHTING_PIXELS_UPDATE_PERCENTAGE_ID
      animation.transition.pixels_to_update_as_percentage.map_id = DEFAULT_LIGHTING_PIXELS_UPDATE_PERCENTAGE_ID<PIXELS_UPDATE_PERCENTAGE_LENGTH_ID?
                            DEFAULT_LIGHTING_PIXELS_UPDATE_PERCENTAGE_ID:PIXELS_UPDATE_PERCENTAGE_20_ID;
    #else
      animation.transition.pixels_to_update_as_percentage.map_id = PIXELS_UPDATE_PERCENTAGE_20_ID;
    #endif

    #ifdef DEFAULT_LIGHTING_BRIGHTNESS_PERCENTAGE
      animation.brightness = (float)DEFAULT_LIGHTING_BRIGHTNESS_PERCENTAGE/100.0f;//mSupport::WithinLimits(0,(float)DEFAULT_LIGHTING_BRIGHTNESS_PERCENTAGE/100.0f,1)
    #else
      // animation.brightness = WithinLimits(0,(float)1/100.0f,1);
      animation.brightness = 1; //default ot 50% normally for power reasons
    #endif

    #ifdef DONT_USE_OLD_PRESETS
    #ifdef DEFAULT_LIGHTING_SCENE_HUE_COLOUR_MAP_ID 
      scene.colour = preset_colour_map[DEFAULT_LIGHTING_SCENE_HUE_COLOUR_MAP_ID];
    #else
      scene.colour = preset_colour_map[COLOUR_MAP_RED_PASTEL80_ID];
    #endif
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
  //   pixels_to_update_as_number_map[val] = ((float)pixels_to_update_as_percentage_map[val]/100.0f)*strip_size;
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



/*******************************************************************************************************************
********************************************************************************************************************
************ START OF SCENES DEFINITIONS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/

void mRGBAnimator::init_Scenes(){
  scene.name_id = scene_stored.name_id = SCENES_COLOURSCENE_ID;
  // scene.colour.H = 1; //chnage to saving in lighting as rgbcct
  // scene.colour.S = 1;
  // scene.colour.B = 1;

  pCONT_iLight->setChannels(0,255,0);
  
    #ifdef DONT_USE_OLD_PRESETS
  scene_preset_dayon.colour       = preset_colour_map[COLOUR_MAP_PINK_ID]; 
  scene_preset_dayoff.colour      = preset_colour_map[COLOUR_MAP_PINK_ID];
  scene_preset_eveningon.colour   = preset_colour_map[COLOUR_MAP_PINK_ID];
  scene_preset_eveningoff.colour  = preset_colour_map[COLOUR_MAP_PINK_ID];
  scene_preset_nighton.colour     = preset_colour_map[COLOUR_MAP_PINK_ID];
  scene_preset_nightoff.colour    = preset_colour_map[COLOUR_MAP_PINK_ID];
  #endif

} //end "init_Scenes"


// This will handle various colour palettes that are not "user lights" in nature (random colours or flashes per pixel)
// Instead, whole colour designs, changes over time (sunrise/wakeup light)
void mRGBAnimator::SubTask_Scenes(){ 
  #ifdef ENABLE_LOG_LEVEL_DEBUG
  AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO D_CLASS_NAME_MRGBANIMATOR "SubTask_Scenes"));
  #endif

  // Check for reseting SCENES
  if(scene_stored.fActive){
    // if(abs(millis()-scene_stored.tStart)>scene_stored.tOnTime){
    //   scene.name_id = scene_stored.name_id; // restore mode
    //   scene_stored.fActive = false;   // finish
    // }
  }

  switch(scene.name_id){

    //  Change these to be saved as 5 user defined profiles that can be fired by time automatically
    case SCENES_DAYON_ID:       FadeToNewColour(HsbColor(scene_preset_dayon.colour),scene_preset_dayon.time_ms);   scene.name_id = SCENES_NOTACTIVE_ID;break;
    case SCENES_DAYOFF_ID:      FadeToNewColour(HsbColor(scene_preset_dayoff.colour),scene_preset_dayoff.time_ms);   scene.name_id = SCENES_NOTACTIVE_ID;break;
    case SCENES_EVENINGON_ID:   FadeToNewColour(HsbColor(scene_preset_eveningon.colour),scene_preset_eveningon.time_ms);   scene.name_id = SCENES_NOTACTIVE_ID;break;
    case SCENES_EVENINGOFF_ID:  FadeToNewColour(HsbColor(scene_preset_eveningoff.colour),scene_preset_eveningoff.time_ms);   scene.name_id = SCENES_NOTACTIVE_ID;break;
    case SCENES_MIDNIGHTON_ID:  FadeToNewColour(HsbColor(scene_preset_nighton.colour),scene_preset_nighton.time_ms);   scene.name_id = SCENES_NOTACTIVE_ID;break;
    case SCENES_MIDNIGHTOFF_ID: FadeToNewColour(HsbColor(scene_preset_nightoff.colour),scene_preset_nightoff.time_ms);   scene.name_id = SCENES_NOTACTIVE_ID;break;
    
    
    case SCENES_COLOURSCENE_ID:{
      //FadeToNewColour(HsbColor(scene.colour.H,scene.colour.S,animation.brightness),animation.transition.time_ms.val);   
      

      // GetColour_HSB();//
      RgbTypeColor rgbcolour = HsbColor(scene.colour.H,scene.colour.S,animation.brightness);
      #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
        rgbcolour.W = scene.colourW;
      #endif
      FadeToNewColour(rgbcolour,animation.transition.time_ms.val);

      scene.name_id = SCENES_NOTACTIVE_ID;
      #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "scene.name_id=SCENES_COLOURSCENE_ID Executing"));
      #endif
    }break;
    case SCENES_FADE_OFF_ID:{
      FadeToNewColour(RgbTypeColor(0),animation.transition.time_ms.val);
      scene.name_id = SCENES_NOTACTIVE_ID;
    }break;
    case SCENES_SUNRISE_SINGLE_ID:
      // SubTask_Scene_Sunrise_Single();
      if(!animations_control->IsAnimating()){   
        // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "!animations_control->IsAnimating scene.parts [%d] animation.transition.time_ms/3 [%d]"),scene.parts,animation.transition.time_ms.val/4);
        switch(scene.parts){
          case STEP1:  FadeToNewColour(HsbColor(HueN2F(240), SatN2F(100), BrtN2F(1)),  1); break; //instant
          case STEP2:  FadeToNewColour(HsbColor(HueN2F(30),  SatN2F(50),  BrtN2F(50)), animation.transition.time_ms.val/2); break;
          case STEP3:  FadeToNewColour(HsbColor(HueN2F(180), SatN2F(60),  BrtN2F(100)),animation.transition.time_ms.val/4); break;
          case STEP4:  FadeToNewColour(HsbColor(HueN2F(180),  SatN2F(70),  BrtN2F(100)),animation.transition.time_ms.val/4); break;
          case STEP5:  FadeToNewColour(HsbColor(HueN2F(180),  SatN2F(80),  BrtN2F(100)),animation.transition.time_ms.val/8); break;
          default: break;
        }
        if(scene.parts<DONE){scene.parts++;};
        if(scene.parts>=DONE){scene.parts=STEP1; 
        //animation.mode_id = ANIMATION_MODE_NONE_ID; 
        scene.name_id = SCENES_NOTACTIVE_ID; break;}// Stop calling parent function
      }
    break;
    case SCENES_FLASHCOLOUR_ID:
      // stripbus->ClearTo(HsbColor(scene_stored.colour)); // instant
      // scene_stored.tOnTime = 200;
      // scene_stored.name_id = scene.name_id; // save mode to return to
      // scene_stored.fActive = true;
      // scene_stored.tStart = millis();
    break;
    case SCENES_NOTACTIVE_ID: break;
  }

  
  //Auto turn off
  //PHASE OUT TO BE SHARED METHOD
  if(mSupport::TimeReached(&scene.tSavedAutoOff,1000)){
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "scene.tSavedAutoOff [%d]"),scene.auto_time_off_secs);
    #endif
    if(scene.auto_time_off_secs==1){ //if =1 then turn off and clear to 0
      scene.name_id = SCENES_FADE_OFF_ID;
      #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "scene.tSavedAutoOff SCENES_FADE_OFF_ID"));
      #endif
    }else
    if(scene.auto_time_off_secs>1){ //if =1 then turn off and clear to 0
      scene.auto_time_off_secs--; //decrease
    }
  }


} // END FUNCTION



const char* mRGBAnimator::GetSceneName(char* buffer){
  // return WARNING_NOTHANDLED_CTR;// PSTR("nomatch\0");

  sprintf(buffer,"%s","New");

  return buffer;

  // return (scene.name_id == SCENES_DAYON_ID ? D_SCENES_DAYON_NAME_CTR :
  //     (scene.name_id == SCENES_DAYOFF_ID ?      D_SCENES_DAYOFF_NAME_CTR :
  //     (scene.name_id == SCENES_EVENINGON_ID ?     D_SCENES_EVENINGON_NAME_CTR :
  //     (scene.name_id == SCENES_EVENINGOFF_ID ?    D_SCENES_EVENINGOFF_NAME_CTR :
  //     (scene.name_id == SCENES_MIDNIGHTON_ID ?    D_SCENES_MIDNIGHTON_NAME_CTR :
  //     (scene.name_id == SCENES_MIDNIGHTOFF_ID ?    D_SCENES_MIDNIGHTOFF_NAME_CTR :
  //     (scene.name_id == SCENES_FLASHCOLOUR_ID ?   D_SCENES_FLASHCOLOUR_NAME_CTR :
  //     (scene.name_id == SCENES_COLOURSCENE_ID ?    D_SCENES_COLOURSCENE_NAME_CTR :
  //     (scene.name_id == SCENES_SUNRISE_SINGLE_ID ?    D_SCENES_SUNRISE_SINGLE_NAME_CTR :
  //     (scene.name_id == SCENES_NOTACTIVE_ID ?    D_SCENES_NOTACTIVE_NAME_CTR :
  //     "NoMatch"))))))))));
}
int8_t mRGBAnimator::GetSceneIDbyName(const char* c){
  if(c=='\0'){
    return -1;
  }

// have a bunch of presets, and 5 user options/naming


  // if(strstr(c,D_SCENES_DAYON_NAME_CTR)){
  //   return SCENES_DAYON_ID;
  // }else if(strstr(c,D_SCENES_DAYOFF_NAME_CTR)){
  //   return SCENES_DAYOFF_ID;
  // }else if(strstr(c,D_SCENES_EVENINGON_NAME_CTR)){
  //   return SCENES_EVENINGON_ID;
  // }else if(strstr(c,D_SCENES_EVENINGOFF_NAME_CTR)){
  //   return SCENES_EVENINGOFF_ID;
  // }else if(strstr(c,D_SCENES_MIDNIGHTON_NAME_CTR)){
  //   return SCENES_MIDNIGHTON_ID;
  // }else if(strstr(c,D_SCENES_MIDNIGHTOFF_NAME_CTR)){
  //   return SCENES_MIDNIGHTOFF_ID;
  // }else if(strstr(c,D_SCENES_FLASHCOLOUR_NAME_CTR)){
  //   return SCENES_FLASHCOLOUR_ID;
  // }else 
  if(strstr_P(c,PM_SCENES_COLOURSCENE_NAME_CTR)){
     return SCENES_COLOURSCENE_ID;
  }
  //else if(strstr(c,D_SCENES_SUNRISE_SINGLE_NAME_CTR)){
  //   return SCENES_SUNRISE_SINGLE_ID;
  // }else if(strstr(c,D_SCENES_NOTACTIVE_NAME_CTR)){
  //   return SCENES_NOTACTIVE_ID;
  // }else{
  //   return -1;
  // }
}



int8_t mRGBAnimator::parsesub_ModeScene(JsonObjectConst obj){
  
   // Create local dereferenced variable
  // JsonObject obj = (*_obj); 

  // DynamicJsonDocument doc(500);
  // DeserializationError error = deserializeJson(doc, data_buffer2.payload.ctr);
  // if(error){
  //   AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_ERROR_JSON_DESERIALIZATION));
  //   Response_mP(S_JSON_COMMAND_SVALUE, D_ERROR,D_ERROR_JSON_DESERIALIZATION);
  //   return 0;
  // }
  // JsonObject obj = doc.as<JsonObject>();

  int8_t tmp_id = 0;
  int8_t isserviced = 0;

  #ifdef USE_JSON_TO_FLASH_MEMORY_TEST

  char buffer[50];

  if(!obj[D_JSON_NAME].isNull()){ 
    const char* scenectr = obj[D_JSON_NAME];
    if((tmp_id=GetSceneIDbyName(scenectr))>=0){
      scene.name_id = tmp_id;
      animation.mode_id = ANIMATION_MODE_SCENE_ID; //#Idea. make optional
      // char buffer[30];
      #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_NAME,GetSceneName(buffer));
      #endif
      // Response_mP(S_JSON_COMMAND_SVALUE,D_JSON_NAME,GetSceneName(buffer));
      isserviced++;
    }else{
      AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_NAME,scenectr);
    }
  }
  
  // Includes special case of hue=361 which sets saturation to 0 for white
  if(!obj[D_JSON_HUE].isNull()){ 
    uint16_t hue = obj[D_JSON_HUE];
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_HUE,hue);
    #endif
    scene.colour.H = HueN2F(hue);
    if(hue==361){scene.colour.S = 0;} // hue of max+1(361) automically sets hsb to be white
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_HUE,scene.colour.H);
    #endif
    Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_HUE,scene.colour.H);
    isserviced++;
  }

  if(!obj[D_JSON_SAT].isNull()){ 
    uint8_t sat = obj[D_JSON_SAT];
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_SAT,sat);
    #endif
    scene.colour.S = SatN2F(sat);
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_SAT,scene.colour.S);
    #endif
    Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_SAT,scene.colour.S);
    isserviced++;
  }

  if(!obj[D_JSON_BRT].isNull()){ 
    uint8_t brt = obj[D_JSON_BRT];
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_BRT,brt);
    #endif
    scene.colour.B = animation.brightness = BrtN2F(brt);
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_BRT,animation.brightness);
    #endif
    Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_BRT,animation.brightness);
    isserviced++;
  }

  #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
  if(!obj[D_JSON_WHITE].isNull()){ 
    uint8_t whitepixel = obj[D_JSON_WHITE];
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_WHITE,whitepixel);
    #endif
    scene.colourW = whitepixel;
    isserviced++;
  }else{
    scene.colourW = 0; // default to off if not demanded
  }
  #endif

  if(!obj[D_JSON_RGB].isNull()){
    const char* rgbpacked = obj[D_JSON_RGB];
    uint32_t colour32bit = 0;
    if(rgbpacked[0]=='#'){ colour32bit = (long) strtol( &rgbpacked[1], NULL, 16);
    }else{ colour32bit = (long) strtol( &rgbpacked[0], NULL, 16); }
    RgbColor rgb;
    rgb.R = colour32bit >> 16; //RGB
    rgb.G = colour32bit >> 8 & 0xFF; //RGB
    rgb.B = colour32bit & 0xFF; //RGB
    scene.colour = HsbColor(RgbColor(rgb.R,rgb.G,rgb.B));
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_RGB ":%s " D_NEOPIXEL_RGB ":%d,%d,%d " D_NEOPIXEL_HSB ":%d,%d,%d"),
      rgbpacked,rgb.R,rgb.G,rgb.B,scene.colour.H,scene.colour.S,scene.colour.B);
    #endif
  }

  if(obj.containsKey(D_JSON_SCENE_COLOUR)){
    // Check HSB format
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO, PSTR("[D_JSON_SCENE_COLOUR]"));
    #endif
    if(!obj[D_JSON_SCENE_COLOUR][D_JSON_HSB].isNull()){
      #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_INFO, PSTR("[D_JSON_SCENE_COLOUR][D_JSON_HSB]"));
      #endif
      JsonArrayConst colourarray = obj[D_JSON_SCENE_COLOUR][D_JSON_HSB];
      uint8_t index = 0;
      for(JsonVariantConst v : colourarray) {
        int val = v.as<int>();
        #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog_P(LOG_LEVEL_INFO, PSTR("[D_JSON_SCENE_COLOUR][D_JSON_HSB]=%d"),val);
        #endif
        switch(index++){
          case 0: scene.colour.H = HueN2F(val);
          case 1: scene.colour.S = SatN2F(val);
          case 2: scene.colour.B = BrtN2F(val);
        }
      }
    }


    // Check RGB format

    // Check known HSB index name

    // if(colour_ctr[0]=='#'){ colour32bit = (long) strtol( &colour_ctr[1], NULL, 16);
    // }else{ colour32bit = (long) strtol( &colour_ctr[0], NULL, 16); }
    // RgbColor rgb;
    // rgb.R = colour32bit >> 16; //RGB
    // rgb.G = colour32bit >> 8 & 0xFF; //RGB
    // rgb.B = colour32bit & 0xFF; //RGB
    // scene.colour = HsbColor(RgbColor(rgb.R,rgb.G,rgb.B));
    // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_RGB ":%s " D_NEOPIXEL_RGB ":%d,%d,%d " D_NEOPIXEL_HSB ":%d,%d,%d"),
    //   colour_ctr,rgb.R,rgb.G,rgb.B,scene.colour.H,scene.colour.S,scene.colour.B);
  }else{
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_INFO, PSTR("NOT [D_JSON_SCENE_COLOUR]"));
    #endif
    }

  // TIME with different units
  if(!obj[D_JSON_TIME].isNull()){ //default to secs
    animation.transition.time_ms.val = obj[D_JSON_TIME];
    animation.transition.time_ms.val *= 1000;
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),animation.transition.time_ms.val);  
    #endif
  }else
  if(!obj[D_JSON_TIME_SECS].isNull()){
    animation.transition.time_ms.val = obj[D_JSON_TIME_SECS];
    animation.transition.time_ms.val *= 1000;
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),animation.transition.time_ms.val);  
    #endif
  }else
  if(!obj[D_JSON_TIME_MS].isNull()){
    animation.transition.time_ms.val = obj[D_JSON_TIME_MS];
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),animation.transition.time_ms.val);  
    #endif
  }


  // TIME on duration for autooff
  if(!obj[D_JSON_TIME_ON].isNull()){ //default to secs
    scene.auto_time_off_secs = obj[D_JSON_TIME_ON];
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),scene.auto_time_off_secs);  
    #endif
  }else
  if(!obj[D_JSON_TIME_ON_SECS].isNull()){
    scene.auto_time_off_secs = obj[D_JSON_TIME_ON_SECS];
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),scene.auto_time_off_secs);  
    #endif
  }else
  if(!obj[D_JSON_TIME_ON_MS].isNull()){
    scene.auto_time_off_secs = obj[D_JSON_TIME_ON_MS];
    scene.auto_time_off_secs /= 1000;
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),scene.auto_time_off_secs);  
    #endif
  }

  #endif
  
// Flash colour needs to NOT be a scene, but part of a manual direct mode
// if(strstr(scenectr,"FLASHCOLOUR")){pCONT->mso->MessagePrintln("FLASHCOLOUR");
//       scene_reseting.name_id = scene.name_id; // remember scene to return to
//       scene.name_id = SCENES_FLASHCOLOUR_ID;
//     }
//     // Parse out flashcolour info if it exists
//     if(scene.name_id == SCENES_FLASHCOLOUR_ID){

//       if(tempctr = obj["hue"]){
//         scene_reseting.hue = obj["hue"];
//         pCONT->mso->MessagePrintln("scene_reseting.hue");
//         pCONT->mso->MessagePrintln(scene_reseting.hue);
//       }
//       if(tempctr = obj["sat"]){
//         scene_reseting.sat = obj["sat"];
//         pCONT->mso->MessagePrintln("scene_reseting.sat");
//         pCONT->mso->MessagePrintln(scene_reseting.sat);
//       }
//       if(tempctr = obj["brt"]){
//         scene_reseting.brt = obj["brt"];
//         pCONT->mso->MessagePrintln("scene_reseting.brt");
//         pCONT->mso->MessagePrintln(scene_reseting.brt);
//       }
//       if(tempctr = obj["time"]){
//         scene_reseting.tOnTime = obj["time"];
//         pCONT->mso->MessagePrintln("scene_reseting.tOnTime");
//         pCONT->mso->MessagePrintln(scene_reseting.tOnTime);
//       }
//     }
  
  return isserviced;

} // END FUNCTION



uint8_t mRGBAnimator::ConstructJSON_Scene(uint8_t json_method){

  #ifdef ENABLE_LOG_LEVEL_DEBUG
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::ConstructJSON_Scene"));
  #endif
  // DynamicJsonDocument doc(200);
  // JsonObject root = doc.to<JsonObject>();

  // root[D_JSON_NAME] = GetSceneName();
  // root[D_JSON_HUE] = round(HueF2N(scene.colour.H));
  // root[D_JSON_SAT] = round(SatF2N(scene.colour.S));
  // root[D_JSON_BRT] = round(BrtF2N(scene.colour.B));
  // root[D_JSON_TIME] = mSupport::safeDivideInt(animation.transition.time_ms.val,1000);
  // root[D_JSON_TIME_MS] = animation.transition.time_ms.val;

  // RgbTypeColor c = HsbColor(scene.colour);
  // char tmpctr[10]; memset(tmpctr,0,sizeof(tmpctr));
  // sprintf(tmpctr,PSTR("%02X%02X%02X"),c.R,c.G,c.B);
  // root[D_JSON_RGB] = tmpctr;

  // data_buffer2.payload.len = measureJson(root)+1;
  // serializeJson(doc,data_buffer2.payload.ctr);

  // return (data_buffer2.payload.len>3?1:0);
  return 0;
}

/*******************************************************************************************************************
********************************************************************************************************************
************ END OF SCENES DEFINITIONS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/


/*******************************************************************************************************************
********************************************************************************************************************
************ START OF AMBILIGHT DEFINITIONS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/


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
    hsbcolour[bottom_start+bottom] = ambilightsettings.screens[SCREEN_CENTRE].bottom.colour;
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
    hsbcolour[left_start+left] = colour_tmp;//ambilightsettings.screens[SCREEN_CENTRE].left.colour;
  }

  uint8_t top_size = ambilightsettings.screens[SCREEN_CENTRE].top.size;
  uint8_t top_start = 52;
  for(int top=0;top<top_size;top++){
    hsbcolour[top_start+top] = ambilightsettings.screens[SCREEN_CENTRE].top.colour;
  }

  uint8_t right_size = ambilightsettings.screens[SCREEN_CENTRE].right.size;
  uint8_t right_start = 85;
  for(int right=0;right<right_size;right++){
    progress = (float)(right)/(float)(right_size);
    colour_tmp = RgbColor::LinearBlend(RgbColor(ambilightsettings.screens[SCREEN_CENTRE].top.colour),
                                       RgbColor(ambilightsettings.screens[SCREEN_CENTRE].bottom.colour),
                                       progress);
    hsbcolour[right_start+right] = colour_tmp;
  }

}

/*******************************************************************************************************************
********************************************************************************************************************
************ END OF AMBILIGHT DEFINITIONS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/



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


        
  flashersettings.random_transitions.array_index_length = 0;
  flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 20;
  flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 40;
  flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 50;
  flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 75;
  flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 100;
  flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 75;
  flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 50;
  flashersettings.random_transitions.array[flashersettings.random_transitions.array_index_length++] = 40;
  
}



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

//       animation.palette_id = mixer.group[mixer.running_id].animation_palette;          
//       animation.transition.order_id = mixer.group[mixer.running_id].animation_transition_order;
//       flashersettings.function = mixer.group[mixer.running_id].flashersettings_function;          
//       mixer.group[mixer.running_id].time_on_secs_decounter = mixer.group[mixer.running_id].run_time_duration_sec*((float)mixer.run_time_duration_scaler_as_percentage/100);
//       //mixer.time_on_secs_active = mixer.group[mixer.running_id].run_time_duration_sec;
//       animation.transition.time_ms = mixer.group[mixer.running_id].transition_time_sec*1000;
//       animation.transition.rate_ms = mixer.group[mixer.running_id].transition_rate_sec*1000;
      
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




void mRGBAnimator::SubTask_Flasher_Animate(){
  
  if((mSupport::TimeReached(&flashersettings.tSaved.Update,animation.transition.rate_ms.val))||(animation.flags.fForceUpdate)){

    if(animation.flags.fForceUpdate){ animation.flags.fForceUpdate=false;
      flashersettings.tSaved.Update = millis();
    }

    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "flashersettings.tSaved.Update"));
    #endif

    switch(flashersettings.function){
      default:
      
      // case FLASHER_FUNCTION_RANDOM_ID:   // replaces transition method
      //   SubTask_Flasher_Animate_Function_Slow_Fade_Saturation_All();
      // break;
      // case FLASHER_FUNCTION_RANDOM_ID:   // replaces transition method
      //   SubTask_Flasher_Animate_Function_Slow_Fade_Saturation_All();
      // break;
      // case FLASHER_FUNCTION_ROTATE_ID:   // replaces transition method
      //   SubTask_Flasher_Animate_Function_Slow_Fade_Saturation_All();
      // break;
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
  if(flashersettings.flags.enable_random_rate){
    if(mSupport::TimeReached(&flashersettings.random_transitions.tSavedNewSpeedUpdate,flashersettings.random_transitions.rate_ms)){
      flashersettings.random_transitions.rate_ms = (random(flashersettings.random_transitions.lower_secs,flashersettings.random_transitions.upper_secs)*1000);
      animation.transition.rate_ms.val = flashersettings.random_transitions.array[flashersettings.random_transitions.array_index++]*10;
      if(flashersettings.random_transitions.array_index>=flashersettings.random_transitions.array_index_length) flashersettings.random_transitions.array_index=0;
      animation.transition.time_ms.val = animation.transition.rate_ms.val/4; //75% of time spent on desired colour
    }
  }
  // #Idea57
  // AnimEaseFunction easing = NeoEase::CubicIn;     
  // float progress = easing(param.progress);  

  




} //end flasher_animate



void mRGBAnimator::SubTask_Flasher_Animate_Function_Slow_Glow(){
  RefreshLED_Presets();
}

void mRGBAnimator::SubTask_Flasher_Animate_Function_Sequential(){
  
  flashersettings.flag_finish_flasher_pair = false;

  flashersettings.flags.enable_random_rate = true;//test

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
        AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_ANIMATE"));
        #endif
        // Shift colours (rotate)
        HsbColor colourfirst = hsbcolour[0];
        for(ledout.index=0;ledout.index<strip_size;ledout.index++){ 
          hsbcolour[ledout.index] = hsbcolour[ledout.index+1]; 
        }
        hsbcolour[strip_size-1] = colourfirst;
        // Change animation speed

        // Move this as option for animations

        // if(mSupport::TimeReached(&flashersettings.function_seq.tSavedNewSpeedUpdate,(random(3,10)*1000))){
        //   uint8_t values[8] = {4,8,10,17,20,17,10,8};
        //   animation.transition.rate_ms = values[flashersettings.function_seq.rate_index++]*50;
        //   if(flashersettings.function_seq.rate_index>8) flashersettings.function_seq.rate_index=0;
        //   animation.transition.time_ms = animation.transition.rate_ms/4; //75% of time spent on desired colour
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
      animation.transition.order_id = TRANSITION_ORDER_INORDER_ID;

      flash_twinkle_random.white_pixel_amount = 8; //how often the leds are
      // by percentage
      //flash_twinkle_random.white_pixel_amount = strip_size/5;

      // Generate ledindex for white flashers
      flash_twinkle_random.white_total_index = 0;
      for(ledout.index=0;ledout.index<strip_size;ledout.index++){ 
        //map type of led they are by numnber 0=off,1=white,2=colour
        if(!(ledout.index%flash_twinkle_random.white_pixel_amount)){
          flash_twinkle_random.white_leds_index[flash_twinkle_random.white_total_index] = ledout.index;
          flash_twinkle_random.stored_colours_index[flash_twinkle_random.white_total_index] = hsbcolour[ledout.index];
          //AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "flash_twinkle_random.white_leds_index %d ledout.index %d"),flash_twinkle_random.white_total_index,ledout.index);
          flash_twinkle_random.white_total_index++;
        }              
      }   
                  
      //flash_twinkle_random.flash_colour = HsbColor(0,0,1); //white

      // Test clear
      // for(int ledout.index=0;ledout.index<strip_size;ledout.index++){ hsbcolour[ledout.index] = HsbColor(RgbColor(0,0,0)); }

      flashersettings.flag_finish_flasher_pair = true; // must complete regions
      
    break;

    case FLASHER_REGION_ANIMATE_ID: //shift along
      AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_ANIMATE"));

      // Reset all back to colour first 
      for(int jj=0;jj<flash_twinkle_random.white_total_index;jj++){
        hsbcolour[flash_twinkle_random.white_leds_index[jj]] = flash_twinkle_random.stored_colours_index[jj];
      }

      // Pick 10 random of the leds to turn to white

      
      for(int jj=0;jj<10;jj++){
        uint16_t led_rand = random(0,flash_twinkle_random.white_total_index);
        hsbcolour[flash_twinkle_random.white_leds_index[led_rand]] = flash_twinkle_random.flash_colour;
      }

      flash_twinkle_random.white_on_index++;
      if(flash_twinkle_random.white_on_index>=flash_twinkle_random.white_pixel_amount){
        flash_twinkle_random.white_on_index = 0;
        flashersettings.region = FLASHER_REGION_COLOUR_SELECT_ID; // Wrap back
      }

      flashersettings.flag_finish_flasher_pair = false; // must complete regions

      // Change animation speed
      if(mSupport::TimeReached(&flashersettings.function_flash_twinkle.tSavedNewSpeedUpdate,random(0,1)?5000:10000)){//(random(5,10)*1000))){
        animation.transition.rate_ms.val = random(0,1)?10:150; // will need to be very fast
        animation.transition.time_ms.val = 0; //instant
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
//             animation.transition.order_id = TRANSITION_ORDER_INORDER_ID;

//             flash_twinkle_random.white_pixel_amount = 7; //how often the leds are
//             // by percentage
//             //flash_twinkle_random.white_pixel_amount = strip_size/5;

//             // Generate ledindex for white flashers
//             flash_twinkle_random.white_total_index = 0;
//             for(ledout.index=0;ledout.index<strip_size;ledout.index++){ 
//               //map type of led they are by numnber 0=off,1=white,2=colour
//               if(!(ledout.index%flash_twinkle_random.white_pixel_amount)){
//                 flash_twinkle_random.white_leds_index[flash_twinkle_random.white_total_index] = ledout.index;
//                 flash_twinkle_random.stored_colours_index[flash_twinkle_random.white_total_index] = hsbcolour[ledout.index];
//                 //AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "flash_twinkle_random.white_leds_index %d ledout.index %d"),flash_twinkle_random.white_total_index,ledout.index);
//                 flash_twinkle_random.white_total_index++;
//               }              
//             }   
                        
//             //flash_twinkle_random.flash_colour = HsbColor(0,0,1); //white
//             //flash_twinkle_random.flash_colour = HsbColor(HueN2F(HUE_HOTPINK),SatN2F(100),BrtN2F(100));

//             // Test clear
//             // for(int ledout.index=0;ledout.index<strip_size;ledout.index++){ hsbcolour[ledout.index] = HsbColor(RgbColor(0,0,0)); }

//             flashersettings.flag_finish_flasher_pair = true; // must complete regions
            
//           break;
//           case FLASHER_REGION_ANIMATE_ID: //shift along
//             AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_ANIMATE"));
    
            
//             for(int jj=0;jj<flash_twinkle_random.white_total_index;jj++){
//               if((jj%flash_twinkle_random.white_pixel_amount)==flash_twinkle_random.white_on_index){
//                 hsbcolour[flash_twinkle_random.white_leds_index[jj]] = flash_twinkle_random.flash_colour;
//               }else{
//                 hsbcolour[flash_twinkle_random.white_leds_index[jj]] = flash_twinkle_random.stored_colours_index[jj];//offcolour;
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

//             // Change animation speed
//             if(mSupport::TimeReached(&flashersettings.function_flash_twinkle.tSavedNewSpeedUpdate,random(0,1)?5000:10000)){//(random(5,10)*1000))){
//               animation.transition.rate_ms = 90;//random(0,1)?30:30; // will need to be very fast
//               animation.transition.time_ms = 0; //instant
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
//             for(ledout.index=0;ledout.index<strip_size;ledout.index++){ hsbcolour[ledout.index].B = direction; }            
//             // Change animation speed
//             if(mSupport::TimeReached(&flashersettings.function_seq.tSavedNewSpeedUpdate,(random(3,10)*100))){
//               uint8_t values[8] = {1000,1000,2000,2000,6000,6000,3000,3000}; //off,on,off
//               animation.transition.rate_ms = values[flashersettings.function_seq.rate_index++]*50;
//               if(flashersettings.function_seq.rate_index>=8) flashersettings.function_seq.rate_index=0;
//               animation.transition.time_ms = animation.transition.rate_ms;///4; // brightness shift takes 100% of the time (no colour shift)
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
//               hsbcolour[ledout.index].S = random_saturation; 
//               hsbcolour[ledout.index].B = adjusted_brightness;//random_saturation<0.5?animation.brightness*0.5:animation.brightness; //test, pair "whiter" less bright (maybe /2)  
//             }            
//             // Change animation speed
//             //if(mSupport::TimeReached(&flashersettings.function_seq.tSavedNewSpeedUpdate,(random(3,10)*100))){
//               uint16_t values[8] = {7,7,9,9,7,7,4,4}; //off,on,off
//               animation.transition.rate_ms = values[flashersettings.function_seq.rate_index++]*1000;
//               if(flashersettings.function_seq.rate_index>=8) flashersettings.function_seq.rate_index=0;
//               // holds colour more
//               //animation.transition.time_ms = direction ? (animation.transition.rate_ms.val/4) : (animation.transition.rate_ms.val); // brightness shift takes 100% of the time (no colour shift)
//               animation.transition.time_ms = random(0,1) ? (animation.transition.rate_ms.val/4) : (animation.transition.rate_ms.va;); // brightness shift takes 100% of the time (no colour shift)
//             //}
//           break;
//         }
//       break;
//       //flasher_start_white_to_Colour: 
//         // method that will start all leds on one colour (white?) and slowly add colours in with fade in
//       //flasher_change_saturation_between 0 and 100, "fade of saturation", "fade of brightness"

}


int8_t mRGBAnimator::parsesub_Flasher(JsonObjectConst obj){
  
   // Create local dereferenced variable
  // JsonObject obj = (*_obj); 

//   DynamicJsonDocument doc(1500);
//   DeserializationError error = deserializeJson(doc, data_buffer2.payload.ctr);
//   if(error){
//     AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_ERROR_JSON_DESERIALIZATION));
//     Response_mP(S_JSON_COMMAND_SVALUE, D_ERROR,D_ERROR_JSON_DESERIALIZATION);
//     return 0;
//   }
//   JsonObject obj = doc.as<JsonObject>();

  int8_t tmp_id = 0;
  int8_t isserviced = 0;

char buffer[50];

  if(!obj["function"].isNull()){ 
    const char* functionctr = obj["function"];
    if((tmp_id=GetFlasherFunctionIDbyName(functionctr))>=0){
      flashersettings.function = tmp_id;

      //new function, then generate its 
      flashersettings.region = FLASHER_REGION_COLOUR_SELECT_ID;

      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),"function",GetFlasherRegionName(buffer));
      Response_mP(S_JSON_COMMAND_SVALUE,"function",GetFlasherFunctionName(buffer));
      isserviced++;
    }else{
      AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_NAME,functionctr);
    }
  }
  
  
  if(!obj["mixer"]["enabled"].isNull()){ 
    mixer.enabled = obj["mixer"]["enabled"];
    AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),"mixer.enabled",mixer.enabled);
  }
   

  
  if(!obj["time_scaler"].isNull()){ 
    mixer.time_scaler = obj["time_scaler"];
    AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),"mixer.time_scaler",mixer.time_scaler);
  }
   

  


  // TIME with different units
  if(!obj[D_JSON_TIME].isNull()){ //default to secs
    animation.transition.time_ms.val = obj["time"];
    animation.transition.time_ms.val *= 1000;
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),animation.transition.time_ms.val);  
  }else
  if(!obj[D_JSON_TIME].isNull()){
    animation.transition.time_ms.val = obj["time_secs"];
    animation.transition.time_ms.val *= 1000;
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),animation.transition.time_ms.val);  
  }else
  if(!obj[D_JSON_TIME_MS].isNull()){
    animation.transition.time_ms.val = obj["time_ms"];
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),animation.transition.time_ms.val);  
  }

  return isserviced;

} // END FUNCTION


// // Update struct that shows overview, always sends
uint8_t mRGBAnimator::ConstructJSON_Flasher(uint8_t json_level){


//   AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::ConstructJSON_Flasher"));
//   DynamicJsonDocument doc(1500);
//   JsonObject root = doc.to<JsonObject>();

//   // root[D_JSON_ONOFF] = fLEDStripOnOff ? "ON" : "OFF";
//   // root[D_JSON_MODE] = GetAnimationModeName();
//   root["function"] = GetFlasherFunctionName();
//   root["region"] = GetFlasherRegionName();
//   // root[D_JSON_COLOUR_PALETTE] = GetPaletteFriendlyName();
//   // root[D_JSON_BRIGHTNESS_PERCENTAGE] = animation.brightness*100;
//   // root[D_JSON_BRIGHTNESS] = animation.brightness;

//   // JsonObject transitionobj = root.createNestedObject(D_JSON_TRANSITION);
//   //   transitionobj[D_JSON_METHOD] = GetTransitionMethodName();
//   //   transitionobj[D_JSON_TIME] = mSupport::safeDivideInt(animation.transition.time_ms,1000);
//   //   transitionobj[D_JSON_TIME_MS] = animation.transition.time_ms;
//   //   transitionobj[D_JSON_RATE] = mSupport::safeDivideInt(animation.transition.rate_ms,1000);
//   //   transitionobj[D_JSON_RATE_MS] = animation.transition.rate_ms;
//   //   transitionobj["function"] = GetFlasherFunctionName();

//   JsonObject seq_obj = root.createNestedObject("sequential");
//     seq_obj["rate_index"] = flashersettings.function_seq.rate_index;

//   JsonObject sloglo_obj = root.createNestedObject("slo_glo");
//     sloglo_obj["rate_index"] = flashersettings.function_slo_glo.rate_index;



//   // Flags and states that are used during one transition and reset when completed
//   // JsonObject overridesobj = root.createNestedObject("transition_overrides"); 
//   //   overridesobj["fRefreshAllPixels"] = animation_override.fRefreshAllPixels;
  
//   // root["ledout.length"] = ledout.length;

//   data_buffer2.payload.len = measureJson(root)+1;
//   serializeJson(doc,data_buffer2.payload.ctr);

//   return (data_buffer2.payload.len>3?1:0);
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

  if(animations_control->IsAnimating()){
    sprintf(buffer, "%s", "Animating");
    return buffer;
  }
  if(animation.flags.fEnable_Animation){
    // (millis since) + (next event millis)
    int until_next_millis = animation.transition.rate_ms.val-(millis()-animation_changed_millis);
    float secs_until_next_event = (float)(until_next_millis);
    secs_until_next_event/=1000;
    // Serial.println(secs_until_next_event);

    char float_ctr[10];
    dtostrf(secs_until_next_event,3,1,float_ctr);
    // Serial.println(float_ctr);

    // AddLog_P(LOG_LEVEL_INFO,PSTR("GetAnimationStatusCtr %d %d"),
    //   until_next_millis,
    //   //millis(),animation_changed_millis,animation.transition.rate_ms.val,
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


//add switch case for how white should be handled
//off - leave it
//on - full direct control
//tied with saturation - works opposite to saturation
//colour temperature?

  #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
  //if rgbw then optional bind white channel
  //color_hardware.W = 1-color_hardware; //need saturation for this
  #endif

  //AddLog_P(LOG_LEVEL_DEBUG,PSTR("pixel[%d] = %d,%d,%d,%d"),indexPixel,color_hardware.R,color_hardware.G,color_hardware.B,color_hardware.W);

  stripbus->SetPixelColor(indexPixel,color_hardware);

}

RgbTypeColor mRGBAnimator::GetPixelColor(uint16_t indexPixel)
{
  DEBUG_LINE;
  if(stripbus == nullptr){    
    #ifdef ENABLE_LOG_LEVEL_DEBUG
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

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.configperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_settings;
  mqtthandler_ptr->ConstructJSON_function = &mRGBAnimator::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_animation_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_animation;
  mqtthandler_ptr->ConstructJSON_function = &mRGBAnimator::ConstructJSON_Animation;

  mqtthandler_ptr = &mqtthandler_ambilight_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_ambilight;
  mqtthandler_ptr->ConstructJSON_function = &mRGBAnimator::ConstructJSON_Ambilight;
  
  #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS      //make this optional, as it uses extra data and is only for special cases
  mqtthandler_ptr = &mqtthandler_notifications_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_notifications;
  mqtthandler_ptr->ConstructJSON_function = &mRGBAnimator::ConstructJSON_Notifications;
  #endif
  
  mqtthandler_ptr = &mqtthandler_scene_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_scene;
  mqtthandler_ptr->ConstructJSON_function = &mRGBAnimator::ConstructJSON_Scene;
  
  mqtthandler_ptr = &mqtthandler_state_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_state;
  mqtthandler_ptr->ConstructJSON_function = &mRGBAnimator::ConstructJSON_State;
  
  mqtthandler_ptr = &mqtthandler_timed_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_timed;
  mqtthandler_ptr->ConstructJSON_function = &mRGBAnimator::ConstructJSON_Timed;

#ifdef USE_TASK_RGBLIGHTING_FLASHER_AND_MIXER
  mqtthandler_ptr = &mqtthandler_flasher_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_flasher;
  mqtthandler_ptr->ConstructJSON_function = &mRGBAnimator::ConstructJSON_Flasher;

  mqtthandler_ptr = &mqtthandler_mixer_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_mixer;
  mqtthandler_ptr->ConstructJSON_function = &mRGBAnimator::ConstructJSON_Mixer;
  #endif

} //end "MQTTHandler_Init"


void mRGBAnimator::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.fSendNow = true;
  mqtthandler_animation_teleperiod.fSendNow = true;
  mqtthandler_ambilight_teleperiod.fSendNow = true;
  #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS 
  mqtthandler_notifications_teleperiod.fSendNow = true;
  #endif
  mqtthandler_scene_teleperiod.fSendNow = true;
  mqtthandler_state_teleperiod.fSendNow = true;
  mqtthandler_timed_teleperiod.fSendNow = true;
  mqtthandler_flasher_teleperiod.fSendNow = true;
  mqtthandler_mixer_teleperiod.fSendNow = true;

} //end "MQTTHandler_Init"


void mRGBAnimator::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_animation_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_ambilight_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS 
  mqtthandler_notifications_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  #endif
  mqtthandler_scene_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_state_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_timed_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_flasher_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_mixer_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mRGBAnimator::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t flag_handle_all = false, handler_not_found = true;
  if(mqtt_handler_id == MQTT_HANDLER_ALL_ID){ flag_handle_all = true; } //else run only the one asked for

  do{

    //AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEO "MQTTHandler_Sender %d"),mqtt_handler_id);

    handler_not_found=false; // start as true, if none found, default clears to false
    switch(mqtt_handler_id){
      case MQTT_HANDLER_SETTINGS_ID:                       mqtthandler_ptr=&mqtthandler_settings_teleperiod; break;
      case MQTT_HANDLER_MODULE_ANIMATION_TELEPERIOD_ID:    mqtthandler_ptr=&mqtthandler_animation_teleperiod; break;
      case MQTT_HANDLER_MODULE_AMBILIGHT_TELEPERIOD_ID:    mqtthandler_ptr=&mqtthandler_ambilight_teleperiod; break;
  #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS 
      case MQTT_HANDLER_MODULE_NOTIFICATION_TELEPERIOD_ID: mqtthandler_ptr=&mqtthandler_notifications_teleperiod; break;
      #endif
      case MQTT_HANDLER_MODULE_SCENE_TELEPERIOD_ID:        mqtthandler_ptr=&mqtthandler_scene_teleperiod; break;
      case MQTT_HANDLER_MODULE_STATE_TELEPERIOD_ID:        mqtthandler_ptr=&mqtthandler_state_teleperiod; break;
      case MQTT_HANDLER_MODULE_TIMED_TELEPERIOD_ID:        mqtthandler_ptr=&mqtthandler_timed_teleperiod; break;
      
      #ifdef USE_TASK_RGBLIGHTING_FLASHER_AND_MIXER
      case MQTT_HANDLER_MODULE_FLASHER_TELEPERIOD_ID:      mqtthandler_ptr=&mqtthandler_flasher_teleperiod; break;
      case MQTT_HANDLER_MODULE_MIXER_TELEPERIOD_ID:        mqtthandler_ptr=&mqtthandler_mixer_teleperiod; break;
      #endif
      default: handler_not_found=true; break; // nothing 
    } // switch

    // Pass handlers into command to test and (ifneeded) execute
    if(!handler_not_found){ pCONT->mqt->MQTTHandler_Command(*this,D_MODULE_LIGHTS_ADDRESSABLE_ID,mqtthandler_ptr); }

    // stop searching
    if(mqtt_handler_id++>MQTT_HANDLER_MODULE_LENGTH_ID){flag_handle_all = false; return;}

  }while(flag_handle_all);

}

////////////////////// END OF MQTT /////////////////////////

// #endif

/*******************************************************************************************************************
********************************************************************************************************************
************CODE ABOVE THIS LINE IS PART OF THE NEWEST REWRITE********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/







void mRGBAnimator::init_Animations(){

  
  // Set default values (ifdef below sets specific)
  animation.transition.order_id = TRANSITION_ORDER_INORDER_ID;
  animation.palette_id = pCONT_iLight->PALETTELIST_VARIABLE_USER_01_ID;
  animation.mode_id = ANIMATION_MODE_FLASHER_ID;
  flashersettings.function = FLASHER_FUNCTION_SLOW_GLOW_ID;
  animation.transition.method_id = TRANSITION_METHOD_BLEND_ID;
  animation.flags.fForceUpdate = true;
  animation.brightness = 0.1;
  animation.transition.time_ms.val = 1000;
  animation.transition.rate_ms.val = 20*1000;
  // tSavedUpdateRGBString = millis() + animation.transition.rate_ms.val;
  first_set = true; //change to one off override flag


  animation.flags.fEnable_Animation = true;

}





void mRGBAnimator::init_Ambilight(){

  ambilightsettings.screens[SCREEN_CENTRE].top.colour    = HsbColor(HueN2F(20),SatN2F(95),BrtN2F(100));
  ambilightsettings.screens[SCREEN_CENTRE].bottom.colour = HsbColor(HueN2F(8),SatN2F(95),BrtN2F(100));
  ambilightsettings.screens[SCREEN_CENTRE].left.colour   = HsbColor(HueN2F(240),SatN2F(100),BrtN2F(100));
  ambilightsettings.screens[SCREEN_CENTRE].right.colour  = HsbColor(HueN2F(330),SatN2F(100),BrtN2F(100));
  ambilightsettings.screens[SCREEN_CENTRE].top.size = 33;
  ambilightsettings.screens[SCREEN_CENTRE].bottom.size = 33;
  ambilightsettings.screens[SCREEN_CENTRE].left.size = 19;
  ambilightsettings.screens[SCREEN_CENTRE].right.size = 19;
  ambilightsettings.screens[SCREEN_CENTRE].left.blend_between_sides_gradient_percentage = 50;

}

// void mRGBAnimator::UpdateBrightness()
// animation.brightness

  //if(timed event, tasker, reduce time, fill command)
  // retains setting to excute later, turn off, change pattern etc
void mRGBAnimator::SubTask_TimedLEDMode(){

  // Need to use stored struct copies for this. 
  // Set flag that data in stored structs are to be executed after x seconds
  // animation_stored,scene_stored
  // copy new pattern over and execute

  // if((abs(millis()-timedon.tick)>=1000)||(timedon.fForceUpdate)){ timedon.tick = millis(); timedon.fForceUpdate = false;

  //   if(timedon.secs>0){timedon.secs--;
  //     // RefreshLED_Presets();
  //     // UpdateLEDs();
  //     // timedon.fUpdated = true;
  //   }else if(timedon.secs==0){timedon.secs = -1;
  //     TurnLEDsOff();
  //     timedon.fUpdated = true;
  //   }else if(timedon.secs<0){
  //     //nothing - should be off
  //   }

  // }

} // END FUNCTION

void mRGBAnimator::SubTask_Presets(){

  //PHASE OUT
    
  // if((abs(millis()-tSavedUpdateRGBString)>(animation.transition.rate_ms.val))||(animation.flags.fForceUpdate)){
  //   tSavedUpdateRGBString = millis(); 
    
  //   animation_changed_millis = millis();

  //   AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "f::SubTask_Presets" "tSavedUpdateRGBString %d"),animation.transition.rate_ms.val);
          
  //   if(animation.flags.fForceUpdate){animation.flags.fForceUpdate=false;}
  //   RefreshLED_Presets();
  //   ConfigureLEDTransitionAnimation();
  // }


}






// Generate random colour between two hsb colours
HsbColor mRGBAnimator::GetRandomColour(HsbColor colour1, HsbColor colour2){
  int random_hue = random(HueF2N(colour1.H),HueF2N(colour2.H));
  int random_sat = random(SatF2N(colour1.S),SatF2N(colour2.S));
  int random_brt = random(BrtF2N(colour1.B),BrtF2N(colour2.B));
  return HsbColor(HueN2F(random_hue),SatN2F(random_sat),BrtN2F(random_brt));
}


/*******************************************************************************************************************
********************************************************************************************************************
************ANIMATION AND BLENDING**************************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/


// CASES: for colour type
// Substate?: for how they are updated, randomly, all
void mRGBAnimator::RefreshLED_Presets(void){ 
  
  #ifdef ENABLE_LOG_LEVEL_DEBUG
  AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "f::RefreshLED_Presets"));
  #endif

  // Update pointer of struct
  pCONT_iLight->SetPaletteListPtrFromID(animation.palette_id);

  // things that use indexes
  RefreshLEDOutputStream();

  // Handle brightness levels == if not set, use default
  for(ledout.index=0;ledout.index<ledout.length;ledout.index++){ 
    //if(!palettelist.ptr->fOverride_animation_brightness){
      hsbcolour[ledout.index].B = animation.brightness;
    //}
  }

} //end function

void mRGBAnimator::RefreshLEDIndexPattern(){

  // Somehow this orders need to become flasher... which I will rename animation

  // Generate lighting pattern
  switch(animation.transition.order_id){
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

  // AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "RefreshLED_Presets animation.palette_id  \"%s\""),GetPaletteFriendlyName());
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
        HsbColor colour = pCONT_iLight->GetColourFromPalette(pCONT_iLight->palettelist.ptr,desired_pixel,&pixel_position);
        
        #ifdef ENABLE_LOG_LEVEL_DEBUG
        AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "dp%d, pp%d, %d,%d %d"),desired_pixel, pixel_position,HueF2N(colour.H),SatF2N(colour.S),BrtF2N(colour.B));
        #endif
        
        if(pixel_position>=0){

          // Set output to this "many" colour
          if(pixel_position == 255){
            for(uint16_t temp=0;temp<ledout.length;temp++){ hsbcolour[temp] = colour; }
            
            #ifdef ENABLE_LOG_LEVEL_DEBUG
            AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "set ALL %d,%d %d"),HueF2N(colour.H),SatF2N(colour.S),BrtF2N(colour.B));
            #endif

          }else{
            hsbcolour[pixel_position] = colour;
            
            #ifdef ENABLE_LOG_LEVEL_DEBUG
            AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "set %d %d,%d %d"), pixel_position, HueF2N(colour.H),SatF2N(colour.S),BrtF2N(colour.B));
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
      //       for(uint16_t temp=0;temp<ledout.length;temp++){ hsbcolour[temp] = colour; }
      //       AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "set ALL %d,%d %d"),HueF2N(colour.H),SatF2N(colour.S),BrtF2N(colour.B));
      //     }else{
      //       hsbcolour[pixel_position] = colour;
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
//         RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(fromcolor, targetColor, progress);  // use the curve value to apply to the animation
//         SetPixelColor(pixel, updatedColor);
//       };


  #ifdef ENABLE_LOG_LEVEL_DEBUG
          AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID"));
          #endif

      uint16_t start_pixel = 0;
      uint16_t end_pixel = 100;
      HsbColor start_colour = HsbColor(0);//preset_colour_map[COLOUR_MAP_RED_ID];
      HsbColor end_colour = HsbColor(0);//preset_colour_map[COLOUR_MAP_GREEN_ID];
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

  #ifdef ENABLE_LOG_LEVEL_DEBUG
        AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "s%d,%d %d %d"),HueF2N(start_colour.H),SatF2N(start_colour.S),BrtF2N(start_colour.B),start_pixel_position);
        //AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "e%d,%d %d %d"),HueF2N(end_colour.H),SatF2N(end_colour.S),BrtF2N(end_colour.B),end_pixel_position);
#endif

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
          hsbcolour[ledout.index] = 
          //HsbColor(
            RgbTypeColor::LinearBlend(start_colour, end_colour, progress)
          //)
          ;
          #else

          RgbTypeColor rgb = RgbTypeColor::LinearBlend(start_colour, end_colour, progress);
          hsbcolour[ledout.index] = HsbColor(RgbColor(rgb.R,rgb.G,rgb.B));

          // hsbcolour[ledout.index] = RgbwColor(0);
          
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
  switch(animation.transition.order_id){
    case TRANSITION_ORDER_INORDER_ID:
    case TRANSITION_ORDER_RANDOM_ID:{
      RefreshLEDIndexPattern();
      int16_t pixel_position = -2;
      for(ledout.index=0;ledout.index<ledout.length;ledout.index++){
        HsbColor colour = pCONT_iLight->GetColourFromPalette(pCONT_iLight->palettelist.ptr,desired_pixel,&pixel_position);

        hsbcolour[ledout.pattern[ledout.index]] = colour;
        
        #ifdef ENABLE_LOG_LEVEL_DEBUG
        AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "colour[p%d:d%d] = %d,%d,%d %d pp%d"),
          ledout.pattern[ledout.index],desired_pixel,
          HueF2N(hsbcolour[ledout.pattern[ledout.index]].H),SatF2N(hsbcolour[ledout.pattern[ledout.index]].S),BrtF2N(hsbcolour[ledout.pattern[ledout.index]].B),
          pCONT_iLight->GetPixelsInMap(pCONT_iLight->palettelist.ptr),pixel_position
        );
        #endif

        if(++desired_pixel>=pCONT_iLight->GetPixelsInMap(pCONT_iLight->palettelist.ptr)){desired_pixel=0;}
      } //end for
    }break;
    case TRANSITION_ORDER_FIXED_ID:

      int16_t pixel_position = -2;
      //move across all encoded values
        // HsbColor colour = GetColourFromPalette(pCONT_iLight->palettelist.ptr,desired_pixel,&pixel_position);

        // if(pixel_position>=0){
        //   hsbcolour[pixel_position] = colour;
        // }else{
        //   hsbcolour[ledout.pattern[ledout.index]] = colour;
        // }
        
        // AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "colour[p%d:d%d] = %d,%d,%d %d pp%d"),
        //   ledout.pattern[ledout.index],desired_pixel,
        //   HueF2N(hsbcolour[ledout.pattern[ledout.index]].H),SatF2N(hsbcolour[ledout.pattern[ledout.index]].S),BrtF2N(hsbcolour[ledout.pattern[ledout.index]].B),
        //   pCONT_iLight->palettelist.ptr->active_pixels_in_map,pixel_position
        // );    

    break;
  }//end switch

} //end function RefreshLEDOutputStream();






// optional fromcolor to merge "FadeToNewColour" and "FadeBetweenColours"
// If unset (as defined in header) the animation will blend from current pixel colours with getpixel
void mRGBAnimator::FadeToNewColour(RgbTypeColor targetColor, uint16_t _time_to_newcolour,  RgbTypeColor fromcolor){ 

  #ifdef ENABLE_LOG_LEVEL_DEBUG
    //AddLog_P(LOG_LEVEL_DEBUG_LOWLEVEL, PSTR(D_LOG_NEO "FadeToNewColour"));
  #endif
  
  if(NEO_ANIMATION_TIMEBASE == NEO_CENTISECONDS){
    _time_to_newcolour /= 1000;// ms to seconds
  }

  // Overwriting single animation methods, set, then clear
  if(animation_override.time_ms){
    _time_to_newcolour =animation_override.time_ms;
    animation_override.time_ms = 0;//reset overwrite
  }
  
  AnimEaseFunction easing = NeoEase::CubicIn;  

  for (uint16_t pixel = 0; pixel < strip_size; pixel++){
    // Check if fromcolor is defined
    if(!(fromcolor.R+fromcolor.G+fromcolor.B)){ // This might cause a bug during fully off to on
      fromcolor = GetPixelColor(pixel);
    }
    
    AnimUpdateCallback animUpdate = [=](const AnimationParam& param){
      float progress = easing(param.progress);  
      RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(fromcolor, targetColor, progress); 
      SetPixelColor(pixel, updatedColor);
    };
    
    animations_control->StartAnimation(pixel, _time_to_newcolour, animUpdate);
  }

} // END function


void mRGBAnimator::SetAnimationProfile(uint8_t profile_id){
  #ifdef ENABLE_LOG_LEVEL_DEBUG
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::SetAnimationProfile" " START"));
  #endif
          
  switch(profile_id){
    case ANIMATION_PROFILE_TURN_OFF_ID:
      //if palette, set colour to black and update all
      switch(animation.mode_id){
        case ANIMATION_MODE_FLASHER_ID://PRESETS_ID:
          #ifdef ENABLE_LOG_LEVEL_DEBUG
          AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::SetAnimationProfile" "ANIMATION_MODE_FLASHER_ID"));
          #endif
          animation_override.time_ms = 1000; //force fast rate to turn on
          flashersettings.function = FLASHER_FUNCTION_SLOW_GLOW_ID;
          animation.flags.fForceUpdate = true;
          animation.brightness = 0;
          first_set = 1;//set all
        break;
        default:
          #ifdef ENABLE_LOG_LEVEL_DEBUG
          AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::SetAnimationProfile" "default"));
          #endif
        case ANIMATION_MODE_SCENE_ID:
          #ifdef ENABLE_LOG_LEVEL_DEBUG
          AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::SetAnimationProfile" "ANIMATION_MODE_SCENE_ID"));
          #endif
          animation.brightness = 0;
          animation.transition.time_ms.val = 1000;
          animation.flags.fForceUpdate = true;
          animation_override.time_ms = 1000; //force fast rate to turn on
          
          scene.name_id = SCENES_COLOURSCENE_ID;
          animation.mode_id = ANIMATION_MODE_SCENE_ID;
          
          first_set = 1;//set all
        break;
      }
      animation.flags.fEndUpdatesWhenAnimationCompletes = true; //once turned off, stop animations
    break;
    case ANIMATION_PROFILE_TURN_ON_ID:

        switch(animation.mode_id){
          // case ANIMATION_MODE_PRESETS_ID:
          
          // break;
          default:
          case ANIMATION_MODE_SCENE_ID:
            scene.name_id = SCENES_COLOURSCENE_ID;
            animation.mode_id = ANIMATION_MODE_SCENE_ID;
          break;
        }

        animation_override.time_ms = 1000; //force fast rate to turn on
        animation.brightness = 1;
        animation.flags.fForceUpdate = true;
        first_set = 1;//set all
    break;
    case ANIMATION_PROFILE_NOTHING_ID:

    break;
  }

}// END SetAnimationProfile function


void mRGBAnimator::ConfigureLEDTransitionAnimation(){ 
  
  #ifdef ENABLE_LOG_LEVEL_DEBUG
  AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "f::ConfigureLEDTransitionAnimation"));
  #endif

  animation_changed_millis = millis();

  uint16_t time_tmp = 0;
  switch(animation.transition.method_id){
    default: 
    case TRANSITION_METHOD_INSTANT_ID: time_tmp = 0; break;
    case TRANSITION_METHOD_BLEND_ID:   time_tmp = animation.transition.time_ms.val; break;
  }

  // Overwriting single animation methods, set, then clear
  if(animation_override.time_ms){
    time_tmp = animation_override.time_ms;
    animation_override.time_ms = 0;//reset overwrite
  }

  if(time_tmp>0){
    if(NEO_ANIMATION_TIMEBASE == NEO_CENTISECONDS){
      time_tmp /= 1000;// ms to seconds
    }
  }

  AnimEaseFunction easing = NeoEase::CubicIn;  
  RgbTypeColor originalColor;
  RgbTypeColor targetColor;

  for (uint16_t pixel = 0; pixel < strip_size; pixel++){
  
    originalColor = GetPixelColor(pixel);
    targetColor = HsbColor(hsbcolour[pixel]);
    
    AnimUpdateCallback animUpdate = [=](const AnimationParam& param){
      SetPixelColor(pixel, RgbTypeColor::LinearBlend(originalColor, targetColor, easing(param.progress)));
    };
          
    animations_control->StartAnimation(pixel, time_tmp, animUpdate);
}

} //end function

void mRGBAnimator::TurnLEDsOff(){
  //stripbus->ClearTo(0);
  for(uint16_t pixel = 0; pixel < strip_size; pixel++){
    SetPixelColor(pixel, 0);
  }
  stripbus->Show();
}

void mRGBAnimator::SetRGBwithNeoPixel(){
  for(uint16_t pixel = 0; pixel < strip_size; pixel++){
    SetPixelColor(pixel,HsbColor(hsbcolour[pixel].H/360.0f, hsbcolour[pixel].S/100.0f, hsbcolour[pixel].B/100.0f));
  }
  stripbus->Show();
}

void mRGBAnimator::SetRGBwithNeoPixelImageTest(){

  
    // for (auto copies = 0; copies < 3; copies++) {
    //   image.Blt(stripbus, copies * image.PixelCount());
      
    // }


  for(uint16_t pixel = 0; pixel < strip_size; pixel++){
    SetPixelColor(pixel,HsbColor(hsbcolour[pixel].H/360.0f, hsbcolour[pixel].S/100.0f, hsbcolour[pixel].B/100.0f));
  }
  stripbus->Show();
}





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


void mRGBAnimator::AddToJsonObject_AddHardware(JsonObject root){

  // JsonObject neopixels = root.createNestedObject("neopixels");

  // #ifdef USE_WS28XX_HARDWARE_WS2801
  //   neopixels["type"] = "WS2801";
  // #else
  //   #ifdef USE_WS2812_DMA
  //     neopixels["type"] = "WS2812_DMA";
  //   #else
  //     neopixels["type"] = "WS2812_PIN_ANY";
  //   #endif
  // #endif
  // #ifdef RGB_DATA_PIN
  // neopixels["datapin"] = RGB_DATA_PIN;
  // #endif
  // neopixels["clockpin"] = 1;//RGB_CLOCK_PIN;
  // neopixels["size"] = strip_size;

}


// Single calls that configure Tasker into correct mode
// ie Set Autumn configures preset mode
// void mRGBAnimator::SetMode_UpdateColour(uint8_t colour){
//   animation.palette_id = colour;
//   animation.mode_id = ANIMATION_MODE_PRESETS_ID;
// }

// H ue360toFloat

// make these static and global supports, requiring no object ? inline??

float mRGBAnimator::HueN2F(uint16_t hue){
  return hue/360.0f;
}
// S at100toFloat
float mRGBAnimator::SatN2F(uint8_t sat){
  return sat/100.0f;
}
// B rt100toFloat
float mRGBAnimator::BrtN2F(uint8_t brt){
  return brt/100.0f;
}

//remove 360/100 and make HueF2N and HueN2F
// H ueFloatto360
uint16_t mRGBAnimator::HueF2N(float hue){
  return round(hue*360.0f);
}
// S atFloatto100
uint8_t mRGBAnimator::SatF2N(float sat){
  return round(sat*100.0f);
}
// B rtFloatto100
uint8_t mRGBAnimator::BrtF2N(float brt){
  return round(brt*100.0f);
}


const char* mRGBAnimator::GetAnimationModeName(char* buffer){
  return GetAnimationModeNameByID(animation.mode_id, buffer);
}
const char* mRGBAnimator::GetAnimationModeNameByID(uint8_t id, char* buffer){
  switch(id){
    default:
    case ANIMATION_MODE_AMBILIGHT_ID:     memcpy_P(buffer, PM_ANIMATION_MODE_AMBILIGHT_NAME_CTR, sizeof(PM_ANIMATION_MODE_AMBILIGHT_NAME_CTR)); break;
    #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
      case ANIMATION_MODE_NOTIFICATIONS_ID: memcpy_P(buffer, PM_ANIMATION_MODE_NOTIFICATIONS_NAME_CTR,sizeof(PM_ANIMATION_MODE_NOTIFICATIONS_NAME_CTR)); break;
    #endif
    case ANIMATION_MODE_FLASHER_ID:       memcpy_P(buffer, PM_ANIMATION_MODE_FLASHER_NAME_CTR, sizeof(PM_ANIMATION_MODE_FLASHER_NAME_CTR)); break;
    case ANIMATION_MODE_SCENE_ID:         memcpy_P(buffer, PM_ANIMATION_MODE_SCENE_NAME_CTR , sizeof(PM_ANIMATION_MODE_SCENE_NAME_CTR)); break;
    case ANIMATION_MODE_NONE_ID:          memcpy_P(buffer, PM_ANIMATION_MODE_NONE_NAME_CTR , sizeof(PM_ANIMATION_MODE_NONE_NAME_CTR)); break;
  }
  return buffer;
} 
int8_t mRGBAnimator::GetAnimationModeIDbyName(const char* c){
  if(c=='\0'){
    return -1;
  }
  if(strstr_P(c,PM_ANIMATION_MODE_NONE_NAME_CTR)){ return ANIMATION_MODE_NONE_ID; }
  #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
  if(strstr_P(c,PM_ANIMATION_MODE_NOTIFICATIONS_NAME_CTR)){  return ANIMATION_MODE_NOTIFICATIONS_ID; }
  #endif
  if(strstr_P(c,PM_ANIMATION_MODE_AMBILIGHT_NAME_CTR)){      return ANIMATION_MODE_AMBILIGHT_ID; }
  if(strstr_P(c,PM_ANIMATION_MODE_SCENE_NAME_CTR)){          return ANIMATION_MODE_SCENE_ID; }
  if(strstr_P(c,PM_ANIMATION_MODE_FLASHER_NAME_CTR)){        return ANIMATION_MODE_FLASHER_ID; }
  return -1;
}




const char* mRGBAnimator::GetHardwareColourTypeName(char* buffer){
  return GetHardwareColourTypeNameByID(animation.mode_id, buffer);
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
  return GetTransitionMethodNameByID(animation.transition.method_id,buffer);
}
const char* mRGBAnimator::GetTransitionMethodNameByID(uint8_t id, char* buffer){
  switch(id){ default: 
    case TRANSITION_METHOD_BLEND_ID: memcpy_P(buffer,PM_TRANSITION_METHOD_BLEND_NAME_CTR,sizeof(PM_TRANSITION_METHOD_BLEND_NAME_CTR)); // smooth shift between them
  //   case TRANSITION_METHOD_INSTANT_ID: return D_TRANSITION_METHOD_INSTANT_NAME_CTR; // instant shift
  //   case TRANSITION_METHOD_TWINKLE_ID: return D_TRANSITION_METHOD_TWINKLE_NAME_CTR; // blend shift with random twinkles on random number of leds
  //   case TRANSITION_METHOD_GLIMMER_ID: return D_TRANSITION_METHOD_GLIMMER_NAME_CTR; // blend shift with random twinkles on random number of leds
  }
  return WARNING_NOTHANDLED_CTR;
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





int8_t mRGBAnimator::GetNearestColourMapIDFromColour(HsbColor hsb){

}

const char* mRGBAnimator::GetColourMapNameNearestbyColour(HsbColor c, char* buffer){

// TOO MUCH MEMROY!!!
  return WARNING_NOTHANDLED_CTR;
// return "memory issue";

  // uint32_t colour32bit_tofind = WebColorFromHsbColour(RgbColor(c));

  // uint64_t colour32bit_tosearch[PRESET_COLOUR_MAP_INDEXES_MAX];
  
  // for(int i=0; i<PRESET_COLOUR_MAP_INDEXES_MAX; i++) {
  //   colour32bit_tosearch[i] = (WebColorFromHsbColour(RgbColor(c))-colour32bit_tofind);
  // }

  // uint64_t temp = colour32bit_tosearch[0];
  // uint16_t index = 0;
  // for(int i=0; i<PRESET_COLOUR_MAP_INDEXES_MAX; i++) {
  //     if(temp>colour32bit_tosearch[i]) {
  //        temp=colour32bit_tosearch[i];
  //        index = i;
  //     }
  // }

  // return GetColourMapNamebyID(index);

}




int8_t mRGBAnimator::GetColourMapIDbyName(const char* c){
  if(c=='\0') return -1;
  // if(strstr(c,D_COLOUR_MAP_RED_CTR))          return COLOUR_MAP_RED_ID;
  // if(strstr(c,D_COLOUR_MAP_RED_PASTEL80_CTR))          return COLOUR_MAP_RED_PASTEL80_ID;
  // if(strstr(c,D_COLOUR_MAP_RED_PASTEL60_CTR))          return COLOUR_MAP_RED_PASTEL60_ID;
  // if(strstr(c,D_COLOUR_MAP_ORANGE_CTR))       return COLOUR_MAP_ORANGE_ID;
  // if(strstr(c,D_COLOUR_MAP_LIGHTORANGE_CTR))  return COLOUR_MAP_LIGHTORANGE_ID;
  // if(strstr(c,D_COLOUR_MAP_YELLOW_CTR))       return COLOUR_MAP_YELLOW_ID;
  // if(strstr(c,D_COLOUR_MAP_LIMEGREEN_CTR))    return COLOUR_MAP_LIMEGREEN_ID;
  // if(strstr(c,D_COLOUR_MAP_GREEN_CTR))        return COLOUR_MAP_GREEN_ID;
  // if(strstr(c,D_COLOUR_MAP_CYAN_CTR))         return COLOUR_MAP_CYAN_ID;
  // if(strstr(c,D_COLOUR_MAP_BLUE_CTR))         return COLOUR_MAP_BLUE_ID;
  // if(strstr(c,D_COLOUR_MAP_BLUEPURPLE_CTR))   return COLOUR_MAP_BLUEPURPLE_ID;
  // if(strstr(c,D_COLOUR_MAP_PURPLE_CTR))       return COLOUR_MAP_PURPLE_ID;
  // if(strstr(c,D_COLOUR_MAP_PINK_CTR))         return COLOUR_MAP_PINK_ID;
  // if(strstr(c,D_COLOUR_MAP_HOTPINK_CTR))      return COLOUR_MAP_HOTPINK_ID;
  // if(strstr(c,D_COLOUR_MAP_NONE_CTR))      return COLOUR_MAP_NONE_ID;
  // if(strstr(c,D_COLOUR_MAP_PALETTE_OPTIONS_END_CTR))      return COLOUR_MAP_PALETTE_OPTIONS_END_ID;

  
  return -1;
}
const char* mRGBAnimator::GetColourMapNamebyID(uint8_t id, char* buffer){

  return 0;

  switch(id){
    // Red
    case COLOUR_MAP_RED_ID:                    return D_COLOUR_MAP_RED_CTR;
    case COLOUR_MAP_RED_PASTEL95_ID:           return D_COLOUR_MAP_RED_PASTEL95_CTR;
    case COLOUR_MAP_RED_PASTEL90_ID:           return D_COLOUR_MAP_RED_PASTEL90_CTR;
    case COLOUR_MAP_RED_PASTEL80_ID:           return D_COLOUR_MAP_RED_PASTEL80_CTR;
    case COLOUR_MAP_RED_PASTEL70_ID:           return D_COLOUR_MAP_RED_PASTEL70_CTR;
    case COLOUR_MAP_RED_PASTEL60_ID:           return D_COLOUR_MAP_RED_PASTEL60_CTR;
    case COLOUR_MAP_RED_PASTEL50_ID:           return D_COLOUR_MAP_RED_PASTEL50_CTR;
    case COLOUR_MAP_RED_PASTEL40_ID:           return D_COLOUR_MAP_RED_PASTEL40_CTR;
    case COLOUR_MAP_RED_PASTEL20_ID:           return D_COLOUR_MAP_RED_PASTEL20_CTR;
    // Orange
    case COLOUR_MAP_ORANGE_ID:        return D_COLOUR_MAP_ORANGE_CTR;
    case COLOUR_MAP_ORANGE_PASTEL95_ID:        return D_COLOUR_MAP_ORANGE_PASTEL95_CTR;
    case COLOUR_MAP_ORANGE_PASTEL90_ID:        return D_COLOUR_MAP_ORANGE_PASTEL90_CTR;
    case COLOUR_MAP_ORANGE_PASTEL80_ID:        return D_COLOUR_MAP_ORANGE_PASTEL80_CTR;
    case COLOUR_MAP_ORANGE_PASTEL70_ID:        return D_COLOUR_MAP_ORANGE_PASTEL70_CTR;
    case COLOUR_MAP_ORANGE_PASTEL60_ID:        return D_COLOUR_MAP_ORANGE_PASTEL60_CTR;
    case COLOUR_MAP_ORANGE_PASTEL50_ID:        return D_COLOUR_MAP_ORANGE_PASTEL50_CTR;
    case COLOUR_MAP_ORANGE_PASTEL40_ID:        return D_COLOUR_MAP_ORANGE_PASTEL40_CTR;
    // Light Orange
    case COLOUR_MAP_LIGHTORANGE_ID:   return D_COLOUR_MAP_LIGHTORANGE_CTR;
    case COLOUR_MAP_LIGHTORANGE_PASTEL95_ID:   return D_COLOUR_MAP_LIGHTORANGE_PASTEL95_CTR;
    case COLOUR_MAP_LIGHTORANGE_PASTEL90_ID:   return D_COLOUR_MAP_LIGHTORANGE_PASTEL90_CTR;
    case COLOUR_MAP_LIGHTORANGE_PASTEL80_ID:   return D_COLOUR_MAP_LIGHTORANGE_PASTEL80_CTR;
    case COLOUR_MAP_LIGHTORANGE_PASTEL70_ID:   return D_COLOUR_MAP_LIGHTORANGE_PASTEL70_CTR;
    case COLOUR_MAP_LIGHTORANGE_PASTEL60_ID:   return D_COLOUR_MAP_LIGHTORANGE_PASTEL60_CTR;
    case COLOUR_MAP_LIGHTORANGE_PASTEL50_ID:   return D_COLOUR_MAP_LIGHTORANGE_PASTEL50_CTR;
    case COLOUR_MAP_LIGHTORANGE_PASTEL40_ID:   return D_COLOUR_MAP_LIGHTORANGE_PASTEL40_CTR;
    // White
    case COLOUR_MAP_WHITE_ID: return D_COLOUR_MAP_WHITE_CTR;
    case COLOUR_MAP_WHITE_PASTEL95_ID: return D_COLOUR_MAP_WHITE_PASTEL95_CTR;
    case COLOUR_MAP_WHITE_PASTEL90_ID: return D_COLOUR_MAP_WHITE_PASTEL90_CTR;
    case COLOUR_MAP_WHITE_PASTEL80_ID: return D_COLOUR_MAP_WHITE_PASTEL80_CTR;
    case COLOUR_MAP_WHITE_PASTEL70_ID: return D_COLOUR_MAP_WHITE_PASTEL70_CTR;
    case COLOUR_MAP_WHITE_PASTEL60_ID: return D_COLOUR_MAP_WHITE_PASTEL60_CTR;
    case COLOUR_MAP_WHITE_PASTEL50_ID: return D_COLOUR_MAP_WHITE_PASTEL50_CTR;
    case COLOUR_MAP_WHITE_PASTEL40_ID: return D_COLOUR_MAP_WHITE_PASTEL40_CTR;
    case COLOUR_MAP_WHITE_PASTEL20_ID: return D_COLOUR_MAP_WHITE_PASTEL20_CTR;
    // Yellow
    case COLOUR_MAP_YELLOW_ID:        return D_COLOUR_MAP_YELLOW_CTR;
    case COLOUR_MAP_YELLOW_PASTEL95_ID:        return D_COLOUR_MAP_YELLOW_PASTEL95_CTR;
    case COLOUR_MAP_YELLOW_PASTEL90_ID:        return D_COLOUR_MAP_YELLOW_PASTEL90_CTR;
    case COLOUR_MAP_YELLOW_PASTEL80_ID:        return D_COLOUR_MAP_YELLOW_PASTEL80_CTR;
    case COLOUR_MAP_YELLOW_PASTEL70_ID:        return D_COLOUR_MAP_YELLOW_PASTEL70_CTR;
    case COLOUR_MAP_YELLOW_PASTEL60_ID:        return D_COLOUR_MAP_YELLOW_PASTEL60_CTR;
    case COLOUR_MAP_YELLOW_PASTEL50_ID:        return D_COLOUR_MAP_YELLOW_PASTEL50_CTR;
    case COLOUR_MAP_YELLOW_PASTEL40_ID:        return D_COLOUR_MAP_YELLOW_PASTEL40_CTR;
    // Lime Green
    case COLOUR_MAP_LIMEGREEN_ID:     return D_COLOUR_MAP_LIMEGREEN_CTR;
    case COLOUR_MAP_LIMEGREEN_PASTEL95_ID:     return D_COLOUR_MAP_LIMEGREEN_PASTEL95_CTR;
    case COLOUR_MAP_LIMEGREEN_PASTEL90_ID:     return D_COLOUR_MAP_LIMEGREEN_PASTEL90_CTR;
    case COLOUR_MAP_LIMEGREEN_PASTEL80_ID:     return D_COLOUR_MAP_LIMEGREEN_PASTEL80_CTR;
    case COLOUR_MAP_LIMEGREEN_PASTEL70_ID:     return D_COLOUR_MAP_LIMEGREEN_PASTEL70_CTR;
    case COLOUR_MAP_LIMEGREEN_PASTEL60_ID:     return D_COLOUR_MAP_LIMEGREEN_PASTEL60_CTR;
    case COLOUR_MAP_LIMEGREEN_PASTEL50_ID:     return D_COLOUR_MAP_LIMEGREEN_PASTEL50_CTR;
    case COLOUR_MAP_LIMEGREEN_PASTEL40_ID:     return D_COLOUR_MAP_LIMEGREEN_PASTEL40_CTR;
    // Green
    case COLOUR_MAP_GREEN_ID:         return D_COLOUR_MAP_GREEN_CTR;
    case COLOUR_MAP_GREEN_PASTEL95_ID:         return D_COLOUR_MAP_GREEN_PASTEL95_CTR;
    case COLOUR_MAP_GREEN_PASTEL90_ID:         return D_COLOUR_MAP_GREEN_PASTEL90_CTR;
    case COLOUR_MAP_GREEN_PASTEL80_ID:         return D_COLOUR_MAP_GREEN_PASTEL80_CTR;
    case COLOUR_MAP_GREEN_PASTEL70_ID:         return D_COLOUR_MAP_GREEN_PASTEL70_CTR;
    case COLOUR_MAP_GREEN_PASTEL60_ID:         return D_COLOUR_MAP_GREEN_PASTEL60_CTR;
    case COLOUR_MAP_GREEN_PASTEL50_ID:         return D_COLOUR_MAP_GREEN_PASTEL50_CTR;
    case COLOUR_MAP_GREEN_PASTEL40_ID:         return D_COLOUR_MAP_GREEN_PASTEL40_CTR;
    // Cyan
    case COLOUR_MAP_CYAN_ID:          return D_COLOUR_MAP_CYAN_CTR;
    case COLOUR_MAP_CYAN_PASTEL95_ID:          return D_COLOUR_MAP_CYAN_PASTEL95_CTR;
    case COLOUR_MAP_CYAN_PASTEL90_ID:          return D_COLOUR_MAP_CYAN_PASTEL90_CTR;
    case COLOUR_MAP_CYAN_PASTEL80_ID:          return D_COLOUR_MAP_CYAN_PASTEL80_CTR;
    case COLOUR_MAP_CYAN_PASTEL70_ID:          return D_COLOUR_MAP_CYAN_PASTEL70_CTR;
    case COLOUR_MAP_CYAN_PASTEL60_ID:          return D_COLOUR_MAP_CYAN_PASTEL60_CTR;
    case COLOUR_MAP_CYAN_PASTEL50_ID:          return D_COLOUR_MAP_CYAN_PASTEL50_CTR;
    case COLOUR_MAP_CYAN_PASTEL40_ID:          return D_COLOUR_MAP_CYAN_PASTEL40_CTR;
    // Blue
    case COLOUR_MAP_BLUE_ID:          return D_COLOUR_MAP_BLUE_CTR;
    case COLOUR_MAP_BLUE_PASTEL95_ID:          return D_COLOUR_MAP_BLUE_PASTEL95_CTR;
    case COLOUR_MAP_BLUE_PASTEL90_ID:          return D_COLOUR_MAP_BLUE_PASTEL90_CTR;
    case COLOUR_MAP_BLUE_PASTEL80_ID:          return D_COLOUR_MAP_BLUE_PASTEL80_CTR;
    case COLOUR_MAP_BLUE_PASTEL70_ID:          return D_COLOUR_MAP_BLUE_PASTEL70_CTR;
    case COLOUR_MAP_BLUE_PASTEL60_ID:          return D_COLOUR_MAP_BLUE_PASTEL60_CTR;
    case COLOUR_MAP_BLUE_PASTEL50_ID:          return D_COLOUR_MAP_BLUE_PASTEL50_CTR;
    case COLOUR_MAP_BLUE_PASTEL40_ID:          return D_COLOUR_MAP_BLUE_PASTEL40_CTR;
    // Blue Purple
    case COLOUR_MAP_BLUEPURPLE_ID:    return D_COLOUR_MAP_BLUEPURPLE_CTR;
    case COLOUR_MAP_BLUEPURPLE_PASTEL95_ID:    return D_COLOUR_MAP_BLUEPURPLE_PASTEL95_CTR;
    case COLOUR_MAP_BLUEPURPLE_PASTEL90_ID:    return D_COLOUR_MAP_BLUEPURPLE_PASTEL90_CTR;
    case COLOUR_MAP_BLUEPURPLE_PASTEL80_ID:    return D_COLOUR_MAP_BLUEPURPLE_PASTEL80_CTR;
    case COLOUR_MAP_BLUEPURPLE_PASTEL70_ID:    return D_COLOUR_MAP_BLUEPURPLE_PASTEL70_CTR;
    case COLOUR_MAP_BLUEPURPLE_PASTEL60_ID:    return D_COLOUR_MAP_BLUEPURPLE_PASTEL60_CTR;
    case COLOUR_MAP_BLUEPURPLE_PASTEL50_ID:    return D_COLOUR_MAP_BLUEPURPLE_PASTEL50_CTR;
    case COLOUR_MAP_BLUEPURPLE_PASTEL40_ID:    return D_COLOUR_MAP_BLUEPURPLE_PASTEL40_CTR;
    // Purple
    case COLOUR_MAP_PURPLE_ID:        return D_COLOUR_MAP_PURPLE_CTR;
    case COLOUR_MAP_PURPLE_PASTEL95_ID:        return D_COLOUR_MAP_PURPLE_PASTEL95_CTR;
    case COLOUR_MAP_PURPLE_PASTEL90_ID:        return D_COLOUR_MAP_PURPLE_PASTEL90_CTR;
    case COLOUR_MAP_PURPLE_PASTEL80_ID:        return D_COLOUR_MAP_PURPLE_PASTEL80_CTR;
    case COLOUR_MAP_PURPLE_PASTEL70_ID:        return D_COLOUR_MAP_PURPLE_PASTEL70_CTR;
    case COLOUR_MAP_PURPLE_PASTEL60_ID:        return D_COLOUR_MAP_PURPLE_PASTEL60_CTR;
    case COLOUR_MAP_PURPLE_PASTEL50_ID:        return D_COLOUR_MAP_PURPLE_PASTEL50_CTR;
    case COLOUR_MAP_PURPLE_PASTEL40_ID:        return D_COLOUR_MAP_PURPLE_PASTEL40_CTR;
    // Pink
    case COLOUR_MAP_PINK_ID:          return D_COLOUR_MAP_PINK_CTR;
    case COLOUR_MAP_PINK_PASTEL95_ID:          return D_COLOUR_MAP_PINK_PASTEL95_CTR;
    case COLOUR_MAP_PINK_PASTEL90_ID:          return D_COLOUR_MAP_PINK_PASTEL90_CTR;
    case COLOUR_MAP_PINK_PASTEL80_ID:          return D_COLOUR_MAP_PINK_PASTEL80_CTR;
    case COLOUR_MAP_PINK_PASTEL70_ID:          return D_COLOUR_MAP_PINK_PASTEL70_CTR;
    case COLOUR_MAP_PINK_PASTEL60_ID:          return D_COLOUR_MAP_PINK_PASTEL60_CTR;
    case COLOUR_MAP_PINK_PASTEL50_ID:          return D_COLOUR_MAP_PINK_PASTEL50_CTR;
    case COLOUR_MAP_PINK_PASTEL40_ID:          return D_COLOUR_MAP_PINK_PASTEL40_CTR;
    // Hotpink
    case COLOUR_MAP_HOTPINK_ID:       return D_COLOUR_MAP_HOTPINK_CTR;
    case COLOUR_MAP_HOTPINK_PASTEL95_ID:       return D_COLOUR_MAP_HOTPINK_PASTEL95_CTR;
    case COLOUR_MAP_HOTPINK_PASTEL90_ID:       return D_COLOUR_MAP_HOTPINK_PASTEL90_CTR;
    case COLOUR_MAP_HOTPINK_PASTEL80_ID:       return D_COLOUR_MAP_HOTPINK_PASTEL80_CTR;
    case COLOUR_MAP_HOTPINK_PASTEL70_ID:       return D_COLOUR_MAP_HOTPINK_PASTEL70_CTR;
    case COLOUR_MAP_HOTPINK_PASTEL60_ID:       return D_COLOUR_MAP_HOTPINK_PASTEL60_CTR;
    case COLOUR_MAP_HOTPINK_PASTEL50_ID:       return D_COLOUR_MAP_HOTPINK_PASTEL50_CTR;
    case COLOUR_MAP_HOTPINK_PASTEL40_ID:       return D_COLOUR_MAP_HOTPINK_PASTEL40_CTR;
    // None ie not set/used
    case COLOUR_MAP_NONE_ID:       return D_COLOUR_MAP_NONE_CTR;
    case COLOUR_MAP_PALETTE_OPTIONS_END_ID: return D_COLOUR_MAP_PALETTE_OPTIONS_END_CTR;
  }
  return WARNING_NOTHANDLED_CTR;
  // return "Unset";
}





const char* mRGBAnimator::GetTransitionOrderName(char* buffer){
  return GetTransitionOrderNameByID(animation.transition.order_id, buffer);
}
const char* mRGBAnimator::GetTransitionOrderNameByID(uint8_t id, char* buffer){
  switch(id){  default:    
    case TRANSITION_ORDER_RANDOM_ID:     memcpy_P(buffer, PM_TRANSITION_ORDER_RANDOM_NAME_CTR, sizeof(PM_TRANSITION_ORDER_RANDOM_NAME_CTR)); break;// smooth shift between them
    case TRANSITION_ORDER_INORDER_ID:    memcpy_P(buffer, PM_TRANSITION_ORDER_INORDER_NAME_CTR, sizeof(PM_TRANSITION_ORDER_INORDER_NAME_CTR)); break;// instant shift
    case TRANSITION_ORDER_CENTRE_OUT_ID: memcpy_P(buffer, PM_TRANSITION_ORDER_CENTRE_OUT_NAME_CTR, sizeof(PM_TRANSITION_ORDER_CENTRE_OUT_NAME_CTR)); break;
    case TRANSITION_ORDER_ROTATE_ID:     memcpy_P(buffer, PM_TRANSITION_ORDER_ROTATE_NAME_CTR, sizeof(PM_TRANSITION_ORDER_ROTATE_NAME_CTR)); break;
    // case TRANSITION_ORDER_FIXED_ID:     memcpy_P(buffer, PM_TRANSITION_ORDER_FIXED_NAME_CTR, sizeof(PM_TRANSITION_ORDER_FIXED_NAME_CTR)); break;// blend shift with random twinkles on random number of leds
    // case TRANSITION_ORDER_NONE_ID:      memcpy_P(buffer, PM_TRANSITION_ORDER_NONE_NAME_CTR, sizeof(PM_TRANSITION_ORDER_NONE_NAME_CTR)); break; // blend shift with random twinkles on random number of leds
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

  animation.flags.fForceUpdate = true;
  animation_override.fRefreshAllPixels = true;
  animation_override.time_ms = 10;

  // Additional flags to force scene updates
  if(animation.mode_id == ANIMATION_MODE_SCENE_ID){
    scene.name_id = SCENES_COLOURSCENE_ID;
  }

}



void mRGBAnimator::EveryLoop(){

    //something smarter for delays
    // pCONT_set->Settings.enable_sleep = (animation.transition.rate_ms.val<pCONT_set->Settings.sleep) ? 0 : 1;

    if(animation.flags.fEnable_Animation){
    // while(blocking_force_animate_to_complete){
      if (animations_control->IsAnimating()){ //Serial.print("~a"); // the normal lop just needs these two to run the active animations_fadeall
        animations_control->UpdateAnimations();
        StripUpdate();
        if(!animation.flags.fRunning){   
          #ifdef ENABLE_LOG_LEVEL_DEBUG
          AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "Animation Started"));
          #endif
        }
        animation.flags.fRunning = true; 
        fPixelsUpdated = true;
        pCONT_set->Settings.enable_sleep = false;    //Make this a function, pause sleep during animations
        //AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "~a"));
      }else{
        if(animation.flags.fRunning){ 
          #ifdef ENABLE_LOG_LEVEL_DEBUG
          AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "Animation Finished")); 
          #endif
          fAnyLEDsOnOffCount = 0;
          for(int i=0;i<strip_size;i++){ 
            if(GetPixelColor(i)!=0){ fAnyLEDsOnOffCount++; }
          }          
        }
        animation.flags.fRunning = false;
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
    }//animation.flags.fEnable_Animation


    //Auto turn off
    if(mSupport::TimeReached(&animation.tSavedAutoOff,1000)){
      //AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "scene.tSavedAutoOff [%d]"),animation.auto_time_off_secs);
      if(animation.auto_time_off_secs==1){ //if =1 then turn off and clear to 0
        // animation.name_id = SCENES_FADE_OFF_ID;
        #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "animation.auto_time_off_secs==1 and disable"));
        #endif       

        SetAnimationProfile(ANIMATION_PROFILE_TURN_OFF_ID);
        //#ifdef ENABLE_LOG_LEVEL_INFO
        animation.auto_time_off_secs=0;
        //#endif
      }else
      if(animation.auto_time_off_secs>1){ //if =1 then turn off and clear to 0
        animation.auto_time_off_secs--; //decrease
        
        #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "animation.auto_time_off_secs=%d dec"),animation.auto_time_off_secs);
        #endif
      }
    }


    //SubTask_TimedLEDMode(); // Delayed commands (should really be generic ie not limited to this class)

    switch(animation.mode_id){
      //#ifdef USE_TASK_RGBLIGHTING_FLASHER_AND_MIXER
      case ANIMATION_MODE_FLASHER_ID:
        SubTask_Flasher_Main();
        fLEDStripOnOff = true;
      break;
      #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
      case ANIMATION_MODE_NOTIFICATIONS_ID:
        SubTask_NotificationPanel();
        fLEDStripOnOff = true;
      break;
      #endif
      case ANIMATION_MODE_SCENE_ID:
        SubTask_Scenes();             //scenes to be moved into interface
        fLEDStripOnOff = true;
      break;
      case ANIMATION_MODE_AMBILIGHT_ID:
        SubTask_Ambilight();
        fLEDStripOnOff = true;
      break;
      case ANIMATION_MODE_NONE_ID: default: break; // resting position
    }


    scene.colour.B = animation.brightness; // animation.brightness is master

    // control relay power
    //(fLEDStripOnOff)
      if(fLEDStripOnOff!=fLEDStripOnOff_Saved){
        fLEDStripOnOff_Saved = fLEDStripOnOff;
        // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "fLEDStripOnOff_Saved"));
        //pCONT->mry->SetRelay(0,fLEDStripOnOff);
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

uint16_t mRGBAnimator::changeUIntScale(uint16_t inum, uint16_t ifrom_min, uint16_t ifrom_max,
                                       uint16_t ito_min, uint16_t ito_max) {
  // guard-rails
  if (ifrom_min >= ifrom_max) {
    if (ito_min > ito_max) {
      return ito_max;
    } else {
      return ito_min;  // invalid input, return arbitrary value
    }
  }
  // convert to uint31, it's more verbose but code is more compact
  uint32_t num = inum;
  uint32_t from_min = ifrom_min;
  uint32_t from_max = ifrom_max;
  uint32_t to_min = ito_min;
  uint32_t to_max = ito_max;

  // check source range
  num = (num > from_max ? from_max : (num < from_min ? from_min : num));

  // check to_* order
  if (to_min > to_max) {
    // reverse order
    num = (from_max - num) + from_min;
    to_min = ito_max;
    to_max = ito_min;
  }

  uint32_t numerator = (num - from_min) * (to_max - to_min);
  uint32_t result;
  if (numerator >= 0x80000000L) {
    // don't do rounding as it would create an overflow
    result = numerator / (from_max - from_min) + to_min;
  } else {
    result = (((numerator * 2) / (from_max - from_min)) + 1) / 2 + to_min;
  }
  return (uint32_t) (result > to_max ? to_max : (result < to_min ? to_min : result));
}







  









/*******************************************************************************************************************
********************************************************************************************************************
************MQTT**************************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/


// CheckAndExecute_JSONCommands


int8_t mRGBAnimator::CheckAndExecute_JSONCommands(JsonObjectConst obj){

  // Check if instruction is for me
  if(mSupport::mSearchCtrIndexOf(data_buffer2.topic.ctr,"set/pixels")>=0){
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_PIXELS));
      pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
      parsesub_JSONCommand(obj);
      return FUNCTION_RESULT_HANDLED_ID;
  }else{
    return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
  }

}

int8_t mRGBAnimator::parsesub_JSONCommand(JsonObjectConst obj){

  // // Check if instruction is for me
  // if(mSupport::mSearchCtrIndexOf(data_buffer2.topic.ctr,"set/pixels")>=0){
  //   #ifdef ENABLE_LOG_LEVEL_INFO
  //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_PIXELS));
  //   #endif
  //   pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
  // }else{
  //   return 0; // not meant for here
  // }
  int8_t isserviced = 0;
  
  // DynamicJsonDocument doc(1500);
  // DeserializationError error = deserializeJson(doc, data_buffer2.payload.ctr);
  // JsonObject obj = doc.as<JsonObject>();

  // StaticJsonDocument<1500> doc;
  // DeserializationError error = deserializeJson(doc, data_buffer2.payload.ctr);
  // if(error){
  //   AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_ERROR_JSON_DESERIALIZATION));
  //   Response_mP(S_JSON_COMMAND_SVALUE, D_ERROR,D_ERROR_JSON_DESERIALIZATION);
  //   return 0;
  // }
  // JsonObject obj = doc.as<JsonObject>();

  //new topic names must include pixels
  
  #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
    if(strstr(data_buffer2.topic.ctr,"/notif")){
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "notif"));    
    #endif
      isserviced += parsesub_NotificationPanel(obj);
    }else 
  #endif
  if(mSupport::memsearch(data_buffer2.topic.ctr,data_buffer2.topic.len,"/manual",sizeof("/manual")-1)>=0){
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "manual"));    
    #endif
    isserviced += parsesub_ModeManual(obj);
  }else 
  if(mSupport::memsearch(data_buffer2.topic.ctr,data_buffer2.topic.len,"/scene",sizeof("/scene")-1)>=0){
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "scene"));    
    #endif
    isserviced += parsesub_ModeScene(obj);
  }else  
  if(mSupport::memsearch(data_buffer2.topic.ctr,data_buffer2.topic.len,"/animation",sizeof("/animation")-1)>=0){
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "animation"));
    #endif    
    isserviced += parsesub_ModeAnimation(obj);
  }else
  if(mSupport::memsearch(data_buffer2.topic.ctr,data_buffer2.topic.len,"/ambilight",sizeof("/ambilight")-1)>=0){
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "ambilight")); 
    #endif   
    isserviced += parsesub_ModeAmbilight(obj);
  }else
  if(mSupport::memsearch(data_buffer2.topic.ctr,data_buffer2.topic.len,"/flasher",sizeof("/flasher")-1)>=0){
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "flasher"));    
    #endif
    isserviced += parsesub_Flasher(obj);
  }else
  // if(mSupport::memsearch(data_buffer2.topic.ctr,data_buffer2.topic.len,"/hardware",sizeof("/hardware")-1)>=0){
  //   
    // #ifdef ENABLE_LOG_LEVEL_INFO
    // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "flasher"));    
  //   isserviced += parsesub_Flasher(obj);
  // }else
  {
    AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "INVALID"));    
  }

  animation.flags.fForceUpdate = true;
  data_buffer2.isserviced += isserviced;

  // t_mqtthandler_status_animation.fSendNow = true;

  return isserviced;

}

int8_t mRGBAnimator::parsesub_CheckAll(JsonObjectConst obj){
//   Serial.println("DISABLED");
// return 0;
    // Serial.println("parsesub_CheckAll1"); Serial.flush();
  parsesub_ModeManual(obj);
    // Serial.println("parsesub_CheckAll2"); Serial.flush();
  parsesub_ModeAmbilight(obj);
    // Serial.println("parsesub_CheckAll3"); Serial.flush();
  parsesub_ModeAnimation(obj);
    // Serial.println("parsesub_CheckAll4"); Serial.flush();
  parsesub_ModeScene(obj);
    // Serial.println("parsesub_CheckAll5"); Serial.flush();
  parsesub_Flasher(obj);
    // Serial.println("parsesub_CheckAll6"); Serial.flush();

}



// Directly change and control flags/elements without changing everything ie update brightness without changing to scene
int8_t mRGBAnimator::parsesub_ModeManual(JsonObjectConst obj){

  int8_t isserviced = -1;

   // Create local dereferenced variable
  // JsonObject obj = (*_obj); 

  #ifdef USE_JSON_TO_FLASH_MEMORY_TEST
  if(!obj["external_power_onoff"].isNull()){ 
    const char* onoff = obj[D_JSON_ONOFF];
    if(strstr(onoff,"ON")){ 
      #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED "\"external_power_onoff\"=\"ON\""));
      #endif
      isserviced++;
    }else 
    if(strstr(onoff,"OFF")){
      #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED "\"external_power_onoff\"=\"OFF\""));
      #endif
      isserviced++;
    }else{
      AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_NOMATCH));
    }
  }

  DEBUG_LINE;

  //external_power_mode = external_power.mode = manual, auto (turn off when off and animation finished)

  if(!obj[D_JSON_ONOFF].isNull()){ 
    const char* onoff = obj[D_JSON_ONOFF];
    if(strstr(onoff,"ON")){ 
      
      #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED "\"onoff\"=\"ON\""));
      #endif
      
      #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "MODE_TURN_ON_ID"));
      #endif
      // Add this as "SAVE" state then "LOAD" state
      memcpy(&animation,&animation_stored,sizeof(animation));// RESTORE copy of state

      SetAnimationProfile(ANIMATION_PROFILE_TURN_ON_ID);
      fLEDStripOnOff = true;

      //animation.mode_id = MODE_TURN_ON_ID;
      isserviced++;
    }else if(strstr(onoff,"OFF")){
      #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED "\"onoff\"=\"OFF\""));
      #endif
      #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "MODE_TURN_OFF_ID"));
      #endif
      memcpy(&animation_stored,&animation,sizeof(animation)); // STORE copy of state
      SetAnimationProfile(ANIMATION_PROFILE_TURN_OFF_ID);
      fLEDStripOnOff = false;

      //animation.mode_id = MODE_TURN_OFF_ID;
      isserviced++;
    }else{
      AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_NOMATCH));
    }
  }

  DEBUG_LINE;

  // if(obj.containsKey("hue")){
  //   int hue = obj["hue"];
  //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED "\"hue\"=[%d]"),hue);
  //   scene.colour.H = HueN2F(hue);
  //   isserviced++;
  // }
  // if(obj.containsKey("sat")){
  //   int sat = obj["sat"];
  //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED "\"sat\"=[%d]"),sat);
  //   scene.colour.S = SatN2F(sat);
  //   isserviced++;
  // }
  // if(obj.containsKey("brt")){
  //   int brt = obj["brt"];
  //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED "\"brt\"=[%d]"),brt);
  //   scene.colour.B = BrtN2F(brt);
  //   isserviced++;
  // }


  
  // TIME on duration for autooff
  if(!obj[D_JSON_TIME_ON].isNull()){ //default to secs
    animation.auto_time_off_secs = obj[D_JSON_TIME_ON];
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),animation.auto_time_off_secs);  
    #endif
  }else
  if(!obj[D_JSON_TIME_ON_SECS].isNull()){
    animation.auto_time_off_secs = obj[D_JSON_TIME_ON_SECS];
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),animation.auto_time_off_secs);  
    #endif
  }else
  if(!obj[D_JSON_TIME_ON_MS].isNull()){
    animation.auto_time_off_secs = obj[D_JSON_TIME_ON_MS];
    animation.auto_time_off_secs /= 1000;
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),animation.auto_time_off_secs);  
    #endif
  }

#endif

  DEBUG_LINE;


  return isserviced;

} // END FUNCTION

//,(animation_override.fRefreshAllPixels?"Set":"UNSET")
int8_t mRGBAnimator::parsesub_ModeAnimation(JsonObjectConst obj){


  char buffer[40];

  uint8_t isserviced = 0;
  int8_t tmp_id = 0;


DEBUG_LINE;

  #ifdef USE_JSON_TO_FLASH_MEMORY_TEST
DEBUG_LINE;
  if(obj.containsKey(D_JSON_COLOUR_PALETTE)){ 
DEBUG_LINE;
    const char* colour = obj[D_JSON_COLOUR_PALETTE];
DEBUG_LINE;
    if((tmp_id=pCONT_iLight->GetPaletteIDbyName(colour))>=0){
      animation.palette_id = tmp_id;
DEBUG_LINE;
      #ifdef USE_TASK_RGBLIGHTING_FLASHER_AND_MIXER
      if(animation.mode_id == ANIMATION_MODE_FLASHER_ID){
        flashersettings.region = FLASHER_REGION_COLOUR_SELECT_ID; //update colours in use
      }
      #endif
      #ifdef ENABLE_PALETTE_FORCED_MODE
      //  animation.mode_id = ANIMATION_MODE_PRESETS_ID;
      #endif
      // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_COLOUR_PALETTE,GetPaletteFriendlyName());
      if(animation_override.fRefreshAllPixels){
        first_set = true; //refresh all
      }
      isserviced++;
    }else{
DEBUG_LINE;
      AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_NOMATCH D_JSON_COMMAND_SVALUE),D_JSON_COLOUR_PALETTE,colour);
    }
  }

DEBUG_LINE;


    //{"colour_selector":{"red":"on"}}
    //{"colour_selector":{"all":"on"}} 
    //{"colour_selector":"reset"} //all off
    //{"colour_selector":"all"} //all off
    //what about colour order? on/off or number for index?
  
      // int randnum = random(0,selectorlist.amount); 
      // hsbcolour[ledsindex[ledout.index]].H = selectorlist.colour[randnum].H;
      
  // if(!obj["colour_selector"].isNull()){ 
  //   const char* onoff = obj["colour_selector"];

  // // if(!obj[D_JSON_ONOFF].isNull()){ 
  // //   const char* onoff = obj[D_JSON_ONOFF];
  // //   if(strstr(onoff,"ON")){ 
  // //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED "\"onoff\"=\"ON\""));

  // }
    
  
  if(!obj[D_JSON_MODE].isNull()){ 
    const char* mode = obj[D_JSON_MODE];
    if((tmp_id=GetAnimationModeIDbyName(mode))>=0){
      animation.mode_id = tmp_id;
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_MODE,GetAnimationModeName(buffer));
     #endif
      // Response_mP(S_JSON_COMMAND_SVALUE,D_JSON_MODE,GetAnimationModeName());
      isserviced++;
    }else{
      AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_MODE,mode);
    }
  }


  if(!obj[D_JSON_RGB_COLOUR_ORDER].isNull()){ 
    const char* mode = obj[D_JSON_RGB_COLOUR_ORDER];
    if((tmp_id=GetHardwareColourTypeIDbyName(mode))>=0){
      settings.pixel_hardware_color_order_id = tmp_id;
      #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_RGB_COLOUR_ORDER,GetHardwareColourTypeName(buffer));
      #endif
      // Response_mP(S_JSON_COMMAND_SVALUE,D_JSON_MODE,GetAnimationModeName());
      isserviced++;
    }else{
      AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_MODE,mode);
    }
  }


  if(!obj[D_JSON_STRIP_SIZE].isNull()){ 
    int amount = obj[D_JSON_STRIP_SIZE];
    // Also convert to percentage equivalent
    strip_size = amount;
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_STRIP_SIZE,strip_size);
    #endif
    // Response_mP(S_JSON_COMMAND_SVALUE_NVALUE,D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE,animation.transition.pixels_to_update_as_percentage.val);
    isserviced++;
  }
  
  
DEBUG_LINE;

  // struct TRANSITIONSETTINGS

  if(!obj[D_JSON_TRANSITION][D_JSON_PIXELS_UPDATE_NUMBER].isNull()){ 
    int amount = obj[D_JSON_TRANSITION][D_JSON_PIXELS_UPDATE_NUMBER];
    // Also convert to percentage equivalent
    animation.transition.pixels_to_update_as_percentage.val = GetPixelsToUpdateAsPercentageFromNumber(amount);
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE,animation.transition.pixels_to_update_as_percentage.val);
    #endif
    // Response_mP(S_JSON_COMMAND_SVALUE_NVALUE,D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE,animation.transition.pixels_to_update_as_percentage.val);
    isserviced++;
  }else
  if(!obj[D_JSON_TRANSITION][D_JSON_PIXELS_UPDATE_PERCENTAGE].isNull()){ 
    int percentage = obj[D_JSON_TRANSITION][D_JSON_PIXELS_UPDATE_PERCENTAGE];
    animation.transition.pixels_to_update_as_percentage.val = constrain(percentage,0,100);
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE,animation.transition.pixels_to_update_as_percentage.val);
    #endif
    Response_mP(S_JSON_COMMAND_SVALUE_NVALUE,D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE,animation.transition.pixels_to_update_as_percentage.val);
    // Also convert to number equivalent
    isserviced++;
  }

DEBUG_LINE;

  
  // Transition time in seconds or ms
  if(!obj[D_JSON_TRANSITION][D_JSON_TIME].isNull()){ 
    animation.transition.time_ms.val = obj[D_JSON_TRANSITION][D_JSON_TIME];
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_TIME,animation.transition.time_ms.val);   
    #endif
    animation.transition.time_ms.val *= 1000; // map into ms
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_TIME,animation.transition.time_ms.val);
    #endif
    Response_mP(S_JSON_COMMAND_NVALUE, D_JSON_TIME_MS,animation.transition.time_ms.val);
    isserviced++;
    //check that rate>animation time
    if(animation.transition.rate_ms.val<animation.transition.time_ms.val){ animation.transition.time_ms.val = animation.transition.rate_ms.val;}
  }else
  if(!obj[D_JSON_TRANSITION][D_JSON_TIME_MS].isNull()){ 
    animation.transition.time_ms.val = obj[D_JSON_TRANSITION][D_JSON_TIME_MS];
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_TIME_MS,animation.transition.time_ms.val);
    #endif
    Response_mP(S_JSON_COMMAND_NVALUE, D_JSON_TIME_MS,animation.transition.time_ms.val);
    isserviced++;
    //check that rate>animation time
    if(animation.transition.rate_ms.val<animation.transition.time_ms.val){ animation.transition.time_ms.val = animation.transition.rate_ms.val;}
  }

DEBUG_LINE;

  // Rate in seconds as default or ms
  if(!obj[D_JSON_TRANSITION][D_JSON_RATE].isNull()){ 
    animation.transition.rate_ms.val = obj[D_JSON_TRANSITION][D_JSON_RATE];
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_RATE,animation.transition.time_ms.val);
    #endif
    animation.transition.rate_ms.val *= 1000; //seconds to milliseconds
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_RATE,animation.transition.time_ms.val);
    Response_mP(S_JSON_COMMAND_NVALUE, D_JSON_TIME_MS,animation.transition.rate_ms.val);
    isserviced++;
    //check that rate>animation time
    if(animation.transition.rate_ms.val<animation.transition.time_ms.val){ animation.transition.time_ms.val = animation.transition.rate_ms.val;}
  }else
  if(!obj[D_JSON_TRANSITION][D_JSON_RATE_MS].isNull()){ 
    animation.transition.rate_ms.val = obj[D_JSON_TRANSITION][D_JSON_RATE_MS];
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_RATE_MS,animation.transition.rate_ms.val);
    #endif
    Response_mP(S_JSON_COMMAND_NVALUE, D_JSON_TIME_MS,animation.transition.rate_ms.val);
    isserviced++;
    //check that rate>animation time
    if(animation.transition.rate_ms.val<animation.transition.time_ms.val){ animation.transition.time_ms.val = animation.transition.rate_ms.val;}
  }

  // probably bring transition_order into one command
  if(!obj[D_JSON_TRANSITION][D_JSON_ORDER].isNull()){ 
    const char* order = obj[D_JSON_TRANSITION][D_JSON_ORDER];
    if((tmp_id=GetTransitionOrderIDbyName(order))>=0){
      animation.transition.order_id = tmp_id;
      // AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_SVALUE),D_JSON_TRANSITION,D_JSON_ORDER,GetTransitionOrderName());
      // Response_mP(S_JSON_COMMAND_SVALUE,D_JSON_ORDER,GetTransitionOrderName());
      isserviced++;
    }else{
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_NOMATCH D_JSON_COMMAND_SVALUE_SVALUE),D_JSON_TRANSITION,D_JSON_ORDER,order);
    #endif
    }
  }

  if(!obj[D_JSON_BRIGHTNESS].isNull()){
    uint16_t brt = obj[D_JSON_BRIGHTNESS];
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_BRIGHTNESS,brt);
    #endif
    animation.brightness = BrtN2F(brt);
    // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_BRIGHTNESS,animation.brightness);
    Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_BRIGHTNESS,animation.brightness);
    isserviced++;
  }

  if(!obj[D_JSON_PIXELSGROUPED].isNull()){
    animation.pixelgrouped = obj[D_JSON_PIXELSGROUPED];
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_PIXELSGROUPED,animation.pixelgrouped);
    #endif
  }

  //override commands that run for one animation cycle then are cleared to 0
  if(!obj[D_JSON_REFRESH_ALLPIXELS].isNull()){
    animation_override.fRefreshAllPixels = obj[D_JSON_REFRESH_ALLPIXELS];
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_REFRESH_ALLPIXELS,animation_override.fRefreshAllPixels);
    #endif
  }

  
  
  // TIME on duration for autooff
  if(!obj[D_JSON_TIME_ON].isNull()){ //default to secs
    animation.auto_time_off_secs = obj[D_JSON_TIME_ON];
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),animation.auto_time_off_secs);  
    #endif
  }else
  if(!obj[D_JSON_TIME_ON_SECS].isNull()){
    animation.auto_time_off_secs = obj[D_JSON_TIME_ON_SECS];
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),animation.auto_time_off_secs);  
    #endif
  }else
  if(!obj[D_JSON_TIME_ON_MS].isNull()){
    animation.auto_time_off_secs = obj[D_JSON_TIME_ON_MS];
    animation.auto_time_off_secs /= 1000;
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),animation.auto_time_off_secs);  
    #endif
  }

  animation_override.time_ms = 1000; // implement in 1 second 


#endif

DEBUG_LINE;

  return isserviced;

} // END FUNCTION



// Final output, but check for power limit
void mRGBAnimator::StripUpdate(){

  // STRIP_SIZE_REPEAT_MAX

  // if(settings.strip_size_repeat_animation)
  
  // Replicate SetPixel for repeated output
  int pixels_existing_index = 0;
  for(int i=0;i<STRIP_REPEAT_OUTPUT_MAX;i++){
    SetPixelColor(i,GetPixelColor(pixels_existing_index++));
    if(pixels_existing_index>=STRIP_SIZE_MAX){ pixels_existing_index = 0;}
  }
  

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
  //   // colour_tmp = hsbcolour[ledout.index];
  //   // estimated_power_new +=  WebColorFromHsbColour(hsbcolour[ledout.index]) * channel_count * 20;
  //   c = hsbcolour[ledout.index];
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
    // colour_tmp = hsbcolour[ledout.index];
    // estimated_power_new +=  WebColorFromHsbColour(hsbcolour[ledout.index]) * channel_count * 20;
    c = hsbcolour[ledout.index];
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

  // for (auto copies = 0; copies < 3; copies++) {
  //   image.Blt(stripbus, copies * image.PixelCount());
  // }

  stripbus->Show();

}



//previous heating panel
int8_t mRGBAnimator::parsesub_ModeAmbilight(JsonObjectConst obj){

   // Create local dereferenced variable
  // JsonObject obj = (*_obj); 

  int8_t tmp_id = 0;
  int8_t isserviced = 0;

  char buffer[50];

  if(!obj[D_JSON_NAME].isNull()){ 
    const char* scenectr = obj[D_JSON_NAME];
    if((tmp_id=GetSceneIDbyName(scenectr))>=0){
      scene.name_id = tmp_id;
      animation.mode_id = ANIMATION_MODE_SCENE_ID;
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_NAME,GetSceneName(buffer));
      Response_mP(S_JSON_COMMAND_SVALUE,D_JSON_NAME,GetSceneName(buffer));
      isserviced++;
    }else{
      AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_NAME,scenectr);
    }
  }

  // USe pointers
  //side
  // struct AMBILIGHT_SCREEN_SETTINGS.SCREENS[0]::
  // ambilightsettings.screens[SCREEN_CENTRE].top
  //screen
  
  if(!obj[F("top")][F(D_JSON_HUE)].isNull()){ 
    uint16_t hue = obj[F("top")][F(D_JSON_HUE)];
    // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_HUE,hue);
    ambilightsettings.screens[SCREEN_CENTRE].top.colour.H = HueN2F(hue);
    // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_HUE,ambilightsettings.screens[SCREEN_CENTRE].top.colour.H);
    // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_HUE,ambilightsettings.screens[SCREEN_CENTRE].top.colour.H);
    isserviced++;
  }
  if(!obj[F("top")][F(D_JSON_SAT)].isNull()){ 
    uint8_t sat = obj[F("top")][F(D_JSON_SAT)];
    // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_SAT,sat);
    ambilightsettings.screens[SCREEN_CENTRE].top.colour.S = SatN2F(sat);
    // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_SAT,ambilightsettings.screens[SCREEN_CENTRE].top.colour.S);
    // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_SAT,ambilightsettings.screens[SCREEN_CENTRE].top.colour.S);
    isserviced++;
  }
  if(!obj[F("top")][F(D_JSON_BRT)].isNull()){ 
    uint8_t brt = obj[F("top")][F(D_JSON_BRT)];
    // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_BRT,brt);
    ambilightsettings.screens[SCREEN_CENTRE].top.colour.B = animation.brightness = BrtN2F(brt);
    // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_BRT,animation.brightness);
    // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_BRT,animation.brightness);
    isserviced++;
  }



  if(!obj[F("bottom")][F(D_JSON_HUE)].isNull()){ 
    uint16_t hue = obj[F("bottom")][F(D_JSON_HUE)];
    // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_HUE,hue);
    ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.H = HueN2F(hue);
    // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_HUE,ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.H);
    // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_HUE,ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.H);
    isserviced++;
  }
  if(!obj[F("bottom")][F(D_JSON_SAT)].isNull()){ 
    uint8_t sat = obj[F("bottom")][F(D_JSON_SAT)];
    // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_SAT,sat);
    ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.S = SatN2F(sat);
    // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_SAT,ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.S);
    // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_SAT,ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.S);
    isserviced++;
  }
  if(!obj[F("bottom")][F(D_JSON_BRT)].isNull()){ 
    uint8_t brt = obj[F("bottom")][F(D_JSON_BRT)];
    // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_BRT,brt);
    ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.B = animation.brightness = BrtN2F(brt);
    // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_BRT,animation.brightness);
    // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_BRT,animation.brightness);
    isserviced++;
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
    scene.colour = HsbColor(RgbColor(rgb.R,rgb.G,rgb.B));
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_RGB ":%s " D_NEOPIXEL_RGB ":%d,%d,%d " D_NEOPIXEL_HSB ":%d,%d,%d"),
      rgbpacked,rgb.R,rgb.G,rgb.B,scene.colour.H,scene.colour.S,scene.colour.B);
  }

  // TIME with different units
  if(!obj[D_JSON_TIME].isNull()){ //default to secs
    animation.transition.time_ms.val = obj["time"];
    animation.transition.time_ms.val *= 1000;
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),animation.transition.time_ms.val);  
  }else
  if(!obj[D_JSON_TIME].isNull()){
    animation.transition.time_ms.val = obj["time_secs"];
    animation.transition.time_ms.val *= 1000;
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),animation.transition.time_ms.val);  
  }else
  if(!obj[D_JSON_TIME_MS].isNull()){
    animation.transition.time_ms.val = obj["time_ms"];
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),animation.transition.time_ms.val);  
  }

  
// Flash colour needs to NOT be a scene, but part of a manual direct mode
// if(strstr(scenectr,"FLASHCOLOUR")){
//       scene_reseting.name_id = scene.name_id; // remember scene to return to
//       scene.name_id = SCENES_FLASHCOLOUR_ID;
//     }
//     // Parse out flashcolour info if it exists
//     if(scene.name_id == SCENES_FLASHCOLOUR_ID){

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
  
  return isserviced;



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

//   // PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM -

//     if(root["RGB"].is<JsonArray>()){

//       JsonArray colourarray = root["RGB"];
//       int array_length = colourarray.size();

//       const char* pixelcolour;
//       for(JsonVariant v : colourarray) {
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
//       for(JsonVariant v : colourarray) {
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
//       for(JsonVariant v : colourarray) {
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

//   animation.mode_id = MODE_AMBILIGHT_ID;
//   fForcePanelUpdate = true;

  return 0;

} // END FUNCTION






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

  // root[D_JSON_ONOFF] = fLEDStripOnOff ? "ON" : "OFF";
  // root[D_JSON_MODE] = GetAnimationModeName();
  // root[D_JSON_COLOUR_PALETTE] = GetPaletteFriendlyName();
  // // root[D_JSON_BRIGHTNESS_PERCENTAGE] = animation.brightness*100;
  // root[D_JSON_BRIGHTNESS] = animation.brightness;

  // JsonObject transitionobj = root.createNestedObject(D_JSON_TRANSITION);
  //   transitionobj[D_JSON_METHOD] = GetTransitionMethodName();
  //   // transitionobj[D_JSON_TIME] = mSupport::safeDivideInt(animation.transition.time_ms.val,1000);
  //   transitionobj[D_JSON_TIME_MS] = animation.transition.time_ms.val;
  //   // transitionobj[D_JSON_RATE] = mSupport::safeDivideInt(animation.transition.rate_ms.val,1000);
  //   transitionobj[D_JSON_RATE_MS] = animation.transition.rate_ms.val;
  //   // transitionobj[D_JSON_PIXELS_UPDATE_NUMBER] = GetPixelsToUpdateAsNumberFromPercentage(animation.transition.pixels_to_update_as_percentage.val);
  //   transitionobj[D_JSON_PIXELS_UPDATE_PERCENTAGE] = animation.transition.pixels_to_update_as_percentage.val;
  //   transitionobj[D_JSON_ORDER] = GetTransitionOrderName();

  // // Flags and states that are used during one transition and reset when completed
  // // JsonObject overridesobj = root.createNestedObject("transition_overrides"); 
  // //   overridesobj["fRefreshAllPixels"] = animation_override.fRefreshAllPixels;
  
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

  #ifdef ENABLE_LOG_LEVEL_DEBUG
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::ConstructJSON_State"));
  #endif
  DynamicJsonDocument doc(1500);
  JsonObject root = doc.to<JsonObject>();

  root[D_JSON_SIZE] = strip_size;

  JsonArray colourarr = root.createNestedArray("rgb");

  int numpixels = strip_size;
  if(numpixels>55){numpixels=55;}

  for(int i=0;i<numpixels;i++){
    char tmpctr[10];  memset(tmpctr,0,sizeof(tmpctr));
    RgbTypeColor c = GetPixelColor(i);
    sprintf(tmpctr,PSTR("%02X%02X%02X"),c.R,c.G,c.B);
    colourarr.add(tmpctr);
  }

  data_buffer2.payload.len = measureJson(root)+1;
  serializeJson(doc,data_buffer2.payload.ctr);

}


uint8_t mRGBAnimator::ConstructJSON_Timed(uint8_t json_level){

  #ifdef ENABLE_LOG_LEVEL_DEBUG
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::ConstructJSON_Timed"));
  #endif

  // DynamicJsonDocument doc(1500);
  // JsonObject root = doc.to<JsonObject>();

  // root[D_JSON_SIZE] = strip_size;
  // root["timed"] = 0;

  // data_buffer2.payload.len = measureJson(root)+1;
  // serializeJson(doc,data_buffer2.payload.ctr);

  // return (data_buffer2.payload.len>3?1:0);

  return 0;
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
      AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "Notif tSaved.AutoOff [%d]"),notif.pixel[i].auto_time_off_secs);
      #endif
      if(notif.pixel[i].auto_time_off_secs==1){ //if =1 then turn off and clear to 0
        SetPixelColor(i,0);
        notif.pixel[i].auto_time_off_secs = 0;
        notif.pixel[i].mode = NOTIF_MODE_OFF_ID;
        #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "Notif tSaved.AutoOff to OFF[%d]"),notif.pixel[i].auto_time_off_secs);
        #endif
      }else
      if(notif.pixel[i].auto_time_off_secs>1){ //if =1 then turn off and clear to 0
        notif.pixel[i].auto_time_off_secs--; //decrease
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
int8_t mRGBAnimator::parsesub_NotificationPanel(JsonObjectConst obj){ 

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
  
  int8_t isserviced = 0;

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
      JsonArray array = obj[D_JSON_PIXELNUM];
      for(JsonVariant v : array) {
        int val = v.as<int>();
        parsed.pixelnum.val[parsed.pixelnum.found_idx++] = val;
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PIXELNUM " JsonArray " " [i%d:v%d]"),parsed.pixelnum.found_idx-1,val);          
      }
      isserviced++;
    }else
    if(obj[D_JSON_PIXELNUM].is<const char*>()){
      const char* name = obj[D_JSON_PIXELNUM];
      if(strstr(name,"all")){
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PIXELNUM " const char* " "all"));     
        for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ parsed.pixelnum.val[parsed.pixelnum.found_idx++] = i; }
      }
      isserviced++;
    }else 
    if(obj[D_JSON_PIXELNUM].is<int>()){
      int val = obj[D_JSON_PIXELNUM];
      parsed.pixelnum.val[parsed.pixelnum.found_idx++] = val;
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PIXELNUM " int" " [i%d:v%d]"),parsed.pixelnum.found_idx-1,val);    
      isserviced++;
    }else{
      AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PIXELNUM "Not Found"));
      return isserviced; //!isserviced
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
      JsonArray array = obj[D_JSON_RGB];
      for(JsonVariant v : array) {
        const char* val = v.as<const char*>();
        if(val[0]=='#'){ colour32bit = (long) strtol( &val[1], NULL, 16); }else{ colour32bit = (long) strtol( &val[0], NULL, 16); }
        parsed.hsb.val[parsed.hue.found_idx++] = HsbColor(RgbColor((colour32bit>>16),(colour32bit>>8&0xFF),(colour32bit&0xFF)));
        #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_RGB " JsonArray " " [i%d:v%s]"),parsed.hsb.found_idx-1,val);    
        #endif
      }
      isserviced++;
    }else
    if(obj[D_JSON_RGB].is<const char*>()){
      const char* val = obj[D_JSON_RGB];
      if(val[0]=='#'){ colour32bit = (long) strtol( &val[1], NULL, 16); }else{ colour32bit = (long) strtol( &val[0], NULL, 16); }
      parsed.hsb.val[parsed.hue.found_idx++] = HsbColor(RgbColor((colour32bit>>16),(colour32bit>>8&0xFF),(colour32bit&0xFF)));
      #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_RGB " JsonArray " " [i%d:v%s]"),parsed.hsb.found_idx-1,val);    
      #endif
      isserviced++;
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
      JsonArray array = obj[D_JSON_HUE];
      for(JsonVariant v : array) {
        int val = v.as<int>();
        parsed.hue.val[parsed.hue.found_idx++] = val;
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_HUE " JsonArray " " [i%d:v%d]"),parsed.hue.found_idx-1,val);    
      }
      isserviced++;
    }else
    if(obj[D_JSON_HUE].is<const char*>()){
      const char* colour = obj[D_JSON_HUE];
      // FUTURE : Add a get hue by colour name function
      // if(strstr(name,"all")){
      //   AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS D_JSON_PIXELNUM " const char* " "all"));     
      //   for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ parsed.pixelnum.val[parsed.pixelnum.found_idx++] = i; }
      // }
      isserviced++;
    }else 
    if(obj[D_JSON_HUE].is<int>()){
      int val = obj[D_JSON_HUE];
      parsed.hue.val[parsed.hue.found_idx++] = val;
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_HUE " int" " [i%d:v%d]"),parsed.hue.found_idx-1,val);    
      isserviced++;
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
      JsonArray array = obj[D_JSON_SAT];
      for(JsonVariant v : array) {
        int val = v.as<int>();
        parsed.sat.val[parsed.sat.found_idx++] = val;
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_BRT " JsonArray " " [i%d:v%d]"),parsed.sat.found_idx-1,val);    
      }
      isserviced++;
    }else
    if(obj[D_JSON_BRT].is<const char*>()){
      const char* name = obj[D_JSON_BRT];
      // FUTURE : Add a get brt by name function
      // if(strstr(name,"all")){
      //   AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS D_JSON_PIXELNUM " const char* " "all"));     
      //   for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ parsed.pixelnum.val[parsed.pixelnum.found_idx++] = i; }
      // }
      isserviced++;
    }else 
    if(obj[D_JSON_SAT].is<int>()){
      int val = obj[D_JSON_SAT];
      parsed.sat.val[parsed.sat.found_idx++] = val;
      #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_SAT " int" " [i%d:v%d]"),parsed.sat.found_idx-1,val);    
      #endif
      isserviced++;
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
      JsonArray array = obj[D_JSON_BRT];
      for(JsonVariant v : array) {
        int val = v.as<int>();
        parsed.brt.val[parsed.brt.found_idx++] = val;
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_BRT " JsonArray " " [i%d:v%d]"),parsed.brt.found_idx-1,val);    
      }
      isserviced++;
    }else
    if(obj[D_JSON_BRT].is<const char*>()){
      const char* name = obj[D_JSON_BRT];
      // FUTURE : Add a get brt by name function
      // if(strstr(name,"all")){
      //   AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS D_JSON_PIXELNUM " const char* " "all"));     
      //   for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ parsed.pixelnum.val[parsed.pixelnum.found_idx++] = i; }
      // }
      isserviced++;
    }else 
    if(obj[D_JSON_BRT].is<int>()){
      int val = obj[D_JSON_BRT];
      parsed.brt.val[parsed.brt.found_idx++] = val;
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_BRT " int" " [i%d:v%d]"),parsed.brt.found_idx-1,val);    
      isserviced++;
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
      JsonArray array = obj[D_JSON_WHITE];
      for(JsonVariant v : array) {
        int val = v.as<int>();
        // Serial.println(parsed.white.found_idx);
        parsed.white.val[parsed.white.found_idx++] = val;//map(val,0,100,0,255); //only 0-100 accepted
        // Serial.println(parsed.white.found_idx);
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_WHITE " JsonArray " " [i%d:v%d]"),parsed.white.found_idx-1,val);    
      }
      isserviced++;
    }else
    if(obj[D_JSON_WHITE].is<const char*>()){
      const char* name = obj[D_JSON_WHITE];
      // FUTURE : Add a get brt by name function
      // if(strstr(name,"all")){
      //   AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS D_JSON_PIXELNUM " const char* " "all"));     
      //   for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ parsed.pixelnum.val[parsed.pixelnum.found_idx++] = i; }
      // }
      isserviced++;
    }else 
    if(obj[D_JSON_WHITE].is<int>()){
      int val = obj[D_JSON_WHITE];
      parsed.white.val[parsed.white.found_idx++] = val;
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_WHITE " int" " [i%d:v%d]"),parsed.white.found_idx-1,val);    
      isserviced++;
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
      JsonArray array = obj[D_JSON_FLASHER];
      for(JsonVariant v : array) {
        int val = v.as<int>();
        parsed.flasher.val[parsed.flasher.found_idx++] = val;
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_FLASHER " JsonArray " " [i%d:v%d]"),parsed.flasher.found_idx-1,val);    
      }
      isserviced++;
    }else
    if(obj[D_JSON_FLASHER].is<int>()){
      int val = obj[D_JSON_FLASHER];
      parsed.flasher.val[parsed.flasher.found_idx++] = val;
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_FLASHER " int" " [i%d:v%d]"),parsed.flasher.found_idx-1,val);    
      isserviced++;
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
      JsonArray array = obj[D_JSON_PULSER];
      for(JsonVariant v : array) {
        int val = v.as<int>();
        parsed.pulser.val[parsed.pulser.found_idx++] = val;
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PULSER " JsonArray " " [i%d:v%d]"),parsed.pulser.found_idx-1,val);    
      }
      isserviced++;
    }else
    if(obj[D_JSON_PULSER].is<int>()){
      int val = obj[D_JSON_PULSER];
      parsed.pulser.val[parsed.pulser.found_idx++] = val;
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PULSER " int" " [i%d:v%d]"),parsed.pulser.found_idx-1,val);    
      isserviced++;
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
      JsonArray array = obj[D_JSON_FADE];
      for(JsonVariant v : array) {
        int val = v.as<int>();
        parsed.fade.val[parsed.fade.found_idx++] = val;
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_FADE " JsonArray " " [i%d:v%d]"),parsed.fade.found_idx-1,val);    
      }
      isserviced++;
    }else
    if(obj[D_JSON_FADE].is<int>()){
      int val = obj[D_JSON_FADE];
      parsed.fade.val[parsed.fade.found_idx++] = val;
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_FADE " int" " [i%d:v%d]"),parsed.fade.found_idx-1,val);    
      isserviced++;
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
      JsonArray array = obj[D_JSON_TIME_SECS];
      for(JsonVariant v : array) {
        int val = v.as<int>();
        parsed.timeon_ms.val[parsed.timeon_ms.found_idx++] = val*1000; //secs2ms
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_SECS " JsonArray " " [i%d:v%d]"),parsed.timeon_ms.found_idx-1,val);    
      }
      isserviced++;
    }else
    //no string command
    if(obj[D_JSON_TIME_SECS].is<int>()){
      int val = obj[D_JSON_TIME_SECS];
      parsed.timeon_ms.val[parsed.timeon_ms.found_idx++] = val*1000; //secs2ms
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_SECS " int" " [i%d:v%d]"),parsed.timeon_ms.found_idx-1,val);    
      isserviced++;
    }else{
      AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_SECS "Not Found"));
    }
  }else
  if(!obj[D_JSON_TIME_MS].isNull()){
    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_TIME_MS));  
    // Arrays
    if(obj[D_JSON_TIME_MS].is<JsonArray>()){   
      JsonArray array = obj[D_JSON_TIME_MS];
      for(JsonVariant v : array) {
        int val = v.as<int>();
        parsed.timeon_ms.val[parsed.timeon_ms.found_idx++] = val;
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_MS " JsonArray " " [i%d:v%d]"),parsed.timeon_ms.found_idx-1,val);    
      }
      isserviced++;
    }else
    //no string command
    if(obj[D_JSON_TIME_MS].is<int>()){
      int val = obj[D_JSON_TIME_MS];
      parsed.timeon_ms.val[parsed.timeon_ms.found_idx++] = val;
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_MS " int" " [i%d:v%d]"),parsed.timeon_ms.found_idx-1,val);    
      isserviced++;
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
      notif.pixel[pixelnum].auto_time_off_secs = (parsed.timeon_ms.val[subidx]/1000); 
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
  animation.mode_id = ANIMATION_MODE_NOTIFICATIONS_ID;
  notif.flags.fForcePanelUpdate = true;

  return isserviced;


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
      
      
      
  // JsonArray mode_arr = root.createNestedArray("auto_time_off_secs");
  // for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){
  //   mode_arr.add(notif.pixel[i].auto_time_off_secs);
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




