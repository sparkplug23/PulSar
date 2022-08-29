#include "mNextionPanel.h"

#ifdef USE_MODULE_DISPLAYS_NEXTION_V2

//https://haswitchplate.github.io/openHASP-docs/0.6.2/design/objects/#common-properties

// int8_t mNextionPanel::CheckAndExecute_JSONCommands(){

//   // Check if instruction is for me
//   if(mSupport::SetTopicMatch(data_buffer.topic.ctr,D_MODULE_LIGHTS_INTERFACE_CTR)>=0){
//     #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_MQTT D_TOPIC_COMMAND D_MODULE_LIGHTS_INTERFACE_CTR));
//     #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
//     parse_JSONCommand();
//     return FUNCTION_RESULT_HANDLED_ID;
//   }else{
//     return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
//   }

// }

void mNextionPanel::parse_JSONCommand(JsonParserObject obj){

//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_NEXTION D_TOPIC "Checking all commands"));
//   #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS

//   char buffer[50];

//   // Need to parse on a copy
//   char parsing_buffer[data_buffer.payload.len+1];
//   memcpy(parsing_buffer,data_buffer.payload.ctr,sizeof(char)*data_buffer.payload.len+1);


// // sprintf(parsing_buffer, "{[\"three=\\\"THREE\\\"\", \"two=\\\"TWO\\\"\"]}");

// // Serial.println(parsing_buffer);

//   JsonParser parser(parsing_buffer);
//   JsonParserObject obj = parser.getRootObject();   
//   if (!obj) { 
//     #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     AddLog(LOG_LEVEL_ERROR, PSTR(D_JSON_DESERIALIZATION_ERROR));
//     #endif //ENABLE_LOG_LEVEL_COMMANDS
//     return;
//   }  
  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  
  /***
   * As order of importance, others that rely on previous commands must come after
   * */
  

//   if(jtok = obj[PM_JSON_COLOUR_PALETTE]){
//     if(jtok.isStr()){
//       if((tmp_id=mPaletteI->GetPaletteIDbyName(jtok.getStr()))>=0){
//         CommandSet_PaletteID(tmp_id);
//         data_buffer.isserviced++;
//       }
//     }else
//     if(jtok.isNum()){
//       CommandSet_PaletteID(jtok.getInt());
//       data_buffer.isserviced++;
//     }
//     #ifdef ENABLE_LOG_LEVEL_DEBUG
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
//     #endif // ENABLE_LOG_LEVEL_DEBUG
//   }


// int8_t mNextionPanel::parsesub_SetMulti(JsonObjectConst obj){

//   AddLog(LOG_LEVEL_INFO, PSTR("F::%s"),__FUNCTION__);

    // if(jtok = obj["commands"]){

    //     JsonParserArray array = jtok;

    //     Serial.println(array.size());

    //     for(auto val : array) {
    //         // AddLog(LOG_LEVEL_INFO, PSTR("F::%s %s"),__FUNCTION__,val.getStr());

    //         // if(val.isStr()){
    //             Serial.println(val.getStr());
    //         // }else{
    //         //     Serial.println("NOT NOT val.isStr()");
    //         // }

    //         // nextionSendCmd(val.getStr());
    //     }

    // }
  

  
    if(jtok = obj["commands"]){
        JsonParserArray array = jtok;
        for(auto val : array) {
            AddLog(LOG_LEVEL_INFO, PSTR("F::%s %s"),__FUNCTION__,val.getStr());
            nextionSendCmd(val.getStr());
        }
    }
  
/**
 * @brief Construct a new if object
 * I need to add check, if failed (ie due to wrong screen) then stop attempting to update the rest of the messages to stop overflow of UART
 * 
 */
    if(jtok = obj["commands_formatted"]){
        JsonParserArray array = jtok;
        for(auto val : array) {
            ALOG_DBM( PSTR("F::%s %s"),__FUNCTION__,val.getStr());
            // need to add bool for bad response
            nextionSendCmd_ContainingFormattedText(val.getStr());
            data_buffer.isserviced++;
        }
    }


    
    if(jtok = obj["Display"].getObject()["Println"]){
        // JsonParserArray array = jtok;
        // for(auto val : array) {
        //     // AddLog(LOG_LEVEL_INFO, PSTR("F::%s %s"),__FUNCTION__,val.getStr());
        //     nextionSendCmd(val.getStr());
        // }
        Serial2.println(jtok.getStr());
    }


    if(jtok = obj["Display"].getObject()["SetPageIfChanged"]){
      Command_SetPageIfChanged(jtok.getInt());
    }


    
//     if(jtok = obj["commands3"]){
//         //temp fix
//         // JsonParserArray array = jtok;

//         // Serial.println(array.size());

//         // Serial.println(jtok.getStr());
//         // jtok.nextOne();
//         // Serial.println(jtok.getStr());
//         // jtok.nextOne();
//         // Serial.println(jtok.getStr());
//         // jtok.nextOne();
//         // Serial.println(jtok.getStr());
//         // jtok.nextOne();
//         // Serial.println(jtok.getStr());
// DynamicJsonDocument doc(1000);

//  // Deserialize the JSON document
//   DeserializationError error = deserializeJson(doc, JsonBuilderI->GetBufferPtr());

//   // Test if parsing succeeds.
//   if (error) {
//     Serial.print(F("deserializeJson() failed: "));
//     Serial.println(error.f_str());
//     return;
//   }

// JsonObject obj = doc.as<JsonObject>();

// JsonArray arr = obj["commands3"];//.as<JsonArray>();

// // using C++11 syntax (preferred):
// for (JsonVariant value : arr) {
//     Serial.println(value.as<char*>());
// }
  

// //   JsonObjectArray


//         // for(auto val : array) {
//         //     // AddLog(LOG_LEVEL_INFO, PSTR("F::%s %s"),__FUNCTION__,val.getStr());

//         //     // if(val.isStr()){
//         //         Serial.println(val.getStr());
//         //     // }else{
//         //     //     Serial.println("NOT NOT val.isStr()");
//         //     // }

//         //     // nextionSendCmd(val.getStr());
//         // }

//     }



    // jsmntok_t *token_next = &


// }








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
//   //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_NEXTION));
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
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "commands"));    
//     #endif
//     isserviced += parsesub_Commands(obj);
//   }else 
//    // '[...]/device/command/json' -m '["dim=5", "page 1"]' = nextionSendCmd("dim=50"), nextionSendCmd("page 1")
//   if(mSupport::memsearch(data_buffer.topic.ctr,data_buffer.topic.len,"/set_multi",sizeof("/set_multi")-1)>=0){
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "set_multi"));    
//     #endif
//     isserviced += parsesub_SetMulti(obj);
//   }else{
//     AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "INVALID"));    
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





}



std::string& replace(std::string& s, const std::string& from, const std::string& to)
{
    if(!from.empty())
        for(size_t pos = 0; (pos = s.find(from, pos)) != std::string::npos; pos += to.size())
            s.replace(pos, from.size(), to);
    return s;
}




// // int8_t mNextionPanel::Tasker(uint8_t function, JsonParserObject obj), JsonObjectConst obj){
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
// //     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_NEXTION));
// //       pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
// //       parsesub_TopicCheck_JSONCommand(obj);
// //       return FUNCTION_RESULT_HANDLED_ID;
// //   }else{
// //     return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
// //   }

// // }

// int8_t mNextionPanel::parsesub_TopicCheck_JSONCommand(JsonObjectConst obj){

//   AddLog(LOG_LEVEL_INFO,PSTR("mDoorBell::parsesub_TopicCheck_JSONCommand"));

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
//   //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_NEXTION));
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
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "commands"));    
//     #endif
//     isserviced += parsesub_Commands(obj);
//   }else 
//    // '[...]/device/command/json' -m '["dim=5", "page 1"]' = nextionSendCmd("dim=50"), nextionSendCmd("page 1")
//   if(mSupport::memsearch(data_buffer.topic.ctr,data_buffer.topic.len,"/set_multi",sizeof("/set_multi")-1)>=0){
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "set_multi"));    
//     #endif
//     isserviced += parsesub_SetMulti(obj);
//   }else{
//     AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "INVALID"));    
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
  
//   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED D_TOPIC "/commands"));    

//     if(!obj["page"].isNull()){ 
//       AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED "page"));    
//       settings.page = obj["page"];
//       sprintf(command_ctr,"page %d",settings.page);
//       nextionSendCmd(command_ctr);
//     }else
//     if(!obj["command"].isNull()){ 
//       AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED "command"));    
//       const char* command = obj["command"];
//       nextionSendCmd(command);
//     }else
//     if(!obj["statusupdate"].isNull()){ 
//       AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED "statusupdate"));    
//       uint8_t statusupdate = obj["statusupdate"];
//       sprintf(command_ctr,"statusupdate %d",statusupdate);
//       mqtthandler_settings_teleperiod.flags.SendNow = true;
//     }else
//     if(!obj["lcdreboot"].isNull()){ 
//       AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED "lcdreboot"));    
//       uint8_t lcdreboot = obj["lcdreboot"];
//       nextionReset();
//     }else
//     if(!obj["onoff"].isNull()){ 
//       const char* onoff = obj["onoff"];
//       if(strstr(onoff,"ON")){
//         AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),"onoff",D_ON);    
//         nextionSendCmd("dim=dims");
//       }else
//       if(strstr(onoff,"OFF")){
//         AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED "lcdreboot"));     
//         nextionSendCmd("dims=dim");
//         nextionSetAttr("dim", "0");
//       }
//     }else{
//       AddLog(LOG_LEVEL_INFO,PSTR("ELSE FOUND"));
//     }

// }





// int8_t mNextionPanel::parsesub_FlashMessage(){

//   // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION "%s"),"parsesub_FlashMessage");

//   // #ifdef JSONDOCUMENT_STATIC
//   //   StaticJsonDocument<800> doc;
//   // #else
//   //   DynamicJsonDocument doc(600);
//   // #endif
//   // DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
//   // if(error){
//   //   AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_DESERIALIZATION_ERROR));
//   //   Response_mP(S_JSON_COMMAND_SVALUE, D_ERROR,D_JSON_DESERIALIZATION_ERROR);
//   //   return 0;
//   // }
//   // JsonObject obj = doc.as<JsonObject>();

//   // int8_t tmp_id = 0;
  

//   // if(!obj["message"].isNull()){ 
//   //   const char* messagectr = obj["message"];
//   //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),"message",messagectr);
//   //   sprintf(flash_message.message,"%s",messagectr);
//   // }

//   // if(!obj["time_secs"].isNull()){ 
//   //   uint8_t time = obj["time_secs"];
//   //   flash_message.cShowSeconds = time>60?60:time;
//   //   AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),"cShowSeconds",flash_message.cShowSeconds);
//   // }

//   // if(!obj["background_colour"].isNull()){ 
//   //   uint32_t background_colour = obj["background_colour"];
//   //   flash_message.background_colour = background_colour;
//   //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),"background_colour",background_colour);
//   // }

//   // if(!obj["font_colour"].isNull()){ 
//   //   uint32_t font_colour = obj["font_colour"];
//   //   flash_message.font_colour = font_colour;
//   //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),"font_colour",font_colour);
//   // }
  


//   // flash_message.page = PAGE_ERROR_ID;

//   // //Save page
//   // settings.page_saved = settings.page;
//   // //Go to message page
//   // Command_SetPage(flash_message.page);

//   // flash_message.cShowSeconds = 3;

  
//   // // char display_ctr[100];memset(display_ctr,0,sizeof(display_ctr));
//   // // // sprintf(display_ctr,"\"WiFi Connected\\r%s\\rMQTT Connected\\r%s",WiFi.localIP().toString(),"192.168.1.65");
//   // // sprintf(display_ctr,"%s",flash_message.message);
        
//   //   SetAttribute_FontColour(flash_message.page,1,flash_message.font_colour);
//   //   SetAttribute_BackgroundColour(flash_message.page,1,flash_message.background_colour);
//   //   SetAttribute_Txt(flash_message.page,1,flash_message.message);




   
//   // flash_message.isrunning = true;  

//   return 0;
// }


// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void mqttProcessInput(String &strTopic, String &strPayload)
// { // Handle incoming commands from MQTT

//   // strTopic: homeassistant/haswitchplate/devicename/command/p[1].b[4].txt
//   // strPayload: "Lights On"
//   // subTopic: p[1].b[4].txt

//   // Incoming Namespace (replace /device/ with /group/ for group commands)
//   // '[...]/device/command' -m '' == No command requested, respond with mqttStatusUpdate()
//   // '[...]/device/command' -m 'dim=50' == nextionSendCmd("dim=50")
//   // '[...]/device/command/json' -m '["dim=5", "page 1"]' == nextionSendCmd("dim=50"), nextionSendCmd("page 1")
//   // '[...]/device/command/p[1].b[4].txt' -m '' == nextionGetAttr("p[1].b[4].txt")
//   // '[...]/device/command/p[1].b[4].txt' -m '"Lights On"' == nextionSetAttr("p[1].b[4].txt", "\"Lights On\"")
//   // '[...]/device/brightness/set' -m '50' == nextionSendCmd("dims=50")
//   // '[...]/device/light/switch' -m 'OFF' == nextionSendCmd("dims=0")
//   // '[...]/device/command/page' -m '1' == nextionSendCmd("page 1")
//   // '[...]/device/command/statusupdate' -m '' == mqttStatusUpdate()
//   // '[...]/device/command/discovery' -m '' == call mqttDiscovery()
//   // '[...]/device/command/lcdupdate' -m 'http://192.168.0.10/local/HASwitchPlate.tft' == nextionOtaStartDownload("http://192.168.0.10/local/HASwitchPlate.tft")
//   // '[...]/device/command/lcdupdate' -m '' == nextionOtaStartDownload("lcdFirmwareUrl")
//   // '[...]/device/command/espupdate' -m 'http://192.168.0.10/local/HASwitchPlate.ino.d1_mini.bin' == espStartOta("http://192.168.0.10/local/HASwitchPlate.ino.d1_mini.bin")
//   // '[...]/device/command/espupdate' -m '' == espStartOta("espFirmwareUrl")
//   // '[...]/device/command/beep' -m '100,200,3' == beep on for 100msec, off for 200msec, repeat 3 times
//   // '[...]/device/command/hassdiscovery' -m 'homeassistant' == hassDiscovery = homeassistant
//   // '[...]/device/command/nextionmaxpages' -m '11' == nextionmaxpages = 11
//   // '[...]/device/command/nextionbaud' -m '921600' == nextionBaud = 921600
//   // '[...]/device/command/debugserialenabled' -m 'true' == enable serial debug output
//   // '[...]/device/command/debugtelnetenabled' -m 'true' == enable telnet debug output
//   // '[...]/device/command/mdnsenabled' -m 'true' == enable mDNS responder
//   // '[...]/device/command/beepenabled' -m 'true' == enable beep output on keypress
//   // '[...]/device/command/ignoretouchwhenoff' -m 'true' == disable actions on keypress

//   debugPrintln(String(F("MQTT IN: '")) + strTopic + String(F("' : '")) + strPayload + String(F("'")));

//   if (((strTopic == mqttCommandTopic) || (strTopic == mqttGroupCommandTopic)) && (strPayload == ""))
//   {                     // '[...]/device/command' -m '' = No command requested, respond with mqttStatusUpdate()
//     mqttStatusUpdate(); // return status JSON via MQTT
//   }
//   else if (strTopic == mqttCommandTopic || strTopic == mqttGroupCommandTopic)
//   { // '[...]/device/command' -m 'dim=50' == nextionSendCmd("dim=50")
//     nextionSendCmd(strPayload);
//   }
//   else if (strTopic == (mqttCommandTopic + "/page") || strTopic == (mqttGroupCommandTopic + "/page"))
//   { // '[...]/device/command/page' -m '1' == nextionSendCmd("page 1")
//     if (strPayload == "")
//     {
//       nextionSendCmd("sendme");
//     }
//     else
//     {
//       nextionActivePage = strPayload.toInt();
//       nextionSendCmd("page " + strPayload);
//     }
//   }
//   else if (strTopic == (mqttCommandTopic + "/json") || strTopic == (mqttGroupCommandTopic + "/json"))
//   { // '[...]/device/command/json' -m '["dim=5", "page 1"]' = nextionSendCmd("dim=50"), nextionSendCmd("page 1")
//     if (strPayload != "")
//     {
//       nextionParseJson(strPayload); // Send to nextionParseJson()
//     }
//   }
//   else if (strTopic == (mqttCommandTopic + "/statusupdate") || strTopic == (mqttGroupCommandTopic + "/statusupdate"))
//   {                     // '[...]/device/command/statusupdate' == mqttStatusUpdate()
//     mqttStatusUpdate(); // return status JSON via MQTT
//   }
//   else if (strTopic == (mqttCommandTopic + "/discovery") || strTopic == (mqttGroupCommandTopic + "/discovery"))
//   {                  // '[...]/device/command/discovery' == mqttDiscovery()
//     mqttDiscovery(); // send Home Assistant discovery message via MQTT
//   }
//   else if (strTopic == (mqttCommandTopic + "/hassdiscovery") || strTopic == (mqttGroupCommandTopic + "/hassdiscovery"))
//   {                                             // '[...]/device/command/hassdiscovery' -m 'homeassistant' == hassDiscovery = homeassistant
//     strPayload.toCharArray(hassDiscovery, 128); // set hassDiscovery to value provided in payload
//     configSave();
//     mqttDiscovery(); // send Home Assistant discovery message on new discovery topic via MQTT
//   }
//   else if ((strTopic == (mqttCommandTopic + "/nextionmaxpages") || strTopic == (mqttGroupCommandTopic + "/nextionmaxpages")) && (strPayload.toInt() < 256) && (strPayload.toInt() > 0))
//   {                                       // '[...]/device/command/nextionmaxpages' -m '11' == nextionmaxpages = 11
//     nextionMaxPages = strPayload.toInt(); // set nextionMaxPages to value provided in payload
//     configSave();
//     mqttDiscovery(); // send Home Assistant discovery message via MQTT
//   }
//   else if ((strTopic == (mqttCommandTopic + "/nextionbaud") || strTopic == (mqttGroupCommandTopic + "/nextionbaud")) &&
//            ((strPayload.toInt() == 2400) ||
//             (strPayload.toInt() == 4800) ||
//             (strPayload.toInt() == 9600) ||
//             (strPayload.toInt() == 19200) ||
//             (strPayload.toInt() == 31250) ||
//             (strPayload.toInt() == 38400) ||
//             (strPayload.toInt() == 57600) ||
//             (strPayload.toInt() == 115200) ||
//             (strPayload.toInt() == 230400) ||
//             (strPayload.toInt() == 250000) ||
//             (strPayload.toInt() == 256000) ||
//             (strPayload.toInt() == 512000) ||
//             (strPayload.toInt() == 921600)))
//   {                                         // '[...]/device/command/nextionbaud' -m '921600' == nextionBaud = 921600
//     strPayload.toCharArray(nextionBaud, 7); // set nextionBaud to value provided in payload
//     nextionAckEnable = false;
//     nextionSendCmd("bauds=" + strPayload); // send baud rate to nextion
//     nextionAckEnable = true;
//     Serial.flush();
//     Serial1.flush();
//     Serial.end();
//     Serial1.end();
//     Serial.begin(atoi(nextionBaud));  // Serial - LCD RX (after swap), debug TX
//     Serial1.begin(atoi(nextionBaud)); // Serial1 - LCD TX, no RX
//     Serial.swap();                    // Swap to allow hardware UART comms to LCD
//     configSave();
//   }
//   else if (strTopic == (mqttCommandTopic + "/debugserialenabled") || strTopic == (mqttGroupCommandTopic + "/debugserialenabled"))
//   {                                             // '[...]/device/command/debugserialenabled' -m 'true' == enable serial debug output
//     if (strPayload.equalsIgnoreCase("true"))
//     {
//       debugSerialEnabled = true;
//       configSave();
//     }
//     else if(strPayload.equalsIgnoreCase("false"))
//     {
//       debugSerialEnabled = false;
//       configSave();
//     }    
//   }
//   else if (strTopic == (mqttCommandTopic + "/debugtelnetenabled") || strTopic == (mqttGroupCommandTopic + "/debugtelnetenabled"))
//   {                                             // '[...]/device/command/debugtelnetenabled' -m 'true' == enable telnet debug output
//     if (strPayload.equalsIgnoreCase("true"))
//     {
//       debugTelnetEnabled = true;
//       configSave();
//     }
//     else if(strPayload.equalsIgnoreCase("false"))
//     {
//       debugTelnetEnabled = false;
//       configSave();
//     }    
//   }
//   else if (strTopic == (mqttCommandTopic + "/mdnsenabled") || strTopic == (mqttGroupCommandTopic + "/mdnsenabled"))
//   {                                             // '[...]/device/command/mdnsenabled' -m 'true' == enable mDNS responder
//     if (strPayload.equalsIgnoreCase("true"))
//     {
//       mdnsEnabled = true;
//       configSave();
//     }
//     else if(strPayload.equalsIgnoreCase("false"))
//     {
//       mdnsEnabled = false;
//       configSave();
//     }    
//   }
//   else if (strTopic == (mqttCommandTopic + "/beepenabled") || strTopic == (mqttGroupCommandTopic + "/beepenabled"))
//   {                                             // '[...]/device/command/beepenabled' -m 'true' == enable beep output on keypress
//     if (strPayload.equalsIgnoreCase("true"))
//     {
//       beepEnabled = true;
//       configSave();
//     }
//     else if(strPayload.equalsIgnoreCase("false"))
//     {
//       beepEnabled = false;
//       configSave();
//     }    
//   }
//   else if (strTopic == (mqttCommandTopic + "/ignoretouchwhenoff") || strTopic == (mqttGroupCommandTopic + "/ignoretouchwhenoff"))
//   {                                             // '[...]/device/command/ignoretouchwhenoff' -m 'true' == disable actions on keypress
//     if (strPayload.equalsIgnoreCase("true"))
//     {
//       ignoreTouchWhenOff = true;
//       configSave();
//     }
//     else if(strPayload.equalsIgnoreCase("false"))
//     {
//       ignoreTouchWhenOff = false;
//       configSave();
//     }    
//   }
//   else if (strTopic == (mqttCommandTopic + "/lcdupdate") || strTopic == (mqttGroupCommandTopic + "/lcdupdate"))
//   { // '[...]/device/command/lcdupdate' -m 'http://192.168.0.10/local/HASwitchPlate.tft' == nextionOtaStartDownload("http://192.168.0.10/local/HASwitchPlate.tft")
//     if (strPayload == "")
//     {
//       nextionOtaStartDownload(lcdFirmwareUrl);
//     }
//     else
//     {
//       nextionOtaStartDownload(strPayload);
//     }
//   }
//   else if (strTopic == (mqttCommandTopic + "/espupdate") || strTopic == (mqttGroupCommandTopic + "/espupdate"))
//   { // '[...]/device/command/espupdate' -m 'http://192.168.0.10/local/HASwitchPlate.ino.d1_mini.bin' == espStartOta("http://192.168.0.10/local/HASwitchPlate.ino.d1_mini.bin")
//     if (strPayload == "")
//     {
//       espStartOta(espFirmwareUrl);
//     }
//     else
//     {
//       espStartOta(strPayload);
//     }
//   }
//   else if (strTopic == (mqttCommandTopic + "/reboot") || strTopic == (mqttGroupCommandTopic + "/reboot"))
//   { // '[...]/device/command/reboot' == reboot microcontroller
//     debugPrintln(F("MQTT: Rebooting device"));
//     espReset();
//   }
//   else if (strTopic == (mqttCommandTopic + "/lcdreboot") || strTopic == (mqttGroupCommandTopic + "/lcdreboot"))
//   { // '[...]/device/command/lcdreboot' == reboot LCD panel
//     debugPrintln(F("MQTT: Rebooting LCD"));
//     nextionReset();
//   }
//   else if (strTopic == (mqttCommandTopic + "/factoryreset") || strTopic == (mqttGroupCommandTopic + "/factoryreset"))
//   { // '[...]/device/command/factoryreset' == clear all saved settings
//     configClearSaved();
//   }
//   else if (strTopic == (mqttCommandTopic + "/beep") || strTopic == (mqttGroupCommandTopic + "/beep"))
//   { // '[...]/device/command/beep' == activate beep function
//     String mqttvar1 = getSubtringField(strPayload, ',', 0);
//     String mqttvar2 = getSubtringField(strPayload, ',', 1);
//     String mqttvar3 = getSubtringField(strPayload, ',', 2);

//     beepOnTime = mqttvar1.toInt();
//     beepOffTime = mqttvar2.toInt();
//     beepCounter = mqttvar3.toInt();
//   }
//   else if (strTopic == (mqttCommandTopic + "/crashtest"))
//   { // '[...]/device/command/crashtest' -m 'divzero' == divide by zero
//     if (strPayload == "divzero")
//     {
//       debugPrintln(String(F("DEBUG: attempt to divide by zero")));
//       int result, zero;
//       zero = 0;
//       result = 1 / zero;
//       debugPrintln(String(F("DEBUG: div zero result: ")) + String(result));
//     }
//     else if (strPayload == "nullptr")
//     { // '[...]/device/command/crashtest' -m 'nullptr' == dereference a null pointer
//       debugPrintln(String(F("DEBUG: attempt to dereference null pointer")));
//       int *nullPointer = NULL;
//       debugPrintln(String(F("DEBUG: dereference null pointer: ")) + String(*nullPointer));
//     }
//     else if (strPayload == "wdt")
//     { // '[...]/device/command/crashtest' -m 'wdt' == trigger soft WDT
//       debugPrintln(String(F("DEBUG: enter tight loop and cause WDT")));
//       while (true)
//       {
//       }
//     }
//   }
//   else if (strTopic.startsWith(mqttCommandTopic) && (strPayload == ""))
//   { // '[...]/device/command/p[1].b[4].txt' -m '' == nextionGetAttr("p[1].b[4].txt")
//     String subTopic = strTopic.substring(mqttCommandTopic.length() + 1);
//     mqttGetSubtopic = "/" + subTopic;
//     nextionGetAttr(subTopic);
//   }
//   else if (strTopic.startsWith(mqttGroupCommandTopic) && (strPayload == ""))
//   { // '[...]/group/command/p[1].b[4].txt' -m '' == nextionGetAttr("p[1].b[4].txt")
//     String subTopic = strTopic.substring(mqttGroupCommandTopic.length() + 1);
//     mqttGetSubtopic = "/" + subTopic;
//     nextionGetAttr(subTopic);
//   }
//   else if (strTopic.startsWith(mqttCommandTopic))
//   { // '[...]/device/command/p[1].b[4].txt' -m '"Lights On"' == nextionSetAttr("p[1].b[4].txt", "\"Lights On\"")
//     String subTopic = strTopic.substring(mqttCommandTopic.length() + 1);
//     nextionSetAttr(subTopic, strPayload);
//   }
//   else if (strTopic.startsWith(mqttGroupCommandTopic))
//   { // '[...]/group/command/p[1].b[4].txt' -m '"Lights On"' == nextionSetAttr("p[1].b[4].txt", "\"Lights On\"")
//     String subTopic = strTopic.substring(mqttGroupCommandTopic.length() + 1);
//     nextionSetAttr(subTopic, strPayload);
//   }
//   else if (strTopic == mqttLightBrightCommandTopic)
//   { // change the brightness from the light topic
//     nextionSetAttr("dim", strPayload);
//     nextionSetAttr("dims", "dim");
//     lcdBacklightDim = strPayload.toInt();
//     debugPrintln(String(F("MQTT OUT: '")) + mqttLightBrightStateTopic + String(F("' : '")) + strPayload + String(F("'")));
//     mqttClient.publish(mqttLightBrightStateTopic, strPayload, true, 0);
//   }
//   else if (strTopic == mqttLightCommandTopic && strPayload == "OFF")
//   { // set the panel dim OFF from the light topic, saving current dim level first
//     nextionSetAttr("dims", "dim");
//     nextionSetAttr("dim", "0");
//     lcdBacklightOn = 0;
//     debugPrintln(String(F("MQTT OUT: '")) + mqttLightStateTopic + String(F("' : 'OFF'")));
//     mqttClient.publish(mqttLightStateTopic, "OFF", true, 0);
//   }
//   else if (strTopic == mqttLightCommandTopic && strPayload == "ON")
//   { // set the panel dim ON from the light topic, restoring saved dim level
//     nextionSetAttr("dim", "dims");
//     nextionSetAttr("sleep", "0");
//     lcdBacklightOn = 1;
//     debugPrintln(String(F("MQTT OUT: '")) + mqttLightStateTopic + String(F("' : 'ON'")));
//     mqttClient.publish(mqttLightStateTopic, "ON", true, 0);
//   }
//   else if (strTopic == mqttStatusTopic && strPayload == "OFF")
//   { // catch a dangling LWT from a previous connection if it appears
//     debugPrintln(String(F("MQTT OUT: '")) + mqttStatusTopic + String(F("' : 'ON'")));
//     mqttClient.publish(mqttStatusTopic, "ON", true, 0);
//     mqttClient.publish(mqttStateJSONTopic, String(F("{\"event_type\":\"hasp_device\",\"event\":\"online\"}")));
//     debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F(" : {\"event_type\":\"hasp_device\",\"event\":\"online\"}")));
//   }
// }


#endif// USE_MODULE_DISPLAYS_NEXTION

/****
 * Example MQTT Commands
 * 
 * 
 * 
 * 
 * 
 * 
{
  "commands": [
    "p[9].b[1].txt=\"hello\""
  ]
}

{
  "commands": [
    "page 5"
  ]
}


 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * */