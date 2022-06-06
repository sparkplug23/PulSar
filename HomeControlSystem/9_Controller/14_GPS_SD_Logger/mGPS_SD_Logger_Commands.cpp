
#include "mGPS_SD_Logger.h"

#ifdef USE_MODULE_CONTROLLER_GPS_SD_LOGGER

void mGPS_SD_Logger::parse_JSONCommand(JsonParserObject obj)
{

}

void mGPS_SD_Logger::CommandSet_SDCard_OpenClose_Toggle()
{//uint16_t time_secs, uint8_t relay_id){


  logger_status.enable ^= 1;

  // sdcard_status.isopened ^= 1;
  // sdcard_status.enable_logging ^= 1;
  // sdcard_status.isopened = sdcard_status.enable_logging; //this should not be linked

  // // If closed, start logging and begin sdcard opening sequence
  // if(sdcard_status.isopened)

  AddLog(LOG_LEVEL_TEST, PSTR("mGPS_SD_Logger CommandSet_SDCard_OpenClose_Toggle %d"),logger_status.isopened);


  // relay_status[relay_id].timer_decounter.seconds = time_secs;
  // relay_status[relay_id].timer_decounter.active = time_secs > 0 ? true : false;
  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_RELAYS "Set" D_JSON_TIME "Relay%d " "%d" D_UNIT_SECOND), relay_id, relay_status[relay_id].timer_decounter.seconds);  
  // #endif

}



void mGPS_SD_Logger::CommandSet_LoggingState(uint8_t state)
{//uint16_t time_secs, uint8_t relay_id){

  if(state == 2) //toggle
  {
    logger_status.enable ^= 1; 
    AddLog(LOG_LEVEL_TEST, PSTR("CommandSet_LoggingState state == 2, %d"),logger_status.enable);

  }else
  {
    logger_status.enable = state;
    AddLog(LOG_LEVEL_TEST, PSTR("CommandSet_LoggingState sdcard_status.enable_logging = state,  %d"),logger_status.enable);

  }



  // sdcard_status.isopened ^= 1;
  // sdcard_status.enable_logging ^= 1;
  // sdcard_status.isopened = sdcard_status.enable_logging; //this should not be linked

  // // If closed, start logging and begin sdcard opening sequence
  // if(sdcard_status.isopened)

  AddLog(LOG_LEVEL_TEST, PSTR("CommandSet_LoggingState %d"),logger_status.enable);


  // relay_status[relay_id].timer_decounter.seconds = time_secs;
  // relay_status[relay_id].timer_decounter.active = time_secs > 0 ? true : false;
  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_RELAYS "Set" D_JSON_TIME "Relay%d " "%d" D_UNIT_SECOND), relay_id, relay_status[relay_id].timer_decounter.seconds);  
  // #endif

}



#endif // USE_MODULE_CONTROLLER_GPS_SD_LOGGER