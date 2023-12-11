#include "mTime.h"

const char* mTime::PM_MODULE_CORE_TIME_CTR = D_MODULE_CORE_TIME_CTR;
const char* mTime::PM_MODULE_CORE_TIME_FRIENDLY_CTR = D_MODULE_CORE_TIME_FRIENDLY_CTR;

int8_t mTime::Tasker(uint8_t function, JsonParserObject obj)
{

  

  switch(function){
    case FUNC_INIT:
      init();
      testtime.millis = millis();
      testtime.run = false;
    break;
  }

  switch(function){
    case FUNC_EVERY_SECOND:{
      
      UpdateStoredRTCVariables();
      UpdateUpTime();
      
      #ifndef DISABLE_SERIAL0_CORE
        #ifdef DEBUG_MODULE_TIME_STD
        char buffer[40];
        AddLog(LOG_LEVEL_TEST, PSTR("DT_DST=%s"), pCONT_time->GetDateAndTimeCtr(DT_DST, buffer, sizeof(buffer)));
        AddLog(LOG_LEVEL_TEST, PSTR("DT_STD=%s"), pCONT_time->GetDateAndTimeCtr(DT_STD, buffer, sizeof(buffer)));
        AddLog(LOG_LEVEL_TEST, PSTR("DT_TIMEZONE=%s IsDst=%d"), pCONT_time->GetDateAndTimeCtr(DT_TIMEZONE, buffer, sizeof(buffer)), IsDst());
        #endif
      #endif

      #ifdef ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
      Serial.println(GetUptime());
      #endif // ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME

    }break;
    case FUNC_EVERY_FIVE_SECOND:

      #ifdef ENABLE_DEVFEATURE__TIME_NTP_UPDATE_WITH_VERSION2

      SystemTime_NTPUpdate();

      #else
      #ifdef USE_MODULE_NETWORK_WIFI
      // if(pCONT_interface_network->Connected(mInterfaceNetwork::NETWORK_TYPE_WIFI))
      // {
#ifndef ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST
        WifiPollNtp();
#endif // ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST
      // }
      #endif // USE_MODULE_NETWORK_WIFI
      #endif // ENABLE_DEVFEATURE__TIME_NTP_UPDATE_WITH_VERSION2

    break;
    case FUNC_EVERY_MINUTE:


    break;
  }

}//end



/**********************************************************************************************************************************************************************************************************
 * @brief
 * 
 * Functions used to set the time
 * 
 * *********************************************************************************************************************************************************************************************************/

/**
 * @brief Allows GPS to update time when no NTP is available (although with network it is possible..?)
 * 
 * 
 * Add that GPS can be called here
 * Have the function argument called always when new time is available eg gps receive
 * but have it that an internal check only sets it as often as I want (eg every 10 minutes even though its called every second). Use flag to force set too if desired from another call (eg new connection)
 * 
 */
// void mTime::SetSystemTime()
// {

// }



/**********************************************************************************************************************************************************************************************************
 * @brief
 * 
 * Functions update time
 * 
 * *********************************************************************************************************************************************************************************************************/

#ifdef ENABLE_DEVFEATURE__TIME_NTP_UPDATE_WITH_VERSION2
void mTime::SystemTime_NTPUpdate()
{

  // Get primary network stream


  uint32_t ntp_time = 0;
  bool ntp_get_success = SystemTime_NTPUpdate_GetNTPTime(&ntp_time, stream);

  




}


bool mTime::SystemTime_NTPUpdate_GetNTPTime(uint32_t* ntp_time)
{



}
#endif // ENABLE_DEVFEATURE__TIME_NTP_UPDATE_WITH_VERSION2




uint32_t mTime::UtcTime(void)
{
  return Rtc.utc_time;
}


/**********************************************************************************************************************************************************************************************************
 * @brief
 * 
 * Helper functions for time conversion
 * 
 * *********************************************************************************************************************************************************************************************************/




/**********************************************************************************************************************************************************************************************************
 * @brief
 * 
 * Helper functions for time comparisons and logic
 * 
 * *********************************************************************************************************************************************************************************************************/









