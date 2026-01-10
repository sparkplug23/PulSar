
#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_NETWORK_WEBSERVER


#include "mWebServer.h"


String messageHead, messageSub;
byte optionType;


// Helper function to avoid code duplication (saves 4k Flash)
void mWebServer::WebGetArg(AsyncWebServerRequest *request, const char* arg, char* out, size_t max)
{
  
  // #ifndef DISABLE_SERIAL_LOGGING
  // P_PHASE_OUT();
  // #endif

  String s = request->arg(arg);
  strlcpy(out, s.c_str(), max);

}






void mWebServer::createEditHandler(bool enable) 
{
  if (editHandler != nullptr) server->removeHandler(editHandler);
  if (enable) 
  {
    #ifdef WLED_ENABLE_FS_EDITOR
      #ifdef ARDUINO_ARCH_ESP32
      editHandler = &server->addHandler(new SPIFFSEditor(FILE_SYSTEM));
      #else
      editHandler = &server->addHandler(new SPIFFSEditor("","",FILE_SYSTEM));
      #endif
    #else
      editHandler = &server->on("/edit", HTTP_GET, [this](AsyncWebServerRequest *request){
        this->serveMessage(request, 501, "Not implemented", F("The FS editor is disabled in this build."), 254);
      });
    #endif
  } 
  else 
  {
    editHandler = &server->on("/edit", HTTP_ANY, [this](AsyncWebServerRequest *request){
      this->serveMessage(request, 500, "Access Denied", FPSTR(s_unlock_cfg), 254);
    });
  }
}

String mWebServer::msgProcessor(const String& var)
{
  if (var == "MSG") {
    String messageBody = messageHead;
    messageBody += F("</h2>");
    messageBody += messageSub;
    uint32_t optt = optionType;

    if (optt < 60) //redirect to settings after optionType seconds
    {
      messageBody += F("<script>setTimeout(RS,");
      messageBody +=String(optt*1000);
      messageBody += F(")</script>");
    } else if (optt < 120) //redirect back after optionType-60 seconds, unused
    {
      //messageBody += "<script>setTimeout(B," + String((optt-60)*1000) + ")</script>";
    } else if (optt < 180) //reload parent after optionType-120 seconds
    {
      messageBody += F("<script>setTimeout(RP,");
      messageBody += String((optt-120)*1000);
      messageBody += F(")</script>");
    } else if (optt == 253)
    {
      messageBody += F("<br><br><form action=/settings><button class=\"bt\" type=submit>Back</button></form>"); //button to settings
    } else if (optt == 254)
    {
      messageBody += F("<br><br><button type=\"button\" class=\"bt\" onclick=\"B()\">Back</button>");
    }
    return messageBody;
  }
  return String();
}


void mWebServer::serveMessage(AsyncWebServerRequest* request, uint16_t code, const String& headl, const String& subl, byte optionT)
{
  messageHead = headl;
  messageSub = subl;
  optionType = optionT;

  request->send_P(code, "text/html", PAGE_msg, msgProcessor);
}



// -----------------------------------------------------------------------------
// serveRedirectMessage
//
// SUMMARY
//   Sends a minimal HTML response which optionally displays a message and then
//   redirects the browser to a specified absolute URL.
//
// ARGUMENTS
//   request      AsyncWebServerRequest*
//   httpCode     e.g. 200
//   msg          short message (may be nullptr)
//   detail       optional detail (may be nullptr)
//   redirectUrl  absolute URL recommended (e.g. "/m/serverrelays")
//   delayMs      delay before redirect (e.g. 750)
//
// RETURNS
//   void
//
// CHANGED
//   05Jan26  Initial.
// -----------------------------------------------------------------------------
void mWebServer::serveRedirectMessage(AsyncWebServerRequest* request,
                                      int httpCode,
                                      const __FlashStringHelper* msg,
                                      const __FlashStringHelper* detail,
                                      const char* redirectUrl,
                                      uint32_t delayMs)
{
  if (!request) return;

  // Must be a valid absolute-ish URL. You can relax this later if you want.
  if (!redirectUrl || redirectUrl[0] == '\0') {
    request->send(500, FPSTR(CONTENT_TYPE_HTML), PSTR("Missing redirectUrl"));
    return;
  }

  AsyncResponseStream* response = request->beginResponseStream(FPSTR(CONTENT_TYPE_HTML));
  response->setCode(httpCode);

  // Cache policy: align with WLED message behavior (no-store)
  response->addHeader(F("Cache-Control"), F("no-store"));
  response->addHeader(F("Expires"), F("0"));

  // Minimal HTML. Use absolute /style.css to avoid /m/style.css mistakes.
  response->print(F("<!DOCTYPE html><html><head>"
                    "<meta charset='utf-8'>"
                    "<meta content='width=device-width,initial-scale=1.0,maximum-scale=1.0,user-scalable=no' name='viewport'>"
                    "<title>PulSar</title>"
                    "<style>@import url('/style.css');</style>"
                    "<script>"));

  // Emit redirect JS (delay optional)
  response->print(F("function Go(){var u='"));
  response->print(redirectUrl);              // assume you pass safe URL (no quotes). If not, escape it.
  response->print(F("';var d="));
  response->print(delayMs);
  response->print(F(";if(!u)return;setTimeout(function(){location.href=u;},d);}"));
  response->print(F("</script></head><body onload='Go()'>"));

  // Optional message. If you want “empty body”, delete this section.
  if (msg)    { response->print(F("<h2>")); response->print(msg);    response->print(F("</h2>")); }
  if (detail) { response->print(F("<div>")); response->print(detail); response->print(F("</div>")); }

  response->print(F("</body></html>"));
  request->send(response);
}





void mWebServer::WebPage_Root_AddHandlers(){

  server->on("/", HTTP_GET, [this](AsyncWebServerRequest *request){
    if (captivePortal(request)) return;
    this->handleStaticContent(request, F("/"), 200, FPSTR(CONTENT_TYPE_HTML), PAGE_root_basic, PAGE_root_basic_length, true);
  });

  server->on("/version", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", (String)PROJECT_VERSION);
  });

  server->on("/uptime", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", (String)millis());
  });

  server->on("/reboot", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->redirect("/reset");
  });

  server->on("/reset", HTTP_GET, [this](AsyncWebServerRequest *request){
    serveMessage(request, 200, F("Rebooting now..."), F("Please wait ~10 seconds..."), 129);
    #ifdef USE_MODULE_LIGHTS_INTERFACE
    tkr_sup->ESP_Restart_InSeconds(1);
    #endif
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
  #ifdef ENABLE_DEVFEATURE_NETWORK__CONSOLE_WEBSOCKET
  server->on("/console", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->HandlePage_Console(request);
  });
  #endif
  #ifdef ENABLE_DEVFEATURE_NETWORK__CONSOLE_POLLING
  server->on("/console_poll", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->HandlePage_Console_Poll(request);
  });
  #endif


#ifdef ENABLE_DEVFEATURE_WEBSERVER__SETTINGS_WEBPAGES



server->on("/settings2", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->SettingsPages_GET(request);
  });

server->on("/settings2", HTTP_POST, [this](AsyncWebServerRequest *request){
    this->SettingsPages_POST(request);
  });

  server->on("/json2", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->serveJson(request);
  });

#endif


  
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
    ALOG_ERR(PSTR("HTTP URI Not-Found: %s"), request->url());    
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


//   server->onNotFound([this](AsyncWebServerRequest *request){HandleNotFound(request); });
  
}


bool mWebServer::HttpCheckPriviledgedAccess()
{

//     bool autorequestauth = true;

//   if (HTTP_USER == webserver_state) {
//     HandleRoot();
//     return false;
//   }
//   if (autorequestauth && !WebAuthenticate()) {
//     server->requestAuthentication();
//     return false;
//   }
  return true; // admin by default
}









// *********************************************************************************************
//  * Interface
// *********************************************************************************************

int8_t mWebServer::Tasker(uint8_t function, JsonParserObject obj)
{

// DEBUG_LINE_HERE;

  switch(function)
  {
    case TASK_INIT:
     
      server = new AsyncWebServer(80);

      #ifdef ENABLE_DEVFEATURE_LIGHTING__JSONLIVE_WEBSOCKETS
      websocket_lights = new AsyncWebSocket("/ws");
      #endif

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
      #ifdef WLED_ENABLE_WEBSOCKETS2
      websocket_lights->closeAll(1012);
      #endif
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

      #ifdef ENABLE_DEVFEATURE_NETWORK__CONSOLE_WEBSOCKET
      handleConsoleWs();
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

  #ifdef WLED_ENABLE_WEBSOCKETS2
  server->addHandler(websocket_lights);
  #endif
  #ifdef ENABLE_DEVFEATURE_NETWORK__CONSOLE_WEBSOCKET
  server->addHandler(websocket_console);
  #endif

  // Let modules add their own URLs
  tkr->Tasker_Interface(TASK_WEB_ADD_HANDLER);

  server->begin();
  
  ALOG_INF(PSTR(D_LOG_HTTP "Webserver started on http://%s.local or http://%s"), cmDNS, tkr_set->runtime.my_hostname);

  ALOG_HGLB(PSTR(D_LOG_HTTP "Started web server"));
}

#endif