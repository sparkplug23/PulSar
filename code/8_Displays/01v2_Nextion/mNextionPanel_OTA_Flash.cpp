

// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void nextionOtaStartDownload(const String &lcdOtaUrl)
// { // Upload firmware to the Nextion LCD via HTTP download
//   // based in large part on code posted by indev2 here:
//   // http://support.iteadstudio.com/support/discussions/topics/11000007686/page/2

//   uint32_t lcdOtaFileSize = 0;
//   String lcdOtaNextionCmd;
//   uint32_t lcdOtaChunkCounter = 0;
//   uint16_t lcdOtaPartNum = 0;
//   uint32_t lcdOtaTransferred = 0;
//   uint8_t lcdOtaPercentComplete = 0;
//   const uint32_t lcdOtaTimeout = 30000; // timeout for receiving new data in milliseconds
//   static uint32_t lcdOtaTimer = 0;      // timer for lcdOtaTimeout

//   HTTPClient lcdOtaHttp;
//   WiFiClientSecure lcdOtaWifiSecure;
//   WiFiClient lcdOtaWifi;
//   if (lcdOtaUrl.startsWith(F("https")))
//   {
//     debugPrintln("LCDOTA: Attempting firmware update from HTTPS host: " + lcdOtaUrl);

//     lcdOtaHttp.begin(lcdOtaWifiSecure, lcdOtaUrl);
//     lcdOtaWifiSecure.setInsecure();
//     lcdOtaWifiSecure.setBufferSizes(512, 512);
//   }
//   else
//   {
//     debugPrintln("LCDOTA: Attempting firmware update from HTTP host: " + lcdOtaUrl);
//     lcdOtaHttp.begin(lcdOtaWifi, lcdOtaUrl);
//   }

//   int lcdOtaHttpReturn = lcdOtaHttp.GET();
//   if (lcdOtaHttpReturn > 0)
//   { // HTTP header has been sent and Server response header has been handled
//     debugPrintln(String(F("LCDOTA: HTTP GET return code:")) + String(lcdOtaHttpReturn));
//     if (lcdOtaHttpReturn == HTTP_CODE_OK)
//     {                                                 // file found at server
//       int32_t lcdOtaRemaining = lcdOtaHttp.getSize(); // get length of document (is -1 when Server sends no Content-Length header)
//       lcdOtaFileSize = lcdOtaRemaining;
//       static uint16_t lcdOtaParts = (lcdOtaRemaining / 4096) + 1;
//       static const uint16_t lcdOtaBufferSize = 1024; // upload data buffer before sending to UART
//       static uint8_t lcdOtaBuffer[lcdOtaBufferSize] = {};

//       debugPrintln(String(F("LCDOTA: File found at Server. Size ")) + String(lcdOtaRemaining) + String(F(" bytes in ")) + String(lcdOtaParts) + String(F(" 4k chunks.")));

//       WiFiUDP::stopAll(); // Keep mDNS responder and MQTT traffic from breaking things
//       if (mqttClient.connected())
//       {
//         debugPrintln(F("LCDOTA: LCD firmware upload starting, closing MQTT connection."));
//         mqttClient.publish(mqttStatusTopic, "OFF", true, 0);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttStatusTopic + String(F("' : 'OFF'")));
//         mqttClient.disconnect();
//       }

//       WiFiClient *stream = lcdOtaHttp.getStreamPtr();      // get tcp stream
//       Serial1.write(nextionSuffix, sizeof(nextionSuffix)); // Send empty command
//       Serial1.flush();
//       nextionHandleInput();
//       String lcdOtaNextionCmd = "whmi-wri " + String(lcdOtaFileSize) + "," + String(nextionBaud) + ",0";
//       debugPrintln(String(F("LCDOTA: Sending LCD upload command: ")) + lcdOtaNextionCmd);
//       Serial1.print(lcdOtaNextionCmd);
//       Serial1.write(nextionSuffix, sizeof(nextionSuffix));
//       Serial1.flush();

//       if (nextionOtaResponse())
//       {
//         debugPrintln(F("LCDOTA: LCD upload command accepted."));
//       }
//       else
//       {
//         debugPrintln(F("LCDOTA: LCD upload command FAILED.  Restarting device."));
//         espReset();
//       }
//       debugPrintln(F("LCDOTA: Starting update"));
//       lcdOtaTimer = millis();
//       while (lcdOtaHttp.connected() && (lcdOtaRemaining > 0 || lcdOtaRemaining == -1))
//       {                                                // Write incoming data to panel as it arrives
//         uint16_t lcdOtaHttpSize = stream->available(); // get available data size

//         if (lcdOtaHttpSize)
//         {
//           uint16_t lcdOtaChunkSize = 0;
//           if ((lcdOtaHttpSize <= lcdOtaBufferSize) && (lcdOtaHttpSize <= (4096 - lcdOtaChunkCounter)))
//           {
//             lcdOtaChunkSize = lcdOtaHttpSize;
//           }
//           else if ((lcdOtaBufferSize <= lcdOtaHttpSize) && (lcdOtaBufferSize <= (4096 - lcdOtaChunkCounter)))
//           {
//             lcdOtaChunkSize = lcdOtaBufferSize;
//           }
//           else
//           {
//             lcdOtaChunkSize = 4096 - lcdOtaChunkCounter;
//           }
//           stream->readBytes(lcdOtaBuffer, lcdOtaChunkSize);
//           Serial1.flush();                              // make sure any previous writes the UART have completed
//           Serial1.write(lcdOtaBuffer, lcdOtaChunkSize); // now send buffer to the UART
//           lcdOtaChunkCounter += lcdOtaChunkSize;
//           if (lcdOtaChunkCounter >= 4096)
//           {
//             Serial1.flush();
//             lcdOtaPartNum++;
//             lcdOtaTransferred += lcdOtaChunkCounter;
//             lcdOtaPercentComplete = (lcdOtaTransferred * 100) / lcdOtaFileSize;
//             lcdOtaChunkCounter = 0;
//             if (nextionOtaResponse())
//             { // We've completed a chunk
//               debugPrintln(String(F("LCDOTA: Part ")) + String(lcdOtaPartNum) + String(F(" OK, ")) + String(lcdOtaPercentComplete) + String(F("% complete")));
//               lcdOtaTimer = millis();
//             }
//             else
//             {
//               debugPrintln(String(F("LCDOTA: Part ")) + String(lcdOtaPartNum) + String(F(" FAILED, ")) + String(lcdOtaPercentComplete) + String(F("% complete")));
//               debugPrintln(F("LCDOTA: failure"));
//               delay(2000); // extra delay while the LCD does its thing
//               espReset();
//             }
//           }
//           else
//           {
//             delay(20);
//           }
//           if (lcdOtaRemaining > 0)
//           {
//             lcdOtaRemaining -= lcdOtaChunkSize;
//           }
//         }
//         delay(10);
//         if ((lcdOtaTimer > 0) && ((millis() - lcdOtaTimer) > lcdOtaTimeout))
//         { // Our timer expired so reset
//           debugPrintln(F("LCDOTA: ERROR: LCD upload timeout.  Restarting."));
//           espReset();
//         }
//       }
//       lcdOtaPartNum++;
//       lcdOtaTransferred += lcdOtaChunkCounter;
//       if ((lcdOtaTransferred == lcdOtaFileSize) && nextionOtaResponse())
//       {
//         debugPrintln(String(F("LCDOTA: Success, wrote ")) + String(lcdOtaTransferred) + String(F(" of ")) + String(tftFileSize) + String(F(" bytes.")));
//         uint32_t lcdOtaDelay = millis();
//         debugPrintln(F("LCDOTA: Waiting 5 seconds to allow LCD to apply updates we've sent."));
//         while ((millis() - lcdOtaDelay) < 5000)
//         { // extra 5sec delay while the LCD handles any local firmware updates from new versions of code sent to it
//           webServer.handleClient();
//           yield();
//         }
//         espReset();
//       }
//       else
//       {
//         debugPrintln(String(F("LCDOTA: Failure, lcdOtaTransferred: ")) + String(lcdOtaTransferred) + String(F(" lcdOtaFileSize: ")) + String(lcdOtaFileSize));
//         espReset();
//       }
//     }
//   }
//   else
//   {
//     debugPrintln(String(F("LCDOTA: HTTP GET failed, error code ")) + lcdOtaHttp.errorToString(lcdOtaHttpReturn));
//     espReset();
//   }
//   lcdOtaHttp.end();
// }

// ////////////////////////////////////////////////////////////////////////////////////////////////////
// bool nextionOtaResponse()
// {                                               // Monitor the serial port for a 0x05 response within our timeout
//   unsigned long nextionCommandTimeout = 2000;   // timeout for receiving termination string in milliseconds
//   unsigned long nextionCommandTimer = millis(); // record current time for our timeout
//   bool otaSuccessVal = false;
//   while ((millis() - nextionCommandTimer) < nextionCommandTimeout)
//   {
//     if (Serial.available())
//     {
//       byte inByte = Serial.read();
//       if (inByte == 0x5)
//       {
//         otaSuccessVal = true;
//         break;
//       }
//     }
//     else
//     {
//       delay(1);
//     }
//   }
//   return otaSuccessVal;
// }

// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void nextionUpdateProgress(const unsigned int &progress, const unsigned int &total)
// {
//   uint8_t progressPercent = (float(progress) / float(total)) * 100;
//   nextionSetAttr("p[0].b[4].val", String(progressPercent));
// }



// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void espStartOta(const String &espOtaUrl)
// { // Update ESP firmware from HTTP/HTTPS URL

//   nextionSetAttr("p[0].b[1].txt", "\"\\rHASPone update:\\r\\r\\r \"");
//   nextionSendCmd("page 0");
//   nextionSendCmd("vis 4,1");

//   WiFiUDP::stopAll(); // Keep mDNS responder from breaking things
//   delay(1);
//   ESPhttpUpdate.rebootOnUpdate(false);
//   ESPhttpUpdate.onProgress(nextionUpdateProgress);
//   t_httpUpdate_return espOtaUrlReturnCode;
//   if (espOtaUrl.startsWith(F("https")))
//   {
//     debugPrintln(String(F("ESPFW: Attempting firmware update from HTTPS host: ")) + espOtaUrl);
//     WiFiClientSecure wifiEspOtaClientSecure;
//     wifiEspOtaClientSecure.setInsecure();
//     wifiEspOtaClientSecure.setBufferSizes(512, 512);
//     espOtaUrlReturnCode = ESPhttpUpdate.update(wifiEspOtaClientSecure, espOtaUrl);
//   }
//   else
//   {
//     debugPrintln(String(F("ESPFW: Attempting firmware update from HTTP host: ")) + espOtaUrl);
//     espOtaUrlReturnCode = ESPhttpUpdate.update(wifiClient, espOtaUrl);
//   }

//   switch (espOtaUrlReturnCode)
//   {
//   case HTTP_UPDATE_FAILED:
//     debugPrintln(String(F("ESPFW: HTTP_UPDATE_FAILED error ")) + String(ESPhttpUpdate.getLastError()) + " " + ESPhttpUpdate.getLastErrorString());
//     nextionSendCmd("vis 4,0");
//     nextionSetAttr("p[0].b[1].txt", "\"HASPone update:\\r FAILED\\rerror: " + ESPhttpUpdate.getLastErrorString() + "\"");
//     break;

//   case HTTP_UPDATE_NO_UPDATES:
//     debugPrintln(F("ESPFW: HTTP_UPDATE_NO_UPDATES"));
//     nextionSendCmd("vis 4,0");
//     nextionSetAttr("p[0].b[1].txt", "\"HASPone update:\\rNo update\"");
//     break;

//   case HTTP_UPDATE_OK:
//     debugPrintln(F("ESPFW: HTTP_UPDATE_OK"));
//     nextionSetAttr("p[0].b[1].txt", "\"\\rHASPone update:\\r\\r Complete!\\rRestarting.\"");
//     nextionSendCmd("vis 4,1");
//     delay(1000);
//     espReset();
//   }
//   delay(1000);
//   nextionSendCmd("page " + String(nextionActivePage));
// }


// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void webHandleFirmware()
// { // http://plate01/firmware
//   if (configPassword[0] != '\0')
//   { //Request HTTP auth if configPassword is set
//     if (!webServer.authenticate(configUser, configPassword))
//     {
//       return webServer.requestAuthentication();
//     }
//   }
//   debugPrintln(String(F("HTTP: Sending /firmware page to client connected from: ")) + webServer.client().remoteIP().toString());
//   String httpHeader = FPSTR(HTTP_HEAD_START);
//   httpHeader.replace("{v}", "HASPone " + String(haspNode) + " Firmware updates");
//   webServer.setContentLength(CONTENT_LENGTH_UNKNOWN);
//   webServer.send(200, "text/html", httpHeader);
//   webServer.sendContent_P(HTTP_SCRIPT);
//   webServer.sendContent_P(HTTP_STYLE);
//   webServer.sendContent_P(HASP_STYLE);
//   webServer.sendContent_P(HTTP_HEAD_END);

//   webServer.sendContent(F("<h1>"));
//   webServer.sendContent(haspNode);
//   webServer.sendContent(F(" Firmware updates</h1><b>Note:</b> If updating firmware for both the ESP8266 and the Nextion LCD, you'll want to update the ESP8266 first followed by the Nextion LCD<br/><hr/>"));

//   // Display main firmware page
//   webServer.sendContent(F("<form method='get' action='/espfirmware'>"));
//   if (updateEspAvailable)
//   {
//     webServer.sendContent(F("<font color='green'><b>HASPone ESP8266 update available!</b></font>"));
//   }
//   webServer.sendContent(F("<br/><b>Update ESP8266 from URL</b>"));
//   webServer.sendContent(F("<br/><input id='espFirmwareURL' name='espFirmware' value='"));
//   webServer.sendContent(espFirmwareUrl);
//   webServer.sendContent(F("'><br/><br/><button type='submit'>Update ESP from URL</button></form>"));

//   webServer.sendContent(F("<br/><form method='POST' action='/update' enctype='multipart/form-data'>"));
//   webServer.sendContent(F("<b>Update ESP8266 from file</b><input type='file' id='espSelect' name='espSelect' accept='.bin'>"));
//   webServer.sendContent(F("<br/><br/><button type='submit' id='espUploadSubmit' onclick='ackEspUploadSubmit()'>Update ESP from file</button></form>"));

//   webServer.sendContent(F("<br/><br/><hr><h1>WARNING!</h1>"));
//   webServer.sendContent(F("<b>Nextion LCD firmware updates can be risky.</b> If interrupted, the LCD will display an error message until a successful firmware update has completed. "));
//   webServer.sendContent(F("<br/><br/><i>Note: Failed LCD firmware updates on HASPone hardware prior to v1.0 may require a hard power cycle of the device, via a circuit breaker or by physically disconnecting the device.</i>"));

//   webServer.sendContent(F("<br/><hr><form method='get' action='lcddownload'>"));
//   if (updateLcdAvailable)
//   {
//     webServer.sendContent(F("<font color='green'><b>HASPone LCD update available!</b></font>"));
//   }
//   webServer.sendContent(F("<br/><b>Update Nextion LCD from URL</b><small><i> http only</i></small>"));
//   webServer.sendContent(F("<br/><input id='lcdFirmware' name='lcdFirmware' value='"));
//   webServer.sendContent(lcdFirmwareUrl);
//   webServer.sendContent(F("'><br/><br/><button type='submit'>Update LCD from URL</button></form>"));

//   webServer.sendContent(F("<br/><form method='POST' action='/lcdupload' enctype='multipart/form-data'>"));
//   webServer.sendContent(F("<br/><b>Update Nextion LCD from file</b><input type='file' id='lcdSelect' name='files[]' accept='.tft'/>"));
//   webServer.sendContent(F("<br/><br/><button type='submit' id='lcdUploadSubmit' onclick='ackLcdUploadSubmit()'>Update LCD from file</button></form>"));

//   // Javascript to collect the filesize of the LCD upload and send it to /tftFileSize
//   webServer.sendContent(F("<script>function handleLcdFileSelect(evt) {"));
//   webServer.sendContent(F("var uploadFile = evt.target.files[0];"));
//   webServer.sendContent(F("document.getElementById('lcdUploadSubmit').innerHTML = 'Upload LCD firmware ' + uploadFile.name;"));
//   webServer.sendContent(F("var tftFileSize = '/tftFileSize?tftFileSize=' + uploadFile.size;"));
//   webServer.sendContent(F("var xhttp = new XMLHttpRequest();xhttp.open('GET', tftFileSize, true);xhttp.send();}"));
//   webServer.sendContent(F("function ackLcdUploadSubmit() {document.getElementById('lcdUploadSubmit').innerHTML = 'Uploading LCD firmware...';}"));
//   webServer.sendContent(F("function handleEspFileSelect(evt) {var uploadFile = evt.target.files[0];document.getElementById('espUploadSubmit').innerHTML = 'Upload ESP firmware ' + uploadFile.name;}"));
//   webServer.sendContent(F("function ackEspUploadSubmit() {document.getElementById('espUploadSubmit').innerHTML = 'Uploading ESP firmware...';}"));
//   webServer.sendContent(F("document.getElementById('lcdSelect').addEventListener('change', handleLcdFileSelect, false);"));
//   webServer.sendContent(F("document.getElementById('espSelect').addEventListener('change', handleEspFileSelect, false);</script>"));

//   webServer.sendContent_P(HTTP_END);
//   webServer.sendContent("");
// }


// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void webHandleLcdUpload()
// { // http://plate01/lcdupload
//   // Upload firmware to the Nextion LCD via HTTP upload

//   if (configPassword[0] != '\0')
//   { //Request HTTP auth if configPassword is set
//     if (!webServer.authenticate(configUser, configPassword))
//     {
//       return webServer.requestAuthentication();
//     }
//   }

//   static uint32_t lcdOtaTransferred = 0;
//   static uint32_t lcdOtaRemaining;
//   static uint16_t lcdOtaParts;
//   const uint32_t lcdOtaTimeout = 30000; // timeout for receiving new data in milliseconds
//   static uint32_t lcdOtaTimer = 0;      // timer for upload timeout

//   HTTPUpload &upload = webServer.upload();

//   if (tftFileSize == 0)
//   {
//     debugPrintln(String(F("LCDOTA: FAILED, no filesize sent.")));
//     String httpHeader = FPSTR(HTTP_HEAD_START);
//     httpHeader.replace("{v}", "HASPone " + String(haspNode) + " LCD update error");
//     webServer.setContentLength(CONTENT_LENGTH_UNKNOWN);
//     webServer.send(200, "text/html", httpHeader);
//     webServer.sendContent_P(HTTP_SCRIPT);
//     webServer.sendContent_P(HTTP_STYLE);
//     webServer.sendContent_P(HASP_STYLE);
//     webServer.sendContent(F("<meta http-equiv='refresh' content='5;url=/firmware' />"));
//     webServer.sendContent_P(HTTP_HEAD_END);
//     webServer.sendContent(F("<h1>"));
//     webServer.sendContent(haspNode);
//     webServer.sendContent(F(" LCD update FAILED</h1>"));
//     webServer.sendContent(F("No update file size reported.  You must use a modern browser with Javascript enabled."));
//     webServer.sendContent_P(HTTP_END);
//     webServer.sendContent("");
//   }
//   else if ((lcdOtaTimer > 0) && ((millis() - lcdOtaTimer) > lcdOtaTimeout))
//   { // Our timer expired so reset
//     debugPrintln(F("LCDOTA: ERROR: LCD upload timeout.  Restarting."));
//     espReset();
//   }
//   else if (upload.status == UPLOAD_FILE_START)
//   {
//     WiFiUDP::stopAll(); // Keep mDNS responder from breaking things

//     debugPrintln(String(F("LCDOTA: Attempting firmware upload")));
//     debugPrintln(String(F("LCDOTA: upload.filename: ")) + String(upload.filename));
//     debugPrintln(String(F("LCDOTA: TFTfileSize: ")) + String(tftFileSize));

//     lcdOtaRemaining = tftFileSize;
//     lcdOtaParts = (lcdOtaRemaining / 4096) + 1;
//     debugPrintln(String(F("LCDOTA: File upload beginning. Size ")) + String(lcdOtaRemaining) + String(F(" bytes in ")) + String(lcdOtaParts) + String(F(" 4k chunks.")));

//     Serial1.write(nextionSuffix, sizeof(nextionSuffix)); // Send empty command to LCD
//     Serial1.flush();
//     nextionHandleInput();

//     String lcdOtaNextionCmd = "whmi-wri " + String(tftFileSize) + "," + String(nextionBaud) + ",0";
//     debugPrintln(String(F("LCDOTA: Sending LCD upload command: ")) + lcdOtaNextionCmd);
//     Serial1.print(lcdOtaNextionCmd);
//     Serial1.write(nextionSuffix, sizeof(nextionSuffix));
//     Serial1.flush();

//     if (nextionOtaResponse())
//     {
//       debugPrintln(F("LCDOTA: LCD upload command accepted"));
//     }
//     else
//     {
//       debugPrintln(F("LCDOTA: LCD upload command FAILED."));
//       espReset();
//     }
//     lcdOtaTimer = millis();
//   }
//   else if (upload.status == UPLOAD_FILE_WRITE)
//   { // Handle upload data
//     static int lcdOtaChunkCounter = 0;
//     static uint16_t lcdOtaPartNum = 0;
//     static int lcdOtaPercentComplete = 0;
//     static const uint16_t lcdOtaBufferSize = 1024; // upload data buffer before sending to UART
//     static uint8_t lcdOtaBuffer[lcdOtaBufferSize] = {};
//     uint16_t lcdOtaUploadIndex = 0;
//     int32_t lcdOtaPacketRemaining = upload.currentSize;

//     while (lcdOtaPacketRemaining > 0)
//     { // Write incoming data to panel as it arrives
//       // determine chunk size as lowest value of lcdOtaPacketRemaining, lcdOtaBufferSize, or 4096 - lcdOtaChunkCounter
//       uint16_t lcdOtaChunkSize = 0;
//       if ((lcdOtaPacketRemaining <= lcdOtaBufferSize) && (lcdOtaPacketRemaining <= (4096 - lcdOtaChunkCounter)))
//       {
//         lcdOtaChunkSize = lcdOtaPacketRemaining;
//       }
//       else if ((lcdOtaBufferSize <= lcdOtaPacketRemaining) && (lcdOtaBufferSize <= (4096 - lcdOtaChunkCounter)))
//       {
//         lcdOtaChunkSize = lcdOtaBufferSize;
//       }
//       else
//       {
//         lcdOtaChunkSize = 4096 - lcdOtaChunkCounter;
//       }

//       for (uint16_t i = 0; i < lcdOtaChunkSize; i++)
//       { // Load up the UART buffer
//         lcdOtaBuffer[i] = upload.buf[lcdOtaUploadIndex];
//         lcdOtaUploadIndex++;
//       }
//       Serial1.flush();                              // Clear out current UART buffer
//       Serial1.write(lcdOtaBuffer, lcdOtaChunkSize); // And send the most recent data
//       lcdOtaChunkCounter += lcdOtaChunkSize;
//       lcdOtaTransferred += lcdOtaChunkSize;
//       if (lcdOtaChunkCounter >= 4096)
//       {
//         Serial1.flush();
//         lcdOtaPartNum++;
//         lcdOtaPercentComplete = (lcdOtaTransferred * 100) / tftFileSize;
//         lcdOtaChunkCounter = 0;
//         if (nextionOtaResponse())
//         {
//           debugPrintln(String(F("LCDOTA: Part ")) + String(lcdOtaPartNum) + String(F(" OK, ")) + String(lcdOtaPercentComplete) + String(F("% complete")));
//         }
//         else
//         {
//           debugPrintln(String(F("LCDOTA: Part ")) + String(lcdOtaPartNum) + String(F(" FAILED, ")) + String(lcdOtaPercentComplete) + String(F("% complete")));
//         }
//       }
//       else
//       {
//         delay(10);
//       }
//       if (lcdOtaRemaining > 0)
//       {
//         lcdOtaRemaining -= lcdOtaChunkSize;
//       }
//       if (lcdOtaPacketRemaining > 0)
//       {
//         lcdOtaPacketRemaining -= lcdOtaChunkSize;
//       }
//     }

//     if (lcdOtaTransferred >= tftFileSize)
//     {
//       if (nextionOtaResponse())
//       {
//         debugPrintln(String(F("LCDOTA: Success, wrote ")) + String(lcdOtaTransferred) + " of " + String(tftFileSize) + " bytes.");
//         webServer.sendHeader("Location", "/lcdOtaSuccess");
//         webServer.send(303);
//         uint32_t lcdOtaDelay = millis();
//         while ((millis() - lcdOtaDelay) < 5000)
//         { // extra 5sec delay while the LCD handles any local firmware updates from new versions of code sent to it
//           webServer.handleClient();
//           delay(1);
//         }
//         espReset();
//       }
//       else
//       {
//         debugPrintln(F("LCDOTA: Failure"));
//         webServer.sendHeader("Location", "/lcdOtaFailure");
//         webServer.send(303);
//         uint32_t lcdOtaDelay = millis();
//         while ((millis() - lcdOtaDelay) < 1000)
//         { // extra 1sec delay for client to grab failure page
//           webServer.handleClient();
//           delay(1);
//         }
//         espReset();
//       }
//     }
//     lcdOtaTimer = millis();
//   }
//   else if (upload.status == UPLOAD_FILE_END)
//   { // Upload completed
//     if (lcdOtaTransferred >= tftFileSize)
//     {
//       if (nextionOtaResponse())
//       { // YAY WE DID IT
//         debugPrintln(String(F("LCDOTA: Success, wrote ")) + String(lcdOtaTransferred) + " of " + String(tftFileSize) + " bytes.");
//         webServer.sendHeader("Location", "/lcdOtaSuccess");
//         webServer.send(303);
//         uint32_t lcdOtaDelay = millis();
//         while ((millis() - lcdOtaDelay) < 5000)
//         { // extra 5sec delay while the LCD handles any local firmware updates from new versions of code sent to it
//           webServer.handleClient();
//           yield();
//         }
//         espReset();
//       }
//       else
//       {
//         debugPrintln(F("LCDOTA: Failure"));
//         webServer.sendHeader("Location", "/lcdOtaFailure");
//         webServer.send(303);
//         uint32_t lcdOtaDelay = millis();
//         while ((millis() - lcdOtaDelay) < 1000)
//         { // extra 1sec delay for client to grab failure page
//           webServer.handleClient();
//           yield();
//         }
//         espReset();
//       }
//     }
//   }
//   else if (upload.status == UPLOAD_FILE_ABORTED)
//   { // Something went kablooey
//     debugPrintln(F("LCDOTA: ERROR: upload.status returned: UPLOAD_FILE_ABORTED"));
//     debugPrintln(F("LCDOTA: Failure"));
//     webServer.sendHeader("Location", "/lcdOtaFailure");
//     webServer.send(303);
//     uint32_t lcdOtaDelay = millis();
//     while ((millis() - lcdOtaDelay) < 1000)
//     { // extra 1sec delay for client to grab failure page
//       webServer.handleClient();
//       yield();
//     }
//     espReset();
//   }
//   else
//   { // Something went weird, we should never get here...
//     debugPrintln(String(F("LCDOTA: upload.status returned: ")) + String(upload.status));
//     debugPrintln(F("LCDOTA: Failure"));
//     webServer.sendHeader("Location", "/lcdOtaFailure");
//     webServer.send(303);
//     uint32_t lcdOtaDelay = millis();
//     while ((millis() - lcdOtaDelay) < 1000)
//     { // extra 1sec delay for client to grab failure page
//       webServer.handleClient();
//       yield();
//     }
//     espReset();
//   }
// }

// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void webHandleLcdUpdateSuccess()
// { // http://plate01/lcdOtaSuccess
//   if (configPassword[0] != '\0')
//   { //Request HTTP auth if configPassword is set
//     if (!webServer.authenticate(configUser, configPassword))
//     {
//       return webServer.requestAuthentication();
//     }
//   }
//   debugPrintln(String(F("HTTP: Sending /lcdOtaSuccess page to client connected from: ")) + webServer.client().remoteIP().toString());
//   String httpHeader = FPSTR(HTTP_HEAD_START);
//   httpHeader.replace("{v}", "HASPone " + String(haspNode) + " LCD firmware update success");
//   webServer.setContentLength(CONTENT_LENGTH_UNKNOWN);
//   webServer.send(200, "text/html", httpHeader);
//   webServer.sendContent_P(HTTP_SCRIPT);
//   webServer.sendContent_P(HTTP_STYLE);
//   webServer.sendContent_P(HASP_STYLE);
//   webServer.sendContent(F("<meta http-equiv='refresh' content='15;url=/' />"));
//   webServer.sendContent_P(HTTP_HEAD_END);
//   webServer.sendContent(F("<h1>"));
//   webServer.sendContent(haspNode);
//   webServer.sendContent(F(" LCD update success</h1>"));
//   webServer.sendContent(F("Restarting HASwitchPlate to apply changes..."));
//   webServer.sendContent_P(HTTP_END);
//   webServer.sendContent("");
// }

// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void webHandleLcdUpdateFailure()
// { // http://plate01/lcdOtaFailure
//   if (configPassword[0] != '\0')
//   { //Request HTTP auth if configPassword is set
//     if (!webServer.authenticate(configUser, configPassword))
//     {
//       return webServer.requestAuthentication();
//     }
//   }
//   debugPrintln(String(F("HTTP: Sending /lcdOtaFailure page to client connected from: ")) + webServer.client().remoteIP().toString());
//   String httpHeader = FPSTR(HTTP_HEAD_START);
//   httpHeader.replace("{v}", "HASPone " + String(haspNode) + " LCD firmware update failed");
//   webServer.setContentLength(CONTENT_LENGTH_UNKNOWN);
//   webServer.send(200, "text/html", httpHeader);
//   webServer.sendContent_P(HTTP_SCRIPT);
//   webServer.sendContent_P(HTTP_STYLE);
//   webServer.sendContent_P(HASP_STYLE);
//   webServer.sendContent(F("<meta http-equiv='refresh' content='15;url=/' />"));
//   webServer.sendContent_P(HTTP_HEAD_END);
//   webServer.sendContent(F("<h1>"));
//   webServer.sendContent(haspNode);
//   webServer.sendContent(F(" LCD update failed :(</h1>"));
//   webServer.sendContent(F("Restarting HASwitchPlate to apply changes..."));
//   webServer.sendContent_P(HTTP_END);
//   webServer.sendContent("");
// }

// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void webHandleLcdDownload()
// { // http://plate01/lcddownload
//   if (configPassword[0] != '\0')
//   { //Request HTTP auth if configPassword is set
//     if (!webServer.authenticate(configUser, configPassword))
//     {
//       return webServer.requestAuthentication();
//     }
//   }
//   debugPrintln(String(F("HTTP: Sending /lcddownload page to client connected from: ")) + webServer.client().remoteIP().toString());
//   String httpHeader = FPSTR(HTTP_HEAD_START);
//   httpHeader.replace("{v}", "HASPone " + String(haspNode) + " LCD firmware update");
//   webServer.setContentLength(CONTENT_LENGTH_UNKNOWN);
//   webServer.send(200, "text/html", httpHeader);
//   webServer.sendContent_P(HTTP_SCRIPT);
//   webServer.sendContent_P(HTTP_STYLE);
//   webServer.sendContent_P(HASP_STYLE);
//   webServer.sendContent_P(HTTP_HEAD_END);
//   webServer.sendContent(F("<h1>"));
//   webServer.sendContent(haspNode);
//   webServer.sendContent(F(" LCD update</h1>"));
//   webServer.sendContent(F("<br/>Updating LCD firmware from: "));
//   webServer.sendContent(webServer.arg("lcdFirmware"));
//   webServer.sendContent_P(HTTP_END);
//   webServer.sendContent("");
//   nextionOtaStartDownload(webServer.arg("lcdFirmware"));
// }

// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void webHandleTftFileSize()
// { // http://plate01/tftFileSize
//   if (configPassword[0] != '\0')
//   { //Request HTTP auth if configPassword is set
//     if (!webServer.authenticate(configUser, configPassword))
//     {
//       return webServer.requestAuthentication();
//     }
//   }
//   debugPrintln(String(F("HTTP: Sending /tftFileSize page to client connected from: ")) + webServer.client().remoteIP().toString());
//   String httpHeader = FPSTR(HTTP_HEAD_START);
//   httpHeader.replace("{v}", "HASPone " + String(haspNode) + " TFT Filesize");
//   webServer.setContentLength(CONTENT_LENGTH_UNKNOWN);
//   webServer.send(200, "text/html", httpHeader);
//   webServer.sendContent_P(HTTP_HEAD_END);
//   tftFileSize = webServer.arg("tftFileSize").toInt();
//   debugPrintln(String(F("WEB: Received tftFileSize: ")) + String(tftFileSize));
// }