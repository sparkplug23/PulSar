/**
 * @file mDevelopmentDebugging.cpp
 * 
 * @author your name (you@domain.com)
 * @brief Special tasker that will be used to develop code, so I can remove it from other builds to move closer to a release version
 * 
 * 1) A way to print DEFINES that are being phased out
 * 2) Idea, configure another laptop (surface book) and make duplicate of project, run on HVAC and try get crash errors
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

      // DEBUG_LINE_HERE;
      
      // Serial.printf("time_start1=%d\n\r",millis());

      #ifdef ENABLE_DEBUG_SPLASH_SYSTEM_PERFORMANCE_METRICS_TO_SERIAL
        ALOG_INF( PSTR(PM_JSON_COMMAND_SVALUE_NVALUE), PM_JSON_LOOPSSEC, pCONT_sup->activity.cycles_per_sec);
        ALOG_INF( PSTR(PM_JSON_COMMAND_SVALUE_NVALUE), PM_JSON_FREEHEAP, ESP.getFreeHeap());
      #endif // ENABLE_DEBUG_SPLASH_SYSTEM_PERFORMANCE_METRICS_TO_SERIAL

    }break;
    case FUNC_EVERY_FIVE_SECOND:

    break;
    case FUNC_EVERY_MINUTE:

      SubTask_Show_Defines_Ready_To_Phase_Out();

      SubTask_Splash_Debug_Info();

    break;
    case FUNC_EVERY_MIDNIGHT:
    
    break;
  }

}//end

void mDevelopmentDebugging::SubTask_Splash_Debug_Info()
{

  ALOG_DBG(PSTR("reboot_count_since_poweron=%d"), RtcFastboot.reboot_count_since_poweron);
  


}


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
  // #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  //   #ifdef ENABLE_LOG_LEVEL_WARN
  //   AddLog(LOG_LEVEL_WARN, PSTR("ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS: Not ready to remove 10May22")); // Save to remove
  //   #endif // ENABLE_LOG_LEVEL_COMMANDS
  //   #warning "ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS"
  // #endif 


  


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
  #ifdef ENABLE_DEVFEATURE_SET_ESP32_RGB_DATAPIN_BY_TEMPLATE
    #error "ENABLE_DEVFEATURE_SET_ESP32_RGB_DATAPIN_BY_TEMPLATE -- Can be removed"
  #endif 
  #ifdef PINSET_TEMP_METHOD_RGB_PIN_RGB
    #error "PINSET_TEMP_METHOD_RGB_PIN_RGB -- Can be removed"
  #endif 
  #ifdef ENABLE_DEVFEATURE_BUTTON_HANDLER_V2
  #error "ENABLE_DEVFEATURE_BUTTON_HANDLER_V2 - Needs to be removed"
  #endif
  #ifdef ENABLE_DEVFEATURE_BUTTON_MULTIPRESS
  #error "ENABLE_DEVFEATURE_BUTTON_MULTIPRESS - Needs to be removed"
  #endif
  #ifdef USE_DEVFEATURE_MOTION_EVENT_WITH_RULE_IDS_FOR_DOORSENSOR
  #error "USE_DEVFEATURE_MOTION_EVENT_WITH_RULE_IDS_FOR_DOORSENSOR - Needs to be removed"
  #endif
  #ifdef USE_DEVFEATURE_MOTION_EVENT_USES_MODULE_ID_FOR_DEVICENAME
  #error "USE_DEVFEATURE_MOTION_EVENT_USES_MODULE_ID_FOR_DEVICENAME - Needs to be removed"
  #endif




}