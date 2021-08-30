#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

const char* mAnimatorLight::PM_MODULE_LIGHTS_ANIMATOR_CTR = D_MODULE_LIGHTS_ANIMATOR_CTR;
const char* mAnimatorLight::PM_MODULE_LIGHTS_ANIMATOR_FRIENDLY_CTR = D_MODULE_LIGHTS_ANIMATOR_FRIENDLY_CTR;


int8_t mAnimatorLight::Tasker(uint8_t function, JsonParserObject obj)
{

  #ifdef ENABLE_DEVFEATURE_Tasker_Override_Forcing_Variables_Testing
  Tasker_Override_Forcing_Variables_Testing();
  #endif // ENABLE_DEVFEATURE_FLICKERING_TEST5

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case FUNC_PRE_INIT:
      Pre_Init();  
      break;
    case FUNC_INIT:
      init();
      break;
    case FUNC_OVERRIDE_BOOT_INIT:

    // stripbus->NeoRgbCurrentSettings(1,2,3);

      #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
      mqtthandler_debug_animations_progress.tRateSecs = 1;
      #endif

    break;
  }
  
  if(!settings.flags.EnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID;}
  
  switch(function){
    /************
     * SETTINGS SECTION * 
    *******************/
    case FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE: 
      // Settings_Load();
    break;
    case FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE: 
      Settings_Save();
    break;
    case FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES:
    case FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT:
      Settings_Default();
    break;
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_EVERY_SECOND:{
      //EverySecond();

//       const NeoRgbcctCurrentSettings settings(200,200,200,201,202);
// uint32_t maTotal = stripbus->CalcTotalMilliAmpere(settings);


    }break;
    case FUNC_LOOP: 
      // EveryLoop();


      // #if defined(USE_MODULE_LIGHTS_ANIMATOR) && !defined(USE_MODULE_LIGHTS_WLED_EFFECTS)
      EveryLoop(); // hacs only
    //   #else



    //   if(pCONT_lAni->flashersettings.function<EFFECTS_FUNCTION_LENGTH_ID)
    //   {
    //     EveryLoop(); // HACS
    //     // AddLog(LOG_LEVEL_INFO, PSTR("RUNNING = HACS %d"), pCONT_lAni->flashersettings.function);
    //     // delay(500);
    //   }
    //   else 
    //   if(
    //     (pCONT_lAni->flashersettings.function>=EFFECTS_MODE_STATIC) &&
    //     (pCONT_lAni->flashersettings.function<EFFECTS_MODE_COUNT)
    //   ){

    //     //shift function wled indexing from 0
    //     uint8_t wled_function_mode_index = EFFECTS_MODE_STATIC - pCONT_lAni->flashersettings.function;

    //     AddLog(LOG_LEVEL_INFO, PSTR("RUNNING = WLED %d -> %d"),pCONT_lAni->flashersettings.function,wled_function_mode_index);
    //     pCONT_lwled->mEffects->setMode(0, wled_function_mode_index);
        
    //     pCONT_lwled->SubTask_Animation();
    //     AddLog(LOG_LEVEL_INFO, PSTR("OVER = WLED %d -> %d"),pCONT_lAni->flashersettings.function,wled_function_mode_index);

    //   }

    // #endif



    break;    
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
    case FUNC_MQTT_HANDLERS_RESET:
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_TelePeriod();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_fSendNow();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
    // Other stuff
    case FUNC_STATUS_MESSAGE_APPEND:
      Append_Hardware_Status_Message();
    break;
  }// switch(command)

  /************
   * WEBPAGE SECTION * 
  *******************/  
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  return Tasker_Web(function);
  #endif // USE_MODULE_NETWORK_WEBSERVER

} // END FUNCTION


#endif //USE_MODULE_LIGHTS_ANIMATOR




