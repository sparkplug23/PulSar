// File contains the formation of pages, while "mWebServer" contains all parsing and commands
// Mostly for refactoring

#include "_WebServer.h"

#ifdef USE_MODULE_NETWORK_WEBSERVER

size_t mWebServer::printSetFormInput(Print& settingsScript, const char* key, const char* selector, int value) {
  return settingsScript.printf_P(PSTR("d.Sf.%s.%s=%d;"), key, selector, value);
}

size_t mWebServer::printSetFormCheckbox(Print& settingsScript, const char* key, int val) {
  return printSetFormInput(settingsScript, key, PSTR("checked"), val);
}
size_t mWebServer::printSetFormValue(Print& settingsScript, const char* key, int val) {
  return printSetFormInput(settingsScript, key, PSTR("value"), val);
}
size_t mWebServer::printSetFormIndex(Print& settingsScript, const char* key, int index) {
  return printSetFormInput(settingsScript, key, PSTR("selectedIndex"), index);
}

size_t mWebServer::printSetFormValue(Print& settingsScript, const char* key, const char* val) {
  return settingsScript.printf_P(PSTR("d.Sf.%s.value=\"%s\";"),key,val);
}

size_t mWebServer::printSetClassElementHTML(Print& settingsScript, const char* key, const int index, const char* val) {
  return settingsScript.printf_P(PSTR("d.getElementsByClassName(\"%s\")[%d].innerHTML=\"%s\";"), key, index, val);
}
// size_t mWebServer::printSetElementStyle(
//   Print& settingsScript,
//   const char* element_id,
//   const char* css_prop,
//   const char* css_val
// ) {
//   return settingsScript.printf_P(
//     PSTR("var e=d.getElementById(\"%s\");if(e)e.style.%s=\"%s\";"),
//     element_id, css_prop, css_val
//   );
// }

// size_t mWebServer::printToggleElementClass(
//   Print& settingsScript,
//   const char* element_id,
//   const char* class_name,
//   bool enable
// ) {
//   return settingsScript.printf_P(
//     PSTR(
//       "var e=d.getElementById(\"%s\");"
//       "if(e){e.classList.%s(\"%s\");}"
//     ),
//     element_id,
//     enable ? "add" : "remove",
//     class_name
//   );
// }
size_t mWebServer::printSetElementHTML(
  Print& settingsScript,
  const char* element_id,
  const char* val
) {
  return settingsScript.printf_P(
    PSTR("var e=d.getElementById(\"%s\");if(e)e.innerHTML=\"%s\";"),
    element_id, val
  );
}
size_t mWebServer::printTableSetCell(Print& s, const char* table_id, uint16_t row, uint8_t col, const char* val) {
  return s.printf_P(
    PSTR("TSet('%s',%u,%u,\"%s\");"),
    table_id, row, col, val
  );
}

size_t mWebServer::printTableAddRow(Print& s, const char* table_id, uint16_t row, const char* c0, const char* c1, const char* c2) {
  return s.printf_P(
    PSTR("TRow('%s',%u,\"%s\",\"%s\",\"%s\");"),
    table_id, row, c0, c1, c2
  );
}

size_t mWebServer::printTableClear(Print& s, const char* table_id) {
  return s.printf_P(PSTR("TClear('%s');"), table_id);
}

size_t mWebServer::printTableSetValue(Print& s, const char* table_id, uint16_t row, const char* val) {
  return s.printf_P(
    PSTR("TSetValue('%s',%u,\"%s\");"),
    table_id, row, val
  );
}

size_t mWebServer::printTableSetNotes(Print& s, const char* table_id, uint16_t row, const char* val) {
  return s.printf_P(
    PSTR("TSetNotes('%s',%u,\"%s\");"),
    table_id, row, val
  );
}


// // Date Modified: 25Jan26

// size_t mWebServer::WebUI_Print_SectionBegin(Print& out, const char* title)
// {
//   size_t n = 0;
//   n += out.print(F("<div class=\"section\"><div class=\"title\">"));
//   if (title) n += out.print(title);
//   n += out.print(F("</div>"));
//   return n;
// }

// size_t mWebServer::WebUI_Print_SectionEnd(Print& out)
// {
//   return out.print(F("</div>"));
// }

// size_t mWebServer::WebUI_Print_TableBegin(Print& out)
// {
//   return out.print(F("<table class=\"kv\">"));
// }

// size_t mWebServer::WebUI_Print_TableEnd(Print& out)
// {
//   return out.print(F("</table>"));
// }

// size_t mWebServer::WebUI_Print_KV_Float(Print& out, const char* key, float value, uint8_t decimals, const char* units)
// {
//   size_t n = 0;
//   n += out.print(F("<tr><td class=\"key\">"));
//   if (key) n += out.print(key);
//   n += out.print(F("</td><td class=\"val\">"));
//   n += out.print(value, decimals);
//   if (units) { n += out.print(' '); n += out.print(units); }
//   n += out.print(F("</td></tr>"));
//   return n;
// }

// size_t mWebServer::WebUI_Print_KV_U32(Print& out, const char* key, uint32_t value, const char* units)
// {
//   size_t n = 0;
//   n += out.print(F("<tr><td class=\"key\">"));
//   if (key) n += out.print(key);
//   n += out.print(F("</td><td class=\"val\">"));
//   n += out.print(value);
//   if (units) { n += out.print(' '); n += out.print(units); }
//   n += out.print(F("</td></tr>"));
//   return n;
// }

// size_t mWebServer::WebUI_Print_KV_Str(Print& out, const char* key, const char* value)
// {
//   size_t n = 0;
//   n += out.print(F("<tr><td class=\"key\">"));
//   if (key) n += out.print(key);
//   n += out.print(F("</td><td class=\"val\">"));
//   if (value) n += out.print(value);
//   n += out.print(F("</td></tr>"));
//   return n;
// }
// // Date Modified: 25Jan26

// size_t mWebServer::WebUI_Print_TableHeaderRow_Begin(Print& out){
//   return out.print(F("<tr>"));
// }

// size_t mWebServer::WebUI_Print_TableHeaderCell(Print& out, const char* text, bool is_first_blank){
//   size_t n = 0;
//   if (is_first_blank) {
//     n += out.print(F("<th class=\"key\"></th>"));
//     return n;
//   }
//   n += out.print(F("<th class=\"col\">"));
//   if (text) n += out.print(text);
//   n += out.print(F("</th>"));
//   return n;
// }

// size_t mWebServer::WebUI_Print_TableHeaderRow_End(Print& out){
//   return out.print(F("</tr>"));
// }

// size_t mWebServer::WebUI_Print_RowBegin(Print& out, const char* key){
//   size_t n = 0;
//   n += out.print(F("<tr><td class=\"key\">"));
//   if (key) n += out.print(key);
//   n += out.print(F("</td>"));
//   return n;
// }

// size_t mWebServer::WebUI_Print_CellText(Print& out, const char* text){
//   size_t n = 0;
//   n += out.print(F("<td class=\"val\">"));
//   if (text) n += out.print(text);
//   n += out.print(F("</td>"));
//   return n;
// }

// size_t mWebServer::WebUI_Print_CellFloat(Print& out, float value, uint8_t decimals, const char* units){
//   size_t n = 0;
//   n += out.print(F("<td class=\"val\">"));
//   n += out.print(value, decimals);
//   if (units) { n += out.print(' '); n += out.print(units); }
//   n += out.print(F("</td>"));
//   return n;
// }

// size_t mWebServer::WebUI_Print_CellDash(Print& out){
//   return out.print(F("<td class=\"val\">—</td>"));
// }

// size_t mWebServer::WebUI_Print_RowEnd(Print& out){
//   return out.print(F("</tr>"));
// }





// Global buffer locking response helper class (to make sure lock is released when AsyncJsonResponse is destroyed)
class LockedJsonResponse2:
  public AsyncJsonResponse 
{
  bool _holding_lock;
  public:
  // WARNING: constructor assumes requestJSONBufferLock() was successfully acquired externally/prior to constructing the instance
  // Not a good practice with C++. Unfortunately AsyncJsonResponse only has 2 constructors - for dynamic buffer or existing buffer,
  // with existing buffer it clears its content during construction
  // if the lock was not acquired (using JSONBufferGuard class) previous implementation still cleared existing buffer
  inline LockedJsonResponse2(JsonDocument* doc, bool isArray) : AsyncJsonResponse(doc, isArray), _holding_lock(true) {};

  virtual size_t _fillBuffer(uint8_t *buf, size_t maxLen) { 
    size_t result = AsyncJsonResponse::_fillBuffer(buf, maxLen);
    // Release lock as soon as we're done filling content
    if (((result + _sentLength) >= (_contentLength)) && _holding_lock) {
      tkr_jsona->releaseJSONBufferLock();
      _holding_lock = false;
    }
    return result;
  }

  // destructor will remove JSON buffer lock when response is destroyed in AsyncWebServer
  virtual ~LockedJsonResponse2() { if (_holding_lock) tkr_jsona->releaseJSONBufferLock(); };
};



/*
 * Sending XML status files to client
 */

//build XML response to HTTP /win API request
void mWebServer::XML_response(Print& dest)
{
  #ifdef USE_MODULE_LIGHTS_ANIMATOR
  dest.printf_P(PSTR("<?xml version=\"1.0\" ?><vs><ac>%d</ac>"), ( tkr_anim->nightlightActive && tkr_anim->nightlightMode > NL_MODE_SET) ? tkr_anim->briT : tkr_anim->getBrightness());
  for (int i = 0; i < 3; i++)
  {
   dest.printf_P(PSTR("<cl>%d</cl>"), tkr_anim->col[i]);
  }
  for (int i = 0; i < 3; i++)
  {
    dest.printf_P(PSTR("<cs>%d</cs>"), tkr_anim->colSec[i]);
  }
  dest.printf_P(PSTR("<ns>%d</ns><nr>%d</nr><nl>%d</nl><nf>%d</nf><nd>%d</nd><nt>%d</nt><fx>%d</fx><sx>%d</sx><ix>%d</ix><fp>%d</fp><wv>%d</wv><ws>%d</ws><ps>%d</ps><cy>%d</cy><ds>%s%s</ds><ss>%d</ss></vs>"),
    tkr_anim->notifyDirect, tkr_anim->receiveGroups!=0, tkr_anim->nightlightActive, tkr_anim->nightlightMode > NL_MODE_SET, tkr_anim->nightlightDelayMins,
    tkr_anim->nightlightTargetBri, tkr_anim->effectCurrent, tkr_anim->effectSpeed, tkr_anim->effectIntensity, tkr_anim->effectPalette,
    tkr_anim->hasWhiteChannel() ? tkr_anim->col[3] : -1, tkr_anim->colSec[3], tkr_anim->currentPreset, tkr_anim->currentPlaylist >= 0,
    tkr_anim->serverDescription, tkr_anim->realtimeMode ? PSTR(" (live)") : "",
    tkr_anim->getFirstSelectedSegId()
  );
  #endif
}

// void mWebServer::extractPin(Print& settingsScript, JsonObject &obj, const char *key) {
//   if (obj[key].is<JsonArray>()) {
//     JsonArray pins = obj[key].as<JsonArray>();
//     for (JsonVariant pv : pins) {
//       if (pv.as<int>() > -1) { settingsScript.print(","); settingsScript.print(pv.as<int>()); }
//     }
//   } else {
//     if (obj[key].as<int>() > -1) { settingsScript.print(","); settingsScript.print(obj[key].as<int>()); }
//   }
// }

// // print used pins by scanning JsonObject (1 level deep)
// void mWebServer::fillUMPins(Print& settingsScript, JsonObject &mods)
// {
//   for (JsonPair kv : mods) {
//     // kv.key() is usermod name or subobject key
//     // kv.value() is object itself
//     JsonObject obj = kv.value();
//     if (!obj.isNull()) {
//       // element is an JsonObject
//       if (!obj["pin"].isNull()) {
//         extractPin(settingsScript, obj, "pin");
//       } else {
//         // scan keys (just one level deep as is possible with usermods)
//         for (JsonPair so : obj) {
//           const char *key = so.key().c_str();
//           if (strstr(key, "pin")) {
//             // we found a key containing "pin" substring
//             if (strlen(strstr(key, "pin")) == 3) {
//               // and it is at the end, we found another pin
//               extractPin(settingsScript, obj, key);
//               continue;
//             }
//           }
//           if (!obj[so.key()].is<JsonObject>()) continue;
//           JsonObject subObj = obj[so.key()];
//           if (!subObj["pin"].isNull()) {
//             // get pins from subobject
//             extractPin(settingsScript, subObj, "pin");
//           }
//         }
//       }
//     }
//   }
// }

void mWebServer::appendGPIOinfo(Print& settingsScript) {
  settingsScript.print(F("d.um_p=[-1")); // has to have 1 element

  int i2c_sda = -1, i2c_scl = -1; //WLED FIX
  int spi_mosi = -1, spi_sclk = -1; //WLED FIX

  if (i2c_sda > -1 && i2c_scl > -1) {
    settingsScript.printf_P(PSTR(",%d,%d"), i2c_sda, i2c_scl);
  }
  if (spi_mosi > -1 && spi_sclk > -1) {
    settingsScript.printf_P(PSTR(",%d,%d"), spi_mosi, spi_sclk);
  }
  // usermod pin reservations will become unnecessary when settings pages will read cfg.json directly
  if (tkr_jsona->requestJSONBufferLock(6)) {
    // if we can't allocate JSON buffer ignore usermod pins
    JsonObject mods = tkr_jsona->pDoc->createNestedObject("um");
    // UsermodManager::addToConfig(mods);
    // if (!mods.isNull()) fillUMPins(settingsScript, mods);
    tkr_jsona->releaseJSONBufferLock();
  }
  settingsScript.print(F("];"));

  // add reserved (unusable) pins
  bool firstPin = true;
  settingsScript.print(F("d.rsvd=["));
  int WLED_NUM_PINS = 2; // WLED FIX
  for (unsigned i = 0; i < WLED_NUM_PINS; i++) {
    if (0){//!PinManager::isPinOk(i, false)) {  // include readonly pins
      if (!firstPin) settingsScript.print(',');
      settingsScript.print(i);
      firstPin = false;
    }
  }
  #ifdef ENABLE_FEATURE_LIGHTING__DMX
  if (!firstPin) settingsScript.print(',');
  settingsScript.print(2); // DMX hardcoded pin
  firstPin = false;
  #endif
  #if !defined(WLED_DEBUG_HOST)
  if (!firstPin) settingsScript.print(',');
  int hardwareTX = -1; // WLED FIX
  settingsScript.print(hardwareTX); // debug output (TX) pin
  firstPin = false;
  #endif
  #ifdef WLED_USE_ETHERNET
  if (ethernetType != WLED_ETH_NONE && ethernetType < WLED_NUM_ETH_TYPES) {
    if (!firstPin) settingsScript.print(',');
    for (unsigned p=0; p<WLED_ETH_RSVD_PINS_COUNT; p++) { settingsScript.printf("%d,",esp32_nonconfigurable_ethernet_pins[p].pin); }
    if (ethernetBoards[ethernetType].eth_power >= 0)    { settingsScript.printf("%d,",ethernetBoards[ethernetType].eth_power); }
    if (ethernetBoards[ethernetType].eth_mdc >= 0)      { settingsScript.printf("%d,",ethernetBoards[ethernetType].eth_mdc); }
    if (ethernetBoards[ethernetType].eth_mdio >= 0)     { settingsScript.printf("%d,",ethernetBoards[ethernetType].eth_mdio); }
    switch (ethernetBoards[ethernetType].eth_clk_mode)  {
      case ETH_CLOCK_GPIO0_IN:
      case ETH_CLOCK_GPIO0_OUT:
        settingsScript.print(0);
        break;
      case ETH_CLOCK_GPIO16_OUT:
        settingsScript.print(16);
        break;
      case ETH_CLOCK_GPIO17_OUT:
        settingsScript.print(17);
        break;
    }
  }
  #endif
  settingsScript.print(F("];")); // rsvd

  // add info for read-only GPIO
  settingsScript.print(F("d.ro_gpio=["));
  firstPin = true;
  for (unsigned i = 0; i < WLED_NUM_PINS; i++) {
    if (0){//PinManager::isReadOnlyPin(i)) {
      // No comma before the first pin
      if (!firstPin) settingsScript.print(',');
      settingsScript.print(i);
      firstPin = false;
    }
  }
  settingsScript.print(F("];"));

  // add info about max. # of pins
  settingsScript.printf_P(PSTR("d.max_gpio=%d;"),WLED_NUM_PINS);
}

//get values for settings form in javascript
void mWebServer::getSettingsJS(byte subPage, Print& settingsScript)
{
  //0: menu 1: wifi 2: leds 3: ui 4: sync 5: time 6: sec
  DEBUG_PRINTF_P(PSTR("settings resp %u\n"), (unsigned)subPage);

  if (subPage <0 || subPage >10) return;
  char nS[32];

  if (subPage == WebSettingsSubPage::MENU)
  {
  #ifndef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS // include only if 2D is not compiled in
    settingsScript.print(F("gId('2dbtn').style.display='none';"));
  #endif
  #ifdef ENABLE_FEATURE_LIGHTING__DMX // include only if DMX is enabled
    settingsScript.print(F("gId('dmxbtn').style.display='';"));
  #endif
  }


  if (subPage == WebSettingsSubPage::SECURITY)
  {
    // byte l = strlen(settingsPIN);
    // char fpass[l+1]; //fill PIN field with 0000
    // fpass[l] = 0;
    // memset(fpass,'0',l);
    // printSetFormValue(settingsScript,PSTR("PIN"),fpass);
    // printSetFormCheckbox(settingsScript,PSTR("NO"),otaLock);
    // printSetFormCheckbox(settingsScript,PSTR("OW"),wifiLock);
    // printSetFormCheckbox(settingsScript,PSTR("AO"),aOtaEnabled);
    // char tmp_buf[128];
    // snprintf_P(tmp_buf,sizeof(tmp_buf),PSTR("WLED %s (build %d)"),versionString, PROJECT_VERSION);
    // printSetClassElementHTML(settingsScript,PSTR("sip"),0,tmp_buf);
    // settingsScript.printf_P(PSTR("sd=\"%s\";"), serverDescription);
  }

  #ifdef ENABLE_FEATURE_LIGHTING__DMX // include only if DMX is enabled
  if (subPage == WebSettingsSubPage::DMX)
  {
    printSetFormValue(settingsScript,PSTR("PU"),e131ProxyUniverse);

    printSetFormValue(settingsScript,PSTR("CN"),DMXChannels);
    printSetFormValue(settingsScript,PSTR("CG"),DMXGap);
    printSetFormValue(settingsScript,PSTR("CS"),DMXStart);
    printSetFormValue(settingsScript,PSTR("SL"),DMXStartLED);

    printSetFormIndex(settingsScript,PSTR("CH1"),DMXFixtureMap[0]);
    printSetFormIndex(settingsScript,PSTR("CH2"),DMXFixtureMap[1]);
    printSetFormIndex(settingsScript,PSTR("CH3"),DMXFixtureMap[2]);
    printSetFormIndex(settingsScript,PSTR("CH4"),DMXFixtureMap[3]);
    printSetFormIndex(settingsScript,PSTR("CH5"),DMXFixtureMap[4]);
    printSetFormIndex(settingsScript,PSTR("CH6"),DMXFixtureMap[5]);
    printSetFormIndex(settingsScript,PSTR("CH7"),DMXFixtureMap[6]);
    printSetFormIndex(settingsScript,PSTR("CH8"),DMXFixtureMap[7]);
    printSetFormIndex(settingsScript,PSTR("CH9"),DMXFixtureMap[8]);
    printSetFormIndex(settingsScript,PSTR("CH10"),DMXFixtureMap[9]);
    printSetFormIndex(settingsScript,PSTR("CH11"),DMXFixtureMap[10]);
    printSetFormIndex(settingsScript,PSTR("CH12"),DMXFixtureMap[11]);
    printSetFormIndex(settingsScript,PSTR("CH13"),DMXFixtureMap[12]);
    printSetFormIndex(settingsScript,PSTR("CH14"),DMXFixtureMap[13]);
    printSetFormIndex(settingsScript,PSTR("CH15"),DMXFixtureMap[14]);
  }
  #endif

}



void mWebServer::serveJson(AsyncWebServerRequest* request)
{

  byte subJson = 0;
  const String& url = request->url();
  if      (url.indexOf("state") > 0) subJson = JSON_PATH_WEB_STATE;
  else if (url.indexOf("info")  > 0) subJson = JSON_PATH_WEB_INFO;
  else if (url.indexOf("si")    > 0) subJson = JSON_PATH_WEB_STATE_INFO;
  else if (url.indexOf("nodes") > 0) subJson = JSON_PATH_WEB_NODES;
  else if (url.indexOf("eff")   > 0) subJson = JSON_PATH_WEB_EFFECTS;
  else if (url.indexOf("palx")  > 0) subJson = JSON_PATH_WEB_PALETTES;
  else if (url.indexOf("fxda")  > 0) subJson = JSON_PATH_WEB_FXDATA;
  else if (url.indexOf("net")   > 0) subJson = JSON_PATH_WEB_NETWORKS;
  else if (url.indexOf("live")  > 0) { 
    // tkr_anim->serveLiveLeds(request);
    return;
  }
  else if (url.indexOf("pal") > 0) { // "/json/palettes" - names only (flat array)
  
    
  return;
}
  else if (url.indexOf("cfg") > 0 && tkr_mfile->handleFileRead(request, "/cfg.json")) {
    return;
  }
  else if (url.length() > 6) { //not just /json
    request->send(501, "application/json", F("{\"error\":\"Not implemented\"}"));
    return;
  }

  // if (!tkr_anim->requestJSONBufferLock(17)) {
    request->send(503, "application/json", F("{\"error\":3}"));
    return;
  // }

  // AsyncJsonResponse *response = new AsyncJsonResponse(tkr_jsona->pDoc, subJson==JSON_PATH_FXDATA || subJson==JSON_PATH_EFFECTS); // will clear and convert JsonDocument into JsonArray if necessary

  // releaseJSONBufferLock() will be called when "response" is destroyed (from AsyncWebServer)
  // make sure you delete "response" if no "request->send(response);" is made
  LockedJsonResponse2 *response = new LockedJsonResponse2(tkr_jsona->pDoc, subJson==JSON_PATH_WEB_FXDATA || subJson==JSON_PATH_WEB_EFFECTS); // will clear and convert JsonDocument into JsonArray if necessary



  JsonVariant lDoc = response->getRoot();

  // switch (subJson)
  // {
  //   case JSON_PATH_STATE:
  //     tkr_anim->serializeState(lDoc); 
  //   break;
  //   case JSON_PATH_INFO:
  //     tkr_anim->serializeInfo(lDoc);     
  //   break;
  //   case JSON_PATH_PALETTES:
  //     tkr_anim->serializePalettes(lDoc, request->hasParam("page") ? request->getParam("page")->value().toInt() : 0); 
  //   break;
  //   case JSON_PATH_EFFECTS:
  //     tkr_anim->serializeModeNames(lDoc); 
  //   break;
  //   case JSON_PATH_FXDATA:
  //     tkr_anim->serializeModeData(lDoc); 
  //   break;
  //   case JSON_PATH_NETWORKS:
  //     tkr_anim->serializeNetworks(lDoc); 
  //   break;
  //   default: // All
  //     JsonObject state = lDoc.createNestedObject("state");
  //     tkr_anim->serializeState(state);
  //     JsonObject info = lDoc.createNestedObject("info");
  //     tkr_anim->serializeInfo(info);

  //     tkr_anim->force_update(); // New data in, so we should update

  //     if (subJson != JSON_PATH_STATE_INFO)
  //     {
  //       JsonArray effects = lDoc.createNestedArray(F("effects"));
  //       tkr_anim->serializeModeNames(effects);

  //       bool flag_get_first_name_only = true;        
  //       char lineBuffer[100] = {0};
  //       JsonArray pal = lDoc.createNestedArray(F("palettes"));
  //       for(uint16_t i = 0; i < mPaletteI->GetPaletteListLength(); i++)
  //       {
  //         tkr_anim->GetPaletteNameByID(i, lineBuffer, sizeof(lineBuffer));
  //         if(flag_get_first_name_only)
  //         {    
  //           char* dataPtr = strchr(lineBuffer,'|');
  //           if (dataPtr) *dataPtr = 0; // replace name dividor with null termination early
  //           // Serial.println(lineBuffer);
  //         }
  //         pal.add(lineBuffer);
  //       }

  //     }
  //     lDoc["m"] = lDoc.memoryUsage(); // JSON buffer usage, for remote debugging
  // }

  #ifdef ENABLE_DEBUG__JSON_BUFFER_LOCKS
  ALOG_DBG(PSTR("JSON buffer size: %u for request: %d\n"), lDoc.memoryUsage(), subJson);
  #endif
  
  size_t len = response->setLength();
  ALOG_DBG(PSTR("JSON content length: %d"), len);

  request->send(response);
  // tkr_anim->releaseJSONBufferLock();
}


void mWebServer::serveSettingsJS(AsyncWebServerRequest* request)
{

  ALOG_INF(PSTR("serveSettingsJS url %s"), request->url().c_str());

  // static const char _common_js[] PROGMEM = "/common.js";
  // if (request->url().indexOf(FPSTR(_common_js)) > 0) {
  //   handleStaticContent(request, FPSTR(_common_js), 200, FPSTR(CONTENT_TYPE_JAVASCRIPT), JS_common_web, JS_common_web_length);
  //   return;
  // }
  
  byte subPage = request->arg(F("p")).toInt();
  if (subPage > 10) {
    request->send_P(501, FPSTR(CONTENT_TYPE_JAVASCRIPT), PSTR("alert('Settings for this request are not implemented.');"));
    return;
  }
  // if (subPage > 0 && !correctPIN && strlen(settingsPIN)>0) {
  //   request->send_P(401, FPSTR(CONTENT_TYPE_JAVASCRIPT), PSTR("alert('PIN incorrect.');"));
  //   return;
  // }
  
  AsyncResponseStream *response = request->beginResponseStream(FPSTR(CONTENT_TYPE_JAVASCRIPT));
  response->addHeader(F("Cache-Control"), F("no-store"));
  response->addHeader(F("Expires"), F("0"));

  response->print(F("function GetV(){var d=document;"));
  getSettingsJS(subPage, *response);
  response->print(F("}"));
  request->send(response);

}

/**
 * @brief Resolve settings subpage from URL path
 */
static inline WebSettingsSubPage GetSubPageID_FromURLPath(const String& url)
{
  if (url.indexOf("settings") >= 0)
  {
    if      (url.indexOf(".js")      > 0) return WebSettingsSubPage::JS;
    // else if (url.indexOf(".css")     > 0) return WebSettingsSubPage::CSS;
    else if (url.indexOf("network")  > 0) return WebSettingsSubPage::NETWORK;
    else if (url.indexOf("hardware") > 0) return WebSettingsSubPage::HARDWARE;
    else if (url.indexOf("system")   > 0) return WebSettingsSubPage::SYSTEM;
    else if (url.indexOf("modules")  > 0) return WebSettingsSubPage::MODULES;
    else if (url.indexOf("storage")  > 0) return WebSettingsSubPage::STORAGE;
    else if (url.indexOf("logging")  > 0) return WebSettingsSubPage::LOGGING;
    else if (url.indexOf("security") > 0) return WebSettingsSubPage::SECURITY;
    else if (url.indexOf("unlock")   > 0) return WebSettingsSubPage::PINREQ;
    else if (url.indexOf("welcome")  > 0) return WebSettingsSubPage::WELCOME;
  }

  return WebSettingsSubPage::MENU;
}


/**
 * @brief WebUI: Settings Pages (POST)
 */
void mWebServer::SettingsPages_POST(AsyncWebServerRequest* request)
{
  const String& url = request->url();
  WebSettingsSubPage subPage = GetSubPageID_FromURLPath(url);
  WebSettingsSubPage originalSubPage = subPage;

#ifdef ENABLE_FEATURE_WEBSERVER__PIN_PROTECTION

  const uint8_t subPageID = static_cast<uint8_t>(subPage);

  bool pinRequired =
    !correctPIN &&
    strlen(settingsPIN) > 0 &&
    subPageID > static_cast<uint8_t>(WebSettingsSubPage::MENU) &&
    subPageID <= static_cast<uint8_t>(WebSettingsSubPage::SECURITY);

  if (pinRequired)
  {
    originalSubPage = subPage;
    subPage = WebSettingsSubPage::PINREQ;
  }

  if ((subPage == WebSettingsSubPage::NETWORK && wifiLock && otaLock) ||
      (pinRequired && !correctPIN && (millis() - lastEditTime < PIN_RETRY_COOLDOWN)))
  {
    serveMessage(request, 401, FPSTR(s_accessdenied), FPSTR(s_unlock_ota), 254);
    return;
  }

#else
  bool pinRequired = false;
#endif

  SettingsPages__ParseForm(request, static_cast<uint8_t>(subPage));

  char s[32];
  char s2[45] = "";

  switch (subPage)
  {
    case WebSettingsSubPage::NETWORK:
      strcpy_P(s, PSTR("Network"));
      strcpy_P(s2, PSTR("Please reconnect if network settings changed"));
    break;

    case WebSettingsSubPage::HARDWARE: strcpy_P(s, PSTR("Hardware")); break;
    case WebSettingsSubPage::SYSTEM:   strcpy_P(s, PSTR("System"));   break;
    case WebSettingsSubPage::MODULES:  strcpy_P(s, PSTR("Modules"));  break;
    case WebSettingsSubPage::STORAGE:  strcpy_P(s, PSTR("Storage"));  break;
    case WebSettingsSubPage::LOGGING:  strcpy_P(s, PSTR("Logging"));  break;

    case WebSettingsSubPage::SECURITY:
      strcpy_P(s, PSTR("Security"));
      if (tkr_sup->ESP_Restart_Scheduled())
        strcpy_P(s2, PSTR("Rebooting, please wait ~10 seconds..."));
    break;

#ifdef ENABLE_FEATURE_WEBSERVER__PIN_PROTECTION
    case WebSettingsSubPage::PINREQ:
      strcpy_P(s, correctPIN ? PSTR("PIN accepted") : PSTR("PIN rejected"));
    break;
#endif

    default:
      strcpy_P(s, PSTR("Settings"));
    break;
  }

#ifdef ENABLE_FEATURE_WEBSERVER__PIN_PROTECTION
  if (subPage != WebSettingsSubPage::PINREQ)
    strcat_P(s, PSTR(" settings saved."));

  if (subPage == WebSettingsSubPage::PINREQ && correctPIN)
  {
    request->redirect(url);
    return;
  }
#endif

  if (!s2[0]) strcpy_P(s2, s_redirecting);

  const bool doReboot = tkr_sup->ESP_Restart_Scheduled();

  const bool redirectAfter9s =
    subPage == WebSettingsSubPage::NETWORK ||
    (subPage == WebSettingsSubPage::SECURITY && doReboot);

  serveMessage(
    request,
    !pinRequired ? 200 : 401,
    s,
    s2,
    redirectAfter9s ? 129 : (!pinRequired ? 1 : 3)
  );
}



/**
 * @brief WebUI: Settings Pages (GET)
 */
void mWebServer::SettingsPages_GET(AsyncWebServerRequest* request)
{
  const String& url = request->url();
  WebSettingsSubPage subPage = GetSubPageID_FromURLPath(url);

  ALOG_INF(
    PSTR("SettingsPages_GET url %s subPage %u"),
    url.c_str(),
    static_cast<uint8_t>(subPage)
  );

  int code = 200;
  String contentType = FPSTR(CONTENT_TYPE_HTML);
  const uint8_t* content = PAGE_settings_web;
  size_t len = PAGE_settings_web_length;

  switch (subPage)
  {
    case WebSettingsSubPage::NETWORK:  content = PAGE_settings_network_web;  len = PAGE_settings_network_web_length;  break;
    case WebSettingsSubPage::HARDWARE: content = PAGE_settings_hardware_web; len = PAGE_settings_hardware_web_length; break;
    case WebSettingsSubPage::SYSTEM:   content = PAGE_settings_system_web;   len = PAGE_settings_system_web_length;   break;
    case WebSettingsSubPage::MODULES:  content = PAGE_settings_modules_web;  len = PAGE_settings_modules_web_length;  break;
    case WebSettingsSubPage::STORAGE:  content = PAGE_settings_storage_web;  len = PAGE_settings_storage_web_length;  break;
    case WebSettingsSubPage::LOGGING:  content = PAGE_settings_logging_web;  len = PAGE_settings_logging_web_length;  break;
    case WebSettingsSubPage::SECURITY: content = PAGE_settings_security_web; len = PAGE_settings_security_web_length; break;
    case WebSettingsSubPage::PINREQ:   content = PAGE_settings_unlock_web; len = PAGE_settings_unlock_web_length; code = 401; break;
    case WebSettingsSubPage::JS:       serveSettingsJS(request); return;
    case WebSettingsSubPage::WELCOME:  content = PAGE_welcome_web; len = PAGE_welcome_web_length; break;
    case WebSettingsSubPage::MENU:
    default: break;
  }

  handleStaticContent(request, "", code, contentType, content, len);
}

#endif //   #ifdef USE_MODULE_NETWORK_WEBSERVER21