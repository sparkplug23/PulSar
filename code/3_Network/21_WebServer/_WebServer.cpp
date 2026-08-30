
#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_NETWORK_WEBSERVER

#include "_WebServer.h"

int8_t mWebServer::Tasker(uint8_t function, JsonParserObject obj)
{

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
      #ifdef ENABLE_FEATURE_WEBSERVER__ADVANCED_WEBPAGES
      websocket_pages = new AsyncWebSocket("/ws/page");
      websocket_pages->onEvent(
        [this](AsyncWebSocket *server,
              AsyncWebSocketClient *client,
              AwsEventType type,
              void *arg,
              uint8_t *data,
              size_t len)
        {
          this->wsEventPages(server, client, type, arg, data, len);
        }
      );
      AddURLtoList("/ws/page", HTTP_GET);
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


      #ifdef ENABLE_FEATURE_WEBSERVER__ADVANCED_WEBPAGES
      if(websocket_pages){
        websocket_pages->cleanupClients();
      }
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


void mWebServer::AddURLasApplication(uint16_t module_id, const char* url, const char* friendly_name, uint16_t port)
{
  if(!url || !url[0]) return;

  String normalised_url = url;
  if(normalised_url[0] != '/') normalised_url = "/" + normalised_url;

  for(const auto& entry : application_urls)
  {
    if(entry.module_id == module_id && entry.url == normalised_url && entry.port == port)
    {
      return;
    }
  }

  WebApplicationURL entry;

  entry.module_id = module_id;
  entry.url = normalised_url;
  entry.port = port;

  if(friendly_name && friendly_name[0])
  {
    entry.friendly_name = friendly_name;
  }

  application_urls.push_back(std::move(entry));
}


void mWebServer::AddURLasApplication(uint16_t module_id, const String& url, const char* friendly_name, uint16_t port)
{
  AddURLasApplication(module_id, url.c_str(), friendly_name, port);
}


void mWebServer::Server_Start()
{
  ALOG_INF(PSTR(D_LOG_HTTP "Starting web server")); 
    
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

  #ifdef ENABLE_FEATURE_WEBSERVER__ADVANCED_WEBPAGES
  if(websocket_pages){
    server->addHandler(websocket_pages);
  }
  #endif

  // // Let modules add their own URLs
  tkr->Tasker_Interface(TASK_WEB_ADD_HANDLER);

  server->begin();
  
  IPAddress localip = WiFi.localIP();
  ALOG_INF(PSTR(D_LOG_HTTP "Webserver started on http://%s.local or http://%s or http://%d.%d.%d.%d"), 
  cmDNS, tkr_set->runtime.my_hostname, localip[0],localip[1],localip[2],localip[3] );

  // ALOG_HGLB(PSTR(D_LOG_HTTP "Started web server"));
}


void mWebServer::WebPage_Root_AddHandlers()
{
  ALOG_DBG(PSTR("mWebServer::WebPage_Root_AddHandlers()"));


  /**************************************************************************************************
   * Core system actions
   **************************************************************************************************/

  SPGM_CTR(PM_URL_REBOOT) "/reboot";
  server->on(PM_URL_REBOOT, HTTP_GET, [this](AsyncWebServerRequest* request){
    serveMessage(request, 200, F("Rebooting now..."), F("Please wait ~10 seconds..."), 129);
    tkr_sup->ESP_Restart_InSeconds(2);
  });
  AddURLtoList(PM_URL_REBOOT, HTTP_GET);


  SPGM_CTR(PM_URL_RESET) "/reset";
  server->on(PM_URL_RESET, HTTP_GET, [this](AsyncWebServerRequest* request){
    if(!request->hasArg("force")){
      request->send(403, FPSTR(CONTENT_TYPE_PLAIN), F("Forbidden: ?force=1 required"));
      return;
    }

    request->send(204);
    ESP.restart();
  });
  AddURLtoList(PM_URL_RESET, HTTP_GET);


  /**************************************************************************************************
   * Shared PulSar WebUI assets
   **************************************************************************************************/

  SPGM_CTR(PM_URL_PULSAR_CSS) "/pulsar.css";
  server->on(PM_URL_PULSAR_CSS, HTTP_GET, [this](AsyncWebServerRequest* request){
    handleStaticContent(request, FPSTR(PM_URL_PULSAR_CSS), 200, FPSTR(CONTENT_TYPE_CSS), PAGE_pulsar_css_web, PAGE_pulsar_css_web_length);
  });
  AddURLtoList(PM_URL_PULSAR_CSS, HTTP_GET);


  SPGM_CTR(PM_URL_PULSAR_JS) "/pulsar.js";
  server->on(PM_URL_PULSAR_JS, HTTP_GET, [this](AsyncWebServerRequest* request){
    handleStaticContent(request, FPSTR(PM_URL_PULSAR_JS), 200, FPSTR(CONTENT_TYPE_JAVASCRIPT), JS_pulsar_web, JS_pulsar_web_length);
  });
  AddURLtoList(PM_URL_PULSAR_JS, HTTP_GET);


  SPGM_CTR(PM_URL_PULSAR_DATA_JS) "/pulsar_data.js";
  server->on(PM_URL_PULSAR_DATA_JS, HTTP_GET, [this](AsyncWebServerRequest* request){
    handleStaticContent(request, FPSTR(PM_URL_PULSAR_DATA_JS), 200, FPSTR(CONTENT_TYPE_JAVASCRIPT), JS_pulsar_data_web, JS_pulsar_data_web_length);
  });
  AddURLtoList(PM_URL_PULSAR_DATA_JS, HTTP_GET);


  /**************************************************************************************************
   * Optional/shared styling assets
   **************************************************************************************************/

  SPGM_CTR(PM_URL_SKIN_CSS) "/skin.css";
  server->on(PM_URL_SKIN_CSS, HTTP_GET, [](AsyncWebServerRequest* request){
    if(tkr_mfile->handleFileRead(request, FPSTR(PM_URL_SKIN_CSS))) return;

    AsyncWebServerResponse* response = request->beginResponse(200, FPSTR(CONTENT_TYPE_CSS));
    request->send(response);
  });
  AddURLtoList(PM_URL_SKIN_CSS, HTTP_GET);


  #ifdef ENABLE_FEATURE_WEBSERVER__ADVANCED_WEBPAGES

  SPGM_CTR(PM_URL_FAVICON_ICO) "/favicon.ico";
  server->on(PM_URL_FAVICON_ICO, HTTP_GET, [this](AsyncWebServerRequest* request){
    handleStaticContent(request, FPSTR(PM_URL_FAVICON_ICO), 200, F("image/x-icon"), favicon2_web, favicon2_web_length, false);
  });
  AddURLtoList(PM_URL_FAVICON_ICO, HTTP_GET);

  #endif


  /**************************************************************************************************
   * Generic PulSar APIs
   **************************************************************************************************/

  SPGM_CTR(PM_URL_API_TELEMETRY) "/api/telemetry";
  server->on(PM_URL_API_TELEMETRY, HTTP_GET, [this](AsyncWebServerRequest* request){ HandleAPI_Telemetry(request); });
  AddURLtoList(PM_URL_API_TELEMETRY, HTTP_GET);


  SPGM_CTR(PM_URL_APPLICATIONS) "/url_apps";
  server->on(PM_URL_APPLICATIONS, HTTP_GET, [this](AsyncWebServerRequest* request){ HandleAPI_URLApplications(request); });
  AddURLtoList(PM_URL_APPLICATIONS, HTTP_GET);


  /**************************************************************************************************
   * Advanced APIs
   *
   * IMPORTANT:
   * Register the longest / most specific paths first.
   * This AsyncWebServer setup matches parent prefixes, so /adv must come LAST.
   **************************************************************************************************/

  #ifdef ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS

  SPGM_CTR(PM_URL_ADV_API_TASKER) "/adv/api/tasker";
  server->on(PM_URL_ADV_API_TASKER, HTTP_GET, [this](AsyncWebServerRequest* request){ HandleAPI_DebugTaskerMetrics(request); });
  AddURLtoList(PM_URL_ADV_API_TASKER, HTTP_GET);

  #endif


  #ifdef ENABLE_FEATURE_WEBSERVER__ADVANCED_URL_LIST

  SPGM_CTR(PM_URL_ADV_API_URLS) "/adv/api/urls";
  server->on(PM_URL_ADV_API_URLS, HTTP_GET, [this](AsyncWebServerRequest* request){ HandlePage_UrlList_JSON(request); });
  AddURLtoList(PM_URL_ADV_API_URLS, HTTP_GET);

  #endif


  /**************************************************************************************************
   * Advanced pages
   *
   * Again: child routes before /adv.
   **************************************************************************************************/

  #ifdef ENABLE_DEBUGFEATURE_WEB__TELEMETRY

  SPGM_CTR(PM_URL_ADV_TELEMETRY) "/adv/telemetry";
  server->on(PM_URL_ADV_TELEMETRY, HTTP_GET, [this](AsyncWebServerRequest* request){ HandlePage_DebugTelemetry(request); });
  AddURLtoList(PM_URL_ADV_TELEMETRY, HTTP_GET);

  #endif


  #ifdef ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS

  SPGM_CTR(PM_URL_ADV_TASKER) "/adv/tasker";
  server->on(PM_URL_ADV_TASKER, HTTP_GET, [this](AsyncWebServerRequest* request){ HandlePage_DebugTaskerMetrics(request); });
  AddURLtoList(PM_URL_ADV_TASKER, HTTP_GET);

  #endif


  #ifdef ENABLE_FEATURE_WEBSERVER__ADVANCED_URL_LIST

  SPGM_CTR(PM_URL_ADV_URLS) "/adv/urls";
  server->on(PM_URL_ADV_URLS, HTTP_GET, [this](AsyncWebServerRequest* request){ HandlePage_UrlList(request); });
  AddURLtoList(PM_URL_ADV_URLS, HTTP_GET);

  #endif


  /**************************************************************************************************
   * Console
   **************************************************************************************************/

  #ifdef ESP8266

  SPGM_CTR(PM_URL_CONSOLE) "/console";
  server->on(PM_URL_CONSOLE, HTTP_GET, [this](AsyncWebServerRequest* request){ HandlePage_Console_Poll(request); });
  AddURLtoList(PM_URL_CONSOLE, HTTP_GET);

  #else

  SPGM_CTR(PM_URL_CONSOLE) "/console";
  server->on(PM_URL_CONSOLE, HTTP_GET, [this](AsyncWebServerRequest* request){ HandlePage_Console_WebSocket(request); });
  AddURLtoList(PM_URL_CONSOLE, HTTP_GET);

    #ifdef ENABLE_FEATURE_WEBSERVER__ADVANCED_WEBPAGES

    SPGM_CTR(PM_URL_CONSOLE_POLL) "/console_poll";
    server->on(PM_URL_CONSOLE_POLL, HTTP_GET, [this](AsyncWebServerRequest* request){ HandlePage_Console_Poll(request); });
    AddURLtoList(PM_URL_CONSOLE_POLL, HTTP_GET);

    #endif

  #endif


  /**************************************************************************************************
   * Settings
   *
   * One shared GET/POST dispatcher owns the settings namespace.
   * SettingsPages_GET/POST inspect request->url() and select WebSettingsSubPage.
   **************************************************************************************************/

  #ifdef ENABLE_FEATURE_WEBSERVER__ADVANCED_WEBPAGES

  SPGM_CTR(PM_URL_SETTINGS2) "/settings";
  server->on(PM_URL_SETTINGS2, HTTP_GET, [this](AsyncWebServerRequest* request){ SettingsPages_GET(request); });
  server->on(PM_URL_SETTINGS2, HTTP_POST, [this](AsyncWebServerRequest* request){ SettingsPages_POST(request); });
  AddURLtoList(PM_URL_SETTINGS2, HTTP_GET);
  AddURLtoList(PM_URL_SETTINGS2, HTTP_POST);


  /**************************************************************************************************
   * Legacy/general JSON endpoint
   **************************************************************************************************/

  SPGM_CTR(PM_URL_JSON2) "/json2";
  server->on(PM_URL_JSON2, HTTP_GET, [this](AsyncWebServerRequest* request){ serveJson(request); });
  AddURLtoList(PM_URL_JSON2, HTTP_GET);

  #endif


  /**************************************************************************************************
   * Captive portal OS detection
   **************************************************************************************************/

  #ifdef ENABLE_DEVFEATURE_NETWORK__CAPTIVE_PORTAL

  SPGM_CTR(PM_URL_HOTSPOT_DETECT) "/hotspot-detect.html";
  server->on(PM_URL_HOTSPOT_DETECT, HTTP_GET, [this](AsyncWebServerRequest* request){
    if(captivePortal(request)) return;
    request->send(200, FPSTR(CONTENT_TYPE_HTML), F("<HTML><HEAD><TITLE>Success</TITLE></HEAD><BODY>Success</BODY></HTML>"));
  });
  AddURLtoList(PM_URL_HOTSPOT_DETECT, HTTP_GET);


  SPGM_CTR(PM_URL_CONNECTTEST) "/connecttest.txt";
  server->on(PM_URL_CONNECTTEST, HTTP_GET, [this](AsyncWebServerRequest* request){
    if(captivePortal(request)) return;
    request->send(200, FPSTR(CONTENT_TYPE_PLAIN), F("Microsoft Connect Test"));
  });
  AddURLtoList(PM_URL_CONNECTTEST, HTTP_GET);


  SPGM_CTR(PM_URL_GENERATE_204) "/generate_204";
  server->on(PM_URL_GENERATE_204, HTTP_GET, [this](AsyncWebServerRequest* request){
    if(captivePortal(request)) return;
    request->send(204);
  });
  AddURLtoList(PM_URL_GENERATE_204, HTTP_GET);


  SPGM_CTR(PM_URL_NCSI) "/ncsi.txt";
  server->on(PM_URL_NCSI, HTTP_GET, [this](AsyncWebServerRequest* request){
    if(captivePortal(request)) return;
    request->send(200, FPSTR(CONTENT_TYPE_PLAIN), F("Microsoft NCSI"));
  });
  AddURLtoList(PM_URL_NCSI, HTTP_GET);

  #endif


  /**************************************************************************************************
   * Advanced landing page
   *
   * MUST come after every /adv/... child route.
   **************************************************************************************************/

  #ifdef ENABLE_FEATURE_WEBSERVER__ADVANCED_WEBPAGES

  SPGM_CTR(PM_URL_ADV) "/adv";
  server->on(PM_URL_ADV, HTTP_GET, [this](AsyncWebServerRequest* request){
    handleStaticContent(request, FPSTR(PM_URL_ADV), 200, FPSTR(CONTENT_TYPE_HTML), PAGE_debug_main_web, PAGE_debug_main_web_length);
  });
  AddURLtoList(PM_URL_ADV, HTTP_GET);

  #endif


  /**************************************************************************************************
   * Root
   *
   * "/" is the broadest route and therefore belongs near the end.
   **************************************************************************************************/

  #ifdef ENABLE_FEATURE_WEBSERVER__ADVANCED_WEBPAGES

  SPGM_CTR(PM_URL_ROOT) "/";
  server->on(PM_URL_ROOT, HTTP_GET, [this](AsyncWebServerRequest* request){
    if(captivePortal(request)) return;
    handleStaticContent(request, F("/"), 200, FPSTR(CONTENT_TYPE_HTML), PAGE_root_main_web, PAGE_root_main_web_L, true);
  });
  AddURLtoList(PM_URL_ROOT, HTTP_GET);

  #endif


  /**************************************************************************************************
   * Fallback
   **************************************************************************************************/

  server->onNotFound([this](AsyncWebServerRequest* request){
    ALOG_ERR(PSTR("HTTP URI Not-Found: %s"), request->url().c_str());

    if(captivePortal(request)) return;

    if(request->method() == HTTP_OPTIONS)
    {
      AsyncWebServerResponse* response = request->beginResponse(200);
      response->addHeader(F("Access-Control-Max-Age"), F("7200"));
      request->send(response);
      return;
    }

    #ifdef USE_MODULE_LIGHTS_ANIMATOR
    #ifdef ENABLE_FEATURE_LIGHTING__WEBUI
    if(tkr_anim->handle__HTTP__GET_QueryAPI(request, request->url())) return;
    #endif
    #endif

    handleStaticContent(request, request->url(), 404, FPSTR(CONTENT_TYPE_HTML), PAGE_404_web, PAGE_404_web_length);
  });
}




void mWebServer::HandleAPI_URLApplications(AsyncWebServerRequest* request)
{
  AsyncResponseStream* response = request->beginResponseStream(FPSTR(CONTENT_TYPE_JSON));

  if(!response)
  {
    request->send(500);
    return;
  }

  response->addHeader(F("Cache-Control"), F("no-store"));

  response->print(F("{\"Applications\":["));

  bool first_module = true;

  for(size_t i = 0; i < application_urls.size(); i++)
  {
    const uint16_t module_id = application_urls[i].module_id;

    // Only output this module once
    bool already_output = false;

    for(size_t j = 0; j < i; j++)
    {
      if(application_urls[j].module_id == module_id)
      {
        already_output = true;
        break;
      }
    }

    if(already_output) continue;

    char buffer[50];

    const char* module_name = tkr->GetModuleNameDisplayEachWord(module_id, buffer, sizeof(buffer));

    if(!module_name)
    {
      module_name = "Unknown";
    }

    if(!first_module)
    {
      response->print(',');
    }

    first_module = false;

    response->print(F("{\"Module\":\""));
    PrintJSONString(*response, module_name);
    response->print(F("\",\"Links\":["));

    bool first_link = true;

    for(size_t j = i; j < application_urls.size(); j++)
    {
      if(application_urls[j].module_id != module_id) continue;

      if(!first_link) response->print(',');
      first_link = false;

      response->print('[');

      if(application_urls[j].friendly_name.length())
      {
        response->print('"');
        PrintJSONString(*response, application_urls[j].friendly_name.c_str());
      }
      else
      {
        response->print('"');
        PrintJSONString(*response, application_urls[j].url.c_str());
      }

      response->print(F("\",\""));
      PrintJSONString(*response, application_urls[j].url.c_str());
      response->print(F("\","));
      response->print(application_urls[j].port);
      response->print(']');
    }

    response->print(F("]}"));
  }

  response->print(F("]}"));

  request->send(response);
}


#ifdef ENABLE_FEATURE_WEBSERVER__ADVANCED_URL_LIST

void mWebServer::HandlePage_UrlList(AsyncWebServerRequest *request)
{
  if (captivePortal(request)) return;
  handleStaticContent(request,F("/url_list.htm"),200,FPSTR(CONTENT_TYPE_HTML),PAGE_url_list,PAGE_url_list_length,true);
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
    PrintJSONString(*response, entry.url.c_str());
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




#endif // USE_MODULE_NETWORK_WEBSERVER