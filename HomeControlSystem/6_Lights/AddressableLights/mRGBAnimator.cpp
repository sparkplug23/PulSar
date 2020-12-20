#include "mRGBAnimator.h"

#ifdef USE_MODULE_LIGHTS_ADDRESSABLE

/*******************************************************************************************************************
********************************************************************************************************************
************TASKER**************************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/

int8_t mRGBAnimator::Tasker(uint8_t function){ 

  int8_t function_result = 0;

  #ifdef DEVICE_RGBDELL

  // pCONT_iLight->animation.mode_id = pCONT_iLight->ANIMATION_MODE_AMBILIGHT_ID;
  // ambilightsettings.ambilight_mode = AMBILIGHT_SIDES_ID;
  // pCONT_set->Settings.light_settings.type == LT_WS2812;

  #endif
  
  #ifdef ENABLE_DEVFEATURE_FLICKERING_TEST5
  // pCONT_iLight->_briRGB = 255;
  // flashersettings.function = FLASHER_FUNCTION_SLOW_GLOW_PARTIAL_PALETTE_STEP_THROUGH_ID;//FLASHER_FUNCTION_PULSE_RANDOM_ON
  // pCONT_iLight->animation.mode_id = pCONT_iLight->ANIMATION_MODE_FLASHER_ID;
  // pCONT_iLight->animation.transition.rate_ms.val = 400; // not sure if this will work? I probably have to force a check somewhere
  // pCONT_iLight->animation.transition.time_ms.val = 2;
  // pCONT_iLight->animation.transition.order_id = TRANSITION_ORDER_INORDER_ID;
  // pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val = 50;
  // pCONT_iLight->animation.palette_id = pCONT_iLight->PALETTELIST_VARIABLE_USER_01_ID;
  // pCONT_iLight->animation.palette_index_range_max_limit = 2;
  // mixer.running_id = 10;
  // mixer.flags.Enabled = false;


  // pCONT_iLight->_briRGB = 255;
  // flashersettings.function = FLASHER_FUNCTION_TESTER_ID;//FLASHER_FUNCTION_PULSE_RANDOM_ON
  // pCONT_iLight->animation.mode_id = pCONT_iLight->ANIMATION_MODE_FLASHER_ID;
  // pCONT_iLight->animation.transition.rate_ms.val = 400; // not sure if this will work? I probably have to force a check somewhere
  // pCONT_iLight->animation.transition.time_ms.val = 350;
  // pCONT_iLight->animation.transition.order_id = TRANSITION_ORDER_INORDER_ID;
  // pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val = 50;
  // pCONT_iLight->animation.palette_id = pCONT_iLight->PALETTELIST_VARIABLE_USER_01_ID;
  // pCONT_iLight->animation.palette_index_range_max_limit = 2;
  // mixer.running_id = 10;
  // mixer.flags.Enabled = false;
  #endif // ENABLE_DEVFEATURE_FLICKERING_TEST5

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case FUNC_PRE_INIT:
      pre_init();  
      break;
    case FUNC_INIT:
      init();
      break;
  }

  if(pCONT_set->Settings.light_settings.type != LT_WS2812){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }
  if(!settings.flags.EnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID;}
  
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

      // pCONT_wif->wifi_counter_tester++;
      // AddLog_P(LOG_LEVEL_INFO,PSTR("wifi_counter_tester=%d"),pCONT_wif->wifi_counter_tester);

//       Serial.println("HERE");
//       for(int i=0;i<45;i++){

// Serial.printf("%02d | %d\n\r",i,editable_mapped_array_data_array[i]);

//       }

// Serial.println();
      


  //     SetPixelColor(0,RgbTypeColor(255,0,0));
  //     SetPixelColor(1,RgbTypeColor(0,255,0));
  //     SetPixelColor(2,RgbTypeColor(0,0,255));
  //     SetPixelColor(3,RgbTypeColor(255,0,255));
  //     SetPixelColor(4,RgbTypeColor(255,255,0));

  // stripbus->Show();

  // #ifdef USE_PM_OUTSIDE_TREE_MIXER_DESCRIPTION
  //   char result[100];
  //   pCONT_sup->GetTextIndexed_P(result, sizeof(result), mixer.running_id, PM_OUTSIDE_TREE_MIXER_DESCRIPTION);  // should this be _P?
  //   AddLog_P(LOG_LEVEL_INFO, PSTR("Mixer routine \"%s\""), result);          
  // #endif // USE_PM_OUTSIDE_TREE_MIXER_DESCRIPTION





    }break;
    case FUNC_EVERY_MINUTE:

    // pCONT_iLight->animation.flags.use_gamma_for_brightness ^= 1;

    break;
    case FUNC_LOOP: 
      EveryLoop();
    break;    
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_CHECK_TOPIC_ID:
      CheckAndExecute_JSONCommands();
    break;
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand();
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




int8_t mRGBAnimator::CheckAndExecute_JSONCommands(){

  // Check if instruction is for me
  if(mSupport::SetTopicMatch(data_buffer.topic.ctr,D_MODULE_LIGHTS_ADDRESSABLE_FRIENDLY_CTR)>=0){
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_PIXELS));
    #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
    pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
    parse_JSONCommand();
    return FUNCTION_RESULT_HANDLED_ID;
  }else{
    return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
  }

}


void mRGBAnimator::parse_JSONCommand(void){

  // Need to parse on a copy
  char parsing_buffer[data_buffer.payload.len+1];
  memcpy(parsing_buffer,data_buffer.payload.ctr,sizeof(char)*data_buffer.payload.len+1);
  JsonParser parser(parsing_buffer);
  JsonParserObject obj = parser.getRootObject();   
  if (!obj) { 
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_ERROR, PSTR("DeserializationError with \"%s\""),parsing_buffer);
    #endif// ENABLE_LOG_LEVEL_INFO
    return;
  }  
  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  char buffer[50];

  if(jtok = obj[PM_JSON_ANIMATIONENABLE]){ 
    const char* onoff = jtok.getStr();
    uint8_t state = pCONT_sup->GetStateNumber(onoff);
    pCONT_iLight->animation.flags.fEnable_Animation = ConvertStateNumberIfToggled(state, pCONT_iLight->animation.flags.fEnable_Animation);
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_ANIMATIONENABLE)), pCONT_iLight->animation.flags.fEnable_Animation);    
    #endif // ENABLE_LOG_LEVEL_COMMANDS
  }

#ifdef ENABLE_PIXEL_FUNCTION_FLASHER
  /**
   *  Flasher function specific commands
   * */
  if(jtok = obj[PM_JSON_FLASHER].getObject()[PM_JSON_FUNCTION]){
    if(jtok.isStr()){
      if((tmp_id=GetFlasherFunctionIDbyName(jtok.getStr()))>=0){
        flashersettings.function = tmp_id;      //make function "changeFlasherFunction" so then the region is automatically updated internally
        flashersettings.region = FLASHER_REGION_COLOUR_SELECT_ID;
        data_buffer.isserviced++;
      }
    }else
    if(jtok.isNum()){
      flashersettings.function = jtok.getInt();
      flashersettings.region = FLASHER_REGION_COLOUR_SELECT_ID; 
      data_buffer.isserviced++;
    }
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_K(D_JSON_FUNCTION)), GetFlasherFunctionName(buffer));
    #endif // ENABLE_LOG_LEVEL_COMMANDS
  }

  if(jtok = obj[PM_JSON_FLASHER].getObject()[PM_JSON_COLOUR_REFRESH_RATE]){ 
    flashersettings.update_colour_region.refresh_secs = jtok.getInt();
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_FLASHER D_JSON_COLOUR_REFRESH_RATE)), flashersettings.update_colour_region.refresh_secs);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
  }

  if(jtok = obj[PM_JSON_FLASHER].getObject()[PM_JSON_DIRECTION]){ 
    flashersettings.flags.movement_direction = jtok.getInt();
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_DIRECTION)), flashersettings.flags.movement_direction);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
  }
  
  if(jtok = obj[PM_JSON_FLASHER].getObject()[PM_JSON_BRIGHTNESS_MIN]){ 
    flashersettings.brightness_min = jtok.getInt();
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_MIN)), flashersettings.brightness_min);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
  }

  if(jtok = obj[PM_JSON_FLASHER].getObject()[PM_JSON_BRIGHTNESS_MAX]){ 
    flashersettings.brightness_max = jtok.getInt();
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_MAX)), flashersettings.brightness_max);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
  }

  if(jtok = obj[PM_JSON_FLASHER].getObject()[PM_JSON_ALTERNATE_BRIGHTNESS_MIN]){ 
    shared_flasher_parameters.alternate_brightness_min = jtok.getInt();
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_ALTERNATE_BRIGHTNESS_MIN)), shared_flasher_parameters.alternate_brightness_min);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
  }

  if(jtok = obj[PM_JSON_FLASHER].getObject()[PM_JSON_ALTERNATE_BRIGHTNESS_MAX]){ 
    shared_flasher_parameters.alternate_brightness_max = jtok.getInt();
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_ALTERNATE_BRIGHTNESS_MAX)), shared_flasher_parameters.alternate_brightness_max);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
  }

  if(jtok = obj[PM_JSON_FLASHER].getObject()[PM_JSON_ALTERNATE_AMOUNT]){ 
    shared_flasher_parameters.alternate_random_amount_as_percentage = jtok.getInt();
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_ALTERNATE D_JSON_RANDOM_AMOUNT)), shared_flasher_parameters.alternate_random_amount_as_percentage);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
  }

  if(jtok = obj[PM_JSON_FLASHER].getObject()[PM_JSON_AGED_COLOURING]){
    pCONT_iLight->animation.flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique = jtok.getInt();
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_AGED_COLOURING)), pCONT_iLight->animation.flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
  }

#endif // ENABLE_PIXEL_FUNCTION_FLASHER

  /**
   * Pixel output grouping settings
   * */
  if(jtok = obj[PM_JSON_PIXELGROUPING].getObject()[PM_JSON_AGED_COLOURING]){
    uint8_t state = pCONT_sup->GetStateNumber(jtok.getStr());
    pixel_group.flags.fEnabled = ConvertStateNumberIfToggled(state, pixel_group.flags.fEnabled);
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_PIXELGROUPING D_JSON_ENABLED)), pixel_group.flags.fEnabled);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
  }

  if(jtok = obj[PM_JSON_PIXELGROUPING].getObject()[PM_JSON_MODE]){
    pixel_group.flags.multiplier_mode_id = jtok.getInt();
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_PIXELGROUPING D_JSON_MODE)), pixel_group.flags.multiplier_mode_id);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
  }

  if(jtok = obj[PM_JSON_PIXELGROUPING].getObject()[PM_JSON_MULTIPLIER]){ 
    pixel_group.multiplier = jtok.getInt();
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_PIXELGROUPING D_JSON_MULTIPLIER)), pixel_group.multiplier);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
  }

  if(jtok = obj[PM_JSON_PIXELGROUPING].getObject()[PM_JSON_MAPPED_MULTIPLIER_DATA]){ 
    if(jtok.isArray()){
      uint8_t index = 0;
      JsonParserArray array = obj[PM_JSON_PIXELGROUPING].getObject()[PM_JSON_MAPPED_MULTIPLIER_DATA];
      for(auto v : array) {
        int val = v.getInt();
        if(index > D_MAPPED_ARRAY_DATA_MAXIMUM_LENGTH){ break; }
        editable_mapped_array_data_array[index++] = val;
    #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_MAPPED_MULTIPLIER_DATA " [i%d:v%d]"),index-1,val);
    #endif// ENABLE_LOG_LEVEL_INFO          
      }
      pixel_group.mapped_array_data.length = index;
      data_buffer.isserviced++;
    }
  }


  // if(jtok = obj[PM_JSON_PIXELSGROUPED]){
  //   animation.pixelgrouped = obj[CFLASH(PM_JSON_PIXELSGROUPED];
  //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE),D_JSON_PIXELSGROUPED,animation.pixelgrouped);
  //   #endif
  // }

  //override commands that run for one animation cycle then are cleared to 0
  // if(!obj[CFLASH(PM_JSON_REFRESH_ALLPIXELS].isNull()){
  //   animation_override.fRefreshAllPixels = obj[CFLASH(PM_JSON_REFRESH_ALLPIXELS];
  //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE),D_JSON_REFRESH_ALLPIXELS,animation_override.fRefreshAllPixels);
  //   #endif
  // }
  
#ifdef ENABLE_PIXEL_FUNCTION_MIXER
  if(jtok = obj[PM_JSON_MIXER].getObject()[PM_JSON_ENABLED]){ 
    mixer.flags.Enabled = jtok.getInt();
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_MIXER D_JSON_ENABLED)), mixer.flags.Enabled);
    #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  }
   
  // if(jtok = obj[PM_JSON_MIXER].getObject()[PM_JSON_TIME_SCALER_AS_PERCENTAGE]){ 
  //   mixer.time_scaler = jtok.getInt();
  //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_TIME_SCALER_AS_PERCENTAGE)), mixer.time_scaler);
  //   #endif //#ifdef ENABLE_LOG_LEVEL_COMMANDS
  // }

  if(jtok = obj[PM_JSON_MIXER].getObject()[PM_JSON_RUNTIME_DURATION_SCALER_PERCENTAGE]){ 
    mixer.run_time_duration_scaler_as_percentage = jtok.getInt();
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_MIXER D_JSON_RUNTIME_DURATION_SCALER_PERCENTAGE)), mixer.run_time_duration_scaler_as_percentage);
    #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  }

  if(jtok = obj[PM_JSON_MIXER].getObject()[PM_JSON_RUNNING_ID]){  
    uint8_t val = jtok.getInt();
    mixer.running_id = val;
    LoadMixerGroupByID(val);
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_RUNNING_ID)), val);
    #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  }
#endif //ENABLE_PIXEL_FUNCTION_MIXER


#ifdef ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL


  if(jtok = obj["Strip"].getObject()["ClearTo"]){ 


    if(jtok.isArray()){


      RgbcctColor colour = RgbcctColor(0);

      uint8_t index = 0;
      JsonParserArray array = obj["Strip"].getObject()["ClearTo"];
      for(auto v : array) {
        switch(index){
          case 0: colour.R = v.getInt();
          case 1: colour.G = v.getInt();
          case 2: colour.B = v.getInt();
          case 3: colour.WC = v.getInt();
          case 4: colour.WW = v.getInt();
        }
        




        int val = v.getInt();
        // if(index > D_MAPPED_ARRAY_DATA_MAXIMUM_LENGTH){ break; }
        // editable_mapped_array_data_array[index++] = val;
        index++;

        // pCONT_iLight->animation.mode_id =  pCONT_iLight->ANIMATION_MODE_MANUAL_SETPIXEL_ID;

        // SetPixelColor(val, pCONT_iLight->mode_singlecolour.colour);

        // animation_colours[val].DesiredColour = pCONT_iLight->mode_singlecolour.colour;

    #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_MAPPED_MULTIPLIER_DATA " [i%d:v%d]"),index-1,val);
    #endif// ENABLE_LOG_LEVEL_INFO          
      }
    




      SetPixelColor_All(colour);
    }

  }

  if(jtok = obj[PM_JSON_MANUAL_SETPIXEL_TO_SCENE]){ 
    if(jtok.isArray()){


//testing, froce it
pCONT_iLight->mode_singlecolour.colour = RgbcctColor(255,255,255);


      uint8_t index = 0;
      JsonParserArray array = obj[PM_JSON_MANUAL_SETPIXEL_TO_SCENE];
      for(auto v : array) {
        int val = v.getInt();
        // if(index > D_MAPPED_ARRAY_DATA_MAXIMUM_LENGTH){ break; }
        // editable_mapped_array_data_array[index++] = val;
        index++;

        //Count active pixels
        uint16_t pixels_on = 0;
        for(uint16_t i=0;i<pCONT_iLight->light_size_count;i++){ 
          if(pCONT_iLight->RgbColorto32bit(GetPixelColor(i))){
            pixels_on++;
          }      
        }

        if(pixels_on <= 5){
          pCONT_iLight->mode_singlecolour.colour = RgbcctColor(255,0,0);
        }else
        if(pixels_on <= 10){
          pCONT_iLight->mode_singlecolour.colour = RgbcctColor(0,255,0);
        }else
        if(pixels_on <= 15){
          pCONT_iLight->mode_singlecolour.colour = RgbcctColor(0,0,255);
        }else
        if(pixels_on <= 20){
          pCONT_iLight->mode_singlecolour.colour = RgbcctColor(255,0,255);
        }else{
          pCONT_iLight->mode_singlecolour.colour = RgbcctColor(255,255,255);
        }

        pCONT_iLight->animation.mode_id =  pCONT_iLight->ANIMATION_MODE_MANUAL_SETPIXEL_ID;

        SetPixelColor(val, pCONT_iLight->mode_singlecolour.colour);

        animation_colours[val].DesiredColour = pCONT_iLight->mode_singlecolour.colour;

    #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_MAPPED_MULTIPLIER_DATA " [i%d:v%d]"),index-1,val);
    #endif// ENABLE_LOG_LEVEL_INFO          
      }
      // pixel_group.mapped_array_data.length = index;

      stripbus->Show();

      data_buffer.isserviced++;
    }
  }



#endif // ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL

// CHANGE TO USE RELAYS INSTEAD
  // if(!obj[CFLASH(PM_JSON_EXTERNAL_POWER_ONOFF)].isNull()){ 
  //   const char* onoff = obj[CFLASH(PM_JSON_EXTERNAL_POWER_ONOFF)];    
  //   uint8_t relay_state_new = pCONT_sup->GetStateNumber(onoff);
  //   uint8_t relay_state_current = 0;
  //   if(relay_state_new==STATE_NUMBER_TOGGLE_ID){
  //     // pCONT_iLight->animation.flags.fEnable_Animation ^= 1;
  //   }else{
  //     // pCONT_iLight->animation.flags.fEnable_Animation = state;
  //   }
  //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_EXTERNAL_POWER_ONOFF,relay_state_new);
  //   #endif
  //   data_buffer.isserviced++;
  // }


  // if(!obj[CFLASH(PM_JSON_ONOFF].isNull()){ 
  //   const char* onoff = obj[CFLASH(PM_JSON_ONOFF];
  //   if(strstr(onoff,"ON")){ 
      
  //     #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED "\"onoff\"=\"ON\""));
  //     #endif
      
  //     #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //     AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "MODE_TURN_ON_ID"));
  //     #endif
  //     // Add this as "SAVE" state then "LOAD" state
  //     memcpy(&pCONT_iLight->animation,&pCONT_iLight->animation_stored,sizeof(pCONT_iLight->animation));// RESTORE copy of state

  //     pCONT_iLight->SetAnimationProfile(pCONT_iLight->ANIMATION_PROFILE_TURN_ON_ID);
  //     pCONT_iLight->light_power_state = true;

  //     //pCONT_iLight->animation.mode_id = MODE_TURN_ON_ID;
  //     data_buffer.isserviced++;
  //   }else if(strstr(onoff,"OFF")){
  //     #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED "\"onoff\"=\"OFF\""));
  //     #endif
  //     #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //     AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "MODE_TURN_OFF_ID"));
  //     #endif
  //     memcpy(&pCONT_iLight->animation_stored,&pCONT_iLight->animation,sizeof(pCONT_iLight->animation)); // STORE copy of state
  //     pCONT_iLight->SetAnimationProfile(pCONT_iLight->ANIMATION_PROFILE_TURN_OFF_ID);
  //     pCONT_iLight->light_power_state = false;

  //     //pCONT_iLight->animation.mode_id = MODE_TURN_OFF_ID;
  //     data_buffer.isserviced++;
  //   }else{
  //     #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //     AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_NOMATCH));
  //     #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   }
  // }

  // #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
  // parsesub_NotificationPanel(obj);
  // #endif // #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS


  //pCONT_iLight->animation.flags.fForceUpdate = true;
  
  if(data_buffer.isserviced){ //update string, move to shared place
   
   //add flag on this, so its not always forced
   // SetRefreshLEDs(); // implement in 1 second 
  }


// #endif // USE_DEVFEATURE_DISABLE_PROGMEM_TEST
  // t_mqtthandler_status_animation.flags.SendNow = true; 

}


/*******************************************************************************************************************
********************************************************************************************************************
************ START OF GENERAL INIT PROCESS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/

void mRGBAnimator::pre_init(void){ 
  
  // Allow runtime changes of animation size
  pCONT_iLight->light_size_count = STRIP_SIZE_MAX; 
  animator_strip_size = ANIMATOR_SIZE_MAX<=STRIP_SIZE_MAX?ANIMATOR_SIZE_MAX:STRIP_SIZE_MAX; 
  pCONT_set->Settings.flag_animations.clear_on_reboot = false; //flag

}

void mRGBAnimator::init(void){ 
    
  // if pixel size changes, free and init again
  uint16_t strip_size_tmp = STRIP_REPEAT_OUTPUT_MAX;//pCONT_iLight->light_size_count<STRIP_SIZE_MAX?pCONT_iLight->light_size_count:STRIP_SIZE_MAX; // Catch values exceeding limit
  ledout.length = STRIP_SIZE_MAX; 
  
  pCONT_iLight->light_size_count = STRIP_SIZE_MAX;

  DEBUG_LINE;
  // #ifdef USE_WS28XX_HARDWARE_WS2801
  //   stripbus = new NeoPixelBus<DotStarBgrFeature, DotStarMethod>(strip_size_tmp, pin_clock, pCONT_pins->GetPin(GPIO_RGB_DATA_ID));
  // #else
  //   #ifdef ENABLE_DEBUG_ESP_DECODER
  //     stripbus = new NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType>(strip_size_tmp, 5);
  //   #else

  //     #ifdef ESP8266
  //       stripbus = new NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType>(strip_size_tmp, pCONT_pins->GetPin(GPIO_RGB_DATA_ID));
  //     #else //esp32 testing
        stripbus = new NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType>(strip_size_tmp, 23);//19); 3 = rx0
  //     #endif //esp8266
  //   #endif
  // #endif //USE_WS28XX_HARDWARE_WS2801

  DEBUG_LINE;

  uint16_t animator_strip_size_tmp = 1;//animator_strip_size<ANIMATOR_SIZE_MAX?animator_strip_size:ANIMATOR_SIZE_MAX; // Catch values exceeding limit

  pCONT_iLight->Init_NeoPixelAnimator(animator_strip_size_tmp, NEO_ANIMATION_TIMEBASE);  

  pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val = 100;    

  pCONT_iLight->animation.flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique = false;


  pixel_group.flags.fEnabled = false;
  pixel_group.flags.multiplier_mode_id = PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_UNIFORM_ID;
  pixel_group.multiplier = 2;
  pixel_group.mapped_array_data.values = editable_mapped_array_data_array;
  pixel_group.flags.mapped_array_editable_or_progmem = 0;
  pixel_group.mapped_array_data.length = 0;
  // flashersettings.flags.enable_startcolour_as_alternate = true;
  // flashersettings.flags.enable_endingcolour_as_alternate = true;
  
#ifdef ESP8266
  randomSeed(analogRead(0));
#else
  randomSeed(analogRead(34)); //esp32
#endif
  
  for(ledout.index=0;
    ledout.index<STRIP_SIZE_MAX;
    ledout.index++){ 
    ledout.pattern[ledout.index] = ledout.index; 
  }

  DEBUG_LINE;


  // Clear stored light output
  memset(&animation_colours,0,sizeof(animation_colours));
  // Start display
  stripbus->Begin();
  if(pCONT_set->Settings.flag_animations.clear_on_reboot){
    stripbus->ClearTo(0);
    stripbus->Show();
  }

  pCONT_iLight->animation_override.fRefreshAllPixels = true;

  GenerateAnimationPixelAmountMaps();


  #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
    init_NotificationPanel();
  #endif
  
  #ifdef ENABLE_PIXEL_FUNCTION_AMBILIGHT
  blocking_force_animate_to_complete = true; //animate to completion on boot (for short animations)
  init_Ambilight();
  #endif // ENABLE_PIXEL_FUNCTION_AMBILIGHT
  #ifdef ENABLE_PIXEL_FUNCTION_FLASHER
  init_flasher_settings();
  #endif
  #ifdef ENABLE_PIXEL_FUNCTION_MIXER
  init_mixer_defaults();
  #endif

  //create copy for pCONT_iLight->animation stored
  memcpy(&pCONT_iLight->animation_stored,&pCONT_iLight->animation,sizeof(pCONT_iLight->animation_stored));// RESTORE copy of state

  settings.flags.EnableModule = true;

  // DrawTailPixels();

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

DEBUG_LINE;
  
  init(); //default values

DEBUG_LINE;
  #ifdef ENABLE_LOG_LEVEL_INFO
  // AddLog_P(LOG_LEVEL_TEST,PSTR("mRGBAnimator::Settings_Default"));
  #endif

  #ifdef USE_LIGHTING_TEMPLATE
    settings.flags.TemplateProvidedInProgMem = true;
  #endif
  
  if(settings.flags.TemplateProvidedInProgMem){// use template provided in progmem || override template
  
    //pCONT_iLight->Template_Load();

  }else{
    
    pCONT_iLight->animation.transition.method_id = TRANSITION_METHOD_BLEND_ID;


    // Check for user defined defaults
    #ifdef DEFAULT_LIGHTING_TRANSITION_ORDER_ID
      pCONT_iLight->animation.transition.order_id = 6;/*DEFAULT_LIGHTING_TRANSITION_ORDER_ID<TRANSITION_ORDER_LENGTH_ID?
                                            DEFAULT_LIGHTING_TRANSITION_ORDER_ID:TRANSITION_ORDER_INORDER_ID;*/
    #else
      pCONT_iLight->animation.transition.order_id = TRANSITION_ORDER_INORDER_ID;
    #endif
    
    #ifdef DEFAULT_LIGHTING_TRANSITION_TIME_MAP_SECS_ID
      pCONT_iLight->animation.transition.time_ms.map_id = 10;/*DEFAULT_LIGHTING_TRANSITION_TIME_MAP_SECS_ID<TIME_MAP_SECS_LENGTH_ID?
                                                DEFAULT_LIGHTING_TRANSITION_TIME_MAP_SECS_ID:TIME_MAP_SECS_15_ID;*/
      pCONT_iLight->animation.transition.time_ms.val = 2000;//PROGMEM rate_map_secs[pCONT_iLight->animation.transition.time_ms.map_id]*1000;
      pCONT_iLight->animation.flags.ftime_use_map = true;
    #else
      pCONT_iLight->animation.transition.time_ms.map_id = TIME_MAP_SECS_15_ID;
      pCONT_iLight->animation.transition.time_ms.val = PROGMEM rate_map_secs[pCONT_iLight->animation.transition.time_ms.map_id]*1000;
      pCONT_iLight->animation.flags.ftime_use_map = true;
    #endif

    #ifdef DEFAULT_LIGHTING_TRANSITION_RATE_MAP_SECS_ID
      pCONT_iLight->animation.transition.rate_ms.map_id = 6;/*DEFAULT_LIGHTING_TRANSITION_RATE_MAP_SECS_ID<RATE_MAP_SECS_LENGTH_ID?
                                                DEFAULT_LIGHTING_TRANSITION_RATE_MAP_SECS_ID:RATE_MAP_SECS_15_ID;*/
      pCONT_iLight->animation.transition.rate_ms.val = 10000;//PROGMEM rate_map_secs[pCONT_iLight->animation.transition.rate_ms.map_id]*1000;
      pCONT_iLight->animation.flags.frate_use_map = true;
    #else
      pCONT_iLight->animation.transition.rate_ms.map_id = RATE_MAP_SECS_15_ID;
      pCONT_iLight->animation.transition.rate_ms.val = PROGMEM rate_map_secs[pCONT_iLight->animation.transition.rate_ms.map_id]*1000;
      pCONT_iLight->animation.flags.frate_use_map = true;
    #endif

    #ifdef DEFAULT_LIGHTING_PIXELS_UPDATE_PERCENTAGE_ID
      pCONT_iLight->animation.transition.pixels_to_update_as_percentage.map_id = 4;/*DEFAULT_LIGHTING_PIXELS_UPDATE_PERCENTAGE_ID<PIXELS_UPDATE_PERCENTAGE_LENGTH_ID?
                            DEFAULT_LIGHTING_PIXELS_UPDATE_PERCENTAGE_ID:PIXELS_UPDATE_PERCENTAGE_20_ID;*/
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
    
    
      #ifdef ENABLE_PIXEL_FUNCTION_FLASHER
    #ifdef DEFAULT_LIGHTING_FLASHER_FUNCTION_ID
      flashersettings.function = 1;/*DEFAULT_LIGHTING_FLASHER_FUNCTION_ID<FLASHER_FUNCTION_LENGTH_ID?
                            DEFAULT_LIGHTING_FLASHER_FUNCTION_ID:FLASHER_FUNCTION_SLOW_GLOW_ID;*/
    #else
      flashersettings.function = FLASHER_FUNCTION_SLOW_GLOW_ID;
    #endif
    #endif // ENABLE_PIXEL_FUNCTION_FLASHER
  }


  uint16_t buffer_length = 0;
  char* buffer = pCONT_set->Settings.animation_settings.palette_user_variable_name_list_ctr;
  for(uint8_t i=0;i<20;i++){
   buffer_length+=sprintf(buffer+buffer_length,"%s%02d|\0","User",i);
  }
  #ifndef DISABLE_SERIAL_LOGGING
  //  Serial.println(buffer); log debuf more
  #endif

  
  memset(&editable_mapped_array_data_array,0,sizeof(editable_mapped_array_data_array));

    // uint8_t mapped_array_data_array_tree_test[] = {
    //   // 20,19, //39
    //   // 15,16, //70
    //   // 13,9, //92
    //   // 7,1  //100
    //   // 22,14,14,15,15,15,12,20,10,8,4,1
    //   #ifdef DEVICE_RGBOUTSIDETREE
    //   50,25,50,25
    //   #else
    //   2,3,4,5
    //   #endif

    // };

    
  // uint16_t single_row_count_array[] = {31,44,39,42,43,43,43,42,42,42,41,39,38,36,33,30,29,28,25,25,23,22,22,20,18,19,17,18,16,14,14,13,10,10,11,11,8,4,4,5,6,6,5,16,3};
  
  
  uint16_t single_row_count_array[] = {
    31,35,39,42,43,//31,44,39,42,43,
    43,43,42,42,42,
    41,39,38,36,33,
    30,29,28,25,25,
    23,22,22,20,18,
    19,17,18,16,14,
    14,13,10,10,11,
    11,8,4,4,5,6,6,
    5,5,5,5,5,
    
    
    
    0,3
  }; // 0 is filled last
  uint8_t single_row_count_array_size = 49;//sizeof(single_row_count_array)/single_row_count_array[0];

  // Serial.println(single_row_count_array_size);
  // delay(5000);
  
  uint16_t count = 0;
  for(int i=0;i<single_row_count_array_size;i++){
    count += single_row_count_array[i];
  }
  single_row_count_array[single_row_count_array_size-2] = 1050-count; // use 2nd last one as filler
  

  memcpy(editable_mapped_array_data_array, single_row_count_array, sizeof(single_row_count_array));


  pixel_group.mapped_array_data.length = ArrayCountUntilNull(editable_mapped_array_data_array, (uint8_t)D_MAPPED_ARRAY_DATA_MAXIMUM_LENGTH); //values before 0

DEBUG_LINE;


}


void mRGBAnimator::GenerateAnimationPixelAmountMaps(){

  // for(int val=0;val<sizeof(pixels_to_update_as_number_map);val++){
  //   pixels_to_update_as_number_map[val] = ((float)PROGMEM pixels_to_update_as_percentage_map[val]/100.0f)*pCONT_iLight->light_size_count;
  //   if(pixels_to_update_as_number_map[val]==0) pixels_to_update_as_number_map[val] = 1;
  // }

}



/*******************************************************************************************************************
********************************************************************************************************************
************ END OF GENERAL INIT ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/



#ifdef ENABLE_PIXEL_FUNCTION_AMBILIGHT

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

  
  ambilightsettings.screens[SCREEN_CENTRE].top.colour    = HsbColor(pCONT_iLight->HueN2F(20),pCONT_iLight->SatN2F(95),pCONT_iLight->BrtN2F(0));
  ambilightsettings.screens[SCREEN_CENTRE].bottom.colour    = HsbColor(pCONT_iLight->HueN2F(20),pCONT_iLight->SatN2F(95),pCONT_iLight->BrtN2F(50));

}

void mRGBAnimator::SubTask_Ambilight_Main(){

  // Add mode to allow orientations, for when screens rotate so they respect top/bottom

  // if(abs(millis()-ambilightsettings.tSavedUpdate)>ambilightsettings.ratemsSavedUpdate){ambilightsettings.tSavedUpdate=millis();
  //   AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "ambilight"));
  // }

  
  // ambilightsettings.screens[SCREEN_CENTRE].top.colour    = RgbColor(255,0,0);
  // ambilightsettings.screens[SCREEN_CENTRE].bottom.colour = RgbColor(0,255,0); 
  // ambilightsettings.screens[SCREEN_CENTRE].left.colour   = RgbColor(0,0,255);
  // ambilightsettings.screens[SCREEN_CENTRE].right.colour  = RgbColor(255,0,255);


  //switch modes : USE presets
  switch(ambilightsettings.ambilight_mode){
    case AMBILIGHT_PRESETS_ID:

    break;
    case AMBILIGHT_SIDES_ID: // IE DELL of dual tone from the past
      // not even splits, setting split point (ie bottom edge only 0%, 25% way up sides, half way 50%)
      if(abs(millis()-ambilightsettings.tSavedUpdate)>ambilightsettings.ratemsSavedUpdate){ambilightsettings.tSavedUpdate=millis();
        AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "ambilight"));
        Ambilight_Sides();
        StartAnimationAsBlendFromStartingColorToDesiredColor();
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
    animation_colours[bottom_start+bottom].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].bottom.colour;
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
    animation_colours[left_start+left].DesiredColour = colour_tmp;//ambilightsettings.screens[SCREEN_CENTRE].left.colour;
  }

  uint8_t top_size = ambilightsettings.screens[SCREEN_CENTRE].top.size;
  uint8_t top_start = 52;
  for(int top=0;top<top_size;top++){
    animation_colours[top_start+top].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].top.colour;
  }

  uint8_t right_size = ambilightsettings.screens[SCREEN_CENTRE].right.size;
  uint8_t right_start = 85;
  for(int right=0;right<right_size;right++){
    progress = (float)(right)/(float)(right_size);
    colour_tmp = RgbColor::LinearBlend(RgbColor(ambilightsettings.screens[SCREEN_CENTRE].top.colour),
                                       RgbColor(ambilightsettings.screens[SCREEN_CENTRE].bottom.colour),
                                       progress);
    animation_colours[right_start+right].DesiredColour = colour_tmp;
  }

}


// void mRGBAnimator::parsesub_ModeAmbilight(){

//    // Create local dereferenced variable
//   // JsonObject obj = (*_obj); 

//   int8_t tmp_id = 0;
  

//   char buffer[50];

// // // #ifndef ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT

// //   if(!obj[D_JSON_NAME].isNull()){ 
// //     const char* scenectr = obj[D_JSON_NAME];
// //     if((tmp_id=GetSceneIDbyName(scenectr))>=0){
// //       scene.name_id = tmp_id;
// //       pCONT_iLight->animation.mode_id = ANIMATION_MODE_SCENE_ID;
// //       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_NAME,GetSceneName(buffer));
// //       // Response_mP(S_JSON_COMMAND_SVALUE,D_JSON_NAME,GetSceneName(buffer));
// //       data_buffer.isserviced++;
// //     }else{
// //       AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_NAME,scenectr);
// //     }
// //   }

// //   // #endif //ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT

// //   // USe pointers
// //   //side
// //   // struct AMBILIGHT_SCREEN_SETTINGS.SCREENS[0]::
// //   // ambilightsettings.screens[SCREEN_CENTRE].top
// //   //screen
  
// //   if(!obj[F("top")][F(D_JSON_HUE)].isNull()){ 
// //     uint16_t hue = obj[F("top")][F(D_JSON_HUE)];
// //     // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_HUE,hue);
// //     ambilightsettings.screens[SCREEN_CENTRE].top.colour.H = pCONT_iLight->HueN2F(hue);
// //     // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_HUE,ambilightsettings.screens[SCREEN_CENTRE].top.colour.H);
// //     // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_HUE,ambilightsettings.screens[SCREEN_CENTRE].top.colour.H);
// //     data_buffer.isserviced++;
// //   }
// //   if(!obj[F("top")][F(D_JSON_SAT)].isNull()){ 
// //     uint8_t sat = obj[F("top")][F(D_JSON_SAT)];
// //     // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_SAT,sat);
// //     ambilightsettings.screens[SCREEN_CENTRE].top.colour.S = pCONT_iLight->SatN2F(sat);
// //     // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_SAT,ambilightsettings.screens[SCREEN_CENTRE].top.colour.S);
// //     // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_SAT,ambilightsettings.screens[SCREEN_CENTRE].top.colour.S);
// //     data_buffer.isserviced++;
// //   }
// //   if(!obj[F("top")][F(D_JSON_BRT)].isNull()){ 
// //     uint8_t brt = obj[F("top")][F(D_JSON_BRT)];
// //     // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_BRT,brt);
// //     ambilightsettings.screens[SCREEN_CENTRE].top.colour.B = pCONT_iLight->animation.brightness = pCONT_iLight->BrtN2F(brt);
// //     // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_BRT,pCONT_iLight->animation.brightness);
// //     // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_BRT,pCONT_iLight->animation.brightness);
// //     data_buffer.isserviced++;
// //   }



// //   if(!obj[F("bottom")][F(D_JSON_HUE)].isNull()){ 
// //     uint16_t hue = obj[F("bottom")][F(D_JSON_HUE)];
// //     // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_HUE,hue);
// //     ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.H = pCONT_iLight->HueN2F(hue);
// //     // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_HUE,ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.H);
// //     // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_HUE,ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.H);
// //     data_buffer.isserviced++;
// //   }
// //   if(!obj[F("bottom")][F(D_JSON_SAT)].isNull()){ 
// //     uint8_t sat = obj[F("bottom")][F(D_JSON_SAT)];
// //     // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_SAT,sat);
// //     ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.S = pCONT_iLight->SatN2F(sat);
// //     // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_SAT,ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.S);
// //     // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_SAT,ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.S);
// //     data_buffer.isserviced++;
// //   }
// //   if(!obj[F("bottom")][F(D_JSON_BRT)].isNull()){ 
// //     uint8_t brt = obj[F("bottom")][F(D_JSON_BRT)];
// //     // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_BRT,brt);
// //     ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.B = pCONT_iLight->animation.brightness = pCONT_iLight->BrtN2F(brt);
// //     // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_FVALUE),D_JSON_BRT,pCONT_iLight->animation.brightness);
// //     // Response_mP(S_JSON_COMMAND_FVALUE,D_JSON_BRT,pCONT_iLight->animation.brightness);
// //     data_buffer.isserviced++;
// //   }




// //   if(!obj[D_JSON_RGB].isNull()){
// //     const char* rgbpacked = obj[D_JSON_RGB];
// //     uint32_t colour32bit = 0;
// //     if(rgbpacked[0]=='#'){ colour32bit = (long) strtol( &rgbpacked[1], NULL, 16);
// //     }else{ colour32bit = (long) strtol( &rgbpacked[0], NULL, 16); }
// //     RgbColor rgb;
// //     rgb.R = colour32bit >> 16; //RGB
// //     rgb.G = colour32bit >> 8 & 0xFF; //RGB
// //     rgb.B = colour32bit & 0xFF; //RGB
// //     // #ifndef ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT
// //     // scene.colour = HsbColor(RgbColor(rgb.R,rgb.G,rgb.B));
// //     // // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_RGB ":%s " D_NEOPIXEL_RGB ":%d,%d,%d " D_NEOPIXEL_HSB ":%d,%d,%d"),
// //     // //   rgbpacked,rgb.R,rgb.G,rgb.B,scene.colour.H,scene.colour.S,scene.colour.B);
// //     // #endif //ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT
// //   }

// //   // TIME with different units
// //   if(!obj[D_JSON_TIME].isNull()){ //default to secs
// //     pCONT_iLight->animation.transition.time_ms.val = obj["time"];
// //     pCONT_iLight->animation.transition.time_ms.val *= 1000;
// //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),pCONT_iLight->animation.transition.time_ms.val);  
// //   }else
// //   if(!obj[D_JSON_TIME].isNull()){
// //     pCONT_iLight->animation.transition.time_ms.val = obj["time_secs"];
// //     pCONT_iLight->animation.transition.time_ms.val *= 1000;
// //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),pCONT_iLight->animation.transition.time_ms.val);  
// //   }else
// //   if(!obj[D_JSON_TIME_MS].isNull()){
// //     pCONT_iLight->animation.transition.time_ms.val = obj["time_ms"];
// //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),pCONT_iLight->animation.transition.time_ms.val);  
// //   }

  
// // Flash colour needs to NOT be a scene, but part of a manual direct mode
// // if(strstr(scenectr,"FLASHCOLOUR")){
// //       scene_reseting.name_id = scene.name_id; // remember scene to return to
// //       scene.name_id = MODE_SINGLECOLOUR_FLASHCOLOUR_ID;
// //     }
// //     // Parse out flashcolour info if it exists
// //     if(scene.name_id == MODE_SINGLECOLOUR_FLASHCOLOUR_ID){

// //       if(tempctr = obj["hue"]){
// //         scene_reseting.hue = obj["hue"];
// //       }
// //       if(tempctr = obj["sat"]){
// //         scene_reseting.sat = obj["sat"];
// //       }
// //       if(tempctr = obj["brt"]){
// //         scene_reseting.brt = obj["brt"];
// //       }
// //       if(tempctr = obj["time"]){
// //         scene_reseting.tOnTime = obj["time"];
// //       }
// //     }
  
  



// //   // create easier names
// //   // char* topic_ctr = data_buffer.topic.ctr;
// //   // uint8_t topic_len = data_buffer.topic.len;
// //   // char* payload_ctr = data_buffer.payload.ctr;
// //   // uint8_t payload_len = data_buffer.payload.len;

// //   if(mSupport::memsearch(data_buffer.topic.ctr,data_buffer.topic.len,"/center",sizeof("/center")-1)>=0){pCONT->mso->MessagePrintln("MATCHED /center");

// //     memset(&parsed,0,sizeof(parsed)); // clear parsing struct

// //     uint16_t index = 0;

// //     ambilightsettings.colour.found_idx = 0;

// //     StaticJsonDocument<300> doc;
// //     DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
// //     JsonObject root = doc.as<JsonObject>();

// //     if(root["RGB"].is<JsonArray>()){

// //       JsonArray colourarray = root["RGB"];
// //       int array_length = colourarray.size();

// //       const char* pixelcolour;
// //       for(JsonVariantConst v : colourarray) {
// //         pixelcolour = v.as<const char*>();

// //         if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
// //         }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }

// //           ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R = colour32bit >> 16; //RGB
// //           ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G = colour32bit >> 8 & 0xFF; //RGB
// //           ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B = colour32bit & 0xFF; //RGB


// //         ambilightsettings.colour.found_idx++;
// //         index++;
// //       }
// //       pCONT->mso->MessagePrint("ENDambilightsettings.col.found_idx");
// //       pCONT->mso->MessagePrintln(ambilightsettings.colour.found_idx);
// //       pCONT->mso->MessagePrintln(index);

// //     }
// //     // else if(root["RGB"].is<const char*>()){ //one colour = string
// //     //
// //     //   const char* pixelcolour;
// //     //   pixelcolour = root["RGB"];
// //     //
// //     //   //Serial.println("pixelcolour"); Serial.println(pixelcolour);
// //     //   if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
// //     //   }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }
// //     //
// //     //   ambilightsettings.colour.r[ambilightsettings.colour.found_idx] = colour32bit >> 16; //RGB
// //     //   ambilightsettings.colour.g[ambilightsettings.colour.found_idx] = colour32bit >> 8 & 0xFF; //RGB
// //     //   ambilightsettings.colour.b[ambilightsettings.colour.found_idx] = colour32bit & 0xFF; //RGB
// //     //
// //     //   // Serial.println(parsed.col.r[parsed.col.found_idx]);
// //     //   // Serial.println(parsed.col.g[parsed.col.found_idx]);
// //     //   // Serial.println(parsed.col.b[parsed.col.found_idx]);
// //     //
// //     //   ambilightsettings.colour.found_idx++;
// //     // }

// //     pCONT->mso->MessagePrint("ambilightsettings.colour.found_idx");
// //     pCONT->mso->MessagePrintln(ambilightsettings.colour.found_idx);
// //     //pCONT_iLight->light_size_count
// //     //ambilightsettings.colour.found_idx
// //     for(int i=0;i<index;i++){
// //         SetPixelColor(i, RgbColor(ambilightsettings.colour.rgb[i].R,ambilightsettings.colour.rgb[i].G,ambilightsettings.colour.rgb[i].B));
// //     }stripbus->Show();


// // } // END center


// // #ifdef DEVICE_RGBDELL

// //   if(mSupport::memsearch(data_buffer.topic.ctr,data_buffer.topic.len,"/left",sizeof("/left")-1)>=0){pCONT->mso->MessagePrintln("MATCHED /left");

// //     memset(&parsed,0,sizeof(parsed)); // clear parsing struct

// //     ambilightsettings.colour.found_idx = 0;

// //     StaticJsonDocument<300> doc;
// //     DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
// //     JsonObject root = doc.as<JsonObject>();

// //   // PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM -

// //     if(root["RGB"].is<JsonArray>()){
// //       //pCONT->mso->MessagePrintln("colour arr ");//Serial.println(rgbname_ctr);

// //       JsonArray colourarray = root["RGB"];
// //       int array_length = colourarray.size();

// //       const char* pixelcolour;
// //       for(JsonVariantConst v : colourarray) {
// //         pixelcolour = v.as<const char*>();

// //         //pCONT->mso->MessagePrintln("pixelcolour"); //pCONT->mso->MessagePrintln(pixelcolour);

// //         if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
// //         }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }

// //         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R = colour32bit >> 16; //RGB
// //         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G = colour32bit >> 8 & 0xFF; //RGB
// //         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B = colour32bit & 0xFF; //RGB

// //          pCONT->mso->MessagePrint(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R);pCONT->mso->MessagePrint("-");
// //          pCONT->mso->MessagePrint(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G);pCONT->mso->MessagePrint("-");
// //          pCONT->mso->MessagePrintln(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B);

// //         ambilightsettings.colour.found_idx++;
// //       }

// //     }else if(root["RGB"].is<const char*>()){ //one colour = string

// //       const char* pixelcolour;
// //       pixelcolour = root["RGB"];

// //       //Serial.println("pixelcolour"); Serial.println(pixelcolour);
// //       if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
// //       }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }

// //       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R = colour32bit >> 16; //RGB
// //       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G = colour32bit >> 8 & 0xFF; //RGB
// //       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B = colour32bit & 0xFF; //RGB

// //       // Serial.println(parsed.col.r[parsed.col.found_idx]);
// //       // Serial.println(parsed.col.g[parsed.col.found_idx]);
// //       // Serial.println(parsed.col.b[parsed.col.found_idx]);

// //       ambilightsettings.colour.found_idx++;
// //     }

// //     pCONT->mso->MessagePrintln(ambilightsettings.colour.found_idx);
// //     //pCONT_iLight->light_size_count
// //     //ambilightsettings.colour.found_idx
// //     //  for(int i=0;i<ambilightsettings.colour.found_idx;i++){
// //     //    SetPixelColor(i,RgbColor(ambilightsettings.colour.r[i],ambilightsettings.colour.g[i],ambilightsettings.colour.b[i]));    //turn every third pixel on
// //     //  }
// //     // /stripbus->Show();

// //     uint32_t c; //colourrgb
// //     pinMode(RGB_DATA_LEFT_PIN,OUTPUT);
// //     pinMode(RGB_CLOCK_LEFT_PIN,OUTPUT);

// //     digitalWrite(RGB_DATA_LEFT_PIN,LOW);digitalWrite(RGB_CLOCK_LEFT_PIN,LOW);
// //     for(int ii=0;ii<ambilightsettings.colour.found_idx;ii++){
// //       shiftOut(RGB_DATA_LEFT_PIN, RGB_CLOCK_LEFT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].R);
// //       shiftOut(RGB_DATA_LEFT_PIN, RGB_CLOCK_LEFT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].G);
// //       shiftOut(RGB_DATA_LEFT_PIN, RGB_CLOCK_LEFT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].B);
// //     }
// //     digitalWrite(RGB_DATA_LEFT_PIN,LOW);digitalWrite(RGB_CLOCK_LEFT_PIN,LOW);



// // } // END left



// //   if(mSupport::memsearch(data_buffer.topic.ctr,data_buffer.topic.len,"/right",sizeof("/right")-1)>=0){pCONT->mso->MessagePrintln("MATCHED /right");

// //     memset(&parsed,0,sizeof(parsed)); // clear parsing struct

// //     ambilightsettings.colour.found_idx = 0;

// //     StaticJsonDocument<300> doc;
// //     DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
// //     JsonObject root = doc.as<JsonObject>();

// //   // PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM -

// //     if(root["RGB"].is<JsonArray>()){
// //       //pCONT->mso->MessagePrintln("colour arr ");//Serial.println(rgbname_ctr);

// //       JsonArray colourarray = root["RGB"];
// //       int array_length = colourarray.size();

// //       const char* pixelcolour;
// //       for(JsonVariantConst v : colourarray) {
// //         pixelcolour = v.as<const char*>();

// //         //pCONT->mso->MessagePrintln("pixelcolour"); //pCONT->mso->MessagePrintln(pixelcolour);

// //         if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
// //         }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }

// //         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R = colour32bit >> 16; //RGB
// //         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G = colour32bit >> 8 & 0xFF; //RGB
// //         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B = colour32bit & 0xFF; //RGB

// //          pCONT->mso->MessagePrint(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R);pCONT->mso->MessagePrint("-");
// //          pCONT->mso->MessagePrint(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G);pCONT->mso->MessagePrint("-");
// //          pCONT->mso->MessagePrintln(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B);

// //         ambilightsettings.colour.found_idx++;
// //       }

// //     }else if(root["RGB"].is<const char*>()){ //one colour = string

// //       const char* pixelcolour;
// //       pixelcolour = root["RGB"];

// //       //Serial.println("pixelcolour"); Serial.println(pixelcolour);
// //       if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
// //       }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }

// //       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R = colour32bit >> 16; //RGB
// //       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G = colour32bit >> 8 & 0xFF; //RGB
// //       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B = colour32bit & 0xFF; //RGB

// //       // Serial.println(parsed.col.r[parsed.col.found_idx]);
// //       // Serial.println(parsed.col.g[parsed.col.found_idx]);
// //       // Serial.println(parsed.col.b[parsed.col.found_idx]);

// //       ambilightsettings.colour.found_idx++;
// //     }

// //     pCONT->mso->MessagePrintln(ambilightsettings.colour.found_idx);
// //     //pCONT_iLight->light_size_count
// //     //ambilightsettings.colour.found_idx
// //     // for(int i=0;i<ambilightsettings.colour.found_idx;i++){
// //     //   mrgbneo_ani->setPixelColor(i,mrgbneo_ani->Color(ambilightsettings.colour.r[i],ambilightsettings.colour.g[i],ambilightsettings.colour.b[i]));    //turn every third pixel on
// //     // }
// //     // mrgbneo_ani->setBrightness(255);
// //     // mrgbneo_ani->show();

// //     uint32_t c; //colourrgb
// //     pinMode(RGB_DATA_RIGHT_PIN,OUTPUT);
// //     pinMode(RGB_CLOCK_RIGHT_PIN,OUTPUT);

// //     digitalWrite(RGB_DATA_RIGHT_PIN,LOW);digitalWrite(RGB_CLOCK_RIGHT_PIN,LOW);
// //     for(int ii=0;ii<ambilightsettings.colour.found_idx;ii++){
// //       shiftOut(RGB_DATA_RIGHT_PIN, RGB_CLOCK_RIGHT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].R);
// //       shiftOut(RGB_DATA_RIGHT_PIN, RGB_CLOCK_RIGHT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].G);
// //       shiftOut(RGB_DATA_RIGHT_PIN, RGB_CLOCK_RIGHT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].B);
// //     }
// //     digitalWrite(RGB_DATA_RIGHT_PIN,LOW);digitalWrite(RGB_CLOCK_RIGHT_PIN,LOW);



// // } // END left

// //       //fShowPanelUpdate = true;
// //     //}


// // //  }

// //   //switch modes : USE serial input stream
// //   //send as json array?
// //   //RGB = [[r,g,b],[r,g,b],[r,g,b]]
// //   //SETTINGS = {pixel ratio, 17,10}{pixel direction, CW}{startposition, bottom right}{timeout,10}{minbrightness,10}{fade,0}

// //   pCONT_iLight->animation.mode_id = MODE_AMBILIGHT_ID;
// //   fForcePanelUpdate = true;

//   // return 0;

// } // END FUNCTION


/*******************************************************************************************************************
********************************************************************************************************************
************ END OF AMBILIGHT DEFINITIONS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/

#endif // ENABLE_PIXEL_FUNCTION_AMBILIGHT


#ifdef ENABLE_PIXEL_FUNCTION_FLASHER

void mRGBAnimator::init_flasher_settings(){

  flashersettings.flags.movement_direction = 0;
  


// A "wipe" or "sine wave" that applies a saturation change across the string. So either, from all christmas coloured to slowly blending from the top of the tree to the bottom, to white (or another solid colour), or apply the saturation change in a loop/rotation. 


  
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
    case FLASHER_FUNCTION_SLOW_GLOW_ID:   memcpy_P(buffer, PM_ANIMATION_MODE_FLASHER_NAME_CTR, sizeof(PM_ANIMATION_MODE_FLASHER_NAME_CTR)); break;


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
  else if(strcmp_P(c,PM_FLASHER_REGION_ANIMATE_NAME_CTR)==0){       return FLASHER_REGION_ANIMATE_ID; }
  return -1;
}
const char* mRGBAnimator::GetFlasherRegionName(char* buffer){
  return GetFlasherRegionNamebyID(flashersettings.region, buffer);
}
const char* mRGBAnimator::GetFlasherRegionNamebyID(uint8_t id, char* buffer){
  switch(id){
    case FLASHER_REGION_COLOUR_SELECT_ID:   memcpy_P(buffer,PM_FLASHER_REGION_COLOUR_SELECT_NAME_CTR,sizeof(PM_FLASHER_REGION_COLOUR_SELECT_NAME_CTR));
    case FLASHER_REGION_ANIMATE_ID:         memcpy_P(buffer,PM_FLASHER_REGION_ANIMATE_NAME_CTR,sizeof(PM_FLASHER_REGION_ANIMATE_NAME_CTR));
  }
  return buffer;
}

void mRGBAnimator::SubTask_Flasher_Main(){

#ifdef ENABLE_PIXEL_FUNCTION_MIXER
  if(mixer.flags.Enabled){
   SubTask_Flasher_Animate_Mixer();
  }
  #endif // ENABLE_PIXEL_FUNCTION_MIXER

  SubTask_Flasher_Animate();

} // SubTask_Flasher_Main



void mRGBAnimator::SubTask_Flasher_Animate(){
  
  /**
   * Timer (seconds) to update the FLASHER_REGION_COLOUR_SELECT_ID when otherwise remains static
   * */
  SubTask_Flasher_Animate_Parameter_Check_Update_Timer_Change_Colour_Region();
  /**
   * Timer (seconds) to randomise the rates of change. Methods include (random between range, linear map, exponential (longer slow), exponential (longer fast))
   * */  
  SubTask_Flasher_Animate_Parameter_Check_Update_Timer_Transition_Rate();
  /**
   * Timer (seconds) to randomise the transition time of change. Methods include (random between range, linear map, exponential (longer slow), exponential (longer fast))
   * */    
  SubTask_Flasher_Animate_Parameter_Check_Update_Timer_Transition_Time();


  if((mTime::TimeReached(&flashersettings.tSaved.Update,pCONT_iLight->animation.transition.rate_ms.val))||(pCONT_iLight->animation.flags.fForceUpdate)){

    // #ifdef ENABLE_DEVFEATURE_FLICKERING_TEST2
    // if(pCONT_iLight->animator_controller->IsAnimating()){
    //   AddLog_P(LOG_LEVEL_TEST, PSTR("Blocking new animation"));
    //   return;
    // }else{
      // flashersettings.tSaved.Update = millis();
    // }
    // #endif // ENABLE_DEVFEATURE_FLICKERING_TEST2

    if(pCONT_iLight->animation.flags.fForceUpdate){ pCONT_iLight->animation.flags.fForceUpdate=false;
      flashersettings.tSaved.Update = millis();
    }


    #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
    char buffer[100];
    //AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "flashersettings.function=%d %s"),flashersettings.function,GetFlasherFunctionNamebyID(flashersettings.function, buffer));
    #endif

    switch(flashersettings.function){
      default:
      case FLASHER_FUNCTION_SLOW_GLOW_ID:
        SubTask_Flasher_Animate_Function_Slow_Glow();
      break;
      case FLASHER_FUNCTION_SLOW_GLOW_ON_BRIGHTNESS_ID:
        SubTask_Flasher_Animate_Function_Slow_Glow_On_Brightness();
      break;
      case FLASHER_FUNCTION_SLOW_GLOW_PARTIAL_PALETTE_STEP_THROUGH_ID:
        SubTask_Flasher_Animate_Function_Slow_Glow_Partial_Palette_Step_Through();
      break;
      case FLASHER_FUNCTION_SEQUENTIAL_ID:
        SubTask_Flasher_Animate_Function_Sequential();
      break;
      case FLASHER_FUNCTION_FLASH_TWINKLE_SINGLE_COLOUR_RANDOM_ID:
        SubTask_Flasher_Animate_Function_Twinkle_SingleColour_Random();
      break;
      case FLASHER_FUNCTION_FLASH_TWINKLE_PALETTE_COLOUR_RANDOM_ID:
        SubTask_Flasher_Animate_Function_Twinkle_PaletteColour_Random();
      break;
      case FLASHER_FUNCTION_SLOW_FADE_BRIGHTNESS_ALL_ID:
        SubTask_Flasher_Animate_Function_Slow_Fade_Brightness_All();
      break;
      case FLASHER_FUNCTION_SLOW_FADE_SATURATION_ALL_ID:
        SubTask_Flasher_Animate_Function_Slow_Fade_Saturation_All();
      break;
      case FLASHER_FUNCTION_FADE_GRADIENT_ID:
        SubTask_Flasher_Animate_Function_Fade_Gradient();
      break;
      case FLASHER_FUNCTION_TESTER_ID:
        SubTask_Flasher_Animate_Function_Tester();
      break; 
      case FLASHER_FUNCTION_PULSE_RANDOM_ON:
        SubTask_Flasher_Animate_Function_Pulse_Random_On();
      break;
      case FLASHER_FUNCTION_PULSE_RANDOM_ON_TWO_ID:
        SubTask_Flasher_Animate_Function_Pulse_Random_On_2();
      break;
      case FLASHER_FUNCTION_POPPING_PALETTE_BRIGHTNESS_FROM_LOWER_TO_UPPER_BOUNDERY:
        SubTask_Flasher_Animate_Function_Popping_Palette_Brightness_From_Lower_To_Upper_Boundary();
      break;
      case FLASHER_FUNCTION_TWINKLE_PALETTE_BRIGHTNESS_FROM_LOWER_TO_UPPER_AND_BACK:
        SubTask_Flasher_Animate_Function_Twinkle_Palette_Brightness_From_Lower_To_Upper_And_Back();
      break;


      // case FLASHER_FUNCTION_SHOWING_MULTIPLES_OF_COLOURS_ID:
      //   SubTask_Flasher_Animate_Function_Slow_Glow_Showing_Multiples_Of_Colours();
      // break;

//new pastel random ie change pixel colour 90-100% saturation with slow blend time and randomly
      // case FLASHER_FUNCTION_FADE_GRADIENT_ID:
      //   SubTask_Flasher_Animate_Function_Fade_Gradient();
      // break;

      // New animation
      // slowly random change between palette paired colours, ie red, green, blue, pink in pallette, first red/green, then blue/pink, or with flag, only skip one colour for each pair, red/green, green/blue, blue/pink etc


//pulse will use the "multiplier" number, and every X will turn on, moving by a few each time, fading off after instant on 
// instant on or pulsing on? 
// maybe I need to make "transition waveforms", square (instant), sine wave, pulsing, saw tooth (rising edge, falling edge)

     

      //https://photos.google.com/search/green%20light/photo/AF1QipONCC4dPcCk3AUmff17venI28Jih0iAsLwwX-Po
      //gradient light

      //apply saturation rotation or "wipe"

    } //end SWITCH

    // // Configure animator to show output
    // StartAnimationAsBlendFromStartingColorToDesiredColor();
    StartAnimation_AsAnimUpdateMemberFunction();

  }//end if update reached

} //end flasher_animate




/**
 * Timer (seconds) to update the FLASHER_REGION_COLOUR_SELECT_ID when otherwise remains static
 * */
void mRGBAnimator::SubTask_Flasher_Animate_Parameter_Check_Update_Timer_Change_Colour_Region(){

  if(flashersettings.update_colour_region.refresh_secs || flashersettings.update_colour_region.refresh_decounter_secs){ // >0 is active
    if(mTime::TimeReached(&flashersettings.update_colour_region.tSaved, 1000)){
      AddLog_P(LOG_LEVEL_TEST, PSTR("rate_colours_secs=%d, rate_decounter=%d"),flashersettings.update_colour_region.refresh_secs, flashersettings.update_colour_region.refresh_decounter_secs);
      if(flashersettings.update_colour_region.refresh_decounter_secs == 1){
        flashersettings.update_colour_region.refresh_decounter_secs = flashersettings.update_colour_region.refresh_secs; // reset
        flashersettings.region = FLASHER_REGION_COLOUR_SELECT_ID;
        flashersettings.flags.force_finish_flasher_pair_once = 1;
      }else
      if(flashersettings.update_colour_region.refresh_decounter_secs > 1){
        flashersettings.update_colour_region.refresh_decounter_secs--;
      }
      //
      else{
        flashersettings.update_colour_region.refresh_decounter_secs = flashersettings.update_colour_region.refresh_secs; // reset
      }
    }
  }

}

 /**
   * Timer (seconds) to randomise the rates of change. Methods include (random between range, linear map, exponential (longer slow), exponential (longer fast))
   * */  
  void mRGBAnimator::SubTask_Flasher_Animate_Parameter_Check_Update_Timer_Transition_Rate(){
  // Generate randomness to transitions if flag is set
  // if(flashersettings.flags.enable_random_rate){
  //   if(mTime::TimeReached(&flashersettings.random_transitions.tSavedNewSpeedUpdate,flashersettings.random_transitions.rate_ms)){
  //     flashersettings.random_transitions.rate_ms = (random(flashersettings.random_transitions.lower_secs,flashersettings.random_transitions.upper_secs)*1000);
  //     pCONT_iLight->animation.transition.rate_ms.val = flashersettings.random_transitions.array[flashersettings.random_transitions.array_index++]*10;
  //     if(flashersettings.random_transitions.array_index>=flashersettings.random_transitions.array_index_length) flashersettings.random_transitions.array_index=0;
  //     pCONT_iLight->animation.transition.time_ms.val = pCONT_iLight->animation.transition.rate_ms.val/4; //75% of time spent on desired colour
  //   }
  // }
  }

  /**
   * Timer (seconds) to randomise the transition time of change. Methods include (random between range, linear map, exponential (longer slow), exponential (longer fast))
   * */ 
  void mRGBAnimator::SubTask_Flasher_Animate_Parameter_Check_Update_Timer_Transition_Time(){
  // if(mTime::TimeReached(&flashersettings.function_seq.tSavedNewSpeedUpdate,(random(3,10)*1000))){
  //   uint8_t values[8] = {4,8,10,17,20,17,10,8};
  //   pCONT_iLight->animation.transition.rate_ms = values[flashersettings.function_seq.rate_index++]*50;
  //   if(flashersettings.function_seq.rate_index>8) flashersettings.function_seq.rate_index=0;
  //   pCONT_iLight->animation.transition.time_ms = pCONT_iLight->animation.transition.rate_ms/4; //75% of time spent on desired colour
  // }
  }


// /****
//  * Changes pixels randomly to new colour, with slow blending
//  * Requires new colour calculation each call
//  */
// void mRGBAnimator::SubTask_Flasher_Animate_Function_Slow_Glow(){
//   // So colour region does not need to change each loop to prevent colour crushing
//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
//   UpdateDesiredColourFromPaletteSelected();
// }



void mRGBAnimator::UpdateStartingColourWithGetPixel(){

  for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){

    // Test fix, if desired does not match getcolour, then use ...


    animation_colours[pixel].StartingColor = GetPixelColor(pixel);
  }

}

void mRGBAnimator::UpdateStartingColourWithSingleColour(RgbcctColor colour){

  for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
    animation_colours[pixel].StartingColor = colour;
  }

}

void mRGBAnimator::UpdateDesiredColourWithSingleColour(RgbcctColor colour){

  for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
    animation_colours[pixel].DesiredColour = colour;
  }

}


void mRGBAnimator::RotateDesiredColour(uint8_t pixels_amount_to_shift, uint8_t direction){

//pixels_amount_to_shift loop this many times

  if(direction){ // direction==1 move right ie AWAY from start

    // Shift colours (rotate)
    RgbcctColor colourlast = animation_colours[pCONT_iLight->light_size_count-1].DesiredColour;//desired_colour[0];
    // Shift towards first pixel
    for(ledout.index=pCONT_iLight->light_size_count-1; //last to first
        ledout.index>0;
        ledout.index--
      ){ //+1?
      // move backwards
      animation_colours[ledout.index].DesiredColour = animation_colours[ledout.index-1].DesiredColour;
    }
    // Put first pixel on the end (wrap around)
    animation_colours[0].DesiredColour = colourlast;

  }else{

    // Shift colours (rotate)
    RgbcctColor colourfirst = animation_colours[0].DesiredColour;//desired_colour[0];
    // Shift towards first pixel
    for(ledout.index=0;ledout.index<pCONT_iLight->light_size_count;ledout.index++){ 
      animation_colours[ledout.index].DesiredColour = animation_colours[ledout.index+1].DesiredColour;
    }
    // Put first pixel on the end (wrap around)
    animation_colours[ledout.index-1].DesiredColour = colourfirst;
  }

}


// void mRGBAnimator::SubTask_Flasher_Animate_Function_Sequential(){

//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
  
//   flashersettings.flag_finish_flasher_pair = false;
//   flashersettings.flags.enable_random_rate = true;
  
//   do{ //must complete the pair together //move inside functions
//     switch(flashersettings.region){
//       case FLASHER_REGION_COLOUR_SELECT_ID: //set colours
//         #ifdef ENABLE_LOG_LEVEL_DEBUG
//         AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_COLOUR_SELECT"));
//         #endif
//         UpdateDesiredColourFromPaletteSelected();
//         flashersettings.region = FLASHER_REGION_ANIMATE_ID;
//       break;
//       case FLASHER_REGION_ANIMATE_ID: //shift along
//         #ifdef ENABLE_LOG_LEVEL_DEBUG
//         AddLog_P(LOG_LEVEL_DEBUG_LOWLEVEL,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_ANIMATE"));
//         #endif
//         RotateDesiredColour(1,flashersettings.flags.movement_direction);
//         if(flashersettings.flags.force_finish_flasher_pair_once){
//           AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("flashersettings.flags.force_finish_flasher_pair_once WAS SET"));
//           flashersettings.flags.force_finish_flasher_pair_once = false;
//         }
//       break;
//     }
//   }while(flashersettings.flag_finish_flasher_pair || flashersettings.flags.force_finish_flasher_pair_once);

// }

// void mRGBAnimator::SubTask_Flasher_Animate_Function_Slow_Glow_On_Brightness(){

//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = false;

//   flashersettings.flag_finish_flasher_pair = false;
//   flashersettings.flags.enable_random_rate = true;

//   // Best "inorder" to compute colour again then brightness
  
//   do{ //must complete the pair together //move inside functions
//     switch(flashersettings.region){
//       case FLASHER_REGION_COLOUR_SELECT_ID: //set colours
//         #ifdef ENABLE_LOG_LEVEL_DEBUG
//         AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_COLOUR_SELECT"));
//         #endif
//         UpdateDesiredColourFromPaletteSelected();
//         flashersettings.region = FLASHER_REGION_ANIMATE_ID;
//       break;
//       case FLASHER_REGION_ANIMATE_ID: //shift along
//         #ifdef ENABLE_LOG_LEVEL_DEBUG
//         AddLog_P(LOG_LEVEL_DEBUG_LOWLEVEL,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_ANIMATE"));
//         #endif
//         // RotateDesiredColour(1,flashersettings.flags.movement_direction);

//         // Calculate new pattern to change
//         RefreshLEDIndexPattern();

//         // Global brightness is already applied, and will be known as "max range"
//         // Min range will be another map change here
//         uint8_t max_brightness = pCONT_iLight->getBriRGB();
//         uint8_t min_brightness = flashersettings.brightness_min;
//         uint8_t random_brightness = 0;

//         if(min_brightness > max_brightness){
//           min_brightness = max_brightness;
//         }

//         // Change only selected pixel brightness levels
//         for(ledout.index=0;ledout.index<strip_size_requiring_update;ledout.index++){
//           random_brightness = random(min_brightness,max_brightness);          
//           animation_colours[ledout.pattern[ledout.index]].DesiredColour = ApplyBrightnesstoDesiredColour(
//                                                                               animation_colours[ledout.pattern[ledout.index]].DesiredColour,
//                                                                               random_brightness);
//         }

//         if(flashersettings.flags.force_finish_flasher_pair_once){
//           AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("flashersettings.flags.force_finish_flasher_pair_once WAS SET"));
//           flashersettings.flags.force_finish_flasher_pair_once = false;
//         }
//       break;
//     }
//   }while(flashersettings.flag_finish_flasher_pair || flashersettings.flags.force_finish_flasher_pair_once);


// }


// // Fade solid colour from 0 to 75%, and a palette from 25 to 100% (50% overlap)
// void mRGBAnimator::SubTask_Flasher_Animate_Function_Fade_Gradient(){
    
// #ifdef ENABLE_FLASHER_ANIMATE_FUNCTION_FADE_GRADIENT

//   flashersettings.flag_finish_flasher_pair = false;
//   flashersettings.flags.enable_random_rate = true;
  
//   // Apply green gradient, brightest at start

//   uint16_t start = pCONT_iLight->light_size_count/2;
//   uint16_t end = pCONT_iLight->light_size_count; 
//   RgbTypeColor colour_gradient = HsbColor(
//                                           pCONT_iLight->HueN2F(120),
//                                           pCONT_iLight->SatN2F(100),
//                                           pCONT_iLight->BrtN2F(map(pCONT_iLight->getBriRGB(),0,255,0,100))  
//                                           );
//   RgbTypeColor colour_random = RgbTypeColor(255,0,0); 
//   HsbColor colour_random_adjusted = HsbColor(0);
//   uint8_t gradient_end_percentage = 75;
//   uint16_t strip_size_gradient = pCONT_iLight->light_size_count*(gradient_end_percentage/100.0f);
//   uint16_t strip_size_single   = pCONT_iLight->light_size_count*(75/100.0f);
  
//   start = 0;
//   end = pCONT_iLight->light_size_count;
//   for(ledout.index=start;ledout.index<end;ledout.index++){ 
//     animation_colours[ledout.index].DesiredColour = RgbTypeColor(0);
//   }
  
//   //0 to 75% 
//   start = 0;
//   end = map(75,0,100,0,pCONT_iLight->light_size_count);
//   for(ledout.index=start;ledout.index<end;ledout.index++){ 
//     animation_colours[ledout.index].DesiredColour.R = pCONT_iLight->ledGamma(map(ledout.index,start,end,colour_gradient.R,0));
//     animation_colours[ledout.index].DesiredColour.G = pCONT_iLight->ledGamma(map(ledout.index,start,end,colour_gradient.G,0));
//     animation_colours[ledout.index].DesiredColour.B = pCONT_iLight->ledGamma(map(ledout.index,start,end,colour_gradient.B,0));
//   }

//   pCONT_iLight->SetPaletteListPtrFromID(pCONT_iLight->animation.palette_id);
//   uint8_t pixels = pCONT_iLight->GetPixelsInMap(pCONT_iLight->palettelist.ptr);
//   uint8_t desired_pixel;
  
//   // 25 to 100%
//   start = map(25,0,100,0,pCONT_iLight->light_size_count);
//   end = pCONT_iLight->light_size_count;
//   for(ledout.index=start;ledout.index<end;ledout.index++){ 
//     desired_pixel = random(0,pixels-1);
//     colour_random = pCONT_iLight->GetColourFromPalette(pCONT_iLight->palettelist.ptr,desired_pixel);
//     if((ledout.index%3)==0){
//       colour_random_adjusted = RgbTypeColor(colour_random);
//       colour_random_adjusted.B = pCONT_iLight->BrtN2F(map(pCONT_iLight->getBriRGB(),0,255,0,100));
//       animation_colours[ledout.index].DesiredColour = colour_random_adjusted;
//     }
//   }

//   #endif

// }


// void mRGBAnimator::SubTask_Flasher_Animate_Function_Twinkle_Random(){

// // #ifdef ENABLE_ADVANCED_MODE_TWINKLE // creating this to reduce "white_leds_index" size

//   switch(flashersettings.region){
//     case FLASHER_REGION_COLOUR_SELECT_ID: //set colours
//       AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_COLOUR_SELECT"));
//       UpdateDesiredColourFromPaletteSelected();
//       flashersettings.region = FLASHER_REGION_ANIMATE_ID;
//       pCONT_iLight->animation.transition.order_id = TRANSITION_ORDER_INORDER_ID;

//       flash_twinkle_random.white_pixel_amount = 8; //how often the leds are
//       // by percentage
//       //flash_twinkle_random.white_pixel_amount = pCONT_iLight->light_size_count/5;

//       // Generate ledindex for white flashers
//       flash_twinkle_random.white_total_index = 0;
//       for(ledout.index=0;ledout.index<pCONT_iLight->light_size_count;ledout.index++){ 
//         //map type of led they are by numnber 0=off,1=white,2=colour
//         if(!(ledout.index%flash_twinkle_random.white_pixel_amount)){
//           flash_twinkle_random.white_leds_index[flash_twinkle_random.white_total_index] = ledout.index;
//           flash_twinkle_random.stored_colours_index[flash_twinkle_random.white_total_index] = desired_colour[ledout.index];
//           //AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "flash_twinkle_random.white_leds_index %d ledout.index %d"),flash_twinkle_random.white_total_index,ledout.index);
//           flash_twinkle_random.white_total_index++;
//         }              
//       }   
                  
//       //flash_twinkle_random.flash_colour = HsbColor(0,0,1); //white

//       // Test clear
//       // for(int ledout.index=0;ledout.index<pCONT_iLight->light_size_count;ledout.index++){ desired_colour[ledout.index] = HsbColor(RgbColor(0,0,0)); }

//       flashersettings.flag_finish_flasher_pair = true; // must complete regions
      
//     break;

//     case FLASHER_REGION_ANIMATE_ID: //shift along
//       AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_ANIMATE"));

//       // Reset all back to colour first 
//       for(int jj=0;jj<flash_twinkle_random.white_total_index;jj++){
//         desired_colour[flash_twinkle_random.white_leds_index[jj]] = flash_twinkle_random.stored_colours_index[jj];
//       }

//       // Pick 10 random of the leds to turn to white

      
//       for(int jj=0;jj<10;jj++){
//         uint16_t led_rand = random(0,flash_twinkle_random.white_total_index);
//         desired_colour[flash_twinkle_random.white_leds_index[led_rand]] = flash_twinkle_random.flash_colour;
//       }

//       flash_twinkle_random.white_on_index++;
//       if(flash_twinkle_random.white_on_index>=flash_twinkle_random.white_pixel_amount){
//         flash_twinkle_random.white_on_index = 0;
//         flashersettings.region = FLASHER_REGION_COLOUR_SELECT_ID; // Wrap back
//       }

//       flashersettings.flag_finish_flasher_pair = false; // must complete regions

//       // Change pCONT_iLight->animation speed
//       if(mTime::TimeReached(&flashersettings.function_flash_twinkle.tSavedNewSpeedUpdate,random(0,1)?5000:10000)){//(random(5,10)*1000))){
//         pCONT_iLight->animation.transition.rate_ms.val = random(0,1)?10:150; // will need to be very fast
//         pCONT_iLight->animation.transition.time_ms.val = 0; //instant
//       }
//     break;
//   }

//   // #endif

// }


// void mRGBAnimator::SubTask_Flasher_Animate_Function_Twinkle_Sequential(){
  
// //       case FLASHER_FUNCTION_FLASH_TWINKLE_SEQUENTIAL_ID:
// //         switch(flashersettings.region){
// //           case FLASHER_REGION_COLOUR_SELECT_ID: //set colours
// //             AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_COLOUR_SELECT"));
// //             UpdateDesiredColourFromPaletteSelected();
// //             flashersettings.region = FLASHER_REGION_ANIMATE_ID;
// //             pCONT_iLight->animation.transition.order_id = TRANSITION_ORDER_INORDER_ID;

// //             flash_twinkle_random.white_pixel_amount = 7; //how often the leds are
// //             // by percentage
// //             //flash_twinkle_random.white_pixel_amount = pCONT_iLight->light_size_count/5;

// //             // Generate ledindex for white flashers
// //             flash_twinkle_random.white_total_index = 0;
// //             for(ledout.index=0;ledout.index<pCONT_iLight->light_size_count;ledout.index++){ 
// //               //map type of led they are by numnber 0=off,1=white,2=colour
// //               if(!(ledout.index%flash_twinkle_random.white_pixel_amount)){
// //                 flash_twinkle_random.white_leds_index[flash_twinkle_random.white_total_index] = ledout.index;
// //                 flash_twinkle_random.stored_colours_index[flash_twinkle_random.white_total_index] = desired_colour[ledout.index];
// //                 //AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "flash_twinkle_random.white_leds_index %d ledout.index %d"),flash_twinkle_random.white_total_index,ledout.index);
// //                 flash_twinkle_random.white_total_index++;
// //               }              
// //             }   
                        
// //             //flash_twinkle_random.flash_colour = HsbColor(0,0,1); //white
// //             //flash_twinkle_random.flash_colour = HsbColor(HueN2F(HUE_HOTPINK),SatN2F(100),BrtN2F(100));

// //             // Test clear
// //             // for(int ledout.index=0;ledout.index<pCONT_iLight->light_size_count;ledout.index++){ desired_colour[ledout.index] = HsbColor(RgbColor(0,0,0)); }

// //             flashersettings.flag_finish_flasher_pair = true; // must complete regions
            
// //           break;
// //           case FLASHER_REGION_ANIMATE_ID: //shift along
// //             AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_ANIMATE"));
    
            
// //             for(int jj=0;jj<flash_twinkle_random.white_total_index;jj++){
// //               if((jj%flash_twinkle_random.white_pixel_amount)==flash_twinkle_random.white_on_index){
// //                 desired_colour[flash_twinkle_random.white_leds_index[jj]] = flash_twinkle_random.flash_colour;
// //               }else{
// //                 desired_colour[flash_twinkle_random.white_leds_index[jj]] = flash_twinkle_random.stored_colours_index[jj];//offcolour;
// //               }
// //               // AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "leds_index=%d on/amount=%d/%d on=%d"),
// //               // flash_twinkle_random.white_leds_index[jj],
// //               // flash_twinkle_random.white_on_index,
// //               // flash_twinkle_random.white_pixel_amount,
// //               // (jj%flash_twinkle_random.white_pixel_amount)==0?1:0);
// //             }

// //             flash_twinkle_random.white_on_index++;
// //             if(flash_twinkle_random.white_on_index>=flash_twinkle_random.white_pixel_amount){
// //               flash_twinkle_random.white_on_index = 0;
// //               flashersettings.region = FLASHER_REGION_COLOUR_SELECT_ID; // Wrap back
// //             }

// //             flashersettings.flag_finish_flasher_pair = false; // must complete regions

// //             // Change pCONT_iLight->animation speed
// //             if(mTime::TimeReached(&flashersettings.function_flash_twinkle.tSavedNewSpeedUpdate,random(0,1)?5000:10000)){//(random(5,10)*1000))){
// //               pCONT_iLight->animation.transition.rate_ms = 90;//random(0,1)?30:30; // will need to be very fast
// //               pCONT_iLight->animation.transition.time_ms = 0; //instant
// //             }
// //           break;
// //         }
// //       break;

// }

// void mRGBAnimator::SubTask_Flasher_Animate_Function_Slow_Fade_Brightness_All(){
     
      
// //       case FLASHER_FUNCTION_SLOW_FADE_BRIGHTNESS_ALL_ID:
// //         switch(flashersettings.region){
// //           case FLASHER_REGION_COLOUR_SELECT_ID: //set colours
// //             AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_COLOUR_SELECT"));
// //             UpdateDesiredColourFromPaletteSelected();
// //             flashersettings.region = FLASHER_REGION_ANIMATE_ID;
// //           break;
// //           case FLASHER_REGION_ANIMATE_ID: //shift along
// //             AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_ANIMATE"));
// //             // Change brightness from 0 to 100% (rotate)
// //             uint8_t direction = flashersettings.function_seq.rate_index%2;//(flashersettings.function_slow_fade.direction^=1);
// //             for(ledout.index=0;ledout.index<pCONT_iLight->light_size_count;ledout.index++){ desired_colour[ledout.index].B = direction; }            
// //             // Change pCONT_iLight->animation speed
// //             if(mTime::TimeReached(&flashersettings.function_seq.tSavedNewSpeedUpdate,(random(3,10)*100))){
// //               uint8_t values[8] = {1000,1000,2000,2000,6000,6000,3000,3000}; //off,on,off
// //               pCONT_iLight->animation.transition.rate_ms = values[flashersettings.function_seq.rate_index++]*50;
// //               if(flashersettings.function_seq.rate_index>=8) flashersettings.function_seq.rate_index=0;
// //               pCONT_iLight->animation.transition.time_ms = pCONT_iLight->animation.transition.rate_ms;///4; // brightness shift takes 100% of the time (no colour shift)
// //             }
// //           break;
// //         }
// //       break;
// }


// void mRGBAnimator::SubTask_Flasher_Animate_Function_Slow_Fade_Saturation_All(){
      
// //       case FLASHER_FUNCTION_SLOW_FADE_SATURATION_ALL_ID:
// //         switch(flashersettings.region){
// //           case FLASHER_REGION_COLOUR_SELECT_ID: //set colours
// //             AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_COLOUR_SELECT"));
// //             UpdateDesiredColourFromPaletteSelected();
// //             flashersettings.region = FLASHER_REGION_ANIMATE_ID;
// //           break;
// //           case FLASHER_REGION_ANIMATE_ID: //shift along
// //             AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "FLASHER_SEQUENTIAL FLASHER_ANIMATE"));
// //             // Change brightness from 0 to 100% (rotate)
// //             uint8_t direction = flashersettings.function_seq.rate_index%2;//(flashersettings.function_slow_fade.direction^=1);
// //             float random_saturation = direction ? 1 : ((float)random(0,100)/(float)100);

// //             float adjusted_brightness = random_saturation;// mSupport::mapfloat(random_saturation,
// //             if(random_saturation<0.6){
// //               adjusted_brightness = mSupport::mapfloat(random_saturation,
// //                                                                     0,  0.6,
// //                                                                     0.4,0.6);
// //             }
                        
// //             for(ledout.index=0;ledout.index<pCONT_iLight->light_size_count;ledout.index++){ 
// //               desired_colour[ledout.index].S = random_saturation; 
// //               desired_colour[ledout.index].B = adjusted_brightness;//random_saturation<0.5?pCONT_iLight->animation.brightness*0.5:pCONT_iLight->animation.brightness; //test, pair "whiter" less bright (maybe /2)  
// //             }            
// //             // Change pCONT_iLight->animation speed
// //             //if(mTime::TimeReached(&flashersettings.function_seq.tSavedNewSpeedUpdate,(random(3,10)*100))){
// //               uint16_t values[8] = {7,7,9,9,7,7,4,4}; //off,on,off
// //               pCONT_iLight->animation.transition.rate_ms = values[flashersettings.function_seq.rate_index++]*1000;
// //               if(flashersettings.function_seq.rate_index>=8) flashersettings.function_seq.rate_index=0;
// //               // holds colour more
// //               //pCONT_iLight->animation.transition.time_ms = direction ? (pCONT_iLight->animation.transition.rate_ms.val/4) : (pCONT_iLight->animation.transition.rate_ms.val); // brightness shift takes 100% of the time (no colour shift)
// //               pCONT_iLight->animation.transition.time_ms = random(0,1) ? (pCONT_iLight->animation.transition.rate_ms.val/4) : (pCONT_iLight->animation.transition.rate_ms.va;); // brightness shift takes 100% of the time (no colour shift)
// //             //}
// //           break;
// //         }
// //       break;
// //       //flasher_start_white_to_Colour: 
// //         // method that will start all leds on one colour (white?) and slowly add colours in with fade in
// //       //flasher_change_saturation_between 0 and 100, "fade of saturation", "fade of brightness"

// }


// // Update struct that shows overview, always sends
uint8_t mRGBAnimator::ConstructJSON_Flasher(uint8_t json_level){

  JsonBuilderI->Start();

//   // root[D_JSON_ONOFF] = pCONT_iLight->light_power_state ? "ON" : "OFF";
//   // root[D_JSON_MODE] = GetAnimationModeName();
//   root[D_JSON_FUNCTION] = GetFlasherFunctionName();
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
//   //   transitionobj[D_JSON_FUNCTION] = GetFlasherFunctionName();

//   JsonObject seq_obj = root.createNestedObject("sequential");
//     seq_obj["rate_index"] = flashersettings.function_seq.rate_index;

// JsonBuilderI->Level_Start("slow_glow");
//   JsonBuilderI->Add("rate_index", flashersettings.function_slo_glo.rate_index);
// JsonBuilderI->Level_End();



//   // Flags and states that are used during one transition and reset when completed
//   // JsonObject overridesobj = root.createNestedObject("transition_overrides"); 
//   //   overridesobj["fRefreshAllPixels"] = pCONT_iLight->animation_override.fRefreshAllPixels;
  
//   // root["ledout.length"] = ledout.length;

//   data_buffer.payload.len = measureJson(root)+1;
//   serializeJson(doc,data_buffer.payload.ctr);

//   return (data_buffer.payload.len>3?1:0);
  return JsonBuilderI->End();
}



#endif // ENABLE_PIXEL_FUNCTION_FLASHER




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

const char* mRGBAnimator::GetAnimationStatusCtr(char* buffer, uint8_t buflen){

  if(pCONT_iLight->animator_controller->IsAnimating()){
    snprintf_P(buffer, buflen, PSTR("Animating"));
    return buffer;
  }
  if(pCONT_iLight->animation.flags.fEnable_Animation){
    // (millis since) + (next event millis)
    int16_t until_next_millis = pCONT_iLight->animation.transition.rate_ms.val-(millis()-pCONT_iLight->animation_changed_millis);
    int16_t secs_until_next_event = until_next_millis/1000;
    // secs_until_next_event/=1000;
    // Serial.println(secs_until_next_event);

    // char float_ctr[10];
    // dtostrf(round(secs_until_next_event),3,1,float_ctr);
    // Serial.println(float_ctr);

    // AddLog_P(LOG_LEVEL_INFO,PSTR("GetAnimationStatusCtr %d %d"),
    //   until_next_millis,
    //   //millis(),pCONT_iLight->animation_changed_millis,pCONT_iLight->animation.transition.rate_ms.val,
    //   secs_until_next_event
    // );  
    
    if(secs_until_next_event>=0){
      snprintf_P(buffer, buflen, PSTR("Enabled (in %d secs)"), secs_until_next_event);//float_ctr);
    }else{
      snprintf_P(buffer, buflen, PSTR("Not currently running"));
      //  sprintf(buffer,PSTR("Not currently running"));//float_ctr);
    }

    // sprintf(ctr,PSTR("Enabled (%d secs)\0"),secs_until_next_event);
    // sprintf(ctr,PSTR("Enabled (123 secs)\0"),secs_until_next_event);
    // Serial.println(ctr);
    return buffer;
  }else{
    return D_CSTRING_ERROR_MESSAGE_CTR;
    // return "Paused";
  }
  return D_CSTRING_ERROR_MESSAGE_CTR;
}


void mRGBAnimator::SetPixelColor(uint16_t indexPixel, RgbTypeColor color_internal)
{

  RgbTypeColor color_hardware = color_internal; //to keep white component if available
  switch (pCONT_iLight->settings.pixel_hardware_color_order_id){
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

// DEBUG_LINE_HERE;
  //AddLog_P(LOG_LEVEL_DEBUG,PSTR("pixel[%d] = %d,%d,%d"),indexPixel,color_hardware.R,color_hardware.G,color_hardware.B);//,color_hardware.W);

  // Output methods here
  // Single: Direct pixel indexing, the default method (i.e., pixel index is exact)
  // Repeated: X pixels repeated after each other (i.e., the same repeated amount, given by a single multiplier red for 10, green for 10) (will require count index)
  // Index Ranges: Using a preset index where pixel_index is shown between ranges (i.e., red for 10 pixels, green for 1 pixel) (passed with a pointer to array, enabling different array for different bit mapping)
  // Using percentage linear blend between "Index Ranges" to allow not only horizontal tree lines, but gradients on each line

  // #ifdef ENABLE_DEVFEATURE_SETPIXELOUTPUT_VARIABLE

  //   if(indexPixel == 0){
  //     setpixel_variable_index_counter = 0; // reset
  //     pCONT_iLight->animation.pixel_group.mapped_array_data.index = 0;
  //   }

  //   if(setpixel_variable_index_counter>50){ return; }

  //   if(pCONT_iLight->animation.pixel_group.flags.fEnabled){

  //     uint8_t pixel_multiplier_count = 0;
  //     switch(pCONT_iLight->animation.pixel_group.flags.multiplier_mode_id){
  //       case PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_UNIFORM_ID:
  //         pixel_multiplier_count = pCONT_iLight->animation.pixel_group.multiplier;
  //         break;
  //       case PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_RANDOM_ID:
  //         pixel_multiplier_count = random(1,pCONT_iLight->animation.pixel_group.multiplier);
  //         break;
  //       case PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID:
  //         if(pCONT_iLight->animation.pixel_group.mapped_array_data.values != nullptr){
  //           // if any match, then jump index of lights forward
  //           uint8_t check_index = pCONT_iLight->animation.pixel_group.mapped_array_data.index;
  //           pixel_multiplier_count = pCONT_iLight->animation.pixel_group.mapped_array_data.values[check_index];
            
  //           //AddLog_P(LOG_LEVEL_WARN, PSTR("PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID %d"),pixel_multiplier_count);
  //           pCONT_iLight->animation.pixel_group.mapped_array_data.index++;
  //           if(pCONT_iLight->animation.pixel_group.mapped_array_data.index >= 15){//pCONT_iLight->animation.pixel_group.mapped_array_data.length){
  //             pCONT_iLight->animation.pixel_group.mapped_array_data.index = 0;
  //           }

  //         }else{
  //           pixel_multiplier_count = pCONT_iLight->animation.pixel_group.multiplier;
  //         }
  //         break;
  //     }

  //     for(uint8_t ii = 0; ii < pixel_multiplier_count; ii++){
  //       if(setpixel_variable_index_counter>50){ return; }
  //       stripbus->SetPixelColor(setpixel_variable_index_counter++,color_hardware);  
  //     }

  //   }else{
  //     stripbus->SetPixelColor(indexPixel,color_hardware);
  //   } // pixel_group.flags.fEnabled

  //   AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("indexPixel=%d,setpixel_variable_index_counterhere=%d"),indexPixel,setpixel_variable_index_counter);

  // #else

    stripbus->SetPixelColor(indexPixel,color_hardware);

  // #endif // ENABLE_DEVFEATURE_SETPIXELOUTPUT_VARIABLE

}

RgbTypeColor mRGBAnimator::GetPixelColor(uint16_t indexPixel)
{
  DEBUG_LINE;
  if(stripbus == nullptr){    
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "stripbus == nullptr"));
    #endif
  }

  // #ifdef USE_DEVFEATURE_PIXEL_OUTPUT_MULTIPLIER
  // indexPixel *= USE_DEVFEATURE_PIXEL_OUTPUT_MULTIPLIER;

  // #endif

  // RgbTypeColor color_hardware = stripbus->GetPixelColor(indexPixel);


  
  // #ifdef ENABLE_DEVFEATURE_SETPIXELOUTPUT_VARIABLE

  //   if(indexPixel == 0){
  //     setpixel_variable_index_counter = 0; // reset
  //     pCONT_iLight->animation.pixel_group.mapped_array_data.index = 0;
  //   }

  //   if(setpixel_variable_index_counter>50){ return; }

  //   if(pCONT_iLight->animation.pixel_group.flags.fEnabled){

  //     uint8_t pixel_multiplier_count = 0;
  //     switch(pCONT_iLight->animation.pixel_group.flags.multiplier_mode_id){
  //       case PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_UNIFORM_ID:
  //         pixel_multiplier_count = pCONT_iLight->animation.pixel_group.multiplier;
  //         break;
  //       case PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_RANDOM_ID:
  //         pixel_multiplier_count = random(1,pCONT_iLight->animation.pixel_group.multiplier);
  //         break;
  //       case PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID:
  //         if(pCONT_iLight->animation.pixel_group.mapped_array_data.values != nullptr){
  //           // if any match, then jump index of lights forward
  //           uint8_t check_index = pCONT_iLight->animation.pixel_group.mapped_array_data.index;
  //           pixel_multiplier_count = pCONT_iLight->animation.pixel_group.mapped_array_data.values[check_index];
            
  //           //AddLog_P(LOG_LEVEL_WARN, PSTR("PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID %d"),pixel_multiplier_count);
  //           pCONT_iLight->animation.pixel_group.mapped_array_data.index++;
  //           if(pCONT_iLight->animation.pixel_group.mapped_array_data.index >= 15){//pCONT_iLight->animation.pixel_group.mapped_array_data.length){
  //             pCONT_iLight->animation.pixel_group.mapped_array_data.index = 0;
  //           }

  //         }else{
  //           pixel_multiplier_count = pCONT_iLight->animation.pixel_group.multiplier;
  //         }
  //         break;
  //     }

  //     for(uint8_t ii = 0; ii < pixel_multiplier_count; ii++){
  //       if(setpixel_variable_index_counter>50){ return; }
  //       stripbus->SetPixelColor(setpixel_variable_index_counter++,color_hardware);  
  //     }

  //   }else{
  //     stripbus->SetPixelColor(indexPixel,color_hardware);
  //   } // pixel_group.flags.fEnabled

  //   AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("indexPixel=%d,setpixel_variable_index_counterhere=%d"),indexPixel,setpixel_variable_index_counter);

  // #else

    RgbTypeColor color_hardware = stripbus->GetPixelColor(indexPixel);

  // #endif

  RgbTypeColor color_internal = color_hardware; // To catch white element if present
  DEBUG_LINE;

  switch (pCONT_iLight->settings.pixel_hardware_color_order_id){
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
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_NOTIFICATIONS_CTR;
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
  mqtthandler_ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_MIXER_CTR;
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


#ifdef ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
  mqtthandler_ptr = &mqtthandler_manual_setpixel;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_MIXER_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETPIXEL_MANUALLY_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mRGBAnimator::ConstructJSON_Manual_SetPixel;  
#endif // ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL



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
    MQTT_HANDLER_MODULE_ANIMATION_TELEPERIOD_ID, MQTT_HANDLER_MODULE_AMBILIGHT_TELEPERIOD_ID,
    MQTT_HANDLER_MODULE_STATE_TELEPERIOD_ID,
    #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS 
      MQTT_HANDLER_MODULE_NOTIFICATION_TELEPERIOD_ID,
    #endif
    #ifdef ENABLE_PIXEL_FUNCTION_FLASHER
      MQTT_HANDLER_MODULE_FLASHER_TELEPERIOD_ID, 
    #endif
    #ifdef ENABLE_PIXEL_FUNCTION_MIXER
      MQTT_HANDLER_MODULE_MIXER_TELEPERIOD_ID,
    #endif
    #ifdef ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
      MQTT_HANDLER_MODULE_MANUAL_SETPIXEL_TELEPERIOD_ID,
    #endif
    MQTT_HANDLER_SETTINGS_ID // Always last, as always included
  };
  
  // Could this be put into a function? to allow me to set things to all in this, or by ID
  struct handler<mRGBAnimator>* mqtthandler_list_ptr[] = {
    &mqtthandler_animation_teleperiod, &mqtthandler_ambilight_teleperiod,
    &mqtthandler_state_teleperiod,
    #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS 
      &mqtthandler_notifications_teleperiod,
    #endif
    #ifdef ENABLE_PIXEL_FUNCTION_FLASHER
      &mqtthandler_flasher_teleperiod, 
    #endif
    #ifdef ENABLE_PIXEL_FUNCTION_MIXER
      &mqtthandler_mixer_teleperiod,
    #endif
    #ifdef ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
      &mqtthandler_manual_setpixel,
    #endif
    &mqtthandler_settings_teleperiod
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, D_MODULE_LIGHTS_ADDRESSABLE_ID,
    mqtthandler_list_ptr, mqtthandler_list_ids,
    sizeof(mqtthandler_list_ptr)/sizeof(mqtthandler_list_ptr[0]),
    mqtt_handler_id
  );

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

  for(uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++ ){
    animation_colours[pixel].DesiredColour.R = mapvalue(animation_colours[pixel].DesiredColour.R, 0, 255, 0, brightness);
    animation_colours[pixel].DesiredColour.G = mapvalue(animation_colours[pixel].DesiredColour.G, 0, 255, 0, brightness);
    animation_colours[pixel].DesiredColour.B = mapvalue(animation_colours[pixel].DesiredColour.B, 0, 255, 0, brightness);
  }

}


RgbcctColor mRGBAnimator::ApplyBrightnesstoDesiredColour(RgbcctColor full_range_colour, uint8_t brightness){

  RgbcctColor colour_adjusted_with_brightness = RgbcctColor(0);
  colour_adjusted_with_brightness.R  = mapvalue(full_range_colour.R,  0,255, 0,brightness);
  colour_adjusted_with_brightness.G  = mapvalue(full_range_colour.G,  0,255, 0,brightness);
  colour_adjusted_with_brightness.B  = mapvalue(full_range_colour.B,  0,255, 0,brightness);
  colour_adjusted_with_brightness.WW = mapvalue(full_range_colour.WW, 0,255, 0,brightness);
  colour_adjusted_with_brightness.WC = mapvalue(full_range_colour.WC, 0,255, 0,brightness);
  return colour_adjusted_with_brightness;

}


RgbcctColor mRGBAnimator::ApplyBrightnesstoDesiredColourWithGamma(RgbcctColor full_range_colour, uint8_t brightness){
  
  uint8_t new_brightness_255 = 0;

// Create new function of ApplyBrightnesstoDesiredColour to do this if statement generically
// if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){

  if(pCONT_iLight->animation.flags.Apply_Upper_And_Lower_Brightness_Randomly_Ranged_To_Palette_Choice){
    new_brightness_255 = random(pCONT_iLight->brtRGB_limits.lower, pCONT_iLight->brtRGB_limits.upper);
  }
  // Default: apply global brightness
  else{
    new_brightness_255 = brightness;
  }

  // AddLog_P(LOG_LEVEL_TEST, PSTR("ledGamma=new_brightness_255=%d - >%d"), 
  //   new_brightness_255, pCONT_iLight->ledGamma(new_brightness_255)
  // );

  #ifdef ENABLE_GAMMA_BRIGHTNESS_ON_DESIRED_COLOUR_GENERATION
  if(pCONT_iLight->animation.flags.use_gamma_for_brightness){
    new_brightness_255 = pCONT_iLight->ledGamma(new_brightness_255);
  }
  #endif // ENABLE_GAMMA_BRIGHTNESS_ON_DESIRED_COLOUR_GENERATION

  return ApplyBrightnesstoDesiredColour(full_range_colour, new_brightness_255);

}



uint16_t mRGBAnimator::SetLEDOutAmountByPercentage(uint8_t percentage){

  strip_size_requiring_update = mapvalue(percentage, 0,100, 0,pCONT_iLight->light_size_count);

// AddLog_P(LOG_LEVEL_TEST, PSTR(DEBUG_INSERT_PAGE_BREAK "SetLEDOutAmountByPercentage = %d"),strip_size_requiring_update);

  return strip_size_requiring_update; // also return the count

}



void mRGBAnimator::RefreshLEDIndexPattern(){

  // AddLog_P(LOG_LEVEL_TEST, PSTR("f::%s"),__FUNCTION__);

  // DEBUG_PRINT_FUNCTION_NAME_TEST;
  // DEBUG_PRINT_FUNCTION_NAME;

  // Somehow this orders need to become flasher... which I will rename pCONT_iLight->animation

  //Serial.printf("animation.transition.order_id=%d\n\r",pCONT_iLight->animation.transition.order_id);

  // Generate lighting pattern
  switch(pCONT_iLight->animation.transition.order_id){
    case TRANSITION_ORDER_INORDER_ID:

      SetLEDOutAmountByPercentage(100);

      for(ledout.index=0;ledout.index<ledout.length;ledout.index++){ 
        ledout.pattern[ledout.index] = ledout.index; 
      }


        // Serial.println("TRANSITION_ORDER_INORDER_ID");
      //for(int jj=0;jj<ledout.length;jj++){ Serial.printf("%d, ",ledout.pattern[jj]); } Serial.println();
// AddLog_Array(LOG_LEVEL_TEST,&ledout.pattern[0],10);

    break;
    case TRANSITION_ORDER_RANDOM_ID:
      switch(transition_order_random_type){
        
        case TRANSITION_ORDER_RANDOM_METHOD_REPEATING_WITH_PERCENTAGE:{ // test case, which includes percentage count to update

// ledout.pattern[

//           uint16_t pixels_to_update_count = map(
//             pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val, 0,100, 0, pCONT_iLight->light_size_count
//           );


      SetLEDOutAmountByPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val);

            // Pick random indexes, from the entire pixel count, but only fill up pattern as needed
            for(ledout.index=0;ledout.index<strip_size_requiring_update;ledout.index++){ 
              ledout.pattern[ledout.index] = random(0,pCONT_iLight->light_size_count); 
            }




// pixels_to_update_count

//           for(uint16_t index=0;index<pixels_to_update_count; index++){ //for as many as I want to update

//           for(ledout.index=0;ledout.index<ledout.length;ledout.index++){ 


//             ledout.pattern[ledout.index] = random(0,ledout.length); 

// strip_size_requiring_update
//           }




      }break;
        case TRANSITION_ORDER_RANDOM_METHOD_REPEATING:     //  Serial.println("TRANSITION_ORDER_INORDER_ID1");
         
          
          #ifdef ENABLE_DEVFEATURE_STDSHUFFLE_PIXEL_RANDOM
            for(ledout.index=0;ledout.index<ledout.length;ledout.index++){ 
              ledout.pattern[ledout.index] = ledout.index;//random(0,ledout.length); 
            }
            std::shuffle(ledout.pattern.begin(),ledout.pattern.end(),std::default_random_engine(analogRead(0)));
          #else 
            for(ledout.index=0;ledout.index<ledout.length;ledout.index++){ 
              ledout.pattern[ledout.index] = random(0,ledout.length); 
            }
          #endif // ENABLE_DEVFEATURE_STDSHUFFLE_PIXEL_RANDOM

      SetLEDOutAmountByPercentage(100);


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

          SetLEDOutAmountByPercentage(100);

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
      // Serial.println();
      // for(int jj=0;jj<ledout.length;jj++){ Serial.printf("%d, ",ledout.pattern[jj]); } Serial.println();

      // shift all values by 1
      for(ledout.index=0;ledout.index<ledout.length/2-1;ledout.index++){ 
        swap(
          ledout.pattern[ledout.length/2-ledout.index],
          ledout.pattern[ledout.length/2+ledout.index]
        );
      }
      // Move into first

      // for(int jj=0;jj<ledout.length;jj++){ Serial.printf("%d, ",ledout.pattern[jj]); } Serial.println();

      // Use existing pattern and rotate it
      // for(ledout.index=0;ledout.index<length_local;ledout.index++){ 
      //   index_pattern[ledout.index] = ledout.index; 
      // }
    break;
  }

} //end function "RefreshLEDIndexPattern"


// Name:UpdateDesiredColourFromPaletteSelected
// Task: 
//
//
void mRGBAnimator::UpdateDesiredColourFromPaletteSelected(void){

  // Update pointer of struct
  pCONT_iLight->SetPaletteListPtrFromID(pCONT_iLight->animation.palette_id);

  // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "UpdateDesiredColourFromPaletteSelected fMapIDs_Type %d"),pCONT_iLight->palettelist.ptr->flags.fMapIDs_Type);// \"%s\""),GetPaletteFriendlyName());
  // AddLog_P(LOG_LEVEL_DEBUG_MORE
  
  /**
   * Handle the retrieval of colours from palettes depending on the palette encoding type
   * */
  switch(pCONT_iLight->palettelist.ptr->flags.fMapIDs_Type){
    // default:
    //   #ifdef ENABLE_LOG_LEVEL_DEBUG
    //   AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "default"));
    //   #endif
    // case MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID:
    default:
    case MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID:
    case MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID:{
    //get colour above

      //apply to positiion below

     // SetLEDOutAmountByPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val);

      //what is this?
      // does this only run if above did not? ie this is default?

      //AddLog_P(LOG_LEVEL_TEST, PSTR("strip_size_requiring_update=%d"),strip_size_requiring_update);
      
      // Other types with random led patterns, or,
      
      switch(pCONT_iLight->animation.transition.order_id){
        case TRANSITION_ORDER_RANDOM_ID:{

          // new transition, so force full update
          if(pCONT_iLight->animation.flags.NewAnimationRequiringCompleteRefresh){
            strip_size_requiring_update = STRIP_SIZE_MAX;
          }
        

          RefreshLEDIndexPattern();
          int16_t pixel_position = -2;
          for(ledout.index=0;ledout.index<strip_size_requiring_update;ledout.index++){

            // For random, desired pixel from map will also be random
            desired_pixel = random(0,pCONT_iLight->GetPixelsInMap(pCONT_iLight->palettelist.ptr));
            
            RgbTypeColor colour = pCONT_iLight->GetColourFromPalette(pCONT_iLight->palettelist.ptr,desired_pixel,&pixel_position);

            // desired_colour[ledout.pattern[ledout.index]] = colour; 
            // AddLog_P(LOG_LEVEL_TEST, PSTR("colour=%d,%d,%d"),
            // colour.R,
            // colour.G,
            // colour.B);

            animation_colours[ledout.pattern[ledout.index]].DesiredColour = colour;

#ifndef ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS
          
            if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
              animation_colours[ledout.pattern[ledout.index]].DesiredColour = ApplyBrightnesstoDesiredColourWithGamma(animation_colours[ledout.pattern[ledout.index]].DesiredColour,pCONT_iLight->getBriRGB());
            }
            #endif // ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS

            // #ifdef ENABLE_DEBUG_MODULE_LIGHTS_ADDRESSABLE
            // AddLog_P(LOG_LEVEL_DEBUG, PSTR("colour=%d,%d,%d brt_set=%d"),
            //   animation_colours[ledout.pattern[ledout.index]].DesiredColour.R,
            //   animation_colours[ledout.pattern[ledout.index]].DesiredColour.G,
            //   animation_colours[ledout.pattern[ledout.index]].DesiredColour.B,
            //   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation
            // );
            // #endif 
          } //end for

        }break;
        case TRANSITION_ORDER_INORDER_ID:{
          // new transition, so force full update
          if(pCONT_iLight->animation.flags.NewAnimationRequiringCompleteRefresh){
            strip_size_requiring_update = STRIP_SIZE_MAX;
          }
          RefreshLEDIndexPattern();
          int16_t pixel_position = -2;
          desired_pixel=0;
          for(ledout.index=0;ledout.index<strip_size_requiring_update;ledout.index++){
            RgbTypeColor colour = pCONT_iLight->GetColourFromPalette(pCONT_iLight->palettelist.ptr,desired_pixel,&pixel_position);

            // desired_colour[ledout.pattern[ledout.index]] = colour; 
            // AddLog_P(LOG_LEVEL_TEST, PSTR("colour=%d,%d,%d"),
            // colour.R,
            // colour.G,
            // colour.B);
            
            // animation_colours[ledout.pattern[ledout.index]].DesiredColour = ApplyBrightnesstoDesiredColour(colour,pCONT_iLight->getBriRGB());
              
            animation_colours[ledout.pattern[ledout.index]].DesiredColour = colour;
           #ifndef ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS
            if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
              animation_colours[ledout.pattern[ledout.index]].DesiredColour = ApplyBrightnesstoDesiredColourWithGamma(animation_colours[ledout.pattern[ledout.index]].DesiredColour,pCONT_iLight->getBriRGB());
            }
            #endif //             ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS

            //   AddLog_P(LOG_LEVEL_TEST, PSTR("colou2=%d,%d,%d"),
            // animation_colours[ledout.pattern[ledout.index]].DesiredColour.R,
            // animation_colours[ledout.pattern[ledout.index]].DesiredColour.G,
            // animation_colours[ledout.pattern[ledout.index]].DesiredColour.B);

            
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


    }break;
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_ID:
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_ID:
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_AND_SETALL_ID:
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_AND_SETALL_ID:{

      // Get active pixels in map
      uint16_t active_pixels_in_map = pCONT_iLight->GetPixelsInMap(pCONT_iLight->palettelist.ptr); //width 2

    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "active_pixels_in_map=%d"),active_pixels_in_map);
    #endif// ENABLE_LOG_LEVEL_INFO

      // Move across map
      int16_t pixel_position = -2;
      for(uint16_t desired_pixel=0;desired_pixel<active_pixels_in_map;desired_pixel++){
        RgbcctColor colour = pCONT_iLight->GetColourFromPalette(pCONT_iLight->palettelist.ptr,desired_pixel,&pixel_position);
        
        #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
        //AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "dp%d, pp%d, %d,%d %d"),desired_pixel, pixel_position,pCONT_iLight->HueF2N(colour.H),pCONT_iLight->SatF2N(colour.S),pCONT_iLight->BrtF2N(colour.B));
        #endif
        
        if(pixel_position>=0){
          // Set output to this "many" colour
          if(pixel_position == 255){
            for(uint16_t temp=0;temp<ledout.length;temp++){ 
              animation_colours[temp].DesiredColour = ApplyBrightnesstoDesiredColour(colour,pCONT_iLight->getBriRGB());
            }            
            #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
            // AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "set ALL %d,%d %d"),pCONT_iLight->HueF2N(colour.H),pCONT_iLight->SatF2N(colour.S),pCONT_iLight->BrtF2N(colour.B));
            #endif
          }else{
            colour.R = 1;colour.R = 2;colour.R = 3;
            // Serial.println("colour.R = 1;colour.R = 2;colour.R = 3;");
            animation_colours[pixel_position].DesiredColour = ApplyBrightnesstoDesiredColour(colour,pCONT_iLight->getBriRGB());
            #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
            //AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "set %d %d,%d %d"), pixel_position,pCONT_iLight->HueF2N(colour.H),pCONT_iLight->SatF2N(colour.S),pCONT_iLight->BrtF2N(colour.B));
            #endif
          }
        }else{          
          #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
          AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "return; //end early %d"),pixel_position);
          #endif
        }

      }
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "return; //end early %d"),pixel_position);
    #endif// ENABLE_LOG_LEVEL_INFO
      break;
      // return;
    }
    break;
    case MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_GRADIENT_ID:
    case MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID:{

      #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID"));
      #endif

      uint16_t start_pixel = 0;
      uint16_t end_pixel = 100;
      RgbTypeColor start_colour = RgbTypeColor(0);
      RgbTypeColor end_colour = RgbTypeColor(0);
      uint16_t desired_pixel = 0;
      int16_t start_pixel_position = -1, end_pixel_position = -1;
      uint8_t pixels_in_map = pCONT_iLight->GetPixelsInMap(pCONT_iLight->palettelist.ptr);

      #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "pixels_in_map %d"),pixels_in_map);
      #endif

      for(uint8_t grad_pair_index=0;grad_pair_index<pixels_in_map;grad_pair_index++){
        
        start_colour = pCONT_iLight->GetColourFromPalette(pCONT_iLight->palettelist.ptr,desired_pixel,  &start_pixel_position);
        end_colour   = pCONT_iLight->GetColourFromPalette(pCONT_iLight->palettelist.ptr,desired_pixel+1,&end_pixel_position);

#ifndef ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS
        start_colour = ApplyBrightnesstoDesiredColour(start_colour,pCONT_iLight->getBriRGB());
        end_colour   = ApplyBrightnesstoDesiredColour(end_colour,pCONT_iLight->getBriRGB());
#endif // ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS

        //#ifdef ENABLE_LOG_LEVEL_DEBUG
        //  AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "s%d,%d %d %d"),pCONT_iLight->HueF2N(start_colour.H),pCONT_iLight->SatF2N(start_colour.S),pCONT_iLight->BrtF2N(start_colour.B),start_pixel_position);
        //  AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "e%d,%d %d %d"),HueF2N(end_colour.H),SatF2N(end_colour.S),BrtF2N(end_colour.B),end_pixel_position);
        //#endif

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
        float progress = 0;
        for(ledout.index=start_pixel_position;ledout.index<end_pixel_position;ledout.index++){
          progress = mSupport::mapfloat(ledout.index,start_pixel_position,end_pixel_position,0,1);
          animation_colours[ledout.index].DesiredColour = RgbTypeColor::LinearBlend(start_colour, end_colour, progress);
        }
        desired_pixel++;
      }

      // Colour applied to entire DesiredColour, leaving now
      break;
      // return; //succcesfully handled

    }break;
    
  }

  #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
  AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "DONE UpdateDesiredColourFromPaletteSelected fMapIDs_Type "));
  #endif  // LOG_LEVEL_DEBUG_MORE

  
  // Handle brightness levels == if not set, use default
  //if(!pCONT_iLight->palettelist.ptr->fOverride_animation_brightness){
  // ApplyBrightnesstoDesiredColour(pCONT_iLight->getBriRGB());
  //}

} //end function UpdateDesiredColourFromPaletteSelected();



// optional fromcolor to merge "FadeToNewColour" and "FadeBetweenColours"
// If unset (as defined in header) the pCONT_iLight->animation will blend from current pixel colours with getpixel
// void mRGBAnimator::FadeToNewColour(RgbTypeColor targetColor, uint16_t _time_to_newcolour,  RgbTypeColor fromcolor){ 
void mRGBAnimator::FadeToNewColour(RgbcctColor targetColor, uint16_t _time_to_newcolour,  RgbcctColor fromcolor){ 

  #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
  AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "FadeToNewColour"));
  #endif
  
    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog_P(LOG_LEVEL_TEST, PSTR("RgbcctColor=%d,%d,%d,%d,%d"),targetColor.R,targetColor.G,targetColor.B,targetColor.WW,targetColor.WC);
    #endif// ENABLE_LOG_LEVEL_INFO
  
  if(NEO_ANIMATION_TIMEBASE == NEO_CENTISECONDS){
    _time_to_newcolour /= 1000;// ms to seconds
  }

  // Overwriting single pCONT_iLight->animation methods, set, then clear
  if(pCONT_iLight->animation_override.time_ms){
    _time_to_newcolour = pCONT_iLight->animation_override.time_ms;
    pCONT_iLight->animation_override.time_ms = 0;//reset overwrite
  }
  
  // AnimEaseFunction easing = NeoEase::CubicIn;  

  // Translation
  // RgbcctColor fromcolor_npb = RgbcctColor(0);
  // if(fromcolor != RgbcctColor(0)){ //? 
  //   fromcolor_npb = fromcolor;
  // }
  RgbcctColor targetColor_npb = RgbcctColor(targetColor.R,targetColor.G,targetColor.B);

  #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
  // AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("fromcolor_npb=%d,%d,%d"),fromcolor_npb.R,fromcolor_npb.G,fromcolor_npb.B);
  // AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("targetColor_npb=%d,%d,%d"),targetColor_npb.R,targetColor_npb.G,targetColor_npb.B);
  #endif // ENABLE_LOG_LEVEL_DEBUG_MORE

  //load start
  for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
    animation_colours[pixel].StartingColor = GetPixelColor(pixel);
    animation_colours[pixel].DesiredColour = targetColor_npb;
    //AddLog_P(LOG_LEVEL_TEST, PSTR("targetColor_npb=%d,%d,%d"),targetColor_npb.R,targetColor_npb.G,targetColor_npb.B);
  }

  
  pCONT_iLight->animator_controller->StartAnimation(0, _time_to_newcolour, [this](const AnimationParam& param){ this->BlendAnimUpdate(param);} );
  
} // END function



// simple blend function
void mRGBAnimator::BlendAnimUpdate(const AnimationParam& param)
{    
  for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
    RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
        animation_colours[pixel].StartingColor,
        animation_colours[pixel].DesiredColour,
        param.progress);
    SetPixelColor(pixel, updatedColor);
  } // END for
}


void mRGBAnimator::StartAnimationAsBlendFromStartingColorToDesiredColor(){ 
  
  #ifdef ENABLE_LOG_LEVEL_DEBUG
  //AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::StartAnimationAsBlendFromStartingColorToDesiredColor"));
  #endif

  pCONT_iLight->animation_changed_millis = millis();

  uint16_t time_tmp = 0;
  // switch(pCONT_iLight->animation.transition.method_id){
  //   default: 
  //   case TRANSITION_METHOD_INSTANT_ID: time_tmp = 0; break;
  //   case TRANSITION_METHOD_BLEND_ID:   
    time_tmp = pCONT_iLight->animation.transition.time_ms.val; 
  //   break;
  // }

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
  for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
    animation_colours[pixel].StartingColor = GetPixelColor(pixel);
    // animation_colours[pixel].DesiredColour = GetPixelColor(pixel); // set elsewhere
    
  
//  AddLog_P(LOG_LEVEL_TEST, PSTR("targetColor_npb=%d,%d,%d %d,%d,%d"),animation_colours[pixel].StartingColor.R,animation_colours[pixel].StartingColor.G,animation_colours[pixel].StartingColor.B,
  // AddLog_P(LOG_LEVEL_TEST, PSTR("DesiredColour=%d,%d,%d"),
  // animation_colours[pixel].DesiredColour.R,animation_colours[pixel].DesiredColour.G,animation_colours[pixel].DesiredColour.B);
  
  
  
  }


  pCONT_iLight->animator_controller->StartAnimation(0, time_tmp, [this](const AnimationParam& param){ this->BlendAnimUpdate(param);} );




} //end function



/**
 * Begin at "StartingColor" at 0% then return to "DesiredColor" at 100%
 * */
void mRGBAnimator::StartAnimationAsSwitchingFromStartingColorToDesiredColor(){ 
  
  #ifdef ENABLE_LOG_LEVEL_DEBUG
  //AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::StartAnimationAsBlendFromStartingColorToDesiredColor"));
  #endif

  pCONT_iLight->animation_changed_millis = millis();

  uint16_t time_tmp = 0;
  // switch(pCONT_iLight->animation.transition.method_id){
  //   default: 
  //   case TRANSITION_METHOD_INSTANT_ID: time_tmp = 0; break;
    // case TRANSITION_METHOD_BLEND_ID:  
     time_tmp = pCONT_iLight->animation.transition.time_ms.val; 
  //    break;
  // }

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

  // Note: Loading of startingcolor and desiredcolor are done elsewhere

  pCONT_iLight->animator_controller->StartAnimation(0, time_tmp, [this](const AnimationParam& param){ this->AnimUpdate_ShowStartingThenDesiredColors(param);} );


} //end function



// simple blend function
void mRGBAnimator::AnimUpdate_ShowStartingThenDesiredColors(const AnimationParam& param)
{    
  // if(param.progress == 0){ // 0%   = Starting
  //   for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
  //     SetPixelColor(pixel, animation_colours[pixel].StartingColor);
  //   }
  // }else
  // if(param.progress == 1){ // 100% = Finishing
  //   for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
  //     SetPixelColor(pixel, animation_colours[pixel].DesiredColour);
  //   }
  // }
}







/**
 * Begin at "StartingColor" at 0% then return to "DesiredColor" at 100%
 * */
void mRGBAnimator::StartAnimation_AsAnimUpdateMemberFunction(){ 
  
  #ifdef ENABLE_LOG_LEVEL_DEBUG
  //AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::StartAnimationAsBlendFromStartingColorToDesiredColor"));
  #endif

  #ifndef ENABLE_DEVFEATURE_FLICKERING_TEST3

  pCONT_iLight->animation_changed_millis = millis();

  uint16_t time_tmp = 0;
  // switch(pCONT_iLight->animation.transition.method_id){
  //   default: 
  //   case TRANSITION_METHOD_INSTANT_ID: time_tmp = 0;

  //   Serial.println("TRANSITION_METHOD_INSTANT_ID");
    
    
    
  //    break;
    // case TRANSITION_METHOD_BLEND_ID:   
    time_tmp = pCONT_iLight->animation.transition.time_ms.val; 
  //   break;
  // }

  // Overwriting single pCONT_iLight->animation methods, set, then clear
  if(pCONT_iLight->animation_override.time_ms){
    time_tmp = pCONT_iLight->animation_override.time_ms;
    pCONT_iLight->animation_override.time_ms = 0;//reset overwrite
  }
  //clear forced once only flags
  if(pCONT_iLight->animation.flags.NewAnimationRequiringCompleteRefresh){
    pCONT_iLight->animation.flags.NewAnimationRequiringCompleteRefresh = false;    
  }
  
    // Serial.printf("TRANSITION_METHOD_INSTANT_ID = %d\n\r",time_tmp);

  if(time_tmp>0){
    if(NEO_ANIMATION_TIMEBASE == NEO_CENTISECONDS){
      time_tmp /= 1000;// ms to seconds
    }
  }

  // Note: Loading of startingcolor and desiredcolor are done elsewhere

  


  // DEBUG_PRINT_FUNCTION_NAME_TEST;
  if(anim_function_callback){
    pCONT_iLight->animator_controller->StartAnimation(0, time_tmp, anim_function_callback );
  }

  #else
    pCONT_iLight->animator_controller->StartAnimation(0, 900, 
      [this](const AnimationParam& param){
        this->AnimUpdateMemberFunction_Tester(param);
      }
    );

  #endif // ENABLE_DEVFEATURE_FLICKERING_TEST3

  // DEBUG_PRINT_FUNCTION_NAME_TEST;

} //end function


mRGBAnimator& mRGBAnimator::setAnimFunctionCallback(ANIM_FUNCTION_SIGNATURE) {
    this->anim_function_callback = anim_function_callback;
    return *this;
}



// simple blend function
void mRGBAnimator::AnimUpdateMemberFunction(const AnimationParam& param)
{    
  // if(param.progress == 0){ // 0%   = Starting
  //   for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
  //     SetPixelColor(pixel, animation_colours[pixel].StartingColor);
  //   }
  // }else
  // if(param.progress == 1){ // 100% = Finishing
  //   for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
  //     SetPixelColor(pixel, animation_colours[pixel].DesiredColour);
  //   }
  // }
}

void mRGBAnimator::AnimUpdateMemberFunction_TraditionalChasing(const AnimationParam& param)
{    

  // As integer so the if statement checks will not fail due to rounding errors
  uint8_t progress_percentage = param.progress*100; 


  // if(progress_percentage == 0){ // 0%   = Starting
  //   for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
  //     SetPixelColor(pixel, RgbTypeColor(255,0,0));//animation_colours[pixel].StartingColor);
  //   }
  // }else
  // if(progress_percentage == 25){ // 100% = Finishing
  //   for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
  //     SetPixelColor(pixel, RgbTypeColor(0,255,0));
  //   }
  // }else
  // if(progress_percentage == 50){ // 100% = Finishing
  //   for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
  //     SetPixelColor(pixel, RgbTypeColor(0,0,255));
  //   }
  // }else
  // if(progress_percentage == 75){ // 100% = Finishing
  //   for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
  //     SetPixelColor(pixel, RgbTypeColor(255,0,255));
  //   }
  // }


    // for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
    //   SetPixelColor(pixel, RgbTypeColor(0,0,0));
    // }


  if(progress_percentage == 0){ // 0%   = Starting
    for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
      switch(pixel%4){
        case 0: SetPixelColor(pixel, RgbTypeColor(255,0,0)); break;
        case 1: SetPixelColor(pixel, RgbTypeColor(0,255,0)); break;
        case 2: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
        case 3: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
      }
      
    }
  }else
  if(progress_percentage == 25){ // 100% = Finishing
    for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
      switch(pixel%4){
        case 0: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
        case 1: SetPixelColor(pixel, RgbTypeColor(0,255,0)); break;
        case 2: SetPixelColor(pixel, RgbTypeColor(0,0,255)); break;
        case 3: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
      }
    }
  }else
  if(progress_percentage == 50){ // 100% = Finishing
    for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
      switch(pixel%4){
        case 0: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
        case 1: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
        case 2: SetPixelColor(pixel, RgbTypeColor(0,0,255)); break;
        case 3: SetPixelColor(pixel, RgbTypeColor(255, 153, 0)); break;
      }
    }
  }else
  if(progress_percentage == 75){ // 100% = Finishing
    for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
      switch(pixel%4){
        case 0: SetPixelColor(pixel, RgbTypeColor(255,0,0)); break;
        case 1: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
        case 2: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
        case 3: SetPixelColor(pixel, RgbTypeColor(255, 153, 0)); break;
      }
    }
  }



  // else
  // if(param.progress == 1){ // 100% = Finishing
  //   for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
  //     SetPixelColor(pixel, RgbTypeColor(255,255,255));
  //   }
  // }

  
    // for (uint16_t pixel = 1; pixel < pCONT_iLight->light_size_count; pixel++){
    //   SetPixelColor(pixel, RgbTypeColor(0,0,0));
    // }



}


void mRGBAnimator::AnimUpdateMemberFunction_TraditionalChasing2(const AnimationParam& param)
{    

  // As integer so the if statement checks will not fail due to rounding errors
  uint8_t progress_percentage = param.progress*100; 

    // for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
    //   SetPixelColor(pixel, RgbTypeColor(0,0,0));
    // }

  if(progress_percentage == 0){ // 0%   = Starting
    for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
      SetPixelColor(pixel, RgbTypeColor(255,0,0));//animation_colours[pixel].StartingColor);
    }
  }else
  if(progress_percentage == 25){ // 100% = Finishing
    for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
      SetPixelColor(pixel, RgbTypeColor(0,255,0));
    }
  }else
  if(progress_percentage == 50){ // 100% = Finishing
    for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
      SetPixelColor(pixel, RgbTypeColor(0,0,255));
    }
  }else
  if(progress_percentage == 75){ // 100% = Finishing
    for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
      SetPixelColor(pixel, RgbTypeColor(255,0,255));
    }
  }




  // if(progress_percentage == 0){ // 0%   = Starting
  //   for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
  //     switch(pixel%4){
  //       case 0: SetPixelColor(pixel, RgbTypeColor(255,0,0)); break;
  //       case 1: SetPixelColor(pixel, RgbTypeColor(0,255,0)); break;
  //       case 2: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
  //       case 3: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
  //     }
      
  //   }
  // }else
  // if(progress_percentage == 25){ // 100% = Finishing
  //   for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
  //     switch(pixel%4){
  //       case 0: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
  //       case 1: SetPixelColor(pixel, RgbTypeColor(0,255,0)); break;
  //       case 2: SetPixelColor(pixel, RgbTypeColor(0,0,255)); break;
  //       case 3: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
  //     }
  //   }
  // }else
  // if(progress_percentage == 50){ // 100% = Finishing
  //   for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
  //     switch(pixel%4){
  //       case 0: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
  //       case 1: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
  //       case 2: SetPixelColor(pixel, RgbTypeColor(0,0,255)); break;
  //       case 3: SetPixelColor(pixel, RgbTypeColor(255, 153, 0)); break;
  //     }
  //   }
  // }else
  // if(progress_percentage == 75){ // 100% = Finishing
  //   for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
  //     switch(pixel%4){
  //       case 0: SetPixelColor(pixel, RgbTypeColor(255,0,0)); break;
  //       case 1: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
  //       case 2: SetPixelColor(pixel, RgbTypeColor(0,0,0)); break;
  //       case 3: SetPixelColor(pixel, RgbTypeColor(255, 153, 0)); break;
  //     }
  //   }
  // }



  // else
  // if(param.progress == 1){ // 100% = Finishing
  //   for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
  //     SetPixelColor(pixel, RgbTypeColor(255,255,255));
  //   }
  // }

  
    // for (uint16_t pixel = 1; pixel < pCONT_iLight->light_size_count; pixel++){
    //   SetPixelColor(pixel, RgbTypeColor(0,0,0));
    // }



}




bool mRGBAnimator::OverwriteUpdateDesiredColourIfMultiplierIsEnabled(){

  //Use starting pixel as temporary buffer, record desired as already set
  for (uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
    animation_colours[pixel].StartingColor = animation_colours[pixel].DesiredColour;
  } // END for


  for(uint16_t indexPixel=0; indexPixel<pCONT_iLight->light_size_count; indexPixel++){
    
    uint8_t pixel_multiplier_count = 0;
    if(indexPixel == 0){
      setpixel_variable_index_counter = 0; // reset
      pixel_group.mapped_array_data.index = 0;
    }

    if(setpixel_variable_index_counter>pCONT_iLight->light_size_count-1){ return true; }

    if(pixel_group.flags.fEnabled){

      switch(pixel_group.flags.multiplier_mode_id){
        // default:
        case PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_UNIFORM_ID:
          pixel_multiplier_count = pixel_group.multiplier;
          break;
        case PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_RANDOM_ID:
          pixel_multiplier_count = random(1,pixel_group.multiplier);
          break;
        case PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID:
          if(pixel_group.mapped_array_data.values != nullptr){
            // if any match, then jump index of lights forward
            pixel_multiplier_count = pixel_group.mapped_array_data.values[pixel_group.mapped_array_data.index];
            //AddLog_P(LOG_LEVEL_WARN, PSTR("PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID %d %d %d"),pixel_multiplier_count,pixel_group.mapped_array_data.index,pixel_group.mapped_array_data.length);
            if(pixel_group.mapped_array_data.index++ >= pixel_group.mapped_array_data.length-1){
              pixel_group.mapped_array_data.index = 0;
            }
          }else{
            pixel_multiplier_count = 1;//pixel_group.multiplier;
          }
          break;
      }

      for(uint8_t ii = 0; ii < pixel_multiplier_count; ii++){
        animation_colours[setpixel_variable_index_counter++].DesiredColour = animation_colours[indexPixel].StartingColor;
        if(setpixel_variable_index_counter>pCONT_iLight->light_size_count-1){ return true; }
      }

    }
    else{
      animation_colours[indexPixel].DesiredColour = animation_colours[indexPixel].StartingColor;
    } // pixel_group.flags.fEnabled

    //AddLog_P(LOG_LEVEL_TEST, PSTR("indexPixel=%d,setpixel_variable_index_counterhere=%d %d"),indexPixel,setpixel_variable_index_counter,pixel_multiplier_count);

  }

  return true; // returns if this function changed the desiredcolour

}








void mRGBAnimator::TurnLEDsOff(){
  //stripbus->ClearTo(0);
  for(uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
    SetPixelColor(pixel, 0);
  }
  stripbus->Show();
}


void mRGBAnimator::SetPixelColor_All(RgbcctColor colour){
  //stripbus->ClearTo(0);
  for(uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
    SetPixelColor(pixel, colour);
  }
  stripbus->Show();
}



// void mRGBAnimator::SetRGBwithNeoPixel(){
//   for(uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
//     // SetPixelColor(pixel,RgbTypeColor(desired_colour[pixel]));//.H/360.0f, desired_colour[pixel].S/100.0f, desired_colour[pixel].B/100.0f));
//   }
//   stripbus->Show();
// }

// void mRGBAnimator::SetRGBwithNeoPixelImageTest(){

  
//     // for (auto copies = 0; copies < 3; copies++) {
//     //   image.Blt(stripbus, copies * image.PixelCount());
      
//     // }


//   for(uint16_t pixel = 0; pixel < pCONT_iLight->light_size_count; pixel++){
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

  // int numpixels = pCONT_iLight->light_size_count;
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


// Single calls that configure Tasker into correct mode
// ie Set Autumn configures preset mode
// void mRGBAnimator::SetMode_UpdateColour(uint8_t colour){
//   pCONT_iLight->animation.palette_id = colour;
//   pCONT_iLight->animation.mode_id = ANIMATION_MODE_PRESETS_ID;
// }

// H ue360toFloat

// make these static and global supports, requiring no object ? inline??




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
  // DEBUG_PRINT_FUNCTION_NAME_TEST;
  // if(stripbus->CanShow()){
  //     stripbus->Show();
  //   }

// stripbus->IsDirty
    
      // StripUpdate();
  // DEBUG_PRINT_FUNCTION_NAME_TEST;
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
      if(pCONT_iLight->animation.flags.fRunning){ // Was running
        #ifdef ENABLE_LOG_LEVEL_DEBUG
        AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "Animation Finished")); 
        #endif
        fAnyLEDsOnOffCount = 0;
        for(int i=0;i<pCONT_iLight->light_size_count;i++){ 
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
  //   if(mTime::TimeReached(&tSavedBlocking_ForceAnimateToComplete,5000)) {blocking_force_animate_to_complete = false; break; }//exit blocking lo op
  // }
  }//pCONT_iLight->animation.flags.fEnable_Animation


  if(stripbus->IsDirty()){
    if(stripbus->CanShow()){
      stripbus->Show();
    }
  }



  // #ifndef ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT
  // // scene.colour.B = pCONT_iLight->animation.brightness; // pCONT_iLight->animation.brightness is master
  // // move to have tasmota way to update brightness stored
  // #endif //ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT

  // control relay power
  //(pCONT_iLight->light_power_state)
    if(pCONT_iLight->light_power_state!=pCONT_iLight->light_power_Saved){
      pCONT_iLight->light_power_Saved = pCONT_iLight->light_power_state;
      // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "pCONT_iLight->light_power_Saved"));
      //pCONT->mry->CommandSet_Relay_Power(0,pCONT_iLight->light_power_state);
    }

    //AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "file%sline%d"),__FILE__,__LINE__);
    DEBUG_LINE;

  if(mTime::TimeReached(&tSavedCheckAnyLEDsOnOff,1000)){
    fAnyLEDsOnOffCount = 0;
    for(int i=0;i<pCONT_iLight->light_size_count;i++){
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
    //   //pCONT->mry->CommandSet_Relay_Power(0,fAnyLEDsOnOffCount>0?1:0);
    // #endif
  }
  DEBUG_LINE;
// 

}










/*******************************************************************************************************************
********************************************************************************************************************
************MQTT**************************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/






// Final output, but check for power limit
void mRGBAnimator::StripUpdate(){

  // STRIP_SIZE_REPEAT_MAX

  // if(settings.strip_size_repeat_animation)

  // Calculate output pixel size
  // uint16_t pixel_output_length = pCONT_iLight->light_size_count;
  // if(pCONT_iLight->light_size_count>STRIP_SIZE_MAX){
  //   pixel_output_length = STRIP_SIZE_MAX;
  // }
  // // Replicate SetPixel for repeated output
  // #ifdef STRIP_REPEAT_OUTPUT_MAX
  // pixel_output_length = STRIP_REPEAT_OUTPUT_MAX;  
  // #endif // STRIP_REPEAT_OUTPUT_MAX



// AddLog_P(LOG_LEVEL_WARN, PSTR("Disabled Replicate SetPixel for repeated output"));
  // // Replicate SetPixel for repeated output
  // #ifdef STRIP_REPEAT_OUTPUT_MAX
  // int pixels_existing_index = 0;
  // for(int i=0;i<STRIP_REPEAT_OUTPUT_MAX;i++){
  //   SetPixelColor(i,GetPixelColor(pixels_existing_index++));
  //   if(pixels_existing_index>=STRIP_SIZE_MAX){ pixels_existing_index = 0;}
  // }
  // #endif // STRIP_REPEAT_OUTPUT_MAX



  
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
    c = desired_colour[ledout.index];, 19
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






uint8_t mRGBAnimator::ConstructJSON_Settings(uint8_t json_method){

  D_DATA_BUFFER_CLEAR();
  pCONT_sup->WriteBuffer_P(data_buffer.payload.ctr,
    PSTR("{"
      "\"" "ledout"       "\":%d,"
      "\"" "pCONT_iLight->light_size_count"        "\":%d,"
      "\"" "STRIP_SIZE_MAX"          "\":%d,"
      "\"" "animator_strip_size"  "\":%d,"
      "\"" "ANIMATOR_SIZE_MAX"         "\":%d"
    "}"),
    ledout.length,
    pCONT_iLight->light_size_count,
    STRIP_SIZE_MAX,
    animator_strip_size,
    ANIMATOR_SIZE_MAX
  );

  return strlen(data_buffer.payload.ctr) ? 1 : 0;

}






uint8_t mRGBAnimator::ConstructJSON_Animation(uint8_t json_method){

  #ifdef ENABLE_LOG_LEVEL_DEBUG
  // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::ConstructJSON_Animation"));
  #endif

  // StaticJsonDocument<600> doc;
  // JsonObject root = doc.to<JsonObject>();

  // root[D_JSON_ONOFF] = pCONT_iLight->light_power_state ? "ON" : "OFF";
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

  // data_buffer.payload.len = measureJson(root)+1;
  // serializeJson(doc,data_buffer.payload.ctr);

  return 0;

}



uint8_t mRGBAnimator::ConstructJSON_Ambilight(uint8_t json_level){
  // Awaiting total redesign
  
  #ifdef ENABLE_LOG_LEVEL_DEBUG
  AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "f::ConstructJSON_Ambilight"));
  #endif

  
  // DynamicJsonDocument doc(1500);
  // JsonObject root = doc.to<JsonObject>();

  // root["ambilight"] =0;//ledout.length;

  // data_buffer.payload.len = measureJson(root)+1;
  // serializeJson(doc,data_buffer.payload.ctr);

  return 0;//data_buffer.payload.len;

}





uint8_t mRGBAnimator::ConstructJSON_State(uint8_t json_level){

  uint8_t numpixels = pCONT_iLight->light_size_count<100?pCONT_iLight->light_size_count:100;
  RgbTypeColor c;
  
  JsonBuilderI->Start();  
    JsonBuilderI->Add_P(PM_JSON_SIZE, pCONT_iLight->light_size_count);
    JsonBuilderI->Array_Start("rgb");
    for(int i=0;i<numpixels;i++){
      RgbTypeColor c = GetPixelColor(i);
      JsonBuilderI->Add_FV(PSTR("%02X%02X%02X"),c.R,c.G,c.B);
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

// void mRGBAnimator::init_NotificationPanel(){

//   for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){
//     notif.flags.fForcePanelUpdate = true; //clear presets
//     notif.pixel[i].mode = NOTIF_MODE_OFF_ID;
//     notif.pixel[i].colour.R = map(i,0,STRIP_NOTIFICATION_SIZE,0,255);
//     notif.pixel[i].colour.G = map(i,0,STRIP_NOTIFICATION_SIZE,255,0);
//     // notif.pixel[i].colour.S = 1;
//     // notif.pixel[i].colour.B = 1;
//     // #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
//     // notif.pixel[i].colourWhite = 0;
//     // #endif
//   }
//   notif.flags.fWhenOnlyWhiteExistsSetBrightnessOfHSBtoZero = true;

// } //end "init_NotificationPanel"

// //void mRGBAnimator::SubTask_HeatingPanel(){
// void mRGBAnimator::SubTask_NotificationPanel(){

//   if(mTime::TimeReached(&notif.tSaved.ForceUpdate,120000)||(notif.flags.fForcePanelUpdate)){
//     notif.flags.fForcePanelUpdate = true;
//   }

//   //Enable a flag to auto turn everything off if inactive for 2 minutes
//   if(notif.flags.fEnableTimeoutAll){
//     if(mTime::TimeReached(&notif.tSaved.TimeoutCounter,30000)){  
//       #ifdef ENABLE_LOG_LEVEL_INFO
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "Seconds until notif timeout: [%d/%d]"),(millis()-notif.tSaved.TimeoutCounter)/1000,120);
//       #endif
//     }
//     if(mTime::TimeReached(&notif.tSaved.Timeout,120000)){
//         #ifdef ENABLE_LOG_LEVEL_INFO
//       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO "tNotifPanelTimeout"));
//       #endif
//       TurnLEDsOff();
//     }
//   }

//   char buffer[50];

//   for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){
//     if(
//         mTime::TimeReached(&notif.pixel[i].tSavedUpdate,notif.pixel[i].tRateUpdate)
//         ||(notif.flags.fForcePanelUpdate)
//       ){ notif.flags.fForcePanelUpdate = false;
      
//         #ifdef ENABLE_LOG_LEVEL_INFO
//         AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "Notif mode %d:%s"),i,GetNotificationModeNamebyID(notif.pixel[i].mode, buffer));
//         #endif
      
//       RgbcctColor colour = RgbcctColor(0);

//       switch(notif.pixel[i].mode){
//         default:
//         case NOTIF_MODE_OFF_ID:
//         case NOTIF_MODE_STATIC_OFF_ID:
//           colour = RgbcctColor(0);
//         break;
//         case NOTIF_MODE_STATIC_ON_ID:{
          
//           colour = notif.pixel[i].colour;

//           // colour = HsbColor(notif.pixel[i].colour);
//           // #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
//           //   colour.W = notif.pixel[i].colourWhite;
//           // #endif


//         }break;
//         case NOTIF_MODE_FADE_ON_ID: //slow transition to static ON
//           if(notif.pixel[i].transition_progess<100){
//             notif.pixel[i].transition_progess++;
//           }else{
//             notif.pixel[i].mode = NOTIF_MODE_STATIC_ON_ID;
//           }
//           notif.pixel[i].tRateUpdate = (notif.pixel[i].period_ms/100);
//           #ifdef ENABLE_LOG_LEVEL_DEBUG
//           AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "FADING on progress [%d]"),notif.pixel[i].transition_progess); 
//           #endif
//           notif.pixel[i].colour.B = BrtN2F(notif.pixel[i].transition_progess);

//           Change_RgbcctColor_Hue

          
//           colour = HsbColor(notif.pixel[i].colour);
//           #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
//             colour.W = map(notif.pixel[i].transition_progess,0,100,0,notif.pixel[i].colourWhite);
//           #endif
//         break;
//         case NOTIF_MODE_FADE_OFF_ID:{
//           if(notif.pixel[i].transition_progess<100){
//             notif.pixel[i].transition_progess++;
//           }else{
//             notif.pixel[i].mode = NOTIF_MODE_STATIC_OFF_ID;
//           }
//           notif.pixel[i].tRateUpdate = (notif.pixel[i].period_ms/100);
//           #ifdef ENABLE_LOG_LEVEL_DEBUG
//           //AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "FADING off progress [%d]"),notif.pixel[i].transition_progess); 
//           #endif
//           notif.pixel[i].colour.B = (100-notif.pixel[i].transition_progess)/100.0f;
//           colour = HsbColor(notif.pixel[i].colour);
//           #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
//             colour.W = map(notif.pixel[i].transition_progess,0,100,notif.pixel[i].colourWhite,0);
//           #endif  
//         }break;
//         case NOTIF_MODE_BLINKING_OFF_ID:
//           colour = RgbcctColor(0);
//           notif.pixel[i].mode = NOTIF_MODE_BLINKING_ON_ID;
//           notif.pixel[i].tRateUpdate = (notif.pixel[i].period_ms/2);
//         break;
//         case NOTIF_MODE_BLINKING_ON_ID:

//           colour = notif.pixel[i].colour;

//           notif.pixel[i].mode = NOTIF_MODE_BLINKING_OFF_ID;
//           notif.pixel[i].tRateUpdate = (notif.pixel[i].period_ms/2);
//           break;
//         case NOTIF_MODE_PULSING_OFF_ID:
//           if(notif.pixel[i].transition_progess<100){
//             notif.pixel[i].transition_progess++;
//           }else{
//             notif.pixel[i].mode = NOTIF_MODE_PULSING_ON_ID;
//           }
//           notif.pixel[i].tRateUpdate = (notif.pixel[i].period_ms/200);
//           #ifdef ENABLE_LOG_LEVEL_DEBUG
//           AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "PULSING progress [%d]"),notif.pixel[i].transition_progess); 
//           #endif
//           notif.pixel[i].colour.B = notif.pixel[i].transition_progess/100.0f;
//           colour = notif.pixel[i].colour;
//         break;
//         case NOTIF_MODE_PULSING_ON_ID:
//           if(notif.pixel[i].transition_progess>0){
//             notif.pixel[i].transition_progess--;
//           }else{
//             notif.pixel[i].mode = NOTIF_MODE_PULSING_OFF_ID;
//           }
//           notif.pixel[i].tRateUpdate = (notif.pixel[i].period_ms/200);
//           #ifdef ENABLE_LOG_LEVEL_DEBUG
//           AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "PULSING progress [%d]"),notif.pixel[i].transition_progess); 
//           #endif
//           notif.pixel[i].colour.B = notif.pixel[i].transition_progess/100.0f;
//           colour = notif.pixel[i].colour;            
//         break;
//       }
//       SetPixelColor(i,colour);    
//       notif.flags.fShowPanelUpdate = true;
//     } //end if
//   } //end timer check

//   //Auto turn off
//   if(mTime::TimeReached(&notif.tSaved.AutoOff,1000)){// if 1 second past
//     for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ //check all
//       #ifdef ENABLE_LOG_LEVEL_DEBUG
//       AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "Notif tSaved.AutoOff [%d]"),notif.pixel[i].auto_time_off_secs);
//       #endif
//       if(notif.pixel[i].auto_time_off_secs==1){ //if =1 then turn off and clear to 0
//         SetPixelColor(i,0);
//         notif.pixel[i].auto_time_off_secs = 0;
//         notif.pixel[i].mode = NOTIF_MODE_OFF_ID;
//         #ifdef ENABLE_LOG_LEVEL_INFO
//         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "Notif tSaved.AutoOff to OFF[%d]"),notif.pixel[i].auto_time_off_secs);
//         #endif
//       }else
//       if(notif.pixel[i].auto_time_off_secs>1){ //if =1 then turn off and clear to 0
//         notif.pixel[i].auto_time_off_secs--; //decrease
//       }
//     }// END for
//   }

//   // Update
//   if(notif.flags.fShowPanelUpdate){notif.flags.fShowPanelUpdate=false;
//     stripbus->Show();
//     notif.tSaved.ForceUpdate = millis(); // so we dont have flasher clashes
//   }

// } // END FUNCTION

// /** Sets to control per pixel for notification purposes. Examples of setting:
//  * {"pixel_num":[0],"hue":[0],"brt":[0],  "flasher_ms":[0]  ,"pulser_ms":[1]}
//  * {"pixel_num":[0,1],"hue":[0],"brt":[0],"flasher_secs":[0],"pulser_secs":[1]}
//  * {"pixel_num":0,"hue":[0],"brt":[0],    D_JSON_FLASHER:[0]}
//  * {"pixel_num":"all","hue":[0],"brt":[0],D_JSON_FLASHER:[0]}
//  * 
//  * 
//  * */
// void mRGBAnimator::parsesub_NotificationPanel(JsonObjectConst obj){ 

//    // Create local dereferenced variable
//   // JsonObject obj = (*_obj); 

//   struct PARSED{
//     // pixel index being targetted
//     struct PIXELNUMS{
//       uint8_t val[STRIP_NOTIFICATION_SIZE];
//       uint8_t found_idx=0;
//     }pixelnum;
//     // Setting as Hsb directly (sent as either rgb or hsb but stored as hsb)
//     struct COL{
//       HsbColor val[STRIP_NOTIFICATION_SIZE];
//       uint8_t found_idx = 0;
//     }hsb;
//     // Hues and brightness with sat 100
//     struct HUES{
//       uint16_t val[STRIP_NOTIFICATION_SIZE];
//       uint8_t found_idx=0;
//     }hue;
//     struct SATURATION{
//       uint8_t val[STRIP_NOTIFICATION_SIZE];
//       uint8_t found_idx=0;
//     }sat;
//     struct BRIGHTNESS{
//       uint8_t val[STRIP_NOTIFICATION_SIZE];
//       uint8_t found_idx=0;
//     }brt;
//     #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
//     struct RGB_WHITE{
//       uint8_t val[STRIP_NOTIFICATION_SIZE];
//       uint8_t found_idx=0;
//     }white;
//     #endif
//     // Set blink mode with val being period
//     struct FLASHER{
//       uint16_t val[STRIP_NOTIFICATION_SIZE];
//       uint8_t found_idx=0;
//     }flasher;
//     // Set pulse mode with val being period
//     struct PULSER{
//       uint16_t val[STRIP_NOTIFICATION_SIZE];
//       uint8_t found_idx=0;
//     }pulser;
//     // Set fade on/off to gradually change between static modes
//     struct FADER{
//       uint16_t val[STRIP_NOTIFICATION_SIZE];
//       uint8_t found_idx=0;
//     }fade;
//     // Set timer until led automatically turns off
//     struct TIMEON_MS{ 
//       uint16_t val[STRIP_NOTIFICATION_SIZE];
//       uint8_t found_idx=0;
//     }timeon_ms;
//   }parsed;

//   Serial.printf("parsed struct size = %d", sizeof(parsed));

//   memset(&parsed,0,sizeof(parsed)); // clear parsing struct
  
  

//   // StaticJsonDocument<300> doc;
//   // DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
//   // JsonObject obj = doc.as<JsonObject>();

//   /**************************************************************************************************
//   PIXELNUM: formats "all",1,[1] (string,int,array of ints)
//   ***************************************************************************************************/

//   // Check pixel num exists (exit if it doesn't)
//   if(!obj[D_JSON_PIXELNUM].isNull()){
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS  " " D_PARSING_MATCHED D_JSON_PIXELNUM));  
//     if(obj[D_JSON_PIXELNUM].is<JsonArray>()){   
//       JsonArrayConst array = obj[D_JSON_PIXELNUM];
//       for(JsonVariantConst v : array) {
//         int val = v.as<int>();
//         parsed.pixelnum.val[parsed.pixelnum.found_idx++] = val;
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PIXELNUM " JsonArray " " [i%d:v%d]"),parsed.pixelnum.found_idx-1,val);          
//       }
//       data_buffer.isserviced++;
//     }else
//     if(obj[D_JSON_PIXELNUM].is<const char*>()){
//       const char* name = obj[D_JSON_PIXELNUM];
//       if(strstr(name,"all")){
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PIXELNUM " const char* " "all"));     
//         for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ parsed.pixelnum.val[parsed.pixelnum.found_idx++] = i; }
//       }
//       data_buffer.isserviced++;
//     }else 
//     if(obj[D_JSON_PIXELNUM].is<int>()){
//       int val = obj[D_JSON_PIXELNUM];
//       parsed.pixelnum.val[parsed.pixelnum.found_idx++] = val;
//       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PIXELNUM " int" " [i%d:v%d]"),parsed.pixelnum.found_idx-1,val);    
//       data_buffer.isserviced++;
//     }else{
//       AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PIXELNUM "Not Found"));
//     }
//   } //end pixelnum
  
//   /**************************************************************************************************
//   COLOUR: formats "RRGGBB" (string,array of strings)
//   ***************************************************************************************************/
// //rgbw? either or scenario
//   if(!obj[D_JSON_RGB].isNull()){
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_RGB)); 
//     #endif
//     uint32_t colour32bit;  
//     // Arrays
//     if(obj[D_JSON_RGB].is<JsonArray>()){ 
//       JsonArrayConst array = obj[D_JSON_RGB];
//       for(JsonVariantConst v : array) {
//         const char* val = v.as<const char*>();
//         if(val[0]=='#'){ colour32bit = (long) strtol( &val[1], NULL, 16); }else{ colour32bit = (long) strtol( &val[0], NULL, 16); }
//         parsed.hsb.val[parsed.hue.found_idx++] = HsbColor(RgbColor((colour32bit>>16),(colour32bit>>8&0xFF),(colour32bit&0xFF)));
//         #ifdef ENABLE_LOG_LEVEL_INFO
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_RGB " JsonArray " " [i%d:v%s]"),parsed.hsb.found_idx-1,val);    
//         #endif
//       }
//       data_buffer.isserviced++;
//     }else
//     if(obj[D_JSON_RGB].is<const char*>()){
//       const char* val = obj[D_JSON_RGB];
//       if(val[0]=='#'){ colour32bit = (long) strtol( &val[1], NULL, 16); }else{ colour32bit = (long) strtol( &val[0], NULL, 16); }
//       parsed.hsb.val[parsed.hue.found_idx++] = HsbColor(RgbColor((colour32bit>>16),(colour32bit>>8&0xFF),(colour32bit&0xFF)));
//       #ifdef ENABLE_LOG_LEVEL_INFO
//       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_RGB " JsonArray " " [i%d:v%s]"),parsed.hsb.found_idx-1,val);    
//       #endif
//       data_buffer.isserviced++;
//     }else{
//       #ifdef ENABLE_LOG_LEVEL_INFO
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_RGB "Not Found"));
//       #endif
//     }
//   } //end hue


//   /**************************************************************************************************
//   HUE: formats "white/green/blue",1,[1] (string,int,array of ints)
//   ***************************************************************************************************/

//   // Check pixel num exists (exit if it doesn't)
//   if(!obj[D_JSON_HUE].isNull()){
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_HUE));  
//     // Arrays
//     if(obj[D_JSON_HUE].is<JsonArray>()){   
//       JsonArrayConst array = obj[D_JSON_HUE];
//       for(JsonVariantConst v : array) {
//         int val = v.as<int>();
//         parsed.hue.val[parsed.hue.found_idx++] = val;
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_HUE " JsonArray " " [i%d:v%d]"),parsed.hue.found_idx-1,val);    
//       }
//       data_buffer.isserviced++;
//     }else
//     if(obj[D_JSON_HUE].is<const char*>()){
//       const char* colour = obj[D_JSON_HUE];
//       // FUTURE : Add a get hue by colour name function
//       // if(strstr(name,"all")){
//       //   AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS D_JSON_PIXELNUM " const char* " "all"));     
//       //   for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ parsed.pixelnum.val[parsed.pixelnum.found_idx++] = i; }
//       // }
//       data_buffer.isserviced++;
//     }else 
//     if(obj[D_JSON_HUE].is<int>()){
//       int val = obj[D_JSON_HUE];
//       parsed.hue.val[parsed.hue.found_idx++] = val;
//       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_HUE " int" " [i%d:v%d]"),parsed.hue.found_idx-1,val);    
//       data_buffer.isserviced++;
//     }else{
//       #ifdef ENABLE_LOG_LEVEL_INFO
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_HUE "Not Found"));
//       #endif
//     }
//   } //end hue
  

//   /**************************************************************************************************
//   SAT: formats 1,[1] (int,array of ints)
//   ***************************************************************************************************/

//   if(!obj[D_JSON_SAT].isNull()){
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_SAT));  
//     // Arrays
//     if(obj[D_JSON_SAT].is<JsonArray>()){   
//       JsonArrayConst array = obj[D_JSON_SAT];
//       for(JsonVariantConst v : array) {
//         int val = v.as<int>();
//         parsed.sat.val[parsed.sat.found_idx++] = val;
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_BRT " JsonArray " " [i%d:v%d]"),parsed.sat.found_idx-1,val);    
//       }
//       data_buffer.isserviced++;
//     }else
//     if(obj[D_JSON_BRT].is<const char*>()){
//       const char* name = obj[D_JSON_BRT];
//       // FUTURE : Add a get brt by name function
//       // if(strstr(name,"all")){
//       //   AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS D_JSON_PIXELNUM " const char* " "all"));     
//       //   for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ parsed.pixelnum.val[parsed.pixelnum.found_idx++] = i; }
//       // }
//       data_buffer.isserviced++;
//     }else 
//     if(obj[D_JSON_SAT].is<int>()){
//       int val = obj[D_JSON_SAT];
//       parsed.sat.val[parsed.sat.found_idx++] = val;
//       #ifdef ENABLE_LOG_LEVEL_INFO
//       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_SAT " int" " [i%d:v%d]"),parsed.sat.found_idx-1,val);    
//       #endif
//       data_buffer.isserviced++;
//     }else{
//       #ifdef ENABLE_LOG_LEVEL_INFO
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_SAT "Not Found"));
//       #endif
//     }
//   } //end brt
  
//   /**************************************************************************************************
//   BRT: formats "low/med/high",1,[1] (string,int,array of ints)
//   ***************************************************************************************************/

//   if(!obj[D_JSON_BRT].isNull()){
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_BRT));  
//     // Arrays
//     if(obj[D_JSON_BRT].is<JsonArray>()){   
//       JsonArrayConst array = obj[D_JSON_BRT];
//       for(JsonVariantConst v : array) {
//         int val = v.as<int>();
//         parsed.brt.val[parsed.brt.found_idx++] = val;
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_BRT " JsonArray " " [i%d:v%d]"),parsed.brt.found_idx-1,val);    
//       }
//       data_buffer.isserviced++;
//     }else
//     if(obj[D_JSON_BRT].is<const char*>()){
//       const char* name = obj[D_JSON_BRT];
//       // FUTURE : Add a get brt by name function
//       // if(strstr(name,"all")){
//       //   AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS D_JSON_PIXELNUM " const char* " "all"));     
//       //   for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ parsed.pixelnum.val[parsed.pixelnum.found_idx++] = i; }
//       // }
//       data_buffer.isserviced++;
//     }else 
//     if(obj[D_JSON_BRT].is<int>()){
//       int val = obj[D_JSON_BRT];
//       parsed.brt.val[parsed.brt.found_idx++] = val;
//       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_BRT " int" " [i%d:v%d]"),parsed.brt.found_idx-1,val);    
//       data_buffer.isserviced++;
//     }else{
//       #ifdef ENABLE_LOG_LEVEL_INFO
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_BRT "Not Found"));
//       #endif
//     }
//   } //end brt
  
//   #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
//   /**************************************************************************************************
//   WHITE of rgbW: formats "low/med/high",1,[1] (string,int,array of ints)
//   ***************************************************************************************************/

//   if(!obj[D_JSON_WHITE].isNull()){
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_WHITE));  
//     // Arrays
//     if(obj[D_JSON_WHITE].is<JsonArray>()){   
//       JsonArrayConst array = obj[D_JSON_WHITE];
//       for(JsonVariantConst v : array) {
//         int val = v.as<int>();
//         // Serial.println(parsed.white.found_idx);
//         parsed.white.val[parsed.white.found_idx++] = val;//map(val,0,100,0,255); //only 0-100 accepted
//         // Serial.println(parsed.white.found_idx);
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_WHITE " JsonArray " " [i%d:v%d]"),parsed.white.found_idx-1,val);    
//       }
//       data_buffer.isserviced++;
//     }else
//     if(obj[D_JSON_WHITE].is<const char*>()){
//       const char* name = obj[D_JSON_WHITE];
//       // FUTURE : Add a get brt by name function
//       // if(strstr(name,"all")){
//       //   AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS D_JSON_PIXELNUM " const char* " "all"));     
//       //   for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ parsed.pixelnum.val[parsed.pixelnum.found_idx++] = i; }
//       // }
//       data_buffer.isserviced++;
//     }else 
//     if(obj[D_JSON_WHITE].is<int>()){
//       int val = obj[D_JSON_WHITE];
//       parsed.white.val[parsed.white.found_idx++] = val;
//       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_WHITE " int" " [i%d:v%d]"),parsed.white.found_idx-1,val);    
//       data_buffer.isserviced++;
//     }else{
//       #ifdef ENABLE_LOG_LEVEL_INFO
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_WHITE "Not Found"));
//       #endif
//     }
//   } //end brt
//   #endif
  

//   /**************************************************************************************************
//   FLASHER: formats "slow/fast",1,[1] (string,int,array of ints)
//   ***************************************************************************************************/

//   if(!obj[D_JSON_FLASHER].isNull()){
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_FLASHER));  
//     // Arrays
//     if(obj[D_JSON_FLASHER].is<JsonArray>()){   
//       JsonArrayConst array = obj[D_JSON_FLASHER];
//       for(JsonVariantConst v : array) {
//         int val = v.as<int>();
//         parsed.flasher.val[parsed.flasher.found_idx++] = val;
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_FLASHER " JsonArray " " [i%d:v%d]"),parsed.flasher.found_idx-1,val);    
//       }
//       data_buffer.isserviced++;
//     }else
//     if(obj[D_JSON_FLASHER].is<int>()){
//       int val = obj[D_JSON_FLASHER];
//       parsed.flasher.val[parsed.flasher.found_idx++] = val;
//       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_FLASHER " int" " [i%d:v%d]"),parsed.flasher.found_idx-1,val);    
//       data_buffer.isserviced++;
//     }else{
//     #ifdef ENABLE_LOG_LEVEL_INFO
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_FLASHER "Not Found"));
//       #endif
//     }
//   } //end flasher
  

//   /**************************************************************************************************
//   PULSER: formats "slow/fast",1,[1] (string,int,array of ints)
//   ***************************************************************************************************/

//   // Check pixel num exists (exit if it doesn't)
//   if(!obj[D_JSON_PULSER].isNull()){
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_PULSER));  
//     // Arrays
//     if(obj[D_JSON_PULSER].is<JsonArray>()){   
//       JsonArrayConst array = obj[D_JSON_PULSER];
//       for(JsonVariantConst v : array) {
//         int val = v.as<int>();
//         parsed.pulser.val[parsed.pulser.found_idx++] = val;
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PULSER " JsonArray " " [i%d:v%d]"),parsed.pulser.found_idx-1,val);    
//       }
//       data_buffer.isserviced++;
//     }else
//     if(obj[D_JSON_PULSER].is<int>()){
//       int val = obj[D_JSON_PULSER];
//       parsed.pulser.val[parsed.pulser.found_idx++] = val;
//       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PULSER " int" " [i%d:v%d]"),parsed.pulser.found_idx-1,val);    
//       data_buffer.isserviced++;
//     }else{
//       AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_PULSER "Not Found"));
//     }
//   } //end pulser
  

//   /**************************************************************************************************
//   FADE: formats "slow/fast",1,[1] (string,int,array of ints)
//   ***************************************************************************************************/

//   // Check pixel num exists (exit if it doesn't)
//   if(!obj[D_JSON_FADE].isNull()){
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_FADE));  
//     // Arrays
//     if(obj[D_JSON_FADE].is<JsonArray>()){   
//       JsonArrayConst array = obj[D_JSON_FADE];
//       for(JsonVariantConst v : array) {
//         int val = v.as<int>();
//         parsed.fade.val[parsed.fade.found_idx++] = val;
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_FADE " JsonArray " " [i%d:v%d]"),parsed.fade.found_idx-1,val);    
//       }
//       data_buffer.isserviced++;
//     }else
//     if(obj[D_JSON_FADE].is<int>()){
//       int val = obj[D_JSON_FADE];
//       parsed.fade.val[parsed.fade.found_idx++] = val;
//       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_FADE " int" " [i%d:v%d]"),parsed.fade.found_idx-1,val);    
//       data_buffer.isserviced++;
//     }else{
//       AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_FADE "Not Found"));
//     }
//   } //end fade
  

//   /**************************************************************************************************
//   TIMEON: "time_secs" or "time_ms" formats 1,[1] (int,array of ints)
//   ***************************************************************************************************/

//   // Check pixel num exists (exit if it doesn't)
//   if(!obj[D_JSON_TIME_SECS].isNull()){
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_TIME_SECS));  
//     // Arrays
//     if(obj[D_JSON_TIME_SECS].is<JsonArray>()){   
//       JsonArrayConst array = obj[D_JSON_TIME_SECS];
//       for(JsonVariantConst v : array) {
//         int val = v.as<int>();
//         parsed.timeon_ms.val[parsed.timeon_ms.found_idx++] = val*1000; //secs2ms
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_SECS " JsonArray " " [i%d:v%d]"),parsed.timeon_ms.found_idx-1,val);    
//       }
//       data_buffer.isserviced++;
//     }else
//     //no string command
//     if(obj[D_JSON_TIME_SECS].is<int>()){
//       int val = obj[D_JSON_TIME_SECS];
//       parsed.timeon_ms.val[parsed.timeon_ms.found_idx++] = val*1000; //secs2ms
//       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_SECS " int" " [i%d:v%d]"),parsed.timeon_ms.found_idx-1,val);    
//       data_buffer.isserviced++;
//     }else{
//       AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_SECS "Not Found"));
//     }
//   }else
//   if(!obj[D_JSON_TIME_MS].isNull()){
//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_PARSING_MATCHED " " D_JSON_TIME_MS));  
//     // Arrays
//     if(obj[D_JSON_TIME_MS].is<JsonArray>()){   
//       JsonArrayConst array = obj[D_JSON_TIME_MS];
//       for(JsonVariantConst v : array) {
//         int val = v.as<int>();
//         parsed.timeon_ms.val[parsed.timeon_ms.found_idx++] = val;
//         AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_MS " JsonArray " " [i%d:v%d]"),parsed.timeon_ms.found_idx-1,val);    
//       }
//       data_buffer.isserviced++;
//     }else
//     //no string command
//     if(obj[D_JSON_TIME_MS].is<int>()){
//       int val = obj[D_JSON_TIME_MS];
//       parsed.timeon_ms.val[parsed.timeon_ms.found_idx++] = val;
//       AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_MS " int" " [i%d:v%d]"),parsed.timeon_ms.found_idx-1,val);    
//       data_buffer.isserviced++;
//     }else{
//       AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS " " D_JSON_TIME_MS "Not Found"));
//     }
//   } 
  
  
//   /***********************************************************************************************************************************************************
//    * ***********************************************************************************************************************************************************
//    * Fill commands using parsed structs *************************************************************************************************************************
//    * ***********************************************************************************************************************************************************
//    * ***********************************************************************************************************************************************************/

//   int pixelidx = 0;
//   int subidx = 0;

//   if(parsed.hsb.found_idx){
//     pixelidx = 0; subidx = 0;
//     while(pixelidx<parsed.pixelnum.found_idx){  // Step across all pixels
//       int pixelnum = parsed.pixelnum.val[pixelidx++];
//       notif.pixel[pixelnum].colour = parsed.hsb.val[subidx];
//       if(subidx<parsed.hsb.found_idx-1){subidx++;}
//     }
//   }
  
//   if(parsed.hue.found_idx){
//     pixelidx = 0; subidx = 0;
//     while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
//       int pixelnum = parsed.pixelnum.val[pixelidx++];
//       notif.pixel[pixelnum].colour.H = HueN2F(parsed.hue.val[subidx]);
//       notif.pixel[pixelnum].mode = NOTIF_MODE_STATIC_ON_ID; //default
//       if(subidx<parsed.hue.found_idx-1){subidx++;}
//     }
//   }

//   if(parsed.sat.found_idx){
//     pixelidx = 0; subidx = 0;
//     while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
//       int pixelnum = parsed.pixelnum.val[pixelidx++];
//       notif.pixel[pixelnum].colour.S = SatN2F(parsed.sat.val[subidx]);
//       notif.pixel[pixelnum].mode = NOTIF_MODE_STATIC_ON_ID; //default
//       if(subidx<parsed.sat.found_idx-1){subidx++;}
//     }
//   }

//   if(parsed.brt.found_idx){
//     pixelidx = 0; subidx = 0;
//     while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
//       int pixelnum = parsed.pixelnum.val[pixelidx++];
//       notif.pixel[pixelnum].colour.B = BrtN2F(parsed.brt.val[subidx]);
//       notif.pixel[pixelnum].mode = NOTIF_MODE_STATIC_ON_ID; //default
//       if(subidx<parsed.brt.found_idx-1){subidx++;}
//     }
//   }
  
//   // #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
//   // // RGB method - white part
//   // if(parsed.white.found_idx){
//   //   pixelidx = 0; subidx = 0;
//   //   while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
//   //     int pixelnum = parsed.pixelnum.val[pixelidx++];
//   //     notif.pixel[pixelnum].colourWhite = parsed.white.val[subidx];
//   //     notif.pixel[pixelnum].mode = NOTIF_MODE_STATIC_ON_ID; //default
//   //     if(subidx<parsed.white.found_idx-1){subidx++;}
//   //     if(notif.flags.fWhenOnlyWhiteExistsSetBrightnessOfHSBtoZero){
//   //       if(!parsed.brt.found_idx){ //no brightness defined
//   //         notif.pixel[pixelnum].colour.B = 0;        
//   //       }
//   //     }

//   //   }
//   // }
//   // #endif

//   if(parsed.flasher.found_idx){
//     pixelidx = 0; subidx = 0;
//     while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
//       int pixelnum = parsed.pixelnum.val[pixelidx++];
//       notif.pixel[pixelnum].period_ms = parsed.flasher.val[subidx]; 
//       notif.pixel[pixelnum].mode = NOTIF_MODE_BLINKING_ON_ID; 
//       // Step across the subset indexes if they exist, else last/only is used for all pixelnum
//       if(subidx<parsed.flasher.found_idx-1){subidx++;}
//     }
//   }

//   if(parsed.pulser.found_idx){
//     pixelidx = 0; subidx = 0;
//     while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
//       int pixelnum = parsed.pixelnum.val[pixelidx++];
//       notif.pixel[pixelnum].period_ms = parsed.pulser.val[subidx]; 
//       notif.pixel[pixelnum].mode = NOTIF_MODE_PULSING_ON_ID; 
//       if(subidx<parsed.pulser.found_idx-1){subidx++;}
//     }
//   }

//   char buffer[30];

//   if(parsed.fade.found_idx){
//     pixelidx = 0; subidx = 0;
//     while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
//       int pixelnum = parsed.pixelnum.val[pixelidx++];
//       notif.pixel[pixelnum].period_ms = parsed.fade.val[subidx];
//       notif.pixel[pixelnum].period_ms = constrain(notif.pixel[pixelnum].period_ms,100,65000);
//       // If pixel was off, then turn on else turn off
      
//       Serial.printf("notif.pixel[%d].mode = %s %d;\n\r",pixelnum,GetNotificationModeNamebyID(notif.pixel[pixelnum].mode, buffer),notif.pixel[pixelnum].period_ms);

//       if(
//         (notif.pixel[pixelnum].mode==NOTIF_MODE_OFF_ID)||
//         (notif.pixel[pixelnum].mode==NOTIF_MODE_STATIC_OFF_ID)||
//         (notif.pixel[pixelnum].mode==NOTIF_MODE_PULSING_OFF_ID)||
//         (notif.pixel[pixelnum].mode==NOTIF_MODE_FADE_OFF_ID)
//         // (notif.pixel[pixelnum].colour.B)
//         // ||(notif.pixel[pixelnum].colourWhite)
//         ){
//           notif.pixel[pixelnum].mode = NOTIF_MODE_FADE_ON_ID; 
//           Serial.printf("notif.pixel[%d].mode = NOTIF_MODE_FADE_ON_ID;\n\r",pixelnum);
//         }else{
//           notif.pixel[pixelnum].mode = NOTIF_MODE_FADE_OFF_ID; 
//           Serial.printf("notif.pixel[%d].mode = NOTIF_MODE_FADE_OFF_ID;\n\r",pixelnum);
//         }
//         notif.pixel[pixelnum].transition_progess = 0; //0 to 100%
//       if(subidx<parsed.fade.found_idx-1){subidx++;}
//     }
//   }

//   // If neither flasher or pulser defined, set as static as default
//   if((!parsed.flasher.found_idx)&&(!parsed.pulser.found_idx)&&(!parsed.fade.found_idx)){
//     pixelidx = 0; subidx = 0;
//     while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels
//       int pixelnum = parsed.pixelnum.val[pixelidx++];
//       notif.pixel[pixelnum].period_ms = 1000; // not used, but set to something reasonable 
//       notif.pixel[pixelnum].mode = NOTIF_MODE_STATIC_ON_ID; 
//     }
//   }

//   if(parsed.timeon_ms.found_idx){
//     pixelidx = 0; subidx = 0;
//     while(pixelidx<parsed.pixelnum.found_idx){     // Step across all pixels

//     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_NEO D_JSON_NOTIFICATIONS "timeon_ms [%d i%d:v%d]"),pixelidx,parsed.timeon_ms.found_idx,parsed.pixelnum.val[pixelidx]);    
      
//       int pixelnum = parsed.pixelnum.val[pixelidx++];
//       notif.pixel[pixelnum].auto_time_off_secs = (parsed.timeon_ms.val[subidx]/1000); 
//       if(subidx<parsed.timeon_ms.found_idx-1){subidx++;}

//       //also reset timer millis so they align and run together
//       notif.pixel[pixelnum].tSavedUpdate = millis();
//     }
//   }else{
//     Serial.println("parsed.timeon_ms.found_idx NOT FOUND");
//   }

//   // for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){
//   //   //you diAddLog_mP2(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_NEO_MODE_NOTIFICATIONS "Index [%d] RgbColor %d\t%d\t%d\t flash [%d]"),i,setcolour[i].r,setcolour[i].g,setcolour[i].b,setcolour[i].flash_enabled);
//   // }

//   notif.tSaved.Timeout = millis(); // resets timeout counter
//   notif.tSaved.TimeoutCounter = millis();
//   pCONT_iLight->animation.mode_id = pCONT_iLight->ANIMATION_MODE_NOTIFICATIONS_ID;
//   notif.flags.fForcePanelUpdate = true;

  


// } // END FUNCTION

// const char* mRGBAnimator::GetNotificationModeNamebyID(uint8_t id, char* buffer){
//   // return 
//   //     (id == NOTIF_MODE_OFF_ID          ? D_NOTIF_MODE_OFF_NAME_CTR  :
//   //     (id == NOTIF_MODE_STATIC_OFF_ID   ? D_NOTIF_MODE_STATIC_OFF_NAME_CTR :
//   //     (id == NOTIF_MODE_STATIC_ON_ID    ? D_NOTIF_MODE_STATIC_ON_NAME_CTR :
//   //     (id == NOTIF_MODE_BLINKING_OFF_ID ? D_NOTIF_MODE_BLINKING_OFF_NAME_CTR :
//   //     (id == NOTIF_MODE_BLINKING_ON_ID  ? D_NOTIF_MODE_BLINKING_ON_NAME_CTR :
//   //     (id == NOTIF_MODE_PULSING_OFF_ID  ? D_NOTIF_MODE_PULSING_OFF_NAME_CTR :
//   //     (id == NOTIF_MODE_PULSING_ON_ID   ? D_NOTIF_MODE_PULSING_ON_NAME_CTR :
//   //     "NoMatch")))))));
//   return WARNING_NOTHANDLED_CTR;
// }
// int8_t mRGBAnimator::GetNotificationModeIDbyName(const char* c){
//   if(c=='\0'){ 
//     return -1;
//   }
//   // if(strstr(c,D_NOTIF_MODE_OFF_NAME_CTR)){
//   //   return NOTIF_MODE_OFF_ID;
//   // }else 
//   // if(strstr(c,D_NOTIF_MODE_STATIC_OFF_NAME_CTR)){
//   //   return NOTIF_MODE_STATIC_OFF_ID;
//   // }else 
//   // if(strstr(c,D_NOTIF_MODE_STATIC_ON_NAME_CTR)){
//   //   return NOTIF_MODE_STATIC_ON_ID;
//   // }else 
//   // if(strstr(c,D_NOTIF_MODE_BLINKING_OFF_NAME_CTR)){
//   //   return NOTIF_MODE_BLINKING_OFF_ID;
//   // }else 
//   // if(strstr(c,D_NOTIF_MODE_BLINKING_ON_NAME_CTR)){
//   //   return NOTIF_MODE_BLINKING_ON_ID;
//   // }else 
//   // if(strstr(c,D_NOTIF_MODE_PULSING_OFF_NAME_CTR)){
//   //   return NOTIF_MODE_PULSING_OFF_ID;
//   // }else 
//   // if(strstr(c,D_NOTIF_MODE_PULSING_ON_NAME_CTR)){
//   //   return NOTIF_MODE_PULSING_ON_ID;
//   // }
//   else{
//     return -1;
//   }
// }

// // Update struct that shows overview, always sends
// uint8_t mRGBAnimator::ConstructJSON_Notifications(uint8_t json_level){

//   #ifdef ENABLE_LOG_LEVEL_DEBUG
//   AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO "f::ConstructJSON_Notifications"));
//   #endif

//   // DynamicJsonDocument doc(1400);
//   // JsonObject root = doc.to<JsonObject>();

//   // root["fForcePanelUpdate"] = notif.flags.fForcePanelUpdate;
//   // root["fShowPanelUpdate"] = notif.flags.fShowPanelUpdate;
//   // root["fEnableTimeoutAll"] = notif.flags.fEnableTimeoutAll;
      
      
      
//   // JsonArray mode_arr = root.createNestedArray("pCONT_iLight->auto_time_off_secs");
//   // for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){
//   //   mode_arr.add(notif.pixel[i].pCONT_iLight->auto_time_off_secs);
//   // }


//   // // JsonArray mode_arr = root.createNestedArray("mode");
//   // // for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){
//   // //   mode_arr.add(GetNotificationModeNamebyID(notif.pixel[i].mode));
//   // // }
//   // // JsonArray period_ms_arr = root.createNestedArray("period_ms");
//   // // for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){
//   // //   period_ms_arr.add(notif.pixel[i].period_ms);
//   // // }
//   // // JsonArray transition_progess_arr = root.createNestedArray(PSTR("transition_progess"));
//   // // for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){
//   // //   transition_progess_arr.add(notif.pixel[i].transition_progess);
//   // // }
//   // // JsonArray hsb_arr = root.createNestedArray("hsb");
//   // // for(int i=0;i<STRIP_NOTIFICATION_SIZE;i++){ 
//   // //   char tmpctr[10];  memset(tmpctr,0,sizeof(tmpctr));
//   // //   sprintf(tmpctr,PSTR("%d,%d,%d"),HueF2N(notif.pixel[i].colour.H),
//   // //                             SatF2N(notif.pixel[i].colour.S),
//   // //                             BrtF2N(notif.pixel[i].colour.B));
//   // //   hsb_arr.add(tmpctr);
//   // // }
//   // //     pixelobj["tSavedUpdate"] = notif.pixel[i].tSavedUpdate;
//   // //     pixelobj["tRateUpdate"] = notif.pixel[i].tRateUpdate;

//   // data_buffer.payload.len = measureJson(root)+1;
//   // serializeJson(doc,data_buffer.payload.ctr);
//   return 0;

// }
#endif

/*******************************************************************************************************************
********************************************************************************************************************
************ END OF NOTIFICATION DEFINITIONS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/




#endif //USE_MODULE_LIGHTS_ADDRESSABLE




