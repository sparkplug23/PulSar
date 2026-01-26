
#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_NETWORK_WEBSERVER

#include "_WebServer.h"

int8_t mWebServer::Tasker(uint8_t function, JsonParserObject obj)
{

// DEBUG_LINE_HERE;

  switch(function)
  {
    case TASK_INIT:

      sprintf(cmDNS, MDNS_NAME);                // mDNS address (*.local, replaced by wledXXXXXX if default is used)

     
      server = new AsyncWebServer(80);

      websocket_lights = new AsyncWebSocket("/ws");

      #ifndef ESP8266
      #ifdef ENABLE_DEVFEATURE_NETWORK__CONSOLE_WEBSOCKET
      websocket_console = new AsyncWebSocket("/ws/console");
      websocket_console->onEvent(
        [this](AsyncWebSocket *server,
              AsyncWebSocketClient *client,
              AwsEventType type,
              void *arg,
              uint8_t *data,
              size_t len)
        {
          this->wsEventConsole(server, client, type, arg, data, len);
        }
      );
      #endif
      #endif

      // generate module IDs must be done before AP setup
      String escapedMac;
      escapedMac = WiFi.macAddress();
      escapedMac.replace(":", "");
      escapedMac.toLowerCase();
      if (strcmp(cmDNS, "pulsar") == 0) sprintf_P(cmDNS, PSTR("pulsar-%*s"), 6, escapedMac.c_str() + 6);


      
    break;
  }


  switch(function){
    /************
     * SYSTEM SECTION * 
    *******************/    
    case TASK_RESTART_SET_DO_FINAL_CLEANUP:    
      websocket_lights->closeAll(1012);
    break;
    /************
     * PERIODIC SECTION * 
    *******************/

    case TASK_INIT:
      // init();



    break;
    case TASK_LOOP:
    {
      // PollDnsWebserver();

    // Serial.printf("Bctr %s [%d]\n\r", data_buffer.payload.ctr, strlen(data_buffer.payload.ctr));
      #ifdef ENABLE_FEATURE_WEBSERVER__DELAYED_JSONLOCKED_COMMAND_PROCESSING
    //   if(data_buffer.IsDelayedJSONCommandWaiting())
    //   {
    // Serial.printf("Cctr %s [%d]\n\r", data_buffer.payload.ctr, strlen(data_buffer.payload.ctr));
    //     ALOG_INF(PSTR(D_LOG_HTTP "Processing delayed JSON locked command %d"), strlen(data_buffer.payload.ctr));
    //     tkr->Tasker_Interface(TASK_JSON_COMMAND_ID);
    //     data_buffer.releaseLock();
    //   }

    if (pending_cmd.has()) {

      if (data_buffer.requestLock(GetModuleUniqueID())) {

        data_buffer.ClearSoft();
        data_buffer.payload.length_used = pending_cmd.len;
        memcpy(data_buffer.payload.ctr, pending_cmd.ptr, pending_cmd.len);
        data_buffer.payload.ctr[pending_cmd.len] = '\0';

        pending_cmd.clear();

        tkr->Tasker_Interface(TASK_JSON_COMMAND_ID);
        data_buffer.releaseLock();
      }
    }



      #endif

      #ifndef ESP8266
      #ifdef ENABLE_DEVFEATURE_NETWORK__CONSOLE_WEBSOCKET
      handleConsoleWs();
      #endif
      #endif


    }
    break; 
    case TASK_NETWORK_CONNECTED: //tmp icnclude as fallthrough
    case TASK_WEBSERVER_START:
      Server_Start();
    break;
    
    case TASK_WEB_ADD_HANDLER:
      WebPage_Root_AddHandlers();
    break;

 
  }


  return FUNCTION_RESULT_UNKNOWN_ID;

// DEBUG_LINE_HERE;
}


void mWebServer::Server_Start()
{
  ALOG_HGLT(PSTR(D_LOG_HTTP "Starting web server")); 
    
  // CORS compatiblity
  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Origin"), "*");
  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Methods"), "*");
  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Headers"), "*");
 
  createEditHandler(true);

  server->addHandler(websocket_lights);
  
  #ifndef ESP8266
  #ifdef ENABLE_DEVFEATURE_NETWORK__CONSOLE_WEBSOCKET
  server->addHandler(websocket_console);
  #endif
  #endif

  // Let modules add their own URLs
  tkr->Tasker_Interface(TASK_WEB_ADD_HANDLER);

  server->begin();
  
  IPAddress localip = WiFi.localIP();
  ALOG_INF(PSTR(D_LOG_HTTP "Webserver started on http://%s.local or http://%s or http://%d.%d.%d.%d"), 
  cmDNS, tkr_set->runtime.my_hostname, localip[0],localip[1],localip[2],localip[3] );

  ALOG_HGLB(PSTR(D_LOG_HTTP "Started web server"));
}



void mWebServer::HandlePage_SystemControls(AsyncWebServerRequest *request)
{
  // If you want captive portal redirects to hit here, you can add:
  // if (captivePortal(request)) return;

  static const char _path[] PROGMEM = "/system/controls";
  this->handleStaticContent(
    request,
    FPSTR(_path),
    200,
    FPSTR(CONTENT_TYPE_HTML),
    PAGE_system_controls,
    PAGE_system_controls_length,
    true
  );
}


void mWebServer::HandlePage_SystemControls_C1(AsyncWebServerRequest *request)
{
  AsyncResponseStream *response = request->beginResponseStream(FPSTR(PM_WEB_CONTENT_TYPE_TEXT_HTML));
  if (!response) { request->send(500); return; }
  setStaticContentCacheHeaders(response, 200, 0);

  // Set append context for Tasker-driven modules
  web_controls_stream = response;
  web_controls_container_id = 1;

  // Let modules append their blocks (modules will use tkr_web->WebControls_GetPrint())
  tkr->Tasker_Interface(TASK_WEB_APPEND_SENSOR_TABLE_VALUES);

  // Clear context
  web_controls_stream = nullptr;
  web_controls_container_id = 0;

  request->send(response);
}



void mWebServer::HandlePage_SystemControls_C2(AsyncWebServerRequest *request)
{
  AsyncResponseStream *response = request->beginResponseStream(FPSTR(PM_WEB_CONTENT_TYPE_TEXT_HTML));
  setStaticContentCacheHeaders(response, 200, 0);

  web_controls_stream = response;
  web_controls_container_id = 2;

  tkr->Tasker_Interface(TASK_WEB_APPEND_DRIVER_TABLE_VALUES);

  web_controls_stream = nullptr;
  web_controls_container_id = 0;

  request->send(response);
}

void mWebServer::HandlePage_SystemControls_C3(AsyncWebServerRequest *request)
{
  AsyncResponseStream *response = request->beginResponseStream(FPSTR(PM_WEB_CONTENT_TYPE_TEXT_HTML));
  setStaticContentCacheHeaders(response, 200, 0);

  web_controls_stream = response;
  web_controls_container_id = 3;

  tkr->Tasker_Interface(TASK_WEB_APPEND_CONTROLLER_TABLE_VALUES);

  web_controls_stream = nullptr;
  web_controls_container_id = 0;

  request->send(response);
}




/**
 * @brief Need to start making "minimal" webpages to test on esp8266
 * 
 * ESP8266 Minimal pages
 * ** basic probe values (version/uptime)
 * ** directed controls (/reboot or /reset?force=1 )
 * ** console for debugging
 * ** update/recovery
 * 
 */
void mWebServer::WebPage_Root_AddHandlers()
{

  server->on("/version", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", (String)PROJECT_VERSION);
  });

  server->on("/uptime", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", (String)millis());
  });

  server->on("/reboot", HTTP_GET, [this](AsyncWebServerRequest *request){
    serveMessage(request, 200,
                F("Rebooting now..."),
                F("Please wait ~10 seconds..."),
                129);

    // Grace period for sockets + subsystems
    tkr_sup->ESP_Restart_InSeconds(2);
  });

  server->on("/reset", HTTP_GET, [this](AsyncWebServerRequest *request){
    if (!request->hasArg("force")) {
      request->send(403, "text/plain", F("Forbidden: ?force=1 required"));
      return;
    }
    request->send(204);
    ESP.restart();
  });

  // --------------------------------------------------------------------------
  // Console
  //   - ESP8266: polling only (and likely lightweight page)
  //   - ESP32  : websocket default, polling endpoint exists only for testing
  // --------------------------------------------------------------------------

  #ifdef ESP8266

    // ESP8266: only /console, polling
    server->on("/console", HTTP_GET, [this](AsyncWebServerRequest *request){
      this->HandlePage_Console_Poll(request);
    });

  #else // ESP32 (and others)

    // ESP32: /console is websocket
    server->on("/console", HTTP_GET, [this](AsyncWebServerRequest *request){
      this->HandlePage_Console_WebSocket(request);
    });

    // ESP32: polling exists only for testing, and only when not minimal
    #ifdef ENABLE_FEATURE_WEBSERVER__ADVANCED_WEBPAGES
    server->on("/console_poll", HTTP_GET, [this](AsyncWebServerRequest *request){
      this->HandlePage_Console_Poll(request);
    });
    #endif

  #endif




  #ifdef ENABLE_FEATURE_WEBSERVER__ADVANCED_WEBPAGES

  server->on("/", HTTP_GET, [this](AsyncWebServerRequest *request){
    if (captivePortal(request)) return;
    this->handleStaticContent(request, F("/"), 200, FPSTR(CONTENT_TYPE_HTML), PAGE_root_basic, PAGE_root_basic_length, true);
  });



#ifdef ENABLE_DEVFEATURE_NETWORK__CAPTIVE_PORTAL
server->on("/generate_204", HTTP_GET, [this](AsyncWebServerRequest *request){
  if (this->captivePortal(request)) return;
  request->redirect("/");
});

server->on("/hotspot-detect.html", HTTP_GET, [this](AsyncWebServerRequest *request){
  if (this->captivePortal(request)) return;
  request->redirect("/");
});

server->on("/ncsi.txt", HTTP_GET, [this](AsyncWebServerRequest *request){
  if (this->captivePortal(request)) return;
  request->send(200, "text/plain", "Microsoft NCSI");
});

server->on("/connecttest.txt", HTTP_GET, [this](AsyncWebServerRequest *request){
  if (this->captivePortal(request)) return;
  request->send(200, "text/plain", "Microsoft Connect Test");
});
#endif

  /**
   * Console Page
   * */



server->on("/settings2", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->SettingsPages_GET(request);
  });

server->on("/settings2", HTTP_POST, [this](AsyncWebServerRequest *request){
    this->SettingsPages_POST(request);
  });

  server->on("/json2", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->serveJson(request);
  });


  static const char _submodule_style_css[] PROGMEM = "/submodule_style.css";
  server->on("/submodule_style.css", HTTP_GET, [this](AsyncWebServerRequest *request){
    handleStaticContent(request, FPSTR(_submodule_style_css), 200, FPSTR(CONTENT_TYPE_CSS), PAGE_submodule_style, PAGE_submodule_style_length);
  });

  
  #ifdef ENABLE_DEVFEATURE_WEBSERVER__STYLES_NOW_SHARED
  static const char _style_css[] PROGMEM = "/style.css";
  server->on("/style.css", HTTP_GET, [this](AsyncWebServerRequest *request){
    handleStaticContent(request, FPSTR(_style_css), 200, FPSTR(CONTENT_TYPE_CSS), PAGE_settingsCss2, PAGE_settingsCss2_length);
  });

  static const char _favicon_ico[] PROGMEM = "/favicon.ico";
  server->on(_favicon_ico, HTTP_GET, [this](AsyncWebServerRequest *request){
    this->handleStaticContent(request, FPSTR(_favicon_ico), 200, F("image/x-icon"), favicon2, favicon2_length, false);
  });

  static const char _skin_css[] PROGMEM = "/skin.css";
  server->on(_skin_css, HTTP_GET, [](AsyncWebServerRequest *request){
    if (tkr_mfile->handleFileRead(request, FPSTR(_skin_css))) return;
    AsyncWebServerResponse *response = request->beginResponse(200, FPSTR(CONTENT_TYPE_CSS));
    request->send(response);
  });
  #endif

 static const char _common_js[] PROGMEM = "/common.js";
  server->on(_common_js, HTTP_GET, [this](AsyncWebServerRequest *request){    
    this->handleStaticContent(request, FPSTR(_common_js), 200, FPSTR(CONTENT_TYPE_JAVASCRIPT), JS_common2, JS_common2_length);
  });
  
  server->on("/debug/main", HTTP_GET, [this](AsyncWebServerRequest *request){
    if (captivePortal(request)) return;
    this->handleStaticContent(request, F("/debug/main"), 200, FPSTR(CONTENT_TYPE_HTML), PAGE_debug_main, PAGE_debug_main_length, true);
  });

  //called when the url is not defined here, ajax-in; get-settings
  server->onNotFound([this](AsyncWebServerRequest *request)
  {
    ALOG_ERR(PSTR("HTTP URI Not-Found: %s"), request->url().c_str());    
    if (captivePortal(request)) return;

    //make API CORS compatible
    if (request->method() == HTTP_OPTIONS)
    {
      AsyncWebServerResponse *response = request->beginResponse(200);
      response->addHeader(F("Access-Control-Max-Age"), F("7200"));
      request->send(response);
      return;
    }
    #ifdef USE_MODULE_LIGHTS_ANIMATOR
    ALOG_ERR(PSTR("Not sure this needs to stay or not"));
    if(tkr_anim->handle__HTTP__GET_QueryAPI(request, request->url())) return;
    #endif
    handleStaticContent(request, request->url(), 404, FPSTR(CONTENT_TYPE_HTML), PAGE_404, PAGE_404_length);
  });


server->on("/system/controls/c1", HTTP_GET, [this](AsyncWebServerRequest *request){
  this->HandlePage_SystemControls_C1(request);
});
server->on("/system/controls/c2", HTTP_GET, [this](AsyncWebServerRequest *request){
  this->HandlePage_SystemControls_C2(request);
});
server->on("/system/controls/c3", HTTP_GET, [this](AsyncWebServerRequest *request){
  this->HandlePage_SystemControls_C3(request);
});

  server->on("/system/controls", HTTP_GET, [this](AsyncWebServerRequest *request){
  this->HandlePage_SystemControls(request);
});



//   server->onNotFound([this](AsyncWebServerRequest *request){HandleNotFound(request); });

  #endif
  
}



#endif