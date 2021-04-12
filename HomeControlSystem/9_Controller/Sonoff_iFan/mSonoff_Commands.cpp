#include "mSonoffIFan.h"

#ifdef USE_MODULE_CONTROLLER_SONOFF_IFAN

void mSonoffIFan::parse_JSONCommand(JsonParserObject obj){

  char buffer[50];

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  
  /***
   * As order of importance, others that rely on previous commands must come after
   * */



  if(jtok = obj[D_JSON_LIGHTPOWER]){
    int light = jtok.getInt();
    if(light == 2){
      SetLightState(!GetLightState());
    }else{
      SetLightState(light);      
    }
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CEILINGFAN D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_LIGHTPOWER,GetLightState()?"On":"Off");
    // Response_mP(S_JSON_COMMAND_SVALUE, D_JSON_LIGHTPOWER,D_TOGGLE);
  
  }


  if(jtok = obj[D_JSON_FANSPEED]){
    int speed = jtok.getInt();
    if(speed>3){
      Response_mP(S_JSON_COMMAND_SVALUE, D_JSON_FANSPEED,D_PARSING_NOMATCH);
      speed=0; //default off
    }      
    SetFanSpeed(speed, false);
    AddLog(LOG_LEVEL_INFO,PSTR("GetFanspeed=%d"),GetFanspeed());
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CEILINGFAN D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_FANSPEED,speed);
    Response_mP(S_JSON_COMMAND_NVALUE,D_JSON_FANSPEED,speed);
  
  }


  // if(jtok = obj[PM_JSON_FANSPEED_MANUAL]){
  // //   if(jtok.isStr()){
  // //     if((tmp_id=mPaletteI->GetPaletteIDbyName(jtok.getStr()))>=0){
  // //       CommandSet_PaletteID(tmp_id);
  // //       data_buffer.isserviced++;
  // //     }
  // //   }else
  //   if(jtok.isNum()){
  //     CommandSet_FanSpeed_Manual(jtok.getInt());
  //     set_fan_pwm = jtok.getInt();
  //     set_fan_speed = map(jtok.getInt(),1,1023,0,3);
  //     data_buffer.isserviced++;
  //   }
  // //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  // //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
  // //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }
  

  // if(jtok = obj[PM_JSON_FANSPEED]){
  // //   if(jtok.isStr()){
  // //     if((tmp_id=mPaletteI->GetPaletteIDbyName(jtok.getStr()))>=0){
  // //       CommandSet_PaletteID(tmp_id);
  // //       data_buffer.isserviced++;
  // //     }
  // //   }else
  //   if(jtok.isNum()){
  //     CommandSet_FanSpeed_Manual(map(jtok.getInt(),0,3,1,1023)); //fix
  //     set_fan_pwm = map(jtok.getInt(),0,3,1,1023);
  //     set_fan_speed = jtok.getInt();
  //     data_buffer.isserviced++;
  //   }
  // //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  // //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
  // //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }
  
  mqtthandler_sensor_ifchanged.flags.SendNow = true;

}





/******************************************************************************************************************************
*******************************************************************************************************************************
****************** PixelHardwareType *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

// void mSonoffIFan::CommandSet_FanSpeed_Manual(uint16_t value){

//   analogWrite(pin, value);

// AddLog(LOG_LEVEL_TEST,PSTR("pwm %d value = %d"),pin,value);



// //   char buffer[20];
// //   pCONT_set->Settings.light_settings.type = value;
// //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_HARDWARE_TYPE)), GetPixelHardwareTypeName(buffer));
// //   #endif // ENABLE_LOG_LEVEL_COMMANDS
// } 
// int8_t mInterfaceLight::GetPixelHardwareTypeIDbyName(const char* c){
//   if(c=='\0') return -1;
//   if(     strcmp_P(c,PM_PIXEL_HARDWARE_TYPE_RGBCCT_PWM_CTR)==0){ return LT_PWM5; }
//   else if(strcmp_P(c,PM_PIXEL_HARDWARE_TYPE_WS28XX_CTR)==0){       return LT_WS2812; }
//   return -1;
// }
// const char* mInterfaceLight::GetPixelHardwareTypeName(char* buffer){
//   return GetPixelHardwareTypeNamebyID(pCONT_set->Settings.light_settings.type, buffer);
// }
// const char* mInterfaceLight::GetPixelHardwareTypeNamebyID(uint8_t id, char* buffer){
//   switch(id){
//     case LT_PWM5:   memcpy_P(buffer,PM_PIXEL_HARDWARE_TYPE_RGBCCT_PWM_CTR,sizeof(PM_PIXEL_HARDWARE_TYPE_RGBCCT_PWM_CTR)); break;
//     case LT_WS2812: memcpy_P(buffer,PM_PIXEL_HARDWARE_TYPE_WS28XX_CTR,sizeof(PM_PIXEL_HARDWARE_TYPE_WS28XX_CTR));         break;
//   }
//   return buffer;
// }


//   //FanPWM = 0-1023
//   if(obj.containsKey(D_JSON_FANPWM)){
//     int pwm_val = obj[D_JSON_FANPWM];
//     // if(speed>3){
//     //   Response_mP(S_JSON_COMMAND_SVALUE, D_JSON_FANPWM,D_PARSING_NOMATCH);
//     //   speed=0; //default off
//     // }      
    
//     set_fan_speed  = map(pwm_val, pwm_range_min, pwm_range_max, 0, 3);;
//     set_fan_pwm = pwm_val;

//     analogWrite(pin,set_fan_pwm);

//     // SetFanspeed(speed, false);
//     AddLog(LOG_LEVEL_INFO,PSTR("GetFanspeed TEST=%d"),GetFanspeed());
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CEILINGFAN D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_FANPWM,set_fan_pwm);
//     Response_mP(S_JSON_COMMAND_NVALUE,D_JSON_FANPWM,set_fan_pwm);
//     // isserviced++;
//   }

#endif // USE_MODULE_LIGHTS_INTERFACE
