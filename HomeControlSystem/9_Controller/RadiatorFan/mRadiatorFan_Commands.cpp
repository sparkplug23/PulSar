#include "mRadiatorFan.h"

#ifdef USE_MODULE_CONTROLLER_RADIATORFAN


void mRadiatorFan::parse_JSONCommand(JsonParserObject obj){

  // Need to parse on a copy
  // char parsing_buffer[data_buffer.payload.len+1];
  // memcpy(parsing_buffer,data_buffer.payload.ctr,sizeof(char)*data_buffer.payload.len+1);
  // JsonParser parser(parsing_buffer);
  // JsonParserObject obj = parser.getRootObject();   
  // if (!obj) { 
  //   #ifdef ENABLE_LOG_LEVEL_ERROR
  //   AddLog(LOG_LEVEL_ERROR, PSTR(D_JSON_DESERIALIZATION_ERROR));
  //   #endif// ENABLE_LOG_LEVEL_ERROR
  //   return;
  // }  
  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  char buffer[50];

//   #ifdef ENABLE_PIXEL_FUNCTION_EFFECTS
//   /**
//    *  Flasher function specific commands
//    * */
//   if(jtok = obj[PM_JSON_EFFECTS].getObject()[PM_JSON_FUNCTION]){
//     if(jtok.isStr()){
//       if((tmp_id=GetFlasherFunctionIDbyName(jtok.getStr()))>=0){
//         CommandSet_Flasher_FunctionID(tmp_id);
//         data_buffer.isserviced++;
//       }
//     }else
//     if(jtok.isNum()){
//       CommandSet_Flasher_FunctionID(jtok.getInt());
//       data_buffer.isserviced++;
//     }
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_K(D_JSON_FUNCTION)), GetFlasherFunctionName(buffer, sizeof(buffer)));
//     #endif // ENABLE_LOG_LEVEL_DEBUG
//   }

}

#endif