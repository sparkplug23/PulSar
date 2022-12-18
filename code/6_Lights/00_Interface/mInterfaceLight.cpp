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
    
  //   #ifdef DEFAULT_LIGHTING_ANIMATION_MODE__ID
  //     animation.mode_id = DEFAULT_LIGHTING_ANIMATION_MODE__ID<ANIMATION_MODE_LENGTH__ID?
  //                           DEFAULT_LIGHTING_ANIMATION_MODE__ID:ANIMATION_MODE_EFFECTS__ID;
  //   #else
  //     animation.mode_id = ANIMATION_MODE_EFFECTS__ID;
  //   #endif

  // }

}



RgbcctColor mInterfaceLight::GetActiveFirstColourFromCurrentPalette(){

  mPaletteI->SetPaletteListPtrFromID(pCONT_lAni->_segments[0].palette.id);
  
  uint8_t pixels_in_map = mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr);
          
  #ifdef ENABLE_LOG_LEVEL_DEBUG
  AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_LIGHT "pixels_in_map %d"),pixels_in_map);
  #endif

  uint16_t desired_pixel = 0; // always first
  uint8_t start_pixel_position = 0;
  // RgbcctColor colour = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr, desired_pixel, &start_pixel_position);

  RgbcctColor colour = mPaletteI->GetColourFromPreloadedPalette(pCONT_lAni->_segments[0].palette.id, desired_pixel, &start_pixel_position);

  return colour;

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

    
  pCONT_lAni->_segments[0].hardware_element_colour_order.red = 0;
  pCONT_lAni->_segments[0].hardware_element_colour_order.green = 1;
  pCONT_lAni->_segments[0].hardware_element_colour_order.blue = 2;
  pCONT_lAni->_segments[0].hardware_element_colour_order.white_cold = 3;
  pCONT_lAni->_segments[0].hardware_element_colour_order.white_warm = 4;

  //temp fix
  if (pCONT_pins->PinUsed(GPIO_RGB_DATA_ID)) { 
    // AddLog(LOG_LEVEL_TEST,PSTR("pCONT_set->Settings.light_settings.type=%d"),pCONT_set->Settings.light_settings.type);
    pCONT_set->Settings.light_settings.type = LT_ADDRESSABLE_WS281X;  // NEEDS METHOD HERE OF SETTING SK6812
  }

  if (pCONT_set->Settings.light_settings.type > LT_BASIC) {
    pCONT_set->devices_present++;
  }

  // // post-process for lights
  // if (Settings.flag3.pwm_multi_channels) {  // SetOption68 - Enable multi-channels PWM instead of Color PWM
  //   uint32_t pwm_channels = (Settings.light_settings.type & 7) > LST_MAX ? LST_MAX : (Settings.light_settings.type & 7);
  //   if (0 == pwm_channels) { pwm_channels = 1; }
  //   devices_present += pwm_channels - 1;    // add the pwm channels controls at the end
  // } else if ((Settings.setoption_255[P_RGB_REMAP] & 128) && (LST_RGBW <= (Settings.light_settings.type & 7))) {
  //   // if RGBW or RGBCW, and SetOption37 >= 128, we manage RGB and W separately, hence adding a device
  //   devices_present++;
  // }

  return (pCONT_set->Settings.light_settings.type > LT_BASIC);
}


void mInterfaceLight::Template_Load(){

  // #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
  AddLog(LOG_LEVEL_HIGHLIGHT, PSTR("mInterfaceLight::Template_Load()"));
  // #endif

  #ifdef USE_LIGHTING_TEMPLATE
  // load from progmem into local
  D_DATA_BUFFER_CLEAR();
  memcpy_P(data_buffer.payload.ctr,LIGHTING_TEMPLATE,sizeof(LIGHTING_TEMPLATE));
  data_buffer.payload.len = strlen(data_buffer.payload.ctr);

  // AddLog(LOG_LEVEL_TEST, PSTR("mInterfaceLight::Template_Load SettingsMerge\n\r\n\r\n\r\n\r"
  //     //Test data
  //     "%d:%d:%d:%d"
  //   ),
  //     //Testdata
  //      pCONT_set->Settings.animation_settings.xmas_controller_params[0]
  //     ,pCONT_set->Settings.animation_settings.xmas_controller_params[1]
  //     ,pCONT_set->Settings.animation_settings.xmas_controller_params[2]
  //     ,pCONT_set->Settings.animation_settings.xmas_controller_params[3]
  // );

  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_DEBUG, PSTR("LIGHTING_TEMPLATE Load"));// " READ = \"%s\""), data_buffer.payload.ctr);
  ALOG_DBM( PSTR("LIGHTING_TEMPLATE" " READ = \"%s\""), data_buffer.payload.ctr);
  #endif // ENABLE_LOG_LEVEL_COMMANDS

  pCONT->Tasker_Interface(FUNC_JSON_COMMAND_ID);
  #endif // USE_LIGHTING_TEMPLATE

}


void mInterfaceLight::Init(void) //LightInit(void)
{

  device = pCONT_set->devices_present;
  
  //subtype = (pCONT_set->Settings.light_settings.type & 7) > LST_MAX ? LST_MAX : (pCONT_set->Settings.light_settings.type & 7); // Always 0 - LST_MAX (5)
  subtype = RgbcctColor_Controller::LightSubType::LIGHT_TYPE__RGBCCT__ID;
  
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
  for (int ii=0;ii<(mPaletteI->PALETTELIST_VARIABLE_HSBID_LENGTH__ID-mPaletteI->PALETTELIST_VARIABLE_HSBID_01__ID);ii++){ 
    sprintf(buffer, D_DEFAULT_DYNAMIC_PALETTE_NAMES__VARIABLE_HSBID__NAME_CTR, ii);
    // DLI->AddDeviceName(buffer,E M_MODULE_LIGHTS_INTERFACE_ID,ii);
    DLI->AddDeviceName(buffer, GetModuleUniqueID(), ii + mPaletteI->PALETTELIST_VARIABLE_HSBID_01__ID);
  }
  for (int ii=0;ii<(mPaletteI->PALETTELIST_VARIABLE_RGBCCT_LENGTH__ID-mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_01__ID);ii++){ 
    sprintf(buffer, D_DEFAULT_DYNAMIC_PALETTE_NAMES__VARIABLE_RGBCCT__NAME_CTR, ii);
    DLI->AddDeviceName(buffer, GetModuleUniqueID(), ii + mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_01__ID);
  }
  for (int ii=0;ii<(mPaletteI->PALETTELIST_VARIABLE_GENERIC_LENGTH__ID-mPaletteI->PALETTELIST_VARIABLE_GENERIC_01__ID);ii++){ 
    sprintf(buffer, D_DEFAULT_DYNAMIC_PALETTE_NAMES__VARIABLE_GENERIC__NAME_CTR, ii);
    DLI->AddDeviceName(buffer, GetModuleUniqueID(), ii + mPaletteI->PALETTELIST_VARIABLE_GENERIC_01__ID);
  }

  #ifdef USE_MODULE_LIGHTS_PWM
    settings.flags.enable_cct_channel_sliders = true;
  #else
    settings.flags.enable_cct_channel_sliders = false;
  #endif // USE_MODULE_LIGHTS_PWM

  // if (LST_RGBW <= subtype) {
  //   // only change if RGBW or RGBCW
  //   // do not allow independant RGB and WC colors
  //   bool ct_rgb_linked = 0;// !(pCONT_set->Settings.setoption_255[P_RGB_REMAP] & 128);
  //   CommandSet_RGBCT_Linked(ct_rgb_linked);
  // }

  // DEBUG_LINE_HERE;



// moved
//   DEBUG_LINE_HERE;
//   /***
//    * Configure RgbcctController Instance
//    * */
//   CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_01__ID);
  
// #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//       pCONT_lAni->_segment_runtimes[0].rgbcct_controller->
// #else
//       rgbcct_controller.
// #endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
// setSubType(subtype);
  
// #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//       pCONT_lAni->_segment_runtimes[0].rgbcct_controller->
// #else
//       rgbcct_controller.
// #endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
// setApplyBrightnessToOutput(false);
//   if(pCONT_set->Settings.light_settings.type == LT_ADDRESSABLE_WS281X){ //RGB only
    
// #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//       pCONT_lAni->_segment_runtimes[0].rgbcct_controller->
// #else
//       rgbcct_controller.
// #endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
// setColorMode(LCM_RGB);
//   }else{
    
// #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//       pCONT_lAni->_segment_runtimes[0].rgbcct_controller->
// #else
//       rgbcct_controller.
// #endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
// setColorMode(LCM_BOTH);
//   }
  
// #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//       pCONT_lAni->_segment_runtimes[0].rgbcct_controller->
// #else
//       rgbcct_controller.
// #endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
// Sync();    // calculate the initial values (#8058)
//   // RGB parts
  
// #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//       pCONT_lAni->_segment_runtimes[0].rgbcct_controller->
// #else
//       rgbcct_controller.
// #endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
// setRGB(1,2,3);
//   // CCT parts
  
// #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//       pCONT_lAni->_segment_runtimes[0].rgbcct_controller->
// #else
//       rgbcct_controller.
// #endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
// setRGBCCTLinked(false);
  
// #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//       pCONT_lAni->_segment_runtimes[0].rgbcct_controller->
// #else
//       rgbcct_controller.
// #endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
// setCCT(153);
  



















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
//   } else if ((Settings.setoption_255[P_RGB_REMAP] & 128) && (LST_RGBW <= (Settings.light_settings.type & 7))) {
//     // if RGBW or RGBCW, and SetOption37 >= 128, we manage RGB and W separately, hence adding a device
//     devices_present++;
//   }

//   return (Settings.light_settings.type > LT_BASIC);
// }

// changeBri(0); //default off

  //create copy for pCONT_iLight->animation stored
  // memcpy(&animation_stored,&animation,sizeof(animation_stored));// RESTORE copy of state

} //light_init



void mInterfaceLight::init_Animations(){

  pCONT_lAni->_segments[0].flags.fEnable_Animation = true;
  pCONT_lAni->_segments[0].flags.NewAnimationRequiringCompleteRefresh = true;

}




// Colours etc will be made or called using a func_update_light_Values
// Another func will push those values to the output, which each hardware class will handle

// // Used for timed on or off events
int8_t mInterfaceLight::Tasker(uint8_t function, JsonParserObject obj)
{
  
  int8_t function_result = 0;

  // As interface module, the parsing of module_init takes precedence over the Settings.light_settings.type
  switch(function){
    case FUNC_TEMPLATE_MODULE_LOAD_FROM_PROGMEM:
    case FUNC_TEMPLATE_DEVICE_LOAD_FROM_PROGMEM:
    
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_TEST, PSTR("mInterfaceLight::Tasker\n\r\n\r\n\r\n\r"
        //Test data
        "%d:%d:%d:%d"
      ),
        //Testdata
         pCONT_set->Settings.animation_settings.xmas_controller_params[0]
        ,pCONT_set->Settings.animation_settings.xmas_controller_params[1]
        ,pCONT_set->Settings.animation_settings.xmas_controller_params[2]
        ,pCONT_set->Settings.animation_settings.xmas_controller_params[3]
    );
    #endif // ENABLE_LOG_LEVEL_INFO



      Template_Load();
    break;
    case FUNC_POINTER_INIT:

      // mpalette = new mPalette();

      mPaletteI->init_PresetColourPalettes();
      
// #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

//       pCONT_lAni->_segment_runtimes[0].rgbcct_controller->setRgbcctColourOutputAddress(mPaletteI->palettelist.rgbcct_users[0].data);
//       // active_scene_palette_id = PALETTELIST_VARIABLE_RGBCCT_COLOUR_01__ID;
//       // active_rgbcct_colour_p = reinterpret_cast<RgbcctColor*>(&pCONT_set->Settings.animation_settings.palette_rgbcct_user_colour_map_ids[0]); // use first for now
//       CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_01__ID);
// #else

//       rgbcct_controller.setRgbcctColourOutputAddress(mPaletteI->palettelist.rgbcct_users[0].data);
//       // active_scene_palette_id = PALETTELIST_VARIABLE_RGBCCT_COLOUR_01__ID;
//       // active_rgbcct_colour_p = reinterpret_cast<RgbcctColor*>(&pCONT_set->Settings.animation_settings.palette_rgbcct_user_colour_map_ids[0]); // use first for now
//       CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_01__ID);
//   DEBUG_LINE_HERE;


// #endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

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
      
      light_power_state = pCONT_lAni->_segment_runtimes[0].rgbcct_controller->getBrightness255()?1:0;

      //AddLog(LOG_LEVEL_TEST, PSTR("light_power_state=%d"),light_power_state);
      #endif // USE_MODULE_LIGHTS_ANIMATOR

#ifdef ENABLE_DEVFEATURE_LEARNING_FASTLED_PALETTES

      uint8_t pbri = 255;

uint8_t paletteIndex = 0; 
CRGB colour;
//  = ColorFromPalette( Test_p, paletteIndex, pbri, NOBLEND);
// AddLog(LOG_LEVEL_TEST, PSTR("colour %d %d,%d,%d"),paletteIndex,colour.r,colour.g,colour.b);


uint8_t index = 15;

paletteIndex = ((255/16)*index)-1; 
colour = ColorFromPalette( Test_p, paletteIndex, pbri, LINEARBLEND);
AddLog(LOG_LEVEL_TEST, PSTR("colour %d %d,%d,%d"),paletteIndex,colour.r,colour.g,colour.b);

paletteIndex = 225; 
colour = ColorFromPalette( Test_p, paletteIndex, pbri, LINEARBLEND);
AddLog(LOG_LEVEL_TEST, PSTR("colour %d %d,%d,%d"),paletteIndex,colour.r,colour.g,colour.b);
paletteIndex = 226; 
colour = ColorFromPalette( Test_p, paletteIndex, pbri, LINEARBLEND);
AddLog(LOG_LEVEL_TEST, PSTR("colour %d %d,%d,%d"),paletteIndex,colour.r,colour.g,colour.b);
paletteIndex = 227; 
colour = ColorFromPalette( Test_p, paletteIndex, pbri, LINEARBLEND);
AddLog(LOG_LEVEL_TEST, PSTR("colour %d %d,%d,%d"),paletteIndex,colour.r,colour.g,colour.b);


paletteIndex = 239;//(((255/16)*index)-1)+15; 
colour = ColorFromPalette( Test_p, paletteIndex, pbri, LINEARBLEND);
AddLog(LOG_LEVEL_TEST, PSTR("colour %d %d,%d,%d"),paletteIndex,colour.r,colour.g,colour.b);


paletteIndex = 250; 
colour = ColorFromPalette( Test_p, paletteIndex, pbri, LINEARBLEND);
AddLog(LOG_LEVEL_TEST, PSTR("colour %d %d,%d,%d"),paletteIndex,colour.r,colour.g,colour.b);
paletteIndex = 251; 
colour = ColorFromPalette( Test_p, paletteIndex, pbri, LINEARBLEND);
AddLog(LOG_LEVEL_TEST, PSTR("colour %d %d,%d,%d"),paletteIndex,colour.r,colour.g,colour.b);
paletteIndex = 252; 
colour = ColorFromPalette( Test_p, paletteIndex, pbri, LINEARBLEND);
AddLog(LOG_LEVEL_TEST, PSTR("colour %d %d,%d,%d"),paletteIndex,colour.r,colour.g,colour.b);
paletteIndex = 253; 
colour = ColorFromPalette( Test_p, paletteIndex, pbri, LINEARBLEND);
AddLog(LOG_LEVEL_TEST, PSTR("colour %d %d,%d,%d"),paletteIndex,colour.r,colour.g,colour.b);
paletteIndex = 254; 
colour = ColorFromPalette( Test_p, paletteIndex, pbri, LINEARBLEND);
AddLog(LOG_LEVEL_TEST, PSTR("colour %d %d,%d,%d"),paletteIndex,colour.r,colour.g,colour.b);
paletteIndex = 255; 
colour = ColorFromPalette( Test_p, paletteIndex, pbri, LINEARBLEND);
AddLog(LOG_LEVEL_TEST, PSTR("colour %d %d,%d,%d"),paletteIndex,colour.r,colour.g,colour.b);


 mPaletteI->currentPalette = Test_p;
 pCONT_lAni->_virtualSegmentLength = 50;
 pCONT_lAni->paletteBlend = 3;
 uint16_t j = 0;
  for(uint16_t i = 0; i < 50; i++) {
    
    j = i;//map(i,0,50,0,255);
    pCONT_lAni->SetPixelColor(j, pCONT_lAni->color_from_palette(i, true, false, 10));
    // pCONT_lAni->SetPixelColor(i, mPaletteI->color_from_palette_internal(i, true, true, 10));
  }
    // pCONT_lAni->SetPixelColor(0, RgbColor(255,0,0));
  pCONT_lAni->_segment_runtimes[0].animation_has_anim_callback = false; // When no animation callback is needed
  pCONT_lAni->StripUpdate();

#endif // ENABLE_DEVFEATURE_LEARNING_FASTLED_PALETTES

    #ifdef ENABLE_DEVFEATURE_PALETTE_ADVANCED_METHODS_GEN2
uint16_t paletteIndex= mPalette::PALETTELIST_STATIC_CHRISTMAS_01__ID;
RgbcctColor colour = mPaletteI->GetColourFromPalette_Gradient(paletteIndex, 0);
AddLog(LOG_LEVEL_TEST, PSTR("colour %d %d,%d,%d"),paletteIndex,colour.R,colour.G,colour.B);
    #endif // ENABLE_DEVFEATURE_PALETTE_ADVANCED_METHODS_GEN2


// RgbcctColor mPalette::GetColourFromPalette_Gradient(uint16_t palette_id, uint16_t pixel_num, int16_t *pixel_position, bool mapping, bool wrap, uint8_t mcol, uint8_t pbri, TBlendType blendType, uint16_t palette_spread_length__virtualSegmentLength)
// {



// Serial.println("palette_rgbcct_users_colour_map");
// for(int i=0;i<25;i++)
// {
//   Serial.printf("%d,", pCONT_set->Settings.animation_settings.palette_rgbcct_users_colour_map[i]);
// }
// Serial.println("every second");

// Serial.println("rgbcct0");
// for(int i=0;i<5;i++)
// {
//   Serial.printf("%d,", mPaletteI->palettelist.rgbcct_users[0].data[i]);
// }
// Serial.println("rgbcct1");
// for(int i=0;i<5;i++)
// {
//   Serial.printf("%d,", mPaletteI->palettelist.rgbcct_users[1].data[i]);
// }
// Serial.println("every second");

// (&pCONT_set->Settings.animation_settings.palette_rgbcct_users_colour_map[5*palette_id_adjusted_to_array_index])



      // AddLog(LOG_LEVEL_TEST, PSTR("sizeof(RgbcctColor_Controller)=%d"),sizeof(RgbcctColor_Controller));
      

      EverySecond_AutoOff();
    
    }break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    // None json commands
    case LIGHT_POWER_STATE_OFF_ID: //probably phase out for "SetPower" is generic event type
      CommandSet_LightPowerState(0);
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
  } // end switch
  
  /************
   * WEBPAGE SECTION * 
  *******************/
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  return Tasker_Web(function);
  #endif // USE_MODULE_NETWORK_WEBSERVER

} // END function


#ifdef USE_MODULE_CORE_RULES

void mInterfaceLight::RulesEvent_Set_Power()
{

  // struct RELAY_EVENT_PARAMETERS{
  //   uint8_t index;
  //   uint8_t state;
  //   uint8_t state;
  // }rule_event_layout;


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



/**
 * This function, is the entry point for the output that is hardware dependent 
 * colour_hardware will already have colour_order mapped
 * */
void mInterfaceLight::SetPixelColourHardwareInterface(RgbcctColor colour, uint16_t index, bool flag_replicate_for_total_pixel_length){

  #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
   ALOG_INF( PSTR("SetPixelColor: %d\t(%d,%d,%d,%d,%d) pal%d"), index, colour.R, colour.G, colour.B, colour.W1, colour.W2, pCONT_lAni->_segments[pCONT_lAni->segment_active_index].palette.id );
  #endif

  switch(pCONT_set->Settings.light_settings.type){
    case LT_ADDRESSABLE_WS281X:
    case LT_ADDRESSABLE_SK6812:
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      pCONT_ladd->SetPixelColorHardware(index, colour, flag_replicate_for_total_pixel_length);
      #endif // USE_MODULE_LIGHTS_ADDRESSABLE
    break;
    case LT_PWM1:
    case LT_PWM2:
    case LT_PWM3:
    case LT_PWM4:
    case LT_PWM5:
      #ifdef USE_MODULE_LIGHTS_PWM
      pCONT_lPWM->SetPixelColorHardware(index, colour);
      #endif // USE_MODULE_LIGHTS_PWM
    break;
  }
  
}
RgbcctColor mInterfaceLight::GetPixelColourHardwareInterface(uint16_t index){

  switch(pCONT_set->Settings.light_settings.type){
    case LT_ADDRESSABLE_WS281X:  
    case LT_ADDRESSABLE_SK6812:
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

  // ALOG_INF(PSTR("t=%d"),pCONT_set->Settings.light_settings.type);

  switch(pCONT_set->Settings.light_settings.type)
  {
    case LT_ADDRESSABLE_WS281X:  
    case LT_ADDRESSABLE_SK6812:
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
      
  // #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
  // AddLog(LOG_LEVEL_TEST, PSTR("Invalid Light LT_ADDRESSABLE_WS281X HERE %d"),pCONT_set->Settings.light_settings.type);
  // #endif
  for( 
    int ii = 0; 
    ii<MAX_NUM_SEGMENTS; 
    ii++
  ){

  // /**
  //  * @brief 
  //  * If PWM types, should merge this into below, as effects are output agnostic
  //  * 
  //  */
  // if(pCONT_set->Settings.light_settings.type < LT_LIGHT_INTERFACE_END){
     
  //   switch(pCONT_lAni->_segments[0].mode_id)    
  //     {
  //     case ANIMATION_MODE_EFFECTS_ID:
  //       pCONT_lAni->SubTask_Segments_Animation();
  //     break;
  //     case ANIMATION_MODE_NONE_ID: default: break; // resting position
  //   }  
  // }

  
  if((pCONT_set->Settings.light_settings.type < LT_LIGHT_INTERFACE_END)||
     (pCONT_set->Settings.light_settings.type == LT_ADDRESSABLE_WS281X)||
     (pCONT_set->Settings.light_settings.type == LT_ADDRESSABLE_SK6812)){

  // if(pCONT_set->Settings.light_settings.type == LT_ADDRESSABLE_WS281X){ 
    
    // AddLog(LOG_LEVEL_DEBUG, PSTR("Invalid Light LT_ADDRESSABLE_WS281X %d"),animation.mode_id);
    #ifdef USE_MODULE_LIGHTS_ANIMATOR
    
    switch(pCONT_lAni->_segments[0].mode_id)    // needs to know the id 
    {
      /**
       * New Segments animations: Merging WLED/HACS into this mode, wait until 2022 to make this happen.
       * */
      #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
      case ANIMATION_MODE_EFFECTS_ID:
        pCONT_lAni->SubTask_Segments_Animation(ii);
      break;
      #endif
      /**
       * Notifications: Single pixel basic animations (pulse, flash, on/off)
       * */
      #ifdef ENABLE_FEATURE_PIXEL__MODE_NOTIFICATION
      case ANIMATION_MODE_NOTIFICATIONS_ID:
        pCONT_lAni->SubTask_Notifications();   // Convert this into its own segment effect, effect will need to be set first
      break;
      #endif
      /**
       * Ambilight: Light strips for behind monitors, either static, sunelevation animations, getting TCP/HTTP/Serial data for the pixels from a computer (Eg wallpapers with putty or movies)
       * */
      #ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
      case ANIMATION_MODE_AMBILIGHT__ID:
        pCONT_lAni->SubTask_Ambilight_Main();
      break;
      #endif
      /**
       * Set Pixel: Manual method via mqtt json
       * */
      #ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL // serial, wifi udp connection
      case ANIMATION_MODE_MANUAL_SETPIXEL_ID:
        // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_LIGHT "ANIMATION_MODE_EFFECTS__ID"));
        pCONT_lAni-> SubTask_Manual_SetPixel();
        // light_power_state = true;
      break;
      #endif
      case ANIMATION_MODE_NONE_ID: default: break; // resting position
    }
    #endif
      
  }
  else
  {
    #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
    ALOG_DBM( PSTR("Invalid Light Type"));
    #endif
  }

  }

} // END everyloop




void mInterfaceLight::EverySecond_AutoOff(){

  //ALOG_DBM( PSTR(D_LOG_LIGHT "scene.auto_off_settings.tSaved [%d]"),animation.auto_off_settings.time_decounter_secs);
  if(auto_off_settings.time_decounter_secs==1){ //if =1 then turn off and clear to 0
    // animation.name_id = MODE_SINGLECOLOUR_FADE_OFF__ID;
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT "auto_off_settings.time_decounter_secs==1 and disable"));
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

  colour->R  = mapvalue(colour->R,  0, 255, 0,pCONT_lAni->_segment_runtimes[0].rgbcct_controller->
getBrightnessRGB255());
  colour->G  = mapvalue(colour->G,  0, 255, 0,pCONT_lAni->_segment_runtimes[0].rgbcct_controller->
getBrightnessRGB255());
  colour->B  = mapvalue(colour->B,  0, 255, 0,pCONT_lAni->_segment_runtimes[0].rgbcct_controller->
getBrightnessRGB255());
  colour->WW = mapvalue(colour->WW, 0, 255, 0,pCONT_lAni->_segment_runtimes[0].rgbcct_controller->
getBrightnessCCT255());
  colour->WC = mapvalue(colour->WC, 0, 255, 0,pCONT_lAni->_segment_runtimes[0].rgbcct_controller->
getBrightnessCCT255());

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




void mInterfaceLight::BootMessage(){
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_INFO, PSTR("BOOT: " "LightCount=%d"), settings.light_size_count);
  #endif // ENABLE_LOG_LEVEL_COMMANDS

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

/**
 * @brief future global way to map colours using palette
 * 
 * @param value 
 * @param map_style_id 
 * @param value_min 
 * @param value_max 
 * @param map_is_palette_id 
 * @return RgbColor 
 */
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

/**
 * @brief maximum sat/brt, hue changes
 * 
 * @param value 
 * @param map_style_id 
 * @param value_min 
 * @param value_max 
 * @param map_is_palette_id 
 * @return RgbColor 
 */
RgbColor mInterfaceLight::GetColourValueUsingMapsMaximumBrightness(float value, 
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
      brt = 100;
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

    RgbColor colour = HsbColor(HueN2F(hue),1.0f,1.0f);

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
//       // (MODULE_PHILIPS__ID == pCONT_set->my_module_type) || 
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
//     // (MODULE_PHILIPS__ID == pCONT_set->my_module_type) || 
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
