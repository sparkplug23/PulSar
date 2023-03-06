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

  mPaletteI->SetPaletteListPtrFromID(pCONT_lAni->SEGMENT_I(0).palette.id);
  
  uint8_t pixels_in_map = mPaletteI->GetNumberOfColoursInPalette(mPaletteI->palettelist.ptr);
          
  #ifdef ENABLE_LOG_LEVEL_DEBUG
  AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_LIGHT "pixels_in_map %d"),pixels_in_map);
  #endif

  uint16_t desired_pixel = 0; // always first
  uint8_t start_pixel_position = 0;
  // RgbcctColor colour = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr, desired_pixel, &start_pixel_position);

  RgbcctColor colour = mPaletteI->GetColourFromPreloadedPalette(pCONT_lAni->SEGMENT_I(0).palette.id, desired_pixel, &start_pixel_position);

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


void mInterfaceLight::Template_Load()
{

  ALOG_DBM(PSTR("mInterfaceLight::Template_Load()"));

  #ifdef USE_LIGHTING_TEMPLATE
  // load from progmem into local
  D_DATA_BUFFER_CLEAR();
  memcpy_P(data_buffer.payload.ctr,LIGHTING_TEMPLATE,sizeof(LIGHTING_TEMPLATE));
  data_buffer.payload.len = strlen(data_buffer.payload.ctr);

  ALOG_DBM( PSTR("LIGHTING_TEMPLATE" " READ = \"%s\""), data_buffer.payload.ctr);

  pCONT->Tasker_Interface(FUNC_JSON_COMMAND_ID);
  #endif // USE_LIGHTING_TEMPLATE

}


void mInterfaceLight::Init(void) //LightInit(void)
{

  device = pCONT_set->devices_present;
  
  //subtype = (pCONT_set->Settings.light_settings.type & 7) > LST_MAX ? LST_MAX : (pCONT_set->Settings.light_settings.type & 7); // Always 0 - LST_MAX (5)
  subtype = RgbcctColor::LightSubType::LIGHT_TYPE__RGBCCT__ID;
  
  pwm_multi_channels = 0;//pCONT_set->Settings.flag3.pwm_multi_channels;  // SetOption68 - Enable multi-channels PWM instead of Color PWM

  pCONT_set->Settings.pwm_range = PWM_RANGE; //tmp

  pCONT_set->Settings.light_settings.light_fade = 1;
  pCONT_set->Settings.light_settings.light_speed = 5*2;
  pCONT_set->power = 1;

  auto_off_settings.time_decounter_secs = 0;

  ALOG_DBM(PSTR("mPaletteI->init_PresetColourPalettes(); to be made internal to class"));
  #ifdef ENABLE_DEVFEATURE_REMOVE_INIT_OUTSIDE_OF_PALETTE_CLASS
  mPaletteI->init_PresetColourPalettes();
  init_Animations();
  #endif // ENABLE_DEVFEATURE_REMOVE_INIT_OUTSIDE_OF_PALETTE_CLASS

  
  char buffer[30];
  // Default user names for palette
  for (int ii=0;ii<(mPaletteI->PALETTELIST_VARIABLE_HSBID_LENGTH__ID-mPaletteI->PALETTELIST_VARIABLE_HSBID_01__ID);ii++){ 
    sprintf(buffer, D_DEFAULT_DYNAMIC_PALETTE_NAMES__VARIABLE_HSBID__NAME_CTR, ii);
    // DLI->AddDeviceName(buffer,E M_MODULE_LIGHTS_INTERFACE_ID,ii);
    DLI->AddDeviceName(buffer, GetModuleUniqueID(), ii + mPaletteI->PALETTELIST_VARIABLE_HSBID_01__ID);
  }
  for (int ii=0;ii<(mPaletteI->PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID-mPaletteI->PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_01__ID);ii++){ 
    sprintf(buffer, D_DEFAULT_DYNAMIC_PALETTE_NAMES__VARIABLE_RGBCCT__NAME_CTR, ii);
    DLI->AddDeviceName(buffer, GetModuleUniqueID(), ii + mPaletteI->PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_01__ID);
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
//   CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_01__ID);
  
// #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//       pCONT_lAni->SEGMENT_I(0).rgbcct_controller->
// #else
//       rgbcct_controller.
// #endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
// setSubType(subtype);
  
// #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//       pCONT_lAni->SEGMENT_I(0).rgbcct_controller->
// #else
//       rgbcct_controller.
// #endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
// setApplyBrightnessToOutput(false);
//   if(pCONT_set->Settings.light_settings.type == LT_ADDRESSABLE_WS281X){ //RGB only
    
// #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//       pCONT_lAni->SEGMENT_I(0).rgbcct_controller->
// #else
//       rgbcct_controller.
// #endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
// setColorMode(LCM_RGB);
//   }else{
    
// #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//       pCONT_lAni->SEGMENT_I(0).rgbcct_controller->
// #else
//       rgbcct_controller.
// #endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
// setColorMode(LCM_BOTH);
//   }
  
// #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//       pCONT_lAni->SEGMENT_I(0).rgbcct_controller->
// #else
//       rgbcct_controller.
// #endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
// Sync();    // calculate the initial values (#8058)
//   // RGB parts
  
// #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//       pCONT_lAni->SEGMENT_I(0).rgbcct_controller->
// #else
//       rgbcct_controller.
// #endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
// setRGB(1,2,3);
//   // CCT parts
  
// #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//       pCONT_lAni->SEGMENT_I(0).rgbcct_controller->
// #else
//       rgbcct_controller.
// #endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
// setRGBCCTLinked(false);
  
// #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//       pCONT_lAni->SEGMENT_I(0).rgbcct_controller->
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

  pCONT_lAni->SEGMENT_I(0).flags.fEnable_Animation = true;

}




// Colours etc will be made or called using a func_update_light_Values
// Another func will push those values to the output, which each hardware class will handle

// // Used for timed on or off events
int8_t mInterfaceLight::Tasker(uint8_t function, JsonParserObject obj)
{
  
  int8_t function_result = 0;

  // As interface module, the parsing of module_init takes precedence over the Settings.light_settings.type
  switch(function){
    case FUNC_TEMPLATE_MODULE_LOAD_FROM_PROGMEM:{
      ALOG_DBM(PSTR("Disabled from conversion to vector segments, not sure if it is needed here"));
    };
    break;
    case FUNC_TEMPLATE_DEVICE_LOAD_FROM_PROGMEM:
    
    // #ifdef ENABLE_LOG_LEVEL_INFO
    // AddLog(LOG_LEVEL_TEST, PSTR("mInterfaceLight::Tasker\n\r\n\r\n\r\n\r"
    //     //Test data
    //     "%d:%d:%d:%d"
    //   ),
    //     //Testdata
    //      pCONT_set->Settings.animation_settings.xmas_controller_params[0]
    //     ,pCONT_set->Settings.animation_settings.xmas_controller_params[1]
    //     ,pCONT_set->Settings.animation_settings.xmas_controller_params[2]
    //     ,pCONT_set->Settings.animation_settings.xmas_controller_params[3]
    // );
    // #endif // ENABLE_LOG_LEVEL_INFO




      Template_Load();
    break;
    case FUNC_POINTER_INIT:

      // mpalette = new mPalette();

      #ifdef ENABLE_DEVFEATURE_REMOVE_INIT_OUTSIDE_OF_PALETTE_CLASS
      mPaletteI->init_PresetColourPalettes();
      #endif // ENABLE_DEVFEATURE_REMOVE_INIT_OUTSIDE_OF_PALETTE_CLASS

// #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

//       pCONT_lAni->SEGMENT_I(0).rgbcct_controller->setRgbcctColourOutputAddress(mPaletteI->palettelist.rgbcct_segment_colour_users[0].data);
//       // active_scene_palette_id = PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_01__ID;
//       // active_rgbcct_colour_p = reinterpret_cast<RgbcctColor*>(&pCONT_set->Settings.animation_settings.palette_rgbcct_user_colour_map_ids[0]); // use first for now
//       CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_01__ID);
// #else

//       rgbcct_controller.setRgbcctColourOutputAddress(mPaletteI->palettelist.rgbcct_segment_colour_users[0].data);
//       // active_scene_palette_id = PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_01__ID;
//       // active_rgbcct_colour_p = reinterpret_cast<RgbcctColor*>(&pCONT_set->Settings.animation_settings.palette_rgbcct_user_colour_map_ids[0]); // use first for now
//       CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(mPaletteI->PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_01__ID);
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
  pCONT_lAni->SEGMENT_I(0).animation_has_anim_callback = false; // When no animation callback is needed
  ShowInterface();

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
//   Serial.printf("%d,", mPaletteI->palettelist.rgbcct_segment_colour_users[0].data[i]);
// }
// Serial.println("rgbcct1");
// for(int i=0;i<5;i++)
// {
//   Serial.printf("%d,", mPaletteI->palettelist.rgbcct_segment_colour_users[1].data[i]);
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
    // case LIGHT_POWER_STATE_OFF_ID: //probably phase out for "SetPower" is generic event type
    // DEBUG_LINE_HERE;
    //   CommandSet_LightPowerState(0);
    // break;
    
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
    DEBUG_LINE_HERE;
  CommandSet_LightPowerState(state);

}
#endif // USE_MODULE_CORE_RULES



/**
 * This function, is the entry point for the output that is hardware dependent 
 * colour_hardware will already have colour_order mapped
 * */
void mInterfaceLight::SetPixelColourHardwareInterface(RgbcctColor colour, uint16_t index, bool flag_replicate_for_total_pixel_length){

  #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
   ALOG_INF( PSTR("SetPixelColor: %d\t(%d,%d,%d,%d,%d) pal%d"), index, colour.R, colour.G, colour.B, colour.W1, colour.W2, pCONT_lAni->segments[pCONT_lAni->_segment_index_primary].palette.id );
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
      return pCONT_lPWM->GetPixelColorHardware(index);
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
      return pCONT_lPWM->ShowHardware();
      #endif // USE_MODULE_LIGHTS_PWM
    break;
  }

} 



void mInterfaceLight::EveryLoop()
{

  ALOG_DBM(PSTR("mInterfaceLight::EveryLoop()"));
        
} // END everyloop




void mInterfaceLight::EverySecond_AutoOff(){

  //ALOG_DBM( PSTR(D_LOG_LIGHT "scene.auto_off_settings.tSaved [%d]"),animation.auto_off_settings.time_decounter_secs);
  if(auto_off_settings.time_decounter_secs==1){ //if =1 then turn off and clear to 0
    // animation.name_id = MODE_SINGLECOLOUR_FADE_OFF__ID;
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT "auto_off_settings.time_decounter_secs==1 and disable"));
    #endif       
    DEBUG_LINE_HERE;
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
 * Option in the future to apply different colours to the map.
 * 
 * Global temp colours reporting should have flag otpion to pick between full/adjusted brightness inside primary unified sensor.
 * I should still produce them all for the sub mqtt colours
 * 
 * @param value 
 * @param map_style_id 
 * @param value_min 
 * @param value_max 
 * @param map_is_palette_id 
 * @return RgbColor 
 */
RgbColor mInterfaceLight::GetColourValueUsingMaps_AdjustedBrightness(float value, 
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
RgbColor mInterfaceLight::GetColourValueUsingMaps_FullBrightness(float value, 
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





/******************************************************************************************************************
 * mInterfaceLight_Commands.cpp
*******************************************************************************************************************/


void mInterfaceLight::parse_JSONCommand(JsonParserObject obj)
{

  ALOG_DBM( PSTR(D_LOG_LIGHT D_TOPIC "mInterfaceLight Checking all commands %d"),obj.isNull());

  char buffer[50];
  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

  uint16_t isserviced_start_count = data_buffer.isserviced;
  
  /***
   * As order of importance, others that rely on previous commands must come after
   * */
  int val = 0;

  if(jtok = obj[PM_JSON_HARDWARE_TYPE]){
    if(jtok.isStr()){
      if((tmp_id=GetPixelHardwareTypeIDbyName(jtok.getStr()))>=0){
        CommandSet_PixelHardwareTypeID(tmp_id);
        data_buffer.isserviced++;
      }
    }else
    if(jtok.isNum()){
      CommandSet_PixelHardwareTypeID(jtok.getInt());
      data_buffer.isserviced++;
    }
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_HARDWARE_TYPE)), GetPixelHardwareTypeName(buffer));
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }
  
  // if(jtok = obj[PM_JSON_RGB]){ // Must accept "RRGGBBCC" and "RR,GG,BB,CC"
  //   CommandSet_ActiveSolidPalette_RGB_Ctr(jtok.getStr());
  //   data_buffer.isserviced++;
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_HUE)), getHue());
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }

  // #ifndef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  // if(jtok = obj[PM_JSON_HUE]){ // Assume range 0-359
  //   CommandSet_ActiveSolidPalette_Hue_360(jtok.getInt());
  //   data_buffer.isserviced++;
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_HUE)), getHue());
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }
  // #endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

  // if(jtok = obj[PM_JSON_SAT]){ // Assume range 0-100
  //   CommandSet_ActiveSolidPalette_Sat_255(mapvalue(jtok.getInt(), 0,100, 0,255));
  //   data_buffer.isserviced++;
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_SAT)), getSat());
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }else
  // if(jtok = obj[PM_JSON_SAT_255]){ // alternate full range 0-255
  //   CommandSet_ActiveSolidPalette_Sat_255(jtok.getInt());
  //   data_buffer.isserviced++;
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_SAT_255)), getSat());
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }

  if(jtok = obj[PM_JSON_BRIGHTNESS]){ // Assume range 0-100
    CommandSet_Brt_255(mapvalue(jtok.getInt(), 0,100, 0,255));
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS)), jtok.getInt());
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }else
  if(jtok = obj[PM_JSON_BRIGHTNESS_255]){ // alternate full range 0-255
    CommandSet_Brt_255(jtok.getInt());
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_255)), getBri());
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  if(jtok = obj[PM_JSON_BRIGHTNESS_RGB]){ // Assume range 0-100
    CommandSet_Global_BrtRGB_255(mapvalue(jtok.getInt(), 0,100, 0,255));
    ALOG_INF(PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_RGB)), getBriRGB_Global());
    data_buffer.isserviced++;
  }else
  if(jtok = obj[PM_JSON_BRIGHTNESS_RGB_255]){ // alternate full range 0-255
    CommandSet_Global_BrtRGB_255(jtok.getInt());
    ALOG_INF(PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_RGB_255)), getBriRGB_Global());
    data_buffer.isserviced++;
  }

  if(jtok = obj[PM_JSON_BRIGHTNESS_CCT]){ // Assume range 0-100
    CommandSet_Global_BrtCCT_255(mapvalue(jtok.getInt(), 0,100, 0,255));
    ALOG_INF(PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_CCT)), getBriCCT_Global());
    data_buffer.isserviced++;
  }else
  if(jtok = obj[PM_JSON_BRIGHTNESS_CCT_255]){ // alternate full range 0-255
    CommandSet_Global_BrtCCT_255(jtok.getInt());
    ALOG_INF(PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_CCT_255)), getBriCCT_Global());
    data_buffer.isserviced++;
  }


  #ifndef ENABLE_DEVFEATURE_INTERFACELIGHT_NEW_UNIQUE_TIMEON
  /**
   * @brief Construct a new if object
   * TimeOn is not unique here, so light prefix needs to be added
   * Maybe "LightTimeOn" or "Light:{TimeOn:x}"
   * 
   */
  if(jtok = obj[PM_JSON_TIME_ON]){ // default to secs
    CommandSet_Auto_Time_Off_Secs(jtok.getInt());
    ALOG_WRN(PSTR("ToBePhasedOut: \"TimeOn\"->\"Light:{\"TimeOn\":x}"));
    data_buffer.isserviced++;
  }
  #endif // ENABLE_DEVFEATURE_INTERFACELIGHT_NEW_UNIQUE_TIMEON


  // New unique version to phase in
  if(jtok = obj["Light"].getObject()[PM_JSON_TIME_ON]){ // default to secs
    CommandSet_Auto_Time_Off_Secs(jtok.getInt());
    data_buffer.isserviced++;
    ALOG_COM( PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_TIME_ON)),auto_off_settings.time_decounter_secs ); 
  }



  if(jtok = obj[PM_JSON_LIGHTPOWER]){
    int8_t state = 0;
    if(jtok.isStr()){
      state = pCONT_sup->GetStateNumber(jtok.getStr());
    }else
    if(jtok.isNum()){
      state = jtok.getInt(); 
    }
    ModifyStateNumberIfToggled(&state, light_power_state);
    DEBUG_LINE_HERE;
    CommandSet_LightPowerState(state);
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_LIGHTPOWER)), light_power_state);
    #endif // ENABLE_LOG_LEVEL_DEBUG       
  }
 
  if(jtok = obj[PM_JSON_PALETTE_EDIT].getObject()[PM_JSON_COLOUR_PALETTE]){

    // #ifdef ENABLE_LOG_LEVEL_INFO
    // AddLog(LOG_LEVEL_TEST, PSTR("Palette: PM_JSON_PALETTE_EDIT "));
    // #endif // ENABLE_LOG_LEVEL_INFO

    JsonParserToken jtok_data = obj[PM_JSON_PALETTE_EDIT].getObject()[PM_JSON_DATA];
    if(!jtok_data.isNull()){ 

      uint8_t jsonpair_count = jtok_data.size();
      uint8_t colour_array[jsonpair_count];
      memset(colour_array,0,sizeof(colour_array));

      for(int index = 0; index < jsonpair_count; index++){
        jtok_data.nextOne(); //skip start of object
        colour_array[index] = jtok_data.getInt();
      }
      
      CommandSet_PaletteColour_RGBCCT_Raw_By_ID(jtok.getInt(), colour_array, sizeof(colour_array));
      data_buffer.isserviced++;

      #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog_Array(LOG_LEVEL_DEBUG, "rgbcct", colour_array, sizeof(colour_array));
      #endif // ENABLE_LOG_LEVEL_DEBUG
    }
  }

  
  if(jtok = obj[PM_JSON_ANIMATIONENABLE]){ 
    int8_t state = 0;
    if(jtok.isStr()){
      state = pCONT_sup->GetStateNumber(jtok.getStr());
    }else
    if(jtok.isNum()){
      state = jtok.getInt(); 
    }
    
    // #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
    // ModifyStateNumberIfToggled(&state, animation.flags.fEnable_Animation);
    // #else
    // ModifyStateNumberIfToggled(&state, pCONT_lAni->SEGMENT_I(0).flags.fEnable_Animation);
    // #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT

    // CommandSet_EnabledAnimation_Flag(state);
    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_ANIMATIONENABLE)), pCONT_iLight->animation.flags.fEnable_Animation);    
    // #endif // ENABLE_LOG_LEVEL_DEBUG

  }

  
  /**
   * @brief Construct a new if object
   * 
   PixelRange replaces this, stripsize will instead be total length of max pixel
   * 
   */
  if(jtok = obj[PM_JSON_STRIP_SIZE]){
    CommandSet_LightSizeCount(jtok.getInt());
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_STRIP_SIZE)), settings.light_size_count);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  // if(jtok = obj[PM_JSON_SCENE].getObject()[PM_JSON_COLOUR]){

  //   uint8_t colour_array[5];
  //   memset(colour_array,0,sizeof(colour_array));
  //   uint8_t jsonpair_count = jtok.size();

  //   for(int index = 0; index < jsonpair_count; index++){
  //     jtok.nextOne(); //skip start of object
  //     Serial.println(jtok.getInt());
  //     colour_array[index] = jtok.getInt();
  //   }
    
  //   CommandSet_ActiveSolidPalette_Raw(colour_array);
  //   data_buffer.isserviced++;

  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   snprintf_P(buffer, sizeof(buffer), PSTR("[%d,%d,%d,%d,%d]"),colour_array[0],colour_array[1],colour_array[2],colour_array[3],colour_array[4]);
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_SVALUE_K(D_JSON_SCENE,D_JSON_COLOUR)), buffer);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }

  //  If command source was webui, then override changes
  if(data_buffer.flags.source_id == DATA_BUFFER_FLAG_SOURCE_WEBUI)
  {
    // pCONT_lAni->segment_animation_override.time_ms = 100;
  }
  
  if(isserviced_start_count != data_buffer.isserviced) //ie something was parsed inside this function
  {
    pCONT_lAni->SEGMENT_I(0).flags.fForceUpdate = true;
  }
}





/******************************************************************************************************************************
*******************************************************************************************************************************
****************** PixelHardwareType *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mInterfaceLight::CommandSet_PixelHardwareTypeID(uint8_t value){
  char buffer[20];
  pCONT_set->Settings.light_settings.type = value;
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_HARDWARE_TYPE)), GetPixelHardwareTypeName(buffer));
  #endif // ENABLE_LOG_LEVEL_COMMANDS
} 
int8_t mInterfaceLight::GetPixelHardwareTypeIDbyName(const char* c){
  if(*c=='\0') return -1;
  if(     strcmp_P(c,PM_PIXEL_HARDWARE_TYPE_RGBCCT_PWM_CTR)==0){ return LT_PWM5; }
  else if(strcmp_P(c,PM_PIXEL_HARDWARE_TYPE_WS28XX_CTR)==0){       return LT_ADDRESSABLE_WS281X; }
  else if(strcmp_P(c,PM_PIXEL_HARDWARE_TYPE_SK6812_CTR)==0){       return LT_ADDRESSABLE_SK6812; }
  return -1;
}
const char* mInterfaceLight::GetPixelHardwareTypeName(char* buffer){
  return GetPixelHardwareTypeNamebyID(pCONT_set->Settings.light_settings.type, buffer);
}
const char* mInterfaceLight::GetPixelHardwareTypeNamebyID(uint8_t id, char* buffer){
  switch(id){
    case LT_PWM5:   memcpy_P(buffer,PM_PIXEL_HARDWARE_TYPE_RGBCCT_PWM_CTR,sizeof(PM_PIXEL_HARDWARE_TYPE_RGBCCT_PWM_CTR)); break;
    case LT_ADDRESSABLE_WS281X: memcpy_P(buffer,PM_PIXEL_HARDWARE_TYPE_WS28XX_CTR,sizeof(PM_PIXEL_HARDWARE_TYPE_WS28XX_CTR));         break;
    case LT_ADDRESSABLE_SK6812: memcpy_P(buffer,PM_PIXEL_HARDWARE_TYPE_SK6812_CTR,sizeof(PM_PIXEL_HARDWARE_TYPE_SK6812_CTR));         break;
  }
  return buffer;
}

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** HardwareColourType (RGB) *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

// #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
// void mInterfaceLight::CommandSet_HardwareColourOrderTypeByStr(const char* c){
//   char buffer[60];
//   // settings.pixel_hardware_color_order_id = value; 

//   // return;
    
//   if(!c){ return; }
//   if(strlen(c)<=5){
//     ALOG_DBM( PSTR("Valid Length"));
//   }else{
//     AddLog(LOG_LEVEL_ERROR, PSTR("INVALID Length"));
//     return;
//   }


  
// #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT


//   mInterfaceLight::HARDWARE_ELEMENT_COLOUR_ORDER* order = &pCONT_iLight->hardware_element_colour_order;

//   order->red = D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE;
//   order->green = D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE;
//   order->blue = D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE;
//   order->white_cold = D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE;
//   order->white_warm = D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE;

//   for(uint8_t index=0;index<strlen(c);index++){
//     // if(c[index]==0){ break; }

//     if((c[index]=='R')||(c[index]=='r')){
//       order->r = index;
//     }else
//     if((c[index]=='G')||(c[index]=='g')){
//       order->g = index;
//     }else
//     if((c[index]=='B')||(c[index]=='b')){
//       order->b = index;
//     }else
//     if((c[index]=='C')||(c[index]=='c')){
//       order->c = index;
//     }else
//     if((c[index]=='W')||(c[index]=='w')){
//       order->w = index;
//     }

//   }

//   ALOG_DBM( PSTR("red=%d, green=%d, blue=%d, cold_white=%d, warm_white=%d"),
//     order->r,
//     order->g,
//     order->b,
//     order->c,
//     order->w
//   );

// #else

// pCONT_lAni->SEGMENT_I(0).hardware_element_colour_order.red = D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE;
//  pCONT_lAni-> SEGMENT_I(0).hardware_element_colour_order.b = D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE;
//  pCONT_lAni-> SEGMENT_I(0).hardware_element_colour_order.blue = D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE;
//  pCONT_lAni-> SEGMENT_I(0).hardware_element_colour_order.white_cold = D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE;
// pCONT_lAni->  SEGMENT_I(0).hardware_element_colour_order.white_warm = D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE;


//   for(uint8_t index=0;index<strlen(c);index++){
//     // if(c[index]==0){ break; }

//     if((c[index]=='R')||(c[index]=='r')){
//       pCONT_lAni->SEGMENT_I(0).hardware_element_colour_order.r = index;
//     }else
//     if((c[index]=='G')||(c[index]=='g')){
//      pCONT_lAni-> SEGMENT_I(0).hardware_element_colour_order.g = index;
//     }else
//     if((c[index]=='B')||(c[index]=='b')){
//      pCONT_lAni-> SEGMENT_I(0).hardware_element_colour_order.b = index;
//     }else
//     if((c[index]=='C')||(c[index]=='c')){
//      pCONT_lAni-> SEGMENT_I(0).hardware_element_colour_order.c = index;
//     }else
//     if((c[index]=='W')||(c[index]=='w')){
//     pCONT_lAni->  SEGMENT_I(0).hardware_element_colour_order.w = index;
//     }

//   }

//   ALOG_DBM( PSTR("red=%d, green=%d, blue=%d, cold_white=%d, warm_white=%d"),
//    pCONT_lAni-> SEGMENT_I(0).hardware_element_colour_order.r,
//    pCONT_lAni-> SEGMENT_I(0).hardware_element_colour_order.g,
//    pCONT_lAni-> SEGMENT_I(0).hardware_element_colour_order.b,
//    pCONT_lAni-> SEGMENT_I(0).hardware_element_colour_order.c,
//   pCONT_lAni->  SEGMENT_I(0).hardware_element_colour_order.w
//   );








// #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT






//   //tmp fix, copy, flip 1 and 2

//   // memcpy(&pCONT_iLight->hardware_element_colour_order[1],&pCONT_iLight->hardware_element_colour_order[0],sizeof(mInterfaceLight::HARDWARE_ELEMENT_COLOUR_ORDER));

  
//   // pCONT_iLight->hardware_element_colour_order[1].r = pCONT_iLight->hardware_element_colour_order[0].g;
//   // pCONT_iLight->hardware_element_colour_order[1].g = pCONT_iLight->hardware_element_colour_order[0].r;

//   // #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_RGB_COLOUR_ORDER)), GetHardwareColourTypeName(buffer, sizeof(buffer)));
//   // #endif



// }





// const char* mInterfaceLight::GetHardwareColourTypeName(char* buffer, uint8_t buflen){

  
// #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
//   return GetHardwareColourTypeNameByID(animation.mode_id, buffer, buflen);
// #else
//   return GetHardwareColourTypeNameByID(pCONT_lAni->SEGMENT_I(0).mode_id, buffer, buflen);
// #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT




// }
// const char* mInterfaceLight::GetHardwareColourTypeNameByID(uint8_t id, char* buffer, uint8_t buflen){
//   sprintf(buffer, PM_SEARCH_NOMATCH);
//   // switch(id){
//   //   default:
//   //   case PIXEL_HARDWARE_COLOR_ORDER_GRB_ID: memcpy_P(buffer, PM_PIXEL_HARDWARE_COLOR_ORDER_GRB_CTR, sizeof(PM_PIXEL_HARDWARE_COLOR_ORDER_GRB_CTR)); break;
//   //   case PIXEL_HARDWARE_COLOR_ORDER_RGB_ID: memcpy_P(buffer, PM_PIXEL_HARDWARE_COLOR_ORDER_RGB_CTR, sizeof(PM_PIXEL_HARDWARE_COLOR_ORDER_RGB_CTR)); break;
//   //   case PIXEL_HARDWARE_COLOR_ORDER_BRG_ID: memcpy_P(buffer, PM_PIXEL_HARDWARE_COLOR_ORDER_BRG_CTR, sizeof(PM_PIXEL_HARDWARE_COLOR_ORDER_BRG_CTR)); break;
//   //   case PIXEL_HARDWARE_COLOR_ORDER_RBG_ID: memcpy_P(buffer, PM_PIXEL_HARDWARE_COLOR_ORDER_RBG_CTR, sizeof(PM_PIXEL_HARDWARE_COLOR_ORDER_RBG_CTR)); break;
//   // }
//   return buffer;
// }
// // uint16_t mInterfaceLight::GetHardwareColourTypeIDbyName(const char* c){
// //   // if(!c){ return -1; }
// //   //3 PIXEL TYPE
// //   // if(     strcmp_P(c,PM_PIXEL_HARDWARE_COLOR_ORDER_GRB_CTR)==0){ return PIXEL_HARDWARE_COLOR_ORDER_GRB_ID; }
// //   // else if(strcmp_P(c,PM_PIXEL_HARDWARE_COLOR_ORDER_RGB_CTR)==0){ return PIXEL_HARDWARE_COLOR_ORDER_RGB_ID; }
// //   // else if(strcmp_P(c,PM_PIXEL_HARDWARE_COLOR_ORDER_BRG_CTR)==0){ return PIXEL_HARDWARE_COLOR_ORDER_BRG_ID; }
// //   // else if(strcmp_P(c,PM_PIXEL_HARDWARE_COLOR_ORDER_RBG_CTR)==0){ return PIXEL_HARDWARE_COLOR_ORDER_RBG_ID; }   

// //   // if(!c){ return -1; }
// //   // if(strlen(c)<=5){
// //   //   AddLog(LOG_LEVEL_TEST, PSTR("Valid Length"));
// //   // }else{
// //   //   AddLog(LOG_LEVEL_TEST, PSTR("INVALID Length"));
// //   //   return -1;
// //   // }

// //   // mInterfaceLight::HARDWARE_ELEMENT_COLOUR_ORDER* order = &pCONT_iLight->hardware_element_colour_order[0];

// //   // order->red = D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE;
// //   // order->green = D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE;
// //   // order->blue = D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE;
// //   // order->white_cold = D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE;
// //   // order->white_warm = D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE;

// //   // for(uint8_t index=0;index<strlen(c);index++){
// //   //   if(c[index]==0){ break; }

// //   //   if((c[index]=='R')||(c[index]=='r')){
// //   //     order->r = index;
// //   //   }else
// //   //   if((c[index]=='G')||(c[index]=='g')){
// //   //     order->g = index;
// //   //   }else
// //   //   if((c[index]=='B')||(c[index]=='b')){
// //   //     order->b = index;
// //   //   }else
// //   //   if((c[index]=='C')||(c[index]=='c')){
// //   //     order->c = index;
// //   //   }else
// //   //   if((c[index]=='W')||(c[index]=='w')){
// //   //     order->w = index;
// //   //   }

// //   // }

// //   // AddLog(LOG_LEVEL_TEST, PSTR("red=%d, green=%d, blue=%d, cold_white=%d, warm_white=%d"),
// //   //   order->r,
// //   //   order->g,
// //   //   order->b,
// //   //   order->c,
// //   //   order->w
// //   // );

// //   // //tmp fix, copy, flip 1 and 2

// //   // memcpy(&pCONT_iLight->hardware_element_colour_order[1],&pCONT_iLight->hardware_element_colour_order[0],sizeof(mInterfaceLight::HARDWARE_ELEMENT_COLOUR_ORDER));

  
// //   // pCONT_iLight->hardware_element_colour_order[1].r = pCONT_iLight->hardware_element_colour_order[0].g;
// //   // pCONT_iLight->hardware_element_colour_order[1].g = pCONT_iLight->hardware_element_colour_order[0].r;



// //   // return -1;
// // }

// /**
//  * Fall back to default if invalid 
//  **/
// void mInterfaceLight::CheckHardwareElementColourOrder(){
  
//   // mInterfaceLight::HARDWARE_ELEMENT_COLOUR_ORDER* order = &pCONT_iLight->hardware_element_colour_order[0];

// }

// #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT


// /******************************************************************************************************************************
// *******************************************************************************************************************************
// ****************** HardwareColourType2 (White)*****************************************************************************************
// *******************************************************************************************************************************
// *******************************************************************************************************************************/




// void mInterfaceLight::CommandSet_HardwareColourOrderTypeID2(uint8_t value){
//   // char buffer[60];
//   // settings.pixel_hardware_color_order_id = value; 
//   // #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_RGB_COLOUR_ORDER)), GetHardwareColourTypeName(buffer, sizeof(buffer)));
//   // #endif

  

//   // order->red = 



// }
// const char* mInterfaceLight::GetHardwareColourTypeName2(char* buffer, uint8_t buflen){
//   return GetHardwareColourTypeNameByID2(pCONT_iLight->animation.mode_id, buffer, buflen);
// }
// const char* mInterfaceLight::GetHardwareColourTypeNameByID2(uint8_t id, char* buffer, uint8_t buflen){




//   // switch(id){
//   //   default:
//   //   case PIXEL_HARDWARE_COLOR_ORDER_GRB_ID: memcpy_P(buffer, PM_PIXEL_HARDWARE_COLOR_ORDER_GRB_CTR, sizeof(PM_PIXEL_HARDWARE_COLOR_ORDER_GRB_CTR)); break;
//   //   case PIXEL_HARDWARE_COLOR_ORDER_RGB_ID: memcpy_P(buffer, PM_PIXEL_HARDWARE_COLOR_ORDER_RGB_CTR, sizeof(PM_PIXEL_HARDWARE_COLOR_ORDER_RGB_CTR)); break;
//   //   case PIXEL_HARDWARE_COLOR_ORDER_BRG_ID: memcpy_P(buffer, PM_PIXEL_HARDWARE_COLOR_ORDER_BRG_CTR, sizeof(PM_PIXEL_HARDWARE_COLOR_ORDER_BRG_CTR)); break;
//   //   case PIXEL_HARDWARE_COLOR_ORDER_RBG_ID: memcpy_P(buffer, PM_PIXEL_HARDWARE_COLOR_ORDER_RBG_CTR, sizeof(PM_PIXEL_HARDWARE_COLOR_ORDER_RBG_CTR)); break;
//   // }
//   return buffer;
// }
// int8_t mInterfaceLight::GetHardwareColourTypeIDbyName2(const char* c){
//   if(!c){ return -1; }
//   if(strlen(c)<=5){
//     AddLog(LOG_LEVEL_TEST, PSTR("Valid Length"));
//   }else{
//     AddLog(LOG_LEVEL_TEST, PSTR("INVALID Length"));
//     return -1;
//   }

//   mInterfaceLight::HARDWARE_ELEMENT_COLOUR_ORDER* order = &pCONT_iLight->hardware_element_colour_order[0];

//   order->red = D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE;
//   order->green = D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE;
//   order->blue = D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE;
//   order->white_cold = D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE;
//   order->white_warm = D_HARDWARE_ELEMENT_COLOUR_ORDER_DISABLED_STATE;

//   for(uint8_t index=0;index<strlen(c);index++){
//     if(c[index]==0){ break; }

//     if((c[index]=='R')||(c[index]=='r')){
//       order->r = index;
//     }else
//     if((c[index]=='G')||(c[index]=='g')){
//       order->g = index;
//     }else
//     if((c[index]=='B')||(c[index]=='b')){
//       order->b = index;
//     }else
//     if((c[index]=='C')||(c[index]=='c')){
//       order->c = index;
//     }else
//     if((c[index]=='W')||(c[index]=='w')){
//       order->w = index;
//     }

//   }

//   AddLog(LOG_LEVEL_TEST, PSTR("red=%d, green=%d, blue=%d, cold_white=%d, warm_white=%d"),
//     order->r,
//     order->g,
//     order->b,
//     order->c,
//     order->w
//   );

//   //tmp fix, copy, flip 1 and 2

//   memcpy(&pCONT_iLight->hardware_element_colour_order[1],&pCONT_iLight->hardware_element_colour_order[0],sizeof(mInterfaceLight::HARDWARE_ELEMENT_COLOUR_ORDER));

  
//   pCONT_iLight->hardware_element_colour_order[1].r = pCONT_iLight->hardware_element_colour_order[0].g;
//   pCONT_iLight->hardware_element_colour_order[1].g = pCONT_iLight->hardware_element_colour_order[0].r;



//   return -1;
// }

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** LightPowerState *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mInterfaceLight::CommandSet_LightPowerState(uint8_t state){

  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_LIGHTPOWER)), light_power_state);
  #endif   

  if(state == LIGHT_POWER_STATE_OFF_ID) // turn off
  {
    // pCONT_lAni->CommandSet_Animation_Transition_Rate_Ms(10000);
    pCONT_lAni->CommandSet_LightsCountToUpdateAsPercentage(100);
    
    pCONT_lAni->SEGMENT_I(0).single_animation_override.time_ms =  pCONT_lAni->SEGMENT_I(0).single_animation_override_turning_off.time_ms; // slow turn on

ALOG_INF(PSTR("Setting override for off %d"), pCONT_lAni->SEGMENT_I(0).single_animation_override.time_ms);

    pCONT_lAni->SEGMENT_I(0).flags.fForceUpdate = true;

    CommandSet_Brt_255(0);
    
  }
  else
  if(state == 1) // turn on
  {

    // pCONT_lAni->CommandSet_Animation_Transition_Time_Ms(1000);

    pCONT_lAni->SEGMENT_I(0).single_animation_override.time_ms = 1000; // slow turn on
    pCONT_lAni->SEGMENT_I(0).flags.fForceUpdate = true;


    // pCONT_lAni->CommandSet_Animation_Transition_Rate_Ms(1000);
    // pCONT_lAni->CommandSet_LightsCountToUpdateAsPercentage(100);
    
    CommandSet_Brt_255(255);

    //make sure both are set
    // CommandSet_Global_BrtRGB_255(255);
    // CommandSet_Global_BrtCCT_255(255);
    
    // pCONT_lAni->CommandSet_PaletteID(10, 0);
    
    // pCONT_lAni->CommandSet_Flasher_FunctionID(0 /**Add define later for "DEFAULT_EFFECT" */);//pCONT_lAni->EFFECTS_FUNCTION__SOLID_COLOUR__ID);



  }

}


bool mInterfaceLight::CommandGet_LightPowerState()
{
  return 
  light_power_state
  // getBri_Global() 
  ? true : false;
}


  

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** RGB *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

// /**
//  * @param rgb Accepts both "RRGGBBWW" and "RR,GG,BB,WW", use of "#" will also be checked and omitted
//  * */
// void mInterfaceLight::CommandSet_ActiveSolidPalette_RGB_Ctr(const char* rgb)
// {

//   uint32_t colour32bit = 0;
//   bool hash_used = false;
//   bool delimeter_used = false;
//   if(strstr(rgb,"#")!=NULL){ hash_used = true; }
//   if(strstr(rgb,",")!=NULL){ delimeter_used = true; }

//   // if(delimeter_used)
//   // {
//     // to be done, with iterator moving across for rgb and rgbw can be parsed
//   // }

//   if(hash_used)
//   {
//     colour32bit = (long) strtol( &rgb[1], NULL, 16);
//   }
//   else
//   {
//     colour32bit = (long) strtol( &rgb[0], NULL, 16);
//   }

//   RgbwColor colour = RgbwColor(0);
//   // Check if upper 8 bits are set
//   if(colour32bit&0xFF000000)
//   { //4 colours
//     colour.R = colour32bit >> 24; //RGB
//     colour.G = colour32bit >> 16; //RGB
//     colour.B = colour32bit >> 8 & 0xFF; //RGB
//     colour.W = colour32bit & 0xFF; //RGB
//   }else{
//     colour.R = colour32bit >> 16; //RGB
//     colour.G = colour32bit >> 8 & 0xFF; //RGB
//     colour.B = colour32bit & 0xFF; //RGB
//   }

 
//   // Serial.println("HER"); Serial.flush();
  
  
//   #ifndef ENABLE_DEVFEATURE_REMOVE_RGBCCT_CONTROLLER
//   pCONT_lAni->SEGMENT_I(0).rgbcct_controller->setRGB(colour.R, colour.G, colour.B);
//   #endif

  
//   pCONT_lAni->SEGMENT_I(0).flags.fForceUpdate = true;
  

//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_HUE)), rgbcct_controller.getrgb());
//   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT "%d  %d,%d,%d,%d"), colour32bit, colour.R, colour.G, colour.B, colour.W);
//   #endif // ENABLE_LOG_LEVEL_COMMANDS

  

// // //   if(!obj[D_JSON_RGB].isNull()){
// // //     const char* rgbpacked = obj[D_JSON_RGB];
// // //     uint32_t colour32bit = 0;
// // //     RgbColor rgb;
// // //     // #ifndef ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT
// // //     // scene.colour = HsbColor(RgbColor(rgb.R,rgb.G,rgb.B));
// // //     // // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_RGB ":%s " D_NEOPIXEL_RGB ":%d,%d,%d " D_NEOPIXEL_HSB ":%d,%d,%d"),
// // //     // //   rgbpacked,rgb.R,rgb.G,rgb.B,scene.colour.H,scene.colour.S,scene.colour.B);
// // //     // #endif //ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT
// // //   }




// }



void mInterfaceLight::CommandSet_Brt_255(uint8_t brt_new){
    
  // pCONT_lAni->SEGMENT_I(0).rgbcct_controller->setBrightness255(brt_new);
  pCONT_lAni->SEGMENT_I(0).flags.fForceUpdate = true;
  setBriRGB_Global(brt_new);
  // probably needs to check if they are linked here, or internally
  setBriCT_Global(brt_new);

  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS)), pCONT_lAni->SEGMENT_I(0).rgbcct_controller->getBrightness255());
  // #endif // ENABLE_LOG_LEVEL_COMMANDS
}

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** BrtRGB *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mInterfaceLight::CommandSet_Global_BrtRGB_255(uint8_t bri) 
{  
  setBriRGB_Global(bri);
}


void mInterfaceLight::CommandSet_Global_BrtCCT_255(uint8_t bri) 
{  
  setBriCT_Global(bri);
}





/******************************************************************************************************************************
*******************************************************************************************************************************
****************** EnabledAnimation_Flag *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mInterfaceLight::CommandSet_EnabledAnimation_Flag(uint8_t value){

  pCONT_lAni->SEGMENT_I(0).flags.fEnable_Animation = value;

  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_ANIMATIONENABLE)), pCONT_lAni->SEGMENT_I(0).flags.fEnable_Animation);    
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}


// #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
// /******************************************************************************************************************************
// *******************************************************************************************************************************
// ****************** Animation_Transition_Time_Ms *****************************************************************************************
// *******************************************************************************************************************************
// *******************************************************************************************************************************/

// void mInterfaceLight::CommandSet_Animation_Transition_Time_Ms(uint16_t value){
    
  

  
// #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
//   animation.transition.time_ms = value;
// #else
//   pCONT_lAni->SEGMENT_I(0).transition.time_ms = value;
// #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT




//   // #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_TIME_MS)),animation.transition.time_ms);  
//   // #endif

// }

// /******************************************************************************************************************************
// *******************************************************************************************************************************
// ****************** Animation_Transition_Rate_Ms *****************************************************************************************
// *******************************************************************************************************************************
// *******************************************************************************************************************************/

// void mInterfaceLight::CommandSet_Animation_Transition_Rate_Ms(uint16_t value){
    
  
// #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
//   animation.transition.rate_ms = value;
  
//   if(animation.transition.rate_ms<animation.transition.time_ms){ 
//     animation.transition.time_ms = animation.transition.rate_ms;
//   }

// #else
//   pCONT_lAni->SEGMENT_I(0).transition.rate_ms = value;

  
//   if(pCONT_lAni->SEGMENT_I(0).transition.rate_ms<pCONT_lAni->SEGMENT_I(0).transition.time_ms){ 
//     pCONT_lAni->SEGMENT_I(0).transition.time_ms = pCONT_lAni->SEGMENT_I(0).transition.rate_ms;
//   }



// #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT



//   // #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_RATE_MS)),animation.transition.time_ms);  
//   // #endif

// }


// #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT


/******************************************************************************************************************************
*******************************************************************************************************************************
****************** TransitionMethodName *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

// const char* mInterfaceLight::GetTransitionMethodName(char* buffer, uint8_t buflen){
//   return GetTransitionMethodNameByID(pCONT_iLight->animation.transition.method_id, buffer, buflen);
// }
// const char* mInterfaceLight::GetTransitionMethodNameByID(uint8_t id, char* buffer, uint8_t buflen){
//   switch(id){ default:   
//     case TRANSITION_METHOD_NONE_ID: memcpy_P(buffer,PM_TRANSITION_METHOD_NONE_NAME_CTR,sizeof(PM_TRANSITION_METHOD_NONE_NAME_CTR)); break; // smooth shift between them
//     case TRANSITION_METHOD_BLEND_ID: memcpy_P(buffer,PM_TRANSITION_METHOD_BLEND_NAME_CTR,sizeof(PM_TRANSITION_METHOD_BLEND_NAME_CTR)); break; 
//     case TRANSITION_METHOD_INSTANT_ID: memcpy_P(buffer,PM_TRANSITION_METHOD_INSTANT_NAME_CTR,sizeof(PM_TRANSITION_METHOD_INSTANT_NAME_CTR)); break;  // smooth shift between them
//     case TRANSITION_METHOD_TWINKLE_ID: memcpy_P(buffer,PM_TRANSITION_METHOD_TWINKLE_NAME_CTR,sizeof(PM_TRANSITION_METHOD_TWINKLE_NAME_CTR));  break; // smooth shift between them
//     case TRANSITION_METHOD_GLIMMER_ID: memcpy_P(buffer,PM_TRANSITION_METHOD_GLIMMER_NAME_CTR,sizeof(PM_TRANSITION_METHOD_GLIMMER_NAME_CTR));  break; // smooth shift between them
//   }
//   return buffer;
// }
// int8_t mInterfaceLight::GetTransitionMethodIDbyName(const char* c){
//   if(*c=='\0'){
//     return -1;
//   }
//   // if(strstr(c,D_TRANSITION_METHOD_BLEND_NAME_CTR)){
//   //   return TRANSITION_METHOD_BLEND_ID;
//   // }else if(strstr(c,D_TRANSITION_METHOD_INSTANT_NAME_CTR)){
//   //   return TRANSITION_METHOD_INSTANT_ID;
//   // }else if(strstr(c,D_TRANSITION_METHOD_TWINKLE_NAME_CTR)){
//   //   return TRANSITION_METHOD_TWINKLE_ID;
//   // }else if(strstr(c,D_TRANSITION_METHOD_GLIMMER_NAME_CTR)){
//   //   return TRANSITION_METHOD_GLIMMER_ID;
//   // }
//   else{
//     return -1;
//   }
// }

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** Auto_Time_Off_Secs *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mInterfaceLight::CommandSet_Auto_Time_Off_Secs(uint16_t value){
    
  auto_off_settings.time_decounter_secs = value;

}

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** LightSizeCount *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mInterfaceLight::CommandSet_LightSizeCount(uint16_t value){

  settings.light_size_count = value;
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_STRIP_SIZE)), settings.light_size_count);
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}


// #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT

// /******************************************************************************************************************************
// *******************************************************************************************************************************
// ****************** LightsCountToUpdateAsNumber DUPLICATED EFFECT 1/2*****************************************************************************************
// *******************************************************************************************************************************
// *******************************************************************************************************************************/

// void mInterfaceLight::CommandSet_LightsCountToUpdateAsNumber(uint16_t value){
  
  
// #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
//   animation
// #else
//   pCONT_lAni->_segments[0]
// #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
// .transition.pixels_to_update_as_number = value;
//   // animation.transition.pixels_to_update_as_percentage.val = GetPixelsToUpdateAsPercentageFromNumber(value);

//   // Tmp fix until I merge them - UNTESTED HOW THIS WILL AFFECT CODE
//   // pCONT_lAni->strip_size_requiring_update = animation.transition.pixels_to_update_as_number ;
//   // AddLog(LOG_LEVEL_WARN, PSTR("pCONT_lAni->strip_size_requiring_update = animation.transition.pixels_to_update_as_number ; COULD CAUSE ERROR "));
//   // SetLEDOutAmountByPercentage(animation.transition.pixels_to_update_as_number)

//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_NUMBER)), value);
//   #endif

// }
// // /**
// //  * Duplicate parameter, needs merging with above in long term if it is really the same
// //  * */
// // uint16_t mAnimatorLight::SetLEDOutAmountByPercentage(uint8_t percentage){

// //   strip_size_requiring_update = mapvalue(percentage, 0,100, 0,pCONT_iLight->settings.light_size_count);

// // // AddLog(LOG_LEVEL_TEST, PSTR(DEBUG_INSERT_PAGE_BREAK "SetLEDOutAmountByPercentage = %d"),strip_size_requiring_update);

// //   return strip_size_requiring_update; // also return the count

// // }


// /******************************************************************************************************************************
// *******************************************************************************************************************************
// ****************** LightsCountToUpdateAsPercentage DUPLICATED EFFECT 2/2*****************************************************************************************
// *******************************************************************************************************************************
// *******************************************************************************************************************************/

// void mInterfaceLight::CommandSet_LightsCountToUpdateAsPercentage(uint8_t value){

  
//   pCONT_lAni->SEGMENT_I(0).transition.pixels_to_update_as_number = GetPixelsToUpdateAsPercentageFromNumber(value);
//   // animation.transition.pixels_to_update_as_percentage = value;
  
//   // TEMP FIX!! - UNTESTED HOW THIS WILL AFFECT CODE
//   // AddLog(LOG_LEVEL_WARN, PSTR("pCONT_lAni->strip_size_requiring_update = animation.transition.pixels_to_update_as_number COULD CAUSE ERROR "));
//   // pCONT_lAni->strip_size_requiring_update = animation.transition.pixels_to_update_as_number ;


//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE)), value);
//   #endif

// }

// uint16_t mInterfaceLight::GetPixelsToUpdateAsNumberFromPercentage(uint8_t percentage){
//   return mapvalue(percentage, 0,100, 0,pCONT_iLight->settings.light_size_count);
// }
// uint8_t mInterfaceLight::GetPixelsToUpdateAsPercentageFromNumber(uint16_t number){
//   return mapvalue(number ,0,pCONT_iLight->settings.light_size_count, 0,100);
// }

// #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT




// #ifndef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
// /******************************************************************************************************************************
// *******************************************************************************************************************************
// ****************** ActiveRgbcctColourPaletteIDUsedAsScene *****************************************************************************************
// *******************************************************************************************************************************
// *******************************************************************************************************************************/

// void mInterfaceLight::CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(uint8_t palette_id){

//   uint8_t palette_id_adjusted_to_array_index = palette_id - mPaletteI->PALETTELIST_VARIABLE_HSBID_LENGTH_ID;  
  
//   //mPaletteI->active_scene_palette_id = palette_id;
//   #ifndef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//   active_rgbcct_colour_p = reinterpret_cast<RgbcctColor*>(&pCONT_set->Settings.animation_settings.palette_rgbcct_users_colour_map[5*palette_id_adjusted_to_array_index]); // use first for now
//   #endif //  ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS



//   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT "CommandSet_ActiveRgbcctColourPalette(%d) as %d"),palette_id,palette_id_adjusted_to_array_index);
// }

// // #endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** PaletteID *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

// void mInterfaceLight::CommandSet_PaletteID(uint8_t value){

//   char buffer[50];
  
//   pCONT_lAni->SEGMENT_I(0).palette.id = value < mPalette::PALETTELIST_STATIC_LENGTH_ID ? value : 0;

//   //If "id" is in the range of rgbcct, make sure to automatically make internal_rgbctt track it
//   if((value>=mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_01_ID)&&(value<mPaletteI->PALETTELIST_VARIABLE_RGBCCT_LENGTH_ID)){
//     CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(value);
//   }

//   // #ifdef ENABLE_PIXEL_AUTOMATION_PLAYLIST
//   // if(animation.mode_id == ANIMATION_MODE__EFFECTS){
//   //     pCONT_ladd->flashersettings.region = pCONT_ladd->EFFECTS_REGION_COLOUR_SELECT_ID; //update colours in use
//   // }
//   // #endif
//   #ifdef ENABLE_PALETTE_FORCED_MODE
//   //  animation.mode_id = ANIMATION_MODE_PRESETS_ID;
//   #endif
  
//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_COLOUR_PALETTE)), pCONT_lAni->SEGMENT_I(0).palette.id);
//   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), mPaletteI->GetPaletteNameByID(pCONT_lAni->SEGMENT_I(0).palette.id, buffer, sizeof(buffer)));
//   #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS

// }

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** PaletteColour_RGBCCT_Raw *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mInterfaceLight::CommandSet_PaletteColour_RGBCCT_Raw_By_ID(uint8_t palette_id, uint8_t* buffer, uint8_t buflen){

      // AddLog(LOG_LEVEL_TEST, PSTR("START[%d|%d] fMapIDs_Type=%d"),
      // palette_id,mPaletteI->PALETTELIST_VARIABLE_HSBID_LENGTH_ID,

  // Serial.flush();
  // delay(3000);

  // Check if palette can be edited
  if(!mPaletteI->CheckPaletteByIDIsEditable(palette_id)){ 
    
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_TEST, PSTR("Palette can not be edited"));
    #endif // ENABLE_LOG_LEVEL_INFO
      return; 
  }

  // Convert id into array index if needed
  uint8_t palette_id_adjusted_to_array_index = 0;
  // Get starting position of buffer
  uint8_t* palette_buffer = nullptr;

  if(palette_id<mPaletteI->PALETTELIST_VARIABLE_HSBID_LENGTH__ID){
    palette_id_adjusted_to_array_index = palette_id;
    palette_buffer = &pCONT_set->Settings.animation_settings.palette_hsbid_users_colour_map[(mPaletteI->PALETTELIST_VARIABLE_HSBID_LENGTH__ID-mPaletteI->PALETTELIST_VARIABLE_HSBID_01__ID)*palette_id_adjusted_to_array_index];
    // Clear the entire new colour to the "unset" values
    memset(palette_buffer,COLOUR_MAP_NONE__ID,20); // change COLOUR_MAP_NONE_ID to be 0 going forward, and as "black", although considered unset
    
    // Add to select correct buffer depending on palette type
    memcpy(palette_buffer,buffer,buflen);

    // rgbcct_controller.UpdateFromExternalBuffer();
    pCONT_lAni->SEGMENT_I(0).flags.fForceUpdate = true;

  }else
  if((palette_id>=mPaletteI->PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_01__ID)&&(palette_id<mPaletteI->PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID)){
 

    palette_id_adjusted_to_array_index = palette_id - mPaletteI->PALETTELIST_VARIABLE_HSBID_LENGTH__ID;    
    palette_buffer = &pCONT_set->Settings.animation_settings.palette_rgbcct_users_colour_map[(mPaletteI->PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID-mPaletteI->PALETTELIST_VARIABLE__RGBCCT_SEGMENT_COLOUR_01__ID)*palette_id_adjusted_to_array_index];
    memset(palette_buffer,0,5); // change COLOUR_MAP_NONE_ID to be 0 going forward, and as "black", although considered unset

    // Add to select correct buffer depending on palette type
    memcpy(palette_buffer,buffer,buflen);

    // pCONT_lAni->SEGMENT_I(0).rgbcct_controller->UpdateFromExternalBuffer();
    pCONT_lAni->SEGMENT_I(0).flags.fForceUpdate = true;

  }else
  if((palette_id>=mPaletteI->PALETTELIST_VARIABLE_GENERIC_01__ID)&&(palette_id<mPaletteI->PALETTELIST_VARIABLE_GENERIC_LENGTH__ID)){
    
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_TEST, PSTR("Palette: Generic \"%d\"\n\r"),palette_id);
    #endif // ENABLE_LOG_LEVEL_INFO

    palette_id_adjusted_to_array_index = palette_id - mPaletteI->PALETTELIST_VARIABLE_GENERIC_LENGTH__ID;    
    palette_buffer = pCONT_set->Settings.animation_settings.palette_encoded_users_colour_map;

    // AddLog(LOG_LEVEL_TEST, PSTR("Buffer len %d"),buflen);
    
    // pCONT_set->Settings.animation_settings.palette_rgbcct_users_colour_map[(mPaletteI->PALETTELIST_VARIABLE_GENERIC_LENGTH_ID-mPaletteI->PALETTELIST_VARIABLE_GENERIC_01_ID)*palette_id_adjusted_to_array_index];
    
    // Clear old buffer space
    memset(palette_buffer, 0, palette_encoded_users_colour_map_LENGTH); //200 now   
    // Write new palette data into buffer space
    memcpy(palette_buffer,buffer,buflen);
    // Parse buffer data to correctly set data parameters
    mPaletteI->init_PresetColourPalettes_User_Generic_Fill(0);

    // DEBUG_LINE_HERE;


  }


  // Update new length stored in palettelist
  mPaletteI->GetColourMapSizeByPaletteID(palette_id);

  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // AddLog_Array(LOG_LEVEL_COMMANDS, "changed colour", palette_buffer, buflen);
  // AddLog_Array(LOG_LEVEL_COMMANDS, "hsbid map", pCONT_set->Settings.animation_settings.palette_hsbid_users_colour_map, 20*2);
  // AddLog_Array(LOG_LEVEL_COMMANDS, "rgbcct map", pCONT_set->Settings.animation_settings.palette_rgbcct_users_colour_map, 5*5);
  // AddLog_Array(LOG_LEVEL_COMMANDS, "encoded map", pCONT_set->Settings.animation_settings.palette_encoded_users_colour_map, 25);
  // AddLog_Array(LOG_LEVEL_COMMANDS, "colour id map", mPaletteI->palettelist.ptr->data, 25);
  // AddLog(LOG_LEVEL_COMMANDS, "colour map size %d", mPaletteI->palettelist.ptr->data_length);
  // #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS

  // Serial.flush();
  // delay(3000);

 

}





/******************************************************************************************************************
 * mInterfaceLight_Web.cpp
*******************************************************************************************************************/



#ifdef USE_MODULE_NETWORK_WEBSERVER

int8_t mInterfaceLight::Tasker_Web(uint8_t function){



  switch(function){  
    
    case FUNC_WEB_ADD_HANDLER:
      WebPage_Root_AddHandlers();
    break;

    case FUNC_WEB_ADD_ROOT_MODULE_TABLE_CONTAINER:
      // #ifdef USE_MODULE_LIGHTS_ANIMATOR
      // pCONT_ladd->
      WebAppend_Root_Draw_Table();
    
    //  WebAppend_Root_Draw_RGBLive();
      // #endif
      
    break; 
    case FUNC_WEB_APPEND_RUNTIME_ROOT_URLS:
      JsonBuilderI->Add(WEB_HANDLE_LIVEPIXELS_SHARED_JSON,liveview.refresh_rate);
      JsonBuilderI->Add("/draw/palette_selector.json",-500);
    break;
    
    case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:

      //if(pCONT_iLight->animation.mode_id == pCONT_iLight->ANIMATION_MODE__EFFECTS){
        WebAppend_Root_RGBPalette();
      //}
      WebAppend_Root_Sliders();  //move webui stuff into here, as "scenes" will soon be replaced by colour[5].. make it a struct, but read/write using bytes when need by pointer of struct

    break;
    case FUNC_WEB_APPEND_ROOT_BUTTONS:
      WebAppend_Root_ControlUI();
    break;
    case FUNC_WEB_COMMAND:
      WebCommand_Parse();
    break;  
  }
  return 0;

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

void mInterfaceLight::WebAppend_Root_ControlUI(){

  char end_colour[8];
  char start_colour[8];
  char buffer[50];

// WRAP SLIDERS IN A CLASS

//ADD METHOD THAT UPDATES HUE,SAT,BRT BASED ON OTHER VALUE
//USE INEAR GRAD TO CHANGE IT

//  pCONT_web->WebAppend_Button(PM_BUTTON_NAME_RGB_CONTROLS_CTR, D_WEB_HANDLE_RGB_CONTROLS_PAGE);

  BufferWriterI->Append_P(PSTR("<tr><td><b>Scene Colour</b></td></tr>"));//GetPaletteFriendlyName(),GetNumberOfColoursInPalette(mPaletteI->palettelist.ptr));

  HsbColor hsb_current = HsbColor(RgbColor(0));//RgbColor(mode_singlecolour.colour.R,mode_singlecolour.colour.G,mode_singlecolour.colour.B));

  // GetColourFromPalette
  
  BufferWriterI->Append_P(PM_SLIDER_BACKGROUND_SINGLE_LINEAR_GRADIENT_JSON_KEY,  // Slider - Colour A to B with gradient
    "hue",               
    "col_sldr",
    "#800", "#f00 5%,#ff0 20%,#0f0 35%,#0ff 50%,#00f 65%,#f0f 80%,#f00 95%,#800",  // Hue colors
    // PSTR("#000"), PSTR("#fff"), //fff should be calculated based on colour[5]
    "hue_sldr",              
    D_JSON_HUE,
    1, 360,  // Range 0/1 to 100% 
    pCONT_iLight->HueF2N(hsb_current.H)
  ); 

  uint8_t dcolor = mapvalue(BrtF2N(hsb_current.B), 0, 100, 0, 255);
  snprintf_P(start_colour, sizeof(start_colour), PSTR("#%02X%02X%02X"), dcolor, dcolor, dcolor);  // Saturation start color from Black to White
  //   uint8_t red, green, blue;
  //   LightHsToRgb(hue, 255, &red, &green, &blue);
  RgbColor rgb = HsbColor(hsb_current.H,1,1);
  snprintf_P(end_colour, sizeof(end_colour), PSTR("#%02X%02X%02X"), rgb.R, rgb.G, rgb.B);  // Saturation end color

  BufferWriterI->Append_P(PM_SLIDER_BACKGROUND_SINGLE_LINEAR_GRADIENT_JSON_KEY,  // Slider - Colour A to B with gradient
    "sat",               
    "col_sldr",
    start_colour, 
    end_colour, //fff should be calculated based on colour[5]
    "sat_sldr",              
    D_JSON_SAT,
    0, 100,  // Range 0/1 to 100% 
    pCONT_iLight->SatF2N(hsb_current.S)
  ); 


  BufferWriterI->Append_P(PM_SLIDER_BACKGROUND_SINGLE_LINEAR_GRADIENT_JSON_KEY,  // Slider - Colour A to B with gradient
    "brt",               
    "col_sldr",
    PSTR("#000"), PSTR("#fff"), //fff should be calculated based on colour[5]
    "brt_sldr",              
    D_JSON_BRIGHTNESS_RGB,
    0, 100,  // Range 0/1 to 100% 

    getBriRGB_Global()
  ); 


  // #ifdef USE_WS28XX_FEATURE_4_PIXEL_TYPE
  // Can I use "ORANGE" with RGB as warm white? 
  // BufferWriterI->Append_P(HTTP_MSG_SLIDER_GRADIENT,  // Cold Warm
  //   "scn_clr_temp",             // a - Unique HTML id
  //   "#fff", "#ff0",  // White to Yellow
  //   1,               // sl1
  //   153, 500,        // Range color temperature
  //   200,//LightGetColorTemp(),
  //   't', 0
  // );         // t0 - Value id releated to lc("t0", value) and WebGetArg("t0", tmp, sizeof(tmp));

  // BufferWriterI->Append_P(PSTR("<div> Manual White Channel </div>"));
  // BufferWriterI->Append_P(HTTP_MSG_SLIDER_GRADIENT3,  // Brightness - Black to White
  //   "scn_w_temp",               // c - Unique HTML id
  //   PSTR("#000"), PSTR("#eee"),//"#fff",    // Black to White
  //   4,                 // sl4 - Unique range HTML id - Used as source for Saturation begin color
  //   0, 100,  // Range 0/1 to 100%
  //   pCONT_iLight->BrtF2N(mode_singlecolour.colourW),
  //   WEB_HANDLE_SCENE_COLOUR_WHITE_SLIDER
  // );           // d0 - Value id is related to lc("d0", value) and WebGetArg(request,"d0", tmp, sizeof(tmp));

  // if(settings.flags.enable_cct_channel_sliders){
  //   BufferWriterI->Append_P(PSTR("<div> CCT Controls</div>"));// BrtF2N(animation.brightness))
  //   // BufferWriterI->Append_P(HTTP_MSG_SLIDER_GRADIENT3,  // Cold Warm
  //   //   "pwm_cct",             // a - Unique HTML id
  //   //   "#fff", "#ff0",  // White to Yellow
  //   //   1,               // sl1
  //   //   153, 500,        // Range color temperature
  //   //   LightGetColorTemp(),
  //   //   "pwm_cct"
  //   // );         // t0 - Value id releated to lc("t0", value) and WebGetArg("t0", tmp, sizeof(tmp));

  //   // BufferWriterI->Append_P(HTTP_MSG_SLIDER_GRADIENT3,  // Brightness - Black to White
  //   //   "pwm_cbrt",               // c - Unique HTML id
  //   //   PSTR("#000"), PSTR("#eee"),//"#fff",    // Black to White
  //   //   4,                 // sl4 - Unique range HTML id - Used as source for Saturation begin color
  //   //   0, 100,  // Range 0/1 to 100%
  //   //   _briCT,
  //   //   "pwm_cbrt"
  //   // );           // d0 - Value id is related to lc("d0", value) and WebGetArg(request,"d0", tmp, sizeof(tmp));

  //   if(rgbcct_controller.getColorMode() & LCM_CT){
  //     BufferWriterI->Append_P(PM_SLIDER_BACKGROUND_SINGLE_LINEAR_GRADIENT_JSON_KEY,  // Slider - Colour A to B with gradient
  //       "cct_temp",               
  //       "col_sldr",
  //       "#fff", "#ff0",  // White to Yellow
  //       "cct_temp",              
  //       D_JSON_CCT_TEMP,
  //       153, 500,        // Range color temperature
  //      rgbcct_controller.getCT()
  //     ); 
  //     BufferWriterI->Append_P(PM_SLIDER_BACKGROUND_SINGLE_LINEAR_GRADIENT_JSON_KEY,  // Slider - Colour A to B with gradient
  //       "cct_brt",               
  //       "col_sldr",
  //       PSTR("#000"), PSTR("#eee"),//"#fff",    // Black to White
  //       "cct_brt",              
  //       D_JSON_BRIGHTNESS_CCT,
  //       0, 100,  // Range 0/1 to 100% 
  //       rgbcct_controller.getBriCT()
  //     ); 
  //   }

  // } // enable cct controls

  // #endif


      #ifdef USE_MODULE_LIGHTS_ANIMATOR
  BufferWriterI->Append_P(PSTR("{t}<tr>"));                            
    BufferWriterI->Append_P(HTTP_DEVICE_CONTROL_BUTTON_JSON_VARIABLE_INSERTS_HANDLE_IHR2,
                              100/2,
                              "", //no span
                              D_CLASSNAME_BUTTON_LIGHTPOWER_CTR " " "buttonh",
                              D_JSON_LIGHTPOWER, 
                              D_DEVICE_CONTROL_BUTTON_TOGGLE_CTR,
                              PSTR("Light Power "),
                              "BROKEN"
                              // rgbcct_controller.getBrightness255() ? D_JSON_ON : D_JSON_OFF //make this a state function
                              //mSupport::GetStateCtr_P()
                            );    
    BufferWriterI->Append_P(HTTP_DEVICE_CONTROL_BUTTON_JSON_VARIABLE_INSERTS_HANDLE_IHR2,
                              100/2,
                              "", //no span
                              D_CLASSNAME_BUTTON_ANIMATIONENABLE_CTR " " "buttonh",
                              D_JSON_ANIMATIONENABLE, 
                              D_DEVICE_CONTROL_BUTTON_TOGGLE_CTR,
                              PSTR("Animation "),
                              "BROKEN"
                              // animation.flags.fEnable_Animation ? D_JSON_ON : D_JSON_OFF
                            );                  
  BufferWriterI->Append_P(PSTR("</tr>{t2}"));
  

  #ifdef USE_WEBSERVER_ADVANCED_MULTIPAGES
  // Will include all settings related to ANY form of lighting hardware
  pCONT_web->WebAppend_Button(PM_BUTTON_NAME_RGB_CONTROLS_CTR, D_WEB_HANDLE_RGB_CONTROLS_PAGE);
  #endif // USE_WEBSERVER_ADVANCED_MULTIPAGES
  
   #endif // USE_MODULE_LIGHTS_ANIMATOR

}


void mInterfaceLight::WebAppend_Root_RGBPalette()
{

  // mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);

  uint8_t length = mPaletteI->GetNumberOfColoursInPalette(mPaletteI->palettelist.ptr); //pixelsinmap + name + length
  uint8_t pal_length = mPaletteI->GetNumberOfColoursInPalette(mPaletteI->palettelist.ptr); //pixelsinmap + name + length
  
  JsonBuilderI->Array_Start("rgb_pal_title");// Class name
    JsonBuilderI->Level_Start();
    char title_ctr[30];
    if(mPaletteI->palettelist.ptr->flags.fMapIDs_Type == MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_GRADIENT__ID){
      JsonBuilderI->Add_FV("ih",PSTR("\"%s (Gradient)\""), mPaletteI->GetPaletteFriendlyNameByID(mPaletteI->palettelist.ptr->id,title_ctr,sizeof(title_ctr)));
    }else{
      JsonBuilderI->Add_FV("ih",PSTR("\"%s (#%d)\""),mPaletteI->GetPaletteFriendlyNameByID(mPaletteI->palettelist.ptr->id,title_ctr,sizeof(title_ctr)),mPaletteI->GetNumberOfColoursInPalette(mPaletteI->palettelist.ptr));
    }
    JsonBuilderI->Level_End();
  JsonBuilderI->Array_End();

  char colourtype[5];
  switch(palette_view.show_type){
    default:
    case RGB_VIEW_SHOW_TYPE_ALWAYS_GRADIENT_ID:
      length = pal_length; //only send title and true length
      sprintf(colourtype,"%s","bclg");
    break;
    case RGB_VIEW_SHOW_TYPE_ALWAYS_BLOCKS_ID:
      sprintf(colourtype,"%s","bcbl");
    break;
  }
  
  JsonBuilderI->Array_Start("rgb_pal");// Class name
    JsonBuilderI->Level_Start();

      JsonBuilderI->Array_Start(colourtype);
      RgbTypeColor c;
      int16_t pixel_position = -2;
      for (uint16_t i= 0; i < length;i++){
        if(i < pal_length){
          c = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,i,&pixel_position);
        }else{      
          c = RgbColor(0);//default black
        }
        JsonBuilderI->Add_FV(PSTR("\"%02X%02X%02X\""),c.R,c.G,c.B);
      }
      JsonBuilderI->Array_End();

    JsonBuilderI->Level_End();
  JsonBuilderI->Array_End();
  
}

// Send updated data for sliders
void mInterfaceLight::WebAppend_Root_Sliders(){

  JsonBuilderI->Array_Start("col_sldr");// Class name


    // JsonBuilderI->Level_Start();
    //   JsonBuilderI->Add("id",0);
    //     JsonBuilderI->Array_Start("bclg");
    //   RgbTypeColor c;
    //   int16_t pixel_position = -2;
    //   for (uint16_t hue= 1; hue < 360;hue += 10){
    //     c = HsbColor(HueN2F(hue),scene.colour.,pCONT_ladd->scene.colour.B);         
    //     JsonBuilderI->Add_FV(PSTR("\"%02X%02X%02X\""),scene.colour.R,scene.colour.G,scene.colour.B);
    //   }
    //   JsonBuilderI->Array_End();
    // JsonBuilderI->Level_End();


    HsbColor hsb = RgbColor(255);//active_rgbcct_colour_p->R,active_rgbcct_colour_p->G,active_rgbcct_colour_p->B);

    JsonBuilderI->Level_Start();
      JsonBuilderI->Add("id",1);
        JsonBuilderI->Array_Start("bclg");
        RgbColor sat_low_colour = HsbColor(hsb.H,0,hsb.B);
        RgbColor sat_high_colour = HsbColor(hsb.H,1,hsb.B);
        JsonBuilderI->Add_FV(PSTR("\"%02X%02X%02X\""),sat_low_colour.R,sat_low_colour.G,sat_low_colour.B);
        JsonBuilderI->Add_FV(PSTR("\"%02X%02X%02X\""),sat_high_colour.R,sat_high_colour.G,sat_high_colour.B);
      JsonBuilderI->Array_End();
    JsonBuilderI->Level_End();

    JsonBuilderI->Level_Start();
      JsonBuilderI->Add("id",2);
        JsonBuilderI->Array_Start("bclg");    
        RgbColor brt_low_colour = HsbColor(hsb.H,hsb.S,0);
        RgbColor brt_high_colour = HsbColor(hsb.H,hsb.S,1);
        JsonBuilderI->Add_FV(PSTR("\"%02X%02X%02X\""),brt_low_colour.R,brt_low_colour.G,brt_low_colour.B);
        JsonBuilderI->Add_FV(PSTR("\"%02X%02X%02X\""),brt_high_colour.R,brt_high_colour.G,brt_high_colour.B);
      JsonBuilderI->Array_End();
    JsonBuilderI->Level_End();




  JsonBuilderI->Array_End();


}






void mInterfaceLight::Web_Root_Draw_PaletteSelect(AsyncWebServerRequest *request){
        
  JsonBuilderI->Start();  
  JsonBuilderI->Array_Start("rgb_palsel_draw");// Class name
    JsonBuilderI->Level_Start();
      JsonBuilderI->AddKey("ihr");           // function
        JsonBuilderI->AppendBuffer("\"");
          WebAppend_Root_Draw_PaletteSelect();
        JsonBuilderI->AppendBuffer("\"");
      JsonBuilderI->Level_End();
    JsonBuilderI->Array_End();
  JsonBuilderI->End();

  pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  

} //end function


void mInterfaceLight::WebAppend_Root_Draw_Table(){

  WebAppend_Root_Draw_RGBLive();
  // if(animation.mode_id == ANIMATION_MODE__EFFECTS){
    WebAppend_Root_Draw_RGBPalette();
  // }
  WebAppend_Root_Draw_PaletteSelect_Placeholder();
  // pCONT_web->WebAppend_Root_Draw_Table_dList(8,"rgb_table", kTitle_TableTitles_Root);
  
}



void mInterfaceLight::WebAppend_Root_Draw_RGBPalette(){
  char listheading[30];
  BufferWriterI->Append_P("%s",PSTR("{t}"));
    BufferWriterI->Append_P(PSTR("<tr><td> Palette: <span class='rgb_pal_title'>?</span></td></tr>"));
    BufferWriterI->Append_P(PSTR("<tr><td><div class='rgb_pal'></div></td></tr>"));       
  BufferWriterI->Append_P("%s",PSTR("{t2}"));
}


void mInterfaceLight::WebAppend_Root_Draw_PaletteSelect_Placeholder(){
  BufferWriterI->Append_P(PSTR("{t}"));
  BufferWriterI->Append_P(PSTR("<tr><div class='%s'></div></tr>"),"rgb_palsel_draw");
  BufferWriterI->Append_P(PSTR("{t2}"));
}


void mInterfaceLight::WebAppend_Root_Draw_PaletteSelect(){
  char listheading[30];
  // BufferWriterI->Append_P("\"%s\":[{\"ihr\":\"","rgb_palsel_draw");  
  
      // BufferWriterI->Append_P(HTTP_MSG_SLIDER_TITLE_JUSTIFIED,"<b>Palette List Select</b>","");

  // BufferWriterI->Append_P(PSTR("<tr><td><b>Palette List Select</b></td></tr>"));
  BufferWriterI->Append_P(PSTR("<p><form>"));
    
    // #ifdef ENABLE_DEVFEATURE_PIXEL_NEW_JSON_PALETTE_SELECT_ROOT
    BufferWriterI->Append_P(PSTR("<select name='pal_set' data-json_keys=\\\"%s\\\">"),D_JSON_COLOUR_PALETTE); //working
    // #else
    // // BufferWriterI->Append_P(PSTR("<select name='pal_set' id='pal_set' onchange='send_value(id);'>")); //not working
    // #endif // ENABLE_DEVFEATURE_PIXEL_NEW_JSON_PALETTE_SELECT_ROOT


      BufferWriterI->Append_P(PSTR("<optgroup label='User Editable'>"));
      
      char buffer[20];
      
      for (uint8_t row_id = mPaletteI->PALETTELIST_VARIABLE_HSBID_01__ID; row_id < mPaletteI->PALETTELIST_VARIABLE_HSBID_LENGTH__ID; row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
        BufferWriterI->Append_P(
          PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_CTR, 
          row_id, 
          mPaletteI->GetPaletteFriendlyNameByID(row_id, buffer, sizeof(buffer))
        );
      }
      BufferWriterI->Append_P(PSTR("</optgroup>"));
      BufferWriterI->Append_P(PSTR("<optgroup label='Preset'>"));
      for (uint8_t row_id = mPaletteI->PALETTELIST_VARIABLE_HSBID_LENGTH__ID; row_id < mPaletteI->PALETTELIST_STATIC_LENGTH__ID; row_id++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
        BufferWriterI->Append_P(
          PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_CTR, 
          row_id, 
          mPaletteI->GetPaletteFriendlyNameByID(row_id, buffer, sizeof(buffer))
        );
      }
      BufferWriterI->Append_P(PSTR("</optgroup>"));
    BufferWriterI->Append_P(PSTR("</select>"));
    // BufferWriterI->Append_P(PSTR("<br><br>"));
    // BufferWriterI->Append_P(PSTR("<input type='submit' value='Set'>"));
  BufferWriterI->Append_P(PSTR("</form></p>"));


}




void mInterfaceLight::WebSend_JSON_RootPage_LiveviewPixels(AsyncWebServerRequest *request)
{

  if(pCONT_web->RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;}  
  
  JsonBuilderI->Start();

    pCONT->Tasker_Interface(FUNC_WEB_RESPOND_LIGHT_LIVEVIEW_JSON);

  // uint16_t leds_max_to_show = STRIP_SIZE_MAX<MAX_LIVE_LEDS?STRIP_SIZE_MAX:MAX_LIVE_LEDS;
  // uint8_t number_of_pixels = map(liveview.pixel_resolution_percentage,0,100,0,STRIP_SIZE_MAX); //only serve every n'th LED if count over MAX_LIVE_LEDS
  // uint8_t pixels_to_iter = ((STRIP_SIZE_MAX-1)/number_of_pixels)+1;

  // char type_ctr[5];
  // switch(liveview.show_type){
  //   default: 
  //   case RGB_VIEW_SHOW_TYPE_ALWAYS_GRADIENT_ID: sprintf(type_ctr,"%s","bclg");  break;
  //   case RGB_VIEW_SHOW_TYPE_ALWAYS_BLOCKS_ID: sprintf(type_ctr,"%s","bcls");  break;
  // }

  // JsonBuilderI->Array_Start(WEB_CLASS_RGB_LIVE);// Class name
  //   JsonBuilderI->Level_Start();

  //     JsonBuilderI->Array_Start(type_ctr);
  //     RgbTypeColor c;
  //     for (uint16_t i= 0; i < leds_max_to_show; i += pixels_to_iter){ 
  //       c = GetPixelColor(i);
  //       JsonBuilderI->Add_FV(PSTR("\"%02X%02X%02X\""),c.R,c.G,c.B);
  //     }  
  //     JsonBuilderI->Array_End();

  //   JsonBuilderI->Level_End();
  // JsonBuilderI->Array_End();

  JsonBuilderI->End();

  pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  

}

void mInterfaceLight::WebPage_Root_AddHandlers(){
  
  /**
   * Root Page 
   * */
  pCONT_web->pWebServer->on(WEB_HANDLE_LIVEPIXELS_SHARED_JSON, [this](AsyncWebServerRequest *request){
    WebSend_JSON_RootPage_LiveviewPixels(request);     
  });
  // pCONT_web->pWebServer->on(WEB_HANDLE_JSON_LIVEPIXELS, [this](AsyncWebServerRequest *request){
  //   WebSend_JSON_RootPage_LiveviewPixels(request); 
  // });
  pCONT_web->pWebServer->on("/draw/palette_selector.json", HTTP_GET, [this](AsyncWebServerRequest *request){
    Web_Root_Draw_PaletteSelect(request);
  });



  // pCONT_web->pWebServer->on("/draw/palette_selector.json", HTTP_GET, [this](AsyncWebServerRequest *request){
  //   Web_Root_Draw_PaletteSelect(request);
  // });



}


//
  // "<div><span class='p'>%s</span><span class='q'>%s</span></div>";
void mInterfaceLight::WebAppend_Root_Draw_RGBLive(){
  
  BufferWriterI->Append_P(PSTR("{t}"));
  #ifdef ENABLE_DEVFEATURE_PIXEL_PIXELCOUNT_IN_LIVEVIEW
  BufferWriterI->Append_P(PSTR("<tr><td>Liveview <span class='p %s'>%d</span></td><td>Amount<span class='q %s'>%d</span></td></tr>"), "rgb_live_ttl", liveview.refresh_rate, "rgb_live_ttl", light_count);//GetPaletteFriendlyName(),GetNumberOfColoursInPalette(palettelist.ptr));
  BufferWriterI->Append_P(PSTR("<tr><td><div class='rgb_live' style='width:100%%;height:%dpx';></div></td></tr>"),
                            map(liveview.height_as_percentage,0,100,1,300)); //pixel height option for liveview
  #else
  BufferWriterI->Append_P(PSTR("<tr><td>Liveview <span class='%s'>%d</span></td></tr>"), "rgb_live_ttl", liveview.refresh_rate);
  BufferWriterI->Append_P(PSTR("<tr><td><div class='rgb_live' style='width:100%%;height:%dpx';></div></td></tr>"), map(liveview.height_as_percentage,0,100,1,300)); //pixel height option for liveview
  #endif // ENABLE_DEVFEATURE_PIXEL_PIXELCOUNT_IN_LIVEVIEW         
  BufferWriterI->Append_P(PSTR("{t2}"));

}




void mInterfaceLight::GetPixelColor(uint16_t indexPixel, uint8_t* _r,uint8_t* _g, uint8_t* _b, uint8_t* _w1, uint8_t* _w2)
{

//if colour is ww or cw, still represent in RGB hex code

// if(*_wc != nullptr)

  // DEBUG_LINE;
  // if(stripbus == nullptr){    
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   ALOG_DBM( PSTR(D_LOG_NEO "stripbus == nullptr"));
  //   #endif
  // }
  // RgbTypeColor color_hardware = stripbus->GetPixelColor(indexPixel);
  // RgbTypeColor color_internal = color_hardware; // To catch white element if present
  // DEBUG_LINE;

  // switch (settings.pixel_hardware_color_order_id){
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

  // return color_internal;
}








void mInterfaceLight::WebCommand_Parse(void)
{
//   char tmp[100];

//   uint16_t  arg_value = 0;
//   char arg_ctr[30]; memset(arg_ctr,0,sizeof(arg_ctr));

//   for (int i=0;i<5;i++){
//     sprintf(arg_ctr,"pwm%02d\0",i);
//     if(pCONT_web->request_web_command->hasParam(arg_ctr)) {
//       pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
//       arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//       AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);

//       // new_color[i] = arg_value;

//       // // use current as start point
//       // memcpy(&fade_start_10,&current_color,sizeof(fade_start_10));

//         arg_value = map(arg_value,0,1023,0,255);
//       switch(i){//}==0){
//         default:
//         case 0:  setRGB(arg_value,mode_singlecolour.colour.G,mode_singlecolour.colour.B); break;
//         case 1:  setRGB(mode_singlecolour.colour.R,arg_value,mode_singlecolour.colour.B); break;
//         case 2:  setRGB(mode_singlecolour.colour.R,mode_singlecolour.colour.G,arg_value); break;
//       }

//       UpdateFinalColourComponents();

//       // memset(entry_color,0,sizeof(entry_color));

//       // entry_color[0] = arg_value;


//       // changeChannels(entry_color);

//       // if (fade_running) {
//       //     // if fade is running, we take the curring value as the start for the next fade
//       //     memcpy(fade_start_10, fade_cur_10, sizeof(fade_start_10));
//       //   }
//       //   // memcpy(fade_end_10, cur_col_10, sizeof(fade_start_10));
//       //   fade_running = true;
//       //   fade_duration = 0;    // set the value to zero to force a recompute
//       //   fade_start = 0;

//       // // fade_start_10[i] = arg_value;
//       // // fade_running = 1;
//       // // UpdateSetOutputs();
//       // // RecalculateHsbfromRgb();

//       // pCONT_set->power = 1;

//     }
//   }

//   // HSB COLOURS
//   sprintf(arg_ctr,"pwm_hue0\0");
//   if(pCONT_web->request_web_command->hasParam(arg_ctr)) {
//     pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
//     arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//     AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
//    // SetColour_Hue(arg_value);
// CommandSet_Hue360(arg_value);
//     // UpdateSetOutputs();
//     // LightSetOutputs2();
//   }

//   sprintf(arg_ctr,"pwm_sat0\0");
//   if(pCONT_web->request_web_command->hasParam(arg_ctr)) {
//     pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
//     arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//     AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);


// arg_value = map(arg_value, 0,100, 0,255);

//     CommandSet_Sat(arg_value);
//     // UpdateSetOutputs();
//     // LightSetOutputs2();
//   }

//   sprintf(arg_ctr,"pwm_brt\0");
//   if(pCONT_web->request_web_command->hasParam(arg_ctr)) {
//     pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
//     arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//     AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
// arg_value = map(arg_value, 0,100, 0,255);
//     CommandSet_BrtRGB255(arg_value);
    
//     // UpdateSetOutputs();
//     // LightSetOutputs2();
//   }


//   sprintf(arg_ctr,"pwm_cct\0");
//   if(pCONT_web->request_web_command->hasParam(arg_ctr)) {
//     pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
//     arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//     AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
//     CommandSet_ColourTemp(arg_value);
//     // UpdateSetOutputs();
//     // LightSetOutputs2();
//   }
  
//   sprintf(arg_ctr,"pwm_cbrt\0");
//   if(pCONT_web->request_web_command->hasParam(arg_ctr)) {
//     pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
//     arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//     AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
// arg_value = map(arg_value, 0,100, 0,255);
//     CommandSet_BrtCT255(arg_value);
//     // UpdateSetOutputs();
//     // LightSetOutputs2();
//   }

//   sprintf(arg_ctr,"pwm_light_toggle\0");
//   if(pCONT_web->request_web_command->hasParam(arg_ctr)) {
//     pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
//     arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//     AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
// // arg_value = map(arg_value, 0,100, 0,255);
// //     CommandSet_BrtCT255(arg_value);
//     // UpdateSetOutputs();
//     // LightSetOutputs2();
//   }


//   sprintf(arg_ctr,"cct_rgb_linked\0");
//   if(pCONT_web->request_web_command->hasParam(arg_ctr)) {
//     pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
//     arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//     AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "hasParam(\"%s\")=%d"),arg_ctr,arg_value);
// // arg_value = map(arg_value, 0,100, 0,255);
//     if(Get_RGBCT_Linked()){
//         CommandSet_RGBCT_Linked(false);
//     }else{
//         CommandSet_RGBCT_Linked(true);
//     }
//     // CommandSet_BrtCT255(arg_value);
//     // UpdateSetOutputs();
//     // LightSetOutputs2();
//   }

}




#endif



/******************************************************************************************************************
 * mInterfaceLight_ConstructJSON.cpp
*******************************************************************************************************************/


uint8_t mInterfaceLight::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  char buffer[30];
  
  JsonBuilderI->Start();

  JsonBuilderI->Add_P(PM_JSON_TYPE, pCONT_set->Settings.light_settings.type);

  // // JsonBuilderI->Add_P(PM_JSON_PIXELS_UPDATE_PERCENTAGE, animation.transition.pixels_to_update_as_percentage);
  // #ifdef USE_MODULE_LIGHTS_ANIMATOR
  // JsonBuilderI->Add_P(PM_JSON_PIXELS_UPDATE_NUMBER, pCONT_lAni->SEGMENT_I(0).transition.pixels_to_update_as_number);
  // #endif // USE_MODULE_LIGHTS_ANIMATOR

  return JsonBuilderI->End();

}

/**
 * @brief Ovverview of what is being show regardless of animation/hardware.
 * I should be able to look here and get values needed for openhab/integration with other software
 * 
 * @param json_method 
 * @return uint8_t 
 */
uint8_t mInterfaceLight::ConstructJSON_State(uint8_t json_level, bool json_appending){

  // Active rgbcct palette used as scene

  char buffer[30];
  
  JsonBuilderI->Start();  

    JBI->Add("ColourPaletteID", pCONT_lAni->SEGMENT_I(0).palette.id);

    JBI->Add("Type", pCONT_set->Settings.light_settings.type);

    JsonBuilderI->Add_P(PM_JSON_TIME, (uint16_t)round(pCONT_lAni->SEGMENT_I(0).transition.time_ms/1000));
    JsonBuilderI->Add_P(PM_JSON_TIME_MS, pCONT_lAni->SEGMENT_I(0).transition.time_ms);


    #ifdef ENABLE_DEVFEATURE_DEBUG_PWM_CHANNELS_MQTT
    mqtthandler__state__ifchanged.tRateSecs = 1; // force this to be 1 second for this debug message
    JBI->Array_Start("PWM_Channels_Read");
    for (uint8_t i = 0; i < 5; i++) {
      if (pCONT_pins->PinUsed(GPIO_PWM1_ID, i)) 
      {
        JBI->Add(analogRead(pCONT_pins->GetPin(GPIO_PWM1_ID, i)));
      }
    }
    JBI->Array_End();
    #endif  // ENABLE_DEVFEATURE_DEBUG_PWM_CHANNELS_MQTT


  return JsonBuilderI->End();

}


#ifdef ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE_DEBUG_CONFIG
uint8_t mInterfaceLight::ConstructJSON_Debug_Module_Config(uint8_t json_level, bool json_appending){

  char buffer[30];
  
  JsonBuilderI->Start();

  JsonBuilderI->Level_Start("RgbcctController");
  
    // JsonBuilderI->Level_Start("raw");
    //   JsonBuilderI->Add("R", rgbcct_controller.R); 
    //   JsonBuilderI->Add("G", rgbcct_controller.G); 
    //   JsonBuilderI->Add("B", rgbcct_controller.B); 
    //   JsonBuilderI->Add("WW", rgbcct_controller.WW); 
    //   JsonBuilderI->Add("WC", rgbcct_controller.WC); 
    // JsonBuilderI->Level_End();
    JsonBuilderI->Level_Start("type");
    
    JsonBuilderI->Add("R", pCONT_lAni->SEGMENT_I(0).hardware_element_colour_order.r); 
    JsonBuilderI->Add("G", pCONT_lAni->SEGMENT_I(0).hardware_element_colour_order.g); 
    JsonBuilderI->Add("B", pCONT_lAni->SEGMENT_I(0).hardware_element_colour_order.b); 
    JsonBuilderI->Add("WW", pCONT_lAni->SEGMENT_I(0).hardware_element_colour_order.w); 
    JsonBuilderI->Add("WC", pCONT_lAni->SEGMENT_I(0).hardware_element_colour_order.c); 


    JsonBuilderI->Level_End();

    // JsonBuilderI->Add("mPaletteI->active_scene_palette_id",mPaletteI->active_scene_palette_id);

  JsonBuilderI->Level_End();


  if(pCONT_lAni->SEGMENT_I(0).palette.id == mPalette::PALETTELIST_VARIABLE_GENERIC_01__ID)
  {

    JBI->Array_AddArray("encoded", pCONT_set->Settings.animation_settings.palette_encoded_users_colour_map, ARRAY_SIZE(pCONT_set->Settings.animation_settings.palette_encoded_users_colour_map));

  }


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
#endif // ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE_DEBUG_CONFIG





/******************************************************************************************************************
 * mInterfaceLight_MQTT.cpp
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mInterfaceLight::MQTTHandler_Init()
{

  struct handler<mInterfaceLight>* ptr;  

  ptr = &mqtthandler__settings__teleperiod;
  ptr->handler_id = MQTT_HANDLER_SETTINGS_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mInterfaceLight::ConstructJSON_Settings;

  ptr = &mqtthandler__state__ifchanged;
  ptr->handler_id = MQTT_HANDLER_MODULE__STATE__TELEPERIOD_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__STATE__CTR;
  ptr->ConstructJSON_function = &mInterfaceLight::ConstructJSON_State;
  
  #ifdef ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE_DEBUG_CONFIG
  ptr = &mqtthandler__debug_module_config__teleperiod;
  ptr->handler_id = MQTT_HANDLER_MODULE__DEBUG_MODULE_CONFIG__TELEPERIOD_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_MODULE_CONFIG__CTR;
  ptr->ConstructJSON_function = &mInterfaceLight::ConstructJSON_Debug_Module_Config;
  #endif // ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE_DEBUG_CONFIG

} //end "MQTTHandler_Init"

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mInterfaceLight::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mInterfaceLight::MQTTHandler_Set_DefaultPeriodRate()
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
void mInterfaceLight::MQTTHandler_Sender(uint8_t id)
{    
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_LIGHTS_INTERFACE_ID, handle, id);
  }
}
  
#endif// USE_MODULE_NETWORK_MQTT




#endif // USE_DRIVER
