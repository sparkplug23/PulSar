/**
 * @file mDevelopmentDebugging.cpp
 * 
 * @author your name (you@domain.com)
 * @brief Special tasker that will be used to develop code, so I can remove it from other builds to move closer to a release version
 * 
 * 1) A way to print DEFINES that are being phased out
 * 
 * @version 0.1
 * @date 2022-03-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "mDevelopmentDebugging.h"


const char* mDevelopmentDebugging::PM_MODULE_CORE_DEVELOPMENT_DEBUGGING_CTR = D_MODULE_CORE_DEVELOPMENT_DEBUGGING_CTR;
const char* mDevelopmentDebugging::PM_MODULE_CORE_DEVELOPMENT_DEBUGGING_FRIENDLY_CTR = D_MODULE_CORE_DEVELOPMENT_DEBUGGING_FRIENDLY_CTR;


int8_t mDevelopmentDebugging::Tasker(uint8_t function, JsonParserObject obj){


  switch(function){
    case FUNC_INIT:
      //
    break;
  }

  switch(function){
    
    case FUNC_LOOP: {
      
    }break;
    case FUNC_EVERY_SECOND:{
      
      // Serial.printf("time_start1=%d\n\r",millis());

    }break;
    case FUNC_EVERY_FIVE_SECOND:

    break;
    case FUNC_EVERY_MINUTE:

      SubTask_Show_Defines_Ready_To_Phase_Out();

    break;
    case FUNC_EVERY_MIDNIGHT:
    
    break;
  }

}//end


void mDevelopmentDebugging::SubTask_Show_Defines_Ready_To_Phase_Out()
{

  #ifdef ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS
    #ifdef ENABLE_LOG_LEVEL_WARN
    AddLog(LOG_LEVEL_WARN, PSTR("ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS: Ready to remove 10Apr22")); // Save to remove
    #endif // ENABLE_LOG_LEVEL_COMMANDS
    #warning "ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS"
  #endif 




}