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


/**
 * @brief 
 * 
 * If modem connection is handled by another task pinned to the other core, then any slow downs will not hold the main processing back
 * Leave this until I better understand pinning tasks and can make them work well
 * 
 */

#ifdef USE_MODULE_NETWORK_CELLULAR

const char* mCellular::PM_MODULE__NETWORK_CELLULAR__CTR = D_MODULE__NETWORK_CELLULAR__CTR;
const char* mCellular::PM_MODULE__NETWORK_CELLULAR__FRIENDLY_CTR = D_MODULE__NETWORK_CELLULAR__FRIENDLY_CTR;

int8_t mCellular::Tasker(uint8_t function, JsonParserObject obj)
{

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
    case FUNC_INIT:
      Init();
    break;
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP: 
    {      
      Handler_ModemResponses_Fast();
    }
    break;
    case FUNC_EVERY_SECOND: 

      //     ALOG_INF(PSTR("analogRead \t\t\t\t%d"),analogRead(35));
      // Serial.printf("adc1_get_raw(ADC1_CHANNEL_7) = %d\n\r", adc1_get_raw(ADC1_CHANNEL_7));
      
      #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
      ModemUpdate_GPS();
      #endif

      ModemUpdate_GPRS();
      
      ModemUpdate_SMS();
      
      if(modem)
      {
        modem->maintain();
      }

    break;
    case FUNC_EVERY_FIVE_SECOND:   

      // ALOG_INF(PSTR("Network connected? %d"), modem->isNetworkConnected());
    
    break;
    case FUNC_EVERY_MINUTE:

      ModemUpdate_BatteryStatus();

      #ifdef ENABLE_DEVFEATURE_TEXT_LOCATION_EVERY_MINUTE_WHEN_POWERED
      SMS_GPSLocation();
      #endif 

      // SendATCommand_SMSFormatAscii(); //tmp

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
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Set_DefaultPeriodRate();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_RefreshAll();
    break;
    #endif //USE_MODULE_NETWORK_MQTT    
  }

} // END function


void mCellular::Pre_Init(void){
  
  settings.fEnableSensor = true;

  pinMode(12, OUTPUT);
  
}


void mCellular::Init(void)
{

  modem = new TinyGsm(SerialAT);

  #ifdef USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED


  /**
   * @brief Change from default
   **/
  SerialAT.begin(115200, SERIAL_8N1, PIN_RX, PIN_TX); // #define UART_BAUD   115200
  delay(500);
  if (modem->testAT()) { 
    ALOG_INF(PSTR("Baud Found: %d"), 115200);
  }else{
    ALOG_INF(PSTR("Baud not Found: %d"), 115200);
  }

  /**
   * @brief Change from default
   **/
  modem->setBaud(UART_CELLULAR_BAUD);

  SerialAT.begin(UART_CELLULAR_BAUD, SERIAL_8N1, PIN_RX, PIN_TX); // #define UART_BAUD   115200
  if (modem->testAT()) { 
    ALOG_INF(PSTR("Baud Found: %d"), UART_CELLULAR_BAUD);
  }else{
    ALOG_INF(PSTR("Baud not Found: %d"), UART_CELLULAR_BAUD);
  }

  #else 
  SerialAT.begin(115200, SERIAL_8N1, PIN_RX, PIN_TX);
  #endif // USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED


  /**
   * @brief Power on  (move into templates)
   * 
   */
  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, HIGH);
  delay(300);
  digitalWrite(PWR_PIN, LOW);

  /**
   * @brief Move into UART config to "claim" for tinygsm
   */
  // SerialAT.begin(UART_CELLULAR_BAUD, SERIAL_8N1, PIN_RX, PIN_TX); // #define UART_BAUD   115200

  #ifdef ENABLE_DEVFEATURE_SIM7000G_INIT_SKIP_MODEM_RESTART
  ALOG_INF(PSTR("Faster init modem"));
  if (!modem->init()) {
    ALOG_INF(PSTR("Failed to init modem"));
  }
  #else
  // Restart takes quite some time, to skip it, call init() instead of restart()
  ALOG_INF(PSTR("Restarting modem (may take 60 seconds)"));
  if (!modem->restart()) {
    ALOG_INF(PSTR("Failed to restart modem, attempting to continue without restarting"));
  }
  #endif
  ALOG_INF(PSTR("Faster init modem DONE"));

  GPRS_Enable();

  #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
  GPS_Enable();
  #endif 

  #ifdef ENABLE_DEVFEATURE_CELLULAR_SMS__PDU_MODE
  SendATCommand_SMSFormatPDU();
  #else
  SendATCommand_SMSFormatAscii();
  #endif

  SendATCommand_SMSImmediateForwardOverSerial();


}



void mCellular::Handler_ModemResponses()
{

  // // Response
  // String incoming = String();
  // // SerialAT.setTimeout(200);
  // if (SerialAT.available()) 
  // {


  //   uint32_t receive_millis = millis();

  //   // ALOG_INF(PSTR("millisA = %d"), millis());

  //   incoming = SerialAT.readString();
  //   ALOG_HGL(PSTR("Incoming \n\r===(%s)==="), incoming.c_str());

  //   ALOG_INF(PSTR("millisB = %d\tR\t%d"), millis(), millis()-receive_millis);
  //   char buffer[300];
  //   char buffer2[300];
  //   // ALOG_INF(PSTR("millisC = %d"), millis());

  //   sprintf(buffer, "%s", incoming.c_str());

  //   // ALOG_INF(PSTR("buffer = %s"), buffer);
  //   // for(int i = 0;i < incoming.length(); i++)
  //   // {
  //   //   Serial.printf("%02d>    %c       \n\r",i,buffer[i]);
  //   //   if(buffer[i]=='\n')
  //   //   {
  //   //     Serial.printf("=====================%02d>\"%c\"\n\r",i,buffer[i]);
  //   //   }
  //   // }
    
  //   char *search = "\r\n+CMT";
  //   char *result = strstr(buffer, search);
  //   if(result)
  //   {
  //     ALOG_INF(PSTR("FOUND CMT MESSAGE result >>>%s<<<"), result);
  //     ATResponse_Parse_CMT(buffer, buffer2, sizeof(buffer2));
  //   }


  //   ALOG_INF(PSTR("buffer2 >>>%s<<<"), buffer2);
    
  //   // ALOG_INF(PSTR("millisD = %d"), millis());

  //   #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
  //   // Check for request at GPS
  //   if (strncasecmp(buffer2, "GPS", 3) == 0)
  //   {
  //     ALOG_INF(PSTR("Request for GPS"));
  //     SMS_GPSLocation();
  //   }
  //   else{
  //     ALOG_INF(PSTR("NO Request for GPS: Sending anyway for now"));
  //     // SMS_GPSLocation();
  //   }
  //   #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

  // }

}


// String Stream::readString()
// {
//     String ret;
//     int c = timedRead();
//     while(c >= 0) {
//         ret += (char) c;
//         c = timedRead();
//     }
//     return ret;
// }




void mCellular::Handler_ModemResponses_Fast()
{

  // Response
  String incoming = String();
  // SerialAT.setTimeout(200);


  if (SerialAT.available()) 
  {

    uint32_t receive_millis = millis();
    // ALOG_INF(PSTR("millisA = %d"), millis());

    uint16_t buflen = 0;
    char buffer[300] = {0};

    while(SerialAT.available())
    {
      buffer[buflen++] = SerialAT.read();
      if(buflen>(sizeof(buffer)-1))
      {
        ALOG_ERR(PSTR("SerialAT overflow"));
      }
    }

    // +CDS:  A GSM/GPRS modem or mobile phone uses +CDS to forward a newly received SMS status report to the computer / PC.
    // +CDSI: A GSM/GPRS modem or mobile phone uses +CDSI to notify the computer / PC that a new SMS status report has been received and the memory location where it is stored.
    // +CMT:  A GSM/GPRS modem or mobile phone uses +CMT to forward a newly received SMS message to the computer / PC.
    // +CMTI: A GSM/GPRS modem or mobile phone uses +CMTI to notify the computer / PC that a new SMS message has been received and the memory location where it is stored.
    /**
     * @brief Temporary method expecting just SMS, later the "CMT" portion of the command needs parsed out
     * 
     */
    char buffer2[160] = {0}; 
    // char *search = "\r\n+CMT";
    char *search = "+CMT"; // A GSM/GPRS modem or mobile phone uses +CMT to forward a newly received SMS message to the computer / PC.
    char *result = strstr(buffer, search);
    if(result)
    {
      ALOG_INF(PSTR("FOUND CMT MESSAGE result >>>%s<<<"), result);
      ATResponse_Parse_CMT(buffer, buffer2, sizeof(buffer2));

      ALOG_INF(PSTR("buffer2 >>>%s<<<"), buffer2);

      #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
      // Check for request at GPS
      if (strncasecmp(buffer2, "GPS", 3) == 0)
      {
        ALOG_INF(PSTR("Request for GPS"));
        SMS_GPSLocation();
      }
      else{
        ALOG_INF(PSTR("Unknown Message"));
        SMS_GPSLocation();
      }
      #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS


    }

    
    // ALOG_INF(PSTR("millisD = %d"), millis());


  }

}



void mCellular::Handler_ModemResponses_Fast_PDU()
{

  // Response
  // String incoming = String();
  // SerialAT.setTimeout(200);


/**
 * @brief 
 * 
  >>> Example: Test from my phone "0000012345432100000" <<<
 
 +CMT: ,36
  0791448720003023040C914457515358790000325060717104401330180C068BC966B41A6D268BC16030180C
 
  // Decoded with https://www.diafaan.com/sms-tutorials/gsm-modem-tutorial/online-sms-pdu-decoder/
  Text message
  From:	+447515358597
  Message:	0000012345432100000

  Additional information
  PDU type:	SMS-DELIVER
  Time stamp:	06/05/2023 17:17:40
  SMSC:	+447802000332
  Data coding:	SMS Default Alphabet

  Original Encoded PDU fields
  SMSC:	0791448720003023
  PDU header:	04
  TP-MTI:	00
  TP-MMS:	04
  TP-SRI:	00
  TP-RP:	00
  TP-UDHI:00
  TP-OA:	0C91445751535879
  TP-PID: 00
  TP-DCS: 00
  TP-SCTS:32506071710440
  TP-UDL:	13
  TP-UD:	30180C068BC966B41A6D268BC16030180C
    
  07 - Length of the SMSC information (in this case 7 octets)
  91 - Type-of-address of the SMSC. (91 means international format of the phone number)
  44 87 20 00 30 23 - Service center number(in decimal semi-octets). The length of the phone  number is odd (11), so a trailing F has been added to form proper octets. 
                      The  phone number of this service center is "+27831000015". See below.
  04 - First octet of this SMS-DELIVER message.
  0C - Address-Length. Length of the sender number (0B hex = 11 dec)
  91 - Type-of-address of the sender number
  44 57 51 53 58 79 - Sender number (decimal semi-octets), with a trailing F
  00
  00
  32 50 60 71 71 04 40
  13
  30180C068BC966B41A6D268BC16030180C

***/




  if (SerialAT.available()) 
  {


    uint32_t receive_millis = millis();

    Serial.write(Serial1.read());

    // ALOG_INF(PSTR("millisA = %d"), millis());

    // char buffer[300];

    // // Add timeout
    // int c = SerialAT.read();
    // while(c >= 0) {
    //   incoming += (char) c;
    //   c = SerialAT.read();
    // }
    
    // // ALOG_HGL(PSTR("while\nIncoming \n\r===(%s)==="), incoming.c_str());

    // // ALOG_HGL(PSTR("Incoming \n\r===(%s)==="), incoming.c_str());

    // // ALOG_INF(PSTR("millisB = %d\tR\t%d"), millis(), millis()-receive_millis);
    // char buffer2[300];
    // // ALOG_INF(PSTR("millisC = %d"), millis());

    // sprintf(buffer, "%s", incoming.c_str());

    // ALOG_INF(PSTR("buffer = %s"), buffer);
    // for(int i = 0;i < incoming.length(); i++)
    // {
    //   Serial.printf("%02d>    %c       \n\r",i,buffer[i]);
    //   if(buffer[i]=='\n')
    //   {
    //     Serial.printf("=====================%02d>\"%c\"\n\r",i,buffer[i]);
    //   }
    // }

    // char *search = "\r\n+CMT";
    // char *result = strstr(buffer, search);
    // if(result)
    // {
    //   ALOG_INF(PSTR("FOUND CMT MESSAGE result >>>%s<<<"), result);
    //   ATResponse_Parse_CMT(buffer, buffer2, sizeof(buffer2));
    // }

    // ALOG_INF(PSTR("buffer2 >>>%s<<<"), buffer2);
    
    // // ALOG_INF(PSTR("millisD = %d"), millis());

    // #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    // // Check for request at GPS
    // if (strncasecmp(buffer2, "GPS", 3) == 0)
    // {
    //   ALOG_INF(PSTR("Request for GPS"));
    //   SMS_GPSLocation();
    // }
    // else{
    //   ALOG_INF(PSTR("Unknown Message"));
    //   // SMS_GPSLocation();
    // }
    // #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

  }

}


char* mCellular::ATResponse_Parse_CMT(char* incoming, char *parsed_buf, uint16_t parsed_buflen)
{

  ALOG_INF(PSTR("ATResponse_Parse_CMT"));

  char *result = strstr(
    &incoming[2],   // skip "\r\n" at start of response
    "\r\n"          // search for split between header/body of SMS
  );

  if(result)
  {
    ALOG_INF(PSTR("ATResponse_Parse_CMT::result \"%s\""), &result[2]); //skip token
    if(parsed_buf != nullptr)
    {
      snprintf(parsed_buf, strlen(&result[2])-1, &result[2]); // Copy and remove the trailing "\r\n"
    }
  }

}




void mCellular::SendATCommand_SMSFormatAscii()
{
  // Enable SMS always send to serial when they arrive, move to function later
  modem->sendAT("+CMGF=1 "); // Set the ascii messages (not HEX)
  if (modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT) != 1) {
    ALOG_INF(PSTR("SMSFormatAscii DONE"));
  }
}


void mCellular::SendATCommand_SMSFormatPDU()
{
  // Enable SMS always send to serial when they arrive, move to function later
  modem->sendAT("+CMGF=0 "); // Set the ascii messages (not HEX)
  if (modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT) != 1) {
    ALOG_INF(PSTR("SMSFormatPDU DONE"));
  }
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
  modem->sendAT("+CNMI= 2,2,0,0,0 ");
  if (modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT) != 1) {
    ALOG_INF(PSTR("SMSImmediateForwardOverSerial"));
  }
}

/**
 * @brief 
 * Response syntax:
  +CFUN: <fun>
  The read response parameter and its defined value are the following:
  <fun> 
    0 – Power off and store. RF circuits are disabled by deactivating LTE and Global Navigation Satellite System (GNSS) services.
    1 – Normal mode. The active mode is either LTE or GNSS, or both. Full functional mode. Active modes depend on %XSYSTEMMODE setting.
    2 – Receive only mode in active system mode. Active modes depend on %XSYSTEMMODE setting.
    4 – Flight mode. RF circuits are disabled by deactivating LTE and GNSS services. 
  **/
void mCellular::SendATCommand_FunctionalityMode_Minimum()
{
  modem->sendAT("+CFUN=0 "); 
  if (modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT) != 1) {
    ALOG_INF(PSTR(" +CFUN=0  false "));
  }
}

void mCellular::SendATCommand_FunctionalityMode_Full()
{
  modem->sendAT("+CFUN=1 ");
  if (modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT) != 1) 
  {
    ALOG_INF(PSTR(" +CFUN=1  false "));
  }
}



void mCellular::ModemUpdate_BatteryStatus()
{
  modem_status.battery.isvalid = modem->getBattStats(
    modem_status.battery.charge_state, 
    modem_status.battery.percentage, 
    modem_status.battery.volts_mv
  );

  ALOG_DBM(PSTR("{\"Volts_mv\":%d,\"Percent\":%d,\"Charging\":%d}"),
    modem_status.battery.volts_mv,modem_status.battery.percentage,modem_status.battery.charge_state);


  #ifdef USE_MODULE_SENSORS_BATTERY_MODEM
  pCONT_batt_modem->readings.battery.volts_mv = modem_status.battery.volts_mv;
  pCONT_batt_modem->readings.battery.percentage = modem_status.battery.percentage;
  #endif

}


void mCellular::modemPowerOn()
{
  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, LOW);
  delay(100);    // Datasheet T_on = 72ms
  digitalWrite(PWR_PIN, HIGH);
}

void mCellular::modemPowerOff()
{
  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, LOW);
  delay(1500);    // Datasheet T_off = 1.2secs
  digitalWrite(PWR_PIN, HIGH);
}

void mCellular::modemRestart()
{
  modemPowerOff();
  delay(1000);
  modemPowerOn();
}


// ==============================================================================================

#ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

void mCellular::GPS_Enable()
{
  if(modem)
  {
    // Set SIM7000G GPIO4 LOW ,turn on GPS power
    // CMD:AT+SGPIO=0,4,1,1
    // Only in version 20200415 is there a function to control GPS power
    modem->sendAT("+SGPIO=0,4,1,1");
    if (modem->waitResponse(10000L) != 1) {
      ALOG_INF(PSTR(" SGPIO=0,4,1,1 false "));
    }
    modem->enableGPS();
    gps.enabled = true;
  }
}


void mCellular::GPS_Disable()
{
  // Set SIM7000G GPIO4 LOW ,turn off GPS power
  // CMD:AT+SGPIO=0,4,1,0
  // Only in version 20200415 is there a function to control GPS power
  modem->sendAT("+SGPIO=0,4,1,0");
  if (modem->waitResponse(10000L) != 1) {
    ALOG_INF(PSTR(" SGPIO=0,4,1,0 false "));
  }
  modem->disableGPS();
  gps.enabled = false;
}


void mCellular::ModemUpdate_GPS()
{
  if(mTime::TimeReached(&gps.tReached_Update, 1000))
  {
    if(gps.enabled)
    {
      
      if(modem->getGPS_Detailed(
        &gps.latitude, 
        &gps.longitude, 
        &gps.speed, 
        &gps.altitude, 
        &gps.vsat, 
        &gps.usat, 
        &gps.accuracy, 
        &gps.year, 
        &gps.month, 
        &gps.day, 
        &gps.hour, 
        &gps.minute, 
        &gps.second, 
        &gps.course, 
        &gps.FixMode, 
        &gps.accuracy_position, 
        &gps.accuracy_vertical, 
        &gps.usat_glonass, 
        &gps.cno_max, 
        &gps.HPA, 
        &gps.VPA
      )) 
      {
        ALOG_INF(PSTR("GPS u/v_sat %d/%d Fix (%d cm)"), gps.usat, gps.vsat, (int)(gps.accuracy*100));
        
        #ifdef USE_MODULE_SENSORS_GPS_MODEM
        pCONT_gps->readings.update_seconds = millis();
        pCONT_gps->location.latitude  = gps.latitude;
        pCONT_gps->location.longitude = gps.longitude;
        pCONT_gps->location.speed = gps.speed;
        pCONT_gps->location.altitude = gps.altitude;
        pCONT_gps->location.accuracy = gps.accuracy;
        #endif // USE_MODULE_SENSORS_GPS_MODEM
      
      }else{
        ALOG_ERR(PSTR("GPS NO FIX"));
        // GPS_Enable(); //need a backoff timer but otherwise should force it
      }

            
        
    }
  }

}

#endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

// ==============================================================================================


void mCellular::GPRS_Connect()
{
  if(modem)
  {

    ALOG_HGL(PSTR("\n\n\nRECONNECTING Waiting for network..."));
    if (!modem->waitForNetwork(DEFAULT_AT_COMMAND_RESPONSE_WAIT)) 
    {
      return;
    }

    if (modem->isNetworkConnected()) 
    {
      ALOG_INF(PSTR("Network connected"));
    }


    ALOG_INF(PSTR("Connecting to: %s"), CELLULAR_APN);
    if (!modem->gprsConnect(CELLULAR_APN, CELLULAR_GPRS_USERNAME, CELLULAR_GPRS_PASSWORD)) 
    {
      return;
    }

    ALOG_INF(PSTR("GPRS Checking..."));
    if (modem->isGprsConnected()) 
    {
      ALOG_INF(PSTR("GPRS Connected"));
      gsm_client = new TinyGsmClient(*modem);
        ALOG_INF(PSTR("MQTT size %d"), pCONT_mqtt->brokers.size());
      if(pCONT_mqtt->brokers.size()<1)
      {
        // TMP fix
        pCONT_mqtt->brokers.push_back(new MQTTConnection());


        ALOG_INF(PSTR("MQTT not working push_back(new MQTTConnection())"));
      }
      pCONT_mqtt->brokers[0]->SetPubSubClient(gsm_client);
    } 
    else 
    {
      ALOG_INF(PSTR("GPRS NOT Connected"));
    }

    // String ccid = modem->getSimCCID();
    // ALOG_INF(PSTR("CCID: %s"), ccid.c_str());

    // String imei = modem->getIMEI();
    // ALOG_INF(PSTR("IMEI: %s"), imei.c_str());

    // String cop = modem->getOperator();
    // ALOG_INF(PSTR("Operator: %s"), cop.c_str());

    // IPAddress local = modem->localIP();
    // ALOG_INF(PSTR("Local IP: %s"), String(local).c_str());

    // int csq = modem->getSignalQuality();
    // ALOG_INF(PSTR("Signal quality: %d"), csq);

  }

}


void mCellular::GPRS_Enable()
{
  if(modem)
  {
    
    ALOG_INF(PSTR("> Check whether Modem is online"));
    
    //test modem is online ?
    uint32_t  timeout = millis();
    while (!modem->testAT()) 
    {
      Serial.print(".");
      if (millis() - timeout > 10000 ) 
      {
        ALOG_INF(PSTR("> It looks like the modem is not responding, trying to restart"));
        modemPowerOff();
        delay(5000);
        modemPowerOn();
        timeout = millis();
      }
    }

    ALOG_INF(PSTR("\nModem is online"));

    timeout = millis();
    ALOG_INF(PSTR("> Get SIM card status"));
    while (modem->getSimStatus() != SIM_READY) {
      Serial.print(".");
      if (millis() - timeout > 10000 ) {
          ALOG_INF(PSTR("It seems that your SIM card has not been detected. Has it been inserted?"));
          return;
      }
    }
    
    //Set mobile operation band
    modem->sendAT("+CBAND=ALL_MODE");
    modem->waitResponse();

    // Args:
    // 1 CAT-M
    // 2 NB-IoT
    // 3 CAT-M and NB-IoT
    // Set network preferre to auto
    uint8_t perferred = 3;
    modem->setPreferredMode(perferred);

    // Args:
    // 2 Automatic
    // 13 GSM only
    // 38 LTE only
    // 51 GSM and LTE only
    // Set network mode to auto
    modem->setNetworkMode(2);

    // Check network signal and registration information
    ALOG_INF(PSTR("> SIM7000/SIM7070 uses automatic mode to access the network. The access speed may be slow. Please wait patiently"));
    RegStatus status;
    timeout = millis();
    do {
      int16_t sq =  modem->getSignalQuality();

      status = modem->getRegistrationStatus();

      if (status == REG_DENIED) {
        ALOG_INF(PSTR("> The SIM card you use has been rejected by the network operator. Please check that the card you use is not bound to a device!"));
        return;
      } else {
        ALOG_INF(PSTR("Signal sq %d"), sq);
      }

      if (millis() - timeout > 10000 ) { //!! THIS IS BLOCKING CODE, NEEDS RESOLVED TO RETRY AGAIN
        if (sq == 99) {
          ALOG_INF(PSTR("It seems that there is no signal."));
          return;
        }
        timeout = millis();
      }

      delay(800);
    } while (status != REG_OK_HOME && status != REG_OK_ROAMING);


    String res;

    ALOG_INF(PSTR("Obtain the APN issued by the network"));
    modem->sendAT("+CGNAPN");
    if (modem->waitResponse(3000, res) == 1) 
    {
      res = res.substring(res.indexOf(",") + 1);
      res.replace("\"", "");
      res.replace("\r", "");
      res.replace("\n", "");
      res.replace("OK", "");
      ALOG_INF(PSTR("The APN issued by the network is: %s"), res.c_str());
    }

    modem->sendAT("+CNACT=1");
    modem->waitResponse();

    modem->sendAT("+CNACT?");
    if (modem->waitResponse("+CNACT: ") == 1) 
    {
      modem->stream.read();
      modem->stream.read();
      res = modem->stream.readStringUntil('\n');
      res.replace("\"", "");
      res.replace("\r", "");
      res.replace("\n", "");
      modem->waitResponse();
      ALOG_INF(PSTR("The current network IP address is: %s"), res.c_str());
    }

    GPRS_Connect();

    modem->sendAT("+CPSI?");
    if (modem->waitResponse("+CPSI: ") == 1) 
    {
      res = modem->stream.readStringUntil('\n');
      res.replace("\r", "");
      res.replace("\n", "");
      modem->waitResponse();
      ALOG_INF(PSTR("The current network parameter is: %s"), res.c_str());
    }
    
    gprs.enabled = true;
  }
}


void mCellular::GPRS_Disable()
{
  
  gprs.enabled = false;
}


void mCellular::ModemUpdate_GPRS()
{
  if(mTime::TimeReached(&gprs.tReached_Update, 5000))
  {
    if(gprs.enabled)
    {
      
      /**
       * @brief Check connection is still active
       */              
      if (modem->isGprsConnected()) 
      {

        int csq = modem->getSignalQuality();


        gprs.signal_quality_raw = csq;

        if(csq == 99)
        {
          gprs.signal_quality_rssi_dbm = -150;
          pCONT_interface_network->data.cellular_state.isvalid = false;
          pCONT_interface_network->data.mqtt_state.isvalid = false;
        }
        else
        {
          gprs.signal_quality_rssi_dbm = map(csq, 0, 31, -113, -51);
          pCONT_interface_network->data.cellular_state.isvalid = true;
        }

        ALOG_INF(PSTR("Signal quality: %d dBm (%d)"), int(gprs.signal_quality_rssi_dbm), csq);


        gprs.connected_seconds++;
        gprs.last_comms_millis_updated = millis();

      } 
      else 
      {
        ALOG_INF(PSTR("GPRS status: NOT connected"));

        // if(gprs.connected_seconds>1) //previously connected
        // {
        //   pCONT->Tasker_Interface(FUNC_ALERT_NETWORK_CONNECTION_LOST);
        // }            

        gprs.connected_seconds = 0;
        GPRS_Connect();
      }

    }
    else
    {
      GPRS_Enable();
    }
  }

}


void mCellular::SMS_Enable()
{
  if(modem)
  {
      
    String name = modem->getModemName();
    ALOG_INF(PSTR("Modem Name: %s"),name.c_str());

    String modemInfo = modem->getModemInfo();
    ALOG_INF(PSTR("Modem Info: %s"),modemInfo.c_str());

    SendATCommand_FunctionalityMode_Minimum();

    SendATCommand_SMSFormatAscii();

    /*
      2 Automatic
      13 GSM only
      38 LTE only
      51 GSM and LTE only
    * * * */
    String res;
    res = modem->setNetworkMode(2);
    if (res != "1") {
        ALOG_INF("setNetworkMode  false ");
        return ;
    }

    /**
     * @brief 
      Set the preferred selection between CAT-M and NB-IOT
      1 CAT-M
      2 NB-Iot
      3 CAT-M and NB-IoT
    **/
    res = modem->setPreferredMode(3);
    if (res != "1")
    {
      ALOG_INF(PSTR("setPreferredMode false"));
    }

    SendATCommand_FunctionalityMode_Full();    

    SendATCommand_SMSImmediateForwardOverSerial();      

    sms.enabled = true;
  }
}


void mCellular::SMS_Disable()
{
  
  sms.enabled = false;
}


void mCellular::ModemUpdate_SMS()
{
  if(mTime::TimeReached(&gprs.tReached_Update, 1000))
  {
    if(sms.enabled)
    {
      
    }
  }

}

#ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
void mCellular::SMS_GPSLocation()
{
  if(modem)
  {
    ALOG_INF(PSTR("SMS: Sending GPS Location"));
        
    // --------TESTING SENDING SMS--------

    // https://www.google.com/maps/dir//54.505,-6.299
    // https://www.google.com/maps/dir//54.505044,-6.298891

    char convf_lat[TBUFFER_SIZE_FLOAT];
    mSupport::float2CString(gps.latitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat);
    char convf_lon[TBUFFER_SIZE_FLOAT];
    mSupport::float2CString(gps.longitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon);
    char convf_fix[TBUFFER_SIZE_FLOAT];
    mSupport::float2CString(gps.accuracy,2,convf_fix);

    uint16_t buflen = 0;
    char     buffer[STANDARD_SMS_CHAR_LENGTH];

    buflen += snprintf_P(buffer+buflen, sizeof(buffer),
      PSTR(
        // "Battery  %d mV\n"
        // "Battery  %d mA\n"
        // "Mission  %d (%dm)\n"
        // "PKT Age  %d\n"
        "Accuracy %s m\n"
        "\n"
        "https://www.google.com/maps/dir//%s,%s"
      ), 
      // pCONT_mavlink->pkt.battery_status.data.battery_remaining,
      // pCONT_mavlink->pkt.battery_status.data.current_consumed,
      // pCONT_mavlink->pkt.mission_current.data.seq,
      // pCONT_mavlink->pkt.nav_controller_output.data.wp_dist,
      // millis()-pCONT_mavlink->pkt.tSaved_Last_Response,
      convf_fix,
      convf_lat, 
      convf_lon
    );

    #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
    /**
     * @brief MAVLink Data
     **/    
    char convf_lat2[TBUFFER_SIZE_FLOAT];
    mSupport::float2CString(pCONT_mavlink->pkt.gps_raw_int.data.lat,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat2);
    char convf_lon2[TBUFFER_SIZE_FLOAT];
    mSupport::float2CString(pCONT_mavlink->pkt.gps_raw_int.data.lon,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon2);

    buflen += snprintf_P(buffer+buflen, sizeof(buffer),
      PSTR(
        "\n"
        "MAV\n"
        "https://www.google.com/maps/dir//%s,%s"
      ), 
      convf_lat2, 
      convf_lon2
    );
    #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER
    
    ALOG_INF(PSTR("buffer  %s"),buffer);
    
    String res;
    res = modem->sendSMS(SMS_TARGET, String(buffer));
    ALOG_INF(PSTR("SMS:"), res ? "OK" : "fail");

  }
}
#endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

void mCellular::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

  
  if(jtok = obj["OpenHAB"].getObject()["SystemTime"]){
    ALOG_INF( PSTR("OpenHAB:SystemTime"));
    ALOG_INF(PSTR("OH Time %s"),jtok.getStr());
    //toggle led
    DIGITAL_INVERT_PIN(12); //esp32 blue led
  }

  #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
  if(jtok = obj["GPS_Enable"]){
    ALOG_INF( PSTR("GPS_Enable"));
    if(jtok.getInt() == 1)
    {
      GPS_Enable();
    }
  }
  #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS


  if(jtok = obj["GPRS_Enable"]){
    ALOG_INF( PSTR("GPRS_Enable"));
    if(jtok.getInt() == 1)
    {
      GPRS_Enable();
    }
  }


  if(jtok = obj["SMS_Enable"]){
    ALOG_INF( PSTR("SMS_Enable"));
    if(jtok.getInt() == 1)
    {
      SMS_Enable();
    }
  }

  #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
  if(jtok = obj["SMS_GPSLocation"]){
    ALOG_INF( PSTR("SMS_GPSLocation"));
    if(jtok.getInt() == 1)
    {
      SMS_GPSLocation();
    }
  }
  #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

  
  if(jtok = obj["BattVolt"]){
    ALOG_INF( PSTR("BattVolt"));
    if(jtok.getInt() == 1)
    {    
      // read the battery voltage and percentage
      uint16_t vbat = modem->getBattVoltage();        
      ALOG_INF(PSTR("%d mV"), vbat);
    }
  }


  if(jtok = obj["ADCVoltage"]){
    ALOG_INF( PSTR("ADCVoltage"));
    if(jtok.getInt() == 1)
    {      
      ModemUpdate_BatteryStatus();
      ALOG_INF(PSTR("{\"volts_mv\":%d,\"percent\":%d,\"milliVolts\":%d}"),
        modem_status.battery.volts_mv,modem_status.battery.percentage,modem_status.battery.charge_state);
    }
  }


  if(jtok = obj["ATCommands"]){
    ALOG_INF( PSTR("ATCommands"));
    JsonParserArray array = jtok;
    for(auto val : array) {
      AddLog(LOG_LEVEL_INFO, PSTR("F::%s %s"),__FUNCTION__,val.getStr());
      SerialAT.println(val.getStr());  
      delay(500);
      if (SerialAT.available()) {
        String r = SerialAT.readString();
        ALOG_INF(PSTR("ATResponse = \"%s\""), r.c_str());
      }
    }

  }

    
}
  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mCellular::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_JSON_COUNT, settings.leds_found);
  return JBI->End();

}


uint8_t mCellular::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[100] = {0};

  JBI->Start();

    JBI->Level_Start("GPRS");
        JBI->Add("ConSec", gprs.connected_seconds);
    JBI->Level_End();
    #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    JBI->Level_Start("GPS");
      JBI->Add("Enabled", gps.enabled);
      JBI->Add("Latitude", gps.latitude);
      JBI->Add("Longitude", gps.longitude);
      JBI->Add("Speed", gps.speed);
      JBI->Add("altitude", gps.altitude);
      JBI->Add("vsat", gps.vsat);
      JBI->Add("usat", gps.usat);
      JBI->Add("accuracy", gps.accuracy);
      JBI->Add("year", gps.year);
      JBI->Add("month", gps.month);
      JBI->Add("day", gps.day);
      JBI->Add("hour", gps.hour);
      JBI->Add("minute", gps.minute);
      JBI->Add("second", gps.second);
      JBI->Add("course", gps.course);
      JBI->Add("FixMode", gps.FixMode);
      JBI->Add("accuracy_position", gps.accuracy_position);
      JBI->Add("accuracy_vertical", gps.accuracy_vertical);
      JBI->Add("usat_glonass", gps.usat_glonass);
      JBI->Add("cno_max", gps.cno_max);
      JBI->Add("HPA", gps.HPA);
      JBI->Add("VPA", gps.VPA);

      char convf_lat[TBUFFER_SIZE_FLOAT];
      mSupport::float2CString(gps.latitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat);
      char convf_lon[TBUFFER_SIZE_FLOAT];
      mSupport::float2CString(gps.longitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon);
      char convf_fix[TBUFFER_SIZE_FLOAT];
      mSupport::float2CString(gps.accuracy,2,convf_fix);

      snprintf_P(buffer, sizeof(buffer),
        PSTR(
          "https://www.google.com/maps/dir//%s,%s"
        ),
        convf_lat, 
        convf_lon
      );

      JBI->Add("url", buffer);
    JBI->Level_End();
    #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    JBI->Level_Start("Battery");
      JBI->Add("Volts_mv", modem_status.battery.volts_mv);
      JBI->Add("Percentage", modem_status.battery.percentage);
      JBI->Add("ChargeState", modem_status.battery.charge_state);
      JBI->Add("Valid", modem_status.battery.isvalid);
    JBI->Level_End();
  
  return JBI->End();

}

  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mCellular::MQTTHandler_Init()
{

  struct handler<mCellular>* ptr;

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 120; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mCellular::ConstructJSON_Settings;

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mCellular::ConstructJSON_State;

} //end "MQTTHandler_Init"


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mCellular::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mCellular::MQTTHandler_Set_DefaultPeriodRate()
{
  // for(auto& handle:mqtthandler_list){
  //   if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
  //     handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  //   if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
  //     handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
  // }
}

/**
 * @brief MQTTHandler_Sender
 * */
void mCellular::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE__NETWORK_CELLULAR__ID, handle, id);
  }
}

#endif // USE_MODULE_NETWORK_MQTT


#endif // USE_MODULE_NETWORK_CELLULAR