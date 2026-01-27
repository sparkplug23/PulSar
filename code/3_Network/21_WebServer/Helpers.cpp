#include "_WebServer.h"


#ifdef USE_MODULE_NETWORK_WEBSERVER


bool mWebServer::captivePortal(AsyncWebServerRequest *request)
{
  // Captive portal redirect only when AP is active
  const auto mode = WiFi.getMode();
  const bool ap_active = (mode == WIFI_AP) || (mode == WIFI_AP_STA);
  if (!ap_active) return false;

  if (!request->hasHeader("Host")) return false;

  String host = request->getHeader("Host")->value();

  // Strip optional ":port" so "4.3.2.1:80" is treated as IP
  const int colon = host.indexOf(':');
  if (colon > 0) host = host.substring(0, colon);

  // If host is already an IP (e.g. 4.3.2.1), don't redirect
  if (isIp(host)) return false;

  // Optional: allow your own hostname (device / device.local) to avoid loops
  const char* dev = tkr_set->Settings.system_name.device;
  if (dev && dev[0] != '\0')
  {
    if (host.equalsIgnoreCase(dev)) return false;

    String devLocal = String(dev) + ".local";
    if (host.equalsIgnoreCase(devLocal)) return false;
  }

  // Redirect to Wi-Fi setup page on AP IP (avoid hardcoding AP IP)
  const IPAddress apIP = WiFi.softAPIP();
  const String url = String(F("http://")) + apIP.toString() + D_CAPTIVE_PORTAL_URL_REDIRECT_PATH;

  AsyncWebServerResponse *response = request->beginResponse(302);
  response->addHeader(F("Location"), url);
  request->send(response);

  return true;
}


/*
  cacheInvalidate this causes the presets to reload!!
  The ETag (or Entity Tag) is a string that serves as a cache validation token.
  To enable better cache detection, I am adding the build time to this as each new compile should force a new load
*/
void mWebServer::generateEtag(char *etag, uint16_t eTagSuffix) {
  snprintf_P(etag, 32, PSTR("%7d-%02x-%04x-BT%S"), PROJECT_VERSION, cacheInvalidate, eTagSuffix, PSTR(__TIME__));
}


void mWebServer::setStaticContentCacheHeaders(AsyncWebServerResponse *response, int code, uint16_t eTagSuffix ) 
{

  // Only send ETag for 200 (OK) responses
  if (code != 200) return;

  // https://medium.com/@codebyamir/a-web-developers-guide-to-browser-caching-cc41f3b73e7c
  #ifndef ENABLE_DEBUGFEATURE_NETWORK__DISABLE_CACHE
  // this header name is misleading, "no-cache" will not disable cache,
  // it just revalidates on every load using the "If-None-Match" header with the last ETag value
  response->addHeader(F("Cache-Control"), F("no-cache"));
  #else
  response->addHeader(F("Cache-Control"), F("no-store,max-age=0"));  // prevent caching if debug build
  #endif
  char etag[32];
  tkr_web->generateEtag(etag, eTagSuffix);
  response->addHeader(F("ETag"), etag);
}


bool mWebServer::handleIfNoneMatchCacheHeader(AsyncWebServerRequest *request, int code, uint16_t eTagSuffix) {
  // Only send 304 (Not Modified) if response code is 200 (OK)
  if (code != 200) return false;

  AsyncWebHeader *header = request->getHeader(F("If-None-Match"));
  char etag[32];
  tkr_web->generateEtag(etag, eTagSuffix);
  if (header && header->value() == etag) {
    AsyncWebServerResponse *response = request->beginResponse(304);
    tkr_web->setStaticContentCacheHeaders(response, code, eTagSuffix);
    request->send(response);
    return true;
  }
  return false;
}

/**
 * Handles the request for a static file.
 * If the file was found in the filesystem, it will be sent to the client.
 * Otherwise it will be checked if the browser cached the file and if so, a 304 response will be sent.
 * If the file was not found in the filesystem and not in the browser cache, the request will be handled as a 200 response with the content of the page.
 *
 * @param request The request object
 * @param path If a file with this path exists in the filesystem, it will be sent to the client. Set to "" to skip this check.
 * @param code The HTTP status code
 * @param contentType The content type of the web page
 * @param content Content of the web page
 * @param len Length of the content
 * @param gzip Optional. Defaults to true. If false, the gzip header will not be added.
 * @param eTagSuffix Optional. Defaults to 0. A suffix that will be added to the ETag header. This can be used to invalidate the cache for a specific page.
 */
void mWebServer::handleStaticContent(AsyncWebServerRequest *request, const String &path, int code, const String &contentType, const uint8_t *content, size_t len, bool gzip, uint16_t eTagSuffix) {
  if (path != "" && tkr_mfile->handleFileRead(request, path)) return;
  if (handleIfNoneMatchCacheHeader(request, code, eTagSuffix)) return;
  AsyncWebServerResponse *response = request->beginResponse_P(code, contentType, content, len);
  if (gzip) response->addHeader(FPSTR(s_content_enc), F("gzip"));
  setStaticContentCacheHeaders(response, code, eTagSuffix);
  request->send(response);
}


bool mWebServer::isIp(String str) {
  for (size_t i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}


bool mWebServer::WebGetArg(AsyncWebServerRequest* request,
                           const char* arg,
                           char* out,
                           size_t max)
{
  out[0] = '\0';

  if (!request->hasArg(arg)) {
    Serial.printf("WebGetArg: not found '%s'\n", arg);
    return false;
  }

  const String& v = request->arg(arg);  // minimal extra copies
  // Serial.printf("WebGetArg: '%s'='%s'\n", arg, v.c_str());
  strlcpy(out, v.c_str(), max);
  return true;
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



void mWebServer::createEditHandler(bool enable) 
{
  if (editHandler != nullptr) server->removeHandler(editHandler);
  if (enable) 
  {
    #ifdef USE_MODULE_CORE_FILESYSTEM
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



void mWebServer::serveMessage(AsyncWebServerRequest* request,
                              uint16_t code,
                              const String& headl,
                              const String& subl,
                              uint8_t optionT)
{
  // Capture per-request copies so concurrent requests cannot interfere.
  const String headCopy = headl;
  const String subCopy  = subl;
  const uint8_t optCopy = optionT;

  request->send_P(
    code,
    PSTR("text/html"),
    PAGE_msg_web,
    [headCopy, subCopy, optCopy](const String& var) -> String
    {
      if (var != F("MSG")) return String();

      String messageBody;
      messageBody.reserve(headCopy.length() + subCopy.length() + 96);

      messageBody += headCopy;
      messageBody += F("</h2>");
      messageBody += subCopy;

      const uint32_t optt = optCopy;

      if (optt < 60) // redirect to settings after optionType seconds
      {
        messageBody += F("<script>setTimeout(RS,");
        messageBody += String(optt * 1000UL);
        messageBody += F(")</script>");
      }
      else if (optt < 120) // redirect back after optionType-60 seconds (unused)
      {
        // messageBody += F("<script>setTimeout(B,");
        // messageBody += String((optt - 60UL) * 1000UL);
        // messageBody += F(")</script>");
      }
      else if (optt < 180) // reload parent after optionType-120 seconds
      {
        messageBody += F("<script>setTimeout(RP,");
        messageBody += String((optt - 120UL) * 1000UL);
        messageBody += F(")</script>");
      }
      else if (optt == 253)
      {
        messageBody += F("<br><br><form action=/settings><button class=\"bt\" type=submit>Back</button></form>");
      }
      else if (optt == 254)
      {
        messageBody += F("<br><br><button type=\"button\" class=\"bt\" onclick=\"B()\">Back</button>");
      }

      return messageBody;
    }
  );
}


#endif // USE_MODULE_NETWORK_WEBSERVER