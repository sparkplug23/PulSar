/**
 * @file mUpdates.cpp
 * @brief Core update / OTA module
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

static void OTA_WiFiQuickConnect_Save(void)
{
  #if defined(ENABLE_FEATURE_WIFI__SSID_QUICK_CONNECT_AFTER_OTA) && defined(ENABLE_FEATURE_RTC__SETTINGS) && defined(USE_MODULE_NETWORK_WIFI)
  if(tkr_wifi)
  {
    tkr_wifi->WiFi_QuickConnect_SaveToRTC();
  }
  #endif
}


static void OTA_WiFiQuickConnect_Clear(void)
{
  #if defined(ENABLE_FEATURE_WIFI__SSID_QUICK_CONNECT_AFTER_OTA) && defined(ENABLE_FEATURE_RTC__SETTINGS)
  RtcMemory__WiFiQuickConnect_Clear();
  #endif
}



#if defined(ESP8266) || defined(ESP32)

static const char* host     = "recovery";
static const char* ssid     = STA_SSID1;
static const char* password = STA_PASS1;

#if defined(USE_MODULE_UPDATES__WEBPAGE_RECOVERY) && !defined(USE_MODULE_NETWORK_WEBSERVER)

  #if defined(ESP32)
    static WebServer* http_safemode_server = nullptr;
  #elif defined(ESP8266)
    static ESP8266WebServer* http_safemode_server = nullptr;
  #endif

  static const char PAGE_update_minimal[] PROGMEM =
  "<!doctype html><html><head>"
  "<meta name='viewport' content='width=device-width,initial-scale=1'>"
  "<title>Update</title>"
  "<style>"
  "body{font-family:Arial;background:#111;color:#eee;margin:20px}"
  ".box{max-width:520px;margin:auto;background:#222;padding:18px;border-radius:8px}"
  "input,button{width:100%;box-sizing:border-box;margin-top:12px;padding:10px}"
  "button{background:#333;color:#fff;border:0;border-radius:4px}"
  "</style>"
  "</head><body><div class='box'>"
  "<h2>Firmware Update</h2>"
  "<form method='POST' action='/update' enctype='multipart/form-data'>"
  "<input type='file' name='update' required>"
  "<button type='submit'>Upload</button>"
  "</form>"
  "</div></body></html>";

#endif


void SafeMode_StartAndAwaitOTA(uint8_t seconds_to_wait)
{
  Serial.begin(115200);

  uint32_t tSaved_heartbeat = millis();

  const char* recovery_ap_ssid = "PulSar-Recovery";
  const char* recovery_ap_pass = "PulSarOTA";

  WiFi.mode(WIFI_AP_STA);

  #ifdef ESP8266
    WiFi.setSleepMode(WIFI_NONE_SLEEP);
  #endif

  #ifdef ESP32
    WiFi.setSleep(false);
  #endif

  const bool ap_ok = WiFi.softAP(recovery_ap_ssid, recovery_ap_pass);

  Serial.print(F("REC AP "));
  Serial.println(ap_ok ? F("OK") : F("FAIL"));
  Serial.println(WiFi.softAPIP());

  WiFi.begin(ssid, password);

  const uint32_t sta_connect_start = millis();
  const uint32_t sta_connect_timeout_ms = 15000UL;

  while ((WiFi.status() != WL_CONNECTED) && ((millis() - sta_connect_start) < sta_connect_timeout_ms))
  {
    delay(250);

    #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
      WDT_Reset();
    #endif
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print(F("STA "));
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println(F("STA FAIL"));
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    if (!MDNS.begin(host)) Serial.println(F("MDNS FAIL"));
  }


  #if defined(USE_MODULE_UPDATES__WEBPAGE_RECOVERY) && !defined(USE_MODULE_NETWORK_WEBSERVER)

    if (http_safemode_server == nullptr)
    {
      #if defined(ESP32)
        http_safemode_server = new WebServer(80);
      #elif defined(ESP8266)
        http_safemode_server = new ESP8266WebServer(80);
      #endif
    }

    http_safemode_server->on("/", [](){
      http_safemode_server->sendHeader(F("Connection"), F("close"));
      http_safemode_server->send_P(200, PSTR("text/html"), PAGE_update_minimal);
    });

    http_safemode_server->on("/update", [](){
      http_safemode_server->sendHeader(F("Connection"), F("close"));
      http_safemode_server->send_P(200, PSTR("text/html"), PAGE_update_minimal);
    });

    http_safemode_server->on("/update", static_cast<HTTPMethod>(HTTP_POST),
      [](){
        const bool ok = !Update.hasError();
        http_safemode_server->sendHeader(F("Connection"), F("close"));
        http_safemode_server->send(ok ? 200 : 500, F("text/plain"), ok ? F("OK") : F("FAIL"));

        if (ok)
        {
          delay(250);
          ESP.restart();
        }
      },
      [](){
        #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
          WDT_Reset();
        #endif

        auto& upload = http_safemode_server->upload();

        if (upload.status == UPLOAD_FILE_START)
        {
          Serial.println(F("OTA START"));

          #ifdef ESP8266
            Update.runAsync(true);
            const uint32_t max_sketch_space = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
            if (!Update.begin(max_sketch_space, U_FLASH))
            {
              Serial.println(F("OTA BEGIN FAIL"));
              Update.printError(Serial);
            }
          #elif defined(ESP32)
            if (!Update.begin(UPDATE_SIZE_UNKNOWN, U_FLASH))
            {
              Serial.println(F("OTA BEGIN FAIL"));
              Update.printError(Serial);
            }
          #endif
        }
        else if (upload.status == UPLOAD_FILE_WRITE)
        {
          if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
          {
            Serial.println(F("OTA WRITE FAIL"));
            Update.printError(Serial);
          }
        }
        else if (upload.status == UPLOAD_FILE_END)
        {
          if (Update.end(true)) Serial.println(F("OTA OK"));
          else
          {
            Serial.println(F("OTA END FAIL"));
            Update.printError(Serial);
          }
        }
        else if (upload.status == UPLOAD_FILE_ABORTED)
        {
          Update.end();
          Serial.println(F("OTA ABORT"));
        }
      }
    );

    http_safemode_server->begin();

  #endif


  ArduinoOTA.setHostname(host);

  ArduinoOTA.onStart([](){
    Serial.println(F("AOTA START"));
  });

  ArduinoOTA.onEnd([](){
    #ifdef ENABLE_FEATURE_FASTBOOT__DETECTION
      RtcMemory__BootState.fast_reboot_count = 0;
      RtcMemory__BootState_Save();
    #endif

    Serial.println(F("AOTA OK"));
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total){
    #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
      WDT_Reset();
    #endif

    if (total)
    {
      static uint8_t last = 255;
      const uint8_t now = (progress * 100U) / total;

      if (now != last && !(now % 10))
      {
        last = now;
        Serial.printf("%u%%\r", now);
      }
    }
  });

  ArduinoOTA.onError([](ota_error_t error){
    Serial.printf("AOTA ERR %u\r\n", error);
  });

  ArduinoOTA.begin();

  Serial.println(F("REC READY"));

  const uint32_t tStart = millis();

  while (true)
  {
    #if defined(USE_MODULE_UPDATES__WEBPAGE_RECOVERY) && !defined(USE_MODULE_NETWORK_WEBSERVER)
      http_safemode_server->handleClient();
    #endif

    ArduinoOTA.handle();

    #ifdef ESP8266
      if (WiFi.status() == WL_CONNECTED) MDNS.update();
    #endif

    if (seconds_to_wait > 0 && (millis() - tStart) > ((uint32_t)seconds_to_wait * 1000UL)) break;

    if ((millis() - tSaved_heartbeat) > 5000UL)
    {
      tSaved_heartbeat = millis();

      Serial.print(F("REC "));
      Serial.print(WiFi.softAPIP());

      if (WiFi.status() == WL_CONNECTED)
      {
        Serial.print(' ');
        Serial.print(WiFi.localIP());
      }

      Serial.println();

      #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
        WDT_Reset();
      #endif
    }

    delay(1);
  }
}

#endif // ESP8266 || ESP32



#ifdef USE_ARDUINO_OTA

void mUpdates::ArduinoOTAInit(void)
{
  if(ota_init_success) return;

  ArduinoOTA.setHostname(tkr_set->runtime.my_hostname);

  ArduinoOTA.onStart([this](){
    #ifdef ENABLE_DEVFEATURE_ARDUINOOTA__ADVANCED
    tkr_set->SettingsSave(1);

    #ifdef USE_MODULE_NETWORK_WEBSERVER
    if(tkr_set->Settings.webserver) tkr_web->StopWebserver();
    #endif

    AllowInterrupts(0);

    if(tkr_set->Settings.sysopt_system.bit.mqtt_enabled) MqttDisconnect();
    #endif

    ALOG_IMP(PSTR(D_LOG_UPLOAD "OTA " D_UPLOAD_STARTED));

    arduino_ota_triggered = true;
    arduino_ota_progress_dot_count = 0;

    #ifdef ENABLE_FEATURE_RTC__SETTINGS
    RtcMemory__RuntimeState.boot_was_completed_ota_event = false;
    #endif

    tkr->Tasker_Interface(TASK_UPDATE_OTA_BEFORE_ON_START);

    tkr_set->Settings.logging.serial_level = LOG_LEVEL_NONE;
    tkr_set->Settings.logging.web_level = LOG_LEVEL_NONE;

    delay(200);
  });

  ArduinoOTA.onProgress([this](unsigned int progress, unsigned int total){
    if(tkr_set->runtime.seriallog_level >= LOG_LEVEL_DEBUG && total)
    {
      const uint8_t progress_now = (progress * 100U) / total;

      if(arduino_ota_progress_dot_count != progress_now)
      {
        arduino_ota_progress_dot_count = progress_now;
        Serial.printf("%u%%\r",progress_now);
      }
    }

    #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
    WDT_Reset();
    #endif
  });

  ArduinoOTA.onError([this](ota_error_t error){
    char error_str[24] = {0};

    switch(error){
      case OTA_AUTH_ERROR:    strncpy_P(error_str,PSTR("AUTH"),sizeof(error_str)); break;
      case OTA_BEGIN_ERROR:   strncpy_P(error_str,PSTR(D_UPLOAD_ERR_2),sizeof(error_str)); break;
      case OTA_CONNECT_ERROR: strncpy_P(error_str,PSTR("CONNECT"),sizeof(error_str)); break;
      case OTA_RECEIVE_ERROR: strncpy_P(error_str,PSTR(D_UPLOAD_ERR_5),sizeof(error_str)); break;
      case OTA_END_ERROR:     strncpy_P(error_str,PSTR(D_UPLOAD_ERR_7),sizeof(error_str)); break;
      default: snprintf_P(error_str,sizeof(error_str),PSTR("ERR %u"),error); break;
    }

    ALOG_IMP(PSTR(D_LOG_OTA "OTA %s"),error_str);

    #ifdef ENABLE_DEVFEATURE_OTA__ENABLE_RECORD_BOOTREASON_IS_OTA
    RtcMemory__RuntimeState.boot_was_completed_ota_event = false;
    #endif

    OTA_WiFiQuickConnect_Clear();

    if(error != OTA_BEGIN_ERROR) ESP.restart();
  });

  ArduinoOTA.onEnd([this](){
    #ifdef ENABLE_FEATURE_FASTBOOT__DETECTION
    RtcMemory__BootState.fast_reboot_count = 0;
    RtcMemory__BootState_Save();
    #endif

    #ifdef ENABLE_DEVFEATURE_OTA__ENABLE_RECORD_BOOTREASON_IS_OTA
    RtcMemory__RuntimeState.boot_was_completed_ota_event = true;

    if(ResetReasonPowerOn()) RtcMemory__RuntimeState_Save();
    #endif

    /*
     * Capture the exact WiFi association which successfully carried this OTA.
     *
     * This includes:
     *   profile
     *   BSSID
     *   channel
     *
     * RtcMemory__WiFiQuickConnect_Set() performs its own raw RTC write, so it
     * does not depend on the legacy boot-reason RTC save above.
     */
    OTA_WiFiQuickConnect_Save();

    delay(150);

    ALOG_IMP(PSTR(D_LOG_UPLOAD "OTA " D_SUCCESSFUL));

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

  ArduinoOTA.handle();

  while (arduino_ota_triggered)
  {
    ArduinoOTA.handle();
    delay(0);
  }
}

#endif // USE_ARDUINO_OTA


int8_t mUpdates::Tasker(uint8_t function, JsonParserObject obj)
{

  switch (function)
  {
    case TASK_EVERY_SECOND:
      #ifdef USE_MODULE_NETWORK_WEBSERVER
        if (update_reboot_pending && millis() > update_reboot_at_ms)
        {
          delay(100);
          ESP.restart();
        }
      #endif
    break;

    case TASK_NETWORK_CONNECTED__WIFI:
    case TASK_NETWORK_CONNECTED__ETHERNET:
    case TASK_NETWORK_CONNECTED__CELLULAR:
      #ifdef USE_ARDUINO_OTA
        ArduinoOTAInit();
      #endif
    break;

    case TASK_EVERY_50_MSECOND:
      #ifdef USE_ARDUINO_OTA
        if (!tkr_set->runtime.global_state.network_down) ArduinoOtaLoop();
      #endif
    break;

    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;

    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case TASK_WEB_ADD_HANDLER:
      WebPage_Root_AddHandlers();
    break;
    #endif
  }

  return TASKER_RESULT__UNKNOWN_ID;
}


#ifdef USE_MODULE_NETWORK_WEBSERVER

void mUpdates::WebPage_Root_AddHandlers()
{
  tkr_web->server->on("/update",HTTP_GET,[this](AsyncWebServerRequest *request){
    tkr_web->handleStaticContent(request,F("/update"),200,FPSTR(CONTENT_TYPE_HTML),PAGE_update_web,PAGE_update_web_length,true);
  });

  tkr_web->server->on("/update",HTTP_POST,
    [this](AsyncWebServerRequest *request){
      if(Update.hasError() || update_upload_failed)
      {
        ALOG_ERR(PSTR("OTA fail %u"),Update.getError());
        tkr_web->serveMessage(request,500,F("Update failed"),F("Invalid firmware or upload error."),254);
        return;
      }

      /*
       * The HTTP response still gets time to return before the delayed reboot,
       * but the current WiFi association is captured now while it is known-good.
       */
      OTA_WiFiQuickConnect_Save();

      update_reboot_pending = true;
      update_reboot_at_ms = millis() + 1000;

      tkr_web->serveMessage(request,200,F("Update successful"),F("Rebooting..."),131);
    },
    [this](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
      HandleFirmwareUpload(request,filename,index,data,len,final);
    }
  );
}


void mUpdates::HandleFirmwareUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
  if (!index)
  {
    update_upload_started = true;
    update_upload_failed = false;

    ALOG_INF(PSTR("OTA %s"), filename.c_str());

    #ifdef ESP8266
      Update.runAsync(true);
      const uint32_t max_sketch_space = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;

      if (!Update.begin(max_sketch_space, U_FLASH))
      {
        update_upload_failed = true;
        ALOG_ERR(PSTR("OTA begin %u"), Update.getError());
        return;
      }
    #elif defined(ESP32)
      if (!Update.begin(UPDATE_SIZE_UNKNOWN, U_FLASH))
      {
        update_upload_failed = true;
        ALOG_ERR(PSTR("OTA begin %u"), Update.getError());
        return;
      }
    #else
      update_upload_failed = true;
      return;
    #endif
  }

  if (update_upload_failed || Update.hasError()) return;

  if (len)
  {
    const size_t written = Update.write(data, len);

    if (written != len)
    {
      update_upload_failed = true;
      ALOG_ERR(PSTR("OTA write %u"), Update.getError());
      return;
    }
  }

  if (final)
  {
    if (!update_upload_started)
    {
      update_upload_failed = true;
      ALOG_ERR(PSTR("OTA state"));
      return;
    }

    if (Update.end(true)) ALOG_INF(PSTR("OTA OK %u"), (unsigned)(index + len));
    else
    {
      update_upload_failed = true;
      ALOG_ERR(PSTR("OTA end %u"), Update.getError());
    }

    update_upload_started = false;
  }
}

#endif // USE_MODULE_NETWORK_WEBSERVER


void mUpdates::parse_JSONCommand(JsonParserObject obj)
{
}


#endif // USE_MODULE_CORE_UPDATES