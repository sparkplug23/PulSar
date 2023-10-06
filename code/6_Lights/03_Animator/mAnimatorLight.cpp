#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

const char* mAnimatorLight::PM_MODULE_LIGHTS_ANIMATOR_CTR = D_MODULE_LIGHTS_ANIMATOR_CTR;
const char* mAnimatorLight::PM_MODULE_LIGHTS_ANIMATOR_FRIENDLY_CTR = D_MODULE_LIGHTS_ANIMATOR_FRIENDLY_CTR;

uint16_t mAnimatorLight::_usedSegmentData = 0;

int8_t mAnimatorLight::Tasker(uint8_t function, JsonParserObject obj)
{


  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case FUNC_PRE_INIT:
      Pre_Init();  
    break;
    case FUNC_INIT:
      Init();
    break;
  }
  
  if(!settings.flags.EnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID;}
  
  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_EVERY_SECOND:{      
      //EverySecond();
      EverySecond_AutoOff(); 

      // ALOG_INF(PSTR("param users %d, %d, %d, %d"), SEGMENT_I(0).params_user.val0, SEGMENT_I(0).params_user.val1, SEGMENT_I(0).params_user.val2, SEGMENT_I(0).params_user.val3);

    }break;
    case FUNC_LOOP:     
      EveryLoop();  
    break;        
    case FUNC_BOOT_MESSAGE:
      BootMessage();
    break;
    /************
     * TRIGGERS SECTION * 
    *******************/
    case FUNC_EVENT_INPUT_STATE_CHANGED_ID:
      #ifdef USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS
      CommandSet_Physical_UserInput_Buttons();
      #endif
    break;
    #ifdef USE_MODULE_CORE_RULES
    case FUNC_EVENT_SET_POWER_ID:
      RulesEvent_Set_Power();
    break;
    #endif// USE_MODULE_CORE_RULES
    /************
     * MQTT SECTION * 
    *******************/   
    #ifdef USE_MODULE_NETWORK_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Set_DefaultPeriodRate();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_RefreshAll();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
    /************
     * WIFI SECTION * 
    *******************/   
    case FUNC_WIFI_CONNECTED:
      #ifdef USE_DEVFEATURE_ANIMATOR_INIT_CODE__SEGMENT_MATRIX_TESTER
      Test_Config();
      #endif
    break;
    /************
     * WEBUI SECTION * 
    *******************/   
    #ifdef USE_MODULE_NETWORK_WEBSERVER23
    case FUNC_WEB_ADD_HANDLER:
      WebPage_Root_AddHandlers();
    break;
    #endif // USE_MODULE_NETWORK_WEBSERVER23
  } // switch(command)


} // END FUNCTION


/**
 * Cleaned version for segments
 * */
void mAnimatorLight::EveryLoop()
{
     
  //LED settings have been saved, re-init busses
  //This code block causes severe FPS drop on ESP32 with the original "if (busConfigs[0] != nullptr)" conditional. Investigate!
  if (doInitBusses) {
    doInitBusses = false;
    DEBUG_PRINTLN(F("Re-init busses."));
    bool aligned = checkSegmentAlignment(); //see if old segments match old bus(ses)
    pCONT_iLight->bus_manager->removeAll();
    uint32_t mem = 0;
    for (uint8_t i = 0; i < WLED_MAX_BUSSES+WLED_MIN_VIRTUAL_BUSSES; i++) 
    {
      if (pCONT_iLight->busConfigs[i] == nullptr) break;
      mem += BusManager::memUsage(*pCONT_iLight->busConfigs[i]);
      if (mem <= MAX_LED_MEMORY) 
      {
        DEBUG_LINE_HERE;
        pCONT_iLight->bus_manager->add(*pCONT_iLight->busConfigs[i]);
        DEBUG_LINE_HERE;
      }else{
        
        ALOG_ERR(PSTR("MEMORY ISSUE"));
        DEBUG_LINE_HERE;

      }
      delete pCONT_iLight->busConfigs[i]; pCONT_iLight->busConfigs[i] = nullptr;
    }
    
    finalizeInit(); // also loads default ledmap if present
    
    if (aligned) makeAutoSegments();
    else fixInvalidSegments();
    
    yield();
    // serializeConfig(); // in WLED This saved everything to json memory
  }

  /**
   * @brief If RealTime modes first
   * - when realtime is used, effects are not to keep performance up and since the realtime aspect (network) needs priority, it is not going to be
   *   sustainable to also have internal animations running 
   * 
   * realtimeModes will always be loaded into the first segment, with other segments paused when realtimeMode is active
   */
  if(realtimeMode)
  {
    switch(SEGMENT_I(0).animation_mode_id)    // needs to know the id 
    {
      #ifdef ENABLE_ANIMATION_MQTT_SETPIXEL
      case ANIMATION_MODE__MQTT_SETPIXEL:
        SubTask_Segments_Effects();
      break;
      #endif
      #ifdef ENABLE_ANIMATION_REALTIME_UDP
      case ANIMATION_MODE__REALTIME_UDP:
        SubTask_Segments_Effects();
      break;
      #endif
      #ifdef ENABLE_ANIMATION_MODE__REALTIME_HYPERION
      case ANIMATION_MODE__REALTIME_HYPERION:
        SubTask_Segments_Effects();
      break;
      #endif
      #ifdef ENABLE_ANIMATION_MODE__REALTIME_E131
      case ANIMATION_MODE__REALTIME_E131:
        SubTask_Segments_Effects();
      break;
      #endif
      #ifdef ENABLE_ANIMATION_MODE__REALTIME_ADALIGHT
      case ANIMATION_MODE__REALTIME_ADALIGHT:
        SubTask_Segments_Effects();
      break;
      #endif
      #ifdef ENABLE_ANIMATION_MODE__REALTIME_ARTNET
      case ANIMATION_MODE__REALTIME_ARTNET:
        SubTask_Segments_Effects();
      break;
      #endif
      #ifdef ENABLE_ANIMATION_MODE__REALTIME_TPM2NET
      case ANIMATION_MODE__REALTIME_TPM2NET:
        SubTask_Segments_Effects();
      break;
      #endif
      #ifdef ENABLE_ANIMATION_MODE__REALTIME_DDP
      case ANIMATION_MODE__REALTIME_DDP:
        SubTask_Segments_Effects();
      break;
      #endif
      case ANIMATION_MODE__DISABLED: default: return; // Leave function
    } // END switch

  } // END realtimeMode


  if(!realtimeMode)
  {

    #ifdef ENABLE_ANIMATION_MODE__EFFECTS
    SubTask_Segments_Effects();
    #endif  

    uint8_t flag_animations_needing_updated = 0;

    DEBUG_PIN3_SET(0);
      
    for (uint16_t seg_i = 0; seg_i < segments.size(); seg_i++)
    {

      _segment_index_primary = seg_i; // Important! Otherwise _segment_index_primary can be incremented out of scope when a later callback is called

      if(SEGMENT_I(seg_i).isActive())
      {
        if (SEGMENT_I(seg_i).animator->IsAnimating())
        {
          /**
           * @brief A Backoff time is needed per animation so the DMA is not overloaded
          **/
          if(mTime::TimeReached(&SEGMENT_I(seg_i).tSaved_AnimateRunTime, ANIMATION_UPDATOR_TIME_MINIMUM))
          {
  DEBUG_PIN4_TOGGLE();
            SEGMENT_I(seg_i).animator->UpdateAnimations(seg_i);
            flag_animations_needing_updated++; // channels needing updated
          } //end TimeReached
    
          SEGMENT.flags.animator_first_run = RESET_FLAG;     // CHANGE to function: reset here for all my methods

          /**
           * @brief If it completes, reset
           *
           * Need proper checks here to only reset if it was running before
           */
          if(!SEGMENT_I(seg_i).animator->IsAnimationActive(0))
          {
            SEGMENT_I(seg_i).transitional = false; // Since this is already inside "IsAnimating" then it should only be set if it was animating but is now stopping
          }

        } // IsAnimating

      } // isActive

    } // loop segments

    /**
     * @brief If any change has happened, update!?
     * IsDirty checks to only update if needed
     */
    if(flag_animations_needing_updated)
    {
      #ifdef ENABLE_DEVFEATURE_DEBUG_SERIAL__ANIMATION_OUTPUT
      ALOG_INF(PSTR("flag_animations_needing_updated=%d"),flag_animations_needing_updated);
      #endif
      #ifdef ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32
        // some buses send asynchronously and this method will return before
        // all of the data has been sent.
        // See https://github.com/Makuna/NeoPixelBus/wiki/ESP32-NeoMethods#neoesp32rmt-methods
        // if(pCONT_iLight->bus_manager)
        // {
        //   pCONT_iLight->bus_manager->show();
        // }        
        pCONT_iLight->neopixel_runner->execute();   
      #else
        // some buses send asynchronously and this method will return before
        // all of the data has been sent.
        // See https://github.com/Makuna/NeoPixelBus/wiki/ESP32-NeoMethods#neoesp32rmt-methods
        if(pCONT_iLight->bus_manager)
        {
          pCONT_iLight->bus_manager->show();
        }
      #endif // ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32

    } // flag_animations_needing_updated
    else
    {
      /**
       * @brief If no animations are running, with backoff, lets check the current output (though, this may be bad since I want to limit BEFORE hitting max)
       **/
      // stripbus->SetPixelSettings(NeoRgbCurrentSettings(1,2,3))
      // ALOG_INF(PSTR("power %d",stripbus->CalcTotalMilliAmpere());
      // float power_level = 0;
      // for(uint16_t i=0;i<STRIP_SIZE_MAX;i++){
      //   stripbus->SEGMENT.GetPixelColor
      // }

    }
    DEBUG_PIN3_SET(1);


    /**
     *  control relay power
     * */
    // //(pCONT_iLight->light_power_state)
    //   if(pCONT_iLight->light_power_state!=pCONT_iLight->light_power_Saved){
    //     pCONT_iLight->light_power_Saved = pCONT_iLight->light_power_state;
    //     // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "pCONT_iLight->light_power_Saved"));
    //     //pCONT->mry->CommandSet_Relay_Power(0,pCONT_iLight->light_power_state);
    //   }


    /**
     * Check any lights are on
     * */
    // if(mTime::TimeReached(&tSavedCheckAnyLEDsOnOff,1000)){
    //   fAnyLEDsOnOffCount = 0;
    //   for(int i=0;i<pCONT_iLight->settings.light_size_count;i++){
    //     #ifdef ENABLE_LOG_LEVEL_DEBUG
    //     ALOG_DBM( PSTR(D_LOG_NEO "fAnyLEDsOnOffCount %d"),fAnyLEDsOnOffCount);
    //     #endif
    //     if(SEGMENT.GetPixelColor(i)!=0){
    //       fAnyLEDsOnOffCount++;
    //     }
    //   }
    //   if(fAnyLEDsOnOffCount>0){
    //     #ifdef ENABLE_LOG_LEVEL_DEBUG
    //     ALOG_DBM( PSTR(D_LOG_NEO "fAnyLEDsOnOffCount %d"),fAnyLEDsOnOffCount);
    //     #endif
    //   }
    //   // #ifdef USE_MODULE_DRIVERS_RELAY
    //   //   //pCONT->mry->CommandSet_Relay_Power(0,fAnyLEDsOnOffCount>0?1:0);
    //   // #endif
    // }

    #ifdef ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
    handlePlaylist();
    #endif
    #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS
    handlePresets();
    #endif

  } // End of !realtimeMode and Effect methods

}


void mAnimatorLight::BootMessage()
{
  
  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // AddLog(LOG_LEVEL_INFO, PSTR("BOOT: " "LightCount=%d"), settings.light_size_count);
  // #endif // ENABLE_LOG_LEVEL_COMMANDS

}


void mAnimatorLight::Pre_Init(void)
{
  
}


void mAnimatorLight::Init_SegmentWS2812FxStrip() //rename later
{

  paletteFade=0;
  paletteBlend = 0;
  milliampsPerLed = 5;
  cctBlending = 0;
  ablMilliampsMax = 0;
  currentMilliamps = 0;
  _now = millis();
  timebase = 0;
  isMatrix = false;
  #ifndef WLED_DISABLE_2D
  hPanels(1),
  vPanels(1),
  panelH(8),
  panelW(8),
  matrix{0,0,0,0},
  panel{{0,0,0,0}},
  #endif
  _virtualSegmentLength = 0;
  _length = DEFAULT_LED_COUNT;
  _brightness = DEFAULT_BRIGHTNESS;
  _transitionDur = 750;
  _targetFps = WLED_FPS;
  _frametime = FRAMETIME_FIXED;
  _cumulativeFps = 2;
  _isServicing = false;
  _isOffRefreshRequired = false;
  _hasWhiteChannel = false;
  _triggered = false;
  effects.count = getEffectsAmount();
  _callback = nullptr;
  customMappingTable = nullptr;
  customMappingSize = 0;
  _lastShow = 0;
  _segment_index_primary = 0;
  _mainSegment = 0;

  effects.function.reserve(effects.count);     // allocate memory to prevent initial fragmentation (does not increase size())
  effects.data.reserve(effects.count); // allocate memory to prevent initial fragmentation (does not increase size())
  
}


void mAnimatorLight::Init__Palettes()
{
  
  char buffer[30];
  
  for (int ii=0;ii<(mPaletteI->PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID-mPaletteI->PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_01__ID);ii++){ 
    sprintf(buffer, D_DEFAULT_DYNAMIC_PALETTE_NAMES__VARIABLE_RGBCCT__NAME_CTR, ii+1);
    uint8_t adjusted_id = ii;
    DLI->AddDeviceName(buffer, GetModuleUniqueID(), adjusted_id);
  }

  for (int ii=0;ii< mPaletteI->user_defined_palette_count;ii++){ 
    sprintf(buffer, D_DEFAULT_MODIFIABLE_PALETTE_NAMES__USER_CREATED__NAME_CTR, ii+1);
    DLI->AddDeviceName(buffer, GetModuleUniqueID(), ii + (mPaletteI->PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID-mPaletteI->PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_01__ID));
  }

}


void mAnimatorLight::Init(void)
{ 

  device = pCONT_set->devices_present;
  
  subtype = RgbcctColor::ColourType::COLOUR_TYPE__RGBCCT__ID;
  
  pwm_multi_channels = 0;

  pCONT_set->Settings.pwm_range = PWM_RANGE;
  pCONT_set->Settings.light_settings.light_fade = 1;
  pCONT_set->Settings.light_settings.light_speed = 5*2;
  pCONT_set->power = 1;

  auto_off_settings.time_decounter_secs = 0;

  #ifdef ENABLE_WEBSERVER_LIGHTING_WEBUI
  sprintf(serverDescription, "WLED");
  sprintf(ntpServerName, "0.wled.pool.ntp.org");  
  sprintf(apPass, CLIENT_SSID);
  sprintf(otaPass, "WLED");
  sprintf(clientSSID, CLIENT_SSID);
  sprintf(clientPass, CLIENT_PASS);
  sprintf(alexaInvocationName, "Light");
  #endif // ENABLE_WEBSERVER_LIGHTING_WEBUI


  #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS
  initPresetsFile();
  #endif // ENABLE_DEVFEATURE_LIGHTING__PRESETS

  Init__Palettes();  

  // Allow runtime changes of animation size
  // settings.light_size_count = STRIP_SIZE_MAX; 
  pCONT_set->Settings.flag_animations.clear_on_reboot = false; //flag

  /**
   * @brief Search for Digital pins
   * I could search for how many pins within each type and configure defaults for them. eg.
   * A channel only: Assume Ws2812 GRB
   * A+(B/C/D/E): PWM type of single for just A and RGBCCT for ABCDE
   **/
  for (uint8_t pins = 0; pins < MAX_PIXELBUS_DIGITAL_PINS; pins++) 
  {
    // ALOG_INF (PSTR(D_LOG_DSB "PinUsed %d %d"), pCONT_pins->PinUsed(GPIO_DSB_1OF2_ID, pins), pCONT_pins->GetPin(GPIO_DSB_1OF2_ID, pins));
    // if (pCONT_pins->PinUsed(GPIO_DSB_1OF2_ID, pins)) 
    // {
    //   ds18x20_gpios[pins] = new OneWire(pCONT_pins->GetPin(GPIO_DSB_1OF2_ID, pins));
    //   ALOG_INF(PSTR(D_LOG_DSB "pins_used %d"), settings.pins_used);
    //   settings.pins_used++;
    // }
  }

  Init_SegmentWS2812FxStrip();

  /**
   * @brief Called the first time to ensure the minimal default version is started
   * Busses must be reconfigured with lighting_template. This process of calling should be optimising later with better integration
   **/
  ALOG_INF(PSTR("Preloading a default single neopixel bus until LIGHTING_TEMPLATE is loaded: This can be changed as flag so this is called in the main loop at runtime"));

  #ifndef ENABLE_DEVFEATURE_LIGHT__MOVE_ALL_BUS_STARTING_CODE_UNTIL_LOOP
  finalizeInit();
  makeAutoSegments();
  #endif 
  
  // settings.light_size_count = STRIP_SIZE_MAX;
  pCONT_set->Settings.flag_animations.clear_on_reboot = false; //flag

  /**
   * @brief Check if any pin is set
   * Note: this is going to clash with PWM types in Lighting and should probably be moved into there, leaving here to check in both
   **/
  bool flag_any_pin_set = false;
  for(uint16_t gpio = GPIO_PIXELBUS_01_A_ID; gpio < GPIO_PIXELBUS_10_E_ID; gpio++)
  {
    if(pCONT_pins->PinUsed(gpio))
    {
      flag_any_pin_set = true;
      break;
    }
  }

  if(!flag_any_pin_set)
  {
    ALOG_ERR(PSTR("NO PIN FOUND"));
  }

  LoadEffects();

  Init_Segments();

  ALOG_INF(PSTR("Segments Initiated and save to use")); 

  #ifdef ENABLE_PIXEL_AUTOMATION_PLAYLIST
  init_mixer_defaults();
  #endif

  settings.flags.EnableModule = true;

} // END "init"


/**
 * @brief Now, do I use "Save" going forward as simple byte packed struct, or, saving them as json encoded in memory (much larger, but more dynamic)
 **/
void mAnimatorLight::Settings_Load()
{
    
  // setBriRGB_Global(pCONT_set->Settings.light_settings.light_brightness_as_percentage);

}


void mAnimatorLight::Settings_Save()
{
  
}


void mAnimatorLight::Settings_Default(){

  init(); //default values

}


#ifdef USE_MODULE_CORE_RULES
void mAnimatorLight::RulesEvent_Set_Power()
{

  AddLog(LOG_LEVEL_TEST, PSTR("MATCHED RulesEvent_Set_Power"));

  uint8_t index = pCONT_rules->rules[pCONT_rules->rules_active_index].command.device_id;
  uint8_t state = pCONT_rules->rules[pCONT_rules->rules_active_index].command.value.data[0];

  bool get_state = CommandGet_LightPowerState();

  AddLog(LOG_LEVEL_TEST, PSTR("CommandGet_LightPowerState() = %d"), get_state);

  // get state
  ModifyStateNumberIfToggled(&state, CommandGet_LightPowerState());

  // apply invert if needed
  CommandSet_LightPowerState(state);

}
#endif // USE_MODULE_CORE_RULES


void mAnimatorLight::EverySecond_AutoOff()
{

  /**
   * @brief Master AutoOff DeCounter
   **/
  //ALOG_DBM( PSTR(D_LOG_LIGHT "scene.auto_off_settings.tSaved [%d]"),animation.auto_off_settings.time_decounter_secs);
  if(auto_off_settings.time_decounter_secs==1){ //if =1 then turn off and clear to 0
    // animation.name_id = MODE_SINGLECOLOUR_FADE_OFF__ID;
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT "auto_off_settings.time_decounter_secs==1 and disable"));
    #endif       
    DEBUG_LINE_HERE;
    pCONT_lAni->CommandSet_LightPowerState(LIGHT_POWER_STATE_OFF_ID);
    //#ifdef ENABLE_LOG_LEVEL_INFO
    auto_off_settings.time_decounter_secs=0;
    //#endif
  }else
  if(auto_off_settings.time_decounter_secs>1){ //if =1 then turn off and clear to 0
    auto_off_settings.time_decounter_secs--; //decrease    
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT "auto_off_settings.time_decounter_secs=%d dec"),auto_off_settings.time_decounter_secs);
    #endif
  }

  /**
   * @brief Segment(s) AutoOff DeCounter
   **/
  for (uint8_t seg_i=0; seg_i<segments.size(); seg_i++) 
  {
  
    if(SEGMENT_I(seg_i).auto_timeoff.UpdateTick())
    {
      ALOG_INF( PSTR(D_LOG_GARAGE D_JSON_COMMAND_NVALUE_K("Running Value")), SEGMENT_I(seg_i).auto_timeoff.Value());
    }

    if(SEGMENT_I(seg_i).auto_timeoff.IsLastTick())
    {
      ALOG_INF(PSTR("Segment Turn OFF"));
      // Set intensity to make all LEDs refresh
      SEGMENT_I(seg_i).set_intensity(255);
      pCONT_lAni->SEGMENT_I(seg_i).single_animation_override.time_ms = 1000; // slow turn off

      ALOG_INF(PSTR("Setting override for off %d"), pCONT_lAni->SEGMENT_I(seg_i).single_animation_override.time_ms);

      pCONT_lAni->SEGMENT_I(seg_i).flags.fForceUpdate = true;

      SEGMENT_I(seg_i).setBrightnessRGB(0);
      SEGMENT_I(seg_i).setBrightnessCCT(0);    
    }

  }

}// END EverySecond_AutoOff


RgbcctColor mAnimatorLight::ApplyBrightnesstoRgbcctColour(RgbcctColor full_range_colour, uint8_t brightness)
{

  RgbcctColor colour_adjusted_with_brightness = RgbcctColor(0);
  colour_adjusted_with_brightness.R  = mapvalue(full_range_colour.R,  0,255, 0,brightness);
  colour_adjusted_with_brightness.G  = mapvalue(full_range_colour.G,  0,255, 0,brightness);
  colour_adjusted_with_brightness.B  = mapvalue(full_range_colour.B,  0,255, 0,brightness);
  colour_adjusted_with_brightness.WW = mapvalue(full_range_colour.WW, 0,255, 0,brightness);
  colour_adjusted_with_brightness.WC = mapvalue(full_range_colour.WC, 0,255, 0,brightness);
  return colour_adjusted_with_brightness;

}


RgbcctColor mAnimatorLight::ApplyBrightnesstoRgbcctColour(RgbcctColor full_range_colour, uint8_t brightnessRGB, uint8_t brightnessCCT)
{

  RgbcctColor colour_adjusted_with_brightness = RgbcctColor(0);
  colour_adjusted_with_brightness.R  = mapvalue(full_range_colour.R,  0,255, 0,brightnessRGB);
  colour_adjusted_with_brightness.G  = mapvalue(full_range_colour.G,  0,255, 0,brightnessRGB);
  colour_adjusted_with_brightness.B  = mapvalue(full_range_colour.B,  0,255, 0,brightnessRGB);
  colour_adjusted_with_brightness.WW = mapvalue(full_range_colour.WW, 0,255, 0,brightnessCCT);
  colour_adjusted_with_brightness.WC = mapvalue(full_range_colour.WC, 0,255, 0,brightnessCCT);
  
  // ALOG_DBM( PSTR("full_range_colour=%d,%d,%d,%d,%d"),desired_colour.R,desired_colour.G,desired_colour.B,desired_colour.WC,desired_colour.WW);
  // ALOG_DBM( PSTR("colour_adjusted_with_brightness=%d,%d,%d,%d,%d"),colour_adjusted_with_brightness.R,colour_adjusted_with_brightness.G,colour_adjusted_with_brightness.B,colour_adjusted_with_brightness.WC,colour_adjusted_with_brightness.WW);

  return colour_adjusted_with_brightness;

}


// Assumed should be integrated into RgbcctController
RgbcctColor mAnimatorLight::ApplyBrightnesstoDesiredColourWithGamma(RgbcctColor full_range_colour, uint8_t brightness){
  
  uint8_t new_brightness_255 = 0;

  new_brightness_255 = brightness;

  // AddLog(LOG_LEVEL_TEST, PSTR("ledGamma=new_brightness_255=%d - >%d"), 
  //   new_brightness_255, pCONT_iLight->ledGamma(new_brightness_255)
  // );

  #ifdef ENABLE_GAMMA_BRIGHTNESS_ON_DESIRED_COLOUR_GENERATION
  if(SEGMENT_I(0).flags.use_gamma_for_brightness)
  {
    new_brightness_255 = pCONT_iLight->ledGamma(new_brightness_255);
  }
  #endif // ENABLE_GAMMA_BRIGHTNESS_ON_DESIRED_COLOUR_GENERATION

  return ApplyBrightnesstoRgbcctColour(full_range_colour, new_brightness_255);

}


#ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
mAnimatorLight& mAnimatorLight::setCallback_ConstructJSONBody_Debug_Animations_Progress(ANIMIMATION_DEBUG_MQTT_FUNCTION_SIGNATURE)
{
  this->anim_progress_mqtt_function_callback = anim_progress_mqtt_function_callback;
  return *this;
}
#endif // USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK


/**
 * @brief Function to load palette_id into the segment
 *   (lets just use largest possible byte value without making it dynamic for now)
 * 
 * For now, calling this function will take all data from the palette (rgb data) and move it into the array
 * 
 * Later, GetColourFromPreloadedPalette will retreive data from ram array, NEVER progmem
 * 
 * loadPalette 
 *  ** create buffer into heap, save pointer
 * 
 * unloadPalette
 *  ** delete buffer data from heap, reset to nullptr
 * 
 * should load even be here? surely into palette container
 * 
 */
void mAnimatorLight::LoadPalette(uint8_t palette_id, uint8_t segment_index, mPaletteLoaded* _palette_container)
{

  // Pass pointer to memory location to load, so I can have additional palettes. If none passed, assume primary storage of segment
  // ALOG_INF(PSTR("============LoadPalette %d %d %d"), palette_id, segment_index, pCONT_lAni->_segment_index_primary);

  /**
   * @brief If none was passed, then assume its the default for that segment and load its container
   **/
  if(_palette_container == nullptr)
  {
    _palette_container = SEGMENT_I(segment_index).palette_container;
  }

  SEGMENT_I(segment_index).palette_container->loaded_palette_id = palette_id;

  if(
    ((palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID) && (palette_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID))
  ){   

    /**
     * @brief Moving out of mPalette since it includes references to PaletteContainer. May need moving back.
     **/
    // mPaletteI->LoadPalette_CRGBPalette16_Static(palette_id, segment_index); // ie WLED22 "loadPalette"
    // ALOG_INF(PSTR("?????????????????????????????????????????? %d"), palette_id);

    /******************************************************
     * PALETTELIST_STATIC_CRGBPALETTE16__IDS
     * No gradient information in palette bytes, CRGB16 will scale equally
     ******************************************************/
    switch (palette_id)
    {
      default:
      case mPalette::PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID:  _palette_container->CRGB16Palette16_Palette.data = RainbowColors_p; break;
      case mPalette::PALETTELIST_STATIC_CRGBPALETTE16__PARTY_COLOUR__ID:    _palette_container->CRGB16Palette16_Palette.data = PartyColors_p;   break;
      case mPalette::PALETTELIST_STATIC_CRGBPALETTE16__CLOUD_COLOURS__ID:   _palette_container->CRGB16Palette16_Palette.data = CloudColors_p;   break;
      case mPalette::PALETTELIST_STATIC_CRGBPALETTE16__LAVA_COLOURS__ID:    _palette_container->CRGB16Palette16_Palette.data = LavaColors_p;    break;
      case mPalette::PALETTELIST_STATIC_CRGBPALETTE16__OCEAN_COLOUR__ID:    _palette_container->CRGB16Palette16_Palette.data = OceanColors_p;   break;
      case mPalette::PALETTELIST_STATIC_CRGBPALETTE16__FOREST_COLOUR__ID:   _palette_container->CRGB16Palette16_Palette.data = ForestColors_p;  break;
      case mPalette::PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_STRIPE_COLOUR__ID: _palette_container->CRGB16Palette16_Palette.data = RainbowStripeColors_p; break;
      case mPalette::PALETTELIST_STATIC_CRGBPALETTE16__HEAT_COLOUR__ID: _palette_container->CRGB16Palette16_Palette.data = HeatColors_p; break;
    }

    _palette_container->CRGB16Palette16_Palette.encoded_index.clear();
    for(uint8_t i=0;i<16;i++){
      _palette_container->CRGB16Palette16_Palette.encoded_index.push_back(map(i, 0,15, 0, 255));
    }

  }else
  if(
    ((palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID)    && (palette_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID))
  ){   

    uint16_t gradient_id = palette_id - mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID;
    byte tcp[72]; //support gradient palettes with up to 18 entries
    memcpy_P(tcp, (byte*)pgm_read_dword(&(gGradientPalettes[gradient_id])), 72);

    /**
     * @brief Loading uses the CRGBPalette to get the colours
     **/
    _palette_container->CRGB16Palette16_Palette.data.loadDynamicGradientPalette(tcp);

    /**
     * @brief To get the gradients data exactly, manually parse them 
     * 
     */    
    _palette_container->CRGB16Palette16_Palette.encoded_index.clear();
    TRGBGradientPaletteEntryUnion* ent = (TRGBGradientPaletteEntryUnion*)(tcp);
    TRGBGradientPaletteEntryUnion u;
    // Count entries
    uint16_t count = 0;
    do {
        u = *(ent + count);
        count++;
    } while ( u.index != 255);
    u = *ent;
    int indexstart = 0;
    while( indexstart < 255) {
      indexstart = u.index;
      _palette_container->CRGB16Palette16_Palette.encoded_index.push_back(u.index);
      ent++;
      u = *ent;
    }

    // ALOG_INF(PSTR("palette_container%d (seg%d) %d %d %d"), gradient_id, seg_i, _palette_container->CRGB16Palette16_Palette.data[0].r, _palette_container->CRGB16Palette16_Palette.data[0].g, _palette_container->CRGB16Palette16_Palette.data[0].b );
   
  }else
  if(
    ((palette_id >= mPalette::PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID) && (palette_id < mPalette::PALETTELIST_STATIC_LENGTH__ID))     
  ){      
    uint16_t palette_id_adj = palette_id - mPalette::PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID;
    // ALOG_HGL(PSTR("ERROR HERE palette_id_adj %d"),palette_id_adj); 

    mPalette::STATIC_PALETTE *ptr = &mPaletteI->static_palettes[palette_id_adj];

    #ifdef ESP32
    _palette_container->pData = std::vector<uint8_t>();
    _palette_container->pData.assign(ptr->data, ptr->data + ptr->data_length);
    // for(int i=0;i<_palette_container->pData.size();i++){
    //   Serial.printf("%d,\n\r", _palette_container->pData[i]);
    // }
    #else // ESP8266 requires safe reading out of progmem first
    // _palette_container->pData = std::vector<uint8_t>();
    // _palette_container->pData.assign(ptr->data, ptr->data + ptr->data_length);
    uint8_t buffer3[255];//ptr->data_length];
    memcpy_P(buffer3, ptr->data, sizeof(uint8_t)*ptr->data_length);
    _palette_container->pData.assign(buffer3, buffer3 + ptr->data_length);
    #endif  

  }else
  if(
    ((palette_id >= mPalette::PALETTELIST_LENGTH_OF_STATIC_IDS)  && (palette_id < mPaletteI->GetPaletteListLength())) // Custom palettes
  ){
    // Preloading is not needed, already in ram
  }else
  if(
    (palette_id >= mPalette::PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_01__ID) && (palette_id < mPalette::PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID)
  ){  
    // Preloading is not needed, already in ram
  }else
  if(
    (palette_id >= mPalette::PALETTELIST_DYNAMIC_CRGBPALETTE16__CUSTOM_COLOURS_PAIRED_TWO_12__ID) && (palette_id < mPalette::PALETTELIST_DYNAMIC_CRGBPALETTE16_USER__LENGTH__ID)
  ){  

    
    _palette_container->CRGB16Palette16_Palette.encoded_index.clear();
    for(uint8_t i=0;i<16;i++){
      _palette_container->CRGB16Palette16_Palette.encoded_index.push_back(map(i, 0,15, 0, 255));
    }


    /**
     * @brief These pri/sec/ter should really be merged into my palettes as another encoded type, thus, removing colors[x] from palette.cpp
     **/
    switch(palette_id)
    {
      case mPalette::PALETTELIST_DYNAMIC_CRGBPALETTE16__RANDOMISE_COLOURS_01_RANDOM_HUE__ID: // Original WLED random
      {        
        uint32_t new_colour_rate_ms = 1000 + (((uint32_t)(255-SEGMENT_I(segment_index).intensity()))*100);
        // ALOG_INF(PSTR("new_colour_rate_ms=%d"),new_colour_rate_ms);
        if (millis() - SEGMENT_I(segment_index).params_internal.aux3 > new_colour_rate_ms)        
        {
          SEGMENT_I(segment_index).palette_container->CRGB16Palette16_Palette.data = CRGBPalette16(
                          CHSV(random8(), 255, random8(128, 255)),
                          CHSV(random8(), 255, random8(128, 255)),
                          CHSV(random8(), 192, random8(128, 255)),
                          CHSV(random8(), 255, random8(128, 255))
          );
          SEGMENT_I(segment_index).params_internal.aux3 = millis();
        }
      }
      break;
      case mPalette::PALETTELIST_DYNAMIC_CRGBPALETTE16__RANDOMISE_COLOURS_02_RANDOM_HUE_80TO100_SATURATIONS__ID: // Random Hue, Slight Random Saturation (80 to 100%) ie 200/255 is 80%
      {        
        uint32_t new_colour_rate_ms = 1000 + (((uint32_t)(255-SEGMENT_I(segment_index).intensity()))*100);
        // ALOG_INF(PSTR("new_colour_rate_ms=%d"),new_colour_rate_ms);
        if (millis() - SEGMENT_I(segment_index).params_internal.aux3 > new_colour_rate_ms)        
        {
          SEGMENT_I(segment_index).palette_container->CRGB16Palette16_Palette.data = CRGBPalette16(  // currentPalette needs moved into the segment? not palette, since each segment needs its own. 
                          CHSV(random8(), random8(204, 255), 255),
                          CHSV(random8(), random8(204, 255), 255),
                          CHSV(random8(), random8(204, 255), 255),
                          CHSV(random8(), random8(204, 255), 255)
          );                          
          SEGMENT_I(segment_index).params_internal.aux3 = millis();
        }
      }
      break;
      case mPalette::PALETTELIST_DYNAMIC_CRGBPALETTE16__RANDOMISE_COLOURS_03_RANDOM_HUE_60TO100_SATURATIONS__ID: // S60-S100%
      {        
        uint32_t new_colour_rate_ms = 1000 + (((uint32_t)(255-SEGMENT_I(segment_index).intensity()))*100);
        // ALOG_INF(PSTR("new_colour_rate_ms=%d"),new_colour_rate_ms);
        if (millis() - SEGMENT_I(segment_index).params_internal.aux3 > new_colour_rate_ms)        
        {
          SEGMENT_I(segment_index).palette_container->CRGB16Palette16_Palette.data = CRGBPalette16(
                          CHSV(random8(), random8(153, 255), 255),
                          CHSV(random8(), random8(153, 255), 255),
                          CHSV(random8(), random8(153, 255), 255),
                          CHSV(random8(), random8(153, 255), 255)
          );                          
          SEGMENT_I(segment_index).params_internal.aux3 = millis();
        }
      }
      break;
      case mPalette::PALETTELIST_DYNAMIC_CRGBPALETTE16__RANDOMISE_COLOURS_04_RANDOM_HUE_60TO85_SATURATIONS__ID: // S60-S85%
      {        
        uint32_t new_colour_rate_ms = 1000 + (((uint32_t)(255-SEGMENT_I(segment_index).intensity()))*100);
        // ALOG_INF(PSTR("new_colour_rate_ms=%d"),new_colour_rate_ms);
        if (millis() - SEGMENT_I(segment_index).params_internal.aux3 > new_colour_rate_ms)        
        {
          SEGMENT_I(segment_index).palette_container->CRGB16Palette16_Palette.data = CRGBPalette16(
                          CHSV(random8(), random8(153, 217), 255),
                          CHSV(random8(), random8(153, 217), 255),
                          CHSV(random8(), random8(153, 217), 255),
                          CHSV(random8(), random8(153, 217), 255)
          );                          
          SEGMENT_I(segment_index).params_internal.aux3 = millis();
        }
      }
      break;
      case mPalette::PALETTELIST_DYNAMIC_CRGBPALETTE16__RANDOMISE_COLOURS_05_RANDOM_HUE_00TO100_SATURATIONS__ID: // S0-S100%
      {        
        uint32_t new_colour_rate_ms = 1000 + (((uint32_t)(255-SEGMENT_I(segment_index).intensity()))*100);
        // ALOG_INF(PSTR("new_colour_rate_ms=%d"),new_colour_rate_ms);
        if (millis() - SEGMENT_I(segment_index).params_internal.aux3 > new_colour_rate_ms)        
        {
          SEGMENT_I(segment_index).palette_container->CRGB16Palette16_Palette.data = CRGBPalette16(
                          CHSV(random8(), random8(0, 255), 255),
                          CHSV(random8(), random8(0, 255), 255),
                          CHSV(random8(), random8(0, 255), 255),
                          CHSV(random8(), random8(0, 255), 255)
                          );
                          
          SEGMENT_I(segment_index).params_internal.aux3 = millis();
        }
      }
      break;    
      case mPalette::PALETTELIST_DYNAMIC_CRGBPALETTE16__CUSTOM_COLOURS_PAIRED_TWO_12__ID:
      { //primary + secondary
        CRGB prim = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[0]));
        CRGB sec  = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[1]));
        _palette_container->CRGB16Palette16_Palette.data = CRGBPalette16(prim,prim,sec,sec); 
      }
      break;
      case mPalette::PALETTELIST_DYNAMIC_CRGBPALETTE16__CUSTOM_COLOURS_PAIRED_THREE_123__ID:
      { //primary + secondary + tertiary
        CRGB prim = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[0]));
        CRGB sec  = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[1]));
        CRGB ter  = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[2]));
        SEGMENT_I(segment_index).palette_container->CRGB16Palette16_Palette.data = CRGBPalette16(prim,sec,ter); 
      }
      break;    
      case mPalette::PALETTELIST_DYNAMIC_CRGBPALETTE16__CUSTOM_COLOURS_PAIRED_FOUR_1234__ID:
      { //primary + secondary + tertiary
        CRGB prim = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[0]));
        CRGB sec  = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[1]));
        CRGB ter  = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[2]));
        CRGB four  = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[3]));
        SEGMENT_I(segment_index).palette_container->CRGB16Palette16_Palette.data = CRGBPalette16(prim,sec,ter,four); 
      }
      break;    
      case mPalette::PALETTELIST_DYNAMIC_CRGBPALETTE16__CUSTOM_COLOURS_PAIRED_FIVE_12345__ID:
      { //primary + secondary + tertiary
        CRGB prim = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[0]));
        CRGB sec  = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[1]));
        CRGB ter  = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[2]));
        CRGB four  = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[3]));
        CRGB five  = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[4]));
        SEGMENT_I(segment_index).palette_container->CRGB16Palette16_Palette.data = CRGBPalette16(prim,prim,prim, sec,sec,sec, ter,ter,ter, four,four,four, five,five,five,five); 
      }
      break;    
      case mPalette::PALETTELIST_DYNAMIC_CRGBPALETTE16__CUSTOM_COLOURS_PAIRED_REPEATED_ACTIVE__ID:
      { //primary + secondary + tertiary
        CRGB prim = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[0]));
        CRGB sec  = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[1]));
        CRGB ter  = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[2]));
        CRGB four  = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[3]));
        CRGB five  = col_to_crgb(RgbcctColor::GetU32Colour(SEGMENT_I(segment_index).rgbcctcolors[4]));
        SEGMENT_I(segment_index).palette_container->CRGB16Palette16_Palette.data = CRGBPalette16(prim,sec,ter,four,five, prim,sec,ter,four,five, prim,sec,ter,four,five, five); 
      }
      break;    
    } // END switch
    
  }

}


/**
 * @brief Gets palette names directly or from palette class when needed
 * 
 * @param palette_id 
 * @param buffer 
 * @param buflen 
 * @return const char* 
 */
const char* mAnimatorLight::GetPaletteNameByID(uint16_t palette_id, char* buffer, uint8_t buflen)
{

  // ALOG_INF(PSTR("GetPaletteNameByID %d"), palette_id);
  // Serial.flush();

  /**************************************************************
   * 
   * Dynamic User Defined Names
   * 
   * PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR__IDS
  ***************************************************************/


  if(
    ((palette_id >= mPalette::PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_01__ID) && (palette_id < mPalette::PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID))
  ){  
    int8_t adjusted_id = palette_id - mPalette::PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_01__ID;
    DLI->GetDeviceName_WithModuleUniqueID(GetModuleUniqueID(), adjusted_id, buffer, buflen);
  // ALOG_INF(PSTR("device name %s %d"),buffer,adjusted_id );
  }

  
  /**************************************************************
   * 
   * Dynamic User Defined Names
   * 
   * PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR__IDS
  ***************************************************************/
 else
  if(
    ((palette_id >= mPalette::PALETTELIST_LENGTH_OF_STATIC_IDS) && (palette_id < mPaletteI->GetPaletteListLength()))
  ){  
    int8_t adjusted_id = palette_id - mPalette::PALETTELIST_LENGTH_OF_STATIC_IDS + 5; //also skip the rgbcct colour names, though, they should be static?
    DLI->GetDeviceName_WithModuleUniqueID(GetModuleUniqueID(), adjusted_id, buffer, buflen);
    ALOG_INF(PSTR("device name %s %d"),buffer,adjusted_id );
  }

  else
    
  /**
   * @brief All static (progmem) palette names are stored in palette class
   * 
   * PALETTELIST_STATIC__IDS
   * PALETTELIST_SEGMENTS_STORED_VARIABLE_CRGBPALETTE16_PALETTES__IDS
   * PALETTELIST_STATIC_CRGBPALETTE16__IDS
   * PALETTELIST_CRGBPALETTE16_GRADIENT___PALETTES__IDS
   * PALETTELIST_STATIC_HTML_COLOUR_CODES__IDS
   */
  if(
    ((palette_id >= mPalette::PALETTELIST_DYNAMIC_CRGBPALETTE16__CUSTOM_COLOURS_PAIRED_TWO_12__ID) && (palette_id < mPalette::PALETTELIST_STATIC_LENGTH__ID))
  ){       
    mPaletteI->GetPaletteNameByID(palette_id, buffer, buflen);
  }

  return buffer;

}



/**
 * @brief Gets palette names directly or from palette class when needed
 * 
 * @param palette_id 
 * @param buffer 
 * @param buflen 
 * @return const char* 
 */
int16_t mAnimatorLight::GetPaletteIDbyName(char* buffer)
{

  // ALOG_INF(PSTR("GetPaletteNameByID %d"), palette_id);
  // Serial.flush();
  int16_t found_id = -1;

  /**************************************************************
   * Check variables first
   * Dynamic User Defined Names
   * 
   * PALETTELIST_VARIABLE_HSBID__IDS
   * PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR__IDS
   * PALETTELIST_VARIABLE_GENERIC__IDS
   * 
   * 
   * check the user defined range
   * 
  ***************************************************************/
  if((found_id = DLI->GetDeviceIDbyName(buffer, GetModuleUniqueID()))>=0)
  {
    ALOG_INF(PSTR("FOUND Var Name(%s) -> %d"), buffer, found_id);
  }
  /**
   * @brief All static (progmem) palette names are stored in palette class
   * 
   * PALETTELIST_STATIC__IDS
   * PALETTELIST_SEGMENTS_STORED_VARIABLE_CRGBPALETTE16_PALETTES__IDS
   * PALETTELIST_STATIC_CRGBPALETTE16__IDS
   * PALETTELIST_CRGBPALETTE16_GRADIENT___PALETTES__IDS
   * PALETTELIST_STATIC_HTML_COLOUR_CODES__IDS
   */
  else
  if((found_id=mPaletteI->Get_Static_PaletteIDbyName(buffer))>=0)
  {
    ALOG_INF(PSTR("FOUND STATIC Name(%s) -> %d"), buffer, found_id);
  }
  else
  {
    ALOG_INF(PSTR("ERROR NO PALETTE ID FOUND FOR Name(%s)"), buffer);

  }
  
  DEBUG_LINE_HERE;

  return found_id;

}


//get RGB values from color temperature in K (https://tannerhelland.com/2012/09/18/convert-temperature-rgb-algorithm-code.html)
void mAnimatorLight::colorKtoRGB(uint16_t kelvin, byte* rgb) //white spectrum to rgb, calc
{
  float r = 0, g = 0, b = 0;
  float temp = kelvin / 100;
  if (temp <= 66) {
    r = 255;
    g = round(99.4708025861 * log(temp) - 161.1195681661);
    if (temp <= 19) {
      b = 0;
    } else {
      b = round(138.5177312231 * log((temp - 10)) - 305.0447927307);
    }
  } else {
    r = round(329.698727446 * pow((temp - 60), -0.1332047592));
    g = round(288.1221695283 * pow((temp - 60), -0.0755148492));
    b = 255;
  } 
  //g += 12; //mod by Aircoookie, a bit less accurate but visibly less pinkish
  rgb[0] = (uint8_t) constrain(r, 0, 255);
  rgb[1] = (uint8_t) constrain(g, 0, 255);
  rgb[2] = (uint8_t) constrain(b, 0, 255);
  rgb[3] = 0;
}


// adjust RGB values based on color temperature in K (range [2800-10200]) (https://en.wikipedia.org/wiki/Color_balance)
uint32_t mAnimatorLight::colorBalanceFromKelvin(uint16_t kelvin, uint32_t rgb)
{
  //remember so that slow colorKtoRGB() doesn't have to run for every setPixelColor()
  if (lastKelvin != kelvin) colorKtoRGB(kelvin, correctionRGB);  // convert Kelvin to RGB
  lastKelvin = kelvin;
  byte rgbw[4];
  rgbw[0] = ((uint16_t) correctionRGB[0] * R(rgb)) /255; // correct R
  rgbw[1] = ((uint16_t) correctionRGB[1] * G(rgb)) /255; // correct G
  rgbw[2] = ((uint16_t) correctionRGB[2] * B(rgb)) /255; // correct B
  rgbw[3] =                                W(rgb);
  return RGBW32(rgbw[0],rgbw[1],rgbw[2],rgbw[3]);
}


//approximates a Kelvin color temperature from an RGB color.
//this does no check for the "whiteness" of the color,
//so should be used combined with a saturation check (as done by auto-white)
//values from http://www.vendian.org/mncharity/dir3/blackbody/UnstableURLs/bbr_color.html (10deg)
//equation spreadsheet at https://bit.ly/30RkHaN
//accuracy +-50K from 1900K up to 8000K
//minimum returned: 1900K, maximum returned: 10091K (range of 8192)
uint16_t mAnimatorLight::approximateKelvinFromRGB(uint32_t rgb) {
  //if not either red or blue is 255, color is dimmed. Scale up
  uint8_t r = R(rgb), b = B(rgb);
  if (r == b) return 6550; //red == blue at about 6600K (also can't go further if both R and B are 0)

  if (r > b) {
    //scale blue up as if red was at 255
    uint16_t scale = 0xFFFF / r; //get scale factor (range 257-65535)
    b = ((uint16_t)b * scale) >> 8;
    //For all temps K<6600 R is bigger than B (for full bri colors R=255)
    //-> Use 9 linear approximations for blackbody radiation blue values from 2000-6600K (blue is always 0 below 2000K)
    if (b < 33)  return 1900 + b       *6;
    if (b < 72)  return 2100 + (b-33)  *10;
    if (b < 101) return 2492 + (b-72)  *14;
    if (b < 132) return 2900 + (b-101) *16;
    if (b < 159) return 3398 + (b-132) *19;
    if (b < 186) return 3906 + (b-159) *22;
    if (b < 210) return 4500 + (b-186) *25;
    if (b < 230) return 5100 + (b-210) *30;
                 return 5700 + (b-230) *34;
  } else {
    //scale red up as if blue was at 255
    uint16_t scale = 0xFFFF / b; //get scale factor (range 257-65535)
    r = ((uint16_t)r * scale) >> 8;
    //For all temps K>6600 B is bigger than R (for full bri colors B=255)
    //-> Use 2 linear approximations for blackbody radiation red values from 6600-10091K (blue is always 0 below 2000K)
    if (r > 225) return 6600 + (254-r) *50;
    uint16_t k = 8080 + (225-r) *86;
    return (k > 10091) ? 10091 : k;
  }
}



void mAnimatorLight::Init_Segments()
{ 

  //reset segment runtimes
  ALOG_ERR(PSTR("Phase out: duplicate code"));
  
  //reset segment runtimes
  for (segment_new &seg : segments) 
  {
    // ALOG_ERR(PSTR("seg.markForReset();A"));  
    seg.markForReset();
    seg.resetIfRequired();
  }

  for (segment_new &seg : segments) 
  {    
    // ALOG_ERR(PSTR("seg.markForReset();B"));
    seg.grouping = 1;
    // SEGMENT_I(i).setOption(SEG_OPTION_ON, 1);
    // SEGMENT_I(i).opacity = 255;    
  }

  // #ifdef USE_MODULE_LIGHTS_PWM
  // SEGMENT_I(0).pixel_range.start = 0;
  // SEGMENT_I(0).pixel_range.stop = STRIP_SIZE_MAX; //since STRIP_SIZE_MAX is total pixels, the value in "stop" will also be STRIP_SIZE_MAX  
  // SEGMENT_I(0).pixel_range.stop = 2; 
  // ALOG_WRN(PSTR("Force fix the stop pixel size"));
  // #endif // USE_MODULE_LIGHTS_PWM
  
  
}


void mAnimatorLight::Segment_AppendNew(uint16_t start_pixel, uint16_t stop_pixel, uint8_t seg_index)
{

  ALOG_INF(PSTR("Segment_AppendNew:: getSegmentsNum() new index %d of count:%d"), seg_index, getSegmentsNum());

  if (seg_index >= getSegmentsNum()) 
  {

    ALOG_INF(PSTR("Segment_AppendNew::new seg_index %d > %d getSegmentsNum(): Creating new segment "), seg_index, getSegmentsNum());
    appendSegment(Segment_New(start_pixel, stop_pixel));
    seg_index = getSegmentsNum()-1; // segments are added at the end of list, -1 for index of LENGTH minus 1
    ALOG_INF(PSTR("Segment_AppendNew::new seg_index %d"), seg_index);
      
  }
  else
  {  
    ALOG_INF(PSTR("ELSEEEEEEEEEEEEEEEEEEEE Segment_AppendNew::new seg_index %d > %d getSegmentsNum(): Creating new segment "), seg_index, getSegmentsNum());
  }

}


/**
 * @brief Disables animator neopixel and configures so effect function will be called again
 * 
 * @param seg_i 
 */
void mAnimatorLight::SetSegment_AnimFunctionCallback_WithoutAnimator(uint8_t seg_i)
{
  SEGMENT_I(seg_i).anim_function_callback = nullptr; // disabled
  SEGMENT_I(seg_i).transitional = true;              // Set that basic manual animator will be called
  SEGMENT_I(seg_i).transition.rate_ms = FRAMETIME;
}



void mAnimatorLight:: addEffect3(uint8_t id, RequiredFunction function, const char *name, const char* effect_config)
{

  // if (id == 255) { // find empty slot
  //   for (size_t i=1; i<_mode.size(); i++) if (_modeData[i] == _data_RESERVED) { id = i; break; }
  // }


  if (id < effects.function.size()) {
    // if (_modeData[id] != _data_RESERVED) return; // do not overwrite alerady added effect
    effects.function[id]     = function;
    effects.data[id] = name;
    effects.data_config[id] = name;
  } else {
    effects.function.push_back(function);
    effects.data.push_back(name);
    effects.data_config.push_back(effect_config);
    if (effects.count < effects.function.size()) effects.count++;
  }
  
}


void mAnimatorLight::SubTask_Segments_Effects()
{

  #ifdef ENABLE_DEVFEATURE_LIGHT__REQUIRE_MINIMUM_BACKOFF_ON_ANIMATION_TO_MAYBE_FIX_BUS_FLICKER
  // Need to also have a general backoff timer so the outputs are never hit faster than neopixelbus can update
  if(!mTime::TimeReached(&tSaved_MinimumAnimateRunTime, MINIMUM_SHOW_BACKOFF_PERIOD_MS)){ return; } // Note: Override must still abide by this backoff
  #endif // ENABLE_DEVFEATURE_LIGHT__REQUIRE_MINIMUM_BACKOFF_ON_ANIMATION_TO_MAYBE_FIX_BUS_FLICKER


  #ifdef ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
  // pCONT_iLight->neopixel_runner->IsBusy();
  // vTaskSuspend(pCONT_iLight->neopixel_runner->_commit_task);
  pCONT_iLight->neopixel_runner->flag_block_show = true;
  #endif // ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE


  _isServicing = true;
  _segment_index_primary = 0;
  for (segment_new &seg : segments) 
  {
    // #ifdef DEBUG_TARGET_ANIMATOR_SEGMENTS
      // AddLog(LOG_LEVEL_DEBUG, PSTR("_segments[%d].isActive()=%d"),_segment_index_primary,seg.isActive());
      // AddLog(LOG_LEVEL_DEBUG, PSTR("_segments[%d].istart/stop=%d %d"),_segment_index_primary,_segments[_segment_index_primary].pixel_range.start,_segments[_segment_index_primary].pixel_range.stop);
    // #endif

    // reset the segment runtime data if needed, called before isActive to ensure deleted segment's buffers are cleared
    seg.resetIfRequired();

    if (!seg.isActive()) continue;    

    if(
      (mTime::TimeReached(&seg.tSaved_AnimateRunTime, seg.get_transition_rate_ms())) ||
      (seg.flags.fForceUpdate)
    ){

      DEBUG_PIN1_SET(LOW);

      if (seg.grouping == 0) seg.grouping = 1; //sanity check == move this into wherever it gets used (ie struct functions)
    
      ALOG_DBM(PSTR("\n\r\n\rseg=%d,rate=%d,%d"),_segment_index_primary, seg.transition.rate_ms, seg.flags.fForceUpdate);

      // if (!seg.freeze) { //only run effect function if not frozen

      /**
       * @brief Add flag that checks if animation is still running from previous call before we start another, 
       * hence, detect when frame_rate is not being met (time_ms not finishing before rate_ms)
       * */

      if(seg.flags.fForceUpdate){ 
        seg.flags.fForceUpdate = false;
        seg.tSaved_AnimateRunTime = millis(); // Not reset inside TimeReached when fForceUpdate is triggered
      }

      // DEBUG_LINE_HERE;
      
      #ifdef ENABLE_DEVFEATURE_ALWAYS_LOAD_PALETTE_WHEN_NOT_TRANSITIONING
      // if(!seg.transitional)  // Needs better integration before it can be used
      // {
        // Moved so load on change now when asked to get
        LoadPalette(seg.palette.id, _segment_index_primary);
      // }
      #else
        #warning "disabled LoadPalette"
      #endif // ENABLE_DEVFEATURE_ALWAYS_LOAD_PALETTE_WHEN_NOT_TRANSITIONING

      
      // DEBUG_LINE_HERE;
      // This maybe cant be global and needs placed inside each effect?
      // Not valid for long term, needs to be changed
      Set_Segment_ColourType(_segment_index_primary, pCONT_set->Settings.light_settings.type);
      
      /**
       * @brief To be safe, always clear and if the effect has a callback, it will be activated inside its function again.
      **/
      #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
      setCallback_ConstructJSONBody_Debug_Animations_Progress(nullptr); // clear to be reset
      #endif

      _virtualSegmentLength = seg.virtualLength();
      _now = millis(); // internal millis used for animation to make calculations work correctly (instead of calling millis lots)

      ALOG_DBM( PSTR("_segments[%d].effect_id=%d \t%d"),_segment_index_primary, seg.effect_id, millis()); 

      if(seg.effect_id < effects.function.size())
      {
        (this->*effects.function[seg.effect_id])(); // Call Effect Function (passes and returns nothing)
      }

      /**
       * @brief Only calls Animator if effects are not directly handled
       **/
      if(seg.anim_function_callback == nullptr) // assumes direct update
      {
        pCONT_iLight->ShowInterface();    // I cant do doShow here, since direct method (no callback) needs to happen now, whereas AnimUpdate will need to call show later. So call them when needed (though, still worth working out a better/unified show/solution)     
        seg.flags.animator_first_run = false; // CHANGE to function: reset here for all WLED methods
        ALOG_DBM(PSTR("Calling"));
      }
      else
      { 
        StartSegmentAnimation_AsAnimUpdateMemberFunction(_segment_index_primary);   
        // First run must be reset after StartAnimation is first called 
      }
              
      seg.call++; // Used as progress counter for animations eg rainbow across all hues

      DEBUG_PIN1_SET(HIGH);
                
    }//end if update reached

    /**
     * @brief Now iter forward on active segment
     **/
    _segment_index_primary++;
  
  } // END for

  
  #ifdef ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
  // pCONT_iLight->neopixel_runner->IsBusy();
  // vTaskResume(pCONT_iLight->neopixel_runner->_commit_task);
  pCONT_iLight->neopixel_runner->flag_block_show = false;
  #endif // ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE


} // SubTask_Effects_PhaseOut


/**
 * @brief New allocated buffers must contain colour info
 * 
 * @param param 
 */
void mAnimatorLight::AnimationProcess_LinearBlend_Dynamic_Buffer(const AnimationParam& param)
{    
  
  DEBUG_PIN6_SET(0);

  RgbcctColor updatedColor;
  TransitionColourPairs colour_pairs;

  for (uint16_t pixel = 0; 
                pixel < SEGMENT.virtualLength();
                pixel++
  ){  
    
  // DEBUG_PIN1_SET(0);
    GetTransitionColourBuffer(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type, &colour_pairs);
  // DEBUG_PIN1_SET(1);

  // DEBUG_PIN2_SET(0);
    updatedColor = RgbcctColor::LinearBlend(colour_pairs.StartingColour, colour_pairs.DesiredColour, param.progress); 
  // DEBUG_PIN2_SET(1); 

    // AddLog(LOG_LEVEL_TEST, PSTR("SI%d,seg_len%d, RGB[%d] %d,%d,%d,%d,%d"),_segment_index_primary,SEGMENT.virtualLength(),  pixel, updatedColor.R, updatedColor.G, updatedColor.B, updatedColor.W1, updatedColor.W2);

  // DEBUG_PIN3_SET(0);
    SEGMENT.SetPixelColor(pixel, updatedColor);
  // DEBUG_PIN3_SET(1);

  }
  
  DEBUG_PIN6_SET(1);


}


/**
 * @brief New allocated buffers must contain colour info
 * @brief Replicate one colour across whole string (ie scences for h801 or addressable)
 * 
 * @param param 
 */
void mAnimatorLight::AnimationProcess_SingleColour_LinearBlend_Dynamic_Buffer(const AnimationParam& param)
{    

  RgbcctColor updatedColor = RgbcctColor(0);
  TransitionColourPairs colour_pairs;
  GetTransitionColourBuffer(SEGMENT.Data(), SEGMENT.DataLength(), 0, SEGMENT.colour_type, &colour_pairs);
  
  updatedColor = RgbcctColor::LinearBlend(colour_pairs.StartingColour, colour_pairs.DesiredColour, param.progress);    
  
  ALOG_DBM( PSTR("SEGMENT.colour_type=%d, seg%d, desired_colour1=%d,%d,%d,%d,%d"),SEGMENT.colour_type, getCurrSegmentId(), updatedColor.R,updatedColor.G,updatedColor.B,updatedColor.WC,updatedColor.WW);

  // AddLog(LOG_LEVEL_TEST, PSTR("Acolour_pairs.StartingColour=%d,%d,%d,%d,%d"),colour_pairs.StartingColour.R,colour_pairs.StartingColour.G,colour_pairs.StartingColour.B,colour_pairs.StartingColour.WC,colour_pairs.StartingColour.WW);
  // AddLog(LOG_LEVEL_TEST, PSTR("Acolour_pairs.DesiredColour=%d,%d,%d,%d,%d"),colour_pairs.DesiredColour.R,colour_pairs.DesiredColour.G,colour_pairs.DesiredColour.B,colour_pairs.DesiredColour.WC,colour_pairs.DesiredColour.WW);
    
  for (uint16_t pixel = 0; 
                pixel < SEGLEN; 
                pixel++
  ){  
    // ALOG_INF(PSTR("SEGMENT.pixel =%d"), pixel);
    // updatedColor.setChannelsRaw(255,0,0,1,2);
    SEGMENT.SetPixelColor(pixel, updatedColor, false);
  }
  
}


/**
 * @brief New allocated buffers must contain colour info
 * @brief Replicate one colour across whole string (ie scences for h801 or addressable)
 * 
 * Using 4 and 5 of rgbcctcolors, which will be set by animator effect
 * 
 * @param param 
 */
void mAnimatorLight::AnimationProcess_SingleColour_LinearBlend_Between_RgbcctSegColours(const AnimationParam& param)
{    
  
  RgbcctColor updatedColor = RgbcctColor::LinearBlend(SEGMENT.rgbcctcolors[4].GetColourWithBrightnessApplied(), SEGMENT.rgbcctcolors[5].GetColourWithBrightnessApplied(), param.progress);    
  
  // ALOG_INF( PSTR("SEGMENT.colour_type=%d, desired_colour1=%d,%d,%d,%d,%d"),SEGMENT.colour_type,updatedColor.R,updatedColor.G,updatedColor.B,updatedColor.WC,updatedColor.WW);

  // AddLog(LOG_LEVEL_TEST, PSTR("Acolour_pairs.StartingColour=%d,%d,%d,%d,%d"),colour_pairs.StartingColour.R,colour_pairs.StartingColour.G,colour_pairs.StartingColour.B,colour_pairs.StartingColour.WC,colour_pairs.StartingColour.WW);
  // AddLog(LOG_LEVEL_TEST, PSTR("Acolour_pairs.DesiredColour=%d,%d,%d,%d,%d"),colour_pairs.DesiredColour.R,colour_pairs.DesiredColour.G,colour_pairs.DesiredColour.B,colour_pairs.DesiredColour.WC,colour_pairs.DesiredColour.WW);
    
  for (uint16_t pixel = 0; 
                pixel < SEGLEN; 
                pixel++
  ){  
    // ALOG_INF(PSTR("SEGMENT.pixel =%d"), pixel);
    SEGMENT.SetPixelColor(pixel, updatedColor, false);
  }
  
}


RgbcctColor mAnimatorLight::GetSegmentColour(uint8_t colour_index, uint8_t segment_index)
{
  return SEGMENT_I(segment_index).rgbcctcolors[colour_index];
}


void mAnimatorLight::Set_Segment_ColourType(uint8_t segment_index, uint8_t light_type)
{

  // switch(light_type)
  // {
  //   case LT_PWM5:
  //     SEGMENT_I(segment_index).colour_type = RgbcctColor::ColourType::COLOUR_TYPE__RGBCCT__ID; 
  //   break;
  //   case LT_ADDRESSABLE_SK6812:
  //     SEGMENT_I(segment_index).colour_type = RgbcctColor::ColourType::COLOUR_TYPE__RGBW__ID; 
  //   break;
  //   default:
  //   case LT_ADDRESSABLE_WS281X:
  //     SEGMENT_I(segment_index).colour_type = RgbcctColor::ColourType::COLOUR_TYPE__RGB__ID;
  //   break;
  // }

}


uint8_t mAnimatorLight::GetSizeOfPixel(RgbcctColor::ColourType colour_type)
{
  switch(colour_type)
  {
    case RgbcctColor::ColourType::COLOUR_TYPE__RGB__ID:     return 3;
    case RgbcctColor::ColourType::COLOUR_TYPE__RGBW__ID:    
    // case RgbcctColor::ColourType::LIGHT_TYPE_RGBC__ID:      
    return 4;
    case RgbcctColor::ColourType::COLOUR_TYPE__RGBCCT__ID:  return 5;
  }

}

void mAnimatorLight::SetTransitionColourBuffer_StartingColour(
  byte* buffer, 
  uint16_t buflen, 
  uint16_t pixel_index, 
  RgbcctColor::ColourType pixel_type, 
  RgbcctColor starting_colour
){

  if(buffer == nullptr)
  {
    return;
  }

  // note right now it is storing the full rgbcct value
  uint16_t pixel_start_i = 0;

  // AddLog(LOG_LEVEL_TEST, PSTR("pixel_start_i/buflen=%d\t%d"),pixel_start_i,buflen);
  switch(pixel_type)
  {
    case RgbcctColor::ColourType::COLOUR_TYPE__RGB__ID:
    
      pixel_start_i = pixel_index*6;

      if(pixel_start_i + 6 <= buflen)
      {
        byte* start_of_pixel_pair = &buffer[pixel_start_i];
        start_of_pixel_pair[0] = starting_colour.R;
        start_of_pixel_pair[1] = starting_colour.G;
        start_of_pixel_pair[2] = starting_colour.B;
      }

    break;
    case RgbcctColor::ColourType::COLOUR_TYPE__RGBW__ID:
    
      pixel_start_i = pixel_index*8;

      if(pixel_start_i + 8 <= buflen)
      {
        byte* start_of_pixel_pair = &buffer[pixel_start_i];
        start_of_pixel_pair[0] = starting_colour.R;
        start_of_pixel_pair[1] = starting_colour.G;
        start_of_pixel_pair[2] = starting_colour.B;
        start_of_pixel_pair[3] = starting_colour.W1;        
      }

    break;
    case RgbcctColor::ColourType::COLOUR_TYPE__RGBCCT__ID:
    
      pixel_start_i = pixel_index*10;

      if(pixel_start_i + 10 <= buflen)
      {
        byte* start_of_pixel_pair = &buffer[pixel_start_i];
        start_of_pixel_pair[0] = starting_colour.R;
        start_of_pixel_pair[1] = starting_colour.G;
        start_of_pixel_pair[2] = starting_colour.B;
        start_of_pixel_pair[3] = starting_colour.WW;
        start_of_pixel_pair[4] = starting_colour.WC;
      }

    break;
  }

  // uint16_t pixel_start_i =pixel_index*sizeof(TransitionColourPairs);
  //   // AddLog(LOG_LEVEL_TEST, PSTR("pixel_start_i/buflen=%d\t%d"),pixel_start_i,buflen); 
  // // AddLog_Array(LOG_LEVEL_TEST, PSTR("buffer"), &buffer[pixel_start_i    ] , 10);

  // // AddLog(LOG_LEVEL_TEST, PSTR("SET colour_pair[%d] = %d,%d,%d, %d,%d,%d"), 
  // //   pixel_index,
  // //   colour_pair->DesiredColour.R,
  // //   colour_pair->DesiredColour.G,
  // //   colour_pair->DesiredColour.B,
  // //   colour_pair->StartingColour.R,
  // //   colour_pair->StartingColour.G,
  // //   colour_pair->StartingColour.B
  // // );

}




uint8_t mAnimatorLight::GetNumberOfColoursInPalette(uint16_t palette_id)
{

  uint8_t palette_colour_count = 0;

  // Pass pointer to memory location to load, so I can have additional palettes. If none passed, assume primary storage of segment

  // ALOG_INF(PSTR("============LoadPalette %d %d %d"), palette_id, segment_index, pCONT_lAni->_segment_index_primary);

  /**
   * @brief PaletteList Vectors should have the length stored in it. Actual pixel count depends on encoding type
   **/
  if(
    ((palette_id >= mPalette::PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID) && (palette_id < mPalette::PALETTELIST_STATIC_LENGTH__ID))
    //  ||
    // ((palette_id >= mPalette::PALETTELIST_VARIABLE_HSBID_01__ID)    && (palette_id < mPalette::PALETTELIST_VARIABLE_HSBID_LENGTH__ID)) ||
    // ((palette_id >= mPalette::PALETTELIST_LENGTH_OF_STATIC_IDS)  && (palette_id < mPaletteI->GetPaletteListLength()))
  ){   

    uint16_t encoded_colour_width = 0;

    uint16_t palette_id_adj = palette_id - mPalette::PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID;
    mPalette::STATIC_PALETTE pal = mPaletteI->static_palettes[constrain(palette_id_adj,0,mPaletteI->static_palettes.size()-1)];

    // Serial.println(ptr->encoding.data, BIN);

    if(pal.encoding.red_enabled){ encoded_colour_width++; }
    if(pal.encoding.green_enabled){ encoded_colour_width++; }
    if(pal.encoding.blue_enabled){ encoded_colour_width++; }
    if(pal.encoding.white_warm_enabled){ encoded_colour_width++; }

    if(pal.encoding.white_cold_enabled){ encoded_colour_width++; }
    if(pal.encoding.encoded_value_byte_width){ encoded_colour_width += pal.encoding.encoded_value_byte_width; }

    // if(pal.encoding.index_exact){ encoded_colour_width++; }
    if(pal.encoding.index_gradient){ encoded_colour_width++; }
    if(pal.encoding.index_is_trigger_value_exact){ encoded_colour_width++; }
    if(pal.encoding.index_is_trigger_value_scaled100){ encoded_colour_width++; }
    
    // if(pal.encoding.encoded_as_hsb_ids){ encoded_colour_width++; }
    if(pal.encoding.encoded_as_crgb_palette_16){ encoded_colour_width++; }
    if(pal.encoding.encoded_as_crgb_palette_256){ encoded_colour_width++; }
    if(pal.encoding.palette_can_be_modified){ encoded_colour_width++; }


    if(encoded_colour_width==0)
    {
      ALOG_ERR(PSTR("encoded_colour_width==0, crash errorAA =%S"), pal.friendly_name_ctr);
      return palette_colour_count;
    }
  
    palette_colour_count = pal.data_length/encoded_colour_width; 
 
  }
  else
  if(
    (palette_id >= mPalette::PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_01__ID) && (palette_id < mPalette::PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID)
  ){
    palette_colour_count = 1;
  }
  else
  if(
    (palette_id >= mPalette::PALETTELIST_DYNAMIC_CRGBPALETTE16__RANDOMISE_COLOURS_01_RANDOM_HUE__ID) && (palette_id < mPalette::PALETTELIST_DYNAMIC_CRGBPALETTE16_USER__LENGTH__ID)
  ){  
    palette_colour_count = 16;    
  }
  else
  if(
    (palette_id >= mPalette::PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID) && (palette_id < mPalette::PALETTELIST_STATIC_SINGLE_COLOUR__LENGTH__ID)
  ){  
    palette_colour_count = 1;    
  }
  else
  if(
    ((palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID) && (palette_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID)) ||
    ((palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID)    && (palette_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID))
  ){   
    palette_colour_count = 16;
  }else
  if(
    (palette_id >= mPalette::PALETTELIST_LENGTH_OF_STATIC_IDS) && (palette_id < mPaletteI->GetPaletteListLength())
  ){  

    uint16_t palette_adjusted_id = palette_id - mPalette::PALETTELIST_LENGTH_OF_STATIC_IDS; // adjust back into correct indexing

    uint8_t colour_width  = mPaletteI->GetEncodedColourWidth(mPaletteI->custom_palettes[palette_adjusted_id].encoding); 
    palette_colour_count = mPaletteI->custom_palettes[palette_adjusted_id].data.size()/colour_width;

  }

  return palette_colour_count;

}


/*
 * Fills segment with color
 */
void mAnimatorLight::fill(uint32_t c, bool apply_brightness) 
{
  for(uint16_t i = 0; i < _virtualSegmentLength; i++) 
  {
    SEGMENT.SetPixelColor(i, c, apply_brightness);
  }
}

void mAnimatorLight::fill(RgbcctColor c, bool apply_brightness) 
{
  for(uint16_t i = 0; i < _virtualSegmentLength; i++) 
  {
    SEGMENT.SetPixelColor(i, c, apply_brightness);
  }
}


void mAnimatorLight::fill_ranged(uint32_t c, bool apply_brightness) 
{

  for(uint16_t i = SEGMENT.pixel_range.start; i <= SEGMENT.pixel_range.stop; i++) {
    SEGMENT.SetPixelColor(i, c, apply_brightness);
  }

}

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Fireworks
 * @note : Converted from WLED Effects
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
uint32_t mAnimatorLight::crgb_to_col(CRGB fastled)
{
  uint32_t colour = 0; // suppress White colour (MSB)
  colour = (((uint32_t)fastled.red << 16) | ((uint32_t)fastled.green << 8) | fastled.blue);
  return colour;
}


CRGB mAnimatorLight::col_to_crgb(uint32_t color)
{
  CRGB fastled_col;
  fastled_col.red =   (color >> 16 & 0xFF);
  fastled_col.green = (color >> 8  & 0xFF);
  fastled_col.blue =  (color       & 0xFF);
  return fastled_col;
}

/*
 * blurs segment content, source: FastLED colorutils.cpp
 */
void mAnimatorLight::blur(uint8_t blur_amount, bool set_brightness)
{
  uint8_t keep = 255 - blur_amount;
  uint8_t seep = blur_amount >> 1;
  CRGB carryover = HTMLColorCode::Black;

  for(uint16_t i = 0; i < _virtualSegmentLength; i++)
  {

    CRGB cur = col_to_crgb( RgbcctColor::GetU32Colour(SEGMENT.GetPixelColor(i)));
    CRGB part = cur;
    part.nscale8(seep);
    cur.nscale8(keep);
    cur += carryover;
    if(i > 0) {
      uint32_t c = RgbcctColor::GetU32Colour(SEGMENT.GetPixelColor(i-1));
      uint8_t r = (c >> 16 & 0xFF);
      uint8_t g = (c >> 8  & 0xFF);
      uint8_t b = (c       & 0xFF);
      SEGMENT.SetPixelColor(i-1, RgbColor(qadd8(r, part.red), qadd8(g, part.green), qadd8(b, part.blue)), set_brightness);
    }
    SEGMENT.SetPixelColor(i, RgbColor(cur.red, cur.green, cur.blue), set_brightness);
    carryover = part;
  }

  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
    
}


/*
 * fade out function, higher rate = quicker fade
 Adding this helper function temporarily until I convert it to linearblend methods
 */
void mAnimatorLight::fade_out(uint8_t rate, bool set_brightness) 
{
  rate = (255-rate) >> 1;
  float mappedRate = float(rate) +1.1;

  uint32_t color = RgbcctColor::GetU32Colour(SEGMENT.rgbcctcolors[1]); // target color

  int w2 = (color >> 24) & 0xff;
  int r2 = (color >> 16) & 0xff;
  int g2 = (color >>  8) & 0xff;
  int b2 =  color        & 0xff;

  // r2 = 255;

  for(uint16_t i = 0; i < _virtualSegmentLength; i++) {
    color = RgbcctColor::GetU32Colour(SEGMENT.GetPixelColor(i));
    int w1 = (color >> 24) & 0xff;
    int r1 = (color >> 16) & 0xff;
    int g1 = (color >>  8) & 0xff;
    int b1 =  color        & 0xff;

    int wdelta = (w2 - w1) / mappedRate;  //linearblend
    int rdelta = (r2 - r1) / mappedRate;
    int gdelta = (g2 - g1) / mappedRate;
    int bdelta = (b2 - b1) / mappedRate;

    // if fade isn't complete, make sure delta is at least 1 (fixes rounding issues)
    wdelta += (w2 == w1) ? 0 : (w2 > w1) ? 1 : -1;
    rdelta += (r2 == r1) ? 0 : (r2 > r1) ? 1 : -1;
    gdelta += (g2 == g1) ? 0 : (g2 > g1) ? 1 : -1;
    bdelta += (b2 == b1) ? 0 : (b2 > b1) ? 1 : -1;

    SEGMENT.SetPixelColor(i, RgbwColor(r1 + rdelta, g1 + gdelta, b1 + bdelta, w1 + wdelta), set_brightness);
  }

  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}


void mAnimatorLight::SetTransitionColourBuffer_DesiredColour(byte* buffer, uint16_t buflen, uint16_t pixel_index, RgbcctColor::ColourType pixel_type, RgbcctColor desired_colour)
{   
  
  // #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
  //ALOG_INF( PSTR("EFFECT sIndexIO=I%d start%d,stop%d\tp%d,PL %d, RGB:%d,%d,%d"), SEGIDX, SEGMENT.pixel_range.start, SEGMENT.pixel_range.stop, pixel, mPaletteI->GetNumberOfColoursInPalette(SEGMENT.palette.id), colour.R, colour.G, colour.B );
  // #endif

  if(buffer == nullptr)
  {
    return;
  } 

  // note right now it is storing the full rgbcct value
  uint16_t pixel_start_i = 0;
  byte* start_of_pixel_pair = nullptr;

  #ifdef ENABLE_DEVFEATURE_PIXEL_MATRIX
  AddLog(LOG_LEVEL_TEST, PSTR("[%d] \t pixel_start_i/buflen=%d\t%d"),pixel_index, pixel_start_i,buflen);
  #endif 

  switch(pixel_type)
  {
    case RgbcctColor::ColourType::COLOUR_TYPE__RGB__ID:
    
      pixel_start_i = pixel_index*6;

      if(pixel_start_i + 6 <= buflen)
      {
        start_of_pixel_pair = &buffer[pixel_start_i];
        start_of_pixel_pair[3] = desired_colour.R;
        start_of_pixel_pair[4] = desired_colour.G;
        start_of_pixel_pair[5] = desired_colour.B;
      }

    break;
    case RgbcctColor::ColourType::COLOUR_TYPE__RGBW__ID:
    
      pixel_start_i = pixel_index*8; // rgbw * 2

      if(pixel_start_i + 8 <= buflen)
      {
        start_of_pixel_pair = &buffer[pixel_start_i];
        start_of_pixel_pair[4] = desired_colour.R;
        start_of_pixel_pair[5] = desired_colour.G;
        start_of_pixel_pair[6] = desired_colour.B;
        start_of_pixel_pair[7] = desired_colour.W1;
      }

    break;
    case RgbcctColor::ColourType::COLOUR_TYPE__RGBCCT__ID:
    
      pixel_start_i = pixel_index*10;

      if(pixel_start_i + 10 <= buflen)
      {
        start_of_pixel_pair = &buffer[pixel_start_i];
        start_of_pixel_pair[5] = desired_colour.R;
        start_of_pixel_pair[6] = desired_colour.G;
        start_of_pixel_pair[7] = desired_colour.B;
        start_of_pixel_pair[8] = desired_colour.WW;
        start_of_pixel_pair[9] = desired_colour.WC;
      }

    break;
    default: return;
  }

  // AddLog_Array(LOG_LEVEL_TEST, PSTR("buffer"), &buffer[pixel_start_i    ] , 10);
  // AddLog(LOG_LEVEL_TEST, PSTR("SET colour_pair[%d] = %d,%d,%d, %d,%d,%d"), 
  //   pixel_index,
  //   colour_pair->DesiredColour.R,
  //   colour_pair->DesiredColour.G,
  //   colour_pair->DesiredColour.B,
  //   colour_pair->StartingColour.R,
  //   colour_pair->StartingColour.G,
  //   colour_pair->StartingColour.B
  // );

}


/**
 * @brief 
 * 
 * @param allocated_buffer 
 * @param pixel_index 
 * @param pixel_type 
 * @param starting_colour 
 * @param desired_colour 
 * @return true succesfully added to buffer
 * @return false 
 */

  /**
buffer = [length_of_pixel_type * length_of_pixels * copies_of_that_pixel]

length_of_pixel_type 
     rgb = 3
     rgbw = 4
     rgbcct = 5
copies_of_that_pixel 
    ie starting and desired, so 2
length_of_pixels
    number of pixel in that segment up to (100?) then replicate and wrap output*/
bool mAnimatorLight::SetTransitionColourBuffer(byte* buffer, uint16_t buflen, uint16_t pixel_index, RgbcctColor::ColourType pixel_type, 
RgbcctColor starting_colour, RgbcctColor desired_colour)
{

  if(buffer == nullptr)
  {
    #ifdef ENABLE_LOG_LEVEL_ERROR
    AddLog(LOG_LEVEL_TEST, PSTR("buffer == nullptr"));
    #endif // ENABLE_LOG_LEVEL_INFO
    return false;
  } 

// note right now it is storing the full rgbcct value

  uint16_t pixel_start_i =pixel_index*sizeof(TransitionColourPairs);


    // AddLog(LOG_LEVEL_TEST, PSTR("pixel_start_i/buflen=%d\t%d"),pixel_start_i,buflen);

  if(pixel_start_i + 10 <= buflen)
  {
    byte* start_of_pixel_pair = &buffer[pixel_start_i];
    start_of_pixel_pair[0] = starting_colour.R;
    start_of_pixel_pair[1] = starting_colour.G;
    start_of_pixel_pair[2] = starting_colour.B;
    start_of_pixel_pair[3] = starting_colour.WW;
    start_of_pixel_pair[4] = starting_colour.WC;
    start_of_pixel_pair[5] = desired_colour.R;
    start_of_pixel_pair[6] = desired_colour.G;
    start_of_pixel_pair[7] = desired_colour.B;
    start_of_pixel_pair[8] = desired_colour.WW;
    start_of_pixel_pair[9] = desired_colour.WC;
  }else{
    
    #ifdef ENABLE_LOG_LEVEL_WARN
    AddLog(LOG_LEVEL_TEST, PSTR("NOT enough memory"));
    #endif // ENABLE_LOG_LEVEL_INFO
  }
  
  // AddLog_Array(LOG_LEVEL_TEST, PSTR("buffer"), &buffer[pixel_start_i    ] , 10);
  // AddLog(LOG_LEVEL_TEST, PSTR("SET colour_pair[%d] = %d,%d,%d, %d,%d,%d"), 
  //   pixel_index,
  //   colour_pair->DesiredColour.R,
  //   colour_pair->DesiredColour.G,
  //   colour_pair->DesiredColour.B,
  //   colour_pair->StartingColour.R,
  //   colour_pair->StartingColour.G,
  //   colour_pair->StartingColour.B
  // );
    
  return true;

}


/**
 * @brief 
 * New method for getting colours from dynamic buffers
 * 
 * @param ptr 
 * @param pixel_num 
 * @param pixel_position 
 * @return RgbcctColor 
 */
// mAnimatorLight::TransitionColourPairs* 
void IRAM_ATTR mAnimatorLight::GetTransitionColourBuffer(
  byte* buffer, 
  uint16_t buflen, 
  uint16_t pixel_index, 
  RgbcctColor::ColourType pixel_type, 
  mAnimatorLight::TransitionColourPairs* colour  // Get and Set colours will still only store in rgb or rgbw format, for now, limited to rgb
){


  if(buffer == nullptr)
  {
    return;
  }  

uint16_t pixel_start_i = 0;

//   uint16_t pixel_pair_index_location_in_buffer = pixel_index*sizeof(TransitionColourPairs);

// // AddLog(LOG_LEVEL_TEST, PSTR("GetTransitionColourBuffer=%d \t%d"),pixel_index, pixel_pair_index_location_in_buffer );
// AddLog(LOG_LEVEL_TEST, PSTR("pixel_type=%d"),pixel_type );

    byte* c = nullptr;
//   byte* start_of_pair = &buffer[pixel_pair_index_location_in_buffer];

  // AddLog_Array(LOG_LEVEL_TEST, PSTR("Abuffer"), &start_of_pair[0] , 10);

  switch(pixel_type)
  {
    default: return;
    case RgbcctColor::ColourType::COLOUR_TYPE__RGB__ID:
      pixel_start_i = pixel_index*6;
      c = &buffer[pixel_start_i];
      colour->StartingColour = RgbColor(c[0], c[1], c[2]);
      colour->DesiredColour  = RgbColor(c[3], c[4], c[5]);
      break;
    case RgbcctColor::ColourType::COLOUR_TYPE__RGBW__ID:
      pixel_start_i = pixel_index*8;
      c = &buffer[pixel_start_i];
      colour->StartingColour = RgbwColor(c[0], c[1], c[2], c[3]);
      colour->DesiredColour  = RgbwColor(c[4], c[5], c[6], c[7]);
      break;
    case RgbcctColor::ColourType::COLOUR_TYPE__RGBCCT__ID:
      pixel_start_i = pixel_index*10;
      c = &buffer[pixel_start_i];
      colour->StartingColour = RgbcctColor(c[0], c[1], c[2], c[3], c[4]);
      colour->DesiredColour  = RgbcctColor(c[5], c[6], c[7], c[8], c[9]);
    break;
  }

// AddLog(LOG_LEVEL_TEST, PSTR("colour->StartingColour=%d,%d,%d,%d,%d"),colour->StartingColour.R,colour->StartingColour.G,colour->StartingColour.B,colour->StartingColour.WC,colour->StartingColour.WW);
// AddLog(LOG_LEVEL_TEST, PSTR("colour->DesiredColour=%d,%d,%d,%d,%d"),colour->DesiredColour.R,colour->DesiredColour.G,colour->DesiredColour.B,colour->DesiredColour.WC,colour->DesiredColour.WW);

      
  // AddLog_Array(LOG_LEVEL_TEST, PSTR("Abuffer"), &start_of_pair[pixel_pair_index_location_in_buffer    ] , 10);

  // AddLog(LOG_LEVEL_TEST, PSTR("GET DesiredColour[%d] = %d,%d,%d,%d,%d"), 
  //   pixel_index,
  //   colour->DesiredColour.R,
  //   colour->DesiredColour.G,
  //   colour->DesiredColour.B,
  //   colour->DesiredColour.WW,
  //   colour->DesiredColour.WC
  // );


  return ;


} // end function






mAnimatorLight& mAnimatorLight::SetSegment_AnimFunctionCallback(uint8_t segment_index, ANIM_FUNCTION_SIGNATURE)
{
  this->SEGMENT_I(segment_index).anim_function_callback = anim_function_callback;
  SEGMENT_I(segment_index).animation_has_anim_callback = true;
  return *this;
}


/**
 * @note The indexing here relates to the buffer storage, and should NOT be confused with pixel indexing
 * 
 * This is retrieving the colour from the neopixelbuffer, so index within segment needs to know the start_index to work out pixel_index from neopixel buffer
 * ie Segment index 10, could be neopixel_index 20 for a segment start_pixel index of 10
 */
void mAnimatorLight::DynamicBuffer_Segments_UpdateStartingColourWithGetPixel()
{
  for(int pixel=0;
          pixel<SEGMENT.virtualLength();
          pixel++
  ){
    SetTransitionColourBuffer_StartingColour( SEGMENT.Data(), 
                                              SEGMENT.DataLength(),
                                              pixel, 
                                              SEGMENT.colour_type, 
                                              SEGMENT.GetPixelColor(pixel)
                                            );
  }

}

void mAnimatorLight::DynamicBuffer_Segments_UpdateStartingColourWithGetPixel_WithFade(uint8_t fade)
{
  RgbcctColor colour;
  for(int pixel=0;
          pixel<SEGMENT.virtualLength();
          pixel++
  ){
    colour = SEGMENT.GetPixelColor(pixel);
    colour.Fade(fade);
    SetTransitionColourBuffer_StartingColour( SEGMENT.Data(), 
                                              SEGMENT.DataLength(),
                                              pixel, 
                                              SEGMENT.colour_type, 
                                              colour
                                            );
  }

}


/**
 * Begin at "StartingColor" at 0% then return to "DesiredColor" at 100%
 * */
void mAnimatorLight::StartSegmentAnimation_AsAnimUpdateMemberFunction(uint8_t segment_index)
{ 
  
  #ifdef ENABLE_LOG_LEVEL_DEBUG
  //AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::StartAnimationAsBlendFromStartingColorToDesiredColor"));
  #endif

  // pCONT_iLight->runtime.animation_changed_millis = millis();

  uint16_t time_tmp = 0;    
  time_tmp = SEGMENT_I(segment_index).transition.time_ms;//    SEGMENT_I(segment_index).transition.time_ms; 

  /**
   * @brief Before starting animation, check the override states to see if they are active
   * 
   */
  // Overwriting single SEGMENT_I(0) methods, set, then clear
  if(SEGMENT_I(segment_index).single_animation_override.time_ms>0)
  {
    time_tmp = SEGMENT_I(segment_index).single_animation_override.time_ms;
    SEGMENT_I(segment_index).single_animation_override.time_ms = 0; // reset overwrite
    ALOG_INF(PSTR("Override: TimeMs: %d"), time_tmp);
  }

  // Serial.printf("TRANSITION_METHOD_INSTANT_ID = %d\n\r",time_tmp);

  if(time_tmp>0){
    if(NEO_ANIMATION_TIMEBASE == NEO_CENTISECONDS){
      time_tmp /= 1000;// ms to seconds
    }
  }

  if(SEGMENT_I(segment_index).animation_has_anim_callback == true)
  {
    SEGMENT_I(segment_index).animator->StartAnimation(0, time_tmp, SEGMENT_I(segment_index).anim_function_callback);
  }

  SEGMENT_I(segment_index).transitional = true;


} //end function


/*
 * Returns a new, random wheel index with a minimum distance of 42 from pos.
 */
uint8_t mAnimatorLight::get_random_wheel_index(uint8_t pos) {
  uint8_t r = 0, x = 0, y = 0, d = 0;

  while(d < 42) {
    r = random8();
    x = abs(pos - r);
    y = 255 - x;
    d = MIN(x, y);
  }
  return r;
}


/********************************************************************************************************************************
**********Flasher Function ******************************************************************************************************
********************************************************************************************************************************/
void mAnimatorLight::CommandSet_Flasher_FunctionID(uint8_t value, uint8_t segment_index ){
  
  if(SEGMENT_I(segment_index).effect_id != value)
  {
    SEGMENT_I(segment_index).markForReset();
    SEGMENT_I(segment_index).effect_id = value;      //make function "changeFlasherFunction" so then the region is automatically updated internally
  }
  SEGMENT_I(segment_index).params_internal.aux0 = EFFECTS_REGION_COLOUR_SELECT_ID;
  SEGMENT_I(segment_index).flags.animator_first_run= true; // first run, so do extra things

  ALOG_DBM(PSTR("segments[segment_index].effect_id=%d"),segments[segment_index].effect_id);
  
  #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
  setCallback_ConstructJSONBody_Debug_Animations_Progress(nullptr); // clear to be reset
  #endif
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  char buffer[30];
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_JSON_COMMAND_SVALUE_SVALUE_K(D_JSON_EFFECTS, D_JSON_FUNCTION)), GetFlasherFunctionName(buffer, sizeof(buffer)));
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}


int8_t mAnimatorLight::GetFlasherFunctionIDbyName(const char* f)
{
      
  if(f==0) return -2;

  for(uint16_t id=0;id<effects.function.size();id++)
  {
    if(mSupport::CheckCommand_P(f, effects.data[id]))
    {   
      ALOG_INF(PSTR("GetFlasherFunctionIDbyName %s %d"),f,id);
      return id; 
    }
  }

  return -1;

}


const char* mAnimatorLight::GetFlasherFunctionName(char* buffer, uint8_t buflen, uint8_t segment_index )
{
  return GetFlasherFunctionNamebyID(SEGMENT_I(segment_index).effect_id, buffer, buflen);
}


const char* mAnimatorLight::GetFlasherFunctionNamebyID(uint8_t id, char* buffer, uint8_t buflen)
{
  if(id<getEffectsAmount()){
    snprintf_P(buffer, buflen, effects.data[id]);  
  }else{
    snprintf(buffer, buflen, "id_err %d", id);  
  }
  return buffer;

}


/**
 * @brief Blends between Rgbcct with 0 to 255 range
 * 
 * @param color1 
 * @param color2 
 * @param blend 
 * @return RgbcctColor 
 */
RgbcctColor mAnimatorLight::ColourBlend(RgbcctColor color1, RgbcctColor color2, uint8_t blend) 
{
  return RgbcctColor::LinearBlend(color1, color2, mSupport::mapfloat(blend, 0,255, 0.0f, 1.0f));
}

uint32_t mAnimatorLight::ColourBlend(uint32_t color1, uint32_t color2, uint8_t blend) 
{
  if(blend == 0)   return color1;
  if(blend == 255) return color2;

  uint32_t w1 = (color1 >> 24) & 0xff;
  uint32_t r1 = (color1 >> 16) & 0xff;
  uint32_t g1 = (color1 >>  8) & 0xff;
  uint32_t b1 =  color1        & 0xff;

  uint32_t w2 = (color2 >> 24) & 0xff;
  uint32_t r2 = (color2 >> 16) & 0xff;
  uint32_t g2 = (color2 >>  8) & 0xff;
  uint32_t b2 =  color2        & 0xff;

  uint32_t w3 = ((w2 * blend) + (w1 * (255 - blend))) >> 8;
  uint32_t r3 = ((r2 * blend) + (r1 * (255 - blend))) >> 8;
  uint32_t g3 = ((g2 * blend) + (g1 * (255 - blend))) >> 8;
  uint32_t b3 = ((b2 * blend) + (b1 * (255 - blend))) >> 8;

  return ((w3 << 24) | (r3 << 16) | (g3 << 8) | (b3));
}


void mAnimatorLight::CommandSet_Flasher_UpdateColourRegion_RefreshSecs(uint8_t value, uint8_t segment_index)
{
  // flashersettings_segments.update_colour_region.refresh_secs = value; 
  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_JSON_COLOUR_REFRESH_RATE)), flashersettings_segments.update_colour_region.refresh_secs);
  // #endif // ENABLE_LOG_LEVEL_COMMANDS
}


///////////////////////////////////////////////////////////////////////////////
// Segment_New class implementation
///////////////////////////////////////////////////////////////////////////////
uint16_t mAnimatorLight::Segment_New::_usedSegmentData = 0U; // amount of RAM all segments use for their data[]
CRGB    *mAnimatorLight::Segment_New::_globalLeds = nullptr;
uint16_t mAnimatorLight::Segment_New::maxWidth = DEFAULT_LED_COUNT;
uint16_t mAnimatorLight::Segment_New::maxHeight = 1;

// copy constructor
// mAnimatorLight::Segment_New(const mAnimatorLight::Segment_New &orig) {
//   //DEBUG_PRINTLN(F("-- Copy segment constructor --"));
//   memcpy(this, &orig, sizeof(mAnimatorLight::Segment_New));
//   name = nullptr;
//   data = nullptr;
//   _dataLen = 0;
//   _t = nullptr;
//   if (leds && !mAnimatorLight::Segment_New::_globalLeds) leds = nullptr;
//   if (orig.name) { name = new char[strlen(orig.name)+1]; if (name) strcpy(name, orig.name); }
//   if (orig.data) { if (allocateData(orig._dataLen)) memcpy(data, orig.data, orig._dataLen); }
//   if (orig._t)   { _t = new Transition(orig._t->_dur, orig._t->_briT, orig._t->_cctT, orig._t->_colorT); }
//   if (orig.leds && !mAnimatorLight::Segment_New::_globalLeds) { leds = (CRGB*)malloc(sizeof(CRGB)*length()); if (leds) memcpy(leds, orig.leds, sizeof(CRGB)*length()); }
// }

// // move constructor
// mAnimatorLight::Segment_New(mAnimatorLight::Segment_New &&orig) noexcept {
//   //DEBUG_PRINTLN(F("-- Move segment constructor --"));
//   memcpy(this, &orig, sizeof(mAnimatorLight::Segment_New));
//   orig.name = nullptr;
//   orig.data = nullptr;
//   orig._dataLen = 0;
//   orig._t   = nullptr;
//   orig.leds = nullptr;
// }

// copy assignment
mAnimatorLight::Segment_New& mAnimatorLight::Segment_New::operator= (const mAnimatorLight::Segment_New &orig) {
  
      DEBUG_LINE_HERE;
  //DEBUG_PRINTLN(F("-- Copying segment --"));
  if (this != &orig) {
    // clean destination
    if (name) delete[] name;
    if (_t)   delete _t;
    if (leds && !mAnimatorLight::Segment_New::_globalLeds) free(leds);
    deallocateData();
    // copy source
    memcpy(this, &orig, sizeof(mAnimatorLight::Segment_New));
    // erase pointers to allocated data
    name = nullptr;
    data = nullptr;
    _dataLen = 0;
    _t = nullptr;
    if (!mAnimatorLight::Segment_New::_globalLeds) leds = nullptr;
    // copy source data
    if (orig.name) { name = new char[strlen(orig.name)+1]; if (name) strcpy(name, orig.name); }
    if (orig.data) { if (allocateData(orig._dataLen)) memcpy(data, orig.data, orig._dataLen); }
    if (orig._t)   { _t = new Transition(orig._t->_dur, orig._t->_briT, orig._t->_cctT, orig._t->_colorT); }
    if (orig.leds && !mAnimatorLight::Segment_New::_globalLeds) { leds = (CRGB*)malloc(sizeof(CRGB)*length()); if (leds) memcpy(leds, orig.leds, sizeof(CRGB)*length()); }
  }
      DEBUG_LINE_HERE;
  return *this;
}

// move assignment
mAnimatorLight::Segment_New& mAnimatorLight::Segment_New::operator= (mAnimatorLight::Segment_New &&orig) noexcept {
  
      DEBUG_LINE_HERE;
  //DEBUG_PRINTLN(F("-- Moving segment --"));
  if (this != &orig) {
    if (name) delete[] name; // free old name
    deallocateData(); // free old runtime data
    if (_t) delete _t;
    if (leds && !mAnimatorLight::Segment_New::_globalLeds) free(leds);
    memcpy(this, &orig, sizeof(mAnimatorLight::Segment_New));
    orig.name = nullptr;
    orig.data = nullptr;
    orig._dataLen = 0;
    orig._t   = nullptr;
    orig.leds = nullptr;
  }
      DEBUG_LINE_HERE;
  return *this;
}

bool mAnimatorLight::Segment_New::allocateData(size_t len) {
      // DEBUG_LINE_HERE;
  if (data && _dataLen == len) return true; //already allocated
  deallocateData();
  if (mAnimatorLight::Segment_New::getUsedSegmentData() + len > MAX_SEGMENT_DATA){ 
    ALOG_ERR( PM_JSON_MEMORY_INSUFFICIENT ); // This is the base case, none will be fallback
    effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID;
    return false; //not enough memory
  }
  // if possible use SPI RAM on ESP32
  #if defined(ARDUINO_ARCH_ESP32) && defined(WLED_USE_PSRAM)
  if (psramFound())
    data = (byte*) ps_malloc(len);
  else
  #endif
  
  data = (byte*) malloc(len);

  if (!data){
    ALOG_ERR( PM_JSON_MEMORY_INSUFFICIENT ); // This is the base case, none will be fallback
    effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID;
    return false; //allocation failed
  }
  // Serial.printf("NEW_MEMORY of %d for segment index %d\n\r\n\r\n\r\n\r", len, palette.id ); delay(2000);
  addUsedSegmentData(len);
  _dataLen = len;
  memset(data, 0, len);
      DEBUG_LINE_HERE;
  return true;
}

void mAnimatorLight::Segment_New::deallocateData()
{
      // DEBUG_LINE_HERE;
  if (!data) return;
      // DEBUG_LINE_HERE;
  free(data);
  data = nullptr;
      // DEBUG_LINE_HERE;
  addUsedSegmentData(-_dataLen);
      // DEBUG_LINE_HERE;
  _dataLen = 0;
}

/** 
  * If reset of this segment was requested, clears runtime
  * settings of this segment.
  * Must not be called while an effect mode function is running
  * because it could access the data buffer and this method 
  * may free that data buffer.
  */
void mAnimatorLight::Segment_New::resetIfRequired() {
  if (reset) {
    
  // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_PIXEL "resetIfRequired AuxOptions Segment = %d,%d,%d,%d"),
  //   aux0,
  //   aux1,
  //   aux2,
  //   aux3
  // );

    if (leds && !mAnimatorLight::Segment_New::_globalLeds) { free(leds); leds = nullptr; }
    //if (_t) { delete _t; _t = nullptr; transitional = false; }
    next_time = 0; step = 0; call = 0; 
    
    /**
     * @brief Potential issue with WLED effects, but removing aux options from reset since they may be used as config options
     * 
     */
    params_internal.aux0 = 0; params_internal.aux1 = 0;  params_internal.aux2 = 0;  params_internal.aux3 = 0; 
    
    
    reset = false; // setOption(SEG_OPTION_RESET, false);
    // Serial.println(DEBUG_INSERT_PAGE_BREAK "mAnimatorLight::Segment_New::resetIfRequired()"); //delay(5000);
  }
}

void mAnimatorLight::Segment_New::setUpLeds() {
  // deallocation happens in resetIfRequired() as it is called when segment changes or in destructor
  if (mAnimatorLight::Segment_New::_globalLeds)
    #ifndef WLED_DISABLE_2D
    leds = &mAnimatorLight::Segment_New::_globalLeds[pixel_range.start + startY*mAnimatorLight::Segment_New::maxWidth];
    #else
    leds = &mAnimatorLight::Segment_New::_globalLeds[pixel_range.start];
    #endif
  else if (!leds) {
    #if defined(ARDUINO_ARCH_ESP32) && defined(WLED_USE_PSRAM)
    if (psramFound())
      leds = (CRGB*)ps_malloc(sizeof(CRGB)*length());
    else
    #endif
      leds = (CRGB*)malloc(sizeof(CRGB)*length());
  }
}


void mAnimatorLight::Segment_New::setUp(uint16_t i1, uint16_t i2, uint8_t grp, uint8_t spc, uint16_t ofs, uint16_t i1Y, uint16_t i2Y) {
  //return if neither bounds nor grouping have changed
  bool boundsUnchanged = (pixel_range.start == i1 && pixel_range.stop == i2);
  #ifndef WLED_DISABLE_2D
  if (Segment_New::maxHeight>1) boundsUnchanged &= (startY == i1Y && stopY == i2Y); // 2D
  #endif
  if (boundsUnchanged
      && (!grp || (grouping == grp && spacing == spc))
      && (ofs == UINT16_MAX || ofs == offset)) return;

  if (pixel_range.stop) fill(BLACK); //turn old segment range off
  if (i2 <= i1) { //disable segment
    pixel_range.stop = 0;
    markForReset();
    return;
  }
  if (i1 < Segment_New::maxWidth || (i1 >= Segment_New::maxWidth*Segment_New::maxHeight && i1 < pCONT_lAni->getLengthTotal())) pixel_range.start = i1; // Segment_New::maxWidth equals strip.getLengthTotal() for 1D
  pixel_range.stop = i2 > Segment_New::maxWidth*Segment_New::maxHeight ? MIN(i2,pCONT_lAni->getLengthTotal()) : (i2 > Segment_New::maxWidth ? Segment_New::maxWidth : MAX(1,i2));
  startY = 0;
  stopY  = 1;
  #ifndef WLED_DISABLE_2D
  if (Segment_New::maxHeight>1) { // 2D
    if (i1Y < Segment_New::maxHeight) startY = i1Y;
    stopY = i2Y > Segment_New::maxHeight ? Segment_New::maxHeight : MAX(1,i2Y);
  }
  #endif
  if (grp) {
    grouping = grp;
    spacing = spc;
  }
  if (ofs < UINT16_MAX) offset = ofs;
  markForReset();
  if (!boundsUnchanged) refreshLightCapabilities();
}




bool mAnimatorLight::Segment_New::setColor(uint8_t slot, uint32_t c) { //returns true if changed


  rgbcctcolors[slot] = RgbcctColor::GetRgbcctFromU32Colour(c); 

  // if (slot >= NUM_COLORS || c == colors[slot]) return false;
  // if (fadeTransition) startTransition(strip.getTransition()); // start transition prior to change
  // colors[slot] = c;
  // stateChanged = true; // send UDP/WS broadcast
  return true;
}

bool mAnimatorLight::Segment_New::setColor(uint8_t slot, RgbcctColor c) { //returns true if changed


  rgbcctcolors[slot] = c;

  // if (slot >= NUM_COLORS || c == colors[slot]) return false;
  // if (fadeTransition) startTransition(strip.getTransition()); // start transition prior to change
  // colors[slot] = c;
  // stateChanged = true; // send UDP/WS broadcast
  return true;
}

void mAnimatorLight::Segment_New::setCCT(uint16_t k) {
  // if (k > 255) { //kelvin value, convert to 0-255
  //   if (k < 1900)  k = 1900;
  //   if (k > 10091) k = 10091;
  //   k = (k - 1900) >> 5;
  // }
  // if (cct == k) return;
  // if (fadeTransition) startTransition(strip.getTransition()); // start transition prior to change
  // cct = k;
  // stateChanged = true; // send UDP/WS broadcast
}

void mAnimatorLight::Segment_New::setOpacity(uint8_t o) {
  if (opacity == o) return;
  // if (fadeTransition) startTransition(strip.getTransition()); // start transition prior to change
  opacity = o;
  // stateChanged = true; // send UDP/WS broadcast
  _brightness_rgb = o;
  _brightness_cct = o;
}

void mAnimatorLight::Segment_New::setOption(uint8_t n, bool val) {
  bool prevOn = on;
  // if (fadeTransition && n == SEG_OPTION_ON && val != prevOn) startTransition(strip.getTransition()); // start transition prior to change
  if (val) options |=   0x01 << n;
  else     options &= ~(0x01 << n);
  // if (!(n == SEG_OPTION_SELECTED || n == SEG_OPTION_RESET || n == SEG_OPTION_TRANSITIONAL)) stateChanged = true; // send UDP/WS broadcast
}

void mAnimatorLight::Segment_New::setMode(uint8_t fx, bool loadDefaults) 
{
  // if we have a valid mode & is not reserved
  // if (fx < pCONT_lAni->getModeCount() && strncmp_P("RSVD", pCONT_lAni->getModeData(fx), 4)) {
  //   if (fx != mode) {
  //     startTransition(strip.getTransition()); // set effect transitions
      //markForReset(); // transition will handle this
      effect_id = fx;

    ALOG_INF(PSTR("setMode These are likely where the parser takes the defaults from effects %d"), effect_id);


      // load default values from effect string
      // if (loadDefaults) {

        int16_t sOpt = -1;

        if ((sOpt = pCONT_lAni->extractModeDefaults(fx, "sx")) >= 0) 
        {
          ALOG_INF(PSTR("Loading default sx=%d"), sOpt);
          set_speed(sOpt);
        }

        if ((sOpt = pCONT_lAni->extractModeDefaults(fx, "ix")) >= 0) 
        {
          ALOG_INF(PSTR("Loading default ix=%d"), sOpt);
          set_intensity(sOpt);
        }

        if ((sOpt = pCONT_lAni->extractModeDefaults(fx, "pal")) >= 0) 
        {
          ALOG_INF(PSTR("Loading default pal=%d"), sOpt);
          setPalette(sOpt);
        }

        if ((sOpt = pCONT_lAni->extractModeDefaults(fx, "ra")) >= 0) 
        {
          ALOG_INF(PSTR("Loading default ra=%d"), sOpt);
          transition.rate_ms = sOpt;
        }

        if ((sOpt = pCONT_lAni->extractModeDefaults(fx, "ti")) >= 0) 
        {
          ALOG_INF(PSTR("Loading default ti=%d"), sOpt);
          transition.time_ms = sOpt;
        }






        // sOpt = extractModeDefaults(fx, "c1");   if (sOpt >= 0) custom1   = sOpt;
        // sOpt = extractModeDefaults(fx, "c2");   if (sOpt >= 0) custom2   = sOpt;
        // sOpt = extractModeDefaults(fx, "c3");   if (sOpt >= 0) custom3   = sOpt;
        // sOpt = extractModeDefaults(fx, "m12");  if (sOpt >= 0) map1D2D   = constrain(sOpt, 0, 7);
        // sOpt = extractModeDefaults(fx, "si");   if (sOpt >= 0) soundSim  = constrain(sOpt, 0, 7);
        // sOpt = extractModeDefaults(fx, "rev");  if (sOpt >= 0) reverse   = (bool)sOpt;
        // sOpt = extractModeDefaults(fx, "mi");   if (sOpt >= 0) mirror    = (bool)sOpt; // NOTE: setting this option is a risky business
        // sOpt = extractModeDefaults(fx, "rY");   if (sOpt >= 0) reverse_y = (bool)sOpt;
        // sOpt = extractModeDefaults(fx, "mY");   if (sOpt >= 0) mirror_y  = (bool)sOpt; // NOTE: setting this option is a risky business
        
      // }



      // stateChanged = true; // send UDP/WS broadcast
    // }
  // }
}




// extracts mode parameter defaults from last section of mode data (e.g. "Juggle@!,Trail;!,!,;!;sx=16,ix=240,1d")
//;sx=16,ix=240,1d
int16_t mAnimatorLight::extractModeDefaults(uint8_t mode, const char *segVar)
{



  if (mode < getModeCount()) {
    char lineBuffer[128] = "";
    strncpy_P(lineBuffer, getModeData_Config(mode), 127);
    lineBuffer[127] = '\0'; // terminate string


    // ALOG_INF(PSTR("if (mode < getModeCount()) %d < %d %s"), mode, getModeCount(), lineBuffer);

    if (lineBuffer[0] != 0) {
      char* startPtr = strrchr(lineBuffer, ';'); // last ";" in FX data
      if (!startPtr) return -1;

      char* stopPtr = strstr(startPtr, segVar);
      if (!stopPtr) return -1;

      stopPtr += strlen(segVar) +1; // skip "="
      return atoi(stopPtr);
    }
  }
  return -1;
}


void mAnimatorLight::Segment_New::setPalette(uint8_t pal) 
{
  palette.id = pal;  
  ALOG_INF(PSTR("setPalette(%d)"), palette.id);
}


uint8_t mAnimatorLight::Segment_New::getBrightnessRGB_WithGlobalApplied()
{

  #ifdef ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL
  

  uint8_t brightness_RGB_segment = _brightness_rgb;
  uint8_t brightness_RGB_global = pCONT_iLight->getBriRGB_Global();
  uint8_t brightness_total = scale8(brightness_RGB_segment,  brightness_RGB_global); // rescale with global brightness

  ALOG_DBM(PSTR("mAnimatorLight::Segment_New::getBrightnessRGB() s%d g%d -> u%d"), brightness_RGB_segment, brightness_RGB_global, brightness_total);
 
  return brightness_total;

  #endif

  return _brightness_rgb;
}


uint8_t mAnimatorLight::Segment_New::getBrightnessCCT_WithGlobalApplied()
{
  #ifdef ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL
  

  uint8_t brightness_CCT_segment = _brightness_cct;
  uint8_t brightness_CCT_global = pCONT_iLight->getBriCCT_Global();
  uint8_t brightness_total = scale8(brightness_CCT_segment,  brightness_CCT_global);


  ALOG_DBM(PSTR("mAnimatorLight::Segment_New::getBrightnessCCT() s%d g%d -> u%d"), brightness_CCT_segment, brightness_CCT_global, brightness_total);

  return brightness_total;

  #endif

  return _brightness_cct;
}


// 2D matrix
uint16_t mAnimatorLight::Segment_New::virtualWidth() const {
  uint16_t groupLen = groupLength();
  uint16_t vWidth = ((transpose ? height() : width()) + groupLen - 1) / groupLen;
  if (mirror) vWidth = (vWidth + 1) /2;  // divide by 2 if mirror, leave at least a single LED
  return vWidth;
}

uint16_t mAnimatorLight::Segment_New::virtualHeight() const {
  uint16_t groupLen = groupLength();
  uint16_t vHeight = ((transpose ? width() : height()) + groupLen - 1) / groupLen;
  if (mirror_y) vHeight = (vHeight + 1) /2;  // divide by 2 if mirror, leave at least a single LED
  return vHeight;
}

uint16_t mAnimatorLight::Segment_New::nrOfVStrips() const {
  uint16_t vLen = 1;
#ifndef WLED_DISABLE_2D
  if (is2D()) {
    switch (map1D2D) {
      case M12_pBar:
        vLen = virtualWidth();
        break;
    }
  }
#endif
  return vLen;
}

// 1D strip
uint16_t mAnimatorLight::Segment_New::virtualLength() const {
#ifndef WLED_DISABLE_2D
  if (is2D()) {
    uint16_t vW = virtualWidth();
    uint16_t vH = virtualHeight();
    uint16_t vLen = vW * vH; // use all pixels from segment
    switch (map1D2D) {
      case M12_pBar:
        vLen = vH;
        break;
      case M12_pCorner:
      case M12_pArc:
        vLen = max(vW,vH); // get the longest dimension
        break;
    }
    return vLen;
  }
#endif
  uint16_t groupLen = groupLength();
  uint16_t vLength = (length() + groupLen - 1) / groupLen;
  if (mirror) vLength = (vLength + 1) /2;  // divide by 2 if mirror, leave at least a single LED
  return vLength;
}

void IRAM_ATTR mAnimatorLight::Segment_New::setPixelColor(int i, uint32_t col)
{
//   int vStrip = i>>16; // hack to allow running on virtual strips (2D segment columns/rows)
//   i &= 0xFFFF;

// Serial.println("ERROR UNSUPPORTED mAnimatorLight::Segment_New::setPixelColor");

SetPixelColor(i,col);

//   if (i >= virtualLength() || i<0) return;  // if pixel would fall out of segment just exit

// #ifndef WLED_DISABLE_2D
//   if (is2D()) {
//     uint16_t vH = virtualHeight();  // segment height in logical pixels
//     uint16_t vW = virtualWidth();
//     switch (map1D2D) {
//       case M12_Pixels:
//         // use all available pixels as a long strip
//         setPixelColorXY(i % vW, i / vW, col);
//         break;
//       case M12_pBar:
//         // expand 1D effect vertically or have it play on virtual strips
//         if (vStrip>0) setPixelColorXY(vStrip - 1, vH - i - 1, col);
//         else          for (int x = 0; x < vW; x++) setPixelColorXY(x, vH - i - 1, col);
//         break;
//       case M12_pArc:
//         // expand in circular fashion from center
//         if (i==0)
//           setPixelColorXY(0, 0, col);
//         else {
//           float step = HALF_PI / (2.85f*i);
//           for (float rad = 0.0f; rad <= HALF_PI+step/2; rad += step) {
//             // may want to try float version as well (with or without antialiasing)
//             int x = roundf(sin_t(rad) * i);
//             int y = roundf(cos_t(rad) * i);
//             setPixelColorXY(x, y, col);
//           }
//         }
//         break;
//       case M12_pCorner:
//         for (int x = 0; x <= i; x++) setPixelColorXY(x, i, col);
//         for (int y = 0; y <  i; y++) setPixelColorXY(i, y, col);
//         break;
//     }
//     return;
//   } else if (mAnimatorLight::Segment_New::maxHeight!=1 && (width()==1 || height()==1)) {
//     // we have a vertical or horizontal 1D segment (WARNING: virtual...() may be transposed)
//     int x = 0, y = 0;
//     if (virtualHeight()>1) y = i;
//     if (virtualWidth() >1) x = i;
//     setPixelColorXY(x, y, col);
//     return;
//   }
// #endif

//   if (leds) leds[i] = col;

//   uint16_t len = length();
//   uint8_t _bri_t = currentBri(on ? opacity : 0);
//   if (_bri_t < 255) {
//     byte r = scale8(R(col), _bri_t);
//     byte g = scale8(G(col), _bri_t);
//     byte b = scale8(B(col), _bri_t);
//     byte w = scale8(W(col), _bri_t);
//     col = RGBW32(r, g, b, w);
//   }

//   // expand pixel (taking into account start, grouping, spacing [and offset])
//   i = i * groupLength();
//   if (reverse) { // is segment reversed?
//     if (mirror) { // is segment mirrored?
//       i = (len - 1) / 2 - i;  //only need to index half the pixels
//     } else {
//       i = (len - 1) - i;
//     }
//   }
//   i += start; // starting pixel in a group

//   // set all the pixels in the group
//   for (int j = 0; j < grouping; j++) {
//     uint16_t indexSet = i + ((reverse) ? -j : j);
//     if (indexSet >= start && indexSet < stop) {
//       if (mirror) { //set the corresponding mirrored pixel
//         uint16_t indexMir = stop - indexSet + start - 1;          
//         indexMir += offset; // offset/phase
//         if (indexMir >= stop) indexMir -= len; // wrap
//         strip.setPixelColor(indexMir, col);
//       }
//       indexSet += offset; // offset/phase
//       if (indexSet >= stop) indexSet -= len; // wrap
//       strip.setPixelColor(indexSet, col);
//     }
//   }
}

// anti-aliased normalized version of setPixelColor()
void mAnimatorLight::Segment_New::setPixelColor(float i, uint32_t col, bool aa)
{

  SetPixelColor(i, col);

  // int vStrip = int(i/10.0f); // hack to allow running on virtual strips (2D segment columns/rows)
  // i -= int(i);

  // if (i<0.0f || i>1.0f) return; // not normalized

  // float fC = i * (virtualLength()-1);
  // if (aa) {
  //   uint16_t iL = roundf(fC-0.49f);
  //   uint16_t iR = roundf(fC+0.49f);
  //   float    dL = (fC - iL)*(fC - iL);
  //   float    dR = (iR - fC)*(iR - fC);
  //   uint32_t cIL = getPixelColor(iL | (vStrip<<16));
  //   uint32_t cIR = getPixelColor(iR | (vStrip<<16));
  //   if (iR!=iL) {
  //     // blend L pixel
  //     cIL = ColourBlend(col, cIL, uint8_t(dL*255.0f));
  //     setPixelColor(iL | (vStrip<<16), cIL);
  //     // blend R pixel
  //     cIR = ColourBlend(col, cIR, uint8_t(dR*255.0f));
  //     setPixelColor(iR | (vStrip<<16), cIR);
  //   } else {
  //     // exact match (x & y land on a pixel)
  //     setPixelColor(iL | (vStrip<<16), col);
  //   }
  // } else {
  //   setPixelColor(uint16_t(roundf(fC)) | (vStrip<<16), col);
  // }
}

uint32_t mAnimatorLight::Segment_New::getPixelColor(int i)
{

  return GetPixelColor(i).getU32();

//   int vStrip = i>>16;
//   i &= 0xFFFF;

// #ifndef WLED_DISABLE_2D
//   if (is2D()) {
//     uint16_t vH = virtualHeight();  // segment height in logical pixels
//     uint16_t vW = virtualWidth();
//     switch (map1D2D) {
//       case M12_Pixels:
//         return getPixelColorXY(i % vW, i / vW);
//         break;
//       case M12_pBar:
//         if (vStrip>0) return getPixelColorXY(vStrip - 1, vH - i -1);
//         else          return getPixelColorXY(0, vH - i -1);
//         break;
//       case M12_pArc:
//       case M12_pCorner:
//         // use longest dimension
//         return vW>vH ? getPixelColorXY(i, 0) : getPixelColorXY(0, i);
//         break;
//     }
//     return 0;
//   }
// #endif

//   if (leds) return RGBW32(leds[i].r, leds[i].g, leds[i].b, 0);

//   if (reverse) i = virtualLength() - i - 1;
//   i *= groupLength();
//   i += start;
//   /* offset/phase */
//   i += offset;
//   if (i >= stop) i -= length();
//   return strip.getPixelColor(i);
}

uint8_t mAnimatorLight::Segment_New::differs(mAnimatorLight::Segment_New& b) const {
  // uint8_t d = 0;
  // if (start != b.start)         d |= SEG_DIFFERS_BOUNDS;
  // if (stop != b.stop)           d |= SEG_DIFFERS_BOUNDS;
  // if (offset != b.offset)       d |= SEG_DIFFERS_GSO;
  // if (grouping != b.grouping)   d |= SEG_DIFFERS_GSO;
  // if (spacing != b.spacing)     d |= SEG_DIFFERS_GSO;
  // if (opacity != b.opacity)     d |= SEG_DIFFERS_BRI;
  // if (mode != b.mode)           d |= SEG_DIFFERS_FX;
  // if (speed != b.speed)         d |= SEG_DIFFERS_FX;
  // if (intensity != b.intensity) d |= SEG_DIFFERS_FX;
  // if (palette != b.palette)     d |= SEG_DIFFERS_FX;
  // if (custom1 != b.custom1)     d |= SEG_DIFFERS_FX;
  // if (custom2 != b.custom2)     d |= SEG_DIFFERS_FX;
  // if (custom3 != b.custom3)     d |= SEG_DIFFERS_FX;
  // if (startY != b.startY)       d |= SEG_DIFFERS_BOUNDS;
  // if (stopY != b.stopY)         d |= SEG_DIFFERS_BOUNDS;

  // //bit pattern: (msb first) sound:3, mapping:3, transposed, mirrorY, reverseY, [transitional, reset,] paused, mirrored, on, reverse, [selected]
  // if ((options & 0b1111111110011110U) != (b.options & 0b1111111110011110U)) d |= SEG_DIFFERS_OPT;
  // if ((options & 0x0001U) != (b.options & 0x0001U))                         d |= SEG_DIFFERS_SEL;
  // for (uint8_t i = 0; i < NUM_COLORS; i++) if (colors[i] != b.colors[i])    d |= SEG_DIFFERS_COL;

  // return d;
}

void mAnimatorLight::Segment_New::refreshLightCapabilities() {
  // uint8_t capabilities = 0x01;

  // for (uint8_t b = 0; b < busses.getNumBusses(); b++) {
  //   Bus *bus = busses.getBus(b);
  //   if (bus == nullptr || bus->getLength()==0) break;
  //   if (!bus->isOk()) continue;
  //   if (bus->getStart() >= stop) continue;
  //   if (bus->getStart() + bus->getLength() <= start) continue;

  //   uint8_t type = bus->getType();
  //   if (type == TYPE_ANALOG_1CH || (!cctFromRgb && type == TYPE_ANALOG_2CH)) capabilities &= 0xFE; // does not support RGB
  //   if (bus->isRgbw()) capabilities |= 0x02; // segment supports white channel
  //   if (!cctFromRgb) {
  //     switch (type) {
  //       case TYPE_ANALOG_5CH:
  //       case TYPE_ANALOG_2CH:
  //         capabilities |= 0x04; //segment supports white CCT 
  //     }
  //   }
  //   if (correctWB && type != TYPE_ANALOG_1CH) capabilities |= 0x04; //white balance correction (uses CCT slider)
  //   uint8_t aWM = Bus::getAutoWhiteMode()<255 ? Bus::getAutoWhiteMode() : bus->getAWMode();
  //   bool whiteSlider = (aWM == RGBW_MODE_DUAL || aWM == RGBW_MODE_MANUAL_ONLY); // white slider allowed
  //   if (bus->isRgbw() && (whiteSlider || !(capabilities & 0x01))) capabilities |= 0x08; // allow white channel adjustments (AWM allows or is not RGB)
  // }
  // _capabilities = capabilities;
}

/*
 * Fills segment with color
 */
void mAnimatorLight::Segment_New::fill(uint32_t c) {
  const uint16_t cols = is2D() ? virtualWidth() : virtualLength();
  const uint16_t rows = virtualHeight(); // will be 1 for 1D
  for(uint16_t y = 0; y < rows; y++) for (uint16_t x = 0; x < cols; x++) {
    if (is2D()) setPixelColorXY(x, y, c);
    else        setPixelColor(x, c);
  }
}

// Blends the specified color with the existing pixel color.
void mAnimatorLight::Segment_New::blendPixelColor(int n, uint32_t color, uint8_t blend) {
  // setPixelColor(n, ColourBlend(getPixelColor(n), color, blend));
}

// Adds the specified color with the existing pixel color perserving color balance.
void mAnimatorLight::Segment_New::addPixelColor(int n, uint32_t color) {
  // setPixelColor(n, color_add(getPixelColor(n), color));
}

void mAnimatorLight::Segment_New::fadePixelColor(uint16_t n, uint8_t fade) {
  // CRGB pix = CRGB(getPixelColor(n)).nscale8_video(fade);
  // setPixelColor(n, pix);
}

/*
 * fade out function, higher rate = quicker fade
 */
void mAnimatorLight::Segment_New::fade_out(uint8_t rate) {
  // const uint16_t cols = is2D() ? virtualWidth() : virtualLength();
  // const uint16_t rows = virtualHeight(); // will be 1 for 1D

  // rate = (255-rate) >> 1;
  // float mappedRate = float(rate) +1.1;

  // uint32_t color = colors[1]; // SEGCOLOR_U32(1); // target color
  // int w2 = W(color);
  // int r2 = R(color);
  // int g2 = G(color);
  // int b2 = B(color);

  // for (uint16_t y = 0; y < rows; y++) for (uint16_t x = 0; x < cols; x++) {
  //   color = is2D() ? getPixelColorXY(x, y) : getPixelColor(x);
  //   int w1 = W(color);
  //   int r1 = R(color);
  //   int g1 = G(color);
  //   int b1 = B(color);

  //   int wdelta = (w2 - w1) / mappedRate;
  //   int rdelta = (r2 - r1) / mappedRate;
  //   int gdelta = (g2 - g1) / mappedRate;
  //   int bdelta = (b2 - b1) / mappedRate;

  //   // if fade isn't complete, make sure delta is at least 1 (fixes rounding issues)
  //   wdelta += (w2 == w1) ? 0 : (w2 > w1) ? 1 : -1;
  //   rdelta += (r2 == r1) ? 0 : (r2 > r1) ? 1 : -1;
  //   gdelta += (g2 == g1) ? 0 : (g2 > g1) ? 1 : -1;
  //   bdelta += (b2 == b1) ? 0 : (b2 > b1) ? 1 : -1;

  //   if (is2D()) setPixelColorXY(x, y, r1 + rdelta, g1 + gdelta, b1 + bdelta, w1 + wdelta);
  //   else        setPixelColor(x, r1 + rdelta, g1 + gdelta, b1 + bdelta, w1 + wdelta);
  // }
}

// fades all pixels to black using nscale8()
void mAnimatorLight::Segment_New::fadeToBlackBy(uint8_t fadeBy) {
  const uint16_t cols = is2D() ? virtualWidth() : virtualLength();
  const uint16_t rows = virtualHeight(); // will be 1 for 1D

  for (uint16_t y = 0; y < rows; y++) for (uint16_t x = 0; x < cols; x++) {
    if (is2D()) setPixelColorXY(x, y, CRGB(getPixelColorXY(x,y)).nscale8(255-fadeBy));
    else        setPixelColor(x, CRGB(getPixelColor(x)).nscale8(255-fadeBy));
  }
}

/*
 * blurs segment content, source: FastLED colorutils.cpp
 */
void mAnimatorLight::Segment_New::blur(uint8_t blur_amount)
{
// #ifndef WLED_DISABLE_2D
//   if (is2D()) {
//     // compatibility with 2D
//     const uint16_t cols = virtualWidth();
//     const uint16_t rows = virtualHeight();
//     for (uint16_t i = 0; i < rows; i++) blurRow(i, blur_amount); // blur all rows
//     for (uint16_t k = 0; k < cols; k++) blurCol(k, blur_amount); // blur all columns
//     return;
//   }
// #endif
//   uint8_t keep = 255 - blur_amount;
//   uint8_t seep = blur_amount >> 1;
//   CRGB carryover = CRGB::Black;
//   for(uint16_t i = 0; i < virtualLength(); i++)
//   {
//     CRGB cur = CRGB(getPixelColor(i));
//     CRGB part = cur;
//     part.nscale8(seep);
//     cur.nscale8(keep);
//     cur += carryover;
//     if(i > 0) {
//       uint32_t c = getPixelColor(i-1);
//       uint8_t r = R(c);
//       uint8_t g = G(c);
//       uint8_t b = B(c);
//       setPixelColor(i-1, qadd8(r, part.red), qadd8(g, part.green), qadd8(b, part.blue));
//     }
//     setPixelColor(i,cur.red, cur.green, cur.blue);
//     carryover = part;
//   }
}

/*
 * Put a value 0 to 255 in to get a color value.
 * The colours are a transition r -> g -> b -> back to r
 * Inspired by the Adafruit examples.
 */
uint32_t mAnimatorLight::Segment_New::color_wheel(uint8_t pos) { // TODO
  // if (palette.id) return color_from_palette(pos, false, true, 0);
  pos = 255 - pos;
  if(pos < 85) {
    return ((uint32_t)(255 - pos * 3) << 16) | ((uint32_t)(0) << 8) | (pos * 3);
  } else if(pos < 170) {
    pos -= 85;
    return ((uint32_t)(0) << 16) | ((uint32_t)(pos * 3) << 8) | (255 - pos * 3);
  } else {
    pos -= 170;
    return ((uint32_t)(pos * 3) << 16) | ((uint32_t)(255 - pos * 3) << 8) | (0);
  }
}

/*
 * Returns a new, random wheel index with a minimum distance of 42 from pos.
 */
uint8_t mAnimatorLight::Segment_New::get_random_wheel_index(uint8_t pos) {
  uint8_t r = 0, x = 0, y = 0, d = 0;

  while(d < 42) {
    r = random8();
    x = abs(pos - r);
    y = 255 - x;
    d = MIN(x, y);
  }
  return r;
}


//do not call this method from system context (network callback)
void mAnimatorLight::finalizeInit(void)
{

  ALOG_INF(PSTR("mAnimatorLight::finalizeInit_PreInit:\n\r bus_manager->getNumBusses() C%d"), pCONT_iLight->bus_manager->getNumBusses());

  #ifdef ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT

  // // for the lack of better place enumerate ledmaps here
  // // if we do it in json.cpp (serializeInfo()) we are getting flashes on LEDs
  // // unfortunately this means we do not get updates after uploads
  // enumerateLedmaps();

  // _hasWhiteChannel = _isOffRefreshRequired = false;

  if(pCONT_iLight->bus_manager == nullptr)
  {
    ALOG_ERR(PSTR("busses null"));
    return;
  }

  //if busses failed to load, add default (fresh install, FS issue, ...)
  if (pCONT_iLight->bus_manager->getNumBusses() == 0) 
  {
    DEBUG_PRINTLN(F("No busses, init default"));
    pCONT_iLight->BusManager_Create_DefaultSingleNeoPixel();
  }

  DEBUG_LINE_HERE;

  DEBUG_PRINTF("busses->getNumBusses() %d\n\r", pCONT_iLight->bus_manager->getNumBusses());

  _length = 0;
  for (uint8_t i=0; i<pCONT_iLight->bus_manager->getNumBusses(); i++) 
  {
    
    DEBUG_PRINTF("getNumBusses %d\n\r", i);

    Bus *bus = pCONT_iLight->bus_manager->getBus(i);
    if (bus == nullptr)
    {
      DEBUG_PRINTF("bus == nullptr\n\r");
      continue; // continue breaks one loop occurance only (unlike break)
    }

    if (bus->getStart() + bus->getLength() > MAX_LEDS)
    {
      DEBUG_PRINTF("bus->getStart() + bus->getLength() > MAX_LEDS\n\r");
      break;
    }
    //RGBW mode is enabled if at least one of the strips is RGBW
    // _hasWhiteChannel |= bus->isRgbw();
    //refresh is required to remain off if at least one of the strips requires the refresh.
    _isOffRefreshRequired |= bus->isOffRefreshRequired();
    uint16_t busEnd = bus->getStart() + bus->getLength();
    
    DEBUG_PRINTF("_hasWhiteChannel %d, _isOffRefreshRequired %d, busEnd %d\n\r", _hasWhiteChannel, _isOffRefreshRequired, busEnd);

    if (busEnd > _length)
    {
      _length = busEnd;
      DEBUG_PRINTF("_length = busEnd;\n\r");
    }

    #ifdef ESP8266
    if ((!IS_BUSTYPE_DIGITAL(bus->getType()) || IS_BUSTYPE_2PIN(bus->getType()))) continue;
    uint8_t pins[5];
    if (!bus->getPins(pins)) continue;
    BusDigital* bd = static_cast<BusDigital*>(bus);
    if (pins[0] == 3) bd->reinit();
    #endif
  }

  #endif // ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT

  //initialize leds array. TBD: realloc if nr of leds change
  if (mAnimatorLight::Segment_New::_globalLeds) {
    purgeSegments(true);
    free(mAnimatorLight::Segment_New::_globalLeds);
    mAnimatorLight::Segment_New::_globalLeds = nullptr;
  }
  if (useLedsArray) {
    #if defined(ARDUINO_ARCH_ESP32) && defined(WLED_USE_PSRAM)
    if (psramFound())
      mAnimatorLight::Segment_New::_globalLeds = (CRGB*) ps_malloc(sizeof(CRGB) * _length);
    else
    #endif
      mAnimatorLight::Segment_New::_globalLeds = (CRGB*) malloc(sizeof(CRGB) * _length);
    memset(mAnimatorLight::Segment_New::_globalLeds, 0, sizeof(CRGB) * _length);
  }

  //segments are created in makeAutoSegments();
  loadCustomPalettes(); // (re)load all custom palettes
  // deserializeMap();     // (re)load default ledmap
  
  ALOG_INF(PSTR("mAnimatorLight::finalizeInit_PreInit:\n\r bus_manager->getNumBusses() D%d"), pCONT_iLight->bus_manager->getNumBusses());

}

// IS contained within segment so busses etc are known (ie the right pin will be used)
void IRAM_ATTR mAnimatorLight::setPixelColor(int i, uint32_t col)
{
  if (i >= _length) return;
  if (i < customMappingSize) i = customMappingTable[i];
  pCONT_iLight->bus_manager->setPixelColor(i, col);
}

uint32_t mAnimatorLight::getPixelColor(uint16_t i)
{
  // if (i >= _length) return 0;
  // if (i < customMappingSize) i = customMappingTable[i];
  return RgbcctColor::GetU32Colour(pCONT_iLight->bus_manager->getPixelColor(i));
}


//DISCLAIMER
//The following function attemps to calculate the current LED power usage,
//and will limit the brightness to stay below a set amperage threshold.
//It is NOT a measurement and NOT guaranteed to stay within the ablMilliampsMax margin.
//Stay safe with high amperage and have a reasonable safety margin!
//I am NOT to be held liable for burned down garages!

//fine tune power estimation constants for your setup                  
#define MA_FOR_ESP        100 //how much mA does the ESP use (Wemos D1 about 80mA, ESP32 about 120mA)
                              //you can set it to 0 if the ESP is powered by USB and the LEDs by external

void mAnimatorLight::estimateCurrentAndLimitBri() {
  //power limit calculation
  //each LED can draw up 195075 "power units" (approx. 53mA)
  //one PU is the power it takes to have 1 channel 1 step brighter per brightness step
  //so A=2,R=255,G=0,B=0 would use 510 PU per LED (1mA is about 3700 PU)
  bool useWackyWS2815PowerModel = false;
  byte actualMilliampsPerLed = milliampsPerLed;

  // if(milliampsPerLed == 255) {
  //   useWackyWS2815PowerModel = true;
  //   actualMilliampsPerLed = 12; // from testing an actual strip
  // }

  // if (ablMilliampsMax < 150 || actualMilliampsPerLed == 0) { //0 mA per LED and too low numbers turn off calculation
  //   currentMilliamps = 0;
  //   busses.setBrightness(_brightness);
  //   return;
  // }

  // uint16_t pLen = getLengthPhysical();
  // uint32_t puPerMilliamp = 195075 / actualMilliampsPerLed;
  // uint32_t powerBudget = (ablMilliampsMax - MA_FOR_ESP) * puPerMilliamp; //100mA for ESP power
  // if (powerBudget > puPerMilliamp * pLen) { //each LED uses about 1mA in standby, exclude that from power budget
  //   powerBudget -= puPerMilliamp * pLen;
  // } else {
  //   powerBudget = 0;
  // }

  // uint32_t powerSum = 0;

  // for (uint8_t b = 0; b < busses.getNumBusses(); b++) {
  //   Bus *bus = busses.getBus(b);
  //   if (bus->getType() >= TYPE_NET_DDP_RGB) continue; //exclude non-physical network busses
  //   uint16_t len = bus->getLength();
  //   uint32_t busPowerSum = 0;
  //   for (uint16_t i = 0; i < len; i++) { //sum up the usage of each LED
  //     uint32_t c = bus->getPixelColor(i);
  //     byte r = R(c), g = G(c), b = B(c), w = W(c);

  //     if(useWackyWS2815PowerModel) { //ignore white component on WS2815 power calculation
  //       busPowerSum += (MAX(MAX(r,g),b)) * 3;
  //     } else {
  //       busPowerSum += (r + g + b + w);
  //     }
  //   }

  //   if (bus->isRgbw()) { //RGBW led total output with white LEDs enabled is still 50mA, so each channel uses less
  //     busPowerSum *= 3;
  //     busPowerSum = busPowerSum >> 2; //same as /= 4
  //   }
  //   powerSum += busPowerSum;
  // }

  // uint32_t powerSum0 = powerSum;
  // powerSum *= _brightness;
  
  // if (powerSum > powerBudget) //scale brightness down to stay in current limit
  // {
  //   float scale = (float)powerBudget / (float)powerSum;
  //   uint16_t scaleI = scale * 255;
  //   uint8_t scaleB = (scaleI > 255) ? 255 : scaleI;
  //   uint8_t newBri = scale8(_brightness, scaleB);
  //   busses.setBrightness(newBri); //to keep brightness uniform, sets virtual busses too
  //   currentMilliamps = (powerSum0 * newBri) / puPerMilliamp;
  // } else {
  //   currentMilliamps = powerSum / puPerMilliamp;
  //   busses.setBrightness(_brightness);
  // }
  // currentMilliamps += MA_FOR_ESP; //add power of ESP back to estimate
  // currentMilliamps += pLen; //add standby power back to estimate
}

void mAnimatorLight::show(void) {

  // avoid race condition, caputre _callback value
  show_callback callback = _callback;
  if (callback) callback();

  estimateCurrentAndLimitBri();
  
  // some buses send asynchronously and this method will return before
  // all of the data has been sent.
  // See https://github.com/Makuna/NeoPixelBus/wiki/ESP32-NeoMethods#neoesp32rmt-methods
  // busses.show();
  unsigned long now = millis();
  unsigned long diff = now - _lastShow;
  uint16_t fpsCurr = 200;
  if (diff > 0) fpsCurr = 1000 / diff;
  _cumulativeFps = (3 * _cumulativeFps + fpsCurr) >> 2;
  _lastShow = now;
}

/**
 * Returns a true value if any of the strips are still being updated.
 * On some hardware (ESP32), strip updates are done asynchronously.
 */
bool mAnimatorLight::isUpdating() {
  // return !busses.canAllShow();
}

/**
 * Returns the refresh rate of the LED strip. Useful for finding out whether a given setup is fast enough.
 * Only updates on show() or is set to 0 fps if last show is more than 2 secs ago, so accurary varies
 */
uint16_t mAnimatorLight::getFps() {
  if (millis() - _lastShow > 2000) return 0;
  return _cumulativeFps +1;
}

void mAnimatorLight::setTargetFps(uint8_t fps) {
  if (fps > 0 && fps <= 120) _targetFps = fps;
  _frametime = 1000 / _targetFps;
}

void mAnimatorLight::setMode(uint8_t segid, uint8_t m) {
  if (segid >= segments.size()) return;
   
  if (m >= getModeCount()) m = getModeCount() - 1;

  if (segments[segid].animation_mode_id != m) {
    // segments[segid].startTransition(_transitionDur); // set effect transitions
    //segments[segid].markForReset();
    segments[segid].animation_mode_id = m;
  }
}

//applies to all active and selected segments
void mAnimatorLight::setColor(uint8_t slot, uint32_t c) {
  if (slot >= NUM_COLORS) return;

  for (segment_new &seg : segments) {
    if (seg.isActive() && seg.isSelected()) {
      seg.setColor(slot, c);
    }
  }
}

void mAnimatorLight::setCCT(uint16_t k) {
  for (segment_new &seg : segments) {
    if (seg.isActive() && seg.isSelected()) {
      seg.setCCT(k);
    }
  }
}

void mAnimatorLight::setBrightness(uint8_t b, bool direct) {
  // if (gammaCorrectBri) b = gamma8(b);  
  if (_brightness == b) return;
  _brightness = b;
  if (_brightness == 0) { //unfreeze all segments on power off
    for (segment_new &seg : segments) {
      seg.freeze = false;
    }
  }
  if (direct) {
    // would be dangerous if applied immediately (could exceed ABL), but will not output until the next show()
    // busses.setBrightness(b);
  } else {
    unsigned long t = millis();
    if (segments[0].next_time > t + 22 && t - _lastShow > MIN_SHOW_DELAY) show(); //apply brightness change immediately if no refresh soon
  }
}

uint8_t mAnimatorLight::getFirstSelectedSegId(void)
{
  size_t i = 0;
  for (segment_new &seg : segments) {
    if (seg.isActive() && seg.isSelected()) return i;
    i++;
  }
  // if none selected, use the main segment
  return getMainSegmentId();
}

void mAnimatorLight::setMainSegmentId(uint8_t n) {
  _mainSegment = 0;
  if (n < segments.size()) {
    _mainSegment = n;
  }
  return;
}

uint8_t mAnimatorLight::getLastActiveSegmentId(void) {
  for (size_t i = segments.size() -1; i > 0; i--) {
    if (segments[i].isActive()) return i;
  }
  return 0;
}

uint8_t mAnimatorLight::getActiveSegmentsNum(void) {
  uint8_t c = 0;
  for (size_t i = 0; i < segments.size(); i++) {
    if (segments[i].isActive()) c++;
  }
  return c;
}

uint16_t mAnimatorLight::getLengthPhysical(void) {
  uint16_t len = 0;
  // for (size_t b = 0; b < busses.getNumBusses(); b++) {
  //   Bus *bus = busses.getBus(b);
  //   if (bus->getType() >= TYPE_NET_DDP_RGB) continue; //exclude non-physical network busses
  //   len += bus->getLength();
  // }
  return len;
}

//used for JSON API info.leds.rgbw. Little practical use, deprecate with info.leds.rgbw.
//returns if there is an RGBW bus (supports RGB and White, not only white)
//not influenced by auto-white mode, also true if white slider does not affect output white channel
bool mAnimatorLight::hasRGBWBus(void) {
  // for (size_t b = 0; b < busses.getNumBusses(); b++) {
  //   Bus *bus = busses.getBus(b);
  //   if (bus == nullptr || bus->getLength()==0) break;
  //   switch (bus->getType()) {
  //     case TYPE_SK6812_RGBW:
  //     case TYPE_TM1814:
  //     case TYPE_ANALOG_4CH:
  //       return true;
  //   }
  // }
  // return false;
}

bool mAnimatorLight::hasCCTBus(void) {
  // if (cctFromRgb && !correctWB) return false;
  // for (size_t b = 0; b < busses.getNumBusses(); b++) {
  //   Bus *bus = busses.getBus(b);
  //   if (bus == nullptr || bus->getLength()==0) break;
  //   switch (bus->getType()) {
  //     case TYPE_ANALOG_5CH:
  //     case TYPE_ANALOG_2CH:
  //       return true;
  //   }
  // }
  // return false;
}

void mAnimatorLight::purgeSegments(bool force) 
{
  ALOG_INF(PSTR("purgeSegments(%d)"),force);
  ALOG_INF(PSTR("segments.size=%d"),segments.size());
  // remove all inactive segments (from the back)
  int deleted = 0;
  if (segments.size() <= 1) return;

  for (size_t i = segments.size()-1; i > 0; i--)
  {
    ALOG_INF(PSTR("i=%d"),i);
    if (segments[i].pixel_range.stop == 0 || force) {
      DEBUG_PRINT(F("Purging segment segment: ")); DEBUG_PRINTLN(i);
      deleted++;
      segments.erase(segments.begin() + i);
    }
  }
  if (deleted) {
    segments.shrink_to_fit();
    if (_mainSegment >= segments.size()) setMainSegmentId(0);
  }
}

mAnimatorLight::Segment_New& mAnimatorLight::getSegment(uint8_t id) {
  return segments[id >= segments.size() ? getMainSegmentId() : id]; // vectors
}

/**
 * @brief For turning on a new effect/segment
 * 
 * @param n 
 * @param i1 
 * @param i2 
 * @param grouping 
 * @param spacing 
 * @param offset 
 * @param startY 
 * @param stopY 
 */
void mAnimatorLight::setSegment(uint8_t n, uint16_t i1, uint16_t i2, uint8_t grouping, uint8_t spacing, uint16_t offset, uint16_t startY, uint16_t stopY) {
  if (n >= segments.size()) return;
  mAnimatorLight::Segment_New& seg = segments[n];

  //return if neither bounds nor grouping have changed
  bool boundsUnchanged = (seg.pixel_range.start == i1 && seg.pixel_range.stop == i2);
  if (isMatrix) {
    boundsUnchanged &= (seg.startY == startY && seg.stopY == stopY);
  }
  if (boundsUnchanged
      && (!grouping || (seg.grouping == grouping && seg.spacing == spacing))
      && (offset == UINT16_MAX || offset == seg.offset)) return;

  //if (seg.stop) setRange(seg.start, seg.stop -1, BLACK); //turn old segment range off
  if (seg.pixel_range.stop) seg.fill(BLACK); //turn old segment range off
  if (i2 <= i1) //disable segment
  {
    // disabled segments should get removed using purgeSegments()
    DEBUG_PRINT(F("-- mAnimatorLight::Segment_New ")); DEBUG_PRINT(n); DEBUG_PRINTLN(F(" marked inactive."));
    seg.pixel_range.stop = 0;
    //if (seg.name) {
    //  delete[] seg.name;
    //  seg.name = nullptr;
    //}
    // if main segment is deleted, set first active as main segment
    if (n == _mainSegment) setMainSegmentId(0);
    seg.markForReset();
    return;
  }
  if (isMatrix) {
    #ifndef WLED_DISABLE_2D
    if (i1 < mAnimatorLight::Segment_New::maxWidth) seg.start = i1;
    seg.stop = i2 > mAnimatorLight::Segment_New::maxWidth ? mAnimatorLight::Segment_New::maxWidth : i2;
    if (startY < mAnimatorLight::Segment_New::maxHeight) seg.startY = startY;
    seg.stopY = stopY > mAnimatorLight::Segment_New::maxHeight ? mAnimatorLight::Segment_New::maxHeight : MAX(1,stopY);
    #endif
  } else {
    if (i1 < _length) seg.pixel_range.start = i1;
    seg.pixel_range.stop = i2 > _length ? _length : i2;
    seg.startY = 0;
    seg.stopY  = 1;
  }
  if (grouping) {
    seg.grouping = grouping;
    seg.spacing = spacing;
  }
  if (offset < UINT16_MAX) seg.offset = offset;
  seg.markForReset();
  if (!boundsUnchanged) seg.refreshLightCapabilities();
}

void mAnimatorLight::restartRuntime() {
  for (segment_new &seg : segments) seg.markForReset();
}

void mAnimatorLight::resetSegments2() {
  segments.clear(); // destructs all mAnimatorLight::Segment_New as part of clearing
  #ifndef WLED_DISABLE_2D
  segment_new seg = isMatrix ? mAnimatorLight::Segment_New(0, mAnimatorLight::Segment_New::maxWidth, 0, mAnimatorLight::Segment_New::maxHeight) : mAnimatorLight::Segment_New(0, _length);
  #else
  segment_new seg = mAnimatorLight::Segment_New(0, _length);
  #endif
  segments.push_back(seg);
  _mainSegment = 0;
}

void mAnimatorLight::makeAutoSegments(bool forceReset) {
  if (isMatrix) {
    #ifndef WLED_DISABLE_2D
    // only create 1 2D segment
    if (forceReset || getSegmentsNum() == 0) resetSegments2(); // initialises 1 segment
    else if (getActiveSegmentsNum() == 1) {
      size_t i = getLastActiveSegmentId();
      segments[i].start  = 0;
      segments[i].stop   = mAnimatorLight::Segment_New::maxWidth;
      segments[i].startY = 0;
      segments[i].stopY  = mAnimatorLight::Segment_New::maxHeight;
      segments[i].grouping = 1;
      segments[i].spacing  = 0;
      _mainSegment = i;
    }
    #endif
  } else 
  // if (autoSegments) { //make one segment per bus
  //   uint16_t segStarts[MAX_NUM_SEGMENTS] = {0};
  //   uint16_t segStops [MAX_NUM_SEGMENTS] = {0};
  //   uint8_t s = 0;
  //   // for (uint8_t i = 0; i < busses.getNumBusses(); i++) {
  //   //   Bus* b = busses.getBus(i);

  //   //   segStarts[s] = b->getStart();
  //   //   segStops[s]  = segStarts[s] + b->getLength();

  //   //   //check for overlap with previous segments
  //   //   for (size_t j = 0; j < s; j++) {
  //   //     if (segStops[j] > segStarts[s] && segStarts[j] < segStops[s]) {
  //   //       //segments overlap, merge
  //   //       segStarts[j] = min(segStarts[s],segStarts[j]);
  //   //       segStops [j] = max(segStops [s],segStops [j]); segStops[s] = 0;
  //   //       s--;
  //   //     }
  //   //   }
  //   //   s++;
  //   // }
  //   segments.clear();
  //   for (size_t i = 0; i < s; i++) {
  //     mAnimatorLight::Segment_New seg = mAnimatorLight::Segment_New(segStarts[i], segStops[i]);
  //     seg.selected = true;
  //     segments.push_back(seg);
  //   }
  //   _mainSegment = 0;
  // } else 
  {
    if (forceReset || getSegmentsNum() == 0) resetSegments2();
    //expand the main seg to the entire length, but only if there are no other segments, or reset is forced
    else if (getActiveSegmentsNum() == 1) {
      size_t i = getLastActiveSegmentId();
      segments[i].pixel_range.start = 0;
      segments[i].pixel_range.stop  = _length;
      _mainSegment = 0;
    }
  }

  fixInvalidSegments();
}

void mAnimatorLight::fixInvalidSegments() {
  //make sure no segment is longer than total (sanity check)
  for (size_t i = getSegmentsNum()-1; i > 0; i--) {
    if (segments[i].pixel_range.start >= _length) { segments.erase(segments.begin()+i); continue; }
    if (segments[i].pixel_range.stop  >  _length) segments[i].pixel_range.stop = _length;
    // this is always called as the last step after finalizeInit(), update covered bus types
    segments[i].refreshLightCapabilities();
  }
}

//true if all segments align with a bus, or if a segment covers the total length
bool mAnimatorLight::checkSegmentAlignment() {
  bool aligned = false;
  for (segment_new &seg : segments) {
    // for (uint8_t b = 0; b<busses.getNumBusses(); b++) {
    //   Bus *bus = busses.getBus(b);
    //   if (seg.start == bus->getStart() && seg.stop == bus->getStart() + bus->getLength()) aligned = true;
    // }
    if (seg.pixel_range.start == 0 && seg.pixel_range.stop == _length) aligned = true;
    if (!aligned) return false;
  }
  return true;
}

//After this function is called, setPixelColor() will use that segment (offsets, grouping, ... will apply)
//Note: If called in an interrupt (e.g. JSON API), original segment must be restored,
//otherwise it can lead to a crash on ESP32 because _segment_index is modified while in use by the main thread
uint8_t mAnimatorLight::setPixelSegment(uint8_t n)
{
  uint8_t prevSegId = _segment_index_primary;
  if (n < segments.size()) {
    _segment_index_primary = n;
    _virtualSegmentLength = segments[_segment_index_primary].virtualLength();
  }
  return prevSegId;
}

void mAnimatorLight::setRange(uint16_t i, uint16_t i2, uint32_t col)
{
  if (i2 >= i)
  {
    for (uint16_t x = i; x <= i2; x++) setPixelColor(x, col);
  } else
  {
    for (uint16_t x = i2; x <= i; x++) setPixelColor(x, col);
  }
}

void mAnimatorLight::setTransitionMode(bool t)
{
  // for (segment &seg : segments) if (!seg.transitional) seg.startTransition(t ? _transitionDur : 0);
}

#ifdef ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES
void mAnimatorLight::printSize()
{
  size_t size = 0;
  for (const mAnimatorLight::Segment_New &seg : segments) size += seg.getSize();
  DEBUG_PRINTF("Segments: %d -> %uB\n", segments.size(), size);
  DEBUG_PRINTF("Modes: %d*%d=%uB\n", sizeof(mode_ptr), effects.function.size(), (effects.function.capacity()*sizeof(mode_ptr)));
  DEBUG_PRINTF("Data: %d*%d=%uB\n", sizeof(const char *), effects.data.size(), (effects.data.capacity()*sizeof(const char *)));
  DEBUG_PRINTF("Map: %d*%d=%uB\n", sizeof(uint16_t), (int)customMappingSize, customMappingSize*sizeof(uint16_t));
  if (useLedsArray) DEBUG_PRINTF("Buffer: %d*%d=%uB\n", sizeof(CRGB), (int)_length, _length*sizeof(CRGB));
}
#endif

void mAnimatorLight::loadCustomPalettes()
{
  byte tcp[72]; //support gradient palettes with up to 18 entries
  CRGBPalette16 targetPalette;
  customPalettes.clear(); // start fresh
  // for (int index = 0; index<10; index++) {
  //   char fileName[32];
  //   sprintf_P(fileName, PSTR("/palette%d.json"), index);

  //   StaticJsonDocument<1536> pDoc; // barely enough to fit 72 numbers
  //   if (WLED_FS.exists(fileName)) {
  //     DEBUG_PRINT(F("Reading palette from "));
  //     DEBUG_PRINTLN(fileName);

  //     if (readObjectFromFile(fileName, nullptr, &pDoc)) {
  //       JsonArray pal = pDoc[F("palette")];
  //       if (!pal.isNull() && pal.size()>7) { // not an empty palette (at least 2 entries)
  //         size_t palSize = MIN(pal.size(), 72);
  //         palSize -= palSize % 4; // make sure size is multiple of 4
  //         for (size_t i=0; i<palSize && pal[i].as<int>()<256; i+=4) {
  //           tcp[ i ] = (uint8_t) pal[ i ].as<int>(); // index
  //           tcp[i+1] = (uint8_t) pal[i+1].as<int>(); // R
  //           tcp[i+2] = (uint8_t) pal[i+2].as<int>(); // G
  //           tcp[i+3] = (uint8_t) pal[i+3].as<int>(); // B
  //           DEBUG_PRINTF("%d(%d) : %d %d %d\n", i, int(tcp[i]), int(tcp[i+1]), int(tcp[i+2]), int(tcp[i+3]));
  //         }
  //         customPalettes.push_back(targetPalette.loadDynamicGradientPalette(tcp));
  //       }
  //     }
  //   } else {
  //     break;
  //   }
  // }
}



RgbcctColor 
#ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
IRAM_ATTR 
#endif 
mAnimatorLight::Segment_New::GetPaletteColour(
  /**
   * @brief _pixel_position
   * ** [0-SEGLEN]
   * ** [0-255]   
   */
  uint16_t pixel_position,
  /**
   * @brief flag_spanned_segment
   * ** [1] : If spanned segment, then indexing (0-255) is expanded into the SEGLEN 
   * ** [0]: Unchanged, index coming in will be 0-SEGLEN but never scaled into 255. Or should it be?
   * ** [2]: preffered
   */
  uint8_t     flag_spanned_segment, 
  /**
   * @brief flag_wrap_hard_edge
   * ** [true] : 16 palette gradients will not blend from 15 back to 0. ie 0-255 does not become 0-240 (where 0,15,31,47,63,79,95,111,127,143,159,175,191,207,223,239)
   * ** [false]: Palette16 with 16 elements, as 0-255 pixel_position, will blend around smoothly using built-in CRGBPalette16
   */
  uint8_t     flag_wrap_hard_edge,
  /**
   * @brief flag_crgb_exact_colour
   * ** [true] : 16 palette gradients will not blend from 15 back to 0. ie 0-255 does not become 0-240 (where 0,15,31,47,63,79,95,111,127,143,159,175,191,207,223,239)
   * ** [false]: Palette16 with 16 elements, as 0-255 pixel_position, will blend around smoothly using built-in CRGBPalette16
   */
  uint8_t     flag_crgb_exact_colour,
  /**
   * @brief encoded_value
   * ** [uint32_t*] : encoded value from palette
   */
  uint8_t* encoded_value  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
){

  if(palette.id != palette_container->loaded_palette_id)
  {
    pCONT_lAni->LoadPalette(palette.id, pCONT_lAni->getCurrSegmentId());  //loadPalette perhaps needs to be a segment instance instead. Though this will block unloaded methods
  }

  return mPaletteI->GetColourFromPreloadedPaletteBuffer_2023(
    palette.id,
    (uint8_t*)palette_container->pData.data(),
    pixel_position,
    encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
    flag_spanned_segment, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
    flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
    flag_crgb_exact_colour
  );

}


uint8_t mAnimatorLight::Segment_New::GetPaletteDiscreteWidth()
{
  if(palette.id != palette_container->loaded_palette_id)
  {
    pCONT_lAni->LoadPalette(palette.id, pCONT_lAni->getCurrSegmentId());  //loadPalette perhaps needs to be a segment instance instead. Though this will block unloaded methods
  }

  return mPaletteI->GetPaletteDiscreteWidth(
    palette.id,
    (uint8_t*)palette_container->pData.data()
  );

}



/**
 * @brief New method that gets any colour without preloading, and hence will be slower but allows getters to use this for multiple uses e.g. webui population
 * 
 */
RgbcctColor 
#ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
IRAM_ATTR 
#endif 
mAnimatorLight::GetColourFromUnloadedPalette2(
  uint16_t palette_id,
  uint16_t _pixel_position,
  bool     flag_spanned_segment, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
  bool     flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
  bool     flag_crgb_exact_colour,
  uint8_t* encoded_value  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
){

  // ALOG_INF(PSTR("Aflag_crgb_exact_colour=%d"), flag_crgb_exact_colour);
  /**
   * @brief Load palette into temporary buffer first, then handle using the default function 
   **/
  mPaletteLoaded palette_container_temp = mPaletteLoaded();
  pCONT_lAni->LoadPalette(palette_id, 0, &palette_container_temp);
  
  RgbcctColor colour = mPaletteI->GetColourFromPreloadedPaletteBuffer_2023(
    palette_id,
    &palette_container_temp.pData[0],
    _pixel_position,
    encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
    flag_spanned_segment, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
    flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
    flag_crgb_exact_colour
  );

  return colour;

}


//load custom mapping table from JSON file (called from finalizeInit() or deserializeState())
void mAnimatorLight::deserializeMap(uint8_t n) {
  // if (isMatrix) return; // 2D support creates its own ledmap

  // char fileName[32];
  // strcpy_P(fileName, PSTR("/ledmap"));
  // if (n) sprintf(fileName +7, "%d", n);
  // strcat(fileName, ".json");
  // bool isFile = WLED_FS.exists(fileName);

  // if (!isFile) {
  //   // erase custom mapping if selecting nonexistent ledmap.json (n==0)
  //   if (!n && customMappingTable != nullptr) {
  //     customMappingSize = 0;
  //     delete[] customMappingTable;
  //     customMappingTable = nullptr;
  //   }
  //   return;
  // }

  // if (!requestJSONBufferLock(7)) return;

  // DEBUG_PRINT(F("Reading LED map from "));
  // DEBUG_PRINTLN(fileName);

  // if (!readObjectFromFile(fileName, nullptr, &doc)) {
  //   releaseJSONBufferLock();
  //   return; //if file does not exist just exit
  // }

  // // erase old custom ledmap
  // if (customMappingTable != nullptr) {
  //   customMappingSize = 0;
  //   delete[] customMappingTable;
  //   customMappingTable = nullptr;
  // }

  // JsonArray map = doc[F("map")];
  // if (!map.isNull() && map.size()) {  // not an empty map
  //   customMappingSize  = map.size();
  //   customMappingTable = new uint16_t[customMappingSize];
  //   for (uint16_t i=0; i<customMappingSize; i++) {
  //     customMappingTable[i] = (uint16_t) map[i];
  //   }
  // }

  // releaseJSONBufferLock();
}


void IRAM_ATTR mAnimatorLight::Segment_New::SetPixelColor_All(RgbcctColor colour)
{
  // for(uint16_t pixel = 0; pixel < pCONT_lAni->settings.light_size_count; pixel++){
  //   SetPixelColor(pixel, colour);
  // }
  // pCONT_iLight->ShowInterface();
  
  // if(bus_manager){ bus_manager->show(); }
}


void IRAM_ATTR mAnimatorLight::Segment_New::SetPixelColor(uint16_t indexPixel, uint8_t red, uint8_t green, uint8_t blue, bool segment_brightness_needs_applied)
{
  SetPixelColor(indexPixel, RgbColor(red,green,blue), segment_brightness_needs_applied);
}
 
  
void IRAM_ATTR mAnimatorLight::Segment_New::SetPixelColor(uint16_t indexPixel, uint32_t color, bool segment_brightness_needs_applied)
{
  RgbcctColor col = RgbcctColor(0);
  col.red =   (color >> 16 & 0xFF);
  col.green = (color >> 8  & 0xFF);
  col.blue =  (color       & 0xFF);
  col.W1 =    (color >> 24 & 0xFF);
  col.W2 =    (color >> 24 & 0xFF);
  SetPixelColor(indexPixel, col, segment_brightness_needs_applied);
}


/**
 * @brief 
 * 
 * @param indexPixel 
 * @param color_internal 
 * @param segment_brightness_needs_applied 
 */
void IRAM_ATTR mAnimatorLight::Segment_New::SetPixelColor(uint16_t indexPixel, RgbcctColor color_internal, bool segment_brightness_needs_applied)
{

  int vStrip = indexPixel>>16; // hack to allow running on virtual strips (2D segment columns/rows)
  indexPixel &= 0xFFFF;
  if (indexPixel >= virtualLength() || indexPixel<0) return;  // if pixel would fall out of segment just exit

  #ifndef WLED_DISABLE_2D
    if (is2D()) {
      uint16_t vH = virtualHeight();  // segment height in logical pixels
      uint16_t vW = virtualWidth();
      switch (map1D2D) {
        case M12_Pixels:
          // use all available pixels as a long strip
          setPixelColorXY(i % vW, i / vW, col);
          break;
        case M12_pBar:
          // expand 1D effect vertically or have it play on virtual strips
          if (vStrip>0) setPixelColorXY(vStrip - 1, vH - i - 1, col);
          else          for (int x = 0; x < vW; x++) setPixelColorXY(x, vH - i - 1, col);
          break;
        case M12_pArc:
          // expand in circular fashion from center
          if (i==0)
            setPixelColorXY(0, 0, col);
          else {
            float step = HALF_PI / (2.85f*i);
            for (float rad = 0.0f; rad <= HALF_PI+step/2; rad += step) {
              // may want to try float version as well (with or without antialiasing)
              int x = roundf(sin_t(rad) * i);
              int y = roundf(cos_t(rad) * i);
              setPixelColorXY(x, y, col);
            }
          }
          break;
        case M12_pCorner:
          for (int x = 0; x <= i; x++) setPixelColorXY(x, i, col);
          for (int y = 0; y <  i; y++) setPixelColorXY(i, y, col);
          break;
      }
      return;
    } else if (Segment::maxHeight!=1 && (width()==1 || height()==1)) {
      // we have a vertical or horizontal 1D segment (WARNING: virtual...() may be transposed)
      int x = 0, y = 0;
      if (virtualHeight()>1) y = i;
      if (virtualWidth() >1) x = i;
      setPixelColorXY(x, y, col);
      return;
    }
  #endif

  // if (leds) leds[i] = col; // custom led mapping

  // uint16_t len = length();


  /**
   * @brief Apply brightness if required
   * 
   */
  if(segment_brightness_needs_applied){
    // uint8_t _bri_t = currentBri(on ? opacity : 0);
    uint8_t brightness = pCONT_iLight->getBriRGB_Global();

    if(_brightness_rgb!=255)
    {
      brightness = scale8(brightness, _brightness_rgb);
    }

    color_internal.R  = scale8(color_internal.R,  brightness);
    color_internal.G  = scale8(color_internal.G,  brightness);
    color_internal.B  = scale8(color_internal.B,  brightness);
    color_internal.W1 = scale8(color_internal.W1, brightness);
    color_internal.W2 = scale8(color_internal.W2, brightness);
    
  }


  // ALOG_INF(PSTR("Segment brightness should already be applied inside the segment, unless the segment_brightness_needs_applied for WLED methods is applying the segment "));






  /**
   * @brief Apply Pixel hardware colour mapping from internal to hardware order
   **/
  RgbcctColor colour_hardware = color_internal; // Copy
    
  uint16_t physical_indexPixel = indexPixel; // Going from virtual/internal index to physical/external index
  uint16_t segment_length      = length();

  /**
   * @brief Modify pixel index if required: start, grouping, spacing, offset
   **/
  physical_indexPixel = physical_indexPixel * groupLength();
  if (reverse) { // is segment reversed?
    if (mirror) { // is segment mirrored?
      physical_indexPixel = (segment_length - 1) / 2 - physical_indexPixel;  //only need to index half the pixels
    } else {
      physical_indexPixel = (segment_length - 1) - physical_indexPixel;
    }
  }
  physical_indexPixel += pixel_range.start;

  #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
  ALOG_INF( PSTR("pIndex=%d,%d"), physical_indexPixel-pixel_range.start, physical_indexPixel);
  #endif // ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR


  /**
   * @brief Set all pixels in the group
   **/
  for (uint16_t j = 0; j < grouping; j++) 
  {

    uint16_t indexSet = physical_indexPixel + ((reverse) ? -j : j);
    
    if (
      indexSet >= pixel_range.start && 
      indexSet <  pixel_range.stop
    ){

      if (mirror) 
      { //set the corresponding mirrored pixel
        uint16_t indexMir = pixel_range.stop - indexSet + pixel_range.start - 1;          
        indexMir += offset; // offset/phase

        if (indexMir >= pixel_range.stop) indexMir -= segment_length; // Wrap
        pCONT_iLight->bus_manager->setPixelColor(indexMir, colour_hardware);

      }
      indexSet += offset; // offset/phase

      if (indexSet >= pixel_range.stop) indexSet -= segment_length; // Wrap
      pCONT_iLight->bus_manager->setPixelColor(indexSet, colour_hardware);
    }
  
    ALOG_DBM(PSTR("colour_hardware[%d] = %d,%d,%d,%d,%d"),physical_indexPixel, colour_hardware.R, colour_hardware.G, colour_hardware.B, colour_hardware.W1, colour_hardware.W2);

  }

}

/**
 * @brief 
 * 
 * @param indexPixel 
 * @return RgbcctColor 
 */
RgbcctColor IRAM_ATTR mAnimatorLight::Segment_New::GetPixelColor(uint16_t indexPixel)
{
  
  int vStrip = indexPixel>>16;
  indexPixel &= 0xFFFF;

#ifndef WLED_DISABLE_2D
  if (is2D()) {
    uint16_t vH = virtualHeight();  // segment height in logical pixels
    uint16_t vW = virtualWidth();
    switch (map1D2D) {
      case M12_Pixels:
        return getPixelColorXY(i % vW, i / vW);
        break;
      case M12_pBar:
        if (vStrip>0) return getPixelColorXY(vStrip - 1, vH - i -1);
        else          return getPixelColorXY(0, vH - i -1);
        break;
      case M12_pArc:
      case M12_pCorner:
        // use longest dimension
        return vW>vH ? getPixelColorXY(i, 0) : getPixelColorXY(0, i);
        break;
    }
    return 0;
  }
#endif

  // Custom mapping
  // if (leds) return RGBW32(leds[i].r, leds[i].g, leds[i].b, 0);

  uint16_t physical_indexPixel = indexPixel;

  
  if (reverse) physical_indexPixel = virtualLength() - physical_indexPixel - 1;
  physical_indexPixel *= groupLength();
  physical_indexPixel += pixel_range.start;
  /* offset/phase */
  physical_indexPixel += offset;
  if (physical_indexPixel >= pixel_range.stop) physical_indexPixel -= length();
  
  RgbcctColor colour_hardware = pCONT_iLight->bus_manager->getPixelColor(physical_indexPixel);
  
  ALOG_DBM(PSTR("colour_hardware[%d] = %d,%d,%d,%d,%d"),physical_indexPixel, colour_hardware.R, colour_hardware.G, colour_hardware.B, colour_hardware.W1, colour_hardware.W2);

  return colour_hardware;

}




/******************************************************************************************************************
 * mInterfaceLight_ConstructJSON.cpp
*******************************************************************************************************************/



uint8_t mAnimatorLight::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();

    // JBI->Add("light_size_count", settings.light_size_count);

    // JBI->Add("BriRGB_Global",  getBriRGB_Global());
    // JBI->Add("BriCCT_Global",  getBriCCT_Global());

  return JBI->End();

}

/**
 * @brief Unlike debug_segments, only the useful info here
 * 
 * @param json_level 
 * @param json_appending 
 * @return uint8_t 
 */
uint8_t mAnimatorLight::ConstructJSON_Segments(uint8_t json_level, bool json_appending)
{

  JBI->Start();

    JBI->Add("millis", millis());

    JBI->Add("Start", SEGMENT_I(0).pixel_range.start);
    JBI->Add("Stop", SEGMENT_I(0).pixel_range.stop);

  return JBI->End();

}



uint8_t mAnimatorLight::ConstructJSON_Playlist(uint8_t json_level, bool json_appending)
{

  JBI->Start();

    JBI->Add("millis", millis());

  return JBI->End();

}



#ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
uint8_t mAnimatorLight::ConstructJSON_Mode_Ambilight(uint8_t json_level, bool json_appending)
{

  JBI->Start();

    JBI->Add("millis", millis());

  return JBI->End();

}
#endif


#ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
uint8_t mAnimatorLight::ConstructJSON_Mode_SetManual(uint8_t json_level, bool json_appending)
{

JBI->Start();

  JBI->Add("millis", millis());

return JBI->End();

}
#endif


/**
 * @brief Debug 
 */
#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
uint8_t mAnimatorLight::ConstructJSON_Debug_Palette(uint8_t json_level, bool json_appending)
{

char buffer[100];

JBI->Start();

    JBI->Add("AvailablePalettes", (uint16_t)mPaletteI->GetPaletteListLength() );

    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    for(uint8_t seg_i = 0; seg_i<  segments.size(); seg_i++)
    {
      JBI->Object_Start_F("Segment%d",seg_i);      
        JBI->Add("dataLen", SEGMENT_I(seg_i).palette_container->pData.size());
        JBI->Array_Start("data");
        for(uint8_t i=0;i<SEGMENT_I(seg_i).palette_container->pData.size();i++)
        {
          JBI->Add(SEGMENT_I(seg_i).palette_container->pData[i]);
        }
        JBI->Array_End();
      JBI->Object_End();
    }
    #endif// ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER

    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE
    uint8_t seg_i = 0;

    JBI->Add("PaletteSize", mPaletteI->GetColoursInCRGB16Palette(SEGMENT.palette.id) );
    JBI->Array_Start("CRGB16Palette16");   

    for(uint8_t elem_i=0;elem_i<16;elem_i++)
    {
      JBI->Array_Start();
        JBI->Add( SEGMENT_I(seg_i).palette_container->CRGB16Palette16_Palette.data[elem_i].r );
        JBI->Add( SEGMENT_I(seg_i).palette_container->CRGB16Palette16_Palette.data[elem_i].g );
        JBI->Add( SEGMENT_I(seg_i).palette_container->CRGB16Palette16_Palette.data[elem_i].b );
      JBI->Array_End();          
    }
    JBI->Array_End();

    JBI->Array_Start("CRGB16Palette16MAN");
    for(uint8_t elem_i=0;elem_i<16;elem_i++)
    {
      JBI->Array_Start();
        JBI->Add( SEGMENT_I(seg_i).palette_container->CRGB16Palette16_Palette.data[elem_i].r );
        JBI->Add( SEGMENT_I(seg_i).palette_container->CRGB16Palette16_Palette.data[elem_i].g );
        JBI->Add( SEGMENT_I(seg_i).palette_container->CRGB16Palette16_Palette.data[elem_i].b );
      JBI->Array_End();          
    }
    JBI->Array_End();
    #endif// ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE


    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    JBI->Object_Start("Encoding");
      for(uint8_t palette_id=mPalette::PALETTELIST_STATIC_HOLLOWEEN_OP__ID;palette_id<mPaletteI->PALETTELIST_STATIC_CHRISTMAS_01__ID;palette_id++)
      {
        // JBI->Array_Start_P("%s", mPaletteI->GetPaletteNameByID( palette_id, buffer, sizeof(buffer) ));
        JBI->Array_Start_P("P_%d", palette_id );        
          mPalette::PALETTELIST::PALETTE *ptr = mPaletteI->GetPalettePointerByID(palette_id);
          uint16_t value = ptr->encoding.data;
          char buffer[33] = {0}; //null terminated
          for(uint8_t i=0;i<16;i++)
          {
            buffer[15-i] = bitRead(value,i) ? '1' : '0';
          }
          JBI->Add(buffer);
        JBI->Array_End();
      }
    JBI->Object_End();
    #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING


    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    JBI->Array_Start_P("DataLength");
      for(uint8_t palette_id=mPalette::PALETTELIST_VARIABLE_HSBID_01__ID;palette_id<mPaletteI->PALETTELIST_STATIC_CHRISTMAS_28__ID;palette_id++)
      {
        mPalette::PALETTELIST::PALETTE *ptr = mPaletteI->GetPalettePointerByID(palette_id);
        JBI->Add(ptr->data_length);
      }
    JBI->Array_End();
    #endif  // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH

    // #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
    // JBI->Array_Start_P("PaletteNames");
    //   for(uint8_t palette_id=mPalette::PALETTELIST_VARIABLE_HSBID_01__ID;palette_id<mPaletteI->PALETTELIST_VARIABLE_GENERIC_LENGTH__ID;palette_id++) // Some
    //   // for(uint16_t palette_id=mPalette::PALETTELIST_VARIABLE_HSBID_01__ID;palette_id<mPaletteI->PALETTELIST_VARIABLE_GENERIC_LENGTH__ID;palette_id++) // All
    //   {
    //     JBI->Add(GetPaletteNameByID(palette_id, buffer, sizeof(buffer)));
    //     // ALOG_INF(PSTR("Name[%d] = %s"),palette_id,GetPaletteNameByID(palette_id, buffer, sizeof(buffer)));
    //   }
    // JBI->Array_End();
    // #endif  // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
    JBI->Array_Start_P("PaletteNames");
      for(uint16_t palette_id=mPalette::PALETTELIST_VARIABLE_HSBID_01__ID;palette_id<mPaletteI->PALETTELIST_STATIC_CHRISTMAS_28__ID;palette_id++) // Some
      // for(uint8_t palette_id=mPalette::PALETTELIST_VARIABLE_HSBID_01__ID;palette_id<mPaletteI->PALETTELIST_TOTAL_LENGTH;palette_id++) // All
      {
        // Option 
        // ALOG_INF(PSTR("Name[%d] = %s"),GetPaletteNameByID(palette_id, buffer, sizeof(buffer)));

        // JBI->Add(GetPaletteNameByID(palette_id, buffer, sizeof(buffer)));
      }
    JBI->Array_End();
    #endif  // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST


    uint16_t id = 0;
    uint16_t pixel = 0;
    uint8_t encoded_value = 0;
    
    // // uint16_t count   = mPaletteI->GetLengthFromPaletteAdvanced(id,pixel,&encoded_value,true,true,255);
    // RgbcctColor colour = mPaletteI->GetColourFromPaletteAdvanced(id,pixel,&encoded_value,true,true,255);

    // JBI->Array_Start("Palette");
    // for(int i=0;i<MAX_NUM_SEGMENTS;i++)
    // {
    //   JBI->Add(encoded_value);
    //   JBI->Add(colour.R);
    //   JBI->Add(colour.G);
    //   JBI->Add(colour.B);
    //   JBI->Add(colour.W1);
    //   JBI->Add(colour.W2);
    // }
    // JBI->Array_End();
    
//   JsonBuilderI->Start();  
//     JsonBuilderI->Add_P(PM_JSON_SIZE, pCONT_iLight->settings.light_size_count);
//     JBI->Add("PaletteMaxID", (uint8_t)mPalette::PALETTELIST_STATIC_LENGTH__ID);
//     JBI->Add("ColourPaletteID", pCONT_lAni->SEGMENT_I(0).palette.id );
//     JBI->Add("ColourPalette", mPaletteI->GetPaletteNameByID( SEGMENT_I(0).palette.id, buffer, sizeof(buffer)));
//     // JsonBuilderI->Array_Start("rgb");
//     // for(int i=0;i<numpixels;i++){
//     //   RgbTypeColor c = GetPixelColor(i);
//     //   JsonBuilderI->Add_FV(PSTR("%02X%02X%02X"),c.R,c.G,c.B);
//     // }
//     // JsonBuilderI->Array_End();
//   return JsonBuilderI->End();


    /**
     * @brief Moving towards preloading palettes from memory into ram/heap for speed (then iram will work)
     * 
     */
    JBI->Object_Start("LoadPalette");

      uint8_t segment_index = 0;

    JBI->Object_End();
    

return JBI->End();

}

#endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE


#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS

/**
 * @brief Multiple large segments may require mutiple topics
 */
uint8_t mAnimatorLight::ConstructJSON_Debug_Segments(uint8_t json_level, bool json_appending)
{

  JBI->Start();

  JBI->Add("Brightness_Master",    pCONT_iLight->getBri_Global());
  JBI->Add("BrightnessRGB_Master", pCONT_iLight->getBriRGB_Global());
  JBI->Add("BrightnessCCT_Master", pCONT_iLight->getBriCCT_Global());

  for(uint8_t seg_i =0; seg_i < getSegmentsNum(); seg_i++)
  {

    Segment_New &seg = getSegment(seg_i);
    JBI->Object_Start_F("Segment%d",seg_i);

      JBI->Add("BrightnessRGB", seg.getBrightnessRGB());
      JBI->Add("BrightnessCCT", seg.getBrightnessCCT());
      
      JBI->Add("BrightnessRGB_wMaster", seg.getBrightnessRGB_WithGlobalApplied());
      JBI->Add("BrightnessCCT_wMaster", seg.getBrightnessCCT_WithGlobalApplied());
      
      JBI->Array_Start("PixelRange");
        JBI->Add(seg.pixel_range.start);
        JBI->Add(seg.pixel_range.stop);
      JBI->Array_End();
      JBI->Add("Offset",    seg.offset);
      JBI->Add("Speed",     seg.speed());
      JBI->Add("Intensity", seg.intensity());
      JBI->Object_Start("Options");
        JBI->Add("Selected",     seg.selected);
        JBI->Add("Reverse",      seg.reverse);
        JBI->Add("On",           seg.on);
        JBI->Add("Mirror",       seg.mirror);
        JBI->Add("Freeze",       seg.freeze);
        JBI->Add("Reset",        seg.reset);
        JBI->Add("Transitional", seg.transitional);
        JBI->Add("Reverse_y",    seg.reverse_y);
        JBI->Add("Mirror_y",     seg.mirror_y);
        JBI->Add("Transpose",    seg.transpose);
        JBI->Add("Map1D2D",      seg.map1D2D);
        JBI->Add("SoundSim",     seg.soundSim);
      JBI->Object_End();
      JBI->Add("ColourType",     (uint8_t)seg.colour_type);
      JBI->Object_Start("Transition");
        JBI->Add("Rate",         seg.transition.rate_ms);
        JBI->Add("Time",         seg.transition.time_ms);
      JBI->Object_End();
      JBI->Object_Start("RgbcctColours");
      for(uint8_t rgb_i = 0; rgb_i<2; rgb_i++)
      {
        JBI->Array_Start_P("Colour%d", rgb_i);
        for(uint8_t c_i=0;c_i<5;c_i++)
        {
          JBI->Add(seg.rgbcctcolors[rgb_i].raw[c_i]);
        }
        JBI->Array_End();
        JBI->Object_Start("ColourTemp");
          JBI->Add("Min",      seg.rgbcctcolors[rgb_i].get_CTRangeMin());
          JBI->Add("Max",      seg.rgbcctcolors[rgb_i].get_CTRangeMax());
          JBI->Add("Set",      seg.rgbcctcolors[rgb_i].getCCT());
        JBI->Object_End();
      }
      JBI->Object_End();
        
    JBI->Object_End();

  } // END seg_i

  return JBI->End();

}

#endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS


#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR

/**
 * @brief Multiple large segments may require mutiple topics
 */
uint8_t mAnimatorLight::ConstructJSON_Debug_Palette_Vector(uint8_t json_level, bool json_appending)
{

  JBI->Start();
  JBI->Add("size", mPaletteI->static_palettes.size());


    char lineBuffer[100] = {0};
    bool flag_get_first_name_only = true;

    JBI->Array_Start("pals");
    // for(uint8_t i=0;i<20;i++)//auto& pal: mPaletteI->palettelist)

    uint16_t i = 0;

    for(auto& pal: mPaletteI->static_palettes)
    {
      // mPalette::PALETTE pal = mPaletteI->palettelist[i];
      JBI->Object_Start();
          
          // pCONT_lAni->GetPaletteNameByID(i, lineBuffer, sizeof(lineBuffer));
          // // snprintf_P(lineBuffer,sizeof(lineBuffer),"%S",pal.friendly_name_ctr); 
          // if(flag_get_first_name_only)
          // {    
          //   char* dataPtr = strchr(lineBuffer,'|');
          //   if (dataPtr) *dataPtr = 0; // replace name dividor with null termination early
          // }
          // // ALOG_INF(PSTR("pal=\"%s\""), lineBuffer);
          // // ALOG_INF(PSTR("pal[%d]=\"%s\""), i, lineBuffer);
          // if(i<10)
          //   JBI->Add("n",lineBuffer);



        // JBI->Add("n", pal.friendly_name_ctr);
        // JBI->Add("i", pal.id);
        
    uint8_t colours_in_palette = pCONT_lAni->GetNumberOfColoursInPalette(pal.id);
        JBI->Add("s",colours_in_palette);



    
        // JBI->Add_FV("e", "%4X", pal.encoding);
        // JBI->Array_Start("d");
        //   for(uint8_t i=0;i<pal.data_length;i++)
        //   {
        //     JBI->Add(pal.data[i]);
        //   }
        // JBI->Array_End();
      JBI->Object_End();

      i++;
    }
    JBI->Array_End();

  return JBI->End();

}

#endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR


#ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
//   uint8_t mAnimatorLight::ConstructJSON_Debug_Animations_Progress(uint8_t json_level, bool json_appending)
//   {

// JBI->Start();

//   JBI->Add("millis", millis());

// return JBI->End();

// }

  ANIMIMATION_DEBUG_MQTT_FUNCTION_SIGNATURE;
  mAnimatorLight& setCallback_ConstructJSONBody_Debug_Animations_Progress(ANIMIMATION_DEBUG_MQTT_FUNCTION_SIGNATURE);  
#endif // USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK


#ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
uint8_t mAnimatorLight::ConstructJSON_Debug_Animations_Progress(uint8_t json_level, bool json_appending)
{

  if(anim_progress_mqtt_function_callback)
  {
    JsonBuilderI->Start();
    anim_progress_mqtt_function_callback(); // Call the function
    return JsonBuilderI->End();
  }
  return false;

}
#endif // USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK


/******************************************************************************************************************
 * mInterfaceLight_MQTT.cpp
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mAnimatorLight::MQTTHandler_Init()
{

  struct handler<mAnimatorLight>* ptr;
  
  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_segments_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__SEGMENTS_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Segments;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_playlists_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__PLAYLISTS_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Playlist;
  mqtthandler_list.push_back(ptr);
  
  #ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
  ptr = &mqtthandler_mode_ambilight_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__MODE_AMBILIGHT__CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Mode_Ambilight;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT

  #ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
  ptr = &mqtthandler_manual_setpixel;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__MODE_MANUAL_SETPIXEL_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Mode_SetManual;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
    
  // #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  // ptr = &mqtthandler_automation_presets;
  // ptr->tSavedLastSent = millis();
  // ptr->flags.PeriodicEnabled = true;
  // ptr->flags.SendNow = true;
  // ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.teleperiod_secs; 
  // ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  // ptr->json_level = JSON_LEVEL_DETAILED;
  // ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__AUTOMATION_PRESETS_CTR;
  // ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Auto_Presets;
  // mqtthandler_list.push_back(ptr);
  // #endif // ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    
  #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PLAYLISTS
  ptr = &mqtthandler_manual_setpixel;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__MODE_MANUAL_SETPIXEL_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Mode_SetManual;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_FEATURE_PIXEL__AUTOMATION_PLAYLISTS

  #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
  ptr = &mqtthandler_debug_palette;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_PALETTE__CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Debug_Palette;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE

  #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
  ptr                         = &mqtthandler_debug_segments;
  ptr->tSavedLastSent         = millis();
  ptr->flags.PeriodicEnabled  = true;
  ptr->flags.SendNow          = true;
  ptr->tRateSecs              = pCONT_set->Settings.sensors.ifchanged_secs; 
  ptr->topic_type             = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level             = JSON_LEVEL_DETAILED;
  ptr->postfix_topic          = PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_SEGMENTS__CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Debug_Segments;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS

  #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
  ptr = &mqtthandler_debug_animations_progress;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__ANIMATIONS_PROGRESS_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Debug_Animations_Progress;
  mqtthandler_list.push_back(ptr);
  #endif

  #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
  ptr                         = &mqtthandler_debug_palette_vector;
  ptr->tSavedLastSent         = millis();
  ptr->flags.PeriodicEnabled  = true;
  ptr->flags.SendNow          = true;
  ptr->tRateSecs              = 1; 
  ptr->topic_type             = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level             = JSON_LEVEL_DETAILED;
  ptr->postfix_topic          = PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_PALETTE_VECTOR__CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Debug_Palette_Vector;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR

} //end "MQTTHandler_Init"

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mAnimatorLight::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mAnimatorLight::MQTTHandler_Set_DefaultPeriodRate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
  }
}

/**
 * @brief MQTTHandler_Sender
 * */
void mAnimatorLight::MQTTHandler_Sender(uint8_t id)
{    
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_LIGHTS_ANIMATOR_ID, handle, id);
  }
}

/**
 * @brief MQTTHandler_AddWebURL_PayloadRequests
 * */
#ifdef USE_MODULE_NETWORK_WEBSERVER23
void mAnimatorLight::MQTTHandler_AddWebURL_PayloadRequests()
{    

  #ifdef ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
  // MQTTHandler_AddWebURL_PayloadRequests2();
  #else
  char uri_buffer[70] = {0};
  snprintf(uri_buffer, sizeof(uri_buffer), "/mqtt/%s/%S", D_TOPIC_STATUS, GetModuleFriendlyName());
  pCONT_web->server->on(uri_buffer, HTTP_GET, 
  [this](AsyncWebServerRequest *request)
    {
      char handle_url[100] = {0};       
      for(auto& handle:mqtthandler_list)
      {      
        pCONT_mqtt->TopicFormatted( GetModuleFriendlyName(), handle->topic_type, handle->postfix_topic, handle_url, sizeof(handle_url)); 
        ALOG_INF(PSTR("handle_url \"%s\" -> \"%s\""), request->url().c_str(), handle_url);
        const String& incoming_uri = request->url();
        if(incoming_uri.indexOf(handle_url) > 0)
        {        
          uint8_t fSendPayload = CALL_MEMBER_FUNCTION(*this, handle->ConstructJSON_function)(handle->json_level, true);
          ALOG_INF(PSTR("data_buffer=%s"), data_buffer.payload.ctr);
          request->send(200, PM_WEB_CONTENT_TYPE_APPLICATION_JSON_JAVASCRIPT, data_buffer.payload.ctr); 
          break; // to stop accidental double matches, only respond once
        }
      }
    }
  );
  #endif

  // pCONT_web->server->on(uri_buffer, HTTP_GET, 
  // []({AsyncWebServerRequest *request, uint8_t test})
  //   {
  //     // char handle_url[100] = {0};       
  //     // for(auto& handle:mqtthandler_list)
  //     // {      
  //     //   pCONT_mqtt->TopicFormatted( GetModuleFriendlyName(), handle->topic_type, handle->postfix_topic, handle_url, sizeof(handle_url)); 
  //     //   ALOG_INF(PSTR("handle_url \"%s\" -> \"%s\""), request->url().c_str(), handle_url);
  //     //   const String& incoming_uri = request->url();
  //     //   if(incoming_uri.indexOf(handle_url) > 0)
  //     //   {        
  //     //     uint8_t fSendPayload = CALL_MEMBER_FUNCTION(*this, handle->ConstructJSON_function)(handle->json_level, true);
  //     //     ALOG_INF(PSTR("data_buffer=%s"), data_buffer.payload.ctr);
  //     //     request->send(200, PM_WEB_CONTENT_TYPE_APPLICATION_JSON_JAVASCRIPT, data_buffer.payload.ctr); 
  //     //     break; // to stop accidental double matches, only respond once
  //     //   }
  //     // }
  //   }
  // );


}
#endif // USE_MODULE_NETWORK_WEBSERVER23


#ifdef ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
void mAnimatorLight::MQTTHandler_AddWebURL_PayloadRequests2()
{    
  for(auto& handle:mqtthandler_list){
    // pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_LIGHTS_ANIMATOR_ID, handle, id);

    pCONT_mqtt->MQTTHandler_AddWebURL(*this, EM_MODULE_LIGHTS_ANIMATOR_ID, handle);


  }
}
#endif // ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED





  
#endif// USE_MODULE_NETWORK_MQTT



// //gamma 2.8 lookup table used for color correction
// uint8_t mAnimatorLight::NeoGammaWLEDMethod::gammaT[256] = {
//     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
//     1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
//     2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
//     5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
//    10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
//    17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
//    25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
//    37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
//    51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
//    69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
//    90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
//   115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
//   144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
//   177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
//   215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

// // re-calculates & fills gamma table
// void mAnimatorLight::NeoGammaWLEDMethod::calcGammaTable(float gamma)
// {
//   for (size_t i = 0; i < 256; i++) {
//     gammaT[i] = (int)(powf((float)i / 255.0f, gamma) * 255.0f + 0.5f);
//   }
// }

// uint8_t mAnimatorLight::NeoGammaWLEDMethod::Correct(uint8_t value)
// {
//   // if (!gammaCorrectCol) return value;
//   // return gammaT[value];
// }

// // used for color gamma correction
// uint32_t mAnimatorLight::NeoGammaWLEDMethod::Correct32(uint32_t color)
// {
//   // if (!gammaCorrectCol) return color;
//   // uint8_t w = W(color);
//   // uint8_t r = R(color);
//   // uint8_t g = G(color);
//   // uint8_t b = B(color);
//   // w = gammaT[w];
//   // r = gammaT[r];
//   // g = gammaT[g];
//   // b = gammaT[b];
//   // return RGBW32(r, g, b, w);
// }



/*
 * UDP sync notifier / Realtime / Hyperion / TPM2.NET
 */

#define UDP_SEG_SIZE 36
#define SEG_OFFSET (41+(MAX_NUM_SEGMENTS*UDP_SEG_SIZE))
#define WLEDPACKETSIZE (41+(MAX_NUM_SEGMENTS*UDP_SEG_SIZE)+0)
#define UDP_IN_MAXSIZE 1472
#define PRESUMED_NETWORK_DELAY 3 //how many ms could it take on avg to reach the receiver? This will be added to transmitted times

#define Network WiFi


void realtimeLock2(uint32_t timeoutMs, byte md);

#define WLED_GLOBAL
#define _INIT(x) = x


//Notifier callMode
#define CALL_MODE_INIT           0     //no updates on init, can be used to disable updates
#define CALL_MODE_DIRECT_CHANGE  1
#define CALL_MODE_BUTTON         2     //default button actions applied to selected segments
#define CALL_MODE_NOTIFICATION   3
#define CALL_MODE_NIGHTLIGHT     4
#define CALL_MODE_NO_NOTIFY      5
#define CALL_MODE_FX_CHANGED     6     //no longer used
#define CALL_MODE_HUE            7
#define CALL_MODE_PRESET_CYCLE   8
#define CALL_MODE_BLYNK          9     //no longer used
#define CALL_MODE_ALEXA         10
#define CALL_MODE_WS_SEND       11     //special call mode, not for notifier, updates websocket only
#define CALL_MODE_BUTTON_PRESET 12     //button/IR JSON preset/macro

void setRealtimePixel(uint16_t i, byte r, byte g, byte b, byte w = 0);

#ifdef ENABLE_WEBSERVER_LIGHTING_WEBUI

//scales the brightness with the briMultiplier factor
byte scaledBri2(byte in)
{
  uint16_t val = ((uint16_t)in*pCONT_lAni->briMultiplier)/100;
  if (val > 255) val = 255;
  return (byte)val;
}


void mAnimatorLight::notify(byte callMode, bool followUp)
{
  if (!pCONT_lAni->udpConnected) return;
  if (!pCONT_lAni->syncGroups) return;
  switch (callMode)
  {
    case CALL_MODE_INIT:          return;
    case CALL_MODE_DIRECT_CHANGE: if (!pCONT_lAni->notifyDirect) return; break;
    case CALL_MODE_BUTTON:        if (!pCONT_lAni->notifyButton) return; break;
    case CALL_MODE_BUTTON_PRESET: if (!pCONT_lAni->notifyButton) return; break;
    case CALL_MODE_NIGHTLIGHT:    if (!pCONT_lAni->notifyDirect) return; break;
    case CALL_MODE_HUE:           if (!pCONT_lAni->notifyHue)    return; break;
    case CALL_MODE_PRESET_CYCLE:  if (!pCONT_lAni->notifyDirect) return; break;
    case CALL_MODE_ALEXA:         if (!pCONT_lAni->notifyAlexa)  return; break;
    default: return;
  }
  byte udpOut[WLEDPACKETSIZE];
  mAnimatorLight::Segment_New& mainseg = pCONT_lAni->getMainSegment();
  udpOut[0] = 0; //0: wled notifier protocol 1: WARLS protocol
  udpOut[1] = callMode;
  udpOut[2] = pCONT_iLight->getBri_Global();
  uint32_t col = mainseg.rgbcctcolors[0].getU32();
  udpOut[3] = R(col);
  udpOut[4] = G(col);
  udpOut[5] = B(col);
  udpOut[6] = pCONT_lAni->nightlightActive;
  udpOut[7] = pCONT_lAni->nightlightDelayMins;
  udpOut[8] = mainseg.effect_id;
  udpOut[9] = mainseg.speed();
  udpOut[10] = W(col);
  //compatibilityVersionByte:
  //0: old 1: supports white 2: supports secondary color
  //3: supports FX intensity, 24 byte packet 4: supports transitionDelay 5: sup palette
  //6: supports timebase syncing, 29 byte packet 7: supports tertiary color 8: supports sys time sync, 36 byte packet
  //9: supports sync groups, 37 byte packet 10: supports CCT, 39 byte packet 11: per segment options, variable packet length (40+MAX_NUM_SEGMENTS*3)
  //12: enhanced effct sliders, 2D & mapping options
  udpOut[11] = 12;
  col = mainseg.rgbcctcolors[1].getU32();
  udpOut[12] = R(col);
  udpOut[13] = G(col);
  udpOut[14] = B(col);
  udpOut[15] = W(col);
  udpOut[16] = mainseg.intensity();
  udpOut[17] = (pCONT_lAni->transitionDelay >> 0) & 0xFF;
  udpOut[18] = (pCONT_lAni->transitionDelay >> 8) & 0xFF;
  udpOut[19] = mainseg.palette.id;
  col = mainseg.rgbcctcolors[2].getU32();
  udpOut[20] = R(col);
  udpOut[21] = G(col);
  udpOut[22] = B(col);
  udpOut[23] = W(col);

  udpOut[24] = followUp;
  uint32_t t = millis() + pCONT_lAni->timebase;
  udpOut[25] = (t >> 24) & 0xFF;
  udpOut[26] = (t >> 16) & 0xFF;
  udpOut[27] = (t >>  8) & 0xFF;
  udpOut[28] = (t >>  0) & 0xFF;

  //sync system time
  udpOut[29] = pCONT_lAni->toki.getTimeSource();
  Toki::Time tm = pCONT_lAni->toki.getTime();
  uint32_t unix = tm.sec;
  udpOut[30] = (unix >> 24) & 0xFF;
  udpOut[31] = (unix >> 16) & 0xFF;
  udpOut[32] = (unix >>  8) & 0xFF;
  udpOut[33] = (unix >>  0) & 0xFF;
  uint16_t ms = tm.ms;
  udpOut[34] = (ms >> 8) & 0xFF;
  udpOut[35] = (ms >> 0) & 0xFF;

  //sync groups
  udpOut[36] = pCONT_lAni->syncGroups;

  //Might be changed to Kelvin in the future, receiver code should handle that case
  //0: byte 38 contains 0-255 value, 255: no valid CCT, 1-254: Kelvin value MSB
  udpOut[37] = pCONT_lAni->hasCCTBus() ? 0 : 255; //check this is 0 for the next value to be significant
  udpOut[38] = mainseg.cct;

  udpOut[39] = pCONT_lAni->getActiveSegmentsNum();
  udpOut[40] = UDP_SEG_SIZE; //size of each loop iteration (one segment)
  size_t s = 0, nsegs = pCONT_lAni->getSegmentsNum();
  for (size_t i = 0; i < nsegs; i++) {
    mAnimatorLight::Segment_New &selseg = pCONT_lAni->getSegment(i);
    if (!selseg.isActive()) continue;
    uint16_t ofs = 41 + s*UDP_SEG_SIZE; //start of segment offset byte
    udpOut[0 +ofs] = s;
    udpOut[1 +ofs] = selseg.pixel_range.start >> 8;
    udpOut[2 +ofs] = selseg.pixel_range.start & 0xFF;
    udpOut[3 +ofs] = selseg.pixel_range.stop >> 8;
    udpOut[4 +ofs] = selseg.pixel_range.stop & 0xFF;
    udpOut[5 +ofs] = selseg.grouping;
    udpOut[6 +ofs] = selseg.spacing;
    udpOut[7 +ofs] = selseg.offset >> 8;
    udpOut[8 +ofs] = selseg.offset & 0xFF;
    udpOut[9 +ofs] = selseg.options & 0x8F; //only take into account selected, mirrored, on, reversed, reverse_y (for 2D); ignore freeze, reset, transitional
    udpOut[10+ofs] = selseg.opacity;
    udpOut[11+ofs] = selseg.effect_id;
    udpOut[12+ofs] = selseg.speed();
    udpOut[13+ofs] = selseg.intensity();
    udpOut[14+ofs] = selseg.palette.id;
    udpOut[15+ofs] = R(selseg.rgbcctcolors[0].getU32());
    udpOut[16+ofs] = G(selseg.rgbcctcolors[0].getU32());
    udpOut[17+ofs] = B(selseg.rgbcctcolors[0].getU32());
    udpOut[18+ofs] = W(selseg.rgbcctcolors[0].getU32());
    udpOut[19+ofs] = R(selseg.rgbcctcolors[1].getU32());
    udpOut[20+ofs] = G(selseg.rgbcctcolors[1].getU32());
    udpOut[21+ofs] = B(selseg.rgbcctcolors[1].getU32());
    udpOut[22+ofs] = W(selseg.rgbcctcolors[1].getU32());
    udpOut[23+ofs] = R(selseg.rgbcctcolors[2].getU32());
    udpOut[24+ofs] = G(selseg.rgbcctcolors[2].getU32());
    udpOut[25+ofs] = B(selseg.rgbcctcolors[2].getU32());
    udpOut[26+ofs] = W(selseg.rgbcctcolors[2].getU32());
    udpOut[27+ofs] = selseg.cct;
    udpOut[28+ofs] = (selseg.options>>8) & 0xFF; //mirror_y, transpose, 2D mapping & sound
    udpOut[29+ofs] = selseg.custom1;
    udpOut[30+ofs] = selseg.custom2;
    udpOut[31+ofs] = selseg.custom3 | (selseg.check1<<5) | (selseg.check2<<6) | (selseg.check3<<7);
    udpOut[32+ofs] = selseg.startY >> 8;
    udpOut[33+ofs] = selseg.startY & 0xFF;
    udpOut[34+ofs] = selseg.stopY >> 8;
    udpOut[35+ofs] = selseg.stopY & 0xFF;
    ++s;
  }

  //uint16_t offs = SEG_OFFSET;
  //next value to be added has index: udpOut[offs + 0]

  IPAddress broadcastIp;
  broadcastIp = ~uint32_t(Network.subnetMask()) | uint32_t(Network.gatewayIP());

  pCONT_lAni->notifierUdp.beginPacket(broadcastIp, pCONT_lAni->udpPort);
  pCONT_lAni->notifierUdp.write(udpOut, WLEDPACKETSIZE);
  pCONT_lAni->notifierUdp.endPacket();
  pCONT_lAni->notificationSentCallMode = callMode;
  pCONT_lAni->notificationSentTime = millis();
  pCONT_lAni->notificationCount = followUp ? pCONT_lAni->notificationCount + 1 : 0;
}

void realtimeLock2(uint32_t timeoutMs, byte md)
{
  if (!pCONT_lAni->realtimeMode && !pCONT_lAni->realtimeOverride) {
    uint16_t stop, start;
    if (pCONT_lAni->useMainSegmentOnly) {
      mAnimatorLight::Segment_New& mainseg = pCONT_lAni->getMainSegment();
      start = mainseg.pixel_range.start;
      stop  = mainseg.pixel_range.stop;
      mainseg.freeze = true;
    } else {
      start = 0;
      stop  = pCONT_lAni->getLengthTotal();
    }
    // clear strip/segment
    for (size_t i = start; i < stop; i++) pCONT_lAni->setPixelColor(i,BLACK);
    // if WLED was off and using main segment only, freeze non-main segments so they stay off
    if (pCONT_lAni->useMainSegmentOnly && pCONT_iLight->getBri_Global() == 0) {
      for (size_t s=0; s < pCONT_lAni->getSegmentsNum(); s++) {
        pCONT_lAni->getSegment(s).freeze = true;
      }
    }
  }
  // if strip is off (bri==0) and not already in RTM
  if (pCONT_lAni->briT == 0 && !pCONT_lAni->realtimeMode && !pCONT_lAni->realtimeOverride) {
    pCONT_lAni->setBrightness(scaledBri2(pCONT_lAni->briLast), true);
  }

  if (pCONT_lAni->realtimeTimeout != UINT32_MAX) {
    pCONT_lAni->realtimeTimeout = (timeoutMs == 255001 || timeoutMs == 65000) ? UINT32_MAX : millis() + timeoutMs;
  }
  pCONT_lAni->realtimeMode = md;

  if (pCONT_lAni->realtimeOverride) return;
  if (pCONT_lAni->arlsForceMaxBri) pCONT_lAni->setBrightness(scaledBri2(255), true);
  if (pCONT_lAni->briT > 0 && md == REALTIME_MODE_GENERIC) pCONT_lAni->show();
}

void exitRealtime2() {
  if (!pCONT_lAni->realtimeMode) return;
  if (pCONT_lAni->realtimeOverride == REALTIME_OVERRIDE_ONCE) pCONT_lAni->realtimeOverride = REALTIME_OVERRIDE_NONE;
  pCONT_lAni->setBrightness(scaledBri2(pCONT_iLight->getBri_Global()), true);
  pCONT_lAni->realtimeTimeout = 0; // cancel realtime mode immediately
  pCONT_lAni->realtimeMode = REALTIME_MODE_INACTIVE; // inform UI immediately
  pCONT_lAni->realtimeIP[0] = 0;
  if (pCONT_lAni->useMainSegmentOnly) { // unfreeze live segment again
    pCONT_lAni->getMainSegment().freeze = false;
  }
  // updateInterfaces(CALL_MODE_WS_SEND);
}


#define TMP2NET_OUT_PORT 65442

void sendTPM2Ack() {
  pCONT_lAni->notifierUdp.beginPacket(pCONT_lAni->notifierUdp.remoteIP(), TMP2NET_OUT_PORT);
  uint8_t response_ack = 0xac;
  pCONT_lAni->notifierUdp.write(&response_ack, 1);
  pCONT_lAni->notifierUdp.endPacket();
}


void handleNotifications()
{
  IPAddress localIP;

  //send second notification if enabled
  if(pCONT_lAni->udpConnected && (pCONT_lAni->notificationCount < pCONT_lAni->udpNumRetries) && ((millis()-pCONT_lAni->notificationSentTime) > 250)){
    // notify(pCONT_lAni->notificationSentCallMode,true);
  }

  if (pCONT_lAni->e131NewData && millis() - pCONT_lAni->getLastShow() > 15)
  {
    pCONT_lAni->e131NewData = false;
    pCONT_lAni->show();
  }

  //unlock strip when realtime UDP times out
  if (pCONT_lAni->realtimeMode && millis() > pCONT_lAni->realtimeTimeout) exitRealtime2();

  //receive UDP notifications
  if (!pCONT_lAni->udpConnected) return;

  bool isSupp = false;
  size_t packetSize = pCONT_lAni->notifierUdp.parsePacket();
  if (!packetSize && pCONT_lAni->udp2Connected) {
    packetSize = pCONT_lAni->notifier2Udp.parsePacket();
    isSupp = true;
  }

  //hyperion / raw RGB
  if (!packetSize && pCONT_lAni->udpRgbConnected) {
    packetSize = pCONT_lAni->rgbUdp.parsePacket();
    if (packetSize) {
      if (!pCONT_lAni->receiveDirect) return;
      if (packetSize > UDP_IN_MAXSIZE || packetSize < 3) return;
      pCONT_lAni->realtimeIP = pCONT_lAni->rgbUdp.remoteIP();
      DEBUG_PRINTLN( pCONT_lAni->rgbUdp.remoteIP());
      uint8_t lbuf[packetSize];
      pCONT_lAni->rgbUdp.read(lbuf, packetSize);
      realtimeLock2(pCONT_lAni->realtimeTimeoutMs, REALTIME_MODE_HYPERION);
      if (pCONT_lAni->realtimeOverride && !(pCONT_lAni->realtimeMode && pCONT_lAni->useMainSegmentOnly)) return;
      uint16_t id = 0;
      uint16_t totalLen = pCONT_lAni->getLengthTotal();
      for (size_t i = 0; i < packetSize -2; i += 3)
      {
        setRealtimePixel(id, lbuf[i], lbuf[i+1], lbuf[i+2], 0);
        id++; if (id >= totalLen) break;
      }
      if (!(pCONT_lAni->realtimeMode && pCONT_lAni->useMainSegmentOnly)) pCONT_lAni->show();
      return;
    }
  }

  if (!(pCONT_lAni->receiveNotifications || pCONT_lAni->receiveDirect)) return;

  localIP = Network.localIP();
  //notifier and UDP realtime
  if (!packetSize || packetSize > UDP_IN_MAXSIZE) return;
  if (!isSupp && pCONT_lAni->notifierUdp.remoteIP() == localIP) return; //don't process broadcasts we send ourselves

  uint8_t udpIn[packetSize +1];
  uint16_t len;
  if (isSupp) len = pCONT_lAni->notifier2Udp.read(udpIn, packetSize);
  else        len =  pCONT_lAni->notifierUdp.read(udpIn, packetSize);

  // // WLED nodes info notifications
  // if (isSupp && udpIn[0] == 255 && udpIn[1] == 1 && len >= 40) {
  //   if (!pCONT_lAni->nodeListEnabled || pCONT_lAni->notifier2Udp.remoteIP() == localIP) return;

  //   uint8_t unit = udpIn[39];
  //   NodesMap::iterator it = Nodes.find(unit);
  //   if (it == Nodes.end() && Nodes.size() < WLED_MAX_NODES) { // Create a new element when not present
  //     Nodes[unit].age = 0;
  //     it = Nodes.find(unit);
  //   }

  //   if (it != Nodes.end()) {
  //     for (size_t x = 0; x < 4; x++) {
  //       it->second.ip[x] = udpIn[x + 2];
  //     }
  //     it->second.age = 0; // reset 'age counter'
  //     char tmpNodeName[33] = { 0 };
  //     memcpy(&tmpNodeName[0], reinterpret_cast<byte *>(&udpIn[6]), 32);
  //     tmpNodeName[32]     = 0;
  //     it->second.nodeName = tmpNodeName;
  //     it->second.nodeName.trim();
  //     it->second.nodeType = udpIn[38];
  //     uint32_t build = 0;
  //     if (len >= 44)
  //       for (size_t i=0; i<sizeof(uint32_t); i++)
  //         build |= udpIn[40+i]<<(8*i);
  //     it->second.build = build;
  //   }
  //   return;
  // }

  //wled notifier, ignore if realtime packets active
  if (udpIn[0] == 0 && !pCONT_lAni->realtimeMode && pCONT_lAni->receiveNotifications)
  {
    //ignore notification if received within a second after sending a notification ourselves
    if (millis() - pCONT_lAni->notificationSentTime < 1000) return;
    if (udpIn[1] > 199) return; //do not receive custom versions

    //compatibilityVersionByte:
    byte version = udpIn[11];

    // if we are not part of any sync group ignore message
    if (version < 9 || version > 199) {
      // legacy senders are treated as if sending in sync group 1 only
      if (!(pCONT_lAni->receiveGroups & 0x01)) return;
    } else if (!(pCONT_lAni->receiveGroups & udpIn[36])) return;

    bool someSel = (pCONT_lAni->receiveNotificationBrightness || pCONT_lAni->receiveNotificationColor || pCONT_lAni->receiveNotificationEffects);

    //apply colors from notification to main segment, only if not syncing full segments
    if ((pCONT_lAni->receiveNotificationColor || !someSel) && (version < 11 || !pCONT_lAni->receiveSegmentOptions)) {
      // primary color, only apply white if intented (version > 0)
      pCONT_lAni->setColor(0, RGBW32(udpIn[3], udpIn[4], udpIn[5], (version > 0) ? udpIn[10] : 0));
      if (version > 1) {
        pCONT_lAni->setColor(1, RGBW32(udpIn[12], udpIn[13], udpIn[14], udpIn[15])); // secondary color
      }
      if (version > 6) {
        pCONT_lAni->setColor(2, RGBW32(udpIn[20], udpIn[21], udpIn[22], udpIn[23])); // tertiary color
        if (version > 9 && version < 200 && udpIn[37] < 255) { // valid CCT/Kelvin value
          uint16_t cct = udpIn[38];
          if (udpIn[37] > 0) { //Kelvin
            cct |= (udpIn[37] << 8);
          }
          pCONT_lAni->setCCT(cct);
        }
      }
    }

    bool timebaseUpdated = false;
    //apply effects from notification
    bool applyEffects = (pCONT_lAni->receiveNotificationEffects || !someSel);
    if (version < 200)
    {
      // if (applyEffects && currentPlaylist >= 0) unloadPlaylist();
      if (version > 10 && (pCONT_lAni->receiveSegmentOptions || pCONT_lAni->receiveSegmentBounds)) {
        uint8_t numSrcSegs = udpIn[39];
        for (size_t i = 0; i < numSrcSegs; i++) {
          uint16_t ofs = 41 + i*udpIn[40]; //start of segment offset byte
          uint8_t id = udpIn[0 +ofs];
          if (id > pCONT_lAni->getSegmentsNum()) break;

          mAnimatorLight::Segment_New& selseg = pCONT_lAni->getSegment(id);
          if (!selseg.isActive() || !selseg.isSelected()) continue; //do not apply to non selected segments

          uint16_t startY = 0, start  = (udpIn[1+ofs] << 8 | udpIn[2+ofs]);
          uint16_t stopY  = 1, stop   = (udpIn[3+ofs] << 8 | udpIn[4+ofs]);
          uint16_t offset = (udpIn[7+ofs] << 8 | udpIn[8+ofs]);
          if (!pCONT_lAni->receiveSegmentOptions) {
            selseg.setUp(start, stop, selseg.grouping, selseg.spacing, offset, startY, stopY);
            continue;
          }
          //for (size_t j = 1; j<4; j++) selseg.setOption(j, (udpIn[9 +ofs] >> j) & 0x01); //only take into account mirrored, on, reversed; ignore selected
          selseg.options = (selseg.options & 0x0071U) | (udpIn[9 +ofs] & 0x0E); // ignore selected, freeze, reset & transitional
          selseg.setOpacity(udpIn[10+ofs]);
          if (applyEffects) {
            pCONT_lAni->setMode(id,  udpIn[11+ofs]);
            selseg.set_speed(      udpIn[12+ofs]);
            selseg.set_intensity(  udpIn[13+ofs]);
            selseg.palette.id   = udpIn[14+ofs];
          }
          if (pCONT_lAni->receiveNotificationColor || !someSel) {
            selseg.setColor(0, RGBW32(udpIn[15+ofs],udpIn[16+ofs],udpIn[17+ofs],udpIn[18+ofs]));
            selseg.setColor(1, RGBW32(udpIn[19+ofs],udpIn[20+ofs],udpIn[21+ofs],udpIn[22+ofs]));
            selseg.setColor(2, RGBW32(udpIn[23+ofs],udpIn[24+ofs],udpIn[25+ofs],udpIn[26+ofs]));
            selseg.setCCT(udpIn[27+ofs]);
          }
          if (version > 11) {
            // when applying synced options ignore selected as it may be used as indicator of which segments to sync
            // freeze, reset & transitional should never be synced
            selseg.options = (selseg.options & 0x0071U) | (udpIn[28+ofs]<<8) | (udpIn[9 +ofs] & 0x8E); // ignore selected, freeze, reset & transitional
            if (applyEffects) {
              selseg.custom1 = udpIn[29+ofs];
              selseg.custom2 = udpIn[30+ofs];
              selseg.custom3 = udpIn[31+ofs] & 0x1F;
              selseg.check1  = (udpIn[31+ofs]>>5) & 0x1;
              selseg.check1  = (udpIn[31+ofs]>>6) & 0x1;
              selseg.check1  = (udpIn[31+ofs]>>7) & 0x1;
            }
            startY = (udpIn[32+ofs] << 8 | udpIn[33+ofs]);
            stopY  = (udpIn[34+ofs] << 8 | udpIn[35+ofs]);
          }
          if (pCONT_lAni->receiveSegmentBounds) {
            selseg.setUp(start, stop, udpIn[5+ofs], udpIn[6+ofs], offset, startY, stopY);
          } else {
            selseg.setUp(selseg.pixel_range.start, selseg.pixel_range.stop, udpIn[5+ofs], udpIn[6+ofs], selseg.offset, selseg.startY, selseg.stopY);
          }
        }
        pCONT_lAni->stateChanged = true;
      }

      // simple effect sync, applies to all selected segments
      if (applyEffects && (version < 11 || !pCONT_lAni->receiveSegmentOptions)) {
        for (size_t i = 0; i < pCONT_lAni->getSegmentsNum(); i++) {
          mAnimatorLight::Segment_New& seg = pCONT_lAni->getSegment(i);
          if (!seg.isActive() || !seg.isSelected()) continue;
          seg.setMode(udpIn[8]);
          seg.set_speed( udpIn[9]);
          if (version > 2) seg.set_intensity( udpIn[16]);
          if (version > 4) seg.setPalette(udpIn[19]);
        }
        pCONT_lAni->stateChanged = true;
      }

      if (applyEffects && version > 5) {
        uint32_t t = (udpIn[25] << 24) | (udpIn[26] << 16) | (udpIn[27] << 8) | (udpIn[28]);
        t += PRESUMED_NETWORK_DELAY; //adjust trivially for network delay
        t -= millis();
        pCONT_lAni->timebase = t;
        timebaseUpdated = true;
      }
    }

    //adjust system time, but only if sender is more accurate than self
    if (version > 7 && version < 200)
    {
      Toki::Time tm;
      tm.sec = (udpIn[30] << 24) | (udpIn[31] << 16) | (udpIn[32] << 8) | (udpIn[33]);
      tm.ms = (udpIn[34] << 8) | (udpIn[35]);
      if (udpIn[29] > pCONT_lAni->toki.getTimeSource()) { //if sender's time source is more accurate
        pCONT_lAni->toki.adjust(tm, PRESUMED_NETWORK_DELAY); //adjust trivially for network delay
        uint8_t ts = TOKI_TS_UDP;
        if (udpIn[29] > 99) ts = TOKI_TS_UDP_NTP;
        else if (udpIn[29] >= TOKI_TS_SEC) ts = TOKI_TS_UDP_SEC;
        pCONT_lAni->toki.setTime(tm, ts);
      } else if (timebaseUpdated && pCONT_lAni->toki.getTimeSource() > 99) { //if we both have good times, get a more accurate timebase
        Toki::Time myTime = pCONT_lAni->toki.getTime();
        uint32_t diff = pCONT_lAni->toki.msDifference(tm, myTime);
        pCONT_lAni->timebase -= PRESUMED_NETWORK_DELAY; //no need to presume, use difference between NTP times at send and receive points
        if (pCONT_lAni->toki.isLater(tm, myTime)) {
          pCONT_lAni->timebase += diff;
        } else {
          pCONT_lAni->timebase -= diff;
        }
      }
    }

    if (version > 3)
    {
      pCONT_lAni->transitionDelayTemp = ((udpIn[17] << 0) & 0xFF) + ((udpIn[18] << 8) & 0xFF00);
    }

    pCONT_lAni->nightlightActive = udpIn[6];
    if (pCONT_lAni->nightlightActive) pCONT_lAni->nightlightDelayMins = udpIn[7];

    if (pCONT_lAni->receiveNotificationBrightness || !someSel) pCONT_iLight->setBriRGB_Global( udpIn[2] );
    // stateUpdated(CALL_MODE_NOTIFICATION);
    return;
  }

  if (!pCONT_lAni->receiveDirect) return;

  //TPM2.NET
  if (udpIn[0] == 0x9c)
  {
    //WARNING: this code assumes that the final TMP2.NET payload is evenly distributed if using multiple packets (ie. frame size is constant)
    //if the number of LEDs in your installation doesn't allow that, please include padding bytes at the end of the last packet
    byte tpmType = udpIn[1];
    if (tpmType == 0xaa) { //TPM2.NET polling, expect answer
      sendTPM2Ack(); return;
    }
    if (tpmType != 0xda) return; //return if notTPM2.NET data

    pCONT_lAni->realtimeIP = (isSupp) ? pCONT_lAni->notifier2Udp.remoteIP() : pCONT_lAni->notifierUdp.remoteIP();
    realtimeLock2(pCONT_lAni->realtimeTimeoutMs, REALTIME_MODE_TPM2NET);
    if (pCONT_lAni->realtimeOverride && !(pCONT_lAni->realtimeMode && pCONT_lAni->useMainSegmentOnly)) return;

    pCONT_lAni->tpmPacketCount++; //increment the packet count
    if (pCONT_lAni->tpmPacketCount == 1) pCONT_lAni->tpmPayloadFrameSize = (udpIn[2] << 8) + udpIn[3]; //save frame size for the whole payload if this is the first packet
    byte packetNum = udpIn[4]; //starts with 1!
    byte numPackets = udpIn[5];

    uint16_t id = (pCONT_lAni->tpmPayloadFrameSize/3)*(packetNum-1); //start LED
    uint16_t totalLen = pCONT_lAni->getLengthTotal();
    for (size_t i = 6; i < pCONT_lAni->tpmPayloadFrameSize + 4U; i += 3)
    {
      if (id < totalLen)
      {
        setRealtimePixel(id, udpIn[i], udpIn[i+1], udpIn[i+2], 0);
        id++;
      }
      else break;
    }
    if (pCONT_lAni->tpmPacketCount == numPackets) //reset packet count and show if all packets were received
    {
      pCONT_lAni->tpmPacketCount = 0;
      pCONT_lAni->show();
    }
    return;
  }

  //UDP realtime: 1 warls 2 drgb 3 drgbw
  if (udpIn[0] > 0 && udpIn[0] < 5)
  {
    pCONT_lAni->realtimeIP = (isSupp) ? pCONT_lAni->notifier2Udp.remoteIP() : pCONT_lAni->notifierUdp.remoteIP();
    DEBUG_PRINTLN(pCONT_lAni->realtimeIP);
    if (packetSize < 2) return;

    if (udpIn[1] == 0)
    {
      pCONT_lAni->realtimeTimeout = 0;
      return;
    } else {
      realtimeLock2(udpIn[1]*1000 +1, REALTIME_MODE_UDP);
    }
    if (pCONT_lAni->realtimeOverride && !(pCONT_lAni->realtimeMode && pCONT_lAni->useMainSegmentOnly)) return;

    uint16_t totalLen = pCONT_lAni->getLengthTotal();
    if (udpIn[0] == 1) //warls
    {
      for (size_t i = 2; i < packetSize -3; i += 4)
      {
        setRealtimePixel(udpIn[i], udpIn[i+1], udpIn[i+2], udpIn[i+3], 0);
      }
    } else if (udpIn[0] == 2) //drgb
    {
      uint16_t id = 0;
      for (size_t i = 2; i < packetSize -2; i += 3)
      {
        setRealtimePixel(id, udpIn[i], udpIn[i+1], udpIn[i+2], 0);

        id++; if (id >= totalLen) break;
      }
    } else if (udpIn[0] == 3) //drgbw
    {
      uint16_t id = 0;
      for (size_t i = 2; i < packetSize -3; i += 4)
      {
        setRealtimePixel(id, udpIn[i], udpIn[i+1], udpIn[i+2], udpIn[i+3]);

        id++; if (id >= totalLen) break;
      }
    } else if (udpIn[0] == 4) //dnrgb
    {
      uint16_t id = ((udpIn[3] << 0) & 0xFF) + ((udpIn[2] << 8) & 0xFF00);
      for (size_t i = 4; i < packetSize -2; i += 3)
      {
        if (id >= totalLen) break;
        setRealtimePixel(id, udpIn[i], udpIn[i+1], udpIn[i+2], 0);
        id++;
      }
    } else if (udpIn[0] == 5) //dnrgbw
    {
      uint16_t id = ((udpIn[3] << 0) & 0xFF) + ((udpIn[2] << 8) & 0xFF00);
      for (size_t i = 4; i < packetSize -2; i += 4)
      {
        if (id >= totalLen) break;
        setRealtimePixel(id, udpIn[i], udpIn[i+1], udpIn[i+2], udpIn[i+3]);
        id++;
      }
    }
    pCONT_lAni->show();
    return;
  }

  // API over UDP
  udpIn[packetSize] = '\0';

  // if (requestJSONBufferLock(18)) {
  //   if (udpIn[0] >= 'A' && udpIn[0] <= 'Z') { //HTTP API
  //     String apireq = "win"; apireq += '&'; // reduce flash string usage
  //     apireq += (char*)udpIn;
  //     handleSet(nullptr, apireq);
  //   } else if (udpIn[0] == '{') { //JSON API
  //     DeserializationError error = deserializeJson(doc, udpIn);
  //     JsonObject root = doc.as<JsonObject>();
  //     if (!error && !root.isNull()) deserializeState(root);
  //   }
  //   releaseJSONBufferLock();
  // }
}


void setRealtimePixel(uint16_t i, byte r, byte g, byte b, byte w)
{
  uint16_t pix = i + pCONT_lAni->arlsOffset;
  if (pix < pCONT_lAni->getLengthTotal()) {
    if (!pCONT_lAni->arlsDisableGammaCorrection && pCONT_lAni->gammaCorrectCol) {
      // r = pCONT_lAni->gamma8(r);
      // g = pCONT_lAni->gamma8(g);
      // b = pCONT_lAni->gamma8(b);
      // w = pCONT_lAni->gamma8(w);
    }
    if (pCONT_lAni->useMainSegmentOnly) {
      mAnimatorLight::Segment_New &seg = pCONT_lAni->getMainSegment();
      if (pix<seg.length()) seg.setPixelColor(pix, r, g, b, w);
    } else {
      pCONT_lAni->setPixelColor(pix, r, g, b, w);
    }
  }
}

/*********************************************************************************************\
   Refresh aging for remote units, drop if too old...
\*********************************************************************************************/
void refreshNodeList()
{
  // for (NodesMap::iterator it = Nodes.begin(); it != Nodes.end();) {
  //   bool mustRemove = true;

  //   if (it->second.ip[0] != 0) {
  //     if (it->second.age < 10) {
  //       it->second.age++;
  //       mustRemove = false;
  //       ++it;
  //     }
  //   }

  //   if (mustRemove) {
  //     it = Nodes.erase(it);
  //   }
  // }
}

/*********************************************************************************************\
   Broadcast system info to other nodes. (to update node lists)
\*********************************************************************************************/
void sendSysInfoUDP()
{
  if (!pCONT_lAni->udp2Connected) return;

  IPAddress ip = Network.localIP();
  if (!ip || ip == IPAddress(255,255,255,255)) ip = IPAddress(4,3,2,1);

  // TODO: make a nice struct of it and clean up
  //  0: 1 byte 'binary token 255'
  //  1: 1 byte id '1'
  //  2: 4 byte ip
  //  6: 32 char name
  // 38: 1 byte node type id
  // 39: 1 byte node id
  // 40: 4 byte version ID
  // 44 bytes total

  // send my info to the world...
  uint8_t data[44] = {0};
  data[0] = 255;
  data[1] = 1;

  for (size_t x = 0; x < 4; x++) {
    data[x + 2] = ip[x];
  }
  memcpy((byte *)data + 6, pCONT_lAni->serverDescription, 32);
  #ifdef ESP8266
  data[38] = NODE_TYPE_ID_ESP8266;
  #elif defined(CONFIG_IDF_TARGET_ESP32C3)
  data[38] = NODE_TYPE_ID_ESP32C3;
  #elif defined(CONFIG_IDF_TARGET_ESP32S3)
  data[38] = NODE_TYPE_ID_ESP32S3;
  #elif defined(CONFIG_IDF_TARGET_ESP32S2)
  data[38] = NODE_TYPE_ID_ESP32S2;
  #elif defined(ARDUINO_ARCH_ESP32)
  data[38] = 32;//NODE_TYPE_ID_ESP32;
  #else
  data[38] = NODE_TYPE_ID_UNDEFINED;
  #endif
  data[39] = ip[3]; // unit ID == last IP number

  uint32_t build = PROJECT_VERSION;
  for (size_t i=0; i<sizeof(uint32_t); i++)
    data[40+i] = (build>>(8*i)) & 0xFF;

  IPAddress broadcastIP(255, 255, 255, 255);
  pCONT_lAni->notifier2Udp.beginPacket(broadcastIP, pCONT_lAni->udpPort2);
  pCONT_lAni->notifier2Udp.write(data, sizeof(data));
  pCONT_lAni->notifier2Udp.endPacket();
}


/*********************************************************************************************\
 * Art-Net, DDP, E131 output - work in progress
\*********************************************************************************************/

#define DDP_HEADER_LEN 10
#define DDP_SYNCPACKET_LEN 10

#define DDP_FLAGS1_VER 0xc0  // version mask
#define DDP_FLAGS1_VER1 0x40 // version=1
#define DDP_FLAGS1_PUSH 0x01
#define DDP_FLAGS1_QUERY 0x02
#define DDP_FLAGS1_REPLY 0x04
#define DDP_FLAGS1_STORAGE 0x08
#define DDP_FLAGS1_TIME 0x10

#define DDP_ID_DISPLAY 1
#define DDP_ID_CONFIG 250
#define DDP_ID_STATUS 251

// 1440 channels per packet
#define DDP_CHANNELS_PER_PACKET 1440 // 480 leds

//
// Send real time UDP updates to the specified client
//
// type   - protocol type (0=DDP, 1=E1.31, 2=ArtNet)
// client - the IP address to send to
// length - the number of pixels
// buffer - a buffer of at least length*4 bytes long
// isRGBW - true if the buffer contains 4 components per pixel

static       size_t sequenceNumber = 0; // this needs to be shared across all outputs
static const size_t ART_NET_HEADER_SIZE = 12;
static const byte   ART_NET_HEADER[] PROGMEM = {0x41,0x72,0x74,0x2d,0x4e,0x65,0x74,0x00,0x00,0x50,0x00,0x0e};

uint8_t realtimeBroadcast(uint8_t type, IPAddress client, uint16_t length, uint8_t *buffer, uint8_t bri, bool isRGBW)  {
  // if (!(apActive || interfacesInited) || !client[0] || !length) return 1;  // network not initialised or dummy/unset IP address  031522 ajn added check for ap

  WiFiUDP ddpUdp;

  switch (type) {
    case 0: // DDP
    {
      // // calculate the number of UDP packets we need to send
      // size_t channelCount = length * (isRGBW? 4:3); // 1 channel for every R,G,B value
      // size_t packetCount = ((channelCount-1) / DDP_CHANNELS_PER_PACKET) +1;

      // // there are 3 channels per RGB pixel
      // uint32_t channel = 0; // TODO: allow specifying the start channel
      // // the current position in the buffer
      // size_t bufferOffset = 0;

      // for (size_t currentPacket = 0; currentPacket < packetCount; currentPacket++) {
      //   if (sequenceNumber > 15) sequenceNumber = 0;

      //   if (!ddpUdp.beginPacket(client, DDP_DEFAULT_PORT)) {  // port defined in ESPAsyncE131.h
      //     DEBUG_PRINTLN(F("WiFiUDP.beginPacket returned an error"));
      //     return 1; // problem
      //   }

      //   // the amount of data is AFTER the header in the current packet
      //   size_t packetSize = DDP_CHANNELS_PER_PACKET;

      //   uint8_t flags = DDP_FLAGS1_VER1;
      //   if (currentPacket == (packetCount - 1U)) {
      //     // last packet, set the push flag
      //     // TODO: determine if we want to send an empty push packet to each destination after sending the pixel data
      //     flags = DDP_FLAGS1_VER1 | DDP_FLAGS1_PUSH;
      //     if (channelCount % DDP_CHANNELS_PER_PACKET) {
      //       packetSize = channelCount % DDP_CHANNELS_PER_PACKET;
      //     }
      //   }

      //   // write the header
      //   /*0*/ddpUdp.write(flags);
      //   /*1*/ddpUdp.write(sequenceNumber++ & 0x0F); // sequence may be unnecessary unless we are sending twice (as requested in Sync settings)
      //   /*2*/ddpUdp.write(isRGBW ?  DDP_TYPE_RGBW32 : DDP_TYPE_RGB24);
      //   /*3*/ddpUdp.write(DDP_ID_DISPLAY);
      //   // data offset in bytes, 32-bit number, MSB first
      //   /*4*/ddpUdp.write(0xFF & (channel >> 24));
      //   /*5*/ddpUdp.write(0xFF & (channel >> 16));
      //   /*6*/ddpUdp.write(0xFF & (channel >>  8));
      //   /*7*/ddpUdp.write(0xFF & (channel      ));
      //   // data length in bytes, 16-bit number, MSB first
      //   /*8*/ddpUdp.write(0xFF & (packetSize >> 8));
      //   /*9*/ddpUdp.write(0xFF & (packetSize     ));

      //   // write the colors, the write write(const uint8_t *buffer, size_t size)
      //   // function is just a loop internally too
      //   for (size_t i = 0; i < packetSize; i += (isRGBW?4:3)) {
      //     ddpUdp.write(scale8(buffer[bufferOffset++], bri)); // R
      //     ddpUdp.write(scale8(buffer[bufferOffset++], bri)); // G
      //     ddpUdp.write(scale8(buffer[bufferOffset++], bri)); // B
      //     if (isRGBW) ddpUdp.write(scale8(buffer[bufferOffset++], bri)); // W
      //   }

      //   if (!ddpUdp.endPacket()) {
      //     DEBUG_PRINTLN(F("WiFiUDP.endPacket returned an error"));
      //     return 1; // problem
      //   }

      //   channel += packetSize;
      // }
    } break;

    case 1: //E1.31
    {
    } break;

    case 2: //ArtNet
    {
      // // calculate the number of UDP packets we need to send
      // const size_t channelCount = length * (isRGBW?4:3); // 1 channel for every R,G,B,(W?) value
      // const size_t ARTNET_CHANNELS_PER_PACKET = isRGBW?512:510; // 512/4=128 RGBW LEDs, 510/3=170 RGB LEDs
      // const size_t packetCount = ((channelCount-1)/ARTNET_CHANNELS_PER_PACKET)+1;

      // uint32_t channel = 0; 
      // size_t bufferOffset = 0;

      // sequenceNumber++;

      // for (size_t currentPacket = 0; currentPacket < packetCount; currentPacket++) {

      //   if (sequenceNumber > 255) sequenceNumber = 0;

      //   if (!ddpUdp.beginPacket(client, ARTNET_DEFAULT_PORT)) {
      //     DEBUG_PRINTLN(F("Art-Net WiFiUDP.beginPacket returned an error"));
      //     return 1; // borked
      //   }

      //   size_t packetSize = ARTNET_CHANNELS_PER_PACKET;

      //   if (currentPacket == (packetCount - 1U)) {
      //     // last packet
      //     if (channelCount % ARTNET_CHANNELS_PER_PACKET) {
      //       packetSize = channelCount % ARTNET_CHANNELS_PER_PACKET;
      //     }
      //   }

      //   byte header_buffer[ART_NET_HEADER_SIZE];
      //   memcpy_P(header_buffer, ART_NET_HEADER, ART_NET_HEADER_SIZE);
      //   ddpUdp.write(header_buffer, ART_NET_HEADER_SIZE); // This doesn't change. Hard coded ID, OpCode, and protocol version.
      //   ddpUdp.write(sequenceNumber & 0xFF); // sequence number. 1..255
      //   ddpUdp.write(0x00); // physical - more an FYI, not really used for anything. 0..3
      //   ddpUdp.write((currentPacket) & 0xFF); // Universe LSB. 1 full packet == 1 full universe, so just use current packet number.
      //   ddpUdp.write(0x00); // Universe MSB, unused.
      //   ddpUdp.write(0xFF & (packetSize >> 8)); // 16-bit length of channel data, MSB
      //   ddpUdp.write(0xFF & (packetSize     )); // 16-bit length of channel data, LSB

      //   for (size_t i = 0; i < packetSize; i += (isRGBW?4:3)) {
      //     ddpUdp.write(scale8(buffer[bufferOffset++], bri)); // R
      //     ddpUdp.write(scale8(buffer[bufferOffset++], bri)); // G
      //     ddpUdp.write(scale8(buffer[bufferOffset++], bri)); // B
      //     if (isRGBW) ddpUdp.write(scale8(buffer[bufferOffset++], bri)); // W
      //   }

      //   if (!ddpUdp.endPacket()) {
      //     DEBUG_PRINTLN(F("Art-Net WiFiUDP.endPacket returned an error"));
      //     return 1; // borked
      //   }
      //   channel += packetSize;
      // }
    } break;
  }
  return 0;
}

#endif // ENABLE_WEBSERVER_LIGHTING_WEBUI



#endif //USE_MODULE_LIGHTS_ANIMATOR
