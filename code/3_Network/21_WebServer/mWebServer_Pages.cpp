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
      JBI->releaseJSONBufferLock();
      _holding_lock = false;
    }
    return result;
  }

  // destructor will remove JSON buffer lock when response is destroyed in AsyncWebServer
  virtual ~LockedJsonResponse2() { if (_holding_lock) JBI->releaseJSONBufferLock(); };
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
  if (JBI->requestJSONBufferLock(6)) {
    // if we can't allocate JSON buffer ignore usermod pins
    JsonObject mods = tkr_mfile->pDoc->createNestedObject("um");
    // UsermodManager::addToConfig(mods);
    // if (!mods.isNull()) fillUMPins(settingsScript, mods);
    JBI->releaseJSONBufferLock();
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

  if (subPage == WebSettingsSubPage::WIFI)
  {
    size_t l;
    settingsScript.printf_P(PSTR("resetWiFi(%d);"), 0);//WLED_MAX_WIFI_COUNT);
    // for (size_t n = 0; n < multiWiFi.size(); n++) {
    //   l = strlen(multiWiFi[n].clientPass);
    //   char fpass[l+1]; //fill password field with ***
    //   fpass[l] = 0;
    //   memset(fpass,'*',l);
    //   settingsScript.printf_P(PSTR("addWiFi(\"%s\",\"%s\",0x%X,0x%X,0x%X);"),
    //     multiWiFi[n].clientSSID,
    //     fpass,
    //     (uint32_t) multiWiFi[n].staticIP, // explicit cast required as this is a struct
    //     (uint32_t) multiWiFi[n].staticGW,
    //     (uint32_t) multiWiFi[n].staticSN);
    // }

    IPAddress dnsAddress = IPAddress(8,8,8,8);

    printSetFormValue(settingsScript,PSTR("D0"),dnsAddress[0]);
    printSetFormValue(settingsScript,PSTR("D1"),dnsAddress[1]);
    printSetFormValue(settingsScript,PSTR("D2"),dnsAddress[2]);
    printSetFormValue(settingsScript,PSTR("D3"),dnsAddress[3]);

#ifdef USE_MODULE_LIGHTS_ANIMATOR
    printSetFormValue(settingsScript,PSTR("CM"), tkr_web->cmDNS);
    printSetFormIndex(settingsScript,PSTR("AB"),tkr_anim->apBehavior);
    printSetFormValue(settingsScript,PSTR("AS"),tkr_anim->apSSID);
    printSetFormCheckbox(settingsScript,PSTR("AH"),tkr_anim->apHide);

    l = strlen(tkr_anim->apPass);
    char fapass[l+1]; //fill password field with ***
    fapass[l] = 0;
    memset(fapass,'*',l);
    printSetFormValue(settingsScript,PSTR("AP"),0);//fapass);

    printSetFormValue(settingsScript,PSTR("AC"),1);//apChannel);
    #ifdef ARDUINO_ARCH_ESP32
    printSetFormValue(settingsScript,PSTR("TX"),0);//txPower);
    #else
    settingsScript.print(F("gId('tx').style.display='none';"));
    #endif
    printSetFormCheckbox(settingsScript,PSTR("FG"),0);//force802_3g);
    printSetFormCheckbox(settingsScript,PSTR("WS"),0);//noWifiSleep);

    #ifndef WLED_DISABLE_ESPNOW
    // printSetFormCheckbox(settingsScript,PSTR("RE"),0);//,enableESPNow);
    // printSetFormValue(settingsScript,PSTR("RMAC"),0);//,linked_remote);
    // #else
    //hide remote settings if not compiled
    // settingsScript.print(F("toggle('ESPNOW');"));  // hide ESP-NOW setting
    #endif

    #ifdef WLED_USE_ETHERNET
    printSetFormValue(settingsScript,PSTR("ETH"),ethernetType);
    #else
    //hide ethernet setting if not compiled in
    settingsScript.print(F("gId('ethd').style.display='none';"));
    #endif
  #endif

    if (Network.isConnected()) //is connected
    {
      char s[32];
      IPAddress localIP = Network.localIP();
      sprintf(s, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);

      #if defined(ARDUINO_ARCH_ESP32) && defined(WLED_USE_ETHERNET)
      if (Network.isEthernet()) strcat_P(s ,PSTR(" (Ethernet)"));
      #endif
      printSetClassElementHTML(settingsScript,PSTR("sip"),0,s);
    } else
    {
      printSetClassElementHTML(settingsScript,PSTR("sip"),0,(char*)F("Not connected"));
    }

    if (WiFi.softAPIP()[0] != 0) //is active
    {
      char s[16];
      IPAddress apIP = WiFi.softAPIP();
      sprintf(s, "%d.%d.%d.%d", apIP[0], apIP[1], apIP[2], apIP[3]);
      printSetClassElementHTML(settingsScript,PSTR("sip"),1,s);
    } else
    {
      printSetClassElementHTML(settingsScript,PSTR("sip"),1,(char*)F("Not active"));
    }

    // #ifndef WLED_DISABLE_ESPNOW
    // if (strlen(tkr_anim->last_signal_src) > 0) { //Have seen an ESP-NOW Remote
    //   printSetClassElementHTML(settingsScript,PSTR("rlid"),0,tkr_anim->last_signal_src);
    // } else if (!0){//enableESPNow) {
    //   printSetClassElementHTML(settingsScript,PSTR("rlid"),0,(char*)F("(Enable ESP-NOW to listen)"));
    // } else {
    //   printSetClassElementHTML(settingsScript,PSTR("rlid"),0,(char*)F("None"));
    // }
    // #endif
  }

//   if (subPage == WebSettingsSubPage::LEDS)
//   {
    
// //     #ifdef ENABLE_FEATURE_LIGHTING__XML_REQUESTS__WebSettingsSubPage::LEDS

// //     appendGPIOinfo(settingsScript);

// //     settingsScript.printf_P(PSTR("d.ledTypes=%s;"), BusManager::getLEDTypesJSONString().c_str());

// //     // set limits
// //     settingsScript.printf_P(PSTR("bLimits(%d,%d,%d,%d,%d,%d,%d,%d);"),
// //       WLED_MAX_BUSSES,
// //       WLED_MIN_VIRTUAL_BUSSES,
// //       MAX_LEDS_PER_BUS,
// //       MAX_LED_MEMORY,
// //       MAX_LEDS_NEO,
// //       WLED_MAX_COLOR_ORDER_MAPPINGS,
// //       WLED_MAX_DIGITAL_CHANNELS,
// //       WLED_MAX_ANALOG_CHANNELS
// //     );

// //     printSetFormCheckbox(settingsScript,PSTR("MS"),tkr_anim->autoSegments);
// //     printSetFormCheckbox(settingsScript,PSTR("CCT"),tkr_anim->correctWB);
// //     printSetFormCheckbox(settingsScript,PSTR("IC"),0);//cctICused);
// //     printSetFormCheckbox(settingsScript,PSTR("CR"),tkr_anim->cctFromRgb);
// //     printSetFormValue(settingsScript,PSTR("CB"),tkr_anim->cctBlending);
// //     printSetFormValue(settingsScript,PSTR("FR"),tkr_anim->getTargetFps());
// //     printSetFormValue(settingsScript,PSTR("AW"),Bus::getGlobalAWMode());
// //     printSetFormCheckbox(settingsScript,PSTR("LD"),0);//useGlobalLedBuffer);

// //     unsigned sumMa = 0;
// //     for (int s = 0; s < BusManager::getNumBusses(); s++) {
// //       Bus* bus = BusManager::getBus(s);
// //       if (bus == nullptr) continue;
// //       int offset = s < 10 ? 48 : 55;
// //       char lp[4] = "L0"; lp[2] = offset+s; lp[3] = 0; //ascii 0-9 //strip data pin
// //       char lc[4] = "LC"; lc[2] = offset+s; lc[3] = 0; //strip length
// //       char co[4] = "CO"; co[2] = offset+s; co[3] = 0; //strip color order
// //       char lt[4] = "LT"; lt[2] = offset+s; lt[3] = 0; //strip type
// //       char ls[4] = "LS"; ls[2] = offset+s; ls[3] = 0; //strip start LED
// //       char cv[4] = "CV"; cv[2] = offset+s; cv[3] = 0; //strip reverse
// //       char sl[4] = "SL"; sl[2] = offset+s; sl[3] = 0; //skip 1st LED
// //       char rf[4] = "RF"; rf[2] = offset+s; rf[3] = 0; //off refresh
// //       char aw[4] = "AW"; aw[2] = offset+s; aw[3] = 0; //auto white mode
// //       char wo[4] = "WO"; wo[2] = offset+s; wo[3] = 0; //swap channels
// //       char sp[4] = "SP"; sp[2] = offset+s; sp[3] = 0; //bus clock speed
// //       char la[4] = "LA"; la[2] = offset+s; la[3] = 0; //LED current
// //       char ma[4] = "MA"; ma[2] = offset+s; ma[3] = 0; //max per-port PSU current
// //       settingsScript.print(F("addLEDs(1);"));
// //       uint8_t pins[5];
// //       int nPins = bus->getPins(pins);
// //       for (int i = 0; i < nPins; i++) {
// //         lp[1] = offset+i;
// //         if (1/*PinManager::isPinOk(pins[i])*/ || bus->isVirtual()) printSetFormValue(settingsScript,lp,pins[i]);
// //       }
// //       printSetFormValue(settingsScript,lc,bus->getLength());
// //       printSetFormValue(settingsScript,lt,bus->getType());
// //       printSetFormValue(settingsScript,co,bus->getColorOrder() & 0x0F);
// //       printSetFormValue(settingsScript,ls,bus->getStart());
// //       printSetFormCheckbox(settingsScript,cv,bus->isReversed());
// //       printSetFormValue(settingsScript,sl,bus->skippedLeds());
// //       printSetFormCheckbox(settingsScript,rf,bus->isOffRefreshRequired());
// //       printSetFormValue(settingsScript,aw,bus->getAutoWhiteMode());
// //       printSetFormValue(settingsScript,wo,bus->getColorOrder() >> 4);
// //       unsigned speed = bus->getFrequency();
// //       if (bus->isPWM()) {
// //         switch (speed) {
// //           case WLED_PWM_FREQ/2    : speed = 0; break;
// //           case WLED_PWM_FREQ*2/3  : speed = 1; break;
// //           default:
// //           case WLED_PWM_FREQ      : speed = 2; break;
// //           case WLED_PWM_FREQ*2    : speed = 3; break;
// //           case WLED_PWM_FREQ*10/3 : speed = 4; break; // uint16_t max (19531 * 3.333)
// //         }
// //       } else if (bus->is2Pin()) {
// //         switch (speed) {
// //           case  1000 : speed = 0; break;
// //           case  2000 : speed = 1; break;
// //           default:
// //           case  5000 : speed = 2; break;
// //           case 10000 : speed = 3; break;
// //           case 20000 : speed = 4; break;
// //         }
// //       }
// //       printSetFormValue(settingsScript,sp,speed);
// //       printSetFormValue(settingsScript,la,bus->getLEDCurrent());
// //       printSetFormValue(settingsScript,ma,bus->getMaxCurrent());
// //       sumMa += bus->getMaxCurrent();
// //     }
// //     printSetFormValue(settingsScript,PSTR("MA"),BusManager::ablMilliampsMax() ? BusManager::ablMilliampsMax() : sumMa);
// //     printSetFormCheckbox(settingsScript,PSTR("ABL"),BusManager::ablMilliampsMax() || sumMa > 0);
// //     printSetFormCheckbox(settingsScript,PSTR("PPL"),!BusManager::ablMilliampsMax() && sumMa > 0);

// //     settingsScript.printf_P(PSTR("resetCOM(%d);"), WLED_MAX_COLOR_ORDER_MAPPINGS);
// //     const ColorOrderMap& com = BusManager::getColorOrderMap();
// //     for (int s = 0; s < com.count(); s++) {
// //       const ColorOrderMapEntry* entry = com.get(s);
// //       if (entry == nullptr) break;
// //       settingsScript.printf_P(PSTR("addCOM(%d,%d,%d);"), entry->start, entry->len, entry->colorOrder);
// //     }

// //     printSetFormValue(settingsScript,PSTR("CA"), 127);//briS);

// //     printSetFormCheckbox(settingsScript,PSTR("BO"),turnOnAtBoot);
// //     printSetFormValue(settingsScript,PSTR("BP"),bootPreset);

// //     printSetFormCheckbox(settingsScript,PSTR("GB"),gammaCorrectBri);
// //     printSetFormCheckbox(settingsScript,PSTR("GC"),gammaCorrectCol);
// //     dtostrf(gammaCorrectVal,3,1,nS); printSetFormValue(settingsScript,PSTR("GV"),nS);
// //     printSetFormCheckbox(settingsScript,PSTR("TF"),fadeTransition);
// //     printSetFormCheckbox(settingsScript,PSTR("EB"),0);//modeBlending);
// //     printSetFormValue(settingsScript,PSTR("TD"),0);//,transitionDelayDefault);
// //     printSetFormCheckbox(settingsScript,PSTR("PF"),paletteFade);
// //     printSetFormValue(settingsScript,PSTR("TP"),randomPaletteChangeTime);
// //     printSetFormCheckbox(settingsScript,PSTR("TH"),0);//,useHarmonicRandomPalette);
// //     printSetFormValue(settingsScript,PSTR("BF"),briMultiplier);
// //     printSetFormValue(settingsScript,PSTR("TB"),nightlightTargetBri);
// //     printSetFormValue(settingsScript,PSTR("TL"),nightlightDelayMinsDefault);
// //     printSetFormValue(settingsScript,PSTR("TW"),nightlightMode);
// //     printSetFormIndex(settingsScript,PSTR("PB"),paletteBlend);
// //     printSetFormValue(settingsScript,PSTR("RL"),0);//,rlyPin);
// //     printSetFormCheckbox(settingsScript,PSTR("RM"),0);//,rlyMde);
// //     printSetFormCheckbox(settingsScript,PSTR("RO"),0);//,rlyOpenDrain);
// //     for (int i = 0; i < WLED_MAX_BUTTONS; i++) {
// //       settingsScript.printf_P(PSTR("addBtn(%d,%d,%d);"), i, 0,0);//btnPin[i], buttonType[i]);
// //     }
// //     printSetFormCheckbox(settingsScript,PSTR("IP"),disablePullUp);
// //     printSetFormValue(settingsScript,PSTR("TT"),touchThreshold);
// // #ifndef WLED_DISABLE_INFRARED
// //     printSetFormValue(settingsScript,PSTR("IR"),0);//,irPin);
// //     printSetFormValue(settingsScript,PSTR("IT"),irEnabled);
// // #endif    
// //     printSetFormCheckbox(settingsScript,PSTR("MSO"),!irApplyToAllSelected);

// //     #endif // ENABLE_FEATURE_LIGHTING__XML_REQUESTS__WebSettingsSubPage::LEDS
//   }

  // if (subPage == WebSettingsSubPage::UI)
  // {
  //   // printSetFormValue(settingsScript,PSTR("DS"),serverDescription);
  //   // printSetFormCheckbox(settingsScript,PSTR("SU"),simplifiedUI);
  // }

  // if (subPage == WebSettingsSubPage::SYNC)
  // {
  // //   #ifdef ENABLE_FEATURE_LIGHTING__XML_REQUESTS__WebSettingsSubPage::SYNC

  // //   printSetFormValue(settingsScript,PSTR("UP"),tkr_anim->udpPort);
  // //   printSetFormValue(settingsScript,PSTR("U2"),udpPort2);
  // // #ifndef WLED_DISABLE_ESPNOW
  // //   // if (enableESPNow) printSetFormCheckbox(settingsScript,PSTR("EN"),useESPNowSync);
  // //   // else              
  // //   settingsScript.print(F("toggle('ESPNOW');"));  // hide ESP-NOW setting
  // // #else
  // //   settingsScript.print(F("toggle('ESPNOW');"));  // hide ESP-NOW setting
  // // #endif
  // //   printSetFormValue(settingsScript,PSTR("GS"),syncGroups);
  // //   printSetFormValue(settingsScript,PSTR("GR"),receiveGroups);

  // //   printSetFormCheckbox(settingsScript,PSTR("RB"),receiveNotificationBrightness);
  // //   printSetFormCheckbox(settingsScript,PSTR("RC"),receiveNotificationColor);
  // //   printSetFormCheckbox(settingsScript,PSTR("RX"),receiveNotificationEffects);
  // //   printSetFormCheckbox(settingsScript,PSTR("RP"),0);//receiveNotificationPalette);
  // //   printSetFormCheckbox(settingsScript,PSTR("SO"),receiveSegmentOptions);
  // //   printSetFormCheckbox(settingsScript,PSTR("SG"),receiveSegmentBounds);
  // //   printSetFormCheckbox(settingsScript,PSTR("SS"),0);//,sendNotifications);
  // //   printSetFormCheckbox(settingsScript,PSTR("SD"),notifyDirect);
  // //   printSetFormCheckbox(settingsScript,PSTR("SB"),notifyButton);
  // //   printSetFormCheckbox(settingsScript,PSTR("SH"),notifyHue);
  // //   printSetFormValue(settingsScript,PSTR("UR"),udpNumRetries);

  // //   printSetFormCheckbox(settingsScript,PSTR("NL"),nodeListEnabled);
  // //   printSetFormCheckbox(settingsScript,PSTR("NB"),nodeBroadcastEnabled);

  // //   printSetFormCheckbox(settingsScript,PSTR("RD"),receiveDirect);
  // //   printSetFormCheckbox(settingsScript,PSTR("MO"),useMainSegmentOnly);
  // //   printSetFormCheckbox(settingsScript,PSTR("RLM"),realtimeRespectLedMaps);
  // //   printSetFormValue(settingsScript,PSTR("EP"),e131Port);
  // //   printSetFormCheckbox(settingsScript,PSTR("ES"),e131SkipOutOfSequence);
  // //   printSetFormCheckbox(settingsScript,PSTR("EM"),e131Multicast);
  // //   printSetFormValue(settingsScript,PSTR("EU"),e131Universe);
  // //   printSetFormValue(settingsScript,PSTR("DA"),DMXAddress);
  // //   printSetFormValue(settingsScript,PSTR("XX"),DMXSegmentSpacing);
  // //   printSetFormValue(settingsScript,PSTR("PY"),0);//,e131Priority);
  // //   printSetFormValue(settingsScript,PSTR("DM"),DMXMode);
  // //   printSetFormValue(settingsScript,PSTR("ET"),realtimeTimeoutMs);
  // //   printSetFormCheckbox(settingsScript,PSTR("FB"),arlsForceMaxBri);
  // //   printSetFormCheckbox(settingsScript,PSTR("RG"),arlsDisableGammaCorrection);
  // //   printSetFormValue(settingsScript,PSTR("WO"),arlsOffset);
  // //   #ifndef WLED_DISABLE_ALEXA
  // //   printSetFormCheckbox(settingsScript,PSTR("AL"),0);//,alexaEnabled);
  // //   printSetFormValue(settingsScript,PSTR("AI"),0);//,alexaInvocationName);
  // //   printSetFormCheckbox(settingsScript,PSTR("SA"),notifyAlexa);
  // //   printSetFormValue(settingsScript,PSTR("AP"),0);//,alexaNumPresets);
  // //   #else
  // //   settingsScript.print(F("toggle('Alexa');"));  // hide Alexa settings
  // //   #endif

  // //   #ifndef WLED_DISABLE_MQTT
  // //   printSetFormCheckbox(settingsScript,PSTR("MQ"),0);//,mqttEnabled);
  // //   printSetFormValue(settingsScript,PSTR("MS"),0);//,mqttServer);
  // //   printSetFormValue(settingsScript,PSTR("MQPORT"),0);//,mqttPort);
  // //   printSetFormValue(settingsScript,PSTR("MQUSER"),0);//,mqttUser);
  // //   byte l = strlen("mqttPass");
  // //   char fpass[l+1]; //fill password field with ***
  // //   fpass[l] = 0;
  // //   memset(fpass,'*',l);
  // //   printSetFormValue(settingsScript,PSTR("MQPASS"),"fpass");
  // //   printSetFormValue(settingsScript,PSTR("MQCID"),"mqttClientID");
  // //   printSetFormValue(settingsScript,PSTR("MD"),"mqttDeviceTopic");
  // //   printSetFormValue(settingsScript,PSTR("MG"),"mqttGroupTopic");
  // //   printSetFormCheckbox(settingsScript,PSTR("BM"),0);//buttonPublishMqtt);
  // //   printSetFormCheckbox(settingsScript,PSTR("RT"),0);//,retainMqttMsg);
  // //   // settingsScript.printf_P(PSTR("d.Sf.MD.maxLength=%d;d.Sf.MG.maxLength=%d;d.Sf.MS.maxLength=%d;"),    MQTT_MAX_TOPIC_LEN, MQTT_MAX_TOPIC_LEN, MQTT_MAX_SERVER_LEN);
  // //   #else
  // //   settingsScript.print(F("toggle('MQTT');"));    // hide MQTT settings
  // //   #endif

  // //   #ifndef WLED_DISABLE_HUESYNC
  // //   printSetFormValue(settingsScript,PSTR("H0"),hueIP[0]);
  // //   printSetFormValue(settingsScript,PSTR("H1"),hueIP[1]);
  // //   printSetFormValue(settingsScript,PSTR("H2"),hueIP[2]);
  // //   printSetFormValue(settingsScript,PSTR("H3"),hueIP[3]);
  // //   printSetFormValue(settingsScript,PSTR("HL"),huePollLightId);
  // //   printSetFormValue(settingsScript,PSTR("HI"),huePollIntervalMs);
  // //   printSetFormCheckbox(settingsScript,PSTR("HP"),huePollingEnabled);
  // //   printSetFormCheckbox(settingsScript,PSTR("HO"),hueApplyOnOff);
  // //   printSetFormCheckbox(settingsScript,PSTR("HB"),hueApplyBri);
  // //   printSetFormCheckbox(settingsScript,PSTR("HC"),hueApplyColor);
  // //   char hueErrorString[25];
  // //   switch (hueError)
  // //   {
  // //     case HUE_ERROR_INACTIVE     : strcpy_P(hueErrorString,PSTR("Inactive"));                break;
  // //     case HUE_ERROR_ACTIVE       : strcpy_P(hueErrorString,PSTR("Active"));                  break;
  // //     case HUE_ERROR_UNAUTHORIZED : strcpy_P(hueErrorString,PSTR("Unauthorized"));            break;
  // //     case HUE_ERROR_LIGHTID      : strcpy_P(hueErrorString,PSTR("Invalid light ID"));        break;
  // //     case HUE_ERROR_PUSHLINK     : strcpy_P(hueErrorString,PSTR("Link button not pressed")); break;
  // //     case HUE_ERROR_JSON_PARSING : strcpy_P(hueErrorString,PSTR("JSON parsing error"));      break;
  // //     case HUE_ERROR_TIMEOUT      : strcpy_P(hueErrorString,PSTR("Timeout"));                 break;
  // //     default: sprintf_P(hueErrorString,PSTR("Bridge Error %i"),hueError);
  // //   }

  // //   printSetClassElementHTML(settingsScript,PSTR("sip"),0,hueErrorString);
  // //   #else
  // //   settingsScript.print(F("toggle('Hue');"));    // hide Hue Sync settings
  // //   #endif
  // //   printSetFormValue(settingsScript,PSTR("BD"),serialBaud);
  // //   #ifndef WLED_ENABLE_ADALIGHT
  // //   settingsScript.print(F("toggle('Serial');"));
  // //   #endif
  // }

  if (subPage == WebSettingsSubPage::TIME)
  {
    // printSetFormCheckbox(settingsScript,PSTR("NT"),ntpEnabled);
    // printSetFormValue(settingsScript,PSTR("NS"),ntpServerName);
    // printSetFormCheckbox(settingsScript,PSTR("CF"),!useAMPM);
    // printSetFormIndex(settingsScript,PSTR("TZ"),currentTimezone);
    // printSetFormValue(settingsScript,PSTR("UO"),utcOffsetSecs);
    // char tm[32];
    // dtostrf(longitude,4,2,tm);
    // printSetFormValue(settingsScript,PSTR("LN"),tm);
    // dtostrf(latitude,4,2,tm);
    // printSetFormValue(settingsScript,PSTR("LT"),tm);
    // // getTimeString(tm);
    // snprintf(tm, sizeof(tm), "%s", tkr_time->GetDateAndTime(DT_UTC).c_str());
    // printSetClassElementHTML(settingsScript,PSTR("times"),0,tm);
    // if ((int)(longitude*10.0f) || (int)(latitude*10.0f)) {
    //   sprintf_P(tm, PSTR("Sunrise: %02d:%02d Sunset: %02d:%02d"), tkr_time->hour(sunrise), tkr_time->minute(sunrise), tkr_time->hour(sunset), tkr_time->minute(sunset));
    //   printSetClassElementHTML(settingsScript,PSTR("times"),1,tm);
    // }
    // printSetFormCheckbox(settingsScript,PSTR("OL"),overlayCurrent);
    // printSetFormValue(settingsScript,PSTR("O1"),overlayMin);
    // printSetFormValue(settingsScript,PSTR("O2"),overlayMax);
    // printSetFormValue(settingsScript,PSTR("OM"),analogClock12pixel);
    // printSetFormCheckbox(settingsScript,PSTR("OS"),analogClockSecondsTrail);
    // printSetFormCheckbox(settingsScript,PSTR("O5"),analogClock5MinuteMarks);
    // printSetFormCheckbox(settingsScript,PSTR("OB"),analogClockSolidBlack);

    // printSetFormCheckbox(settingsScript,PSTR("CE"),countdownMode);
    // printSetFormValue(settingsScript,PSTR("CY"),countdownYear);
    // printSetFormValue(settingsScript,PSTR("CI"),countdownMonth);
    // printSetFormValue(settingsScript,PSTR("CD"),countdownDay);
    // printSetFormValue(settingsScript,PSTR("CH"),countdownHour);
    // printSetFormValue(settingsScript,PSTR("CM"),countdownMin);
    // printSetFormValue(settingsScript,PSTR("CS"),countdownSec);

    // printSetFormValue(settingsScript,PSTR("A0"),macroAlexaOn);
    // printSetFormValue(settingsScript,PSTR("A1"),macroAlexaOff);
    // printSetFormValue(settingsScript,PSTR("MC"),macroCountdown);
    // printSetFormValue(settingsScript,PSTR("MN"),macroNl);
    // for (unsigned i=0; i<WLED_MAX_BUTTONS; i++) {
    //   settingsScript.printf_P(PSTR("addRow(%d,%d,%d,%d);"), i, macroButton[i], macroLongPress[i], macroDoublePress[i]);
    // }

    // char k[4];
    // k[2] = 0; //Time macros
    // for (int i = 0; i<10; i++)
    // {
    //   k[1] = 48+i; //ascii 0,1,2,3
    //   if (i<8) { k[0] = 'H'; printSetFormValue(settingsScript,k,timerHours[i]); }
    //   k[0] = 'N'; printSetFormValue(settingsScript,k,timerMinutes[i]);
    //   k[0] = 'T'; printSetFormValue(settingsScript,k,timerMacro[i]);
    //   k[0] = 'W'; printSetFormValue(settingsScript,k,timerWeekday[i]);
    //   if (i<8) {
    //     k[0] = 'M'; printSetFormValue(settingsScript,k,(timerMonth[i] >> 4) & 0x0F);
		// 		k[0] = 'P'; printSetFormValue(settingsScript,k,timerMonth[i] & 0x0F);
    //     k[0] = 'D'; printSetFormValue(settingsScript,k,timerDay[i]);
		// 		k[0] = 'E'; printSetFormValue(settingsScript,k,timerDayEnd[i]);
    //   }
    // }

    // #endif // ENABLE_FEATURE_LIGHTING__XML_REQUESTS__WebSettingsSubPage::SYNC
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

  // if (subPage == WebSettingsSubPage::UM) //usermods
  // {
  //   appendGPIOinfo(settingsScript);
  //   // settingsScript.printf_P(PSTR("numM=%d;"), UsermodManager::getModCount());
  //   // printSetFormValue(settingsScript,PSTR("SDA"),i2c_sda);
  //   // printSetFormValue(settingsScript,PSTR("SCL"),i2c_scl);
  //   // printSetFormValue(settingsScript,PSTR("MOSI"),spi_mosi);
  //   // printSetFormValue(settingsScript,PSTR("MISO"),spi_miso);
  //   // printSetFormValue(settingsScript,PSTR("SCLK"),spi_sclk);
  //   // settingsScript.printf_P(PSTR("addInfo('SDA','%d');"
  //   //              "addInfo('SCL','%d');"
  //   //              "addInfo('MOSI','%d');"
  //   //              "addInfo('MISO','%d');"
  //   //              "addInfo('SCLK','%d');"),
  //   //   HW_PIN_SDA, HW_PIN_SCL, HW_PIN_DATASPI, HW_PIN_MISOSPI, HW_PIN_CLOCKSPI
  //   // );
  //   // UsermodManager::appendConfigData(settingsScript);
  // }

  if (subPage == WebSettingsSubPage::UPDATE) // update
  {
    // char tmp_buf[128];
    // snprintf_P(tmp_buf,sizeof(tmp_buf),PSTR("WLED %s<br>%s<br>(%s build %d)"),
    //   versionString,
    //   releaseString,
    // #if defined(ARDUINO_ARCH_ESP32)
    //   ESP.getChipModel(),
    // #else
    //   "esp8266",
    // #endif
    //   PROJECT_VERSION);

    // printSetClassElementHTML(settingsScript,PSTR("sip"),0,tmp_buf);
  }

  // if (subPage == WebSettingsSubPage::2D) // 2D matrices
  // {
  //   // printSetFormValue(settingsScript,PSTR("SOMP"),isMatrix);
  //   // #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  //   // settingsScript.printf_P(PSTR("maxPanels=%d;resetPanels();"),WLED_MAX_PANELS);
  //   // if (isMatrix) {
  //   //   if(panels>0){
  //   //     printSetFormValue(settingsScript,PSTR("PW"),panel[0].width); //Set generator Width and Height to first panel size for convenience
  //   //     printSetFormValue(settingsScript,PSTR("PH"),panel[0].height);
  //   //   }
  //   //   printSetFormValue(settingsScript,PSTR("MPC"),panels);
  //   //   // panels
  //   //   for (unsigned i=0; i<panels; i++) {
  //   //     settingsScript.printf_P(PSTR("addPanel(%d);"), i);
  //   //     char pO[8] = { '\0' };
  //   //     snprintf_P(pO, 7, PSTR("P%d"), i);       // WLED_MAX_PANELS is 18 so pO will always only be 4 characters or less
  //   //     pO[7] = '\0';
  //   //     unsigned l = strlen(pO);
  //   //     // create P0B, P1B, ..., P63B, etc for other PxxX
  //   //     pO[l] = 'B'; printSetFormValue(settingsScript,pO,panel[i].bottomStart);
  //   //     pO[l] = 'R'; printSetFormValue(settingsScript,pO,panel[i].rightStart);
  //   //     pO[l] = 'V'; printSetFormValue(settingsScript,pO,panel[i].vertical);
  //   //     pO[l] = 'S'; printSetFormCheckbox(settingsScript,pO,panel[i].serpentine);
  //   //     pO[l] = 'X'; printSetFormValue(settingsScript,pO,panel[i].xOffset);
  //   //     pO[l] = 'Y'; printSetFormValue(settingsScript,pO,panel[i].yOffset);
  //   //     pO[l] = 'W'; printSetFormValue(settingsScript,pO,panel[i].width);
  //   //     pO[l] = 'H'; printSetFormValue(settingsScript,pO,panel[i].height);
  //   //   }
  //   // }
  //   // #else
  //   // settingsScript.print(F("gId(\"somp\").remove(1);")); // remove 2D option from dropdown
  //   // #endif
  // }
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

  // AsyncJsonResponse *response = new AsyncJsonResponse(tkr_mfile->pDoc, subJson==JSON_PATH_FXDATA || subJson==JSON_PATH_EFFECTS); // will clear and convert JsonDocument into JsonArray if necessary

  // releaseJSONBufferLock() will be called when "response" is destroyed (from AsyncWebServer)
  // make sure you delete "response" if no "request->send(response);" is made
  LockedJsonResponse2 *response = new LockedJsonResponse2(tkr_mfile->pDoc, subJson==JSON_PATH_WEB_FXDATA || subJson==JSON_PATH_WEB_EFFECTS); // will clear and convert JsonDocument into JsonArray if necessary



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
    // case WebSettingsSubPage::CSS:      content = PAGE_settingsCss_web; len = PAGE_settingsCss_web_length; contentType = FPSTR(CONTENT_TYPE_CSS); break;
    case WebSettingsSubPage::JS:       serveSettingsJS(request); return;
    case WebSettingsSubPage::WELCOME:  content = PAGE_welcome_web; len = PAGE_welcome_web_length; break;

    case WebSettingsSubPage::MENU:
    default: break;
  }

  handleStaticContent(request, "", code, contentType, content, len);
}

#endif //   #ifdef USE_MODULE_NETWORK_WEBSERVER21