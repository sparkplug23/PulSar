#include "mInterfaceLight.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE // interface is the gateway



/***********
 * 
RAM:   [======    ]  56.0% (used 45908 bytes from 81920 bytes)
Flash: [======    ]  56.6% (used 579680 bytes from 1023984 bytes)
********************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************
****************** CommandSet_x START *************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/


void mInterfaceLight::CommandSet_PixelHardwareTypeID(uint8_t value){
  char buffer[20];
  pCONT_set->Settings.light_settings.type = value;
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_HARDWARE_TYPE)), GetPixelHardwareTypeName(buffer));
  #endif // ENABLE_LOG_LEVEL_COMMANDS
} 
int8_t mInterfaceLight::GetPixelHardwareTypeIDbyName(const char* c){
  if(c=='\0') return -1;
  if(     strcmp_P(c,PM_PIXEL_HARDWARE_TYPE_RGBCCT_PWM_CTR)==0){ return LT_PWM5; }
  else if(strcmp_P(c,PM_PIXEL_HARDWARE_TYPE_WS28XX_CTR)==0){       return LT_WS2812; }
  return -1;
}
const char* mInterfaceLight::GetPixelHardwareTypeName(char* buffer){
  return GetPixelHardwareTypeNamebyID(pCONT_set->Settings.light_settings.type, buffer);
}
const char* mInterfaceLight::GetPixelHardwareTypeNamebyID(uint8_t id, char* buffer){
  switch(id){
    case LT_PWM5:   memcpy_P(buffer,PM_PIXEL_HARDWARE_TYPE_RGBCCT_PWM_CTR,sizeof(PM_PIXEL_HARDWARE_TYPE_RGBCCT_PWM_CTR)); break;
    case LT_WS2812: memcpy_P(buffer,PM_PIXEL_HARDWARE_TYPE_WS28XX_CTR,sizeof(PM_PIXEL_HARDWARE_TYPE_WS28XX_CTR));         break;
  }
  return buffer;
}



void mInterfaceLight::CommandSet_HardwareColourTypeID(uint8_t value){
  char buffer[60];
  settings.pixel_hardware_color_order_id = value; 
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_RGB_COLOUR_ORDER)), GetHardwareColourTypeName(buffer, sizeof(buffer)));
  #endif
}
const char* mInterfaceLight::GetHardwareColourTypeName(char* buffer, uint8_t buflen){
  return GetHardwareColourTypeNameByID(pCONT_iLight->animation.mode_id, buffer, buflen);
}
const char* mInterfaceLight::GetHardwareColourTypeNameByID(uint8_t id, char* buffer, uint8_t buflen){
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


void mInterfaceLight::CommandSet_LightPowerState(uint8_t state){
 #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_LIGHTPOWER)), light_power_state);
    #endif   

  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_LIGHT DEBUG_INSERT_PAGE_BREAK "f::CommandSet_LightPowerState %d"), state);
  #endif
          
  switch(state){
    case LIGHT_POWER_STATE_ON_ID:
      // SetAnimationProfile(ANIMATION_PROFILE_TURN_ON_ID);

      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_LIGHT "animation.mode_id=%d"), animation.mode_id);
      memcpy(&animation,&animation_stored,sizeof(animation));// RESTORE copy of state
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_LIGHT "animation loading previous state"));
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_LIGHT "animation.mode_id=%d"), animation.mode_id);

      // Handle depending on hardware type
      
      switch(animation.mode_id){
        #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
        case ANIMATION_MODE_FLASHER_ID:
          AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT "f::SetAnimationProfile" "ANIMATION_MODE_FLASHER_ID"));
          pCONT_ladd->flashersettings.function = pCONT_ladd->FLASHER_FUNCTION_SLOW_GLOW_ID;
          

        break;
        #endif // 
        default:
          AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT "f::SetAnimationProfile" "default"));
        case ANIMATION_MODE_SCENE_ID:
          AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT "f::SetAnimationProfile" "scene"));
          // mode_singlecolour.name_id = MODE_SINGLECOLOUR_COLOURSCENE_ID;
          // animation.mode_id = ANIMATION_MODE_SCENE_ID;
        break;
      }

      animation_override.time_ms = 1000; //force fast rate to turn on
      changeBri(255);
      animation.flags.fForceUpdate = true;
      pCONT_ladd->first_set = 1;//set all

      light_power_state = true;

    break;
    case LIGHT_POWER_STATE_OFF_ID:
      // SetAnimationProfile(ANIMATION_PROFILE_TURN_OFF_ID);


      // Remember previous state for returning to later  
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_LIGHT "animation.mode_id=%d"), animation.mode_id);
      memcpy(&animation_stored,&animation,sizeof(animation)); // STORE copy of state
      AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_LIGHT "animation SAVING state to return to"));
      // If I use overrides, is this neccesary?
        AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_LIGHT "animation.mode_id=%d"), animation.mode_id);
      light_power_state = false;

      //if palette, set colour to black and update all
      switch(animation.mode_id){
        case ANIMATION_MODE_FLASHER_ID://PRESETS_ID:
          // #ifdef ENABLE_LOG_LEVEL_DEBUG
          AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT "f::SetAnimationProfile" "ANIMATION_MODE_FLASHER_ID"));
          // #endif
          animation_override.time_ms = 1000; //force fast rate to turn on
          // flashersettings.function = FLASHER_FUNCTION_SLOW_GLOW_ID;
          // animation.flags.fForceUpdate = true;
          // animation.brightness = 0;
          // first_set = 1;//set all

          animation.transition.time_ms.val = 1000;
          animation.flags.fForceUpdate = true;
          animation_override.time_ms = 1000; //force fast rate to turn on

          changeBri(0);

          

        break;
        default:
          // #ifdef ENABLE_LOG_LEVEL_DEBUG
          AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT "f::SetAnimationProfile" "default"));
          // #endif
        case ANIMATION_MODE_SCENE_ID:
          // #ifdef ENABLE_LOG_LEVEL_DEBUG
          AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT "f::SetAnimationProfile" "ANIMATION_MODE_SCENE_ID"));
          // #endif
          // setBriRGB(0);
          // animation.brightness = 0;
          animation.transition.time_ms.val = 1000;
          animation.flags.fForceUpdate = true;
          animation_override.time_ms = 1000; //force fast rate to turn on

          changeBri(0);

          // mode_singlecolour.name_id = MODE_SINGLECOLOUR_COLOURSCENE_ID;
          // animation.mode_id = ANIMATION_MODE_SCENE_ID;
          
          // first_set = 1;//set all
        break;
      }
      animation.flags.fEndUpdatesWhenAnimationCompletes = true; //once turned off, stop animations





    break;
  } // END switch

  
}



void mInterfaceLight::CommandSet_Hue_360(uint16_t hue_new){
    uint16_t hue = 0;
    uint8_t  sat = 0;
    uint8_t  brt = 0;
    getHSB(&hue,&sat,&brt);  
    changeHSB(hue_new, sat, brt);
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_HUE)), getHue());
    #endif // ENABLE_LOG_LEVEL_COMMANDS
}


void mInterfaceLight::CommandSet_Sat_255(uint8_t sat_new){
  uint16_t hue = 0;
  uint8_t  sat = 0;
  uint8_t  brt = 0;
  getHSB(&hue,&sat,&brt);
  changeHSB(hue, sat_new, brt);  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_SAT)), getSat());
  #endif // ENABLE_LOG_LEVEL_COMMANDS
}


void mInterfaceLight::CommandSet_Brt_255(uint8_t brt_new){
  setBri(brt_new);
  UpdateFinalColourComponents();
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS)), getBri());
  #endif // ENABLE_LOG_LEVEL_COMMANDS
}


void mInterfaceLight::CommandSet_BrtRGB_255(uint8_t bri) {
  setBriRGB(bri);
  // saveSettings();
  UpdateFinalColourComponents();
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS)), getBri());
  #endif // ENABLE_LOG_LEVEL_COMMANDS
}


void mInterfaceLight::CommandSet_SceneColour_Raw(uint8_t* values){
  setChannelsRaw(values);
  UpdateFinalColourComponents();
  
  #ifdef ENABLE_LOG_LEVEL_INFO
  char buffer[30];
  snprintf_P(buffer, sizeof(buffer), PSTR("[%d,%d,%d,%d,%d]"),values[0],values[1],values[2],values[3],values[4]);
  AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_SVALUE_K(D_JSON_SCENE,D_JSON_COLOUR)), buffer);
  #endif // ENABLE_LOG_LEVEL_DEBUG

}


void mInterfaceLight::CommandSet_Animation_Transition_Time_Ms(uint16_t value){
    
    animation.transition.time_ms.val = value;

    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_TIME_MS)),animation.transition.time_ms.val);  
    #endif

}

void mInterfaceLight::CommandSet_Animation_Transition_Rate_Ms(uint16_t value){
    
    animation.transition.rate_ms.val = value;

    if(animation.transition.rate_ms.val<animation.transition.time_ms.val){ animation.transition.time_ms.val = animation.transition.rate_ms.val;}

    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_RATE_MS)),animation.transition.time_ms.val);  
    #endif

}



const char* mInterfaceLight::GetTransitionMethodName(char* buffer, uint8_t buflen){
  return GetTransitionMethodNameByID(pCONT_iLight->animation.transition.method_id, buffer, buflen);
}
const char* mInterfaceLight::GetTransitionMethodNameByID(uint8_t id, char* buffer, uint8_t buflen){
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


void mInterfaceLight::CommandSet_TransitionOrderID(uint8_t value){
  
  animation.transition.order_id = value;
  
  char buffer[50];

    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_SVALUE_K(D_JSON_TRANSITION,D_JSON_ORDER)), GetTransitionOrderName(buffer, sizeof(buffer)));
    #endif // ENABLE_LOG_LEVEL_COMMANDS

}

const char* mInterfaceLight::GetTransitionOrderName(char* buffer, uint8_t buflen){
  return GetTransitionOrderNameByID(animation.transition.order_id, buffer, buflen);
}
const char* mInterfaceLight::GetTransitionOrderNameByID(uint8_t id, char* buffer, uint8_t buflen){
  switch(id){  default:    
    case TRANSITION_ORDER_NONE_ID:       memcpy_P(buffer, PM_TRANSITION_ORDER_NONE_NAME_CTR, sizeof(PM_TRANSITION_ORDER_NONE_NAME_CTR)); break;// smooth shift between them
    case TRANSITION_ORDER_RANDOM_ID:     memcpy_P(buffer, PM_TRANSITION_ORDER_RANDOM_NAME_CTR, sizeof(PM_TRANSITION_ORDER_RANDOM_NAME_CTR)); break;// smooth shift between them
    case TRANSITION_ORDER_INORDER_ID:    memcpy_P(buffer, PM_TRANSITION_ORDER_INORDER_NAME_CTR, sizeof(PM_TRANSITION_ORDER_INORDER_NAME_CTR)); break;// instant shift
    case TRANSITION_ORDER_CENTRE_OUT_ID: memcpy_P(buffer, PM_TRANSITION_ORDER_CENTRE_OUT_NAME_CTR, sizeof(PM_TRANSITION_ORDER_CENTRE_OUT_NAME_CTR)); break;
    case TRANSITION_ORDER_ROTATE_ID:     memcpy_P(buffer, PM_TRANSITION_ORDER_ROTATE_NAME_CTR, sizeof(PM_TRANSITION_ORDER_ROTATE_NAME_CTR)); break;
    case TRANSITION_ORDER_FIXED_ID:      memcpy_P(buffer, PM_TRANSITION_ORDER_FIXED_NAME_CTR, sizeof(PM_TRANSITION_ORDER_FIXED_NAME_CTR)); break;// blend shift with random twinkles on random number of leds
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


void mInterfaceLight::CommandSet_Auto_Time_Off_Secs(uint16_t value){
    
    auto_time_off_secs = value;

    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_TIME_ON)),auto_time_off_secs);  
    #endif

}

void mInterfaceLight::CommandSet_LightSizeCount(uint16_t value){

    light_size_count = value;
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_STRIP_SIZE)), light_size_count);
    #endif // ENABLE_LOG_LEVEL_COMMANDS



}

void mInterfaceLight::CommandSet_LightsCountToUpdateAsNumber(uint16_t value){
  
  animation.transition.pixels_to_update_as_number.val = value;
  animation.transition.pixels_to_update_as_percentage.val = GetPixelsToUpdateAsPercentageFromNumber(value);

  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_NUMBER)), value);
  #endif

}

void mInterfaceLight::CommandSet_LightsCountToUpdateAsPercentage(uint8_t value){

  animation.transition.pixels_to_update_as_number.val = GetPixelsToUpdateAsPercentageFromNumber(value);
  animation.transition.pixels_to_update_as_percentage.val = value;
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION,D_JSON_PIXELS_UPDATE_PERCENTAGE)), value);
  #endif

}
uint16_t mInterfaceLight::GetPixelsToUpdateAsNumberFromPercentage(uint8_t percentage){
  return mapvalue(percentage, 0,100, 0,pCONT_iLight->light_size_count);
}
uint8_t mInterfaceLight::GetPixelsToUpdateAsPercentageFromNumber(uint16_t number){
  return mapvalue(number ,0,pCONT_iLight->light_size_count, 0,100);
}



void mInterfaceLight::CommandSet_SingleColourMode_ID(uint8_t value){
    

    mode_singlecolour.name_id = value;
    char buffer[40];
    animation.mode_id = ANIMATION_MODE_SCENE_ID; //#Idea. make optional
    // if(mode_singlecolour.name_id == MODE_SINGLECOLOUR_PALETTE_SINGLE_ID){
    //   mode_singlecolour.parts = 0;
    // }
    mode_singlecolour.parts = 0;



    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_SCENE_NAME)), GetSceneName(buffer, sizeof(buffer)));
    #endif // ENABLE_LOG_LEVEL_COMMANDS

}
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
  if(strcmp_P(c,PM_MODE_SINGLECOLOUR_COLOURSCENE_NAME_CTR)==0){
    return MODE_SINGLECOLOUR_COLOURSCENE_ID;
  }
  if(strcmp_P(c,PM_MODE_SINGLECOLOUR_PALETTE_SINGLE_NAME_CTR)==0){
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


void mInterfaceLight::CommandSet_PaletteID(uint8_t value){

char buffer[50];



      animation.palette_id = value < PALETTELIST_STATIC_LENGTH_ID ? value : 0;

// animation.palette_id = tmp_id;



    #ifdef ENABLE_PIXEL_FUNCTION_MIXER
    if(animation.mode_id == ANIMATION_MODE_FLASHER_ID){
        pCONT_ladd->flashersettings.region = pCONT_ladd->FLASHER_REGION_COLOUR_SELECT_ID; //update colours in use
    }
    #endif
    #ifdef ENABLE_PALETTE_FORCED_MODE
    //  animation.mode_id = ANIMATION_MODE_PRESETS_ID;
    #endif
    
#ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_COLOUR_PALETTE)), animation.palette_id);
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette_id, buffer, sizeof(buffer)));
#endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS


}


void mInterfaceLight::CommandSet_AnimationModeID(uint8_t value){

  char buffer[60];
  animation.mode_id = value;        
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_ANIMATIONMODE)), GetAnimationModeName(buffer, sizeof(buffer)));
  #endif

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
    #ifdef ENABLE_PIXEL_FUNCTION_AMBILIGHT
    case ANIMATION_MODE_AMBILIGHT_ID:     memcpy_P(buffer, PM_ANIMATION_MODE_AMBILIGHT_NAME_CTR, sizeof(PM_ANIMATION_MODE_AMBILIGHT_NAME_CTR)); break;
    #endif // ENABLE_PIXEL_FUNCTION_AMBILIGHT
    #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
      case ANIMATION_MODE_NOTIFICATIONS_ID: memcpy_P(buffer, PM_ANIMATION_MODE_NOTIFICATIONS_NAME_CTR,sizeof(PM_ANIMATION_MODE_NOTIFICATIONS_NAME_CTR)); break;
    #endif
    case ANIMATION_MODE_FLASHER_ID:       memcpy_P(buffer, PM_ANIMATION_MODE_FLASHER_NAME_CTR, sizeof(PM_ANIMATION_MODE_FLASHER_NAME_CTR)); break;
    case ANIMATION_MODE_SCENE_ID:         memcpy_P(buffer, PM_ANIMATION_MODE_SCENE_NAME_CTR , sizeof(PM_ANIMATION_MODE_SCENE_NAME_CTR)); break;
    
    #ifdef ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
    case ANIMATION_MODE_MANUAL_SETPIXEL_ID:         memcpy_P(buffer, PM_ANIMATION_MODE_MANUAL_SETPIXEL_NAME_CTR , sizeof(PM_ANIMATION_MODE_MANUAL_SETPIXEL_NAME_CTR)); break;
    #endif // ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
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
  #ifdef ENABLE_PIXEL_FUNCTION_AMBILIGHT
  if(strstr_P(c,PM_ANIMATION_MODE_AMBILIGHT_NAME_CTR)){      return ANIMATION_MODE_AMBILIGHT_ID; }
  #endif // ENABLE_PIXEL_FUNCTION_AMBILIGHT
  if(strstr_P(c,PM_ANIMATION_MODE_SCENE_NAME_CTR)){          return ANIMATION_MODE_SCENE_ID; }
  if(strstr_P(c,PM_ANIMATION_MODE_FLASHER_NAME_CTR)){        return ANIMATION_MODE_FLASHER_ID; }
  return -1;
}




#endif // USE_MODULE_LIGHTS_INTERFACE




/*******************************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************
****************** CommandSet_x END *************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/
