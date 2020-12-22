#include "mRGBAnimator.h"

#ifdef USE_MODULE_LIGHTS_ADDRESSABLE


  #ifdef ENABLE_PIXEL_FUNCTION_FLASHER
  /********************************************************************************************************************************
  **********Flasher Function ******************************************************************************************************
  ********************************************************************************************************************************/
void mRGBAnimator::CommandSet_Flasher_FunctionID(uint8_t value){
  
  flashersettings.function = value;      //make function "changeFlasherFunction" so then the region is automatically updated internally
  flashersettings.region = FLASHER_REGION_COLOUR_SELECT_ID;
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  char buffer[30];
  AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_K(D_JSON_FUNCTION)), GetFlasherFunctionName(buffer));
  #endif // ENABLE_LOG_LEVEL_COMMANDS

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

  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/


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





  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/




void mRGBAnimator::CommandSet_Flasher_UpdateColourRegion_RefreshSecs(uint8_t value){
  
  flashersettings.update_colour_region.refresh_secs = value; 
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_FLASHER D_JSON_COLOUR_REFRESH_RATE)), flashersettings.update_colour_region.refresh_secs);
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}

  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/

void mRGBAnimator::CommandSet_Flasher_Flags_Movement_Direction(uint8_t value){

  flashersettings.flags.movement_direction = value;
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_DIRECTION)), flashersettings.flags.movement_direction);
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}

  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/

void mRGBAnimator::CommandSet_Flasher_Brightness_Min(uint8_t value){
  
  flashersettings.brightness_min = value;
  
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_MIN)), flashersettings.brightness_min);
    #endif // ENABLE_LOG_LEVEL_COMMANDS

}

  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/

void mRGBAnimator::CommandSet_Flasher_Brightness_Max(uint8_t value){
  
  flashersettings.brightness_max = value;
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_MAX)), flashersettings.brightness_max);
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}

  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/

void mRGBAnimator::CommandSet_Flasher_Alternate_Brightness_Min(uint8_t value){
  
  shared_flasher_parameters.alternate_brightness_min = value;
  
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_ALTERNATE_BRIGHTNESS_MIN)), shared_flasher_parameters.alternate_brightness_min);
    #endif // ENABLE_LOG_LEVEL_COMMANDS

}

  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/


void mRGBAnimator::CommandSet_Flasher_Alternate_Brightness_Max(uint8_t value){
  
  shared_flasher_parameters.alternate_brightness_max = value;
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_ALTERNATE_BRIGHTNESS_MAX)), shared_flasher_parameters.alternate_brightness_max);
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}


  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/

void mRGBAnimator::CommandSet_Flasher_Alternate_RandomAmountPercentage(uint8_t value){
    shared_flasher_parameters.alternate_random_amount_as_percentage = value;
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_ALTERNATE D_JSON_RANDOM_AMOUNT)), shared_flasher_parameters.alternate_random_amount_as_percentage);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
}

  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/


void mRGBAnimator::CommandSet_Flasher_Flags_ApplySaturationRandomnessOnPaletteColours(uint8_t value){
    pCONT_iLight->animation.flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique = value;
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_AGED_COLOURING)), pCONT_iLight->animation.flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
}
  #endif // ENABLE_PIXEL_FUNCTION_FLASHER


  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/


#ifdef ENABLE_PIXEL_FUNCTION_PIXELGROUPING
void mRGBAnimator::CommandSet_PixelGrouping_Flag_AgedColouring(uint8_t value){
    pixel_group.flags.fEnabled = value;
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_PIXELGROUPING D_JSON_ENABLED)), pixel_group.flags.fEnabled);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
}

  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/

void mRGBAnimator::CommandSet_PixelGrouping_Flag_ModeID(uint8_t value){
    pixel_group.flags.multiplier_mode_id = value;
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_PIXELGROUPING D_JSON_MODE)), pixel_group.flags.multiplier_mode_id);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
}
  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/


void mRGBAnimator::CommandSet_PixelGrouping_Flag_Multiplier(uint8_t value){
    pixel_group.multiplier = value;
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_PIXELGROUPING,D_JSON_MULTIPLIER)), pixel_group.multiplier);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
}

  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/

void mRGBAnimator::CommandSet_PixelGrouping_MappedMultiplierData(uint8_t* value, uint8_t length){

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
 void mRGBAnimator:: CommandSet_Mixer_Flags_Enabled
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
  void mRGBAnimator::  CommandSet_Mixer_RunTimeScalerPercentage 
    mixer.run_time_duration_scaler_as_percentage = jtok.getInt();
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_MIXER D_JSON_RUNTIME_DURATION_SCALER_PERCENTAGE)), mixer.run_time_duration_scaler_as_percentage);
    #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  }
  /********************************************************************************************************************************
  **********Flasher Region ******************************************************************************************************
  ********************************************************************************************************************************/


  if(jtok = obj[PM_JSON_MIXER].getObject()[PM_JSON_RUNNING_ID]){  
    void mRGBAnimator::
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



#endif //USE_MODULE_LIGHTS_ADDRESSABLE




