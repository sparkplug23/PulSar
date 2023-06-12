/**
 * @file mCellular.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-29
 * 
 * @note Cellular should only be handling interface level. 
 *       In relation to GPS, it should be read here if available then inserted into sensor interface
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "mCellular.h"

#ifdef USE_MODULE_NETWORK_CELLULAR



void mCellular::SMS_Enable()
{
    
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS_Enable++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"));

  if(modem)
  {
    
    SendATCommand_FunctionalityMode_Full();    

    SendATCommand_SMSFormatAscii();

    sms.enabled = true;
  }
}


void mCellular::SMS_Disable()
{
  
  sms.enabled = false;
}


void mCellular::ModemUpdate_SMS()
{
  


  if(mTime::TimeReached(&sms.tReached_Update, 1000))
  {
    if(sms.enabled)
    {
      // Although SMS should be forwarded, lets also check if any are waiting and read them out

      /**
       * @brief Check for waiting SMS messages
       **/
      SendAT_ATParseResponse_F(200, LOG_LEVEL_DEBUG, PSTR("+CMGD=?"));

    }
  }

}



void mCellular::SMSReadAndEraseSavedSMS()
{

  #ifdef ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_CELLULAR "SMSReadAndEraseSavedSMS -- START"));
  #endif 

  for(auto& id:sms.messages_incoming_index_list)
  {
    if(SendAT_ATParseResponse_F(200, LOG_LEVEL_COMMANDS, PSTR("+CMGR=%d"), id))
    {
      // modem->waitResponse(200);
      // delay(200);
      
      // Erase from memory
      if(SendAT_F(200, PSTR("+CMGD=%d"), id))
      {
        ALOG_INF(PSTR("SMSReadAndEraseSavedSMS %d deleted"), id);
      }else{
        ALOG_INF(PSTR("SMSReadAndEraseSavedSMS %d FAILED TO delete"), id);

      }
    }
  }

  sms.messages_incoming_index_list.clear(); // for now assumed its done

  #ifdef ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_CELLULAR "SMSReadAndEraseSavedSMS -- END"));
  #endif

}



void mCellular::SendATCommand_SMSFormatAscii()
{
  // Enable SMS always send to serial when they arrive, move to function later
  modem->sendAT("+CMGF=1"); // Set the ascii messages (not HEX)
  bool result = modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT);
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMSFormatAscii %d"), result);
}


void mCellular::SendATCommand_SMSFormatPDU()
{
  // Enable SMS always send to serial when they arrive, move to function later
  modem->sendAT("+CMGF=0"); // Set the ascii messages (not HEX)
  bool result = modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT);
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMSFormatPDU %d"), result);
}


/**
 * @brief Make it so new SMS messages are send over serial automatically
 * +CNMI: <mode>,<mt>,<bm>,<ds>,<bfr>
 * The read command parameters and their defined values are the following:
  <mode>
    0 – Do not forward unsolicited result codes to the Terminal Equipment (TE) (default)
    3 – Forward unsolicited result codes directly to the TE
  <mt>
    0 – No received message notifications, the modem acts as an SMS client
    2 – SMS-DELIVERs (except class 2 and message waiting indication group) are routed directly to the TE
  <bm>
    No CBM notifications are routed to the TE
  <ds>
    0 – No SMS-STATUS-REPORTs are routed to the TE
    1 – SMS-STATUS-REPORTs are routed to the TE using unsolicited result code: +CDS: <length><CR><LF><pdu>
  <bfr>
    1 – The buffer of unsolicited result codes is cleared when <mode> 1 to 3 is entered
**/
void mCellular::SendATCommand_SMSImmediateForwardOverSerial()
{
  // Enable SMS always send to serial when they arrive, move to function later
  modem->sendAT("+CNMI=3,2,0,0,0");
  bool result = modem->waitResponse(AT_COMMAND_RESPONSE_TIMEOUT__CNMI);
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMSImmediateForwardOverSerial %d"), result);
}



void mCellular::AutoSMS_Messages_Handle()
{

  /**
   * @brief Message1: Send GPS if requested
   * 
   */
  if(smsauto_gps_messages.rate_seconds>0) //if it set to auto
  {

    if(mTime::TimeReached(&smsauto_gps_messages.tSaved_LastSent, smsauto_gps_messages.rate_seconds*1000))
    {
      SMS_GPSLocationAuto();
    }


  }



}

#endif // USE_MODULE_NETWORK_CELLULAR