#include "mShellyDimmer.h"

#ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER

void mShellyDimmer::parse_JSONCommand(JsonParserObject obj)
{

// int8_t mShellyDimmer::CheckAndExecute_JSONCommands(){

//   // Check if instruction is for me
//   if(mSupport::SetTopicMatch(data_buffer.topic.ctr,D_MODULE_CONTROLLER_FAN_FRIENDLY_CTR)>=0){
//     #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_MQTT D_TOPIC_COMMAND D_MODULE_CONTROLLER_FAN_FRIENDLY_CTR));
//     #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
//     parse_JSONCommand();
//     return FUNCTION_RESULT_HANDLED_ID;
//   }else{
//     return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
//   }

// }

// void mShellyDimmer::parse_JSONCommand(void){

//   char buffer[50];

//   // Need to parse on a copy
//   char parsing_buffer[data_buffer.payload.len+1];
//   memcpy(parsing_buffer,data_buffer.payload.ctr,sizeof(char)*data_buffer.payload.len+1);
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
    

  if(jtok = obj[PM_JSON_BRIGHTNESS]){

    req_brightness = map(jtok.getInt(),0,100,0,1000);
    SetBrightness();

  }

  if(jtok = obj["BrightnessFade"]){

    req_brightness = jtok.getInt();
    SetBrightnessFade();

  }
  
  
  mqtthandler_state_teleperiod.flags.SendNow = true;

}




bool mShellyDimmer::SetChannels(void)
{
#ifdef SHELLY_DIMMER_DEBUG
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(SHD_LOGNAME "SetChannels:"));
    AddLogBuffer(LOG_LEVEL_DEBUG_MORE, (uint8_t *)XdrvMailbox.data, XdrvMailbox.data_len);
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
  AddLog(LOG_LEVEL_INFO, PSTR(SHD_LOGNAME "Set Power, Power 0x%02x"), XdrvMailbox.index);
  #endif  // SHELLY_DIMMER_DEBUG

  req_on = 1;//(bool)XdrvMailbox.index;
  return SyncState();
}



/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

#ifdef SHELLY_CMDS

const char kShdCommands[] PROGMEM = D_PRFX_SHD "|"  // Prefix
  D_CMND_LEADINGEDGE "|"  D_CMND_WARMUPBRIGHTNESS "|" D_CMND_WARMUPTIME;

void (* const ShdCommand[])(void) PROGMEM = {
  &CmndShdLeadingEdge, &CmndShdWarmupBrightness, &CmndShdWarmupTime };

void CmndShdLeadingEdge(void)
{
    if (XdrvMailbox.payload == 0 || XdrvMailbox.payload == 1)
    {
        leading_edge = 2 - XdrvMailbox.payload;
        Settings.shd_leading_edge = XdrvMailbox.payload;
#ifdef SHELLY_DIMMER_DEBUG
        if (leading_edge == 1)
            AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Set to trailing edge"));
        else
            AddLog(LOG_LEVEL_DEBUG, PSTR(SHD_LOGNAME "Set to leading edge"));
#endif  // SHELLY_DIMMER_DEBUG
        ShdSendSettings();
    }
    ShdSaveSettings();
    ResponseCmndNumber(Settings.shd_leading_edge);
}

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
