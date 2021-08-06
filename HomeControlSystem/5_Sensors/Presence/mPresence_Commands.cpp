
#include "mPresence.h" 

/***
 * New joint motion triggered class, all future motion events will also trigger a response from this class (no rules required)
 * */

#ifdef USE_MODULE_SENSORS_PRESENCE 


void mPresence::parse_JSONCommand(JsonParserObject obj){

//   AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_LIGHT D_TOPIC "Checking all commands mAnimatorLight::parse_JSONCommand"));
//   JsonParserToken jtok = 0; 
//   int8_t tmp_id = 0;
//   char buffer[50];

  /**
   *  Flasher function specific commands
   * */
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
