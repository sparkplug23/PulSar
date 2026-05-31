/**
 * @file mUpdates.cpp
 * @author your name (you@domain.com)
 * @brief Core update / OTA module
 * @version 0.2
 * @date 2026-05-21
 * 
 * @copyright Copyright (c) 2023
 * 
 * DESCRIPTION:
 *   Main entry point for system update functions.
 *
 *   Critical firmware update route:
 *     GET  /update -> serves embedded PAGE_update_web
 *     POST /update -> receives firmware .bin and flashes device
 *
 *   Notes:
 *     - /update is intentionally not routed through shared settings hooks.
 *     - /common.js is already served by mWebServer.
 *     - update_web.htm must be generated as PAGE_update_web.
 */

#include "mUpdates.h"

#ifdef USE_MODULE_CORE_UPDATES


#if defined(ESP32)
  #include <Update.h>
#elif defined(ESP8266)
  #include <Updater.h>
#endif



#if defined(ESP32)

  #include <WiFi.h>
  #include <WiFiClient.h>
  #include <WebServer.h>
  #include <ESPmDNS.h>
  #include <WiFiUdp.h>
  #include <Update.h>
  #include <ArduinoOTA.h>

#elif defined(ESP8266)

  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
  #include <WiFiUdp.h>
  #include <ESP8266WebServer.h>
  #include <Updater.h>
  #include <ArduinoOTA.h>

#endif


#if defined(ESP8266) || defined(ESP32)

/************************************************************************************************
 * SECTION: SafeMode OTA recovery
 *
 * This block must remain independent of:
 * - normal webserver module,
 * - filesystem,
 * - saved settings,
 * - full WebUI assets.
 *
 * It provides:
 * - STA + AP recovery WiFi,
 * - minimal HTTP firmware upload page at /update,
 * - ArduinoOTA support.
 ************************************************************************************************/

static const char* host     = "recovery";
static const char* ssid     = STA_SSID1;
static const char* password = STA_PASS1;


#ifdef USE_MODULE_UPDATES__WEBPAGE_RECOVERY

#if defined(ESP32)
static WebServer* http_safemode_server = nullptr;
#elif defined(ESP8266)
// static ESP8266WebServer* http_safemode_server = nullptr;
static WebServer* http_safemode_server = nullptr;
#endif


static const char PAGE_update_minimal[] PROGMEM =
"<!doctype html><html><head>"
"<meta name='viewport' content='width=device-width,initial-scale=1'>"
"<title>Firmware Update</title>"
"<style>"
"body{font-family:Arial,sans-serif;background:#111;color:#eee;margin:20px;}"
".box{max-width:520px;margin:auto;background:#1c1c1c;padding:18px;border-radius:8px;}"
"input,button{width:100%;box-sizing:border-box;margin-top:12px;padding:10px;}"
"button{background:#2d6cdf;color:white;border:0;border-radius:4px;}"
"</style>"
"</head><body><div class='box'>"
"<h2>Firmware Update</h2>"
"<form method='POST' action='/update' enctype='multipart/form-data'>"
"<input type='file' name='update' required>"
"<button type='submit'>Upload firmware</button>"
"</form>"
"<p>Use this page for recovery firmware upload.</p>"
"</div></body></html>";

#endif // USE_MODULE_UPDATES__WEBPAGE_RECOVERY


/************************************************************************************************
 * FUNCTION: SafeMode_StartAndAwaitOTA
 *
 * SafeMode recovery behaviour:
 * - Starts AP+STA mode.
 * - AP is always started, so recovery is available even if STA fails.
 * - STA attempts to connect to the configured fallback network.
 * - Minimal HTTP firmware upload is exposed at /update.
 * - ArduinoOTA is also started.
 *
 * seconds_to_wait:
 * - 0 = wait indefinitely.
 * - N = wait N seconds, then return to caller.
 ************************************************************************************************/
void SafeMode_StartAndAwaitOTA(uint8_t seconds_to_wait)
{
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("SafeMode_StartAndAwaitOTA"));

  uint32_t tSaved_heartbeat = millis();

  /********************************************************************************************
   ** WiFi: AP + STA recovery *****************************************************************
   *
   * AP:
   *   Always started, so recovery is possible even when STA fails.
   *
   * STA:
   *   Attempts to join the known fallback network.
   *
   * Recovery URLs:
   *   AP:  http://192.168.4.1/update
   *   STA: http://<router-ip>/update
  ********************************************************************************************/

  const char* recovery_ap_ssid = "PulSar-Recovery";
  const char* recovery_ap_pass = "PulSarOTA";   // WPA AP password must be 8+ chars

  WiFi.mode(WIFI_AP_STA);

  #ifdef ESP8266
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  #endif

  #ifdef ESP32
  WiFi.setSleep(false);
  #endif

  bool ap_ok = WiFi.softAP(recovery_ap_ssid, recovery_ap_pass);

  Serial.print(F("Recovery AP: "));
  Serial.println(ap_ok ? F("started") : F("failed"));

  Serial.print(F("Recovery AP SSID: "));
  Serial.println(recovery_ap_ssid);

  Serial.print(F("Recovery AP IP: "));
  Serial.println(WiFi.softAPIP());

  WiFi.begin(ssid, password);

  Serial.print(F("SafeMode STA connecting"));

  const uint32_t sta_connect_start      = millis();
  const uint32_t sta_connect_timeout_ms = 15000UL;

  while ((WiFi.status() != WL_CONNECTED) &&
         ((millis() - sta_connect_start) < sta_connect_timeout_ms))
  {
    delay(250);
    Serial.print('.');

    #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
    WDT_Reset();
    #endif
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print(F("STA connected to "));
    Serial.println(ssid);

    Serial.print(F("STA IP: "));
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println(F("STA not connected. AP recovery remains active."));
  }


  /********************************************************************************************
   ** mDNS ************************************************************************************
   *
   * mDNS is only useful on the STA/LAN side.
   * AP recovery should use the AP IP directly.
  ********************************************************************************************/

  if (WiFi.status() == WL_CONNECTED)
  {
    if (MDNS.begin(host))
    {
      Serial.print(F("mDNS responder started: http://"));
      Serial.print(host);
      Serial.println(F(".local/update"));
    }
    else
    {
      Serial.println(F("mDNS responder failed"));
    }
  }


  /********************************************************************************************
   ** Minimal HTTP firmware upload server *****************************************************
   *
   * GET  /        -> minimal upload page
   * GET  /update  -> minimal upload page
   * POST /update  -> firmware upload
   *
   * Do not use HTTP_GET here. In this project it can resolve to the AsyncWebServer
   * WebRequestMethod enum, which does not match ESP8266WebServer::on().
  ********************************************************************************************/

  #ifdef USE_MODULE_UPDATES__WEBPAGE_RECOVERY

  if (http_safemode_server == nullptr)
  {
    #if defined(ESP32)
    http_safemode_server = new WebServer(80);
    #elif defined(ESP8266)
    http_safemode_server = new ESP8266WebServer(80);
    #endif
  }

  http_safemode_server->on("/", []()
  {
    http_safemode_server->sendHeader(F("Connection"), F("close"));
    http_safemode_server->send_P(200, PSTR("text/html"), PAGE_update_minimal);
  });

  http_safemode_server->on("/update", []()
  {
    http_safemode_server->sendHeader(F("Connection"), F("close"));
    http_safemode_server->send_P(200, PSTR("text/html"), PAGE_update_minimal);
  });

  http_safemode_server->on(
    "/update",
    static_cast<HTTPMethod>(HTTP_POST),
    []()
    {
      const bool ok = !Update.hasError();

      http_safemode_server->sendHeader(F("Connection"), F("close"));
      http_safemode_server->send(
        ok ? 200 : 500,
        F("text/plain"),
        ok ? F("OK. Rebooting.") : F("FAIL")
      );

      if (ok)
      {
        // RtcMemory__BootState_Reset();
        delay(250);
        ESP.restart();
      }
    },
    []()
    {
      #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
      WDT_Reset();
      #endif

      auto& upload = http_safemode_server->upload();

      if (upload.status == UPLOAD_FILE_START)
      {
        Serial.printf("HTTP OTA start: %s\r\n", upload.filename.c_str());

        #ifdef ESP8266

        Update.runAsync(true);

        const uint32_t max_sketch_space =
          (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;

        if (!Update.begin(max_sketch_space, U_FLASH))
        {
          Serial.printf(
            "HTTP OTA begin failed, max=%u err=%u\r\n",
            max_sketch_space,
            Update.getError()
          );
          Update.printError(Serial);
        }

        #elif defined(ESP32)

        if (!Update.begin(UPDATE_SIZE_UNKNOWN, U_FLASH))
        {
          Serial.printf("HTTP OTA begin failed, err=%u\r\n", Update.getError());
          Update.printError(Serial);
        }

        #endif
      }
      else if (upload.status == UPLOAD_FILE_WRITE)
      {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
        {
          Serial.printf("HTTP OTA write failed, err=%u\r\n", Update.getError());
          Update.printError(Serial);
        }
      }
      else if (upload.status == UPLOAD_FILE_END)
      {
        if (Update.end(true))
        {
          Serial.printf("HTTP OTA success, size=%u\r\n", upload.totalSize);
        }
        else
        {
          Serial.printf("HTTP OTA end failed, err=%u\r\n", Update.getError());
          Update.printError(Serial);
        }
      }
      else if (upload.status == UPLOAD_FILE_ABORTED)
      {
        Update.end();
        Serial.println(F("HTTP OTA aborted"));
      }
    }
  );

  http_safemode_server->begin();

  Serial.println(F("SafeMode HTTP updater started"));

  Serial.print(F("Recovery URL AP:  http://"));
  Serial.print(WiFi.softAPIP());
  Serial.println(F("/update"));

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print(F("Recovery URL STA: http://"));
    Serial.print(WiFi.localIP());
    Serial.println(F("/update"));
  }

  #endif


  /********************************************************************************************
   ** ArduinoOTA ******************************************************************************
  ********************************************************************************************/

  ArduinoOTA.setHostname(host);

  ArduinoOTA.onStart([]()
  {
    Serial.println(F("ArduinoOTA start"));
  });

  ArduinoOTA.onEnd([]()
  {
    Serial.println(F("ArduinoOTA end"));
    // RtcMemory__BootState_Reset();
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
  {
    if (total > 0)
    {
      Serial.printf("ArduinoOTA progress: %u%%\r", (progress * 100U) / total);
    }

    #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
    WDT_Reset();
    #endif
  });

  ArduinoOTA.onError([](ota_error_t error)
  {
    Serial.printf("ArduinoOTA error[%u]: ", error);

    if (error == OTA_AUTH_ERROR) {
      Serial.println(F("Auth failed"));
    }
    else if (error == OTA_BEGIN_ERROR) {
      Serial.println(F("Begin failed"));
    }
    else if (error == OTA_CONNECT_ERROR) {
      Serial.println(F("Connect failed"));
    }
    else if (error == OTA_RECEIVE_ERROR) {
      Serial.println(F("Receive failed"));
    }
    else if (error == OTA_END_ERROR) {
      Serial.println(F("End failed"));
    }
    else {
      Serial.println(F("Unknown"));
    }
  });

  ArduinoOTA.begin();

  Serial.println(F("SafeMode ArduinoOTA started"));


  /********************************************************************************************
   ** Blocking recovery loop ******************************************************************
  ********************************************************************************************/

  Serial.printf(
    "SafeMode awaiting OTA/HTTP recovery for %u seconds\r\n",
    seconds_to_wait
  );

  const uint32_t tStart = millis();

  while (true)
  {
    #ifdef USE_MODULE_UPDATES__WEBPAGE_RECOVERY
    http_safemode_server->handleClient();
    #endif
    ArduinoOTA.handle();

    #ifdef ESP8266
    if (WiFi.status() == WL_CONNECTED)
    {
      MDNS.update();
    }
    #endif

    if (seconds_to_wait > 0)
    {
      if ((millis() - tStart) > ((uint32_t)seconds_to_wait * 1000UL))
      {
        break;
      }
    }

    if ((millis() - tSaved_heartbeat) > 1000UL)
    {
      tSaved_heartbeat = millis();

      Serial.println(F("Waiting for recovery"));

      Serial.print(F("AP  URL: http://"));
      Serial.print(WiFi.softAPIP());
      Serial.println(F("/update"));

      if (WiFi.status() == WL_CONNECTED)
      {
        Serial.print(F("STA URL: http://"));
        Serial.print(WiFi.localIP());
        Serial.println(F("/update"));
      }
      else
      {
        Serial.println(F("STA: not connected"));
      }

      #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
      WDT_Reset();
      #endif
    }

    delay(1);
  }
}

#endif // defined(ESP8266) || defined(ESP32)












#ifdef USE_ARDUINO_OTA

/*********************************************************************************************\
 * Allow updating via the Arduino OTA-protocol.
 *
 * - Once started disables current wifi clients and udp
 * - Perform restart when done to re-init wifi clients
\*********************************************************************************************/

void mUpdates::ArduinoOTAInit(void)
{

  if(ota_init_success){ return; }

  ArduinoOTA.setHostname(tkr_set->runtime.my_hostname);
  
  ArduinoOTA.onStart([this]()
  {
    #ifdef ENABLE_DEVFEATURE_ARDUINOOTA__ADVANCED
      tkr_set->SettingsSave(1);    // Free flash for OTA update
      #ifdef USE_MODULE_NETWORK_WEBSERVER
        if (tkr_set->Settings.webserver) { tkr_web->StopWebserver(); }
      #endif  // USE_MODULE_NETWORK_WEBSERVER
      AllowInterrupts(0);
      if (tkr_set->Settings.sysopt_system.bit.mqtt_enabled) {
        MqttDisconnect();      // SetOption3  - Enable MQTT
      }
    #endif

    ALOG_IMP(PSTR(D_LOG_UPLOAD "OTA " D_UPLOAD_STARTED));

    arduino_ota_triggered = true;
    arduino_ota_progress_dot_count = 0;

    #ifdef ENABLE_FEATURE_RTC__SETTINGS
    RtcMemory__RuntimeState.boot_was_completed_ota_event = false;
    #endif

    // Disable parts (e.g. RF receive interrupts) before starting update
    tkr->Tasker_Interface(TASK_UPDATE_OTA_BEFORE_ON_START);

    tkr_set->Settings.logging.serial_level = LOG_LEVEL_NONE; // Disable serial logging
    tkr_set->Settings.logging.web_level = LOG_LEVEL_NONE; // Disable web logging
    
    delay(200);       // Allow time for message xfer
  });


  ArduinoOTA.onProgress([this](unsigned int progress, unsigned int total)
  {
    if (tkr_set->runtime.seriallog_level >= LOG_LEVEL_DEBUG) { // for when hardware serial is in use for modules
      uint8_t progress_now = (progress/(total/100));
      if(arduino_ota_progress_dot_count != progress_now){
        arduino_ota_progress_dot_count = progress_now;
        Serial.printf("Progress: %u%%\r", progress_now); // using return, not newline, to overwrite the line instead of spamming new lines
      }
    }

    #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
    WDT_Reset();
    #endif
    
  });


  ArduinoOTA.onError([this](ota_error_t error)
  {
    /*
    From ArduinoOTA.h:
    typedef enum { OTA_AUTH_ERROR, OTA_BEGIN_ERROR, OTA_CONNECT_ERROR, OTA_RECEIVE_ERROR, OTA_END_ERROR } ota_error_t;
    */
    char error_str[30];
    memset(error_str,0,sizeof(error_str));

    switch (error) {
      case OTA_AUTH_ERROR:    strncpy_P(error_str, PSTR("OTA_AUTH_ERROR"), sizeof(error_str)); break;    
      case OTA_BEGIN_ERROR:   strncpy_P(error_str, PSTR(D_UPLOAD_ERR_2), sizeof(error_str)); break;
      case OTA_CONNECT_ERROR: sprintf(error_str, PSTR("Connect Error")); break;
      case OTA_RECEIVE_ERROR: strncpy_P(error_str, PSTR(D_UPLOAD_ERR_5), sizeof(error_str)); break;
      case OTA_END_ERROR:     strncpy_P(error_str, PSTR(D_UPLOAD_ERR_7), sizeof(error_str)); break;
      default:
        snprintf_P(error_str, sizeof(error_str), PSTR(D_UPLOAD_ERROR_CODE " %d"), error);
    }

    ALOG_IMP(PSTR(D_LOG_OTA "Arduino OTA  %s. %d " D_RESTARTING), error_str,ESP.getFreeSketchSpace());

    #ifdef ENABLE_DEVFEATURE_OTA__ENABLE_RECORD_BOOTREASON_IS_OTA
    RtcMemory__RuntimeState.boot_was_completed_ota_event = false; // Reset the flag as an error has occured
    #endif


    if(error != OTA_BEGIN_ERROR)
      ESP.restart(); // Should only reach if the first failed

  });

  ArduinoOTA.onEnd([this]()
  {
    #ifdef ENABLE_DEVFEATURE_OTA__ENABLE_RECORD_BOOTREASON_IS_OTA
    RtcMemory__RuntimeState.boot_was_completed_ota_event = true; // To enable skip of delayed WiFi start when this individual device has succesful flash
        
    if(ResetReasonPowerOn()) // Only if we can trust stability from a good restart
    {
      ALOG_INF(PSTR("Previous start was safe, so RTCSettingsSave trusted"));
      RtcMemory__RuntimeState_Save();
    }
    #endif // ENABLE_DEVFEATURE_OTA__ENABLE_RECORD_BOOTREASON_IS_OTA

    delay(150); // Allow time for the OTA success response to reach the host

    ALOG_IMP(PSTR(D_LOG_UPLOAD "OTA " D_SUCCESSFUL ". " D_RESTARTING));
    Serial.flush();
    ESP.restart();
	});

  ArduinoOTA.begin();
  ota_init_success = true;

  ALOG_IMP(PSTR(D_LOG_OTA "Started"));

}


void mUpdates::ArduinoOtaLoop(void)
{

  #ifdef ESP8266
  MDNS.update();
  #endif
  // if(!ota_init_success){ Serial.println("block OTA"); return; }
  ArduinoOTA.handle();
  // Once OTA is triggered, only handle that and dont do other stuff. (otherwise it fails)
  // Note async stuff can still occur, so I need to disable them
  while (arduino_ota_triggered){ 
    ArduinoOTA.handle(); 
    delay(0); // yield() is ESP8266 specific, delay(0) works for both ESP32 and ESP8266
  }
}

#endif  // USE_ARDUINO_OTA





void mUpdates::Init(void)
{
}

void mUpdates::Pre_Init()
{
}

int8_t mUpdates::Tasker(uint8_t function, JsonParserObject obj)
{
  /************
   * INIT SECTION * 
  *******************/
  if(function == TASK_PRE_INIT){
    Pre_Init();
  }else
  if(function == TASK_INIT){
    Init();
  }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:
      #ifdef USE_MODULE_NETWORK_WEBSERVER
      if(update_reboot_pending && millis() > update_reboot_at_ms){
        ALOG_INF(PSTR("OTA update reboot"));
        delay(100);
        ESP.restart();
      }
      #endif
    break;
    case TASK_NETWORK_CONNECTED__WIFI:
    case TASK_NETWORK_CONNECTED__ETHERNET:
    case TASK_NETWORK_CONNECTED__CELLULAR:
      ArduinoOTAInit();
    break;
    case TASK_EVERY_50_MSECOND:
      #ifdef USE_ARDUINO_OTA
        if(!tkr_set->runtime.global_state.network_down)
          ArduinoOtaLoop();
      #endif
    break;
    case TASK_EVERY_FIVE_MINUTE:
      // Run OTA without network check, in case we are in unknown state to allow recovery
      #ifdef USE_ARDUINO_OTA
        // ArduinoOtaLoop();
      #endif
    break;

    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * WEBSERVER SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case TASK_WEB_ADD_HANDLER:
      WebPage_Root_AddHandlers();
    break;
    #endif // USE_MODULE_NETWORK_WEBSERVER
  }

  return TASKER_RESULT__UNKNOWN_ID;

} // END Tasker

#ifdef USE_MODULE_NETWORK_WEBSERVER

void mUpdates::WebPage_Root_AddHandlers()
{
  #ifndef WLED_DISABLE_OTA

  tkr_web->server->on("/update", HTTP_GET, [this](AsyncWebServerRequest *request){
    ALOG_INF(PSTR("URL HTTP_GET \"/update\""));

    AsyncWebServerResponse* response = request->beginResponse_P(
      200,
      FPSTR(CONTENT_TYPE_HTML),
      PAGE_welcome_web2,
      PAGE_welcome_web2_length
    );

    response->addHeader(F("Content-Encoding"), F("gzip"));
    request->send(response);
  });

  tkr_web->server->on("/update", HTTP_POST, [this](AsyncWebServerRequest *request){
    ALOG_INF(PSTR("URL HTTP_POST \"/update\""));

    if(Update.hasError() || update_upload_failed){
      ALOG_ERR(PSTR("OTA update failed, error=%u"), Update.getError());
      tkr_web->serveMessage(request, 500, F("Update failed!"), F("Please check your firmware binary and retry."), 254);
      return;
    }

    update_reboot_pending = true;
    update_reboot_at_ms = millis() + 1000;

    tkr_web->serveMessage(request, 200, F("Update successful!"), F("Rebooting..."), 131);
  },[this](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
    this->HandleFirmwareUpload(request, filename, index, data, len, final);
  });

  #else

  tkr_web->server->on("/update", HTTP_GET, [this](AsyncWebServerRequest *request){
    tkr_web->serveMessage(request, 501, F("Not implemented"), F("OTA updating is disabled in this build."), 254);
  });

  tkr_web->server->on("/update", HTTP_POST, [this](AsyncWebServerRequest *request){
    tkr_web->serveMessage(request, 501, F("Not implemented"), F("OTA updating is disabled in this build."), 254);
  });

  #endif // WLED_DISABLE_OTA
}

void mUpdates::HandleFirmwareUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{

  if(!index){
    update_upload_started = true;
    update_upload_failed = false;

    ALOG_INF(PSTR("OTA update start: %s"), filename.c_str());

    #ifdef ESP8266
    Update.runAsync(true);
    const uint32_t max_sketch_space = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
    if(!Update.begin(max_sketch_space, U_FLASH)){
      update_upload_failed = true;
      ALOG_ERR(PSTR("OTA Update.begin failed, error=%u"), Update.getError());
      return;
    }
    #elif defined(ESP32)
    if(!Update.begin(UPDATE_SIZE_UNKNOWN, U_FLASH)){
      update_upload_failed = true;
      ALOG_ERR(PSTR("OTA Update.begin failed, error=%u"), Update.getError());
      return;
    }
    #else
    update_upload_failed = true;
    return;
    #endif
  }

  if(update_upload_failed || Update.hasError()){
    return;
  }

  if(len){
    const size_t written = Update.write(data, len);

    if(written != len){
      update_upload_failed = true;
      ALOG_ERR(PSTR("OTA Update.write failed, written=%u len=%u error=%u"), (unsigned)written, (unsigned)len, Update.getError());
      return;
    }
  }

  if(final){
    if(!update_upload_started){
      update_upload_failed = true;
      ALOG_ERR(PSTR("OTA upload final received without upload start"));
      return;
    }

    if(Update.end(true)){
      ALOG_INF(PSTR("OTA update success, size=%u"), (unsigned)(index + len));
    }else{
      update_upload_failed = true;
      ALOG_ERR(PSTR("OTA Update.end failed, error=%u"), Update.getError());
    }

    update_upload_started = false;
  }
}

#endif // USE_MODULE_NETWORK_WEBSERVER

void mUpdates::parse_JSONCommand(JsonParserObject obj)
{
  
}


#endif // USE_MODULE_CORE_UPDATES