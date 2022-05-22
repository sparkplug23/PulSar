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
  
  /**
   * @brief Phasing out list, so remind me via messages that this should either be properly added into the code, or deleted code depending on the flag
   * 
   */
  #ifdef ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS
    #ifdef ENABLE_LOG_LEVEL_WARN
    AddLog(LOG_LEVEL_WARN, PSTR("ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS: Ready to remove 10Apr22")); // Save to remove
    #endif // ENABLE_LOG_LEVEL_COMMANDS
    #warning "ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS"
  #endif 
  #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    #ifdef ENABLE_LOG_LEVEL_WARN
    AddLog(LOG_LEVEL_WARN, PSTR("ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS: Not ready to remove 10May22")); // Save to remove
    #endif // ENABLE_LOG_LEVEL_COMMANDS
    #warning "ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS"
  #endif 


  


  /**
   * @brief When a define has been stripped from code, but not templates, this will alert user to ensure new commands
   * are properly integrated into the newer code format.    * 
   */
  #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    // #error "ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS -- Can be removed"
  #endif 
  #ifdef ENABLE_FEATURE_INCLUDE_WLED_PALETTES
    #error "ENABLE_FEATURE_INCLUDE_WLED_PALETTES -- Can be removed"
  #endif 
  #ifdef ENABLE_CRGBPALETTES_IN_PROGMEM
    #error "ENABLE_CRGBPALETTES_IN_PROGMEM -- Can be removed"
  #endif 
  #ifdef ENABLE_DEVFEATURE_SHIMMERING_PALETTE_BRIGHTNESS_LIMIT
    #error "ENABLE_DEVFEATURE_SHIMMERING_PALETTE_BRIGHTNESS_LIMIT -- Can be removed"
  #endif 
  #ifdef ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS
    #error "ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS -- Can be removed"
  #endif 




}