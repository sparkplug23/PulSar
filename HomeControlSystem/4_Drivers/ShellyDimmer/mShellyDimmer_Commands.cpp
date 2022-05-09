#include "mShellyDimmer.h"

#ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER

void mShellyDimmer::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
    

  if(jtok = obj[PM_JSON_BRIGHTNESS]){

    req_brightness = map(jtok.getInt(),0,100,0,1000);
    SetBrightnessReq();

  }

  if(jtok = obj["BrightnessFade"]){

    req_brightness = jtok.getInt();
    SetBrightnessFade();

  }

  if(jtok = obj["ShellyDimmer"].getObject()["TriggerEdge"]){

    CmndShdLeadingEdge( jtok.getInt());

  }
  
  if(jtok = obj["ShellyDimmer"].getObject()[PM_JSON_TIME_ON]){
    CommandSet_Timer_Decounter(jtok.getInt());
  }else
  if(jtok = obj["ShellyDimmer"].getObject()[PM_JSON_TIME_ON_SECS]){
    CommandSet_Timer_Decounter(jtok.getInt());
  }else
  if(jtok = obj["ShellyDimmer"].getObject()[PM_JSON_TIME_ON_MINUTES]){
    CommandSet_Timer_Decounter(jtok.getInt()*60);
  }
  
  mqtthandler_state_teleperiod.flags.SendNow = true;

}


/**********************************************************************************************
 *********************************************************************************************
  Parameter: TimerDecounter
 *********************************************************************************************
 ********************************************************************************************/

void mShellyDimmer::CommandSet_Timer_Decounter(uint16_t time_secs)
{
  timer_decounter.seconds = time_secs;
  timer_decounter.active = time_secs > 0 ? true : false;
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_RELAYS "Set" D_JSON_TIME "%d" D_UNIT_SECOND), timer_decounter.seconds);  
  #endif
}

uint16_t mShellyDimmer::CommandGet_SecondsToRemainOn()
{
  // relay_status[relay_id].timer_decounter.seconds = time_secs;
  // relay_status[relay_id].timer_decounter.active = time_secs > 0 ? true : false;
  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_RELAYS "Set" D_JSON_TIME "Relay%d " "%d" D_UNIT_SECOND), relay_id, relay_status[relay_id].timer_decounter.seconds);  
  // #endif

  return timer_decounter.seconds;
}



bool mShellyDimmer::SetChannels(void)
{
#ifdef SHELLY_DIMMER_DEBUG
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(SHD_LOGNAME "SetChannels:"));
    // AddLogBuffer(LOG_LEVEL_DEBUG_MORE, (uint8_t *)XdrvMailbox.data, XdrvMailbox.data_len);
#endif  // SHELLY_DIMMER_DEBUG

    // uint16_t brightness = ((uint32_t *)XdrvMailbox.data)[0];
    // // Use dimmer_hw_min and dimmer_hw_max to constrain our values if the light should be on
    // if (brightness > 0)
    //     brightness = changeUIntScale(brightness, 0, 255, Settings.dimmer_hw_min * 10, Settings.dimmer_hw_max * 10);
    // req_brightness = brightness;

    // ShdDebugState();

    return SyncState();
}

bool mShellyDimmer::SetPower(void)
{
  #ifdef SHELLY_DIMMER_DEBUG
  // AddLog(LOG_LEVEL_INFO, PSTR(SHD_LOGNAME "Set Power, Power 0x%02x"), XdrvMailbox.index);
  #endif  // SHELLY_DIMMER_DEBUG

  req_on = 1;//(bool)XdrvMailbox.index;
  return SyncState();
}



/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void mShellyDimmer::CmndShdLeadingEdge(uint8_t edge_type)
{
    if (edge_type == 0 || edge_type == 1)
    {
        leading_edge = 2 - edge_type;
        // pCN Settings.shd_leading_edge = edge_type;
#ifdef SHELLY_DIMMER_DEBUG
        if (leading_edge == 1)
            AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Set to trailing edge"));
        else
            AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Set to leading edge"));
#endif  // SHELLY_DIMMER_DEBUG
        SendSettings();
    }
    SaveSettings();
    // ResponseCmndNumber(Settings.shd_leading_edge);
}



#ifdef SHELLY_CMDS

const char kShdCommands[] PROGMEM = D_PRFX_SHD "|"  // Prefix
  D_CMND_LEADINGEDGE "|"  D_CMND_WARMUPBRIGHTNESS "|" D_CMND_WARMUPTIME;

void (* const ShdCommand[])(void) PROGMEM = {
  &CmndShdLeadingEdge, &CmndShdWarmupBrightness, &CmndShdWarmupTime };

void CmndShdWarmupBrightness(void)
{
    if ((10 <= XdrvMailbox.payload) && (XdrvMailbox.payload <= 100))
    {
        warmup_brightness = XdrvMailbox.payload * 10;
        Settings.shd_warmup_brightness = XdrvMailbox.payload;
#ifdef SHELLY_DIMMER_DEBUG
        AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Set warmup brightness to %d%%"), XdrvMailbox.payload);
#endif  // SHELLY_DIMMER_DEBUG
        ShdSendSettings();
    }
    ShdSaveSettings();
    ResponseCmndNumber(Settings.shd_warmup_brightness);
}

void CmndShdWarmupTime(void)
{
    if ((20 <= XdrvMailbox.payload) && (XdrvMailbox.payload <= 200))
    {
        warmup_time = XdrvMailbox.payload;
        Settings.shd_warmup_time = XdrvMailbox.payload;
#ifdef SHELLY_DIMMER_DEBUG
        AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Set warmup time to %dms"), XdrvMailbox.payload);
#endif  // SHELLY_DIMMER_DEBUG
        ShdSendSettings();
    }
    ShdSaveSettings();
    ResponseCmndNumber(Settings.shd_warmup_time);
}

#endif // SHELLY_CMDS

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** CommandSet_ReadFile *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

// void mShellyDimmer::CommandSet_ReadFile(const char* filename){

//   readFile(SD_MMC, filename);

//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_SDCARD D_JSON_COMMAND_SVALUE_K("ReadFile")), filename);
//   #endif // ENABLE_LOG_LEVEL_COMMANDS

// } 


#endif // USE_MODULE_LIGHTS_INTERFACE
