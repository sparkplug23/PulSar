#include "mRGBAnimator.h"

#ifdef USE_MODULE_LIGHTS_ADDRESSABLE

/*******************************************************************************************************************
********************************************************************************************************************
************TASKER**************************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/

int8_t mRGBAnimator::Tasker(uint8_t function){ 

  int8_t function_result = 0;
  
  #ifdef ENABLE_DEVFEATURE_Tasker_Override_Forcing_Variables_Testing
  Tasker_Override_Forcing_Variables_Testing();
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
    case FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT:
      Settings_Default();
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
    #ifdef ENABLE_LOG_LEVEL_ERROR
    AddLog_P(LOG_LEVEL_ERROR, PSTR(D_JSON_DESERIALIZATION_ERROR));
    #endif// ENABLE_LOG_LEVEL_ERROR
    return;
  }  
  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  char buffer[50];

  #ifdef ENABLE_PIXEL_FUNCTION_FLASHER
  /**
   *  Flasher function specific commands
   * */
  if(jtok = obj[PM_JSON_FLASHER].getObject()[PM_JSON_FUNCTION]){
    if(jtok.isStr()){
      if((tmp_id=GetFlasherFunctionIDbyName(jtok.getStr()))>=0){
        CommandSet_Flasher_FunctionID(jtok.getInt());
        data_buffer.isserviced++;
      }
    }else
    if(jtok.isNum()){
      CommandSet_Flasher_FunctionID(jtok.getInt());
      data_buffer.isserviced++;
    }
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_K(D_JSON_FUNCTION)), GetFlasherFunctionName(buffer));
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  if(jtok = obj[PM_JSON_FLASHER].getObject()[PM_JSON_COLOUR_REFRESH_RATE]){ 
    CommandSet_Flasher_UpdateColourRegion_RefreshSecs(jtok.getInt());
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_FLASHER D_JSON_COLOUR_REFRESH_RATE)), flashersettings.update_colour_region.refresh_secs);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  if(jtok = obj[PM_JSON_FLASHER].getObject()[PM_JSON_DIRECTION]){ 
    CommandSet_Flasher_Flags_Movement_Direction(jtok.getInt());
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_DIRECTION)), flashersettings.flags.movement_direction);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }
  
  if(jtok = obj[PM_JSON_FLASHER].getObject()[PM_JSON_BRIGHTNESS_MIN]){ 
    CommandSet_Flasher_Brightness_Min(jtok.getInt());
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_MIN)), flashersettings.brightness_min);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  if(jtok = obj[PM_JSON_FLASHER].getObject()[PM_JSON_BRIGHTNESS_MAX]){ 
    CommandSet_Flasher_Brightness_Max(jtok.getInt());
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_MAX)), flashersettings.brightness_max);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  if(jtok = obj[PM_JSON_FLASHER].getObject()[PM_JSON_ALTERNATE_BRIGHTNESS_MIN]){ 
    CommandSet_Flasher_Alternate_Brightness_Min(jtok.getInt());
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_ALTERNATE_BRIGHTNESS_MIN)), shared_flasher_parameters.alternate_brightness_min);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  if(jtok = obj[PM_JSON_FLASHER].getObject()[PM_JSON_ALTERNATE_BRIGHTNESS_MAX]){ 
    CommandSet_Flasher_Alternate_Brightness_Max(jtok.getInt());
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_ALTERNATE_BRIGHTNESS_MAX)), shared_flasher_parameters.alternate_brightness_max);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  if(jtok = obj[PM_JSON_FLASHER].getObject()[PM_JSON_ALTERNATE_AMOUNT]){ 
    CommandSet_Flasher_Alternate_RandomAmountPercentage(jtok.getInt());
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_ALTERNATE D_JSON_RANDOM_AMOUNT)), shared_flasher_parameters.alternate_random_amount_as_percentage);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  if(jtok = obj[PM_JSON_FLASHER].getObject()[PM_JSON_AGED_COLOURING]){
    pCONT_iLight->animation.flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique = jtok.getInt();
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_AGED_COLOURING)), pCONT_iLight->animation.flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }
  #endif // ENABLE_PIXEL_FUNCTION_FLASHER


  #ifdef ENABLE_PIXEL_FUNCTION_PIXELGROUPING
  /**
   * Pixel output grouping settings
   * */
  if(jtok = obj[PM_JSON_PIXELGROUPING].getObject()[PM_JSON_AGED_COLOURING]){
    CommandSet_PixelGrouping_Flag_AgedColouring(jtok.getInt());
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_PIXELGROUPING D_JSON_ENABLED)), pixel_group.flags.fEnabled);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  if(jtok = obj[PM_JSON_PIXELGROUPING].getObject()[PM_JSON_MODE]){
    CommandSet_PixelGrouping_Flag_ModeID(jtok.getInt());
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_PIXELGROUPING D_JSON_MODE)), pixel_group.flags.multiplier_mode_id);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  if(jtok = obj[PM_JSON_PIXELGROUPING].getObject()[PM_JSON_MULTIPLIER]){ 
    CommandSet_PixelGrouping_Flag_Multiplier(jtok.getInt());
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_PIXELGROUPING D_JSON_MULTIPLIER)), pixel_group.multiplier);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  if(jtok = obj[PM_JSON_PIXELGROUPING].getObject()[PM_JSON_MAPPED_MULTIPLIER_DATA]){ 
    // CommandSet_PixelGrouping_MappedMultiplierData();
    if(jtok.isArray()){
      uint8_t array[60];
      uint8_t arrlen = 0;
      JsonParserArray arrobj = obj[PM_JSON_PIXELGROUPING].getObject()[PM_JSON_MAPPED_MULTIPLIER_DATA];
      for(auto v : arrobj) {
        if(arrlen > D_MAPPED_ARRAY_DATA_MAXIMUM_LENGTH){ break; }
        array[arrlen++] = v.getInt();
        #ifdef ENABLE_LOG_LEVEL_DEBUG
        AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_JSON_MAPPED_MULTIPLIER_DATA " [i%d:v%d]"),arrlen-1,array[arrlen-1]);
        #endif// ENABLE_LOG_LEVEL_DEBUG          
      }
      CommandSet_PixelGrouping_MappedMultiplierData(array, arrlen);
      data_buffer.isserviced++;
    }
  }
  #endif // ENABLE_PIXEL_FUNCTION_PIXELGROUPING

  #ifdef ENABLE_PIXEL_FUNCTION_MIXER
  if(jtok = obj[PM_JSON_MIXER].getObject()[PM_JSON_ENABLED]){ 
    CommandSet_Mixer_Flags_Enabled(jtok.getInt());
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_MIXER D_JSON_ENABLED)), mixer.flags.Enabled);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }
   
  // if(jtok = obj[PM_JSON_MIXER].getObject()[PM_JSON_TIME_SCALER_AS_PERCENTAGE]){ 
  //   mixer.time_scaler = jtok.getInt();
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_TIME_SCALER_AS_PERCENTAGE)), mixer.time_scaler);
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }

  if(jtok = obj[PM_JSON_MIXER].getObject()[PM_JSON_RUNTIME_DURATION_SCALER_PERCENTAGE]){ 
    CommandSet_Mixer_RunTimeScalerPercentage(jtok.getInt());
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_MIXER D_JSON_RUNTIME_DURATION_SCALER_PERCENTAGE)), mixer.run_time_duration_scaler_as_percentage);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  if(jtok = obj[PM_JSON_MIXER].getObject()[PM_JSON_RUNNING_ID]){  
    CommandSet_Mixer_RunningID(jtok.getInt());
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_RUNNING_ID)), val);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }
  #endif //ENABLE_PIXEL_FUNCTION_MIXER


  if(jtok = obj[PM_RGB_DATA_STREAM]){  // PM_RGB_DATA_STREAM

    // uint8_t colour_array[5];
    // memset(colour_array,0,sizeof(colour_array));
    // uint8_t jsonpair_count = jtok.size();

    // for(int index = 0; index < jsonpair_count; index++){
    //   jtok.nextOne(); //skip start of object
    //   Serial.println(jtok.getInt());
    //   colour_array[index] = jtok.getInt();
    // }
    
    // CommandSet_SceneColour_Raw(colour_array);
    // data_buffer.isserviced++;

    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    // snprintf_P(buffer, sizeof(buffer), PSTR("[%d,%d,%d,%d,%d]"),colour_array[0],colour_array[1],colour_array[2],colour_array[3],colour_array[4]);
    // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_SVALUE_K(D_JSON_SCENE,D_JSON_COLOUR)), buffer);
    // #endif // ENABLE_LOG_LEVEL_DEBUG
  }



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


#ifdef ENABLE_PIXEL_FUNCTION_FLASHER

void mRGBAnimator::init_flasher_settings(){

  flashersettings.flags.movement_direction = 0;
  


// A "wipe" or "sine wave" that applies a saturation change across the string. So either, from all christmas coloured to slowly blending from the top of the tree to the bottom, to white (or another solid colour), or apply the saturation change in a loop/rotation. 


  
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

  #ifdef ENABLE_DEVFEATURE_SETPIXELOUTPUT_VARIABLE

    if(indexPixel == 0){
      setpixel_variable_index_counter = 0; // reset
      pixel_group.mapped_array_data.index = 0;
    }

    if(setpixel_variable_index_counter>50){ return; }

    if(pixel_group.flags.fEnabled){

      uint8_t pixel_multiplier_count = 0;
      switch(pixel_group.flags.multiplier_mode_id){
        case PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_UNIFORM_ID:
          pixel_multiplier_count = pixel_group.multiplier;
          break;
        case PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_RANDOM_ID:
          pixel_multiplier_count = random(1,pixel_group.multiplier);
          break;
        case PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID:
          if(pixel_group.mapped_array_data.values != nullptr){
            // if any match, then jump index of lights forward
            uint8_t check_index = pixel_group.mapped_array_data.index;
            pixel_multiplier_count = pixel_group.mapped_array_data.values[check_index];
            
            //AddLog_P(LOG_LEVEL_WARN, PSTR("PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID %d"),pixel_multiplier_count);
            pixel_group.mapped_array_data.index++;
            if(pixel_group.mapped_array_data.index >= 15){//pixel_group.mapped_array_data.length){
              pixel_group.mapped_array_data.index = 0;
            }

          }else{
            pixel_multiplier_count = pixel_group.multiplier;
          }
          break;
      }

      for(uint8_t ii = 0; ii < pixel_multiplier_count; ii++){
        if(setpixel_variable_index_counter>50){ return; }
        stripbus->SetPixelColor(setpixel_variable_index_counter++,color_hardware);  
      }

    }else{
      stripbus->SetPixelColor(indexPixel,color_hardware);
    } // pixel_group.flags.fEnabled

    AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("indexPixel=%d,setpixel_variable_index_counterhere=%d"),indexPixel,setpixel_variable_index_counter);

  #else

    stripbus->SetPixelColor(indexPixel,color_hardware);

  #endif // ENABLE_DEVFEATURE_SETPIXELOUTPUT_VARIABLE

}

RgbTypeColor mRGBAnimator::GetPixelColor(uint16_t indexPixel)
{
  DEBUG_LINE;
  if(stripbus == nullptr){    
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_NEO "stripbus == nullptr"));
    #endif
  }

  
  #ifdef ENABLE_DEVFEATURE_SETPIXELOUTPUT_VARIABLE

  #ifdef USE_DEVFEATURE_PIXEL_OUTPUT_MULTIPLIER
  indexPixel *= USE_DEVFEATURE_PIXEL_OUTPUT_MULTIPLIER;
  #endif
    if(indexPixel == 0){
      setpixel_variable_index_counter = 0; // reset
      pixel_group.mapped_array_data.index = 0;
    }

    if(setpixel_variable_index_counter>50){ return; }

    if(pixel_group.flags.fEnabled){

      uint8_t pixel_multiplier_count = 0;
      switch(pixel_group.flags.multiplier_mode_id){
        case PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_UNIFORM_ID:
          pixel_multiplier_count = pixel_group.multiplier;
          break;
        case PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_RANDOM_ID:
          pixel_multiplier_count = random(1,pixel_group.multiplier);
          break;
        case PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID:
          if(pixel_group.mapped_array_data.values != nullptr){
            // if any match, then jump index of lights forward
            uint8_t check_index = pixel_group.mapped_array_data.index;
            pixel_multiplier_count = pixel_group.mapped_array_data.values[check_index];
            
            //AddLog_P(LOG_LEVEL_WARN, PSTR("PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID %d"),pixel_multiplier_count);
            pixel_group.mapped_array_data.index++;
            if(pixel_group.mapped_array_data.index >= 15){//pixel_group.mapped_array_data.length){
              pixel_group.mapped_array_data.index = 0;
            }

          }else{
            pixel_multiplier_count = pixel_group.multiplier;
          }
          break;
      }

      for(uint8_t ii = 0; ii < pixel_multiplier_count; ii++){
        if(setpixel_variable_index_counter>50){ return; }
        stripbus->SetPixelColor(setpixel_variable_index_counter++,color_hardware);  
      }

    }else{
      stripbus->SetPixelColor(indexPixel,color_hardware);
    } // pixel_group.flags.fEnabled

    AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("indexPixel=%d,setpixel_variable_index_counterhere=%d"),indexPixel,setpixel_variable_index_counter);

  #else

    RgbTypeColor color_hardware = stripbus->GetPixelColor(indexPixel);

  #endif

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
  RgbcctColor targetColor_npb = targetColor;//RgbcctColor(targetColor.R,targetColor.G,targetColor.B);

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


#ifdef ENABLE_DEVFEATURE_Tasker_Override_Forcing_Variables_Testing

  Tasker_Override_Forcing_Variables_Testing();
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

#endif // ENABLE_DEVFEATURE_Tasker_Override_Forcing_Variables_Testing






#endif //USE_MODULE_LIGHTS_ADDRESSABLE




