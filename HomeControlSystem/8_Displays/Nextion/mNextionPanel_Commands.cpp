#include "mNextionPanel.h"


// // int8_t mNextionPanel::Tasker(uint8_t function, JsonObjectConst obj){
// //   switch(function){
// //     case FUNC_JSON_COMMAND_OBJECT:
// //       parse_JSONCommand(obj);
// //     break;
// //     case FUNC_JSON_COMMAND_OBJECT_WITH_TOPIC:
// //       return CheckAndExecute_JSONCommands(obj);
// //     break;
// //   }
// // }


// // int8_t mNextionPanel::CheckAndExecute_JSONCommands(JsonObjectConst obj){

// //   // Check if instruction is for me
// //   if(mSupport::mSearchCtrIndexOf(data_buffer.topic.ctr,"set/nextion")>=0){
// //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_NEXTION));
// //       pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
// //       parsesub_TopicCheck_JSONCommand(obj);
// //       return FUNCTION_RESULT_HANDLED_ID;
// //   }else{
// //     return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
// //   }

// // }

// int8_t mNextionPanel::parsesub_TopicCheck_JSONCommand(JsonObjectConst obj){

//   AddLog_P(LOG_LEVEL_INFO,PSTR("mDoorBell::parsesub_TopicCheck_JSONCommand"));

//   uint8_t name_num=-1,state=-1;    


// ////////////////////////////////////////////////////////////////////////////////////////////////////
// // Incoming Namespace (replace /device/ with /group/ for group commands)
// // '[...]/device/command' -m 'dim=50' = nextionSendCmd("dim=50")
// // '[...]/device/command/json' -m '["dim=5", "page 1"]' = nextionSendCmd("dim=50"), nextionSendCmd("page 1")
// // '[...]/device/command/page' -m '1' = nextionSendCmd("page 1")
// // '[...]/device/command/lcdupdate' -m 'http://192.168.0.10/local/HASwitchPlate.tft' = nextionStartOtaDownload("http://192.168.0.10/local/HASwitchPlate.tft")
// // '[...]/device/command/lcdupdate' -m '' = nextionStartOtaDownload("lcdFirmwareUrl")
// // '[...]/device/command/espupdate' -m 'http://192.168.0.10/local/HASwitchPlate.ino.d1_mini.bin' = espStartOta("http://192.168.0.10/local/HASwitchPlate.ino.d1_mini.bin")
// // '[...]/device/command/espupdate' -m '' = espStartOta("espFirmwareUrl")
// // '[...]/device/command/p[1].b[4].txt' -m '' = nextionGetAttr("p[1].b[4].txt")
// // '[...]/device/command/p[1].b[4].txt' -m '"Lights On"' = nextionSetAttr("p[1].b[4].txt", "\"Lights On\"")
// // uint8_t mNextionPanel::parse_JSONCommand(){

//   // Check if instruction is for me
//   // if(mSupport::mSearchCtrIndexOf(data_buffer.topic.ctr,"set/nextion")>=0){
//   //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_NEXTION));
//   //   pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
//   //   fOpenHABDataStreamActive_last_secs = 1; // set to be positive to start
//   //   fOpenHABDataStreamActive = true;
//   // }else{
//   //   return 0; // not meant for here
//   // }



  
// //these need to be subtopic functions



//   // if(strstr(data_buffer.topic.ctr,"/commands")){ 
//   // '[...]/device/command/page' -m '1' == nextionSendCmd("page 1")
//   if(mSupport::memsearch(data_buffer.topic.ctr,data_buffer.topic.len,"/commands",sizeof("/commands")-1)>=0){
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "commands"));    
//     #endif
//     isserviced += parsesub_Commands(obj);
//   }else 
//    // '[...]/device/command/json' -m '["dim=5", "page 1"]' = nextionSendCmd("dim=50"), nextionSendCmd("page 1")
//   if(mSupport::memsearch(data_buffer.topic.ctr,data_buffer.topic.len,"/set_multi",sizeof("/set_multi")-1)>=0){
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "set_multi"));    
//     #endif
//     isserviced += parsesub_SetMulti(obj);
//   }else{
//     AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "INVALID"));    
//   }  
  
  
//   // if(strstr(data_buffer.topic.ctr,"/set_single")){
    
//   //   StaticJsonDocument<300> doc;
//   //   DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
//   //   JsonObject obj = doc.as<JsonObject>();

//   //   if((!obj["attribute"].isNull())&&(!obj["value"].isNull())){ 
//   //     const char* attribute = obj["attribute"];
//   //     const char* value = obj["value"];
//   //     nextionSetAttr(attribute,value);
//   //   }

//   // }else
//   // // Get element (ask, no value)
//   // if(strstr(data_buffer.topic.ctr,"/get_single")){
    
//   //   StaticJsonDocument<300> doc;
//   //   DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
//   //   JsonObject obj = doc.as<JsonObject>();

//   //   if(!obj["attribute"].isNull()){ 
//   //     const char* attribute = obj["attribute"];
//   //     nextionGetAttr(attribute);
//   //   }

//   // }else
//   // // Get element (ask, no value)
//   // if(strstr(data_buffer.topic.ctr,"/nextion/flash_message")){
//   //   isserviced += parsesub_FlashMessage();
//   // }

//   // else if (strTopic == (mqttCommandTopic + "/lcdupdate") || strTopic == (mqttGroupCommandTopic + "/lcdupdate"))
//   // { // '[...]/device/command/lcdupdate' -m 'http://192.168.0.10/local/HASwitchPlate.tft' 
//   //== nextionStartOtaDownload("http://192.168.0.10/local/HASwitchPlate.tft")
//   //   if (strPayload == ""){
//   //     nextionStartOtaDownload(lcdFirmwareUrl);
//   //   }else{
//   //     nextionStartOtaDownload(strPayload);
//   //   }
//   // }
//   // else if (strTopic == (mqttCommandTopic + "/espupdate") || strTopic == (mqttGroupCommandTopic + "/espupdate"))
//   // { // '[...]/device/command/espupdate' -m 'http://192.168.0.10/local/HASwitchPlate.ino.d1_mini.bin' 
//   // == espStartOta("http://192.168.0.10/local/HASwitchPlate.ino.d1_mini.bin")
//   //   if (strPayload == ""){
//   //     espStartOta(espFirmwareUrl);
//   //   }else{
//   //     espStartOta(strPayload);
//   //   }
//   // }

//   data_buffer.isserviced += isserviced;
 
// //  



// }




// int8_t mNextionPanel::parsesub_Commands(JsonObjectConst obj){

//   char command_ctr[100]; memset(command_ctr,0,sizeof(command_ctr));
  
//   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED D_TOPIC "/commands"));    

//     if(!obj["page"].isNull()){ 
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED "page"));    
//       settings.page = obj["page"];
//       sprintf(command_ctr,"page %d",settings.page);
//       nextionSendCmd(command_ctr);
//     }else
//     if(!obj["command"].isNull()){ 
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED "command"));    
//       const char* command = obj["command"];
//       nextionSendCmd(command);
//     }else
//     if(!obj["statusupdate"].isNull()){ 
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED "statusupdate"));    
//       uint8_t statusupdate = obj["statusupdate"];
//       sprintf(command_ctr,"statusupdate %d",statusupdate);
//       mqtthandler_settings_teleperiod.flags.SendNow = true;
//     }else
//     if(!obj["brightness"].isNull()){ 
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED "brightness"));    
//       uint8_t brightness = obj["brightness"];
//       //nextionSetAttr("dim", String(brightness));
//       //sprintf(command_ctr,"dims=%d",brightness);
//       nextionSendCmd("dims=dim");
//     }else
//     if(!obj["lcdreboot"].isNull()){ 
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED "lcdreboot"));    
//       uint8_t lcdreboot = obj["lcdreboot"];
//       nextionReset();
//     }else
//     if(!obj["onoff"].isNull()){ 
//       const char* onoff = obj["onoff"];
//       if(strstr(onoff,"ON")){
//         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),"onoff",D_ON);    
//         nextionSendCmd("dim=dims");
//       }else
//       if(strstr(onoff,"OFF")){
//         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED "lcdreboot"));     
//         nextionSendCmd("dims=dim");
//         nextionSetAttr("dim", "0");
//       }
//     }else{
//       AddLog_P(LOG_LEVEL_INFO,PSTR("ELSE FOUND"));
//     }

// }



// int8_t mNextionPanel::parsesub_SetMulti(JsonObjectConst obj){

//   AddLog_P(LOG_LEVEL_INFO, PSTR("F::%s"),__FUNCTION__);

//   if(!obj["commands"].isNull()){
//     JsonArrayConst array = obj["commands"];
//     for(JsonVariantConst val : array) {
//       nextionSendCmd(val.as<const char*>());
//     }
//   }
  
// }