#include "mSDCard.h"

#ifdef USE_MODULE_DRIVERS_SDCARD


void mSDCard::parse_JSONCommand(JsonParserObject obj){

  JsonParserToken jtok = 0; 


  if(jtok = obj["ListDir"]){

    CommandSet_SerialPrint_FileNames(jtok.getStr());
    
  }
  

  if(jtok = obj["WriteFile"]){

    // Also check for datafile
    JsonParserToken jtok_data = obj["DataFile"];
    if(!jtok_data.isNull()){
      CommandSet_WriteFile(jtok.getStr(), jtok_data.getStr());
    }
    else{
      CommandSet_WriteFile(jtok.getStr());
    }


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
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  }
  
  if(jtok = obj["ReadFile"]){

    CommandSet_ReadFile(jtok.getStr());
    
  }


  /**
   * Set flag, write 100 single bytes, 1 per second
   * */
  if(jtok = obj["Debug"].getObject()["WriteTest"]){

    // CommandSet_ReadFile(jtok.getStr());
    debug.bytes_to_write = jtok.getInt();
    debug.test_mode = WRITE_BYTES_ID;

  }

  if(jtok = obj["Debug"].getObject()["OpenFile"]){

    // CommandSet_ReadFile(jtok.getStr());
    // debug.bytes_to_write = jtok.getInt();
    // debug.test_mode = WRITE_BYTES_ID;
    char buffer[5] = {0,1,2,3,4};
    uint8_t close_decounter = 10;
    writer_settings.status = FILE_STATUS_OPENING_ID;
    SubTask_Append_To_Open_File(buffer, 5);

  }

  

  // /**
  //  * Set flag, write 512 bytes, same time
  //  * */
  // if(jtok = obj["Debug"].getObject()["LargeWriteTest"]){

  //   // CommandSet_ReadFile(jtok.getStr());

  // }
  
  #ifdef USE_MODULE_NETWORK_MQTT
  MQTTHandler_Set_fSendNow();
  #endif// USE_MODULE_NETWORK_MQTT

}



void mSDCard::CommandSet_SDCard_Appending_File_Method_State(uint8_t state)
{

  if(state == 2) //toggle
  {
    sdcard_status.isopened ^= 1; 
    AddLog(LOG_LEVEL_TEST, PSTR("CommandSet_LoggingState sdcard_status.isopened == 2, %d"),sdcard_status.isopened);

  }else
  {
    sdcard_status.isopened = state;
    AddLog(LOG_LEVEL_TEST, PSTR("CommandSet_LoggingState sdcard_status.isopened = state,  %d"),sdcard_status.isopened);

  }

}















/******************************************************************************************************************************
*******************************************************************************************************************************
****************** CommandSet_ReadFile *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mSDCard::CommandSet_ReadFile(const char* filename){

  readFile(SD, filename);

  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_SDCARD D_JSON_COMMAND_SVALUE_K("ReadFile")), filename);
  #endif // ENABLE_LOG_LEVEL_COMMANDS

} 


/******************************************************************************************************************************
*******************************************************************************************************************************
****************** CommandSet_WriteFile *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mSDCard::CommandSet_WriteFile(const char* filename, const char* data){

  char filename_with_extention[50];
  sprintf(filename_with_extention, "%s.txt", filename);
  if(data == nullptr){
    writeFile(SD, filename_with_extention, "Empty File!");
  }else{
    writeFile(SD, filename_with_extention, "Hello ");
  }

  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_SDCARD D_JSON_COMMAND_SVALUE_K("WriteFile")), filename_with_extention);
  #endif // ENABLE_LOG_LEVEL_COMMANDS

} 


/******************************************************************************************************************************
*******************************************************************************************************************************
****************** PixelHardwareType *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mSDCard::CommandSet_CreateFile_WithName(char* value){

//   analogWrite(pin, value);
  createDir(SD, "/mydir");

// AddLog(LOG_LEVEL_TEST,PSTR("pwm %d value = %d"),pin,value);


  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_SDCARD D_JSON_COMMAND_SVALUE_K("ListDir")), dirname);
  #endif // ENABLE_LOG_LEVEL_COMMANDS

} 

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** PixelHardwareType *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mSDCard::CommandSet_SerialPrint_FileNames(const char* dirname){

  // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_SDCARD D_JSON_COMMAND_SVALUE_K("TESTListDir")), dirname);
  listDir(SD, dirname, 0);

  // listDir(SD, "/", 0);

  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_SDCARD D_JSON_COMMAND_SVALUE_K("ListDir")), dirname);
  #endif // ENABLE_LOG_LEVEL_COMMANDS

} 

#endif // USE_MODULE_LIGHTS_INTERFACE
