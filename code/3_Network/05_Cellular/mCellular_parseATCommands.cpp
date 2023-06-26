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



bool mCellular::parse_ATCommands(char* buffer, uint16_t buflen, uint8_t response_loglevel)
{

  // #ifdef ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
  //AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_CELLULAR "parse_ATCommands %d>> Buffer[%d] \"%s\""),response_loglevel, buflen, buffer);
  // #endif

  // +CDS:  A GSM/GPRS modem or mobile phone uses +CDS to forward a newly received SMS status report to the computer / PC.
  // +CDSI: A GSM/GPRS modem or mobile phone uses +CDSI to notify the computer / PC that a new SMS status report has been received and the memory location where it is stored.
  // +CMT:  A GSM/GPRS modem or mobile phone uses +CMT to forward a newly received SMS message to the computer / PC.
  // +CMTI: A GSM/GPRS modem or mobile phone uses +CMTI to notify the computer / PC that a new SMS message has been received and the memory location where it is stored.
  /**
   * @brief CMT is when messages are set to be forwarded, but may be missed, so leaving this as legacy to match messages
   **/
  char buffer2[300] = {0};
  char *search = "+CMT:"; // A GSM/GPRS modem or mobile phone uses +CMT to forward a newly received SMS message to the computer / PC.
  char *result = strstr(buffer, search);
  if(result)
  {
    ALOG_INF(PSTR(D_LOG_CELLULAR "Parsed \"CMT\" result >>>\n\r%s\n\r<<<"), result);

    // ATResponse_Parse_CMT(buffer, buffer2, sizeof(buffer2));

    // ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Content \"%s\""), buffer2);

    // #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    // // Check for request at GPS
    // if (strcasecmp(buffer2, "GPS") == 0)
    // {
    //   ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for GPS"));
    //   SMS_GPSLocation();
    // }else
    // if (strcasecmp(buffer2, "Batt") == 0)
    // {
    //   ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for Battery"));
    //   SMS_BatteryDetailed();
    // }
    // else{
    //   ALOG_INF(PSTR(D_LOG_CELLULAR "Unknown Message"));
    //   SMS_GPSLocation();
    // }
    // #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS


    // const char* delims = "\r\n"; //space + ,
    // char* tok = strtok(result, delims); // I believe all delims are replaced by NULL?

    // if(tok)
    // {  
    //   /**
    //   for (int i=0;i<5;i++)
    //   {
    //     ALOG_INF(PSTR("tok[%d] = %s"), i, tok?tok:"ERROR");
    //     tok = strtok(NULL, delims);
    //   }

    //   01:06 INF tok[0] = +CMGR: "REC READ","+447515358597",,"23/05/12,21:28:36+04"
    //   01:06 INF tok[1] = Message1
    //   01:06 INF tok[2] = OK
    //   01:06 INF tok[3] = ERROR
    //   01:06 INF tok[4] = ERROR
    //   **/
    

    //   tok = strtok(NULL, delims);   // SMS header
    //   // tok = strtok(NULL, delims); 
    //   char sms_contents[160];

    //   if(tok != NULL) 
    //   {   
    //     snprintf(sms_contents, sizeof(sms_contents), tok); 
    //     ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Message = \"%s\""), sms_contents);

    //     #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    //     if (strcasecmp(sms_contents, "GPS") == 0)
    //     {
    //       ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for GPS"));
    //       SMS_GPSLocation();
    //     }
    //     else
    //     if (strcasecmp(sms_contents, "Batt") == 0)
    //     {
    //       ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for Battery"));
    //       SMS_BatteryDetailed();
    //     }
    //     else
    //     {
    //       ALOG_INF(PSTR(D_LOG_CELLULAR "Unknown Message"));
    //       SMS_GPSLocation();
    //     }
    //     #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

    //   }

    // }


  }
  
  char *result2 = strstr(buffer, "+CMTI:"); 
  if(result2)
  {
    ALOG_INF(PSTR(D_LOG_CELLULAR "Parsed \"CMTI\" result >>>\n\r%s\n\r<<<"), result2);


    const char* delims = ",\r"; //space + ,
    char* tokens = strtok(result2, delims); // I believe all delims are replaced by NULL?

    if(tokens)
    {  
    tokens = strtok(NULL, delims); 
    int new_sms_index = atoi(tokens);

    /**
     * @brief Add unique name
     **/
    
    sms.messages_incoming_index_list.push_back(new_sms_index);
    }



    // /**
    //  * @brief Splitting the Indexs out
    //  **/
    // const char* delims2 = ","; //space + ,
    // char* tok_ids = strtok(buffer_id, delims2); 
    // uint8_t i = 0;
    // sms.messages_incoming_index_list.clear();
    // while(tok_ids){
    //   ALOG_INF(PSTR("tok_ids[%d] = %s"), i++, tok_ids?tok_ids:"ERROR");
    //   sms.messages_incoming_index_list.push_back(atoi(tok_ids));
    //   tok_ids = strtok(NULL, delims2);
    // }



    // ATResponse_Parse_CMT(buffer, buffer2, sizeof(buffer2));

    // ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Content \"%s\""), buffer2);

    // #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    // // Check for request at GPS
    // if (strcasecmp(buffer2, "GPS") == 0)
    // {
    //   ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for GPS"));
    //   SMS_GPSLocation();
    // }
    // else{
    //   ALOG_INF(PSTR(D_LOG_CELLULAR "Unknown Message"));
    //   SMS_GPSLocation();
    // }
    // #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

  }

  /**
   * @brief 
   * +CPMS: used_space1,max_space1,used_space2,max_space2,used_space3,max_space3
   * used_space1 is the number of messages currently stored in the message storage area specified by the message_storage1 parameter.
   * max_space1 is the maximum number of messages that can be stored in the message storage area specified by the message_storage1 parameter. 
   */
  result2 = strstr(buffer, "+CPMS:"); 
  if(result2)
  {
    ALOG_INF(PSTR(D_LOG_CELLULAR "MATCHED \"CPMS\" >>%s<<"), result2);

    const char* delims = ","; //space + ,
    char* tok = strtok(result2, delims); // I believe all delims are replaced by NULL?

    if(tok)
    {  
    tok = strtok(NULL, delims); 
    int used_space1 = atoi(tok);
    tok = strtok(NULL, delims); 
    int max_space1  = atoi(tok);

    ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Stored = %d/%d"), used_space1, max_space1);
    }

    // tok


    // for (int i=0;i<5;i++)
    // {
    //   ALOG_INF(PSTR("tok[%d] = %s"), i, tok);
    //   tok = strtok(NULL, delims);
    // }

  /**
   *                           +CPMS: (storage1a,storage1b,...),(storage2a,storage2b,...),(storage3a,storage3b,...)
   * INF CEL: MATCHED "CPMS" >>+CPMS: "SM",5,10,"SM",5,10,"SM",5,10
   * OK<<
   * 16:35 INF tok[0] = 5
   * 16:35 INF tok[1] = 10
   * 16:35 INF tok[2] = "SM"
   * 16:35 INF tok[3] = 5
   * 16:35 INF tok[4] = 10 
   */



    // ATResponse_Parse_CMT(buffer, buffer2, sizeof(buffer2));

    // ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Content \"%s\""), buffer2);

    // #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    // // Check for request at GPS
    // if (strcasecmp(buffer2, "GPS") == 0)
    // {
    //   ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for GPS"));
    //   SMS_GPSLocation();
    // }
    // else{
    //   ALOG_INF(PSTR(D_LOG_CELLULAR "Unknown Message"));
    //   SMS_GPSLocation();
    // }
    // #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

  }

  result2 = strstr(buffer, "+CMGR:"); 
  if(result2)
  {
    ALOG_HGL(PSTR(D_LOG_CELLULAR "MATCHED \"CMGR\"\n\r>>%s<<"), result2);

    const char* delims = "\r\n"; //space + ,
    char* tok = strtok(result2, delims); // I believe all delims are replaced by NULL?

    if(tok)
    {  
      /**
      for (int i=0;i<5;i++)
      {
        ALOG_INF(PSTR("tok[%d] = %s"), i, tok?tok:"ERROR");
        tok = strtok(NULL, delims);
      }

      01:06 INF tok[0] = +CMGR: "REC READ","+447515358597",,"23/05/12,21:28:36+04"
      01:06 INF tok[1] = Message1
      01:06 INF tok[2] = OK
      01:06 INF tok[3] = ERROR
      01:06 INF tok[4] = ERROR
      **/
    

      tok = strtok(NULL, delims);   // SMS header
      // tok = strtok(NULL, delims); 
      char sms_contents[160];

      if(tok != NULL) 
      {   
        snprintf(sms_contents, sizeof(sms_contents), tok); 
        ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Message = \"%s\""), sms_contents);

        #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
        if (strcasecmp(sms_contents, "GPS") == 0)
        {
          ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for GPS"));
          SMS_GPSLocation();
        }
        else
        if (strcasecmp(sms_contents, "Batt") == 0)
        {
          ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for Battery"));
          SMS_BatteryDetailed();
        }
        else
        if (strstr(sms_contents, "Command") != NULL)
        {
          ALOG_INF(PSTR(D_LOG_CELLULAR "Future Command with format \"Command KEY VALUE\" that will be shaped into JSON and send to parser"));
          SMS_CommandIntoJSONCommand(sms_contents);
        }
        else
        {
          ALOG_INF(PSTR(D_LOG_CELLULAR "Unknown Message"));
          SMS_GPSLocation();
        }
        #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

      }

    }

  }


  if(strstr(buffer, "+CMGD:"))
  {
    #ifdef ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
    ALOG_INF(PSTR("ATParse_CMGD__CommandNameInTextDeleteMessage[%d] %s"), buflen , buffer);
    #endif
    ATParse_CMGD__CommandNameInTextDeleteMessage(buffer, buflen, response_loglevel);
  }

}



void mCellular::ATParse_CMGD__CommandNameInTextDeleteMessage(char* buffer, uint8_t buflen, uint8_t response_loglevel)
{

  char* result2 = strstr(buffer, "+CMGD:"); 
  if(result2)
  {
    #ifdef ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
    ALOG_INF(PSTR("ATParse_CMGD__CommandNameInTextDeleteMessage S %s"),result2);
    #endif

    const char* delims = "()"; //space + ,
    char* tok = strtok(result2, delims); // I believe all delims are replaced by NULL?

    if(tok)
    {                 
      // 00:23 INF tok[0] = +CMGD: 
      // 00:23 INF tok[1] = 0,1,2,4,5
      // 00:23 INF tok[2] = ,
      // 00:23 INF tok[3] = 0-4
      // 00:23 INF tok[4] =       

      ALOG_DBG(PSTR(D_LOG_CELLULAR "MATCHED \"CMGD\" >>%s<<"), result2);

      tok = strtok(NULL, delims); //skip 

      if(tok)
      {

        char buffer[100];
        snprintf(buffer, sizeof(buffer), tok); // 0,1,2,4,5
        AddLog(response_loglevel,PSTR("hereeeeeeeeeeeeeeeeeeeeeeeeee buffer split = %s"), buffer);

        /**
         * @brief Splitting the Indexs out
         **/
        const char* delims2 = ","; //space + ,
        char* tok_ids = strtok(buffer, delims2); 
        uint8_t i = 0;
        sms.messages_incoming_index_list.clear();
        while(tok_ids){
          AddLog(response_loglevel,PSTR("CommandNameInTextDeleteMessage SMS IDs[%d] = %s"), i++, tok_ids?tok_ids:"ERROR");
          sms.messages_incoming_index_list.push_back(atoi(tok_ids));
          tok_ids = strtok(NULL, delims2);
        }

      }

    }
  }

    #ifdef ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
    ALOG_INF(PSTR("ATParse_CMGD__CommandNameInTextDeleteMessage E"));
    #endif

}



#endif // USE_MODULE_NETWORK_CELLULAR