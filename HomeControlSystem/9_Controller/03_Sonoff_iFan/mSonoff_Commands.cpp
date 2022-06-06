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

    ALOG_COM( PSTR(D_LOG_CEILINGFAN D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_LIGHTPOWER,GetLightState()?"On":"Off");
    // Response_mP(S_JSON_COMMAND_SVALUE, D_JSON_LIGHTPWER,D_TOGGLE);
  }


  if(jtok = obj[D_JSON_FANSPEED]){
    int speed = jtok.getInt();
    if(speed>3){
      // Response_mP(S_JSON_COMMAND_SVALUE, D_JSON_FANSPEED,D_PARSING_NOMATCH);
      speed=0; //default off
    }      
    SetFanSpeed(speed, false);
    AddLog(LOG_LEVEL_INFO,PSTR("GetFanspeed=%d"),GetFanspeed());
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CEILINGFAN D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_FANSPEED,speed);
    // Response_mP(S_JSON_COMMAND_NVALUE,D_JSON_FANSPEED,speed);
  }
  
  mqtthandler_power_ifchanged.flags.SendNow = true;

}


#endif // USE_MODULE_LIGHTS_INTERFACE
