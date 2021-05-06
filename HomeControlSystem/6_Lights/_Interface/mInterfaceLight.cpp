#include "mInterfaceLight.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE // interface is the gateway


const char* mInterfaceLight::PM_MODULE_LIGHTS_INTERFACE_CTR = D_MODULE_LIGHTS_INTERFACE_CTR;
const char* mInterfaceLight::PM_MODULE_LIGHTS_INTERFACE_FRIENDLY_CTR = D_MODULE_LIGHTS_INTERFACE_FRIENDLY_CTR;


void mInterfaceLight::Init_NeoPixelAnimator(uint16_t size, uint8_t timebase){

  animator_controller = new NeoPixelAnimator(size, timebase); // NeoPixel animation management object
  
}  


void mInterfaceLight::Settings_Default(){

  // #ifdef ENABLE_LOG_LEVEL_INFO
  // AddLog(LOG_LEVEL_TEST,PSTR("mInterfaceLight::Settings_Default"));
  // #endif

  // #ifdef USE_LIGHTING_TEMPLATE
  //   settings.flags.TemplateProvidedInProgMem = true;
  // #endif
  
  // if(settings.flags.TemplateProvidedInProgMem){// use template provided in progmem || override template
  
  //   Template_Load();

  // }else{
    
  //   #ifdef DEFAULT_LIGHTING_ANIMATION_MODE_ID
  //     animation.mode_id = DEFAULT_LIGHTING_ANIMATION_MODE_ID<ANIMATION_MODE_LENGTH_ID?
  //                           DEFAULT_LIGHTING_ANIMATION_MODE_ID:ANIMATION_MODE_EFFECTS_ID;
  //   #else
  //     animation.mode_id = ANIMATION_MODE_EFFECTS_ID;
  //   #endif

  // }

}



RgbcctColor mInterfaceLight::GetActiveFirstColourFromCurrentPalette(){

  // Update pointer of struct
  mPaletteI->SetPaletteListPtrFromID(animation.palette.id);
  
  uint8_t pixels_in_map = mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr);
          
  #ifdef ENABLE_LOG_LEVEL_DEBUG
  AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_LIGHT "pixels_in_map %d"),pixels_in_map);
  #endif

  uint16_t desired_pixel = 0; // always first
  int16_t start_pixel_position = -1;
  RgbcctColor colour = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr, desired_pixel, &start_pixel_position);

  return colour;

}




uint8_t mInterfaceLight::ConstructJSON_Scene(uint8_t json_method){

  // Active rgbcct palette used as scene

  char buffer[30];
  
  JsonBuilderI->Start();  

  // Got to ConstructJson_Scene out, or rename all the parameters as something else, or rgbcctactivepalette, or show them all? though that would need to run through, can only show
  // active_id, plus the values below
  // #ifndef ENABLE_DEVFEATURE_PHASING_SCENE_OUT
  //   JsonBuilderI->Add_P(PM_JSON_SCENE_NAME, GetSceneName(buffer, sizeof(buffer)));  
  //   #endif //  ENABLE_DEVFEATURE_PHASING_SCENE_OUT
  
    JsonBuilderI->Add_P(PM_JSON_HUE, rgbcct_controller.getHue360());
    JsonBuilderI->Add_P(PM_JSON_SAT, rgbcct_controller.getSat255());
    JsonBuilderI->Add_P(PM_JSON_BRIGHTNESS_RGB, rgbcct_controller.getBrightnessRGB255());
    JsonBuilderI->Add_P(PM_JSON_TIME, (uint16_t)round(animation.transition.time_ms/1000));
    JsonBuilderI->Add_P(PM_JSON_TIME_MS, animation.transition.time_ms);
  return JsonBuilderI->End();

}


bool mInterfaceLight::Pre_Init(void)
{
  pCONT_set->Settings.light_settings.type = LT_BASIC;                    // Use basic PWM control if SetOption15 = 0

  if (pCONT_set->Settings.flag_system.pwm_control) {          // SetOption15 - Switch between commands PWM or COLOR/DIMMER/CT/CHANNEL
    for (uint32_t i = 0; i < MAX_PWMS; i++) {
      if (pCONT_pins->PinUsed(GPIO_PWM1_ID, i)) { 
        pCONT_set->Settings.light_settings.type++; 
      #ifdef ENABLE_LOG_LEVEL_COMMANDS
        AddLog(LOG_LEVEL_DEBUG,PSTR("Pre_Init PWM%d"),i);    
      #endif // ENABLE_LOG_LEVEL_COMMANDS    
      }  // Use Dimmer/Color control for all PWM as SetOption15 = 1
    }
  }

  
  mInterfaceLight::HARDWARE_ELEMENT_COLOUR_ORDER* order = &pCONT_iLight->hardware_element_colour_order[0];
  order->red = 0;
  order->green = 1;
  order->blue = 2;
  order->white_cold = 3;
  order->white_warm = 4;


  //temp fix
  if (pCONT_pins->PinUsed(GPIO_RGB_DATA_ID)) { 
    // AddLog(LOG_LEVEL_TEST,PSTR("pCONT_set->Settings.light_settings.type=%d"),pCONT_set->Settings.light_settings.type);
    pCONT_set->Settings.light_settings.type = LT_ADDRESSABLE; 
  }

  if (pCONT_set->Settings.light_settings.type > LT_BASIC) {
    pCONT_set->devices_present++;
  }

  // // post-process for lights
  // if (Settings.flag3.pwm_multi_channels) {  // SetOption68 - Enable multi-channels PWM instead of Color PWM
  //   uint32_t pwm_channels = (Settings.light_settings.type & 7) > LST_MAX ? LST_MAX : (Settings.light_settings.type & 7);
  //   if (0 == pwm_channels) { pwm_channels = 1; }
  //   devices_present += pwm_channels - 1;    // add the pwm channels controls at the end
  // } else if ((Settings.param[P_RGB_REMAP] & 128) && (LST_RGBW <= (Settings.light_settings.type & 7))) {
  //   // if RGBW or RGBCW, and SetOption37 >= 128, we manage RGB and W separately, hence adding a device
  //   devices_present++;
  // }

  return (pCONT_set->Settings.light_settings.type > LT_BASIC);
}


void mInterfaceLight::Template_Load(){

  #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
  AddLog(LOG_LEVEL_DEBUG, PSTR("mInterfaceLight::Template_Load()"));
  #endif

  #ifdef USE_LIGHTING_TEMPLATE
  // load from progmem into local
  D_DATA_BUFFER_CLEAR();
  memcpy_P(data_buffer.payload.ctr,LIGHTING_TEMPLATE,sizeof(LIGHTING_TEMPLATE));
  data_buffer.payload.len = strlen(data_buffer.payload.ctr);

  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_DEBUG, PSTR("LIGHTING_TEMPLATE Load"));// " READ = \"%s\""), data_buffer.payload.ctr);
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("LIGHTING_TEMPLATE" " READ = \"%s\""), data_buffer.payload.ctr);
  #endif // ENABLE_LOG_LEVEL_COMMANDS

  pCONT->Tasker_Interface(FUNC_JSON_COMMAND_ID);
  #endif // USE_LIGHTING_TEMPLATE

}


void mInterfaceLight::Init(void) //LightInit(void)
{

  device = pCONT_set->devices_present;
  
  //subtype = (pCONT_set->Settings.light_settings.type & 7) > LST_MAX ? LST_MAX : (pCONT_set->Settings.light_settings.type & 7); // Always 0 - LST_MAX (5)
  subtype = LST_RGBCW;
  
  pwm_multi_channels = 0;//pCONT_set->Settings.flag3.pwm_multi_channels;  // SetOption68 - Enable multi-channels PWM instead of Color PWM

  pCONT_set->Settings.pwm_range = PWM_RANGE; //tmp

  pCONT_set->Settings.light_settings.light_fade = 1;
  pCONT_set->Settings.light_settings.light_speed = 5*2;
  pCONT_set->power = 1;

  auto_off_settings.time_decounter_secs = 0;

  mPaletteI->init_PresetColourPalettes();
  init_Animations();

  
  char buffer[30];
  // Default user names for palette
  for (int ii=mPaletteI->PALETTELIST_VARIABLE_HSBID_01_ID;ii<(mPaletteI->PALETTELIST_VARIABLE_HSBID_LENGTH_ID);ii++){ 
    sprintf(buffer, "User Test %02d", ii);
    DLI->AddDeviceName(buffer,EM_MODULE_LIGHTS_INTERFACE_ID,ii);
  }
  for (int ii=mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_01_ID;ii<(mPaletteI->PALETTELIST_VARIABLE_RGBCCT_LENGTH_ID);ii++){ 
    sprintf(buffer, "Solid Rgbcct %02d", ii);
    DLI->AddDeviceName(buffer,EM_MODULE_LIGHTS_INTERFACE_ID,ii);
  }
  for (int ii=mPaletteI->PALETTELIST_VARIABLE_GENERIC_01_ID;ii<(mPaletteI->PALETTELIST_VARIABLE_GENERIC_LENGTH_ID);ii++){ 
    sprintf(buffer, "Encoded %02d", ii);
    DLI->AddDeviceName(buffer,EM_MODULE_LIGHTS_INTERFACE_ID,ii);
  }

  #ifdef USE_MODULE_LIGHTS_PWM
    settings.flags.enable_cct_channel_sliders = true;
  #else
    settings.flags.enable_cct_channel_sliders = false;
  #endif // USE_MODULE_LIGHTS_PWM

  // if (LST_RGBW <= subtype) {
  //   // only change if RGBW or RGBCW
  //   // do not allow independant RGB and WC colors
  //   bool ct_rgb_linked = 0;// !(pCONT_set->Settings.param[P_RGB_REMAP] & 128);
  //   CommandSet_RGBCT_Linked(ct_rgb_linked);
  // }

  // DEBUG_LINE_HERE;




  /***
   * Configure RgbcctController Instance
   * */
  CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_01_ID);
  rgbcct_controller.setSubType(subtype);
  rgbcct_controller.setApplyBrightnessToOutput(false);
  if(pCONT_set->Settings.light_settings.type == LT_ADDRESSABLE){ //RGB only
    rgbcct_controller.setColorMode(LCM_RGB);
  }else{
    rgbcct_controller.setColorMode(LCM_BOTH);
  }
  rgbcct_controller.Sync();    // calculate the initial values (#8058)
  // RGB parts
  rgbcct_controller.setRGB(1,2,3);
  // CCT parts
  rgbcct_controller.setRGBCCTLinked(false);
  rgbcct_controller.setCCT(153);
  



















  // if ((LST_SINGLE <= subtype) && pwm_multi_channels) {
  //   // we treat each PWM channel as an independant one, hence we switch to
  //   // setPWMMultiChannel(true);
  //   rgbcct_controller.setPWMMultiChannel(true);
  //   device = pCONT_set->devices_present - subtype + 1; // adjust if we also have relays
  // } else if (!rgbcct_controller.getRGBCCTLinked()) {
  //   // if RGBW or RGBCW, and SetOption37 >= 128, we manage RGB and W separately
  //   device--;   // we take the last two devices as lights
  // }

  // DEBUG_LINE_HERE;
  //LightCalcPWMRange();
  // #ifdef DEBUG_LIGHT
  //   AddLog(LOG_LEVEL_TEST, "LightInit pwm_multi_channels=%d subtype=%d device=%d devices_present=%d",
  //     pwm_multi_channels, subtype, device, pCONT_set->devices_present);
  // #endif


  // DEBUG_LINE_HERE;
  // Serial.println("UpdateFinalColourComponents DONE"); Serial.flush();

  // if (LST_SINGLE == subtype) {
  //   //pCONT_set->Settings.light_settings.light_color[0] = 255;      // One channel only supports Dimmer but needs max color
  // }
  // if (Settings.light_settings.type < LT_PWM6) {           // PWM
  //   for (uint32_t i = 0; i < Settings.light_settings.type; i++) {
  //     Settings.pwm_value[i] = 0;        // Disable direct PWM control
  //     if (PinUsed(GPIO_PWM1, i)) {
  //       pinMode(Pin(GPIO_PWM1, i), OUTPUT);
  //     }
  //   }
  // }

  // // Set light mode
  


  // uint32_t max_scheme = max_scheme;
  // if (subtype < LST_RGB) {
  //   max_scheme = LS_POWER;
  // }
  // if ((LS_WAKEUP == pCONT_set->Settings.light_settings.light_scheme) || (pCONT_set->Settings.light_settings.light_scheme > max_scheme)) {
  //   pCONT_set->Settings.light_settings.light_scheme = LS_POWER;
  // }
  // pCONT_set->power = 1;
  
  
  // if (pCONT_set->Settings.flag4.fade_at_startup) {
  //   fade_initialized = true;      // consider fade intialized starting from black
  // }

  // pCONT_set->Settings.light_settings.type = LT_BASIC;                    // Use basic PWM control if SetOption15 = 0

  // if (pCONT_set->Settings.flag.pwm_control) {          // SetOption15 - Switch between commands PWM or COLOR/DIMMER/CT/CHANNEL
  //   for (uint32_t i = 0; i < MAX_PWMS; i++) {
  //     if (PinUsed(pCONT_set->GPIO_PWM1, i)) { pCONT_set->Settings.light_settings.type++; }  // Use Dimmer/Color control for all PWM as SetOption15 = 1
  //   }
  // }

//   // post-process for lights
//   if (Settings.flag3.pwm_multi_channels) {  // SetOption68 - Enable multi-channels PWM instead of Color PWM
//     uint32_t pwm_channels = (Settings.light_settings.type & 7) > LST_MAX ? LST_MAX : (Settings.light_settings.type & 7);
//     if (0 == pwm_channels) { pwm_channels = 1; }
//     devices_present += pwm_channels - 1;    // add the pwm channels controls at the end
//   } else if ((Settings.param[P_RGB_REMAP] & 128) && (LST_RGBW <= (Settings.light_settings.type & 7))) {
//     // if RGBW or RGBCW, and SetOption37 >= 128, we manage RGB and W separately, hence adding a device
//     devices_present++;
//   }

//   return (Settings.light_settings.type > LT_BASIC);
// }

// changeBri(0); //default off

  //create copy for pCONT_iLight->animation stored
  memcpy(&animation_stored,&animation,sizeof(animation_stored));// RESTORE copy of state

} //light_init



void mInterfaceLight::init_Animations(){

// add these into settings as defaults as these currently overwrite templates

  // // Set default values (ifdef below sets specific)
  // #ifdef USE_MODULE_LIGHTS_ANIMATOR
  // animation.transition.order_id = TRANSITION_ORDER_INORDER_ID;
  // #endif // USE_MODULE_LIGHTS_ANIMATOR
  // // animation.palette.id = PALETTELIST_VARIABLE_USER_01_ID;
  // animation.mode_id = ANIMATION_MODE_EFFECTS_ID;
  // // flashersettings.function = EFFECTS_FUNCTION_SLOW_GLOW_ID;
  // #ifdef USE_MODULE_LIGHTS_ANIMATOR
  // animation.transition.method_id = TRANSITION_METHOD_BLEND_ID;
  // #endif // USE_MODULE_LIGHTS_ANIMATOR
  // animation.flags.fForceUpdate = true;
  // animation.transition.time_ms.val = 1000;
  // animation.transition.rate_ms = 20*1000;
  animation.flags.fEnable_Animation = true;
  animation.flags.NewAnimationRequiringCompleteRefresh = true;

}




// Colours etc will be made or called using a func_update_light_Values
// Another func will push those values to the output, which each hardware class will handle

// // Used for timed on or off events
int8_t mInterfaceLight::Tasker(uint8_t function, JsonParserObject obj){
  
  int8_t function_result = 0;

  // As interface module, the parsing of module_init takes precedence over the Settings.light_settings.type
  switch(function){
    case FUNC_TEMPLATE_MODULE_LOAD:
    case FUNC_TEMPLATE_DEVICE_LOAD:
      Template_Load();
    break;
    case FUNC_POINTER_INIT:

      mPaletteI->init_PresetColourPalettes();
      rgbcct_controller.setRgbcctColourOutputAddress(mPaletteI->palettelist.rgbcct_users[0].colour_map_id);
      // active_scene_palette_id = PALETTELIST_VARIABLE_RGBCCT_COLOUR_01_ID;
      // active_rgbcct_colour_p = reinterpret_cast<RgbcctColor*>(&pCONT_set->Settings.animation_settings.palette_rgbcct_user_colour_map_ids[0]); // use first for now
      CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_01_ID);

    break;
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
    case FUNC_INIT:
      Init();
    break;
    case FUNC_BOOT_MESSAGE:
      BootMessage();
    break;
  }

  // if(settings.flags. == nullptr){ return 0; }

  switch(function){
    /************
     * SETTINGS SECTION * 
    *******************/
    case FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE: 
      // Settings_Load();
    break;
    case FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE: 
      //Settings_Save();
    break;
    case FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES:
      Settings_Default();
      // Module_Init(); // User defined values in progmem, later to be parsed via webcommand
    break;
    case FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT:
      Settings_Default();
      //pCONT_set->SettingsSave(2);
    break;
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP:
      EveryLoop();
    break;
    case FUNC_EVERY_SECOND:{
      //Temp fix until proper monitoring of on/off states
      #ifdef USE_MODULE_LIGHTS_ANIMATOR
      // replace with GetLightPower() so its updated internally
      light_power_state = rgbcct_controller.getBrightness255()?1:0;
      //AddLog(LOG_LEVEL_TEST, PSTR("light_power_state=%d"),light_power_state);
      #endif // USE_MODULE_LIGHTS_ANIMATOR

      EverySecond_AutoOff();
    
    }break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    // case FUNC_SET_POWER:
    //   LightSetPower();
    // break;
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
  } // end switch
  
  /************
   * WEBPAGE SECTION * 
  *******************/
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  return Tasker_Web(function);
  #endif // USE_MODULE_NETWORK_WEBSERVER

} // END function


/**
 * This function, is the entry point for the output that is hardware dependent 
 * colour_hardware will already have colour_order mapped
 * */
void mInterfaceLight::SetPixelColourHardwareInterface(RgbcctColor colour_hardware, uint16_t index, bool flag_replicate_for_total_pixel_length){

  switch(pCONT_set->Settings.light_settings.type){
    case LT_ADDRESSABLE:
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      pCONT_ladd->SetPixelColorHardware(index,colour_hardware, flag_replicate_for_total_pixel_length);
      #endif // USE_MODULE_LIGHTS_ADDRESSABLE
    break;
    case LT_PWM1:
    case LT_PWM2:
    case LT_PWM3:
    case LT_PWM4:
    case LT_PWM5:
      #ifdef USE_MODULE_LIGHTS_PWM
      pCONT_lPWM->SetPixelColorHardware(index,colour_hardware);
      #endif // USE_MODULE_LIGHTS_PWM
    break;
  }
  
}
RgbcctColor mInterfaceLight::GetPixelColourHardwareInterface(uint16_t index){

  switch(pCONT_set->Settings.light_settings.type){
    case LT_ADDRESSABLE:  
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      return pCONT_ladd->GetPixelColorHardware(index);
      #endif // USE_MODULE_LIGHTS_ADDRESSABLE
    break;
    case LT_PWM1:
    case LT_PWM2:
    case LT_PWM3:
    case LT_PWM4:
    case LT_PWM5:
      #ifdef USE_MODULE_LIGHTS_PWM
      pCONT_lPWM->GetPixelColorHardware(index);
      #endif // USE_MODULE_LIGHTS_PWM
    break;
  }

}

void mInterfaceLight::ShowInterface(){

  switch(pCONT_set->Settings.light_settings.type){
    case LT_ADDRESSABLE:  
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      return pCONT_ladd->ShowHardware();
      #endif // USE_MODULE_LIGHTS_ADDRESSABLE
    break;
    case LT_PWM1:
    case LT_PWM2:
    case LT_PWM3:
    case LT_PWM4:
    case LT_PWM5:
      #ifdef USE_MODULE_LIGHTS_PWM
      pCONT_lPWM->ShowHardware();
      #endif // USE_MODULE_LIGHTS_PWM
    break;
  }

} 



void mInterfaceLight::EveryLoop(){
      
  // AddLog(LOG_LEVEL_TEST, PSTR("Invalid Light LT_ADDRESSABLE HERE %d"),pCONT_set->Settings.light_settings.type);
  // Single colour methods
  if((pCONT_set->Settings.light_settings.type < LT_LIGHT_INTERFACE_END)||
     (pCONT_set->Settings.light_settings.type == LT_ADDRESSABLE)){
     
    switch(animation.mode_id){
      #ifdef ENABLE_PIXEL_FUNCTION_EFFECTS
      case ANIMATION_MODE_EFFECTS_ID:
        //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_LIGHT "ANIMATION_MODE_EFFECTS_ID"));
        pCONT_lAni->SubTask_Flasher_Main();
        // light_power_state = true;
      break;
      #endif
      case ANIMATION_MODE_NONE_ID: default: break; // resting position
    }
  
  }
  
  if(pCONT_set->Settings.light_settings.type == LT_ADDRESSABLE){ 
    
    // AddLog(LOG_LEVEL_DEBUG, PSTR("Invalid Light LT_ADDRESSABLE %d"),animation.mode_id);
    #ifdef USE_MODULE_LIGHTS_ANIMATOR
    switch(animation.mode_id){
      #ifdef ENABLE_PIXEL_FUNCTION_EFFECTS
      case ANIMATION_MODE_EFFECTS_ID:
        // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_LIGHT "ANIMATION_MODE_8EFFECTS_ID"));
        pCONT_lAni->SubTask_Flasher_Main();
        // light_power_state = true;
      break;
      #endif
      #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
      case ANIMATION_MODE_NOTIFICATIONS_ID:
        // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_LIGHT "ANIMATION_MODE_NOTIFICATIONS_IDu"));
        pCONT_lAni->SubTask_Notifications();
      break;
      #endif
      #ifdef ENABLE_PIXEL_FUNCTION_AMBILIGHT // Basic colours around boxed objects
      case ANIMATION_MODE_AMBILIGHT_ID:
        // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_LIGHT "ANIMATION_MODE_AMBILIGHT_ID"));
        pCONT_ladd->SubTask_Ambilight_Main();
        // light_power_state = true;
      break;
      #endif
      #ifdef ENABLE_PIXEL_FUNCTION_ADALIGHT // serial, wifi udp connection
      case ANIMATION_MODE_EFFECTS_ID:
        // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_LIGHT "ANIMATION_MODE_EFFECTS_ID"));
        pCONT_ladd->SubTask_Flasher_Main();
        // light_power_state = true;
      break;
      #endif
      #ifdef ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL // serial, wifi udp connection
      case ANIMATION_MODE_MANUAL_SETPIXEL_ID:
        // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_LIGHT "ANIMATION_MODE_EFFECTS_ID"));
        pCONT_ladd->SubTask_Manual_SetPixel();
        // light_power_state = true;
      break;
      #endif
      case ANIMATION_MODE_NONE_ID: default: break; // resting position
    }
    #endif
      
  }
  else
  {
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("Invalid Light Type"));
  }

} // END everyloop




void mInterfaceLight::EverySecond_AutoOff(){

  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_LIGHT "scene.auto_off_settings.tSaved [%d]"),animation.auto_off_settings.time_decounter_secs);
  if(auto_off_settings.time_decounter_secs==1){ //if =1 then turn off and clear to 0
    // animation.name_id = MODE_SINGLECOLOUR_FADE_OFF_ID;
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT "animation.auto_off_settings.time_decounter_secs==1 and disable"));
    #endif       
    CommandSet_LightPowerState(LIGHT_POWER_STATE_OFF_ID);
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

}// END EverySecond_AutoOff


void mInterfaceLight::ApplyGlobalBrightnesstoColour(RgbcctColor* colour){

  colour->R  = mapvalue(colour->R,  0, 255, 0, rgbcct_controller.getBrightnessRGB255());
  colour->G  = mapvalue(colour->G,  0, 255, 0, rgbcct_controller.getBrightnessRGB255());
  colour->B  = mapvalue(colour->B,  0, 255, 0, rgbcct_controller.getBrightnessRGB255());
  colour->WW = mapvalue(colour->WW, 0, 255, 0, rgbcct_controller.getBrightnessCCT255());
  colour->WC = mapvalue(colour->WC, 0, 255, 0, rgbcct_controller.getBrightnessCCT255());

}

float mInterfaceLight::HueN2F(uint16_t hue){
  return hue/360.0f;
}
float mInterfaceLight::SatN2F(uint8_t sat){
  return sat/100.0f;
}
float mInterfaceLight::BrtN2F(uint8_t brt){
  return brt/100.0f;
}

uint16_t mInterfaceLight::HueF2N(float hue){
  return round(hue*360.0f);
}
uint8_t mInterfaceLight::SatF2N(float sat){
  return round(sat*100.0f);
}
uint8_t mInterfaceLight::BrtF2N(float brt){
  return round(brt*100.0f);
}


// Generate random colour between two hsb colours
// Needs to be totally random or hue random only, same brightness
RgbcctColor mInterfaceLight::GetRandomColour(RgbcctColor colour1, RgbcctColor colour2){
  // int random_hue = random(HueF2N(colour1.H),HueF2N(colour2.H));
  // int random_sat = random(SatF2N(colour1.S),SatF2N(colour2.S));
  // int random_brt = random(BrtF2N(colour1.B),BrtF2N(colour2.B));
  return RgbcctColor(0);//HueN2F(random_hue),SatN2F(random_sat),BrtN2F(random_brt));
}



uint8_t mInterfaceLight::ConstructJSON_Settings(uint8_t json_method){

  char buffer[30];
  
  JsonBuilderI->Start();

  JsonBuilderI->Add_P(PM_JSON_TYPE, pCONT_set->Settings.light_settings.type);
  
  JsonBuilderI->Add_P(PM_JSON_HUE, rgbcct_controller.getHue360());
  JsonBuilderI->Add_P(PM_JSON_SAT, rgbcct_controller.getSat255());

  JsonBuilderI->Add_P(PM_JSON_BRIGHTNESS_RGB, rgbcct_controller.getBrightnessRGB255());

  
  // JBI->Array_AddArray(PM_JSON_RGB_COLOUR_ORDER, hardware_element_colour_order);
  // JBI->Array_AddArray(PM_JSON_RGB_COLOUR_ORDER, hardware_element_c12olour_order);


  JsonBuilderI->Add_P(PM_JSON_BRIGHTNESS_CCT, rgbcct_controller.getBrightnessCCT255());


  // JsonBuilderI->Add_P(PM_JSON_PIXELS_UPDATE_PERCENTAGE, animation.transition.pixels_to_update_as_percentage);
  #ifdef USE_MODULE_LIGHTS_ANIMATOR
  JsonBuilderI->Add_P(PM_JSON_PIXELS_UPDATE_NUMBER, animation.transition.pixels_to_update_as_number);
  
  // GetPixelsToUpdateAsNumberFromPercentage(animation.transition.pixels_to_update_as_percentage));
  #endif // USE_MODULE_LIGHTS_ANIMATOR

  return JsonBuilderI->End();

}

uint8_t mInterfaceLight::ConstructJSON_Debug(uint8_t json_method){

  char buffer[30];
  
  JsonBuilderI->Start();

  JsonBuilderI->Level_Start("RgbcctController");
  
    JsonBuilderI->Level_Start("raw");
      JsonBuilderI->Add("R", rgbcct_controller.R); 
      JsonBuilderI->Add("G", rgbcct_controller.G); 
      JsonBuilderI->Add("B", rgbcct_controller.B); 
      JsonBuilderI->Add("WW", rgbcct_controller.WW); 
      JsonBuilderI->Add("WC", rgbcct_controller.WC); 
    JsonBuilderI->Level_End();
    JsonBuilderI->Level_Start("type");
      JsonBuilderI->Add("R", hardware_element_colour_order[0].r); 
      JsonBuilderI->Add("G", hardware_element_colour_order[0].g); 
      JsonBuilderI->Add("B", hardware_element_colour_order[0].b); 
      JsonBuilderI->Add("WW", hardware_element_colour_order[0].w); 
      JsonBuilderI->Add("WC", hardware_element_colour_order[0].c); 
    JsonBuilderI->Level_End();

    JsonBuilderI->Add("mPaletteI->active_scene_palette_id",mPaletteI->active_scene_palette_id);

  JsonBuilderI->Level_End();

  // JsonBuilderI->Level_Start("singlecolour");
  //   JsonBuilderI->Add_P(PM_R", mode_singlecolour.colour.R);
  //   JsonBuilderI->Add_P(PM_G", mode_singlecolour.colour.G);
  //   JsonBuilderI->Add_P(PM_B", mode_singlecolour.colour.B);
  //   JsonBuilderI->Add_P(PM_WW", mode_singlecolour.colour.WW);
  //   JsonBuilderI->Add_P(PM_WC", mode_singlecolour.colour.WC);
  //   JsonBuilderI->Add_P(PM_WC", mode_singlecolour.colour.WC);
  // JsonBuilderI->Level_End();
  // JsonBuilderI->Level_Start("active_rgbcct_colour_p");
  //   JsonBuilderI->Add_P(PM_R", active_rgbcct_colour_p->R);
  //   JsonBuilderI->Add_P(PM_G", active_rgbcct_colour_p->G);
  //   JsonBuilderI->Add_P(PM_B", active_rgbcct_colour_p->B);
  //   JsonBuilderI->Add_P(PM_WW", active_rgbcct_colour_p->WW);
  //   JsonBuilderI->Add_P(PM_WC", active_rgbcct_colour_p->WC);
  //   JsonBuilderI->Add_P(PM_WC", active_rgbcct_colour_p->WC);
  // JsonBuilderI->Level_End();

  // JsonBuilderI->Array_AddArray("singlecolour", current_color, 5);   

  // JsonBuilderI->Level_Start("fade");
  //   JsonBuilderI->Add("running", fade.running);
  // JsonBuilderI->Level_End();

  return JsonBuilderI->End();

}






////////////////////// START OF MQTT /////////////////////////

void mInterfaceLight::MQTTHandler_Init(){

  struct handler<mInterfaceLight>* mqtthandler_ptr;
  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.configperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mInterfaceLight::ConstructJSON_Settings;

  
  mqtthandler_ptr = &mqtthandler_scene_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SCENE_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mInterfaceLight::ConstructJSON_Scene;

  mqtthandler_ptr = &mqtthandler_debug_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mInterfaceLight::ConstructJSON_Debug;

} //end "MQTTHandler_Init"


void mInterfaceLight::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  // mqtthandler_animation_teleperiod.flags.SendNow = true;
  // mqtthandler_ambilight_teleperiod.flags.SendNow = true;
  mqtthandler_scene_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mInterfaceLight::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  // // mqtthandler_animation_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  // // mqtthandler_ambilight_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_scene_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  
} //end "MQTTHandler_Set_TelePeriod"


void mInterfaceLight::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, EM_MODULE_LIGHTS_INTERFACE_ID,
    mqtthandler_list_ptr, mqtthandler_list_ids, sizeof(mqtthandler_list_ids)/sizeof(mqtthandler_list_ids[0]), mqtt_handler_id
  );

}



void mInterfaceLight::BootMessage(){
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_INFO, PSTR("BOOT: " "LightCount=%d"), settings.light_size_count);
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}


uint32_t mInterfaceLight::WebColorFromColourMap(uint8_t i)
{
  RgbColor rgb = RgbColor(mPaletteI->GetHsbColour(i));
  uint32_t tcolor = (rgb.R << 16) | (rgb.G << 8) | rgb.B;
  return tcolor;
}
uint32_t mInterfaceLight::WebColorFromColourType(RgbColor rgb)
{
  uint32_t tcolor = (rgb.R << 16) | (rgb.G << 8) | rgb.B;
  return tcolor;
}

RgbcctColor mInterfaceLight::Color32bit2RgbColour(uint32_t colour32bit){
  RgbcctColor rgb;
  // #if RgbTypeColor == RgbwColor
    rgb.R = colour32bit >> 24; //RGB
    rgb.G = colour32bit >> 16; //RGB
    rgb.B = colour32bit >> 8 & 0xFF; //RGB
  //   rgb.W = colour32bit & 0xFF; //RGB
  // #else
  //   rgb.R = colour32bit >> 16; //RGB
  //   rgb.G = colour32bit >> 8 & 0xFF; //RGB
  //   rgb.B = colour32bit & 0xFF; //RGB
  // #endif
  return rgb;
}

uint32_t mInterfaceLight::RgbColorto32bit(RgbColor rgb){
  return (rgb.R << 16) | (rgb.G << 8) | rgb.B;
}

/********************************************************************************************************************
*******************************************************************************************************************
*******************************************************************************************************************
************* Generate colour values that are maps ******************************************************************************************************
*******************************************************************************************************************
*******************************************************************************************************************
*******************************************************************************************************************
*******************************************************************************************************************/

RgbColor mInterfaceLight::GetColourValueUsingMaps(float value, 
                                            uint8_t map_style_id,
                                            float value_min, float value_max, //not need for some mappings
                                            bool map_is_palette_id
                                          ){

  // map_style_id can use some internal ones here, or else scale and get from palettes

  // Heating rainbow with brighter red end
  uint16_t hue = 0;
  uint8_t  sat = 0;
  uint8_t  brt = 0;

  // if(map is water temperature in celcius ie have different range styles) then convert into rainbow gradient

    // Generate Hue and Brt values
    if(value<20){
      hue = 240;
      brt = 10;
    }else
    if((value>=20)&&(value<50)){
      hue = mSupport::mapfloat(value, 20,50, 180,0);
      brt = mSupport::mapfloat(value, 20,50, 10,100);
    }else
    if((value>=50)&&(value<60)){      
      hue = mSupport::mapfloat(value, 50,60, 359,345);
      brt = 100;
    }else
    if(value>=60){
      hue = 340;
      brt = 100;
    }

    RgbColor colour = HsbColor(HueN2F(hue),1.0f,BrtN2F(brt));

    // colour = RgbColor((int)value, 2, 3);

    return colour;



}



/*********************************************************************************************\
 * Change scales from 8 bits to 10 bits and vice versa
\*********************************************************************************************/
// 8 to 10 to 8 is garanteed to give the same result
uint16_t mInterfaceLight::change8to10(uint8_t v) {
  return mapvalue(v, 0, 255, 0, 1023);
}
// change from 10 bits to 8 bits, but any non-zero input will be non-zero
uint8_t mInterfaceLight::change10to8(uint16_t v) {
  return (0 == v) ? 0 : mapvalue(v, 4, 1023, 1, 255);
}

/*********************************************************************************************\
 * Gamma correction
\*********************************************************************************************/
#ifdef ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION


  // compute actual PWM min/max values from DimmerRange
  // must be called when DimmerRange is changed or LedTable
  void mInterfaceLight::LightCalcPWMRange(void) {
    uint16_t _pwm_min=0, _pwm_max=1023;

    _pwm_min = change8to10(0);//DimmerToBri(pCONT_set->Settings.dimmer_hw_min));   // default 0
    _pwm_max = change8to10(255);//DimmerToBri(pCONT_set->Settings.dimmer_hw_max));   // default 100
    // if (pCONT_set->Settings.light_correction) {
      _pwm_min = ledGamma10_10(_pwm_min);       // apply gamma correction
      _pwm_max = ledGamma10_10(_pwm_max);       // 0..1023
    // }
    _pwm_min = _pwm_min > 0 ? mapvalue(_pwm_min, 1, 1023, 1, pCONT_set->Settings.pwm_range) : 0;  // adapt range but keep zero and non-zero values
    _pwm_max = mapvalue(_pwm_max, 1, 1023, 1, pCONT_set->Settings.pwm_range);  // _pwm_max cannot be zero

     pCONT_iLight->pwm_min = _pwm_min;
     pCONT_iLight->pwm_max = _pwm_max;
    // AddLog_P2(LOG_LEVEL_TEST, PSTR("LightCalcPWMRange %d %d - %d %d"), settings.dimmer_hw_min, settings.dimmer_hw_max, pwm_min, pwm_max);
  }

// Calculate the gamma corrected value for LEDS
uint16_t mInterfaceLight::ledGamma_internal(uint16_t v, const struct gamma_table_t *gt_ptr) {
  uint16_t from_src = 0;
  uint16_t from_gamma = 0;

  for (const gamma_table_t *gt = gt_ptr; ; gt++) {
    uint16_t to_src = gt->to_src;
    uint16_t to_gamma = gt->to_gamma;
    if (v <= to_src) {
      return mapvalue(v, from_src, to_src, from_gamma, to_gamma);
    }
    from_src = to_src;
    from_gamma = to_gamma;
  }
}
// Calculate the reverse gamma value for LEDS
uint16_t mInterfaceLight::ledGammaReverse_internal(uint16_t vg, const struct gamma_table_t *gt_ptr) {
  uint16_t from_src = 0;
  uint16_t from_gamma = 0;

  for (const gamma_table_t *gt = gt_ptr; ; gt++) {
    uint16_t to_src = gt->to_src;
    uint16_t to_gamma = gt->to_gamma;
    if (vg <= to_gamma) {
      return mapvalue(vg, from_gamma, to_gamma, from_src, to_src);
    }
    from_src = to_src;
    from_gamma = to_gamma;
  }
}

// 10 bits in, 10 bits out
uint16_t mInterfaceLight::ledGamma10_10(uint16_t v) {
  return ledGamma_internal(v, gamma_table);
}
// 10 bits resolution, 8 bits in
uint16_t mInterfaceLight::ledGamma10(uint8_t v) {
  return ledGamma10_10(change8to10(v));
}

// Legacy function
uint8_t mInterfaceLight::ledGamma(uint8_t v) {
  return change10to8(ledGamma10(v));
}



// Just apply basic Gamma to each channel
void mInterfaceLight::calcGammaMultiChannels(uint16_t cur_col_10[5]) {
  // Apply gamma correction for 8 and 10 bits resolutions, if needed
  if (pCONT_set->Settings.light_settings.light_correction) {
    for (uint32_t i = 0; i < LST_MAX; i++) {
      cur_col_10[i] = ledGamma10_10(cur_col_10[i]);
    }
  }
}

void mInterfaceLight::calcGammaBulbs(uint16_t cur_col_10[5]) {
//   // Apply gamma correction for 8 and 10 bits resolutions, if needed

//   // First apply combined correction to the overall white power
//   if ((LST_COLDWARM == subtype) || (LST_RGBCW == subtype)) {
//     // channels for white are always the last two channels
//     uint32_t cw1 = subtype - 1;       // address for the ColorTone PWM
//     uint32_t cw0 = subtype - 2;       // address for the White Brightness PWM
//     uint16_t white_bri10 = cur_col_10[cw0] + cur_col_10[cw1];   // cumulated brightness
//     uint16_t white_bri10_1023 = (white_bri10 > 1023) ? 1023 : white_bri10;    // max 1023

// #ifdef ESP8266
//     if (
//       // (MODULE_PHILIPS_ID == pCONT_set->my_module_type) || 
//       (pCONT_set->Settings.flag4.pwm_ct_mode)) {   // channel 1 is the color tone, mapped to cold channel (0..255)
//       // Xiaomi Philips bulbs follow a different scheme:
//       cur_col_10[cw1] = getCT10bits();
//       // channel 0=intensity, channel1=temperature
//       if (pCONT_set->Settings.light_settings.light_correction) { // gamma correction
//         cur_col_10[cw0] = ledGamma10_10(white_bri10_1023);    // 10 bits gamma correction
//       } else {
//         cur_col_10[cw0] = white_bri10_1023;  // no gamma, extend to 10 bits
//       }
//     } else
// #endif  // ESP8266
//     if (pCONT_set->Settings.light_settings.light_correction) {
//       // if sum of both channels is > 255, then channels are probably uncorrelated
//       if (white_bri10 <= 1031) {      // take a margin of 8 above 1023 to account for rounding errors
//         // we calculate the gamma corrected sum of CW + WW
//         uint16_t white_bri_gamma10 = ledGamma10_10(white_bri10_1023);
//         // then we split the total energy among the cold and warm leds
//         cur_col_10[cw0] = mapvalue(cur_col_10[cw0], 0, white_bri10_1023, 0, white_bri_gamma10);
//         cur_col_10[cw1] = mapvalue(cur_col_10[cw1], 0, white_bri10_1023, 0, white_bri_gamma10);
//       } else {
//         cur_col_10[cw0] = ledGamma10_10(cur_col_10[cw0]);
//         cur_col_10[cw1] = ledGamma10_10(cur_col_10[cw1]);
//       }
//     }
//   }

//   if (pCONT_set->Settings.light_settings.light_correction) {
//     // then apply gamma correction to RGB channels
//     if (LST_RGB <= subtype) {
//       for (uint32_t i = 0; i < 3; i++) {
//         cur_col_10[i] = ledGamma10_10(cur_col_10[i]);
//       }
//     }
//     // If RGBW or Single channel, also adjust White channel
//     if ((LST_SINGLE == subtype) || (LST_RGBW == subtype)) {
//       cur_col_10[subtype - 1] = ledGamma10_10(cur_col_10[subtype - 1]);
//     }
//   }
}


bool mInterfaceLight::isChannelGammaCorrected(uint32_t channel) {
  if (!pCONT_set->Settings.light_settings.light_correction) { return false; }   // Gamma correction not activated
  if (channel >= subtype) { return false; }     // Out of range
#ifdef ESP8266
//   if (
//     // (MODULE_PHILIPS_ID == pCONT_set->my_module_type) || 
//   (pCONT_set->Settings.flag4.pwm_ct_mode)) {
//     if ((LST_COLDWARM == subtype) && (1 == channel)) { return false; }   // PMW reserved for CT
//     if ((LST_RGBCW == subtype) && (4 == channel)) { return false; }   // PMW reserved for CT
//   }
#endif  // ESP8266
  return true;
}


// Calculate the Gamma correction, if any, for fading, using the fast Gamma curve (10 bits in+out)
uint16_t mInterfaceLight::fadeGamma(uint32_t channel, uint16_t v) {
  if (isChannelGammaCorrected(channel)) {
    return ledGamma_internal(v, gamma_table_fast);
  } else {
    return v;
  }
}
uint16_t mInterfaceLight::fadeGammaReverse(uint32_t channel, uint16_t vg) {
  if (isChannelGammaCorrected(channel)) {
    return ledGammaReverse_internal(vg, gamma_table_fast);
  } else {
    return vg;
  }
}

#endif // ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

#endif // USE_DRIVER
