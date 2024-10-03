#include "mInternalClock.h"

#ifdef USE_MODULE_CONTROLLER_INTERNAL_CLOCK

void mInternalClock::parse_JSONCommand(JsonParserObject obj){

  char buffer[50];

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  
  /***
   * As order of importance, others that rely on previous commands must come after
   * */
  if(jtok = obj[D_LIGHTPOWER]){
    int light = jtok.getInt();
    if(light == 2){
      SetLightState(!GetLightState());
    }else{
      SetLightState(light);      
    }

    ALOG_COM( PSTR(D_LOG_CEILINGFAN D_PARSING_MATCHED D_COMMAND_SVALUE),D_LIGHTPOWER,GetLightState()?"On":"Off");
    // Response_mP(S_JSON_COMMAND_SVALUE, D_LIGHTPWER,D_TOGGLE);
  }


  if(jtok = obj[D_FANSPEED]){
    int speed = jtok.getInt();
    if(speed>3){
      // Response_mP(S_JSON_COMMAND_SVALUE, D_FANSPEED,D_PARSING_NOMATCH);
      speed=0; //default off
    }      
    SetFanSpeed(speed, false);
    AddLog(LOG_LEVEL_INFO,PSTR("GetFanspeed=%d"),GetFanspeed());
    ALOG_INF(PSTR(D_LOG_CEILINGFAN D_PARSING_MATCHED D_COMMAND_NVALUE),D_FANSPEED,speed);
    // Response_mP(S_JSON_COMMAND_NVALUE,D_FANSPEED,speed);
  }
  
  mqtthandler_power_ifchanged.flags.SendNow = true;

}


#endif // USE_MODULE_LIGHTS_INTERFACE
