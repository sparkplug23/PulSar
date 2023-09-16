/**
 * @file mNextionPanel.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-09
 * 
 * @copyright Copyright (c) 2022
 * 
 * @note Initially based on oneHASP but should become its own code in due time
 *       Latest code revision with oneHASP is version 1.03 on 9Aug22

 * @desc To enable control of Nextion panels using mqtt, allowing pass through commands and improved automation (eg dimmer, regexp of colours)
 */
#include "mNextionPanel.h"

#ifdef USE_MODULE_DISPLAYS_NEXTION


#ifdef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER

#define webServer pCONT_web->server

#endif


#ifndef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER

#ifndef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleNotFound()
{ // webServer 404
  ALOG_INF(PSTR(D_LOG_NEXTION "HTTP: Sending 404 to client connected from: %s"), webServer->client().remoteIP().toString());
  String httpHeader = FPSTR(HTTP_HEAD_START);//FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", "HASPone " + String(haspNode) + " 404");
  webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  webServer->send(404, "text/html", httpHeader);
  webServer->sendContent_P(HTTP_SCRIPT);
  webServer->sendContent_P(HTTP_STYLE);
  webServer->sendContent_P(HASP_STYLE);
  webServer->sendContent_P(HTTP_HEAD_END);
  webServer->sendContent(F("<h1>404: File Not Found</h1>One of us appears to have done something horribly wrong-><hr/><b>URI: </b>"));
  webServer->sendContent(webServer->uri());
  webServer->sendContent(F("<br/><b>Method: </b>"));
  webServer->sendContent((webServer->method() == HTTP_GET) ? F("GET") : F("POST"));
  webServer->sendContent(F("<br/><b>Arguments: </b>"));
  webServer->sendContent(String(webServer->args()));
  for (uint8_t i = 0; i < webServer->args(); i++)
  {
    webServer->sendContent(F("<br/><b>"));
    webServer->sendContent(String(webServer->argName(i)));
    webServer->sendContent(F(":</b> "));
    webServer->sendContent(String(webServer->arg(i)));
  }
  webServer->sendContent("");
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleRoot()
{ 

  ALOG_INF(PSTR(D_LOG_NEXTION DEBUG_INSERT_PAGE_BREAK "HTTP: Sending root page to client connected from: %s"), webServer->client().remoteIP().toString());

  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", "HASPone " + String(haspNode));
  webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  webServer->send(200, "text/html", httpHeader);
  webServer->sendContent(httpHeader);
  webServer->sendContent_P(HTTP_SCRIPT);
  webServer->sendContent_P(HTTP_STYLE);
  webServer->sendContent_P(HASP_STYLE);
  webServer->sendContent_P(HTTP_HEAD_END);

  webServer->sendContent(F("<br/><hr><button type='submit'>save settings</button></form>"));

  webServer->sendContent(F("<hr><form method='get' action='firmware'>"));
  webServer->sendContent(F("<button type='submit'>update firmware</button></form>"));

  webServer->sendContent(F("<hr><form method='get' action='reboot'>"));
  webServer->sendContent(F("<button type='submit'>reboot device</button></form>"));

  webServer->sendContent(F("<hr><form method='get' action='resetConfig'>"));
  webServer->sendContent(F("<button type='submit'>factory reset settings</button></form>"));

  webServer->sendContent_P(HTTP_END);
  webServer->sendContent("");

}

#endif // ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER




////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionOtaStartDownload(const String &lcdOtaUrl)
{ // Upload firmware to the Nextion LCD via HTTP download
  // based in large part on code posted by indev2 here:
  // http://support.iteadstudio.com/support/discussions/topics/11000007686/page/2

  uint32_t lcdOtaFileSize = 0;
  String lcdOtaNextionCmd;
  uint32_t lcdOtaChunkCounter = 0;
  uint16_t lcdOtaPartNum = 0;
  uint32_t lcdOtaTransferred = 0;
  uint8_t lcdOtaPercentComplete = 0;
  const uint32_t lcdOtaTimeout = 30000; // timeout for receiving new data in milliseconds
  static uint32_t lcdOtaTimer = 0;      // timer for lcdOtaTimeout

  HTTPClient lcdOtaHttp;
  WiFiClientSecure lcdOtaWifiSecure;
  WiFiClient lcdOtaWifi;
  if (lcdOtaUrl.startsWith(F("https")))
  {
    debugPrintln("LCDOTA: Attempting firmware update from HTTPS host: " + lcdOtaUrl);

    lcdOtaHttp.begin(lcdOtaWifiSecure, lcdOtaUrl);
    lcdOtaWifiSecure.setInsecure();
    // lcdOtaWifiSecure.setBufferSizes(512, 512);
  }
  else
  {
    debugPrintln("LCDOTA: Attempting firmware update from HTTP host: " + lcdOtaUrl);
    lcdOtaHttp.begin(lcdOtaWifi, lcdOtaUrl);
  }

  int lcdOtaHttpReturn = lcdOtaHttp.GET();
  if (lcdOtaHttpReturn > 0)
  { // HTTP header has been sent and Server response header has been handled
    ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: HTTP GET return code: %d"), lcdOtaHttpReturn);
    if (lcdOtaHttpReturn == HTTP_CODE_OK)
    {                                                 // file found at server
      int32_t lcdOtaRemaining = lcdOtaHttp.getSize(); // get length of document (is -1 when Server sends no Content-Length header)
      lcdOtaFileSize = lcdOtaRemaining;
      static uint16_t lcdOtaParts = (lcdOtaRemaining / 4096) + 1;
      static const uint16_t lcdOtaBufferSize = 1024; // upload data buffer before sending to UART
      static uint8_t lcdOtaBuffer[lcdOtaBufferSize] = {};

      ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: File found at Server. Size %d bytes in %d 4k chunks"), lcdOtaRemaining, lcdOtaParts);

      // WiFiUDP::stop(); // Keep mDNS responder and MQTT traffic from breaking things
      // if (mqttClient.connected())
      // {
      //   ALOG_INF(PSTR("LCDOTA: LCD firmware upload starting, closing MQTT connection."));
      //   mqttClient.publish(mqttStatusTopic, "OFF", true, 0);
      //   debugPrintln(String(F("MQTT OUT: '")) + mqttStatusTopic + String(F("' : 'OFF'")));
      //   mqttClient.disconnect();
      // }

      WiFiClient *stream = lcdOtaHttp.getStreamPtr();      // get tcp stream
      Serial2.write(nextionSuffix, sizeof(nextionSuffix)); // Send empty command
      Serial2.flush();
      nextionHandleInput();

      String lcdOtaNextionCmd = "whmi-wri " + String(lcdOtaFileSize) + "," + String(nextionBaud) + ",0";
      ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Sending LCD upload command: %s"), lcdOtaNextionCmd);

      Serial2.print(lcdOtaNextionCmd);
      Serial2.write(nextionSuffix, sizeof(nextionSuffix));
      Serial2.flush();

      if (nextionOtaResponse())
      {
        ALOG_INF(PSTR("LCDOTA: LCD upload command accepted."));
      }
      else
      {
        ALOG_INF(PSTR("LCDOTA: LCD upload command FAILED.  Restarting device."));
        espReset();
      }
      ALOG_INF(PSTR("LCDOTA: Starting update"));
      lcdOtaTimer = millis();
      while (lcdOtaHttp.connected() && (lcdOtaRemaining > 0 || lcdOtaRemaining == -1))
      {                                                // Write incoming data to panel as it arrives
        uint16_t lcdOtaHttpSize = stream->available(); // get available data size

        if (lcdOtaHttpSize)
        {
          uint16_t lcdOtaChunkSize = 0;
          if ((lcdOtaHttpSize <= lcdOtaBufferSize) && (lcdOtaHttpSize <= (4096 - lcdOtaChunkCounter)))
          {
            lcdOtaChunkSize = lcdOtaHttpSize;
          }
          else if ((lcdOtaBufferSize <= lcdOtaHttpSize) && (lcdOtaBufferSize <= (4096 - lcdOtaChunkCounter)))
          {
            lcdOtaChunkSize = lcdOtaBufferSize;
          }
          else
          {
            lcdOtaChunkSize = 4096 - lcdOtaChunkCounter;
          }
          stream->readBytes(lcdOtaBuffer, lcdOtaChunkSize);
          Serial2.flush();                              // make sure any previous writes the UART have completed
          Serial2.write(lcdOtaBuffer, lcdOtaChunkSize); // now send buffer to the UART
          lcdOtaChunkCounter += lcdOtaChunkSize;
          if (lcdOtaChunkCounter >= 4096)
          {
            Serial2.flush();
            lcdOtaPartNum++;
            lcdOtaTransferred += lcdOtaChunkCounter;
            lcdOtaPercentComplete = (lcdOtaTransferred * 100) / lcdOtaFileSize;
            lcdOtaChunkCounter = 0;
            if (nextionOtaResponse())
            { // We've completed a chunk
              ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Part %d OK, %d%% complete"), lcdOtaPartNum, lcdOtaPercentComplete);
              lcdOtaTimer = millis();
            }
            else
            {
              ALOG_ERR(PSTR(D_LOG_NEXTION "LCDOTA: Part %d FAILED, %d%% complete"), lcdOtaPartNum, lcdOtaPercentComplete);
              delay(2000); // extra delay while the LCD does its thing
              espReset();
            }
          }
          else
          {
            delay(20);
          }
          if (lcdOtaRemaining > 0)
          {
            lcdOtaRemaining -= lcdOtaChunkSize;
          }
        }
        delay(10);
        if ((lcdOtaTimer > 0) && ((millis() - lcdOtaTimer) > lcdOtaTimeout))
        { // Our timer expired so reset
          ALOG_INF(PSTR("LCDOTA: ERROR: LCD upload timeout. Restarting."));
          espReset();
        }
      }
      lcdOtaPartNum++;
      lcdOtaTransferred += lcdOtaChunkCounter;
      if ((lcdOtaTransferred == lcdOtaFileSize) && nextionOtaResponse())
      {
        ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Success, wrote %d of %d bytes"), lcdOtaTransferred, tftFileSize);
        uint32_t lcdOtaDelay = millis();
        ALOG_INF(PSTR("LCDOTA: Waiting 5 seconds to allow LCD to apply updates we've sent."));
        while ((millis() - lcdOtaDelay) < 5000)
        { // extra 5sec delay while the LCD handles any local firmware updates from new versions of code sent to it
          webServer->handleClient();
          yield();
        }
        espReset();
      }
      else
      {
        ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Failure, lcdOtaTransferred: %d lcdOtaFileSize: %d"), lcdOtaTransferred, lcdOtaFileSize);
        espReset();
      }
    }
  }
  else
  {
    ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: HTTP GET failed, error code %s"), lcdOtaHttp.errorToString(lcdOtaHttpReturn));
    espReset();
  }
  lcdOtaHttp.end();
}


void mNextionPanel::webHandleTftFileSize()
{ // http://plate01/tftFileSize
  // if (configPassword[0] != '\0')
  // { //Request HTTP auth if configPassword is set
  //   if (!webServer->authenticate(configUser, configPassword))
  //   {
  //     return webServer->requestAuthentication();
  //   }
  // }

  ALOG_INF(PSTR(D_LOG_NEXTION DEBUG_INSERT_PAGE_BREAK  "HTTP: Sending /tftFileSize page to client connected from: %s"), webServer->client().remoteIP().toString());

  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", String(pCONT_set->Settings.system_name.friendly) + " TFT Filesize");
  webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  webServer->send(200, "text/html", httpHeader);
  webServer->sendContent_P(HTTP_HEAD_END);
  tftFileSize = webServer->arg("tftFileSize").toInt();
  
  ALOG_INF(PSTR(D_LOG_NEXTION "Received tftFileSize: %d"), tftFileSize);

}


/**
 * @brief 
 * 
 */
void mNextionPanel::webHandleLcdUpload()
{ // http://plate01/lcdupload

  ALOG_INF(PSTR("webHandleLcdUpload"));

  static uint32_t lcdOtaTransferred = 0;
  static uint32_t lcdOtaRemaining;
  static uint16_t lcdOtaParts;
  const uint32_t lcdOtaTimeout = 30000; // timeout for receiving new data in milliseconds
  static uint32_t lcdOtaTimer = 0;      // timer for upload timeout

  HTTPUpload &upload = webServer->upload();

  ALOG_INF(PSTR(D_LOG_NEXTION "here Received tftFileSize: %d"), tftFileSize);

  if (tftFileSize == 0)
  {
    ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: FAILED, no filesize sent."));
    String httpHeader = FPSTR(HTTP_HEAD_START);
    httpHeader.replace("{v}", String(pCONT_set->Settings.system_name.friendly) + " LCD update error");
    webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
    webServer->send(200, "text/html", httpHeader);
    webServer->sendContent_P(HTTP_SCRIPT);
    webServer->sendContent_P(HTTP_STYLE);
    webServer->sendContent_P(HASP_STYLE);
    webServer->sendContent(F("<meta http-equiv='refresh' content='5;url=/firmware' />"));
    webServer->sendContent_P(HTTP_HEAD_END);
    webServer->sendContent(F("<h1>"));
    webServer->sendContent(haspNode);
    webServer->sendContent(F(" LCD update FAILED</h1>"));
    webServer->sendContent(F("No update file size reported. You must use a modern browser with Javascript enabled."));
    webServer->sendContent_P(HTTP_END);
    webServer->sendContent("");
  }
  else if ((lcdOtaTimer > 0) && ((millis() - lcdOtaTimer) > lcdOtaTimeout))
  { // Our timer expired so reset
    ALOG_INF(PSTR("LCDOTA: ERROR: LCD upload timeout.  Restarting."));
    espReset();
  }
  else if (upload.status == UPLOAD_FILE_START)
  {
    // WiFiUDP::stopAll(); // Keep mDNS responder from breaking things
    // WiFiUDP::stop(); // Keep mDNS responder from breaking things
  
    ALOG_INF(PSTR("(upload.status == UPLOAD_FILE_START)"));

    ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Attempting firmware upload"));
    ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: upload.filename: %s"), upload.filename);
    ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: TFTfileSize: %d"), tftFileSize);

    lcdOtaRemaining = tftFileSize;
    lcdOtaParts = (lcdOtaRemaining / 4096) + 1;
    ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: File upload beginning. Size %d bytes in %d 4k chunks"), lcdOtaRemaining, lcdOtaParts);
    
    Serial2.write(nextionSuffix, sizeof(nextionSuffix)); // Send empty command to LCD
    Serial2.flush();
    nextionHandleInput();

    String lcdOtaNextionCmd = "whmi-wri " + String(tftFileSize) + "," + String(nextionBaud) + ",0";
    ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Sending LCD upload command: %s"), lcdOtaNextionCmd);
    Serial2.print(lcdOtaNextionCmd);
    Serial2.write(nextionSuffix, sizeof(nextionSuffix));
    Serial2.flush();

    
    if (nextionOtaResponse())
    {
      ALOG_INF(PSTR("LCDOTA: LCD upload command accepted"));
    }
    else
    {
      ALOG_INF(PSTR("LCDOTA: LCD upload command FAILED."));
      espReset();
    }
    lcdOtaTimer = millis();
  }
  else if (upload.status == UPLOAD_FILE_WRITE)
  { // Handle upload data
    static int lcdOtaChunkCounter = 0;
    static uint16_t lcdOtaPartNum = 0;
    static int lcdOtaPercentComplete = 0;
    static const uint16_t lcdOtaBufferSize = 1024; // upload data buffer before sending to UART
    static uint8_t lcdOtaBuffer[lcdOtaBufferSize] = {};
    uint16_t lcdOtaUploadIndex = 0;
    int32_t lcdOtaPacketRemaining = upload.currentSize;
    
    ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: File upload beginning. Size %d bytes in %d 4k chunks"), lcdOtaRemaining, lcdOtaParts);

    while (lcdOtaPacketRemaining > 0)
    { 
      
      #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
      WDT_Reset();
      #endif
                
      // Write incoming data to panel as it arrives
      // determine chunk size as lowest value of lcdOtaPacketRemaining, lcdOtaBufferSize, or 4096 - lcdOtaChunkCounter
      uint16_t lcdOtaChunkSize = 0;
      if ((lcdOtaPacketRemaining <= lcdOtaBufferSize) && (lcdOtaPacketRemaining <= (4096 - lcdOtaChunkCounter)))
      {
        lcdOtaChunkSize = lcdOtaPacketRemaining;
      }
      else if ((lcdOtaBufferSize <= lcdOtaPacketRemaining) && (lcdOtaBufferSize <= (4096 - lcdOtaChunkCounter)))
      {
        lcdOtaChunkSize = lcdOtaBufferSize;
      }
      else
      {
        lcdOtaChunkSize = 4096 - lcdOtaChunkCounter;
      }

      for (uint16_t i = 0; i < lcdOtaChunkSize; i++)
      { // Load up the UART buffer
        lcdOtaBuffer[i] = upload.buf[lcdOtaUploadIndex];
        // 
        // ALOG_INF(PSTR("lcdOtaBuffer[%d|%d] = [%d]%d"), i, lcdOtaChunkSize, lcdOtaUploadIndex, lcdOtaBuffer[i]);

        lcdOtaUploadIndex++;
      }
      Serial2.flush();                              // Clear out current UART buffer
      Serial2.write(lcdOtaBuffer, lcdOtaChunkSize); // And send the most recent data
      lcdOtaChunkCounter += lcdOtaChunkSize;
      lcdOtaTransferred += lcdOtaChunkSize;


      // AddLog_Array5(LOG_LEVEL_INFO, PSTR("lcdOtaBuffer"), lcdOtaBuffer, 10);





      if (lcdOtaChunkCounter >= 4096)
      {
        Serial2.flush();
        lcdOtaPartNum++;
        lcdOtaPercentComplete = (lcdOtaTransferred * 100) / tftFileSize;
        lcdOtaChunkCounter = 0;
        if (nextionOtaResponse())
        {
          ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Part %d OK, %d bytes send, %d%% complete"), lcdOtaPartNum, lcdOtaTransferred, lcdOtaPercentComplete);
        }
        else
        {
          ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Part %d FAILED, %d%% complete"), lcdOtaPartNum, lcdOtaPercentComplete);
        }
      }
      else
      {
        delay(10);
      }
      if (lcdOtaRemaining > 0)
      {
        lcdOtaRemaining -= lcdOtaChunkSize;
      }
      if (lcdOtaPacketRemaining > 0)
      {
        lcdOtaPacketRemaining -= lcdOtaChunkSize;
      }
    }

    if (lcdOtaTransferred >= tftFileSize)
    {
      if (nextionOtaResponse())
      {
        ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Success, wrote %d of %d bytes"), lcdOtaTransferred, tftFileSize);
        webServer->sendHeader("Location", "/lcdOtaSuccess");
        webServer->send(303);
        uint32_t lcdOtaDelay = millis();
        while ((millis() - lcdOtaDelay) < 5000)
        { // extra 5sec delay while the LCD handles any local firmware updates from new versions of code sent to it
          webServer->handleClient();
          delay(1);
        }
        espReset();
      }
      else
      {
        ALOG_INF(PSTR("LCDOTA: Failure"));
        webServer->sendHeader("Location", "/lcdOtaFailure");
        webServer->send(303);
        uint32_t lcdOtaDelay = millis();
        while ((millis() - lcdOtaDelay) < 1000)
        { // extra 1sec delay for client to grab failure page
          webServer->handleClient();
          delay(1);
        }
        espReset();
      }
    }
    lcdOtaTimer = millis();
  }
  else if (upload.status == UPLOAD_FILE_END)
  { // Upload completed
    if (lcdOtaTransferred >= tftFileSize)
    {
      if (nextionOtaResponse())
      { // YAY WE DID IT
        ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Success, wrote %d of %d bytes"), lcdOtaTransferred, tftFileSize);
        webServer->sendHeader("Location", "/lcdOtaSuccess");
        webServer->send(303);
        uint32_t lcdOtaDelay = millis();
        while ((millis() - lcdOtaDelay) < 5000)
        { // extra 5sec delay while the LCD handles any local firmware updates from new versions of code sent to it
          webServer->handleClient();
          yield();
        }
        espReset();
      }
      else
      {
        ALOG_INF(PSTR("LCDOTA: Failure"));
        webServer->sendHeader("Location", "/lcdOtaFailure");
        webServer->send(303);
        uint32_t lcdOtaDelay = millis();
        while ((millis() - lcdOtaDelay) < 1000)
        { // extra 1sec delay for client to grab failure page
          webServer->handleClient();
          yield();
        }
        espReset();
      }
    }
  }
  else if (upload.status == UPLOAD_FILE_ABORTED)
  { // Something went kablooey
    ALOG_INF(PSTR("LCDOTA: ERROR: upload.status returned: UPLOAD_FILE_ABORTED"));
    ALOG_INF(PSTR("LCDOTA: Failure"));
    webServer->sendHeader("Location", "/lcdOtaFailure");
    webServer->send(303);
    uint32_t lcdOtaDelay = millis();
    while ((millis() - lcdOtaDelay) < 1000)
    { // extra 1sec delay for client to grab failure page
      webServer->handleClient();
      yield();
    }
    espReset();
  }
  else
  { // Something went weird, we should never get here...
    ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: upload.status returned: %s"), String(upload.status));
    ALOG_INF(PSTR("LCDOTA: Failure"));
    webServer->sendHeader("Location", "/lcdOtaFailure");
    webServer->send(303);
    uint32_t lcdOtaDelay = millis();
    while ((millis() - lcdOtaDelay) < 1000)
    { // extra 1sec delay for client to grab failure page
      webServer->handleClient();
      yield();
    }
    espReset();
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleLcdUpdateSuccess()
{ // http://plate01/lcdOtaSuccess
//   if (configPassword[0] != '\0')
//   { //Request HTTP auth if configPassword is set
//     if (!webServer->authenticate(configUser, configPassword))
//     {
//       return webServer->requestAuthentication();
//     }
//   }
  ALOG_INF(PSTR(D_LOG_NEXTION "HTTP: Sending /lcdOtaSuccess page to client connected from: %s"), webServer->client().remoteIP().toString());
  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", "HASPone " + String(haspNode) + " LCD firmware update success");
  webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  webServer->send(200, "text/html", httpHeader);
  webServer->sendContent_P(HTTP_SCRIPT);
  webServer->sendContent_P(HTTP_STYLE);
  webServer->sendContent_P(HASP_STYLE);
  webServer->sendContent(F("<meta http-equiv='refresh' content='15;url=/' />"));
  webServer->sendContent_P(HTTP_HEAD_END);
  webServer->sendContent(F("<h1>"));
  webServer->sendContent(haspNode);
  webServer->sendContent(F(" LCD update success</h1>"));
  webServer->sendContent(F("Restarting HASwitchPlate to apply changes..."));
  webServer->sendContent_P(HTTP_END);
  webServer->sendContent("");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleLcdUpdateFailure()
{ // http://plate01/lcdOtaFailure
  // if (configPassword[0] != '\0')
  // { //Request HTTP auth if configPassword is set
  //   if (!webServer->authenticate(configUser, configPassword))
  //   {
  //     return webServer->requestAuthentication();
  //   }
  // }
  ALOG_INF(PSTR(D_LOG_NEXTION "HTTP: Sending /lcdOtaFailure page to client connected from: %s"), webServer->client().remoteIP().toString());
  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", "HASPone " + String(haspNode) + " LCD firmware update failed");
  webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  webServer->send(200, "text/html", httpHeader);
  webServer->sendContent_P(HTTP_SCRIPT);
  webServer->sendContent_P(HTTP_STYLE);
  webServer->sendContent_P(HASP_STYLE);
  webServer->sendContent(F("<meta http-equiv='refresh' content='15;url=/' />"));
  webServer->sendContent_P(HTTP_HEAD_END);
  webServer->sendContent(F("<h1>"));
  webServer->sendContent(haspNode);
  webServer->sendContent(F(" LCD update failed :(</h1>"));
  webServer->sendContent(F("Restarting HASwitchPlate to apply changes..."));
  webServer->sendContent_P(HTTP_END);
  webServer->sendContent("");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleLcdDownload()
{ // http://plate01/lcddownload
  // if (configPassword[0] != '\0')
  // { //Request HTTP auth if configPassword is set
  //   if (!webServer->authenticate(configUser, configPassword))
  //   {
  //     return webServer->requestAuthentication();
  //   }
  // }
  ALOG_INF(PSTR(D_LOG_NEXTION "HTTP: Sending /lcddownload page to client connected from: %s"), webServer->client().remoteIP().toString());
  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", "HASPone " + String(haspNode) + " LCD firmware update");
  webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  webServer->send(200, "text/html", httpHeader);
  webServer->sendContent_P(HTTP_SCRIPT);
  webServer->sendContent_P(HTTP_STYLE);
  webServer->sendContent_P(HASP_STYLE);
  webServer->sendContent_P(HTTP_HEAD_END);
  webServer->sendContent(F("<h1>"));
  webServer->sendContent(haspNode);
  webServer->sendContent(F(" LCD update</h1>"));
  webServer->sendContent(F("<br/>Updating LCD firmware from: "));
  webServer->sendContent(webServer->arg("lcdFirmware"));
  webServer->sendContent_P(HTTP_END);
  webServer->sendContent("");
  nextionOtaStartDownload(webServer->arg("lcdFirmware"));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleReboot()
{ // http://plate01/reboot
//   if (configPassword[0] != '\0')
//   { //Request HTTP auth if configPassword is set
//     if (!webServer->authenticate(configUser, configPassword))
//     {
//       return webServer->requestAuthentication();
//     }
//   }
  ALOG_INF(PSTR(D_LOG_NEXTION "HTTP: Sending /reboot page to client connected from: %s"), webServer->client().remoteIP().toString());
  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", "HASPone " + String(haspNode) + " reboot");
  webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  webServer->send(200, "text/html", httpHeader);
  webServer->sendContent_P(HTTP_SCRIPT);
  webServer->sendContent_P(HTTP_STYLE);
  webServer->sendContent_P(HASP_STYLE);
  webServer->sendContent(F("<meta http-equiv='refresh' content='10;url=/' />"));
  webServer->sendContent_P(HTTP_HEAD_END);
  webServer->sendContent(F("<h1>"));
  webServer->sendContent(haspNode);
  webServer->sendContent(F(" Reboot</h1>"));
  webServer->sendContent(F("<br/>Rebooting device"));
  webServer->sendContent_P(HTTP_END);
  webServer->sendContent("");
  nextionSendCmd("page 0");
  nextionSetAttr("p[0].b[1].txt", "\"Rebooting...\"");
  espReset();
}

void mNextionPanel::espReset(){
esp_restart();
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleFirmware()
{ // http://plate01/firmware
 

  ALOG_INF(PSTR(D_LOG_NEXTION "HTTP: Sending /firmware page to client connected from: %s"), webServer->client().remoteIP().toString());
  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", "HASPone " + String(haspNode) + " Firmware updates");
  webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  webServer->send(200, "text/html", httpHeader);
  webServer->sendContent_P(HTTP_SCRIPT);
  webServer->sendContent_P(HTTP_STYLE);
  webServer->sendContent_P(HASP_STYLE);
  webServer->sendContent_P(HTTP_HEAD_END);

  webServer->sendContent(F("<h1>"));
  webServer->sendContent(pCONT_set->Settings.system_name.friendly);
  webServer->sendContent(F("</h1>"));
  webServer->sendContent(F("<hr><h1>"));
  webServer->sendContent(F("Nextion Firmware Update</h1>"));

  /**
   * @brief Update LCD via URL
   **/
  webServer->sendContent(F("<form method='get' action='lcddownload'>"));  
  webServer->sendContent(F("<br/><b>Update Nextion LCD from URL</b><small><i> http only</i></small>"));
  webServer->sendContent(F("<br/><input id='lcdFirmware' name='lcdFirmware' value='"));
  webServer->sendContent("lcdFirmwareUrl");
  webServer->sendContent(F("'><br/><br/><button type='submit'>Update LCD from URL</button></form>"));

  /**
   * @brief Update LCD via tft file upload
   **/
  webServer->sendContent(F("<br/><form method='POST' action='/lcdupload' enctype='multipart/form-data'>"));
  webServer->sendContent(F("<br/><b>Update Nextion LCD from file</b><input type='file' id='lcdSelect' name='files[]' accept='.tft'/>"));
  webServer->sendContent(F("<br/><br/><button type='submit' id='lcdUploadSubmit' onclick='ackLcdUploadSubmit()'>Update LCD from file</button></form>"));

    // Javascript to collect the filesize of the LCD upload and send it to /tftFileSize
    webServer->sendContent(F("<script>function handleLcdFileSelect(evt) {"));
    webServer->sendContent(F("var uploadFile = evt.target.files[0];"));
    webServer->sendContent(F("document.getElementById('lcdUploadSubmit').innerHTML = 'Upload LCD firmware ' + uploadFile.name;"));
    webServer->sendContent(F("var tftFileSize = '/tftFileSize?tftFileSize=' + uploadFile.size;"));
    webServer->sendContent(F("var xhttp = new XMLHttpRequest();xhttp.open('GET', tftFileSize, true);xhttp.send();}"));
    webServer->sendContent(F("function ackLcdUploadSubmit() {document.getElementById('lcdUploadSubmit').innerHTML = 'Uploading LCD firmware...';}"));
    webServer->sendContent(F("function handleEspFileSelect(evt) {var uploadFile = evt.target.files[0];document.getElementById('espUploadSubmit').innerHTML = 'Upload ESP firmware ' + uploadFile.name;}"));
    webServer->sendContent(F("function ackEspUploadSubmit() {document.getElementById('espUploadSubmit').innerHTML = 'Uploading ESP firmware...';}"));
    webServer->sendContent(F("document.getElementById('lcdSelect').addEventListener('change', handleLcdFileSelect, false);"));
    webServer->sendContent(F("document.getElementById('espSelect').addEventListener('change', handleEspFileSelect, false);</script>"));

  webServer->sendContent_P(HTTP_END);
  webServer->sendContent("");

}


/**
 * @brief New 2023 version of my nextion code
 * Basic webui page that will permit uploading TFT files to update the panel
 * 
 * Ideally, this page can easily be integrated with NETWORK_WEBSERVER on the unified system
 * 
 */
void mNextionPanel::WebPage_LCD_Update_TFT()
{ 
  
  // if (configPassword[0] != '\0')  // Keep! I will want to integrate this in the future too
  // { //Request HTTP auth if configPassword is set
  //   if (!webServer->authenticate(configUser, configPassword))
  //   {
  //     return webServer->requestAuthentication();
  //   }
  // }

  ALOG_INF(PSTR("HTTP: Sending /firmware page to client connected from:  %s"), webServer->client().remoteIP().toString());

  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", String(pCONT_set->Settings.system_name.friendly) + " Firmware updates");
  webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  webServer->send(200, "text/html", httpHeader);
  webServer->sendContent_P(HTTP_SCRIPT);
  webServer->sendContent_P(HTTP_STYLE);
  webServer->sendContent_P(HASP_STYLE);
  webServer->sendContent_P(HTTP_HEAD_END);

  webServer->sendContent(F("<h1>"));
  webServer->sendContent(pCONT_set->Settings.system_name.friendly);
  webServer->sendContent(F(" Nextion Firmware Update</h1>"));
  

  /**
   * @brief Update with URL only
   **/
  webServer->sendContent(F("<br/><hr><form method='get' action='lcddownload'>"));
  if(updateLcdAvailable){ webServer->sendContent(F("<font color='green'><b>LCD update available!</b></font>")); }
  webServer->sendContent(F("<br/><b>Update Nextion LCD from URL</b><small><i> http only</i></small>"));
  webServer->sendContent(F("<br/><input id='lcdFirmware' name='lcdFirmware' value='"));
  webServer->sendContent("No Url Set");
  webServer->sendContent(F("'><br/><br/><button type='submit'>Update LCD from URL</button></form>"));

  /**
   * @brief Update with File
   **/
  webServer->sendContent(F("<br/><form method='POST' action='/lcdupload' enctype='multipart/form-data'>"));
  webServer->sendContent(F("<br/><b>Update Nextion LCD from file</b><input type='file' id='lcdSelect' name='files[]' accept='.tft'/>"));
  webServer->sendContent(F("<br/><br/><button type='submit' id='lcdUploadSubmit' onclick='ackLcdUploadSubmit()'>Update LCD from file</button></form>"));

  /**
   * @brief Javascript to collect the filesize of the LCD upload and send it to /tftFileSize
   **/
  webServer->sendContent(F("<script>function handleLcdFileSelect(evt) {"));
  webServer->sendContent(F("var uploadFile = evt.target.files[0];"));
  webServer->sendContent(F("document.getElementById('lcdUploadSubmit').innerHTML = 'Upload LCD firmware ' + uploadFile.name;"));
  webServer->sendContent(F("var tftFileSize = '/tftFileSize?tftFileSize=' + uploadFile.size;"));
  webServer->sendContent(F("var xhttp = new XMLHttpRequest();xhttp.open('GET', tftFileSize, true);xhttp.send();}"));
  webServer->sendContent(F("function ackLcdUploadSubmit() {document.getElementById('lcdUploadSubmit').innerHTML = 'Uploading LCD firmware...';}"));
  webServer->sendContent(F("function handleEspFileSelect(evt) {var uploadFile = evt.target.files[0];document.getElementById('espUploadSubmit').innerHTML = 'Upload ESP firmware ' + uploadFile.name;}"));
  webServer->sendContent(F("function ackEspUploadSubmit() {document.getElementById('espUploadSubmit').innerHTML = 'Uploading ESP firmware...';}"));
  webServer->sendContent(F("document.getElementById('lcdSelect').addEventListener('change', handleLcdFileSelect, false);"));
  webServer->sendContent(F("document.getElementById('espSelect').addEventListener('change', handleEspFileSelect, false);</script>"));

  webServer->sendContent_P(HTTP_END);
  webServer->sendContent("");

}

        #endif // ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER









#ifdef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER


////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::webHandleFirmware(AsyncWebServerRequest *request)
{ // http://plate01/firmware
 
    String data = "";

  // ALOG_INF(PSTR(D_LOG_NEXTION "HTTP: Sending /firmware page to client connected from: %s"), webServer->client().remoteIP().toString());
  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", "HASPone " + String(haspNode) + " Firmware updates");
  // webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  // webServer->send(200, "text/html", httpHeader);
  
    data += httpHeader;
    data += FPSTR(HTTP_SCRIPT3);
    data += FPSTR(HTTP_STYLE3);
    data += FPSTR(HASP_STYLE);
    data += FPSTR(HTTP_HEAD_END3);

  data += F("<h1>");
  data += String(pCONT_set->Settings.system_name.friendly);
  data += F("</h1>");
  data += F("<hr><h1>");
  data += F("Nextion Firmware Update</h1>");

  /**
   * @brief Update LCD via URL
   **/
  data += F("<form method='get' action='lcddownload'>");  
  data += F("<br/><b>Update Nextion LCD from URL</b><small><i> http only</i></small>");
  data += F("<br/><input id='lcdFirmware' name='lcdFirmware' value='");
  data += "lcdFirmwareUrl";
  data += F("'><br/><br/><button type='submit'>Update LCD from URL</button></form>");

  /**
   * @brief Update LCD via tft file upload
   **/
  data += F("<br/><form method='POST' action='/lcdupload' enctype='multipart/form-data'>");
  data += F("<br/><b>Update Nextion LCD from file</b><input type='file' id='lcdSelect' name='files[]' accept='.tft'/>");
  data += F("<br/><br/><button type='submit' id='lcdUploadSubmit' onclick='ackLcdUploadSubmit()'>Update LCD from file</button></form>");

    // Javascript to collect the filesize of the LCD upload and send it to /tftFileSize
    data += F("<script>function handleLcdFileSelect(evt) {");
    data += F("var uploadFile = evt.target.files[0];");
    data += F("document.getElementById('lcdUploadSubmit').innerHTML = 'Upload LCD firmware ' + uploadFile.name;");
    data += F("var tftFileSize = '/tftFileSize?tftFileSize=' + uploadFile.size;");
    data += F("var xhttp = new XMLHttpRequest();xhttp.open('GET', tftFileSize, true);xhttp.send();}");
    data += F("function ackLcdUploadSubmit() {document.getElementById('lcdUploadSubmit').innerHTML = 'Uploading LCD firmware...';}");
    data += F("function handleEspFileSelect(evt) {var uploadFile = evt.target.files[0];document.getElementById('espUploadSubmit').innerHTML = 'Upload ESP firmware ' + uploadFile.name;}");
    data += F("function ackEspUploadSubmit() {document.getElementById('espUploadSubmit').innerHTML = 'Uploading ESP firmware...';}");
    data += F("document.getElementById('lcdSelect').addEventListener('change', handleLcdFileSelect, false);");
    data += F("document.getElementById('espSelect').addEventListener('change', handleEspFileSelect, false);</script>");

  data += FPSTR(HTTP_END3);
  
  request->send(200, "text/html", data);

}



/**
 * @brief 
 * 
 */
void mNextionPanel::webHandleLcdUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{ // http://plate01/lcdupload

  ALOG_INF(PSTR("================================================================\r\nUpload i=%d|len=%d|rem=%d|final=%d|tx=%d"), index, len, tftFileSize-len, final, transmitted_bytes);

    String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
  Serial.println(logmessage);

  static uint32_t lcdOtaTransferred = 0;
  static uint32_t lcdOtaRemaining;
  static uint16_t lcdOtaParts;
  const uint32_t lcdOtaTimeout = 120000; // timeout for receiving new data in milliseconds
  static uint32_t lcdOtaTimer = 0;      // timer for upload timeout

  // HTTPUpload &upload = webServer->upload();

  // ALOG_INF(PSTR(D_LOG_NEXTION "here Received tftFileSize: %d"), tftFileSize);
  // ALOG_INF(PSTR(D_LOG_NEXTION "here Received index: %d"), index);

  if (tftFileSize == 0)
  {

    String data2 = "";
    ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: FAILED, no filesize sent."));
    String httpHeader = FPSTR(HTTP_HEAD_START);
    httpHeader.replace("{v}", String(pCONT_set->Settings.system_name.friendly) + " LCD update error");
    // webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);

    data2 += httpHeader;
    data2 += FPSTR(HTTP_SCRIPT3);
    data2 += FPSTR(HTTP_STYLE3);
    data2 += FPSTR(HASP_STYLE);
    data2 += (F("<meta http-equiv='refresh' content='5;url=/firmware' />"));
    data2 += FPSTR(HTTP_HEAD_END3);

    data2 += (F("<h1>"));
    data2 += String(haspNode);
    data2 += (F(" LCD update FAILED</h1>"));
    data2 += (F("No update file size reported. You must use a modern browser with Javascript enabled."));
    
    data2 += FPSTR(HTTP_END3);
  
    request->send(200, "text/html", data2);

    return;
  }
  
   
  // if ((lcdOtaTimer > 0) && ((millis() - lcdOtaTimer) > lcdOtaTimeout))
  // { // Our timer expired so reset
  //   ALOG_INF(PSTR("LCDOTA: ERROR: LCD upload timeout.  Restarting."));
  //   espReset();
  // }
  // else if (upload.status == UPLOAD_FILE_START)
  


  if(index == 0) // First byte of file, initialise the start of the upload
  {

    Serial.println("UPLOAD_FILE_START");  
    ALOG_INF(PSTR("(upload.status == UPLOAD_FILE_START)"));
    logmessage = "Upload Start: " + String(filename);    
    Serial.println(logmessage);
    ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Attempting firmware upload"));
    ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: TFTfileSize: %d"), tftFileSize);

    lcdOtaRemaining = tftFileSize;
    lcdOtaParts = (lcdOtaRemaining / 4096) + 1;
    ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: File upload beginning. Size %d bytes in %d 4k chunks"), lcdOtaRemaining, lcdOtaParts);

    transmitted_bytes = 0;

    // Serial2.begin(115200); delay(200);
    // while (Serial2.available() > 0) { Serial2.read(); } // Clear out any old data
    
    Serial2.write(nextionSuffix, sizeof(nextionSuffix)); // Send empty command to LCD
    Serial2.flush();
    nextionHandleInput();

    String lcdOtaNextionCmd = "whmi-wri " + String(tftFileSize) + "," + String(nextionBaud) + ",0";
    ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Sending LCD upload command: %s"), lcdOtaNextionCmd.c_str());
    Serial2.print(lcdOtaNextionCmd);
    Serial2.write(nextionSuffix, sizeof(nextionSuffix));
    Serial2.flush();

    
    ALOG_INF(PSTR("LCDOTA: nextionOtaResponse------------------?"));

    if (nextionOtaResponse())
    {
      ALOG_INF(PSTR("LCDOTA: LCD upload command accepted"));
    }
    else
    {
      ALOG_INF(PSTR("LCDOTA: LCD upload command FAILED."));

      delay(5000);
      espReset();
    }

    lcdOtaTimer = millis();
  }
  
  


  // else if (upload.status == UPLOAD_FILE_WRITE)
  if ( index>=0) // Now sending data
  { 
    
    // Handle upload data
    // Serial.println("upload.status == UPLOAD_FILE_WRITE");

    static int lcdOtaChunkCounter = 0;
    static uint16_t lcdOtaPartNum = 0;
    static int lcdOtaPercentComplete = 0;
    static const uint16_t lcdOtaBufferSize = 1024; // upload data buffer before sending to UART
    static uint8_t lcdOtaBuffer[lcdOtaBufferSize] = {};
    uint16_t lcdOtaUploadIndex = 0;


    int32_t lcdOtaPacketRemaining = len;//upload.currentSize;


    Serial.printf("UPLOAD: Receiving: '%s'\n\r", filename.c_str());
    Serial.printf( "%i bytes received.\n\r", index );
    const char* FILESIZE_HEADER{"FileSize"};
    Serial.printf("UPLOAD: fileSize: %s\n\r", request->header(FILESIZE_HEADER));
    Serial.printf("UPLOAD: lcdOtaPacketRemaining: %d\n\r", lcdOtaPacketRemaining);


    while (lcdOtaPacketRemaining > 0)
    { 

      DEBUG_LINE_HERE;
      
      #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
      WDT_Reset();
      #endif
                
      // Write incoming data to panel as it arrives
      // determine chunk size as lowest value of lcdOtaPacketRemaining, lcdOtaBufferSize, or 4096 - lcdOtaChunkCounter
      uint16_t lcdOtaChunkSize = 0;
      if ((lcdOtaPacketRemaining <= lcdOtaBufferSize) && (lcdOtaPacketRemaining <= (4096 - lcdOtaChunkCounter)))
      {
        lcdOtaChunkSize = lcdOtaPacketRemaining;
      }
      else 
      if ((lcdOtaBufferSize <= lcdOtaPacketRemaining) && (lcdOtaBufferSize <= (4096 - lcdOtaChunkCounter)))
      {
        lcdOtaChunkSize = lcdOtaBufferSize;
      }
      else
      {
        lcdOtaChunkSize = 4096 - lcdOtaChunkCounter;
      }
      
      for (uint16_t i = 0; i < lcdOtaChunkSize; i++)
      { // Load up the UART buffer
        lcdOtaBuffer[i] = data[lcdOtaUploadIndex];
        // ALOG_INF(PSTR("lcdOtaBuffer[%d|%d] = [%d]%d   %d"), i, lcdOtaChunkSize, lcdOtaUploadIndex, lcdOtaBuffer[i], data[lcdOtaUploadIndex]);
        lcdOtaUploadIndex++;
        transmitted_bytes++;
      }
      Serial2.flush();                              // Clear out current UART buffer
      Serial2.write(lcdOtaBuffer, lcdOtaChunkSize); // And send the most recent data

      ALOG_INF(PSTR("Sending lcdOtaBuffer %d\t\ttransmitted_bytes=%d"), lcdOtaBuffer[0], transmitted_bytes);

      // AddLog_Array5(LOG_LEVEL_INFO, PSTR("lcdOtaBuffer"), lcdOtaBuffer, 10);

      lcdOtaChunkCounter += lcdOtaChunkSize;
      lcdOtaTransferred += lcdOtaChunkSize;
      
      if (lcdOtaChunkCounter >= 4096) // Everytime we exceed the max buffer, check the status of the display
      {
        DEBUG_LINE_HERE;
        Serial2.flush();
        lcdOtaPartNum++;
        lcdOtaPercentComplete = (lcdOtaTransferred * 100) / tftFileSize;
        lcdOtaChunkCounter = 0;
        if (nextionOtaResponse())
        {
          ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Part %d OK, %d bytes send, %d%% complete"), lcdOtaPartNum, lcdOtaTransferred, lcdOtaPercentComplete);
        }
        else
        {
          ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Part %d FAILED, %d%% complete"), lcdOtaPartNum, lcdOtaPercentComplete);
        }
      }
      else
      {
        delay(10);
      }
      
      if(lcdOtaRemaining > 0)
      {
        lcdOtaRemaining -= lcdOtaChunkSize;
      }

      if(lcdOtaPacketRemaining > 0)
      {
        lcdOtaPacketRemaining -= lcdOtaChunkSize;
      }

      if(lcdOtaPacketRemaining==0){
        Serial.println("About to leave while, did I want to?");
      }

    }


    if (lcdOtaTransferred >= tftFileSize)
    {
      ALOG_INF(PSTR("(lcdOtaTransferred >= tftFileSize) \t Total bytes should have been sent, checking completion?"));
      if (nextionOtaResponse())
      {
        ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Success, wrote %d of %d bytes"), lcdOtaTransferred, tftFileSize);
        request->redirect("/lcdOtaSuccess"); 
        delay(10);
        espReset();
      }
      else
      {
        ALOG_INF(PSTR("LCDOTA: Failure"));
        request->redirect("/lcdOtaFailure"); 
        delay(10);
        espReset();
      }
    }
    lcdOtaTimer = millis();
  }

  // else if (upload.status == UPLOAD_FILE_END)
  
   
  if (final)
  { // Upload completed -- repeated from above, we should

  Serial.println("upload.status == UPLOAD_FILE_END");
    if (lcdOtaTransferred >= tftFileSize)
    {
      if (nextionOtaResponse())
      { // YAY WE DID IT
        ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Success, wrote %d of %d bytes"), lcdOtaTransferred, tftFileSize);
        request->redirect("/lcdOtaSuccess"); 
        delay(10);
        ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Delay to allow LCD to update"));//, lcdOtaTransferred, tftFileSize);
        espReset();
      }
      else
      {
        ALOG_INF(PSTR("LCDOTA: Failure"));
        request->redirect("/lcdOtaFailure");
        delay(10);
        espReset();
      }
    }
  }

  // else if (upload.status == UPLOAD_FILE_ABORTED)
  // { // Something went kablooey
  //   ALOG_INF(PSTR("LCDOTA: ERROR: upload.status returned: UPLOAD_FILE_ABORTED"));
  //   ALOG_INF(PSTR("LCDOTA: Failure"));
  //   webServer->sendHeader("Location", "/lcdOtaFailure");
  //   webServer->send(303);
  //   uint32_t lcdOtaDelay = millis();
  //   while ((millis() - lcdOtaDelay) < 1000)
  //   { // extra 1sec delay for client to grab failure page
  //     webServer->handleClient();
  //     yield();
  //   }
  //   espReset();
  // }
  else
  { // Something went weird, we should never get here...
  //   ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: upload.status returned: %s"), String(upload.status));
  //   ALOG_INF(PSTR("LCDOTA: Failure"));
  //   webServer->sendHeader("Location", "/lcdOtaFailure");
  //   webServer->send(303);
  //   uint32_t lcdOtaDelay = millis();
  //   while ((millis() - lcdOtaDelay) < 1000)
  //   { // extra 1sec delay for client to grab failure page
  //     webServer->handleClient();
  //     yield();
  //   }
  //   espReset();
  }
              
  DEBUG_LINE_HERE;

}


void mNextionPanel::espReset(){
  esp_restart();
}


void mNextionPanel::WebPage_LCD_Update_TFT(AsyncWebServerRequest *request)
{ 
  
  // if (configPassword[0] != '\0')  // Keep! I will want to integrate this in the future too
  // { //Request HTTP auth if configPassword is set
  //   if (!webServer->authenticate(configUser, configPassword))
  //   {
  //     return webServer->requestAuthentication();
  //   }
  // }

  ALOG_INF(PSTR("HTTP: Sending /firmware page to client connected from:  "));//, webServer->host());

  String data = String();


  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", String(pCONT_set->Settings.system_name.friendly) + " Firmware updates");
  // webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  // webServer->send(200, "text/html", httpHeader);
  
  data += httpHeader;
  data += FPSTR(HTTP_SCRIPT3);
  data += FPSTR(HTTP_STYLE3);
  data += FPSTR(HASP_STYLE);
  data += FPSTR(HTTP_HEAD_END3);

  data += F("<h1>");
  data += pCONT_set->Settings.system_name.friendly;
  data += F(" Nextion Firmware Update</h1>");
  

  /**
   * @brief Update with URL only
   **/
  data += F("<br/><hr><form method='get' action='lcddownload'>");
  if(updateLcdAvailable){ data += F("<font color='green'><b>LCD update available!</b></font>"); }
  data += F("<br/><b>Update Nextion LCD from URL</b><small><i> http only</i></small>");
  data += F("<br/><input id='lcdFirmware' name='lcdFirmware' value='");
  data += "No Url Set";
  data += F("'><br/><br/><button type='submit'>Update LCD from URL</button></form>");

  /**
   * @brief Update with File
   **/
  data += F("<br/><form method='POST' action='/lcdupload' enctype='multipart/form-data'>");
  data += F("<br/><b>Update Nextion LCD from file</b><input type='file' id='lcdSelect' name='files[]' accept='.tft'/>");
  data += F("<br/><br/><button type='submit' id='lcdUploadSubmit' onclick='ackLcdUploadSubmit()'>Update LCD from file</button></form>");

  /**
   * @brief Javascript to collect the filesize of the LCD upload and send it to /tftFileSize
   **/
  data += F("<script>function handleLcdFileSelect(evt) {");
  data += F("var uploadFile = evt.target.files[0];");
  data += F("document.getElementById('lcdUploadSubmit').innerHTML = 'Upload LCD firmware ' + uploadFile.name;");
  data += F("var tftFileSize = '/tftFileSize?tftFileSize=' + uploadFile.size;");
  data += F("var xhttp = new XMLHttpRequest();xhttp.open('GET', tftFileSize, true);xhttp.send();}");
  data += F("function ackLcdUploadSubmit() {document.getElementById('lcdUploadSubmit').innerHTML = 'Uploading LCD firmware...';}");
  data += F("function handleEspFileSelect(evt) {var uploadFile = evt.target.files[0];document.getElementById('espUploadSubmit').innerHTML = 'Upload ESP firmware ' + uploadFile.name;}");
  data += F("function ackEspUploadSubmit() {document.getElementById('espUploadSubmit').innerHTML = 'Uploading ESP firmware...';}");
  data += F("document.getElementById('lcdSelect').addEventListener('change', handleLcdFileSelect, false);");
  data += F("document.getElementById('espSelect').addEventListener('change', handleEspFileSelect, false);</script>");

  data += FPSTR(HTTP_END3);
  
  
  request->send(200, "text/html", data);

}


void mNextionPanel::webHandleTftFileSize(AsyncWebServerRequest *request)
{ // http://plate01/tftFileSize


  // ALOG_INF(PSTR(D_LOG_NEXTION DEBUG_INSERT_PAGE_BREAK  "HTTP: Sending /tftFileSize page to client connected from: %s"), webServer->client().remoteIP().toString());

  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", String(pCONT_set->Settings.system_name.friendly) + " TFT Filesize");
  // webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);

  httpHeader += FPSTR(HTTP_END3);


  request->send(200, "text/html", httpHeader);


  // webServer->sendContent_P(HTTP_HEAD_END);
  tftFileSize =  request->arg(F("tftFileSize")).toInt();

  // tftFileSize = webServer->arg("tftFileSize").toInt();
  
  
  ALOG_INF(PSTR(D_LOG_NEXTION "Received tftFileSize: %d"), tftFileSize);

}

void mNextionPanel::webHandleRoot(AsyncWebServerRequest* request)
{ 

  ALOG_INF(PSTR(D_LOG_NEXTION DEBUG_INSERT_PAGE_BREAK "HTTP: Sending root page to client connected from: %s"), request->host());

  String conv = String();

  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", "HASPone " + String(haspNode));
  conv += httpHeader;
  conv += FPSTR(HTTP_SCRIPT3);
  conv += FPSTR(HTTP_STYLE3);
  conv += FPSTR(HASP_STYLE);
  conv += FPSTR(HTTP_HEAD_END3);

  conv += (F("<br/><hr><button type='submit'>save settings</button></form>"));

  conv += (F("<hr><form method='get' action='firmware'>"));
  conv += (F("<button type='submit'>update firmware</button></form>"));

  conv += (F("<hr><form method='get' action='reboot'>"));
  conv += (F("<button type='submit'>reboot device</button></form>"));

  conv += (F("<hr><form method='get' action='resetConfig'>"));
  conv += (F("<button type='submit'>factory reset settings</button></form>"));

  conv += FPSTR(HTTP_END3);
  
  request->send(200, "text/html", conv);

}



#endif // ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER



#endif