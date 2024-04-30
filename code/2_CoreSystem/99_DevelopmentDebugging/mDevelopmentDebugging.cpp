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

      #ifdef ENABLE_DEBUG_SPLASH_SYSTEM_PERFORMANCE_METRICS_TO_SERIAL
        ALOG_INF( PSTR(PM_JSON_COMMAND_SVALUE_NVALUE), PM_JSON_LOOPSSEC, pCONT_sup->activity.cycles_per_sec);
        ALOG_INF( PSTR(PM_JSON_COMMAND_SVALUE_NVALUE), PM_JSON_FREEHEAP, ESP.getFreeHeap());
      #endif // ENABLE_DEBUG_SPLASH_SYSTEM_PERFORMANCE_METRICS_TO_SERIAL

      // ALOG_INF( PSTR("loops_per_second %d"), pCONT_sup->loops_per_second);
      // ALOG_INF( PSTR("this_cycle_ratio %d"), pCONT_sup->this_cycle_ratio);
      // ALOG_INF( PSTR("loop_load_avg %d"), pCONT_set->loop_load_avg);
    
    }break;
    case FUNC_EVERY_FIVE_SECOND:

    break;
    case FUNC_EVERY_MINUTE:

      SubTask_Show_Defines_Ready_To_Phase_Out();

    break;
    case FUNC_EVERY_MIDNIGHT:
    
    break;
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
  }

}//end


void mDevelopmentDebugging::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

	if(jtok = obj["DebugInput"].getObject()["Float1"])
	{
		debug_data.input_float1 = jtok.getFloat();
    char buffer[20];
    mSupport::float2CString(debug_data.input_float1, JSON_VARIABLE_FLOAT_PRECISION_LENGTH, buffer);    
    ALOG_INF(PSTR("DebugInput Float1: %s"), buffer);
	}



    
}

void mDevelopmentDebugging::SubTask_Show_Defines_Ready_To_Phase_Out()
{
  
  /**
   * @brief Phasing out list, so remind me via messages that this should either be properly added into the code, or deleted code depending on the flag
   * 
   */
  #ifdef ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS
    #ifdef ENABLE_LOG_LEVEL_WARNING
    AddLog(LOG_LEVEL_WARN, PSTR("ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS: Ready to remove 10Apr22")); // Save to remove
    #endif // ENABLE_LOG_LEVEL_COMMANDS
    #warning "ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS"
  #endif 
  // #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  //   #ifdef ENABLE_LOG_LEVEL_WARNING
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
  #ifdef ENABLE_DEVFEATURE_COLOUR_PALETTE_REMOVE_OLD
  #error "ENABLE_DEVFEATURE_COLOUR_PALETTE_REMOVE_OLD"
  #endif
  #ifdef ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
  #error "ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID"
  #endif
  #ifdef ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
  #error "ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS"
  #endif
  #ifdef ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
  #error "ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE"
  #endif
  #ifdef ENABLE_DEVFEATURE_MOVING_GETCOLOUR_AND_PALETTE_TO_RAM
  #error "ENABLE_DEVFEATURE_MOVING_GETCOLOUR_AND_PALETTE_TO_RAM"
  #endif
  #ifdef ENABLE_DEVFEATURE_REMOVE_BRIGHTNESS_RANDOMNESS_INSIDE_APPLY_BRIGHTNESS
  #error "ENABLE_DEVFEATURE_REMOVE_BRIGHTNESS_RANDOMNESS_INSIDE_APPLY_BRIGHTNESS
  #endif
  #ifdef ENABLE_DEVFEATURE_FIX_STRIPSIZE_LENGTH_ISSUE_XMAS2022
  #error "ENABLE_DEVFEATURE_FIX_STRIPSIZE_LENGTH_ISSUE_XMAS2022
  #endif
  #ifdef ENABLE_DEVFEATURE_REMOVE_SEG_STOP_INDEX
  #error "ENABLE_DEVFEATURE_REMOVE_SEG_STOP_INDEX
  #endif
  #ifdef ENABLE_DEVFEATURE_COLOUR_PALETTE_MERGED
  #error "ENABLE_DEVFEATURE_COLOUR_PALETTE_MERGED
  #endif
  #ifdef ENABLE_DEVFEATURE_CANSHOW_VERSION3
  #error "ENABLE_DEVFEATURE_CANSHOW_VERSION3
  #endif
  #ifdef ENABLE_DEVFEATURE_FORCED_REMOVE_091122
  #error "ENABLE_DEVFEATURE_FORCED_REMOVE_091122
  #endif
  #ifdef ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
  #error "ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
  #endif
  
  #ifdef ENABLE_DEVFEATURE_REMOVING_HSBID
  #error "ENABLE_DEVFEATURE_REMOVING_HSBID
  #endif
  #ifdef ENABLE_DEVFEATURE_PALETTE__VECTORED
  #error "ENABLE_DEVFEATURE_PALETTE__VECTORED
  #endif
  #ifdef ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
  #error "ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
  #endif
  #ifdef ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
  #error "ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
  #endif
  #ifdef ENABLE_DEVFEATURE__PIXEL_USE_I2S_FOR_BUS
  #error "ENABLE_DEVFEATURE__PIXEL_USE_I2S_FOR_BUS
  #endif
  #ifdef ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28
  #warning "ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28
  #endif
  #ifdef ENABLE_DEBUG_MULTIPIN
  #error "ENABLE_DEBUG_MULTIPIN
  #endif
  #ifdef DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
  #error "DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
  #endif

  #ifdef ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT
  #error "ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT"
  #endif
  #ifdef ENABLE_DEVFEATURE_SWITCH_TO_U16_GPIO_FUNCTIONS
  #warning "ENABLE_DEVFEATURE_SWITCH_TO_U16_GPIO_FUNCTIONS phase out"
  #else
  #error "ENABLE_DEVFEATURE_SWITCH_TO_U16_GPIO_FUNCTIONS missing too soon"
  #endif
  #ifdef ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD
  #error "ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD"
  #endif
  #ifdef ENABLE_DEVFEATURE_LIGHT__WLED_WEBUI_SEND_MY_PALETTE_COLOUR_BARS
  #warning "ENABLE_DEVFEATURE_LIGHT__WLED_WEBUI_SEND_MY_PALETTE_COLOUR_BARS phase out"
  #endif
  #ifdef ENABLE_DEVFEATURE_LIGHT__WEBUI_APPEND_EFFECT_CONFIG_TO_JSON_RESPONSE
  #error "ENABLE_DEVFEATURE_LIGHT__WEBUI_APPEND_EFFECT_CONFIG_TO_JSON_RESPONSE"
  #endif
  #ifdef ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
  #error "ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN"
  #endif 
  #ifdef ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
  #error "ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS"
  #endif 
  #ifdef ENABLE_DEVFEATURE_REMOVE_INIT_OUTSIDE_OF_PALETTE_CLASS
  #error "ENABLE_DEVFEATURE_REMOVE_INIT_OUTSIDE_OF_PALETTE_CLASS"
  #endif 
  #ifdef STRIP_SIZE_MAX
  #error "STRIP_SIZE_MAX is not longer used since multi-pin"
  #endif
  #ifdef ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS
  #warning "ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS should stay, but I dont want this enabled by default until motion bug is fixed"
  #endif

  #ifdef ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL
  #error "ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL"
  #endif



}


/**
 * @brief DEBUG_NOTES
 * 
 * Describing planned unified define methods to be used everywhere
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */

/**
 * @brief 
 * "DEBUG_TRACE"
 * Debug via serial that should only be active when needed
 * 
 * ENABLE_DEBUG_TRACE__##
 * 
 * eg
 * ENABLE_DEBUG_TRACE__MQTT_TOPIC_AS_TRASNMITTED
 * ENABLE_DEBUG_MESSAGE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
 */