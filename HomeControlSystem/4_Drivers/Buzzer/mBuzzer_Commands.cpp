
#include "mBuzzer.h"

#ifdef USE_MODULE_DRIVERS_BUZZER

int8_t mBuzzer::CheckAndExecute_JSONCommands(){

  if(mSupport::SetTopicMatch(data_buffer.topic.ctr,D_MODULE_DRIVERS_BUZZER_FRIENDLY_CTR)>=0){
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_MODULE_DRIVERS_BUZZER_FRIENDLY_CTR));
    #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
    pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
    parse_JSONCommand();
    return FUNCTION_RESULT_HANDLED_ID;
  }
  return FUNCTION_RESULT_UNKNOWN_ID;

}


void mBuzzer::parse_JSONCommand(void){

  // Need to parse on a copy
  char parsing_buffer[data_buffer.payload.len+1];
  memcpy(parsing_buffer,data_buffer.payload.ctr,sizeof(char)*data_buffer.payload.len+1);
  // AddLog(LOG_LEVEL_TEST, PSTR("\"%s\""),parsing_buffer);
  JsonParser parser(parsing_buffer);
  JsonParserObject obj = parser.getRootObject();   
  if (!obj) { 
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_ERROR, PSTR("DeserializationError with \"%s\""),parsing_buffer);
    #endif// ENABLE_LOG_LEVEL_INFO
    return;
  }  
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