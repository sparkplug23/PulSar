#include "mInterfaceLight.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE // interface is the gateway


/**
 * 

{"AnimationMode":"Scene","SceneName":"ColourSingle","hue":120,"sat":100,"brt_rgb":5,"brt_cct":100}
{"SceneName":"PaletteSingle","ColourPalette":31,"brt_rgb":20,"brt_cct":100,"time_ms":1000}
{"SceneName":"PaletteSingle","ColourPalette":31,"brt_rgb":100,"brt_cct":100,"time_ms":1000}
{"AnimationMode":"Scene","SceneName":"ColourSingle","hue":0,"sat":100,"cct_temp":600,"brt_rgb":100,"brt_cct":10,"time_ms":10000}
{"AnimationMode":"Flasher","ColourPalette":1,"brt_rgb":100,"brt_cct":1,"time_ms":1000}
{"AnimationMode":"Flasher","flasher":{"function":1},"transition":{"order":"Random","rate":1},"ColourPalette":1,"brt_rgb":100,"brt_cct":1,"time_ms":1000}
{"AnimationMode":"Flasher","flasher":{"function":1},"transition":{"order":"Random","rate_ms":1000},"ColourPalette":1,"brt_rgb":100,"brt_cct":1,"time_ms":1000}


 */



/*** 
 * * Dev notes
 * 
//  * - Any non add LEDs will be controlled through here for the short term 
//  * - Long term, with several colour types and gamma stuff, introduce this is a gateway to rgbanimator (ie if led type, send command to the class AFTER calculating its colour etc for mode_singlecolours
 * )
 * 
 * */


void mInterfaceLight::Init_NeoPixelAnimator(uint16_t size, uint8_t timebase){

  animator_controller = new NeoPixelAnimator(size, timebase); // NeoPixel animation management object
  
}  




void mInterfaceLight::Settings_Default(){

  // #ifdef ENABLE_LOG_LEVEL_INFO
  // AddLog_P(LOG_LEVEL_TEST,PSTR("mInterfaceLight::Settings_Default"));
  // #endif

  // #ifdef USE_LIGHTING_TEMPLATE
  //   settings.flags.TemplateProvidedInProgMem = true;
  // #endif
  
  // if(settings.flags.TemplateProvidedInProgMem){// use template provided in progmem || override template
  
  //   Template_Load();

  // }else{
    
  //   #ifdef DEFAULT_LIGHTING_ANIMATION_MODE_ID
  //     animation.mode_id = DEFAULT_LIGHTING_ANIMATION_MODE_ID<ANIMATION_MODE_LENGTH_ID?
  //                           DEFAULT_LIGHTING_ANIMATION_MODE_ID:ANIMATION_MODE_FLASHER_ID;
  //   #else
  //     animation.mode_id = ANIMATION_MODE_FLASHER_ID;
  //   #endif

  // }

}



/*******************************************************************************************************************
********************************************************************************************************************
************ START OF SCENES DEFINITIONS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/

void mInterfaceLight::init_Scenes(){

  mode_singlecolour.name_id = mode_singlecolour_stored.name_id = MODE_SINGLECOLOUR_COLOURSCENE_ID;

  mode_singlecolour.name_id = MODE_SINGLECOLOUR_PALETTE_SINGLE_ID;

  changeHSB(120,100,0);
  
  // #ifdef DONT_USE_OLD_PRESETS
  // mode_singlecolour_preset_dayon.colour       = preset_colour_map[COLOUR_MAP_PINK_ID]; 
  // mode_singlecolour_preset_dayoff.colour      = preset_colour_map[COLOUR_MAP_PINK_ID];
  // mode_singlecolour_preset_eveningon.colour   = preset_colour_map[COLOUR_MAP_PINK_ID];
  // mode_singlecolour_preset_eveningoff.colour  = preset_colour_map[COLOUR_MAP_PINK_ID];
  // mode_singlecolour_preset_nighton.colour     = preset_colour_map[COLOUR_MAP_PINK_ID];
  // mode_singlecolour_preset_nightoff.colour    = preset_colour_map[COLOUR_MAP_PINK_ID];
  // #endif

} //end "init_Scenes"


void mInterfaceLight::StartFadeToNewColour(RgbcctColor targetColor, uint16_t _time_to_newcolour,  RgbcctColor fromcolor){ 

  AddLog_P(LOG_LEVEL_INFO, PSTR("BEFORE1 _briCT, _briRGB %d %d"), _briCT, _briRGB);

  #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
  pCONT_ladd->FadeToNewColour(targetColor, _time_to_newcolour);
  #endif // USE_MODULE_LIGHTS_ADDRESSABLE
  #ifdef USE_MODULE_LIGHTS_PWM
  pCONT_lPWM->FadeToNewColour(targetColor, _time_to_newcolour);
  #endif // USE_MODULE_LIGHTS_ADDRESSABLE

}

// This will handle various colour palettes that are not "user lights" in nature (random colours or flashes per pixel)
// Instead, whole colour designs, changes over time (sunrise/wakeup light)
void mInterfaceLight::SubTask_SingleColour(){ 
  // #ifdef ENABLE_LOG_LEVEL_DEBUG
  // AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO D_CLASS_NAME_mInterfaceLight "SubTask_SingleColour"));
  // #endif
  
  // // Check for reseting SCENES
  // if(mode_singlecolour_stored.fActive){
  //   // if(abs(millis()-scene_stored.tStart)>scene_stored.tOnTime){
  //   //   scene.name_id = scene_stored.name_id; // restore mode
  //   //   scene_stored.fActive = false;   // finish
  //   // }
  // }

  //if enable scene sleep

  switch(mode_singlecolour.name_id){
    case MODE_SINGLECOLOUR_COLOURSCENE_ID:{

      //Need running flag AND update flag
      if(mode_singlecolour.flag.generate_new_colours){
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "mode_singlecolour.generate_new_colours == true"));
        
      // if(mode_singlecolour.update == true){
        
        
        // adjusted for brightness
        RgbcctColor targetColor = RgbcctColor(0);
        targetColor.R = current_color[0];
        targetColor.G = current_color[1];
        targetColor.B = current_color[2];
        targetColor.WC = current_color[3];
        targetColor.WW = current_color[4];

        AddLog_P(LOG_LEVEL_TEST, PSTR("animation.transition.time_ms.val=%d"),animation.transition.time_ms.val);
        
        // AddLog_P(LOG_LEVEL_TEST, PSTR("[%d|%d] colour\t%d\t%d\t%d\t%d\t%d"),mode_singlecolour.parts,pixels_in_map,colour.R,colour.G,colour.B,colour.WW,colour.WC);                        
        StartFadeToNewColour(targetColor, animation.transition.time_ms.val);

        // mode_singlecolour.name_id = MODE_SINGLECOLOUR_NOCHANGE_ID;
        #ifdef ENABLE_LOG_LEVEL_DEBUG
        AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "mode_singlecolour.name_id=MODE_SINGLECOLOUR_COLOURSCENE_ID Executing"));
        #endif

        //disable future update
        animation.flags.fForceUpdate = true; // update output
        // mode_singlecolour.update = false;
        // memcpy(last_colour, current_color, sizeof(last_colour)); //temp fix
        // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "mode_singlecolour.update == false"));

        mode_singlecolour.flag.generate_new_colours = false; // as this runs ONCE
      }
      

    }break;
    case MODE_SINGLECOLOUR_PALETTE_SINGLE_ID:{
      
      if(mode_singlecolour.flag.generate_new_colours){

        if(mTime::TimeReached(&mode_singlecolour_tSaved,animation.transition.time_ms.val)){
          
          AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "mode_singlecolour.generate_new_colours == true"));
          
          // Update pointer of struct
          SetPaletteListPtrFromID(animation.palette_id);
          
          uint8_t pixels_in_map = GetPixelsInMap(palettelist.ptr);
          
          #ifdef ENABLE_LOG_LEVEL_DEBUG
          AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "pixels_in_map %d"),pixels_in_map);
          #endif
        
          uint16_t desired_pixel = mode_singlecolour.parts < pixels_in_map ? mode_singlecolour.parts : pixels_in_map;
          int16_t start_pixel_position = -1;
          RgbcctColor colour = GetColourFromPalette(palettelist.ptr, desired_pixel, &start_pixel_position);

          ApplyGlobalBrightnesstoColour(&colour);

          // force instant change on first part
          if(mode_singlecolour.parts == 0){ animation_override.time_ms = 1;}
          
          AddLog_P(LOG_LEVEL_TEST, PSTR("[%d|%d] colour\t%d\t%d\t%d\t%d\t%d"),mode_singlecolour.parts,pixels_in_map,colour.R,colour.G,colour.B,colour.WW,colour.WC);                        
          StartFadeToNewColour(colour, animation.transition.time_ms.val);
    
          mode_singlecolour.parts++;
          if(mode_singlecolour.parts>=pixels_in_map){
            mode_singlecolour.parts = 0;
            // mode_singlecolour.name_id = MODE_SINGLECOLOUR_NOCHANGE_ID;
            // mode_singlecolour.update = false;
            mode_singlecolour.flag.generate_new_colours = false; // as this runs ONCE
            // memcpy(last_colour, current_color, sizeof(last_colour)); //temp fix
          }else{
            AddLog_P(LOG_LEVEL_TEST, PSTR("mode_singlecolour.update = false NOT CLEARED %d %d"),mode_singlecolour.parts,pixels_in_map);
          }
        }

      }

    }break;
    default:
      AddLog_P(LOG_LEVEL_INFO, PSTR("default"));
    case MODE_SINGLECOLOUR_FADE_OFF_ID:{
      changeRGB(0,0,0);
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      pCONT_ladd->FadeToNewColour(mode_singlecolour.colour,animation.transition.time_ms.val);
      #endif // USE_MODULE_LIGHTS_ADDRESSABLE
      // mode_singlecolour.name_id = MODE_SINGLECOLOUR_NOCHANGE_ID;
      mode_singlecolour.update = false;
      memcpy(last_colour, current_color, sizeof(last_colour)); //temp fix
    }break;

    // case MODE_SINGLECOLOUR_FLASHCOLOUR_ID:
    //   // stripbus->ClearTo(HsbColor(scene_stored.colour)); // instant
    //   // scene_stored.tOnTime = 200;
    //   // scene_stored.name_id = scene.name_id; // save mode to return to
    //   // scene_stored.fActive = true;
    //   // scene_stored.tStart = millis();
    // break;
    // case MODE_SINGLECOLOUR_NOCHANGE_ID: break;
  }

} // END FUNCTION



const char* mInterfaceLight::GetSceneName(char* buffer, uint8_t buflen){
  return GetSceneNameByID(mode_singlecolour.name_id, buffer, buflen);
}
const char* mInterfaceLight::GetSceneNameByID(uint8_t id, char* buffer, uint8_t buflen){
  switch(id){
    default:
    case MODE_SINGLECOLOUR_COLOURSCENE_ID:    snprintf_P(buffer, buflen, PM_MODE_SINGLECOLOUR_COLOURSCENE_NAME_CTR); break;
    case MODE_SINGLECOLOUR_PALETTE_SINGLE_ID: snprintf_P(buffer, buflen, PM_MODE_SINGLECOLOUR_PALETTE_SINGLE_NAME_CTR); break;
  }
  return buffer;
  
  // return (scene.name_id == MODE_SINGLECOLOUR_DAYON_ID ? D_MODE_SINGLECOLOUR_DAYON_NAME_CTR :
  //     (scene.name_id == MODE_SINGLECOLOUR_DAYOFF_ID ?      D_MODE_SINGLECOLOUR_DAYOFF_NAME_CTR :
  //     (scene.name_id == MODE_SINGLECOLOUR_EVENINGON_ID ?     D_MODE_SINGLECOLOUR_EVENINGON_NAME_CTR :
  //     (scene.name_id == MODE_SINGLECOLOUR_EVENINGOFF_ID ?    D_MODE_SINGLECOLOUR_EVENINGOFF_NAME_CTR :
  //     (scene.name_id == MODE_SINGLECOLOUR_MIDNIGHTON_ID ?    D_MODE_SINGLECOLOUR_MIDNIGHTON_NAME_CTR :
  //     (scene.name_id == MODE_SINGLECOLOUR_MIDNIGHTOFF_ID ?    D_MODE_SINGLECOLOUR_MIDNIGHTOFF_NAME_CTR :
  //     (scene.name_id == MODE_SINGLECOLOUR_FLASHCOLOUR_ID ?   D_MODE_SINGLECOLOUR_FLASHCOLOUR_NAME_CTR :
  //     (scene.name_id == MODE_SINGLECOLOUR_COLOURSCENE_ID ?    D_MODE_SINGLECOLOUR_COLOURSCENE_NAME_CTR :
  //     (scene.name_id == MODE_SINGLECOLOUR_SUNRISE_SINGLE_ID ?    D_MODE_SINGLECOLOUR_SUNRISE_SINGLE_NAME_CTR :
  //     (scene.name_id == MODE_SINGLECOLOUR_NOCHANGE_ID ?    D_MODE_SINGLECOLOUR_NOTACTIVE_NAME_CTR :
  //     "NoMatch"))))))))));

}


int8_t mInterfaceLight::GetSceneIDbyName(const char* c){
  if(c=='\0'){
    return -1;
  }

// have a bunch of presets, and 5 user options/naming


  // if(strstr(c,D_MODE_SINGLECOLOUR_DAYON_NAME_CTR)){
  //   return MODE_SINGLECOLOUR_DAYON_ID;
  // }else if(strstr(c,D_MODE_SINGLECOLOUR_DAYOFF_NAME_CTR)){
  //   return MODE_SINGLECOLOUR_DAYOFF_ID;
  // }else if(strstr(c,D_MODE_SINGLECOLOUR_EVENINGON_NAME_CTR)){
  //   return MODE_SINGLECOLOUR_EVENINGON_ID;
  // }else if(strstr(c,D_MODE_SINGLECOLOUR_EVENINGOFF_NAME_CTR)){
  //   return MODE_SINGLECOLOUR_EVENINGOFF_ID;
  // }else if(strstr(c,D_MODE_SINGLECOLOUR_MIDNIGHTON_NAME_CTR)){
  //   return MODE_SINGLECOLOUR_MIDNIGHTON_ID;
  // }else if(strstr(c,D_MODE_SINGLECOLOUR_MIDNIGHTOFF_NAME_CTR)){
  //   return MODE_SINGLECOLOUR_MIDNIGHTOFF_ID;
  // }else if(strstr(c,D_MODE_SINGLECOLOUR_FLASHCOLOUR_NAME_CTR)){
  //   return MODE_SINGLECOLOUR_FLASHCOLOUR_ID;
  // }else 
  if(strstr_P(c,PM_MODE_SINGLECOLOUR_COLOURSCENE_NAME_CTR)){
    return MODE_SINGLECOLOUR_COLOURSCENE_ID;
  }
  if(strstr_P(c,PM_MODE_SINGLECOLOUR_PALETTE_SINGLE_NAME_CTR)){
    return MODE_SINGLECOLOUR_PALETTE_SINGLE_ID;
  }
  //else if(strstr(c,D_MODE_SINGLECOLOUR_SUNRISE_SINGLE_NAME_CTR)){
  //   return MODE_SINGLECOLOUR_SUNRISE_SINGLE_ID;
  // }else if(strstr(c,D_MODE_SINGLECOLOUR_NOTACTIVE_NAME_CTR)){
  //   return MODE_SINGLECOLOUR_NOCHANGE_ID;
  // }else{
  //   return -1;
  // }
}


RgbTypeColor mInterfaceLight::Color32bit2RgbColour(uint32_t colour32bit){
  RgbTypeColor rgb;
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



uint8_t mInterfaceLight::ConstructJSON_Scene(uint8_t json_method){

  char buffer[30];
  
  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_SCENENAME, GetSceneName(buffer, sizeof(buffer)));
    JsonBuilderI->Add(D_JSON_HUE, getHue());
    JsonBuilderI->Add(D_JSON_SAT, getSat());
    JsonBuilderI->Add(D_JSON_BRT, getBriRGB());
    JsonBuilderI->Add(D_JSON_TIME, (uint16_t)round(animation.transition.time_ms.val/1000));
    JsonBuilderI->Add(D_JSON_TIME_MS, animation.transition.time_ms.val);
  return JsonBuilderI->End();

}

/*******************************************************************************************************************
********************************************************************************************************************
************ END OF SCENES DEFINITIONS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/

bool mInterfaceLight::LightModuleInit(void)
{
  pCONT_set->Settings.light_settings.type = LT_BASIC;                    // Use basic PWM control if SetOption15 = 0

  if (pCONT_set->Settings.flag_system.pwm_control) {          // SetOption15 - Switch between commands PWM or COLOR/DIMMER/CT/CHANNEL
    for (uint32_t i = 0; i < MAX_PWMS; i++) {
      if (pCONT_pins->PinUsed(GPIO_PWM1_ID, i)) { 
        pCONT_set->Settings.light_settings.type++; 
        AddLog_P(LOG_LEVEL_DEBUG,PSTR("LightModuleInit PWM%d"),i);
        // delay(1000);
        
      }  // Use Dimmer/Color control for all PWM as SetOption15 = 1
    }
  }

  //light_flg = 0;
  // Create another array index that holds group of module (lights, drivers ID) and then make fucntion to check it
  // if (XlgtCall(FUNC_MODULE_INIT)) {
  //   // serviced
  // }

  //temp fix
  if (pCONT_pins->PinUsed(GPIO_RGB_DATA_ID)) { 
    
    // AddLog_P(LOG_LEVEL_TEST,PSTR("pCONT_set->Settings.light_settings.type=%d"),pCONT_set->Settings.light_settings.type);


    pCONT_set->Settings.light_settings.type = LT_WS2812; }


// #ifdef ESP8266
//   else if (SONOFF_BN == my_module_type) {   // PWM Single color led (White)
//     Settings.light_settings.type = LT_PWM1;
//   }
//   else if (SONOFF_LED == my_module_type) {  // PWM Dual color led (White warm and cold)
//     if (!my_module.io[4]) {                 // Fix Sonoff Led instabilities
//       pinMode(4, OUTPUT);                   // Stop floating outputs
//       digitalWrite(4, LOW);
//     }
//     if (!my_module.io[5]) {
//       pinMode(5, OUTPUT);                   // Stop floating outputs
//       digitalWrite(5, LOW);
//     }
//     if (!my_module.io[14]) {
//       pinMode(14, OUTPUT);                  // Stop floating outputs
//       digitalWrite(14, LOW);
//     }
//     Settings.light_settings.type = LT_PWM2;
//   }
// #endif  // ESP8266

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

  // #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("mInterfaceLight::Template_Load()"));
  // #endif

  // load from progmem
  uint16_t progmem_size = sizeof(LIGHTING_TEMPLATE);
  progmem_size = progmem_size>1500?1500:progmem_size;
  // create parse buffer
  char buffer[progmem_size];
  // Read into local
  memcpy_P(buffer,LIGHTING_TEMPLATE,sizeof(LIGHTING_TEMPLATE));
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("LIGHTING_TEMPLATE" " READ = \"%s\""), buffer);

  DynamicJsonDocument doc(1500);
  DeserializationError error = deserializeJson(doc, buffer);

  if(error){
    AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_ERROR_JSON_DESERIALIZATION));
    return;
  }

  pCONT->Tasker_Interface(FUNC_JSON_COMMAND_OBJECT, doc.as<JsonObjectConst>());

}


void mInterfaceLight::Init(void) //LightInit(void)
{

  // if(function == FUNC_INIT){
    //colour_arr_ptr = reinterpret_cast<uint8_t*>(&scene_colour);
  // }

  device = pCONT_set->devices_present;
  
  //subtype = (pCONT_set->Settings.light_settings.type & 7) > LST_MAX ? LST_MAX : (pCONT_set->Settings.light_settings.type & 7); // Always 0 - LST_MAX (5)
  subtype = LST_RGBCW;
  
  pwm_multi_channels = 0;//pCONT_set->Settings.flag3.pwm_multi_channels;  // SetOption68 - Enable multi-channels PWM instead of Color PWM

  pCONT_set->Settings.pwm_range = PWM_RANGE; //tmp

  // pCONT_set->Settings.dimmer_hw_min = 0;
  // pCONT_set->Settings.dimmer_hw_max =100;
  // pCONT_set->Settings.light_correction = 0;
  
  pCONT_set->Settings.light_settings.light_fade = 1;
  pCONT_set->Settings.light_settings.light_speed = 5*2;
  pCONT_set->power = 1;

  auto_time_off_secs = 0;
  
  init_Scenes();

  init_PresetColourPalettes();
  init_Animations();

  #ifdef USE_MODULE_LIGHTS_PWM
    settings.flags.enable_cct_channel_sliders = true;
  #else
    settings.flags.enable_cct_channel_sliders = false;
  #endif // USE_MODULE_LIGHTS_PWM



  //new_color[0] = 255;
  // LightApplyPower(new_color, 1);

  // if (LST_RGBW <= subtype) {
  //   // only change if RGBW or RGBCW
  //   // do not allow independant RGB and WC colors
  //   bool ct_rgb_linked = 0;// !(pCONT_set->Settings.param[P_RGB_REMAP] & 128);
  //   setCTRGBLinked(ct_rgb_linked);
  // }
  setCTRGBLinked(false); //temp
  setCT(CT_MIN);

  if ((LST_SINGLE <= subtype) && pwm_multi_channels) {
    // we treat each PWM channel as an independant one, hence we switch to
    setPWMMultiChannel(true);
    device = pCONT_set->devices_present - subtype + 1; // adjust if we also have relays
  } else if (!isCTRGBLinked()) {
    // if RGBW or RGBCW, and SetOption37 >= 128, we manage RGB and W separately
    device--;   // we take the last two devices as lights
  }

  LightCalcPWMRange();
  #ifdef DEBUG_LIGHT
    AddLog_P(LOG_LEVEL_TEST, "LightInit pwm_multi_channels=%d subtype=%d device=%d devices_present=%d",
      pwm_multi_channels, subtype, device, pCONT_set->devices_present);
  #endif

  setSubType(subtype);
  // loadSettings();
  UpdateFinalColourComponents();    // calculate the initial values (#8058)

  // Serial.println("UpdateFinalColourComponents DONE"); Serial.flush();

  if (LST_SINGLE == subtype) {
    pCONT_set->Settings.light_settings.light_color[0] = 255;      // One channel only supports Dimmer but needs max color
  }
  // if (Settings.light_settings.type < LT_PWM6) {           // PWM
  //   for (uint32_t i = 0; i < Settings.light_settings.type; i++) {
  //     Settings.pwm_value[i] = 0;        // Disable direct PWM control
  //     if (PinUsed(GPIO_PWM1, i)) {
  //       pinMode(Pin(GPIO_PWM1, i), OUTPUT);
  //     }
  //   }
  //   if (PinUsed(GPIO_ARIRFRCV)) {
  //     if (PinUsed(GPIO_ARIRFSEL)) {
  //       pinMode(Pin(GPIO_ARIRFSEL), OUTPUT);
  //       digitalWrite(Pin(GPIO_ARIRFSEL), 1);  // Turn off RF
  //     }
  //   }
  // }

  // Set light mode
  if(pCONT_set->Settings.light_settings.type == LT_WS2812){ //RGB only
    setColorMode(LCM_RGB);
  }else{
    setColorMode(LCM_BOTH);
  }


  uint32_t max_scheme = max_scheme;
  if (subtype < LST_RGB) {
    max_scheme = LS_POWER;
  }
  if ((LS_WAKEUP == pCONT_set->Settings.light_settings.light_scheme) || (pCONT_set->Settings.light_settings.light_scheme > max_scheme)) {
    pCONT_set->Settings.light_settings.light_scheme = LS_POWER;
  }
  pCONT_set->power = 1;
  update = true;
  wakeup_active = 0;
  // if (pCONT_set->Settings.flag4.fade_at_startup) {
    fade_initialized = true;      // consider fade intialized starting from black
  // }

//   pCONT_set->Settings.light_settings.type = LT_BASIC;                    // Use basic PWM control if SetOption15 = 0

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

changeBri(0); //default off


} //light_init



void mInterfaceLight::init_Animations(){

  
  // Set default values (ifdef below sets specific)
  #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
  animation.transition.order_id = TRANSITION_ORDER_INORDER_ID;
  #endif // USE_MODULE_LIGHTS_ADDRESSABLE
  animation.palette_id = PALETTELIST_VARIABLE_USER_01_ID;
  animation.mode_id = ANIMATION_MODE_FLASHER_ID;
  // flashersettings.function = FLASHER_FUNCTION_SLOW_GLOW_ID;
  #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
  animation.transition.method_id = TRANSITION_METHOD_BLEND_ID;
  #endif // USE_MODULE_LIGHTS_ADDRESSABLE
  animation.flags.fForceUpdate = true;
  // animation.brightness = 0.1;
  animation.transition.time_ms.val = 1000;
  animation.transition.rate_ms.val = 20*1000;
  // tSavedUpdateRGBString = millis() + animation.transition.rate_ms.val;
  // first_set = true; //change to one off override flag


  animation.flags.fEnable_Animation = true;

}




// Colours etc will be made or called using a func_update_light_Values
// Another func will push those values to the output, which each hardware class will handle

// // Used for timed on or off events
int8_t mInterfaceLight::Tasker(uint8_t function){
  
  int8_t function_result = 0;

  // return 0;
 
  // As interface module, the parsing of module_init takes precedence over the Settings.light_settings.type
  switch(function){
    case FUNC_TEMPLATE_MODULE_LOAD:
      Template_Load();
      break;
    case FUNC_MODULE_INIT:
      LightModuleInit();
      break;
    case FUNC_PRE_INIT:
      return FUNCTION_RESULT_HANDLED_ID;
    case FUNC_BOOT_MESSAGE:
      BootMessage();
      break;
  }

  switch(function){
    case FUNC_INIT:
      Init();
    break;    
    case FUNC_POINTER_INIT:
      init_PresetColourPalettes();
    break;
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
    case FUNC_EVERY_SECOND:

    
      // mode_singlecolour.name_id = MODE_SINGLECOLOUR_PALETTE_SINGLE_ID;

    
    break;
    case FUNC_EVERY_MINUTE:

    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_COMMAND:

    break;
    // case FUNC_SET_POWER:
    //   LightSetPower();
    // break;
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
  } // end switch
  
  /************
   * WEBPAGE SECTION * 
  *******************/
  #ifdef USE_MODULE_CORE_WEBSERVER
  return Tasker_Web(function);
  #endif // USE_MODULE_CORE_WEBSERVER

} // END function
int8_t mInterfaceLight::Tasker(uint8_t function, JsonObjectConst obj){
  switch(function){
    case FUNC_JSON_COMMAND_OBJECT:
      parse_JSONCommand(obj);
    break;
    case FUNC_JSON_COMMAND_OBJECT_WITH_TOPIC:
      return CheckAndExecute_JSONCommands(obj);
    break;
  }
}


int8_t mInterfaceLight::CheckAndExecute_JSONCommands(JsonObjectConst obj){

  // Check if instruction is for me
  if(mSupport::mSearchCtrIndexOf(data_buffer.topic.ctr,"set/light")>=0){
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND "mInterfaceLight"));
    #endif //#ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
    parse_JSONCommand(obj);
    return FUNCTION_RESULT_HANDLED_ID;
  }else{
    return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
  }

}


void mInterfaceLight::parse_JSONCommand(JsonObjectConst obj){
  
  int8_t tmp_id = 0;
  char buffer[50];
  // Check all commands
  #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
  AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "Checking all commands"));
  #endif // #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
  // parsesub_CheckAll(obj); 
    
  //   #ifdef USE_MODULE_LIGHTS_PWM
  //   // pCONT_lPWM->parsesub_CheckAll(obj);
  //   #endif // USE_MODULE_LIGHTS_PWM
  // // }

  #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
  pCONT_ladd->parse_JSONCommand(obj);
  #endif // USE_MODULE_LIGHTS_ADDRESSABLE


  if(!obj[D_JSON_HARDWARE_TYPE].isNull()){ 
    const char* onoff = obj[D_JSON_HARDWARE_TYPE];
    //make function
    if(strstr(onoff,"RGBCCT_PWM")){ 
      #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_HARDWARE_TYPE, "RGBCCT_PWM");
      #endif
      pCONT_set->Settings.light_settings.type = LT_PWM5;
      data_buffer.isserviced++;
    }else 
    if(strstr(onoff,D_JSON_WS2812)){
      #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_HARDWARE_TYPE, "WS2812");
      #endif
      pCONT_set->Settings.light_settings.type = LT_WS2812;
      data_buffer.isserviced++;
    }else{
      #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
      AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_NOMATCH));
      #endif // #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    }
  }


  // if(!obj[D_JSON_ONOFF].isNull()){ 
  //   uint8_t value = obj[D_JSON_ONOFF];
  //   setBri(value ? 255 : 0);
  //   #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
  //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_ONOFF,0);
  //   #endif //#ifdef ENABLE_LOG_LEVEL_INFO_PARSING
  //   data_buffer.isserviced++;
  // }

  if(!obj[D_JSON_HUE].isNull()){ 
    uint16_t hue = obj[D_JSON_HUE];
    changeHSB_Hue(hue);
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_HUE,hue);
    #endif // ENABLE_LOG_LEVEL_INFO_PARSING
    data_buffer.isserviced++;
  }

  if(!obj[D_JSON_SAT].isNull()){    // Assume range 0-100
    uint8_t sat = obj[D_JSON_SAT];
    sat = sat > 100 ? 100 : sat;
    sat = map(sat, 0,100, 0,255);
    changeHSB_Sat(sat);
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_SAT,sat);
    #endif // ENABLE_LOG_LEVEL_INFO_PARSING
    data_buffer.isserviced++;
  }

  if(!obj[D_JSON_BRT].isNull()){    // Assume range 0-100
    uint8_t brt = obj[D_JSON_BRT];
    brt = brt > 100 ? 100 : brt;
    brt = map(brt, 0,100, 0,255);
    changeHSB_Brt(brt);
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_BRT,brt);
    #endif // ENABLE_LOG_LEVEL_INFO_PARSING
    data_buffer.isserviced++;
  }
  

  if(!obj[D_JSON_BRT_RGB].isNull()){    // Assume range 0-100
    uint8_t brt = obj[D_JSON_BRT_RGB];
    brt = brt > 100 ? 100 : brt;
    brt = map(brt, 0,100, 0,255);
    changeBriRGB(brt);
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_BRT_RGB,brt);
    #endif //#ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    data_buffer.isserviced++;
  }

  if(!obj[D_JSON_BRT_CCT].isNull()){    // Assume range 0-100
    uint8_t brt = obj[D_JSON_BRT_CCT];
    brt = brt > 100 ? 100 : brt;
    brt = map(brt, 0,100, 0,255);
    // setBriCT(brt);
    changeBriCT(brt);
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_BRT_CCT,brt);
    #endif //#ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    data_buffer.isserviced++;
  }

  if(!obj[D_JSON_CCT_PERCENTAGE].isNull()){    // Assume range 0-100
    uint16_t cct_value = obj[D_JSON_CCT_PERCENTAGE];
    LightSetColorTemp(cct_value);
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_CCT_PERCENTAGE,cct_value);
    #endif //#ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    data_buffer.isserviced++;
  }

  if(!obj[D_JSON_CCT_TEMP].isNull()){    // Assume range 0-100
    uint16_t cct_value = obj[D_JSON_CCT_TEMP];
    LightSetColorTemp(cct_value);
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_CCT_TEMP,cct_value);
    #endif // #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    data_buffer.isserviced++;
  }

  if(!obj[D_JSON_RGBCCT_LINKED].isNull()){    // Assume range 0-100
    uint8_t value = obj[D_JSON_RGBCCT_LINKED];
    setCTRGBLinked(value);
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_RGBCCT_LINKED,value);
    #endif // #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    data_buffer.isserviced++;
  } 

  // if(!obj[D_JSON_RGB].isNull()){
  //   const char* rgbpacked = obj[D_JSON_RGB];
  //   uint32_t colour32bit = 0;
  //   if(rgbpacked[0]=='#'){ colour32bit = (long) strtol( &rgbpacked[1], NULL, 16);
  //   }else{ colour32bit = (long) strtol( &rgbpacked[0], NULL, 16); }
  //   RgbColor rgb;
  //   rgb.R = colour32bit >> 16; //RGB
  //   rgb.G = colour32bit >> 8 & 0xFF; //RGB
  //   rgb.B = colour32bit & 0xFF; //RGB
  //   scene.colour = HsbColor(RgbColor(rgb.R,rgb.G,rgb.B));
  //   #ifdef ENABLE_LOG_LEVEL_INFO
  //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_RGB ":%s " D_NEOPIXEL_RGB ":%d,%d,%d " D_NEOPIXEL_HSB ":%d,%d,%d"),
  //     rgbpacked,rgb.R,rgb.G,rgb.B,scene.colour.H,scene.colour.S,scene.colour.B);
  //   #endif
  // }

  if(obj.containsKey(D_JSON_SCENE_COLOUR)){  //set hsb   D_JSON_HSB
    // Check HSB format
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    // AddLog_P(LOG_LEVEL_INFO, PSTR("[D_JSON_SCENE_COLOUR]"));
    #endif
    if(!obj[D_JSON_SCENE_COLOUR][D_JSON_HSB].isNull()){
      JsonArrayConst colourarray = obj[D_JSON_SCENE_COLOUR][D_JSON_HSB];
      uint8_t index = 0;
      for(JsonVariantConst v : colourarray) {
        int val = v.as<int>();
        #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
        AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR("[D_JSON_SCENE_COLOUR][D_JSON_HSB]=%d"),val);
        #endif
        switch(index++){
          case 0: changeHSB_Hue(val); break;
          case 1: changeHSB_Sat(val); break;
          case 2: changeHSB_Brt(val); break;
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
  }
  // #endif //ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT


  // TIME with different units
  if(!obj[D_JSON_TIME].isNull()){ //default to secs /// D_JSON_TRANSITIONTIME
    animation.transition.time_ms.val = obj[D_JSON_TIME];
    animation.transition.time_ms.val *= 1000;
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_TIME "%d" D_UNIT_MILLISECOND),animation.transition.time_ms.val);  
    #endif
  }else
  if(!obj[D_JSON_TIME_SECS].isNull()){
    animation.transition.time_ms.val = obj[D_JSON_TIME_SECS];
    animation.transition.time_ms.val *= 1000;
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_TIME "%d" D_UNIT_MILLISECOND),animation.transition.time_ms.val);  
    #endif
  }else
  if(!obj[D_JSON_TIME_MS].isNull()){
    animation.transition.time_ms.val = obj[D_JSON_TIME_MS];
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_TIME "%d" D_UNIT_MILLISECOND),animation.transition.time_ms.val);  
    #endif
  }


  // TIME on duration for autooff
  if(!obj[D_JSON_TIME_ON].isNull()){ //default to secs
    auto_time_off_secs = obj[D_JSON_TIME_ON];
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_TIME "%d" D_UNIT_MILLISECOND),auto_time_off_secs);  
    #endif
  }else
  if(!obj[D_JSON_TIME_ON_SECS].isNull()){
    auto_time_off_secs = obj[D_JSON_TIME_ON_SECS];
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_TIME "%d" D_UNIT_MILLISECOND),auto_time_off_secs);  
    #endif
  }else
  if(!obj[D_JSON_TIME_ON_MS].isNull()){
    auto_time_off_secs = obj[D_JSON_TIME_ON_MS];
    auto_time_off_secs /= 1000;
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_TIME "%d" D_UNIT_MILLISECOND),auto_time_off_secs);  
    #endif
  }

    
  if(!obj[D_JSON_SCENENAME].isNull()){ 
    const char* mode_singlecolourctr = obj[D_JSON_SCENENAME];
    if((tmp_id=GetSceneIDbyName(mode_singlecolourctr))>=0){
      mode_singlecolour.name_id = tmp_id;
      animation.mode_id = ANIMATION_MODE_SCENE_ID; //#Idea. make optional

      // if(mode_singlecolour.name_id == MODE_SINGLECOLOUR_PALETTE_SINGLE_ID){
      //   mode_singlecolour.parts = 0;
      // }
      mode_singlecolour.parts = 0;

      //#ifdef ENABLE_LOG_LEVEL_INFO_PARSING
      char buffer[30];
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_SCENENAME,GetSceneName(buffer, sizeof(buffer)));
      //#endif
      data_buffer.isserviced++;
    }else{
      #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
      AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_SCENENAME,mode_singlecolourctr);
      #endif // #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    }
  }


  if(!obj[D_JSON_LIGHTPOWER].isNull()){ 
    const char* state_ctr = obj[D_JSON_LIGHTPOWER];

    int8_t state = pCONT_sup->GetStateNumber(state_ctr);
    state = pCONT_sup->ConvertStateNumberIfToggled(state, light_power);
    
    switch(state){
      case STATE_NUMBER_ON_ID:

        // #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
        // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED "\"onoff\"=\"ON\""));
        // #endif
        #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
        AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "MODE_TURN_ON_ID"));
        #endif

        // Add this as "SAVE" state then "LOAD" state
        // memcpy(&animation,&animation_stored,sizeof(animation));// RESTORE copy of state

        SetAnimationProfile(ANIMATION_PROFILE_TURN_ON_ID);
        light_power = true;

        //animation.mode_id = MODE_TURN_ON_ID;
        data_buffer.isserviced++;
        
      break;
      case STATE_NUMBER_OFF_ID:

        // #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
        // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED "\"onoff\"=\"OFF\""));
        // #endif
        #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
        AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "MODE_TURN_OFF_ID"));
        #endif
        
        // memcpy(&animation_stored,&animation,sizeof(animation)); // STORE copy of state
        
        SetAnimationProfile(ANIMATION_PROFILE_TURN_OFF_ID);
        light_power = false;

        //animation.mode_id = MODE_TURN_OFF_ID;
        data_buffer.isserviced++;

      break;
    } // END switch

  }


  if(obj.containsKey(D_JSON_COLOUR_PALETTE)){ 

    const char* colour = obj[D_JSON_COLOUR_PALETTE];
    if((tmp_id=GetPaletteIDbyName(colour))>=0){
      animation.palette_id = tmp_id;

      #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
      AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR("GetPaletteIDbyName=%d"),tmp_id);
      #endif // #ifdef ENABLE_LOG_LEVEL_INFO_PARSING

      #ifdef ENABLE_PIXEL_FUNCTION_MIXER
      if(animation.mode_id == ANIMATION_MODE_FLASHER_ID){
        flashersettings.region = FLASHER_REGION_COLOUR_SELECT_ID; //update colours in use
      }
      #endif
      #ifdef ENABLE_PALETTE_FORCED_MODE
      //  animation.mode_id = ANIMATION_MODE_PRESETS_ID;
      #endif
      data_buffer.isserviced++;
    }else{
      uint8_t colour = obj[D_JSON_COLOUR_PALETTE];
      animation.palette_id = colour < PALETTELIST_STATIC_LENGTH_ID ? colour : 0;
      #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
        AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_NOMATCH D_JSON_COMMAND_NVALUE),D_JSON_COLOUR_PALETTE,animation.palette_id);
        AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_NOMATCH D_JSON_COMMAND_SVALUE),D_JSON_COLOUR_PALETTE,GetPaletteNameByID(animation.palette_id, buffer));
      #endif // #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    }

  }

    
  if(!obj[D_JSON_ANIMATIONMODE].isNull()){ 
    const char* mode = obj[D_JSON_ANIMATIONMODE];
    if((tmp_id=GetAnimationModeIDbyName(mode))>=0){
      animation.mode_id = tmp_id;
      #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
      AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_ANIMATIONMODE,GetAnimationModeName(buffer, sizeof(buffer)));
      #endif
      // Response_mP(S_JSON_COMMAND_SVALUE,D_JSON_ANIMATIONMODE,GetAnimationModeName());
      data_buffer.isserviced++;
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
      data_buffer.isserviced++;
    }else{
      #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
      AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_MODE,mode);
      #endif // #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    }
  }


  if(!obj[D_JSON_STRIP_SIZE].isNull()){ 
    int amount = obj[D_JSON_STRIP_SIZE];
    // Also convert to percentage equivalent
    #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
    pCONT_ladd->strip_size = amount;
    #endif // USE_MODULE_LIGHTS_ADDRESSABLE
    pCONT_iLight->light_count = amount;
    #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_STRIP_SIZE,pCONT_ladd->strip_size);
    #endif
    #endif //    #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
    // Response_mP(S_JSON_COMMAND_SVALUE_NVALUE,D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE,animation.transition.pixels_to_update_as_percentage.val);
    data_buffer.isserviced++;
  }
  
  
// DEBUG_LINE;

//   // struct TRANSITIONSETTINGS

//   if(!obj[D_JSON_TRANSITION][D_JSON_PIXELS_UPDATE_NUMBER].isNull()){ 
//     int amount = obj[D_JSON_TRANSITION][D_JSON_PIXELS_UPDATE_NUMBER];
//     // Also convert to percentage equivalent
//     animation.transition.pixels_to_update_as_percentage.val = GetPixelsToUpdateAsPercentageFromNumber(amount);
//     #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
//     AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE,animation.transition.pixels_to_update_as_percentage.val);
//     #endif
//     // Response_mP(S_JSON_COMMAND_SVALUE_NVALUE,D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE,animation.transition.pixels_to_update_as_percentage.val);
//     data_buffer.isserviced++;
//   }else
//   if(!obj[D_JSON_TRANSITION][D_JSON_PIXELS_UPDATE_PERCENTAGE].isNull()){ 
//     int percentage = obj[D_JSON_TRANSITION][D_JSON_PIXELS_UPDATE_PERCENTAGE];
//     animation.transition.pixels_to_update_as_percentage.val = constrain(percentage,0,100);
//     #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
//     AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE,animation.transition.pixels_to_update_as_percentage.val);
//     #endif
//     Response_mP(S_JSON_COMMAND_SVALUE_NVALUE,D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE,animation.transition.pixels_to_update_as_percentage.val);
//     // Also convert to number equivalent
//     data_buffer.isserviced++;
//   }

// DEBUG_LINE;

  
//   // Transition time in seconds or ms
//   if(!obj[D_JSON_TRANSITION][D_JSON_TIME].isNull()){ 
//     animation.transition.time_ms.val = obj[D_JSON_TRANSITION][D_JSON_TIME];
//     #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
//     AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_TIME,animation.transition.time_ms.val);   
//     #endif
//     animation.transition.time_ms.val *= 1000; // map into ms
//     #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
//     AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_TIME,animation.transition.time_ms.val);
//     #endif
//     Response_mP(S_JSON_COMMAND_NVALUE, D_JSON_TIME_MS,animation.transition.time_ms.val);
//     data_buffer.isserviced++;
//     //check that rate>animation time
//     if(animation.transition.rate_ms.val<animation.transition.time_ms.val){ animation.transition.time_ms.val = animation.transition.rate_ms.val;}
//   }else
//   if(!obj[D_JSON_TRANSITION][D_JSON_TIME_MS].isNull()){ 
//     animation.transition.time_ms.val = obj[D_JSON_TRANSITION][D_JSON_TIME_MS];
//     #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
//     AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_TIME_MS,animation.transition.time_ms.val);
//     #endif
//     Response_mP(S_JSON_COMMAND_NVALUE, D_JSON_TIME_MS,animation.transition.time_ms.val);
//     data_buffer.isserviced++;
//     //check that rate>animation time
//     if(animation.transition.rate_ms.val<animation.transition.time_ms.val){ animation.transition.time_ms.val = animation.transition.rate_ms.val;}
//   }

// DEBUG_LINE;

  // Rate in seconds as default or ms
  if(!obj[D_JSON_TRANSITION][D_JSON_RATE].isNull()){ 
    animation.transition.rate_ms.val = obj[D_JSON_TRANSITION][D_JSON_RATE];
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_RATE,animation.transition.time_ms.val);
    #endif
    animation.transition.rate_ms.val *= 1000; //seconds to milliseconds
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_RATE,animation.transition.time_ms.val);
    Response_mP(S_JSON_COMMAND_NVALUE, D_JSON_TIME_MS,animation.transition.rate_ms.val);
    #endif // #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    data_buffer.isserviced++;
    //check that rate>animation time
    if(animation.transition.rate_ms.val<animation.transition.time_ms.val){ animation.transition.time_ms.val = animation.transition.rate_ms.val;}
  }else
  if(!obj[D_JSON_TRANSITION][D_JSON_RATE_MS].isNull()){ 
    animation.transition.rate_ms.val = obj[D_JSON_TRANSITION][D_JSON_RATE_MS];
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE),D_JSON_TRANSITION,D_JSON_RATE_MS,animation.transition.rate_ms.val);
    #endif
    Response_mP(S_JSON_COMMAND_NVALUE, D_JSON_TIME_MS,animation.transition.rate_ms.val);
    data_buffer.isserviced++;
    //check that rate>animation time
    if(animation.transition.rate_ms.val<animation.transition.time_ms.val){ animation.transition.time_ms.val = animation.transition.rate_ms.val;}
  }

  // probably bring transition_order into one command
  if(!obj[D_JSON_TRANSITION][D_JSON_ORDER].isNull()){ // ie when also doing palette single, randomise the order
    const char* order = obj[D_JSON_TRANSITION][D_JSON_ORDER];
    if((tmp_id=GetTransitionOrderIDbyName(order))>=0){
      animation.transition.order_id = tmp_id;
      // AddLog_P(LOG_LEVEL_INFO_PARSING, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_SVALUE),D_JSON_TRANSITION,D_JSON_ORDER,GetTransitionOrderName());
      // Response_mP(S_JSON_COMMAND_SVALUE,D_JSON_ORDER,GetTransitionOrderName());
      data_buffer.isserviced++;
    }else{
    #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_NOMATCH D_JSON_COMMAND_SVALUE_SVALUE),D_JSON_TRANSITION,D_JSON_ORDER,order);
    #endif
    }
  }
  

//   if(!obj[D_JSON_PIXELSGROUPED].isNull()){
//     animation.pixelgrouped = obj[D_JSON_PIXELSGROUPED];
//     #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
//     AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_PIXELSGROUPED,animation.pixelgrouped);
//     #endif
//   }

//   //override commands that run for one animation cycle then are cleared to 0
//   if(!obj[D_JSON_REFRESH_ALLPIXELS].isNull()){
//     animation_override.fRefreshAllPixels = obj[D_JSON_REFRESH_ALLPIXELS];
//     #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
//     AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_REFRESH_ALLPIXELS,animation_override.fRefreshAllPixels);
//     #endif
//   }

  mode_singlecolour.flag.generate_new_colours = true;
  mqtthandler_debug_teleperiod.flags.SendNow = true;
  animation.flags.fForceUpdate = true;

}



void mInterfaceLight::EveryLoop(){
      
  // AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "settings.type == EveryLoop"));
  SubTask_AutoOff();

  // if(animation.flags.fForceUpdate){
  //  if(animation.mode_id == ANIMATION_MODE_SCENE_ID){ //if scene, with colour change push output (change this to be a flag "animation.force" then do this check)
  //     mode_singlecolour.name_id = MODE_SINGLECOLOUR_COLOURSCENE_ID;
  //   }
  // }


  // Check if update is needed, ONLY update output, not new numbers
  // if(!memcmp(last_colour, current_color, sizeof(last_colour))){
  //   // memcpy(last_colour, current_color, sizeof(last_colour)); //temp fix
  //   AddLog_P(LOG_LEVEL_INFO, PSTR("!memcmp(last_colour, current_color, 5)"));
  //   animation.flags.fForceUpdate = true;
  //   if(!mode_singlecolour.flag.running){ 
  //     mode_singlecolour.flag.running = true; 
  //     AddLog_P(LOG_LEVEL_INFO, PSTR("mode_singlecolour.flag.running"));
  //   }
  // }

  //   // If not running, I need to run this to
  //   if(!mode_singlecolour.flag.running){ mode_singlecolour.flag.running = true; }
  //   animation_override.time_ms = 1;
  //   //based on current mode, other flags need to be set
  //   if(animation.mode_id == ANIMATION_MODE_SCENE_ID){      
  //     mode_singlecolour.update = true;
  //     //mode_singlecolour.name_id = MODE_SINGLECOLOUR_COLOURSCENE_ID;
  //   }
  // }

  // Single colour methods
  if((pCONT_set->Settings.light_settings.type < LT_LIGHT_INTERFACE_END)||
     (pCONT_set->Settings.light_settings.type == LT_WS2812)){
     
    switch(animation.mode_id){
      case ANIMATION_MODE_SCENE_ID:
        SubTask_SingleColour();             //scenes to be moved into interface
        
        //CHANGE TO LIGHTPOWER
        light_power = true;

        //AddLog_P(LOG_LEVEL_DEBUG, PSTR("ANIMATION_MODE_SCENE_ID"));
      break;
      // case ANIMATION_MODE_AMBILIGHT_ID:
      //   SubTask_Ambilight();
      //   light_power = true;
      // break;
      case ANIMATION_MODE_NONE_ID: default: break; // resting position
    }
  
  }

  if(pCONT_set->Settings.light_settings.type == LT_WS2812){ 
    
    #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
    switch(animation.mode_id){
      case ANIMATION_MODE_FLASHER_ID:
        // AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "ANIMATION_MODE_FLASHER_ID"));
        pCONT_ladd->SubTask_Flasher_Main();
        light_power = true;
      break;
      #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
      case ANIMATION_MODE_NOTIFICATIONS_ID:
        SubTask_NotificationPanel();
        light_power = true;
      break;
      #endif
      case ANIMATION_MODE_NONE_ID: default: break; // resting position
    }
    #endif
      
  }
  // else
  // {
  //   AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("Invalid Light Type"));
  // }

  // Clear any flags now it should have been handled
  // if(animation.flags.fForceUpdate){
  //   animation.flags.fForceUpdate = false;
  // }

} // END everyloop



void mInterfaceLight::SubTask_AutoOff(){
  if(mTime::TimeReached(&tSavedAutoOff,1000)){
    //AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "scene.tSavedAutoOff [%d]"),animation.auto_time_off_secs);
    if(auto_time_off_secs==1){ //if =1 then turn off and clear to 0
      // animation.name_id = MODE_SINGLECOLOUR_FADE_OFF_ID;
      #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "animation.auto_time_off_secs==1 and disable"));
      #endif       

      SetAnimationProfile(ANIMATION_PROFILE_TURN_OFF_ID);
      //#ifdef ENABLE_LOG_LEVEL_INFO
      auto_time_off_secs=0;
      //#endif
    }else
    if(auto_time_off_secs>1){ //if =1 then turn off and clear to 0
      auto_time_off_secs--; //decrease
      
      #ifdef ENABLE_LOG_LEVEL_INFO_PARSING
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "auto_time_off_secs=%d dec"),auto_time_off_secs);
      #endif
    }
  }
}// END SubTask_AutoOff


/*******************************************************************************************************************
********************************************************************************************************************
************MQTT**************************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/



void mInterfaceLight::ApplyGlobalBrightnesstoColour(RgbcctColor* colour){

  colour->R  = mSupport::changeUIntScale(colour->R,  0, 255, 0, _briRGB);
  colour->G  = mSupport::changeUIntScale(colour->G,  0, 255, 0, _briRGB);
  colour->B  = mSupport::changeUIntScale(colour->B,  0, 255, 0, _briRGB);
  colour->WW = mSupport::changeUIntScale(colour->WW, 0, 255, 0, _briCT);
  colour->WC = mSupport::changeUIntScale(colour->WC, 0, 255, 0, _briCT);

}



const char* mInterfaceLight::GetTransitionMethodName(char* buffer){
  return GetTransitionMethodNameByID(pCONT_iLight->animation.transition.method_id,buffer);
}
const char* mInterfaceLight::GetTransitionMethodNameByID(uint8_t id, char* buffer){
  switch(id){ default:   
    case TRANSITION_METHOD_NONE_ID: memcpy_P(buffer,PM_TRANSITION_METHOD_NONE_NAME_CTR,sizeof(PM_TRANSITION_METHOD_NONE_NAME_CTR)); break; // smooth shift between them
    case TRANSITION_METHOD_BLEND_ID: memcpy_P(buffer,PM_TRANSITION_METHOD_BLEND_NAME_CTR,sizeof(PM_TRANSITION_METHOD_BLEND_NAME_CTR)); break; 
    case TRANSITION_METHOD_INSTANT_ID: memcpy_P(buffer,PM_TRANSITION_METHOD_INSTANT_NAME_CTR,sizeof(PM_TRANSITION_METHOD_INSTANT_NAME_CTR)); break;  // smooth shift between them
    case TRANSITION_METHOD_TWINKLE_ID: memcpy_P(buffer,PM_TRANSITION_METHOD_TWINKLE_NAME_CTR,sizeof(PM_TRANSITION_METHOD_TWINKLE_NAME_CTR));  break; // smooth shift between them
    case TRANSITION_METHOD_GLIMMER_ID: memcpy_P(buffer,PM_TRANSITION_METHOD_GLIMMER_NAME_CTR,sizeof(PM_TRANSITION_METHOD_GLIMMER_NAME_CTR));  break; // smooth shift between them
  }
  return buffer;
}
int8_t mInterfaceLight::GetTransitionMethodIDbyName(const char* c){
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



const char* mInterfaceLight::GetTransitionOrderName(char* buffer){
  return GetTransitionOrderNameByID(pCONT_iLight->animation.transition.order_id, buffer);
}
const char* mInterfaceLight::GetTransitionOrderNameByID(uint8_t id, char* buffer){
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
int8_t mInterfaceLight::GetTransitionOrderIDbyName(const char* c){
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







void mInterfaceLight::SetAnimationProfile(uint8_t profile_id){
  #ifdef ENABLE_LOG_LEVEL_DEBUG
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::SetAnimationProfile" " START"));
  #endif
          
  switch(profile_id){
    case ANIMATION_PROFILE_TURN_OFF_ID:

      // Remember previous state for returning to later  
      memcpy(&animation_stored,&animation,sizeof(animation)); // STORE copy of state
      light_power = false;

      //if palette, set colour to black and update all
      switch(animation.mode_id){
        // case ANIMATION_MODE_FLASHER_ID://PRESETS_ID:
        //   #ifdef ENABLE_LOG_LEVEL_DEBUG
        //   AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::SetAnimationProfile" "ANIMATION_MODE_FLASHER_ID"));
        //   #endif
        //   animation_override.time_ms = 1000; //force fast rate to turn on
        //   flashersettings.function = FLASHER_FUNCTION_SLOW_GLOW_ID;
        //   animation.flags.fForceUpdate = true;
        //   animation.brightness = 0;
        //   first_set = 1;//set all
        // break;
        default:
          #ifdef ENABLE_LOG_LEVEL_DEBUG
          AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::SetAnimationProfile" "default"));
          #endif
        case ANIMATION_MODE_SCENE_ID:
          #ifdef ENABLE_LOG_LEVEL_DEBUG
          AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::SetAnimationProfile" "ANIMATION_MODE_SCENE_ID"));
          #endif
          // setBriRGB(0);
          // animation.brightness = 0;
          animation.transition.time_ms.val = 1000;
          animation.flags.fForceUpdate = true;
          animation_override.time_ms = 1000; //force fast rate to turn on

          changeBri(0);

          mode_singlecolour.name_id = MODE_SINGLECOLOUR_COLOURSCENE_ID;
          animation.mode_id = ANIMATION_MODE_SCENE_ID;
          
          // first_set = 1;//set all
        break;
      }
      animation.flags.fEndUpdatesWhenAnimationCompletes = true; //once turned off, stop animations
    break;
    case ANIMATION_PROFILE_TURN_ON_ID:

        memcpy(&animation,&animation_stored,sizeof(animation));// RESTORE copy of state
        light_power = true;

        switch(animation.mode_id){
          // case ANIMATION_MODE_PRESETS_ID:
          
          // break;
          default:
          case ANIMATION_MODE_SCENE_ID:
            mode_singlecolour.name_id = MODE_SINGLECOLOUR_COLOURSCENE_ID;
            animation.mode_id = ANIMATION_MODE_SCENE_ID;
          break;
        }

        animation_override.time_ms = 1000; //force fast rate to turn on
        // animation.brightness = 1;
        changeBri(255);
        animation.flags.fForceUpdate = true;
        // pCONT_ladd->first_set = 1;//set all
    break;
    case ANIMATION_PROFILE_NOTHING_ID:

    break;
  }

}// END SetAnimationProfile function







const char* mInterfaceLight::GetHardwareColourTypeName(char* buffer){
  return GetHardwareColourTypeNameByID(pCONT_iLight->animation.mode_id, buffer);
}
const char* mInterfaceLight::GetHardwareColourTypeNameByID(uint8_t id, char* buffer){
  switch(id){
    default:
    case PIXEL_HARDWARE_COLOR_ORDER_GRB_ID: memcpy_P(buffer, PM_PIXEL_HARDWARE_COLOR_ORDER_GRB_CTR, sizeof(PM_PIXEL_HARDWARE_COLOR_ORDER_GRB_CTR)); break;
    case PIXEL_HARDWARE_COLOR_ORDER_RGB_ID: memcpy_P(buffer, PM_PIXEL_HARDWARE_COLOR_ORDER_RGB_CTR, sizeof(PM_PIXEL_HARDWARE_COLOR_ORDER_RGB_CTR)); break;
    case PIXEL_HARDWARE_COLOR_ORDER_BRG_ID: memcpy_P(buffer, PM_PIXEL_HARDWARE_COLOR_ORDER_BRG_CTR, sizeof(PM_PIXEL_HARDWARE_COLOR_ORDER_BRG_CTR)); break;
    case PIXEL_HARDWARE_COLOR_ORDER_RBG_ID: memcpy_P(buffer, PM_PIXEL_HARDWARE_COLOR_ORDER_RBG_CTR, sizeof(PM_PIXEL_HARDWARE_COLOR_ORDER_RBG_CTR)); break;
  }
  return buffer;
}
int8_t mInterfaceLight::GetHardwareColourTypeIDbyName(const char* c){
  if(!c){ return -1; }
  //3 PIXEL TYPE
  if(     strcmp_P(c,PM_PIXEL_HARDWARE_COLOR_ORDER_GRB_CTR)==0){ return PIXEL_HARDWARE_COLOR_ORDER_GRB_ID; }
  else if(strcmp_P(c,PM_PIXEL_HARDWARE_COLOR_ORDER_RGB_CTR)==0){ return PIXEL_HARDWARE_COLOR_ORDER_RGB_ID; }
  else if(strcmp_P(c,PM_PIXEL_HARDWARE_COLOR_ORDER_BRG_CTR)==0){ return PIXEL_HARDWARE_COLOR_ORDER_BRG_ID; }
  else if(strcmp_P(c,PM_PIXEL_HARDWARE_COLOR_ORDER_RBG_CTR)==0){ return PIXEL_HARDWARE_COLOR_ORDER_RBG_ID; }   

  return -1;
}




float mInterfaceLight::HueN2F(uint16_t hue){
  return hue/360.0f;
}
// S at100toFloat
float mInterfaceLight::SatN2F(uint8_t sat){
  return sat/100.0f;
}
// B rt100toFloat
float mInterfaceLight::BrtN2F(uint8_t brt){
  return brt/100.0f;
}

// H ueFloatto360
uint16_t mInterfaceLight::HueF2N(float hue){
  return round(hue*360.0f);
}
// S atFloatto100
uint8_t mInterfaceLight::SatF2N(float sat){
  return round(sat*100.0f);
}
// B rtFloatto100
uint8_t mInterfaceLight::BrtF2N(float brt){
  return round(brt*100.0f);
}



// Generate random colour between two hsb colours
RgbTypeColor mInterfaceLight::GetRandomColour(RgbTypeColor colour1, RgbTypeColor colour2){
  // int random_hue = random(HueF2N(colour1.H),HueF2N(colour2.H));
  // int random_sat = random(SatF2N(colour1.S),SatF2N(colour2.S));
  // int random_brt = random(BrtF2N(colour1.B),BrtF2N(colour2.B));
  return RgbTypeColor(0);//HueN2F(random_hue),SatN2F(random_sat),BrtN2F(random_brt));
}








const char* mInterfaceLight::GetAnimationModeName(char* buffer, uint16_t buflen){
  return GetAnimationModeNameByID(animation.mode_id, buffer, buflen);
}
const char* mInterfaceLight::GetAnimationModeNameByID(uint8_t id, char* buffer, uint16_t buflen){
  switch(id){
    default:
    case ANIMATION_MODE_NONE_ID:          memcpy_P(buffer, PM_ANIMATION_MODE_NONE_NAME_CTR , sizeof(PM_ANIMATION_MODE_NONE_NAME_CTR)); break;
    case ANIMATION_MODE_TURN_ON_ID:          memcpy_P(buffer, PM_ANIMATION_MODE_TURN_ON_NAME_CTR , sizeof(PM_ANIMATION_MODE_TURN_ON_NAME_CTR)); break;
    case ANIMATION_MODE_TURN_OFF_ID:          memcpy_P(buffer, PM_ANIMATION_MODE_TURN_OFF_NAME_CTR , sizeof(PM_ANIMATION_MODE_TURN_OFF_NAME_CTR)); break;
    case ANIMATION_MODE_AMBILIGHT_ID:     memcpy_P(buffer, PM_ANIMATION_MODE_AMBILIGHT_NAME_CTR, sizeof(PM_ANIMATION_MODE_AMBILIGHT_NAME_CTR)); break;
    #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
      case ANIMATION_MODE_NOTIFICATIONS_ID: memcpy_P(buffer, PM_ANIMATION_MODE_NOTIFICATIONS_NAME_CTR,sizeof(PM_ANIMATION_MODE_NOTIFICATIONS_NAME_CTR)); break;
    #endif
    case ANIMATION_MODE_FLASHER_ID:       memcpy_P(buffer, PM_ANIMATION_MODE_FLASHER_NAME_CTR, sizeof(PM_ANIMATION_MODE_FLASHER_NAME_CTR)); break;
    case ANIMATION_MODE_SCENE_ID:         memcpy_P(buffer, PM_ANIMATION_MODE_SCENE_NAME_CTR , sizeof(PM_ANIMATION_MODE_SCENE_NAME_CTR)); break;
    }
  return buffer;
} 
int8_t mInterfaceLight::GetAnimationModeIDbyName(const char* c){
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



void mInterfaceLight::changeHSB_Hue(uint16_t hue_new){
  uint16_t hue = 0;
  uint8_t  sat = 0;
  uint8_t  brt = 0;
  getHSB(&hue,&sat,&brt);  
  changeHSB(hue_new, sat, brt);
}


void mInterfaceLight::changeHSB_Sat(uint8_t sat_new){
  uint16_t hue = 0;
  uint8_t  sat = 0;
  uint8_t  brt = 0;
  getHSB(&hue,&sat,&brt);
  changeHSB(hue, sat_new, brt);
}


void mInterfaceLight::changeHSB_Brt(uint8_t brt_new){
  setBriRGB(brt_new);
}


uint8_t mInterfaceLight::ConstructJSON_Settings(uint8_t json_method){

  char buffer[30];
  
  JsonBuilderI->Start();

  JsonBuilderI->Add(D_JSON_TYPE, pCONT_set->Settings.light_settings.type);
  
  JsonBuilderI->Add(D_JSON_HUE, getHue());
  JsonBuilderI->Add(D_JSON_SAT, getSat());

  JsonBuilderI->Add("briRGB", getBriRGB());
  JsonBuilderI->Add("briCT", getBriCT());
  
  //   _subtype,
  //   _ct,
  //   scene.colour.WC,
  //   scene.colour.WW,
  //   _briCT,
  //   _color_mode,
  //   _ct_rgb_linked,
  //   fade.running,
  //   fade.start_ms,
  //   fade.duration_ms,
  //   fade.end_ms, //   pCONT_lPWM->fade_duration,
  //   pCONT_lPWM->fade_start,
  //   pwm_min,
  //   pwm_max,
  //   _pwm_multi_channels,
  //   _ct_min_range,
  //   _ct_max_range,
  // );
  return JsonBuilderI->End();

}

uint8_t mInterfaceLight::ConstructJSON_Debug(uint8_t json_method){

  char buffer[30];
  
  JsonBuilderI->Start();

  JsonBuilderI->Level_Start("singlecolour");
    JsonBuilderI->Add("R", mode_singlecolour.colour.R);
    JsonBuilderI->Add("G", mode_singlecolour.colour.G);
    JsonBuilderI->Add("B", mode_singlecolour.colour.B);
    JsonBuilderI->Add("WW", mode_singlecolour.colour.WW);
    JsonBuilderI->Add("WC", mode_singlecolour.colour.WC);
    JsonBuilderI->Add("WC", mode_singlecolour.colour.WC);
  JsonBuilderI->Level_End();
  JsonBuilderI->Level_Start("tasint_colour");
    JsonBuilderI->Add("R", tasint_colour.R);
    JsonBuilderI->Add("G", tasint_colour.G);
    JsonBuilderI->Add("B", tasint_colour.B);
    JsonBuilderI->Add("WW", tasint_colour.WW);
    JsonBuilderI->Add("WC", tasint_colour.WC);
    JsonBuilderI->Add("WC", tasint_colour.WC);
  JsonBuilderI->Level_End();


  JsonBuilderI->Array_AddArray("singlecolour", current_color, 5);   


  JsonBuilderI->Level_Start("fade");
    JsonBuilderI->Add("running", fade.running);
  JsonBuilderI->Level_End();
  
  //   _sat,
  //   _briRGB,
  //   scene.colour.R,
  //   scene.colour.G,
  //   scene.colour.B,
  //   _subtype,
  //   _ct,
  //   scene.colour.WC,
  //   scene.colour.WW,
  //   _briCT,
  //   _color_mode,
  //   _ct_rgb_linked,
  //   fade.start_ms,
  //   fade.duration_ms,
  //   fade.end_ms,
  //   entry_color[0],entry_color[1],entry_color[2],entry_color[3],entry_color[4],
  //   current_color[0],current_color[1],current_color[2],current_color[3],current_color[4],
  //   new_color[0],new_color[1],new_color[2],new_color[3],new_color[4],
  //   last_color[0],last_color[1],last_color[2],last_color[3],last_color[4],
  //   // color_remap[0],color_remap[1],color_remap[2],color_remap[3],color_remap[4],
  //   pCONT_lPWM->fade_start_10[0],pCONT_lPWM->fade_start_10[1],pCONT_lPWM->fade_start_10[2],pCONT_lPWM->fade_start_10[3],pCONT_lPWM->fade_start_10[4],
  //   pCONT_lPWM->fade_cur_10[0],pCONT_lPWM->fade_cur_10[1],pCONT_lPWM->fade_cur_10[2],pCONT_lPWM->fade_cur_10[3],pCONT_lPWM->fade_cur_10[4],
  //   pCONT_lPWM->fade_end_10[0],pCONT_lPWM->fade_end_10[1],pCONT_lPWM->fade_end_10[2],pCONT_lPWM->fade_end_10[3],pCONT_lPWM->fade_end_10[4],
  //   pCONT_lPWM->fade_duration,
  //   pCONT_lPWM->fade_start,
  //   pwm_min,
  //   pwm_max,
  //   _pwm_multi_channels,
  //   _ct_min_range,
  //   _ct_max_range,
  //   pCONT_set->Settings.light_settings.type
  // );
  return JsonBuilderI->End();

}






////////////////////// START OF MQTT /////////////////////////

void mInterfaceLight::MQTTHandler_Init(){

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

  uint8_t mqtthandler_list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID, MQTT_HANDLER_MODULE_SCENE_TELEPERIOD_ID, MQTT_HANDLER_MODULE_DEBUG_PARAMETERS_TELEPERIOD_ID
  };
  
  struct handler<mInterfaceLight>* mqtthandler_list_ptr[] = {
    &mqtthandler_settings_teleperiod, &mqtthandler_scene_teleperiod, &mqtthandler_debug_teleperiod
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, D_MODULE_LIGHTS_INTERFACE_ID,
    mqtthandler_list_ptr, mqtthandler_list_ids, 
    sizeof(mqtthandler_list_ids)/sizeof(mqtthandler_list_ids[0]),
    mqtt_handler_id
  );

}







/****
 * 
 * 
 * 
 * 
 * NOT NEEDED
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * */


void mInterfaceLight::LightSetPower(void)
{
// //  power = XdrvMailbox.index;
//   old_power = power;
//   //power = bitRead(XdrvMailbox.index, device -1);
//   uint32_t mask = 1;  // default mask
//   if (pwm_multi_channels) {
//     mask = (1 << subtype) - 1;   // wider mask
//   } else if (!light_controller.isCTRGBLinked()) {
//     mask = 3;   // we got 2 devices, for RGB and White
//   }
//   uint32_t shift = device - 1;
//   // If PWM multi_channels
//   // Ex: 3 Relays and 4 PWM - devices_present = 7, device = 4, subtype = 4
//   // Result: mask = 0b00001111 = 0x0F, shift = 3.
//   // Power bits we consider are: 0b01111000 = 0x78
//   // If regular situation: devices_present == subtype
//   power = (XdrvMailbox.index & (mask << shift)) >> shift;
//   if (wakeup_active) {
//     wakeup_active--;
//   }
// #ifdef DEBUG_LIGHT
//   AddLog_P2(LOG_LEVEL_TEST, "LightSetPower XdrvMailbox.index=%d old_power=%d power=%d mask=%d shift=%d",
//     XdrvMailbox.index, old_power, power, mask, shift);
// #endif
//   if (power != old_power) {
//     update = true;
//   }
//   LightAnimate();
}


void mInterfaceLight::LightPreparePower(power_t channels) {    // 1 = only RGB, 2 = only CT, 3 = both RGB and CT

#ifdef DEBUG_LIGHT
  // AddLog_P2(LOG_LEVEL_DEBUG, "LightPreparePower power=%d power=%d", power, power);
#endif
  // If multi-channels, then we only switch off channels with a value of zero
  // if (pwm_multi_channels) {
  //   for (uint32_t i = 0; i < subtype; i++) {
  //     if (bitRead(channels, i)) {
  //       // if channel is non-null, channel is supposed to be on, but it is off, do Power On
  //       if ((current_color[i]) && (!bitRead(power, i))) {
  //         if (!Settings.flag.not_power_linked) {  // SetOption20 - Control power in relation to Dimmer/Color/Ct changes
  //           ExecuteCommandPower(device + i, POWER_ON_NO_STATE, SRC_LIGHT);
  //         }
  //       } else {
  //         // if channel is zero and channel is on, set it off
  //         if ((0 == current_color[i]) && bitRead(power, i)) {
  //           ExecuteCommandPower(device + i, POWER_OFF_NO_STATE, SRC_LIGHT);
  //         }
  //       }
  // #ifdef USE_DOMOTICZ
  //       DomoticzUpdatePowerState(device + i);
  // #endif  // USE_DOMOTICZ
  //     }
  //   }
  // } else {
    // if (light_controller.isCTRGBLinked()) {   // linked, standard
    //   if (light_state.getBri() && !(power)) {
    //     if (!Settings.flag.not_power_linked) {  // SetOption20 - Control power in relation to Dimmer/Color/Ct changes
    //       ExecuteCommandPower(device, POWER_ON_NO_STATE, SRC_LIGHT);
    //     }
    //   } else if (!light_state.getBri() && power) {
    //     ExecuteCommandPower(device, POWER_OFF_NO_STATE, SRC_LIGHT);
    //   }
    // } else {
    //   // RGB
    //   if (channels & 1) {
    //     if (light_state.getBriRGB() && !(power & 1)) {
    //       if (!Settings.flag.not_power_linked) {  // SetOption20 - Control power in relation to Dimmer/Color/Ct changes
    //         ExecuteCommandPower(device, POWER_ON_NO_STATE, SRC_LIGHT);
    //       }
    //     } else if (!light_state.getBriRGB() && (power & 1)) {
    //       ExecuteCommandPower(device, POWER_OFF_NO_STATE, SRC_LIGHT);
    //     }
    //   }
    //   // White CT
    //   if (channels & 2) {
    //     if (light_state.getBriCT() && !(power & 2)) {
    //       if (!Settings.flag.not_power_linked) {  // SetOption20 - Control power in relation to Dimmer/Color/Ct changes
    //         ExecuteCommandPower(device + 1, POWER_ON_NO_STATE, SRC_LIGHT);
    //       }
    //     } else if (!light_state.getBriCT() && (power & 2)) {
    //       ExecuteCommandPower(device + 1, POWER_OFF_NO_STATE, SRC_LIGHT);
    //     }
    //   }
    // }

    // ExecuteCommandPower(0, POWER_ON_NO_STATE, SRC_LIGHT);


// #ifdef USE_DOMOTICZ
//     DomoticzUpdatePowerState(device);
// #endif  // USE_DOMOTICZ
//   }

//   if (Settings.flag3.hass_tele_on_power) {  // SetOption59 - Send tele/%topic%/STATE in addition to stat/%topic%/RESULT
//     MqttPublishTeleState();
//   }

// #ifdef DEBUG_LIGHT
//   AddLog_P2(LOG_LEVEL_DEBUG, "LightPreparePower End power=%d power=%d", power, power);
// #endif
//   power = power >> (device - 1);  // reset next state, works also with unlinked RGB/CT
  // LightState(0);
  
}




// convert channels to string, use Option 17 to foce decimal, unless force_hex
char* mInterfaceLight::LightGetColor(char* scolor, boolean force_hex)
{
  // if ((0 == Settings.light_scheme) || (!pwm_multi_channels)) {
  //   light_controller.UpdateFinalColourComponents();      // recalculate levels only if Scheme 0, otherwise we mess up levels
  // }
  // scolor[0] = '\0';
  // for (uint32_t i = 0; i < subtype; i++) {
  //   if (!force_hex && Settings.flag.decimal_text) {  // SetOption17 - Switch between decimal or hexadecimal output
  //     snprintf_P(scolor, LIGHT_COLOR_SIZE, PSTR("%s%s%d"), scolor, (i > 0) ? "," : "", current_color[i]);
  //   } else {
  //     snprintf_P(scolor, LIGHT_COLOR_SIZE, PSTR("%s%02X"), scolor, current_color[i]);
  //   }
  // }
  return scolor;
}

void mInterfaceLight::LightPowerOn(void)
{
//   if (light_state.getBri() && !(power)) {
//     ExecuteCommandPower(device, POWER_ON, SRC_LIGHT);
//   }
}

void mInterfaceLight::LightState(uint8_t append)
{
//   char scolor[LIGHT_COLOR_SIZE];
//   char scommand[33];
//   bool unlinked = !light_controller.isCTRGBLinked() && (subtype >= LST_RGBW);  // there are 2 power and dimmers for RGB and White

//   if (append) {
//     ResponseAppend_P(PSTR(","));
//   } else {
//     Response_P(PSTR("{"));
//   }
//   if (!pwm_multi_channels) {
//     if (unlinked) {
//       // RGB and W are unlinked, we display the second Power/Dimmer
//       ResponseAppend_P(PSTR("\"" D_RSLT_POWER "%d\":\"%s\",\"" D_JSON_DIMMER "%d\":%d"
//                            ",\"" D_RSLT_POWER "%d\":\"%s\",\"" D_JSON_DIMMER "%d\":%d"),
//                             device, GetStateText(power & 1), device, light_state.getDimmer(1),
//                             device + 1, GetStateText(power & 2 ? 1 : 0), device + 1, light_state.getDimmer(2));
//     } else {
//       GetPowerDevice(scommand, device, sizeof(scommand), Settings.flag.device_index_enable);  // SetOption26 - Switch between POWER or POWER1
//       ResponseAppend_P(PSTR("\"%s\":\"%s\",\"" D_JSON_DIMMER "\":%d"), scommand, GetStateText(power & 1),
//                       light_state.getDimmer());
//     }


//     if (subtype > LST_SINGLE) {
//       ResponseAppend_P(PSTR(",\"" D_JSON_COLOR "\":\"%s\""), LightGetColor(scolor));
//       if (LST_RGB <= subtype) {
//         uint16_t hue;
//         uint8_t  sat, bri;
//         light_state.getHSB(&hue, &sat, &bri);
//         sat = mSupport::changeUIntScale(sat, 0, 255, 0, 100);
//         bri = mSupport::changeUIntScale(bri, 0, 255, 0, 100);

//         ResponseAppend_P(PSTR(",\"" D_JSON_HSBCOLOR "\":\"%d,%d,%d\""), hue,sat,bri);
//       }
//       // Add White level
//       if ((LST_COLDWARM == subtype) || (LST_RGBW <= subtype)) {
//         ResponseAppend_P(PSTR(",\"" D_JSON_WHITE "\":%d"), light_state.getDimmer(2));
//       }
//       // Add CT
//       if ((LST_COLDWARM == subtype) || (LST_RGBCW == subtype)) {
//         ResponseAppend_P(PSTR(",\"" D_JSON_COLORTEMPERATURE "\":%d"), light_state.getCT());
//       }
//       // Add status for each channel
//       ResponseAppend_P(PSTR(",\"" D_JSON_CHANNEL "\":[" ));
//       for (uint32_t i = 0; i < subtype; i++) {
//         uint8_t channel_raw = current_color[i];
//         uint8_t channel = mSupport::changeUIntScale(channel_raw,0,255,0,100);
//         // if non null, force to be at least 1
//         if ((0 == channel) && (channel_raw > 0)) { channel = 1; }
//         ResponseAppend_P(PSTR("%s%d" ), (i > 0 ? "," : ""), channel);
//       }
//       ResponseAppend_P(PSTR("]"));
//     }

//     if (append) {
//       if (subtype >= LST_RGB) {
//         ResponseAppend_P(PSTR(",\"" D_JSON_SCHEME "\":%d"), Settings.light_scheme);
//       }
//       if (max_scheme > LS_MAX) {
//         ResponseAppend_P(PSTR(",\"" D_JSON_WIDTH "\":%d"), Settings.light_width);
//       }
//       ResponseAppend_P(PSTR(",\"" D_JSON_FADE "\":\"%s\",\"" D_JSON_SPEED "\":%d,\"" D_JSON_LEDTABLE "\":\"%s\""),
//         GetStateText(Settings.light_fade), Settings.light_speed, GetStateText(Settings.light_correction));
//     }
//   } else {  // pwm_multi_channels
//     for (uint32_t i = 0; i < subtype; i++) {
//       GetPowerDevice(scommand, device + i, sizeof(scommand), 1);
//       uint32_t light_power_masked = power & (1 << i);    // the power value for this device
//       light_power_masked = light_power_masked ? 1 : 0;                    // convert to on/off
//       ResponseAppend_P(PSTR("\"%s\":\"%s\",\"" D_JSON_CHANNEL "%d\":%d,"), scommand, GetStateText(light_power_masked), device + i,
//         mSupport::changeUIntScale(current_color[i], 0, 255, 0, 100));
//     }
//     ResponseAppend_P(PSTR("\"" D_JSON_COLOR "\":\"%s\""), LightGetColor(scolor));
//   }   // pwm_multi_channels

//   if (!append) {
//     ResponseJsonEnd();
//   }
}



// On entry we take the 5 channels 8 bits entry, and we apply Power modifiers
// I.e. shut down channels that are powered down
void mInterfaceLight::LightApplyPower(uint8_t new_color[LST_MAX], power_t power) {
  // If SetOption68, multi_channels

  //tmp 
  // uint8_t pwm_multi_channels= false;

  // if (pwm_multi_channels) {
  //   // if multi-channels, specifically apply the power bits
  //   for (uint32_t i = 0; i < LST_MAX; i++) {
  //     if (0 == bitRead(power,i)) {  // if power down bit is zero
  //       new_color[i] = 0;   // shut down this channel
  //     }
  //   }
  //   #ifdef DEBUG_LIGHT
  //   AddLog_P(LOG_LEVEL_TEST, PSTR("Animate>> power=%d new_color=[%d,%d,%d,%d,%d]"),
  //     pCONT_set->power, new_color[0], new_color[1], new_color[2],
  //     new_color[3], new_color[4]);
  //   #endif
  // } else {
    if (!isCTRGBLinked()) {
      // we have 2 power bits for RGB and White
      if (0 == (pCONT_set->power & 1)) {
        new_color[0] = new_color[1] = new_color[2] = 0;
      }
      if (0 == (pCONT_set->power & 2)) {
        new_color[3] = new_color[4] = 0;
      }
    } else 
    if (!pCONT_set->power) {
      for (uint32_t i = 0; i < LST_MAX; i++) {
        new_color[i] = 0;
      }
    }
  // }
}



void mInterfaceLight::BootMessage(){
  
  AddLog_P(LOG_LEVEL_INFO, PSTR("BOOT: " "LightCount=%d"), light_count);

}



int8_t mInterfaceLight::GetNearestColourMapIDFromColour(HsbColor hsb){

}

const char* mInterfaceLight::GetColourMapNameNearestbyColour(HsbColor c, char* buffer){

// TOO MUCH MEMROY!!!
  return WARNING_NOTHANDLED_CTR;
// return "memory issue";

  // uint32_t colour32bit_tofind = WebColorFromColourType(RgbColor(c));

  // uint64_t colour32bit_tosearch[PRESET_COLOUR_MAP_INDEXES_MAX];
  
  // for(int i=0; i<PRESET_COLOUR_MAP_INDEXES_MAX; i++) {
  //   colour32bit_tosearch[i] = (WebColorFromColourType(RgbColor(c))-colour32bit_tofind);
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




int8_t mInterfaceLight::GetColourMapIDbyName(const char* c){
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
const char* mInterfaceLight::GetColourMapNamebyID(uint8_t id, char* buffer){

  return 0;
  // turn this into a dlist?

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




//   void loadSettings() {
// #ifdef DEBUG_LIGHT
//     AddLog_P2(LOG_LEVEL_TEST, "LightControllerClass::loadSettings Settings.light_color (%d %d %d %d %d - %d)",
//       Settings.light_color[0], Settings.light_color[1], Settings.light_color[2],
//       Settings.light_color[3], Settings.light_color[4], Settings.light_dimmer);
//     AddLog_P2(LOG_LEVEL_TEST, "LightControllerClass::loadSettings Settings.light_settings.type/sub (%d %d)",
//       Settings.light_settings.type, subtype);
// #endif
//     if (_pwm_multi_channels) {
//     //   _state->setChannelsRaw(Settings.LightSettings.light_color);
//     } else {
//       // first try setting CW, if zero, it select RGB mode
//     //   _state->setCW(Settings.light_color[3], Settings.light_color[4], true);
//     //   _state->setRGB(Settings.light_color[0], Settings.light_color[1], Settings.light_color[2]);

//       // only if non-multi channel
//       // We apply dimmer in priority to RGB
//     //   uint8_t bri = _state->DimmerToBri(Settings.light_dimmer);

//     //   // The default values are #FFFFFFFFFF, in this case we avoid setting all channels
//     //   // at the same time, see #6534 and #8120
//     //   if ((DEFAULT_LIGHT_COMPONENT == Settings.light_color[0]) &&
//     //       (DEFAULT_LIGHT_COMPONENT == Settings.light_color[1]) &&
//     //       (DEFAULT_LIGHT_COMPONENT == Settings.light_color[2]) &&
//     //       (DEFAULT_LIGHT_COMPONENT == Settings.light_color[3]) &&
//     //       (DEFAULT_LIGHT_COMPONENT == Settings.light_color[4]) &&
//     //       (DEFAULT_LIGHT_DIMMER    == Settings.light_dimmer) ) {
//     //     if ((LST_COLDWARM == subtype) || (LST_RGBCW == subtype)) {
//     //       _state->setCW(255, 0);       // avoid having both white channels at 100%, zero second channel (#see 8120)
//     //     }
//     //     _state->setBriCT(bri);
//     //     _state->setBriRGB(bri);
//     //     _state->setColorMode(LCM_RGB);
//       }
//     //   if (Settings.light_color[0] + Settings.light_color[1] + Settings.light_color[2] > 0) {
//     //     _state->setBriRGB(bri);
//     //   } else {
//     //     _state->setBriCT(bri);
//     //   }
//     //}
//   }


// save the current light state to Settings.
void mInterfaceLight::saveSettings() {
// //     if (pwm_multi_channels) {
// //       // simply save each channel
// //       _state->getChannelsRaw(Settings.light_color);
// //       Settings.light_dimmer = 100;    // arbitrary value, unused in this mode
// //     } else {
// //       uint8_t cm = _state->getColorMode();

// //       memset(&Settings.light_color[0], 0, sizeof(Settings.light_color));    // clear all channels
// //       if (LCM_RGB & cm) {   // can be either LCM_RGB or LCM_BOTH
// //         _state->getRGB(&Settings.light_color[0], &Settings.light_color[1], &Settings.light_color[2]);
// //         Settings.light_dimmer = _state->BriToDimmer(_state->getBriRGB());
// //         // anyways we always store RGB with BrightnessRGB
// //         if (LCM_BOTH == cm) {
// //           // then store at actual brightness CW/WW if dual mode
// //           _state->getActualRGBCW(nullptr, nullptr, nullptr, &Settings.light_color[3], &Settings.light_color[4]);
// //         }
// //       } else if (LCM_CT == cm) {    // cm can only be LCM_CT
// //         _state->getCW(&Settings.light_color[3], &Settings.light_color[4]);
// //         Settings.light_dimmer = _state->BriToDimmer(_state->getBriCT());
// //       }
// //     }
// // #ifdef DEBUG_LIGHT
// //     AddLog_P2(LOG_LEVEL_TEST, "LightControllerClass::saveSettings Settings.light_color (%d %d %d %d %d - %d)",
// //       Settings.light_color[0], Settings.light_color[1], Settings.light_color[2],
// //       Settings.light_color[3], Settings.light_color[4], Settings.light_dimmer);
// // #endif
}


/***************************************************************************************************************************************************************************
/***************************************************************************************************************************************************************************
/***************************************************************************************************************************************************************************
/***************************************************************************************************************************************************************************
/***************************************************************************************************************************************************************************
/***************************************************************************************************************************************************************************
/***************************************************************************************************************************************************************************
/***************************************************************************************************************************************************************************
/***************************************************************************************************************************************************************************
/***************************************************************************************************************************************************************************
/****************MOVED FROM ADDRESABLE and deemed "common" ***********************************************************************************************************************************************************
/***************************************************************************************************************************************************************************
/***************************************************************************************************************************************************************************
/***************************************************************************************************************************************************************************
/***************************************************************************************************************************************************************************
/***************************************************************************************************************************************************************************
/***************************************************************************************************************************************************************************
/***************************************************************************************************************************************************************************
/***************************************************************************************************************************************************************************
/***************************************************************************************************************************************************************************
/***************************************************************************************************************************************************************************
/***************************************************************************************************************************************************************************/


void mInterfaceLight::init_PresetColourPalettes(){
  
  
  #ifdef ENABLE_LOG_LEVEL_INFO
  // AddLog_P(LOG_LEVEL_WARN,PSTR("init_PresetColourPalettes"));
  #endif
  

  for(uint8_t id=0;id<20;id++){
    init_PresetColourPalettes_UserFill(id);
    setdefault_PresetColourPalettes_UserFill(id);
    
  // // for(uint8_t id=0;id<400;id++){
  // AddLog_Array(LOG_LEVEL_TEST, "test3", 
  //   &pCONT_set->Settings.animation_settings.palette_user_colour_map_ids[0],
  //   (uint8_t)400);//palettelist.ptr->colour_map_size);
  // // }
  }

// AddLog_Array(LOG_LEVEL_TEST,PSTR("test=%d"),Settings.animation_settings.palette_user_colour_map_ids[palettelist.ptr->id][0])

  // memcpy_P(pCONT_set->Settings.animation_settings.palette_user_colour_map_ids,DEFAULT_COLOUR_PALETTE_USER_01_IDS,sizeof(DEFAULT_COLOUR_PALETTE_USER_01_IDS));

  // memcpy_P(palettelist.ptr->colour_map_id,DEFAULT_COLOUR_PALETTE_USER_01_IDS,sizeof(DEFAULT_COLOUR_PALETTE_USER_01_IDS));

  // init defaults
  init_ColourPalettes_Autumn_Red();
  init_ColourPalettes_Autumn();
  init_ColourPalettes_Winter(); 
  init_ColourPalettes_Ocean_01();     //add way to take gradient patterns and use thier colours only (not gradient indexes)
  init_ColourPalettes_Rainbow();
  init_ColourPalettes_RainbowInverted();
  init_ColourPalettes_Holloween_OP();
  init_ColourPalettes_Holloween_OGP();
  init_ColourPalettes_Shelf_Hearts();
  init_ColourPalettes_Pastel();
  init_ColourPalettes_Gradient_01();
  init_ColourPalettes_Gradient_02();
  init_ColourPalettes_Sunrise_01();

}



void mInterfaceLight::init_ColourPalettes_Holloween_OP(){
  palettelist.ptr     = &palettelist.holloween_op;  
  palettelist.ptr->id = PALETTELIST_STATIC_HOLLOWEEN_OP_ID;
  palettelist.ptr->colour_map_size    = sizeof(colour_map_holloween_op_id);
  palettelist.ptr->colour_map_id      = (uint8_t*)colour_map_holloween_op_id;
  palettelist.ptr->friendly_name_ctr  = (char*)PM_PALETTE_HOLLOWEEN_OP_NAME_CTR;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
}

void mInterfaceLight::init_ColourPalettes_Holloween_OGP(){
  palettelist.ptr     = &palettelist.holloween_ogp;  
  palettelist.ptr->id = PALETTELIST_STATIC_HOLLOWEEN_OGP_ID;
  palettelist.ptr->colour_map_size    = sizeof(colour_map_holloween_ogp_id);
  palettelist.ptr->colour_map_id      = (uint8_t*)colour_map_holloween_ogp_id;
  palettelist.ptr->friendly_name_ctr  = (char*)PM_PALETTE_HOLLOWEEN_OGP_NAME_CTR;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
}
void mInterfaceLight::init_ColourPalettes_Pastel(){
  palettelist.ptr     = &palettelist.pastel;  
  palettelist.ptr->id = PALETTELIST_STATIC_PASTEL_ID;
  palettelist.ptr->colour_map_size    = sizeof(COLOUR_PALETTE_PASTEL_MAP_IDS);
  palettelist.ptr->colour_map_id      = (uint8_t*)COLOUR_PALETTE_PASTEL_MAP_IDS;
  palettelist.ptr->friendly_name_ctr  = (char*)PM_PALETTE_PASTEL_NAME_CTR;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
}

void mInterfaceLight::init_ColourPalettes_Shelf_Hearts(){
  palettelist.ptr     = &palettelist.shelf_hearts;  
  palettelist.ptr->id = PALETTELIST_STATIC_SHELF_HEARTS_ID;
  palettelist.ptr->colour_map_size = sizeof(colour_map_shelf_hearts_id);
  palettelist.ptr->colour_map_id = (uint8_t*)colour_map_shelf_hearts_id;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_SHELF_HEARTS_NAME_CTR;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_ID;
  // Need a solution to remember these ie if shelf01, hearts are pink, else yellow
  
    #ifdef DONT_USE_OLD_PRESETS
  preset_colour_map[COLOUR_MAP_SPECIAL_ONEINMANY_MANY_ID] = preset_colour_map[COLOUR_MAP_YELLOW_ID]; 
  preset_colour_map[COLOUR_MAP_SPECIAL_ONEINMANY_ONE_ID]  = preset_colour_map[COLOUR_MAP_HOTPINK_ID]; 
  #endif
}

void mInterfaceLight::init_ColourPalettes_Gradient_01(){
  palettelist.ptr     = &palettelist.gradient_01;  
  palettelist.ptr->id = PALETTELIST_STATIC_GRADIENT_01_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_GRADIENT_01_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_GRADIENT_01_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_GRADIENT_01_NAME_CTR;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_100;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID;
  
  #ifdef ENABLE_LOG_LEVEL_INFO
  // CAUSED DEBUG MODE CRASH
  //AddLog_Array_P(LOG_LEVEL_TEST, "Grad01", palettelist.ptr->colour_map_id,palettelist.ptr->colour_map_size);
  #endif
}
void mInterfaceLight::init_ColourPalettes_Gradient_02(){
  palettelist.ptr     = &palettelist.gradient_02;  
  palettelist.ptr->id = PALETTELIST_STATIC_GRADIENT_02_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_GRADIENT_02_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_GRADIENT_02_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_GRADIENT_02_NAME_CTR;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_GRADIENT_ID;
  
  #ifdef ENABLE_LOG_LEVEL_INFO
  //AddLog_Array_P(LOG_LEVEL_TEST, "Grad02", palettelist.ptr->colour_map_id,palettelist.ptr->colour_map_size);
  #endif
}
void mInterfaceLight::init_ColourPalettes_Sunrise_01(){
  palettelist.ptr     = &palettelist.sunrise_01;  
  palettelist.ptr->id = PALETTELIST_STATIC_SUNRISE_01_ID;
  palettelist.ptr->colour_map_size = sizeof(PM_PALETTE_SUNRISE_01_COLOUR_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)PM_PALETTE_SUNRISE_01_COLOUR_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_SUNRISE_01_NAME_CTR;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_NONE;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX_ID;
  
  #ifdef ENABLE_LOG_LEVEL_INFO
  //AddLog_Array_P(LOG_LEVEL_TEST, "Sunrise01", palettelist.ptr->colour_map_id,palettelist.ptr->colour_map_size);
  #endif
}


void mInterfaceLight::init_ColourPalettes_Autumn_Red(){
  palettelist.ptr     = &palettelist.autumn_red;
  palettelist.ptr->id = PALETTELIST_STATIC_AUTUMN_RED_ID;
  palettelist.ptr->colour_map_size = sizeof(colour_map_autumnred_id);
  palettelist.ptr->colour_map_id = (uint8_t*)colour_map_autumnred_id;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_AUTUMN_RED_NAME_CTR;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = true;
}

void mInterfaceLight::init_ColourPalettes_Autumn(){
  palettelist.ptr     = &palettelist.autumn;
  palettelist.ptr->id = PALETTELIST_STATIC_AUTUMN_ID;
  palettelist.ptr->colour_map_size = sizeof(colour_map_autumn_id);
  palettelist.ptr->colour_map_id = (uint8_t*)colour_map_autumn_id;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_AUTUMN_NAME_CTR;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = true;
}

void mInterfaceLight::init_ColourPalettes_Winter(){
  palettelist.ptr     = &palettelist.winter;
  palettelist.ptr->id = PALETTELIST_STATIC_WINTER_ID;
  palettelist.ptr->colour_map_size = sizeof(COLOUR_PALETTE_WINTER_MAP_IDS);
  palettelist.ptr->colour_map_id = (uint8_t*)COLOUR_PALETTE_WINTER_MAP_IDS;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_WINTER_NAME_CTR;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = true;
}

void mInterfaceLight::init_ColourPalettes_Ocean_01(){
  palettelist.ptr     = &palettelist.ocean_01;
  palettelist.ptr->id = PALETTELIST_STATIC_OCEAN_01_ID;
  palettelist.ptr->colour_map_size = sizeof(colour_map_ocean_01_id);
  palettelist.ptr->colour_map_id = (uint8_t*)colour_map_ocean_01_id;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_OCEAN_01_NAME_CTR;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID;
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = true;
  // palettelist.ptr->flags.fOverride_Saturation_To_Random = true;
}

void mInterfaceLight::init_ColourPalettes_Rainbow(){
  palettelist.ptr     = &palettelist.rainbow;
  palettelist.ptr->id = PALETTELIST_STATIC_RAINBOW_ID;
  palettelist.ptr->colour_map_size = sizeof(colour_map_rainbow_id);
  palettelist.ptr->colour_map_id = (uint8_t*)colour_map_rainbow_id;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_RAINBOW_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID;
}

void mInterfaceLight::init_ColourPalettes_RainbowInverted(){
  palettelist.ptr     = &palettelist.rainbow_inverted;
  palettelist.ptr->id = PALETTELIST_STATIC_RAINBOW_INVERTED_ID;
  palettelist.ptr->colour_map_size = sizeof(colour_map_rainbow_inverted_id);
  palettelist.ptr->colour_map_id = (uint8_t*)colour_map_rainbow_inverted_id;
  palettelist.ptr->friendly_name_ctr = (char*)PM_PALETTE_RAINBOW_INVERTED_NAME_CTR;
  palettelist.ptr->flags.fIndexs_Type = INDEX_TYPE_SCALED_255;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID;
}



/*******************************************************************************************************************
********************************************************************************************************************
************ START OF PALETTE DEFINITIONS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/

HsbColor mInterfaceLight::GetHsbColour(uint8_t id){

  HsbColor colour = HsbColor(1,1,1);


  // Get Saturation
  uint8_t sat_val = id%12;
  // AddLog_P(LOG_LEVEL_INFO,PSTR("sat_val = %d   %d"), sat_val, id);


  switch(sat_val){
    default:
    case 0: colour.S = 1.0f; break;
    case 1: colour.S = 0.95f; break;
    case 2: colour.S = 0.9f; break;
    case 3: colour.S = 0.8f; break;
    case 4: colour.S = 0.7f; break;
    case 5: colour.S = 0.6f; break;
    case 6: colour.S = 0.5f; break;
    case 7: colour.S = 0.4f; break;
    case 8: colour.S = 0.3f; break;
    case 9: colour.S = 0.2f; break;
    case 10: colour.S = 0.1f; break;
    case 11: colour.S = 0.0f; break;
  }


  // Get hue
  if(id>=COLOUR_MAP_RED_ID    && id<=COLOUR_MAP_RED_PASTEL0_ID){
    colour.H = pgm_read_float(colour_float_map_values); // progmem here might not be useful
  }else
  if(id>=COLOUR_MAP_ORANGE_ID && id<=COLOUR_MAP_ORANGE_PASTEL0_ID){
    colour.H = pgm_read_float(colour_float_map_values+1);
  }else
  if(id>=COLOUR_MAP_LIGHTORANGE_ID && id<=COLOUR_MAP_LIGHTORANGE_PASTEL0_ID){
    colour.H = pgm_read_float(colour_float_map_values+2);
  }else
  if(id>=COLOUR_MAP_WHITE_ID && id<=COLOUR_MAP_WHITE_PASTEL0_ID){
    colour.H = pgm_read_float(colour_float_map_values+3);
  }else
  if(id>=COLOUR_MAP_YELLOW_ID && id<=COLOUR_MAP_YELLOW_PASTEL0_ID){
    colour.H = pgm_read_float(colour_float_map_values+4);
  }else
  if(id>=COLOUR_MAP_LIMEGREEN_ID && id<=COLOUR_MAP_LIMEGREEN_PASTEL0_ID){
    colour.H = pgm_read_float(colour_float_map_values+5);
  }else
  if(id>=COLOUR_MAP_GREEN_ID && id<=COLOUR_MAP_GREEN_PASTEL0_ID){
    colour.H = pgm_read_float(colour_float_map_values+6);
  }else
  if(id>=COLOUR_MAP_CYAN_ID && id<=COLOUR_MAP_CYAN_PASTEL0_ID){
    colour.H = pgm_read_float(colour_float_map_values+7);
  }else
  if(id>=COLOUR_MAP_BLUE_ID && id<=COLOUR_MAP_BLUE_PASTEL0_ID){
    colour.H = pgm_read_float(colour_float_map_values+8);
  }else
  if(id>=COLOUR_MAP_BLUEPURPLE_ID && id<=COLOUR_MAP_BLUEPURPLE_PASTEL0_ID){
    colour.H = pgm_read_float(colour_float_map_values+9);
  }else
  if(id>=COLOUR_MAP_PURPLE_ID && id<=COLOUR_MAP_PURPLE_PASTEL0_ID){
    colour.H = pgm_read_float(colour_float_map_values+10);
  }else
  if(id>=COLOUR_MAP_PINK_ID && id<=COLOUR_MAP_PINK_PASTEL0_ID){
    colour.H = pgm_read_float(colour_float_map_values+11);
  }else
  if(id>=COLOUR_MAP_HOTPINK_ID && id<=COLOUR_MAP_HOTPINK_PASTEL0_ID){
    colour.H = pgm_read_float(colour_float_map_values+12);
  }

  // AddLog_P(LOG_LEVEL_INFO,PSTR("id mod 12 = %d   %d"), id%12, id);
  // colour.S = (12-sat_val)

  return colour;

}


int8_t mInterfaceLight::GetPaletteIDbyName(const char* c){
  if(c=='\0'){
    return -1;
  }

  PALETTELIST::PALETTE *ptr = NULL;

DEBUG_LINE;
  for(uint8_t ii=0;ii<PALETTELIST_STATIC_LENGTH_ID;ii++){
    ptr = GetPalettePointerByID(ii);



// AddLog_P(LOG_LEVEL_ERROR, PSTR("ptr->id %d"),ptr->id);
// AddLog_P(LOG_LEVEL_ERROR, PSTR("ptr->friendly_name_ctr %s"),ptr->friendly_name_ctr);

//needs to use the shared buffer in settings dlist


    if(ptr->friendly_name_ctr == nullptr){ 
      AddLog_P(LOG_LEVEL_ERROR, PSTR("ptr->friendly_name_ctr == nullptr %d %s"),ii,c);
      // return 0;
      break; //break for loop and ignore this .. NEEDS fixed
      
      
    }
    if(strstr(c,ptr->friendly_name_ctr)){
// AddLog_P(LOG_LEVEL_ERROR, PSTR("strstr(c,ptr->friendly_name_ctr"));
      return ii;
    }
  }

DEBUG_LINE;
  // Check for default user names
  char name_ctr[100];
  for(uint8_t ii=0;ii<20;ii++){
    memset(name_ctr,0,sizeof(name_ctr));
    sprintf_P(name_ctr,PSTR("%s %02d\0"),D_PALETTE_USER_NAME_CTR,ii);
    // Default names
    if(strstr(c,name_ctr)){
      return PALETTELIST_VARIABLE_USER_01_ID+ii;
    }
    // Friendly names in memory
    // if(strstr(c,palettelist.users[PALETTELIST_VARIABLE_USER_01_ID+ii].friendly_name_ctr)){
    //   return PALETTELIST_VARIABLE_USER_01_ID+ii;
    // }
  }
  
  return -1;

}


mInterfaceLight::PALETTELIST::PALETTE* mInterfaceLight::GetPalettePointerByID(uint8_t id){
  switch(id){
    default:
    case PALETTELIST_VARIABLE_USER_01_ID: return &palettelist.users[0];
    case PALETTELIST_VARIABLE_USER_02_ID: return &palettelist.users[1];
    case PALETTELIST_VARIABLE_USER_03_ID: return &palettelist.users[2];
    case PALETTELIST_VARIABLE_USER_04_ID: return &palettelist.users[3];
    case PALETTELIST_VARIABLE_USER_05_ID: return &palettelist.users[4];
    case PALETTELIST_VARIABLE_USER_06_ID: return &palettelist.users[5];
    case PALETTELIST_VARIABLE_USER_07_ID: return &palettelist.users[6];
    case PALETTELIST_VARIABLE_USER_08_ID: return &palettelist.users[7];
    case PALETTELIST_VARIABLE_USER_09_ID: return &palettelist.users[8];
    case PALETTELIST_VARIABLE_USER_10_ID: return &palettelist.users[9];
    case PALETTELIST_VARIABLE_USER_11_ID: return &palettelist.users[10];
    case PALETTELIST_VARIABLE_USER_12_ID: return &palettelist.users[11];
    case PALETTELIST_VARIABLE_USER_13_ID: return &palettelist.users[12];
    case PALETTELIST_VARIABLE_USER_14_ID: return &palettelist.users[13];
    case PALETTELIST_VARIABLE_USER_15_ID: return &palettelist.users[14];
    case PALETTELIST_VARIABLE_USER_16_ID: return &palettelist.users[15];
    case PALETTELIST_VARIABLE_USER_17_ID: return &palettelist.users[16];
    case PALETTELIST_VARIABLE_USER_18_ID: return &palettelist.users[17];
    case PALETTELIST_VARIABLE_USER_19_ID: return &palettelist.users[18];
    case PALETTELIST_VARIABLE_USER_20_ID: return &palettelist.users[19];
    case PALETTELIST_STATIC_SHELF_HEARTS_ID:    return &palettelist.shelf_hearts;
    case PALETTELIST_STATIC_HOLLOWEEN_OP_ID:    return &palettelist.holloween_op;
    case PALETTELIST_STATIC_HOLLOWEEN_OGP_ID:   return &palettelist.holloween_ogp;
    case PALETTELIST_STATIC_RAINBOW_ID:         return &palettelist.rainbow;
    case PALETTELIST_STATIC_RAINBOW_INVERTED_ID:return &palettelist.rainbow_inverted;
    case PALETTELIST_STATIC_PASTEL_ID:          return &palettelist.pastel;
    case PALETTELIST_STATIC_WINTER_ID:          return &palettelist.winter;
    case PALETTELIST_STATIC_AUTUMN_ID:          return &palettelist.autumn;
    case PALETTELIST_STATIC_AUTUMN_RED_ID:      return &palettelist.autumn_red;
    case PALETTELIST_STATIC_OCEAN_01_ID:        return &palettelist.ocean_01;
    case PALETTELIST_STATIC_GRADIENT_01_ID:     return &palettelist.gradient_01;
    case PALETTELIST_STATIC_GRADIENT_02_ID:     return &palettelist.gradient_02;
    case PALETTELIST_STATIC_SUNRISE_01_ID:      return &palettelist.sunrise_01;
  }
}





int8_t mInterfaceLight::CheckPaletteIsEditable(PALETTELIST::PALETTE *ptr){
  if( (ptr->id>=PALETTELIST_VARIABLE_USER_01_ID)&&
      (ptr->id<=PALETTELIST_VARIABLE_USER_20_ID) ){
    return true;
  }
  return false;
}



// Check for friendly names first
const char* mInterfaceLight::GetPaletteFriendlyName(char* buffer){
#ifdef USE_MODULE_LIGHTS_ADDRESSABLE
  return GetPaletteFriendlyNameByID(animation.palette_id, buffer);
  #else
  return GetPaletteFriendlyNameByID(0, buffer);

  #endif
} 
const char* mInterfaceLight::GetPaletteFriendlyNameByID(uint8_t id, char* name, uint8_t name_size){
  PALETTELIST::PALETTE *ptr = GetPalettePointerByID(id);
  // if user range, ram
  if(id<PALETTELIST_VARIABLE_USER_LENGTH_ID){
    //search via list
    if(name_size != 0){
      pCONT_sup->GetTextIndexed_P(name, name_size, id, pCONT_set->Settings.animation_settings.palette_user_variable_name_list_ctr);
      return name;
    }
  return WARNING_NOTHANDLED_CTR;
    // return "name";
  }
  //progmem
  else{
    return ptr->friendly_name_ctr!=nullptr?ptr->friendly_name_ctr:WARNING_NOTHANDLED_CTR;//"NULL";
  }
}



const char* mInterfaceLight::GetPaletteName(char* buffer){
  
#ifdef USE_MODULE_LIGHTS_ADDRESSABLE
  return GetPaletteNameByID(animation.palette_id, buffer);
  #else
  return GetPaletteNameByID(0, buffer);

  #endif
} 
const char* mInterfaceLight::GetPaletteNameByID(uint8_t id, char* buffer){  
  PALETTELIST::PALETTE *ptr = GetPalettePointerByID(id);
  return ptr->friendly_name_ctr!=nullptr?ptr->friendly_name_ctr:WARNING_NOTHANDLED_CTR;//"NULL";
  
  return WARNING_NOTHANDLED_CTR;
  // return "Unknown";
}


uint16_t mInterfaceLight::GetPixelsInMap(PALETTELIST::PALETTE *ptr){
  uint16_t pixel_width = 0;
  switch(ptr->flags.fMapIDs_Type){
    case MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID: pixel_width = 1; break;
    case MAPIDS_TYPE_HSBCOLOUR_NOINDEX_ID: pixel_width = 1; break;
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_ID: pixel_width = 2; break;
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_AND_SETALL_ID: pixel_width = 2; break;
    case MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID: pixel_width = 3; break;
    case MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX_ID: pixel_width = 5; break;
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_ID: pixel_width = 4; break;
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_AND_SETALL_ID: pixel_width = 4; break;
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID: pixel_width = 2; break;
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_GRADIENT_ID: pixel_width = 4; break;
  }
  if(pixel_width){
    return ptr->colour_map_size/pixel_width;
  }
  return 0;
}



const uint8_t* mInterfaceLight::GetDefaultColourPaletteUserIDs_P(uint8_t id){
  switch(id){
    default: 
    case PALETTELIST_VARIABLE_USER_01_ID: return DEFAULT_COLOUR_PALETTE_USER_01_IDS;
    case PALETTELIST_VARIABLE_USER_02_ID: return DEFAULT_COLOUR_PALETTE_USER_02_IDS;
    case PALETTELIST_VARIABLE_USER_03_ID: return DEFAULT_COLOUR_PALETTE_USER_03_IDS;
    case PALETTELIST_VARIABLE_USER_04_ID: return DEFAULT_COLOUR_PALETTE_USER_04_IDS;
    case PALETTELIST_VARIABLE_USER_05_ID: return DEFAULT_COLOUR_PALETTE_USER_05_IDS;
    case PALETTELIST_VARIABLE_USER_06_ID: return DEFAULT_COLOUR_PALETTE_USER_06_IDS;
    case PALETTELIST_VARIABLE_USER_07_ID: return DEFAULT_COLOUR_PALETTE_USER_07_IDS;
    case PALETTELIST_VARIABLE_USER_08_ID: return DEFAULT_COLOUR_PALETTE_USER_08_IDS;
    case PALETTELIST_VARIABLE_USER_09_ID: return DEFAULT_COLOUR_PALETTE_USER_09_IDS;
    case PALETTELIST_VARIABLE_USER_10_ID: return DEFAULT_COLOUR_PALETTE_USER_10_IDS;
    case PALETTELIST_VARIABLE_USER_11_ID: return DEFAULT_COLOUR_PALETTE_USER_11_IDS;
    case PALETTELIST_VARIABLE_USER_12_ID: return DEFAULT_COLOUR_PALETTE_USER_12_IDS;
    case PALETTELIST_VARIABLE_USER_13_ID: return DEFAULT_COLOUR_PALETTE_USER_13_IDS;
    case PALETTELIST_VARIABLE_USER_14_ID: return DEFAULT_COLOUR_PALETTE_USER_14_IDS;
    case PALETTELIST_VARIABLE_USER_15_ID: return DEFAULT_COLOUR_PALETTE_USER_15_IDS;
    case PALETTELIST_VARIABLE_USER_16_ID: return DEFAULT_COLOUR_PALETTE_USER_16_IDS;
    case PALETTELIST_VARIABLE_USER_17_ID: return DEFAULT_COLOUR_PALETTE_USER_17_IDS;
    case PALETTELIST_VARIABLE_USER_18_ID: return DEFAULT_COLOUR_PALETTE_USER_18_IDS;
    case PALETTELIST_VARIABLE_USER_19_ID: return DEFAULT_COLOUR_PALETTE_USER_19_IDS;
    case PALETTELIST_VARIABLE_USER_20_ID: return DEFAULT_COLOUR_PALETTE_USER_20_IDS;
  }
}


uint8_t mInterfaceLight::GetDefaultColourPaletteUserIDsCount(uint8_t id){


  // Serial.printf("GetDefaultColourPaletteUserIDsCount\n\r"); Serial.flush();

  auto id_ptr = GetDefaultColourPaletteUserIDs_P(id);
  // Serial.printf(GetDefaultColourPaletteUserIDs_P(id));

  
  // const uint8_t* id_ptr = DEFAULT_COLOUR_PALETTE_USER_01_IDS;

  // Serial.printf("GetDefaultColourPaletteUserIDsCountBB=%d\n\r",   pgm_read_byte(id_ptr)); Serial.flush();

  //search for COLOUR_MAP_NONE_ID to signify end, must be less than 20 in length

  uint8_t pixel_active = 0;
  uint8_t byte_read = 0;
  while(pixel_active<20){
    byte_read = pgm_read_byte(id_ptr+pixel_active);
    // Serial.printf("byte_read=%d",byte_read);
    if(byte_read!=COLOUR_MAP_NONE_ID){//} && byte_read<COLOUR_MAP_PALETTE_OPTIONS_END_ID){
      pixel_active++;
    }else{
      break;
    }
  }
  // Serial.printf("pixel_active=%d\n\r",   pixel_active); Serial.flush();

  return pixel_active>PALETTELIST_COLOUR_USERS_AMOUNT_MAX?PALETTELIST_COLOUR_USERS_AMOUNT_MAX:pixel_active;
}




// point things to memory locations
void mInterfaceLight::init_PresetColourPalettes_UserFill(uint8_t id){//} colour_ids, const uint8_t colour_ids_len){

  palettelist.ptr     = &palettelist.users[id];
  palettelist.ptr->id = id;

  // Point to memory location
  palettelist.ptr->colour_map_id = &pCONT_set->Settings.animation_settings.palette_user_colour_map_ids[id*20];//palettelist.ptr->id*20];//[0];

  // for()
  
  palettelist.ptr->colour_map_size = 0;//Get
  palettelist.ptr->friendly_name_ctr = nullptr;//pCONT_set->Settings.animation_settings.palette_user_variable_name_list_ctr;//&palettelist_variable_users_ctr[ptr->id][0];
  
  palettelist.ptr->flags.fRandomise_Between_Colour_Pairs = false;
  palettelist.ptr->flags.fMapIDs_Type = MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID;

  // Serial.printf("colour_ids[%d]\n\r",id); Serial.flush();
}

void mInterfaceLight::setdefault_PresetColourPalettes_UserFill(uint8_t id){//} colour_ids, const uint8_t colour_ids_len){

  palettelist.ptr     = &palettelist.users[id];

  // Overwrite memory locations
  // memset(&palettelist.ptr->colour_map_id,COLOUR_MAP_NONE_ID,sizeof(char)*PALETTELIST_VARIABLE_USER_LENGTH_ID); // assume >amount == NONE

  // fill memory location to defaults
  uint8_t get_pixels_used = GetDefaultColourPaletteUserIDsCount(id);
  // char* test = "test"; //GetDefaultColourPaletteUserIDs_P(id)

  auto id_ptr = GetDefaultColourPaletteUserIDs_P(id);
  // Serial.printf("get_pixels_used=%d\n\r",get_pixels_used); Serial.flush();

  //clear back to none_id
  memset(palettelist.ptr->colour_map_id,COLOUR_MAP_NONE_ID,sizeof(uint8_t)*20);

  // Read from PROGMEM the defaults
  memcpy_P(palettelist.ptr->colour_map_id,id_ptr,sizeof(uint8_t)*get_pixels_used);

  //set pixels in palette
  palettelist.ptr->colour_map_size = get_pixels_used;


  // palettelist.ptr->colour_map_id = test;

  // memcpy_P(palettelist.ptr->colour_map_id, id_ptr, sizeof(uint8_t)*1);

  // uint8_t test[20];
  // uint8_t* colour_map_id = test;
  // memcpy_P(colour_map_id, id_ptr, sizeof(uint8_t)*1);

  
  // Serial.printf("colour_map_id[%d]\n\r",palettelist.ptr->colour_map_id[0]); Serial.flush();
  // Serial.printf("test[%d]\n\r",palettelist.ptr->colour_map_id[0]); Serial.flush();
  // Serial.printf("colour_map_id[%d]\n\r",palettelist.ptr->colour_map_id[1]); Serial.flush();

}

// uint8_t mInterfaceLight::GetPaletteUserActivePixels()


void mInterfaceLight::SetPaletteListPtrFromID(uint8_t id){
  //AddLog_P(LOG_LEVEL_TEST,PSTR("SetPaletteListPtrFromID(%d)"),id);
  palettelist.ptr = GetPalettePointerByID(id);
}



RgbcctColor mInterfaceLight::GetColourFromPalette(PALETTELIST::PALETTE *ptr, uint16_t pixel_num, int16_t *pixel_position)
{

  RgbcctColor colour = RgbcctColor(0);
  uint8_t palette_elements[ptr->colour_map_size];
  uint16_t index_relative = 0; // get expected pixel position
  uint16_t expected_pixel_count = 0; // get expected pixel position

  if(ptr->id < PALETTELIST_VARIABLE_USER_LENGTH_ID){
    memcpy(&palette_elements,ptr->colour_map_id,sizeof(uint8_t)*ptr->colour_map_size);
  }
  // progmem
  else{
    memcpy_P(&palette_elements,ptr->colour_map_id,sizeof(uint8_t)*ptr->colour_map_size);
  }

  #ifdef ENABLE_LOG_LEVEL_DEBUG
  AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "fMapIDs_Type=%d"),ptr->flags.fMapIDs_Type);
  #endif

  switch(ptr->flags.fMapIDs_Type){
    default:
    case MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID:{
      if(pixel_position != nullptr){ *pixel_position = 1; }
      colour = RgbcctColor(
        GetHsbColour(palette_elements[pixel_num])
      );
    }
    break;
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_GRADIENT_ID:{

      index_relative = pixel_num*4; // get expected pixel position
      expected_pixel_count = ptr->colour_map_size/4; // get expected pixel position
      
      if(pixel_position != nullptr){ *pixel_position = palette_elements[index_relative]; }
      // Get colour
      colour = RgbcctColor(
        palette_elements[index_relative+1],
        palette_elements[index_relative+2],
        palette_elements[index_relative+3],
        0,
        0
      );

    }break;
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID:
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_AND_SETALL_ID: //0,h,s,b (only if uint16 is allowed for hue>255)
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_ID:{
      
      index_relative = pixel_num*2; // get expected pixel position
      expected_pixel_count = ptr->colour_map_size/2; // get expected pixel position
      
      if(pixel_position != nullptr){ *pixel_position = palette_elements[index_relative]; }

      colour = RgbcctColor(
        GetHsbColour(palette_elements[index_relative+1])
      );
      
    }
    break;
    case MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID:{
      // Get expected pixel position
      index_relative = pixel_num*3;
      // Get colour
      colour = RgbcctColor(
        palette_elements[index_relative  ],
        palette_elements[index_relative+1],
        palette_elements[index_relative+2],
        0,
        0
      );
    }
    break;
    case MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX_ID:{
      // Get expected pixel position
      index_relative = pixel_num*5;
      // Get colour
      colour = RgbcctColor(
        palette_elements[index_relative  ],
        palette_elements[index_relative+1],
        palette_elements[index_relative+2],
        palette_elements[index_relative+3],
        palette_elements[index_relative+4]
      );
    }
    break;
  }

  return colour;

} // end function

/*******************************************************************************************************************
********************************************************************************************************************
************ END OF PALETTE DEFINITIONS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/



uint32_t mInterfaceLight::WebColorFromColourMap(uint8_t i)
{
  RgbColor rgb = RgbColor(GetHsbColour(i));
  uint32_t tcolor = (rgb.R << 16) | (rgb.G << 8) | rgb.B;
  return tcolor;
}
uint32_t mInterfaceLight::WebColorFromColourType(RgbColor rgb)
{
  uint32_t tcolor = (rgb.R << 16) | (rgb.G << 8) | rgb.B;
  return tcolor;
}



















































































































/****************************************************************************
 ****************************************************************************
 ****************************************************************************
 ****************************************************************************
 ****************************************************************************
 ****************************************************************************
 ****************************************************************************
 ************************************************************************** 
 ************************************************************************** 
 ************************************************************************** 
 *START                  "Setters" aka internal as "light_controller", dont use directly
 *****************************************************************************
 ****************************************************************************
 ****************************************************************************
 ****************************************************************************
 ****************************************************************************
 ****************************************************************************
 ****************************************************************************
 ************************************************************************* 
 ************************************************************************** 
 ************************************************************************** 
 *******************************************************************************/



// This function is a bit hairy, it will try to match the required
// colormode with the features of the device:
//   LST_NONE:      LCM_RGB
//   LST_SINGLE:    LCM_RGB
//   LST_COLDWARM:  LCM_CT
//   LST_RGB:       LCM_RGB
//   LST_RGBW:      LCM_RGB, LCM_CT or LCM_BOTH
//   LST_RGBCW:     LCM_RGB, LCM_CT or LCM_BOTH
uint8_t mInterfaceLight::setColorMode(uint8_t cm) {
  uint8_t prev_cm = _color_mode;
  if (cm < LCM_RGB) { cm = LCM_RGB; }
  if (cm > LCM_BOTH) { cm = LCM_BOTH; }
  uint8_t maxbri = (_briRGB >= _briCT) ? _briRGB : _briCT;

  switch (_subtype) {
    case LST_COLDWARM:
      _color_mode = LCM_CT;
      break;
    case LST_NONE:
    case LST_SINGLE:
    case LST_RGB:
    default:
      _color_mode = LCM_RGB;
      break;
    case LST_RGBW:
    case LST_RGBCW:
      _color_mode = cm;
      break;
  }
  if (LCM_RGB == _color_mode) {
    _briCT = 0;
    if (0 == _briRGB) { _briRGB = maxbri; }
  }
  if (LCM_CT == _color_mode) {
    _briRGB = 0;
    if (0 == _briCT) { _briCT = maxbri; }
  }
// #ifdef DEBUG_LIGHT
  AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("setColorMode prev_cm (%d) req_cm (%d) new_cm (%d) subtype (%d)"), prev_cm, cm, _color_mode, _subtype);
// #endif
  return prev_cm;
}


uint8_t mInterfaceLight::getColorMode() {
  return _color_mode;
}

void mInterfaceLight::addRGBMode() {  
  // AddLog_P(LOG_LEVEL_TEST, PSTR("f::addRGBMode"));
  setColorMode(_color_mode | LCM_RGB);
}
void mInterfaceLight::addCTMode() {
  // AddLog_P(LOG_LEVEL_TEST, PSTR("f::addCTMode"));
  setColorMode(_color_mode | LCM_CT);
}

// Get RGB color, always at full brightness (ie. one of the components is 255)
void mInterfaceLight::getRGB(uint8_t *r, uint8_t *g, uint8_t *b) {
  if (r) { *r = tasint_colour.R; }
  if (g) { *g = tasint_colour.G; }
  if (b) { *b = tasint_colour.B; }
}

// get full brightness values for warm and cold channels.
// either w=c=0 (off) or w+c >= 255
void mInterfaceLight::getCW(uint8_t *rc, uint8_t *rw) {
  if (rc) { *rc = tasint_colour.WC; }
  if (rw) { *rw = tasint_colour.WW; }
}

// Get the actual values for each channel, ie multiply with brightness
void mInterfaceLight::getActualRGBCW(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *c, uint8_t *w) {

  bool rgb_channels_on = _color_mode & LCM_RGB;
  bool ct_channels_on =   _color_mode & LCM_CT;

  #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("bgetActualRGBCW result %d,%d,%d,%d,%d %d,%d"),
    tasint_colour.R,
    tasint_colour.G,
    tasint_colour.B,
    tasint_colour.WC,
    tasint_colour.WW,
    // *r,*g,*b,*w,*c,  
     _briRGB,_briCT);
  #endif
  
  if (r) { *r = rgb_channels_on ? mSupport::changeUIntScale(tasint_colour.R, 0, 255, 0, _briRGB) : 0; }
  if (g) { *g = rgb_channels_on ? mSupport::changeUIntScale(tasint_colour.G, 0, 255, 0, _briRGB) : 0; }
  if (b) { *b = rgb_channels_on ? mSupport::changeUIntScale(tasint_colour.B, 0, 255, 0, _briRGB) : 0; }

  if (c) { *c = ct_channels_on  ? mSupport::changeUIntScale(tasint_colour.WC, 0, 255, 0, _briCT) : 0; }
  if (w) { *w = ct_channels_on  ? mSupport::changeUIntScale(tasint_colour.WW, 0, 255, 0, _briCT) : 0; }

  #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("agetActualRGBCW result %d,%d,%d,%d,%d %d,%d"),*r,*g,*b,*w,*c,   _briRGB,_briCT);
  #endif

}

uint8_t mInterfaceLight::getChannels(uint8_t *channels) {
  getActualRGBCW(&channels[0], &channels[1], &channels[2], &channels[3], &channels[4]);
}

void mInterfaceLight::getChannelsRaw(uint8_t *channels) {
    channels[0] = tasint_colour.R;
    channels[1] = tasint_colour.G;
    channels[2] = tasint_colour.B;
    channels[3] = tasint_colour.WC;
    channels[4] = tasint_colour.WW;
}

void mInterfaceLight::getHSB(uint16_t *hue, uint8_t *sat, uint8_t *bri) {
  if (hue) { *hue = _hue; }
  if (sat) { *sat = _sat; }
  if (bri) { *bri = _briRGB; }
}

// getBri() is guaranteed to give the same result as setBri() - no rounding errors.
uint8_t mInterfaceLight::getBri(void) {
  // return the max of _briCT and _briRGB
  return (_briRGB >= _briCT) ? _briRGB : _briCT;
}

// get the white Brightness
uint8_t mInterfaceLight::getBriCT() {
  return _briCT;
}


uint8_t mInterfaceLight::DimmerToBri(uint8_t dimmer) {
  return mSupport::changeUIntScale(dimmer, 0, 100, 0, 255);  // 0..255
}

uint8_t mInterfaceLight::BriToDimmer(uint8_t bri) {
  uint8_t dimmer = mSupport::changeUIntScale(bri, 0, 255, 0, 100);
  // if brightness is non zero, force dimmer to be non-zero too
  if ((dimmer == 0) && (bri > 0)) { dimmer = 1; }
  return dimmer;
}

uint8_t mInterfaceLight::getDimmer(uint32_t mode) {
  uint8_t bri;
  switch (mode) {
    case 1:
      bri = getBriRGB();
      break;
    case 2:
      bri = getBriCT();
      break;
    default:
      bri = getBri();
      break;
  }
  return BriToDimmer(bri);
}


uint16_t mInterfaceLight::getCT(){
  return _ct; // 153..500, or CT_MIN..CT_MAX
}

// get the CT value within the range into a 10 bits 0..1023 value
uint16_t mInterfaceLight::getCT10bits() {
  return mSupport::changeUIntScale(_ct, _ct_min_range, _ct_max_range, 0, 1023);
}

void mInterfaceLight::setCTRange(uint16_t ct_min_range, uint16_t ct_max_range) {
  _ct_min_range = ct_min_range;
  _ct_max_range = ct_max_range;
}

void mInterfaceLight::getCTRange(uint16_t *ct_min_range, uint16_t *ct_max_range) {
  if (ct_min_range) { *ct_min_range = _ct_min_range; }
  if (ct_max_range) { *ct_max_range = _ct_max_range; }
}



// setters -- do not use directly, use the light_controller instead
// sets both master Bri and whiteBri
void mInterfaceLight::setBri(uint8_t bri) {
  setBriRGB(_color_mode & LCM_RGB ? bri : 0);
  setBriCT(_color_mode & LCM_CT ? bri : 0);
#ifdef DEBUG_LIGHT
  AddLog_P(LOG_LEVEL_TEST, PSTR("setBri RGB raw (%d %d %d) HS (%d %d) bri (%d)"), tasint_colour.R, tasint_colour.G, tasint_colour.B, _hue, _sat, _briRGB);
#endif
}

// changes the RGB brightness alone
uint8_t mInterfaceLight::setBriRGB(uint8_t bri_rgb) {
  uint8_t prev_bri = _briRGB;
  _briRGB = bri_rgb;

//needs to map in to RGB? or I need to keep RGBCCT as colour only, with these brightness separate


  if (bri_rgb > 0) { addRGBMode(); }
  return prev_bri;
}

// changes the white brightness alone
uint8_t mInterfaceLight::setBriCT(uint8_t bri_ct) {
  uint8_t prev_bri = _briCT;
  _briCT = bri_ct;
  if (bri_ct > 0) { addCTMode(); }
  return prev_bri;
}

uint8_t mInterfaceLight::getBriRGB() {
  return _briRGB;
}

void mInterfaceLight::setDimmer(uint8_t dimmer) {
  setBri(DimmerToBri(dimmer));
}

void mInterfaceLight::setCT(uint16_t ct) {
  if (0 == ct) {
    // disable ct mode
    setColorMode(LCM_RGB);  // try deactivating CT mode, setColorMode() will check which is legal
  } else {
    ct = (ct < CT_MIN ? CT_MIN : (ct > CT_MAX ? CT_MAX : ct));
    tasint_colour.WW = mSupport::changeUIntScale(ct, _ct_min_range, _ct_max_range, 0, 255);
    tasint_colour.WC = 255 - tasint_colour.WW;
    _ct = ct;
    addCTMode();
  }
#ifdef DEBUG_LIGHT
  AddLog_P(LOG_LEVEL_INFO, PSTR("setCT RGB raw (%d %d %d) HS (%d %d) briRGB (%d) briCT (%d) CT (%d)"), tasint_colour.R, tasint_colour.G, tasint_colour.B, _hue, _sat, _briRGB, _briCT, _ct);
#endif
}


// Manually set Cold/Warm channels.
// There are two modes:
// 1. (free_range == false, default)
//    In this mode there is only one virtual white channel with color temperature
//    As a side effect, WC+WW = 255. It means also that the sum of light power
//    from white LEDs is always equal to briCT. It is not possible here
//    to set both white LEDs at full power, hence protecting power supplies
//    from overlaoding.
// 2. (free_range == true)
//    In this mode, values of WC and WW are free -- both channels can be set
//    at full power.
//    In this mode, we always scale both channels so that one at least is 255.
// We automatically adjust briCT to have the right values of channels
void mInterfaceLight::setCW(uint8_t c, uint8_t w, bool free_range) {
  uint16_t max = (w > c) ? w : c;   // 0..255
  uint16_t sum = c + w;
  if (sum <= 257) { free_range = false; }    // if we don't allow free range or if sum is below 255 (with tolerance of 2)

  if (0 == max) {
    _briCT = 0;       // brightness set to null
  AddLog_P(LOG_LEVEL_TEST, PSTR("f::addCTMode (LCM_RGB)"));
    setColorMode(LCM_RGB);  // try deactivating CT mode, setColorMode() will check which is legal
  } else {
    if (!free_range) {
      // we need to normalize to sum = 255
      tasint_colour.WW = mSupport::changeUIntScale(w, 0, sum, 0, 255);
      tasint_colour.WC = 255 - tasint_colour.WW;
    } else {  // we normalize to max = 255
      tasint_colour.WW = mSupport::changeUIntScale(w, 0, max, 0, 255);
      tasint_colour.WC = mSupport::changeUIntScale(c, 0, max, 0, 255);
    }
    _ct = mSupport::changeUIntScale(w, 0, sum, _ct_min_range, _ct_max_range);
    addCTMode();   // activate CT mode if needed
    if (_color_mode & LCM_CT) { _briCT = free_range ? max : (sum > 255 ? 255 : sum); }
  }
  #ifdef DEBUG_LIGHT
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("setCW CW (%d %d) CT (%d) briCT (%d)"), c, w, _ct, _briCT);
  #endif
}

// sets RGB and returns the Brightness. Bri is updated unless keep_bri is true
uint8_t mInterfaceLight::setRGB(uint8_t r, uint8_t g, uint8_t b, bool keep_bri) {
  
  uint16_t hue;
  uint8_t  sat;
  #ifdef DEBUG_LIGHT
    AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("setRGB RGB input (%d %d %d)"), r, g, b);
  #endif

  uint8_t max = (r > g && r > b) ? r : (g > b) ? g : b;   // 0..255

  if (0 == max) {
    r = g = b = 255;
    setColorMode(LCM_CT);   // try deactivating RGB, setColorMode() will check if this is legal
  } else {
    if (255 > max) {        // we need to normalize rgb
      r = mSupport::changeUIntScale(r, 0, max, 0, 255);
      g = mSupport::changeUIntScale(g, 0, max, 0, 255);
      b = mSupport::changeUIntScale(b, 0, max, 0, 255);
    }
    addRGBMode();
  }
  if (!keep_bri) {
    _briRGB = (_color_mode & LCM_RGB) ? max : 0;
  }

  RgbToHsb(r, g, b, &hue, &sat, nullptr);
  tasint_colour.R = r;
  tasint_colour.G = g;
  tasint_colour.B = b;
  _hue = hue;
  _sat = sat;
#ifdef DEBUG_LIGHT
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("setRGB RGB raw (%d %d %d) HS (%d %d) bri (%d)"), tasint_colour.R, tasint_colour.G, tasint_colour.B, _hue, _sat, _briRGB);
#endif
  return max;
}

void mInterfaceLight::setHS(uint16_t hue, uint8_t sat) {
  uint8_t r, g, b;
  HsToRgb(hue, sat, &r, &g, &b);
  
  tasint_colour.R = r;
  tasint_colour.G = g;
  tasint_colour.B = b;

  _hue = hue;
  _sat = sat;
  addRGBMode();
#ifdef DEBUG_LIGHT
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("setHS HS (%d %d) rgb (%d %d %d)"), hue, sat, r, g, b);
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("setHS RGB raw (%d %d %d) HS (%d %d) bri (%d)"), tasint_colour.R, tasint_colour.G, tasint_colour.B, _hue, _sat, _briRGB);
#endif
}

// set all 5 channels at once, don't modify the values in ANY way
// Channels are: R G B CW WW
void mInterfaceLight::setChannelsRaw(uint8_t *channels) {
  tasint_colour.R = channels[0];
  tasint_colour.G = channels[1];
  tasint_colour.B = channels[2];
  tasint_colour.WC = channels[3];
  tasint_colour.WW = channels[4];
}


void mInterfaceLight::setChannelsRaw(uint8_t r, uint8_t g, uint8_t b, uint8_t wc, uint8_t ww) {
  tasint_colour.R = r;
  tasint_colour.G = g;
  tasint_colour.B = b;
  tasint_colour.WC = wc;
  tasint_colour.WW = ww;

}


void mInterfaceLight::setChannels(uint8_t r, uint8_t g, uint8_t b, uint8_t wc, uint8_t ww) {

  uint8_t buffer[5];
  buffer[0] = r; 
  buffer[1] = g; 
  buffer[2] = b; 
  buffer[3] = wc; 
  buffer[4] = ww; 
  setChannels(buffer);

}

// set all 5 channels at once.
// Channels are: R G B CW WW
// Brightness is automatically recalculated to adjust channels to the desired values
void mInterfaceLight::setChannels(uint8_t *channels) {
  setRGB(channels[0], channels[1], channels[2]);
  setCW(channels[3], channels[4], true);  // free range for WC and WW
#ifdef DEBUG_LIGHT
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("setChannels (%d %d %d %d %d)"), channels[0], channels[1], channels[2], channels[3], channels[4]);
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("setChannels CT (%d) briRGB (%d) briCT (%d)"), _ct, _briRGB, _briCT);
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("setChannels Actuals (%d %d %d %d %d)"), tasint_colour.R, tasint_colour.G, tasint_colour.B, tasint_colour.WC, tasint_colour.WW);
#endif
}

// // new version with only integer computing
// // brightness is not needed, it is controlled via Dimmer
void mInterfaceLight::RgbToHsb(uint8_t ir, uint8_t ig, uint8_t ib, uint16_t *r_hue, uint8_t *r_sat, uint8_t *r_bri) {
  uint32_t r = ir;
  uint32_t g = ig;
  uint32_t b = ib;
  uint32_t max = (r > g && r > b) ? r : (g > b) ? g : b;   // 0..255
  uint32_t min = (r < g && r < b) ? r : (g < b) ? g : b;   // 0..255
  uint32_t d = max - min;   // 0..255

  uint16_t hue = 0;   // hue value in degrees ranges from 0 to 359
  uint8_t sat = 0;    // 0..255
  uint8_t bri = max;  // 0..255

  if (d != 0) {
    sat = mSupport::changeUIntScale(d, 0, max, 0, 255);
    if (r == max) {
      hue = (g > b) ?       mSupport::changeUIntScale(g-b,0,d,0,60) : 360 - mSupport::changeUIntScale(b-g,0,d,0,60);
    } else if (g == max) {
      hue = (b > r) ? 120 + mSupport::changeUIntScale(b-r,0,d,0,60) : 120 - mSupport::changeUIntScale(r-b,0,d,0,60);
    } else {
      hue = (r > g) ? 240 + mSupport::changeUIntScale(r-g,0,d,0,60) : 240 - mSupport::changeUIntScale(g-r,0,d,0,60);
    }
    hue = hue % 360;    // 0..359
  }

  if (r_hue) *r_hue = hue;
  if (r_sat) *r_sat = sat;
  if (r_bri) *r_bri = bri;
#ifdef DEBUG_LIGHT
  AddLog_P(LOG_LEVEL_DEBUG_MORE, "RgbToHsb rgb (%d %d %d) hsb (%d %d %d)", r, g, b, hue, sat, bri);
#endif
}

// uint32_t to uint8_t might cause issues
void mInterfaceLight::HsToRgb(uint16_t hue, uint8_t sat, uint8_t *r_r, uint8_t *r_g, uint8_t *r_b) {
  uint8_t r = 255; // default to white
  uint8_t g = 255;
  uint8_t b = 255; // we take brightness at 100%, brightness should be set separately
  hue = hue % 360;  // normalize to 0..359
  if (sat > 0) {
    uint8_t i = hue / 60;   // quadrant 0..5
    uint8_t f = hue % 60;   // 0..59
    uint8_t q = 255 - mSupport::changeUIntScale(f, 0, 60, 0, sat);  // 0..59
    uint8_t p = 255 - sat;
    uint8_t t = 255 - mSupport::changeUIntScale(60 - f, 0, 60, 0, sat);
    switch (i) {
      case 0: g = t; b = p; break;
      case 1: r = q; b = p; break;
      case 2: r = p; b = t; break;
      case 3: r = p; g = q; break;
      case 4: r = t; g = p; break;
      default: g = p; b = q; break;
      }
    }
  if (r_r)  *r_r = r;
  if (r_g)  *r_g = g;
  if (r_b)  *r_b = b;
}


// Matrix 3x3 multiplied to a 3 vector, result in a 3 vector
void mInterfaceLight::mat3x3(const float *mat33, const float *vec3, float *res3) {
  for (uint8_t i = 0; i < 3; i++) {
    const float * v = vec3;
    *res3 = 0.0f;
    for (uint8_t j = 0; j < 3; j++) {
      *res3 += *mat33++ * *v++;
    }
    res3++;
  }
}


void mInterfaceLight::setSubType(uint8_t sub_type) {
  _subtype = sub_type;
}


bool mInterfaceLight::setCTRGBLinked(bool ct_rgb_linked) {
  bool prev = _ct_rgb_linked;
  if (_pwm_multi_channels) {
    _ct_rgb_linked = false;   // force to false if _pwm_multi_channels is set
  } else {
    _ct_rgb_linked = ct_rgb_linked;
  }
  return prev;
}


bool mInterfaceLight::isCTRGBLinked() {
  return _ct_rgb_linked;
}


bool mInterfaceLight::setPWMMultiChannel(bool pwm_multi_channels) {
  bool prev = _pwm_multi_channels;
  _pwm_multi_channels = pwm_multi_channels;
  if (pwm_multi_channels)  setCTRGBLinked(false);    // if pwm multi channel, then unlink RGB and CT
  return prev;
}


bool mInterfaceLight::isPWMMultiChannel(void) {
  return _pwm_multi_channels;
}



// If SetOption68 is set, get the brightness for a specific device
uint8_t mInterfaceLight::LightGetBri(uint8_t device) {
  uint8_t bri = 254;   // default value if relay
  if (pwm_multi_channels) {
    if ((device >= device) && (device < device + LST_MAX) && (device <= pCONT_set->devices_present)) {
      bri = current_color[device - device];
    }
  } else if (isCTRGBLinked()) {   // standard behavior
    if (device == device) {
      bri = getBri();
    }
  } else {    // unlinked
    if (device == device) {
      bri = getBriRGB();
    } else if (device == device + 1) {
      bri = getBriCT();
    }
  }
  return bri;
}

// If SetOption68 is set, set the brightness for a specific device
void mInterfaceLight::LightSetBri(uint8_t device, uint8_t bri) {
  if (pwm_multi_channels) {
    if ((device >= device) && (device < device + LST_MAX) && (device <= pCONT_set->devices_present)) {
      current_color[device - device] = bri;
      changeChannels(current_color);
    }
  } else if (isCTRGBLinked()) {  // standard
    if (device == device) {
      changeBri(bri);
    }
  } else {  // unlinked
    if (device == device) {
      changeBriRGB(bri);
    } else if (device == device + 1) {
      changeBriCT(bri);
    }
  }
}

void mInterfaceLight::LightSetColorTemp(uint16_t ct)
{
/* Color Temperature (https://developers.meethue.com/documentation/core-concepts)
 * ct = 153 = 6500K = Cold = CCWW = FF00
 * ct = 600 = 2000K = Warm = CCWW = 00FF  */
  // don't set CT if not supported
  if ((LST_COLDWARM != subtype) && (LST_RGBCW != subtype)) {
    return;
  }
  changeCTB(ct, getBriCT());
}

uint16_t mInterfaceLight::LightGetColorTemp(void)
{
  // don't calculate CT for unsupported devices
  if ((LST_COLDWARM != subtype) && (LST_RGBCW != subtype)) {
    return 0;
  }
  return (getColorMode() & LCM_CT) ? getCT() : 0;
}


// is the channel a regular PWM or ColorTemp control
bool mInterfaceLight::isChannelCT(uint32_t channel) {
  // if (
  //   // (MODULE_PHILIPS_ID == pCONT_set->my_module_type) || 
  //   (pCONT_set->Settings.flag4.pwm_ct_mode)) {
    if ((LST_COLDWARM == subtype) && (1 == channel)) { return true; }   // PMW reserved for CT
    if ((LST_RGBCW == subtype) && (4 == channel)) { return true; }   // PMW reserved for CT
  // }
  return false;
}





void mInterfaceLight::changeCTB(uint16_t new_ct, uint8_t briCT) {
  /* Color Temperature (https://developers.meethue.com/documentation/core-concepts)
    * ct = 153 = 6500K = Cold = CCWW = FF00
    * ct = 500 = 2000K = Warm = CCWW = 00FF
    */
  // don't set CT if not supported
  if ((LST_COLDWARM != subtype) && (LST_RGBW > subtype)) {
    return;
  }
  setCT(new_ct);
  setBriCT(briCT);
  if (_ct_rgb_linked) { setColorMode(LCM_CT); }   // try to force CT
  // saveSettings();
  UpdateFinalColourComponents();
  //debugLogs();
}


void mInterfaceLight::changeDimmer(uint8_t dimmer, uint32_t mode) {
  uint8_t bri = mSupport::changeUIntScale(dimmer, 0, 100, 0, 255);
  switch (mode) {
    case 1:
      changeBriRGB(bri);
      if (_ct_rgb_linked) { 
        AddLog_P(LOG_LEVEL_TEST, PSTR("f::addCTMode rgb linked rgb"));
        setColorMode(LCM_RGB);
      }   // try to force CT
      break;
    case 2:
      changeBriCT(bri);
      if (_ct_rgb_linked) { 
        AddLog_P(LOG_LEVEL_TEST, PSTR("f::addCTMode ct linked rgb"));
        setColorMode(LCM_CT); 
      }   // try to force CT
      break;
    default:
      changeBri(bri);
      break;
  }
}

void mInterfaceLight::changeBri(uint8_t bri) {
  setBri(bri);
  // saveSettings();
  UpdateFinalColourComponents();
}

void mInterfaceLight::changeBriRGB(uint8_t bri) {
  setBriRGB(bri);
  // saveSettings();
  UpdateFinalColourComponents();
}

void mInterfaceLight::changeBriCT(uint8_t bri) {
  setBriCT(bri);
  // saveSettings();
  UpdateFinalColourComponents();
}

void mInterfaceLight::changeRGB(uint8_t r, uint8_t g, uint8_t b, bool keep_bri) {
  setRGB(r, g, b, keep_bri);
  if (_ct_rgb_linked) { 
    AddLog_P(LOG_LEVEL_TEST, PSTR("f::addCTMode ct linked rgb changeRGB"));
    setColorMode(LCM_RGB); 
  }   // try to force RGB
  // saveSettings();
  UpdateFinalColourComponents();
}

//   calculate the levels for each channel
//   if no parameter, results are stored in current_color
void mInterfaceLight::UpdateFinalColourComponents(uint8_t *_current_color) {

  // AddLog_P(LOG_LEVEL_INFO, PSTR("UpdateFinalColourComponents"));

  if(mode_singlecolour.flag.update_colour_on_input_command){
    AddLog_P(LOG_LEVEL_INFO, PSTR("mode_singlecolour.flag.update_colour_on_input_command"));
    mode_singlecolour.name_id = MODE_SINGLECOLOUR_COLOURSCENE_ID;
  }

  DEBUG_LINE;

  uint8_t r,g,b,c,w,briRGB,briCT;
  if (_current_color == nullptr) { 
    //AddLog_P(LOG_LEVEL_TEST, PSTR("f:_current_color == nullptr use global"));    
    _current_color = current_color; 
  }else{
   // AddLog_P(LOG_LEVEL_TEST, PSTR("NOT f:_current_color == nullptr use global"));    
  }


  // if (_pwm_multi_channels) { // if PWM multi channel, no more transformation required
  //   _state->getChannelsRaw(current_color);
  //   return;
  // }

  DEBUG_LINE;

  // #ifdef ENABLE_LOG_LEVEL_DEBUG
  // AddLog_P(LOG_LEVEL_DEBUG_MORE, "BEFORE brightness _current_color (%d %d %d %d %d)",
  //   tasint_colour.R,tasint_colour.G,tasint_colour.B,tasint_colour.WW,tasint_colour.WC
  // );
  // #endif // ENABLE_LOG_LEVEL_DEBUG_MORE

  getActualRGBCW(&r,&g,&b,&c,&w);
  briRGB = getBriRGB();
  briCT  = getBriCT();
  
  #ifdef ENABLE_LOG_LEVEL_DEBUG
  AddLog_P(LOG_LEVEL_INFO, "After1 brightness _current_color (%d %d %d %d %d -- rgb%d ct%d)",
    r,g,b,c,w,briRGB,briCT
  );
  #endif // ENABLE_LOG_LEVEL_DEBUG_MORE

  // record previous colour
  memcpy(last_colour, current_color, sizeof(last_colour));

  DEBUG_LINE;

  _current_color[0] = _current_color[1] = _current_color[2] = 0;
  _current_color[3] = _current_color[4] = 0;
  switch (subtype) {
    case LST_NONE:
      _current_color[0] = 255;
      break;
    case LST_SINGLE:
      _current_color[0] = briRGB;
      break;
    case LST_COLDWARM:
      _current_color[0] = c;
      _current_color[1] = w;
      break;
    case LST_RGBW:
    case LST_RGBCW:
      if (LST_RGBCW == subtype) {
        _current_color[3] = c;
        _current_color[4] = w;
      } else {
        _current_color[3] = briCT;
      }
      // continue
    case LST_RGB:
      _current_color[0] = r;
      _current_color[1] = g;
      _current_color[2] = b;
      break;
  }

  animation.flags.ColourComponentsUpdatedButNotYetApplied = true;
   
  // #endif // ENABLE_LOG_LEVEL_DEBUG_MORE
  #ifdef ENABLE_LOG_LEVEL_DEBUG
  AddLog_P(LOG_LEVEL_INFO, "AFTER2 brightness _current_color (%d %d %d %d %d)",
    _current_color[0],_current_color[1],_current_color[2],_current_color[3],_current_color[4]
  );
  #endif // ENABLE_LOG_LEVEL_DEBUG_MORE

  // mqtthandler_debug_teleperiod.flags.SendNow = true;

  DEBUG_LINE;

}

void mInterfaceLight::changeHSB(uint16_t hue, uint8_t sat, uint8_t briRGB) {
  setHS(hue, sat);
  setBriRGB(briRGB);
  if (_ct_rgb_linked) { setColorMode(LCM_RGB); }   // try to force RGB
  // saveSettings();
  UpdateFinalColourComponents();
}


// set all 5 channels at once.
// Channels are: R G B CW WW
// Brightness is automatically recalculated to adjust channels to the desired values
void mInterfaceLight::changeChannels(uint8_t *channels) {
  if (pwm_multi_channels) {
    setChannelsRaw(channels);
  } else if (LST_COLDWARM == subtype) {
    // remap channels 0-1 to 3-4 if cold/warm
    uint8_t remapped_channels[5] = {0,0,0,channels[0],channels[1]};
    setChannels(remapped_channels);
  } else {
    setChannels(channels);
  }

  // saveSettings();
  UpdateFinalColourComponents();
}

uint16_t mInterfaceLight::getHue(){
  return _hue;
}
uint8_t mInterfaceLight::getSat(){
  return _sat;
}

HsbColor mInterfaceLight::GetColour_HSB(void){
  uint16_t hue = 0;
  uint8_t  sat = 0;
  uint8_t  brt = 0;
  getHSB(&hue,&sat,&brt);  
  return HsbColor(hue,sat,brt);
}


// void mInterfaceLight::Change_RgbcctColor_Hue(){

// }


/*********************************************************************************************\
 * Change scales from 8 bits to 10 bits and vice versa
\*********************************************************************************************/
// 8 to 10 to 8 is garanteed to give the same result
uint16_t mInterfaceLight::change8to10(uint8_t v) {
  return mSupport::changeUIntScale(v, 0, 255, 0, 1023);
}
// change from 10 bits to 8 bits, but any non-zero input will be non-zero
uint8_t mInterfaceLight::change10to8(uint16_t v) {
  return (0 == v) ? 0 : mSupport::changeUIntScale(v, 4, 1023, 1, 255);
}

/*********************************************************************************************\
 * Gamma correction
\*********************************************************************************************/
#ifdef ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
// Calculate the gamma corrected value for LEDS
uint16_t mInterfaceLight::ledGamma_internal(uint16_t v, const struct gamma_table_t *gt_ptr) {
  uint16_t from_src = 0;
  uint16_t from_gamma = 0;

  for (const gamma_table_t *gt = gt_ptr; ; gt++) {
    uint16_t to_src = gt->to_src;
    uint16_t to_gamma = gt->to_gamma;
    if (v <= to_src) {
      return mSupport::changeUIntScale(v, from_src, to_src, from_gamma, to_gamma);
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
      return mSupport::changeUIntScale(vg, from_gamma, to_gamma, from_src, to_src);
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
//         cur_col_10[cw0] = mSupport::changeUIntScale(cur_col_10[cw0], 0, white_bri10_1023, 0, white_bri_gamma10);
//         cur_col_10[cw1] = mSupport::changeUIntScale(cur_col_10[cw1], 0, white_bri10_1023, 0, white_bri_gamma10);
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
//   if (!pCONT_set->Settings.light_settings.light_correction) { return false; }   // Gamma correction not activated
//   if (channel >= subtype) { return false; }     // Out of range
// #ifdef ESP8266
//   if (
//     // (MODULE_PHILIPS_ID == pCONT_set->my_module_type) || 
//   (pCONT_set->Settings.flag4.pwm_ct_mode)) {
//     if ((LST_COLDWARM == subtype) && (1 == channel)) { return false; }   // PMW reserved for CT
//     if ((LST_RGBCW == subtype) && (4 == channel)) { return false; }   // PMW reserved for CT
//   }
// #endif  // ESP8266
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

void mInterfaceLight::LightPwmOffset(uint32_t offset)
{
  pwm_offset = offset;
}


// compute actual PWM min/max values from DimmerRange
// must be called when DimmerRange is changed or LedTable
void mInterfaceLight::LightCalcPWMRange(void) {
  uint16_t pwm_min=0, pwm_max=1023;

  // pwm_min = change8to10(DimmerToBri(pCONT_set->Settings.dimmer_hw_min));   // default 0
  // pwm_max = change8to10(DimmerToBri(pCONT_set->Settings.dimmer_hw_max));   // default 100
  // if (pCONT_set->Settings.light_correction) {
  //   pwm_min = ledGamma10_10(pwm_min);       // apply gamma correction
  //   pwm_max = ledGamma10_10(pwm_max);       // 0..1023
  // }
  // pwm_min = pwm_min > 0 ? mSupport::changeUIntScale(pwm_min, 1, 1023, 1, Settings.pwm_range) : 0;  // adapt range but keep zero and non-zero values
  // pwm_max = mSupport::changeUIntScale(pwm_max, 1, 1023, 1, Settings.pwm_range);  // pwm_max cannot be zero

  pwm_min = pwm_min;
  pwm_max = pwm_max;
  //AddLog_P2(LOG_LEVEL_TEST, PSTR("LightCalcPWMRange %d %d - %d %d"), Settings.dimmer_hw_min, Settings.dimmer_hw_max, pwm_min, pwm_max);
}


/****************************************************************************
 ****************************************************************************
 ****************************************************************************
 ****************************************************************************
 ****************************************************************************
 ****************************************************************************
 ****************************************************************************
 ************************************************************************** 
 ************************************************************************** 
 ************************************************************************** 
 *END                  "Setters" aka internal as "light_controller", dont use directly
 *****************************************************************************
 ****************************************************************************
 ****************************************************************************
 ****************************************************************************
 ****************************************************************************
 ****************************************************************************
 ****************************************************************************
 ************************************************************************* 
 ************************************************************************** 
 ************************************************************************** 
 *******************************************************************************/



#endif // USE_DRIVER
