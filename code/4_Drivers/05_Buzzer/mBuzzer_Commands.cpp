
#include "mBuzzer.h"

#ifdef USE_MODULE_DRIVERS_BUZZER

void mBuzzer::parse_JSONCommand(JsonParserObject obj){

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  JsonParserObject jobj = 0;
 
  // uint8_t relay_id= 0,state=-1;    //assume index 0 if none given

  // if(jtok = obj[PM_JSON_POWERNAME]){
  //   if(jtok.isStr()){
  //     relay_id = GetRelayIDbyName(jtok.getStr());
  //   }else 
  //   if(jtok.isNum()){
  //     relay_id  = jtok.getInt();
  //   }
  // }


  // if(jtok = obj[PM_JSON_RELAY].getObject()[PM_JSON_TIME_ON]){
  //   CommandSet_Timer_Decounter(jtok.getInt(), relay_id);
  // }else
  // if(jtok = obj[PM_JSON_RELAY].getObject()[PM_JSON_TIME_ON_SECS]){
  //   CommandSet_Timer_Decounter(jtok.getInt(), relay_id);
  // }

  uint8_t beeps = 0;

  if(jtok = obj["beeps"]){
    AddLog(LOG_LEVEL_TEST, PSTR("bbeeps=%d"),jtok.getInt());
    BuzzerBeep(jtok.getInt());
    // pinMode(Buzzer.pin, jtok.getInt());

  }


  mqtthandler_settings_teleperiod.flags.SendNow = true;

}


// void mBuzzer::CommandSet_BuzzerBeep(uint16_t time_secs, uint8_t relay_id){
//   relay_status[relay_id].timer_decounter.seconds = time_secs;
//   relay_status[relay_id].timer_decounter.active = time_secs > 0 ? true : false;
//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_RELAYS "Set" D_JSON_TIME "Relay%d " "%d" D_UNIT_SECOND), relay_id, relay_status[relay_id].timer_decounter.seconds);  
//   #endif
// }


#endif // USE_MODULE_DRIVERS_RELAY