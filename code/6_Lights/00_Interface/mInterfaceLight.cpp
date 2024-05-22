#include "mInterfaceLight.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE 

const char* mInterfaceLight::PM_MODULE_LIGHTS_INTERFACE_CTR = D_MODULE_LIGHTS_INTERFACE_CTR;
const char* mInterfaceLight::PM_MODULE_LIGHTS_INTERFACE_FRIENDLY_CTR = D_MODULE_LIGHTS_INTERFACE_FRIENDLY_CTR;

int8_t mInterfaceLight::Tasker(uint8_t function, JsonParserObject obj)
{
  
  int8_t function_result = 0;

  // As interface module, the parsing of module_init takes precedence over the Settings.light_settings.type
  switch(function){
    case FUNC_TEMPLATE_DEVICE_LOAD_FROM_PROGMEM:
      Template_Load();
    break;
    case FUNC_POINTER_INIT:

    break;
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
    case FUNC_INIT:
      Init();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP:
      EveryLoop();
    break;
    /************
     * STORAGE SECTION * 
    *******************/    
    case FUNC_TEMPLATE_MODULE_LOAD_AFTER_INIT_DEFAULT_CONFIG_ID:
      Template_Load_DefaultConfig();
    break;
    #ifdef ENABLE_DEVFEATURE__SAVE_MODULE_DATA
    case FUNC_FILESYSTEM__SAVE__MODULE_DATA__ID:
      Save_Module();
    break;
    #endif
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
    /************
     * WEB SECTION * 
    *******************/   
    case FUNC_WEB_ADD_HANDLER:    
      #ifdef USE_MODULE_NETWORK_WEBSERVER
      // MQTTHandler_AddWebURL_PayloadRequests(); // Therefore MQTT must be initialised before webui
      #endif
    break;

  } // end switch
  
} // END function


void mInterfaceLight::Pre_Init(void)
{

  bus_manager = new BusManager();

}


void mInterfaceLight::Template_Load()
{

  bool template_loaded = false;

  /**
   * @brief Check first if alternate templates are being used, to be selected by pin
   * 
   */
  #ifdef ENABLE_FEATURE_LIGHTING__ENABLE_ALTERANTE_TEMPLATE_USING_GPIO_SWITCH_ONE

  if(pCONT_pins->PinUsed(GPIO_SWT1_INV_ID))
  {
    if(pCONT_pins->DigitalRead(GPIO_SWT1_INV_ID)==0) // switch to LOW to activate alternate template
    {
      template_loaded = true;
          
      ALOG_INF(PSTR("buffer_writer Template_Load ------- A >>>>>>>>>> %d"),JBI->GetBufferSize());
      D_DATA_BUFFER_CLEAR();


      ALOG_INF(PSTR("buffer_writer STTemplate_LoadART ------F- >>>>>>>>>> %d"),JBI->GetBufferSize());


      // memcpy_P(data_buffer.payload.ctr,LIGHTING_TEMPLATE,sizeof(LIGHTING_TEMPLATE));
      // strncpy_P(data_buffer.payload.ctr,LIGHTING_TEMPLATE,sizeof(data_buffer.payload.ctr));
      memcpy_P(data_buffer.payload.ctr,LIGHTING_TEMPLATE_ALTERNATE,sizeof(LIGHTING_TEMPLATE_ALTERNATE));

      ALOG_INF(PSTR("Tasker_Interface before parser ------------ >>>>>>>>>> %d"), JBI->GetBufferSize());
      data_buffer.payload.length_used = strlen(data_buffer.payload.ctr);

      data_buffer.payload.ctr[data_buffer.payload.length_used] = '\0'; // to avoid need to memset everything

      ALOG_INF(PSTR("Tasker_Interface before parser ------------ >>>>>>>>>> %d"), JBI->GetBufferSize());
      ALOG_HGL(PSTR("Template Json Size %d/%d %d%%"), sizeof(LIGHTING_TEMPLATE_ALTERNATE), data_buffer.payload.length_used, (sizeof(LIGHTING_TEMPLATE_ALTERNATE)*100)/DATA_BUFFER_PAYLOAD_MAX_LENGTH);

      #ifdef ENABLE_DEBUGCRITICAL__STOPPING_CODE_AFTER_TEMPLATE_LOAD
      Serial.println(data_buffer.payload.ctr);
      #endif // ENABLE_DEBUGCRITICAL__STOPPING_CODE_AFTER_TEMPLATE_LOAD


      ALOG_INF(PSTR("Tasker_Interface before parser ------------ >>>>>>>>>> %d"), JBI->GetBufferSize());

      // ALOG_HGL( PSTR("LIGHTING_TEMPLATE" " READ = \"%s\""), data_buffer.payload.ctr);

      pCONT->Tasker_Interface(FUNC_JSON_COMMAND_ID);

      ALOG_INF(PSTR("buffer_writer STTemplate_LoadART ------G- >>>>>>>>>> %d"),JBI->GetBufferSize());
      
      runtime.template_loading.status.lighting = TemplateSource::HEADER_TEMPLATE;

    }

  }

  #endif  // ENABLE_FEATURE_LIGHTING__ENABLE_ALTERANTE_TEMPLATE_USING_GPIO_SWITCH_ONE



  #ifdef USE_LIGHTING_TEMPLATE
  // load from progmem into local

  if(!template_loaded)
  {


  ALOG_INF(PSTR("buffer_writer Template_Load ------- A >>>>>>>>>> %d"),JBI->GetBufferSize());
  D_DATA_BUFFER_CLEAR();

  
  // ALOG_INF(PSTR("buffer_writer STTemplate_LoadART ------B- >>>>>>>>>> %d"),JBI->GetBufferSize());
  //   memset(data_buffer.topic.ctr,0,sizeof(data_buffer.topic.ctr)); 
  // ALOG_INF(PSTR("buffer_writer STTemplate_LoadART ------C- >>>>>>>>>> %d"),JBI->GetBufferSize());
  //   data_buffer.topic.length_used = 0; 
  // ALOG_INF(PSTR("buffer_writer STTemplate_LoadART ------D- >>>>>>>>>> %d"),JBI->GetBufferSize());
  //   memset(data_buffer.payload.ctr,0,sizeof(data_buffer.payload.ctr)); 
  // ALOG_INF(PSTR("buffer_writer STTemplate_LoadART ------E- >>>>>>>>>> %d"),JBI->GetBufferSize());
  //   data_buffer.payload.length_used = 0; 


  ALOG_INF(PSTR("buffer_writer STTemplate_LoadART ------F- >>>>>>>>>> %d"),JBI->GetBufferSize());


  // memcpy_P(data_buffer.payload.ctr,LIGHTING_TEMPLATE,sizeof(LIGHTING_TEMPLATE));
  // strncpy_P(data_buffer.payload.ctr,LIGHTING_TEMPLATE,sizeof(data_buffer.payload.ctr));
  memcpy_P(data_buffer.payload.ctr,LIGHTING_TEMPLATE,sizeof(LIGHTING_TEMPLATE));

  ALOG_INF(PSTR("Tasker_Interface before parser ------------ >>>>>>>>>> %d"), JBI->GetBufferSize());
  data_buffer.payload.length_used = strlen(data_buffer.payload.ctr);

  data_buffer.payload.ctr[data_buffer.payload.length_used] = '\0'; // to avoid need to memset everything

  ALOG_INF(PSTR("Tasker_Interface before parser ------------ >>>>>>>>>> %d"), JBI->GetBufferSize());
  ALOG_HGL(PSTR("Template Json Size %d/%d %d%%"), sizeof(LIGHTING_TEMPLATE), data_buffer.payload.length_used, (sizeof(LIGHTING_TEMPLATE)*100)/DATA_BUFFER_PAYLOAD_MAX_LENGTH);

  #ifdef ENABLE_DEBUGCRITICAL__STOPPING_CODE_AFTER_TEMPLATE_LOAD
  Serial.println(data_buffer.payload.ctr);
  #endif // ENABLE_DEBUGCRITICAL__STOPPING_CODE_AFTER_TEMPLATE_LOAD


  ALOG_INF(PSTR("Tasker_Interface before parser ------------ >>>>>>>>>> %d"), JBI->GetBufferSize());

  // ALOG_HGL( PSTR("LIGHTING_TEMPLATE" " READ = \"%s\""), data_buffer.payload.ctr);

  pCONT->Tasker_Interface(FUNC_JSON_COMMAND_ID);

  ALOG_INF(PSTR("buffer_writer STTemplate_LoadART ------G- >>>>>>>>>> %d"),JBI->GetBufferSize());

  }

  #endif // USE_LIGHTING_TEMPLATE

  #ifdef ENABLE_DEBUGCRITICAL__STOPPING_CODE_AFTER_TEMPLATE_LOAD
  Serial.println("Template_Load PAUSE");
  delay(10000);
  #endif // ENABLE_DEBUGCRITICAL__STOPPING_CODE_AFTER_TEMPLATE_LOAD

}


void mInterfaceLight::Init(void)
{
  #ifdef ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32__WARNING_REQUIRES_FUTURE_LOCKING_OF_UPDATES_DURING_TASK_RUNNING
  Init_NeoPixelBus_PinnedTask(void)
  #endif  
  
  pCONT_set->Settings.pwm_range = PWM_RANGE;
  pCONT_set->Settings.light_settings.light_fade = 1;
  pCONT_set->Settings.light_settings.light_speed = 5*2;
  pCONT_set->runtime.power = 1;

  module_state.mode = ModuleStatus::Running;

}


void mInterfaceLight::ShowInterface()
{

  if(bus_manager){ bus_manager->show(); }
        
}




void mInterfaceLight::EveryLoop()
{


        
} // END everyloop

/**
 * @brief If this function is called, it is already assumed that the command required overriding filesystem settings (e.g. file "config_module.json" is missing, "reset" is requested)
 * 
 */
void mInterfaceLight::Template_Load_DefaultConfig()
{

  #ifdef USE_LIGHTING_TEMPLATE_DEFAULT
  
  if(!JBI->RequestLock(GetModuleUniqueID())){
    return;
  }

  // load from progmem into local
  D_DATA_BUFFER_CLEAR();
  memcpy_P(data_buffer.payload.ctr, LIGHTING_TEMPLATE_DEFAULT, sizeof(LIGHTING_TEMPLATE_DEFAULT));
  data_buffer.payload.len = strlen(data_buffer.payload.ctr);

  ALOG_DBM( PSTR("LIGHTING_TEMPLATE_DEFAULT" " READ = \"%s\""), data_buffer.payload.ctr);

  pCONT->Tasker_Interface(FUNC_JSON_COMMAND_ID);

  JBI->ReleaseLock();

  #endif // LIGHTING_TEMPLATE_DEFAULT

}


void mInterfaceLight::Save_Module()
{
  
  ALOG_INF(PSTR("mInterfaceLight::Save_Module()"));

  char buffer[100] = {0};

  /********************************************************************
   * Byte Data
   * ******************************************************************/

  char filename_byte[50];
  snprintf_P(filename_byte, sizeof(filename_byte), "/lgt_%S_byte.txt", GetModuleFriendlyName()); // debugging in .txt so webui/edt can read it
  // snprintf_P(filename_byte, sizeof(filename_byte), "/lgt_%S.bin", GetModuleFriendlyName()); // possibly switch to .bin for binary data, though UI wont be able to read it

  struct TEST{
    uint16_t test = 1234;
    char message[7] = "hello\0";
  }test;

  pCONT_mfile->ByteFile_Save(filename_byte, (uint8_t*)&test, sizeof(TEST));


  /********************************************************************
   * Json Data
   * ******************************************************************/

  if(!JBI->RequestLock(GetModuleUniqueID())){
    return;
  }
 
  JBI->Start();

  #ifdef ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES
    JBI->Add(PM_JSON_UTC_TIME, pCONT_time->GetDateAndTime(DT_UTC).c_str() );
  #endif // ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  
    JBI->Array_Start("BusConfig");
    for(uint8_t bus_i = 0; bus_i < bus_manager->getNumBusses(); bus_i++)
    {    
      JBI->Object_Start();

        uint8_t pins[5] = {0};
        uint8_t pin_count = bus_manager->busses[bus_i]->getPins(pins);
        JBI->Array_Start("Pin");
          for(uint8_t ii=0;ii<pin_count;ii++){ JBI->Add(pins[ii]); }
        JBI->Array_End();

        JBI->Add("Start", bus_manager->busses[bus_i]->getStart());
        JBI->Add("Length", bus_manager->busses[bus_i]->getLength());
        JBI->Add_P("BusType", bus_manager->busses[bus_i]->getTypeName());

        JBI->Add_P("ColourOrder", bus_manager->getColourOrderName(bus_manager->busses[bus_i]->getColorOrder(), buffer, sizeof(buffer)) );

      JBI->Object_End();
    }
    JBI->Array_End();

    JBI->Add("BrightnessRGB", getBriRGB_Global());
    JBI->Add("BrightnessCCT", getBriCCT_Global());



  //

  JBI->End();

  char filename_json[50];
  snprintf_P(filename_json, sizeof(filename_json), "/lgt_%S.json", GetModuleFriendlyName());

  pCONT_mfile->JSONFile_Save(filename_json, JBI->GetBuffer(), JBI->GetBufferLength());

  JBI->ReleaseLock();




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

    RgbColor colour = HsbColor(HUE_N2F(hue),1.0f,BRT_N2F(brt));

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

    RgbColor colour = HsbColor(HUE_N2F(hue),1.0f,1.0f);

    // colour = RgbColor((int)value, 2, 3);

    return colour;

}



#ifdef ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

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
  _pwm_max = mapvalue(_pwm_max, 1, 1023,  1, pCONT_set->Settings.pwm_range);  // _pwm_max cannot be zero

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
    for (uint32_t i = 0; i < 5; i++) {
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
  // if (channel >= pCONT_lAni->subtype) { return false; }     // Out of range
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


void mInterfaceLight::BusManager_Create_DefaultSingleNeoPixel()
{

  const uint16_t _STRIP_SIZE_MAX = 100;
  const uint8_t defDataPins[] = {4};
  const uint16_t defCounts[] = {_STRIP_SIZE_MAX};
  const uint8_t defNumBusses = ((sizeof defDataPins) / (sizeof defDataPins[0]));
  const uint8_t defNumCounts = ((sizeof defCounts)   / (sizeof defCounts[0]));

  DEBUG_PRINTF("defDataPins %d, defCounts %d, defNumBusses %d, defNumCounts %d \n\r", defDataPins[0], defCounts[0], defNumBusses, defNumCounts);

  uint16_t prevLen = 0;
  for (uint8_t i = 0; i < defNumBusses && i < WLED_MAX_BUSSES; i++) 
  {
    uint8_t defPin[] = {defDataPins[i]};
    uint16_t start = prevLen;
    uint16_t count = defCounts[(i < defNumCounts) ? i : defNumCounts -1];
    prevLen += count;
    BusConfig defCfg = BusConfig(DEFAULT_LED_TYPE, defPin, start, count);
    if(bus_manager->add(defCfg) == -1) 
    {
      ALOG_ERR(PSTR("bus_manager->add(defCfg) == -1"));
      break;
    }
  }

}


void mInterfaceLight::BusManager_Create_DefaultSinglePWM_5CH()
{

  const uint16_t _STRIP_SIZE_MAX = 100;
  const uint8_t defDataPins[] = {15,13,12,14,4};
  const uint16_t defCounts[] = {_STRIP_SIZE_MAX};
  const uint8_t defNumBusses = ((sizeof defDataPins) / (sizeof defDataPins[0]));
  const uint8_t defNumCounts = ((sizeof defCounts)   / (sizeof defCounts[0]));

  DEBUG_PRINTF("defDataPins %d, defCounts %d, defNumBusses %d, defNumCounts %d \n\r", defDataPins[0], defCounts[0], defNumBusses, defNumCounts);

  uint16_t prevLen = 0;
  for (uint8_t i = 0; i < defNumBusses && i < WLED_MAX_BUSSES; i++) 
  {
    uint8_t defPin[] = {defDataPins[i]};
    uint16_t start = prevLen;
    uint16_t count = defCounts[(i < defNumCounts) ? i : defNumCounts -1];
    prevLen += count;


    BusConfig defCfg = BusConfig(BUSTYPE_ANALOG_4CH, defPin, start, count);
    if(bus_manager->add(defCfg) == -1) 
    {
      ALOG_ERR(PSTR("bus_manager->add(defCfg) == -1"));
      break;
    }else{
      ALOG_DBG(PSTR("bus_manager->add(defCfg) "));
    }
  }

}



/**
 * @brief Testing that each object passed into here is a new busconfig
 * 
 * @param obj 
 */
void mInterfaceLight::parseJSONObject__BusConfig(JsonParserObject obj)
{
  JsonParserToken jtok = 0;
  JsonParserToken jtok2 = 0;

  DEBUG_LINE_HERE;

  // busConfigs
  uint8_t bus_count = 0;//bus_manager->getNumBusses();
  for(uint8_t ii=0;ii<(WLED_MAX_BUSSES+WLED_MIN_VIRTUAL_BUSSES);ii++)
  {
    if (busConfigs[ii] != nullptr)
    {
      bus_count++;
    }
  }

  ALOG_INF(PSTR("getNumBusses %d"), bus_count);

  uint16_t start = 0;
  uint16_t length = 10;
  int8_t bus_type = BUSTYPE_NONE;
  COLOUR_ORDER_T ColourOrder = {COLOUR_ORDER_INIT_DISABLED};
  uint8_t pins[5] = {255}; // 255 is unset
  

  if(jtok2 = obj["Pin"])
  {
    if(jtok2.isNum())
    {
      pins[0] = jtok2.getInt();
    }
    else
    if(jtok2.isArray())
    {
      uint8_t ii = 0;
      JsonParserArray arrobj = jtok2;
      for(auto value : arrobj) 
      {
        pins[ii++] = value.getInt();
      }
    }
    else
    {
      ALOG_INF(PSTR("Invalid pin type A"));
    }  
    ALOG_INF(PSTR("pins %d,%d,%d,%d,%d"), pins[0], pins[1], pins[2], pins[3], pins[4]);  
  }


  if(jtok = obj["Start"])
  {
    start = jtok.getInt();
    ALOG_INF(PSTR("start %d"), start);
  }
  // if(jtok = obj["S"])
  // {
  //   start = jtok.getInt();
  //   ALOG_INF(PSTR("start %d"), start);
  // }


  if(jtok = obj["Length"])
  {
    length = jtok.getInt();
    ALOG_INF(PSTR("length %d"), length);
  }
  // if(jtok = obj["L"])
  // {
  //   length = jtok.getInt();
  //   ALOG_INF(PSTR("length %d"), length);
  // }


  if(jtok = obj["BusType"])
  {
    if(jtok.isInt())
    {
      bus_type = jtok.getInt();
    }
    else
    if(jtok.isStr())
    {
      bus_type = Bus::getTypeIDbyName(jtok.getStr());
    }
    ALOG_INF(PSTR("bus_type %d"), bus_type);
  }
  

  if(jtok = obj[PM_JSON_RGB_COLOUR_ORDER])
  {
    if(jtok.isStr())
    {
      ColourOrder = GetColourOrder_FromName(jtok.getStr());
    }
  }
  // if(jtok = obj["CO"])
  // {
  //   if(jtok.isStr())
  //   {
  //     ColourOrder = GetColourOrder_FromName(jtok.getStr());
  //   }
  // }


  uint8_t bus_index = bus_count; // next bus space 
  if (busConfigs[bus_index] != nullptr) delete busConfigs[bus_index];


  ALOG_HGL(PSTR("============BusConfig(type%d,pin0=%d,start%d,len%d,CO%d)"),
    bus_type,
    pins[0],
    start,
    length,
    ColourOrder.data
  );


  DEBUG_LINE_HERE;
  busConfigs[bus_index] = new BusConfig(
    bus_type, 
    pins, 
    start, 
    length,
    ColourOrder,
    false, 
    0, 
    RGBW_MODE_MANUAL_ONLY
  );    
  
  DEBUG_LINE_HERE;
  pCONT_lAni->doInitBusses = true; // adds checks

  ALOG_DBG( PSTR("mInterfaceLight::parseJSONObject__BusConfig Finished"));
  DEBUG_LINE_HERE;

}


COLOUR_ORDER_T mInterfaceLight::GetColourOrder_FromName(const char* c)
{

  COLOUR_ORDER_T colour_order = {COLOUR_ORDER_INIT_DISABLED};

  if(!c){ return colour_order; }
  if(strlen(c)<=5){
    ALOG_DBM( PSTR("Valid Length"));
  }else{
    ALOG_INF(PSTR("INVALID Length"));
    return colour_order;
  }

  for(uint8_t index=0;index<strlen(c);index++)
  {
    if((c[index]=='R')||(c[index]=='r')){
      colour_order.red = index;
    }else
    if((c[index]=='G')||(c[index]=='g')){
      colour_order.green = index;
    }else
    if((c[index]=='B')||(c[index]=='b')){
      colour_order.blue = index;
    }else
    if((c[index]=='C')||(c[index]=='c')){
      colour_order.white_cold = index;
    }else
    if((c[index]=='W')||(c[index]=='w')){
      colour_order.white_warm = index;
    }
  }

  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  ALOG_INF( PSTR("colour_order == R=%d, G=%d, B=%d, CW=%d, WW=%d, dec%d, %X"),
    colour_order.red,
    colour_order.green,
    colour_order.blue,
    colour_order.white_cold,
    colour_order.white_warm,
    colour_order.data,
    colour_order.data
  );
  #endif  

  return colour_order;

}


/******************************************************************************************************************
 * mInterfaceLight_Commands.cpp
*******************************************************************************************************************/


void mInterfaceLight::parse_JSONCommand(JsonParserObject obj)
{

  ALOG_DBM( PSTR(D_LOG_LIGHT D_TOPIC "mInterfaceLight Checking all commands %d"),obj.isNull());

  char buffer[50];
  JsonParserToken jtok = 0; 
  JsonParserToken jtok2 = 0; 
  int8_t tmp_id = 0;

  // uint16_t isserviced_start_count = data_buffer.isserviced;  


  /**
   * @brief Bus needs to be configured first, and probably built immediately from within here so plattes etc next are added properly, otherwise EVERY_LOOP would have to happen
   * 
   */
  if(jtok = obj["BusConfig"])
  { 
    if(jtok.isArray())
    {      
      ALOG_INF(PSTR("BusConfig **************  \t%d"), jtok.getType());
      JsonParserArray arrobj = jtok;      
      for(auto value : arrobj) 
      {
        parseJSONObject__BusConfig(value.getObject());
      }
    }
  }else{
    ALOG_DBM(PSTR("BusConfig MISSING"));
    
    #ifdef ENABLE_DEBUGFEATURE__16PIN_PARALLEL_OUTPUT
    if(jtok = obj["Segment0"])
    {
      ALOG_INF(PSTR("Segment0 YES"));
    }else{
      ALOG_INF(PSTR("Segment0 MISSING"));
    }
    // delay(5000);
    #endif
  }


  /**
   * @brief First thing after parsing the BusConfig, Segments should be created before any further commands
   * 
   */
  pCONT_lAni->parse_JSONCommand(obj);
  


  /**
   * @brief Master (previously global) shall control the final output, but per segment within animator can exist. 
   * Ie the true colour out is 
   * 
   * SetBrightnessOutput <= Raw255Colour * MasterBrightness * SegmentBrightness
   * 
   */

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




  ALOG_DBM(PSTR("void mInterfaceLight::parse_JSONCommand(JsonParserObject obj)======================"));
  
}



void mInterfaceLight::CommandSet_Brt_255(uint8_t brt_new){
    
  // pCONT_lAni->SEGMENT_I(0).rgbcct_controller->setBrightness255(brt_new);

  if(!pCONT_lAni->segments.size()){ return; }

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

void mInterfaceLight::CommandSet_Global_BrtRGB_255(uint8_t bri, uint8_t segment_index)
{
  if(!pCONT_lAni->segments.size()){ return; }

  // SEGMENT_I(segment_index).rgbcct_controller->setBrightnessRGB255(bri);
 pCONT_lAni->SEGMENT_I(segment_index).flags.fForceUpdate = true;

  _briRGB_Global = bri;
  setBriRGB_Global(bri);
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS)), SEGMENT_I(segment_index).rgbcct_controller->getBrightnessRGB());
  #endif // ENABLE_LOG_LEVEL_COMMANDS
}

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** BrtCCT *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mInterfaceLight::CommandSet_Global_BrtCCT_255(uint8_t bri, uint8_t segment_index) 
{
  if(!pCONT_lAni->segments.size()){ return; }
  pCONT_lAni->SEGMENT_I(segment_index).flags.fForceUpdate = true; 
  setBriCT_Global(bri);
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_CCT)), SEGMENT_I(segment_index).rgbcct_controller->getBrightnessCCT255());
  #endif // ENABLE_LOG_LEVEL_COMMANDS
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

  JsonBuilderI->Object_Start("RgbcctController");
  
    // JsonBuilderI->Object_Start("raw");
    //   JsonBuilderI->Add("R", rgbcct_controller.R); 
    //   JsonBuilderI->Add("G", rgbcct_controller.G); 
    //   JsonBuilderI->Add("B", rgbcct_controller.B); 
    //   JsonBuilderI->Add("WW", rgbcct_controller.WW); 
    //   JsonBuilderI->Add("WC", rgbcct_controller.WC); 
    // JsonBuilderI->Object_End();
    JsonBuilderI->Object_Start("type");
    
    JsonBuilderI->Add("R", pCONT_lAni->SEGMENT_I(0).hardware_element_colour_order.r); 
    JsonBuilderI->Add("G", pCONT_lAni->SEGMENT_I(0).hardware_element_colour_order.g); 
    JsonBuilderI->Add("B", pCONT_lAni->SEGMENT_I(0).hardware_element_colour_order.b); 
    JsonBuilderI->Add("WW", pCONT_lAni->SEGMENT_I(0).hardware_element_colour_order.w); 
    JsonBuilderI->Add("WC", pCONT_lAni->SEGMENT_I(0).hardware_element_colour_order.c); 


    JsonBuilderI->Object_End();

    // JsonBuilderI->Add("mPaletteI->active_scene_palette_id",mPaletteI->active_scene_palette_id);

  JsonBuilderI->Object_End();


  if(pCONT_lAni->SEGMENT_I(0).palette_id == mPalette::PALETTELIST_VARIABLE_GENERIC_01__ID)
  {

    JBI->Array_AddArray("encoded", pCONT_set->Settings.animation_settings.palette_encoded_users_colour_map, ARRAY_SIZE(pCONT_set->Settings.animation_settings.palette_encoded_users_colour_map));

  }


  // JsonBuilderI->Object_Start("singlecolour");
  //   JsonBuilderI->Add_P(PM_R", mode_singlecolour.colour.R);
  //   JsonBuilderI->Add_P(PM_G", mode_singlecolour.colour.G);
  //   JsonBuilderI->Add_P(PM_B", mode_singlecolour.colour.B);
  //   JsonBuilderI->Add_P(PM_WW", mode_singlecolour.colour.WW);
  //   JsonBuilderI->Add_P(PM_WC", mode_singlecolour.colour.WC);
  //   JsonBuilderI->Add_P(PM_WC", mode_singlecolour.colour.WC);
  // JsonBuilderI->Object_End();
  // JsonBuilderI->Object_Start("active_rgbcct_colour_p");
  //   JsonBuilderI->Add_P(PM_R", active_rgbcct_colour_p->R);
  //   JsonBuilderI->Add_P(PM_G", active_rgbcct_colour_p->G);
  //   JsonBuilderI->Add_P(PM_B", active_rgbcct_colour_p->B);
  //   JsonBuilderI->Add_P(PM_WW", active_rgbcct_colour_p->WW);
  //   JsonBuilderI->Add_P(PM_WC", active_rgbcct_colour_p->WC);
  //   JsonBuilderI->Add_P(PM_WC", active_rgbcct_colour_p->WC);
  // JsonBuilderI->Object_End();

  // JsonBuilderI->Array_AddArray("singlecolour", current_color, 5);   

  // JsonBuilderI->Object_Start("fade");
  //   JsonBuilderI->Add("running", fade.running);
  // JsonBuilderI->Object_End();

  return JsonBuilderI->End();

}
#endif // ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE_DEBUG_CONFIG

#ifdef ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG
uint8_t mInterfaceLight::ConstructJSON_Debug__BusConfig(uint8_t json_level, bool json_appending)
{

  JBI->Start();

  for(uint8_t bus_i = 0; bus_i < bus_manager->getNumBusses(); bus_i++)
  {
    JBI->Object_Start_F("Bus%d", bus_i);

      JBI->Add("getLength", bus_manager->busses[bus_i]->getLength());
      JBI->Add("s", bus_manager->busses[bus_i]->_start);
      JBI->Add("l", bus_manager->busses[bus_i]->_len);

      COLOUR_ORDER_T colour_order = bus_manager->busses[bus_i]->getColorOrder();
      JBI->Array_Start("CO");
        JBI->Add(colour_order.red);
        JBI->Add(colour_order.green);
        // JBI->Add(colour_order.blue);
        // JBI->Add(colour_order.white_cold);
        // JBI->Add(colour_order.white_warm);
      JBI->Array_End();

      JBI->Add("getType", (uint8_t)bus_manager->busses[bus_i]->getType());

      JBI->Add("getTypeName", bus_manager->busses[bus_i]->getTypeName());

      

      uint8_t pins[5] = {0};
      uint8_t pin_count = 0;
      pin_count = bus_manager->busses[bus_i]->getPins(pins);
      JBI->Array_Start("p");
      // JBI->Add(pin_count);
      for(uint8_t ii=0;ii<pin_count;ii++)
      {
        JBI->Add(pins[ii]);
      }
      JBI->Array_End();

    JBI->Object_End();
  }

  return JBI->End();

}
#endif // ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG


/******************************************************************************************************************
 * mInterfaceLight_MQTT.cpp
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mInterfaceLight::MQTTHandler_Init()
{

  struct handler<mInterfaceLight>* ptr;  

  ptr = &mqtthandler__settings__teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mInterfaceLight::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler__state__ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__STATE__CTR;
  ptr->ConstructJSON_function = &mInterfaceLight::ConstructJSON_State;
  mqtthandler_list.push_back(ptr);
  
  #ifdef ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE_DEBUG_CONFIG
  ptr = &mqtthandler__debug_module_config__teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_MODULE_CONFIG__CTR;
  ptr->ConstructJSON_function = &mInterfaceLight::ConstructJSON_Debug_Module_Config;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE_DEBUG_CONFIG

  
  #ifdef ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG
  ptr = &mqtthandler__debug_bus_config__teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_BUS_CONFIG__CTR;
  ptr->ConstructJSON_function = &mInterfaceLight::ConstructJSON_Debug__BusConfig;
  mqtthandler_list.push_back(ptr);
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
void mInterfaceLight::MQTTHandler_Sender()
{    
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_LIGHTS_INTERFACE_ID, handle);
  }
}
  
#ifdef USE_MODULE_NETWORK_WEBSERVER
#ifdef ENABLE_FEATURE_MQTT__ADD_WEBURL_FOR_PAYLOAD_REQUESTS
void mInterfaceLight::MQTTHandler_AddWebURL_PayloadRequests()
{    
  CODE_BLOCK__MQTTHandler_AddWebURL_PayloadRequests();
}
#endif // ENABLE_FEATURE_MQTT__ADD_WEBURL_FOR_PAYLOAD_REQUESTS
#endif // USE_MODULE_NETWORK_WEBSERVER

#endif// USE_MODULE_NETWORK_MQTT


#endif // USE_DRIVER
