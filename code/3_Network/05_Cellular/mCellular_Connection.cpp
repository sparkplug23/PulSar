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

/**
 * @brief Check for active comms on the serial line
 * 
 */
bool mCellular::Modem__PowerUntilRunning(uint16_t wait_millis)
{

  Serial.println("> Check whether Modem is online");
    //test modem is online ?
    uint32_t  timeout = millis();
    while (!Modem__Running(100)) {
        Serial.print(".");
        if (millis() - timeout > wait_millis ) {
            Serial.println("> It looks like the modem is not responding, trying to restart");
            modemPowerOff();
            delay(5000);
            modemPowerOn();
            timeout = millis();
        }
    }
    Serial.println("\nModem is online");

}


/**
 * @brief Check for active comms on the serial line
 * 
 */
bool mCellular::Modem__Running(uint16_t wait_millis)
{

  ALOG_INF(PSTR("mCellular::Modem__Running"));
  bool modem_responded = false;
  modem_responded = modem->testAT(wait_millis);
  ALOG_INF(PSTR("modem_responded = %d"), modem_responded);
  return modem_responded;

}


/**
 * @brief Commands required after restart
 * 
 */
bool mCellular::SimNetwork__InitConfig()
{


}

/**
 * @brief Commands required to connect
 * 
 */
bool mCellular::SimNetwork__StartConnection()
{


}

/**
 * @brief Check status of connection
 * 
 */
bool mCellular::SimNetwork__CheckConnection()
{


}


/**
 * @brief Commands required after restart
 * 
 */
bool mCellular::DataNetwork__InitConfig()
{
  
  ALOG_HGL(PSTR(D_LOG_CELLULAR "DataNetwork__InitConfig()"));


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
        return false;
      } else {
        ALOG_INF(PSTR(D_LOG_CELLULAR "Signal %d dBm, %d, pause %d"), (int)GetSignalQualityPower(sq), status, 10000-(timeout-millis()) );
      }

      if (millis() - timeout > 10000 ) { //!! THIS IS BLOCKING CODE, NEEDS RESOLVED TO RETRY AGAIN
        if (sq == 99) {
          ALOG_INF(PSTR(D_LOG_CELLULAR "It seems that there is no signal."));
          return false;
        }
        timeout = millis();
      }

      delay(50);
      // DEBUG_LINE_HERE;
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
    // modem->waitResponse();
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

    
    gprs.enabled = true;

}

/**
 * @brief Commands required to connect
 * 
 */
bool mCellular::DataNetwork__StartConnection()
{

  ALOG_HGL(PSTR(D_LOG_CELLULAR "DataNetwork__StartConnection()"));

  if(modem)
  {

    ALOG_INF(PSTR(D_LOG_CELLULAR "RECONNECTING GPRS_Connect Waiting for CELL TOWER network..."));
    if (!modem->waitForNetwork())  // Sim to cell tower connection, not the gprs
    {
      ALOG_INF(PSTR(D_LOG_CELLULAR "RECONNECTING GPRS_Connect Timeout?? I need to connect here??Waiting for network..."));
      // if this was false, I should really restart the cell connection
      // SimNetwork_Connect(); // might take long, so I need a "re-enable"
      return false;
    }

    if (modem->isNetworkConnected()) 
    {
      ALOG_INF(PSTR(D_LOG_CELLULAR "Network connected"));
    }else{
      
    }

    
    modem->gprsDisconnect();
    delay(50);


    ALOG_INF(PSTR(D_LOG_CELLULAR "Connecting to: %s"), CELLULAR_APN);
    bool gprs_connection_successful = modem->gprsConnect(CELLULAR_APN, CELLULAR_GPRS_USERNAME, CELLULAR_GPRS_PASSWORD);

    if(gprs_connection_successful)
    {
      ALOG_INF(PSTR("modem->gprsConnect                 YEEEEEEEEEEEEEEEEEEEES"));
      gprs.apn_connect_called++;
    }else{
      ALOG_INF(PSTR("modem->gprsConnect                 NOOOOOOOOOOOOOOOOOOOOT"));

    }


    if (!gprs_connection_successful) 
    {
      ALOG_INF(PSTR(D_LOG_CELLULAR "GPRS !gprs_connection_successful..."));
      return false;
    }

    ALOG_INF(PSTR(D_LOG_CELLULAR "GPRS Checking..."));
    if (modem->isGprsConnected()) 
    {
      DEBUG_LINE_HERE;
      #ifdef ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
      ALOG_INF(PSTR(D_LOG_CELLULAR "GPRS Connected"));
      ALOG_HGL(PSTR("Start MQTTConnection with Cellular"));

      /**
       * @brief The idea is on a connection, the new connection should be added to the network interface
       * For now, set the mqtt directly
       **/
      if(gsm_client != nullptr)
      {
        gsm_client->stop();
        // gsm_client->setTimeout
        delay(50); //flush time
      }
      if(gsm_client == nullptr)
      {
        gsm_client = new TinyGsmClient(*modem); // Only create new if it didnt already exist
      }
      pCONT_mqtt->CreateConnection(gsm_client, MQTT_HOST_CELLULAR, MQTT_PORT_CELLULAR);
        


      #endif // ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
      
      DEBUG_LINE_HERE;
      GPRS_UpdateConnectionState(true);
      DEBUG_LINE_HERE;


    } 
    else 
    {
      ALOG_HGL(PSTR(D_LOG_CELLULAR "GPRS NOT Connected: REINIT THE LAZY WAY"));
      
      // Init();
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

    String res;
    modem->sendAT("+CPSI?");
    // modem->waitResponse();
    if (modem->waitResponse("+CPSI: ") == 1) 
    {
      res = modem->stream.readStringUntil('\n');
      res.replace("\r", "");
      res.replace("\n", "");
      modem->waitResponse();
      ALOG_INF(PSTR(D_LOG_CELLULAR "The current network parameter is: %s"), res.c_str());
    }

}


/**
 * @brief Check status of connection
 * 
 */
bool mCellular::DataNetwork__CheckConnection()
{


}













#endif // USE_MODULE_NETWORK_CELLULAR