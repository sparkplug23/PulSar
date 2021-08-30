#include "mWLEDEffects.h"

#ifdef USE_MODULE_LIGHTS_WLED_EFFECTS 

void mWLEDEffects::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  char buffer[50];

  // if(pCONT_time->uptime.seconds_nonreset<10){ return ; }

  byte effectCurrent=0,effectSpeed = 0,effectIntensity=0,effectPalette=0;

  if(jtok = obj["Effects"].getObject()["Current"]){ // default to secs
    effectCurrent = jtok.getInt();
    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_TIME)),animation.transition.time_ms.val);  
    // #endif // ENABLE_LOG_LEVEL_DEBUG
  }
  if(jtok = obj["Effects"].getObject()["Speed"]){ // default to secs
    effectSpeed = jtok.getInt();
    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_TIME)),animation.transition.time_ms.val);  
    // #endif // ENABLE_LOG_LEVEL_DEBUG
  }
  if(jtok = obj["Effects"].getObject()["Intensity"]){ // default to secs
    effectIntensity = jtok.getInt();
    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_TIME)),animation.transition.time_ms.val);  
    // #endif // ENABLE_LOG_LEVEL_DEBUG
  }
  if(jtok = obj["Effects"].getObject()["Palette"]){ // default to secs
    effectPalette = jtok.getInt();
    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_TIME)),animation.transition.time_ms.val);  
    // #endif // ENABLE_LOG_LEVEL_DEBUG
    mEffects->setEffectConfig(effectCurrent, effectSpeed, effectIntensity, effectPalette);
  }


  if(jtok = obj["Effects"].getObject()["Mode"]){ // default to secs
    CommandSet_EffectsModeID(jtok.getInt());
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    //AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_TIME)), animation.transition.time_ms.val);  
    #endif // ENABLE_LOG_LEVEL_DEBUG
  }


}




void mWLEDEffects::CommandSet_EffectsModeID(uint8_t mode, uint8_t segment)
{
  char buffer[50];
  mEffects->setMode(segment, mode);
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_HARDWARE_TYPE)), GetEffectsModeName(buffer, sizeof(buffer)));
  #endif // ENABLE_LOG_LEVEL_COMMANDS
} 


#endif // USE_DRIVER
