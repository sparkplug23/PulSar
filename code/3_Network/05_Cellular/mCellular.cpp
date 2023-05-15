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

#ifdef ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
StreamDebugger debugger(SerialAT, Serial);
#endif // ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT

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
      Handler_ModemResponses(LOG_LEVEL_DEBUG_MORE);
      
      /**
       * @brief Here so it can be set from anywhere/method and then read later
       * 
       */
      if(sms.texts_saved_on_sim_indexs.size())
      {
        SMSReadAndEraseSavedSMS();
      } 

    }
    break;
    case FUNC_EVERY_SECOND: 

      if(!flags_modem_init_commands)
      {

        bool status = 0;

        Modem_Enable();

        #ifndef ENABLE_DEVFEATURE_STOP_MQTT_FROM_CONNECTING
        GPRS_Enable();
        #endif

        #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
        GPS_Enable();
        #endif 

        SMS_Enable();

        flags_modem_init_commands = true;

      }
      
      if(flags_modem_init_commands)
      {

        if(modem){
          modem->maintain();
        }

        #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
        ModemUpdate_GPS();
        #endif

        #ifndef ENABLE_DEVFEATURE_STOP_MQTT_FROM_CONNECTING
        ModemUpdate_GPRS();
        #endif
        
        ModemUpdate_SMS();

        
      }

    break;
    case FUNC_EVERY_FIVE_SECOND:   
  
  // SendATCommand_SMSImmediateForwardOverSerial();
      
      #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
      ALOG_INF(PSTR(D_LOG_CELLULAR "GPS u/v_sat %d/%d Fix (%d cm)"), gps.usat, gps.vsat, (int)(gps.accuracy*100)); 
      #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

      // modem->sendAT("AT+CMGL=\"ALL\"");

      
      // modem->sendAT("+CPMS?"); // How many SMS are waiting?

    break;
    case FUNC_EVERY_MINUTE:

        // SendATCommand_SMSImmediateForwardOverSerial();
      // ModemUpdate_BatteryStatus();
      // SMS_Send_TimedHeartbeat();

      #ifdef ENABLE_DEVFEATURE_TEXT_LOCATION_EVERY_MINUTE_WHEN_POWERED
      SMS_GPSLocation();
      #endif 

      // SendATCommand_SMSFormatAscii(); //tmp
      ALOG_INF(PSTR(D_LOG_CELLULAR "gprs.reconnect_init_counts %d"), gprs.reconnect_init_counts); 
      ALOG_INF(PSTR(D_LOG_CELLULAR "gprs.connected_seconds %d"), gprs.connected_seconds);    

      /**
       * @brief For debugging, lets check states
       * 
       */
      // modem->sendAT("+CMFG?"); // 1= ascii mode //expected 1
      // modem->sendAT("+CNMI?"); // <mode>,<mt>,<bm>,<ds>,<bfr> //expected 2,2,0,0,0 


      // modem->sendAT("+CPMS?"); // How many SMS are waiting?
      // modem->sendAT("+CNMI?"); // <mode>,<mt>,<bm>,<ds>,<bfr> //expected 2,2,0,0,0 


    // "AT+CMGD=,4",// DELETE ALL MESSAGES (Read or not)
    // "AT+CMGR=1",
    // "AT+CMGL=\"REC UNREAD\""
    // "AT+CPMS?"     // Number of stored SMS

/*


{
  "ATCommands": [
    "AT+CMGR=1",
    "AT+CNMI=3,2,0,0,0",
    "AT+CMGL=\"REC UNREAD\""
  ]
}

*/
    break;
    case FUNC_EVERY_HOUR:
#ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
      SMS_Send_TimedHeartbeat();
#endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
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

  #ifdef ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT  // if enabled it requires the streamDebugger lib
  modem = new TinyGsm(debugger);
  #else
  modem = new TinyGsm(SerialAT);
  #endif


  #ifdef USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED
    // SerialAT.begin(115200, SERIAL_8N1, PIN_RX, PIN_TX); // #define UART_BAUD   115200
    // modem->setBaud(UART_CELLULAR_BAUD); // Change from default
    SerialAT.begin(UART_CELLULAR_BAUD, SERIAL_8N1, PIN_RX, PIN_TX); // #define UART_BAUD   115200
  #else 
    SerialAT.begin(115200, SERIAL_8N1, 26,27);// PIN_RX, PIN_TX);
  #endif // USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED

  SerialAT.setTimeout(100); //ms

  /**
   * @brief Power on
   **/ 
  modemPowerOn();

  #ifdef ENABLE_DEVFEATURE_SIM7000G_INIT_SKIP_MODEM_RESTART
    ALOG_INF(PSTR(D_LOG_CELLULAR "Faster init modem"));
    if (!modem->init()) {
      ALOG_INF(PSTR(D_LOG_CELLULAR "Failed to init modem"));
    }
  #else
    // Restart takes quite some time, to skip it, call init() instead of restart()
    ALOG_INF(PSTR(D_LOG_CELLULAR "Slower restarting modem"));
    if (!modem->restart()) {
      ALOG_INF(PSTR(D_LOG_CELLULAR "Failed to restart modem"));
    }
  #endif

  gprs.reconnect_init_counts++;

}


bool mCellular::Handler_ModemResponses(uint8_t response_loglevel, uint16_t wait_millis)
{

  if (SerialAT.available()) 
  {

    uint32_t receive_millis = millis();

    uint16_t buflen = 0;
    char buffer[300] = {0};

    while(SerialAT.available())
    {
      buffer[buflen++] = SerialAT.read();
      if(buflen>(sizeof(buffer)-1))
      {
        ALOG_ERR(PSTR(D_LOG_CELLULAR "RX Overflow"));
        break;
      }
      if((millis()-receive_millis)>100)
      {
        ALOG_ERR(PSTR(D_LOG_CELLULAR "RX Timeout"));
        break;
      }
    }

    AddLog(response_loglevel, PSTR(D_LOG_CELLULAR "Handler_ModemResponses %d>> Buffer[%d] \"%s\""),response_loglevel, buflen, buffer);
    
    return parse_ATCommands(buffer, buflen, response_loglevel);

  }

  return false; // nothing handled or received

}


void mCellular::SMSReadAndEraseSavedSMS()
{

  #ifdef ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_CELLULAR "SMSReadAndEraseSavedSMS -- START"));
  #endif 

  for(auto& id:sms.texts_saved_on_sim_indexs)
  {
    if(SendAT_ATParseResponse_F(200, LOG_LEVEL_COMMANDS, PSTR("+CMGR=%d"), id))
    {
      // modem->waitResponse(200);
      // delay(200);
      
      // Erase from memory
      if(SendAT_F(200, PSTR("+CMGD=%d"), id))
      {
        ALOG_INF(PSTR("SMSReadAndEraseSavedSMS %d deleted"), id);
      }
    }
  }

  sms.texts_saved_on_sim_indexs.clear(); // for now assumed its done

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
  modem->sendAT("+CFUN=0"); 
  bool result = modem->waitResponse(AT_COMMAND_RESPONSE_TIMEOUT__CFUN);
  ALOG_INF(PSTR(D_LOG_CELLULAR "FunctionalityMode_Minimum %d"), result);
}
void mCellular::SendATCommand_FunctionalityMode_Full()
{
  modem->sendAT("+CFUN=1");
  bool result = modem->waitResponse(AT_COMMAND_RESPONSE_TIMEOUT__CFUN); 
  ALOG_INF(PSTR(D_LOG_CELLULAR "FunctionalityMode_Full %d"), result);
}



void mCellular::ModemUpdate_BatteryStatus()
{
  modem_status.battery.isvalid = modem->getBattStats(
    modem_status.battery.charge_state, 
    modem_status.battery.percentage, 
    modem_status.battery.volts_mv
  );

  ALOG_DBM(PSTR(D_LOG_CELLULAR "{\"Volts_mv\":%d,\"Percent\":%d,\"Charging\":%d}"),
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
  delay(1000);    // Datasheet T_on = 72ms
  digitalWrite(PWR_PIN, HIGH);
  flags_modem_init_commands = false;
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
  delay(5000);
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
      ALOG_INF(PSTR(D_LOG_CELLULAR " SGPIO=0,4,1,1 false "));
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
    ALOG_INF(PSTR(D_LOG_CELLULAR " SGPIO=0,4,1,0 false "));
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
        // ALOG_INF(PSTR(D_LOG_CELLULAR "GPS u/v_sat %d/%d Fix (%d cm)"), gps.usat, gps.vsat, (int)(gps.accuracy*100));
        
        #ifdef USE_MODULE_SENSORS_GPS_MODEM
        pCONT_gps->readings.update_seconds = millis();
        pCONT_gps->location.latitude  = gps.latitude;
        pCONT_gps->location.longitude = gps.longitude;
        pCONT_gps->location.speed = gps.speed;
        pCONT_gps->location.altitude = gps.altitude;
        pCONT_gps->location.accuracy = gps.accuracy;
        #endif // USE_MODULE_SENSORS_GPS_MODEM
      
      }else{
        ALOG_ERR(PSTR(D_LOG_CELLULAR "GPS NO FIX"));
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

    ALOG_HGL(PSTR(D_LOG_CELLULAR "\n\n\nRECONNECTING Waiting for network..."));
    if (!modem->waitForNetwork(DEFAULT_AT_COMMAND_RESPONSE_WAIT)) 
    {
      return;
    }

    if (modem->isNetworkConnected()) 
    {
      ALOG_INF(PSTR(D_LOG_CELLULAR "Network connected"));
    }


    ALOG_INF(PSTR(D_LOG_CELLULAR "Connecting to: %s"), CELLULAR_APN);
    if (!modem->gprsConnect(CELLULAR_APN, CELLULAR_GPRS_USERNAME, CELLULAR_GPRS_PASSWORD)) 
    {
      return;
    }

    ALOG_INF(PSTR(D_LOG_CELLULAR "GPRS Checking..."));
    if (modem->isGprsConnected()) 
    {
      #ifndef USE_GROUPFEATURE__MQTT_AS_WIFI_WHEN_CELLULAR_IS_ACTIVE
      ALOG_INF(PSTR(D_LOG_CELLULAR "GPRS Connected"));
      gsm_client = new TinyGsmClient(*modem);
        ALOG_INF(PSTR(D_LOG_CELLULAR "MQTT size %d"), pCONT_mqtt->brokers.size());
      if(pCONT_mqtt->brokers.size()<1)
      {
        // TMP fix
        pCONT_mqtt->brokers.push_back(new MQTTConnection());


        ALOG_INF(PSTR(D_LOG_CELLULAR "MQTT not working push_back(new MQTTConnection())"));
      }
      pCONT_mqtt->brokers[0]->SetPubSubClient(gsm_client);
      #endif // USE_GROUPFEATURE__MQTT_AS_WIFI_WHEN_CELLULAR_IS_ACTIVE
    } 
    else 
    {
      ALOG_INF(PSTR(D_LOG_CELLULAR "GPRS NOT Connected: REINIT THE LAZY WAY"));
      
      Init();
    }

    // String ccid = modem->getSimCCID();
    // ALOG_INF(PSTR(D_LOG_CELLULAR "CCID: %s"), ccid.c_str());

    // String imei = modem->getIMEI();
    // ALOG_INF(PSTR(D_LOG_CELLULAR "IMEI: %s"), imei.c_str());

    // String cop = modem->getOperator();
    // ALOG_INF(PSTR(D_LOG_CELLULAR "Operator: %s"), cop.c_str());

    // IPAddress local = modem->localIP();
    // ALOG_INF(PSTR(D_LOG_CELLULAR "Local IP: %s"), String(local).c_str());

    // int csq = modem->getSignalQuality();
    // ALOG_INF(PSTR(D_LOG_CELLULAR "Signal quality: %d"), csq);

  }

}


bool mCellular::Modem_CheckSerialConnection(bool flag_enable_retries)
{

    //test modem is online ?
    uint8_t timeout_counter = 0;
    uint32_t  timeout = millis();
    while (!modem->testAT()) 
    {
      Serial.print(".");
      if (millis() - timeout > 1000 ) 
      {
        ALOG_INF(PSTR(D_LOG_CELLULAR "> It looks like the modem is not responding, trying to restart %d"), timeout_counter);
        // modemRestart();
        timeout = millis();
        // if(timeout_counter++ > 5)
        // {
        //   ALOG_INF(PSTR(D_LOG_CELLULAR "> Modem_Enable FAILED"));
        //   return;
        // }
      }
    }





}
bool mCellular::Modem_RestartUntilSerialResponse(bool flag_enable_retries)
{

    //test modem is online ?
    uint8_t timeout_counter = 0;
    uint32_t  timeout = millis();
    while (!modem->testAT()) 
    {
      Serial.print(".");
      if (millis() - timeout > 20000 ) 
      {
        ALOG_INF(PSTR(D_LOG_CELLULAR "> It looks like the modem is not responding, trying to restart %d"), timeout_counter);
        modemRestart();
        timeout = millis();
        // if(timeout_counter++ > 5)
        // {
        //   ALOG_INF(PSTR(D_LOG_CELLULAR "> Modem_Enable FAILED"));
        //   return;
        // }
      }
    }





}



void mCellular::Modem_Enable()
{
  if(modem)
  {
    
    ALOG_INF(PSTR(D_LOG_CELLULAR "Modem_Enable Start"));
    Modem_RestartUntilSerialResponse();
    ALOG_INF(PSTR(D_LOG_CELLULAR "\nModem is online"));

    // timeout = millis();
    // ALOG_INF(PSTR(D_LOG_CELLULAR "> Get SIM card status"));
    // while (modem->getSimStatus() != SIM_READY) {
    //   Serial.print(".");
    //   if (millis() - timeout > 10000 ) {
    //       ALOG_INF(PSTR(D_LOG_CELLULAR "It seems that your SIM card has not been detected. Has it been inserted?"));
    //       return;
    //   }
    // }
    
    ALOG_INF(PSTR(D_LOG_CELLULAR "Modem_Enable End"));

  }
}


void mCellular::GPRS_Enable()
{
  if(modem)
  {
        
    //Set mobile operation band
    modem->sendAT("+CBAND=ALL_MODE");
    modem->waitResponse();

    // Args:
    // 1 CAT-M
    // 2 NB-IoT
    // 3 CAT-M and NB-IoT
    // Set network preferre to auto
    modem->setPreferredMode(3);

    // Args:
    // 2 Automatic
    // 13 GSM only
    // 38 LTE only
    // 51 GSM and LTE only
    // Set network mode to auto
    modem->setNetworkMode(2);

    uint32_t  timeout = millis();
    // Check network signal and registration information
    // ALOG_INF(PSTR(D_LOG_CELLULAR "> SIM7000/SIM7070 uses automatic mode to access the network. The access speed may be slow. Please wait patiently"));
    RegStatus status;
    timeout = millis();
    do {
      int16_t sq =  modem->getSignalQuality();

      status = modem->getRegistrationStatus();

      if (status == REG_DENIED) {
        ALOG_INF(PSTR(D_LOG_CELLULAR "The SIM card you use has been rejected by the network operator"));
        return;
      } else {
        ALOG_INF(PSTR(D_LOG_CELLULAR "Signal %d dBm"), (int)GetSignalQualityPower(sq));
      }

      if (millis() - timeout > 10000 ) { //!! THIS IS BLOCKING CODE, NEEDS RESOLVED TO RETRY AGAIN
        if (sq == 99) {
          ALOG_INF(PSTR(D_LOG_CELLULAR "It seems that there is no signal."));
          return;
        }
        timeout = millis();
      }

      delay(800);
    } while (status != REG_OK_HOME && status != REG_OK_ROAMING);


    String res;

    ALOG_INF(PSTR(D_LOG_CELLULAR "Obtain the APN issued by the network"));
    modem->sendAT("+CGNAPN");
    if (modem->waitResponse(3000, res) == 1) 
    {
      res = res.substring(res.indexOf(",") + 1);
      res.replace("\"", "");
      res.replace("\r", "");
      res.replace("\n", "");
      res.replace("OK", "");
      ALOG_INF(PSTR(D_LOG_CELLULAR "The APN issued by the network is: %s"), res.c_str());
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
      ALOG_INF(PSTR(D_LOG_CELLULAR "The current network IP address is: %s"), res.c_str());
    }

    GPRS_Connect();

    modem->sendAT("+CPSI?");
    if (modem->waitResponse("+CPSI: ") == 1) 
    {
      res = modem->stream.readStringUntil('\n');
      res.replace("\r", "");
      res.replace("\n", "");
      modem->waitResponse();
      ALOG_INF(PSTR(D_LOG_CELLULAR "The current network parameter is: %s"), res.c_str());
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

        gprs.signal_quality_raw = modem->getSignalQuality();

        if(gprs.signal_quality_raw == 99)
        {
          gprs.signal_quality_rssi_dbm = -150;
          pCONT_interface_network->data.cellular_state.isvalid = false;
          pCONT_interface_network->data.mqtt_state.isvalid = false;
        }
        else
        {
          gprs.signal_quality_rssi_dbm = mSupport::mapfloat(gprs.signal_quality_raw, 0, 31, -113, -51);
          pCONT_interface_network->data.cellular_state.isvalid = true;
        }

        ALOG_INF(PSTR(D_LOG_CELLULAR "Signal quality: %d dBm (%d)"), int(gprs.signal_quality_rssi_dbm), gprs.signal_quality_raw);

        gprs.connected_seconds += 5;
        gprs.last_comms_millis_updated = millis();

      } 
      else 
      {
        ALOG_INF(PSTR(D_LOG_CELLULAR "GPRS status: NOT connected"));

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


float mCellular::GetSignalQualityPower()
{
  return GetSignalQualityPower(modem->getSignalQuality());
}

/**
 * @brief Conversion without calling modem again
 * 
 * @param signal_quality_raw 
 * @return float 
 */
float mCellular::GetSignalQualityPower(int16_t signal_quality_raw)
{

  float signal_quality_rssi_dbm = 0;

  if(signal_quality_raw == 99)
  {
    signal_quality_rssi_dbm = -150;
  }
  else
  {
    signal_quality_rssi_dbm = mSupport::mapfloat(signal_quality_raw, 0, 31, -113, -51);
  }

  return signal_quality_rssi_dbm;

}




void mCellular::SMS_Enable()
{
    
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS_Enable++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"));

  if(modem)
  {
    

    // THIS SECTION SHOULD ONLY BE SET IF NOT CONFIGURED, AND NOT RESET!!
  //   SendATCommand_FunctionalityMode_Minimum();

  //   /*
  //     2 Automatic
  //     13 GSM only
  //     38 LTE only
  //     51 GSM and LTE only
  //   * * * */
  //  if(modem->setNetworkMode(2))
  //  {
  //   ALOG_INF(PSTR(D_LOG_CELLULAR "setNetworkMode Pass"));
  //  }

  //   /**
  //    * @brief 
  //     Set the preferred selection between CAT-M and NB-IOT
  //     1 CAT-M
  //     2 NB-Iot
  //     3 CAT-M and NB-IoT
  //   **/
  //  if(modem->setPreferredMode(3))
  //  {
  //   ALOG_INF(PSTR(D_LOG_CELLULAR "setPreferredMode Pass"));
  //  }

    SendATCommand_FunctionalityMode_Full();    





    SendATCommand_SMSFormatAscii();

    // SendATCommand_SMSImmediateForwardOverSerial();      

    sms.enabled = true;
  }
}


void mCellular::SMS_Disable()
{
  
  sms.enabled = false;
}


void mCellular::ModemUpdate_SMS()
{
  
  /**
   * @brief Check for waiting SMS messages
   **/
  SendAT_ATParseResponse_F(100, LOG_LEVEL_DEBUG_MORE, PSTR("+CMGD=?"));


  if(mTime::TimeReached(&sms.tReached_Update, 1000))
  {
    if(sms.enabled)
    {
      // Although SMS should be forwarded, lets also check if any are waiting and read them out







    }
  }

}

void mCellular::Get_Modem_Hardware()
{

  String name = modem->getModemName();
  ALOG_INF(PSTR(D_LOG_CELLULAR "Modem Name: %s"),name.c_str());

  String modemInfo = modem->getModemInfo();
  ALOG_INF(PSTR(D_LOG_CELLULAR "Modem Info: %s"),modemInfo.c_str());


}


#ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
void mCellular::SMS_GPSLocation()
{
  if(modem)
  {
    ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: Sending GPS Location"));
        
    // --------TESTING SENDING SMS--------

    uint16_t buflen = 0;
    char     buffer[STANDARD_SMS_CHAR_LENGTH] = {0};

    // https://www.google.com/maps/dir//54.505,-6.299
    // https://www.google.com/maps/dir//54.505044,-6.298891

    
    /**
     * @brief Mavlink
     **/

    #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
    /**
     * @brief MAVLink Data
     **/    
    float mavlink_lat = pCONT_mavlink->pkt.gps_raw_int.data.lat/10000000;
    float mavlink_lon = pCONT_mavlink->pkt.gps_raw_int.data.lon/10000000;

    char convf_mavlink_lat[TBUFFER_SIZE_FLOAT];
    mSupport::float2CString(mavlink_lat,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_mavlink_lat);
    char convf_mavlink_lon[TBUFFER_SIZE_FLOAT];
    mSupport::float2CString(mavlink_lon,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_mavlink_lon);

    buflen += sprintf_P(buffer+buflen, 
      PSTR(
        "b%d%% c%d\n"
        "WP i%d %dm\n"
        "%dms\n"
        "https://www.google.com/maps/dir//%s,%s\n"
      ), 
      pCONT_mavlink->pkt.battery_status.data.battery_remaining,
      pCONT_mavlink->pkt.battery_status.data.current_consumed,

      pCONT_mavlink->pkt.mission_current.data.seq,
      pCONT_mavlink->pkt.nav_controller_output.data.wp_dist,

      millis()-pCONT_mavlink->pkt.tSaved_Last_Response,
      convf_mavlink_lat, 
      convf_mavlink_lon
    );

    #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER

    /**
     * @brief GPS Modem
     **/

    char convf_lat[TBUFFER_SIZE_FLOAT];
    mSupport::float2CString(gps.latitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat);
    char convf_lon[TBUFFER_SIZE_FLOAT];
    mSupport::float2CString(gps.longitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon);
    char convf_fix[TBUFFER_SIZE_FLOAT];
    mSupport::float2CString(gps.accuracy,2,convf_fix);

    buflen += sprintf_P(buffer+buflen, 
      PSTR(
        "f%s m\n"
        "https://www.google.com/maps/dir//%s,%s"
      ), 
      convf_fix,
      convf_lat, 
      convf_lon
    );
    
    ALOG_INF(PSTR(D_LOG_CELLULAR "buffer[%d] \"%s\""), strlen(buffer), buffer);
    
    String res;
    res = modem->sendSMS(SMS_TARGET, String(buffer));
    ALOG_INF(PSTR(D_LOG_CELLULAR "SMS:"), res ? "OK" : "fail");

  }
}
// void mCellular::SMS_GPSLocation()
// {
//   if(modem)
//   {
//     ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: Sending GPS Location"));
        
//     // --------TESTING SENDING SMS--------

//     // https://www.google.com/maps/dir//54.505,-6.299
//     // https://www.google.com/maps/dir//54.505044,-6.298891

//     char convf_lat[TBUFFER_SIZE_FLOAT];
//     mSupport::float2CString(gps.latitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat);
//     char convf_lon[TBUFFER_SIZE_FLOAT];
//     mSupport::float2CString(gps.longitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon);
//     char convf_fix[TBUFFER_SIZE_FLOAT];
//     mSupport::float2CString(gps.accuracy,2,convf_fix);

//     uint16_t buflen = 0;
//     char     buffer[STANDARD_SMS_CHAR_LENGTH] = {0};

// // DEBUG_LINE_HERE;
//     buflen += sprintf_P(buffer+buflen, //sizeof(buffer),
//     // buflen += snprintf_P(buffer+buflen, sizeof(buffer),
//       PSTR(
//         "Batt %d%% %dmV\n"
//         "WP  %d (%dm)\n"
//         "PKT Age  %d\n"
//         "Accuracy %s m\n"
//         "https://www.google.com/maps/dir//%s,%s"
//       ), 
//       pCONT_mavlink->pkt.battery_status.data.battery_remaining,
//       pCONT_mavlink->pkt.battery_status.data.current_consumed,

//       pCONT_mavlink->pkt.mission_current.data.seq,
//       pCONT_mavlink->pkt.nav_controller_output.data.wp_dist,

//       millis()-pCONT_mavlink->pkt.tSaved_Last_Response,
//       convf_fix,
//       convf_lat, 
//       convf_lon
//     );
//     // buflen += sprintf_P(buffer+buflen, //sizeof(buffer),
//     //   PSTR(
//     //     "Accuracy %s m\n"
//     //     "https://www.google.com/maps/dir//%s,%s"
//     //   ), 
//     //   convf_fix,
//     //   convf_lat, 
//     //   convf_lon
//     // );
// //     ALOG_INF(PSTR(D_LOG_CELLULAR "bufferA%d \"%s\""), buflen, buffer);
// // DEBUG_LINE_HERE;
//     #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
//     /**
//      * @brief MAVLink Data
//      **/    
//     float mavlink_lat = pCONT_mavlink->pkt.gps_raw_int.data.lat/10000000;
//     float mavlink_lon = pCONT_mavlink->pkt.gps_raw_int.data.lon/10000000;


//     char convf_lat2[TBUFFER_SIZE_FLOAT];
//     mSupport::float2CString(mavlink_lat,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat2);
//     char convf_lon2[TBUFFER_SIZE_FLOAT];
//     mSupport::float2CString(mavlink_lon,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon2);

// //     ALOG_INF(PSTR(D_LOG_CELLULAR "convf_lat2 \"%s\""),convf_lat2);
// //     ALOG_INF(PSTR(D_LOG_CELLULAR "convf_lon2 \"%s\""), convf_lon2);
// // DEBUG_LINE_HERE;
//     buflen += sprintf_P(buffer+buflen, //sizeof(buffer),
//       PSTR(
//         "\n"
//         "MAV\n"
//         "https://www.google.com/maps/dir//%s,%s"
//       ), 
//       convf_lat2, 
//       convf_lon2
//     );
//     #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER
    
// //     ALOG_INF(PSTR(D_LOG_CELLULAR "bufferB%d \"%s\""), buflen, buffer);
// // DEBUG_LINE_HERE;
//     ALOG_INF(PSTR(D_LOG_CELLULAR "buffer \"%s\""), buffer);
    
// // DEBUG_LINE_HERE;
//     String res;
//     res = modem->sendSMS(SMS_TARGET, String(buffer));
//     ALOG_INF(PSTR(D_LOG_CELLULAR "SMS:"), res ? "OK" : "fail");

// // DEBUG_LINE_HERE;
//   }
// }

void mCellular::SMS_Send_TimedHeartbeat()
{
  if(modem)
  {
    ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: Sending GPS Location"));
        
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
        "Timed Heartbeat\n"
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

    // buflen += snprintf_P(buffer+buflen, sizeof(buffer),
    //   PSTR(
    //     "\n"
    //     "MAV\n"
    //     "https://www.google.com/maps/dir//%s,%s"
    //   ), 
    //   convf_lat2, 
    //   convf_lon2
    // );
    #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER
    
    ALOG_INF(PSTR(D_LOG_CELLULAR "buffer  %s"),buffer);
    
    String res;
    res = modem->sendSMS(SMS_TARGET, String(buffer));
    ALOG_INF(PSTR(D_LOG_CELLULAR "SMS:"), res ? "OK" : "fail");

  }
}

#endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

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


  if(jtok = obj["GPRS_Enable"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "GPRS_Enable"));
    if(jtok.getInt() == 1)
    {
      GPRS_Enable();
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
    flags_modem_init_commands = jtok.getInt();
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

    // flags_modem_init_commands = jtok.getInt();
  
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
        // if(sms.texts_saved_on_sim_indexs.size())
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

    // flags_modem_init_commands = jtok.getInt();
  
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

    
}


/**
 * @brief Mostly to debug, but also return if it worked
 * 
 * @param buffer 
 * @return true 
 * @return false 
 */
bool mCellular::SendAT(const char* buffer, uint16_t wait_millis)
{
  if(!modem){ return false; }

  uint32_t tSaved_millis = millis();

  modem->sendAT(buffer);
  bool result = true;
  if(wait_millis)
  {
    result = modem->waitResponse(wait_millis);
  }
  uint32_t tSaved_Elapsed = millis()-tSaved_millis;

  ALOG_INF(PSTR(D_LOG_CELLULAR ">>>%s %s (%dms)"), buffer, result?"Success":"FAILED", tSaved_Elapsed);

  return result;
  
}


bool mCellular::SendAT_F(uint16_t wait_millis, PGM_P formatP, ...)
{

  char command_buffer[200];

  va_list arg;
  va_start(arg, formatP);
  vsnprintf_P(command_buffer, sizeof(command_buffer), formatP, arg);
  va_end(arg);

  return SendAT(command_buffer, wait_millis);

}


bool mCellular::SendAT_ATParseResponse_F(uint16_t wait_millis, uint8_t response_loglevel, PGM_P formatP, ...)
{

  // AddLog(response_loglevel, PSTR("response_loglevel=%d"),response_loglevel);

  char command_buffer[200];

  va_list arg;
  va_start(arg, formatP);
  vsnprintf_P(command_buffer, sizeof(command_buffer), formatP, arg);
  va_end(arg);
  
  if(!modem){ return false; }

  uint32_t tSaved_millis = millis();

  modem->sendAT(command_buffer);

  /**
   * @brief With timeout, expect response to be parsed
   * 
   */
  uint32_t tSaved_Elapsed = millis()-tSaved_millis;
  while(!Handler_ModemResponses(response_loglevel))
  {
    if(
      (tSaved_Elapsed = (millis()-tSaved_millis)) > wait_millis
    ){
      ALOG_ERR(PSTR(D_LOG_CELLULAR "No response (%dms)"), tSaved_Elapsed);
      return false;
    }

  }

  // ALOG_ERR(PSTR(D_LOG_CELLULAR "========================================Response (%dms)"), millis()-tSaved_millis);
  
  return true;

}

char* mCellular::ATResponse_Parse_CMT(char* incoming, char *parsed_buf, uint16_t parsed_buflen)
{

  ALOG_INF(PSTR(D_LOG_CELLULAR "ATResponse_Parse_CMT"));

  char *result = strstr(
    &incoming[2],   // skip "\r\n" at start of response
    "\r\n"          // search for split between header/body of SMS
  );

  if(result)
  {
    ALOG_INF(PSTR(D_LOG_CELLULAR "ATResponse_Parse_CMT::result \"%s\""), &result[2]); //skip token
    if(parsed_buf != nullptr)
    {
      snprintf(parsed_buf, strlen(&result[2])-1, &result[2]); // Copy and remove the trailing "\r\n"
    }
  }

}

  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mCellular::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add("SMSEnabled", sms.enabled);
    JBI->Add("GPRSEnabled", gprs.enabled);
    
    #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    JBI->Add("GPSEnabled", gps.enabled);
    #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    
  return JBI->End();

}


uint8_t mCellular::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[100] = {0};

  JBI->Start();

    JBI->Add("uptime", pCONT_time->uptime_seconds_nonreset);
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
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mCellular::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mCellular::ConstructJSON_State;
  mqtthandler_list.push_back(ptr);

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