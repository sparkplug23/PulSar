
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
      AddURLtoList("/ws/console", HTTP_GET);
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
    case TASK_EVERY_SECOND:

      #ifdef ENABLE_DEBUGFEATURE_WEBSERVER_URL_LIST
      // for(int i=0;i<gWebUrlTracker.urls.size();i++){
      //   ALOG_INF(PSTR("url %d, %s"), i, gWebUrlTracker.urls[i]);
      // }
      #endif
    break;
    case TASK_NETWORK_CONNECTED__WIFI:
    case TASK_NETWORK_CONNECTED__ETHERNET:
    case TASK_NETWORK_CONNECTED__CELLULAR:
      Server_Start();
    break;    
    case TASK_WEB_ADD_HANDLER:
      WebPage_Root_AddHandlers();
    break;
  }

  return TASKER_RESULT__UNKNOWN_ID;

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
    PAGE_system_controls_web,
    PAGE_system_controls_web_length,
    true
  );
}


void mWebServer::HandlePage_SystemControls_C1(AsyncWebServerRequest *request)
{

  // DELETED METHOD??


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


void mWebServer::WebPage_Root_AddHandlers()
{
  ALOG_HGL(PSTR("mWebServer::WebPage_Root_AddHandlers()"));
  
  SPGM_CTR(PM_URL_VERSION) "/version";
  server->on(PM_URL_VERSION, HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", (String)PROJECT_VERSION);
  });
  AddURLtoList(PM_URL_VERSION, HTTP_GET);

  SPGM_CTR(PM_URL_UPTIME) "/uptime";
  server->on(PM_URL_UPTIME, HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", (String)millis());
  });
  AddURLtoList(PM_URL_UPTIME, HTTP_GET);

  SPGM_CTR(PM_URL_REBOOT) "/reboot";
  server->on(PM_URL_REBOOT, HTTP_GET, [this](AsyncWebServerRequest *request){
    serveMessage(request, 200,
                F("Rebooting now..."),
                F("Please wait ~10 seconds..."),
                129);

    // Grace period for sockets + subsystems
    tkr_sup->ESP_Restart_InSeconds(2);
  });
  AddURLtoList(PM_URL_REBOOT, HTTP_GET);

  SPGM_CTR(PM_URL_RESET) "/reset";
  server->on(PM_URL_RESET, HTTP_GET, [this](AsyncWebServerRequest *request){
    if (!request->hasArg("force")) {
      request->send(403, "text/plain", F("Forbidden: ?force=1 required"));
      return;
    }
    request->send(204);
    ESP.restart();
  });
  AddURLtoList(PM_URL_RESET, HTTP_GET);

  // --------------------------------------------------------------------------
  // Console
  //   - ESP8266: polling only (and likely lightweight page)
  //   - ESP32  : websocket default, polling endpoint exists only for testing
  // --------------------------------------------------------------------------

  #ifdef ESP8266

    // ESP8266: only /console, polling
    SPGM_CTR(PM_URL_CONSOLE) "/console";
    server->on(PM_URL_CONSOLE, HTTP_GET, [this](AsyncWebServerRequest *request){
      this->HandlePage_Console_Poll(request);
    });
    AddURLtoList(PM_URL_CONSOLE, HTTP_GET);

  #else // ESP32 (and others)

    // ESP32: /console is websocket
    SPGM_CTR(PM_URL_CONSOLE) "/console";
    server->on(PM_URL_CONSOLE, HTTP_GET, [this](AsyncWebServerRequest *request){
      this->HandlePage_Console_WebSocket(request);
    });
    AddURLtoList(PM_URL_CONSOLE, HTTP_GET);

    // ESP32: polling exists only for testing, and only when not minimal
    #ifdef ENABLE_FEATURE_WEBSERVER__ADVANCED_WEBPAGES
    SPGM_CTR(PM_URL_CONSOLE_POLL) "/console_poll";
    server->on(PM_URL_CONSOLE_POLL, HTTP_GET, [this](AsyncWebServerRequest *request){
      this->HandlePage_Console_Poll(request);
    });
    AddURLtoList(PM_URL_CONSOLE_POLL, HTTP_GET);
    #endif

  #endif // ESP32

  #ifdef ENABLE_FEATURE_WEBSERVER__ADVANCED_WEBPAGES

    SPGM_CTR(PM_URL_ROOT) "/";
    server->on(PM_URL_ROOT, HTTP_GET, [this](AsyncWebServerRequest *request){
      if (captivePortal(request)) return;
      this->handleStaticContent(request, F("/"), 200, FPSTR(CONTENT_TYPE_HTML), PAGE_root_basic_web, PAGE_root_basic_web_length, true);
    });
    AddURLtoList(PM_URL_ROOT, HTTP_GET);


    #ifdef ENABLE_DEVFEATURE_NETWORK__CAPTIVE_PORTAL

      SPGM_CTR(PM_URL_GENERATE_204) "/generate_204";
      server->on(PM_URL_GENERATE_204, HTTP_GET, [this](AsyncWebServerRequest *request){
        if (this->captivePortal(request)) return;
        request->redirect("/");
      });
      AddURLtoList(PM_URL_GENERATE_204, HTTP_GET);


      SPGM_CTR(PM_URL_HOTSPOT_DETECT) "/hotspot-detect.html";
      server->on(PM_URL_HOTSPOT_DETECT, HTTP_GET, [this](AsyncWebServerRequest *request){
        if (this->captivePortal(request)) return;
        request->redirect("/");
      });
      AddURLtoList(PM_URL_HOTSPOT_DETECT, HTTP_GET);


      SPGM_CTR(PM_URL_NCSI) "/ncsi.txt";
      server->on(PM_URL_NCSI, HTTP_GET, [this](AsyncWebServerRequest *request){
        if (this->captivePortal(request)) return;
        request->send(200, "text/plain", "Microsoft NCSI");
      });
      AddURLtoList(PM_URL_NCSI, HTTP_GET);


      SPGM_CTR(PM_URL_CONNECTTEST) "/connecttest.txt";
      server->on(PM_URL_CONNECTTEST, HTTP_GET, [this](AsyncWebServerRequest *request){
        if (this->captivePortal(request)) return;
        request->send(200, "text/plain", "Microsoft Connect Test");
      });
      AddURLtoList(PM_URL_CONNECTTEST, HTTP_GET);

    #endif // ENABLE_DEVFEATURE_NETWORK__CAPTIVE_PORTAL


    SPGM_CTR(PM_URL_SETTINGS2) "/settings2";
    server->on(PM_URL_SETTINGS2, HTTP_GET, [this](AsyncWebServerRequest *request){
      this->SettingsPages_GET(request);
    });
    AddURLtoList(PM_URL_SETTINGS2, HTTP_GET);


    server->on(PM_URL_SETTINGS2, HTTP_POST, [this](AsyncWebServerRequest *request){
      this->SettingsPages_POST(request);
    });
    AddURLtoList(PM_URL_SETTINGS2, HTTP_POST);


    SPGM_CTR(PM_URL_JSON2) "/json2";
    server->on(PM_URL_JSON2, HTTP_GET, [this](AsyncWebServerRequest *request){
      this->serveJson(request);
    });
    AddURLtoList(PM_URL_JSON2, HTTP_GET);


    SPGM_CTR(PM_URL_SUBMODULE_STYLE_CSS) "/submodule_style.css";
    server->on(PM_URL_SUBMODULE_STYLE_CSS, HTTP_GET, [this](AsyncWebServerRequest *request){
      handleStaticContent(request, FPSTR(PM_URL_SUBMODULE_STYLE_CSS), 200, FPSTR(CONTENT_TYPE_CSS), PAGE_submodule_style_web, PAGE_submodule_style_web_length);
    });
    AddURLtoList(PM_URL_SUBMODULE_STYLE_CSS, HTTP_GET);

    
    SPGM_CTR(PM_URL_STYLE_CSS) "/style.css";
    server->on(PM_URL_STYLE_CSS, HTTP_GET, [this](AsyncWebServerRequest *request){
      handleStaticContent(request, FPSTR(PM_URL_STYLE_CSS), 200, FPSTR(CONTENT_TYPE_CSS), PAGE_settingsCss2_web, PAGE_settingsCss2_web_length);
    });
    AddURLtoList(PM_URL_STYLE_CSS, HTTP_GET);


    SPGM_CTR(PM_URL_FAVICON_ICO) "/favicon.ico";
    server->on(PM_URL_FAVICON_ICO, HTTP_GET, [this](AsyncWebServerRequest *request){
      this->handleStaticContent(request, FPSTR(PM_URL_FAVICON_ICO), 200, F("image/x-icon"), favicon2_web, favicon2_web_length, false);
    });
    AddURLtoList(PM_URL_FAVICON_ICO, HTTP_GET);


    SPGM_CTR(PM_URL_SKIN_CSS) "/skin.css";
    server->on(PM_URL_SKIN_CSS, HTTP_GET, [](AsyncWebServerRequest *request){
      if (tkr_mfile->handleFileRead(request, FPSTR(PM_URL_SKIN_CSS))) return;
      AsyncWebServerResponse *response = request->beginResponse(200, FPSTR(CONTENT_TYPE_CSS));
      request->send(response);
    });
    AddURLtoList(PM_URL_SKIN_CSS, HTTP_GET);


    SPGM_CTR(PM_URL_COMMON_JS) "/common.js";
    server->on(PM_URL_COMMON_JS, HTTP_GET, [this](AsyncWebServerRequest *request){    
      this->handleStaticContent(request, FPSTR(PM_URL_COMMON_JS), 200, FPSTR(CONTENT_TYPE_JAVASCRIPT), JS_common2_web, JS_common2_web_length);
    });
    AddURLtoList(PM_URL_COMMON_JS, HTTP_GET);


    SPGM_CTR(PM_URL_DEBUG_MAIN) "/debug/main";
    server->on(PM_URL_DEBUG_MAIN, HTTP_GET, [this](AsyncWebServerRequest *request){
      if (captivePortal(request)) return;
      this->handleStaticContent(request, F("/debug/main"), 200, FPSTR(CONTENT_TYPE_HTML), PAGE_debug_main_web, PAGE_debug_main_web_length, true);
    });
    AddURLtoList(PM_URL_DEBUG_MAIN, HTTP_GET);

    SPGM_CTR(PM_URL_SYSTEM_CONTROLS_C1) "/system/controls/c1";
    server->on(PM_URL_SYSTEM_CONTROLS_C1, HTTP_GET, [this](AsyncWebServerRequest *request){
      this->HandlePage_SystemControls_C1(request);
    });
    AddURLtoList(PM_URL_SYSTEM_CONTROLS_C1, HTTP_GET);


    SPGM_CTR(PM_URL_SYSTEM_CONTROLS_C2) "/system/controls/c2";
    server->on(PM_URL_SYSTEM_CONTROLS_C2, HTTP_GET, [this](AsyncWebServerRequest *request){
      this->HandlePage_SystemControls_C2(request);
    });
    AddURLtoList(PM_URL_SYSTEM_CONTROLS_C2, HTTP_GET);


    SPGM_CTR(PM_URL_SYSTEM_CONTROLS_C3) "/system/controls/c3";
    server->on(PM_URL_SYSTEM_CONTROLS_C3, HTTP_GET, [this](AsyncWebServerRequest *request){
      this->HandlePage_SystemControls_C3(request);
    });
    AddURLtoList(PM_URL_SYSTEM_CONTROLS_C3, HTTP_GET);


    SPGM_CTR(PM_URL_SYSTEM_CONTROLS) "/system/controls";
    server->on(PM_URL_SYSTEM_CONTROLS, HTTP_GET, [this](AsyncWebServerRequest *request){
      this->HandlePage_SystemControls(request);
    });
    AddURLtoList(PM_URL_SYSTEM_CONTROLS, HTTP_GET);

    #ifdef ENABLE_DEBUGFEATURE_WEBSERVER_URL_LIST
      SPGM_CTR(PM_URL_URL_LIST) "/url_list";
      server->on(PM_URL_URL_LIST, HTTP_GET, [this](AsyncWebServerRequest *request){
        this->HandlePage_UrlList(request);
      });
      AddURLtoList(PM_URL_URL_LIST, HTTP_GET);

      SPGM_CTR(PM_URL_URL_LIST_JSON) "/url_list.json";
      server->on(PM_URL_URL_LIST_JSON, HTTP_GET, [this](AsyncWebServerRequest *request){
        this->HandlePage_UrlList_JSON(request);
      });
      AddURLtoList(PM_URL_URL_LIST_JSON, HTTP_GET);
    #endif

  #endif // ENABLE_FEATURE_WEBSERVER__ADVANCED_WEBPAGES
  
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
    #ifdef ENABLE_FEATURE_LIGHTING__WEBUI
    ALOG_ERR(PSTR("Not sure this needs to stay or not"));
    if(tkr_anim->handle__HTTP__GET_QueryAPI(request, request->url())) return;
    #endif
    #endif
    handleStaticContent(request, request->url(), 404, FPSTR(CONTENT_TYPE_HTML), PAGE_404_web, PAGE_404_web_length);
  });

  
}


#ifdef ENABLE_DEBUGFEATURE_WEBSERVER_URL_LIST
void mWebServer::HandlePage_UrlList(AsyncWebServerRequest *request)
{
  if (captivePortal(request)) return;

  handleStaticContent(
    request,
    F("/url_list.htm"),
    200,
    FPSTR(CONTENT_TYPE_HTML),
    PAGE_url_list,
    PAGE_url_list_length,
    true
  );
}


void mWebServer::HandlePage_UrlList_JSON(AsyncWebServerRequest *request)
{
  AsyncResponseStream *response = request->beginResponseStream(FPSTR(PM_WEB_CONTENT_TYPE_APPLICATION_JSON_JAVASCRIPT));
  if (!response) {
    request->send(500, FPSTR(CONTENT_TYPE_PLAIN), F("Failed to allocate JSON response"));
    return;
  }

  response->print(F("{\"items\":["));

  for (size_t i = 0; i < gWebUrlTracker.urls.size(); i++)
  {
    const auto &entry = gWebUrlTracker.urls[i];

    if (i) {
      response->print(',');
    }

    response->print(F("{\"url\":\""));

    // minimal JSON escaping for url
    for (size_t j = 0; j < entry.url.length(); j++)
    {
      char c = entry.url[j];
      switch (c)
      {
        case '\"': response->print(F("\\\"")); break;
        case '\\': response->print(F("\\\\")); break;
        case '\n': response->print(F("\\n"));  break;
        case '\r': response->print(F("\\r"));  break;
        case '\t': response->print(F("\\t"));  break;
        default:   response->write(c);         break;
      }
    }

    response->print(F("\",\"method\":"));
    response->print(entry.method);
    response->print(F(",\"port\":"));
    response->print(entry.port);
    response->print('}');
  }

  response->print(F("]}"));
  request->send(response);
}
#endif

#endif