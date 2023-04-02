#include "mCellular.h"

#ifdef USE_MODULE_NETWORK_CELLULAR



    #define SerialMon Serial


void mCellular::EveryLoop()
{

//   if(pCONT_time->uptime_seconds_nonreset<120){ return; }


  
//     const char apn[]  = "giffgaff.com";     //SET TO YOUR APN
//     const char gprsUser[] = "gg";
//     const char gprsPass[] = "p";

//     // Restart takes quite some time
//     // To skip it, call init() instead of restart()
//     Serial.println("Initializing modem->..");
//     if (!modem->init()) {
//         Serial.println("Failed to restart modem, attempting to continue without restarting");
//     }

//     String name = modem->getModemName();
//     delay(500);
//     Serial.println("Modem Name: " + name);

//     String modemInfo = modem->getModemInfo();
//     delay(500);
//     Serial.println("Modem Info: " + modemInfo);

//   // WDT_Reset(); yield();

//     // Set SIM7000G GPIO4 LOW ,turn off GPS power
//     // CMD:AT+SGPIO=0,4,1,0
//     // Only in version 20200415 is there a function to control GPS power
//     modem->sendAT("+SGPIO=0,4,1,0");
//     if (modem->waitResponse(10000L) != 1) {
//         DBG(" SGPIO=0,4,1,0 false ");
//     }

// #if TINY_GSM_TEST_GPRS
//     // Unlock your SIM card with a PIN if needed
//     if ( GSM_PIN && modem->getSimStatus() != 3 ) {
//         modem->simUnlock(GSM_PIN);
//     }
// #endif

//     modem->sendAT("+CFUN=0 ");
//     if (modem->waitResponse(10000L) != 1) {
//         DBG(" +CFUN=0  false ");
//     }
    
//   /*
//     2 Automatic
//     13 GSM only
//     38 LTE only
//     51 GSM and LTE only
//   * * * */
//   String res;
//   res = modem->setNetworkMode(2);
//   if (res != "1") {
//       DBG("setNetworkMode  false ");
//       return ;
//   }

//   /**
//    * @brief 
//     Set the preferred selection between CAT-M and NB-IOT
//     1 CAT-M
//     2 NB-Iot
//     3 CAT-M and NB-IoT
//   **/
//   res = modem->setPreferredMode(3);
//   if (res != "1")
//   {
//     ALOG_DBG(PSTR("setPreferredMode false"));
//   }

//   /**
//    * @brief 
//    * Response syntax:
//     +CFUN: <fun>
//     The read response parameter and its defined value are the following:
//     <fun>
//       0 – Power off and store. RF circuits are disabled by deactivating LTE and Global Navigation Satellite System (GNSS) services.
//       1 – Normal mode. The active mode is either LTE or GNSS, or both. Full functional mode. Active modes depend on %XSYSTEMMODE setting.
//       2 – Receive only mode in active system mode. Active modes depend on %XSYSTEMMODE setting.
//       4 – Flight mode. RF circuits are disabled by deactivating LTE and GNSS services. 
//     **/
//   modem->sendAT("+CFUN=1 ");
//   if (modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT) != 1) 
//   {
//     ALOG_DBG(PSTR(" +CFUN=1  false "));
//   }

//   /**
//    * @brief Make it so new SMS messages are send over serial automatically
//    * +CNMI: <mode>,<mt>,<bm>,<ds>,<bfr>
//    * The read command parameters and their defined values are the following:
//     <mode>
//       0 – Do not forward unsolicited result codes to the Terminal Equipment (TE) (default)
//       3 – Forward unsolicited result codes directly to the TE
//     <mt>
//       0 – No received message notifications, the modem acts as an SMS client
//       2 – SMS-DELIVERs (except class 2 and message waiting indication group) are routed directly to the TE
//     <bm>
//       No CBM notifications are routed to the TE
//     <ds>
//       0 – No SMS-STATUS-REPORTs are routed to the TE
//       1 – SMS-STATUS-REPORTs are routed to the TE using unsolicited result code: +CDS: <length><CR><LF><pdu>
//     <bfr>
//       1 – The buffer of unsolicited result codes is cleared when <mode> 1 to 3 is entered
//   **/
//   modem->sendAT("+CNMI= 2,2,0,0,0 "); 
//   if (modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT) != 1) 
//   {
//     ALOG_DBG(PSTR(" +CNMI= 2,2,0,0,0  false "));
//   }




// // =============================================================================

// #if TINY_GSM_TEST_GPRS

//     SerialAT.println("AT+CGDCONT?");
//     delay(500);
//     if (SerialAT.available()) {
//         input = SerialAT.readString();
//         for (int i = 0; i < input.length(); i++) {
//             if (input.substring(i, i + 1) == "\n") {
//                 pieces[counter] = input.substring(lastIndex, i);
//                 lastIndex = i + 1;
//                 counter++;
//             }
//             if (i == input.length() - 1) {
//                 pieces[counter] = input.substring(lastIndex, i);
//             }
//         }
//         // Reset for reuse
//         input = "";
//         counter = 0;
//         lastIndex = 0;

//         for ( int y = 0; y < numberOfPieces; y++) {
//             for ( int x = 0; x < pieces[y].length(); x++) {
//                 char c = pieces[y][x];  //gets one byte from buffer
//                 if (c == ',') {
//                     if (input.indexOf(": ") >= 0) {
//                         String data = input.substring((input.indexOf(": ") + 1));
//                         if ( data.toInt() > 0 && data.toInt() < 25) {
//                             modem->sendAT("+CGDCONT=" + String(data.toInt()) + ",\"IP\",\"" + String(apn) + "\",\"0.0.0.0\",0,0,0,0");
//                         }
//                         input = "";
//                         break;
//                     }
//                     // Reset for reuse
//                     input = "";
//                 } else {
//                     input += c;
//                 }
//             }
//         }
//     } else {
//         Serial.println("Failed to get PDP!");
//     }


//     Serial.println("\n\n\nWaiting for network...");
//     if (!modem->waitForNetwork()) {
//         delay(10000);
//         return;
//     }

//     if (modem->isNetworkConnected()) {
//         Serial.println("Network connected");
//     }

//     Serial.println("\n---Starting GPRS TEST---\n");
//     Serial.println("Connecting to: " + String(apn));
//     if (!modem->gprsConnect(apn, gprsUser, gprsPass)) {
//         delay(10000);
//         return;
//     }

//     Serial.print("GPRS status: ");
//     if (modem->isGprsConnected()) {
//         Serial.println("connected");
//     } else {
//         Serial.println("not connected");
//     }

//     String ccid = modem->getSimCCID();
//     Serial.println("CCID: " + ccid);

//     String imei = modem->getIMEI();
//     Serial.println("IMEI: " + imei);

//     String cop = modem->getOperator();
//     Serial.println("Operator: " + cop);

//     IPAddress local = modem->localIP();
//     Serial.println("Local IP: " + String(local));

//     int csq = modem->getSignalQuality();
//     Serial.println("Signal quality: " + String(csq));

//     SerialAT.println("AT+CPSI?");     //Get connection type and band
//     delay(500);
//     if (SerialAT.available()) {
//         String r = SerialAT.readString();
//         Serial.println(r);
//     }

//     Serial.println("\n---End of GPRS TEST---\n");
// #endif

// // =============================================================================

// #if TINY_GSM_TEST_GPRS
//     modem->gprsDisconnect();
//     if (!modem->isGprsConnected()) {
//         Serial.println("GPRS disconnected");
//     } else {
//         Serial.println("GPRS disconnect: Failed.");
//     }
// #endif

// #if TINY_GSM_TEST_GPS
//     Serial.println("\n---Starting GPS TEST---\n");
//     // Set SIM7000G GPIO4 HIGH ,turn on GPS power
//     // CMD:AT+SGPIO=0,4,1,1
//     // Only in version 20200415 is there a function to control GPS power
//     modem->sendAT("+SGPIO=0,4,1,1");
//     if (modem->waitResponse(10000L) != 1) {
//         DBG(" SGPIO=0,4,1,1 false ");
//     }
//     modem->enableGPS();
//     float lat,  lon;
//     while (1) {
//         if (modem->getGPS(&lat, &lon)) {
//             Serial.printf("lat:%f lon:%f\n", lat, lon);
//             break;
//         } else {
//             Serial.print("getGPS ");
//             Serial.println(millis());
//         }
//         delay(2000);
//     }
//     modem->disableGPS();

//     // Set SIM7000G GPIO4 LOW ,turn off GPS power
//     // CMD:AT+SGPIO=0,4,1,0
//     // Only in version 20200415 is there a function to control GPS power
//     modem->sendAT("+SGPIO=0,4,1,0");
//     if (modem->waitResponse(10000L) != 1) {
//         DBG(" SGPIO=0,4,1,0 false ");
//     }
//     Serial.println("\n---End of GPRS TEST---\n");
// #endif


// #if TINY_GSM_POWERDOWN
//     // Try to power-off (modem may decide to restart automatically)
//     // To turn off modem completely, please use Reset/Enable pins
//     modem->sendAT("+CPOWD=1");
//     if (modem->waitResponse(10000L) != 1) {
//         DBG("+CPOWD=1");
//     }
//     modem->poweroff();
//     Serial.println("Poweroff.");
// #endif

//     esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
//     delay(200);
//     esp_deep_sleep_start();

//     // Do nothing forevermore
//     while (true) {
//         modem->maintain();
//     }

}
  


  
void mCellular::TestCode_RunAll()
{

  
    const char apn[]  = "giffgaff.com";     //SET TO YOUR APN
    const char gprsUser[] = "gg";
    const char gprsPass[] = "p";

    // Restart takes quite some time
    // To skip it, call init() instead of restart()
    Serial.println("Initializing modem->..");
    if (!modem->init()) {
        Serial.println("Failed to restart modem, attempting to continue without restarting");
    }

    String name = modem->getModemName();
    delay(500);
    Serial.println("Modem Name: " + name);

    String modemInfo = modem->getModemInfo();
    delay(500);
    Serial.println("Modem Info: " + modemInfo);

  // WDT_Reset(); yield();

    // Set SIM7000G GPIO4 LOW ,turn off GPS power
    // CMD:AT+SGPIO=0,4,1,0
    // Only in version 20200415 is there a function to control GPS power
    modem->sendAT("+SGPIO=0,4,1,0");
    if (modem->waitResponse(10000L) != 1) {
        DBG(" SGPIO=0,4,1,0 false ");
    }

#if TINY_GSM_TEST_GPRS
    // Unlock your SIM card with a PIN if needed
    if ( GSM_PIN && modem->getSimStatus() != 3 ) {
        modem->simUnlock(GSM_PIN);
    }
#endif

    modem->sendAT("+CFUN=0 ");
    if (modem->waitResponse(10000L) != 1) {
        DBG(" +CFUN=0  false ");
    }
    delay(200);

  // WDT_Reset(); yield();
    /*
      2 Automatic
      13 GSM only
      38 LTE only
      51 GSM and LTE only
    * * * */
    String res;
    res = modem->setNetworkMode(2);
    if (res != "1") {
        DBG("setNetworkMode  false ");
        return ;
    }
    delay(200);

  // WDT_Reset(); yield();
    /*
      1 CAT-M
      2 NB-Iot
      3 CAT-M and NB-IoT
    * * */
    res = modem->setPreferredMode(3);
    if (res != "1") {

        DBG("setPreferredMode  false ");
        return ;
    }
    delay(200);

  // WDT_Reset(); yield();
    /*AT+CBANDCFG=<mode>,<band>[,<band>…]
     * <mode> "CAT-M"   "NB-IOT"
     * <band>  The value of <band> must is in the band list of getting from  AT+CBANDCFG=?
     * For example, my SIM card carrier "NB-iot" supports B8.  I will configure +CBANDCFG= "Nb-iot ",8
     */
    /* modem->sendAT("+CBANDCFG=\"NB-IOT\",8 ");
     if (modem->waitResponse(10000L) != 1) {
         DBG(" +CBANDCFG=\"NB-IOT\" ");
     }
     delay(200);*/

    modem->sendAT("+CFUN=1 ");
    if (modem->waitResponse(10000L) != 1) {
        DBG(" +CFUN=1  false ");
    }
    delay(200);
  // WDT_Reset(); yield();


#if TINY_GSM_TEST_GPRS

    SerialAT.println("AT+CGDCONT?");
    delay(500);
    if (SerialAT.available()) {
        input = SerialAT.readString();
        for (int i = 0; i < input.length(); i++) {
            if (input.substring(i, i + 1) == "\n") {
                pieces[counter] = input.substring(lastIndex, i);
                lastIndex = i + 1;
                counter++;
            }
            if (i == input.length() - 1) {
                pieces[counter] = input.substring(lastIndex, i);
            }
        }
        // Reset for reuse
        input = "";
        counter = 0;
        lastIndex = 0;

        for ( int y = 0; y < numberOfPieces; y++) {
            for ( int x = 0; x < pieces[y].length(); x++) {
                char c = pieces[y][x];  //gets one byte from buffer
                if (c == ',') {
                    if (input.indexOf(": ") >= 0) {
                        String data = input.substring((input.indexOf(": ") + 1));
                        if ( data.toInt() > 0 && data.toInt() < 25) {
                            modem->sendAT("+CGDCONT=" + String(data.toInt()) + ",\"IP\",\"" + String(apn) + "\",\"0.0.0.0\",0,0,0,0");
                        }
                        input = "";
                        break;
                    }
                    // Reset for reuse
                    input = "";
                } else {
                    input += c;
                }
            }
        }
    } else {
        Serial.println("Failed to get PDP!");
    }


    Serial.println("\n\n\nWaiting for network...");
    if (!modem->waitForNetwork()) {
        delay(10000);
        return;
    }

    if (modem->isNetworkConnected()) {
        Serial.println("Network connected");
    }

    Serial.println("\n---Starting GPRS TEST---\n");
    Serial.println("Connecting to: " + String(apn));
    if (!modem->gprsConnect(apn, gprsUser, gprsPass)) {
        delay(10000);
        return;
    }

    Serial.print("GPRS status: ");
    if (modem->isGprsConnected()) {
        Serial.println("connected");
    } else {
        Serial.println("not connected");
    }

    String ccid = modem->getSimCCID();
    Serial.println("CCID: " + ccid);

    String imei = modem->getIMEI();
    Serial.println("IMEI: " + imei);

    String cop = modem->getOperator();
    Serial.println("Operator: " + cop);

    IPAddress local = modem->localIP();
    Serial.println("Local IP: " + String(local));

    int csq = modem->getSignalQuality();
    Serial.println("Signal quality: " + String(csq));

    SerialAT.println("AT+CPSI?");     //Get connection type and band
    delay(500);
    if (SerialAT.available()) {
        String r = SerialAT.readString();
        Serial.println(r);
    }

    Serial.println("\n---End of GPRS TEST---\n");
#endif

#if TINY_GSM_TEST_GPRS
    modem->gprsDisconnect();
    if (!modem->isGprsConnected()) {
        Serial.println("GPRS disconnected");
    } else {
        Serial.println("GPRS disconnect: Failed.");
    }
#endif

#if TINY_GSM_TEST_GPS
    Serial.println("\n---Starting GPS TEST---\n");
    // Set SIM7000G GPIO4 HIGH ,turn on GPS power
    // CMD:AT+SGPIO=0,4,1,1
    // Only in version 20200415 is there a function to control GPS power
    modem->sendAT("+SGPIO=0,4,1,1");
    if (modem->waitResponse(10000L) != 1) {
        DBG(" SGPIO=0,4,1,1 false ");
    }
    modem->enableGPS();
    float lat,  lon;
    while (1) {
        if (modem->getGPS(&lat, &lon)) {
            Serial.printf("lat:%f lon:%f\n", lat, lon);
            break;
        } else {
            Serial.print("getGPS ");
            Serial.println(millis());
        }
        delay(2000);
    }
    modem->disableGPS();

    // Set SIM7000G GPIO4 LOW ,turn off GPS power
    // CMD:AT+SGPIO=0,4,1,0
    // Only in version 20200415 is there a function to control GPS power
    modem->sendAT("+SGPIO=0,4,1,0");
    if (modem->waitResponse(10000L) != 1) {
        DBG(" SGPIO=0,4,1,0 false ");
    }
    Serial.println("\n---End of GPRS TEST---\n");
#endif


#if TINY_GSM_POWERDOWN
    // Try to power-off (modem may decide to restart automatically)
    // To turn off modem completely, please use Reset/Enable pins
    modem->sendAT("+CPOWD=1");
    if (modem->waitResponse(10000L) != 1) {
        DBG("+CPOWD=1");
    }
    modem->poweroff();
    Serial.println("Poweroff.");
#endif

    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    delay(200);
    esp_deep_sleep_start();

    // Do nothing forevermore
    while (true) {
        modem->maintain();
    }

}



void mCellular::ArduinoNetworkTest()
{    
    // // Set console baud rate
    // // SerialMon.begin(115200);

    // // Set LED OFF
    // pinMode(LED_PIN, OUTPUT);
    // digitalWrite(LED_PIN, HIGH);

    // modemPowerOn();

    // SPI.begin(SD_SCLK, SD_MISO, SD_MOSI);
    // if (!SD.begin(SD_CS)) {
    //     Serial.println("> It looks like you haven't inserted the SD card..");
    // } else {
    //     uint32_t cardSize = SD.cardSize() / (1024 * 1024);
    //     String str = "> SDCard Size: " + String(cardSize) + "MB";
    //     Serial.println(str);
    // }

    // SerialAT.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);

    // Serial.println("> Check whether Modem is online");
    // //test modem is online ?
    // uint32_t  timeout = millis();
    // while (!modem2.testAT()) {
    //     Serial.print(".");
    //     if (millis() - timeout > 60000 ) {
    //         Serial.println("> It looks like the modem is not responding, trying to restart");
    //         modemPowerOff();
    //         delay(5000);
    //         modemPowerOn();
    //         timeout = millis();
    //     }
    // }
    // Serial.println("\nModem is online");

    // //test sim card is online ?
    // timeout = millis();
    // Serial.print("> Get SIM card status");
    // while (modem2.getSimStatus() != SIM_READY) {
    //     Serial.print(".");
    //     if (millis() - timeout > 60000 ) {
    //         Serial.println("It seems that your SIM card has not been detected. Has it been inserted?");
    //         Serial.println("If you have inserted the SIM card, please remove the power supply again and try again!");
    //         return;
    //     }

    // }
    // Serial.println();
    // Serial.println("> SIM card exists");


    // Serial.println("> /**********************************************************/");
    // Serial.println("> Please make sure that the location has 2G/NB-IOT signal");
    // Serial.println("> SIM7000/SIM707G does not support 4G network. Please ensure that the USIM card you use supports 2G/NB access");
    // Serial.println("> /**********************************************************/");

    // String res = modem2.getIMEI();
    // Serial.print("IMEI:");
    // Serial.println(res);
    // Serial.println();

    // /*
    // * Tips:
    // * When you are not sure which method of network access is supported by the network you use,
    // * please use the automatic mode. If you are sure, please change the parameters to speed up the network access
    // * * * * */

    // //Set mobile operation band
    // modem2.sendAT("+CBAND=ALL_MODE");
    // modem2.waitResponse();

    // // Args:
    // // 1 CAT-M
    // // 2 NB-IoT
    // // 3 CAT-M and NB-IoT
    // // Set network preferre to auto
    // uint8_t perferred = 3;
    // modem2.setPreferredMode(perferred);

    // if (perferred == 2) {
    //     Serial.println("When you select 2, please ensure that your SIM card operator supports NB-IOT");
    // }

    // // Args:
    // // 2 Automatic
    // // 13 GSM only
    // // 38 LTE only
    // // 51 GSM and LTE only
    // // Set network mode to auto
    // modem2.setNetworkMode(2);


    // // Check network signal and registration information
    // Serial.println("> SIM7000/SIM7070 uses automatic mode to access the network. The access speed may be slow. Please wait patiently");
    // RegStatus status;
    // timeout = millis();
    // do {
    //     int16_t sq =  modem2.getSignalQuality();

    //     status = modem2.getRegistrationStatus();

    //     if (status == REG_DENIED) {
    //         Serial.println("> The SIM card you use has been rejected by the network operator. Please check that the card you use is not bound to a device!");
    //         return;
    //     } else {
    //         Serial.print("Signal:");
    //         Serial.println(sq);
    //     }

    //     if (millis() - timeout > 360000 ) {
    //         if (sq == 99) {
    //             Serial.println("> It seems that there is no signal. Please check whether the"\
    //                            "LTE antenna is connected. Please make sure that the location has 2G/NB-IOT signal\n"\
    //                            "SIM7000G does not support 4G network. Please ensure that the USIM card you use supports 2G/NB access");
    //             return;
    //         }
    //         timeout = millis();
    //     }

    //     delay(800);
    // } while (status != REG_OK_HOME && status != REG_OK_ROAMING);

    // Serial.println("Obtain the APN issued by the network");
    // modem2.sendAT("+CGNAPN");
    // if (modem2.waitResponse(3000, res) == 1) {
    //     res = res.substring(res.indexOf(",") + 1);
    //     res.replace("\"", "");
    //     res.replace("\r", "");
    //     res.replace("\n", "");
    //     res.replace("OK", "");
    //     Serial.print("The APN issued by the network is:");
    //     Serial.println(res);
    // }

    // modem2.sendAT("+CNACT=1");
    // modem2.waitResponse();


    // // res = modem.getLocalIP();
    // modem2.sendAT("+CNACT?");
    // if (modem2.waitResponse("+CNACT: ") == 1) {
    //     modem2.stream.read();
    //     modem2.stream.read();
    //     res = modem2.stream.readStringUntil('\n');
    //     res.replace("\"", "");
    //     res.replace("\r", "");
    //     res.replace("\n", "");
    //     modem2.waitResponse();
    //     Serial.print("The current network IP address is:");
    //     Serial.println(res);
    // }


    // modem2.sendAT("+CPSI?");
    // if (modem2.waitResponse("+CPSI: ") == 1) {
    //     res = modem2.stream.readStringUntil('\n');
    //     res.replace("\r", "");
    //     res.replace("\n", "");
    //     modem2.waitResponse();
    //     Serial.print("The current network parameter is:");
    //     Serial.println(res);
    // }


    // Serial.println("/**********************************************************/");
    // Serial.println("After the network test is complete, please enter the  ");
    // Serial.println("AT command in the serial terminal.");
    // Serial.println("/**********************************************************/\n\n");


}



void mCellular::ArduinoExample_GPRSConnect()
{




    const char apn[]  = "giffgaff.com";     //SET TO YOUR APN
    const char gprsUser[] = "gg";
    const char gprsPass[] = "p";

    SerialAT.println("AT+CGDCONT?");
    delay(500);

    DEBUG_LINE_HERE;

    if (SerialAT.available()) {
    DEBUG_LINE_HERE;

        input = SerialAT.readString();
    DEBUG_LINE_HERE;

        ALOG_INF(PSTR("input=%s"),input);
        ALOG_INF(PSTR("input.length()=%d"),input.length());
    DEBUG_LINE_HERE;

        for (int i = 0; i < input.length(); i++) {
            if (input.substring(i, i + 1) == "\n") {
                pieces[counter] = input.substring(lastIndex, i);
                lastIndex = i + 1;
                counter++;
            }
            if (i == input.length() - 1) {
                pieces[counter] = input.substring(lastIndex, i);
            }
        }
        // Reset for reuse
        input = "";
        counter = 0;
        lastIndex = 0;

    DEBUG_LINE_HERE;

        for ( int y = 0; y < numberOfPieces; y++) {
            for ( int x = 0; x < pieces[y].length(); x++) {
                char c = pieces[y][x];  //gets one byte from buffer
                if (c == ',') {
                    if (input.indexOf(": ") >= 0) {
                        String data = input.substring((input.indexOf(": ") + 1));
                        if ( data.toInt() > 0 && data.toInt() < 25) {
                            modem->sendAT("+CGDCONT=" + String(data.toInt()) + ",\"IP\",\"" + String(apn) + "\",\"0.0.0.0\",0,0,0,0");
                        }
                        input = "";
                        break;
                    }
                    // Reset for reuse
                    input = "";
                } else {
                    input += c;
                }
            }
        }
    DEBUG_LINE_HERE;

    } else {
        Serial.println("Failed to get PDP!");
    }

    DEBUG_LINE_HERE;


    Serial.println("\n\n\nWaiting for network...");
    if (!modem->waitForNetwork()) {
        delay(10000);
        return;
    }

    if (modem->isNetworkConnected()) {
        Serial.println("Network connected");
    }

    Serial.println("\n---Starting GPRS TEST---\n");
    Serial.println("Connecting to: " + String(apn));
    if (!modem->gprsConnect(apn, gprsUser, gprsPass)) {
        delay(10000);
        return;
    }

    Serial.print("GPRS status: ");
    if (modem->isGprsConnected()) {
        Serial.println("connected");
    } else {
        Serial.println("not connected");
    }

    String ccid = modem->getSimCCID();
    Serial.println("CCID: " + ccid);

    String imei = modem->getIMEI();
    Serial.println("IMEI: " + imei);

    String cop = modem->getOperator();
    Serial.println("Operator: " + cop);

    IPAddress local = modem->localIP();
    Serial.println("Local IP: " + String(local));

    int csq = modem->getSignalQuality();
    Serial.println("Signal quality: " + String(csq));

    SerialAT.println("AT+CPSI?");     //Get connection type and band
    delay(500);
    if (SerialAT.available()) {
        String r = SerialAT.readString();
        Serial.println(r);
    }

    Serial.println("\n---End of GPRS TEST---\n");



}




void mCellular::ArduinoExample_GPRSConnect1()
{

    
    const char apn[]  = "giffgaff.com";     //SET TO YOUR APN
    const char gprsUser[] = "gg";
    const char gprsPass[] = "p";

    // Restart takes quite some time
    // To skip it, call init() instead of restart()
    Serial.println("Initializing modem->..");
    if (!modem->init()) {
        Serial.println("Failed to restart modem, attempting to continue without restarting");
    }

    String name = modem->getModemName();
    Serial.println("Modem Name: " + name);

    String modemInfo = modem->getModemInfo();
    Serial.println("Modem Info: " + modemInfo);

    // Set SIM7000G GPIO4 LOW ,turn off GPS power
    // CMD:AT+SGPIO=0,4,1,0
    // Only in version 20200415 is there a function to control GPS power
    modem->sendAT("+SGPIO=0,4,1,0");
    if (modem->waitResponse(10000L) != 1) {
        DBG(" SGPIO=0,4,1,0 false ");
    }

#if TINY_GSM_TEST_GPRS
    // Unlock your SIM card with a PIN if needed
    if ( GSM_PIN && modem->getSimStatus() != 3 ) {
        modem->simUnlock(GSM_PIN);
    }
#endif

    modem->sendAT("+CFUN=0 ");
    if (modem->waitResponse(10000L) != 1) {
        DBG(" +CFUN=0  false ");
    }
    delay(200);

  // WDT_Reset(); yield();
    /*
      2 Automatic
      13 GSM only
      38 LTE only
      51 GSM and LTE only
    * * * */
    String res;
    res = modem->setNetworkMode(2);
    if (res != "1") {
        DBG("setNetworkMode  false ");
        return ;
    }
    delay(200);

  // WDT_Reset(); yield();
    /*
      1 CAT-M
      2 NB-Iot
      3 CAT-M and NB-IoT
    * * */
    res = modem->setPreferredMode(3);
    if (res != "1") {

        DBG("setPreferredMode  false ");
        return ;
    }
    delay(200);

  // WDT_Reset(); yield();
    /*AT+CBANDCFG=<mode>,<band>[,<band>…]
     * <mode> "CAT-M"   "NB-IOT"
     * <band>  The value of <band> must is in the band list of getting from  AT+CBANDCFG=?
     * For example, my SIM card carrier "NB-iot" supports B8.  I will configure +CBANDCFG= "Nb-iot ",8
     */
    /* modem->sendAT("+CBANDCFG=\"NB-IOT\",8 ");
     if (modem->waitResponse(10000L) != 1) {
         DBG(" +CBANDCFG=\"NB-IOT\" ");
     }
     delay(200);*/

    modem->sendAT("+CFUN=1 ");
    if (modem->waitResponse(10000L) != 1) {
        DBG(" +CFUN=1  false ");
    }
    delay(200);
  // WDT_Reset(); yield();



}


#endif





// #include "mTinyGSM.h"

// #ifdef USE_MODULE_DRIVERS_TINYGSM



//     #define SerialMon Serial


// void mTinyGSM::EveryLoop()
// {

// //   if(pCONT_time->uptime_seconds_nonreset<120){ return; }


  
// //     const char apn[]  = "giffgaff.com";     //SET TO YOUR APN
// //     const char gprsUser[] = "gg";
// //     const char gprsPass[] = "p";

// //     // Restart takes quite some time
// //     // To skip it, call init() instead of restart()
// //     Serial.println("Initializing modem->..");
// //     if (!modem->init()) {
// //         Serial.println("Failed to restart modem, attempting to continue without restarting");
// //     }

// //     String name = modem->getModemName();
// //     delay(500);
// //     Serial.println("Modem Name: " + name);

// //     String modemInfo = modem->getModemInfo();
// //     delay(500);
// //     Serial.println("Modem Info: " + modemInfo);

// //   // WDT_Reset(); yield();

// //     // Set SIM7000G GPIO4 LOW ,turn off GPS power
// //     // CMD:AT+SGPIO=0,4,1,0
// //     // Only in version 20200415 is there a function to control GPS power
// //     modem->sendAT("+SGPIO=0,4,1,0");
// //     if (modem->waitResponse(10000L) != 1) {
// //         DBG(" SGPIO=0,4,1,0 false ");
// //     }

// // #if TINY_GSM_TEST_GPRS
// //     // Unlock your SIM card with a PIN if needed
// //     if ( GSM_PIN && modem->getSimStatus() != 3 ) {
// //         modem->simUnlock(GSM_PIN);
// //     }
// // #endif

// //     modem->sendAT("+CFUN=0 ");
// //     if (modem->waitResponse(10000L) != 1) {
// //         DBG(" +CFUN=0  false ");
// //     }
    
// //   /*
// //     2 Automatic
// //     13 GSM only
// //     38 LTE only
// //     51 GSM and LTE only
// //   * * * */
// //   String res;
// //   res = modem->setNetworkMode(2);
// //   if (res != "1") {
// //       DBG("setNetworkMode  false ");
// //       return ;
// //   }

// //   /**
// //    * @brief 
// //     Set the preferred selection between CAT-M and NB-IOT
// //     1 CAT-M
// //     2 NB-Iot
// //     3 CAT-M and NB-IoT
// //   **/
// //   res = modem->setPreferredMode(3);
// //   if (res != "1")
// //   {
// //     ALOG_DBG(PSTR("setPreferredMode false"));
// //   }

// //   /**
// //    * @brief 
// //    * Response syntax:
// //     +CFUN: <fun>
// //     The read response parameter and its defined value are the following:
// //     <fun>
// //       0 – Power off and store. RF circuits are disabled by deactivating LTE and Global Navigation Satellite System (GNSS) services.
// //       1 – Normal mode. The active mode is either LTE or GNSS, or both. Full functional mode. Active modes depend on %XSYSTEMMODE setting.
// //       2 – Receive only mode in active system mode. Active modes depend on %XSYSTEMMODE setting.
// //       4 – Flight mode. RF circuits are disabled by deactivating LTE and GNSS services. 
// //     **/
// //   modem->sendAT("+CFUN=1 ");
// //   if (modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT) != 1) 
// //   {
// //     ALOG_DBG(PSTR(" +CFUN=1  false "));
// //   }

// //   /**
// //    * @brief Make it so new SMS messages are send over serial automatically
// //    * +CNMI: <mode>,<mt>,<bm>,<ds>,<bfr>
// //    * The read command parameters and their defined values are the following:
// //     <mode>
// //       0 – Do not forward unsolicited result codes to the Terminal Equipment (TE) (default)
// //       3 – Forward unsolicited result codes directly to the TE
// //     <mt>
// //       0 – No received message notifications, the modem acts as an SMS client
// //       2 – SMS-DELIVERs (except class 2 and message waiting indication group) are routed directly to the TE
// //     <bm>
// //       No CBM notifications are routed to the TE
// //     <ds>
// //       0 – No SMS-STATUS-REPORTs are routed to the TE
// //       1 – SMS-STATUS-REPORTs are routed to the TE using unsolicited result code: +CDS: <length><CR><LF><pdu>
// //     <bfr>
// //       1 – The buffer of unsolicited result codes is cleared when <mode> 1 to 3 is entered
// //   **/
// //   modem->sendAT("+CNMI= 2,2,0,0,0 "); 
// //   if (modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT) != 1) 
// //   {
// //     ALOG_DBG(PSTR(" +CNMI= 2,2,0,0,0  false "));
// //   }




// // // =============================================================================

// // #if TINY_GSM_TEST_GPRS

// //     SerialAT.println("AT+CGDCONT?");
// //     delay(500);
// //     if (SerialAT.available()) {
// //         input = SerialAT.readString();
// //         for (int i = 0; i < input.length(); i++) {
// //             if (input.substring(i, i + 1) == "\n") {
// //                 pieces[counter] = input.substring(lastIndex, i);
// //                 lastIndex = i + 1;
// //                 counter++;
// //             }
// //             if (i == input.length() - 1) {
// //                 pieces[counter] = input.substring(lastIndex, i);
// //             }
// //         }
// //         // Reset for reuse
// //         input = "";
// //         counter = 0;
// //         lastIndex = 0;

// //         for ( int y = 0; y < numberOfPieces; y++) {
// //             for ( int x = 0; x < pieces[y].length(); x++) {
// //                 char c = pieces[y][x];  //gets one byte from buffer
// //                 if (c == ',') {
// //                     if (input.indexOf(": ") >= 0) {
// //                         String data = input.substring((input.indexOf(": ") + 1));
// //                         if ( data.toInt() > 0 && data.toInt() < 25) {
// //                             modem->sendAT("+CGDCONT=" + String(data.toInt()) + ",\"IP\",\"" + String(apn) + "\",\"0.0.0.0\",0,0,0,0");
// //                         }
// //                         input = "";
// //                         break;
// //                     }
// //                     // Reset for reuse
// //                     input = "";
// //                 } else {
// //                     input += c;
// //                 }
// //             }
// //         }
// //     } else {
// //         Serial.println("Failed to get PDP!");
// //     }


// //     Serial.println("\n\n\nWaiting for network...");
// //     if (!modem->waitForNetwork()) {
// //         delay(10000);
// //         return;
// //     }

// //     if (modem->isNetworkConnected()) {
// //         Serial.println("Network connected");
// //     }

// //     Serial.println("\n---Starting GPRS TEST---\n");
// //     Serial.println("Connecting to: " + String(apn));
// //     if (!modem->gprsConnect(apn, gprsUser, gprsPass)) {
// //         delay(10000);
// //         return;
// //     }

// //     Serial.print("GPRS status: ");
// //     if (modem->isGprsConnected()) {
// //         Serial.println("connected");
// //     } else {
// //         Serial.println("not connected");
// //     }

// //     String ccid = modem->getSimCCID();
// //     Serial.println("CCID: " + ccid);

// //     String imei = modem->getIMEI();
// //     Serial.println("IMEI: " + imei);

// //     String cop = modem->getOperator();
// //     Serial.println("Operator: " + cop);

// //     IPAddress local = modem->localIP();
// //     Serial.println("Local IP: " + String(local));

// //     int csq = modem->getSignalQuality();
// //     Serial.println("Signal quality: " + String(csq));

// //     SerialAT.println("AT+CPSI?");     //Get connection type and band
// //     delay(500);
// //     if (SerialAT.available()) {
// //         String r = SerialAT.readString();
// //         Serial.println(r);
// //     }

// //     Serial.println("\n---End of GPRS TEST---\n");
// // #endif

// // // =============================================================================

// // #if TINY_GSM_TEST_GPRS
// //     modem->gprsDisconnect();
// //     if (!modem->isGprsConnected()) {
// //         Serial.println("GPRS disconnected");
// //     } else {
// //         Serial.println("GPRS disconnect: Failed.");
// //     }
// // #endif

// // #if TINY_GSM_TEST_GPS
// //     Serial.println("\n---Starting GPS TEST---\n");
// //     // Set SIM7000G GPIO4 HIGH ,turn on GPS power
// //     // CMD:AT+SGPIO=0,4,1,1
// //     // Only in version 20200415 is there a function to control GPS power
// //     modem->sendAT("+SGPIO=0,4,1,1");
// //     if (modem->waitResponse(10000L) != 1) {
// //         DBG(" SGPIO=0,4,1,1 false ");
// //     }
// //     modem->enableGPS();
// //     float lat,  lon;
// //     while (1) {
// //         if (modem->getGPS(&lat, &lon)) {
// //             Serial.printf("lat:%f lon:%f\n", lat, lon);
// //             break;
// //         } else {
// //             Serial.print("getGPS ");
// //             Serial.println(millis());
// //         }
// //         delay(2000);
// //     }
// //     modem->disableGPS();

// //     // Set SIM7000G GPIO4 LOW ,turn off GPS power
// //     // CMD:AT+SGPIO=0,4,1,0
// //     // Only in version 20200415 is there a function to control GPS power
// //     modem->sendAT("+SGPIO=0,4,1,0");
// //     if (modem->waitResponse(10000L) != 1) {
// //         DBG(" SGPIO=0,4,1,0 false ");
// //     }
// //     Serial.println("\n---End of GPRS TEST---\n");
// // #endif


// // #if TINY_GSM_POWERDOWN
// //     // Try to power-off (modem may decide to restart automatically)
// //     // To turn off modem completely, please use Reset/Enable pins
// //     modem->sendAT("+CPOWD=1");
// //     if (modem->waitResponse(10000L) != 1) {
// //         DBG("+CPOWD=1");
// //     }
// //     modem->poweroff();
// //     Serial.println("Poweroff.");
// // #endif

// //     esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
// //     delay(200);
// //     esp_deep_sleep_start();

// //     // Do nothing forevermore
// //     while (true) {
// //         modem->maintain();
// //     }

// }
  


  
// void mTinyGSM::TestCode_RunAll()
// {

  
//     const char apn[]  = "giffgaff.com";     //SET TO YOUR APN
//     const char gprsUser[] = "gg";
//     const char gprsPass[] = "p";

//     // Restart takes quite some time
//     // To skip it, call init() instead of restart()
//     Serial.println("Initializing modem->..");
//     if (!modem->init()) {
//         Serial.println("Failed to restart modem, attempting to continue without restarting");
//     }

//     String name = modem->getModemName();
//     delay(500);
//     Serial.println("Modem Name: " + name);

//     String modemInfo = modem->getModemInfo();
//     delay(500);
//     Serial.println("Modem Info: " + modemInfo);

//   // WDT_Reset(); yield();

//     // Set SIM7000G GPIO4 LOW ,turn off GPS power
//     // CMD:AT+SGPIO=0,4,1,0
//     // Only in version 20200415 is there a function to control GPS power
//     modem->sendAT("+SGPIO=0,4,1,0");
//     if (modem->waitResponse(10000L) != 1) {
//         DBG(" SGPIO=0,4,1,0 false ");
//     }

// #if TINY_GSM_TEST_GPRS
//     // Unlock your SIM card with a PIN if needed
//     if ( GSM_PIN && modem->getSimStatus() != 3 ) {
//         modem->simUnlock(GSM_PIN);
//     }
// #endif

//     modem->sendAT("+CFUN=0 ");
//     if (modem->waitResponse(10000L) != 1) {
//         DBG(" +CFUN=0  false ");
//     }
//     delay(200);

//   // WDT_Reset(); yield();
//     /*
//       2 Automatic
//       13 GSM only
//       38 LTE only
//       51 GSM and LTE only
//     * * * */
//     String res;
//     res = modem->setNetworkMode(2);
//     if (res != "1") {
//         DBG("setNetworkMode  false ");
//         return ;
//     }
//     delay(200);

//   // WDT_Reset(); yield();
//     /*
//       1 CAT-M
//       2 NB-Iot
//       3 CAT-M and NB-IoT
//     * * */
//     res = modem->setPreferredMode(3);
//     if (res != "1") {

//         DBG("setPreferredMode  false ");
//         return ;
//     }
//     delay(200);

//   // WDT_Reset(); yield();
//     /*AT+CBANDCFG=<mode>,<band>[,<band>…]
//      * <mode> "CAT-M"   "NB-IOT"
//      * <band>  The value of <band> must is in the band list of getting from  AT+CBANDCFG=?
//      * For example, my SIM card carrier "NB-iot" supports B8.  I will configure +CBANDCFG= "Nb-iot ",8
//      */
//     /* modem->sendAT("+CBANDCFG=\"NB-IOT\",8 ");
//      if (modem->waitResponse(10000L) != 1) {
//          DBG(" +CBANDCFG=\"NB-IOT\" ");
//      }
//      delay(200);*/

//     modem->sendAT("+CFUN=1 ");
//     if (modem->waitResponse(10000L) != 1) {
//         DBG(" +CFUN=1  false ");
//     }
//     delay(200);
//   // WDT_Reset(); yield();


// #if TINY_GSM_TEST_GPRS

//     SerialAT.println("AT+CGDCONT?");
//     delay(500);
//     if (SerialAT.available()) {
//         input = SerialAT.readString();
//         for (int i = 0; i < input.length(); i++) {
//             if (input.substring(i, i + 1) == "\n") {
//                 pieces[counter] = input.substring(lastIndex, i);
//                 lastIndex = i + 1;
//                 counter++;
//             }
//             if (i == input.length() - 1) {
//                 pieces[counter] = input.substring(lastIndex, i);
//             }
//         }
//         // Reset for reuse
//         input = "";
//         counter = 0;
//         lastIndex = 0;

//         for ( int y = 0; y < numberOfPieces; y++) {
//             for ( int x = 0; x < pieces[y].length(); x++) {
//                 char c = pieces[y][x];  //gets one byte from buffer
//                 if (c == ',') {
//                     if (input.indexOf(": ") >= 0) {
//                         String data = input.substring((input.indexOf(": ") + 1));
//                         if ( data.toInt() > 0 && data.toInt() < 25) {
//                             modem->sendAT("+CGDCONT=" + String(data.toInt()) + ",\"IP\",\"" + String(apn) + "\",\"0.0.0.0\",0,0,0,0");
//                         }
//                         input = "";
//                         break;
//                     }
//                     // Reset for reuse
//                     input = "";
//                 } else {
//                     input += c;
//                 }
//             }
//         }
//     } else {
//         Serial.println("Failed to get PDP!");
//     }


//     Serial.println("\n\n\nWaiting for network...");
//     if (!modem->waitForNetwork()) {
//         delay(10000);
//         return;
//     }

//     if (modem->isNetworkConnected()) {
//         Serial.println("Network connected");
//     }

//     Serial.println("\n---Starting GPRS TEST---\n");
//     Serial.println("Connecting to: " + String(apn));
//     if (!modem->gprsConnect(apn, gprsUser, gprsPass)) {
//         delay(10000);
//         return;
//     }

//     Serial.print("GPRS status: ");
//     if (modem->isGprsConnected()) {
//         Serial.println("connected");
//     } else {
//         Serial.println("not connected");
//     }

//     String ccid = modem->getSimCCID();
//     Serial.println("CCID: " + ccid);

//     String imei = modem->getIMEI();
//     Serial.println("IMEI: " + imei);

//     String cop = modem->getOperator();
//     Serial.println("Operator: " + cop);

//     IPAddress local = modem->localIP();
//     Serial.println("Local IP: " + String(local));

//     int csq = modem->getSignalQuality();
//     Serial.println("Signal quality: " + String(csq));

//     SerialAT.println("AT+CPSI?");     //Get connection type and band
//     delay(500);
//     if (SerialAT.available()) {
//         String r = SerialAT.readString();
//         Serial.println(r);
//     }

//     Serial.println("\n---End of GPRS TEST---\n");
// #endif

// #if TINY_GSM_TEST_GPRS
//     modem->gprsDisconnect();
//     if (!modem->isGprsConnected()) {
//         Serial.println("GPRS disconnected");
//     } else {
//         Serial.println("GPRS disconnect: Failed.");
//     }
// #endif

// #if TINY_GSM_TEST_GPS
//     Serial.println("\n---Starting GPS TEST---\n");
//     // Set SIM7000G GPIO4 HIGH ,turn on GPS power
//     // CMD:AT+SGPIO=0,4,1,1
//     // Only in version 20200415 is there a function to control GPS power
//     modem->sendAT("+SGPIO=0,4,1,1");
//     if (modem->waitResponse(10000L) != 1) {
//         DBG(" SGPIO=0,4,1,1 false ");
//     }
//     modem->enableGPS();
//     float lat,  lon;
//     while (1) {
//         if (modem->getGPS(&lat, &lon)) {
//             Serial.printf("lat:%f lon:%f\n", lat, lon);
//             break;
//         } else {
//             Serial.print("getGPS ");
//             Serial.println(millis());
//         }
//         delay(2000);
//     }
//     modem->disableGPS();

//     // Set SIM7000G GPIO4 LOW ,turn off GPS power
//     // CMD:AT+SGPIO=0,4,1,0
//     // Only in version 20200415 is there a function to control GPS power
//     modem->sendAT("+SGPIO=0,4,1,0");
//     if (modem->waitResponse(10000L) != 1) {
//         DBG(" SGPIO=0,4,1,0 false ");
//     }
//     Serial.println("\n---End of GPRS TEST---\n");
// #endif


// #if TINY_GSM_POWERDOWN
//     // Try to power-off (modem may decide to restart automatically)
//     // To turn off modem completely, please use Reset/Enable pins
//     modem->sendAT("+CPOWD=1");
//     if (modem->waitResponse(10000L) != 1) {
//         DBG("+CPOWD=1");
//     }
//     modem->poweroff();
//     Serial.println("Poweroff.");
// #endif

//     esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
//     delay(200);
//     esp_deep_sleep_start();

//     // Do nothing forevermore
//     while (true) {
//         modem->maintain();
//     }

// }



// void mTinyGSM::ArduinoNetworkTest()
// {    
//     // // Set console baud rate
//     // // SerialMon.begin(115200);

//     // // Set LED OFF
//     // pinMode(LED_PIN, OUTPUT);
//     // digitalWrite(LED_PIN, HIGH);

//     // modemPowerOn();

//     // SPI.begin(SD_SCLK, SD_MISO, SD_MOSI);
//     // if (!SD.begin(SD_CS)) {
//     //     Serial.println("> It looks like you haven't inserted the SD card..");
//     // } else {
//     //     uint32_t cardSize = SD.cardSize() / (1024 * 1024);
//     //     String str = "> SDCard Size: " + String(cardSize) + "MB";
//     //     Serial.println(str);
//     // }

//     // SerialAT.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);

//     // Serial.println("> Check whether Modem is online");
//     // //test modem is online ?
//     // uint32_t  timeout = millis();
//     // while (!modem2.testAT()) {
//     //     Serial.print(".");
//     //     if (millis() - timeout > 60000 ) {
//     //         Serial.println("> It looks like the modem is not responding, trying to restart");
//     //         modemPowerOff();
//     //         delay(5000);
//     //         modemPowerOn();
//     //         timeout = millis();
//     //     }
//     // }
//     // Serial.println("\nModem is online");

//     // //test sim card is online ?
//     // timeout = millis();
//     // Serial.print("> Get SIM card status");
//     // while (modem2.getSimStatus() != SIM_READY) {
//     //     Serial.print(".");
//     //     if (millis() - timeout > 60000 ) {
//     //         Serial.println("It seems that your SIM card has not been detected. Has it been inserted?");
//     //         Serial.println("If you have inserted the SIM card, please remove the power supply again and try again!");
//     //         return;
//     //     }

//     // }
//     // Serial.println();
//     // Serial.println("> SIM card exists");


//     // Serial.println("> /**********************************************************/");
//     // Serial.println("> Please make sure that the location has 2G/NB-IOT signal");
//     // Serial.println("> SIM7000/SIM707G does not support 4G network. Please ensure that the USIM card you use supports 2G/NB access");
//     // Serial.println("> /**********************************************************/");

//     // String res = modem2.getIMEI();
//     // Serial.print("IMEI:");
//     // Serial.println(res);
//     // Serial.println();

//     // /*
//     // * Tips:
//     // * When you are not sure which method of network access is supported by the network you use,
//     // * please use the automatic mode. If you are sure, please change the parameters to speed up the network access
//     // * * * * */

//     // //Set mobile operation band
//     // modem2.sendAT("+CBAND=ALL_MODE");
//     // modem2.waitResponse();

//     // // Args:
//     // // 1 CAT-M
//     // // 2 NB-IoT
//     // // 3 CAT-M and NB-IoT
//     // // Set network preferre to auto
//     // uint8_t perferred = 3;
//     // modem2.setPreferredMode(perferred);

//     // if (perferred == 2) {
//     //     Serial.println("When you select 2, please ensure that your SIM card operator supports NB-IOT");
//     // }

//     // // Args:
//     // // 2 Automatic
//     // // 13 GSM only
//     // // 38 LTE only
//     // // 51 GSM and LTE only
//     // // Set network mode to auto
//     // modem2.setNetworkMode(2);


//     // // Check network signal and registration information
//     // Serial.println("> SIM7000/SIM7070 uses automatic mode to access the network. The access speed may be slow. Please wait patiently");
//     // RegStatus status;
//     // timeout = millis();
//     // do {
//     //     int16_t sq =  modem2.getSignalQuality();

//     //     status = modem2.getRegistrationStatus();

//     //     if (status == REG_DENIED) {
//     //         Serial.println("> The SIM card you use has been rejected by the network operator. Please check that the card you use is not bound to a device!");
//     //         return;
//     //     } else {
//     //         Serial.print("Signal:");
//     //         Serial.println(sq);
//     //     }

//     //     if (millis() - timeout > 360000 ) {
//     //         if (sq == 99) {
//     //             Serial.println("> It seems that there is no signal. Please check whether the"\
//     //                            "LTE antenna is connected. Please make sure that the location has 2G/NB-IOT signal\n"\
//     //                            "SIM7000G does not support 4G network. Please ensure that the USIM card you use supports 2G/NB access");
//     //             return;
//     //         }
//     //         timeout = millis();
//     //     }

//     //     delay(800);
//     // } while (status != REG_OK_HOME && status != REG_OK_ROAMING);

//     // Serial.println("Obtain the APN issued by the network");
//     // modem2.sendAT("+CGNAPN");
//     // if (modem2.waitResponse(3000, res) == 1) {
//     //     res = res.substring(res.indexOf(",") + 1);
//     //     res.replace("\"", "");
//     //     res.replace("\r", "");
//     //     res.replace("\n", "");
//     //     res.replace("OK", "");
//     //     Serial.print("The APN issued by the network is:");
//     //     Serial.println(res);
//     // }

//     // modem2.sendAT("+CNACT=1");
//     // modem2.waitResponse();


//     // // res = modem.getLocalIP();
//     // modem2.sendAT("+CNACT?");
//     // if (modem2.waitResponse("+CNACT: ") == 1) {
//     //     modem2.stream.read();
//     //     modem2.stream.read();
//     //     res = modem2.stream.readStringUntil('\n');
//     //     res.replace("\"", "");
//     //     res.replace("\r", "");
//     //     res.replace("\n", "");
//     //     modem2.waitResponse();
//     //     Serial.print("The current network IP address is:");
//     //     Serial.println(res);
//     // }


//     // modem2.sendAT("+CPSI?");
//     // if (modem2.waitResponse("+CPSI: ") == 1) {
//     //     res = modem2.stream.readStringUntil('\n');
//     //     res.replace("\r", "");
//     //     res.replace("\n", "");
//     //     modem2.waitResponse();
//     //     Serial.print("The current network parameter is:");
//     //     Serial.println(res);
//     // }


//     // Serial.println("/**********************************************************/");
//     // Serial.println("After the network test is complete, please enter the  ");
//     // Serial.println("AT command in the serial terminal.");
//     // Serial.println("/**********************************************************/\n\n");


// }



// void mTinyGSM::ArduinoExample_GPRSConnect()
// {




//     const char apn[]  = "giffgaff.com";     //SET TO YOUR APN
//     const char gprsUser[] = "gg";
//     const char gprsPass[] = "p";

//     SerialAT.println("AT+CGDCONT?");
//     delay(500);

//     DEBUG_LINE_HERE;

//     if (SerialAT.available()) {
//     DEBUG_LINE_HERE;

//         input = SerialAT.readString();
//     DEBUG_LINE_HERE;

//         ALOG_INF(PSTR("input=%s"),input);
//         ALOG_INF(PSTR("input.length()=%d"),input.length());
//     DEBUG_LINE_HERE;

//         for (int i = 0; i < input.length(); i++) {
//             if (input.substring(i, i + 1) == "\n") {
//                 pieces[counter] = input.substring(lastIndex, i);
//                 lastIndex = i + 1;
//                 counter++;
//             }
//             if (i == input.length() - 1) {
//                 pieces[counter] = input.substring(lastIndex, i);
//             }
//         }
//         // Reset for reuse
//         input = "";
//         counter = 0;
//         lastIndex = 0;

//     DEBUG_LINE_HERE;

//         for ( int y = 0; y < numberOfPieces; y++) {
//             for ( int x = 0; x < pieces[y].length(); x++) {
//                 char c = pieces[y][x];  //gets one byte from buffer
//                 if (c == ',') {
//                     if (input.indexOf(": ") >= 0) {
//                         String data = input.substring((input.indexOf(": ") + 1));
//                         if ( data.toInt() > 0 && data.toInt() < 25) {
//                             modem->sendAT("+CGDCONT=" + String(data.toInt()) + ",\"IP\",\"" + String(apn) + "\",\"0.0.0.0\",0,0,0,0");
//                         }
//                         input = "";
//                         break;
//                     }
//                     // Reset for reuse
//                     input = "";
//                 } else {
//                     input += c;
//                 }
//             }
//         }
//     DEBUG_LINE_HERE;

//     } else {
//         Serial.println("Failed to get PDP!");
//     }

//     DEBUG_LINE_HERE;


//     Serial.println("\n\n\nWaiting for network...");
//     if (!modem->waitForNetwork()) {
//         delay(10000);
//         return;
//     }

//     if (modem->isNetworkConnected()) {
//         Serial.println("Network connected");
//     }

//     Serial.println("\n---Starting GPRS TEST---\n");
//     Serial.println("Connecting to: " + String(apn));
//     if (!modem->gprsConnect(apn, gprsUser, gprsPass)) {
//         delay(10000);
//         return;
//     }

//     Serial.print("GPRS status: ");
//     if (modem->isGprsConnected()) {
//         Serial.println("connected");
//     } else {
//         Serial.println("not connected");
//     }

//     String ccid = modem->getSimCCID();
//     Serial.println("CCID: " + ccid);

//     String imei = modem->getIMEI();
//     Serial.println("IMEI: " + imei);

//     String cop = modem->getOperator();
//     Serial.println("Operator: " + cop);

//     IPAddress local = modem->localIP();
//     Serial.println("Local IP: " + String(local));

//     int csq = modem->getSignalQuality();
//     Serial.println("Signal quality: " + String(csq));

//     SerialAT.println("AT+CPSI?");     //Get connection type and band
//     delay(500);
//     if (SerialAT.available()) {
//         String r = SerialAT.readString();
//         Serial.println(r);
//     }

//     Serial.println("\n---End of GPRS TEST---\n");



// }




// void mTinyGSM::ArduinoExample_GPRSConnect1()
// {

    
//     const char apn[]  = "giffgaff.com";     //SET TO YOUR APN
//     const char gprsUser[] = "gg";
//     const char gprsPass[] = "p";

//     // Restart takes quite some time
//     // To skip it, call init() instead of restart()
//     Serial.println("Initializing modem->..");
//     if (!modem->init()) {
//         Serial.println("Failed to restart modem, attempting to continue without restarting");
//     }

//     String name = modem->getModemName();
//     Serial.println("Modem Name: " + name);

//     String modemInfo = modem->getModemInfo();
//     Serial.println("Modem Info: " + modemInfo);

//     // Set SIM7000G GPIO4 LOW ,turn off GPS power
//     // CMD:AT+SGPIO=0,4,1,0
//     // Only in version 20200415 is there a function to control GPS power
//     modem->sendAT("+SGPIO=0,4,1,0");
//     if (modem->waitResponse(10000L) != 1) {
//         DBG(" SGPIO=0,4,1,0 false ");
//     }

// #if TINY_GSM_TEST_GPRS
//     // Unlock your SIM card with a PIN if needed
//     if ( GSM_PIN && modem->getSimStatus() != 3 ) {
//         modem->simUnlock(GSM_PIN);
//     }
// #endif

//     modem->sendAT("+CFUN=0 ");
//     if (modem->waitResponse(10000L) != 1) {
//         DBG(" +CFUN=0  false ");
//     }
//     delay(200);

//   // WDT_Reset(); yield();
//     /*
//       2 Automatic
//       13 GSM only
//       38 LTE only
//       51 GSM and LTE only
//     * * * */
//     String res;
//     res = modem->setNetworkMode(2);
//     if (res != "1") {
//         DBG("setNetworkMode  false ");
//         return ;
//     }
//     delay(200);

//   // WDT_Reset(); yield();
//     /*
//       1 CAT-M
//       2 NB-Iot
//       3 CAT-M and NB-IoT
//     * * */
//     res = modem->setPreferredMode(3);
//     if (res != "1") {

//         DBG("setPreferredMode  false ");
//         return ;
//     }
//     delay(200);

//   // WDT_Reset(); yield();
//     /*AT+CBANDCFG=<mode>,<band>[,<band>…]
//      * <mode> "CAT-M"   "NB-IOT"
//      * <band>  The value of <band> must is in the band list of getting from  AT+CBANDCFG=?
//      * For example, my SIM card carrier "NB-iot" supports B8.  I will configure +CBANDCFG= "Nb-iot ",8
//      */
//     /* modem->sendAT("+CBANDCFG=\"NB-IOT\",8 ");
//      if (modem->waitResponse(10000L) != 1) {
//          DBG(" +CBANDCFG=\"NB-IOT\" ");
//      }
//      delay(200);*/

//     modem->sendAT("+CFUN=1 ");
//     if (modem->waitResponse(10000L) != 1) {
//         DBG(" +CFUN=1  false ");
//     }
//     delay(200);
//   // WDT_Reset(); yield();



// }


// #endif





