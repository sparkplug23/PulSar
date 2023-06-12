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


/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

void mCellular::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  JsonParserToken jtok2 = 0; 
  int8_t tmp_id = 0;

  
  if(jtok = obj["OpenHAB"].getObject()["SystemTime"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "OpenHAB:SystemTime"));
    ALOG_INF(PSTR(D_LOG_CELLULAR "OH Time %s"),jtok.getStr());
    //toggle led
    DIGITAL_INVERT_PIN(12); //esp32 blue led
  }

  #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
  if(jtok = obj["GPS_Enable"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "GPS_Enable"));
    if(jtok.getInt() == 1)
    {
      GPS_Enable();
    }
  }
  #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS


  if(jtok = obj["SMSAuto_GPS"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "SMSAuto_GPS smsauto_gps_messages.rate_seconds %d"), jtok.getInt());
    smsauto_gps_messages.rate_seconds = jtok.getInt();
    
  }



  if(jtok = obj["GPRS_Enable"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "GPRS_Enable"));
    if(jtok.getInt() == 1)
    {
      DataNetwork__InitConfig();
    }
  }


  if(jtok = obj["SMS_Enable"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "SMS_Enable"));
    if(jtok.getInt() == 1)
    {
      SMS_Enable();
    }
  }

  #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
  if(jtok = obj["SMS_GPSLocation"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "SMS_GPSLocation"));
    if(jtok.getInt() == 1)
    {
      SMS_GPSLocation();
    }
  }
  #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS


  if(jtok = obj["SMSForward"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "SMSForward"));
    if(jtok.getInt() == 1)
    {
      SendATCommand_SMSImmediateForwardOverSerial();
    }
  }

  
  if(jtok = obj["BattVolt"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "BattVolt"));
    if(jtok.getInt() == 1)
    {    
      // read the battery voltage and percentage
      uint16_t vbat = modem->getBattVoltage();        
      ALOG_INF(PSTR(D_LOG_CELLULAR "%d mV"), vbat);
    }
  }

  if(jtok = obj["ModemInit"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "ModemInit"));
    flag_modem_initialized = jtok.getInt();
  }

  if(jtok = obj["ADCVoltage"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "ADCVoltage"));
    if(jtok.getInt() == 1)
    {      
      ModemUpdate_BatteryStatus();
      ALOG_INF(PSTR(D_LOG_CELLULAR "{\"volts_mv\":%d,\"percent\":%d,\"milliVolts\":%d}"),
        modem_status.battery.volts_mv,modem_status.battery.percentage,modem_status.battery.charge_state);
    }
  }


  if(jtok = obj["ReadSMS"]){

    ALOG_INF( PSTR(D_LOG_CELLULAR "ReadSMS"));

    // flag_modem_initialized = jtok.getInt();
  
      // modem->sendAT("+CNMI?"); // <mode>,<mt>,<bm>,<ds>,<bfr> //expected 2,2,0,0,0 

      modem->sendAT("+CMGR=0");

      //parser right here

      bool result = modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT);
      ALOG_INF(PSTR(D_LOG_CELLULAR "ReadSMS %d"), result);
      

    // "AT+CMGD=,4",// DELETE ALL MESSAGES (Read or not)
    // "AT+CMGR=1",
    // "AT+CMGL=\"REC UNREAD\""


  }

  
  if(jtok = obj["ATDebugCommand"])
  {

    if(jtok2 = jtok.getObject()["SMSReadOnly"])
    {
      ALOG_INF( PSTR(D_LOG_CELLULAR "SMSReadOnly"));

      if(jtok2.isStr()) //Assumed worded group type
      {        
        SendAT_F(100, PSTR("+CMGR=\"%s\""), jtok2.getStr());
      }
      if(jtok2.isNum()) // Assumed index (From 0-max)
      {        
        SendAT_ATParseResponse_F(100, LOG_LEVEL_COMMANDS, PSTR("+CMGR=%d"), jtok2.getInt());
      }
    }

    if(jtok2 = jtok.getObject()["SMSReadAndClear"])
    {
      ALOG_INF( PSTR(D_LOG_CELLULAR "SMSReadAndClear"));

      if(jtok2.isNum()) // Assumed index (From 0-max)
      {        
        if(SendAT_ATParseResponse_F(100, LOG_LEVEL_COMMANDS, PSTR("+CMGR=%d"), jtok2.getInt()))
        {
          // Clear it if it was successfully read
          SendAT_F(100, PSTR("+CMGD=%d"), jtok2.getInt());
        }
      }
    }


    if(jtok2 = jtok.getObject()["SMSReadAndClearAll"])
    {
      ALOG_INF( PSTR(D_LOG_CELLULAR "SMSReadAndClearAll"));
      
      if(SendAT_ATParseResponse_F(100, LOG_LEVEL_COMMANDS, PSTR("+CMGD=?")))
      {
        /**
         * @brief Placing in here now
         *  */
        // if(sms.messages_incoming_index_list.size())
        // {
        //   SMSReadAndEraseSavedSMS();
        // } 

      }
      
    }

    
  





    if(jtok2 = jtok.getObject()["SMSCheckWaitingIndexs"])
    {
      ALOG_INF( PSTR(D_LOG_CELLULAR "SMSCheckWaitingIndexs"));
      
      if(SendAT_ATParseResponse_F(100, LOG_LEVEL_COMMANDS, PSTR("+CMGD=?")))
      {
        // Clear it if it was successfully read
        // SendAT_F(100, PSTR("+CMGD=%d"), jtok2.getInt());
      }
      
    }


    if(jtok2 = jtok.getObject()["SMSClearAll"])
    {
      ALOG_INF( PSTR(D_LOG_CELLULAR "SMSClearAll"));
      uint8_t flag = 4; // delete all
      SendAT_F(100, PSTR("+CMGR=%d[,%d]"), jtok2.getInt(), flag);
    }

  }

  if(jtok = obj["ReadDeleteSMS"]){

    ALOG_INF( PSTR(D_LOG_CELLULAR "ReadDeleteSMS"));

    // flag_modem_initialized = jtok.getInt();
  
      // modem->sendAT("+CNMI?"); // <mode>,<mt>,<bm>,<ds>,<bfr> //expected 2,2,0,0,0 


      // SendAT("+CMGR=0[,4]", 100);

      // uint8_t flag = 4; // delete all

      // SendAT_F(PSTR("+CMGR=%d[,%d]"), index, flag);

      modem->sendAT("+CMGR=0[,4]");

// //+CMGD: (list_of_indexes)[,(list_of_flag_values)]

//       //parser right here

      bool result = modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT);
//       ALOG_INF(PSTR(D_LOG_CELLULAR "ReadDeleteSMS %d"), result);
      

      modem->sendAT("+CMGD=0");

      
       result = modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT);
      ALOG_INF(PSTR(D_LOG_CELLULAR "ReadDeleteSMS %d"), result);
    // "AT+CMGD=,4",// DELETE ALL MESSAGES (Read or not)
    // "AT+CMGR=1",
    // "AT+CMGL=\"REC UNREAD\""


  }

      // modem->sendAT("AT+CMGL=\"ALL\"");

      // IPAddress local = modem->localIP();
      // ALOG_INF(PSTR(D_LOG_CELLULAR "Local IP: %s"), String(local).c_str());
      
      // modem->sendAT("+CPMS?"); // How many SMS are waiting?
  // SendATCommand_SMSImmediateForwardOverSerial();



  if(jtok = obj["ATCommands"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "ATCommands"));
    JsonParserArray array = jtok;
    for(auto val : array) {
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CELLULAR "F::%s %s"),__FUNCTION__,val.getStr());
      SerialAT.println(val.getStr());  
      delay(500);
      if (SerialAT.available()) {
        String r = SerialAT.readString();
        ALOG_INF(PSTR(D_LOG_CELLULAR "ATResponse = \"%s\""), r.c_str());
      }
    }

  }

  if(jtok = obj["ATCommandsParsing"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "ATCommandsParsing"));
    JsonParserArray array = jtok;
    for(auto val : array) {
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CELLULAR "F::%s %s"),__FUNCTION__,val.getStr());
      SerialAT.println(val.getStr());  
      delay(500);

      uint32_t wait_millis = 1000;
      
      uint32_t tSaved_millis = millis();
      uint32_t tSaved_Elapsed = millis()-tSaved_millis;
      while(!Handler_ModemResponses(LOG_LEVEL_DEBUG))
      {
        if(
          (tSaved_Elapsed = (millis()-tSaved_millis)) > wait_millis
        ){
          ALOG_ERR(PSTR(D_LOG_CELLULAR "No response (%dms)"), tSaved_Elapsed);
        }

      }

    }

  }

    
}

#endif // USE_MODULE_NETWORK_CELLULAR