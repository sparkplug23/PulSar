#include "mSecurityLight.h"

#ifdef USE_MODULE_CUSTOM_SECURITY_LIGHT


int8_t mSecurityLight::CheckAndExecute_JSONCommands(){

  // Check if instruction is for me
  if(mSupport::SetTopicMatch(data_buffer.topic.ctr,D_MODULE_CUSTOM_SECURITYLIGHT_FRIENDLY_CTR)>=0){
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    // AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_PIXELS));
    #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
    pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
    parse_JSONCommand();
    return FUNCTION_RESULT_HANDLED_ID;
  }else{
    return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
  }

}


void mSecurityLight::parse_JSONCommand(void){

  // Need to parse on a copy
  char parsing_buffer[data_buffer.payload.len+1];
  memcpy(parsing_buffer,data_buffer.payload.ctr,sizeof(char)*data_buffer.payload.len+1);
  AddLog_P(LOG_LEVEL_TEST, PSTR("\"%s\""),parsing_buffer);
  JsonParser parser(parsing_buffer);
  JsonParserObject obj = parser.getRootObject();   
  if (!obj) { 
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_ERROR, PSTR("DeserializationError with \"%s\""),parsing_buffer);
    #endif// ENABLE_LOG_LEVEL_INFO
    return;
  }  
  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

  // int8_t device_id = -1;
  // int8_t isserviced=-1;

  // if((device_id = pCONT->mry->GetRelayIDbyName(obj[D_JSON_DEVICE]))>=0){
  //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_GARAGE D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_DEVICE,device_id);
  //   Response_mP(S_JSON_COMMAND_NVALUE, D_DEVICE,device_id);
  // }else{
  //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_GARAGE D_PARSING_NOMATCH));
  //   // Response_mP(S_JSON_COMMAND_SVALUE, D_DEVICE,D_PARSING_NOMATCH);
  //   return ; // Unknown device, can't execute
  // }

  // switch(device_id){
  //   case LIGHT_DRIVEWAY_ID: light_control_ptr = &light_control_driveway; break;
  //   case LIGHT_GARDEN_ID: light_control_ptr = &light_control_garden; break;
  // }

  // if(obj.containsKey(D_JSON_TIME_ON)){
  //   light_control_ptr->seconds_on = obj[D_JSON_TIME_ON];
  //   AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_GARAGE D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_TIME_ON,light_control_ptr->seconds_on);
  //   Response_mP(S_JSON_COMMAND_NVALUE, D_JSON_TIME_ON,light_control_ptr->seconds_on);
  //   isserviced++;
  // }

  // if(obj.containsKey(D_JSON_ONOFF)){
  //   const char* onoff = obj[D_JSON_ONOFF];
  //   if(strstr(onoff,"ON")){ 
  //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_GARAGE D_PARSING_MATCHED "\"onoff\"=\"ON\""));
  //     SetLight(device_id,ON);
  //     isserviced++;
  //   }else if(strstr(onoff,"OFF")){
  //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_GARAGE D_PARSING_MATCHED "\"onoff\"=\"OFF\""));
  //     SetLight(device_id,OFF);
  //     isserviced++;
  //   }else{
  //     AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_GARAGE D_PARSING_NOMATCH));
  //   }
  //   AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_GARAGE D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_ONOFF,onoff);
  //   Response_mP(S_JSON_COMMAND_SVALUE,D_JSON_ONOFF,onoff);
  //   isserviced++;
  // }

}


#endif
