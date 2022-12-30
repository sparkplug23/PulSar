
#include "mNextionPanel.h"

#ifdef USE_MODULE_DISPLAYS_NEXTION

// OTA updates for display



////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::espSetupOta()
{ // (mostly) boilerplate OTA setup from library examples

  // ArduinoOTA.setHostname(nextionNode);
  // ArduinoOTA.setPassword(configPassword);

  // ArduinoOTA.onStart([]() {
  //   //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"ESP OTA: update start"));
  //   nextionSendCmd("page 0");
  //   nextionSetAttr("p[0].b[1].txt", "\"ESP OTA Update\"");
  // });
  // ArduinoOTA.onEnd([]() {
  //   nextionSendCmd("page 0");
  //   //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"ESP OTA: update complete"));
  //   nextionSetAttr("p[0].b[1].txt", "\"ESP OTA Update\\rComplete!\"");
  //   //espReset();
  // });
  // ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  //   nextionSetAttr("p[0].b[1].txt", "\"ESP OTA Update\\rProgress: " + String(progress / (total / 100)) + "%\"");
  // });
  // ArduinoOTA.onError([](ota_error_t error) {
  //   //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"ESP OTA: ERROR code ")) + String(error));
  //   if (error == OTA_AUTH_ERROR)
  //     //debugPrintln(F("ESP OTA: ERROR - Auth Failed"));
  //   else if (error == OTA_BEGIN_ERROR)
  //     //debugPrintln(F("ESP OTA: ERROR - Begin Failed"));
  //   else if (error == OTA_CONNECT_ERROR)
  //     //debugPrintln(F("ESP OTA: ERROR - Connect Failed"));
  //   else if (error == OTA_RECEIVE_ERROR)
  //     //debugPrintln(F("ESP OTA: ERROR - Receive Failed"));
  //   else if (error == OTA_END_ERROR)
  //     //debugPrintln(F("ESP OTA: ERROR - End Failed"));
  //   nextionSetAttr("p[0].b[1].txt", "\"ESP OTA FAILED\"");
  //   delay(5000);
  //   nextionSendCmd("page " + String(settings.page));
  // });
  // ArduinoOTA.begin();
  // //debugPrintln(F("ESP OTA: Over the Air firmware update ready"));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::espStartOta(String espOtaUrl)
{ // Update ESP firmware from HTTP
  // nextionSendCmd("page 0");
  // nextionSetAttr("p[0].b[1].txt", "\"HTTP update\\rstarting...\"");
  // WiFiUDP::stopAll(); // Keep mDNS responder from breaking things

  // t_httpUpdate_return returnCode = ESPhttpUpdate.update(wifiClient, espOtaUrl);
  // switch (returnCode)
  // {
  // case HTTP_UPDATE_FAILED:
  //   //debugPrintln("ESPFW: HTTP_UPDATE_FAILED error " + String(ESPhttpUpdate.getLastError()) + " " + ESPhttpUpdate.getLastErrorString());
  //   nextionSetAttr("p[0].b[1].txt", "\"HTTP Update\\rFAILED\"");
  //   break;

  // case HTTP_UPDATE_NO_UPDATES:
  //   //debugPrintln(F("ESPFW: HTTP_UPDATE_NO_UPDATES"));
  //   nextionSetAttr("p[0].b[1].txt", "\"HTTP Update\\rNo update\"");
  //   break;

  // case HTTP_UPDATE_OK:
  //   //debugPrintln(F("ESPFW: HTTP_UPDATE_OK"));
  //   nextionSetAttr("p[0].b[1].txt", "\"HTTP Update\\rcomplete!\\r\\rRestarting.\"");
  //   espReset();
  // }
  // delay(5000);
  // nextionSendCmd("page " + String(settings.page));
  
}


////////////////////////////////////////////////////////////////////////////////////////////////////
bool mNextionPanel::updateCheck()
{ // firmware update check
  // HTTPClient updateClient;
  // //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"UPDATE: Checking update URL: ")) + String(UPDATE_URL));
  // String updatePayload;
  // updateClient.begin(wifiClient, UPDATE_URL);
  // int httpCode = updateClient.GET(); // start connection and send HTTP header

  // if (httpCode > 0)
  // { // httpCode will be negative on error
  //   if (httpCode == HTTP_CODE_OK)
  //   { // file found at server
  //     updatePayload = updateClient.getString();
  //   }
  // }
  // else
  // {
  //   //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"UPDATE: Update check failed: ")) + updateClient.errorToString(httpCode));
  //   return false;
  // }
  // updateClient.end();

  // DynamicJsonDocument updateJson(2048);
  // DeserializationError jsonError = deserializeJson(updateJson, updatePayload);

  // if (jsonError)
  // { // Couldn't parse the returned JSON, so bail
  //   //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"UPDATE: JSON parsing failed: ")) + String(jsonError.c_str()));
  //   return false;
  // }
  // else
  // {
  //   if (!updateJson["d1_mini"]["version"].isNull())
  //   {
  //     updateEspAvailableVersion = updateJson["d1_mini"]["version"].as<float>();
  //     //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"UPDATE: updateEspAvailableVersion: ")) + String(updateEspAvailableVersion));
  //     espFirmwareUrl = updateJson["d1_mini"]["firmware"].as<String>();
  //     if (updateEspAvailableVersion > nextionVersion)
  //     {
  //       updateEspAvailable = true;
  //       //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"UPDATE: New ESP version available: ")) + String(updateEspAvailableVersion));
  //     }
  //   }
  //   if (nextionModel && !updateJson[nextionModel]["version"].isNull())
  //   {
  //     updateLcdAvailableVersion = updateJson[nextionModel]["version"].as<int>();
  //     //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"UPDATE: updateLcdAvailableVersion: ")) + String(updateLcdAvailableVersion));
  //     lcdFirmwareUrl = updateJson[nextionModel]["firmware"].as<String>();
  //     if (updateLcdAvailableVersion > lcdVersion)
  //     {
  //       updateLcdAvailable = true;
  //       //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"UPDATE: New LCD version available: ")) + String(updateLcdAvailableVersion));
  //     }
  //   }
  //   //debugPrintln(F("UPDATE: Update check completed"));
  // }
  
  // return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool mNextionPanel::nextionOtaResponse()
{ // Monitor the serial port for a 0x05 response within our timeout

  unsigned long nextionCommandTimeout = 2000;   // timeout for receiving termination string in milliseconds
  unsigned long nextionCommandTimer = millis(); // record current time for our timeout
  bool otaSuccessVal = false;
  while ((millis() - nextionCommandTimer) < nextionCommandTimeout)
  {

    if(otatransfererror){
      AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "QUIT OTA and Restart display"));
      nextionReset();
      break;
    }

  
      #ifdef USE_NEXTION_SOFTWARE_SERIAL
      if (swSer->available())
      {
      byte inByte = swSer->read();
      #else 
      if (Serial.available())
      {
      byte inByte = Serial.read();
      #endif

      if (inByte == 0x5)
      {
        AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"inByte == 0x5");
        otaSuccessVal = true;
        otatransfererror = true;
        break;
      }
      else
      {
        otatransfererror = false;
AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%x"),inByte);
        // //debugPrintln(String(inByte, HEX));
      }
    }
    else
    {
      delay(1);
    }
  }
  // delay(50);
  return otaSuccessVal;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionStartOtaDownload(String otaUrl)
{ // Upload firmware to the Nextion LCD via HTTP download
  // based in large part on code posted by indev2 here:
  // http://support.iteadstudio.com/support/discussions/topics/11000007686/page/2

  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "nextionStartOtaDownload otaurl"));

  // uint32_t lcdOtaFileSize = 0;
  // String lcdOtaNextionCmd;
  // uint32_t lcdOtaChunkCounter = 0;
  // uint16_t lcdOtaPartNum = 0;
  // uint32_t lcdOtaTransferred = 0;
  // uint8_t lcdOtaPercentComplete = 0;
  // const uint32_t lcdOtaTimeout = 30000; // timeout for receiving new data in milliseconds
  // static uint32_t lcdOtaTimer = 0;      // timer for upload timeout

  // //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Attempting firmware download from: ")) + otaUrl);
  // WiFiClient lcdOtaWifi;
  // HTTPClient lcdOtaHttp;
  // lcdOtaHttp.begin(lcdOtaWifi, otaUrl);
  // int lcdOtaHttpReturn = lcdOtaHttp.GET();
  // if (lcdOtaHttpReturn > 0)
  // { // HTTP header has been sent and Server response header has been handled
  //   //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: HTTP GET return code:")) + String(lcdOtaHttpReturn));
  //   if (lcdOtaHttpReturn == HTTP_CODE_OK)
  //   {                                                 // file found at server
  //     int32_t lcdOtaRemaining = lcdOtaHttp.getSize(); // get length of document (is -1 when Server sends no Content-Length header)
  //     lcdOtaFileSize = lcdOtaRemaining;
  //     static uint16_t lcdOtaParts = (lcdOtaRemaining / 4096) + 1;
  //     static const uint16_t lcdOtaBufferSize = 1024; // upload data buffer before sending to UART
  //     static uint8_t lcdOtaBuffer[lcdOtaBufferSize] = {};

  //     //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: File found at Server. Size ")) + String(lcdOtaRemaining) + String(F(" bytes in ")) + String(lcdOtaParts) + String(F(" 4k chunks.")));

  //     WiFiUDP::stopAll(); // Keep mDNS responder and MQTT traffic from breaking things
  //     if (//mqttClient.connected())
  //     {
  //       //debugPrintln(F("LCD OTA: LCD firmware upload starting, closing MQTT connection."));
  //       //mqttClient.publish(mqttStatusTopic, WILLMESSAGE_ONDISCONNECT_CTR);
  //       //mqttClient.publish(mqttSensorTopic, "{\"status\": \"unavailable\"}");
  //       //mqttClient.disconnect();
  //     }

  //     WiFiClient *stream = lcdOtaHttp.getStreamPtr();      // get tcp stream
  //     SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix)); // Send empty command
  //     SERIAL_NEXTION_TX.flush();
  //     nextionHandleInput();
  //     String lcdOtaNextionCmd = "whmi-wri " + String(lcdOtaFileSize) + ",115200,0";
  //     //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Sending LCD upload command: ")) + lcdOtaNextionCmd);
  //     SERIAL_NEXTION_TX.print(lcdOtaNextionCmd);
  //     SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
  //     SERIAL_NEXTION_TX.flush();

  //     if (nextionOtaResponse())
  //     {
  //       //debugPrintln(F("LCD OTA: LCD upload command accepted."));
  //     }
  //     else
  //     {
  //       //debugPrintln(F("LCD OTA: LCD upload command FAILED.  Restarting device."));
  //       espReset();
  //     }
  //     //debugPrintln(F("LCD OTA: Starting update"));
  //     lcdOtaTimer = millis();
  //     while (lcdOtaHttp.connected() && (lcdOtaRemaining > 0 || lcdOtaRemaining == -1))
  //     {                                                // Write incoming data to panel as it arrives
  //       uint16_t lcdOtaHttpSize = stream->available(); // get available data size

  //       if (lcdOtaHttpSize)
  //       {
  //         uint16_t lcdOtaChunkSize = 0;
  //         if ((lcdOtaHttpSize <= lcdOtaBufferSize) && (lcdOtaHttpSize <= (4096 - lcdOtaChunkCounter)))
  //         {
  //           lcdOtaChunkSize = lcdOtaHttpSize;
  //         }
  //         else if ((lcdOtaBufferSize <= lcdOtaHttpSize) && (lcdOtaBufferSize <= (4096 - lcdOtaChunkCounter)))
  //         {
  //           lcdOtaChunkSize = lcdOtaBufferSize;
  //         }
  //         else
  //         {
  //           lcdOtaChunkSize = 4096 - lcdOtaChunkCounter;
  //         }
  //         stream->readBytes(lcdOtaBuffer, lcdOtaChunkSize);
  //         SERIAL_NEXTION_TX.flush();                              // make sure any previous writes the UART have completed
  //         SERIAL_NEXTION_TX.write(lcdOtaBuffer, lcdOtaChunkSize); // now send buffer to the UART
  //         lcdOtaChunkCounter += lcdOtaChunkSize;
  //         if (lcdOtaChunkCounter >= 4096)
  //         {
  //           SERIAL_NEXTION_TX.flush();
  //           lcdOtaPartNum++;
  //           lcdOtaTransferred += lcdOtaChunkCounter;
  //           lcdOtaPercentComplete = (lcdOtaTransferred * 100) / lcdOtaFileSize;
  //           lcdOtaChunkCounter = 0;
  //           if (nextionOtaResponse())
  //           { // We've completed a chunk
  //             //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Part ")) + String(lcdOtaPartNum) + String(F(" OK, ")) + String(lcdOtaPercentComplete) + String(F("% complete")));
  //             lcdOtaTimer = millis();
  //           }
  //           else
  //           {
  //             //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Part ")) + String(lcdOtaPartNum) + String(F(" FAILED, ")) + String(lcdOtaPercentComplete) + String(F("% complete")));
  //             //debugPrintln(F("LCD OTA: failure"));
  //             delay(2000); // extra delay while the LCD does its thing
  //             espReset();
  //           }
  //         }
  //         else
  //         {
  //           delay(20);
  //         }
  //         if (lcdOtaRemaining > 0)
  //         {
  //           lcdOtaRemaining -= lcdOtaChunkSize;
  //         }
  //       }
  //       delay(10);
  //       if ((lcdOtaTimer > 0) && ((millis() - lcdOtaTimer) > lcdOtaTimeout))
  //       { // Our timer expired so reset
  //         //debugPrintln(F("LCD OTA: ERROR: LCD upload timeout.  Restarting."));
  //         espReset();
  //       }
  //     }
  //     lcdOtaPartNum++;
  //     lcdOtaTransferred += lcdOtaChunkCounter;
  //     if ((lcdOtaTransferred == lcdOtaFileSize) && nextionOtaResponse())
  //     {
  //       //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: Success, wrote ")) + String(lcdOtaTransferred) + " of " + String(tftFileSize) + " bytes.");
  //       uint32_t lcdOtaDelay = millis();
  //       while ((millis() - lcdOtaDelay) < 5000)
  //       { // extra 5sec delay while the LCD handles any local firmware updates from new versions of code sent to it
  //         mcl->mweb->pWebServer->handleClient();
  //         delay(1);
  //       }
  //       espReset();
  //     }
  //     else
  //     {
  //       //debugPrintln(F("LCD OTA: Failure"));
  //       espReset();
  //     }
  //   }
  // }
  // else
  // {
  //   //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"LCD OTA: HTTP GET failed, error code ")) + lcdOtaHttp.errorToString(lcdOtaHttpReturn));
  //   espReset();
  // }
  // lcdOtaHttp.end();
  
}



#endif