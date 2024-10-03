#include "mUserMod_01.h"

#ifdef USE_MODULE_CONTROLLER_USERMOD_01

void mUserMod_01::parse_JSONCommand(JsonParserObject obj){

  char buffer[50];
  
  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  
  /***
   * As order of importance, others that rely on previous commands must come after
   * */

  // if(jtok = obj[PM_FANSPEED_MANUAL]){
  // //   if(jtok.isStr()){
  // //     if((tmp_id=mPaletteI->GetPaletteIDbyName(jtok.getStr()))>=0){
  // //       CommandSet_PaletteID(tmp_id);
  // //       data_buffer.isserviced++;
  // //     }
  // //   }else
  //   // if(jtok.isNum()){
  //   //   CommandSet_FanSpeed_Manual(jtok.getInt());
  //   //   set_fan_pwm = jtok.getInt();
  //   //   set_fan_speed = map(jtok.getInt(),1,1023,0,3);
  //   //   data_buffer.isserviced++;
  //   // }
  // //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  // //   ALOG_DBG(PSTR(D_LOG_LIGHT D_COMMAND_SVALUE_K(D_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette_id, buffer, sizeof(buffer)));
  // //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }
  

  mqtthandler_sensor_ifchanged.flags.SendNow = true;

}


#endif // USE_MODULE_CONTROLLER_USERMOD_01
