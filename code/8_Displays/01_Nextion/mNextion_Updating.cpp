#include "mNextion.h"

#ifdef USE_MODULE_DISPLAYS_NEXTION




/******************************************************************************************************************
 * SECTION: WebServer - LCD OTA Download via URL
 *
 * Route:  GET /lcddownload?lcdFirmware=http://host/file.tft
 *
 * Notes:
 * - Mirrors the legacy oneHASP behavior: serves a simple HTML status page then starts download.
 * - Uses AsyncWebServerRequest::authenticate() if you have configUser/configPassword defined.
 * - Keeps the handler self-contained so it “fits into your code”.
 *******************************************************************************************************************/

void mNextion::webHandleLcdDownload(AsyncWebServerRequest* request)
{
  if (!request) return;

  // --------------------------------------------------------------------------
  // Extract URL
  // --------------------------------------------------------------------------
  if (!request->hasArg(F("lcdFirmware")))
  {
    request->send(400, FPSTR(PM_WEB_CONTENT_TYPE_TEXT_HTML), F("Missing query arg: lcdFirmware"));
    return;
  }

  const String lcdUrl = request->arg(F("lcdFirmware"));
  if (lcdUrl.length() == 0)
  {
    request->send(400, FPSTR(PM_WEB_CONTENT_TYPE_TEXT_HTML), F("Empty lcdFirmware URL"));
    return;
  }

  // --------------------------------------------------------------------------
  // Serve a simple status page immediately (like oneHASP)
  // --------------------------------------------------------------------------
  String data;

  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", String(F("HASPone ")) + String(tkr_set->Settings.system_name.friendly) + String(F(" LCD firmware update")));

  data += httpHeader;
  data += FPSTR(HTTP_SCRIPT3);   // If you only have HTTP_SCRIPT, swap to that
  data += FPSTR(HTTP_STYLE3);    // If you only have HTTP_STYLE, swap to that
  data += FPSTR(HASP_STYLE);
  data += FPSTR(HTTP_HEAD_END3); // If you only have HTTP_HEAD_END, swap to that

  data += F("<h1>");
  data += String(tkr_set->Settings.system_name.friendly);
  data += F(" LCD update</h1>");

  data += F("<br/>Updating LCD firmware from: ");
  data += lcdUrl;

  data += FPSTR(HTTP_END3);

  request->send(200, FPSTR(CONTENT_TYPE_HTML), data);

  // --------------------------------------------------------------------------
  // Start the OTA download AFTER responding (non-blocking is ideal)
  // --------------------------------------------------------------------------
  ALOG_INF(PSTR(D_LOG_NEXTION "HTTP: /lcddownload lcdFirmware=%s"), lcdUrl.c_str());

  // If you track state, set it here (optional)
  // update_in_progress = true;

  nextionOtaStartDownload(request, lcdUrl);
}


/**
 * @brief 
 * 
 */
void mNextion::webHandleLcdUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{ 

  if(update_in_progress == false) // Not previously set, and hence upload
  {
    update_in_progress = true;
    ALOG_INF(PSTR("LCDOTA: LCD update started, update_in_progress SET, short delay for async"));
    delay(100);
  }

  ALOG_INF(PSTR(D_LOG_NEXTION "Upload i=%d|len=%d|rem=%d|f=%d|tx=%d"), index, len, tftFileSize-len, final, transmitted_bytes);

  static uint32_t lcdOtaTransferred = 0;
  static uint32_t lcdOtaRemaining;
  static uint16_t lcdOtaParts;
  const uint32_t lcdOtaTimeout = 120000; // timeout for receiving new data in milliseconds
  static uint32_t lcdOtaTimer = 0;      // timer for upload timeout

  bool success = false;

  if (tftFileSize == 0)
  {

    String data2 = "";
    ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: FAILED, no filesize sent."));
    String httpHeader = FPSTR(HTTP_HEAD_START);
    httpHeader.replace("{v}", String(tkr_set->Settings.system_name.friendly) + " LCD update error");

    data2 += httpHeader;
    data2 += FPSTR(HTTP_SCRIPT3);
    data2 += FPSTR(HTTP_STYLE3);
    data2 += FPSTR(HASP_STYLE);
    data2 += (F("<meta http-equiv='refresh' content='5;url=/firmware' />"));
    data2 += FPSTR(HTTP_HEAD_END3);

    data2 += (F("<h1>"));
    data2 += String(tkr_set->Settings.system_name.friendly);
    data2 += (F(" LCD update FAILED</h1>"));
    data2 += (F("No update file size reported. You must use a modern browser with Javascript enabled."));
    
    data2 += FPSTR(HTTP_END3);
  
    request->send(200, "text/html", data2);

    return;
  }
  
   
  // if ((lcdOtaTimer > 0) && ((millis() - lcdOtaTimer) > lcdOtaTimeout))
  // { // Our timer expired so reset
  //   ALOG_INF(PSTR("LCDOTA: ERROR: LCD upload timeout.  Restarting."));
  //   tkr_wifi->EspRestart();
  // }
  // else if (upload.status == UPLOAD_FILE_START)
  


  if(index == 0) // First byte of file, initialise the start of the upload
  {

    Serial.println("UPLOAD_FILE_START");  
    // ALOG_INF(PSTR("(upload.status == UPLOAD_FILE_START)"));
    // logmessage = "Upload Start: " + String(filename);    
    // Serial.println(logmessage);
    // ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Attempting firmware upload"));
    // ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: TFTfileSize: %d"), tftFileSize);

    // WiFiUDP::stopAll(); // Stop UDP to prevent any issues with OTA

    lcdOtaRemaining = tftFileSize;
    lcdOtaParts = (lcdOtaRemaining / 4096) + 1;
    ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: File upload beginning. Size %d bytes in %d 4k chunks"), lcdOtaRemaining, lcdOtaParts);

    transmitted_bytes = 0;

    // display->begin(115200); delay(200);
    // while (display->available() > 0) { display->read(); } // Clear out any old data
    
    display->write(nextionSuffix, sizeof(nextionSuffix)); // Send empty command to LCD
    display->flush();
    nextionHandleInput();

    String lcdOtaNextionCmd = "whmi-wri " + String(tftFileSize) + "," + String(ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT) + ",0";
    ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Sending LCD upload command: %s"), lcdOtaNextionCmd.c_str());
    display->print(lcdOtaNextionCmd);
    display->write(nextionSuffix, sizeof(nextionSuffix));
    display->flush();
    
    ALOG_INF(PSTR("LCDOTA: nextionOtaResponse------------------?"));

    if (nextionOtaResponse())
    {
      ALOG_INF(PSTR("LCDOTA: LCD upload command accepted"));
    }
    else
    {
      ALOG_INF(PSTR("LCDOTA: LCD upload command FAILED."));

      tkr_sup->ESP_Restart_InSeconds(5);

      // delay(5000);
      // tkr_wifi->EspRestart();
    }

    lcdOtaTimer = millis();
  }
  
  /**
   * @brief UPLOAD_FILE_WRITE
   **/
  if ( index >= 0)
  { 
    
    static int lcdOtaChunkCounter = 0;
    static uint16_t lcdOtaPartNum = 0;
    static int lcdOtaPercentComplete = 0;
    static const uint16_t lcdOtaBufferSize = 1024; // upload data buffer before sending to UART
    static uint8_t lcdOtaBuffer[lcdOtaBufferSize] = {};
    uint16_t lcdOtaUploadIndex = 0;

    int32_t lcdOtaPacketRemaining = len;//upload.currentSize;

    // Serial.printf("UPLOAD: Receiving: '%s'\n\r", filename.c_str());
    // Serial.printf( "%i bytes received.\n\r", index );
    // const char* FILESIZE_HEADER{"FileSize"};
    // Serial.printf("UPLOAD: fileSize: %s\n\r", request->header(FILESIZE_HEADER));
    // Serial.printf("UPLOAD: lcdOtaPacketRemaining: %d\n\r", lcdOtaPacketRemaining);


    while (lcdOtaPacketRemaining > 0)
    { 

      // DEBUG_LINE_HERE;
      
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
      display->flush();                              // Clear out current UART buffer
      display->write(lcdOtaBuffer, lcdOtaChunkSize); // And send the most recent data

      // ALOG_INF(PSTR("Sending lcdOtaBuffer %d\t\ttransmitted_bytes=%d"), lcdOtaBuffer[0], transmitted_bytes);

      // AddLog_Array5(LOG_LEVEL_INFO, PSTR("lcdOtaBuffer"), lcdOtaBuffer, 10);

      lcdOtaChunkCounter += lcdOtaChunkSize;
      lcdOtaTransferred += lcdOtaChunkSize;
      
      if (lcdOtaChunkCounter >= 4096) // Everytime we exceed the max buffer, check the status of the display
      {
        // DEBUG_LINE_HERE;
        display->flush();
        lcdOtaPartNum++;
        lcdOtaPercentComplete = (lcdOtaTransferred * 100) / tftFileSize;
        lcdOtaChunkCounter = 0;
        if (nextionOtaResponse())
        {
          ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Part %d OK (%d%%|%d)"), lcdOtaPartNum, lcdOtaPercentComplete, lcdOtaTransferred);
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

      // if(lcdOtaPacketRemaining==0){
      //   Serial.println("About to leave while, did I want to?");
      // }

    }


    if (lcdOtaTransferred >= tftFileSize)
    {
      ALOG_INF(PSTR("(lcdOtaTransferred >= tftFileSize) \t Total bytes should have been sent, checking completion?"));
      if (nextionOtaResponse())
      {
        ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Success, wrote %d of %d bytes"), lcdOtaTransferred, tftFileSize);
        request->redirect("/lcdOtaSuccess"); 
        // delay(10);
        // tkr_wifi->EspRestart();
        success = true;
      ALOG_INF(PSTR("update_in_progress CLEARED"));
        update_in_progress = false;
      }
      else
      {
        ALOG_INF(PSTR("LCDOTA: Failure"));
        request->redirect("/lcdOtaFailure"); 
        // delay(10);
        // tkr_wifi->EspRestart();
      }
    }
    lcdOtaTimer = millis();
  }

  /******************************************************************
  ***  UPLOAD_FILE_END)  
  ******************************************************************/
  if (final && !success)
  {

    Serial.println("upload.status == UPLOAD_FILE_END");
    if (lcdOtaTransferred >= tftFileSize)
    {
      if (nextionOtaResponse())
      { 
        ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Success, wrote %d of %d bytes"), lcdOtaTransferred, tftFileSize);
        request->redirect("/lcdOtaSuccess"); 
        ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Delay to allow LCD to update"));
      }
      else
      {
        ALOG_INF(PSTR("LCDOTA: Failure"));
        request->redirect("/lcdOtaFailure");
      }
      tkr_sup->ESP_Restart_Safe(); // Change later to schedule a restart
    }
  }

}


void mNextion::nextionOtaStartDownload(AsyncWebServerRequest *request, const String &lcdOtaUrl)
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
    ALOG_INF(PSTR("LCDOTA: Attempting firmware update from HTTPS host: %s"), lcdOtaUrl.c_str());

    lcdOtaHttp.begin(lcdOtaWifiSecure, lcdOtaUrl);
    lcdOtaWifiSecure.setInsecure();
    // lcdOtaWifiSecure.setBufferSizes(512, 512);
  }
  else
  {
    ALOG_INF(PSTR("LCDOTA: Attempting firmware update from HTTP host: %s"), lcdOtaUrl.c_str());
    lcdOtaHttp.begin(lcdOtaWifi, lcdOtaUrl);
  }

  lcdOtaHttp.addHeader("Accept-Encoding","identity"); // added jan26

  int lcdOtaHttpReturn = lcdOtaHttp.GET();
  if (lcdOtaHttpReturn > 0)
  { // HTTP header has been sent and Server response header has been handled
    ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: HTTP GET return code: %d"), lcdOtaHttpReturn);
    if (lcdOtaHttpReturn == HTTP_CODE_OK)
    {                                                 // file found at server
      int32_t lcdOtaRemaining = lcdOtaHttp.getSize(); // get length of document (is -1 when Server sends no Content-Length header)
      ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: HTTP file size: %d"), lcdOtaRemaining);
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
      display->write(nextionSuffix, sizeof(nextionSuffix)); // Send empty command
      display->flush();
      nextionHandleInput();

      String lcdOtaNextionCmd = "whmi-wri " + String(lcdOtaFileSize) + "," + String(ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT) + ",0";
      ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Sending LCD upload command: %s"), lcdOtaNextionCmd);

      display->print(lcdOtaNextionCmd);
      display->write(nextionSuffix, sizeof(nextionSuffix));
      display->flush();

      if (nextionOtaResponse())
      {
        ALOG_INF(PSTR("LCDOTA: LCD upload command accepted."));
      }
      else
      {
        ALOG_INF(PSTR("LCDOTA: LCD upload command FAILED.  Restarting device."));
        // tkr_wifi->EspRestart();
        tkr_sup->ESP_Restart_Safe();
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
          display->flush();                              // make sure any previous writes the UART have completed
          display->write(lcdOtaBuffer, lcdOtaChunkSize); // now send buffer to the UART
          lcdOtaChunkCounter += lcdOtaChunkSize;
          if (lcdOtaChunkCounter >= 4096)
          {
            display->flush();
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
              // delay(2000); // extra delay while the LCD does its thing
              // tkr_wifi->EspRestart();
              tkr_sup->ESP_Restart_Safe();
            }
          }
          else
          {
            delay(20); //check these delays 
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
          // tkr_wifi->EspRestart();
          
        tkr_sup->ESP_Restart_Safe();
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
          // request->handleClient();
          yield();
        }
        // tkr_wifi->EspRestart();
        tkr_sup->ESP_Restart_Safe();
      }
      else
      {
        ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Failure, lcdOtaTransferred: %d lcdOtaFileSize: %d"), lcdOtaTransferred, lcdOtaFileSize);
        // tkr_wifi->EspRestart();
        tkr_sup->ESP_Restart_Safe();
      }
    }
  }
  else
  {
    ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: HTTP GET failed, error code %s"), lcdOtaHttp.errorToString(lcdOtaHttpReturn));
    // tkr_wifi->EspRestart();
    
        tkr_sup->ESP_Restart_Safe();
  }
  lcdOtaHttp.end();
}


void mNextion::webHandleTftFileSize(AsyncWebServerRequest* request)
{
  // GET /tftFileSize?tftFileSize=<bytes>

  if (!request) return;

  // Parse first, then respond (so logging reflects reality)
  uint32_t new_size = 0;

  if (request->hasArg(F("tftFileSize")))
  {
    // toInt() returns long; clamp to uint32_t
    long v = request->arg(F("tftFileSize")).toInt();
    if (v > 0) new_size = (uint32_t)v;
  }

  tftFileSize = new_size;

  ALOG_INF(PSTR(D_LOG_NEXTION "Received tftFileSize: %lu"), (unsigned long)tftFileSize);

  // Minimal response: No Content
  request->send(204);
}


bool mNextion::nextionOtaResponse()
{                                               // Monitor the serial port for a 0x05 response within our timeout
  unsigned long nextionCommandTimeout = 2000;   // timeout for receiving termination string in milliseconds
  unsigned long nextionCommandTimer = millis(); // record current time for our timeout
  bool otaSuccessVal = false;
  while ((millis() - nextionCommandTimer) < nextionCommandTimeout)
  {
    if (display->available())
    {
      byte inByte = display->read();
      // Serial.println(inByte, HEX);
      if (inByte == 0x5)
      {
        otaSuccessVal = true;
        break;
      }
    }
    else
    {
      delay(1);
    }
  }

  if(!otaSuccessVal){
    AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_NEXTION "QUIT OTA and NOOOOOOOOOT Restart display"));
    // nextionReset();
  }


  return otaSuccessVal;
}




#endif  // USE_MODULE_DISPLAYS_NEXTION