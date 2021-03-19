#include "mDoorBell.h"

#ifdef USE_MODULE_CONTROLLER_DOORCHIME


int8_t mDoorBell::CheckAndExecute_JSONCommands(){

  // Check if instruction is for me
  if(mSupport::SetTopicMatch(data_buffer.topic.ctr,"doorbell")>=0){
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_MQTT D_TOPIC_COMMAND "doorbell"));//D_MODULE_CUSTOM_FAN_FRIENDLY_CTR));
    #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
    pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
    parse_JSONCommand();
    return FUNCTION_RESULT_HANDLED_ID;
  }else{
    return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
  }

}

void mDoorBell::parse_JSONCommand(void){

  char buffer[50];

  // Need to parse on a copy
  char parsing_buffer[data_buffer.payload.len+1];
  memcpy(parsing_buffer,data_buffer.payload.ctr,sizeof(char)*data_buffer.payload.len+1);
  JsonParser parser(parsing_buffer);
  JsonParserObject obj = parser.getRootObject();   
  if (!obj) { 
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_ERROR, PSTR(D_JSON_DESERIALIZATION_ERROR));
    #endif //ENABLE_LOG_LEVEL_COMMANDS
    return;
  }  
  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

  uint8_t default_seconds = 2;
  uint16_t default_freq = 500;
  
  /***
   * As order of importance, others that rely on previous commands must come after
   * */
  
  if(jtok = obj[D_JSON_FREQUENCY]){
    default_freq = jtok.getInt();
  }

  if(jtok = obj[D_JSON_RINGDOORBELL]){
  //   if(jtok.isStr()){
  //     if((tmp_id=mPaletteI->GetPaletteIDbyName(jtok.getStr()))>=0){
  //       CommandSet_PaletteID(tmp_id);
  //       data_buffer.isserviced++;
  //     }
  //   }else
    // if(jtok.isNum()){
    //   CommandSet_FanSpeed_Manual(jtok.getInt());
    //   set_fan_pwm = jtok.getInt();
    //   set_fan_speed = map(jtok.getInt(),1,1023,0,3);
    //   data_buffer.isserviced++;
    // }
    
    RingDoorBellSet(jtok.getInt(), default_freq);
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  }
  

  if(jtok = obj[D_JSON_RINGDOORBELL "_Single"]){
  //   if(jtok.isStr()){
  //     if((tmp_id=mPaletteI->GetPaletteIDbyName(jtok.getStr()))>=0){
  //       CommandSet_PaletteID(tmp_id);
  //       data_buffer.isserviced++;
  //     }
  //   }else
    // if(jtok.isNum()){
    //   CommandSet_FanSpeed_Manual(map(jtok.getInt(),0,3,1,1023)); //fix
    //   set_fan_pwm = map(jtok.getInt(),0,3,1,1023);
    //   set_fan_speed = jtok.getInt();
    //   data_buffer.isserviced++;
    // }
    
    RingDoorBellSet(1, 500);
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  }

  
  mqtthandler_sensor_ifchanged.flags.SendNow = true;

}

#endif // USE_MODULE_LIGHTS_INTERFACE