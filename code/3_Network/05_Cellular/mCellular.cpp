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

const char* mCellular::PM_MODULE__NETWORK_CELLULAR__CTR = D_MODULE__NETWORK_CELLULAR__CTR;
const char* mCellular::PM_MODULE__NETWORK_CELLULAR__FRIENDLY_CTR = D_MODULE__NETWORK_CELLULAR__FRIENDLY_CTR;

//even though celullar here can report GPS, GPS should be rolled into driver/sensor? probably sensor, since its location data

//https://infocenter.nordicsemi.com/index.jsp?topic=%2Fref_at_commands%2FREF%2Fat_commands%2Ftext_mode%2Fcnmi_read.html
  
const char apn[]  = "giffgaff.com";     //SET TO YOUR APN
const char gprsUser[] = "gg";
const char gprsPass[] = "p";

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
      Handler_ModemResponses();
    }
    break;
    case FUNC_EVERY_SECOND: 

      ModemUpdate_GPS();

      ModemUpdate_GPRS();

      ModemUpdate_BatteryStatus();
      
      ModemUpdate_SMS();
      
      if(sms.enabled)
      {
        // ALOG_INF(PSTR("Maintain"));
        modem->maintain();
      }

    break;
    case FUNC_EVERY_FIVE_SECOND:

      ALOG_INF(PSTR("Network connected? %d"), modem->isNetworkConnected());
    
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

}


void mCellular::Init(void)
{

  modem = new TinyGsm(SerialAT);

  /**
   * @brief Power on  (move into templates)
   * 
   */
  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, HIGH);
  delay(300);
  digitalWrite(PWR_PIN, LOW);

  /**
   * @brief Move into SDCard code later
   * 
   */
  SPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS)) {
    ALOG_INF(PSTR("SDCard MOUNT FAIL"));
  } else {
    uint32_t cardSize = SD.cardSize() / (1024 * 1024);
    String str = "SDCard Size: " + String(cardSize) + "MB";
    ALOG_INF(PSTR("%s"),str.c_str());
  }

  /**
   * @brief Move into UART config to "claim" for tinygsm
   * 
   */
  SerialAT.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);

  // Restart takes quite some time, to skip it, call init() instead of restart()
  ALOG_INF(PSTR("Restarting modem (may take 60 seconds)"));
  if (!modem->restart()) {
    ALOG_INF(PSTR("Failed to restart modem, attempting to continue without restarting"));
  }

  SendATCommand_SMSFormatAscii();
  SendATCommand_SMSImmediateForwardOverSerial();

  DEBUG_LINE_HERE;

  GPS_Enable();
  DEBUG_LINE_HERE;

  GPRS_Enable();
  DEBUG_LINE_HERE;

}



void mCellular::Handler_ModemResponses()
{

      // while (SerialAT.available()) {
      //  Serial.write(SerialAT.read());
      // }
      // while (Serial.available()) {
      //   SerialAT.write(Serial.read());
      // }

  // Response
  String incoming = String();
  if (SerialAT.available()) 
  {
    incoming = SerialAT.readString();
    ALOG_HGL(PSTR("Incoming \n\r===(%s)==="), incoming.c_str());

    char buffer[300] = {0};
    char buffer2[300] = {0};

    sprintf(buffer, "%s", incoming.c_str());


    // ALOG_INF(PSTR("buffer = %s"), buffer);
    // for(int i = 0;i < incoming.length(); i++)
    // {
    //   Serial.printf("%02d>    %c       \n\r",i,buffer[i]);
    //   if(buffer[i]=='\n')
    //   {
    //     Serial.printf("=====================%02d>\"%c\"\n\r",i,buffer[i]);
    //   }
    // }

    
    char *search = "\r\n+CMT";
    char *result = strstr(buffer, search);
    if(result)
    {
      ALOG_INF(PSTR("FOUND CMT MESSAGE result >>>%s<<<"), result);
      ATResponse_Parse_CMT(buffer, buffer2, sizeof(buffer2));
    }


    ALOG_INF(PSTR("buffer2 >>>%s<<<"), buffer2);
    
    // Check for request at GPS
    if (strncasecmp(buffer2, "GPS", 3) == 0)
    {
      ALOG_INF(PSTR("Request for GPS"));
      SMS_GPSLocation();
    }
    else{
      ALOG_INF(PSTR("NO Request for GPS"));

    }


    


  }


}


char* mCellular::ATResponse_Parse_CMT(char* incoming, char *parsed_buf, uint16_t parsed_buflen)
{

  ALOG_DBM(PSTR("ATResponse_Parse_CMT"));

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
    ALOG_INF(PSTR(" +CNMI= 2,2,0,0,0  false "));
  }
}


void mCellular::SendATCommand_SMSImmediateForwardOverSerial()
{
  // Enable SMS always send to serial when they arrive, move to function later
  modem->sendAT("+CNMI= 2,2,0,0,0 ");
  if (modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT) != 1) {
    ALOG_INF(PSTR(" +CNMI= 2,2,0,0,0  false "));
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
      
      if(modem->getGPS(
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
        &gps.second
      )) 
      {
        ALOG_INF(PSTR("GPS Fix (%d cm)"), (int)(gps.accuracy*100));
      }else{
        ALOG_ERR(PSTR("GPS NO FIX"));

        GPS_Enable(); //need a backoff timer but otherwise should force it
      }

            
      #ifdef USE_MODULE_SENSORS_GPS_MODEM
      pCONT_gps->location.latitude  = gps.latitude;
      pCONT_gps->location.longitude = gps.longitude;
      pCONT_gps->location.speed = gps.speed;
      pCONT_gps->location.altitude = gps.altitude;





      #endif // USE_MODULE_SENSORS_GPS_MODEM
        
    }
  }

}


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


    ALOG_INF(PSTR("\n---Starting GPRS TEST---\n"));
    ALOG_INF(PSTR("Connecting to: %s"), apn);
    if (!modem->gprsConnect(apn, gprsUser, gprsPass)) 
    {
      return;
    }

    ALOG_INF(PSTR("GPRS status: "));
    if (modem->isGprsConnected()) 
    {
      ALOG_INF(PSTR("connected"));

      // pCONT_mqtt->SetPubSubClient();//new TinyGsmClient(modem));

DEBUG_LINE_HERE;
      gsm_client = new TinyGsmClient(*modem);
DEBUG_LINE_HERE;
      pCONT_mqtt->SetPubSubClient_Cellular(gsm_client);
DEBUG_LINE_HERE;

    } 
    else 
    {
      ALOG_INF(PSTR("not connected"));
    }

    String ccid = modem->getSimCCID();
    ALOG_INF(PSTR("CCID: %s"), ccid.c_str());

    String imei = modem->getIMEI();
    ALOG_INF(PSTR("IMEI: %s"), imei.c_str());

    String cop = modem->getOperator();
    ALOG_INF(PSTR("Operator: %s"), cop.c_str());

    IPAddress local = modem->localIP();
    ALOG_INF(PSTR("Local IP: %s"), String(local).c_str());

    int csq = modem->getSignalQuality();
    ALOG_INF(PSTR("Signal quality: %d"), csq);

  }

}


void mCellular::GPRS_Enable()
{
  if(modem)
  {
    
    ALOG_INF(PSTR("> Check whether Modem is online"));
    DEBUG_LINE_HERE;
    //test modem is online ?
    uint32_t  timeout = millis();
    while (!modem->testAT()) 
    {
      Serial.print(".");
      if (millis() - timeout > 60000 ) 
      {
        ALOG_INF(PSTR("> It looks like the modem is not responding, trying to restart"));
        modemPowerOff();
        delay(5000);
        modemPowerOn();
        timeout = millis();
      }
    }
    ALOG_INF(PSTR("\nModem is online"));

  DEBUG_LINE_HERE;

    //test sim card is online ?
    timeout = millis();
    Serial.print("> Get SIM card status");
    while (modem->getSimStatus() != SIM_READY) {
        Serial.print(".");
        if (millis() - timeout > 60000 ) {
            ALOG_INF(PSTR("It seems that your SIM card has not been detected. Has it been inserted?"));
            ALOG_INF(PSTR("If you have inserted the SIM card, please remove the power supply again and try again!"));
            return;
        }

    }
    

    DEBUG_LINE_HERE;
    String res = modem->getIMEI();
    Serial.print("IMEI:");
    ALOG_INF(PSTR("%s"),res.c_str());

    DEBUG_LINE_HERE;
    /*
    * Tips:
    * When you are not sure which method of network access is supported by the network you use,
    * please use the automatic mode. If you are sure, please change the parameters to speed up the network access
    * * * * */

    //Set mobile operation band
    modem->sendAT("+CBAND=ALL_MODE");
    modem->waitResponse();

    DEBUG_LINE_HERE;
    // Args:
    // 1 CAT-M
    // 2 NB-IoT
    // 3 CAT-M and NB-IoT
    // Set network preferre to auto
    uint8_t perferred = 3;
    modem->setPreferredMode(perferred);

    if (perferred == 2) {
      ALOG_INF(PSTR("When you select 2, please ensure that your SIM card operator supports NB-IOT"));
    }
    DEBUG_LINE_HERE;

    // Args:
    // 2 Automatic
    // 13 GSM only
    // 38 LTE only
    // 51 GSM and LTE only
    // Set network mode to auto
    modem->setNetworkMode(2);

  DEBUG_LINE_HERE;


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
        ALOG_INF(PSTR("Signal:"));
        ALOG_INF(PSTR("sq=%d"),sq);
      }

      if (millis() - timeout > 360000 ) {
        if (sq == 99) {
          ALOG_INF(PSTR("> It seems that there is no signal. Please check whether the"\
                          "LTE antenna is connected. Please make sure that the location has 2G/NB-IOT signal\n"\
                          "SIM7000G does not support 4G network. Please ensure that the USIM card you use supports 2G/NB access"));
          return;
        }
        timeout = millis();
      }

      delay(800);
    } while (status != REG_OK_HOME && status != REG_OK_ROAMING);

  DEBUG_LINE_HERE;

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

  DEBUG_LINE_HERE;

    modem->sendAT("+CNACT=1");
    modem->waitResponse();

  DEBUG_LINE_HERE;

    // res = modem->getLocalIP();
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

  DEBUG_LINE_HERE;



    ALOG_INF(PSTR("\n\n\nWaiting for network..."));
    if (!modem->waitForNetwork(DEFAULT_AT_COMMAND_RESPONSE_WAIT)) 
    {
      return;
    }

    if (modem->isNetworkConnected()) 
    {
      ALOG_INF(PSTR("Network connected"));
    }

  DEBUG_LINE_HERE;


    ALOG_INF(PSTR("\n---Starting GPRS TEST---\n"));
    ALOG_INF(PSTR("Connecting to: %s"), apn);
    if (!modem->gprsConnect(apn, gprsUser, gprsPass)) 
    {
      return;
    }

    ALOG_INF(PSTR("GPRS status: "));
    if (modem->isGprsConnected()) 
    {
      ALOG_INF(PSTR("connected"));

DEBUG_LINE_HERE;
      gsm_client = new TinyGsmClient(*modem);
DEBUG_LINE_HERE;
      pCONT_mqtt->SetPubSubClient_Cellular(gsm_client);
DEBUG_LINE_HERE;
  DEBUG_LINE_HERE;

      // pCONT_mqtt->SetPubSubClient();//new TinyGsmClient(modem));
  DEBUG_LINE_HERE;

    } 
    else 
    {
      ALOG_INF(PSTR("not connected"));
    }

    String ccid = modem->getSimCCID();
    ALOG_INF(PSTR("CCID: %s"), ccid.c_str());

    String imei = modem->getIMEI();
    ALOG_INF(PSTR("IMEI: %s"), imei.c_str());

    String cop = modem->getOperator();
    ALOG_INF(PSTR("Operator: %s"), cop.c_str());

    IPAddress local = modem->localIP();
    ALOG_INF(PSTR("Local IP: %s"), String(local).c_str());

    int csq = modem->getSignalQuality();
    ALOG_INF(PSTR("Signal quality: %d"), csq);

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
  if(mTime::TimeReached(&gprs.tReached_Update, 1000))
  {
    if(gprs.enabled)
    {
      
        /**
         * @brief Check connection is still active
         * 
         */
                
        if (modem->isGprsConnected()) 
        {
            // ALOG_INF(PSTR("GPRS status: connected"));
            
            // // String cop = modem->getOperator();
            // // ALOG_INF(PSTR("Operator: %s"), cop.c_str());

            // String localIP = modem->getLocalIP();
            // ALOG_INF(PSTR("Local IP: %s"), localIP.c_str());

            int csq = modem->getSignalQuality();
            ALOG_INF(PSTR("Signal quality: %d"), csq);

            gprs.connected_seconds++;


        } 
        else 
        {
            ALOG_INF(PSTR("GPRS status: NOT connected"));
            
            gprs.connected_seconds = 0;
            GPRS_Connect();
        }



    }
  }

}


//https://cplusplus.com/reference/cstdio/sscanf/

// Set phone number, if you want to test SMS
// Set a recipient phone number to test sending SMS (it must be in international format including the "+" sign)
#define SMS_TARGET  "+447515358597"

void mCellular::SMS_Enable()
{
  if(modem)
  {
      
    const char apn[]  = "giffgaff.com";     //SET TO YOUR APN
    const char gprsUser[] = "gg";
    const char gprsPass[] = "p";

    String name = modem->getModemName();
    ALOG_INF(PSTR("Modem Name: %s"),name.c_str());

    String modemInfo = modem->getModemInfo();
    ALOG_INF(PSTR("Modem Info: %s"),modemInfo.c_str());

    SendATCommand_FunctionalityMode_Minimum();

    /**
     * @brief 
    AT+CMGR is the command to read the SMS.
    You have to wait till you receive CMT/CMTI indication to arrive and then you can read the SMS. So to get the CMT/CMTI indications, you have to set CNMI command.
    d click on Send button to select SMS message format. 1 Text mode, 0 PDU mode
     added, not tested
     */
    modem->sendAT("+CMGF=1 ");
    if (modem->waitResponse(10000L) != 1) {
        ALOG_INF(PSTR(" +CMGF=1  false "));
    }


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
    modem->sendAT("+CNMI= 2,2,0,0,0 "); 
    if (modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT) != 1) 
    {
      ALOG_INF(PSTR(" +CNMI= 2,2,0,0,0  false "));
    }


      

  // =============================================================================

    /**
     * @brief The +CGDCONT command defines Packet Data Protocol (PDP) Context.

      Response syntax:
      +CGDCONT: <cid>,<PDP_type>,<APN>,<PDP_addr>,<d_comp>,<h_comp>
      The read command parameters and their defined values are the following:

      <cid>
        Integer, 0–10.
      <PDP_type>
        String
        IP – Internet Protocol
        IPV6 – Internet Protocol version 6
        IPV4V6 – Virtual type of dual IP stack
        Non-IP – Transfer of non-IP data to external packet data network (see 3GPP TS 23.401 [82])
      <APN>
        String. Access Point Name (APN).
      <PDP_addr>
        String. IP address.
      <d_comp>
        0 – Compression not supported
      <h_comp>
        0 – Compression not supported
        
      The following command example reads configured default bearers:

      AT+CGDCONT?
        +CGDCONT: 0,"IP","internet","10.0.1.1",0,0
        +CGDCONT: 1,"IP","IOT_apn","10.0.1.2",0,0
        OK
                                                      
        +CGDCONT: 1,"IP","giffgaff.com","0.0.0.0",0,0,0,0
        +CGDCONT: 13,"IP","giffgaff.com","0.0.0.0",0,0,0,0                                                                                
        OK
    **/

    // Command
    SerialAT.println("AT+CGDCONT?");
    delay(500);
    // Response
    if (SerialAT.available()) 
    {

      input = SerialAT.readString();
      ALOG_INF(PSTR("Response ==(%s)=="), input.c_str());

      /**
       * @brief Splitting multiline messages into single messages
       * 
       */
      for (int i = 0; i < input.length(); i++) 
      {        
        if (input.substring(i, i + 1) == "\n") 
        {
          ALOG_INF(PSTR("counter = %d"), counter);
          pieces[counter] = input.substring(lastIndex, i);
          lastIndex = i + 1;
          counter++;
        }
        if (i == input.length() - 1) 
        {
          pieces[counter] = input.substring(lastIndex, i);
        }
      }

      // Reset for reuse
      input = "";
      counter = 0;
      lastIndex = 0;

      for ( int y = 0; y < numberOfPieces; y++) 
      {
        for ( int x = 0; x < pieces[y].length(); x++) 
        {
          char c = pieces[y][x];  //gets one byte from buffer
          if (c == ',') 
          {
            if (input.indexOf(": ") >= 0) 
            {
              String data = input.substring((input.indexOf(": ") + 1));
              if ( data.toInt() > 0 && data.toInt() < 25) 
              {
                modem->sendAT("+CGDCONT=" + String(data.toInt()) + ",\"IP\",\"" + String(apn) + "\",\"0.0.0.0\",0,0,0,0");
              }
              input = "";
              break;
            }
            // Reset for reuse
            input = "";
          } 
          else 
          {
            input += c;
          }
        }
      }
    } 
    else 
    {
      ALOG_INF(PSTR("Failed to get PDP!"));
    }


    ALOG_INF(PSTR("\n\n\nWaiting for network..."));
    if (!modem->waitForNetwork(DEFAULT_AT_COMMAND_RESPONSE_WAIT)) 
    {
      return;
    }

    if (modem->isNetworkConnected()) 
    {
      ALOG_INF(PSTR("Network connected"));
    }

    ALOG_INF(PSTR("\n---Starting GPRS TEST---\n"));
    ALOG_INF(PSTR("Connecting to: %s"), apn);
    if (!modem->gprsConnect(apn, gprsUser, gprsPass)) 
    {
      return;
    }

    ALOG_INF(PSTR("GPRS status: "));
    if (modem->isGprsConnected()) 
    {
      ALOG_INF(PSTR("connected"));
    } 
    else 
    {
      ALOG_INF(PSTR("not connected"));
    }

    String ccid = modem->getSimCCID();
    ALOG_INF(PSTR("CCID: %s"), ccid.c_str());

    String imei = modem->getIMEI();
    ALOG_INF(PSTR("IMEI: %s"), imei.c_str());

    String cop = modem->getOperator();
    ALOG_INF(PSTR("Operator: %s"), cop.c_str());

    IPAddress local = modem->localIP();
    ALOG_INF(PSTR("Local IP: %s"), String(local).c_str());

    int csq = modem->getSignalQuality();
    ALOG_INF(PSTR("Signal quality: %d"), csq);

    SerialAT.println("AT+CPSI?");     //Get connection type and band
    delay(500);
    if (SerialAT.available()) {
      String r = SerialAT.readString();
      ALOG_INF(PSTR("r=%s"),r.c_str());
    }

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

    char message2[STANDARD_SMS_CHAR_LENGTH];

    snprintf_P(message2, sizeof(message2),
      PSTR(
        "Battery  %d mV\n"
        "Accuracy %s m\n"
        "\n"
        "https://www.google.com/maps/dir//%s,%s"
      ), 
      modem_status.battery.volts_mv,
      convf_fix,
      convf_lat, 
      convf_lon
    );
    
    ALOG_INF(PSTR("message2  %s"),message2);
    
    String res;
    res = modem->sendSMS(SMS_TARGET, String(message2));
    ALOG_INF(PSTR("SMS:"), res ? "OK" : "fail");

  }
}


/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

void mCellular::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

//https://github.com/vshymanskyy/TinyGSM/pull/260/files#diff-49f12f4a048fa1f63d160e1adb91526d97e2a16cd3ba3898525ac5d1a44ddb99
  if(jtok = obj["GPS_Enable"]){
    ALOG_INF( PSTR("GPS_Enable"));
    if(jtok.getInt() == 1)
    {
      GPS_Enable();
    }
  }
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
  if(jtok = obj["SMS_GPSLocation"]){
    ALOG_INF( PSTR("SMS_GPSLocation"));
    if(jtok.getInt() == 1)
    {
      SMS_GPSLocation();
    }
  }
  
  

  if(jtok = obj["BattVolt"]){
    ALOG_INF( PSTR("BattVolt"));
    if(jtok.getInt() == 1)
    {
      // SMS_GPSLocation();

      
    
        // read the battery voltage and percentage
        uint16_t vbat = modem->getBattVoltage();
        // if (! modem->getBattVoltage(&vbat)) {
        //   ALOG_INF(PSTR(F("Failed to read Batt"));
        // } else {
          
        ALOG_INF(PSTR("%d mV"), vbat);
        // }

    }


  }


  if(jtok = obj["ADCVoltage"]){
    ALOG_INF( PSTR("ADCVoltage"));
    if(jtok.getInt() == 1)
    {
      
      ModemUpdate_BatteryStatus();

      modem_status.battery.isvalid = modem->getBattStats(
        modem_status.battery.charge_state, 
        modem_status.battery.percentage, 
        modem_status.battery.volts_mv
      );

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


  if(jtok = obj["TestCode_RunAll"]){
    ALOG_INF( PSTR("TestCode_RunAll"));
    TestCode_RunAll();
  }

  if(jtok = obj["SIM"].getObject()["Connect1"])
  {
    ALOG_INF( PSTR("Connect1"));
    ArduinoExample_GPRSConnect1();
  }
  
  if(jtok = obj["SIM"].getObject()["GPRSConnect"])
  {
    ALOG_INF( PSTR("GPRSConnect"));
    ArduinoExample_GPRSConnect();
  }

  if(jtok = obj["NetworkTest"])
  {
    ArduinoNetworkTest();
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

// https://www.google.co.uk/maps/place/@52.410555,-0.5150276,12/ 17

// Or you can use OpenStreetMap directly in a Frame widget with a pin like this —

// https://www.openstreetmap.org/?mlat=54.02227&mlon=-0.66649#map=19/54.02227/-0.66649


uint8_t mCellular::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[100] = {0};

  JBI->Start();

    JBI->Level_Start("GPRS");
        JBI->Add("ConSec", gprs.connected_seconds);
    JBI->Level_End();
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
//   for(auto& handle:mqtthandler_list){
//     if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
//       handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
//     if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
//       handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
//   }
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