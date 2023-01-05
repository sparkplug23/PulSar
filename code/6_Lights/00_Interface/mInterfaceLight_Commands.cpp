
#include "mInterfaceLight.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE

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
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_TIME_ON)),auto_off_settings.time_decounter_secs);  
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }
  // New unique version to phase in
  if(jtok = obj["Light"].getObject()[PM_JSON_TIME_ON]){ // default to secs
    CommandSet_Auto_Time_Off_Secs(jtok.getInt());
    data_buffer.isserviced++;
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_TIME_ON)),auto_off_settings.time_decounter_secs);  
    #endif // ENABLE_LOG_LEVEL_DEBUG
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

#ifdef USE_MODULE_NETWORK_MQTT
  // mqtthandler_debug_teleperiod.flags.SendNow = true;
  // mqtthandler_scene_teleperiod.flags.SendNow = true;
  MQTTHandler_Set_RefreshAll();
#endif //ifdef USE_MODULE_NETWORK_MQTT
  
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

  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_TIME_ON)),auto_off_settings.time_decounter_secs);  
  #endif

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



#endif // USE_MODULE_LIGHTS_INTERFACE
