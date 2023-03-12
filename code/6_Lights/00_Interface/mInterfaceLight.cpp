#include "mInterfaceLight.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE 

const char* mInterfaceLight::PM_MODULE_LIGHTS_INTERFACE_CTR = D_MODULE_LIGHTS_INTERFACE_CTR;
const char* mInterfaceLight::PM_MODULE_LIGHTS_INTERFACE_FRIENDLY_CTR = D_MODULE_LIGHTS_INTERFACE_FRIENDLY_CTR;

void mInterfaceLight::Pre_Init(void)
{

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

}


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
      Template_Load();
    break;
    case FUNC_POINTER_INIT:

      #ifdef ENABLE_DEVFEATURE_REMOVE_INIT_OUTSIDE_OF_PALETTE_CLASS
      mPaletteI->init_PresetColourPalettes();
      #endif // ENABLE_DEVFEATURE_REMOVE_INIT_OUTSIDE_OF_PALETTE_CLASS

    break;
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
    case FUNC_INIT:
      Init();
    break;
  }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
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
  
} // END function


// void mInterfaceLight::ShowInterface(){

//   // ALOG_INF(PSTR("t=%d"),pCONT_set->Settings.light_settings.type);

//   switch(pCONT_set->Settings.light_settings.type)
//   {
//     case LT_ADDRESSABLE_WS281X:  
//     case LT_ADDRESSABLE_SK6812:
//       #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
//       return pCONT_ladd->ShowHardware();
//       #endif // USE_MODULE_LIGHTS_ADDRESSABLE
//     break;
//     case LT_PWM1:
//     case LT_PWM2:
//     case LT_PWM3:
//     case LT_PWM4:
//     case LT_PWM5:
//       #ifdef USE_MODULE_LIGHTS_PWM
//       return pCONT_lPWM->ShowHardware();
//       #endif // USE_MODULE_LIGHTS_PWM
//     break;
//   }

// } 



void mInterfaceLight::EveryLoop()
{

  ALOG_DBM(PSTR("mInterfaceLight::EveryLoop()"));
        
} // END everyloop



















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
  if (channel >= pCONT_lAni->subtype) { return false; }     // Out of range
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

  // ALOG_DBM( PSTR(D_LOG_LIGHT D_TOPIC "mInterfaceLight Checking all commands %d"),obj.isNull());

  char buffer[50];
  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

  // uint16_t isserviced_start_count = data_buffer.isserviced;
  
}





/******************************************************************************************************************
 * mInterfaceLight_Web.cpp
*******************************************************************************************************************/



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
