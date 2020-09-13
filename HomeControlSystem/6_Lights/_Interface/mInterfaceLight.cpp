#include "mInterfaceLight.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE // interface is the gateway

/*** 
 * * Dev notes
 * 
 * - Any non add LEDs will be controlled through here for the short term 
 * - Long term, with several colour types and gamma stuff, introduce this is a gateway to rgbanimator (ie if led type, send command to the class AFTER calculating its colour etc for scenes
 * )
 * 
 * */

bool mInterfaceLight::LightModuleInit(void)
{
  pCONT_set->light_type = LT_BASIC;                    // Use basic PWM control if SetOption15 = 0

  if (pCONT_set->Settings.flag_system_phaseout.pwm_control) {          // SetOption15 - Switch between commands PWM or COLOR/DIMMER/CT/CHANNEL
    for (uint32_t i = 0; i < MAX_PWMS; i++) {
      if (pCONT_pins->PinUsed(GPIO_PWM1_ID, i)) { 
        pCONT_set->light_type++; 
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
  if (pCONT_pins->PinUsed(GPIO_RGB_DATA_ID)) { pCONT_set->light_type = LT_WS2812; }


// #ifdef ESP8266
//   else if (SONOFF_BN == my_module_type) {   // PWM Single color led (White)
//     light_type = LT_PWM1;
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
//     light_type = LT_PWM2;
//   }
// #endif  // ESP8266

  if (pCONT_set->light_type > LT_BASIC) {
    pCONT_set->devices_present++;
  }

  // // post-process for lights
  // if (Settings.flag3.pwm_multi_channels) {  // SetOption68 - Enable multi-channels PWM instead of Color PWM
  //   uint32_t pwm_channels = (light_type & 7) > LST_MAX ? LST_MAX : (light_type & 7);
  //   if (0 == pwm_channels) { pwm_channels = 1; }
  //   devices_present += pwm_channels - 1;    // add the pwm channels controls at the end
  // } else if ((Settings.param[P_RGB_REMAP] & 128) && (LST_RGBW <= (light_type & 7))) {
  //   // if RGBW or RGBCW, and SetOption37 >= 128, we manage RGB and W separately, hence adding a device
  //   devices_present++;
  // }

  return (pCONT_set->light_type > LT_BASIC);
}



// void mInterfaceLight::Module_Init(){
void mInterfaceLight::Template_Load(){

  #ifndef DISABLE_SERIAL_LOGGING
  Serial.printf("mInterfaceLight::Template_Load()"); Serial.flush();
  #endif

  // load from progmem
  uint16_t progmem_size = sizeof(LIGHTING_TEMPLATE);
  progmem_size = progmem_size>1500?1500:progmem_size;
  // create parse buffer
  char buffer[progmem_size];
  // Read into local
  memcpy_P(buffer,LIGHTING_TEMPLATE,sizeof(LIGHTING_TEMPLATE));
  AddLog_P(LOG_LEVEL_INFO, PSTR("LIGHTING_TEMPLATE" D_SERIAL_NEWLINE_BLOCK " READ = \"%s\""), buffer);

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

  memset(&pwm_channel_test,0,sizeof(pwm_channel_test));

  device = pCONT_set->devices_present;
  
  //subtype = (pCONT_set->light_type & 7) > LST_MAX ? LST_MAX : (pCONT_set->light_type & 7); // Always 0 - LST_MAX (5)
  subtype = LST_RGBCW;
  
  pwm_multi_channels = 0;//pCONT_set->Settings.flag3.pwm_multi_channels;  // SetOption68 - Enable multi-channels PWM instead of Color PWM

  pCONT_set->Settings.pwm_range = PWM_RANGE; //tmp

  // pCONT_set->Settings.dimmer_hw_min = 0;
  // pCONT_set->Settings.dimmer_hw_max =100;
  // pCONT_set->Settings.light_correction = 0;
  
  // hsb_colour = HsbColor(0);

  pCONT_set->Settings.light_settings.light_fade = 1;
  pCONT_set->Settings.light_settings.light_speed = 5*2;
  pCONT_set->power = 1;

  init_PresetColourPalettes();

  //new_color[0] = 255;
  // LightApplyPower(new_color, 1);

  // if (LST_RGBW <= subtype) {
  //   // only change if RGBW or RGBCW
  //   // do not allow independant RGB and WC colors
  //   bool ct_rgb_linked = 0;// !(pCONT_set->Settings.param[P_RGB_REMAP] & 128);
  //   setCTRGBLinked(ct_rgb_linked);
  // }
  setCTRGBLinked(false); //temp

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
  calcLevels();    // calculate the initial values (#8058)

  Serial.println("calcLevels DONE"); Serial.flush();

  if (LST_SINGLE == subtype) {
    pCONT_set->Settings.light_settings.light_color[0] = 255;      // One channel only supports Dimmer but needs max color
  }
  // if (light_type < LT_PWM6) {           // PWM
  //   for (uint32_t i = 0; i < light_type; i++) {
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

  Serial.println("light_init");

//   pCONT_set->light_type = LT_BASIC;                    // Use basic PWM control if SetOption15 = 0

  // if (pCONT_set->Settings.flag.pwm_control) {          // SetOption15 - Switch between commands PWM or COLOR/DIMMER/CT/CHANNEL
  //   for (uint32_t i = 0; i < MAX_PWMS; i++) {
  //     if (PinUsed(pCONT_set->GPIO_PWM1, i)) { pCONT_set->light_type++; }  // Use Dimmer/Color control for all PWM as SetOption15 = 1
  //   }
  // }

//   // post-process for lights
//   if (Settings.flag3.pwm_multi_channels) {  // SetOption68 - Enable multi-channels PWM instead of Color PWM
//     uint32_t pwm_channels = (light_type & 7) > LST_MAX ? LST_MAX : (light_type & 7);
//     if (0 == pwm_channels) { pwm_channels = 1; }
//     devices_present += pwm_channels - 1;    // add the pwm channels controls at the end
//   } else if ((Settings.param[P_RGB_REMAP] & 128) && (LST_RGBW <= (light_type & 7))) {
//     // if RGBW or RGBCW, and SetOption37 >= 128, we manage RGB and W separately, hence adding a device
//     devices_present++;
//   }

//   return (light_type > LT_BASIC);
// }

changeBri(0); //default off


} //light_init


// // Used for timed on or off events
int8_t mInterfaceLight::Tasker(uint8_t function){
  
  int8_t function_result = 0;

  // As interface module, the parsing of module_init takes precedence over the light_type
  if(function == FUNC_TEMPLATE_MODULE_LOAD){
    Template_Load();
  }else
  if(function == FUNC_MODULE_INIT){ //rename interface_init?
    LightModuleInit();
  }else
  if(function == FUNC_PRE_INIT){
    return FUNCTION_RESULT_HANDLED_ID;
  }
  
  // Check if light is being handled by another function eg ws2812 (long term probably included into this as commands pipe into this)
  // if(pCONT_set->light_type == LT_WS2812){ 
  //   // Serial.println("light_type == LT_WS2812"); 
  //   return 0; 
  // }

  // Check if light is being handled by another function eg ws2812 (long term probably included into this as commands pipe into this)
  // if(pCONT_set->light_type > LT_LIGHT_INTERFACE_END){
  //   // Serial.printf("Skipping light type %d\n\r",pCONT_set->light_type); 
  //   return FUNCTION_RESULT_MODULE_DISABLED_ID; 
  // }

  switch(function){
    case FUNC_INIT:
      Init();
    break;
    case FUNC_LOOP:{
      

    }break;
    case FUNC_EVERY_SECOND:{

      // changeDimmer(random(0,100));
      // LightPreparePower(2);
      // _r = random(0,5)*50;
      // calcLevels();
    
    }break;
    case FUNC_EVERY_MINUTE:

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
      //Append_Hardware_Status_Message();
    break;
  } // end switch
  
  /************
   * WEBPAGE SECTION * 
  *******************/
  return Tasker_Web(function);

} // END function // END FUNCTION
int8_t mInterfaceLight::Tasker(uint8_t function, JsonObjectConst obj){
  switch(function){
    case FUNC_JSON_COMMAND_OBJECT:
      parsesub_CheckAll(obj);
    break;
    case FUNC_JSON_COMMAND_OBJECT_WITH_TOPIC:
      return CheckAndExecute_JSONCommands(obj);
    break;
  }
}


int8_t mInterfaceLight::CheckAndExecute_JSONCommands(JsonObjectConst obj){

  // Check if instruction is for me
  if(mSupport::mSearchCtrIndexOf(data_buffer2.topic.ctr,"set/light")>=0){
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND "mInterfaceLight"));
      pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
      parsesub_TopicCheck_JSONCommand(obj);
      return FUNCTION_RESULT_HANDLED_ID;
  }else{
    return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
  }

}


int8_t mInterfaceLight::parsesub_TopicCheck_JSONCommand(JsonObjectConst obj){

  int8_t isserviced = 0;
  
  if(mSupport::memsearch(data_buffer2.topic.ctr,data_buffer2.topic.len,"/manual",sizeof("/manual")-1)>=0){
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "manual"));    
    isserviced += parsesub_ModeManual(obj);
  }else  
  {
    AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "INVALID"));    
  }

  return isserviced;

}


// Directly change and control flags/elements without changing everything ie update brightness without changing to scene
int8_t mInterfaceLight::parsesub_Settings(JsonObjectConst obj){
  
  int8_t isserviced = -1;

  if(!obj[D_JSON_HARDWARE_TYPE].isNull()){ 
    const char* onoff = obj[D_JSON_HARDWARE_TYPE];
    if(strstr(onoff,"RGBCCT_PWM")){ 
      // #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_HARDWARE_TYPE, "RGBCCT_PWM");
      // #endif
      pCONT_set->light_type = LT_PWM5;
      isserviced++;
    }else 
    if(strstr(onoff,"WS2812")){
      // #ifdef ENABLE_LOG_LEVEL_INFOdd
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_HARDWARE_TYPE, "WS2812");
      // #endif
      pCONT_set->light_type = LT_WS2812;
      isserviced++;
    }else{
      AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_NOMATCH));
    }
  }

  return isserviced;

} // END FUNCTION


int8_t mInterfaceLight::parsesub_CheckAll(JsonObjectConst obj){

  parsesub_Settings(obj); 
  parsesub_ModeManual(obj);

}


/*******************************************************************************************************************
********************************************************************************************************************
************MQTT**************************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/


int8_t mInterfaceLight::parsesub_ModeManual(JsonObjectConst obj){

  int8_t tmp_id = 0;
  int8_t isserviced = 0;
  
  if(!obj[D_JSON_ONOFF].isNull()){ 
    uint8_t value = obj[D_JSON_ONOFF];
    setBri(value ? 255 : 0);
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_ONOFF,0);
    isserviced++;
  }

  if(!obj[D_JSON_HUE].isNull()){ 
    uint16_t hue = obj[D_JSON_HUE];
    changeHSB_Hue(hue);
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_HUE,hue);
    isserviced++;
  }

  if(!obj[D_JSON_SAT].isNull()){    // Assume range 0-100
    uint8_t sat = obj[D_JSON_SAT];
    sat = sat > 100 ? 100 : sat;
    sat = map(sat, 0,100, 0,255);
    changeHSB_Sat(sat);
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_SAT,sat);
    isserviced++;
  }

  if(!obj[D_JSON_BRT].isNull()){    // Assume range 0-100
    uint8_t brt = obj[D_JSON_BRT];
    brt = brt > 100 ? 100 : brt;
    brt = map(brt, 0,100, 0,255);
    changeHSB_Brt(brt);
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_BRT,brt);
    isserviced++;
  }
  if(!obj[D_JSON_BRT_RGB].isNull()){    // Assume range 0-100
    uint8_t brt = obj[D_JSON_BRT_RGB];
    brt = brt > 100 ? 100 : brt;
    brt = map(brt, 0,100, 0,255);
    setBriRGB(brt);
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_BRT_RGB,brt);
    isserviced++;
  }
  if(!obj[D_JSON_BRT_CCT].isNull()){    // Assume range 0-100
    uint8_t brt = obj[D_JSON_BRT_CCT];
    brt = brt > 100 ? 100 : brt;
    brt = map(brt, 0,100, 0,255);
    setBriCT(brt);
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_BRT_CCT,brt);
    isserviced++;
  }

  if(!obj[D_JSON_CCT_PERCENTAGE].isNull()){    // Assume range 0-100
    uint16_t cct_value = obj[D_JSON_CCT_PERCENTAGE];
    LightSetColorTemp(cct_value);
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_CCT_PERCENTAGE,cct_value);
    isserviced++;
  }
  if(!obj[D_JSON_CCT_TEMP].isNull()){    // Assume range 0-100
    uint16_t cct_value = obj[D_JSON_CCT_TEMP];
    LightSetColorTemp(cct_value);
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_CCT_TEMP,cct_value);
    isserviced++;
  }

  if(!obj[D_JSON_RGBCCT_LINKED].isNull()){    // Assume range 0-100
    uint8_t value = obj[D_JSON_RGBCCT_LINKED];
    setCTRGBLinked(value);
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_RGBCCT_LINKED,value);
    isserviced++;
  }
  
  // TIME on duration for autooff
  if(!obj[D_JSON_TIME_ON].isNull()){ //default to secs
    auto_time_off_secs = obj[D_JSON_TIME_ON];
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),auto_time_off_secs);  
  }else
  if(!obj[D_JSON_TIME_ON_SECS].isNull()){
    auto_time_off_secs = obj[D_JSON_TIME_ON_SECS];
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),auto_time_off_secs);  
  }else
  if(!obj[D_JSON_TIME_ON_MS].isNull()){
    auto_time_off_secs = obj[D_JSON_TIME_ON_MS];
    auto_time_off_secs /= 1000;
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),auto_time_off_secs);  
  }
  
  return isserviced;

} // END FUNCTION




















































HsbColor mInterfaceLight::GetColour_HSB(void){
  uint16_t hue = 0;
  uint8_t  sat = 0;
  uint8_t  brt = 0;
  getHSB(&hue,&sat,&brt);  
  return HsbColor(hue,sat,brt);
}





//Scene should be rolled into this?
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

  memset(&data_buffer2,0,sizeof(data_buffer2));
  // pCONT_sup->WriteBuffer_P(data_buffer2.payload.ctr,
  //   PSTR("{"
  //     "\"" "hue"                  "\":%d,"
  //     "\"" "sat"                  "\":%d,"
  //     "\"" "briRGB"               "\":%d,"
  //     "\"" "r"                  "\":%d,"
  //     "\"" "g"                  "\":%d,"
  //     "\"" "b"                  "\":%d,"
  //     "\"" "subtype"      "\":%d,"
  //     "\"" "ct"           "\":%d,"
  //     "\"" "wc"           "\":%d,"
  //     "\"" "ww"           "\":%d,"
  //     "\"" "briCT"        "\":%d,"
  //     "\"" "colour_mode"  "\":%d,"
  //     "\"" "_ct_rgb_linked"  "\":%d,"
  //     "\"" "fade.running"  "\":%d,"
  //     "\"" "fade.start_ms"  "\":%d,"
  //     "\"" "fade.duration_ms"  "\":%d,"
  //     "\"" "fade.end_ms"  "\":%d,"
  //     "\"" "entry_color"  "\":[%d,%d,%d,%d,%d],"
  //     "\"" "current_color"  "\":[%d,%d,%d,%d,%d],"
  //     "\"" "new_color"  "\":[%d,%d,%d,%d,%d],"
  //     "\"" "last_color"  "\":[%d,%d,%d,%d,%d],"
  //     // "\"" "color_remap"  "\":[%d,%d,%d,%d,%d],"
  //     "\"" "fade_start_10"  "\":[%d,%d,%d,%d,%d],"
  //     "\"" "fade_cur_10"  "\":[%d,%d,%d,%d,%d],"
  //     "\"" "fade_end_10"  "\":[%d,%d,%d,%d,%d],"
  //     "\"" "fade_duration"  "\":%d,"
  //     "\"" "fade_start"  "\":%d,"
  //     "\"" "pwm_min"  "\":%d,"
  //     "\"" "pwm_max"  "\":%d,"
  //     "\"" "_pwm_multi_channels"  "\":%d,"
  //     "\"" "ct_min_range"  "\":%d,"
  //     "\"" "ct_max_range"  "\":%d,"
  //     "\"" "light_type"  "\":%d"
  //   "}"),
  //   _hue,
  //   _sat,
  //   _briRGB,
  //   _r,
  //   _g,
  //   _b,
  //   _subtype,
  //   _ct,
  //   _wc,
  //   _ww,
  //   _briCT,
  //   _color_mode,
  //   _ct_rgb_linked,
  //   fade.running,
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
  //   pCONT_set->light_type
  // );

  return strlen(data_buffer2.payload.ctr) ? 1 : 0;

}






////////////////////// START OF MQTT /////////////////////////

void mInterfaceLight::MQTTHandler_Init(){

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->fPeriodicEnabled = true;
  mqtthandler_ptr->fSendNow = true;
  mqtthandler_ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.configperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = postfix_topic_settings;
  mqtthandler_ptr->ConstructJSON_function = &mInterfaceLight::ConstructJSON_Settings;

  // mqtthandler_ptr = &mqtthandler_animation_teleperiod;
  // mqtthandler_ptr->tSavedLastSent = millis();
  // mqtthandler_ptr->fPeriodicEnabled = true;
  // mqtthandler_ptr->fSendNow = true;
  // mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  // mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  // mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  // mqtthandler_ptr->postfix_topic = postfix_topic_animation;
  // mqtthandler_ptr->ConstructJSON_function = &mInterfaceLight::ConstructJSON_Animation;

  // mqtthandler_ptr = &mqtthandler_ambilight_teleperiod;
  // mqtthandler_ptr->tSavedLastSent = millis();
  // mqtthandler_ptr->fPeriodicEnabled = true;
  // mqtthandler_ptr->fSendNow = true;
  // mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
  // mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  // mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  // mqtthandler_ptr->postfix_topic = postfix_topic_ambilight;
  // mqtthandler_ptr->ConstructJSON_function = &mInterfaceLight::ConstructJSON_Ambilight;
  
} //end "MQTTHandler_Init"


void mInterfaceLight::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.fSendNow = true;
  // mqtthandler_animation_teleperiod.fSendNow = true;
  // mqtthandler_ambilight_teleperiod.fSendNow = true;

} //end "MQTTHandler_Init"


void mInterfaceLight::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  // mqtthandler_animation_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  // mqtthandler_ambilight_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mInterfaceLight::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t flag_handle_all = false, handler_not_found = true;
  if(mqtt_handler_id == MQTT_HANDLER_ALL_ID){ flag_handle_all = true; } //else run only the one asked for

  do{

    //AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_NEO "MQTTHandler_Sender %d"),mqtt_handler_id);

    handler_not_found=false; // start as true, if none found, default clears to false
    switch(mqtt_handler_id){
      case MQTT_HANDLER_SETTINGS_ID:                       mqtthandler_ptr=&mqtthandler_settings_teleperiod; break;
      // case MQTT_HANDLER_MODULE_ANIMATION_TELEPERIOD_ID:    mqtthandler_ptr=&mqtthandler_animation_teleperiod; break;
      // case MQTT_HANDLER_MODULE_AMBILIGHT_TELEPERIOD_ID:    mqtthandler_ptr=&mqtthandler_ambilight_teleperiod; break;
      default: handler_not_found=true; break; // nothing 
    } // switch

    // Pass handlers into command to test and (ifneeded) execute
    if(!handler_not_found){ pCONT->mqt->MQTTHandler_Command(*this,D_MODULE_LIGHTS_INTERFACE_ID,mqtthandler_ptr); }

    // stop searching
    if(mqtt_handler_id++>MQTT_HANDLER_MODULE_LENGTH_ID){flag_handle_all = false; return;}

  }while(flag_handle_all);

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
#ifdef DEBUG_LIGHT
  AddLog_P(LOG_LEVEL_TEST, PSTR("setColorMode prev_cm (%d) req_cm (%d) new_cm (%d)"), prev_cm, cm, _color_mode);
#endif
  return prev_cm;
}


uint8_t mInterfaceLight::getColorMode() {
  return _color_mode;
}

void mInterfaceLight::addRGBMode() {  
  AddLog_P(LOG_LEVEL_TEST, PSTR("f::addRGBMode"));
  setColorMode(_color_mode | LCM_RGB);
}
void mInterfaceLight::addCTMode() {
  AddLog_P(LOG_LEVEL_TEST, PSTR("f::addCTMode"));
  setColorMode(_color_mode | LCM_CT);
}

// Get RGB color, always at full brightness (ie. one of the components is 255)
void mInterfaceLight::getRGB(uint8_t *r, uint8_t *g, uint8_t *b) {
  if (r) { *r = _r; }
  if (g) { *g = _g; }
  if (b) { *b = _b; }
}

// get full brightness values for warm and cold channels.
// either w=c=0 (off) or w+c >= 255
void mInterfaceLight::getCW(uint8_t *rc, uint8_t *rw) {
  if (rc) { *rc = _wc; }
  if (rw) { *rw = _ww; }
}

// Get the actual values for each channel, ie multiply with brightness
void mInterfaceLight::getActualRGBCW(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *c, uint8_t *w) {

  bool rgb_channels_on = _color_mode & LCM_RGB;
  bool ct_channels_on =   _color_mode & LCM_CT;

  if (r) { *r = rgb_channels_on ? mSupport::changeUIntScale(_r, 0, 255, 0, _briRGB) : 0; }
  if (g) { *g = rgb_channels_on ? mSupport::changeUIntScale(_g, 0, 255, 0, _briRGB) : 0; }
  if (b) { *b = rgb_channels_on ? mSupport::changeUIntScale(_b, 0, 255, 0, _briRGB) : 0; }

  if (c) { *c = ct_channels_on  ? mSupport::changeUIntScale(_wc, 0, 255, 0, _briCT) : 0; }
  if (w) { *w = ct_channels_on  ? mSupport::changeUIntScale(_ww, 0, 255, 0, _briCT) : 0; }

}

uint8_t mInterfaceLight::getChannels(uint8_t *channels) {
  getActualRGBCW(&channels[0], &channels[1], &channels[2], &channels[3], &channels[4]);
}

void mInterfaceLight::getChannelsRaw(uint8_t *channels) {
    channels[0] = _r;
    channels[1] = _g;
    channels[2] = _b;
    channels[3] = _wc;
    channels[4] = _ww;
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
  AddLog_P(LOG_LEVEL_TEST, PSTR("setBri RGB raw (%d %d %d) HS (%d %d) bri (%d)"), _r, _g, _b, _hue, _sat, _briRGB);
#endif
}

// changes the RGB brightness alone
uint8_t mInterfaceLight::setBriRGB(uint8_t bri_rgb) {
  uint8_t prev_bri = _briRGB;
  _briRGB = bri_rgb;
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
    _ww = mSupport::changeUIntScale(ct, _ct_min_range, _ct_max_range, 0, 255);
    _wc = 255 - _ww;
    _ct = ct;
    addCTMode();
  }
#ifdef DEBUG_LIGHT
  AddLog_P(LOG_LEVEL_INFO, PSTR("setCT RGB raw (%d %d %d) HS (%d %d) briRGB (%d) briCT (%d) CT (%d)"), _r, _g, _b, _hue, _sat, _briRGB, _briCT, _ct);
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
      _ww = mSupport::changeUIntScale(w, 0, sum, 0, 255);
      _wc = 255 - _ww;
    } else {  // we normalize to max = 255
      _ww = mSupport::changeUIntScale(w, 0, max, 0, 255);
      _wc = mSupport::changeUIntScale(c, 0, max, 0, 255);
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
  _r = r;
  _g = g;
  _b = b;
  _hue = hue;
  _sat = sat;
#ifdef DEBUG_LIGHT
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("setRGB RGB raw (%d %d %d) HS (%d %d) bri (%d)"), _r, _g, _b, _hue, _sat, _briRGB);
#endif
  return max;
}

void mInterfaceLight::setHS(uint16_t hue, uint8_t sat) {
  uint8_t r, g, b;
  HsToRgb(hue, sat, &r, &g, &b);
  _r = r;
  _g = g;
  _b = b;
  _hue = hue;
  _sat = sat;
  addRGBMode();
#ifdef DEBUG_LIGHT
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("setHS HS (%d %d) rgb (%d %d %d)"), hue, sat, r, g, b);
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("setHS RGB raw (%d %d %d) HS (%d %d) bri (%d)"), _r, _g, _b, _hue, _sat, _briRGB);
#endif
}

// set all 5 channels at once, don't modify the values in ANY way
// Channels are: R G B CW WW
void mInterfaceLight::setChannelsRaw(uint8_t *channels) {
  _r = channels[0];
  _g = channels[1];
  _b = channels[2];
  _wc = channels[3];
  _ww = channels[4];
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
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("setChannels Actuals (%d %d %d %d %d)"), _r, _g, _b, _wc, _ww);
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
  // Apply gamma correction for 8 and 10 bits resolutions, if needed

  // First apply combined correction to the overall white power
  if ((LST_COLDWARM == subtype) || (LST_RGBCW == subtype)) {
    // channels for white are always the last two channels
    uint32_t cw1 = subtype - 1;       // address for the ColorTone PWM
    uint32_t cw0 = subtype - 2;       // address for the White Brightness PWM
    uint16_t white_bri10 = cur_col_10[cw0] + cur_col_10[cw1];   // cumulated brightness
    uint16_t white_bri10_1023 = (white_bri10 > 1023) ? 1023 : white_bri10;    // max 1023

#ifdef ESP8266
    if (
      // (MODULE_PHILIPS_ID == pCONT_set->my_module_type) || 
      (pCONT_set->Settings.flag4.pwm_ct_mode)) {   // channel 1 is the color tone, mapped to cold channel (0..255)
      // Xiaomi Philips bulbs follow a different scheme:
      cur_col_10[cw1] = getCT10bits();
      // channel 0=intensity, channel1=temperature
      if (pCONT_set->Settings.light_settings.light_correction) { // gamma correction
        cur_col_10[cw0] = ledGamma10_10(white_bri10_1023);    // 10 bits gamma correction
      } else {
        cur_col_10[cw0] = white_bri10_1023;  // no gamma, extend to 10 bits
      }
    } else
#endif  // ESP8266
    if (pCONT_set->Settings.light_settings.light_correction) {
      // if sum of both channels is > 255, then channels are probably uncorrelated
      if (white_bri10 <= 1031) {      // take a margin of 8 above 1023 to account for rounding errors
        // we calculate the gamma corrected sum of CW + WW
        uint16_t white_bri_gamma10 = ledGamma10_10(white_bri10_1023);
        // then we split the total energy among the cold and warm leds
        cur_col_10[cw0] = mSupport::changeUIntScale(cur_col_10[cw0], 0, white_bri10_1023, 0, white_bri_gamma10);
        cur_col_10[cw1] = mSupport::changeUIntScale(cur_col_10[cw1], 0, white_bri10_1023, 0, white_bri_gamma10);
      } else {
        cur_col_10[cw0] = ledGamma10_10(cur_col_10[cw0]);
        cur_col_10[cw1] = ledGamma10_10(cur_col_10[cw1]);
      }
    }
  }

  if (pCONT_set->Settings.light_settings.light_correction) {
    // then apply gamma correction to RGB channels
    if (LST_RGB <= subtype) {
      for (uint32_t i = 0; i < 3; i++) {
        cur_col_10[i] = ledGamma10_10(cur_col_10[i]);
      }
    }
    // If RGBW or Single channel, also adjust White channel
    if ((LST_SINGLE == subtype) || (LST_RGBW == subtype)) {
      cur_col_10[subtype - 1] = ledGamma10_10(cur_col_10[subtype - 1]);
    }
  }
}


bool mInterfaceLight::isChannelGammaCorrected(uint32_t channel) {
  if (!pCONT_set->Settings.light_settings.light_correction) { return false; }   // Gamma correction not activated
  if (channel >= subtype) { return false; }     // Out of range
#ifdef ESP8266
  if (
    // (MODULE_PHILIPS_ID == pCONT_set->my_module_type) || 
  (pCONT_set->Settings.flag4.pwm_ct_mode)) {
    if ((LST_COLDWARM == subtype) && (1 == channel)) { return false; }   // PMW reserved for CT
    if ((LST_RGBCW == subtype) && (4 == channel)) { return false; }   // PMW reserved for CT
  }
#endif  // ESP8266
  return true;
}

// is the channel a regular PWM or ColorTemp control
bool mInterfaceLight::isChannelCT(uint32_t channel) {
  if (
    // (MODULE_PHILIPS_ID == pCONT_set->my_module_type) || 
    (pCONT_set->Settings.flag4.pwm_ct_mode)) {
    if ((LST_COLDWARM == subtype) && (1 == channel)) { return true; }   // PMW reserved for CT
    if ((LST_RGBCW == subtype) && (4 == channel)) { return true; }   // PMW reserved for CT
  }
  return false;
}





void mInterfaceLight::changeCTB(uint16_t new_ct, uint8_t briCT) {
  /* Color Temperature (https://developers.meethue.com/documentation/core-concepts)
    *
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
  calcLevels();
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
  calcLevels();
}

void mInterfaceLight::changeBriRGB(uint8_t bri) {
  setBriRGB(bri);
  // saveSettings();
  calcLevels();
}

void mInterfaceLight::changeBriCT(uint8_t bri) {
  setBriCT(bri);
  // saveSettings();
  calcLevels();
}

void mInterfaceLight::changeRGB(uint8_t r, uint8_t g, uint8_t b, bool keep_bri) {
  setRGB(r, g, b, keep_bri);
  if (_ct_rgb_linked) { 
  AddLog_P(LOG_LEVEL_TEST, PSTR("f::addCTMode ct linked rgb changeRGB"));
    setColorMode(LCM_RGB); }   // try to force RGB
  // saveSettings();
  calcLevels();
}

//   calculate the levels for each channel
//   if no parameter, results are stored in current_color
void mInterfaceLight::calcLevels(uint8_t *_current_color) {

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

  getActualRGBCW(&r,&g,&b,&c,&w);
  briRGB = getBriRGB();
  briCT  = getBriCT();

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
  
    // AddLog_P(LOG_LEVEL_TEST, "_current_color (%d %d %d %d %d)",
    //   _current_color[0], 
    //   _current_color[1], 
    //   _current_color[2], 
    //   _current_color[3], 
    //   _current_color[4]
    // );

  DEBUG_LINE;

}

void mInterfaceLight::changeHSB(uint16_t hue, uint8_t sat, uint8_t briRGB) {
  setHS(hue, sat);
  setBriRGB(briRGB);
  if (_ct_rgb_linked) { setColorMode(LCM_RGB); }   // try to force RGB
  // saveSettings();
  calcLevels();
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
  calcLevels();
}

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

/**********
 * 
 * *    Helper functions (non setters)
 * 


 ***/



 
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


void mInterfaceLight::LightCycleColor(int8_t direction)
{
// //  if (strip_timer_counter % (Settings.light_speed * 2)) { return; }  // Speed 1: 24sec, 2: 48sec, 3: 72sec, etc
//   if (Settings.light_speed > 3) {
//     if (strip_timer_counter % (Settings.light_speed - 2)) { return; }  // Speed 4: 24sec, 5: 36sec, 6: 48sec, etc
//   }

// #ifdef USE_LIGHT_PALETTE
//   if (palette_count) {
//     if (!fade_running) {
//       if (0 == direction) {
//         wheel = random(palette_count);
//       }
//       else {
//         wheel += direction;
//         if (wheel >= palette_count) {
//           wheel = 0;
//           if (direction < 0) wheel = palette_count - 1;
//         }
//       }
//       LightSetPaletteEntry();
//     }
//     return;
//   }
// #endif  // USE_LIGHT_PALETTE

//   if (0 == direction) {
//     if (random == wheel) {
//       random = random(255);

//       uint8_t my_dir = (random < wheel -128) ? 1 :
//                        (random < wheel     ) ? 0 :
//                        (random > wheel +128) ? 0 : 1;  // Increment or Decrement and roll-over
//       random = (random & 0xFE) | my_dir;

// //      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("LGT: random %d"), random);
//     }
// //    direction = (random < wheel) ? -1 : 1;
//     direction = (random &0x01) ? 1 : -1;
//   }

// //  if (Settings.light_speed < 3) { direction <<= (3 - Settings.light_speed); }  // Speed 1: 12/4=3sec, 2: 12/2=6sec, 3: 12sec
//   if (Settings.light_speed < 3) { direction *= (4 - Settings.light_speed); }  // Speed 1: 12/3=4sec, 2: 12/2=6sec, 3: 12sec
//   wheel += direction;
//   uint16_t hue = mSupport::changeUIntScale(wheel, 0, 255, 0, 359);  // Scale to hue to keep amount of steps low (max 255 instead of 359)

// //  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("LGT: random %d, wheel %d, hue %d"), random, wheel, hue);

//   if (!pwm_multi_channels) {
//   uint8_t sat;
//   light_state.getHSB(nullptr, &sat, nullptr);  // Allow user control over Saturation
//   light_state.setHS(hue, sat);
//   } else {
//     light_state.setHS(hue, 255);
//     light_state.setBri(255);        // If multi-channel, force bri to max, it will be later dimmed to correct value
//   }
//   light_controller.calcLevels(new_color);
}

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
  //   light_controller.calcLevels();      // recalculate levels only if Scheme 0, otherwise we mess up levels
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




//   void loadSettings() {
// #ifdef DEBUG_LIGHT
//     AddLog_P2(LOG_LEVEL_TEST, "LightControllerClass::loadSettings Settings.light_color (%d %d %d %d %d - %d)",
//       Settings.light_color[0], Settings.light_color[1], Settings.light_color[2],
//       Settings.light_color[3], Settings.light_color[4], Settings.light_dimmer);
//     AddLog_P2(LOG_LEVEL_TEST, "LightControllerClass::loadSettings light_type/sub (%d %d)",
//       light_type, subtype);
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
  AddLog_P(LOG_LEVEL_WARN,PSTR("init_PresetColourPalettes"));
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



  // // for(uint8_t id=0;id<400;id++){
  // AddLog_Array(LOG_LEVEL_TEST, "test3", 
  //   &pCONT_set->Settings.animation_settings.palette_user_colour_map_ids[0],
  //   (uint8_t)400);//palettelist.ptr->colour_map_size);
  // // }


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
  AddLog_Array_P(LOG_LEVEL_TEST, "Grad01", palettelist.ptr->colour_map_id,palettelist.ptr->colour_map_size);
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
  AddLog_Array_P(LOG_LEVEL_TEST, "Grad02", palettelist.ptr->colour_map_id,palettelist.ptr->colour_map_size);
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
    if(ptr->friendly_name_ctr == nullptr){ return 0;}
    if(strstr(c,ptr->friendly_name_ctr)){
      return ii;
    }
  }

DEBUG_LINE;
  // Check for default user names
  char name_ctr[100];
  for(uint8_t ii=0;ii<20;ii++){
    memset(name_ctr,0,sizeof(name_ctr));
    sprintf(name_ctr,PSTR("%s %02d\0"),D_PALETTE_USER_NAME_CTR,ii);
    // Default names
    if(strstr(c,name_ctr)){
      return pCONT_iLight->PALETTELIST_VARIABLE_USER_01_ID+ii;
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
    case PALETTELIST_STATIC_SHELF_HEARTS_ID:   return &palettelist.shelf_hearts;
    case PALETTELIST_STATIC_HOLLOWEEN_OP_ID:  return &palettelist.holloween_op;
    case PALETTELIST_STATIC_HOLLOWEEN_OGP_ID: return &palettelist.holloween_ogp;
    case PALETTELIST_STATIC_RAINBOW_ID:       return &palettelist.rainbow;
    case PALETTELIST_STATIC_RAINBOW_INVERTED_ID:       return &palettelist.rainbow_inverted;
    case PALETTELIST_STATIC_PASTEL_ID:        return &palettelist.pastel;
    case PALETTELIST_STATIC_WINTER_ID:        return &palettelist.winter;
    case PALETTELIST_STATIC_AUTUMN_ID:        return &palettelist.autumn;
    case PALETTELIST_STATIC_AUTUMN_RED_ID:    return &palettelist.autumn_red;
    case PALETTELIST_STATIC_OCEAN_01_ID:       return &palettelist.ocean_01;
    case PALETTELIST_STATIC_GRADIENT_01_ID:         return &palettelist.gradient_01;
    case PALETTELIST_STATIC_GRADIENT_02_ID:         return &palettelist.gradient_02;
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
  return GetPaletteFriendlyNameByID(pCONT_ladd->animation.palette_id, buffer);
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
    return ptr->friendly_name_ctr!=nullptr?ptr->friendly_name_ctr:WARNING_NOTHANDLED_CTR;//"NULL\0";
  }
}



const char* mInterfaceLight::GetPaletteName(char* buffer){
  
#ifdef USE_MODULE_LIGHTS_ADDRESSABLE
  return GetPaletteNameByID(pCONT_ladd->animation.palette_id, buffer);
  #else
  return GetPaletteNameByID(0, buffer);

  #endif
} 
const char* mInterfaceLight::GetPaletteNameByID(uint8_t id, char* buffer){  
  PALETTELIST::PALETTE *ptr = GetPalettePointerByID(id);
  return ptr->friendly_name_ctr!=nullptr?ptr->friendly_name_ctr:WARNING_NOTHANDLED_CTR;//"NULL\0";
  
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
  // char* test = "test\0"; //GetDefaultColourPaletteUserIDs_P(id)

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


HsbColor mInterfaceLight::GetColourFromPalette(PALETTELIST::PALETTE *ptr, uint16_t pixel_num, int16_t *pixel_position)
{

  HsbColor colour = HsbColor(0);

  // if(ptr->id >= PALETTELIST_VARIABLE_USER_LENGTH_ID){ //temp progmem issue
  //   return colour;
  // }

  //load palette
  uint8_t palette_elements[ptr->colour_map_size];
  
  if(ptr->id < PALETTELIST_VARIABLE_USER_LENGTH_ID){
    memcpy(&palette_elements,ptr->colour_map_id,sizeof(uint8_t)*ptr->colour_map_size);
  }
  // ptr
  else{
    memcpy_P(&palette_elements,ptr->colour_map_id,sizeof(uint8_t)*ptr->colour_map_size);
  }

  *pixel_position = -1;

  #ifdef ENABLE_LOG_LEVEL_DEBUG
  AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "fMapIDs_Type=%d"),ptr->flags.fMapIDs_Type);
  #endif

  switch(ptr->flags.fMapIDs_Type){
    case MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID: default:{
      *pixel_position = 1;
      
      #ifdef DONT_USE_OLD_PRESETS
      colour = preset_colour_map[palette_elements[pixel_num]]; // direct
      #else
      colour = GetHsbColour(palette_elements[pixel_num]);
      #endif
    }
    break;
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_GRADIENT_ID:{

      uint16_t index_relative = pixel_num*4; // get expected pixel position
      uint16_t expected_pixel_count = ptr->colour_map_size/4; // get expected pixel position
      
      *pixel_position = palette_elements[index_relative];
      // Get colour
      colour = HsbColor(RgbColor(
        palette_elements[index_relative+1],
        palette_elements[index_relative+2],
        palette_elements[index_relative+3]
      ));

    }break;
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID:
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_AND_SETALL_ID: //0,h,s,b (only if uint16 is allowed for hue>255)
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_ID:{
      
      uint16_t index_relative = pixel_num*2; // get expected pixel position
      uint16_t expected_pixel_count = ptr->colour_map_size/2; // get expected pixel position
      
      *pixel_position = palette_elements[index_relative];
      // colour = preset_colour_map[palette_elements[index_relative+1]];

      #ifdef DONT_USE_OLD_PRESETS
      colour = preset_colour_map[palette_elements[index_relative+1]]; // direct
      #else
      colour = GetHsbColour(palette_elements[index_relative+1]);
      #endif
      
    }
    break;
    case MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID:{
      // Get expected pixel position
      uint16_t index_relative = pixel_num*3;
      // Get colour
      colour = HsbColor(RgbColor(
        palette_elements[index_relative  ],
        palette_elements[index_relative+1],
        palette_elements[index_relative+2]
      ));
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
uint32_t mInterfaceLight::WebColorFromHsbColour(RgbColor rgb)
{
  uint32_t tcolor = (rgb.R << 16) | (rgb.G << 8) | rgb.B;
  return tcolor;
}
uint32_t mInterfaceLight::WebColorFromHsbColour(HsbColor hsb)
{
  RgbColor rgb = RgbColor(hsb);
  uint32_t tcolor = (rgb.R << 16) | (rgb.G << 8) | rgb.B;
  return tcolor;
}
uint32_t mInterfaceLight::WebColorFromHsbColour(RgbwColor rgbw)
{
  uint32_t tcolor = (rgbw.R << 16) | (rgbw.G << 8) | rgbw.B;
  return tcolor;
}







#endif // USE_DRIVER
