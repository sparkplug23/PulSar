#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR


void mAnimatorLight::parse_JSONCommand(void){

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

  #ifdef ENABLE_PIXEL_FUNCTION_EFFECTS
  /**
   *  Flasher function specific commands
   * */
  if(jtok = obj[PM_JSON_EFFECTS].getObject()[PM_JSON_FUNCTION]){
    if(jtok.isStr()){
      if((tmp_id=GetFlasherFunctionIDbyName(jtok.getStr()))>=0){
        CommandSet_Flasher_FunctionID(tmp_id);
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

  if(jtok = obj[PM_JSON_EFFECTS].getObject()[PM_JSON_COLOUR_REFRESH_RATE]){ 
    CommandSet_Flasher_UpdateColourRegion_RefreshSecs(jtok.getInt());
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_JSON_COLOUR_REFRESH_RATE)), flashersettings.update_colour_region.refresh_secs);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  if(jtok = obj[PM_JSON_EFFECTS].getObject()[PM_JSON_DIRECTION]){ 
    CommandSet_Flasher_Flags_Movement_Direction(jtok.getInt());
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_DIRECTION)), flashersettings.flags.movement_direction);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }
  
  if(jtok = obj[PM_JSON_EFFECTS].getObject()[PM_JSON_BRIGHTNESS_MIN]){ 
    CommandSet_Flasher_Brightness_Min(jtok.getInt());
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_MIN)), flashersettings.brightness_min);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  if(jtok = obj[PM_JSON_EFFECTS].getObject()[PM_JSON_BRIGHTNESS_MAX]){ 
    CommandSet_Flasher_Brightness_Max(jtok.getInt());
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_MAX)), flashersettings.brightness_max);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  if(jtok = obj[PM_JSON_EFFECTS].getObject()[PM_JSON_ALTERNATE_BRIGHTNESS_MIN]){ 
    CommandSet_Flasher_Alternate_Brightness_Min(jtok.getInt());
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_ALTERNATE_BRIGHTNESS_MIN)), shared_flasher_parameters.alternate_brightness_min);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  if(jtok = obj[PM_JSON_EFFECTS].getObject()[PM_JSON_ALTERNATE_BRIGHTNESS_MAX]){ 
    CommandSet_Flasher_Alternate_Brightness_Max(jtok.getInt());
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_ALTERNATE_BRIGHTNESS_MAX)), shared_flasher_parameters.alternate_brightness_max);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  if(jtok = obj[PM_JSON_EFFECTS].getObject()[PM_JSON_ALTERNATE_AMOUNT]){ 
    CommandSet_Flasher_Alternate_RandomAmountPercentage(jtok.getInt());
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_ALTERNATE D_JSON_RANDOM_AMOUNT)), shared_flasher_parameters.alternate_random_amount_as_percentage);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  if(jtok = obj[PM_JSON_EFFECTS].getObject()[PM_JSON_AGED_COLOURING]){
    pCONT_iLight->animation.flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique = jtok.getInt();
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_AGED_COLOURING)), pCONT_iLight->animation.flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique);
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }
  #endif // ENABLE_PIXEL_FUNCTION_EFFECTS


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

  // maybe to be done with a topic directly so its faster, straight buffer into setpixel

    // uint8_t colour_array[5];
    // memset(colour_array,0,sizeof(colour_array));
    // uint8_t jsonpair_count = jtok.size();

    // for(int index = 0; index < jsonpair_count; index++){
    //   jtok.nextOne(); //skip start of object
    //   Serial.println(jtok.getInt());
    //   colour_array[index] = jtok.getInt();
    // }
    
    // CommandSet_ActiveSolidPalette_Raw(colour_array);
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
        for(uint16_t i=0;i<pCONT_iLight->settings.light_size_count;i++){ 
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

      pCONT_iLight->ShowInterface();

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


  #ifdef ENABLE_PIXEL_FUNCTION_EFFECTS
  /********************************************************************************************************************************
  **********Flasher Function ******************************************************************************************************
  ********************************************************************************************************************************/
void mAnimatorLight::CommandSet_Flasher_FunctionID(uint8_t value){
  
  flashersettings.function = value;      //make function "changeFlasherFunction" so then the region is automatically updated internally
  flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID;
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  char buffer[30];
  AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_SVALUE_K(D_JSON_EFFECTS, D_JSON_FUNCTION)), GetFlasherFunctionName(buffer, sizeof(buffer)));
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}

int8_t mAnimatorLight::GetFlasherFunctionIDbyName(const char* f){

  if(f=='\0') return -2;
  if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_SOLID_RGBCCT_NAME_CTR)){ return EFFECTS_FUNCTION_SOLID_RGBCCT_ID; }
  if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_SLOW_GLOW_NAME_CTR)){  return EFFECTS_FUNCTION_SLOW_GLOW_ID; }
  if(mSupport::CheckCommand_P(f, PM_EFFECTS_FUNCTION_SEQUENTIAL_NAME_CTR)){ return EFFECTS_FUNCTION_SEQUENTIAL_ID; }
  return -1;

}
const char* mAnimatorLight::GetFlasherFunctionName(char* buffer, uint8_t buflen){
  return GetFlasherFunctionNamebyID(flashersettings.function, buffer, buflen);
}
const char* mAnimatorLight::GetFlasherFunctionNamebyID(uint8_t id, char* buffer, uint8_t buflen){
  switch(id){
    default:  snprintf_P(buffer, buflen, PM_SEARCH_NOMATCH);  break;
    case EFFECTS_FUNCTION_SLOW_GLOW_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_SLOW_GLOW_NAME_CTR);  break;
    case EFFECTS_FUNCTION_SEQUENTIAL_ID:  snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_SEQUENTIAL_NAME_CTR); break;
    case EFFECTS_FUNCTION_SOLID_RGBCCT_ID:   snprintf_P(buffer, buflen, PM_EFFECTS_FUNCTION_SOLID_RGBCCT_NAME_CTR);  break;
  }
  return buffer;
}

  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/


int8_t mAnimatorLight::GetFlasherRegionIDbyName(const char* c){
  if(c=='\0') return -1;
  if(     strcmp_P(c,PM_EFFECTS_REGION_COLOUR_SELECT_NAME_CTR)==0){ return EFFECTS_REGION_COLOUR_SELECT_ID; }
  else if(strcmp_P(c,PM_EFFECTS_REGION_ANIMATE_NAME_CTR)==0){       return EFFECTS_REGION_ANIMATE_ID; }
  return -1;
}
const char* mAnimatorLight::GetFlasherRegionName(char* buffer){
  return GetFlasherRegionNamebyID(flashersettings.region, buffer);
}
const char* mAnimatorLight::GetFlasherRegionNamebyID(uint8_t id, char* buffer){
  switch(id){
    case EFFECTS_REGION_COLOUR_SELECT_ID:   memcpy_P(buffer,PM_EFFECTS_REGION_COLOUR_SELECT_NAME_CTR,sizeof(PM_EFFECTS_REGION_COLOUR_SELECT_NAME_CTR));
    case EFFECTS_REGION_ANIMATE_ID:         memcpy_P(buffer,PM_EFFECTS_REGION_ANIMATE_NAME_CTR,sizeof(PM_EFFECTS_REGION_ANIMATE_NAME_CTR));
  }
  return buffer;
}





  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/




void mAnimatorLight::CommandSet_Flasher_UpdateColourRegion_RefreshSecs(uint8_t value){
  
  flashersettings.update_colour_region.refresh_secs = value; 
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_JSON_COLOUR_REFRESH_RATE)), flashersettings.update_colour_region.refresh_secs);
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}

  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/

void mAnimatorLight::CommandSet_Flasher_Flags_Movement_Direction(uint8_t value){

  flashersettings.flags.movement_direction = value;
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_DIRECTION)), flashersettings.flags.movement_direction);
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}

  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/

void mAnimatorLight::CommandSet_Flasher_Brightness_Min(uint8_t value){
  
  flashersettings.brightness_min = value;
  
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_MIN)), flashersettings.brightness_min);
    #endif // ENABLE_LOG_LEVEL_COMMANDS

}

  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/

void mAnimatorLight::CommandSet_Flasher_Brightness_Max(uint8_t value){
  
  flashersettings.brightness_max = value;
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_MAX)), flashersettings.brightness_max);
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}

  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/

void mAnimatorLight::CommandSet_Flasher_Alternate_Brightness_Min(uint8_t value){
  
  shared_flasher_parameters.alternate_brightness_min = value;
  
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_ALTERNATE_BRIGHTNESS_MIN)), shared_flasher_parameters.alternate_brightness_min);
    #endif // ENABLE_LOG_LEVEL_COMMANDS

}

  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/


void mAnimatorLight::CommandSet_Flasher_Alternate_Brightness_Max(uint8_t value){
  
  shared_flasher_parameters.alternate_brightness_max = value;
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_ALTERNATE_BRIGHTNESS_MAX)), shared_flasher_parameters.alternate_brightness_max);
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}


  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/

void mAnimatorLight::CommandSet_Flasher_Alternate_RandomAmountPercentage(uint8_t value){
    shared_flasher_parameters.alternate_random_amount_as_percentage = value;
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_ALTERNATE D_JSON_RANDOM_AMOUNT)), shared_flasher_parameters.alternate_random_amount_as_percentage);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
}

  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/


void mAnimatorLight::CommandSet_Flasher_Flags_ApplySaturationRandomnessOnPaletteColours(uint8_t value){
    pCONT_iLight->animation.flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique = value;
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_AGED_COLOURING)), pCONT_iLight->animation.flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
}
  #endif // ENABLE_PIXEL_FUNCTION_EFFECTS


  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/


#ifdef ENABLE_PIXEL_FUNCTION_PIXELGROUPING
void mAnimatorLight::CommandSet_PixelGrouping_Flag_AgedColouring(uint8_t value){
    pixel_group.flags.fEnabled = value;
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_PIXELGROUPING D_JSON_ENABLED)), pixel_group.flags.fEnabled);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
}

  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/

void mAnimatorLight::CommandSet_PixelGrouping_Flag_ModeID(uint8_t value){
    pixel_group.flags.multiplier_mode_id = value;
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_PIXELGROUPING D_JSON_MODE)), pixel_group.flags.multiplier_mode_id);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
}
  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/


void mAnimatorLight::CommandSet_PixelGrouping_Flag_Multiplier(uint8_t value){
    pixel_group.multiplier = value;
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_PIXELGROUPING,D_JSON_MULTIPLIER)), pixel_group.multiplier);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
}

  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/

void mAnimatorLight::CommandSet_PixelGrouping_MappedMultiplierData(uint8_t* value, uint8_t length){

  memcpy(editable_mapped_array_data_array, value, sizeof(uint8_t)*length);

  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  for(uint8_t index=0;index<length;index++){
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_JSON_MAPPED_MULTIPLIER_DATA " [i%d:v%d]"),index,editable_mapped_array_data_array[index]);
  }
  #endif// ENABLE_LOG_LEVEL_COMMANDS         
  
}
#endif // ENABLE_PIXEL_FUNCTION_PIXELGROUPING


  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/

  #ifdef ENABLE_PIXEL_FUNCTION_MIXER
 void mAnimatorLight:: CommandSet_Mixer_Flags_Enabled
  if(jtok = obj[PM_JSON_MIXER].getObject()[PM_JSON_ENABLED]){ 
    mixer.flags.Enabled = jtok.getInt();
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_MIXER D_JSON_ENABLED)), mixer.flags.Enabled);
    #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  }
   
  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/

  // if(jtok = obj[PM_JSON_MIXER].getObject()[PM_JSON_TIME_SCALER_AS_PERCENTAGE]){ 
  //   mixer.time_scaler = jtok.getInt();
  //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_TIME_SCALER_AS_PERCENTAGE)), mixer.time_scaler);
  //   #endif //#ifdef ENABLE_LOG_LEVEL_COMMANDS
  // }

  if(jtok = obj[PM_JSON_MIXER].getObject()[PM_JSON_RUNTIME_DURATION_SCALER_PERCENTAGE]){
  void mAnimatorLight::  CommandSet_Mixer_RunTimeScalerPercentage 
    mixer.run_time_duration_scaler_as_percentage = jtok.getInt();
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_MIXER D_JSON_RUNTIME_DURATION_SCALER_PERCENTAGE)), mixer.run_time_duration_scaler_as_percentage);
    #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  }
  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/


  if(jtok = obj[PM_JSON_MIXER].getObject()[PM_JSON_RUNNING_ID]){  
    void mAnimatorLight::
    CommandSet_Mixer_RunningID(jtok.getInt());
    uint8_t val = jtok.getInt();
    mixer.running_id = val;
    LoadMixerGroupByID(val);
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_RUNNING_ID)), val);
    #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  }
  #endif //ENABLE_PIXEL_FUNCTION_MIXER


  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/



#endif //USE_MODULE_LIGHTS_ANIMATOR




