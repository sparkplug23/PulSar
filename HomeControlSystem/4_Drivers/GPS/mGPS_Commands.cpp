#include "mGPS.h"

#ifdef USE_MODULE_DRIVERS_GPS

int8_t mGPS::CheckAndExecute_JSONCommands(){

  // Check if instruction is for me
  if(mSupport::SetTopicMatch(data_buffer.topic.ctr,D_MODULE_CUSTOM_FAN_FRIENDLY_CTR)>=0){
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_MQTT D_TOPIC_COMMAND D_MODULE_CUSTOM_FAN_FRIENDLY_CTR));
    #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
    pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
    parse_JSONCommand();
    return FUNCTION_RESULT_HANDLED_ID;
  }else{
    return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
  }

}

void mGPS::parse_JSONCommand(void){

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
    

  // if(jtok = obj["ListDir"]){

  //   CommandSet_SerialPrint_FileNames(jtok.getStr());
    
  // }
  

  // if(jtok = obj["WriteFile"]){

  //   // Also check for datafile
  //   JsonParserToken jtok_data = obj["DataFile"];
  //   if(!jtok_data.isNull()){
  //     CommandSet_WriteFile(jtok.getStr(), jtok_data.getStr());
  //   }
  //   else{
  //     CommandSet_WriteFile(jtok.getStr());
  //   }


  // //   if(jtok.isStr()){
  // //     if((tmp_id=mPaletteI->GetPaletteIDbyName(jtok.getStr()))>=0){
  // //       CommandSet_PaletteID(tmp_id);
  // //       data_buffer.isserviced++;
  // //     }
  // //   }else
  //   // if(jtok.isNum()){
  //   //   CommandSet_FanSpeed_Manual(map(jtok.getInt(),0,3,1,1023)); //fix
  //   //   set_fan_pwm = map(jtok.getInt(),0,3,1,1023);
  //   //   set_fan_speed = jtok.getInt();
  //   //   data_buffer.isserviced++;
  //   // }
  // //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  // //   AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
  // //   #endif // ENABLE_LOG_LEVEL_DEBUG
  // }
  
  // if(jtok = obj["ReadFile"]){

  //   CommandSet_ReadFile(jtok.getStr());
    
  // }
  
  mqtthandler_sensor_ifchanged.flags.SendNow = true;

}


/******************************************************************************************************************************
*******************************************************************************************************************************
****************** CommandSet_ReadFile *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mGPS::CommandSet_ReadFile(const char* filename){

  // readFile(SD_MMC, filename);

  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_SDCARD D_JSON_COMMAND_SVALUE_K("ReadFile")), filename);
  // #endif // ENABLE_LOG_LEVEL_COMMANDS

} 


/******************************************************************************************************************************
*******************************************************************************************************************************
****************** CommandSet_WriteFile *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mGPS::CommandSet_WriteFile(const char* filename, const char* data){

  // char file//name_with_extention[50];
  // sprintf(filename_with_extention, "%s.txt", filename);
  // if(data == nullptr){
  //   writeFile(SD_MMC, filename_with_extention, "Empty File!");
  // }else{
  //   writeFile(SD_MMC, filename_with_extention, "Hello ");
  // }

  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_SDCARD D_JSON_COMMAND_SVALUE_K("WriteFile")), filename_with_extention);
  // #endif // ///ENABLE_LOG_LEVEL_COMMANDS

} 


/******************************************************************************************************************************
*******************************************************************************************************************************
****************** PixelHardwareType *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mGPS::CommandSet_CreateFile_WithName(char* value){

// //   analogWrite(pin, value);
//   createDir(SD_MMC, "/mydir");

// // AddLog_P(LOG_LEVEL_TEST,PSTR("pwm %d value = %d"),pin,value);


//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   // AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_SDCARD D_JSON_COMMAND_SVALUE_K("ListDir")), dirname);
//   #endif // ENABLE_LOG_LEVEL_COMMANDS

} 

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** PixelHardwareType *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mGPS::CommandSet_SerialPrint_FileNames(const char* dirname){

  // // AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_SDCARD D_JSON_COMMAND_SVALUE_K("TESTListDir")), dirname);
  // listDir(SD_MMC, dirname, 0);

  // // listDir(SD_MMC, "/", 0);

  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_SDCARD D_JSON_COMMAND_SVALUE_K("ListDir")), dirname);
  // #endif // ENABLE_LOG_LEVEL_COMMANDS

} 

#endif // USE_MODULE_LIGHTS_INTERFACE
